#ifndef BusManager_h
#define BusManager_h

#include "1_TaskerManager/mTaskerManager.h"

#ifdef ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

#include <Arduino.h>
#include <IPAddress.h>
#include "const.h"
#include "pin_manager.h"
#include "bus_wrapper.h"

#include "const.h"
#include "pin_manager.h"
#include "bus_wrapper.h"
#include <Arduino.h>


#ifdef ENABLE_DEBUG_MULTIPIN
  #define DEBUGOUT Serial
  #define DEBUG_PRINT(x) DEBUGOUT.print(x)
  #define DEBUG_PRINTLN(x) DEBUGOUT.println(x); \
                            Serial.flush();
  #define DEBUG_PRINTF(x...) DEBUGOUT.printf(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
  #define DEBUG_PRINTF(x...)
#endif


//color mangling macros
#define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
#define R(c) (byte((c) >> 16))
#define G(c) (byte((c) >> 8))
#define B(c) (byte(c))
#define W(c) (byte((c) >> 24))



/*
 * Class for addressing various light types
 */

#include "const.h"

#define GET_BIT(var,bit)    (((var)>>(bit))&0x01)
#define SET_BIT(var,bit)    ((var)|=(uint16_t)(0x0001<<(bit)))
#define UNSET_BIT(var,bit)  ((var)&=(~(uint16_t)(0x0001<<(bit))))

#define NUM_ICS_WS2812_1CH_3X(len) (((len)+2)/3)   // 1 WS2811 IC controls 3 zones (each zone has 1 LED, W)
#define IC_INDEX_WS2812_1CH_3X(i)  ((i)/3)

#define NUM_ICS_WS2812_2CH_3X(len) (((len)+1)*2/3) // 2 WS2811 ICs control 3 zones (each zone has 2 LEDs, CW and WW)
#define IC_INDEX_WS2812_2CH_3X(i)  ((i)*2/3)
#define WS2812_2CH_3X_SPANS_2_ICS(i) ((i)&0x01)    // every other LED zone is on two different ICs

//temporary struct for passing bus configuration to bus
struct BusConfig {
  uint8_t type;
  uint16_t count;
  uint16_t start;
  uint8_t colorOrder;
  bool reversed;
  uint8_t skipAmount;
  bool refreshReq;
  uint8_t autoWhite;
  uint8_t pins[5] = {LEDPIN, 255, 255, 255, 255};
  BusConfig(uint8_t busType, uint8_t* ppins, uint16_t pstart, uint16_t len = 1, uint8_t pcolorOrder = COL_ORDER_GRB, bool rev = false, uint8_t skip = 0, byte aw=RGBW_MODE_MANUAL_ONLY) {
    refreshReq = (bool) GET_BIT(busType,7);
    type = busType & 0x7F;  // bit 7 may be/is hacked to include refresh info (1=refresh in off state, 0=no refresh)
    count = len; start = pstart; colorOrder = pcolorOrder; reversed = rev; skipAmount = skip; autoWhite = aw;
    uint8_t nPins = 1;
    if (type >= TYPE_NET_DDP_RGB && type < 96) nPins = 4; //virtual network bus. 4 "pins" store IP address
    else if (type > 47) nPins = 2;
    else if (type > 40 && type < 46) nPins = NUM_PWM_PINS(type);
    for (uint8_t i = 0; i < nPins; i++) pins[i] = ppins[i];
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
struct ColorOrderMapEntry {
  uint16_t start;
  uint16_t len;
  uint8_t colorOrder;
};

struct ColorOrderMap {
    void add(uint16_t start, uint16_t len, uint8_t colorOrder);

    uint8_t count() const {
      return _count;
    }

    void reset() {
      _count = 0;
      memset(_mappings, 0, sizeof(_mappings));
    }

    const ColorOrderMapEntry* get(uint8_t n) const {
      if (n > _count) {
        return nullptr;
      }
      return &(_mappings[n]);
    }

    uint8_t getPixelColorOrder(uint16_t pix, uint8_t defaultColorOrder) const;

  private:
    uint8_t _count;
    ColorOrderMapEntry _mappings[WLED_MAX_COLOR_ORDER_MAPPINGS];
};

//parent class of BusDigital, BusPwm, and BusNetwork
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

    virtual ~Bus() {} //throw the bus under the bus

    virtual void     show() = 0;
    virtual bool     canShow() { return true; }
    virtual void     setStatusPixel(uint32_t c) {}
    
    #ifndef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    virtual void     setPixelColor(uint16_t pix, uint32_t c) = 0;
    virtual uint32_t getPixelColor(uint16_t pix) { return 0; }
    #endif //DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED

    virtual void     setPixelColorNew(uint16_t pix, RgbcctColor c) = 0;
    virtual RgbcctColor getPixelColorNew(uint16_t pix) { return 0; }

    virtual void     setBrightness(uint8_t b) { _bri = b; };
    virtual void     cleanup() = 0;
    virtual uint8_t  getPins(uint8_t* pinArray) { return 0; }
    virtual uint16_t getLength() { return _len; }
    virtual void     setColorOrder() {}
    virtual uint8_t  getColorOrder() { return COL_ORDER_RGB; }
    virtual uint8_t  skippedLeds() { return 0; }
    inline  uint16_t getStart() { return _start; }
    inline  void     setStart(uint16_t start) { _start = start; }
    inline  uint8_t  getType() { return _type; }
    inline  bool     isOk() { return _valid; }
    inline  bool     isOffRefreshRequired() { return _needsRefresh; }
            bool     containsPixel(uint16_t pix) { return pix >= _start && pix < _start+_len; }

    virtual bool hasRGB() {
      if ((_type >= TYPE_WS2812_1CH && _type <= TYPE_WS2812_WWA) || _type == TYPE_ANALOG_1CH || _type == TYPE_ANALOG_2CH || _type == TYPE_ONOFF) return false;
      return true;
    }
    virtual bool hasWhite() { return Bus::hasWhite(_type); }
    static  bool hasWhite(uint8_t type) {
      if ((type >= TYPE_WS2812_1CH && type <= TYPE_WS2812_WWA) || type == TYPE_SK6812_RGBW || type == TYPE_TM1814) return true; // digital types with white channel
      if (type > TYPE_ONOFF && type <= TYPE_ANALOG_5CH && type != TYPE_ANALOG_3CH) return true; // analog types with white channel
      if (type == TYPE_NET_DDP_RGBW) return true; // network types with white channel
      return false;
    }
    virtual bool hasCCT() {
      if (_type == TYPE_WS2812_2CH_X3 || _type == TYPE_WS2812_WWA ||
          _type == TYPE_ANALOG_2CH    || _type == TYPE_ANALOG_5CH) return true;
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
    void setStatusPixel(uint32_t c);

    #ifndef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    void setPixelColor(uint16_t pix, uint32_t c);
    uint32_t getPixelColor(uint16_t pix);
    #endif // DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED

    void setPixelColorNew(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColorNew(uint16_t pix);

    uint8_t getColorOrder() 
    {
      return _colorOrder;
    }

    uint16_t getLength() 
    {
      return _len - _skip;
    }

    uint8_t getPins(uint8_t* pinArray);

    void setColorOrder(uint8_t colorOrder);

    uint8_t skippedLeds() 
    {
      return _skip;
    }

    void reinit();

    void cleanup();

    ~BusDigital() {
      cleanup();
    }

  private:
    uint8_t _colorOrder = COL_ORDER_GRB;
    uint8_t _pins[2] = {255, 255};
    uint8_t _iType = 0; //I_NONE;
    uint8_t _skip = 0;
    void * _busPtr = nullptr;
    const ColorOrderMap &_colorOrderMap;
};


class BusPwm : public Bus {
  public:
    BusPwm(BusConfig &bc);

    #ifndef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    void setPixelColor(uint16_t pix, uint32_t c);
    uint32_t getPixelColor(uint16_t pix); //does no index check
    #endif // DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED

    void setPixelColorNew(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColorNew(uint16_t pix);

    void show();

    uint8_t getPins(uint8_t* pinArray);

    void cleanup() {
      deallocatePins();
    }

    ~BusPwm() {
      cleanup();
    }

  private:
    uint8_t _pins[5] = {255, 255, 255, 255, 255};
    uint8_t _data[5] = {0};
    #ifdef ARDUINO_ARCH_ESP32
    uint8_t _ledcStart = 255;
    #endif

    void deallocatePins();
};


class BusOnOff : public Bus {
  public:
    BusOnOff(BusConfig &bc);

    #ifndef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    void setPixelColor(uint16_t pix, uint32_t c);
    uint32_t getPixelColor(uint16_t pix);
    #endif // DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED

    void setPixelColorNew(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColorNew(uint16_t pix);

    void show();

    uint8_t getPins(uint8_t* pinArray);

    void cleanup() {
      // pinManager.deallocatePin(_pin, PinOwner::BusOnOff);
    }

    ~BusOnOff() {
      cleanup();
    }

  private:
    uint8_t _pin = 255;
    uint8_t _data = 0;
};


class BusNetwork : public Bus {
  public:
    BusNetwork(BusConfig &bc);

    bool hasRGB() { return true; }
    bool hasWhite() { return _rgbw; }

    #ifndef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    void setPixelColor(uint16_t pix, uint32_t c);
    uint32_t getPixelColor(uint16_t pix);
    #endif // DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED

    void setPixelColorNew(uint16_t pix, RgbcctColor c);
    RgbcctColor getPixelColorNew(uint16_t pix);

    void show();

    bool canShow() 
    {// this should be a return value from UDP routine if it is still sending data out
      return !_broadcastLock;
    }

    uint8_t getPins(uint8_t* pinArray);

    uint16_t getLength() {
      return _len;
    }

    void cleanup();

    ~BusNetwork() {
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


class BusManager {
  public:
    BusManager() {};

    //utility to get the approx. memory usage of a given BusConfig
    static uint32_t memUsage(BusConfig &bc);

    int add(BusConfig &bc);

    //do not call this method from system context (network callback)
    void removeAll();

    void show();

    void setStatusPixel(uint32_t c);

    #ifndef DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
    void IRAM_ATTR setPixelColor(uint16_t pix, uint32_t c, int16_t cct=-1);
    uint32_t getPixelColor(uint16_t pix);
    #endif // DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED

    void IRAM_ATTR setPixelColorNew(uint16_t index, RgbcctColor c, int16_t cct=-1);
    RgbcctColor getPixelColorNew(uint16_t pix);



    void setBrightness(uint8_t b);

    void setSegmentCCT(int16_t cct, bool allowWBCorrection = false);


    bool canAllShow();

    Bus* getBus(uint8_t busNr);

    //semi-duplicate of strip.getLengthTotal() (though that just returns strip._length, calculated in finalizeInit())
    uint16_t getTotalLength();

    inline void updateColorOrderMap(const ColorOrderMap &com) {
      memcpy(&colorOrderMap, &com, sizeof(ColorOrderMap));
    }

    inline const ColorOrderMap& getColorOrderMap() const {
      return colorOrderMap;
    }

    inline uint8_t getNumBusses() {
      return numBusses;
    }

    Bus* busses[WLED_MAX_BUSSES+WLED_MIN_VIRTUAL_BUSSES];

  private:
    uint8_t numBusses = 0;
    ColorOrderMap colorOrderMap;

    inline uint8_t getNumVirtualBusses() {
      int j = 0;
      for (int i=0; i<numBusses; i++) if (busses[i]->getType() >= TYPE_NET_DDP_RGB && busses[i]->getType() < 96) j++;
      return j;
    }
};




// #define GET_BIT(var,bit)    (((var)>>(bit))&0x01)
// #define SET_BIT(var,bit)    ((var)|=(uint16_t)(0x0001<<(bit)))
// #define UNSET_BIT(var,bit)  ((var)&=(~(uint16_t)(0x0001<<(bit))))

// //color mangling macros
// #define RGBW32(r,g,b,w) (uint32_t((byte(w) << 24) | (byte(r) << 16) | (byte(g) << 8) | (byte(b))))
// #define R(c) (byte((c) >> 16))
// #define G(c) (byte((c) >> 8))
// #define B(c) (byte(c))
// #define W(c) (byte((c) >> 24))

// //temporary struct for passing bus configuration to bus
// struct BusConfig 
// {
//   uint8_t type = 0;
//   uint16_t count;
//   uint16_t start = 0;
//   uint8_t colorOrder;
//   bool reversed;
//   uint8_t skipAmount;
//   bool refreshReq;
//   uint8_t autoWhite = 123;
//   uint8_t pins[5] = {LEDPIN, 255, 255, 255, 255};
//   BusConfig(
//     uint8_t busType, 
//     uint8_t* ppins, 
//     uint16_t pstart, 
//     uint16_t len = 1, 
//     uint8_t pcolorOrder = COL_ORDER_GRB, 
//     bool rev = false, 
//     uint8_t skip = 0, 
//     byte aw=RGBW_MODE_MANUAL_ONLY
//   ){
//     refreshReq = (bool) GET_BIT(busType,7);
//     type = busType & 0x7F;  // bit 7 may be/is hacked to include refresh info (1=refresh in off state, 0=no refresh)
//     count = len; 
//     start = pstart; 
//     colorOrder = pcolorOrder; 
//     reversed = rev; 
//     skipAmount = skip; 
//     autoWhite = aw;
    
//     DEBUG_PRINTF(
//       "BusConfig()\n\r"
//       "\trefreshReq %d\n\r"
//       "\ttype %d\n\r"
//       "\tcount %d\n\r"
//       "\tstart %d\n\r"
//       "\tcolorOrder %d\n\r"
//       "\treversed %d\n\r"
//       "\tskipAmount %d\n\r"
//       "\tautoWhite %d\n\r"
//       , refreshReq, type, count, start, colorOrder, reversed, skipAmount, autoWhite
//     );

//     uint8_t nPins = 1;
//     if (type >= TYPE_NET_DDP_RGB && type < 96)
//     {    
//       DEBUG_PRINTF("Virtual Network: 4 pins for IP\n\r");
//       nPins = 4; // Virtual network bus. 4 "pins" store IP address
//     }    
//     else 
//     if (type > 47)
//     {
//       nPins = 2;
//     }
//     else 
//     if (type > 40 && type < 46)
//     {
//       nPins = NUM_PWM_PINS(type);
//     } 

//     for (uint8_t i = 0; i < nPins; i++) 
//     {
//       pins[i] = ppins[i];
//       DEBUG_PRINTF("pins[%d] => %d\n\r",i,pins[i]);
//     }

//   }

//   //validates start and length and extends total if needed
//   bool adjustBounds(uint16_t& total) 
//   {
//     DEBUG_PRINTF("adjustBounds\n\r");
//     if (!count) count = 1;
//     if (count > MAX_LEDS_PER_BUS) count = MAX_LEDS_PER_BUS;
//     if (start >= MAX_LEDS) return false;
//     //limit length of strip if it would exceed total permissible LEDs
//     if (start + count > MAX_LEDS) count = MAX_LEDS - start;
//     //extend total count accordingly
//     if (start + count > total) total = start + count;
//     return true;
//   }

// }; // END BusConfig

// // Defines an LED Strip and its color ordering.
// struct ColorOrderMapEntry 
// {
//   uint16_t start;
//   uint16_t len;
//   uint8_t colorOrder;
// };

// struct ColorOrderMap 
// {
//   // void add(uint16_t start, uint16_t len, uint8_t colorOrder) 
//   // // {
//   // //   DEBUG_PRINTF("ColorOrderMap::add\n\r");
//   // //   if (_count >= WLED_MAX_COLOR_ORDER_MAPPINGS) {
//   // //     return;
//   // //   }
//   // //   if (len == 0) {
//   // //     return;
//   // //   }
//   // //   if (colorOrder > COL_ORDER_MAX) {
//   // //     return;
//   // //   }
//   // //   _mappings[_count].start = start;
//   // //   _mappings[_count].len = len;
//   // //   _mappings[_count].colorOrder = colorOrder;
//   // //   _count++;
//   // // }

//   // uint8_t count() const 
//   // {
//   //   DEBUG_PRINTF("ColorOrderMap::count\n\r");
//   //   return _count;
//   // }

//   // void reset() 
//   // {
//   //   DEBUG_PRINTF("ColorOrderMap::add\n\r");
//   //   _count = 0;
//   //   memset(_mappings, 0, sizeof(_mappings));
//   // }

//   // const ColorOrderMapEntry* get(uint8_t n) const 
//   // {
//   //   DEBUG_PRINTF("ColorOrderMap::get\n\r");
//   //   if (n > _count) {
//   //     return nullptr;
//   //   }
//   //   return &(_mappings[n]);
//   // }

//   // inline uint8_t IRAM_ATTR getPixelColorOrder(uint16_t pix, uint8_t defaultColorOrder) const 
//   // {
//   //   DEBUG_PRINTF("ColorOrderMap::getPixelColorOrder\n\r");
//   //   if (_count == 0) return defaultColorOrder;
//   //   // upper nibble containd W swap information
//   //   uint8_t swapW = defaultColorOrder >> 4;
//   //   for (uint8_t i = 0; i < _count; i++) {
//   //     if (pix >= _mappings[i].start && pix < (_mappings[i].start + _mappings[i].len)) {
//   //       return _mappings[i].colorOrder | (swapW << 4);
//   //     }
//   //   }
//   //   return defaultColorOrder;
//   // }

//   // private:
//   // uint8_t _count;
//   // ColorOrderMapEntry _mappings[WLED_MAX_COLOR_ORDER_MAPPINGS];

//     void add(uint16_t start, uint16_t len, uint8_t colorOrder);

//     uint8_t count() const {
//       return _count;
//     }

//     void reset() {
//       _count = 0;
//       memset(_mappings, 0, sizeof(_mappings));
//     }

//     const ColorOrderMapEntry* get(uint8_t n) const {
//       if (n > _count) {
//         return nullptr;
//       }
//       return &(_mappings[n]);
//     }

//     uint8_t getPixelColorOrder(uint16_t pix, uint8_t defaultColorOrder) const;

//   private:
//     uint8_t _count;
//     ColorOrderMapEntry _mappings[WLED_MAX_COLOR_ORDER_MAPPINGS];


// };

// //parent class of BusDigital, BusPwm, and BusNetwork
// class Bus {
//   public:
//     Bus(uint8_t type, uint16_t start, uint8_t aw)
//     : _bri(255)
//     , _len(1)
//     , _valid(false)
//     , _needsRefresh(false)
//     {
//       DEBUG_PRINTF("clas Bus::constructor\n\r");
//       _type = type;
//       _start = start;
//       _autoWhiteMode = Bus::isRgbw(_type) ? aw : RGBW_MODE_MANUAL_ONLY;
//     };

//     virtual ~Bus() {} //throw the bus under the bus

//     virtual void     show() = 0;
//     virtual bool     canShow() { return true; }
//     virtual void     setStatusPixel(uint32_t c) {}
//     virtual void     setPixelColor(uint16_t pix, uint32_t c) = 0;
//     virtual uint32_t getPixelColor(uint16_t pix) { return 0; }
//     virtual void     setBrightness(uint8_t b) { _bri = b; };
//     virtual void     cleanup() = 0;
//     virtual uint8_t  getPins(uint8_t* pinArray) { return 0; }
//     virtual uint16_t getLength() { return _len; }
//     virtual void     setColorOrder() {}
//     virtual uint8_t  getColorOrder() { return COL_ORDER_RGB; }
//     virtual uint8_t  skippedLeds() { return 0; }
//     inline  uint16_t getStart() { return _start; }
//     inline  void     setStart(uint16_t start) { _start = start; }
//     inline  uint8_t  getType() { return _type; }
//     inline  bool     isOk() { return _valid; }
//     inline  bool     isOffRefreshRequired() { return _needsRefresh; }
//             bool     containsPixel(uint16_t pix) { return pix >= _start && pix < _start+_len; }

//     virtual bool isRgbw() 
//     { 
//       return Bus::isRgbw(_type); 
//     }
//     static  bool isRgbw(uint8_t type) 
//     {
//       if (type == TYPE_SK6812_RGBW || type == TYPE_TM1814) return true;
//       if (type > TYPE_ONOFF && type <= TYPE_ANALOG_5CH && type != TYPE_ANALOG_3CH) return true;
//       if (type == TYPE_NET_DDP_RGBW) return true;
//       return false;
//     }
//     virtual bool hasRGB() 
//     {
//       if (_type == TYPE_WS2812_1CH || _type == TYPE_WS2812_WWA || _type == TYPE_ANALOG_1CH || _type == TYPE_ANALOG_2CH || _type == TYPE_ONOFF) return false;
//       return true;
//     }
//     virtual bool hasWhite() 
//     {
//       if (_type == TYPE_SK6812_RGBW || _type == TYPE_TM1814 || _type == TYPE_WS2812_1CH || _type == TYPE_WS2812_WWA ||
//           _type == TYPE_ANALOG_1CH || _type == TYPE_ANALOG_2CH || _type == TYPE_ANALOG_4CH || _type == TYPE_ANALOG_5CH || _type == TYPE_NET_DDP_RGBW) return true;
//       return false;
//     }
//     static void setCCT(uint16_t cct) 
//     {
//       _cct = cct;
//     }
// 		static void setCCTBlend(uint8_t b) 
//     {
// 			if (b > 100) b = 100;
// 			_cctBlend = (b * 127) / 100;
// 			//compile-time limiter for hardware that can't power both white channels at max
// 			#ifdef WLED_MAX_CCT_BLEND
// 				if (_cctBlend > WLED_MAX_CCT_BLEND) _cctBlend = WLED_MAX_CCT_BLEND;
// 			#endif
// 		}
// 		inline        void    setAWMode(uint8_t m)        { if (m < 4) _autoWhiteMode = m; }
// 		inline        uint8_t getAWMode()                 { return _autoWhiteMode; }
//     inline static void    setAutoWhiteMode(uint8_t m) { if (m < 4) _gAWM = m; else _gAWM = 255; }
//     inline static uint8_t getAutoWhiteMode()          { return _gAWM; }

//     bool reversed = false;

//   protected:
//     uint8_t  _type;
//     uint8_t  _bri;
//     uint16_t _start;
//     uint16_t _len;
//     bool     _valid;
//     bool     _needsRefresh;
//     uint8_t  _autoWhiteMode;
//     static uint8_t _gAWM;     // definition in FX_fcn.cpp
//     static int16_t _cct;      // definition in FX_fcn.cpp
// 		static uint8_t _cctBlend; // definition in FX_fcn.cpp
  
//     uint32_t autoWhiteCalc(uint32_t c) 
//     {
//       DEBUG_PRINTF("ColorOrderMap::autoWhiteCalc\n\r");
//       uint8_t aWM = _autoWhiteMode;
//       if (_gAWM < 255) aWM = _gAWM;
//       if (aWM == RGBW_MODE_MANUAL_ONLY) return c;
//       uint8_t w = W(c);
//       //ignore auto-white calculation if w>0 and mode DUAL (DUAL behaves as BRIGHTER if w==0)
//       if (w > 0 && aWM == RGBW_MODE_DUAL) return c;
//       uint8_t r = R(c);
//       uint8_t g = G(c);
//       uint8_t b = B(c);
//       w = r < g ? (r < b ? r : b) : (g < b ? g : b);
//       if (aWM == RGBW_MODE_AUTO_ACCURATE) { r -= w; g -= w; b -= w; } //subtract w in ACCURATE mode
//       return RGBW32(r, g, b, w);
//     }
// };


// class BusDigital : public Bus 
// {
//   public:
//   BusDigital(BusConfig &bc, uint8_t nr, const ColorOrderMap &com) : Bus(bc.type, bc.start, bc.autoWhite), _colorOrderMap(com) 
//   {
    
//     DEBUG_PRINTLN("BusDigital() Start");


//     DEBUG_PRINTF("bc.type %d, bc.start %d, bc.autoWhite %d\n\r", bc.type, bc.start, bc.autoWhite);
    
//     if (!IS_DIGITAL(bc.type) || !bc.count){
//       Serial.println("public:BusDigital (!IS_DIGITAL(bc.type)");     
//       return;
//     }
    
//     // if (!pinManager.allocatePin(bc.pins[0], true, PinOwner::BusDigital)){
     
//     //   return;
//     // }

//     _pins[0] = bc.pins[0];
//     if (IS_2PIN(bc.type)) {
//     DEBUG_PRINTLN("public:BusDigital if (IS_2PIN(bc.type)) {");
//       // if (!pinManager.allocatePin(bc.pins[1], true, PinOwner::BusDigital)) {
//       //   cleanup(); return;
//       // }
//       _pins[1] = bc.pins[1];
//     }
//     reversed = bc.reversed;
//     _needsRefresh = bc.refreshReq || bc.type == TYPE_TM1814;
//     _skip = bc.skipAmount;    //sacrificial pixels
//     _len = bc.count + _skip;
//     DEBUG_PRINTLN("public:BusDigital A");



//     _iType = PolyBus::getI(bc.type, _pins, nr);
//     DEBUG_PRINTLN("public:BusDigital B");
//     Serial.println(_iType);
//     if (_iType == I_NONE) return;
//     DEBUG_PRINTLN("public:BusDigital C");
//     _busPtr = PolyBus::create(_iType, _pins, _len, nr);
//     DEBUG_PRINTLN("public:BusDigital D");
//     _valid = (_busPtr != nullptr);
//     _colorOrder = bc.colorOrder;
//     DEBUG_PRINTF("%successfully inited strip %u (len %u) with type %u and pins %u,%u (itype %u)\n", _valid?"S":"Uns", nr, _len, bc.type, _pins[0],_pins[1],_iType);
  
  
  
//     DEBUG_PRINTLN("BusDigital() END");
//   };

//   inline void show() {
//     PolyBus::show(_busPtr, _iType);
//   }

//   inline bool canShow() {
//     return PolyBus::canShow(_busPtr, _iType);
//   }

//   void setBrightness(uint8_t b) {
//     //Fix for turning off onboard LED breaking bus
//     #ifdef LED_BUILTIN
//     if (_bri == 0 && b > 0) {
//       if (_pins[0] == LED_BUILTIN || _pins[1] == LED_BUILTIN) PolyBus::begin(_busPtr, _iType, _pins); 
//     }
//     #endif
//     Bus::setBrightness(b);
//     // PolyBus::setBrightness(_busPtr, _iType, b);
//   }

// 	//If LEDs are skipped, it is possible to use the first as a status LED.
// 	//TODO only show if no new show due in the next 50ms
// 	void setStatusPixel(uint32_t c) 
//   {
//     if (_skip && canShow()) {
//       PolyBus::setPixelColor(_busPtr, _iType, 0, c, _colorOrderMap.getPixelColorOrder(_start, _colorOrder));
//       PolyBus::show(_busPtr, _iType);
//     }
//   }

//   void setPixelColor(uint16_t pix, uint32_t c) 
//   {
//     if (_type == TYPE_SK6812_RGBW || _type == TYPE_TM1814) c = autoWhiteCalc(c);
//     // if (_cct >= 1900) c = 100;//pCONT_lAni->colorBalanceFromKelvin(_cct, c); //color correction from CCT
//     if (reversed) pix = _len - pix -1;
//     else pix += _skip;
//     PolyBus::setPixelColor(_busPtr, _iType, pix, c, _colorOrderMap.getPixelColorOrder(pix+_start, _colorOrder));
//   }

//   uint32_t getPixelColor(uint16_t pix) 
//   {
//     if (reversed) pix = _len - pix -1;
//     else pix += _skip;
//     return PolyBus::getPixelColor(_busPtr, _iType, pix, _colorOrderMap.getPixelColorOrder(pix+_start, _colorOrder));
//   }

//   inline uint8_t getColorOrder() 
//   {
//     return _colorOrder;
//   }

//   uint16_t getLength() 
//   {
//     return _len - _skip;
//   }

//   uint8_t getPins(uint8_t* pinArray) 
//   {
//     uint8_t numPins = IS_2PIN(_type) ? 2 : 1;
//     for (uint8_t i = 0; i < numPins; i++) pinArray[i] = _pins[i];
//     return numPins;
//   }

//   void setColorOrder(uint8_t colorOrder) 
//   {
//     // upper nibble contains W swap information
//     if ((colorOrder & 0x0F) > 5) return;
//     _colorOrder = colorOrder;
//   }

//   inline uint8_t skippedLeds() 
//   {
//     return _skip;
//   }

//   inline void reinit() {
//     PolyBus::begin(_busPtr, _iType, _pins);
//   }

//   void cleanup() 
//   {
//     DEBUG_PRINTLN(F("Digital Cleanup."));
//     PolyBus::cleanup(_busPtr, _iType);
//     _iType = I_NONE;
//     _valid = false;
//     _busPtr = nullptr;
//     // pinManager.deallocatePin(_pins[1], PinOwner::BusDigital);
//     // pinManager.deallocatePin(_pins[0], PinOwner::BusDigital);
//   }

//   ~BusDigital() 
//   {
//     cleanup();
//   }

//   private: 
//   uint8_t _colorOrder = COL_ORDER_GRB;
//   uint8_t _pins[2] = {255, 255};
//   uint8_t _iType = I_NONE;
//   uint8_t _skip = 0;
//   void * _busPtr = nullptr;
//   const ColorOrderMap &_colorOrderMap;
// };


// class BusPwm : public Bus 
// {
//   public:
//   BusPwm(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) {
//     _valid = false;
//     if (!IS_PWM(bc.type)) return;
//     uint8_t numPins = NUM_PWM_PINS(bc.type);

//     #ifdef ESP8266
//     analogWriteRange(255);  //same range as one RGB channel
//     analogWriteFreq(WLED_PWM_FREQ);
//     #else
//     // _ledcStart = pinManager.allocateLedc(numPins);
//     // if (_ledcStart == 255) 
//     // { //no more free LEDC channels
//     //   deallocatePins(); return;
//     // }
//     #endif

//     for (uint8_t i = 0; i < numPins; i++) 
//     {
//       uint8_t currentPin = bc.pins[i];
//       // if (!pinManager.allocatePin(currentPin, true, PinOwner::BusPwm)) 
//       // {
//       //   deallocatePins(); return;
//       // }
//       _pins[i] = currentPin; //store only after allocatePin() succeeds
//       #ifdef ESP8266
//       pinMode(_pins[i], OUTPUT);
//       #else
//       ledcSetup(_ledcStart + i, WLED_PWM_FREQ, 8);
//       ledcAttachPin(_pins[i], _ledcStart + i);
//       #endif
//     }
//     reversed = bc.reversed;
//     _valid = true;
//   };

//   void setPixelColor(uint16_t pix, uint32_t c) 
//   {
//     if (pix != 0 || !_valid) return; //only react to first pixel
// 		if (_type != TYPE_ANALOG_3CH) c = autoWhiteCalc(c);
//     // if (_cct >= 1900 && (_type == TYPE_ANALOG_3CH || _type == TYPE_ANALOG_4CH)) {
//     //   c = colorBalanceFromKelvin(_cct, c); //color correction from CCT
//     // }
//     uint8_t r = R(c);
//     uint8_t g = G(c);
//     uint8_t b = B(c);
//     uint8_t w = W(c);
//     uint8_t cct = 0; //0 - full warm white, 255 - full cold white
//     // if (_cct > -1) {
//     //   if (_cct >= 1900)    cct = (_cct - 1900) >> 5;
//     //   else if (_cct < 256) cct = _cct;
//     // } else {
//     //   cct = (approximateKelvinFromRGB(c) - 1900) >> 5;
//     // }

// 		uint8_t ww, cw;
// 		#ifdef WLED_USE_IC_CCT
// 		ww = w;
// 		cw = cct;
// 		#else
// 		//0 - linear (CCT 127 = 50% warm, 50% cold), 127 - additive CCT blending (CCT 127 = 100% warm, 100% cold)
// 		if (cct       < _cctBlend) ww = 255;
// 		else ww = ((255-cct) * 255) / (255 - _cctBlend);

// 		if ((255-cct) < _cctBlend) cw = 255;
// 		else                       cw = (cct * 255) / (255 - _cctBlend);

// 		ww = (w * ww) / 255; //brightness scaling
// 		cw = (w * cw) / 255;
// 		#endif

//     switch (_type) {
//       case TYPE_ANALOG_1CH: //one channel (white), relies on auto white calculation
//         _data[0] = w;
//         break;
//       case TYPE_ANALOG_2CH: //warm white + cold white
//         _data[1] = cw;
//         _data[0] = ww;
//         break;
//       case TYPE_ANALOG_5CH: //RGB + warm white + cold white
//         _data[4] = cw;
//         w = ww;
//       case TYPE_ANALOG_4CH: //RGBW
//         _data[3] = w;
//       case TYPE_ANALOG_3CH: //standard dumb RGB
//         _data[0] = r; _data[1] = g; _data[2] = b;
//         break;
//     }
//   }

//   //does no index check
//   uint32_t getPixelColor(uint16_t pix) 
//   {
//     if (!_valid) return 0;
//     return RGBW32(_data[0], _data[1], _data[2], _data[3]);
//   }

//   void show() 
//   {
//     if (!_valid) return;
//     uint8_t numPins = NUM_PWM_PINS(_type);
//     for (uint8_t i = 0; i < numPins; i++) {
//       uint8_t scaled = (_data[i] * _bri) / 255;
//       if (reversed) scaled = 255 - scaled;
//       #ifdef ESP8266
//       analogWrite(_pins[i], scaled);
//       #else
//       ledcWrite(_ledcStart + i, scaled);
//       #endif
//     }
//   }

//   uint8_t getPins(uint8_t* pinArray) 
//   {
//     if (!_valid) return 0;
//     uint8_t numPins = NUM_PWM_PINS(_type);
//     for (uint8_t i = 0; i < numPins; i++) {
//       pinArray[i] = _pins[i];
//     }
//     return numPins;
//   }

//   void cleanup() 
//   {
//     deallocatePins();
//   }

//   ~BusPwm() 
//   {
//     cleanup();
//   }

//   private: 
//   uint8_t _pins[5] = {255, 255, 255, 255, 255};
//   uint8_t _data[5] = {0};
//   #ifdef ARDUINO_ARCH_ESP32
//   uint8_t _ledcStart = 255;
//   #endif

//   void deallocatePins() 
//   {
//     uint8_t numPins = NUM_PWM_PINS(_type);
//     for (uint8_t i = 0; i < numPins; i++) {
//       // pinManager.deallocatePin(_pins[i], PinOwner::BusPwm);
//       // if (!pinManager.isPinOk(_pins[i])) continue;
//       #ifdef ESP8266
//       digitalWrite(_pins[i], LOW); //turn off PWM interrupt
//       #else
//       if (_ledcStart < 16) ledcDetachPin(_pins[i]);
//       #endif
//     }
//     // #ifdef ARDUINO_ARCH_ESP32
//     // pinManager.deallocateLedc(_ledcStart, numPins);
//     // #endif
//   }

// };


// class BusOnOff : public Bus {
//   public:
//   BusOnOff(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) {
//     _valid = false;
//     if (bc.type != TYPE_ONOFF) return;

//     uint8_t currentPin = bc.pins[0];
//     // if (!pinManager.allocatePin(currentPin, true, PinOwner::BusOnOff)) {
//     //   return;
//     // }
//     _pin = currentPin; //store only after allocatePin() succeeds
//     pinMode(_pin, OUTPUT);
//     reversed = bc.reversed;
//     _valid = true;
//   };

//   void setPixelColor(uint16_t pix, uint32_t c) {
//     if (pix != 0 || !_valid) return; //only react to first pixel
// 		c = autoWhiteCalc(c);
//     uint8_t r = R(c);
//     uint8_t g = G(c);
//     uint8_t b = B(c);
//     uint8_t w = W(c);

//     _data = bool((r+g+b+w) && _bri) ? 0xFF : 0;
//   }

//   uint32_t getPixelColor(uint16_t pix) {
//     if (!_valid) return 0;
//     return RGBW32(_data, _data, _data, _data);
//   }

//   void show() {
//     if (!_valid) return;
//     digitalWrite(_pin, reversed ? !(bool)_data : (bool)_data);
//   }

//   uint8_t getPins(uint8_t* pinArray) {
//     if (!_valid) return 0;
//     pinArray[0] = _pin;
//     return 1;
//   }

//   void cleanup() {
//     // pinManager.deallocatePin(_pin, PinOwner::BusOnOff);
//   }

//   ~BusOnOff() {
//     cleanup();
//   }

//   private: 
//   uint8_t _pin = 255;
//   uint8_t _data = 0;
// };


// class BusNetwork : public Bus {
//   public:
//     BusNetwork(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) {
//       _valid = false;
// //      switch (bc.type) {
// //        case TYPE_NET_ARTNET_RGB:
// //          _rgbw = false;
// //          _UDPtype = 2;
// //          break;
// //        case TYPE_NET_E131_RGB:
// //          _rgbw = false;
// //          _UDPtype = 1;
// //          break;
// //        case TYPE_NET_DDP_RGB:
// //          _rgbw = false;
// //          _UDPtype = 0;
// //          break;
// //        default: // TYPE_NET_DDP_RGB / TYPE_NET_DDP_RGBW
//           _rgbw = bc.type == TYPE_NET_DDP_RGBW;
//           _UDPtype = 0;
// //          break;
// //      }
//       _UDPchannels = _rgbw ? 4 : 3;
//       _data = (byte *)malloc(bc.count * _UDPchannels);
//       if (_data == nullptr) return;
//       memset(_data, 0, bc.count * _UDPchannels);
//       _len = bc.count;
//       _client = IPAddress(bc.pins[0],bc.pins[1],bc.pins[2],bc.pins[3]);
//       _broadcastLock = false;
//       _valid = true;
//     };

//   bool hasRGB() { return true; }
//   bool hasWhite() { return _rgbw; }

//   void setPixelColor(uint16_t pix, uint32_t c) {
//     if (!_valid || pix >= _len) return;
// 		if (isRgbw()) c = autoWhiteCalc(c);
//     // if (_cct >= 1900) c = colorBalanceFromKelvin(_cct, c); //color correction from CCT
//     uint16_t offset = pix * _UDPchannels;
//     _data[offset]   = R(c);
//     _data[offset+1] = G(c);
//     _data[offset+2] = B(c);
//     if (_rgbw) _data[offset+3] = W(c);
//   }

//   uint32_t getPixelColor(uint16_t pix) {
//     if (!_valid || pix >= _len) return 0;
//     uint16_t offset = pix * _UDPchannels;
//     return RGBW32(_data[offset], _data[offset+1], _data[offset+2], _rgbw ? (_data[offset+3] << 24) : 0);
//   }

//   void show() {
//     // if (!_valid || !canShow()) return;
//     // _broadcastLock = true;
//     // realtimeBroadcast(_UDPtype, _client, _len, _data, _bri, _rgbw);
//     // _broadcastLock = false;
//   }

//   inline bool canShow() {
//     // this should be a return value from UDP routine if it is still sending data out
//     return !_broadcastLock;
//   }

//   uint8_t getPins(uint8_t* pinArray) {
//     for (uint8_t i = 0; i < 4; i++) {
//       pinArray[i] = _client[i];
//     }
//     return 4;
//   }

//   inline bool isRgbw() {
//     return _rgbw;
//   }

//   inline uint16_t getLength() {
//     return _len;
//   }

//   void cleanup() {
//     _type = I_NONE;
//     _valid = false;
//     if (_data != nullptr) free(_data);
//     _data = nullptr;
//   }

//   ~BusNetwork() {
//     cleanup();
//   }

//   private:
//     IPAddress _client;
//     uint8_t   _UDPtype;
//     uint8_t   _UDPchannels;
//     bool      _rgbw;
//     bool      _broadcastLock;
//     byte     *_data;
// };


// class BusManager {
//   public:
//   BusManager() {};

//   //utility to get the approx. memory usage of a given BusConfig
//   static uint32_t memUsage(BusConfig &bc) 
//   {

//     DEBUG_PRINTF("BusManager::memUsage bc.type %d\n\r", bc.type);

//     uint8_t type = bc.type;
//     uint16_t len = bc.count + bc.skipAmount;
//     if (type > 15 && type < 32) {
//       #ifdef ESP8266
//         if (bc.pins[0] == 3) { //8266 DMA uses 5x the mem
//           if (type > 29) return len*20; //RGBW
//           return len*15;
//         }
//         if (type > 29) return len*4; //RGBW
//         return len*3;
//       #else //ESP32 RMT uses double buffer?
//         if (type > 29) return len*8; //RGBW
//         return len*6;
//       #endif
//     }
//     if (type > 31 && type < 48)   return 5;
//     if (type == 44 || type == 45) return len*4; //RGBW
//     return len*3; //RGB
//   }
  
//   int add(BusConfig &bc) {
//       DEBUG_PRINTF("add  \n");
//     if (numBusses >= WLED_MAX_BUSSES) return -1;
//     if (bc.type >= TYPE_NET_DDP_RGB && bc.type < 96) {
//       DEBUG_PRINTF("IS_DIGITALa  \n");
//       busses[numBusses] = new BusNetwork(bc);
//     } else if (IS_DIGITAL(bc.type)) {
//       DEBUG_PRINTF("BusDigital:  bc.type %d, numBusses %d, colorOrderMap %d \n", bc.type, numBusses, colorOrderMap);
//       busses[numBusses] = new BusDigital(bc, numBusses, colorOrderMap);
//     } else if (bc.type == TYPE_ONOFF) {
//       DEBUG_PRINTF("IS_DIGITALc  \n");
//       busses[numBusses] = new BusOnOff(bc);
//     } else {
//       DEBUG_PRINTF("IS_DIGITALd  \n");
//       busses[numBusses] = new BusPwm(bc);
//     }
//     return numBusses++;
//   }

//   //do not call this method from system context (network callback)
//   void removeAll() {
//     DEBUG_PRINTLN(F("Removing all."));
//     //prevents crashes due to deleting busses while in use. 
//     while (!canAllShow()) yield();
//     for (uint8_t i = 0; i < numBusses; i++) delete busses[i];
//     numBusses = 0;
//   }

//   void show() 
//   {
    
//     DEBUG_PRINTF("BusManager::show\n\r");

//     for (uint8_t i = 0; i < numBusses; i++) 
//     {
//       busses[i]->setPixelColor(0, RGBW32(255,0,0,0));
//       busses[i]->show();
//     }
//   }

// 	void setStatusPixel(uint32_t c) 
//   {
    
//     DEBUG_PRINTF("BusManager::setStatusPixel %d\n\r", c);

//     for (uint8_t i = 0; i < numBusses; i++) {
// 			busses[i]->setStatusPixel(c);
// 		}
// 	}

//   void IRAM_ATTR setPixelColor(uint16_t pix, uint32_t c, int16_t cct=-1) 
//   {
    
//     DEBUG_PRINTF("BusManager::setPixelColor pix %d, c %d, cct %d\n\r", pix, c, cct);

//     for (uint8_t i = 0; i < numBusses; i++) {
//       Bus* b = busses[i];
//       uint16_t bstart = b->getStart();
//       if (pix < bstart || pix >= bstart + b->getLength()) continue;
//       busses[i]->setPixelColor(pix - bstart, c);
//     }
//   }

//   void setBrightness(uint8_t b) 
//   {

//     DEBUG_PRINTF("BusManager::setBrightness b %d\n\r", b);

//     for (uint8_t i = 0; i < numBusses; i++) {
//       busses[i]->setBrightness(b);
//     }
//   }

//   void setSegmentCCT(int16_t cct, bool allowWBCorrection = false) {
//     if (cct > 255) cct = 255;
//     if (cct >= 0) {
//       //if white balance correction allowed, save as kelvin value instead of 0-255
//       if (allowWBCorrection) cct = 1900 + (cct << 5);
//     } else cct = -1;
//     Bus::setCCT(cct);
//   }

//   uint32_t getPixelColor(uint16_t pix) {
//     for (uint8_t i = 0; i < numBusses; i++) {
//       Bus* b = busses[i];
//       uint16_t bstart = b->getStart();
//       if (pix < bstart || pix >= bstart + b->getLength()) continue;
//       return b->getPixelColor(pix - bstart);
//     }
//     return 0;
//   }

//   bool canAllShow() {
//     for (uint8_t i = 0; i < numBusses; i++) {
//       if (!busses[i]->canShow()) return false;
//     }
//     return true;
//   }

//   Bus* getBus(uint8_t busNr) 
//   {
    
//     DEBUG_PRINTF("BusManager::getBus busNr %d\n\r", busNr);

//     if (busNr >= numBusses) return nullptr;
//     return busses[busNr];
//   }

//   inline uint8_t getNumBusses() {
//     return numBusses;
//   }

//   //semi-duplicate of strip.getLengthTotal() (though that just returns strip._length, calculated in finalizeInit())
//   uint16_t getTotalLength() {
//     uint16_t len = 0;
//     for (uint8_t i=0; i<numBusses; i++) len += busses[i]->getLength();
//     return len;
//   }

//   void updateColorOrderMap(const ColorOrderMap &com) {
//     memcpy(&colorOrderMap, &com, sizeof(ColorOrderMap));
//   }

//   const ColorOrderMap& getColorOrderMap() const {
//     return colorOrderMap;
//   }

//   private:
//   uint8_t numBusses = 0;
//   Bus* busses[WLED_MAX_BUSSES];
//   ColorOrderMap colorOrderMap;
// };

#endif // ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT

#endif // guard