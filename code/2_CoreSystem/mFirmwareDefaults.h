
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

/***** SECTION REMOVED DECEMBER 2024 */
#include "0_ConfigUser/G0_mFirmwareCustom_Secret.h"
#ifdef USE_USER_MICHAEL
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_LongTerm.h"
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_Temporary.h"
#include "0_ConfigUser/01_mFirmwareCustom_Secret_ExampleTemplates.h"
#include "0_ConfigUser/02_mFirmwareCustom_Secret_DevTestbeds.h"
#include "0_ConfigUser/03_mFirmware_Secret__ActiveDevelopment.h"
#include "0_ConfigUser/04_mFirmwareCustom_Secret__Christmas_2023.h"
#include "0_ConfigUser/04_mFirmwareCustom_Secret__Christmas_2024.h"
#include "0_ConfigUser/05_mFirmwareCustom_Secret__Colorado_2024.h"
  #include "0_ConfigUser/DevelopingByTypes/FirmwareGroup_LightingEffects.h"
  #include "0_ConfigUser/DevelopingByTypes/FirmwareGroup_MotionDetectors.h"
  #include "0_ConfigUser/DevelopingByTypes/FirmwareGroup_BoardBuilds.h"
  #include "0_ConfigUser/DevelopingByTypes/FirmwareGroup_Cameras.h"
  #include "0_ConfigUser/DevelopingByTypes/FirmwareGroup_HVAC.h"
  #include "0_ConfigUser/DevelopingByTypes/FirmwareGroup_PZEM.h"
#include "0_ConfigUser/BuiltUsingGroups/GroupUsing_LightingEffects.h"
#include "0_ConfigUser/DevelopingByTypes/FirmwareGroup_CellularDatalinks.h"
  #include "0_ConfigUser/Meadows/FirmwareConfig_Deployed.h"
#else
  // #error "error"
#endif // USE_USER_MICHAEL

#include "0_ConfigUser/Templates/TemplateBase__HardwareSpecific.h"

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



/**-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---
FIRMWARE DEFAULT:: LIGHTING CONFIGS
---
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/***
 * Below will be replaced with these core options. Only actual things under test will be used in FIRMWARE_DEVTEST__LIGHTING_CONFIG__AUGUST2025
 * 
 * FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC     // ie home and webui
 * FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE  // all 1D
 * FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D        // all 1D and 2D
 * FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE // all 1D and sound reactive (2D must be activated too if desired)
 * FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA      // all light code ready to cross-test with other modules for inclusion in the next release
 * 
 */

#define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA // FORCE ON

// -------------------------------------------------------------------------------------------------------------------------------------------------------


#ifdef FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES


  #define USE_MODULE_SENSORS_SUN_TRACKING     
  #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
    #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
  #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
  #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
    // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS2
  #define USE_MAXELEVATION_CALC_JULY2025

  #define USE_MODULE_SENSORS_SUN_TRACKING

#endif



#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  
  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE // Inherit base config

  /************************************************************************
   * BUS: 
   ************************************************************************/
  #define ENABLE_DEVFEATURE_LIGHT__PWM_DITHER_V2

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING__EXTERNAL_LIGHT_SENSING
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

  
  // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
  // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
  #define WLED_DEBUG
  #define DEBUG_ASYNC
  #define WLED_DISABLE_HUESYNC
  #define ENABLE_DEVFEATURE_LIGHTING__SHOW_FALLBACK_MINIMAL_2024
  #define ENABLE_DEVFEATURE_LIGHTING__SETGETPIXEL_FALLBACK_MINIMAL_2024
  #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
  #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
  #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
  #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER  
  #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
  #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION  
  #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
  #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
  #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
  #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE  
  #define ENABLE_DEVFEATURE_WEBSERVER__ETAGS_ENABLED_FOR_RELOADING_PALETTES_ON_FRESH_COMPILE
  #define ENABLE_DEVFEATURE_LIGHTING__SUPPRESS_WHITE_OUTPUT

#endif

// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE // Inherit base config

  #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  
  // temporary fix until rgbww is added with matrix support functions
  #ifndef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE // setPixelXY needs to be added to rgbww
  #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
  #define WLED_DEBUG
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
  #endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

#endif

// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE
  /**
   * SOUND: Reactive can be just 1D or 2D, so it does not really cause 2D base, for sound+2D 2D still needs manually added
   **/

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE // Inherit base config

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
  
  #if defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D) || defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D)
    #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
  #endif

#endif

// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC // Inherit base config

  /************************************************************************
   * EFFECTS: 
   ************************************************************************/
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

  // #define FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES // need to introduce a basic/lite version when only being used for palettes.
  // this is probably the angles etc, so module needs split effeciently for full data, or just palette info very minute for performance
  #ifndef USE_MODULE_SENSORS_SUN_TRACKING
  #define USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE
  #endif
  
  #define ENABLE_NEW_LIVE_PALETTES

  // #define ENABLE_DEBUGFEATURE__LIGHTING__MATCH_FEWER_PHYSICAL_PIXELS 10
  
  #define ENABLE_ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

#endif

// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC

  /************************************************************************
   * FILESYSTEM: 
   ************************************************************************/
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device
  #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /************************************************************************
   * WEBPAGE:
   ************************************************************************/
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI  
  #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
  #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

  #define WLED_ENABLE_WEBSOCKETS2
  #define ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
  #define WLED_ENABLE_JSONLIVE

  /************************************************************************
  * LIGHTING:
  ************************************************************************/

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS

  /************************************************************************
   * EFFECTS: 
   ************************************************************************/
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

  /************************************************************************
   * BUS:
   ************************************************************************/
  #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

  /************************************************************************
   * BRIGHTNESS:
   ************************************************************************/
  #define ANIM_BRIGHTNESS_REQUIRED true // only when manually testing it to be manual, but not part of the feb2025 method
  // #define ANIM_BRIGHTNESS_REQUIRED false // when using LG bus

  #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS // handle it without BusLg methods

    #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS
    #undef ANIM_BRIGHTNESS_REQUIRED
    #define ANIM_BRIGHTNESS_REQUIRED true // when controlling it directly
    #endif

  /************************************************************************
   * PRESETS:
   ************************************************************************/
  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG

  /************************************************************************
   * PLAYLISTS:
   ************************************************************************/
  #define ENABLE_FEATURE_LIGHTING__EFFECTS
  #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES


#endif





#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__AUGUST2025
  #define FIRMWARE_DEFAULT_DESCRIPTION "LIGHTING_CONFIG__AUGUST2025"
  #error "Removed"    
#endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__AUGUST2025__NO_MODULE_GPIO




/**-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---
Everything after this will be temporary configs outlining the above converted into defaults/base values future builds should use
This enables switching to newer firmware versions, but falling back when an issue happens. Each version below shall be given a string name to be known in the build/mqtt

--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/





#ifdef ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE
/**
 * @brief This section enables the use of a single button (as default, KEY1) that must be set outside of this
 * When pressed, a few methods will automatically be useful for debugging and testing
 * 
 */
 /**
 * @brief 
 * Button: Multipress
 * ** (1) "Demo: 1 minute"
 * ** (2) "Colour Test": 
 *           Part 1: 20 seconds
 *            Full brightness
 *            First 4 pixels will show R, G, B, white always
 *            Remaining pixels will sweep across with random colours
 *           Part 2: 20 seconds
 *            Rainbow Cycle
 *           Part 3: 20 seconds
 *            Gradient with Rainbow 16
 * ** (3) "Bus Test"
 *            Part 1: 
 *              Busses will show the "hardware test" effect, where the first pixel(s) of each bus is white (based on the bus index, ie bus 2 is 2 white pixels), then the remaining are a gradient of set colours
 * ** (4) ""
 * Button: Hold (Once, even though multiple can happen)
 *        "Toggle Power"
 *           Brightness from max to min
 * 
 */


  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS
    

  // #define ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS
  // #define ENABLE_FEATURE_LIGHTS__DEMO_MODE


#endif // ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

  
// /**
//  * Lighting as default:
//  * ** no bus
//  * ** normal webui, presets, playlists are enabled
//  * 
//  * Update Date: 12 November 2024
//  * End Date: This should be removed
// */
// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_NOVEMBER_2024

//   #error "FIRMWARE removed"

//   /**
//    * Hardware 
//    */
//   #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//   /**
//    * Software: Fully or largely incorporated
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//   // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

//   ///////// Presets ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   ///////// Playlists ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   /**
//    * Software: Testing and not yet considered "standard"
//    **/ 
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL


//   #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
//   #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #endif
  
//   #define DEBUG_ASYNC
//   #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
//   #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_NOVEMBER_2024







// /**
//  * Lighting as default:
//  * ** no bus
//  * ** normal webui, presets, playlists are enabled
//  * ** New U32 methods, testing on sgrp 60 devices to become primary lighting code config
//  * 
//  * Update Date: 12 November 2024
//  * End Date: This should be removed
// */
// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025


//   #define FIRMWARE_DEFAULT_DESCRIPTION /*USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__*/ "LATEST_LIGHTING_JANUARY_2025"

//   #error "USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025"


//   #error "FIRMWARE removed"
//   #ifndef DEVICE_TESTGROUP__LIGHTING_EFFECTS__60__SUBGROUP__BASE
//   #ifndef DEVICENAME_CTR
//   #define DEVICENAME_CTR          "testbed_default"
//   #endif
//   #ifndef DEVICENAME_FRIENDLY_CTR
//   #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
//   #endif
//   #ifndef DEVICENAME_DESCRIPTION_CTR
//   #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
//   #endif
//   #define DEVICENAME_ROOMHINT_CTR "testgroup"
//   #define MQTT_HOST   "192.168.3.70"
//     #define MQTT_HOST     MQTT_HOST
//     #define MQTT_PORT     1883
    
// //   #define SETTINGS_HOLDER 1239

// //   // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

// //   // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

// //   /***********************************
// //    * SECTION: System Debug Options
// //   ************************************/    
// //   ///////////////////////////////////////////// Enable Logs
// //   // #define DISABLE_SERIAL
// //   // #define DISABLE_SERIAL0_CORE
// //   // #define DISABLE_SERIAL_LOGGING
// //   // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
  
// //   ///////////////////////////////////////////// System Logs
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES
//   // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
//   // #define ENABLE_DEBUG_LINE_HERE
//   // #define ENABLE_DEBUG_LINE_HERE2
//   // #define ENABLE_DEBUG_LINE_HERE3
//   // #define ENABLE_DEBUG_LINE_HERE_TRACE
//   // #define ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
// //   // #define ENABLE_DEBUG_PRINT_F
// //   // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// //   // #define USE_DEBUG_PRINT
// //   // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// // //   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
// // //   #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

// //   #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

// // // #define ENABLE_DEBUG_MANUAL_DELAYS

// //   ///////////////////////////////////////////// Module Logs
// //   // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// //   // #define ENABLE_FREERAM_APPENDING_SERIAL
  

// //   // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

// //   /***********************************
// //    * SECTION: System Configs
// //   ************************************/    

// //   // #define USE_MODULE_CORE_FILESYSTEM
// //   //   #define WLED_ENABLE_FS_EDITOR
// //   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
// //   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
// //   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

// //   // Settings saving and loading
// //   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
// //   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
// //   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
// //   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
// //   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES



// //   /***********************************
// //    * SECTION: Network Configs
// //   ************************************/    

// //   // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

// //   // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
// //   // #define USE_MODULE_NETWORK_WEBSERVER
// //   // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  

// //   // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
// //   // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
// //   // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

// //   // // #define DISABLE_NETWORK
// //   // // #define DISABLE_NETWORK_WIFI
// //   // #define USE_MODULE_NETWORK_WIFI
// //   // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI  


// //   /***********************************
// //    * SECTION: Lighting Configs
// //   ************************************/    

// //   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025

// //   /***********************************
// //    * SECTION: MODULE_TEMPLATE Configs
// //   ************************************/    

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
//     "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";

//   // #define ENABLE_DEVFEATURE_LIGHT__EFFECT_SHOW_TIME_NEW
//   #define WLED_DEBUG

//   #endif


//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
//   #define WLED_DISABLE_HUESYNC

//   #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

//   #define ENABLE_DEVFEATURE_LIGHTING__SHOW_FALLBACK_MINIMAL_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__SETGETPIXEL_FALLBACK_MINIMAL_2024

//   // #define ENABLE_DEVFEATURE_LIGHTING__BUSPWM_DITHERING_PWM_ENABLED
//   // #define ENABLE_DEVFEATURE_LIGHTING__BUSPWM_2025_METHOD
  
//   #define ANIM_BRIGHTNESS_REQUIRED true // only when manually testing it to be manual, but not part of the feb2025 method
//   // #define ANIM_BRIGHTNESS_REQUIRED false // when using LG bus

//   #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS // handle it without BusLg methods

//   #ifdef ENABLE_FEATURE_LIGHTING__USE_NEOPIXELBUS_LIGHT_GAMMA_LG
//   #undef ANIM_BRIGHTNESS_REQUIRED
//   #define ANIM_BRIGHTNESS_REQUIRED false // when using LG bus
//   #endif

//   #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS
//   #undef ANIM_BRIGHTNESS_REQUIRED
//   #define ANIM_BRIGHTNESS_REQUIRED true // when controlling it directly
//   #endif

//   #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
  
  
//   /**
//    * @brief Feb2025 Debug serial logs
//    * 
//    */
//   // #define ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE 1
//   // #define ENABLE_DEVFEATURE_LIGHT__SERIAL_SHOW_PRE_EFFECT_CALL




//   // 
//   /**
//    * Hardware 
//    */
//   #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//   /**
//    * Software: Fully or largely incorporated
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//   // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

//   ///////// Presets ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   ///////// Playlists ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   /**
//    * Software: Testing and not yet considered "standard"
//    **/ 
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL


//   #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
//   #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #endif
  
//   #define DEBUG_ASYNC
//   #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
//   #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE
//   /**
//    * Hardware 
//    */
//   #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//   /**
//    * Software: Fully or largely incorporated
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//   // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

//   ///////// Presets ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   ///////// Playlists ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   /**
//    * Software: Testing and not yet considered "standard"
//    **/ 
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL


//   #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
//   #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #endif
  
//   #define DEBUG_ASYNC
//   #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
//   #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE

 
//   // #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D

//   // temporary fix until rgbww is added with matrix support functions
//   #ifndef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE // setPixelXY needs to be added to rgbww

//   // #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
//     #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
//     #define WLED_DEBUG
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
//   #endif 

//   #endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE


//   // #define ENABLE_DEVFEATURE_LIGHT__CREATE_MATRIX_IMMEDIATELY

//   #if defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D) || defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D)
//     #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
//   #endif

// /**
//  * @brief Tmp fixes to hard code matrix setup
//  * 
//  */
// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_HORIZONTAL_8W32H
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 32 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 8 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_HORIZONTAL_32W8H
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 8 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 32 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_16W16H
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 16 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 16 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_16W16H_VERTICAL
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 16 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 16 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_VERTICAL
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__VERTICAL
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 8 // Height
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 32 // Width
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// //  #endif


  

//   #define ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS

//   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
//   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG


//   #define ENABLE_DEVFEATURE_LIGHT__PWM_DITHER_V2
  
  
//   #define ENABLE_DEVFEATURE_WEBSERVER__ETAGS_ENABLED_FOR_RELOADING_PALETTES_ON_FRESH_COMPILE


//   #define ENABLE_DEVFEATURE_LIGHTING__SUPPRESS_WHITE_OUTPUT
  
//   // #define WLED_ENABLE_WEBSOCKETS
//   #define WLED_ENABLE_WEBSOCKETS2
//   #define ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
//   #define WLED_ENABLE_JSONLIVE

//   #define ENABLE_ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025



// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO
//   #define FIRMWARE_DEFAULT_DESCRIPTION /*USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__*/ "LATEST_LIGHTING_JANUARY_2025"

//   #error "FIRMWARE removed"
//   #ifndef DEVICE_TESTGROUP__LIGHTING_EFFECTS__60__SUBGROUP__BASE
//   #ifndef DEVICENAME_CTR
//   #define DEVICENAME_CTR          "testbed_default"
//   #endif
//   #ifndef DEVICENAME_FRIENDLY_CTR
//   #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
//   #endif
//   #ifndef DEVICENAME_DESCRIPTION_CTR
//   #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
//   #endif
//   #define DEVICENAME_ROOMHINT_CTR "testgroup"

//   #ifndef MQTT_HOST
//     #define MQTT_HOST   "192.168.1.71"
//     #error "I dont want this anymore, mqtt defined in ini"
//   #endif


//     #define MQTT_PORT     1883
    
// //   #define SETTINGS_HOLDER 1239

// //   // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

// //   // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

// //   /***********************************
// //    * SECTION: System Debug Options
// //   ************************************/    
// //   ///////////////////////////////////////////// Enable Logs
// //   // #define DISABLE_SERIAL
// //   // #define DISABLE_SERIAL0_CORE
// //   // #define DISABLE_SERIAL_LOGGING
// //   // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
  
// //   ///////////////////////////////////////////// System Logs
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES
//   // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
//   // #define ENABLE_DEBUG_LINE_HERE
//   // #define ENABLE_DEBUG_LINE_HERE2
//   // #define ENABLE_DEBUG_LINE_HERE3
//   // #define ENABLE_DEBUG_LINE_HERE_TRACE
//   // #define ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
// //   // #define ENABLE_DEBUG_PRINT_F
// //   // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// //   // #define USE_DEBUG_PRINT
// //   // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// // //   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
// // //   #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

// //   #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

// // // #define ENABLE_DEBUG_MANUAL_DELAYS

// //   ///////////////////////////////////////////// Module Logs
// //   // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// //   // #define ENABLE_FREERAM_APPENDING_SERIAL
  

// //   // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

// //   /***********************************
// //    * SECTION: System Configs
// //   ************************************/    

// //   // #define USE_MODULE_CORE_FILESYSTEM
// //   //   #define WLED_ENABLE_FS_EDITOR
// //   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
// //   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
// //   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

// //   // Settings saving and loading
// //   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
// //   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
// //   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
// //   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
// //   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES



// //   /***********************************
// //    * SECTION: Network Configs
// //   ************************************/    

// //   // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

// //   // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
// //   // #define USE_MODULE_NETWORK_WEBSERVER
// //   // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  

// //   // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
// //   // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
// //   // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

// //   // // #define DISABLE_NETWORK
// //   // // #define DISABLE_NETWORK_WIFI
// //   // #define USE_MODULE_NETWORK_WIFI
// //   // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI  


// //   /***********************************
// //    * SECTION: Lighting Configs
// //   ************************************/    

// //   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025

// //   /***********************************
// //    * SECTION: MODULE_TEMPLATE Configs
// //   ************************************/    

//   // #define USE_MODULE_TEMPLATE
//   // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   // "{"
//   //   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
//   //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//   //   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//   //   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   // "}";

//   // #define ENABLE_DEVFEATURE_LIGHT__EFFECT_SHOW_TIME_NEW
//   #define WLED_DEBUG

//   #endif


//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
//   #define WLED_DISABLE_HUESYNC

//   #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

//   #define ENABLE_DEVFEATURE_LIGHTING__SHOW_FALLBACK_MINIMAL_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__SETGETPIXEL_FALLBACK_MINIMAL_2024

//   // #define ENABLE_DEVFEATURE_LIGHTING__BUSPWM_DITHERING_PWM_ENABLED
//   // #define ENABLE_DEVFEATURE_LIGHTING__BUSPWM_2025_METHOD
  
//   #define ANIM_BRIGHTNESS_REQUIRED true // only when manually testing it to be manual, but not part of the feb2025 method
//   // #define ANIM_BRIGHTNESS_REQUIRED false // when using LG bus

//   #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS // handle it without BusLg methods

//   #ifdef ENABLE_FEATURE_LIGHTING__USE_NEOPIXELBUS_LIGHT_GAMMA_LG
//   #undef ANIM_BRIGHTNESS_REQUIRED
//   #define ANIM_BRIGHTNESS_REQUIRED false // when using LG bus
//   #endif

//   #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS
//   #undef ANIM_BRIGHTNESS_REQUIRED
//   #define ANIM_BRIGHTNESS_REQUIRED true // when controlling it directly
//   #endif

//   #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
  
  
//   /**
//    * @brief Feb2025 Debug serial logs
//    * 
//    */
//   // #define ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE 1
//   // #define ENABLE_DEVFEATURE_LIGHT__SERIAL_SHOW_PRE_EFFECT_CALL




//   // 
//   /**
//    * Hardware 
//    */
//   #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//   /**
//    * Software: Fully or largely incorporated
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//   // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

//   ///////// Presets ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   ///////// Playlists ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   /**
//    * Software: Testing and not yet considered "standard"
//    **/ 
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL


//   #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
//   #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #endif
  
//   #define DEBUG_ASYNC
//   #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
//   #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE
//   /**
//    * Hardware 
//    */
//   #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//   /**
//    * Software: Fully or largely incorporated
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//   // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

//   ///////// Presets ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   ///////// Playlists ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   /**
//    * Software: Testing and not yet considered "standard"
//    **/ 
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL


//   #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
//   #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #endif
  
//   #define DEBUG_ASYNC
//   #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
//   #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE

 
//   // #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D

//   // temporary fix until rgbww is added with matrix support functions
//   #ifndef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE // setPixelXY needs to be added to rgbww

//   // #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
//     #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
//     #define WLED_DEBUG
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
//   #endif 

//   #endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE


//   // #define ENABLE_DEVFEATURE_LIGHT__CREATE_MATRIX_IMMEDIATELY

//   #if defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D) || defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D)
//     #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
//   #endif

// /**
//  * @brief Tmp fixes to hard code matrix setup
//  * 
//  */
// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_HORIZONTAL_8W32H
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 32 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 8 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_HORIZONTAL_32W8H
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 8 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 32 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_16W16H
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 16 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 16 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_16W16H_VERTICAL
// // // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__HORIZONTAL
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 16 // Height
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 16 // Width
// // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// // #endif


// // #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_VERTICAL
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_32X8__VERTICAL
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__START_Y 0
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP_Y 8 // Height
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__STOP 32 // Width
// //  #define ENABLE_DEVFEATURE_LIGHT__MATRIX_HARDCODED_INIT_VALUES__VERTICAL 0 // Vertical
// //  #endif


  

//   #define ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS

//   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
//   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG


//   #define ENABLE_DEVFEATURE_LIGHT__PWM_DITHER_V2
  
  
//   #define ENABLE_DEVFEATURE_WEBSERVER__ETAGS_ENABLED_FOR_RELOADING_PALETTES_ON_FRESH_COMPILE


//   #define ENABLE_DEVFEATURE_LIGHTING__SUPPRESS_WHITE_OUTPUT
  
//   // #define WLED_ENABLE_WEBSOCKETS
//   #define WLED_ENABLE_WEBSOCKETS2
//   #define ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
//   #define WLED_ENABLE_JSONLIVE

//   #define ENABLE_ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025



// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__JUNE2025__NO_MODULE_GPIO
//   #define FIRMWARE_DEFAULT_DESCRIPTION /*USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__*/ "LATEST_LIGHTING_JANUARY_2025"

//   #error "FIRMWARE removed"
//   #error "FIRMWARE removed"

//   // #ifndef DEVICE_TESTGROUP__LIGHTING_EFFECTS__60__SUBGROUP__BASE
//   // #ifndef DEVICENAME_CTR
//   // #define DEVICENAME_CTR          "testbed_default"
//   // #endif
//   // #ifndef DEVICENAME_FRIENDLY_CTR
//   // #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
//   // #endif
//   // #ifndef DEVICENAME_DESCRIPTION_CTR
//   // #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
//   // #endif
//   // #define DEVICENAME_ROOMHINT_CTR "testgroup"

//   // #ifndef MQTT_HOST
//   //   #define MQTT_HOST   "192.168.1.71"
//   //   #error "I dont want this anymore, mqtt defined in ini"
//   // #endif


//     #define MQTT_PORT     1883
    
// //   #define SETTINGS_HOLDER 1239

// //   // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

// //   // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

// //   /***********************************
// //    * SECTION: System Debug Options
// //   ************************************/    
// //   ///////////////////////////////////////////// Enable Logs
// //   // #define DISABLE_SERIAL
// //   // #define DISABLE_SERIAL0_CORE
// //   // #define DISABLE_SERIAL_LOGGING
// //   // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
  
// //   ///////////////////////////////////////////// System Logs
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES
//   // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
//   // #define ENABLE_DEBUG_LINE_HERE
//   // #define ENABLE_DEBUG_LINE_HERE2
//   // #define ENABLE_DEBUG_LINE_HERE3
//   // #define ENABLE_DEBUG_LINE_HERE_TRACE
//   // #define ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING
// //   // #define ENABLE_DEBUG_PRINT_F
// //   // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// //   // #define USE_DEBUG_PRINT
// //   // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// // //   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
// // //   #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

// //   #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

// // // #define ENABLE_DEBUG_MANUAL_DELAYS

// //   ///////////////////////////////////////////// Module Logs
// //   // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// //   // #define ENABLE_FREERAM_APPENDING_SERIAL
  

// //   // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

// //   /***********************************
// //    * SECTION: System Configs
// //   ************************************/    

// //   // #define USE_MODULE_CORE_FILESYSTEM
// //   //   #define WLED_ENABLE_FS_EDITOR
// //   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
// //   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
// //   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

// //   // Settings saving and loading
// //   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
// //   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
// //   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
// //   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
// //   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES



// //   /***********************************
// //    * SECTION: Network Configs
// //   ************************************/    

// //   // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

// //   // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
// //   // #define USE_MODULE_NETWORK_WEBSERVER
// //   // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  

// //   // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
// //   // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
// //   // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

// //   // // #define DISABLE_NETWORK
// //   // // #define DISABLE_NETWORK_WIFI
// //   // #define USE_MODULE_NETWORK_WIFI
// //   // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI  


// //   /***********************************
// //    * SECTION: Lighting Configs
// //   ************************************/    

// //   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025

// //   /***********************************
// //    * SECTION: MODULE_TEMPLATE Configs
// //   ************************************/    

//   // #define USE_MODULE_TEMPLATE
//   // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   // "{"
//   //   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
//   //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//   //   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//   //   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   // "}";

//   // #define ENABLE_DEVFEATURE_LIGHT__EFFECT_SHOW_TIME_NEW
//   #define WLED_DEBUG

//   // # endif


//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING

//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
//   #define WLED_DISABLE_HUESYNC

//   #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
//   #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

//   #define ENABLE_DEVFEATURE_LIGHTING__SHOW_FALLBACK_MINIMAL_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__SETGETPIXEL_FALLBACK_MINIMAL_2024

//   // #define ENABLE_DEVFEATURE_LIGHTING__BUSPWM_DITHERING_PWM_ENABLED
//   // #define ENABLE_DEVFEATURE_LIGHTING__BUSPWM_2025_METHOD
  
//   #define ANIM_BRIGHTNESS_REQUIRED true // only when manually testing it to be manual, but not part of the feb2025 method
//   // #define ANIM_BRIGHTNESS_REQUIRED false // when using LG bus

//   #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS // handle it without BusLg methods

//   #ifdef ENABLE_FEATURE_LIGHTING__USE_NEOPIXELBUS_LIGHT_GAMMA_LG
//   #undef ANIM_BRIGHTNESS_REQUIRED
//   #define ANIM_BRIGHTNESS_REQUIRED false // when using LG bus
//   #endif

//   #ifdef ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_MANUAL_CONTROLS
//   #undef ANIM_BRIGHTNESS_REQUIRED
//   #define ANIM_BRIGHTNESS_REQUIRED true // when controlling it directly
//   #endif

//   #define ENABLE_DEVFEATURE_LIGHTING__BRIGHTNESS_ALREADY_SET_FUNCTION_ARGUMENT
  
  
//   /**
//    * @brief Feb2025 Debug serial logs
//    * 
//    */
//   // #define ENABLE_DEBUGFEATURE_LIGHTING__TRACE_PIXEL_SET_GET_SHOW_FIRST_NUMBER_LOGGED_WITH_VALUE 1
//   // #define ENABLE_DEVFEATURE_LIGHT__SERIAL_SHOW_PRE_EFFECT_CALL




//   // 
//   /**
//    * Hardware 
//    */
//   #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//   /**
//    * Software: Fully or largely incorporated
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//   // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

//   ///////// Presets ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   ///////// Playlists ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   /**
//    * Software: Testing and not yet considered "standard"
//    **/ 
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL


//   #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
//   #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #endif
  
//   #define DEBUG_ASYNC
//   #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
//   #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE
//   /**
//    * Hardware 
//    */
//   #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT

//   /**
//    * Software: Fully or largely incorporated
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//   // #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC

//   ///////// Presets ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   ///////// Playlists ///////////////////////////////////////////////////////////////////////////////
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHT__PLAYLISTS_2024
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   /**
//    * Software: Testing and not yet considered "standard"
//    **/ 
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS
//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER
//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL


//   #define ENABLE_DEVFEATURE_TIME__TIME_SHORT_FUNCTIONS
//   #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_TIMING
//   #define ENABLE_DEVFEATURE_LIGHTING__OCT24_COLOUR_ORDER
//   // #ifndef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   // #endif
  
//   #define DEBUG_ASYNC
//   #define ENABLE_DEVFEATURE_WEBPAGE__FORCE_NO_CACHE_WITH_RELOAD_ON_WEB_REFRESH
//   #define ENABLE_DEVFEATURE_LIGHTING__DEFAULT_ESP32_BUTTON_IS_DEMO_SEQUENCE

 
//   // #define ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D

//   // temporary fix until rgbww is added with matrix support functions
//   #ifndef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE // setPixelXY needs to be added to rgbww

//   // #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
//   #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
//     #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
//     #define WLED_DEBUG
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
//   #endif 

//   #endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE


//   // #define ENABLE_DEVFEATURE_LIGHT__CREATE_MATRIX_IMMEDIATELY

//   #if defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D) || defined(ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D)
//     #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
//   #endif
  

//   #define ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS

//   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
//   // #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG


//   #define ENABLE_DEVFEATURE_LIGHT__PWM_DITHER_V2
  
  
//   #define ENABLE_DEVFEATURE_WEBSERVER__ETAGS_ENABLED_FOR_RELOADING_PALETTES_ON_FRESH_COMPILE


//   #define ENABLE_DEVFEATURE_LIGHTING__SUPPRESS_WHITE_OUTPUT
  
//   // #define WLED_ENABLE_WEBSOCKETS
//   #define WLED_ENABLE_WEBSOCKETS2
//   #define ENABLE_DEVFEATURE_LIGHTING__JSONLIVE_WEBSOCKETS
//   #define WLED_ENABLE_JSONLIVE

//   #define ENABLE_ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

// #endif // 


























// /**
//  * Going forward, here will support define that simply insert in firmwarecustom
//  * Ie, instead of bathroom/ensuite/bedroom sensor lighting type having the same ifdefs configed, here will
//  * be the define for it
//  * 
//  * Update Date: 2 Feb 23
// */

// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS

//   #error "FIRMWARE removed"
//   #error "REMOVED"
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
    
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
//     // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL
//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items
    
//     #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
//     #define USE_SK6812_METHOD_DEFAULT

//     #error "Remove this and follow normal method"
    

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS

// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF


//   #error "FIRMWARE removed"
//   #ifdef USE_MODULE_LIGHTS_INTERFACE
//   #define USE_SK6812_METHOD_DEFAULT
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   R"=====(
//   {
//     "HardwareType":"SK6812",
//     "AnimationMode":"Effects",
//     "ColourOrder":"grbw",
//     "ColourPalette":"Rgbcct 01",
//     "Effects": {
//       "Function":0,
//       "Intensity":50
//     },
//     "Transition": {
//       "TimeMs": 0,
//       "RateMs": 1000
//     },
//     "SegColour": {
//       "Hue": 120,
//       "Sat": 100,
//       "SubType":3
//     },
//     "BrightnessRGB": 0,
//     "BrightnessCCT": 0
//   }
//   )=====";
//   #endif // USE_MODULE_LIGHTS_INTERFACE


//   #error "Remove this and follow normal method"

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF


// /**
//  * After WEBUI added and multipin testing, consolidate into one define
//  * 
//  * Update Date: 13 August 2023
//  * End Date: This should be removed
// */
// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

//   #error "REMOVED"
//   #error "FIRMWARE removed"
//   #error "switch to USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023"

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
    
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD
//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL
    
    
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW
//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

    

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS


// /**
//  * Reducing options where they can be
//  * 
//  * Update Date: 21 September 2023
// */

// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023

//   #error "REMOVED"
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//     /********* Group: Testing ************************/
//       // Phase out
//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEBUG_SERIAL    

    
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS
//   #define USE_MODULE_CORE_FILESYSTEM
//   #define WLED_ENABLE_FS_EDITOR
//   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS


// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS

// /**
//  * @brief Going to start the minimal options this file from now on, and slowly over time will become the main base config for lighting
//  * 
//  */
// #ifdef FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24

//   #error "REMOVED"
//   #error "FIRMWARE removed"


//   /***********************************
//    * SECTION: System Debug Options
//   ************************************/    
//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING
  
//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

//   // #define ENABLE_DEBUG_LINE_HERE_TRACE
//   // #define ENABLE_DEBUG_LINE_HERE

//   // #define ENABLE_FREERAM_APPENDING_SERIAL

//   // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE



//   /***********************************
//    * SECTION: System Configs
//   ************************************/     

//  #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  

  

//   #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

//   // #define USE_MODULE_CORE_FILESYSTEM
//   //   #define WLED_ENABLE_FS_EDITOR
//   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
//   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

//   // Settings saving and loading
//   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
//   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
//   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
//   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
//   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    
//  #ifndef ESP8266
//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI
// #endif
//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  

//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
//   

//   #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

//   #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER

//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
  
//   #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    
//   // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

  
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//     /********* Group: Testing ************************/
//       // Phase out
//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     /********* Group: Debug options only ************************/
//     // #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEBUG_SERIAL    

    
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


//   // #define USE_FUNCTION_TEMPLATE
//   // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   // "{"
//   //   "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
//   //   "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
//   // "}";



// #endif // FIRMWARE_DEFAULT__LIGHTING__ESP32_OPTIONS_MINIMAL__MAY24


// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

//   #error "FIRMWARE removed"
//   #error "REMOVED"
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//     /********* Group: Testing ************************/
//       // Phase out
//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     /********* Group: Debug options only ************************/
//     // #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEBUG_SERIAL    

    
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS


// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023

//   #error "REMOVED"
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//     /********* Group: Testing ************************/
//       // Phase out
//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     /********* Group: Debug options only ************************/
//     // #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEBUG_SERIAL    

    
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   #define ENABLE_FEATURE_LIGHTING__EFFECTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS


// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023_ESP8266

//   #error "FIRMWARE removed"
//   #error "REMOVED"

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//     /********* Group: Testing ************************/
//       // Phase out
//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEBUG_SERIAL    

    
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
//   // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS



// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_RGBCCT_PWM_H801

//   #error "FIRMWARE removed"
//   #error "REMOVED"
//   #define USE_SERIAL_ALTERNATE_TX

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_PWM  
    
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     // 
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL
//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items
    

// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_RGBCCT_PWM_H801


// #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_RGBCCT_PWM_H801_DECEMBER2023

//   #error "FIRMWARE removed"
//   #error "REMOVED"
//   #define USE_SERIAL_ALTERNATE_TX

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_PWM  
    
//     // /********* Group: Needed to build ************************/
//     // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     // /********* Group: Ready for full integration ************************/
//     // // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     // /********* Group: Testing ************************/
//     // // 
//     // #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     // 
//     // 
//     // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     // // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
//     // // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     // // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     // // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
//     // // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
//     // // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
//     // // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     // /********* Group: Debug options only ************************/
//     // // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     // #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     // #define ENABLE_DEBUG_SERIAL
//     // // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // // #define ENABLE_LOG_LEVEL_DEBUG
//     // // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items
    

//   #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
//   #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
//   // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
//   #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
//   // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS

  
//   #define USE_MODULE_SENSORS_SUN_TRACKING

  
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS 
//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20
//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
//   #define ENABLE_DEBUG_MANUAL_DELAYS
//   #define ENABLE_DEVFEATURE_PALETTE__CHANGE_MY_PALETTE_INDEXING_TO_255_RANGE


// #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_RGBCCT_PWM_H801




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



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*************************************************************************************************************************************************
****** SECTION: Base defines for specialised hardware ******************************************************************************************************************
****************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*******************************************************************************************************************************************/





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*************************************************************************************************************************************************
****** SECTION: Base defines for just lighting ******************************************************************************************************************
****************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*******************************************************************************************************************************************/







/**************************************************************************************************************************************************
***************************************************************************************************************************************************
**************************************************************************************************************************************************
***************************************************************************************************************************************************
**************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*************************************************************************************************************************************************
****** SECTION: Boards for testing ******************************************************************************************************************
**************************************************************************************************************************************************
***************************************************************************************************************************************************
**************************************************************************************************************************************************
***************************************************************************************************************************************************
**************************************************************************************************************************************************
***************************************************************************************************************************************************
****************************************************************************************************************************************************
***************************************************************************************************************************************************
***************************************************************************************************************************************************
*******************************************************************************************************************************************/







/**
 * @brief This method allows the storage to be loaded, but will always then load templates and override anything from settings
 *        to make sure the device starts in a known state. This will make sure SSID etc are loaded 
 */
#ifdef ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE

  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    #define WLED_ENABLE_FS_EDITOR
    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
    #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM




  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_HOUR
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    


#endif // ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE


/**
 * @brief This is for release to other people. A templated version will be compiled, and enable an ESP to be flashed
 *        and then fully configured via the webui. This will include SSIDs, MQTT, and other settings, so will require settings
 *        to be fully working and with fallback to starting in direct wifi mode for webpage configuration
 */
#ifdef ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_ONLY_ON_RESET





#endif // ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_ONLY_ON_RESET



/**
 * @brief To enable ESP to load and save playlists, with access to the file edit URL
 * 
 */
#ifdef ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS


  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    #define WLED_ENABLE_FS_EDITOR
    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
    #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

#endif // ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS


// Until fully working, just enable by group [CORE / NETWORKING / LIGHTING / DRIVERS / SENSORS]
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS

  #define USE_MODULE_CORE_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    #define WLED_ENABLE_FS_EDITOR


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
#undef ENABLE_WEBSERVER_LIGHTING_WEBUI
#define DISABLE_NETWORK


#endif // ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING


#endif