#ifndef _ADALIGHTNEOPIXEL_H
#define _ADALIGHTNEOPIXEL_H 7.0

#include "2_CoreSystem/mFirmwareDefaults.h"   // New final firmware defines for modules desired
#include "0_ConfigUser/mFirmwareCustom.h"   // New final firmware defines for modules desired

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE


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


#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "6_Lights/_Interface/palettes.h"
#include "0_ConfigUser/mUserConfig.h"
#include "mRGBAnimator_web.h"
#include "1_TaskerManager/mTaskerManager.h"

#include "6_Lights/_Interface/mInterfaceLight.h"

// Default enables, unless I add disable versions
#ifndef DISABLE_PIXEL_FUNCTION_FLASHER 
  #define ENABLE_PIXEL_FUNCTION_FLASHER
#endif // DISABLE_PIXEL_FUNCTION_FLASHER


// #define ENABLE_PIXEL_FUNCTION_FLASHER
#define ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL

#if   defined(USE_WS28XX_FEATURE_3_PIXEL_TYPE)
  typedef NeoRgbFeature selectedNeoFeatureType;
#elif defined(PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL)   //includes 4th channel of white, pixel order variable still important
  typedef NeoGrbwFeature selectedNeoFeatureType;
#else
  typedef NeoGrbFeature selectedNeoFeatureType;
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
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FLASHER_CTR)       "flasher";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MIXER_CTR)         "mixer";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETPIXEL_MANUALLY_CTR) "setpixel_manually";


    
    
//     #define D_NOTIF_MODE_OFF_NAME_CTR           "OFF"
//     #define D_NOTIF_MODE_STATIC_OFF_NAME_CTR    "Static OFF"
//     #define D_NOTIF_MODE_STATIC_ON_NAME_CTR     "Static ON"
//     #define D_NOTIF_MODE_BLINKING_OFF_NAME_CTR  "Blinking ON"
//     #define D_NOTIF_MODE_BLINKING_ON_NAME_CTR   "Blinking OFF"
//     #define D_NOTIF_MODE_PULSING_OFF_NAME_CTR   "Pulsing ON"
//     #define D_NOTIF_MODE_PULSING_ON_NAME_CTR    "Pulsing OFF"


DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_SLOW_GLOW_NAME_CTR)          "Slow Glow"    ;     

DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_SEQUENTIAL_NAME_CTR)         "SEQUENTIAL"; 


DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_NAME_CTR)         "SLOW_FADE_BRIGHTNESS_ALL";
DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_NAME_CTR)         "SLOW_FADE_SATURATION_ALL";
DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_NAME_CTR)      "SLOW_FADE_BRIGHTNESS_RANDOM";
DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_SATURATION_RANDOM_NAME_CTR)      "SLOW_FADE_SATURATION_RANDOM";
DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_NAME_CTR)             "FLASH_TWINKLE_RANDOM";                   
DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_NAME_CTR)         "FLASH_TWINKLE_SEQUENTIAL";                          
DEFINE_PGM_CTR(PM_FLASHER_FUNCTION_FLASH_GLIMMER_RANDOM_NAME_CTR)             "FLASH_GLIMMER_RANDOM"; // sequential of another colour (white) every x bulbs. ie 20 bulbs total, every 4th part of sequential  

DEFINE_PGM_CTR(PM_FLASHER_REGION_SLOW_FADE_NAME_CTR)                          "SLOW_FADE";
DEFINE_PGM_CTR(PM_FLASHER_REGION_TWINKLE_FLASH_NAME_CTR)                      "TWINKLE_FLASH";

#define D_FLASHER_REGION_COLOUR_SELECT_NAME_CTR "Colour Select"
#define D_FLASHER_REGION_ANIMATE_NAME_CTR "Animate"

DEFINE_PGM_CTR(PM_FLASHER_REGION_COLOUR_SELECT_NAME_CTR) D_FLASHER_REGION_COLOUR_SELECT_NAME_CTR;
DEFINE_PGM_CTR(PM_FLASHER_REGION_ANIMATE_NAME_CTR) D_FLASHER_REGION_ANIMATE_NAME_CTR;

#include <functional>
// #define ANIM_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
// #define ANIM_CALLBACK_SIGNATURE std::function<void(const AnimationParam& param)> callback
#define ANIM_FUNCTION_SIGNATURE std::function<void(const AnimationParam& param)> anim_function_callback

class mRGBAnimator{
  private:
  public:
    mRGBAnimator(){};
    void pre_init(void);
    void init(void);
    
    // Put settings at top of class from now on, use it for common settings
    struct SETTINGS{
      struct FLAGS{
          uint8_t EnableSceneModeWithSliders = true;
          uint8_t TemplateProvidedInProgMem = false;
          uint8_t EnableModule = false;
      }flags;
    }settings;
  

    void SetPixelColor(uint16_t indexPixel, RgbTypeColor color);
    RgbTypeColor GetPixelColor(uint16_t indexPixel);

    uint8_t counter_test = 0;
    

    #ifdef PIXEL_LIGHTING_HARDWARE_SK6812_STRIP
      typedef NeoSk6812Method selectedNeoSpeedType;
      NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;
    #endif    
    #ifndef PIXEL_LIGHTING_HARDWARE_SK6812_STRIP
    // Define pixel hardware sublibrary to use
    #ifdef USE_WS28XX_HARDWARE_WS2801
      NeoPixelBus<DotStarBgrFeature, DotStarMethod> *stripbus = nullptr; 
    #else 
    // SET Method
      // #ifdef USE_WS28XX_METHOD_800KBPS
      //   typedef Neo800KbpsMethod selectedNeoSpeedType;
      // #elif USE_WS28XX_METHOD_BITBANG
      //   typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
      // #elif USE_WS28XX_METHOD_UART800KBPS
      //   typedef NeoEsp8266Uart800KbpsMethod selectedNeoSpeedType;
      // #elif USE_WS28XX_METHOD_I2S1_800KBPS_ESP32
      //   typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
      // #elif USE_WS28XX_METHOD_RMT0_800KBPS_ESP32
      //   typedef NeoEsp32Rmt0800KbpsMethod selectedNeoSpeedType;        
      // #else
      //   //   typedef Neo800KbpsMethod selectedNeoSpeedType;
      //   #ifdef ENABLE_DEBUG_ESP_DECODER
      //   typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
      //   #else
      //   typedef Neo800KbpsMethod selectedNeoSpeedType;
      //   #endif
      // #endif  
      #ifdef USE_WS28XX_METHOD_800KBPS
        typedef Neo800KbpsMethod selectedNeoSpeedType;
      #elif defined(USE_WS28XX_METHOD_BITBANG)
        typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
      // #elif USE_WS28XX_METHOD_UART800KBPS
      //   typedef NeoEsp8266Uart800KbpsMethod selectedNeoSpeedType;
      // #elif USE_WS28XX_METHOD_I2S1_800KBPS_ESP32
      //   typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
      // #elif defined(USE_WS28XX_METHOD_RMT0_800KBPS_ESP32)
      //  // typedef NeoEsp32Rmt0800KbpsMethod selectedNeoSpeedType;    
      //  typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;    
      #else
        //   typedef Neo800KbpsMethod selectedNeoSpeedType;
        // #ifdef ENABLE_DEBUG_ESP_DECODER
        // typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
        // #else
        // typedef Neo800KbpsMethod selectedNeoSpeedType;

        // NeoEsp32I2s1800KbpsMethod

        // working, no wifi
        //typedef NeoEsp32Rmt6800KbpsMethod selectedNeoSpeedType;
        //test 2
        #ifdef ESP32
        typedef NeoEsp32Rmt1800KbpsMethod selectedNeoSpeedType;
        // typedef NeoEsp32Rmt7800KbpsMethod selectedNeoSpeedType;
        #else
        //  typedef NeoEsp32I2s1800KbpsMethod selectedNeoSpeedType;
        // typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
        // typedef NeoWs2811Method  selectedNeoSpeedType;
        typedef Neo800KbpsMethod selectedNeoSpeedType; //default for all
        #endif

        // #endif
      #endif  



      // Configure
        NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;
      #endif
    #endif
    
    // #ifndef ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE
    // NeoPixelAnimator* animator_controller = nullptr;
    // #endif // ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE

    uint8_t test_flag = false;

  
    // Group of ifndef's to allow defaults to be set, and users to set defaults using basic numbers
    #ifdef STRIP_SIZE_MAX
      #ifndef STRIP_REPEAT_OUTPUT_MAX
        #define STRIP_REPEAT_OUTPUT_MAX STRIP_SIZE_MAX
      #endif
    #else
      #define STRIP_SIZE_MAX 50 //default length of 50
    #endif
    
    #ifndef STRIP_SIZE_REPEAT_MAX
      #define STRIP_SIZE_REPEAT_MAX 50 //default length of 50
    #endif
    #ifndef STRIP_OUTPUT_REPEATED_LENGTH
      #define STRIP_OUTPUT_REPEATED_LENGTH 20
    #endif
    #ifndef STRIP_REPEAT_OUTPUT_MAX
      #define STRIP_REPEAT_OUTPUT_MAX 200
    #endif
    #ifndef ANIMATOR_SIZE_MAX
      #ifdef ENABLE_PIXEL_SINGLE_ANIMATION_CHANNEL
        #define ANIMATOR_SIZE_MAX 1//STRIP_SIZE_MAX
      #else
        #define ANIMATOR_SIZE_MAX STRIP_SIZE_MAX
      #endif
    #endif

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    #define PRESET_COLOUR_MAP_INDEXES_MAX COLOUR_MAP_LENGTH_ID 
    uint16_t strip_size = STRIP_SIZE_MAX; //allow variable control of size
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
    void AnimUpdateMemberFunction_TraditionalChasing(const AnimationParam& param);
    void AnimUpdateMemberFunction_TraditionalChasing2(const AnimationParam& param);
    void AnimUpdateMemberFunction_BlendStartingToDesiredColour(const AnimationParam& param);
    void AnimUpdateMemberFunction_TwinkleSingleColourRandom(const AnimationParam& param);
    void AnimUpdateMemberFunction_TwinkleUsingPaletteColourRandom(const AnimationParam& param);
    
    void AnimUpdateMemberFunction_Sequential(const AnimationParam& param);
    void LoadMixerGroupByID(uint8_t id);

    // void (mRGBAnimator::*AnimUpdateMemberFunction_Ptr)(const AnimationParam& param) = nullptr; // member-function to sender with one args

    // #define D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH 20
    // // uint8_t mapped_array_data_array[15] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
    // uint8_t mapped_array_data_array[D_MAPPED_ARRAY_DATA_MAXIMUM_LENGTH];
    //  = {
    //   20,19, //39
    //   15,16, //70
    //   13,9, //92
    //   7,1  //100
    // };
    

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
  


    // void (mRGBAnimator::*AnimUpdateMemberFunction_Ptr2)() = nullptr; // member-function to sender with one args

    ANIM_FUNCTION_SIGNATURE;
    mRGBAnimator& setAnimFunctionCallback(ANIM_FUNCTION_SIGNATURE);

    bool OverwriteUpdateDesiredColourIfMultiplierIsEnabled();

    #ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
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
    enum AMBILIGHT_SCREEN_INDEX{SCREEN_CENTRE=0,SCREEN_LEFT=1,SCREEN_RIGHT=2};
    struct AMBILIGHT_SCREEN_SETTINGS{
      uint8_t ambilight_mode=AMBILIGHT_SIDES_ID;
      uint32_t tSavedUpdate = millis();
      uint32_t ratemsSavedUpdate = 1000;
      uint8_t fUpdate = false;

      struct SCREENS{
        struct EDGE{
          HsbColor colour;
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

    /*******USER LIGHTS & FLASHER**
     * 
  *******************************************************************************************/

    void RotateDesiredColour(uint8_t pixels, uint8_t direction = 0);
    void UpdateStartingColourWithGetPixel();
    


  #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
  const char* GetFlasherFunctionName(char* buffer);
  const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer);
  int8_t      GetFlasherFunctionIDbyName(const char* c);
  enum FLASHER_FUNCTION_IDS{
    FLASHER_FUNCTION_NONE_ID = 0,
    FLASHER_FUNCTION_SLOW_GLOW_ID, // solid colours, 1 100%, moving from previous to next
    FLASHER_FUNCTION_SEQUENTIAL_ID, //instant, or faded (ie INWAVES) // FLASHER_FUNCTION_SEQUENTIAL_FLASH_ID, //FLASHER_FUNCTION_CHASING_FLASH_ID,
    FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID, // change ALL, 0 - 100%
    FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_ID, // change ALL, 0 - 100%
    FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_ID, // change ALL, 0 - 100%
    FLASHER_FUNCTION_SLOW_FADE_SATURATION_RANDOM_ID, // change ALL, 0 - 100%
    FLASHER_FUNCTION_FLASH_TWINKLE_SINGLE_COLOUR_RANDOM_ID, //random leds flash to 100% brightness (modes=instant on/off, multiple pulses)
    FLASHER_FUNCTION_FLASH_TWINKLE_PALETTE_COLOUR_RANDOM_ID,

    /**
     * Palette is first drawn with certain lower brightness level, then
     * additional changes takes those colours and bumbs their brightness up to desired brightness
     * Few change, random pixels bumped up
     * Check if their brightness is <upper, before asserting them brighter (or convert to hsbcolour, flick up brightness if needed then change back to rgb)
     * Goes from low to high then stops
     * */
    FLASHER_FUNCTION_POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY,


    /**
     * 0/100% animaiton progress is low brightness, 50% flips to upper brightness
    */
    FLASHER_FUNCTION_TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK,

    // FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID, // sequential flash of white on x leds 
    FLASHER_FUNCTION_FADE_GRADIENT_ID, //single pixels: static, twinkle, pick from palette
    //another flash to "off" or simple set flash colour to off??
    FLASHER_FUNCTION_FLASH_GLIMMER_RANDOM_ID, // tinkle=leds flash independant, glimmer=leds flash with dimming effect on others
    
    FLASHER_FUNCTION_TESTER_ID,
    
    // The one when applybrightness was broke
    // done by applying new cl/brightness to some, then each call reducing the colour crushing it
    //do with percent to show on at one time
    FLASHER_FUNCTION_PULSE_RANDOM_ON, 
    FLASHER_FUNCTION_PULSE_RANDOM_ON_TWO_ID,

    /**
     * Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     * */
    // FLASHER_FUNCTION_SHOWING_MULTIPLES_OF_COLOURS_ID,

    /**
     * Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     * Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * */
    FLASHER_FUNCTION_SLOW_GLOW_PARTIAL_PALETTE_STEP_THROUGH_ID,

    // Apply sinewave to brightness, 25% increasing, 50% decreasing, 25% off
    FLASHER_FUNCTION_PULSE_SINE_WAVE_BRIGHTNESS,

    // Instead of just applying new colours, also randomise the brightness levels between range
    FLASHER_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS_ID,
    
    FLASHER_FUNCTION_LENGTH_ID
  };                                   
  //https://www.youtube.com/watch?v=D2IBGlDJ3lg @2.50

  
// const uint16_t PixelCount = 16; // make sure to set this to the number of pixels in your strip
// const uint16_t PixelPin = 2;  // make sure to set this to the correct pin, ignored for Esp8266
// const uint16_t AnimCount = 1; // we only need one
// const uint16_t TailLength = 6; // length of the tail, must be shorter than PixelCount
// const float MaxLightness = 0.4f; // max lightness at the head of the tail (0.5f is full bright)

// NeoGamma<NeoGammaTableMethod> colorGamma; // for any fade animations, best to correct gamma
// void DrawTailPixels();





  int8_t GetFlasherRegionIDbyName(const char* c);
  const char* GetFlasherRegionName(char* buffer);
  const char* GetFlasherRegionNamebyID(uint8_t id, char* buffer); 
  enum FLASHERREGION{FLASHER_REGION_COLOUR_SELECT_ID=1,
                     FLASHER_REGION_ANIMATE_ID};           

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
  } FLASHERSETTINGS_FLAGS;

  enum RANDOM_TRANSITION_TIMINGS_PROFILES_IDS{
    RANDOM_TRANSITION_TIMINGS_PROFILES_TRIANGLE_RAMP_ID=0,
    RANDOM_TRANSITION_TIMINGS_PROFILES_EXPONENTIAL_HIGH_ID, //more on higher
    RANDOM_TRANSITION_TIMINGS_PROFILES_EXPONENTIAL_LOW_ID,  //More on lower
  };
  // Make generate function with lots of mapped values, with percentage 0 to 255 to retrieve it

  struct FLASHERSETTINGS{
    // Flashing function
    uint8_t function = FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_ID;
    // Section of animation
    uint8_t region = FLASHER_REGION_COLOUR_SELECT_ID;
    // flag needed to force completion between regions
    uint8_t flag_finish_flasher_pair = false;

    FLASHERSETTINGS_FLAGS flags;

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


    // Shared timers
    struct TSAVED{
      uint32_t Update = millis();
      // uint32_t tSavedChange_animation_something;
      //uint32_t Update2 = millis();
    }tSaved;
    
    
    // Function specific animations
    struct FUNCTION_SEQUENTIAL{
      //uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
    }function_seq;
    struct FUNCTION_SLO_GLO{
      // uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
    }function_slo_glo;
    struct FUNCTION_SLOW_FADE{
      //uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
      uint8_t direction = 0;
    }function_slow_fade;

    struct FUNCTION_FLASH_TWINKLE{
      //uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
    }function_flash_twinkle;

    // struct FLASHER_FUNCTION_SLOW_GLOW_ON_BRIGHTNESS{
    //   //uint32_t tSavedNewSpeedUpdate = millis();
    //   // // uint8_t rate_index = 0;
    //   // uint8_t bri
    // }function_flash_twinkle;

  }flashersettings;



  void SubTask_Flasher_Main();
  void SubTask_Flasher_Animate();
  void SubTask_Flasher_Animate_Mixer();
  
  // Functions
  void SubTask_Flasher_Animate_Function_Slow_Glow();
  void SubTask_Flasher_Animate_Function_Sequential();
  void SubTask_Flasher_Animate_Function_Twinkle_SingleColour_Random();
  void SubTask_Flasher_Animate_Function_Twinkle_PaletteColour_Random();
  void SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
  void SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
  void SubTask_Flasher_Animate_Function_Fade_Gradient();
  void SubTask_Flasher_Animate_Function_Slow_Glow_On_Brightness();


  void SubTask_Flasher_Animate_Function_Tester();
  void AnimUpdateMemberFunction_Tester(const AnimationParam& param);

  void SubTask_Flasher_Animate_Function_Pulse_Random_On();
  void AnimUpdateMemberFunction_Pulse_Random_On(const AnimationParam& param);


  void SubTask_Flasher_Animate_Function_Pulse_Random_On_2();
  void AnimUpdateMemberFunction_Pulse_Random_On_2(const AnimationParam& param);


  void SubTask_Flasher_Animate_Function_Slow_Glow_Partial_Palette_Step_Through();
  void AnimUpdateMemberFunction_Slow_Glow_Partial_Palette_Step_Through(const AnimationParam& param);


  void SubTask_Flasher_Animate_Function_Popping_Palette_Brightness_From_Lower_To_Upper_Boundary();


void SubTask_Flasher_Animate_Function_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back();
  void AnimUpdateMemberFunction_Twinkle_Palette_Brightness_From_Lower_To_Upper_And_Back(const AnimationParam& param);



  void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Change_Colour_Region();
  void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Time();
  void SubTask_Flasher_Animate_Parameter_Check_Update_Timer_Transition_Rate();


#endif // ENABLE_PIXEL_FUNCTION_FLASHER

  struct SHARED_FLASHER_PARAMETERS{
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
  enum FLASHER_FUNCTION_MIXER{
    FLASHER_FUNCTION_MIXER_01_ID=0,
    FLASHER_FUNCTION_MIXER_02_ID,
    FLASHER_FUNCTION_MIXER_03_ID,
    FLASHER_FUNCTION_MIXER_04_ID,
    FLASHER_FUNCTION_MIXER_05_ID,
    FLASHER_FUNCTION_MIXER_06_ID,
    FLASHER_FUNCTION_MIXER_07_ID,
    FLASHER_FUNCTION_MIXER_08_ID,
    FLASHER_FUNCTION_MIXER_09_ID,
    FLASHER_FUNCTION_MIXER_10_ID,
    FLASHER_FUNCTION_MIXER_11_ID,
    FLASHER_FUNCTION_MIXER_12_ID,
    // FLASHER_FUNCTION_MIXER_13_ID,
    // FLASHER_FUNCTION_MIXER_14_ID, 
    // FLASHER_FUNCTION_MIXER_15_ID, 
    FLASHER_FUNCTION_MIXER_LENGTH_ID
  };
  #define FLASHER_FUNCTION_MIXER_MAX FLASHER_FUNCTION_MIXER_LENGTH_ID
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
      uint8_t running_id = FLASHER_FUNCTION_MIXER_05_ID;
      uint16_t time_on_secs[FLASHER_FUNCTION_MIXER_MAX]; // stores which to use
      int16_t time_on_secs_active = 60; //signed
      // char running_friendly_name_ctr[40];
      struct TIMES{
       // uint16_t enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_MAX]; // if set, this mode will only run if INSIDE the "flashy" time period
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
    uint8_t running_id = FLASHER_FUNCTION_MIXER_01_ID;
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

      uint8_t animation_transition_order = TRANSITION_ORDER_INORDER_ID;
      uint8_t flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
      uint16_t run_time_duration_sec = 120;
      uint8_t palette_id = 0;

      uint8_t pixel_multiplier_id = PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID;
      uint8_t pixel_multiplier_enabled = false;

      MIXER_SETTINGS_GROUP_FLAGS flags;

    }group[FLASHER_FUNCTION_MIXER_MAX];

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


    uint8_t fPixelsUpdated = false;
    uint8_t update_all = false;
    uint8_t blocking_force_animate_to_complete = true;
    uint32_t tSavedBlocking_ForceAnimateToComplete = millis();
    
    int8_t Tasker(uint8_t function);
    int8_t Tasker_Web(uint8_t function);

    int8_t CheckAndExecute_JSONCommands(void);
    void parse_JSONCommand(void);


    
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
    uint16_t GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage);
    uint8_t  GetPixelsToUpdateAsPercentageFromNumber(uint16_t number);
    

    /***
     * Possible bit level packing of enabling or disabling palette colours allowed in a palette
     * */
    // uint32_t palette_pixel_enabled_32bitpacked = 0xFFFF;

    
void ApplyBrightnesstoDesiredColour(uint8_t brightness);

RgbcctColor ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness);



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




RgbcctColor ApplyBrightnesstoDesiredColour(RgbcctColor full_range_colour, uint8_t brightness);


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
    //   WEBHANDLE_RGBCONTROLS_ITEM_IDS_FLASHER,
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

  #ifdef USE_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mRGBAnimator>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mRGBAnimator> mqtthandler_settings_teleperiod;

    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
      #endif
      MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_FLASHER_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID,
      #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
        MQTT_HANDLER_MODULE_MANUAL_SETPIXEL_TELEPERIOD_ID,
      #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };


    struct handler<mRGBAnimator> mqtthandler_animation_teleperiod;  
    struct handler<mRGBAnimator> mqtthandler_ambilight_teleperiod;
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    struct handler<mRGBAnimator> mqtthandler_notifications_teleperiod;
    #endif
    struct handler<mRGBAnimator> mqtthandler_state_teleperiod;
    struct handler<mRGBAnimator> mqtthandler_flasher_teleperiod;
    struct handler<mRGBAnimator> mqtthandler_mixer_teleperiod;
    #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      struct handler<mRGBAnimator> mqtthandler_manual_setpixel;
    #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  #endif
  
  /*******NOTIFICATIONS*********************************************************************************************/

  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    #ifndef STRIP_NOTIFICATION_SIZE
      #define STRIP_NOTIFICATION_SIZE (STRIP_SIZE_MAX>20?20:STRIP_SIZE_MAX) // Set max if not defined as 20
    #endif

    // #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    void init_NotificationPanel();
    // #endif

    void SubTask_NotificationPanel();
    void SubTask_Notifications();
    
    const char* GetNotificationModeNamebyID(uint8_t id, char* buffer);
    int8_t      GetNotificationModeIDbyName(const char* c);
    enum NOTIF_MODE{NOTIF_MODE_OFF_ID=0,
                    NOTIF_MODE_STATIC_OFF_ID,
                    NOTIF_MODE_STATIC_ON_ID,
                    NOTIF_MODE_FADE_OFF_ID,
                    NOTIF_MODE_FADE_ON_ID,
                    NOTIF_MODE_BLINKING_OFF_ID,
                    NOTIF_MODE_BLINKING_ON_ID,
                    NOTIF_MODE_PULSING_OFF_ID,
                    NOTIF_MODE_PULSING_ON_ID};
    
    typedef union {
      uint8_t data; // allows full manipulating
      struct { 
        
      uint8_t fForcePanelUpdate : 1;//= false;
      uint8_t fShowPanelUpdate  : 1;//= false;
      uint8_t fEnableTimeoutAll : 1;//   = false;
      uint8_t fWhenOnlyWhiteExistsSetBrightnessOfHSBtoZero : 1;

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
        uint32_t tSavedUpdateHeatingPanel;
        uint32_t tNotifPanelTimeout;
      }tSaved;
      struct PIXELN{
        uint8_t  mode = NOTIF_MODE_OFF_ID; // Type of light pattern
        uint16_t period_ms = 1000; // Time between fully on and off
        uint8_t  transition_progess = 0; // Used for pulsing only   || reuse for fade progress
        uint16_t fade_time_ms = 1000;
        // HsbColor colour; // colour of the led
        // #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
        //   uint8_t colourWhite;
        // #endif
        RgbcctColor colour;
        uint32_t tSavedUpdate; // millis last updated
        uint16_t tRateUpdate = 10; // time between updating
        uint16_t auto_time_off_secs = 0; // reset pixel to off
      }pixel[STRIP_NOTIFICATION_SIZE];
    }notif;
    void parsesub_NotificationPanel(JsonObjectConst _obj);
#endif




};

#endif

#endif
