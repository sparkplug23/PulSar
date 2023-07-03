#ifndef _DRIVER_INTERFACE_LIGHTS1_H
#define _DRIVER_INTERFACE_LIGHTS1_H 0.1

#define D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID ((6*1000)+00)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE


#ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <freertos/task.h>
#include <Arduino.h>

typedef std::function<void(void)> CommitHandler;

struct CommitParams
{
  CommitHandler handler;
  xSemaphoreHandle semaphore = NULL;
};

class NeoPixelShowTask
{
private:
  CommitParams _commit_params;
  // TaskHandle_t _commit_task;

public:
  TaskHandle_t _commit_task;

  bool flag_block_show = false;

  NeoPixelShowTask() : _commit_task(NULL){}
  void begin(CommitHandler handler, uint8_t core_id);
  void execute();
  bool IsBusy();

/**
 * @brief 
 * 
 * Two issues to test
 * 
 * 1) does execute start, and then the process runs forever? if so, then it could be calling show WAY too much. Either needs a delay or to make sure execute is one and done
 * 
 * Try adding delay after "show()" inside begin function, also try make sure its once only, remove the while(true) 
 * 
 */

};

#endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32




  #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  #include "6_Lights/00_Interface/mBusManager.h"
  #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

// New sections of code that should be enabled by default
// #define ENABLE_CRGBPALETTES_IN_PROGMEM
// #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
#define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS


  #define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_HSBID__NAME_CTR   "HsbID %02d"
  // #define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR  "Solid Rgbcct %02d"
  #define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR  "Rgbcct %02d"
  #define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_GENERIC__NAME_CTR "Generic %02d"

#define ENABLE_ANIMATION_MODE__EFFECTS

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"

#include "JsonParser.h"

#ifdef ENABLE_DEVFEATURE_PALETTE_LOADED_AS_NEW_CLASS
#include "6_Lights/02_Palette/mColourUtils.h"
#endif // ENABLE_DEVFEATURE_PALETTE_LOADED_AS_NEW_CLASS

#include "6_Lights/02_Palette/mPaletteContainer.h"
#include "6_Lights/02_Palette/mPalette.h"

//Required as defualt for now
#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

#define D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR "RGBCCT_PWM"
#define D_PIXEL_HARDWARE_TYPE_WS28XX_CTR "WS28XX"
#define D_PIXEL_HARDWARE_TYPE_SK6812_CTR "SK6812"


DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR)       D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR;
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_WS28XX_CTR)       D_PIXEL_HARDWARE_TYPE_WS28XX_CTR;
DEFINE_PGM_CTR(PM_PIXEL_HARDWARE_TYPE_SK6812_CTR)       D_PIXEL_HARDWARE_TYPE_SK6812_CTR;


// #define CLEAR_PALETTELIST_WITH_PTR(x) memset(x,0,sizeof(PALETTELIST::PALETTE));

// These should be removed in favour of busses method!
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

#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/02_Time/mTime.h"
#include "6_Lights/02_Palette/mPalette_Progmem.h"

#define LST_MAX 5

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__STATE__CTR) "state";  
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_MODULE_CONFIG__CTR) "debug/module";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_BUS_CONFIG__CTR) "debug/busconfig";

enum LIHGT_POWER_STATE_IDS{
  LIGHT_POWER_STATE_OFF_ID=0,
  LIGHT_POWER_STATE_ON_ID,
  LIGHT_POWER_STATE_TOGGLE_ID,
  LIGHT_POWER_STATE_PAUSED,
  LIGHT_POWER_STATE_ANIMATING_ID,
  LIGHT_POWER_STATE_LENGTH_ID
};

#define PALETTELIST_COLOUR_HSBID_AMOUNT_MAX PALETTELIST_COLOUR_AMOUNT_MAX

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
DEFINE_PGM_CTR(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR   )      "Ambilight"  ;                
DEFINE_PGM_CTR(PM_ANIMATION_MODE_EFFECTS_NAME_CTR  )         D_JSON_EFFECTS;
#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR) "Manual SetPixel";
#endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL


#include "1_TaskerManager/mTaskerManager.h"


class mInterfaceLight :
  public mTaskerInterface
{
  public:
    mInterfaceLight(){};
    void Pre_Init(void);

    
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

    struct SETTINGS
    {
      
    }settings;

    int8_t Get_BusTypeID_FromName(const char* c);
    COLOUR_ORDER_T GetColourOrder_FromName(const char* c);
    
    void ShowInterface();

    void LightCalcPWMRange();
    void parseJSONObject__BusConfig(JsonParserObject obj); 

    void EveryLoop();
    
    uint32_t RgbColorto32bit(RgbColor rgb);
    
        
    #ifdef ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED
        void StartFadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0) );
    #endif // ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED


    void Template_Load();

    void parse_JSONCommand(JsonParserObject obj);

    // void ShowInterface();

  
  


/******************************************************************************************************************
 * 
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/

/******************************************************************************************************************
 * 
*******************************************************************************************************************/



    #ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
    NeoPixelShowTask* neopixel_runner = nullptr;
    #endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32




    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);
    
    void changeChannels(uint8_t *channels);
    uint16_t change8to10(uint8_t v);
    uint8_t change10to8(uint16_t v);

   uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
    uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023

    BusManager* bus_manager = nullptr;
    BusConfig* busConfigs[WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES] = {nullptr};
    void BusManager_Create_DefaultSingleNeoPixel();
    void BusManager_Create_DefaultSinglePWM_5CH();

    // int8_t Get



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
    

    // Please note that you can still set CT to 153..500, but any value below _ct_min_range or above _ct_max_range not change the CT
    // uint16_t _ct_min_range = 153;   // the minimum CT rendered range
    // uint16_t _ct_max_range = 500;   // the maximum CT rendered range
    uint8_t  _briRGB_Global = 255;  // 0..255 // Used for ws28xx
    uint8_t  _briCT_Global = 255;

    uint8_t getBri_Global(void) { // return the max of _briCT and _briRGB
      return (_briRGB_Global >= _briCT_Global) ? _briRGB_Global : _briCT_Global;
    }
    uint8_t getBriRGB_Global()
    {
      return _briRGB_Global;
    }
    uint8_t getBriCCT_Global()
    {
      return _briCT_Global;
    }
    void setBriRGB_Global(uint8_t bri_rgb) 
    {
      _briRGB_Global = bri_rgb;
    }
    void setBriCT_Global(uint8_t bri_ct) 
    {
      _briCT_Global = bri_ct;
    }

    void CommandSet_Brt_255(uint8_t value);
    void CommandSet_Global_BrtRGB_255(uint8_t bri, uint8_t segment_index = 0);
    void CommandSet_Global_BrtCCT_255(uint8_t bri, uint8_t segment_index = 0);


    RgbcctColor GetRandomColour(RgbcctColor colour1, RgbcctColor colour2);
    HsbColor GetHSBColour();

    RgbcctColor Color32bit2RgbColour(uint32_t colour32bit);

    void UpdateSetOutputs();  
    
    RgbColor GetColourValueUsingMaps_FullBrightness(float value, uint8_t map_style_id = 0, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    RgbColor GetColourValueUsingMaps_AdjustedBrightness(float value, uint8_t map_style_id, float value_min=0, float value_max=0,  bool map_is_palette_id = false);
    
    /******************************************************************************************************************
     * ConstructJson
    *******************************************************************************************************************/

  
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
    uint8_t ConstructJSON_Debug_Module_Config(uint8_t json_level = 0, bool json_appending = true);
    #endif

    #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG
    uint8_t ConstructJSON_Debug__BusConfig(uint8_t json_level, bool json_appending);
    #endif
  
    /******************************************************************************************************************
     * MQTT
    *******************************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mInterfaceLight> mqtthandler__settings__teleperiod;
    struct handler<mInterfaceLight> mqtthandler__state__ifchanged;
    struct handler<mInterfaceLight> mqtthandler__debug_module_config__teleperiod;
    struct handler<mInterfaceLight> mqtthandler__debug_bus_config__teleperiod;
                
    std::vector<struct handler<mInterfaceLight>*> mqtthandler_list;

    #endif
    
  #ifdef USE_MODULE_NETWORK_WEBSERVER23

/**
 * @brief MQTTHandler_AddWebURL_PayloadRequests
 * */
void MQTTHandler_AddWebURL_PayloadRequests();

  #endif // USE_MODULE_NETWORK_WEBSERVER23



};
#endif

#endif
