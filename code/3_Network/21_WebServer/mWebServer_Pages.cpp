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
size_t mWebServer::printSetElementStyle(
  Print& settingsScript,
  const char* element_id,
  const char* css_prop,
  const char* css_val
) {
  return settingsScript.printf_P(
    PSTR("var e=d.getElementById(\"%s\");if(e)e.style.%s=\"%s\";"),
    element_id, css_prop, css_val
  );
}

size_t mWebServer::printToggleElementClass(
  Print& settingsScript,
  const char* element_id,
  const char* class_name,
  bool enable
) {
  return settingsScript.printf_P(
    PSTR(
      "var e=d.getElementById(\"%s\");"
      "if(e){e.classList.%s(\"%s\");}"
    ),
    element_id,
    enable ? "add" : "remove",
    class_name
  );
}

// Date Modified: 25Jan26

size_t mWebServer::WebUI_Print_SectionBegin(Print& out, const char* title)
{
  size_t n = 0;
  n += out.print(F("<div class=\"section\"><div class=\"title\">"));
  if (title) n += out.print(title);
  n += out.print(F("</div>"));
  return n;
}

size_t mWebServer::WebUI_Print_SectionEnd(Print& out)
{
  return out.print(F("</div>"));
}

size_t mWebServer::WebUI_Print_TableBegin(Print& out)
{
  return out.print(F("<table class=\"kv\">"));
}

size_t mWebServer::WebUI_Print_TableEnd(Print& out)
{
  return out.print(F("</table>"));
}

size_t mWebServer::WebUI_Print_KV_Float(Print& out, const char* key, float value, uint8_t decimals, const char* units)
{
  size_t n = 0;
  n += out.print(F("<tr><td class=\"key\">"));
  if (key) n += out.print(key);
  n += out.print(F("</td><td class=\"val\">"));
  n += out.print(value, decimals);
  if (units) { n += out.print(' '); n += out.print(units); }
  n += out.print(F("</td></tr>"));
  return n;
}

size_t mWebServer::WebUI_Print_KV_U32(Print& out, const char* key, uint32_t value, const char* units)
{
  size_t n = 0;
  n += out.print(F("<tr><td class=\"key\">"));
  if (key) n += out.print(key);
  n += out.print(F("</td><td class=\"val\">"));
  n += out.print(value);
  if (units) { n += out.print(' '); n += out.print(units); }
  n += out.print(F("</td></tr>"));
  return n;
}

size_t mWebServer::WebUI_Print_KV_Str(Print& out, const char* key, const char* value)
{
  size_t n = 0;
  n += out.print(F("<tr><td class=\"key\">"));
  if (key) n += out.print(key);
  n += out.print(F("</td><td class=\"val\">"));
  if (value) n += out.print(value);
  n += out.print(F("</td></tr>"));
  return n;
}
// Date Modified: 25Jan26

size_t mWebServer::WebUI_Print_TableHeaderRow_Begin(Print& out){
  return out.print(F("<tr>"));
}

size_t mWebServer::WebUI_Print_TableHeaderCell(Print& out, const char* text, bool is_first_blank){
  size_t n = 0;
  if (is_first_blank) {
    n += out.print(F("<th class=\"key\"></th>"));
    return n;
  }
  n += out.print(F("<th class=\"col\">"));
  if (text) n += out.print(text);
  n += out.print(F("</th>"));
  return n;
}

size_t mWebServer::WebUI_Print_TableHeaderRow_End(Print& out){
  return out.print(F("</tr>"));
}

size_t mWebServer::WebUI_Print_RowBegin(Print& out, const char* key){
  size_t n = 0;
  n += out.print(F("<tr><td class=\"key\">"));
  if (key) n += out.print(key);
  n += out.print(F("</td>"));
  return n;
}

size_t mWebServer::WebUI_Print_CellText(Print& out, const char* text){
  size_t n = 0;
  n += out.print(F("<td class=\"val\">"));
  if (text) n += out.print(text);
  n += out.print(F("</td>"));
  return n;
}

size_t mWebServer::WebUI_Print_CellFloat(Print& out, float value, uint8_t decimals, const char* units){
  size_t n = 0;
  n += out.print(F("<td class=\"val\">"));
  n += out.print(value, decimals);
  if (units) { n += out.print(' '); n += out.print(units); }
  n += out.print(F("</td>"));
  return n;
}

size_t mWebServer::WebUI_Print_CellDash(Print& out){
  return out.print(F("<td class=\"val\">—</td>"));
}

size_t mWebServer::WebUI_Print_RowEnd(Print& out){
  return out.print(F("</tr>"));
}





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

  if (subPage == SUBPAGE_WEB_MENU)
  {
  #ifndef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS // include only if 2D is not compiled in
    settingsScript.print(F("gId('2dbtn').style.display='none';"));
  #endif
  #ifdef ENABLE_FEATURE_LIGHTING__DMX // include only if DMX is enabled
    settingsScript.print(F("gId('dmxbtn').style.display='';"));
  #endif
  }

  if (subPage == SUBPAGE_WEB_WIFI)
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

  if (subPage == SUBPAGE_WEB_LEDS)
  {
    
//     #ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_WEB_LEDS

//     appendGPIOinfo(settingsScript);

//     settingsScript.printf_P(PSTR("d.ledTypes=%s;"), BusManager::getLEDTypesJSONString().c_str());

//     // set limits
//     settingsScript.printf_P(PSTR("bLimits(%d,%d,%d,%d,%d,%d,%d,%d);"),
//       WLED_MAX_BUSSES,
//       WLED_MIN_VIRTUAL_BUSSES,
//       MAX_LEDS_PER_BUS,
//       MAX_LED_MEMORY,
//       MAX_LEDS_NEO,
//       WLED_MAX_COLOR_ORDER_MAPPINGS,
//       WLED_MAX_DIGITAL_CHANNELS,
//       WLED_MAX_ANALOG_CHANNELS
//     );

//     printSetFormCheckbox(settingsScript,PSTR("MS"),tkr_anim->autoSegments);
//     printSetFormCheckbox(settingsScript,PSTR("CCT"),tkr_anim->correctWB);
//     printSetFormCheckbox(settingsScript,PSTR("IC"),0);//cctICused);
//     printSetFormCheckbox(settingsScript,PSTR("CR"),tkr_anim->cctFromRgb);
//     printSetFormValue(settingsScript,PSTR("CB"),tkr_anim->cctBlending);
//     printSetFormValue(settingsScript,PSTR("FR"),tkr_anim->getTargetFps());
//     printSetFormValue(settingsScript,PSTR("AW"),Bus::getGlobalAWMode());
//     printSetFormCheckbox(settingsScript,PSTR("LD"),0);//useGlobalLedBuffer);

//     unsigned sumMa = 0;
//     for (int s = 0; s < BusManager::getNumBusses(); s++) {
//       Bus* bus = BusManager::getBus(s);
//       if (bus == nullptr) continue;
//       int offset = s < 10 ? 48 : 55;
//       char lp[4] = "L0"; lp[2] = offset+s; lp[3] = 0; //ascii 0-9 //strip data pin
//       char lc[4] = "LC"; lc[2] = offset+s; lc[3] = 0; //strip length
//       char co[4] = "CO"; co[2] = offset+s; co[3] = 0; //strip color order
//       char lt[4] = "LT"; lt[2] = offset+s; lt[3] = 0; //strip type
//       char ls[4] = "LS"; ls[2] = offset+s; ls[3] = 0; //strip start LED
//       char cv[4] = "CV"; cv[2] = offset+s; cv[3] = 0; //strip reverse
//       char sl[4] = "SL"; sl[2] = offset+s; sl[3] = 0; //skip 1st LED
//       char rf[4] = "RF"; rf[2] = offset+s; rf[3] = 0; //off refresh
//       char aw[4] = "AW"; aw[2] = offset+s; aw[3] = 0; //auto white mode
//       char wo[4] = "WO"; wo[2] = offset+s; wo[3] = 0; //swap channels
//       char sp[4] = "SP"; sp[2] = offset+s; sp[3] = 0; //bus clock speed
//       char la[4] = "LA"; la[2] = offset+s; la[3] = 0; //LED current
//       char ma[4] = "MA"; ma[2] = offset+s; ma[3] = 0; //max per-port PSU current
//       settingsScript.print(F("addLEDs(1);"));
//       uint8_t pins[5];
//       int nPins = bus->getPins(pins);
//       for (int i = 0; i < nPins; i++) {
//         lp[1] = offset+i;
//         if (1/*PinManager::isPinOk(pins[i])*/ || bus->isVirtual()) printSetFormValue(settingsScript,lp,pins[i]);
//       }
//       printSetFormValue(settingsScript,lc,bus->getLength());
//       printSetFormValue(settingsScript,lt,bus->getType());
//       printSetFormValue(settingsScript,co,bus->getColorOrder() & 0x0F);
//       printSetFormValue(settingsScript,ls,bus->getStart());
//       printSetFormCheckbox(settingsScript,cv,bus->isReversed());
//       printSetFormValue(settingsScript,sl,bus->skippedLeds());
//       printSetFormCheckbox(settingsScript,rf,bus->isOffRefreshRequired());
//       printSetFormValue(settingsScript,aw,bus->getAutoWhiteMode());
//       printSetFormValue(settingsScript,wo,bus->getColorOrder() >> 4);
//       unsigned speed = bus->getFrequency();
//       if (bus->isPWM()) {
//         switch (speed) {
//           case WLED_PWM_FREQ/2    : speed = 0; break;
//           case WLED_PWM_FREQ*2/3  : speed = 1; break;
//           default:
//           case WLED_PWM_FREQ      : speed = 2; break;
//           case WLED_PWM_FREQ*2    : speed = 3; break;
//           case WLED_PWM_FREQ*10/3 : speed = 4; break; // uint16_t max (19531 * 3.333)
//         }
//       } else if (bus->is2Pin()) {
//         switch (speed) {
//           case  1000 : speed = 0; break;
//           case  2000 : speed = 1; break;
//           default:
//           case  5000 : speed = 2; break;
//           case 10000 : speed = 3; break;
//           case 20000 : speed = 4; break;
//         }
//       }
//       printSetFormValue(settingsScript,sp,speed);
//       printSetFormValue(settingsScript,la,bus->getLEDCurrent());
//       printSetFormValue(settingsScript,ma,bus->getMaxCurrent());
//       sumMa += bus->getMaxCurrent();
//     }
//     printSetFormValue(settingsScript,PSTR("MA"),BusManager::ablMilliampsMax() ? BusManager::ablMilliampsMax() : sumMa);
//     printSetFormCheckbox(settingsScript,PSTR("ABL"),BusManager::ablMilliampsMax() || sumMa > 0);
//     printSetFormCheckbox(settingsScript,PSTR("PPL"),!BusManager::ablMilliampsMax() && sumMa > 0);

//     settingsScript.printf_P(PSTR("resetCOM(%d);"), WLED_MAX_COLOR_ORDER_MAPPINGS);
//     const ColorOrderMap& com = BusManager::getColorOrderMap();
//     for (int s = 0; s < com.count(); s++) {
//       const ColorOrderMapEntry* entry = com.get(s);
//       if (entry == nullptr) break;
//       settingsScript.printf_P(PSTR("addCOM(%d,%d,%d);"), entry->start, entry->len, entry->colorOrder);
//     }

//     printSetFormValue(settingsScript,PSTR("CA"), 127);//briS);

//     printSetFormCheckbox(settingsScript,PSTR("BO"),turnOnAtBoot);
//     printSetFormValue(settingsScript,PSTR("BP"),bootPreset);

//     printSetFormCheckbox(settingsScript,PSTR("GB"),gammaCorrectBri);
//     printSetFormCheckbox(settingsScript,PSTR("GC"),gammaCorrectCol);
//     dtostrf(gammaCorrectVal,3,1,nS); printSetFormValue(settingsScript,PSTR("GV"),nS);
//     printSetFormCheckbox(settingsScript,PSTR("TF"),fadeTransition);
//     printSetFormCheckbox(settingsScript,PSTR("EB"),0);//modeBlending);
//     printSetFormValue(settingsScript,PSTR("TD"),0);//,transitionDelayDefault);
//     printSetFormCheckbox(settingsScript,PSTR("PF"),paletteFade);
//     printSetFormValue(settingsScript,PSTR("TP"),randomPaletteChangeTime);
//     printSetFormCheckbox(settingsScript,PSTR("TH"),0);//,useHarmonicRandomPalette);
//     printSetFormValue(settingsScript,PSTR("BF"),briMultiplier);
//     printSetFormValue(settingsScript,PSTR("TB"),nightlightTargetBri);
//     printSetFormValue(settingsScript,PSTR("TL"),nightlightDelayMinsDefault);
//     printSetFormValue(settingsScript,PSTR("TW"),nightlightMode);
//     printSetFormIndex(settingsScript,PSTR("PB"),paletteBlend);
//     printSetFormValue(settingsScript,PSTR("RL"),0);//,rlyPin);
//     printSetFormCheckbox(settingsScript,PSTR("RM"),0);//,rlyMde);
//     printSetFormCheckbox(settingsScript,PSTR("RO"),0);//,rlyOpenDrain);
//     for (int i = 0; i < WLED_MAX_BUTTONS; i++) {
//       settingsScript.printf_P(PSTR("addBtn(%d,%d,%d);"), i, 0,0);//btnPin[i], buttonType[i]);
//     }
//     printSetFormCheckbox(settingsScript,PSTR("IP"),disablePullUp);
//     printSetFormValue(settingsScript,PSTR("TT"),touchThreshold);
// #ifndef WLED_DISABLE_INFRARED
//     printSetFormValue(settingsScript,PSTR("IR"),0);//,irPin);
//     printSetFormValue(settingsScript,PSTR("IT"),irEnabled);
// #endif    
//     printSetFormCheckbox(settingsScript,PSTR("MSO"),!irApplyToAllSelected);

//     #endif // ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_WEB_LEDS
  }

  if (subPage == SUBPAGE_WEB_UI)
  {
    // printSetFormValue(settingsScript,PSTR("DS"),serverDescription);
    // printSetFormCheckbox(settingsScript,PSTR("SU"),simplifiedUI);
  }

  if (subPage == SUBPAGE_WEB_SYNC)
  {
  //   #ifdef ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_WEB_SYNC

  //   printSetFormValue(settingsScript,PSTR("UP"),tkr_anim->udpPort);
  //   printSetFormValue(settingsScript,PSTR("U2"),udpPort2);
  // #ifndef WLED_DISABLE_ESPNOW
  //   // if (enableESPNow) printSetFormCheckbox(settingsScript,PSTR("EN"),useESPNowSync);
  //   // else              
  //   settingsScript.print(F("toggle('ESPNOW');"));  // hide ESP-NOW setting
  // #else
  //   settingsScript.print(F("toggle('ESPNOW');"));  // hide ESP-NOW setting
  // #endif
  //   printSetFormValue(settingsScript,PSTR("GS"),syncGroups);
  //   printSetFormValue(settingsScript,PSTR("GR"),receiveGroups);

  //   printSetFormCheckbox(settingsScript,PSTR("RB"),receiveNotificationBrightness);
  //   printSetFormCheckbox(settingsScript,PSTR("RC"),receiveNotificationColor);
  //   printSetFormCheckbox(settingsScript,PSTR("RX"),receiveNotificationEffects);
  //   printSetFormCheckbox(settingsScript,PSTR("RP"),0);//receiveNotificationPalette);
  //   printSetFormCheckbox(settingsScript,PSTR("SO"),receiveSegmentOptions);
  //   printSetFormCheckbox(settingsScript,PSTR("SG"),receiveSegmentBounds);
  //   printSetFormCheckbox(settingsScript,PSTR("SS"),0);//,sendNotifications);
  //   printSetFormCheckbox(settingsScript,PSTR("SD"),notifyDirect);
  //   printSetFormCheckbox(settingsScript,PSTR("SB"),notifyButton);
  //   printSetFormCheckbox(settingsScript,PSTR("SH"),notifyHue);
  //   printSetFormValue(settingsScript,PSTR("UR"),udpNumRetries);

  //   printSetFormCheckbox(settingsScript,PSTR("NL"),nodeListEnabled);
  //   printSetFormCheckbox(settingsScript,PSTR("NB"),nodeBroadcastEnabled);

  //   printSetFormCheckbox(settingsScript,PSTR("RD"),receiveDirect);
  //   printSetFormCheckbox(settingsScript,PSTR("MO"),useMainSegmentOnly);
  //   printSetFormCheckbox(settingsScript,PSTR("RLM"),realtimeRespectLedMaps);
  //   printSetFormValue(settingsScript,PSTR("EP"),e131Port);
  //   printSetFormCheckbox(settingsScript,PSTR("ES"),e131SkipOutOfSequence);
  //   printSetFormCheckbox(settingsScript,PSTR("EM"),e131Multicast);
  //   printSetFormValue(settingsScript,PSTR("EU"),e131Universe);
  //   printSetFormValue(settingsScript,PSTR("DA"),DMXAddress);
  //   printSetFormValue(settingsScript,PSTR("XX"),DMXSegmentSpacing);
  //   printSetFormValue(settingsScript,PSTR("PY"),0);//,e131Priority);
  //   printSetFormValue(settingsScript,PSTR("DM"),DMXMode);
  //   printSetFormValue(settingsScript,PSTR("ET"),realtimeTimeoutMs);
  //   printSetFormCheckbox(settingsScript,PSTR("FB"),arlsForceMaxBri);
  //   printSetFormCheckbox(settingsScript,PSTR("RG"),arlsDisableGammaCorrection);
  //   printSetFormValue(settingsScript,PSTR("WO"),arlsOffset);
  //   #ifndef WLED_DISABLE_ALEXA
  //   printSetFormCheckbox(settingsScript,PSTR("AL"),0);//,alexaEnabled);
  //   printSetFormValue(settingsScript,PSTR("AI"),0);//,alexaInvocationName);
  //   printSetFormCheckbox(settingsScript,PSTR("SA"),notifyAlexa);
  //   printSetFormValue(settingsScript,PSTR("AP"),0);//,alexaNumPresets);
  //   #else
  //   settingsScript.print(F("toggle('Alexa');"));  // hide Alexa settings
  //   #endif

  //   #ifndef WLED_DISABLE_MQTT
  //   printSetFormCheckbox(settingsScript,PSTR("MQ"),0);//,mqttEnabled);
  //   printSetFormValue(settingsScript,PSTR("MS"),0);//,mqttServer);
  //   printSetFormValue(settingsScript,PSTR("MQPORT"),0);//,mqttPort);
  //   printSetFormValue(settingsScript,PSTR("MQUSER"),0);//,mqttUser);
  //   byte l = strlen("mqttPass");
  //   char fpass[l+1]; //fill password field with ***
  //   fpass[l] = 0;
  //   memset(fpass,'*',l);
  //   printSetFormValue(settingsScript,PSTR("MQPASS"),"fpass");
  //   printSetFormValue(settingsScript,PSTR("MQCID"),"mqttClientID");
  //   printSetFormValue(settingsScript,PSTR("MD"),"mqttDeviceTopic");
  //   printSetFormValue(settingsScript,PSTR("MG"),"mqttGroupTopic");
  //   printSetFormCheckbox(settingsScript,PSTR("BM"),0);//buttonPublishMqtt);
  //   printSetFormCheckbox(settingsScript,PSTR("RT"),0);//,retainMqttMsg);
  //   // settingsScript.printf_P(PSTR("d.Sf.MD.maxLength=%d;d.Sf.MG.maxLength=%d;d.Sf.MS.maxLength=%d;"),    MQTT_MAX_TOPIC_LEN, MQTT_MAX_TOPIC_LEN, MQTT_MAX_SERVER_LEN);
  //   #else
  //   settingsScript.print(F("toggle('MQTT');"));    // hide MQTT settings
  //   #endif

  //   #ifndef WLED_DISABLE_HUESYNC
  //   printSetFormValue(settingsScript,PSTR("H0"),hueIP[0]);
  //   printSetFormValue(settingsScript,PSTR("H1"),hueIP[1]);
  //   printSetFormValue(settingsScript,PSTR("H2"),hueIP[2]);
  //   printSetFormValue(settingsScript,PSTR("H3"),hueIP[3]);
  //   printSetFormValue(settingsScript,PSTR("HL"),huePollLightId);
  //   printSetFormValue(settingsScript,PSTR("HI"),huePollIntervalMs);
  //   printSetFormCheckbox(settingsScript,PSTR("HP"),huePollingEnabled);
  //   printSetFormCheckbox(settingsScript,PSTR("HO"),hueApplyOnOff);
  //   printSetFormCheckbox(settingsScript,PSTR("HB"),hueApplyBri);
  //   printSetFormCheckbox(settingsScript,PSTR("HC"),hueApplyColor);
  //   char hueErrorString[25];
  //   switch (hueError)
  //   {
  //     case HUE_ERROR_INACTIVE     : strcpy_P(hueErrorString,PSTR("Inactive"));                break;
  //     case HUE_ERROR_ACTIVE       : strcpy_P(hueErrorString,PSTR("Active"));                  break;
  //     case HUE_ERROR_UNAUTHORIZED : strcpy_P(hueErrorString,PSTR("Unauthorized"));            break;
  //     case HUE_ERROR_LIGHTID      : strcpy_P(hueErrorString,PSTR("Invalid light ID"));        break;
  //     case HUE_ERROR_PUSHLINK     : strcpy_P(hueErrorString,PSTR("Link button not pressed")); break;
  //     case HUE_ERROR_JSON_PARSING : strcpy_P(hueErrorString,PSTR("JSON parsing error"));      break;
  //     case HUE_ERROR_TIMEOUT      : strcpy_P(hueErrorString,PSTR("Timeout"));                 break;
  //     default: sprintf_P(hueErrorString,PSTR("Bridge Error %i"),hueError);
  //   }

  //   printSetClassElementHTML(settingsScript,PSTR("sip"),0,hueErrorString);
  //   #else
  //   settingsScript.print(F("toggle('Hue');"));    // hide Hue Sync settings
  //   #endif
  //   printSetFormValue(settingsScript,PSTR("BD"),serialBaud);
  //   #ifndef WLED_ENABLE_ADALIGHT
  //   settingsScript.print(F("toggle('Serial');"));
  //   #endif
  }

  if (subPage == SUBPAGE_WEB_TIME)
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

    // #endif // ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_WEB_SYNC
  }

  if (subPage == SUBPAGE_WEB_SEC)
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
  if (subPage == SUBPAGE_WEB_DMX)
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

  if (subPage == SUBPAGE_WEB_UM) //usermods
  {
    appendGPIOinfo(settingsScript);
    // settingsScript.printf_P(PSTR("numM=%d;"), UsermodManager::getModCount());
    // printSetFormValue(settingsScript,PSTR("SDA"),i2c_sda);
    // printSetFormValue(settingsScript,PSTR("SCL"),i2c_scl);
    // printSetFormValue(settingsScript,PSTR("MOSI"),spi_mosi);
    // printSetFormValue(settingsScript,PSTR("MISO"),spi_miso);
    // printSetFormValue(settingsScript,PSTR("SCLK"),spi_sclk);
    // settingsScript.printf_P(PSTR("addInfo('SDA','%d');"
    //              "addInfo('SCL','%d');"
    //              "addInfo('MOSI','%d');"
    //              "addInfo('MISO','%d');"
    //              "addInfo('SCLK','%d');"),
    //   HW_PIN_SDA, HW_PIN_SCL, HW_PIN_DATASPI, HW_PIN_MISOSPI, HW_PIN_CLOCKSPI
    // );
    // UsermodManager::appendConfigData(settingsScript);
  }

  if (subPage == SUBPAGE_WEB_UPDATE) // update
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

  if (subPage == SUBPAGE_WEB_2D) // 2D matrices
  {
    // printSetFormValue(settingsScript,PSTR("SOMP"),isMatrix);
    // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    // settingsScript.printf_P(PSTR("maxPanels=%d;resetPanels();"),WLED_MAX_PANELS);
    // if (isMatrix) {
    //   if(panels>0){
    //     printSetFormValue(settingsScript,PSTR("PW"),panel[0].width); //Set generator Width and Height to first panel size for convenience
    //     printSetFormValue(settingsScript,PSTR("PH"),panel[0].height);
    //   }
    //   printSetFormValue(settingsScript,PSTR("MPC"),panels);
    //   // panels
    //   for (unsigned i=0; i<panels; i++) {
    //     settingsScript.printf_P(PSTR("addPanel(%d);"), i);
    //     char pO[8] = { '\0' };
    //     snprintf_P(pO, 7, PSTR("P%d"), i);       // WLED_MAX_PANELS is 18 so pO will always only be 4 characters or less
    //     pO[7] = '\0';
    //     unsigned l = strlen(pO);
    //     // create P0B, P1B, ..., P63B, etc for other PxxX
    //     pO[l] = 'B'; printSetFormValue(settingsScript,pO,panel[i].bottomStart);
    //     pO[l] = 'R'; printSetFormValue(settingsScript,pO,panel[i].rightStart);
    //     pO[l] = 'V'; printSetFormValue(settingsScript,pO,panel[i].vertical);
    //     pO[l] = 'S'; printSetFormCheckbox(settingsScript,pO,panel[i].serpentine);
    //     pO[l] = 'X'; printSetFormValue(settingsScript,pO,panel[i].xOffset);
    //     pO[l] = 'Y'; printSetFormValue(settingsScript,pO,panel[i].yOffset);
    //     pO[l] = 'W'; printSetFormValue(settingsScript,pO,panel[i].width);
    //     pO[l] = 'H'; printSetFormValue(settingsScript,pO,panel[i].height);
    //   }
    // }
    // #else
    // settingsScript.print(F("gId(\"somp\").remove(1);")); // remove 2D option from dropdown
    // #endif
  }
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

  ALOG_DBG(PSTR("JSON buffer size: %u for request: %d\n"), lDoc.memoryUsage(), subJson);

  size_t len = response->setLength();
  ALOG_DBG(PSTR("JSON content length: %d"), len);

  request->send(response);
  // tkr_anim->releaseJSONBufferLock();
}


void mWebServer::serveSettingsJS(AsyncWebServerRequest* request)
{

  ALOG_INF(PSTR("serveSettingsJS url %s"), request->url().c_str());

  static const char _common_js[] PROGMEM = "/common.js";
  if (request->url().indexOf(FPSTR(_common_js)) > 0) {
    handleStaticContent(request, FPSTR(_common_js), 200, FPSTR(CONTENT_TYPE_JAVASCRIPT), JS_common_web, JS_common_web_length);
    return;
  }
  
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
 * @brief Resolve settings subpage ID from URL path
 */
static inline uint8_t GetSubPageID_FromURLPath(const String& url)
{
  if (url.indexOf("sett") >= 0)
  {
    if      (url.indexOf(".js")  > 0) return SUBPAGE_WEB_JS;
    else if (url.indexOf(".css") > 0) return SUBPAGE_WEB_CSS;
    else if (url.indexOf("wifi") > 0) return SUBPAGE_WEB_WIFI;
    else if (url.indexOf("leds") > 0) return SUBPAGE_WEB_LEDS;
    else if (url.indexOf("ui")   > 0) return SUBPAGE_WEB_UI;
    else if (url.indexOf("sync") > 0) return SUBPAGE_WEB_SYNC;
    else if (url.indexOf("time") > 0) return SUBPAGE_WEB_TIME;
    else if (url.indexOf("sec")  > 0) return SUBPAGE_WEB_SEC;
    #ifdef ENABLE_FEATURE_LIGHTING__DMX
    else if (url.indexOf("dmx")  > 0) return SUBPAGE_WEB_DMX;
    #endif
    else if (url.indexOf("um")   > 0) return SUBPAGE_WEB_UM;
    #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
    else if (url.indexOf("2D")   > 0) return SUBPAGE_WEB_2D;
    #endif
    else if (url.indexOf("lock") > 0) return SUBPAGE_WEB_LOCK;
  }
  else if (url.indexOf("/update") >= 0)
  {
    return SUBPAGE_WEB_UPDATE;
  }

  return SUBPAGE_WEB_WELCOME;
}


/**
 * @brief WebUI: Settings Pages (POST)
 */
void mWebServer::SettingsPages_POST(AsyncWebServerRequest* request)
{
  const String& url = request->url();
  uint8_t subPage   = GetSubPageID_FromURLPath(url);
  uint8_t originalSubPage = subPage;

  #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
  // Determine whether PIN is required for this subpage
  bool pinRequired = (!correctPIN && (strlen(settingsPIN) > 0) && (subPage > 0 && subPage < 11));
  if (pinRequired) {
    originalSubPage = subPage;
    subPage = SUBPAGE_WEB_PINREQ;
  }

  // if OTA locked or too frequent PIN entry requests fail hard
  if ((subPage == SUBPAGE_WEB_WIFI && wifiLock && otaLock) ||
      (pinRequired && !correctPIN && (millis() - lastEditTime < PIN_RETRY_COOLDOWN)))
  {
    serveMessage(request, 401, FPSTR(s_accessdenied), FPSTR(s_unlock_ota), 254);
    return;
  }
  #else
  bool pinRequired = false;
  #endif

  // Locks removed → always process POST
  SettingsPages__ParseForm(request, subPage);

  char s[32];
  char s2[45] = "";

  switch (subPage)
  {
    case SUBPAGE_WEB_WIFI: strcpy_P(s, PSTR("WiFi")); strcpy_P(s2, PSTR("Please connect to the new IP (if changed)")); break;
    case SUBPAGE_WEB_LEDS: strcpy_P(s, PSTR("LED")); break;
    case SUBPAGE_WEB_UI:   strcpy_P(s, PSTR("UI")); break;
    case SUBPAGE_WEB_SYNC: strcpy_P(s, PSTR("Sync")); break;
    case SUBPAGE_WEB_TIME: strcpy_P(s, PSTR("Time")); break;
    case SUBPAGE_WEB_SEC:
      strcpy_P(s, PSTR("Security"));
      if (tkr_sup->ESP_Restart_Scheduled()) strcpy_P(s2, PSTR("Rebooting, please wait ~10 seconds..."));
      break;

    #ifdef ENABLE_FEATURE_LIGHTING__DMX
    case SUBPAGE_WEB_DMX:  strcpy_P(s, PSTR("DMX")); break;
    #endif

    case SUBPAGE_WEB_UM:   strcpy_P(s, PSTR("Usermods")); break;

    #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
    case SUBPAGE_WEB_2D:   strcpy_P(s, PSTR("2D")); break;
    #endif

    #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
    case SUBPAGE_WEB_PINREQ:
      strcpy_P(s, correctPIN ? PSTR("PIN accepted") : PSTR("PIN rejected"));
      break;
    #endif

    default:
      // Fallback to avoid uninitialised 's'
      strcpy_P(s, PSTR("Settings"));
      break;
  }

  #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
  if (subPage != SUBPAGE_WEB_PINREQ) strcat_P(s, PSTR(" settings saved."));
  #endif

  // --- Critical fix: always terminate POST with a response ---
  #ifdef ENABLE_FEATURE_WEBSERVER__PIN_PROTECTION
  if (subPage == SUBPAGE_WEB_PINREQ && correctPIN) {
    // Correct PIN: redirect to the settings page the user originally intended (POST/Redirect/GET)
    // If your PIN form posts to a different URL than the intended page, replace 'url' with a mapper:
    // request->redirect(GetURL_FromSubPageID(originalSubPage));
    request->redirect(url);
    return;
  }
  #endif

  if (!s2[0]) strcpy_P(s2, s_redirecting);

  bool doReboot = tkr_sup->ESP_Restart_Scheduled(); // keep semantics consistent with your switch text above
  bool redirectAfter9s = (subPage == SUBPAGE_WEB_WIFI ||
                          ((subPage == SUBPAGE_WEB_SEC || subPage == SUBPAGE_WEB_UM) && doReboot));

  // If PIN was required and not yet satisfied, keep 401 to force the PIN flow.
  // Otherwise standard 200.
  serveMessage(request,
               (!pinRequired ? 200 : 401),
               s,
               s2,
               redirectAfter9s ? 129 : (!pinRequired ? 1 : 3));
  return;
}


/**
 * @brief WebUI: Settings Pages (GET)
 */
void mWebServer::SettingsPages_GET(AsyncWebServerRequest* request)
{
  const String& url = request->url();
  uint8_t subPage   = GetSubPageID_FromURLPath(url);

  int code = 200;
  String contentType = FPSTR(CONTENT_TYPE_HTML);
  const uint8_t* content = nullptr;
  size_t len = 0;

  switch (subPage)
  {
    case SUBPAGE_WEB_WIFI:    content = PAGE_settings_wifi_web; len = PAGE_settings_wifi_web_length; break;
    case SUBPAGE_WEB_LEDS:    content = PAGE_settings_leds_web;  len = PAGE_settings_leds_web_length;  break;
    case SUBPAGE_WEB_UI:      content = PAGE_settings_ui_web;    len = PAGE_settings_ui_web_length;    break;
    case SUBPAGE_WEB_SYNC:    content = PAGE_settings_sync_web;  len = PAGE_settings_sync_web_length;  break;
    case SUBPAGE_WEB_TIME:    content = PAGE_settings_time_web;  len = PAGE_settings_time_web_length;  break;
    case SUBPAGE_WEB_SEC:     content = PAGE_settings_sec_web;   len = PAGE_settings_sec_web_length;   break;
    #ifdef ENABLE_FEATURE_LIGHTING__DMX
    case SUBPAGE_WEB_DMX:     content = PAGE_settings_dmx;   len = PAGE_settings_dmx_length;   break;
    #endif
    case SUBPAGE_WEB_UM:      content = PAGE_settings_um_web;    len = PAGE_settings_um_web_length;    break;
    case SUBPAGE_WEB_UPDATE:  content = PAGE_update_web;         len = PAGE_update_web_length;         break;
    #ifdef ENABLE_FEATURE_LIGHTING__2D_MATRIX
    case SUBPAGE_WEB_2D:      content = PAGE_settings_2D;    len = PAGE_settings_2D_length;    break;
    #endif
    case SUBPAGE_WEB_PINREQ:  content = PAGE_settings_pin_web;   len = PAGE_settings_pin_web_length;   code = 401; break;
    case SUBPAGE_WEB_CSS:     content = PAGE_settingsCss_web;    len = PAGE_settingsCss_web_length;    contentType = FPSTR(CONTENT_TYPE_CSS); break;
    case SUBPAGE_WEB_JS:      serveSettingsJS(request); return;
    case SUBPAGE_WEB_WELCOME: content = PAGE_welcome_web;        len = PAGE_welcome_web_length;        break;
    default:              content = PAGE_settings_web;       len = PAGE_settings_web_length;       break;
  }

  handleStaticContent(request, "", code, contentType, content, len);
}








#ifdef USE_MODULE_NETWORK_WEBSERVER21

// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Shared resources
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// void mWebServer::WebSend_JSON_RootStatus_Table(AsyncWebServerRequest *request){

//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     tkr->Tasker_Interface(TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED);
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// }


// void mWebServer::Web_Base_Page_Draw(AsyncWebServerRequest *request){
        
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     WebAppend_Root_Draw_PageTitleFields();
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function


// void mWebServer::WebSend_JSON_WebServer_TopBar(AsyncWebServerRequest *request){
    
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;} 

//   char buffer[30];

//   JBI->Start();
//   JBI->Array_Start("info_row");// Class name
//     for(int row=0;row<4;row++){
//     JBI->Object_Start();
//       JBI->Add("id",row);
//       switch(row){
//         case 0:
//           JBI->Add_FV("ih",PSTR("\"%s U%s\""), tkr_time->RtcTime.hhmmss_ctr, tkr_time->uptime.hhmmss_ctr);
//           JBI->Add("fc", tkr_time->uptime_seconds_nonreset<SEC_IN_HOUR?PSTR("#ff0000"):PSTR("#ffffff"));    
//         break;
//         case 1:{        
//           int8_t wifi_perc = tkr_wifi->GetRSSPercentage();
//           char colour_ctr[7];
//           if(wifi_perc<20){      sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ff0000")); }
//           else if(wifi_perc<30){ sprintf_P(colour_ctr,PSTR("%s"),PSTR("#fcba03")); }
//           else{                  sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ffffff")); }
//           JBI->Add_FV("ih",PSTR("\"%s %d%% (%d&nbsp;dBm)\""), WiFi.SSID().c_str(),wifi_perc,tkr_wifi->GetRSSdBm());
//           JBI->Add("fc", colour_ctr);    
//         }break;
//         case 2:
//           JBI->Add("ihr",tkr_set->runtime.firmware_version.current.name_ctr);
//           JBI->Add("fc", tkr_sup->GetVersionColour(buffer));    

//         break;
//         case 3:
//             JBI->Add_FV("ih",PSTR("\"ROW3%dc %d %s|%s PT(%s) LPS(%d)\""), 
//                 tkr_set->Settings.bootcount, 
//                 ESP.getFreeHeap(), 
//                 F(__DATE__), 
//                 F(__TIME__), 
//                 tkr_set->runtime.boot_status.module_template_used ? "Y" : "N", 
//                 tkr_sup->activity.cycles_per_sec
//             );
//         break;
//       } //end switch 
//    JBI->Object_End();
//    } // end for

//   JBI->Array_End();
  
//   // JBI->Array_Start("debug_line");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add_FV("ih",PSTR("\"%dc %d %s|%s PT(%s) LPS(%d)\""), tkr_set->Settings.bootcount, ESP.getFreeHeap(), F(__DATE__), F(__TIME__), tkr_set->runtime.boot_status.module_template_used ? "Y" : "N", tkr_sup->activity.cycles_per_sec);
//   //   JBI->Object_End();
//   // JBI->Array_End();
//   JBI->End();


//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } // end function




// void mWebServer::WebSend_JSON_WebServer_StatusPopoutData(AsyncWebServerRequest *request){
    
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;} 

//   char buffer[30];

//   JBI->Start();
//   JBI->Array_Start("info_row");// Class name
//     for(int row=0;row<6;row++){
//     JBI->Object_Start();
//       JBI->Add("id",row);
//       switch(row){
//         case 0:
//           JBI->Add_FV("ih",PSTR("\"%s U%s\""), tkr_time->RtcTime.hhmmss_ctr, tkr_time->uptime.hhmmss_ctr);
//           JBI->Add("fc", tkr_time->uptime_seconds_nonreset<SEC_IN_HOUR?PSTR("#ff0000"):PSTR("#ffffff"));    
//         break;
//         case 1:{        
//           int8_t wifi_perc = tkr_wifi->GetRSSPercentage();
//           char colour_ctr[7];
//           if(wifi_perc<20){      sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ff0000")); }
//           else if(wifi_perc<30){ sprintf_P(colour_ctr,PSTR("%s"),PSTR("#fcba03")); }
//           else{                  sprintf_P(colour_ctr,PSTR("%s"),PSTR("#ffffff")); }
//           JBI->Add_FV("ih",PSTR("\"%s %d%% (%d&nbsp;dBm)\""), WiFi.SSID().c_str(),wifi_perc,tkr_wifi->GetRSSdBm());
//           JBI->Add("fc", colour_ctr);   
//         }break;
//         case 2:
//           JBI->Add("ihr",tkr_set->runtime.firmware_version.current.name_ctr);
//           JBI->Add("fc", tkr_sup->GetVersionColour(buffer));    
//         break;
//         case 3:
//           JBI->Add_FV("ih",PSTR("\"Boot: %dc PT(%s)\""), tkr_set->Settings.bootcount, tkr_set->runtime.boot_status.module_template_used ? "Y" : "N");
//         break;
//         case 4:
//           JBI->Add_FV("ih",PSTR("\"Firmware: %s %s|%s\""), tkr_set->runtime.firmware_version.current.name_ctr, F(__DATE__), F(__TIME__));
//           JBI->Add("fc", tkr_sup->GetVersionColour(buffer));    
//         break;
//         case 5:
//           JBI->Add_FV("ih",PSTR("\"Runtime: LPS(%d Hz | %d ms) FreeHeap:%d\""), tkr_sup->activity.cycles_per_sec, 1000/tkr_sup->activity.cycles_per_sec, ESP.getFreeHeap());
//         break;
//         case 6:
//           JBI->Add_FV("ih",PSTR("\"Templates: M%d R%d L%d D%d\""), 0, 0, 0, 0);
//         break;
//       } //end switch 
//     JBI->Object_End();
//     } // end for

//   JBI->Array_End();
  
//   // JBI->Array_Start("debug_line");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add_FV("ih",PSTR("\"%dc %d %s|%s PT(%s) LPS(%d)\""), tkr_set->Settings.bootcount, ESP.getFreeHeap(), F(__DATE__), F(__TIME__), tkr_set->runtime.boot_status.module_template_used ? "Y" : "N", tkr_sup->activity.cycles_per_sec);
//   //   JBI->Object_End();
//   // JBI->Array_End();
//   JBI->End();


//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } // end function



// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Root Page
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// void mWebServer::HandlePage_Root(AsyncWebServerRequest *request){

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_TST(PSTR("mWebServer::HandlePage_Root"));
//     #endif //  ENABLE_LOG_LEVEL_INFO

//   //AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_ASYNC WEB_HANDLER_SCRIPT_WEB_DATA_FETCHER_RUNTIME_URLS_RATES_VAR "Sf::%s"),"HandlePage_Root");

//   // if (CaptivePortal(request)) { return; }  // If captive portal redirect instead of displaying the page.

//   // if (WifiIsInManagerMode()) {
//   //   #ifndef FIRMWARE_MINIMAL
//   //     if (
//   //         (tkr_set->Settings.web_password[0] != 0) && 
//   //         !(request->hasParam("USER1")) && !(request->hasParam("PASS1")
//   //       ) && HTTP_MANAGER_RESET_ONLY != webserver_state) {
//   //       HandleWifiLogin(request);
//   //     } else {
//   //       if (!(tkr_set->Settings.web_password[0] != 0) || 
//   //           (((request->arg("USER1") == WEB_USERNAME ) && (request->arg("PASS1") == tkr_set->Settings.web_password )) || HTTP_MANAGER_RESET_ONLY == webserver_state)) {
//   //         HandleWifiConfiguration(request);
//   //       } else {
//   //         // wrong user and pass
//   //         HandleWifiLogin(request);
//   //       }
//   //     }
//   //   #endif  // Not FIRMWARE_MINIMAL
//   //   return;
//   // }
  
//   // delay(1000);


//   // JBI->Start();
//   //   JBI->Object_Start("function");
//   //     JBI->Object_Start("Parse_Urls");
//   //       // tkr->Tasker_Interface(TASK_WEB_APPEND_RUNTIME_ROOT_URLS);
//   //     JBI->Object_End();
//   //   JBI->Object_End();
//   // JBI->End();

//   // request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);

//   AsyncWebServerResponse *response = request->beginResponse_P(200, CONTENT_TYPE_TEXT_HTML_ID, PAGE_ROOT, PAGE_ROOT_L);
//   response->addHeader("Content-Encoding","gzip");
//   request->send(response);

// }


// void mWebServer::Web_Root_Draw(AsyncWebServerRequest *request){
        
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     WebAppend_Root_Draw_PageTitleFields();
//     WebAppend_Root_Draw_PageTable();
//     WebAppend_Root_Draw_PageButtons();
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// } //end function


// void mWebServer::Web_Root_Draw_Modules(AsyncWebServerRequest *request){
        
//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
//     WebAppend_Root_Draw_ModuleTable();
//     WebAppend_Root_Draw_ModuleButtons();
//   JBI->End();
  
//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  
  
// } //end function


// //append to internal buffer if any root messages table
// void mWebServer::WebAppend_Root_Draw_ModuleButtons(){

//   JBI->Array_Start("container_4");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"");
//         tkr->Tasker_Interface(TASK_WEB_APPEND_ROOT_BUTTONS);
//       JBI->AppendBuffer("\"");
//     JBI->Object_End();
//   JBI->Array_End();

// }

// void mWebServer::WebAppend_Root_Draw_PageButtons(){
  
//   // JBI->Array_Start("container_5");// Class name
//   //   JBI->Object_Start();
//   //     JBI->AddKey("ihr");           // function
//   //       JBI->AppendBuffer("\"");
//   //       WebAppend_Button_Spaced(BUTTON_CONSOLE);
//   //       WebAppend_Button(BUTTON_SYSTEM_SETTINGS);
//   //     JBI->AppendBuffer("\"");
//   //   JBI->Object_End();
//   // JBI->Array_End();

// }


// void mWebServer::Web_Root_UpdateURLs(AsyncWebServerRequest *request){
  
//   JBI->Start();
//     JBI->Object_Start("function");
//       JBI->Object_Start("Parse_Urls");
//         tkr->Tasker_Interface(TASK_WEB_APPEND_RUNTIME_ROOT_URLS);
//       JBI->Object_End();
//     JBI->Object_End();
//   JBI->End();

//   request->send_P(200, CONTENT_TYPE_APPLICATION_JSON_ID, data_buffer.payload.ctr);
  
// }



// void mWebServer::WebAppend_Root_Draw_PageTitleFields(){

//   JBI->Array_Start("page_title");// Class name
//     JBI->Object_Start();
//       JBI->Add("ihr",tkr_set->Settings.system_name.friendly);
//       JBI->Add("fc", D_COLOUR_PAGE_TITLE);           
//     JBI->Object_End();
//   JBI->Array_End();

//   // char buffer[10];
//   // JBI->Array_Start("row_version_data");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add("ihr",tkr_set->runtime.firmware_version.current.name_ctr);
//   //     JBI->Add("fc", tkr_sup->GetVersionColour(buffer));           
//   //   JBI->Object_End();
//   // JBI->Array_End();

//   // JBI->Object_Start("function");
//   //     JBI->Add("SetTitle",tkr_set->Settings.system_name.friendly);
//   // JBI->Object_End();  

  
//   // JBI->Array_Start("function");// Class name
//   //   JBI->Object_Start();
//   //     JBI->Add("SetTitle","Heelo");//tkr_set->Settings.system_name.friendly);
//   //   JBI->Object_End();
//   // JBI->Array_End();


// }

// void mWebServer::WebAppend_Root_Draw_PageTable(){

//   JBI->Array_Start("container_2");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"{t}");
//         tkr->Tasker_Interface(TASK_WEB_ADD_ROOT_TABLE_ROWS);
//       JBI->AppendBuffer("{t2}\"");
//     JBI->Object_End();
//   JBI->Array_End();

// }


// void mWebServer::WebAppend_Root_Draw_ModuleTable(){

//   JBI->Array_Start("container_3");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("ihr");           // function
//         JBI->AppendBuffer("\"");
//         tkr->Tasker_Interface(TASK_WEB_ADD_ROOT_MODULE_TABLE_CONTAINER);
//       JBI->AppendBuffer("\"");
//     JBI->Object_End();
//   JBI->Array_End();

// }


// void mWebServer::Console_JSON_Data(AsyncWebServerRequest *request){

//   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
  
//   JBI->Start();
    
//   JBI->Array_Start("function");// Class name
//     JBI->Object_Start();
//       JBI->AddKey("Append_Console");

//         char buffer[500];
//         // sprintf(buffer, "{'link':1,'text':'hello','reset':0}");
//         sprintf(buffer, "{\\\"link\\\":1}");

// // I need to think how ANYTHING can pass through serial, json might not work


//         // sprintf(buffer, "{}");
//         JBI->AppendBuffer("\"");
//         JBI->AppendBuffer(buffer);
//         JBI->AppendBuffer("\"");

//     JBI->Object_End();
//   JBI->Array_End();
        
//   JBI->End();

//   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  




// }




// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Information Page
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */


// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Configuration and Templates
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */



// void mWebServer::HandleWifiConfiguration(AsyncWebServerRequest *request)
// {
// //   //if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

// //   ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_WIFI);

// //   if (request->hasParam("save") && HTTP_MANAGER_RESET_ONLY != webserver_state) {
// //     WifiSaveSettings(request);
// //     WebRestart(request, 2);
// //     return;
// //   }

// //   WSStartAppend_P(request, S_CONFIGURE_WIFI, !WifiIsInManagerMode());
// //   WSBufferAppend_P(response, HTTP_SCRIPT_WIFI);
// //   WSContentSendStyle(request);

// //   if (HTTP_MANAGER_RESET_ONLY != webserver_state) {
// //     if (request->hasParam("scan")) {
// // #ifdef USE_EMULATION
// //       //UdpDisconnect();
// // #endif  // USE_EMULATION
// //       int n = WiFi.scanNetworks();
// //       ALOG_DBG(PSTR(D_LOG_WIFI D_SCAN_DONE));

// //       if (0 == n) {
// //         ALOG_DBG(S_LOG_WIFI, S_NO_NETWORKS_FOUND);
// //         WSBufferAppend_P(response, S_NO_NETWORKS_FOUND);
// //         WSBufferAppend_P(response, PSTR(". " D_REFRESH_TO_SCAN_AGAIN "."));
// //       } else {
// //         //sort networks
// //         int indices[n];
// //         for (int i = 0; i < n; i++) {
// //           indices[i] = i;
// //         }

// //         // RSSI SORT
// //         for (int i = 0; i < n; i++) {
// //           for (int j = i + 1; j < n; j++) {
// //             if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
// //               std::swap(indices[i], indices[j]);
// //             }
// //           }
// //         }

// //         // remove duplicates ( must be RSSI sorted )
// //         if (remove_duplicate_access_points) {
// //           String cssid;
// //           for (int i = 0; i < n; i++) {
// //             if (-1 == indices[i]) { continue; }
// //             cssid = WiFi.SSID(indices[i]);
// //             for (int j = i + 1; j < n; j++) {
// //               if (cssid == WiFi.SSID(indices[j])) {
// //                 ALOG_DBG(PSTR(D_LOG_WIFI D_DUPLICATE_ACCESSPOINT " %s"), WiFi.SSID(indices[j]).c_str());
// //                 indices[j] = -1;  // set dup aps to index -1
// //               }
// //             }
// //           }
// //         }

// //         //display networks in page
// //         for (int i = 0; i < n; i++) {
// //           if (-1 == indices[i]) { continue; }  // skip dups
// //           ALOG_DBG(PSTR(D_LOG_WIFI D_SSID " %s, " D_BSSID " %s, " D_CHANNEL " %d, " D_RSSI " %d"), WiFi.SSID(indices[i]).c_str(), WiFi.BSSIDstr(indices[i]).c_str(), WiFi.channel(indices[i]), WiFi.RSSI(indices[i]));
          
// //           int quality = tkr_wifi->WifiGetRssiAsQuality(WiFi.RSSI(indices[i]));
// //           int rss = WiFi.RSSI(indices[i]);

// //           if (minimum_signal_quality == -1 || minimum_signal_quality < quality) {
// //             uint8_t auth = WiFi.encryptionType(indices[i]);
            
// //             #ifdef ESP8266
// //             WSBufferAppend_P(response, PSTR("<div><a href='#p' onclick='c(this)'>%s</a>&nbsp;(%d)&nbsp<span class='q'>%s %d dBm (%d%%)</span></div>"),
// //               htmlEscape(WiFi.SSID(indices[i])).c_str(),
// //               WiFi.channel(indices[i]),
// //               (ENC_TYPE_WEP == auth) ? D_WEP : (ENC_TYPE_TKIP == auth) ? D_WPA_PSK : (ENC_TYPE_CCMP == auth) ? D_WPA2_PSK : (ENC_TYPE_AUTO == auth) ? D_AUTO : "",
// //               rss,
// //               quality
// //             );
// //             #endif
// //             delay(0);
// //           } else {
// //             ALOG_DBG(PSTR(D_LOG_WIFI D_SKIPPING_LOW_QUALITY));
// //           }

// //         }
// //         WSBufferAppend_P(response, PSTR("<br/>"));
// //       }
// //     } else {
// //       WSBufferAppend_P(response, PSTR("<div><a href='/wi?scan='>" D_SCAN_FOR_WIFI_NETWORKS "</a></div><br/>"));
// //     }

// //     // As WIFI_HOSTNAME may contain %s-%04d it cannot be part of HTTP_FORM_WIFI where it will exception
// //     WSBufferAppend_P(response, HTTP_FORM_WIFI, tkr_set->Settings.sta_ssid[0], tkr_set->Settings.sta_ssid[1], WIFI_HOSTNAME, WIFI_HOSTNAME, tkr_set->Settings.hostname);
// //     WSBufferAppend_P(response, HTTP_FORM_END);
// //   }

// //   if (WifiIsInManagerMode()) {
// //     WSContentSpaceButton(request, BUTTON_RESTART);
// // #ifndef FIRMWARE_MINIMAL
// //     WSContentSpaceButton(request, BUTTON_RESET_CONFIGURATION);
// // #endif  // FIRMWARE_MINIMAL
// //   } else {
// //     WSContentSpaceButton(request, BUTTON_CONFIGURATION);
// //   }
// //   WSContentStop(request);
// }

// void mWebServer::WifiSaveSettings(AsyncWebServerRequest *request)
// {
//   // char tmp[sizeof(tkr_set->Settings.sta_pwd[0])];  // Max length is currently 65

//   // WebGetArg(request,"h", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.hostname, (!strlen(tmp)) ? WIFI_HOSTNAME : tmp, sizeof(tkr_set->Settings.hostname));
//   // if (strstr(tkr_set->Settings.hostname, "%") != nullptr) {
//   //   strlcpy(tkr_set->Settings.hostname, WIFI_HOSTNAME, sizeof(tkr_set->Settings.hostname));
//   // }
//   // WebGetArg(request,"s1", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_ssid[0], (!strlen(tmp)) ? STA_SSID1 : tmp, sizeof(tkr_set->Settings.sta_ssid[0]));
//   // WebGetArg(request,"s2", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_ssid[1], (!strlen(tmp)) ? STA_SSID2 : tmp, sizeof(tkr_set->Settings.sta_ssid[1]));
//   // WebGetArg(request,"s3", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_ssid[2], (!strlen(tmp)) ? STA_SSID3 : tmp, sizeof(tkr_set->Settings.sta_ssid[2]));
//   // WebGetArg(request,"p1", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_pwd[0], (!strlen(tmp)) ? "" : (strlen(tmp) < 5) ? tkr_set->Settings.sta_pwd[0] : tmp, sizeof(tkr_set->Settings.sta_pwd[0]));
//   // WebGetArg(request,"p2", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.sta_pwd[1], (!strlen(tmp)) ? "" : (strlen(tmp) < 5) ? tkr_set->Settings.sta_pwd[1] : tmp, sizeof(tkr_set->Settings.sta_pwd[1]));
//   // ALOG_INF(PSTR(D_LOG_WIFI D_HOSTNAME " %s, " D_SSID "1 %s, " D_SSID "2 %s"), tkr_set->Settings.hostname, tkr_set->Settings.sta_ssid[0], tkr_set->Settings.sta_ssid[1]);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleLoggingConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_LOGGING);

//   // if (request->hasParam("save")) {
//   //   LoggingSaveSettings(request);
//   //   HandleConfiguration(request);
//   //   return;
//   // }

//   // WSStartAppend_P(request, S_CONFIGURE_LOGGING);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_LOG1);
//   // char stemp1[32];
//   // char stemp2[32];
//   // uint8_t dlevel[3] = { LOG_LEVEL_INFO, LOG_LEVEL_INFO, LOG_LEVEL_NONE };
//   // for (uint8_t idx = 0; idx < 3; idx++) {
//   //   uint8_t llevel = (0==idx)?tkr_set->Settings.logging.serial_level:(1==idx)?tkr_set->Settings.logging.web_level:tkr_set->Settings.syslog_level;
//   //   WSBufferAppend_P(response, PSTR("<p><b>%s</b> (%s)<br/><select id='l%d' name='l%d'>"),
//   //     tkr_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), idx, kLoggingOptions),
//   //     tkr_sup->GetTextIndexed_P(stemp2, sizeof(stemp2), dlevel[idx], kLoggingLevels),
//   //     idx, idx);
//   //   for (uint8_t i = LOG_LEVEL_NONE; i < LOG_LEVEL_ALL; i++) {
//   //     WSBufferAppend_P(response, PSTR("<option%s value='%d'>%d %s</option>"),
//   //       (i == llevel) ? " selected" : "", i, i,
//   //       tkr_sup->GetTextIndexed_P(stemp1, sizeof(stemp1), i, kLoggingLevels));
//   //   }
//   //   WSBufferAppend_P(response, PSTR("</select></p>"));
//   // }
//   // WSBufferAppend_P(response, HTTP_FORM_LOG2, tkr_set->Settings.syslog_host, tkr_set->Settings.syslog_port, tkr_set->Settings.tele_period);
//   // WSBufferAppend_P(response, HTTP_FORM_END);
//   // WSContentSpaceButton(request, BUTTON_CONFIGURATION);
//   // WSContentStop(request);
// }

// void mWebServer::LoggingSaveSettings(AsyncWebServerRequest *request)
// {
//   // char tmp[sizeof(tkr_set->Settings.syslog_host)];  // Max length is currently 33

//   // WebGetArg(request,"l0", tmp, sizeof(tmp));
//   // tkr_set->Settings.logging.serial_level = (!strlen(tmp)) ? SERIAL_LOG_LEVEL : atoi(tmp);
//   // WebGetArg(request,"l1", tmp, sizeof(tmp));
//   // tkr_set->Settings.logging.web_level = (!strlen(tmp)) ? WEB_LOG_LEVEL : atoi(tmp);
//   // WebGetArg(request,"l2", tmp, sizeof(tmp));
//   // tkr_set->Settings.syslog_level = (!strlen(tmp)) ? SYS_LOG_LEVEL : atoi(tmp);
//   // tkr_set->syslog_level = tkr_set->Settings.syslog_level;
//   // tkr_set->syslog_timer = 0;
//   // WebGetArg(request,"lh", tmp, sizeof(tmp));
//   // strlcpy(tkr_set->Settings.syslog_host, (!strlen(tmp)) ? SYS_LOG_HOST : tmp, sizeof(tkr_set->Settings.syslog_host));
//   // WebGetArg(request,"lp", tmp, sizeof(tmp));
//   // tkr_set->Settings.syslog_port = (!strlen(tmp)) ? SYS_LOG_PORT : atoi(tmp);
//   // WebGetArg(request,"lt", tmp, sizeof(tmp));
//   // tkr_set->Settings.tele_period = (!strlen(tmp)) ? TELE_PERIOD : atoi(tmp);
//   // if ((tkr_set->Settings.tele_period > 0) && (tkr_set->Settings.tele_period < 10)) {
//   //   tkr_set->Settings.tele_period = 10;   // Do not allow periods < 10 seconds
//   // }
//   // ALOG_INF(PSTR(D_LOG_LOG D_SERIALLOG " %d, " D_WEBLOG " %d, " D_SYSLOG " %d, " D_LOGHOST " %s, " D_LOGPORT " %d, " D_TELEPERIOD " %d"),
//   //   tkr_set->Settings.logging.serial_level, tkr_set->Settings.logging.web_level, tkr_set->Settings.syslog_level, tkr_set->Settings.syslog_host, tkr_set->Settings.syslog_port, tkr_set->Settings.tele_period);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleOtherConfiguration(AsyncWebServerRequest *request)
// {
// //   if (!HttpCheckPriviledgedAccess()) { return; }

// //   ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_OTHER);

// //   if (request->hasParam("save")) {
// //     OtherSaveSettings(request);
// //     WebRestart(request, 1);
// //     return;
// //   }

// //   WSStartAppend_P(request, S_CONFIGURE_OTHER);
// //   WSContentSendStyle(request);

// //   tkr_sup->TemplateJson(); 
// //   char stemp[strlen(data_buffer.payload.ctr) +1];
// //   strlcpy(stemp, data_buffer.payload.ctr, sizeof(stemp));  // Get JSON template
// //   WSBufferAppend_P(response, HTTP_FORM_OTHER, stemp, (USER_MODULE == tkr_set->Settings.module) ? " checked disabled" : "", (tkr_set->Settings.flag_system.mqtt_enabled) ? " checked" : "");

// //   uint8_t maxfn = (tkr_set->devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : (!tkr_set->devices_present) ? 1 : tkr_set->devices_present;
// //   if (MODULE_SONOFF_IFAN02 == tkr_set->my_module_type) { maxfn = 1; }
// //   for (uint8_t i = 0; i < maxfn; i++) {
// //     snprintf_P(stemp, sizeof(stemp), PSTR("%d"), i +1);
// //     WSBufferAppend_P(response, PSTR("<b>" D_FRIENDLY_NAME " %d</b> (" FRIENDLY_NAME "%s)<br/><input id='a%d' name='a%d' placeholder='" FRIENDLY_NAME "%s' value='%s'><p></p>"),
// //       i +1,
// //       (i) ? stemp : "",
// //       i, i,
// //       (i) ? stemp : "",
// //       tkr_set->Settings.system_name.friendly[i]);
// //   }

// // #ifdef USE_EMULATION
// //   WSBufferAppend_P(response, PSTR("<p></p><fieldset><legend><b>&nbsp;" D_EMULATION "&nbsp;</b></legend><p>"));  // Keep close to Friendlynames so do not use <br/>
// //   for (uint8_t i = 0; i < EMUL_MAX; i++) {
// //     WSBufferAppend_P(response, PSTR("<input id='r%d' name='b2' type='radio' value='%d'%s><b>%s</b> %s<br/>"),  // Different id only used for labels
// //       i, i,
// //       (i == tkr_set->Settings.flag_power.emulation) ? " checked" : "",
// //       tkr_sup->GetTextIndexed_P(stemp, sizeof(stemp), i, kEmulationOptions),
// //       (i == EMUL_NONE) ? "" : (i == EMUL_WEMO) ? D_SINGLE_DEVICE : D_MULTI_DEVICE);
// //   }
// //   WSBufferAppend_P(response, PSTR("</p></fieldset>"));
// // #endif  // USE_EMULATION

// //   WSBufferAppend_P(response, HTTP_FORM_END);
// //   WSContentSpaceButton(request, BUTTON_CONFIGURATION);
// //   WSContentStop(request);
// }

// void mWebServer::OtherSaveSettings(AsyncWebServerRequest *request)
// {
// //   char tmp[128];
// //   char webindex[5];
// //   char friendlyname[sizeof(tkr_set->Settings.system_name.friendly[0])];

// //   WebGetArg(request,"wp", tmp, sizeof(tmp));
// //   strlcpy(tkr_set->Settings.web_password, (!strlen(tmp)) ? "" : (strchr(tmp,'*')) ? tkr_set->Settings.web_password : tmp, sizeof(tkr_set->Settings.web_password));
// //   tkr_set->Settings.flag_system.mqtt_enabled = request->hasParam("b1");
// // #ifdef USE_EMULATION
// //   WebGetArg(request,"b2", tmp, sizeof(tmp));
// //   tkr_set->Settings.flag_power.emulation = (!strlen(tmp)) ? 0 : atoi(tmp);
// // #endif  // USE_EMULATION
// //   snprintf_P(tkr_set->log_data, sizeof(tkr_set->log_data), PSTR(D_LOG_OTHER D_MQTT_ENABLE " %s, " D_EMULATION " %d, " D_FRIENDLYNAME), tkr_sup->GetStateText(tkr_set->Settings.flag_system.mqtt_enabled), tkr_set->Settings.flag_power.emulation);
// //   for (uint8_t i = 0; i < MAX_FRIENDLYNAMES; i++) {
// //     snprintf_P(webindex, sizeof(webindex), PSTR("a%d"), i);
// //     WebGetArg(request, webindex, tmp, sizeof(tmp));
// //     snprintf_P(friendlyname, sizeof(friendlyname), PSTR(FRIENDLY_NAME"%d"), i +1);
// //     strlcpy(tkr_set->Settings.system_name.friendly[i], (!strlen(tmp)) ? (i) ? friendlyname : FRIENDLY_NAME : tmp, sizeof(tkr_set->Settings.system_name.friendly[i]));
// //     snprintf_P(tkr_set->log_data, sizeof(tkr_set->log_data), PSTR("%s%s %s"), tkr_set->log_data, (i) ? "," : "", tkr_set->Settings.system_name.friendly[i]);
// //   }
// //   //AddLogAddLog(LOG_LEVEL_INFO);
// //   WebGetArg(request,"t1", tmp, sizeof(tmp));
// //   if (strlen(tmp)) {  // {"NAME":"12345678901234","GPIO":[255,255,255,255,255,255,255,255,255,255,255,255,255],"FLAG":255,"BASE":255}
// //     char svalue[128];
// //     snprintf_P(svalue, sizeof(svalue), PSTR(D_TEMPLATE " %s"), tmp);
// //     ExecuteWebCommand(svalue, SRC_WEBGUI);

// //     if (request->hasParam("t2")) {
// //       snprintf_P(svalue, sizeof(svalue), PSTR(D_MODULE " 0"));
// //       ExecuteWebCommand(svalue, SRC_WEBGUI);
// //     }

// //   }
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleBackupConfiguration(AsyncWebServerRequest *request)
// {
// //   if (!HttpCheckPriviledgedAccess()) { return; }

// //   ALOG_DBG(PSTR(D_LOG_HTTP D_BACKUP_CONFIGURATION));

// //   if (!tkr_set->SettingsBufferAlloc()) { return; }

// //   // WiFiClient myClient = pWebServer->client();
// //   pWebServer->setContentLength(sizeof(tkr_set->Settings));

// //   char attachment[100];
// //   char friendlyname[sizeof(tkr_set->Settings.system_name.friendly)];
// //   snprintf_P(attachment, sizeof(attachment), PSTR("attachment; filename=Config_%s_%s.dmp"), tkr_sup->NoAlNumToUnderscore(friendlyname, tkr_set->Settings.system_name.friendly), tkr_set->my_version);
// //   //request->sendHeader(F("Content-Disposition"), attachment);// asyncedit

// //   WSSend(200, CT_STREAM, "");

// //   uint16_t cfg_crc = tkr_set->Settings.cfg_crc;
// //   tkr_set->Settings.cfg_crc = tkr_set->GetSettingsCrc();  // Calculate crc (again) as it might be wrong when savedata = 0 (#3918)

// //   memcpy(tkr_set->settings_buffer, &tkr_set->Settings, sizeof(tkr_set->Settings));
// //   if (config_xor_on_set) {
// //     for (uint16_t i = 2; i < sizeof(tkr_set->Settings); i++) {
// //       tkr_set->settings_buffer[i] ^= (config_xor_on_set +i);
// //     }
// //   }

// // #ifdef ARDUINO_ESP8266_RELEASE_2_3_0
// //   size_t written = myClient.write((const char*)tkr_set->settings_buffer, sizeof(tkr_set->Settings));
// //   if (written < sizeof(tkr_set->Settings)) {  // https://github.com/esp8266/Arduino/issues/3218
// //     myClient.write((const char*)tkr_set->settings_buffer +written, sizeof(tkr_set->Settings) -written);
// //   }
// // #else
// //   myClient.write((const char*)tkr_set->settings_buffer, sizeof(tkr_set->Settings));
// // #endif

// //   tkr_set->SettingsBufferFree();

// //   tkr_set->Settings.cfg_crc = cfg_crc;  // Restore crc in case savedata = 0 to make sure settings will be noted as changed
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleResetConfiguration(AsyncWebServerRequest *request)
// {
//   // //if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_RESET_CONFIGURATION);

//   // WSStartAppend_P(request, S_RESET_CONFIGURATION, !WifiIsInManagerMode());
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, PSTR("<div style='text-align:center;'>" D_CONFIGURATION_RESET "</div>"));
//   // WSBufferAppend_P(response, HTTP_MSG_RSTRT);
//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);

//   // char command[CMDSZ];
//   // snprintf_P(command, sizeof(command), PSTR(D_RESET " 1"));
//   // ExecuteWebCommand(command, SRC_WEBGUI);
// }

// void mWebServer::HandleReset(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess(!WifiIsInManagerMode())) { return; }

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(S_LOG_HTTP, S_RESET_CONFIGURATION);
//     #endif// ENABLE_LOG_LEVEL_INFO

//   tkr_wifi->EspRestart();

// }

// void mWebServer::HandleRestoreConfiguration(AsyncWebServerRequest *request)
// {
//   if (!HttpCheckPriviledgedAccess()) { return; }

//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(S_LOG_HTTP, S_RESTORE_CONFIGURATION);
//     #endif// ENABLE_LOG_LEVEL_INFO

//   // WSStartAppend_P(request, S_RESTORE_CONFIGURATION);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_RST);
//   // WSBufferAppend_P(response, HTTP_FORM_RST_UPG, D_RESTORE);
//   // WSContentSpaceButton(request, BUTTON_CONFIGURATION);
//   // WSContentStop(request);

//   // upload_error = 0;
//   // upload_file_type = UPL_SETTINGS;
// }

// /*-------------------------------------------------------------------------------------------*/

// // #endif  // Not FIRMWARE_MINIMAL


// void mWebServer::HandleConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURATION);

//   // WSStartAppend_P(request, S_CONFIGURATION);
//   // WSContentSendStyle(request);

//   // WSContentButton(request, BUTTON_MODULE);
//   // WSContentButton(request, BUTTON_WIFI);

//   // tkr->Tasker_Interface(TASK_WEB_ADD_BUTTON);

//   // WSContentButton(request, BUTTON_LOGGING);
//   // WSContentButton(request, BUTTON_OTHER);
//   // WSContentButton(request, BUTTON_TEMPLATE);

//   // WSContentSpaceButton(request, BUTTON_RESET_CONFIGURATION);
//   // WSContentButton(request, BUTTON_BACKUP);
//   // WSContentButton(request, BUTTON_RESTORE);

//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleTemplateConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // if (pWebServer->hasParam("save")) {
//   //   TemplateSaveSettings();
//   //   WebRestart(1);
//   //   return;
//   // }

//   // char stemp[20];                                           // Template number and Sensor name

//   // if (pWebServer->hasParam("m")) {
//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   for (uint8_t i = 0; i < sizeof(ModuleList_IDs); i++) {  // "}2'%d'>%s (%d)}3" - "}2'0'>Sonoff Basic (1)}3"
//   //     uint8_t midx = pgm_read_byte(ModuleList_IDs + i);
//   //     WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, midx, tkr_sup->AnyModuleName(midx).c_str(), midx +1);
//   //   }
//   //   WSContentEnd(request);
//   //   return;
//   // }

//   // WebGetArg(request,"t", stemp, sizeof(stemp));                     // 0 - 69 Template number
//   // if (strlen(stemp)) {
//   //   uint8_t module = atoi(stemp);
//   //   uint8_t module_save = tkr_set->Settings.module;
//   //   tkr_set->Settings.module = module;
//   //   myio cmodule;
//   //   tkr_sup->TemplateGPIOs(&cmodule);
//   //   gpio_flag flag = tkr_sup->ModuleFlag();
//   //   tkr_set->Settings.module = module_save;

//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   WSBufferAppend_P(response, PSTR("%s}1"), tkr_sup->AnyModuleName(module).c_str());  // NAME: Generic
//   //   for (uint8_t i = 0; i < sizeof(kGpioNiceList); i++) {   // GPIO: }2'0'>None (0)}3}2'17'>Button1 (17)}3...

//   //     if (1 == i) {
//   //       WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, 255, D_SENSOR_USER, 255);  // }2'255'>User (255)}3
//   //     }

//   //     uint8_t midx = pgm_read_byte(kGpioNiceList + i);
//   //     WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, midx, tkr_sup->GetTextIndexed_P(stemp, sizeof(stemp), midx, kSensorNames), midx);
//   //   }

//   //   WSBufferAppend_P(response, PSTR("}1"));                                   // Field separator
//   //   for (uint8_t i = 0; i < sizeof(cmodule); i++) {         // 17,148,29,149,7,255,255,255,138,255,139,255,255
//   //     if ((i < 6) || ((i > 8) && (i != 11))) {              // Ignore flash pins GPIO06, 7, 8 and 11
//   //       WSBufferAppend_P(response, PSTR("%s%d"), (i>0)?",":"", cmodule.io[i]);
//   //     }
//   //   }
//   //   WSBufferAppend_P(response, PSTR("}1%d}1%d"), flag, tkr_set->Settings.user_template_base);  // FLAG: 1  BASE: 17
//   //   WSContentEnd(request);
//   //   return;
//   // }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_TEMPLATE);

//   // WSStartAppend_P(S_CONFIGURE_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_TEMPLATE);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_TABLE100);
//   // WSBufferAppend_P(response, PSTR("<tr><td><b>" D_TEMPLATE_NAME "</b></td><td style='width:200px'><input id='s1' name='s1' placeholder='" D_TEMPLATE_NAME "'></td></tr>"
//   //                      "<tr><td><b>" D_BASE_TYPE "</b></td><td><select id='g99' name='g99' onchange='st(this.value)'></select></td></tr>"
//   //                      "</table>"
//   //                      "<hr/>"));
//   // WSBufferAppend_P(response, HTTP_TABLE100);
//   // for (uint8_t i = 0; i < 17; i++) {
//   //   if ((i < 6) || ((i > 8) && (i != 11))) {                // Ignore flash pins GPIO06, 7, 8 and 11
//   //     WSBufferAppend_P(response, PSTR("<tr><td><b><font color='#%06x'>" D_GPIO "%d</font></b></td><td%s><select id='g%d' name='g%d'></select></td></tr>"),
//   //       ((9==i)||(10==i)) ? WebColor(tkr_set->COL_TEXT_WARNING) : WebColor(tkr_set->COL_TEXT), i, (0==i) ? " style='width:200px'" : "", i, i);
//   //   }
//   // }
//   // WSBufferAppend_P(response, PSTR("</table>"));
//   // WSBufferAppend_P(response, HTTP_FORM_TEMPLATE_FLAG);
//   // WSBufferAppend_P(response, HTTP_FORM_END);
//   // WSContentSpaceButton(BUTTON_CONFIGURATION);
//   // WSContentStop();
// }

// void mWebServer::TemplateSaveSettings(void)
// {
//   // char tmp[sizeof(tkr_set->Settings.user_template.name)];            // WebGetArg NAME and GPIO/BASE/FLAG byte value
//   // char webindex[5];                                         // WebGetArg name
//   // char svalue[128];                                         // Template command string

//   // WebGetArg(request,"s1", tmp, sizeof(tmp));                        // NAME
//   // snprintf_P(svalue, sizeof(svalue), PSTR(D_TEMPLATE " {\"" D_NAME "\":\"%s\",\"" D_GPIO "\":["), tmp);

//   // uint8_t j = 0;
//   // for (uint8_t i = 0; i < sizeof(tkr_set->Settings.user_template.hardware.gp); i++) {
//   //   if (6 == i) { j = 9; }
//   //   if (8 == i) { j = 12; }
//   //   snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), j);
//   //   WebGetArg(webindex, tmp, sizeof(tmp));                  // GPIO
//   //   uint8_t gpio = atoi(tmp);
//   //   snprintf_P(svalue, sizeof(svalue), PSTR("%s%s%d"), svalue, (i>0)?",":"", gpio);
//   //   j++;
//   // }

//   // uint8_t flag = 0;
//   // for (uint8_t i = 0; i < GPIO_FLAG_USED; i++) {
//   //   snprintf_P(webindex, sizeof(webindex), PSTR("c%d"), i);
//   //   uint8_t state = pWebServer->hasParam(webindex) << i;       // FLAG
//   //   flag += state;
//   // }
//   // WebGetArg(request,"g99", tmp, sizeof(tmp));                       // BASE
//   // uint8_t base = atoi(tmp) +1;

//   // snprintf_P(svalue, sizeof(svalue), PSTR("%s],\"" D_FLAG "\":%d,\"" D_BASE "\":%d}"), svalue, flag, base);
//   // ExecuteWebCommand(svalue, SRC_WEBGUI);
// }

// /*-------------------------------------------------------------------------------------------*/

// void mWebServer::HandleModuleConfiguration(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // if (pWebServer->hasParam("save")) {
//   //   ModuleSaveSettings();
//   //   //WebRestart(1);
//   //   return;
//   // }

//   // char stemp[20];  // Sensor name
//   // uint8_t midx;
//   // myio cmodule;
//   // tkr_sup->TemplateGPIOs(&cmodule);

//   // if (pWebServer->hasParam("m")) {
//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   uint8_t vidx = 0;
//   //   for (uint8_t i = 0; i <= sizeof(ModuleList_IDs); i++) {  // "}2'%d'>%s (%d)}3" - "}2'255'>UserTemplate (0)}3" - "}2'0'>Sonoff Basic (1)}3"
//   //     if (0 == i) {
//   //       midx = USER_MODULE;
//   //       vidx = 0;
//   //     } else {
//   //       midx = pgm_read_byte(ModuleList_IDs + (i-1)); // -1 to offset USER_MODULE
//   //       vidx = midx +1;
//   //     }
//   //     //ALOG_TST(PSTR(D_LOG_HTTP "hasParam(\"m\"),i=%d,midx=%d,vidx=%d"),i,midx,vidx);
//   //     WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, 
//   //       midx, 
//   //       tkr_sup->AnyModuleName(midx).c_str(), 
//   //       vidx
//   //     );
//   //   }
//   //   WSContentEnd(request);

//   //   ALOG_TST(PSTR(D_LOG_HTTP "sizeof(ModuleList_IDs)=%d"),sizeof(ModuleList_IDs));
    
//   //   return;
//   // }

//   // if (pWebServer->hasParam("g")) {
//   //  WSContentBegin(request, 200, CT_PLAIN);
//   //   for (uint8_t j = 0; j < sizeof(kGpioNiceList); j++) {
//   //     midx = pgm_read_byte(kGpioNiceList + j);
//   //     if (!tkr_sup->GetUsedInModule(midx, cmodule.io)) {
//   //       WSBufferAppend_P(response, HTTP_MODULE_TEMPLATE_REPLACE, 
//   //                       midx, 
//   //                       tkr_sup->GetTextIndexed_P(stemp, sizeof(stemp), midx, kSensorNames), 
//   //                       midx
//   //                     );

//   //     // ALOG_TST(PSTR(D_LOG_HTTP "GetTextIndexed_P=%s,midx=%d,j=%d"),
//   //     // tkr_sup->GetTextIndexed_P(stemp, sizeof(stemp), midx, kSensorNames),midx,j);
        
//   //     }
//   //   }
//   //   WSContentEnd(request);
//   //   return;
//   // }

//   // ALOG_DBG(S_LOG_HTTP, S_CONFIGURE_MODULE);

//   // ALOG_TST(PSTR(D_LOG_HTTP "tkr_set->Settings.module=%d,sizeof(cmodule)=%d"),tkr_set->Settings.module,sizeof(cmodule));
  

//   // WSStartAppend_P(S_CONFIGURE_MODULE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE_TEMPLATE);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE1, tkr_set->Settings.module);
//   // for (uint8_t i = 0; i < sizeof(cmodule); i++) {
//   //   if (tkr_sup->ValidGPIO(i, cmodule.io[i])) {
//   //     WSBufferAppend_P(response, PSTR("sk(%d,%d);"), tkr_set->my_module.io[i], i);  // g0 - g16
//   //   }
//   // }
//   // WSBufferAppend_P(response, HTTP_SCRIPT_MODULE2);
//   // WSContentSendStyle(request);
  
//   // // ALOG_TST(PSTR(D_LOG_HTTP "AnyModuleName=%s,%d,%d"),tkr_sup->AnyModuleName(MODULE).c_str(),MODULE,sizeof(cmodule));

//   // uint8_t show_pin_names = false;
//   // if((tkr_set->Settings.module==MODULE_WEMOS)||(tkr_set->Settings.module==MODULE_NODEMCU)){
//   //   show_pin_names = true;
//   // }

//   // WSBufferAppend_P(response, HTTP_FORM_MODULE, tkr_sup->AnyModuleName(tkr_set->Settings.module).c_str());
//   // for (uint8_t i = 0; i < sizeof(cmodule); i++) {
//   //   if (tkr_sup->ValidGPIO(i, cmodule.io[i])) {
//   //     snprintf_P(stemp, 3, PINS_WEMOS +i*2);
//   //     char sesp8285[40];
//   //     snprintf_P(sesp8285, sizeof(sesp8285), PSTR("<font color='#%06x'>ESP8285</font>"), WebColor(tkr_set->COL_TEXT_WARNING));
//   //     WSBufferAppend_P(response, PSTR("<tr><td style='width:190px'>%s <b>" D_GPIO "%d</b> %s</td><td style='width:176px'><select id='g%d' name='g%d'></select></td></tr>"),
//   //       show_pin_names?stemp:"", 
//   //       i,
//   //       (0==i)? D_SENSOR_BUTTON "1":
//   //       (1==i)? D_SERIAL_OUT :
//   //       (3==i)? D_SERIAL_IN :
//   //       ((9==i)||(10==i))? sesp8285 :
//   //       (12==i)? D_SENSOR_RELAY "1":
//   //       (13==i)? D_SENSOR_LED "1i":
//   //       (14==i)? D_SENSOR :"",
//   //       i,
//   //       i
//   //     );
//   //   }
//   // }
//   // WSBufferAppend_P(response, PSTR("</table>"));
//   // WSBufferAppend_P(response, HTTP_FORM_END);
//   // WSContentSpaceButton(BUTTON_CONFIGURATION);
//   // WSContentStop();
// }



// void mWebServer::ModuleSaveSettings(AsyncWebServerRequest *request)
// {
//   // char tmp[8];         // WebGetArg numbers only
//   // char webindex[5];    // WebGetArg name

//   // WebGetArg(request,"g99", tmp, sizeof(tmp));
//   // uint8_t new_module = (!strlen(tmp)) ? MODULE : atoi(tmp);
//   // tkr_set->Settings.last_module = tkr_set->Settings.module;
//   // tkr_set->Settings.module = new_module;
//   // tkr_sup->SetModuleType();
//   // myio cmodule;
//   // tkr_sup->TemplateGPIOs(&cmodule);
//   // String gpios = "";
//   // for (uint8_t i = 0; i < sizeof(cmodule); i++) {
//   //   if (tkr_set->Settings.last_module != new_module) {
//   //     tkr_set->Settings.module_pins.io[i] = GPIO_NONE;
//   //   } else {
//   //     if (tkr_sup->ValidGPIO(i, cmodule.io[i])) {
//   //       snprintf_P(webindex, sizeof(webindex), PSTR("g%d"), i);
//   //       WebGetArg(webindex, tmp, sizeof(tmp));
//   //       tkr_set->Settings.module_pins.io[i] = (!strlen(tmp)) ? 0 : atoi(tmp);
//   //       gpios += F(", " D_GPIO ); gpios += String(i); gpios += F(" "); gpios += String(tkr_set->Settings.module_pins.io[i]);
//   //     }
//   //   }
//   // }
//   // ALOG_INF(PSTR(D_LOG_MODULE "%s " D_MODULE "%s"), tkr_sup->ModuleName().c_str(), gpios.c_str());
// }


// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * System Settings
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */


// void mWebServer::HandleSystemSettings(AsyncWebServerRequest *request)
// {
//   if (!HttpCheckPriviledgedAccess()) { return; }

//   if (request->hasParam("rst")) {
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_DEV_TEST,PSTR("hasParam rst"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//     WebRestart(request, 0);
//     return;
//   }
  
//   // ALOG_DBG(S_LOG_HTTP, S_SYSTEM_SETTINGS);

//   // data_buffer.ClearDeep();
//   // char *buf = data_buffer.payload.ctr;
//   // char **buffer = &buf;
//   // buffer_writer_len = 0;

//   // ALOG_INF(PSTR(D_LOG_HTTP D_CONSOLE));

//   // WSStartAppend_P2(buffer, S_SYSTEM_SETTINGS);

//   // //? basic style
//   // WSBufferAppend_P2(buffer, "<style>");
//   // tkr_web->WSBufferAppend_P2(buffer, PM_HTTP_HEAD_STYLE1_MINIMAL,
//   //   tkr_web->WebColor(tkr_set->COL_FORM), 
//   //   tkr_web->WebColor(tkr_set->COL_BACKGROUND),
//   //   tkr_web->WebColor(tkr_set->COL_BUTTON),  
//   //   tkr_web->WebColor(tkr_set->COL_BUTTON_TEXT)
//   // );
//   // WSBufferAppend_P2(buffer, "</style>");

//   // WSBufferAppend_P2(buffer, "</head>" "<body>");
//   // WSBufferAppend_P2(buffer, PSTR("<div class='tb_con'></div>"));
//   // WSBufferAppend_P2(buffer, HTTP_STYLE_TITLE_CONTAINER_HEAD, 
//   //   WebColor(tkr_set->COL_TEXT),
//   //   WebColor(tkr_set->COL_TEXT_MODULE_TITLE), 
//   //   //PROJECT_NAME_CTR, 
//   //   tkr_set->Settings.system_name.friendly
//   // );
//   //   // tkr->Tasker_Interface(TASK_WEB_ADD_BUTTON_SYSTEM_SETTINGS);

//   //   WSButtonAppend2(buffer, BUTTON_CONFIGURATION);
//   //   WSButtonAppend2(buffer, BUTTON_INFORMATION);
//   //   WSButtonAppend2(buffer, BUTTON_FIRMWARE_UPGRADE);
//   //   WSButtonAppend2(buffer, BUTTON_RESTART);

//   // WSSpaceButtonAppend2(buffer, BUTTON_MAIN);
  
//   // // End of body
//   // WSBufferAppend_SystemVersionBar(buffer);

//   // WSBufferAppend_P2(buffer, PSTR("<link rel='stylesheet' href='" WEB_HANDLER_STYLE_WEB_BASIC_ROOT "'>"));

//   // WSBufferAppend_P2(buffer, "</body></html>");
  

// }


// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Restart and Update
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */



// void mWebServer::WebRestart(AsyncWebServerRequest *request, uint8_t type)
// {
//   // type 0 = restart
//   // type 1 = restart after config change
//   // type 2 = restart after config change with possible ip address change too
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(S_LOG_HTTP, S_RESTART);
//     #endif// ENABLE_LOG_LEVEL_INFO

//   // bool reset_only = (HTTP_MANAGER_RESET_ONLY == webserver_state);

//   // WSStartAppend_P(request, (type) ? S_SAVE_CONFIGURATION : S_RESTART, !reset_only);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_RELOAD);
//   // WSContentSendStyle(request);
//   // if (type) {
//   //   WSBufferAppend_P(response, PSTR("<div style='text-align:center;'><b>" D_CONFIGURATION_SAVED "</b><br/>"));
//   //   if (2 == type) {
//   //     WSBufferAppend_P(response, PSTR("<br/>" D_TRYING_TO_CONNECT "<br/>"));
//   //   }
//   //   WSBufferAppend_P(response, PSTR("</div>"));
//   // }
//   // WSBufferAppend_P(response, HTTP_MSG_RSTRT);
//   // if (HTTP_MANAGER == webserver_state || reset_only) {
//   //   webserver_state = HTTP_ADMIN;
//   // } else {
//   //   WSContentSpaceButton(request, BUTTON_MAIN);
//   // }
//   // WSContentStop(request);

//   // ShowWebSource(SRC_WEBGUI);
//   tkr_set->restart_flag = 2;
// }


// void mWebServer::HandleWifiLogin(AsyncWebServerRequest *request)
// {
// //   WSStartAppend_P(request, S_CONFIGURE_WIFI, false);  // false means show page no matter if the client has or has not credentials
// //   WSContentSendStyle(request);
// //   WSBufferAppend_P(response, HTTP_FORM_LOGIN);

// //   if (HTTP_MANAGER_RESET_ONLY == webserver_state) {
// //     WSContentSpaceButton(request, BUTTON_RESTART);
// // #ifndef FIRMWARE_MINIMAL
// //     WSContentSpaceButton(request, BUTTON_RESET_CONFIGURATION);
// // #endif  // FIRMWARE_MINIMAL
// //   }

// //   WSContentStop(request);
// }


// void mWebServer::HandleUpgradeFirmware(AsyncWebServerRequest *request)
// {
//   // if (!HttpCheckPriviledgedAccess()) { return; }

//   // ALOG_DBG(S_LOG_HTTP, S_FIRMWARE_UPGRADE);

//   // WSStartAppend_P(request, S_FIRMWARE_UPGRADE);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, HTTP_FORM_UPG, tkr_set->Settings.ota_url);
//   // WSBufferAppend_P(response, HTTP_FORM_RST_UPG, D_UPGRADE);
//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);

//   // upload_error = 0;
//   // upload_file_type = UPL_TASMOTA;
// }

// void mWebServer::HandleUpgradeFirmwareStart(AsyncWebServerRequest *request)
// {
//   // //if (!HttpCheckPriviledgedAccess()) { return; }

//   // char command[sizeof(tkr_set->Settings.ota_url) + 10];  // OtaUrl

//   // ALOG_DBG(PSTR(D_LOG_HTTP D_UPGRADE_STARTED));
//   // tkr_wifi->WiFi_Config_ConnectWindow_Expired();

//   // char otaurl[sizeof(tkr_set->Settings.ota_url)];
//   // WebGetArg(request,"o", otaurl, sizeof(otaurl));
//   // if (strlen(otaurl)) {
//   //   snprintf_P(command, sizeof(command), PSTR(D_OTAURL " %s"), otaurl);
//   //   ExecuteWebCommand(command, SRC_WEBGUI);
//   // }

//   // WSStartAppend_P(request, PM_INFORMATION);
//   // WSBufferAppend_P(response, HTTP_SCRIPT_RELOAD_OTA);
//   // WSContentSendStyle(request);
//   // WSBufferAppend_P(response, PSTR("<div style='text-align:center;'><b>" D_UPGRADE_STARTED " ...</b></div>"));
//   // WSBufferAppend_P(response, HTTP_MSG_RSTRT);
//   // WSContentSpaceButton(request, BUTTON_MAIN);
//   // WSContentStop(request);

//   // snprintf_P(command, sizeof(command), PSTR(D_UPGRADE " 1"));
//   // ExecuteWebCommand(command, SRC_WEBGUI);
// }

// void mWebServer::HandleUploadDone(AsyncWebServerRequest *request)
// {
// //   if (!HttpCheckPriviledgedAccess()) { return; }

// //   ALOG_DBG(PSTR(D_LOG_HTTP D_UPLOAD_DONE));

// //   char error[100];

// //   tkr_wifi->WiFi_Config_ConnectWindow_Expired();
// //   tkr_set->restart_flag = 0;
// //   //MqttRetryCounter(0);

// //   WSStartAppend_P(request, PM_INFORMATION);
// //   if (!upload_error) {
// //     WSBufferAppend_P(response, HTTP_SCRIPT_RELOAD_OTA);  // Refesh main web ui after OTA upgrade
// //   }
// //   WSContentSendStyle(request);
// //   WSBufferAppend_P(response, PSTR("<div style='text-align:center;'><b>" D_UPLOAD " <font color='#"));
// //   if (upload_error) {
// // //    WSBufferAppend_P(response, PSTR(COLOR_TEXT_WARNING "'>" D_FAILED "</font></b><br/><br/>"));
// //     WSBufferAppend_P(response, PSTR("%06x'>" D_FAILED "</font></b><br/><br/>"), WebColor(tkr_set->COL_TEXT_WARNING));
// // #ifdef USE_RF_FLASH
// //     if (upload_error < 14) {
// // #else
// //     if (upload_error < 10) {
// // #endif
// //       tkr_sup->GetTextIndexed_P(error, sizeof(error), upload_error -1, kUploadErrors);
// //     } else {
// //       snprintf_P(error, sizeof(error), PSTR(D_UPLOAD_ERROR_CODE " %d"), upload_error);
// //     }
// //     WSBufferAppend_P(response, error);
// //     ALOG_DBG(PSTR(D_UPLOAD ": %s"), error);
// //     tkr_set->stop_flash_rotate = tkr_set->Settings.flag_system.stop_flash_rotate;
// //   } else {
// //     WSBufferAppend_P(response, PSTR("%06x'>" D_SUCCESSFUL "</font></b><br/>"), WebColor(tkr_set->COL_TEXT_SUCCESS));
// //     WSBufferAppend_P(response, HTTP_MSG_RSTRT);
// //     ShowWebSource(SRC_WEBGUI);
// //     tkr_set->restart_flag = 2;  // Always restart to re-enable disabled features during update
// //   }
// //   //SettingsBufferFree();
// //   WSBufferAppend_P(response, PSTR("</div><br/>"));
// //   WSContentSpaceButton(request, BUTTON_MAIN);
// //   WSContentStop(request);
// }

// void mWebServer::HandleUploadLoop(AsyncWebServerRequest *request)
// {
  
// //   //AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HandleUploadLoop1"));
// //   // Based on ESP8266HTTPUpdateServer.cpp uses ESP8266WebServer Parsing.cpp and Cores Updater.cpp (Update)
// //   bool _serialoutput = (LOG_LEVEL_DEBUG <= tkr_set->seriallog_level);

// //   // if (HTTP_USER == webserver_state) { 
// //   //   AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HTTP_USER == webserver_state"));
// //   //   return; 
// //   // }
// //   if (upload_error) {
// //     AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "upload_error"));
// //     #ifdef ESP8266
// //       if (UPL_TASMOTA == upload_file_type) { Update.end(); }
// //     #endif
// //     return;
// //   }

// //   //AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "HandleUploadLoop2"));

// //   HTTPUpload& upload = pWebServer->upload();

// //   //AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "upload.status=%d"),upload.status);

// //   if (UPLOAD_FILE_START == upload.status) { 

// //     tkr_set->restart_flag = 60;
// //     if (0 == upload.filename.c_str()[0]) {
// //       upload_error = 1;  // No file selected
// //       return;
// //     }
// //     //SettingsSave(1);  // Free flash for upload
// //     AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD D_FILE " %s ..."), upload.filename.c_str());
// //     if (UPL_SETTINGS == upload_file_type) {
// //       if (!tkr_set->SettingsBufferAlloc()) {
// //         upload_error = 2;  // Not enough space
        
// //   AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "Not enough space"));
// //         return;
// //       }
// //     } else {
// //      // MqttRetryCounter(60);
// // #ifdef USE_EMULATION
// //       //UdpDisconnect();
// // #endif  // USE_EMULATION

// //       //if (tkr_set->Settings.flag_system.mqtt_enabled) MqttDisconnect();
// //       uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      
// //   AddSerialLog_mP2(LOG_LEVEL_INFO, PSTR(D_LOG_HTTP "maxSketchSpace=%d"),maxSketchSpace);

// //           #ifdef ESP8266
// //       if (!Update.begin(maxSketchSpace)) {         //start with max available size

// //        if (_serialoutput) Update.printError(Serial);
// //        if (Update.getError() == UPDATE_ERROR_BOOTSTRAP) {
// //          if (_serialoutput) Serial.println("Device still in UART update mode, perform powercycle");
// //        }

// //         upload_error = 2;  // Not enough space
// //         return;
// //       }
// //       #endif
// //     }
// //     upload_progress_dot_count = 0;
// //   } else if (!upload_error && (UPLOAD_FILE_WRITE == upload.status)) {
// //     if (0 == upload.totalSize) {
// //       if (UPL_SETTINGS == upload_file_type) {
// //         config_block_count = 0;
// //       }
// //       else {
// // // #ifdef USE_RF_FLASH
// // //         if ((SONOFF_BRIDGE == tkr_set->my_module_type) && (upload.buf[0] == ':')) {  // Check if this is a RF bridge FW file
// // //           Update.end();              // End esp8266 update session
// // //           upload_file_type = UPL_EFM8BB1;
// // //
// // //           upload_error = SnfBrUpdateInit();
// // //           if (upload_error != 0) { return; }
// // //         } else
// // // #endif  // USE_RF_FLASH
// //         {
// //           if (upload.buf[0] != 0xE9) {
// //             upload_error = 3;  // Magic byte is not 0xE9
// //             return;
// //           }
// //           uint32_t bin_flash_size = ESP.magicFlashChipSize((upload.buf[3] & 0xf0) >> 4);
// //           #ifdef ESP8266
// //           if(bin_flash_size > ESP.getFlashChipRealSize()) {
// //             upload_error = 4;  // Program flash size is larger than real flash size
// //             return;
// //           }
// //           #endif
// // //          upload.buf[2] = 3;  // Force DOUT - ESP8285
// //         }
// //       }
// //     }
// //     if (UPL_SETTINGS == upload_file_type) {
// //       if (!upload_error) {
// //         if (upload.currentSize > (sizeof(tkr_set->Settings) - (config_block_count * HTTP_UPLOAD_BUFLEN))) {
// //           upload_error = 9;  // File too large
// //           return;
// //         }
// //         memcpy(tkr_set->settings_buffer + (config_block_count * HTTP_UPLOAD_BUFLEN), upload.buf, upload.currentSize);
// //         config_block_count++;
// //       }
// //     }
// // // #ifdef USE_RF_FLASH
// // //     else if (UPL_EFM8BB1 == upload_file_type) {
// // //       if (efm8bb1_update != nullptr) {    // We have carry over data since last write, i. e. a start but not an end
// // //         ssize_t result = rf_glue_remnant_with_new_data_and_write(efm8bb1_update, upload.buf, upload.currentSize);
// // //         free(efm8bb1_update);
// // //         efm8bb1_update = nullptr;
// // //         if (result != 0) {
// // //           upload_error = abs(result);  // 2 = Not enough space, 8 = File invalid
// // //           return;
// // //         }
// // //       }
// // //       ssize_t result = rf_search_and_write(upload.buf, upload.currentSize);
// // //       if (result < 0) {
// // //         upload_error = abs(result);
// // //         return;
// // //       } else if (result > 0) {
// // //         if ((size_t)result > upload.currentSize) {
// // //           // Offset is larger than the buffer supplied, this should not happen
// // //           upload_error = 9;  // File too large - Failed to decode RF firmware
// // //           return;
// // //         }
// // //         // A remnant has been detected, allocate data for it plus a null termination byte
// // //         size_t remnant_sz = upload.currentSize - result;
// // //         efm8bb1_update = (uint8_t *) malloc(remnant_sz + 1);
// // //         if (efm8bb1_update == nullptr) {
// // //           upload_error = 2;  // Not enough space - Unable to allocate memory to store new RF firmware
// // //           return;
// // //         }
// // //         memcpy(efm8bb1_update, upload.buf + result, remnant_sz);
// // //         // Add null termination at the end of of remnant buffer
// // //         efm8bb1_update[remnant_sz] = '\0';
// // //       }
// // //     }
// // // #endif  // USE_RF_FLASH
// //     else {  // firmware
    
// //           #ifdef ESP8266
// //       if (!upload_error && (Update.write(upload.buf, upload.currentSize) != upload.currentSize)) {
// //         upload_error = 5;  // Upload buffer miscompare
// //         return;
// //       }
// //       #endif
// //       if (_serialoutput) {
// //         Serial.printf(".");
// //         upload_progress_dot_count++;
// //         if (!(upload_progress_dot_count % 80)) { Serial.println(); }
// //       }
// //     }
// //   } else if(!upload_error && (UPLOAD_FILE_END == upload.status)) {
// //     if (_serialoutput && (upload_progress_dot_count % 80)) {
// //       Serial.println();
// //     }
// //     if (UPL_SETTINGS == upload_file_type) {
// //       if (config_xor_on_set) {
// //         for (uint16_t i = 2; i < sizeof(tkr_set->Settings); i++) {
// //           tkr_set->settings_buffer[i] ^= (config_xor_on_set +i);
// //         }
// //       }
// //       bool valid_settings = false;
// //       unsigned long buffer_version = tkr_set->settings_buffer[11] << 24 | tkr_set->settings_buffer[10] << 16 | tkr_set->settings_buffer[9] << 8 | tkr_set->settings_buffer[8];
// //       if (buffer_version > 0x06000000) {
// //         uint16_t buffer_size = tkr_set->settings_buffer[3] << 8 | tkr_set->settings_buffer[2];
// //         uint16_t buffer_crc = tkr_set->settings_buffer[15] << 8 | tkr_set->settings_buffer[14];
// //         uint16_t crc = 0;
// //         for (uint16_t i = 0; i < buffer_size; i++) {
// //           if ((i < 14) || (i > 15)) { crc += tkr_set->settings_buffer[i]*(i+1); }  // Skip crc
// //         }
// //         valid_settings = (buffer_crc == crc);
// //       } else {
// //         valid_settings = (tkr_set->settings_buffer[0] == CONFIG_FILE_SIGN);
// //       }
// //       if (valid_settings) {
// //         //SystemSettings_DefaultBody();
// //         memcpy((char*)&tkr_set->Settings +16, tkr_set->settings_buffer +16, sizeof(tkr_set->Settings) -16);
// //         tkr_set->Settings.version = buffer_version;  // Restore version and auto upgrade after restart
// //         //SettingsBufferFree();
// //       } else {
// //         upload_error = 8;  // File invalid
// //         return;
// //       }
// //     }
// // #ifdef USE_RF_FLASH
// //     else if (UPL_EFM8BB1 == upload_file_type) {
// //       // RF FW flash done
// //       upload_file_type = UPL_TASMOTA;
// //     }
// // #endif  // USE_RF_FLASH
// //     else {
// //           #ifdef ESP8266
// //       if (!Update.end(true)) { // true to set the size to the current progress
// //         if (_serialoutput) { Update.printError(Serial); }
// //         upload_error = 6;  // Upload failed. Enable logging 3
// //         return;
// //       }
// //       #endif
// //     }
// //     if (!upload_error) {
// //       ALOG_INF(PSTR(D_LOG_UPLOAD D_SUCCESSFUL " %u bytes. " D_RESTARTING), upload.totalSize);
// //     }
// //   } else if (UPLOAD_FILE_ABORTED == upload.status) {
// //     tkr_set->restart_flag = 0;
// //     //MqttRetryCounter(0);
// //     upload_error = 7;  // Upload aborted
// //     #ifdef ESP8266
// //       if (UPL_TASMOTA == upload_file_type) { Update.end(); }
// //     #endif
// //   }
// //   delay(0);
// }



// /*************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//    * Handle Not Found
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
//   ************************************************************************************************************************************************************************************
// */

// void mWebServer::HandleNotFound(AsyncWebServerRequest *request)
// {
//     #ifdef ENABLE_LOG_LEVEL_INFO
//   ALOG_DBG(PSTR(D_LOG_HTTP "Not found (%s)"), request->url().c_str());
//     #endif// ENABLE_LOG_LEVEL_INFO

//   // if (CaptivePortal(request)) { return; }  // If captive portal redirect instead of displaying the error page.

//   // AsyncResponseStream *response = request->beginResponseStream(404,"html/text");//, root.measureJson() + 1);
  
//   // // WSBufferAppend_P(response, PSTR(D_FILE_NOT_FOUND "\n\nURI: %s\nMethod: %s\nArguments: %d\n"), 
//   // //   request->uri().c_str(), (request->method() == HTTP_GET) ? "GET" : "POST", request->args());

//   // for (uint8_t i = 0; i < request->args(); i++) {
//   //   WSBufferAppend_P(response, PSTR(" %s: %s\n"), request->argName(i).c_str(), request->arg(i).c_str());
//   // }
//   char message[45];
//     sprintf_P(message,PSTR("Missing"));// too low - %d needed (%d free)"),memory_needed,ESP.getFreeHeap());
//     request->send(404, CONTENT_TYPE_TEXT_HTML_ID, message);

//   // No subscriber handled the request, return a 404 with implicit "Connection: close"
//   // request->send(404);

// }





// /*****************************************************************************************************************************************************************************************************************
//  **************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ********HELPERS FUNCTION ONLY BELOW********************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************
//  *************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  *****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************/


// void mWebServer::WebAppend_Start_Head_P(const char* title)
// {
//   WebAppend_Start_Head_P(title, true);
// }
// void mWebServer::WebAppend_Start_Head_P(const char* title, bool auth)
// {
//   // if (auth && (tkr_set->Settings.web_password[0] != 0) && !pWebServer->authenticate(WEB_USERNAME, tkr_set->Settings.web_password)) {
//   //  return pWebServer->requestAuthentication();
//   // }

//   // if (title != nullptr) {
//   //   char ctitle[strlen_P(title) +1];
//   //   strcpy_P(ctitle, title);                       // Get title from flash to RAM
    
//   //   #ifdef ESP8266
//   //     BufferWriterI->Append_P(PM_HTTP_HEAD3, tkr_set->Settings.system_name.friendly, ctitle);
//   //   #else
//   //     BufferWriterI->Append_P(HTTP_HEAD2, tkr_set->Settings.system_name.friendly, ctitle);
//   //   #endif

//   // }
// }

// void mWebServer::WebAppend_Button_Spaced(uint8_t title_index)
// {
//   BufferWriterI->Append_P(PSTR("<div></div>"));            // 5px padding
//   WebAppend_Button(title_index);
// }
// void mWebServer::WebAppend_Button(uint8_t title_index)
// {
//   char action[20];
//   char title[32];
//   if (title_index <= BUTTON_RESET_CONFIGURATION) {
//     char confirm[64];
//     BufferWriterI->Append_P(PSTR("<p><form action='%s' method='get' onsubmit='return confirm(\"%s\");'><button name='%s' class='buttonh bred'>%s</button></form></p>"),
//       tkr_sup->GetTextIndexed_P(action, sizeof(action), title_index, kButtonAction),
//       tkr_sup->GetTextIndexed_P(confirm, sizeof(confirm), title_index, kButtonConfirm),
//       (!title_index) ? "rst" : "non",
//       tkr_sup->GetTextIndexed_P(title, sizeof(title), title_index, kButtonTitle));
//   } else {
//     BufferWriterI->Append_P(PSTR("<p><form action='%s' method='get'><button class='buttonh'>%s</button></form></p>"),
//       tkr_sup->GetTextIndexed_P(action, sizeof(action), title_index, kButtonAction),
//       tkr_sup->GetTextIndexed_P(title, sizeof(title), title_index, kButtonTitle));
//   }
// }


// // MainButton? PageLoadButton?
// void mWebServer::WebAppend_Button_Spaced(const char* button_title_ctr, const char* action_ctr, const char* confirm_message_ctr, const char* confirm_action_ctr)
// {
//   BufferWriterI->Append_P(PSTR("<div></div>"));            // 5px padding
//   WebAppend_Button(button_title_ctr, action_ctr, confirm_message_ctr, confirm_action_ctr);
// }
// void mWebServer::WebAppend_Button(const char* button_title_ctr, const char* action_ctr, const char* confirm_message_ctr, const char* confirm_action_ctr)
// {
//   // long press button options?
//   // check for confirmation popout needed
//   if(confirm_message_ctr != nullptr){
//     char confirm[64];
//     BufferWriterI->Append_P(PSTR(
//       "<p>"
//         "<form action='%s' method='get' onsubmit='return confirm(\"%s\");'>"
//           "<button name='%s' class='buttonh bred'>%s</button>"
//         "</form>"
//       "</p>"),
//       action_ctr,
//       confirm_message_ctr,
//       confirm_action_ctr!=nullptr?confirm_action_ctr:"non",//(!title_index) ? "rst" : "non", //leave for now, but add passing button colour, else assume default
//       button_title_ctr
//     );
//   } else {
//     BufferWriterI->Append_P(PSTR("<p><form action='%s' method='get'><button class='buttonh'>%s</button></form></p>"),
//       action_ctr,
//       button_title_ctr
//     );
//   }
// }


// void mWebServer::WebAppend_Button2(const char* button_title_ctr, const char* action_ctr, const char* button_css_extra_style_ctr)
// {
//   // long press button options?
//   // check for confirmation popout needed
//   // if(confirm_message_ctr != nullptr){
//   //   char confirm[64];
//   //   BufferWriterI->Append_P(PSTR(
//   //     "<p>"
//   //       "<form action='%s' method='get' onsubmit='return confirm(\"%s\");'>"
//   //         "<button name='%s' class='buttonh bred'>%s</button>"
//   //       "</form>"
//   //     "</p>"),
//   //     action_ctr,
//   //     confirm_message_ctr,
//   //     confirm_action_ctr!=nullptr?confirm_action_ctr:"non",//(!title_index) ? "rst" : "non", //leave for now, but add passing button colour, else assume default
//   //     button_title_ctr
//   //   );
//   // } else {
//     BufferWriterI->Append_P(PSTR(
      
//       // "<p><form action='%s' method='get'><button>%s</button></form></p>"

//     "<button name='%s' type='submit' class='buttonh %s'>%s</button>"),

//       action_ctr,
//       button_css_extra_style_ctr,
//       button_title_ctr
//     );
  
// }




// /*****************************************************************************************************************************************************************************************************************
//  **************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ******** UNUSED FUNCTION BELOW ********************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************
//  *************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  ***************************************************************************************************************************************************************************************************************** 
//  *****************************************************************************************************************************************************************************************************************
//  ***************************************************************************************************************************************************************************************************************/




// // void mWebServer::HandleInformation(AsyncWebServerRequest *request)
// // {
// //     if (!HttpCheckPriviledgedAccess()) { return; }

// //     ALOG_DBG(S_LOG_HTTP, PM_INFORMATION);

// //     BufferWriterI->Start();

// //     // Head
// //     WebAppend_Start_Head_P(PM_INFORMATION); //<head>
// //         WebAppend_Minimal_Style();
// //         BufferWriterI->Append_P("</head>");

// //         // Body
// //         BufferWriterI->Append_P("<body>");
// //             WebAppend_Title();

// //             // Add information table position
// //             BufferWriterI->Append_P(PSTR("<fieldset><legend><b>&nbsp;Information&nbsp;</b></legend>"));
// //             BufferWriterI->Append_P(PSTR("<style>td{padding:0px 5px;}</style>"
// //                                         "<div class='info_table_draw'></div>"));
// //             BufferWriterI->Append_P(PSTR("</fieldset>"));

// //             WebAppend_Button_Spaced(BUTTON_MAIN);
        
// //             WebAppend_SystemVersionBar();

// //             BufferWriterI->Append_P(PSTR("</div>"));

// //             WebAppend_Script_P(PM_WEBURL_PAGE_INFO_LOAD_SCRIPT);    

// //         BufferWriterI->Append_P("</body>");
// //     BufferWriterI->Append_P("</html>");

// //     WebSend_Response(request,200,CONTENT_TYPE_TEXT_HTML_ID,data_buffer.payload.ctr);
// // }



// // void mWebServer::WebSend_Information_Fetcher_URLs(AsyncWebServerRequest *request){

// //   // // Serial.println(WEB_HANDLER_SCRIPT_INFROMATION_DATA_FETCHER_URLS_RATES_VAR); Serial.flush();      

// //   // data_buffer.ClearDeep();
// //   // char *buf = data_buffer.payload.ctr;
// //   // char **iter = &buf;
// //   // buffer_writer_internal = iter;

// //   // AppendBuffer_PI2(PSTR("const dfurls=["));

// //   //     AppendBuffer_PI2(PSTR("\"%s\","),"/script/parse_jsondata.js");
// //   //     AppendBuffer_PI2(PSTR("\"%s\","),WEB_HANDLER_SCRIPT_WEB_PARSE_ADDSCRIPTSTYLE_FUNCTION);
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),"/style/web_notif_bar.css");
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),"/div/web_drawdiv_root.json");


      
// //   //     AppendBuffer_PI2(PSTR("\"%s\","),"/div/web_info_table.json");
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),WEB_HANDLER_SCRIPT_ROOT_MICHAEL);
// //   //     // AppendBuffer_PI2(PSTR("\"%s\","),"/runtime/data_urls.json");
    
// //   //   // tkr->Tasker_Interface(TASK_WEB_APPEND_LOADTIME_ROOT_URLS);
// //   //   *buffer_writer_internal = (*buffer_writer_internal) - 1;// remove extra comma
// //   // AppendBuffer_PI2(PSTR("];var dfrates=["));



// //   //     AppendBuffer_PI2(PSTR("%d,"),-1);
// //   //     AppendBuffer_PI2(PSTR("%d,"),-50);
// //   //     // AppendBuffer_PI2(PSTR("%d,"),-1000);
// //   //     AppendBuffer_PI2(PSTR("%d,"),-100);
// //   //     // AppendBuffer_PI2(PSTR("%d,"),-1500);
// //   //     // AppendBuffer_PI2(PSTR("%d,"),-2500);
// //   //   // tkr->Tasker_Interface(TASK_WEB_APPEND_LOADTIME_ROOT_RATES);
// //   //   *buffer_writer_internal = (*buffer_writer_internal) - 1;// remove extra comma
// //   // AppendBuffer_PI2(PSTR("];"));

// // }



// // // this table exists on the root page and is there to take over tasmota m=1 duty
// // // when drawn, each module is asked for its row that sends its title and unique number class name
// // // when asked to update, each module will append their data to a shared sender here.
// // void mWebServer::HandleInformation_TableDraw(AsyncWebServerRequest *request){
    
// //     return ;
// //   // if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return;}  
// //   // uint16_t freemem_start = ESP.getFreeHeap();  
  
// //   // data_buffer.ClearDeep();
// //   // char *buf = data_buffer.payload.ctr;
// //   // char **buffer = &buf;

// //   // AppendBuffer_P2(buffer,PSTR("{"));
// //   //   buffer_writer_internal = buffer;
// //   //   // tkr->Tasker_Interface(TASK_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED);
// //   //   // WebAppend_Root_Draw_TopBar();
// //   //   WebAppend_Page_InformationTable();
// //   //   // extra "," is automatically appending for repeated cases across modules, and should be removed
// //   //   *buffer_writer_internal = (*buffer_writer_internal) - 1;// remove extra comma
// //   // AppendBuffer_P2(buffer,PSTR("}"));

// //   // buffer_writer_internal = nullptr; // Anytime I use it, clear to back to null when finished
// // }

// // void mWebServer::WebAppend_Page_InformationTable(){

// //   return ;

// //   // AppendBuffer_PI2("\"%s\":[{\"ihr\":\"","info_table_draw");
// //   // AppendBuffer_PI2("%s","{t}");

// //   // AppendBuffer_PI2("<tr><th>");

// //   // // move sections into progmem
    
// //   // AppendBuffer_PI2(PSTR(D_PROGRAM_VERSION "}2%s(%s)"), tkr_set->my_version, "my_image");//tkr_set->my_image
// //   // // #ifdef ESP8266
// //   // //   AppendBuffer_PI2(PSTR("}1" D_BUILD_DATE_AND_TIME "}2%s"), tkr_time->GetBuildDateAndTime(dd));
// //   // //   AppendBuffer_PI2(PSTR("}1" D_CORE_AND_SDK_VERSION "}2" ARDUINO_ESP8266_RELEASE "/%s"), ESP.getSdkVersion());
// //   // // #endif
// //   // // AppendBuffer_PI2(PSTR("}1" D_UPTIME "}2%s"), tkr_time->getFormattedUptime());
// //   // // #ifdef ESP8266
// //   // //   AppendBuffer_PI2(PSTR("}1" D_FLASH_WRITE_COUNT "}2%d at 0x%X"), tkr_set->Settings.save_flag, tkr_set->GetSettingsAddress());
// //   // // #endif
// //   // AppendBuffer_PI2(PSTR("}1" D_BOOT_COUNT "}2%d"), tkr_set->Settings.bootcount);
// //   // #ifdef ESP8266
// //   //   AppendBuffer_PI2(PSTR("}1" D_RESTART_REASON "}2%s"), tkr_sup->GetResetReason().c_str());
// //   // #endif
// //   // // uint8_t maxfn = 1;//(tkr_set->devices_present > MAX_FRIENDLYNAMES) ? MAX_FRIENDLYNAMES : tkr_set->devices_present;
// //   // // //if (SONOFF_IFAN02 == tkr_set->my_module_type) { maxfn = 1; }
// //   // // for (uint8_t i = 0; i < maxfn; i++) {
// //   // //   AppendBuffer_PI2(PSTR("}1" D_FRIENDLY_NAME " %d}2%s"), i +1, tkr_set->Settings.system_name.friendly[i]);
// //   // // }
// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // // Show SSID direct for testing
// //   // AppendBuffer_PI2(PSTR("}1" "SSID (RSS)" "}2%s (%d dBm)"), WiFi.SSID().c_str(), WiFi.RSSI());
    
// //   // AppendBuffer_PI2(PSTR("}1" D_AP "%d " D_SSID " (" D_RSSI ")}2%s (%d%%)"), tkr_set->Settings.sta_active +1, tkr_set->Settings.sta_ssid[tkr_set->Settings.sta_active], tkr_wifi->WifiGetRssiAsQuality(WiFi.RSSI()));
// //   // AppendBuffer_PI2(PSTR("}1" D_HOSTNAME "}2%s%s"), tkr_set->my_hostname, (tkr_wifi->mdns_begun) ? ".local" : "");
// //   // if (static_cast<uint32_t>(WiFi.localIP()) != 0) {
// //   //   AppendBuffer_PI2(PSTR("}1" D_IP_ADDRESS "}2%s"), WiFi.localIP().toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_GATEWAY "}2%s"), IPAddress(tkr_set->Settings.ip_address[1]).toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_SUBNET_MASK "}2%s"), IPAddress(tkr_set->Settings.ip_address[2]).toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_DNS_SERVER "}2%s"), IPAddress(tkr_set->Settings.ip_address[3]).toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.macAddress().c_str());
// //   // }
// //   // if (static_cast<uint32_t>(WiFi.softAPIP()) != 0) {
// //   //   AppendBuffer_PI2(PSTR("}1" D_IP_ADDRESS "}2%s"), WiFi.softAPIP().toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_GATEWAY "}2%s"), WiFi.softAPIP().toString().c_str());
// //   //   AppendBuffer_PI2(PSTR("}1" D_MAC_ADDRESS "}2%s"), WiFi.softAPmacAddress().c_str());
// //   // }
// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // // if (tkr_set->Settings.flag_system.mqtt_enabled) {
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_HOST "}2%s"), tkr_set->Settings.mqtt_host);
// //   // //   AppendBuffer_PI2(PSTR("}1" MQTT_PORT "}2%d"), tkr_set->Settings.mqtt_port);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_USER "}2%s"), tkr_set->Settings.mqtt_user);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_CLIENT "}2%s"), tkr_set->mqtt_client);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_TOPIC "}2%s"), tkr_set->Settings.mqtt_topic);
// //   // //   AppendBuffer_PI2(PSTR("}1" D_MQTT_GROUP_TOPIC "}2%s"), tkr_set->Settings.mqtt_grptopic);
// //   // // } else {
// //   // //   //AppendBuffer_PI2(PSTR("}1" D_MQTT "}2%s"), D_DISABLED);
// //   // // }
// //   // // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line
// //   // AppendBuffer_PI2(PSTR("}1" "Module Config" "}2"));//,       ESP.getChipId());
// //   // // Class/Tasks info
// //   // // buffer_writer_internal = buffer;
// //   // // tkr->Tasker_Interface(TASK_WEB_PAGEINFORMATION_SEND_MODULE);
// //   // AppendBuffer_PI2(PSTR("}1}2&nbsp;"));  // Empty line

// //   // #ifdef ESP8266
// //   //   AppendBuffer_PI2(PSTR("}1" D_ESP_CHIP_ID "}2%d"),       ESP.getChipId());
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_ID "}20x%06X"), ESP.getFlashChipId());
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_SIZE "}2%dkB"), ESP.getFlashChipRealSize() / 1024);
// //   // #endif
// //   // #ifdef ESP32
// //   //   AppendBuffer_PI2(PSTR("}1" D_ESP_CHIP_ID "}2%d"),       1);
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_ID "}20x%06X"), 2);
// //   //   AppendBuffer_PI2(PSTR("}1" D_FLASH_CHIP_SIZE "}2%dkB"), 3);
// //   // #endif
// //   // AppendBuffer_PI2(PSTR("}1" D_PROGRAM_FLASH_SIZE "}2%dkB"), ESP.getFlashChipSize() / 1024);
// //   // AppendBuffer_PI2(PSTR("}1" D_PROGRAM_SIZE "}2%dkB"),       ESP.getSketchSize() / 1024);
// //   // AppendBuffer_PI2(PSTR("}1" D_FREE_PROGRAM_SPACE "}2%dkB"), ESP.getFreeSketchSpace() / 1024);
// //   // AppendBuffer_PI2(PSTR("}1" D_FREE_MEMORY "}2%dkB"),        ESP.getFreeHeap() / 1024);
// //   // AppendBuffer_PI2(PSTR("</td></tr>"));

// //   // tkr_web->AppendBuffer_PI2("%s","{t2}");
// //   // tkr_web->AppendBuffer_PI2("\"}]");  
// //   // tkr_web->AppendBuffer_PI2(PSTR(","));  

// // } //end function




//         //   "var x2=null,lt2='';"
//         //   "var sn2=0,id2=0;" //sn2 starts at top of page, web_log_index starts at 0
//         //   "function l(p){"
//         //     "var c,o='',t;"
//         //     "clearTimeout(lt2);"
//         //     "t=document.getElementById('t1');"
//         //     "if(p==1){"
//         //       "c=document.getElementById('c1');"
//         //       "o='&c1='+encodeURIComponent(c.value);"
//         //       "c.value='';"
//         //       "t.scrollTop=sn2;"
//         //     "}"
//         //     //scrolltop == 0 is top of textbox, larger number = bottom
//         //     "if(t.scrollTop>=sn2){" //if scrolled up at all, don't update
//         //       "if(x2!=null){ x2.abort(); }"
//         //       "x2=new XMLHttpRequest();"
//         //       "x2.onreadystatechange=function(){"
//         //         "if(x2.readyState==4&&x2.status==200){"
//         //           "var z,d;"
//         //           //[web_log_index][reset_web_log_flag][text]
//         //           "d=x2.responseText.split(/}1/);"
//         //           "id2=d.shift();"   //web_log_index //removes first to last element
//         //           "if(d.shift()==0){" //reset_web_log_flag == 0
//         //             "t.value='';" //clear value back to start
//         //           "}"
//         //           "z=d.shift();" //get the text
//         //           "if(z.length>0){" //if new text
//         //             "t.value+=z;"  //append text
//         //           "}"
//         //           "t.scrollTop=99999;" //force to the very bottom
//         //           "sn2=t.scrollTop;"   //get scroll of the bottom line now 
//         //         "}"
//         //       "};"
//         //       "x2.open('GET','" D_WEB_HANDLE_CONSOLE_PAGE "?c2='+id2+o,true);" //current weblog_index + any new commands
//         //       "x2.send();"
//         //     "}"
//         //   "lt2=setTimeout(l,200);"
//         //   "return false;"
//         // "}"
//         // "l(0);"
//         // "function jd(){"
//         //   "var t=0,i=document.querySelectorAll('input,button,textarea,select');"
//         //   "while(i.length>=t){"
//         //     "if(i[t]){"
//         //       "i[t]['name']=(i[t].hasAttribute('id')&&(!i[t].hasAttribute('name')))?i[t]['id']:i[t]['name'];"
//         //     "}"
//         //     "t++;"
//         //   "}"
//         // "}"
//         // "jd();"

// // bool mWebServer::HandleRootStatusRefresh(AsyncWebServerRequest *request)
// // {
// //   // if (!WebAuthenticate()) {
// //   //   request->requestAuthentication();
// //   //   return true;
// //   // }

// //   if (!request->hasParam("m")) {     // Status refresh requested
// //     // AddLog(LOG_LEVEL_DEV_TEST,PSTR("!request->hasParam(\"m\")"));
// //     return false; 
// //   }else{
// //     AddLog(LOG_LEVEL_DEV_TEST,PSTR("request->hasParam(\"m\")"));
// //     // continue through root function
// //   }

// //   request_web_command = request;

// //   char tmp[30];                       // WebGetArg numbers only
// //   char svalue[32];                   // Command and number parameter

// //   // WebGetArg(request, "o", tmp, sizeof(tmp));  // 1 - 16 Device number for button Toggle or Fanspeed
// //   // if (strlen(tmp)) {
// //   //   ShowWebSource(SRC_WEBGUI);
// //   //   uint8_t device = atoi(tmp);
// //   //   if (MODULE_SONOFF_IFAN02 == tkr_set->my_module_type) {
// //   //     if (device < 2) {
// //   //       //ExecuteCommandPower(1, POWER_TOGGLE, SRC_IGNORE);
// //   //     } else {
// //   //       snprintf_P(svalue, sizeof(svalue), PSTR(D_FANSPEED " %d"), device -2);
// //   //       //ExecuteCommand(svalue, SRC_WEBGUI);
// //   //     }
// //   //   } else {
// //   //     //ExecuteCommandPower(device, POWER_TOGGLE, SRC_IGNORE);
// //   //   }
// //   // }
// //   WebGetArg(request,"d", tmp, sizeof(tmp));  // 0 - 100 Dimmer value
// //   if (strlen(tmp)) {
// //     snprintf_P(svalue, sizeof(svalue), PSTR(D_DIMMER " %s"), tmp);
// //     ExecuteWebCommand(svalue, SRC_WEBGUI);
// //   }
// //   // WebGetArg(request,"t", tmp, sizeof(tmp));  // 153 - 500 Color temperature
// //   // if (strlen(tmp)) {
// //   //   // Convert ?command to longer (t -> CT)
// //   //   snprintf_P(svalue, sizeof(svalue), PSTR(D_COLORTEMPERATURE " %s"), tmp);
// //   //   ExecuteWebCommand(svalue, SRC_WEBGUI);
// //   // }
// //   // WebGetArg(request,"k", tmp, sizeof(tmp));  // 1 - 16 Pre defined RF keys
// //   // if (strlen(tmp)) {
// //   //   snprintf_P(svalue, sizeof(svalue), PSTR(D_RFKEY "%s"), tmp);
// //   //   ExecuteWebCommand(svalue, SRC_WEBGUI);
// //   // }

// //   tkr->Tasker_Interface(TASK_WEB_COMMAND); //parse any webcommands


// //   if(RespondWebSendFreeMemoryTooLow(request,WEBSEND_FREEMEMORY_START_LIMIT)){return true;} 

// //   JBI->Start();
// //     JBI->AppendBuffer(PSTR("t}")); //temp fix
// //     // all but phased out 
// //     // REMOVE html part
// //     // tkr->Tasker_Interface(TASK_WEB_SHOW_PARAMETERS);
// //     JBI->AppendBuffer(PSTR("{t2")); //temp fix
// //   JBI->End();

// //   WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);  

// //   return true;

// // }


// // void mWebServer::WebAppend_SystemVersionBar(){  

// //   uint32_t text_colour = WebColor(255,255,255);
// //   if(tkr_set->runtime.firmware_version.fNewVersionAvailable){
// //     text_colour = WebColor(tkr_set->COL_TEXT_SUCCESS);
// //   }
// //   if(tkr_set->runtime.firmware_version.fCurrentVersionNotSupported){
// //     text_colour = WebColor(tkr_set->COL_TEXT_WARNING);
// //   }

// //   char message_version1[100];
// //   sprintf(message_version1,PSTR("%s %s%s"),
// //     PROJECT_NAME_CTR,
// //     tkr_set->runtime.firmware_version.current.name_ctr,
// //     tkr_set->runtime.firmware_version.fNewVersionAvailable ? " Update Available" : ""  
// //   );

// //   BufferWriterI->Append_P(PM_HTTP_END_WITH_UPDATE_NOEND,
// //     "https://github.com/sparkplug23/HomeAutomationControlSystem",  // URL to open when clicked
// //     text_colour,
// //     message_version1
// //   );

// // }//end function





// // void mWebServer::WebAppend_Minimal_Style(){

// //   BufferWriterI->Append_P("<style>");
// //   BufferWriterI->Append_P(PM_HTTP_HEAD_STYLE1_MINIMAL,
// //     tkr_web->WebColor(tkr_set->COL_FORM), 
// //     tkr_web->WebColor(tkr_set->COL_BACKGROUND),
// //     tkr_web->WebColor(tkr_set->COL_BUTTON),  
// //     tkr_web->WebColor(tkr_set->COL_BUTTON_TEXT)
// //   );
// //   BufferWriterI->Append_P("</style>");

// // }

// // void mWebServer::WebAppend_Title(){
// //   BufferWriterI->Append_P(HTTP_STYLE_TITLE_CONTAINER_HEAD, 
// //     WebColor(tkr_set->COL_TEXT),
// //     WebColor(tkr_set->COL_TEXT_MODULE_TITLE), 
// //     tkr_set->Settings.system_name.friendly
// //   );
// // }

// // void mWebServer::WebAppend_Script_P(PGM_P url){
// //     BufferWriterI->Append_P(PSTR("<script type='text/javascript' src='%s'></script>"), url);    
// // }


#endif


#endif //   #ifdef USE_MODULE_NETWORK_WEBSERVER21