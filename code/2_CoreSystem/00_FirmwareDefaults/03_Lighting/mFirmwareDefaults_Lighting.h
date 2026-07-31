
#ifndef _FIRMWARE_DEFAULTS__LIGHTING_H_
#define _FIRMWARE_DEFAULTS__LIGHTING_H_



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

  #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
  
  // temporary fix until rgbww is added with matrix support functions
  // #ifndef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE // setPixelXY needs to be added to rgbww
  #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
  #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D
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
  #define ENABLE_DEVFEATURE_LIGHTS__PLAYLIST_NAME_BASED_LOADING
  #define ENABLE_DEVFEATURE_LIGHTS__PLAYLIST_BY_NAME_AUTOGENERATE_ID_LIST  
  #define ENABLE_FEATURE_LIGHTS__PRESETS_DEBUG



  // Everything moved into complete, anything working unless in tests phase will be phased in.


  
  #define ENABLE_DEVFEATURE_SETTINGS__SAVE_SETTINGS_ON_SUCCESFUL_BOOT__THEN_SPLASH_ON_REBOOT_PRIOR_TO_DEFAULT_LOAD_FOR_SAVE_TESTING
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA  
  #define ENABLE_FEATURE__DATABUFFER_LOCK
  // #define USE_MODULE_SENSORS_SUN_TRACKING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED

  #ifndef ENABLE_FEATURE_LIGHTING__DISABLE_WEBPAGE_TO_REDUCE_MEMORY_USAGE
    #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS
    #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS
    #define ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC
    #define ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS 
  #endif


  #define ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE
  
  #define ENABLE_DEVFEATURE_LIGHTING__SUPPRESS_WHITE_OUTPUT // Fix flickering of white channel

  /************************************************************************
   * SECTION: defines for external libaries
   ************************************************************************/
  #define DEBUG_ASYNC


#endif

// -------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE
  /**
   * SOUND: Reactive can be just 1D or 2D, so it does not really cause 2D base, for sound+2D 2D still needs manually added
   **/

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE // Inherit base config

  #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D
  
  #if defined(ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D) || defined(ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D)
    #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
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
  #define ENABLE_FEATURE_LIGHTING__WEBUI
  #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER


  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED

  
  #ifndef ESP8266
    #define ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES
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

  #define  ENABLE_FEATURE_FIRMWAREDEFAULT__LOAD_WITH_TEMPLATES_OVERRIDE

  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS

  #define ENABLE_FEATURE_LIGHTING__CHRISTMAS_EFFECT_PRECOMPUTE_POWF_INTO_COLOURDATA_BUFFER

  #define ENABLE_FEATURE_LIGHTS__PLAYLISTS_INCLUDE_PRIMARY_JSON_COMMANDS

  #define ENABLE_FEATURE_LIGHTS__PRESETS
  #define ENABLE_FEATURE_LIGHTS__PLAYLISTS

  /************************************************************************
   * EFFECTS: 
   ************************************************************************/

  #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  // // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
  // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER
  #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__HARDWARE_TESTING
  #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE

  #define ENABLE_FEATURE_LIGHTING__EFFECTS
  #define ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS
  
  
  #define ENABLE_ANIMATION_MODE__INTERNAL_CONTROL_FROM_ANOTHER_MODULE

  #define ENABLE_FEATURE_FILESYSTEM__ADD_TIMESTAMP_ON_SAVE_FILES


  #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  #define ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
  #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  #define ENABLE_FEATURE_LIGHTS__GAMMA_CORRECTION
  
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
  #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME

  /************************************************************************
   * BUS:
   ************************************************************************/
  #ifndef ENABLE_DEVFEATURE_NEOBUS__RMT_AS_PRIMARY
  #define ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT
  #endif

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

  // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS

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

  
#endif