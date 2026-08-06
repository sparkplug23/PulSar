#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI

void mWiFi::WiFi_Sta_Maintain_Periodic(void)
{
  const bool connected = (WiFi.status() == WL_CONNECTED);

  // Connected and holding a usable IP address
  if (connected && WiFi_Link_IsIpRoutable())
  {
    if (!connection.fConnected)
    {
      WiFi2_Sta_Connected_Enter();
    }

    WiFi_Sta_OnConnected_ResetOutageScanFlags();
    return;
  }

  // Previously connected, but the connection has now been lost
  if (connection.fConnected)
  {
    WiFi2_Sta_Disconnected_Enter();
  }

  // Maintain outage duration
  if (connection.downtime < 0xFFFFFFFFUL)
  {
    connection.downtime++;
  }

  // Nothing to connect to
  if (!WiFi2_HasAnyStaProfileConfigured())
  {
    return;
  }

  // Wait before trying the next profile
  if (connection.counter > 0)
  {
    ALOG_DBG(
      PSTR(D_LOG_WIFI "Reconnecting in %u seconds"),
      connection.counter
    );

    connection.counter--;
    return;
  }

  /*
   * On boot, or after a long outage, allow a scan-based selection.
   *
   * During ordinary retries, the selector advances to the next configured
   * profile after config.station.active_profile.
   */
  const bool do_scan = WiFi_Sta_ShouldScanNow_OnBootOrOutage();

  const uint8_t profile_i =
    WiFi_Sta_SelectProfileIndex_WithScanPreference(do_scan);

  ALOG_INF(
    PSTR(D_LOG_WIFI "Trying WiFi profile %u: %s"),
    profile_i,
    config.station.profiles[profile_i].ssid
  );

  WiFi_Sta_ProfileIndex_Connect(profile_i);

  // Wait before trying another configured profile
  connection.counter = WIFI_CHECK_SEC;
}


uint8_t mWiFi::WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured(void) const
{
  DEBUG_LINE_HERE3
  // ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  for (uint8_t i = 0; i < WIFI_MAXIMUM_CONNECTIONS; i++)
  {
    if (config.station.profiles[i].ssid[0] != '\0')
    {
      ALOG_INF(PSTR("config.station.profiles[i].ssid[0] %s %d"),config.station.profiles[i].ssid,i);
      return i;
    } 
  }
  DEBUG_LINE_HERE3
  return 0;
}
bool mWiFi::WiFi_Sta_ShouldScanNow_OnBootOrOutage(void)
{
  // ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
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
//   ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  // When link is back, allow a future outage to trigger a scan again
  s_wifi2_scanned_on_this_outage = false;
  connection.downtime = 0;
}

uint8_t mWiFi::WiFi_Sta_SelectProfileIndex_WithScanPreference(bool force_scan)
{
  /*
   * Normal retry behaviour:
   *
   * Start after active_profile and return the next configured profile.
   * This produces:
   *
   *   profile 0 -> profile 1 -> profile 2 -> profile 0
   *
   * Empty profile slots are skipped.
   */
  if (!force_scan)
  {
    const uint8_t start_i =
      (config.station.active_profile + 1) % WIFI_MAXIMUM_CONNECTIONS;

    for (uint8_t offset = 0;
         offset < WIFI_MAXIMUM_CONNECTIONS;
         offset++)
    {
      const uint8_t profile_i =
        (start_i + offset) % WIFI_MAXIMUM_CONNECTIONS;

      if (config.station.profiles[profile_i].ssid[0] != '\0')
      {
        return profile_i;
      }
    }

    // Defensive fallback
    return WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured();
  }

  /*
   * Scan-based selection is used once during boot and once after the
   * configured long-outage interval.
   */
  const uint8_t ordered_first =
    WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured();

  const int network_count = WiFi.scanNetworks();

  if (network_count <= 0)
  {
    WiFi.scanDelete();
    return ordered_first;
  }

  // Strongest observed RSSI for every configured profile
  int16_t best_rssi_by_profile[WIFI_MAXIMUM_CONNECTIONS];

  for (uint8_t profile_i = 0;
       profile_i < WIFI_MAXIMUM_CONNECTIONS;
       profile_i++)
  {
    best_rssi_by_profile[profile_i] = INT16_MIN;
  }

  for (int scan_i = 0; scan_i < network_count; scan_i++)
  {
    const String scanned_ssid = WiFi.SSID(scan_i);
    const int16_t scanned_rssi = (int16_t)WiFi.RSSI(scan_i);

    for (uint8_t profile_i = 0;
         profile_i < WIFI_MAXIMUM_CONNECTIONS;
         profile_i++)
    {
      const char* configured_ssid =
        config.station.profiles[profile_i].ssid;

      if (configured_ssid[0] == '\0')
      {
        continue;
      }

      if (scanned_ssid.equals(configured_ssid))
      {
        if (scanned_rssi > best_rssi_by_profile[profile_i])
        {
          best_rssi_by_profile[profile_i] = scanned_rssi;
        }
      }
    }
  }

  WiFi.scanDelete();

  int8_t strongest_profile = -1;
  int16_t strongest_rssi = INT16_MIN;
  int16_t second_strongest_rssi = INT16_MIN;

  for (uint8_t profile_i = 0;
       profile_i < WIFI_MAXIMUM_CONNECTIONS;
       profile_i++)
  {
    const int16_t profile_rssi =
      best_rssi_by_profile[profile_i];

    if (profile_rssi == INT16_MIN)
    {
      continue;
    }

    if (profile_rssi > strongest_rssi)
    {
      second_strongest_rssi = strongest_rssi;
      strongest_rssi = profile_rssi;
      strongest_profile = (int8_t)profile_i;
    }
    else if (profile_rssi > second_strongest_rssi)
    {
      second_strongest_rssi = profile_rssi;
    }
  }

  // None of the configured networks were visible
  if (strongest_profile < 0)
  {
    return ordered_first;
  }

  // Ordered-first profile is already strongest
  if ((uint8_t)strongest_profile == ordered_first)
  {
    return ordered_first;
  }

  const int16_t ordered_first_rssi =
    best_rssi_by_profile[ordered_first];

  const bool ordered_first_seen =
    (ordered_first_rssi != INT16_MIN);

  /*
   * Prefer configured order unless another configured SSID is clearly
   * stronger by WIFI_RSSI_THRESHOLD.
   */
  if (ordered_first_seen)
  {
    if ((strongest_rssi - ordered_first_rssi) >=
        (int16_t)WIFI_RSSI_THRESHOLD)
    {
      return (uint8_t)strongest_profile;
    }

    return ordered_first;
  }

  /*
   * Ordered-first was not visible.
   *
   * Use the strongest visible configured profile. The old implementation
   * could incorrectly return the invisible ordered-first profile when only
   * one alternative network was visible.
   */
  return (uint8_t)strongest_profile;
}


void mWiFi::WiFi_Sta_Connect_Start(void)
{
  // ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
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
{
  // ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  // Bounds + configured check
  if (profile_i >= WIFI_MAXIMUM_CONNECTIONS) { return; }

  const auto& p = config.station.profiles[profile_i];
  if (p.ssid[0] == '\0') { return; }

  // Preserve existing semantics: record which slot we are attempting
  config.station.active_profile = profile_i;

  // Optional: keep your existing behaviour stable
  WiFi.persistent(false);

  // Ensure STA mode
  WiFi.mode(WIFI_STA);

  #ifdef ENABLE_DEBUGFEATURE_WIFI__SUPERMINI_REDUCE_WIFI_BAD_ANTENNA_HARDWARE
  esp_wifi_set_max_tx_power(40); 
  #endif


  #ifdef ESP8266
    // ESP8266 hostname must be set after WiFi.mode(WIFI_STA)
    WiFi.hostname(tkr_set->runtime.my_hostname);
  #endif

  // Apply static/DHCP config from your new settings struct
  const auto& ipcfg = config.station.ipv4;
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

  // Update connection bookkeeping (preserve your existing fields)
  connection.fReconnect = true;
  connection.last_event = millis();

  SET_SYSTEM_LED__NO_NETWORK(true);
}


static inline bool _ssid_is_configured(const char* s) { return (s && s[0] != '\0'); }

IPAddress mWiFi::IPv4ArrayToIP(const uint8_t a[4])
{
  // ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  return IPAddress(a[0], a[1], a[2], a[3]);
}

bool mWiFi::WiFi2_HasAnyStaProfileConfigured(void) const
{
  if (!config.station.enabled)
  {
    return false;
  }

  for (uint8_t profile_i = 0; profile_i < WIFI_MAXIMUM_CONNECTIONS; profile_i++)
  {
    if (config.station.profiles[profile_i].ssid[0] != '\0')
    {
      return true;
    }
  }

  return false;
}

uint8_t mWiFi::WiFi2_GetFirstConfiguredProfileIndex(void) const
{
  for (uint8_t profile_i = 0; profile_i < WIFI_MAXIMUM_CONNECTIONS; profile_i++)
  {
    if (config.station.profiles[profile_i].ssid[0] != '\0')
    {
      return profile_i;
    }
  }

  return 0;
}


void mWiFi::WiFi2_Sta_Connected_Enter(void)
{
  ALOG_DBG(PSTR(D_LOG_WIFI "%s"),"WiFi2_Sta_Connected_Enter");
  
  connection.fConnected = true;
  connection.fReconnect = false;
  connection.link_count++;

  // ALOG_INF(
  //   PSTR(D_LOG_WIFI "STA connected | SSID=%s | IP=%s | GW=%s | RSSI=%d dBm"),
  //   WiFi.SSID().c_str(),
  //   WiFi.localIP().toString().c_str(),
  //   WiFi.gatewayIP().toString().c_str(),
  //   WiFi.RSSI()
  // );

  // Preserve existing system flags pattern
  tkr_set->Settings.sysopt_network.bit.network_wifi = 1;
  tkr_set->runtime.global_state.wifi_down = false;

  tkr_set->runtime.global_state.network_down = false;

  // Trigger existing task event flow
  tkr->Tasker_Interface(TASK_NETWORK_CONNECTED__WIFI);
}

void mWiFi::WiFi2_Sta_Disconnected_Enter(void)
{
  // ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  connection.fConnected = false;
  
  tkr_set->runtime.global_state.network_down = true;

  tkr_set->Settings.sysopt_network.bit.network_wifi = 0;
  tkr_set->runtime.global_state.wifi_down = true;
  
  // Trigger existing task event flow
  tkr->Tasker_Interface(TASK_NETWORK_LOST__WIFI);
}

void mWiFi::WiFi2_Sta_EnsureConnecting(void)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    return;
  }

  if (!WiFi2_HasAnyStaProfileConfigured())
  {
    return;
  }

  // Select the next configured profile after active_profile
  const uint8_t profile_i =
    WiFi_Sta_SelectProfileIndex_WithScanPreference(false);

  ALOG_INF(
    PSTR(D_LOG_WIFI "Ensuring connection using profile %u: %s"),
    profile_i,
    config.station.profiles[profile_i].ssid
  );

  WiFi_Sta_ProfileIndex_Connect(profile_i);
}


void mWiFi::WiFi_Sta_State_Set(uint8_t state)
{
  // ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"), __FILE__, __LINE__);

  // Normalise: treat any non-zero as "connected"
  state = (state != 0) ? 1 : 0;

  const bool prev_connected = (connection.fConnected != 0);
  const bool new_connected  = (state != 0);

  // Edge-trigger only
  if (prev_connected != new_connected)
  {

    // NOTE: this function historically triggers task events directly
    if (new_connected)
    {
      tkr->Tasker_Interface(TASK_NETWORK_CONNECTED__WIFI);
      loglevel_with_connection_status = LOG_LEVEL_DEBUG_MORE;

      // Transition into connected:
      // - Count link ups
      // - Reset downtime tracking and last_event for next outage window
      connection.link_count++;
      connection.downtime  = 0;
      connection.last_event = tkr_time ? tkr_time->UpTime() : 0;
    }
    else
    {
      tkr->Tasker_Interface(TASK_NETWORK_LOST__WIFI);
      loglevel_with_connection_status = LOG_LEVEL_INFO;

      // Transition into disconnected:
      // - Start outage timer
      connection.last_event = tkr_time ? tkr_time->UpTime() : 0;
    }

  }

  // Persist state
  connection.fConnected = state;

  if (state == 0)
  {
    ALOG_DBG(PSTR(D_LOG_DEBUG "%s=%d"), "WiFi_Sta_State_Set", state);
  }

  // -------------------------------------------------------------------------
  // Global flags must be consistent:
  // wifi_down == !connected
  // network_down cleared only when wifi comes up
  // -------------------------------------------------------------------------
  tkr_set->runtime.global_state.wifi_down = new_connected ? 0 : 1;

  if (new_connected)
  {
    tkr_set->runtime.global_state.network_down = 0;
  }
  
}




#endif // USE_MODULE_NETWORK_WIFI