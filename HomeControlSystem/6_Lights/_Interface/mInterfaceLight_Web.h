#ifndef _DRIVER_INTERFACE_LIGHTS_WEB_H
#define _DRIVER_INTERFACE_LIGHTS_WEB_H 0.1

#ifdef USE_MODULE_LIGHTS_INTERFACE

// no class, include within header class

int test = 0;

void WebPage_Root_AddHandlers();
void GetPixelColor(uint16_t indexPixel, uint8_t* _r,uint8_t* _g, uint8_t* _b,uint8_t* _w1, uint8_t* _w2);
void WebSend_JSON_RootPage_LiveviewPixels(AsyncWebServerRequest *request);


void WebAppend_Root_Sliders();

void WebAppend_Root_ControlUI();

#endif
#endif