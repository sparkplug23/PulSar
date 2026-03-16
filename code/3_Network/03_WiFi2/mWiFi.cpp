#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI

#ifdef ESP8266
extern "C" {
 #include "user_interface.h"
}
#endif


int8_t mWiFi::Tasker(uint8_t function, JsonParserObject obj)
{

  // return 0;


  switch (function)
  {
    case TASK_INIT:
    {
      Init_Preload_Wifi2_Settings();

      // Simple delayed start for both STA/AP.
      // (Later you will replace 10 with chip-id derived jitter.)
      connection.seconds_to_wait_for_fresh_connection_attempt = 10;
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
ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
  memset(tkr_set->Settings.network.wifi[0].ssid, 0, sizeof(tkr_set->Settings.network.wifi[0].ssid));
  memset(tkr_set->Settings.network.wifi[0].pass, 0, sizeof(tkr_set->Settings.network.wifi[0].pass));
  memset(tkr_set->Settings.network.wifi[1].ssid, 0, sizeof(tkr_set->Settings.network.wifi[1].ssid));
  memset(tkr_set->Settings.network.wifi[1].pass, 0, sizeof(tkr_set->Settings.network.wifi[1].pass));
  memset(tkr_set->Settings.network.wifi[2].ssid, 0, sizeof(tkr_set->Settings.network.wifi[2].ssid));
  memset(tkr_set->Settings.network.wifi[2].pass, 0, sizeof(tkr_set->Settings.network.wifi[2].pass));

  #ifndef ENABLE_DEVFEATURE_WIFI__FORCE_SOFTAP_MODE_BY_BLOCKING_SSIDS
  // Slot 0
  snprintf(tkr_set->Settings.network.wifi[0].ssid,
           sizeof(tkr_set->Settings.network.wifi[0].ssid),
           "%s", STA_SSID1);

  snprintf(tkr_set->Settings.network.wifi[0].pass,
           sizeof(tkr_set->Settings.network.wifi[0].pass),
           "%s", STA_PASS1);

  // Slot 1
  snprintf(tkr_set->Settings.network.wifi[1].ssid,
           sizeof(tkr_set->Settings.network.wifi[1].ssid),
           "%s", STA_SSID2);

  snprintf(tkr_set->Settings.network.wifi[1].pass,
           sizeof(tkr_set->Settings.network.wifi[1].pass),
           "%s", STA_PASS2);
  // Slot 2
  snprintf(tkr_set->Settings.network.wifi[2].ssid,
           sizeof(tkr_set->Settings.network.wifi[2].ssid),
           "%s", STA_SSID3);

  snprintf(tkr_set->Settings.network.wifi[2].pass,
           sizeof(tkr_set->Settings.network.wifi[2].pass),
           "%s", STA_PASS3);

  #endif
}

#endif // USE_MODULE_NETWORK_WIFI