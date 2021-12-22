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

// #define DEBUG_ANIMATIONS_SEGMENT_EFFECTS

#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION 
#include "6_Lights/Animator/EffectsWLEDConversion/Animations.h"
#endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION 


#ifdef ENABLE_DEVFEATURE_WLED_CONVERTED_TO_SEGMENTS
#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/FastLED_Modified/FastLED.h"
#endif // ENABLE_DEVFEATURE_WLED_CONVERTED_TO_SEGMENTS

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


// enum TIME_MAP_SECS_IDS{
//   TIME_MAP_SECS_0_ID = 0,
//   TIME_MAP_SECS_1_ID,
//   TIME_MAP_SECS_2_ID,
//   TIME_MAP_SECS_4_ID,
//   TIME_MAP_SECS_6_ID,
//   TIME_MAP_SECS_10_ID,
//   TIME_MAP_SECS_15_ID,
//   TIME_MAP_SECS_20_ID,
//   TIME_MAP_SECS_30_ID,
//   TIME_MAP_SECS_60_ID,
//   TIME_MAP_SECS_LENGTH_ID
// };
// const uint8_t time_map_secs[] PROGMEM = {0,1,2,4,6,10,15,20,30,60};

// enum RATE_MAP_SECS_IDS{
//   RATE_MAP_SECS_0_ID = 0,
//   RATE_MAP_SECS_1_ID,
//   RATE_MAP_SECS_2_ID,
//   RATE_MAP_SECS_4_ID,
//   RATE_MAP_SECS_6_ID,
//   RATE_MAP_SECS_10_ID,
//   RATE_MAP_SECS_15_ID,
//   RATE_MAP_SECS_20_ID,
//   RATE_MAP_SECS_30_ID,
//   RATE_MAP_SECS_60_ID,
//   RATE_MAP_SECS_LENGTH_ID
// };
// const uint8_t rate_map_secs[] PROGMEM = {0,1,2,4,6,10,15,20,30,60};

// enum PIXELS_UPDATE_PERCENTAGE_IDS{
//   PIXELS_UPDATE_PERCENTAGE_0_ID = 0,
//   PIXELS_UPDATE_PERCENTAGE_5_ID,
//   PIXELS_UPDATE_PERCENTAGE_10_ID,
//   PIXELS_UPDATE_PERCENTAGE_15_ID,
//   PIXELS_UPDATE_PERCENTAGE_20_ID,
//   PIXELS_UPDATE_PERCENTAGE_30_ID,
//   PIXELS_UPDATE_PERCENTAGE_40_ID,
//   PIXELS_UPDATE_PERCENTAGE_50_ID,
//   PIXELS_UPDATE_PERCENTAGE_60_ID,
//   PIXELS_UPDATE_PERCENTAGE_70_ID,
//   PIXELS_UPDATE_PERCENTAGE_80_ID,
//   PIXELS_UPDATE_PERCENTAGE_90_ID,
//   PIXELS_UPDATE_PERCENTAGE_100_ID,
//   PIXELS_UPDATE_PERCENTAGE_LENGTH_ID,
// };
// const uint8_t pixels_to_update_as_percentage_map[] PROGMEM =
//   {0,5,10,15,20,30,40,50,60,70,80,90,100};


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
 * Effect Names
 **/
#include "6_Lights/Animator/mAnimatorLight_ProgMem_Defines.h"


#include <functional>
#define ANIM_FUNCTION_SIGNATURE                     std::function<void(const AnimationParam& param)>                              anim_function_callback
#define ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE   std::function<void()>                                                         anim_progress_mqtt_function_callback
#define ANIM_FUNCTION_SIGNATURE_SEGMENT_INDEXED     std::function<void(const uint8_t segment_index, const AnimationParam& param)> anim_function_callback_indexed


class mAnimatorLight :
  public mTaskerInterface
{
  private:
  
    static uint16_t _usedSegmentData;

  public:
    mAnimatorLight(){};
    void Pre_Init(void);
    void Init(void);
        
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

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    int8_t Tasker_Web(uint8_t function);

    void Settings_Load();
    void Settings_Save();
    void Settings_Default();
    void Settings_Reset();

    void parse_JSONCommand(JsonParserObject obj);
    uint8_t subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index = 255);
      
    // Put settings at top of class from now on, use it for common settings
    struct SETTINGS{
      struct FLAGS{
          uint8_t EnableSceneModeWithSliders = true;
          uint8_t TemplateProvidedInProgMem = false;
          uint8_t EnableModule = false;
      }flags;
    }settings;

    /**
     * @brief To be changed into runtime methods
     * 
     */
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
    
    void SetPixelColor(uint16_t indexPixel, RgbcctColor color, uint16_t segment_length = 0);
    RgbcctColor GetPixelColor(uint16_t indexPixel = 0);

    /**
     * @brief 
     * Tmp wled conversions
     * 
     */    
    void SetPixelColor(uint16_t indexPixel, uint8_t red, uint8_t green, uint8_t blue, uint16_t segment_length = 0);  
    void SetPixelColor(uint16_t indexPixel, uint32_t color, uint16_t segment_length = 0);

    #ifndef ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    // what is stored for state is specific to the need, in this case, the colors.
    // Basically what ever you need inside the animation update function
    // Can this be moved into the segment_runtime data?
    struct AnimationColours
    {
      RgbTypeColor StartingColor; // or should this be a pointer buffer only
      RgbTypeColor DesiredColour;
    };
    AnimationColours animation_colours[STRIP_SIZE_MAX];
    #endif // ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #ifndef ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    /**
     * Make this definable, and or remove it, because it grows with pixel size and will waste memory
     * If its placed into the segment/runtime animation, then it can be held in the shared struct memory for that exact animation
     * WARNING! : Significant memory usage, needs changed to dynamic runtime
     * */
    struct LEDOUTPUTSETTINGS{
      uint16_t length = 0;
      uint16_t index = 0;
      uint16_t pattern[STRIP_SIZE_MAX];
    }ledout;    
    #endif


    void StartAnimation_AsAnimUpdateMemberFunction();
    
    
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
    
    const char* GetAnimationStatusCtr(char* buffer, uint8_t buflen);

    #ifdef ENABLE_PIXEL_FUNCTION_PIXELGROUPING
    void CommandSet_PixelGrouping_Flag_AgedColouring(uint8_t value);
    void CommandSet_PixelGrouping_Flag_ModeID(uint8_t value);
    void CommandSet_PixelGrouping_Flag_Multiplier(uint8_t value);
    void CommandSet_PixelGrouping_MappedMultiplierData(uint8_t* value, uint8_t length);
    bool OverwriteUpdateDesiredColourIfMultiplierIsEnabled();
    #endif // ENABLE_PIXEL_FUNCTION_PIXELGROUPING

    
    #ifdef ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED
        void FadeToNewColour(RgbcctColor newcolor, uint16_t _time_to_newcolour = 1000, RgbcctColor fromcolor = RgbcctColor(0));
        
      // totally random, or random but not repeating
      enum TRANSITION_ORDER_RANDOM_METHOD_IDS{
        TRANSITION_ORDER_RANDOM_METHOD_REPEATING = 0,
        TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE,
        TRANSITION_ORDER_RANDOM_METHOD_NONREPEATING
      };
      uint8_t transition_order_random_type = TRANSITION_ORDER_RANDOM_METHOD_REPEATING_WITH_PERCENTAGE;
   
      uint8_t update_all = false;
      uint32_t tSavedBlocking_ForceAnimateToComplete = millis();
      uint16_t setpixel_variable_index_counter = 0;  
      uint32_t tSavedUpdateRGBString;
      uint8_t first_set;
      uint16_t rotate_start_index = 0;
      uint16_t LEDp, LEDe,             desired_pixel;  // PHASE OUT!

      void AnimUpdate_ShowStartingThenDesiredColors(const AnimationParam& param);
      void AnimUpdateMemberFunction(const AnimationParam& param);
      void AnimationProcess_TraditionalChasing(const AnimationParam& param);
      void AnimationProcess_TraditionalChasing2(const AnimationParam& param);
      void DesiredColourWrite_Safe(RgbcctColor colour, uint16_t index);
      
      void StartAnimationAsBlendFromStartingColorToDesiredColor(void);
      void StartAnimationAsSwitchingFromStartingColorToDesiredColor();

      void WebPage_Root_AddHandlers();
      void WebPage_Root_SendInformationModule();
      void WebAppend_JSON_RootPage_LiveviewPixels();
    
     #endif // ENABLE_PIXEL_GENERAL_PHASEDOUT_CODE_TO_BE_REMOVED_IF_NOT_NEEDED

    /**
     * @brief 
     Confirmed as needed variables
     * 
     */
    uint16_t fAnyLEDsOnOffCount = 0;
    uint8_t blocking_force_animate_to_complete = true;
    uint8_t fPixelsUpdated = false;
    uint16_t desired_pixel;
    void EveryLoop();
    void SetRefreshLEDs();    
    void StripUpdate();
    void SetPixelColor_All(RgbcctColor colour);
        

    HsbColor GetColourFromMapUsingType(
      uint16_t pixel_I_want,
      uint8_t *colour_index, uint16_t colour_array_length, uint16_t fMapIDs_Type, //inputs
      uint16_t* total_hsbcolours_in_map, uint16_t* pixel_index_out, uint16_t* pixel_width_including_indexing //returns
    );
        

    RgbcctColor ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness);
    RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT);
    RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness);

    void DynamicBuffer_Segments_UpdateDesiredColourFromPaletteSelected(uint16_t palette_id, uint8_t runtime_segment_index);

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



  #ifdef ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION
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
  #endif // ENABLE_PIXEL_OUTPUT_POWER_ESTIMATION

  #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  void SubTask_Manual_SetPixel();
  uint8_t ConstructJSON_Manual_SetPixel(uint8_t json_level = 0);
  #endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

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

    struct handler<mAnimatorLight> mqtthandler_settings_teleperiod;
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
      #ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
        +1
      #endif
      #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
        +1
      #endif
      ] = {
      &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
      &mqtthandler_state_teleperiod,
      #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
        &mqtthandler_notifications_teleperiod,
      #endif
      #ifdef ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
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
#include "mAnimatorLight_Mixer.h"
#endif

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
**  @note:     This is highly specialised effect, and will unlikely ever run next to "animated segment effect"
               It makes sense to keep this as its own effect type
**************************************************************************************************************************************************************************
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
    RgbcctColor ApplySimpleEffectOnColour(RgbcctColor colour_start, RgbcctColor colour_end, float progress, uint8_t effect_type);

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
*** Animation Effect:   WLED Testing code   ***************************************************************************************************************************************************************************
**  @note:    HACS animation, to be phased out into Segments **************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/


#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION 
#include "6_Lights/Animator/EffectsWLEDConversion/mAnimations_WLED_Conversion.h"
#endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION 

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
    EFFECTS_FUNCTION_SEQUENTIAL_PALETTE_ID,
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
