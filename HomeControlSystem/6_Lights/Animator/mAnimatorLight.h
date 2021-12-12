#ifndef _AnimatorDALIGHTNEOPIXEL_H
#define _AnimatorDALIGHTNEOPIXEL_H 7.0

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID 141

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#include "6_Lights/Palette/mPalette_Progmem.h"
#include "6_Lights/Palette/mPalette.h"

#include "6_Lights/_Interface/mInterfaceLight.h"

// #define DEBUG_TRACE_ANIMATOR_SEGMENTS

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
#include "6_Lights/Palette/mPalette.h"
#include "mAnimatorLight_web.h"
#include "6_Lights/Animator/EffectsWLED/Animations.h"

// #define ENABLE_PIXEL_FUNCTION_PIXELGROUPING
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
#elif defined(USE_WS28XX_FEATURE_4_PIXEL_TYPE)   
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
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_EFFECTS_CTR)               "flasher";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MIXER_CTR)                 "mixer";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SETPIXEL_MANUALLY_CTR)     "setpixel_manually";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_ANIMATIONS_PROGRESS_CTR)   "debug/animation_details";



#define D_EFFECTS_REGION_COLOUR_SELECT_NAME_CTR "Colour Select"
#define D_EFFECTS_REGION_ANIMATE_NAME_CTR "Animate"


/**
 * Effects Function names (Giving multiple names using delimeter)
 * */
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SLOW_GLOW_NAME_CTR)                        "Slow Glow|Blend";    
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_STATIC_PALETTE_NAME_CTR)                   "Static Glow|Static|InOrder";     
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SEQUENTIAL_NAME_CTR)                       "Sequential"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR)                     D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR;//"Solid RGBCCT"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_CTR)                  "Step Palette";

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




#include <functional>
// #define ANIM_CALLBACK_SIGNATURE std::function<void(char*, uint8_t*, unsigned int)> callback
// #define ANIM_CALLBACK_SIGNATURE std::function<void(const AnimationParam& param)> callback
#define ANIM_FUNCTION_SIGNATURE std::function<void(const AnimationParam& param)> anim_function_callback
// #define ANIM_FUNCTION_SIGNATURE std::function<void(const AnimationParam& param)> anim_function_callback
#define ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE std::function<void()> anim_progress_mqtt_function_callback


#define ANIM_FUNCTION_SIGNATURE_SEGMENT_INDEXED std::function<void(const uint8_t segment_index, const AnimationParam& param)> anim_function_callback_indexed


class mAnimatorLight :
  public mTaskerInterface
{
  private:
  
    static uint16_t _usedSegmentData;

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

    // Put settings at top of class from now on, use it for common settings
    struct SETTINGS{
      struct FLAGS{
          uint8_t EnableSceneModeWithSliders = true;
          uint8_t TemplateProvidedInProgMem = false;
          uint8_t EnableModule = false;
      }flags;
    }settings;

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
        typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
      #endif
    #endif   

    void Init_NeoPixelBus();
    NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;

    #ifndef STRIP_OUTPUT_REPEATED_LENGTH
      #define STRIP_OUTPUT_REPEATED_LENGTH 20
    #endif
    #define ANIMATOR_SIZE_MAX 1

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    #define PRESET_COLOUR_MAP_INDEXES_MAX COLOUR_MAP_LENGTH_ID 
    uint16_t strip_size_requiring_update = STRIP_SIZE_MAX;  // This may not be the right thing I want animation.transition.pixels_to_update_as_number
    uint16_t animator_strip_size = ANIMATOR_SIZE_MAX; //allow variable control of size

    
    void SetPixelColor(uint16_t indexPixel, RgbcctColor color);
    RgbcctColor GetPixelColor(uint16_t indexPixel = 0);
    

    // what is stored for state is specific to the need, in this case, the colors.
    // Basically what ever you need inside the animation update function
    struct AnimationColours
    {
      RgbTypeColor StartingColor; // or should this be a pointer buffer only
      RgbTypeColor DesiredColour;
    };
    AnimationColours animation_colours[STRIP_SIZE_MAX];

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

    ANIM_FUNCTION_SIGNATURE;
    mAnimatorLight& setAnimFunctionCallback(ANIM_FUNCTION_SIGNATURE);

    

    void RotateDesiredColour(uint8_t pixels, uint8_t direction = 0);
    void UpdateStartingColourWithGetPixel();
    void UpdateDesiredColourWithGetPixel();
    void Segments_UpdateStartingColourWithGetPixel();
    

    void init_flasher_settings();
    uint16_t SetLEDOutAmountByPercentage(uint8_t percentage);
    void ShowStatusViaPixels(uint8_t mode);      


    uint8_t fPixelsUpdated = false;
    uint8_t update_all = false;
    uint8_t blocking_force_animate_to_complete = true;
    uint32_t tSavedBlocking_ForceAnimateToComplete = millis();
    uint16_t setpixel_variable_index_counter = 0;
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    int8_t Tasker_Web(uint8_t function);


    void parse_JSONCommand(JsonParserObject obj);
      
    #ifdef ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT
      void CommandSet_Flasher_FunctionID(uint8_t value);
      void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value);
      void CommandSet_Flasher_Flags_Movement_Direction(uint8_t value);
      void CommandSet_Flasher_Alternate_Brightness_Min(uint8_t value);
      void CommandSet_Flasher_Alternate_Brightness_Max(uint8_t value);
      void CommandSet_Flasher_Alternate_RandomAmountPercentage(uint8_t value);
      void CommandSet_Flasher_Flags_ApplySaturationRandomnessOnPaletteColours(uint8_t value);
      void CommandSet_Brightness_Min(uint8_t value);
      void CommandSet_Brightness_Max(uint8_t value);    
    void CommandSet_Palette_Generation_Randomise_Brightness_Mode(uint8_t value);
    #endif // ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT


    #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
    void CommandSet_PixelGrouping_Flag_AgedColouring(uint8_t value);
    void CommandSet_PixelGrouping_Flag_ModeID(uint8_t value);
    void CommandSet_PixelGrouping_Flag_Multiplier(uint8_t value);
    void CommandSet_PixelGrouping_MappedMultiplierData(uint8_t* value, uint8_t length);
    bool OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
    #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING

    

    void CommandSet_BrightnessRGB_Limits_Lower(uint8_t value);
    void CommandSet_BrightnessRGB_Limits_Upper(uint8_t value);
      
    void FadeToNewColour(RgbcctColor newcolor, uint16_t _time_to_newcolour = 1000, RgbcctColor fromcolor = RgbcctColor(0));

    const char* GetAnimationStatusCtr(char* buffer, uint8_t buflen);

    uint32_t tSavedCheckAnyLEDsOnOff = 0;
    uint16_t fAnyLEDsOnOffCount = 0;

    /**
     * Make this definable, and or remove it, because it grows with pixel size and will waste memory
     * If its placed into the segment/runtime animation, then it can be held in the shared struct memory for that exact animation
     * */
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
    uint16_t rotate_start_index = 0;
    uint16_t LEDp, LEDe,             desired_pixel;  // PHASE OUT!
    

    HsbColor GetColourFromMapUsingType(
      uint16_t pixel_I_want,
      uint8_t *colour_index, uint16_t colour_array_length, uint16_t fMapIDs_Type, //inputs
      uint16_t* total_hsbcolours_in_map, uint16_t* pixel_index_out, uint16_t* pixel_width_including_indexing //returns
    );
        
    void ApplyBrightnesstoDesiredColour(uint8_t brightness);

    RgbcctColor ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness);
    RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT);
    RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness);
    RgbcctColor ApplySimpleEffectOnColour(RgbcctColor colour_start, RgbcctColor colour_end, float progress, uint8_t effect_type);

    void CommandSet_EffectsModeID(uint8_t mode, uint8_t segment = 0);
    const char* GetEffectsModeNamebyID(uint8_t id, char* buffer, uint8_t buflen);

    void Settings_Load();
    void Settings_Save();
    void Settings_Default();
    void Settings_Reset();

    void RefreshLEDIndexPattern();
    void StripUpdate();

    void StartAnimationAsBlendFromStartingColorToDesiredColor(void);
    void StartAnimationAsSwitchingFromStartingColorToDesiredColor();

    void UpdateDesiredColourFromPaletteSelected(void);
    void TurnLEDsOff();
    void EveryLoop();
    void WebPage_Root_AddHandlers();
    void WebPage_Root_SendInformationModule();
    void SetRefreshLEDs();    
    void GenerateAnimationPixelAmountMaps();
    void WebAppend_JSON_RootPage_LiveviewPixels();


    #ifndef DISABLE_WEBSERVER
    #ifdef USE_WEBSERVER_ADVANCED_MULTIPAGES
    void Web_RGBLightSettings_Draw(AsyncWebServerRequest *request);
    void Web_RGBLightSettings_RunTimeScript(AsyncWebServerRequest *request);
    void WebAppend_RGBLightSettings_FillOptions_Controls();
    void WebAppend_RGBLightSettings_FillOptions_Controls_Selected();
    void WebAppend_RGBLightSettings_Draw_Controls();
    #endif // USE_WEBSERVER_ADVANCED_MULTIPAGES
    void Web_RGBLightSettings_UpdateURLs(AsyncWebServerRequest *request);
    void Web_PaletteEditor_UpdateURLs(AsyncWebServerRequest *request);
    void WebAppend_RGBLightSettings_Draw_Animation_Options();
    void WebAppend_RGBLightSettings_Draw_Flasher_Options();
    void Web_RGBLightSettings_ListOptions(AsyncWebServerRequest *request);
    void Web_PaletteEditor_ListOptions(AsyncWebServerRequest *request);
    void WebAppend_PaletteEditor_FillOptions_Controls();
    void WebAppend_RGBLightSettings_Draw_PageButtons();
    void WebAppend_Root_Draw_Table();
    void Web_PaletteEditor_Draw(AsyncWebServerRequest *request);
    void WebAppend_PaletteEditor_Draw_Editor_Form();
    void WebAppend_Root_ControlUI();
    void HandlePage_PaletteEditor(AsyncWebServerRequest *request);
    void HandleParameters_RGBLightSettings(AsyncWebServerRequest *request);
    void HandlePage_RGBLightSettings(AsyncWebServerRequest *request);
    void WebAppend_Root_Status_Table();
    void WebSave_RGBColourSelector(void);
    #endif //DISABLE_WEBSERVER



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
    uint8_t ConstructJSON_Manual_SetPixel(uint8_t json_level = 0);
  #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  void SetPixelColor_All(RgbcctColor colour);


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
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
  
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
      4
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
        +1
      #endif
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
        +1
      #endif
      #ifdef ENABLE_PIXEL_FUNCTION_MIXER
        +1
      #endif
      #if defined(ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT) || defined(ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS)
        +1
      #endif
      ] = {
      &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
      &mqtthandler_state_teleperiod,
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
        &mqtthandler_notifications_teleperiod,
      #endif
      #if defined(ENABLE_PIXEL_FUNCTION_HACS_EFFECTS_PHASEOUT) || defined(ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS)
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

  #endif // USE_MODULE_NETWORK_MQTT
 

/*************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Subtask:   Mixer (for changing animations)   ***************************************************************************************************************************************************************************
**  @note:     to be renamed, "profiles?"  **************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*****************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


  #ifdef ENABLE_PIXEL_FUNCTION_MIXER

  enum EFFECTS_FUNCTION_MIXER{
    EFFECTS_FUNCTION_MIXER_01_ID=0,
    // EFFECTS_FUNCTION_MIXER_02_ID,
    // EFFECTS_FUNCTION_MIXER_03_ID,
    // EFFECTS_FUNCTION_MIXER_04_ID,
    // EFFECTS_FUNCTION_MIXER_05_ID,
    // EFFECTS_FUNCTION_MIXER_06_ID,
    // EFFECTS_FUNCTION_MIXER_07_ID,
    // EFFECTS_FUNCTION_MIXER_08_ID,
    // EFFECTS_FUNCTION_MIXER_09_ID,
    // EFFECTS_FUNCTION_MIXER_10_ID,
    // EFFECTS_FUNCTION_MIXER_11_ID,
    // EFFECTS_FUNCTION_MIXER_12_ID,
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


/**
 * Any mixer setting that needed this, should probably just becomes its own animation, and hence just a different animation mode will be chosen
 * "Mixer" should be distilled down to ONLY picking animation, setting brightness, intensities etc
 * 
 * */
      uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Exactly_To_Palette_Choice : 1;
      uint16_t Apply_Upper_And_Lower_Brightness_Randomly_Ranged_To_Palette_Choice : 1;



    };
  } MIXER_SETTINGS_GROUP_FLAGS;
  

  struct MIXER_SETTINGS{
   // #ifdef DEVICE_OUTSIDETREE
    struct MODE{
      uint8_t running_id = EFFECTS_FUNCTION_MIXER_01_ID;
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

/*********************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
***************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   Notifications   ***************************************************************************************************************************************************************************
**  @note:     **************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


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
      uint8_t fForcePanelUpdate : 1;
      uint8_t fShowPanelUpdate  : 1;
      uint8_t fEnableTimeoutAll : 1;    
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

#endif // USE_TASK_RGBLIGHTING_NOTIFICATIONS


/*****************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   Ambilight   ***************************************************************************************************************************************************************************
**  @note:     **************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/

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

// enum RgbColorTypeID{
//   RgbColorTypeID_RGB=0,
//   RgbColorTypeID_RGB_CW,
//   RgbColorTypeID_RGB_WW,
//   RgbColorTypeID_RGB_WW_CW}; 


struct AMBILIGHT_SCREEN_SETTINGS{
  uint8_t ambilight_mode=AMBILIGHT_SIDES_ID;
  uint32_t tSavedUpdate = millis();
  uint32_t ratemsSavedUpdate = 1000;
  uint8_t fUpdate = false;
  
  // mInterfaceLight::HARDWARE_ELEMENT_COLOUR_ORDER hardware_element_colour_order;
  //mInterfaceLight::LightTypes_IDS light_type = LT_ADDRESSABLE;



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
  uint8_t hacs_animation_runtime_data_buffer[100] = {0};



  




/**
 * inline animation header includes
 * */
#include "EffectsHACS/Slow_Glow/Animator.h"
#include "EffectsHACS/Static_Glow/Animator.h"
#include "EffectsHACS/Fireplace_1D_01/Animator.h"
// #include "6_Lights/Animator/EffectsHACS/Sequential/Animator.h"
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
#include "6_Lights/Animator/EffectsHACS/SunPositions_SolidColourElevation05/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Sun_Elevation_RGBCCT_Palette_Indexed_Positions_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_RgbcctPaletteElevationAugmented01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/SunPositions_Elevation_Only_Controlled_CCT_Temperature_01/Animator.h"
#include "6_Lights/Animator/EffectsHACS/Development_Tester/Animator.h"






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

  
/************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   WLED (Legacy)   ***************************************************************************************************************************************************************************
**  @note:    WLED initial port, running as its own effect. To be phased out into Segments **************************************************************************************************************************************************************************
*************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


                    
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



/******************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** Animation Effect:   Segments   ***************************************************************************************************************************************************************************
**  @note:    New effect that merges HACS and WLED with segments and dynamic animation storage **************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/

#ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
#include "6_Lights/Animator/mAnimatorLight_Segments.h"   
#endif // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

/******************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
*** USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS   ***************************************************************************************************************************************************************************
**  @note:     **************************************************************************************************************************************************************************
********************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
****************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


#ifdef USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS


  /**
   * Palettes 
   * */
  // uint8_t palette_options_ids[30] = 
  // {
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_01_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_02_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_03_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_04_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_05_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_06_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_07_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_08_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_09_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_10_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_11_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_12_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_13_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_14_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_15_ID,
  //   mPalette::PALETTELIST_STATIC_CHRISTMAS_16_ID,
  //   mPalette::PALETTELIST_STATIC_WINTER_ID,
  //   mPalette::PALETTELIST_STATIC_AUTUMN_ID,
  //   mPalette::PALETTELIST_STATIC_AUTUMN_RED_ID,
  //   mPalette::PALETTELIST_STATIC_FLOWER_SWEATPEAS_01_ID,
  //   mPalette::PALETTELIST_STATIC_FLOWER_SWEATPEAS_02_ID,
  //   mPalette::PALETTELIST_STATIC_HOLLOWEEN_OGP_ID,
  //   mPalette::PALETTELIST_STATIC_HOLLOWEEN_OP_ID,
  //   mPalette::PALETTELIST_STATIC_MICROLEDS_PASTEL_01_ID,
  //   mPalette::PALETTELIST_STATIC_PINK_PURPLE_01_ID,
  //   mPalette::PALETTELIST_STATIC_HOT_PINK_NEON_WITH_NAVY_ID,
  //   /**
  //    * Gradients applied across the output
  //    * */
  //   mPalette::PALETTELIST_STATIC_RAINBOW_ID,
  //   mPalette::PALETTELIST_STATIC_RAINBOW_INVERTED_ID,
  //   mPalette::PALETTELIST_STATIC_GRADIENT_PASTEL_TONES_PURPLE_ID,
  //   mPalette::PALETTELIST_STATIC_GRADIENT_FIRE_01_ID,
  // };

  /**
   * skewed brightness options
   * */
  uint8_t brightness_options_ids[10] = //as percentage
  {
    0,
    5,
    10,
    25,
    50,
    60,
    70,
    80,
    90,
    100
  };

  /**
   * Controller effects, limit to the ones most likely to be used
   * */
  uint8_t effects_options_ids[4] =
  {
    EFFECTS_FUNCTION_STATIC_PALETTE_ID,
    EFFECTS_FUNCTION_SEQUENTIAL_ID, //sequence after, so it takes the static pallette and moves it
    EFFECTS_FUNCTION_SLOW_GLOW_ID,
    EFFECTS_FUNCTION_STEP_THROUGH_PALETTE_ID,
  };

  /**
   * This will translate into rate/time_ms depending on select effects above, probably just mapping them as percentage scales
   * */
  uint8_t intensity_options_ids[7] = //as percentage
  {
    0,
    5,
    10,
    25,
    50,
    75,
    100
  };

  
void CommandSet_Physical_UserInput_Buttons();
void PhysicalController_Convert_IDs_to_Internal_Parameters();

#ifdef ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM
void Physical_UserInput_Init();
void Physical_UserInput_Load();
void Physical_UserInput_Save();
#endif // ENABLE_XMAS_CONTROLLER_SAVING_IN_EEPROM


#define EEPROM_SIZE 12
#define USER_INPUT_BUTTON_1_LEFT 1
#define USER_INPUT_BUTTON_1_RIGHT 0
#define USER_INPUT_BUTTON_2_LEFT 3
#define USER_INPUT_BUTTON_2_RIGHT 2
#define USER_INPUT_BUTTON_3_LEFT 5
#define USER_INPUT_BUTTON_3_RIGHT 4
#define USER_INPUT_BUTTON_4_LEFT 7
#define USER_INPUT_BUTTON_4_RIGHT 6

/**
 * This will be saved and loaded from eeprom
 * */
struct USER_INPUT_SAVED_IN_EEPROM
{ 
  struct ACTIVE_OPTION{
    uint8_t palette_id = 0;
    uint8_t brightness_id = 0;
    uint8_t effects_id = 0;
    uint8_t intensity_id = 0;
    uint8_t loaded_from_eeprom = 0; // only set when loaded succesfully
  }selected;

}user_input;


#endif // USE_MODULE_LIGHTS_USER_INPUT_BASIC_BUTTONS
    




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


    // uint32_t tSavedSendData, tSavedSendData2, fSendRGBDellStatus;
    // uint8_t testvariable=0;
    // uint8_t testvariable2=0,testvariable3=0;
    // uint8_t timerenabled = false;

    // int blend_count=0;
    // enum BLEND_STEPS{BLEND_START=1,
    //                  BLEND_DELAYS,
    //                  BLEND_DONE
    // };
    // uint8_t blend_state;
    // uint32_t tTimeout;



        // make number generator, random, with skewness
        // int Range0to100[20] = {0,10,20,30,40, // random(12,24)*10;
        //                 50,60,70,80,90,
        //                 91,92,93,94,95,
        //                 96,97,98,99,100};