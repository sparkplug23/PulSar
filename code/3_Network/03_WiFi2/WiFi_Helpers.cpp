#include "mWiFi.h"


#ifdef USE_MODULE_NETWORK_WIFI

bool mWiFi::WiFi_IsAPMode(void)
{
  const auto mode = WiFi.getMode();
  return (mode == WIFI_AP);
}


bool mWiFi::WiFi_IsSTAMode(void)
{
  const auto mode = WiFi.getMode();
  return (mode == WIFI_STA);
}


bool mWiFi::WiFi_IsAPSTAMode(void)
{
  const auto mode = WiFi.getMode();
  return (mode == WIFI_AP_STA);
}


bool mWiFi::WiFi_HasLocalConnectivity(void)
{
  // Local reachability exists if:
  //  - AP is active (clients can connect to SoftAP IP), OR
  //  - STA is connected and has a non-zero IP.
  const auto mode = WiFi.getMode();

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
  const auto mode = WiFi.getMode();

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
      ALOG_DBG(PSTR(D_LOG_WIFI "DNS resolved '%s' (%s) in %i ms"), aHostname, aResult.toString().c_str(), dns_end - dns_start);
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



const char* mWiFi::GetWiFiStatusCtr(void)
{
  switch (WiFi.status())
  {
    case WL_IDLE_STATUS     : return PSTR("WL_IDLE_STATUS");
    case WL_NO_SSID_AVAIL   : return PSTR("WL_NO_SSID_AVAIL");
    case WL_SCAN_COMPLETED  : return PSTR("WL_SCAN_COMPLETED");
    case WL_CONNECTED       : return PSTR("WL_CONNECTED");
    case WL_CONNECT_FAILED  : return PSTR("WL_CONNECT_FAILED");
    case WL_CONNECTION_LOST : return PSTR("WL_CONNECTION_LOST");
    case WL_DISCONNECTED    : return PSTR("WL_DISCONNECTED");
    default                 : return PSTR("Unknown");
  }
}

void mWiFi::parse_JSONCommand(JsonParserObject obj){};
   



void mWiFi::Init(void){
  
//  AddLog(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_RELAYS D_DEBUG_FUNCTION "\"%s\""),"mRelays::init");

}





void mWiFi::WiFi_Radio_Shutdown(bool option)
{
  // option = false - Legacy disconnect also used by DeepSleep
  // option = true  - Disconnect with SDK wifi calibrate sector erase when WIFI_FORCE_RF_CAL_ERASE enabled
  delay(100);                 // Allow time for message xfer - disabled v6.1.0b

// #ifdef USE_EMULATION
//   UdpDisconnect();
//   delay(100);                 // Flush anything in the network buffers.
// #endif  // USE_EMULATION

  // if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
  //   MqttDisconnect();
  //   delay(100);               // Flush anything in the network buffers.
  // }

// #ifdef WIFI_FORCE_RF_CAL_ERASE
//   if (option) {
//     WiFi.disconnect(false);   // Disconnect wifi
//     SettingsErase(4);         // Delete SDK wifi config and calibrate data
//   } else
// #endif  // WIFI_FORCE_RF_CAL_ERASE
  // {
    // Enable from 6.0.0a until 6.1.0a - disabled due to possible cause of bad wifi connect on core 2.3.0
    // Re-enabled from 6.3.0.7 with ESP.restart replaced by ESP.reset
    // Courtesy of EspEasy
    // WiFi.persistent(true);    // use SDK storage of SSID/WPA parameters
    ETS_UART_INTR_DISABLE();

  //tmp fix
#ifdef ESP8266
    wifi_station_disconnect();  // this will store empty ssid/wpa into sdk storage
    #else

    // erase ap: empty ssid, ...
    WiFi.disconnect(true, true); //inside compat
    #endif
    ETS_UART_INTR_ENABLE();
    // WiFi.persistent(false);   // Do not use SDK storage of SSID/WPA parameters
  // }
  delay(100);                 // Flush anything in the network buffers.

}

/*********************************************************************************************\
 * MDNS
\*********************************************************************************************/

#ifdef USE_DISCOVERY

void mWiFi::WiFi_Mdns_StartOrRestart(void)
{
  if (Mdns.begun) return;

  const char* originalName = tkr_set->Settings.system_name.device;

  // Define a temporary buffer (max 64 bytes: 63 + null terminator)
  char hostname[64];
  strncpy(hostname, originalName, 63);
  hostname[63] = '\0';

  // Replace invalid underscores with dashes
  for (char* p = hostname; *p; ++p)
  {
    if (*p == '_') *p = '-';
  }

  // Platform-specific hostname setter
  #if defined(ESP8266)
    WiFi.hostname(hostname);
  #elif defined(ESP32)
    WiFi.setHostname(hostname);
  #endif

  // OTA hostname
  ArduinoOTA.setHostname(hostname);

  // Close existing session to prevent failure
  MDNS.end();

  // Begin with sanitized hostname
  Mdns.begun = (uint8_t)MDNS.begin(hostname);

  ALOG_INF(PSTR(D_LOG_MDNS "%s with %s"),
           (Mdns.begun) ? PSTR(D_INITIALIZED) : PSTR(D_FAILED),
           hostname);

  #if defined(ESP32)
    // Register service
    MDNS.addService("_http", "_tcp", 80);

    String escapedMac = WiFi.macAddress();
    escapedMac.replace(":", "");
    escapedMac.toLowerCase();

    MDNS.addService("http", "tcp", 80);
    MDNS.addService("pulsar", "tcp", 80);
    MDNS.addServiceTxt("pulsar", "tcp", "mac", escapedMac.c_str());
  #endif
}






#ifdef MQTT_HOST_DISCOVERY
void mWiFi::WiFi_Mdns_DiscoverMqttBroker(void)
{
  if (!Mdns.begun) { return; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  ALOG_INF( PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);

  if (n > 0) {
    uint32_t i = 0;            // If the hostname isn't set, use the first record found.
    #ifdef MDNS_HOSTNAME
    for (i = n; i > 0; i--) {  // Search from last to first and use first if not found
      if (!strcmp(MDNS.hostname(i).c_str(), MDNS_HOSTNAME)) {
        break;                 // Stop at matching record
      }
    }
    #endif  // MDNS_HOSTNAME
    // SettingsUpdateText(SET_MQTT_HOST, MDNS.hostname(i).c_str());
    // Settings.mqtt_port = MDNS.port(i);
    // ALOG_INF(PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s," D_PORT " %d"), SettingsText(SET_MQTT_HOST), Settings.mqtt_port);
  }
}
#endif  // MQTT_HOST_DISCOVERY

#ifdef WEBSERVER_HOST_DISCOVERY
void mWiFi::WiFi_Mdns_AdvertiseHttpService(void) {
  if (1 == Mdns.begun) {
    Mdns.begun = 2;
    MDNS.addService("http", "tcp", WEB_PORT);
    MDNS.addServiceTxt("http", "tcp", "devicetype", "tasmota");
  }
}
#endif  // WEBSERVER_HOST_DISCOVERY
#endif  // USE_DISCOVERY



#if defined(USE_NETWORK_MDNS) && defined(ESP8266) //Not needed with esp32 mdns
void mWiFi::WiFi_Mdns_Tick(void) 
{
  MDNS.update();
  if (2 == Mdns.begun) {
    MDNS.update(); // this is basically passpacket like a webserver
   // ALOG_DBM( PSTR(D_LOG_MDNS "MDNS.update"));
  }
}
#endif  // ESP8266


#endif