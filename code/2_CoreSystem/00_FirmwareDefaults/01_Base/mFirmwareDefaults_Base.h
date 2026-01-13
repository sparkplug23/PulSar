
#ifndef _FIRMWARE_DEFAULTS__BASE_H_
#define _FIRMWARE_DEFAULTS__BASE_H_

#define ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI

/**
 * @brief Added 10Jan26
 * New WiFi2
 * Webserver standalone (detangled from lights)
 * new npm builds, lights, webserver and submodules.
 * 
 */
#ifdef ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI

  #ifndef ESP8266
    #define ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
  #endif

  //filesystem to become standard!
  // 
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    
    
    

  //webserver?
  
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
    
  

  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

  #define ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
  



// lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


  #define ENABLE_DEVFEATURE_DATABUFFER_LOCK
  #define ENABLE_DEBUGFEATURE_TASKER__SPLASH_JSON_BUFFER
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED



#endif // ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI


/**
 * @brief Include everything
 * 
 */
#ifdef FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_FULL


  #ifndef ESP8266
    #define ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
  #endif

  //filesystem to become standard!
  // 
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    
    
    

  //webserver?
  
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
    
  

  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

  #define ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
  



// lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


  #define ENABLE_DEVFEATURE_DATABUFFER_LOCK
  #define ENABLE_DEBUGFEATURE_TASKER__SPLASH_JSON_BUFFER
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED


#endif

/**
 * @brief Basic meaning the minimal amount, safe for esp8266 and esp32
 * basic logging, and uploading new binary
 * 
 */
#ifdef FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC

  #ifndef ESP8266
    #define ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
  #endif

  //filesystem to become standard!
  // 
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    
    
    

  //webserver?
  
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
    
  

  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

  #define ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
  



// lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


  #define ENABLE_DEVFEATURE_DATABUFFER_LOCK
  #define ENABLE_DEBUGFEATURE_TASKER__SPLASH_JSON_BUFFER
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED



#endif // ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI


#endif