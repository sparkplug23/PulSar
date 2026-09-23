#ifndef _FIRMWARE_DEFAULTS__BASE_H_
#define _FIRMWARE_DEFAULTS__BASE_H_


/**
 * ============================================================================
 * PulSar Core Firmware Defaults
 * ============================================================================
 *
 * This file defines compile-time defaults for core PulSar functionality.
 *
 * Module-specific configuration belongs in the corresponding subsystem
 * defaults file and should not be enabled here.
 *
 * In particular:
 *
 *   - lighting functionality belongs in mFirmwareDefaults_Lighting2.h
 *   - lighting WebUI functionality belongs in mFirmwareDefaults_Lighting2.h
 *
 * The generic network webserver is independent from any module-specific WebUI.
 * Modules may require the webserver to be enabled, but they must not silently
 * enable it themselves.
 * ============================================================================
 */


/**
 * ============================================================================
 * SECTION 1
 * CORE WEBSERVER LEVEL
 * ============================================================================
 *
 * The generic PulSar webserver is independent from module-specific WebUIs.
 *
 * A device may therefore use:
 *
 *   - the core webserver without lighting
 *   - lighting without a lighting WebUI
 *   - a lighting WebUI with BASIC core webserver functionality
 *   - ADVANCED core webserver functionality with a minimal lighting WebUI
 *
 * The two capability levels are intentionally independent.
 *
 *
 * BASIC
 * -----
 *
 * Enables the core network webserver and JSON support required by normal
 * HTTP/WebSocket handlers.
 *
 *
 * NORMAL
 * ------
 *
 * BASIC plus normal system-control webserver functionality.
 *
 *
 * ADVANCED
 * --------
 *
 * NORMAL plus additional development/diagnostic endpoints and tools.
 *
 * This currently includes:
 *
 *   - advanced URL tracking/listing
 *   - pin viewer
 *
 *
 * NONE
 * ----
 *
 * Explicitly indicates that the normal PulSar webserver is not required.
 *
 * Module-specific defaults which require a webserver should fail compilation
 * if USE_MODULE_NETWORK_WEBSERVER is unavailable.
 *
 * Exactly one level should normally be selected.
 * ============================================================================
 */

// #define FIRMWARE_DEFAULT__WEBSERVER__NONE
// #define FIRMWARE_DEFAULT__WEBSERVER__BASIC
// #define FIRMWARE_DEFAULT__WEBSERVER__NORMAL
// #define FIRMWARE_DEFAULT__WEBSERVER__ADVANCED


#if (defined(FIRMWARE_DEFAULT__WEBSERVER__NONE) + \
     defined(FIRMWARE_DEFAULT__WEBSERVER__BASIC) + \
     defined(FIRMWARE_DEFAULT__WEBSERVER__NORMAL) + \
     defined(FIRMWARE_DEFAULT__WEBSERVER__ADVANCED)) > 1

  #error "Only one FIRMWARE_DEFAULT__WEBSERVER__xxx may be selected"

#endif


/**
 * ============================================================================
 * SECTION 2
 * CORE WEBSERVER FEATURE EXPANSION
 * ============================================================================
 */


/**
 * All enabled webserver levels require the network webserver module and
 * AsyncJson support.
 */
#if defined(FIRMWARE_DEFAULT__WEBSERVER__BASIC) || \
    defined(FIRMWARE_DEFAULT__WEBSERVER__NORMAL) || \
    defined(FIRMWARE_DEFAULT__WEBSERVER__ADVANCED)

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_FEATURE_JSON__ASYNCJSON_V6

#endif


/**
 * Normal system-control functionality.
 */
#if defined(FIRMWARE_DEFAULT__WEBSERVER__NORMAL) || \
    defined(FIRMWARE_DEFAULT__WEBSERVER__ADVANCED)

  #define ENABLE_FEATURE_WEBSERVER__SYSTEM_CONTROLS
  #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET

#endif


/**
 * Additional webserver tooling/endpoints.
 */
#ifdef FIRMWARE_DEFAULT__WEBSERVER__ADVANCED

  #define ENABLE_FEATURE_WEBSERVER__ADVANCED_URL_LIST

  #ifdef ESP32
    #define USE_MODULE_CORE_PINVIEWER
  #endif

#endif


/**
 * ============================================================================
 * SECTION 3
 * CORE DEBUGGING DEFAULTS
 * ============================================================================
 */

#ifdef FIRMWARE_DEFAULT__INCLUDE_DEBUGGING_BASIC

  #define ENABLE_DEBUGFEATURE_TASKER__SPLASH_JSON_BUFFER

#endif


#endif // _FIRMWARE_DEFAULTS__BASE_H_






// #ifndef _FIRMWARE_DEFAULTS__BASE_H_
// #define _FIRMWARE_DEFAULTS__BASE_H_

// // #define ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI

// /**
//  * @brief Added 10Jan26
//  * New WiFi2
//  * Webserver standalone (detangled from lights)
//  * new npm builds, lights, webserver and submodules.
//  * 
//  */
// #ifdef ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI

// #error "not to use"

//   #ifndef ESP8266
//     #define ENABLE_FEATURE_LIGHTING__WEBUI__ADVANCED_PAGES
//   #endif

//   //filesystem to become standard!
//   // 
  
//   #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_MODULE_DATA
//    // until devices can reliably be used without compiling per device
  

//   #ifdef ESP32
//   #define USE_MODULE_CORE_FILESYSTEM
//   #endif
    
    
    
    

//   //webserver?
  
//   #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
//   #define USE_MODULE_NETWORK_WEBSERVER
    
  

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__XML_API
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_LEDS
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_SYNC

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__URL_QUERY_SETTINGS



// // lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_POLLING
  
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CAPTIVE_PORTAL

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SHARED_STYLES



// #endif // ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI


// /**
//  * @brief Include everything
//  * 
//  */
// #ifdef FIRMWARE_DEFAULT__WEBSERVER__ADVANCED


//   #ifndef ESP8266
//     #define ENABLE_FEATURE_LIGHTING__WEBUI__ADVANCED_PAGES
//   #endif

//   //filesystem to become standard!
//   // 
  
//   #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_MODULE_DATA
//    // until devices can reliably be used without compiling per device
  

//   #ifdef ESP32
//   #define USE_MODULE_CORE_FILESYSTEM
//   #define USE_MODULE_CORE_PINVIEWER
//   #endif
    
  
//   #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
//   #define USE_MODULE_NETWORK_WEBSERVER
    

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET
  

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__XML_API
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_LEDS
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_SYNC

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__URL_QUERY_SETTINGS




// // lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_POLLING
  
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CAPTIVE_PORTAL

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SHARED_STYLES


// #endif

// /**
//  * @brief Basic meaning the minimal amount, safe for esp8266 and esp32
//  * basic logging, and uploading new binary
//  * 
//  */
// #ifdef FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC

//   #ifndef ESP8266
//     #define ENABLE_FEATURE_LIGHTING__WEBUI__ADVANCED_PAGES
//   #endif

//   //filesystem to become standard!
//   // 
  
//   #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_MODULE_DATA
//    // until devices can reliably be used without compiling per device
  

//   #ifdef ESP32
//   #define USE_MODULE_CORE_FILESYSTEM
//   #endif
    
    
    

//   //webserver?
  
//   #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
//   #define USE_MODULE_NETWORK_WEBSERVER
    
  

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__XML_API
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_LEDS
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_SYNC

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__URL_QUERY_SETTINGS



// // lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_POLLING
  
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET
//   #define ENABLE_FEATURE_LIGHTING__WEBUI__CAPTIVE_PORTAL

//   #define ENABLE_FEATURE_LIGHTING__WEBUI__SHARED_STYLES



// #endif // ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI




// /**
//  * @brief Basic meaning the minimal amount, safe for esp8266 and esp32
//  * basic logging, and uploading new binary
//  * 
//  */
// #ifdef FIRMWARE_DEFAULT__NO_WEBSERVER

//   // #ifndef ESP8266
//   //   #define ENABLE_FEATURE_LIGHTING__WEBUI__ADVANCED_PAGES
//   // #endif

//   //filesystem to become standard!
//   // 
  
//   #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_MODULE_DATA
//    // until devices can reliably be used without compiling per device
  

//   #ifdef ESP32
//   #define USE_MODULE_CORE_FILESYSTEM
//   #endif
    
    
    
    

// //   //webserver?
  
// //   #define ENABLE_FEATURE_JSON__ASYNCJSON_V6
// //   // #define USE_MODULE_NETWORK_WEBSERVER
    
  

// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__XML_API
// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_LEDS
// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_SYNC

// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__URL_QUERY_SETTINGS



// // // lets wire with relay8 as "PRE_ARM", which means it must also be powered to give the other relays power, to stop restarts causing resets.


// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_POLLING
  
// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET
// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__CAPTIVE_PORTAL

// //   #define ENABLE_FEATURE_LIGHTING__WEBUI__SHARED_STYLES



// #endif // ENABLE_FEATURE_BUILD__11JAN25_WIFI_AND_WEBUI



// #ifdef FIRMWARE_DEFAULT__INCLUDE_DEBUGGING_BASIC // No more than a few per second




//   #define ENABLE_DEBUGFEATURE_TASKER__SPLASH_JSON_BUFFER

// #endif



// #endif
