/**
 * @file mBusManager.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-04-06
 * 
 * @copyright Copyright (c) 2024
 * 
 * @note This file is a derivative of the original WLED project.
 *       The CCT funtionality has been removed, and instead will be handled by effect functions.
 *       The auto white will remain, where the white channel will be adjusted based on the RGB values when desired, but otherwise will be calculated in the effects. 
 * 
 */
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

#define NUM_ICS_WS2812_1CH_3X(len)    (((len)+2)/3)   // 1 WS2811 IC controls 3 zones (each zone has 1 LED, W)
#define IC_INDEX_WS2812_1CH_3X(i)     ((i)/3)

#define NUM_ICS_WS2812_2CH_3X(len)    (((len)+1)*2/3) // 2 WS2811 ICs control 3 zones (each zone has 2 LEDs, CW and WW)
#define IC_INDEX_WS2812_2CH_3X(i)     ((i)*2/3)
#define WS2812_2CH_3X_SPANS_2_ICS(i)  ((i)&0x01)    // every other LED zone is on two different ICs


DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_1CH__CTR) "WS2812_1CH";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_1CH_X3__CTR) "WS2812_1CH_X3";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_2CH_X3__CTR) "WS2812_2CH_X3";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_WWA__CTR) "WS2812_WWA";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2812_RGB__CTR) "WS2812_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__GS8608__CTR) "GS8608";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2811_400KHZ__CTR) "WS2811_400KHZ";
DEFINE_PGM_CTR(PM_BUSTYPE__TM1829__CTR) "TM1829";
DEFINE_PGM_CTR(PM_BUSTYPE__SK6812_RGBW__CTR) "SK6812_RGBW";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2805_RGBWW__CTR) "WS2805_RGBWW";
DEFINE_PGM_CTR(PM_BUSTYPE__TM1814__CTR) "TM1814";
DEFINE_PGM_CTR(PM_BUSTYPE__ONOFF__CTR) "ONOFF";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_1CH__CTR) "ANALOG_1CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_2CH__CTR) "ANALOG_2CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_3CH__CTR) "ANALOG_3CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_4CH__CTR) "ANALOG_4CH";
DEFINE_PGM_CTR(PM_BUSTYPE__ANALOG_5CH__CTR) "ANALOG_5CH";
DEFINE_PGM_CTR(PM_BUSTYPE__WS2801__CTR) "WS2801";
DEFINE_PGM_CTR(PM_BUSTYPE__APA102__CTR) "APA102";
DEFINE_PGM_CTR(PM_BUSTYPE__LPD8806__CTR) "LPD8806";
DEFINE_PGM_CTR(PM_BUSTYPE__P9813__CTR) "P9813";
DEFINE_PGM_CTR(PM_BUSTYPE__LPD6803__CTR) "LPD6803";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_DDP_RGB__CTR) "NET_DDP_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_E131_RGB__CTR) "NET_E131_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_ARTNET_RGB__CTR) "NET_ARTNET_RGB";
DEFINE_PGM_CTR(PM_BUSTYPE__NET_DDP_RGBW__CTR) "NET_DDP_RGBW";
DEFINE_PGM_CTR(PM_BUSTYPE__RESERVED__CTR) "RESERVED";

// Temporary struct for passing bus configuration to bus
struct BusConfig 
{
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

  // Validates start and length and extends total if needed
  bool adjustBounds(uint16_t& total) 
  {
    if (!count) count = 1;
    if (count > MAX_LEDS_PER_BUS) count = MAX_LEDS_PER_BUS;
    if (start >= MAX_LEDS_NEO) return false;
    // Limit length of strip if it would exceed total permissible LEDs
    if (start + count > MAX_LEDS_NEO) count = MAX_LEDS_NEO - start;
    // Extend total count accordingly
    if (start + count > total) total = start + count;
    return true;
  }

};


// Defines an LED Strip and its color ordering.
struct ColorOrderMapEntry 
{
  // Mappings here sets what order to use and between which index range
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
    uint8_t _count = 0;
    ColorOrderMapEntry _mappings[WLED_MAX_COLOR_ORDER_MAPPINGS];
};

// Parent class of BusDigital, BusPwm, and BusNetwork
class Bus {
  public:
    Bus(uint8_t type, uint16_t start, uint8_t aw)
    : _len(1)
    , _valid(false)
    , _needsRefresh(false)
    {
      _type = type;
      _start = start;
      #ifdef ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES
      _autoWhiteMode = Bus::hasWhite(_type) ? aw : RGBW_MODE_MANUAL_ONLY;
      #endif // ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES
    };

    virtual ~Bus(){}

    virtual void     show() = 0;
    virtual bool     canShow() { return true; }
    
    virtual void     setPixelColor(uint16_t pix, RgbcctColor c) = 0;
    virtual RgbcctColor getPixelColor(uint16_t pix) { return RgbcctColor(); }

    virtual void     cleanup() = 0;
    virtual uint8_t  getPins(uint8_t* pinArray) { return 0; }
    virtual uint16_t getLength() { return _len; }
    virtual void     setColorOrder() {}
    virtual COLOUR_ORDER_T  getColorOrder() { return {0}; }
    virtual uint8_t  skippedLeds() { return 0; }
    inline  uint16_t getStart() { return _start; }
    inline  void     setStart(uint16_t start) { _start = start; }
    inline  uint8_t  getType() { return _type; }

    const char* getTypeName();
    const char* getTypeName(uint8_t id);

    static uint8_t getTypeIDbyName(const char* name);

    inline  bool     isOk() { return _valid; }
    inline  bool     isOffRefreshRequired() { return _needsRefresh; }
            bool     containsPixel(uint16_t pix) { return pix >= _start && pix < _start+_len; }

    virtual bool hasRGB() {
      if ((_type >= BUSTYPE_WS2812_1CH && _type <= BUSTYPE_WS2812_WWA) || _type == BUSTYPE_ANALOG_1CH || _type == BUSTYPE_ANALOG_2CH || _type == BUSTYPE_ONOFF) return false;
      return true;
    }
    virtual bool hasWhite() { return Bus::hasWhite(_type); }
    static  bool hasWhite(uint8_t type) {
      if ((type >= BUSTYPE_WS2812_1CH && type <= BUSTYPE_WS2812_WWA) || type == BUSTYPE_SK6812_RGBW || type == BUSTYPE_WS2805_RGBWW || type == BUSTYPE_TM1814) return true; // digital types with white channel
      if (type > BUSTYPE_ONOFF && type <= BUSTYPE_ANALOG_5CH && type != BUSTYPE_ANALOG_3CH) return true; // analog types with white channel
      if (type == BUSTYPE_NET_DDP_RGBW) return true; // network types with white channel
      return false;
    }
    virtual bool hasCCT() {
      if (_type == BUSTYPE_WS2812_2CH_X3 || _type == BUSTYPE_WS2812_WWA ||
          _type == BUSTYPE_ANALOG_2CH    || _type == BUSTYPE_ANALOG_5CH) return true;
      return false;
    }

    #ifdef ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES    
    inline        void    setAutoWhiteMode(uint8_t m) { if (m < 5) _autoWhiteMode = m; }
    inline        uint8_t getAutoWhiteMode()          { return _autoWhiteMode; }
    inline static void    setGlobalAWMode(uint8_t m)  { if (m < 5) _gAWM = m; else _gAWM = AW_GLOBAL_DISABLED; }
    inline static uint8_t getGlobalAWMode()           { return _gAWM; }
    #endif // ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES

    bool reversed = false;

    uint16_t _start;
    uint16_t _len;
  protected:
    uint8_t  _type;
    bool     _valid;
    bool     _needsRefresh;

    #ifdef ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES
    uint8_t  _autoWhiteMode;
    static uint8_t _gAWM;
    #endif // ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES

    uint32_t autoWhiteCalc(uint32_t c);
};


class BusDigital : public Bus {
  public:
    BusDigital(BusConfig &bc, uint8_t nr, const ColorOrderMap &com);

    inline void show();
    bool canShow();

    void setBrightness(uint8_t b);

    void        setPixelColor(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColor(uint16_t pix);

    void           setColorOrder(COLOUR_ORDER_T colorOrder);
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
    COLOUR_ORDER_T _colorOrder = {COLOUR_ORDER_INIT_DISABLED};
    const ColorOrderMap &_colorOrderMap;
    uint8_t _pins[2] = {255, 255};
    uint8_t _iType = 0;
    uint8_t _skip = 0;
    void* _busPtr = nullptr;
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

    void setColorOrder(COLOUR_ORDER_T colorOrder);
    COLOUR_ORDER_T getColorOrder() 
    {
      return _colorOrder;
    }
      
    byte allocateLedc(byte channels);
    void deallocateLedc(byte pos, byte channels);

  private:
    uint8_t _pins[5] = {255, 255, 255, 255, 255};
    RgbcctColor output_colour;
    #ifdef ARDUINO_ARCH_ESP32
    uint8_t _ledcStart = 255;
    #endif
    uint8_t ledcAlloc[2] = {0x00, 0x00}; //16 LEDC channels

    COLOUR_ORDER_T _colorOrder = {COLOUR_ORDER_INIT_DISABLED};

    void deallocatePins();
};

/**
 * @brief Potential use case, driven traditional christmas light controllers
 * 
 */
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
      Serial.println("updateColorOrderMap");
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

    const char* getColourOrderName(COLOUR_ORDER_T _colorOrder, char* buffer, uint8_t len);

    Bus* busses[WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES] = {nullptr};

  private:
    uint8_t numBusses = 0;
    ColorOrderMap colorOrderMap;  // which contains all the maps, really this could just be held within the bus? Or is it since busses are different types?

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
