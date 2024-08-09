/*-------------------------------------------------------------------------
RgbcctColor provides a color object that can be directly consumed by NeoPixelBus

Written by Michael C. Miller.

I invest time and resources providing this open source code,
please support me by dontating (see https://github.com/Makuna/NeoPixelBus)

-------------------------------------------------------------------------
This file is part of the Makuna/NeoPixelBus library.

NeoPixelBus is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of
the License, or (at your option) any later version.

NeoPixelBus is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with NeoPixel.  If not, see
<http://www.gnu.org/licenses/>.
-------------------------------------------------------------------------*/
#pragma once

struct RgbColor;
struct HslColor;
struct HsbColor;

/**
 * 2024: 
 * 
 * Keeping although Rgbww now exists for 5 pixels colours
 * Rgbcct is a convenience class that allows for the manipulation of a color (where the two whites are assumed to be warm and cold white)
 * Therefore, unlike Rgbww that can have 5 pixel full brightness, Rgbcct will scale the whites
 * If operations are not used with CCT, and the two whites set directly, then they can be considered independent of each other as only CCT operations do scaled operations
 * */

#ifndef CCT_MIN_DEFAULT
  #define CCT_MIN_DEFAULT 153          // 6500K
#endif
#ifndef CCT_MAX_DEFAULT
  #define CCT_MAX_DEFAULT 500          // 2000K
#endif

// ------------------------------------------------------------------------
// RgbcctColor represents a color object that is represented by Red, Green, Blue
// component values and an extra White component.  It contains helpful color 
// routines to manipulate the color.
// ------------------------------------------------------------------------
struct RgbcctColor : RgbColorBase
{
    typedef NeoRgbcctCurrentSettings SettingsObject;

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using R, G, B, W values (0-255)
    // ------------------------------------------------------------------------
    RgbcctColor(uint8_t r, uint8_t g, uint8_t b, uint8_t ww = 0, uint8_t wc = 0) :
        R(r), G(g), B(b), WW(ww), CW(wc)
    {
    };

    // ------------------------------------------------------------------------
    // Construct a RgbColor using a single brightness value (0-255)
    // This works well for creating gray tone colors
    // (0) = black, (255) = white, (128) = gray
    // ------------------------------------------------------------------------
    RgbcctColor(uint8_t brightness) :
        R(0), G(0), B(0), WW(brightness), CW(brightness)
    {
    };

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using RgbColor
    // ------------------------------------------------------------------------
    RgbcctColor(const RgbColor& color) :
        R(color.R),
        G(color.G),
        B(color.B),
        WW(0),
        CW(0)
    {
    };

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using RgbWColor
    // ------------------------------------------------------------------------
    RgbcctColor(const RgbwColor& color) :
      R(color.R),
      G(color.G),
      B(color.B),
      WW(color.W),    //to be renamed W1/W2, since it could be warm white or cold white
      CW(color.W)
    {
    };

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using a uint32 packed WRGB colour
    // ------------------------------------------------------------------------
    RgbcctColor(uint32_t color) :
        R(static_cast<uint8_t>(color >> 16)),
        G(static_cast<uint8_t>(color >> 8)),
        B(static_cast<uint8_t>(color)),
        WW(0),
        CW(static_cast<uint8_t>(color >> 24))
    {
    };

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using RgbWColor
    // ------------------------------------------------------------------------
    RgbcctColor(const RgbwwColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using HtmlColor
    // ------------------------------------------------------------------------
    RgbcctColor(const HtmlColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using HslColor
    // ------------------------------------------------------------------------
    RgbcctColor(const HslColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using HsbColor
    // ------------------------------------------------------------------------
    RgbcctColor(const HsbColor& color);

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor that will have its values set in latter operations
    // CAUTION:  The R,G,B, W members are not initialized and may not be consistent
    // ------------------------------------------------------------------------
    RgbcctColor()
    {
    };

    // ------------------------------------------------------------------------
    // Comparison operators
    // ------------------------------------------------------------------------
    bool operator==(const RgbcctColor& other) const
    {
        return (R == other.R && G == other.G && B == other.B && WW == other.WW && CW == other.CW);
    };

    bool operator!=(const RgbcctColor& other) const
    {
        return !(*this == other);
    };

    // ------------------------------------------------------------------------
    // Returns if the color is grey, all values are equal other than white
    // ------------------------------------------------------------------------
    bool IsMonotone() const
    {
        return (R == B && R == G);
    };

    /*
     So RGB can be edited without changing the white values
    */
    void setRGB(uint8_t r, uint8_t g, uint8_t b)
    {
      R = r;
      G = g;
      B = b;
    }

   // add one RgbcctColor to another, saturating at 0xFF for each channel
    inline RgbcctColor& operator+= (const RgbcctColor& rhs )
    {
        // R = qadd8( R, rhs.R);
        // G = qadd8( G, rhs.G);
        // B = qadd8( B, rhs.B);
        // WW = qadd8( WW, rhs.WW);
        // CW = qadd8( CW, rhs.CW);
        return *this;
    }

    /// scale down a RGB to N 256ths of it's current brightness, using
    /// 'plain math' dimming rules, which means that if the low light levels
    /// may dim all the way to 100% black.
    inline RgbcctColor& nscale8 (const RgbcctColor & scaledown )
    {
        // R = ::scale8(R, scaledown.R);
        // G = ::scale8(G, scaledown.G);
        // B = ::scale8(B, scaledown.B);
        return *this;
    }

   // add one RgbcctColor to another, saturating at 0xFF for each channel
    inline RgbcctColor& operator-= (const RgbcctColor& rhs )
    {
        // R = qsub8( R, rhs.R);
        // G = qsub8( G, rhs.G);
        // B = qsub8( B, rhs.B);
        // WW = qsub8( WW, rhs.WW);
        // CW = qsub8( CW, rhs.CW);
        return *this;
    }
    
    // ------------------------------------------------------------------------
    // Returns if the color components are all zero, the white component maybe 
    // anything
    // ------------------------------------------------------------------------
    bool IsColorLess() const
    {
        return (R == 0 && B == 0 && G == 0);
    };

    // ------------------------------------------------------------------------
    // CalculateBrightness will calculate the overall brightness
    // NOTE: This is a simple linear brightness
    // ------------------------------------------------------------------------
    uint8_t CalculateBrightness() const;

    // ------------------------------------------------------------------------
    // Darken will adjust the color by the given delta toward black
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to dim the color
    // ------------------------------------------------------------------------
    void Darken(uint8_t delta);

    // ------------------------------------------------------------------------
    // Fade will adjust the color divided by the fade amount by the given toward black
    // NOTE: This is a nonlinear change
    // delta - (0-255) the amount to divide the color
    // ------------------------------------------------------------------------
    void Fade(uint8_t fade);

    // ------------------------------------------------------------------------
    // Lighten will adjust the color by the given delta toward white
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to lighten the color
    // ------------------------------------------------------------------------
    void Lighten(uint8_t delta);

    // ------------------------------------------------------------------------
    // Lighten will adjust the color by the given delta toward white
    // NOTE: This is a simple linear change
    // delta - (0-255) the amount to lighten the color
    // ------------------------------------------------------------------------
    void Variance(uint8_t delta);

    // ------------------------------------------------------------------------
    // LinearBlend between two colors by the amount defined by progress variable
    // left - the color to start the blend at
    // right - the color to end the blend at
    // progress - (0.0 - 1.0) value where 0 will return left and 1.0 will return right
    //     and a value between will blend the color weighted linearly between them
    // ------------------------------------------------------------------------
    static RgbcctColor LinearBlend(const RgbcctColor& left, const RgbcctColor& right, float progress);
    // progress - (0 - 255) value where 0 will return left and 255 will return right
    //     and a value between will blend the color weighted linearly between them
    // ------------------------------------------------------------------------
    static RgbcctColor LinearBlend(const RgbcctColor& left, const RgbcctColor& right, uint8_t progress);
    
    // ------------------------------------------------------------------------
    // BilinearBlend between four colors by the amount defined by 2d variable
    // c00 - upper left quadrant color
    // c01 - upper right quadrant color
    // c10 - lower left quadrant color
    // c11 - lower right quadrant color
    // x - unit value (0.0 - 1.0) that defines the blend progress in horizontal space
    // y - unit value (0.0 - 1.0) that defines the blend progress in vertical space
    // ------------------------------------------------------------------------
    static RgbcctColor BilinearBlend(const RgbcctColor& c00, 
        const RgbcctColor& c01, 
        const RgbcctColor& c10, 
        const RgbcctColor& c11, 
        float x, 
        float y);

    // Unions allow overlapping of parameters, size of parameters below is only 5 bytes, but can be accessed by multiple ways
    union {
		struct {
            union {
                uint8_t R;
                uint8_t red;
            };
            union {
                uint8_t G;
                uint8_t green;
            };
            union {
                uint8_t B;
                uint8_t blue;
            };
            union {
                uint8_t WW;
                uint8_t W1; // for when the white channel does not matter e.g. rgbw
                uint8_t W;
            };
            union {
                uint8_t CW;
                uint8_t W2;
            };
        };
		uint8_t raw[5];
	};
  
  // Array access operator to index into the color object
  inline const uint8_t& operator[] (uint8_t x) const __attribute__((always_inline))
  {
    return raw[x];
  }
    
  /*********************************************************************************************************************************
   * ****** Hue and Saturation Methods **********************************************************************************************
   * ********************************************************************************************************************************/
  
  HsbColor getHsbColor() const {
    return HsbColor(RgbColor(R, G, B));
  }

  void setHue360(uint16_t hue_new) {
    HsbColor hsb = HsbColor(RgbColor(R, G, B));
    hsb.H = hue_new / 360.0f;
    *this = RgbcctColor(hsb);
  }

  void setSat255(uint8_t sat_new) {
    HsbColor hsb = HsbColor(RgbColor(R, G, B));
    hsb.S = sat_new / 255.0f;
    *this = RgbcctColor(hsb);
  }

  uint16_t getHue360() const {
    return static_cast<uint16_t>(HsbColor(RgbColor(R, G, B)).H * 360.0f);
  }

  uint8_t getSat255() const {
    return static_cast<uint8_t>(HsbColor(RgbColor(R, G, B)).S * 255.0f);
  }


  /************************************************************************************************************************************
  ******* BrightnessRGB *****************************************************************************************************************************
  ************************************************************************************************************************************/
 
  uint8_t  _briRGB = 255;                      // 0..255

  void setBrightnessRGB(uint8_t bri_rgb) 
  {
    _briRGB = bri_rgb;
  }

  uint8_t getBrightnessRGB() 
  {
    return _briRGB;
  }

  uint8_t getBrightnessCCT() 
  {
    return _briCCT;
  }

  /************************************************************************************************************************************
  ******* BrightnessCCT *****************************************************************************************************************************
  ************************************************************************************************************************************/

  uint8_t  _briCCT = 255;                      // 0..255

  void setBrightnessCCT(uint8_t bri_cct) 
  {
    _briCCT = bri_cct;
  }

  uint8_t getBrightnessCCT255() {
    return _briCCT;
  }

  /************************************************************************************************************************************
  ******* Brightness *****************************************************************************************************************************
  ************************************************************************************************************************************/
 
  void setBrightness(uint8_t bri) 
  {
    _briRGB = _briCCT = bri;
  }

  uint8_t getMaxBrightness(void) 
  {
    return (_briRGB >= _briCCT) ? _briRGB : _briCCT;
  }

  /************************************************************************************************************************************
  ******* ColorTemperature *****************************************************************************************************************************
  ************************************************************************************************************************************/

  /**
   * @brief 
   * Removed unnecessary void in function signatures: Functions that do not take parameters do not need the void keyword.
   * Replaced pointer parameters with references in getCCTRange: Using references makes the code cleaner and avoids null checks.
   * Used bit shifts for mapping in setCCT and getCCT: This can be faster than the map function as it avoids function call overhead.
   * Simplified setCCT_Kelvin logic: Used direct arithmetic operations for scaling, which can be more efficient.
   * Made functions const where applicable: Indicating that these functions do not modify the state of the object.
   * 
   */

  uint16_t _cct = CCT_MIN_DEFAULT;             // 153..500, default to 153 (cold white)
  uint16_t _cct_min_range = CCT_MIN_DEFAULT;   // the minimum CT rendered range
  uint16_t _cct_max_range = CCT_MAX_DEFAULT;   // the maximum CT rendered range

  uint16_t get_CTRangeMin() const {
    return _cct_min_range;
  }

  uint16_t get_CTRangeMax() const {
    return _cct_max_range;
  }

  void setCCTRange(uint16_t cct_min_range, uint16_t cct_max_range) {
    _cct_min_range = cct_min_range;
    _cct_max_range = cct_max_range;
  }

  void getCCTRange(uint16_t &cct_min_range, uint16_t &cct_max_range) const {
    cct_min_range = _cct_min_range;
    cct_max_range = _cct_max_range;
  }

  void setCCT(uint8_t cct_255) {
    uint16_t cct = _cct_min_range + ((static_cast<uint32_t>(cct_255) * (_cct_max_range - _cct_min_range)) >> 8);
    setCCT_Kelvin(cct);
  }

  void setCCT_Kelvin(uint16_t cct) {
    cct = (cct < CCT_MIN_DEFAULT) ? CCT_MIN_DEFAULT : (cct > CCT_MAX_DEFAULT ? CCT_MAX_DEFAULT : cct);
    WW = ((cct - _cct_min_range) * 255) / (_cct_max_range - _cct_min_range);
    CW = 255 - WW;
    _cct = cct;
  }

  uint16_t getCCT_Kelvin() const {
    return _cct;
  }

  uint8_t getCCT() const {
    return ((static_cast<uint32_t>(_cct - _cct_min_range) * 255) / (_cct_max_range - _cct_min_range));
  }


  /************************************************************************************************************************************
  ******* Getters with modifiers ******************************************************************************************************
  ************************************************************************************************************************************/

  static RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor colour, uint8_t brightness) 
  {
    RgbcctColor out;
    // Pre-calculate scale factor
    uint16_t scale = brightness + 1;  // Adding 1 to avoid division by zero and maintain full range

    // Apply the same brightness to all components using bit shifts for division by 256
    out.R  = (colour.R * scale) >> 8;
    out.G  = (colour.G * scale) >> 8;
    out.B  = (colour.B * scale) >> 8;
    out.WW = (colour.WW * scale) >> 8;
    out.CW = (colour.CW * scale) >> 8;

    return out;
  }

  static RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor colour, uint8_t brightnessRGB, uint8_t brightnessCCT) 
  {
    RgbcctColor out;
    // Pre-calculate scale factors
    uint16_t scaleRGB = brightnessRGB + 1;  // Adding 1 to avoid division by zero and maintain full range
    uint16_t scaleCCT = brightnessCCT + 1;  // Adding 1 to avoid division by zero and maintain full range

    // Apply RGB brightness using bit shifts for division by 256
    out.R = (colour.R * scaleRGB) >> 8;
    out.G = (colour.G * scaleRGB) >> 8;
    out.B = (colour.B * scaleRGB) >> 8;

    // Apply CCT brightness using bit shifts for division by 256
    out.WW = (colour.WW * scaleCCT) >> 8;
    out.CW = (colour.CW * scaleCCT) >> 8;

    return out;
  }

  /**
   * Get RgbcctColor with brightness applied
   * replacing GetU32ColourBrightnessApplied   --> WithBrightness                    
   * replacing GetColourWithBrightnessApplied  --> rgbcctcolors[1].WithBrightness().getU32()
   * Bit Shifts for Division: Using >> 8 instead of / 255 for division by 256 can be faster on some platforms.
   * Avoid Division by Zero: Adding 1 to the scale factors ensures we avoid potential division by zero errors while still maintaining the correct scaling for full brightness.
   * */
  RgbcctColor WithBrightness() const
  {
    RgbcctColor colour;

    // Pre-calculate scale factors
    uint16_t scaleRGB = (_briRGB * 256) / 255;  // Multiply by 256 to use bit shifts later
    uint16_t scaleCCT = (_briCCT * 256) / 255;  // Multiply by 256 to use bit shifts later

    // Apply scale factors using bit shifts
    colour.R  = (R * scaleRGB) >> 8;
    colour.G  = (G * scaleRGB) >> 8;
    colour.B  = (B * scaleRGB) >> 8;
    colour.CW = (CW * scaleCCT) >> 8;
    colour.WW = (WW * scaleCCT) >> 8;

    return colour;
  }

  // rgbcct.getU32() could replace this
  // static uint32_t GetU32Colour(const RgbcctColor& c)
  // {
  //   return (static_cast<uint32_t>(c.W) << 24) | 
  //          (static_cast<uint32_t>(c.R) << 16) | 
  //          (static_cast<uint32_t>(c.G) << 8) | 
  //           static_cast<uint32_t>(c.B);
  // }

  // RgbcctColor(uint32_t color) :
  // static RgbcctColor GetRgbcctFromU32Colour(uint32_t c)
  // { 
  //   return RgbcctColor(static_cast<uint8_t>(c >> 16), 
  //                      static_cast<uint8_t>(c >> 8), 
  //                      static_cast<uint8_t>(c), 
  //                      static_cast<uint8_t>(c >> 24));
  // }

  uint32_t getU32() const
  {
    return (static_cast<uint32_t>(W1) << 24) | 
           (static_cast<uint32_t>(R) << 16) | 
           (static_cast<uint32_t>(G) << 8) | 
           static_cast<uint32_t>(B);
  }


  /************************************************************************************************************************************
  ******* Debug                  ******************************************************************************************************
  ************************************************************************************************************************************/

  void debug_print(const char* name = nullptr) 
  {
    if (name) {
      Serial.print(name);
      Serial.print('=');
    }
    Serial.printf("%d,%d,%d,%d,%d\n\r", R, G, B, CW, WW);
  }



};

