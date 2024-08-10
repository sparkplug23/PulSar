#ifndef _CONFIG__FIRMWARE_GROUP__LIGHTING_EFFECTS_H
#define _CONFIG__FIRMWARE_GROUP__LIGHTING_EFFECTS_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are unique with lighting
 *  ** Any second versions of these devices should be placed into other files
 *  ** This list should track the ini file
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "6_Lights/03_Animator/EffectNames/defines.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

//--------------------------------[Enable Device]-------------------------------------

//    ;;;;;;;;;;;; ESP32 ;;;;;;;;;;;;;;;;
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__01__ESP32_1CH                               // original, no parallel
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__02__ESP32_PARALLEL_4CH                      // garage (100 x4) - this could become the garden lights for a practical test                     
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__03__ESP32_PARALLEL_8CH                      // garage (100 x8)                         
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__04__ESP32_PARALLEL_16CH                     // garage (100 x16)                        
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__05__ESP32_PARALLEL_4CH_VARIED_BUSTYPE       // varied strings (ws2812, sk6812)
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__06__ESP32_1CH_WS2805_3X                     // new RGBCCT 3X 12V pixels
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__07__ESP32_1CH_7SEGMENTCLOCK                 // ESP32 testing 3D printed 7 segment clock
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__08__ESP32_1CH_NOWEBUI                       // ESP32 - Single Bus - Single Segment - 100 rgb leds 
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__20__ESP32_1CH_MATRIX_16X16                  // ESP32 testing 16x16 matrix
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__21__ESP32_1CH_MATRIX_32X8                   // ESP32 testing 32x8 matrix
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__30__ESP32_PWM_RGBCCT_5CH_RGBCCT              // Garage as lighting at night, long term tester
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__31__ESP32_PWM_RGBCCT_2x2CH_WHITE_CHANNELS   // For testing the dual white channels
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__32__ESP32_PWM_RGBCCT_5x1CH_WHITE_CHANNELS   // For testing single white channels
#define DEVICE_TESTGROUP__LIGHTING_EFFECTS__41__ESP32_PARALLEL_4CH_DEV                  // desk/wall testbed (10 leds per channel) 
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__42__ESP32_PWM_RGBCCT_5CH_RGBWW_DEV          // Desk/under Testbed - do sun elevation white control

//    ;;;;;;;;;;;; ESP8266 ;;;;;;;;;;;;;;;;
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__50__ESP82_1CH                               // ESP8266 - Single Bus - Single Segment - 100 rgb leds
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__51__ESP82_1CH_NOWEB                         // ESP8266 - Single Bus - Single Segment - 100 rgb leds
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__52__ESP82_H801_5CH_PWM_RGBCCT               // normal H801
        // #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__53__ESP82_LOW_MEMORY                        // for developing the lighting module with low memory, this will enable it to be used in complex systems with many other modules
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__54__ESP82_MINIMAL_OTA_MIDSTAGE


// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__BASE

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__BASE


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE



  /***********************************
   * SECTION: System Configs
  ************************************/     

 #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  


  /***********************************
   * SECTION: Network Configs
  ************************************/    
 #ifndef ESP8266
  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
#endif
  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM

  #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

  #define ENABLE_DEVFEATURE_LIGHT__HIDE_CODE_NOT_ACTIVE_TO_BE_INTEGRATED_LATER

  #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.
  






  
  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE








  #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
  #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
    /********* Group: Testing ************************/
    #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS  // Phase out
    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    /********* Group: Debug options only ************************/
    // #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEBUG_SERIAL    

    
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE
  // #define ENABLE_NEOPIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

    
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS
  #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  // #define ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  #define ENABLE_FEATURE_LIGHTING__EFFECTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
  #define ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS_DEBUG_LINES
  // #define ENABLE_DEVFEATURE_LIGHTING__SETTINGS


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";



#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__BASE




/************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************
  @brief TESTGROUP__LIGHTING
 ************************************************************************************************************************************************************************
 *************************************************************************************************************************************************************************/



/**
 * @brief 
 * Wall floor for testing I2S without parallel. 
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__01__ESP32_1CH
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

 #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
 
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":60
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        60
      ],
      "ColourPalette":"Sunset",
      "Effects": {
        "Function":"Hour Progress",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__01__ESP32_1CH




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__02__ESP32_PARALLEL_4CH
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  #define STRIP_SIZE_MAX 600
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      },
      {
        "Pin":18,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":100
      },
      {
        "Pin":19,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        400
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 35,
    "BrightnessCCT": 0
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_I2S_PARALLEL_4CH


/**
 * @brief 
 * Two RGB and two RGBW output pins
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__05__ESP32_PARALLEL_4CH_VARIED_BUSTYPE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME          "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME  "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE          "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT      "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  // /**
  //  * @brief 
  //  * 
  //  * RGB first channels
  //  * RGBW second channels
  //  * 
  //  */
  // #define STRIP_SIZE_MAX 600
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":2,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":100
  //     },
  //     {
  //       "Pin":4,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":100,
  //       "Length":100
  //     },
  //     {
  //       "Pin":18,
  //       "ColourOrder":"RGBW",
  //       "BusType":"SK6812_RGBW",
  //       "Start":200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"RGBW",
  //       "BusType":"SK6812_RGBW",
  //       "Start":300,
  //       "Length":100
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       400
  //     ],
  //     "ColourPalette":"Snowy 02",
  //     "Effects": {
  //       "Function":"Static Palette",
  //       "Speed":127,
  //       "Intensity":127,
  //       "Grouping":1
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100
  //   },
  //   "BrightnessRGB": 35,
  //   "BrightnessCCT": 0
  // }
  // )=====";
  

  /**
   * @brief 
   * RGB first channels
   * RGBW second channels
   */
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":144
      },
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":144,
        "Length":144
      },
      {
        "Pin":18,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":288,
        "Length":100
      },
      {
        "Pin":19,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":388,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        488
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 35,
    "BrightnessCCT": 0
  }
  )=====";
  
#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__05__ESP32_PARALLEL_4CH_VARIED_BUSTYPE




/**
 * @brief 
 * Wall floor for testing I2S without parallel. 
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__06__ESP32_1CH_WS2805_3X
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define ENABLE_DEVFEATURE__WS2805

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

 #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
 
    
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"RGBWC",
        "BusType":"WS2805_RGBWW",
        "Start":0,
        "Length":60
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        60
      ],
      "ColourPalette":"Sunset",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 5
    },
    "BrightnessRGB": 5,
    "BrightnessCCT": 5
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__06__ESP32_1CH_WS2805_3X







/**
 * @brief 
 * Wall floor for testing I2S without parallel. 
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__07__ESP32_1CH_7SEGMENTCLOCK
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  #define LOG_BUFFER_SIZE 2000

  /***********************************
   * SECTION: System Configs
  ************************************/     
 
  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  #define ENABLE_DEVFEATURE_LIGHT__FORCE_EFFECT_COLOUR_TYPE_AS_RGBCCT

  #define ENABLE_DEVFEATURE_SETTINGS__TEXT_BUFFER
  #define DEBUG_FUNC_SETTINGSUPDATETEXT

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Core Configs
  ************************************/    


  /***********************************
   * SECTION: Network Configs
  ************************************/    



  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  
 
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS
  #define ENABLE_FEATURE_LIGHTS__ADD_WHITE_TO_LIVEVIEW

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":93
      }
    ],
    "Segment0": {
      "PixelRange":[0,93],
      "ColourPalette":"Party 16",
      "Effects": {
        "Function":"Clock Basic 01"
      },
      "SegColour0": {
        "Hue": 0,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":100,
        "ColourType":4
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 100
  }
  )=====";
  

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__07__ESP32_1CH_7SEGMENTCLOCK





#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__20__ESP32_1CH_MATRIX_16X16
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     

//   #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

//   #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

//   // #define USE_MODULE_DRIVERS_FILESYSTEM
//   //   #define WLED_ENABLE_FS_EDITOR
//   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
//   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

//   // Settings saving and loading
//   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
//   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
//   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
//   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
//   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
//   #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
//   #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

//   // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//   // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

//   // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

//   #define USE_MODULE_SENSORS_SOLAR_LUNAR


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

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

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MATRIX
    #define ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP  
    #define WLED_DEBUG
    #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
    // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_LOAD_PALETTE_PATCH_IN_WEBUI_PALETTE_CHANGE
  #endif 

//   #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM


//   #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.



//   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
//   #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
//   #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
//   #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
//   #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI


  

  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // 21 = on PCB (4 pixels)
  // 32 = external
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":32,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":256
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        256
      ],
      "ColourPalette":"Party 16",
      "PaletteMappingValues":[10,15,20],
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":1
      },
      "SegColour1": {
        "Hue": 120,
        "Sat":20,
        "BrightnessRGB":1
      },
      "SegColour2": {
        "Hue": 240,
        "Sat":100,
        "BrightnessRGB":1
      },
      "Effects": {
        "Function":"Spanned Palette",
        "Speed":127,
        "Intensity":127,
        "Decimate":0,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 20,
    "BrightnessCCT": 0
  }
  )=====";

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       #ifdef USE_MODULE_DRIVERS_RELAY
//       "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
//       "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
//       "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
//       "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
//       #endif
//       "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
//       "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
//       #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
//       "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
//       #endif    
//       #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
//       "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
//       #endif
//       #ifdef USE_MODULE_DISPLAYS_NEXTION
//       "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
//       "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
//       #endif
//       "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
//       // 32 - LED Strip External
//       // 21 - LED Strip Onboard
//       // 25?
//       // 
//     "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


//   /**
//    * @brief Drivers and Sensors for HVAC zones
//    **/
//   #define D_DEVICE_HEATER_0_NAME "Dryer"
//   #define D_DEVICE_HEATER_1_NAME "FloorMat"
//   #define D_DEVICE_HEATER_2_NAME "FanHeater"
//   #define D_DEVICE_HEATER_3_NAME "OilRadiator"

//   #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// // {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

//   /** 
//    * MainBoard
//    * */
//   #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
//   #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

//   #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
//   #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

//   #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
//   #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

//   #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
//   #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

//   #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
//   #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

//   #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

//   #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
//   #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
//   #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
//   #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
//   #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
//   #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
//   #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
//   #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
//   #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
//   #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
//   #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
//   #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_ENERGY "\":{"
//         "\"DeviceCount\":4"    
//     "},"
//     "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":{"
//         "\"DeviceCount\":4"    
//     "},"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
//         // Downstairs
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SOLAR_LUNAR_FRIENDLY_CTR "\":["
//         "\"" "Desk" "\""
//       "],"  
//       "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
//         "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_INA219_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_CURRENT "\""
//       "],"
//       "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
//       "],"
//       "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
//         "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
//         "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_HEATER_0_NAME "\","
//         "\"" D_DEVICE_HEATER_1_NAME "\","
//         "\"" D_DEVICE_HEATER_2_NAME "\","
//         "\"" D_DEVICE_HEATER_3_NAME "\""
//       "]"
//     "},"
//     "\"" D_JSON_SENSORADDRESS "\":{"
//       "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
//         // Downstairs
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
//         "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
//       "},"  
//       "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
//         D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
//         D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
//       "]"  
//     "},"
//     "\"" "HVACZone" "\":{"
//       "\"" "SetSensor" "\":["
//         "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
//       "],"
//       "\"" "SetOutput" "\":["
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "},"
//         "{"
//           "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
//           "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
//           "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
//         "}"
//       "]"
//     "},"
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
//   "}";



  
#endif



#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__41__ESP32_PARALLEL_4CH_DEV
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  // #define ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  // New way to start the save into memory periodically, and then recover if available on boot. Init phase of full system.
  #define ENABLE_FILESYSTEM__MODULES_CORE__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_CORE__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_SENSORS__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_SENSORS__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__RESTORE_ON_BOOT
  

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  
 
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__LOG_MESSAGES

    // #undef ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    // #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  // #define ENABLE_DEVFEATURE__LIGHTING_TEMPLATE__GAZEBO
  #define ENABLE_DEVFEATURE__LIGHTING_TEMPLATE__TEST

  #if defined(ENABLE_DEVFEATURE__LIGHTING_TEMPLATE__GAZEBO)
  
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)  // side 2
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      },
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "Pin":19,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        500
      ],
      "ColourPalette":"Sweetpea",
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "Segment1": {
      "PixelRange": [
        500,
        750
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Slow Glow",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "Segment2": {
      "PixelRange": [
        750,
        1000
      ],
      "ColourPalette":172,
      "CustomPalette":{"Encoding":"RGB","Index":9,"Data":[55,55,55,0,255,0]},
      "Effects": {
        "Function":"Static Palette",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  #elif defined(ENABLE_DEVFEATURE__LIGHTING_TEMPLATE__TEST)

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)  // side 2
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      },
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      },
      {
        "Pin":18,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":500,
        "Length":250
      },
      {
        "Pin":19,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":750,
        "Length":250
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        1000
      ],
      "ColourPalette":"Cyan",
      "Effects": {
        "Function":"Candles",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";



  #else

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":10
      },
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":10,
        "Length":10
      },
      {
        "Pin":18,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":20,
        "Length":10
      },
      {
        "Pin":19,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":30,
        "Length":10
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        40
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Slow Glow",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 500,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  
  #endif // 

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L1__ESP32_I2S_PARALLEL_4CH



#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__30__ESP32_PWM_RGBCCT_5CH_RGBCCT 
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

  // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES



  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

  // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  

  // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // // #define DISABLE_NETWORK
  // // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_WIFI
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 1
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":[5,18,19,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],    
    "Segment0":{
      "PixelRange": [
        0,
        1
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 0,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":100,
        "ColourType":4
      },
      "Effects": {
        "Function":0
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB":100,
      "BrightnessCCT":100
    },
    "BrightnessRGB":100,
    "BrightnessCCT":100
  }
  )=====";


#endif





#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__51__ESP82_1CH_NOWEB 
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

  // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION

  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  // #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  // #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define DEBUG_FASTBOOT

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

  // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  // #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE
  

  // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // // #define DISABLE_NETWORK
  // // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_WIFI
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  #define STRIP_SIZE_MAX 600
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":60
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        60
      ],
      "ColourPalette":"Sunset",
      "Effects": {
        "Function":"Hour Progress",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";


#endif



#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__52__ESP82_H801_5CH_PWM_RGBCCT 
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

  // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES



  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

  // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  

  // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // // #define DISABLE_NETWORK
  // // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_WIFI
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 1
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":[15,13,12,14,4],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],    
    "Segment0":{
      "PixelRange": [
        0,
        1
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 0,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":100,
        "ColourType":4
      },
      "Effects": {
        "Function":0
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB":100,
      "BrightnessCCT":100
    },
    "BrightnessRGB":100,
    "BrightnessCCT":100
  }
  )=====";


#endif



#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__53__ESP82_LOW_MEMORY 
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

  // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION

  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  // #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  // #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES



  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

  // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  // #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE
  

  // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // // #define DISABLE_NETWORK
  // // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_WIFI
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 1
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":[15,13,12,14,4],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],    
    "Segment0":{
      "PixelRange": [
        0,
        1
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 0,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":100,
        "ColourType":4
      },
      "Effects": {
        "Function":0
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB":100,
      "BrightnessCCT":100
    },
    "BrightnessRGB":100,
    "BrightnessCCT":100
  }
  )=====";


#endif


/**
 * @brief OTA to be used when a device does not have enough memory to allow full OTA.
 * Flash this first, it should have recovery but otherwise no other features.
 * A full OTA should then be possible.
 * 
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__54__ESP82_MINIMAL_OTA_MIDSTAGE 
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP8266 NoWEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

  // #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_DEBUG_LINE_HERE_TRACE
  #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define USE_TEMPLATED_DEFAULT_OTA_RECOVERY_METHODS

  #define FIRMWARE_MINIMAL2

  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  // #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  // #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES



  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

  // #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  

  // // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  // #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // // #define DISABLE_NETWORK
  // // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_WIFI
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIO_FUNCTION "\":{},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
  //   "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  // #define USE_LIGHTING_TEMPLATE
  // #define STRIP_SIZE_MAX 1
  // #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":[15,13,12,14,4],
  //       "ColourOrder":"RGBCW",
  //       "BusType":"ANALOG_5CH",
  //       "Start":0,
  //       "Length":1
  //     }
  //   ],    
  //   "Segment0":{
  //     "PixelRange": [
  //       0,
  //       1
  //     ],
  //     "ColourPalette":0,
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat": 100,
  //       "BrightnessRGB": 100,
  //       "BrightnessCCT": 100,
  //       "CCT_TempPercentage":100,
  //       "ColourType":4
  //     },
  //     "Effects": {
  //       "Function":0
  //     },
  //     "Transition": {
  //       "TimeMs": 900,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB":100,
  //     "BrightnessCCT":100
  //   },
  //   "BrightnessRGB":100,
  //   "BrightnessCCT":100
  // }
  // )=====";


#endif

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//-----SORT ALL DEVICES BELOW THIS LINE ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------





/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L3__ESP8266_1CH_RGB_100LEDS
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  // #define NEOPIXEL_DISABLE_I2S0_PIXELBUS
  // #define NEOPIXEL_DISABLE_I2S1_PIXELBUS

  #define ESP8266
  #undef  ESP32

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define DATA_BUFFER_PAYLOAD_MAX_LENGTH 3000

  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES
    
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_BUILD__RELEASE_TO_OTHERS_WITHOUT_NETWORKING 

  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023
  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023_ESP8266
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":3"   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
  //   "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  // "}";


  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 100
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":3,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":"Christmas Snowy 02",
      "Effects": {
        "Function":"Slow Glow",
        "Speed":127,
        "Intensity":127,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 500,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 100
  }
  )=====";

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L3__ESP8266_1CH_RGB_100LEDS














































































// define all shared params here, and only device related in "subdevice"
#ifdef DEVICE_TESTGROUP__LIGHTING_MULTIPIN

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS



  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
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
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    

    
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



// #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

// #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



#endif // DEVICE_TESTGROUP__LIGHTING_MULTIPIN


#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS2
  #define DEVICENAME_CTR          "testgroup_multipin_h801_rgbsplitcct"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 RGB Split CCT Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


//   #define USE_SERIAL_ALTERNATE_TX
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   // #define USE_MODULE_DRIVERS_PWM


  

//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define USE_MODULE_NETWORK_WEBSERVER

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS
    #define STRIP_SIZE_MAX 2
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13,12],
          "ColourOrder":"RGB",
          "BusType":"ANALOG_3CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        }
      ],    
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":90,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      },
      "BrightnessRGB":10,
      "BrightnessCCT":10
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_RGB_AND_CCT_SPLIT_CHANNELS


#ifdef SUBDEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS
  #define DEVICENAME_CTR          "testgroup_multipin_h801_5white"
  #define DEVICENAME_FRIENDLY_CTR "TestGroup H801 Five White Channels"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM


  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS
    #define STRIP_SIZE_MAX 5
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          2,
          5
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },
      "BrightnessRGB":100,
      "BrightnessCCT":1
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // DEVICE_TESTGROUP__LIGHTING_MULTIPIN__H801_AS_FIVE_SINGLE_WHITE_CHANNELS






#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L4__H801_5CH_PWM_RGBCCT
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM

#define USE_SERIAL_ALTERNATE_TX
  

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER
    // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  // #define ESP8266
  // #undef ESP32
  

  


    



  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

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
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define USE_MODULE_NETWORK_WEBSERVER

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP


  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"    // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 2
  #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":[15,13,12,14,4],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      }
    ],    
    "Segment0":{
      "PixelRange": [
        0,
        1
      ],
      "ColourPalette":10,
      "SegColour0": {
        "Hue": 0,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":90,
        "ColourType":4
      },
      "Effects": {
        "Function":0
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB":100,
      "BrightnessCCT":100
    },
    "BrightnessRGB":10,
    "BrightnessCCT":10
  }
  )=====";


#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L4__H801_5CH_PWM_RGBCCT








#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L5__H801_3CHRGB_AND_2CHCW
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM

#define USE_SERIAL_ALTERNATE_TX
  

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER
    // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  // #define ESP8266
  // #undef ESP32
  

  


    



  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

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
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



//   #define USE_SERIAL_ALTERNATE_TX
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   // #define USE_MODULE_DRIVERS_PWM


  

//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define USE_MODULE_NETWORK_WEBSERVER

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_SPLIT_RGB_WHITE_CHANNELS_TWO_SEGMENTS
    #define STRIP_SIZE_MAX 2
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15,13,12],
          "ColourOrder":"RGB",
          "BusType":"ANALOG_3CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[14,4],
          "ColourOrder":"CW",
          "BusType":"ANALOG_2CH",
          "Start":1,
          "Length":1
        }
      ],    
      "Segment0":{
        "PixelRange": [
          0,
          1
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":90,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          1,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      },
      "BrightnessRGB":10,
      "BrightnessCCT":10
    }
    )=====";

    /*

    {
      "Segment0": {
        "PixelRange": [
          0,
          5
        ],
        "ColourPalette": 10,
        "SegColour0": {
          "Hue": 240,
          "Sat": 0,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage": 100,
          "ColourType": 4
        },
        "Effects": {
          "Function": 0
        },
        "Transition": {
          "TimeMs":0,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT_255": 255,
        "TimeOnSecs":10,
        "Override":{"Animation_Off":{"TimeMs":10000}}
      },
      "BrightnessRGB":100,
      "BrightnessCCT": 100
    }

    */

  #endif // USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS


#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L5__H801_3CHRGB_AND_2CHCW
















#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_MODULE_DRIVERS_PWM

#define USE_SERIAL_ALTERNATE_TX
  

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER
    // #define ENABLE_WEBSERVER_LIGHTING_WEBUI

  // #define ESP8266
  // #undef ESP32
  

  


    



  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

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
    
    #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
    #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
    #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

    // Needs significant change to merge WLED and HACS
    #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD



    // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP



//   #define USE_SERIAL_ALTERNATE_TX
//   #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
//   // #define USE_MODULE_DRIVERS_PWM


  

//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define USE_MODULE_NETWORK_WEBSERVER

//   // #define ESP32
//   // #undef ESP8266

//   // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
//   #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//     #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     #define ENABLE_DEVFEATURE_REMOVE_INIT_OUTSIDE_OF_PALETTE_CLASS
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     #define ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
//     
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     #define ENABLE_DEVFEATURE_SWITCH_TO_U16_GPIO_FUNCTIONS
//     #define ENABLE_DEVFEATURE_MOVE_HARDWARE_COLOUR_ORDER_TO_BUS
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // #define ENABLE_DEVFEATURE_SHOWHARDWARE_NEOPIXEL_CANSHOW
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32__WARNING_REQUIRES_FUTURE_LOCKING_OF_UPDATES_DURING_TASK_RUNNING

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     #define ENABLE_WEBSERVER_LIGHTING_WEBUI

//     // #define ENABLE_DEVFEATURE_WEBUI__INCLUDE_URI_PRE2023



//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

//     // #define ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     #define ENABLE_DEVFEATURE_LIGHT__MOVE_ALL_BUS_STARTING_CODE_UNTIL_LOOP

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";



  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS
    #define STRIP_SIZE_MAX 5
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          2
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },      
      "Segment1":{
        "PixelRange": [
          2,
          5
        ],
        "ColourPalette":10,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":50,
          "ColourType":4
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },
      "BrightnessRGB":100,
      "BrightnessCCT":1
    }
    )=====";


#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS

#endif


/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L9__ESP8266_WEBUI
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883


  #define ENABLE_FREERAM_APPENDING_SERIAL

  

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define  ENABLE_WEBSERVER_LIGHTING_WEBUI

  #undef ESP32
  #define ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE
  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":3"   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";


  #define USE_LIGHTING_TEMPLATE
  #define STRIP_SIZE_MAX 100
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":3,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 3,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 900,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L9__ESP8266_WEBUI















#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L10__ESP32__7SEGMENTCLOCK
  #define DEVICENAME_CTR            "rgbclock_testclock_01"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Dell 32"
  #define DEVICENAME_ROOMHINT_CTR   "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
  
  /***********************************
   * SECTION: System Configs
  ************************************/    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // #define ENABLE_FEATURE_WEBSERVER__MQTT_PAYLOADS_ACCESSABLE_WITH_URL
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define USE_MODULE_NETWORK_WEBSERVER
  #define USE_MODULE_NETWORK_WEBSERVER

  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS 
  #define ANIMATION_UPDATOR_TIME_MINIMUM 20
  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
  #define ENABLE_DEBUG_MANUAL_DELAYS

  #define ENABLE_DEVFEATURE_PALETTE__CHANGE_MY_PALETTE_INDEXING_TO_255_RANGE

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

  #define USE_LIGHTING_TEMPLATE
  // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

  #define STRIP_SIZE_MAX 93 
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":93
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        93
      ],
      "ColourPalette":97,
      "SegColour0": {
        "Hue": 330,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function":"Clock Basic 01",
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 2000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

#endif






// #ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L11__ESP32_LARGE_SINGLE_PIN_TESTER
//   #define DEVICENAME_CTR          "testgroup_lighting_L11"
//   #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
//   #ifndef DEVICENAME_CTR
//     #define DEVICENAME_CTR          "testbed_default"
//   #endif
//   #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
//   #define DEVICENAME_ROOMHINT_CTR "testgroup"
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
//     #define MQTT_PORT     1883

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

//   #define ENABLE_FREERAM_APPENDING_SERIAL

//   /***********************************
//    * SECTION: System Configs
//   ************************************/    
 
//   #define SETTINGS_HOLDER 1239

//   // #define USE_MODULE_DRIVERS_FILESYSTEM
//   //   #define WLED_ENABLE_FS_EDITOR
//   //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
//   //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
//   //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM
//   //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
//   //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
//   //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
//   //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
//   //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA

    
//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   #define USE_MODULE_NETWORK_WEBSERVER
//   #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/    
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

//   #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

//   #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_PARALLEL_CHANNELS_AS_PRIMARY_METHOD
//   // #define ENABLE_DEVFEATURE_LIGHT_ESP32_RMT_METHOD_AS_PRIMARY

//     // #define USE_DEVFEATURE_PRESETS_MANUALUSERCUSTOM_OUTSIDETREE // view so I can replicate in json
//     // #define ENABLE_DEVFEATURE_DEBUG_GARGAE_PIXEL_OUTPUT

//   // #define USE_LIGHTING_TEMPLATE_DEFAULT
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE_DEFAULT) 
//   // #define USE_LIGHTING_TEMPLATE
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // R"=====(
//   // {
//   //   "BusConfig":[
//   //     {
//   //       "Pin":4,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":0,
//   //       "Length":100
//   //     },
//   //     {
//   //       "Pin":13,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":100,
//   //       "Length":200
//   //     }
//   //   ],
//   //   "Segment0": {
//   //     "PixelRange": [
//   //       0,
//   //       200
//   //     ],
//   //     "ColourPalette":"*Two Colours",
//   //     "SegColour0": {
//   //       "Hue": 0,
//   //       "Sat":100,
//   //       "BrightnessRGB":5
//   //     },
//   //     "Effects": {
//   //       "Function": 0,
//   //       "Speed":1,
//   //       "Intensity":255
//   //     },
//   //     "Transition": {
//   //       "TimeMs": 0,
//   //       "RateMs": 1000
//   //     },
//   //     "BrightnessRGB": 100,
//   //     "BrightnessCCT": 0
//   //   },
//   //   "BrightnessRGB": 5,
//   //   "BrightnessCCT": 0
//   // }
//   // )=====";



//   // // #define USE_LIGHTING_TEMPLATE_DEFAULT
//   // // DEFINE_PGM_CTR(LIGHTING_TEMPLATE_DEFAULT) 
//   // /***
//   //  * 2000 test
//   // */
//   // #define USE_LIGHTING_TEMPLATE
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // R"=====(
//   // {
//   //   "BusConfig":[
//   //     {
//   //       "Pin":4,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":0,
//   //       "Length":250
//   //     },
//   //     {
//   //       "Pin":13,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":250,
//   //       "Length":250
//   //     },
//   //     {
//   //       "Pin":14,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":500,
//   //       "Length":250
//   //     },
//   //     {
//   //       "Pin":27,
//   //       "ColourOrder":"BGR",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":750,
//   //       "Length":250
//   //     },
//   //     {
//   //       "Pin":23,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":1000,
//   //       "Length":250
//   //     },
//   //     {
//   //       "Pin":22,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":1250,
//   //       "Length":250
//   //     },
//   //     {
//   //       "Pin":21,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":1500,
//   //       "Length":250
//   //     },
//   //     {
//   //       "Pin":19,
//   //       "ColourOrder":"BGR",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":1750,
//   //       "Length":250
//   //     }
//   //   ],
//   //   "Segment0": {
//   //     "PixelRange": [
//   //       0,
//   //       2000
//   //     ],
//   //     "ColourPalette":"*Two Colours",
//   //     "SegColour0": {
//   //       "Hue": 0,
//   //       "Sat":100,
//   //       "BrightnessRGB":5
//   //     },
//   //     "Effects": {
//   //       "Function": 1,
//   //       "Speed":1,
//   //       "Intensity":255
//   //     },
//   //     "Transition": {
//   //       "TimeMs": 0,
//   //       "RateMs": 1000
//   //     },
//   //     "BrightnessRGB": 100,
//   //     "BrightnessCCT": 0
//   //   },
//   //   "BrightnessRGB": 20,
//   //   "BrightnessCCT": 0
//   // }
//   // )=====";



//   // #define USE_LIGHTING_TEMPLATE_DEFAULT
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE_DEFAULT) 
//   /***
//    * 2000 test
//   */
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   R"=====(
//   {
//     "BusConfig":[
//       {
//         "Pin":4,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":0,
//         "Length":300
//       },
//       {
//         "Pin":13,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":300,
//         "Length":300
//       },
//       {
//         "Pin":14,
//         "ColourOrder":"GRB",
//         "BusType":"WS2812_RGB",
//         "Start":600,
//         "Length":300
//       },
//       {
//         "Pin":27,
//         "ColourOrder":"BGR",
//         "BusType":"WS2812_RGB",
//         "Start":900,
//         "Length":300
//       },
//       {
//         "Pin":23,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":1200,
//         "Length":300
//       },
//       {
//         "Pin":22,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":1500,
//         "Length":250
//       },
//       {
//         "Pin":21,
//         "ColourOrder":"RGB",
//         "BusType":"WS2812_RGB",
//         "Start":1800,
//         "Length":300
//       },
//       {
//         "Pin":19,
//         "ColourOrder":"BGR",
//         "BusType":"WS2812_RGB",
//         "Start":2100,
//         "Length":300
//       }
//     ],
//     "Segment0": {
//       "PixelRange": [
//         0,
//         2400
//       ],
//       "ColourPalette":"*Two Colours",
//       "SegColour0": {
//         "Hue": 0,
//         "Sat":100,
//         "BrightnessRGB":5
//       },
//       "Effects": {
//         "Function": 1,
//         "Speed":1,
//         "Intensity":255
//       },
//       "Transition": {
//         "TimeMs": 0,
//         "RateMs": 1000
//       },
//       "BrightnessRGB": 100,
//       "BrightnessCCT": 0
//     },
//     "BrightnessRGB": 20,
//     "BrightnessCCT": 0
//   }
//   )=====";

  
//   // // #define USE_LIGHTING_TEMPLATE_DEFAULT
//   // // DEFINE_PGM_CTR(LIGHTING_TEMPLATE_DEFAULT) 
//   // #define USE_LIGHTING_TEMPLATE
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // R"=====(
//   // {
//   //   "BusConfig":[
//   //     {
//   //       "Pin":4,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":0,
//   //       "Length":100
//   //     },
//   //     {
//   //       "Pin":13,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":100,
//   //       "Length":100
//   //     },
//   //     {
//   //       "Pin":14,
//   //       "ColourOrder":"RGB",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":200,
//   //       "Length":100
//   //     },
//   //     {
//   //       "Pin":27,
//   //       "ColourOrder":"BGR",
//   //       "BusType":"WS2812_RGB",
//   //       "Start":300,
//   //       "Length":100
//   //     }
//   //   ],
//   //   "Segment0": {
//   //     "PixelRange": [
//   //       0,
//   //       400
//   //     ],
//   //     "ColourPalette":"*Two Colours",
//   //     "SegColour0": {
//   //       "Hue": 0,
//   //       "Sat":100,
//   //       "BrightnessRGB":5
//   //     },
//   //     "Effects": {
//   //       "Function": 1,
//   //       "Speed":1,
//   //       "Intensity":255
//   //     },
//   //     "Transition": {
//   //       "TimeMs": 0,
//   //       "RateMs": 1000
//   //     },
//   //     "BrightnessRGB": 100,
//   //     "BrightnessCCT": 0
//   //   },
//   //   "BrightnessRGB": 100,
//   //   "BrightnessCCT": 0
//   // }
//   // )=====";

//   /***********************************
//    * SECTION: Template Configs
//   ************************************/    

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIO_FUNCTION "\":{" 
//       #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//       // "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
//       #endif
//     "},"
//     "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";

//   /***********************************
//    * SECTION: Device Configs
//   ************************************/    

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
//     "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
//   "}";

// #endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN




#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L12__ESP32_I2S_PARALLEL_4CH_INCLUDING_DEBUG_PINS
  #define DEVICENAME_CTR          "testgroup_lighting_L12"
  #define USE_LIGHTING_TEMPLATE__PALETTE_TESTING_METAL
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "testbed_default"
  #endif
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023


      // {
      //   "Pin":4,
      //   "ColourOrder":"GRBW",
      //   "BusType":"SK6812_RGBW",
      //   "Start":0,
      //   "Length":3
      // },

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":5,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":3
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        3
      ],
      "ColourPalette":"*Two Colours",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function": 1,
        "Speed":1,
        "Intensity":255
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";



  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":1000
  //     },
  //     {
  //       "Pin":5,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1000,
  //       "Length":2000
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       2000
  //     ],
  //     "ColourPalette":"Colourful Default",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function": 1,
  //       "Speed":1,
  //       "Intensity":255
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 100,
  //   "BrightnessCCT": 0
  // }
  // )=====";

  /***********************************
   * SECTION: Debug Pins
  ************************************/    

  #define DEBUG_PIN1_GPIO       32
  #define DEBUG_PIN2_GPIO       33
  #define DEBUG_PIN3_GPIO       25
  #define DEBUG_PIN4_GPIO       14
  #define DEBUG_PIN5_GPIO       12
  #define DEBUG_PIN6_GPIO       13
  #define DEBUG_PIN7_GPIO       5 //motion pin

  #define USE_FEATURE_DEBUG_PIN_INIT_LOGIC_LEVEL_SEQUENCE_CHECK

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_02_A_CTR "\":13,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_03_A_CTR "\":14,"               // Digital WS2812
      "\"" D_GPIO_FUNCTION_PIXELBUS_04_A_CTR "\":27"               // Digital SK6812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";

#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN



/**
 * @brief Outside Tree controller should not load its saved state, but instead use the default template
 * Presets/Playlists will still need to load from the filesystem
 * 
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L14__ESP32_I2S_PARALLEL_8CH
  #define DEVICENAME_CTR          "testgroup_lighting_L13"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Outside Tree"
  #define DEVICENAME_ROOMHINT_CTR "XMAS|Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

    #error "here"

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  #define SETTINGS_HOLDER 1239

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE

  // #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM



  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":13,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      },
      {
        "Pin":14,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":100
      },
      {
        "Pin":27,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        400
      ],
      "ColourPalette":"Christmas Snowy 02",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function":"Slow Glow",
        "Speed":127,
        "Intensity":255,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 980,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";



  // // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":27,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":300
  //     },
  //     {
  //       "Pin":13,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":300,
  //       "Length":300
  //     },
  //     {
  //       "Pin":25,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":600,
  //       "Length":300
  //     },
  //     {
  //       "Pin":26,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":900,
  //       "Length":300
  //     },
  //     {
  //       "Pin":22,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1200,
  //       "Length":300
  //     },
  //     {
  //       "Pin":23,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1500,
  //       "Length":300
  //     },
  //     {
  //       "Pin":18,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":1800,
  //       "Length":300
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":2100,
  //       "Length":300
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       2400
  //     ],
  //     "ColourPalette":"Christmas Snowy 02",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function":"Slow Glow",
  //       "Speed":127,
  //       "Intensity":255,
  //       "Grouping":1
  //     },
  //     "Transition": {
  //       "TimeMs": 980,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 7,
  //   "BrightnessCCT": 0
  // }
  // )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      // "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"                // Digital WS2812
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}" 
  "}";

#endif // DEVICE_TESTBED_LIGHT_SEGMENT_ESP32__MULTIPIN





/**
 * @brief Outside Tree controller should not load its saved state, but instead use the default template
 * Presets/Playlists will still need to load from the filesystem
 * 
 */
#ifdef DEVICE_TESTGROUP__LIGHTING_EFFECTS__L16__ESP32_I2S_PARALLEL_RED_BOARD_4CH
  #define DEVICENAME_CTR          "testgroup_lighting_L15"
  #define DEVICENAME_FRIENDLY_CTR "XMAS Outside Tree"
  #define DEVICENAME_ROOMHINT_CTR "XMAS|Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/    
 
  #define SETTINGS_HOLDER 1239

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE

  #define ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM

  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":4,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      },
      {
        "Pin":18,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":100
      },
      {
        "Pin":19,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":300,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        400
      ],
      "ColourPalette":"Christmas Snowy 02",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function":"Slow Glow",
        "Speed":127,
        "Intensity":255,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 980,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 2,
    "BrightnessCCT": 0
  }
  )=====";

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{" 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      // "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":4,"
      #endif
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}" 
  "}";

#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L16__ESP32_I2S_PARALLEL_RED_BOARD_4CH











#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE_TESTGROUPS_BETA_H

