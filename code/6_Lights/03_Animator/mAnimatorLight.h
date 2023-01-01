#ifndef _AnimatorDALIGHTNEOPIXEL_H
#define _AnimatorDALIGHTNEOPIXEL_H 7.0

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID 141

#ifdef USE_MODULE_LIGHTS_ANIMATOR


  #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

  #include "bus_manager.h"
  #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

/**
 * @brief List of planned changes and the order to be done (change as needed)
 * 
 * ** x) Figure out how to merge WLED/PulSar colour palettes, so a single GetColour can be used universally
 * ** x) 
 * ** x) Rewrite Segment runtime/SEG to be together
 * ** x) Figure out how to effectively store active palettes in memory, so each segment can be unique




 */


//color mangling macros
#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
// #define R(c) (byte((c) >> 16))
// #define G(c) (byte((c) >> 8))
// #define B(c) (byte(c))
// #define W(c) (byte((c) >> 24))

#define WLED_DISABLE_2D


// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
#define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
// #define ENABLE_WLED_EFFECTS


#ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
#ifndef PIXEL_COUNTS
  #define PIXEL_COUNTS DEFAULT_LED_COUNT
#endif

#ifndef DATA_PINS
  #define DATA_PINS LEDPIN
#endif

#ifndef DEFAULT_LED_TYPE
  #define DEFAULT_LED_TYPE TYPE_WS2812_RGB
#endif

#ifndef DEFAULT_LED_COLOR_ORDER
  #define DEFAULT_LED_COLOR_ORDER COL_ORDER_GRB  //default to GRB
#endif
#endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

// #define ENABLE_MODULE_DEBUG__ANIMATOR_LIGHT_EFFECTS_HIGH_LEVEL //ie not time sensitive
// #define ENABLE_MODULE_DEBUG__ANIMATOR_LIGHT_EFFECTS_LOW_LEVEL  // will be detailed, and slow timing down



#include "6_Lights/02_Palette/mPalette_Progmem.h"
#include "6_Lights/02_Palette/mPalette.h"
#include "6_Lights/02_Palette/mPaletteContainer.h"

#include "6_Lights/00_Interface/mInterfaceLight.h"

// #define DEBUG_TRACE_ANIMATOR_SEGMENTS

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  //?#include <ESPAsyncWebServer.h>
#endif

#include "math.h"
#include "fastmath.h"


#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>



#include "6_Lights/02_Palette/mPaletteContainer.h"
class mPaletteContainer;


#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/98_FastLED_Modified/FastLED.h"

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

// When no callback is needed for animator and effect function (e.g. WLED) must be called
#ifdef ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
#define SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR()  \
                                  SEGMENT.anim_function_callback = nullptr; 
                                  // \
                                  // SEGMENT_I(strip->_segment_index_primary).transitional = true;
#else
#define SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR()  SEGMENT.anim_function_callback = nullptr 
#endif


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATION_ACTIVE_CTR)       "animation";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR)              "playlists";

#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR)        "mode_ambilight";
#endif
#ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR)   "mode_setpixel";
#endif
#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR)   "presets";
#endif
#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PLAYLISTS_CTR)   "playlists";
#endif


#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR)         "debug/palette";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_HARDWARE__CTR)         "debug/hardware";
#endif
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_SEGMENTS__CTR)        "debug/segments";
#endif 
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_SEGMENTS_NEW__CTR)        "debug/segments_new";
#endif 
#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATIONS_PROGRESS_CTR)    "debug/animation_progress";
#endif 

#define D_EFFECTS_REGION__COLOUR_SELECT__NAME_CTR "Colour Select"
#define D_EFFECTS_REGION__ANIMATE__NAME_CTR "Animate"

/**
 * Effect Names
 **/
#include "6_Lights/03_Animator/mAnimatorLight_ProgMem_Defines.h"


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
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID; }

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
      #elif defined(USE_SK6812_METHOD_DEFAULT)

        // #ifdef USE_DEVFEATURE_SK6812_METHOD_DEFAULT_ALTERNATE
        // typedef NeoSk6812Method selectedNeoSpeedType;  // freezing
        // #else
        // typedef NeoEsp32Rmt0Sk6812Method selectedNeoSpeedType; // flickering on the off pixels
        //   #endif
        // typedef NeoEsp32Rmt1Sk6812Method selectedNeoSpeedType;
        // typedef NeoEsp32I2s0Sk6812Method selectedNeoSpeedType;
        // typedef Neo800KbpsMethod selectedNeoSpeedType;

        #ifdef ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
        typedef NeoEsp32I2s0Sk6812Method selectedNeoSpeedType;  // hangs
        #else      
        // typedef NeoEsp32Rmt0800KbpsMethod selectedNeoSpeedType; // flickering on off pixels
        typedef NeoEsp32Rmt0Sk6812Method selectedNeoSpeedType; // flickering
        // typedef NeoEsp32I2s0Sk6812Method selectedNeoSpeedType;  // hangs
        // typedef NeoEsp32I2s0800KbpsMethod selectedNeoSpeedType;
        // typedef NeoSk6812Method selectedNeoSpeedType;
        // typedef NeoEsp32I2s1Sk6812Method  selectedNeoSpeedType; //recommended by Makuna
        #endif // ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
      
      #else          
        // typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
        // typedef Neo800KbpsMethod selectedNeoSpeedType;

        #ifdef ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
        typedef NeoEsp32I2s1Ws2812xMethod selectedNeoSpeedType;
        #else
        typedef Neo800KbpsMethod selectedNeoSpeedType;
        #endif

      #endif
    #endif   

    #ifndef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    void Init_NeoPixelBus(int8_t pin);
    NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;  // needs moved to segments array so a pin can also be in there
    #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    #ifdef ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    // // void Init_NeoPixelBus(int8_t pin);
    NeoPixelBus<selectedNeoFeatureType, selectedNeoSpeedType> *stripbus = nullptr;  // needs moved to segments array so a pin can also be in there
    #endif // ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT


    #ifndef STRIP_OUTPUT_REPEATED_LENGTH
      #define STRIP_OUTPUT_REPEATED_LENGTH 20
    #endif
    #define ANIMATOR_SIZE_MAX 1

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    #define PRESET_COLOUR_MAP_INDEXES_MAX COLOUR_MAP_LENGTH_ID 
    uint16_t strip_size_requiring_update = STRIP_SIZE_MAX;  // This may not be the right thing I want animation.transition.pixels_to_update_as_number
    
    

    /************** DEC22 List
    EFFECTS       // PulSar and WLED
    MQTT_SETPIXEL // Using json encoded message to set the pixels directly
    REALTIME_UDP
    REALTIME_HYPERION
    REALTIME_E131
    REALTIME_ADALIGHT
    REALTIME_ARTNET
    REALTIME_TPM2NET
    REALTIME_DDP
    **************/ 
    enum ANIMATION_MODE
    {
      ANIMATION_MODE__DISABLED,      
      ANIMATION_MODE__EFFECTS,       // PulSar and WLED
      ANIMATION_MODE__MQTT_SETPIXEL, // Using json encoded message to set the pixels directly
      ANIMATION_MODE__REALTIME_UDP,
      ANIMATION_MODE__REALTIME_HYPERION,
      ANIMATION_MODE__REALTIME_E131,
      #ifdef ENABLE_FEATURE_PIXEL_MODE__REALTIME_ADALIGHT
      ANIMATION_MODE__REALTIME_ADALIGHT,
      #endif
      ANIMATION_MODE__REALTIME_ARTNET,
      ANIMATION_MODE__REALTIME_TPM2NET,
      ANIMATION_MODE__REALTIME_DDP,  
      ANIMATION_MODE__LENGTH_ID
    };             
    int8_t GetAnimationModeIDbyName(const char* c);
    const char* GetAnimationModeName(char* buffer, uint16_t buflen);
    const char* GetAnimationModeNameByID(uint8_t id, char* buffer, uint16_t buflen);
    void CommandSet_AnimationModeID(uint8_t value);

    void StartAnimation_AsAnimUpdateMemberFunction();
    
    const char* GetAnimationStatusCtr(char* buffer, uint8_t buflen);
            
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebPage_Root_AddHandlers();
    void WebAppend_JSON_RootPage_LiveviewPixels();
    void WebPage_Root_SendInformationModule();
    #endif // USE_MODULE_NETWORK_WEBSERVER

    #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

    BusManager* busses = nullptr; //BusManager();



    byte correctionRGB[4] = {0,0,0,0};
    uint16_t lastKelvin = 0;
    void colorKtoRGB(uint16_t kelvin, byte* rgb);
    uint32_t colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb);
    uint16_t approximateKelvinFromRGB(uint32_t rgb);

    #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT


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
    // void SetRefreshLEDs();    
    void StripUpdate();
                

    RgbcctColor ApplyBrightnesstoDesiredColourWithGamma(RgbcctColor full_range_colour, uint8_t brightness);
    RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightnessRGB, uint8_t brightnessCCT);
    RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor full_range_colour, uint8_t brightness);


    #ifdef USE_MODULE_NETWORK_WEBSERVER
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
    #endif //USE_MODULE_NETWORK_WEBSERVER

    #ifdef USE_DEVFEATURE_ANIMATOR_INIT_CODE__SEGMENT_MATRIX_TESTER
    void Test_Config();
    #endif

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

    #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    void SubTask_Manual_SetPixel();
    uint8_t ConstructJSON_Manual_SetPixel(uint8_t json_level = 0);
    #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    void loadPalette_Michael(uint8_t palette_id, uint8_t segment_index);




  /**
   * @brief 
   * ConstructJSON_Settings
   * ConstructJSON_AnimationActive (replacing animation/segments) = show information about what is running now, main info point
   * ConstructJSON_ModeAmbilight
   * ConstructJSON_Playlist (aka Playlist/Mixer)
   * ConstructJSON_Debug_Palette
   * ConstructJSON_Debug_Segments
   * */
  
  uint8_t ConstructJSON_Settings(uint8_t json_level = 0);
  uint8_t ConstructJSON_Animation_Active(uint8_t json_level = 0);
  uint8_t ConstructJSON_Playlist(uint8_t json_level = 0);
  /**
   * @brief Each mode
   */
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  uint8_t ConstructJSON_Mode_Ambilight(uint8_t json_level = 0);
  #endif
  #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  uint8_t ConstructJSON_Mode_SetManual(uint8_t json_level = 0); // probably falls into the E131 type, but here set my mqtt
  #endif
  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  uint8_t ConstructJSON_Auto_Presets(uint8_t json_level = 0);
  #endif

  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
  uint8_t ConstructJSON_Auto_Playlists(uint8_t json_level = 0);
  #endif

  
  /**
   * @brief Debug 
   */
  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  uint8_t ConstructJSON_Debug_Palette(uint8_t json_level = 0);
  #endif
  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
  uint8_t ConstructJSON_Debug_Hardware(uint8_t json_level = 0);
  #endif
  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
  uint8_t ConstructJSON_Debug_Segments(uint8_t json_level = 0);
  #endif
  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
  uint8_t ConstructJSON_Debug_Segments_New(uint8_t json_level = 0);
  #endif
  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
    uint8_t ConstructJSON_Debug_Animations_Progress(uint8_t json_level = 0);  
    ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
    mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);  
  #endif
  
  #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();  
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
  
    // Are these really needed?? now I am use auto for loops
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE__ANIMATION_ACTIVE_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE__PLAYLISTS_TELEPERIOD_ID,
      #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      MQTT_HANDLER_MODULE__MODE_AMBILIGHT_TELEPERIOD_ID,
      #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      MQTT_HANDLER_MODULE__MODE_MANUAL_SETPIXEL_TELEPERIOD_ID,
      #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      
      #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
      MQTT_HANDLER_MODULE__AUTOMATION_PRESETS__ID,
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
      MQTT_HANDLER_MODULE__AUTOMATION_PLAYLSITS__ID,
      #endif

      /**
       * @brief Debug
       **/
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
      MQTT_HANDLER_MODULE__DEBUG_PALETTE_TELEPERIOD_ID,
      #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
      MQTT_HANDLER_MODULE__DEBUG_SEGMENTS_TELEPERIOD_ID,
      #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
      MQTT_HANDLER_MODULE__DEBUG_SEGMENTS_NEW_TELEPERIOD_ID,
      #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      MQTT_HANDLER_MODULE__DEBUG_ANIMATOR_ANIMATION_PROGRESS_TELEPERIOD_ID,
      #endif
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    struct handler<mAnimatorLight> mqtthandler_settings_teleperiod;    
    struct handler<mAnimatorLight> mqtthandler_animation_active_teleperiod;  
    struct handler<mAnimatorLight> mqtthandler_playlists_teleperiod;
    /**
     * @brief Each mode
     **/
    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    struct handler<mAnimatorLight> mqtthandler_mode_ambilight_teleperiod;
    #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    struct handler<mAnimatorLight> mqtthandler_manual_setpixel;
    #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    struct handler<mAnimatorLight> mqtthandler_automation_presets;
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
    struct handler<mAnimatorLight> mqtthandler_automation_playlists;
    #endif
    
    /**
     * @brief Debug
     **/
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
      struct handler<mAnimatorLight> mqtthandler_debug_palette;
    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
      struct handler<mAnimatorLight> mqtthandler_debug_hardware;
    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
      struct handler<mAnimatorLight> mqtthandler_debug_segments;
    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
      struct handler<mAnimatorLight> mqtthandler_debug_segments_new;
    #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      struct handler<mAnimatorLight> mqtthandler_debug_animations_progress;
    #endif

    // Could this be put into a function? to allow me to set things to all in this, or by ID
    struct handler<mAnimatorLight>* mqtthandler_list[
      2
      /**
       * @brief Each mode
       **/
      #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      +1
      #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      +1
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
        +1
      #endif
      /**
       * @brief Debug
       **/
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
        +1
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
        +1
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
        +1
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
        +1
      #endif
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
        +1
      #endif
      +1 // settings at the end
      ] = {
        
        &mqtthandler_animation_active_teleperiod,
        &mqtthandler_playlists_teleperiod,
        /**
         * @brief Each mode
         **/
        #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
        &mqtthandler_mode_ambilight_teleperiod,
        #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
        #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
        &mqtthandler_manual_setpixel,
        #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
        #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
        &mqtthandler_automation_presets,
        #endif
        #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
        &mqtthandler_automation_playlists,
        #endif
        /**
         * @brief Debug
         **/
        #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
        &mqtthandler_debug_palette,
        #endif
        #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
        &mqtthandler_debug_hardware,
        #endif
        #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
        &mqtthandler_debug_segments,
        #endif
        #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
        &mqtthandler_debug_segments_new,
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

#ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST
#include "mAnimatorLight_Auto_Playlists.h"
#endif
#ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
#include "mAnimatorLight_Auto_Presets.h"
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
To be phased into normal segment effect
**************************************************************************************************************************************************************************
**************************************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
**********

lets work on this next

- create each notification type as its own effect
- speed will contr
  
      RgbcctColor colour;
      uint8_t brightness;
      intensity     "how often to be shown"  0-255 is slow and fast (just have few levels of blinking or fading, really speed will control duration of one cycle, 0-253 will be very to few seconds, but make 255[last_value] be like 30 seconds for very long, or 252=30secs, 253=1min)
      speed  "how visible it should be" ie pulsing fast
      aux0   how many seconds to show
      

********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************
**********************************************************************************************************************************************************************
******************************************************************************************************************************************************************************/

#ifdef ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION

// create subsub_json_parse_command that is triggered and passes to special commands with "Notifications" as json level 1 that then passes the json object level 2
   
  
  #ifndef STRIP_NOTIFICATION_SIZE
    #define STRIP_NOTIFICATION_SIZE (STRIP_SIZE_MAX>12?12:STRIP_SIZE_MAX) // Set max if not defined as 20
  #endif
  RgbcctColor ApplySimpleEffectOnColour(RgbcctColor colour_start, RgbcctColor colour_end, float progress, uint8_t effect_type);

  void init_Notifications();
  void SubTask_Notifications();
  void parsesub_Notifications(JsonParserObject obj);
  
  const char* GetNotificationModeNamebyID(uint8_t id, char* buffer);
  int8_t      GetNotificationModeIDbyName(const char* c);

  /**
   * @brief These need to be turned into a list of segment style animations
   * 
   */
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
    struct TSAVED{ // this all needs phased out and included directly into the pixel struct so it can be added as a segment animation
      uint32_t ForceUpdate = millis(); 
      uint32_t TimeoutCounter = millis();
      uint32_t Timeout = millis();
      uint32_t AutoOff = millis();
      uint32_t tSavedForceUpdate=millis();
      uint32_t tNotifPanelTimeout;
    }tSaved;
    struct PIXELN{ // changed to one struct, then each notification is a segment of length one, or else, segment==length to be all
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

#endif // ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION


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

#ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
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
#include "6_Lights/03_Animator/mAnimatorLight_Segments.h"   
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


  #ifdef ENABLE_DEVFEATURE_WS2812FX_SEGMENT_CONSTRUCTOR
  // Do I need this? Or can I remove it as an unneeded subclass level
  WS2812FX *strip = nullptr;//new WS2812FX();
  #endif // ENABLE_DEVFEATURE_WS2812FX_SEGMENT_CONSTRUCTOR




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
    EFFECTS_FUNCTION_STEPPING_PALETTE_ID,
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
