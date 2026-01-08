#include "mWiFi.h"


#ifdef USE_MODULE_NETWORK_WIFI
#ifdef ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2


bool mWiFi::WiFi_IsAPMode(void)
{
  wifi_mode_t mode = WiFi.getMode();
  return (mode == WIFI_AP);
}

bool mWiFi::WiFi_IsSTAMode(void)
{
  wifi_mode_t mode = WiFi.getMode();
  return (mode == WIFI_STA);
}

bool mWiFi::WiFi_IsAPSTAMode(void)
{
  wifi_mode_t mode = WiFi.getMode();
  return (mode == WIFI_AP_STA);
}

bool mWiFi::WiFi_HasLocalConnectivity(void)
{
  // Local reachability exists if:
  //  - AP is active (clients can connect to SoftAP IP), OR
  //  - STA is connected and has a non-zero IP.
  wifi_mode_t mode = WiFi.getMode();

  if ((mode == WIFI_AP) || (mode == WIFI_AP_STA))
  {
    // AP-side local connectivity; softAPIP is valid when AP is started.
    return true;
  }

  if ((mode == WIFI_STA) || (mode == WIFI_AP_STA))
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      IPAddress ip = WiFi.localIP();
      return (ip[0] != 0); // 0.0.0.0 means no DHCP address yet
    }
  }

  return false;
}

bool mWiFi::WiFi_HasExternalConnectivity(void)
{
  // External connectivity means "safe for MQTT/NTP":
  // STA connected AND your routable check passes (gateway/upstream usable).
  wifi_mode_t mode = WiFi.getMode();
  if ((mode != WIFI_STA) && (mode != WIFI_AP_STA))
  {
    return false;
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    return false;
  }

  return WiFi_Link_IsIpRoutable();
}


bool mWiFi::WiFi_Link_IsIpRoutable()
{

  if ((WL_CONNECTED == WiFi.status()) && 
      (static_cast<uint32_t>(WiFi.localIP()) != 0) &&
      (WiFi.localIP().toString()!="(IP unset)")
  ) 
  {
    return true;  
  }
  
  DEBUG_LINE_HERE;
  Serial.println(WiFi.status());

  return false;

}




/// SECTION: DNS /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mWiFi::WiFi2_Ap_Dns_Start(void)
{
  if (dnsServerRunning) return;

  // Redirect all DNS queries to the AP IP
  dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  dnsServer.start(53, "*", WiFi.softAPIP());

  dnsServerRunning = true;
  ALOG_INF(PSTR(D_LOG_WIFI "DNS captive portal started @ %s"), WiFi.softAPIP().toString().c_str());
}

void mWiFi::WiFi2_Ap_Dns_Stop(void)
{
  if (!dnsServerRunning) return;

  dnsServer.stop();
  dnsServerRunning = false;
  ALOG_INF(PSTR(D_LOG_WIFI "DNS captive portal stopped"));
}

void mWiFi::WiFi2_Ap_Dns_Tick(void)
{
  if (!dnsServerRunning) return;
  dnsServer.processNextRequest();
}


bool mWiFi::WiFi_Dns_ResolveHostname(const char* aHostname, IPAddress& aResult) {ALOG_INF(PSTR(D_LOG_WIFI "%s|%d"),__FILE__,__LINE__);
#ifdef USE_IPV6
#if ESP_IDF_VERSION_MAJOR >= 5
  // try converting directly to IP
  if (aResult.fromString(aHostname)) {
    return true;   // we're done
  }
#endif
#endif // USE_IPV6

  tkr_set->Settings.dns_timeout = 1000;

  uint32_t dns_start = millis();
  bool success = WiFi.hostByName(aHostname, aResult);//, tkr_set->Settings.dns_timeout);
  uint32_t dns_end = millis();
  if (success) {
    // Host name resolved
    if (0xFFFFFFFF != (uint32_t)aResult) {
      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_WIFI "DNS resolved '%s' (%s) in %i ms"), aHostname, aResult.toString().c_str(), dns_end - dns_start);
      return true;
    }
  }
  ALOG_DBG(PSTR(D_LOG_WIFI "DNS failed for %s after %i ms"), aHostname, dns_end - dns_start);
  return false;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// In 1dB increments
int8_t mWiFi::GetRSSdBm(){
  return WiFi.RSSI();
}

uint8_t mWiFi::GetRSSPercentage(){
  return constrain(map(GetRSSdBm(),-40,-100,100,0),0,100);
}




const char* mWiFi::GetWiFiStatusCtr(void){                          // UNSTABLE CODE, RETURNING NULL!!

//NULL

  switch(WiFi.status()){
    case WL_IDLE_STATUS      : return PSTR("WL_IDLE_STATUS");
    case WL_NO_SSID_AVAIL    : return PSTR("WL_NO_SSID_AVAIL");
    case WL_SCAN_COMPLETED   : return PSTR("WL_SCAN_COMPLETED");
    case WL_CONNECTED        : return PSTR("WL_CONNECTED");
    case WL_CONNECT_FAILED   : return PSTR("WL_CONNECT_FAILED");
    case WL_CONNECTION_LOST  : return PSTR("WL_CONNECTION_LOST");
    case WL_DISCONNECTED     : return PSTR("WL_DISCONNECTED");
    default: PSTR("Unknown");
  }
}

const char* mWiFi::GetWiFiConfigTypeCtr(void){

  //NULL 

  switch(connection.config_type){
    case WIFI_RESTART      : return PSTR("WIFI_RESTART");
    case WIFI_SMARTCONFIG    : return PSTR("WIFI_SMARTCONFIG");
    case WIFI_MANAGER   : return PSTR("WIFI_MANAGER");
    case WIFI_WPSCONFIG        : return PSTR("WIFI_WPSCONFIG");
    case WIFI_RETRY   : return PSTR("WIFI_RETRY");
    case WIFI_WAIT  : return PSTR("WIFI_WAIT");
    case WIFI_SERIAL     : return PSTR("WIFI_SERIAL");
    case WIFI_MANAGER_RESET_ONLY  : return PSTR("WIFI_MANAGER_RESET_ONLY");
    case MAX_WIFI_OPTION     : return PSTR("MAX_WIFI_OPTION");
    default: PSTR("Unknown");
  }
}





#endif
#endif