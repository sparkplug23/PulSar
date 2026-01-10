#include "mWiFi.h"


#ifdef USE_MODULE_NETWORK_WIFI
#ifdef ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2




void mWiFi::parse_JSONCommand(JsonParserObject obj){};
   



void mWiFi::init(void){
  
//  AddLog(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::init");

}

// void mWiFi::Handle_WiFiConnection(void)
// {
//   // Called each second by Tasker when Wifi2 is enabled.
//   // Keep behaviour stable: no portal-on-failure, only portal-on-unconfigured.

//   if (!WiFi2_HasAnyStaProfileConfigured())
//   {
//     // Ensure AP mode is up only in the unconfigured case.
//     WiFi2_Ap_EnsureStarted();
//     return;
//   }

//   // STA maintenance
//   wl_status_t st = WiFi.status();

//   if (st == WL_CONNECTED)
//   {
//     // IP / routability check can remain legacy
//     if (WiFi_Link_IsIpRoutable())
//     {
//       if (!connection.fConnected) { WiFi2_Sta_Connected_Enter(); }
//     }
//     else
//     {
//       // Connected but no routable IP; treat as disconnected and retry
//       if (connection.fConnected) { WiFi2_Sta_Disconnected_Enter(); }
//       WiFi2_Sta_EnsureConnecting();
//     }
//   }
//   else
//   {
//     if (connection.fConnected) { WiFi2_Sta_Disconnected_Enter(); }

//     // Backoff/retry timing using your existing counters:
//     // - Use connection.counter as “seconds until next attempt”
//     if (connection.counter > 0) { connection.counter--; return; }

//     // Re-arm retry window (you already have retry_init logic in legacy)
//     if (connection.retry_init == 0) {
//       connection.retry_init = WIFI_RETRY_OFFSET_SEC + ((mSupportHardware::ESP_getChipId() & 0xF) * 2);
//     }
//     connection.counter = WIFI_CHECK_SEC;

//     WiFi2_Sta_EnsureConnecting();
//   }
// }



// void mWiFi::WiFi_Config_Mode_Set(uint8_t type)
// {

//   ALOG_INF(PSTR(D_LOG_DEBUG "mWiFi::WiFi_Config_Mode_Set=%s"),GetWiFiConfigTypeCtr());

//   if (!connection.config_type) {
    
//     ALOG_INF(PSTR("!connection.config_type"));
    
//     if ((WIFI_RETRY == type) || (WIFI_WAIT == type)) {
//       ALOG_INF(PSTR("return"));
//       return; 
//     }
    
//     WiFi.disconnect();                       // Solve possible Wifi hangs
//     connection.config_type = type;

//     #ifndef USE_MODULE_NETWORK_WEBSERVER
//     if (WIFI_MANAGER == connection.config_type) { 
//       connection.config_type = WIFI_SERIAL; 
//     }
//     #endif
    
//     connection.config_counter = WIFI_CONFIG_SEC;   // Allow up to WIFI_CONFIG_SECS seconds for phone to provide ssid/pswd
//     connection.counter = connection.config_counter +5;
    
//     if (WIFI_RESTART == connection.config_type) {// connection.reconnects_counter++;
    
//     }

//     else if (WIFI_SERIAL == connection.config_type) {
      
//       ALOG_INF(S_LOG_WIFI, PSTR(D_WCFG_6_SERIAL " " D_ACTIVE_FOR_3_MINUTES));
      
//     }
//     // #ifdef USE_MODULE_NETWORK_WEBSERVER
//     // else if (WIFI_MANAGER == connection.config_type || WIFI_MANAGER_RESET_ONLY == connection.config_type) {
    
//     //  ALOG_INF(S_LOG_WIFI, PSTR(D_WCFG_2_WIFIMANAGER " " D_ACTIVE_FOR_3_MINUTES));
    
//     //  tkr_web->WifiManagerBegin(WIFI_MANAGER_RESET_ONLY == connection.config_type);
//     // }
//     // #endif  // USE_MODULE_NETWORK_WEBSERVER
//   }else{
//     ALOG_INF(PSTR("else connection.config_type"));
//   }

// }



// void mWiFi::WiFi_Sta_Begin_LegacyTasmota(uint8_t flag, uint8_t channel)
// {


//   ALOG_HGL(PSTR("mWiFi::WiFi_Sta_Begin_LegacyTasmota %d:%d"), flag, channel);

//   // delay(2000);

//   // DEBUG_LINE_HERE_PAUSE;
//   tkr->Tasker_Interface(TASK_WIFI_STARTING_CONNECTION);

//   ALOG_INF(PSTR(D_LOG_WIFI "mWiFi::WiFi_Sta_Begin_LegacyTasmota TASK_WIFI_STARTING_CONNECTION over")); Serial.flush();

//   const char kWifiPhyMode[] = " BGN";

// // #ifdef ARDUINO_ESP8266_RELEASE_2_3_0  // (!strncmp_P(ESP.getSdkVersion(),PSTR("1.5.3"),5))
// //   ALOG_DBG(S_LOG_WIFI, PSTR(D_PATCH_ISSUE_2186));
// //   WiFi.mode(WIFI_OFF);      // See https://github.com/esp8266/Arduino/issues/2186
// // #endif

//   WiFi.persistent(false);   // Solve possible wifi init errors (re-add at 6.2.1.16 #4044, #4083)
//   WiFi.disconnect(true);    // Delete SDK wifi config
//   delay(200);
//   WiFi.mode(WIFI_STA);      // Disable AP mode
  
// //   #ifdef ESP8266
// //  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11N) { WiFi.setPhyMode(WIFI_PHY_MODE_11N); }  // B/G/N
// //  if (WiFi.getPhyMode() != WIFI_PHY_MODE_11G) { WiFi.setPhyMode(WIFI_PHY_MODE_11G); }  // B/G
// //  #endif
//   if (!WiFi.getAutoConnect()) { WiFi.setAutoConnect(true); }
//   // WiFi.setAutoReconnect(true);

// //  SetSSIDofAPwithIndex();
//   switch (flag) {
//     default:
//     case 0:  // AP1
//     case 1:  // AP2
//       tkr_set->Settings.network.sta_active = flag;
//     break;
//     case 2:  // Toggle
//       tkr_set->Settings.network.sta_active ^= 1;
//     break;
//   }        // 3: Current AP

//   if (!strlen(tkr_set->SettingsText(SET_STASSID1 + tkr_set->Settings.network.sta_active))) {
//     tkr_set->Settings.network.sta_active ^= 1;  // Skip empty SSID
//   }






//   // Set static IP  // Set static IP
//   // if (tkr_set->Settings.network.ip_address[0]) {
//   //   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI "Settings.network.ip_address=%s"),"true");
//   //   WiFi.config(tkr_set->Settings.network.ip_address[0], 
//   //               tkr_set->Settings.network.ip_address[1], 
//   //               tkr_set->Settings.network.ip_address[2], 
//   //               tkr_set->Settings.network.ip_address[3]
//   //               );  // Set static IP
//   // }else{
//   //   AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI "Settings.network.ip_address=%s"),"false");
//   // }
  
//   #ifdef ESP8266
//     WiFi.hostname(tkr_set->runtime.my_hostname);   // ESP8266 needs this here (after WiFi.mode)
//   #endif
  
//     AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_ssid[%d]=%s"), tkr_set->Settings.network.sta_active, tkr_set->SettingsText(SET_STASSID1 + tkr_set->Settings.network.sta_active));
//     AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_pwd[%d]=%s"), tkr_set->Settings.network.sta_active, tkr_set->SettingsText(SET_STAPWD1 + tkr_set->Settings.network.sta_active));
  
//   if (channel) {
//     WiFi.begin(
//       tkr_set->SettingsText(SET_STASSID1 + tkr_set->Settings.network.sta_active), 
//       tkr_set->SettingsText(SET_STAPWD1 + tkr_set->Settings.network.sta_active), 
//       channel, 
//       connection.bssid
//     );


//     // Add connected BSSID and channel for multi-AP installations
//     // char hex_char[18];
//     // snprintf_P(stemp, sizeof(stemp), PSTR(" Channel %d BSSId %s"), channel, ToHex_P((unsigned char*)Wifi.bssid, 6, hex_char, sizeof(hex_char), ':'));
  
//   } else {
    
//     WiFi.begin(tkr_set->SettingsText(SET_STASSID1 + tkr_set->Settings.network.sta_active), tkr_set->SettingsText(SET_STAPWD1 + tkr_set->Settings.network.sta_active));
//   }

//   #ifdef ENABLE_FEATURE_WIFI__SET_TXPOWER
//   WiFi.setTxPower(WIFI_POWER_8_5dBm);
//   ALOG_INF(PSTR(D_LOG_WIFI "TxPower=%d"), WiFi.getTxPower()); // to fix ESP32C3 antenna issues
//   #endif
  
//   #ifdef ESP8266
//   ALOG_INF(PSTR(D_LOG_WIFI D_CONNECTING_TO_AP "%d \"%s\" \"%s\" " D_IN_MODE " 11%c " D_AS " %s"),
//     tkr_set->Settings.network.sta_active +1, 
//     tkr_set->SettingsText(SET_STASSID1 + tkr_set->Settings.network.sta_active), 
//     tkr_set->SettingsText(SET_STAPWD1 + tkr_set->Settings.network.sta_active), 
//     kWifiPhyMode[WiFi.getPhyMode() & 0x3], 
//     tkr_set->runtime.my_hostname);
//   #endif

// }


// void mWiFi::WiFi_Sta_Scan_SelectBest_AndBegin()
// {

//   uint8_t log = LOG_LEVEL_INFO;

//   AddLog(log, PSTR(D_LOG_WIFI "WiFi_Sta_Scan_SelectBest_AndBegin"));

//   static int8_t best_network_db;

//   // Not active
//   if (0 == connection.scan_state) {

//     ALOG_INF(PSTR(D_LOG_WIFI "(0 == connection.scan_state)"));
    
//     return;
//   }
//   // Init scan when not connected
//   if (1 == connection.scan_state) {
    
//     ALOG_INF(PSTR(D_LOG_WIFI "(1 == connection.scan_state) Init scan when not connected"));
    
//     memset((void*) &connection.bssid, 0, sizeof(connection.bssid));
//     best_network_db = -127;
//     connection.scan_state = 3;
//   }
//   // Init scan when connected
//   if (2 == connection.scan_state) {
    
//     ALOG_INF(PSTR(D_LOG_WIFI "(2 == connection.scan_state) Init scan when connected"));
    
//     uint8_t* bssid = WiFi.BSSID();                  // Get current bssid
//     memcpy((void*) &connection.bssid, (void*) bssid, sizeof(connection.bssid));
//     best_network_db = WiFi.RSSI();                  // Get current rssi and add threshold
//     if (best_network_db < -WIFI_RSSI_THRESHOLD) { best_network_db += WIFI_RSSI_THRESHOLD; }
//     connection.scan_state = 3;
//   }

  
//   if(WiFi.scanComplete() == WIFI_SCAN_RUNNING){
//     ALOG_INF(PSTR("WiFi.scanComplete() RUNNING"));
//     ALOG_INF(PSTR("WiFi.IP() %s"),WiFi.localIP().toString().c_str());
//   }


//   // Init scan
//   if (3 == connection.scan_state) {
    
//     ALOG_INF(PSTR(D_LOG_WIFI "(3 == connection.scan_state) Init scan"));
    
//     if (WiFi.scanComplete() != WIFI_SCAN_RUNNING) {
//       WiFi.scanNetworks(true);                      // Start wifi scan async
//       connection.scan_state++;
//       ALOG_INF(PSTR("Network (re)scan started..."));
//       return;
//     }
//     // else{
//     //   ALOG_INF(PSTR("WiFi.scanComplete() %d"),WiFi.scanComplete());
//     // }
//   }


//   int8_t scan_result = WiFi.scanComplete();
  
//   ALOG_INF(PSTR(D_LOG_WIFI "scan_result=%d"),scan_result);
  
//   // Check scan done
//   if (4 == connection.scan_state) {
//     ALOG_INF(PSTR(D_LOG_WIFI "(4 == scan_state) // Check scan done"));
//     if (scan_result != WIFI_SCAN_RUNNING) {
//       connection.scan_state++;
//     }
//   }
//   // Scan done
//   if (5 == connection.scan_state) {
//     ALOG_INF(PSTR(D_LOG_WIFI "(5 == scan_state) Scan done"));
//     int32_t channel = 0;                            // No scan result
//     int8_t ap = 3;                                  // AP default if not found
//     uint8_t last_bssid[6];                          // Save last bssid
//     memcpy((void*) &last_bssid, (void*) &connection.bssid, sizeof(last_bssid));

//     if (scan_result > 0) {
//       // Networks found
//       for (int8_t i = 0; i < scan_result; ++i) {

//         String ssid_scan;
//         int32_t rssi_scan;
//         uint8_t sec_scan;
//         uint8_t* bssid_scan;
//         int32_t chan_scan;
//         bool hidden_scan;
        
//         // WiFiHelper::getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);
//         #ifdef ESP8266
//           WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan, hidden_scan);
//         #else
//           WiFi.getNetworkInfo(i, ssid_scan, sec_scan, rssi_scan, bssid_scan, chan_scan);
//         #endif
        
//         bool known = false;
//         uint8_t j;
//         // DEBUG_LINE_HERE;
//         for (j = 0; j < MAX_SSIDS; j++) {
//           // DEBUG_LINE_HERE;

//           // if (ssid_scan == tkr_set->Settings.sta_ssid[j]) {  // SSID match
//           if (ssid_scan == tkr_set->SettingsText(SET_STASSID1 + j)) {  // SSID match



//           // DEBUG_LINE_HERE;
//           // #ifdef 
//           //   ALOG_TST(PSTR("%s"),ssid_scan);

//             known = true;
//             if (rssi_scan > best_network_db) {      // Best network
              
//             ALOG_TST(PSTR("rssi_scan > best_network_db"));
            
//                #ifdef ESP8266
//                 if (sec_scan == ENC_TYPE_NONE || tkr_set->SettingsText(SET_STAPWD1 + j)) {  // Check for passphrase if not open wlan
//                #else
//                 if (tkr_set->SettingsText(SET_STAPWD1 + j)) {  // Check for passphrase if not open wlan               
//                #endif
//                 best_network_db = (int8_t)rssi_scan;
//                 channel = chan_scan;
//                 ap = j;                             // AP1 or AP2
//                 memcpy((void*) &connection.bssid, (void*) bssid_scan, sizeof(connection.bssid));

                
//             AddLog_Array(log, "break", connection.bssid, (uint8_t)6);

            
//               }
//             }
//             break;
//           }else{
          
//             // ALOG_TST(PSTR("ssid_scan[\"%s\"] != tkr_set->Settings.sta_ssid[%d] \"%s\""),ssid_scan,j,tkr_set->Settings.sta_ssid[j]);

//           }
//         }
//         // ALOG_INF(
//         //   PSTR(D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, BSSId %02X:%02X:%02X:%02X:%02X:%02X, RSSI %d, Encryption %d"),
//         //   i, (known) ? (j) ? '2' : '1' : '-', ssid_scan.c_str(), 
//         //   chan_scan, bssid_scan[0], bssid_scan[1], bssid_scan[2], bssid_scan[3], bssid_scan[4], bssid_scan[5], 
//         //   rssi_scan, 
          
//         //   #ifdef ESP8266
//         //     (sec_scan == ENC_TYPE_NONE) ? 0 : 1); // Check for passphrase if not open wlan
//         //   #else
//         //     (sec_scan == 0) ? 0 : 1);  // Check for passphrase if not open wlan               
//         //   #endif   

//         if(known){
//           char known_c = 
//                     (j==0?'1':
//                     (j==1?'2':
//                     (j==2?'3':
//                     '-')));
          
//           AddLog(log, PSTR(DEBUG_INSERT_PAGE_BREAK D_LOG_WIFI "Network %d, AP%c, SSId %s, Channel %d, RSSI %d"), 
//                         i, known_c, ssid_scan.c_str(), chan_scan, rssi_scan);
//         }
          
//         delay(0);
//       }
//       WiFi.scanDelete();                            // Clean up Ram
//       delay(0);
//     }
//     // DEBUG_LINE_HERE;
//     connection.scan_state = 0;
//     // If bssid changed then (re)connect wifi
//     for (uint8_t i = 0; i < sizeof(connection.bssid); i++) 
//     {    
//       if (last_bssid[i] != connection.bssid[i]) 
//       {
//         AddLog(log, PSTR(D_LOG_WIFI "last_bssid[i] != connection.bssid[i]"));
//         WiFi_Sta_Begin_LegacyTasmota(ap, channel);                     // 0 (AP1), 1 (AP2) or 3 (default AP)
//         break;
//       }
//       else
//       {
//         AddLog(log, PSTR(D_LOG_WIFI "last_bssid[i] ================= connection.bssid[i]"));
//       }
//     }
//   }
// }









// uint16_t mWiFi::WiFi_Link_ReconnectCount_Get()
// {
//   return connection.link_count;
// }

// String mWiFi::WifiDowntime()
// {
//    //return GetDuration(connection.downtime);
// }

// checked


// /**
//  * @brief Causing issues with neopixel timing
//  * 
//  */
// void mWiFi::WiFi_Link_CheckIp_AndHandleTransitions(void)
// {
//   AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI D_CHECKING_CONNECTION));

//   AddLog(loglevel_with_connection_status, PSTR(D_LOG_DEBUG "mWiFi::WiFi_Link_CheckIp_AndHandleTransitions"));

// // #if defined(LWIP_IPV6) && defined(ESP8266)
// //   if(WifiCheckIPAddrStatus()) {
// //     Wifi.status = WL_CONNECTED;
// // #else

// // bool test = (WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()));


//   if ((WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0)) 
//   {



// // #endif  // LWIP_IPV6=1

//     AddLog(loglevel_with_connection_status,  PSTR("WL_CONNECTED %s"), WiFi.localIP().toString().c_str() );

//     WiFi_Sta_State_Set(1);
//     connection.counter = WIFI_CHECK_SEC; //20 secs
//     connection.retry = connection.retry_init;

//     // AddLog(
//     //   (connection.status != WL_CONNECTED) ? LOG_LEVEL_INFO : LOG_LEVEL_DEBUG_MORE,  //if it was not previously connected
//     //   PSTR(D_LOG_WIFI D_CONNECTED));

//     if (connection.status != WL_CONNECTED) {                                        //if it was not previously connected, get router info
      
//       ALOG_INF(S_LOG_WIFI, PSTR(D_CONNECTED));
//       // ALOG_INF(PSTR(D_LOG_WIFI "Set IP addresses"));
//       tkr_set->Settings.network.ip_address[1] = (uint32_t)WiFi.gatewayIP();
//       tkr_set->Settings.network.ip_address[2] = (uint32_t)WiFi.subnetMask();
//       tkr_set->Settings.network.ip_address[3] = (uint32_t)WiFi.dnsIP();
      
//       // Save current AP parameters for quick reconnect
//       tkr_set->Settings.network.wifi_channel = WiFi.channel();
//       uint8_t *bssid = WiFi.BSSID();
//       memcpy((void*) &tkr_set->Settings.network.wifi_bssid, (void*) bssid, sizeof(tkr_set->Settings.network.wifi_bssid));
//     }

//     connection.status = WL_CONNECTED;    //assert status to be connected
    

//   } 
//   else
//   { //not connected
    
//    // ALOG_INF(PSTR("%s" " NOT connected"),__FUNCTION__);//WiFi.status() %s, IP \"%s\" %s"),GetWiFiStatusCtr(),WiFi.localIP().toString().c_str(),WiFi.localIP().toString()=="(IP unset)"?"matched":"nomatch");
 
//     WiFi_Sta_State_Set(0);DEBUG_LINE_HERE
//     uint8_t wifi_config_tool = tkr_set->Settings.network.sta_config;
//     connection.status = WiFi.status();
//     switch (connection.status) {
//       case WL_CONNECTED:
      
//         ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_NO_IP_ADDRESS));
        
//         connection.status = 0;
//         connection.retry = connection.retry_init;        
//         connection.fConnected = true;
//         break;
//       case WL_NO_SSID_AVAIL:
//         ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_NOT_REACHED));
        
//         tkr_set->Settings.network.wifi_channel = 0;  // Disable stored AP

//         if (WIFI_WAIT == tkr_set->Settings.network.sta_config) {
//           ALOG_INF(PSTR(D_LOG_WIFI "1%s"),"WIFI_WAIT == tkr_set->Settings.network.sta_config");
//           connection.retry = connection.retry_init;
//         } else {
//           if (connection.retry > (connection.retry_init / 2)) {
//             ALOG_INF(PSTR(D_LOG_WIFI "2%s"),"ELSE WIFI_WAIT == tkr_set->Settings.network.sta_config retry>");
//             connection.retry = connection.retry_init / 2;
//           }
//           else if (connection.retry) {
//             ALOG_INF(PSTR(D_LOG_WIFI "3%s"),"ELSE WIFI_WAIT == tkr_set->Settings.network.sta_config retry else");
//             connection.retry = 0;
//           }else{
//             ALOG_INF(PSTR(D_LOG_WIFI "4%s %d %s %d"),"retry else",connection.retry,"connection.retry_init",connection.retry_init);
//           }
//         }
//         connection.fConnected = false;
//         connection.fReconnect = true;
//         break;
//       case WL_CONNECT_FAILED:
      
//         ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_WRONG_PASSWORD));
        
//         tkr_set->Settings.network.wifi_channel = 0;  // Disable stored AP
//         if (connection.retry > (connection.retry_init / 2)) {
//           connection.retry = connection.retry_init / 2;
//         }
//         else if (connection.retry) {
//           connection.retry = 0;
//         }
//         connection.fConnected = false;
//         connection.fReconnect = true;
//         break;
//       // case WL_IDLE_STATUS: ALOG_INF(PSTR(D_LOG_WIFI "case WL_IDLE_STATUS"));
//       // case WL_DISCONNECTED: ALOG_DBM( PSTR(D_LOG_WIFI "case WL_DISCONNECTED"));
//       default:  // WL_IDLE_STATUS and WL_DISCONNECTED
//         //ALOG_INF(PSTR(D_LOG_WIFI "default"));

//         if (!connection.retry || ((connection.retry_init / 2) == connection.retry)) {
          
//           ALOG_INF(PSTR(D_LOG_WIFI D_CONNECT_FAILED_AP_TIMEOUT));
          
//           tkr_set->Settings.network.wifi_channel = 0;  // Disable stored AP
//         } else {
//           #if __cplusplus >= 201703L  // C++17 or newer
//             if ((tkr_set->SettingsText(SET_STASSID1)[0] == '\0') &&
//                 (tkr_set->SettingsText(SET_STASSID2)[0] == '\0'))
//           #else  // C++14 or older
//             if (('\0' == tkr_set->SettingsText(SET_STASSID1)[0]) &&
//                 ('\0' == tkr_set->SettingsText(SET_STASSID2)[0]))
//           #endif
//           {
//             //          if (('\0' == tkr_set->SettingsText(SET_STASSID1)) && ('\0' == tkr_set->SettingsText(SET_STASSID2))) {
//           tkr_set->Settings.network.wifi_channel = 0;  // Disable stored AP
//             wifi_config_tool = WIFI_CONFIG_NO_SSID; // SHOULD BE WIFI_MANAGER   // Skip empty SSIDs and start Wifi config tool
//             connection.retry = 0;
//             ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "WIFI_CONFIG_NO_SSID"));
//           } 
//           else {
//             ALOG_DBM( PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "1"));
//           }
//         }

//         connection.fConnected = false;
//         connection.fReconnect = true;

//     } // END of switch


//     if (connection.retry) 
//     {
      
//       ALOG_INF(PSTR(D_LOG_WIFI "connection retry %d"), connection.retry_init - connection.retry);
      
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "sta_ssid[%d]=%s"), tkr_set->Settings.network.sta_active, tkr_set->SettingsText(SET_STASSID1 + tkr_set->Settings.network.sta_active));
//       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_WIFI "sta_pwd[%d]=%s"), tkr_set->Settings.network.sta_active, tkr_set->SettingsText(SET_STAPWD1 + tkr_set->Settings.network.sta_active));

//       if (tkr_set->Settings.network.flag.use_wifi_scan) 
//       {
//         if (connection.retry_init == connection.retry) 
//         {
//           connection.scan_state = 1;    // Select scanned SSID
//           ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select scanned SSID"));          
//         }
//         else
//         {
//           ALOG_INF(PSTR(D_LOG_WIFI "connection.retry_init == connection.retry %d=%d"), connection.retry_init, connection.retry);
//         }
//       } else {
//         if (connection.retry_init == connection.retry) 
//         {
          
//           WiFi_Sta_Begin_LegacyTasmota(WIFIBEGIN_FLAG_TOGGLE_SSIDS_ID, tkr_set->Settings.network.wifi_channel);        // Select alternate SSID
//           ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select default SSID"));
          
//         }
//         if ((tkr_set->Settings.network.sta_config != WIFI_WAIT) && ((connection.retry_init / 2) == connection.retry)) 
//         {

//           WiFi_Sta_Begin_LegacyTasmota(WIFIBEGIN_FLAG_TOGGLE_SSIDS_ID, 0);        // Select alternate SSID
//           ALOG_INF(PSTR(D_LOG_WIFI D_ATTEMPTING_CONNECTION "Select alternate SSID"));
          
//         }
//       }

//       /*
//       if (Settings.flag3.use_connection.scan) {  // SetOption56 - Scan wifi network at restart for configured AP's
//         if (Wifi.retry_init == Wifi.retry) {
//           Wifi.scan_state = 1;    // Select scanned SSID
//         }
//       } else {
//         if (Wifi.retry_init == Wifi.retry) {
//           WiFi_Sta_Begin_LegacyTasmota(3, Settings.connection.channel);  // Select default SSID
//         }
//         if ((Settings.network.sta_config != WIFI_WAIT) && ((Wifi.retry_init / 2) == Wifi.retry)) {
//           WiFi_Sta_Begin_LegacyTasmota(2, 0);        // Select alternate SSID
//         }
//       }*/
//       connection.counter = 1;
//       connection.retry--;
//     } 
//     else 
//     {
      
//       ALOG_INF(PSTR(D_LOG_WIFI "wifi_retry=FALSE"));
    
//       WiFi_Config_Mode_Set(wifi_config_tool);
//       connection.counter = 1;
//       connection.retry = connection.retry_init;
//     }
//   }

//   //ALOG_INF(PSTR(D_LOG_DEBUG "%s"),"WiFi_Sta_State_Set checkip END");


// }


// Primary health checker
// void mWiFi::WiFi_Sta_Maintain_Periodic(uint8_t param)
// {

//   AddLog(loglevel_with_connection_status, PSTR("Delay below"));

  
//   #ifdef ENABLE_WIFI_DEVELOPMENT
//   ALOG_DBG(PSTR("F::%s"),__FUNCTION__);
//   #endif

//   // ALOG_TST(PSTR("connection.config_counter=%d"),connection.config_counter);


//   // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI D_COMMAND_NVALUE ", " D_COMMAND_NVALUE),"connection.counter",connection.counter,"param",param);
//   // AddLog(LOG_LEVEL_DEV_TEST,PSTR(D_LOG_WIFI D_COMMAND_NVALUE ", " D_COMMAND_NVALUE),"config_counter",connection.config_counter,"counter",connection.counter);

//   // if ((WL_CONNECTED != WiFi.status()) || (static_cast<uint32_t>(WiFi.localIP()) == 0)) {
//   //   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s=%d,%s=%d,%s=%d"),"connection.+_counter",connection.config_counter,"connection.counter",connection.counter,"connection.+_state",connection.scan_state);
//   // }

//   connection.counter--;

//   switch (param) {
//     case WIFI_SERIAL:
//     case WIFI_MANAGER:
//       WiFi_Config_Mode_Set(param);
//       break;
//     // case WIFI_RESTART:
//     default: 
//       /**
//        * config_counter updates config
//        * */
//       if (connection.config_counter) 
//       {

//         ALOG_INF( PSTR(D_LOG_WIFI "WiFi_Sta_Maintain_Periodic " "config_counter=%d"), connection.config_counter);
    
//         connection.config_counter--;
//         connection.counter = connection.config_counter +5;
//     //     if (connection.config_counter) {
          
//     //       // If we reach ZERO, then retry connection on default
//     //       if (!connection.config_counter) {
//     //         if (strlen(WiFi.SSID().c_str())) {
//     //           strlcpy(tkr_set->Settings.sta_ssid[0], WiFi.SSID().c_str(), sizeof(tkr_set->Settings.sta_ssid[0]));
//     //         }
//     //         if (strlen(WiFi.psk().c_str())) {
//     //           strlcpy(tkr_set->Settings.sta_pwd[0], WiFi.psk().c_str(), sizeof(tkr_set->Settings.sta_pwd[0]));
//     //         }
//     //         tkr_set->Settings.network.sta_active = 0;
//     //         ALOG_INF(PSTR(D_LOG_WIFI D_SSID "Retrying original config \"%s\""), tkr_set->Settings.sta_ssid[0]);
//     //       }

//     //     }
//         // Delayed by the above code by 5 seconds
//         if (!connection.config_counter) 
//         {
//           // tkr_set->restart_flag = 2;
//           ALOG_INF(PSTR(D_LOG_WIFI "WiFi_Sta_Maintain_Periodic " "restart_flag = 2"));
//         }

//       } else {

//         // DEBUG_LINE_HERE;

//         if (connection.scan_state) {
//           AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_WIFI D_COMMAND_NVALUE),"scan_state",connection.scan_state);
//           WiFi_Sta_Scan_SelectBest_AndBegin(); 
//         }

//         if (connection.counter <= 0) {
//           // ALOG_INF(PSTR(D_LOG_WIFI D_CHECKING_CONNECTION));
          
//           connection.counter = WIFI_CHECK_SEC;
          
//           WiFi_Link_CheckIp_AndHandleTransitions();
//         }

// // #ifdef ENABLE_FORCED_SKIP_AP_ON_IPUNSET
// //       //skip to next AP
// //       if(strcmp(WiFi.localIP().toString().c_str(),"(IP unset)")==0){
// //         AddLog(LOG_LEVEL_WARN, PSTR("Forcing new AP %s"),tkr_set->Settings.network.sta_active+1);
// //   //cant toggle, needs to shift between 3
// //   if ('\0' == tkr_set->Settings.sta_ssid[tkr_set->Settings.network.sta_active][0]) { 
// //     if(tkr_set->Settings.network.sta_active++>2){ tkr_set->Settings.network.sta_active = WIFIBEGIN_FLAG_SSID0_ID; } 
// //   }  // Skip empty SSID
// //       }
// // #endif //ENABLE_FORCED_SKIP_AP_ON_IPUNSET


//       // Still connected
//       // if (WiFi_Link_IsIpRoutable() && !connection.config_type){
          
//       //   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s"),"Connected, Restarted, Begin services");

//       //   WiFi_Sta_State_Set(1);
        
//       //   tkr->Tasker_Interface(TASK_WIFI_CONNECTED);

//       //   //if (tkr_set->Settings.network.flag.use_wifi_rescan) {
//       //     if (!(tkr_time->UpTime() % (60 * WIFI_RESCAN_MINUTES))) {
//       //       connection.scan_state = 2;
//       //       // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_WIFI "%s"),"WIFI_RESCAN_MINUTES occurred connection.scan_state = 2");
//       //     }
//       //   //}


//     // #if LWIP_IPV6
//     //       if (WifiCheckIPAddrStatus()) {
//     // #else

//           // bool is_connected = tkr_time->UpTime()>20;//(WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !connection.config_type;

//           bool is_connected = (WL_CONNECTED == WiFi.status()) && (static_cast<uint32_t>(WiFi.localIP()) != 0) && !connection.config_type;


//           if (is_connected) 
//           {
//         // DEBUG_LINE_HERE;
//     //#endif /// LWIP_IPV6=1
    
//         // DEBUG_PRINTF( " if ((WL_CONNECTED == WiFi.status())\n\r");

//         //resting state, connected and healthy


//             WiFi_Sta_State_Set(1);
            
//             if (tkr_set->Settings.network.flag.use_wifi_rescan) {  // SetOption57 - Scan wifi network every 44 minutes for configured AP's
//               if (!(tkr_time->UpTime() % (60 * WIFI_RESCAN_MINUTES))) {
//                 connection.scan_state = 2;
//               }
//             }
            
            
//             // #ifdef USE_MODULE_NETWORK_WEBSERVER
//             //   if (tkr_set->Settings.webserver) {
//             //     tkr_web->StartWebserver(tkr_set->Settings.webserver, WiFi.localIP());
//             //   } else {
//             //     tkr_web->StopWebserver();
//             //   }
//             // #endif  // USE_MODULE_NETWORK_WEBSERVER


//           } 
//           else 
//           {
//             DEBUG_LINE_HERE
//             // Serial.println( " ELSE if ((WL_CONNECTED == WiFi.status())\n\r");
//             WiFi_Sta_State_Set(0);
//             Mdns.begun = 0;
//           }
//         }//END else


//       // } else {

//       //   WiFi_Sta_State_Set(0);
    
//       //   //tkr->Tasker_Interface(TASK_WIFI_DISCONNECTED);

//       // }
//     //} //if discovery
//   }
// }

// int mWiFi::WiFi_Sta_State_Get(void)
// {
//   int state = -1;
//   if (!tkr_set->runtime.global_state.wifi_down) { state = WIFI_RESTART; }
//   if (connection.config_type) { state = connection.config_type; }
//   return state;
// }








void mWiFi::WiFi_Radio_Shutdown(bool option)
{
  // option = false - Legacy disconnect also used by DeepSleep
  // option = true  - Disconnect with SDK wifi calibrate sector erase when WIFI_FORCE_RF_CAL_ERASE enabled
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b

// #ifdef USE_EMULATION
//   UdpDisconnect();
//   delay(100);                 // Flush anything in the network buffers.
// #endif  // USE_EMULATION

  // if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
  //   MqttDisconnect();
  //   delay(100);               // Flush anything in the network buffers.
  // }

// #ifdef WIFI_FORCE_RF_CAL_ERASE
//   if (option) {
//     WiFi.disconnect(false);   // Disconnect wifi
//     SettingsErase(4);         // Delete SDK wifi config and calibrate data
//   } else
// #endif  // WIFI_FORCE_RF_CAL_ERASE
  // {
    // Enable from 6.0.0a until 6.1.0a - disabled due to possible cause of bad wifi connect on core 2.3.0
    // Re-enabled from 6.3.0.7 with ESP.restart replaced by ESP.reset
    // Courtesy of EspEasy
    // WiFi.persistent(true);    // use SDK storage of SSID/WPA parameters
    ETS_UART_INTR_DISABLE();

  //tmp fix
#ifdef ESP8266
    wifi_station_disconnect();  // this will store empty ssid/wpa into sdk storage
    #else

    // erase ap: empty ssid, ...
    WiFi.disconnect(true, true); //inside compat
    #endif
    ETS_UART_INTR_ENABLE();
    // WiFi.persistent(false);   // Do not use SDK storage of SSID/WPA parameters
  // }
  delay(100);                 // Flush anything in the network buffers.

}









/***************************************************************************************************************************
 **********************************************************************************************************************************
 *  HELPER FUNCTIONS
 **********************************************************************************************************************************
 *******************************************************************************************************************************/




// int mWiFi::WifiGetRssiAsQuality(int rssi)
// {
//   int quality = 0;

//   if (rssi <= -100) {
//     quality = 0;
//   } else if (rssi >= -50) {
//     quality = 100;
//   } else {
//     quality = 2 * (rssi + 100);
//   }
//   return quality;
// }

// bool mWiFi::WiFi_Config_ConnectWindow_Expired(void)
// {
//   if (connection.config_counter) {
//     connection.config_counter = WIFI_CONFIG_SEC;
//   }
//   return connection.config_counter;
// }


/*********************************************************************************************\
 * MDNS
\*********************************************************************************************/

#ifdef USE_DISCOVERY

void mWiFi::WiFi_Mdns_StartOrRestart(void)
{
  if (Mdns.begun) return;

  const char* originalName = tkr_set->Settings.system_name.device;

  // Define a temporary buffer (max 64 bytes: 63 + null terminator)
  char hostname[64];
  strncpy(hostname, originalName, 63);
  hostname[63] = '\0';

  // Replace invalid underscores with dashes
  for (char* p = hostname; *p; ++p)
  {
    if (*p == '_') *p = '-';
  }

  // Platform-specific hostname setter
  #if defined(ESP8266)
    WiFi.hostname(hostname);
  #elif defined(ESP32)
    WiFi.setHostname(hostname);
  #endif

  // OTA hostname
  ArduinoOTA.setHostname(hostname);

  // Close existing session to prevent failure
  MDNS.end();

  // Begin with sanitized hostname
  Mdns.begun = (uint8_t)MDNS.begin(hostname);

  ALOG_INF(PSTR(D_LOG_MDNS "%s with %s"),
           (Mdns.begun) ? PSTR(D_INITIALIZED) : PSTR(D_FAILED),
           hostname);

  #if defined(ESP32)
    // Register service
    MDNS.addService("_http", "_tcp", 80);

    String escapedMac = WiFi.macAddress();
    escapedMac.replace(":", "");
    escapedMac.toLowerCase();

    MDNS.addService("http", "tcp", 80);
    MDNS.addService("pulsar", "tcp", 80);
    MDNS.addServiceTxt("pulsar", "tcp", "mac", escapedMac.c_str());
  #endif
}






#ifdef MQTT_HOST_DISCOVERY
void mWiFi::WiFi_Mdns_DiscoverMqttBroker(void)
{
  if (!Mdns.begun) { return; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  ALOG_INF( PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);

  if (n > 0) {
    uint32_t i = 0;            // If the hostname isn't set, use the first record found.
    #ifdef MDNS_HOSTNAME
    for (i = n; i > 0; i--) {  // Search from last to first and use first if not found
      if (!strcmp(MDNS.hostname(i).c_str(), MDNS_HOSTNAME)) {
        break;                 // Stop at matching record
      }
    }
    #endif  // MDNS_HOSTNAME
    // SettingsUpdateText(SET_MQTT_HOST, MDNS.hostname(i).c_str());
    // Settings.mqtt_port = MDNS.port(i);
    // ALOG_INF(PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s," D_PORT " %d"), SettingsText(SET_MQTT_HOST), Settings.mqtt_port);
  }
}
#endif  // MQTT_HOST_DISCOVERY

#ifdef WEBSERVER_HOST_DISCOVERY
void mWiFi::WiFi_Mdns_AdvertiseHttpService(void) {
  if (1 == Mdns.begun) {
    Mdns.begun = 2;
    MDNS.addService("http", "tcp", WEB_PORT);
    MDNS.addServiceTxt("http", "tcp", "devicetype", "tasmota");
  }
}
#endif  // WEBSERVER_HOST_DISCOVERY
#endif  // USE_DISCOVERY



#if defined(USE_NETWORK_MDNS) && defined(ESP8266) //Not needed with esp32 mdns
void mWiFi::WiFi_Mdns_Tick(void) 
{
  MDNS.update();
  if (2 == Mdns.begun) {
    MDNS.update(); // this is basically passpacket like a webserver
   // ALOG_DBM( PSTR(D_LOG_MDNS "MDNS.update"));
  }
}
#endif  // ESP8266





#endif
#endif