#ifndef _ADALIGHTNEOPIXEL_H
#define _ADALIGHTNEOPIXEL_H 7.0

#include "2_CoreSystem/mFirmwareDefaults.h"   // New final firmware defines for modules desired
#include "0_ConfigUser/mFirmwareCustom.h"   // New final firmware defines for modules desired

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "6_Lights/_Interface/palettes.h"
#include "0_ConfigUser/mUserConfig.h"
#include "mRGBAnimator_web.h"
#include "1_TaskerManager/mTaskerManager.h"

#include "6_Lights/_Interface/mInterfaceLight.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

#define ENABLE_PIXEL_FUNCTION_FLASHER
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

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_AMBILIGHT_CTR) "ambilight";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_ANIMATION_CTR) "animation";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_NOTIFICATIONS_CTR) "notifications";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_FLASHER_CTR) "flasher";

// TRANSITION_METHOD
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_NONE_NAME_CTR)    "None";   
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_BLEND_NAME_CTR)    "Blend";      
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_INSTANT_NAME_CTR)  "Instant";      
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_TWINKLE_NAME_CTR)  "Twinkle";     
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_GLIMMER_NAME_CTR)  "Glimmer";

// order is maybe becoming flasher? mode ie applied animations          //new flasher name
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_NONE_NAME_CTR) "None";
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_BLEND_NAME_CTR)      "Blend";   //slow_glow
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_RANDOM_NAME_CTR)      D_JSON_RANDOM;  //random
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR) "Centre Out" ;   // work out flasher for half, then mirror image  
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_INORDER_NAME_CTR)    D_JSON_INORDER;   //none
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_ROTATE_NAME_CTR)     "Rotate";  //rotate
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_FIXED_NAME_CTR)     "Fixed";  //rotate
    
    
    
//     #define D_NOTIF_MODE_OFF_NAME_CTR           "OFF"
//     #define D_NOTIF_MODE_STATIC_OFF_NAME_CTR    "Static OFF"
//     #define D_NOTIF_MODE_STATIC_ON_NAME_CTR     "Static ON"
//     #define D_NOTIF_MODE_BLINKING_OFF_NAME_CTR  "Blinking ON"
//     #define D_NOTIF_MODE_BLINKING_ON_NAME_CTR   "Blinking OFF"
//     #define D_NOTIF_MODE_PULSING_OFF_NAME_CTR   "Pulsing ON"
//     #define D_NOTIF_MODE_PULSING_ON_NAME_CTR    "Pulsing OFF"


DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SLOW_GLOW_NAME_CTR)          "Slow Glow"    ;     

DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SEQUENTIAL_NAME_CTR)         "SEQUENTIAL"; 


DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_ALL_NAME_CTR)         "SLOW_FADE_BRIGHTNESS_ALL";
DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_SATURATION_ALL_NAME_CTR)         "SLOW_FADE_SATURATION_ALL";
DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_BRIGHTNESS_RANDOM_NAME_CTR)      "SLOW_FADE_BRIGHTNESS_RANDOM";
DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SLOW_FADE_SATURATION_RANDOM_NAME_CTR)      "SLOW_FADE_SATURATION_RANDOM";
DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_NAME_CTR)             "FLASH_TWINKLE_RANDOM";                   
DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_NAME_CTR)         "FLASH_TWINKLE_SEQUENTIAL";                          
DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_FLASH_GLIMMER_RANDOM_NAME_CTR)             "FLASH_GLIMMER_RANDOM"; // sequential of another colour (white) every x bulbs. ie 20 bulbs total, every 4th part of sequential  

DEFINE_PROGMEM_CTR(PM_FLASHER_REGION_SLOW_FADE_NAME_CTR)                          "SLOW_FADE";
DEFINE_PROGMEM_CTR(PM_FLASHER_REGION_TWINKLE_FLASH_NAME_CTR)                      "TWINKLE_FLASH";

#define D_FLASHER_REGION_COLOUR_SELECT_NAME_CTR "Colour Select"
#define D_FLASHER_REGION_ANIMATE_NAME_CTR "Animate"

DEFINE_PGM_CTR(PM_FLASHER_REGION_COLOUR_SELECT_NAME_CTR) D_FLASHER_REGION_COLOUR_SELECT_NAME_CTR;
DEFINE_PGM_CTR(PM_FLASHER_REGION_ANIMATE_NAME_CTR) D_FLASHER_REGION_ANIMATE_NAME_CTR;


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
      #ifdef USE_WS28XX_METHOD_800KBPS
        typedef Neo800KbpsMethod selectedNeoSpeedType;
      #elif USE_WS28XX_METHOD_BITBANG
        typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
      #elif USE_WS28XX_METHOD_UART800KBPS
        typedef NeoEsp8266Uart800KbpsMethod selectedNeoSpeedType;
      #elif USE_WS28XX_METHOD_I2S1_800KBPS_ESP32
        typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
      #elif USE_WS28XX_METHOD_RMT0_800KBPS_ESP32
        //typedef NeoEsp32Rmt0Ws2811Method selectedNeoSpeedType;
      #else
        //   typedef Neo800KbpsMethod selectedNeoSpeedType;
        #ifdef ENABLE_DEBUG_ESP_DECODER
        typedef NeoEsp8266BitBang800KbpsMethod selectedNeoSpeedType;
        #else
        typedef Neo800KbpsMethod selectedNeoSpeedType;
        #endif
      #endif  
      // Configure
        NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;
      #endif
    #endif
    
    // #ifndef ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE
    // NeoPixelAnimator* animator_controller = nullptr;
    // #endif // ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE

  
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

    /**************
     * BLEND - Move between colours with delay
     * INSTANT   - no delay
     * TWINKLE1 - random bulbs flash
     * GLIMMER - random bulbs blend (flash with blend) - could use progress in animation random(progress)
    **************/ 
    enum TRANSITION_METHOD{
      TRANSITION_METHOD_NONE_ID,
      TRANSITION_METHOD_BLEND_ID,
      TRANSITION_METHOD_INSTANT_ID,
      TRANSITION_METHOD_TWINKLE_ID,
      TRANSITION_METHOD_GLIMMER_ID, //should be an animation only, on brightness #Idea#105
      TRANSITION_METHOD_LENGTH_ID
    };   
    int8_t GetTransitionMethodIDbyName(const char* c);     
    const char* GetTransitionMethodName(char* buffer);
    const char* GetTransitionMethodNameByID(uint8_t id, char* buffer);

    /**************
     * RANDOM - LED pixel indexing chosen at random
     * CENTRE_OUT   - Chosen from center out, showing same pixel on outward pairs (update together)
     * INORDER - first to last pixel index
     * ROTATE - rotate pixel colours along index
    **************/ 
    enum TRANSITION_ORDER{
      TRANSITION_ORDER_NONE_ID=0,
      TRANSITION_ORDER_RANDOM_ID,
      TRANSITION_ORDER_CENTRE_OUT_ID, //mirror image fill from center
      TRANSITION_ORDER_INORDER_ID,
      TRANSITION_ORDER_ROTATE_ID,
      TRANSITION_ORDER_FIXED_ID, //use fMapIDs_Are_Position_HSB_Pairs
      TRANSITION_ORDER_LENGTH_ID
    }; // rotate should really be a flasher animation only
    int8_t GetTransitionOrderIDbyName(const char* c);     
    const char* GetTransitionOrderName(char* buffer);
    const char* GetTransitionOrderNameByID(uint8_t id, char* buffer);
 
    #ifdef USE_PIXEL_ANIMATION_MODE_PIXEL_AMBILIGHT
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

    void SubTask_Ambilight();
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

    /*******USER LIGHTS & FLASHER*********************************************************************************************/

  // #ifdef ENABLE_PIXEL_FUNCTION_FLASHER
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
    FLASHER_FUNCTION_FLASH_TWINKLE_RANDOM_ID, //random leds flash to 100% brightness (modes=instant on/off, multiple pulses)
    FLASHER_FUNCTION_FLASH_TWINKLE_SEQUENTIAL_ID, // sequential flash of white on x leds 
    FLASHER_FUNCTION_FADE_GRADIENT_ID, //single pixels: static, twinkle, pick from palette
    //another flash to "off" or simple set flash colour to off??
    FLASHER_FUNCTION_FLASH_GLIMMER_RANDOM_ID, // tinkle=leds flash independant, glimmer=leds flash with dimming effect on others
    FLASHER_FUNCTION_LENGTH_ID
  };                                   
  //https://www.youtube.com/watch?v=D2IBGlDJ3lg @2.50

  int8_t GetFlasherRegionIDbyName(const char* c);
  const char* GetFlasherRegionName(char* buffer);
  const char* GetFlasherRegionNamebyID(uint8_t id, char* buffer); 
  enum FLASHERREGION{FLASHER_REGION_COLOUR_SELECT_ID=1,
                     FLASHER_REGION_ANIMATE_ID};                    

  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // enable animations (pause)
      uint16_t enable_random_rate : 1;

      // Reserved
      uint16_t reserved : 15;
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


    // Timed numbers
    struct TSAVED{
      uint32_t Update = millis();
      //uint32_t Update2 = millis();
    }tSaved;
    // Function specific animations
    struct FUNCTION_SEQUENTIAL{
      //uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
    }function_seq;
    struct FUNCTION_SLO_GLO{
      uint32_t tSavedNewSpeedUpdate = millis();
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

  }flashersettings;
  void SubTask_Flasher_Main();
  void SubTask_Flasher_Animate();
  void SubTask_Flasher_Animate_Mixer();
  
  void SubTask_Flasher_Animate_Function_Slow_Glow();
  void SubTask_Flasher_Animate_Function_Sequential();
  void SubTask_Flasher_Animate_Function_Twinkle_Random();
  void SubTask_Flasher_Animate_Function_Twinkle_Sequential();
  void SubTask_Flasher_Animate_Function_Slow_Fade_Brightness_All();
  void SubTask_Flasher_Animate_Function_Slow_Fade_Saturation_All();
  void SubTask_Flasher_Animate_Function_Fade_Gradient();

  void parsesub_Flasher(JsonObjectConst obj);
  void init_flasher_settings();
  
  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  #define FLASHER_FUNCTION_MIXER_MAX 10
  enum FLASHER_FUNCTION_MIXER{
    FLASHER_FUNCTION_MIXER_0_ID=0,
    FLASHER_FUNCTION_MIXER_1_ID,
    FLASHER_FUNCTION_MIXER_2_ID,
    FLASHER_FUNCTION_MIXER_3_ID,
    FLASHER_FUNCTION_MIXER_4_ID,
    FLASHER_FUNCTION_MIXER_5_ID,
    FLASHER_FUNCTION_MIXER_6_ID,
    FLASHER_FUNCTION_MIXER_NONE_ID
  };
  #define MIXER_GROUP_MAX 7
  struct MIXER_SETTINGS{
   // #ifdef DEVICE_OUTSIDETREE
    struct MODE{
      uint8_t running_id = FLASHER_FUNCTION_MIXER_0_ID;
      uint16_t time_on_secs[FLASHER_FUNCTION_MIXER_MAX]; // stores which to use
      int16_t time_on_secs_active = 10; //signed
      // char running_friendly_name_ctr[40];
      uint8_t list_len = 0; // 10 max
      struct TIMES{
       // uint16_t enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_MAX]; // if set, this mode will only run if INSIDE the "flashy" time period
        struct datetime flashing_starttime;
        struct datetime flashing_endtime;
        uint8_t skip_restricted_by_time_isactive = 0;
      }times;
    }mode; //active 
    // #endif
    
    uint32_t tSavedMillisToChangeAt = 0;
    uint32_t tSavedTrigger = millis();
    uint32_t tSavedRestart = millis();
    uint8_t enabled = false;
    uint8_t time_scaler = 6;
    uint32_t tSavedSendData;


    //int16_t time_on_secs_active = 10; //signed
    // char running_friendly_name_ctr[100];
    uint8_t running_id = FLASHER_FUNCTION_MIXER_0_ID;
    uint8_t run_time_duration_scaler_as_percentage = 6;
      
    struct GROUPS_SETTINGS{
      int16_t time_on_secs_decounter = -1;
      struct time_short starttime;
      struct time_short endtime;
      uint8_t enable_skip_restricted_by_time = 0;
      uint8_t isenabled = true;
      uint8_t transition_time_sec = 1;
      uint8_t transition_rate_sec = 1;
      uint8_t animation_transition_order = TRANSITION_ORDER_INORDER_ID;
      uint8_t flashersettings_function = FLASHER_FUNCTION_SLOW_GLOW_ID;
      uint16_t run_time_duration_sec = 60;
      uint8_t animation_palette = 0;
    }group[MIXER_GROUP_MAX];

  }mixer;

  void init_mixer_defaults();
  #endif //ENABLE_PIXEL_FUNCTION_MIXER


  //flasher structs for each
  struct FLASH_TWINKLE_RANDOM{
    uint8_t white_on_index;
    uint8_t white_total_index = 0;
    uint8_t white_pixel_amount = 0;
    // #ifdef ENABLE_ADVANCED_MODE_TWINKLE // creating this to reduce "white_leds_index" size
    //   uint16_t white_leds_index[STRIP_SIZE_MAX];
    // #endif
    //HsbColor stored_colours_index[STRIP_SIZE_MAX];///4]; // THIS DOESNT NEED TO BE THIS LONG
    HsbColor flash_colour;
  }flash_twinkle_random;

// #endif

    void ShowStatusViaPixels(uint8_t mode);


    uint8_t fPixelsUpdated = false;
    uint8_t update_all = false;
    uint8_t blocking_force_animate_to_complete = true;
    uint32_t tSavedBlocking_ForceAnimateToComplete = millis();
    
    int8_t Tasker(uint8_t function);
    int8_t Tasker(uint8_t function, JsonObjectConst obj);
    int8_t Tasker_Web(uint8_t function);

    int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
    
    void SubTask_Presets();
    
    void parsesub_TopicCheck_JSONCommand(JsonObjectConst obj);
    void parsesub_ModeManual(JsonObjectConst obj);
    void parsesub_ModeAnimation(JsonObjectConst obj);
    void parsesub_ModeAmbilight(JsonObjectConst obj);

void parse_JSONCommand(JsonObjectConst obj);

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
      TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING
    };
    uint8_t transition_order_random_type = TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING;
    
  
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
    

    
void ApplyBrightnesstoDesiredColour(uint8_t brightness);


void Settings_Load();
void Settings_Save();
void Settings_Default();
void Settings_Reset();
void ConfigureLEDTransitionAnimation(void);
// void UpdateLEDs2(void);
void RefreshLED_Presets(void);
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
void RefreshLEDIndexPattern();
void RefreshLEDOutputStream();
void StripUpdate();
// void SetMode_UpdateColour(uint8_t colour);
void HandlePage_PaletteEditor(AsyncWebServerRequest *request);
// void HandleMixerEditor();
void HandleParameters_RGBLightSettings(AsyncWebServerRequest *request);
void HandlePage_RGBLightSettings(AsyncWebServerRequest *request);
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
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    // const char* postfix_topic_animation = "animation";
    struct handler<mRGBAnimator> mqtthandler_animation_teleperiod;    
    // const char* postfix_topic_ambilight = "ambilight";
    struct handler<mRGBAnimator> mqtthandler_ambilight_teleperiod;
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    // const char* postfix_topic_notifications = "notifications";
    struct handler<mRGBAnimator> mqtthandler_notifications_teleperiod;
    #endif
    // const char* postfix_topic_state = "state";
    struct handler<mRGBAnimator> mqtthandler_state_teleperiod;
    // const char* postfix_topic_flasher = "flasher/animator";
    struct handler<mRGBAnimator> mqtthandler_flasher_teleperiod;
    // const char* postfix_topic_mixer = "flasher/mixer";
    struct handler<mRGBAnimator> mqtthandler_mixer_teleperiod;
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
