//this code is a modified version of https://github.com/Makuna/NeoPixelBus/issues/103
#ifndef NpbWrapper_h
#define NpbWrapper_h


//#define ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER


// try pulling in my animator directly
#ifdef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
//#include "6_Lights/_Interface/mInterfaceLight.h"
#endif// ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER


//PIN CONFIGURATION
#ifndef LEDPIN
#define LEDPIN 3  //strip pin. Any for ESP32, gpio2 or 3 is recommended for ESP8266 (gpio2/3 are labeled D4/RX on NodeMCU and Wemos)
#endif

//END CONFIGURATION

#if defined(USE_APA102) || defined(USE_WS2801) || defined(USE_LPD8806) || defined(USE_P9813)
 #define CLKPIN 0
 #define DATAPIN 2
 #if BTNPIN == CLKPIN || BTNPIN == DATAPIN
  #undef BTNPIN   // Deactivate button pin if it conflicts with one of the APA102 pins.
 #endif
#endif

//automatically uses the right driver method for each platform
#ifdef ARDUINO_ARCH_ESP32
 #ifdef USE_APA102
  #define PIXELMETHOD DotStarMethod
 #elif defined(USE_WS2801)
  #define PIXELMETHOD NeoWs2801Method
 #elif defined(USE_LPD8806)
  #define PIXELMETHOD Lpd8806Method
 #elif defined(USE_TM1814)
  #define PIXELMETHOD NeoTm1814Method  
 #elif defined(USE_P9813)
  #define PIXELMETHOD P9813Method  
 #else
  #define PIXELMETHOD NeoEsp32Rmt0Ws2812xMethod
 #endif
#else //esp8266
 //autoselect the right method depending on strip pin
 #ifdef USE_APA102
  #define PIXELMETHOD DotStarMethod
 #elif defined(USE_WS2801)
  #define PIXELMETHOD NeoWs2801Method
 #elif defined(USE_LPD8806)
  #define PIXELMETHOD Lpd8806Method
 #elif defined(USE_TM1814)
  #define PIXELMETHOD NeoTm1814Method  
 #elif defined(USE_P9813)
  #define PIXELMETHOD P9813Method  
 #elif LEDPIN == 2
  #define PIXELMETHOD NeoEsp8266Uart1Ws2813Method //if you get an error here, try to change to NeoEsp8266UartWs2813Method or update Neopixelbus
 #elif LEDPIN == 3
  #define PIXELMETHOD NeoEsp8266Dma800KbpsMethod
 #else
  #define PIXELMETHOD NeoEsp8266BitBang800KbpsMethod
  #pragma message "Software BitBang will be used because of your selected LED pin. This may cause flicker. Use GPIO 2 or 3 for best results."
 #endif
#endif

#define PIXELFEATURE3 NeoGrbFeature
#define PIXELFEATURE4 NeoGrbwFeature

#include <NeoPixelBrightnessBus.h>

enum NeoPixelType
{
  NeoPixelType_None = 0,
  NeoPixelType_Grb  = 1,
  NeoPixelType_Grbw = 2,
  NeoPixelType_End  = 3
};

class NeoPixelWrapper
{
public:
  NeoPixelWrapper() :
    // initialize each member to null
    #ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    _pGrb(NULL),
    _pGrbw(NULL),
    #endif// ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    _type(NeoPixelType_None)
  {

  }

  ~NeoPixelWrapper()
  {
    cleanup();
  }

  void Begin(NeoPixelType type, uint16_t countPixels)
  {
    cleanup();
    _type = type;

    #ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    switch (_type)
    {
      case NeoPixelType_Grb:
      #if defined(USE_APA102) || defined(USE_WS2801) || defined(USE_LPD8806) || defined(USE_P9813)
        _pGrb = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD>(countPixels, CLKPIN, DATAPIN);
      #else
        _pGrb = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD>(countPixels, LEDPIN);
      #endif
        _pGrb->Begin();
      break;

      case NeoPixelType_Grbw:
      #if defined(USE_APA102) || defined(USE_WS2801) || defined(USE_LPD8806) || defined(USE_P9813)
        _pGrbw = new NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD>(countPixels, CLKPIN, DATAPIN);
      #else
        _pGrbw = new NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD>(countPixels, LEDPIN);
      #endif
        _pGrbw->Begin();
      break;
    }
    #endif// ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER

  }

  void Show()
  {
    byte b;
#ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    switch (_type)
    {
      case NeoPixelType_Grb:  _pGrb->Show();  break;
      case NeoPixelType_Grbw: _pGrbw->Show(); break;
    }
#endif// ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
  }

  void SetPixelColor(uint16_t indexPixel, RgbwColor color)
  {
#ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    switch (_type) {
      case NeoPixelType_Grb: {
        _pGrb->SetPixelColor(indexPixel, RgbColor(color.R,color.G,color.B));
      }
      break;
      case NeoPixelType_Grbw: {
        #if defined(USE_LPD8806) || defined(USE_WS2801)
        _pGrbw->SetPixelColor(indexPixel, RgbColor(color.R,color.G,color.B));
        #else
        _pGrbw->SetPixelColor(indexPixel, color);
        #endif
      }
      break;
    }
#endif// ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    
  }

  void SetBrightness(byte b)
  {
#ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    switch (_type) {
      case NeoPixelType_Grb: _pGrb->SetBrightness(b);   break;
      case NeoPixelType_Grbw:_pGrbw->SetBrightness(b);  break;
    }
#endif // ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
  }

  // NOTE:  Due to feature differences, some support RGBW but the method name
  // here needs to be unique, thus GetPixeColorRgbw
  RgbwColor GetPixelColorRgbw(uint16_t indexPixel) const
  {
#ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    switch (_type) {
      case NeoPixelType_Grb:  return _pGrb->GetPixelColor(indexPixel);  break;
      case NeoPixelType_Grbw: return _pGrbw->GetPixelColor(indexPixel); break;
    }
#endif// ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    return 0;
  }

  uint8_t* GetPixels(void)
  {
    
#ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
    switch (_type) {
      case NeoPixelType_Grb:  return _pGrb->Pixels();  break;
      case NeoPixelType_Grbw: return _pGrbw->Pixels(); break;
    }
    return 0;
#endif // ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER

  }


private:
  NeoPixelType _type;

#ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
  // have a member for every possible type
  NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD>*  _pGrb;
  NeoPixelBrightnessBus<PIXELFEATURE4,PIXELMETHOD>* _pGrbw;  
#endif // ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER

  void cleanup()
  {
    switch (_type) {
      
#ifndef ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER
      case NeoPixelType_Grb:  delete _pGrb ; _pGrb  = NULL; break;
      case NeoPixelType_Grbw: delete _pGrbw; _pGrbw = NULL; break;      
#endif // ENABLE_DEVFEATURE_PHASEOUT_NEO_WRAPPER

    }
  }
};
#endif
