
#ifndef _FIRMWARE_DEFAULTS__BASE_H_
#define _FIRMWARE_DEFAULTS__BASE_H_

// #define ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI

/**
 * @brief Added 10Jan26
 * New WiFi2
 * Webserver standalone (detangled from lights)
 * new npm builds, lights, webserver and submodules.
 * 
 */
#ifdef ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI

#error "not to use"

  #ifndef ESP8266
    #define ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
  #endif

  //filesystem to become standard!
  // 
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  #ifdef ESP32
  #define USE_MODULE_CORE_FILESYSTEM
  #endif
    
    
    
    

  //webserver?
  
  #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
    
  

  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS



// lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


  #define ENABLE_FEATURE__DATABUFFER_LOCK
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
  

  #ifdef ESP32
  #define USE_MODULE_CORE_FILESYSTEM
  #endif
    
#define USE_MODULE_CORE_PINVIEWER
    

  //webserver?
  
  #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
    
  

  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS




// lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


  #define ENABLE_FEATURE__DATABUFFER_LOCK
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
  

  #ifdef ESP32
  #define USE_MODULE_CORE_FILESYSTEM
  #endif
    
    
    

  //webserver?
  
  #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
    
  

  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS



// lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


  #define ENABLE_FEATURE__DATABUFFER_LOCK
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED



#endif // ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI




/**
 * @brief Basic meaning the minimal amount, safe for esp8266 and esp32
 * basic logging, and uploading new binary
 * 
 */
#ifdef FIRMWARE_DEFAULT__NO_WEBSERVER

  // #ifndef ESP8266
  //   #define ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
  // #endif

  //filesystem to become standard!
  // 
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  #ifdef ESP32
  #define USE_MODULE_CORE_FILESYSTEM
  #endif
    
    
    
    

//   //webserver?
  
//   #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
//   // #define USE_MODULE_NETWORK_WEBSERVER
    
  

//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

//   #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS



// // lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


//   #define ENABLE_FEATURE__DATABUFFER_LOCK
//   #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  
//   #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
//   #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

//   #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED



#endif // ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI



#ifdef FIRMWARE_DEFAULT__INCLUDE_DEBUGGING_BASIC // No more than a few per second




  #define ENABLE_DEBUGFEATURE_TASKER__SPLASH_JSON_BUFFER

#endif



#endif
