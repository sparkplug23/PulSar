#ifndef _DRIVER_INTERFACE_LIGHTS_H
#define _DRIVER_INTERFACE_LIGHTS_H 0.1

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE


// #ifndef USE_MODULE_LIGHTS_ADDRESSABLE
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>
// #endif //  USE_MODULE_LIGHTS_ADDRESSABLE 


#if   defined(USE_WS28XX_FEATURE_3_PIXEL_TYPE)
  typedef RgbColor RgbTypeColor;
#elif defined(PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL)
  typedef RgbwColor RgbTypeColor;
#else
  typedef RgbColor RgbTypeColor;
#endif

// TRANSITION_METHOD
DEFINE_PGM_CTR(PM_TRANSITION_METHOD_NONE_NAME_CTR)    "None";   
DEFINE_PGM_CTR(PM_TRANSITION_METHOD_BLEND_NAME_CTR)    "Blend";      
DEFINE_PGM_CTR(PM_TRANSITION_METHOD_INSTANT_NAME_CTR)  "Instant";      
DEFINE_PGM_CTR(PM_TRANSITION_METHOD_TWINKLE_NAME_CTR)  "Twinkle";     
DEFINE_PGM_CTR(PM_TRANSITION_METHOD_GLIMMER_NAME_CTR)  "Glimmer";


// order is maybe becoming flasher? mode ie applied animations          //new flasher name
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_NONE_NAME_CTR) "None";
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_BLEND_NAME_CTR)      "Blend";   //slow_glow
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_RANDOM_NAME_CTR)      D_JSON_RANDOM;  //random
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_CENTRE_OUT_NAME_CTR) "Centre Out" ;   // work out flasher for half, then mirror image  
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_INORDER_NAME_CTR)    D_JSON_INORDER;   //none
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_ROTATE_NAME_CTR)     "Rotate";  //rotate
DEFINE_PGM_CTR(PM_TRANSITION_ORDER_FIXED_NAME_CTR)     "Fixed";  //rotate
    

DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR)  
  "{o1}%d'>%s{o2}";   
DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM)
  "{o1}%d'>%d{o2}";


DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM_NUM)  
  "{o1}%d'>%d{o2}";  
DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_NUM_CTR)  
  "{o1}%d'>%s{o2}";   
DEFINE_PGM_CTR(PM_HTTP_OPTION_SELECT_TEMPLATE_REPLACE_CTR_CTR)  
  "{o1}%s'>%s{o2}";   


DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR)        D_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR;
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR)                      "RGB";
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_BRG_CTR)                      "BRG";
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_COLOR_ORDER_RBG_CTR)                      "RBG";


// struct RgbcctColor{
//   uint8_t R;
//   uint8_t G;
//   uint8_t B;
//   uint8_t WW;
//   uint8_t WC;
// };

enum LightTypes_IDS{
  LT_BASIC, //relay?
  LT_PWM1,    LT_PWM2, LT_PWM3,   LT_PWM4,  LT_PWM5,  LT_PWM6, LT_PWM7,
  LT_NU8,   LT_SERIAL1, LT_SERIAL2,   
  LT_LIGHT_INTERFACE_END,
  // Anything after this will not be handled in lightinterface
  LT_WS2812, 
  LT_ADDRESSABLE, //replacing above
  LT_RGBW,  LT_RGBWC,
};


enum LightSubtypes{ 
        LST_NONE, 
        LST_SINGLE, 
        LST_COLDWARM, 
        LST_RGB,   
        LST_RGBW, 
        LST_RGBWC, 
        LST_RGBCW
};

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
    
#include "1_TaskerManager/mTaskerManager.h"

#include "3_Network/MQTT/mMQTT.h"
#include "2_CoreSystem/Time/mTime.h"

#include "6_Lights/_Interface/palettes.h"

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#define LST_MAX 5

// DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PARAMETERS_CTR) "values/parameters";


  #define WEB_CLASS_RGB_LIVE "rgb_live"
  
    // Arrays to hold palettes, called via pointer
    // Variable user maps need to be variable  
    #ifndef D_PALETTE_USER_NAME_CTR
    #define D_PALETTE_USER_NAME_CTR        "User"   
    #endif
    #define PALETTELIST_COLOUR_USERS_AMOUNT_MAX PALETTELIST_COLOUR_AMOUNT_MAX


//#define DEBUG_LIGHT

typedef unsigned long power_t;              // Power (Relay) type

// Light color mode, either RGB alone, or white-CT alone, or both only available if ct_rgb_linked is false
enum LightColorModes {
  LCM_RGB = 1, LCM_CT = 2, LCM_BOTH = 3 
};

// struct LRgbColor {
//   uint8_t R, G, B;
// };
// const uint8_t MAX_FIXED_COLOR = 12;
// // const LRgbColor kFixedColor[MAX_FIXED_COLOR] PROGMEM =
// //   { 255,0,0, 0,255,0, 0,0,255, 228,32,0, 0,228,32, 0,32,228, 188,64,0, 0,160,96, 160,32,240, 255,255,0, 255,0,170, 255,255,255 };

// struct LWColor {
//   uint8_t W;
// };
// const uint8_t MAX_FIXED_WHITE = 4;
// // const LWColor kFixedWhite[MAX_FIXED_WHITE] PROGMEM = { 0, 255, 128, 32 };

// struct LCwColor {
//   uint8_t C, W;
// };
// const uint8_t MAX_FIXED_COLD_WARM = 4;
// const LCwColor kFixedColdWarm[MAX_FIXED_COLD_WARM] PROGMEM = { 0,0, 255,0, 0,255, 128,128 };


#ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
// New version of Gamma correction compute
// Instead of a table, we do a multi-linear approximation, which is close enough
// At low levels, the slope is a bit higher than actual gamma, to make changes smoother
// Internal resolution is 10 bits.

typedef struct gamma_table_t {
  uint16_t to_src;
  uint16_t to_gamma;
} gamma_table_t;

const gamma_table_t gamma_table[] = {   // don't put in PROGMEM for performance reasons
  {    1,      1 },
  {    4,      1 },
  {  209,     13 },
  {  312,     41 },
  {  457,    106 },
  {  626,    261 },
  {  762,    450 },
  {  895,    703 },
  { 1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};

// simplified Gamma table for Fade, cheating a little at low brightness
const gamma_table_t gamma_table_fast[] = {
  {   384,    192 },
  {   768,    576 },
  {  1023,   1023 },
  { 0xFFFF, 0xFFFF }          // fail-safe if out of range
};
#endif // ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

// For reference, below are the computed gamma tables, via ledGamma()
// for 8 bits output:
//   0,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,  1,
//   1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
//   2,  2,  2,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,
//   3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  5,  5,  5,  6,  6,  6,
//   6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9,  9, 10, 10, 10, 11,
//  11, 12, 12, 12, 13, 13, 14, 14, 15, 15, 16, 16, 17, 17, 17, 18,
//  18, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23, 24, 24, 25, 25,
//  25, 26, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 36, 37, 38,
//  39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50, 51, 52, 53,
//  54, 55, 56, 57, 58, 59, 60, 61, 61, 62, 63, 64, 65, 67, 68, 69,
//  71, 72, 73, 75, 76, 78, 79, 80, 82, 83, 85, 86, 87, 89, 90, 91,
//  93, 94, 95, 97, 98,100,101,102,104,105,107,108,109,111,112,114,
// 116,118,120,122,124,125,127,129,131,133,135,137,139,141,143,144,
// 146,148,150,152,154,156,158,160,162,164,166,168,170,171,173,175,
// 178,180,183,185,188,190,193,195,198,200,203,205,208,210,213,215,
// 218,220,223,225,228,230,233,235,238,240,243,245,248,250,253,255
//
// and for 10 bits output:
//    0,   1,   1,   1,   2,   2,   2,   2,   3,   3,   3,   3,   4,   4,   4,   4,
//    5,   5,   5,   5,   5,   6,   6,   6,   6,   7,   7,   7,   7,   8,   8,   8,
//    8,   8,   9,   9,   9,   9,  10,  10,  10,  10,  11,  11,  11,  11,  12,  12,
//   12,  12,  13,  13,  13,  14,  15,  16,  17,  18,  20,  21,  22,  23,  24,  25,
//   26,  27,  28,  29,  30,  31,  33,  34,  35,  36,  37,  38,  39,  40,  41,  43,
//   45,  47,  49,  50,  52,  54,  56,  58,  59,  61,  63,  65,  67,  68,  70,  72,
//   74,  76,  77,  79,  81,  83,  84,  86,  88,  90,  92,  93,  95,  97,  99, 101,
//  102, 104, 106, 110, 113, 117, 121, 124, 128, 132, 135, 139, 143, 146, 150, 154,
//  158, 162, 166, 169, 173, 177, 180, 184, 188, 191, 195, 199, 202, 206, 210, 213,
//  217, 221, 224, 228, 232, 235, 239, 243, 246, 250, 254, 257, 261, 267, 272, 278,
//  283, 289, 294, 300, 305, 311, 317, 322, 328, 333, 339, 344, 350, 356, 361, 367,
//  372, 378, 383, 389, 394, 400, 406, 411, 417, 422, 428, 433, 439, 444, 450, 458,
//  465, 473, 480, 488, 496, 503, 511, 518, 526, 534, 541, 549, 557, 564, 572, 579,
//  587, 595, 602, 610, 617, 627, 635, 642, 650, 657, 665, 673, 680, 688, 695, 703,
//  713, 723, 733, 743, 753, 763, 773, 783, 793, 803, 813, 823, 833, 843, 853, 863,
//  873, 883, 893, 903, 913, 923, 933, 943, 953, 963, 973, 983, 993,1003,1013,1023
//
// Output for Dimmer 0..100 values
//   0,  1,  2,  3,  3,  4,  4,  5,  5,  6,  7,  7,  8,  8,  9,
//  10, 10, 11, 12, 12, 13, 15, 17, 21, 23, 26, 28, 31, 34, 37,
//  40, 43, 49, 52, 58, 61, 67, 70, 76, 79, 84, 90, 93, 99,102,
// 110,117,128,135,146,158,166,177,184,195,202,213,221,232,239,
// 250,261,272,289,300,317,328,344,356,372,389,400,417,428,444,
// 458,480,496,518,534,557,579,595,617,635,657,673,695,713,743,
// 773,793,823,843,873,893,923,943,973,993,1023

// const char kListPWM_TestColours[] PROGMEM =  
//   "#ff0000" "|" 
//   "#00ff00" "|" 
//   "#0000ff" "|" 
//   "#ffffff" "|"  
//   "#FFE076" "|" ;

  
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SCENE_CTR) "scene";


DEFINE_PGM_CTR(PM_ANIMATION_MODE_NONE_NAME_CTR )   "None"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_TURN_ON_NAME_CTR )   "Turn On"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_TURN_OFF_NAME_CTR )   "Turn Off"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR   )      "Ambilight"  ;                
DEFINE_PGM_CTR(PM_ANIMATION_MODE_SCENE_NAME_CTR   )          "Scene"      ;             
DEFINE_PGM_CTR(PM_ANIMATION_MODE_FLASHER_NAME_CTR  )         D_JSON_FLASHER;
#ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS       
DEFINE_PGM_CTR(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)   D_JSON_NOTIFICATIONS;      
#endif   

  
//     #define D_MODE_SINGLECOLOUR_NOTACTIVE_NAME_CTR       "NOTACTIVE"    
//     #define D_MODE_SINGLECOLOUR_DAYON_NAME_CTR           "DAYON"    
//     #define D_MODE_SINGLECOLOUR_DAYOFF_NAME_CTR          "DAYOFF"    
//     #define D_MODE_SINGLECOLOUR_EVENINGON_NAME_CTR       "EVENINGON"    
//     #define D_MODE_SINGLECOLOUR_EVENINGOFF_NAME_CTR      "EVENINGOFF"    
//     #define D_MODE_SINGLECOLOUR_MIDNIGHTON_NAME_CTR      "MIDNIGHTON"    
//     #define D_MODE_SINGLECOLOUR_MIDNIGHTOFF_NAME_CTR     "MIDNIGHTOFF"    
DEFINE_PGM_CTR(PM_MODE_SINGLECOLOUR_COLOURSCENE_NAME_CTR)   "ColourSingle"; //COLOURSCENE
DEFINE_PGM_CTR(PM_MODE_SINGLECOLOUR_PALETTE_SINGLE_NAME_CTR) "PaletteSingle";
DEFINE_PGM_CTR(PM_MODE_SINGLECOLOUR_PALETTELOOP_NAME_CTR)   "PaletteLoop";
//     #define D_MODE_SINGLECOLOUR_FLASHCOLOUR_NAME_CTR     "FLASHCOLOUR"    
//     #define D_MODE_SINGLECOLOUR_SUNRISE_SINGLE_NAME_CTR  "SUNRISE_SINGLE"  
//     #define D_MODE_SINGLECOLOUR_SUNRISE_DUAL_NAME_CTR    "SUNRISE_DUAL" 



  #define WEB_HANDLE_LIVEPIXELS_SHARED_JSON "/shared/rgb_livepixels.json"


    enum RGB_VIEW_SHOW_TYPE_IDS{
      RGB_VIEW_SHOW_TYPE_NONE_ID=0,
      RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID,
      RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID,
      RGB_VIEW_SHOW_TYPE_FOLLOW_PALETTE_ID,

      RGB_VIEW_SHOW_TYPE_LENGTH_ID
    };
  
    enum fMapIDs_Type_IDS{
      MAPIDS_TYPE_HSBCOLOURMAP_NOINDEX_ID=0,
      MAPIDS_TYPE_HSBCOLOUR_NOINDEX_ID,
      MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_ID,
      MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_AND_SETALL_ID, //phase out
      
      MAPIDS_TYPE_HSBCOLOUR_WITHINDEX_GRADIENT_ID,
      MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_GRADIENT_ID,
      
      MAPIDS_TYPE_RGBCOLOUR_NOINDEX_ID,
      MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_ID,
      MAPIDS_TYPE_RGBCOLOUR_WITHINDEX_AND_SETALL_ID, //phase out, assume max index IS set all

      MAPIDS_TYPE_RGBCCTCOLOUR_NOINDEX_ID,

      MAPIDS_TYPE_RGBCOLOUR_LENGTH_ID
    };

    enum fIndexs_Type_IDS{
      INDEX_TYPE_NONE = 0,
      INDEX_TYPE_DIRECT,
      INDEX_TYPE_SCALED_255,
      INDEX_TYPE_SCALED_100 
    };


class mInterfaceLight{ //name reverse, as Interface is the linking/grouping factor
  public:
    mInterfaceLight(){};

    // void Module_Init();

    uint8_t force_strip_update_test = false;

    typedef union {
      uint8_t data; // allows full manipulating
      struct { 
        // enable animations (pause)
        uint8_t enable_cct_channel_sliders : 1;

        // Reserved
        uint8_t reserved : 7;
      };
    } SETTINGS_FLAGS;


    struct SETTINGS{
      SETTINGS_FLAGS flags;
      uint8_t pixel_hardware_color_order_id = PIXEL_HARDWARE_COLOR_ORDER_RGB_ID;

    }settings;


    //#ifdef ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE
    NeoPixelAnimator* animator_controller = nullptr;
    //#endif // ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE

  void Init_NeoPixelAnimator(uint16_t size, uint8_t timebase);  

// New feature, turning off will overwrite animation struct on "on" will restore to previous

  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // enable animations (pause)
      uint16_t fEnable_Animation : 1;
      uint16_t ColourComponentsUpdatedButNotYetApplied : 1; // new colours, not yet applied


      uint16_t fForceUpdate : 1; 
      uint16_t fRunning : 1;//= false;
      uint16_t fEndUpdatesWhenAnimationCompletes : 1;// = false;
      
      uint16_t ftime_use_map : 1;//= true;
      uint16_t frate_use_map : 1;//= true;

      // Reserved
      uint16_t reserved : 11;
    };
  } ANIMATION_FLAGS;

    struct ANIMATIONSETTINGS{
      //mode = turn on/off/scene/pallette
      uint8_t mode_id; //update mode
      //palette - stores colour patterns for single led setting
      uint8_t palette_id; 

      uint8_t pixelgrouped=3; // nearby pixels repeat colours
      
      // float brightness = 1; // MOVE OUT OF ANIMATION?? move to "light class"

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

    const char* GetHardwareColourTypeName(char* buffer);
    const char* GetHardwareColourTypeNameByID(uint8_t id, char* buffer);
    int8_t GetHardwareColourTypeIDbyName(const char* c);

    // Flags and states that are used during one transition and reset when completed
    struct ANIMATIONOVERRIDES{
      uint8_t fRefreshAllPixels = false;
      uint16_t time_ms = 1000; //on boot
    }animation_override;
    
    /**************
     * TURN_ON - fade ON, returns to previous lighting array
     * TURN_OFF - fade off, either turns off, or calls "SCENE" to set number = ALSO SAVES CURRENT OUTPUT
     * NOTHING - to be named "off", does nothing, simply remains off
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
    const char* GetAnimationModeName(char* buffer, uint16_t buflen);
    const char* GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen);

    
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

    void Settings_Default();  

    void BootMessage();

    // RgbcctColor current_color_main;

    int     hue_test = 0;

    void EveryLoop();

    
      uint16_t auto_time_off_secs = 0;
      // uint32_t tSavedAutoOff = 0;
    uint32_t tSavedAutoOff = 0;
    

    // enum MODE_SINGLECOLOUR_PARTS{STEP1=1,STEP2,STEP3,STEP4,STEP5,DONE};


    
    typedef union {
      uint8_t data; // allows full manipulating
      struct { 
        uint8_t running : 1;
        uint8_t generate_new_colours : 1;
        uint8_t colours_need_applied : 1;
        uint8_t update_colour_on_input_command : 1;
        // uint8_t running : 1;
        
        // Reserved
        uint8_t reserved : 4;
      };
    } MODE_SINGLECOLOUR_CONFIG_FLAGS;
    struct MODE_SINGLECOLOUR_CONFIG{
      uint8_t name_id;
      RgbcctColor colour;
      MODE_SINGLECOLOUR_CONFIG_FLAGS flag;
      uint8_t fActive = false; // isrunning
      uint8_t parts = 0;//DONE; //parts aka index
      uint32_t tStart;
      uint32_t tOnTime;
      uint8_t update = true;  // to replace "NOCHANGE"
    };
    // hold current set scene
    struct MODE_SINGLECOLOUR_CONFIG mode_singlecolour;
    // hold scene to switch to later
    struct MODE_SINGLECOLOUR_CONFIG mode_singlecolour_stored;

    
    timereached_t mode_singlecolour_tSaved;

    
      RgbcctColor tasint_colour;
      
void setChannelsRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t wc, uint8_t ww);


void StartFadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0) );



    // hold when to switch to scene_stored
    struct SCENE_CONFIG_RUNTIME{
      uint8_t  fWaiting = false;
      // For ms or sec delay (until execution) time
      uint32_t delay_time_ms   = 0; //0 if not set
      uint32_t delay_time_secs = 0;
    }scene_stored_runtime;





    uint8_t kitchen_preset_set_id;
    struct SCENE_PRESETS{
      RgbTypeColor colour; 
      uint16_t time_ms=1000;
    };
    //change to scenes users
    struct SCENE_PRESETS scene_preset_dayon;
    // struct SCENE_PRESETS scene_preset_dayoff;
    // struct SCENE_PRESETS scene_preset_eveningon;
    // struct SCENE_PRESETS scene_preset_eveningoff;
    // struct SCENE_PRESETS scene_preset_nighton;
    // struct SCENE_PRESETS scene_preset_nightoff;
    struct SCENE_PRESETS* presenttmp;






void Template_Load();


int8_t Tasker(uint8_t function, JsonObjectConst obj);
int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);
// void parsesub_TopicCheck_JSONCommand(JsonObjectConst obj);
    
// void parsesub_CheckAll(JsonObjectConst obj);
// void parsesub_Settings(JsonObjectConst obj);
void parse_JSONCommand(JsonObjectConst obj);




    int8_t Tasker_Web(uint8_t function);
    #include "6_Lights/_Interface/mInterfaceLight_Web.h"

    enum RELAY_IDS{RELAY_0_ID=0,RELAY_1_ID,RELAY_2_ID,RELAY_3_ID};

    int8_t Tasker(uint8_t function);
    void Init(void);

    void parse_JSONCommand(void);
    // void parsesub_ModeManual(JsonObjectConst obj);
    
  // are RGB and CT linked, i.e. if we set CT then RGB channels are off
  bool     _ct_rgb_linked = true;
  bool     _pwm_multi_channels = false;    // treat each channel as independant dimmer


    uint16_t getCT10bits();
    void setCTRange(uint16_t ct_min_range, uint16_t ct_max_range) ;
    void getCTRange(uint16_t ct_min_range, uint16_t ct_max_range) ;
    void getCTRange(uint16_t *ct_min_range, uint16_t *ct_max_range) ;


    void WebAppend_Root_Buttons();

    struct FADE_SETTINGS{
      bool running = false;
      uint32_t start_ms = 0;
      uint32_t end_ms = 0;
      uint16_t duration_ms = 0;

    }fade;



  //inline 
  // bool isCTRGBLinked();
  // bool setPWMMultiChannel(bool pwm_multi_channels);
  // bool isPWMMultiChannel(void);


// class LightStateClass {
//   private:
   
//   public:
//     LightStateClass() {
//       //AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::Constructor RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _bri);
//     }


void setChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t wc = 0, uint8_t ww = 0);


uint16_t getHue();
uint8_t  getSat();

// CT min and max
const uint16_t CT_MIN = 153;          // 6500K
const uint16_t CT_MAX = 500;          // 2000K
// Ranges used for Alexa
const uint16_t CT_MIN_ALEXA = 200;    // also 5000K
const uint16_t CT_MAX_ALEXA = 380;    // also 2600K

    uint16_t _hue = 0;  // 0..359
    uint8_t  _sat = 255;  // 0..255
    uint8_t  _briRGB = 255;  // 0..255
    // dimmer is same as _bri but with a range of 0%-100%
    // uint8_t  _r = 255;  // 0..255
    // uint8_t  _g = 255;  // 0..255
    // uint8_t  _b = 255;  // 0..255

    uint8_t  _subtype = 0;  // local copy of Light.subtype, if we need multiple lights
    uint16_t _ct = CT_MIN;  // 153..500, default to 153 (cold white)
    // uint8_t  _wc = 255;  // white cold channel
    // uint8_t  _ww = 0;    // white warm channel
    uint8_t  _briCT = 255;

    uint8_t  _color_mode = LCM_RGB; // RGB by default
    // Please note that you can still set CT to 153..500, but any
    // value below _ct_min_range or above _ct_max_range not change the CT
    uint16_t _ct_min_range = CT_MIN;   // the minimum CT rendered range
    uint16_t _ct_max_range = CT_MAX;   // the maximum CT rendered range

    /**************
     * Predefined SCENES by name
     * SCENES are colours defined as one colour for all pixels
    **************/ 
   // These scenes should be palettes "single type"(day/night) and "multi type"(waking up)
    enum SCENES{
      //MODE_SINGLECOLOUR_NOCHANGE_ID=0,    // REMOVING mode, as I dont want to change it, but add "nochange" within existing modes
      // MODE_SINGLECOLOUR_DAYON_ID=1,
      // MODE_SINGLECOLOUR_DAYOFF_ID,
      // MODE_SINGLECOLOUR_EVENINGON_ID,
      // MODE_SINGLECOLOUR_EVENINGOFF_ID,
      // MODE_SINGLECOLOUR_MIDNIGHTON_ID,
      // MODE_SINGLECOLOUR_MIDNIGHTOFF_ID,
      MODE_SINGLECOLOUR_COLOURSCENE_ID,
      MODE_SINGLECOLOUR_PALETTE_SINGLE_ID,     //use palette as colours
      MODE_SINGLECOLOUR_FADE_OFF_ID,
      MODE_SINGLECOLOUR_FLASHCOLOUR_ID,
      // MODE_SINGLECOLOUR_WHITEON_ID,
      // MODE_SINGLECOLOUR_SUNRISE_SINGLE_ID,     // From dark blue to sky blue (cyan)
      // MODE_SINGLECOLOUR_SUNRISE_DUAL_ID,       // From dark blue, sunrise orange, sky blue (cyan)
      // MODE_SINGLECOLOUR_SUNRISE_AMBILIGHT_ID,
      MODE_SINGLECOLOUR_LENGTH_ID
    };
    int8_t GetSceneIDbyName(const char* c);
    const char* GetSceneName(char* buffer, uint8_t buflen);
    const char* GetSceneNameByID(uint8_t id, char* buffer, uint8_t buflen);

    int8_t GetTransitionMethodIDbyName(const char* c);     
    const char* GetTransitionMethodName(char* buffer);
    const char* GetTransitionMethodNameByID(uint8_t id, char* buffer);

    int8_t GetTransitionOrderIDbyName(const char* c);     
    const char* GetTransitionOrderName(char* buffer);
    const char* GetTransitionOrderNameByID(uint8_t id, char* buffer);


    uint8_t ConstructJSON_Scene(uint8_t json_level = 0);
    // void parsesub_ModeScene(JsonObjectConst obj);
    
    void SubTask_SingleColour();
    void init_Scenes();



  uint8_t current_color[LST_MAX]; // r,r,b,cw,ww
  uint8_t last_colour[LST_MAX];




  uint8_t entry_color[LST_MAX];



  uint8_t new_color[LST_MAX];
  uint8_t color_remap[LST_MAX];

  
void SubTask_AutoOff();


  uint8_t light_power = 0;
  uint8_t light_power_Saved = 0;


// struct LIGHT {
  uint32_t strip_timer_counter = 0;  // Bars and Gradient
  // power_t power = 0;                      // Power<x> for each channel if SetOption68, or boolean if single light

  uint16_t wakeup_counter = 0;

  enum COLOUR_IDS{
    COLOUR_RED_ID=0,
    COLOUR_GREEN_ID,
    COLOUR_BLUE_ID,
    COLOUR_WHITE_ID,
    COLOUR_WARM_WHITE_ID,
    COLOUR_LENGTH_ID
  };

uint32_t tSavedTest= 0;
uint32_t tSavedTest2 = 0;

  // uint8_t entry_color[LST_MAX];
  // uint8_t current_color[LST_MAX]; // r,g,b,cw,ww
  // uint8_t new_color[LST_MAX];
  // uint8_t last_color[LST_MAX];
  // uint8_t color_remap[LST_MAX];

  //Store RGB part of colour as HSB space
  // HsbColor hsb_colour;
  

void RecalculateHsbfromRgb();
void RecalculateRgbfromHsb();
void SetColour_CCT(uint16_t val);
void SetColour_CBrt(uint16_t val);


// uint16_t auto_time_off_secs = 0; 

  uint8_t wheel = 0;
  uint8_t random2 = 0;
  uint8_t subtype = 0;                    // LST_ subtype
  uint8_t device = 0;
  uint8_t old_power = 1;
  uint8_t wakeup_active = 0;
  uint8_t wakeup_dimmer = 0;
  uint8_t fixed_color_index = 1;
  uint8_t pwm_offset = 0;                 // Offset in color buffer
  // uint8_t max_scheme = LS_MAX -1;

  bool update = true;
  bool pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer

  bool     fade_initialized = false;      // dont't fade at startup
  bool     fade_running = false;
// #ifdef USE_DEVICE_GROUPS
//   uint8_t  last_scheme = 0;
//   bool     devgrp_no_channels_out = false; // don't share channels with device group (e.g. if scheme set by other device)
// #ifdef USE_DGR_LIGHT_SEQUENCE
//   uint8_t  sequence_offset = 0;            // number of channel changes this light is behind the master
//   uint8_t * channels_fifo;
// #endif  // USE_DGR_LIGHT_SEQUENCE
// #endif  // USE_DEVICE_GROUPS
// #ifdef USE_LIGHT_PALETTE
//   uint8_t  palette_count = 0;            // palette entry count
//   uint8_t * palette;                     // dynamically allocated palette color array
// #endif  // USE_LIGHT_PALETTE
  uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
  uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023
// } Light;


void SetPWMChannelSweepRange();

uint8_t test_index = 0;


void mat3x3(const float *mat33, const float *vec3, float *res3);

// uint16_t pwm_tester_val = 0;
// uint8_t pwm_tester_dir = 0;
// uint32_t tSavedTest3=0;

// uint16_t pwm_channel_test[5];


void WebCommand_Parse();


void WebAppend_Root_Draw_RGBLive();

void WebAppend_Root_Draw_RGBTable(uint8_t rows = 8);

    struct LIVEVIEW_SETTINGS{
      uint8_t show_type = RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID;
      uint8_t height_as_percentage = 15;
      uint8_t pixel_resolution_percentage = 100; //100% is send all, 0-99 is percentage of all
      uint16_t refresh_rate = 250;
    }liveview;




  uint8_t channels[5]; //rgbwcww
  

power_t LightPower(void);
power_t LightPowerIRAM(void);
uint8_t LightDevice(void);
uint8_t setColorMode(uint8_t cm);
void addRGBMode();
void addCTMode();
void getRGB(uint8_t *r, uint8_t *g, uint8_t *b);
void getCW(uint8_t *rc, uint8_t *rw);
void getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w);
uint8_t getChannels(uint8_t *channels);
void getChannelsRaw(uint8_t *channels);
void getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri);
uint8_t getBri(void);
uint8_t getBriCT();
static inline uint8_t DimmerToBri(uint8_t dimmer);
static uint8_t BriToDimmer(uint8_t bri);
uint8_t getDimmer(uint32_t mode = 0);

//     inline 
uint16_t getCT();// const {
//       return _ct; // 153..500, or CT_MIN..CT_MAX
//     }


    struct PALETTE_VIEW_SETTINGS{
      uint8_t show_type = 2;//RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID;
      uint8_t height_as_percentage = 15;
      uint8_t pixel_resolution_percentage = 100; //100% is send all, 0-99 is percentage of all
    }palette_view;


//     // get the CT value within the range into a 10 bits 0..1023 value
//     uint16_t getCT10bits() const {
//       return mSupport::changeUIntScale(_ct, _ct_min_range, _ct_max_range, 0, 1023);
//     }

//     inline void setCTRange(uint16_t ct_min_range, uint16_t ct_max_range) {
//       _ct_min_range = ct_min_range;
//       _ct_max_range = ct_max_range;
//     }

//     inline void getCTRange(uint16_t *ct_min_range, uint16_t *ct_max_range) const {
//       if (ct_min_range) { *ct_min_range = _ct_min_range; }
//       if (ct_max_range) { *ct_max_range = _ct_max_range; }
//     }

void getXY(float *x, float *y);
void setBri(uint8_t bri);
uint8_t setBriRGB(uint8_t bri_rgb);
uint8_t setBriCT(uint8_t bri_ct);
uint8_t getBriRGB();
void setDimmer(uint8_t dimmer);
void setCT(uint16_t ct);
void setCW(uint8_t c, uint8_t w, bool free_range = false);
uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false);
void setHS(uint16_t hue, uint8_t sat);
void setChannelsRaw(uint8_t *channels);
void setChannels(uint8_t *channels);
void RgbToHsb(uint8_t r, uint8_t g, uint8_t b, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri);
void HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b);
void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y);
void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb);
// void mat3x3(const float *mat33, const float *vec3, float *res3);
// void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y);
// void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb);
void setSubType(uint8_t sub_type);
bool setCTRGBLinked(bool ct_rgb_linked);
void setAlexaCTRange(bool alexa_ct_range);
bool isCTRGBLinked();
bool setPWMMultiChannel(bool pwm_multi_channels);
bool isPWMMultiChannel(void);
void debugLogs();
void loadSettings();
void changeCTB(uint16_t new_ct, uint8_t briCT);
void changeDimmer(uint8_t dimmer, uint32_t mode = 0);
void changeBri(uint8_t bri);
void changeBriRGB(uint8_t bri);
void changeBriCT(uint8_t bri);
void changeRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false);
void UpdateFinalColourComponents(uint8_t *current_color = nullptr);
void changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB);


void changeHSB_Hue(uint16_t hue);
void changeHSB_Sat(uint8_t sat);
void changeHSB_Brt(uint8_t brt);
    
  void init_Animations();


void saveSettings();
void changeChannels(uint8_t *channels);
uint16_t change8to10(uint8_t v);
uint8_t change10to8(uint16_t v);


#ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
uint16_t ledGamma_internal(uint16_t v, const struct gamma_table_t *gt_ptr);
uint16_t ledGammaReverse_internal(uint16_t vg, const struct gamma_table_t *gt_ptr);
uint16_t ledGamma10_10(uint16_t v);
uint16_t ledGamma10(uint8_t v);
uint8_t ledGamma(uint8_t v);
void calcGammaMultiChannels(uint16_t cur_col_10[5]);
void calcGammaBulbs(uint16_t cur_col_10[5]);
bool isChannelGammaCorrected(uint32_t channel);
uint16_t fadeGamma(uint32_t channel, uint16_t v);
uint16_t fadeGammaReverse(uint32_t channel, uint16_t vg);
#endif //ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION


void LightPwmOffset(uint32_t offset);
bool LightModuleInit(void);
void LightCalcPWMRange(void);
void LightUpdateColorMapping(void);
uint8_t LightGetDimmer(uint8_t dimmer);
void LightSetDimmer(uint8_t dimmer);
void LightGetHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri);
void LightGetXY(float *X, float *Y);
void LightHsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b);
uint8_t LightGetBri(uint8_t device);
void LightSetBri(uint8_t device, uint8_t bri);
void LightSetColorTemp(uint16_t ct);
uint16_t LightGetColorTemp(void);
void LightSetSignal(uint16_t lo, uint16_t hi, uint16_t value);
char* LightGetColor(char* scolor, boolean force_hex = false);
void LightPowerOn(void);
void LightState(uint8_t append);
void LightPreparePower(power_t channels = 0xFFFFFFFF);
void LightSetPaletteEntry(void);
void LightCycleColor(int8_t direction);
void LightSetPower(void);
void LightAnimate(void);
bool LightApplyFade(void);
void LightApplyPower(uint8_t new_color[LST_MAX], power_t power);
bool isChannelCT(uint32_t channel);

uint8_t flag_test = true;

uint8_t* GetColour_RGB();    // r,g,b only
uint8_t* GetColour_RGBCT();  // r,g,b,white and warm white
uint8_t* GetColour_CT();     // white and warm white
uint8_t* GetColour_White();         // white
uint8_t* GetColour_WarmWhite();     // warm white
// uint8_t* GetColour_HSB();
uint8_t* GetColour_Hue();
uint8_t* GetColour_Sat();
uint8_t* GetColour_Brightness();

void WebAppend_Root_Draw_Table();

void SetColour_RGB();    // r,g,b only
void SetColour_RGBCT();  // r,g,b,white and warm white
void SetColour_CT();     // white and warm white
void SetColour_White();         // white
void SetColour_WarmWhite();     // warm white
void SetColour_HSB();

float    HueN2F(uint16_t hue);
float    SatN2F(uint8_t sat);
float    BrtN2F(uint8_t brt);
uint16_t HueF2N(float hue);
uint8_t  SatF2N(float sat);
uint8_t  BrtF2N(float brt);

RgbTypeColor GetRandomColour(RgbTypeColor colour1, RgbTypeColor colour2);
HsbColor GetHSBColour();




// void parsesub_ModeAnimation(JsonObjectConst obj);

void SetHSBColour();
void SetColour_HSB(HsbColor hsb);
HsbColor GetColour_HSB(void);
void SetColour_Hue(uint16_t hue);
void SetColour_Sat(uint8_t  sat);
void SetColour_Brt(uint8_t  brt);


uint16_t light_count = 1;

  uint32_t WebColorFromColourMap(uint8_t i);
  // uint32_t WebColorFromColourType(RgbwColor rgb);
  uint32_t WebColorFromColourType(RgbColor rgb);
  
RgbTypeColor Color32bit2RgbColour(uint32_t colour32bit);

  
    const uint8_t* GetDefaultColourPaletteUserIDs_P(uint8_t id);
    uint8_t  GetDefaultColourPaletteUserIDsCount(uint8_t id);


void UpdateSetOutputs();


uint8_t getColorMode();



    
    void init_PresetColourPalettes(); 
    // First bytes stores length 
    void init_ColourPalettes_Autumn_Red();
    void init_ColourPalettes_Autumn();
    void init_ColourPalettes_Winter();
    void init_ColourPalettes_Pastel();
    void init_ColourPalettes_Ocean_01();
    void init_ColourPalettes_Rainbow();
    void init_ColourPalettes_RainbowInverted();
    void init_ColourPalettes_Holloween_OP();
    void init_ColourPalettes_Holloween_OGP();
    void init_ColourPalettes_Shelf_Hearts();
    void init_ColourPalettes_Gradient_01();
    void init_ColourPalettes_Gradient_02();
    void init_ColourPalettes_Sunrise_01();


    enum SHELFFLIES_HEART_INDEX{ //right to left
      SHELFFLIES_HEART_INDEX1=1,
      SHELFFLIES_HEART_INDEX2=6,
      SHELFFLIES_HEART_INDEX3=10,
      SHELFFLIES_HEART_INDEX4=16,
      SHELFFLIES_HEART_INDEX5=21,
      SHELFFLIES_HEART_INDEX6=25,
      SHELFFLIES_HEART_INDEX7=31
    };


    /**************
     * COLOUR Designs by name
     * PALETTE are different colours by pixel
    **************/                            
    int8_t GetPaletteIDbyName(const char* c);
    const char* GetPaletteName(char* buffer);  
    const char* GetPaletteNameByID(uint8_t id, char* buffer);
    const char* GetPaletteFriendlyName(char* buffer);

  // #define D_PALETTE_USER_DEFAULT_NAME_CTR "User"

  enum PALETTELIST_VARIABLE_USER_IDS{ // 20 TOTAL variable and can be deleted by the user, saved in memory
    PALETTELIST_VARIABLE_USER_01_ID = 0,
    PALETTELIST_VARIABLE_USER_02_ID,
    PALETTELIST_VARIABLE_USER_03_ID,
    PALETTELIST_VARIABLE_USER_04_ID,
    PALETTELIST_VARIABLE_USER_05_ID,
    PALETTELIST_VARIABLE_USER_06_ID,
    PALETTELIST_VARIABLE_USER_07_ID,
    PALETTELIST_VARIABLE_USER_08_ID,
    PALETTELIST_VARIABLE_USER_09_ID,
    PALETTELIST_VARIABLE_USER_10_ID, 
    PALETTELIST_VARIABLE_USER_11_ID, 
    PALETTELIST_VARIABLE_USER_12_ID,  
    PALETTELIST_VARIABLE_USER_13_ID, 
    PALETTELIST_VARIABLE_USER_14_ID, 
    PALETTELIST_VARIABLE_USER_15_ID, 
    PALETTELIST_VARIABLE_USER_16_ID, 
    PALETTELIST_VARIABLE_USER_17_ID, 
    PALETTELIST_VARIABLE_USER_18_ID, 
    PALETTELIST_VARIABLE_USER_19_ID, 
    PALETTELIST_VARIABLE_USER_20_ID, 
    // Count of total handlers and starting point for other modules
    PALETTELIST_VARIABLE_USER_LENGTH_ID 
  };
  enum PALETTELIST_STATIC_IDS{
    // Shelf Lights ie has some static leds
    PALETTELIST_STATIC_SHELF_HEARTS_ID = PALETTELIST_VARIABLE_USER_LENGTH_ID,
    // Special patterns
    PALETTELIST_STATIC_HOLLOWEEN_OP_ID,
    PALETTELIST_STATIC_HOLLOWEEN_OGP_ID,
    PALETTELIST_STATIC_RAINBOW_ID,
    PALETTELIST_STATIC_RAINBOW_INVERTED_ID,
    PALETTELIST_STATIC_PASTEL_ID, //PASTEL
    PALETTELIST_STATIC_WINTER_ID,
    PALETTELIST_STATIC_AUTUMN_ID,
    PALETTELIST_STATIC_AUTUMN_RED_ID,
    PALETTELIST_STATIC_GRADIENT_01_ID,
    PALETTELIST_STATIC_GRADIENT_02_ID,
    PALETTELIST_STATIC_SUNRISE_01_ID,
    PALETTELIST_STATIC_OCEAN_01_ID,

    // Count of total handlers and starting point for other modules
    PALETTELIST_STATIC_LENGTH_ID 
  };

    #define PALETTELIST_COLOUR_AMOUNT_MAX 20//15 new max

    
    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        //random palettes will require estimations of nearest hues
        //uint16_t fColours_from_mapTBR : 1;
         // randomise between the hue and saturation values
        uint16_t fRandomise_Between_Colour_Pairs : 1;
         // Scaled between first and second colour, ignores the rest
        //uint16_t fLinearBlend_Between_Colour_Pairs : 1; //in gradient?
        // Enable the brightness parameter from colourmap, scale with animation.brightness
        uint16_t fUse_Brightness_From_ColourMap_As_Ratio_Of_Global_Brightness : 1; 
        // Use mapped IDs as RGB values, {R,G,B...}
        //uint16_t fMapIDs_Are_RGB_TripletsTBR : 1; 
        // (max 255 length string, if string>255, assume percentage)
        //uint16_t fIndexs_Are_Percentage : 1;
        // Override Saturation and randomise, skewed high for visual similarity.
        uint16_t fOverride_Saturation_To_Random : 1;
        // Blends colours between indexs (used for fMapIDs_Type=4)
        //uint16_t fIndexs_Are_Gradient : 1;
        // Hold type of map
        uint16_t fMapIDs_Type : 4; //9 possible types
          // 0 - HsbColors directly, no index eg {hsb_color_map1, hsb_color_map2, hsb_color_map3,...}
          // 1 - HsbColors with indexes {index1,hsb_color_map1, index1,hsb_color_map1,...}
          // 2 - RgbColors only, triplets eg {R1,G1,B1, R2,G2,B2,...}
          // 3 - RgbColors triplets with indexed eg {index1,R1,G1,B1, index2,R2,G2,B2,...}
          // 4 - RgbColors triplets with indexed and largest index sets all as option eg {index1,R1,G1,B1, index2,R2,G2,B2,... MAX,R_all,G_all,B_all}
          // 5 - HsbColors only
          // 6 - HsbColors only
          // 7 - HsbColors only      
          // phase down to be only RGB, HSB?  
        uint16_t fIndexs_Type : 2;
        // 0 - exact
        // 1 - gradient range 0-255
        // 2 - gradient range 0-100%
        // 3 - reserved
        // Reserved
        uint16_t reserved : 7;
      };
    } PALETTE_FLAGS;

    // Consider template split
    struct PALETTELIST{
      struct PALETTE{ //6 bytes per palette
        // 8 bits
        PALETTE_FLAGS flags;        
        // enum: Used to call for ctr of friendly name
        uint8_t id = 0;
        // Pointer to name
        char* friendly_name_ctr = nullptr;
        // Map IDs
        uint8_t* colour_map_id; 
        // Active pixels
        uint8_t colour_map_size = 0;
      };
      PALETTE *ptr;
      // Can be edited
      PALETTE users[20];
      // Add static palettes here
      PALETTE holloween_op;
      PALETTE holloween_ogp;
      PALETTE winter;
      PALETTE autumn;
      PALETTE autumn_red;
      PALETTE rainbow;
      PALETTE rainbow_inverted;
      PALETTE pastel;
      PALETTE ocean_01;
      PALETTE shelf_hearts;
      PALETTE gradient_01;
      PALETTE gradient_02;
      PALETTE sunrise_01;
    }palettelist;


    const char* GetColourMapNamebyID(uint8_t id, char* buffer);
    const char* GetColourMapNameNearestbyColour(HsbColor c, char* buffer);
    int8_t      GetColourMapIDbyName(const char* c);
    int8_t GetNearestColourMapIDFromColour(HsbColor hsb);

    void setdefault_PresetColourPalettes_UserFill(PALETTELIST::PALETTE *ptr, uint8_t* colour_ids, uint8_t colour_ids_len);


  const char* GetPaletteNameFriendlyFirstByID(uint8_t id, char* buffer);
    const char* GetPaletteFriendlyNameByID(uint8_t id, char* name = nullptr, uint8_t name_size= 0);    

      uint16_t GetPixelsInMap(PALETTELIST::PALETTE *ptr);
      HsbColor GetHsbColour(uint8_t id);

        // RgbTypeColor GetColourFromPalette(
        //   PALETTELIST::PALETTE *ptr, 
        //   uint16_t pixel_num, int16_t* pixel_position);//, 
        //   // int32_t  *pixel_position);80
        
void ApplyGlobalBrightnesstoColour(RgbcctColor* colour);

          
RgbcctColor GetColourFromPalette(PALETTELIST::PALETTE *ptr, uint16_t pixel_num, int16_t *pixel_position = nullptr);

          PALETTELIST::PALETTE* GetPalettePointerByID(uint8_t id);

          
    void setdefault_PresetColourPalettes_UserFill(uint8_t id);
    void init_PresetColourPalettes_UserFill(uint8_t id);
    void setdefault_PresetColourPalettes_UserFill(PALETTELIST::PALETTE *ptr, const uint8_t* colour_ids, const uint8_t colour_ids_len); //declare

    int8_t CheckPaletteIsEditable(PALETTELIST::PALETTE *ptr);
  void SetPaletteListPtrFromID(uint8_t id);



    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
    uint8_t ConstructJSON_Debug(uint8_t json_method = 0);

  
    #ifdef USE_MQTT 
        void MQTTHandler_Init();
        void MQTTHandler_Set_fSendNow();
        void MQTTHandler_Set_TelePeriod();
        
        struct handler<mInterfaceLight>* mqtthandler_ptr;
        void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

        // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
        struct handler<mInterfaceLight> mqtthandler_settings_teleperiod;
        
        // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
        struct handler<mInterfaceLight> mqtthandler_sensor_ifchanged;
        struct handler<mInterfaceLight> mqtthandler_sensor_teleperiod;
        
        // const char* postfix_topic_debug = "debug/parameters";
        // to be PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PARAMETERS_CTR but needs to handle progmem
        struct handler<mInterfaceLight> mqtthandler_debug_teleperiod;   

        // const char* postfix_topic_scene = "scene";
        struct handler<mInterfaceLight> mqtthandler_scene_teleperiod;
            
        // Extra module only handlers
        // enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
        //   MQTT_HANDLER_MODULE_ENERGYSTATS_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
        //   MQTT_HANDLER_MODULE_ENERGYSTATS_TELEPERIOD_ID,
        //   MQTT_HANDLER_MODULE_LENGTH_ID, // id count
        // };

 enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_SCENE_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_DEBUG_PARAMETERS_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
        
      //,


        // const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    #endif






};
#endif

#endif




// /*
//   xdrv_04_light.ino - PWM, WS2812 and sonoff led support for Tasmota

//   Copyright (C) 2020  Theo Arends

//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.

//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.

//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.
// */

// // #ifdef USE_LIGHT
// /*********************************************************************************************\
//  * PWM, WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01, H801, MagicHome and Arilux
//  *
//  * Settings.light_settings.type  Module     Color  ColorTemp  Modules
//  * ----------  ---------  -----  ---------  ----------------------------
//  *  0          -                 no         (Sonoff Basic)
//  *  1          PWM1       W      no         (Sonoff BN-SZ)
//  *  2          PWM2       CW     yes        (Sonoff Led)
//  *  3          PWM3       RGB    no         (H801, MagicHome and Arilux LC01)
//  *  4          PWM4       RGBW   no         (H801, MagicHome and Arilux)
//  *  5          PWM5       RGBCW  yes        (H801, Arilux LC11)
//  *  9          reserved          no
//  * 10          reserved          yes
//  * 11          +WS2812    RGB    no         (One WS2812 RGB or RGBW ledstrip)
//  * 12          AiLight    RGBW   no
//  * 13          Sonoff B1  RGBCW  yes
//  *
//  * light_scheme  WS2812  3+ Colors  1+2 Colors  Effect
//  * ------------  ------  ---------  ----------  -----------------
//  *  0            yes     yes        yes         Color On/Off
//  *  1            yes     yes        yes         Wakeup light
//  *  2            yes     yes        no          Color cycle RGB
//  *  3            yes     yes        no          Color cycle RBG
//  *  4            yes     yes        no          Random RGB colors
//  *  5            yes     no         no          Clock
//  *  6            yes     no         no          Incandescent
//  *  7            yes     no         no          RGB
//  *  8            yes     no         no          Christmas
//  *  9            yes     no         no          Hanukkah
//  * 10            yes     no         no          Kwanzaa
//  * 11            yes     no         no          Rainbow
//  * 12            yes     no         no          Fire
//  *
// \*********************************************************************************************/

// /*********************************************************************************************\
//  *
//  * Light management has been refactored to provide a cleaner class-based interface.
//  * Also, now all values are stored as integer, no more floats that could generate
//  * rounding errors.
//  *
//  * Two singletons are now used to control the state of the light.
//  *  - light_state (LightStateClass) stores the color / white temperature and
//  *    brightness. Use this object to READ only.
//  *  - light_controller (LightControllerClass) is used to change light state
//  *    and adjust all Settings and levels accordingly.
//  *    Always use this object to change light status.
//  *
//  * As there have been lots of changes in light control, here is a summary out
//  * the whole flow from setting colors to drving the PMW pins.
//  *
//  * 1.  To change colors, always use 'light_controller' object.
//  *     'light_state' is only to be used to read current state.
//  *  .a For color bulbs, set color via changeRGB() or changeHS() for Hue/Sat.
//  *     Set the overall brightness changeBri(0..255) or changeDimmer(0..100%)
//  *     RGB and Hue/Sat are always kept in sync. Internally, RGB are stored at
//  *     full range (max brightness) so that when you reduce brightness and
//  *     raise it back again, colors don't change due to rounding errors.
//  *  .b For white bulbs with Cold/Warm colortone, use changeCW() or changeCT()
//  *     to change color-tone. Set overall brightness separately.
//  *     Color-tone temperature can range from 153 (Cold) to 500 (Warm).
//  *     SetOption82 can expand the rendering from 200-380 due to Alexa reduced range.
//  *     CW channels are stored at full brightness to avoid rounding errors.
//  *  .c Alternatively, you can set all 5 channels at once with changeChannels(),
//  *     in this case it will also set the corresponding brightness.
//  *
//  * 2.a After any change, the Settings object is updated so that changes
//  *     survive a reboot and can be stored in flash - in saveSettings()
//  *  .b Actual channel values are computed from RGB or CT combined with brightness.
//  *     Range is still 0..255 (8 bits) - in getActualRGBCW()
//  *  .c The 5 internal channels RGBWC are mapped to the actual channels supported
//  *     by the Settings.light_settings.type: in UpdateFinalColourComponents()
//  *     1 channel  - 0:Brightness
//  *     2 channels - 0:Coldwhite 1:Warmwhite
//  *     3 channels - 0:Red 1:Green 2:Blue
//  *     4 chennels - 0:Red 1:Green 2:Blue 3:White
//  *     5 chennels - 0:Red 1:Green 2:Blue 3:ColdWhite 4:Warmwhite
//  *
//  * 3.  In LightAnimate(), final PWM values are computed at next tick.
//  *  .a If color did not change since last tick - ignore.
//  *  .b Extend resolution from 8 bits to 10 bits, which makes a significant
//  *     difference when applying gamma correction at low brightness.
//  *  .c Apply Gamma Correction if LedTable==1 (by default).
//  *     Gamma Correction uses an adaptative resolution table from 11 to 8 bits.
//  *  .d For Warm/Cold-white channels, Gamma correction is calculated in combined mode.
//  *     Ie. total white brightness (C+W) is used for Gamma correction and gives
//  *     the overall light power required. Then this light power is split among
//  *     Wamr/Cold channels.
//  *  .e Gamma correction is still applied to 8 bits channels for compatibility
//  *     with other non-PMW modules.
//  *  .f Apply color balance correction from rgbwwTable[].
//  *     Note: correction is done after Gamma correction, it is meant
//  *     to adjust leds with different power
//  *  .g If rgbwwTable[4] is zero, blend RGB with White and adjust the level of
//  *     White channel according to rgbwwTable[3]
//  *  .h Scale ranges from 10 bits to 0..PWMRange (by default 1023) so no change
//  *     by default.
//  *  .i Apply port remapping from Option37
//  *  .j Invert PWM value if port is of type PMWxi instead of PMWx
//  *  .k Apply PWM value with analogWrite() - if pin is configured
//  *
// \*********************************************************************************************/

// // #ifdef USE_LIGHT
// /*********************************************************************************************\
//  * PWM, WS2812, Sonoff B1, AiLight, Sonoff Led and BN-SZ01, H801, MagicHome and Arilux
//  *
//  * Settings.light_settings.type  Module     Color  ColorTemp  Modules
//  * ----------  ---------  -----  ---------  ----------------------------
//  *  0          -                 no         (Sonoff Basic)
//  *  1          PWM1       W      no         (Sonoff BN-SZ)
//  *  2          PWM2       CW     yes        (Sonoff Led)
//  *  3          PWM3       RGB    no         (H801, MagicHome and Arilux LC01)
//  *  4          PWM4       RGBW   no         (H801, MagicHome and Arilux)
//  *  5          PWM5       RGBCW  yes        (H801, Arilux LC11)
//  *  9          reserved          no
//  * 10          reserved          yes
//  * 11          +WS2812    RGB    no         (One WS2812 RGB or RGBW ledstrip)
//  * 12          AiLight    RGBW   no
//  * 13          Sonoff B1  RGBCW  yes
//  *
//  * light_scheme  WS2812  3+ Colors  1+2 Colors  Effect
//  * ------------  ------  ---------  ----------  -----------------
//  *  0            yes     yes        yes         Color On/Off
//  *  1            yes     yes        yes         Wakeup light
//  *  2            yes     yes        no          Color cycle RGB
//  *  3            yes     yes        no          Color cycle RBG
//  *  4            yes     yes        no          Random RGB colors
//  *  5            yes     no         no          Clock
//  *  6            yes     no         no          Incandescent
//  *  7            yes     no         no          RGB
//  *  8            yes     no         no          Christmas
//  *  9            yes     no         no          Hanukkah
//  * 10            yes     no         no          Kwanzaa
//  * 11            yes     no         no          Rainbow
//  * 12            yes     no         no          Fire
//  *
// \*********************************************************************************************/

// /*********************************************************************************************\
//  *
//  * Light management has been refactored to provide a cleaner class-based interface.
//  * Also, now all values are stored as integer, no more floats that could generate
//  * rounding errors.
//  *
//  * Two singletons are now used to control the state of the 
//  *  - light_state (LightStateClass) stores the color / white temperature and
//  *    brightness. Use this object to READ only.
//  *  - light_controller (LightControllerClass) is used to change light state
//  *    and adjust all Settings and levels accordingly.
//  *    Always use this object to change light status.
//  *
//  * As there have been lots of changes in light control, here is a summary out
//  * the whole flow from setting colors to drving the PMW pins.
//  *
//  * 1.  To change colors, always use 'light_controller' object.
//  *     'light_state' is only to be used to read current state.
//  *  .a For color bulbs, set color via changeRGB() or changeHS() for Hue/Sat.
//  *     Set the overall brightness changeBri(0..255) or changeDimmer(0..100%)
//  *     RGB and Hue/Sat are always kept in sync. Internally, RGB are stored at
//  *     full range (max brightness) so that when you reduce brightness and
//  *     raise it back again, colors don't change due to rounding errors.
//  *  .b For white bulbs with Cold/Warm colortone, use changeCW() or changeCT()
//  *     to change color-tone. Set overall brightness separately.
//  *     Color-tone temperature can range from 153 (Cold) to 500 (Warm).
//  *     SetOption82 can expand the rendering from 200-380 due to Alexa reduced range.
//  *     CW channels are stored at full brightness to avoid rounding errors.
//  *  .c Alternatively, you can set all 5 channels at once with changeChannels(),
//  *     in this case it will also set the corresponding brightness.
//  *
//  * 2.a After any change, the Settings object is updated so that changes
//  *     survive a reboot and can be stored in flash - in saveSettings()
//  *  .b Actual channel values are computed from RGB or CT combined with brightness.
//  *     Range is still 0..255 (8 bits) - in getActualRGBCW()
//  *  .c The 5 internal channels RGBWC are mapped to the actual channels supported
//  *     by the Settings.light_settings.type: in UpdateFinalColourComponents()
//  *     1 channel  - 0:Brightness
//  *     2 channels - 0:Coldwhite 1:Warmwhite
//  *     3 channels - 0:Red 1:Green 2:Blue
//  *     4 chennels - 0:Red 1:Green 2:Blue 3:White
//  *     5 chennels - 0:Red 1:Green 2:Blue 3:ColdWhite 4:Warmwhite
//  *
//  * 3.  In LightAnimate(), final PWM values are computed at next tick.
//  *  .a If color did not change since last tick - ignore.
//  *  .b Extend resolution from 8 bits to 10 bits, which makes a significant
//  *     difference when applying gamma correction at low brightness.
//  *  .c Apply Gamma Correction if LedTable==1 (by default).
//  *     Gamma Correction uses an adaptative resolution table from 11 to 8 bits.
//  *  .d For Warm/Cold-white channels, Gamma correction is calculated in combined mode.
//  *     Ie. total white brightness (C+W) is used for Gamma correction and gives
//  *     the overall light power required. Then this light power is split among
//  *     Wamr/Cold channels.
//  *  .e Gamma correction is still applied to 8 bits channels for compatibility
//  *     with other non-PMW modules.
//  *  .f Apply color balance correction from rgbwwTable[].
//  *     Note: correction is done after Gamma correction, it is meant
//  *     to adjust leds with different power
//  *  .g If rgbwwTable[4] is zero, blend RGB with White and adjust the level of
//  *     White channel according to rgbwwTable[3]
//  *  .h Scale ranges from 10 bits to 0..PWMRange (by default 1023) so no change
//  *     by default.
//  *  .i Apply port remapping from Option37
//  *  .j Invert PWM value if port is of type PMWxi instead of PMWx
//  *  .k Apply PWM value with analogWrite() - if pin is configured
//  *
// \*********************************************************************************************/



// //
// // LightStateClass
// // This class is an abstraction of the current light state.
// // It allows for b/w, full colors, or white colortone
// //
// // This class has 2 independant slots
// // 1/ Brightness 0.255, dimmer controls both RGB and WC (warm-cold)
// // 1/ RGB and Hue/Sat - always kept in sync and stored at full brightness,
// //    i.e. R G or B are 255
// //    briRGB specifies the brightness for the RGB slot.
// //    If Brightness is 0, it is equivalent to Off (for compatibility)
// //    Dimmer is Brightness converted to range 0..100
// // 2/ White with colortone - or CW (Cold / Warm)
// //    ct is 153..500 temperature (153=cold, 500=warm)
// //    briCT specifies the brightness for white channel
// //
// // Dimmer (0.100) is automatically derived from brightness
// //
// // INVARIANTS:
// // 1.  RGB components are always stored at full brightness and modulated with briRGB
// //     ((R == 255) || (G == 255) || (B == 255))
// // 2.  RGB and Hue/Sat are always kept in sync whether you use setRGB() or setHS()
// // 3.  Warm/Cold white channels are always stored at full brightness
// //     ((WW == 255) || (WC == 255))
// // 4.  WC/WW and CT are always kept in sync.
// //     Note: if you use setCT() then WC+WW == 255 (both channels are linked)
// //     but if you use setCW() both channels can be set independantly
// // 5.  If RGB or CT channels are deactivated, then corresponding brightness is zero
// //     if (colot_tone == LCM_RGB) then briCT = 0
// //     if (color_tone == LCM_CT)  then briRGB = 0
// //     if (colot_tone == LCM_BOTH) then briRGB and briCT can have any values
// //
// // Note:  If you want the actual RGB, you need to multiply with Bri, or use getActualRGBCW()
// // Note: all values are stored as unsigned integer, no floats.
// // Note: you can query vaules from this singleton. But to change values,
// //   use the LightController - changing this object will have no effect on actual 
// //