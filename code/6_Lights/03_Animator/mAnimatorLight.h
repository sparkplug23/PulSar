#ifndef _AnimatorDALIGHTNEOPIXEL_H
#define _AnimatorDALIGHTNEOPIXEL_H 7.0

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID 141

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#include "6_Lights/00_Interface/mBusManager.h"


//color mangling macros
#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
// #define R(c) (byte((c) >> 16))
// #define G(c) (byte((c) >> 8))
// #define B(c) (byte(c))
// #define W(c) (byte((c) >> 24))

#define WLED_DISABLE_2D


// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE


#ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
#ifndef PIXEL_COUNTS
  #define PIXEL_COUNTS DEFAULT_LED_COUNT
#endif

#ifndef DATA_PINS
  #define DATA_PINS LEDPIN
#endif
#define DATA_PINS_BUSSES 23, 22

#ifndef DEFAULT_LED_TYPE
  #define DEFAULT_LED_TYPE BUSTYPE_WS2812_RGB
#endif

#ifndef DEFAULT_LED_COLOR_ORDER
  #define DEFAULT_LED_COLOR_ORDER COL_ORDER_GRB  //default to GRB
#endif
#endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

// #define ENABLE_MODULE_DEBUG__ANIMATOR_LIGHT_EFFECTS_HIGH_LEVEL //ie not time sensitive
// #define ENABLE_MODULE_DEBUG__ANIMATOR_LIGHT_EFFECTS_LOW_LEVEL  // will be detailed, and slow timing down

#define MAX_PIXELBUS_DIGITAL_PINS 20


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

#ifdef USE_SK6812_METHOD_DEFAULT
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
#endif // USE_SK6812_METHOD_DEFAULT


#include "6_Lights/02_Palette/mPaletteContainer.h"
class mPaletteContainer;


#define FASTLED_INTERNAL // suppress pragma warning messages
#include "6_Lights/00_Interface/FastLED/FastLED.h"


/**
 * @brief For integrated effects on each frame, disable animation callback
 */
#ifdef ENABLE_DEVFEATURE_ALWAYS_LOAD_PALETTE_WHEN_NOT_TRANSITIONING
#define SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR()  \
                                  SEGMENT.anim_function_callback = nullptr; 
#else
#define SET_ANIMATION_DOES_NOT_REQUIRE_NEOPIXEL_ANIMATOR()  SEGMENT.anim_function_callback = nullptr 
#endif


DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SEGMENTS_CTR)       "segments";
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
#ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_SEGMENTS__CTR)        "debug/segments";
#endif 
#ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATIONS_PROGRESS_CTR)    "debug/animation_progress";
#endif 

#define D_EFFECTS_REGION__COLOUR_SELECT__NAME_CTR "Colour Select"
#define D_EFFECTS_REGION__ANIMATE__NAME_CTR       "Animate"

/**
 * Effect Names
 **/
#include "6_Lights/03_Animator/mAnimatorLight_ProgMem_Defines.h"


#include <functional>
// #define ANIM_FUNCTION_SIGNATURE_TWO_COLOURS         std::function<void(const AnimationParam& param, RgbcctColor c1, RgbcctColor c2)>  anim_function_callback
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

    static const char* PM_MODULE_LIGHTS_ANIMATOR_CTR;
    static const char* PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_ANIMATOR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){ return sizeof(mAnimatorLight);};
    #endif

    void Pre_Init(void);
    void Init(void);        
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    int8_t Tasker_Web(uint8_t function);

    // Splitting into into subsections for easier reading
    void Init__Palettes();

    void Settings_Load();
    void Settings_Save();
    void Settings_Default();
    void Settings_Reset();

    void parse_JSONCommand(JsonParserObject obj);
    uint8_t subparse_JSONCommand(JsonParserObject obj, uint8_t segment_index = 255);
    
    void parsesub_json_object_notification_shortcut(JsonParserObject obj);
      
    // Put settings at top of class from now on, use it for common settings
    struct SETTINGS{
      struct FLAGS{
          uint8_t EnableSceneModeWithSliders = true;
          uint8_t TemplateProvidedInProgMem = false;
          uint8_t EnableModule = false;
      }flags;
      uint16_t light_size_count = 1;
      uint8_t pwm_offset = 0;
    }settings;

    void TestCode_AddBus1();

    bool doInitBusses = false; // debug

 
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


    /******************************************************************************************************************************
    ****************** CommandSet_x *************************************************************************************************************
    ******************************************************************************************************************************/

    void CommandSet_LightPowerState(uint8_t value);
    bool CommandGet_LightPowerState();
    void CommandSet_Brt_255(uint8_t value);
    void CommandSet_Auto_Time_Off_Secs(uint16_t value);    
    void CommandSet_LightSizeCount(uint16_t value);
    void CommandSet_EnabledAnimation_Flag(uint8_t value);
    void CommandSet_PaletteColour_RGBCCT_Raw_By_ID(uint8_t palette_id, uint8_t* buffer, uint8_t buflen);
    
    /******************************************************************************************************************************
    ****************** CommandGet_x *************************************************************************************************************
    ******************************************************************************************************************************/

    uint16_t GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage);
    void CheckHardwareElementColourOrder();
 
    uint8_t light_power_state = 0;
    uint8_t light_power_Saved = 0;
    uint8_t subtype = 0;                    // LST_ subtype
    uint8_t device = 0;
    bool    pwm_multi_channels = false;        // SetOption68, treat each PWM channel as an independant dimmer
    bool    fade_initialized = false;      // dont't fade at startup
   

    #ifdef USE_MODULE_CORE_RULES
    void RulesEvent_Set_Power();
    #endif // rules
  

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

    // Please note that you can still set CT to 153..500, but any value below _ct_min_range or above _ct_max_range not change the CT
    // uint16_t _ct_min_range = 153;   // the minimum CT rendered range
    // uint16_t _ct_max_range = 500;   // the maximum CT rendered range
    uint8_t  _briRGB_Global = 255;  // 0..255 // Used for ws28xx
    uint8_t  _briCT_Global = 255;

    // void setChannels(uint8_t r, uint8_t g, uint8_t b, uint8_t wc = 0, uint8_t ww = 0);
    // void setChannelsRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t wc, uint8_t ww);
   
    void EverySecond_AutoOff();
    void BootMessage();

    struct AUTO_OFF_SETTINGS{
      uint16_t time_decounter_secs = 0;
    }auto_off_settings;

    void StartAnimation_AsAnimUpdateMemberFunction();
    
    const char* GetAnimationStatusCtr(char* buffer, uint8_t buflen);
            
    #ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    byte correctionRGB[4] = {0,0,0,0};
    uint16_t lastKelvin = 0;
    void colorKtoRGB(uint16_t kelvin, byte* rgb);
    uint32_t colorBalanceFromKelvin(uint16_t kelvin, uint32_t rgb);
    uint16_t approximateKelvinFromRGB(uint32_t rgb);
    #endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT


    uint16_t fAnyLEDsOnOffCount = 0;
    uint8_t blocking_force_animate_to_complete = true;
    uint8_t fPixelsUpdated = false;
    uint16_t desired_pixel;
    void EveryLoop();    
                
    void Init_SegmentWS2812FxStrip();

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
    void WebPage_Root_AddHandlers();
    void WebAppend_JSON_RootPage_LiveviewPixels();
    void WebPage_Root_SendInformationModule();
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
    uint8_t ConstructJSON_Manual_SetPixel(uint8_t json_level = 0, bool json_appending = true);
    #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    void LoadPalette(uint8_t palette_id, uint8_t segment_index, uint8_t* palette_buffer = nullptr, uint16_t palette_buflen = 0);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Segments(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Playlist(uint8_t json_level = 0, bool json_appending = true);
    /**
     * @brief Each mode
     */
    #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
    uint8_t ConstructJSON_Mode_Ambilight(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    uint8_t ConstructJSON_Mode_SetManual(uint8_t json_level = 0, bool json_appending = true); // probably falls into the E131 type, but here set my mqtt
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    uint8_t ConstructJSON_Auto_Presets(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
    uint8_t ConstructJSON_Auto_Playlists(uint8_t json_level = 0, bool json_appending = true);
    #endif  
    /**
     * @brief Debug 
     */
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    uint8_t ConstructJSON_Debug_Palette(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    uint8_t ConstructJSON_Debug_Segments(uint8_t json_level = 0, bool json_appending = true);
    #endif
    #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      uint8_t ConstructJSON_Debug_Animations_Progress(uint8_t json_level = 0, bool json_appending = true);  
      ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE;
      mAnimatorLight& setCallback_ConstructJSONBody_Debug_Animations_Progress(ANIMIMATION_DEBUG_MQTT_FUNCTION_SIGNATURE);  
    #endif
    
    #ifdef USE_MODULE_NETWORK_MQTT
      void MQTTHandler_Init();
      void MQTTHandler_Set_RefreshAll();
      void MQTTHandler_Set_DefaultPeriodRate();  
      void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
      
      std::vector<struct handler<mAnimatorLight>*> mqtthandler_list;
    
      struct handler<mAnimatorLight> mqtthandler_settings_teleperiod;    
      struct handler<mAnimatorLight> mqtthandler_segments_teleperiod;  
      struct handler<mAnimatorLight> mqtthandler_playlists_teleperiod;
      /**
       * @brief Each mode
       **/
      #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
      struct handler<mAnimatorLight> mqtthandler_mode_ambilight_teleperiod;
      #endif
      #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
      struct handler<mAnimatorLight> mqtthandler_manual_setpixel;
      #endif
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
      #endif
      #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
      struct handler<mAnimatorLight> mqtthandler_debug_segments;
      #endif
      #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
      struct handler<mAnimatorLight> mqtthandler_debug_animations_progress;
      #endif
    #endif // USE_MODULE_NETWORK_MQTT
      

    /*****************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Subtask:   Mixer (for changing animations)   *****************************************************************************************************************************
    **  @note:     to be renamed, "profiles?"  ***********************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #ifdef ENABLE_PIXEL_AUTOMATION_PLAYLIST
    #include "mAnimatorLight_Auto_Playlists.h"
    #endif
    #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    #include "mAnimatorLight_Auto_Presets.h"
    #endif

    /*****************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Animation Effect:   Ambilight   ***************************************************************************************************************************************************************************
    **  @note:     **************************************************************************************************************************************************************************
    ************************************************************************************************************************************************************************************
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
    *******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Animation Effect:   Segments   ***************************************************************************************************************************************************************************
    **  @note:    New effect that merges HACS and WLED with segments and dynamic animation storage **************************************************************************************************************************************************************************
    ********************************************************************************************************************************************************************************
    *****************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    #define PALETTE_SOLID_WRAP (paletteBlend == 1 || paletteBlend == 3)

    #define MIN(a,b) ((a)<(b)?(a):(b))
    #define MAX(a,b) ((a)>(b)?(a):(b))

    /* Not used in all effects yet */
    #define WLED_FPS         42
    #define FRAMETIME_FIXED  (1000/WLED_FPS)
    #define FRAMETIME_MS     24
    #define FRAMETIME        getFrameTime()

    /* each segment uses 52 bytes of SRAM memory, so if you're application fails because of
      insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
    #ifdef ESP8266
      #define MAX_NUM_SEGMENTS    16
      /* How much data bytes all segments combined may allocate */
      #define MAX_SEGMENT_DATA  5120
    #else
      #ifndef MAX_NUM_SEGMENTS
        #define MAX_NUM_SEGMENTS  6//32
      #endif
      #define MAX_SEGMENT_DATA  32767
    #endif

    #define IBN 5100

    /* How much data bytes each segment should max allocate to leave enough space for other segments,
      assuming each segment uses the same amount of data. 256 for ESP8266, 640 for ESP32. */
    // #define FAIR_DATA_PER_SEG (MAX_SEGMENT_DATA / strip.getMaxSegments())

    #define MIN_SHOW_DELAY   (_frametime < 16 ? 8 : 15)

    #define SET_BRIGHTNESS true

    // options
    // bit    7: segment is in transition mode
    // bits 4-6: TBD
    // bit    3: mirror effect within segment
    // bit    2: segment is on
    // bit    1: reverse segment
    // bit    0: segment is selected
    #define NO_OPTIONS   (uint8_t)0x00
    #define TRANSITIONAL (uint8_t)0x80
    #define MIRROR       (uint8_t)0x08
    #define SEGMENT_ON   (uint8_t)0x04
    #define REVERSE      (uint8_t)0x02
    #define SELECTED     (uint8_t)0x01
    #define IS_TRANSITIONAL ((SEGMENT_I(_segment_index_primary).options & TRANSITIONAL) == TRANSITIONAL)
    #define IS_MIRROR       ((SEGMENT_I(_segment_index_primary).options & MIRROR      ) == MIRROR      )
    #define IS_SEGMENT_ON   ((SEGMENT_I(_segment_index_primary).options & SEGMENT_ON  ) == SEGMENT_ON  )
    #define IS_REVERSE      ((SEGMENT_I(_segment_index_primary).options & REVERSE     ) == REVERSE     )
    #define IS_SELECTED     ((SEGMENT_I(_segment_index_primary).options & SELECTED    ) == SELECTED    )

    // #define MIN_SHOW_DELAY   (_frametime < 16 ? 8 : 15)
    #define MINIMUM_SHOW_BACKOFF_PERIOD_MS 30//15

    /* How much data bytes all segments combined may allocate */
    #ifdef ESP8266
    #define MAX_SEGMENT_DATA 2048
    #else
    #define MAX_SEGMENT_DATA 8192
    #endif

    #define FLASH_COUNT 4 
    #define LED_SKIP_AMOUNT  0
    #define MIN_SHOW_DELAY  15
    #define DEFAULT_LED_COUNT 30

    #define DEFAULT_BRIGHTNESS (uint8_t)127
    #define DEFAULT_MODE       (uint8_t)0
    #define DEFAULT_SPEED      (uint8_t)128
    #define DEFAULT_INTENSITY  (uint8_t)128
    #define DEFAULT_COLOR      (uint32_t)0xFFAA00
    #define DEFAULT_C1         (uint8_t)128
    #define DEFAULT_C2         (uint8_t)128
    #define DEFAULT_C3         (uint8_t)16


    //Segment option byte bits
    #define SEG_OPTION_SELECTED       0
    #define SEG_OPTION_REVERSED       1
    #define SEG_OPTION_ON             2
    #define SEG_OPTION_MIRROR         3            //Indicates that the effect will be mirrored within the segment
    #define SEG_OPTION_NONUNITY       4            //Indicates that the effect does not use FRAMETIME_MS or needs getPixelColor
    #define SEG_OPTION_TRANSITIONAL   7

    // some common colors
    // white, red, green, blue
    #define RED        (uint32_t)0xFF0000
    #define GREEN      (uint32_t)0x00FF00
    #define BLUE       (uint32_t)0x0000FF
    #define WHITE      (uint32_t)0xFFFFFF
    #define BLACK      (uint32_t)0x000000
    #define YELLOW     (uint32_t)0xFFFF00
    #define CYAN       (uint32_t)0x00FFFF
    #define MAGENTA    (uint32_t)0xFF00FF
    #define PURPLE     (uint32_t)0x400080
    #define ORANGE     (uint32_t)0xFF3000
    #define PINK       (uint32_t)0xFF1493
    #define ULTRAWHITE (uint32_t)0xFFFFFFFF
    #define DARKSLATEGRAY (uint32_t)0x2F4F4F
    #define DARKSLATEGREY (uint32_t)0x2F4F4F

    #define NUM_COLORS 3 

    #define SEGIDX           getCurrSegmentId()

    /**
     * @brief 
     * Note: WLED used full colour RGB SEGCOLOR_U32, here SEGCOLOR_U32 going forward will also have brightness applied otherwise it is assumed as 100% brightness and therefore the same as WLED
     * 
     */
    #define SEGCOLOR_U32(x)      RgbcctColor::GetU32ColourBrightnessApplied(segments[getCurrSegmentId()].rgbcctcolors[x])
    #define SEGCOLOR_RGBCCT(x)      segments[getCurrSegmentId()].rgbcctcolors[x].GetColourWithBrightnessApplied()
    // #define SEGCOLOR_U32(s,x)  RgbcctColor::GetU32Colour(segments[s].rgbcctcolors[x])

    #define SEGMENT          segments[getCurrSegmentId()]
    #define SEGMENT_I(X)     segments[X]
    #define SEGLEN           _virtualSegmentLength

    #define SPEED_FORMULA_L  5U + (50U*(255U - SEGMENT.speed_value))/SEGLEN

    void fill(uint32_t c, bool apply_brightness = false);
    void fill_ranged(uint32_t c, bool apply_brightness = false); 
    // void seg_fill_ranged(uint32_t c, bool apply_brightness = false);

    uint32_t color_wheel(uint8_t pos);
    uint32_t ColourBlend(uint32_t color1, uint32_t color2, uint8_t blend);

    void Init_Segments();

    /**
     * Minor Code version 96 onwards requires names, as reshuffle is happening. Any old device will not respond to the correct command via number until remap is performed
     * */
    enum EFFECTS_FUNCTION__IDS
    {
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__SOLID_COLOUR__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__STATIC_PALETTE__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__SLOW_GLOW__ID,
    #endif
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__ID,
    EFFECTS_FUNCTION__WLED_CANDLE_MULTIPLE__ID,
    EFFECTS_FUNCTION__SHIMMERING_PALETTE__ID,
    EFFECTS_FUNCTION__SHIMMERING_PALETTE_TO_ANOTHER__ID,
    #endif













    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__ROTATING_PALETTE_NEW__ID,
    #endif
    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__ROTATING_PALETTE__ID,
    #endif
    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__ID,
    #endif
    /**
     * Desc: pixels are rotated
     * Para: direction of motion, speed, instant or blend change
     * 
     * draw static palette, then use neopixel to rotate with animator, no need for dynamic animationpair
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION_BLENDED__ID,
    #endif
    /**
     * Desc: Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     *       Stepping through them with a count, ie pixel 0/1 then 1/2 then 2/3, first pixel overwrite
     * Para: Amount of pixels to show from palette as it steps through (eg 2, 3 etc)
     * TODO: Add size of step as percentage ie to show 50% of 4 colours would be 2 of 4, 75% of 4 is 3
     * 
     * Note: allocate_buffer is used as transition data
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__STEPPING_PALETTE__ID,
    #endif
    /**
     * Desc: Spread palette across segment
     * If gradient value exists, then use it to spread colour across segment
     * If no index in palette, then spread palette equal distant across palette
     * */
    // #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
    // EFFECTS_FUNCTION__STATIC_PALETTE_GRADIENT__ID,
    // #endif
    /**
     * Desc: Animation, that fades from selected palette to white,
     *       The intensity of fade (closer to white, ie loss of saturation) will depend on intensity value
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__BLEND_PALETTE_SATURATION_TO_WHITE__ID,
    #endif
    /**
     * Desc: Animation, that fades from selected palette to white,
     *       The intensity of fade (closer to white, ie loss of saturation) will depend on intensity value
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__CANDLE_FLICKER_PALETTE_SATURATION_TO_WHITE__ID,
    #endif
    /**
     * Desc: Animation, that fades from selected palette to anothor palette,
     *       The intensity of fade (closer to palette B) will depend on intensity value
     *       ie intensity of 255 means Palette A (primary) can fade into palette B (set by option)
     * 
     *       New method to set options
     *        option8  for 255 value range.... ie allows animations to be configured and saved in their "aux0"
     *        option16 for 65000 value range
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__BLEND_PALETTE_BETWEEN_ANOTHER_PALETTE__ID,
    #endif
    /**
     * Desc: Draws palette_primary in order, then randomly takes from a second palette (saved by aux0)
     *       This will allow white palettes to work, or else applying coloured twinkles over a white palette too
     * 
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__ID,
    #endif
    /**
     * Desc: Picks colours from palette and randomly adds them as full brightness
     *       With each call of the animation (as new colours are added), all pixels will
     *       decay in brightness (easiest by half each time until 0).
     *       Note: The decay must happen first, so the new draws are still at full brightness
     *       This will require saving the total output in the buffer.
     * 
     * Para: 
     * TODO: 
     * 
     * Note: allocate_buffer is used as transition data
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__ID, //ie use "FadeOut()"
    #endif
    /**
     * Desc: Any set palette, will be set to span (with linearblend) the width of the segment
     *
     * 
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
    EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__ID,
    #endif

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS // SELECTIVE meaning optional extras then "of type notification"
    EFFECTS_FUNCTION__NOTIFICATION_STATIC__ID,
    EFFECTS_FUNCTION__NOTIFICATION_BLINKING__ID,
    EFFECTS_FUNCTION__NOTIFICATION_FADE__ID,
    EFFECTS_FUNCTION__NOTIFICATION_PULSING__ID,    
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS // SELECTIVE meaning optional extras then "of type notification"
  
  
    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: WLED Animations converted  ***************************************************************************************************************************************************************************
    **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/
    
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
    // Static
    EFFECTS_FUNCTION__WLED_STATIC__ID,
    EFFECTS_FUNCTION__WLED_STATIC_PATTERN__ID,
    EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__ID,
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__WLED_SPOTS__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__WLED_PERCENT__ID,
    // One colour changes
    EFFECTS_FUNCTION__WLED_RANDOM_COLOR__ID,
    // Wipe/Sweep/Runners     
    EFFECTS_FUNCTION__WLED_COLOR_WIPE__ID,
    EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__ID, // 1 direction only
    EFFECTS_FUNCTION__WLED_COLOR_SWEEP__ID,
    EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__ID, //start to end to start again

    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__ID,
    EFFECTS_FUNCTION__WLED_ANDROID__ID,
    EFFECTS_FUNCTION__WLED_RUNNING_RED_BLUE__ID,
    EFFECTS_FUNCTION__WLED_RUNNING_COLOR__ID,
    EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_GRADIENT__ID,
    EFFECTS_FUNCTION__WLED_LOADING__ID,
    EFFECTS_FUNCTION__WLED_POLICE__ID,
    EFFECTS_FUNCTION__WLED_POLICE_ALL__ID,
    EFFECTS_FUNCTION__WLED_TWO_DOTS__ID,
    EFFECTS_FUNCTION__WLED_TWO_AREAS__ID,
    EFFECTS_FUNCTION__WLED_MULTI_COMET__ID,
    EFFECTS_FUNCTION__WLED_OSCILLATE__ID,
    EFFECTS_FUNCTION__WLED_BPM__ID,
    EFFECTS_FUNCTION__WLED_JUGGLE__ID,
    EFFECTS_FUNCTION__WLED_PALETTE__ID,
    EFFECTS_FUNCTION__WLED_COLORWAVES__ID,
    EFFECTS_FUNCTION__WLED_LAKE__ID,
    EFFECTS_FUNCTION__WLED_GLITTER__ID,
    EFFECTS_FUNCTION__WLED_METEOR__ID, 
    EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__ID,
    EFFECTS_FUNCTION__WLED_PRIDE_2015__ID,
    EFFECTS_FUNCTION__WLED_PACIFICA__ID,
    EFFECTS_FUNCTION__WLED_SUNRISE__ID,
    EFFECTS_FUNCTION__WLED_SINEWAVE__ID,
    EFFECTS_FUNCTION__WLED_FLOW__ID,
    EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__ID,
    EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__ID,
    EFFECTS_FUNCTION__WLED_MERRY_CHRISTMAS__ID,
    EFFECTS_FUNCTION__WLED_HALLOWEEN__ID,
    // Chase
    EFFECTS_FUNCTION__WLED_CHASE_COLOR__ID,
    EFFECTS_FUNCTION__WLED_CHASE_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__ID, 
    EFFECTS_FUNCTION__WLED_CHASE_FLASH__ID,
    EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__ID, 
    EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__ID,
    EFFECTS_FUNCTION__WLED_CHASE_THEATER__ID,
    EFFECTS_FUNCTION__WLED_CHASE_THEATER_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_CHASE_TRICOLOR__ID,
    EFFECTS_FUNCTION__WLED_CIRCUS_COMBUSTUS__ID,
    // Breathe/Fade/Pulse
    EFFECTS_FUNCTION__WLED_BREATH__ID,
    EFFECTS_FUNCTION__WLED_FADE__ID,
    EFFECTS_FUNCTION__WLED_FADE_TRICOLOR__ID,
    EFFECTS_FUNCTION__WLED_FADE_SPOTS__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE    
    // Fireworks
    EFFECTS_FUNCTION__WLED_FIREWORKS__ID,
    EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING__ID,
    EFFECTS_FUNCTION__WLED_FIREWORKS_EXPLODING_NO_LAUNCH__ID,
    EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST__ID,
    EFFECTS_FUNCTION__WLED_FIREWORKS_STARBURST_GLOWS__ID,
    EFFECTS_FUNCTION__WLED_RAIN__ID,
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    // Sparkle/Twinkle
    EFFECTS_FUNCTION__WLED_SOLID_GLITTER__ID,
    EFFECTS_FUNCTION__WLED_POPCORN__ID,
    EFFECTS_FUNCTION__WLED_PLASMA__ID,
    EFFECTS_FUNCTION__WLED_SPARKLE__ID,
    EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__ID,
    EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__ID,
    EFFECTS_FUNCTION__WLED_TWINKLE__ID,
    EFFECTS_FUNCTION__WLED_COLORTWINKLE__ID,
    EFFECTS_FUNCTION__WLED_TWINKLEFOX__ID,
    EFFECTS_FUNCTION__WLED_TWINKLECAT__ID,
    EFFECTS_FUNCTION__WLED_TWINKLEUP__ID,
    EFFECTS_FUNCTION__WLED_SAW__ID,
    EFFECTS_FUNCTION__WLED_DISSOLVE__ID,
    EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__ID,
    EFFECTS_FUNCTION__WLED_COLORFUL__ID,
    EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE    
        
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
    // Blink/Strobe
    EFFECTS_FUNCTION__WLED_BLINK__ID,
    EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_STROBE__ID,
    EFFECTS_FUNCTION__WLED_MULTI_STROBE__ID,
    EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__ID, 
    EFFECTS_FUNCTION__WLED_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_LIGHTNING__ID,     
    EFFECTS_FUNCTION__WLED_FIRE_2012__ID,
    EFFECTS_FUNCTION__WLED_RAILWAY__ID,
    EFFECTS_FUNCTION__WLED_HEARTBEAT__ID, 
    // Noise
    EFFECTS_FUNCTION__WLED_FILLNOISE8__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_1__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_2__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_3__ID,
    EFFECTS_FUNCTION__WLED_NOISE16_4__ID,
    EFFECTS_FUNCTION__WLED_NOISEPAL__ID,
    EFFECTS_FUNCTION__WLED_PHASEDNOISE__ID,
    EFFECTS_FUNCTION__WLED_PHASED__ID,
    // Scan
    EFFECTS_FUNCTION__WLED_SCAN__ID,
    EFFECTS_FUNCTION__WLED_DUAL_SCAN__ID,
    EFFECTS_FUNCTION__WLED_LARSON_SCANNER__ID,
    EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__ID,
    EFFECTS_FUNCTION__WLED_ICU__ID,
    EFFECTS_FUNCTION__WLED_RIPPLE__ID,
    EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_COMET__ID,
    EFFECTS_FUNCTION__WLED_CHUNCHUN__ID,
    EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__ID,
    EFFECTS_FUNCTION__WLED_SINELON__ID,
    EFFECTS_FUNCTION__WLED_SINELON_DUAL__ID,
    EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__ID,
    EFFECTS_FUNCTION__WLED_DRIP__ID,     
    // Added 2022
    EFFECTS_FUNCTION__WLED_BLENDS__ID,            // mode_blends
    EFFECTS_FUNCTION__WLED_TV_SIMULATOR__ID,      // mode_tv_simulator
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE    
    EFFECTS_FUNCTION__WLED_LENGTH__ID, // to show end of declared animation, this will have no actual effect     
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED


    /**
     * Development effects
     * @note These need to be last since without proper defines of effect names they can only be addressed by their numeric ID
     * 
     */
    
    // /**
    //  * Desc: Twinkle on, fade off
    //  *      Random burst of pixels are turned on based on palette, then fade off
    //  * 
    //  * Method: randomly pick burst of pixels to turn on, ie animation period starts of Y, then decays (thereby increasing next run time)
    //  *         Do this until X pixels (animation periods) have occured,
    //  *         Every 1 second of animation that elapses, half the brightness of all pixels (ie the fade)
    //  *         Intensity1: will dictate how many pixels are turned on
    //  *         Intensity2: may dictate the decay rate (ie much slower would leave pixels on for longer meaning more of the lights remain on) 
    //  * NotActive
    //  * EFFECTS_FUNCTION__PULSE_RANDOM_ON_FADE_OFF_ID
    //  * */
    // EFFECTS_FUNCTION__TWINKLE_RANDOM_BURSTS_ON_WITH_SLOW_FADE_OFF__ID,
    // /**
    //  * Palette is first drawn with certain lower brightness level, then
    //  * additional changes takes those colours and bumbs their brightness up to desired brightness
    //  * Few change, random pixels bumped up
    //  * Check if their brightness is <upper, before asserting them brighter (or convert to hsbcolour, flick up brightness if needed then change back to rgb)
    //  * Goes from low to high then stops
    //  * */
    // EFFECTS_FUNCTION__POPPING_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_BOUNDERY,
    // /**
    //  * Desc: Twinkle
    //  *  Random pixels will turn off for random times (ie apply inorder palette always, then random turn colours to off and on(brightness) again)
    //  * */

    // /**
    //  * Desc: Twinkle with glimmer
    //  *  Twinkle as above, but the entire global brightness will flicker gently (so limit how much) based on pixels energy count ie twinkle filament lights
    //  * */

    // /**
    //  * 0/100% animaiton progress is low brightness, 50% flips to upper brightness
    //  * ie randomly change pixels between two brightness levels, with optional boundaries only or as range
    // */
    // EFFECTS_FUNCTION__TWINKLE_PALETTE_BRIGHTNESS_FROM_LOWER_TO_UPPER_AND_BACK,

    // /**
    //  *  Instead of just applying new colours, also randomise the brightness levels between range
    //  * */
    // EFFECTS_FUNCTION__SLOW_GLOW_ON_BRIGHTNESS__ID,

    // /**
    //  * Desc: randomly slow glow with triangular brightness between colours
    //  * Or, so at 50% transition, the new colour is at 100%, with 100% brightness, then fade brightness back down to lower_global brightness
    //  * So, pixel red (50%) changing to pixel blue, would go red to blue (bright 100%) then fade down brightness 50%
    //  * Optional, could also be non-equal triangle ie have value for peak of traingle, so 20% would mean 0.2 seconds flash to new colour then slowly decay (0.8 seconds) to new brightness
    //  * */
    // // EFECT

    // /**
    //  * Fire animations
    //  * rename from fireplace to another name, simply fire
    //  * */

    // this will likely just use candle effect, maybe forced colour palette?/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
     EFFECTS_FUNCTION__FIREPLACE_1D_01__ID, // Custom to using matrix for fireplace light effect
    #endif 

    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Matric 3D Animations (with MxN pixel grid) ***************************************************************************************************************************************************************************
    **  Requires:     **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/



    // EFFECTS_FUNCTION__SLOW_FADE_BRIGHTNESS_ALL__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__SLOW_FADE_SATURATION_ALL__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__SLOW_FADE_BRIGHTNESS_RANDOM__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__SLOW_FADE_SATURATION_RANDOM__ID, // change ALL, 0 - 100%
    // EFFECTS_FUNCTION__FLASH_TWINKLE_SINGLE_COLOUR_RANDOM__ID, //random leds flash to 100% brightness (modes=instant on/off, multiple pulses)
    // EFFECTS_FUNCTION__FLASH_TWINKLE_PALETTE_COLOUR_RANDOM__ID,   
   
    /******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    *** Specialised: Sun positions to control animation***************************************************************************************************************************************************************************
    **  Requires:    Internet to know time and location for sun calculations **************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************
    ******************************************************************************************************************************************************************************/

    // //linear palette group colour, changing by triggered and over a period of time (eg an alarm -30 minutes)
    // EFFECTS_FUNCTION__SUNPOSITIONS_SUNRISE_ALARM_01, //group01? bring all together, have settings to configure how the effect runs
    // //linear palettes group colour, enabled by a time window, but sets linear gradient by sun azimuth
    // EFFECTS_FUNCTION__SUNPOSITIONS_GRADIENT_SUN_ELEVATION_01, //this is elevation only
    // //enabled by a time window, GENERATE manually, brightness by sun elevaltion, sun position from left to right by azimuth
    // //far left = east from position, and far right = wast from position using sunset position
    // EFFECTS_FUNCTION__SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_01,
    // EFFECTS_FUNCTION__SUNPOSITIONS_GRADIENT_SUN_ELEVATION_AND_AZIMUTH_2D_01, // If matrix/grid is connected, enable drawing this

    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_01__ID,
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_02__ID,
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_03__ID, // Using stored rgbcct palette
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_04__ID, // Using stored rgbcct palette
    // EFFECTS_FUNCTION__SUNPOSITIONS_SOLID_COLOUR_BASED_ON_SUN_ELEVATION_ONLY_05__ID, // CCT only, mapped directly, no palette

    /**
     * @brief 
     * 
     * sunrise/sunset, ie day is one colour of rgbcct and night is another (two colours only) 
     * sunrise/sunset controls colour_temp. Note: I need optional "elevation above horizon for transition" ie 5 degrees above horizon consider "sun down"
     *         or, instead of sun position, start change 30 minutes priod to sun elevation (with adjustment allowed)
     * 
     * 
     */
    
    // /**
    //  * Eleveation controls that use rgbcct palette (that has elevation in its index byte) to display solid colour only
    //  * */
    // EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01__ID,
    // EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01__ID,

    // /**
    //  * Elevation controls the CCT channels only
    //  **/
    // EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01__ID,


    // // palette to step through, which gives single colour sun
    // EFFECTS_FUNCTION__SUNPOSITIONS_STEP_RGBCCT_ALARM_01,
    // //enabled CCT by azimuth 
    // EFFECTS_FUNCTION__SUNPOSITIONS_STEP_RGBCCT_SUN_ELEVATION_01,

    
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS 
    /**
     * @brief Same as above, but no blending on palette colours, just use stepped values
     **/
    EFFECTS_FUNCTION__SUNPOSITION_ELEVATION_PALETTE_PROGRESS_STEP__ID,
    /**
     * @brief Using the elevation of the sun stored in settings, linear blend between all colours in a palette
     *        The start and end colour from the palette (0 to 100% index) is defined by the max/min elevation for that day
     *        I will also want a way to pick what these max/min are (ie setting dusk/dawn as limits for transition for CCT colours etc)
     **/
    EFFECTS_FUNCTION__SUNPOSITION_ELEVATION_PALETTE_PROGRESS_LINEAR__ID,
   
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

    // /******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // *** Specialised: RGB Segment Clock  ***************************************************************************************************************************************************************************
    // **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************/

    // EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
    // EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
    // EFFECTS_FUNCTION__LCD_DISPLAY_BASIC_01__ID, // show number commanded via mqtt


    // /******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // *** Specialised: RGB Segment Clock  ***************************************************************************************************************************************************************************
    // **  Requires:    Based on 3D printed clock, requires pixels in grid formation. In the future, perhaps parts of number could be wled segments with segments added to be number **************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************
    // ******************************************************************************************************************************************************************************/




    // // Apply sinewave to brightness, 25% increasing, 50% decreasing, 25% off
    // EFFECTS_FUNCTION__PULSE_SINE_WAVE_BRIGHTNESS, //pul
    // // The one when applybrightness was broke
    // // done by applying new cl/brightness to some, then each call reducing the colour crushing it
    // //do with percent to show on at one time
    // EFFECTS_FUNCTION__PULSE_RANDOM_ON, 
    // EFFECTS_FUNCTION__PULSE_RANDOM_ON_TWO__ID,    
    // EFFECTS_FUNCTION__FLASH_TWINKLE_SEQUENTIAL__ID, // sequential flash of white on x leds 
    // EFFECTS_FUNCTION__FADE_GRADIENT__ID, //single pixels: static, twinkle, pick from palette
    // //another flash to "off" or simple set flash colour to off??
    // EFFECTS_FUNCTION__FLASH_GLIMMER_RANDOM__ID, // tinkle=leds flash independant, glimmer=leds flash with dimming effect on others
    /**
     * Show an exact amount of pixels only from a palette, where "show_length" would be pixel=0:pixel_length
     * */
    // EFFECTS_FUNCTION__SHOWING_MULTIPLES_OF_COLOURS__ID,
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

    EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID


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
    // EFFECTS_FUNCTION__CHRISTMAS_EIGHT_FUNCTION_CONTROLLER_IN_WAVES_ID
    // EFFECTS_FUNCTION__CHRISTMAS_EIGHT_FUNCTION_CONTROLLER__SEQUENTIAL_ID
    // EFFECTS_FUNCTION__CHRISTMAS_EIGHT_FUNCTION_CONTROLLER__SLO_GLO_ID
    // EFFECTS_FUNCTION__CHRISTMAS_EIGHT_FUNCTION_CONTROLLER__CHASING_AND_FLASHING_ID
    // EFFECTS_FUNCTION__CHRISTMAS_EIGHT_FUNCTION_CONTROLLER__SLOW_FADE_ID
    // EFFECTS_FUNCTION__CHRISTMAS_EIGHT_FUNCTION_CONTROLLER__TWINKLE_AND_FLASH_ID
    // EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    // EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID
    // EFFECTS_FUNCTION__XMAS_INWAVES_SINE_WINE_BLEND_OF_GROUP_ID


    // need to make another effect, at least until later palettes can be stretched (wrapped?) across the segment
    // This effect here "Static Palette Spanned" will allow immerison tank to show the colour from paletteEdit 15 correctly

    /**
     * Development methods
     * */

    /**
     * Functional methods of sunrise
     * EFFECTS_FUNCTION__SUNPOSITIONS_<SOLIDCOLOUR/2D_ARRAY>
     * */
    //create a palette to leave on ALL the time, ie deep red at night instead of blue... testing for the clocks later I guess

    /**
     * Not currently working
     * */
    /**
     * Clock animations for 3d printed display
     **/
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
    EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__ID,
    EFFECTS_FUNCTION__LCD_CLOCK_BASIC_02__ID,
    EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_NUMBER_01__ID,
    EFFECTS_FUNCTION__LCD_DISPLAY_MANUAL_STRING_01__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

    /**
     * Create special musical christmas lights, that will only be available on esp32 with a speaker (larger RAM)
     * */
    #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
    /**
     * Palette developing
     * */
    // Step 1 of palette merging: Making it so any palette can span the entire segment, static, no effect.
    EFFECTS_FUNCTION__STATIC_PALETTE_SPANNING_SEGMENT__ID,
    /**
     * Designing and quick test of animations before creating its own animaiton profile
     * */
    EFFECTS_FUNCTION__TESTER_01__ID,
    EFFECTS_FUNCTION__TESTER_02__ID,
    EFFECTS_FUNCTION__STATIC_PALETTE_NEW__ID,
    EFFECTS_FUNCTION__SLOW_GLOW_NEW__ID,
    /**
     * @brief Testing can animation_colours be moved into the dynamic data array
     * 
     */
    // EFFECTS_FUNCTION__STATIC_PALETTE_DYNAMIC_METHOD__ID,
    // EFFECTS_FUNCTION__SLOW_GLOW_DYNAMIC_METHOD__ID,
    #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING

    // Length
    EFFECTS_FUNCTION__LENGTH__ID
  };         

  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  #define DEFAULT_EFFECTS_FUNCTION    EFFECTS_FUNCTION__STATIC_PALETTE__ID
  #else
  #define DEFAULT_EFFECTS_FUNCTION    0
  #endif

  void SubTask_Segments_Animation();
  void Segments_RefreshLEDIndexPattern(uint8_t segment_index = 0);
  
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

  #define NUM_COLORS2       3 /* number of colors per segment */

  typedef union {
    uint16_t data; // allows full manipulating
    struct { 
      // enable animations (pause)
      uint16_t fEnable_Animation : 1;
      uint16_t fForceUpdate : 1; 
      uint16_t fRunning : 1;
      uint16_t animator_first_run : 1;
      // Reserved
      uint16_t reserved : 12;
    };
  } ANIMATION_FLAGS;

  uint8_t GetSizeOfPixel(RgbcctColor::ColourType colour_type);


  /**
   * Transition settings
   * */
  struct TRANSITION_SETTINGS{
    /**
     * Pixels to change up to maximum of segment
     * */
    uint16_t pixels_to_update_as_number  = 1;
    /**
     * Refresh rate, calculate new colours
     * */
    uint16_t rate_ms = 2000;
    /**
     * Time to new colours
     * */
    uint16_t time_ms = 1000;    
  };


  struct TransitionColourPairs
  {
    RgbcctColor StartingColour;
    RgbcctColor DesiredColour;
  };
  
  // TransitionColourPairs* 
  void GetTransitionColourBuffer(byte* allocated_buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor::ColourType pixel_type, mAnimatorLight::TransitionColourPairs* pair_test);
  bool SetTransitionColourBuffer(byte* allocated_buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor::ColourType pixel_type, RgbcctColor starting_colour, RgbcctColor desired_colour);


  /**
   * @brief New method that allows getting WLED basic 3 colours, but also applying the colour when getting, the same as my palette method
   * 
   */
  RgbcctColor GetSegmentColour(uint8_t colour_index, uint8_t segment_index = 0);

  enum EFFECTSREGION
  {
      EFFECTS_REGION_COLOUR_SELECT_ID=0,
      EFFECTS_REGION_ANIMATE_ID
  };      


  void Init_Segments_RgbcctControllers();
  mAnimatorLight& SetSegment_AnimFunctionCallback(uint8_t segment_index, ANIM_FUNCTION_SIGNATURE);
  void StartSegmentAnimation_AsAnimUpdateMemberFunction(uint8_t segment_index = 0);
  void Segments_SetPixelColor_To_Static_Pallete(uint16_t palette_id);
  void Segments_UpdateDesiredColourFromPaletteSelected(uint16_t segment_index = 0);
  void AnimationProcess_Generic_AnimationColour_LinearBlend_Segments(const AnimationParam& param);
  void AnimationProcess_LinearBlend_Dynamic_Buffer(const AnimationParam& param);
  void AnimationProcess_SingleColour_LinearBlend_Dynamic_Buffer(const AnimationParam& param);
  void AnimationProcess_SingleColour_LinearBlend_Between_RgbcctSegColours(const AnimationParam& param);

  bool SetTransitionColourBuffer_StartingColour(byte* buffer, uint16_t buflen, uint16_t pixel_index, RgbcctColor::ColourType pixel_type, RgbcctColor starting_colour);
  bool SetTransitionColourBuffer_DesiredColour(byte* buffer, uint16_t buflen, uint16_t pixel_index,  RgbcctColor::ColourType pixel_type, RgbcctColor starting_colour);

  void DynamicBuffer_Segments_UpdateStartingColourWithGetPixel();
  void DynamicBuffer_Segments_UpdateStartingColourWithGetPixel_FromBus();
  void Segments_Dynamic_Buffer_UpdateStartingColourWithGetPixel();


  /**
   * My animations (and their animators where applicable)
   * */
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  void SubTask_Segment_Animate_Function__Solid_Colour(); 
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  void SubTask_Segment_Animate_Function__Static_Palette();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  void SubTask_Segment_Animate_Function__Slow_Glow();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
  void SubTask_Segment_Animation__Flicker_Base(bool use_multi = false, uint16_t flicker_palette = 0);
  void SubTask_Segment_Animation__Candle_Single();
  void SubTask_Segment_Animation__Candle_Multiple();
  void SubTask_Segment_Animation__Shimmering_Palette();
  void SubTask_Segment_Animation__Shimmering_Palette_To_Another_Palette();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  void SubTask_Segment_Animate_Function__Static_Gradient_Palette();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  void SubTask_Segment_Animation__Stepping_Palette();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  void SubTask_Segment_Animation__Rotating_Palette_New();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  void SubTask_Segment_Animation__Rotating_Palette();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  void SubTask_Segment_Animation__Rotating_Previous_Animation();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  void Segments_RotateDesiredColour(uint8_t pixels_amount_to_shift, uint8_t direction);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  void SubTask_Segment_Animation__Blend_Palette_To_White();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  void SubTask_Segment_Animation__Blend_Palette_Between_Another_Palette();
  #endif
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  void SubTask_Segment_Animation__Twinkle_Palette_Onto_Palette();
  #endif  
  // Static
  void SubTask_Segment_Animation__Static();
  void SubTask_Segment_Animation__Static_Pattern();
  void SubTask_Segment_Animation__Tri_Static_Pattern();
  void SubTask_Segment_Animation__Base_Spots(uint16_t threshold);
  void SubTask_Segment_Animation__Spots();
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  void SubTask_Segment_Animation__Percent();
  #endif
  // One colour changes
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  void SubTask_Segment_Animation__Random_Colour();
  #endif
  // Wipe/Sweep/Runners 
  void BaseSubTask_Segment_Animation__Base_Colour_Wipe(bool rev, bool useRandomColors);
  void SubTask_Segment_Animation__Colour_Wipe();
  void SubTask_Segment_Animation__Colour_Wipe_Random();
  void SubTask_Segment_Animation__Colour_Sweep();
  void SubTask_Segment_Animation__Colour_Sweep_Random();
  void SubTask_Segment_Animation__TriColour();
  void SubTask_Segment_Animation__Android();
  void SubTask_Segment_Animation__Base_Running(bool saw);
  void SubTask_Segment_Animation__Base_Running(uint32_t color1, uint32_t color2);
  void SubTask_Segment_Animation__Running_Red_Blue();
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  void SubTask_Segment_Animation__Running_Colour();
  void SubTask_Segment_Animation__Running_Random();
  void SubTask_Segment_Animation__Base_Gradient(bool loading);
  void SubTask_Segment_Animation__Gradient();
  void SubTask_Segment_Animation__Loading();
  void SubTask_Segment_Animation__Base_Police(uint32_t color1, uint32_t color2, bool all);
  void SubTask_Segment_Animation__Police();
  void SubTask_Segment_Animation__Polce_All();
  void SubTask_Segment_Animation__Two_Dots();
  void SubTask_Segment_Animation__Two_Areas();
  void SubTask_Segment_Animation__Multi_Comet();
  void SubTask_Segment_Animation__Oscillate();
  void SubTask_Segment_Animation__BPM();
  void SubTask_Segment_Animation__Juggle();
  void SubTask_Segment_Animation__Palette();
  void SubTask_Segment_Animation__ColourWaves();
  void SubTask_Segment_Animation__Lake();
  void SubTask_Segment_Animation__Glitter();
  void SubTask_Segment_Animation__Meteor();
  void SubTask_Segment_Animation__Metoer_Smooth();    
  void SubTask_Segment_Animation__Pride_2015();    
  CRGB pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);   
  void SubTask_Segment_Animation__Pacifica();    
  void SubTask_Segment_Animation__Sunrise();    
  void SubTask_Segment_Animation__Sinewave();    
  void SubTask_Segment_Animation__Flow();    
  void SubTask_Segment_Animation__Base_Phased(uint8_t moder);
  void SubTask_Segment_Animation__PhasedNoise();    
  void SubTask_Segment_Animation__Phased();    
  void SubTask_Segment_Animation__Running_Lights();    
  void SubTask_Segment_Animation__Rainbow_Cycle();    
  void SubTask_Segment_Animation__Merry_Christmas();    
  void SubTask_Segment_Animation__Halloween();    
  // Chase    
  void SubTask_Segment_Animation__Base_Chase(uint32_t color1, uint32_t color2, uint32_t color3, bool do_palette);
  void SubTask_Segment_Animation__Chase_Colour();
  void SubTask_Segment_Animation__Chase_Random();
  void SubTask_Segment_Animation__Chase_Rainbow();
  void SubTask_Segment_Animation__Base_Chase_Theater(uint32_t color1, uint32_t color2, bool do_palette);
  void SubTask_Segment_Animation__Chase_Flash();
  void SubTask_Segment_Animation__Chase_Flash_Random();
  void SubTask_Segment_Animation__Chase_Rainbow_White();
  void SubTask_Segment_Animation__Chase_Theater();
  void SubTask_Segment_Animation__Chase_Theatre_Rainbow();
  void SubTask_Segment_Animation__Base_Chase_TriColour(uint32_t color1, uint32_t color2);
  void SubTask_Segment_Animation__Chase_TriColour();
  void SubTask_Segment_Animation__Circus_Combustus();
  // Breathe/Fade/Pulse
  void SubTask_Segment_Animation__Breath();
  void SubTask_Segment_Animation__Fade();
  void SubTask_Segment_Animation__Fade_TriColour();
  void SubTask_Segment_Animation__Fade_Spots();
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  // Fireworks
  void SubTask_Segment_Animation__Fireworks();
  void SubTask_Segment_Animation__Exploding_Fireworks();
  void SubTask_Segment_Animation__Fireworks_Starburst();
  void SubTask_Segment_Animation__Fireworks_Starburst_Glows();
  void SubTask_Segment_Animation__Rain();
  void SubTask_Segment_Animation__Exploding_Fireworks_NoLaunch();
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  // Sparkle/Twinkle
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  void SubTask_Segment_Animation__Solid_Glitter();
  void SubTask_Segment_Animation__Popcorn();
  void SubTask_Segment_Animation__Plasma();
  void SubTask_Segment_Animation__Sparkle();
  void SubTask_Segment_Animation__Sparkle_Flash();
  void SubTask_Segment_Animation__Sparkle_Hyper();
  void SubTask_Segment_Animation__Twinkle();
  CRGB SubTask_Segment_Animation__Base_Twinkle_Fox_One_Twinkle(uint32_t ms, uint8_t salt, bool cat);
  void SubTask_Segment_Animation__Base_Twinkle_Fox(bool cat);
  void SubTask_Segment_Animation__Twinkle_Colour();
  void SubTask_Segment_Animation__Twinkle_Fox();
  void SubTask_Segment_Animation__Twinkle_Cat();
  void SubTask_Segment_Animation__Twinkle_Up();
  void SubTask_Segment_Animation__Saw();
  void SubTask_Segment_Animation__Base_Dissolve(uint32_t color);
  void SubTask_Segment_Animation__Dissolve();
  void SubTask_Segment_Animation__Dissolve_Random();
  void SubTask_Segment_Animation__ColourFul();
  void SubTask_Segment_Animation__Traffic_Light();
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE  
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  // Blink/Strobe
  void SubTask_Segment_Animation__Base_Blink(uint32_t color1, uint32_t color2, bool strobe, bool do_palette);
  void SubTask_Segment_Animation__Blink();
  void SubTask_Segment_Animation__Blink_Rainbow();
  void SubTask_Segment_Animation__Strobe();
  void SubTask_Segment_Animation__Strobe_Multi();
  void SubTask_Segment_Animation__Strobe_Rainbow();
  void SubTask_Segment_Animation__Rainbow();
  void SubTask_Segment_Animation__Lightning();
  void SubTask_Segment_Animation__Fire_2012();
  void SubTask_Segment_Animation__Railway();
  void SubTask_Segment_Animation__Heartbeat();
  //Noise
  void SubTask_Segment_Animation__FillNoise8();
  void SubTask_Segment_Animation__Noise16_1();
  void SubTask_Segment_Animation__Noise16_2();
  void SubTask_Segment_Animation__Noise16_3();
  void SubTask_Segment_Animation__Noise16_4();
  void SubTask_Segment_Animation__Noise_Pal();
  // Scan
  void SubTask_Segment_Animation__Base_Scan(bool dual);
  void SubTask_Segment_Animation__Scan();
  void SubTask_Segment_Animation__Scan_Dual();
  void SubTask_Segment_Animation__Base_Larson_Scanner(bool dual);
  void SubTask_Segment_Animation__Larson_Scanner();
  void SubTask_Segment_Animation__Larson_Scanner_Dual();
  void SubTask_Segment_Animation__ICU();
  void SubTask_Segment_Animation__Base_Ripple(bool rainbow);
  void SubTask_Segment_Animation__Ripple();
  void SubTask_Segment_Animation__Ripple_Rainbow(); 
  void SubTask_Segment_Animation__Comet();
  void SubTask_Segment_Animation__Chunchun();
  void SubTask_Segment_Animation__Bouncing_Balls();
  void SubTask_Segment_Animation__Base_Sinelon(bool dual, bool rainbow=false);
  void SubTask_Segment_Animation__Sinelon();
  void SubTask_Segment_Animation__Sinelon_Dual();
  void SubTask_Segment_Animation__Sinelon_Rainbow();
  void SubTask_Segment_Animation__Drip();
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  void SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_Step();       
  void SubTask_Segment_Animation__SunPositions_Elevation_Palette_Progress_LinearBlend();   
  void SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_RGBCCT_Palette_Indexed_Positions_01();
  void SubTask_Segment_Animate_Function__SunPositions_Elevation_Only_Controlled_CCT_Temperature_01();
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
  void SubTask_Flasher_Animate_Function_Tester_01();
  void SubTask_Flasher_Animate_Function_Tester_02();
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
  void SubTask_Segment_Animate_Function__Static_Palette_New();
  void SubTask_Segment_Animate_Function__Slow_Glow_New();

  // Temporary helper functions to be cleaned up and converted
  void blur(uint8_t blur_amount, bool set_brightness = false);
  void fade_out(uint8_t rate, bool set_brightness = false);
  uint32_t crgb_to_col(CRGB fastled);
  CRGB col_to_crgb(uint32_t);
  uint8_t get_random_wheel_index(uint8_t pos);
  uint16_t triwave16(uint16_t in);
  uint16_t mode_palette();

  #ifdef ENABLE_DEVFEATURE_LIGHT__REQUIRE_MINIMUM_BACKOFF_ON_ANIMATION_TO_MAYBE_FIX_BUS_FLICKER
  uint32_t tSaved_MinimumAnimateRunTime = millis();
  #endif

  void CommandSet_ColourTypeID(uint8_t id, uint8_t segment_index = 0);
  const char* GetColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen);
  int8_t GetColourTypeIDbyName(const char* c);

  void CommandSet_ColourHeatMap_Palette(float* array_val, uint8_t array_length, uint8_t style_index = 0, uint8_t palette_id = 255);

  void Set_Segment_ColourType(uint8_t segment_index, uint8_t light_type);


  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  void SubTask_Segment_Animate_Function__Notification_Static();
  void SubTask_Segment_Animate_Function__Notification_Fade();
  void SubTask_Segment_Animate_Function__Notification_Blinking();
  void SubTask_Segment_Animate_Function__Notification_Pulsing();
  void SubTask_Segment_Animate_Function__Notification_Base(bool flag_static = false, bool flag_blink = false, bool flag_pulse = false);
  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS



  #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  byte displayMode = 1;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)
  byte lastSecond = 0;
  uint16_t testnum = 0;
  byte startColor = 0;                             // "index" for the palette color used for drawing
  // byte displayMode = 0;                            // 0 = 12h, 1 = 24h (will be saved to EEPROM once set using buttons)
  byte colorOffset = 29;//32;                           // default distance between colors on the color palette used between digits/leds (in overlayMode)

  void LCDDisplay_colorOverlay() ;
  void LCDDisplay_updateDisplay(byte color, byte colorSpacing) ;

  #define LED_PWR_LIMIT 750                        // 750mA - Power limit in mA (voltage is set in setup() to 5v)
  #define LED_DIGITS 4                             // 4 or 6 digits, can only be an even number as...
  // notice 3 less below since I soldered, no single strip
  #define LED_PER_DIGITS_STRIP 44//47                  // ...two digits are made out of one piece of led strip with 47 leds...
  #define LED_BETWEEN_DIGITS_STRIPS 5              // 5 leds between above strips - and all this gives us LED_COUNT... :D
  #define LED_COUNT ( LED_DIGITS / 2 ) * LED_PER_DIGITS_STRIP + ( LED_DIGITS / 3 ) * LED_BETWEEN_DIGITS_STRIPS

  byte segGroups[14][2] = {         // 14 segments per strip, each segment has 1-x led(s). So lets assign them in a way we get something similar for both digits
    // right (seen from front) digit. This is which led(s) can be seen in which of the 7 segments (two numbers: First and last led inside the segment, same on TE):
    {  13,  15 },                     // top, a
    {  10, 12 },                     // top right, b
    { 6, 8 },                     // bottom right, c
    { 3, 5 },                     // bottom, d
    { 0, 2 },                     // bottom left, e
    {  16,  18 },                     // top left, f
    {  19,  21 },                     // center, g
    // left (seen from front) digit
    { 35, 37 },                     // top, a
    { 38, 40 },                     // top right, b
    { 22, 24 },                     // bottom right, c
    { 25, 27 },                     // bottom, d
    { 28, 30 },                     // bottom left, e
    { 32, 34 },                     // top left, f
    { 41, 43 }                      // center, g
  };


  // Using above arrays it's very easy to "talk" to the segments. Simply use 0-6 for the first 7 segments, add 7 (7-13) for the following ones per strip/two digits
  byte digits[14][7] = {                    // Lets define 10 numbers (0-9) with 7 segments each, 1 = segment is on, 0 = segment is off
    {   1,   1,   1,   1,   1,   1,   0 },  // 0 -> Show segments a - f, don't show g (center one)
    {   0,   1,   1,   0,   0,   0,   0 },  // 1 -> Show segments b + c (top and bottom right), nothing else
    {   1,   1,   0,   1,   1,   0,   1 },  // 2 -> and so on...
    {   1,   1,   1,   1,   0,   0,   1 },  // 3
    {   0,   1,   1,   0,   0,   1,   1 },  // 4
    {   1,   0,   1,   1,   0,   1,   1 },  // 5
    {   1,   0,   1,   1,   1,   1,   1 },  // 6
    {   1,   1,   1,   0,   0,   0,   0 },  // 7
    {   1,   1,   1,   1,   1,   1,   1 },  // 8
    {   1,   1,   1,   1,   0,   1,   1 },  // 9
    {   0,   0,   0,   1,   1,   1,   1 },  // t -> some letters from here on (index 10-13, so this won't interfere with using digits 0-9 by using index 0-9
    {   0,   0,   0,   0,   1,   0,   1 },  // r
    {   0,   1,   1,   1,   0,   1,   1 },  // y
    {   0,   1,   1,   1,   1,   0,   1 }   // d
  };

  void SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_01();
  void SubTask_Segment_Animate_Function__LCD_Clock_Time_Basic_02();
  void SubTask_Flasher_Animate_LCD_Display_Show_Numbers_Basic_01();
  void SubTask_Flasher_Animate_LCD_Display_Show_String_01();
  void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_01();
  void ConstructJSONBody_Animation_Progress__LCD_Clock_Time_Basic_02();
  void LCDDisplay_displayTime(time_t t, byte color, byte colorSpacing);
  void LCDDisplay_showDigit(byte digit, byte color, byte pos);
  void LCDDisplay_showSegment(byte segment, byte color, byte segDisplay);
  void LCDDisplay_showDots(byte dots, byte color);
  // RgbcctColor ColorFromPaletteLCD(uint16_t palette_id, uint8_t index, bool apply_global_brightness = true);
  uint8_t tempcol = 0;
  uint16_t lcd_display_show_number = 0;
  char lcd_display_show_string[5] = {0}; //temporary solution, will be removed once newer commend to save effect runtime struct works

  #endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK


  void Segment_SubTask_Flasher_Animate_Function__TEST_SolidRandom();
  void Segments_SetLEDOutAmountByPercentage(uint8_t percentage, uint8_t segment_index = 0); 
  void resetSegments1();
  

  /***************
   * 
   * Command List
   * 
   * START
   * *********************/
  
  void CommandSet_PaletteID(uint8_t value, uint8_t segment_index = 0);

  void CommandSet_Flasher_FunctionID(uint8_t value, uint8_t segment_index = 0);
  int8_t GetFlasherFunctionIDbyName(const char* f);
  const char* GetFlasherFunctionName(char* buffer, uint8_t buflen, uint8_t segment_index = 0);
  const char* GetFlasherFunctionNamebyID(uint8_t id, char* buffer, uint8_t buflen);

  void CommandSet_Flasher_UpdateColourRegion_RefreshSecs(uint8_t value, uint8_t segment_index = 0);

  void CommandSet_HardwareColourOrderTypeByStr(const char* value, uint8_t segment_index = 0);
  void CommandSet_ColourTypeByStr(const char* value, uint8_t segment_index = 0);
  // const char* GetHardwareColourTypeName(char* buffer, uint8_t buflen, uint8_t segment_index= 0);
  // const char* GetHardwareColourTypeNameByID(uint8_t id, char* buffer, uint8_t buflen, uint8_t segment_index= 0);

  void CommandSet_Animation_Transition_Time_Ms(uint16_t value, uint8_t segment_index= 0);
  void CommandSet_Animation_Transition_Rate_Ms(uint16_t value, uint8_t segment_index= 0);

  
  void CommandSet_LightsCountToUpdateAsNumber(uint16_t value, uint8_t segment_index = 0);
  void CommandSet_LightsCountToUpdateAsPercentage(uint8_t value, uint8_t segment_index = 0);
  uint16_t GetPixelsToUpdateAsNumberFromPercentage(uint8_t percentage, uint8_t segment_index = 0);
  uint8_t  GetPixelsToUpdateAsPercentageFromNumber(uint16_t number, uint8_t segment_index = 0);

  
  void CommandSet_Effect_Intensity(uint8_t value, uint8_t segment_index = 0);
  void CommandSet_Effect_Speed(uint8_t value, uint8_t segment_index = 0);
  

  /**
   * rgbcctcontroller commands
   */

  // void CommandSet_ActiveRgbcctColourPaletteIDUsedAsScene(uint8_t palette_id, uint8_t segment_index = 0); //remove
  void CommandSet_SegColour_RgbcctColour_Hue_360(uint16_t hue_new, uint8_t colour_index = 0, uint8_t segment_index = 0);
  void CommandSet_SegColour_RgbcctColour_Sat_255(uint8_t sat_new , uint8_t colour_index = 0, uint8_t segment_index = 0);
  void CommandSet_SegColour_RgbcctColour_ColourTemp_Kelvin(uint16_t ct, uint8_t colour_index = 0, uint8_t segment_index = 0);
  void CommandSet_SegColour_RgbcctColour_LightSubType(uint8_t subtype, uint8_t colour_index = 0, uint8_t segment_index = 0);
  void CommandSet_SegColour_RgbcctColour_BrightnessRGB(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);
  void CommandSet_SegColour_RgbcctColour_BrightnessCCT(uint8_t brightness, uint8_t colour_index = 0, uint8_t segment_index = 0);
  void CommandSet_SegColour_RgbcctColour_Manual(uint8_t* values, uint8_t value_count, uint8_t colour_index = 0, uint8_t segment_index = 0);



  void CommandSet_Global_BrtRGB_255(uint8_t bri, uint8_t segment_index = 0);
  void CommandSet_Global_BrtCCT_255(uint8_t bri, uint8_t segment_index = 0);

  /***************
   * END
   * 
   * Command List 
   * 
   * *********************/

  void setValuesFromMainSeg();
  void resetTimebase();
  void toggleOnOff();
  void setAllLeds();
  void setLedsStandard(bool justColors = false);
  bool colorChanged();
  void colorUpdated(int callMode);
  void updateInterfaces(uint8_t callMode);
  void handleTransitions();

  void colorFromUint32(uint32_t in, bool secondary = false);
  void colorFromUint24(uint32_t in, bool secondary = false);
  void relativeChangeWhite(int8_t amount, byte lowerBoundary = 0);
  void colorHStoRGB(uint16_t hue, byte sat, byte* rgb); //hue, sat to rgb
  void colorCTtoRGB(uint16_t mired, byte* rgb); //white spectrum to rgb

  void colorFromDecOrHexString(byte* rgb, char* in);
  void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)

  float minf2(float v, float w);
  float maxf2(float v, float w);




  #define DEBUG_LINE_HERE    Serial.printf("DEBUG HERE: ");\
                        Serial.print(__FILE__);\
                        Serial.println(__LINE__);\
                        Serial.flush();


void Segment_AppendNew(uint16_t start_pixel, uint16_t stop_pixel, uint8_t seg_index = 0);


void SetSegment_AnimFunctionCallback_WithoutAnimator(uint8_t seg_i = 0);

/**
* Segment, 72 bytes
**/
typedef struct Segment_New {
  public:
    struct PIXEL_INDEX_RANGE{
        uint16_t start = 0; // start means first led index within segment : start index / start X coordinate 2D (left)
        uint16_t stop  = 0; // stop means total leds within the segment (not the index of last pixel) : stop index / stop X coordinate 2D (right); segment is invalid if stop == 0
    }pixel_range;
    uint16_t offset = 0;
    uint8_t  _speed = DEFAULT_SPEED; // rate/blend time
    uint8_t  _intensity = 127;
    /**
     * @brief Note with union here not having a name, all options are accesible directly in Segment_New
     **/
    union {
      uint16_t options; //bit pattern: msb first: [transposed mirrorY reverseY] transitional (tbd) paused needspixelstate mirrored on reverse selected
      struct {
        bool    selected    : 1;  //     0 : selected
        bool    reverse     : 1;  //     1 : reversed
        bool    on          : 1;  //     2 : is On
        bool    mirror      : 1;  //     3 : mirrored
        bool    freeze      : 1;  //     4 : paused/frozen
        bool    reset       : 1;  //     5 : indicates that Segment runtime requires reset
        bool    transitional: 1;  //     6 : transitional (there is transition occuring)
        bool    reverse_y   : 1;  //     7 : reversed Y (2D)
        bool    mirror_y    : 1;  //     8 : mirrored Y (2D)
        bool    transpose   : 1;  //     9 : transposed (2D, swapped X & Y)
        uint8_t map1D2D     : 3;  // 10-12 : mapping for 1D effect on 2D (0-use as strip, 1-expand vertically, 2-circular/arc, 3-rectangular/corner, ...)
        uint8_t soundSim    : 3;  // 13-15 : 0-7 sound simulation types
      };
    };

    /**
     * @brief Animation ColourType of the minimal output. Full Rgbcct is calculated anyway and then output bus type dictates this, so remove? 
     * Although, it is used for saving of the buffer
     * Rename this one to "AnimationColourType" "animation_colour_type"
     * @NOTE: minimal/default should be RGB, only when white channels are needed should that also be computed
     **/
    RgbcctColor::ColourType colour_type = RgbcctColor::ColourType::COLOUR_TYPE__RGB__ID; 

    /**
     * @brief Stores at least 5 full RgbcctColours with all internal manipulations as needed
     * @NOTE: Replaces WLED 3 colour options
     **/
    #ifdef ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
    // std::vector<RgbcctColor> rgbcctcolors = {RgbcctColor(0), RgbcctColor(0), RgbcctColor(0), RgbcctColor(0), RgbcctColor(0)};// = RgbcctColor();
    #define RGBCCTCOLOURS_SIZE 5
    RgbcctColor rgbcctcolors[5] = {RgbcctColor(0), RgbcctColor(0), RgbcctColor(0), RgbcctColor(0), RgbcctColor(0)};// = RgbcctColor();
    #else
    std::vector<RgbcctColor> rgbcctcolors; // memory leak as size/memcpy is used with new segments. This needs to be predefined, so maybe move back to array?
    #endif

    void set_colors(uint8_t index, uint8_t r, uint8_t g, uint8_t b, uint8_t w )
    {
      uint8_t index_t = index<RGBCCTCOLOURS_SIZE?index:0;
      if(index>RGBCCTCOLOURS_SIZE){ Serial.println("ERROR"); }
      rgbcctcolors[index_t] = RgbcctColor(r,g,b,w,w);
    }

    /**
     * values that are used as global iters rather than passing between each function
     * likely phase out?
     * */
    struct segment_shared_iters
    {
      uint8_t index_palette_last = 99; //mine is longer
    }segment_iters;

    ANIMATION_FLAGS flags;
    /**
     * Effects (Scenes & Flasher), Ambilight, Adalight
     * */
    uint8_t animation_mode_id = ANIMATION_MODE__EFFECTS; // rename to "effect_id"

    
    uint16_t get_transition_rate_ms() // Effect that require call for every update, must be called at FRAMETIME_MS, otherwise, can manually be set
    {
      #ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
      if(effect_id >= EFFECTS_FUNCTION__WLED_STATIC__ID)
      {
        return FRAMETIME_MS;
      }
      else
        return transition.rate_ms;
      #endif
        return transition.rate_ms;
    };

    /**
     * Palette (Options on getting from palette)
     * */
    struct PALETTE{
      uint16_t id = 0; 
      /**
       * Index range, ie with 50%, for palette with 10 colours will only use first 5 colours
       * Value '0': is unused
       * */
      uint8_t index_range_max_limit = 0; 
      uint8_t index_range_as_percentage = 0;
      /**
       * @brief Temporary solution to store palette info "loaded" here
       * 
       */
      // struct LOADED{
      //   uint8_t* buffer = nullptr;
      //   uint16_t buflen = 0;
      //   uint8_t buffer_static[200]; //tmp fixed buffer
      // }loaded;
    }palette;
    /**
     * This allows the segment to have optional brightness per segment, by default is set to maximum
     * and will therefore have no effect 
     **/
    uint8_t brightness_optional = 255;

    TRANSITION_SETTINGS transition;

    // Flags and states that are used during one transition and reset when completed
    struct ANIMATION_SINGLE_USE_OVERRIDES_ANYTIME
    {
      // uint8_t fRefreshAllPixels = false;
      /**
       * Can't be zero, as that means not active
       * */
      uint16_t time_ms = 1000; //on boot
      // uint16_t rate_ms = 1000;
    }single_animation_override; // ie "oneshot" variables that get checked and executed one time only
    struct ANIMATION_SINGLE_USE_OVERRIDES_TURNING_OFF
    {
      // uint8_t fRefreshAllPixels = false;
      /**
       * Can't be zero, as that means not active
       * */
      uint16_t time_ms = 0; //on boot
      // uint16_t rate_ms = 1000;
    }single_animation_override_turning_off; // ie "oneshot" variables that get checked and executed one time only
    
    uint8_t speed(){ return _speed; }
    void set_speed(uint8_t v){ _speed = v; }

    uint8_t intensity(){ return _intensity; }
    void set_intensity(uint8_t v){ _intensity = v; }

    uint8_t get_grouping(){ return _speed; }
    void set_grouping(uint8_t v){ _speed = v; }

    uint16_t groupLength() { return grouping + spacing; }
    uint16_t virtualLength()
    {
      uint16_t groupLen = groupLength();
      uint16_t vLength = (length() + groupLen - 1) / groupLen;
      if (options & MIRROR)
        vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED
      return vLength;
    }

    uint8_t  grouping = 1;
    uint8_t  spacing = 0;
    uint8_t opacity = 255; // ie opacity is the segment "brightness"
    uint8_t seg_brightness = 255; // this will be merged with overall brightness to allow per segment brightness value

    uint8_t getBrightnessRGB();
    uint8_t getBrightnessCCT();

    uint8_t effect_id = 0;
    /**
     * @brief Allow a second effect to be loaded at the same time, this will enable a single effect to show just once, and then the id_next will be loaded into the primary position
     * e.g. For rotating effect, preload the initial animation and then rotate it
     * 
     */
    uint8_t effect_id_next = 0;  

    #ifdef ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    uint8_t  cct;                 //0==1900K, 255==10091K
    uint8_t  custom1, custom2;    // custom FX parameters/sliders
    struct {
      uint8_t custom3 : 5;        // reduced range slider (0-31)
      bool    check1  : 1;        // checkmark 1
      bool    check2  : 1;        // checkmark 2
      bool    check3  : 1;        // checkmark 3
    };
    #endif // ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    uint8_t startY;  // start Y coodrinate 2D (top); there should be no more than 255 rows
    uint8_t stopY;   // stop Y coordinate 2D (bottom); there should be no more than 255 rows
    char *name; // Keep, segment name to be added later by me

    // runtime data
    unsigned long next_time;  // millis() of next update
    uint32_t step;  // custom "step" var
    uint32_t call;  // call counter

    /**
     * @brief AUX options going forward must only be internal effect save states, and NOT user defined options (these should be "effect_option")
     * Internal multi-use variables
     */
    // INTERNAL_MULTIUSE_PARAMETERS
    uint16_t aux0 = 0;  // custom var
    uint16_t aux1 = 0;  // custom var
    uint16_t aux2 = 0;
    uint16_t aux3 = 0; // Also used for random CRGBPALETTE16 timing

    /**
     * @brief 
     * External user controllable multi-use variables
     */
    struct USER_PARAMETER_OPTIONS
    {
      uint16_t val0 = 0;
      uint16_t val1 = 0;
      uint16_t val2 = 0;
      uint16_t val3 = 0;
    }user_params;


    byte* data;     // effect data pointer
    CRGB* leds;     // local leds[] array (may be a pointer to global)
    static CRGB *_globalLeds;             // global leds[] array
    static uint16_t maxWidth, maxHeight;  // these define matrix width & height (max. segment dimensions)
    
  // private:
    union {
      uint8_t  _capabilities;
      struct {
        bool    _isRGB    : 1;
        bool    _hasW     : 1;
        bool    _isCCT    : 1;
        bool    _manualW  : 1;
        uint8_t _reserved : 4;
      };
    };
    uint16_t _dataLen;
    static uint16_t _usedSegmentData;

    uint16_t DataLength(){ return _dataLen; };
    byte* Data(){ return data; };


    #ifdef ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS
    // transition data, valid only if transitional==true, holds values during transition
    struct Transition {
      uint32_t      _colorT[NUM_COLORS];
      uint8_t       _briT;        // temporary brightness
      uint8_t       _cctT;        // temporary CCT
      CRGBPalette16 _palT;        // temporary palette
      uint8_t       _prevPaletteBlends; // number of previous palette blends (there are max 255 belnds possible)
      uint8_t       _modeP;       // previous mode/effect
      //uint16_t      _aux0, _aux1; // previous mode/effect runtime data
      //uint32_t      _step, _call; // previous mode/effect runtime data
      //byte         *_data;        // previous mode/effect runtime data
      uint32_t      _start;
      uint16_t      _dur;
      Transition(uint16_t dur=750)
        : _briT(255)
        , _cctT(127)
        , _palT(CRGBPalette16(HTMLColorCode::Black))
        , _prevPaletteBlends(0)
        , _modeP(EFFECTS_FUNCTION__STATIC_PALETTE__ID)
        , _start(millis())
        , _dur(dur)
      {}
      Transition(uint16_t d, uint8_t b, uint8_t c, const uint32_t *o)
        : _briT(b)
        , _cctT(c)
        , _palT(CRGBPalette16(HTMLColorCode::Black))
        , _prevPaletteBlends(0)
        , _modeP(EFFECTS_FUNCTION__STATIC_PALETTE__ID)
        , _start(millis())
        , _dur(d)
      {
        for (size_t i=0; i<NUM_COLORS; i++) _colorT[i] = o[i];
      }
    } *_t;
    #endif // ENABLE_DEVFEATURE_UNNEEDED_WLED_ONLY_PARAMETERS


      /**
       * Each segment will have its own animator
       * This will also need to share its index into the animation so it knows what segments to run
       * */
      NeoPixelAnimator* animator = new NeoPixelAnimator(1, NEO_MILLISECONDS); //one animator for each segment, which is only init when needed or else delete

      bool animation_has_anim_callback = false; //should be dafult on start but causing no animation on start right now
      
      mPaletteContainer* palette_container = nullptr;
      // mPaletteContainer* palette_container = new mPaletteContainer(0);

      uint32_t tSaved_LastUpdated = millis();
      uint32_t tTick_maximum_call_ms = 10;

      /**
       * Using "index" inside animator as segment index
       * */
      ANIM_FUNCTION_SIGNATURE;
      

      uint32_t tSaved_AnimateRunTime = millis();













  // public:

    Segment_New(uint16_t sStart=0, uint16_t sStop=30) :
      // start(sStart),
      // stop(sStop),
      offset(0),
      _speed(DEFAULT_SPEED),
      _intensity(DEFAULT_INTENSITY),
      effect_id(DEFAULT_MODE),
      options(SELECTED | SEGMENT_ON),
      grouping(1),
      spacing(0),
      opacity(255),
      // colors{DEFAULT_COLOR,BLACK,BLACK},
      cct(127),
      custom1(DEFAULT_C1),
      custom2(DEFAULT_C2),
      custom3(DEFAULT_C3),
      check1(false),
      check2(false),
      check3(false),
      startY(0),
      stopY(1),
      name(nullptr),
      next_time(0),
      step(0),
      call(0),
      aux0(0),
      aux1(0),
      data(nullptr),
      leds(nullptr),
      _capabilities(0),
      _dataLen(0),
      _t(nullptr)
    {
      pixel_range.start = sStart;
      pixel_range.stop = sStop;
      refreshLightCapabilities();
      
      // // Init
      // palette_container = new mPaletteContainer(0);//nullptr;

      #ifdef ENABLE_DEVFEATURE_INTERNALISE_PALETTE_CONTAINER_TO_SEGMENT_NEW
      palette_container = new mPaletteContainer(); 
      #endif // ENABLE_DEVFEATURE_INTERNALISE_PALETTE_CONTAINER_TO_SEGMENT_NEW

      DEBUG_LINE_HERE;

      // if(RGBCCTCOLOURS_SIZE != 5) //minimum as default
      // {
      //   rgbcctcolors.clear();
      //   rgbcctcolors.push_back(RgbcctColor(255,0,0,1,2));
      //   rgbcctcolors.push_back(RgbcctColor(0,255,0,3,4));
      //   rgbcctcolors.push_back(RgbcctColor(0,0,255,5,6));
      //   rgbcctcolors.push_back(RgbcctColor(255,0,255,7,8));
      //   rgbcctcolors.push_back(RgbcctColor(255,255,0,9,10));
      // }
      

    }

    Segment_New(uint16_t sStartX, uint16_t sStopX, uint16_t sStartY, uint16_t sStopY) : Segment_New(sStartX, sStopX) {
      startY = sStartY;
      stopY  = sStopY;
    }

    Segment_New(const Segment_New &orig) // copy constructor
     {
      // DEBUG_LINE_HERE;
      Serial.println(F("-- Copy segment constructor --"));
      

      // Serial.println(F("-- Copy segment constructor -- palette_container = new mPaletteContainer(0);"));
      // palette_container = new mPaletteContainer(0);

      memcpy(this, &orig, sizeof(Segment_New));
      name = nullptr;
      data = nullptr;
      _dataLen = 0;
      _t = nullptr;
      if (leds && !Segment_New::_globalLeds) leds = nullptr;
      if (orig.name) { name = new char[strlen(orig.name)+1]; if (name) strcpy(name, orig.name); }
      if (orig.data) { if (allocateData(orig._dataLen)) memcpy(data, orig.data, orig._dataLen); }
      if (orig._t)   { _t = new Transition(orig._t->_dur, orig._t->_briT, orig._t->_cctT, orig._t->_colorT); }
      if (orig.leds && !Segment_New::_globalLeds) { leds = (CRGB*)malloc(sizeof(CRGB)*length()); if (leds) memcpy(leds, orig.leds, sizeof(CRGB)*length()); }
    
      // DEBUG_LINE_HERE;
  
    };



    Segment_New(Segment_New &&orig) noexcept // move constructor
    {
      // DEBUG_LINE_HERE;
      Serial.println(F("-- Move segment constructor --"));
      memcpy(this, &orig, sizeof(mAnimatorLight::Segment_New));
      orig.name = nullptr;
      orig.data = nullptr;
      orig._dataLen = 0;
      orig._t   = nullptr;
      orig.leds = nullptr;
      // DEBUG_LINE_HERE;
    }


    ~Segment_New() {

      // DEBUG_LINE_HERE;
      #ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
      Serial.print(F("Destroying segment:"));
      if (name) Serial.printf(" %s (%p)", name, name);
      if (data) Serial.printf(" %d (%p)", (int)_dataLen, data);
      if (leds) Serial.printf(" [%u]", length()*sizeof(CRGB));
      Serial.println();
      #endif
      if (!Segment_New::_globalLeds && leds) free(leds);
      if (name) delete[] name;
      if (_t) delete _t;
      // DEBUG_LINE_HERE;
      deallocateData();
      // DEBUG_LINE_HERE;
    }

    Segment_New& operator= (const Segment_New &orig); // copy assignment
    Segment_New& operator= (Segment_New &&orig) noexcept; // move assignment

    #ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
    size_t getSize() const { return sizeof(Segment_New) + (data?_dataLen:0) + 
    (name?strlen(name):0) + 
    (_t?sizeof(Transition):0) + (!Segment_New::_globalLeds && leds?sizeof(CRGB)*length():0); }
    #endif

    inline bool     getOption(uint8_t n) const { return ((options >> n) & 0x01); }
    inline bool     isSelected(void)     const { return selected; }
    inline bool     isActive(void)       const { return pixel_range.stop > pixel_range.start; }
    inline bool     is2D(void)           const { return (width()>1 && height()>1); }
    inline uint16_t width(void)          const { return pixel_range.stop - pixel_range.start; }       // segment width in physical pixels (length if 1D)
    inline uint16_t height(void)         const { return stopY - startY; }     // segment height (if 2D) in physical pixels
    inline uint16_t length(void)         const { return width() * height(); } // segment length (count) in physical pixels
    inline uint16_t groupLength(void)    const { return grouping + spacing; }
    inline uint8_t  getLightCapabilities(void) const { return _capabilities; }

    static uint16_t getUsedSegmentData(void)    { return _usedSegmentData; }
    static void     addUsedSegmentData(int len) { _usedSegmentData += len; }

    bool    setColor(uint8_t slot, uint32_t c); //returns true if changed
    void    setCCT(uint16_t k);
    void    setOpacity(uint8_t o);
    void    setOption(uint8_t n, bool val);
    void    setMode(uint8_t fx, bool loadDefaults = false);
    void    setPalette(uint8_t pal);
    uint8_t differs(Segment_New& b) const;
    void    refreshLightCapabilities(void);

    // runtime data functions
    inline uint16_t dataSize(void) const { return _dataLen; }
    bool allocateData(size_t len);
    void deallocateData(void);
    void resetIfRequired(void);
    /** 
      * Flags that before the next effect is calculated,
      * the internal segment state should be reset. 
      * Call resetIfRequired before calling the next effect function.
      * Safe to call from interrupts and network requests.
      */
    inline void markForReset(void) 
    {
      // Serial.println(DEBUG_INSERT_PAGE_BREAK "mAnimatorLight::Segment_New::markForReset()"); delay(5000);      
      reset = true; 
    }  // setOption(SEG_OPTION_RESET, true)
    void setUpLeds(void);   // set up leds[] array for loseless getPixelColor()

    // transition functions
    void     startTransition(uint16_t dur); // transition has to start before actual segment values change
    void     handleTransition(void);
    uint16_t progress(void); //transition progression between 0-65535
    uint8_t  currentBri(uint8_t briNew, bool useCct = false);
    uint8_t  currentMode(uint8_t modeNew);
    uint32_t currentColor(uint8_t slot, uint32_t colorNew);
    CRGBPalette16 &loadPalette(CRGBPalette16 &tgt, uint8_t pal);
    CRGBPalette16 &currentPalette(CRGBPalette16 &tgt, uint8_t paletteID);

    // 1D strip
    uint16_t virtualLength(void) const;
    void setPixelColor(int n, uint32_t c); // set relative pixel within segment with color
    void setPixelColor(int n, byte r, byte g, byte b, byte w = 0) { setPixelColor(n, RGBW32(r,g,b,w)); } // automatically inline
    void setPixelColor(int n, CRGB c)                             { setPixelColor(n, RGBW32(c.r,c.g,c.b,0)); } // automatically inline
    void setPixelColor(float i, uint32_t c, bool aa = true);
    void setPixelColor(float i, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0, bool aa = true) { setPixelColor(i, RGBW32(r,g,b,w), aa); }
    void setPixelColor(float i, CRGB c, bool aa = true)                                         { setPixelColor(i, RGBW32(c.r,c.g,c.b,0), aa); }
    uint32_t getPixelColor(int i);
    
    
    void SetPixelColor(uint16_t indexPixel, RgbcctColor color, bool brightness_needs_applied = false);// uint16_t segment_length = 0);
    RgbcctColor GetPixelColor(uint16_t indexPixel = 0);       
    void SetPixelColor(uint16_t indexPixel, uint8_t red, uint8_t green, uint8_t blue, bool brightness_needs_applied = false);  
    void SetPixelColor(uint16_t indexPixel, uint32_t color, bool brightness_needs_applied = false);//, uint16_t segment_length = 0);
    void SetPixelColor_All(RgbcctColor colour);
    
    

    // 1D support functions (some implement 2D as well)
    void blur(uint8_t);
    void fill(uint32_t c);
    void fade_out(uint8_t r);
    void fadeToBlackBy(uint8_t fadeBy);
    void blendPixelColor(int n, uint32_t color, uint8_t blend);
    void blendPixelColor(int n, CRGB c, uint8_t blend)            { blendPixelColor(n, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColor(int n, uint32_t color);
    void addPixelColor(int n, byte r, byte g, byte b, byte w = 0) { addPixelColor(n, RGBW32(r,g,b,w)); } // automatically inline
    void addPixelColor(int n, CRGB c)                             { addPixelColor(n, RGBW32(c.r,c.g,c.b,0)); } // automatically inline
    void fadePixelColor(uint16_t n, uint8_t fade);


    uint8_t get_random_wheel_index(uint8_t pos);
    uint32_t color_from_palette(uint16_t, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255);
    uint32_t color_wheel(uint8_t pos);

    /**
     * @brief Adding Palette Get functions inside segment so it can automatically load buffers and set current palette_id
     * 
     */
    RgbcctColor 
    #ifdef ENABLE_DEVFEATURE_LIGHTING_PALETTE_IRAM
    IRAM_ATTR 
    #endif 
    GetColourFromPalette(
      uint16_t desired_index_from_palette = 0,
      uint8_t* encoded_index = nullptr,
      bool     flag_map_scaling = true, // true(default):"desired_index_from_palette is exact pixel index", false:"desired_index_from_palette is scaled between 0 to 255, where (127/155 would be the center pixel)"
      bool     flag_wrap = true,        // true(default):"hard edge for wrapping wround, so last to first pixel (wrap) is blended", false: "hard edge, palette resets without blend on last/first pixels"
      uint8_t mcol = 0, // will be phased out
      bool     flag_convert_pixel_index_to_get_exact_crgb_colour = false,   // added by me, to make my effects work with CRGBPalette16
      uint8_t  brightness_scale = 255//, //255(default): No scaling, 0-255 scales the brightness of returned colour (remember all colours are saved in full 255 scale)
      // uint8_t* discrete_colours_in_palette = nullptr
    );




    // 2D matrix
    uint16_t virtualWidth(void)  const;
    uint16_t virtualHeight(void) const;
    uint16_t nrOfVStrips(void) const;
  #ifndef WLED_DISABLE_2D
    uint16_t XY(uint16_t x, uint16_t y); // support function to get relative index within segment (for leds[])
    void setPixelColorXY(int x, int y, uint32_t c); // set relative pixel within segment with color
    void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColorXY(x, y, RGBW32(r,g,b,w)); } // automatically inline
    void setPixelColorXY(int x, int y, CRGB c)                             { setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); } // automatically inline
    void setPixelColorXY(float x, float y, uint32_t c, bool aa = true);
    void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true) { setPixelColorXY(x, y, RGBW32(r,g,b,w), aa); }
    void setPixelColorXY(float x, float y, CRGB c, bool aa = true)                             { setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), aa); }
    uint32_t getPixelColorXY(uint16_t x, uint16_t y);
    // 2D support functions
    void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t color, uint8_t blend);
    void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend)  { blendPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColorXY(int x, int y, uint32_t color);
    void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { addPixelColorXY(x, y, RGBW32(r,g,b,w)); } // automatically inline
    void addPixelColorXY(int x, int y, CRGB c)                             { addPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); }
    void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade);
    void box_blur(uint16_t i, bool vertical, fract8 blur_amount); // 1D box blur (with weight)
    void blurRow(uint16_t row, fract8 blur_amount);
    void blurCol(uint16_t col, fract8 blur_amount);
    void moveX(int8_t delta);
    void moveY(int8_t delta);
    void move(uint8_t dir, uint8_t delta);
    void fill_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c);
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c) { drawLine(x0, y0, x1, y1, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color);
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB c) { drawCharacter(chr, x, y, w, h, RGBW32(c.r,c.g,c.b,0)); } // automatic inline
    void wu_pixel(uint32_t x, uint32_t y, CRGB c);
    void blur1d(fract8 blur_amount); // blur all rows in 1 dimension
    void blur2d(fract8 blur_amount) { blur(blur_amount); }
    void fill_solid(CRGB c) { fill(RGBW32(c.r,c.g,c.b,0)); }
    void nscale8(uint8_t scale);
  #else
    uint16_t XY(uint16_t x, uint16_t y)                                    { return x; }
    void setPixelColorXY(int x, int y, uint32_t c)                         { setPixelColor(x, c); }
    void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColor(x, RGBW32(r,g,b,w)); }
    void setPixelColorXY(int x, int y, CRGB c)                             { setPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    void setPixelColorXY(float x, float y, uint32_t c, bool aa = true)     { setPixelColor(x, c, aa); }
    void setPixelColorXY(float x, float y, byte r, byte g, byte b, byte w = 0, bool aa = true) { setPixelColor(x, RGBW32(r,g,b,w), aa); }
    void setPixelColorXY(float x, float y, CRGB c, bool aa = true)         { setPixelColor(x, RGBW32(c.r,c.g,c.b,0), aa); }
    uint32_t getPixelColorXY(uint16_t x, uint16_t y)                       { return getPixelColor(x); }
    void blendPixelColorXY(uint16_t x, uint16_t y, uint32_t c, uint8_t blend) { blendPixelColor(x, c, blend); }
    void blendPixelColorXY(uint16_t x, uint16_t y, CRGB c, uint8_t blend)  { blendPixelColor(x, RGBW32(c.r,c.g,c.b,0), blend); }
    void addPixelColorXY(int x, int y, uint32_t color)                     { addPixelColor(x, color); }
    void addPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { addPixelColor(x, RGBW32(r,g,b,w)); }
    void addPixelColorXY(int x, int y, CRGB c)                             { addPixelColor(x, RGBW32(c.r,c.g,c.b,0)); }
    void fadePixelColorXY(uint16_t x, uint16_t y, uint8_t fade)            { fadePixelColor(x, fade); }
    void box_blur(uint16_t i, bool vertical, fract8 blur_amount) {}
    void blurRow(uint16_t row, fract8 blur_amount) {}
    void blurCol(uint16_t col, fract8 blur_amount) {}
    void moveX(int8_t delta) {}
    void moveY(int8_t delta) {}
    void move(uint8_t dir, uint8_t delta) {}
    void fill_circle(uint16_t cx, uint16_t cy, uint8_t radius, CRGB c) {}
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint32_t c) {}
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, CRGB c) {}
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, uint32_t color) {}
    void drawCharacter(unsigned char chr, int16_t x, int16_t y, uint8_t w, uint8_t h, CRGB color) {}
    void wu_pixel(uint32_t x, uint32_t y, CRGB c) {}
  #endif
} segment_new;
//static int segSize = sizeof(Segment_New);


const char* GetPaletteNameByID(uint16_t palette_id, char* buffer = nullptr, uint8_t buflen = 0);


RgbcctColor ColourBlend(RgbcctColor color1, RgbcctColor color2, uint8_t blend);

/**
 * @brief Temporary use the WS2812FX class, but work towards removing it as the mAnimatorLight class is already the class
 * 
 */

// // main "strip" class
// class WS2812FX {  // 96 bytes

  typedef uint16_t (*mode_ptr)(void); // pointer to mode function
  typedef void (*show_callback)(void); // pre show callback
  typedef struct ModeData {
    uint8_t     _id;   // mode (effect) id
    mode_ptr    _fcn;  // mode (effect) function
    const char *_data; // mode (effect) name and its UI control data
    ModeData(uint8_t id, uint16_t (*fcn)(void), const char *data) : _id(id), _fcn(fcn), _data(data) {}
  } mode_data_t;

//   static WS2812FX* instance;
//   public:

//     WS2812FX() :
//       paletteFade(0),
//       paletteBlend(0),
//       milliampsPerLed(55),
//       cctBlending(0),
//       ablMilliampsMax(0),//ABL_MILLIAMPS_DEFAULT),
//       currentMilliamps(0),
//       now(millis()),
//       timebase(0),
//       isMatrix(false),
//       #ifndef WLED_DISABLE_2D
//       hPanels(1),
//       vPanels(1),
//       panelH(8),
//       panelW(8),
//       matrix{0,0,0,0},
//       panel{{0,0,0,0}},
//       #endif
//       // semi-private (just obscured) used in effect functions through macros
//       _currentPalette(CRGBPalette16(HTMLColorCode::Black)),
//       _colors_t{0,0,0},
//       _virtualSegmentLength(0),
//       // true private variables
//       _length(DEFAULT_LED_COUNT),
//       _brightness(DEFAULT_BRIGHTNESS),
//       _transitionDur(750),
//       _targetFps(WLED_FPS),
//       _frametime(FRAMETIME_FIXED),
//       _cumulativeFps(2),
//       _isServicing(false),
//       _isOffRefreshRequired(false),
//       _hasWhiteChannel(false),
//       _triggered(false),
//       _modeCount(EFFECTS_FUNCTION__LENGTH__ID),
//       _callback(nullptr),
//       customMappingTable(nullptr),
//       customMappingSize(0),
//       _lastShow(0),
//       _segment_index_primary(0),
//       _mainSegment(0)
//     {
//       WS2812FX::instance = this;
//       _mode.reserve(_modeCount);     // allocate memory to prevent initial fragmentation (does not increase size())
//       _modeData.reserve(_modeCount); // allocate memory to prevent initial fragmentation (does not increase size())
//       if (_mode.capacity() <= 1 || _modeData.capacity() <= 1) _modeCount = 1; // memory allocation failed only show Solid
//       else setupEffectData();
//     }

//     ~WS2812FX() {
//       if (customMappingTable) delete[] customMappingTable;
//       _mode.clear();
//       _modeData.clear();
//       segments.clear();
//       customPalettes.clear();
//       if (useLedsArray && Segment_New::_globalLeds) free(Segment_New::_globalLeds);
//     }

//     static WS2812FX* getInstance(void) { return instance; }


    void
#ifdef ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES
      printSize(),
#endif
      finalizeInit(),
      service(void),
      setMode(uint8_t segid, uint8_t m),
      setColor(uint8_t slot, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0),
      setColor(uint8_t slot, uint32_t c),
      setCCT(uint16_t k),
      setBrightness(uint8_t b, bool direct = false),
      setRange(uint16_t i, uint16_t i2, uint32_t col),
      setTransitionMode(bool t),
      purgeSegments(bool force = false),
      setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t grouping = 1, uint8_t spacing = 0, uint16_t offset = UINT16_MAX, uint16_t startY=0, uint16_t stopY=1),
      setMainSegmentId(uint8_t n),
      restartRuntime(),
      resetSegments2(),
      makeAutoSegments(bool forceReset = false),
      fixInvalidSegments(),
      setPixelColor(int n, uint32_t c),
      show(void),
      setTargetFps(uint8_t fps),
      deserializeMap(uint8_t n=0);

    // using public variables to reduce code size increase due to inline function getSegment() (with bounds checking)
    // and color transitions
    uint32_t _colors_t[3]; // color used for effect (includes transition)
    uint16_t _virtualSegmentLength;

    std::vector<segment_new> segments;
    friend class Segment_New;

  // private:
    uint16_t _length;
    uint8_t  _brightness;
    uint16_t _transitionDur;

    uint8_t  _targetFps;
    uint16_t _frametime;
    uint16_t _cumulativeFps;

    // will require only 1 byte
    struct {
      bool _isServicing          : 1;
      bool _isOffRefreshRequired : 1; //periodic refresh is required for the strip to remain off.
      bool _hasWhiteChannel      : 1;
      bool _triggered            : 1;
    };

    uint8_t                  _modeCount;
    std::vector<mode_ptr>    _mode;     // SRAM footprint: 4 bytes per element
    std::vector<const char*> _modeData; // mode (effect) name and its slider control data array

    show_callback _callback;

    uint16_t* customMappingTable;
    uint16_t  customMappingSize;
    
    uint32_t _lastShow;
    
    uint8_t _segment_index_primary;
    uint8_t _mainSegment;



    void fill2(uint32_t c) { for (int i = 0; i < _length; i++) setPixelColor(i, c); } // fill whole strip with color (inline)
    void addEffect(uint8_t id, mode_ptr mode_fn, const char *mode_name); // add effect to the list; defined in FX.cpp
    void setupEffectData(void); // add default effects to the list; defined in FX.cpp

    // outsmart the compiler :) by correctly overloading
    inline void setPixelColor(int n, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0) { setPixelColor(n, RGBW32(r,g,b,w)); }
    inline void setPixelColor(int n, CRGB c) { setPixelColor(n, c.red, c.green, c.blue); }
    inline void trigger(void) { _triggered = true; } // Forces the next frame to be computed on all active segments.
    inline void setShowCallback(show_callback cb) { _callback = cb; }
    inline void setTransition(uint16_t t) { _transitionDur = t; }
    inline void appendSegment(const Segment_New &seg = Segment_New()) {

      DEBUG_LINE_HERE;
       segments.push_back(seg); 
       
       }

    bool
      checkSegmentAlignment(void),
      hasRGBWBus(void),
      hasCCTBus(void),
      // return true if the strip is being sent pixel updates
      isUpdating(void),
      useLedsArray = false;

    inline bool isServicing(void) { return _isServicing; }
    inline bool hasWhiteChannel(void) {return _hasWhiteChannel;}
    inline bool isOffRefreshRequired(void) {return _isOffRefreshRequired;}

    uint8_t
      paletteFade,
      paletteBlend,
      milliampsPerLed,
      cctBlending,
      getActiveSegmentsNum(void),
      getFirstSelectedSegId(void),
      getLastActiveSegmentId(void),
      setPixelSegment(uint8_t n);

    inline uint8_t getBrightness(void) { return _brightness; }
    inline uint8_t getMaxSegments(void) { return MAX_NUM_SEGMENTS; }  // returns maximum number of supported segments (fixed value)
    inline uint8_t getSegmentsNum(void) { return segments.size(); }  // returns currently present segments
    inline uint8_t getCurrSegmentId(void) { return _segment_index_primary; }
    inline uint8_t getMainSegmentId(void) { return _mainSegment; }
    inline uint8_t getPaletteCount() { return 13 + GRADIENT_PALETTE_COUNT; }  // will only return built-in palette count
    inline uint8_t getTargetFps() { return _targetFps; }
    inline uint8_t getModeCount() { return _modeCount; }

    uint16_t
      ablMilliampsMax,
      currentMilliamps,
      getLengthPhysical(void),
      getFps();

    inline uint16_t getFrameTime(void) { return _frametime; }
    inline uint16_t getMinShowDelay(void) { return MIN_SHOW_DELAY; }
    inline uint16_t getLengthTotal(void) { return _length; }
    inline uint16_t getTransition(void) { return _transitionDur; }

    uint32_t
      now,
      timebase,
      currentColor(uint32_t colorNew, uint8_t tNr),
      getPixelColor(uint16_t);

    inline uint32_t getLastShow(void) { return _lastShow; }
    inline uint32_t segColor(uint8_t i) { return _colors_t[i]; }

    const char *
      getModeData(uint8_t id = 0) { return (id && id<_modeCount) ? _modeData[id] : PSTR("Solid"); }

    const char **
      getModeDataSrc(void) { return &(_modeData[0]); } // vectors use arrays for underlying data

    Segment_New&        getSegment(uint8_t id);
    inline Segment_New& getFirstSelectedSeg(void) { return segments[getFirstSelectedSegId()]; }
    inline Segment_New& getMainSegment(void)      { return segments[getMainSegmentId()]; }
    inline Segment_New* getSegments(void)         { return &(segments[0]); }

  // 2D support (panels)
    bool
      isMatrix;

#ifndef WLED_DISABLE_2D
    #define WLED_MAX_PANELS 64
    uint8_t
      hPanels,
      vPanels;

    uint16_t
      panelH,
      panelW;

    typedef struct panel_bitfield_t {
      bool bottomStart : 1; // starts at bottom?
      bool rightStart  : 1; // starts on right?
      bool vertical    : 1; // is vertical?
      bool serpentine  : 1; // is serpSentine?
    } Panel;
    Panel
      matrix,
      panel[WLED_MAX_PANELS];
#endif

    void
      setUpMatrix(),
      setPixelColorXY(int x, int y, uint32_t c);

    // outsmart the compiler :) by correctly overloading
    inline void setPixelColorXY(int x, int y, byte r, byte g, byte b, byte w = 0) { setPixelColorXY(x, y, RGBW32(r,g,b,w)); } // automatically inline
    inline void setPixelColorXY(int x, int y, CRGB c)                             { setPixelColorXY(x, y, RGBW32(c.r,c.g,c.b,0)); }

    uint32_t
      getPixelColorXY(uint16_t, uint16_t);

  // end 2D support

    void loadCustomPalettes(void); // loads custom palettes from JSON
    CRGBPalette16 _currentPalette; // palette used for current effect (includes transition)
    std::vector<CRGBPalette16> customPalettes; // TODO: move custom palettes out of WS2812FX class


    void
      estimateCurrentAndLimitBri(void);
  





};

#endif

#endif
