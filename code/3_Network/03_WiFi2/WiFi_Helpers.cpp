#include "mWiFi.h"


#ifdef USE_MODULE_NETWORK_WIFI
#ifdef ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2




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