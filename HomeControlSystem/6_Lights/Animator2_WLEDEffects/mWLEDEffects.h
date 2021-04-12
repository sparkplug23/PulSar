#ifndef MWLEDEFFECTS_H
#define MWLEDEFFECTS_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS

#include "1_TaskerManager/mTaskerManager.h"
// #include "6_Lights/WLEDEffects/mWLEDEffects_web.h"

// #include "6_Lights/WLEDEffects/wled.h"
#include "wled_palettes.h"


#define D_FX_MODE_STATIC_CTR                  "Static"
#define D_FX_MODE_STATIC_PATTERN_CTR          "Solid Pattern"
#define D_FX_MODE_TRI_STATIC_PATTERN_CTR      "Solid Pattern Tri"
#define D_FX_MODE_BLINK_CTR                   "Blink"
#define D_FX_MODE_BREATH_CTR                  "Breathe"
#define D_FX_MODE_COLOR_WIPE_CTR              "Wipe"
#define D_FX_MODE_COLOR_WIPE_RANDOM_CTR       "Wipe Random"
#define D_FX_MODE_RANDOM_COLOR_CTR            "Random Colors"
#define D_FX_MODE_COLOR_SWEEP_CTR             "Sweep"
#define D_FX_MODE_DYNAMIC_CTR                 "Dynamic"
#define D_FX_MODE_RAINBOW_CTR                 "Colorloop"
#define D_FX_MODE_RAINBOW_CYCLE_CTR           "Rainbow"
#define D_FX_MODE_SCAN_CTR                    "Scan"
#define D_FX_MODE_DUAL_SCAN_CTR               "Scan Dual"
#define D_FX_MODE_FADE_CTR                    "Fade"
#define D_FX_MODE_THEATER_CHASE_CTR           "Theater"
#define D_FX_MODE_THEATER_CHASE_RAINBOW_CTR   "Theater Rainbow"
#define D_FX_MODE_RUNNING_LIGHTS_CTR          "Running"
#define D_FX_MODE_SAW_CTR                     "Saw"
#define D_FX_MODE_TWINKLE_CTR                 "Twinkle"
#define D_FX_MODE_DISSOLVE_CTR                "Dissolve"
#define D_FX_MODE_DISSOLVE_RANDOM_CTR         "Dissolve Rnd"
#define D_FX_MODE_SPARKLE_CTR                 "Sparkle"
#define D_FX_MODE_FLASH_SPARKLE_CTR           "Sparkle Dark"
#define D_FX_MODE_HYPER_SPARKLE_CTR           "Sparkle+"
#define D_FX_MODE_STROBE_CTR                  "Strobe"
#define D_FX_MODE_STROBE_RAINBOW_CTR          "Strobe Rainbow"
#define D_FX_MODE_MULTI_STROBE_CTR            "Strobe Mega"
#define D_FX_MODE_BLINK_RAINBOW_CTR           "Blink Rainbow"
#define D_FX_MODE_ANDROID_CTR                 "Android"
#define D_FX_MODE_CHASE_COLOR_CTR             "Chase"
#define D_FX_MODE_CHASE_RANDOM_CTR            "Chase Random"
#define D_FX_MODE_CHASE_RAINBOW_CTR           "Chase Rainbow"
#define D_FX_MODE_CHASE_FLASH_CTR             "Chase Flash"
#define D_FX_MODE_CHASE_FLASH_RANDOM_CTR      "Chase Flash Rnd"
#define D_FX_MODE_CHASE_RAINBOW_WHITE_CTR     "Rainbow Runner"
#define D_FX_MODE_COLORFUL_CTR                "Colorful"
#define D_FX_MODE_TRAFFIC_LIGHT_CTR           "Traffic Light"
#define D_FX_MODE_COLOR_SWEEP_RANDOM_CTR      "Sweep Random"
#define D_FX_MODE_RUNNING_COLOR_CTR           "Running 2"
#define D_FX_MODE_RUNNING_RED_BLUE_CTR        "Red & Blue"
#define D_FX_MODE_RUNNING_RANDOM_CTR          "Stream"
#define D_FX_MODE_LARSON_SCANNER_CTR          "Scanner"
#define D_FX_MODE_COMET_CTR                   "Lighthouse"
#define D_FX_MODE_FIREWORKS_CTR               "Fireworks"
#define D_FX_MODE_RAIN_CTR                    "Rain"
#define D_FX_MODE_MERRY_CHRISTMAS_CTR         "Merry Christmas"
#define D_FX_MODE_FIRE_FLICKER_CTR            "Fire Flicker"
#define D_FX_MODE_GRADIENT_CTR                "Gradient"
#define D_FX_MODE_LOADING_CTR                 "Loading"
#define D_FX_MODE_POLICE_CTR                  "Police"
#define D_FX_MODE_POLICE_ALL_CTR              "Police All"
#define D_FX_MODE_TWO_DOTS_CTR                "Two Dots"
#define D_FX_MODE_TWO_AREAS_CTR               "Two Areas"
#define D_FX_MODE_CIRCUS_COMBUSTUS_CTR        "Circus"
#define D_FX_MODE_HALLOWEEN_CTR               "Halloween"
#define D_FX_MODE_TRICOLOR_CHASE_CTR          "Tri Chase"
#define D_FX_MODE_TRICOLOR_WIPE_CTR           "Tri Wipe"
#define D_FX_MODE_TRICOLOR_FADE_CTR           "Tri Fade"
#define D_FX_MODE_LIGHTNING_CTR               "Lightning"
#define D_FX_MODE_ICU_CTR                     "ICU"
#define D_FX_MODE_MULTI_COMET_CTR             "Multi Comet"
#define D_FX_MODE_DUAL_LARSON_SCANNER_CTR     "Scanner Dual"
#define D_FX_MODE_RANDOM_CHASE_CTR            "Stream 2"
#define D_FX_MODE_OSCILLATE_CTR               "Oscillate"
#define D_FX_MODE_PRIDE_2015_CTR              "Pride 2015"
#define D_FX_MODE_JUGGLE_CTR                  "Juggle"
#define D_FX_MODE_PALETTE_CTR                 "Palette"
#define D_FX_MODE_FIRE_2012_CTR               "Fire 2012"
#define D_FX_MODE_COLORWAVES_CTR              "Colorwaves"
#define D_FX_MODE_BPM_CTR                     "Bpm"
#define D_FX_MODE_FILLNOISE8_CTR              "Fill Noise"
#define D_FX_MODE_NOISE16_1_CTR               "Noise 1"
#define D_FX_MODE_NOISE16_2_CTR               "Noise 2"
#define D_FX_MODE_NOISE16_3_CTR               "Noise 3"
#define D_FX_MODE_NOISE16_4_CTR               "Noise 4"
#define D_FX_MODE_COLORTWINKLE_CTR            "Colortwinkles"
#define D_FX_MODE_LAKE_CTR                    "Lake"
#define D_FX_MODE_METEOR_CTR                  "Meteor"
#define D_FX_MODE_METEOR_SMOOTH_CTR           "Meteor Smooth"
#define D_FX_MODE_RAILWAY_CTR                 "Railway"
#define D_FX_MODE_RIPPLE_CTR                  "Ripple"
#define D_FX_MODE_TWINKLEFOX_CTR              "Twinklefox"
#define D_FX_MODE_TWINKLECAT_CTR              "Twinklecat"
#define D_FX_MODE_HALLOWEEN_EYES_CTR          "Halloween Eyes"
#define D_FX_MODE_SPOTS_CTR                   "Spots"
#define D_FX_MODE_SPOTS_FADE_CTR              "Spots Fade"
#define D_FX_MODE_GLITTER_CTR                 "Glitter"
#define D_FX_MODE_CANDLE_CTR                  "Candle"
#define D_FX_MODE_STARBURST_CTR               "Fireworks Starburst"
#define D_FX_MODE_EXPLODING_FIREWORKS_CTR     "Fireworks 1D"
#define D_FX_MODE_BOUNCINGBALLS_CTR           "Bouncing Balls"
#define D_FX_MODE_SINELON_CTR                 "Sinelon"
#define D_FX_MODE_SINELON_DUAL_CTR            "Sinelon Dual"
#define D_FX_MODE_SINELON_RAINBOW_CTR         "Sinelon Rainbow"
#define D_FX_MODE_POPCORN_CTR                 "Popcorn"
#define D_FX_MODE_DRIP_CTR                    "Drip"
#define D_FX_MODE_PLASMA_CTR                  "Plasma"
#define D_FX_MODE_PERCENT_CTR                 "Percent"
#define D_FX_MODE_RIPPLE_RAINBOW_CTR          "Ripple Rainbow"
#define D_FX_MODE_HEARTBEAT_CTR               "Heartbeat"
#define D_FX_MODE_PACIFICA_CTR                "Pacifica"
#define D_FX_MODE_CANDLE_MULTI_CTR            "Candle Multi"
#define D_FX_MODE_SOLID_GLITTER_CTR           "Solid Glitter"
#define D_FX_MODE_SUNRISE_CTR                 "Sunrise"
#define D_FX_MODE_PHASED_CTR                  "Phased"
#define D_FX_MODE_TWINKLEUP_CTR               "Twinkleup"
#define D_FX_MODE_NOISEPAL_CTR                "Noise Pal"
#define D_FX_MODE_SINEWAVE_CTR                "Sine"
#define D_FX_MODE_PHASEDNOISE_CTR             "Phased Noise"
#define D_FX_MODE_FLOW_CTR                    "Flow"
#define D_FX_MODE_CHUNCHUN_CTR                "Chunchun"


// Doing this with memcmp_p until effects are fully designed, then for memory reasons
// move to "dlist", "SearchForTextIndexedID" and "GetTextIndexed_P"
DEFINE_PGM_CTR(PM_FX_MODE_STATIC_CTR)                 D_FX_MODE_STATIC_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_BLINK_CTR)                  D_FX_MODE_BLINK_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_BREATH_CTR)                 D_FX_MODE_BREATH_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COLOR_WIPE_CTR)             D_FX_MODE_COLOR_WIPE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COLOR_WIPE_RANDOM_CTR)      D_FX_MODE_COLOR_WIPE_RANDOM_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RANDOM_COLOR_CTR)           D_FX_MODE_RANDOM_COLOR_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COLOR_SWEEP_CTR)            D_FX_MODE_COLOR_SWEEP_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_DYNAMIC_CTR)                D_FX_MODE_DYNAMIC_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RAINBOW_CTR)                D_FX_MODE_RAINBOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RAINBOW_CYCLE_CTR)          D_FX_MODE_RAINBOW_CYCLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SCAN_CTR)                   D_FX_MODE_SCAN_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_DUAL_SCAN_CTR)              D_FX_MODE_DUAL_SCAN_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_FADE_CTR)                   D_FX_MODE_FADE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_THEATER_CHASE_CTR)          D_FX_MODE_THEATER_CHASE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_THEATER_CHASE_RAINBOW_CTR)  D_FX_MODE_THEATER_CHASE_RAINBOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RUNNING_LIGHTS_CTR)         D_FX_MODE_RUNNING_LIGHTS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SAW_CTR)                    D_FX_MODE_SAW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TWINKLE_CTR)                D_FX_MODE_TWINKLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_DISSOLVE_CTR)               D_FX_MODE_DISSOLVE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_DISSOLVE_RANDOM_CTR)        D_FX_MODE_DISSOLVE_RANDOM_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SPARKLE_CTR)                D_FX_MODE_SPARKLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_FLASH_SPARKLE_CTR)          D_FX_MODE_FLASH_SPARKLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_HYPER_SPARKLE_CTR)          D_FX_MODE_HYPER_SPARKLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_STROBE_CTR)                 D_FX_MODE_STROBE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_STROBE_RAINBOW_CTR)         D_FX_MODE_STROBE_RAINBOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_MULTI_STROBE_CTR)           D_FX_MODE_MULTI_STROBE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_BLINK_RAINBOW_CTR)          D_FX_MODE_BLINK_RAINBOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_ANDROID_CTR)                D_FX_MODE_ANDROID_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CHASE_COLOR_CTR)            D_FX_MODE_CHASE_COLOR_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CHASE_RANDOM_CTR)           D_FX_MODE_CHASE_RANDOM_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CHASE_RAINBOW_CTR)          D_FX_MODE_CHASE_RAINBOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CHASE_FLASH_CTR)            D_FX_MODE_CHASE_FLASH_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CHASE_FLASH_RANDOM_CTR)     D_FX_MODE_CHASE_FLASH_RANDOM_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CHASE_RAINBOW_WHITE_CTR)    D_FX_MODE_CHASE_RAINBOW_WHITE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COLORFUL_CTR)               D_FX_MODE_COLORFUL_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TRAFFIC_LIGHT_CTR)          D_FX_MODE_TRAFFIC_LIGHT_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COLOR_SWEEP_RANDOM_CTR)     D_FX_MODE_COLOR_SWEEP_RANDOM_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RUNNING_COLOR_CTR)          D_FX_MODE_RUNNING_COLOR_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RUNNING_RED_BLUE_CTR)       D_FX_MODE_RUNNING_RED_BLUE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RUNNING_RANDOM_CTR)         D_FX_MODE_RUNNING_RANDOM_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_LARSON_SCANNER_CTR)         D_FX_MODE_LARSON_SCANNER_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COMET_CTR)                  D_FX_MODE_COMET_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_FIREWORKS_CTR)              D_FX_MODE_FIREWORKS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RAIN_CTR)                   D_FX_MODE_RAIN_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_MERRY_CHRISTMAS_CTR)        D_FX_MODE_MERRY_CHRISTMAS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_FIRE_FLICKER_CTR)           D_FX_MODE_FIRE_FLICKER_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_GRADIENT_CTR)               D_FX_MODE_GRADIENT_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_LOADING_CTR)                D_FX_MODE_LOADING_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_POLICE_CTR)                 D_FX_MODE_POLICE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_POLICE_ALL_CTR)             D_FX_MODE_POLICE_ALL_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TWO_DOTS_CTR)               D_FX_MODE_TWO_DOTS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TWO_AREAS_CTR)              D_FX_MODE_TWO_AREAS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CIRCUS_COMBUSTUS_CTR)       D_FX_MODE_CIRCUS_COMBUSTUS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_HALLOWEEN_CTR)              D_FX_MODE_HALLOWEEN_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TRICOLOR_CHASE_CTR)         D_FX_MODE_TRICOLOR_CHASE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TRICOLOR_WIPE_CTR)          D_FX_MODE_TRICOLOR_WIPE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TRICOLOR_FADE_CTR)          D_FX_MODE_TRICOLOR_FADE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_LIGHTNING_CTR)              D_FX_MODE_LIGHTNING_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_ICU_CTR)                    D_FX_MODE_ICU_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_MULTI_COMET_CTR)            D_FX_MODE_MULTI_COMET_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_DUAL_LARSON_SCANNER_CTR)    D_FX_MODE_DUAL_LARSON_SCANNER_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RANDOM_CHASE_CTR)           D_FX_MODE_RANDOM_CHASE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_OSCILLATE_CTR)              D_FX_MODE_OSCILLATE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_PRIDE_2015_CTR)             D_FX_MODE_PRIDE_2015_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_JUGGLE_CTR)                 D_FX_MODE_JUGGLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_PALETTE_CTR)                D_FX_MODE_PALETTE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_FIRE_2012_CTR)              D_FX_MODE_FIRE_2012_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COLORWAVES_CTR)             D_FX_MODE_COLORWAVES_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_BPM_CTR)                    D_FX_MODE_BPM_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_FILLNOISE8_CTR)             D_FX_MODE_FILLNOISE8_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_NOISE16_1_CTR)              D_FX_MODE_NOISE16_1_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_NOISE16_2_CTR)              D_FX_MODE_NOISE16_2_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_NOISE16_3_CTR)              D_FX_MODE_NOISE16_3_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_NOISE16_4_CTR)              D_FX_MODE_NOISE16_4_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_COLORTWINKLE_CTR)           D_FX_MODE_COLORTWINKLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_LAKE_CTR)                   D_FX_MODE_LAKE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_METEOR_CTR)                 D_FX_MODE_METEOR_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_METEOR_SMOOTH_CTR)          D_FX_MODE_METEOR_SMOOTH_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RAILWAY_CTR)                D_FX_MODE_RAILWAY_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RIPPLE_CTR)                 D_FX_MODE_RIPPLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TWINKLEFOX_CTR)             D_FX_MODE_TWINKLEFOX_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TWINKLECAT_CTR)             D_FX_MODE_TWINKLECAT_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_HALLOWEEN_EYES_CTR)         D_FX_MODE_HALLOWEEN_EYES_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_STATIC_PATTERN_CTR)         D_FX_MODE_STATIC_PATTERN_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TRI_STATIC_PATTERN_CTR)     D_FX_MODE_TRI_STATIC_PATTERN_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SPOTS_CTR)                  D_FX_MODE_SPOTS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SPOTS_FADE_CTR)             D_FX_MODE_SPOTS_FADE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_GLITTER_CTR)                D_FX_MODE_GLITTER_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CANDLE_CTR)                 D_FX_MODE_CANDLE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_STARBURST_CTR)              D_FX_MODE_STARBURST_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_EXPLODING_FIREWORKS_CTR)    D_FX_MODE_EXPLODING_FIREWORKS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_BOUNCINGBALLS_CTR)          D_FX_MODE_BOUNCINGBALLS_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SINELON_CTR)                D_FX_MODE_SINELON_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SINELON_DUAL_CTR)           D_FX_MODE_SINELON_DUAL_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SINELON_RAINBOW_CTR)        D_FX_MODE_SINELON_RAINBOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_POPCORN_CTR)                D_FX_MODE_POPCORN_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_DRIP_CTR)                   D_FX_MODE_DRIP_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_PLASMA_CTR)                 D_FX_MODE_PLASMA_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_PERCENT_CTR)                D_FX_MODE_PERCENT_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_RIPPLE_RAINBOW_CTR)         D_FX_MODE_RIPPLE_RAINBOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_HEARTBEAT_CTR)              D_FX_MODE_HEARTBEAT_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_PACIFICA_CTR)               D_FX_MODE_PACIFICA_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CANDLE_MULTI_CTR)           D_FX_MODE_CANDLE_MULTI_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SOLID_GLITTER_CTR)          D_FX_MODE_SOLID_GLITTER_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SUNRISE_CTR)                D_FX_MODE_SUNRISE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_PHASED_CTR)                 D_FX_MODE_PHASED_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_TWINKLEUP_CTR)              D_FX_MODE_TWINKLEUP_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_NOISEPAL_CTR)               D_FX_MODE_NOISEPAL_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_SINEWAVE_CTR)               D_FX_MODE_SINEWAVE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_PHASEDNOISE_CTR)            D_FX_MODE_PHASEDNOISE_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_FLOW_CTR)                   D_FX_MODE_FLOW_CTR;
DEFINE_PGM_CTR(PM_FX_MODE_CHUNCHUN_CTR)               D_FX_MODE_CHUNCHUN_CTR;

// You need to choose some of these features to disable:
#define WLED_DISABLE_ALEXA       // saves 11kb
#define WLED_DISABLE_BLYNK       // saves 6kb
#define WLED_DISABLE_CRONIXIE    // saves 3kb
#define WLED_DISABLE_HUESYNC     // saves 4kb
#define WLED_DISABLE_INFRARED    // there is no pin left for this on ESP8266-01, saves 12kb
#define WLED_DISABLE_FILESYSTEM      

// to toggle usb serial debug (un)comment the following line
#define WLED_DEBUG

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

// maximum number of LEDs - MAX_LEDS is coming from the JSON response getting too big, MAX_LEDS_DMA will become a timing issue
#define MAX_LEDS_WLED 1500
#define MAX_LEDS_DMA 500


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

// #define MODE_COUNT  112

DEFINE_PGM_CTR(PM_FX_MODES_NAMES_CTR)
{
  // Static
  D_FX_MODE_STATIC_CTR  "|"
  D_FX_MODE_STATIC_PATTERN_CTR  "|"
  D_FX_MODE_TRI_STATIC_PATTERN_CTR  "|"
  D_FX_MODE_SPOTS_CTR  "|"
  D_FX_MODE_PERCENT_CTR  "|"
    // One colour changes
  D_FX_MODE_RANDOM_COLOR_CTR  "|"
    // Wipe
  D_FX_MODE_COLOR_WIPE_CTR  "|" 
  D_FX_MODE_COLOR_WIPE_RANDOM_CTR  "|"
  D_FX_MODE_COLOR_SWEEP_CTR  "|"
  D_FX_MODE_COLOR_SWEEP_RANDOM_CTR  "|"
  D_FX_MODE_TRICOLOR_WIPE_CTR  "|"
  D_FX_MODE_ANDROID_CTR  "|"
  D_FX_MODE_RUNNING_RED_BLUE_CTR  "|"
  D_FX_MODE_RUNNING_COLOR_CTR  "|"
  D_FX_MODE_RUNNING_RANDOM_CTR  "|"
  D_FX_MODE_GRADIENT_CTR  "|"
  D_FX_MODE_LOADING_CTR  "|"
  D_FX_MODE_POLICE_CTR  "|"
  D_FX_MODE_POLICE_ALL_CTR  "|"
  D_FX_MODE_TWO_DOTS_CTR  "|"
  D_FX_MODE_TWO_AREAS_CTR  "|"
  D_FX_MODE_MULTI_COMET_CTR  "|"
  D_FX_MODE_OSCILLATE_CTR  "|"
  D_FX_MODE_BPM_CTR  "|"
  D_FX_MODE_JUGGLE_CTR  "|"
  D_FX_MODE_PALETTE_CTR  "|"
  D_FX_MODE_COLORWAVES_CTR  "|"
  D_FX_MODE_LAKE_CTR  "|"
  D_FX_MODE_GLITTER_CTR  "|"
  D_FX_MODE_METEOR_CTR  "|"
  D_FX_MODE_METEOR_SMOOTH_CTR  "|"
  D_FX_MODE_PRIDE_2015_CTR  "|"
  D_FX_MODE_RIPPLE_RAINBOW_CTR  "|"
  D_FX_MODE_PACIFICA_CTR  "|"
  D_FX_MODE_SUNRISE_CTR  "|"
  D_FX_MODE_SINEWAVE_CTR  "|"
  D_FX_MODE_FLOW_CTR  "|"
  D_FX_MODE_PHASEDNOISE_CTR  "|"
  D_FX_MODE_PHASED_CTR  "|"
  D_FX_MODE_RUNNING_LIGHTS_CTR  "|"
  D_FX_MODE_RAINBOW_CYCLE_CTR  "|"
  D_FX_MODE_MERRY_CHRISTMAS_CTR  "|"
  D_FX_MODE_HALLOWEEN_CTR  "|"
    // Chase
  D_FX_MODE_CHASE_COLOR_CTR  "|"
  D_FX_MODE_CHASE_RANDOM_CTR  "|"
  D_FX_MODE_CHASE_RAINBOW_CTR  "|" 
  D_FX_MODE_CHASE_FLASH_CTR  "|"
  D_FX_MODE_CHASE_FLASH_RANDOM_CTR  "|" 
  D_FX_MODE_CHASE_RAINBOW_WHITE_CTR  "|"
  D_FX_MODE_THEATER_CHASE_CTR  "|"
  D_FX_MODE_THEATER_CHASE_RAINBOW_CTR  "|"
  D_FX_MODE_TRICOLOR_CHASE_CTR  "|"
  D_FX_MODE_RANDOM_CHASE_CTR  "|"
  D_FX_MODE_CIRCUS_COMBUSTUS_CTR  "|"
    // Breathe/Fade/Pulse
  D_FX_MODE_BREATH_CTR  "|"
  D_FX_MODE_FADE_CTR  "|"
  D_FX_MODE_TRICOLOR_FADE_CTR  "|"
  D_FX_MODE_SPOTS_FADE_CTR  "|"
    // Fireworks
  D_FX_MODE_FIREWORKS_CTR  "|"
  D_FX_MODE_STARBURST_CTR  "|"
  D_FX_MODE_EXPLODING_FIREWORKS_CTR  "|"
  D_FX_MODE_RAIN_CTR  "|"
    // Sparkle/Twinkle
  D_FX_MODE_SOLID_GLITTER_CTR  "|"
  D_FX_MODE_POPCORN_CTR  "|"
  D_FX_MODE_PLASMA_CTR  "|"
  D_FX_MODE_FIRE_FLICKER_CTR  "|"
  D_FX_MODE_SPARKLE_CTR  "|"
  D_FX_MODE_FLASH_SPARKLE_CTR  "|"
  D_FX_MODE_HYPER_SPARKLE_CTR  "|"
  D_FX_MODE_TWINKLE_CTR  "|"
  D_FX_MODE_COLORTWINKLE_CTR  "|"
  D_FX_MODE_TWINKLEFOX_CTR  "|"
  D_FX_MODE_TWINKLECAT_CTR  "|"
  D_FX_MODE_TWINKLEUP_CTR  "|"
  D_FX_MODE_DYNAMIC_CTR  "|"
  D_FX_MODE_SAW_CTR  "|"
  D_FX_MODE_DISSOLVE_CTR  "|"
  D_FX_MODE_DISSOLVE_RANDOM_CTR  "|"
  D_FX_MODE_COLORFUL_CTR  "|"
  D_FX_MODE_TRAFFIC_LIGHT_CTR  "|"
  D_FX_MODE_CANDLE_CTR  "|"
  D_FX_MODE_CANDLE_MULTI_CTR  "|"
  D_FX_MODE_HALLOWEEN_EYES_CTR  "|"
  #ifdef ENABLE_ADVANCED_EFFECTS  
    // Blink/Strobe
  D_FX_MODE_BLINK_CTR  "|"
  D_FX_MODE_BLINK_RAINBOW_CTR  "|"
  D_FX_MODE_STROBE_CTR  "|"
  D_FX_MODE_MULTI_STROBE_CTR  "|"
  D_FX_MODE_STROBE_RAINBOW_CTR  "|" 
  D_FX_MODE_RAINBOW_CTR  "|"
  D_FX_MODE_LIGHTNING_CTR  "|"     
  D_FX_MODE_FIRE_2012_CTR  "|"
  D_FX_MODE_RAILWAY_CTR  "|"
  D_FX_MODE_HEARTBEAT_CTR  "|" 
    // Noise
  D_FX_MODE_FILLNOISE8_CTR  "|"
  D_FX_MODE_NOISE16_1_CTR  "|"
  D_FX_MODE_NOISE16_2_CTR  "|"
  D_FX_MODE_NOISE16_3_CTR  "|"
  D_FX_MODE_NOISE16_4_CTR  "|"
  D_FX_MODE_NOISEPAL_CTR  "|"
    // Scan
  D_FX_MODE_SCAN_CTR  "|"
  D_FX_MODE_DUAL_SCAN_CTR  "|"
  D_FX_MODE_LARSON_SCANNER_CTR  "|"
  D_FX_MODE_DUAL_LARSON_SCANNER_CTR  "|"
  D_FX_MODE_ICU_CTR  "|"
  D_FX_MODE_RIPPLE_CTR  "|"
  D_FX_MODE_COMET_CTR  "|"
  D_FX_MODE_CHUNCHUN_CTR  "|"
  D_FX_MODE_BOUNCINGBALLS_CTR  "|"
  D_FX_MODE_SINELON_CTR  "|"
  D_FX_MODE_SINELON_DUAL_CTR  "|"
  D_FX_MODE_SINELON_RAINBOW_CTR  "|"
  D_FX_MODE_DRIP_CTR
  #endif // ENABLE_ADVANCED_EFFECTS   
};



class WS2812FX {
  typedef uint16_t (WS2812FX::*mode_ptr)(void);

  // pre show callback
  typedef void (*show_callback) (void);
  
  // segment parameters
  public:
    typedef struct Segment { // 24 bytes
      uint16_t start;
      uint16_t stop; //segment invalid if stop == 0
      uint8_t speed;
      uint8_t intensity;
      uint8_t palette;
      uint8_t mode;
      uint8_t options; //bit pattern: msb first: transitional needspixelstate tbd tbd (paused) on reverse selected
      uint8_t grouping, spacing;
      uint8_t opacity;
      uint32_t colors[NUM_COLORS];
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
        return stop > start;
      }
      uint16_t length()
      {
        return stop - start;
      }
      uint16_t groupLength()
      {
        return grouping + spacing;
      }
      uint16_t virtualLength()
      {
        uint16_t groupLen = groupLength();
        uint16_t vLength = (length() + groupLen - 1) / groupLen;
        if (options & MIRROR)
          vLength = (vLength + 1) /2;  // divide by 2 if mirror, leave at least a signle LED
        return vLength;
      }
    } segment;

    // segment runtime parameters
    typedef struct Segment_runtime { // 28 bytes
      unsigned long next_time;
      uint32_t step;
      uint32_t call;
      uint16_t aux0;
      uint16_t aux1;
      byte* data = nullptr;
      bool allocateData(uint16_t len){
        if (data && _dataLen == len) return true; //already allocated
        deallocateData();
        if (WS2812FX::_usedSegmentData + len > MAX_SEGMENT_DATA) return false; //not enough memory
        data = new (std::nothrow) byte[len];
        if (!data) return false; //allocation failed
        WS2812FX::_usedSegmentData += len;
        _dataLen = len;
        memset(data, 0, len);
        return true;
      }
      void deallocateData(){
        delete[] data;
        data = nullptr;
        WS2812FX::_usedSegmentData -= _dataLen;
        _dataLen = 0;
      }
      void reset(){next_time = 0; step = 0; call = 0; aux0 = 0; aux1 = 0; deallocateData();}
      private:
        uint16_t _dataLen = 0;
    } segment_runtime;

    WS2812FX() {
      /**assign each member of the _mode[] array to its respective function reference 
      * I think this needs to be done here, so the space is also allocated during the class construct
      * */
      // Static
      _mode[FX_MODE_STATIC]                  = &WS2812FX::mode_static;
      _mode[FX_MODE_STATIC_PATTERN]          = &WS2812FX::mode_static_pattern;
      _mode[FX_MODE_TRI_STATIC_PATTERN]      = &WS2812FX::mode_tri_static_pattern;
      _mode[FX_MODE_SPOTS]                   = &WS2812FX::mode_spots;
      _mode[FX_MODE_PERCENT]                 = &WS2812FX::mode_percent;
      // Solid colour changes
      _mode[FX_MODE_RANDOM_COLOR]            = &WS2812FX::mode_random_color;
      // Wipe/Sweep/Runners 
      _mode[FX_MODE_COLOR_WIPE]              = &WS2812FX::mode_color_wipe;
      _mode[FX_MODE_COLOR_WIPE_RANDOM]       = &WS2812FX::mode_color_wipe_random;
      _mode[FX_MODE_COLOR_SWEEP]             = &WS2812FX::mode_color_sweep;
      _mode[FX_MODE_COLOR_SWEEP_RANDOM]      = &WS2812FX::mode_color_sweep_random;
      _mode[FX_MODE_TRICOLOR_WIPE]           = &WS2812FX::mode_tricolor_wipe;
      _mode[FX_MODE_ANDROID]                 = &WS2812FX::mode_android;
      _mode[FX_MODE_RUNNING_RED_BLUE]        = &WS2812FX::mode_running_red_blue;
      _mode[FX_MODE_RUNNING_COLOR]           = &WS2812FX::mode_running_color;
      _mode[FX_MODE_RUNNING_RANDOM]          = &WS2812FX::mode_running_random;
      _mode[FX_MODE_GRADIENT]                = &WS2812FX::mode_gradient;
      _mode[FX_MODE_LOADING]                 = &WS2812FX::mode_loading;
      _mode[FX_MODE_POLICE]                  = &WS2812FX::mode_police;
      _mode[FX_MODE_POLICE_ALL]              = &WS2812FX::mode_police_all;
      _mode[FX_MODE_TWO_DOTS]                = &WS2812FX::mode_two_dots;
      _mode[FX_MODE_TWO_AREAS]               = &WS2812FX::mode_two_areas;
      _mode[FX_MODE_MULTI_COMET]             = &WS2812FX::mode_multi_comet;
      _mode[FX_MODE_OSCILLATE]               = &WS2812FX::mode_oscillate;
      _mode[FX_MODE_BPM]                     = &WS2812FX::mode_bpm;
      _mode[FX_MODE_JUGGLE]                  = &WS2812FX::mode_juggle;
      _mode[FX_MODE_PALETTE]                 = &WS2812FX::mode_palette;
      _mode[FX_MODE_COLORWAVES]              = &WS2812FX::mode_colorwaves;
      _mode[FX_MODE_LAKE]                    = &WS2812FX::mode_lake;
      _mode[FX_MODE_GLITTER]                 = &WS2812FX::mode_glitter;
      _mode[FX_MODE_METEOR]                  = &WS2812FX::mode_meteor;
      _mode[FX_MODE_METEOR_SMOOTH]           = &WS2812FX::mode_meteor_smooth;
      _mode[FX_MODE_PRIDE_2015]              = &WS2812FX::mode_pride_2015;
      _mode[FX_MODE_RIPPLE_RAINBOW]          = &WS2812FX::mode_ripple_rainbow;
      _mode[FX_MODE_PACIFICA]                = &WS2812FX::mode_pacifica;
      _mode[FX_MODE_SUNRISE]                 = &WS2812FX::mode_sunrise;
      _mode[FX_MODE_SINEWAVE]                = &WS2812FX::mode_sinewave;
      _mode[FX_MODE_FLOW]                    = &WS2812FX::mode_flow;
      _mode[FX_MODE_PHASEDNOISE]             = &WS2812FX::mode_phased_noise;
      _mode[FX_MODE_PHASED]                  = &WS2812FX::mode_phased;
      _mode[FX_MODE_RUNNING_LIGHTS]          = &WS2812FX::mode_running_lights;
      _mode[FX_MODE_RAINBOW_CYCLE]           = &WS2812FX::mode_rainbow_cycle;
      _mode[FX_MODE_MERRY_CHRISTMAS]         = &WS2812FX::mode_merry_christmas;
      _mode[FX_MODE_HALLOWEEN]               = &WS2812FX::mode_halloween;
      // Chase
      _mode[FX_MODE_CHASE_COLOR]             = &WS2812FX::mode_chase_color;
      _mode[FX_MODE_CHASE_RANDOM]            = &WS2812FX::mode_chase_random;
      _mode[FX_MODE_CHASE_RAINBOW]           = &WS2812FX::mode_chase_rainbow;
      _mode[FX_MODE_CHASE_FLASH]             = &WS2812FX::mode_chase_flash;
      _mode[FX_MODE_CHASE_FLASH_RANDOM]      = &WS2812FX::mode_chase_flash_random;
      _mode[FX_MODE_CHASE_RAINBOW_WHITE]     = &WS2812FX::mode_chase_rainbow_white;
      _mode[FX_MODE_THEATER_CHASE]           = &WS2812FX::mode_theater_chase;
      _mode[FX_MODE_THEATER_CHASE_RAINBOW]   = &WS2812FX::mode_theater_chase_rainbow;
      _mode[FX_MODE_TRICOLOR_CHASE]          = &WS2812FX::mode_tricolor_chase;
      _mode[FX_MODE_RANDOM_CHASE]            = &WS2812FX::mode_random_chase;
      _mode[FX_MODE_CIRCUS_COMBUSTUS]        = &WS2812FX::mode_circus_combustus;
      // Breathe/Fade/Pulse
      _mode[FX_MODE_BREATH]                  = &WS2812FX::mode_breath;
      _mode[FX_MODE_FADE]                    = &WS2812FX::mode_fade;
      _mode[FX_MODE_TRICOLOR_FADE]           = &WS2812FX::mode_tricolor_fade;
      _mode[FX_MODE_SPOTS_FADE]              = &WS2812FX::mode_spots_fade;
      // Fireworks
      _mode[FX_MODE_FIREWORKS]               = &WS2812FX::mode_fireworks;
      _mode[FX_MODE_STARBURST]               = &WS2812FX::mode_starburst;
      _mode[FX_MODE_EXPLODING_FIREWORKS]     = &WS2812FX::mode_exploding_fireworks;
      _mode[FX_MODE_RAIN]                    = &WS2812FX::mode_rain;
      // Sparkle/Twinkle
      _mode[FX_MODE_SOLID_GLITTER]           = &WS2812FX::mode_solid_glitter;
      _mode[FX_MODE_POPCORN]                 = &WS2812FX::mode_popcorn;
      _mode[FX_MODE_PLASMA]                  = &WS2812FX::mode_plasma;
      _mode[FX_MODE_FIRE_FLICKER]            = &WS2812FX::mode_fire_flicker;
      _mode[FX_MODE_SPARKLE]                 = &WS2812FX::mode_sparkle;
      _mode[FX_MODE_FLASH_SPARKLE]           = &WS2812FX::mode_flash_sparkle;
      _mode[FX_MODE_HYPER_SPARKLE]           = &WS2812FX::mode_hyper_sparkle;
      _mode[FX_MODE_TWINKLE]                 = &WS2812FX::mode_twinkle;
      _mode[FX_MODE_COLORTWINKLE]            = &WS2812FX::mode_colortwinkle;
      _mode[FX_MODE_TWINKLEFOX]              = &WS2812FX::mode_twinklefox;
      _mode[FX_MODE_TWINKLECAT]              = &WS2812FX::mode_twinklecat;
      _mode[FX_MODE_TWINKLEUP]               = &WS2812FX::mode_twinkleup;
      _mode[FX_MODE_DYNAMIC]                 = &WS2812FX::mode_dynamic;
      _mode[FX_MODE_SAW]                     = &WS2812FX::mode_saw;
      _mode[FX_MODE_DISSOLVE]                = &WS2812FX::mode_dissolve;
      _mode[FX_MODE_DISSOLVE_RANDOM]         = &WS2812FX::mode_dissolve_random;
      _mode[FX_MODE_COLORFUL]                = &WS2812FX::mode_colorful;
      _mode[FX_MODE_TRAFFIC_LIGHT]           = &WS2812FX::mode_traffic_light;
      _mode[FX_MODE_CANDLE]                  = &WS2812FX::mode_candle;
      _mode[FX_MODE_CANDLE_MULTI]            = &WS2812FX::mode_candle_multi;
      _mode[FX_MODE_HALLOWEEN_EYES]          = &WS2812FX::mode_halloween_eyes; 
      #ifdef ENABLE_ADVANCED_EFFECTS   
      // Blink/Strobe
      _mode[FX_MODE_BLINK]                   = &WS2812FX::mode_blink;
      _mode[FX_MODE_BLINK_RAINBOW]           = &WS2812FX::mode_blink_rainbow;
      _mode[FX_MODE_STROBE]                  = &WS2812FX::mode_strobe;
      _mode[FX_MODE_MULTI_STROBE]            = &WS2812FX::mode_multi_strobe;
      _mode[FX_MODE_STROBE_RAINBOW]          = &WS2812FX::mode_strobe_rainbow;
      _mode[FX_MODE_RAINBOW]                 = &WS2812FX::mode_rainbow;
      _mode[FX_MODE_LIGHTNING]               = &WS2812FX::mode_lightning;      
      _mode[FX_MODE_FIRE_2012]               = &WS2812FX::mode_fire_2012;
      _mode[FX_MODE_RAILWAY]                 = &WS2812FX::mode_railway;
      _mode[FX_MODE_HEARTBEAT]               = &WS2812FX::mode_heartbeat;
      // Noise
      _mode[FX_MODE_FILLNOISE8]              = &WS2812FX::mode_fillnoise8;
      _mode[FX_MODE_NOISE16_1]               = &WS2812FX::mode_noise16_1;
      _mode[FX_MODE_NOISE16_2]               = &WS2812FX::mode_noise16_2;
      _mode[FX_MODE_NOISE16_3]               = &WS2812FX::mode_noise16_3;
      _mode[FX_MODE_NOISE16_4]               = &WS2812FX::mode_noise16_4;
      _mode[FX_MODE_NOISEPAL]                = &WS2812FX::mode_noisepal;
      // Scan
      _mode[FX_MODE_SCAN]                    = &WS2812FX::mode_scan;
      _mode[FX_MODE_DUAL_SCAN]               = &WS2812FX::mode_dual_scan;
      _mode[FX_MODE_LARSON_SCANNER]          = &WS2812FX::mode_larson_scanner;
      _mode[FX_MODE_DUAL_LARSON_SCANNER]     = &WS2812FX::mode_dual_larson_scanner;
      _mode[FX_MODE_ICU]                     = &WS2812FX::mode_icu;
      _mode[FX_MODE_RIPPLE]                  = &WS2812FX::mode_ripple;
      _mode[FX_MODE_COMET]                   = &WS2812FX::mode_comet;
      _mode[FX_MODE_CHUNCHUN]                = &WS2812FX::mode_chunchun;
      _mode[FX_MODE_BOUNCINGBALLS]           = &WS2812FX::mode_bouncing_balls;
      _mode[FX_MODE_SINELON]                 = &WS2812FX::mode_sinelon;
      _mode[FX_MODE_SINELON_DUAL]            = &WS2812FX::mode_sinelon_dual;
      _mode[FX_MODE_SINELON_RAINBOW]         = &WS2812FX::mode_sinelon_rainbow;
      _mode[FX_MODE_DRIP]                    = &WS2812FX::mode_drip;
      #endif // ENABLE_ADVANCED_EFFECTS
      
      _brightness = DEFAULT_BRIGHTNESS;
      currentPalette = CRGBPalette16(CRGB::Black);
      targetPalette = CloudColors_p;
      ablMilliampsMax = 850;
      currentMilliamps = 0;
      timebase = 0;
      bus = new NeoPixelWrapper();
      resetSegments();     
    };

    void
      init(bool supportWhite, uint16_t countPixels, bool skipFirst),
      service(void),
      blur(uint8_t),
      fade_out(uint8_t r),
      setMode(uint8_t segid, uint8_t m),
      setColor(uint8_t slot, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0),
      setColor(uint8_t slot, uint32_t c),
      setBrightness(uint8_t b),
      setRange(uint16_t i, uint16_t i2, uint32_t col),
      setShowCallback(show_callback cb),
      setTransitionMode(bool t),
      trigger(void),
      setSegment(uint8_t n, uint16_t start, uint16_t stop, uint8_t grouping = 0, uint8_t spacing = 0),
      resetSegments(),
      setPixelColor(uint16_t n, uint32_t c),
      setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w = 0),
      show(void),
      setRgbwPwm(void);

    bool
      reverseMode = false,
      gammaCorrectBri = false,
      gammaCorrectCol = true,
      applyToAllSelected = true,
      segmentsAreIdentical(Segment* a, Segment* b),
      setEffectConfig(uint8_t m, uint8_t s, uint8_t i, uint8_t p);

    uint8_t
      mainSegment = 0,
      rgbwMode = 3,//RGBW_MODE_DUAL,
      paletteFade = 0,
      paletteBlend = 0,
      colorOrder = 0, // 0 = GRB, 1 = RGB
      milliampsPerLed = 55,
      getBrightness(void),
      getMode(void),
      getSpeed(void),
      getModeCount(void),
      getPaletteCount(void),
      getMaxSegments(void),
      getMainSegmentId(void),
      gamma8(uint8_t),
      get_random_wheel_index(uint8_t);

    uint16_t
      ablMilliampsMax,
      currentMilliamps,
      triwave16(uint16_t);

    uint32_t
      timebase,
      color_wheel(uint8_t),
      color_from_palette(uint16_t, bool mapping, bool wrap, uint8_t mcol, uint8_t pbri = 255),
      color_blend(uint32_t,uint32_t,uint8_t),
      gamma32(uint32_t),
      getLastShow(void),
      getPixelColor(uint16_t),
      getColor(void);

    WS2812FX::Segment&
      getSegment(uint8_t n);

    WS2812FX::Segment_runtime
      getSegmentRuntime(void);

    WS2812FX::Segment*
      getSegments(void);

    // builtin modes
    uint16_t
      mode_static(void),
      mode_blink(void),
      mode_blink_rainbow(void),
      mode_strobe(void),
      mode_strobe_rainbow(void),
      mode_color_wipe(void),
      mode_color_sweep(void),
      mode_color_wipe_random(void),
      mode_color_sweep_random(void),
      mode_random_color(void),
      mode_dynamic(void),
      mode_breath(void),
      mode_fade(void),
      mode_scan(void),
      mode_dual_scan(void),
      mode_theater_chase(void),
      mode_theater_chase_rainbow(void),
      mode_rainbow(void),
      mode_rainbow_cycle(void),
      mode_running_lights(void),
      mode_saw(void),
      mode_twinkle(void),
      mode_dissolve(void),
      mode_dissolve_random(void),
      mode_sparkle(void),
      mode_flash_sparkle(void),
      mode_hyper_sparkle(void),
      mode_multi_strobe(void),
      mode_android(void),
      mode_chase_color(void),
      mode_chase_random(void),
      mode_chase_rainbow(void),
      mode_chase_flash(void),
      mode_chase_flash_random(void),
      mode_chase_rainbow_white(void),
      mode_colorful(void),
      mode_traffic_light(void),
      mode_running_color(void),
      mode_running_red_blue(void),
      mode_running_random(void),
      mode_larson_scanner(void),
      mode_comet(void),
      mode_fireworks(void),
      mode_rain(void),
      mode_merry_christmas(void),
      mode_halloween(void),
      mode_fire_flicker(void),
      mode_gradient(void),
      mode_loading(void),
      mode_police(void),
      mode_police_all(void),
      mode_two_dots(void),
      mode_two_areas(void),
      mode_circus_combustus(void),
      mode_bicolor_chase(void),
      mode_tricolor_chase(void),
      mode_tricolor_wipe(void),
      mode_tricolor_fade(void),
      mode_lightning(void),
      mode_icu(void),
      mode_multi_comet(void),
      mode_dual_larson_scanner(void),
      mode_random_chase(void),
      mode_oscillate(void),
      mode_fire_2012(void),
      mode_pride_2015(void),
      mode_bpm(void),
      mode_juggle(void),
      mode_palette(void),
      mode_colorwaves(void),
      mode_fillnoise8(void),
      mode_noise16_1(void),
      mode_noise16_2(void),
      mode_noise16_3(void),
      mode_noise16_4(void),
      mode_colortwinkle(void),
      mode_lake(void),
      mode_meteor(void),
      mode_meteor_smooth(void),
      mode_railway(void),
      mode_ripple(void),
      mode_twinklefox(void),
      mode_twinklecat(void),
      mode_halloween_eyes(void),
      mode_static_pattern(void),
      mode_tri_static_pattern(void),
      mode_spots(void),
      mode_spots_fade(void),
      mode_glitter(void),
      mode_candle(void),
      mode_starburst(void),
      mode_exploding_fireworks(void),
      mode_bouncing_balls(void),
      mode_sinelon(void),
      mode_sinelon_dual(void),
      mode_sinelon_rainbow(void),
      mode_popcorn(void),
      mode_drip(void),
      mode_plasma(void),
      mode_percent(void),
      mode_ripple_rainbow(void),
      mode_heartbeat(void),
      mode_pacifica(void),
      mode_candle_multi(void),
      mode_solid_glitter(void),
      mode_sunrise(void),
      mode_phased(void),
      mode_twinkleup(void),
      mode_noisepal(void),
      mode_sinewave(void),
      mode_phased_noise(void),
      mode_flow(void),
      mode_chunchun(void);

  private:
    NeoPixelWrapper *bus;

    uint32_t crgb_to_col(CRGB fastled);
    CRGB col_to_crgb(uint32_t);
    CRGBPalette16 currentPalette;
    CRGBPalette16 targetPalette;

    uint32_t now;
    uint16_t _length, _lengthRaw, _virtualSegmentLength;
    uint16_t _rand16seed;
    uint8_t _brightness;
    static uint16_t _usedSegmentData;

    void load_gradient_palette(uint8_t);
    void handle_palette(void);
    void fill(uint32_t);

    bool
      _useRgbw = false,
      _skipFirstMode,
      _triggered;

    mode_ptr _mode[MODE_COUNT]; // SRAM footprint: 4 bytes per element

    // show_callback _callback = nullptr;

    // mode helper functions
    uint16_t
      blink(uint32_t, uint32_t, bool strobe, bool),
      candle(bool),
      color_wipe(bool, bool),
      scan(bool),
      theater_chase(uint32_t, uint32_t, bool),
      running_base(bool),
      larson_scanner(bool),
      sinelon_base(bool,bool),
      dissolve(uint32_t),
      chase(uint32_t, uint32_t, uint32_t, bool),
      gradient_base(bool),
      ripple_base(bool),
      police_base(uint32_t, uint32_t, bool),
      running(uint32_t, uint32_t),
      tricolor_chase(uint32_t, uint32_t),
      twinklefox_base(bool),
      spots_base(uint16_t),
      phased_base(uint8_t);

    CRGB twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat);
    CRGB pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
    
    uint32_t _lastPaletteChange = 0;
    uint32_t _lastShow = 0;
    
    #ifdef WLED_USE_ANALOG_LEDS
    uint32_t _analogLastShow = 0;
    RgbwColor _analogLastColor = 0;
    uint8_t _analogLastBri = 0;
    #endif
    
    uint8_t _segment_index = 0;
    uint8_t _segment_index_palette_last = 99;
    segment _segments[MAX_NUM_SEGMENTS] = { // SRAM footprint: 24 bytes per element
      // start, stop, speed, intensity, palette, mode, options, grouping, spacing, opacity (unused), color[]
      { 0, 7, DEFAULT_SPEED, 128, 0, DEFAULT_MODE, NO_OPTIONS, 1, 0, 255, {DEFAULT_COLOR}}
    };
    segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 28 bytes per element
    friend class Segment_runtime;

    uint16_t realPixelIndex(uint16_t i);
};


// const char JSON_palette_names[] PROGMEM = R"=====([
// "Default","* Random Cycle","* Color 1","* Colors 1&2","* Color Gradient","* Colors Only","Party","Cloud","Lava","Ocean",
// "Forest","Rainbow","Rainbow Bands","Sunset","Rivendell","Breeze","Red & Blue","Yellowout","Analogous","Splash",
// "Pastel","Sunset 2","Beech","Vintage","Departure","Landscape","Beach","Sherbet","Hult","Hult 64",
// "Drywet","Jul","Grintage","Rewhi","Tertiary","Fire","Icefire","Cyane","Light Pink","Autumn",
// "Magenta","Magred","Yelmag","Yelblu","Orange & Teal","Tiamat","April Night","Orangery","C9","Sakura",
// "Aurora","Atlantica"
// ])=====";

// #include ""


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
// const char* GetEffectsModeName(char* buffer);
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
void handleSettingsSet(AsyncWebServerRequest *request, byte subPage);
bool handleSet(AsyncWebServerRequest *request, const String& req);
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

byte lastRandomIndex = 0;        // used to save last random color so the new one is not the same

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
// IPAddress realtimeIP = (0, 0, 0, 0);
unsigned long realtimeTimeout = 0;
uint16_t tpmFirstFrameSize = 0;

// Temp buffer
char* obuf;
uint16_t olen = 0;

// presets
uint16_t savedPresets = 0;
int8_t currentPreset = -1;
bool isPreset = false;

byte errorFlag = 0;

String messageHead, messageSub;
byte optionType;


    void LightSetPWMOutputsArray10bit(const uint16_t *cur_col_10);

    void FadeToNewColour(RgbcctColor targetColor, uint16_t _time_to_newcolour,  RgbcctColor fromcolor = RgbcctColor(0));
                
    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(void);

    #define LST_MAX 5
    
    struct AnimationColours
    {
      RgbcctColor StartingColor;
      RgbcctColor DesiredColour;
    };
    AnimationColours animation_colours;

    void BlendAnimUpdate(const AnimationParam& param);

    #define NEO_ANIMATION_TIMEBASE NEO_MILLISECONDS
    RgbcctColor output_colour;
    void LightUpdate();
    void SubTask_Animation();
    void LightSetPWMOutputsRgbcctColor(RgbcctColor colour);

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

