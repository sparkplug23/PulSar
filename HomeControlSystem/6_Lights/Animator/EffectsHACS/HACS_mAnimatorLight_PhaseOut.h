#ifndef _AnimatorDALIGHTNEOPIXEL7_segH
#define _AnimatorDALIGHTNEOPIXEL7_segH 7.0

#include "1_TaskerManager/mTaskerManager.h"


/*************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   HACS (Legacy)   ***************************************************************************************************************************************************************************
**  @note:    HACS animation, to be phased out into Segments **************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/

    #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


void Init_HACS(void);
void Init_WLED();

    void ApplyBrightnesstoDesiredColour(uint8_t brightness);
    void init_flasher_settings();
    uint16_t SetLEDOutAmountByPercentage(uint8_t percentage);
    void ShowStatusViaPixels(uint8_t mode);      
    
    ANIM_FUNCTION_SIGNATURE;
    mAnimatorLight& setAnimFunctionCallback(ANIM_FUNCTION_SIGNATURE);


    void RotateDesiredColour(uint8_t pixels, uint8_t direction = 0);
    void UpdateStartingColourWithGetPixel();
    void UpdateDesiredColourWithGetPixel();

    void CommandSet_Flasher_FunctionID(uint8_t value);
    int8_t GetFlasherFunctionIDbyName(const char* f);
    const char* GetFlasherFunctionName(char* buffer, uint8_t buflen);
    const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen);
    void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value);
    void CommandSet_Flasher_Flags_Movement_Direction(uint8_t value);
    void CommandSet_Flasher_Alternate_Brightness_Min(uint8_t value);
    void CommandSet_Flasher_Alternate_Brightness_Max(uint8_t value);
    void CommandSet_Flasher_Alternate_RandomAmountPercentage(uint8_t value);
    void CommandSet_Flasher_Flags_ApplySaturationRandomnessOnPaletteColours(uint8_t value);
    void CommandSet_Brightness_Min(uint8_t value);
    void CommandSet_Brightness_Max(uint8_t value);    
    void CommandSet_Palette_Generation_Randomise_Brightness_Mode(uint8_t value);
    
    void CommandSet_EffectsModeID(uint8_t mode, uint8_t segment = 0);
    const char* GetEffectsModeNamebyID(uint8_t id, char* buffer, uint8_t buflen);

    void CommandSet_BrightnessRGB_Limits_Lower(uint8_t value);
    void CommandSet_BrightnessRGB_Limits_Upper(uint8_t value);
      

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

// this will likely just use candle effect, maybe forced colour palette?/

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

  //https://www.youtube.com/watch?v=D2IBGlDJ3lg @2.50


  // Make generate function with lots of mapped values, with percentage 0 to 255 to retrieve it

  void SubTask_Effects_PhaseOut();
  void SubTask_Flasher_Animate();
  void SubTask_Flasher_Animate_Mixer();    

  void AnimationProcess_Generic_RGBCCT_Single_Colour_All(const AnimationParam& param);
    

  struct AnimationColours_SOLID_COLOUR
  {
    RgbcctColor StartingColor;
    RgbcctColor DesiredColour;
  };
  AnimationColours_SOLID_COLOUR animation_colours_rgbcct;


  /**
   * This will be moved into wled segments runtime. 
   * Its a byte buffer, that holds the animation settings of the active animation
   * This is performed by segment_runtime allocate later
   * */
  // struct HACS_TEMPORARY_SEGMENT_RUNTIME_DATA_BUFFER
  // uint8_t hacs_animation_runtime_data_buffer[100] = {0};


      void UpdateDesiredColourFromPaletteSelected(void);
      void AnimationProcess_Generic_AnimationColour_LinearBlend(const AnimationParam& param);
      
      void RefreshLEDIndexPattern();
      uint32_t tSavedCheckAnyLEDsOnOff = 0;
        
      // totally random, or random but not repeating
      enum TRANSITION_ORDER_RANDOM_METHOD_IDS{
        TRANSITION_ORDER_RANDOM_METHOD_REPEATING = 0,
        TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE,
        TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING
      };
      uint8_t transition_order_random_type = TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE;
   

      void TurnLEDsOff();
  
  enum PALETTE_PATTERN_IDS{ //TRANSITION_ORDER{
    TRANSITION_ORDER_NONE_ID=0,
    TRANSITION_ORDER_RANDOM_ID,
    TRANSITION_ORDER_INORDER_ID,
    // TRANSITION_ORDER_CENTRE_OUT_ID,
    // TRANSITION_ORDER_GRADIENT_ID,
    TRANSITION_ORDER_LENGTH_ID
  }; 




/**
 * inline animation header includes
 * */
#include "6_Lights/Animator/EffectsHACS/Slow_Glow/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Static_Glow/Animator.h"
// #include "EffectsHACS/Fireplace_1D_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Sequential/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Pulser_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Pulser_02/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Twinkle_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Twinkle_02/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Pulse_Random_On_Fade_Off/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Popping_Palette_Brightness_From_Lower_To_Upper_Boundary/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Slow_Glow_Partial_Palette_Step_Through/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Rgbcct_Solid_Static_Colour/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Fade_Gradient_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Slow_Fade_Brightness_All/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Slow_Fade_Saturation_All/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Slow_Glow_On_Brightness/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_Gradient_Alarm_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_Gradient_Sun_Elevation_And_Azimuth_01/Animator.h"


// #include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_02/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_03/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_04/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_SolidColourElevation05/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Sun_Elevation_RGBCCT_Palette_Indexed_Positions_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_RgbcctPaletteElevationAugmented01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/SunPositions_Elevation_Only_Controlled_CCT_Temperature_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Development_Tester/Animator.h"






void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region();
void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time();
void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate();


  /**
   * This is what needs converted to permitting "segments", and thus, the eventual inclusion of WLED directly 
   * 
   * The animator will need moved into here, and hence, each segment will need its own animator! (this is probably why wled wrote its own, instead, tighten up neopixel)
   * 
   * 
   * */
  
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
      uint16_t movement_direction : 2;

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
    uint8_t region = EFFECTS_REGION_COLOUR_SELECT_ID;
    // flag needed to force completion between regions
    uint8_t flag_finish_flasher_pair = false;

    EFFECTSSETTINGS_FLAGS flags;

    struct RANDOM_TRANSITIONS{
      uint32_t rate_ms = 1000;
      uint8_t upper_secs = 20;
      uint8_t lower_secs = 1;
      uint8_t array_index = 0;
      uint8_t array_index_length = 0;
      //uint8_t array[10];   //use different array profiles, "linear ramp", "exponentialhigh", "exp low" (lingers on low)
      // uint8_t profile_id;
      uint32_t tSavedNewSpeedUpdate = millis();
    }random_transitions;
    

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
    struct TSAVED{
      uint32_t Update = millis();
      // uint32_t tSavedChange_animation_something;
      //uint32_t Update2 = millis();
    }tSaved;
    
  }flashersettings;
  
  int8_t GetFlasherRegionIDbyName(const char* c);
  const char* GetFlasherRegionName(char* buffer);
  const char* GetFlasherRegionNamebyID(uint8_t id, char* buffer); 
  enum EFFECTSREGION{EFFECTS_REGION_COLOUR_SELECT_ID=1,
                     EFFECTS_REGION_ANIMATE_ID};           

  void UpdateStartingColourWithSingleColour(RgbcctColor colour);      
  void UpdateDesiredColourWithSingleColour(RgbcctColor colour);   


  enum RANDOM_TRANSITION_TIMINGS_PROFILES_IDS{
    RANDOM_TRANSITION_TIMINGS_PROFILES_TRIANGLE_RAMP_ID=0,
    RANDOM_TRANSITION_TIMINGS_PROFILES_EXPONENTIAL_HIGH_ID, //more on higher
    RANDOM_TRANSITION_TIMINGS_PROFILES_EXPONENTIAL_LOW_ID,  //More on lower
  };

  
    // #define D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH 20
    // // uint8_t mapped_array_data_array[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    // uint8_t mapped_array_data_array[D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH];
    //  = {
    //   20,19, //39
    //   15,16, //70
    //   13,9, //92
    //   7,1  //100
    // };
    

  
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

  

  struct SHARED_EFFECTS_PARAMETERS{
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

    struct INDEX_COUNTERS{
      uint16_t lower_limit = 0;
      uint16_t upper_limit = 0; 
      uint16_t active = 0;
      uint8_t counter = 0;


    }indexes;


  }shared_flasher_parameters;
  

#endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


#endif

// #endif
