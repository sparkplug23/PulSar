#ifndef _ADALIGHTNEOPIXEL_H
#define _ADALIGHTNEOPIXEL_H 7.0

#include "2_CoreSystem/mFirmwareDefaults.h"   // New final firmware defines for modules desired
#include "1_ConfigUser/mFirmwareCustom.h"   // New final firmware defines for modules desired

#ifdef USE_MODULE_LIGHTS_ADDRESSABLE

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
#include "6_Lights/_Interface/palettes.h"
#include "1_ConfigUser/mUserConfig.h"
#include "mRGBAnimator_web.h"
#include "2_CoreSystem/InterfaceController/mInterfaceController.h"

#include "6_Lights/_Interface/mInterfaceLight.h"

#include "AsyncJson.h"
#include "ArduinoJson.h"

#if   defined(USE_WS28XX_FEATURE_3_PIXEL_TYPE)
  typedef NeoRgbFeature selectedNeoFeatureType;
  typedef RgbColor RgbTypeColor;
#elif defined(PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL)   //includes 4th channel of white, pixel order variable still important
  typedef NeoGrbwFeature selectedNeoFeatureType;
  typedef RgbwColor RgbTypeColor;
#else
  typedef NeoGrbFeature selectedNeoFeatureType;
  typedef RgbColor RgbTypeColor;
#endif



// TRANSITION_METHOD
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_BLEND_NAME_CTR)    "Blend";      
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_INSTANT_NAME_CTR)  "Instant";      
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_TWINKLE_NAME_CTR)  "Twinkle";     
DEFINE_PROGMEM_CTR(PM_TRANSITION_METHOD_GLIMMER_NAME_CTR)  "Glimmer";

// order is maybe becoming flasher? mode ie applied animations          //new flasher name
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_BLEND_NAME_CTR)      "Blend";   //slow_glow
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_RANDOM_NAME_CTR)      D_JSON_RANDOM;  //random
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR) "Centre Out" ;   // work out flasher for half, then mirror image  
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_INORDER_NAME_CTR)    D_JSON_INORDER;   //none
DEFINE_PROGMEM_CTR(PM_TRANSITION_ORDER_ROTATE_NAME_CTR)     "Rotate";  //rotate
    
DEFINE_PROGMEM_CTR(PM_ANIMATION_MODE_NONE_NAME_CTR )   "None"     ;    
DEFINE_PROGMEM_CTR(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR   )      "Ambilight"  ;                
DEFINE_PROGMEM_CTR(PM_ANIMATION_MODE_SCENE_NAME_CTR   )          "Scene"      ;             
DEFINE_PROGMEM_CTR(PM_ANIMATION_MODE_FLASHER_NAME_CTR  )         D_JSON_FLASHER;
#ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS       
DEFINE_PROGMEM_CTR(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)   D_JSON_NOTIFICATIONS;      
#endif   

    
//     #define D_SCENES_NOTACTIVE_NAME_CTR       "NOTACTIVE"    
//     #define D_SCENES_DAYON_NAME_CTR           "DAYON"    
//     #define D_SCENES_DAYOFF_NAME_CTR          "DAYOFF"    
//     #define D_SCENES_EVENINGON_NAME_CTR       "EVENINGON"    
//     #define D_SCENES_EVENINGOFF_NAME_CTR      "EVENINGOFF"    
//     #define D_SCENES_MIDNIGHTON_NAME_CTR      "MIDNIGHTON"    
//     #define D_SCENES_MIDNIGHTOFF_NAME_CTR     "MIDNIGHTOFF"    
DEFINE_PROGMEM_CTR(PM_SCENES_COLOURSCENE_NAME_CTR)     "COLOURSCENE"    ; //PGM_P
//     #define D_SCENES_FLASHCOLOUR_NAME_CTR     "FLASHCOLOUR"    
//     #define D_SCENES_SUNRISE_SINGLE_NAME_CTR  "SUNRISE_SINGLE"  
//     #define D_SCENES_SUNRISE_DUAL_NAME_CTR    "SUNRISE_DUAL" 

    
//     #define D_NOTIF_MODE_OFF_NAME_CTR           "OFF"
//     #define D_NOTIF_MODE_STATIC_OFF_NAME_CTR    "Static OFF"
//     #define D_NOTIF_MODE_STATIC_ON_NAME_CTR     "Static ON"
//     #define D_NOTIF_MODE_BLINKING_OFF_NAME_CTR  "Blinking ON"
//     #define D_NOTIF_MODE_BLINKING_ON_NAME_CTR   "Blinking OFF"
//     #define D_NOTIF_MODE_PULSING_OFF_NAME_CTR   "Pulsing ON"
//     #define D_NOTIF_MODE_PULSING_ON_NAME_CTR    "Pulsing OFF"



DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SLOW_GLOW_NAME_CTR    )          "Slow Glow"    ;     

DEFINE_PROGMEM_CTR(PM_FLASHER_FUNCTION_SEQUENTIAL_NAME_CTR) "SEQUENTIAL"; 


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

DEFINE_PROGMEM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)                      "GRB";
DEFINE_PROGMEM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)                      "RGB";
DEFINE_PROGMEM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)                      "BRG";
DEFINE_PROGMEM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)                      "RBG";


void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total);


class mRGBAnimator{
  private:
    // RgbTypeColor needs to be a pointer type so I can pass as single byte 3/4/5 colour types
    void SetPixelColor(uint16_t indexPixel, RgbTypeColor color);
    RgbTypeColor GetPixelColor(uint16_t indexPixel);
  public:
    //#define D_MODULE_TOPIC_NAME "pixels"
    mRGBAnimator(){};
    void pre_init(void);
    void init(void);

    

    enum PIXEL_HARDWARE_COLOR_ORDER_IDS{
      PIXEL_HARDWARE_COLOR_ORDER_GRB_ID = 0, //default
      PIXEL_HARDWARE_COLOR_ORDER_RGB_ID, //common for WS2811
      PIXEL_HARDWARE_COLOR_ORDER_BRG_ID, 
      PIXEL_HARDWARE_COLOR_ORDER_RBG_ID, 
      PIXEL_HARDWARE_COLOR_ORDER_BGR_ID, 
      PIXEL_HARDWARE_COLOR_ORDER_GBR_ID,
      PIXEL_HARDWARE_COLOR_ORDER_GRBW_ID,
      PIXEL_HARDWARE_COLOR_LENGTH_ID
    };
    
    const char* GetHardwareColourTypeName(char* buffer);
    const char* GetHardwareColourTypeNameByID(uint8_t id, char* buffer);
    int8_t GetHardwareColourTypeIDbyName(const char* c);

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
  
    // Group of ifndef's to allow defaults to be set, and users to set defaults using basic numbers
    #ifndef STRIP_SIZE_MAX
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
      #define ANIMATOR_SIZE_MAX STRIP_SIZE_MAX
    #endif
    uint16_t strip_size = STRIP_SIZE_MAX; //allow variable control of size
    uint16_t animator_strip_size = ANIMATOR_SIZE_MAX; //allow variable control of size

    uint16_t GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage);
    uint8_t  GetPixelsToUpdateAsPercentageFromNumber(uint16_t number);
    
    struct SETTINGS{
      uint8_t pixel_hardware_color_order_id = PIXEL_HARDWARE_COLOR_ORDER_RGB_ID;
      struct FLAGS{
          uint8_t EnableSceneModeWithSliders = true;
          uint8_t TemplateProvidedInProgMem = false;
          uint8_t EnableModule = false;
      }flags;
    }settings;
  
    /**************
     * BLEND - Move between colours with delay
     * INSTANT   - no delay
     * TWINKLE1 - random bulbs flash
     * GLIMMER - random bulbs blend (flash with blend) - could use progress in animation random(progress)
    **************/ 
    enum TRANSITION_METHOD{
      TRANSITION_METHOD_BLEND_ID,
      TRANSITION_METHOD_INSTANT_ID,
      TRANSITION_METHOD_TWINKLE_ID,
      TRANSITION_METHOD_GLIMMER_ID, //should be an animation only, on brightness #Idea#105
      TRANSITION_METHOD_NONE_ID,
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

    /**************
     * TURN_ON - fade ON, returns to previous lighting array
     * TURN_OFF - fade off, either turns off, or calls "SCENE" to set number = ALSO SAVES CURRENT OUTPUT
     * NOTHING - to be named "off", does nothing, simply remains off
     * TIMED  - stores exstruction for later execution, ie set on, with auto off - add scene turn off feature "offtimer"
     * AMBILIHGT - keep, dell screen, set directly
     * PRESETS - animation from a group of colour patterns
     * FLASHER - christmad themes, standard 8, shimmering/twinkling 
     * SCENE - scene names might be same as presets? or preset subset of scene? scene might best be named "direct"
    **************/ 
    enum ANIMATION_MODE{
      ANIMATION_MODE_NONE_ID = 0,
      ANIMATION_MODE_TURN_ON_ID,
      ANIMATION_MODE_TURN_OFF_ID,
      ANIMATION_MODE_AMBILIGHT_ID,
      ANIMATION_MODE_SCENE_ID,
      ANIMATION_MODE_FLASHER_ID,
      //user option, cant comment out but can disable elsewhere
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
        ANIMATION_MODE_NOTIFICATIONS_ID,
      #endif
      ANIMATION_MODE_LENGTH_ID
    };             
    int8_t GetAnimationModeIDbyName(const char* c);
    const char* GetAnimationModeName(char* buffer);
    const char* GetAnimationModeNameByID(uint8_t id, char* buffer);
  
    /**************
     * Predefined SCENES by name
     * SCENES are colours defined as one colour for all pixels
    **************/ 
    enum SCENES{
      SCENES_NOTACTIVE_ID=0,
      SCENES_DAYON_ID=1,
      SCENES_DAYOFF_ID,
      SCENES_EVENINGON_ID,
      SCENES_EVENINGOFF_ID,
      SCENES_MIDNIGHTON_ID,
      SCENES_MIDNIGHTOFF_ID,
      SCENES_COLOURSCENE_ID,
      SCENES_FADE_OFF_ID,
      SCENES_FLASHCOLOUR_ID,
      SCENES_WHITEON_ID,
      SCENES_SUNRISE_SINGLE_ID,     // From dark blue to sky blue (cyan)
      SCENES_SUNRISE_DUAL_ID,       // From dark blue, sunrise orange, sky blue (cyan)
      SCENES_SUNRISE_AMBILIGHT_ID,
      SCENES_LENGTH_ID
    };
    int8_t GetSceneIDbyName(const char* c);
    const char* GetSceneName(char* buffer);

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

    /**************
     * ANIMATION_PROFILE - As function argument of "SetAnimationProfile"
     * This sets numerous aspects of animation struct to cause set animations to happen:
     * TURN_OFF - will fade leds off
     * TURN_ON  - will fade leds on
    **************/ 
    enum ANIMATION_PROFILE_IDS{
      ANIMATION_PROFILE_TURN_OFF_ID=0,
      ANIMATION_PROFILE_TURN_ON_ID,
      ANIMATION_PROFILE_NOTHING_ID,
      ANIMAITON_PROFILE_LENGTH_ID    
    };
    void SetAnimationProfile(uint8_t profile_id);

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS

    const char* GetColourMapNamebyID(uint8_t id, char* buffer);
    const char* GetColourMapNameNearestbyColour(HsbColor c, char* buffer);
    int8_t      GetColourMapIDbyName(const char* c);

    #define PRESET_COLOUR_MAP_INDEXES_MAX COLOUR_MAP_LENGTH_ID 
    // // #ifdef DONT_USE_OLD_PRESETS
    // HsbColor preset_colour_map[PRESET_COLOUR_MAP_INDEXES_MAX]; //save in eeprom?
    // // #endif
    // void init_ColourMap();

    void WebSave_RGBColourSelector(void);


    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    void init_NotificationPanel();
    #endif
    void init_Scenes();

    int16_t FindNearestValueIndexUInt8(uint8_t* tosearch, uint8_t tosearch_len, uint8_t tofind);
    int32_t FindNearestValueIndexUInt16(uint16_t* tosearch, uint16_t tosearch_len, uint16_t tofind);

    uint16_t changeUIntScale(uint16_t inum, uint16_t ifrom_min, uint16_t ifrom_max,
                                       uint16_t ito_min, uint16_t ito_max) ;

                                       

    /*******AMBILIGHT*********************************************************************************************/

    NeoPixelAnimator* animations_control = NULL;

    uint8_t fLEDStripOnOff = false;
    uint8_t fLEDStripOnOff_Saved = false;
    uint8_t fPixelsUpdated = false;
    void SetRGBwithNeoPixel();

    uint8_t blocking_force_animate_to_complete = true;
    uint32_t tSavedBlocking_ForceAnimateToComplete = millis();


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
    
uint8_t update_all = false;

    /*******USER LIGHTS & FLASHER*********************************************************************************************/

// #ifdef USE_TASK_RGBLIGHTING_FLASHER_AND_MIXER
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
      uint8_t array[10];   //use different array profiles, "linear ramp", "exponentialhigh", "exp low" (lingers on low)

      // uint8_t profile_id;

      uint32_t tSavedNewSpeedUpdate = millis();
    }random_transitions;


    // Timed numbers
    struct TSAVED{
      uint32_t Update = millis();
      uint32_t Update2 = millis();
    }tSaved;
    // Function specific animations
    struct FUNCTION_SEQUENTIAL{
      uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
    }function_seq;
    struct FUNCTION_SLO_GLO{
      uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
    }function_slo_glo;
    struct FUNCTION_SLOW_FADE{
      uint32_t tSavedNewSpeedUpdate = millis();
      uint8_t rate_index = 0;
      uint8_t direction = 0;
    }function_slow_fade;

    struct FUNCTION_FLASH_TWINKLE{
      uint32_t tSavedNewSpeedUpdate = millis();
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

  int8_t parsesub_Flasher(JsonObjectConst obj);
  void init_flasher_settings();

  
  #define FLASHER_FUNCTION_MIXER_MAX 10
  enum FLASHER_FUNCTION_MIXER{FLASHER_FUNCTION_MIXER_0_ID=0,
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
      char running_friendly_name_ctr[40];
      uint8_t list_len = 0; // 10 max
      struct TIMES{
        uint16_t enable_skip_restricted_by_time[FLASHER_FUNCTION_MIXER_MAX]; // if set, this mode will only run if INSIDE the "flashy" time period
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
    char running_friendly_name_ctr[100];
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


  //flasher structs for each
  struct FLASH_TWINKLE_RANDOM{
    uint8_t white_on_index;
    uint8_t white_total_index = 0;
    uint8_t white_pixel_amount = 0;
    #ifdef ENABLE_ADVANCED_MODE_TWINKLE // creating this to reduce "white_leds_index" size
      uint16_t white_leds_index[STRIP_SIZE_MAX];
    #endif
    HsbColor stored_colours_index[STRIP_SIZE_MAX];///4]; // THIS DOESNT NEED TO BE THIS LONG
    HsbColor flash_colour;
  }flash_twinkle_random;

// #endif

    void ShowStatusViaPixels(uint8_t mode);

    //overload
    // int8_t Tasker(uint8_t function, uint8_t param1);
    
    // template<typename T>
    // int8_t Tasker(uint8_t function, T param1);

    
    int8_t Tasker(uint8_t function);
    int8_t Tasker(uint8_t function, JsonObjectConst obj);

    
int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
    

//     int8_t mRGBAnimator::Tasker(uint8_t function, uint8_t param1){  
//   JsonObjectConst dummy; Tasker(function, dummy);
// }

    int8_t Tasker_Web(uint8_t function);


    void SubTask_Scenes();
    void SubTask_Presets();

    void WebAppend_Root_Status_Table();
    
    int8_t parsesub_JSONCommand(JsonObjectConst obj);
    int8_t parsesub_ModeManual(JsonObjectConst obj);
    int8_t parsesub_ModeAnimation(JsonObjectConst obj);
    int8_t parsesub_ModeScene(JsonObjectConst obj);
    int8_t parsesub_ModeAmbilight(JsonObjectConst obj);

        // make number generator, random, with skewness
        // int Range0to100[20] = {0,10,20,30,40, // random(12,24)*10;
        //                 50,60,70,80,90,
        //                 91,92,93,94,95,
        //                 96,97,98,99,100};


    float rainbowpartialprogress = 0;

    void Ws2812Init();
    
    // Default values if not specified

    
    // #ifdef USE_WS28XX_FEATURE_GRBW
    //   void FadeToNewColour(RgbwColor newcolor, uint16_t _time_to_newcolour = 1000, RgbwColor fromcolor = RgbwColor(0,0,0,0));
    // #else
      void FadeToNewColour(RgbTypeColor newcolor, uint16_t _time_to_newcolour = 1000, RgbTypeColor fromcolor = RgbTypeColor(0));
    // #endif

const char* GetAnimationStatusCtr(char* buffer);

    uint32_t tSavedSendData, tSavedSendData2, fSendRGBDellStatus;
    uint8_t testvariable=0;
    uint8_t testvariable2=0,testvariable3=0;
    uint8_t timerenabled = false;

    int blend_count=0;
    enum BLEND_STEPS{BLEND_START=1,
                     BLEND_DELAYS,
                     BLEND_DONE
    };
    uint8_t blend_state;uint32_t tTimeout;

    uint32_t tSavedCheckAnyLEDsOnOff = 0;
    uint16_t fAnyLEDsOnOffCount = 0;


    // struct TSAVEDTEST{
    //   uint32_t millis_saved;
    //   uint32_t trigger; // send early
    // }tSavedCheckAnyLEDsOnOff;
    
    // struct TIMEREACHED_SAVED tSavedCheckAnyLEDsOnOfftest;

    struct LEDOUTPUTSETTINGS{
      uint16_t length = 0;
      uint16_t index = 0;
      uint16_t pattern[STRIP_SIZE_MAX];
    }ledout;


    struct PALETTE_VIEW_SETTINGS{
      uint8_t show_type = 2;//RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID;
      uint8_t height_as_percentage = 15;
      uint8_t pixel_resolution_percentage = 100; //100% is send all, 0-99 is percentage of all

    }palette_view;
    void WebSendBody_Palette();
    void WebSendBody_Liveview();
    void WebSendBody_InfoTable();

    
void EveryLoop();

void WebPage_Root_SendStyle();
void WebPage_Root_SendBody();
void WebPage_Root_SendStatus();
void WebAppend_Root_Add_Main_Buttons();
void WebPage_Root_AddHandlers();
void WebPage_Root_SendParseJSONScripts();
void WebPage_Root_SendScripts();
void WebPage_Root_SendInformationModule();
void WebSendBody_InfoTable2();
void WebSendBody_Palette2();

    // totally random, or random but not repeating
    enum TRANSITION_ORDER_RANDOM_METHOD_IDS{
      TRANSITION_ORDER_RANDOM_METHOD_REPEATING = 0,
      TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING
    };
    uint8_t transition_order_random_type = TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING;
    
    void ConfigureLEDTransitionAnimation(void);
    void UpdateLEDs2(void);
    void RefreshLED_Presets(void);

    int8_t parsesub_CheckAll(JsonObjectConst obj);

    void SubTask_TimedLEDMode();
    void TurnLEDsOff();
    void AddToJsonObject_AddHardware(JsonObject root);
    void Append_Hardware_Status_Message();

    HsbColor hsbcolour[STRIP_SIZE_MAX];

    void Settings_Load();
    void Settings_Save();
    void Settings_Default();
    void Settings_Reset();

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
uint8_t time_map_secs[TIME_MAP_SECS_LENGTH_ID] = {0,1,2,4,6,10,15,20,30,60};

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
uint8_t rate_map_secs[RATE_MAP_SECS_LENGTH_ID] = {0,1,2,4,6,10,15,20,30,60};

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
uint8_t pixels_to_update_as_percentage_map[PIXELS_UPDATE_PERCENTAGE_LENGTH_ID] =
   {0,5,10,15,20,30,40,50,60,70,80,90,100};
   // NOT NEEDED as array, if I simply set via enums and loop over them


// //phase this out, I only want to record as percentage but can calculate each time what "as number" is
//     uint8_t pixels_to_update_as_number_map[11] = {0,1,2,3,4,5,6,7,8,9,10}; //to be calculated in init      
   
   void SetRGBwithNeoPixelImageTest();




// New feature, turning off will overwrite animation struct on "on" will restore to previous

  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // enable animations (pause)
      uint16_t fEnable_Animation : 1;


      uint16_t fForceUpdate : 1;
      uint16_t fRunning : 1;//= false;
      uint16_t fEndUpdatesWhenAnimationCompletes : 1;// = false;
      
        uint16_t ftime_use_map : 1;//= true;
        uint16_t frate_use_map : 1;//= true;

      // Reserved
      uint16_t reserved : 12;
    };
  } ANIMATION_FLAGS;

    struct ANIMATIONSETTINGS{
      //mode = turn on/off/scene/pallette
      uint8_t mode_id; //update mode
      //palette - stores colour patterns for single led setting
      uint8_t palette_id; 

      uint8_t pixelgrouped=3; // nearby pixels repeat colours
      
      float brightness = 1; // MOVE OUT OF ANIMATION?? move to "light class"

      
      uint16_t auto_time_off_secs = 0;
      uint32_t tSavedAutoOff = 0;

      ANIMATION_FLAGS flags;

      struct TRANSITIONSETTINGS{
        uint8_t order_id;
        uint8_t method_id;

        struct PIXELS_TO_UPDATE_AS_PERCENTAGE{
          uint8_t val;
          uint8_t map_id = 2;  
        }pixels_to_update_as_percentage;
        struct TIME{
          uint16_t val = 5000;
          uint8_t  map_id = 2;
        }time_ms;
        struct RATE{
          uint16_t val = 5000;
          uint8_t  map_id = 2;
        }rate_ms;
      
      }transition;
    };
    // store the current state
    ANIMATIONSETTINGS animation;
    // store animation to swtich to
    ANIMATIONSETTINGS animation_stored;
    // hold when to switch to animation_stored
    struct ANIMATION_CONFIG_RUNTIME{
      uint8_t  fWaiting = false;
      // For ms or sec delay (until execution) time
      uint32_t delay_time_ms   = 0; //0 if not set
      uint32_t delay_time_secs = 0;
    }animation_stored_runtime;

    
    uint32_t animation_changed_millis = 0;


    // Flags and states that are used during one transition and reset when completed
    struct ANIMATIONOVERRIDES{
      uint8_t fRefreshAllPixels = false;
      uint16_t time_ms = 1000; //on boot
    }animation_override;

    void SetRefreshLEDs();
    
    void GenerateAnimationPixelAmountMaps();

    int8_t parse_JSONModuleSettings(JsonObject obj);

    void SetAnimationBrightness(uint8_t brt);


    enum SCENE_PARTS{STEP1=1,STEP2,STEP3,STEP4,STEP5,DONE};
    struct SCENES_CONFIG{
      uint8_t name_id;

      uint8_t colour_255[6];


      HsbColor colour;
      uint8_t colourW; // save white channel always, even when not needed for struct size compatability
      
      
      
      uint8_t fActive = false;
      uint8_t parts = DONE;
      uint32_t tStart;
      uint32_t tOnTime;
      uint16_t auto_time_off_secs = 0;
      uint32_t tSavedAutoOff = 0;
    };
    // hold current set scene
    struct SCENES_CONFIG scene;
    // hold scene to switch to later
    struct SCENES_CONFIG scene_stored;
    // hold when to switch to scene_stored
    struct SCENE_CONFIG_RUNTIME{
      uint8_t  fWaiting = false;
      // For ms or sec delay (until execution) time
      uint32_t delay_time_ms   = 0; //0 if not set
      uint32_t delay_time_secs = 0;
    }scene_stored_runtime;




    float    HueN2F(uint16_t hue);
    float    SatN2F(uint8_t sat);
    float    BrtN2F(uint8_t brt);
    uint16_t HueF2N(float hue);
    uint8_t  SatF2N(float sat);
    uint8_t  BrtF2N(float brt);

    uint32_t tSavedUpdateRGBString;
    uint8_t first_set;
    uint8_t hue_preset1_idx;
    uint16_t rotate_start_index = 0;

    
    // This value holds the brightness of the string and is shared by animations and scene selection.
    // When scene is set, this will also be set automatically.
    float brightness_as_percentage = 100;

    //types RGB/HSB, Index/not


void init_Animations();



  // uint8_t test[20];




// template <class ForwardIterator>
//   void rotate (ForwardIterator first, ForwardIterator middle,
//                ForwardIterator last)
// {
//   ForwardIterator next = middle;
//   while (first!=next)
//   {
//     swap (*first++,*next++);
//     if (next==last) next=middle;
//     else if (first==middle) middle=next;
//   }
// }

// #include <algorithm.h>    // std::rotate








        HsbColor GetColourFromMapUsingType(
          uint16_t pixel_I_want,
          uint8_t *colour_index, uint16_t colour_array_length, uint16_t fMapIDs_Type, //inputs
          uint16_t* total_hsbcolours_in_map, uint16_t* pixel_index_out, uint16_t* pixel_width_including_indexing //returns
          );


      void WebAppend_JSON_RootPage_LiveviewPixels();



      void WebSend_DivStyle_Root(AsyncWebServerRequest *request);
      void WebSend_JSON_Append_Root();
      void WebSend_JSON_RootPage_Parameters_RGBTable(AsyncWebServerRequest *request);
      void WebSend_JSON_RootPage_Parameters_RGBPalette(AsyncWebServerRequest *request);
      void WebPage_Root_SendJSONFetchURL();
      void SendScript_Root(AsyncWebServerRequest *request);
      void SendStyle_Root(AsyncWebServerRequest *request);
      void SendDrawDiv_Root(AsyncWebServerRequest *request);

      void Web_RGBLightSettings_RunTime_URLs(AsyncWebServerRequest *request);
      void Web_RGBLightSettings_LoadTime_URLs(AsyncWebServerRequest *request);
      void Web_RGBLightSettings_Draw(AsyncWebServerRequest *request);
      void Web_RGBLightSettings_LoadScript(AsyncWebServerRequest *request);
      void Web_RGBLightSettings_RunTimeScript(AsyncWebServerRequest *request);
      void Web_RGBLightSettings_FillOptions(AsyncWebServerRequest *request);
      void WebAppend_RGBLightSettings_FillOptions_Controls();
      void WebAppend_RGBLightSettings_FillOptions_Controls_Selected();
      void WebAppend_RGBLightSettings_Draw_Controls();

      void Web_RGBPaletteEditor_RunTime_URLs(AsyncWebServerRequest *request);
      void Web_RGBPaletteEditor_LoadTime_URLs(AsyncWebServerRequest *request);
      void Web_RGBPaletteEditor_Draw(AsyncWebServerRequest *request);
      void Web_RGBPaletteEditor_LoadScript(AsyncWebServerRequest *request);
      void Web_RGBPaletteEditor_RunTimeScript(AsyncWebServerRequest *request);
      void Web_RGBPaletteEditor_FillOptions(AsyncWebServerRequest *request);
      void WebAppend_RGBPaletteEditor_FillOptions_Controls();
      void WebAppend_RGBPaletteEditor_FillOptions_Controls_Selected();
      void WebAppend_RGBPaletteEditor_Draw_Controls();


      void Web_Root_Draw_PaletteSelect(AsyncWebServerRequest *request);
    void Web_RGBLightSettings_UpdateURLs(AsyncWebServerRequest *request);


void Web_PaletteEditor_UpdateURLs(AsyncWebServerRequest *request);

void WebAppend_RGBLightSettings_Draw_Animation_Options();
void WebAppend_RGBLightSettings_Draw_Flasher_Options();

void Web_RGBLightSettings_ListOptions(AsyncWebServerRequest *request);

void Web_PaletteEditor_ListOptions(AsyncWebServerRequest *request);
void WebAppend_PaletteEditor_FillOptions_Controls();

void WebAppend_RGBLightSettings_Draw_PageButtons();

void WebAppend_Root_Draw_Table();
      void WebAppend_Root_Draw_RGBTable();
      void WebAppend_Root_Draw_RGBLive();
      void WebAppend_Root_Draw_RGBPalette();
      void WebAppend_Root_Draw_PaletteSelect();

      void WebAppend_Root_Draw_PaletteSelect_Placeholder();
void Web_PaletteEditor_Draw(AsyncWebServerRequest *request);

void WebAppend_PaletteEditor_Draw_Editor_Form();

void WebAppend_Root_ControlUI();


      void SendDrawDiv_Root2(AsyncWebServerRequest *request);

      void SendDrawDiv_Root3(AsyncWebServerRequest *request);

      // For debugging only
      struct FREEMEM_USAGE{
        uint16_t WebSend_JSON_RootPage_LiveviewPixels;
      }freemem_usage;

      void WebSend_JSON_RootPage_Palette_AJ(AsyncWebServerRequest* request);

      void WebSend_JSON_RootPage_Parameters_Liveview(AsyncWebServerRequest *request);

      void WebSend_JSON_RGBTable(AsyncWebServerRequest *request);

    HsbColor GetRandomColour(HsbColor colour1, HsbColor colour2);

    
    struct JSON_RESPONSES{

      timereached_t livepixels;
      
    }json_responses;

    void WebSend_JSON_Empty_Response();

    void RefreshLEDIndexPattern();
    void   RefreshLEDOutputStream();

    void StripUpdate();
    struct POWER_RATING{
      float Average_mA_Per_Pixel_Step = 0.0784; //  20/255 
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

    uint16_t LEDp, LEDe, desired_pixel; //pixel and element

    // enum FSTATE{ON=1,OFF=2};
    struct TIMEDON{
      int16_t secs=60;
      uint32_t tick = millis();
      uint8_t fState = 0;
      uint8_t fEnabled = true;
      uint8_t fUpdated = true;
      uint8_t fForceUpdate = true;
    }timedon;

    uint8_t kitchen_preset_set_id;
    struct SCENE_PRESETS{
      HsbColor colour; 
      uint16_t time_ms=1000;
    };
    //change to scenes users
    struct SCENE_PRESETS scene_preset_dayon;
    struct SCENE_PRESETS scene_preset_dayoff;
    struct SCENE_PRESETS scene_preset_eveningon;
    struct SCENE_PRESETS scene_preset_eveningoff;
    struct SCENE_PRESETS scene_preset_nighton;
    struct SCENE_PRESETS scene_preset_nightoff;
    struct SCENE_PRESETS* presenttmp;

    void SetMode_UpdateColour(uint8_t colour);

    uint8_t fRxRGBStringToParseWaiting;
    void setfRxRGBStringToParseWaiting(uint8_t state);
    uint8_t getfRxRGBStringToParseWaiting(void);
    uint8_t getclearfRxRGBStringToParseWaiting(void);

    #define WEB_HANDLE_ANIMATION_BRIGHTNESS_SLIDER "animation_brightness"

    #define WEB_HANDLE_SCENE_COLOUR_WHITE_SLIDER "scn_sld_w"


void AddToHardwareMessage();



  void HandlePage_PaletteEditor(AsyncWebServerRequest *request);
  
void BuildPage_RGBPaletteEditor(char** buffer);

  void HandleMixerEditor();

  // Page RGBLightSettings
  void HandleParameters_RGBLightSettings(AsyncWebServerRequest *request);
  void HandlePage_RGBLightSettings(AsyncWebServerRequest *request);
  void BuildPage_RGBLightSettings(char** buffer);


void WebAppend_Root_RGBPalette();

  

  void HandleTimerConfiguration();
  void TimerSaveSettings();

  void WebSend_JSON_RootPage_Parameters();
  void WebSend_JSON_RGBPalette();
  void WebSend_JSON_RootPage_Parameters_Append_Palette(JsonObject root);
  void WebSend_JSON_RootPage_Parameters_Append_Table(JsonObject root);

  
  int8_t GetNearestColourMapIDFromColour(HsbColor hsb);
  
  void WebSave_RGBColourSelectorOnly();
  void WebSave_RGBControls(AsyncWebServerRequest *request);

  #define USE_APPENDED_COLOUR_VALUES_TO_EDITOR_LIST
  
  // struct PALETTELIST::PALETTE* mRGBAnimator::GetPaletteListPtrFromID(uint8_t id){
  
  void WebCommand_Parse(void);

  // #define WEB_HANDLE_TIMER "tm"

  
    enum WEBHANDLE_RGBCONTROLS_ITEM_IDS{
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELSUPDATED_PERCENTAGE=0,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_PIXELORDER,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_RATE,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_PERIOD,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_TRANSITIONMETHOD,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_ANIMATIONMODE,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_PALETTE,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_FLASHER,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_MIXER_RUNNING_ID,
      WEBHANDLE_RGBCONTROLS_ITEM_IDS_NONE
    };

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0);
    uint8_t ConstructJSON_Animation(uint8_t json_level = 0);
    uint8_t ConstructJSON_Ambilight(uint8_t json_level = 0);
    #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    uint8_t ConstructJSON_Notifications(uint8_t json_level = 0);
    #endif
    uint8_t ConstructJSON_Scene(uint8_t json_level = 0);
    uint8_t ConstructJSON_State(uint8_t json_level = 0);
    uint8_t ConstructJSON_Timed(uint8_t json_level = 0);
    uint8_t ConstructJSON_Flasher(uint8_t json_level = 0);
    uint8_t ConstructJSON_Mixer(uint8_t json_level = 0);

  #ifdef USE_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mRGBAnimator>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    const char* postfix_topic_settings = "settings\0";
    struct handler<mRGBAnimator> mqtthandler_settings_teleperiod;

    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
      #endif
      MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_TIMED_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_FLASHER_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    const char* postfix_topic_animation = "animation\0";
    struct handler<mRGBAnimator> mqtthandler_animation_teleperiod;
    
    const char* postfix_topic_ambilight = "ambilight\0";
    struct handler<mRGBAnimator> mqtthandler_ambilight_teleperiod;
    
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    const char* postfix_topic_notifications = "notifications\0";
    struct handler<mRGBAnimator> mqtthandler_notifications_teleperiod;
  #endif

    const char* postfix_topic_scene = "scene\0";
    struct handler<mRGBAnimator> mqtthandler_scene_teleperiod;

    const char* postfix_topic_state = "state\0";
    struct handler<mRGBAnimator> mqtthandler_state_teleperiod;

    const char* postfix_topic_timed = "timed\0";
    struct handler<mRGBAnimator> mqtthandler_timed_teleperiod;

    const char* postfix_topic_flasher = "flasher/animator\0";
    struct handler<mRGBAnimator> mqtthandler_flasher_teleperiod;

    const char* postfix_topic_mixer = "flasher/mixer\0";
    struct handler<mRGBAnimator> mqtthandler_mixer_teleperiod;

  #endif


  
    /*******NOTIFICATIONS*********************************************************************************************/

  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
    #ifndef STRIP_NOTIFICATION_SIZE
      #define STRIP_NOTIFICATION_SIZE (STRIP_SIZE_MAX>20?20:STRIP_SIZE_MAX) // Set max if not defined as 20
    #endif

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
        HsbColor colour; // colour of the led
        #ifdef PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
          uint8_t colourWhite;
        #endif
        uint32_t tSavedUpdate; // millis last updated
        uint16_t tRateUpdate = 10; // time between updating
        uint16_t auto_time_off_secs = 0; // reset pixel to off
      }pixel[STRIP_NOTIFICATION_SIZE];
    }notif;
    int8_t parsesub_NotificationPanel(JsonObjectConst _obj);
#endif




};

#endif

#endif
