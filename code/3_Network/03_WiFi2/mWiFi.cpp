#include "mWiFi.h"

#ifdef USE_MODULE_NETWORK_WIFI

#ifdef ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2

#ifdef ESP8266
extern "C" {
 #include "user_interface.h"
}
#endif

int8_t mWiFi::Tasker(uint8_t function, JsonParserObject obj){

  // DEBUG_LINE_HERE3
  #ifdef ENABLE_DEVFEATURE_NETWORK__BLOCK_CONNECT_PUSH_BACKOFF_LONG_AS_TEMP_SOLUTION_TO_NO_WIFI
  return 0;
  #endif

  #ifdef ENABLE_DEBUGFEATURE_LIGHTS__ESP32C3_FLICKER_TEST
  return 0;
  #endif

  switch(function){
    case TASK_INIT:

      #ifdef ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025
      WifiConnect(); // new SoftAP + multiSSID method
      #else
      WifiConnect(); // Old method from Tas
      #endif 

    break;
    case TASK_LOOP: 
    
      #if defined(USE_NETWORK_MDNS) && defined(ESP8266)
        MdnsUpdate();
      #endif // USE_NETWORK_MDNS
    
    break;
    case TASK_EVERY_SECOND:{
      
      AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "network_wifi=%d"), tkr_set->Settings.flag_network.network_wifi);

      #ifdef ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025
        Handle_WiFiConnection(); // new SoftAP + multiSSID method  
        // if (tkr_set->Settings.flag_network.network_wifi) 
        // {
        //   WifiCheck(tkr_set->runtime.wifi_state_flag);// Old method from Tas
        //   tkr_set->runtime.wifi_state_flag = WIFI_RESTART;
        // } // new SoftAP + multiSSID method
      #else
        if (tkr_set->Settings.flag_network.network_wifi) 
        {
          WifiCheck(tkr_set->runtime.wifi_state_flag);// Old method from Tas
          tkr_set->runtime.wifi_state_flag = WIFI_RESTART;
        }
      #endif // ENABLE_DEVFEATURE_WIFI_CONNECTION_VERSION2_2025

      AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_ssid[%d]=%s"),tkr_set->Settings.sta_active, tkr_set->SettingsText(SET_STASSID1 + tkr_set->Settings.sta_active) );
      AddLog(loglevel_with_connection_status, PSTR(D_LOG_WIFI "sta_pwd[%d]=%s"), tkr_set->Settings.sta_active, tkr_set->SettingsText(SET_STAPWD1 + tkr_set->Settings.sta_active) );

    }
    break;
    case TASK_EVERY_MINUTE:
    
      ALOG_INF( PSTR("WL_CONNECTED %s"), WiFi.localIP().toString().c_str() );

      break;
    case TASK_EVERY_FIVE_MINUTE:
      // ALOG_INF( PSTR("WL_CONNECTED %s"), WiFi.localIP().toString().c_str() );
      
      #ifdef USE_NETWORK_MDNS
        StartMdns();
      #endif  // USE_NETWORK_MDNS

    break;
    case TASK_WIFI_CONNECTED:{

      #ifdef USE_NETWORK_MDNS
        StartMdns();
      #endif  // USE_NETWORK_MDNS

      // #ifndef ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

      //   ALOG_HGL(PSTR("Start MQTTConnection with WiFi"));

      //   #ifdef USE_MODULE_NETWORK_MQTT

      //   DEBUG_LINE_HERE3

      //     mqtt_client = new WiFiClient();
      //     DEBUG_LINE_HERE3

      //     tkr_mqtt->CreateConnection(mqtt_client, MQTT_HOST, MQTT_PORT, CLIENT_TYPE_WIFI_ID);
      //     DEBUG_LINE_HERE3
          
      //     tkr_mqtt->brokers.back()->SetCredentials(MQTT_USER, MQTT_PASS);
      //     DEBUG_LINE_HERE3

      //     tkr_mqtt->brokers.back()->SetReConnectBackoffTime(MQTT_RETRY_SECS);
      //     DEBUG_LINE_HERE3
          
      //     // char client_name[100]; snprintf_P(client_name, sizeof(client_name), PSTR("%s-%s"), tkr_set->Settings.system_name.device, WiFi.macAddress().c_str()); 
          
      //     uint8_t mac[6];           WiFi.macAddress(mac);
      //     DEBUG_LINE_HERE3
      //     char client_name[100]; snprintf_P(client_name, sizeof(client_name), PSTR("%s-%02X:%02X:%02X"), tkr_set->Settings.system_name.device, mac[3], mac[4], mac[5]); 
      //     DEBUG_LINE_HERE3
      //     tkr_mqtt->brokers.back()->SetClientName(client_name);
      //     DEBUG_LINE_HERE3

      //     tkr_mqtt->brokers.back()->SetTopicPrefix(tkr_set->Settings.system_name.device);
      //     DEBUG_LINE_HERE3

      //   #endif // USE_MODULE_NETWORK_MQTT
      // #endif // ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      
      DEBUG_LINE_HERE;
    }
    break;
  }

  // DEBUG_LINE_HERE3
  return FUNCTION_RESULT_UNKNOWN_ID;


} // END function


#endif // ENABLE_DEVFEATURE_NETOWRK__WIFI_VERSION_2026V2

#endif // USE_MODULE_NETWORK_WIFI