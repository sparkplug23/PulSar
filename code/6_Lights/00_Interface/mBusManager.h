#ifndef BusManager_h
#define BusManager_h

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE

#include "const.h"
#include "mBusNeoWrapper.h"
#include <Arduino.h>
#include <NeoPixelBus.h>
#include <NeoPixelAnimator.h>

#include "mHardwareColourType.h"

#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
#define R(c) (byte((c) >> 16))
#define G(c) (byte((c) >> 8))
#define B(c) (byte(c))
#define W(c) (byte((c) >> 24))

#define GET_BIT(var,bit)    (((var)>>(bit))&0x01)
#define SET_BIT(var,bit)    ((var)|=(uint16_t)(0x0001<<(bit)))
#define UNSET_BIT(var,bit)  ((var)&=(~(uint16_t)(0x0001<<(bit))))

#define NUM_ICS_WS2812_1CH_3X(len) (((len)+2)/3)   // 1 WS2811 IC controls 3 zones (each zone has 1 LED, W)
#define IC_INDEX_WS2812_1CH_3X(i)  ((i)/3)

#define NUM_ICS_WS2812_2CH_3X(len) (((len)+1)*2/3) // 2 WS2811 ICs control 3 zones (each zone has 2 LEDs, CW and WW)
#define IC_INDEX_WS2812_2CH_3X(i)  ((i)*2/3)
#define WS2812_2CH_3X_SPANS_2_ICS(i) ((i)&0x01)    // every other LED zone is on two different ICs



// Temporary struct for passing bus configuration to bus
struct BusConfig {
  uint8_t type;
  uint16_t count;
  uint16_t start;
  COLOUR_ORDER_T colorOrder;
  bool reversed;
  uint8_t skipAmount;
  bool refreshReq;
  uint8_t autoWhite;
  uint8_t pins[5] = {LEDPIN, 255, 255, 255, 255};

  BusConfig(
    uint8_t busType, 
    uint8_t* ppins, 
    uint16_t pstart, 
    uint16_t length = 1,
    COLOUR_ORDER_T _ColourOrder = {COLOUR_ORDER_INIT_DISABLED},
    bool rev = false, 
    uint8_t skip = 0, 
    byte aw = RGBW_MODE_MANUAL_ONLY
  ){
        
    refreshReq = (bool) GET_BIT(busType,7);
    type = busType & 0x7F;  // bit 7 may be/is hacked to include refresh info (1=refresh in off state, 0=no refresh)
    count = length; 
    start = pstart; 
    reversed = rev; 
    skipAmount = skip; 
    autoWhite = aw;

    uint8_t nPins = 1;
    if (type >= BUSTYPE_NET_DDP_RGB && type < 96) nPins = 4; //virtual network bus. 4 "pins" store IP address
    else if (type > 47) nPins = 2;
    else if (type > 40 && type < 46) nPins = NUM_BUSTYPE_PWM_PINS(type);
    for (uint8_t i = 0; i < nPins; i++) pins[i] = ppins[i];

    colorOrder = _ColourOrder;

  }

  //validates start and length and extends total if needed
  bool adjustBounds(uint16_t& total) {
    if (!count) count = 1;
    if (count > MAX_LEDS_PER_BUS) count = MAX_LEDS_PER_BUS;
    if (start >= MAX_LEDS) return false;
    //limit length of strip if it would exceed total permissible LEDs
    if (start + count > MAX_LEDS) count = MAX_LEDS - start;
    //extend total count accordingly
    if (start + count > total) total = start + count;
    return true;
  }
};



// Defines an LED Strip and its color ordering.
struct ColorOrderMapEntry 
{
  uint16_t start;
  uint16_t len;
  COLOUR_ORDER_T colorOrder;
};


struct ColorOrderMap 
{
    void add(uint16_t start, uint16_t len, COLOUR_ORDER_T colorOrder);

    uint8_t count() const 
    {
      return _count;
    }

    void reset() 
    {
      _count = 0;
      memset(_mappings, 0, sizeof(_mappings));
    }

    const ColorOrderMapEntry* get(uint8_t n) const 
    {
      if (n > _count) {
        return nullptr;
      }
      return &(_mappings[n]);
    }

    COLOUR_ORDER_T getPixelColorOrder(uint16_t pix, COLOUR_ORDER_T defaultColorOrder) const;

  private:
    uint8_t _count;
    ColorOrderMapEntry _mappings[WLED_MAX_COLOR_ORDER_MAPPINGS];
};

// Parent class of BusDigital, BusPwm, and BusNetwork
class Bus {
  public:
    Bus(uint8_t type, uint16_t start, uint8_t aw)
    : _bri(255)
    , _len(1)
    , _valid(false)
    , _needsRefresh(false)
    {
      _type = type;
      _start = start;
      _autoWhiteMode = Bus::hasWhite(_type) ? aw : RGBW_MODE_MANUAL_ONLY;
    };

    virtual ~Bus(){}

    virtual void     show() = 0;
    virtual bool     canShow() { return true; }
    
    virtual void     setPixelColor(uint16_t pix, RgbcctColor c) = 0;
    virtual RgbcctColor getPixelColor(uint16_t pix) { return 0; }

    virtual void     setBrightness(uint8_t b) { _bri = b; };
    virtual void     cleanup() = 0;
    virtual uint8_t  getPins(uint8_t* pinArray) { return 0; }
    virtual uint16_t getLength() { return _len; }
    virtual void     setColorOrder() {}
    virtual COLOUR_ORDER_T  getColorOrder() { return {0}; }
    virtual uint8_t  skippedLeds() { return 0; }
    inline  uint16_t getStart() { return _start; }
    inline  void     setStart(uint16_t start) { _start = start; }
    inline  uint8_t  getType() { return _type; }
    inline  bool     isOk() { return _valid; }
    inline  bool     isOffRefreshRequired() { return _needsRefresh; }
            bool     containsPixel(uint16_t pix) { return pix >= _start && pix < _start+_len; }

    virtual bool hasRGB() {
      if ((_type >= BUSTYPE_WS2812_1CH && _type <= BUSTYPE_WS2812_WWA) || _type == BUSTYPE_ANALOG_1CH || _type == BUSTYPE_ANALOG_2CH || _type == BUSTYPE_ONOFF) return false;
      return true;
    }
    virtual bool hasWhite() { return Bus::hasWhite(_type); }
    static  bool hasWhite(uint8_t type) {
      if ((type >= BUSTYPE_WS2812_1CH && type <= BUSTYPE_WS2812_WWA) || type == BUSTYPE_SK6812_RGBW || type == BUSTYPE_TM1814) return true; // digital types with white channel
      if (type > BUSTYPE_ONOFF && type <= BUSTYPE_ANALOG_5CH && type != BUSTYPE_ANALOG_3CH) return true; // analog types with white channel
      if (type == BUSTYPE_NET_DDP_RGBW) return true; // network types with white channel
      return false;
    }
    virtual bool hasCCT() {
      if (_type == BUSTYPE_WS2812_2CH_X3 || _type == BUSTYPE_WS2812_WWA ||
          _type == BUSTYPE_ANALOG_2CH    || _type == BUSTYPE_ANALOG_5CH) return true;
      return false;
    }
    static void setCCT(uint16_t cct) {
      _cct = cct;
    }
    static void setCCTBlend(uint8_t b) {
      if (b > 100) b = 100;
      _cctBlend = (b * 127) / 100;
      //compile-time limiter for hardware that can't power both white channels at max
      #ifdef WLED_MAX_CCT_BLEND
        if (_cctBlend > WLED_MAX_CCT_BLEND) _cctBlend = WLED_MAX_CCT_BLEND;
      #endif
    }
    inline        void    setAutoWhiteMode(uint8_t m) { if (m < 5) _autoWhiteMode = m; }
    inline        uint8_t getAutoWhiteMode()          { return _autoWhiteMode; }
    inline static void    setGlobalAWMode(uint8_t m)  { if (m < 5) _gAWM = m; else _gAWM = AW_GLOBAL_DISABLED; }
    inline static uint8_t getGlobalAWMode()           { return _gAWM; }

    bool reversed = false;

  protected:
    uint8_t  _type;
    uint8_t  _bri;
    uint16_t _start;
    uint16_t _len;
    bool     _valid;
    bool     _needsRefresh;
    uint8_t  _autoWhiteMode;
    static uint8_t _gAWM;
    static int16_t _cct;
    static uint8_t _cctBlend;

    uint32_t autoWhiteCalc(uint32_t c);
};


class BusDigital : public Bus {
  public:
    BusDigital(BusConfig &bc, uint8_t nr, const ColorOrderMap &com);

    inline void show();
    bool canShow();

    void setBrightness(uint8_t b);

    void setPixelColor(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColor(uint16_t pix);

    void setColorOrder(COLOUR_ORDER_T colorOrder);
    COLOUR_ORDER_T getColorOrder() 
    {
      return _colorOrder;
    }

    uint16_t getLength() 
    {
      return _len - _skip;
    }

    uint8_t getPins(uint8_t* pinArray);


    uint8_t skippedLeds() 
    {
      return _skip;
    }

    void reinit();

    void cleanup();

    ~BusDigital() 
    {
      cleanup();
    }

  private:
    COLOUR_ORDER_T _colorOrder = {0};//COL_ORDER_GRB;
    uint8_t _pins[2] = {255, 255};
    uint8_t _iType = 0; //I_NONE;
    uint8_t _skip = 0;
    void * _busPtr = nullptr;
    const ColorOrderMap &_colorOrderMap;
};


class BusPwm : public Bus {
  public:
    BusPwm(BusConfig &bc);

    void setPixelColor(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColor(uint16_t pix);
    
    void show();

    uint8_t getPins(uint8_t* pinArray);

    void cleanup() 
    {
      deallocatePins();
    }

    ~BusPwm() 
    {
      cleanup();
    }
      
    byte allocateLedc(byte channels);
    void deallocateLedc(byte pos, byte channels);

  private:
    uint8_t _pins[5] = {255, 255, 255, 255, 255};
    // uint16_t _data[5] = {0}; // 10 bit
    RgbcctColor output_colour;
    #ifdef ARDUINO_ARCH_ESP32
    uint8_t _ledcStart = 255;
    #endif
    uint8_t ledcAlloc[2] = {0x00, 0x00}; //16 LEDC channels

    void deallocatePins();
};


class BusOnOff : public Bus {
  public:
    BusOnOff(BusConfig &bc);

    void setPixelColor(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColor(uint16_t pix);

    void show();

    uint8_t getPins(uint8_t* pinArray);

    void cleanup() 
    {
      
    }

    ~BusOnOff() 
    {
      cleanup();
    }

  private:
    uint8_t _pin = 255;
    uint8_t _data = 0;
};


class BusNetwork : public Bus 
{
  public:
    BusNetwork(BusConfig &bc);

    bool hasRGB() { return true; }
    bool hasWhite() { return _rgbw; }

    void setPixelColor(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColor(uint16_t pix);

    void show();

    bool canShow() 
    { // this should be a return value from UDP routine if it is still sending data out
      return !_broadcastLock;
    }

    uint8_t getPins(uint8_t* pinArray);

    uint16_t getLength() 
    {
      return _len;
    }

    void cleanup();

    ~BusNetwork() 
    {
      cleanup();
    }

  private:
    IPAddress _client;
    uint8_t   _UDPtype;
    uint8_t   _UDPchannels;
    bool      _rgbw;
    bool      _broadcastLock;
    byte     *_data;
};


class BusManager 
{
  public:
    BusManager(){};

    // Utility to get the approx. memory usage of a given BusConfig
    static uint32_t memUsage(BusConfig &bc);

    int add(BusConfig &bc);

    //do not call this method from system context (network callback)
    void removeAll();

    void show();

    void IRAM_ATTR setPixelColor(uint16_t index, RgbcctColor c, int16_t cct=-1);
    RgbcctColor getPixelColor(uint16_t pix);

    void setBrightness(uint8_t b);
    void setSegmentCCT(int16_t cct, bool allowWBCorrection = false);

    bool canAllShow();

    Bus* getBus(uint8_t busNr);

    uint16_t getTotalLength(); //semi-duplicate of strip.getLengthTotal() (though that just returns strip._length, calculated in finalizeInit())

    inline void updateColorOrderMap(const ColorOrderMap &com) 
    {
      memcpy(&colorOrderMap, &com, sizeof(ColorOrderMap));
    }

    inline const ColorOrderMap& getColorOrderMap() const 
    {
      return colorOrderMap;
    }

    inline uint8_t getNumBusses() 
    {
      return numBusses;
    }

    Bus* busses[WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES];

  private:
    uint8_t numBusses = 0;
    ColorOrderMap colorOrderMap;

    inline uint8_t getNumVirtualBusses() 
    {
      int j = 0;
      for (int i=0; i<numBusses; i++) 
      {
        if (busses[i]->getType() >= BUSTYPE_NET_DDP_RGB && busses[i]->getType() < 96)
        { 
          j++;
        }
      }
      // Serial.printf("getNumVirtualBusses \n\r\n\r\n\r\n\r", numBusses);
      return j;
    }

};

#endif // USE_MODULE_LIGHTS_INTERFACE

#endif // BusManager_h
