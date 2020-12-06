#ifndef _DRIVER_INTERFACE_LIGHTS_WEB_H
#define _DRIVER_INTERFACE_LIGHTS_WEB_H 0.1

#ifdef USE_MODULE_LIGHTS_INTERFACE

  #ifdef USE_MODULE_CORE_WEBSERVER

// no class, include within header class

// int test = 0;


#ifdef ESP32
#include <WiFi.h>
#ifndef DISABLE_NETWORK
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif // DISABLE_NETWORK
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif



void WebPage_Root_AddHandlers();
void GetPixelColor(uint16_t indexPixel, uint8_t* _r,uint8_t* _g, uint8_t* _b,uint8_t* _w1, uint8_t* _w2);
void WebSend_JSON_RootPage_LiveviewPixels(AsyncWebServerRequest *request);


void WebAppend_Root_Sliders();

void WebAppend_Root_ControlUI();


void WebAppend_Root_Draw_RGBPalette();
void WebAppend_Root_Draw_PaletteSelect();
void WebAppend_Root_Draw_PaletteSelect_Placeholder();

void Web_Root_Draw_PaletteSelect(AsyncWebServerRequest *request);

void WebAppend_Root_RGBPalette();

#endif //   #ifdef USE_MODULE_CORE_WEBSERVER

#endif
#endif