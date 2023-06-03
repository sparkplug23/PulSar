
#include <Arduino.h>
#include <IPAddress.h>
#include "const.h"
#include "mBusNeoWrapper.h"
#include "mBusManager.h"


void ColorOrderMap::add(uint16_t start, uint16_t len, COLOUR_ORDER_T colorOrder) {
  if (_count >= WLED_MAX_COLOR_ORDER_MAPPINGS) {
    return;
  }
  if (len == 0) {
    return;
  }
  _mappings[_count].start = start;
  _mappings[_count].len = len;  
  _mappings[_count].colorOrder = colorOrder;  
  _count++;
}


COLOUR_ORDER_T IRAM_ATTR ColorOrderMap::getPixelColorOrder(uint16_t pix, COLOUR_ORDER_T defaultColorOrder) const 
{
  if (_count == 0) return defaultColorOrder;
  for (uint8_t i = 0; i < _count; i++) 
  {
    if (pix >= _mappings[i].start && pix < (_mappings[i].start + _mappings[i].len)) 
    {
      return _mappings[i].colorOrder;
    }
  }
  return defaultColorOrder;
}


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


/*****************************************************************************************************************************************************************
 ***************************************************************************************************************************************************************** 
 ** BusDigital *************************************************************************************************************************************************** 
 ***************************************************************************************************************************************************************** 
 *****************************************************************************************************************************************************************/

BusDigital::BusDigital(BusConfig &bc, uint8_t nr, const ColorOrderMap &com) : Bus(bc.type, bc.start, bc.autoWhite), _colorOrderMap(com) 
{
  if (!IS_BUSTYPE_DIGITAL(bc.type) || !bc.count) return;
  _pins[0] = bc.pins[0];
  if (IS_BUSTYPE_2PIN(bc.type)) 
  {
    _pins[1] = bc.pins[1];
  }
  reversed = bc.reversed;
  _needsRefresh = bc.refreshReq || bc.type == BUSTYPE_TM1814;
  _skip = bc.skipAmount;    //sacrificial pixels
  _len = bc.count + _skip;
  _iType = PolyBus::getI(bc.type, _pins, nr);
  if (_iType == BUSTYPE__NONE__ID) return;
  uint16_t lenToCreate = _len;
  if (bc.type == BUSTYPE_WS2812_1CH_X3) lenToCreate = NUM_ICS_WS2812_1CH_3X(_len); // only needs a third of "RGB" LEDs for NeoPixelBus 
  _busPtr = PolyBus::create(_iType, _pins, lenToCreate, nr);
  _valid = (_busPtr != nullptr);
  _colorOrder = bc.colorOrder;
  DEBUG_PRINTF("%successfully inited strip %u (len %u) with type %u and pins %u,%u (itype %u)\n", _valid?"S":"Uns", nr, _len, bc.type, _pins[0],_pins[1],_iType);
}


void BusDigital::show() 
{
  PolyBus::show(_busPtr, _iType);
}


bool BusDigital::canShow() 
{
  return PolyBus::canShow(_busPtr, _iType);
}


void BusDigital::setBrightness(uint8_t b) 
{
  Bus::setBrightness(b);
  // PolyBus::setBrightness(_busPtr, _iType, b);
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
#endif

void BusPwm::deallocateLedc(byte pos, byte channels)
{
  for (byte j = pos; j < pos + channels; j++) {
    if (j > MAX_LED_CHANNELS) return;
    byte by = j >> 3;
    byte bi = j - 8*by;
    bitWrite(ledcAlloc[by], bi, false);
  }
}

BusPwm::BusPwm(BusConfig &bc) : Bus(bc.type, bc.start, bc.autoWhite) 
{

  DEBUG_PRINTF("BusPwm bc.type %d\n\r", bc.type);

  _valid = false;
  if (!IS_BUSTYPE_PWM(bc.type)) return;
  uint8_t numPins = NUM_BUSTYPE_PWM_PINS(bc.type);

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
    #ifdef ESP8266
    pinMode(_pins[i], OUTPUT);
    #else
    ledcSetup(_ledcStart + i, WLED_PWM_FREQ, 10);//8); //10bit
    ledcAttachPin(_pins[i], _ledcStart + i);
    #endif

    bitSet(pCONT_set->pwm_inverted, i); //tmp fix


  }
  reversed = bc.reversed;
  _valid = true;
}

void BusPwm::setPixelColor(uint16_t pix, RgbcctColor c) 
{

  // Serial.printf("void BusPwm::setPixelColor(uint16_t %d, RgbcctColor c)\n\r",pix);

  if (pix != 0 || !_valid) 
  {
    // Serial.println("RETURN");
    return; //only react to first pixel
  }
    // Serial.println("NOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOT RETURN");
  // if (_type != TYPE_ANALOG_3CH) c = autoWhiteCalc(c);
  // if (_cct >= 1900 && (_type == TYPE_ANALOG_3CH || _type == TYPE_ANALOG_4CH)) {
  //   // c = colorBalanceFromKelvin(_cct, c); //color correction from CCT
  // }
  // uint8_t cct = 0; //0 - full warm white, 255 - full cold white

  output_colour = c;

  // ALOG_INF(PSTR(" pix %d. BusPwm::setPixelColor %d [%d,%d,%d,%d,%d]\n\r"), pix, _ledcStart, c.R, c.G, c.B, c.W1, c.W2);
  // [0], colour10bit[1], colour10bit[2], colour10bit[3], colour10bit[4]);

  // uint16_t r = c.R;
  // uint16_t g = c.G;
  // uint16_t b = c.B;
  // uint16_t ww= c.WW;
  // uint16_t cw = c.WC;

  // /**
  //  * @brief Calculate white value as maximum of channels etc
  //  **/
  // uint16_t w = c.W1;
  // if (_type != TYPE_ANALOG_3CH)
  // {
  //   // c = autoWhiteCalc(c);
  // }



  // // _data10[0] = mapvalue(c.R,  0, 255, 0, 1023);
  // // _data10[1] = mapvalue(c.G,  0, 255, 0, 1023);
  // // _data10[2] = mapvalue(c.B,  0, 255, 0, 1023);
  // // _data10[3] = mapvalue(c.WC, 0, 255, 0, 1023);
  // // _data10[4] = mapvalue(c.WW, 0, 255, 0, 1023);




  //       // // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "Cur_Col%d 10 bits %d"), i, cur_col_10[i]);
  //       // cur_col = cur_col_10[i + pCONT_iLight->settings.pwm_offset]; //leak chance
        
  //       // cur_col = cur_col > 0 ? mapvalue(cur_col, 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0;   
                
  //       // pwm_value = bitRead(pCONT_set->pwm_inverted, i) ? pCONT_set->Settings.pwm_range - cur_col : cur_col;


  // switch (_type) {
  //   case TYPE_ANALOG_1CH: //one channel (white), relies on auto white calculation
  //     _data[0] = w;
  //     break;
  //   case TYPE_ANALOG_2CH: //warm white + cold white
  //     _data[1] = cw;
  //     _data[0] = ww;
  //     break;
  //   case TYPE_ANALOG_5CH: //RGB + warm white + cold white
  //     _data[4] = cw;
  //     w = ww;
  //     // No Break
  //   case TYPE_ANALOG_4CH: //RGBW
  //     _data[3] = w;
  //     // No Break
  //   case TYPE_ANALOG_3CH: //standard dumb RGB
  //     _data[0] = r; 
  //     _data[1] = g; 
  //     _data[2] = b;
  //     break;
  // }
  
  // _data[0] = c.R; 
  // _data[1] = c.G; 
  // _data[2] = c.B;
  // _data[3] = c.W1; 
  // _data[4] = c.W2;

  // /**
  //  * @brief Final conversions
  //  * ** Upscale to 10 bit
  //  * ** Shrink into desired PWM range limits
  //  */
  // uint16_t cur_col;
  // uint8_t numPins = NUM_PWM_PINS(_type);
  // for(uint8_t ii=0;ii<numPins;ii++)
  // {
  //   cur_col = _data[ii];
  //   cur_col = mapvalue(cur_col, 0, 255, 0, 1023);
  //   cur_col = cur_col > 0 ? mapvalue(cur_col, 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0; 
  //   _data[ii] = cur_col;
  // }

  // DEBUG_PRINTF("BusPwm::setPixelColor _type %d, data [%d,%d,%d,%d,%d]\n\r", _type, _data[0], _data[1], _data[2], _data[3], _data[4]);


}

//does no index check
RgbcctColor BusPwm::getPixelColor(uint16_t pix) 
{
  if (!_valid){
   
  //  Serial.println("INVALID=============================================================");
    return 0;
  }
  //  Serial.println("VALID=============================================================");
  return output_colour;
}

void BusPwm::show() 
{
  if (!_valid) return;
  uint16_t pwm_value;

  uint16_t colour10bit[5];

  // Convert into full range value
  colour10bit[0] = mapvalue(output_colour.R, 0, 255, 0, 1023);
  colour10bit[1] = mapvalue(output_colour.G, 0, 255, 0, 1023);
  colour10bit[2] = mapvalue(output_colour.B, 0, 255, 0, 1023);
  colour10bit[3] = mapvalue(output_colour.WC, 0, 255, 0, 1023);
  colour10bit[4] = mapvalue(output_colour.WW, 0, 255, 0, 1023);
  
  // DEBUG_PRINTF("BusPwm::show %d [%d,%d,%d,%d,%d]\n\r", _ledcStart, colour10bit[0], colour10bit[1], colour10bit[2], colour10bit[3], colour10bit[4]);

  /**
   * @brief Final conversions
   * ** Upscale to 10 bit
   * ** Shrink into desired PWM range limits
   */
  uint16_t cur_col;
  uint8_t numPins = NUM_BUSTYPE_PWM_PINS(_type);
  for(uint8_t ii=0;ii<numPins;ii++)
  {
    colour10bit[ii] = colour10bit[ii] > 0 ? mapvalue(colour10bit[ii], 0, pCONT_set->Settings.pwm_range, pCONT_iLight->pwm_min, pCONT_iLight->pwm_max) : 0; 
    pwm_value = bitRead(pCONT_set->pwm_inverted, ii) ? pCONT_set->Settings.pwm_range - colour10bit[ii] : colour10bit[ii];
    #ifdef ESP8266
    analogWrite(_pins[i], pwm_value);
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
  _pin = currentPin; //store only after allocatePin() succeeds
  pinMode(_pin, OUTPUT);
  reversed = bc.reversed;
  _valid = true;
}

void BusOnOff::setPixelColor(uint16_t pix, RgbcctColor c) 
{
  if (pix != 0 || !_valid) return; //only react to first pixel
  uint32_t colour = RgbcctColor::GetU32Colour(c);
  _data = bool(colour && _bri) ? 0xFF : 0;
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

  uint8_t bus_count = getNumBusses() - getNumVirtualBusses();
  if (bus_count >= WLED_MAX_BUSSES) 
  {
    Serial.printf("if (bus_count >= WLED_MAX_BUSSES) %d\n\r", bus_count);
    return -1;
  }

  if (bc.type >= BUSTYPE_NET_DDP_RGB && bc.type < 96) 
  {
    busses[numBusses] = new BusNetwork(bc);   // IP
  } 
  else if (IS_BUSTYPE_DIGITAL(bc.type)) 
  {
    busses[numBusses] = new BusDigital(bc, numBusses, colorOrderMap);  // Neopixel
  } 
  else if (bc.type == BUSTYPE_ONOFF) 
  {
    busses[numBusses] = new BusOnOff(bc);  // Relays
  } 
  else 
  {
    busses[numBusses] = new BusPwm(bc); // H801
  }
  
  return numBusses++;

}

//do not call this method from system context (network callback)
void BusManager::removeAll() 
{
  DEBUG_PRINTLN(F("Removing all."));
  //prevents crashes due to deleting busses while in use.
  while (!canAllShow()) yield(); //potentially freezing code
  for (uint8_t i = 0; i < numBusses; i++) delete busses[i];
  numBusses = 0;
}

void BusManager::show() 
{
  for (uint8_t i = 0; i < numBusses; i++) 
  {
    busses[i]->show();
  }
}


void IRAM_ATTR BusManager::setPixelColor(uint16_t pix, RgbcctColor c, int16_t cct) 
{
  // ALOG_INF(PSTR("numBusses = %d"),numBusses);
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









void BusManager::setBrightness(uint8_t b) {
  for (uint8_t i = 0; i < numBusses; i++) {
    busses[i]->setBrightness(b);
  }
}

void BusManager::setSegmentCCT(int16_t cct, bool allowWBCorrection) {
  if (cct > 255) cct = 255;
  if (cct >= 0) {
    //if white balance correction allowed, save as kelvin value instead of 0-255
    if (allowWBCorrection) cct = 1900 + (cct << 5);
  } else cct = -1;
  Bus::setCCT(cct);
}


bool BusManager::canAllShow() {
  for (uint8_t i = 0; i < numBusses; i++) {
    if (!busses[i]->canShow()) return false;
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

// Bus static member definition
int16_t Bus::_cct = -1;
uint8_t Bus::_cctBlend = 0;
uint8_t Bus::_gAWM = 255;

