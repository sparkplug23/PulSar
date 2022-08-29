#ifndef __INC_COLORPALETTES_H
#define __INC_COLORPALETTES_H
#define FASTLED_INTERNAL
#include "FastLED.h"
#include "colorutils.h"
#include "colorpalettes.h"

FASTLED_USING_NAMESPACE


// Preset color schemes, such as they are.

// These schemes are all declared as "PROGMEM", meaning
// that they won't take up SRAM on AVR chips until used.
// Furthermore, the compiler won't even include these
// in your PROGMEM (flash) storage unless you specifically
// use each one, so you only 'pay for' those you actually use.


extern const TProgmemRGBPalette16 CloudColors_p FL_PROGMEM =
{
    HTMLColorCode::Blue,
    HTMLColorCode::DarkBlue,
    HTMLColorCode::DarkBlue,
    HTMLColorCode::DarkBlue,

    HTMLColorCode::DarkBlue,
    HTMLColorCode::DarkBlue,
    HTMLColorCode::DarkBlue,
    HTMLColorCode::DarkBlue,

    HTMLColorCode::Blue,
    HTMLColorCode::DarkBlue,
    HTMLColorCode::SkyBlue,
    HTMLColorCode::SkyBlue,

    HTMLColorCode::LightBlue,
    HTMLColorCode::White,
    HTMLColorCode::LightBlue,
    HTMLColorCode::SkyBlue
};

extern const TProgmemRGBPalette16 LavaColors_p FL_PROGMEM =
{
    HTMLColorCode::Black,
    HTMLColorCode::Maroon,
    HTMLColorCode::Black,
    HTMLColorCode::Maroon,

    HTMLColorCode::DarkRed,
    HTMLColorCode::Maroon,
    HTMLColorCode::DarkRed,

    HTMLColorCode::DarkRed,
    HTMLColorCode::DarkRed,
    HTMLColorCode::Red,
    HTMLColorCode::Orange,

    HTMLColorCode::White,
    HTMLColorCode::Orange,
    HTMLColorCode::Red,
    HTMLColorCode::DarkRed
};


extern const TProgmemRGBPalette16 OceanColors_p FL_PROGMEM =
{
    HTMLColorCode::MidnightBlue,
    HTMLColorCode::DarkBlue,
    HTMLColorCode::MidnightBlue,
    HTMLColorCode::Navy,

    HTMLColorCode::DarkBlue,
    HTMLColorCode::MediumBlue,
    HTMLColorCode::SeaGreen,
    HTMLColorCode::Teal,

    HTMLColorCode::CadetBlue,
    HTMLColorCode::Blue,
    HTMLColorCode::DarkCyan,
    HTMLColorCode::CornflowerBlue,

    HTMLColorCode::Aquamarine,
    HTMLColorCode::SeaGreen,
    HTMLColorCode::Aqua,
    HTMLColorCode::LightSkyBlue
};

extern const TProgmemRGBPalette16 ForestColors_p FL_PROGMEM =
{
    HTMLColorCode::DarkGreen,
    HTMLColorCode::DarkGreen,
    HTMLColorCode::DarkOliveGreen,
    HTMLColorCode::DarkGreen,

    HTMLColorCode::Green,
    HTMLColorCode::ForestGreen,
    HTMLColorCode::OliveDrab,
    HTMLColorCode::Green,

    HTMLColorCode::SeaGreen,
    HTMLColorCode::MediumAquamarine,
    HTMLColorCode::LimeGreen,
    HTMLColorCode::YellowGreen,

    HTMLColorCode::LightGreen,
    HTMLColorCode::LawnGreen,
    HTMLColorCode::MediumAquamarine,
    HTMLColorCode::ForestGreen
};

/// HSV Rainbow
extern const TProgmemRGBPalette16 RainbowColors_p FL_PROGMEM =
{
    0xFF0000, 0xD52A00, 0xAB5500, 0xAB7F00,
    0xABAB00, 0x56D500, 0x00FF00, 0x00D52A,
    0x00AB55, 0x0056AA, 0x0000FF, 0x2A00D5,
    0x5500AB, 0x7F0081, 0xAB0055, 0xD5002B
};

/// HSV Rainbow colors with alternatating stripes of black
#define RainbowStripesColors_p RainbowStripeColors_p
extern const TProgmemRGBPalette16 RainbowStripeColors_p FL_PROGMEM =
{
    0xFF0000, 0x000000, 0xAB5500, 0x000000,
    0xABAB00, 0x000000, 0x00FF00, 0x000000,
    0x00AB55, 0x000000, 0x0000FF, 0x000000,
    0x5500AB, 0x000000, 0xAB0055, 0x000000
};

/// HSV color ramp: blue purple ping red orange yellow (and back)
/// Basically, everything but the greens, which tend to make
/// people's skin look unhealthy.  This palette is good for
/// lighting at a club or party, where it'll be shining on people.
extern const TProgmemRGBPalette16 PartyColors_p FL_PROGMEM =
{
    0x5500AB, 0x84007C, 0xB5004B, 0xE5001B,
    0xE81700, 0xB84700, 0xAB7700, 0xABAB00,
    0xAB5500, 0xDD2200, 0xF2000E, 0xC2003E,
    0x8F0071, 0x5F00A1, 0x2F00D0, 0x0007F9
};

extern const TProgmemRGBPalette16 Test_p FL_PROGMEM =
{
    0xFF0000, 0x00FF00, 0x0000FF, 0xFF00FF,
    0x300000, 0x003000, 0x000030, 0x300030,
    0xAA0000, 0x00AA00, 0x0000AA, 0xAA00AA,
    0x010000, 0x000100, 0x000001, 0xFFFFFF
};


/// Approximate "black body radiation" palette, akin to
/// the FastLED 'HeatColor' function.
/// Recommend that you use values 0-240 rather than
/// the usual 0-255, as the last 15 colors will be
/// 'wrapping around' from the hot end to the cold end,
/// which looks wrong.
extern const TProgmemRGBPalette16 HeatColors_p FL_PROGMEM =
{
    0x000000,
    0x330000, 0x660000, 0x990000, 0xCC0000, 0xFF0000,
    0xFF3300, 0xFF6600, 0xFF9900, 0xFFCC00, 0xFFFF00,
    0xFFFF33, 0xFFFF66, 0xFFFF99, 0xFFFFCC, 0xFFFFFF
};


// Gradient palette "Rainbow_gp",
// provided for situations where you're going
// to use a number of other gradient palettes, AND
// you want a 'standard' FastLED rainbow as well.

DEFINE_GRADIENT_PALETTE( Rainbow_gp ) {
      0,  255,  0,  0, // Red
     32,  171, 85,  0, // Orange
     64,  171,171,  0, // Yellow
     96,    0,255,  0, // Green
    128,    0,171, 85, // Aqua
    160,    0,  0,255, // Blue
    192,   85,  0,171, // Purple
    224,  171,  0, 85, // Pink
    255,  255,  0,  0};// and back to Red

#endif
