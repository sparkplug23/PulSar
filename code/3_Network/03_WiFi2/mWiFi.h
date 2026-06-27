#ifndef _MWIFI_H
#define _MWIFI_H

#define D_UNIQUE_MODULE_NETWORK_WIFI_ID  3003 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_WIFI

#ifdef ESP32
  #include <WiFi.h>
  #include <ESPmDNS.h>
  #include "2_CoreSystem/06_Support/mSupport.h"
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  // #include <ESP8266mDNS.h>
#endif

#ifdef ESP8266
  #include "2_CoreSystem/06_Support/SupportESP8266.h"
  #define SupportHardware SupportESP8266
#endif
#ifdef ESP32
  #include "2_CoreSystem/06_Support/SupportESP32.h"
  #define mSupportHardware SupportESP32
#endif
#include <DNSServer.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mWiFi :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mWiFi(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};
       
    static constexpr const char* PM_MODULE_NETWORK_WIFI_CTR = D_MODULE_NETWORK_WIFI_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_WIFI_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_WIFI_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);
   
    void WiFi_Sta_ProfileIndex_Connect(uint8_t ap_index);
    int8_t GetRSSdBm();
    uint8_t GetRSSPercentage();

    const uint8_t WIFI_CHECK_SEC = 20;         // seconds

    struct WiFiProfile
    {
      char    id[24] = {0};
      char    ssid[33] = {0};
      char    pass[65] = {0};
      uint8_t priority = 0;
      uint8_t ssid_hidden = 0;
      uint8_t has_bssid = 0;
      uint8_t bssid[6] = {0};
    };


    struct IPv4Config
    {
      uint8_t is_static = 0;
      uint8_t ip[4]   = {0,0,0,0};
      uint8_t gw[4]   = {0,0,0,0};
      uint8_t sn[4]   = {255,255,255,0};
      uint8_t dns1[4] = {0,0,0,0};
      uint8_t dns2[4] = {0,0,0,0};
    };


    struct WiFiStationConfig
    {
      uint8_t enabled = 1;
      WiFiProfile profiles[WIFI_MAXIMUM_CONNECTIONS];
      uint8_t profile_count = 0;
      uint8_t active_profile = 0;
      IPv4Config ipv4;
    };


    struct WiFiSoftApConfig
    {
      uint8_t enabled = 1;
      char ssid[33] = {0};
      char pass[65] = {0};
      uint8_t channel = 1;
      uint8_t hidden = 0;
    };

    struct WiFiModuleConfig
    {
      WiFiStationConfig station;
      WiFiSoftApConfig softap;
    };

    WiFiModuleConfig config;
    void WiFi_Config_Clear(void);
    void WiFi_Config_LoadLegacyDefaults(void);
    
    uint8_t loglevel_with_connection_status = 10; // When connected

    bool     s_wifi2_scanned_on_boot = false;
    bool     s_wifi2_scanned_on_this_outage = false;

    
    bool WiFi_Link_IsIpRoutable();

    #ifndef WIFI_RSSI_THRESHOLD
     #define WIFI_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
    #endif
    // #ifndef WIFI_RESCAN_MINUTES
    //  #define WIFI_RESCAN_MINUTES     44         // Number of minutes between wifi network rescan
    // #endif
    #ifndef WIFI_RSSI_THRESHOLD
     #define WIFI_RSSI_THRESHOLD 10 // dB
    #endif
    #ifndef WIFI_OUTAGE_RESCAN_SECONDS
     #define WIFI_OUTAGE_RESCAN_SECONDS (10UL * 60UL) // 10 minutes
    #endif

    struct WIFI_CONNECTION{
      uint32_t last_event = 0;       // Last wifi connection event
      uint32_t downtime = 0;         // Wifi down duration
      uint16_t link_count = 0;       // Number of wifi re-connect
      uint8_t counter = 0;
      uint8_t config_type = 0;
      uint8_t bssid[6];
      uint8_t fConnected = false;
      uint8_t fReconnect = false;
      int8_t seconds_to_wait_for_fresh_connection_attempt = 0;
    }connection;

    uint8_t WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured(void) const;
    uint8_t WiFi_Sta_SelectProfileIndex_WithScanPreference(bool force_scan);
    bool    WiFi_Sta_ShouldScanNow_OnBootOrOutage(void);
    void    WiFi_Sta_OnConnected_ResetOutageScanFlags(void);

    void WiFi_Mdns_StartOrRestart(void);
    void WiFi_Mdns_DiscoverMqttBroker(void);

    bool WiFi_Dns_ResolveHostname(const char* aHostname, IPAddress& aResult);

    
    struct {
      uint8_t begun = 0;                  // mDNS active
    } Mdns;    

    #if defined(USE_NETWORK_MDNS) && defined(ESP8266) //Not needed with esp32 mdns
    void WiFi_Mdns_Tick(void);
    #endif

    bool WiFi2_HasAnyStaProfileConfigured(void) const;
    uint8_t WiFi2_GetFirstConfiguredProfileIndex(void) const;

    void WiFi2_Sta_EnsureConnecting(void);
    void WiFi2_Sta_Connected_Enter(void);
    void WiFi2_Sta_Disconnected_Enter(void);

    void Init_Preload_Wifi2_Settings();

    bool WiFi2_Ap_EnsureStarted(void);

    static IPAddress IPv4ArrayToIP(const uint8_t a[4]);

    void Task_EverySecond();

    // ------------------------------------------------------------------
    // WiFi mode helpers (AP/STA/AP+STA)
    // ------------------------------------------------------------------
    bool WiFi_IsAPMode(void) ;
    bool WiFi_IsSTAMode(void) ;
    bool WiFi_IsAPSTAMode(void);

    // ------------------------------------------------------------------
    // Connectivity helpers
    //   - Local: device is reachable on some WiFi interface (AP or STA w/ IP)
    //   - External: safe for MQTT/NTP etc. (STA connected + routable gateway)
    // ------------------------------------------------------------------
    bool WiFi_HasLocalConnectivity(void);
    bool WiFi_HasExternalConnectivity(void);

    DNSServer dnsServer;
    bool      dnsServerRunning = false;

    void WiFi2_Ap_Dns_Start(void);
    void WiFi2_Ap_Dns_Stop(void);
    void WiFi2_Ap_Dns_Tick(void);

    void WiFi_Sta_Maintain_Periodic();
    void WiFi_Sta_State_Set(uint8_t state);
    void WiFi_Sta_Connect_Start(void);
    void WiFi_Radio_Shutdown(bool option = false);

    const char* GetWiFiStatusCtr(void);

    #ifdef ESP32
    static inline float WiFiPower_To_dBm(wifi_power_t p){
      return (float)p * 0.25f; // ie 19.5dBm = 78enum -> 78*0.25f => 19.5
    }
    #endif


};

#endif

#endif //   #ifdef USE_MODULE_NETWORK_WIFI