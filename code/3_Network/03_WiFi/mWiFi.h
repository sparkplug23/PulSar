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
  #include <ESP8266WiFi.h>
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
       
    static constexpr const char* PM_MODULE_NETWORK_WIFI_CTR = D_MODULE__NETWORK__WIFI__CTR;
    PGM_P GetModuleName(){ return PM_MODULE_NETWORK_WIFI_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_WIFI_ID; }

    struct ClassState
    {
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);

    int8_t GetRSSdBm();
    uint8_t GetRSSPercentage();

    const uint8_t WIFI_CHECK_SEC = 20;

    #ifndef WIFI_RSSI_THRESHOLD
      #define WIFI_RSSI_THRESHOLD 10
    #endif

    #ifndef WIFI_OUTAGE_RESCAN_SECONDS
      #define WIFI_OUTAGE_RESCAN_SECONDS (10UL * 60UL)
    #endif

    #ifndef WIFI_VISIBLE_CANDIDATE_CONNECT_TIMEOUT_SEC
      #define WIFI_VISIBLE_CANDIDATE_CONNECT_TIMEOUT_SEC 6
    #endif

    #ifndef WIFI_QUICK_CONNECT_TIMEOUT_SEC
      #define WIFI_QUICK_CONNECT_TIMEOUT_SEC 6
    #endif

    struct WiFiProfile
    {
      char id[24] = {0};
      char ssid[33] = {0};
      char pass[65] = {0};
      uint8_t priority = 0;
      bool ssid_hidden = false;
      bool has_bssid = false;
      uint8_t bssid[6] = {0};
    };

    struct IPv4Config
    {
      bool is_static = false;
      uint8_t ip[4] = {0,0,0,0};
      uint8_t gw[4] = {0,0,0,0};
      uint8_t sn[4] = {255,255,255,0};
      uint8_t dns1[4] = {0,0,0,0};
      uint8_t dns2[4] = {0,0,0,0};
    };

    struct WiFiStationConfig
    {
      bool enabled = true;
      WiFiProfile profiles[WIFI_MAXIMUM_CONNECTIONS];
      uint8_t profile_count = 0;
      uint8_t active_profile = 0;
      IPv4Config ipv4;
    };

    struct WiFiSoftApConfig
    {
      bool enabled = true;
      bool always_on = false;
      char ssid[33] = {0};
      char pass[65] = {0};
      uint8_t channel = 1;
      bool hidden = false;
    };

    struct WiFiModuleConfig
    {
      WiFiStationConfig station;
      WiFiSoftApConfig softap;
    };

    WiFiModuleConfig config;

    void WiFi_Config_Clear(void);
    void WiFi_Config_LoadLegacyDefaults(void);

    uint8_t loglevel_with_connection_status = 10;

    bool s_wifi2_scanned_on_boot = false;
    bool s_wifi2_scanned_on_this_outage = false;

    bool WiFi_Link_IsIpRoutable();

    struct WIFI_CONNECTION
    {
      uint32_t last_event = 0;
      uint32_t downtime = 0;
      uint16_t link_count = 0;
      uint8_t counter = 0;
      uint8_t config_type = 0;
      uint8_t bssid[6] = {0};
      uint8_t fConnected = false;
      uint8_t fReconnect = false;
      int8_t seconds_to_wait_for_fresh_connection_attempt = 0;

      #ifdef ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA
      bool quick_connect_active = false;
      #endif
    }connection;

    /************************************************************************************************
     * SECTION: STA profile selection
     ************************************************************************************************/

    uint8_t WiFi_Sta_SelectProfileIndex_OrderedFirstConfigured(void) const;
    uint8_t WiFi_Sta_SelectProfileIndex_WithScanPreference(bool force_scan);
    bool WiFi_Sta_ShouldScanNow_OnBootOrOutage(void);
    void WiFi_Sta_OnConnected_ResetOutageScanFlags(void);

    /************************************************************************************************
     * SECTION: Optional scan/ranked candidate connection
     *
     * ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES
     *
     * A scan produces only configured SSIDs which were actually visible.
     * Those are ranked using RSSI plus configured priority and tried quickly
     * before falling back to the normal profile cycling behaviour.
     ************************************************************************************************/

    #ifdef ENABLE_FEATURE_WIFI__SCAN_AND_RANK_PROFILES

    struct WiFiCandidate
    {
      uint8_t profile = 0;
      int16_t rssi = INT16_MIN;
      uint8_t channel = 0;
      uint8_t bssid[6] = {0};
    };

    WiFiCandidate wifi_candidates[WIFI_MAXIMUM_CONNECTIONS];
    uint8_t wifi_candidate_count = 0;
    uint8_t wifi_candidate_index = 0;

    void WiFi_Sta_CandidateList_Clear(void);
    bool WiFi_Sta_CandidateList_BuildFromScan(void);
    bool WiFi_Sta_CandidateList_TryNext(void);
    bool WiFi_Sta_CandidateShouldComeBefore(const WiFiCandidate& a, const WiFiCandidate& b) const;

    #endif

    /************************************************************************************************
     * SECTION: Optional OTA RTC quick-connect
     *
     * ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA
     *
     * On successful OTA:
     *   - record current profile
     *   - record actual BSSID
     *   - record actual channel
     *
     * On next boot:
     *   - consume record once
     *   - start an asynchronous direct association
     *   - continue normal PulSar boot
     *   - fall back to normal scan/cycling if it fails
     *
     * Requires ENABLE_FEATURE_RTC__SETTINGS for actual retained storage.
     ************************************************************************************************/

    #ifdef ENABLE_FEATURE_WIFI__SSID_QUICK_CONNECT_AFTER_OTA
    bool WiFi_QuickConnect_SaveToRTC(void);
    bool WiFi_QuickConnect_TryFromRTC(void);
    #endif

    /************************************************************************************************
     * SECTION: WiFi station
     ************************************************************************************************/

    void WiFi_Sta_Maintain_Periodic(void);
    void WiFi_Sta_State_Set(uint8_t state);
    void WiFi_Sta_Connect_Start(void);
    void WiFi_Sta_ProfileIndex_Connect(uint8_t profile_i, uint8_t runtime_channel = 0, const uint8_t* runtime_bssid = nullptr);

    bool WiFi2_HasAnyStaProfileConfigured(void) const;
    uint8_t WiFi2_GetFirstConfiguredProfileIndex(void) const;

    void WiFi2_Sta_EnsureConnecting(void);
    void WiFi2_Sta_Connected_Enter(void);
    void WiFi2_Sta_Disconnected_Enter(void);

    void Init_Preload_Wifi2_Settings();

    /************************************************************************************************
     * SECTION: SoftAP
     ************************************************************************************************/

    bool WiFi2_Ap_EnsureStarted(void);

    DNSServer dnsServer;
    bool dnsServerRunning = false;

    void WiFi2_Ap_Dns_Start(void);
    void WiFi2_Ap_Dns_Stop(void);
    void WiFi2_Ap_Dns_Tick(void);

    /************************************************************************************************
     * SECTION: Helpers
     ************************************************************************************************/

    static IPAddress IPv4ArrayToIP(const uint8_t a[4]);

    void Task_EverySecond();

    bool WiFi_IsAPMode(void);
    bool WiFi_IsSTAMode(void);
    bool WiFi_IsAPSTAMode(void);

    bool WiFi_HasLocalConnectivity(void);
    bool WiFi_HasExternalConnectivity(void);

    void WiFi_Radio_Shutdown(bool option = false);

    const char* GetWiFiStatusCtr(void);

    /************************************************************************************************
     * SECTION: mDNS
     ************************************************************************************************/

    void WiFi_Mdns_StartOrRestart(void);
    void WiFi_Mdns_DiscoverMqttBroker(void);

    bool WiFi_Dns_ResolveHostname(const char* aHostname, IPAddress& aResult);

    struct
    {
      uint8_t begun = 0;
    }Mdns;

    #if defined(USE_NETWORK_MDNS) && defined(ESP8266)
    void WiFi_Mdns_Tick(void);
    #endif

    #ifdef ESP32
    static inline float WiFiPower_To_dBm(wifi_power_t p){ return (float)p * 0.25f; }
    #endif
};

#endif

#endif