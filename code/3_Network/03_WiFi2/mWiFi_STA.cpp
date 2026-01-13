#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI

void mWiFi::WiFi_Sta_Maintain_Periodic(void)
{
//   ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"), __FILE__, __LINE__);

  const bool connected = (WiFi.status() == WL_CONNECTED);

  // Good link if connected AND routable
  if (connected && WiFi_Link_IsIpRoutable())
  {
    // If previously not connected, we are now connected and should broadcast that
    if (!connection.fConnected)
    {
      WiFi2_Sta_Connected_Enter();
    }

    WiFi_Sta_OnConnected_ResetOutageScanFlags();
    return;
  }

  // If we preceed beyond this point, connection above was not establashed and we will attempt reconnects

  // Not good (either not connected, or no routable IP)
  if (connection.fConnected)
  {
    WiFi2_Sta_Disconnected_Enter();
  }

  // Maintain downtime (seconds)
  if (connection.downtime < 0xFFFFFFFFUL) { connection.downtime++; }

  // If unconfigured: do not start AP here.
  // AP start is orchestrated by Tasker using seconds_to_wait_for_fresh_connection_attempt.
  if (!WiFi2_HasAnyStaProfileConfigured())
  {
    return;
  }

  // Backoff window using your existing counter
  if (connection.counter > 0)
  {
    ALOG_INF(PSTR(D_LOG_WIFI "Reconnecting in %d seconds"),connection.counter);
    connection.counter--;
    return;
  }

  // Choose profile (scan once on boot; once per outage after threshold)
  const bool do_scan = WiFi_Sta_ShouldScanNow_OnBootOrOutage();
  const uint8_t profile_i = WiFi_Sta_SelectProfileIndex_WithScanPreference(do_scan);

  // Connect attempt (the only connect primitive)
  WiFi_Sta_ProfileIndex_Connect(profile_i);

  // Rearm attempt pacing
  connection.counter = WIFI_CHECK_SEC;
}



uint8_t mWiFi::WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured(void) const
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  for (uint8_t i = 0; i < WIFI_MAXIMUM_CONNECTIONS; i++)
  {
    if (tkr_set->Settings.network.wifi[i].ssid[0] != '\0') return i;
  }
  return 0;
}
bool mWiFi::WiFi_Sta_ShouldScanNow_OnBootOrOutage(void)
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  // Boot scan exactly once
  if (!s_wifi2_scanned_on_boot)
  {
    s_wifi2_scanned_on_boot = true;
    return true;
  }

  // Outage scan exactly once per outage, after downtime threshold
  if (!s_wifi2_scanned_on_this_outage && (connection.downtime >= WIFI_OUTAGE_RESCAN_SECONDS))
  {
    s_wifi2_scanned_on_this_outage = true;
    return true;
  }

  return false;
}

void mWiFi::WiFi_Sta_OnConnected_ResetOutageScanFlags(void)
{
//   ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  // When link is back, allow a future outage to trigger a scan again
  s_wifi2_scanned_on_this_outage = false;
  connection.downtime = 0;
}

uint8_t mWiFi::WiFi_Sta_SelectProfileIndex_WithScanPreference(bool force_scan)
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  const uint8_t ordered_first = WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured();

  if (!force_scan)
  {
    return ordered_first;
  }

  int n = WiFi.scanNetworks();
  if (n <= 0)
  {
    WiFi.scanDelete();
    return ordered_first;
  }

  // best RSSI per profile (only for configured SSIDs)
  int16_t best_rssi_by_profile[WIFI_MAXIMUM_CONNECTIONS];
  for (uint8_t p = 0; p < WIFI_MAXIMUM_CONNECTIONS; p++) best_rssi_by_profile[p] = (int16_t)-32768;

  for (int i = 0; i < n; i++)
  {
    String ssid = WiFi.SSID(i);
    int16_t rssi = (int16_t)WiFi.RSSI(i);

    for (uint8_t p = 0; p < WIFI_MAXIMUM_CONNECTIONS; p++)
    {
      const char* cfg = tkr_set->Settings.network.wifi[p].ssid;
      if (cfg[0] == '\0') continue;

      if (ssid.equals(cfg))
      {
        if (rssi > best_rssi_by_profile[p]) best_rssi_by_profile[p] = rssi;
      }
    }
  }

  WiFi.scanDelete();

  // find best + second best among configured profiles seen
  int8_t  scan_best = -1;
  int16_t scan_best_rssi = (int16_t)-32768;
  int16_t scan_second_rssi = (int16_t)-32768;

  for (uint8_t p = 0; p < WIFI_MAXIMUM_CONNECTIONS; p++)
  {
    int16_t r = best_rssi_by_profile[p];
    if (r == (int16_t)-32768) continue;

    if (r > scan_best_rssi)
    {
      scan_second_rssi = scan_best_rssi;
      scan_best_rssi = r;
      scan_best = (int8_t)p;
    }
    else if (r > scan_second_rssi)
    {
      scan_second_rssi = r;
    }
  }

  if (scan_best < 0)
  {
    // none of our SSIDs found in scan
    return ordered_first;
  }

  if ((uint8_t)scan_best == ordered_first)
  {
    return ordered_first;
  }

  const int16_t ordered_first_rssi = best_rssi_by_profile[ordered_first];
  const bool ordered_seen = (ordered_first_rssi != (int16_t)-32768);

  bool override_order = false;

  if (ordered_seen)
  {
    if ((scan_best_rssi - ordered_first_rssi) >= (int16_t)WIFI_RSSI_THRESHOLD) override_order = true;
  }
  else
  {
    // ordered-first not seen; only override if best clearly stronger than second best
    if ((scan_second_rssi != (int16_t)-32768) &&
        ((scan_best_rssi - scan_second_rssi) >= (int16_t)WIFI_RSSI_THRESHOLD))
    {
      override_order = true;
    }
  }

  return override_order ? (uint8_t)scan_best : ordered_first;
}

void mWiFi::WiFi_Sta_Connect_Start(void)
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  // Policy: SoftAP only if ALL SSIDs are empty
  if (!WiFi2_HasAnyStaProfileConfigured())
  {
    WiFi2_Ap_EnsureStarted();
    return;
  }

  const bool do_scan = WiFi_Sta_ShouldScanNow_OnBootOrOutage();
  const uint8_t profile_i = WiFi_Sta_SelectProfileIndex_WithScanPreference(do_scan);

  WiFi_Sta_ProfileIndex_Connect(profile_i);
}




void mWiFi::WiFi_Sta_ProfileIndex_Connect(uint8_t profile_i)
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  // Bounds + configured check
  if (profile_i >= WIFI_MAXIMUM_CONNECTIONS) { return; }

  const auto& p = tkr_set->Settings.network.wifi[profile_i];
  if (p.ssid[0] == '\0') { return; }

  // Preserve existing semantics: record which slot we are attempting
  tkr_set->Settings.network.sta_active = profile_i;

  // Optional: keep your existing behaviour stable
  WiFi.persistent(false);

  // Ensure STA mode
  WiFi.mode(WIFI_STA);

  #ifdef ESP8266
    // ESP8266 hostname must be set after WiFi.mode(WIFI_STA)
    WiFi.hostname(tkr_set->runtime.my_hostname);
  #endif

  // Apply static/DHCP config from your new settings struct
  const auto& ipcfg = tkr_set->Settings.network.wifi_ipv4;
  if (ipcfg.is_static)
  {
    IPAddress ip   = IPv4ArrayToIP(ipcfg.ip);
    IPAddress gw   = IPv4ArrayToIP(ipcfg.gw);
    IPAddress sn   = IPv4ArrayToIP(ipcfg.sn);
    IPAddress dns1 = IPv4ArrayToIP(ipcfg.dns1);
    IPAddress dns2 = IPv4ArrayToIP(ipcfg.dns2);

    WiFi.config(ip, gw, sn, dns1, dns2);
  }
  else
  {
    // Reset to DHCP (core-compatible)
    WiFi.config(IPAddress((uint32_t)0), IPAddress((uint32_t)0), IPAddress((uint32_t)0));
  }

  // Start connection (ONLY place we call WiFi.begin)
  if (p.has_bssid)
  {
    WiFi.begin(p.ssid, p.pass, 0 /*channel*/, p.bssid);
    memcpy(connection.bssid, p.bssid, 6);
  }
  else
  {
    WiFi.begin(p.ssid, p.pass);
    memset(connection.bssid, 0, 6);
  }

  #ifdef ENABLE_FEATURE_WIFI__SET_TXPOWER
    WiFi.setTxPower(WIFI_POWER_8_5dBm);
  #endif

  #ifdef ENABLE_DEVFEATURE_NETWORK__WIFI_DUPLICATED_WRITEBACK
    WiFi2_LegacyWriteback_FromNetworkSettings();
  #endif

  // Update connection bookkeeping (preserve your existing fields)
  connection.fReconnect = true;
  connection.last_event = millis();
}


static inline bool _ssid_is_configured(const char* s) { return (s && s[0] != '\0'); }

IPAddress mWiFi::IPv4ArrayToIP(const uint8_t a[4])
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  return IPAddress(a[0], a[1], a[2], a[3]);
}

bool mWiFi::WiFi2_HasAnyStaProfileConfigured(void) const
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  for (uint8_t i = 0; i < WIFI_MAXIMUM_CONNECTIONS; i++)
  {
    const auto& p = tkr_set->Settings.network.wifi[i];
    if (_ssid_is_configured(p.ssid)) { return true; }
  }
  return false;
}

uint8_t mWiFi::WiFi2_GetFirstConfiguredProfileIndex(void) const
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  for (uint8_t i = 0; i < WIFI_MAXIMUM_CONNECTIONS; i++)
  {
    const auto& p = tkr_set->Settings.network.wifi[i];
    if (_ssid_is_configured(p.ssid)) { return i; }
  }
  return 0;
}




void mWiFi::WiFi2_Sta_Connected_Enter(void)
{
  ALOG_INF(PSTR(D_LOG_WIFI "%s"),"WiFi2_Sta_Connected_Enter");
  
  connection.fConnected = true;
  connection.fReconnect = false;
  connection.link_count++;

  // Preserve existing system flags pattern
  tkr_set->Settings.network.flag.network_wifi = 1;
  tkr_set->runtime.global_state.wifi_down = false;

  tkr_set->runtime.global_state.network_down = false;

  // Trigger existing task event flow
  tkr->Tasker_Interface(TASK_WIFI_CONNECTED);
}

void mWiFi::WiFi2_Sta_Disconnected_Enter(void)
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  connection.fConnected = false;

  
  tkr_set->runtime.global_state.network_down = true;

  tkr_set->Settings.network.flag.network_wifi = 0;
  tkr_set->runtime.global_state.wifi_down = true;
}

void mWiFi::WiFi2_Sta_EnsureConnecting(void)
{ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  if (WL_CONNECTED == WiFi.status()) { return; }
  if (!WiFi2_HasAnyStaProfileConfigured()) { return; }

  // Try in configured order starting from current active
  uint8_t start_i = tkr_set->Settings.network.sta_active;

  for (uint8_t off = 0; off < WIFI_MAXIMUM_CONNECTIONS; off++)
  {
    uint8_t i = (start_i + off) % WIFI_MAXIMUM_CONNECTIONS;
    const auto& p = tkr_set->Settings.network.wifi[i];
    if (p.ssid[0] == '\0') { continue; }

    WiFi_Sta_ProfileIndex_Connect(i);
    return;
  }
}


void mWiFi::WiFi_Sta_State_Set(uint8_t state)
{
ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  //check for change in state
  if(connection.fConnected != state)
  {
    DEBUG_LINE_HERE;

    if(state){ //new state 
    // pinMode(2,OUTPUT);
    // digitalWrite(2,LOW);
      tkr->Tasker_Interface(TASK_WIFI_CONNECTED);
      loglevel_with_connection_status = LOG_LEVEL_DEBUG_MORE;
    }else{
    // pinMode(2,OUTPUT);
    // digitalWrite(2,HIGH);
      tkr->Tasker_Interface(TASK_WIFI_DISCONNECTED);
      loglevel_with_connection_status = LOG_LEVEL_INFO;
    }
  }

  connection.fConnected = state;

  
  if(state == 0){
    ALOG_INF(PSTR(D_LOG_DEBUG "%s=%d"),"WiFi_Sta_State_Set",state);
  }

  if (state == tkr_set->runtime.global_state.wifi_down) {
    DEBUG_LINE_HERE;
    if (state) {
      // tkr_set->rules_flag.wifi_connected = 1;
      connection.link_count++;
      connection.downtime += tkr_time->UpTime() - connection.last_event;
    } else {
      // tkr_set->rules_flag.wifi_disconnected = 1;
      connection.last_event = tkr_time->UpTime();
    }DEBUG_LINE_HERE

  }
  // if(tkr_time==NULL){
  //    ALOG_DBM( "tkr_time==NULL");
  // }
  tkr_set->runtime.global_state.wifi_down = state ^1;
  if (!tkr_set->runtime.global_state.wifi_down) {
    // DEBUG_LINE_HERE;
    tkr_set->runtime.global_state.network_down = 0;
  }

  // ALOG_INF(PSTR(D_LOG_DEBUG "%s"),"WiFi_Sta_State_Set end");
    
}

#endif // USE_MODULE_NETWORK_WIFI