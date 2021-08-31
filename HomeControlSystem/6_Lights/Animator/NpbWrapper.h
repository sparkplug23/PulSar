// //this code is a modified version of https://github.com/Makuna/NeoPixelBus/issues/103
// #ifndef NpbWrapper_h
// #define NpbWrapper_h

// /**
//  * To combine software together, it is essential this wrapper is removed to use that found in ilight
//  * 
//  * */


// #include "1_TaskerManager/mTaskerInterface.h"
// // #include "6_Lights/_Interface/mInterfaceLight.h"
// // #include "6_Lights/Animator/mAnimatorLight.h"

// //PIN CONFIGURATION
// #ifndef LEDPIN
// #define LEDPIN 3  //strip pin. Any for ESP32, gpio2 or 3 is recommended for ESP8266 (gpio2/3 are labeled D4/RX on NodeMCU and Wemos)
// #endif
// #define PIXELMETHOD NeoEsp8266Dma800KbpsMethod
// #define PIXELFEATURE3 NeoGrbFeature

// #include <NeoPixelBrightnessBus.h>

// enum NeoPixelType
// {
//   NeoPixelType_None = 0,
//   NeoPixelType_Grb  = 1,
//   NeoPixelType_Grbw = 2,
//   NeoPixelType_End  = 3
// };

// class NeoPixelWrapper
// {
// public:
//   NeoPixelWrapper() :
//     _pGrb(NULL)
//   {

//   }

//   ~NeoPixelWrapper()
//   {
//     cleanup();
//   }

//   void Begin(NeoPixelType type, uint16_t countPixels)
//   {
//     cleanup();
    
//     // _pGrb = new NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD>(countPixels, LEDPIN);
//     // _pGrb->Begin();

//   }

//   void Show()
//   {
//     // pCONT_lAni->stripbus->Show();
//     // _pGrb->Show();
//     pCONT_lAni;
//   }

//   void SetPixelColor(uint16_t indexPixel, RgbwColor color)
//   {
//     _pGrb->SetPixelColor(indexPixel, RgbColor(color.R,color.G,color.B));
//   }

//   void SetBrightness(byte b)
//   {
//     _pGrb->SetBrightness(b);
//   }

//   // NOTE:  Due to feature differences, some support RGBW but the method name
//   // here needs to be unique, thus GetPixeColorRgbw
//   RgbwColor GetPixelColorRgbw(uint16_t indexPixel) const
//   {
//     return _pGrb->GetPixelColor(indexPixel);
//   }

//   uint8_t* GetPixels(void)
//   {
//     return _pGrb->Pixels();  
//   }


// private:

//   NeoPixelBrightnessBus<PIXELFEATURE3,PIXELMETHOD>*  _pGrb;

//   void cleanup()
//   {
//     delete _pGrb ; 
//     _pGrb  = NULL; 
//   }

// };
// #endif
