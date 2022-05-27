// #defines for my credentials and passwards to NOT sync to github.
// This file will override those found in "mSystemConfig"
#ifndef mUserConfigSecret_Example_H
#define mUserConfigSecret_Example_H

#include "2_CoreSystem/mGlobalMacros.h"
// #include "6_Lights/00_Interface/palettes.h"
#ifdef ESP8266
#include <avr/pgmspace.h>
#endif // ESP8266
#ifdef ESP32
#define PROGMEM // is nothing, as "const" is enough in esp32 to push to progmem space
#endif
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"



// Wifi

#undef  STA_SSID1
#define STA_SSID1            "**********"                // [Ssid1] Wifi SSID
#undef  STA_PASS1
#define STA_PASS1            "**********"                // [Password1] Wifi password

#undef  STA_SSID2
#define STA_SSID2            "**********"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS2
#define STA_PASS2            "**********"                // [Password2] Optional alternate AP Wifi password

#undef  STA_SSID3
#define STA_SSID3            "**********"                // [Ssid2] Optional alternate AP Wifi SSID
#undef  STA_PASS3
#define STA_PASS3            "**********"                // [Password2] Optional alternate AP Wifi password

// MQTT

// eg. `192,168,1,65`
#define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED       192,168,1,65

// Belfast
#define LATITUDE               54.5973         // [Latitude] Your location to be used with sunrise and sunset
#define LONGITUDE              5.9301          // [Longitude] Your location to be used with sunrise and sunset


/************************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ****** CUSTOM USERS - For Deployment by others for testing ************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************/

/**
 * Uncomment (by removing the double backslash `\\` for ONE line only) to select the active example
 * */
// #define DEVICE_CUSTOM_USER_01
// #define DEVICE_CUSTOM_USER_02


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
#ifdef DEVICE_CUSTOM_USER_01
  #define DEVICENAME_CTR            "rgbcustomuser01"
  #define DEVICENAME_FRIENDLY_CTR   "String Lights 01"

  //#define FORCE_TEMPLATE_LOADING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

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

#endif


/**
 * Example: For PWM H801 Lights
 * */
#ifdef DEVICE_CUSTOM_USER_02
  #define DEVICENAME_CTR            "rgbcustomuser02"
  #define DEVICENAME_FRIENDLY_CTR   "PWM Lights 02"
  
  #define USE_SERIAL_ALTERNATE_TX
  //#define FORCE_TEMPLATE_LOADING 

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
      
#endif



#endif // mUserConfigSecret_Example