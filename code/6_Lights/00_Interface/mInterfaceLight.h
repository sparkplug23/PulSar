#ifndef _DRIVER_INTERFACE_LIGHTS1_H
#define _DRIVER_INTERFACE_LIGHTS1_H 0.1

#define D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID 6000 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#define HUE_N2F(h) h/360.0f
#define SAT_N2F(s) s/100.0f
#define BRT_N2F(v) v/100.0f

#define HUE_F2N(h) round(h*360.0f)
#define SAT_F2N(s) round(s*100.0f)
#define BRT_F2N(v) round(v*100.0f)

#include "6_Lights/00_Interface/PowerProfiles.h"

#include "6_Lights/00_Interface/mBusManager.h"

#define D_DEFAULT_DYNAMIC_PALETTE_NAMES__VARIABLE_RGBCCT__NAME_CTR  "Colour %02d"
#define D_DEFAULT_MODIFIABLE_PALETTE_NAMES__USER_CREATED__NAME_CTR  "Palette %02d"

#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "JsonParser.h"
#include "6_Lights/02_Palette/mPaletteLoaded.h"

#include "6_Lights/02_Palette/mPalette.h"

#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/07_Time/mTime.h"
#include "6_Lights/02_Palette/mPalette_Progmem.h"

enum LIGHT_POWER_STATE_IDS{
  LIGHT_POWER_STATE_OFF_ID=0,
  LIGHT_POWER_STATE_ON_ID,
  LIGHT_POWER_STATE_TOGGLE_ID,
  LIGHT_POWER_STATE_PAUSED,
  LIGHT_POWER_STATE_ANIMATING_ID,
  LIGHT_POWER_STATE_LENGTH_ID
};

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


DEFINE_PGM_CTR(PM_ANIMATION_MODE_NONE_NAME_CTR )   "None"     ;    
DEFINE_PGM_CTR(PM_ANIMATION_MODE_CHANGE_POWER_NAME_CTR )   "Change Power"     ;   
DEFINE_PGM_CTR(PM_ANIMATION_MODE_AMBILIGHT_NAME_CTR   )      "Ambilight"  ;                
DEFINE_PGM_CTR(PM_ANIMATION_MODE_EFFECTS_NAME_CTR  )         D_EFFECTS;
#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_ANIMATION_MODE_MANUAL_SETPIXEL_NAME_CTR) "Manual SetPixel";
#endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__STATE__CTR) "state";  
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_MODULE_CONFIG__CTR) "debug/module";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_BUS_CONFIG__CTR) "debug/busconfig";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_POWER_PROFILES__CTR) "debug/powerprofiles";


#include "1_TaskerManager/mTaskerManager.h"


class mInterfaceLight :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mInterfaceLight(){};
    void   Init(void);
    void   Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);
    
    static constexpr const char* PM_MODULE_LIGHTS_INTERFACE_CTR = D_MODULE_LIGHTS_INTERFACE_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_INTERFACE_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID; }
    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);

    // Add all data here that needs to be saved to filesystem
    struct MODULE_RUNTIME{ 


    }rt;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    uint8_t GetColourOrder_FromName(const char* c);
    
    void EveryLoop();
    
    void LightCalcPWMRange();
    void parseJSONObject__BusConfig(JsonParserObject obj); 
        
    void Template_Load();
    void Template_Load_DefaultConfig();
        
    uint16_t pwm_min = 0;                  // minimum value for PWM, from DimmerRange, 0..1023
    uint16_t pwm_max = 1023;               // maxumum value for PWM, from DimmerRange, 0..1023

    BusManager* bus_manager = nullptr;
    BusConfig* busConfigs[WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES] = {nullptr};
    

    #ifdef USE_MODULE_CORE_RULES
    void RulesEvent_Set_Power();
    #endif // rules
    
    void EverySecond_AutoOff();

    void BusManager_Create_DefaultSingleNeoPixel();
    void BusManager_Create_DefaultSinglePWM_5CH();

    uint8_t light_power_state = 0;
    uint8_t light_power_Saved = 0;
    void CommandSet_LightPowerState(uint8_t value);
    bool CommandGet_LightPowerState();
    void CommandSet_Auto_Time_Off_Secs(uint16_t value);
    struct AUTO_OFF_SETTINGS{
      uint16_t time_decounter_secs = 0;
    }auto_off_settings;

    uint8_t g_led_profile_active_index = 0; // which profile is currently active

    String GetColourOrderString(uint8_t colour_order);

    #ifdef ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
    uint16_t change8to10(uint8_t v);
    uint8_t change10to8(uint16_t v);
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

    uint8_t  _briRGB_Global = 255;  // in place of WLED "bri" 0..255 // Used for ws28xx
    uint8_t  _briCT_Global = 255;

    inline uint8_t getBri_Global(void) { // return the max of _briCT and _briRGB
      return (_briRGB_Global >= _briCT_Global) ? _briRGB_Global : _briCT_Global;
    }
    inline uint8_t getBriRGB_Global()
    {
      return _briRGB_Global;
    }
    inline uint8_t getBriCCT_Global()
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
    #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__POWER_PROFILES
    uint8_t ConstructJSON_Debug__PowerProfiles(uint8_t json_level, bool json_appending);
    #endif
  
    /******************************************************************************************************************
     * MQTT
    *******************************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();
    struct handler<mInterfaceLight> mqtthandler__settings__teleperiod;
    struct handler<mInterfaceLight> mqtthandler__state__ifchanged;
    struct handler<mInterfaceLight> mqtthandler__debug_module_config__teleperiod;
    struct handler<mInterfaceLight> mqtthandler__debug_bus_config__teleperiod;
    #ifdef ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__POWER_PROFILES
    struct handler<mInterfaceLight> mqtthandler__debug_power_profiles__teleperiod;
    #endif
                
    std::vector<struct handler<mInterfaceLight>*> mqtthandler_list;

    #endif

    /******************************************************************************************************************
     * WEBSERVER
    *******************************************************************************************************************/


};
#endif

#endif
