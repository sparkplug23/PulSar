#ifndef _M_ETHERNET_H
#define _M_ETHERNET_H

#define D_UNIQUE_MODULE_NETWORK_ETHERNET_ID  3004 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_NETWORK_ETHERNET

#include "1_TaskerManager/mTaskerInterface.h"

#ifdef ESP32
  #include <ETH.h>
  #include <WiFi.h>   // for WiFi.onEvent + WiFi.mode(WIFI_OFF)
#endif

class mEthernet : public mTaskerInterface
{
public:
  mEthernet() {}

  int8_t  Tasker(uint8_t function, JsonParserObject obj = 0);
  void    init(void);
  void    parse_JSONCommand(JsonParserObject obj);

  static constexpr const char* PM_MODULE_NETWORK_ETHERNET_CTR = D_MODULE_NETWORK_ETHERNET_CTR;
  PGM_P   GetModuleName()      { return PM_MODULE_NETWORK_ETHERNET_CTR; }
  uint16_t GetModuleUniqueID() { return D_UNIQUE_MODULE_NETWORK_ETHERNET_ID; }

  // High-level status
  bool Ethernet_HasLink() const;
  bool Ethernet_HasIP() const;
  bool Ethernet_IsRoutable() const;

  // Optional: reuse your existing naming if other code expects it
  bool Link_IsIpRoutable() const { return Ethernet_IsRoutable(); }

private:
#ifdef ESP32
  // ---- Board / PHY config (override with -D in build flags as needed) ----
  #ifndef PULSAR_ETH_PHY_TYPE
    #define PULSAR_ETH_PHY_TYPE   ETH_PHY_LAN8720
  #endif
  #ifndef PULSAR_ETH_PHY_ADDR
    #define PULSAR_ETH_PHY_ADDR   0
  #endif
  #ifndef PULSAR_ETH_MDC_PIN
    #define PULSAR_ETH_MDC_PIN    23
  #endif
  #ifndef PULSAR_ETH_MDIO_PIN
    #define PULSAR_ETH_MDIO_PIN   18
  #endif
  #ifndef PULSAR_ETH_POWER_PIN
    #define PULSAR_ETH_POWER_PIN  -1
  #endif
  #ifndef PULSAR_ETH_CLK_MODE
    // LilyGO T-Internet-POE family often needs GPIO17 clock out (Tasmota EthClockMode 3)
    #define PULSAR_ETH_CLK_MODE   ETH_CLOCK_GPIO17_OUT
  #endif

  char eth_hostname[64]; // max 63 + null char

  // ---- State ----
  struct {
    bool delayed_start = true;
    bool     begun        = false;
    bool     link_up      = false;
    bool     got_ip       = false;
    bool     connected    = false;   // “routable up” (what you want for MQTT/NTP)
    uint32_t last_change  = 0;
  } eth;

  WiFiClient* mqtt_client = nullptr;

  static void EthernetEvent(arduino_event_t* event);



bool EthernetGetIP(IPAddress *ip);
bool EthernetHasIP(void);
String EthernetGetIPStr(void);
char* EthernetHostname(void);
String EthernetMacAddress(void);
void EthernetConfigChange(void);
IPAddress EthernetLocalIP(void);


  void Ethernet_Begin();
  void Ethernet_Stop();
  void Ethernet_Tick_1s();        // called from TASK_EVERY_SECOND

  void OnBecameConnected();
  void OnBecameDisconnected();
#endif
};

#endif // USE_MODULE_NETWORK_ETHERNET
#endif // _M_ETHERNET_H



// #ifndef _M_ETHERNET_H
// #define _M_ETHERNET_H

// #define D_UNIQUE_MODULE_NETWORK_ETHERNET_ID  3004 // [(Folder_Number*100)+ID_File]

// #include "1_TaskerManager/mTaskerManager.h"

// #ifdef USE_MODULE_NETWORK_ETHERNET

// #include "1_TaskerManager/mTaskerInterface.h"

// class mWiFi :
//   public mTaskerInterface
// {
//   public:
//     mWiFi(){};
    
//     #define D_MAX_SSIDS 3
//     int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
//     void init(void);    

//     static constexpr const char* PM_MODULE_NETWORK_ETHERNET_CTR = D_MODULE_NETWORK_ETHERNET_CTR;
//     PGM_P GetModuleName(){          return PM_MODULE_NETWORK_ETHERNET_CTR; }
//     uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_NETWORK_ETHERNET_ID; }

//     void parse_JSONCommand(JsonParserObject obj);
   
//     void WiFi_Sta_ProfileIndex_Connect(uint8_t ap_index);
//     int8_t GetRSSdBm();
//     uint8_t GetRSSPercentage();

    
//     const uint8_t ETHERNET_CONFIG_SEC = 180;       // seconds before restart
//     const uint8_t ETHERNET_CHECK_SEC = 20;         // seconds
//     const uint8_t ETHERNET_RETRY_OFFSET_SEC = 20;  // seconds

//     #define D_ETHERNET_CONFIG_SEC 180
//     #define D_ETHERNET_cONFIG_SEC_FIRST_CONNECT 5

//     uint32_t tSavedWiFi;
//     uint32_t tSavedWiFiCheckIP;
//     uint32_t tSavedWiFiReconnect;

//     WiFiClient* mqtt_client = nullptr;
    
//     uint16_t wifi_counter_tester = 0;
    
//     bool WiFi_Link_IsIpRoutable();

//     #ifndef ETHERNET_RSSI_THRESHOLD
//       #define ETHERNET_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
//     #endif
//     #ifndef ETHERNET_RESCAN_MINUTES
//       #define ETHERNET_RESCAN_MINUTES     44         // Number of minutes between wifi network rescan
//     #endif



//     struct ETHERNET_CONNECTION{
//       uint32_t last_event = 0;       // Last wifi connection event
//       uint32_t downtime = 0;         // Wifi down duration
//       uint16_t link_count = 0;       // Number of wifi re-connect
//       uint8_t counter = 0;
//       uint8_t retry_init = 0;
//       uint8_t retry = 0;
//       uint8_t status = 0;
//       uint8_t config_type = 0;
//       uint8_t config_counter = D_ETHERNET_cONFIG_SEC_FIRST_CONNECT; // IMPORTANT!!
//       uint8_t scan_state = 0;
//       uint8_t bssid[6];
//       uint8_t fConnected = false;
//       uint8_t fReconnect = false;
//     }connection;


// void WiFi_Mdns_StartOrRestart(void);
// void WiFi_Mdns_DiscoverMqttBroker(void);
// void WiFi_Mdns_AdvertiseHttpService(void);
// void WiFi_Mdns_Tick(void);

// struct {
//   uint8_t begun = 0;                  // mDNS active
// } Mdns;

//     uint8_t wps_result;



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
//     void WiFi_Radio_SleepMode_Apply(void);
//     void WiFi_Sta_Scan_SelectBest_AndBegin();
//     uint16_t WiFi_Link_ReconnectCount_Get();
//     //String WifiDowntime();
//     void WiFi_Sta_State_Set(uint8_t state);
//     void WiFi_Sta_Begin_LegacyTasmota(uint8_t flag, uint8_t channel = 0);
//     void WiFi_Link_CheckIp_AndHandleTransitions(void);
//     void WiFi_Sta_Maintain_Periodic(uint8_t param);
//     int WiFi_Sta_State_Get(void);
//     void WiFi_Sta_Connect_Start(void);
//     // void WifiDisconnect(void);
//     void WiFi_Radio_Shutdown(bool option = false);
//     void EspRestart(void);

//     const char* GetWiFiStatusCtr(void);
//     const char* GetWiFiConfigTypeCtr(void);

//     enum ETHERNETBEGIN_Flags{ 
//       ETHERNETBEGIN_FLAG_SSID0_ID=0,
//       ETHERNETBEGIN_FLAG_SSID1_ID,
//       ETHERNETBEGIN_FLAG_SSID2_ID,
//       ETHERNETBEGIN_FLAG_TOGGLE_SSIDS_ID
//     };

// };

// #endif

// #endif // _M_ETHERNET_H