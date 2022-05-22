#ifndef _AnimatorDALIGHTNEOPIXEL7_segH
#define _AnimatorDALIGHTNEOPIXEL7_segH 7.0

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

#define PALETTE_SOLID_WRAP (paletteBlend == 1 || paletteBlend == 3)

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

/* each segment uses 52 bytes of SRAM memory, so if you're application fails because of
  insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
#ifndef MAX_NUM_SEGMENTS
#define MAX_NUM_SEGMENTS 5 // cant be made smaller than 5 until the init process (ie rgbcctcontroller) is properly tested for all sizes
#endif // MAX_NUM_SEGMENTS

#define IBN 5100

// options
// bit    7: segment is in transition mode
// bits 4-6: TBD
// bit    3: mirror effect within segment
// bit    2: segment is on
// bit    1: reverse segment
// bit    0: segment is selected
#define NO_OPTIONS   (uint8_t)0x00
#define TRANSITIONAL (uint8_t)0x80
#define MIRROR       (uint8_t)0x08
#define SEGMENT_ON   (uint8_t)0x04
#define REVERSE      (uint8_t)0x02
#define SELECTED     (uint8_t)0x01
#define IS_TRANSITIONAL ((_segments[segment_active_index].options & TRANSITIONAL) == TRANSITIONAL)
#define IS_MIRROR       ((_segments[segment_active_index].options & MIRROR      ) == MIRROR      )
#define IS_SEGMENT_ON   ((_segments[segment_active_index].options & SEGMENT_ON  ) == SEGMENT_ON  )
#define IS_REVERSE      ((_segments[segment_active_index].options & REVERSE     ) == REVERSE     )
#define IS_SELECTED     ((_segments[segment_active_index].options & SELECTED    ) == SELECTED    )

#define DEFAULT_BRIGHTNESS (uint8_t)127
#define DEFAULT_MODE       (uint8_t)0
#define DEFAULT_SPEED      (uint8_t)128
#define DEFAULT_COLOR      (uint32_t)0xFFAA00


/* How much data bytes all segments combined may allocate */
#ifdef ESP8266
#define MAX_SEGMENT_DATA 2048
#else
#define MAX_SEGMENT_DATA 8192
#endif

#define FLASH_COUNT 4 
#define LED_SKIP_AMOUNT  0
#define MIN_SHOW_DELAY  15


//Segment option byte bits
#define SEG_OPTION_SELECTED       0
#define SEG_OPTION_REVERSED       1
#define SEG_OPTION_ON             2
#define SEG_OPTION_MIRROR         3            //Indicates that the effect will be mirrored within the segment
#define SEG_OPTION_NONUNITY       4            //Indicates that the effect does not use FRAMETIME or needs getPixelColor
#define SEG_OPTION_TRANSITIONAL   7

// some common colors
// white, red, green, blue
#define RED        (uint32_t)0xFF0000
#define GREEN      (uint32_t)0x00FF00
#define BLUE       (uint32_t)0x0000FF
#define WHITE      (uint32_t)0xFFFFFF
#define BLACK      (uint32_t)0x000000
#define YELLOW     (uint32_t)0xFFFF00
#define CYAN       (uint32_t)0x00FFFF
#define MAGENTA    (uint32_t)0xFF00FF
#define PURPLE     (uint32_t)0x400080
#define ORANGE     (uint32_t)0xFF3000
#define PINK       (uint32_t)0xFF1493
#define ULTRAWHITE (uint32_t)0xFFFFFFFF

#define NUM_COLORS 3
#define SEGCOLOR(x)      _segments[segment_active_index].colors[x]

#define FRAMETIME 25

#define SPEED_FORMULA_L  5 + (50*(255 - _segments[segment_index].speed()))/_virtualSegmentLength


  uint32_t color_from_palette(uint16_t i, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255);

  void fill(uint32_t c);
  void fill_ranged(uint32_t c); 
  void seg_fill_ranged(uint32_t c);

  uint32_t color_wheel(uint8_t pos);
  uint32_t color_blend(uint32_t color1, uint32_t color2, uint8_t blend);

  void Init_Segments();

  /**
   * Minor Code version 96 onwards requires names, as reshuffle is happening. Any old device will not respond to the correct command via number until remap is performed
   * */
  enum EFFECTS_FUNCTION__IDS
  {
    /**
     * Default
     * Desc: Same colour across all pixels.
     * Param: palette (will always use index 0 colour regardless of palette length)
     * 
     * no transition pair, draw directly (or maybe optional flag to use dynamic transition if memory allows, otherwise conserve memory for other segments)
     * */
    EFFECTS_FUNCTION__SOLID_COLOUR__ID,
    /**
     * Desc: Using EFFECTS_FUNCTION__SLOW_GLOW__ID, with pixel order set to "inorder"
     *       Each sequential pixel will repeat the palette if type is list, if gradient, then palette is linear blended across output
     * Parameters: Palette, time to blend, rate of new colours, percentage of new colours changed
     * 
     * no transition pair, draw directly (or maybe optional flag to use dynamic transition if memory allows, otherwise conserve memory for other segments)
     * */
    EFFECTS_FUNCTION__STATIC_PALETTE__ID,
    /**
     * Desc: Random change of colours (pixel order is random)
     *       Pixel elements randomly select colour from palette, then slowly blend those pixels to the new colour
     * Parameters: 
     *    Palette = Pick colours to chose from
     *    Transition time = blend time between last and new colours
     *    Transition rate = time between new colours are selected
     *    Percentage of change = Number of new colours to change this time (eg 10% of total count will randomly change)
     * 
     * Note: allocate_buffer is used as transition data
     * */
    EFFECTS_FUNCTION__SLOW_GLOW__ID,
    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */
    EFFECTS_FUNCTION__ROTATING_PALETTE__ID,
    /**
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * TODO: Add size of step as percentage ie to show 50% of 4 colours would be 2 of 4, 75% of 4 is 3
     * 
     * Note: allocate_buffer is used as transition data
     * */
    EFFECTS_FUNCTION__SEQUENTIAL_PALETTE__ID,
    
    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: WLED Animations converted  ***************************************************************************************************************************************************************************
    **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    // Static
    EFFECTS_FUNCTION__WLED_STATIC__ID,
    EFFECTS_FUNCTION__WLED_STATIC_PATTERN__ID,
    EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__ID,
    EFFECTS_FUNCTION__WLED_SPOTS__ID,
    EFFECTS_FUNCTION__WLED_PERCENT__ID,

// Palette as gradient, show active gradient of anytype spread across the palette

    // One colour changes
    EFFECTS_FUNCTION__WLED_RANDOM_COLOR__ID,
    // Wipe/Sweep/Runners     
    EFFECTS_FUNCTION__WLED_COLOR_WIPE__ID,
    EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_COLOR_SWEEP__ID,
    EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__ID,
    EFFECTS_FUNCTION__WLED_ANDROID__ID,
    EFFECTS_FUNCTION__WLED_RUNNING_RED_BLUE__ID,
    #ifdef ENABLE_EXTRA_WLED_EFFECTS
    EFFECTS_FUNCTION__WLED_RUNNING_COLOR__ID,
    EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_GRADIENT__ID,
    EFFECTS_FUNCTION__WLED_LOADING__ID,
    EFFECTS_FUNCTION__WLED_POLICE__ID,
    EFFECTS_FUNCTION__WLED_POLICE_ALL__ID,
    EFFECTS_FUNCTION__WLED_TWO_DOTS__ID,
    EFFECTS_FUNCTION__WLED_TWO_AREAS__ID,
    EFFECTS_FUNCTION__WLED_MULTI_COMET__ID,
    EFFECTS_FUNCTION__WLED_OSCILLATE__ID,
    EFFECTS_FUNCTION__WLED_BPM__ID,
    EFFECTS_FUNCTION__WLED_JUGGLE__ID,
    EFFECTS_FUNCTION__WLED_PALETTE__ID,
    EFFECTS_FUNCTION__WLED_COLORWAVES__ID,
    EFFECTS_FUNCTION__WLED_LAKE__ID,
    EFFECTS_FUNCTION__WLED_GLITTER__ID, //outside tree
    EFFECTS_FUNCTION__WLED_METEOR__ID,  //outside tree
    EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__ID,
    EFFECTS_FUNCTION__WLED_PRIDE_2015__ID,
    EFFECTS_FUNCTION__WLED_PACIFICA__ID, //outside tree
    EFFECTS_FUNCTION__WLED_SUNRISE__ID, //outside tree
    EFFECTS_FUNCTION__WLED_SINEWAVE__ID, //outside tree
    EFFECTS_FUNCTION__WLED_FLOW__ID, //outside tree
    EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__ID,
    EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__ID,
    EFFECTS_FUNCTION__WLED_MERRY_CHRISTMAS__ID,
    EFFECTS_FUNCTION__WLED_HALLOWEEN__ID,
    // Chase
    EFFECTS_FUNCTION__WLED_CHASE_COLOR__ID,
    EFFECTS_FUNCTION__WLED_CHASE_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__ID, 
    EFFECTS_FUNCTION__WLED_CHASE_FLASH__ID,
    EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__ID, 
    EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__ID,
    EFFECTS_FUNCTION__WLED_CHASE_THEATER__ID,
    EFFECTS_FUNCTION__WLED_CHASE_THEATER_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_CHASE_TRICOLOR__ID,
    EFFECTS_FUNCTION__WLED_CIRCUS_COMBUSTUS__ID,
    // Breathe/Fade/Pulse
    EFFECTS_FUNCTION__WLED_BREATH__ID,
    EFFECTS_FUNCTION__WLED_FADE__ID,
    EFFECTS_FUNCTION__WLED_FADE_TRICOLOR__ID,
    EFFECTS_FUNCTION__WLED_FADE_SPOTS__ID,
    #endif // ENABLE_EXTRA_WLED_EFFECTS    
    // Fireworks
    EFFECTS_FUNCTION__WLED_FIREWORKS__ID,
    EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID,
    EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST__ID,
    EFFECTS_FUNCTION__WLED_RAIN__ID,
    #ifdef ENABLE_EXTRA_WLED_EFFECTS
    // Sparkle/Twinkle
    EFFECTS_FUNCTION__WLED_SOLID_GLITTER__ID,
    EFFECTS_FUNCTION__WLED_POPCORN__ID,
    EFFECTS_FUNCTION__WLED_PLASMA__ID,
    EFFECTS_FUNCTION__WLED_SPARKLE__ID,
    EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__ID,
    EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__ID,
    EFFECTS_FUNCTION__WLED_TWINKLE__ID,
    EFFECTS_FUNCTION__WLED_COLORTWINKLE__ID,
    EFFECTS_FUNCTION__WLED_TWINKLEFOX__ID,
    EFFECTS_FUNCTION__WLED_TWINKLECAT__ID,
    EFFECTS_FUNCTION__WLED_TWINKLEUP__ID,
    EFFECTS_FUNCTION__WLED_DYNAMIC__ID,
    EFFECTS_FUNCTION__WLED_SAW__ID,
    EFFECTS_FUNCTION__WLED_DISSOLVE__ID,
    EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_COLORFUL__ID,
    EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__ID,
    #endif // ENABLE_EXTRA_WLED_EFFECTS    
    EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__ID,
    EFFECTS_FUNCTION__WLED_CANDLE_MULTI__ID,
    EFFECTS_FUNCTION__WLED_FIRE_FLICKER__ID,
    EFFECTS_FUNCTION__WLED_SHIMMERING_PALETTE__ID,
    
    #ifdef ENABLE_EXTRA_WLED_EFFECTS
    // Blink/Strobe
    EFFECTS_FUNCTION__WLED_BLINK__ID,
    EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_STROBE__ID,
    EFFECTS_FUNCTION__WLED_MULTI_STROBE__ID,
    EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__ID, 
    EFFECTS_FUNCTION__WLED_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_LIGHTNING__ID,     
    EFFECTS_FUNCTION__WLED_FIRE_2012__ID,
    EFFECTS_FUNCTION__WLED_RAILWAY__ID,
    EFFECTS_FUNCTION__WLED_HEARTBEAT__ID, 
    // Noise
    EFFECTS_FUNCTION__WLED_FILLNOISE8__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_1__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_2__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_3__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_4__ID,
    EFFECTS_FUNCTION__WLED_NOISEPAL__ID,
    EFFECTS_FUNCTION__WLED_PHASEDNOISE__ID,
    EFFECTS_FUNCTION__WLED_PHASED__ID,
    // Scan
    EFFECTS_FUNCTION__WLED_SCAN__ID,
    EFFECTS_FUNCTION__WLED_DUAL_SCAN__ID,
    EFFECTS_FUNCTION__WLED_LARSON_SCANNER__ID,
    EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__ID,
    EFFECTS_FUNCTION__WLED_ICU__ID,
    EFFECTS_FUNCTION__WLED_RIPPLE__ID,
    EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_COMET__ID,
    EFFECTS_FUNCTION__WLED_CHUNCHUN__ID,
    EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__ID,
    EFFECTS_FUNCTION__WLED_SINELON__ID,
    EFFECTS_FUNCTION__WLED_SINELON_DUAL__ID,
    EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_DRIP__ID,     
    #endif // ENABLE_EXTRA_WLED_EFFECTS    
    EFFECTS_FUNCTION__WLED_LENGTH__ID, // to show end of declared animation, this will have no actual effect     

    #ifdef ENABLE_SEGMENT_EFFECTS_SELECTIVE_NOTIFICATIONS // SELECTIVE meaning optional extras then "of type notification"
    EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__ID,
    EFFECTS_FUNCTION__NOTIFICATION_STATIC_OFF__ID,
    EFFECTS_FUNCTION__NOTIFICATION_FADE_ON__ID,
    EFFECTS_FUNCTION__NOTIFICATION_FADE_OFF__ID,
    EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID,
    EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID,
    #endif // ENABLE_SEGMENT_EFFECTS_SELECTIVE_NOTIFICATIONS // SELECTIVE meaning optional extras then "of type notification"
 
    /**
     * Development effects
     * @note These need to be last since without proper defines of effect names they can only be addressed by their numeric ID
     * 
     */
    
    // /**
    //  * Desc: Twinkle on, fade off
    //  *      Random burst of pixels are turned on based on palette, then fade off
    //  * 
    //  * Method: randomly pick burst of pixels to turn on, ie animation period starts of Y, then decays (thereby increasing next run time)
    //  *         Do this until X pixels (animation periods) have occured,
    //  *         Every 1 second of animation that elapses, half the brightness of all pixels (ie the fade)
    //  *         Intensity1: will dictate how many pixels are turned on
    //  *         Intensity2: may dictate the decay rate (ie much slower would leave pixels on for longer meaning more of the lights remain on) 
    //  * NotActive
    //  * EFFECTS_FUNCTION__PULSE_RANDOM_ON_FADE_OFF_ID
    //  * */
    // EFFECTS_FUNCTION__TWINKLE_RANDOM_BURSTS_ON_WITH_SLOW_FADE_OFF__ID,
    // /**
    //  * Palette is first drawn with certain lower brightness level, then
    //  * additional changes takes those colours and bumbs their brightness up to desired brightness
    //  * Few change, random pixels bumped up
    //  * Check if their brightness is <upper, before asserting them brighter (or convert to hsbcolour, flick up brightness if needed then change back to rgb)
    //  * Goes from low to high then stops
    //  * */
    // EFFECTS_FUNCTION__POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY,
    // /**
    //  * Desc: Twinkle
    //  *  Random pixels will turn off for random times (ie apply inorder palette always, then random turn colours to off and on(brightness) again)
    //  * */

    // /**
    //  * Desc: Twinkle with glimmer
    //  *  Twinkle as above, but the entire global brightness will flicker gently (so limit how much) based on pixels energy count ie twinkle filament lights
    //  * */

    // /**
    //  * 0/100% animaiton progress is low brightness, 50% flips to upper brightness
    //  * ie randomly change pixels between two brightness levels, with optional boundaries only or as range
    // */
    // EFFECTS_FUNCTION__TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK,

    // /**
    //  *  Instead of just applying new colours, also randomise the brightness levels between range
    //  * */
    // EFFECTS_FUNCTION__SLOW_GLOW_ON_BRIGHTNESS__ID,

    // /**
    //  * Desc: randomly slow glow with triangular brightness between colours
    //  * Or, so at 50% transition, the new colour is at 100%, with 100% brightness, then fade brightness back down to lower_global brightness
    //  * So, pixel red (50%) changing to pixel blue, would go red to blue (bright 100%) then fade down brightness 50%
    //  * Optional, could also be non-equal triangle ie have value for peak of traingle, so 20% would mean 0.2 seconds flash to new colour then slowly decay (0.8 seconds) to new brightness
    //  * */
    // // EFECT

    // /**
    //  * Fire animations
    //  * rename from fireplace to another name, simply fire
    //  * */

    // this will likely just use candle effect, maybe forced colour palette?/
    //  EFFECTS_FUNCTION__FIREPLACE_1D_01__ID, // solid colours, 1 100%, moving from previous to next

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Matric 3D Animations (with MxN pixel grid) ***************************************************************************************************************************************************************************
    **  Requires:     **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/



    // EFFECTS_FUNCTION__SLOW_FADE_BRIGHTNESS_ALL__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__SLOW_FADE_SATURATION_ALL__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__SLOW_FADE_BRIGHTNESS_RANDOM__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__SLOW_FADE_SATURATION_RANDOM__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__FLASH_TWINKLE_SINGLE_COLOUR_RANDOM__ID, //random leds flash to 100% brightness (modes=instant on/off, multiple pulses)
    // EFFECTS_FUNCTION__FLASH_TWINKLE_PALETTE_COLOUR_RANDOM__ID,   
   
    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Sun positions to control animation***************************************************************************************************************************************************************************
    **  Requires:    Internet to know time and location for sun calculations **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    // //linear palette group colour, changing by triggered and over a period of time (eg an alarm -30 minutes)
    // EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01, //group01? bring all together, have settings to configure how the effect runs
    // //linear palettes group colour, enabled by a time window, but sets linear gradient by sun azimuth
    // EFFECTS_FUNCTION__SUNPOSITIONS_GRADIENT_SUN_ELEVATION_01, //this is elevation only
    // //enabled by a time window, GENERATE manually, brightness by sun elevaltion, sun position from left to right by azimuth
    // //far left = east from position, and far right = wast from position using sunset position
    // EFFECTS_FUNCTION__SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01,
    // EFFECTS_FUNCTION__SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_2D_01, // If matrix/grid is connected, enable drawing this

    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_01__ID,
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_02__ID,
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_03__ID, // Using stored rgbcct palette
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_04__ID, // Using stored rgbcct palette
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_05__ID, // CCT only, mapped directly, no palette
    
    // /**
    //  * Eleveation controls that use rgbcct palette (that has elevation in its index byte) to display solid colour only
    //  * */
    // EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01__ID,
    // EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01__ID,

    // /**
    //  * Elevation controls the CCT channels only
    //  **/
    // EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01__ID,


    // // palette to step through, which gives single colour sun
    // EFFECTS_FUNCTION__SUNPOSITIONS_STEP_RGBCCT_ALARM_01,
    // //enabled CCT by azimuth 
    // EFFECTS_FUNCTION__SUNPOSITIONS_STEP_RGBCCT_SUN_ELEVATION_01,

    
    #ifdef ENABLE_EXTRA_EFFECTS_SUNPOSITIONS 
    /**
     * @brief Same as above, but no blending on palette colours, just use stepped values
     **/
    EFFECTS_FUNCTION__SUNPOSITION_ELEVATION_PALETTE_PROGRESS_STEP__ID,
    /**
     * @brief Using the elevation of the sun stored in settings, linear blend between all colours in a palette
     *        The start and end colour from the palette (0 to 100% index) is defined by the max/min elevation for that day
     *        I will also want a way to pick what these max/min are (ie setting dusk/dawn as limits for transition for CCT colours etc)
     **/
    EFFECTS_FUNCTION__SUNPOSITION_ELEVATION_PALETTE_PROGRESS_LINEAR__ID,
   


    #endif // ENABLE_EXTRA_EFFECTS_SUNPOSITIONS

    // /******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // *** Specialised: RGB Segment Clock  ***************************************************************************************************************************************************************************
    // **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************/

    // EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
    // EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
    // EFFECTS_FUNCTION__LCD_DISPLAY_BASIC_01__ID, // show number commanded via mqtt


    // /******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // *** Specialised: RGB Segment Clock  ***************************************************************************************************************************************************************************
    // **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************/




    // // Apply sinewave to brightness, 25% increasing, 50% decreasing, 25% off
    // EFFECTS_FUNCTION__PULSE_SINE_WAVE_BRIGHTNESS, //pul
    // // The one when applybrightness was broke
    // // done by applying new cl/brightness to some, then each call reducing the colour crushing it
    // //do with percent to show on at one time
    // EFFECTS_FUNCTION__PULSE_RANDOM_ON, 
    // EFFECTS_FUNCTION__PULSE_RANDOM_ON_TWO__ID,    
    // EFFECTS_FUNCTION__FLASH_TWINKLE_SEQUENTIAL__ID, // sequential flash of white on x leds 
    // EFFECTS_FUNCTION__FADE_GRADIENT__ID, //single pixels: static, twinkle, pick from palette
    // //another flash to "off" or simple set flash colour to off??
    // EFFECTS_FUNCTION__FLASH_GLIMMER_RANDOM__ID, // tinkle=leds flash independant, glimmer=leds flash with dimming effect on others
    /**
     * Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     * */
    // EFFECTS_FUNCTION__SHOWING_MULTIPLES_OF_COLOURS__ID,
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

    EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID


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
    // EFFECTS_FUNCTION__XMAS_IN_WAVES_ID
    // EFFECTS_FUNCTION__XMAS_SEQUENTIAL_ID
    // EFFECTS_FUNCTION__XMAS_SLO_GLO_ID
    // EFFECTS_FUNCTION__XMAS_CHASING_AND_FLASHING_ID
    // EFFECTS_FUNCTION__XMAS_SLOW_FADE_ID
    // EFFECTS_FUNCTION__XMAS_TWINKLE_AND_FLASH_ID
    // EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    // EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    // EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID

    /**
     * Development methods
     * */

    /**
     * Functional methods of sunrise
     * EFFECTS_FUNCTION__SUNPOSITIONS_<SOLIDCOLOUR/2D_ARRAY>
     * */
    //create a palette to leave on ALL the time, ie deep red at night instead of blue... testing for the clocks later I guess

    /**
     * Not currently working
     * */
    /**
     * Clock animations for 3d printed display
     **/
    EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
    EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
    EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID,

    /**
     * Create special musical christmas lights, that will only be available on esp32 with a speaker (larger RAM)
     * */

    /**
     * Palette developing
     * */
    // Step 1 of palette merging: Making it so any palette can span the entire segment, static, no effect.
    EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__ID,
    /**
     * Designing and quick test of animations before creating its own animaiton profile
     * */
    EFFECTS_FUNCTION__TESTER__ID,
    /**
     * @brief Testing can animation_colours be moved into the dynamic data array
     * 
     */
    // EFFECTS_FUNCTION__STATIC_PALETTE_DYNAMIC_METHOD__ID,
    // EFFECTS_FUNCTION__SLOW_GLOW_DYNAMIC_METHOD__ID,

    // Length
    EFFECTS_FUNCTION__LENGTH__ID
  };         


  /** PATTERNS ************
   * RANDOM - LED pixel indexing chosen at random
   * INORDER - first to last pixel index
   * CENTRE_OUT   - Chosen from center out, showing same pixel on outward pairs (update together)
   * GRADIENT - takes any palette, and stretches the palette across the leds
   *    -- Full span
   *    -- Repeated X times   * 
   * MAPPED -
  **************/ 
/**
 * This should be phased out as an option, instead be part of effects only
 * perhaps moved into "getting palette colour" instead of how its displayed
 * */
  enum PALETTE_PATTERN_IDS{ //TRANSITION_ORDER{
    TRANSITION_ORDER__NONE__ID=0,
    TRANSITION_ORDER__RANDOM__ID,
    TRANSITION_ORDER__INORDER__ID,
    // TRANSITION_ORDER_CENTRE_OUT_ID,
    // TRANSITION_ORDER_GRADIENT_ID,
    TRANSITION_ORDER_LENGTH_ID
  }; 
  

  // /**
  //  * @brief 
  //  * Phased out since new dyanmic buffer can hold this data
  //  * not needed when blend can now record full rgbcct type
  //  * 
  //  */
  // #ifndef DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD
  // struct AnimationColours_SOLID_COLOUR
  // {
  //   RgbcctColor StartingColor;
  //   RgbcctColor DesiredColour;
  // };
  // AnimationColours_SOLID_COLOUR animation_colours_rgbcct;
  // #endif // DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD


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
    uint8_t function = EFFECTS_FUNCTION__STATIC_PALETTE__ID;
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


    //realtime modes
    #define REALTIME_MODE_INACTIVE    0
    #define REALTIME_MODE_GENERIC     1
    #define REALTIME_MODE_UDP         2
    #define REALTIME_MODE_HYPERION    3
    #define REALTIME_MODE_E131        4
    #define REALTIME_MODE_ADALIGHT    5
    #define REALTIME_MODE_ARTNET      6
    #define REALTIME_MODE_TPM2NET     7

    //realtime override modes
    #define REALTIME_OVERRIDE_NONE    0
    #define REALTIME_OVERRIDE_ONCE    1
    #define REALTIME_OVERRIDE_ALWAYS  2


    // #define SEGLEN           _virtualSegmentLength
    // #define SPEED_FORMULA_L  5 + (50*(255 - _segments[_segment_index].speed))/_virtualSegmentLength
    // #define RESET_RUNTIME    memset(_segment_runtimes, 0, sizeof(_segment_runtimes))
    /**
     * First animations to be ported to new method, they shall both be displayed on string together
     * Slow Glow (50%)
     * Solid (50%) with random colour changes
     * */

    #define NUM_COLORS2       3 /* number of colors per segment */

    uint32_t _lastPaletteChange = 0;
    // uint32_t _lastShow = 0;

    
    //uint32_t timebase;
    //uint32_t now;
    // bool
    //   _useRgbw = false,
    //   // _skipFirstMode,
    //   _triggered;
    // uint8_t
    //   mainSegment = 0,
    //   rgbwMode = 3,
    uint8_t  paletteFade = 0;
   uint8_t   paletteBlend = 0;

    uint16_t //_length, _lengthRaw, 
    _virtualSegmentLength;

    #define D_HARDWARE_ELEMENT_COLOUR_ORDER_UNUSED_STATE 7
    typedef union {
      uint16_t data; // allows full manipulating using union memory overlap
      struct { 
        /** 3 bits of memory = 9 combinations
         *  0 - channel 0 eg [R----]
         *  1 - channel 1 eg [-R---]
         *  2 - channel 2 eg [--R--]
         *  3 - channel 3 eg [---R-]
         *  4 - channel 4 eg [----R]
         *  5 - reserved
         *  6 - reserved
         *  7 - reserved
         *  8 - not active eg -----
         * */
        union{
          uint16_t red : 3;        // bit 0-2
          uint16_t r   : 3;        // bit 0-2
        };
        union{
          uint16_t green : 3;      // bit 3-5
          uint16_t g     : 3;      // bit 3-5
        };
        union{
          uint16_t blue : 3;       // bit 6-8
          uint16_t b : 3;       // bit 6-8
        };
        union{
          uint16_t white_warm : 3; // bit 9-11
          uint16_t w : 3; // bit 9-11
          uint16_t w1 : 3; // bit 9-11
        };
        union{
          uint16_t white_cold : 3; // bit 12-14
          uint16_t c : 3; // bit 12-14
          uint16_t w2 : 3; // bit 12-14
        };
        uint16_t reserved : 1;   // bit 15
      };
    } HARDWARE_ELEMENT_COLOUR_ORDER;


    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        // enable animations (pause)
        uint16_t fEnable_Animation : 1;
        uint16_t fForceUpdate : 1; 
        uint16_t fRunning : 1;


        uint16_t ColourComponentsUpdatedButNotYetApplied : 1; // new colours, not yet applied

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

    // enum Colour_Type{
    //   COLOUR_TYPE_RGB_ID,
    //   COLOUR_TYPE_RGBW_ID,
    //   COLOUR_TYPE_RGBCCT_ID,
    // };
    uint8_t GetSizeOfPixel(RgbcctColor_Controller::LightSubType colour_type);

/**
 * How to handle multiple neopixelbus, since each segment can share them
 * 
 * */




    /**
     * previous animation settings will be moved into here
     * */
    typedef struct Segment 
    { // 24 bytes
      struct PIXEL_INDEX_RANGE{
          uint16_t start = 0;
          uint16_t stop  = 0; // if equal, range is not active
      }pixel_range;

      uint8_t options = NO_OPTIONS; //bit pattern: msb first: transitional needspixelstate tbd tbd (paused) on reverse selected
      uint8_t grouping = 1; //multiplers
      uint8_t spacing = 0;
      uint32_t tSaved_AnimateRunTime = millis();

      uint8_t effect_id = EFFECTS_FUNCTION__STATIC_PALETTE__ID;

      uint16_t pixels_to_update_this_cycle = 0;

      HARDWARE_ELEMENT_COLOUR_ORDER hardware_element_colour_order;
      RgbcctColor_Controller::LightSubType colour_type = 
      RgbcctColor_Controller::LightSubType::LIGHT_TYPE__RGB__ID; // default is RGB, this is used by animations to know what method to generate

      /**
       * @brief Moving neopixelbus and pin here, so each segment can also be on different pins
       **/
      // #ifdef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
      //   NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;  // needs moved to segments array so a pin can also be in there
      //   int8_t bus_pin = -1;
      //   // void Init_NeoPixelBus(int8_t pin);
      // #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
  
    //   uint8_t opacity = 255; //??

    // To be phased out, only for easy conversion
      uint32_t colors[NUM_COLORS2] = {DEFAULT_COLOR}; //? really not needed or understood

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
       * This allows the segment to have optional brightness per segment, by default is set to maximum
       * and will therefore have no effect 
       **/
      uint8_t brightness_optional = 255;
      /**
       * Transition settings
       * */
      struct TRANSITION_SETTINGS{
        /**
         * Pixel order (eg Random, InOrder)
         * */
        uint8_t order_id = 0; // Is this still needed? it probably just needs passing as a method for getting colours from palette
        /**
         * Change method (eg Blend, Instant)
         * */
        uint8_t method_id = 2;  // Is this still needed?
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
        uint16_t time_ms = 1000;  
        /**
         * legacy value from wled, when updated via function, it will select the rate/time for animation
         * */
        uint8_t speed = DEFAULT_SPEED; // rate/blend time
      }transition;

      uint8_t speed() //legacy for wled effects, made up from time/rate
      {
        return transition.speed;
      }
      void set_speed(uint8_t v) //legacy for wled effects, made up from time/rate
      {
        transition.speed = v;
        // return 127;
      }

      uint8_t intensity_value = 127; //tmp value, as intensity/rate/time need to be make into a value (or optional link them together)      
      void set_intensity(uint8_t v) //legacy for wled effects, made up from time/rate
      {
        intensity_value = v;
      }
      uint8_t intensity() //legacy for wled effects, made up from time/rate
      {
        return intensity_value; // amount of animations in it
      }

      void setOption(uint8_t n, bool val)
      {
        if (val) {
          options |= 0x01 << n;
        } else
        {
          options &= ~(0x01 << n);
        }
      }
      bool getOption(uint8_t n)
      {
        return ((options >> n) & 0x01);
      }
      bool isSelected()
      {
        return getOption(0);
      }
      bool isActive()
      {
        return pixel_range.stop > pixel_range.start;
      }
      #ifdef USE_DEVFEATURE_FIX_TO_PIXEL_LENGTH
      uint16_t length()
      {
        return pixel_range.stop - pixel_range.start; // this +1 needs removed, as "legnth" should only be "50-0 => 50 total" but for loops need to do "length-1 so its 0 to 49"
      }
      #else
      uint16_t length()
      {
        return pixel_range.stop - pixel_range.start + 1; // this +1 needs removed, as "legnth" should only be "50-0 => 50 total" but for loops need to do "length-1 so its 0 to 49"
      }
      #endif // USE_DEVFEATURE_FIX_TO_PIXEL_LENGTH
      
      uint16_t length_m() //temporary using my version until the above function indexing is fixed
      {
        return pixel_range.stop - pixel_range.start; // this +1 needs removed, as "legnth" should only be "50-0 => 50 total" but for loops need to do "length-1 so its 0 to 49"
      }
      uint16_t groupLength()
      {
        return grouping + spacing;
      }
      uint16_t virtualLength()
      {
        uint16_t groupLen = groupLength();
        uint16_t vLength = (length() + groupLen - 1) / groupLen;
        if (options & MIRROR)
          vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED
        return vLength;
      }
      
    }segment_settings;
    segment_settings _segments[MAX_NUM_SEGMENTS];
    
    // Flags and states that are used during one transition and reset when completed
    struct SEGMENT_ANIMATION_OVERRIDES
    {
      uint8_t fRefreshAllPixels = false;
      /**
       * Can't be zero, as that means not active
       * */
      uint16_t time_ms = 1000; //on boot
      uint16_t rate_ms = 1000;
    }segment_animation_override; // ie "oneshot" variables that get checked and executed one time only

    /**
     * values that are used as global iters rather than passing between each function
     * */
    struct segment_shared_iters
    {
      // uint8_t index = 0;
      // uint8_t index = 0; segment_active_index
      uint8_t index_palette_last = 99; //mine is longer
    }segment_iters;

    uint8_t segment_active_index = 0;
      
    /**
     * Runtime data for that segment, ie holds animation data
     * */
    typedef struct Segment_runtime
    { // 28 bytes

      /**
       * Next few bytes are a simple version that dont require structs
       * */
      // unsigned long next_time;
      uint32_t step;
      uint32_t call;

      /**
       * Simple data values animations can use, anything beyound these will require large buffer
       * */
      uint16_t aux0 = 0;
      uint16_t aux1 = 0;
      uint16_t aux2 = 0;
      uint16_t aux3 = 0;

      /**
       * This likely wont work, as it needs to be moved into its own class so it can have mulitple instances
       * This should work, as its pointers and creating on the heap
       * */
      byte* data = nullptr; // note: how all my previous hardcorded structs will need to conform to this
      bool allocateData(uint16_t len)
      {
        if (data && _dataLen == len) return true; //already allocated
        deallocateData();
        if (_usedSegmentData + len > MAX_SEGMENT_DATA)
        {
          // AddLog(LOG_LEVEL_WARN, PSTR("allocateData not enough memory"));
          return false; //not enough memory
        }
        data = new (std::nothrow) byte[len];
        if (!data) return false; //allocation failed
        _usedSegmentData += len;
        _dataLen = len;
        memset(data, 0, len);
        return true;
      }
      void deallocateData()
      {
        delete[] data;
        data = nullptr;
        _usedSegmentData -= _dataLen;
        _dataLen = 0;
      }

      /**
       * Each segment will have its own animator
       * This will also need to share its index into the animation so it knows what segments to run
       * */
      NeoPixelAnimator* animator = new NeoPixelAnimator(1, NEO_MILLISECONDS); //one animator for each segment, which is only init when needed or else delete

      bool animation_has_anim_callback = false; //should be dafult on start but causing no animation on start right now
      
      // can I default this to nullptr when not used, will this actually reduce memory?
      RgbcctColor_Controller* rgbcct_controller = new RgbcctColor_Controller(); // can this be rolled into a buffer? so its only defined when needed
      RgbcctColor* active_rgbcct_colour_p = nullptr; //what is this then? internal conversions to output? (ie can I leave this as private)
    
      /**
       * Using "index" inside animator as segment index
       * */
      ANIM_FUNCTION_SIGNATURE;

      void reset(){
        //next_time = 0; 
        step = 0; 
        call = 0; 
        aux0 = 0; 
        aux1 = 0; 
        animator = new NeoPixelAnimator(1, NEO_MILLISECONDS);

        // palette.id = mPaletteI->PALETTELIST_STATIC_CHRISTMAS_03_ID;
        // transition.rate_ms = 100;
        // transition.time_ms = 80;
        // transition.order_id = TRANSITION_ORDER_INORDER_ID;
        // effect_id = EFFECTS_FUNCTION__STATIC_PALETTE_ID;
        // aux0 = EFFECTS_REGION_COLOUR_SELECT_ID;

        deallocateData();
                
      }

      // private:
        uint16_t _dataLen = 0;
    }segment_runtime;
    segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 28 bytes per element



  struct TransitionColourPairs
  {
    RgbcctColor StartingColour;
    RgbcctColor DesiredColour;
  };
  
  // TransitionColourPairs* 
  void GetTransitionColourBuffer(byte* allocated_buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor_Controller::LightSubType pixel_type, mAnimatorLight::TransitionColourPairs* pair_test);
  bool SetTransitionColourBuffer(byte* allocated_buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor_Controller::LightSubType pixel_type, RgbcctColor starting_colour, RgbcctColor desired_colour);


  /**
   * @brief New method that allows getting WLED basic 3 colours, but also applying the colour when getting, the same as my palette method
   * 
   */
  RgbcctColor GetSegmentColour(uint8_t colour_index, uint8_t segment_index = 0);
// #define SEGCOLOR(x)      _segments[segment_active_index].colors[x]





  enum EFFECTSREGION
  {
      EFFECTS_REGION_COLOUR_SELECT_ID=0,
      EFFECTS_REGION_ANIMATE_ID
  };      

  /**
   * @brief 
   *to be sorted
    * 
    */
  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
    #define D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH 55
    uint16_t editable_mapped_array_data_array[D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH];
    
    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        // enable animations (pause)
        uint16_t fEnabled : 1;
        /**
         * 0 - None
         * 1 - Basic multiplier
         * 2 - Using mapped index array 
        */
        uint16_t multiplier_mode_id : 4; // 2 bit : 4 levels
        uint16_t mapped_array_editable_or_progmem : 1;
      };
    } PIXEL_GROUPED_FLAGS;
    
    struct PIXEL_GROUPED{
      uint8_t method_id = 0;
      uint8_t multiplier = 5; // nearby pixels repeat colours
      struct MAPPED_ARRAY_DATA{
        uint16_t* values = nullptr;
        uint8_t  index = 0;
        uint8_t  length = 0;
      }mapped_array_data;
      PIXEL_GROUPED_FLAGS flags;
    }pixel_group;
  
  #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING



  void Init_Segments_RgbcctControllers();


  mAnimatorLight& setAnimFunctionCallback_Segments_Indexed(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE);
  void StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index = 0);


  void Segments_SetPixelColor_To_Static_Pallete(uint16_t palette_id);


  void Segments_UpdateDesiredColourFromPaletteSelected(uint16_t segment_index = 0);
  void AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(const AnimationParam& param);
  void AnimationProcess_Generic_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(const AnimationParam& param);
  void AnimationProcess_Generic_SingleColour_AnimationColour_LinearBlend_Segments_Dynamic_Buffer(const AnimationParam& param);

  bool SetTransitionColourBuffer_StartingColour(byte* buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor_Controller::LightSubType pixel_type, RgbcctColor starting_colour);
  bool SetTransitionColourBuffer_DesiredColour(byte* buffer, uint16_t buflen, uint16_t pixel_index,  RgbcctColor_Controller::LightSubType pixel_type, RgbcctColor starting_colour);

  void DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();


  void Segments_Dynamic_Buffer_UpdateStartingColourWithGetPixel();

  /**
   * My animations (and their animators where applicable)
   * */
  void SubTask_Segment_Animate_Function__Solid_Static_Single_Colour(); 
  void SubTask_Segment_Animate_Function__Static_Palette();
  void SubTask_Segment_Animate_Function__Slow_Glow();
  void SubTask_Segment_Animation__Sequential_Palette();
  void SubTask_Segment_Animation__Rotating_Palette();
  void Segments_RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction);
  #ifdef ENABLE_EXTRA_EFFECTS_SUNPOSITIONS
  void SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_Step();       
  void SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_LinearBlend();   
  void SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01();
  void SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01();
  #endif // ENABLE_EXTRA_EFFECTS_SUNPOSITIONS

  void SubTask_Flasher_Animate_Function_Tester();
  // void SubTask_Segment_Animate_Function__Slow_Glow_Animation_Struct_Testing();

  
  // Static
  void SubTask_Segment_Animation__Static();
  void SubTask_Segment_Animation__Static_Pattern();
  void SubTask_Segment_Animation__Tri_Static_Pattern();
  void SubTask_Segment_Animation__Base_Spots(uint16_t threshold);
  void SubTask_Segment_Animation__Spots();
  void SubTask_Segment_Animation__Percent();
  // One colour changes
  void SubTask_Segment_Animation__Random_Colour();
  // Wipe/Sweep/Runners 
  void BaseSubTask_Segment_Animation__Base_Colour_Wipe(bool rev, bool useRandomColors);
  void SubTask_Segment_Animation__Colour_Wipe();
  void SubTask_Segment_Animation__Colour_Wipe_Random();
  void SubTask_Segment_Animation__Colour_Sweep();
  void SubTask_Segment_Animation__Colour_Sweep_Random();
  void SubTask_Segment_Animation__TriColour();
  void SubTask_Segment_Animation__Android();
  void SubTask_Segment_Animation__Base_Running(bool saw);
  void SubTask_Segment_Animation__Base_Running(uint32_t color1, uint32_t color2);
  void SubTask_Segment_Animation__Running_Red_Blue();
  #ifdef ENABLE_EXTRA_WLED_EFFECTS
  void SubTask_Segment_Animation__Running_Colour();
  void SubTask_Segment_Animation__Running_Random();
  void SubTask_Segment_Animation__Base_Gradient(bool loading);
  void SubTask_Segment_Animation__Gradient();
  void SubTask_Segment_Animation__Loading();
  void SubTask_Segment_Animation__Base_Police(uint32_t color1, uint32_t color2, bool all);
  void SubTask_Segment_Animation__Police();
  void SubTask_Segment_Animation__Polce_All();
  void SubTask_Segment_Animation__Two_Dots();
  void SubTask_Segment_Animation__Two_Areas();
  void SubTask_Segment_Animation__Multi_Comet();
  void SubTask_Segment_Animation__Oscillate();
  void SubTask_Segment_Animation__BPM();
  void SubTask_Segment_Animation__Juggle();
  void SubTask_Segment_Animation__Palette();
  void SubTask_Segment_Animation__ColourWaves();
  void SubTask_Segment_Animation__Lake();
  void SubTask_Segment_Animation__Glitter();
  void SubTask_Segment_Animation__Meteor();
  void SubTask_Segment_Animation__Metoer_Smooth();    
  void SubTask_Segment_Animation__Pride_2015();    
  CRGB pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);   
  void SubTask_Segment_Animation__Pacifica();    
  void SubTask_Segment_Animation__Sunrise();    
  void SubTask_Segment_Animation__Sinewave();    
  void SubTask_Segment_Animation__Flow();    
  void SubTask_Segment_Animation__Base_Phased(uint8_t moder);
  void SubTask_Segment_Animation__PhasedNoise();    
  void SubTask_Segment_Animation__Phased();    
  void SubTask_Segment_Animation__Running_Lights();    
  void SubTask_Segment_Animation__Rainbow_Cycle();    
  void SubTask_Segment_Animation__Merry_Christmas();    
  void SubTask_Segment_Animation__Halloween();    
  // Chase    
  void SubTask_Segment_Animation__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette);
  void SubTask_Segment_Animation__Chase_Colour();
  void SubTask_Segment_Animation__Chase_Random();
  void SubTask_Segment_Animation__Chase_Rainbow();
  void SubTask_Segment_Animation__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette);
  void SubTask_Segment_Animation__Chase_Flash();
  void SubTask_Segment_Animation__Chase_Flash_Random();
  void SubTask_Segment_Animation__Chase_Rainbow_White();
  void SubTask_Segment_Animation__Chase_Theater();
  void SubTask_Segment_Animation__Chase_Theatre_Rainbow();
  void SubTask_Segment_Animation__Base_Chase_TriColour(uint32_t color1, uint32_t color2);
  void SubTask_Segment_Animation__Chase_TriColour();
  void SubTask_Segment_Animation__Circus_Combustus();
  // Breathe/Fade/Pulse
  void SubTask_Segment_Animation__Breath();
  void SubTask_Segment_Animation__Fade();
  void SubTask_Segment_Animation__Fade_TriColour();
  void SubTask_Segment_Animation__Fade_Spots();
  #endif // ENABLE_EXTRA_WLED_EFFECTS
  // Fireworks
  void SubTask_Segment_Animation__Fireworks();
  void SubTask_Segment_Animation__Exploding_Fireworks();
  void SubTask_Segment_Animation__Fireworks_Starburst();
  void SubTask_Segment_Animation__Rain();
// Sparkle/Twinkle
  #ifdef ENABLE_EXTRA_WLED_EFFECTS
  void SubTask_Segment_Animation__Solid_Glitter();
  void SubTask_Segment_Animation__Popcorn();
  void SubTask_Segment_Animation__Plasma();
  void SubTask_Segment_Animation__Sparkle();
  void SubTask_Segment_Animation__Sparkle_Flash();
  void SubTask_Segment_Animation__Sparkle_Hyper();
  void SubTask_Segment_Animation__Twinkle();
  CRGB SubTask_Segment_Animation__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat);
  void SubTask_Segment_Animation__Base_Twinkle_Fox(bool cat);
  void SubTask_Segment_Animation__Twinkle_Colour();
  void SubTask_Segment_Animation__Twinkle_Fox();
  void SubTask_Segment_Animation__Twinkle_Cat();
  void SubTask_Segment_Animation__Twinkle_Up();
  void SubTask_Segment_Animation__Dynamic();
  void SubTask_Segment_Animation__Saw();
  void SubTask_Segment_Animation__Base_Dissolve(uint32_t color);
  void SubTask_Segment_Animation__Dissolve();
  void SubTask_Segment_Animation__Dissolve_Random();
  void SubTask_Segment_Animation__ColourFul();
  void SubTask_Segment_Animation__Traffic_Light();
  #endif // ENABLE_EXTRA_WLED_EFFECTS
  void SubTask_Segment_Animation__Candle_Base(uint8_t use_multi = false);
  void SubTask_Segment_Animation__Candle_Single();
  void SubTask_Segment_Animation__Candle_Multi();
  void SubTask_Segment_Animation__Fire_Flicker();
  void SubTask_Segment_Animation__Shimmering_Palette();
  
  #ifdef ENABLE_EXTRA_WLED_EFFECTS
  // Blink/Strobe
  void SubTask_Segment_Animation__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette);
  void SubTask_Segment_Animation__Blink();
  void SubTask_Segment_Animation__Blink_Rainbow();
  void SubTask_Segment_Animation__Strobe();
  void SubTask_Segment_Animation__Strobe_Multi();
  void SubTask_Segment_Animation__Strobe_Rainbow();
  void SubTask_Segment_Animation__Rainbow();
  void SubTask_Segment_Animation__Lightning();
  void SubTask_Segment_Animation__Fire_2012();
  void SubTask_Segment_Animation__Railway();
  void SubTask_Segment_Animation__Heartbeat();
  //Noise
  void SubTask_Segment_Animation__FillNoise8();
  void SubTask_Segment_Animation__Noise16_1();
  void SubTask_Segment_Animation__Noise16_2();
  void SubTask_Segment_Animation__Noise16_3();
  void SubTask_Segment_Animation__Noise16_4();
  void SubTask_Segment_Animation__Noise_Pal();
  // Scan
  void SubTask_Segment_Animation__Base_Scan(bool dual);
  void SubTask_Segment_Animation__Scan();
  void SubTask_Segment_Animation__Scan_Dual();
  void SubTask_Segment_Animation__Base_Larson_Scanner(bool dual);
  void SubTask_Segment_Animation__Larson_Scanner();
  void SubTask_Segment_Animation__Larson_Scanner_Dual();
  void SubTask_Segment_Animation__ICU();
  void SubTask_Segment_Animation__Base_Ripple(bool rainbow);
  void SubTask_Segment_Animation__Ripple();
  void SubTask_Segment_Animation__Ripple_Rainbow(); 
  void SubTask_Segment_Animation__Comet();
  void SubTask_Segment_Animation__Chunchun();
  void SubTask_Segment_Animation__Bouncing_Balls();
  void SubTask_Segment_Animation__Base_Sinelon(bool dual, bool rainbow=false);
  void SubTask_Segment_Animation__Sinelon();
  void SubTask_Segment_Animation__Sinelon_Dual();
  void SubTask_Segment_Animation__Sinelon_Rainbow();
  void SubTask_Segment_Animation__Drip();
  #endif // ENABLE_EXTRA_WLED_EFFECTS

  // Temporary helper functions to be cleaned up and converted
  void blur(uint8_t blur_amount);
  void fade_out(uint8_t rate);
  uint32_t crgb_to_col(CRGB fastled);
  CRGB col_to_crgb(uint32_t);
  uint8_t get_random_wheel_index(uint8_t pos);
  uint16_t triwave16(uint16_t in);
  uint16_t mode_palette();



  void CommandSet_ColourTypeID(uint8_t id, uint8_t segment_index = 0);
  const char* GetColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen);
  int8_t GetColourTypeIDbyName(const char* c);

  void CommandSet_ColourHeatMap_Palette(float* array_val, uint8_t array_length, uint8_t style_index = 0, uint8_t palette_id = 255);



  void Set_Segment_ColourType(uint8_t segment_index, uint8_t light_type);

  // #ifdef ENABLE_CRGBPALETTES_IN_PROGMEM
  // void load_gradient_palette(uint8_t index);
  // #endif // ENABLE_CRGBPALETTES_IN_PROGMEM


#ifdef ENABLE_SEGMENT_EFFECTS_SELECTIVE_NOTIFICATIONS // SELECTIVE meaning optional extras then "of type notification"


  /**
   * @brief These need to be turned into a list of segment style animations
   * 
   */
  enum NOTIF_MODE{
    NOTIF_MODE_STATIC_OFF_ID=0,
    NOTIF_MODE_STATIC_ON_ID,
    NOTIF_MODE_FADE_OFF_ID,
    NOTIF_MODE_FADE_ON_ID,
    NOTIF_MODE_BLINKING_ID,
    NOTIF_MODE_PULSING_ID,
    NOTIF_MODE_LENGTH_ID
  };

  enum SIMPLE_EFFECT_ON_COLOUR_IDS{
    SIMPLE_EFFECT_ON_COLOUR_LINEAR_BLEND_ID=0,
    SIMPLE_EFFECT_ON_COLOUR_LINEAR_BLEND_AND_BACK_ID,
    SIMPLE_EFFECT_ON_COLOUR_BLINK5050_ID,
    SIMPLE_EFFECT_ON_COLOUR_BLINK1090_ID,
    SIMPLE_EFFECT_ON_COLOUR_STATIC_ON_ID,
    SIMPLE_EFFECT_ON_COLOUR_FADE_ON_ID,
    SIMPLE_EFFECT_ON_COLOUR_LENGTH_IDS
  };


  typedef union {
    uint8_t data; // allows full manipulating
    struct { 
      uint8_t fForcePanelUpdate : 1;
      uint8_t fShowPanelUpdate  : 1;
      uint8_t fEnableTimeoutAll : 1;    
      // Reserved
      uint8_t reserved : 4;
    };
  } NOTIFICATION_FLAGS;
  struct NOTIFICATIONS_DATA{
    NOTIFICATION_FLAGS flags;
    /**
     * Using full Rgbcct, with possible Rgbw only to save 1 byte per notif pixel
     * */        
    RgbcctColor colour;
    /**
     * Using full 255 range
     * */
    // uint8_t brightness = 0;  
    /**
     * Using full 255 range
     * */
    uint8_t progress = 0;
    /**
     * Mode
     * */
    uint8_t  mode = 0;//NOTIF_MODE_STATIC_OFF_ID;
    /**
     * Mode
     * */    
    // uint16_t period_ms = 1000; 
    /**
     * Speed 0-255, relative to period
     * */       
    // uint8_t speed = 0;  // speed should control period for timing, or Rate? 
    /**
     * Trigger times
     * */   
    uint32_t tSavedUpdate = 0;
    uint16_t tRateUpdate = 10;
    uint32_t tSavedDuration = 0;
    /**
     * Seconds until auto turn off
     * */   
    uint16_t auto_time_off_secs = 0; // reset pixel to off
  }notification_data;
  void SubTask_Segment_Animate_Function__Notification_Static_On();
  void SubTask_Segment_Animate_Function__Notification_Static_Off();
  void SubTask_Segment_Animate_Function__Notification_Fade_On();
  void SubTask_Segment_Animate_Function__Notification_Fade_Off();
  void SubTask_Segment_Animate_Function__Notification_Blinking();
  void SubTask_Segment_Animate_Function__Notification_Pulsing();
#endif // ENABLE_SEGMENT_EFFECTS_SELECTIVE_NOTIFICATIONS



  #ifdef ENABLE_DEVFEATURE_RGB_CLOCK

  byte displayMode = 1;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)

  byte lastSecond = 0;
  uint16_t testnum = 0;
  byte startColor = 0;                             // "index" for the palette color used for drawing
  // byte displayMode = 0;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)
  byte colorOffset = 29;//32;                           // default distance between colors on the color palette used between digits/leds (in overlayMode)


  void LCDDisplay_colorOverlay() ;
  void LCDDisplay_updateDisplay(byte color, byte colorSpacing) ;


  #define LED_PWR_LIMIT 750                        // 750mA - Power limit in mA (voltage is set in setup() to 5v)
  #define LED_DIGITS 4                             // 4 or 6 digits, can only be an even number as...
  // notice 3 less below since I soldered, no single strip
  #define LED_PER_DIGITS_STRIP 44//47                  // ...two digits are made out of one piece of led strip with 47 leds...
  #define LED_BETWEEN_DIGITS_STRIPS 5              // 5 leds between above strips - and all this gives us LED_COUNT... :D
  #define LED_COUNT ( LED_DIGITS / 2 ) * LED_PER_DIGITS_STRIP + ( LED_DIGITS / 3 ) * LED_BETWEEN_DIGITS_STRIPS

  byte segGroups[14][2] = {         // 14 segments per strip, each segment has 1-x led(s). So lets assign them in a way we get something similar for both digits
    // right (seen from front) digit. This is which led(s) can be seen in which of the 7 segments (two numbers: First and last led inside the segment, same on TE):
    {  13,  15 },                     // top, a
    {  10, 12 },                     // top right, b
    { 6, 8 },                     // bottom right, c
    { 3, 5 },                     // bottom, d
    { 0, 2 },                     // bottom left, e
    {  16,  18 },                     // top left, f
    {  19,  21 },                     // center, g
    // left (seen from front) digit
    { 35, 37 },                     // top, a
    { 38, 40 },                     // top right, b
    { 22, 24 },                     // bottom right, c
    { 25, 27 },                     // bottom, d
    { 28, 30 },                     // bottom left, e
    { 32, 34 },                     // top left, f
    { 41, 43 }                      // center, g
  };


  // Using above arrays it's very easy to "talk" to the segments. Simply use 0-6 for the first 7 segments, add 7 (7-13) for the following ones per strip/two digits
  byte digits[14][7] = {                    // Lets define 10 numbers (0-9) with 7 segments each, 1 = segment is on, 0 = segment is off
    {   1,   1,   1,   1,   1,   1,   0 },  // 0 -> Show segments a - f, don't show g (center one)
    {   0,   1,   1,   0,   0,   0,   0 },  // 1 -> Show segments b + c (top and bottom right), nothing else
    {   1,   1,   0,   1,   1,   0,   1 },  // 2 -> and so on...
    {   1,   1,   1,   1,   0,   0,   1 },  // 3
    {   0,   1,   1,   0,   0,   1,   1 },  // 4
    {   1,   0,   1,   1,   0,   1,   1 },  // 5
    {   1,   0,   1,   1,   1,   1,   1 },  // 6
    {   1,   1,   1,   0,   0,   0,   0 },  // 7
    {   1,   1,   1,   1,   1,   1,   1 },  // 8
    {   1,   1,   1,   1,   0,   1,   1 },  // 9
    {   0,   0,   0,   1,   1,   1,   1 },  // t -> some letters from here on (index 10-13, so this won't interfere with using digits 0-9 by using index 0-9
    {   0,   0,   0,   0,   1,   0,   1 },  // r
    {   0,   1,   1,   1,   0,   1,   1 },  // y
    {   0,   1,   1,   1,   1,   0,   1 }   // d
  };

    void SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_01();
    void SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_02();
    void SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01();
    void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01();
    void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02();
    void LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing);
    void LCDDisplay_showDigit(byte digit, byte color, byte pos);
    void LCDDisplay_showSegment(byte segment, byte color, byte segDisplay);
    void LCDDisplay_showDots(byte dots, byte color);
    RgbcctColor ColorFromPaletteLCD(uint16_t palette_id, uint8_t index, bool apply_global_brightness = true);
    uint8_t tempcol = 0;
    uint16_t lcd_display_show_number = 0;

    #endif // ENABLE_DEVFEATURE_RGB_CLOCK


    void Segment_SubTask_Flasher_Animate_Function__TEST_SolidRandom();
    void Segments_SetLEDOutAmountByPercentage(uint8_t percentage, uint8_t segment_index = 0); 
    void resetSegments();
    // void UpdatePalette_FastLED_TargetPalette(void);
    

    /***************
     * 
     * Command List
     * 
     * START
     * *********************/
    
    void CommandSet_PaletteID(uint8_t value, uint8_t segment_index = 0);

    void CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index = 0);
    int8_t GetFlasherFunctionIDbyName(const char* f);
    const char* GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index = 0);
    const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen);

    void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index = 0);

    void CommandSet_HardwareColourOrderTypeByStr(const char* value, uint8_t segment_index = 0);
    void CommandSet_ColourTypeByStr(const char* value, uint8_t segment_index = 0);
    const char* GetHardwareColourTypeName(char* buffer, uint8_t buflen, uint8_t segment_index= 0);
    const char* GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen, uint8_t segment_index= 0);

    void CommandSet_Animation_Transition_Time_Ms(uint16_t value, uint8_t segment_index= 0);
    void CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index= 0);

    void CommandSet_TransitionOrderID(uint8_t value, uint8_t segment_index = 0);
    const char* GetTransitionOrderName(char* buffer, uint8_t buflen, uint8_t segment_index = 0);
    const char* GetTransitionOrderNameByID(uint8_t id, char* buffer, uint8_t buflen);
    int8_t GetTransitionOrderIDbyName(const char* c);
    
    void CommandSet_LightsCountToUpdateAsNumber(uint16_t value, uint8_t segment_index = 0);
    void CommandSet_LightsCountToUpdateAsPercentage(uint8_t value, uint8_t segment_index = 0);
    uint16_t GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage, uint8_t segment_index = 0);
    uint8_t  GetPixelsToUpdateAsPercentageFromNumber(uint16_t number, uint8_t segment_index = 0);

    /**
     * rgbcctcontroller commands
     */
    void CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(uint8_t palette_id, uint8_t segment_index = 0);
    void CommandSet_ActiveSolidPalette_Hue_360(uint16_t hue_new, uint8_t segment_index = 0);

    // void CommandSet_PixelHardwareTypeID(uint8_t value);
    // void CommandSet_LightPowerState(uint8_t value);
    // void CommandSet_ActiveSolidPalette_RGB_Ctr(const char* rgb);
    void CommandSet_ActiveSolidPalette_Sat_255(uint8_t sat_new, uint8_t segment_index = 0);
    // void CommandSet_Brt_255(uint8_t value);
    void CommandSet_BrtRGB_255(uint8_t bri, uint8_t segment_index = 0);
    void CommandSet_BrtCT_255(uint8_t bri, uint8_t segment_index = 0);
    void CommandSet_ActiveSolidPalette_ColourTemp(uint16_t ct, uint8_t segment_index = 0);

    void CommandSet_RgbcctController_SubType(uint8_t subtype, uint8_t segment_index = 0);

    // void CommandSet_ActiveSolidPalette_Hue_360(uint16_t value);
    // void CommandSet_ActiveSolidPalette_Sat_255(uint8_t value);
    // void CommandSet_ActiveSolidPalette_ColourTemp(uint16_t ct);
    // bool CommandSet_ActiveSolidPalette_RGBCT_Linked(uint16_t ct_rgb_linked);
    // void CommandSet_ActiveSolidPalette_Raw(uint8_t* values);
    // void CommandSet_ActiveSolidPalette_Raw(uint8_t r,uint8_t g,uint8_t b,uint8_t ww,uint8_t wc);
    // void CommandSet_ActiveSolidPalette_ColourTemp_Percentage(uint8_t percentage);
    // void CommandSet_Auto_Time_Off_Secs(uint16_t value);
    // void CommandSet_PaletteID(uint8_t value);
    // void CommandSet_AnimationModeID(uint8_t value);


    /***************
     * END
     * 
     * Command List 
     * 
     * *********************/


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

    void colorFromDecOrHexString(byte* rgb, char* in);
    void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)

    float minf2(float v, float w);
    float maxf2(float v, float w);

#endif // USE_MODULE_LIGHTS_ANIMATOR

#endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

#endif // Guard
