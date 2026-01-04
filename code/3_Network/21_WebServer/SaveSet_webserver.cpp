#include "mWebServer.h"

/**
 * @brief 
 * 
 * 
 * 
 * The commands that can be placed into a URL, such as IP?key=value, are called query parameters. They are used to pass additional information to the server as part of the URL. Here's an explanation of their structure and how they work with multiple parameters:
Structure of Query Parameters

A URL with query parameters generally looks like this:

http://<IP-Address>/<path>?<key1>=<value1>&<key2>=<value2>

    ? marks the beginning of the query string.
    key=value pairs are query parameters, where:
        key is the name of the parameter.
        value is the value assigned to that parameter.
    & is used to separate multiple key-value pairs.

How Query Parameters Work

    Single Parameter:
        Example: http://192.168.1.10/?mode=1
            The server at 192.168.1.10 receives the parameter mode with the value 1.
        The server processes this parameter to decide what action to perform, depending on its implementation.

    Multiple Parameters:
        Example: http://192.168.1.10/?mode=1&speed=100&intensity=255
            The server at 192.168.1.10 receives the parameters:
                mode=1
                speed=100
                intensity=255
        Each parameter is processed individually by the server, which uses them to perform the requested action (e.g., setting the mode, adjusting speed, etc.).

Rules for Query Parameters

    Order Doesn't Matter:
        The order of the parameters doesn't usually affect how they are processed.
        Example: ?mode=1&speed=100 is equivalent to ?speed=100&mode=1.

    URL Encoding:
        Certain characters (like spaces, &, =) need to be encoded to ensure the URL remains valid.
        For example:
            A space is encoded as %20 or +.
            A parameter like key=value with space becomes key=value%20with%20space.

    Default Values:
        If a parameter is omitted, the server may use a default value (depending on the implementation).
        Example: http://192.168.1.10/?mode=1 might use a default speed if it's not provided.

Examples

    Control a device:

http://192.168.1.10/?power=on&brightness=80

    Turns on the device and sets the brightness to 80.

Send data to a server:

    http://192.168.1.10/?sensor=temperature&value=25

        Sends the temperature data (25) from a sensor to the server.

How the Server Handles Query Parameters

    Server-Side Parsing:
        The server parses the query string and extracts the key-value pairs.
        In a typical web server, languages like Python, JavaScript (Node.js), PHP, or frameworks handle query parameters automatically.

    Action Based on Parameters:
        The server uses the extracted values to determine the response or perform certain actions.

Summary

    Query parameters are key-value pairs added to the URL after a ?.
    Multiple parameters are separated by &.
    They are commonly used in web applications, IoT devices, and APIs to send and receive data.
    For example: http://IP/?key1=value1&key2=value2 sends key1 and key2 with their respective values to the server.
 * 
 */
#ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS
/*
 * Receives client input
 */

//called upon POST settings form submit
void mWebServer::SettingsPages__ParseForm(AsyncWebServerRequest *request, byte subPage)
{

  if (subPage == SUBPAGE_PINREQ)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_PINREQ
    checkSettingsPIN(request->arg(F("PIN")).c_str());
    return;
    #endif
  }

  //0: menu 1: wifi 2: leds 3: ui 4: sync 5: time 6: sec 7: DMX 8: usermods 9: N/A 10: 2D
  // if (subPage < 1 || subPage > 10 || !correctPIN) return;

  //WIFI SETTINGS
  if (subPage == SUBPAGE_WIFI)
  {
  #define WLED_MAX_WIFI_COUNT 3
    
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_WIFI
  // -----------------------------------------------------------------------------
// WIFI SETTINGS (DUMMY PARSE) : Log incoming POST args only, do not apply/save
// Date Modified: 02Jan26
// -----------------------------------------------------------------------------
{
  ALOG_INF(PSTR("---- WIFI SETTINGS (dummy parse) ----"));

  // Helper for presence/checkbox-type fields (WLED checkboxes are typically presence-based)
  ALOG_INF(PSTR("hasArg(FG) Force 802.11g (ESP8266)    = %d"), request->hasArg(F("FG")) ? 1 : 0);
  ALOG_INF(PSTR("hasArg(WS) Disable WiFi sleep         = %d"), request->hasArg(F("WS")) ? 1 : 0);
  ALOG_INF(PSTR("hasArg(AH) Hide AP name               = %d"), request->hasArg(F("AH")) ? 1 : 0);

  // DNS
  if (request->hasArg(F("D0")) && request->hasArg(F("D1")) && request->hasArg(F("D2")) && request->hasArg(F("D3"))) {
    ALOG_INF(PSTR("DNS | D0.D1.D2.D3 = %d.%d.%d.%d"),
      request->arg(F("D0")).toInt(),
      request->arg(F("D1")).toInt(),
      request->arg(F("D2")).toInt(),
      request->arg(F("D3")).toInt()
    );
  } else {
    ALOG_INF(PSTR("DNS | D0..D3 not fully present"));
  }

  // mDNS
  if (request->hasArg(F("CM"))) {
    ALOG_INF(PSTR("cmDNS | CM = %s"), request->arg(F("CM")).c_str());
  } else {
    ALOG_INF(PSTR("cmDNS | CM not present"));
  }

  // AP behaviour + settings
  if (request->hasArg(F("AB"))) {
    ALOG_INF(PSTR("AP Behaviour | AB = %d"), request->arg(F("AB")).toInt());
  } else {
    ALOG_INF(PSTR("AP Behaviour | AB not present"));
  }

  if (request->hasArg(F("AS"))) {
    ALOG_INF(PSTR("AP SSID | AS = %s"), request->arg(F("AS")).c_str());
  } else {
    ALOG_INF(PSTR("AP SSID | AS not present"));
  }

  if (request->hasArg(F("AP"))) {
    // IMPORTANT: do not print passwords in production; for dummy wiring test only.
    // If you want safe logging, log length or asterisks instead.
    ALOG_INF(PSTR("AP Pass | AP = %s"), request->arg(F("AP")).c_str());
  } else {
    ALOG_INF(PSTR("AP Pass | AP not present"));
  }

  if (request->hasArg(F("AC"))) {
    ALOG_INF(PSTR("AP Channel | AC = %d"), request->arg(F("AC")).toInt());
  } else {
    ALOG_INF(PSTR("AP Channel | AC not present"));
  }

  // TX power (ESP32)
  if (request->hasArg(F("TX"))) {
    ALOG_INF(PSTR("TX Power | TX = %d"), request->arg(F("TX")).toInt());
  } else {
    ALOG_INF(PSTR("TX Power | TX not present"));
  }

  // -----------------------------------------------------------------------------
  // Single-network (SSID/pass + static IPv4) logging only.
  // Your page still uses CS0/PW0 and IP0{0..3}, GW0{0..3}, SN0{0..3}.
  // -----------------------------------------------------------------------------
  if (request->hasArg(F("CS0"))) ALOG_INF(PSTR("Client SSID | CS0 = %s"), request->arg(F("CS0")).c_str());
  else                          ALOG_INF(PSTR("Client SSID | CS0 not present"));

  if (request->hasArg(F("PW0"))) {
    // IMPORTANT: do not print passwords in production; for dummy wiring test only.
    ALOG_INF(PSTR("Client Pass | PW0 = %s"), request->arg(F("PW0")).c_str());
  } else {
    ALOG_INF(PSTR("Client Pass | PW0 not present"));
  }

  // Static IP fields (little-endian in your JS builder, but POST fields are explicit octets)
  auto logIPv4 = [&](const __FlashStringHelper* label, const char* baseName) {
    char k0[8]; char k1[8]; char k2[8]; char k3[8];
    // baseName is like "IP00" "GW00" "SN00" but we construct "IP00".."IP03"
    // Here we expect baseName with last char being the octet index placeholder.
    // We will overwrite last char with '0'..'3'.
    strlcpy(k0, baseName, sizeof(k0));
    strlcpy(k1, baseName, sizeof(k1));
    strlcpy(k2, baseName, sizeof(k2));
    strlcpy(k3, baseName, sizeof(k3));
    k0[strlen(k0)-1] = '0';
    k1[strlen(k1)-1] = '1';
    k2[strlen(k2)-1] = '2';
    k3[strlen(k3)-1] = '3';

    if (request->hasArg(k0) && request->hasArg(k1) && request->hasArg(k2) && request->hasArg(k3)) {
      ALOG_INF(PSTR("%S | %s.%s.%s.%s = %d.%d.%d.%d"),
        label,
        k0, k1, k2, k3,
        request->arg(k0).toInt(),
        request->arg(k1).toInt(),
        request->arg(k2).toInt(),
        request->arg(k3).toInt()
      );
    } else {
      ALOG_INF(PSTR("%S | missing one or more octets (%s*)"), label, baseName);
    }
  };

  // Expecting IP00..IP03, GW00..GW03, SN00..SN03 from your HTML
  logIPv4(F("Static IP"),      "IP00");
  logIPv4(F("Static Gateway"), "GW00");
  logIPv4(F("Static Subnet"),  "SN00");

  // Optional: dump all args (useful once, then delete)
  ALOG_INF(PSTR("Args count = %u"), (unsigned)request->args());
  for (size_t i = 0; i < request->args(); i++) {
    ALOG_INF(PSTR("ARG[%u] %s = %s"),
      (unsigned)i,
      request->argName(i).c_str(),
      request->arg(i).c_str()
    );
  }

  ALOG_INF(PSTR("---- WIFI SETTINGS (dummy parse) end ----"));
}


    // unsigned cnt = 0;
    // for (size_t n = 0; n < WLED_MAX_WIFI_COUNT; n++) {
    //   char cs[4] = "CS"; cs[2] = 48+n; cs[3] = 0; //client SSID
    //   char pw[4] = "PW"; pw[2] = 48+n; pw[3] = 0; //client password
    //   char bs[4] = "BS"; bs[2] = 48+n; bs[3] = 0; //BSSID
    //   char ip[5] = "IP"; ip[2] = 48+n; ip[4] = 0; //IP address
    //   char gw[5] = "GW"; gw[2] = 48+n; gw[4] = 0; //GW address
    //   char sn[5] = "SN"; sn[2] = 48+n; sn[4] = 0; //subnet mask
    //   if (request->hasArg(cs)) {
    //     if (n >= multiWiFi.size()) multiWiFi.emplace_back(); // expand vector by one
    //     char oldSSID[33]; strcpy(oldSSID, multiWiFi[n].clientSSID);
    //     char oldPass[65]; strcpy(oldPass, multiWiFi[n].clientPass);

    //     strlcpy(multiWiFi[n].clientSSID, request->arg(cs).c_str(), 33);
    //     if (strlen(oldSSID) == 0 || !strncmp(multiWiFi[n].clientSSID, oldSSID, 32)) {
    //       forceReconnect = true;
    //     }
    //     if (!isAsterisksOnly(request->arg(pw).c_str(), 65)) {
    //       strlcpy(multiWiFi[n].clientPass, request->arg(pw).c_str(), 65);
    //       forceReconnect = true;
    //     }
    //     fillStr2MAC(multiWiFi[n].bssid, request->arg(bs).c_str());
    //     for (size_t i = 0; i < 4; i++) {
    //       ip[3] = 48+i;
    //       gw[3] = 48+i;
    //       sn[3] = 48+i;
    //       multiWiFi[n].staticIP[i] = request->arg(ip).toInt();
    //       multiWiFi[n].staticGW[i] = request->arg(gw).toInt();
    //       multiWiFi[n].staticSN[i] = request->arg(sn).toInt();
    //     }
    //     cnt++;
    //   }
    // }
    // // remove unused
    // if (cnt < multiWiFi.size()) {
    //   cnt = multiWiFi.size() - cnt;
    //   while (cnt--) multiWiFi.pop_back();
    //   multiWiFi.shrink_to_fit(); // release memory
    // }

    // if (request->hasArg(F("D0"))) {
    //   dnsAddress = IPAddress(request->arg(F("D0")).toInt(),request->arg(F("D1")).toInt(),request->arg(F("D2")).toInt(),request->arg(F("D3")).toInt());
    // }

    // strlcpy(cmDNS, request->arg(F("CM")).c_str(), 33);

    // apBehavior = request->arg(F("AB")).toInt();
    // char oldSSID[33]; strcpy(oldSSID, apSSID);
    // strlcpy(apSSID, request->arg(F("AS")).c_str(), 33);
    // if (!strcmp(oldSSID, apSSID) && apActive) forceReconnect = true;
    // apHide = request->hasArg(F("AH"));
    // int passlen = request->arg(F("AP")).length();
    // if (passlen == 0 || (passlen > 7 && !isAsterisksOnly(request->arg(F("AP")).c_str(), 65))) {
    //   strlcpy(apPass, request->arg(F("AP")).c_str(), 65);
    //   forceReconnect = true;
    // }
    // int t = request->arg(F("AC")).toInt();
    // if (t != apChannel) forceReconnect = true;
    // if (t > 0 && t < 14) apChannel = t;

    // #ifdef ARDUINO_ARCH_ESP32
    // int tx = request->arg(F("TX")).toInt();
    // txPower = min(max(tx, (int)WIFI_POWER_2dBm), (int)WIFI_POWER_19_5dBm);
    // #endif

    // force802_3g = request->hasArg(F("FG"));
    // noWifiSleep = request->hasArg(F("WS"));

    #if defined(ARDUINO_ARCH_ESP32) && defined(WLED_USE_ETHERNET)
    ethernetType = request->arg(F("ETH")).toInt();
    initEthernet();
    #endif
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_WIFI
  }

  //LED SETTINGS
  if (subPage == SUBPAGE_LEDS)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_LEDS
    int t = 0;

    if (rlyPin>=0 && PinManager::isPinAllocated(rlyPin, PinOwner::Relay)) {
       PinManager::deallocatePin(rlyPin, PinOwner::Relay);
    }
    #ifndef WLED_DISABLE_INFRARED
    if (irPin>=0 && PinManager::isPinAllocated(irPin, PinOwner::IR)) {
      deInitIR();
      PinManager::deallocatePin(irPin, PinOwner::IR);
    }
    #endif
    for (unsigned s=0; s<WLED_MAX_BUTTONS; s++) {
      if (btnPin[s]>=0 && PinManager::isPinAllocated(btnPin[s], PinOwner::Button)) {
        PinManager::deallocatePin(btnPin[s], PinOwner::Button);
        #ifdef SOC_TOUCH_VERSION_2 // ESP32 S2 and S3 have a function to check touch state, detach interrupt
        if (digitalPinToTouchChannel(btnPin[s]) >= 0) // if touch capable pin
          touchDetachInterrupt(btnPin[s]);            // if not assigned previously, this will do nothing
        #endif
      }
    }

    unsigned colorOrder, type, skip, awmode, channelSwap, maPerLed;
    unsigned length, start, maMax;
    uint8_t pins[5] = {255, 255, 255, 255, 255};

    unsigned ablMilliampsMax = request->arg(F("MA")).toInt();
    BusManager::setMilliampsMax(ablMilliampsMax);

    autoSegments = request->hasArg(F("MS"));
    correctWB = request->hasArg(F("CCT"));
    cctFromRgb = request->hasArg(F("CR"));
    cctICused = request->hasArg(F("IC"));
    cctBlending = request->arg(F("CB")).toInt();
    Bus::setCCTBlend(cctBlending);
    Bus::setGlobalAWMode(request->arg(F("AW")).toInt());
    setTargetFps(request->arg(F("FR")).toInt());
    useGlobalLedBuffer = request->hasArg(F("LD"));

    bool busesChanged = false;
    for (int s = 0; s < WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES; s++) {
      int offset = s < 10 ? 48 : 55;
      char lp[4] = "L0"; lp[2] = offset+s; lp[3] = 0; //ascii 0-9 //strip data pin
      char lc[4] = "LC"; lc[2] = offset+s; lc[3] = 0; //strip length
      char co[4] = "CO"; co[2] = offset+s; co[3] = 0; //strip color order
      char lt[4] = "LT"; lt[2] = offset+s; lt[3] = 0; //strip type
      char ls[4] = "LS"; ls[2] = offset+s; ls[3] = 0; //strip start LED
      char cv[4] = "CV"; cv[2] = offset+s; cv[3] = 0; //strip reverse
      char sl[4] = "SL"; sl[2] = offset+s; sl[3] = 0; //skip first N LEDs
      char rf[4] = "RF"; rf[2] = offset+s; rf[3] = 0; //refresh required
      char aw[4] = "AW"; aw[2] = offset+s; aw[3] = 0; //auto white mode
      char wo[4] = "WO"; wo[2] = offset+s; wo[3] = 0; //channel swap
      char sp[4] = "SP"; sp[2] = offset+s; sp[3] = 0; //bus clock speed (DotStar & PWM)
      char la[4] = "LA"; la[2] = offset+s; la[3] = 0; //LED mA
      char ma[4] = "MA"; ma[2] = offset+s; ma[3] = 0; //max mA
      if (!request->hasArg(lp)) {
        DEBUG_PRINTF_P(PSTR("No data for %d\n"), s);
        break;
      }
      for (int i = 0; i < 5; i++) {
        lp[1] = offset+i;
        if (!request->hasArg(lp)) break;
        pins[i] = (request->arg(lp).length() > 0) ? request->arg(lp).toInt() : 255;
      }
      type = request->arg(lt).toInt();
      skip = request->arg(sl).toInt();
      colorOrder = request->arg(co).toInt();
      start = (request->hasArg(ls)) ? request->arg(ls).toInt() : t;
      if (request->hasArg(lc) && request->arg(lc).toInt() > 0) {
        t += length = request->arg(lc).toInt();
      } else {
        break;  // no parameter
      }
      awmode = request->arg(aw).toInt();
      uint16_t freq = request->arg(sp).toInt();
      if (Bus::isPWM(type)) {
        switch (freq) {
          case 0 : freq = WLED_PWM_FREQ/2;    break;
          case 1 : freq = WLED_PWM_FREQ*2/3;  break;
          default:
          case 2 : freq = WLED_PWM_FREQ;      break;
          case 3 : freq = WLED_PWM_FREQ*2;    break;
          case 4 : freq = WLED_PWM_FREQ*10/3; break; // uint16_t max (19531 * 3.333)
        }
      } else if (Bus::is2Pin(type)) {
        switch (freq) {
          default:
          case 0 : freq =  1000; break;
          case 1 : freq =  2000; break;
          case 2 : freq =  5000; break;
          case 3 : freq = 10000; break;
          case 4 : freq = 20000; break;
        }
      } else {
        freq = 0;
      }
      channelSwap = Bus::hasWhite(type) ? request->arg(wo).toInt() : 0;
      if (Bus::isOnOff(type) || Bus::isPWM(type) || Bus::isVirtual(type)) { // analog and virtual
        maPerLed = 0;
        maMax = 0;
      } else {
        maPerLed = request->arg(la).toInt();
        maMax = request->arg(ma).toInt(); // if ABL is disabled this will be 0
      }
      type |= request->hasArg(rf) << 7; // off refresh override
      // actual finalization is done in WLED::loop() (removing old busses and adding new)
      // this may happen even before this loop is finished so we do "doInitBusses" after the loop
      if (busConfigs[s] != nullptr) delete busConfigs[s];
      busConfigs[s] = new BusConfig(type, pins, start, length, colorOrder | (channelSwap<<4), request->hasArg(cv), skip, awmode, freq, useGlobalLedBuffer, maPerLed, maMax);
      busesChanged = true;
    }
    //doInitBusses = busesChanged; // we will do that below to ensure all input data is processed

    // we will not bother with pre-allocating ColorOrderMappings vector
    BusManager::getColorOrderMap().reset();
    for (int s = 0; s < WLED_MAX_COLOR_ORDER_MAPPINGS; s++) {
      int offset = s < 10 ? 48 : 55;
      char xs[4] = "XS"; xs[2] = offset+s; xs[3] = 0; //start LED
      char xc[4] = "XC"; xc[2] = offset+s; xc[3] = 0; //strip length
      char xo[4] = "XO"; xo[2] = offset+s; xo[3] = 0; //color order
      char xw[4] = "XW"; xw[2] = offset+s; xw[3] = 0; //W swap
      if (request->hasArg(xs)) {
        start = request->arg(xs).toInt();
        length = request->arg(xc).toInt();
        colorOrder = request->arg(xo).toInt() & 0x0F;
        colorOrder |= (request->arg(xw).toInt() & 0x0F) << 4; // add W swap information
        if (!BusManager::getColorOrderMap().add(start, length, colorOrder)) break;
      }
    }

    // update other pins
    #ifndef WLED_DISABLE_INFRARED
    int hw_ir_pin = request->arg(F("IR")).toInt();
    if (PinManager::allocatePin(hw_ir_pin,false, PinOwner::IR)) {
      irPin = hw_ir_pin;
    } else {
      irPin = -1;
    }
    irEnabled = request->arg(F("IT")).toInt();
    initIR();
    #endif
    irApplyToAllSelected = !request->hasArg(F("MSO"));

    int hw_rly_pin = request->arg(F("RL")).toInt();
    if (PinManager::allocatePin(hw_rly_pin,true, PinOwner::Relay)) {
      rlyPin = hw_rly_pin;
    } else {
      rlyPin = -1;
    }
    rlyMde = (bool)request->hasArg(F("RM"));
    rlyOpenDrain = (bool)request->hasArg(F("RO"));

    disablePullUp = (bool)request->hasArg(F("IP"));
    touchThreshold = request->arg(F("TT")).toInt();
    for (int i = 0; i < WLED_MAX_BUTTONS; i++) {
      int offset = i < 10 ? 48 : 55;
      char bt[4] = "BT"; bt[2] = offset+i; bt[3] = 0; // button pin (use A,B,C,... if WLED_MAX_BUTTONS>10)
      char be[4] = "BE"; be[2] = offset+i; be[3] = 0; // button type (use A,B,C,... if WLED_MAX_BUTTONS>10)
      int hw_btn_pin = request->arg(bt).toInt();
      if (hw_btn_pin >= 0 && PinManager::allocatePin(hw_btn_pin,false,PinOwner::Button)) {
        btnPin[i] = hw_btn_pin;
        buttonType[i] = request->arg(be).toInt();
      #ifdef ARDUINO_ARCH_ESP32
        // ESP32 only: check that button pin is a valid gpio
        if ((buttonType[i] == BTN_TYPE_ANALOG) || (buttonType[i] == BTN_TYPE_ANALOG_INVERTED))
        {
          if (digitalPinToAnalogChannel(btnPin[i]) < 0) {
            // not an ADC analog pin
            DEBUG_PRINTF_P(PSTR("PIN ALLOC error: GPIO%d for analog button #%d is not an analog pin!\n"), btnPin[i], i);
            btnPin[i] = -1;
            PinManager::deallocatePin(hw_btn_pin,PinOwner::Button);
          } else {
            analogReadResolution(12); // see #4040
          }
        }
        else if ((buttonType[i] == BTN_TYPE_TOUCH || buttonType[i] == BTN_TYPE_TOUCH_SWITCH))
        {
          if (digitalPinToTouchChannel(btnPin[i]) < 0)
          {
            // not a touch pin
            DEBUG_PRINTF_P(PSTR("PIN ALLOC error: GPIO%d for touch button #%d is not an touch pin!\n"), btnPin[i], i);
            btnPin[i] = -1;
            PinManager::deallocatePin(hw_btn_pin,PinOwner::Button);
          }          
          #ifdef SOC_TOUCH_VERSION_2 // ESP32 S2 and S3 have a fucntion to check touch state but need to attach an interrupt to do so
          else                    
          {
            touchAttachInterrupt(btnPin[i], touchButtonISR, touchThreshold << 4); // threshold on Touch V2 is much higher (1500 is a value given by Espressif example, I measured changes of over 5000)
          }
          #endif          
        }
        else
      #endif
        {
          if (disablePullUp) {
            pinMode(btnPin[i], INPUT);
          } else {
            #ifdef ESP32
            pinMode(btnPin[i], buttonType[i]==BTN_TYPE_PUSH_ACT_HIGH ? INPUT_PULLDOWN : INPUT_PULLUP);
            #else
            pinMode(btnPin[i], INPUT_PULLUP);
            #endif
          }
        }
      } else {
        btnPin[i] = -1;
        buttonType[i] = BTN_TYPE_NONE;
      }
    }

    briS = request->arg(F("CA")).toInt();

    turnOnAtBoot = request->hasArg(F("BO"));
    t = request->arg(F("BP")).toInt();
    if (t <= 250) bootPreset = t;
    gammaCorrectBri = request->hasArg(F("GB"));
    gammaCorrectCol = request->hasArg(F("GC"));
    gammaCorrectVal = request->arg(F("GV")).toFloat();
    if (gammaCorrectVal <= 1.0f || gammaCorrectVal > 3) {
      gammaCorrectVal = 1.0f; // no gamma correction
      gammaCorrectBri = false;
      gammaCorrectCol = false;
    }
    NeoGammaWLEDMethod::calcGammaTable(gammaCorrectVal); // fill look-up table

    fadeTransition = request->hasArg(F("TF"));
    modeBlending = request->hasArg(F("EB"));
    t = request->arg(F("TD")).toInt();
    if (t >= 0) transitionDelayDefault = t;
    paletteFade = request->hasArg(F("PF"));
    t = request->arg(F("TP")).toInt();
    randomPaletteChangeTime = MIN(255,MAX(1,t));
    useHarmonicRandomPalette = request->hasArg(F("TH"));

    nightlightTargetBri = request->arg(F("TB")).toInt();
    t = request->arg(F("TL")).toInt();
    if (t > 0) nightlightDelayMinsDefault = t;
    nightlightDelayMins = nightlightDelayMinsDefault;
    nightlightMode = request->arg(F("TW")).toInt();

    t = request->arg(F("PB")).toInt();
    if (t >= 0 && t < 4) paletteBlend = t;
    t = request->arg(F("BF")).toInt();
    if (t > 0) briMultiplier = t;

    doInitBusses = busesChanged;
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_LEDS
  }

  //UI
  if (subPage == SUBPAGE_UI)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_UI
    strlcpy(serverDescription, request->arg(F("DS")).c_str(), 33);
    //syncToggleReceive = request->hasArg(F("ST"));
    simplifiedUI = request->hasArg(F("SU"));
    DEBUG_PRINTLN(F("Enumerating ledmaps"));
    enumerateLedmaps();
    DEBUG_PRINTLN(F("Loading custom palettes"));
    loadCustomPalettes(); // (re)load all custom palettes
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_UI
  }

  //SYNC
  if (subPage == SUBPAGE_SYNC)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_SYNC
    int t = request->arg(F("UP")).toInt();
    if (t > 0) udpPort = t;
    t = request->arg(F("U2")).toInt();
    if (t > 0) udpPort2 = t;

    #ifndef WLED_DISABLE_ESPNOW
    useESPNowSync = request->hasArg(F("EN"));
    #endif

    syncGroups = request->arg(F("GS")).toInt();
    receiveGroups = request->arg(F("GR")).toInt();

    receiveNotificationBrightness = request->hasArg(F("RB"));
    receiveNotificationColor = request->hasArg(F("RC"));
    receiveNotificationEffects = request->hasArg(F("RX"));
    receiveNotificationPalette = request->hasArg(F("RP"));
    receiveSegmentOptions = request->hasArg(F("SO"));
    receiveSegmentBounds = request->hasArg(F("SG"));
    sendNotifications = request->hasArg(F("SS"));
    notifyDirect = request->hasArg(F("SD"));
    notifyButton = request->hasArg(F("SB"));
    notifyAlexa = request->hasArg(F("SA"));
    notifyHue = request->hasArg(F("SH"));

    t = request->arg(F("UR")).toInt();
    if ((t>=0) && (t<30)) udpNumRetries = t;


    nodeListEnabled = request->hasArg(F("NL"));
    if (!nodeListEnabled) Nodes.clear();
    nodeBroadcastEnabled = request->hasArg(F("NB"));

    receiveDirect = request->hasArg(F("RD")); // UDP realtime
    useMainSegmentOnly = request->hasArg(F("MO"));
    realtimeRespectLedMaps = request->hasArg(F("RLM"));
    e131SkipOutOfSequence = request->hasArg(F("ES"));
    e131Multicast = request->hasArg(F("EM"));
    t = request->arg(F("EP")).toInt();
    if (t > 0) e131Port = t;
    t = request->arg(F("EU")).toInt();
    if (t >= 0  && t <= 63999) e131Universe = t;
    t = request->arg(F("DA")).toInt();
    if (t >= 0  && t <= 510) DMXAddress = t;
    t = request->arg(F("XX")).toInt();
    if (t >= 0  && t <= 150) DMXSegmentSpacing = t;
    t = request->arg(F("PY")).toInt();
    if (t >= 0  && t <= 200) e131Priority = t;
    t = request->arg(F("DM")).toInt();
    if (t >= DMX_MODE_DISABLED && t <= DMX_MODE_PRESET) DMXMode = t;
    t = request->arg(F("ET")).toInt();
    if (t > 99  && t <= 65000) realtimeTimeoutMs = t;
    arlsForceMaxBri = request->hasArg(F("FB"));
    arlsDisableGammaCorrection = request->hasArg(F("RG"));
    t = request->arg(F("WO")).toInt();
    if (t >= -255  && t <= 255) arlsOffset = t;

    #ifndef WLED_DISABLE_ALEXA
    alexaEnabled = request->hasArg(F("AL"));
    strlcpy(alexaInvocationName, request->arg(F("AI")).c_str(), 33);
    t = request->arg(F("AP")).toInt();
    if (t >= 0 && t <= 9) alexaNumPresets = t;
    #endif

    #ifndef WLED_DISABLE_MQTT
    mqttEnabled = request->hasArg(F("MQ"));
    strlcpy(mqttServer, request->arg(F("MS")).c_str(), MQTT_MAX_SERVER_LEN+1);
    t = request->arg(F("MQPORT")).toInt();
    if (t > 0) mqttPort = t;
    strlcpy(mqttUser, request->arg(F("MQUSER")).c_str(), 41);
    if (!isAsterisksOnly(request->arg(F("MQPASS")).c_str(), 41)) strlcpy(mqttPass, request->arg(F("MQPASS")).c_str(), 65);
    strlcpy(mqttClientID, request->arg(F("MQCID")).c_str(), 41);
    strlcpy(mqttDeviceTopic, request->arg(F("MD")).c_str(), MQTT_MAX_TOPIC_LEN+1);
    strlcpy(mqttGroupTopic, request->arg(F("MG")).c_str(), MQTT_MAX_TOPIC_LEN+1);
    buttonPublishMqtt = request->hasArg(F("BM"));
    retainMqttMsg = request->hasArg(F("RT"));
    #endif

    #ifndef WLED_DISABLE_HUESYNC
    for (int i=0;i<4;i++){
      String a = "H"+String(i);
      hueIP[i] = request->arg(a).toInt();
    }

    t = request->arg(F("HL")).toInt();
    if (t > 0) huePollLightId = t;

    t = request->arg(F("HI")).toInt();
    if (t > 50) huePollIntervalMs = t;

    hueApplyOnOff = request->hasArg(F("HO"));
    hueApplyBri = request->hasArg(F("HB"));
    hueApplyColor = request->hasArg(F("HC"));
    huePollingEnabled = request->hasArg(F("HP"));
    hueStoreAllowed = true;
    reconnectHue();
    #endif

    t = request->arg(F("BD")).toInt();
    if (t >= 96 && t <= 15000) serialBaud = t;
    updateBaudRate(serialBaud *100);
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_SYNC
  }

  //TIME
  if (subPage == SUBPAGE_TIME)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_TIME
    ntpEnabled = request->hasArg(F("NT"));
    strlcpy(ntpServerName, request->arg(F("NS")).c_str(), 33);
    useAMPM = !request->hasArg(F("CF"));
    currentTimezone = request->arg(F("TZ")).toInt();
    utcOffsetSecs = request->arg(F("UO")).toInt();

    //start ntp if not already connected
    if (ntpEnabled && WLED_CONNECTED && !ntpConnected) ntpConnected = ntpUdp.begin(ntpLocalPort);
    ntpLastSyncTime = NTP_NEVER; // force new NTP query

    longitude = request->arg(F("LN")).toFloat();
    latitude = request->arg(F("LT")).toFloat();
    // force a sunrise/sunset re-calculation
    calculateSunriseAndSunset();

    overlayCurrent = request->hasArg(F("OL")) ? 1 : 0;

    overlayMin = request->arg(F("O1")).toInt();
    overlayMax = request->arg(F("O2")).toInt();
    analogClock12pixel = request->arg(F("OM")).toInt();
    analogClock5MinuteMarks = request->hasArg(F("O5"));
    analogClockSecondsTrail = request->hasArg(F("OS"));
    analogClockSolidBlack = request->hasArg(F("OB"));

    countdownMode = request->hasArg(F("CE"));
    countdownYear = request->arg(F("CY")).toInt();
    countdownMonth = request->arg(F("CI")).toInt();
    countdownDay = request->arg(F("CD")).toInt();
    countdownHour = request->arg(F("CH")).toInt();
    countdownMin = request->arg(F("CM")).toInt();
    countdownSec = request->arg(F("CS")).toInt();
    setCountdown();

    macroAlexaOn = request->arg(F("A0")).toInt();
    macroAlexaOff = request->arg(F("A1")).toInt();
    macroCountdown = request->arg(F("MC")).toInt();
    macroNl = request->arg(F("MN")).toInt();
    for (unsigned i=0; i<WLED_MAX_BUTTONS; i++) {
      char mp[4] = "MP"; mp[2] = (i<10?48:55)+i; mp[3] = 0; // short
      char ml[4] = "ML"; ml[2] = (i<10?48:55)+i; ml[3] = 0; // long
      char md[4] = "MD"; md[2] = (i<10?48:55)+i; md[3] = 0; // double
      //if (!request->hasArg(mp)) break;
      macroButton[i] = request->arg(mp).toInt();      // these will default to 0 if not present
      macroLongPress[i] = request->arg(ml).toInt();
      macroDoublePress[i] = request->arg(md).toInt();
    }

    char k[3]; k[2] = 0;
    for (int i = 0; i<10; i++) {
      k[1] = i+48;//ascii 0,1,2,3,...
      k[0] = 'H'; //timer hours
      timerHours[i] = request->arg(k).toInt();
      k[0] = 'N'; //minutes
      timerMinutes[i] = request->arg(k).toInt();
      k[0] = 'T'; //macros
      timerMacro[i] = request->arg(k).toInt();
      k[0] = 'W'; //weekdays
      timerWeekday[i] = request->arg(k).toInt();
      if (i<8) {
        k[0] = 'M'; //start month
        timerMonth[i] = request->arg(k).toInt() & 0x0F;
        timerMonth[i] <<= 4;
        k[0] = 'P'; //end month
        timerMonth[i] += (request->arg(k).toInt() & 0x0F);
        k[0] = 'D'; //start day
        timerDay[i] = request->arg(k).toInt();
        k[0] = 'E'; //end day
        timerDayEnd[i] = request->arg(k).toInt();
      }
    }
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_TIME
  }

  //SECURITY
  if (subPage == SUBPAGE_SEC)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_SEC
    if (request->hasArg(F("RS"))) //complete factory reset
    {
      WLED_FS.format();
      #ifdef WLED_ADD_EEPROM_SUPPORT
      clearEEPROM();
      #endif
      serveMessage(request, 200, F("All Settings erased."), F("Connect to WLED-AP to setup again"),255);
      doReboot = true; // may reboot immediately on dual-core system (race condition) which is desireable in this case
    }

    if (request->hasArg(F("PIN"))) {
      const char *pin = request->arg(F("PIN")).c_str();
      unsigned pinLen = strlen(pin);
      if (pinLen == 4 || pinLen == 0) {
        unsigned numZeros = 0;
        for (unsigned i = 0; i < pinLen; i++) numZeros += (pin[i] == '0');
        if (numZeros < pinLen || pinLen == 0) { // ignore 0000 input (placeholder)
          strlcpy(settingsPIN, pin, 5);
        }
        settingsPIN[4] = 0;
      }
    }

    bool pwdCorrect = !otaLock; //always allow access if ota not locked
    if (request->hasArg(F("OP")))
    {
      if (otaLock && strcmp(otaPass,request->arg(F("OP")).c_str()) == 0)
      {
        // brute force protection: do not unlock even if correct if last save was less than 3 seconds ago
        if (millis() - lastEditTime > PIN_RETRY_COOLDOWN) pwdCorrect = true;
      }
      if (!otaLock && request->arg(F("OP")).length() > 0)
      {
        strlcpy(otaPass,request->arg(F("OP")).c_str(), 33); // set new OTA password
      }
    }

    if (pwdCorrect) //allow changes if correct pwd or no ota active
    {
      otaLock = request->hasArg(F("NO"));
      wifiLock = request->hasArg(F("OW"));
      aOtaEnabled = request->hasArg(F("AO"));
      //createEditHandler(correctPIN && !otaLock);
    }
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_SEC
  }

  #ifdef ENABLE_FEATURE_LIGHTING__DMX // include only if DMX is enabled
  if (subPage == SUBPAGE_DMX)
  {
    int t = request->arg(F("PU")).toInt();
    if (t >= 0  && t <= 63999) e131ProxyUniverse = t;

    t = request->arg(F("CN")).toInt();
    if (t>0 && t<16) {
      DMXChannels = t;
    }
    t = request->arg(F("CS")).toInt();
    if (t>0 && t<513) {
      DMXStart = t;
    }
    t = request->arg(F("CG")).toInt();
    if (t>0 && t<513) {
      DMXGap = t;
    }
    t = request->arg(F("SL")).toInt();
    if (t>=0 && t < MAX_LEDS) {
      DMXStartLED = t;
    }
    for (int i=0; i<15; i++) {
      String argname = "CH" + String((i+1));
      t = request->arg(argname).toInt();
      DMXFixtureMap[i] = t;
    }
  }
  #endif

  //USERMODS
  if (subPage == SUBPAGE_UM)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_UM
    if (!requestJSONBufferLock(5)) return;

    // global I2C & SPI pins
    int8_t hw_sda_pin  = !request->arg(F("SDA")).length() ? -1 : (int)request->arg(F("SDA")).toInt();
    int8_t hw_scl_pin  = !request->arg(F("SCL")).length() ? -1 : (int)request->arg(F("SCL")).toInt();
    if (i2c_sda != hw_sda_pin || i2c_scl != hw_scl_pin) {
      // only if pins changed
      uint8_t old_i2c[2] = { static_cast<uint8_t>(i2c_scl), static_cast<uint8_t>(i2c_sda) };
      PinManager::deallocateMultiplePins(old_i2c, 2, PinOwner::HW_I2C); // just in case deallocation of old pins

      PinManagerPinType i2c[2] = { { hw_sda_pin, true }, { hw_scl_pin, true } };
      if (hw_sda_pin >= 0 && hw_scl_pin >= 0 && PinManager::allocateMultiplePins(i2c, 2, PinOwner::HW_I2C)) {
        i2c_sda = hw_sda_pin;
        i2c_scl = hw_scl_pin;
        // no bus re-initialisation as usermods do not get any notification
        //Wire.begin(i2c_sda, i2c_scl);
      } else {
        // there is no Wire.end()
        DEBUG_PRINTLN(F("Could not allocate I2C pins."));
        i2c_sda = -1;
        i2c_scl = -1;
      }
    }
    int8_t hw_mosi_pin = !request->arg(F("MOSI")).length() ? -1 : (int)request->arg(F("MOSI")).toInt();
    int8_t hw_miso_pin = !request->arg(F("MISO")).length() ? -1 : (int)request->arg(F("MISO")).toInt();
    int8_t hw_sclk_pin = !request->arg(F("SCLK")).length() ? -1 : (int)request->arg(F("SCLK")).toInt();
    #ifdef ESP8266
    // cannot change pins on ESP8266
    if (hw_mosi_pin >= 0 && hw_mosi_pin != HW_PIN_DATASPI)  hw_mosi_pin = HW_PIN_DATASPI;
    if (hw_miso_pin >= 0 && hw_miso_pin != HW_PIN_MISOSPI)  hw_mosi_pin = HW_PIN_MISOSPI;
    if (hw_sclk_pin >= 0 && hw_sclk_pin != HW_PIN_CLOCKSPI) hw_sclk_pin = HW_PIN_CLOCKSPI;
    #endif
    if (spi_mosi != hw_mosi_pin || spi_miso != hw_miso_pin || spi_sclk != hw_sclk_pin) {
      // only if pins changed
      uint8_t old_spi[3] = { static_cast<uint8_t>(spi_mosi), static_cast<uint8_t>(spi_miso), static_cast<uint8_t>(spi_sclk) };
      PinManager::deallocateMultiplePins(old_spi, 3, PinOwner::HW_SPI); // just in case deallocation of old pins
      PinManagerPinType spi[3] = { { hw_mosi_pin, true }, { hw_miso_pin, true }, { hw_sclk_pin, true } };
      if (hw_mosi_pin >= 0 && hw_sclk_pin >= 0 && PinManager::allocateMultiplePins(spi, 3, PinOwner::HW_SPI)) {
        spi_mosi = hw_mosi_pin;
        spi_miso = hw_miso_pin;
        spi_sclk = hw_sclk_pin;
        // no bus re-initialisation as usermods do not get any notification
        //SPI.end();
        #ifdef ESP32
        //SPI.begin(spi_sclk, spi_miso, spi_mosi);
        #else
        //SPI.begin();
        #endif
      } else {
        //SPI.end();
        DEBUG_PRINTLN(F("Could not allocate SPI pins."));
        spi_mosi = -1;
        spi_miso = -1;
        spi_sclk = -1;
      }
    }

    JsonObject um = pDoc->createNestedObject("um");

    size_t args = request->args();
    unsigned j=0;
    for (size_t i=0; i<args; i++) {
      String name = request->argName(i);
      String value = request->arg(i);

      // POST request parameters are combined as <usermodname>_<usermodparameter>
      int umNameEnd = name.indexOf(":");
      if (umNameEnd<1) continue;  // parameter does not contain ":" or on 1st place -> wrong

      JsonObject mod = um[name.substring(0,umNameEnd)]; // get a usermod JSON object
      if (mod.isNull()) {
        mod = um.createNestedObject(name.substring(0,umNameEnd)); // if it does not exist create it
      }
      DEBUG_PRINT(name.substring(0,umNameEnd));
      DEBUG_PRINT(":");
      name = name.substring(umNameEnd+1); // remove mod name from string

      // if the resulting name still contains ":" this means nested object
      JsonObject subObj;
      int umSubObj = name.indexOf(":");
      DEBUG_PRINTF_P(PSTR("(%d):"),umSubObj);
      if (umSubObj>0) {
        subObj = mod[name.substring(0,umSubObj)];
        if (subObj.isNull())
          subObj = mod.createNestedObject(name.substring(0,umSubObj));
        name = name.substring(umSubObj+1); // remove nested object name from string
      } else {
        subObj = mod;
      }
      DEBUG_PRINT(name);

      // check if parameters represent array
      if (name.endsWith("[]")) {
        name.replace("[]","");
        value.replace(",",".");      // just in case conversion
        if (!subObj[name].is<JsonArray>()) {
          JsonArray ar = subObj.createNestedArray(name);
          if (value.indexOf(".") >= 0) ar.add(value.toFloat());  // we do have a float
          else                         ar.add(value.toInt());    // we may have an int
          j=0;
        } else {
          if (value.indexOf(".") >= 0) subObj[name].add(value.toFloat());  // we do have a float
          else                         subObj[name].add(value.toInt());    // we may have an int
          j++;
        }
        DEBUG_PRINTF_P(PSTR("[%d] = %s\n"), j, value.c_str());
      } else {
        // we are using a hidden field with the same name as our parameter (!before the actual parameter!)
        // to describe the type of parameter (text,float,int), for boolean parameters the first field contains "off"
        // so checkboxes have one or two fields (first is always "false", existence of second depends on checkmark and may be "true")
        if (subObj[name].isNull()) {
          // the first occurrence of the field describes the parameter type (used in next loop)
          if (value == "false") subObj[name] = false; // checkboxes may have only one field
          else                  subObj[name] = value;
        } else {
          String type = subObj[name].as<String>();  // get previously stored value as a type
          if (subObj[name].is<bool>())   subObj[name] = true;   // checkbox/boolean
          else if (type == "number") {
            value.replace(",",".");      // just in case conversion
            if (value.indexOf(".") >= 0) subObj[name] = value.toFloat();  // we do have a float
            else                         subObj[name] = value.toInt();    // we may have an int
          } else if (type == "int")      subObj[name] = value.toInt();
          else                           subObj[name] = value;  // text fields
        }
        DEBUG_PRINTF_P(PSTR(" = %s\n"), value.c_str());
      }
    }
    UsermodManager::readFromConfig(um);  // force change of usermod parameters
    DEBUG_PRINTLN(F("Done re-init UsermodManager::"));
    releaseJSONBufferLock();
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_UM
  }

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  //2D panels
  if (subPage == SUBPAGE_2D)
  {
    #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_2D
    isMatrix = request->arg(F("SOMP")).toInt();
    panel.clear(); // release memory if allocated
    ALOG_INF(PSTR("panel.clear() HERE F?????????????????????????????????????????????????????"));
    if (isMatrix) {
      panels  = MAX(1,MIN(WLED_MAX_PANELS,request->arg(F("MPC")).toInt()));
      panel.reserve(panels); // pre-allocate memory
      for (unsigned i=0; i<panels; i++) {
        WS2812FX::Panel p;
        char pO[8] = { '\0' };
        snprintf_P(pO, 7, PSTR("P%d"), i);       // MAX_PANELS is 64 so pO will always only be 4 characters or less
        pO[7] = '\0';
        unsigned l = strlen(pO);
        // create P0B, P1B, ..., P63B, etc for other PxxX
        pO[l] = 'B'; if (!request->hasArg(pO)) break;
        pO[l] = 'B'; p.bottomStart = request->arg(pO).toInt();
        pO[l] = 'R'; p.rightStart  = request->arg(pO).toInt();
        pO[l] = 'V'; p.vertical    = request->arg(pO).toInt();
        pO[l] = 'S'; p.serpentine  = request->hasArg(pO);
        pO[l] = 'X'; p.xOffset     = request->arg(pO).toInt();
        pO[l] = 'Y'; p.yOffset     = request->arg(pO).toInt();
        pO[l] = 'W'; p.width       = request->arg(pO).toInt();
        pO[l] = 'H'; p.height      = request->arg(pO).toInt();
        panel.push_back(p);
      }
      setUpMatrix(); // will check limits
      makeAutoSegments(true);
      deserializeMap();
    } else {
      Segment::maxWidth  = getLengthTotal();
      Segment::maxHeight = 1;
    }
    #endif // ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_2D
  }
  #endif

  tkr_anim->lastEditTime = millis();
  // do not save if factory reset or LED settings (which are saved after LED re-init)
  tkr_anim->doSerializeConfig = subPage != SUBPAGE_LEDS && !(subPage == SUBPAGE_SEC && tkr_anim->doReboot);
  if (subPage == SUBPAGE_UM) tkr_anim->doReboot = request->hasArg(F("RBT")); // prevent race condition on dual core system (set reboot here, after doSerializeConfig has been set)
  #ifndef WLED_DISABLE_ALEXA
  #ifdef ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS__SUBPAGE_SYNC
  if (subPage == SUBPAGE_SYNC) alexaInit();
  #endif
  #endif
}




#endif // ENABLE_DEVFEATURE_LIGHTING__SETTINGS