#pragma once

/**
 * Interface
 * */

/**
 * AnimatorLight
 * */
#define D_EFFECTS_FUNCTION__SOLID_COLOUR__NAME_CTR      "Solid RGBCCT|Solid" // "Solid Colour"
#define D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR         "Slow Glow|Blend" /// I need to add that no only does it check the individual ones, but also the entire string so this can be used in templates fully
#define D_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_01__NAME_CTR "Sun Elevation RGBCCT Solid Palette 01"
#define D_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_RGBCCT_PALETTE_INDEXED_POSITIONS_WITH_AUGMENTED_TRANSITIONS_01__NAME_CTR "Sun Elevation RGBCCT Solid Palette Augmented 01"
#define D_EFFECTS_FUNCTION__SUNPOSITIONS_ELEVATION_ONLY_CONTROLLED_CCT_TEMPERATURE_01__NAME_CTR "Sun Elevation CCT Temp 01"
#define D_EFFECTS_FUNCTION__FIREPLACE_1D_01__NAME_CTR "FirePlace01"

#define D_EFFECTS_FUNCTION__SHIMMERING_PALETTE__NAME_CTR      "Shimmering Palette|Candle Multi"
#define D_EFFECTS_FUNCTION__SHIMMERING_PALETTE_TO_ANOTHER__NAME_CTR      "Shimmering Two Palettes"




#define D_EFFECTS_FUNCTION__WLED_STATIC__NAME_CTR                  "Static Single Colour"
#define D_EFFECTS_FUNCTION__WLED_STATIC_PATTERN__NAME_CTR          "Static Pattern Lit"
#define D_EFFECTS_FUNCTION__WLED_TRI_STATIC_PATTERN__NAME_CTR      "Static Pattern Tri"
#define D_EFFECTS_FUNCTION__WLED_BLINK__NAME_CTR                   "Blink"
#define D_EFFECTS_FUNCTION__WLED_BREATH__NAME_CTR                  "Breathe"
#define D_EFFECTS_FUNCTION__WLED_COLOR_WIPE__NAME_CTR              "Wipe"
#define D_EFFECTS_FUNCTION__WLED_COLOR_WIPE_RANDOM__NAME_CTR       "Wipe Random"
#define D_EFFECTS_FUNCTION__WLED_RANDOM_COLOR__NAME_CTR            "Random Colors"
#define D_EFFECTS_FUNCTION__WLED_COLOR_SWEEP__NAME_CTR             "Sweep"
#define D_EFFECTS_FUNCTION__WLED_DYNAMIC__NAME_CTR                 "Dynamic"
#define D_EFFECTS_FUNCTION__WLED_RAINBOW__NAME_CTR                 "Colorloop"
#define D_EFFECTS_FUNCTION__WLED_RAINBOW_CYCLE__NAME_CTR           "Rainbow"
#define D_EFFECTS_FUNCTION__WLED_SCAN__NAME_CTR                    "Scan"
#define D_EFFECTS_FUNCTION__WLED_DUAL_SCAN__NAME_CTR               "Scan Dual"
#define D_EFFECTS_FUNCTION__WLED_FADE__NAME_CTR                    "Fade"
#define D_EFFECTS_FUNCTION__WLED_THEATER_CHASE__NAME_CTR           "Theater"
#define D_EFFECTS_FUNCTION__WLED_THEATER_CHASE_RAINBOW__NAME_CTR   "Theater Rainbow"
#define D_EFFECTS_FUNCTION__WLED_RUNNING_LIGHTS__NAME_CTR          "Running"
#define D_EFFECTS_FUNCTION__WLED_SAW__NAME_CTR                     "Saw"
#define D_EFFECTS_FUNCTION__WLED_TWINKLE__NAME_CTR                 "Twinkle"
#define D_EFFECTS_FUNCTION__WLED_DISSOLVE__NAME_CTR                "Dissolve"
#define D_EFFECTS_FUNCTION__WLED_DISSOLVE_RANDOM__NAME_CTR         "Dissolve Rnd"
#define D_EFFECTS_FUNCTION__WLED_SPARKLE__NAME_CTR                 "Sparkle"
#define D_EFFECTS_FUNCTION__WLED_FLASH_SPARKLE__NAME_CTR           "Sparkle Dark"
#define D_EFFECTS_FUNCTION__WLED_HYPER_SPARKLE__NAME_CTR           "Sparkle+"
#define D_EFFECTS_FUNCTION__WLED_STROBE__NAME_CTR                  "Strobe"
#define D_EFFECTS_FUNCTION__WLED_STROBE_RAINBOW__NAME_CTR          "Strobe Rainbow"
#define D_EFFECTS_FUNCTION__WLED_MULTI_STROBE__NAME_CTR            "Strobe Mega"
#define D_EFFECTS_FUNCTION__WLED_BLINK_RAINBOW__NAME_CTR           "Blink Rainbow"
#define D_EFFECTS_FUNCTION__WLED_ANDROID__NAME_CTR                 "Android"
#define D_EFFECTS_FUNCTION__WLED_CHASE_COLOR__NAME_CTR             "Chase"
#define D_EFFECTS_FUNCTION__WLED_CHASE_RANDOM__NAME_CTR            "Chase Random"
#define D_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW__NAME_CTR           "Chase Rainbow"
#define D_EFFECTS_FUNCTION__WLED_CHASE_FLASH__NAME_CTR             "Chase Flash"
#define D_EFFECTS_FUNCTION__WLED_CHASE_FLASH_RANDOM__NAME_CTR      "Chase Flash Rnd"
#define D_EFFECTS_FUNCTION__WLED_CHASE_RAINBOW_WHITE__NAME_CTR     "Rainbow Runner"
#define D_EFFECTS_FUNCTION__WLED_COLORFUL__NAME_CTR                "Colorful"
#define D_EFFECTS_FUNCTION__WLED_TRAFFIC_LIGHT__NAME_CTR           "Traffic Light"
#define D_EFFECTS_FUNCTION__WLED_COLOR_SWEEP_RANDOM__NAME_CTR      "Sweep Random"
#define D_EFFECTS_FUNCTION__WLED_RUNNING_COLOR__NAME_CTR           "Running 2"
#define D_EFFECTS_FUNCTION__WLED_RUNNING_RED_BLUE__NAME_CTR        "Red & Blue"
#define D_EFFECTS_FUNCTION__WLED_RUNNING_RANDOM__NAME_CTR          "Stream"
#define D_EFFECTS_FUNCTION__WLED_LARSON_SCANNER__NAME_CTR          "Scanner"
#define D_EFFECTS_FUNCTION__WLED_COMET__NAME_CTR                   "Lighthouse"
#define D_EFFECTS_FUNCTION__WLED_FIREWORKS__NAME_CTR               "Fireworks"
#define D_EFFECTS_FUNCTION__WLED_RAIN__NAME_CTR                    "Rain"
#define D_EFFECTS_FUNCTION__WLED_MERRY_CHRISTMAS__NAME_CTR         "Merry Christmas"
#define D_EFFECTS_FUNCTION__WLED_FIRE_FLICKER__NAME_CTR            "Fire Flicker"
#define D_EFFECTS_FUNCTION__WLED_GRADIENT__NAME_CTR                "Gradient"
#define D_EFFECTS_FUNCTION__WLED_LOADING__NAME_CTR                 "Loading"
#define D_EFFECTS_FUNCTION__WLED_POLICE__NAME_CTR                  "Police"
#define D_EFFECTS_FUNCTION__WLED_POLICE_ALL__NAME_CTR              "Police All"
#define D_EFFECTS_FUNCTION__WLED_TWO_DOTS__NAME_CTR                "Two Dots"
#define D_EFFECTS_FUNCTION__WLED_TWO_AREAS__NAME_CTR               "Two Areas"
#define D_EFFECTS_FUNCTION__WLED_CIRCUS_COMBUSTUS__NAME_CTR        "Circus"
#define D_EFFECTS_FUNCTION__WLED_HALLOWEEN__NAME_CTR               "Halloween"
#define D_EFFECTS_FUNCTION__WLED_TRICOLOR_CHASE__NAME_CTR          "Tri Chase"
#define D_EFFECTS_FUNCTION__WLED_TRICOLOR_WIPE__NAME_CTR           "Tri Wipe"
#define D_EFFECTS_FUNCTION__WLED_TRICOLOR_FADE__NAME_CTR           "Tri Fade"
#define D_EFFECTS_FUNCTION__WLED_LIGHTNING__NAME_CTR               "Lightning"
#define D_EFFECTS_FUNCTION__WLED_ICU__NAME_CTR                     "ICU"
#define D_EFFECTS_FUNCTION__WLED_MULTI_COMET__NAME_CTR             "Multi Comet"
#define D_EFFECTS_FUNCTION__WLED_DUAL_LARSON_SCANNER__NAME_CTR     "Scanner Dual"
#define D_EFFECTS_FUNCTION__WLED_RANDOM_CHASE__NAME_CTR            "Stream 2"
#define D_EFFECTS_FUNCTION__WLED_OSCILLATE__NAME_CTR               "Oscillate"
#define D_EFFECTS_FUNCTION__WLED_PRIDE_2015__NAME_CTR              "Pride 2015"
#define D_EFFECTS_FUNCTION__WLED_JUGGLE__NAME_CTR                  "Juggle"
#define D_EFFECTS_FUNCTION__WLED_PALETTE__NAME_CTR                 "Palette"
#define D_EFFECTS_FUNCTION__WLED_FIRE_2012__NAME_CTR               "Fire 2012"
#define D_EFFECTS_FUNCTION__WLED_COLORWAVES__NAME_CTR              "Colorwaves"
#define D_EFFECTS_FUNCTION__WLED_BPM__NAME_CTR                     "Bpm"
#define D_EFFECTS_FUNCTION__WLED_FILLNOISE8__NAME_CTR              "Fill Noise"
#define D_EFFECTS_FUNCTION__WLED_NOISE16_1__NAME_CTR               "Noise 1"
#define D_EFFECTS_FUNCTION__WLED_NOISE16_2__NAME_CTR               "Noise 2"
#define D_EFFECTS_FUNCTION__WLED_NOISE16_3__NAME_CTR               "Noise 3"
#define D_EFFECTS_FUNCTION__WLED_NOISE16_4__NAME_CTR               "Noise 4"
#define D_EFFECTS_FUNCTION__WLED_COLORTWINKLE__NAME_CTR            "Colortwinkles"
#define D_EFFECTS_FUNCTION__WLED_LAKE__NAME_CTR                    "Lake"
#define D_EFFECTS_FUNCTION__WLED_METEOR__NAME_CTR                  "Meteor"
#define D_EFFECTS_FUNCTION__WLED_METEOR_SMOOTH__NAME_CTR           "Meteor Smooth"
#define D_EFFECTS_FUNCTION__WLED_RAILWAY__NAME_CTR                 "Railway"
#define D_EFFECTS_FUNCTION__WLED_RIPPLE__NAME_CTR                  "Ripple"
#define D_EFFECTS_FUNCTION__WLED_TWINKLEFOX__NAME_CTR              "Twinklefox"
#define D_EFFECTS_FUNCTION__WLED_TWINKLECAT__NAME_CTR              "Twinklecat"
#define D_EFFECTS_FUNCTION__WLED_HALLOWEEN_EYES__NAME_CTR          "Halloween Eyes"
#define D_EFFECTS_FUNCTION__WLED_SPOTS__NAME_CTR                   "Spots"
#define D_EFFECTS_FUNCTION__WLED_SPOTS_FADE__NAME_CTR              "Spots Fade"
#define D_EFFECTS_FUNCTION__WLED_GLITTER__NAME_CTR                 "Glitter"
#define D_EFFECTS_FUNCTION__WLED_CANDLE__NAME_CTR                  "Candle"
#define D_EFFECTS_FUNCTION__WLED_STARBURST__NAME_CTR               "Fireworks Starburst"
#define D_EFFECTS_FUNCTION__WLED_STARBURST_GLOWS__NAME_CTR               "Fireworks Starburst Glows"
#define D_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS__NAME_CTR     "Fireworks 1D"
#define D_EFFECTS_FUNCTION__WLED_EXPLODING_FIREWORKS_NO_LAUNCH__NAME_CTR "Fireworks 1D No Launch"
#define D_EFFECTS_FUNCTION__WLED_BOUNCINGBALLS__NAME_CTR           "Bouncing Balls"
#define D_EFFECTS_FUNCTION__WLED_SINELON__NAME_CTR                 "Sinelon"
#define D_EFFECTS_FUNCTION__WLED_SINELON_DUAL__NAME_CTR            "Sinelon Dual"
#define D_EFFECTS_FUNCTION__WLED_SINELON_RAINBOW__NAME_CTR         "Sinelon Rainbow"
#define D_EFFECTS_FUNCTION__WLED_POPCORN__NAME_CTR                 "Popcorn"
#define D_EFFECTS_FUNCTION__WLED_DRIP__NAME_CTR                    "Drip"
#define D_EFFECTS_FUNCTION__WLED_PLASMA__NAME_CTR                  "Plasma"
#define D_EFFECTS_FUNCTION__WLED_PERCENT__NAME_CTR                 "Percent"
#define D_EFFECTS_FUNCTION__WLED_RIPPLE_RAINBOW__NAME_CTR          "Ripple Rainbow"
#define D_EFFECTS_FUNCTION__WLED_HEARTBEAT__NAME_CTR               "Heartbeat"
#define D_EFFECTS_FUNCTION__WLED_PACIFICA__NAME_CTR                "Pacifica"
#define D_EFFECTS_FUNCTION__WLED_CANDLE_MULTI__NAME_CTR            "Candle Multi"
#define D_EFFECTS_FUNCTION__WLED_SOLID_GLITTER__NAME_CTR           "Solid Glitter"
#define D_EFFECTS_FUNCTION__WLED_SUNRISE__NAME_CTR                 "Sunrise"
#define D_EFFECTS_FUNCTION__WLED_PHASED__NAME_CTR                  "Phased"
#define D_EFFECTS_FUNCTION__WLED_TWINKLEUP__NAME_CTR               "Twinkleup"
#define D_EFFECTS_FUNCTION__WLED_NOISEPAL__NAME_CTR                "Noise Pal"
#define D_EFFECTS_FUNCTION__WLED_SINEWAVE__NAME_CTR                "Sine"
#define D_EFFECTS_FUNCTION__WLED_PHASEDNOISE__NAME_CTR             "Phased Noise"
#define D_EFFECTS_FUNCTION__WLED_FLOW__NAME_CTR                    "Flow"
#define D_EFFECTS_FUNCTION__WLED_CHUNCHUN__NAME_CTR                "Chunchun"
#define D_EFFECTS_FUNCTION__WLED_BLENDS__NAME_CTR "Blends WLED"
#define D_EFFECTS_FUNCTION__WLED_TV_SIMULATOR__NAME_CTR "TV Simulator"
#define D_EFFECTS_FUNCTION__WLED_CANDLE_SINGLE__NAME_CTR      "Candle Single"
#define D_EFFECTS_FUNCTION__WLED_CANDLE_MULTIPLE__NAME_CTR    "Candle Multiple"


#define D_EFFECTS_FUNCTION__ROTATING_PALETTE_NEW__NAME_CTR "Rotating Palette New"
#define D_EFFECTS_FUNCTION__ROTATING_PALETTE__NAME_CTR "Rotating Palette"
#define D_EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION__NAME_CTR "Rotating Previous"
#define D_EFFECTS_FUNCTION__ROTATING_PREVIOUS_ANIMATION_BLENDED__NAME_CTR "Rotating Blended Previous"

#define D_EFFECTS_FUNCTION__CHRISTMAS_MUSICAL__01__NAME_CTR "Chrimstas Musical 01"



#define D_EFFECTS_FUNCTION__PALETTE_COLOUR_FADE_SATURATION__NAME_CTR "Fade Palette Saturation"
#define D_EFFECTS_FUNCTION__SHIMMERING_PALETTE_SATURATION__NAME_CTR "Shimmering Palette Saturation"

#define D_EFFECTS_FUNCTION__SEVEN_SEGMENT_DISPLAY_MANUAL_NUMBER__NAME_CTR "Seven-Segment Manual"

#define D_EFFECTS_FUNCTION__LCD_CLOCK_BASIC_01__NAME_CTR        "Clock Basic 01"

#define D_EFFECTS_FUNCTION__NOTIFICATION_STATIC_ON__NAME_CTR "Notification On"


#define D_EFFECTS_FUNCTION__BLEND_TWO_PALETTES__NAME_CTR                            "Blend Two Palettes"
#define D_EFFECTS_FUNCTION__TWINKLE_PALETTE_SEC_ON_ORDERED_PALETTE_PRI__NAME_CTR    "Twinkle Palette Two on One"
#define D_EFFECTS_FUNCTION__TWINKLE_DECAYING_PALETTE__NAME_CTR                      "Twinkle Decaying Palette"
#define D_EFFECTS_FUNCTION__STATIC_GRADIENT_PALETTE__NAME_CTR                       "Static Gradient Palette"








