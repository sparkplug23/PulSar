#ifndef _MWIFI_H
#define _MWIFI_H

#define D_UNIQUE_MODULE_NETWORK_WIFI_ID  3003 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2

// Included in baseconfig
enum WifiConfigOptions {WIFI_RESTART, WIFI_SMARTCONFIG, WIFI_MANAGER, WIFI_WPSCONFIG, WIFI_RETRY, WIFI_WAIT, WIFI_SERIAL, WIFI_MANAGER_RESET_ONLY, MAX_WIFI_OPTION};

#ifdef USE_MODULE_NETWORK_WIFI

// #define ENABLE_WIFI_DEVELOPMENT

#include "2_CoreSystem/mBaseConfig.h"

#ifdef ESP32
#include <esp8266toEsp32.h>
#endif

#ifdef ENABLE_USER_CONFIG_OVERRIDE 
  #include "0_ConfigUser/G1_mUserConfig_Secret.h"
#endif
#include "2_CoreSystem/08_Logging/mLogging.h"

#include "2_CoreSystem/01_Settings/mSettings.h"

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
#include "2_CoreSystem/07_Time/mTime.h"

class mWiFi :
  public mTaskerInterface
{
  public:
    mWiFi(){};
    
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);    

    static constexpr const char* PM_MODULE_NETWORK_WIFI_CTR = D_MODULE_NETWORK_WIFI_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_WIFI_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_WIFI_ID; }

    void parse_JSONCommand(JsonParserObject obj);
   
    void WiFi_Sta_ProfileIndex_Connect(uint8_t ap_index);
    int8_t GetRSSdBm();
    uint8_t GetRSSPercentage();

    // void WifiConnect(void);
    
    const uint8_t WIFI_CONFIG_SEC = 180;       // seconds before restart
    const uint8_t WIFI_CHECK_SEC = 20;         // seconds
    const uint8_t WIFI_RETRY_OFFSET_SEC = 20;  // seconds

    #define D_WIFI_CONFIG_SEC 180
    #define D_WIFI_cONFIG_SEC_FIRST_CONNECT 5

    uint32_t tSavedWiFi;
    uint32_t tSavedWiFiCheckIP;
    uint32_t tSavedWiFiReconnect;  



    WiFiClient* mqtt_client2 = nullptr;
    
    uint16_t wifi_counter_tester = 0;
    
    uint8_t loglevel_with_connection_status = 10; // When connected

    

bool     s_wifi2_scanned_on_boot = false;
bool     s_wifi2_scanned_on_this_outage = false;

    
    bool WiFi_Link_IsIpRoutable();

    #ifndef WIFI_RSSI_THRESHOLD
      #define WIFI_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
    #endif
    #ifndef WIFI_RESCAN_MINUTES
      #define WIFI_RESCAN_MINUTES     44         // Number of minutes between wifi network rescan
    #endif

    // struct wifi_profile_t
    // {
    //   char    ssid[33] = {0};
    //   char    pass[65] = {0};

    //   bool    has_bssid = false;
    //   uint8_t bssid[6]  = {0};

    //   bool    use_static = false;
    //   uint32_t ip   = 0;          // host endian ok internally; document it
    //   uint32_t gw   = 0;
    //   uint32_t sn   = 0x00FFFFFF; // 255.255.255.0 default, per your WLED example
    //   uint32_t dns1 = 0;
    //   uint32_t dns2 = 0;

    //   uint8_t connect_attempt_priority = 0;       // lower = earlier
    //   bool    ssid_hidden   = false;

    // };



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
      uint8_t retry_init = 0;
      uint8_t retry = 0;
      uint8_t max_retry;
      uint8_t status = 0;
      uint8_t config_type = 0;
      uint8_t config_counter = D_WIFI_cONFIG_SEC_FIRST_CONNECT; // IMPORTANT!!
      uint8_t scan_state = 0;
      uint8_t bssid[6];
      uint8_t fConnected = false;
      uint8_t fReconnect = false;

      int8_t seconds_to_wait_for_fresh_connection_attempt = 0;


    }connection;
// WiFi2 scan/select helpers
uint8_t WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured(void) const;
uint8_t WiFi_Sta_SelectProfileIndex_WithScanPreference(bool force_scan);
bool    WiFi_Sta_ShouldScanNow_OnBootOrOutage(void);
void    WiFi_Sta_OnConnected_ResetOutageScanFlags(void);


    void WiFi_Mdns_StartOrRestart(void);
    void WiFi_Mdns_DiscoverMqttBroker(void);
    void WiFi_Mdns_AdvertiseHttpService(void);
    void WiFi_Mdns_Tick(void);

    bool WiFi_Dns_ResolveHostname(const char* aHostname, IPAddress& aResult);

    
    struct {
      uint8_t begun = 0;                  // mDNS active
    } Mdns;

    uint8_t wps_result;

 
    // ----------------------------------------------------------------------------------
// WiFi2 orchestration (uses Settings.network_settings, preserves legacy variables)
// ----------------------------------------------------------------------------------
// void Handle_WiFiConnection(void);          // called every second from Tasker
void WiFi2_Init_FromSettings(void);        // build internal caches once
bool WiFi2_HasAnyStaProfileConfigured(void) const;
uint8_t WiFi2_GetFirstConfiguredProfileIndex(void) const;

void WiFi2_Sta_BeginProfile(uint8_t profile_i);
void WiFi2_Sta_EnsureConnecting(void);
void WiFi2_Sta_Connected_Enter(void);
void WiFi2_Sta_Disconnected_Enter(void);

void Init_Preload_Wifi2_Settings();

bool WiFi2_Ap_EnsureStarted(void);


#ifdef ENABLE_DEVFEATURE_NETWORK__WIFI_DUPLICATED_WRITEBACK
void WiFi2_LegacyWriteback_FromNetworkSettings(void);
#endif

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
bool WiFi_HasLocalConnectivity(void) ;
bool WiFi_HasExternalConnectivity(void) ;



DNSServer dnsServer;
bool      dnsServerRunning = false;

void WiFi2_Ap_Dns_Start(void);
void WiFi2_Ap_Dns_Stop(void);
void WiFi2_Ap_Dns_Tick(void);



void WiFi_Sta_Maintain_Periodic();
//     void WiFi_Sta_Scan_LogSummary();
//     void WiFi_Sta_Connect_ForceRestart();
//     int WifiGetRssiAsQuality(int rssi);
//     bool WiFi_Config_ConnectWindow_Expired(void);
//     #ifndef ESP32
//     void WiFi_Wps_StatusCallback(wps_cb_status status);
//     #endif
//     bool WiFi_Wps_IsComplete(void);
//     bool WiFi_Wps_Start(void);
//     void WiFi_Config_Mode_Set(uint8_t type);
    
//     void WiFi_Sta_Scan_SelectBest_AndBegin();
//     uint16_t WiFi_Link_ReconnectCount_Get();
//     //String WifiDowntime();
    void WiFi_Sta_State_Set(uint8_t state);
//     void WiFi_Sta_Begin_LegacyTasmota(uint8_t flag, uint8_t channel = 0);
//     void WiFi_Link_CheckIp_AndHandleTransitions(void);
//     void WiFi_Sta_Maintain_Periodic(uint8_t param);
//     int WiFi_Sta_State_Get(void);
    void WiFi_Sta_Connect_Start(void);
//     // void WifiDisconnect(void);
    void WiFi_Radio_Shutdown(bool option = false);

    const char* GetWiFiStatusCtr(void);
    const char* GetWiFiConfigTypeCtr(void);

    enum WIFIBEGIN_Flags{ 
      WIFIBEGIN_FLAG_SSID0_ID=0,
      WIFIBEGIN_FLAG_SSID1_ID,
      // WIFIBEGIN_FLAG_SSID2_ID,
      WIFIBEGIN_FLAG_TOGGLE_SSIDS_ID
    };

    static inline float WiFiPower_To_dBm(wifi_power_t p)
    {
      return (float)p * 0.25f; // ie 19.5dBm = 78enum -> 78*0.25f => 19.5
    }


};

#endif

#endif // ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2

#endif //   #ifdef USE_MODULE_NETWORK_WIFI