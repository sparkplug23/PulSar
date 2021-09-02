#ifndef _AnimatorDALIGHTNEOPIXEL_H
#define _AnimatorDALIGHTNEOPIXEL_H 7.0

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID 141

#ifdef USE_MODULE_LIGHTS_ANIMATOR


#include "6_Lights/_Interface/mInterfaceLight.h"

#ifdef ESP32
#include <WiFi.h>
#ifndef DISABLE_NETWORK
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif // DISABLE_NETWORK
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif

#include "math.h"
#include "fastmath.h"

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "6_Lights/Palette/mPalette_Progmem.h"
// #include "2_CoreSystem/mBaseConfig.h"
#include "mAnimatorLight_web.h"
// #include "6_Lights/_Interface/mInterfaceLight.h"

// #include "6_Lights/mAnimatorLight_WLED_Shared_Index.h"

// // #include "EffectsWLED/Animations.h"
// #include "6_Lights/Animator/EffectsWLED/Animations.h"

#include "6_Lights/Animator/EffectsWLED/Animations.h"

// #define FASTLED_INTERNAL //remove annoying pragma messages
// #include "../Animator2_WLEDEffects/FastLED_Modified/FastLED.h"

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

// Default enables, unless I add disable versions
#ifndef DISABLE_PIXEL_FUNCTION_EFFECTS 
  #define ENABLE_PIXEL_FUNCTION_EFFECTS
#endif // DISABLE_PIXEL_FUNCTION_EFFECTS
#define ENABLE_PIXEL_FUNCTION_PIXELGROUPING

// #define ENABLE_PIXEL_FUNCTION_EFFECTS
#define ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL


#if   defined(USE_WS28XX_FEATURE_3_PIXEL_TYPE)
  typedef RgbColor RgbTypeColor;
#elif defined(USE_WS28XX_FEATURE_4_PIXEL_TYPE)
  typedef RgbwColor RgbTypeColor;
#else
  typedef RgbColor RgbTypeColor;
#endif

#if   defined(USE_WS28XX_FEATURE_3_PIXEL_TYPE)
  typedef NeoRgbFeature selectedNeoFeatureType;
#elif defined(USE_WS28XX_FEATURE_4_PIXEL_TYPE)   //includes 4th channel of white, pixel order variable still important
  // typedef NeoGrbwFeature selectedNeoFeatureType; 
  typedef NeoRgbwFeature selectedNeoFeatureType;
#else
  typedef NeoRgbFeature selectedNeoFeatureType;
#endif

  enum TIME_MAP_SECS_IDS{
    TIME_MAP_SECS_0_ID = 0,
    TIME_MAP_SECS_1_ID,
    TIME_MAP_SECS_2_ID,
    TIME_MAP_SECS_4_ID,
    TIME_MAP_SECS_6_ID,
    TIME_MAP_SECS_10_ID,
    TIME_MAP_SECS_15_ID,
    TIME_MAP_SECS_20_ID,
    TIME_MAP_SECS_30_ID,
    TIME_MAP_SECS_60_ID,
    TIME_MAP_SECS_LENGTH_ID
  };
  const uint8_t time_map_secs[] PROGMEM = {0,1,2,4,6,10,15,20,30,60};

  enum RATE_MAP_SECS_IDS{
    RATE_MAP_SECS_0_ID = 0,
    RATE_MAP_SECS_1_ID,
    RATE_MAP_SECS_2_ID,
    RATE_MAP_SECS_4_ID,
    RATE_MAP_SECS_6_ID,
    RATE_MAP_SECS_10_ID,
    RATE_MAP_SECS_15_ID,
    RATE_MAP_SECS_20_ID,
    RATE_MAP_SECS_30_ID,
    RATE_MAP_SECS_60_ID,
    RATE_MAP_SECS_LENGTH_ID
  };
  const uint8_t rate_map_secs[] PROGMEM = {0,1,2,4,6,10,15,20,30,60};

  enum PIXELS_UPDATE_PERCENTAGE_IDS{
    PIXELS_UPDATE_PERCENTAGE_0_ID = 0,
    PIXELS_UPDATE_PERCENTAGE_5_ID,
    PIXELS_UPDATE_PERCENTAGE_10_ID,
    PIXELS_UPDATE_PERCENTAGE_15_ID,
    PIXELS_UPDATE_PERCENTAGE_20_ID,
    PIXELS_UPDATE_PERCENTAGE_30_ID,
    PIXELS_UPDATE_PERCENTAGE_40_ID,
    PIXELS_UPDATE_PERCENTAGE_50_ID,
    PIXELS_UPDATE_PERCENTAGE_60_ID,
    PIXELS_UPDATE_PERCENTAGE_70_ID,
    PIXELS_UPDATE_PERCENTAGE_80_ID,
    PIXELS_UPDATE_PERCENTAGE_90_ID,
    PIXELS_UPDATE_PERCENTAGE_100_ID,
    PIXELS_UPDATE_PERCENTAGE_LENGTH_ID,
  };
  const uint8_t pixels_to_update_as_percentage_map[] PROGMEM =
    {0,5,10,15,20,30,40,50,60,70,80,90,100};

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_AMBILIGHT_CTR)     "ambilight";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_ANIMATION_CTR)     "animation";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_NOTIFICATIONS_CTR) "notifications";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_EFFECTS_CTR)       "flasher";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MIXER_CTR)         "mixer";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETPIXEL_MANUALLY_CTR) "setpixel_manually";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_ANIMATIONS_PROGRESS_CTR)         "debug/animation_details";


    
    
//     #define D_NOTIF_MODE_OFF_NAME_CTR           "OFF"
//     #define D_NOTIF_MODE_STATIC_OFF_NAME_CTR    "Static OFF"
//     #define D_NOTIF_MODE_STATIC_ON_NAME_CTR     "Static ON"
//     #define D_NOTIF_MODE_BLINKING_OFF_NAME_CTR  "Blinking ON"
//     #define D_NOTIF_MODE_BLINKING_ON_NAME_CTR   "Blinking OFF"
//     #define D_NOTIF_MODE_PULSING_OFF_NAME_CTR   "Pulsing ON"
//     #define D_NOTIF_MODE_PULSING_ON_NAME_CTR    "Pulsing OFF"

#define D_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR "Colour Select"
#define D_EFFECTS_REGION_ANIMATE_NAME_CTR "Animate"


/**
 * Effects Function names (Giving multiple names using delimeter)
 * */
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR)                        "Slow Glow|Blend";    
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_STATIC_GLOW_NAME_CTR)                      "Static Glow|InOrder";     
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SEQUENTIAL_NAME_CTR)                       "Sequential"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR)                     D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR;//"Solid RGBCCT"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_NAME_CTR)         "SLOW_FADE_BRIGHTNESS_ALL";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_NAME_CTR)         "SLOW_FADE_SATURATION_ALL";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_NAME_CTR)      "SLOW_FADE_BRIGHTNESS_RANDOM";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SLOW_FADE_SATURATION_RANDOM_NAME_CTR)      "SLOW_FADE_SATURATION_RANDOM";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_FLASH_TWINKLE_RANDOM_NAME_CTR)             "FLASH_TWINKLE_RANDOM";                   
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_NAME_CTR)         "FLASH_TWINKLE_SEQUENTIAL";                          
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_FLASH_GLIMMER_RANDOM_NAME_CTR)             "FLASH_GLIMMER_RANDOM"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR)                  D_EFFECTS_FUNCTION_FIREPLACE_1D_01_NAME_CTR;//   "FirePlace01";     
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_TESTER_NAME_CTR)                           "Tester";


DEFINE_PGM_CTR(PM_EFFECTS_REGION_SLOW_FADE_NAME_CTR)                          "SLOW_FADE";
DEFINE_PGM_CTR(PM_EFFECTS_REGION_TWINKLE_FLASH_NAME_CTR)                      "TWINKLE_FLASH";
DEFINE_PGM_CTR(PM_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR)                      D_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_REGION_ANIMATE_NAME_CTR)                            D_EFFECTS_REGION_ANIMATE_NAME_CTR;




DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR) D_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR) D_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR) D_EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_NAME_CTR;

// #include "6_Lights/Animator2_WLEDEffects/EffectsWLED/Animations.h"


#include <functional>
// #define ANIM_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
// #define ANIM_CALLBACK_SIGNATURE std::function<void(const AnimationParam& param)> callback
#define ANIM_FUNCTION_SIGNATURE std::function<void(const AnimationParam& param)> anim_function_callback
// #define ANIM_FUNCTION_SIGNATURE std::function<void(const AnimationParam& param)> anim_function_callback
#define ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE std::function<void()> anim_progress_mqtt_function_callback

class mAnimatorLight :
  public mTaskerInterface
{
  private:
  public:
    mAnimatorLight(){};
    void Pre_Init(void);
    void init(void);
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mAnimatorLight);
    };
    #endif
    
    static const char* PM_MODULE_LIGHTS_ANIMATOR_CTR;
    static const char* PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_ANIMATOR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID; }

    
    // led fx library object
    // WS2812FX* mEffects = nullptr;


    // Put settings at top of class from now on, use it for common settings
    struct SETTINGS{
      struct FLAGS{
          uint8_t EnableSceneModeWithSliders = true;
          uint8_t TemplateProvidedInProgMem = false;
          uint8_t EnableModule = false;
      }flags;
    }settings;
  
    void SetPixelColor(uint16_t indexPixel, RgbcctColor color);
    RgbcctColor GetPixelColor(uint16_t indexPixel = 0);
    
    #if defined(ESP8266)
      #if defined(USE_WS28XX_METHOD_800KBPS)
        typedef Neo800KbpsMethod selectedNeoSpeedType;
      #elif defined(USE_WS28XX_METHOD_BITBANG)
        typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
      #elif defined(USE_WS28XX_METHOD_UART800KBPS)
        typedef NeoEsp8266Uart800KbpsMethod selectedNeoSpeedType;   
      #elif defined(USE_SK6812_METHOD_DEFAULT)
        typedef NeoSk6812Method selectedNeoSpeedType;   
      #else
        typedef Neo800KbpsMethod selectedNeoSpeedType; // default
      #endif  
    #elif defined(ESP32)
      #if defined(USE_WS28XX_METHOD_I2S1_800KBPS_ESP32)
        typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
      #elif defined(USE_WS28XX_METHOD_RMT0_800KBPS_ESP32)
        typedef NeoEsp32Rmt0800KbpsMethod selectedNeoSpeedType; 
      #else          
        // typedef NeoEsp32Rmt7800KbpsMethod selectedNeoSpeedType; // 1st
        
        typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType; //esp32 workss

        // typedef Neo800KbpsMethod selectedNeoSpeedType;
        // typedef NeoEsp32Rmt7Ws2812xMethod selectedNeoSpeedType; //test A

        // typedef NeoEsp32Rmt7800KbpsMethod selectedNeoSpeedType;
        //  typedef NeoEsp32I2s1800KbpsMethod selectedNeoSpeedType;
        // typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
      #endif
    #endif   
    NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;
    
    


  
    // Group of ifndef's to allow defaults to be set, and users to set defaults using basic numbers
    #ifdef STRIP_SIZE_MAX
      // #ifndef STRIP_REPEAT_OUTPUT_MAX
      //   #define STRIP_REPEAT_OUTPUT_MAX STRIP_SIZE_MAX
      // #endif
    #else
      #define STRIP_SIZE_MAX 50 //default length of 50
    #endif
    
    // #ifndef STRIP_SIZE_REPEAT_MAX
    //   #define STRIP_SIZE_REPEAT_MAX 50 //default length of 50
    // #endif
    #ifndef STRIP_OUTPUT_REPEATED_LENGTH
      #define STRIP_OUTPUT_REPEATED_LENGTH 20
    #endif
    // #ifndef STRIP_REPEAT_OUTPUT_MAX
    //   #define STRIP_REPEAT_OUTPUT_MAX 200
    // #endif
    #ifndef ANIMATOR_SIZE_MAX
      #ifdef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
        #define ANIMATOR_SIZE_MAX 1//STRIP_SIZE_MAX
      #else
        #define ANIMATOR_SIZE_MAX STRIP_SIZE_MAX
      #endif
    #endif

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    #define PRESET_COLOUR_MAP_INDEXES_MAX COLOUR_MAP_LENGTH_ID 
    // uint16_t settings.light_size_count = STRIP_SIZE_MAX; //allow variable control of size
    uint16_t strip_size_requiring_update = STRIP_SIZE_MAX;
    uint16_t animator_strip_size = ANIMATOR_SIZE_MAX; //allow variable control of size

    // what is stored for state is specific to the need, in this case, the colors.
    // Basically what ever you need inside the animation update function
    struct AnimationColours
    {
      RgbTypeColor StartingColor;
      RgbTypeColor DesiredColour;
    };
    AnimationColours animation_colours[STRIP_SIZE_MAX];

    void BlendAnimUpdate(const AnimationParam& param);
    void AnimUpdate_ShowStartingThenDesiredColors(const AnimationParam& param);

    void StartAnimation_AsAnimUpdateMemberFunction();
    void AnimUpdateMemberFunction(const AnimationParam& param);
    void AnimationProcess_TraditionalChasing(const AnimationParam& param);
    void AnimationProcess_TraditionalChasing2(const AnimationParam& param);
    void AnimationProcess_Generic_AnimationColour_LinearBlend(const AnimationParam& param);
    
    void LoadMixerGroupByID(uint8_t id);

    
    void Draw_DesiredColour_LinearGradientMirrored(RgbcctColor colour_center, 
                                      RgbcctColor colour_circumference, 
                                      uint16_t radius_pixel,
                                      uint16_t radius_size,
                                      uint8_t center_repeat_size = 0,
                                      bool colour_is_additive = false,
                                      bool values_are_percentage_of_strip_size = false
    );
    void Remove_DesiredColour_LinearGradientMirrored(RgbcctColor colour_center, 
                                      RgbcctColor colour_circumference, 
                                      uint16_t radius_pixel,
                                      uint16_t radius_size,
                                      uint8_t center_repeat_size = 0,
                                      bool colour_is_additive = false,
                                      bool values_are_percentage_of_strip_size = false
    );
    
    void Draw_DesiredColour_LinearGradientMirrored2(RgbcctColor colour_center, 
                                      RgbcctColor colour_circumference, 
                                      uint16_t radius_pixel,
                                      uint16_t radius_size,
                                      uint8_t center_repeat_size = 0,
                                      bool colour_is_additive = false,
                                      bool values_are_percentage_of_strip_size = false
    );
    
    void DesiredColourWrite_Safe(RgbcctColor colour, uint16_t index);

    uint8_t counter_test = 0;
    uint8_t test_flag = false;
    // void (mAnimatorLight::*AnimationProcess_Ptr)(const AnimationParam& param) = nullptr; // member-function to sender with one args

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
  


    // void (mAnimatorLight::*AnimationProcess_Ptr2)() = nullptr; // member-function to sender with one args

    ANIM_FUNCTION_SIGNATURE;
    mAnimatorLight& setAnimFunctionCallback(ANIM_FUNCTION_SIGNATURE);

    bool OverwriteUpdateDesiredColourIfMultiplierIsEnabled();

    #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
    // Move completely into its own class, with its own tasker
    /**************
     * Ambilight is light patterns around screens or pictures
     * PRESETS - patterns
     * INPUT_STREAM - set leds directly through mqtt (wallpaper) or serial lighting
    **************/ 
    enum AMBILIGHT_MODES_IDS{
      AMBILIGHT_PRESETS_ID=0,
      AMBILIGHT_SIDES_ID,
      AMBILIGHT_INPUT_STREAM_ID,
      AMBILIGHT_LENGTH_ID
    };
    int8_t GetAmbilightModeIDbyName(const char* c);
    const char* GetAmbilightModeName(char* buffer);

    /*******AMBILIGHT*********************************************************************************************/

    void SubTask_Ambilight_Main();
    void Ambilight_Sides();
    void Ambilight_Presets();
    void Ambilight_InputStream();
    void init_Ambilight();
    #define AMBILIGHT_SCREENS_CONNECTED 1

    /**
     * 
     * Long term, multiple screens will be used as their own subclass, containing all the info about that screen instead of struct arrays
     * */
    enum AMBILIGHT_SCREEN_INDEX{SCREEN_CENTRE=0,SCREEN_LEFT=1,SCREEN_RIGHT=2};



    struct AMBILIGHT_SCREEN_SETTINGS{
      uint8_t ambilight_mode=AMBILIGHT_SIDES_ID;
      uint32_t tSavedUpdate = millis();
      uint32_t ratemsSavedUpdate = 1000;
      uint8_t fUpdate = false;

      struct SCREENS{
        struct EDGE{
          RgbcctColor colour;
          int8_t blend_between_sides_gradient_percentage = -1; // -1 is unset/none/solid, 0% is bottom/left, 100% is top/right
          uint8_t size = 5;
        };
        struct EDGE top;
        struct EDGE bottom;
        struct EDGE left;
        struct EDGE right;
        struct FIRST_PIXEL{
          //uint8_t bottom_right_clockwise = store as packed bit? 
        }start_pixel;
      }screens[AMBILIGHT_SCREENS_CONNECTED]; //0,1,2 where 0 is centre and only screen

    }ambilightsettings;
    
    #endif

    /*******USER LIGHTS & EFFECTS**
     * 
  *******************************************************************************************/

    void RotateDesiredColour(uint8_t pixels, uint8_t direction = 0);
    void UpdateStartingColourWithGetPixel();
    void     UpdateDesiredColourWithGetPixel();
    


  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  const char* GetFlasherFunctionName(char* buffer, uint8_t buflen);
  const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen);
  int8_t      GetFlasherFunctionIDbyName(const char* c);

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
     * Desc: Random change of colours (pixel order is random)
     * Parameters: Palette, time to blend, rate of new colours, percentage of new colours changed
     * */
    EFFECTS_FUNCTION_SLOW_GLOW_ID,
    /**
     * Desc: Using EFFECTS_FUNCTION_SLOW_GLOW_ID, with pixel order set to "inorder"
     * Parameters: Palette, time to blend, rate of new colours, percentage of new colours changed
     * */
    EFFECTS_FUNCTION_STATIC_GLOW_ID,
    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * */
    EFFECTS_FUNCTION_SEQUENTIAL_ID,
    /**
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * */
    EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID,






    // Apply sinewave to brightness, 25% increasing, 50% decreasing, 25% off
    EFFECTS_FUNCTION_PULSE_SINE_WAVE_BRIGHTNESS, //pul
    // The one when applybrightness was broke
    // done by applying new cl/brightness to some, then each call reducing the colour crushing it
    //do with percent to show on at one time
    EFFECTS_FUNCTION_PULSE_RANDOM_ON, 
    EFFECTS_FUNCTION_PULSE_RANDOM_ON_TWO_ID,
    /**
     * half all pixels brightness on each refresh, random on to full brightness
     * */
    EFFECTS_FUNCTION_PULSE_RANDOM_ON_FADE_OFF_ID,     
    /**
     * Palette is first drawn with certain lower brightness level, then
     * additional changes takes those colours and bumbs their brightness up to desired brightness
     * Few change, random pixels bumped up
     * Check if their brightness is <upper, before asserting them brighter (or convert to hsbcolour, flick up brightness if needed then change back to rgb)
     * Goes from low to high then stops
     * */
    EFFECTS_FUNCTION_POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY,
    /**
     * 0/100% animaiton progress is low brightness, 50% flips to upper brightness
    */
    EFFECTS_FUNCTION_TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK,
    // EFFECTS_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID, // sequential flash of white on x leds 
    EFFECTS_FUNCTION_FADE_GRADIENT_ID, //single pixels: static, twinkle, pick from palette
    //another flash to "off" or simple set flash colour to off??
    EFFECTS_FUNCTION_FLASH_GLIMMER_RANDOM_ID, // tinkle=leds flash independant, glimmer=leds flash with dimming effect on others
    /**
     * Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     * */
    // EFFECTS_FUNCTION_SHOWING_MULTIPLES_OF_COLOURS_ID,
    // Instead of just applying new colours, also randomise the brightness levels between range
    EFFECTS_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS_ID,
    /**
     * Sun positional effects
     */
    //linear palette group colour, changing by triggered and over a period of time (eg an alarm -30 minutes)
    EFFECTS_FUNCTION_SUNPOSITIONS_SUNRISE_ALARM_01, //group01? bring all together, have settings to configure how the effect runs
    //linear palettes group colour, enabled by a time window, but sets linear gradient by sun azimuth
    EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_01, //this is elevation only
    //enabled by a time window, GENERATE manually, brightness by sun elevaltion, sun position from left to right by azimuth
    //far left = east from position, and far right = wast from position using sunset position
    EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01,
    EFFECTS_FUNCTION_SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_2D_01, // If matrix/grid is connected, enable drawing this
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
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_01_ID,
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_02_ID,
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_03_ID, // Using stored rgbcct palette
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_04_ID, // Using stored rgbcct palette
    EFFECTS_FUNCTION_SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_05_ID, // CCT only, mapped directly, no palette

    /**
     * Functional methods of sunrise
     * EFFECTS_FUNCTION_SUNPOSITIONS_<SOLIDCOLOUR/2D_ARRAY>
     * */
    /**
     * Eleveation controls that use rgbcct palette (that has elevation in its index byte) to display solid colour only
     * */
    EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01_ID,
    EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01_ID,

    //create a palette to leave on ALL the time, ie deep red at night instead of blue... testing for the clocks later I guess

    /**
     * Elevation controls the CCT channels only
     **/
    EFFECTS_FUNCTION_SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01_ID,


    // palette to step through, which gives single colour sun
    EFFECTS_FUNCTION_SUNPOSITIONS_STEP_RGBCCT_ALARM_01,
    //enabled CCT by azimuth 
    EFFECTS_FUNCTION_SUNPOSITIONS_STEP_RGBCCT_SUN_ELEVATION_01,
    /**
     * Not currently working
     * */
    EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID, // change ALL, 0 - 100%
    EFFECTS_FUNCTION_SLOW_FADE_SATURATION_ALL_ID, // change ALL, 0 - 100%
    EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_ID, // change ALL, 0 - 100%
    EFFECTS_FUNCTION_SLOW_FADE_SATURATION_RANDOM_ID, // change ALL, 0 - 100%
    EFFECTS_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID, //random leds flash to 100% brightness (modes=instant on/off, multiple pulses)
    EFFECTS_FUNCTION_FLASH_TWINKLE_PALETTE_COLOUR_RANDOM_ID,   
    EFFECTS_FUNCTION_FIREPLACE_1D_01_ID, // solid colours, 1 100%, moving from previous to next
    /**
     * Clock animations for 3d printed display
     **/
    EFFECTS_FUNCTION_LCD_CLOCK_BASIC_01_ID,
    EFFECTS_FUNCTION_LCD_CLOCK_BASIC_02_ID,
    EFFECTS_FUNCTION_LCD_DISPLAY_BASIC_01_ID, // show number commanded via mqtt

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
//   // WLED functions I am merging, will use the 'ID' to select which animator to use, as I migrate between animator methods        
//   enum EFFECTS_WLED_FUNCTION_IDS{
//     EFFECTS_WLED_FUNCTION_01_ID = EFFECTS_FUNCTION_LENGTH_ID,
// // Static
//   FX_MODE_STATIC=0,
//   FX_MODE_STATIC_PATTERN,
//   FX_MODE_TRI_STATIC_PATTERN,
//   FX_MODE_SPOTS,
//   FX_MODE_PERCENT,
//   // One colour changes
//   FX_MODE_RANDOM_COLOR,
//   // Wipe/Sweep/Runners 
//   FX_MODE_COLOR_WIPE, 
//   FX_MODE_COLOR_WIPE_RANDOM,
//   FX_MODE_COLOR_SWEEP,
//   FX_MODE_COLOR_SWEEP_RANDOM,
//   FX_MODE_TRICOLOR_WIPE,
//   FX_MODE_ANDROID,
//   FX_MODE_RUNNING_RED_BLUE,
//   FX_MODE_RUNNING_COLOR,
//   FX_MODE_RUNNING_RANDOM,
//   FX_MODE_GRADIENT,
//   FX_MODE_LOADING,
//   FX_MODE_POLICE,
//   FX_MODE_POLICE_ALL,
//   FX_MODE_TWO_DOTS,
//   FX_MODE_TWO_AREAS,
//   FX_MODE_MULTI_COMET,
//   FX_MODE_OSCILLATE,
//   FX_MODE_BPM,
//   FX_MODE_JUGGLE,
//   FX_MODE_PALETTE,
//   FX_MODE_COLORWAVES,
//   FX_MODE_LAKE,
//   FX_MODE_GLITTER,
//   FX_MODE_METEOR,
//   FX_MODE_METEOR_SMOOTH,
//   FX_MODE_PRIDE_2015,
//   FX_MODE_RIPPLE_RAINBOW,
//   FX_MODE_PACIFICA,
//   FX_MODE_SUNRISE,
//   FX_MODE_SINEWAVE,
//   FX_MODE_FLOW,
//   FX_MODE_PHASEDNOISE,
//   FX_MODE_PHASED,
//   FX_MODE_RUNNING_LIGHTS,
//   FX_MODE_RAINBOW_CYCLE,
//   FX_MODE_MERRY_CHRISTMAS,
//   FX_MODE_HALLOWEEN,
//   // Chase
//   FX_MODE_CHASE_COLOR,
//   FX_MODE_CHASE_RANDOM,
//   FX_MODE_CHASE_RAINBOW, 
//   FX_MODE_CHASE_FLASH,
//   FX_MODE_CHASE_FLASH_RANDOM, 
//   FX_MODE_CHASE_RAINBOW_WHITE,
//   FX_MODE_THEATER_CHASE,
//   FX_MODE_THEATER_CHASE_RAINBOW,
//   FX_MODE_TRICOLOR_CHASE,
//   FX_MODE_RANDOM_CHASE,
//   FX_MODE_CIRCUS_COMBUSTUS,
//   // Breathe/Fade/Pulse
//   FX_MODE_BREATH,
//   FX_MODE_FADE,
//   FX_MODE_TRICOLOR_FADE,
//   FX_MODE_SPOTS_FADE,
//   // Fireworks
//   FX_MODE_FIREWORKS,
//   FX_MODE_STARBURST,
//   FX_MODE_EXPLODING_FIREWORKS,
//   FX_MODE_RAIN,
//   // Sparkle/Twinkle
//   FX_MODE_SOLID_GLITTER,
//   FX_MODE_POPCORN,
//   FX_MODE_PLASMA,
//   FX_MODE_FIRE_FLICKER,
//   FX_MODE_SPARKLE,
//   FX_MODE_FLASH_SPARKLE,
//   FX_MODE_HYPER_SPARKLE,
//   FX_MODE_TWINKLE,
//   FX_MODE_COLORTWINKLE,
//   FX_MODE_TWINKLEFOX,
//   FX_MODE_TWINKLECAT,
//   FX_MODE_TWINKLEUP,
//   FX_MODE_DYNAMIC,
//   FX_MODE_SAW,
//   FX_MODE_DISSOLVE,
//   FX_MODE_DISSOLVE_RANDOM,
//   FX_MODE_COLORFUL,
//   FX_MODE_TRAFFIC_LIGHT,
//   FX_MODE_CANDLE,
//   FX_MODE_CANDLE_MULTI,
//   FX_MODE_HALLOWEEN_EYES,
//   #ifdef ENABLE_ADVANCED_EFFECTS  
//   // Blink/Strobe
//   FX_MODE_BLINK,
//   FX_MODE_BLINK_RAINBOW,
//   FX_MODE_STROBE,
//   FX_MODE_MULTI_STROBE,
//   FX_MODE_STROBE_RAINBOW, 
//   FX_MODE_RAINBOW,
//   FX_MODE_LIGHTNING,     
//   FX_MODE_FIRE_2012,
//   FX_MODE_RAILWAY,
//   FX_MODE_HEARTBEAT, 
//   // Noise
//   FX_MODE_FILLNOISE8,
//   FX_MODE_NOISE16_1,
//   FX_MODE_NOISE16_2,
//   FX_MODE_NOISE16_3,
//   FX_MODE_NOISE16_4,
//   FX_MODE_NOISEPAL,
//   // Scan
//   FX_MODE_SCAN,
//   FX_MODE_DUAL_SCAN,
//   FX_MODE_LARSON_SCANNER,
//   FX_MODE_DUAL_LARSON_SCANNER,
//   FX_MODE_ICU,
//   FX_MODE_RIPPLE,
//   FX_MODE_COMET,
//   FX_MODE_CHUNCHUN,
//   FX_MODE_BOUNCINGBALLS,
//   FX_MODE_SINELON,
//   FX_MODE_SINELON_DUAL,
//   FX_MODE_SINELON_RAINBOW,
//   FX_MODE_DRIP,      
//   #endif // ENABLE_ADVANCED_EFFECTS   
//   // Length of Effects
//   MODE_COUNT,

//     EFFECTS_WLED_FUNCTION_LENGTH_ID,
//   };

  //https://www.youtube.com/watch?v=D2IBGlDJ3lg @2.50

  int8_t GetFlasherRegionIDbyName(const char* c);
  const char* GetFlasherRegionName(char* buffer);
  const char* GetFlasherRegionNamebyID(uint8_t id, char* buffer); 
  enum EFFECTSREGION{EFFECTS_REGION_COLOUR_SELECT_ID=1,
                     EFFECTS_REGION_ANIMATE_ID};           

  void UpdateStartingColourWithSingleColour(RgbcctColor colour);      
  void UpdateDesiredColourWithSingleColour(RgbcctColor colour);   

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

  enum RANDOM_TRANSITION_TIMINGS_PROFILES_IDS{
    RANDOM_TRANSITION_TIMINGS_PROFILES_TRIANGLE_RAMP_ID=0,
    RANDOM_TRANSITION_TIMINGS_PROFILES_EXPONENTIAL_HIGH_ID, //more on higher
    RANDOM_TRANSITION_TIMINGS_PROFILES_EXPONENTIAL_LOW_ID,  //More on lower
  };
  // Make generate function with lots of mapped values, with percentage 0 to 255 to retrieve it


  /**
   * This is what needs converted to permitting "segments", and thus, the eventual inclusion of WLED directly 
   * 
   * The animator will need moved into here, and hence, each segment will need its own animator! (this is probably why wled wrote its own, instead, tighten up neopixel)
   * 
   * 
   * */
  struct EFFECTSSETTINGS{
    // Flashing function
    uint8_t function = EFFECTS_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID;
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



  void SubTask_Flasher_Main();
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
 * inline animation header includes
 * */
#include "EffectsHACS/Slow_Glow/Animator.h"
#include "EffectsHACS/Static_Glow/Animator.h"
#include "EffectsHACS/Fireplace_1D_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Sequential/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Pulser_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Pulser_02/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Twinkle_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Twinkle_02/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Pulse_Random_On_Fade_Off/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Popping_Palette_Brightness_From_Lower_To_Upper_Boundary/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Slow_Glow_Partial_Palette_Step_Through/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Rgbcct_Solid_Static_Colour/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Fade_Gradient_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Slow_Fade_Brightness_All/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Slow_Fade_Saturation_All/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Slow_Glow_On_Brightness/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Gradient_Alarm_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Gradient_Sun_Elevation_And_Azimuth_01/Animator.h"


#include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_02/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_03/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_04/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Solid_Colour_Based_On_Sun_Elevation_05/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_With_Augmented_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Elevation_Only_Controlled_CCT_Temperature_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Development_Tester/Animator.h"






void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region();
void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time();
void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate();


#endif // ENABLE_PIXEL_FUNCTION_EFFECTS

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


  uint16_t setpixel_variable_index_counter = 0;


  void init_flasher_settings();

  uint16_t SetLEDOutAmountByPercentage(uint8_t percentage);
  
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  enum EFFECTS_FUNCTION_MIXER{
    EFFECTS_FUNCTION_MIXER_01_ID=0,
    EFFECTS_FUNCTION_MIXER_02_ID,
    EFFECTS_FUNCTION_MIXER_03_ID,
    EFFECTS_FUNCTION_MIXER_04_ID,
    EFFECTS_FUNCTION_MIXER_05_ID,
    EFFECTS_FUNCTION_MIXER_06_ID,
    EFFECTS_FUNCTION_MIXER_07_ID,
    EFFECTS_FUNCTION_MIXER_08_ID,
    EFFECTS_FUNCTION_MIXER_09_ID,
    EFFECTS_FUNCTION_MIXER_10_ID,
    EFFECTS_FUNCTION_MIXER_11_ID,
    EFFECTS_FUNCTION_MIXER_12_ID,
    // EFFECTS_FUNCTION_MIXER_13_ID,
    // EFFECTS_FUNCTION_MIXER_14_ID, 
    // EFFECTS_FUNCTION_MIXER_15_ID, 
    EFFECTS_FUNCTION_MIXER_LENGTH_ID
  };
  #define EFFECTS_FUNCTION_MIXER_MAX EFFECTS_FUNCTION_MIXER_LENGTH_ID
  // #define MIXER_GROUP_MAX 7

  
  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // enable animations (pause)
      uint16_t Enabled : 1;
      /**
       * 0 - None
       * 1 - Basic multiplier
       * 2 - Using mapped index array 
      */
      // uint16_t multiplier_mode_id : 4; // 2 bit : 4 levels
      // uint16_t mapped_array_editable_or_progmem : 1;

      


    };
  } MIXER_SETTINGS_FLAGS;

   typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // enable animations (pause)
      uint16_t Enabled : 1;
      /**
       * 0 - None
       * 1 - Basic multiplier
       * 2 - Using mapped index array 
      */
      // uint16_t multiplier_mode_id : 4; // 2 bit : 4 levels
      // uint16_t mapped_array_editable_or_progmem : 1;
      
      uint16_t enable_force_preset_brightness_scaler : 1; // to allow manual brightness leveling

      uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Exactly_To_Palette_Choice : 1;
      uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice : 1;



    };
  } MIXER_SETTINGS_GROUP_FLAGS;
  

  struct MIXER_SETTINGS{
   // #ifdef DEVICE_OUTSIDETREE
    struct MODE{
      uint8_t running_id = EFFECTS_FUNCTION_MIXER_05_ID;
      uint16_t time_on_secs[EFFECTS_FUNCTION_MIXER_MAX]; // stores which to use
      int16_t time_on_secs_active = 60; //signed
      // char running_friendly_name_ctr[40];
      struct TIMES{
       // uint16_t enable_skip_restricted_by_time[EFFECTS_FUNCTION_MIXER_MAX]; // if set, this mode will only run if INSIDE the "flashy" time period
        struct time_short flashing_starttime; //shortime
        struct time_short flashing_endtime;
        uint8_t skip_restricted_by_time_isactive = 0;
      }times;
    }mode; //active 
    // #endif
    
    // uint32_t tSavedMillisToChangeAt = 0;
    uint32_t tSavedTrigger = millis();
    // uint32_t tSavedRestart = millis();
    // uint8_t enabled = false;
    // uint8_t time_scaler = 6;
    // uint32_t tSavedSendData;
    uint8_t enabled_mixer_count = 0; // 10 max

    // Shared loaded values to act on
    uint8_t brightness_lower_255 = 0;
    uint8_t brightness_higher_255 = 255;

    MIXER_SETTINGS_FLAGS flags;


    //int16_t time_on_secs_active = 10; //signed
    // char running_friendly_name_ctr[100];
    uint8_t running_id = EFFECTS_FUNCTION_MIXER_01_ID;
    uint8_t run_time_duration_scaler_as_percentage = 100; // for debugging, running faster
      
    struct GROUPS_SETTINGS{
      int16_t time_on_secs_decounter = -1;
      struct time_short starttime;
      struct time_short endtime;
      uint8_t enable_skip_restricted_by_time = 0;
      uint8_t isenabled = true;

      uint8_t ifenabled_forced_brightness_level_percentage = 100;
      int8_t pixels_to_update_as_percentage = 10;//-1; // -1 disables, ie ALL

      struct TRANSITION{
        uint16_t time = 0;
        uint16_t rate = 0;
        uint8_t  time_unit_id = 0; // secs vs ms
      }transition;

      uint8_t brightness_lower_255 = 0;
      uint8_t brightness_higher_255 = 255;

      uint8_t animation_transition_order = 0;//TRANSITION_ORDER_INORDER_ID;
      uint8_t flashersettings_function = EFFECTS_FUNCTION_SLOW_GLOW_ID;
      uint16_t run_time_duration_sec = 120;
      uint8_t palette_id = 0;

      uint8_t pixel_multiplier_id = 0;//PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID;
      uint8_t pixel_multiplier_enabled = false;

      MIXER_SETTINGS_GROUP_FLAGS flags;

    }group[EFFECTS_FUNCTION_MIXER_MAX];

  }mixer;

  void init_mixer_defaults();
  #endif //ENABLE_PIXEL_FUNCTION_MIXER


  //flasher structs for each
  // struct FLASH_TWINKLE_RANDOM{
  //   uint8_t white_on_index;
  //   uint8_t white_total_index = 0;
  //   uint8_t white_pixel_amount = 0;
  //   // #ifdef ENABLE_ADVANCED_MODE_TWINKLE // creating this to reduce "white_leds_index" size
  //   //   uint16_t white_leds_index[STRIP_SIZE_MAX];
  //   // #endif
  //   //HsbColor stored_colours_index[STRIP_SIZE_MAX];///4]; // THIS DOESNT NEED TO BE THIS LONG
  //   HsbColor flash_colour;
  // }flash_twinkle_random;

// #endif

    void ShowStatusViaPixels(uint8_t mode);

    
    // void colorXYtoRGB(float x, float y, byte* rgb); // only defined if huesync disabled TODO
    // void colorRGBtoXY(byte* rgb, float* xy); // only defined if huesync disabled TODO
    void colorFromDecOrHexString(byte* rgb, char* in);
    void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)

    float minf2(float v, float w);
    float maxf2(float v, float w);

    uint8_t fPixelsUpdated = false;
    uint8_t update_all = false;
    uint8_t blocking_force_animate_to_complete = true;
    uint32_t tSavedBlocking_ForceAnimateToComplete = millis();
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    int8_t Tasker_Web(uint8_t function);


    void parse_JSONCommand(JsonParserObject obj);
    
  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
    void CommandSet_Flasher_FunctionID(uint8_t value);
    void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value);
    void CommandSet_Flasher_Flags_Movement_Direction(uint8_t value);
    void CommandSet_Flasher_Alternate_Brightness_Min(uint8_t value);
    void CommandSet_Flasher_Alternate_Brightness_Max(uint8_t value);
    void CommandSet_Flasher_Alternate_RandomAmountPercentage(uint8_t value);
    void CommandSet_Flasher_Flags_ApplySaturationRandomnessOnPaletteColours(uint8_t value);

    void CommandSet_Brightness_Min(uint8_t value);
    void CommandSet_Brightness_Max(uint8_t value);
    
  #endif // ENABLE_PIXEL_FUNCTION_EFFECTS


  #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
  void CommandSet_PixelGrouping_Flag_AgedColouring(uint8_t value);
  void CommandSet_PixelGrouping_Flag_ModeID(uint8_t value);
  void CommandSet_PixelGrouping_Flag_Multiplier(uint8_t value);
  void CommandSet_PixelGrouping_MappedMultiplierData(uint8_t* value, uint8_t length);
  #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING

  

void CommandSet_Palette_Generation_Randomise_Brightness_Mode(uint8_t value);
void CommandSet_BrightnessRGB_Limits_Lower(uint8_t value);
void CommandSet_BrightnessRGB_Limits_Upper(uint8_t value);
  


    
    void SubTask_Presets();
    


        // make number generator, random, with skewness
        // int Range0to100[20] = {0,10,20,30,40, // random(12,24)*10;
        //                 50,60,70,80,90,
        //                 91,92,93,94,95,
        //                 96,97,98,99,100};


    float rainbowpartialprogress = 0;

    void Ws2812Init();
    
    // Default values if not specified

    // void FadeToNewColour(RgbTypeColor newcolor, uint16_t _time_to_newcolour = 1000, RgbTypeColor fromcolor = RgbTypeColor(0));
    void FadeToNewColour(RgbcctColor newcolor, uint16_t _time_to_newcolour = 1000, RgbcctColor fromcolor = RgbcctColor(0));

    const char* GetAnimationStatusCtr(char* buffer, uint8_t buflen);

    // uint32_t tSavedSendData, tSavedSendData2, fSendRGBDellStatus;
    // uint8_t testvariable=0;
    // uint8_t testvariable2=0,testvariable3=0;
    // uint8_t timerenabled = false;

    int blend_count=0;
    enum BLEND_STEPS{BLEND_START=1,
                     BLEND_DELAYS,
                     BLEND_DONE
    };
    uint8_t blend_state;
    uint32_t tTimeout;

    uint32_t tSavedCheckAnyLEDsOnOff = 0;
    uint16_t fAnyLEDsOnOffCount = 0;


    struct LEDOUTPUTSETTINGS{
      uint16_t length = 0;
      uint16_t index = 0;
      uint16_t pattern[STRIP_SIZE_MAX];
    }ledout;


    // totally random, or random but not repeating
    enum TRANSITION_ORDER_RANDOM_METHOD_IDS{
      TRANSITION_ORDER_RANDOM_METHOD_REPEATING = 0,
      TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE,
      TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING
    };
    uint8_t transition_order_random_type = TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE;
    
  
  uint32_t tSavedUpdateRGBString;
  uint8_t first_set;
  // uint8_t hue_preset1_idx;
  uint16_t rotate_start_index = 0;
  uint16_t LEDp, LEDe, desired_pixel; //pixel and element
  
  // This value holds the brightness of the string and is shared by animations and scene selection.
  // When scene is set, this will also be set automatically.
  //float brightness_as_percentage = 100;

  HsbColor GetColourFromMapUsingType(
    uint16_t pixel_I_want,
    uint8_t *colour_index, uint16_t colour_array_length, uint16_t fMapIDs_Type, //inputs
    uint16_t* total_hsbcolours_in_map, uint16_t* pixel_index_out, uint16_t* pixel_width_including_indexing //returns
    );




    void WebSave_RGBColourSelector(void);

    /***
     * Possible bit level packing of enabling or disabling palette colours allowed in a palette
     * */
    // uint32_t palette_pixel_enabled_32bitpacked = 0xFFFF;

    
void ApplyBrightnesstoDesiredColour(uint8_t brightness);

RgbcctColor ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness);


RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT);
RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness);




void Settings_Load();
void Settings_Save();
void Settings_Default();
void Settings_Reset();

void StartAnimationAsBlendFromStartingColorToDesiredColor(void);
void StartAnimationAsSwitchingFromStartingColorToDesiredColor();


// void UpdateLEDs2(void);
void UpdateDesiredColourFromPaletteSelected(void);
// void parsesub_CheckAll(JsonObjectConst obj);
void TurnLEDsOff();
// void AddToJsonObject_AddHardware(JsonObject root);
void Append_Hardware_Status_Message();
// void WebSendBody_Palette();
// void WebSendBody_Liveview();
// void WebSendBody_InfoTable();
void EveryLoop();
// void WebPage_Root_SendStyle();
// void WebPage_Root_SendBody();
// void WebPage_Root_SendStatus();
// void WebAppend_Root_Add_Main_Buttons();
void WebPage_Root_AddHandlers();
// void WebPage_Root_SendParseJSONScripts();
// void WebPage_Root_SendScripts();
void WebPage_Root_SendInformationModule();
// void WebSendBody_InfoTable2();
// void WebSendBody_Palette2();
// void SetRGBwithNeoPixel();
// void SetRGBwithNeoPixelImageTest();
void SetRefreshLEDs();    
void GenerateAnimationPixelAmountMaps();
// void SetAnimationBrightness(uint8_t brt);
void WebAppend_JSON_RootPage_LiveviewPixels();
// void WebSend_DivStyle_Root(AsyncWebServerRequest *request);
// void WebSend_JSON_Append_Root();
// void WebSend_JSON_RootPage_Parameters_RGBTable(AsyncWebServerRequest *request);
// void WebSend_JSON_RootPage_Parameters_RGBPalette(AsyncWebServerRequest *request);
// void WebPage_Root_SendJSONFetchURL();
// void SendScript_Root(AsyncWebServerRequest *request);
// void SendStyle_Root(AsyncWebServerRequest *request);
// void SendDrawDiv_Root(AsyncWebServerRequest *request);
// void Web_RGBLightSettings_RunTime_URLs(AsyncWebServerRequest *request);
// void Web_RGBLightSettings_LoadTime_URLs(AsyncWebServerRequest *request);


#ifndef DISABLE_WEBSERVER


  #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
void Web_RGBLightSettings_Draw(AsyncWebServerRequest *request);
// void Web_RGBLightSettings_LoadScript(AsyncWebServerRequest *request);
void Web_RGBLightSettings_RunTimeScript(AsyncWebServerRequest *request);
// void Web_RGBLightSettings_FillOptions(AsyncWebServerRequest *request);
void WebAppend_RGBLightSettings_FillOptions_Controls();
void WebAppend_RGBLightSettings_FillOptions_Controls_Selected();
void WebAppend_RGBLightSettings_Draw_Controls();
// void Web_RGBPaletteEditor_RunTime_URLs(AsyncWebServerRequest *request);
// void Web_RGBPaletteEditor_LoadTime_URLs(AsyncWebServerRequest *request);
// void Web_RGBPaletteEditor_Draw(AsyncWebServerRequest *request);
// void Web_RGBPaletteEditor_LoadScript(AsyncWebServerRequest *request);
// void Web_RGBPaletteEditor_RunTimeScript(AsyncWebServerRequest *request);
// void Web_RGBPaletteEditor_FillOptions(AsyncWebServerRequest *request);
#endif // USE_WEBSERVER_ADVANCED_MULTIPAGES
// void WebAppend_RGBPaletteEditor_FillOptions_Controls();
// void WebAppend_RGBPaletteEditor_FillOptions_Controls_Selected();
// void WebAppend_RGBPaletteEditor_Draw_Controls();
void Web_RGBLightSettings_UpdateURLs(AsyncWebServerRequest *request);
void Web_PaletteEditor_UpdateURLs(AsyncWebServerRequest *request);
void WebAppend_RGBLightSettings_Draw_Animation_Options();
void WebAppend_RGBLightSettings_Draw_Flasher_Options();
void Web_RGBLightSettings_ListOptions(AsyncWebServerRequest *request);
void Web_PaletteEditor_ListOptions(AsyncWebServerRequest *request);
void WebAppend_PaletteEditor_FillOptions_Controls();
void WebAppend_RGBLightSettings_Draw_PageButtons();
void WebAppend_Root_Draw_Table();
// void WebAppend_Root_Draw_RGBTable();
// // void WebAppend_Root_Draw_RGBLive();
void Web_PaletteEditor_Draw(AsyncWebServerRequest *request);
void WebAppend_PaletteEditor_Draw_Editor_Form();
void WebAppend_Root_ControlUI();
// void WebSend_JSON_RootPage_Parameters_Liveview(AsyncWebServerRequest *request);
// void WebSend_JSON_RGBTable(AsyncWebServerRequest *request);
// void WebSend_JSON_Empty_Response();

void HandlePage_PaletteEditor(AsyncWebServerRequest *request);
// void HandleMixerEditor();
void HandleParameters_RGBLightSettings(AsyncWebServerRequest *request);
void HandlePage_RGBLightSettings(AsyncWebServerRequest *request);

// void SetMode_UpdateColour(uint8_t colour);
// void HandleTimerConfiguration();
// void TimerSaveSettings();
// void WebSend_JSON_RootPage_Parameters();
// void WebSend_JSON_RGBPalette();
// void WebSend_JSON_RootPage_Parameters_Append_Palette(JsonObject root);
// void WebSend_JSON_RootPage_Parameters_Append_Table(JsonObject root);
// void WebSave_RGBColourSelectorOnly();
// void WebSave_RGBControls(AsyncWebServerRequest *request);
// void WebCommand_Parse(void);
void WebAppend_Root_Status_Table();

#endif //DISABLE_WEBSERVER


void RefreshLEDIndexPattern();
// void UpdateDesiredColourFromPaletteSelected();



void StripUpdate();


  struct POWER_RATING{
    float Average_mA_Per_Pixel_Step = 0.0784; //  20/255    //MAKE PROGMEM
    // uint8_t G_Channel_mA = 20;
    // uint8_t B_Channel_mA = 20;
    // uint8_t W_Channel_mA = 20;
    float current_mA;
    uint8_t power;
  }power_rating;

  template<typename T>
  void swap(T& x, T& y)
  {
    T tmp = x;
    x = y;
    y = tmp;
  }

  timereached_t tSavedCalculatePowerUsage;



    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
    void SubTask_Manual_SetPixel();
    #endif// ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL



  #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
    uint8_t ConstructJSON_Manual_SetPixel(uint8_t json_level = 0);
  #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL


void SetPixelColor_All(RgbcctColor colour);

  // #define WEB_HANDLE_ANIMATION_BRIGHTNESS_SLIDER "animation_brightness"
  // #define WEB_HANDLE_SCENE_COLOUR_WHITE_SLIDER "scn_sld_w"
  // #define USE_APPENDED_COLOUR_VALUES_TO_EDITOR_LIST
  
  
    // enum WEBHANDLE_RGBCONTROLS_ITEM_IDS{
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELSUPDATED_PERCENTAGE=0,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELORDER,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_RATE,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_PERIOD,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_TRANSITIONMETHOD,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_ANIMATIONMODE,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_PALETTE,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_EFFECTS,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_MIXER_RUNNING_ID,
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_NONE
    // };

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0);
    uint8_t ConstructJSON_Animation(uint8_t json_level = 0);
    uint8_t ConstructJSON_Ambilight(uint8_t json_level = 0);
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    uint8_t ConstructJSON_Notifications(uint8_t json_level = 0);
    #endif
    uint8_t ConstructJSON_State(uint8_t json_level = 0);
    uint8_t ConstructJSON_Flasher(uint8_t json_level = 0);
    uint8_t ConstructJSON_Mixer(uint8_t json_level = 0);
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
    uint8_t ConstructJSON_Debug_Animations_Progress(uint8_t json_level = 0);
    
    ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
    mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);

    
    #endif // USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK

  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mAnimatorLight>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

  
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mAnimatorLight> mqtthandler_settings_teleperiod;

    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
      #endif
      MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_EFFECTS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID,
      #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
        MQTT_HANDLER_MODULE_MANUAL_SETPIXEL_TELEPERIOD_ID,
      #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
        MQTT_HANDLER_MODULE_DEBUG_ANIMATION_PROGRESS_TELEPERIOD_ID,
      #endif
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };


    struct handler<mAnimatorLight> mqtthandler_animation_teleperiod;  
    struct handler<mAnimatorLight> mqtthandler_ambilight_teleperiod;
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    struct handler<mAnimatorLight> mqtthandler_notifications_teleperiod;
    #endif
    struct handler<mAnimatorLight> mqtthandler_state_teleperiod;
    struct handler<mAnimatorLight> mqtthandler_flasher_teleperiod;
    struct handler<mAnimatorLight> mqtthandler_mixer_teleperiod;
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      struct handler<mAnimatorLight> mqtthandler_manual_setpixel;
    #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      struct handler<mAnimatorLight> mqtthandler_debug_animations_progress;
    #endif

    
  // Could this be put into a function? to allow me to set things to all in this, or by ID
  struct handler<mAnimatorLight>* mqtthandler_list[
    5
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      +1
    #endif
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      +1
    #endif
    ] = {
    &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
    &mqtthandler_state_teleperiod,
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
      &mqtthandler_notifications_teleperiod,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
      &mqtthandler_flasher_teleperiod, 
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MIXER
      &mqtthandler_mixer_teleperiod,
    #endif
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      &mqtthandler_manual_setpixel,
    #endif
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      &mqtthandler_debug_animations_progress,
    #endif
    &mqtthandler_settings_teleperiod
  };



  #endif
  
  /*******NOTIFICATIONS*********************************************************************************************/

  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    #ifndef STRIP_NOTIFICATION_SIZE
      #define STRIP_NOTIFICATION_SIZE (STRIP_SIZE_MAX>12?12:STRIP_SIZE_MAX) // Set max if not defined as 20
    #endif

    void init_Notifications();
    void SubTask_Notifications();
    void parsesub_Notifications(JsonParserObject obj);
    
    const char* GetNotificationModeNamebyID(uint8_t id, char* buffer);
    int8_t      GetNotificationModeIDbyName(const char* c);
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

    #define D_NOTIFICATION_PROGRESS_RANGE 100
    #define D_NOTIFICATION_PROGRESS_RANGE_F 100.0f
    
    typedef union {
      uint8_t data; // allows full manipulating
      struct { 
        
        uint8_t fForcePanelUpdate : 1;//= false;
        uint8_t fShowPanelUpdate  : 1;//= false;
        uint8_t fEnableTimeoutAll : 1;//   = false;
      
        // Reserved
        uint8_t reserved : 4;
      };
    } NOTIFICATION_FLAGS;
    struct NOTIF{
      NOTIFICATION_FLAGS flags;
      struct TSAVED{
        uint32_t ForceUpdate = millis();
        uint32_t TimeoutCounter = millis();
        uint32_t Timeout = millis();
        uint32_t AutoOff = millis();
        uint32_t tSavedForceUpdate=millis();
        uint32_t tNotifPanelTimeout;
      }tSaved;
      struct PIXELN{
        /**
         * Using full Rgbcct, with possible Rgbw only to save 1 byte per notif pixel
         * */        
        RgbcctColor colour;
        /**
         * Using full 255 range
         * */
        uint8_t brightness;
        /**
         * Using full 255 range
         * */
        uint8_t progress = 0;
        /**
         * Mode
         * */
        uint8_t  mode = NOTIF_MODE_STATIC_OFF_ID;
        /**
         * Mode
         * */    
        uint16_t period_ms = 1000;
        /**
         * Speed 0-255, relative to period
         * */       
        uint8_t speed = 0;
        /**
         * Trigger times
         * */   
        uint32_t tSavedUpdate;
        uint16_t tRateUpdate = 10;
        /**
         * Seconds until auto turn off
         * */   
        uint16_t auto_time_off_secs = 0; // reset pixel to off
      }pixel[STRIP_NOTIFICATION_SIZE];
    }notif;

#endif


RgbcctColor ApplySimpleEffectOnColour(RgbcctColor colour_start, RgbcctColor colour_end, float progress, uint8_t effect_type);
 

    void CommandSet_EffectsModeID(uint8_t mode, uint8_t segment = 0);
    // int8_t GetEffectsModeIDbyName(const char* c);
    const char* GetEffectsModeNamebyID(uint8_t id, char* buffer, uint8_t buflen);






    byte getSameCodeLength(char code, int index, char const cronixieDisplay[]);
    void setCronixie();
    void _overlayCronixie();    
    void _drawOverlayCronixie();

    //set.cpp
    void _setRandomColor(bool _sec,bool fromButton=false);

    // int getNumVal(const String* req, uint16_t pos);
    // bool updateVal(const String* req, const char* key, byte* val, byte minv=0, byte maxv=255);

    // bool applyPreset(byte index, bool loadBri = true);
    // void savePreset(byte index, bool persist = true);
        

    uint8_t flags_hac_wled_animator_switch = 1; // deflault hacs


    // led fx library object
    WS2812FX* mEffects = nullptr;

 void Init_WLED();
    void SubTask_WLED_Animation();
 

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
    uint16_t ledCount = 30;          // overcurrent prevented by ABL
    bool useRGBW      = false;       // SK6812 strips can contain an extra White channel
    bool turnOnAtBoot = true;        // turn on LEDs at power-up
    byte bootPreset   = 0;           // save preset to load after power-up


    byte col[4]    = { 255, 160, 0, 0 };  // current RGB(W) primary color. col[] should be updated if you want to change the color.
    byte colSec[4] = { 0, 0, 0, 0 };      // current RGB(W) secondary color
    byte briS     = 128;                     // default brightness

    bool fadeTransition      = true;   // enable crossfading color transition
    uint16_t transitionDelay = 750;    // default crossfade duration in ms

    bool skipFirstLed  = false;        // ignore first LED in strip (useful if you need the LED as signal repeater)
    byte briMultiplier = 100;          // % of brightness to set (to limit power, if you set it to 50 and set bri to 255, actual brightness will be 127)

    uint16_t realtimeTimeoutMs = 2500;               // ms timeout of realtime mode before returning to normal mode
    int arlsOffset = 0;                              // realtime LED offset
    bool arlsDisableGammaCorrection = true;          // activate if gamma correction is handled by the source
    bool arlsForceMaxBri = false;                    // enable to force max brightness if source has very dark colors that would be black

    // wifi
    bool apActive = false;
    bool forceReconnect = false;
    uint32_t lastReconnectAttempt = 0;
    bool interfacesInited = false;
    bool wasConnected = false;

    // color
    byte colOld[4]    = { 0, 0, 0, 0 };        // color before transition
    byte colT[4]      = { 0, 0, 0, 0 };          // color that is currently displayed on the LEDs
    byte colIT[4]     = { 0, 0, 0, 0 };         // color that was last sent to LEDs
    byte colSecT[4]   = { 0, 0, 0, 0 };
    byte colSecOld[4] = { 0, 0, 0, 0 };
    byte colSecIT[4]  = { 0, 0, 0, 0 };

    // transitions
    bool transitionActive = false;
    uint16_t transitionDelayDefault = transitionDelay;
    uint16_t transitionDelayTemp = transitionDelay;
    unsigned long transitionStartTime;
    float tperLast = 0;        // crossfade transition progress, 0.0f - 1.0f
    bool jsonTransitionOnce = false;

    // brightness
    unsigned long lastOnTime = 0;
    bool offMode = !turnOnAtBoot;
    byte bri = briS;
    byte briOld = 0;
    byte briT = 0;
    byte briIT = 0;
    byte briLast = 128;          // brightness before turned off. Used for toggle function
    byte whiteLast = 128;        // white channel before turned off. Used for toggle function

   
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

/**************
 * 
 * 
 * WLED Adaptations
 * 
 * 
 * ****/





};

#endif

#endif

// {
//   "Effects": {
//     "Function":3,
//     "Mode":88,
//     "Speed":100,
//     "Current":1,
//     "Intensity":100,
//     "Palette":30
//   }
// }


// {
//   "AnimationMode": "Effects",
//   "Effects": {
//     "Function":34
//   },
//   "ColourPalette":62,
//   "BrightnessRGB": 0,
//   "PaletteGeneration":{"RandomiseBrightnessMode":1},
//   "Transition": {
//     "Time": 20,
//     "RateMs":100,
//     "Order":"Random"
//   }
// }