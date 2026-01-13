
#ifndef _FIRMWARE_DEFAULTS_H_
#define _FIRMWARE_DEFAULTS_H_

// This should maybe become its own subfolder, where one (top) includes others. This is to allow more unified way.
// Then I can have a clean files/dev files.


/*********************************************************************************************\
 * Function declarations
\*********************************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef ESP8266
#include "user_interface.h"
#endif

#ifdef ESP8266
// Function prototypes
void WiFi_Wps_StatusCallback(wps_cb_status status);
#endif

#ifdef __cplusplus
}
#endif

// Create warning messages about the use of USBC and Serial
#ifdef CONFIG_IDF_TARGET_ESP32C3
#if ARDUINO_USB_CDC_ON_BOOT == 1
#warning "ARDUINO_USB_CDC_ON_BOOT=1 permits Serial over USBC when connected to computer. CRITCIAL will stop boot when power via USBC only."
#else
#warning "ARDUINO_USB_CDC_ON_BOOT=0 allows normal boot over power only, but NO SERIAL IS PRESET!"
#endif
#endif


#include "2_CoreSystem/mBaseConfig.h"

/***** SECTION REMOVED DECEMBER 2024 */
#include "0_ConfigUser/G0_mFirmwareCustom_Secret.h"

/**
 * @brief This file will become a wrapper to the subfolders
 **/
#include "2_CoreSystem/00_FirmwareDefaults/01_Base/mFirmwareDefaults_Base.h"
#include "2_CoreSystem/00_FirmwareDefaults/03_Lighting/mFirmwareDefaults_Lighting.h"

/*********************************************************************************************\
 * Default global defines
\*********************************************************************************************/

#ifndef MODULE
  #define MODULE                 MODULE_BASIC   // [Module] Select default model
#endif

//Minimal
#define USE_MODULE_CORE_HARDWAREPINS
#define USE_MODULE_CORE_SETTINGS
#define USE_MODULE_CORE_SUPPORT
#define USE_MODULE_CORE_LOGGING
#define USE_MODULE_CORE_TELEMETRY
#define USE_MODULE_CORE_TIME
#define USE_MODULE_CORE_RULES
#define USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
#define USE_MODULE_CORE_EVENTS

// #define USE_MODULE_SENSORS_SUN_TRACKING

// #ifndef DISABLE_NETWORK
// #define USE_MODULE_NETWORK_MQTT
// #endif // DISABLE_NETWORK


// #ifndef DISABLE_NETWORK_WIFI
// #define USE_MODULE_NETWORK_WIFI
// #endif // DISABLE_NETWORK

#if !defined(DISABLE_NETWORK)
  #warning "==========================================================FirmwareDefault: Enable Wifi v1
  // #define USE_MODULE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_INTERFACE

  #ifndef DISABLE_DEVFEATURE_NETWORK_WIFI
    // #define USE_MODULE_NETWORK_WIFI
  #endif
#endif


#ifndef USE_MODULE_NETWORK_WIFI
  #warning "USE_MODULE_NETWORK_WIFI" has not been defined -- NO WIFI!
#endif
#ifndef USE_MODULE_NETWORK_MQTT
  #warning "USE_MODULE_NETWORK_MQTT" has not been defined -- NO MQTT!
#endif


#ifdef ESP8266
  #include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#endif

#ifndef ARDUINO_ESP8266_RELEASE
#define ARDUINO_ESP8266_RELEASE "STAGE"
#endif

#ifdef ARDUINO_ESP8266_RELEASE_2_3_0          // Disable not supported features in core 2.3.0
#undef USE_MQTT_TLS_CA_CERT
#endif

#ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
struct RgbwwColor;  // Forward declaration
typedef RgbwwColor ColourBaseType;
#else
typedef uint32_t ColourBaseType;
#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Default Firmware Configurations *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/*****
 * Starting from here, a minimal/generic template config should be created here
 */

#ifdef FIRMWARE_DEFAULT__MODULE_TEMPLATE__GENERIC_MINIMUM
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "template_module"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
    #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
    #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template"
  #ifndef MQTT_HOST
    #define MQTT_HOST   "192.168.3.70"
  #endif
  #define MQTT_PORT     1883
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"    
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif





#ifdef USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT

  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{" // for PIR to follow
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_PIR_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";

  // #error "hitting here"


#endif // USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT


#ifdef USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


#endif // USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS






/**
 * @brief This method allows the storage to be loaded, but will always then load templates and override anything from settings
 *        to make sure the device starts in a known state. This will make sure SSID etc are loaded 
 */
#ifdef ENABLE_FEATURE_FIRMWAREDEFAULT__LOAD_WITH_TEMPLATES_OVERRIDE

  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    
    
    
    




  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    


#endif // ENABLE_FEATURE_FIRMWAREDEFAULT__LOAD_WITH_TEMPLATES_OVERRIDE


/**
 * @brief This is for release to other people. A templated version will be compiled, and enable an ESP to be flashed
 *        and then fully configured via the webui. This will include SSIDs, MQTT, and other settings, so will require settings
 *        to be fully working and with fallback to starting in direct wifi mode for webpage configuration
 */
#ifdef ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_ONLY_ON_RESET





#endif // ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_ONLY_ON_RESET





// Until fully working, just enable by group [CORE / NETWORKING / LIGHTING / DRIVERS / SENSORS]
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS

  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    


#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__LIGHTING



#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__LIGHTING

// Until fully working, just enable by group [CORE / NETWORKING / LIGHTING / DRIVERS / SENSORS]
#ifdef ENABLE_DEVFEATURE_STORAGE__RESTORE_MODULE__LIGHTING
// For much later. After saving works and is testing with the webui, I can start loading it on reboot.
// This loading will only work after proper validation can be done.



#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__LIGHTING




/**
 * @brief This needs to be at the end to undef above
 * 
 */
#ifdef ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING

#undef USE_MODULE_NETWORK_WEBSERVER
#undef USE_MODULE_NETWORK_WIFI
#define DISABLE_NETWORK


#endif // ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING



#endif