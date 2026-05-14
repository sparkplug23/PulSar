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
      // ALOG_INF(PSTR("WL_CONNECTED=%d IP=%s"),
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


void mWiFi::Task_EverySecond()
{
  // If already started chosen mode, do nothing here.
  if (connection.seconds_to_wait_for_fresh_connection_attempt == -1)
  {
    const auto mode_now = WiFi.getMode();
    const bool ap_only_now = (mode_now == WIFI_AP);

    if (!ap_only_now)
    {
      WiFi_Sta_Maintain_Periodic();
    }
    return;
  }

  // Count down to next attempt
  if (connection.seconds_to_wait_for_fresh_connection_attempt > 0)
  {
    connection.seconds_to_wait_for_fresh_connection_attempt--;
    return;
  }

  // When we hit zero, attempt a "fresh start" of either STA or AP.
  bool ok = false;

  if (WiFi2_HasAnyStaProfileConfigured())
  {
    WiFi_Sta_Connect_Start();
    ok = true; // async initiation treated as success
  }
  else
  {
    ok = WiFi2_Ap_EnsureStarted();
  }

  // Retry delay on failure
  connection.seconds_to_wait_for_fresh_connection_attempt = ok ? -1 : 15;

  // Re-sample mode AFTER any start attempt
  const auto mode_now = WiFi.getMode();
  const bool ap_only_now = (mode_now == WIFI_AP);

  // Maintain STA unless we are AP-only
  if (!ap_only_now)
  {
    WiFi_Sta_Maintain_Periodic();
  }
}




void mWiFi::Init_Preload_Wifi2_Settings()
{
  ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"), __FILE__, __LINE__);

  WiFi_Config_Clear();
  WiFi_Config_LoadLegacyDefaults();
}


void mWiFi::WiFi_Config_Clear(void)
{
  memset(&config, 0, sizeof(config));

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

  config.softap.enabled = 1;
  config.softap.channel = 1;
  config.softap.hidden = 0;
}


void mWiFi::WiFi_Config_LoadLegacyDefaults(void)
{
#ifndef ENABLE_DEVFEATURE_WIFI__FORCE_SOFTAP_MODE_BY_BLOCKING_SSIDS

  snprintf(config.station.profiles[0].id,
           sizeof(config.station.profiles[0].id),
           "%s",
           "home");

  snprintf(config.station.profiles[0].ssid,
           sizeof(config.station.profiles[0].ssid),
           "%s",
           STA_SSID1);

  snprintf(config.station.profiles[0].pass,
           sizeof(config.station.profiles[0].pass),
           "%s",
           STA_PASS1);

  config.station.profiles[0].priority = 10;


  snprintf(config.station.profiles[1].id,
           sizeof(config.station.profiles[1].id),
           "%s",
           "backup");

  snprintf(config.station.profiles[1].ssid,
           sizeof(config.station.profiles[1].ssid),
           "%s",
           STA_SSID2);

  snprintf(config.station.profiles[1].pass,
           sizeof(config.station.profiles[1].pass),
           "%s",
           STA_PASS2);

  config.station.profiles[1].priority = 20;


  snprintf(config.station.profiles[2].id,
           sizeof(config.station.profiles[2].id),
           "%s",
           "mobile");

  snprintf(config.station.profiles[2].ssid,
           sizeof(config.station.profiles[2].ssid),
           "%s",
           STA_SSID3);

  snprintf(config.station.profiles[2].pass,
           sizeof(config.station.profiles[2].pass),
           "%s",
           STA_PASS3);

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
  snprintf(config.softap.ssid,
           sizeof(config.softap.ssid),
           "%s",
           SOFTAP_SSID);
#else
  snprintf(config.softap.ssid,
           sizeof(config.softap.ssid),
           "%s",
           "PulSar-Setup");
#endif

#ifdef SOFTAP_PASSWORD
  snprintf(config.softap.pass,
           sizeof(config.softap.pass),
           "%s",
           SOFTAP_PASSWORD);
#else
  config.softap.pass[0] = '\0';
#endif
}

#endif // USE_MODULE_NETWORK_WIFI