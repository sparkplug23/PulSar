#ifndef _CONFIG_USER_FIRMWARE_TEMPLATE_LIGHTING_H2
#define _CONFIG_USER_FIRMWARE_TEMPLATE_LIGHTING_H2

#error "here2"
/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are actively being ran as test devices.
 * Aiming to move these into the garage on a shelf, with a camera looking at them. Everything should be OTA flashed and debugged.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Devices]-------------------------------------

//////////////////////// ESP32
// #define DEVICE_TEMPLATE_BASE__LIGHTING__ESP32_1CH                                         
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_I2S_PARALLEL_4CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L14__ESP32_I2S_PARALLEL_8CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L14__ESP32_I2S_PARALLEL_16CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L16__ESP32_I2S_PARALLEL_RED_BOARD_4CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L10__ESP32__7SEGMENTCLOCK
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L12__ESP32_I2S_PARALLEL_4CH_INCLUDING_DEBUG_PINS       // Timing Tester

/////////////////////// ESP8266
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L3__ESP8266_1CH_RGB_100LEDS                            // in room
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L4__H801_5CH_PWM_RGBCCT                                // in room under desk to develop for h801
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L5__H801_3CHRGB_AND_2CHCW
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L9__ESP8266_WEBUI

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------




#ifdef DEVICE_TEMPLATE__LIGHTING__NEOPIXELSONOFF_BASIC_R2
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_HOST     MQTT_HOST
    #define MQTT_PORT     1883


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  
  
  #define USE_MODULE_TEMPLATE_SONOFF_BASIC_R2

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
    #define MAX_RELAYS 1
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR  "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
   
  // Default Rule Defined (DefaultRule_Sonoff_Basic_R2)

#endif



#endif // _CONFIG_USER_FIRMWARE_TEMPLATE_LIGHTING_H

