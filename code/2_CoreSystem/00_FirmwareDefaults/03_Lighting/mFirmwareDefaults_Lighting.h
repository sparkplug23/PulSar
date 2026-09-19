
#ifndef _FIRMWARE_DEFAULTS__LIGHTING_H_
#define _FIRMWARE_DEFAULTS__LIGHTING_H_


#ifdef FIRMWARE_DEFAULT__USE_DEFAULT2
#error "throw error - should not reach this file"
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

// #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA // FORCE ON
// #define FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES
  

// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D // auto inherit all baseline COMPLETE + 2D


  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE // Inherit base config

  #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
  
  // temporary fix until rgbww is added with matrix support functions
  // #ifndef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE // setPixelXY needs to be added to rgbww
  #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_2D
  // #endif // ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

#endif


// -------------------------------------------------------------------------------------------------------------------------------------------------------

// For Jan2026, BETA is alway considered as complete.
#if defined(FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA) ||  defined(FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE)
// #ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA

  /**
   * Permenant Development Tools
   **/
  #define ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS__ANIMATOR  
  #define ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS

  /**
   * Actual beta features 2026
   **/
  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__LOAD_BY_NAME
  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__AUTOGENERATE_ID_LIST  
  #define ENABLE_FEATURE_LIGHTING__PRESETS__DEBUG



  // Everything moved into complete, anything working unless in tests phase will be phased in.


  
  #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_AFTER_SUCCESSFUL_BOOT_TEST
  #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_MODULE_DATA  
  #define ENABLE_FEATURE_LIGHTING__CORE__DATABUFFER_LOCK
  // #define USE_MODULE_SENSORS_SUN_TRACKING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED

  #ifndef ENABLE_FEATURE_LIGHTING__DISABLE_WEBPAGE_TO_REDUCE_MEMORY_USAGE
    #define ENABLE_FEATURE_LIGHTING__WEBUI__XML_API
    #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_LEDS
    #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_SYNC
    #define ENABLE_FEATURE_LIGHTING__WEBUI__URL_QUERY_SETTINGS 
  #endif


  #define ENABLE_FEATURE_LIGHTING__PRESETS__LOAD_FROM_FILE
  
  #define ENABLE_FEATURE_LIGHTING__OUTPUT__SUPPRESS_WHITE // Fix flickering of white channel

  /************************************************************************
   * SECTION: defines for external libaries
   ************************************************************************/
  #define ENABLE_DEBUG_ASYNC


#endif

// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE
  /**
   * SOUND: Reactive can be just 1D or 2D, so it does not really cause 2D base, for sound+2D 2D still needs manually added
   **/

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE // Inherit base config

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D
  
  #if defined(ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D) || defined(ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_2D)
    #define ENABLE_FEATURE_LIGHTING__AUDIO__USERMOD_IMPLEMENTATION
  #endif

#endif

// -------------------------------------------------------------------------------------------------------------------------------------------------------


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE


#ifdef ENABLE_FEATURE_LIGHTING__DISABLE_WEBPAGE_TO_REDUCE_MEMORY_USAGE // lower memory footprint
  #warning "WEBPAGE disabled for lighting to conserve memory
#else // default includes full webpage


  /************************************************************************
   * WEBPAGE:
   ************************************************************************/
  
  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_FEATURE_LIGHTING__WEBUI__CORE
  #define ENABLE_FEATURE_LIGHTING__WEBUI__SHOW_BUILD_DATETIME


  #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_POLLING
  #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET
  #define ENABLE_FEATURE_LIGHTING__WEBUI__CAPTIVE_PORTAL

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SHARED_STYLES

  
  #define ENABLE_FEATURE_LIGHTING__GAMMA__SKIP_PULSAR_NATIVE_PALETTES

  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM

  #define ENABLE_FEATURE_LIGHTING__CORE__PIXEL_DECIMATION

  
  #ifndef ESP8266
    #define ENABLE_FEATURE_LIGHTING__WEBUI__ADVANCED_PAGES
  #endif


#endif

  /************************************************************************
   * SECTION: Inherit other defaults
   ************************************************************************/
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC

  /************************************************************************
   * SECTION: New defines
   ************************************************************************/
  #define ENABLE_FEATURE_JSON__ASYNCJSON_V6

  #define  ENABLE_FEATURE_LIGHTING__CORE__PERSISTENT_CONFIG

  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define ENABLE_FEATURE_LIGHTING__ANIMATOR__GLOBAL_LIGHT_ACCESS

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__CHRISTMAS_PRECOMPUTE_POWF

  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__PRIMARY_JSON_COMMANDS

  #define ENABLE_FEATURE_LIGHTING__CORE__PRESETS
  #define ENABLE_FEATURE_LIGHTING__CORE__PLAYLISTS

  /************************************************************************
   * EFFECTS: Just have 1 line to turn on effect parts. 
   ************************************************************************/

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE
  // // #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SOLAR_POSITION
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_CHRISTMAS_CONTROLLER
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_HARDWARE_TESTING
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_EXTERNAL_MODULE_CONTROL
  
  // #define ENABLE_FEATURE_LIGHTING__AUDIO__USERMOD_IMPLEMENTATION
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D

  ////////////// other

  #define ENABLE_FEATURE_LIGHTING__CORE__EFFECT_ENGINE
  #define ENABLE_FEATURE_LIGHTING__CORE__PIXEL_DECIMATION
  
  
  #define ENABLE_ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE

  #define ENABLE_FEATURE_FILESYSTEM__ADD_TIMESTAMP_ON_SAVE_FILES


  #define ENABLE_FEATURE_LIGHTING__CORE__AUTOMATION_PRESETS
  #define ENABLE_FEATURE_LIGHTING__PRESETS__FILE_METADATA
  #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  #define ENABLE_FEATURE_LIGHTING__CORE__GAMMA_CORRECTION
  
  #define PHASEIN_ANIM_BRIGHTNESS_REQUIRED_AS_TRUE true // MUST be true, as we are not using NPB_LG method


  
  /************************************************************************
   * FILESYSTEM: 
   ************************************************************************/
  #define USE_MODULE_CORE_FILESYSTEM

#endif


// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC

  /************************************************************************
   * FILESYSTEM: 
   ************************************************************************/

  /************************************************************************
   * WEBPAGE:
   ************************************************************************/

  /************************************************************************
  * LIGHTING:
  ************************************************************************/


  /************************************************************************
   * EFFECTS: 
   ************************************************************************/
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME

  /************************************************************************
   * BUS:
   ************************************************************************/


  /************************************************************************
   * BRIGHTNESS:
   ************************************************************************/

  /************************************************************************
   * PRESETS:
   ************************************************************************/

  /************************************************************************
   * PLAYLISTS:
   ************************************************************************/


#endif



// This stays after the lighting, since depending on basic/advanced what is included here will differ
// long term though, as solar is complex, we need to make a high speed version
// ie, at boot (or midnight), create a TOD vs elevation/azimuth map (this can probably be done with simple sine wave and TOD offset)
// Lighting infact, should never use the complex version which will remain of rules based only.
#ifdef FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES


  // #define USE_MODULE_SENSORS_SUN_TRACKING     
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
  //   #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
  //   // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SOLAR_POSITION

  // #define USE_MODULE_SENSORS_SUN_TRACKING

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





#ifdef ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_DEMO_MODE
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
    

  // #define ENABLE_FEATURE_LIGHTING__INPUT__BUTTON_CONTROLS
  // #define ENABLE_FEATURE_LIGHTING__DEMO_MODE


#endif // ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_DEMO_MODE

  
#endif