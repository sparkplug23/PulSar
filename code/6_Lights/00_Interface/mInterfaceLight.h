#ifndef _DRIVER_INTERFACE_LIGHTS1_H
#define _DRIVER_INTERFACE_LIGHTS1_H 0.1

#define D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID ((6*1000)+00)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

// New sections of code that should be enabled by default
// #define ENABLE_CRGBPALETTES_IN_PROGMEM
// #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
#define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS


  #define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_HSBID__NAME_CTR   "HsbID %02d"
  #define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR  "Solid Rgbcct %02d"
  #define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_GENERIC__NAME_CTR "Generic %02d"


#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"

#include "JsonParser.h"

#ifdef ENABLE_DEVFEATURE_PALETTE_LOADED_AS_NEW_CLASS
#include "6_Lights/02_Palette/mColourUtils.h"
#endif // ENABLE_DEVFEATURE_PALETTE_LOADED_AS_NEW_CLASS

#ifdef ENABLE_DEVFEATURE_NEWPALETTE_CONTAINER
#include "6_Lights/02_Palette/aPaletteContainer.h"
// #else
#include "6_Lights/02_Palette/mPalette.h"
#endif





//Required as defualt for now
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

enum PIXEL_MULTIPLIER_MODE_IDS{
  PIXEL_MULTIPLIER_MODE_NONE_ID,
  /**
   * using a fixed amount, multiply the output/set, divide the input/get
   * */ 
  PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_UNIFORM_ID,
  /**
   * Random will not work, unless I record the mapped values
   * */
  // PIXEL_MULTIPLIER_MODE_BASIC_MULTIPLIER_RANDOM_ID,
  /**
   * I need this for the outside tree
   * */
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
#define D_PIXEL_HARDWARE_TYPE_SK6812_CTR "SK6812"


DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)       D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR;
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)       D_PIXEL_HARDWARE_TYPE_WS28XX_CTR;
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_SK6812_CTR)       D_PIXEL_HARDWARE_TYPE_SK6812_CTR;


// #define CLEAR_PALETTELIST_WITH_PTR(x) memset(x,0,sizeof(PALETTELIST::PALETTE));

enum LightTypes_IDS{
  LT_BASIC, //relay?
  // PWM types gives the amount of channels, but not the type of led, thats handled by rgbcw order
  LT_PWM1,    LT_PWM2, LT_PWM3,   LT_PWM4,  LT_PWM5,  LT_PWM6, LT_PWM7,
  LT_NU8,   LT_SERIAL1, LT_SERIAL2,   
  LT_LIGHT_INTERFACE_END,
  // Anything after this will not be handled in lightinterface
  LT_ADDRESSABLE_WS281X,  // this needs split out, into WS2812, SK6812
  LT_ADDRESSABLE_SK6812,
  LT_RGBW,  LT_RGBWC, // This are not needed, as they are either ADD or PWM
};


// enum LightSubtypes{ 
//   LST_NONE, 
//   LST_SINGLE, 
//   LST_COLDWARM, 
//   LST_RGB,   
//   LST_RGBW, 
//   LST_RGBWC, 
//   LST_RGBCCT, 
//   LST_RGBCW
// };



#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/02_Time/mTime.h"

#include "6_Lights/02_Palette/mPalette_Progmem.h"

#define LST_MAX 5

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__STATE__CTR) "state";
  
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_MODULE_CONFIG__CTR) "debug/module";


#define WEB_CLASS_RGB_LIVE "rgb_live"

// Arrays to hold palettes, called via pointer
// Variable user maps need to be variable  
// #ifndef D_PALETTE_HSBID_NAME_CTR
// #define D_PALETTE_HSBID_NAME_CTR        "User"   
// #endif
// #ifndef D_PALETTE_RGBCCT_COLOURS_NAME_CTR
// #define D_PALETTE_RGBCCT_COLOURS_NAME_CTR        "RGBCCTColour"   THESE ARE WRONG
// #endif
#define PALETTELIST_COLOUR_HSBID_AMOUNT_MAX PALETTELIST_COLOUR_AMOUNT_MAX


//#define DEBUG_LIGHT

typedef unsigned long power_t;              // Power (Relay) type


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


DEFINE_PGM_CTR(PM_ANIMATION_MODE_NONE_NAME_CTR )   "None"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_CHANGE_POWER_NAME_CTR )   "Change Power"     ;    
// DEFINE_PGM_CTR(PM_ANIMATION_MODE_TURN_OFF_NAME_CTR )   "Turn Off"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR   )      "Ambilight"  ;                
// DEFINE_PGM_CTR(PM_ANIMATION_MODE_SCENE_NAME_CTR   )          "Scene"      ;             
DEFINE_PGM_CTR(PM_ANIMATION_MODE_EFFECTS_NAME_CTR  )         D_JSON_EFFECTS;
DEFINE_PGM_CTR(PM_ANIMATION_MODE_WLED_NAME_CTR) "WLED";
#ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION       
DEFINE_PGM_CTR(PM_ANIMATION_MODE_NOTIFICATIONS_NAME_CTR)   D_JSON_NOTIFICATIONS;      
#endif   
#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR) "Manual SetPixel";
#endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

#define WEB_HANDLE_LIVEPIXELS_SHARED_JSON "/shared/rgb_livepixels.json"

enum RGB_VIEW_SHOW_TYPE_IDS{
  RGB_VIEW_SHOW_TYPE_NONE_ID=0,
  RGB_VIEW_SHOW_TYPE_ALWAYS_GRADIENT_ID,
  RGB_VIEW_SHOW_TYPE_ALWAYS_BLOCKS_ID,
  RGB_VIEW_SHOW_TYPE_FOLLOW_PALETTE_ID,

  RGB_VIEW_SHOW_TYPE_LENGTH_ID
};


#include "1_TaskerManager/mTaskerManager.h"

// #define mPaletteI (pCONT_iLight->mpalette)
// class mPalette;

class mInterfaceLight :
  public mTaskerInterface
{
  public:
    mInterfaceLight(){};
    bool Pre_Init(void);

    
    static const char* PM_MODULE_LIGHTS_INTERFACE_CTR;
    static const char* PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_INTERFACE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mInterfaceLight);
    };
    #endif

    // mPalette* mpalette = nullptr;//new mPalette();

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


    const char* GetHardwareColourTypeName(char* buffer, uint8_t buflen);
    const char* GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen);
    int8_t GetHardwareColourTypeIDbyName(const char* c);


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
      ANIMATION_MODE_CHANGE_POWER_ID,
      /**
       * @brief Segments effects
       **/
      ANIMATION_MODE_EFFECTS_ID,
      #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      ANIMATION_MODE_AMBILIGHT_ID,
      #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      #ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION
      ANIMATION_MODE_NOTIFICATIONS_ID,
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      ANIMATION_MODE_MANUAL_SETPIXEL_ID,
      #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      /**
       * @brief WLED is only included so debugging WLED effects insitu can be performed, no release build will include this option
       * **/
      ANIMATION_MODE_WLED_ID, 
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
    bool CommandGet_LightPowerState();

    void CommandSet_ActiveSolidPalette_RGB_Ctr(const char* rgb);
    void CommandSet_Brt_255(uint8_t value);
    void CommandSet_BrtRGB_255(uint8_t bri);
    void CommandSet_BrtCT_255(uint8_t bri);
    void CommandSet_ActiveSolidPalette_Hue_360(uint16_t value);
    void CommandSet_ActiveSolidPalette_Sat_255(uint8_t value);
    void CommandSet_ActiveSolidPalette_ColourTemp(uint16_t ct);
    bool CommandSet_ActiveSolidPalette_RGBCT_Linked(uint16_t ct_rgb_linked);
    void CommandSet_ActiveSolidPalette_Raw(uint8_t* values);
    void CommandSet_ActiveSolidPalette_Raw(uint8_t r,uint8_t g,uint8_t b,uint8_t ww,uint8_t wc);
    void CommandSet_ActiveSolidPalette_ColourTemp_Percentage(uint8_t percentage);
    void CommandSet_Auto_Time_Off_Secs(uint16_t value);
    // void CommandSet_PaletteID(uint8_t value);
    void CommandSet_AnimationModeID(uint8_t value);
    
    void CommandSet_LightSizeCount(uint16_t value);
    void CommandSet_EnabledAnimation_Flag(uint8_t value);
    void CommandSet_PaletteColour_RGBCCT_Raw_By_ID(uint8_t palette_id, uint8_t* buffer, uint8_t buflen);
    // void CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(uint8_t palette_id);
    
    /******************************************************************************************************************************
    ****************** CommandGet_x *************************************************************************************************************
    ******************************************************************************************************************************/

    uint16_t GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage);
    uint8_t  GetPixelsToUpdateAsPercentageFromNumber(uint16_t number);

    void CheckHardwareElementColourOrder();

    int8_t GetPixelHardwareTypeIDbyName(const char* c);
    const char* GetPixelHardwareTypeName(char* buffer);
    const char* GetPixelHardwareTypeNamebyID(uint8_t id, char* buffer);

    void SetPixelColourHardwareInterface(RgbcctColor colour_hardware, uint16_t index = 0, bool flag_replicate_for_total_pixel_length = false);
    RgbcctColor GetPixelColourHardwareInterface(uint16_t index = 0);
    void ShowInterface();


    /******************************************************************************************************************************
    ****************** unsorted *************************************************************************************************************
    ******************************************************************************************************************************/



#ifdef USE_MODULE_CORE_RULES
void RulesEvent_Set_Power();
#endif // rules

    
    void Settings_Default();  

    void BootMessage();

    void EveryLoop();

    void EverySecond_AutoOff();
    struct AUTO_OFF_SETTINGS{
      uint16_t time_decounter_secs = 0;
      // uint32_t tSaved = 0;
    }auto_off_settings;
    
    uint32_t RgbColorto32bit(RgbColor rgb);
    
    RgbcctColor GetActiveFirstColourFromCurrentPalette();
        
    #ifdef ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED
        void StartFadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0) );
    #endif // ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED

    void setChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t wc = 0, uint8_t ww = 0);
    void setChannelsRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t wc, uint8_t ww);

//     // need multiple controllers per segment, but only when needed, so it should be a segment buffer device
//     RgbcctColor_Controller rgbcct_controller = RgbcctColor_Controller();

//     RgbcctColor* active_rgbcct_colour_p = nullptr; //what is this then? internal conversions to output? (ie can I leave this as private)
    

    void Template_Load();

    void parse_JSONCommand(JsonParserObject obj);
  
    int8_t Tasker_Web(uint8_t function);
    #include "6_Lights/00_Interface/mInterfaceLight_Web.h"

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);
    

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebAppend_Root_Buttons();
    void WebAppend_Root_Draw_Table();
    void WebCommand_Parse();
    void WebAppend_Root_Draw_RGBLive();
    void WebAppend_Root_Draw_RGBTable(uint8_t rows = 8);
    
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
    uint32_t WebColorFromColourMap(uint8_t i);
    uint32_t WebColorFromColourType(RgbColor rgb);
    #endif // USE_MODULE_NETWORK_WEBSERVER

 
    uint8_t light_power_state = 0;
    uint8_t light_power_Saved = 0;
    uint8_t subtype = 0;                    // LST_ subtype
    uint8_t device = 0;
    bool pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer
    bool     fade_initialized = false;      // dont't fade at startup
    uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
    uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023


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

    void InternalSet_ActiveSolidPalette_ColourTemp(uint16_t ct) ;

    float    HueN2F(uint16_t hue);
    float    SatN2F(uint8_t sat);
    float    BrtN2F(uint8_t brt);
    uint16_t HueF2N(float hue);
    uint8_t  SatF2N(float sat);
    uint8_t  BrtF2N(float brt);

    RgbcctColor GetRandomColour(RgbcctColor colour1, RgbcctColor colour2);
    HsbColor GetHSBColour();

    RgbcctColor Color32bit2RgbColour(uint32_t colour32bit);

    void UpdateSetOutputs();  
    void ApplyGlobalBrightnesstoColour(RgbcctColor* colour);

    
    RgbColor GetColourValueUsingMaps(float value, uint8_t map_style_id = 0, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
RgbColor GetColourValueUsingMapsMaximumBrightness(float value, uint8_t map_style_id, float value_min=0, float value_max=0,  bool map_is_palette_id = false);

    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_State(uint8_t json_level = 0);
    #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
    uint8_t ConstructJSON_Debug_Module_Config(uint8_t json_method = 0);
    #endif

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mInterfaceLight> mqtthandler__settings__teleperiod;
    struct handler<mInterfaceLight> mqtthandler__state__ifchanged;
    struct handler<mInterfaceLight> mqtthandler__debug_module_config__teleperiod;
        
    enum MQTT_HANDLER_MODULE_IDS{
      MQTT_HANDLER_MODULE__STATE__TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE__DEBUG_MODULE_CONFIG__TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID,
    };
    
    // Could this be put into a function? to allow me to set things to all in this, or by ID
    struct handler<mInterfaceLight>* mqtthandler_list[
      1
      #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
        +1
      #endif
      +1 // settings at the end
      ] = {        
        &mqtthandler__state__ifchanged,
        #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
        &mqtthandler__debug_module_config__teleperiod,
        #endif
        &mqtthandler__settings__teleperiod        
    };


    #endif

  
    uint8_t getBri_Global(void) { // return the max of _briCT and _briRGB
      return (_briRGB_Global >= _briCT_Global) ? _briRGB_Global : _briCT_Global;
    }
    uint8_t getBriRGB_Global(){
      return _briRGB_Global;
    }
    uint8_t getBriCCT_Global(){
      return _briCT_Global;
    }

    void setBriRGB_Global(uint8_t bri_rgb) {
      _briRGB_Global = bri_rgb;
    }
    void setBriCT_Global(uint8_t bri_ct) {
      _briCT_Global = bri_ct;
    }


    
    uint16_t get_CTRangeMin(void)
    {
      return _ct_min_range;
    }
    uint16_t get_CTRangeMax(void)
    {
      return _ct_max_range;
    }

    // Please note that you can still set CT to 153..500, but any value below _ct_min_range or above _ct_max_range not change the CT
    uint16_t _ct_min_range = 153;   // the minimum CT rendered range
    uint16_t _ct_max_range = 500;   // the maximum CT rendered range


  // I need to keep brightness on its own, with functions for it  
    uint8_t  _briRGB_Global = 255;  // 0..255 // Used for ws28xx
    uint8_t  _briCT_Global = 255;


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







};
#endif

#endif
