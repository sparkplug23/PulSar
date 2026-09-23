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
 * Effect packs are independent and may be combined freely.
 *
 * LEVEL4 DOES NOT ENABLE LEVEL1, LEVEL2 OR LEVEL3.
 *
 * If no general effect pack is explicitly selected, the default is:
 *
 *   LEVEL1_MINIMAL_HOME
 *   LEVEL2_FLASHING_BASIC
 *
 * ============================================================================
 */

// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL0_DEVELOPMENT
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM


// Primary desired amount included
#ifdef ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_DEFAULT
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL0_DEVELOPMENT
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM

#endif


// Default if none are defined
#if !defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL0_DEVELOPMENT) && \
    !defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME) && \
    !defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC) && \
    !defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED) && \
    !defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE) && \
    !defined(ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM)

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC

#endif

// If particle is added, currently this requires audiodata
#ifdef ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM
#define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D
#define ENABLE_FEATURE_LIGHTING__AUDIO__USERMOD_IMPLEMENTATION
#endif

/****
 * A clear way to include the maximum, particularly for esp32s3
 */
#ifdef FIRMWARE_DEFAULT__LIGHTING_EFFECTS__ALL
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL0_DEVELOPMENT
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL1_MINIMAL_HOME
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL2_FLASHING_BASIC
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL5_PARTICLE_SYSTEM
#endif


/**
 * ============================================================================
 * SECTION 5
 * SPECIALISED EFFECT PACKS
 * ============================================================================
 *
 * Specialised effect packs are independent from the general effect packs and
 * may be enabled individually as required.
 *
 * There is no specialised-effect fallback/default.
 *
 * ============================================================================
 */

// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_HARDWARE_TESTING
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_EXTERNAL_MODULE_CONTROL
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SOLAR_POSITION
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_SEGMENT_CLOCK
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_CHRISTMAS_CONTROLLER
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_BORDER_WALLPAPERS
// #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_NOTIFICATIONS


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
  #define USE_MODULE_LIGHTS_INTERFACE

  #define USE_MODULE_LIGHTS_ANIMATOR

  #define ENABLE_FEATURE_LIGHTING__CORE__EFFECT_ENGINE


  /**
   * Filesystem is considered part of normal lighting firmware.
   */
  #ifndef DISABLE_FILESYSTEM

    #define USE_MODULE_CORE_FILESYSTEM

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
  #define ENABLE_FEATURE_LIGHTING__CORE__PRESETS
  #define ENABLE_FEATURE_LIGHTING__CORE__PLAYLISTS
  #define ENABLE_FEATURE_LIGHTING__CORE__AUTOMATION_PRESETS
  #define ENABLE_FEATURE_LIGHTING__CORE__PERSISTENT_CONFIG
  #define ENABLE_FEATURE_LIGHTING__CORE__PIXEL_DECIMATION
  #define ENABLE_FEATURE_LIGHTING__CORE__GAMMA_CORRECTION

  #define USE_MODULE_CORE__JSON_ARDUINO

  /**
   * Preset / playlist support.
   */
  #define ENABLE_FEATURE_LIGHTING__PRESETS__FILE_METADATA
  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__PRIMARY_JSON_COMMANDS

  /**
   * Colour processing.
   */

  #define ENABLE_FEATURE_LIGHTING__GAMMA__SKIP_PULSAR_NATIVE_PALETTES


  /**
   * Animator access.
   */

  #define ENABLE_FEATURE_LIGHTING__ANIMATOR__GLOBAL_LIGHT_ACCESS


  /**
   * Existing stable optimisation.
   */

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__CHRISTMAS_PRECOMPUTE_POWF


  /**
   * Existing brightness implementation requirement.
   *
   * This should eventually be reviewed/removed as the NeoPixelBus luminance
   * handling is cleaned up.
   */

  #define PHASEIN_ANIM_BRIGHTNESS_REQUIRED_AS_TRUE true

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

  #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE // force audio with 2d

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED // temprary force
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE // temporary force

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

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D


  /**
   * Temporary implementation gate.
   *
   * This remains because the audio-reactive implementation is still wrapped
   * as a development/usermod feature internally.
   *
   * Once audio reactive becomes a normal part of the lighting subsystem this
   * extra gate should disappear entirely.
   */
  #define ENABLE_FEATURE_LIGHTING__AUDIO__USERMOD_IMPLEMENTATION


  #ifdef FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D
    #define ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_2D
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

#if defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__MINIMAL) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED)

  #ifndef USE_MODULE_NETWORK_WEBSERVER
    #error "Lighting WebUI requires USE_MODULE_NETWORK_WEBSERVER"
  #endif

#endif


/**
 * ============================================================================
 * SECTION 12
 * WEB UI FEATURE EXPANSION
 * ============================================================================
 */


#if defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NORMAL) || \
    defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED)

  #define ENABLE_FEATURE_LIGHTING__WEBUI__CORE

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS
  #define ENABLE_FEATURE_LIGHTING__WEBUI__XML_API
  #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_LEDS
  #define ENABLE_FEATURE_LIGHTING__WEBUI__SETTINGS_SYNC
  #define ENABLE_FEATURE_LIGHTING__WEBUI__URL_QUERY_SETTINGS

#endif



#ifdef FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED

  #define ENABLE_FEATURE_LIGHTING__WEBUI__ADVANCED_PAGES
  #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_POLLING
  #define ENABLE_FEATURE_LIGHTING__WEBUI__CONSOLE_WEBSOCKET
  #define ENABLE_FEATURE_LIGHTING__WEBUI__CAPTIVE_PORTAL
  #define ENABLE_FEATURE_LIGHTING__WEBUI__SHARED_STYLES

#endif



#if !defined(FIRMWARE_DEFAULT__LIGHTING_WEBUI__NONE)

  #define ENABLE_FEATURE_LIGHTING__WEBUI__SHOW_BUILD_DATETIME

#endif


/**
 * ============================================================================
 * SECTION 13
 * PIXEL BUS OUTPUT METHODS
 * ============================================================================
 *
 * These define the output allocation method used by the lighting bus wrapper.
 *
 * Only one should normally be enabled.
 *
 * AUTO_PARALLEL
 * -------------
 * Starts with the normal individual-output arrangement, but automatically
 * decides whether parallel output should be used from the configured bus count
 * and output size.
 *
 * When parallel output is selected:
 *   <= 8 outputs  -> X8 parallel
 *   <= 16 outputs -> X16 parallel
 *
 *
 * I2S_THEN_RMT
 * ------------
 * Use the individual I2S outputs first, followed by RMT outputs.
 *
 *
 * RMT_THEN_I2S
 * ------------
 * Use the available RMT outputs first, followed by individual I2S outputs.
 *
 *
 * PARALLEL_X8
 * -----------
 * Explicit X8 parallel output method.
 *
 *
 * PARALLEL_X16
 * ------------
 * Explicit X16 parallel output method.
 * ============================================================================
 */

// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__I2S_THEN_RMT
// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__RMT_THEN_I2S
// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_AUTO
// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X8
// #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X16


#if (defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__I2S_THEN_RMT) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__RMT_THEN_I2S) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_AUTO) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X8) + \
     defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X16)) > 1

  #error "Only one ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__xxx may be selected"

#endif

#if !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__I2S_THEN_RMT) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__RMT_THEN_I2S) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_AUTO) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X8) && \
    !defined(ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__PARALLEL_FORCED_X16)

  // Fallback to default
  #define ENABLE_FEATURE_LIGHTING__BUS_OUTPUT_METHODS__I2S_THEN_RMT

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

  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__LOAD_BY_NAME
  #define ENABLE_FEATURE_LIGHTING__PLAYLISTS__AUTOGENERATE_ID_LIST


  /**
   * Preset development.
   */

  #define ENABLE_FEATURE_LIGHTING__PRESETS__DEBUG
  #define ENABLE_FEATURE_LIGHTING__PRESETS__LOAD_FROM_FILE


  /**
   * Settings/persistence development.
   */

  #define ENABLE_FEATURE_LIGHTING__SETTINGS__SAVE_MODULE_DATA

  /**
   * Async library debugging.
   */

  #define ENABLE_DEBUG_ASYNC

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


#ifdef ENABLE_FEATURE_LIGHTING__SINGLE_BUTTON_DEMO_MODE

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

  // #define ENABLE_FEATURE_LIGHTING__INPUT__BUTTON_CONTROLS

  // #define ENABLE_FEATURE_LIGHTING__DEMO_MODE

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
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_1D) || \
    defined(ENABLE_FEATURE_LIGHTING__EFFECTS__AUDIO_2D)

  #define ENABLE_FEATURE_LIGHTING__CORE__EFFECT_ENGINE

#endif



/**
 * Presets require persistence/filesystem in normal builds.
 */


#ifdef ENABLE_FEATURE_LIGHTING__CORE__PRESETS

  #if defined(DISABLE_FILESYSTEM) || !defined(USE_MODULE_CORE_FILESYSTEM)
    #error "DEFINE: Presets require filesystem" // lets not silently enable filesystem here
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


#endif // _FIRMWARE_DEFAULTS__LIGHTING2_H_