#include "mWiFi.h"

// #ifdef ESP8266

#ifdef ESP8266
extern "C" {
 #include "user_interface.h"
}
#endif


void mWiFi::init(void){
  
//  AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::init");

}


int mWiFi::WifiGetRssiAsQuality(int rssi)
{
  int quality = 0;

  if (rssi <= -100) {
    quality = 0;
  } else if (rssi >= -50) {
    quality = 100;
  } else {
    quality = 2 * (rssi + 100);
  }
  return quality;
}

bool mWiFi::WifiConfigCounter(void)
{
  if (wifi_config_counter) {
    wifi_config_counter = WIFI_CONFIG_SEC;
  }
  return wifi_config_counter;
}


void mWiFi::SplashWifiScan(){
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  if (n == 0) {
    AddLog_P(LOG_LEVEL_INFO,PSTR("NO networks found"));
  } else {
    AddLog_P(LOG_LEVEL_INFO,PSTR("%d networks found"),n);
    for (int i = 0; i < n; ++i) {
      AddLog_P(LOG_LEVEL_INFO,PSTR("%d : %s\t %d dBm"),i+1,WiFi.SSID(i).c_str(),WiFi.RSSI(i));
    }
  }
}


//#if defined(USE_WIFI_FORCED_PRIMARY) || defined(USE_WIFI_FORCED_SECONDARY)
void mWiFi::WifiConnectForced(){

  #if defined(USE_WIFI_FORCED_SECONDARY)
    const char* ssid     = STA_SSID2;
    const char* password = STA_PASS2;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "USE_WIFI_FORCED_SECONDARY"));
  #elif defined(USE_WIFI_FORCED_PRIMARY)
    const char* ssid     = STA_SSID1;
    const char* password = STA_PASS1;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "USE_WIFI_FORCED_PRIMARY")); 
  #else 
    const char* ssid     = STA_SSID1;
    const char* password = STA_PASS1;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "USE_WIFI_FORCED_PRIMARY"));
  #endif

  uint8_t timeoutcounts = 0; 
  uint32_t tSaved = millis();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  while (WiFi.status() != WL_CONNECTED) {
    #ifdef ESP8266
      ESP.wdtFeed();
    #endif
    //delay(1000);
    if(abs(millis()-tSaved)>1000){tSaved=millis();
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "Waiting for Connection to %s : %d/%d seconds..."),ssid,timeoutcounts,10);
      Serial.print(".");
      if(timeoutcounts++ > 10){
        break;
      }
    }
  }

  if(WiFi.status() == WL_CONNECTED){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "Connected to %s with %s (%d dBm)"),WiFi.SSID().c_str(),WiFi.localIP().toString().c_str(),WiFi.RSSI());
    #ifdef USE_NETWORK_MDNS
    #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
    //MDNS.begin(pCONT_set->Settings.system_name.device);
    #endif // #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
    #endif // #ifdef USE_NETWORK_MDNS
  }else{
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_WIFI "FAILED to connect!"));
  }
  
}
//#endif


void mWiFi::WifiConfig(uint8_t type)
{
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "mWiFi::WifiConfig=%s"),GetWiFiConfigTypeCtr());

  if (!wifi_config_type) {
    
  AddLog_P(LOG_LEVEL_INFO, PSTR("!wifi_config_type"));


    if ((WIFI_RETRY == type) || (WIFI_WAIT == type)) { 
  AddLog_P(LOG_LEVEL_INFO, PSTR("return"));
      return; 
    }
    #if defined(USE_MODULE_CORE_WEBSERVER) && defined(USE_EMULATION)
      //  UdpDisconnect();
    #endif  // USE_EMULATION
    WiFi.disconnect();                       // Solve possible Wifi hangs
    wifi_config_type = type;

    #ifndef USE_WPS
        if (WIFI_WPSCONFIG == wifi_config_type) { wifi_config_type = WIFI_MANAGER; }
    #endif  // USE_WPS
    #ifndef USE_MODULE_CORE_WEBSERVER
        if (WIFI_MANAGER == wifi_config_type) { wifi_config_type = WIFI_SMARTCONFIG; }
    #endif  // USE_MODULE_CORE_WEBSERVER
    #ifndef USE_SMARTCONFIG
        if (WIFI_SMARTCONFIG == wifi_config_type) { wifi_config_type = WIFI_SERIAL; }
    #endif  // USE_SMARTCONFIG

    wifi_config_counter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
    wifi_counter = wifi_config_counter +5;
    // blinks = 1999;
    if (WIFI_RESTART == wifi_config_type) {// wifi_reconnects_counter++;
    //  restart_flag = 2; restarts device, make this a user flag
    }
    else if (WIFI_SERIAL == wifi_config_type) {
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_6_SERIAL " " D_ACTIVE_FOR_3_MINUTES));
    }
#ifdef USE_MODULE_CORE_WEBSERVER
    else if (WIFI_MANAGER == wifi_config_type || WIFI_MANAGER_RESET_ONLY == wifi_config_type) {
     AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_WCFG_2_WIFIMANAGER " " D_ACTIVE_FOR_3_MINUTES));
     pCONT_web->WifiManagerBegin(WIFI_MANAGER_RESET_ONLY == wifi_config_type);
    }
#endif  // USE_MODULE_CORE_WEBSERVER
  }else{
    
  AddLog_P(LOG_LEVEL_INFO, PSTR("else wifi_config_type"));
  }
}

void mWiFi::WiFiSetSleepMode(void)
{
/* Excerpt from the esp8266 non os sdk api reference (v2.2.1):
 * Sets sleep type for power saving. Set WIFI_NONE_SLEEP to disable power saving.
 * - Default mode: WIFI_MODEM_SLEEP.
 * - In order to lower the power comsumption, ESP8266 changes the TCP timer
 *   tick from 250ms to 3s in WIFI_LIGHT_SLEEP mode, which leads to increased timeout for
 *   TCP timer. Therefore, the WIFI_MODEM_SLEEP or deep-sleep mode should be used
 *   where there is a requirement for the accurancy of the TCP timer.
 *
 * Sleep is disabled in core 2.4.1 and 2.4.2 as there are bugs in their SDKs
 * See https://github.com/arendst/Sonoff-Tasmota/issues/2559
 */

// Sleep explanation: https://github.com/esp8266/Arduino/blob/3f0c601cfe81439ce17e9bd5d28994a7ed144482/libraries/ESP8266WiFi/src/ESP8266WiFiGeneric.cpp#L255
#if defined(ARDUINO_ESP8266_RELEASE_2_4_1) || defined(ARDUINO_ESP8266_RELEASE_2_4_2)
#else  // Enabled in 2.3.0, 2.4.0 and stage
  // if (sleep && Settings.flag_network.sleep_normal) {
  //   WiFi.setSleepMode(WIFI_LIGHT_SLEEP);  // Allow light sleep during idle times
  // } else {
  //   WiFi.setSleepMode(WIFI_MODEM_SLEEP);  // Disable sleep (Esp8288/Arduino core and sdk default)
  // }
#endif
}

void mWiFi::WifiBegin(uint8_t flag, uint8_t channel)
{
// AddLog_P(LOG_LEVEL_INFO, PSTR("mWiFi::WifiBegin %d:%d"), flag,channel);

  AddLog_P(LOG_LEVEL_DEBUG, PSTR("F::%s"),__FUNCTION__);

  const char kWifiPhyMode[] = " BGN";

#if defined(USE_MODULE_CORE_WEBSERVER) && defined(USE_EMULATION)
//  UdpDisconnect();
#endif  // USE_EMULATION

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0  // (!strncmp_P(ESP.getSdkVersion(),PSTR("1.5.3"),5))
  AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR(D_PATCH_ISSUE_2186));
  WiFi.mode(WIFI_OFF);      // See https://github.com/esp8266/Arduino/issues/2186
#endif

  WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
  WiFi.disconnect(true);    // Delete SDK wifi config
  delay(200);
  WiFi.mode(WIFI_STA);      // Disable AP mode
  WiFiSetSleepMode();
  #ifdef ESP8266
 if (WiFi.getPhyMode() != WIFI_PHY_MODE_11N) { WiFi.setPhyMode(WIFI_PHY_MODE_11N); }  // B/G/N
 if (WiFi.getPhyMode() != WIFI_PHY_MODE_11G) { WiFi.setPhyMode(WIFI_PHY_MODE_11G); }  // B/G
 #endif
  if (!WiFi.getAutoConnect()) { WiFi.setAutoConnect(true); }
  WiFi.setAutoReconnect(true);

  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_WIFI "flag=%d"),flag);

  switch (flag) {
    default:
    case WifiBegin_FLAG_SSID0_ID:  // AP1
    case WifiBegin_FLAG_SSID1_ID:  // AP2
    case WifiBegin_FLAG_SSID2_ID:  // AP3
      pCONT_set->Settings.sta_active = flag;
      break;
    case WifiBegin_FLAG_TOGGLE_SSIDS_ID:  // Toggle now CYCLE
      if(pCONT_set->Settings.sta_active>=2){ pCONT_set->Settings.sta_active = WifiBegin_FLAG_SSID0_ID; }
      else{ pCONT_set->Settings.sta_active++; } 
    break;
  }        // 3: Current AP

  //cant toggle, needs to shift between 3
  if ('\0' == pCONT_set->Settings.sta_ssid[pCONT_set->Settings.sta_active][0]) { 
    if(pCONT_set->Settings.sta_active>=2){ pCONT_set->Settings.sta_active = WifiBegin_FLAG_SSID0_ID; }
    else{ pCONT_set->Settings.sta_active++; } 
  }  // Skip empty SSID


  // if (pCONT_set->Settings.ip_address[0]) {
  //   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_WIFI "Settings.ip_address=%s"),"true");
  //   WiFi.config(pCONT_set->Settings.ip_address[0], 
  //               pCONT_set->Settings.ip_address[1], 
  //               pCONT_set->Settings.ip_address[2], 
  //               pCONT_set->Settings.ip_address[3]
  //               );  // Set static IP
  // }else{
  //   AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_WIFI "Settings.ip_address=%s"),"false");
  // }
  
  #ifdef ESP8266
    WiFi.hostname(pCONT_set->my_hostname);
  #endif
  // if (channel) {
  //   WiFi.begin(pCONT_set->Settings.sta_ssid[pCONT_set->Settings.sta_active], pCONT_set->Settings.sta_pwd[pCONT_set->Settings.sta_active], channel, wifi_bssid);
  // } else {
    
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "sta_ssid[%d]=%s"),pCONT_set->Settings.sta_active,pCONT_set->Settings.sta_ssid[pCONT_set->Settings.sta_active]);
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "sta_pwd[%d]=%s"),pCONT_set->Settings.sta_active,pCONT_set->Settings.sta_pwd[pCONT_set->Settings.sta_active]);

    WiFi.begin(pCONT_set->Settings.sta_ssid[pCONT_set->Settings.sta_active], pCONT_set->Settings.sta_pwd[pCONT_set->Settings.sta_active]);
  //}
  
  #ifdef ESP8266
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d \"%s\" \"%s\" " D_IN_MODE " 11%c " D_AS " %s"),
    pCONT_set->Settings.sta_active +1, 
    pCONT_set->Settings.sta_ssid[pCONT_set->Settings.sta_active], 
    pCONT_set->Settings.sta_pwd[pCONT_set->Settings.sta_active], 
    kWifiPhyMode[WiFi.getPhyMode() & 0x3], 
    pCONT_set->my_hostname);
  #endif
}

void mWiFi::WifiBeginAfterScan()
{
  // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "WifiBeginAfterScan"));
  
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("F::%s"),__FUNCTION__);

  static int8_t best_network_db;

  // Not active
  if (0 == wifi_scan_state) { 
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "(0 == wifi_scan_state)"));
    //return;
    wifi_scan_state = 2;   
  }
  // Init scan when not connected
  if (1 == wifi_scan_state) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "(1 == wifi_scan_state) Init scan when not connected"));
    memset((void*) &wifi_bssid, 0, sizeof(wifi_bssid));
    best_network_db = -127;
    wifi_scan_state = 3;
  }
  // Init scan when connected
  if (2 == wifi_scan_state) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "(2 == wifi_scan_state) Init scan when connected"));
    uint8_t* bssid = WiFi.BSSID();                  // Get current bssid
    memcpy((void*) &wifi_bssid, (void*) bssid, sizeof(wifi_bssid));
    best_network_db = WiFi.RSSI();                  // Get current rssi and add threshold
    if (best_network_db < -WIFI_RSSI_THRESHOLD) { best_network_db += WIFI_RSSI_THRESHOLD; }
    wifi_scan_state = 3;
  }
  // Init scan
  if (3 == wifi_scan_state) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "(3 == wifi_scan_state) Init scan"));
    if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
      WiFi.scanNetworks(true);                      // Start wifi scan async
      wifi_scan_state++;
      AddLog_P(LOG_LEVEL_DEBUG, S_LOG_WIFI, PSTR("Network (re)scan started..."));
      return;
    }
  }
  int8_t wifi_scan_result = WiFi.scanComplete();
  // Check scan done
  if (4 == wifi_scan_state) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "(4 == wifi_scan_state) // Check scan done"));
    if (wifi_scan_result != WIFI_SCAN_RUNNING) {
      wifi_scan_state++;
    }
  }
  // Scan done
  if (5 == wifi_scan_state) {
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "(5 == wifi_scan_state) Scan done"));
    int32_t channel = 0;                            // No scan result
    int8_t ap = 3;                                  // AP default if not found
    uint8_t last_bssid[6];                          // Save last bssid
    memcpy((void*) &last_bssid, (void*) &wifi_bssid, sizeof(last_bssid));

    if (wifi_scan_result > 0) {
      // Networks found
      for (int8_t i = 0; i < wifi_scan_result; ++i) {

        String ssid_scan;
        int32_t rssi_scan;
        uint8_t sec_scan;
        uint8_t* bssid_scan;
        int32_t chan_scan;
        bool hidden_scan;

        #ifdef ESP8266
          WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);
        #endif
        bool known = false;
        uint8_t j;
        for (j = 0; j < 3; j++) {
          if (ssid_scan == pCONT_set->Settings.sta_ssid[j]) {  // SSID match
            known = true;
            if (rssi_scan > best_network_db) {      // Best network
               #ifdef ESP8266
                if (sec_scan == ENC_TYPE_NONE || pCONT_set->Settings.sta_pwd[j]) {  // Check for passphrase if not open wlan
               #else
                if (pCONT_set->Settings.sta_pwd[j]) {  // Check for passphrase if not open wlan               
               #endif
                best_network_db = (int8_t)rssi_scan;
                channel = chan_scan;
                ap = j;                             // AP1 or AP2
                memcpy((void*) &wifi_bssid, (void*) bssid_scan, sizeof(wifi_bssid));
              }
            }
            break;
          }
        }
        // AddLog_P(LOG_LEVEL_INFO, 
        //   PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, BSSId %02X:%02X:%02X:%02X:%02X:%02X, RSSI %d, Encryption %d"),
        //   i, (known) ? (j) ? '2' : '1' : '-', ssid_scan.c_str(), 
        //   chan_scan, bssid_scan[0], bssid_scan[1], bssid_scan[2], bssid_scan[3], bssid_scan[4], bssid_scan[5], 
        //   rssi_scan, 
          
        //   #ifdef ESP8266
        //     (sec_scan == ENC_TYPE_NONE) ? 0 : 1); // Check for passphrase if not open wlan
        //   #else
        //     (sec_scan == 0) ? 0 : 1);  // Check for passphrase if not open wlan               
        //   #endif   

        if(known){
          char known_c = 
                    (j==0?'1':
                    (j==1?'2':
                    (j==2?'3':
                    '-')));
          
            AddLog_P(LOG_LEVEL_INFO, 
                        PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, RSSI %d"), 
                        i, known_c, ssid_scan.c_str(), chan_scan, rssi_scan
                      );
        }
          
        delay(0);
      }
      WiFi.scanDelete();                            // Clean up Ram
      delay(0);
    }
    wifi_scan_state = 0;
    // If bssid changed then (re)connect wifi
    for (uint8_t i = 0; i < sizeof(wifi_bssid); i++) {
    
      if (last_bssid[i] != wifi_bssid[i]) {
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "last_bssid[i] != wifi_bssid[i]"));
        WifiBegin(ap, channel);                     // 0 (AP1), 1 (AP2) or 3 (default AP)
        break;
      }
    }
  }
}

uint16_t mWiFi::WifiLinkCount()
{
  return wifi_link_count;
}

// String mWiFi::WifiDowntime()
// {
//    //return GetDuration(wifi_downtime);
// }

void mWiFi::WifiSetState(uint8_t state)
{
  //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "%s=%d"),"state",state);
  if (state == pCONT_set->global_state.wifi_down) {
    if (state) {
      pCONT_set->rules_flag.wifi_connected = 1;
      wifi_link_count++;
      wifi_downtime += pCONT->mt->UpTime() - wifi_last_event;
    } else {
      pCONT_set->rules_flag.wifi_disconnected = 1;
      wifi_last_event = pCONT->mt->UpTime();
    }

  }
  // if(pCONT->mt==NULL){
  //    AddLog_P(LOG_LEVEL_DEBUG_MORE, "pCONT->mt==NULL");
  // }
  pCONT_set->global_state.wifi_down = state ^1;
}

bool mWiFi::WifiCheckIpConnected(){
  if ((WL_CONNECTED == WiFi.status()) && 
      (static_cast<uint32_t>(WiFi.localIP()) != 0) &&
      (WiFi.localIP().toString()!="(IP unset)")
  ) { 
    return true;  
  }
  return false;
}

void mWiFi::WifiCheckIp(void)
{
  //AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_DEBUG "mWiFi::WifiCheckIp"));

  if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) { //WIFI CONNECTED so assumed wifi_status is connected
    
    //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_DEBUG "WiFi.status() = WL_CONNECTED"));
    WifiSetState(1);
    wifi_counter = WIFI_CHECK_SEC; //20 secs
    wifi_retry = wifi_retry_init;

    // AddLog_P(
    //   (wifi_status != WL_CONNECTED) ? LOG_LEVEL_INFO : LOG_LEVEL_DEBUG_MORE,  //if it was not previously connected
    //   PSTR(D_LOG_WIFI D_CONNECTED));

    if (wifi_status != WL_CONNECTED) {                                        //if it was not previously connected, get router info
      
      AddLog_P(LOG_LEVEL_INFO, S_LOG_WIFI, PSTR(D_CONNECTED));
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "Set IP addresses"));
      pCONT_set->Settings.ip_address[1] = (uint32_t)WiFi.gatewayIP();
      pCONT_set->Settings.ip_address[2] = (uint32_t)WiFi.subnetMask();
      pCONT_set->Settings.ip_address[3] = (uint32_t)WiFi.dnsIP();
    }

    wifi_status = WL_CONNECTED;    //assert status to be connected
    // #ifdef USE_DISCOVERY
    #ifdef USE_NETWORK_MDNS
    #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
    //   #ifdef USE_NETWORK_MDNS
        if (2 == mdns_begun) {
          #ifdef ESP8266
          
            MDNS.update();
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS "%s"), "MDNS.update");
                  pCONT_set->boot_status.mdns_started_succesfully = mdns_begun;
          #endif
        }else{
          AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS "%s %d"), "MDNS.update NOT SET", mdns_begun);
        }
    //   #endif  // USE_DISCOVERY
    #endif  // USE_NETWORK_MDNS // #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
    #endif // #ifdef USE_NETWORK_MDNS

  } else { //not connected
    
    AddLog_P(LOG_LEVEL_INFO, PSTR("%s" " NOT connected"),__FUNCTION__);//WiFi.status() %s, IP \"%s\" %s"),GetWiFiStatusCtr(),WiFi.localIP().toString().c_str(),WiFi.localIP().toString()=="(IP unset)"?"matched":"nomatch");


    WifiSetState(0);
    uint8_t wifi_config_tool = pCONT_set->Settings.sta_config;
    wifi_status = WiFi.status();
    switch (wifi_status) {
      case WL_CONNECTED:
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_NO_IP_ADDRESS));
        wifi_status = 0;
        wifi_retry = wifi_retry_init;        
        wifi_connection_status.fConnected = true;
        break;
      case WL_NO_SSID_AVAIL:
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_NOT_REACHED));
        // if (WIFI_WAIT == pCONT_set->Settings.sta_config) {
        //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "%s"),"WIFI_WAIT == pCONT_set->Settings.sta_config");
        //   wifi_retry = wifi_retry_init;
        // } else {
          if (wifi_retry > (wifi_retry_init / 2)) {
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "%s"),"ELSE WIFI_WAIT == pCONT_set->Settings.sta_config retry>");
            wifi_retry = wifi_retry_init / 2;
          }
          else if (wifi_retry) {
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "%s"),"ELSE WIFI_WAIT == pCONT_set->Settings.sta_config retry else");
            wifi_retry = 0;
          }else{
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "%s %d %s %d"),"retry else",wifi_retry,"wifi_retry_init",wifi_retry_init);
          }
        //}
        wifi_connection_status.fConnected = false;
        wifi_connection_status.fReconnect = true;
        break;
      case WL_CONNECT_FAILED:
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_WRONG_PASSWORD));
        if (wifi_retry > (wifi_retry_init / 2)) {
          wifi_retry = wifi_retry_init / 2;
        }
        else if (wifi_retry) {
          wifi_retry = 0;
        }
        wifi_connection_status.fConnected = false;
        wifi_connection_status.fReconnect = true;
        break;
      case WL_IDLE_STATUS: AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "case WL_IDLE_STATUS"));
      case WL_DISCONNECTED: AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI "case WL_DISCONNECTED"));
      default:  // WL_IDLE_STATUS and WL_DISCONNECTED
        //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "default"));

        if (!wifi_retry || ((wifi_retry_init / 2) == wifi_retry)) {
          AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_TIMEOUT));
        } else {
          if (('\0' == pCONT_set->Settings.sta_ssid[0][0]) && ('\0' == pCONT_set->Settings.sta_ssid[1][0])) {
            wifi_config_tool = WIFI_CONFIG_NO_SSID;    // Skip empty SSIDs and start Wifi config tool
            wifi_retry = 0;
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "WIFI_CONFIG_NO_SSID"));
          } 
          else {
            AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "1"));
          }
        }

        wifi_connection_status.fConnected = false;
        wifi_connection_status.fReconnect = true;

    } // END of switch


    if (wifi_retry) {
      
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "wifi_retry %d"),wifi_retry);

      if (pCONT_set->Settings.flag_network.use_wifi_scan) {
        if (wifi_retry_init == wifi_retry) {
          wifi_scan_state = 1;    // Select scanned SSID
          AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select scanned SSID"));
        }else{
          AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "wifi_retry_init == wifi_retry %d=%d"),wifi_retry_init,wifi_retry);
        }
      } else {
        if (wifi_retry_init == wifi_retry) {
          // WifiBegin(WifiBegin_FLAG_SSID0_ID, 0);        // Select default SSID


          
          WifiBegin(WifiBegin_FLAG_TOGGLE_SSIDS_ID, 0);        // Select alternate SSID

            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select default SSID"));
        }
        if ((pCONT_set->Settings.sta_config != WIFI_WAIT) && ((wifi_retry_init / 2) == wifi_retry)) {
          WifiBegin(WifiBegin_FLAG_TOGGLE_SSIDS_ID, 0);        // Select alternate SSID
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "here Select alternate SSID"));
        }
      }
      wifi_counter = 1;
      wifi_retry--;
    } else {
      
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "wifi_retry=FALSE"));
      WifiConfig(wifi_config_tool);
      wifi_counter = 1;
      wifi_retry = wifi_retry_init;
    }
  }
}



void mWiFi::WifiCheck(uint8_t param)
{
  
  #ifdef ENABLE_WIFI_DEVELOPMENT
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("F::%s"),__FUNCTION__);
  #endif

  // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_WIFI D_JSON_COMMAND_NVALUE ", " D_JSON_COMMAND_NVALUE),"wifi_counter",wifi_counter,"param",param);
  // AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_WIFI D_JSON_COMMAND_NVALUE ", " D_JSON_COMMAND_NVALUE),"wifi_config_counter",wifi_config_counter,"wifi_counter",wifi_counter);

  // if ((WL_CONNECTED != WiFi.status()) || (static_cast<uint32_t>(WiFi.localIP()) == 0)) {
  //   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s=%d,%s=%d,%s=%d"),"wifi_+_counter",wifi_config_counter,"wifi_counter",wifi_counter,"wifi_+_state",wifi_scan_state);
  // }

  wifi_counter--;

  switch (param) {
    case WIFI_SERIAL:// AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "%s"),"WIFI_SERIAL");
    // case WIFI_SMARTCONFIG: AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "%s"),"WIFI_SMARTCONFIG");
    case WIFI_MANAGER: AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "%s"),"WIFI_MANAGER");
    // case WIFI_WPSCONFIG: AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "%s"),"WIFI_WPSCONFIG");
    //   WifiConfig(param);
    //   break;
    case WIFI_RESTART: AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "%s"),"WIFI_RESTART");
    default: //AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s"),"default");

// AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s %d"),"wifi_config_counter",wifi_config_counter);
    if (wifi_config_counter) {
      wifi_config_counter--;
      wifi_counter = wifi_config_counter +5;
      if (wifi_config_counter) {
        
        if (!wifi_config_counter) {
          if (strlen(WiFi.SSID().c_str())) {
            strlcpy(pCONT_set->Settings.sta_ssid[0], WiFi.SSID().c_str(), sizeof(pCONT_set->Settings.sta_ssid[0]));
          }
          if (strlen(WiFi.psk().c_str())) {
            strlcpy(pCONT_set->Settings.sta_pwd[0], WiFi.psk().c_str(), sizeof(pCONT_set->Settings.sta_pwd[0]));
          }
          pCONT_set->Settings.sta_active = 0;
          AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI D_WCFG_1_SMARTCONFIG D_JSON_SSID "1 %s"), pCONT_set->Settings.sta_ssid[0]);
        }

      }
      // if (!wifi_config_counter) {        
      //   pCONT_set->restart_flag = 2;
      // }
    } else {

      if (wifi_scan_state) { 
        AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_WIFI D_JSON_COMMAND_NVALUE),"wifi_scan_state",wifi_scan_state);
        WifiBeginAfterScan(); 
      }

      if (wifi_counter <= 0) {
        AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI D_CHECKING_CONNECTION "%s=%d"),"wifi_counter LESS THAN 0",wifi_counter);
        wifi_counter = WIFI_CHECK_SEC;
        WifiCheckIp();
      }

// #ifdef ENABLE_FORCED_SKIP_AP_ON_IPUNSET
//       //skip to next AP
//       if(strcmp(WiFi.localIP().toString().c_str(),"(IP unset)")==0){
//         AddLog_P(LOG_LEVEL_WARN, PSTR("Forcing new AP %s"),pCONT_set->Settings.sta_active+1);
//   //cant toggle, needs to shift between 3
//   if ('\0' == pCONT_set->Settings.sta_ssid[pCONT_set->Settings.sta_active][0]) { 
//     if(pCONT_set->Settings.sta_active++>2){ pCONT_set->Settings.sta_active = WifiBegin_FLAG_SSID0_ID; } 
//   }  // Skip empty SSID
//       }
// #endif //ENABLE_FORCED_SKIP_AP_ON_IPUNSET


      // Still connected
      if ((WL_CONNECTED == WiFi.status()) && 
          (static_cast<uint32_t>(WiFi.localIP()) != 0) 
          // && !(WiFi.localIP()!="")
          ){//} && !wifi_config_type) {

        // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s"),"Connected, Restarted, Begin services");

        WifiSetState(1);
        
        pCONT->Tasker_Interface(FUNC_WIFI_CONNECTED);

        //if (pCONT_set->Settings.flag_network.use_wifi_rescan) {
          if (!(pCONT->mt->UpTime() % (60 * WIFI_RESCAN_MINUTES))) {
            wifi_scan_state = 2;
            // AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s"),"WIFI_RESCAN_MINUTES occurred wifi_scan_state = 2");
          }
        //}

        #ifdef FIRMWARE_MINIMAL
          if (1 == RtcSettings.ota_loader) {
            RtcSettings.ota_loader = 0;
            ota_state_flag = 3;
          }
        #endif  // FIRMWARE_MINIMAL

        #ifdef USE_DISCOVERY
        #ifdef USE_NETWORK_MDNS
        // #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
         // if (pCONT_set->Settings.flag_network.mdns_enabled) {
            // if (!mdns_begun) {
            //   if (pCONT_set->mdns_delayed_start) {
            //     // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_ATTEMPTING_CONNECTION "mdns_delayed_start %d"),pCONT_set->mdns_delayed_start);
            //     pCONT_set->mdns_delayed_start--;
            //   } else {
            //     pCONT_set->mdns_delayed_start = 10;//pCONT_set->Settings.param[P_MDNS_DELAYED_START];
                
            //     // #ifdef ESP8266
            //       mdns_begun = MDNS.begin(pCONT_set->my_hostname);
            //       pCONT_set->boot_status.mdns_started_succesfully = mdns_begun;
            //     // #endif

            //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_MDNS "%s %s"), (mdns_begun) ? D_INITIALIZED : D_FAILED, pCONT_set->my_hostname);
            //   }
            // }
            // else{
            //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_ATTEMPTING_CONNECTION " already started"));
            // }
          //}
          #endif // #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
        #endif  // USE_DISCOVERY

        #ifdef USE_MODULE_CORE_WEBSERVER
          if (pCONT_set->Settings.webserver) {
            pCONT_web->StartWebserver(pCONT_set->Settings.webserver, WiFi.localIP());
            #ifdef USE_DISCOVERY
            // #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
            //   #ifdef USE_NETWORK_MDNS
            //   if (1 == mdns_begun) {
            //     mdns_begun = 2;
            //     //#ifdef ESP8266
            //       MDNS.addService("http", "tcp", WEB_PORT);
            //     //#endif
            //   }
            //   #endif  // USE_NETWORK_MDNS
            // #endif  // USE_DISCOVERY
            #endif // #ifdef ENABLE_DEVFEATURE_WIFI_MDNS
          } else {
            pCONT_web->StopWebserver();
          }
        #endif  // USE_MODULE_CORE_WEBSERVER

      } else {

        WifiSetState(0);
    
        //pCONT->Tasker_Interface(FUNC_WIFI_DISCONNECTED);

        #if defined(USE_MODULE_CORE_WEBSERVER)
          //  UdpDisconnect();
        #endif  // USE_EMULATION
        #ifdef USE_NETWORK_MDNS
        mdns_begun = 0;
        #endif // #ifdef USE_NETWORK_MDNS
      }
    } //if discovery
  }
}

int mWiFi::WifiState(void)
{
  int state = -1;
  if (!pCONT_set->global_state.wifi_down) { state = WIFI_RESTART; }
  if (wifi_config_type) { state = wifi_config_type; }
  return state;
}

void mWiFi::WifiConnect(void)
{ 
  WifiSetState(0);
  WiFi.persistent(false);     // Solve possible wifi init errors
  wifi_status = 0;
  
  #ifdef ESP8266
    wifi_retry_init = WIFI_RETRY_OFFSET_SEC + ((ESP.getChipId() & 0xF) * 2);
  #else
    wifi_retry_init = WIFI_RETRY_OFFSET_SEC + ((750325 & 0xF) * 2);
  #endif

  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_WIFI "wifi_retry_init=%d"),wifi_retry_init);

  wifi_retry = wifi_retry_init;
  wifi_counter = 1;

//  #ifdef ESP8266

 #ifdef USE_NETWORK_MDNS
  uint8_t result = MDNS.begin(pCONT_set->Settings.system_name.device); // NEEDED OR CAUSES ERROR MDNS FAILURE
  MDNS.update();
  if(result){
    AddLog_P(LOG_LEVEL_INFO,PSTR("MDNS with %s started succesfully"),pCONT_set->Settings.system_name.device);
  }else{
    AddLog_P(LOG_LEVEL_ERROR,PSTR("MDNS with %s started failed to start!"),pCONT_set->Settings.system_name.device);
  }
  #endif // ENABLE_DEVFEATURE_WIFI_MDNS
  
}

// Enable from 6.0.0a until 6.1.0a - disabled due to possible cause of bad wifi connect on core 2.3.0
// Re-enabled from 6.3.0.7 with ESP.restart replaced by ESP.reset
void mWiFi::WifiDisconnect(void)
{
  #ifdef ESP8266
  // Courtesy of EspEasy
  WiFi.persistent(true);      // use SDK storage of SSID/WPA parameters
  ETS_UART_INTR_DISABLE();
  wifi_station_disconnect();  // this will store empty ssid/wpa into sdk storage
  ETS_UART_INTR_ENABLE();
  WiFi.persistent(false);     // Do not use SDK storage of SSID/WPA parameters
  #endif
}

void mWiFi::EspRestart(void)
{
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b
  //if (Settings.flag_system.mqtt_enabled) MqttDisconnect();
  WifiDisconnect();
  //pCONT_sup->CrashDumpClear();
  ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  #ifdef ESP8266
    ESP.reset();
  #endif
}

void mWiFi::WifiAddDelayWhenDisconnected(void)
{
  // if (APP_BAUDRATE == pCONT_set->baudrate) {  // When baudrate too low it will fail on Sonoff Pow R2 and S31 serial interface initialization
  //   if (pCONT_set->global_state.wifi_down) {
  //     delay(DRIVER_BOOT_DELAY);
  //   }
  // }
}

const char* mWiFi::GetWiFiStatusCtr(void){
  switch(WiFi.status()){
    case WL_IDLE_STATUS      : return PSTR("WL_IDLE_STATUS");
    case WL_NO_SSID_AVAIL    : return PSTR("WL_NO_SSID_AVAIL");
    case WL_SCAN_COMPLETED   : return PSTR("WL_SCAN_COMPLETED");
    case WL_CONNECTED        : return PSTR("WL_CONNECTED");
    case WL_CONNECT_FAILED   : return PSTR("WL_CONNECT_FAILED");
    case WL_CONNECTION_LOST  : return PSTR("WL_CONNECTION_LOST");
    case WL_DISCONNECTED     : return PSTR("WL_DISCONNECTED");
    default: PSTR("Unknown");
  }
}

const char* mWiFi::GetWiFiConfigTypeCtr(void){
  switch(wifi_config_type){
    case WIFI_RESTART      : return PSTR("WIFI_RESTART");
    case WIFI_SMARTCONFIG    : return PSTR("WIFI_SMARTCONFIG");
    case WIFI_MANAGER   : return PSTR("WIFI_MANAGER");
    case WIFI_WPSCONFIG        : return PSTR("WIFI_WPSCONFIG");
    case WIFI_RETRY   : return PSTR("WIFI_RETRY");
    case WIFI_WAIT  : return PSTR("WIFI_WAIT");
    case WIFI_SERIAL     : return PSTR("WIFI_SERIAL");
    case WIFI_MANAGER_RESET_ONLY  : return PSTR("WIFI_MANAGER_RESET_ONLY");
    case MAX_WIFI_OPTION     : return PSTR("MAX_WIFI_OPTION");
    default: PSTR("Unknown");
  }
}





// Used for timed on or off events
int8_t mWiFi::Tasker(uint8_t function){


  switch(function){
    case FUNC_INIT:
      WifiConnect();
    break;
    case FUNC_LOOP: 
    
      // if(mSupport::TimeReached(&tSavedWiFi,10000)){
      //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "{\"" D_SSID "\":%s,\"" D_IP "\":\"%s\",\"" D_RSSI "\":%d}"),WiFi.SSID().c_str(),WiFi.localIP().toString().c_str(),WiFi.RSSI()); 
      //   // WifiBeginAfterScan();
      //   // wifi_retry = 8;
      //   // wifi_retry_init = 8;
      // }

      //check wifi
      // if(mSupport::TimeReached(&tSavedWiFiCheckIP,60000)){
      //   // WifiCheckIp();
      //   // AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_WIFI "WifiCheckIp()"));
        
      //   if (( WiFi.status() != WL_CONNECTED) || (static_cast<uint32_t>(WiFi.localIP()) == 0)) {
      //     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_WIFI "( WiFi.status() != WL_CONNECTED) || (static_cast<uint32_t>(WiFi.localIP()) == 0)"));
      //     //skip this AP, invalid IP
      //     // wifi_scan_state = 1; //init scan when no connected
      //     // wifi_retry = wifi_retry_init+5;
      //     // WifiBeginAfterScan();
      //   }else{
      //     AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "NOT ( WiFi.status() != WL_CONNECTED) || (static_cast<uint32_t>(WiFi.localIP()) == 0)"));
      //   }


      //   // if(static_cast<uint32_t>(WiFi.localIP()) == 0){
      //   //   // wifi_counter = 5; //restart
      //   //   // wifi_retry--;
      //   // }

      // }

      //if(mSupport::TimeReached(&tSavedWiFiReconnect,5000)){
      // if(!wifi_connection_status.fConnected){
      //   //AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_WIFI "wifi_connection_status.fConnected != connected"));
      //   if(wifi_connection_status.fReconnect){ wifi_connection_status.fReconnect = false;
      //     AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_WIFI "wifi_connection_status.fReconnect = true"));
      //     WifiBeginAfterScan();
      //   }
      // }

    break;
    case FUNC_EVERY_SECOND:
      // AddLog_P(LOG_LEVEL_INFO,PSTR("wifi_config_type=%s"),GetWiFiConfigTypeCtr());
    break;
  }


} // END function


//#endif