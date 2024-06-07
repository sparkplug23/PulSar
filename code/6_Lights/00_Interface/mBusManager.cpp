
#include <Arduino.h>
#include <IPAddress.h>
#include "const.h"
#include "mBusNeoWrapper.h"
#include "mBusManager.h"

#ifdef USE_MODULE_LIGHTS_INTERFACE


void ColorOrderMap::add(uint16_t start, uint16_t len, COLOUR_ORDER_T colorOrder) 
{

  // Serial.println("ColorOrderMap::add");

  if (_count >= WLED_MAX_COLOR_ORDER_MAPPINGS) {
  // Serial.printf("ColorOrderMap::add %d return \n\r",_count);
    return;
  }
  if (len == 0) {
  // Serial.printf("ColorOrderMap::add %d len == 0\n\r",_count);
    return;
  }
  _mappings[_count].start = start;
  _mappings[_count].len = len;  
  _mappings[_count].colorOrder = colorOrder;  
  _count++;
  Serial.printf("ColorOrderMap::add %d\n\r",_count);
}


COLOUR_ORDER_T IRAM_ATTR ColorOrderMap::getPixelColorOrder(uint16_t pix, COLOUR_ORDER_T defaultColorOrder) const 
{

  // COLOUR_ORDER_T colourtmp = {COLOUR_ORDER_INIT_DISABLED};
  // colourtmp.red = 0;
  // colourtmp.green = 1;
  // colourtmp.blue = 2;
  // Serial.println("getPixelColorOrder");
  // return colourtmp;

  if (_count == 0) return defaultColorOrder;
  for (uint8_t i = 0; i < _count; i++) 
  {
    if(
      pix >= _mappings[i].start && 
      pix < (_mappings[i].start + _mappings[i].len)
    ){
      return _mappings[i].colorOrder;
    }
  }
  return defaultColorOrder;
}


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



/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusDigital *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

BusDigital::BusDigital(BusConfig &bc, uint8_t digital_bus_number, const ColorOrderMap &com) : Bus(bc.type, bc.start, bc.autoWhite), _colorOrderMap(com) 
{
  DEBUG_LINE_HERE;
  if (!IS_BUSTYPE_DIGITAL(bc.type) || !bc.count)
  {    
    ALOG_ERR(PSTR("BusDigital::BusDigital !IS_BUSTYPE_DIGITAL(bc.type) || !bc.count"));
    return;
  }
  _pins[0] = bc.pins[0];
  if (IS_BUSTYPE_2PIN(bc.type)) 
  {
    _pins[1] = bc.pins[1];
  }
  reversed = bc.reversed;
  _needsRefresh = bc.refreshReq || bc.type == BUSTYPE_TM1814;
  _skip = bc.skipAmount;    //sacrificial pixels
  _len = bc.count + _skip;
  // DEBUG_LINE_HERE;
  _iType = PolyBus::getI(bc.type, _pins, digital_bus_number);
  // DEBUG_LINE_HERE;
  if (_iType == BUSTYPE__NONE__ID)
  {
    ALOG_ERR(PSTR("BusDigital::BusDigital _iType == BUSTYPE__NONE__ID"));
    return;
  }
  ALOG_INF(PSTR("iType %d"), _iType);
  // DEBUG_LINE_HERE;
  uint16_t lenToCreate = _len;
  // DEBUG_LINE_HERE;
  if (bc.type == BUSTYPE_WS2812_1CH_X3) lenToCreate = NUM_ICS_WS2812_1CH_3X(_len); // only needs a third of "RGB" LEDs for NeoPixelBus 
  // DEBUG_LINE_HERE;
  _busPtr = PolyBus::create(_iType, _pins, lenToCreate, digital_bus_number);
  // DEBUG_LINE_HERE;
  _valid = (_busPtr != nullptr);
  _colorOrder = bc.colorOrder;
  ALOG_INF(PSTR("%successfully inited strip %u (len %u) with type %u and pins %u,%u (itype %u)"), _valid?"S":"Uns", digital_bus_number, _len, bc.type, _pins[0],_pins[1],_iType);
}


void BusDigital::show() 
{
  // ALOG_INF(PSTR("show() %d"), micros());
  PolyBus::show(_busPtr, _iType);
}


bool BusDigital::canShow() 
{
  return PolyBus::canShow(_busPtr, _iType);
}


void IRAM_ATTR BusDigital::setPixelColor(uint16_t pix, RgbcctColor c) 
{
  if (reversed) pix = _len - pix -1;
  else pix += _skip;
  COLOUR_ORDER_T co = _colorOrderMap.getPixelColorOrder(pix+_start, _colorOrder);
  PolyBus::setPixelColor(_busPtr, _iType, pix, c, co);
}

RgbcctColor BusDigital::getPixelColor(uint16_t pix) 
{
  if (reversed) pix = _len - pix -1;
  else pix += _skip;
  COLOUR_ORDER_T co = _colorOrderMap.getPixelColorOrder(pix+_start, _colorOrder);
  return PolyBus::getPixelColor(_busPtr, _iType, pix, co);
}


uint8_t BusDigital::getPins(uint8_t* pinArray) 
{
  uint8_t numPins = IS_BUSTYPE_2PIN(_type) ? 2 : 1;
  for (uint8_t i = 0; i < numPins; i++) 
  {
    pinArray[i] = _pins[i];
  }
  return numPins;
}

void BusDigital::setColorOrder(COLOUR_ORDER_T colorOrder) 
{
  _colorOrder = colorOrder;
}

void BusDigital::reinit() {
  PolyBus::begin(_busPtr, _iType, _pins);
}

void BusDigital::cleanup() 
{
  DEBUG_PRINTLN(F("Digital Cleanup."));
  PolyBus::cleanup(_busPtr, _iType);
  _iType = BUSTYPE__NONE__ID;
  _valid = false;
  _busPtr = nullptr;
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


BusPwm::BusPwm(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) 
{

  ALOG_DBM(PSTR("BusPwm bc.type %d"), bc.type);

  _valid = false;
  if (!IS_BUSTYPE_PWM(bc.type)) return;
  uint8_t numPins = NUM_BUSTYPE_PWM_PINS(bc.type);

  ALOG_INF(PSTR("BusPwm::BusPwm numPins %d"), numPins);

  #ifdef ESP8266
  analogWriteRange(255);  //same range as one RGB channel
  analogWriteFreq(WLED_PWM_FREQ);
  #else
  _ledcStart = allocateLedc(numPins);
  if (_ledcStart == 255) { //no more free LEDC channels
    deallocatePins(); return;
  }
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
  reversed = bc.reversed;
  _valid = true;
}


void BusPwm::setPixelColor(uint16_t pix, RgbcctColor c) 
{
  if (pix != 0 || !_valid) return; // only react to first pixel
  // ALOG_INF(PSTR("BusPwm::setPixelColor seg%d, pix%d"), pCONT_lAni->getCurrSegmentId(), pix);
  output_colour = c;
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  output_colour.debug_print("BusPwm::setPixelColor::output_colour++++++++++++++++++++++++++");
  #endif
}


RgbcctColor BusPwm::getPixelColor(uint16_t pix) 
{
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "BusPwm::getPixelColor ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^seg%d, pix%d"), pCONT_lAni->getCurrSegmentId(), pix);
  #endif
  if (pix != 0 || !_valid) return RgbcctColor(0); // only react to first pixel
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  output_colour.debug_print("BusPwm::getPixelColor::output_colour");
  #endif //  ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  return output_colour;
}


void BusPwm::show() 
{

  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  ALOG_INF(PSTR("*********************************************************BusPwm::show"));
  #endif // ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

  if (!_valid){
    return;
  }

  uint16_t r = mapvalue(output_colour.R, 0, 255, 0, 1023);
  uint16_t g = mapvalue(output_colour.G, 0, 255, 0, 1023);
  uint16_t b = mapvalue(output_colour.B, 0, 255, 0, 1023);
  uint16_t w1 = mapvalue(output_colour.W1, 0, 255, 0, 1023);
  uint16_t w2 = mapvalue(output_colour.W2, 0, 255, 0, 1023);
  
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  output_colour.debug_print_states("output_colour");
  #endif // ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

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
  
  #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  ALOG_INF(PSTR("BusPwm::show [%d,%d,%d,%d,%d]"), colour10bit[0], colour10bit[1], colour10bit[2], colour10bit[3], colour10bit[4]);
  #endif // ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

  /**
   * @brief Final conversions
   * ** Upscale to 10 bit
   * ** Shrink into desired PWM range limits
   * Here colour is just a PWM value, the actual colour information is above and should be inserted correctly
   */
  uint16_t pwm_value;
  uint8_t numPins = NUM_BUSTYPE_PWM_PINS(_type);
  for(uint8_t ii=0;ii<numPins;ii++)
  {
    colour10bit[ii] = colour10bit[ii] > 0 ? mapvalue(colour10bit[ii], 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0; 
    pwm_value = bitRead(pCONT_set->runtime.pwm_inverted, ii) ? pCONT_set->Settings.pwm_range - colour10bit[ii] : colour10bit[ii];

    #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
    ALOG_DBM(PSTR("BusPwm[%d]::pwm_value[%d] %d"), pCONT_lAni->getCurrSegmentId(), ii, pwm_value);
    #endif // ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28

    #ifdef ESP8266
    analogWrite(_pins[ii], pwm_value);
    #else
    ledcWrite(_ledcStart + ii, pwm_value);
    #endif
  }
  
}


uint8_t BusPwm::getPins(uint8_t* pinArray) 
{
  if (!_valid) return 0;
  uint8_t numPins = NUM_BUSTYPE_PWM_PINS(_type);
  for (uint8_t i = 0; i < numPins; i++) {
    pinArray[i] = _pins[i];
  }
  return numPins;
}


void BusPwm::deallocatePins() 
{
  uint8_t numPins = NUM_BUSTYPE_PWM_PINS(_type);
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

void BusPwm::setColorOrder(COLOUR_ORDER_T colorOrder) 
{
  _colorOrder = colorOrder;
}

/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusOnOff *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/


BusOnOff::BusOnOff(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) 
{
  _valid = false;
  if (bc.type != BUSTYPE_ONOFF) return;
  uint8_t currentPin = bc.pins[0];
  _pin = currentPin; // store only after allocatePin() succeeds
  pinMode(_pin, OUTPUT);
  reversed = bc.reversed;
  _valid = true;
}

void BusOnOff::setPixelColor(uint16_t pix, RgbcctColor c) 
{
  if (pix != 0 || !_valid) return; // only react to first pixel
  uint32_t colour = RgbcctColor::GetU32Colour(c);
  _data = bool(colour) ? 0xFF : 0;
}

RgbcctColor BusOnOff::getPixelColor(uint16_t pix) 
{
  if (!_valid) return 0;
  return RgbcctColor(_data);
}

void BusOnOff::show() 
{
  if (!_valid) return;
  digitalWrite(_pin, reversed ? !(bool)_data : (bool)_data);
}

uint8_t BusOnOff::getPins(uint8_t* pinArray) 
{
  if (!_valid) return 0;
  pinArray[0] = _pin;
  return 1;
}


/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusNetwork *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/


BusNetwork::BusNetwork(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) {
  _valid = false;
//      switch (bc.type) {
//        case BUSTYPE_NET_ARTNET_RGB:
//          _rgbw = false;
//          _UDPtype = 2;
//          break;
//        case BUSTYPE_NET_E131_RGB:
//          _rgbw = false;
//          _UDPtype = 1;
//          break;
//        case BUSTYPE_NET_DDP_RGB:
//          _rgbw = false;
//          _UDPtype = 0;
//          break;
//        default: // BUSTYPE_NET_DDP_RGB / BUSTYPE_NET_DDP_RGBW
      _rgbw = bc.type == BUSTYPE_NET_DDP_RGBW;
      _UDPtype = 0;
//          break;
//      }
  _UDPchannels = _rgbw ? 4 : 3;
  _data = (byte *)malloc(bc.count * _UDPchannels);
  if (_data == nullptr) return;
  memset(_data, 0, bc.count * _UDPchannels);
  _len = bc.count;
  _client = IPAddress(bc.pins[0],bc.pins[1],bc.pins[2],bc.pins[3]);
  _broadcastLock = false;
  _valid = true;
}

void BusNetwork::setPixelColor(uint16_t pix, RgbcctColor c) 
{
//   if (!_valid || pix >= _len) return;
//   if (hasWhite()) c = autoWhiteCalc(c);
// //   if (_cct >= 1900) c = colorBalanceFromKelvin(_cct, c); //color correction from CCT
//   uint16_t offset = pix * _UDPchannels;
//   _data[offset]   = R(c);
//   _data[offset+1] = G(c);
//   _data[offset+2] = B(c);
//   if (_rgbw) _data[offset+3] = W(c);
}

RgbcctColor BusNetwork::getPixelColor(uint16_t pix) 
{
  // if (!_valid || pix >= _len) return 0;
  // uint16_t offset = pix * _UDPchannels;
  // return RGBW32(_data[offset], _data[offset+1], _data[offset+2], _rgbw ? (_data[offset+3] << 24) : 0);
}

void BusNetwork::show() 
{
  if (!_valid || !canShow()) return;
  _broadcastLock = true;
//   realtimeBroadcast(_UDPtype, _client, _len, _data, _bri, _rgbw);
  _broadcastLock = false;
}

uint8_t BusNetwork::getPins(uint8_t* pinArray) 
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


/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ** BusManager *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/



//utility to get the approx. memory usage of a given BusConfig
uint32_t BusManager::memUsage(BusConfig &bc) 
{
  uint8_t type = bc.type;
  uint16_t len = bc.count + bc.skipAmount;
  if (type > 15 && type < 32) 
  {
    #ifdef ESP8266
      if (bc.pins[0] == 3) { //8266 DMA uses 5x the mem
        if (type > 29) return len*20; //RGBW
        return len*15;
      }
      if (type > 29) return len*4; //RGBW
      return len*3;
    #else //ESP32 RMT uses double buffer?
      if (type > 29) return len*8; //RGBW
      return len*6;
    #endif
  }
  if (type > 31 && type < 48)   return 5;
  if (type == 44 || type == 45) return len*4; //RGBW
  return len*3; //RGB
}

int BusManager::add(BusConfig &bc) 
{

  DEBUG_LINE_HERE;

  uint8_t bus_count = getNumBusses() - getNumVirtualBusses();
  if (bus_count >= WLED_MAX_BUSSES) 
  {
    Serial.printf("if (bus_count >= WLED_MAX_BUSSES) %d\n\r", bus_count);
    return -1;
  }

  DEBUG_LINE_HERE;
  if(
    bc.type >= BUSTYPE_NET_DDP_RGB && 
    bc.type < 96) 
  {
    ALOG_INF(PSTR("BusManager::add::Type BusNetwork"));
    busses[numBusses] = new BusNetwork(bc); // IP
  } 
  else if(IS_BUSTYPE_DIGITAL(bc.type)) 
  {
    ALOG_INF(PSTR("BusManager::add::Type BusDigital"));
    busses[numBusses] = new BusDigital(bc, numBusses, colorOrderMap); // Neopixel
  } 
  else if(bc.type == BUSTYPE_ONOFF) 
  {
    ALOG_INF(PSTR("BusManager::add::Type BUSTYPE_ONOFF"));
    busses[numBusses] = new BusOnOff(bc); // Relays
  } 
  else 
  {
    ALOG_INF(PSTR("BusManager::add::Type ELSE"));
    busses[numBusses] = new BusPwm(bc); // H801
  }

  numBusses++;

  ALOG_INF(PSTR("BusManager::add::numBusses %d"), numBusses);
  
  return numBusses;

}

//do not call this method from system context (network callback)
void BusManager::removeAll() 
{
  DEBUG_PRINTLN(F("Removing all."));
  //prevents crashes due to deleting busses while in use.
  DEBUG_LINE_HERE;
  // while (!canAllShow()) yield(); //potentially freezing code YES, FREEZES ON BOOT
  DEBUG_LINE_HERE;

  for (uint8_t i = 0; i < numBusses; i++) 
  {
    ALOG_INF(PSTR("i=>%d"), i);
    if(busses[i]!=nullptr)
    {
      delete busses[i]; // change to vectors?
    }
  }
  
  DEBUG_LINE_HERE;
  
  numBusses = 0;
}

void BusManager::show() 
{
  for (uint8_t i = 0; i < numBusses; i++) 
  {
    #ifdef ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
    Serial.printf("busses[%d|%d]->show()------------------------------------\n\r", i,numBusses);
    #endif // ENABLE_DEBUGFEATURE_LIGHT__MULTIPIN_JUNE28
  
    busses[i]->show();
  }
}


void IRAM_ATTR BusManager::setPixelColor(uint16_t pix, RgbcctColor c, int16_t cct) 
{
  // DEBUG_LINE_HERE;
  // ALOG_INF(PSTR("numBusses = %d"),numBusses);
  // c.debug_print("BusManager::setPixelColor");
  for (uint8_t i = 0; i < numBusses; i++) 
  {
    Bus* b = busses[i];
    uint16_t bstart = b->getStart();
    if (pix < bstart || pix >= bstart + b->getLength()) continue;
    // ALOG_INF(PSTR("busses %d pix %d"), i, pix);
    busses[i]->setPixelColor(pix - bstart, c);
  }
}


RgbcctColor BusManager::getPixelColor(uint16_t pix) 
{
  for (uint8_t i = 0; i < numBusses; i++) {
    Bus* b = busses[i];
    uint16_t bstart = b->getStart();
    if (pix < bstart || pix >= bstart + b->getLength()) continue;
    return b->getPixelColor(pix - bstart);
  }
  return 0;
}


void BusManager::setSegmentCCT(int16_t cct, bool allowWBCorrection) {
  ALOG_ERR(PSTR("BusManager::setSegmentCCT needs WLED converted to PulSar CCT typing.")); // these functions are likely still needed to pass into the proper functions. 
  // if (cct > 255) cct = 255;
  // if (cct >= 0) {
  //   //if white balance correction allowed, save as kelvin value instead of 0-255
  //   if (allowWBCorrection) cct = 1900 + (cct << 5);
  // } else cct = -1;
  // Bus::setCCT(cct);
}


bool BusManager::canAllShow() {
  ALOG_INF(PSTR("BusManager::canAllShow::numBusses=%d"),numBusses);
  for (uint8_t i = 0; i < numBusses; i++) {
    if(busses[i])
    {
      if (!busses[i]->canShow())
      {
        return false;
      }
    }
  }
  return true;
}


Bus* BusManager::getBus(uint8_t busNr) {
  if (busNr >= numBusses) return nullptr;
  return busses[busNr];
}

//semi-duplicate of strip.getLengthTotal() (though that just returns strip._length, calculated in finalizeInit())
uint16_t BusManager::getTotalLength() {
  uint16_t len = 0;
  for (uint8_t i=0; i<numBusses; i++) len += busses[i]->getLength();
  return len;
}


const char* BusManager::getColourOrderName(COLOUR_ORDER_T _colorOrder, char* buffer, uint8_t len)
{
  Serial.println(_colorOrder.data, BIN);

  if(len>=5)
  {
    if(_colorOrder.red < 5) buffer[_colorOrder.red] = 'r';
    if(_colorOrder.green < 5) buffer[_colorOrder.green] = 'g';
    if(_colorOrder.blue < 5) buffer[_colorOrder.blue] = 'b';
    if(_colorOrder.white_cold < 5) buffer[_colorOrder.white_cold] = 'c';
    if(_colorOrder.white_warm < 5) buffer[_colorOrder.white_warm] = 'w';
  }
  return buffer;
}


#ifdef ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES
uint32_t Bus::autoWhiteCalc(uint32_t c) 
{
  uint8_t aWM = _autoWhiteMode;
  if (_gAWM < 255) aWM = _gAWM;
  if (aWM == RGBW_MODE_MANUAL_ONLY) return c;
  uint8_t w = W(c);
  //ignore auto-white calculation if w>0 and mode DUAL (DUAL behaves as BRIGHTER if w==0)
  if (w > 0 && aWM == RGBW_MODE_DUAL) return c;
  uint8_t r = R(c);
  uint8_t g = G(c);
  uint8_t b = B(c);
  if (aWM == RGBW_MODE_MAX) return RGBW32(r, g, b, r > g ? (r > b ? r : b) : (g > b ? g : b)); // brightest RGB channel
  w = r < g ? (r < b ? r : b) : (g < b ? g : b);
  if (aWM == RGBW_MODE_AUTO_ACCURATE) { r -= w; g -= w; b -= w; } //subtract w in ACCURATE mode
  return RGBW32(r, g, b, w);
}
#endif // ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES

// Bus static member definition
#ifdef ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES
uint8_t Bus::_gAWM = 255; // auto white mode
#endif // ENABLE_DEVFEATURE_LIGHT__BUS_AUTO_WHITE_MODES

#endif // USE_MODULE_LIGHTS_INTERFACE