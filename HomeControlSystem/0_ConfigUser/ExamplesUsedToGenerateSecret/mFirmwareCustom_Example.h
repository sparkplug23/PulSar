#ifndef MFIRMWARECUSTOM_EXAMPLE_HEADER
#define MFIRMWARECUSTOM_EXAMPLE_HEADER

// only hardware level defines

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
 * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
 * This can also include lights or displays as notification/information systems. 
 * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
 * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#ifdef ESP8266
#include <avr/pgmspace.h>
#endif // ESP8266
#ifdef ESP32
#define PROGMEM
#endif
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "6_Lights/_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"


/************************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ****** CUSTOM USERS - For Deployment by others ************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************/

/**
 * Uncomment (by removing the double backslash `\\` for ONE line only) to select the active example
 * */
// #define DEVICE_EXAMPLE_RGB_ADDRESSABLE
// #define DEVICE_EXAMPLE_H801


/************************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ****** Configuring ************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************/

/**
 * Example: For WS28xx Addressable lights
 * */
#ifdef DEVICE_EXAMPLE_RGB_ADDRESSABLE
  #define DEVICENAME_CTR            "rgbcustomuser01"
  #define DEVICENAME_FRIENDLY_CTR   "String Lights 01"

  #define FORCE_TEMPLATE_LOADING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define STRIP_SIZE_MAX 50

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":5000,"
    "\"ColourPalette\":\"" "Custom User 01" "\","
    "\"BrightnessRGB\":100"
  "}";
  #define STRIP_SIZE_MAX 60
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
    "},"
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":25,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";

#endif


/**
 * Example: For PWM H801 Lights
 * */
#ifdef DEVICE_EXAMPLE_H801
  #define DEVICENAME_CTR            "rgbcustomuser02"
  #define DEVICENAME_FRIENDLY_CTR   "PWM Lights 02"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1   

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1 // PWM type, set size to 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
    "},"
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME "\":1,"
      "\"" D_JSON_RATE "\":5,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":25,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";
          
#endif




#endif // MFIRMWARECUSTOM_EXAMPLE_HEADER