#ifndef MWLEDEFFECTS_prog1_H
#define MWLEDEFFECTS_prog1_H

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR


DEFINE_PGM_CTR(PM_EFFECTS_REGION_SLOW_FADE__NAME_CTR)                          "SLOW_FADE";
DEFINE_PGM_CTR(PM_EFFECTS_REGION_TWINKLE_FLASH__NAME_CTR)                      "TWINKLE_FLASH";
DEFINE_PGM_CTR(PM_EFFECTS_REGION_COLOUR_SELECT__NAME_CTR)                      D_EFFECTS_REGION__COLOUR_SELECT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_REGION_ANIMATE__NAME_CTR)                            D_EFFECTS_REGION__ANIMATE__NAME_CTR;


/**
 * Effects Function names (Giving multiple names using delimeter)
 * */
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR)                        D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__STATIC_PALETTE__NAME_CTR)                   "Static Palette|Static Glow|Static";     
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SPANNED_PALETTE__NAME_CTR)   "Spanned Palette";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SEQUENTIAL__NAME_CTR)                       "Sequential"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SOLID_COLOUR__NAME_CTR)                     D_EFFECTS_FUNCTION__SOLID_COLOUR__NAME_CTR;//"Solid RGBCCT"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__STEPPING_PALETTE__NAME_CTR)                  "Step Palette";


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__POPPING_DECAY_PALETTE__NAME_CTR)                    "Popping Decay Palette";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__POPPING_DECAY_RANDOM__NAME_CTR)                  "Popping Decay Random";
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC


DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SLOW_FADE_BRIGHTNESS_ALL__NAME_CTR)         "SLOW_FADE_BRIGHTNESS_ALL";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SLOW_FADE_SATURATION_ALL__NAME_CTR)         "SLOW_FADE_SATURATION_ALL";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SLOW_FADE_BRIGHTNESS_RANDOM__NAME_CTR)      "SLOW_FADE_BRIGHTNESS_RANDOM";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SLOW_FADE_SATURATION_RANDOM__NAME_CTR)      "SLOW_FADE_SATURATION_RANDOM";
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__FLASH_TWINKLE_RANDOM__NAME_CTR)             "FLASH_TWINKLE_RANDOM";                   
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__FLASH_TWINKLE_SEQUENTIAL__NAME_CTR)         "FLASH_TWINKLE_SEQUENTIAL";                          
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__FLASH_GLIMMER_RANDOM__NAME_CTR)             "FLASH_GLIMMER_RANDOM"; 
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__FIREPLACE_1D_01__NAME_CTR)                  D_EFFECTS_FUNCTION__FIREPLACE_1D_01__NAME_CTR;//   "FirePlace01";     
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__TESTER__NAME_CTR)                           "Tester";


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SUNPOSITIONS__SUNRISE_ALARM_01__NAME_CTR)                                        D_EFFECTS_FUNCTION__SUNPOSITIONS__SUNRISE_ALARM_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SUNPOSITIONS__AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__NAME_CTR)                  D_EFFECTS_FUNCTION__SUNPOSITIONS__AZIMUTH_SELECTS_GRADIENT_OF_PALETTE_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SUNPOSITIONS__SUNSET_BLENDED_PALETTES_01__NAME_CTR)                              D_EFFECTS_FUNCTION__SUNPOSITIONS__SUNSET_BLENDED_PALETTES_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SUNPOSITIONS__DRAWSUN_1D_ELEVATION_01__NAME_CTR)                                 D_EFFECTS_FUNCTION__SUNPOSITIONS__DRAWSUN_1D_ELEVATION_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SUNPOSITIONS__DRAWSUN_1D_AZIMUTH_01__NAME_CTR)                                   D_EFFECTS_FUNCTION__SUNPOSITIONS__DRAWSUN_1D_AZIMUTH_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SUNPOSITIONS__DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__NAME_CTR)                     D_EFFECTS_FUNCTION__SUNPOSITIONS__DRAWSUN_2D_ELEVATION_AND_AZIMUTH_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SUNPOSITIONS__WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__NAME_CTR)      D_EFFECTS_FUNCTION__SUNPOSITIONS__WHITE_COLOUR_TEMPERATURE_CCT_BASED_ON_ELEVATION_01__NAME_CTR;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS

// My effects

DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__NAME_CTR)    D_EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION_BLENDED__NAME_CTR)   D_EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION_BLENDED__NAME_CTR;

DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01__NAME_CTR) D_EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01__NAME_CTR;


DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__ROTATING_PALETTE_NEW__NAME_CTR)   D_EFFECTS_FUNCTION__ROTATING_PALETTE_NEW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__ROTATING_PALETTE__NAME_CTR)   D_EFFECTS_FUNCTION__ROTATING_PALETTE__NAME_CTR;

DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__PALETTE_COLOUR_FADE_SATURATION__NAME_CTR)               D_EFFECTS_FUNCTION__PALETTE_COLOUR_FADE_SATURATION__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__NAME_CTR)               D_EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__NAME_CTR;

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__7SEGMENTDISPLAY__CLOCKTIME_01__NAME_CTR)         D_EFFECTS_FUNCTION__7SEGMENTDISPLAY__CLOCKTIME_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__7SEGMENTDISPLAY__CLOCKTIME_02__NAME_CTR)         D_EFFECTS_FUNCTION__7SEGMENTDISPLAY__CLOCKTIME_02__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__7SEGMENTDISPLAY__MANUALNUMBER_01__NAME_CTR)      D_EFFECTS_FUNCTION__7SEGMENTDISPLAY__MANUALNUMBER_01__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__7SEGMENTDISPLAY__MANUALSTRING_01__NAME_CTR)      D_EFFECTS_FUNCTION__7SEGMENTDISPLAY__MANUALSTRING_01__NAME_CTR;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK




DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__BLEND_TWO_PALETTES__NAME_CTR) D_EFFECTS_FUNCTION__BLEND_TWO_PALETTES__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__NAME_CTR) D_EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__NAME_CTR) D_EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__NAME_CTR) D_EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__NAME_CTR;

DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SHIMMERING_PALETTE__NAME_CTR)     D_EFFECTS_FUNCTION__SHIMMERING_PALETTE__NAME_CTR;

DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__SHIMMERING_TWO_PALETTES__NAME_CTR)     D_EFFECTS_FUNCTION__SHIMMERING_PALETTE_TO_ANOTHER__NAME_CTR;

#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__NAME_CTR)  D_EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__NAME_CTR; 
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS


#ifdef ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT__NAME_CTR)   D_EFFECTS_FUNCTION__BORDER_WALLPAPER__TWOCOLOUR_GRADIENT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT__NAME_CTR)  D_EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_GRADIENT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_SOLID__NAME_CTR)     D_EFFECTS_FUNCTION__BORDER_WALLPAPER__FOURCOLOUR_SOLID__NAME_CTR;
#endif // ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__BORDER_WALLPAPERS


/**
 * @brief WLED Imported effects
 * 
 */


// Doing this with memcmp_p until effects are fully designed, then for memory reasons
// move to "dlist", "SearchForTextIndexedID" and "GetTextIndexed_P"
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_STATIC__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_STATIC__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_BLINK__NAME_CTR)                  D_EFFECTS_FUNCTION__WLED_BLINK__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_BREATH__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_BREATH__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COLOR_WIPE__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_COLOR_WIPE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__NAME_CTR)      D_EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RANDOM_COLOR__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_RANDOM_COLOR__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COLOR_SWEEP__NAME_CTR)            D_EFFECTS_FUNCTION__WLED_COLOR_SWEEP__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_DYNAMIC__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_DYNAMIC__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RAINBOW__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_RAINBOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SCAN__NAME_CTR)                   D_EFFECTS_FUNCTION__WLED_SCAN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_DUAL_SCAN__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_DUAL_SCAN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_FADE__NAME_CTR)                   D_EFFECTS_FUNCTION__WLED_FADE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_THEATER_CHASE__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_THEATER_CHASE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_THEATER_CHASE_RAINBOW__NAME_CTR)  D_EFFECTS_FUNCTION__WLED_THEATER_CHASE_RAINBOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SAW__NAME_CTR)                    D_EFFECTS_FUNCTION__WLED_SAW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TWINKLE__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_TWINKLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_DISSOLVE__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_DISSOLVE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__NAME_CTR)        D_EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SPARKLE__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_SPARKLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_STROBE__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_STROBE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_MULTI_STROBE__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_MULTI_STROBE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_ANDROID__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_ANDROID__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CHASE_COLOR__NAME_CTR)            D_EFFECTS_FUNCTION__WLED_CHASE_COLOR__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CHASE_RANDOM__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_CHASE_RANDOM__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CHASE_FLASH__NAME_CTR)            D_EFFECTS_FUNCTION__WLED_CHASE_FLASH__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__NAME_CTR)     D_EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__NAME_CTR)    D_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COLORFUL__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_COLORFUL__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__NAME_CTR)     D_EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RUNNING_COLOR__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_RUNNING_COLOR__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RUNNING_RED_BLUE__NAME_CTR)       D_EFFECTS_FUNCTION__WLED_RUNNING_RED_BLUE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_LARSON_SCANNER__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_LARSON_SCANNER__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COMET__NAME_CTR)                  D_EFFECTS_FUNCTION__WLED_COMET__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_FIREWORKS__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_FIREWORKS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RAIN__NAME_CTR)                   D_EFFECTS_FUNCTION__WLED_RAIN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_MERRY_CHRISTMAS__NAME_CTR)        D_EFFECTS_FUNCTION__WLED_MERRY_CHRISTMAS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_FIRE_FLICKER__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_FIRE_FLICKER__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_GRADIENT__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_GRADIENT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_LOADING__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_LOADING__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_POLICE__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_POLICE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_POLICE_ALL__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_POLICE_ALL__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TWO_DOTS__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_TWO_DOTS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TWO_AREAS__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_TWO_AREAS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CIRCUS_COMBUSTUS__NAME_CTR)       D_EFFECTS_FUNCTION__WLED_CIRCUS_COMBUSTUS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_HALLOWEEN__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_HALLOWEEN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TRICOLOR_CHASE__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_TRICOLOR_CHASE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TRICOLOR_FADE__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_TRICOLOR_FADE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_LIGHTNING__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_LIGHTNING__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_ICU__NAME_CTR)                    D_EFFECTS_FUNCTION__WLED_ICU__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_MULTI_COMET__NAME_CTR)            D_EFFECTS_FUNCTION__WLED_MULTI_COMET__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__NAME_CTR)    D_EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RANDOM_CHASE__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_RANDOM_CHASE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_OSCILLATE__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_OSCILLATE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_PRIDE_2015__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_PRIDE_2015__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_JUGGLE__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_JUGGLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_PALETTE__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_PALETTE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_FIRE_2012__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_FIRE_2012__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COLORWAVES__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_COLORWAVES__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_BPM__NAME_CTR)                    D_EFFECTS_FUNCTION__WLED_BPM__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_FILLNOISE8__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_FILLNOISE8__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_NOISE16_1__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_NOISE16_1__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_NOISE16_2__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_NOISE16_2__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_NOISE16_3__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_NOISE16_3__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_NOISE16_4__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_NOISE16_4__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_COLORTWINKLE__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_COLORTWINKLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_LAKE__NAME_CTR)                   D_EFFECTS_FUNCTION__WLED_LAKE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_METEOR__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_METEOR__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RAILWAY__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_RAILWAY__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RIPPLE__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_RIPPLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TWINKLEFOX__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_TWINKLEFOX__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TWINKLECAT__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_TWINKLECAT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_HALLOWEEN_EYES__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_HALLOWEEN_EYES__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_STATIC_PATTERN__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_STATIC_PATTERN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__NAME_CTR)     D_EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SPOTS__NAME_CTR)                  D_EFFECTS_FUNCTION__WLED_SPOTS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SPOTS_FADE__NAME_CTR)             D_EFFECTS_FUNCTION__WLED_SPOTS_FADE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_GLITTER__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_GLITTER__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CANDLE__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_CANDLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_STARBURST__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_STARBURST__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_STARBURST_GLOWS__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_STARBURST_GLOWS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS__NAME_CTR)    D_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS_NO_LAUNCH__NAME_CTR)    D_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS_NO_LAUNCH__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SINELON__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_SINELON__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SINELON_DUAL__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_SINELON_DUAL__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__NAME_CTR)        D_EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_POPCORN__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_POPCORN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_DRIP__NAME_CTR)                   D_EFFECTS_FUNCTION__WLED_DRIP__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_PLASMA__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_PLASMA__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_PERCENT__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_PERCENT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__NAME_CTR)         D_EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_HEARTBEAT__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_HEARTBEAT__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_PACIFICA__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_PACIFICA__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CANDLE_MULTI__NAME_CTR)           D_EFFECTS_FUNCTION__WLED_CANDLE_MULTI__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SOLID_GLITTER__NAME_CTR)          D_EFFECTS_FUNCTION__WLED_SOLID_GLITTER__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SUNRISE__NAME_CTR)                D_EFFECTS_FUNCTION__WLED_SUNRISE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_PHASED__NAME_CTR)                 D_EFFECTS_FUNCTION__WLED_PHASED__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TWINKLEUP__NAME_CTR)              D_EFFECTS_FUNCTION__WLED_TWINKLEUP__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_NOISEPAL__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_NOISEPAL__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_SINEWAVE__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_SINEWAVE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_PHASEDNOISE__NAME_CTR)            D_EFFECTS_FUNCTION__WLED_PHASEDNOISE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_FLOW__NAME_CTR)                   D_EFFECTS_FUNCTION__WLED_FLOW__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CHUNCHUN__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_CHUNCHUN__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_BLENDS__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_BLENDS__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_TV_SIMULATOR__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_TV_SIMULATOR__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__NAME_CTR;
DEFINE_PGM_CTR(PM_EFFECTS_FUNCTION__WLED_CANDLE_MULTIPLE__NAME_CTR)               D_EFFECTS_FUNCTION__WLED_CANDLE_MULTIPLE__NAME_CTR;


#endif // USE_MODULE_LIGHTS_ANIMATOR


#endif

