#ifndef MWLEDEFFECTS_prog2_H
#define MWLEDEFFECTS_prog2_H 0.1

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS

class WS2812FX 
{
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
      colorOrder = 1, // 0 = GRB, 1 = RGB
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
      realPixelIndex(uint16_t i),
      phased_base(uint8_t);

    CRGB twinklefox_one_twinkle(uint32_t ms, uint8_t salt, bool cat);
    CRGB pacifica_one_layer(uint16_t i, CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff);
    
    uint32_t _lastPaletteChange = 0;
    uint32_t _lastShow = 0;
        
    uint8_t _segment_index = 0;
    uint8_t _segment_index_palette_last = 99;
    segment _segments[MAX_NUM_SEGMENTS] = { // SRAM footprint: 24 bytes per element
      // start, stop, speed, intensity, palette, mode, options, grouping, spacing, opacity (unused), color[]
      { 0, 7, DEFAULT_SPEED, 128, 0, DEFAULT_MODE, NO_OPTIONS, 1, 0, 255, {DEFAULT_COLOR}}
    };
    segment_runtime _segment_runtimes[MAX_NUM_SEGMENTS]; // SRAM footprint: 28 bytes per element
    friend class Segment_runtime;

};

#endif

#endif

