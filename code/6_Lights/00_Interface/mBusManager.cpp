
#include <Arduino.h>
#include <IPAddress.h>
#include "const.h"
#include "mBusNeoWrapper.h"
#include "mBusManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE


#ifdef ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED
uint16_t PolyBus::reduced_physical_render_length = ENABLE_FEATURE_LIGHTING__REDUCED_PHYSICAL_OUTPUT_PIXELS_RENDERED_LENGTH;//50;
uint16_t PolyBus::total_virtual_length = 0;
#endif


/***************************************************************************************************************************************************************** 
 ** ColorOrderMap *************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

static ColorOrderMap _colorOrderMap = {};


bool ColorOrderMap::add(uint16_t start, uint16_t len, uint8_t colorOrder) {
  if (count() >= WLED_MAX_COLOR_ORDER_MAPPINGS || len == 0 || (colorOrder & 0x0F) > COL_ORDER_MAX) return false; // upper nibble contains W swap information
  _mappings.push_back({start,len,colorOrder});
  return true;
}

uint8_t IRAM_ATTR ColorOrderMap::getPixelColorOrder(uint16_t pix, uint8_t defaultColorOrder) const {
  // upper nibble contains W swap information
  // when ColorOrderMap's upper nibble contains value >0 then swap information is used from it, otherwise global swap is used
  for (unsigned i = 0; i < count(); i++) {
    if (pix >= _mappings[i].start && pix < (_mappings[i].start + _mappings[i].len)) {
      return _mappings[i].colorOrder | ((_mappings[i].colorOrder >> 4) ? 0 : (defaultColorOrder & 0xF0));
    }
  }
  return defaultColorOrder;
}

ColorOrderMap& BusManager::getColorOrderMap() { return _colorOrderMap; }

/***************************************************************************************************************************************************************** 
 ** Bus: Parent class of BusDigital, BusPwm, and BusNetwork *************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

const char* Bus::getTypeName()
{
  return getTypeName(getType());
}

const char* Bus::getTypeName(uint8_t id)
{
  switch(getType()){
    default:
    //Digital types (data pin only) (16-31)
    case BUSTYPE_WS2812_1CH:        return PM_BUSTYPE__WS2812_1CH__CTR;
    case BUSTYPE_WS2812_1CH_X3:     return PM_BUSTYPE__WS2812_1CH_X3__CTR;
    case BUSTYPE_WS2812_2CH_X3:     return PM_BUSTYPE__WS2812_2CH_X3__CTR;
    case BUSTYPE_WS2812_WWA:        return PM_BUSTYPE__WS2812_WWA__CTR;
    case BUSTYPE_WS2812_RGB:        return PM_BUSTYPE__WS2812_RGB__CTR;
    case BUSTYPE_GS8608:            return PM_BUSTYPE__GS8608__CTR;
    case BUSTYPE_WS2811_400KHZ:     return PM_BUSTYPE__WS2811_400KHZ__CTR;
    case BUSTYPE_TM1829:            return PM_BUSTYPE__TM1829__CTR;
    case BUSTYPE_SK6812_RGBW:       return PM_BUSTYPE__SK6812_RGBW__CTR;
    case BUSTYPE_WS2805_RGBWW:      return PM_BUSTYPE__WS2805_RGBWW__CTR;
    case BUSTYPE_TM1814:            return PM_BUSTYPE__TM1814__CTR;
    //"Analog" types (PWM) (32-47)
    case BUSTYPE_ONOFF:             return PM_BUSTYPE__ONOFF__CTR;
    case BUSTYPE_ANALOG_1CH:        return PM_BUSTYPE__ANALOG_1CH__CTR;
    case BUSTYPE_ANALOG_2CH:        return PM_BUSTYPE__ANALOG_2CH__CTR;
    case BUSTYPE_ANALOG_3CH:        return PM_BUSTYPE__ANALOG_3CH__CTR;
    case BUSTYPE_ANALOG_4CH:        return PM_BUSTYPE__ANALOG_4CH__CTR;
    case BUSTYPE_ANALOG_5CH:        return PM_BUSTYPE__ANALOG_5CH__CTR;
    //Digital types (data + clock / SPI) (48-63)
    case BUSTYPE_WS2801:            return PM_BUSTYPE__WS2801__CTR;
    case BUSTYPE_APA102:            return PM_BUSTYPE__APA102__CTR;
    case BUSTYPE_LPD8806:           return PM_BUSTYPE__LPD8806__CTR;
    case BUSTYPE_P9813:             return PM_BUSTYPE__P9813__CTR;
    case BUSTYPE_LPD6803:           return PM_BUSTYPE__LPD6803__CTR;
    //Network types (master broadcast) (80-95)
    case BUSTYPE_NET_DDP_RGB:       return PM_BUSTYPE__NET_DDP_RGB__CTR;
    case BUSTYPE_NET_E131_RGB:      return PM_BUSTYPE__NET_E131_RGB__CTR;
    case BUSTYPE_NET_ARTNET_RGB:    return PM_BUSTYPE__NET_ARTNET_RGB__CTR;
    case BUSTYPE_NET_DDP_RGBW:      return PM_BUSTYPE__NET_DDP_RGBW__CTR;
    case BUSTYPE_RESERVED:          return PM_BUSTYPE__RESERVED__CTR;
  }
}

uint8_t Bus::getTypeIDbyName(const char* c)
{

  // Digital types (data pin only) (16-31)
  if     (strcmp_P(c,PM_BUSTYPE__WS2812_1CH__CTR)==0){ return BUSTYPE_WS2812_1CH; }
  else if(strcmp_P(c,PM_BUSTYPE__WS2812_1CH_X3__CTR)==0){ return BUSTYPE_WS2812_1CH_X3; }
  else if(strcmp_P(c,PM_BUSTYPE__WS2812_2CH_X3__CTR)==0){ return BUSTYPE_WS2812_2CH_X3; }
  else if(strcmp_P(c,PM_BUSTYPE__WS2812_WWA__CTR)==0){ return BUSTYPE_WS2812_WWA; }
  else if(strcmp_P(c,PM_BUSTYPE__WS2812_RGB__CTR)==0){ return BUSTYPE_WS2812_RGB; }
  else if(strcmp_P(c,PM_BUSTYPE__GS8608__CTR)==0){ return BUSTYPE_GS8608; }
  else if(strcmp_P(c,PM_BUSTYPE__WS2811_400KHZ__CTR)==0){ return BUSTYPE_WS2811_400KHZ; }
  else if(strcmp_P(c,PM_BUSTYPE__TM1829__CTR)==0){ return BUSTYPE_TM1829; }
  else if(strcmp_P(c,PM_BUSTYPE__SK6812_RGBW__CTR)==0){ return BUSTYPE_SK6812_RGBW; }
  else if(strcmp_P(c,PM_BUSTYPE__WS2805_RGBWW__CTR)==0){ return BUSTYPE_WS2805_RGBWW; }
  else if(strcmp_P(c,PM_BUSTYPE__TM1814__CTR)==0){ return BUSTYPE_TM1814; }
  // "Analog" types (PWM) (32-47)
  else if(strcmp_P(c,PM_BUSTYPE__ONOFF__CTR)==0){ return BUSTYPE_ONOFF; }
  else if(strcmp_P(c,PM_BUSTYPE__ANALOG_1CH__CTR)==0){ return BUSTYPE_ANALOG_1CH; }
  else if(strcmp_P(c,PM_BUSTYPE__ANALOG_2CH__CTR)==0){ return BUSTYPE_ANALOG_2CH; }
  else if(strcmp_P(c,PM_BUSTYPE__ANALOG_3CH__CTR)==0){ return BUSTYPE_ANALOG_3CH; }
  else if(strcmp_P(c,PM_BUSTYPE__ANALOG_4CH__CTR)==0){ return BUSTYPE_ANALOG_4CH; }
  else if(strcmp_P(c,PM_BUSTYPE__ANALOG_5CH__CTR)==0){ return BUSTYPE_ANALOG_5CH; }
  // Digital types (data + clock / SPI) (48-63)
  else if(strcmp_P(c,PM_BUSTYPE__WS2801__CTR)==0){ return BUSTYPE_WS2801; }
  else if(strcmp_P(c,PM_BUSTYPE__APA102__CTR)==0){ return BUSTYPE_APA102; }
  else if(strcmp_P(c,PM_BUSTYPE__LPD8806__CTR)==0){ return BUSTYPE_LPD8806; }
  else if(strcmp_P(c,PM_BUSTYPE__P9813__CTR)==0){ return BUSTYPE_P9813; }
  else if(strcmp_P(c,PM_BUSTYPE__LPD6803__CTR)==0){ return BUSTYPE_LPD6803; }
  // Network types (master broadcast) (80-95)
  else if(strcmp_P(c,PM_BUSTYPE__NET_DDP_RGB__CTR)==0){ return BUSTYPE_NET_DDP_RGB; }
  else if(strcmp_P(c,PM_BUSTYPE__NET_E131_RGB__CTR)==0){ return BUSTYPE_NET_E131_RGB; }
  else if(strcmp_P(c,PM_BUSTYPE__NET_ARTNET_RGB__CTR)==0){ return BUSTYPE_NET_ARTNET_RGB; }
  else if(strcmp_P(c,PM_BUSTYPE__NET_DDP_RGBW__CTR)==0){ return BUSTYPE_NET_DDP_RGBW; }
  else if(strcmp_P(c,PM_BUSTYPE__RESERVED__CTR)==0){ return BUSTYPE_RESERVED; }
  
  return BUSTYPE_NONE;
  
}



void Bus::calculateCCT(uint32_t c, uint8_t &ww, uint8_t &cw) {
  unsigned cct = 0; //0 - full warm white, 255 - full cold white
  unsigned w = W(c);

  if (_cct > -1) {                                    // using RGB?
    if (_cct >= 1900)    cct = (_cct - 1900) >> 5;    // convert K in relative format
    else if (_cct < 256) cct = _cct;                  // already relative
  } else {
    cct = (tkr_anim->approximateKelvinFromRGB(c) - 1900) >> 5;  // convert K (from RGB value) to relative format
  }

  // CCT blending modes (_cctBlend):
  // blend<0: ww: ▓▓▒░__  | blend=0: ww: ▓▒▒░░ |  blend>0 ww: ▓▓▓▒░
  //          cw: __░▒▓▓  |          cw: ░░▒▒▓ |          cw: ░▒▓▓▓
  int32_t ww_val, cw_val;
  if (_cctBlend < 0) {
    uint16_t range = 255 - 2 * (uint16_t)(-_cctBlend);
    if (range > 255) range = 255; // prevent overflow
    ww_val = range ? ((int32_t)(255 + _cctBlend - cct) * 255) / range : (cct < 128 ? 255 : 0); // exclusive blending
    cw_val = 255 - ww_val;
  } else {
    ww_val = _cctBlend ? ((int32_t)(255 - cct) * 255) / (255 - _cctBlend) : 255 - cct; // additive blending
    cw_val = _cctBlend ? ((int32_t) cct      * 255) / (255 - _cctBlend) : cct;
  }
  ww = (uint8_t)(ww_val < 0 ? 0 : ww_val > 255 ? 255 : ww_val);
  cw = (uint8_t)(cw_val < 0 ? 0 : cw_val > 255 ? 255 : cw_val);

  ww = (w * ww) / 255; //brightness scaling
  cw = (w * cw) / 255;
}

// calculates white channel and CCT values based on given settings
uint32_t Bus::autoWhiteCalc(uint32_t c, uint8_t &ww, uint8_t &cw) const {
  unsigned aWM = _autoWhiteMode;
  if (_gAWM < AW_GLOBAL_DISABLED) aWM = _gAWM;
  CRGBW cIn = c; // save original color for CCT calculation
  unsigned w = W(c);
  if (aWM != RGBW_MODE_MANUAL_ONLY) {
    unsigned r = R(c); // note: using uint8_t generates larger code
    unsigned g = G(c);
    unsigned b = B(c);
    if (aWM == RGBW_MODE_DUAL && w > 0) {
      //ignore auto-white calculation if w>0 and mode DUAL (DUAL behaves as BRIGHTER if w==0)
    } else if (aWM == RGBW_MODE_MAX) {
      w = r > g ? (r > b ? r : b) : (g > b ? g : b); // brightest RGB channel
    } else {
      w = r < g ? (r < b ? r : b) : (g < b ? g : b); // darkest RGB channel
      if (aWM == RGBW_MODE_AUTO_ACCURATE) { r -= w; g -= w; b -= w; } //subtract w in ACCURATE mode
    }
    c = RGBW32(r, g, b, w);
  }
  if (_hasCCT) {
    cIn.w = w; // need original rgb values in case CCT is derived from RGB
    calculateCCT(cIn, ww, cw);
  }
  return c;
}


uint8_t *Bus::allocateData(size_t size) {
  if (_data) free(_data); // should not happen, but for safety
  return _data = (uint8_t *)(size>0 ? calloc(size, sizeof(uint8_t)) : nullptr);
}


/***************************************************************************************************************************************************************** 
 ** BusDigital *************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/


// BusDigital::BusDigital(const BusConfig &bc)//, uint8_t nr, const ColorOrderMap &com)
// : Bus(bc.type, bc.start, bc.autoWhite, bc.length, bc.reversed, (bc.refreshReq))
// , _skip(bc.skipAmount) //sacrificial pixels
// , _colorOrder(bc.colorOrder)
// , _milliAmpsPerLed(bc.milliAmpsPerLed)
// , _milliAmpsMax(bc.milliAmpsMax)
// // , _colorOrderMap(com)
// {

//   if (!isDigital(bc.type) || !bc.length)
//   {    
//     ALOG_ERR(PSTR("BusDigital type%d or length%d"), bc.type, bc.length);
//     return;
//   }
//   _frequencykHz = 0U;
//   _pins[0] = bc.pins[0];
//   if (is2Pin(bc.type)) 
//   {
//     _pins[1] = bc.pins[1];
//     _frequencykHz = bc.frequency ? bc.frequency : 2000U; // 2MHz clock if undefined
//   }

//   _iType = PolyBus::getI(bc.type, _pins, nr);
//   if (_iType == BUSTYPE__NONE__ID)
//   {
//     Serial.println("BusDigital: No Bus");
//     return;
//   }

//   _hasRgb = hasRGB(bc.type);
//   _hasWhite = hasWhite(bc.type);
//   _hasCCT = hasCCT(bc.type);
//   /**
//    * Next line creates the buffer needed to store the pixels
//    * number of pixels * number of channels [3 (RGB) or 4 (RGBW) ] later I may want to add a check for RGBWW, and making the buffer twice as long for my transition effects 
//    */
//   if (bc.doubleBuffer && !allocateData(bc.length * Bus::getNumberOfChannels(bc.type))) return;
//   //_buffering = bc.doubleBuffer;
//   uint16_t lenToCreate = bc.length;
//   if (bc.type == BUSTYPE_WS2812_1CH_X3) lenToCreate = NUM_ICS_WS2812_1CH_3X(bc.length); // only needs a third of "RGB" LEDs for NeoPixelBus
//   _busPtr = PolyBus::create(_iType, _pins, lenToCreate + _skip, nr);
//   _valid = (_busPtr != nullptr);
//   ALOG_INF(PSTR("%successfully inited strip %u (len %u) with type %u and pins %u,%u (itype %u). mA=%d/%d\n"), _valid?"S":"Uns", nr, bc.length, bc.type, _pins[0], is2Pin(bc.type)?_pins[1]:255, _iType, _milliAmpsPerLed, _milliAmpsMax);
// }



BusDigital::BusDigital(const BusConfig &bc)
: Bus(bc.type, bc.start, bc.autoWhite, bc.count, bc.reversed, (bc.refreshReq || bc.type == TYPE_TM1814))
, _skip(bc.skipAmount) //sacrificial pixels
, _colorOrder(bc.colorOrder)
, _milliAmpsPerLed(bc.milliAmpsPerLed)
, _milliAmpsMax(bc.milliAmpsMax)
, _driverType(bc.driverType) // Store driver preference (0=RMT, 1=I2S)
{
  ALOG_INF(PSTR("bc.type %d"), bc.type);
  DEBUGBUS_PRINTLN(F("Bus: Creating digital bus."));
  if (!isDigital(bc.type) || !bc.count) { DEBUGBUS_PRINTLN(F("Not digial or empty bus!")); return; }
  _iType = bc.iType; // reuse the iType that was determined by polyBus in getI() in finalizeInit()

  ALOG_INF(PSTR("type %d"), _iType);

  if (_iType == BUSTYPE__NONE__ID) { DEBUGBUS_PRINTLN(F("Incorrect iType!")); return; }

  // if (!PinManager::allocatePin(bc.pins[0], true, PinOwner::BusDigital)) { DEBUGBUS_PRINTLN(F("Pin 0 allocated!")); return; }
  _frequencykHz = 0U;
  _colorSum = 0;
  _pins[0] = bc.pins[0];
  if (is2Pin(bc.type)) {
    // if (!PinManager::allocatePin(bc.pins[1], true, PinOwner::BusDigital)) {
    //   cleanup();
    //   DEBUGBUS_PRINTLN(F("Pin 1 allocated!"));
    //   return;
    // }
    _pins[1] = bc.pins[1];
    _frequencykHz = bc.frequency ? bc.frequency : 2000U; // 2MHz clock if undefined
  }

  // uint8_t nr = 0;

  _hasRgb = hasRGB(bc.type);
  _hasWhite = hasWhite(bc.type);
  _hasCCT = hasCCT(bc.type);
  uint16_t lenToCreate = bc.count;
  if (bc.type == TYPE_WS2812_1CH_X3) lenToCreate = NUM_ICS_WS2812_1CH_3X(bc.count); // only needs a third of "RGB" LEDs for NeoPixelBus
  _busPtr = PolyBus::create(_iType, _pins, lenToCreate + _skip, bc.channel);
  _valid = (_busPtr != nullptr) && bc.count > 0;
  // fix for wled#4759
  if (_valid) for (unsigned i = 0; i < _skip; i++) {
    PolyBus::setPixelColor(_busPtr, _iType, i, 0, COL_ORDER_GRB); // set sacrificial pixels to black (CO does not matter here)
  }
  else {
    cleanup();
  }
  DEBUGBUS_PRINTF_P(PSTR("Bus len:%u, type:%u (RGB:%d, W:%d, CCT:%d), pins:%u,%u [itype:%u, channel:%u, driver:%s] mA=%d/%d %s\n"),
  (int)bc.count,
  (int)bc.type,
  (int)_hasRgb, (int)_hasWhite, (int)_hasCCT,
  (unsigned)_pins[0], is2Pin(bc.type)?(unsigned)_pins[1]:255U,
  (unsigned)_iType,
  (unsigned)bc.channel,
  isI2S() ? "I2S" : "RMT",
  (int)_milliAmpsPerLed, (int)_milliAmpsMax,
  _valid ? " " : "FAILED"
);
}


// To disable brightness limiter we either set output max current to 0 or single LED current to 0
uint8_t BusDigital::estimateCurrentAndLimitBri() {
  bool useWackyWS2815PowerModel = false;
  byte actualMilliampsPerLed = _milliAmpsPerLed;

  // return 255; // foorce force, ignore check

  if (_milliAmpsMax < MA_FOR_ESP/BusManager::getNumBusses() || actualMilliampsPerLed == 0) { //0 mA per LED and too low numbers turn off calculation
    return _bri;
  }

  if (_milliAmpsPerLed == 255) {
    useWackyWS2815PowerModel = true;
    actualMilliampsPerLed = 12; // from testing an actual strip
  }

  size_t powerBudget = (_milliAmpsMax - MA_FOR_ESP/BusManager::getNumBusses()); //80/120mA for ESP power
  if (powerBudget > getLength()) { //each LED uses about 1mA in standby, exclude that from power budget
    powerBudget -= getLength();
  } else {
    powerBudget = 0;
  }

  uint32_t busPowerSum = 0;
  for (unsigned i = 0; i < getLength(); i++) {  //sum up the usage of each LED
    uint32_t c = getPixelColor(i); // always returns original or restored color without brightness scaling
    byte r = R(c), g = G(c), b = B(c), w = W(c);

    if (useWackyWS2815PowerModel) { //ignore white component on WS2815 power calculation
      busPowerSum += (max(max(r,g),b)) * 3;
    } else {
      busPowerSum += (r + g + b + w);
    }
  }

  if (hasWhite()) { //RGBW led total output with white LEDs enabled is still 50mA, so each channel uses less
    busPowerSum *= 3;
    busPowerSum >>= 2; //same as /= 4
  }

  // powerSum has all the values of channels summed (max would be getLength()*765 as white is excluded) so convert to milliAmps
  busPowerSum = (busPowerSum * actualMilliampsPerLed) / 765;
  _milliAmpsTotal = busPowerSum * _bri / 255;

  uint8_t newBri = _bri;
  if (busPowerSum * _bri / 255 > powerBudget) { //scale brightness down to stay in current limit
    float scale = (float)(powerBudget * 255) / (float)(busPowerSum * _bri);
    if (scale >= 1.0f) return _bri;
    _milliAmpsTotal = ceilf((float)_milliAmpsTotal * scale);
    uint8_t scaleB = min((int)(scale * 255), 255);
    newBri = unsigned(_bri * scaleB) / 256 + 1;
  }
  return newBri;
}


// void BusDigital::show() {

//   // #ifdef ENABLE_DEVFEATURE_LIGHTING__SHOW_FALLBACK_MINIMAL_2024

//   PolyBus::show(_busPtr, _iType);

// //   #else
  
// //   _milliAmpsTotal = 0;
// //   if (!_valid) return;
  
// //       DEBUG_LINE_HERE

// //   uint8_t cctWW = 0, cctCW = 0;
// //   unsigned newBri = estimateCurrentAndLimitBri();  // will fill _milliAmpsTotal
// //   if (newBri < _bri) PolyBus::setBrightness(_busPtr, _iType, newBri); // limit brightness to stay within current limits

// //   /**
// //    * @brief Method: Buffer
// //    * A internal buffer is used to store the pixel data before it is sent to the bus.
// //    * This uses more memory, but enables transitions.
// //    * 
// //    */
// //   if (_data) 
// //   {
// //     ALOG_INF(PSTR("buffer method active %d %d %d\t %d %d %d"), _len, getNumberOfChannels(), _colorOrder, _data[0], _data[1], _data[2]);
// //     size_t channels = getNumberOfChannels();
// //     int16_t oldCCT = Bus::_cct; // temporarily save bus CCT
// //     for (size_t i=0; i<_len; i++) {
// //       size_t offset = i * channels;
// //       unsigned co = _colorOrderMap.getPixelColorOrder(i+_start, _colorOrder);
// //       uint32_t c;
// //       if (_type == BUSTYPE_WS2812_1CH_X3) { // map to correct IC, each controls 3 LEDs (_len is always a multiple of 3)
// //         switch (i%3) {
// //           case 0: c = RGBW32(_data[offset]  , _data[offset+1], _data[offset+2], 0); break;
// //           case 1: c = RGBW32(_data[offset-1], _data[offset]  , _data[offset+1], 0); break;
// //           case 2: c = RGBW32(_data[offset-2], _data[offset-1], _data[offset]  , 0); break;
// //         }
// //       } else {
// //         if (hasRGB()) c = RGBW32(_data[offset], _data[offset+1], _data[offset+2], hasWhite() ? _data[offset+3] : 0);
// //         else          c = RGBW32(0, 0, 0, _data[offset]);
// //       }
// //       if (hasCCT()) {
// //         // unfortunately as a segment may span multiple buses or a bus may contain multiple segments and each segment may have different CCT
// //         // we need to extract and appy CCT value for each pixel individually even though all buses share the same _cct variable
// //         // TODO: there is an issue if CCT is calculated from RGB value (_cct==-1), we cannot do that with double buffer
// //         Bus::_cct = _data[offset+channels-1];
// //         Bus::calculateCCT(c, cctWW, cctCW);
// //       }
// //       unsigned pix = i;
// //       if (_reversed) pix = _len - pix -1;
// //       pix += _skip;
// //       Serial.printf("Setting pixel %d to %d %d %d %d\n", pix, c, co, (cctCW<<8) | cctWW);
// //       PolyBus::setPixelColor(_busPtr, _iType, pix, c, co, (cctCW<<8) | cctWW);
// //     }
// //     #if !defined(STATUSLED) || STATUSLED>=0
// //     if (_skip) PolyBus::setPixelColor(_busPtr, _iType, 0, 0, _colorOrderMap.getPixelColorOrder(_start, _colorOrder)); // paint skipped pixels black
// //     #endif
// //     for (int i=1; i<_skip; i++) PolyBus::setPixelColor(_busPtr, _iType, i, 0, _colorOrderMap.getPixelColorOrder(_start, _colorOrder)); // paint skipped pixels black
// //     Bus::_cct = oldCCT;
// //   }
// //   /**
// //    * @brief Method: Direct
// //    * Pixels are set directly on the bus, no buffer is used.
// //    * This section sole purpose is to fix colour inaccuracies that may occur when using the buffer method.
// //    * It reads what has already been set on the bus, but not yet transmitted. 
// //    */
// //   else 
// //   {
// //     #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG
// //     ALOG_INF(PSTR("direct method active %d %d %d"), _len, getNumberOfChannels(), _colorOrder);
// //     #endif
// //     if (newBri < _bri) {
// //       Serial.println("I dont want this");
// //       unsigned hwLen = _len;
// //       if (_type == BUSTYPE_WS2812_1CH_X3) hwLen = NUM_ICS_WS2812_1CH_3X(_len); // only needs a third of "RGB" LEDs for NeoPixelBus
// //       for (unsigned i = 0; i < hwLen; i++) {
// //         // use 0 as color order, actual order does not matter here as we just update the channel values as-is
// //         #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
// //         uint32_t c = PolyBus::getPixelColor(_busPtr, _iType, i, 0); // tmp fix for RGBWW
// //         ALOG_INF(PSTR("direct method active lossy %d %d %d\t %d %d %d"), i, c.R, c.G, c.B, c.WW, c.CW);
// //         #else
// //         uint32_t c = restoreColorLossy(PolyBus::getPixelColor(_busPtr, _iType, i, 0), _bri);
// //         #endif
// //         #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
// //         // if (hasCCT()) Bus::calculateCCT(c, cctWW, cctCW); // this will unfortunately corrupt (segment) CCT data on every bus
// //         PolyBus::setPixelColor(_busPtr, _iType, i, c, 0);//, 0, (cctCW<<8) | cctWW); // repaint all pixels with new brightness
// //         #else
// //         if (hasCCT()) Bus::calculateCCT(c, cctWW, cctCW); // this will unfortunately corrupt (segment) CCT data on every bus
// //         PolyBus::setPixelColor(_busPtr, _iType, i, c, 0, (cctCW<<8) | cctWW); // repaint all pixels with new brightness
// //         #endif
// //       }
// //     }
// //   }


// //   /**
// //    * @brief To account for effects that require direct control, the brightness of the bus should be overridden
// //    * 
// //    * OR, most likely, what I should be doing is instead writing the "full brightness" output, and hence, it should
// //    * actually be the "under palette" that is set to a lower brightness. 
// //    * 
// //    */

// //   // PolyBus::setPixelColor(_busPtr, _iType, 0, RgbColor(255, 0, 0), 0);



// //   PolyBus::show(_busPtr, _iType, !_data); // faster if buffer consistency is not important (use !_buffering this causes 20% FPS drop)
// //   // restore bus brightness to its original value
// //   // this is done right after show, so this is only OK if LED updates are completed before show() returns
// //   // or async show has a separate buffer (ESP32 RMT and I2S are ok)
// //   if (newBri < _bri) PolyBus::setBrightness(_busPtr, _iType, _bri);
  
// //   DEBUG_LINE_HERE
// // #endif

// }

void BusDigital::show() {
  if (!_valid) return;
  _NPBbri = (_NPBbri * _bri) / 255;      // total applied brightness for use in restoreColorLossy (see applyBriLimit())
  PolyBus::show(_busPtr, _iType, _skip); // faster if buffer consistency is not important (no skipped LEDs)
}

bool BusDigital::canShow() const
{
  if (!_valid) return true;
  // ALOG_INF(PSTR("BusDigital::canShow %d %d"), _busPtr != nullptr, _iType);
  return PolyBus::canShow(_busPtr, _iType);
}


//If LEDs are skipped, it is possible to use the first as a status LED.
//TODO only show if no new show due in the next 50ms
void BusDigital::setStatusPixel(uint32_t c) {
  if (_valid && _skip) {
    PolyBus::setPixelColor(_busPtr, _iType, 0, c, _colorOrderMap.getPixelColorOrder(_start, _colorOrder));
    if (canShow()) PolyBus::show(_busPtr, _iType);
  }
}



// note: using WLED_O2_ATTR makes this function ~7% faster at the expense of 600 bytes of flash
void IRAM_ATTR BusDigital::setPixelColor(unsigned pix, uint32_t c) {
  if (!_valid) return;
  if (Bus::_cct >= 1900) c = tkr_anim->colorBalanceFromKelvin(Bus::_cct, c); //color correction from CCT
  uint8_t cctWW = 0, cctCW = 0;
  uint16_t wwcw = 0;
  if (hasWhite()) c = autoWhiteCalc(c, cctWW, cctCW);
  c = color_fade(c, _bri, true); // apply brightness

  // if(pix==0) ALOG_INF(PSTR("_bri %d"),_bri);

  if (hasCCT()) {
    wwcw = ((cctCW + 1) * _bri) & 0xFF00; // apply brightness to CCT (store CW in upper byte)
    wwcw |= ((cctWW + 1) * _bri) >> 8;
    if (_type == TYPE_WS2812_WWA) c = RGBW32(wwcw, wwcw >> 8, 0, W(c)); // ww,cw, 0, w
  }

  if (BusManager::_useABL) {
    // if using ABL, sum all color channels to estimate current and limit brightness in show()
    uint8_t r = R(c), g = G(c), b = B(c);
    if (_milliAmpsPerLed < 255) { // normal ABL
      _colorSum += r + g + b + W(c);
    } else { // wacky WS2815 power model, ignore white channel, use max of RGB (issue #549)
      _colorSum += ((r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b));
    }
  }

  if (_reversed) pix = _len - pix -1;
  pix += _skip;
  const uint8_t co = _colorOrderMap.getPixelColorOrder(pix+_start, _colorOrder);
  if (_type == TYPE_WS2812_1CH_X3) { // map to correct IC, each controls 3 LEDs
    unsigned pOld = pix;
    pix = IC_INDEX_WS2812_1CH_3X(pix);
    uint32_t cOld = PolyBus::getPixelColor(_busPtr, _iType, pix, co);
    switch (pOld % 3) { // change only the single channel (TODO: this can cause loss because of get/set)
      case 0: c = RGBW32(R(cOld), W(c)   , B(cOld), 0); break;
      case 1: c = RGBW32(W(c)   , G(cOld), B(cOld), 0); break;
      case 2: c = RGBW32(R(cOld), G(cOld), W(c)   , 0); break;
    }
  }

  PolyBus::setPixelColor(_busPtr, _iType, pix, c, co, wwcw);

}

// returns lossly restored color from bus
uint32_t IRAM_ATTR BusDigital::getPixelColor(unsigned pix) const {
  if (!_valid) return 0;
  if (_reversed) pix = _len - pix -1;
  pix += _skip;
  const uint8_t co = _colorOrderMap.getPixelColorOrder(pix+_start, _colorOrder);
  uint32_t c = restoreColorLossy(PolyBus::getPixelColor(_busPtr, _iType, (_type==TYPE_WS2812_1CH_X3) ? IC_INDEX_WS2812_1CH_3X(pix) : pix, co),_NPBbri);
  if (_type == TYPE_WS2812_1CH_X3) { // map to correct IC, each controls 3 LEDs
    uint8_t r = R(c);
    uint8_t g = _reversed ? B(c) : G(c); // should G and B be switched if _reversed?
    uint8_t b = _reversed ? G(c) : B(c);
    switch (pix % 3) { // get only the single channel
      case 0: c = RGBW32(g, g, g, g); break;
      case 1: c = RGBW32(r, r, r, r); break;
      case 2: c = RGBW32(b, b, b, b); break;
    }
  }
  if (_type == TYPE_WS2812_WWA) {
    uint8_t w = R(c) | G(c);
    c = RGBW32(w, w, 0, w);
  }
  return c;
  
}


uint8_t BusDigital::getPins(uint8_t* pinArray) const
{
  uint8_t numPins = is2Pin(_type) ? 2 : 1;
  for (uint8_t i = 0; i < numPins; i++) 
  {
    pinArray[i] = _pins[i];
  }
  return numPins;
}


size_t BusDigital::getBusSize() const {
  return sizeof(BusDigital) + (isOk() ? PolyBus::getDataSize(_busPtr, _iType) : 0); // does not include common I2S DMA buffer
}

void BusDigital::setColorOrder(uint8_t colorOrder) 
{
  _colorOrder = colorOrder;
}


// credit @willmmiles & @netmindz https://github.com/Aircoookie/WLED/pull/4056
std::vector<LEDType> BusDigital::getLEDTypes() {
  return {
    {BUSTYPE_WS2812_RGB,    "D",  PSTR("WS281x")},
    {BUSTYPE_SK6812_RGBW,   "D",  PSTR("SK6812/WS2814 RGBW")},
    {BUSTYPE_TM1814,        "D",  PSTR("TM1814")},
    {BUSTYPE_WS2811_400KHZ, "D",  PSTR("400kHz")},
    {BUSTYPE_TM1829,        "D",  PSTR("TM1829")},
    {BUSTYPE_UCS8903,       "D",  PSTR("UCS8903")},
    {BUSTYPE_APA106,        "D",  PSTR("APA106/PL9823")},
    {BUSTYPE_TM1914,        "D",  PSTR("TM1914")},
    {BUSTYPE_FW1906,        "D",  PSTR("FW1906 GRBCW")},
    {BUSTYPE_UCS8904,       "D",  PSTR("UCS8904 RGBW")},
    {BUSTYPE_WS2805,        "D",  PSTR("WS2805 RGBCW")},
    {BUSTYPE_SM16825,       "D",  PSTR("SM16825 RGBCW")},
    {BUSTYPE_WS2812_1CH_X3, "D",  PSTR("WS2811 White")},
    //{BUSTYPE_WS2812_2CH_X3, "D",  PSTR("WS2811 CCT")}, // not implemented
    //{BUSTYPE_WS2812_WWA,    "D",  PSTR("WS2811 WWA")}, // not implemented
    {BUSTYPE_WS2801,        "2P", PSTR("WS2801")},
    {BUSTYPE_APA102,        "2P", PSTR("APA102")},
    {BUSTYPE_LPD8806,       "2P", PSTR("LPD8806")},
    {BUSTYPE_LPD6803,       "2P", PSTR("LPD6803")},
    {BUSTYPE_P9813,         "2P", PSTR("PP9813")},
  };
}


bool BusDigital::isI2S() {
  return (_iType & 0x01) == 0; // I2S types have even iType values
}

void BusDigital::begin() {
  PolyBus::begin(_busPtr, _iType, _pins);
}

void BusDigital::cleanup() 
{
  DEBUG_PRINTLN(F("Digital Cleanup."));
  PolyBus::cleanup(_busPtr, _iType);
  _iType = BUSTYPE__NONE__ID;
  _valid = false;
  _busPtr = nullptr;
  if (_data != nullptr) freeData();
}


//DISCLAIMER
//The following function attemps to calculate the current LED power usage,
//and will limit the brightness to stay below a set amperage threshold.
//It is NOT a measurement and NOT guaranteed to stay within the ablMilliampsMax margin.
//Stay safe with high amperage and have a reasonable safety margin!
//I am NOT to be held liable for burned down garages or houses!

// note on ABL implementation:
// ABL is set up in finalizeInit()
// scaled color channels are summed in BusDigital::setPixelColor()
// the used current is estimated and limited in BusManager::show()
// if limit is set too low, brightness is limited to 1 to at least show some light
// to disable brightness limiter for a bus, set LED current to 0

void BusDigital::estimateCurrent() {
  uint32_t actualMilliampsPerLed = _milliAmpsPerLed;
  if (_milliAmpsPerLed == 255) {
    // use wacky WS2815 power model, see WLED issue #549
    _colorSum *= 3; // sum is sum of max value for each color, need to multiply by three to account for clrUnitsPerChannel being 3*255
    actualMilliampsPerLed = 12; // from testing an actual strip
  }
  // _colorSum has all the values of color channels summed, max would be getLength()*(3*255 + (255 if hasWhite()): convert to milliAmps
  uint32_t clrUnitsPerChannel = hasWhite() ? 4*255 : 3*255;
  _milliAmpsTotal = ((uint64_t)_colorSum * actualMilliampsPerLed) / clrUnitsPerChannel + getLength(); // add 1mA standby current per LED to total (WS2812: ~0.7mA, WS2815: ~2mA)
}

void BusDigital::applyBriLimit(uint8_t newBri) {
  // a newBri of 0 means calculate per-bus brightness limit
  _NPBbri = 255; // reset, intermediate value is set below, final value is calculated in bus::show()
  if (newBri == 0) {
    if (_milliAmpsLimit == 0 || _milliAmpsTotal == 0) return; // ABL not used for this bus
    newBri = 255;

    if (_milliAmpsLimit > getLength()) { // each LED uses about 1mA in standby
      if (_milliAmpsTotal > _milliAmpsLimit) {
        // scale brightness down to stay in current limit
        newBri = ((uint32_t)_milliAmpsLimit * 255) / _milliAmpsTotal + 1; // +1 to avoid 0 brightness
        _milliAmpsTotal = _milliAmpsLimit;
      }
    } else {
      newBri = 1; // limit too low, set brightness to 1, this will dim down all colors to minimum since we use video scaling
      _milliAmpsTotal = getLength(); // estimate bus current as minimum
    }
  }

  if (newBri < 255) {
    _NPBbri = newBri; // store value so it can be updated in show() (must be updated even if ABL is not used)
    uint16_t wwcw = 0;
    unsigned hwLen = _len;
    if (_type == TYPE_WS2812_1CH_X3) hwLen = NUM_ICS_WS2812_1CH_3X(_len); // only needs a third of "RGB" LEDs for NeoPixelBus
    for (unsigned i = 0; i < hwLen; i++) {
      uint8_t co = _colorOrderMap.getPixelColorOrder(i+_start, _colorOrder); // need to revert color order for correct color scaling and CCT calc in case white is swapped
      uint32_t c = PolyBus::getPixelColor(_busPtr, _iType, i, co); // Note: if ABL would be calculated as a seperate loop (as it was before) it is slower but could use original color, making it more color-accurate
      if (hasCCT()) {
        uint8_t cctWW, cctCW;
        Bus::calculateCCT(c, cctWW, cctCW); // calculate CCT before fade (more accurate) | Note: if using "accurate" white calculation mode, approximateKelvinFromRGB can be very inaccurate (white is subtracted)
        wwcw = ((cctCW + 1) * newBri) & 0xFF00; // apply brightness to CCT (leave it in upper byte for 16bit NeoPixelBus value)
        wwcw |= ((cctWW + 1) * newBri) >> 8;
      }
      c = color_fade(c, newBri, true); // apply additional dimming  note: using inline version is a bit faster but overhead of getPixelColor() dominates the speed impact by far
      PolyBus::setPixelColor(_busPtr, _iType, i, c, co, wwcw); // repaint all pixels with new brightness
    }
  }

  _colorSum = 0; // reset for next frame
}

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusPwm *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

/* see https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/gpio.html
 * The ESP32-S3 chip features 45 physical GPIO pins (GPIO0 ~ GPIO21 and GPIO26 ~ GPIO48). Each pin can be used as a general-purpose I/O
 * Strapping pins: GPIO0, GPIO3, GPIO45 and GPIO46 are strapping pins. For more infomation, please refer to ESP32-S3 datasheet.
 * Serial TX = GPIO43, RX = GPIO44; LED BUILTIN is usually GPIO39
 * USB-JTAG: GPIO 19 and 20 are used by USB-JTAG by default. In order to use them as GPIOs, USB-JTAG will be disabled by the drivers.
 * SPI0/1: GPIO26-32 are usually used for SPI flash and PSRAM and not recommended for other uses.
 * When using Octal Flash or Octal PSRAM or both, GPIO33~37 are connected to SPIIO4 ~ SPIIO7 and SPIDQS. Therefore, on boards embedded with ESP32-S3R8 / ESP32-S3R8V chip, GPIO33~37 are also not recommended for other uses.
 *
 * see https://docs.espressif.com/projects/esp-idf/en/v4.4.2/esp32s3/api-reference/peripherals/adc.html
 *     https://docs.espressif.com/projects/esp-idf/en/latest/esp32s3/api-reference/peripherals/adc_oneshot.html
 * ADC1: GPIO1  - GPIO10 (channel 0..9)
 * ADC2: GPIO11 - GPIO20 (channel 0..9)
 * adc_power_acquire(): Please do not use the interrupt of GPIO36 and GPIO39 when using ADC or Wi-Fi and Bluetooth with sleep mode enabled. As a workaround, call adc_power_acquire() in the APP.
 * Since the ADC2 module is also used by the Wi-Fi, reading operation of adc2_get_raw() may fail between esp_wifi_start() and esp_wifi_stop(). Use the return code to see whether the reading is successful.
 */

#ifdef ARDUINO_ARCH_ESP32
#if defined(CONFIG_IDF_TARGET_ESP32C3)
  #define MAX_LED_CHANNELS 6
#else
  #if defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3)
    #define MAX_LED_CHANNELS 8
  #else
    #define MAX_LED_CHANNELS 16
  #endif
#endif
byte BusPwm::allocateLedc(byte channels)
{
  if (channels > MAX_LED_CHANNELS || channels == 0) return 255;
  byte ca = 0;
  for (byte i = 0; i < MAX_LED_CHANNELS; i++) {
    byte by = i >> 3;
    byte bi = i - 8*by;
    if (bitRead(ledcAlloc[by], bi)) { //found occupied pin
      ca = 0;
    } else {
      ca++;
    }
    if (ca >= channels) { //enough free channels
      byte in = (i + 1) - ca;
      for (byte j = 0; j < ca; j++) {
        byte b = in + j;
        byte by = b >> 3;
        byte bi = b - 8*by;
        bitWrite(ledcAlloc[by], bi, true);
      }
      return in;
    }
  }
  return 255; //not enough consecutive free LEDC channels
}

void BusPwm::deallocateLedc(byte pos, byte channels)
{
  for (byte j = pos; j < pos + channels; j++) {
    if (j > MAX_LED_CHANNELS) return;
    byte by = j >> 3;
    byte bi = j - 8*by;
    bitWrite(ledcAlloc[by], bi, false);
  }
}

#endif


#ifdef ESP8266
  // 1 MHz clock
  #define CLOCK_FREQUENCY 1000000UL
#else
  // Use XTAL clock if possible to avoid timer frequency error when setting APB clock < 80 Mhz
  // https://github.com/espressif/arduino-esp32/blob/2.0.2/cores/esp32/esp32-hal-ledc.c
  #ifdef SOC_LEDC_SUPPORT_XTAL_CLOCK
    #define CLOCK_FREQUENCY 40000000UL
  #else
    #define CLOCK_FREQUENCY 80000000UL
  #endif
#endif

#ifdef ESP8266
  #define MAX_BIT_WIDTH 10
#else
  #ifdef SOC_LEDC_TIMER_BIT_WIDE_NUM
    // C6/H2/P4: 20 bit, S2/S3/C2/C3: 14 bit
    #define MAX_BIT_WIDTH SOC_LEDC_TIMER_BIT_WIDE_NUM 
  #else
    // ESP32: 20 bit (but in reality we would never go beyond 16 bit as the frequency would be to low)
    #define MAX_BIT_WIDTH 14
  #endif
#endif


BusPwm::BusPwm(const BusConfig &bc)
: Bus(bc.type, bc.start, bc.autoWhite, 1, bc.reversed, bc.refreshReq) // hijack Off refresh flag to indicate usage of dithering
{
  if (!isPWM(bc.type)) return;
  unsigned numPins = numPWMPins(bc.type);
  [[maybe_unused]] const bool dithering = _needsRefresh;
  _frequency = bc.frequency ? bc.frequency : WLED_PWM_FREQ;
  // duty cycle resolution (_depth) can be extracted from this formula: CLOCK_FREQUENCY > _frequency * 2^_depth
  for (_depth = MAX_BIT_WIDTH; _depth > 8; _depth--) if (((CLOCK_FREQUENCY/_frequency) >> _depth) > 0) break;

  #ifdef ENABLE_DEVFEATURE_LIGHTING__BUSPWM_2025_METHOD

    // managed_pin_type pins[numPins];
    // for (unsigned i = 0; i < numPins; i++) pins[i] = {(int8_t)bc.pins[i], true};
    // if (!PinManager::allocateMultiplePins(pins, numPins, PinOwner::BusPwm)) return;

    // #ifdef ESP8266
    //   analogWriteRange((1<<_depth)-1);
    //   analogWriteFreq(_frequency);
    // #else
    //   // for 2 pin PWM CCT strip pinManager will make sure both LEDC channels are in the same speed group and sharing the same timer
    //   _ledcStart = PinManager::allocateLedc(numPins);
    //   if (_ledcStart == 255) { //no more free LEDC channels
    //     PinManager::deallocateMultiplePins(pins, numPins, PinOwner::BusPwm);
    //     return;
    //   }
    //   // if _needsRefresh is true (UI hack) we are using dithering (credit @dedehai & @zalatnaicsongor)
    //   if (dithering) _depth = 12; // fixed 8 bit depth PWM with 4 bit dithering (ESP8266 has no hardware to support dithering)
    // #endif

  #else

    #ifdef ESP8266
    analogWriteRange(255);  //same range as one RGB channel
    analogWriteFreq(WLED_PWM_FREQ);
    #else
    _ledcStart = allocateLedc(numPins);
    if (_ledcStart == 255) { //no more free LEDC channels
      deallocatePins(); return;
    }

    
    // if _needsRefresh is true (UI hack) we are using dithering (credit @dedehai & @zalatnaicsongor)
    if (dithering) _depth = 12; // fixed 8 bit depth PWM with 4 bit dithering (ESP8266 has no hardware to support dithering)

    #endif

    for (uint8_t i = 0; i < numPins; i++) 
    { 
      uint8_t currentPin = bc.pins[i];
      _pins[i] = currentPin;
      ALOG_INF(PSTR("_pins[%d]=>%d"),i,_pins[i]);
      #ifdef ESP8266
      pinMode(_pins[i], OUTPUT);
      #else
      ledcSetup(_ledcStart + i, WLED_PWM_FREQ, 10); // Hi, the maximum frequency is 80000000 / 2resolution. At 1-bit resolution => 40MHz At 8-bits resolution => 312,5 kHz // 80MHz / 1024 = 78125 Hz
      ledcAttachPin(_pins[i], _ledcStart + i);
      #endif
    }


    _hasRgb = hasRGB(bc.type);
    _hasWhite = hasWhite(bc.type);
    _hasCCT = hasCCT(bc.type);
    _data = _pwmdata; // avoid malloc() and use stack
    _valid = true;
    DEBUG_PRINTF_P(PSTR("%successfully inited PWM strip with type %u, frequency %u, bit depth %u and pins %u,%u,%u,%u,%u\n"), _valid?"S":"Uns", bc.type, _frequency, _depth, _pins[0], _pins[1], _pins[2], _pins[3], _pins[4]);

  #endif // ENABLE_DEVFEATURE_LIGHTING__BUSPWM_DITHERING_PWM_ENABLED

}


void BusPwm::setPixelColor(unsigned pix, uint32_t c) {
  if (pix != 0 || !_valid) return; //only react to first pixel
  if (Bus::_cct >= 1900 && (_type == TYPE_ANALOG_3CH || _type == TYPE_ANALOG_4CH)) {
    c = tkr_anim->colorBalanceFromKelvin(Bus::_cct, c); //color correction from CCT
  }
  uint8_t cctWW, cctCW;
  if (_type != TYPE_ANALOG_3CH) c = autoWhiteCalc(c, cctWW, cctCW);
  uint8_t r = R(c), g = G(c), b = B(c), w = W(c);
  // note: no color scaling, brightness is applied in show()

  switch (_type) {
    case TYPE_ANALOG_1CH: //one channel (white), relies on auto white calculation
      _data[0] = w;
      break;
    case TYPE_ANALOG_2CH: //warm white + cold white
      if (tkr_anim->cctICused) {
        _data[0] = w;
        _data[1] = Bus::_cct < 0 || Bus::_cct > 255 ? 127 : Bus::_cct;
      } else {
        _data[0] = cctWW;
        _data[1] = cctCW;
      }
      break;
    case TYPE_ANALOG_5CH: //RGB + warm white + cold white
      if (tkr_anim->cctICused)
        _data[4] = Bus::_cct < 0 || Bus::_cct > 255 ? 127 : Bus::_cct;
      else {
        w = cctWW;
        _data[4] = cctCW;
      }
      // fall through to set RGBW channels
    case TYPE_ANALOG_4CH: //RGBW
      _data[3] = w;
    case TYPE_ANALOG_3CH: //standard dumb RGB
      _data[0] = r; _data[1] = g; _data[2] = b;
      break;
  }
}

//does no index check
uint32_t BusPwm::getPixelColor(unsigned pix) const {
  if (!_valid) return 0;
  // TODO getting the reverse from CCT is involved (a quick approximation when CCT blending is ste to 0 implemented)
  switch (_type) {
    case TYPE_ANALOG_1CH: //one channel (white), relies on auto white calculation
      return RGBW32(0, 0, 0, _data[0]);
    case TYPE_ANALOG_2CH: //warm white + cold white
      if (tkr_anim->cctICused) return RGBW32(0, 0, 0, _data[0]);
      else           return RGBW32(0, 0, 0, _data[0] + _data[1]);
    case TYPE_ANALOG_5CH: //RGB + warm white + cold white
      if (tkr_anim->cctICused) return RGBW32(_data[0], _data[1], _data[2], _data[3]);
      else           return RGBW32(_data[0], _data[1], _data[2], _data[3] + _data[4]);
    case TYPE_ANALOG_4CH: //RGBW
      return RGBW32(_data[0], _data[1], _data[2], _data[3]);
    case TYPE_ANALOG_3CH: //standard dumb RGB
      return RGBW32(_data[0], _data[1], _data[2], 0);
  }
  return RGBW32(_data[0], _data[0], _data[0], _data[0]);
}

void BusPwm::show() 
{
  
  #ifdef ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE


  uint16_t r = mapvalue(_data[0], 0, 255, 0, 1023);
  uint16_t g = mapvalue(_data[1], 0, 255, 0, 1023);
  uint16_t b = mapvalue(_data[2], 0, 255, 0, 1023);
  uint16_t w1 = mapvalue(_data[3], 0, 255, 0, 1023);
  uint16_t w2 =  mapvalue(_data[4], 0, 255, 0, 1023);
  
  uint16_t colour10bit[5] = {0};
  switch (_type) {
    default:
    case BUSTYPE_ANALOG_5CH: //RGB + warm white + cold white
      colour10bit[4] = w2;
      // NO BREAK
    case BUSTYPE_ANALOG_4CH: //RGBW
      colour10bit[3] = w1;
      // NO BREAK
    case BUSTYPE_ANALOG_3CH: //standard dumb RGB
      colour10bit[0] = r; 
      colour10bit[1] = g; 
      colour10bit[2] = b;
      break;
    case BUSTYPE_ANALOG_2CH: //warm white + cold white
      colour10bit[0] = w1;
      colour10bit[1] = w2;
      break;
    case BUSTYPE_ANALOG_1CH: //one channel (white), relies on auto white calculation
      colour10bit[0] = w1;
      break;
  }
  
  /**
   * @brief Final conversions
   * ** Upscale to 10 bit
   * ** Shrink into desired PWM range limits
   * Here colour is just a PWM value, the actual colour information is above and should be inserted correctly
   */
      DEBUG_LINE_HERE
  uint16_t pwm_value;
  uint8_t numPins = numPWMPins(_type);

  for(uint8_t ii=0;ii<numPins;ii++)
  {
    colour10bit[ii] = colour10bit[ii] > 0 ? mapvalue(colour10bit[ii], 0, tkr_set->Settings.pwm_range, tkr_iLight->pwm_min, tkr_iLight->pwm_max) : 0; 
    pwm_value = bitRead(tkr_set->runtime.pwm_inverted, ii) ? tkr_set->Settings.pwm_range - colour10bit[ii] : colour10bit[ii];


      DEBUG_LINE_HERE
    #ifdef ESP8266
    analogWrite(_pins[ii], pwm_value);
    #else
    ledcWrite(_ledcStart + ii, pwm_value);
    #endif
      DEBUG_LINE_HERE
  }

  #else
  if (!_valid) return;
  // if _needsRefresh is true (UI hack) we are using dithering (credit @dedehai & @zalatnaicsongor)
  // https://github.com/Aircoookie/WLED/pull/4115 and https://github.com/zalatnaicsongor/WLED/pull/1)
  const bool     dithering = _needsRefresh; // avoid working with bitfield
  const unsigned numPins = getPins();
  const unsigned maxBri = (1<<_depth);      // possible values: 16384 (14), 8192 (13), 4096 (12), 2048 (11), 1024 (10), 512 (9) and 256 (8) 
  [[maybe_unused]] const unsigned bitShift = dithering * 4;  // if dithering, _depth is 12 bit but LEDC channel is set to 8 bit (using 4 fractional bits)

  // use CIE brightness formula (linear + cubic) to approximate human eye perceived brightness
  // see: https://en.wikipedia.org/wiki/Lightness
  unsigned pwmBri = _bri;
  if (pwmBri < 21) {                                   // linear response for values [0-20]
    pwmBri = (pwmBri * maxBri + 2300 / 2) / 2300 ;     // adding '0.5' before division for correct rounding, 2300 gives a good match to CIE curve
  } else {                                             // cubic response for values [21-255]
    float temp = float(pwmBri + 41) / float(255 + 41); // 41 is to match offset & slope to linear part
    temp = temp * temp * temp * (float)maxBri;
    pwmBri = (unsigned)temp;                           // pwmBri is in range [0-maxBri] C
  }

  [[maybe_unused]] unsigned hPoint = 0;  // phase shift (0 - maxBri)
  // we will be phase shifting every channel by previous pulse length (plus dead time if required)
  // phase shifting is only mandatory when using H-bridge to drive reverse-polarity PWM CCT (2 wire) LED type 
  // CCT additive blending must be 0 (WW & CW will not overlap) otherwise signals *will* overlap
  // for all other cases it will just try to "spread" the load on PSU
  // Phase shifting requires that LEDC timers are synchronised (see setup()). For PWM CCT (and H-bridge) it is
  // also mandatory that both channels use the same timer (pinManager takes care of that).
  for (unsigned i = 0; i < numPins; i++) {
    unsigned duty = (_data[i] * pwmBri) / 255;    
    #ifdef ESP8266
    if (_reversed) duty = maxBri - duty;
    analogWrite(_pins[i], duty);
    #else
    int deadTime = 0;
    if (_type == BUSTYPE_ANALOG_2CH && Bus::getCCTBlend() == 0) {
      // add dead time between signals (when using dithering, two full 8bit pulses are required)
      deadTime = (1+dithering) << bitShift;
      // we only need to take care of shortening the signal at (almost) full brightness otherwise pulses may overlap
      if (_bri >= 254 && duty >= maxBri / 2 && duty < maxBri) duty -= deadTime << 1; // shorten duty of larger signal except if full on
      if (_reversed) deadTime = -deadTime; // need to invert dead time to make phaseshift go the opposite way so low signals dont overlap
    }
    if (_reversed) duty = maxBri - duty;
    unsigned channel = _ledcStart + i;
    unsigned gr = channel/8;  // high/low speed group
    unsigned ch = channel%8;  // group channel
    // directly write to LEDC struct as there is no HAL exposed function for dithering
    // duty has 20 bit resolution with 4 fractional bits (24 bits in total)
                                  // LEDC.channel_group[gr].channel[ch].duty.duty = duty << ((!dithering)*4);  // lowest 4 bits are used for dithering, shift by 4 bits if not using dithering
                                  // LEDC.channel_group[gr].channel[ch].hpoint.hpoint = hPoint >> bitShift;    // hPoint is at _depth resolution (needs shifting if dithering)
                                  // ledc_update_duty((ledc_mode_t)gr, (ledc_channel_t)ch);
    hPoint += duty + deadTime;        // offset to cascade the signals
    if (hPoint >= maxBri) hPoint = 0; // offset it out of bounds, reset
    #endif
  }
  #endif
  
      DEBUG_LINE_HERE
}

uint8_t BusPwm::getPins(uint8_t* pinArray) const {
  if (!_valid) return 0;
  unsigned numPins = numPWMPins(_type);
  if (pinArray) for (unsigned i = 0; i < numPins; i++) pinArray[i] = _pins[i];
  return numPins;
}

// credit @willmmiles & @netmindz https://github.com/Aircoookie/WLED/pull/4056
std::vector<LEDType> BusPwm::getLEDTypes() {
  return {
    {BUSTYPE_ANALOG_1CH, "A",      PSTR("PWM White")},
    {BUSTYPE_ANALOG_2CH, "AA",     PSTR("PWM CCT")},
    {BUSTYPE_ANALOG_3CH, "AAA",    PSTR("PWM RGB")},
    {BUSTYPE_ANALOG_4CH, "AAAA",   PSTR("PWM RGBW")},
    {BUSTYPE_ANALOG_5CH, "AAAAA",  PSTR("PWM RGB+CCT")},
    //{BUSTYPE_ANALOG_6CH, "AAAAAA", PSTR("PWM RGB+DCCT")}, // unimplementable ATM
  };
}


void BusPwm::deallocatePins() 
{
  uint8_t numPins = numPWMPins(_type);
  for (uint8_t i = 0; i < numPins; i++) {
    #ifdef ESP8266
    digitalWrite(_pins[i], LOW); //turn off PWM interrupt
    #else
    if (_ledcStart < 16) ledcDetachPin(_pins[i]);
    #endif
  }
  #ifdef ARDUINO_ARCH_ESP32
  // pinManager.deallocateLedc(_ledcStart, numPins);
  #endif
}

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusOnOff *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

BusOnOff::BusOnOff(const BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) 
{
  _valid = false;
  if (bc.type != BUSTYPE_ONOFF) return;
  uint8_t currentPin = bc.pins[0];
  _pin = currentPin; // store only after allocatePin() succeeds
  pinMode(_pin, OUTPUT);
  _reversed = bc.reversed;
  _valid = true;
}


void BusOnOff::setPixelColor(unsigned pix, uint32_t c) {
  if (pix != 0 || !_valid) return; //only react to first pixel
  _data = (c > 0) && bool(_bri) ? 0xFF : 0; // if any color channel is on and brightness is not zero, set to on
}

uint32_t BusOnOff::getPixelColor(unsigned pix) const {
  if (!_valid) return 0;
  return RGBW32(_data, _data, _data, _data);
}


void BusOnOff::show() {
  if (!_valid) return;
  digitalWrite(_pin, _reversed ? !(bool)_data : (bool)_data);
}

uint8_t BusOnOff::getPins(uint8_t* pinArray) const
{
  if (!_valid) return 0;
  pinArray[0] = _pin;
  return 1;
}

// credit @willmmiles & @netmindz https://github.com/Aircoookie/WLED/pull/4056
std::vector<LEDType> BusOnOff::getLEDTypes() {
  return {
    {BUSTYPE_ONOFF, "", PSTR("On/Off")},
  };
}


/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusNetwork *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

BusNetwork::BusNetwork(const BusConfig &bc)
: Bus(bc.type, bc.start, bc.autoWhite, bc.count)
, _broadcastLock(false)
{
  switch (bc.type) {
    case BUSTYPE_NET_ARTNET_RGB:
      _UDPtype = 2;
      break;
    case BUSTYPE_NET_ARTNET_RGBW:
      _UDPtype = 2;
      break;
    case BUSTYPE_NET_E131_RGB:
      _UDPtype = 1;
      break;
    default: // TYPE_NET_DDP_RGB / TYPE_NET_DDP_RGBW
      _UDPtype = 0;
      break;
  }
  _hasRgb = hasRGB(bc.type);
  _hasWhite = hasWhite(bc.type);
  _hasCCT = false;
  _UDPchannels = _hasWhite + 3;
  _client = IPAddress(bc.pins[0],bc.pins[1],bc.pins[2],bc.pins[3]);
  _valid = (allocateData(_len * _UDPchannels) != nullptr);
  DEBUG_PRINTF_P(PSTR("%successfully inited virtual strip with type %u and IP %u.%u.%u.%u\n"), _valid?"S":"Uns", bc.type, bc.pins[0], bc.pins[1], bc.pins[2], bc.pins[3]);
}


void BusNetwork::setPixelColor(unsigned pix, uint32_t c) {
  if (!_valid || pix >= _len) return;
  uint8_t ww, cw; // dummy, unused
  if (_hasWhite) c = autoWhiteCalc(c, ww, cw);
  if (Bus::_cct >= 1900) c = tkr_anim->colorBalanceFromKelvin(Bus::_cct, c); //color correction from CCT
  unsigned offset = pix * _UDPchannels;
  _data[offset]   = R(c);
  _data[offset+1] = G(c);
  _data[offset+2] = B(c);
  if (_hasWhite) _data[offset+3] = W(c);
}

uint32_t BusNetwork::getPixelColor(unsigned pix) const {
  if (!_valid || pix >= _len) return 0;
  unsigned offset = pix * _UDPchannels;
  return RGBW32(_data[offset], _data[offset+1], _data[offset+2], (hasWhite() ? _data[offset+3] : 0));
}



void BusNetwork::show() 
{
  if (!_valid || !canShow()) return;
  _broadcastLock = true;
  #ifdef ENABLE_FEATURE_LIGHTING__UDP_NOTIFIER
  realtimeBroadcast(_UDPtype, _client, _len, _data, _bri, _rgbw);
  #endif
  _broadcastLock = false;
}


uint8_t BusNetwork::getPins(uint8_t* pinArray) const
{
  for (uint8_t i = 0; i < 4; i++) {
    pinArray[i] = _client[i];
  }
  return 4;
}


void BusNetwork::cleanup() 
{
  _type = BUSTYPE__NONE__ID;
  _valid = false;
  if (_data != nullptr) free(_data);
  _data = nullptr;
}


// credit @willmmiles & @netmindz https://github.com/Aircoookie/WLED/pull/4056
std::vector<LEDType> BusNetwork::getLEDTypes() {
  return {
    {BUSTYPE_NET_DDP_RGB,     "N",     PSTR("DDP RGB (network)")},      // should be "NNNN" to determine 4 "pin" fields
    {BUSTYPE_NET_ARTNET_RGB,  "N",     PSTR("Art-Net RGB (network)")},
    {BUSTYPE_NET_DDP_RGBW,    "N",     PSTR("DDP RGBW (network)")},
    {BUSTYPE_NET_ARTNET_RGBW, "N",     PSTR("Art-Net RGBW (network)")},
    // hypothetical extensions
    //{BUSTYPE_VIRTUAL_I2C_W,   "V",     PSTR("I2C White (virtual)")}, // allows setting I2C address in _pin[0]
    //{BUSTYPE_VIRTUAL_I2C_CCT, "V",     PSTR("I2C CCT (virtual)")}, // allows setting I2C address in _pin[0]
    //{BUSTYPE_VIRTUAL_I2C_RGB, "VVV",   PSTR("I2C RGB (virtual)")}, // allows setting I2C address in _pin[0] and 2 additional values in _pin[1] & _pin[2]
    //{BUSTYPE_USERMOD,         "VVVVV", PSTR("Usermod (virtual)")}, // 5 data fields (see https://github.com/Aircoookie/WLED/pull/4123)
  };
}


BusPlaceholder::BusPlaceholder(const BusConfig &bc)
: Bus(bc.type, bc.start, bc.autoWhite, bc.count, bc.reversed, bc.refreshReq)
, _colorOrder(bc.colorOrder)
, _skipAmount(bc.skipAmount)
, _driverType(bc.driverType)
, _frequency(bc.frequency)
, _milliAmpsPerLed(bc.milliAmpsPerLed)
, _milliAmpsMax(bc.milliAmpsMax)
, _text(bc.text)
{
  memcpy(_pins, bc.pins, sizeof(_pins));
}

uint8_t BusPlaceholder::getPins(uint8_t* pinArray) const {
  size_t nPins = Bus::getNumberOfPins(_type);
  if (pinArray) {
    for (size_t i = 0; i < nPins; i++) pinArray[i] = _pins[i];
  }
  return nPins;
}

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusManager *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/



//utility to get the approx. memory usage of a given BusConfig inclduding segmentbuffer and global buffer (4 bytes per pixel)
size_t BusConfig::memUsage() const {
  
  ALOG_INF(PSTR(D_LOG_PIXEL "count=%d"),count);
  size_t mem = (count + skipAmount) * 8; // 8 bytes per pixel for segment + global buffer
  if (Bus::isVirtual(type)) {
    mem += sizeof(BusNetwork) + (count * Bus::getNumberOfChannels(type)); // note: getNumberOfChannels() includes CCT channel if applicable but virtual buses do not use CCT channel buffer
  } else if (Bus::isDigital(type)) {
    // if any of digital buses uses I2S, there is additional common I2S DMA buffer not accounted for here
    mem += sizeof(BusDigital) + PolyBus::memUsage(count + skipAmount, iType);
  } else if (Bus::isOnOff(type)) {
    mem += sizeof(BusOnOff);
  } else {
    mem += sizeof(BusPwm);
  }

  ALOG_INF(PSTR(D_LOG_PIXEL "memUsage=%d"),mem);

  return mem;
}


// int BusManager::add(BusConfig &bc) 
// {

//   DEBUG_LINE_HERE;

//   uint8_t bus_count = getNumBusses() - getNumVirtualBusses();
//   if (bus_count >= WLED_MAX_BUSSES) 
//   {
//     Serial.printf("if (bus_count >= WLED_MAX_BUSSES) %d\n\r", bus_count);
//     return -1;
//   }

//   DEBUG_LINE_HERE;
//   if (Bus::isVirtual(bc.type)) {
//     ALOG_INF(PSTR("BusManager::add::Type BusNetwork"));
//     busses[numBusses] = new BusNetwork(bc); // IP
//   } 
//    else if (Bus::isDigital(bc.type)) {
//     ALOG_INF(PSTR("BusManager::add::Type BusDigital"));
//     busses[numBusses] = new BusDigital(bc, numBusses, colorOrderMap); // Neopixel
   
//   } else if (Bus::isOnOff(bc.type)) {
//     ALOG_INF(PSTR("BusManager::add::Type BUSTYPE_ONOFF"));
//     busses[numBusses] = new BusOnOff(bc); // Relays
//   } 
//   else 
//   {
//     ALOG_INF(PSTR("BusManager::add::Type ELSE BusPwm"));
//     busses[numBusses] = new BusPwm(bc); // H801
//   }

//   numBusses++;
  
//   return numBusses;

// }

uint8_t BusManager::getI(uint8_t busType, const uint8_t* pins, uint8_t driverPreference) {
  return PolyBus::getI(busType, pins, driverPreference);
}

int BusManager::add(const BusConfig &bc, bool placeholder) 
{

  DEBUG_PRINTF_P(PSTR("Bus: Adding bus (p:%d v:%d)\n"), getNumBusses(), getNumVirtualBusses());

  ALOG_INF(
    PSTR(
      "BusConfig: "
      "type=%u count=%u count=%u start=%u "
      "colorOrder=%u reversed=%u skipAmount=%u refreshReq=%u autoWhite=%u "
      "pins=[%u,%u,%u,%u,%u] "
      "frequency=%u doubleBuffer=%u "
      "milliAmpsPerLed=%u milliAmpsMax=%u "
      "driverType=%u iType=%u text=\"%s\""
    ),
    (unsigned)bc.type,
    (unsigned)bc.count,
    (unsigned)bc.count,
    (unsigned)bc.start,
    (unsigned)bc.colorOrder,
    (unsigned)bc.reversed,
    (unsigned)bc.skipAmount,
    (unsigned)bc.refreshReq,
    (unsigned)bc.autoWhite,
    (unsigned)bc.pins[0],
    (unsigned)bc.pins[1],
    (unsigned)bc.pins[2],
    (unsigned)bc.pins[3],
    (unsigned)bc.pins[4],
    (unsigned)bc.frequency,
    (unsigned)bc.doubleBuffer,
    (unsigned)bc.milliAmpsPerLed,
    (unsigned)bc.milliAmpsMax,
    (unsigned)bc.driverType,
    (unsigned)bc.iType,
    bc.text.c_str()
  );

  unsigned digital = 0;
  unsigned analog  = 0;
  unsigned twoPin  = 0;
  for (const auto &bus : busses) {
    if (bus->isPWM()) analog += bus->getPins(); // number of analog channels used
    if (bus->isDigital() && !bus->is2Pin()) digital++;
    if (bus->is2Pin()) twoPin++;
  }
  digital += (Bus::isDigital(bc.type) && !Bus::is2Pin(bc.type));
  analog  += (Bus::isPWM(bc.type) ? Bus::numPWMPins(bc.type) : 0);
  if (digital > WLED_MAX_DIGITAL_CHANNELS || analog > WLED_MAX_ANALOG_CHANNELS) placeholder = true; // TODO: add errorFlag here
  if (placeholder) {
    busses.push_back(make_unique<BusPlaceholder>(bc));
  } else if (Bus::isVirtual(bc.type)) {
    busses.push_back(make_unique<BusNetwork>(bc));
#ifdef WLED_ENABLE_HUB75MATRIX
  } else if (Bus::isHub75(bc.type)) {
    busses.push_back(make_unique<BusHub75Matrix>(bc));
#endif
  } else if (Bus::isDigital(bc.type)) {
    ALOG_INF(PSTR("BusManager::add Digital %d"), bc.type);
    busses.push_back(make_unique<BusDigital>(bc));
  } else if (Bus::isOnOff(bc.type)) {
    busses.push_back(make_unique<BusOnOff>(bc));
  } else {
    busses.push_back(make_unique<BusPwm>(bc));
  }
  return busses.size();
}




// credit @willmmiles
static String LEDTypesToJson(const std::vector<LEDType>& types) {
  String json;
  for (const auto &type : types) {
    // capabilities follows similar pattern as JSON API
    int capabilities = Bus::hasRGB(type.id) | Bus::hasWhite(type.id)<<1 | Bus::hasCCT(type.id)<<2 | Bus::is16bit(type.id)<<4 | Bus::mustRefresh(type.id)<<5;
    char str[256];
    sprintf_P(str, PSTR("{i:%d,c:%d,t:\"%s\",n:\"%s\"},"), type.id, capabilities, type.type, type.name);
    json += str;
  }
  return json;
}


// credit @willmmiles & @netmindz https://github.com/Aircoookie/WLED/pull/4056
String BusManager::getLEDTypesJSONString() {
  String json = "[";
  json += LEDTypesToJson(BusDigital::getLEDTypes());
  json += LEDTypesToJson(BusOnOff::getLEDTypes());
  json += LEDTypesToJson(BusPwm::getLEDTypes());
  json += LEDTypesToJson(BusNetwork::getLEDTypes());
  //json += LEDTypesToJson(BusVirtual::getLEDTypes());
  json.setCharAt(json.length()-1, ']'); // replace last comma with bracket
  return json;
}


void BusManager::useParallelOutput(bool enable) // workaround for inaccessible PolyBus
{
  PolyBus::useParallelOutput(enable);
}


void BusManager::setRequiredChannels(uint8_t channels)
{
  PolyBus::setRequiredChannels(channels);
}


//do not call this method from system context (network callback)
void BusManager::removeAll() 
{
  ALOG_INF(PSTR(D_LOG_PIXEL "removeAll"));
  
  //prevents crashes due to deleting busses while in use.
  uint32_t start = millis();
  while (!canAllShow()) { 
    yield();
    if (millis() - start > 2000) break; // limit to at most 2 seconds
  }
  
  busses.clear();

  PolyBus::useParallelOutput(false);
}


// void BusManager::show() 
// {
//   _milliAmpsUsed = 0;
//   for (uint8_t i = 0; i < numBusses; i++) 
//   {
//     busses[i]->show();
//     _milliAmpsUsed += busses[i]->getUsedCurrent();
//   }
// }

void BusManager::show() {
  applyABL(); // apply brightness limit, updates _gMilliAmpsUsed
  for (auto &bus : busses) {
    bus->show();
  }
}

// bool BusManager::canAllShow() {
//   // #ifdef ENABLE_DEVFEATURE_LIGHTING__CANSHOW_BACKOFF
//   for (unsigned i = 0; i < numBusses; i++) {
//     if (!busses[i]->canShow()) return false;
//   }
//   // #endif
//   return true;
// }

bool BusManager::canAllShow() {
  for (const auto &bus : busses) if (!bus->canShow()) return false;
  return true;
}




void BusManager::initializeABL() {

  DEBUG_PRINT_LN("initializeABL");

  _useABL = false; // reset
  if (_gMilliAmpsMax > 0) {
    // check global brightness limit
    for (auto &bus : busses) {
  DEBUG_PRINT_LN("initializeABL 3"); 
      if (bus->isDigital() && bus->getLEDCurrent() > 0) {
        _useABL = true; // at least one bus has valid LED current
        
  DEBUG_PRINT_LN("initializeABL 1"); 
        return;
      }
    }
  } else {
    // check per bus brightness limit
    unsigned numABLbuses = 0;
    for (auto &bus : busses) {
      if (bus->isDigital() && bus->getLEDCurrent() > 0 && bus->getMaxCurrent() > 0)
        numABLbuses++; // count ABL enabled buses
    }
    if (numABLbuses > 0) {
      _useABL = true; // at least one bus has ABL set
      uint32_t ESPshare = MA_FOR_ESP / numABLbuses; // share of ESP current per ABL bus
      for (auto &bus : busses) {
        if (bus->isDigital() && bus->isOk()) {
          BusDigital &busd = static_cast<BusDigital&>(*bus);
          uint32_t busLength = busd.getLength();
          uint32_t busDemand = busLength * busd.getLEDCurrent();
          uint32_t busMax    = busd.getMaxCurrent();
          if (busMax > ESPshare)  busMax -= ESPshare;
          if (busMax < busLength) busMax  = busLength; // give each LED 1mA, ABL will dim down to minimum
          if (busDemand == 0) busMax = 0; // no LED current set, disable ABL for this bus
          busd.setCurrentLimit(busMax);
        }
      }
    }
  }
  DEBUG_PRINT_LN("initializeABL 2");
}

void BusManager::applyABL() {
  if (_useABL) {
    // ALOG_INF(PSTR(D_LOG_PIXEL "applyABL"));
    unsigned milliAmpsSum = 0; // use temporary variable to always return a valid _gMilliAmpsUsed to UI
    unsigned totalLEDs = 0;
    for (auto &bus : busses) {
      if (bus->isDigital() && bus->isOk()) {
        BusDigital &busd = static_cast<BusDigital&>(*bus);
        busd.estimateCurrent(); // sets _milliAmpsTotal, current is estimated for all buses even if they have the limit set to 0
        if (_gMilliAmpsMax == 0)
          busd.applyBriLimit(0); // apply per bus ABL limit, updates _milliAmpsTotal if limit reached
        milliAmpsSum += busd.getUsedCurrent();
        totalLEDs += busd.getLength(); // sum total number of LEDs for global Limit
      }
    }
    // check global current limit and apply global ABL limit, total current is summed above
    if (_gMilliAmpsMax > 0) {
      uint8_t  newBri = 255;
      uint32_t globalMax = _gMilliAmpsMax > MA_FOR_ESP ? _gMilliAmpsMax - MA_FOR_ESP : 1; // subtract ESP current consumption, fully limit if too low
      if (globalMax > totalLEDs) { // check if budget is larger than standby current
        if (milliAmpsSum > globalMax) {
          newBri = globalMax * 255 / milliAmpsSum + 1; // scale brightness down to stay in current limit, +1 to avoid 0 brightness
          milliAmpsSum = globalMax; // update total used current
        }
      } else {
        newBri = 1; // limit too low, set brightness to minimum
        milliAmpsSum = totalLEDs; // estimate total used current as minimum
      }

      // apply brightness limit to each bus, if its 255 it will only reset _colorSum
      for (auto &bus : busses) {
        if (bus->isDigital() && bus->isOk()) {
          BusDigital &busd = static_cast<BusDigital&>(*bus);
          if (busd.getLEDCurrent() > 0)  // skip buses with LED current set to 0
            busd.applyBriLimit(newBri);
        }
      }
    }
    _gMilliAmpsUsed = milliAmpsSum;
  }
  else
    _gMilliAmpsUsed = 0; // reset, we have no current estimation without ABL
}


void IRAM_ATTR BusManager::setPixelColor(unsigned pix, uint32_t c) {
  for (auto &bus : busses) {
    if (!bus->containsPixel(pix)) continue;
    bus->setPixelColor(pix - bus->getStart(), c);
  }
}


uint32_t BusManager::getPixelColor(unsigned pix) {
  for (auto &bus : busses) {
    if (!bus->containsPixel(pix)) continue;
    return bus->getPixelColor(pix - bus->getStart());
  }
  return 0;
}


void BusManager::setSegmentCCT(int16_t cct, bool allowWBCorrection) {
  if (cct > 255) cct = 255;
  if (cct >= 0) {
    //if white balance correction allowed, save as kelvin value instead of 0-255
    if (allowWBCorrection) cct = 1900 + (cct << 5);
  } else cct = -1; // will use kelvin approximation from RGB
  Bus::setCCT(cct);
}


// Bus static member definition
int16_t Bus::_cct = -1;
uint8_t Bus::_cctBlend = 0;
uint8_t Bus::_gAWM = 255;

uint16_t BusDigital::_milliAmpsTotal = 0;

bool PolyBus::useParallelI2S = false;
uint8_t PolyBus::_bri_rgb = 255; // current brightness for RGB
uint8_t PolyBus::required_channels = 0;


std::vector<std::unique_ptr<Bus>> BusManager::busses;
uint16_t BusManager::_gMilliAmpsUsed = 0;
uint16_t BusManager::_gMilliAmpsMax = ABL_MILLIAMPS_DEFAULT_MAX_SUPPLY_CURRENT;
bool BusManager::_useABL = false;


#endif // USE_MODULE_LIGHTS_INTERFACE