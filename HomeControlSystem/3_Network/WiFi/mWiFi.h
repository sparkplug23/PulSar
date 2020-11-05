#ifndef _MWIFI_H
#define _MWIFI_H 0.4

#include "0_ConfigUser/mUserConfig.h"

// #define ENABLE_WIFI_DEVELOPMENT

#ifdef ENABLE_USER_CONFIG_OVERRIDE 
  #include "0_ConfigUser/mUserConfigSecret.h"
#endif
#include "2_CoreSystem/Logging/mLogging.h"

#include <ArduinoJson.h>
#include "1_TaskerManager/mTaskerManager.h"


#include "2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/Settings/mSettings.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  // #include <ESP8266mDNS.h>
#endif

#include "2_CoreSystem/Time/mTime.h"

class mWiFi{
  public:
    mWiFi(){};
    
    #define D_MAX_SSIDS 3

    int8_t Tasker(uint8_t function);
    void init(void);

    int8_t GetRSSdBm();
    uint8_t GetRSSPercentage();

    uint32_t tSavedWiFi;
    uint32_t tSavedWiFiCheckIP;
    uint32_t tSavedWiFiReconnect;
    
    bool WifiCheckIpConnected();

    #ifndef WIFI_RSSI_THRESHOLD
      #define WIFI_RSSI_THRESHOLD     10         // Difference in dB between current network and scanned network
    #endif
    #ifndef WIFI_RESCAN_MINUTES
      #define WIFI_RESCAN_MINUTES     30         // Number of minutes between wifi network rescan
    #endif

    const uint8_t WIFI_CONFIG_SEC = 180;       // seconds before restart
    const uint8_t WIFI_CHECK_SEC = 20;         // seconds
    const uint8_t WIFI_RETRY_OFFSET_SEC = 20;  // seconds


    struct WIFI_CONNECTION{
      uint32_t last_event = 0;       // Last wifi connection event
      uint32_t downtime = 0;         // Wifi down duration
      uint16_t link_count = 0;       // Number of wifi re-connect
      uint8_t counter;
      uint8_t retry_init;
      uint8_t retry;
      uint8_t status;
      uint8_t config_type = 0;
      uint8_t config_counter = 0;
      uint8_t scan_state;
      uint8_t bssid[6];
      uint8_t fConnected = false;
      uint8_t fReconnect = false;
    }connection;




    uint8_t mdns_begun = 0;             // mDNS active
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
    void WifiBeginAfterScan();
    uint16_t WifiLinkCount();
    //String WifiDowntime();
    void WifiSetState(uint8_t state);
    void WifiBegin(uint8_t flag, uint8_t channel);
    void WifiCheckIp(void);
    void WifiCheck(uint8_t param);
    int WifiState(void);
    void WifiConnect(void);
    // void WifiDisconnect(void);
    void WifiShutdown(bool option = false);
    void EspRestart(void);

    const char* GetWiFiStatusCtr(void);
    const char* GetWiFiConfigTypeCtr(void);

    enum WIFIBEGIN_Flags{ 
      WIFIBEGIN_FLAG_SSID0_ID=0,
      WIFIBEGIN_FLAG_SSID1_ID,
      WIFIBEGIN_FLAG_SSID2_ID,
      WIFIBEGIN_FLAG_TOGGLE_SSIDS_ID
    };

};

#endif
