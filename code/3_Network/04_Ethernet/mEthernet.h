#ifndef _M_ETHERNET_H
#define _M_ETHERNET_H

#define D_UNIQUE_MODULE_NETWORK_ETHERNET_ID  3004 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_ETHERNET

#include "1_TaskerManager/mTaskerInterface.h"

class mWiFi :
  public mTaskerInterface
{
  public:
    mWiFi(){};
    
    #define D_MAX_SSIDS 3
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);    

    static constexpr const char* PM_MODULE_NETWORK_ETHERNET_CTR = D_MODULE_NETWORK_ETHERNET_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_NETWORK_ETHERNET_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_ETHERNET_ID; }

    void parse_JSONCommand(JsonParserObject obj);
   
    void WifiConnectAP(uint8_t ap_index);
    int8_t GetRSSdBm();
    uint8_t GetRSSPercentage();

    
    const uint8_t ETHERNET_CONFIG_SEC = 180;       // seconds before restart
    const uint8_t ETHERNET_CHECK_SEC = 20;         // seconds
    const uint8_t ETHERNET_RETRY_OFFSET_SEC = 20;  // seconds

    #define D_ETHERNET_CONFIG_SEC 180
    #define D_ETHERNET_cONFIG_SEC_FIRST_CONNECT 5

    uint32_t tSavedWiFi;
    uint32_t tSavedWiFiCheckIP;
    uint32_t tSavedWiFiReconnect;

    WiFiClient* mqtt_client = nullptr;
    
    uint16_t wifi_counter_tester = 0;
    
    bool WifiCheckIpConnected();

    #ifndef ETHERNET_RSSI_THRESHOLD
      #define ETHERNET_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
    #endif
    #ifndef ETHERNET_RESCAN_MINUTES
      #define ETHERNET_RESCAN_MINUTES     44         // Number of minutes between wifi network rescan
    #endif



    struct ETHERNET_CONNECTION{
      uint32_t last_event = 0;       // Last wifi connection event
      uint32_t downtime = 0;         // Wifi down duration
      uint16_t link_count = 0;       // Number of wifi re-connect
      uint8_t counter = 0;
      uint8_t retry_init = 0;
      uint8_t retry = 0;
      uint8_t status = 0;
      uint8_t config_type = 0;
      uint8_t config_counter = D_ETHERNET_cONFIG_SEC_FIRST_CONNECT; // IMPORTANT!!
      uint8_t scan_state = 0;
      uint8_t bssid[6];
      uint8_t fConnected = false;
      uint8_t fReconnect = false;
    }connection;


void StartMdns(void);
void MqttDiscoverServer(void);
void MdnsAddServiceHttp(void);
void MdnsUpdate(void);

struct {
  uint8_t begun = 0;                  // mDNS active
} Mdns;

    uint8_t wps_result;



    void SplashWifiScan();
    void WifiConnectForced();
    int WifiGetRssiAsQuality(int rssi);
    bool WifiConfigCounter(void);
    #ifndef ESP32
    void WifiWpsStatusCallback(wps_cb_status status);
    #endif
    bool WifiWpsConfigDone(void);
    bool WifiWpsConfigBegin(void);
    void WifiConfig(uint8_t type);
    void WiFiSetSleepMode(void);
    void ScanBestAndBeginWifi();
    uint16_t WifiLinkCount();
    //String WifiDowntime();
    void WifiSetState(uint8_t state);
    void WifiBegin(uint8_t flag, uint8_t channel = 0);
    void WifiCheckIp(void);
    void WifiCheck(uint8_t param);
    int WifiState(void);
    void WifiConnect(void);
    // void WifiDisconnect(void);
    void WifiShutdown(bool option = false);
    void EspRestart(void);

    const char* GetWiFiStatusCtr(void);
    const char* GetWiFiConfigTypeCtr(void);

    enum ETHERNETBEGIN_Flags{ 
      ETHERNETBEGIN_FLAG_SSID0_ID=0,
      ETHERNETBEGIN_FLAG_SSID1_ID,
      ETHERNETBEGIN_FLAG_SSID2_ID,
      ETHERNETBEGIN_FLAG_TOGGLE_SSIDS_ID
    };

};

#endif

#endif // _M_ETHERNET_H