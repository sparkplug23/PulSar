#ifndef _AnimatorDALIGHTNEOPIXEL7_segH
#define _AnimatorDALIGHTNEOPIXEL7_segH 7.0

#include "1_TaskerManager/mTaskerManager.h"

// #ifdef USE_MODULE_LIGHTS_ANIMATOR


    #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

//     const char* GetFlasherFunctionName(char* buffer, uint8_t buflen);
//   const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen);
//   int8_t      GetFlasherFunctionIDbyName(const char* c);

  /**
   * Minor Code version 96 onwards requires names, as reshuffle is happening. Any old device will not respond to the correct command via number until remap is performed
   * */
  enum EFFECTS_FUNCTION_IDS{
    /**
     * Default
     * Desc: Same colour across all pixels.
     * Param: palette (will always use index 0 colour regardless of palette length)
     * */
    EFFECTS_FUNCTION_SOLID_COLOUR_ID,
    /**
     * Desc: Using EFFECTS_FUNCTION_SLOW_GLOW_ID, with pixel order set to "inorder"
     *       Each sequential pixel will repeat the palette if type is list, if gradient, then palette is linear blended across output
     * Parameters: Palette, time to blend, rate of new colours, percentage of new colours changed
     * */
    EFFECTS_FUNCTION_STATIC_PALETTE_ID,
    /**
     * Desc: Random change of colours (pixel order is random)
     *       Pixel elements randomly select colour from palette, then slowly blend those pixels to the new colour
     * Parameters: 
     *    Palette = Pick colours to chose from
     *    Transition time = blend time between last and new colours
     *    Transition rate = time between new colours are selected
     *    Percentage of change = Number of new colours to change this time (eg 10% of total count will randomly change)
     * */
    EFFECTS_FUNCTION_SLOW_GLOW_ID,
    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * */
    EFFECTS_FUNCTION_SEQUENTIAL_ID,
    /**
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * TODO: Add size of step as percentage ie to show 50% of 4 colours would be 2 of 4, 75% of 4 is 3
     * */
    EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID,
    /**
     * Desc: Twinkle on, fade off
     *      Random burst of pixels are turned on based on palette, then fade off
     * 
     * Method: randomly pick burst of pixels to turn on, ie animation period starts of Y, then decays (thereby increasing next run time)
     *         Do this until X pixels (animation periods) have occured,
     *         Every 1 second of animation that elapses, half the brightness of all pixels (ie the fade)
     *         Intensity1: will dictate how many pixels are turned on
     *         Intensity2: may dictate the decay rate (ie much slower would leave pixels on for longer meaning more of the lights remain on) 
     * NotActive
     * EFFECTS_FUNCTION_PULSE_RANDOM_ON_FADE_OFF_ID
     * */
    EFFECTS_FUNCTION_TWINKLE_RANDOM_BURSTS_ON_WITH_SLOW_FADE_OFF_ID,
    /**
     * Palette is first drawn with certain lower brightness level, then
     * additional changes takes those colours and bumbs their brightness up to desired brightness
     * Few change, random pixels bumped up
     * Check if their brightness is <upper, before asserting them brighter (or convert to hsbcolour, flick up brightness if needed then change back to rgb)
     * Goes from low to high then stops
     * */
    EFFECTS_FUNCTION_POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY,
    /**
     * Desc: Twinkle
     *  Random pixels will turn off for random times (ie apply inorder palette always, then random turn colours to off and on(brightness) again)
     * */

    /**
     * Desc: Twinkle with glimmer
     *  Twinkle as above, but the entire global brightness will flicker gently (so limit how much) based on pixels energy count ie twinkle filament lights
     * */

    /**
     * 0/100% animaiton progress is low brightness, 50% flips to upper brightness
     * ie randomly change pixels between two brightness levels, with optional boundaries only or as range
    */
    EFFECTS_FUNCTION_TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK,

    /**
     *  Instead of just applying new colours, also randomise the brightness levels between range
     * */
    EFFECTS_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS_ID,

    /**
     * Desc: randomly slow glow with triangular brightness between colours
     * Or, so at 50% transition, the new colour is at 100%, with 100% brightness, then fade brightness back down to lower_global brightness
     * So, pixel red (50%) changing to pixel blue, would go red to blue (bright 100%) then fade down brightness 50%
     * Optional, could also be non-equal triangle ie have value for peak of traingle, so 20% would mean 0.2 seconds flash to new colour then slowly decay (0.8 seconds) to new brightness
     * */
    // EFECT

    /**
     * Fire animations
     * rename from fireplace to another name, simply fire
     * */

     EFFECTS_FUNCTION_FIREPLACE_1D_01_ID, // solid colours, 1 100%, moving from previous to next

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Matric 3D Animations (with MxN pixel grid) ***************************************************************************************************************************************************************************
    **  Requires:     **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/



    // EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION_SLOW_FADE_SATURATION_RANDOM_ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID, //random leds flash to 100% brightness (modes=instant on/off, multiple pulses)
    // EFFECTS_FUNCTION_FLASH_TWINKLE_PALETTE_COLOUR_RANDOM_ID,   
   
    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Sun positions to control animation***************************************************************************************************************************************************************************
    **  Requires:    Internet to know time and location for sun calculations **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    //linear palette group colour, changing by triggered and over a period of time (eg an alarm -30 minutes)
    EFFECTS_FUNCTION_SUNPOSITIONS_SUNRISE_ALARM_01, //group01? bring all together, have settings to configure how the effect runs
    //linear palettes group colour, enabled by a time window, but sets linear gradient by sun azimuth
    EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_01, //this is elevation only
    //enabled by a time window, GENERATE manually, brightness by sun elevaltion, sun position from left to right by azimuth
    //far left = east from position, and far right = wast from position using sunset position
    EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01,
    EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_2D_01, // If matrix/grid is connected, enable drawing this

    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_01_ID,
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_02_ID,
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_03_ID, // Using stored rgbcct palette
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_04_ID, // Using stored rgbcct palette
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_05_ID, // CCT only, mapped directly, no palette
    
    /**
     * Eleveation controls that use rgbcct palette (that has elevation in its index byte) to display solid colour only
     * */
    EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID,
    EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID,

    /**
     * Elevation controls the CCT channels only
     **/
    EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID,


    // palette to step through, which gives single colour sun
    EFFECTS_FUNCTION_SUNPOSITIONS_STEP_RGBCCT_ALARM_01,
    //enabled CCT by azimuth 
    EFFECTS_FUNCTION_SUNPOSITIONS_STEP_RGBCCT_SUN_ELEVATION_01,

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: RGB Segment Clock  ***************************************************************************************************************************************************************************
    **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    EFFECTS_FUNCTION_LCD_CLOCK_BASIC_01_ID,
    EFFECTS_FUNCTION_LCD_CLOCK_BASIC_02_ID,
    EFFECTS_FUNCTION_LCD_DISPLAY_BASIC_01_ID, // show number commanded via mqtt


    // /******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // *** Specialised: RGB Segment Clock  ***************************************************************************************************************************************************************************
    // **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************/


    // // Apply sinewave to brightness, 25% increasing, 50% decreasing, 25% off
    // EFFECTS_FUNCTION_PULSE_SINE_WAVE_BRIGHTNESS, //pul
    // // The one when applybrightness was broke
    // // done by applying new cl/brightness to some, then each call reducing the colour crushing it
    // //do with percent to show on at one time
    // EFFECTS_FUNCTION_PULSE_RANDOM_ON, 
    // EFFECTS_FUNCTION_PULSE_RANDOM_ON_TWO_ID,
    
    // EFFECTS_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID, // sequential flash of white on x leds 
    // EFFECTS_FUNCTION_FADE_GRADIENT_ID, //single pixels: static, twinkle, pick from palette
    // //another flash to "off" or simple set flash colour to off??
    // EFFECTS_FUNCTION_FLASH_GLIMMER_RANDOM_ID, // tinkle=leds flash independant, glimmer=leds flash with dimming effect on others
    /**
     * Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     * */
    // EFFECTS_FUNCTION_SHOWING_MULTIPLES_OF_COLOURS_ID,
    /**
     * Sun positional effects
     */
    // New with solarlunar


    /**
     * Christmas Controller Effects: 6 (without static) that replicate christmas lights
     * Note: This will take several extra forms, as I will want fading of those lights with the animations, some lights are instant
     * Also, I might keep 5 lights static and apply the pattern by turned off bulbs like real lights 
     * 8 different control settings

      make special christmas ones that apply this over a group, ie single red, green, blue,pink light would instead be a sine wave of brightness (including into)

      basic xmas inwaves, but lights are drawn (in sine wave with size/length parameter), with an "off" section in between
      basic xmas inwaves, but lights are drawn (in sine wave with size/length parameter), BUT "off" section in between IS INSTEAD INVERTING WITH ON SECTION
      basic xmas inwaves, but lights are drawn (in sine wave with size/length parameter), but no off section, instead it blends evening between the two colours with varying blend weights (ie even blend, or only 10% of pixel length blend of each colour group)

    EFFECTS_FUNCTION_XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID


     * */

    /**
     * Palette is drawn "inorder" and animations are drawn with X outputs (4 or 5 like normal lights) to create the effect like real lights
     *
      1 - Combination (ommited, can be performed using mixer)
      2 - In Waves
      3 - Sequentials
      4 - Slo-Glo
      5 - Chasing / Flash
      6 - Slow Fade
      7 - Twinkle / Flash
      8 - Steady on (already added)
     * */
    // EFFECTS_FUNCTION_XMAS_IN_WAVES_ID
    // EFFECTS_FUNCTION_XMAS_SEQUENTIAL_ID
    // EFFECTS_FUNCTION_XMAS_SLO_GLO_ID
    // EFFECTS_FUNCTION_XMAS_CHASING_AND_FLASHING_ID
    // EFFECTS_FUNCTION_XMAS_SLOW_FADE_ID
    // EFFECTS_FUNCTION_XMAS_TWINKLE_AND_FLASH_ID

    // EFFECTS_FUNCTION_XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID

    // EFFECTS_FUNCTION_XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    // EFFECTS_FUNCTION_XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID


    /**
     * Development methods
     * */

    /**
     * Functional methods of sunrise
     * EFFECTS_FUNCTION_SUNPOSITIONS_<SOLIDCOLOUR/2D_ARRAY>
     * */
    //create a palette to leave on ALL the time, ie deep red at night instead of blue... testing for the clocks later I guess

    /**
     * Not currently working
     * */
    /**
     * Clock animations for 3d printed display
     **/

    /**
     * Create special musical christmas lights, that will only be available on esp32 with a speaker (larger RAM)
     * */

    /**
     * Designing and quick test of animations before creating its own animaiton profile
     * */
    EFFECTS_FUNCTION_TESTER_ID,
    // Length
    EFFECTS_FUNCTION_LENGTH_ID
  };         

  
  struct AnimationColours_SOLID_COLOUR
  {
    RgbcctColor StartingColor;
    RgbcctColor DesiredColour;
  };
  AnimationColours_SOLID_COLOUR animation_colours_rgbcct;


/*****************************************
 * To be absorbed by segment settings
 * START
 * *//////
  typedef union {
    uint16_t data;
    struct { 
      // enable animations (pause)
      uint16_t enable_random_rate : 1;
      // used when internal timers or modes change and both need to execute the one time
      uint16_t force_finish_flasher_pair_once : 1;
      /* direction of animation
      / 00(0) - clockwise
      / 01(1) - anticlockwise
      / 10(2) - edges towards center (flipped image)
      / 11(3) - center towards edges (flipped image) */
      uint16_t movement_direction : 2; // keep this is an option later (32bit option list)

      uint16_t enable_startcolour_as_alternate : 1;
      uint16_t enable_endingcolour_as_alternate : 1;
    
      // Reserved
      uint16_t reserved : 12;
    };
  } EFFECTSSETTINGS_FLAGS;
  

  // Segments runtime??
  struct EFFECTSSETTINGS{
    // Flashing function
    uint8_t function = EFFECTS_FUNCTION_STATIC_PALETTE_ID;
    // Section of animation
    uint8_t region = 1;//EFFECTS_REGION_COLOUR_SELECT_ID;
    // flag needed to force completion between regions
    uint8_t flag_finish_flasher_pair = false;

    EFFECTSSETTINGS_FLAGS flags;

    // struct RANDOM_TRANSITIONS{
    //   uint32_t rate_ms = 1000;
    //   uint8_t upper_secs = 20;
    //   uint8_t lower_secs = 1;
    //   uint8_t array_index = 0;
    //   uint8_t array_index_length = 0;
    //   //uint8_t array[10];   //use different array profiles, "linear ramp", "exponentialhigh", "exp low" (lingers on low)
    //   // uint8_t profile_id;
    //   uint32_t tSavedNewSpeedUpdate = millis();
    // }random_transitions;
    

    struct UPDATE_COLOUR_GENERATION_REGION{
      // 0 is not active, 
      // 1 is everytime
      // 2+ is seconds
      uint16_t refresh_secs = 0; 
      uint16_t refresh_decounter_secs = 10; 
      uint32_t tSaved = millis();
    }update_colour_region;
    
    uint8_t brightness_max = 255;
    uint8_t brightness_min = 0;
    // struct TSAVED{
    //   uint32_t Update = millis();
    //   // uint32_t tSavedChange_animation_something;
    //   //uint32_t Update2 = millis();
    // }tSaved;
    
  }flashersettings_segments;

/*****************************************
 * To be absorbed by segment settings
 * END
 * *//////


  void SubTask_Segments_Animation();
void Segments_RefreshLEDIndexPattern(uint8_t segment_index = 0);
/**
 * Create a duplicate of this in HACS, and slowly move HACS to follow this basic layout ("layout" = struct of what animation runs in that section)
 * */

/**
 * how a section (single animation) is configured -- my HACS needs to change to conform to this
 * */
  
// options
// bit    7: segment is in transition mode
// bits 4-6: TBD
// bit    3: mirror effect within segment
// bit    2: segment is on
// bit    1: reverse segment
// bit    0: segment is selected
#define NO_OPTIONS2   (uint8_t)0x00
#define TRANSITIONAL2 (uint8_t)0x80
#define MIRROR2       (uint8_t)0x08
#define SEGMENT_ON2   (uint8_t)0x04
#define REVERSE2      (uint8_t)0x02
#define SELECTED2     (uint8_t)0x01
// #define IS_TRANSITIONAL2 ((_segments[_segment_index].options & TRANSITIONAL2) == TRANSITIONAL2)
// #define IS_MIRROR2       ((_segments[_segment_index].options & MIRROR2      ) == MIRROR2      )
// #define IS_SEGMENT_ON2   ((_segments[_segment_index].options & SEGMENT_ON2  ) == SEGMENT_ON2  )
// #define IS_REVERSE2      ((_segments[_segment_index].options & REVERSE2     ) == REVERSE2     )
// #define IS_SELECTED2     ((_segments[_segment_index].options & SELECTED2    ) == SELECTED2    )

#define DEFAULT_BRIGHTNESS2 (uint8_t)127
#define DEFAULT_MODE2       (uint8_t)0
#define DEFAULT_SPEED2      (uint8_t)128
#define DEFAULT_COLOR2      (uint32_t)0xFFAA00

// #define NUM_COLORS       3 /* number of colors per segment */
#define SEGCOLOR2(x)      _segments[_segment_index].colors[x]

    // #define SEGCOLOR(x)      gamma32(_segments[_segment_index].colors[x])

// #define SEGLEN           _virtualSegmentLength
// #define SPEED_FORMULA_L  5 + (50*(255 - _segments[_segment_index].speed))/_virtualSegmentLength
// #define RESET_RUNTIME    memset(_segment_runtimes, 0, sizeof(_segment_runtimes))
/**
 * First animations to be ported to new method, they shall both be displayed on string together
 * Slow Glow (50%)
 * Solid (50%) with random colour changes
 * */

    void showWLED(void);

    void fill_ranged(uint32_t c);
    void seg_fill_ranged(uint32_t c);


    // builtin modes
    uint16_t
      seg_mode_static(void),
      mode_static(void),
      // mode_blink(void),
      // mode_blink_rainbow(void),
      // mode_strobe(void),
      // mode_strobe_rainbow(void),
      mode_color_wipe(void),
      mode_color_sweep(void),
      mode_color_wipe_random(void),
      mode_color_sweep_random(void),
      // mode_random_color(void),
      // mode_dynamic(void),
      // mode_breath(void),
      // mode_fade(void),
      // mode_scan(void),
      // mode_dual_scan(void),
      // mode_theater_chase(void),
      // mode_theater_chase_rainbow(void),
      // mode_rainbow(void),
      // mode_rainbow_cycle(void),
      // mode_running_lights(void),
      // mode_saw(void),
      // mode_twinkle(void),
      // mode_dissolve(void),
      // mode_dissolve_random(void),
      // mode_sparkle(void),
      // mode_flash_sparkle(void),
      // mode_hyper_sparkle(void),
      // mode_multi_strobe(void),
      // mode_android(void),
      // mode_chase_color(void),
      // mode_chase_random(void),
      // mode_chase_rainbow(void),
      // mode_chase_flash(void),
      // mode_chase_flash_random(void),
      // mode_chase_rainbow_white(void),
      // mode_colorful(void),
      // mode_traffic_light(void),
      // mode_running_color(void),
      // mode_running_red_blue(void),
      // mode_running_random(void),
      // mode_larson_scanner(void),
      // mode_comet(void),
      // mode_fireworks(void),
      // mode_rain(void),
      // mode_merry_christmas(void),
      // mode_halloween(void),
      // mode_fire_flicker(void),
      // mode_gradient(void),
      // mode_loading(void),
      // mode_police(void),
      // mode_police_all(void),
      // mode_two_dots(void),
      // mode_two_areas(void),
      // mode_circus_combustus(void),
      // mode_bicolor_chase(void),
      // mode_tricolor_chase(void),
      // mode_tricolor_wipe(void),
      // mode_tricolor_fade(void),
      // mode_lightning(void),
      // mode_icu(void),
      // mode_multi_comet(void),
      // mode_dual_larson_scanner(void),
      // mode_random_chase(void),
      // mode_oscillate(void),
      // mode_fire_2012(void),
      // mode_pride_2015(void),
      // mode_bpm(void),
      // mode_juggle(void),
      // mode_palette(void),
      // mode_colorwaves(void),
      // mode_fillnoise8(void),
      // mode_noise16_1(void),
      // mode_noise16_2(void),
      // mode_noise16_3(void),
      // mode_noise16_4(void),
      // mode_colortwinkle(void),
      // mode_lake(void),
      // mode_meteor(void),
      // mode_meteor_smooth(void),
      // mode_railway(void),
      // mode_ripple(void),
      // mode_twinklefox(void),
      // mode_twinklecat(void),
      // mode_halloween_eyes(void),
      mode_static_pattern(void),
      mode_tri_static_pattern(void);//,
      // mode_spots(void),
      // mode_spots_fade(void),
      // mode_glitter(void),
      // mode_candle(void),
      // mode_starburst(void),
      // mode_exploding_fireworks(void),
      // mode_bouncing_balls(void),
      // mode_sinelon(void),
      // mode_sinelon_dual(void),
      // mode_sinelon_rainbow(void),
      // mode_popcorn(void),
      // mode_drip(void),
      // mode_plasma(void),
      // mode_percent(void),
      // mode_ripple_rainbow(void),
      // mode_heartbeat(void),
      // mode_pacifica(void),
      // mode_candle_multi(void),
      // mode_solid_glitter(void),
      // mode_sunrise(void),
      // mode_phased(void),
      // mode_twinkleup(void),
      // mode_noisepal(void),
      // mode_sinewave(void),
      // mode_phased_noise(void),
      // mode_flow(void),
      // mode_chunchun(void);

    // mode helper functions
    uint16_t
    //   blink(uint32_t, uint32_t, bool strobe, bool),
    //   candle(bool),
      color_wipe(bool, bool);//
    //   scan(bool),
    //   theater_chase(uint32_t, uint32_t, bool),
    //   running_base(bool),
    //   larson_scanner(bool),
    //   sinelon_base(bool,bool),
    //   dissolve(uint32_t),
    //   chase(uint32_t, uint32_t, uint32_t, bool),
    //   gradient_base(bool),
    //   ripple_base(bool),
    //   police_base(uint32_t, uint32_t, bool),
    //   running(uint32_t, uint32_t),
    //   tricolor_chase(uint32_t, uint32_t),
    //   twinklefox_base(bool),
    //   spots_base(uint16_t),
    //   realPixelIndex(uint16_t i),
    //   phased_base(uint8_t);


    #define NUM_COLORS2       3 /* number of colors per segment */

    uint32_t _lastPaletteChange = 0;
    uint32_t _lastShow = 0;

    
    uint32_t timebase;
    uint32_t now;
    bool
      _useRgbw = false,
      _skipFirstMode,
      _triggered;
    uint8_t
      mainSegment = 0,
      rgbwMode = 3,
      paletteFade = 0,
      paletteBlend = 0;

    uint16_t _length, _lengthRaw, _virtualSegmentLength;


    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        // enable animations (pause)
        uint16_t fEnable_Animation : 1;
        uint16_t ColourComponentsUpdatedButNotYetApplied : 1; // new colours, not yet applied

        uint16_t fForceUpdate : 1; 
        uint16_t fRunning : 1;//= false;
        uint16_t fEndUpdatesWhenAnimationCompletes : 1;// = false;

        uint16_t NewAnimationRequiringCompleteRefresh : 1; 
        
        uint16_t ftime_use_map : 1;//= true;
        uint16_t frate_use_map : 1;//= true;

        uint16_t brightness_applied_during_colour_generation : 1;
        // This emulates aging of traditional lights, making them less uniform
        uint16_t apply_small_saturation_randomness_on_palette_colours_to_make_them_unique : 1;

        uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Exactly_To_Palette_Choice : 1;
        uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice : 1;

        uint16_t Limit_Upper_Brightness_With_BrightnessRGB : 1;

        uint16_t use_gamma_for_brightness : 1;

        uint16_t animator_first_run : 1;


        // Reserved
        uint16_t reserved : 3;
      };
    } ANIMATION_FLAGS;


    /**
     * previous animation settings will be moved into here
     * */
    typedef struct Segment { // 24 bytes
        struct PIXEL_INDEX_RANGE{
            uint16_t start = 0;
            uint16_t stop  = 0; // if equal, range is not active
        }pixel_range;
        
    //   uint8_t palette = 0;
    //   uint8_t mode = DEFAULT_MODE2; //animation_id
      uint8_t options = NO_OPTIONS2; //bit pattern: msb first: transitional needspixelstate tbd tbd (paused) on reverse selected
      uint8_t grouping = 1; //multiplers
      uint8_t spacing = 0; //??

      uint32_t tSaved_AnimateRunTime = millis();
    uint8_t effect_id = EFFECTS_FUNCTION_STATIC_PALETTE_ID;

      uint16_t pixels_to_update_this_cycle = 0;

    //   uint8_t opacity = 255; //??
    //   uint32_t colors[NUM_COLORS2] = {DEFAULT_COLOR2}; //? really not needed or understood
      /**
       * HACS animation settings
       * */
      /**
       * Flags
       * */
      ANIMATION_FLAGS flags;
      /**
       * Effects (Scenes & Flasher), Ambilight, Adalight
       * */
      uint8_t mode_id = 0;
      /**
       * Palette (Options on getting from palette)
       * */
      struct PALETTE{
        uint8_t id = 0; 
        uint8_t pattern = 0; //Single, Gradient 
        /**
         * Index range, ie with 50%, for palette with 10 colours will only use first 5 colours
         * Value '0': is unused
         * */
        uint8_t index_range_max_limit = 0; 
        uint8_t index_range_as_percentage = 0;
      }palette;
      /**
       * Transition settings
       * */
      struct TRANSITION_SETTINGS{
        /**
         * Pixel order (eg Random, InOrder)
         * */
        uint8_t order_id = 0;
        /**
         * Change method (eg Blend, Instant)
         * */
        uint8_t method_id = 2;
        /**
         * Pixels to change up to maximum of segment
         * */
        uint16_t pixels_to_update_as_number  = 1;
        /**
         * Refresh rate, calculate new colours
         * */
        uint16_t rate_ms = 2000;
        /**
         * Time to new colours
         * */
        uint16_t time_ms = 0;  
        /**
         * legacy value from wled, when updated via function, it will select the rate/time for animation
         * */
        uint8_t speed = DEFAULT_SPEED2; // rate/blend time
      }transition;
      uint8_t speed() //legacy for wled effects, made up from time/rate
      {
        return 128;
      }
      uint8_t intensity() //legacy for wled effects, made up from time/rate
      {
        return 128; // amount of animations in it
      }
      // void setOption(uint8_t n, bool val)
      // {
      //   if (val) {
      //     options |= 0x01 << n;
      //   } else
      //   {
      //     options &= ~(0x01 << n);
      //   }
      // }
      // bool getOption(uint8_t n)
      // {
      //   return ((options >> n) & 0x01);
      // }
      // bool isSelected()
      // {
      //   return getOption(0);
      // }
      bool isActive()
      {
        return pixel_range.stop > pixel_range.start;
      }
      uint16_t length()
      {
        return pixel_range.stop - pixel_range.start;
      }
      uint16_t groupLength()
      {
        return grouping + spacing;
      }
      uint16_t virtualLength()
      {
        uint16_t groupLen = groupLength();
        uint16_t vLength = (length() + groupLen - 1) / groupLen;
        if (options & MIRROR2)
          vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED
        return vLength;
      }
      
    } segment_settings;

    // Change this to define them above in header
    segment_settings _segments[MAX_NUM_SEGMENTS];
    //  = { // SRAM footprint: 24 bytes per element
    //   // start, stop, speed, intensity, palette, mode, options, grouping, spacing, opacity (unused), color[]
    //   { 0, 7, DEFAULT_SPEED2, 128, 0, DEFAULT_MODE2, NO_OPTIONS2, 1, 0, 255, {DEFAULT_COLOR2}}
    // };


    /**
     * values that are used as global iters rather than passing between each function
     * */
    struct segment_shared_iters{
      // uint8_t index = 0;
      uint8_t _segment_index = 0;
      uint8_t _segment_index_palette_last = 99; //mine is longer

    }segment_iters;
      
    /**
     * Runtime data for that segment, ie holds animation data
     * */
    typedef struct Segment_runtime { // 28 bytes

      /**
       * Next few bytes are a simple version that dont require structs
       * */
      unsigned long next_time;
      uint32_t step;
      uint32_t call;

    /**
     * replacing "flashersettings_segments.region"
     * */
      uint16_t aux0 = 0; //flashersettings_segments.region
      uint16_t aux1 = 0;
      uint16_t aux2 = 0;
      uint16_t aux3 = 0;

      /**
       * This likely wont work, as it needs to be moved into its own class so it can have mulitple instances
       * This should work, as its pointers and creating on the heap
       * */
      byte* data = nullptr; // note: how all my previous hardcorded structs will need to conform to this
      bool allocateData(uint16_t len){
        if (data && _dataLen == len) return true; //already allocated
        deallocateData();
        if (_usedSegmentData + len > MAX_SEGMENT_DATA)
        {
          Serial.println("return false; //not enough memory");
          return false; //not enough memory
        }
        data = new (std::nothrow) byte[len];
        if (!data) return false; //allocation failed
        _usedSegmentData += len;
        _dataLen = len;
        memset(data, 0, len);
        return true;
      }
      void deallocateData(){
        delete[] data;
        data = nullptr;
        _usedSegmentData -= _dataLen;
        _dataLen = 0;
      }
      void reset(){next_time = 0; step = 0; call = 0; aux0 = 0; aux1 = 0; deallocateData();}

      /**
       * Each segment will have its own animator
       * This will also need to share its index into the animation so it knows what segments to run
       * */
      NeoPixelAnimator* animator = nullptr; //one animator for each segment, which is only init when needed or else delete
      
      /**
       * Using "index" inside animator as segment index
       * */
      ANIM_FUNCTION_SIGNATURE;

      // private:
        uint16_t _dataLen = 0;
    }segment_runtime;
    segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 28 bytes per element

    

    struct SHARED_EFFECTS_PARAMETERS
    {
        // uint8_t alternate_brightness;
        uint8_t alternate_brightness_min = 0; //set min and max to same to not use a range
        uint8_t alternate_brightness_max = 255;
        //flag use single bringhtess or range
        uint8_t alternate_random_amount_as_percentage = 2;
        /*
        * Holds the last percentage animation was run on, to be checked in animupdates to make sure not to double change values
        * ie anim.progress as float 0 to 1, in range 0 to 100 will repeat each integer value many times
        * */
        uint8_t progress_percentage_last_animated_on = 100;

        // struct INDEX_COUNTERS{
        //     uint16_t lower_limit = 0;
        //     uint16_t upper_limit = 0; 
        //     uint16_t active = 0;
        //     uint8_t counter = 0;
        // }indexes;

    }shared_flasher_parameters_segments;

    enum EFFECTSREGION
    {
        EFFECTS_REGION_COLOUR_SELECT_ID=0,
        EFFECTS_REGION_ANIMATE_ID
    };      


    mAnimatorLight& setAnimFunctionCallback_Segments_Indexed(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE);
    void StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index = 0);




    void Segments_UpdateDesiredColourFromPaletteSelected(uint16_t segment_index = 0);
    void AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(const AnimationParam& param);

    /**
     * My animations (and their animators where applicable)
     * */
    void SubTask_Segment_Animate_Function__Solid_Static_Single_Colour();
    void AnimationProcess_Generic_RGBCCT_LinearBlend_Segments(const AnimationParam& param);    
    void SubTask_Segment_Animate_Function__Static_Palette();
    void SubTask_Segment_Animate_Function__Slow_Glow();
    void SubTask_Segment_Animate_Function__Step_Through_Palette();
    void SubTask_Segment_Flasher_Animate_Function__Sequential();
    void Segments_RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction);



    void Segment_SubTask_Flasher_Animate_Function__TEST_SolidRandom();


    void Segments_SetLEDOutAmountByPercentage(uint8_t percentage, uint8_t segment_index = 0);
      
    void resetSegments();
    void handle_palette(void);
    
    #ifndef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;
    #endif// ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

    uint32_t tSaved_Test_Segment_Animation = 0;

    byte getSameCodeLength(char code, int index, char const cronixieDisplay[]);

    //set.cpp
    void _setRandomColor(bool _sec,bool fromButton=false);

    // int getNumVal(const String* req, uint16_t pos);
    // bool updateVal(const String* req, const char* key, byte* val, byte minv=0, byte maxv=255);

    // bool applyPreset(byte index, bool loadBri = true);
    // void savePreset(byte index, bool persist = true);
        

    // uint8_t flags_hac_wled_animator_switch = 1; // deflault hacs

    // #ifdef USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT
    //ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // led fx library object
    // WS2812FX* mEffects = nullptr; // this new segments replaces this, though the need for static buffers may require this as a sub-class??
    // #endif //    USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT
    // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

    #ifdef USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT
    void Init_WLED();
    void SubTask_WLED_Animation_PhaseOut();
    #endif // USE_DEVFEATURE_WLED_METHOD_ORIGINAL_ADDED_AS_EFFECT
 

    void setValuesFromMainSeg();
    void resetTimebase();
    void toggleOnOff();
    void setAllLeds();
    void setLedsStandard(bool justColors = false);
    bool colorChanged();
    void colorUpdated(int callMode);
    void updateInterfaces(uint8_t callMode);
    void handleTransitions();

    //colors.cpp
    void colorFromUint32(uint32_t in, bool secondary = false);
    void colorFromUint24(uint32_t in, bool secondary = false);
    void relativeChangeWhite(int8_t amount, byte lowerBoundary = 0);
    void colorHStoRGB(uint16_t hue, byte sat, byte* rgb); //hue, sat to rgb
    void colorCTtoRGB(uint16_t mired, byte* rgb); //white spectrum to rgb

    // LED CONFIG
    // uint16_t ledCount = 30;          // overcurrent prevented by ABL
    // bool useRGBW      = false;       // SK6812 strips can contain an extra White channel
    // bool turnOnAtBoot = true;        // turn on LEDs at power-up
    // byte bootPreset   = 0;           // save preset to load after power-up


    // byte col[4]    = { 255, 160, 0, 0 };  // current RGB(W) primary color. col[] should be updated if you want to change the color.
    // byte colSec[4] = { 0, 0, 0, 0 };      // current RGB(W) secondary color
    // byte briS     = 128;                     // default brightness

    // bool fadeTransition      = true;   // enable crossfading color transition
    // uint16_t transitionDelay = 750;    // default crossfade duration in ms

    // bool skipFirstLed  = false;        // ignore first LED in strip (useful if you need the LED as signal repeater)
    // byte briMultiplier = 100;          // % of brightness to set (to limit power, if you set it to 50 and set bri to 255, actual brightness will be 127)

    // uint16_t realtimeTimeoutMs = 2500;               // ms timeout of realtime mode before returning to normal mode
    // int arlsOffset = 0;                              // realtime LED offset
    // bool arlsDisableGammaCorrection = true;          // activate if gamma correction is handled by the source
    // bool arlsForceMaxBri = false;                    // enable to force max brightness if source has very dark colors that would be black

    // // wifi
    // bool apActive = false;
    // bool forceReconnect = false;
    // uint32_t lastReconnectAttempt = 0;
    // bool interfacesInited = false;
    // bool wasConnected = false;

    // color
    // byte colOld[4]    = { 0, 0, 0, 0 };        // color before transition
    // byte colT[4]      = { 0, 0, 0, 0 };          // color that is currently displayed on the LEDs
    // byte colIT[4]     = { 0, 0, 0, 0 };         // color that was last sent to LEDs
    // byte colSecT[4]   = { 0, 0, 0, 0 };
    // byte colSecOld[4] = { 0, 0, 0, 0 };
    // byte colSecIT[4]  = { 0, 0, 0, 0 };

    // transitions
    // bool transitionActive = false;
    // uint16_t transitionDelayDefault = transitionDelay;
    // uint16_t transitionDelayTemp = transitionDelay;
    // unsigned long transitionStartTime;
    // float tperLast = 0;        // crossfade transition progress, 0.0f - 1.0f
    // bool jsonTransitionOnce = false;

    // // brightness
    // unsigned long lastOnTime = 0;
    // bool offMode = !turnOnAtBoot;
    // byte bri = briS;
    // byte briOld = 0;
    // byte briT = 0;
    // byte briIT = 0;
    // byte briLast = 128;          // brightness before turned off. Used for toggle function
    // byte whiteLast = 128;        // white channel before turned off. Used for toggle function

   
    // effects
    byte effectCurrent = 0;
    byte effectSpeed = 128;
    byte effectIntensity = 128;
    byte effectPalette = 0;

    // preset cycling
    bool presetCyclingEnabled = false;
    byte presetCycleMin = 1, presetCycleMax = 5;
    uint16_t presetCycleTime = 12;
    unsigned long presetCycledTime = 0;
    byte presetCycCurr = presetCycleMin;
    bool presetApplyBri = true;
    bool saveCurrPresetCycConf = false;


    // presets
    uint16_t savedPresets = 0;
    int8_t currentPreset = -1;
    bool isPreset = false;

    // realtime
    byte realtimeMode = REALTIME_MODE_INACTIVE;
    byte realtimeOverride = REALTIME_OVERRIDE_NONE;
    unsigned long realtimeTimeout = 0;
    uint16_t tpmFirstFrameSize = 0;

        // void colorXYtoRGB(float x, float y, byte* rgb); // only defined if huesync disabled TODO
    // void colorRGBtoXY(byte* rgb, float* xy); // only defined if huesync disabled TODO
    void colorFromDecOrHexString(byte* rgb, char* in);
    void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)

    float minf2(float v, float w);
    float maxf2(float v, float w);




//   /**
//    * Create a duplicate of this in HACS, and slowly move HACS to follow this basic layout ("layout" = struct of what animation runs in that section)
//    * */

//   /**
//    * how a section (single animation) is configured -- my HACS needs to change to conform to this
//    * */
  
// // options
// // bit    7: segment is in transition mode
// // bits 4-6: TBD
// // bit    3: mirror effect within segment
// // bit    2: segment is on
// // bit    1: reverse segment
// // bit    0: segment is selected
// #define NO_OPTIONS2   (uint8_t)0x00
// #define TRANSITIONAL2 (uint8_t)0x80
// #define MIRROR2       (uint8_t)0x08
// #define SEGMENT_ON2   (uint8_t)0x04
// #define REVERSE2      (uint8_t)0x02
// #define SELECTED2     (uint8_t)0x01
// // #define IS_TRANSITIONAL2 ((_segments[_segment_index].options & TRANSITIONAL2) == TRANSITIONAL2)
// // #define IS_MIRROR2       ((_segments[_segment_index].options & MIRROR2      ) == MIRROR2      )
// // #define IS_SEGMENT_ON2   ((_segments[_segment_index].options & SEGMENT_ON2  ) == SEGMENT_ON2  )
// // #define IS_REVERSE2      ((_segments[_segment_index].options & REVERSE2     ) == REVERSE2     )
// // #define IS_SELECTED2     ((_segments[_segment_index].options & SELECTED2    ) == SELECTED2    )

// #define DEFAULT_BRIGHTNESS2 (uint8_t)127
// #define DEFAULT_MODE2       (uint8_t)0
// #define DEFAULT_SPEED2      (uint8_t)128
// #define DEFAULT_COLOR2      (uint32_t)0xFFAA00

// // #define NUM_COLORS       3 /* number of colors per segment */
// #define SEGCOLOR2(x)      _segments[_segment_index].colors[x]

//     // #define SEGCOLOR(x)      gamma32(_segments[_segment_index].colors[x])

// // #define SEGLEN           _virtualSegmentLength
// // #define SPEED_FORMULA_L  5 + (50*(255 - _segments[_segment_index].speed))/_virtualSegmentLength
// // #define RESET_RUNTIME    memset(_segment_runtimes, 0, sizeof(_segment_runtimes))
// /**
//  * First animations to be ported to new method, they shall both be displayed on string together
//  * Slow Glow (50%)
//  * Solid (50%) with random colour changes
//  * */

//     void showWLED(void);

//     void fill(uint32_t c);


//     uint16_t mode_static(void);
//     uint16_t mode_static_pattern();
//     uint16_t mode_tri_static_pattern();
//     uint16_t color_wipe(bool rev, bool useRandomColors);
//     uint16_t mode_color_wipe(void);
//     uint16_t mode_color_wipe_random(void);
//     uint16_t mode_color_sweep(void);
//     uint16_t mode_color_sweep_random(void);

//     #define NUM_COLORS2       3 /* number of colors per segment */



    
//     typedef struct Segment { // 24 bytes
//       uint16_t start_pixel_index;
//       uint16_t stop_pixel_index; //segment invalid if stop == 0
//       uint8_t speed; // rate/blend time
//       uint8_t intensity; // amount of animations in it
//       uint8_t palette;
//       uint8_t mode; //animation_id
//       uint8_t options; //bit pattern: msb first: transitional needspixelstate tbd tbd (paused) on reverse selected
//       uint8_t grouping;
//       uint8_t spacing;
//       uint8_t opacity;
//       uint32_t colors[NUM_COLORS2];
//       void setOption(uint8_t n, bool val)
//       {
//         if (val) {
//           options |= 0x01 << n;
//         } else
//         {
//           options &= ~(0x01 << n);
//         }
//       }
//       bool getOption(uint8_t n)
//       {
//         return ((options >> n) & 0x01);
//       }
//       bool isSelected()
//       {
//         return getOption(0);
//       }
//       bool isActive()
//       {
//         return stop_pixel_index > start_pixel_index;
//       }
//       uint16_t length()
//       {
//         return stop_pixel_index - start_pixel_index;
//       }
//       uint16_t groupLength()
//       {
//         return grouping + spacing;
//       }
//       uint16_t virtualLength()
//       {
//         uint16_t groupLen = groupLength();
//         uint16_t vLength = (length() + groupLen - 1) / groupLen;
//         if (options & MIRROR2)
//           vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED
//         return vLength;
//       }
//     } segment;
//     uint8_t _segment_index = 0;
//     uint8_t _segment_index_palette_last = 99;
//     segment _segments[MAX_NUM_SEGMENTS] = { // SRAM footprint: 24 bytes per element
//       // start, stop, speed, intensity, palette, mode, options, grouping, spacing, opacity (unused), color[]
//       { 0, 7, DEFAULT_SPEED2, 128, 0, DEFAULT_MODE2, NO_OPTIONS2, 1, 0, 255, {DEFAULT_COLOR2}}
//     };

//     /**
//      * Runtime data for that segment, ie holds animation data
//      * */
//     typedef struct Segment_runtime { // 28 bytes
//       unsigned long next_time;
//       uint32_t step;
//       uint32_t call;
//       uint16_t aux0;
//       uint16_t aux1;
//       byte* data = nullptr; // note: how all my previous hardcorded structs will need to conform to this
//       bool allocateData(uint16_t len){
//         if (data && _dataLen == len) return true; //already allocated
//         deallocateData();
//         if (_usedSegmentData + len > MAX_SEGMENT_DATA) return false; //not enough memory
//         data = new (std::nothrow) byte[len];
//         if (!data) return false; //allocation failed
//         _usedSegmentData += len;
//         _dataLen = len;
//         memset(data, 0, len);
//         return true;
//       }
//       void deallocateData(){
//         delete[] data;
//         data = nullptr;
//         _usedSegmentData -= _dataLen;
//         _dataLen = 0;
//       }
//       void reset(){next_time = 0; step = 0; call = 0; aux0 = 0; aux1 = 0; deallocateData();}
//       private:
//         uint16_t _dataLen = 0;
//     }segment_runtime;
      
//     segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 28 bytes per element
    
    
//     uint32_t _lastPaletteChange = 0;
//     uint32_t _lastShow = 0;
    
//     uint32_t timebase;
//     uint32_t now;
//     static uint16_t _usedSegmentData;
//     bool
//       _useRgbw = false,
//       _skipFirstMode,
//       _triggered;
//     uint8_t
//       mainSegment = 0,
//       rgbwMode = 3,
//       paletteFade = 0,
//       paletteBlend = 0;

//     uint16_t _length, _lengthRaw, _virtualSegmentLength;
      

//     void resetSegments();
//     void handle_palette(void);
//     CRGBPalette16 currentPalette;
//     CRGBPalette16 targetPalette;

    #endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS



#endif

// #endif
