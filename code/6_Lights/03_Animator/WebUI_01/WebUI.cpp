// #include "mChild_WebUI.h"

#include "6_Lights/03_Animator/mAnimatorLight.h"

#ifdef ENABLE_DEVFEATURE_LIGHT__WEBUI_STYLE01

StaticJsonDocument<JSON_BUFFER_SIZE> doc;


// similar to NeoPixelBus NeoGammaTableMethod but allows dynamic changes (superseded by NPB::NeoGammaDynamicTableMethod)
class NeoGammaWLEDMethod {
  public:
    static uint8_t Correct(uint8_t value);      // apply Gamma to single channel
    static uint32_t Correct32(uint32_t color);  // apply Gamma to RGBW32 color (WLED specific, not used by NPB)
    static void calcGammaTable(float gamma);    // re-calculates & fills gamma table
    static inline uint8_t rawGamma8(uint8_t val) { return gammaT[val]; }  // get value from Gamma table (WLED specific, not used by NPB)
  private:
    static uint8_t gammaT[];
};
#define gamma32(c) NeoGammaWLEDMethod::Correct32(c)


//gamma 2.8 lookup table used for color correction
uint8_t NeoGammaWLEDMethod::gammaT[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// re-calculates & fills gamma table
void NeoGammaWLEDMethod::calcGammaTable(float gamma)
{
  for (size_t i = 0; i < 256; i++) {
    gammaT[i] = (int)(powf((float)i / 255.0f, gamma) * 255.0f + 0.5f);
  }
}

uint8_t NeoGammaWLEDMethod::Correct(uint8_t value)
{
  if (!pCONT_lAni->gammaCorrectCol) return value;
  return gammaT[value];
}

// used for color gamma correction
uint32_t NeoGammaWLEDMethod::Correct32(uint32_t color)
{
  if (!pCONT_lAni->gammaCorrectCol) return color;
  uint8_t w = W(color);
  uint8_t r = R(color);
  uint8_t g = G(color);
  uint8_t b = B(color);
  w = gammaT[w];
  r = gammaT[r];
  g = gammaT[g];
  b = gammaT[b];
  return RGBW32(r, g, b, w);
}


// // re-calculates & fills gamma table
// void NeoGammaWLEDMethod::calcGammaTable(float gamma)
// {
//   for (size_t i = 0; i < 256; i++) {
//     gammaT[i] = (int)(powf((float)i / 255.0f, gamma) * 255.0f + 0.5f);
//   }
// }

// uint8_t NeoGammaWLEDMethod::Correct(uint8_t value)
// {
//   if (!gammaCorrectCol) return value;
//   return gammaT[value];
// }

// // used for color gamma correction
// uint32_t NeoGammaWLEDMethod::Correct32(uint32_t color)
// {
//   if (!gammaCorrectCol) return color;
//   uint8_t w = W(color);
//   uint8_t r = R(color);
//   uint8_t g = G(color);
//   uint8_t b = B(color);
//   w = gammaT[w];
//   r = gammaT[r];
//   g = gammaT[g];
//   b = gammaT[b];
//   return RGBW32(r, g, b, w);
// }



void mAnimatorLight::toggleOnOff()
{
  if (pCONT_iLight->_briRGB_Global == 0)
  {
    pCONT_iLight->_briRGB_Global = briLast;
  } else
  {
    briLast = pCONT_iLight->_briRGB_Global;
    pCONT_iLight->_briRGB_Global = 0;
  }
  stateChanged = true;
}


//scales the brightness with the briMultiplier factor
byte mAnimatorLight::scaledBri(byte in)
{
  uint16_t val = ((uint16_t)in*briMultiplier)/100;
  if (val > 255) val = 255;
  return (byte)val;
}


void mAnimatorLight::realtimeLock(uint32_t timeoutMs, byte md)
{
  if (!realtimeMode && !realtimeOverride) {
    uint16_t stop, start;
    if (useMainSegmentOnly) {
      mAnimatorLight::Segment_New& mainseg = getMainSegment();
      start = mainseg.pixel_range.start;
      stop  = mainseg.pixel_range.stop;
      mainseg.freeze = true;
    } else {
      start = 0;
      stop  = getLengthTotal();
    }
    // clear strip/segment
    for (size_t i = start; i < stop; i++) setPixelColor(i,BLACK);
    // if WLED was off and using main segment only, freeze non-main segments so they stay off
    if (useMainSegmentOnly && pCONT_iLight->_briRGB_Global == 0) {
      for (size_t s=0; s < getSegmentsNum(); s++) {
        getSegment(s).freeze = true;
      }
    }
  }
  // if strip is off (bri==0) and not already in RTM
  if (briT == 0 && !realtimeMode && !realtimeOverride) {
    setBrightness(scaledBri(briLast), true);
  }

  if (realtimeTimeout != UINT32_MAX) {
    realtimeTimeout = (timeoutMs == 255001 || timeoutMs == 65000) ? UINT32_MAX : millis() + timeoutMs;
  }
  realtimeMode = md;

  if (realtimeOverride) return;
  if (arlsForceMaxBri) setBrightness(scaledBri(255), true);
  if (briT > 0 && md == REALTIME_MODE_GENERIC) show();
}

void mAnimatorLight::exitRealtime() {
  if (!realtimeMode) return;
  if (realtimeOverride == REALTIME_OVERRIDE_ONCE) realtimeOverride = REALTIME_OVERRIDE_NONE;
  setBrightness(scaledBri(pCONT_iLight->_briRGB_Global), true);
  realtimeTimeout = 0; // cancel realtime mode immediately
  realtimeMode = REALTIME_MODE_INACTIVE; // inform UI immediately
  realtimeIP[0] = 0;
  if (useMainSegmentOnly) { // unfreeze live segment again
    getMainSegment().freeze = false;
  }
  // updateInterfaces(CALL_MODE_WS_SEND);
}






//simple macro for ArduinoJSON's or syntax
#define CJSON(a,b) a = b | a

void mAnimatorLight::getStringFromJson(char* dest, const char* src, size_t len) {
  if (src != nullptr) strlcpy(dest, src, len);
}

bool mAnimatorLight::deserializeConfig(JsonObject doc, bool fromFS) {
  bool needsSave = false;
  //int rev_major = doc["rev"][0]; // 1
  //int rev_minor = doc["rev"][1]; // 0

  //long vid = doc[F("vid")]; // 2010020

  #ifdef WLED_USE_ETHERNET
  JsonObject ethernet = doc[F("eth")];
  CJSON(ethernetType, ethernet["type"]);
  // NOTE: Ethernet configuration takes priority over other use of pins
  WLED::instance().initEthernet();
  #endif

  JsonObject id = doc["id"];
  // getStringFromJson(pCONT_set->Settings.system_name.device, id[F("mdns")], 33);
  getStringFromJson(serverDescription, id[F("name")], 33);
  getStringFromJson(alexaInvocationName, id[F("inv")], 33);
#ifdef WLED_ENABLE_SIMPLE_UI
  CJSON(simplifiedUI, id[F("sui")]);
#endif

  JsonObject nw_ins_0 = doc["nw"]["ins"][0];
  getStringFromJson(clientSSID, nw_ins_0[F("ssid")], 33);
  //int nw_ins_0_pskl = nw_ins_0[F("pskl")];
  //The WiFi PSK is normally not contained in the regular file for security reasons.
  //If it is present however, we will use it
  getStringFromJson(clientPass, nw_ins_0["psk"], 65);

  JsonArray nw_ins_0_ip = nw_ins_0["ip"];
  JsonArray nw_ins_0_gw = nw_ins_0["gw"];
  JsonArray nw_ins_0_sn = nw_ins_0["sn"];

  for (byte i = 0; i < 4; i++) {
    CJSON(staticIP[i], nw_ins_0_ip[i]);
    CJSON(staticGateway[i], nw_ins_0_gw[i]);
    CJSON(staticSubnet[i], nw_ins_0_sn[i]);
  }

  JsonObject ap = doc["ap"];
  getStringFromJson(apSSID, ap[F("ssid")], 33);
  getStringFromJson(apPass, ap["psk"] , 65); //normally not present due to security
  //int ap_pskl = ap[F("pskl")];

  CJSON(apChannel, ap[F("chan")]);
  if (apChannel > 13 || apChannel < 1) apChannel = 1;

  CJSON(apHide, ap[F("hide")]);
  if (apHide > 1) apHide = 1;

  CJSON(apBehavior, ap[F("behav")]);

  /*
  JsonArray ap_ip = ap["ip"];
  for (byte i = 0; i < 4; i++) {
    apIP[i] = ap_ip;
  }
  */

  noWifiSleep = doc[F("wifi")][F("sleep")] | !noWifiSleep; // inverted
  noWifiSleep = !noWifiSleep;
  //int wifi_phy = doc[F("wifi")][F("phy")]; //force phy mode n?

  JsonObject hw = doc[F("hw")];

  // initialize LED pins and lengths prior to other HW (except for ethernet)
  JsonObject hw_led = hw["led"];

  uint8_t autoWhiteMode = RGBW_MODE_MANUAL_ONLY;
  CJSON(ablMilliampsMax, hw_led[F("maxpwr")]);
  CJSON(milliampsPerLed, hw_led[F("ledma")]);
  Bus::setGlobalAWMode(hw_led[F("rgbwm")] | 255);
  CJSON(correctWB, hw_led["cct"]);
  CJSON(cctFromRgb, hw_led[F("cr")]);
  CJSON(cctBlending, hw_led[F("cb")]);
  Bus::setCCTBlend(cctBlending);
  setTargetFps(hw_led["fps"]); //NOP if 0, default 42 FPS
  CJSON(useLedsArray, hw_led[F("ld")]);

  #ifndef WLED_DISABLE_2D
  // 2D Matrix Settings
  JsonObject matrix = hw_led[F("matrix")];
  if (!matrix.isNull()) {
    isMatrix = true;
    CJSON(panels, matrix[F("mpc")]);
    panel.clear();
    JsonArray panels = matrix[F("panels")];
    uint8_t s = 0;
    if (!panels.isNull()) {
      panel.reserve(max(1U,min((size_t)panels,(size_t)WLED_MAX_PANELS)));  // pre-allocate memory for panels
      for (JsonObject pnl : panels) {
        WS2812FX::Panel p;
        CJSON(p.bottomStart, pnl["b"]);
        CJSON(p.rightStart,  pnl["r"]);
        CJSON(p.vertical,    pnl["v"]);
        CJSON(p.serpentine,  pnl["s"]);
        CJSON(p.xOffset,     pnl["x"]);
        CJSON(p.yOffset,     pnl["y"]);
        CJSON(p.height,      pnl["h"]);
        CJSON(p.width,       pnl["w"]);
        panel.push_back(p);
        if (++s >= WLED_MAX_PANELS || s >= panels) break; // max panels reached
      }
    } else {
      // fallback
      WS2812FX::Panel p;
      panels = 1;
      p.height = p.width = 8;
      p.xOffset = p.yOffset = 0;
      p.options = 0;
      panel.push_back(p);
    }
    // cannot call setUpMatrix() here due to already locked JSON buffer
  }
  #endif

  JsonArray ins = hw_led["ins"];

  if (fromFS || !ins.isNull()) {
    uint8_t s = 0;  // bus iterator
    if (fromFS) pCONT_iLight->bus_manager->removeAll(); // can't safely manipulate busses directly in network callback
    uint32_t mem = 0;
    bool busesChanged = false;
    for (JsonObject elm : ins) {
      if (s >= WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES) break;
      uint8_t pins[5] = {255, 255, 255, 255, 255};
      JsonArray pinArr = elm["pin"];
      if (pinArr.size() == 0) continue;
      pins[0] = pinArr[0];
      uint8_t i = 0;
      for (int p : pinArr) {
        pins[i++] = p;
        if (i>4) break;
      }

      // uint16_t length = elm["len"] | 1;
      // uint8_t colorOrder = (int)elm[F("order")];
      // uint8_t skipFirst = elm[F("skip")];
      // uint16_t start = elm["start"] | 0;
      // if (length==0 || start + length > MAX_LEDS) continue; // zero length or we reached max. number of LEDs, just stop
      // uint8_t ledType = elm["type"] | TYPE_WS2812_RGB;
      // bool reversed = elm["rev"];
      // bool refresh = elm["ref"] | false;
      // uint16_t freqkHz = elm[F("freq")] | 0;  // will be in kHz for DotStar and Hz for PWM (not yet implemented fully)
      // ledType |= refresh << 7; // hack bit 7 to indicate strip requires off refresh
      // uint8_t AWmode = elm[F("rgbwm")] | autoWhiteMode;
      // if (fromFS) {
      //   BusConfig bc = BusConfig(ledType, pins, start, length, colorOrder, reversed, skipFirst, AWmode, freqkHz);
      //   mem += BusManager::memUsage(bc);
      //   if (mem <= MAX_LED_MEMORY) if (busses.add(bc) == -1) break;  // finalization will be done in WLED::beginStrip()
      // } else {
      //   if (pCONT_iLight->busConfigs[s] != nullptr) delete pCONT_iLight->busConfigs[s];
      //   pCONT_iLight->busConfigs[s] = new mInterfaceLight::BusConfig(ledType, pins, start, length, colorOrder, reversed, skipFirst, AWmode);
      //   busesChanged = true;
      // }
      s++;
    }
    doInitBusses = busesChanged;
    // finalization done in beginStrip()
  }
  if (hw_led["rev"]) pCONT_iLight->bus_manager->getBus(0)->reversed = true; //set 0.11 global reversed setting for first bus

  // read color order map configuration
  JsonArray hw_com = hw[F("com")];
  if (!hw_com.isNull()) {
    ColorOrderMap com = {};
    uint8_t s = 0;
    for (JsonObject entry : hw_com) {
      if (s > WLED_MAX_COLOR_ORDER_MAPPINGS) break;
      uint16_t start = entry["start"] | 0;
      uint16_t len = entry["len"] | 0;
      uint8_t colorOrder = (int)entry[F("order")];
      // com.add(start, len, colorOrder);
      s++;
    }
    pCONT_iLight->bus_manager->updateColorOrderMap(com);
  }

  // read multiple button configuration
  JsonObject btn_obj = hw["btn"];
  bool pull = btn_obj[F("pull")] | (!disablePullUp); // if true, pullup is enabled
  disablePullUp = !pull;
  JsonArray hw_btn_ins = btn_obj[F("ins")];
  if (!hw_btn_ins.isNull()) {
    uint8_t s = 0;
    for (JsonObject btn : hw_btn_ins) {
      CJSON(buttonType[s], btn["type"]);
      int8_t pin = btn["pin"][0] | -1;
      // if (pin > -1 && pinManager.allocatePin(pin, false, PinOwner::Button)) {
      //   btnPin[s] = pin;
      // #ifdef ARDUINO_ARCH_ESP32
      //   // ESP32 only: check that analog button pin is a valid ADC gpio
      //   if (((buttonType[s] == BTN_TYPE_ANALOG) || (buttonType[s] == BTN_TYPE_ANALOG_INVERTED)) && (digitalPinToAnalogChannel(btnPin[s]) < 0))
      //   {
      //     // not an ADC analog pin
      //     DEBUG_PRINTF("PIN ALLOC error: GPIO%d for analog button #%d is not an analog pin!\n", btnPin[s], s);
      //     btnPin[s] = -1;
      //     pinManager.deallocatePin(pin,PinOwner::Button);
      //   }
      //   else
      // #endif
      //   {
      //     if (disablePullUp) {
      //       pinMode(btnPin[s], INPUT);
      //     } else {
      //       #ifdef ESP32
      //       pinMode(btnPin[s], buttonType[s]==BTN_TYPE_PUSH_ACT_HIGH ? INPUT_PULLDOWN : INPUT_PULLUP);
      //       #else
      //       pinMode(btnPin[s], INPUT_PULLUP);
      //       #endif
      //     }
      //   }
      // } else {
      //   // btnPin[s] = -1;
      // }
      JsonArray hw_btn_ins_0_macros = btn["macros"];
      CJSON(macroButton[s], hw_btn_ins_0_macros[0]);
      CJSON(macroLongPress[s],hw_btn_ins_0_macros[1]);
      CJSON(macroDoublePress[s], hw_btn_ins_0_macros[2]);
      if (++s >= WLED_MAX_BUTTONS) break; // max buttons reached
    }
    // clear remaining buttons
    // for (; s<WLED_MAX_BUTTONS; s++) {
    //   btnPin[s]           = -1;
    //   buttonType[s]       = BTN_TYPE_NONE;
    //   macroButton[s]      = 0;
    //   macroLongPress[s]   = 0;
    //   macroDoublePress[s] = 0;
    // }
  } else {
    // new install/missing configuration (button 0 has defaults)
    if (fromFS) {
      // // relies upon only being called once with fromFS == true, which is currently true.
      // uint8_t s = 0;
      // if (pinManager.allocatePin(btnPin[0], false, PinOwner::Button)) { // initialized to #define value BTNPIN, or zero if not defined(!)
      //   ++s; // do not clear default button if allocated successfully
      // }
      // for (; s<WLED_MAX_BUTTONS; s++) {
      //   btnPin[s]           = -1;
      //   buttonType[s]       = BTN_TYPE_NONE;
      //   macroButton[s]      = 0;
      //   macroLongPress[s]   = 0;
      //   macroDoublePress[s] = 0;
      // }
    }
  }
  CJSON(touchThreshold,btn_obj[F("tt")]);
  CJSON(buttonPublishMqtt,btn_obj["mqtt"]);

  // int hw_ir_pin = hw["ir"]["pin"] | -2; // 4
  // if (hw_ir_pin > -2) {
  //   if (pinManager.allocatePin(hw_ir_pin, false, PinOwner::IR)) {
  //     irPin = hw_ir_pin;
  //   } else {
  //     irPin = -1;
  //   }
  // }
  CJSON(irEnabled, hw["ir"]["type"]);
  CJSON(irApplyToAllSelected, hw["ir"]["sel"]);

  // JsonObject relay = hw[F("relay")];
  // int hw_relay_pin = relay["pin"] | -2;
  // if (hw_relay_pin > -2) {
  //   if (pinManager.allocatePin(hw_relay_pin,true, PinOwner::Relay)) {
  //     rlyPin = hw_relay_pin;
  //     pinMode(rlyPin, OUTPUT);
  //   } else {
  //     rlyPin = -1;
  //   }
  // }
  // if (relay.containsKey("rev")) {
  //   rlyMde = !relay["rev"];
  // }

  CJSON(serialBaud, hw[F("baud")]);
  // if (serialBaud < 96 || serialBaud > 15000) serialBaud = 1152;
  // updateBaudRate(serialBaud *100);

  // JsonArray hw_if_i2c = hw[F("if")][F("i2c-pin")];
  // CJSON(i2c_sda, hw_if_i2c[0]);
  // CJSON(i2c_scl, hw_if_i2c[1]);
  // PinManagerPinType i2c[2] = { { i2c_sda, true }, { i2c_scl, true } };
  // if (i2c_scl >= 0 && i2c_sda >= 0 && pinManager.allocateMultiplePins(i2c, 2, PinOwner::HW_I2C)) {
  //   #ifdef ESP32
  //   if (!Wire.setPins(i2c_sda, i2c_scl)) { i2c_scl = i2c_sda = -1; } // this will fail if Wire is initilised (Wire.begin() called prior)
  //   else Wire.begin();
  //   #else
  //   Wire.begin(i2c_sda, i2c_scl);
  //   #endif
  // } else {
  //   i2c_sda = -1;
  //   i2c_scl = -1;
  // }
  JsonArray hw_if_spi = hw[F("if")][F("spi-pin")];
  // CJSON(spi_mosi, hw_if_spi[0]);
  // CJSON(spi_sclk, hw_if_spi[1]);
  // CJSON(spi_miso, hw_if_spi[2]);
  // PinManagerPinType spi[3] = { { spi_mosi, true }, { spi_miso, true }, { spi_sclk, true } };
  // if (spi_mosi >= 0 && spi_sclk >= 0 && pinManager.allocateMultiplePins(spi, 3, PinOwner::HW_SPI)) {
  //   #ifdef ESP32
  //   SPI.begin(spi_sclk, spi_miso, spi_mosi);  // SPI global uses VSPI on ESP32 and FSPI on C3, S3
  //   #else
  //   SPI.begin();
  //   #endif
  // } else {
  //   spi_mosi = -1;
  //   spi_miso = -1;
  //   spi_sclk = -1;
  // }

  //int hw_status_pin = hw[F("status")]["pin"]; // -1

  JsonObject light = doc[F("light")];
  CJSON(briMultiplier, light[F("scale-bri")]);
  CJSON(paletteBlend, light[F("pal-mode")]);
  CJSON(autoSegments, light[F("aseg")]);

  CJSON(gammaCorrectVal, light["gc"]["val"]); // default 2.8
  float light_gc_bri = light["gc"]["bri"];
  float light_gc_col = light["gc"]["col"];
  // if (light_gc_bri > 1.0f) gammaCorrectBri = true;
  // else                     gammaCorrectBri = false;
  // if (light_gc_col > 1.0f) gammaCorrectCol = true;
  // else                     gammaCorrectCol = false;
  // if (gammaCorrectVal > 1.0f && gammaCorrectVal <= 3) {
  //   if (gammaCorrectVal != 2.8f) NeoGammaWLEDMethod::calcGammaTable(gammaCorrectVal);
  // } else {
  //   gammaCorrectVal = 1.0f; // no gamma correction
  //   gammaCorrectBri = false;
  //   gammaCorrectCol = false;
  // }

  JsonObject light_tr = light["tr"];
  CJSON(fadeTransition, light_tr["mode"]);
  int tdd = light_tr["dur"] | -1;
  if (tdd >= 0) transitionDelay = transitionDelayDefault = tdd * 100;
  CJSON(paletteFade, light_tr["pal"]);
  CJSON(randomPaletteChangeTime, light_tr[F("rpc")]);

  JsonObject light_nl = light["nl"];
  CJSON(nightlightMode, light_nl["mode"]);
  byte prev = nightlightDelayMinsDefault;
  CJSON(nightlightDelayMinsDefault, light_nl["dur"]);
  if (nightlightDelayMinsDefault != prev) nightlightDelayMins = nightlightDelayMinsDefault;

  CJSON(nightlightTargetBri, light_nl[F("tbri")]);
  CJSON(macroNl, light_nl["macro"]);

  JsonObject def = doc["def"];
  CJSON(bootPreset, def["ps"]);
  CJSON(turnOnAtBoot, def["on"]); // true
  CJSON(pCONT_iLight->_briRGB_Global, def["bri"]); // 128

  JsonObject interfaces = doc["if"];

  JsonObject if_sync = interfaces["sync"];
  CJSON(udpPort, if_sync[F("port0")]); // 21324
  CJSON(udpPort2, if_sync[F("port1")]); // 65506

  JsonObject if_sync_recv = if_sync["recv"];
  CJSON(receiveNotificationBrightness, if_sync_recv["bri"]);
  CJSON(receiveNotificationColor, if_sync_recv["col"]);
  CJSON(receiveNotificationEffects, if_sync_recv["fx"]);
  CJSON(receiveGroups, if_sync_recv["grp"]);
  CJSON(receiveSegmentOptions, if_sync_recv["seg"]);
  CJSON(receiveSegmentBounds, if_sync_recv["sb"]);
  //! following line might be a problem if called after boot
  receiveNotifications = (receiveNotificationBrightness || receiveNotificationColor || receiveNotificationEffects || receiveSegmentOptions);

  JsonObject if_sync_send = if_sync["send"];
  prev = notifyDirectDefault;
  CJSON(notifyDirectDefault, if_sync_send[F("dir")]);
  if (notifyDirectDefault != prev) notifyDirect = notifyDirectDefault;
  CJSON(notifyButton, if_sync_send["btn"]);
  CJSON(notifyAlexa, if_sync_send["va"]);
  CJSON(notifyHue, if_sync_send["hue"]);
  CJSON(notifyMacro, if_sync_send["macro"]);
  CJSON(syncGroups, if_sync_send["grp"]);
  if (if_sync_send[F("twice")]) udpNumRetries = 1; // import setting from 0.13 and earlier
  CJSON(udpNumRetries, if_sync_send["ret"]);

  JsonObject if_nodes = interfaces["nodes"];
  CJSON(nodeListEnabled, if_nodes[F("list")]);
  CJSON(nodeBroadcastEnabled, if_nodes[F("bcast")]);

  JsonObject if_live = interfaces["live"];
  CJSON(receiveDirect, if_live["en"]);
  CJSON(useMainSegmentOnly, if_live[F("mso")]);
  CJSON(e131Port, if_live["port"]); // 5568
  // if (e131Port == DDP_DEFAULT_PORT) e131Port = E131_DEFAULT_PORT; // prevent double DDP port allocation
  CJSON(e131Multicast, if_live[F("mc")]);

  JsonObject if_live_dmx = if_live[F("dmx")];
  CJSON(e131Universe, if_live_dmx[F("uni")]);
  CJSON(e131SkipOutOfSequence, if_live_dmx[F("seqskip")]);
  CJSON(DMXAddress, if_live_dmx[F("addr")]);
  if (!DMXAddress || DMXAddress > 510) DMXAddress = 1;
  CJSON(DMXSegmentSpacing, if_live_dmx[F("dss")]);
  if (DMXSegmentSpacing > 150) DMXSegmentSpacing = 0;
  // CJSON(e131Priority, if_live_dmx[F("e131prio")]);
  // if (e131Priority > 200) e131Priority = 200;
  CJSON(DMXMode, if_live_dmx["mode"]);

  tdd = if_live[F("timeout")] | -1;
  if (tdd >= 0) realtimeTimeoutMs = tdd * 100;
  CJSON(arlsForceMaxBri, if_live[F("maxbri")]);
  CJSON(arlsDisableGammaCorrection, if_live[F("no-gc")]); // false
  CJSON(arlsOffset, if_live[F("offset")]); // 0

  CJSON(alexaEnabled, interfaces["va"][F("alexa")]); // false

  CJSON(macroAlexaOn, interfaces["va"]["macros"][0]);
  CJSON(macroAlexaOff, interfaces["va"]["macros"][1]);

  CJSON(alexaNumPresets, interfaces["va"]["p"]);

#ifdef WLED_ENABLE_MQTT
  JsonObject if_mqtt = interfaces["mqtt"];
  CJSON(mqttEnabled, if_mqtt["en"]);
  getStringFromJson(mqttServer, if_mqtt[F("broker")], 33);
  CJSON(mqttPort, if_mqtt["port"]); // 1883
  getStringFromJson(mqttUser, if_mqtt[F("user")], 41);
  getStringFromJson(mqttPass, if_mqtt["psk"], 65); //normally not present due to security
  getStringFromJson(mqttClientID, if_mqtt[F("cid")], 41);

  getStringFromJson(mqttDeviceTopic, if_mqtt[F("topics")][F("device")], 33); // "wled/test"
  getStringFromJson(mqttGroupTopic, if_mqtt[F("topics")][F("group")], 33); // ""
  CJSON(retainMqttMsg, if_mqtt[F("rtn")]);
#endif

#ifndef WLED_DISABLE_ESPNOW
  JsonObject remote = doc["remote"];
  CJSON(enable_espnow_remote, remote[F("remote_enabled")]);
  getStringFromJson(linked_remote, remote[F("linked_remote")], 13);
#endif


#ifndef WLED_DISABLE_HUESYNC
  JsonObject if_hue = interfaces["hue"];
  CJSON(huePollingEnabled, if_hue["en"]);
  CJSON(huePollLightId, if_hue["id"]);
  tdd = if_hue[F("iv")] | -1;
  if (tdd >= 2) huePollIntervalMs = tdd * 100;

  JsonObject if_hue_recv = if_hue["recv"];
  CJSON(hueApplyOnOff, if_hue_recv["on"]);
  CJSON(hueApplyBri, if_hue_recv["bri"]);
  CJSON(hueApplyColor, if_hue_recv["col"]);

  JsonArray if_hue_ip = if_hue["ip"];

  for (byte i = 0; i < 4; i++)
    CJSON(hueIP[i], if_hue_ip[i]);
#endif

  JsonObject if_ntp = interfaces[F("ntp")];
  CJSON(ntpEnabled, if_ntp["en"]);
  getStringFromJson(ntpServerName, if_ntp[F("host")], 33); // "1.wled.pool.ntp.org"
  CJSON(currentTimezone, if_ntp[F("tz")]);
  CJSON(utcOffsetSecs, if_ntp[F("offset")]);
  CJSON(useAMPM, if_ntp[F("ampm")]);
  // CJSON(longitude, if_ntp[F("ln")]);
  // CJSON(latitude, if_ntp[F("lt")]);

  JsonObject ol = doc[F("ol")];
  CJSON(overlayCurrent ,ol[F("clock")]); // 0
  CJSON(countdownMode, ol[F("cntdwn")]);

  CJSON(overlayMin, ol["min"]);
  CJSON(overlayMax, ol[F("max")]);
  CJSON(analogClock12pixel, ol[F("o12pix")]);
  CJSON(analogClock5MinuteMarks, ol[F("o5m")]);
  CJSON(analogClockSecondsTrail, ol[F("osec")]);

  //timed macro rules
  JsonObject tm = doc[F("timers")];
  JsonObject cntdwn = tm[F("cntdwn")];
  JsonArray cntdwn_goal = cntdwn[F("goal")];
  CJSON(countdownYear,  cntdwn_goal[0]);
  CJSON(countdownMonth, cntdwn_goal[1]);
  CJSON(countdownDay,   cntdwn_goal[2]);
  CJSON(countdownHour,  cntdwn_goal[3]);
  CJSON(countdownMin,   cntdwn_goal[4]);
  CJSON(countdownSec,   cntdwn_goal[5]);
  CJSON(macroCountdown, cntdwn["macro"]);
  // setCountdown();

  JsonArray timers = tm["ins"];
  uint8_t it = 0;
  for (JsonObject timer : timers) {
    if (it > 9) break;
    if (it<8 && timer[F("hour")]==255) it=8;  // hour==255 -> sunrise/sunset
    CJSON(timerHours[it], timer[F("hour")]);
    CJSON(timerMinutes[it], timer["min"]);
    CJSON(timerMacro[it], timer["macro"]);

    byte dowPrev = timerWeekday[it];
    //note: act is currently only 0 or 1.
    //the reason we are not using bool is that the on-disk type in 0.11.0 was already int
    int actPrev = timerWeekday[it] & 0x01;
    CJSON(timerWeekday[it], timer[F("dow")]);
    if (timerWeekday[it] != dowPrev) { //present in JSON
      timerWeekday[it] <<= 1; //add active bit
      int act = timer["en"] | actPrev;
      if (act) timerWeekday[it]++;
    }
    if (it<8) {
      JsonObject start = timer["start"];
      byte startm = start["mon"];
      if (startm) timerMonth[it] = (startm << 4);
      CJSON(timerDay[it], start["day"]);
      JsonObject end = timer["end"];
      CJSON(timerDayEnd[it], end["day"]);
      byte endm = end["mon"];
      if (startm) timerMonth[it] += endm & 0x0F;
      if (!(timerMonth[it] & 0x0F)) timerMonth[it] += 12; //default end month to 12
    }
    it++;
  }

  JsonObject ota = doc["ota"];
  const char* pwd = ota["psk"]; //normally not present due to security

  bool pwdCorrect = !otaLock; //always allow access if ota not locked
  if (pwd != nullptr && strncmp(otaPass, pwd, 33) == 0) pwdCorrect = true;

  if (pwdCorrect) { //only accept these values from cfg.json if ota is unlocked (else from wsec.json)
    CJSON(otaLock, ota[F("lock")]);
    CJSON(wifiLock, ota[F("lock-wifi")]);
    CJSON(aOtaEnabled, ota[F("aota")]);
    getStringFromJson(otaPass, pwd, 33); //normally not present due to security
  }

  #ifdef WLED_ENABLE_DMX
  JsonObject dmx = doc["dmx"];
  CJSON(DMXChannels, dmx[F("chan")]);
  CJSON(DMXGap,dmx[F("gap")]);
  CJSON(DMXStart, dmx["start"]);
  CJSON(DMXStartLED,dmx[F("start-led")]);

  JsonArray dmx_fixmap = dmx[F("fixmap")];
  for (int i = 0; i < dmx_fixmap.size(); i++) {
    if (i > 14) break;
    CJSON(DMXFixtureMap[i],dmx_fixmap[i]);
  }

  CJSON(e131ProxyUniverse, dmx[F("e131proxy")]);
  #endif

  DEBUG_PRINTLN(F("Starting usermod config."));
  JsonObject usermods_settings = doc["um"];
  if (!usermods_settings.isNull()) {
    // needsSave = !usermods.readFromConfig(usermods_settings);
  }

  if (fromFS) return needsSave;
  // if from /json/cfg
  doReboot = doc[F("rb")] | doReboot;
  if (doInitBusses) return false; // no save needed, will do after bus init in wled.cpp loop
  return (doc["sv"] | true);
}






// /*
//  * Methods to handle saving and loading presets to/from the filesystem
//  */

// #ifdef ARDUINO_ARCH_ESP32
// static char *tmpRAMbuffer = nullptr;
// #endif

// static volatile byte presetToApply = 0;
// static volatile byte callModeToApply = 0;
// static volatile byte presetToSave = 0;
// static volatile int8_t saveLedmap = -1;
// static char quickLoad[9];
// static char saveName[33];
// static bool includeBri = true, segBounds = true, selectedOnly = false, playlistSave = false;;

// static const char *getFileName(bool persist = true) {
//   return persist ? "/presets.json" : "/tmp.json";
// }

// static void doSaveState() {
//   bool persist = (presetToSave < 251);
//   const char *filename = getFileName(persist);

//   if (!requestJSONBufferLock(10)) return; // will set fileDoc

//   initPresetsFile(); // just in case if someone deleted presets.json using /edit
//   JsonObject sObj = doc.to<JsonObject>();

//   DEBUG_PRINTLN(F("Serialize current state"));
//   if (playlistSave) {
//     serializePlaylist(sObj);
//     if (includeBri) sObj["on"] = true;
//   } else {
//     serializeState(sObj, true, includeBri, segBounds, selectedOnly);
//   }
//   sObj["n"] = saveName;
//   if (quickLoad[0]) sObj[F("ql")] = quickLoad;
//   if (saveLedmap >= 0) sObj[F("ledmap")] = saveLedmap;
// /*
//   #ifdef WLED_DEBUG
//     DEBUG_PRINTLN(F("Serialized preset"));
//     serializeJson(doc,Serial);
//     DEBUG_PRINTLN();
//   #endif
// */
//   #if defined(ARDUINO_ARCH_ESP32)
//   if (!persist) {
//     if (tmpRAMbuffer!=nullptr) free(tmpRAMbuffer);
//     size_t len = measureJson(*fileDoc) + 1;
//     DEBUG_PRINTLN(len);
//     // if possible use SPI RAM on ESP32
//     #if defined(BOARD_HAS_PSRAM) && defined(WLED_USE_PSRAM)
//     if (psramFound())
//       tmpRAMbuffer = (char*) ps_malloc(len);
//     else
//     #endif
//       tmpRAMbuffer = (char*) malloc(len);
//     if (tmpRAMbuffer!=nullptr) {
//       serializeJson(*fileDoc, tmpRAMbuffer, len);
//     } else {
//       writeObjectToFileUsingId(filename, presetToSave, fileDoc);
//     }
//   } else
//   #endif
//   writeObjectToFileUsingId(filename, presetToSave, fileDoc);

//   if (persist) presetsModifiedTime = toki.second(); //unix time
//   releaseJSONBufferLock();
//   updateFSInfo();

//   // clean up
//   saveLedmap   = -1;
//   presetToSave = 0;
//   saveName[0]  = '\0';
//   quickLoad[0] = '\0';
//   playlistSave = false;
// }

// bool getPresetName(byte index, String& name)
// {
//   if (!requestJSONBufferLock(9)) return false;
//   bool presetExists = false;
//   if (readObjectFromFileUsingId(getFileName(), index, &doc))
//   {
//     JsonObject fdo = doc.as<JsonObject>();
//     if (fdo["n"]) {
//       name = (const char*)(fdo["n"]);
//       presetExists = true;
//     }
//   }
//   releaseJSONBufferLock();
//   return presetExists;
// }

// void initPresetsFile()
// {
//   if (WLED_FS.exists(getFileName())) return;

//   StaticJsonDocument<64> doc;
//   JsonObject sObj = doc.to<JsonObject>();
//   sObj.createNestedObject("0");
//   File f = WLED_FS.open(getFileName(), "w");
//   if (!f) {
//     errorFlag = ERR_FS_GENERAL;
//     return;
//   }
//   serializeJson(doc, f);
//   f.close();
// }

// bool applyPreset(byte index, byte callMode)
// {
//   DEBUG_PRINT(F("Request to apply preset: "));
//   DEBUG_PRINTLN(index);
//   presetToApply = index;
//   callModeToApply = callMode;
//   return true;
// }

// // apply preset or fallback to a effect and palette if it doesn't exist
// void applyPresetWithFallback(uint8_t index, uint8_t callMode, uint8_t effectID, uint8_t paletteID)
// {
//   applyPreset(index, callMode);
//   //these two will be overwritten if preset exists in handlePresets()
//   effectCurrent = effectID;
//   effectPalette = paletteID;
// }

// void handlePresets()
// {
//   if (presetToSave) {
//     doSaveState();
//     return;
//   }

//   if (presetToApply == 0 || fileDoc) return; // no preset waiting to apply, or JSON buffer is already allocated, return to loop until free

//   bool changePreset = false;
//   uint8_t tmpPreset = presetToApply; // store temporary since deserializeState() may call applyPreset()
//   uint8_t tmpMode   = callModeToApply;

//   JsonObject fdo;
//   const char *filename = getFileName(tmpPreset < 255);

//   // allocate buffer
//   if (!requestJSONBufferLock(9)) return;  // will also assign fileDoc

//   presetToApply = 0; //clear request for preset
//   callModeToApply = 0;

//   DEBUG_PRINT(F("Applying preset: "));
//   DEBUG_PRINTLN(tmpPreset);

//   #ifdef ARDUINO_ARCH_ESP32
//   if (tmpPreset==255 && tmpRAMbuffer!=nullptr) {
//     deserializeJson(*fileDoc,tmpRAMbuffer);
//     errorFlag = ERR_NONE;
//   } else
//   #endif
//   {
//   errorFlag = readObjectFromFileUsingId(filename, tmpPreset, fileDoc) ? ERR_NONE : ERR_FS_PLOAD;
//   }
//   fdo = fileDoc->as<JsonObject>();

//   //HTTP API commands
//   const char* httpwin = fdo["win"];
//   if (httpwin) {
//     String apireq = "win"; // reduce flash string usage
//     apireq += F("&IN&"); // internal call
//     apireq += httpwin;
//     handleSet(nullptr, apireq, false); // may call applyPreset() via PL=
//     setValuesFromFirstSelectedSeg(); // fills legacy values
//     changePreset = true;
//   } else {
//     if (!fdo["seg"].isNull() || !fdo["on"].isNull() || !fdo["bri"].isNull() || !fdo["nl"].isNull() || !fdo["ps"].isNull() || !fdo[F("playlist")].isNull()) changePreset = true;
//     if (!(tmpMode == CALL_MODE_BUTTON_PRESET && fdo["ps"].is<const char *>() && strchr(fdo["ps"].as<const char *>(),'~') != strrchr(fdo["ps"].as<const char *>(),'~')))
//       fdo.remove("ps"); // remove load request for presets to prevent recursive crash (if not called by button and contains preset cycling string "1~5~")
//     deserializeState(fdo, CALL_MODE_NO_NOTIFY, tmpPreset); // may change presetToApply by calling applyPreset()
//   }
//   if (!errorFlag && tmpPreset < 255 && changePreset) presetCycCurr = currentPreset = tmpPreset;

//   #if defined(ARDUINO_ARCH_ESP32)
//   //Aircoookie recommended not to delete buffer
//   if (tmpPreset==255 && tmpRAMbuffer!=nullptr) {
//     free(tmpRAMbuffer);
//     tmpRAMbuffer = nullptr;
//   }
//   #endif

//   releaseJSONBufferLock(); // will also clear fileDoc
//   if (changePreset) notify(tmpMode); // force UDP notification
//   stateUpdated(tmpMode);  // was colorUpdated() if anything breaks
//   updateInterfaces(tmpMode);
// }

// //called from handleSet(PS=) [network callback (fileDoc==nullptr), IR (irrational), deserializeState, UDP] and deserializeState() [network callback (filedoc!=nullptr)]
// void savePreset(byte index, const char* pname, JsonObject sObj)
// {
//   if (index == 0 || (index > 250 && index < 255)) return;
//   if (pname) strlcpy(saveName, pname, 33);
//   else {
//     if (sObj["n"].is<const char*>()) strlcpy(saveName, sObj["n"].as<const char*>(), 33);
//     else                             sprintf_P(saveName, PSTR("Preset %d"), index);
//   }

//   DEBUG_PRINT(F("Saving preset (")); DEBUG_PRINT(index); DEBUG_PRINT(F(") ")); DEBUG_PRINTLN(saveName);

//   presetToSave = index;
//   playlistSave = false;
//   if (sObj[F("ql")].is<const char*>()) strlcpy(quickLoad, sObj[F("ql")].as<const char*>(), 9); // client limits QL to 2 chars, buffer for 8 bytes to allow unicode

//   if (sObj["o"].isNull()) { // no "o" means not a playlist or custom API call, saving of state is async (not immediately)
//     includeBri   = sObj["ib"].as<bool>() || index==255; // temporary preset needs brightness
//     segBounds    = sObj["sb"].as<bool>() || index==255; // temporary preset needs bounds
//     selectedOnly = sObj[F("sc")].as<bool>();
//     saveLedmap   = sObj[F("ledmap")] | -1;
//   } else {
//     // this is a playlist or API call
//     if (sObj[F("playlist")].isNull()) {
//       // we will save API call immediately (often causes presets.json corruption)
//       presetToSave = 0;
//       if (index > 250 || !fileDoc) return; // cannot save API calls to temporary preset (255)
//       sObj.remove("o");
//       sObj.remove("v");
//       sObj.remove("time");
//       sObj.remove(F("error"));
//       sObj.remove(F("psave"));
//       if (sObj["n"].isNull()) sObj["n"] = saveName;
//       initPresetsFile(); // just in case if someone deleted presets.json using /edit
//       writeObjectToFileUsingId(getFileName(index<255), index, fileDoc);
//       presetsModifiedTime = toki.second(); //unix time
//       updateFSInfo();
//     } else {
//       // store playlist
//       // WARNING: playlist will be loaded in json.cpp after this call and will have repeat counter increased by 1
//       includeBri   = true; // !sObj["on"].isNull();
//       playlistSave = true;
//     }
//   }
// }

// void deletePreset(byte index) {
//   StaticJsonDocument<24> empty;
//   writeObjectToFileUsingId(getFileName(), index, &empty);
//   presetsModifiedTime = toki.second(); //unix time
//   updateFSInfo();
// }
























///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//macro to convert F to const
#define SET_F(x)  (const char*)F(x)

bool mAnimatorLight::oappend(const char* txt)
{
  uint16_t len = strlen(txt);
  if (olen + len >= SETTINGS_STACK_BUF_SIZE)
    return false;        // buffer full
  strcpy(obuf + olen, txt);
  olen += len;
  return true;
}


bool mAnimatorLight::oappendi(int i)
{
  char s[11];
  sprintf(s, "%d", i);
  return oappend(s);
}

//append a string setting to buffer
void mAnimatorLight::sappends(char stype, const char* key, char* val)
{
  switch(stype)
  {
    case 's': {//string (we can interpret val as char*)
      String buf = val;
      //convert "%" to "%%" to make EspAsyncWebServer happy
      //buf.replace("%","%%");
      oappend("d.Sf.");
      oappend(key);
      oappend(".value=\"");
      oappend(buf.c_str());
      oappend("\";");
      break;}
    case 'm': //message
      oappend(SET_F("d.getElementsByClassName"));
      oappend(key);
      oappend(SET_F(".innerHTML=\""));
      oappend(val);
      oappend("\";");
      break;
  }
}

//append a numeric setting to string buffer
void mAnimatorLight::sappend(char stype, const char* key, int val)
{
  char ds[] = "d.Sf.";

  switch(stype)
  {
    case 'c': //checkbox
      oappend(ds);
      oappend(key);
      oappend(".checked=");
      oappendi(val);
      oappend(";");
      break;
    case 'v': //numeric
      oappend(ds);
      oappend(key);
      oappend(".value=");
      oappendi(val);
      oappend(";");
      break;
    case 'i': //selectedIndex
      oappend(ds);
      oappend(key);
      oappend(SET_F(".selectedIndex="));
      oappendi(val);
      oappend(";");
      break;
  }
}



          


#define Network WiFi



/*
 * Binary array for the Web UI.
 * gzip is used for smaller size and improved speeds.
 * 
 * Please see https://kno.wled.ge/advanced/custom-features/#changing-web-ui
 * to find out how to easily modify the web UI source!
 */
 
// #include <AsyncTCP.h>
// #include <ESPAsyncWebServer.h>

// #if defined(ESP32)
// AsyncWebServer server(80);
// #endif  // ESP32

// define flash strings once (saves flash memory)
static const char s_redirecting[] PROGMEM = "Redirecting...";
static const char s_content_enc[] PROGMEM = "Content-Encoding";
static const char s_unlock_ota [] PROGMEM = "Please unlock OTA in security settings!";
static const char s_unlock_cfg [] PROGMEM = "Please unlock settings using PIN code!";


String messageHead, messageSub;
byte optionType;

String mAnimatorLight::msgProcessor(const String& var)
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


void mAnimatorLight::serveMessage(AsyncWebServerRequest* request, uint16_t code, const String& headl, const String& subl, byte optionT)
{
  messageHead = headl;
  messageSub = subl;
  optionType = optionT;

  request->send_P(code, "text/html", PAGE_msg, msgProcessor);
}


AsyncWebHandler *editHandler = nullptr;

void mAnimatorLight::createEditHandler(bool enable) {
  if (editHandler != nullptr) pCONT_web->server->removeHandler(editHandler);
  if (enable) {
    #ifdef WLED_ENABLE_FS_EDITOR
      #ifdef ARDUINO_ARCH_ESP32
      editHandler = &server.addHandler(new SPIFFSEditor(WLED_FS));//http_username,http_password));
      #else
      editHandler = &server.addHandler(new SPIFFSEditor("","",WLED_FS));//http_username,http_password));
      #endif
    #else
      editHandler = &pCONT_web->server->on("/edit", HTTP_GET, [this](AsyncWebServerRequest *request){
        this->serveMessage(request, 501, "Not implemented", F("The FS editor is disabled in this build."), 254);
      });
    #endif
  } else {
    editHandler = &pCONT_web->server->on("/edit", HTTP_ANY, [this](AsyncWebServerRequest *request){
      this->serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_cfg), 254);
    });
  }
}

//Un-comment any file types you need
String mAnimatorLight::getContentType(AsyncWebServerRequest* request, String filename){
  if(request->hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".json")) return "application/json";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
//  else if(filename.endsWith(".xml")) return "text/xml";
//  else if(filename.endsWith(".pdf")) return "application/x-pdf";
//  else if(filename.endsWith(".zip")) return "application/x-zip";
//  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


bool mAnimatorLight::handleIfNoneMatchCacheHeader(AsyncWebServerRequest* request)
{
  AsyncWebHeader* header = request->getHeader("If-None-Match");
  if (header && header->value() == String(PROJECT_VERSION)) {
    request->send(304);
    return true;
  }
  return false;
}

void mAnimatorLight::setStaticContentCacheHeaders(AsyncWebServerResponse *response)
{
  char tmp[40];
  // https://medium.com/@codebyamir/a-web-developers-guide-to-browser-caching-cc41f3b73e7c
  #ifndef WLED_DEBUG
  // This header name is misleading, "no-cache" will not disable cache,
  // it just revalidates on every load using the "If-None-Match" header with the last ETag value
  response->addHeader(F("Cache-Control"),"no-cache");
  #else
  response->addHeader(F("Cache-Control"),"no-store,max-age=0"); // prevent caching if debug build
  #endif
  snprintf_P(tmp, sizeof(tmp), PSTR("%d-%02x"), PROJECT_VERSION, cacheInvalidate);
  response->addHeader(F("ETag"), tmp);
}


bool mAnimatorLight::handleFileRead(AsyncWebServerRequest* request, String path){
  DEBUG_PRINTLN("WS FileRead: " + path);
  if(path.endsWith("/")) path += "index.htm";
  if(path.indexOf("sec") > -1) return false;
  String contentType = getContentType(request, path);
  /*String pathWithGz = path + ".gz";
  if(WLED_FS.exists(pathWithGz)){
    request->send(WLED_FS, pathWithGz, contentType);
    return true;
  }*/
  if(WLED_FS.exists(path)) {
    request->send(WLED_FS, path, contentType);
    return true;
  }
  return false;
}



void mAnimatorLight::serveIndex(AsyncWebServerRequest* request)
{
  if (handleFileRead(request, "/index.htm"))
  {
    return;
  }

  if (handleIfNoneMatchCacheHeader(request))
  {
    return;
  }

  AsyncWebServerResponse *response;
  #ifdef WLED_ENABLE_SIMPLE_UI
  if (simplifiedUI)
    response = request->beginResponse_P(200, "text/html", PAGE_simple, PAGE_simple_L);
  else
  #endif
    response = request->beginResponse_P(200, "text/html", PAGE_index, PAGE_index_L);

  response->addHeader(FPSTR(s_content_enc),"gzip");
  setStaticContentCacheHeaders(response);
  request->send(response);
}

//get values for settings form in javascript
void mAnimatorLight::getSettingsJS(byte subPage, char* dest)
{
  //0: menu 1: wifi 2: leds 3: ui 4: sync 5: time 6: sec
  DEBUG_PRINT(F("settings resp"));
  DEBUG_PRINTLN(subPage);
  obuf = dest;
  olen = 0;

  if (subPage <0 || subPage >10) return;

  if (subPage == SUBPAGE_MENU)
  {
  #ifndef WLED_DISABLE_2D // include only if 2D is compiled in
    oappend(PSTR("gId('2dbtn').style.display='';"));
  #endif
  #ifdef WLED_ENABLE_DMX // include only if DMX is enabled
    oappend(PSTR("gId('dmxbtn').style.display='';"));
  #endif
  }

  if (subPage == SUBPAGE_WIFI)
  {
    sappends('s',SET_F("CS"),"clientSSID");

    byte l = strlen(clientPass);
    char fpass[l+1]; //fill password field with ***
    fpass[l] = 0;
    memset(fpass,'*',l);
    sappends('s',SET_F("CP"),fpass);

    char k[3]; k[2] = 0; //IP addresses
    for (int i = 0; i<4; i++)
    {
      k[1] = 48+i; //ascii 0,1,2,3
      k[0] = 'I'; sappend('v',k,staticIP[i]);
      k[0] = 'G'; sappend('v',k,staticGateway[i]);
      k[0] = 'S'; sappend('v',k,staticSubnet[i]);
    }

    sappends('s',SET_F("CM"),pCONT_set->Settings.system_name.device);
    sappend('i',SET_F("AB"),apBehavior);
    sappends('s',SET_F("AS"),apSSID);
    sappend('c',SET_F("AH"),apHide);

    l = strlen(apPass);
    char fapass[l+1]; //fill password field with ***
    fapass[l] = 0;
    memset(fapass,'*',l);
    sappends('s',SET_F("AP"),fapass);

    sappend('v',SET_F("AC"),apChannel);
    sappend('c',SET_F("WS"),noWifiSleep);

    #ifndef WLED_DISABLE_ESPNOW
    sappend('c',SET_F("RE"),enable_espnow_remote);
    sappends('s',SET_F("RMAC"),linked_remote);
    #else
    //hide remote settings if not compiled
    oappend(SET_F("document.getElementById('remd').style.display='none';"));
    #endif

    #ifdef WLED_USE_ETHERNET
    sappend('v',SET_F("ETH"),ethernetType);
    #else
    //hide ethernet setting if not compiled in
    oappend(SET_F("document.getElementById('ethd').style.display='none';"));
    #endif

    if (Network.isConnected()) //is connected
    {
      char s[32];
      IPAddress localIP = Network.localIP();
      sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);

      #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_ETHERNET)
      if (Network.isEthernet()) strcat_P(s ,SET_F(" (Ethernet)"));
      #endif
      sappends('m',SET_F("(\"sip\")[0]"),s);
    } else
    {
      sappends('m',SET_F("(\"sip\")[0]"),(char*)F("Not connected"));
    }

    if (WiFi.softAPIP()[0] != 0) //is active
    {
      char s[16];
      IPAddress apIP = WiFi.softAPIP();
      sprintf(s, "%d.%d.%d.%d", apIP[0], apIP[1], apIP[2], apIP[3]);
      sappends('m',SET_F("(\"sip\")[1]"),s);
    } else
    {
      sappends('m',SET_F("(\"sip\")[1]"),(char*)F("Not active"));
    }

    #ifndef WLED_DISABLE_ESPNOW
    if (last_signal_src[0] != 0) //Have seen an ESP-NOW Remote
    {
      sappends('m',SET_F("(\"rlid\")[0]"),last_signal_src);
    } else if (!enable_espnow_remote)
    {
      sappends('m',SET_F("(\"rlid\")[0]"),(char*)F("(Enable remote to listen)"));
    } else 
    {
      sappends('m',SET_F("(\"rlid\")[0]"),(char*)F("None"));
    }
    #endif
  }

  if (subPage == SUBPAGE_LEDS)
  {
    char nS[32];

    // appendGPIOinfo();

    // set limits
    // oappend(SET_F("bLimits("));
    // oappend(itoa(WLED_MAX_BUSSES,nS,10));  oappend(",");
    // oappend(itoa(WLED_MIN_VIRTUAL_BUSSES,nS,10));  oappend(",");
    // oappend(itoa(MAX_LEDS_PER_BUS,nS,10)); oappend(",");
    // oappend(itoa(MAX_LED_MEMORY,nS,10));   oappend(",");
    // oappend(itoa(MAX_LEDS,nS,10));
    // oappend(SET_F(");"));

  //   sappend('c',SET_F("MS"),autoSegments);
  //   sappend('c',SET_F("CCT"),correctWB);
  //   sappend('c',SET_F("CR"),cctFromRgb);
  //   sappend('v',SET_F("CB"),cctBlending);
  //   sappend('v',SET_F("FR"),getTargetFps());
  //   sappend('v',SET_F("AW"),Bus::getGlobalAWMode());
  //   sappend('c',SET_F("LD"),useLedsArray);

  //   for (uint8_t s=0; s < busses.getNumBusses(); s++) {
  //     Bus* bus = busses.getBus(s);
  //     if (bus == nullptr) continue;
  //     char lp[4] = "L0"; lp[2] = 48+s; lp[3] = 0; //ascii 0-9 //strip data pin
  //     char lc[4] = "LC"; lc[2] = 48+s; lc[3] = 0; //strip length
  //     char co[4] = "CO"; co[2] = 48+s; co[3] = 0; //strip color order
  //     char lt[4] = "LT"; lt[2] = 48+s; lt[3] = 0; //strip type
  //     char ls[4] = "LS"; ls[2] = 48+s; ls[3] = 0; //strip start LED
  //     char cv[4] = "CV"; cv[2] = 48+s; cv[3] = 0; //strip reverse
  //     char sl[4] = "SL"; sl[2] = 48+s; sl[3] = 0; //skip 1st LED
  //     char rf[4] = "RF"; rf[2] = 48+s; rf[3] = 0; //off refresh
  //     char aw[4] = "AW"; aw[2] = 48+s; aw[3] = 0; //auto white mode
  //     char wo[4] = "WO"; wo[2] = 48+s; wo[3] = 0; //swap channels
  //     char sp[4] = "SP"; sp[2] = 48+s; sp[3] = 0; //bus clock speed
  //     oappend(SET_F("addLEDs(1);"));
  //     uint8_t pins[5];
  //     uint8_t nPins = bus->getPins(pins);
  //     for (uint8_t i = 0; i < nPins; i++) {
  //       lp[1] = 48+i;
  //       if (pinManager.isPinOk(pins[i]) || bus->getType()>=TYPE_NET_DDP_RGB) sappend('v',lp,pins[i]);
  //     }
  //     sappend('v',lc,bus->getLength());
  //     sappend('v',lt,bus->getType());
  //     sappend('v',co,bus->getColorOrder() & 0x0F);
  //     sappend('v',ls,bus->getStart());
  //     sappend('c',cv,bus->reversed);
  //     sappend('v',sl,bus->skippedLeds());
  //     sappend('c',rf,bus->isOffRefreshRequired());
  //     sappend('v',aw,bus->getAutoWhiteMode());
  //     sappend('v',wo,bus->getColorOrder() >> 4);
  //     uint16_t speed = bus->getFrequency();
  //     if (bus->getType() > TYPE_ONOFF && bus->getType() < 48) {
  //       switch (speed) {
  //         case WLED_PWM_FREQ/3 : speed = 0; break;
  //         case WLED_PWM_FREQ/2 : speed = 1; break;
  //         default:
  //         case WLED_PWM_FREQ   : speed = 2; break;
  //         case WLED_PWM_FREQ*2 : speed = 3; break;
  //         case WLED_PWM_FREQ*3 : speed = 4; break;
  //       }
  //     } else {
  //       switch (speed) {
  //         case  1000 : speed = 0; break;
  //         case  2000 : speed = 1; break;
  //         default:
  //         case  5000 : speed = 2; break;
  //         case 10000 : speed = 3; break;
  //         case 20000 : speed = 4; break;
  //       }
  //     }
  //     sappend('v',sp,speed);
  //   }
  //   sappend('v',SET_F("MA"),ablMilliampsMax);
  //   sappend('v',SET_F("LA"),milliampsPerLed);
  //   if (currentMilliamps)
  //   {
  //     sappends('m',SET_F("(\"pow\")[0]"),(char*)"");
  //     olen -= 2; //delete ";
  //     oappendi(currentMilliamps);
  //     oappend(SET_F("mA\";"));
  //   }

  //   oappend(SET_F("resetCOM("));
  //   oappend(itoa(WLED_MAX_COLOR_ORDER_MAPPINGS,nS,10));
  //   oappend(SET_F(");"));
  //   const ColorOrderMap& com = busses.getColorOrderMap();
  //   for (uint8_t s=0; s < com.count(); s++) {
  //     const ColorOrderMapEntry* entry = com.get(s);
  //     if (entry == nullptr) break;
  //     oappend(SET_F("addCOM("));
  //     oappend(itoa(entry->start,nS,10));  oappend(",");
  //     oappend(itoa(entry->len,nS,10));  oappend(",");
  //     oappend(itoa(entry->colorOrder,nS,10));  oappend(");");
  //   }

  //   sappend('v',SET_F("CA"),briS);

  //   sappend('c',SET_F("BO"),turnOnAtBoot);
  //   sappend('v',SET_F("BP"),bootPreset);

  //   sappend('c',SET_F("GB"),gammaCorrectBri);
  //   sappend('c',SET_F("GC"),gammaCorrectCol);
  //   dtostrf(gammaCorrectVal,3,1,nS); sappends('s',SET_F("GV"),nS);
  //   sappend('c',SET_F("TF"),fadeTransition);
  //   sappend('v',SET_F("TD"),transitionDelayDefault);
  //   sappend('c',SET_F("PF"),paletteFade);
  //   sappend('v',SET_F("TP"),randomPaletteChangeTime);
  //   sappend('v',SET_F("BF"),briMultiplier);
  //   sappend('v',SET_F("TB"),nightlightTargetBri);
  //   sappend('v',SET_F("TL"),nightlightDelayMinsDefault);
  //   sappend('v',SET_F("TW"),nightlightMode);
  //   sappend('i',SET_F("PB"),paletteBlend);
  //   sappend('v',SET_F("RL"),rlyPin);
  //   sappend('c',SET_F("RM"),rlyMde);
  //   for (uint8_t i=0; i<WLED_MAX_BUTTONS; i++) {
  //     oappend(SET_F("addBtn("));
  //     oappend(itoa(i,nS,10));  oappend(",");
  //     oappend(itoa(btnPin[i],nS,10)); oappend(",");
  //     oappend(itoa(buttonType[i],nS,10));
  //     oappend(SET_F(");"));
  //   }
  //   sappend('c',SET_F("IP"),disablePullUp);
  //   sappend('v',SET_F("TT"),touchThreshold);
  //   sappend('v',SET_F("IR"),irPin);
  //   sappend('v',SET_F("IT"),irEnabled);
  //   sappend('c',SET_F("MSO"),!irApplyToAllSelected);
  }

  if (subPage == SUBPAGE_UI)
  {
  //   sappends('s',SET_F("DS"),serverDescription);
  //   sappend('c',SET_F("ST"),syncToggleReceive);
  // #ifdef WLED_ENABLE_SIMPLE_UI
  //   sappend('c',SET_F("SU"),simplifiedUI);
  // #else
  //   oappend(SET_F("toggle('Simple');"));    // hide Simplified UI settings
  // #endif
  }

  if (subPage == SUBPAGE_SYNC)
  {
  //   sappend('v',SET_F("UP"),udpPort);
  //   sappend('v',SET_F("U2"),udpPort2);
  //   sappend('v',SET_F("GS"),syncGroups);
  //   sappend('v',SET_F("GR"),receiveGroups);

  //   sappend('c',SET_F("RB"),receiveNotificationBrightness);
  //   sappend('c',SET_F("RC"),receiveNotificationColor);
  //   sappend('c',SET_F("RX"),receiveNotificationEffects);
  //   sappend('c',SET_F("SO"),receiveSegmentOptions);
  //   sappend('c',SET_F("SG"),receiveSegmentBounds);
  //   sappend('c',SET_F("SD"),notifyDirectDefault);
  //   sappend('c',SET_F("SB"),notifyButton);
  //   sappend('c',SET_F("SH"),notifyHue);
  //   sappend('c',SET_F("SM"),notifyMacro);
  //   sappend('v',SET_F("UR"),udpNumRetries);

  //   sappend('c',SET_F("NL"),nodeListEnabled);
  //   sappend('c',SET_F("NB"),nodeBroadcastEnabled);

  //   sappend('c',SET_F("RD"),receiveDirect);
  //   sappend('c',SET_F("MO"),useMainSegmentOnly);
  //   sappend('v',SET_F("EP"),e131Port);
  //   sappend('c',SET_F("ES"),e131SkipOutOfSequence);
  //   sappend('c',SET_F("EM"),e131Multicast);
  //   sappend('v',SET_F("EU"),e131Universe);
  //   sappend('v',SET_F("DA"),DMXAddress);
  //   sappend('v',SET_F("XX"),DMXSegmentSpacing);
  //   sappend('v',SET_F("PY"),e131Priority);
  //   sappend('v',SET_F("DM"),DMXMode);
  //   sappend('v',SET_F("ET"),realtimeTimeoutMs);
  //   sappend('c',SET_F("FB"),arlsForceMaxBri);
  //   sappend('c',SET_F("RG"),arlsDisableGammaCorrection);
  //   sappend('v',SET_F("WO"),arlsOffset);
  //   sappend('c',SET_F("AL"),alexaEnabled);
  //   sappends('s',SET_F("AI"),alexaInvocationName);
  //   sappend('c',SET_F("SA"),notifyAlexa);
  //   sappend('v',SET_F("AP"),alexaNumPresets);
  //   #ifdef WLED_DISABLE_ALEXA
  //   oappend(SET_F("toggle('Alexa');"));  // hide Alexa settings
  //   #endif

  //   #ifdef WLED_ENABLE_MQTT
  //   sappend('c',SET_F("MQ"),mqttEnabled);
  //   sappends('s',SET_F("MS"),mqttServer);
  //   sappend('v',SET_F("MQPORT"),mqttPort);
  //   sappends('s',SET_F("MQUSER"),mqttUser);
  //   byte l = strlen(mqttPass);
  //   char fpass[l+1]; //fill password field with ***
  //   fpass[l] = 0;
  //   memset(fpass,'*',l);
  //   sappends('s',SET_F("MQPASS"),fpass);
  //   sappends('s',SET_F("MQCID"),mqttClientID);
  //   sappends('s',"MD",mqttDeviceTopic);
  //   sappends('s',SET_F("MG"),mqttGroupTopic);
  //   sappend('c',SET_F("BM"),buttonPublishMqtt);
  //   sappend('c',SET_F("RT"),retainMqttMsg);
  //   #else
  //   oappend(SET_F("toggle('MQTT');"));    // hide MQTT settings
  //   #endif

  //   #ifndef WLED_DISABLE_HUESYNC
  //   sappend('v',SET_F("H0"),hueIP[0]);
  //   sappend('v',SET_F("H1"),hueIP[1]);
  //   sappend('v',SET_F("H2"),hueIP[2]);
  //   sappend('v',SET_F("H3"),hueIP[3]);
  //   sappend('v',SET_F("HL"),huePollLightId);
  //   sappend('v',SET_F("HI"),huePollIntervalMs);
  //   sappend('c',SET_F("HP"),huePollingEnabled);
  //   sappend('c',SET_F("HO"),hueApplyOnOff);
  //   sappend('c',SET_F("HB"),hueApplyBri);
  //   sappend('c',SET_F("HC"),hueApplyColor);
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

  //   sappends('m',SET_F("(\"sip\")[0]"),hueErrorString);
  //   #else
  //   oappend(SET_F("toggle('Hue');"));    // hide Hue Sync settings
  //   #endif
  //   sappend('v',SET_F("BD"),serialBaud);
  }

  if (subPage == SUBPAGE_TIME)
  {
  //   sappend('c',SET_F("NT"),ntpEnabled);
  //   sappends('s',SET_F("NS"),ntpServerName);
  //   sappend('c',SET_F("CF"),!useAMPM);
  //   sappend('i',SET_F("TZ"),currentTimezone);
  //   sappend('v',SET_F("UO"),utcOffsetSecs);
  //   char tm[32];
  //   dtostrf(longitude,4,2,tm);
  //   sappends('s',SET_F("LN"),tm);
  //   dtostrf(latitude,4,2,tm);
  //   sappends('s',SET_F("LT"),tm);
  //   getTimeString(tm);
  //   sappends('m',SET_F("(\"times\")[0]"),tm);
  //   if ((int)(longitude*10.) || (int)(latitude*10.)) {
  //     sprintf_P(tm, PSTR("Sunrise: %02d:%02d Sunset: %02d:%02d"), hour(sunrise), minute(sunrise), hour(sunset), minute(sunset));
  //     sappends('m',SET_F("(\"times\")[1]"),tm);
  //   }
  //   sappend('c',SET_F("OL"),overlayCurrent);
  //   sappend('v',SET_F("O1"),overlayMin);
  //   sappend('v',SET_F("O2"),overlayMax);
  //   sappend('v',SET_F("OM"),analogClock12pixel);
  //   sappend('c',SET_F("OS"),analogClockSecondsTrail);
  //   sappend('c',SET_F("O5"),analogClock5MinuteMarks);

  //   sappend('c',SET_F("CE"),countdownMode);
  //   sappend('v',SET_F("CY"),countdownYear);
  //   sappend('v',SET_F("CI"),countdownMonth);
  //   sappend('v',SET_F("CD"),countdownDay);
  //   sappend('v',SET_F("CH"),countdownHour);
  //   sappend('v',SET_F("CM"),countdownMin);
  //   sappend('v',SET_F("CS"),countdownSec);

  //   sappend('v',SET_F("A0"),macroAlexaOn);
  //   sappend('v',SET_F("A1"),macroAlexaOff);
  //   sappend('v',SET_F("MC"),macroCountdown);
  //   sappend('v',SET_F("MN"),macroNl);
  //   for (uint8_t i=0; i<WLED_MAX_BUTTONS; i++) {
  //     oappend(SET_F("addRow("));
  //     oappend(itoa(i,tm,10));  oappend(",");
  //     oappend(itoa(macroButton[i],tm,10)); oappend(",");
  //     oappend(itoa(macroLongPress[i],tm,10)); oappend(",");
  //     oappend(itoa(macroDoublePress[i],tm,10));
  //     oappend(SET_F(");"));
  //   }

  //   char k[4];
  //   k[2] = 0; //Time macros
  //   for (int i = 0; i<10; i++)
  //   {
  //     k[1] = 48+i; //ascii 0,1,2,3
  //     if (i<8) { k[0] = 'H'; sappend('v',k,timerHours[i]); }
  //     k[0] = 'N'; sappend('v',k,timerMinutes[i]);
  //     k[0] = 'T'; sappend('v',k,timerMacro[i]);
  //     k[0] = 'W'; sappend('v',k,timerWeekday[i]);
  //     if (i<8) {
  //       k[0] = 'M'; sappend('v',k,(timerMonth[i] >> 4) & 0x0F);
	// 			k[0] = 'P'; sappend('v',k,timerMonth[i] & 0x0F);
  //       k[0] = 'D'; sappend('v',k,timerDay[i]);
	// 			k[0] = 'E'; sappend('v',k,timerDayEnd[i]);
  //     }
  //   }
  }

  if (subPage == SUBPAGE_SEC)
  {
  //   byte l = strlen(settingsPIN);
  //   char fpass[l+1]; //fill PIN field with 0000
  //   fpass[l] = 0;
  //   memset(fpass,'0',l);
  //   sappends('s',SET_F("PIN"),fpass);
  //   sappend('c',SET_F("NO"),otaLock);
  //   sappend('c',SET_F("OW"),wifiLock);
  //   sappend('c',SET_F("AO"),aOtaEnabled);
  //   sappends('m',SET_F("(\"sip\")[0]"),(char*)F("WLED "));
  //   olen -= 2; //delete ";
  //   oappend(versionString);
  //   oappend(SET_F(" (build "));
  //   oappendi(VERSION);
  //   oappend(SET_F(")\";"));
  //   oappend(SET_F("sd=\""));
  //   oappend(serverDescription);
  //   oappend(SET_F("\";"));
  // }

  // #ifdef WLED_ENABLE_DMX // include only if DMX is enabled
  // if (subPage == SUBPAGE_DMX)
  // {
  //   sappend('v',SET_F("PU"),e131ProxyUniverse);

  //   sappend('v',SET_F("CN"),DMXChannels);
  //   sappend('v',SET_F("CG"),DMXGap);
  //   sappend('v',SET_F("CS"),DMXStart);
  //   sappend('v',SET_F("SL"),DMXStartLED);

  //   sappend('i',SET_F("CH1"),DMXFixtureMap[0]);
  //   sappend('i',SET_F("CH2"),DMXFixtureMap[1]);
  //   sappend('i',SET_F("CH3"),DMXFixtureMap[2]);
  //   sappend('i',SET_F("CH4"),DMXFixtureMap[3]);
  //   sappend('i',SET_F("CH5"),DMXFixtureMap[4]);
  //   sappend('i',SET_F("CH6"),DMXFixtureMap[5]);
  //   sappend('i',SET_F("CH7"),DMXFixtureMap[6]);
  //   sappend('i',SET_F("CH8"),DMXFixtureMap[7]);
  //   sappend('i',SET_F("CH9"),DMXFixtureMap[8]);
  //   sappend('i',SET_F("CH10"),DMXFixtureMap[9]);
  //   sappend('i',SET_F("CH11"),DMXFixtureMap[10]);
  //   sappend('i',SET_F("CH12"),DMXFixtureMap[11]);
  //   sappend('i',SET_F("CH13"),DMXFixtureMap[12]);
  //   sappend('i',SET_F("CH14"),DMXFixtureMap[13]);
  //   sappend('i',SET_F("CH15"),DMXFixtureMap[14]);
  }
  // #endif

  if (subPage == SUBPAGE_UM) //usermods
  {
  //   appendGPIOinfo();
  //   oappend(SET_F("numM="));
  //   oappendi(usermods.getModCount());
  //   oappend(";");
  //   sappend('v',SET_F("SDA"),i2c_sda);
  //   sappend('v',SET_F("SCL"),i2c_scl);
  //   sappend('v',SET_F("MOSI"),spi_mosi);
  //   sappend('v',SET_F("MISO"),spi_miso);
  //   sappend('v',SET_F("SCLK"),spi_sclk);
  //   oappend(SET_F("addInfo('SDA','"));  oappendi(HW_PIN_SDA);      oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('SCL','"));  oappendi(HW_PIN_SCL);      oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('MOSI','")); oappendi(HW_PIN_DATASPI);  oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('MISO','")); oappendi(HW_PIN_MISOSPI);  oappend(SET_F("');"));
  //   oappend(SET_F("addInfo('SCLK','")); oappendi(HW_PIN_CLOCKSPI); oappend(SET_F("');"));
  //   usermods.appendConfigData();
  // }

  // if (subPage == SUBPAGE_UPDATE) // update
  // {
  //   sappends('m',SET_F("(\"sip\")[0]"),(char*)F("WLED "));
  //   olen -= 2; //delete ";
  //   oappend(versionString);
  //   oappend(SET_F("<br>("));
  //   #if defined(ARDUINO_ARCH_ESP32)
  //   oappend(ESP.getChipModel());
  //   #else
  //   oappend("esp8266");
  //   #endif
  //   oappend(SET_F(" build "));
  //   oappendi(VERSION);
  //   oappend(SET_F(")\";"));
  // }

  // if (subPage == SUBPAGE_2D) // 2D matrices
  // {
  //   sappend('v',SET_F("SOMP"),isMatrix);
  //   #ifndef WLED_DISABLE_2D
  //   oappend(SET_F("maxPanels=")); oappendi(WLED_MAX_PANELS); oappend(SET_F(";"));
  //   oappend(SET_F("resetPanels();"));
  //   if (isMatrix) {
  //     if(panels>0){
  //       sappend('v',SET_F("PW"),panel[0].width); //Set generator Width and Height to first panel size for convenience
  //       sappend('v',SET_F("PH"),panel[0].height);
  //     }
  //     sappend('v',SET_F("MPC"),panels);
  //     // panels
  //     for (uint8_t i=0; i<panels; i++) {
  //       char n[5];
  //       oappend(SET_F("addPanel("));
  //       oappend(itoa(i,n,10));
  //       oappend(SET_F(");"));
  //       char pO[8] = { '\0' };
  //       snprintf_P(pO, 7, PSTR("P%d"), i);       // MAX_PANELS is 64 so pO will always only be 4 characters or less
  //       pO[7] = '\0';
  //       uint8_t l = strlen(pO);
  //       // create P0B, P1B, ..., P63B, etc for other PxxX
  //       pO[l] = 'B'; sappend('v',pO,panel[i].bottomStart);
  //       pO[l] = 'R'; sappend('v',pO,panel[i].rightStart);
  //       pO[l] = 'V'; sappend('v',pO,panel[i].vertical);
  //       pO[l] = 'S'; sappend('c',pO,panel[i].serpentine);
  //       pO[l] = 'X'; sappend('v',pO,panel[i].xOffset);
  //       pO[l] = 'Y'; sappend('v',pO,panel[i].yOffset);
  //       pO[l] = 'W'; sappend('v',pO,panel[i].width);
  //       pO[l] = 'H'; sappend('v',pO,panel[i].height);
  //     }
  //   }
  //   #else
  //   oappend(SET_F("gId(\"somp\").remove(1);")); // remove 2D option from dropdown
  //   #endif
  }
}





void mAnimatorLight::handleUpload(AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
  if (!correctPIN) {
    if (final) request->send(500, "text/plain", FPSTR(s_unlock_cfg));
    return;
  }
  if (!index) {
    String finalname = filename;
    if (finalname.charAt(0) != '/') {
      finalname = '/' + finalname; // prepend slash if missing
    }

    request->_tempFile = WLED_FS.open(finalname, "w");
    DEBUG_PRINT(F("Uploading "));
    DEBUG_PRINTLN(finalname);
    if (finalname.equals("/presets.json"))
    { 
      // presetsModifiedTime = toki.second();
    }
  }
  if (len) {
    request->_tempFile.write(data,len);
  }
  if (final) {
    request->_tempFile.close();
    if (filename.indexOf(F("cfg.json")) >= 0) { // check for filename with or without slash
      doReboot = true;
      request->send(200, "text/plain", F("Configuration restore successful.\nRebooting..."));
    } else {
      if (filename.indexOf(F("palette")) >= 0 && filename.indexOf(F(".json")) >= 0)
      {
        // loadCustomPalettes();
      }
      request->send(200, "text/plain", F("File Uploaded!"));
    }
    cacheInvalidate++;
  }
}



// ESP32-WROVER features SPI RAM (aka PSRAM) which can be allocated using ps_malloc()
// we can create custom PSRAMDynamicJsonDocument to use such feature (replacing DynamicJsonDocument)
// The following is a construct to enable code to compile without it.
// There is a code thet will still not use PSRAM though:
//    AsyncJsonResponse is a derived class that implements DynamicJsonDocument (AsyncJson-v6.h)
#if defined(ARDUINO_ARCH_ESP32) && defined(BOARD_HAS_PSRAM) && defined(WLED_USE_PSRAM)
struct PSRAM_Allocator {
  void* allocate(size_t size) {
    if (psramFound()) return ps_malloc(size); // use PSRAM if it exists
    else              return malloc(size);    // fallback
  }
  void* reallocate(void* ptr, size_t new_size) {
    if (psramFound()) return ps_realloc(ptr, new_size); // use PSRAM if it exists
    else              return realloc(ptr, new_size);    // fallback
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};
using PSRAMDynamicJsonDocument = BasicJsonDocument<PSRAM_Allocator>;
#else
#define PSRAMDynamicJsonDocument DynamicJsonDocument
#endif

//threading/network callback details: https://github.com/Aircoookie/WLED/pull/2336#discussion_r762276994
bool mAnimatorLight::requestJSONBufferLock(uint8_t module)
{
  unsigned long now = millis();

  // This assumption here is another http thread must release itself to permit this function to proceed
  while (jsonBufferLock && millis()-now < 1000) delay(1); // wait for a second for buffer lock

  if (millis()-now >= 1000) {
    DEBUG_PRINT(F("ERROR: Locking JSON buffer failed! ("));
    DEBUG_PRINT(jsonBufferLock);
    DEBUG_PRINTLN(")");
    return false; // waiting time-outed
  }

  jsonBufferLock = module ? module : 255;
  DEBUG_PRINT(F("LOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOCKED    JSON buffer locked. ("));
  DEBUG_PRINT(jsonBufferLock);
  DEBUG_PRINTLN(")");
  // fileDoc = &doc;  // used for applying presets (presets.cpp)
  // doc.clear();
  return true;
}

void mAnimatorLight::releaseJSONBufferLock()
{
  DEBUG_PRINT(F("UUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUUNLOCKED JSON buffer released. ("));
  DEBUG_PRINT(jsonBufferLock);
  DEBUG_PRINTLN(")");
  // fileDoc = nullptr;
  jsonBufferLock = 0;
}


void mAnimatorLight::serializeSegment(JsonObject& root, mAnimatorLight::Segment_New& seg, byte id, bool forPreset, bool segmentBounds)
{
  root["id"] = id;
  if (segmentBounds) {
    root["start"] = seg.pixel_range.start;
    root["stop"] = seg.pixel_range.stop;
    if (isMatrix) {
      root[F("startY")] = seg.startY;
      root[F("stopY")]  = seg.stopY;
    }
  }
  if (!forPreset) root["len"] = seg.pixel_range.stop - seg.pixel_range.start;
  root["grp"]    = seg.grouping;
  root[F("spc")] = seg.spacing;
  root[F("of")]  = seg.offset;
  root["on"]     = seg.on;
  root["frz"]    = seg.freeze;
  byte segbri    = seg.opacity;
  root["bri"]    = (segbri) ? segbri : 255;
  root["cct"]    = seg.cct;
  root[F("set")] = 0;//seg.set;

  if (segmentBounds && seg.name != nullptr) root["n"] = reinterpret_cast<const char *>(seg.name); //not good practice, but decreases required JSON buffer

  // to conserve RAM we will serialize the col array manually
  // this will reduce RAM footprint from ~300 bytes to 84 bytes per segment
  char colstr[70]; colstr[0] = '['; colstr[1] = '\0';  //max len 68 (5 chan, all 255)
  const char *format = hasWhiteChannel() ? PSTR("[%u,%u,%u,%u]") : PSTR("[%u,%u,%u]");
  for (size_t i = 0; i < 5; i++)
  {
    byte segcol[4]; byte* c = segcol;
    segcol[0] = seg.rgbcctcolors[i].R;
    segcol[1] = seg.rgbcctcolors[i].G;
    segcol[2] = seg.rgbcctcolors[i].B;
    segcol[3] = seg.rgbcctcolors[i].W1;
    char tmpcol[22];
    sprintf_P(tmpcol, format, (unsigned)c[0], (unsigned)c[1], (unsigned)c[2], (unsigned)c[3]);
    strcat(colstr, i<4 ? strcat(tmpcol, ",") : tmpcol);
  }
  strcat(colstr, "]");
  ALOG_INF(PSTR("colstr OUT = %s"), colstr);
  root["col"] = serialized(colstr);

  root["fx"]  = seg.effect_id;
  root["sx"]  = seg.speed();
  root["ix"]  = seg.intensity();

  // root["tt"]  = constrain(map(seg.transition.time_ms, 0,10000, 0,255),0,255);
  // root["tr"]  = constrain(map(seg.transition.rate_ms, 0,10000, 0,255),0,255);
  root["tt"]  = seg.transition.time_ms;//, 0,10000, 0,255),0,255);
  root["tr"]  = seg.transition.rate_ms;//, 0,10000, 0,255),0,255);

  root["pal"] = seg.palette.id;
  root["c1"]  = seg.custom1;
  root["c2"]  = seg.custom2;
  root["c3"]  = seg.custom3;
  root["sel"] = seg.isSelected();
  root["rev"] = seg.reverse;
  root["mi"]  = seg.mirror;
  #ifndef WLED_DISABLE_2D
  if (isMatrix) {
    root["rY"] = seg.reverse_y;
    root["mY"] = seg.mirror_y;
    root[F("tp")] = seg.transpose;
  }
  #endif
  root["o1"]  = seg.check1;
  root["o2"]  = seg.check2;
  root["o3"]  = seg.check3;
  root["si"]  = seg.soundSim;
  root["m12"] = seg.map1D2D;
}

void mAnimatorLight::serializeState(JsonObject root, bool forPreset, bool includeBri, bool segmentBounds, bool selectedSegmentsOnly)
{
  if (includeBri) {
    root["on"] = pCONT_iLight->_briRGB_Global > 0;//(bri > 0);
    root["bri"] = pCONT_iLight->_briRGB_Global;//briLast;
    root[F("transition")] = transitionDelay/100; //in 100ms
  }

  if (!forPreset) {
    if (errorFlag) {root[F("error")] = errorFlag; errorFlag = ERR_NONE;} //prevent error message to persist on screen

    root["ps"] = (currentPreset > 0) ? currentPreset : -1;
    root[F("pl")] = currentPlaylist;

    JsonObject nl = root.createNestedObject("nl");
    nl["on"] = nightlightActive;
    nl["dur"] = nightlightDelayMins;
    nl["mode"] = nightlightMode;
    nl[F("tbri")] = nightlightTargetBri;
    if (nightlightActive) {
      nl[F("rem")] = (nightlightDelayMs - (millis() - nightlightStartTime)) / 1000; // seconds remaining
    } else {
      nl[F("rem")] = -1;
    }

    JsonObject udpn = root.createNestedObject("udpn");
    udpn["send"] = notifyDirect;
    udpn["recv"] = receiveNotifications;

    root[F("lor")] = realtimeOverride;
  }

  root[F("mainseg")] = getMainSegmentId();

  JsonArray seg = root.createNestedArray("seg");
  for (size_t s = 0; s < getMaxSegments(); s++) {
    if (s >= getSegmentsNum()) {
      if (forPreset && segmentBounds && !selectedSegmentsOnly) { //disable segments not part of preset
        JsonObject seg0 = seg.createNestedObject();
        seg0["stop"] = 0;
        continue;
      } else
        break;
    }
    mAnimatorLight::Segment_New &sg = getSegment(s);
    if (forPreset && selectedSegmentsOnly && !sg.isSelected()) continue;
    if (sg.isActive()) {
      JsonObject seg0 = seg.createNestedObject();
      serializeSegment(seg0, sg, s, forPreset, segmentBounds);
    } else if (forPreset && segmentBounds) { //disable segments not part of preset
      JsonObject seg0 = seg.createNestedObject();
      seg0["stop"] = 0;
    }
  }


}

void mAnimatorLight::serializeInfo(JsonObject root)
{
  root[F("ver")] = "versionString";
  root[F("vid")] = PROJECT_VERSION;
  root[F("cn")] = "WLED_CODENAME";

  JsonObject leds = root.createNestedObject("leds");
  leds[F("count")] = 123;//getLengthTotal();
  leds[F("pwr")] = 123;//currentMilliamps;
  leds["fps"] = getFps();
  leds[F("maxpwr")] = (currentMilliamps)? ablMilliampsMax : 0;
  leds[F("maxseg")] = getMaxSegments();
  leds[F("actseg")] = getActiveSegmentsNum();
  leds[F("seglock")] = false; //might be used in the future to prevent modifications to segment config

  #ifndef WLED_DISABLE_2D
  if (isMatrix) {
    JsonObject matrix = leds.createNestedObject("matrix");
    matrix["w"] = 1;//mAnimatorLight::Segment_New::maxWidth;
    matrix["h"] = 2;//mAnimatorLight::Segment_New::maxHeight;
  }
  #endif

  uint8_t totalLC = 0;
  JsonArray lcarr = leds.createNestedArray(F("seglc"));
  size_t nSegs = getSegmentsNum();
  for (size_t s = 0; s < nSegs; s++) {
    if (!getSegment(s).isActive()) continue;
    uint8_t lc = getSegment(s).getLightCapabilities();
    totalLC |= lc;
    lcarr.add(lc);  // This is what enable colour picker!!!
  }

  leds["lc"] = totalLC;

  // leds[F("rgbw")] = hasRGBWBus(); // deprecated, use info.leds.lc
  leds[F("wv")]   = totalLC & 0x02;     // deprecated, true if white slider should be displayed for any segment
  leds["cct"]     = totalLC & 0x04;     // deprecated, use info.leds.lc

  #ifdef WLED_DEBUG
  JsonArray i2c = root.createNestedArray(F("i2c"));
  i2c.add(i2c_sda);
  i2c.add(i2c_scl);
  JsonArray spi = root.createNestedArray(F("spi"));
  spi.add(spi_mosi);
  spi.add(spi_sclk);
  spi.add(spi_miso);
  #endif

  root[F("str")] = syncToggleReceive;

  root[F("name")] = serverDescription;
  root[F("udpport")] = 123;//udpPort;
  root["live"] = (bool)realtimeMode;
  root[F("liveseg")] = -1;//useMainSegmentOnly ? getMainSegmentId() : -1;  // if using main segment only for live

  switch (realtimeMode) {
    case REALTIME_MODE_INACTIVE: root["lm"] = ""; break;
    case REALTIME_MODE_GENERIC:  root["lm"] = ""; break;
    case REALTIME_MODE_UDP:      root["lm"] = F("UDP"); break;
    case REALTIME_MODE_HYPERION: root["lm"] = F("Hyperion"); break;
    case REALTIME_MODE_E131:     root["lm"] = F("E1.31"); break;
    case REALTIME_MODE_ADALIGHT: root["lm"] = F("USB Adalight/TPM2"); break;
    case REALTIME_MODE_ARTNET:   root["lm"] = F("Art-Net"); break;
    case REALTIME_MODE_TPM2NET:  root["lm"] = F("tpm2.net"); break;
    case REALTIME_MODE_DDP:      root["lm"] = F("DDP"); break;
  }

  if (realtimeIP[0] == 0)
  {
    root[F("lip")] = "";
  } else {
    root[F("lip")] = realtimeIP.toString();
  }

  #ifdef WLED_ENABLE_WEBSOCKETS
  root[F("ws")] = ws.count();
  #else
  root[F("ws")] = -1;
  #endif

  root[F("fxcount")] = getModeCount();
  root[F("palcount")] = getPaletteCount();
  root[F("cpalcount")] = customPalettes.size(); //number of custom palettes

  JsonArray ledmaps = root.createNestedArray(F("maps"));
  // for (size_t i=0; i<WLED_MAX_LEDMAPS; i++) {
  //   if ((ledMaps>>i) & 0x00000001U) {
  //     JsonObject ledmaps0 = ledmaps.createNestedObject();
  //     ledmaps0["id"] = i;
  //     #ifndef ESP8266
  //     if (i && ledmapNames[i-1]) ledmaps0["n"] = ledmapNames[i-1];
  //     #endif
  //   }
  // }

  JsonObject wifi_info = root.createNestedObject("wifi");
  wifi_info[F("bssid")] = WiFi.BSSIDstr();
  int qrssi = WiFi.RSSI();
  wifi_info[F("rssi")] = qrssi;
  wifi_info[F("signal")] = 126;//getSignalQuality(qrssi);
  wifi_info[F("channel")] = WiFi.channel();

  JsonObject fs_info = root.createNestedObject("fs");
  fs_info["u"] = 123;//fsBytesUsed / 1000;
  fs_info["t"] = 123;//fsBytesTotal / 1000;
  fs_info[F("pmt")] = 123;//presetsModifiedTime;

  root[F("ndc")] =123;// nodeListEnabled ? (int)Nodes.size() : -1;

  #ifdef ARDUINO_ARCH_ESP32
  #ifdef WLED_DEBUG
    wifi_info[F("txPower")] = (int) WiFi.getTxPower();
    wifi_info[F("sleep")] = (bool) WiFi.getSleep();
  #endif
  #if !defined(CONFIG_IDF_TARGET_ESP32C2) && !defined(CONFIG_IDF_TARGET_ESP32C3) && !defined(CONFIG_IDF_TARGET_ESP32S2) && !defined(CONFIG_IDF_TARGET_ESP32S3)
    root[F("arch")] = "esp32";
  #else
    root[F("arch")] = ESP.getChipModel();
  #endif
  root[F("core")] = ESP.getSdkVersion();
  //root[F("maxalloc")] = ESP.getMaxAllocHeap();
  #ifdef WLED_DEBUG
    root[F("resetReason0")] = (int)rtc_get_reset_reason(0);
    root[F("resetReason1")] = (int)rtc_get_reset_reason(1);
  #endif
  root[F("lwip")] = 0; //deprecated
  #else
  root[F("arch")] = "esp8266";
  root[F("core")] = ESP.getCoreVersion();
  //root[F("maxalloc")] = ESP.getMaxFreeBlockSize();
  #ifdef WLED_DEBUG
    root[F("resetReason")] = (int)ESP.getResetInfoPtr()->reason;
  #endif
  root[F("lwip")] = LWIP_VERSION_MAJOR;
  #endif

  root[F("freeheap")] = ESP.getFreeHeap();
  #if defined(ARDUINO_ARCH_ESP32) && defined(BOARD_HAS_PSRAM)
  if (psramFound()) root[F("psram")] = ESP.getFreePsram();
  #endif
  // root[F("uptime")] = millis()/1000 + rolloverMillis*4294967;

  uint16_t os = 0;
  #ifdef WLED_DEBUG
  os  = 0x80;
    #ifdef WLED_DEBUG_HOST
    os |= 0x0100;
    if (!netDebugEnabled) os &= ~0x0080;
    #endif
  #endif
  #ifndef WLED_DISABLE_ALEXA
  os += 0x40;
  #endif

  //os += 0x20; // indicated now removed Blynk support, may be reused to indicate another build-time option

  #ifdef USERMOD_CRONIXIE
  os += 0x10;
  #endif
  #ifndef WLED_DISABLE_FILESYSTEM
  os += 0x08;
  #endif
  #ifndef WLED_DISABLE_HUESYNC
  os += 0x04;
  #endif
  #ifdef WLED_ENABLE_ADALIGHT
  os += 0x02;
  #endif
  #ifndef WLED_DISABLE_OTA
  os += 0x01;
  #endif
  root[F("opt")] = os;

  root[F("brand")] = "WLED";
  root[F("product")] = F("FOSS");
  // root["mac"] = escapedMac;
  char s[16] = "";
  // if (Network.isConnected())
  // {
  //   IPAddress localIP = Network.localIP();
  //   sprintf(s, "%d.%d.%d.%d", localIP[0], localIP[1], localIP[2], localIP[3]);
  // }
  root["ip"] = s;
}


void mAnimatorLight::setPaletteColors(JsonArray json, CRGBPalette16 palette)
{
    for (int i = 0; i < 16; i++) {
      JsonArray colors =  json.createNestedArray();
      CRGB color = palette[i];
      colors.add(i<<4);
      colors.add(color.red);
      colors.add(color.green);
      colors.add(color.blue);
    }
}


void mAnimatorLight::serializePalettes(JsonObject root, int page)
{
  byte tcp[72];
  #ifdef ESP8266
  int itemPerPage = 5;
  #else
  int itemPerPage = 8;
  #endif

  int palettesCount = mPaletteI->GetPaletteListLength();
  int customPalettes = pCONT_lAni->customPalettes.size();

  // ALOG_HGL(PSTR("palettesCount=%d"), palettesCount); 

  int maxPage = (palettesCount + customPalettes -1) / itemPerPage;
  if (page > maxPage) page = maxPage;

  int start = itemPerPage * page;
  int end = start + itemPerPage;
  if (end > palettesCount + customPalettes) end = palettesCount + customPalettes;

  root[F("m")] = maxPage; // inform caller how many pages there are
  JsonObject palettes  = root.createNestedObject("p");
  JsonObject palettes_style  = root.createNestedObject("s");

  uint8_t encoded_gradient = 0;

/**
 * @brief 
 * Start by sending the current palette loaded
 */

  for (int palette_id = start; palette_id < end; palette_id++) 
  {
        
    bool banded_gradient = false;

    ALOG_INF(PSTR("i=%d|p%d|m%d"),palette_id,page,maxPage);

    LoadPalette(palette_id); // Assume segment 1 exists, and use it to load all palettes. Effect should reset to active palette in main loop. Or here, have it then flip back. Though this may cause flickering midanimation. Animation may also need paused on esp32.
    uint16_t colours_in_palette = GetNumberOfColoursInPalette(palette_id);
   
    ALOG_INF(PSTR("colours_in_palette=%d"),colours_in_palette);

    JsonArray curPalette_obj = palettes.createNestedArray(String(palette_id));
    JsonObject curPalette_s_obj = palettes_style.createNestedObject(String(palette_id));

    // palettes.createNestedObject("")
    // JsonObject obj2 = curPalette_obj.createNestedObject();
    //   obj2["TEST"] = 1;//.add("test",1);

    /**
     * @brief To reduce memory usage, the static gradients that are stored with less than 16 colours, shall be read directly
     * 
     */
    if(
      (palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID) && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID)
    ){ 

      uint8_t adjusted_id = palette_id - mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID;

      byte tcp[72];
      memcpy_P(tcp, (byte*)pgm_read_dword(&(gGradientPalettes[adjusted_id])), 72);

      TRGBGradientPaletteEntryUnion* ent = (TRGBGradientPaletteEntryUnion*)(tcp);
      TRGBGradientPaletteEntryUnion u;

      // Count entries
      uint16_t count = 0;
      do {
          u = *(ent + count);
          count++;
      } while ( u.index != 255);

      ALOG_INF(PSTR("palette_id%d,count=%d"),palette_id,count);

      u = *ent;
      int indexstart = 0;
      while( indexstart < 255) {
        indexstart = u.index;

        JsonArray colors =  curPalette_obj.createNestedArray();
        colors.add(u.index);
        colors.add(u.r);
        colors.add(u.g);
        colors.add(u.b);

        ent++;
        u = *ent;
      }
      
      banded_gradient = false;

    }
    else
    {

      
      for (int j = 0; j < colours_in_palette; j++) {

        encoded_gradient = 0;
        
        RgbcctColor color =    RgbcctColor(0);
      
        
        color = mPaletteI->GetColourFromPreloadedPaletteBuffer(
            palette_id, segments[getCurrSegmentId()].palette_container->pData.data(),//desired_index_from_palette,  
            j, &encoded_gradient,
            false, false, true
          );

        JsonArray colors =  curPalette_obj.createNestedArray();

        WDT_Reset();

        // Serial.println(encoded_gradient);

        /**
         * @brief 
         * First colour needs to be applied twice, or at least have the index increased
         * 
         */

        /**
         * @brief 
         * Gradient Exists: encoded > 0, then take it directly. If it exists but is 0, then the else will still work to catch it
         * Gradient Empty:  encoded == 0, scale colour_count into range
         */
        if(encoded_gradient)
        {
          
          // #ifdef ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW
          // uint8_t range_width = 255/(colours_in_palette+1);
          // colors.add(map(encoded_gradient, 0,255, range_width,255)); // Rescale encoded value into new scale
          // #else
          colors.add(encoded_gradient); // when palette has gradient index value, it should be used instead of this
          // #endif
                    

        }
        else
        {
          // #ifdef ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW
          // uint8_t range_width = 255/(colours_in_palette+1);
          // colors.add(map(j, 0,colours_in_palette, range_width,255)); // when palette has gradient index value, it should be used instead of this
          // #else
          // colors.add(map(j, 0,colours_in_palette, 0,255)); // when palette has gradient index value, it should be used instead of this
          // #endif

          #ifdef ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW
          // uint8_t range_width = 255/(colours_in_palette+1);
          colors.add(map(j, 0,colours_in_palette-1, 0,255)); // when palette has gradient index value, it should be used instead of this
          #else
          colors.add(map(j, 0,colours_in_palette, 0,255)); // when palette has gradient index value, it should be used instead of this
          #endif

          banded_gradient = true;
                    

        }

        colors.add(color.red);
        colors.add(color.green);
        colors.add(color.blue);

      }

            
        // ALOG_INF(PSTR("palette_id %d"), palette_id);
      /**************************************************************
       * 
       * PALETTELIST_FIXED__IDS
       * PALETTELIST_VARIABLE_HSBID__IDS
       * PALETTELIST_VARIABLE_GENERIC__IDS
       * 
      ***************************************************************/
      if(
        ((palette_id >= mPalette::PALETTELIST_FIXED_COLOURFUL_DEFAULT__ID) && (palette_id < mPalette::PALETTELIST_FIXED_LENGTH__ID))
         ||
        // ((palette_id >= mPalette::PALETTELIST_VARIABLE_HSBID_01__ID)    && (palette_id < mPalette::PALETTELIST_VARIABLE_HSBID_LENGTH__ID)) ||
        ((palette_id >= mPalette::PALETTELIST_LENGTH_OF_STATIC_IDS)  && (palette_id < mPaletteI->GetPaletteListLength()))
      )
      {  
          
        banded_gradient = true;

        mPalette::STATIC_PALETTE *ptr = &mPaletteI->palettelist[palette_id];

        if(ptr->encoding.index_scaled_to_segment)
        {
          banded_gradient = false;
        };

      }

      // /**************************************************************
      //  * 
      //  * PALETTELIST_FIXED_HTML_COLOUR_CODES__IDS
      //  * 
      // ***************************************************************/
      // if(
      //   (palette_id >= mPalette::PALETTELIST_HTML_COLOUR__AliceBlue__ID) && (palette_id < mPalette::PALETTELIST_HTML_COLOUR__LENGTH__ID)
      // ){  
      //   banded_gradient = false;
      // }

      /**************************************************************
       * 
       * PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR__IDS
       * 
      ***************************************************************/
      if(
        (palette_id >= mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_01__ID) && (palette_id < mPalette::PALETTELIST_MODIFIABLE__RGBCCT_SEGMENT_COLOUR_LENGTH__ID)
      ){  
        banded_gradient = false;
      }

      /**************************************************************
       * 
       * PALETTELIST_FIXED_CRGBPALETTE16__IDS
       * PALETTELIST_CRGBPALETTE16_GRADIENT___PALETTES__IDS
       * 
      ***************************************************************/
      if(
        ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16__CLOUD_COLOURS__ID) && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16__LENGTH__ID)) ||
        ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT__SUNSET__ID)    && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16_GRADIENT_LENGTH__ID))
        
        ||
        ((palette_id >= mPalette::PALETTELIST_FIXED_CRGBPALETTE16__RANDOMISE_COLOURS_01__ID)    && (palette_id < mPalette::PALETTELIST_FIXED_CRGBPALETTE16_USER__LENGTH__ID))
      ){  
        banded_gradient = false;
      }

      if(banded_gradient)
      {
        curPalette_s_obj["bg"] = "L";//"B";
      }else{
        curPalette_s_obj["bg"] = "L";
      }
          
    }


  }




// return;

//   for (int i = start; i < end; i++) {
//     JsonArray curPalette = palettes.createNestedArray(String(i>=palettesCount ? 255 - i + palettesCount : i));
//     switch (i) {
//       case 1: //random
//           curPalette.add("r");
//           curPalette.add("r");
//           curPalette.add("r");
//           curPalette.add("r");
//         break;
//       case 2: //primary color only
//         curPalette.add("c1");
//         break;
//       case 3: //primary + secondary
//         curPalette.add("c1");
//         curPalette.add("c1");
//         curPalette.add("c2");
//         curPalette.add("c2");
//         break;
//       case 4: //primary + secondary + tertiary
//         curPalette.add("c3");
//         curPalette.add("c2");
//         curPalette.add("c1");
//         break;
//       case 5: //primary + secondary (+tert if not off), more distinct
//         curPalette.add("c1");
//         curPalette.add("c1");
//         curPalette.add("c1");
//         curPalette.add("c1");
//         curPalette.add("c1");
//         curPalette.add("c2");
//         curPalette.add("c2");
//         curPalette.add("c2");
//         curPalette.add("c2");
//         curPalette.add("c2");
//         curPalette.add("c3");
//         curPalette.add("c3");
//         curPalette.add("c3");
//         curPalette.add("c3");
//         curPalette.add("c3");
//         curPalette.add("c1");
//         break;
//   }



}


void mAnimatorLight::serializeNetworks(JsonObject root)
{
  JsonArray networks = root.createNestedArray(F("networks"));
  int16_t status = WiFi.scanComplete();

  switch (status) {
    case WIFI_SCAN_FAILED:
      WiFi.scanNetworks(true);
      return;
    case WIFI_SCAN_RUNNING:
      return;
  }

  for (int i = 0; i < status; i++) {
    JsonObject node = networks.createNestedObject();
    node["ssid"]    = WiFi.SSID(i);
    node["rssi"]    = WiFi.RSSI(i);
    node["bssid"]   = WiFi.BSSIDstr(i);
    node["channel"] = WiFi.channel(i);
    node["enc"]     = WiFi.encryptionType(i);
  }

  WiFi.scanDelete();

  if (WiFi.scanComplete() == WIFI_SCAN_FAILED) {
    WiFi.scanNetworks(true);
  }
}


void mAnimatorLight::serializeNodes(JsonObject root)
{
  JsonArray nodes = root.createNestedArray("nodes");

  // for (NodesMap::iterator it = Nodes.begin(); it != Nodes.end(); ++it)
  // {
  //   if (it->second.ip[0] != 0)
  //   {
  //     JsonObject node = nodes.createNestedObject();
  //     node[F("name")] = it->second.nodeName;
  //     node["type"]    = it->second.nodeType;
  //     node["ip"]      = it->second.ip.toString();
  //     node[F("age")]  = it->second.age;
  //     node[F("vid")]  = it->second.build;
  //   }
  // }
}


// deserializes mode data string into JsonArray
void mAnimatorLight::serializeModeData(JsonArray fxdata)
{

  
    // #ifdef ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // uint8_t effect_name_len = strlen(lineBuffer);
    // if(effects.data_config[i] != nullptr)
    // {
    //   snprintf_P(lineBuffer+effect_name_len, sizeof(lineBuffer)-effect_name_len, "@%s", effects.data_config[i]);
    // }

    // #endif 



  char lineBuffer[128];
  for (size_t i = 0; i < getModeCount(); i++) {

    
    // if (lineBuffer[0] != 0) {
    //   char* dataPtr = strchr(lineBuffer,'@');
    //   if (dataPtr) fxdata.add(dataPtr+1);
    //   else         fxdata.add("");
    // }



    if(effects.data_config[i] != nullptr)
    {
    // strncpy_P(lineBuffer, effects.data_config[i], 127);
      fxdata.add(effects.data_config[i]);
      // snprintf_P(lineBuffer+effect_name_len, sizeof(lineBuffer)-effect_name_len, "@%s", effects.data_config[i]);
    }else{
      fxdata.add("");
    }



  }
}


// deserializes mode names string into JsonArray
// also removes effect data extensions (@...) from deserialised names
void mAnimatorLight::serializeModeNames(JsonArray arr) {
  char lineBuffer[128];
  for (size_t i = 0; i < getModeCount(); i++) {
    strncpy_P(lineBuffer, getModeData(i), 127);
    if (lineBuffer[0] != 0) {
      char* dataPtr = strchr(lineBuffer,'@');
      if (dataPtr) *dataPtr = 0; // terminate mode data after name
      arr.add(lineBuffer);
    }
  }
}


// deserializes mode names string into JsonArray
// also removes effect data extensions (@...) from deserialised names
void mAnimatorLight::serializeModeNames2(JsonArray arr, bool flag_get_first_name_only) 
{

  // ALOG_INF(PSTR("serializeModeNames2 %d"), getEffectsAmount());

  char lineBuffer[128] = {0};
  for(uint16_t i = 0; i < getEffectsAmount(); i++)
  {
    GetFlasherFunctionNamebyID(i, lineBuffer, sizeof(lineBuffer));

  // ALOG_INF(PSTR("serializeModeNames2 %d"), getEffectsAmount());
    if(flag_get_first_name_only)
    {    
      char* dataPtr = strchr(lineBuffer,'|');
      if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
    }




  // ALOG_INF(PSTR("serializeModeNames2 %d %s"), getEffectsAmount(), lineBuffer);

    arr.add(lineBuffer);
  }
}


#ifdef WLED_ENABLE_JSONLIVE
#define MAX_LIVE_LEDS 180

bool mAnimatorLight::serveLiveLeds(AsyncWebServerRequest* request, uint32_t wsClient)
{
  #ifdef WLED_ENABLE_WEBSOCKETS
  AsyncWebSocketClient * wsc = nullptr;
  if (!request) { //not HTTP, use Websockets
    wsc = ws.client(wsClient);
    if (!wsc || wsc->queueLength() > 0) return false; //only send if queue free
  }
  #endif

  uint16_t used = getLengthTotal();
  uint16_t n = (used -1) /MAX_LIVE_LEDS +1; //only serve every n'th LED if count over MAX_LIVE_LEDS
  char buffer[2000];
  strcpy_P(buffer, PSTR("{\"leds\":["));
  obuf = buffer;
  olen = 9;

  for (size_t i= 0; i < used; i += n)
  {
    uint32_t c = getPixelColor(i);
    uint8_t r = qadd8(W(c), R(c)); //add white channel to RGB channels as a simple RGBW -> RGB map
    uint8_t g = qadd8(W(c), G(c));
    uint8_t b = qadd8(W(c), B(c));
    olen += sprintf(obuf + olen, "\"%06X\",", RGBW32(r,g,b,0));
  }
  olen -= 1;
  oappend((const char*)F("],\"n\":"));
  oappendi(n);
  oappend("}");
  if (request) {
    request->send(200, "application/json", buffer);
  }
  #ifdef WLED_ENABLE_WEBSOCKETS
  else {
    wsc->text(obuf, olen);
  }
  #endif
  return true;
}
#endif



void mAnimatorLight::serveJson(AsyncWebServerRequest* request)
{
  byte subJson = 0;
  const String& url = request->url();
  if      (url.indexOf("state") > 0) subJson = JSON_PATH_STATE;
  else if (url.indexOf("info")  > 0) subJson = JSON_PATH_INFO;
  else if (url.indexOf("si")    > 0) subJson = JSON_PATH_STATE_INFO;
  else if (url.indexOf("nodes") > 0) subJson = JSON_PATH_NODES;
  else if (url.indexOf("eff")   > 0) subJson = JSON_PATH_EFFECTS;
  else if (url.indexOf("palx")  > 0) subJson = JSON_PATH_PALETTES;
  else if (url.indexOf("fxda")  > 0) subJson = JSON_PATH_FXDATA;
  else if (url.indexOf("net")   > 0) subJson = JSON_PATH_NETWORKS;
  #ifdef WLED_ENABLE_JSONLIVE
  else if (url.indexOf("live")  > 0) {
    serveLiveLeds(request);
    return;
  }
  #endif
  else if (url.indexOf("pal") > 0) {

    JBI->Start();
      JBI->Array_Start();

      char lineBuffer[100] = {0};
      bool flag_get_first_name_only = true;
        
        // for(uint16_t i = 0; i < 10; i++)//< mPalette::PALETTELIST_VARIABLE_CRGBPALETTE16__LENGTH__ID; i++)
        for(uint16_t i = 0; i < mPaletteI->GetPaletteListLength(); i++)
        {

          GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
          if(flag_get_first_name_only)
          {    
            char* dataPtr = strchr(lineBuffer,'|');
            if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
          }
          ALOG_INF(PSTR("pal[%d]=\"%s\""), i, lineBuffer);
          JBI->Add(lineBuffer);
        }

      JBI->Array_End();
    JBI->End();




    // remove leading and trailing json parts as temp measure
    char* data = JBI->GetBufferPtr();
    uint16_t data_len = strlen(data);
    if(data) data[data_len-1] = '\0';


    request->send_P(200, "application/json", &data[1]);



    // request->send_P(200, "application/json", JSON_palette_names);
    return;
  }
  else if (url.indexOf("cfg") > 0 && handleFileRead(request, "/cfg.json")) {
    return;
  }
  else if (url.length() > 6) { //not just /json
    request->send(501, "application/json", F("{\"error\":\"Not implemented\"}"));
    return;
  }

  if (!requestJSONBufferLock(17)) {
    request->send(503, "application/json", F("{\"error\":3}"));
    return;
  }
  AsyncJsonResponse *response = new AsyncJsonResponse(&doc, subJson==JSON_PATH_FXDATA || subJson==JSON_PATH_EFFECTS); // will clear and convert JsonDocument into JsonArray if necessary

  JsonVariant lDoc = response->getRoot();

  switch (subJson)
  {
    case JSON_PATH_STATE:
      serializeState(lDoc); break;
    case JSON_PATH_INFO:
      serializeInfo(lDoc); break;
    case JSON_PATH_PALETTES:
      serializePalettes(lDoc, request->hasParam("page") ? request->getParam("page")->value().toInt() : 0); 
      break;
    case JSON_PATH_EFFECTS:
      serializeModeNames2(lDoc); break;
    case JSON_PATH_FXDATA:
      serializeModeData(lDoc); break;
    case JSON_PATH_NETWORKS:
      serializeNetworks(lDoc); break;
    default: // All
      JsonObject state = lDoc.createNestedObject("state");
      serializeState(state);
      JsonObject info = lDoc.createNestedObject("info");
      serializeInfo(info);
      if (subJson != JSON_PATH_STATE_INFO)
      {
        JsonArray effects = lDoc.createNestedArray(F("effects"));
        serializeModeNames2(effects); // remove WLED-SR extensions from effect names

        lDoc[F("palettes")] = serialized((const __FlashStringHelper*)JSON_palette_names);

      //   char lineBuffer[100] = {0};
      //   JsonArray pal = lDoc.createNestedArray(F("palettes"));
      //   // for(size_t i = 0; i < 10; i++)
      //   // {
      //   //   pal.add(GetPaletteNameByID(i, buffer, sizeof(buffer)));
      //   // }

      // bool flag_get_first_name_only = true;
        
      //   for(uint16_t i = 0; i < 3; i++)
      //   {
      //     GetPaletteNameByID(i, lineBuffer, sizeof(lineBuffer));
      //     if(flag_get_first_name_only)
      //     {    
      //       char* dataPtr = strchr(lineBuffer,'|');
      //       if (dataPtr) *dataPtr = 0; // replace name dividor with null termination early
      //     }
      //     pal.add(lineBuffer);
      //   }

      }
      lDoc["m"] = lDoc.memoryUsage(); // JSON buffer usage, for remote debugging
  }

  DEBUG_PRINTF("JSON buffer size: %u for request: %d\n", lDoc.memoryUsage(), subJson);

  size_t len = response->setLength();
  DEBUG_PRINT(F("JSON content length: ")); DEBUG_PRINTLN(len);

  request->send(response);
  releaseJSONBufferLock();
}


void mAnimatorLight::serveSettingsJS(AsyncWebServerRequest* request)
{
  char buf[SETTINGS_STACK_BUF_SIZE+37];
  buf[0] = 0;
  byte subPage = request->arg(F("p")).toInt();
  if (subPage > 10) {
    strcpy_P(buf, PSTR("alert('Settings for this request are not implemented.');"));
    request->send(501, "application/javascript", buf);
    return;
  }
  if (subPage > 0 && !correctPIN && strlen(settingsPIN)>0) {
    strcpy_P(buf, PSTR("alert('PIN incorrect.');")); 
    request->send(403, "application/javascript", buf);
    return;
  }
  strcat_P(buf,PSTR("function GetV(){var d=document;"));
  getSettingsJS(subPage, buf+strlen(buf));  // this may overflow by 35bytes!!!
  strcat_P(buf,PSTR("}"));
  request->send(200, "application/javascript", buf);
}


void mAnimatorLight::serveSettings(AsyncWebServerRequest* request, bool post)
{
  byte subPage = 0, originalSubPage = 0;
  const String& url = request->url();

  if (url.indexOf("sett") >= 0)
  {
    if      (url.indexOf(".js")  > 0) subPage = SUBPAGE_JS;
    else if (url.indexOf(".css") > 0) subPage = SUBPAGE_CSS;
    else if (url.indexOf("wifi") > 0) subPage = SUBPAGE_WIFI;
    else if (url.indexOf("leds") > 0) subPage = SUBPAGE_LEDS;
    else if (url.indexOf("ui")   > 0) subPage = SUBPAGE_UI;
    else if (url.indexOf("sync") > 0) subPage = SUBPAGE_SYNC;
    else if (url.indexOf("time") > 0) subPage = SUBPAGE_TIME;
    else if (url.indexOf("sec")  > 0) subPage = SUBPAGE_SEC;
    else if (url.indexOf("dmx")  > 0) subPage = SUBPAGE_DMX;
    else if (url.indexOf("um")   > 0) subPage = SUBPAGE_UM;
    else if (url.indexOf("2D")   > 0) subPage = SUBPAGE_2D;
    else if (url.indexOf("lock") > 0) subPage = SUBPAGE_LOCK;
  }
  else if (url.indexOf("/update") >= 0) subPage = SUBPAGE_UPDATE; // update page, for PIN check
  else subPage = SUBPAGE_WELCOME;

  if (!correctPIN && strlen(settingsPIN) > 0 && (subPage > 0 && subPage < 11)) {
    originalSubPage = subPage;
    subPage = SUBPAGE_PINREQ; // require PIN
  }

  if (post) { //settings/set POST request, saving
    // if (subPage != SUBPAGE_WIFI || !(wifiLock && otaLock)) handleSettingsSet(request, subPage);

    char s[32];
    char s2[45] = "";

    switch (subPage) {
      case SUBPAGE_WIFI   : strcpy_P(s, PSTR("WiFi")); strcpy_P(s2, PSTR("Please connect to the new IP (if changed)")); break;//forceReconnect = true; break;
      case SUBPAGE_LEDS   : strcpy_P(s, PSTR("LED")); break;
      case SUBPAGE_UI     : strcpy_P(s, PSTR("UI")); break;
      case SUBPAGE_SYNC   : strcpy_P(s, PSTR("Sync")); break;
      case SUBPAGE_TIME   : strcpy_P(s, PSTR("Time")); break;
      case SUBPAGE_SEC    : strcpy_P(s, PSTR("Security")); break;//if (doReboot) strcpy_P(s2, PSTR("Rebooting, please wait ~10 seconds...")); break;
      case SUBPAGE_DMX    : strcpy_P(s, PSTR("DMX")); break;
      case SUBPAGE_UM     : strcpy_P(s, PSTR("Usermods")); break;
      case SUBPAGE_2D     : strcpy_P(s, PSTR("2D")); break;
      case SUBPAGE_PINREQ : strcpy_P(s, correctPIN ? PSTR("PIN accepted") : PSTR("PIN rejected")); break;
    }

    if (subPage != SUBPAGE_PINREQ) strcat_P(s, PSTR(" settings saved."));

    if (subPage == SUBPAGE_PINREQ && correctPIN) {
      subPage = originalSubPage; // on correct PIN load settings page the user intended
    } else {
      if (!s2[0]) strcpy_P(s2, s_redirecting);

      // bool redirectAfter9s = (subPage == SUBPAGE_WIFI || ((subPage == SUBPAGE_SEC || subPage == SUBPAGE_UM) && doReboot));
      // serveMessage(request, 200, s, s2, redirectAfter9s ? 129 : (correctPIN ? 1 : 3));
      return;
    }
  }

  AsyncWebServerResponse *response;
  switch (subPage)
  {
    case SUBPAGE_WIFI    : response = request->beginResponse_P(200, "text/html", PAGE_settings_wifi, PAGE_settings_wifi_length); break;
    case SUBPAGE_LEDS    : response = request->beginResponse_P(200, "text/html", PAGE_settings_leds, PAGE_settings_leds_length); break;
    case SUBPAGE_UI      : response = request->beginResponse_P(200, "text/html", PAGE_settings_ui,   PAGE_settings_ui_length);   break;
    case SUBPAGE_SYNC    : response = request->beginResponse_P(200, "text/html", PAGE_settings_sync, PAGE_settings_sync_length); break;
    case SUBPAGE_TIME    : response = request->beginResponse_P(200, "text/html", PAGE_settings_time, PAGE_settings_time_length); break;
    case SUBPAGE_SEC     : response = request->beginResponse_P(200, "text/html", PAGE_settings_sec,  PAGE_settings_sec_length);  break;
    #ifdef WLED_ENABLE_DMX
    case SUBPAGE_DMX     : response = request->beginResponse_P(200, "text/html", PAGE_settings_dmx,  PAGE_settings_dmx_length);  break;
    #endif
    case SUBPAGE_UM      : response = request->beginResponse_P(200, "text/html", PAGE_settings_um,   PAGE_settings_um_length);   break;
    case SUBPAGE_UPDATE  : response = request->beginResponse_P(200, "text/html", PAGE_update,        PAGE_update_length);        break;
    #ifndef WLED_DISABLE_2D
    case SUBPAGE_2D      : response = request->beginResponse_P(200, "text/html", PAGE_settings_2D,   PAGE_settings_2D_length);   break;
    #endif
    case SUBPAGE_LOCK    : {
      correctPIN = !strlen(settingsPIN); // lock if a pin is set
      createEditHandler(correctPIN);
      serveMessage(request, 200, strlen(settingsPIN) > 0 ? PSTR("Settings locked") : PSTR("No PIN set"), FPSTR(s_redirecting), 1);
      return;
    }
    case SUBPAGE_PINREQ  : response = request->beginResponse_P(200, "text/html", PAGE_settings_pin,  PAGE_settings_pin_length);  break;
    case SUBPAGE_CSS     : response = request->beginResponse_P(200, "text/css",  PAGE_settingsCss,   PAGE_settingsCss_length);   break;
    case SUBPAGE_JS      : serveSettingsJS(request); return;
    case SUBPAGE_WELCOME : response = request->beginResponse_P(200, "text/html", PAGE_welcome,       PAGE_welcome_length);       break;
    default:  response = request->beginResponse_P(200, "text/html", PAGE_settings,      PAGE_settings_length);      break;
  }
  response->addHeader(FPSTR(s_content_enc),"gzip");
  setStaticContentCacheHeaders(response);
  request->send(response);
}

//Is this an IP?
bool mAnimatorLight::isIp(String str) {
  for (size_t i = 0; i < str.length(); i++) {
    int c = str.charAt(i);
    if (c != '.' && (c < '0' || c > '9')) {
      return false;
    }
  }
  return true;
}


//helper to get int value with in/decrementing support via ~ syntax
void mAnimatorLight::parseNumber(const char* str, byte* val, byte minv, byte maxv)
{
  if (str == nullptr || str[0] == '\0') return;
  if (str[0] == 'r') {*val = random8(minv,maxv?maxv:255); return;} // maxv for random cannot be 0
  bool wrap = false;
  if (str[0] == 'w' && strlen(str) > 1) {str++; wrap = true;}
  if (str[0] == '~') {
    int out = atoi(str +1);
    if (out == 0) {
      if (str[1] == '0') return;
      if (str[1] == '-') {
        *val = (int)(*val -1) < (int)minv ? maxv : min((int)maxv,(*val -1)); //-1, wrap around
      } else {
        *val = (int)(*val +1) > (int)maxv ? minv : max((int)minv,(*val +1)); //+1, wrap around
      }
    } else {
      if (wrap && *val == maxv && out > 0) out = minv;
      else if (wrap && *val == minv && out < 0) out = maxv;
      else {
        out += *val;
        if (out > maxv) out = maxv;
        if (out < minv) out = minv;
      }
      *val = out;
    }
    return;
  } else if (minv == maxv && minv == 0) { // limits "unset" i.e. both 0
    byte p1 = atoi(str);
    const char* str2 = strchr(str,'~'); // min/max range (for preset cycle, e.g. "1~5~")
    if (str2) {
      byte p2 = atoi(++str2);           // skip ~
      if (p2 > 0) {
        while (isdigit(*(++str2)));     // skip digits
        parseNumber(str2, val, p1, p2);
        return;
      }
    }
  }
  *val = atoi(str);
}


bool mAnimatorLight::getVal(JsonVariant elem, byte* val, byte vmin, byte vmax) {
  if (elem.is<int>()) {
		if (elem < 0) return false; //ignore e.g. {"ps":-1}
    *val = elem;
    return true;
  } else if (elem.is<const char*>()) {
    const char* str = elem;
    size_t len = strnlen(str, 12);
    if (len == 0 || len > 10) return false;
    parseNumber(str, val, vmin, vmax);
    return true;
  }
  return false; //key does not exist
}


//contrary to the colorFromDecOrHexString() function, this uses the more standard RRGGBB / RRGGBBWW order
bool mAnimatorLight::colorFromHexString(byte* rgb, const char* in) {
  if (in == nullptr) return false;
  size_t inputSize = strnlen(in, 9);
  if (inputSize != 6 && inputSize != 8) return false;

  uint32_t c = strtoul(in, NULL, 16);

  if (inputSize == 6) {
    rgb[0] = (c >> 16);
    rgb[1] = (c >>  8);
    rgb[2] =  c       ;
  } else {
    rgb[0] = (c >> 24);
    rgb[1] = (c >> 16);
    rgb[2] = (c >>  8);
    rgb[3] =  c       ;
  }
  return true;
}


bool mAnimatorLight::deserializeSegment(JsonObject elem, byte it, byte presetId)
{

  ALOG_INF(PSTR("================deserializeSegment"));

  byte id = elem["id"] | it;
  if (id >= getMaxSegments()) return false;

  int stop = elem["stop"] | -1;

  // if using vectors use this code to append segment
  if (id >= getSegmentsNum()) {

    ALOG_HGL(PSTR("DESTROYING SEGMENT, BAD %d %d"), id, getSegmentsNum());

    if (stop <= 0) return false; // ignore empty/inactive segments
    appendSegment(mAnimatorLight::Segment_New(0, getLengthTotal()));
    id = getSegmentsNum()-1; // segments are added at the end of list
  }

  mAnimatorLight::Segment_New& seg = getSegment(id);
  mAnimatorLight::Segment_New prev = seg; //make a backup so we can tell if something changed

  uint16_t start = elem["start"] | seg.pixel_range.start;
  if (stop < 0) {
    uint16_t len = elem["len"];
    stop = (len > 0) ? start + len : seg.pixel_range.stop;
  }
  // 2D segments
  uint16_t startY = elem["startY"] | seg.startY;
  uint16_t stopY = elem["stopY"] | seg.stopY;

  //repeat, multiplies segment until all LEDs are used, or max segments reached
  bool repeat = elem["rpt"] | false;
  if (repeat && stop>0) {
    elem.remove("id");  // remove for recursive call
    elem.remove("rpt"); // remove for recursive call
    elem.remove("n");   // remove for recursive call
    uint16_t len = stop - start;
    for (size_t i=id+1; i<getMaxSegments(); i++) {
      start = start + len;
      if (start >= getLengthTotal()) break;
      //TODO: add support for 2D
      elem["start"] = start;
      elem["stop"]  = start + len;
      elem["rev"]   = !elem["rev"]; // alternate reverse on even/odd segments
      deserializeSegment(elem, i, presetId); // recursive call with new id
    }
    return true;
  }

  if (elem["n"]) {
    // name field exists
    if (seg.name) { //clear old name
      delete[] seg.name;
      seg.name = nullptr;
    }

    const char * name = elem["n"].as<const char*>();
    size_t len = 0;
    if (name != nullptr) len = strlen(name);
    if (len > 0) {
      if (len > WLED_MAX_SEGNAME_LEN) len = WLED_MAX_SEGNAME_LEN;
      seg.name = new char[len+1];
      if (seg.name) strlcpy(seg.name, name, WLED_MAX_SEGNAME_LEN+1);
    } else {
      // but is empty (already deleted above)
      elem.remove("n");
    }
  } else if (start != seg.pixel_range.start || stop != seg.pixel_range.stop) {
    // clearing or setting segment without name field
    if (seg.name) {
      delete[] seg.name;
      seg.name = nullptr;
    }
  }

  uint16_t grp = elem["grp"] | seg.grouping;
  uint16_t spc = elem[F("spc")] | seg.spacing;
  uint16_t of  = seg.offset;
  uint8_t  soundSim = elem["si"] | seg.soundSim;
  uint8_t  map1D2D  = elem["m12"] | seg.map1D2D;

  if ((spc>0 && spc!=seg.spacing) || seg.map1D2D!=map1D2D) seg.fill(BLACK); // clear spacing gaps

  seg.map1D2D  = constrain(map1D2D, 0, 7);
  seg.soundSim = constrain(soundSim, 0, 1);

  uint8_t set = elem[F("set")] | 0;//seg.set;
  // seg.set = constrain(set, 0, 3);

  uint16_t len = 1;
  if (stop > start) len = stop - start;
  int offset = elem[F("of")] | INT32_MAX;
  if (offset != INT32_MAX) {
    int offsetAbs = abs(offset);
    if (offsetAbs > len - 1) offsetAbs %= len;
    if (offset < 0) offsetAbs = len - offsetAbs;
    of = offsetAbs;
  }
  if (stop > start && of > len -1) of = len -1;
  seg.setUp(start, stop, grp, spc, of, startY, stopY);

  if (seg.reset && seg.pixel_range.stop == 0) return true; // segment was deleted & is marked for reset, no need to change anything else

  byte segbri = seg.opacity;
  if (getVal(elem["bri"], &segbri)) {
    if (segbri > 0) seg.setOpacity(segbri);
    seg.setOption(SEG_OPTION_ON, segbri); // use transition
  }

  bool on = elem["on"] | seg.on;
  if (elem["on"].is<const char*>() && elem["on"].as<const char*>()[0] == 't') on = !on;
  seg.setOption(SEG_OPTION_ON, on); // use transition
  bool frz = elem["frz"] | seg.freeze;
  if (elem["frz"].is<const char*>() && elem["frz"].as<const char*>()[0] == 't') frz = !seg.freeze;
  seg.freeze = frz;

  seg.setCCT(elem["cct"] | seg.cct);

  JsonArray colarr = elem["col"];
  if (!colarr.isNull())
  {

    ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "JsonArray colarr"));

    if (seg.getLightCapabilities() & 3) {
      // segment has RGB or White
      for (size_t i = 0; i < 5; i++)
      {
        int rgbw[] = {0,0,0,0};
        bool colValid = false;
        JsonArray colX = colarr[i];
        if (colX.isNull()) {
          byte brgbw[] = {0,0,0,0};
          const char* hexCol = colarr[i];
          if (hexCol == nullptr) { //Kelvin color temperature (or invalid), e.g 2400
            int kelvin = colarr[i] | -1;
            if (kelvin <  0) continue;
            if (kelvin == 0)
            {
              ALOG_INF(PSTR("seg.setColor(i, 0); %d"), i);
              seg.setColor(i, 0);
            }
            if (kelvin >  0) colorKtoRGB(kelvin, brgbw);
            colValid = true;
          } else { //HEX string, e.g. "FFAA00"
            colValid = colorFromHexString(brgbw, hexCol);
          }
          for (size_t c = 0; c < 4; c++) rgbw[c] = brgbw[c];
        } else { //Array of ints (RGB or RGBW color), e.g. [255,160,0]
          byte sz = colX.size();
          if (sz == 0) continue; //do nothing on empty array

          copyArray(colX, rgbw, 4);
          colValid = true;
        }

        if (!colValid) continue;

        ALOG_INF(PSTR("seg.setColor(i, RGBW32(rgbw[0],rgbw[1],rgbw[2],rgbw[3]));"));
    
        seg.setColor(i, RGBW32(rgbw[0],rgbw[1],rgbw[2],rgbw[3]));
        if (seg.animation_mode_id == 0) trigger(); //instant refresh
      }
    } else {
      // non RGB & non White segment (usually On/Off bus)
      seg.setColor(0, ULTRAWHITE);
      seg.setColor(1, BLACK);
    }
  }

  // lx parser
  #ifdef WLED_ENABLE_LOXONE
  int lx = elem[F("lx")] | -1;
  if (lx > 0) {
    parseLxJson(lx, id, false);
  }
  int ly = elem[F("ly")] | -1;
  if (ly > 0) {
    parseLxJson(ly, id, true);
  }
  #endif

  #ifndef WLED_DISABLE_2D
  bool reverse  = seg.reverse;
  bool mirror   = seg.mirror;
  #endif
  seg.selected  = elem["sel"] | seg.selected;
  seg.reverse   = elem["rev"] | seg.reverse;
  seg.mirror    = elem["mi"]  | seg.mirror;
  #ifndef WLED_DISABLE_2D
  bool reverse_y = seg.reverse_y;
  bool mirror_y  = seg.mirror_y;
  seg.reverse_y  = elem["rY"]  | seg.reverse_y;
  seg.mirror_y   = elem["mY"]  | seg.mirror_y;
  seg.transpose  = elem[F("tp")] | seg.transpose;
  if (seg.is2D() && seg.map1D2D == M12_pArc && (reverse != seg.reverse || reverse_y != seg.reverse_y || mirror != seg.mirror || mirror_y != seg.mirror_y)) seg.fill(BLACK); // clear entire segment (in case of Arc 1D to 2D expansion)
  #endif

  byte fx = seg.animation_mode_id;
  if (getVal(elem["fx"], &fx, 0, getModeCount())) { //load effect ('r' random, '~' inc/dec, 0-255 exact value)
    ALOG_INF(PSTR("getVal(elem[\"fx\"], &fx, 0, getModeCount()) %d"), fx);
    // if (!presetId && currentPlaylist>=0) unloadPlaylist();
    // if (fx != seg.animation_mode_id) 
    seg.setMode(fx, elem[F("fxdef")]);
  }

  //getVal also supports inc/decrementing and random
  getVal(elem["sx"], &seg._speed);
  getVal(elem["ix"], &seg._intensity);

  // uint8_t transition_slider_time = 0;
  // getVal(elem["tt"], &transition_slider_time);
  // Map scale into internal rate
  if (elem["tt"].is<int>()) {
    seg.transition.time_ms = elem["tt"];//map(transition_slider_time, 0,255, 0,10000);
    ALOG_INF(PSTR("seg.transition.time_ms = %d"), seg.transition.time_ms);
  }

  // uint8_t transition_slider_rate = 0;
  // getVal(elem["tr"], &transition_slider_rate);
  // Map scale into internal rate
  if (elem["tr"].is<int>()) {
    seg.transition.rate_ms = elem["tr"];//map(transition_slider_rate, 0,255, 0,10000);
    ALOG_INF(PSTR("seg.transition.rate_ms = %d"), seg.transition.rate_ms);
  }

  // getVal(elem["tr"], &seg.transition.rate_ms);

  uint8_t pal = seg.palette.id;
  if (seg.getLightCapabilities() & 1) {  // ignore palette for White and On/Off segments
  
    ALOG_INF(PSTR("getVal(elem[\"pal\"], &pal)"));


    if (getVal(elem["pal"], &pal)) seg.setPalette(pal);
  }

  getVal(elem["c1"], &seg.custom1);
  getVal(elem["c2"], &seg.custom2);
  uint8_t cust3 = seg.custom3;
  getVal(elem["c3"], &cust3); // we can't pass reference to bifield
  seg.custom3 = constrain(cust3, 0, 31);

  seg.check1 = elem["o1"] | seg.check1;
  seg.check2 = elem["o2"] | seg.check2;
  seg.check3 = elem["o3"] | seg.check3;

  JsonArray iarr = elem[F("i")]; //set individual LEDs
  if (!iarr.isNull()) {
    uint8_t oldMap1D2D = seg.map1D2D;
    seg.map1D2D = M12_Pixels; // no mapping

    // set brightness immediately and disable transition
    transitionDelayTemp = 0;
    jsonTransitionOnce = true;
    // setBrightness(scaledBri(
    //   // bri
      
    //   pCONT_iLight->
      
    //   ), true);

    // freeze and init to black
    if (!seg.freeze) {
      seg.freeze = true;
      seg.fill(BLACK);
    }

    uint16_t start = 0, stop = 0;
    byte set = 0; //0 nothing set, 1 start set, 2 range set

    for (size_t i = 0; i < iarr.size(); i++) {
      if(iarr[i].is<JsonInteger>()) {
        if (!set) {
          start = abs(iarr[i].as<int>());
          set++;
        } else {
          stop = abs(iarr[i].as<int>());
          set++;
        }
      } else { //color
        uint8_t rgbw[] = {0,0,0,0};
        JsonArray icol = iarr[i];
        if (!icol.isNull()) { //array, e.g. [255,0,0]
          byte sz = icol.size();
          if (sz > 0 && sz < 5) copyArray(icol, rgbw);
        } else { //hex string, e.g. "FF0000"
          byte brgbw[] = {0,0,0,0};
          const char* hexCol = iarr[i];
          if (colorFromHexString(brgbw, hexCol)) {
            for (size_t c = 0; c < 4; c++) rgbw[c] = brgbw[c];
          }
        }

        if (set < 2 || stop <= start) stop = start + 1;
        uint32_t c = gamma32(RGBW32(rgbw[0], rgbw[1], rgbw[2], rgbw[3]));
        while (start < stop) seg.setPixelColor(start++, c);
        set = 0;
      }
    }
    seg.map1D2D = oldMap1D2D; // restore mapping
    trigger(); // force segment update
  }
  // send UDP/WS if segment options changed (except selection; will also deselect current preset)
  if (seg.differs(prev) & 0x7F) stateChanged = true;

  return true;
}

// deserializes WLED state (fileDoc points to doc object if called from web server)
// presetId is non-0 if called from handlePreset()
bool  mAnimatorLight::deserializeState(JsonObject root, byte callMode, byte presetId)
{
  ALOG_INF(PSTR("================deserializeState"));


  bool stateResponse = root[F("v")] | false;

                                        
// {"seg":{"col":[[21,255,0,128],[],[]]},"v":true,"time":1688540567}


  #if defined(WLED_DEBUG) && defined(WLED_DEBUG_HOST)
  netDebugEnabled = root[F("debug")] | netDebugEnabled;
  #endif

  bool onBefore = pCONT_iLight->_briRGB_Global ; //bri
  getVal(root["bri"], &pCONT_iLight->_briRGB_Global);

  bool on = root["on"] | (pCONT_iLight->_briRGB_Global > 0);
  if (!on != !pCONT_iLight->_briRGB_Global) toggleOnOff();

  if (root["on"].is<const char*>() && root["on"].as<const char*>()[0] == 't') {
    if (onBefore || !pCONT_iLight->_briRGB_Global) toggleOnOff(); // do not toggle off again if just turned on by bri (makes e.g. "{"on":"t","bri":32}" work)
  }

  if (pCONT_iLight->_briRGB_Global && !onBefore) { // unfreeze all segments when turning on
    for (size_t s=0; s < getSegmentsNum(); s++) {
      getSegment(s).freeze = false;
    }
    if (realtimeMode && !realtimeOverride && useMainSegmentOnly) { // keep live segment frozen if live
      getMainSegment().freeze = true;
    }
  }

  int tr = -1;
  if (!presetId || currentPlaylist < 0) { //do not apply transition time from preset if playlist active, as it would override playlist transition times
    tr = root[F("transition")] | -1;
    if (tr >= 0)
    {
      transitionDelay = tr;
      transitionDelay *= 100;
      transitionDelayTemp = transitionDelay;
    }
  }

  // temporary transition (applies only once)
  tr = root[F("tt")] | -1;
  if (tr >= 0)
  {
    transitionDelayTemp = tr;
    transitionDelayTemp *= 100;
    jsonTransitionOnce = true;
  }
  setTransition(transitionDelayTemp); // required here for color transitions to have correct duration

  tr = root[F("tb")] | -1;
  if (tr >= 0) timebase = ((uint32_t)tr) - millis();

  JsonObject nl       = root["nl"];
  nightlightActive    = nl["on"]      | nightlightActive;
  nightlightDelayMins = nl["dur"]     | nightlightDelayMins;
  nightlightMode      = nl["mode"]    | nightlightMode;
  nightlightTargetBri = nl[F("tbri")] | nightlightTargetBri;

  JsonObject udpn      = root["udpn"];
  notifyDirect         = udpn["send"] | notifyDirect;
  receiveNotifications = udpn["recv"] | receiveNotifications;
  if ((bool)udpn[F("nn")]) callMode = CALL_MODE_NO_NOTIFY; //send no notification just for this request

  unsigned long timein = root["time"] | UINT32_MAX; //backup time source if NTP not synced
  if (timein != UINT32_MAX) {
    // setTimeFromAPI(timein);
    // if (presetsModifiedTime == 0) presetsModifiedTime = timein;
  }

  if (root[F("psave")].isNull()) doReboot = root[F("rb")] | doReboot;

  // do not allow changing main segment while in realtime mode (may get odd results else)
  if (!realtimeMode) setMainSegmentId(root[F("mainseg")] | getMainSegmentId()); // must be before realtimeLock() if "live"

  realtimeOverride = root[F("lor")] | realtimeOverride;
  if (realtimeOverride > 2) realtimeOverride = REALTIME_OVERRIDE_ALWAYS;
  if (realtimeMode && useMainSegmentOnly) {
    getMainSegment().freeze = !realtimeOverride;
  }

  if (root.containsKey("live")) {
    if (root["live"].as<bool>()) {
      transitionDelayTemp = 0;
      jsonTransitionOnce = true;
      realtimeLock(65000);
    } else {
      exitRealtime();
    }
  }

  int it = 0;
  JsonVariant segVar = root["seg"];
  if (segVar.is<JsonObject>())
  {

    DEBUG_LINE_HERE;

    int id = segVar["id"] | -1;
    //if "seg" is not an array and ID not specified, apply to all selected/checked segments
    if (id < 0) {
      //apply all selected segments
      //bool didSet = false;
      for (size_t s = 0; s < getSegmentsNum(); s++) {
    DEBUG_LINE_HERE;
        mAnimatorLight::Segment_New &sg = getSegment(s);
        if (sg.isSelected()) {
    DEBUG_LINE_HERE;
          deserializeSegment(segVar, s, presetId);
          //didSet = true;
        }
      }
      //TODO: not sure if it is good idea to change first active but unselected segment
      //if (!didSet) deserializeSegment(segVar, getMainSegmentId(), presetId);
    } else {
    DEBUG_LINE_HERE;
      deserializeSegment(segVar, id, presetId); //apply only the segment with the specified ID
    }
  } else {
    size_t deleted = 0;
    JsonArray segs = segVar.as<JsonArray>();
    for (JsonObject elem : segs) {
      if (deserializeSegment(elem, it++, presetId) && !elem["stop"].isNull() && elem["stop"]==0) deleted++;
    }
    if (getSegmentsNum() > 3 && deleted >= getSegmentsNum()/2U) purgeSegments(); // batch deleting more than half segments
  }

  // usermods.readFromJsonState(root);

  // loadLedmap = root[F("ledmap")] | loadLedmap;

  // byte ps = root[F("psave")];
  // if (ps > 0 && ps < 251) savePreset(ps, nullptr, root);

  // ps = root[F("pdel")]; //deletion
  // if (ps > 0 && ps < 251) deletePreset(ps);

  // // HTTP API commands (must be handled before "ps")
  // const char* httpwin = root["win"];
  // if (httpwin) {
  //   String apireq = "win"; apireq += '&'; // reduce flash string usage
  //   apireq += httpwin;
  //   handleSet(nullptr, apireq, false);    // may set stateChanged
  // }

  // applying preset (2 cases: a) API call includes all preset values ("pd"), b) API only specifies preset ID ("ps"))
  byte presetToRestore = 0;
  // a) already applied preset content (requires "seg" or "win" but will ignore the rest)
  if (!root["pd"].isNull() && stateChanged) {
    currentPreset = root[F("pd")] | currentPreset;
    if (root["win"].isNull()) presetCycCurr = currentPreset; // otherwise it was set in handleSet() [set.cpp]
    presetToRestore = currentPreset; // stateUpdated() will clear the preset, so we need to restore it after
    //unloadPlaylist(); // applying a preset unloads the playlist, may be needed here too?
  } else if (!root["ps"].isNull()) {
    // ps = presetCycCurr;
    // if (root["win"].isNull() && getVal(root["ps"], &ps, 0, 0) && ps > 0 && ps < 251 && ps != currentPreset) {
    //   // b) preset ID only or preset that does not change state (use embedded cycling limits if they exist in getVal())
    //   presetCycCurr = ps;
    //   // unloadPlaylist();          // applying a preset unloads the playlist
    //   // applyPreset(ps, callMode); // async load from file system (only preset ID was specified)
    //   return stateResponse;
    // }
  }

  // JsonObject playlist = root[F("playlist")];
  // if (!playlist.isNull() && loadPlaylist(playlist, presetId)) {
  //   //do not notify here, because the first playlist entry will do
  //   if (root["on"].isNull()) callMode = CALL_MODE_NO_NOTIFY;
  //   else callMode = CALL_MODE_DIRECT_CHANGE;  // possible bugfix for playlist only containing HTTP API preset FX=~
  // }

  if (root.containsKey(F("rmcpal")) && root[F("rmcpal")].as<bool>()) {
    if (customPalettes.size()) {
      char fileName[32];
      sprintf_P(fileName, PSTR("/palette%d.json"), customPalettes.size()-1);
      if (WLED_FS.exists(fileName)) WLED_FS.remove(fileName);
      loadCustomPalettes();
    }
  }

  // stateUpdated(callMode);
  if (presetToRestore) currentPreset = presetToRestore;

  return stateResponse;
}


bool  mAnimatorLight::captivePortal(AsyncWebServerRequest *request)
{
  if (ON_STA_FILTER(request)) return false; //only serve captive in AP mode
  String hostH;
  if (!request->hasHeader("Host")) return false;
  hostH = request->getHeader("Host")->value();

  if (!isIp(hostH) && hostH.indexOf("wled.me") < 0 && hostH.indexOf(pCONT_set->Settings.system_name.device) < 0) {
    DEBUG_PRINTLN("Captive portal");
    AsyncWebServerResponse *response = request->beginResponse(302);
    response->addHeader(F("Location"), F("http://4.3.2.1"));
    request->send(response);
    return true;
  }
  return false;
}


void  mAnimatorLight::notFound(AsyncWebServerRequest *request) 
{
    request->send(404, "text/plain", "Not found");
}


void mAnimatorLight::initServer()
{
  //CORS compatiblity
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Origin"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Methods"), "*");
  DefaultHeaders::Instance().addHeader(F("Access-Control-Allow-Headers"), "*");

  releaseJSONBufferLock();

  #ifdef WLED_ENABLE_WEBSOCKETS
  #ifndef WLED_DISABLE_2D
  pCONT_web->server->on("/liveview2D", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_liveviewws2D, PAGE_liveviewws2D_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
  #endif
  #endif
  
  pCONT_web->server->on("/liveview", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (this->handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_liveview, PAGE_liveview_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    this->setStaticContentCacheHeaders(response);
    request->send(response);
  });

  //settings page
  pCONT_web->server->on("/settings", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveSettings(request);
  });

  // "/settings/settings.js&p=x" request also handled by serveSettings()

  pCONT_web->server->on("/style.css", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", PAGE_settingsCss, PAGE_settingsCss_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    this->setStaticContentCacheHeaders(response);
    request->send(response);
  });

  pCONT_web->server->on("/favicon.ico", HTTP_GET, [this](AsyncWebServerRequest *request){
    if(!handleFileRead(request, "/favicon.ico"))
    {
      request->send_P(200, "image/x-icon", favicon, 156);
    }
  });

  pCONT_web->server->on("/welcome", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveSettings(request);
  });

  pCONT_web->server->on("/reset", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveMessage(request, 200,F("Rebooting now..."),F("Please wait ~10 seconds..."),129);
    // doReboot = true;
  });

  pCONT_web->server->on("/settings", HTTP_POST, [this](AsyncWebServerRequest *request){
    this->serveSettings(request, true);
  });

  pCONT_web->server->on("/json", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveJson(request);
  });

  AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("/json", [this](AsyncWebServerRequest *request) {
    bool verboseResponse = false;
    bool isConfig = false;

    ALOG_INF(PSTR("AsyncCallbackJsonWebHandler"));

    Serial.println((char*)request->_tempObject);

    if (!this->requestJSONBufferLock(14)) return;

    DeserializationError error = deserializeJson(doc, (uint8_t*)(request->_tempObject));
    JsonObject root = doc.as<JsonObject>();
    if (error || root.isNull()) {
      this->releaseJSONBufferLock();
      request->send(400, "application/json", F("{\"error\":9}")); // ERR_JSON
      return;
    }
    // if (root.containsKey("pin")) checkSettingsPIN(root["pin"].as<const char*>());

    const String& url = request->url();
    isConfig = url.indexOf("cfg") > -1; 
    if (!isConfig) {
      /*
      #ifdef WLED_DEBUG
        DEBUG_PRINTLN(F("Serialized HTTP"));
        serializeJson(root,Serial);
        DEBUG_PRINTLN();
      #endif
      */
      ALOG_INF(PSTR("deserializeState"));
      verboseResponse = this->deserializeState(root);
    } else {
      if (!correctPIN && strlen(settingsPIN)>0) {
        request->send(403, "application/json", F("{\"error\":1}")); // ERR_DENIED
        this->releaseJSONBufferLock();
        return;
      }
    ALOG_INF(PSTR("deserializeConfig"));
      verboseResponse = this->deserializeConfig(root); //use verboseResponse to determine whether cfg change should be saved immediately
    }
    this->releaseJSONBufferLock();

    if (verboseResponse) {
      if (!isConfig) {
        this->serveJson(request); return; //if JSON contains "v"
      } else {
        // doSerializeConfig = true; //serializeConfig(); //Save new settings to FS
      }
    }
    request->send(200, "application/json", F("{\"success\":true}"));
  }, JSON_BUFFER_SIZE);
  pCONT_web->server->addHandler(handler);

  pCONT_web->server->on("/version", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)PROJECT_VERSION);
  });

  pCONT_web->server->on("/uptime", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)millis());
  });

  pCONT_web->server->on("/freeheap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", (String)ESP.getFreeHeap());
  });

#ifdef WLED_ENABLE_USERMOD_PAGE
  pCONT_web->server->on("/u", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_usermod, PAGE_usermod_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
#endif

  pCONT_web->server->on("/teapot", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveMessage(request, 418, F("418. I'm a teapot."), F("(Tangible Embedded Advanced Project Of Twinkling)"), 254);
  });

  pCONT_web->server->on("/upload", HTTP_POST, [this](AsyncWebServerRequest *request) {},
        [this](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data,
                      size_t len, bool final) {this->handleUpload(request, filename, index, data, len, final);}
  );

#ifdef WLED_ENABLE_SIMPLE_UI
  pCONT_web->server->on("/simple.htm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleFileRead(request, "/simple.htm")) return;
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_simple, PAGE_simple_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
#endif

  pCONT_web->server->on("/iro.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", iroJs, iroJs_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    this->setStaticContentCacheHeaders(response);
    request->send(response);
  });

  pCONT_web->server->on("/rangetouch.js", HTTP_GET, [this](AsyncWebServerRequest *request){
    AsyncWebServerResponse *response = request->beginResponse_P(200, "application/javascript", rangetouchJs, rangetouchJs_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    this->setStaticContentCacheHeaders(response);
    request->send(response);
  });

  // createEditHandler(correctPIN);

#ifndef WLED_DISABLE_OTA
  //init ota page
  pCONT_web->server->on("/update", HTTP_GET, [this](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_GET \"/update\""));
    // if (otaLock) {
    //   serveMessage(request, 500, "Access Denied", FPSTR(s_unlock_ota), 254);
    // } else
      this->serveSettings(request); // checks for "upd" in URL and handles PIN
  });

  pCONT_web->server->on("/update", HTTP_POST, [this](AsyncWebServerRequest *request){
    ALOG_INF(PSTR("URL HTTP_POST \"/update\""));
    // if (!correctPIN) {
    //   serveSettings(request, true); // handle PIN page POST request
    //   return;
    // }
    if (Update.hasError() || otaLock) {
      this->serveMessage(request, 500, F("Update failed!"), F("Please check your file and retry!"), 254);
    } else {
      this->serveMessage(request, 200, F("Update successful!"), F("Rebooting..."), 131);
      // doReboot = true;
    }
  },[this](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final){
    if (!this->correctPIN || this->otaLock) return;
    if(!index){
      DEBUG_PRINTLN(F("OTA Update Start"));
      // WLED::instance().disableWatchdog();
      // usermods.onUpdateBegin(true); // notify usermods that update is about to begin (some may require task de-init)
      // lastEditTime = millis(); // make sure PIN does not lock during update
      #ifdef ESP8266
      Update.runAsync(true);
      #endif
      Update.begin((ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000);
    }
    if(!Update.hasError()) Update.write(data, len);
    if(final){
      if(Update.end(true)){
        DEBUG_PRINTLN(F("Update Success"));
      } else {
        DEBUG_PRINTLN(F("Update Failed"));
        // usermods.onUpdateBegin(false); // notify usermods that update has failed (some may require task init)
        // WLED::instance().enableWatchdog();
      }
    }
  });
#else
  pCONT_web->server->on("/update", HTTP_GET, [](AsyncWebServerRequest *request){
    serveMessage(request, 501, "Not implemented", F("OTA updating is disabled in this build."), 254);
  });
#endif


  #ifdef WLED_ENABLE_DMX
  pCONT_web->server->on("/dmxmap", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", PAGE_dmxmap     , dmxProcessor);
  });
  #else
  pCONT_web->server->on("/dmxmap", HTTP_GET, [this](AsyncWebServerRequest *request){
    this->serveMessage(request, 501, "Not implemented", F("DMX support is not enabled in this build."), 254);
  });
  #endif

  pCONT_web->server->on("/", HTTP_GET, [this](AsyncWebServerRequest *request){
    // if (captivePortal(request)) return;
    bool showWelcomePage = false;
    if (!showWelcomePage || request->hasArg(F("sliders"))){
      this->serveIndex(request);
    } else {
      this->serveSettings(request);
    }
  });

  #ifdef WLED_ENABLE_PIXART
  pCONT_web->server->on("/pixart.htm", HTTP_GET, [](AsyncWebServerRequest *request){
    if (handleFileRead(request, "/pixart.htm")) return;
    if (handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_pixart, PAGE_pixart_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    setStaticContentCacheHeaders(response);
    request->send(response);
  });
  #endif

  #ifndef WLED_DISABLE_PXMAGIC
  pCONT_web->server->on("/pxmagic.htm", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (this->handleFileRead(request, "/pxmagic.htm")) return;
    if (this->handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_pxmagic, PAGE_pxmagic_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    this->setStaticContentCacheHeaders(response);
    request->send(response);
  });
  #endif

  pCONT_web->server->on("/cpal.htm", HTTP_GET, [this](AsyncWebServerRequest *request){
    if (this->handleFileRead(request, "/cpal.htm")) return;
    if (this->handleIfNoneMatchCacheHeader(request)) return;
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/html", PAGE_cpal, PAGE_cpal_L);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    this->setStaticContentCacheHeaders(response);
    request->send(response);
  });

  #ifdef WLED_ENABLE_WEBSOCKETS
  pCONT_web->server->addHandler(&ws);
  #endif


  // pCONT->Tasker_Interface(FUNC_WEB_ADD_HANDLER);/
  pCONT->Tasker_Interface(FUNC_WEB_ADD_HANDLER);

  //called when the url is not defined here, ajax-in; get-settings
  pCONT_web->server->onNotFound([this](AsyncWebServerRequest *request){
    DEBUG_PRINTLN("Not-Found HTTP call:");
    DEBUG_PRINTLN("URI: " + request->url());
    if (this->captivePortal(request)) return;

    //make API CORS compatible
    if (request->method() == HTTP_OPTIONS)
    {
      AsyncWebServerResponse *response = request->beginResponse(200);
      response->addHeader(F("Access-Control-Max-Age"), F("7200"));
      request->send(response);
      return;
    }

    // if(handleSet(request, request->url())) return;
    // #ifndef WLED_DISABLE_ALEXA
    // if(espalexa.handleAlexaApiCall(request)) return;
    // #endif
    // if(handleFileRead(request, request->url())) return;
    AsyncWebServerResponse *response = request->beginResponse_P(404, "text/html", PAGE_404, PAGE_404_length);
    response->addHeader(FPSTR(s_content_enc),"gzip");
    this->setStaticContentCacheHeaders(response);
    request->send(response);
  });
}

#endif // ENABLE_DEVFEATURE_LIGHT__WEBUI_STYLE01
