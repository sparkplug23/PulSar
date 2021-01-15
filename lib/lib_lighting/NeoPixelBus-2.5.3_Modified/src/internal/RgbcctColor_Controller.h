#ifndef _M_PALETTE4_H
#define _M_PALETTE4_H 0.1

// #include "0_ConfigUser/mUserConfig.h"

#include "NeoPixelBus.h"

// Consider this as an extension to Rgbccttype in neopixel?

// Add "_FullColour" to getting colours without brightness added

/**
 * 
 * "Set" changes internal commands, "Change" used to also change output, but
 * this must be handled by CommandSet now to automatically do it
 * 
 * 
 * */


enum _LightColorModes {
  _LCM_RGB = 1, 
  _LCM_CT = 2, 
  _LCM_BOTH = 3 
};


enum _LightSubtypes{ 
  _LST_NONE, 
  _LST_SINGLE, 
  _LST_COLDWARM, 
  _LST_RGB,   
  _LST_RGBW, 
  _LST_RGBWC, 
  _LST_RGBCW
};


    // CT min and max
    #define CT_MIN_DEFAULT 153          // 6500K
    #define CT_MAX_DEFAULT 500          // 2000K


class RgbcctColor_Controller{
  private:

    /**
     * Set internal colours, also buffer writer memory location if set
     * */
    uint8_t* external_rgbcct_memory_writer = nullptr; //use reintepret for this, 5 byte address

    // Rgbcct replacement to allow storing in pointer place too
    void set_R(uint8_t r){
      R = r;
      if(external_rgbcct_memory_writer != nullptr){ *(external_rgbcct_memory_writer+0) = R; }
    }
    void set_G(uint8_t g){
      G = g;
      if(external_rgbcct_memory_writer != nullptr){ *(external_rgbcct_memory_writer+1) = G; }
    }
    void set_B(uint8_t b){
      B = b;
      if(external_rgbcct_memory_writer != nullptr){ *(external_rgbcct_memory_writer+2) = B; }
    }
    void set_WW(uint8_t ww){
      WW = ww;
      if(external_rgbcct_memory_writer != nullptr){ *(external_rgbcct_memory_writer+3) = WW; }
    }
    void set_WC(uint8_t wc){
      WC = wc;
      if(external_rgbcct_memory_writer != nullptr){ *(external_rgbcct_memory_writer+4) = WC; }
    }
    

    uint16_t _hue = 0;  // 0..359
    uint8_t  _sat = 255;  // 0..255
    
    uint8_t  _r = 255;  // 0..255
    uint8_t  _g = 255;  // 0..255
    uint8_t  _b = 255;  // 0..255
    
    uint8_t  _briRGB = 255;  // 0..255
    uint8_t  _briCT = 255;

    // are RGB and CT linked, i.e. if we set CT then RGB channels are off
    bool     _ct_rgb_linked = true;
    bool     _pwm_multi_channels = false;    // treat each channel as independant dimmer
    uint8_t  _subtype = 0;  // local copy of Light.subtype, if we need multiple lights
    uint16_t _ct = CT_MIN_DEFAULT;  // 153..500, default to 153 (cold white)
    uint8_t  _color_mode = _LCM_RGB; // RGB by default
    // Please note that you can still set CT to 153..500, but any value below _ct_min_range or above _ct_max_range not change the CT
    uint16_t _ct_min_range = CT_MIN_DEFAULT;   // the minimum CT rendered range
    uint16_t _ct_max_range = CT_MAX_DEFAULT;   // the maximum CT rendered range

    
    union {
      struct {
        union {
            uint8_t r;
            uint8_t R;
            uint8_t red;
        };
        union {
            uint8_t g;
            uint8_t G;
            uint8_t green;
        };
        union {
            uint8_t b;
            uint8_t B;
            uint8_t blue;
        };
        union {
            uint8_t wc;
            uint8_t WC;
            uint8_t white_cold;
        };
        union {
            uint8_t ww;
            uint8_t WW;
            uint8_t warm_white;
        };
      };
      uint8_t raw[5];
    };

    
    /// Array access operator to index into the crgb object
    inline uint8_t& operator[] (uint8_t x) __attribute__((always_inline))
    {
        return raw[x];
    }

    /// Array access operator to index into the crgb object
    inline const uint8_t& operator[] (uint8_t x) const __attribute__((always_inline))
    {
        return raw[x];
    }

    // RgbcctColor 

  //    /// allow assignment from one RGB struct to another
	// inline RgbcctColor& operator= (const CRGB& rhs) __attribute__((always_inline))
  //   {
  //       r = rhs.r;
  //       g = rhs.g;
  //       b = rhs.b;
  //       return *this;
  //   }

        
    // // new version with only integer computing
    // // brightness is not needed, it is controlled via Dimmer
    void RgbToHsb(uint8_t ir, uint8_t ig, uint8_t ib, uint16_t *r_hue, uint8_t *r_sat, uint8_t *r_bri) {
      uint32_t r = ir;
      uint32_t g = ig;
      uint32_t b = ib;
      uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255
      uint32_t min = (r < g && r < b) ? r : (g < b) ? g : b;   // 0..255
      uint32_t d = max - min;   // 0..255

      uint16_t hue = 0;   // hue value in degrees ranges from 0 to 359
      uint8_t sat = 0;    // 0..255
      uint8_t bri = max;  // 0..255

      if (d != 0) {
        sat = map(d, 0, max, 0, 255);
        if (r == max) {
          hue = (g > b) ?       map(g-b,0,d,0,60) : 360 - map(b-g,0,d,0,60);
        } else if (g == max) {
          hue = (b > r) ? 120 + map(b-r,0,d,0,60) : 120 - map(r-b,0,d,0,60);
        } else {
          hue = (r > g) ? 240 + map(r-g,0,d,0,60) : 240 - map(g-r,0,d,0,60);
        }
        hue = hue % 360;    // 0..359
      }

      if (r_hue) *r_hue = hue;
      if (r_sat) *r_sat = sat;
      if (r_bri) *r_bri = bri;
    #ifdef DEBUG_LIGHT
      AddLog_P(LOG_LEVEL_DEBUG_MORE, "RgbToHsb rgb (%d %d %d) hsb (%d %d %d)", r, g, b, hue, sat, bri);
    #endif
    }

    // uint32_t to uint8_t might cause issues
    void HsToRgb(uint16_t hue, uint8_t sat, uint8_t *r_r, uint8_t *r_g, uint8_t *r_b) {
      uint8_t r = 255; // default to white
      uint8_t g = 255;
      uint8_t b = 255; // we take brightness at 100%, brightness should be set separately
      hue = hue % 360;  // normalize to 0..359
      if (sat > 0) {
        uint8_t i = hue / 60;   // quadrant 0..5
        uint8_t f = hue % 60;   // 0..59
        uint8_t q = 255 - map(f, 0, 60, 0, sat);  // 0..59
        uint8_t p = 255 - sat;
        uint8_t t = 255 - map(60 - f, 0, 60, 0, sat);
        switch (i) {
          case 0: g = t; b = p; break;
          case 1: r = q; b = p; break;
          case 2: r = p; b = t; break;
          case 3: r = p; g = q; break;
          case 4: r = t; g = p; break;
          default: g = p; b = q; break;
          }
        }
      if (r_r)  *r_r = r;
      if (r_g)  *r_g = g;
      if (r_b)  *r_b = b;
    }

    

    void addRGBMode() {
      setColorMode(_color_mode |_LCM_RGB);
    }
    void addCTMode() {
      setColorMode(_color_mode |_LCM_CT);
    }

    /**
     * UpdateInternalColour
     * Using latest values, update
     * */
    void UpdateInternalColour()
    {
      
      RgbcctColor current = GetColourWithBrightnessApplied();

      uint8_t briRGB, briCT;
      briRGB = getBriRGB();
      briCT  = getBriCT();
      
      #ifdef ENABLE_LOG_LEVEL_DEBUG
      AddLog_P(LOG_LEVEL_DEBUG_MORE, "After1 brightness _current_color (%d %d %d %d %d -- rgb%d ct%d)", 
        current.R,current.G,current.B,current.WC, current.WW,
        briRGB,briCT
      );
      #endif // ENABLE_LOG_LEVEL_DEBUG

      switch (_subtype) {
        case _LST_NONE:
          // _current_color[0] = 255;
          break;
        case _LST_SINGLE:
          // _current_color[0] = briRGB;
          break;
        case _LST_COLDWARM:
          set_WC(current.WC);
          set_WW(current.WW);
          break;
        case _LST_RGBW:
        case _LST_RGBCW:
          if (_LST_RGBCW == _subtype) {
            set_WC(current.WC);
            set_WW(current.WW);
          } else {
            set_WC(briCT);
            set_WW(briCT);
          }
          // continue
        case _LST_RGB:
          set_R(current.R);
          set_G(current.G);
          set_B(current.B);
          break;
      }
    }

  public:
    RgbcctColor_Controller(){
      // Set defaults
      set_R(0);
      set_G(0);
      set_B(0);
      set_WW(0);
      set_WC(0);

    };

    void Sync(){
      UpdateInternalColour();
    }

    /**
     * External buffer was changed, refresh internal values
     * */
    void UpdateFromExternalBuffer(){

      if(external_rgbcct_memory_writer != nullptr){ 
        R  = *(external_rgbcct_memory_writer+0);
        G  = *(external_rgbcct_memory_writer+1);
        B  = *(external_rgbcct_memory_writer+2);
        WW = *(external_rgbcct_memory_writer+3);
        WC = *(external_rgbcct_memory_writer+4);    
      }

    }


    /**
     * Assumes a pointer to a 5 byte address, which stores the colours as [r,g,b,ww,wc]
     * */
    void setRgbcctColourOutputAddress(uint8_t* addr, bool flag_optional_load_from_address = false){

      external_rgbcct_memory_writer = addr;

      if(flag_optional_load_from_address){
        R = *(addr+0);
        G = *(addr+1);
        B = *(addr+2);
        WW = *(addr+3);
        WC = *(addr+4);
      }     
    
    }


    /*********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ****** Setters **************************************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************/
    
    void setSubType(uint8_t sub_type) {
      _subtype = sub_type;    // set sub_type at initialization, shoudln't be changed afterwards
    }

    // This function is a bit hairy, it will try to match the rerquired
    // colormode with the features of the device:
    //  _LST_NONE:     _LCM_RGB
    //  _LST_SINGLE:   _LCM_RGB
    //  _LST_COLDWARM: _LCM_CT
    //  _LST_RGB:      _LCM_RGB
    //  _LST_RGBW:     _LCM_RGB,_LCM_CT or_LCM_BOTH
    //  _LST_RGBCW:    _LCM_RGB,_LCM_CT or_LCM_BOTH
    uint8_t setColorMode(uint8_t cm) {
      uint8_t prev_cm = _color_mode;
      if (cm <_LCM_RGB) { cm =_LCM_RGB; }
      if (cm >_LCM_BOTH) { cm =_LCM_BOTH; }
      uint8_t maxbri = (_briRGB >= _briCT) ? _briRGB : _briCT;

      switch (_subtype) {
        case _LST_COLDWARM:
          _color_mode =_LCM_CT;
          break;
        case _LST_NONE:
        case _LST_SINGLE:
        case _LST_RGB:
        default:
          _color_mode =_LCM_RGB;
          break;
        case _LST_RGBW:
        case _LST_RGBCW:
          _color_mode = cm;
          break;
      }
      if (_LCM_RGB == _color_mode) {
        _briCT = 0;
        if (0 == _briRGB) { _briRGB = maxbri; }
      }
      if (_LCM_CT == _color_mode) {
        _briRGB = 0;
        if (0 == _briCT) { _briCT = maxbri; }
      }
      //#ifdef DEBUG_LIGHT
      Serial.printf("prev_cm (%d) req_cm (%d) new_cm (%d)\n\r", prev_cm, cm, _color_mode);
      //#endif
      return prev_cm;
    }

    
    // sets RGB and returns the Brightness. Bri is updated unless keep_bri is true
    uint8_t setRGB(uint8_t r, uint8_t g, uint8_t b, bool keep_bri = false) {
      uint16_t hue;
      uint8_t  sat;
      #ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB input (%d %d %d)", r, g, b);
      #endif

      uint32_t max = (r > g && r > b) ? r : (g > b) ? g : b;   // 0..255

      if (0 == max) {
        r = g = b = 255;
        setColorMode(_LCM_CT);   // try deactivating RGB, setColorMode() will check if this is legal
      } else {
        if (255 > max) {
          // we need to normalize rgb
          r = map(r, 0, max, 0, 255); //constrain needed?  changeUIntScale
          g = map(g, 0, max, 0, 255);
          b = map(b, 0, max, 0, 255);
        }
        addRGBMode();
      }
      if (!keep_bri) {
        _briRGB = (_color_mode & _LCM_RGB) ? max : 0;
      }

      RgbToHsb(r, g, b, &hue, &sat, nullptr);
      set_R(r);
      set_G(g);
      set_B(b);
      _hue = hue;
      _sat = sat;
      #ifdef DEBUG_LIGHT
      AddLog_P2(LOG_LEVEL_DEBUG_MORE, "LightStateClass::setRGB RGB raw (%d %d %d) HS (%d %d) bri (%d)", _r, _g, _b, _hue, _sat, _briRGB);
      #endif
      return max;
    }

    void getHSB(uint16_t *hue, uint8_t *sat, uint8_t *bri) {
      if (hue) { *hue = _hue; }
      if (sat) { *sat = _sat; }
      if (bri) { *bri = _briRGB; }
    }

    void changeHue(uint16_t hue_new){
      uint16_t hue = 0;
      uint8_t  sat = 0;
      uint8_t  brt = 0;
      getHSB(&hue,&sat,&brt);  //internal, so can prob do it directly
      changeHSB(hue_new, sat, brt);
    }

    void changeSat(uint8_t sat_new){
      uint16_t hue = 0;
      uint8_t  sat = 0;
      uint8_t  brt = 0;
      getHSB(&hue,&sat,&brt);  //internal, so can prob do it directly
      changeHSB(hue, sat_new, brt);
    }

    void changeBrt255(uint8_t brt){
      setBri(brt);
      UpdateInternalColour();
    }

    
    void changeHSB(uint16_t hue, uint8_t sat, uint8_t briRGB) {
      setHS(hue, sat);
      setBriRGB(briRGB);
      if (_ct_rgb_linked) { setColorMode(_LCM_RGB); }   // try to force RGB
      // saveSettings();
      UpdateInternalColour();
    }

    void setHS(uint16_t hue, uint8_t sat) {
      uint8_t r, g, b;
      HsToRgb(hue, sat, &r, &g, &b);
      set_R(r);
      set_G(g);
      set_B(b);
      _hue = hue;
      _sat = sat;
      addRGBMode();
      #ifdef DEBUG_LIGHT
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("setHS HS (%d %d) rgb (%d %d %d)"), hue, sat, r, g, b);
      AddLog_P(LOG_LEVEL_DEBUG, PSTR("setHS RGB raw (%d %d %d) HS (%d %d) bri (%d)"), R, G, B, _hue, _sat, _briRGB);
      #endif
    }


    // setters -- do not use directly, use the light_controller instead
    // sets both master Bri and whiteBri
    void setBri(uint8_t bri) {
      setBriRGB(_color_mode & _LCM_RGB ? bri : 0);
      setBriCT(_color_mode & _LCM_CT ? bri : 0);
      #ifdef DEBUG_LIGHT
      AddLog_P(LOG_LEVEL_TEST, PSTR("setBri RGB raw (%d %d %d) HS (%d %d) bri (%d)"), R, G, B, _hue, _sat, _briRGB);
      #endif
    }

    void setCTRange(uint16_t ct_min_range, uint16_t ct_max_range) {
      _ct_min_range = ct_min_range;
      _ct_max_range = ct_max_range;
    }


    // // changes the RGB brightness alone
    // uint8_t setBriRGB(uint8_t bri_rgb) {
    //   uint8_t prev_bri = _briRGB;
    //   _briRGB = bri_rgb;
    // //needs to map in to RGB? or I need to keep RGBCCT as colour only, with these brightness separate
    //   if (bri_rgb > 0) { addRGBMode(); }
    //   return prev_bri;
    // }

    
    // changes the white brightness alone
    uint8_t setBriCT(uint8_t bri_ct) {
      uint8_t prev_bri = _briCT;
      _briCT = bri_ct;
      if (bri_ct > 0) { addCTMode(); }
      return prev_bri;
    }

        
    // getBri() is guaranteed to give the same result as setBri() - no rounding errors.
    uint8_t getBri(void) {
      // return the max of _briCT and _briRGB
      return (_briRGB >= _briCT) ? _briRGB : _briCT;
    }


    // is the channel a regular PWM or ColorTemp control
    bool isChannelCT(uint32_t channel) {
      if ((_LST_COLDWARM == _subtype) && (1 == channel)) { return true; }   // PMW reserved for CT
      if ((_LST_RGBCW == _subtype) && (4 == channel)) { return true; }   // PMW reserved for CT
      return false;
    }


    void changeCTB(uint16_t new_ct, uint8_t briCT) {
      /* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
        * ct = 153 = 6500K = Cold = CCWW = FF00
        * ct = 500 = 2000K = Warm = CCWW = 00FF
        */
      // don't set CT if not supported
      if ((_LST_COLDWARM != _subtype) && (_LST_RGBW > _subtype)) {
        return;
      }
      setCT(new_ct);
      setBriCT(briCT);
      if (_ct_rgb_linked) { setColorMode(_LCM_CT); }   // try to force CT
      UpdateInternalColour();
    }

    void setCT(uint16_t ct) {
      if (0 == ct) {
        // disable ct mode
        setColorMode(_LCM_RGB);  // try deactivating CT mode, setColorMode() will check which is legal
      } else {
        ct = (ct < CT_MIN_DEFAULT ? CT_MIN_DEFAULT : (ct > CT_MAX_DEFAULT ? CT_MAX_DEFAULT : ct));

        WW = map(ct, _ct_min_range, _ct_max_range, 0, 255);
        WC = 255 - WW;
        _ct = ct;
        addCTMode();
      }
      #ifdef DEBUG_LIGHT
      AddLog_P(LOG_LEVEL_INFO, PSTR("setCT RGB raw (%d %d %d) HS (%d %d) briRGB (%d) briCT (%d) CT (%d)"), R, G, B, _hue, _sat, _briRGB, _briCT, _ct);
      #endif
    }

    /**
     * SetBrightnessRGB (rnaged 0..255)
     * */
    void SetBrightnessRGB(uint8_t brightness){
      
      // setBriRGB(_color_mode &_LCM_RGB ? bri : 0);
      // setBriCT(_color_mode &_LCM_CT ? bri : 0);

    }

    // changes the RGB brightness alone
    uint8_t setBriRGB(uint8_t bri_rgb) {
      uint8_t prev_bri = _briRGB;
      _briRGB = bri_rgb;
      //needs to map in to RGB? or I need to keep RGBCCT as colour only, with these brightness separate
      if (bri_rgb > 0) { addRGBMode(); }
      return prev_bri;
    }

    // changes the RGB brightness alone
    uint8_t setBriRGB_As_Percentage(uint8_t bri_rgb) {
      return setBriRGB(map(bri_rgb, 0,100, 0,255));
    }

    // changes the RGB brightness alone
    uint8_t changeBriRGB(uint8_t bri_rgb) {
      uint8_t prev_bri = _briRGB;
      _briRGB = bri_rgb;
      //needs to map in to RGB? or I need to keep RGBCCT as colour only, with these brightness separate
      if (bri_rgb > 0) { addRGBMode(); }
      return prev_bri;
    }




    /*********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ****** Getters **************************************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************/

    uint8_t getBriRGB() {
      return _briRGB;
    }
    
    uint8_t getColorMode() {
      return _color_mode;
    }
    uint16_t getHue(){
      return _hue;
    }
    uint8_t getSat(){
      return _sat;
    }

    uint8_t getBriCT() {
      return _briCT;
    }
    
    /**
     * Get RgbcctColor with brightness applied
     * getActualRGBCW
     * */
    RgbcctColor GetColourWithBrightnessApplied(){

      bool rgb_channels_on = _color_mode &_LCM_RGB;
      bool ct_channels_on =   _color_mode &_LCM_CT;

      // #ifdef ENABLE_LOG_LEVEL_INFO
      //   AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR("GetColourWithBrightnessApplied %d,%d,%d,%d,%d %d,%d"),
      //   full_range_colour.R,
      //   full_range_colour.G,
      //   full_range_colour.B,
      //   full_range_colour.WC,
      //   full_range_colour.WW,
      //   // *r,*g,*b,*w,*c,  
      //   _briRGB,_briCT);
      // #endif

      RgbcctColor adjusted_colour;
      
      adjusted_colour.R = rgb_channels_on ? map(R, 0, 255, 0, _briRGB) : 0;
      adjusted_colour.G = rgb_channels_on ? map(G, 0, 255, 0, _briRGB) : 0;
      adjusted_colour.B = rgb_channels_on ? map(B, 0, 255, 0, _briRGB) : 0;

      adjusted_colour.WC = ct_channels_on  ? map(WC, 0, 255, 0, _briCT) : 0; 
      adjusted_colour.WW = ct_channels_on  ? map(WW, 0, 255, 0, _briCT) : 0;

      return adjusted_colour;

    }

    RgbcctColor GetColourWithBrightnessApplied(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *c, uint8_t *w){

      RgbcctColor colour = GetColourWithBrightnessApplied();
      *r = colour.R;
      *g = colour.G;
      *b = colour.B;
      *c = colour.WW;
      *w = colour.WC;
      return colour;

    }

    uint16_t getCT(){
      return _ct; // 153..500, or CT_MIN..CT_MAX
    }

    // get the CT value within the range into a 10 bits 0..1023 value
    uint16_t getCT10bits() {
      return map(_ct, _ct_min_range, _ct_max_range, 0, 1023);
    }

    void getCTRange(uint16_t *ct_min_range, uint16_t *ct_max_range) {
      if (ct_min_range) { *ct_min_range = _ct_min_range; }
      if (ct_max_range) { *ct_max_range = _ct_max_range; }
    }
    
    /*********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ****** Internal Conversions **************************************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************/

      uint8_t DimmerToBri(uint8_t dimmer) {
        return map(dimmer, 0, 100, 0, 255);  // 0..255
      }

      uint8_t BriToDimmer(uint8_t bri) {
        uint8_t dimmer = map(bri, 0, 255, 0, 100);
        // if brightness is non zero, force dimmer to be non-zero too
        if ((dimmer == 0) && (bri > 0)) { dimmer = 1; }
        return dimmer;
      }

      uint8_t getDimmer(uint32_t mode) {
        uint8_t bri;
        switch (mode) {
          case 1:
            bri = getBriRGB();
            break;
          case 2:
            bri = getBriCT();
            break;
          default:
            bri = getBri();
            break;
        }
        return BriToDimmer(bri);
      }



    


// set all 5 channels at once.
// Channels are: R G B CW WW
// Brightness is automatically recalculated to adjust channels to the desired values
void changeChannels(uint8_t *channels) {
  // if (pwm_multi_channels) {
  //   setChannelsRaw(channels);
  // } else if (LST_COLDWARM == subtype) {
  //   // remap channels 0-1 to 3-4 if cold/warm
  //   uint8_t remapped_channels[5] = {0,0,0,channels[0],channels[1]};
  //   setChannels(remapped_channels);
  // } else {
  //   setChannels(channels);
  // }

  // saveSettings();
  UpdateInternalColour();
}


bool CommandSet_RGBCT_Linked(bool ct_rgb_linked) {
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_RGBCCT_LINKED)), ct_rgb_linked);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

  bool prev = _ct_rgb_linked;
  if (_pwm_multi_channels) {
    _ct_rgb_linked = false;   // force to false if _pwm_multi_channels is set
  } else {
    _ct_rgb_linked = ct_rgb_linked;
  }
  return prev;
}


bool isCTRGBLinked() {
  return _ct_rgb_linked;
}


bool setPWMMultiChannel(bool pwm_multi_channels) {
  bool prev = _pwm_multi_channels;
  _pwm_multi_channels = pwm_multi_channels;
  if (pwm_multi_channels)  CommandSet_RGBCT_Linked(false);    // if pwm multi channel, then unlink RGB and CT
  return prev;
}


bool isPWMMultiChannel(void) {
  return _pwm_multi_channels;
}



// If SetOption68 is set, get the brightness for a specific device
uint8_t LightGetBri(uint8_t device) {
  uint8_t bri = 254;   // default value if relay
  // if (pwm_multi_channels) {
  //   if ((device >= device) && (device < device + LST_MAX) && (device <= pCONT_set->devices_present)) {
  //     bri = current_color[device - device];
  //   }
  // } else if (isCTRGBLinked()) {   // standard behavior
  //   if (device == device) {
  //     bri = getBri();
  //   }
  // } else {    // unlinked
  //   if (device == device) {
  //     bri = getBriRGB();
  //   } else if (device == device + 1) {
  //     bri = getBriCT();
  //   }
  // }
  return bri;
}

// If SetOption68 is set, set the brightness for a specific device
void LightSetBri(uint8_t device, uint8_t bri) {
  // if (pwm_multi_channels) {
  //   if ((device >= device) && (device < device + LST_MAX) && (device <= pCONT_set->devices_present)) {
  //     current_color[device - device] = bri;
  //     changeChannels(current_color);
  //   }
  // } else if (isCTRGBLinked()) {  // standard
  //   if (device == device) {
  //     changeBri(bri);
  //   }
  // } else {  // unlinked
  //   if (device == device) {
  //     CommandSet_BrtRGB_255(bri);
  //   } else if (device == device + 1) {
  //     CommandSet_BrtCT_255(bri);
  //   }
  // }
}

void CommandSet_ColourTemp(uint16_t ct)
{
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_LIGHT D_JSON_COMMAND_NVALUE_K(D_JSON_CCT_TEMP)), LightGetColorTemp());
  // #endif //#ifdef ENABLE_LOG_LEVEL_COMMANDS

/* Color Temperature (https://developers.meethue.com/documentation/core-concepts)
 * ct = 153 = 6500K = Cold = CCWW = FF00
 * ct = 600 = 2000K = Warm = CCWW = 00FF  */
  // don't set CT if not supported
  if ((_LST_COLDWARM != _subtype) && (_LST_RGBCW != _subtype)) {
    return;
  }
  changeCTB(ct, getBriCT());
}

uint16_t LightGetColorTemp(void)
{
  // don't calculate CT for unsupported devices
  if ((_LST_COLDWARM != _subtype) && (_LST_RGBCW != _subtype)) {
    return 0;
  }
  return (getColorMode() & _LCM_CT) ? getCT() : 0;
}



void CommandSet_BrtCT_255(uint8_t bri) {
  setBriCT(bri);
  // saveSettings();
  UpdateInternalColour();
}


// compute actual PWM min/max values from DimmerRange
// must be called when DimmerRange is changed or LedTable
void LightCalcPWMRange(void) {
  uint16_t pwm_min=0, pwm_max=1023;

  // pwm_min = change8to10(DimmerToBri(pCONT_set->Settings.dimmer_hw_min));   // default 0
  // pwm_max = change8to10(DimmerToBri(pCONT_set->Settings.dimmer_hw_max));   // default 100
  // if (pCONT_set->Settings.light_correction) {
  //   pwm_min = ledGamma10_10(pwm_min);       // apply gamma correction
  //   pwm_max = ledGamma10_10(pwm_max);       // 0..1023
  // }
  // pwm_min = pwm_min > 0 ? mapvalue(pwm_min, 1, 1023, 1, Settings.pwm_range) : 0;  // adapt range but keep zero and non-zero values
  // pwm_max = mapvalue(pwm_max, 1, 1023, 1, Settings.pwm_range);  // pwm_max cannot be zero

  pwm_min = pwm_min;
  pwm_max = pwm_max;
  //AddLog_P2(LOG_LEVEL_TEST, PSTR("LightCalcPWMRange %d %d - %d %d"), Settings.dimmer_hw_min, Settings.dimmer_hw_max, pwm_min, pwm_max);
}







    /*********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************
     * ****** Base Component Storage **************************************************************************************************
     * ********************************************************************************************************************************
     * ********************************************************************************************************************************/


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





};


#endif // _M_PALETTE_H


