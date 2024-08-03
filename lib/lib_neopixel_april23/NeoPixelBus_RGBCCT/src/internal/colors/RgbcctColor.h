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

// Forward declarations of used color structures if not included
struct RgbColor;
struct RgbwColor;
struct HtmlColor;
struct HslColor;
struct HsbColor;

#define CCT_MIN_DEFAULT 153          // 6500K
#define CCT_MAX_DEFAULT 500          // 2000K

// RgbcctColor represents a color object with RGB and two white components (warm and cold)
struct RgbcctColor : RgbColorBase
{
    typedef NeoRgbcctCurrentSettings SettingsObject;
  
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
          uint8_t WC;
          uint8_t W1;
          uint8_t CW;
          uint8_t white_cold;  // Assume cold is the normal white, its only "warm" when an alternate exists
        };
        union {
          uint8_t WW;
          uint8_t W2;
          uint8_t warm_white;
        };
      };
      uint8_t raw[5];
    };

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using R, G, B, W values (0-255)
    // ------------------------------------------------------------------------
    RgbcctColor(uint8_t r, uint8_t g, uint8_t b, uint8_t ww = 0, uint8_t wc = 0) :
        R(r), G(g), B(b), WW(ww), WC(wc)
    {
        // Serial.printf("RgbcctColor %d\n\r", r);
        // delay(1000);
        // Serial.printf("RgbcctColor %d\n\r", r);
    };

    // ------------------------------------------------------------------------
    // Construct a RgbColor using a single brightness value (0-255)
    // This works well for creating gray tone colors
    // (0) = black, (255) = white, (128) = gray
    // ------------------------------------------------------------------------
    RgbcctColor(uint8_t brightness) :
        R(0), G(0), B(0), WW(brightness), WC(brightness)
    {
    };

    // RgbcctColor(uint32_t wrgb_packed) :
    //     R(byte((wrgb_packed) >> 16)), G(byte((wrgb_packed) >> 8)), B(byte(wrgb_packed)), WW(byte((wrgb_packed) >> 24)), WC(byte((wrgb_packed) >> 24))
    // {
    // };

    // ------------------------------------------------------------------------
    // Construct a RgbColor using a single brightness value (0-255)
    // This works well for creating gray tone colors
    // (0) = black, (255) = white, (128) = gray
    // ------------------------------------------------------------------------
    // RgbcctColor(uint32_t colour_32bit) : // W,R,G,B
    //     R(0), G(0), B(0), WW(brightness), WC(brightness)
    // {
    // };

    // ------------------------------------------------------------------------
    // Construct a RgbcctColor using RgbColor
    // ------------------------------------------------------------------------
    RgbcctColor(const RgbColor& color) :
        R(color.R),
        G(color.G),
        B(color.B),
        WW(0),
        WC(0)
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
        WC(color.W)
        // R(0),G(0),B(0),WC(0)    
    {
    };

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
    bool operator==(const RgbcctColor& other) const {
        return R == other.R && G == other.G && B == other.B && WW == other.WW && WC == other.WC;
    }
    bool operator!=(const RgbcctColor& other) const {
        return !(*this == other);
    }

    // Utility functions
    bool isMonotone() const { return (R == G && R == B); }
    // bool isColorLess() const { return (R == 0 && G == 0 && B == 0); }

    bool IsColorLess() const
    {
        return (R == 0 && B == 0 && G == 0);
    };



   /// add one RgbcctColor to another, saturating at 0xFF for each channel
    inline RgbcctColor& operator+= (const RgbcctColor& rhs )
    {
        // R = qadd8( R, rhs.R);
        // G = qadd8( G, rhs.G);
        // B = qadd8( B, rhs.B);
        // WW = qadd8( WW, rhs.WW);
        // WC = qadd8( WC, rhs.WC);
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





   /// add one RgbcctColor to another, saturating at 0xFF for each channel
    inline RgbcctColor& operator-= (const RgbcctColor& rhs )
    {
        // R = qsub8( R, rhs.R);
        // G = qsub8( G, rhs.G);
        // B = qsub8( B, rhs.B);
        // WW = qsub8( WW, rhs.WW);
        // WC = qsub8( WC, rhs.WC);
        return *this;
    }



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

    /**
     * @brief Instead of changing by a delta value, the colour component is divided by the fade amount
     * 
     * @param fade 
     */
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

    // Example implementation of fromUint32
    // Unpacks a 32-bit unsigned integer into an RgbcctColor object assuming the format RRGGBBWW
    #define GetRgbcctFromU32Colour fromUint32
    static RgbcctColor fromUint32(uint32_t color) {
        uint8_t w = (color >> 24) & 0xFF; // Extract the red component
        uint8_t r = (color >> 16) & 0xFF; // Extract the green component
        uint8_t g = (color >> 8) & 0xFF;  // Extract the blue component
        uint8_t b = color & 0xFF;         // Extract the warm white component

        return RgbcctColor(r, g, b, w);
    }

    // Example implementation of toUint32
    // Packs R, G, B, and one white component into a 32-bit unsigned integer
    #define GetU32Colour toUint32
    static uint32_t toUint32(const RgbcctColor& color) {
        return (static_cast<uint32_t>(color.W1) << 24) |
              (static_cast<uint32_t>(color.R) << 16) |
              (static_cast<uint32_t>(color.G) << 8)  |
              (static_cast<uint32_t>(color.B));  // Assume we are packing WW as the white component
    }








    // /**
    //  * @brief 
    //  * Conversion from Rgbcct into uint32_t (dropping the second white colour)
    //  * 
    //  */
    // static uint32_t GetU32Colour(RgbcctColor c)
    // {
    //     uint32_t c_out =
    //         ((uint32_t)c.warm_white << 24) |
    //         ((uint32_t)c.red        << 16) |
    //         ((uint32_t)c.green      <<  8) |
    //         ((uint32_t)c.blue            ) ;
    //     return c_out;
    // }

    // static RgbcctColor GetRgbcctFromU32Colour(uint32_t c)
    // {
        
    //     uint8_t white = (uint8_t)(c >> 24);
    //     uint8_t red   = (uint8_t)(c >> 16);
    //     uint8_t green = (uint8_t)(c >> 8);
    //     uint8_t blue  = (uint8_t)(c);
    //     return RgbcctColor(red,green,blue,white);
    // }
    
    uint32_t getU32()
    {
        uint32_t c_out =
            ((uint32_t)W1 << 24) |
            ((uint32_t)R        << 16) |
            ((uint32_t)G      <<  8) |
            ((uint32_t)B            ) ;
        return c_out;
    }

    RgbcctColor& setU32(uint32_t c)
    {
        W1 = (uint8_t)(c >> 24);
        R = (uint8_t)(c >> 16);
        G = (uint8_t)(c >> 8);
        B = (uint8_t)(c);
        return *this; 
    //     return *this;
    }










  
  uint16_t _hue = 0;  // 0..359
  uint8_t  _sat = 255;  // 0..255
      
  uint8_t  _briRGB = 255;  // 0..255
  uint8_t  _briCCT = 255;

  // Enable manual control (may not be needed)
  bool     _pwm_multi_channels = false;
  bool     _base_colour_contains_brightness_applied = false;
  
  // Color or mono
  uint8_t  _color_mode = LIGHT_MODE_BOTH; // remove colour mode, as it should be defined by lighttype only


  // Using rgbcct partially, for RGB, RGBW, RGBWW, RGBCCT etc with the unused channel turned off
  uint8_t  _subtype = COLOUR_TYPE__RGBCCT__ID;

  
  // are RGB and CT linked, i.e. if we set CT then RGB channels are off
  bool     _cct_rgb_linked = true;


  uint16_t _cct = CCT_MIN_DEFAULT;  // 153..500, default to 153 (cold white)
  uint16_t _cct_min_range = CCT_MIN_DEFAULT;   // the minimum CT rendered range
  uint16_t _cct_max_range = CCT_MAX_DEFAULT;   // the maximum CT rendered range

    uint16_t get_CTRangeMin(void)
    {
      return _cct_min_range;
    }
    uint16_t get_CTRangeMax(void)
    {
      return _cct_max_range;
    }

      
  /***
   *  Enable Color Modes
   * */
  void addRGBMode() {
    setColorMode(_color_mode | LIGHT_MODE_RGB);
  }
  void addCCTMode() {
    setColorMode(_color_mode | LIGHT_MODE_CCT);
  }


  /***
   *  Static conversions to add colours
   * */

  static RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor colour, uint8_t brightness) {
    RgbcctColor colour_with_brightness;

    // Apply the same brightness to all components
    colour_with_brightness.R = (colour.R * brightness) / 255;
    colour_with_brightness.G = (colour.G * brightness) / 255;
    colour_with_brightness.B = (colour.B * brightness) / 255;
    colour_with_brightness.WW = (colour.WW * brightness) / 255;
    colour_with_brightness.WC = (colour.WC * brightness) / 255;

    return colour_with_brightness;
}



  static RgbcctColor ApplyBrightnesstoRgbcctColour(RgbcctColor colour, uint8_t brightnessRGB, uint8_t brightnessCCT) {
    RgbcctColor colour_with_brightness;

    // Apply RGB brightness
    colour_with_brightness.R = (colour.R * brightnessRGB) / 255;
    colour_with_brightness.G = (colour.G * brightnessRGB) / 255;
    colour_with_brightness.B = (colour.B * brightnessRGB) / 255;

    // Apply CCT brightness
    colour_with_brightness.WW = (colour.WW * brightnessCCT) / 255;
    colour_with_brightness.WC = (colour.WC * brightnessCCT) / 255;

    return colour_with_brightness;
}





  /**
   * UpdateInternalColour
   * Generate rgbcct object values with latest values
   * 
   * Note that this may need changed, as I still want the full colour saved.
   * The get colour will just need to correctly apply brightness before giving it
   * 
   * */
  void UpdateInternalColour()
  {

    // Serial.println("UpdateInternalColour");
    
    RgbcctColor colour_full_range = GetColourFullRange();
    RgbcctColor colour_with_brightness = GetColourWithBrightnessApplied();
    RgbcctColor colour_out;

    if(_base_colour_contains_brightness_applied){
      colour_out = colour_with_brightness;
    }else{
      colour_out = colour_full_range;
    }

    // uint8_t briRGB = getBrightnessRGB();
    uint8_t briCCT = getBrightnessCCT();
    
    #ifdef ENABLE_RGBCCT_DEBUG
    Serial.printf("UpdateInternalColour\n\r colour_full_range (%d %d %d %d %d)\n\r colour_with_brightness (%d %d %d %d %d)\n\r -- rgb%d ct%d\n\r", 
      colour_full_range.R,colour_full_range.G,colour_full_range.B,colour_full_range.WC, colour_full_range.WW,
      colour_with_brightness.R,colour_with_brightness.G,colour_with_brightness.B,colour_with_brightness.WC, colour_with_brightness.WW,
      _briRGB,briCCT
    );
    #endif // ENABLE_RGBCCT_DEBUG
    
    // Serial.printf("getSubType %d\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r", _subtype);

    switch (_subtype) {
      // Use RGB brightness for all channels
      case COLOUR_TYPE__SINGLE__ID:
        R = 255;//_briRGB;
        G = 255;//_briRGB;
        B = 255;//_briRGB;
        WC = 255;//_briRGB;
        WW = 255;//_briRGB;
        break;
      case COLOUR_TYPE__COLDWARM__ID:
        R = 0;
        G = 0;
        B = 0;
        WC = colour_out.WC;
        WW = colour_out.WW;
        break;
      // case COLOUR_TYPE__RGBCW__ID:
      default:
      case COLOUR_TYPE__RGBCCT__ID:      
        R = colour_out.R;
        G = colour_out.G;
        B = colour_out.B;
        WC = colour_out.WC;
        WW = colour_out.WW;
        // Serial.printf(PSTR("COLOUR_TYPE__RGBCCT__ID [%d,%d,%d,%d,%d]\n\r"), R, G, B, W1, W2);
  
      break;
      case COLOUR_TYPE__RGBW__ID:  //white   "WHITE" as rgbW should be generic to any white
        // Serial.println("COLOUR_TYPE__RGBW__ID");

        WC = 255;//briCCT;   // Since I am saving in full colour, these should be 100%: The getters later will add the brightness if a flag is set to use it
        WW = 255;//briCCT;
        R = colour_out.R;
        G = colour_out.G;
        B = colour_out.B;

      break;

/**
 * @brief 
 * CW and WW, ie signel channel white should be handled here?
 * 
 */
      case COLOUR_TYPE__RGBCW__ID: //cold white?
        WC = 255;//briCCT;
        WW = 255;//briCCT;
        R = colour_out.R;
        G = colour_out.G;
        B = colour_out.B;
      break;


      case COLOUR_TYPE__RGB__ID:
        WC = 0;
        WW = 0;
        R = colour_out.R;
        G = colour_out.G;
        B = colour_out.B;
        break;
    }

    
    // Serial.printf("RGBWC=%d,%d,%d,%d,%d\n\r",R,G,B,WW,WC);


  };



    
  void setHS_FullBrightness(HsbColor hsb, bool flag_hue_sat_stored_as_full_brightness_range = true);

//     HsbColor hsb_new = hsb;
//     if(flag_hue_sat_stored_as_full_brightness_range){
//       hsb_new.B = 1; // full
//     }
//     RgbColor rgb = hsb_new;
//     R = rgb.R;
//     G = rgb.G;
//     B = rgb.B;
//     _hue = hsb_new.H*360;
//     _sat = hsb_new.S*255;
//     addRGBMode();
//   }



    enum ColourType{ 
        // COLOUR_TYPE__NONE__ID=0, 
        COLOUR_TYPE__SINGLE__ID, // likely never used for me, remove
        COLOUR_TYPE__COLDWARM__ID,  //CCT Only
        COLOUR_TYPE__RGB__ID,   
        COLOUR_TYPE__RGBW__ID, 
        COLOUR_TYPE__RGBCCT__ID, // CW/WW 
        
        // Previous methods that remember colour order, probably not needed or at least cct assume default of RGBWC
        COLOUR_TYPE__RGBWC__ID, 
        COLOUR_TYPE__RGBCW__ID
    };

    enum LightColorModes {
        LIGHT_MODE_RGB = 1, 
        LIGHT_MODE_CCT = 2, 
        LIGHT_MODE_BOTH = 3 
    };

  // void Sync(){
  //   UpdateInternalColour();
  // }
  

  // Array access operator to index into the color object
  inline const uint8_t& operator[] (uint8_t x) const __attribute__((always_inline))
  {
    return raw[x];
  }
  
  /**
   * @brief Note that no update is performed
   * 
   * @param channels 
   */
  void setChannelsRaw(uint8_t *channels) {
    R = channels[0];
    G = channels[1];
    B = channels[2];
    WC = channels[3];
    WW = channels[4];
  }

  void setChannelsRaw(uint8_t r, uint8_t g, uint8_t b, uint8_t wc, uint8_t ww) {
    R = r;
    G = g;
    B = b;
    WC = wc;
    WW = ww;
  }
  
  /**
   * _base_colour_contains_brightness_applied
   * 
   * probably phasing this out, since colour stored will ALWAYS be full range to stop conpression
   * */
  void setApplyBrightnessToOutput(bool state){
    _base_colour_contains_brightness_applied = state;
  }
  bool getApplyBrightnessToOutput(){
    return _base_colour_contains_brightness_applied;
  }

  /*********************************************************************************************************************************
   * ****** Subtype **************************************************************************************************************************
   * ********************************************************************************************************************************/
  
  void setSubType(uint8_t sub_type)
  {
    Serial.println(sub_type);
    _subtype = sub_type;
  }

  uint8_t getSubType(void) 
  {
    return _subtype;
  }

  
  /*********************************************************************************************************************************
   * ****** ColorMode **************************************************************************************************************************
   * ********************************************************************************************************************************/
  
  // This function is a bit hairy, it will try to match the rerquired colormode with the features of the device:
  //  LIGHT_TYPE_NONE:     LIGHT_MODE_RGB
  //  LIGHT_TYPE_SINGLE:   LIGHT_MODE_RGB
  //  LIGHT_TYPE_COLDWARM: LIGHT_MODE_CT
  //  LIGHT_TYPE_RGB:      LIGHT_MODE_RGB
  //  LIGHT_TYPE_RGBW:     LIGHT_MODE_RGB,LIGHT_MODE_CT or LIGHT_MODE_BOTH
  //  LIGHT_TYPE_RGBCW:    LIGHT_MODE_RGB,LIGHT_MODE_CT or LIGHT_MODE_BOTH

  uint8_t setColorMode(uint8_t cm) {

    uint8_t prev_cm = _color_mode;
    if (cm < LIGHT_MODE_RGB) { cm = LIGHT_MODE_RGB; }
    if (cm > LIGHT_MODE_BOTH) { cm = LIGHT_MODE_BOTH; }
    uint8_t maxbri = (_briRGB >= _briCCT) ? _briRGB : _briCCT;

    switch (_subtype) {
      case COLOUR_TYPE__COLDWARM__ID:
        _color_mode = LIGHT_MODE_CCT;
        break;
      case COLOUR_TYPE__SINGLE__ID:
      case COLOUR_TYPE__RGB__ID:
      default:
        _color_mode = LIGHT_MODE_RGB;
        break;
      case COLOUR_TYPE__RGBW__ID:
      case COLOUR_TYPE__RGBCW__ID:
      case COLOUR_TYPE__RGBCCT__ID:
        _color_mode = cm;
        break;
    }
    if (LIGHT_MODE_RGB == _color_mode) {
      _briCCT = 0;
      if (0 == _briRGB) { _briRGB = maxbri; }
    }
    if (LIGHT_MODE_CCT == _color_mode) {
      _briRGB = 0;
      if (0 == _briCCT) { _briCCT = maxbri; }
    }
    // #ifdef ENABLE_RGBCCT_DEBUG
    // Serial.printf("prev_cm (%d) req_cm (%d) new_cm (%d)\n\r", prev_cm, cm, _color_mode);
    // #endif // ENABLE_RGBCCT_DEBUG

    return prev_cm;
  
  }
  
  uint8_t getColorMode() {
    return _color_mode;
  }

  
  /*********************************************************************************************************************************
   * ****** RGB **************************************************************************************************************************
   * ********************************************************************************************************************************/
  
  // sets RGB and returns the Brightness. Bri is updated unless keep_bri is true
  uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false) {
    
    #ifdef ENABLE_RGBCCT_DEBUG
    Serial.printf("setRGB RGB input (%d %d %d)\n\r", r, g, b);
    #endif

    uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255

    if (0 == max) {
      // No active RGB channels, disable RGB mode
      r = g = b = 255;
      setColorMode(LIGHT_MODE_CCT);
    } else {
      if (255 > max) {
        // we need to normalize rgb
        r = map(r, 0, max, 0, 255);
        g = map(g, 0, max, 0, 255);
        b = map(b, 0, max, 0, 255);
      }
      // Enable RGB mode
      addRGBMode();
    }
    if (!keep_bri) {
      _briRGB = (_color_mode & LIGHT_MODE_RGB) ? max : 0;
    }

    // Update RGB Components
    R = r;
    G = g;
    B = b;
    
    // Update HSB Components
    // HsbColor hsb = RgbColor(r,g,b);
    // _hue = hsb.H*360;
    // _sat = hsb.S*255;

    #ifdef ENABLE_RGBCCT_DEBUG
    // Serial.printf("setRGB RGB output (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
    #endif

    return max;
  }

  
  /*********************************************************************************************************************************
   * ****** RgbcctColors **************************************************************************************************************************
   * ********************************************************************************************************************************/
  
  /**
   * Get RgbcctColor with brightness applied
   * */
  RgbcctColor GetColourWithBrightnessApplied(){

    bool rgb_channels_on = _color_mode & LIGHT_MODE_RGB;
    bool ct_channels_on =   _color_mode & LIGHT_MODE_CCT;

    RgbcctColor adjusted_colour;
    
    adjusted_colour.R = rgb_channels_on ? map(R, 0, 255, 0, _briRGB) : 0;
    adjusted_colour.G = rgb_channels_on ? map(G, 0, 255, 0, _briRGB) : 0;
    adjusted_colour.B = rgb_channels_on ? map(B, 0, 255, 0, _briRGB) : 0;

    adjusted_colour.WC = ct_channels_on  ? map(WC, 0, 255, 0, _briCCT) : 0; 
    adjusted_colour.WW = ct_channels_on  ? map(WW, 0, 255, 0, _briCCT) : 0;

    return adjusted_colour;

  }

  /**
   * Get RgbcctColor withOUT brightness applied
   * */
  RgbcctColor GetColourFullRange(){

    bool rgb_channels_on = _color_mode & LIGHT_MODE_RGB;
    bool ct_channels_on =   _color_mode & LIGHT_MODE_CCT;

    RgbcctColor adjusted_colour;
    
    adjusted_colour.R = rgb_channels_on ? R : 0;
    adjusted_colour.G = rgb_channels_on ? G : 0;
    adjusted_colour.B = rgb_channels_on ? B : 0;

    adjusted_colour.WC = ct_channels_on  ? WC : 0; 
    adjusted_colour.WW = ct_channels_on  ? WW : 0;

    return adjusted_colour;

  }

  
  /*********************************************************************************************************************************
   * ****** HsbColors **************************************************************************************************************************
   * ********************************************************************************************************************************/

HsbColor getHsbColor(void);
//   {
//     HsbColor hsb = HsbColor(RgbColor(0));
//     hsb.H = _hue/360.0f;
//     hsb.S = _sat/255.0f;
//     hsb.B = _briRGB/255.0f;
//     return hsb;
//   }
  

  void setHue360(uint16_t hue_new);

//     HsbColor hsb = getHsbColor();
//     hsb.H = hue_new/360.0f;
//     setHsbColor(hsb);
//   }


void setSat255(uint8_t sat_new);
//{
//     HsbColor hsb = getHsbColor();
//     hsb.S = sat_new/255.0f;
//     setHsbColor(hsb);
//   }

  void setHsbColor(HsbColor hsb);
// {
//     setHS_FullBrightness(hsb);
//     setBrightnessRGB255(hsb.B*255); // Set brightness independently
//     if (_cct_rgb_linked) { setColorMode(LIGHT_MODE_RGB); }   // try to force RGB
//     UpdateInternalColour();
//   }

  
  uint16_t getHue360(){
    return _hue;
  }
  
  uint8_t getSat255(){
    return _sat;
  }
  
  /************************************************************************************************************************************
  ******* Brightness *****************************************************************************************************************************
  ************************************************************************************************************************************/
 
  // sets both master Bri and whiteBri
  void setBrightness(uint8_t bri) 
  {
    setBrightnessRGB(_color_mode & LIGHT_MODE_RGB ? bri : 0);
    setBrightnessCCT(_color_mode & LIGHT_MODE_CCT ? bri : 0);
    UpdateInternalColour();
    #ifdef ENABLE_RGBCCT_DEBUG
    // AddLog(LOG_LEVEL_TEST, PSTR("setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)"), R, G, B, _hue, _sat, _briRGB);
    #endif
  }

  uint8_t getBrightness(void) 
  {
    // return the max of _briCCT and _briRGB
    return (_briRGB >= _briCCT) ? _briRGB : _briCCT;
  }

  /************************************************************************************************************************************
  ******* BrightnessRGB *****************************************************************************************************************************
  ************************************************************************************************************************************/
 
  uint8_t setBrightnessRGB(uint8_t bri_rgb) {
    uint8_t prev_bri = _briRGB;
    _briRGB = bri_rgb;
    //needs to map in to RGB? or I need to keep RGBCCT as colour only, with these brightness separate
    if (bri_rgb > 0) { addRGBMode(); }
    UpdateInternalColour();
    return prev_bri;
  }

  uint8_t getBrightnessRGB() {
    return _briRGB;
  }

  uint8_t getBrightnessCCT() {
    return _briCCT;
  }

  /************************************************************************************************************************************
  ******* BrightnessCCT *****************************************************************************************************************************
  ************************************************************************************************************************************/

  // changes the white brightness alone
  uint8_t setBrightnessCCT(uint8_t bri_cct) {
    uint8_t prev_bri = _briCCT;
    _briCCT = bri_cct;
    if (bri_cct > 0) { addCCTMode(); }
    UpdateInternalColour();
    return prev_bri;
  }


  RgbcctColor WithBrightness(){ //legacy
    return GetColourWithBrightnessApplied();
  }

  // uint8_t getMaxWhiteness() {
    
  //   return max(W1,W2);
      
  // }

  /************************************************************************************************************************************
  ******* ColorTemperature *****************************************************************************************************************************
  ************************************************************************************************************************************/

  void setCCTRange(uint16_t cct_min_range, uint16_t cct_max_range) {
    
    _cct_min_range = cct_min_range;
    _cct_max_range = cct_max_range;
  }
  void getCTRange(uint16_t *cct_min_range, uint16_t *cct_max_range) {
    if (cct_min_range) { *cct_min_range = _cct_min_range; }
    if (cct_max_range) { *cct_max_range = _cct_max_range; }
  }

  // is the channel a regular PWM or ColorTemp control
  bool isChannelCCT(uint32_t channel) 
  {
    // if ((LIGHT_TYPE_COLDWARM_ID == _subtype) && (1 == channel)) { return true; }   // PMW reserved for CT
    // if ((LIGHT_TYPE_RGBCW_ID == _subtype) && (4 == channel)) { return true; }   // PMW reserved for CT

  //    if (
  //   // (MODULE_PHILIPS_ID == pCONT_set->my_module_type) || 
  //   (pCONT_set->Settings.flag4.pwm_ct_mode)) {
  //   if ((LST_COLDWARM == subtype) && (1 == channel)) { return true; }   // PMW reserved for CT
  //   if ((LST_RGBCW == subtype) && (4 == channel)) { return true; }   // PMW reserved for CT
  // }
  // return false;
    return false;
  }


  void setColorTempAndBrightnessCCT(uint16_t new_cct, uint8_t briCCT) {
    /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
      * ct = 153 = 6500K = Cold = CCWW = FF00
      * ct = 500 = 2000K = Warm = CCWW = 00FF
      */
    // don't set CT if not supported
    if ((COLOUR_TYPE__COLDWARM__ID != _subtype) && (COLOUR_TYPE__RGBW__ID > _subtype)) {
      return;
    }
    setCCT_Kelvin(new_cct);
    setBrightnessCCT(briCCT);
    if (_cct_rgb_linked) { setColorMode(LIGHT_MODE_CCT); }   // try to force CT
    UpdateInternalColour();
  }

   void setCCT_255Range(uint16_t cct_255) 
  {
    uint16_t cct = map(cct_255, 0, 255, _cct_min_range, _cct_max_range);
    setCCT_Kelvin(cct);
  }


  void setCCT_Kelvin(uint16_t cct) // sets the kelvin value
  {
    
// Serial.printf("_cct_min_range=%d\n\r",_cct_min_range);
// Serial.printf("cct_max_range=%d\n\r",_cct_max_range);
// Serial.printf("cctA=%d\n\r",cct);

    if (0 == cct) {
      // disable cct mode
      setColorMode(LIGHT_MODE_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
    } else {
      cct = (cct < CCT_MIN_DEFAULT ? CCT_MIN_DEFAULT : (cct > CCT_MAX_DEFAULT ? CCT_MAX_DEFAULT : cct));

      WW = map(cct, _cct_min_range, _cct_max_range, 0, 255);
      WC = 255 - WW;
      _cct = cct;
      addCCTMode();
    }
    UpdateInternalColour();
    #ifdef ENABLE_RGBCCT_DEBUG
    // Serial.printf("setCT RGB raw (%d %d %d) HS (%d %d) briRGB (%d) briCCT (%d) CT (%d)", R, G, B, _hue, _sat, _briRGB, _briCCT, _ct);
    #endif
  }

  uint16_t getCCT(){
    return _cct; // 153..500, or CT_MIN..CT_MAX
  }
  uint8_t getCCT255(){
    return map(_cct, _cct_min_range, _cct_max_range, 0, 255);
  }

  uint16_t getCTifEnabled(){
    // don't calculate CT for unsupported devices
    if ((COLOUR_TYPE__COLDWARM__ID != _subtype) && (COLOUR_TYPE__RGBCW__ID != _subtype)) {
      return 0;
    }
    return (getColorMode() & LIGHT_MODE_CCT) ? getCCT() : 0;
  }

  // get the CT value within the range into a 10 bits 0..1023 value
  uint16_t getCCT10bits() {
    return map(_cct, _cct_min_range, _cct_max_range, 0, 1023);
  }

  bool setRGBCCTLinked(bool cct_rgb_linked) {
    
    bool prev = _cct_rgb_linked;
    if (_pwm_multi_channels) {
      _cct_rgb_linked = false;   // force to false if _pwm_multi_channels is set
    } else {
      _cct_rgb_linked = cct_rgb_linked;
    }
    return prev;
  }

  bool getRGBCCTLinked(){
    return _cct_rgb_linked;
  }


  /************************************************************************************************************************************
  ******* PWMChannels ie Direct control *****************************************************************************************************************************
  ************************************************************************************************************************************/

  bool setPWMMultiChannel(bool pwm_multi_channels) {
    bool prev = _pwm_multi_channels;
    _pwm_multi_channels = pwm_multi_channels;
    if (pwm_multi_channels)  setRGBCCTLinked(false);    // if pwm multi channel, then unlink RGB and CT
    return prev;
  }

  bool isPWMMultiChannel(void) {
    return _pwm_multi_channels;
  }


  static uint32_t GetU32ColourBrightnessApplied(RgbcctColor c)
  {
    
    // GetColourWithBrightnessApplied
    

    bool rgb_channels_on = c._color_mode & LIGHT_MODE_RGB;
    bool ct_channels_on =   c._color_mode & LIGHT_MODE_CCT;

    RgbcctColor adjusted_colour;
    
    adjusted_colour.R = rgb_channels_on ? map(c.red, 0, 255, 0, c._briRGB) : 0;
    adjusted_colour.G = rgb_channels_on ? map(c.green, 0, 255, 0, c._briRGB) : 0;
    adjusted_colour.B = rgb_channels_on ? map(c.blue, 0, 255, 0, c._briRGB) : 0;

    adjusted_colour.W1 = ct_channels_on  ? map(c.W1, 0, 255, 0, c._briCCT) : 0; 
    // adjusted_colour.WW = ct_channels_on  ? map(c.warm_white, 0, 255, 0, _briCCT) : 0;


      uint32_t c_out =
          ((uint32_t)adjusted_colour.W1 << 24) |
          ((uint32_t)adjusted_colour.R  << 16) |
          ((uint32_t)adjusted_colour.G  <<  8) |
          ((uint32_t)adjusted_colour.B       ) ;

      return c_out;
  }

  void debug_print(char* name = nullptr)
  {
    Serial.printf("%s%s%d,%d,%d,%d,%d\n\r", name==nullptr?"":name, name==nullptr?"":"=",  R, G, B, WC, WW);
  }

  void debug_print_states(char* name = nullptr)
  {
    Serial.printf("%s%sHS(%d %d) briRGB(%d) briCCT(%d) cct(%d)\n\r", name==nullptr?"":name, name==nullptr?"":"=", _hue, _sat, _briRGB, _briCCT, _cct);
  }





};

