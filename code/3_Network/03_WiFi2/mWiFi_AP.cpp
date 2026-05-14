#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI


bool mWiFi::WiFi2_Ap_EnsureStarted(void)
{
  ALOG_INF(PSTR(D_LOG_WIFI "WiFi2_Ap_EnsureStarted"));

  // Start minimal SoftAP (no dependency on initAP)
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(4,3,2,1), IPAddress(4,3,2,1), IPAddress(255,255,255,0));

  const char* ap_ssid = SOFTAP_SSID;
  const char* ap_pass = SOFTAP_PASSWORD;

  bool ok = false;

  // Empty password => open AP
  if ((ap_pass == nullptr) || (ap_pass[0] == '\0'))
  {
    ok = WiFi.softAP(ap_ssid); // open
  }
  else
  {
    // ESP32 requires >= 8 chars for WPA2
    const size_t pass_len = strlen(ap_pass);
    if (pass_len < 8)
    {
      ALOG_ERR(PSTR(D_LOG_WIFI "SoftAP password too short (<8), starting OPEN AP"));
      ok = WiFi.softAP(ap_ssid); // open fallback
    }
    else
    {
      ok = WiFi.softAP(ap_ssid, ap_pass); // protected
    }
  }

  if (!ok){
    return false;
  }

  // Captive portal support (DNS hijack) is OPTIONAL
  #ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
    WiFi2_Ap_Dns_Start();
  #endif

  // TEMPORARY bridge: start webserver/pages
  tkr->Tasker_Interface(TASK_NETWORK_CONNECTED__WIFI);

  SET_SYSTEM_LED__AP_MODE(true);

  return true;
}



#endif // USE_MODULE_NETWORK_WIFI