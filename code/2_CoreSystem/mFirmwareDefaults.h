
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
#include "0_ConfigUser/01_mFirmwareCustom_Secret_ExampleTemplates.h"
#include "0_ConfigUser/02_mFirmwareCustom_Secret_DevTestbeds.h"
#include "0_ConfigUser/04_mFirmwareCustom_Secret__Christmas_2023.h"
#include "0_ConfigUser/TestGroups/FirmwareGroup_LightingEffects.h"
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


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Default Firmware Configurations *******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * Going forward, here will support define that simply insert in firmwarecustom
 * Ie, instead of bathroom/ensuite/bedroom sensor lighting type having the same ifdefs configed, here will
 * be the define for it
 * 
 * Update Date: 2 Feb 23
*/

#ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
    #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
    // #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items
    
    #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
    #define USE_SK6812_METHOD_DEFAULT

    #error "Remove this and follow normal method"
    

#endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS

#ifdef USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF


  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_SK6812_METHOD_DEFAULT
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"SK6812",
    "AnimationMode":"Effects",
    "ColourOrder":"grbw",
    "ColourPalette":"Rgbcct 01",
    "Effects": {
      "Function":0,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "SegColour": {
      "Hue": 120,
      "Sat": 100,
      "SubType":3
    },
    "BrightnessRGB": 0,
    "BrightnessCCT": 0
  }
  )=====";
  #endif // USE_MODULE_LIGHTS_INTERFACE


  #error "Remove this and follow normal method"

#endif // USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF





#ifdef USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT

  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{" // for PIR to follow
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
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


#ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_RGBCCT_PWM_H801

  #define USE_SERIAL_ALTERNATE_TX

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM  
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL
    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items
    

#endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_RGBCCT_PWM_H801


/**
 * After WEBUI added and multipin testing, consolidate into one define
 * 
 * Update Date: 13 August 2023
*/

#ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #error "switch to USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023"

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD
    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP
    #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    #define ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW
    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    

#endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS


/**
 * Reducing options where they can be
 * 
 * Update Date: 21 September 2023
*/

#ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS  // Phase out
    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_SERIAL    

    
  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
  // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS
  #define USE_MODULE_DRIVERS_FILESYSTEM
  #define WLED_ENABLE_FS_EDITOR
  #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS


#endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS


#ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS  // Phase out
    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_SERIAL    

    
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_DUAL_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
  // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS

#endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS



/**
 * @brief This method allows the storage to be loaded, but will always then load templates and override anything from settings
 *        to make sure the device starts in a known state. This will make sure SSID etc are loaded 
 */
#ifdef ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE

  #define USE_MODULE_DRIVERS_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    #define WLED_ENABLE_FS_EDITOR
    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
    #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM




  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
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


  #define USE_MODULE_DRIVERS_FILESYSTEM
    #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
    #define WLED_ENABLE_FS_EDITOR
    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
    #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM



#endif // ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS



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