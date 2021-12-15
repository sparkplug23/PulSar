#ifndef _AnimatorDALIGHTNEOPIXEL712_segH
#define _AnimatorDALIGHTNEOPIXEL712_segH 7.0

#include "1_TaskerManager/mTaskerManager.h"


#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION


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

    // presets
    uint16_t savedPresets = 0;
    int8_t currentPreset = -1;
    bool isPreset = false;

    // realtime
    byte realtimeMode = REALTIME_MODE_INACTIVE;
    byte realtimeOverride = REALTIME_OVERRIDE_NONE;
    unsigned long realtimeTimeout = 0;
    uint16_t tpmFirstFrameSize = 0;

// #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
//     ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//     led fx library object
    WS2812FX* mEffects = nullptr; // this new segments replaces this, though the need for static buffers may require this as a sub-class??
    // #endif //    USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
    // ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS

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

   

// void Init();
void SubTask_WLED_Animation_PhaseOut();

float minf2(float v, float w);
float maxf2(float v, float w);
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

//colors.cpp
void colorFromUint32(uint32_t in, bool secondary = false);
void colorFromUint24(uint32_t in, bool secondary = false);
void relativeChangeWhite(int8_t amount, byte lowerBoundary = 0);
void colorHStoRGB(uint16_t hue, byte sat, byte* rgb); //hue, sat to rgb
void colorCTtoRGB(uint16_t mired, byte* rgb); //white spectrum to rgb






#endif // USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION



#endif

// #endif
