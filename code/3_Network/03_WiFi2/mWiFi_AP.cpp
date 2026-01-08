#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI

#ifdef ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2


bool mWiFi::WiFi2_Ap_EnsureStarted(void)
{
  ALOG_INF(PSTR(D_LOG_WIFI "WiFi2_Ap_EnsureStarted"));

  // Start minimal SoftAP (no dependency on initAP)
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(IPAddress(4,3,2,1), IPAddress(4,3,2,1), IPAddress(255,255,255,0));

  const bool ok = WiFi.softAP("PulSar-Setup", nullptr);
  if (!ok)
  {
    return false;
  }

  // Captive portal support (DNS hijack) is OPTIONAL
  #ifdef ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
    WiFi2_Ap_Dns_Start();
  #endif

  // TEMPORARY bridge: start webserver/pages
  tkr->Tasker_Interface(TASK_NETWORK_CONNECTED);

  return true;
}





#endif // ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2

#endif // USE_MODULE_NETWORK_WIFI