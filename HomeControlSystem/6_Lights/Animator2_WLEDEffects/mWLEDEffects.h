#ifndef MWLEDEFFECTS_H
#define MWLEDEFFECTS_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_LIGHTS_WLED_ID 141

#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS

#include "1_TaskerManager/mTaskerManager.h"
// #include "6_Lights/WLEDEffects/mWLEDEffects_web.h"

// #include "6_Lights/WLEDEffects/wled.h"
#include "wled_palettes.h"

#include <NeoPixelAnimator.h>

#include "mWLEDEffects_progmem_defines.h"

//Notifier callMode 
#define NOTIFIER_CALL_MODE_INIT           0    //no updates on init, can be used to disable updates
#define NOTIFIER_CALL_MODE_DIRECT_CHANGE  1
#define NOTIFIER_CALL_MODE_BUTTON         2
#define NOTIFIER_CALL_MODE_NOTIFICATION   3
#define NOTIFIER_CALL_MODE_NIGHTLIGHT     4
#define NOTIFIER_CALL_MODE_NO_NOTIFY      5
#define NOTIFIER_CALL_MODE_FX_CHANGED     6    //no longer used
#define NOTIFIER_CALL_MODE_HUE            7
#define NOTIFIER_CALL_MODE_PRESET_CYCLE   8
#define NOTIFIER_CALL_MODE_BLYNK          9
#define NOTIFIER_CALL_MODE_ALEXA         10

//RGB to RGBW conversion mode
#define RGBW_MODE_MANUAL_ONLY     0            //No automatic white channel calculation. Manual white channel slider
#define RGBW_MODE_AUTO_BRIGHTER   1            //New algorithm. Adds as much white as the darkest RGBW channel
#define RGBW_MODE_AUTO_ACCURATE   2            //New algorithm. Adds as much white as the darkest RGBW channel and subtracts this amount from each RGB channel
#define RGBW_MODE_DUAL            3            //Manual slider + auto calculation. Automatically calculates only if manual slider is set to off (0)  
#define RGBW_MODE_LEGACY          4            //Old floating algorithm. Too slow for realtime and palette support

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

//E1.31 DMX modes
#define DMX_MODE_DISABLED         0            //not used
#define DMX_MODE_SINGLE_RGB       1            //all LEDs same RGB color (3 channels)
#define DMX_MODE_SINGLE_DRGB      2            //all LEDs same RGB color and master dimmer (4 channels)
#define DMX_MODE_EFFECT           3            //trigger standalone effects of WLED (11 channels)
#define DMX_MODE_MULTIPLE_RGB     4            //every LED is addressed with its own RGB (ledCount * 3 channels)
#define DMX_MODE_MULTIPLE_DRGB    5            //every LED is addressed with its own RGB and share a master dimmer (ledCount * 3 + 1 channels)

//Light capability byte (unused)
#define TYPE_NONE                 0            //light is not configured
#define TYPE_RESERVED             1            //unused. Might indicate a "virtual" light
#define TYPE_WS2812_RGB           2
#define TYPE_SK6812_RGBW          3
#define TYPE_WS2812_WWA           4            //amber + warm + cold white
#define TYPE_WS2801               5
#define TYPE_ANALOG_1CH           6            //single channel PWM. Uses value of brightest RGBW channel
#define TYPE_ANALOG_2CH           7            //analog WW + CW
#define TYPE_ANALOG_3CH           8            //analog RGB
#define TYPE_ANALOG_4CH           9            //analog RGBW
#define TYPE_ANALOG_5CH          10            //analog RGB + WW + CW
#define TYPE_APA102              11
#define TYPE_LPD8806             12

//Segment option byte bits
#define SEG_OPTION_SELECTED       0
#define SEG_OPTION_REVERSED       1
#define SEG_OPTION_ON             2
#define SEG_OPTION_MIRROR         3            //Indicates that the effect will be mirrored within the segment
#define SEG_OPTION_NONUNITY       4            //Indicates that the effect does not use FRAMETIME or needs getPixelColor
#define SEG_OPTION_TRANSITIONAL   7

// GLOBAL VARIABLES
// both declared and defined in header (solution from http://www.keil.com/support/docs/1868.htm)
//
//e.g. byte test = 2 becomes WLED_GLOBAL byte test _INIT(2);
//     int arr[]{0,1,2} becomes WLED_GLOBAL int arr[] _INIT_N(({0,1,2}));

#ifndef WLED_DEFINE_GLOBAL_VARS
# define WLED_GLOBAL extern
# define _INIT(x)
# define _INIT_N(x)
#else
# define WLED_GLOBAL
# define _INIT(x) = x
//needed to ignore commas in array definitions
#define UNPACK( ... ) __VA_ARGS__
# define _INIT_N(x) UNPACK x
#endif

//Segment option byte bits
#define SEG_OPTION_SELECTED       0
#define SEG_OPTION_REVERSED       1
#define SEG_OPTION_ON             2
#define SEG_OPTION_MIRROR         3            //Indicates that the effect will be mirrored within the segment
#define SEG_OPTION_NONUNITY       4            //Indicates that the effect does not use FRAMETIME or needs getPixelColor
#define SEG_OPTION_TRANSITIONAL   7

//RGB to RGBW conversion mode
#define RGBW_MODE_MANUAL_ONLY     0            //No automatic white channel calculation. Manual white channel slider
#define RGBW_MODE_AUTO_BRIGHTER   1            //New algorithm. Adds as much white as the darkest RGBW channel
#define RGBW_MODE_AUTO_ACCURATE   2            //New algorithm. Adds as much white as the darkest RGBW channel and subtracts this amount from each RGB channel
#define RGBW_MODE_DUAL            3            //Manual slider + auto calculation. Automatically calculates only if manual slider is set to off (0)  
#define RGBW_MODE_LEGACY          4            //Old floating algorithm. Too slow for realtime and palette support

#define ENABLE_ADVANCED_EFFECTS // ability to disable most for testing

#define IBN 5100
#define PALETTE_SOLID_WRAP (paletteBlend == 1 || paletteBlend == 3)

#include "NpbWrapper.h"

#define FASTLED_INTERNAL //remove annoying pragma messages
#include "FastLED_Modified/FastLED.h"

#define DEFAULT_BRIGHTNESS (uint8_t)127
#define DEFAULT_MODE       (uint8_t)0
#define DEFAULT_SPEED      (uint8_t)128
#define DEFAULT_COLOR      (uint32_t)0xFFAA00

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

/* Not used in all effects yet */
#define WLED_FPS         42
#define FRAMETIME        (1000/WLED_FPS)

/* each segment uses 52 bytes of SRAM memory, so if you're application fails because of
  insufficient memory, decreasing MAX_NUM_SEGMENTS may help */
#define MAX_NUM_SEGMENTS 1//10

/* How much data bytes all segments combined may allocate */
#ifdef ESP8266
#define MAX_SEGMENT_DATA 2048
#else
#define MAX_SEGMENT_DATA 8192
#endif

#define LED_SKIP_AMOUNT  1
#define MIN_SHOW_DELAY  15

#define NUM_COLORS       3 /* number of colors per segment */
// #define _segments[_segment_index]          _segments[_segment_index]
// #define SEGCOLOR(x)      SEGCOLOR()
// #define SEGENV           _segment_runtimes[_segment_index]
#define SEGCOLOR(x)      gamma32(_segments[_segment_index].colors[x])
#define SEGLEN           _virtualSegmentLength
// #define SEGACT           _segments[_segment_index].stop
#define SPEED_FORMULA_L  5 + (50*(255 - _segments[_segment_index].speed))/_virtualSegmentLength
#define RESET_RUNTIME    memset(_segment_runtimes, 0, sizeof(_segment_runtimes))

// some common colors
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
#define IS_TRANSITIONAL ((_segments[_segment_index].options & TRANSITIONAL) == TRANSITIONAL)
#define IS_MIRROR       ((_segments[_segment_index].options & MIRROR      ) == MIRROR      )
#define IS_SEGMENT_ON   ((_segments[_segment_index].options & SEGMENT_ON  ) == SEGMENT_ON  )
#define IS_REVERSE      ((_segments[_segment_index].options & REVERSE     ) == REVERSE     )
#define IS_SELECTED     ((_segments[_segment_index].options & SELECTED    ) == SELECTED    )


enum FX_MODES{
  // Static
  FX_MODE_STATIC=0,
  FX_MODE_STATIC_PATTERN,
  FX_MODE_TRI_STATIC_PATTERN,
  FX_MODE_SPOTS,
  FX_MODE_PERCENT,
  // One colour changes
  FX_MODE_RANDOM_COLOR,
  // Wipe/Sweep/Runners 
  FX_MODE_COLOR_WIPE, 
  FX_MODE_COLOR_WIPE_RANDOM,
  FX_MODE_COLOR_SWEEP,
  FX_MODE_COLOR_SWEEP_RANDOM,
  FX_MODE_TRICOLOR_WIPE,
  FX_MODE_ANDROID,
  FX_MODE_RUNNING_RED_BLUE,
  FX_MODE_RUNNING_COLOR,
  FX_MODE_RUNNING_RANDOM,
  FX_MODE_GRADIENT,
  FX_MODE_LOADING,
  FX_MODE_POLICE,
  FX_MODE_POLICE_ALL,
  FX_MODE_TWO_DOTS,
  FX_MODE_TWO_AREAS,
  FX_MODE_MULTI_COMET,
  FX_MODE_OSCILLATE,
  FX_MODE_BPM,
  FX_MODE_JUGGLE,
  FX_MODE_PALETTE,
  FX_MODE_COLORWAVES,
  FX_MODE_LAKE,
  FX_MODE_GLITTER,
  FX_MODE_METEOR,
  FX_MODE_METEOR_SMOOTH,
  FX_MODE_PRIDE_2015,
  FX_MODE_RIPPLE_RAINBOW,
  FX_MODE_PACIFICA,
  FX_MODE_SUNRISE,
  FX_MODE_SINEWAVE,
  FX_MODE_FLOW,
  FX_MODE_PHASEDNOISE,
  FX_MODE_PHASED,
  FX_MODE_RUNNING_LIGHTS,
  FX_MODE_RAINBOW_CYCLE,
  FX_MODE_MERRY_CHRISTMAS,
  FX_MODE_HALLOWEEN,
  // Chase
  FX_MODE_CHASE_COLOR,
  FX_MODE_CHASE_RANDOM,
  FX_MODE_CHASE_RAINBOW, 
  FX_MODE_CHASE_FLASH,
  FX_MODE_CHASE_FLASH_RANDOM, 
  FX_MODE_CHASE_RAINBOW_WHITE,
  FX_MODE_THEATER_CHASE,
  FX_MODE_THEATER_CHASE_RAINBOW,
  FX_MODE_TRICOLOR_CHASE,
  FX_MODE_RANDOM_CHASE,
  FX_MODE_CIRCUS_COMBUSTUS,
  // Breathe/Fade/Pulse
  FX_MODE_BREATH,
  FX_MODE_FADE,
  FX_MODE_TRICOLOR_FADE,
  FX_MODE_SPOTS_FADE,
  // Fireworks
  FX_MODE_FIREWORKS,
  FX_MODE_STARBURST,
  FX_MODE_EXPLODING_FIREWORKS,
  FX_MODE_RAIN,
  // Sparkle/Twinkle
  FX_MODE_SOLID_GLITTER,
  FX_MODE_POPCORN,
  FX_MODE_PLASMA,
  FX_MODE_FIRE_FLICKER,
  FX_MODE_SPARKLE,
  FX_MODE_FLASH_SPARKLE,
  FX_MODE_HYPER_SPARKLE,
  FX_MODE_TWINKLE,
  FX_MODE_COLORTWINKLE,
  FX_MODE_TWINKLEFOX,
  FX_MODE_TWINKLECAT,
  FX_MODE_TWINKLEUP,
  FX_MODE_DYNAMIC,
  FX_MODE_SAW,
  FX_MODE_DISSOLVE,
  FX_MODE_DISSOLVE_RANDOM,
  FX_MODE_COLORFUL,
  FX_MODE_TRAFFIC_LIGHT,
  FX_MODE_CANDLE,
  FX_MODE_CANDLE_MULTI,
  FX_MODE_HALLOWEEN_EYES,
  #ifdef ENABLE_ADVANCED_EFFECTS  
  // Blink/Strobe
  FX_MODE_BLINK,
  FX_MODE_BLINK_RAINBOW,
  FX_MODE_STROBE,
  FX_MODE_MULTI_STROBE,
  FX_MODE_STROBE_RAINBOW, 
  FX_MODE_RAINBOW,
  FX_MODE_LIGHTNING,     
  FX_MODE_FIRE_2012,
  FX_MODE_RAILWAY,
  FX_MODE_HEARTBEAT, 
  // Noise
  FX_MODE_FILLNOISE8,
  FX_MODE_NOISE16_1,
  FX_MODE_NOISE16_2,
  FX_MODE_NOISE16_3,
  FX_MODE_NOISE16_4,
  FX_MODE_NOISEPAL,
  // Scan
  FX_MODE_SCAN,
  FX_MODE_DUAL_SCAN,
  FX_MODE_LARSON_SCANNER,
  FX_MODE_DUAL_LARSON_SCANNER,
  FX_MODE_ICU,
  FX_MODE_RIPPLE,
  FX_MODE_COMET,
  FX_MODE_CHUNCHUN,
  FX_MODE_BOUNCINGBALLS,
  FX_MODE_SINELON,
  FX_MODE_SINELON_DUAL,
  FX_MODE_SINELON_RAINBOW,
  FX_MODE_DRIP,      
  #endif // ENABLE_ADVANCED_EFFECTS   
  // Length of Effects
  MODE_COUNT,
};

#include "AnimationFunctions/WLED_Animations.h"

class mWLEDEffects :
  public mTaskerInterface
{
  public:
    mWLEDEffects(){};
    void Init();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    // led fx library object
    WS2812FX* mEffects = nullptr;

    void CommandSet_EffectsModeID(uint8_t mode, uint8_t segment = 0);
    int8_t GetEffectsModeIDbyName(const char* c);
    const char* GetEffectsModeNamebyID(uint8_t id, char* buffer, uint8_t buflen);

    float minf2(float v, float w);
    float maxf2(float v, float w);

    //colors.cpp
    void colorFromUint32(uint32_t in, bool secondary = false);
    void colorFromUint24(uint32_t in, bool secondary = false);
    void relativeChangeWhite(int8_t amount, byte lowerBoundary = 0);
    void colorHStoRGB(uint16_t hue, byte sat, byte* rgb); //hue, sat to rgb
    void colorCTtoRGB(uint16_t mired, byte* rgb); //white spectrum to rgb

    void colorXYtoRGB(float x, float y, byte* rgb); // only defined if huesync disabled TODO
    void colorRGBtoXY(byte* rgb, float* xy); // only defined if huesync disabled TODO

    void colorFromDecOrHexString(byte* rgb, char* in);
    void colorRGBtoRGBW(byte* rgb); //rgb to rgbw (http://codewelt.com/rgbw). (RGBW_MODE_LEGACY)


    void setValuesFromMainSeg();
    void resetTimebase();
    void toggleOnOff();
    void setAllLeds();
    void setLedsStandard(bool justColors = false);
    bool colorChanged();
    void colorUpdated(int callMode);
    void updateInterfaces(uint8_t callMode);
    void handleTransitions();


    byte getSameCodeLength(char code, int index, char const cronixieDisplay[]);
    void setCronixie();
    void _overlayCronixie();    
    void _drawOverlayCronixie();

    //set.cpp
    void _setRandomColor(bool _sec,bool fromButton=false);
    bool isAsterisksOnly(const char* str, byte maxLen);



    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void handleSettingsSet(AsyncWebServerRequest *request, byte subPage);
    bool handleSet(AsyncWebServerRequest *request, const String& req);
    #endif // USE_MODULE_NETWORK_WEBSERVER


    int getNumVal(const String* req, uint16_t pos);
    bool updateVal(const String* req, const char* key, byte* val, byte minv=0, byte maxv=255);

    bool applyPreset(byte index, bool loadBri = true);
    void savePreset(byte index, bool persist = true);
        
    // LED CONFIG
    uint16_t ledCount = 30;          // overcurrent prevented by ABL
    bool useRGBW      = false;       // SK6812 strips can contain an extra White channel
    bool turnOnAtBoot = true;        // turn on LEDs at power-up
    byte bootPreset   = 0;           // save preset to load after power-up


    byte col[4]    = { 255, 160, 0, 0 };  // current RGB(W) primary color. col[] should be updated if you want to change the color.
    byte colSec[4] = { 0, 0, 0, 0 };      // current RGB(W) secondary color
    byte briS     = 128;                     // default brightness

    bool fadeTransition      = true;   // enable crossfading color transition
    uint16_t transitionDelay = 750;    // default crossfade duration in ms

    bool skipFirstLed  = false;        // ignore first LED in strip (useful if you need the LED as signal repeater)
    byte briMultiplier = 100;          // % of brightness to set (to limit power, if you set it to 50 and set bri to 255, actual brightness will be 127)

    uint16_t realtimeTimeoutMs = 2500;               // ms timeout of realtime mode before returning to normal mode
    int arlsOffset = 0;                              // realtime LED offset
    bool arlsDisableGammaCorrection = true;          // activate if gamma correction is handled by the source
    bool arlsForceMaxBri = false;                    // enable to force max brightness if source has very dark colors that would be black

    // wifi
    bool apActive = false;
    bool forceReconnect = false;
    uint32_t lastReconnectAttempt = 0;
    bool interfacesInited = false;
    bool wasConnected = false;

    // color
    byte colOld[4]    = { 0, 0, 0, 0 };        // color before transition
    byte colT[4]      = { 0, 0, 0, 0 };          // color that is currently displayed on the LEDs
    byte colIT[4]     = { 0, 0, 0, 0 };         // color that was last sent to LEDs
    byte colSecT[4]   = { 0, 0, 0, 0 };
    byte colSecOld[4] = { 0, 0, 0, 0 };
    byte colSecIT[4]  = { 0, 0, 0, 0 };

    // transitions
    bool transitionActive = false;
    uint16_t transitionDelayDefault = transitionDelay;
    uint16_t transitionDelayTemp = transitionDelay;
    unsigned long transitionStartTime;
    float tperLast = 0;        // crossfade transition progress, 0.0f - 1.0f
    bool jsonTransitionOnce = false;

    // brightness
    unsigned long lastOnTime = 0;
    bool offMode = !turnOnAtBoot;
    byte bri = briS;
    byte briOld = 0;
    byte briT = 0;
    byte briIT = 0;
    byte briLast = 128;          // brightness before turned off. Used for toggle function
    byte whiteLast = 128;        // white channel before turned off. Used for toggle function

    // effects
    byte effectCurrent = 0;
    byte effectSpeed = 128;
    byte effectIntensity = 128;
    byte effectPalette = 0;

    // preset cycling
    bool presetCyclingEnabled = false;
    byte presetCycleMin = 1, presetCycleMax = 5;
    uint16_t presetCycleTime = 12;
    unsigned long presetCycledTime = 0;
    byte presetCycCurr = presetCycleMin;
    bool presetApplyBri = true;
    bool saveCurrPresetCycConf = false;

    // realtime
    byte realtimeMode = REALTIME_MODE_INACTIVE;
    byte realtimeOverride = REALTIME_OVERRIDE_NONE;
    unsigned long realtimeTimeout = 0;
    uint16_t tpmFirstFrameSize = 0;

    // presets
    uint16_t savedPresets = 0;
    int8_t currentPreset = -1;
    bool isPreset = false;

    void parse_JSONCommand(JsonParserObject obj);

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    RgbcctColor output_colour;
    void SubTask_Animation();

    static const char* PM_MODULE_LIGHTS_WLED_CTR;
    static const char* PM_MODULE_LIGHTS_WLED_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_LIGHTS_WLED_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_LIGHTS_WLED_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_LIGHTS_WLED_ID; }

    #ifdef USE_MODULE_NETWORK_WEBSERVER
      void WebCommand_Parse();
      int8_t Tasker_Web(uint8_t function);
      void WebAppend_JSON_RootPage_LiveviewPixels();
      void WebAppend_Root_ControlUI();
      void WebAppend_Root_Status_Table();
    #endif // USE_MODULE_NETWORK_WEBSERVER

};


#endif

#endif


// GLOBAL VARIABLES
// both declared and defined in header (solution from http://www.keil.com/support/docs/1868.htm)
//
//e.g. byte test = 2 becomes WLED_GLOBAL byte test _INIT(2);
//     int arr[]{0,1,2} becomes WLED_GLOBAL int arr[] _INIT_N(({0,1,2}));

// #ifndef WLED_DEFINE_GLOBAL_VARS
// # define WLED_GLOBAL extern
// # define _INIT(x)
// # define _INIT_N(x)
// #else
// # define WLED_GLOBAL
// # define _INIT(x) = x

// //needed to ignore commas in array definitions
// #define UNPACK( ... ) __VA_ARGS__
// # define _INIT_N(x) UNPACK x
// #endif

// // LED CONFIG
// WLED_GLOBAL uint16_t ledCount2 _INIT(30);          // overcurrent prevented by ABL
// WLED_GLOBAL bool useRGBW2      _INIT(false);       // SK6812 strips can contain an extra White channel
// WLED_GLOBAL bool turnOnAtBoot2 _INIT(true);        // turn on LEDs at power-up
// WLED_GLOBAL byte bootPreset2   _INIT(0);           // save preset to load after power-up


