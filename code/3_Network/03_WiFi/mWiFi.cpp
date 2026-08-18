#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI

#ifdef ESP8266
extern "C" {
 #include "user_interface.h"
}
#endif


int8_t mWiFi::Tasker(uint8_t function, JsonParserObject obj)
{

  switch (function)
  {
    case TASK_INIT:
    {
      Init_Preload_Wifi2_Settings();

      // Simple delayed start for both STA/AP.
      // (Later you will replace 10 with chip-id derived jitter.)
      connection.seconds_to_wait_for_fresh_connection_attempt = 10;
      
      SET_SYSTEM_LED__NO_NETWORK(true);
    }
    break;
  }

  #ifdef ENABLE_FEATURE_WIFI__BLOCK_CONNECTION
  return false;
  #endif

  switch (function)
  {
    case TASK_LOOP:
    {
      // DNS captive portal needs frequent polling
      WiFi2_Ap_Dns_Tick();

      #if defined(USE_NETWORK_MDNS) && defined(ESP8266)
        WiFi_Mdns_Tick();
      #endif
    }
    break;

    case TASK_EVERY_SECOND:
    {
      Task_EverySecond();
    }
    break;

    case TASK_EVERY_MINUTE:
    {
      // ALOG_DBG(PSTR("WL_CONNECTED=%d IP=%s"),
      //          (WiFi.status() == WL_CONNECTED),
      //          WiFi.localIP().toString().c_str());
    }
    break;

    case TASK_EVERY_FIVE_MINUTE:
    {
      #ifdef USE_NETWORK_MDNS
        WiFi_Mdns_StartOrRestart();
      #endif
    }
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    case TASK_NETWORK_CONNECTED__WIFI:
    {
      #ifdef USE_NETWORK_MDNS
        WiFi_Mdns_StartOrRestart();
      #endif
    }
    break;

    /************
     * MQTT SECTION * 
    *******************/
    // For Networks, telemetry shows connection status
  }

  return TASKER_RESULT__SUCCESS_ID;
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

// -----------------------------------------------------------------------------
// WiFi IPv4 JSON parsing helpers
// Date Modified: 23Jun26
// -----------------------------------------------------------------------------

static bool ParseIPv4StringToArray(const char* str, uint8_t out[4])
{
  if (!str) { return false; }

  int a = -1;
  int b = -1;
  int c = -1;
  int d = -1;

  if (sscanf(str, "%d.%d.%d.%d", &a, &b, &c, &d) != 4)
  {
    return false;
  }

  if ((a < 0) || (a > 255) ||
      (b < 0) || (b > 255) ||
      (c < 0) || (c > 255) ||
      (d < 0) || (d > 255))
  {
    return false;
  }

  out[0] = (uint8_t)a;
  out[1] = (uint8_t)b;
  out[2] = (uint8_t)c;
  out[3] = (uint8_t)d;

  return true;
}


static bool IPv4ArrayIsZero(const uint8_t ip[4])
{
  return (ip[0] == 0) &&
         (ip[1] == 0) &&
         (ip[2] == 0) &&
         (ip[3] == 0);
}


static void ParseIPv4Config(JsonParserObject obj_ipv4, mWiFi::IPv4Config& ipv4)
{
  JsonParserToken jtok = 0;

  if (jtok = obj_ipv4["Static"])
  {
    ipv4.is_static = jtok.getBool() ? 1 : 0;
  }

  if (jtok = obj_ipv4["IP"])
  {
    ParseIPv4StringToArray(jtok.getStr(), ipv4.ip);
  }

  if (jtok = obj_ipv4["Gateway"])
  {
    ParseIPv4StringToArray(jtok.getStr(), ipv4.gw);
  }

  if (jtok = obj_ipv4["Subnet"])
  {
    ParseIPv4StringToArray(jtok.getStr(), ipv4.sn);
  }

  if (jtok = obj_ipv4["DNS1"])
  {
    ParseIPv4StringToArray(jtok.getStr(), ipv4.dns1);
  }

  if (jtok = obj_ipv4["DNS2"])
  {
    ParseIPv4StringToArray(jtok.getStr(), ipv4.dns2);
  }

  // Debug/default behaviour:
  // If IP, gateway, DNS1, and DNS2 are all zero, treat as DHCP even if Static was
  // accidentally left true. Subnet is ignored here because the struct default is
  // normally 255.255.255.0.
  if (IPv4ArrayIsZero(ipv4.ip)   &&
      IPv4ArrayIsZero(ipv4.gw)   &&
      IPv4ArrayIsZero(ipv4.dns1) &&
      IPv4ArrayIsZero(ipv4.dns2))
  {
    ipv4.is_static = 0;
  }

  ALOG_INF(
    PSTR("WiFi IPv4: static=%d ip=%d.%d.%d.%d gw=%d.%d.%d.%d sn=%d.%d.%d.%d dns1=%d.%d.%d.%d dns2=%d.%d.%d.%d"),
    ipv4.is_static,
    ipv4.ip[0],   ipv4.ip[1],   ipv4.ip[2],   ipv4.ip[3],
    ipv4.gw[0],   ipv4.gw[1],   ipv4.gw[2],   ipv4.gw[3],
    ipv4.sn[0],   ipv4.sn[1],   ipv4.sn[2],   ipv4.sn[3],
    ipv4.dns1[0], ipv4.dns1[1], ipv4.dns1[2], ipv4.dns1[3],
    ipv4.dns2[0], ipv4.dns2[1], ipv4.dns2[2], ipv4.dns2[3]
  );
}


void mWiFi::parse_JSONCommand(JsonParserObject obj)
{
  JsonParserToken jtok = 0;

  JsonParserObject obj_wifi = obj["WiFi"];
  if (!obj_wifi)
  {
    return;
  }


  // -------------------------------------------------------------------------
  // WiFi Mode
  // -------------------------------------------------------------------------
  JsonParserObject obj_mode = obj_wifi["Mode"];
  if (obj_mode)
  {
    if (jtok = obj_mode["APAlwaysOn"])
    {
      config.softap.always_on = jtok.getBool();
      ALOG_INF(PSTR(D_LOG_WIFI "APAlwaysOn=%u"),config.softap.always_on);
    }
  }


  // -------------------------------------------------------------------------
  // Station
  // -------------------------------------------------------------------------
  JsonParserObject obj_station = obj_wifi["Station"];
  if (obj_station)
  {
    JsonParserObject obj_ipv4 = obj_station["IPv4"];
    if (obj_ipv4)
    {
      ParseIPv4Config(obj_ipv4, config.station.ipv4);
    }
  }
}
  


void mWiFi::Task_EverySecond()
{
  // -------------------------------------------------------------------------
  // APAlwaysOn policy
  //
  // Keep the SoftAP present independently of STA connection state.
  // WiFi2_Ap_EnsureStarted() is idempotent, so this also repairs the AP if
  // something unexpectedly changes radio mode.
  // -------------------------------------------------------------------------
  if (config.softap.enabled && config.softap.always_on)
  {
    WiFi2_Ap_EnsureStarted();
  }


  // -------------------------------------------------------------------------
  // Initial connection/startup phase completed
  // -------------------------------------------------------------------------
  if (connection.seconds_to_wait_for_fresh_connection_attempt == -1)
  {
    const auto mode_now = WiFi.getMode();

    const bool ap_only_now = (mode_now == WIFI_AP);

    // AP+STA is NOT AP-only, therefore normal STA maintenance continues.
    if (!ap_only_now)
    {
      WiFi_Sta_Maintain_Periodic();
    }

    return;
  }


  // -------------------------------------------------------------------------
  // Initial delayed start
  // -------------------------------------------------------------------------
  if (connection.seconds_to_wait_for_fresh_connection_attempt > 0)
  {
    connection.seconds_to_wait_for_fresh_connection_attempt--;
    return;
  }


  // -------------------------------------------------------------------------
  // Fresh network start
  // -------------------------------------------------------------------------
  bool ok = false;

  if (WiFi2_HasAnyStaProfileConfigured())
  {
    // If APAlwaysOn is active, AP was ensured above and the STA connection
    // function will preserve it by using WIFI_AP_STA.
    WiFi_Sta_Connect_Start();

    // STA connect is asynchronous.
    ok = true;
  }
  else
  {
    // No station profiles -> AP-only fallback.
    ok = WiFi2_Ap_EnsureStarted();
  }


  // -------------------------------------------------------------------------
  // Initial attempt complete
  // -------------------------------------------------------------------------
  connection.seconds_to_wait_for_fresh_connection_attempt = ok ? -1 : 15;


  // -------------------------------------------------------------------------
  // Maintain STA unless genuinely AP-only
  // -------------------------------------------------------------------------
  const auto mode_now = WiFi.getMode();

  const bool ap_only_now =
    (mode_now == WIFI_AP);

  if (!ap_only_now)
  {
    WiFi_Sta_Maintain_Periodic();
  }
}




void mWiFi::Init_Preload_Wifi2_Settings()
{
  ALOG_DBG(PSTR(D_LOG_WIFI "%s|%d"), __FILE__, __LINE__);

  WiFi_Config_Clear();
  WiFi_Config_LoadLegacyDefaults();
}


void mWiFi::WiFi_Config_Clear(void)
{
  memset(&config, 0, sizeof(config));

  // -------------------------------------------------------------------------
  // Station
  // -------------------------------------------------------------------------
  config.station.enabled = 1;
  config.station.profile_count = 0;
  config.station.active_profile = 0;

  config.station.ipv4.is_static = 0;

  config.station.ipv4.ip[0]   = 0;
  config.station.ipv4.ip[1]   = 0;
  config.station.ipv4.ip[2]   = 0;
  config.station.ipv4.ip[3]   = 0;

  config.station.ipv4.gw[0]   = 0;
  config.station.ipv4.gw[1]   = 0;
  config.station.ipv4.gw[2]   = 0;
  config.station.ipv4.gw[3]   = 0;

  config.station.ipv4.sn[0]   = 255;
  config.station.ipv4.sn[1]   = 255;
  config.station.ipv4.sn[2]   = 255;
  config.station.ipv4.sn[3]   = 0;

  config.station.ipv4.dns1[0] = 0;
  config.station.ipv4.dns1[1] = 0;
  config.station.ipv4.dns1[2] = 0;
  config.station.ipv4.dns1[3] = 0;

  config.station.ipv4.dns2[0] = 0;
  config.station.ipv4.dns2[1] = 0;
  config.station.ipv4.dns2[2] = 0;
  config.station.ipv4.dns2[3] = 0;


  // -------------------------------------------------------------------------
  // SoftAP
  // -------------------------------------------------------------------------
  config.softap.enabled = 1;
  config.softap.always_on = 0;

  config.softap.channel = 1;
  config.softap.hidden = 0;
}


void mWiFi::WiFi_Config_LoadLegacyDefaults(void)
{
#ifndef ENABLE_DEVFEATURE_WIFI__FORCE_SOFTAP_MODE_BY_BLOCKING_SSIDS

  snprintf(config.station.profiles[0].id,   sizeof(config.station.profiles[0].id),   "%s", "home");
  snprintf(config.station.profiles[0].ssid, sizeof(config.station.profiles[0].ssid), "%s", STA_SSID1);
  snprintf(config.station.profiles[0].pass, sizeof(config.station.profiles[0].pass), "%s", STA_PASS1);
  config.station.profiles[0].priority = 10;


  snprintf(config.station.profiles[1].id,   sizeof(config.station.profiles[1].id),   "%s", "backup");
  snprintf(config.station.profiles[1].ssid, sizeof(config.station.profiles[1].ssid), "%s", STA_SSID2);
  snprintf(config.station.profiles[1].pass, sizeof(config.station.profiles[1].pass), "%s", STA_PASS2);
  config.station.profiles[1].priority = 20;


  snprintf(config.station.profiles[2].id,   sizeof(config.station.profiles[2].id),   "%s", "mobile");
  snprintf(config.station.profiles[2].ssid, sizeof(config.station.profiles[2].ssid), "%s", STA_SSID3);
  snprintf(config.station.profiles[2].pass, sizeof(config.station.profiles[2].pass), "%s", STA_PASS3);
  config.station.profiles[2].priority = 30;

  config.station.profile_count = 0;

  for (uint8_t profile_i = 0; profile_i < WIFI_MAXIMUM_CONNECTIONS; profile_i++)
  {
    if (config.station.profiles[profile_i].ssid[0] != '\0')
    {
      config.station.profile_count++;
    }
  }

#endif


#ifdef SOFTAP_SSID
  snprintf(config.softap.ssid, sizeof(config.softap.ssid), "%s", SOFTAP_SSID);
#else
  snprintf(config.softap.ssid, sizeof(config.softap.ssid), "%s", "PulSar-Setup");
#endif

#ifdef SOFTAP_PASSWORD
  snprintf(config.softap.pass, sizeof(config.softap.pass), "%s", SOFTAP_PASSWORD);
#else
  config.softap.pass[0] = '\0';
#endif

}

#endif // USE_MODULE_NETWORK_WIFI