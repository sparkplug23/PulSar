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



/**
 * @brief 
 * 
crgb16__indexs = 16

colourmap = parula(16)


rgb_out = mapfloat( colourmap, 0,1, 0,255)
rgb_out = round(rgb_out)

for i=1:16
    rgb2_out{i} = sprintf("%02X%02X%02X", rgb_out(i,1),  rgb_out(i,2),  rgb_out(i,3) )
end

rgb2_out = rgb2_out'




 * 
 */

/// Matlab
extern const TProgmemRGBPalette16 Matlab_Purula_p FL_PROGMEM =
{
    0x3E26A8, 0x4538D7, 0x484FF2, 0x4367FD, 
    0x2F80FA, 0x2797EB, 0x1CAADF, 0x00B9C7,
    0x29C3AA, 0x48CB86, 0x81CC59, 0xBBC42F,
    0xEABA30, 0xFEC735, 0xF5E128, 0xF9FB15
};

/// Matlab
extern const TProgmemRGBPalette16 Matlab_Turbo_p FL_PROGMEM =
{
    0x30123B, 0x4143A7, 0x4771E9, 0x3E9BFE,
    0x22C5E2, 0x1AE4B6, 0x46F884, 0x88FF4E,
    0xB9F635, 0xE1DD37, 0xFABA39, 0xFD8D27,
    0xF05B12, 0xD63506, 0xAF1801, 0x7A0403
};

/// Matlab
extern const TProgmemRGBPalette16 Matlab_Hot_p FL_PROGMEM =
{
0x2B0000,
0x550000,
0x800000,
0xAA0000,
0xD50000,
0xFF0000,
0xFF2B00,
0xFF5500,
0xFF8000,
0xFFAA00,
0xFFD500,
0xFFFF00,
0xFFFF40,
0xFFFF80,
0xFFFFBF,
0xFFFFFF
};

/// Matlab
extern const TProgmemRGBPalette16 Matlab_Cool_p FL_PROGMEM =
{
    0x00FFFF,
0x11EEFF,
0x22DDFF,
0x33CCFF,
0x44BBFF,
0x55AAFF,
0x6699FF,
0x7788FF,
0x8877FF,
0x9966FF,
0xAA55FF,
0xBB44FF,
0xCC33FF,
0xDD22FF,
0xEE11FF,
0xFF00FF
};

/// Matlab
extern const TProgmemRGBPalette16 Matlab_Spring_p FL_PROGMEM =
{
    0xFF00FF,
0xFF11EE,
0xFF22DD,
0xFF33CC,
0xFF44BB,
0xFF55AA,
0xFF6699,
0xFF7788,
0xFF8877,
0xFF9966,
0xFFAA55,
0xFFBB44,
0xFFCC33,
0xFFDD22,
0xFFEE11,
0xFFFF00
};

/// Matlab
extern const TProgmemRGBPalette16 Matlab_Autumn_p FL_PROGMEM =
{
    0xFF0000,
0xFF1100,
0xFF2200,
0xFF3300,
0xFF4400,
0xFF5500,
0xFF6600,
0xFF7700,
0xFF8800,
0xFF9900,
0xFFAA00,
0xFFBB00,
0xFFCC00,
0xFFDD00,
0xFFEE00,
0xFFFF00
};




/// Matlab
extern const TProgmemRGBPalette16 Matlab_Jet_p FL_PROGMEM =
{
  0x0000BF,
0x0000FF,
0x0040FF,
0x0080FF,
0x00BFFF,
0x00FFFF,
0x40FFBF,
0x80FF80,
0xBFFF40,
0xFFFF00,
0xFFBF00,
0xFF8000,
0xFF4000,
0xFF0000,
0xBF0000,
0x800000,

};









#endif
