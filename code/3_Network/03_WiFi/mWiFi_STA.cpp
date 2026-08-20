#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI


void mWiFi::WiFi_Sta_Maintain_Periodic(void)
{
  const bool connected = (WiFi.status() == WL_CONNECTED);

  // Connected and holding a usable IP address
  if(connected && WiFi_Link_IsIpRoutable())
  {
    if(!connection.fConnected)
    {
      WiFi2_Sta_Connected_Enter();
    }

    #ifdef ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA
    connection.quick_connect_active = false;
    #endif

    #ifdef ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES
    WiFi_Sta_CandidateList_Clear();
    #endif

    WiFi_Sta_OnConnected_ResetOutageScanFlags();
    return;
  }

  // Previously connected, but the connection has now been lost
  if(connection.fConnected)
  {
    WiFi2_Sta_Disconnected_Enter();
  }

  if(connection.downtime < 0xFFFFFFFFUL)
  {
    connection.downtime++;
  }

  if(!WiFi2_HasAnyStaProfileConfigured())
  {
    return;
  }

  // -------------------------------------------------------------------------
  // Active connection-attempt timeout
  // -------------------------------------------------------------------------
  if(connection.counter > 0)
  {
    const wl_status_t status_now = WiFi.status();

    bool immediate_failure = false;

    if(status_now == WL_NO_SSID_AVAIL || status_now == WL_CONNECT_FAILED)
    {
      immediate_failure = true;
    }

    if(immediate_failure)
    {
      connection.counter = 0;
    }
    else
    {
      ALOG_DBG(PSTR(D_LOG_WIFI "Connection attempt remaining %u seconds"),connection.counter);
      connection.counter--;
      return;
    }
  }

  // -------------------------------------------------------------------------
  // OTA quick-connect has now timed out.
  //
  // The RTC record has already been consumed. Fall through into the normal
  // boot scan path. Since s_wifi2_scanned_on_boot is still false, this will
  // perform the normal initial scan.
  // -------------------------------------------------------------------------
  #ifdef ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA
  if(connection.quick_connect_active)
  {
    connection.quick_connect_active = false;
    ALOG_INF(PSTR(D_LOG_WIFI "OTA quick-connect failed, falling back to normal selection"));
  }
  #endif

  // -------------------------------------------------------------------------
  // Ranked visible candidate list
  //
  // If a scan previously produced several configured visible networks,
  // rapidly try the next one before falling back to ordinary profile cycling.
  // -------------------------------------------------------------------------
  #ifdef ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES
  if(wifi_candidate_index < wifi_candidate_count)
  {
    if(WiFi_Sta_CandidateList_TryNext())
    {
      return;
    }
  }

  if(wifi_candidate_count > 0 && wifi_candidate_index >= wifi_candidate_count)
  {
    ALOG_INF(PSTR(D_LOG_WIFI "Visible candidate list exhausted"));
    WiFi_Sta_CandidateList_Clear();

    // All networks that were known-visible have failed.
    // Re-enter the normal slower retry policy rather than hammering them.
    connection.counter = WIFI_CHECK_SEC;
    return;
  }
  #endif

  const bool do_scan = WiFi_Sta_ShouldScanNow_OnBootOrOutage();

  #ifdef ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES

  if(do_scan)
  {
    if(WiFi_Sta_CandidateList_BuildFromScan())
    {
      if(WiFi_Sta_CandidateList_TryNext())
      {
        return;
      }
    }

    // Scan found none of our configured profiles.
    const uint8_t profile_i = WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured();

    ALOG_INF(PSTR(D_LOG_WIFI "No configured SSID visible; fallback profile %u: %s"),profile_i,config.station.profiles[profile_i].ssid);

    WiFi_Sta_ProfileIndex_Connect(profile_i);
    connection.counter = WIFI_CHECK_SEC;
    return;
  }

  #endif

  const uint8_t profile_i = WiFi_Sta_SelectProfileIndex_WithScanPreference(do_scan);

  ALOG_INF(PSTR(D_LOG_WIFI "Trying WiFi profile %u: %s"),profile_i,config.station.profiles[profile_i].ssid);

  WiFi_Sta_ProfileIndex_Connect(profile_i);
  connection.counter = WIFI_CHECK_SEC;
}


uint8_t mWiFi::WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured(void) const
{
  DEBUG_LINE_HERE3

  for(uint8_t i=0;i<WIFI_MAXIMUM_CONNECTIONS;i++)
  {
    if(config.station.profiles[i].ssid[0] != '\0')
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
  if(!s_wifi2_scanned_on_boot)
  {
    s_wifi2_scanned_on_boot = true;
    return true;
  }

  if(!s_wifi2_scanned_on_this_outage && (connection.downtime >= WIFI_OUTAGE_RESCAN_SECONDS))
  {
    s_wifi2_scanned_on_this_outage = true;
    return true;
  }

  return false;
}


void mWiFi::WiFi_Sta_OnConnected_ResetOutageScanFlags(void)
{
  s_wifi2_scanned_on_this_outage = false;
  connection.downtime = 0;
}


uint8_t mWiFi::WiFi_Sta_SelectProfileIndex_WithScanPreference(bool force_scan)
{
  /*
   * Existing/default behaviour.
   *
   * This remains in place so builds without
   * ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES behave as before.
   */
  if(!force_scan)
  {
    const uint8_t start_i = (config.station.active_profile + 1) % WIFI_MAXIMUM_CONNECTIONS;

    for(uint8_t offset=0;offset<WIFI_MAXIMUM_CONNECTIONS;offset++)
    {
      const uint8_t profile_i = (start_i + offset) % WIFI_MAXIMUM_CONNECTIONS;

      if(config.station.profiles[profile_i].ssid[0] != '\0')
      {
        return profile_i;
      }
    }

    return WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured();
  }

  const uint8_t ordered_first = WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured();

  const int network_count = WiFi.scanNetworks();

  if(network_count <= 0)
  {
    WiFi.scanDelete();
    return ordered_first;
  }

  int16_t best_rssi_by_profile[WIFI_MAXIMUM_CONNECTIONS];

  for(uint8_t profile_i=0;profile_i<WIFI_MAXIMUM_CONNECTIONS;profile_i++)
  {
    best_rssi_by_profile[profile_i] = INT16_MIN;
  }

  for(int scan_i=0;scan_i<network_count;scan_i++)
  {
    const String scanned_ssid = WiFi.SSID(scan_i);
    const int16_t scanned_rssi = (int16_t)WiFi.RSSI(scan_i);

    for(uint8_t profile_i=0;profile_i<WIFI_MAXIMUM_CONNECTIONS;profile_i++)
    {
      const char* configured_ssid = config.station.profiles[profile_i].ssid;

      if(configured_ssid[0] == '\0')
      {
        continue;
      }

      if(scanned_ssid.equals(configured_ssid))
      {
        if(scanned_rssi > best_rssi_by_profile[profile_i])
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

  for(uint8_t profile_i=0;profile_i<WIFI_MAXIMUM_CONNECTIONS;profile_i++)
  {
    const int16_t profile_rssi = best_rssi_by_profile[profile_i];

    if(profile_rssi == INT16_MIN)
    {
      continue;
    }

    if(profile_rssi > strongest_rssi)
    {
      second_strongest_rssi = strongest_rssi;
      strongest_rssi = profile_rssi;
      strongest_profile = (int8_t)profile_i;
    }
    else if(profile_rssi > second_strongest_rssi)
    {
      second_strongest_rssi = profile_rssi;
    }
  }

  if(strongest_profile < 0)
  {
    return ordered_first;
  }

  if((uint8_t)strongest_profile == ordered_first)
  {
    return ordered_first;
  }

  const int16_t ordered_first_rssi = best_rssi_by_profile[ordered_first];
  const bool ordered_first_seen = (ordered_first_rssi != INT16_MIN);

  if(ordered_first_seen)
  {
    if((strongest_rssi - ordered_first_rssi) >= (int16_t)WIFI_RSSI_THRESHOLD)
    {
      return (uint8_t)strongest_profile;
    }

    return ordered_first;
  }

  return (uint8_t)strongest_profile;
}


/************************************************************************************************
 * SECTION: Scan and ranked visible candidates
 ************************************************************************************************/

#ifdef ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES

void mWiFi::WiFi_Sta_CandidateList_Clear(void)
{
  wifi_candidate_count = 0;
  wifi_candidate_index = 0;
  memset(wifi_candidates,0,sizeof(wifi_candidates));
}


bool mWiFi::WiFi_Sta_CandidateShouldComeBefore(const WiFiCandidate& a, const WiFiCandidate& b) const
{
  /*
   * A clearly stronger signal wins.
   *
   * If the networks are within WIFI_RSSI_THRESHOLD dB, retain user-defined
   * preference by the profile priority value. Lower priority number means
   * preferred.
   */
  if(a.rssi >= (b.rssi + WIFI_RSSI_THRESHOLD))
  {
    return true;
  }

  if(b.rssi >= (a.rssi + WIFI_RSSI_THRESHOLD))
  {
    return false;
  }

  const uint8_t priority_a = config.station.profiles[a.profile].priority;
  const uint8_t priority_b = config.station.profiles[b.profile].priority;

  if(priority_a != priority_b)
  {
    return priority_a < priority_b;
  }

  return a.profile < b.profile;
}


bool mWiFi::WiFi_Sta_CandidateList_BuildFromScan(void)
{
  WiFi_Sta_CandidateList_Clear();

  const int network_count = WiFi.scanNetworks();

  if(network_count <= 0)
  {
    WiFi.scanDelete();
    ALOG_INF(PSTR(D_LOG_WIFI "WiFi scan returned no networks"));
    return false;
  }

  for(int scan_i=0;scan_i<network_count;scan_i++)
  {
    const String scanned_ssid = WiFi.SSID(scan_i);
    const int16_t scanned_rssi = (int16_t)WiFi.RSSI(scan_i);

    for(uint8_t profile_i=0;profile_i<WIFI_MAXIMUM_CONNECTIONS;profile_i++)
    {
      const auto& profile = config.station.profiles[profile_i];

      if(profile.ssid[0] == '\0')
      {
        continue;
      }

      if(!scanned_ssid.equals(profile.ssid))
      {
        continue;
      }

      int8_t existing_index = -1;

      for(uint8_t candidate_i=0;candidate_i<wifi_candidate_count;candidate_i++)
      {
        if(wifi_candidates[candidate_i].profile == profile_i)
        {
          existing_index = candidate_i;
          break;
        }
      }

      if(existing_index < 0)
      {
        if(wifi_candidate_count >= WIFI_MAXIMUM_CONNECTIONS)
        {
          continue;
        }

        existing_index = wifi_candidate_count++;
        wifi_candidates[existing_index].profile = profile_i;
        wifi_candidates[existing_index].rssi = INT16_MIN;
      }

      WiFiCandidate& candidate = wifi_candidates[existing_index];

      if(scanned_rssi > candidate.rssi)
      {
        candidate.rssi = scanned_rssi;
        candidate.channel = (uint8_t)WiFi.channel(scan_i);

        const uint8_t* scanned_bssid = WiFi.BSSID(scan_i);

        if(scanned_bssid)
        {
          memcpy(candidate.bssid,scanned_bssid,6);
        }
        else
        {
          memset(candidate.bssid,0,6);
        }
      }
    }
  }

  WiFi.scanDelete();

  // Small fixed list: simple insertion sort is sufficient.
  for(uint8_t i=1;i<wifi_candidate_count;i++)
  {
    WiFiCandidate moving = wifi_candidates[i];
    int8_t j = i - 1;

    while(j >= 0 && WiFi_Sta_CandidateShouldComeBefore(moving,wifi_candidates[j]))
    {
      wifi_candidates[j + 1] = wifi_candidates[j];
      j--;
    }

    wifi_candidates[j + 1] = moving;
  }

  wifi_candidate_index = 0;

  for(uint8_t i=0;i<wifi_candidate_count;i++)
  {
    const WiFiCandidate& candidate = wifi_candidates[i];

    ALOG_INF(
      PSTR(D_LOG_WIFI "Candidate rank=%u profile=%u SSID=%s RSSI=%d channel=%u priority=%u"),
      i,
      candidate.profile,
      config.station.profiles[candidate.profile].ssid,
      candidate.rssi,
      candidate.channel,
      config.station.profiles[candidate.profile].priority
    );
  }

  return wifi_candidate_count > 0;
}


bool mWiFi::WiFi_Sta_CandidateList_TryNext(void)
{
  if(wifi_candidate_index >= wifi_candidate_count)
  {
    return false;
  }

  const WiFiCandidate candidate = wifi_candidates[wifi_candidate_index++];

  ALOG_INF(
    PSTR(D_LOG_WIFI "Trying visible candidate %u/%u | profile=%u | SSID=%s | RSSI=%d | channel=%u"),
    wifi_candidate_index,
    wifi_candidate_count,
    candidate.profile,
    config.station.profiles[candidate.profile].ssid,
    candidate.rssi,
    candidate.channel
  );

  WiFi_Sta_ProfileIndex_Connect(candidate.profile,candidate.channel,candidate.bssid);
  connection.counter = WIFI_VISIBLE_CANDIDATE_CONNECT_TIMEOUT_SEC;

  return true;
}

#endif


/************************************************************************************************
 * SECTION: OTA RTC quick-connect
 ************************************************************************************************/

#ifdef ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA

bool mWiFi::WiFi_QuickConnect_SaveToRTC(void)
{
  #ifndef ENABLE_FEATURE_RTC__SETTINGS
  return false;
  #else

  if(WiFi.status() != WL_CONNECTED)
  {
    return false;
  }

  uint8_t profile_i = config.station.active_profile;

  // Defensive match against the actual connected SSID in case active_profile
  // was not the source of the current association.
  const String current_ssid = WiFi.SSID();

  for(uint8_t i=0;i<WIFI_MAXIMUM_CONNECTIONS;i++)
  {
    if(config.station.profiles[i].ssid[0] == '\0')
    {
      continue;
    }

    if(current_ssid.equals(config.station.profiles[i].ssid))
    {
      profile_i = i;
      break;
    }
  }

  if(profile_i >= WIFI_MAXIMUM_CONNECTIONS)
  {
    return false;
  }

  const uint8_t* bssid = WiFi.BSSID();

  if(!bssid)
  {
    return false;
  }

  const int32_t current_channel = WiFi.channel();

  if(current_channel <= 0 || current_channel > 255)
  {
    return false;
  }

  RtcMemory__WiFiQuickConnect_Set(profile_i,(uint8_t)current_channel,bssid);

  ALOG_INF(
    PSTR(D_LOG_WIFI "OTA quick-connect stored | profile=%u | SSID=%s | channel=%u"),
    profile_i,
    config.station.profiles[profile_i].ssid,
    (uint8_t)current_channel
  );

  return true;

  #endif
}


bool mWiFi::WiFi_QuickConnect_TryFromRTC(void)
{
  #ifndef ENABLE_FEATURE_RTC__SETTINGS
  return false;
  #else

  if(!RtcMemory__RuntimeState_Valid())
  {
    return false;
  }

  if(!RtcMemory__WiFiQuickConnect_Valid())
  {
    return false;
  }

  const uint8_t profile_i = RtcMemory__RuntimeState.wifi_quick_connect_profile;
  const uint8_t channel = RtcMemory__RuntimeState.wifi_quick_connect_channel;

  uint8_t bssid[6];
  memcpy(bssid,RtcMemory__RuntimeState.wifi_quick_connect_bssid,6);

  // One-shot state. Consume before attempting so a later crash/reboot does not
  // repeatedly force the stale quick-connect path.
  RtcMemory__WiFiQuickConnect_Clear();

  if(profile_i >= WIFI_MAXIMUM_CONNECTIONS)
  {
    return false;
  }

  if(config.station.profiles[profile_i].ssid[0] == '\0')
  {
    return false;
  }

  if(channel == 0)
  {
    return false;
  }

  bool bssid_valid = false;

  for(uint8_t i=0;i<6;i++)
  {
    if(bssid[i] != 0)
    {
      bssid_valid = true;
      break;
    }
  }

  if(!bssid_valid)
  {
    return false;
  }

  ALOG_INF(
    PSTR(D_LOG_WIFI "OTA quick-connect | profile=%u | SSID=%s | channel=%u"),
    profile_i,
    config.station.profiles[profile_i].ssid,
    channel
  );

  connection.quick_connect_active = true;

  WiFi_Sta_ProfileIndex_Connect(profile_i,channel,bssid);

  // Short attempt only. If it fails the normal boot scan follows.
  connection.counter = WIFI_QUICK_CONNECT_TIMEOUT_SEC;

  return true;

  #endif
}

#endif


void mWiFi::WiFi_Sta_Connect_Start(void)
{
  if(!WiFi2_HasAnyStaProfileConfigured())
  {
    WiFi2_Ap_EnsureStarted();
    return;
  }

  const bool do_scan = WiFi_Sta_ShouldScanNow_OnBootOrOutage();

  #ifdef ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES

  if(do_scan)
  {
    if(WiFi_Sta_CandidateList_BuildFromScan())
    {
      if(WiFi_Sta_CandidateList_TryNext())
      {
        return;
      }
    }

    const uint8_t profile_i = WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured();
    WiFi_Sta_ProfileIndex_Connect(profile_i);
    connection.counter = WIFI_CHECK_SEC;
    return;
  }

  #endif

  const uint8_t profile_i = WiFi_Sta_SelectProfileIndex_WithScanPreference(do_scan);

  WiFi_Sta_ProfileIndex_Connect(profile_i);
  connection.counter = WIFI_CHECK_SEC;
}


void mWiFi::WiFi_Sta_ProfileIndex_Connect(uint8_t profile_i, uint8_t runtime_channel, const uint8_t* runtime_bssid)
{
  if(profile_i >= WIFI_MAXIMUM_CONNECTIONS)
  {
    return;
  }

  const auto& p = config.station.profiles[profile_i];

  if(p.ssid[0] == '\0')
  {
    return;
  }

  config.station.active_profile = profile_i;

  WiFi.persistent(false);

  if(config.softap.enabled && config.softap.always_on)
  {
    WiFi.mode(WIFI_AP_STA);
  }
  else
  {
    WiFi.mode(WIFI_STA);
  }

  #ifdef ENABLE_DEBUGFEATURE_WIFI__SUPERMINI_REDUCE_WIFI_BAD_ANTENNA_HARDWARE
  esp_wifi_set_max_tx_power(40);
  #endif

  #ifdef ESP8266
  WiFi.hostname(tkr_set->runtime.my_hostname);
  #endif

  // -------------------------------------------------------------------------
  // IPv4 configuration
  // -------------------------------------------------------------------------
  const auto& ipcfg = config.station.ipv4;

  if(ipcfg.is_static)
  {
    IPAddress ip = IPv4ArrayToIP(ipcfg.ip);
    IPAddress gw = IPv4ArrayToIP(ipcfg.gw);
    IPAddress sn = IPv4ArrayToIP(ipcfg.sn);
    IPAddress dns1 = IPv4ArrayToIP(ipcfg.dns1);
    IPAddress dns2 = IPv4ArrayToIP(ipcfg.dns2);

    WiFi.config(ip,gw,sn,dns1,dns2);
  }
  else
  {
    WiFi.config(IPAddress((uint32_t)0),IPAddress((uint32_t)0),IPAddress((uint32_t)0));
  }

  // -------------------------------------------------------------------------
  // Start STA connection.
  //
  // Priority:
  //   1. Runtime BSSID/channel from scan or OTA RTC quick-connect.
  //   2. Statically configured BSSID.
  //   3. Ordinary SSID/password association.
  //
  // This remains the ONLY normal mWiFi location that calls WiFi.begin().
  // -------------------------------------------------------------------------
  if(runtime_bssid)
  {
    WiFi.begin(p.ssid,p.pass,runtime_channel,runtime_bssid);
    memcpy(connection.bssid,runtime_bssid,6);
  }
  else if(p.has_bssid)
  {
    WiFi.begin(p.ssid,p.pass,0,p.bssid);
    memcpy(connection.bssid,p.bssid,6);
  }
  else
  {
    WiFi.begin(p.ssid,p.pass);
    memset(connection.bssid,0,6);
  }

  #ifdef ENABLE_FEATURE_WIFI__SET_TXPOWER
  WiFi.setTxPower(WIFI_POWER_8_5dBm);
  #endif

  connection.fReconnect = true;
  connection.last_event = millis();

  ALOG_INF(
    PSTR(D_LOG_WIFI "STA connect started | profile=%u | SSID=%s | channel=%u | directBSSID=%u | APAlwaysOn=%u"),
    profile_i,
    p.ssid,
    runtime_channel,
    runtime_bssid ? 1 : 0,
    config.softap.always_on
  );

  SET_SYSTEM_LED__NO_NETWORK(true);
}


static inline bool _ssid_is_configured(const char* s) { return (s && s[0] != '\0'); }


IPAddress mWiFi::IPv4ArrayToIP(const uint8_t a[4])
{
  return IPAddress(a[0],a[1],a[2],a[3]);
}


bool mWiFi::WiFi2_HasAnyStaProfileConfigured(void) const
{
  if(!config.station.enabled)
  {
    return false;
  }

  for(uint8_t profile_i=0;profile_i<WIFI_MAXIMUM_CONNECTIONS;profile_i++)
  {
    if(config.station.profiles[profile_i].ssid[0] != '\0')
    {
      return true;
    }
  }

  return false;
}


uint8_t mWiFi::WiFi2_GetFirstConfiguredProfileIndex(void) const
{
  for(uint8_t profile_i=0;profile_i<WIFI_MAXIMUM_CONNECTIONS;profile_i++)
  {
    if(config.station.profiles[profile_i].ssid[0] != '\0')
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

  tkr_set->Settings.sysopt_network.bit.network_wifi = 1;
  tkr_set->runtime.global_state.wifi_down = false;
  tkr_set->runtime.global_state.network_down = false;

  tkr->Tasker_Interface(TASK_NETWORK_CONNECTED__WIFI);
}


void mWiFi::WiFi2_Sta_Disconnected_Enter(void)
{
  connection.fConnected = false;
  
  tkr_set->runtime.global_state.network_down = true;
  tkr_set->Settings.sysopt_network.bit.network_wifi = 0;
  tkr_set->runtime.global_state.wifi_down = true;
  
  tkr->Tasker_Interface(TASK_NETWORK_LOST__WIFI);
}


void mWiFi::WiFi2_Sta_EnsureConnecting(void)
{
  if(WiFi.status() == WL_CONNECTED)
  {
    return;
  }

  if(!WiFi2_HasAnyStaProfileConfigured())
  {
    return;
  }

  const uint8_t profile_i = WiFi_Sta_SelectProfileIndex_WithScanPreference(false);

  ALOG_INF(PSTR(D_LOG_WIFI "Ensuring connection using profile %u: %s"),profile_i,config.station.profiles[profile_i].ssid);

  WiFi_Sta_ProfileIndex_Connect(profile_i);
}


void mWiFi::WiFi_Sta_State_Set(uint8_t state)
{
  state = (state != 0) ? 1 : 0;

  const bool prev_connected = (connection.fConnected != 0);
  const bool new_connected = (state != 0);

  if(prev_connected != new_connected)
  {
    if(new_connected)
    {
      tkr->Tasker_Interface(TASK_NETWORK_CONNECTED__WIFI);
      loglevel_with_connection_status = LOG_LEVEL_DEBUG_MORE;

      connection.link_count++;
      connection.downtime = 0;
      connection.last_event = tkr_time ? tkr_time->UpTime() : 0;
    }
    else
    {
      tkr->Tasker_Interface(TASK_NETWORK_LOST__WIFI);
      loglevel_with_connection_status = LOG_LEVEL_INFO;

      connection.last_event = tkr_time ? tkr_time->UpTime() : 0;
    }
  }

  connection.fConnected = state;

  if(state == 0)
  {
    ALOG_DBG(PSTR(D_LOG_DEBUG "%s=%d"),"WiFi_Sta_State_Set",state);
  }

  tkr_set->runtime.global_state.wifi_down = new_connected ? 0 : 1;

  if(new_connected)
  {
    tkr_set->runtime.global_state.network_down = 0;
  }
}

#endif