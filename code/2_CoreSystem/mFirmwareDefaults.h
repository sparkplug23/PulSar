
#ifndef _FIRMWARE_DEFAULTS_H_
#define _FIRMWARE_DEFAULTS_H_

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
void WifiWpsStatusCallback(wps_cb_status status);
#endif

#ifdef __cplusplus
}
#endif

#include "2_CoreSystem/mBaseConfig.h"
#include "0_ConfigUser/G0_mFirmwareCustom_Secret.h"
#ifdef D_USER_MICHAEL
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_LongTerm.h"
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_Temporary.h"
#include "0_ConfigUser/01_mFirmwareCustom_Secret_Testbeds.h"
#include "0_ConfigUser/02_mFirmwareCustom_Secret_Dev.h"
#endif // D_USER_MICHAEL

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

#ifndef DISABLE_NETWORK
// #define USE_MODULE_NETWORK_WIFI
      // #ifndef ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812
#define USE_MODULE_NETWORK_MQTT
      // #endif // ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812
#endif // DISABLE_NETWORK

#if !defined(USE_MODULE_NETWORK_WIFI_V2) && !defined(DISABLE_NETWORK)
#warning "==========================================================FirmwareDefault: Enable Wifi v1
#define USE_MODULE_NETWORK_WIFI
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


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Default Firmware Configurations *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef USE_DEFAULT_FIRMWARE_SENSORS
  #define USE_MODULE_SENSORS_INTERFACE // Base interface
  // #define USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DOOR

#endif // USE_DEFAULT_FIRMWARE_SENSORS

#endif 
//#endif