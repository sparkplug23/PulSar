#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI


bool mWiFi::WiFi2_Ap_EnsureStarted(void)
{
  ALOG_DBG(PSTR(D_LOG_WIFI "WiFi2_Ap_EnsureStarted"));

  if (!config.softap.enabled)
  {
    return false;
  }


  // -------------------------------------------------------------------------
  // Determine required radio mode.
  //
  // APAlwaysOn + configured STA:
  //      WIFI_AP_STA
  //
  // AP without STA:
  //      WIFI_AP
  // -------------------------------------------------------------------------
  const bool sta_required = config.softap.always_on && WiFi2_HasAnyStaProfileConfigured();

  const wifi_mode_t required_mode = sta_required ? WIFI_AP_STA : WIFI_AP;


  // -------------------------------------------------------------------------
  // If AP is already running, do not recreate it.
  //
  // We may still need to promote WIFI_AP -> WIFI_AP_STA.
  // -------------------------------------------------------------------------
  const auto mode_now = WiFi.getMode();

  const bool ap_running =
    (mode_now == WIFI_AP) ||
    (mode_now == WIFI_AP_STA);

  if (ap_running)
  {
    if ((required_mode == WIFI_AP_STA) &&
        (mode_now != WIFI_AP_STA))
    {
      WiFi.mode(WIFI_AP_STA);

      ALOG_INF(
        PSTR(D_LOG_WIFI "SoftAP retained; WiFi mode changed to AP+STA")
      );
    }

    return true;
  }


  // -------------------------------------------------------------------------
  // Starting AP while STA must remain available requires AP+STA mode.
  // -------------------------------------------------------------------------
  WiFi.mode(required_mode);


  // -------------------------------------------------------------------------
  // AP network
  // -------------------------------------------------------------------------
  WiFi.softAPConfig(
    IPAddress(4,3,2,1),
    IPAddress(4,3,2,1),
    IPAddress(255,255,255,0)
  );


  const char* ap_ssid = config.softap.ssid;
  const char* ap_pass = config.softap.pass;

  bool ok = false;


  // -------------------------------------------------------------------------
  // Empty password -> open AP
  // -------------------------------------------------------------------------
  if ((ap_pass == nullptr) || (ap_pass[0] == '\0'))
  {
    ok = WiFi.softAP(ap_ssid,nullptr,config.softap.channel,config.softap.hidden);
  }
  else
  {
    const size_t pass_len = strlen(ap_pass);

    // ESP32 WPA password must be at least 8 characters
    if (pass_len < 8)
    {
      ALOG_ERR(PSTR(D_LOG_WIFI "SoftAP password too short (<8), starting OPEN AP"));
      ok = WiFi.softAP(ap_ssid,nullptr,config.softap.channel,config.softap.hidden);
    }
    else
    {
      ok = WiFi.softAP(ap_ssid,ap_pass,config.softap.channel,config.softap.hidden);
    }
  }


  if (!ok)
  {
    ALOG_ERR(PSTR(D_LOG_WIFI "SoftAP start failed"));
    return false;
  }


  ALOG_INF(PSTR(D_LOG_WIFI "SoftAP started | SSID=%s | IP=%s | mode=%s"),ap_ssid,WiFi.softAPIP().toString().c_str(),(required_mode == WIFI_AP_STA) ? "AP+STA" : "AP");


  // -------------------------------------------------------------------------
  // Captive portal
  // -------------------------------------------------------------------------
  #ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
    WiFi2_Ap_Dns_Start();
  #endif


  // TEMPORARY bridge: start webserver/pages
  tkr->Tasker_Interface(TASK_NETWORK_CONNECTED__WIFI);

  SET_SYSTEM_LED__AP_MODE(true);

  return true;
}



#endif // USE_MODULE_NETWORK_WIFI