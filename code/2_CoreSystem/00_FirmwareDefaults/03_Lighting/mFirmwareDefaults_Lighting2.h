#ifndef _FIRMWARE_DEFAULTS__LIGHTING2_H_
#define _FIRMWARE_DEFAULTS__LIGHTING2_H_


/**
 * ============================================================================
 * PulSar Lighting Firmware Defaults - Generation 2
 * ============================================================================
 *
 * PURPOSE
 * -------
 *
 * This file defines the high-level compile-time policy for the PulSar lighting
 * subsystem.
 *
 * Device configurations should select HIGH-LEVEL INTENT:
 *
 *   - BASIC / COMPLETE / EVERYTHING
 *   - optional 2D matrix support
 *   - optional sound-reactive support
 *   - independent general effect packs
 *   - independent specialised effect packs
 *   - WebUI capability level
 *   - pixel-bus output allocation policy
 *
 * Low-level implementation features should then be derived here.
 *
 *
 * ============================================================================
 * DESIGN RULES
 * ============================================================================
 *
 * 1. DEVICE CONFIG SELECTS POLICY
 *
 *    Device INI/config files should increasingly contain only meaningful
 *    high-level selections.
 *
 *
 * 2. INTERNAL FEATURES ARE DERIVED
 *
 *    Presets, playlists, animator plumbing, filesystem integration, WebUI
 *    implementation details, audio-reactive implementation gates, etc. should
 *    not normally be selected individually by devices.
 *
 *
 * 3. EFFECT LEVELS ARE INDEPENDENT
 *
 *    LEVEL4 DOES NOT imply LEVEL1 + LEVEL2 + LEVEL3.
 *
 *    This is intentional.
 *
 *    Individual effect packs must be independently compilable so constrained
 *    devices and effect-development builds can include only the required code.
 *
 *
 * 4. 2D AND SOUND REACTIVE ARE ORTHOGONAL
 *
 *    2D:
 *      - matrix infrastructure
 *      - normal 2D effects
 *
 *    SOUND_REACTIVE:
 *      - audio-reactive infrastructure
 *      - 1D audio-reactive effects
 *
 *    2D + SOUND_REACTIVE:
 *      - matrix infrastructure
 *      - normal 2D effects
 *      - 1D audio-reactive effects
 *      - 2D audio-reactive effects
 *
 *    2D alone MUST NOT enable audio-reactive effects.
 *
 *
 * 5. AUDIO SOURCE IS INDEPENDENT
 *
 *    "Sound reactive" describes the data/effect processing capability.
 *
 *    The audio data may come from:
 *
 *      - simulated/test data
 *      - microphone
 *      - network source
 *      - another module
 *      - future input mechanism
 *
 *    A physical microphone is therefore NOT implied by SOUND_REACTIVE.
 *
 *
 * 6. BUS OUTPUT SELECTION IS A POLICY
 *
 *    Bus policy does NOT mean:
 *
 *      "use only RMT"
 *      "use only I2S"
 *
 *    It specifies the preferred allocation strategy.
 *
 *    The bus implementation may use several hardware methods depending on:
 *
 *      - ESP32 target
 *      - number of outputs
 *      - available RMT channels
 *      - available I2S/LCD hardware
 *      - parallel output suitability
 *      - LED protocol
 *
 *
 * 7. FILESYSTEM IS EXPECTED
 *
 *    Normal lighting firmware assumes filesystem support.
 *
 *    Filesystem-dependent implementation code should still retain compile
 *    guards as good defensive practice.
 *
 *    Explicit filesystem-less builds remain possible using DISABLE_FILESYSTEM.
 *
 *
 * 8. WEBUI HAS FOUR EXPLICIT LEVELS
 *
 *      NONE
 *      MINIMAL
 *      NORMAL
 *      ADVANCED
 *
 *    WebUI capability is deliberately independent from the effect set.
 *
 * ============================================================================
 */



/**
 * ============================================================================
 * SECTION 1
 * HIGH-LEVEL LIGHTING PROFILES
 * ============================================================================
 *
 * BASIC
 * -----
 *
 * Small, usable lighting firmware.
 *
 * Intended for:
 *
 *   - constrained devices
 *   - simple room lighting
 *   - devices where flash/RAM need to be controlled carefully
 *
 *
 * COMPLETE
 * --------
 *
 * Normal full PulSar lighting firmware.
 *
 * Provides the standard stable infrastructure:
 *
 *   - light interface
 *   - animator
 *   - effect engine
 *   - presets
 *   - playlists
 *   - filesystem
 *   - persistence
 *   - JSON support
 *
 * COMPLETE does NOT mean every effect pack is compiled.
 *
 *
 * EVERYTHING
 * ----------
 *
 * Large-memory general-purpose lighting build.
 *
 * Intended particularly for hardware such as ESP32-S3 N16R8 where flash/RAM
 * constraints are substantially relaxed.
 *
 * EVERYTHING explicitly enables:
 *
 *   - COMPLETE
 *   - all general effect packs
 *   - particle system
 *   - 2D
 *   - sound reactive
 *   - advanced WebUI
 *
 * Application-specific specialised effects remain separate.
 *
 *
 * BETA
 * ----
 *
 * Development overlay.
 *
 * BETA implies COMPLETE, but COMPLETE does NOT imply BETA.
 *
 * Only features genuinely undergoing validation should live under BETA.
 * ============================================================================
 */


// #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC

// #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

// #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__EVERYTHING

// #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA



/**
 * ============================================================================
 * SECTION 2
 * OPTIONAL HIGH-LEVEL CAPABILITIES
 * ============================================================================
 */


/**
 * Enable matrix infrastructure and all normal 2D effects.
 *
 * DOES NOT enable audio-reactive 2D effects by itself.
 */
// #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D


/**
 * Enable sound-reactive infrastructure and 1D audio effects.
 *
 * When FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D is also enabled, 2D audio effects
 * are additionally included.
 */
// #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE



/**
 * ============================================================================
 * SECTION 3
 * PROFILE RELATIONSHIPS
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__EVERYTHING
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
#endif


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
#endif


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC
#endif



/**
 * ============================================================================
 * SECTION 4
 * GENERAL EFFECT PACKS
 * ============================================================================
 *
 * IMPORTANT:
 *
 * THESE PACKS ARE INDEPENDENT.
 *
 * LEVEL4 DOES NOT ENABLE LEVEL1, LEVEL2 OR LEVEL3.
 *
 * This makes it possible to:
 *
 *   - test one effect pack in isolation
 *   - minimise flash use
 *   - construct purpose-specific firmware
 *
 * ============================================================================
 */


/**
 * Experimental/development effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL0_DEVELOPMENT // replaces ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING


/**
 * Minimal/static/general household effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME // replaces ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME


/**
 * Basic animated/flashing effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC // replaces ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


/**
 * Extended animated/seasonal effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED // replaces ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


/**
 * Additional large/general flashing effect collection.
 *
 * This is a pack name only.
 * It DOES NOT imply Levels 1-3.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE // replaces ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


/**
 * Particle-system effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM // replaces ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM



/**
 * ============================================================================
 * SECTION 5
 * SPECIALISED EFFECT PACKS
 * ============================================================================
 *
 * These remain independent from the normal general effect packs.
 *
 * EVERYTHING intentionally does NOT enable every specialised effect because
 * some require application-specific hardware, modules or configuration.
 * ============================================================================
 */


/**
 * Visual hardware / bus / pixel testing effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_HARDWARE_TESTING // replaces ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__HARDWARE_TESTING


/**
 * Effects whose state/data is driven directly from another PulSar module.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_EXTERNAL_MODULE_CONTROL // replaces ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CONTROLLED_FROM_ANOTHER_MODULE


/**
 * Solar-position based effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SOLAR_POSITION // replaces ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS


/**
 * LED / seven-segment clock effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SEGMENT_CLOCK // replaces ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK


/**
 * Christmas multifunction-controller effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_CHRISTMAS_CONTROLLER // replaces ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER


/**
 * Border / wallpaper effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_BORDER_WALLPAPERS // replaces ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__BORDER_WALLPAPERS


/**
 * Notification effects.
 */
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_NOTIFICATIONS // replaces ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__NOTIFICATIONS



/**
 * ============================================================================
 * SECTION 6
 * EVERYTHING PROFILE EFFECT SELECTION
 * ============================================================================
 *
 * General effect levels remain independent everywhere else.
 *
 * EVERYTHING explicitly requests each one because that is the purpose of the
 * EVERYTHING profile.
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__EVERYTHING

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

#endif



/**
 * ============================================================================
 * SECTION 7
 * BASIC PROFILE
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC

  /**
   * Core lighting engine.
   */
  #define ENABLE_FEATURE_LIGHTING__CORE__INTERFACE // replaces USE_MODULE_LIGHTS_INTERFACE

  #define ENABLE_FEATURE_LIGHTING__CORE__ANIMATOR // replaces USE_MODULE_LIGHTS_ANIMATOR

  #define ENABLE_FEATURE_LIGHTING__CORE__EFFECT_ENGINE // replaces ENABLE_FEATURE_LIGHTING__EFFECTS


  /**
   * Filesystem is considered part of normal lighting firmware.
   */
  #ifndef DISABLE_FILESYSTEM

    #define ENABLE_FEATURE_LIGHTING__CORE__FILESYSTEM
    // replaces USE_MODULE_CORE_FILESYSTEM

  #endif


  /**
   * BASIC receives the minimal general effect pack unless another build
   * deliberately changes this policy later.
   */
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME

#endif



/**
 * ============================================================================
 * SECTION 8
 * COMPLETE PROFILE
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

  /**
   * Core feature infrastructure.
   */

  #define ENABLE_FEATURE_LIGHTING__CORE__PRESETS // replaces ENABLE_FEATURE_LIGHTS__PRESETS

  #define ENABLE_FEATURE_LIGHTING__CORE__PLAYLISTS // replaces ENABLE_FEATURE_LIGHTS__PLAYLISTS

  #define ENABLE_FEATURE_LIGHTING__CORE__AUTOMATION_PRESETS // replaces ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS

  #define ENABLE_FEATURE_LIGHTING__CORE__ASYNC_JSON // replaces ENABLE_FEATURE_JSON__ASYNCJSON_V6

  #define ENABLE_FEATURE_LIGHTING__CORE__PERSISTENT_CONFIG // replaces ENABLE_FEATURE_FIRMWAREDEFAULT__LOAD_WITH_TEMPLATES_OVERRIDE

  #define ENABLE_FEATURE_LIGHTING__CORE__PIXEL_DECIMATION // replaces ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS

  #define ENABLE_FEATURE_LIGHTING__CORE__GAMMA_CORRECTION // replaces ENABLE_FEATURE_LIGHTS__GAMMA_CORRECTION


  /**
   * Preset / playlist support.
   */

  #define ENABLE_FEATURE_LIGHTING__PRESETS__FILE_METADATA // replaces ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA

  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__PRIMARY_JSON_COMMANDS // replaces ENABLE_FEATURE_LIGHTS__PLAYLISTS_INCLUDE_PRIMARY_JSON_COMMANDS


  /**
   * Filesystem behaviour.
   */

  #ifndef DISABLE_FILESYSTEM

    #define ENABLE_FEATURE_LIGHTING__FILESYSTEM__TIMESTAMP_SAVED_FILES
    // replaces ENABLE_FEATURE_FILESYSTEM__ADD_TIMESTAMP_ON_SAVE_FILES

    #define ENABLE_FEATURE_LIGHTING__FILESYSTEM__LOAD_MODULE_CONFIG_ON_BOOT
    // replaces ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT

  #endif


  /**
   * Colour processing.
   */

  #define ENABLE_FEATURE_LIGHTING__GAMMA__SKIP_PULSAR_NATIVE_PALETTES // replaces ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION_ON_PULSAR_PALETTES


  /**
   * Animator access.
   */

  #define ENABLE_FEATURE_LIGHTING__ANIMATOR__GLOBAL_LIGHT_ACCESS // replaces ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS


  /**
   * Existing stable optimisation.
   */

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__CHRISTMAS_PRECOMPUTE_POWF // replaces ENABLE_FEATURE_LIGHTING__CHRISTMAS_EFFECT_PRECOMPUTE_POWF_INTO_COLOURDATA_BUFFER


  /**
   * Existing brightness implementation requirement.
   *
   * This should eventually be reviewed/removed as the NeoPixelBus luminance
   * handling is cleaned up.
   */

  #define ENABLE_FEATURE_LIGHTING__ANIMATOR__REQUIRE_BRIGHTNESS_PHASE true // replaces PHASEIN_ANIM_BRIGHTNESS_REQUIRED_AS_TRUE

#endif



/**
 * ============================================================================
 * SECTION 9
 * 2D MATRIX SUPPORT
 * ============================================================================
 *
 * 2D enables:
 *
 *   - matrix addressing/infrastructure
 *   - normal 2D effects
 *
 * 2D DOES NOT enable audio-reactive effects.
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D

  #define ENABLE_FEATURE_LIGHTING__MATRIX__CORE // replaces ENABLE_FEATURE_LIGHTING__2D_MATRIX

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__2D // replaces ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

#endif



/**
 * ============================================================================
 * SECTION 10
 * SOUND REACTIVE
 * ============================================================================
 *
 * SOUND_REACTIVE enables:
 *
 *   - audio-reactive processing infrastructure
 *   - 1D audio-reactive effects
 *
 * If 2D is ALSO enabled:
 *
 *   - 2D audio-reactive effects are enabled
 *
 *
 * The source of audio data is not defined here.
 *
 * It may be simulated, microphone-derived, externally supplied, etc.
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

  #define ENABLE_FEATURE_LIGHTING__AUDIO__CORE

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D // replaces ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D


  /**
   * Temporary implementation gate.
   *
   * This remains because the audio-reactive implementation is still wrapped
   * as a development/usermod feature internally.
   *
   * Once audio reactive becomes a normal part of the lighting subsystem this
   * extra gate should disappear entirely.
   */

  #define ENABLE_FEATURE_LIGHTING__AUDIO__USERMOD_IMPLEMENTATION // replaces ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD


  #ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D

    #define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_2D
    // replaces ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D

  #endif

#endif



/**
 * ============================================================================
 * SECTION 11
 * WEB UI POLICY
 * ============================================================================
 *
 * Exactly ONE WebUI level should be active.
 *
 *
 * NONE
 * ----
 *
 * No lighting WebUI.
 *
 * Control remains available through MQTT / JSON / other non-WebUI mechanisms.
 *
 *
 * MINIMAL
 * -------
 *
 * Main lighting control page only.
 *
 *
 * NORMAL
 * ------
 *
 * Main lighting control page
 * + normal lighting settings pages.
 *
 *
 * ADVANCED
 * --------
 *
 * Everything in NORMAL
 * + newer/advanced lighting subpages
 * + playlist viewer
 * + richer tooling/diagnostics.
 *
 *
 * Defaults:
 *
 *   BASIC      -> MINIMAL
 *   COMPLETE   -> NORMAL
 *   EVERYTHING -> ADVANCED
 *
 * A device may explicitly override the default.
 * ============================================================================
 */


// #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__NONE

// #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__MINIMAL

// #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL

// #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED



#if (defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NONE) + \
     defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__MINIMAL) + \
     defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL) + \
     defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED)) > 1

  #error "Only one FIRMWARE_DEFAULT__LIGHTING_WEBUI__xxx may be selected"

#endif



#if !defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NONE) && \
    !defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__MINIMAL) && \
    !defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL) && \
    !defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED)

  #ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__EVERYTHING

    #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED

  #elif defined(FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE)

    #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL

  #elif defined(FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC)

    #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__MINIMAL

  #endif

#endif



/**
 * ============================================================================
 * SECTION 12
 * WEB UI FEATURE EXPANSION
 * ============================================================================
 */


#if defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__MINIMAL) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED)

  #define ENABLE_FEATURE_LIGHTING__WEBUI__CORE // replaces ENABLE_FEATURE_LIGHTING__WEBUI

  #define USE_MODULE_NETWORK_WEBSERVER

#endif



#if defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED)

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS

  #define ENABLE_FEATURE_LIGHTING__WEBUI__XML_API // replaces ENABLE_FEATURE_LIGHTING__XML_REQUESTS

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_LEDS // replaces ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_LEDS

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_SYNC // replaces ENABLE_FEATURE_LIGHTING__XML_REQUESTS__SUBPAGE_SYNC

  #define ENABLE_FEATURE_LIGHTING__WEBUI__URL_QUERY_SETTINGS // replaces ENABLE_FEATURE_LIGHTING__SETTINGS_URL_QUERY_PARAMETERS

#endif



#ifdef FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED

  #define ENABLE_FEATURE_LIGHTING__WEBUI__ADVANCED_PAGES // replaces ENABLE_FEATURE_WEBSERVER__ADVANCED_WEBPAGES

  #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_POLLING // replaces ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING

  #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET // replaces ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET

  #define ENABLE_FEATURE_LIGHTING__WEBUI__CAPTIVE_PORTAL // replaces ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SHARED_STYLES // replaces ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED

#endif



#if !defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NONE)

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SHOW_BUILD_DATETIME // replaces ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER

#endif



/**
 * ============================================================================
 * SECTION 13
 * PIXEL BUS OUTPUT POLICY
 * ============================================================================
 *
 * These are allocation PRIORITIES.
 *
 * They do NOT mean that only one output peripheral is permitted.
 *
 *
 * AUTO
 * ----
 *
 * Architecture-aware/default policy.
 *
 * The bus wrapper decides the preferred combination based on:
 *
 *   - ESP target
 *   - output count
 *   - available RMT channels
 *   - I2S/LCD capability
 *   - parallel-output suitability
 *
 *
 * I2S_THEN_RMT
 * ------------
 *
 * Prefer individual I2S-capable outputs first, then use RMT for additional
 * outputs where appropriate.
 *
 *
 * RMT_THEN_I2S
 * ------------
 *
 * Prefer RMT first, then use I2S-capable outputs when required.
 *
 *
 * PARALLEL_8_PREFERRED
 * --------------------
 *
 * Prefer an 8-channel parallel implementation where output topology and target
 * support make that appropriate.
 *
 * Other methods may still be used when required.
 *
 *
 * PARALLEL_16_PREFERRED
 * ---------------------
 *
 * Prefer a 16-channel parallel implementation where supported and appropriate.
 *
 * Other methods may still be used when required.
 *
 *
 * Exactly one policy should normally be active.
 * ============================================================================
 */


// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__AUTO // replaces ENABLE_FEATURE_LIGHTING__I2S_SINGLE_AND_PARALLEL_AUTO_DETECT


// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__I2S_THEN_RMT // replaces ENABLE_PIXELBUS_BUSMETHODS__I2S_SINGLE_CHANNELS_THEN_8_RMT_CHANNELS


// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__RMT_THEN_I2S // replaces ENABLE_PIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS


// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__PARALLEL_8_PREFERRED // replaces ENABLE_PIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE


// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__PARALLEL_16_PREFERRED // replaces ENABLE_PIXELBUS_BUSMETHODS__I2S0_PARALLEL_16_CHANNELS_MODE


// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__RMT_PRIMARY // replaces ENABLE_DEVFEATURE_NEOBUS__RMT_AS_PRIMARY



/**
 * Default policy.
 */


#if !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__AUTO) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__I2S_THEN_RMT) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__RMT_THEN_I2S) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__PARALLEL_8_PREFERRED) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__PARALLEL_16_PREFERRED) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__RMT_PRIMARY)

  #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__AUTO

#endif



#if (defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__AUTO) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__I2S_THEN_RMT) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__RMT_THEN_I2S) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__PARALLEL_8_PREFERRED) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__PARALLEL_16_PREFERRED) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__RMT_PRIMARY)) > 1

  #error "Only one ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_POLICY__xxx may be selected"

#endif



/**
 * ============================================================================
 * SECTION 14
 * FILESYSTEM POLICY
 * ============================================================================
 *
 * Filesystem support is expected for normal lighting firmware.
 *
 * The source should still retain defensive compile-time guards around
 * filesystem-specific implementation.
 *
 * DISABLE_FILESYSTEM remains the explicit exception.
 * ============================================================================
 */


#if defined(FIRMWARE_DEFAULT__LIGHTING_CONFIG__BASIC) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_CONFIG__EVERYTHING)

  #ifndef DISABLE_FILESYSTEM

    #define ENABLE_FEATURE_LIGHTING__CORE__FILESYSTEM

  #endif

#endif



/**
 * ============================================================================
 * SECTION 15
 * BETA / DEVELOPMENT OVERLAY
 * ============================================================================
 *
 * BETA is explicitly an overlay.
 *
 * COMPLETE does NOT automatically compile these features.
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA

  /**
   * Development task hooks.
   */

  #define ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS__ANIMATOR

  #define ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS


  /**
   * Playlist development.
   */

  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__LOAD_BY_NAME // replaces ENABLE_DEVFEATURE_LIGHTS__PLAYLIST_NAME_BASED_LOADING

  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__AUTOGENERATE_ID_LIST // replaces ENABLE_DEVFEATURE_LIGHTS__PLAYLIST_BY_NAME_AUTOGENERATE_ID_LIST


  /**
   * Preset development.
   */

  #define ENABLE_FEATURE_LIGHTING__PRESETS__DEBUG // replaces ENABLE_FEATURE_LIGHTS__PRESETS_DEBUG

  #define ENABLE_FEATURE_LIGHTING__PRESETS__LOAD_FROM_FILE // replaces ENABLE_DEVFEATURE_LIGHTING__PRESET_LOAD_FROM_FILE


  /**
   * Settings/persistence development.
   */

  #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_AFTER_SUCCESSFUL_BOOT_TEST // replaces ENABLE_DEVFEATURE_SETTINGS__SAVE_SETTINGS_ON_SUCCESFUL_BOOT__THEN_SPLASH_ON_REBOOT_PRIOR_TO_DEFAULT_LOAD_FOR_SAVE_TESTING

  #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_MODULE_DATA // replaces ENABLE_DEVFEATURE__SAVE_MODULE_DATA

  #define ENABLE_FEATURE_LIGHTING__CORE__DATABUFFER_LOCK // replaces ENABLE_FEATURE__DATABUFFER_LOCK


  /**
   * Temporary RGBWW / white-output workaround.
   */

  #define ENABLE_FEATURE_LIGHTING__OUTPUT__SUPPRESS_WHITE // replaces ENABLE_DEVFEATURE_LIGHTING__SUPPRESS_WHITE_OUTPUT


  /**
   * Async library debugging.
   */

  #define ENABLE_DEBUG_ASYNC // replaces DEBUG_ASYNC

#endif



/**
 * ============================================================================
 * SECTION 16
 * OPTIONAL SOLAR LIGHTING SUPPORT
 * ============================================================================
 *
 * Long-term intent:
 *
 * Lighting should use a lightweight solar representation rather than depending
 * on the full complex sun-tracking/rules implementation.
 *
 * For example:
 *
 *   - calculate/update solar state periodically
 *   - expose simple current solar position/state to lighting
 *   - avoid performing expensive full solar calculations inside effects
 *
 * ============================================================================
 */


#ifdef FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES

  // Future:
  //
  // #define ENABLE_FEATURE_LIGHTING__SOLAR__PALETTES
  //
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SOLAR_POSITION

#endif



/**
 * ============================================================================
 * SECTION 17
 * SINGLE-BUTTON DEMO / HARDWARE TEST MODE
 * ============================================================================
 *
 * Optional application/test feature.
 *
 * This remains independent from BASIC / COMPLETE / EVERYTHING.
 * ============================================================================
 */


#ifdef ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_DEMO_MODE // replaces ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_AS_DEMO_MODE

  #define USE_MODULE_SENSORS_INTERFACE

  #define USE_MODULE_SENSORS_BUTTONS


  /**
   * Intended controls may include:
   *
   * Multipress:
   *
   *   1. effect demo
   *   2. colour/pixel test
   *   3. bus identification test
   *
   * Hold:
   *
   *   power / brightness control
   */

  // #define ENABLE_FEATURE_LIGHTING__INPUT__BUTTON_CONTROLS // replaces ENABLE_FEATURE_LIGHTS__KEY_INPUT_CONTROLS

  // #define ENABLE_FEATURE_LIGHTING__DEMO_MODE // replaces ENABLE_FEATURE_LIGHTS__DEMO_MODE

#endif



/**
 * ============================================================================
 * SECTION 18
 * INTERNAL FEATURE CONSISTENCY
 * ============================================================================
 *
 * These relationships are consequences of the selected features rather than
 * user-facing configuration choices.
 * ============================================================================
 */


/**
 * Any effect pack requires the effect engine.
 */


#if defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL0_DEVELOPMENT) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_HARDWARE_TESTING) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_EXTERNAL_MODULE_CONTROL) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SOLAR_POSITION) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SEGMENT_CLOCK) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_CHRISTMAS_CONTROLLER) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_BORDER_WALLPAPERS) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_NOTIFICATIONS) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__2D) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_2D)

  #define ENABLE_FEATURE_LIGHTING__CORE__EFFECT_ENGINE

#endif



/**
 * Presets require persistence/filesystem in normal builds.
 */


#ifdef ENABLE_FEATURE_LIGHTING__CORE__PRESETS

  #ifndef DISABLE_FILESYSTEM
    #define ENABLE_FEATURE_LIGHTING__CORE__FILESYSTEM
  #endif

#endif



/**
 * Playlists require presets.
 */


#ifdef ENABLE_FEATURE_LIGHTING__CORE__PLAYLISTS

  #define ENABLE_FEATURE_LIGHTING__CORE__PRESETS

#endif



/**
 * 2D audio effects can only exist when both parent capabilities exist.
 */


#ifdef ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_2D

  #ifndef FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D
    #error "2D audio effects require FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D"
  #endif

  #ifndef FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE
    #error "2D audio effects require FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE"
  #endif

#endif



/**
 * ============================================================================
 * SECTION 19
 * NOTES FOR SOURCE-TREE MIGRATION
 * ============================================================================
 *
 * The comments marked:
 *
 *      replaces OLD_DEFINE
 *
 * are intentional migration markers.
 *
 * Migration process:
 *
 *   1. adopt this file
 *   2. compile one device/profile at a time
 *   3. find OLD_DEFINE through the source tree
 *   4. replace with the new define
 *   5. remove the corresponding "replaces" comment
 *   6. continue until no old lighting feature names remain
 *
 * There is deliberately NO legacy compatibility alias layer in this file.
 *
 * This avoids creating another temporary configuration architecture that would
 * itself later need to be removed.
 *
 * ============================================================================
 */


#endif // _FIRMWARE_DEFAULTS__LIGHTING2_H_