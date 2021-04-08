#ifndef _DRIVER_INTERFACE_LIGHTS1_H
#define _DRIVER_INTERFACE_LIGHTS1_H 0.1

#define D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID 140

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

//#ifdef ENABLE_DEVFEATURE_JSONPARSER
#include "JsonParser.h"
//#endif // ENABLE_DEVFEATURE_JSONPARSER


#include "6_Lights/Palette/mPalette.h"

// #include "6_Lights/RGBCCT/RgbcctType.h"


enum PIXEL_MULTIPLIER_MODE_IDS{
  PIXEL_MULTIPLIER_MODE_NONE_ID,
  PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID,
  PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID,
  PIXEL_MULTIPLIER_MODE_MAPPED_INDEX_ARRAY_ID
};

// TRANSITION_METHOD
// DEFINE_PGM_CTR(PM_TRANSITION_METHOD_NONE_NAME_CTR)    "None";   
// DEFINE_PGM_CTR(PM_TRANSITION_METHOD_BLEND_NAME_CTR)    "Blend";      
// DEFINE_PGM_CTR(PM_TRANSITION_METHOD_INSTANT_NAME_CTR)  "Instant";      
// DEFINE_PGM_CTR(PM_TRANSITION_METHOD_TWINKLE_NAME_CTR)  "Twinkle";     
// DEFINE_PGM_CTR(PM_TRANSITION_METHOD_GLIMMER_NAME_CTR)  "Glimmer";


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



#define D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR "RGBCCT_PWM"
#define D_PIXEL_HARDWARE_TYPE_WS28XX_CTR "WS28XX"


DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)       D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR;
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)       D_PIXEL_HARDWARE_TYPE_WS28XX_CTR;


// #define CLEAR_PALETTELIST_WITH_PTR(x) memset(x,0,sizeof(PALETTELIST::PALETTE));

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

// enum PIXEL_HARDWARE_COLOR_ORDER_IDS{
//   PIXEL_HARDWARE_COLOR_ORDER_GRB_ID = 0, //default
//   PIXEL_HARDWARE_COLOR_ORDER_RGB_ID, //common for WS2811
//   PIXEL_HARDWARE_COLOR_ORDER_BRG_ID, 
//   PIXEL_HARDWARE_COLOR_ORDER_RBG_ID, 
//   PIXEL_HARDWARE_COLOR_ORDER_BGR_ID, 
//   PIXEL_HARDWARE_COLOR_ORDER_GBR_ID,
//   PIXEL_HARDWARE_COLOR_ORDER_GRBW_ID,
//   PIXEL_HARDWARE_COLOR_LENGTH_ID
// };

/**************
 * BLEND - Move between colours with delay
 * INSTANT   - no delay
 * TWINKLE1 - random bulbs flash
 * GLIMMER - random bulbs blend (flash with blend) - could use progress in animation random(progress)
**************/ 
// enum TRANSITION_METHOD{
//   TRANSITION_METHOD_NONE_ID,
//   TRANSITION_METHOD_BLEND_ID,
//   TRANSITION_METHOD_INSTANT_ID,
//   TRANSITION_METHOD_TWINKLE_ID,
//   TRANSITION_METHOD_GLIMMER_ID, //should be an animation only, on brightness #Idea#105
//   TRANSITION_METHOD_LENGTH_ID
// };   

    /** PATTERNS ************
     * RANDOM - LED pixel indexing chosen at random
     * INORDER - first to last pixel index
     * CENTRE_OUT   - Chosen from center out, showing same pixel on outward pairs (update together)
     * GRADIENT - takes any palette, and stretches the palette across the leds
     *    -- Full span
     *    -- Repeated X times   * 
     * MAPPED -
    **************/ 

    enum PALETTE_PATTERN_IDS{ //TRANSITION_ORDER{
      TRANSITION_ORDER_NONE_ID=0,
      TRANSITION_ORDER_RANDOM_ID,
      TRANSITION_ORDER_INORDER_ID,
      TRANSITION_ORDER_CENTRE_OUT_ID,
      TRANSITION_ORDER_GRADIENT_ID,
      TRANSITION_ORDER_LENGTH_ID
    }; 


#include "1_TaskerManager/mTaskerManager.h"

#include "3_Network/MQTT/mMQTT.h"
#include "2_CoreSystem/Time/mTime.h"

#include "6_Lights/Palette/mPalette_Progmem.h"

#define LST_MAX 5

// DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PARAMETERS_CTR) "values/parameters";


  #define WEB_CLASS_RGB_LIVE "rgb_live"
  
    // Arrays to hold palettes, called via pointer
    // Variable user maps need to be variable  
    #ifndef D_PALETTE_HSBID_NAME_CTR
    #define D_PALETTE_HSBID_NAME_CTR        "User"   
    #endif
    #ifndef D_PALETTE_RGBCCT_COLOURS_NAME_CTR
    #define D_PALETTE_RGBCCT_COLOURS_NAME_CTR        "RGBCCTColour"   
    #endif
    #define PALETTELIST_COLOUR_HSBID_AMOUNT_MAX PALETTELIST_COLOUR_AMOUNT_MAX


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

  
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SCENE_CTR) "scene";


DEFINE_PGM_CTR(PM_ANIMATION_MODE_NONE_NAME_CTR )   "None"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_CHANGE_POWER_NAME_CTR )   "Change Power"     ;    
// DEFINE_PGM_CTR(PM_ANIMATION_MODE_TURN_OFF_NAME_CTR )   "Turn Off"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR   )      "Ambilight"  ;                
// DEFINE_PGM_CTR(PM_ANIMATION_MODE_SCENE_NAME_CTR   )          "Scene"      ;             
DEFINE_PGM_CTR(PM_ANIMATION_MODE_EFFECTS_NAME_CTR  )         D_JSON_EFFECTS;
#ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS       
DEFINE_PGM_CTR(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)   D_JSON_NOTIFICATIONS;      
#endif   
#ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR) "Manual SetPixel";
#endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  
//     #define D_MODE_SINGLECOLOUR_NOTACTIVE_NAME_CTR       "NOTACTIVE"    
//     #define D_MODE_SINGLECOLOUR_DAYON_NAME_CTR           "DAYON"    
//     #define D_MODE_SINGLECOLOUR_DAYOFF_NAME_CTR          "DAYOFF"    
//     #define D_MODE_SINGLECOLOUR_EVENINGON_NAME_CTR       "EVENINGON"    
//     #define D_MODE_SINGLECOLOUR_EVENINGOFF_NAME_CTR      "EVENINGOFF"    
//     #define D_MODE_SINGLECOLOUR_MIDNIGHTON_NAME_CTR      "MIDNIGHTON"    
//     #define D_MODE_SINGLECOLOUR_MIDNIGHTOFF_NAME_CTR     "MIDNIGHTOFF"    
// DEFINE_PGM_CTR(PM_MODE_SINGLECOLOUR_COLOURSCENE_NAME_CTR)   "ColourSingle"; //COLOURSCENE
// DEFINE_PGM_CTR(PM_MODE_SINGLECOLOUR_PALETTE_SINGLE_NAME_CTR) "PaletteSingle";
// DEFINE_PGM_CTR(PM_MODE_SINGLECOLOUR_PALETTELOOP_NAME_CTR)   "PaletteLoop";
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


#include "1_TaskerManager/mTaskerManager.h"


class mInterfaceLight :
  public mTaskerInterface
{
  public:
    mInterfaceLight(){};

    
    static const char* PM_MODULE_LIGHTS_INTERFACE_CTR;
    static const char* PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_INTERFACE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mInterfaceLight);
    };
    #endif



    /**
     * Module settings other tasks/module may need
     * */
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
      
      //phase out
      // uint8_t pixel_hardware_color_order_id = PIXEL_HARDWARE_COLOR_ORDER_RGB_ID;


      // uint16_t strip_size = STRIP_SIZE_MAX; //allow variable control of size
      uint16_t light_size_count = 1;
      uint8_t pwm_offset = 0;                 // Offset in color buffer
    }settings;

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
        };
        union{
          uint16_t white_cold : 3; // bit 12-14
          uint16_t c : 3; // bit 12-14
        };
        uint16_t reserved : 1;   // bit 15
      };
    } HARDWARE_ELEMENT_COLOUR_ORDER;

    HARDWARE_ELEMENT_COLOUR_ORDER hardware_element_colour_order[2];
    // FUTURE, using range values (0-50 = this, 51-100 = that) or wled segment style


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

        uint8_t use_gamma_for_brightness : 1;

        // Reserved
        uint16_t reserved : 4;
      };
    } ANIMATION_FLAGS;


    struct ANIMATION_SETTINGS{
      /**
       * Flags
       * */
      ANIMATION_FLAGS flags;
      /**
       * Effects (Scenes & Flasher), Ambilight, Adalight
       * */
      uint8_t mode_id;
      /**
       * Palette (Options on getting from palette)
       * */
      struct PALETTE{
        uint8_t id; 
        uint8_t pattern; //Single, Gradient 
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
        uint8_t order_id;
        /**
         * Change method (eg Blend, Instant)
         * */
        uint8_t method_id;
        /**
         * Pixels to change up to maximum of segment
         * */
        uint16_t pixels_to_update_as_number;
        /**
         * Refresh rate, calculate new colours
         * */
        uint16_t rate_ms = 5000;
        /**
         * Time to new colours
         * */
        uint16_t time_ms = 5000;  
      }transition;
      /**
       * WLED Test Variables 
       * */
      uint16_t start_index;
      uint16_t stop_index; //segment invalid if stop == 0
      uint8_t speed; 
      uint8_t intensity;
      uint8_t options; //bit pattern: msb first: transitional needspixelstate tbd tbd (paused) on reverse selected
      uint8_t grouping;
      uint8_t spacing;
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
        return stop_index > start_index;
      }
      uint16_t length()
      {
        return stop_index - start_index;
      }
      uint16_t groupLength()
      {
        return grouping + spacing;
      }
      uint16_t virtualLength()
      {
        uint16_t groupLen = groupLength();
        uint16_t vLength = (length() + groupLen - 1) / groupLen;
        // if (options & MIRROR)
        //   vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED
        return vLength;
      }   
    };
    // store the current state
    ANIMATION_SETTINGS animation;
    // store animation to return to, use override when possible
    ANIMATION_SETTINGS animation_stored;

    #define MAX_NUM_SEGMENTS 2
    ANIMATION_SETTINGS animation_segment[MAX_NUM_SEGMENTS];
    struct SEGMENT_RUNTIME{
      uint8_t _segment_index = 0;
      uint8_t _segment_index_palette_last = 99;
    };

    bool flag_test= false;
    uint16_t    pwm_test = 0;

    // segment _segments[MAX_NUM_SEGMENTS];
    //  = { // SRAM footprint: 24 bytes per element
    //   // start, stop, speed, intensity, palette, mode, options, grouping, spacing, opacity (unused), color[]
    //   { 0, 7, DEFAULT_SPEED, 128, 0, DEFAULT_MODE, NO_OPTIONS, 1, 0, 255, {DEFAULT_COLOR}}
    // };

    //Temp array, to be later absorbed like wled
    #define D_EFFECTS_DATA_BUFFER_SIZE 100
    uint8_t effects_data_buffer[D_EFFECTS_DATA_BUFFER_SIZE];
    

    const char* GetHardwareColourTypeName(char* buffer, uint8_t buflen);
    const char* GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen);
    int8_t GetHardwareColourTypeIDbyName(const char* c);

    // Flags and states that are used during one transition and reset when completed
    struct ANIMATIONOVERRIDES{
      uint8_t fRefreshAllPixels = false;
      uint16_t time_ms = 1000; //on boot
    }animation_override; // ie "oneshot" variables that get checked and executed one time only
    
    NeoPixelAnimator* animator_controller = nullptr;
    void Init_NeoPixelAnimator(uint16_t size, uint8_t timebase);  

    // Values not ever saved
    struct RUNTIME_VALUES{
      uint32_t animation_changed_millis = 0;
    }runtime;


void LightCalcPWMRange();

    /**************
     * CHANGE_POWER - TURN_ON - fade ON, returns to previous lighting array.  TURN_OFF - fade off, either turns off, or calls "SCENE" to set number = ALSO SAVES CURRENT OUTPUT
     * NOTHING - to be named "off", does nothing, simply remains off
     * AMBILIHGT - keep, dell screen, set directly
     * PRESETS - animation from a group of colour patterns
     * EFFECTS - christmad themes, standard 8, shimmering/twinkling. Now also SCENE - scene names might be same as presets? or preset subset of scene? scene might best be named "direct"
    **************/ 
    enum ANIMATION_MODE{
      ANIMATION_MODE_NONE_ID = 0,
      ANIMATION_MODE_CHANGE_POWER_ID = 0,
      //#ifdef ENABLE_PIXEL_FUNCTION_AMBILIGHT
      ANIMATION_MODE_AMBILIGHT_ID,
      //#endif // ENABLE_PIXEL_FUNCTION_AMBILIGHT
      //#ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
      ANIMATION_MODE_EFFECTS_ID,
      //#endif // ENABLE_PIXEL_FUNCTION_EFFECTS
      //#ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS
      ANIMATION_MODE_NOTIFICATIONS_ID,
      //#endif
      #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      ANIMATION_MODE_MANUAL_SETPIXEL_ID,
      #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
      ANIMATION_MODE_LENGTH_ID
    };             
    int8_t GetAnimationModeIDbyName(const char* c);
    const char* GetAnimationModeName(char* buffer, uint16_t buflen);
    const char* GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen);

  
    enum LIHGT_POWER_STATE_IDS{
      LIGHT_POWER_STATE_OFF_ID=0,
      LIGHT_POWER_STATE_ON_ID,
      LIGHT_POWER_STATE_TOGGLE_ID,
      LIGHT_POWER_STATE_PAUSED,
      LIGHT_POWER_STATE_ANIMATING_ID,
      LIGHT_POWER_STATE_LENGTH_ID
    };
    // void SetAnimationProfile(uint8_t profile_id);

    /******************************************************************************************************************************
    ****************** CommandSet_x *************************************************************************************************************
    ******************************************************************************************************************************/

    void CommandSet_PixelHardwareTypeID(uint8_t value);
    void CommandSet_LightPowerState(uint8_t value);

    // Global and shared with activergb
    void CommandSet_Brt_255(uint8_t value);
    void CommandSet_BrtRGB_255(uint8_t bri);
    void CommandSet_BrtCT_255(uint8_t bri);

    void CommandSet_ActiveSolidPalette_Hue_360(uint16_t value);
    void CommandSet_ActiveSolidPalette_Sat_255(uint8_t value);
    void CommandSet_ActiveSolidPalette_ColourTemp(uint16_t ct);
    bool CommandSet_ActiveSolidPalette_RGBCT_Linked(uint16_t ct_rgb_linked);
    void CommandSet_ActiveSolidPalette_Raw(uint8_t* values);

    void CommandSet_Animation_Transition_Time_Ms(uint16_t value);
    void CommandSet_Animation_Transition_Rate_Ms(uint16_t value);
    void CommandSet_Auto_Time_Off_Secs(uint16_t value);
    // #ifndef ENABLE_DEVFEATURE_PHASING_SCENE_OUT
    // void CommandSet_SingleColourMode_ID(uint8_t value);
    // #endif// ENABLE_DEVFEATURE_PHASING_SCENE_OUT
    void CommandSet_PaletteID(uint8_t value);
    void CommandSet_AnimationModeID(uint8_t value);
    void CommandSet_HardwareColourOrderTypeByStr(const char* value);
    void CommandSet_LightSizeCount(uint16_t value);
    void CommandSet_LightsCountToUpdateAsNumber(uint16_t value);
    void CommandSet_LightsCountToUpdateAsPercentage(uint8_t value);
    void CommandSet_TransitionOrderID(uint8_t value);
    void CommandSet_EnabledAnimation_Flag(uint8_t value);
    void CommandSet_PaletteColour_RGBCCT_Raw_By_ID(uint8_t palette_id, uint8_t* buffer, uint8_t buflen);
    void CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(uint8_t palette_id);
    
    /******************************************************************************************************************************
    ****************** CommandGet_x *************************************************************************************************************
    ******************************************************************************************************************************/

    // int8_t GetTransitionMethodIDbyName(const char* c);     
    // const char* GetTransitionMethodName(char* buffer, uint8_t buflen);
    // const char* GetTransitionMethodNameByID(uint8_t id, char* buffer, uint8_t buflen);

    int8_t GetTransitionOrderIDbyName(const char* c);     
    const char* GetTransitionOrderName(char* buffer, uint8_t buflen);
    const char* GetTransitionOrderNameByID(uint8_t id, char* buffer, uint8_t buflen);

    uint16_t GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage);
    uint8_t  GetPixelsToUpdateAsPercentageFromNumber(uint16_t number);

    void CheckHardwareElementColourOrder();

    
// #ifndef ENABLE_DEVFEATURE_PHASING_TAS_CCT_OUT
//     uint16_t getCT10bits();
//     void setCTRange(uint16_t ct_min_range, uint16_t ct_max_range) ;
//     void getCTRange(uint16_t ct_min_range, uint16_t ct_max_range) ;
//     void getCTRange(uint16_t *ct_min_range, uint16_t *ct_max_range) ;
//     uint16_t getHue();
//     uint8_t  getSat();
//     uint8_t getColorMode();

//      uint8_t setBriRGB(uint8_t bri_rgb);
//     uint8_t setBriRGB_As_Percentage(uint8_t bri_rgb);

//     void getXY(float *x, float *y);
//     void setBri(uint8_t bri);
   
//  uint8_t SetBrightnessCCT255(uint8_t bri_ct);
//     uint8_t getBriRGB();
//     void setDimmer(uint8_t dimmer);
//     void setCT(uint16_t ct);
//     void setCW(uint8_t c, uint8_t w, bool free_range = false);
//     uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false);
//     void setHS(uint16_t hue, uint8_t sat);
//     void setChannelsRaw(uint8_t *channels);
//     void setChannels(uint8_t *channels);
//     void RgbToHsb(uint8_t r, uint8_t g, uint8_t b, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri);
//     void HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b);
//     void RgbToXy(uint8_t i_r, uint8_t i_g, uint8_t i_b, float *r_x, float *r_y);
//     void XyToRgb(float x, float y, uint8_t *rr, uint8_t *rg, uint8_t *rb);
//     void setSubType(uint8_t sub_type);
//     bool CommandSet_RGBCT_Linked(bool ct_rgb_linked);
//     void setAlexaCTRange(bool alexa_ct_range);
//     bool Get_RGBCT_Linked();
//     bool setPWMMultiChannel(bool pwm_multi_channels);
//     bool isPWMMultiChannel(void);
//     void debugLogs();
//     void loadSettings();
//     void changeCTB(uint16_t new_ct, uint8_t briCT);
//     void changeDimmer(uint8_t dimmer, uint32_t mode = 0);
//     void changeBri(uint8_t bri);
//     void changeRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false);
//     void UpdateFinalColourComponents(uint8_t *current_color = nullptr);
//     void changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB);   
   
//     power_t LightPower(void);
//     power_t LightPowerIRAM(void);
//     uint8_t LightDevice(void);
//     uint8_t setColorMode(uint8_t cm);
//     void addRGBMode();
//     void addCTMode();
//     void getRGB(uint8_t *r, uint8_t *g, uint8_t *b);
//     void getCW(uint8_t *rc, uint8_t *rw);
//     void getActualRGBCW(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w);
//     uint8_t getChannels(uint8_t *channels);
//     void getChannelsRaw(uint8_t *channels);
//     void getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri);
//     uint8_t getBri(void);
//     uint8_t getBriCT();
//     static inline uint8_t DimmerToBri(uint8_t dimmer);
//     static uint8_t BriToDimmer(uint8_t bri);
//     uint8_t getDimmer(uint32_t mode = 0);
//     uint16_t getCT();
// #endif // ENABLE_DEVFEATURE_PHASING_TAS_CCT_OUT

    int8_t GetPixelHardwareTypeIDbyName(const char* c);
    const char* GetPixelHardwareTypeName(char* buffer);
    const char* GetPixelHardwareTypeNamebyID(uint8_t id, char* buffer);


void SetPixelColourHardwareInterface(RgbcctColor colour_hardware, uint16_t index = 0, bool flag_replicate_for_total_pixel_length = false);
RgbcctColor GetPixelColourHardwareInterface(uint16_t index = 0);
void ShowInterface();


    /******************************************************************************************************************************
    ****************** unsorted *************************************************************************************************************
    ******************************************************************************************************************************/



    
    void Settings_Default();  

    void BootMessage();

    void EveryLoop();

    void SubTask_AutoOff();
    struct AUTO_OFF_SETTINGS{
      uint16_t time_decounter_secs = 0;
      uint32_t tSaved = 0;
    }auto_off_settings;
    
    uint32_t RgbColorto32bit(RgbColor rgb);
    
    RgbcctColor GetActiveFirstColourFromCurrentPalette();
    
    void StartFadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0) );
    void setChannelsRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t wc, uint8_t ww);

    RgbcctColor* active_rgbcct_colour_p = nullptr; //what is this then? internal conversions to output? (ie can I leave this as private)
    
    void Template_Load();

    int8_t CheckAndExecute_JSONCommands(void);
    void parse_JSONCommand(void);
  
    int8_t Tasker_Web(uint8_t function);
    #include "6_Lights/_Interface/mInterfaceLight_Web.h"

    int8_t Tasker(uint8_t function);
    void Init(void);
    

    void WebAppend_Root_Buttons();
    void WebAppend_Root_Draw_Table();
    void WebCommand_Parse();
    void WebAppend_Root_Draw_RGBLive();
    void WebAppend_Root_Draw_RGBTable(uint8_t rows = 8);

    void setChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t wc = 0, uint8_t ww = 0);

    
    // struct BRIGHTNESS_RGB_BOUNDARIES{
    //   uint8_t  lower = 0;  // 0..255
    //   uint8_t  upper = 255;  // 0..255
    // }brtRGB_limits;

    




    uint8_t light_power_state = 0;
    uint8_t light_power_Saved = 0;
    uint8_t subtype = 0;                    // LST_ subtype
    uint8_t device = 0;
    bool pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer
    bool     fade_initialized = false;      // dont't fade at startup
    uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
    uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023


    struct LIVEVIEW_SETTINGS{
      uint8_t show_type = RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID;
      uint8_t height_as_percentage = 15;
      uint8_t pixel_resolution_percentage = 100; //100% is send all, 0-99 is percentage of all
      uint16_t refresh_rate = 250;
    }liveview;


    struct PALETTE_VIEW_SETTINGS{
      uint8_t show_type = 2;//RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID;
      uint8_t height_as_percentage = 15;
      uint8_t pixel_resolution_percentage = 100; //100% is send all, 0-99 is percentage of all
    }palette_view;


    void init_Animations();
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


    RgbcctColor_Controller rgbcct_controller = RgbcctColor_Controller();

    bool LightModuleInit(void);

    float    HueN2F(uint16_t hue);
    float    SatN2F(uint8_t sat);
    float    BrtN2F(uint8_t brt);
    uint16_t HueF2N(float hue);
    uint8_t  SatF2N(float sat);
    uint8_t  BrtF2N(float brt);

    RgbcctColor GetRandomColour(RgbcctColor colour1, RgbcctColor colour2);
    HsbColor GetHSBColour();
    uint32_t WebColorFromColourMap(uint8_t i);
    // uint32_t WebColorFromColourType(RgbwColor rgb);
    uint32_t WebColorFromColourType(RgbColor rgb);

    RgbcctColor Color32bit2RgbColour(uint32_t colour32bit);

    void UpdateSetOutputs();  
    void ApplyGlobalBrightnesstoColour(RgbcctColor* colour);

    
    RgbColor GetColourValueUsingMaps(float value, uint8_t map_style_id, float value_min=0, float value_max=0,  bool map_is_palette_id = false);

    
    uint8_t ConstructJSON_Scene(uint8_t json_level = 0);
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
    uint8_t ConstructJSON_Debug(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
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

    
// void CommandSet_ColourTemp(uint16_t ct)
// {
  
//   // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//   // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
//   // #endif //#ifdef ENABLE_LOG_LEVEL_COMMANDS

// /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
//  * ct = 153 = 6500K = Cold = CCWW = FF00
//  * ct = 600 = 2000K = Warm = CCWW = 00FF  */
//   // don't set CT if not supported
//   if ((_LST_COLDWARM != _subtype) && (_LST_RGBCW != _subtype)) {
//     return;
//   }
//   changeCTB(ct, getBriCT());
// }





    uint8_t getBri_Global(void) { // return the max of _briCT and _briRGB
      return (_briRGB_Global >= _briCT_Global) ? _briRGB_Global : _briCT_Global;
    }
    uint8_t getBriRGB_Global(){
      return _briRGB_Global;
    }
    void setBriRGB_Global(uint8_t bri_rgb) {
      _briRGB_Global = bri_rgb;
    }
    void setBriCT_Global(uint8_t bri_ct) {
      _briCT_Global = bri_ct;
    }

private:

// #ifndef ENABLE_DEVFEATURE_PHASING_TAS_CCT_OUT
//     // CT min and max
//     const uint16_t CT_MIN = 153;          // 6500K
//     const uint16_t CT_MAX = 500;          // 2000K
//     // Ranges used for Alexa
//     const uint16_t CT_MIN_ALEXA = 200;    // also 5000K
//     const uint16_t CT_MAX_ALEXA = 380;    // also 2600K

//     uint16_t _hue = 0;  // 0..359
//     uint8_t  _sat = 255;  // 0..255
//     // are RGB and CT linked, i.e. if we set CT then RGB channels are off
//     bool     _ct_rgb_linked = true;
//     bool     _pwm_multi_channels = false;    // treat each channel as independant dimmer
//     uint8_t  _subtype = 0;  // local copy of Light.subtype, if we need multiple lights
//     uint16_t _ct = CT_MIN;  // 153..500, default to 153 (cold white)
//     uint8_t  _color_mode = LCM_RGB; // RGB by default
// #endif // ENABLE_DEVFEATURE_PHASING_TAS_CCT_OUT

    // Please note that you can still set CT to 153..500, but any value below _ct_min_range or above _ct_max_range not change the CT
    uint16_t _ct_min_range = 153;   // the minimum CT rendered range
    uint16_t _ct_max_range = 500;   // the maximum CT rendered range


  // I need to keep brightness on its own, with functions for it  
    uint8_t  _briRGB_Global = 255;  // 0..255 // Used for ws28xx
    uint8_t  _briCT_Global = 255;






};
#endif

#endif




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



/**
{"AnimationMode":"Scene","SceneName":"ColourSingle","hue":120,"sat":100,"brt_rgb":5,"brt_cct":100}
{"SceneName":"PaletteSingle","ColourPalette":31,"brt_rgb":20,"brt_cct":100,"time_ms":1000}
{"SceneName":"PaletteSingle","ColourPalette":31,"brt_rgb":100,"brt_cct":100,"time_ms":1000}
{"AnimationMode":"Scene","SceneName":"ColourSingle","hue":0,"sat":100,"cct_temp":600,"brt_rgb":100,"brt_cct":10,"time_ms":10000}
{"AnimationMode":"Flasher","ColourPalette":1,"brt_rgb":100,"brt_cct":1,"time_ms":1000}
{"AnimationMode":"Flasher","flasher":{"function":1},"transition":{"order":"Random","rate":1},"ColourPalette":1,"brt_rgb":100,"brt_cct":1,"time_ms":1000}
{"AnimationMode":"Flasher","flasher":{"function":1},"transition":{"order":"Random","rate_ms":1000},"ColourPalette":1,"brt_rgb":100,"brt_cct":1,"time_ms":1000}
*/
