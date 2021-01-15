#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_H

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
 * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
 * This can also include lights or displays as notification/information systems. 
 * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
 * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/mUserOptions.h"
// #include "6_Lights/_Interface/palettes.h"
#ifdef ESP8266
#include <avr/pgmspace.h>
#endif // ESP8266
#ifdef ESP32
#define PROGMEM // is nothing, as "const" is enough in esp32 to push to progmem space
#endif
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/mHardwareTemplates.h"

//--------------------------------[Global configs used when testing]-------------------------------------

#define DISABLE_WEBSERVER

//--------------------------------[Enable Device]-------------------------------------

/**
 *  LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- 
**/
// #define DEVICE_RGBROOF                                
// #define DEVICE_RGBDELL
// #define DEVICE_RGBCRYSTAL1
//#define DEVICE_RGBCRYSTAL2
// #define DEVICE_RGBSHELF
// #define DEVICE_RGBMICRO1 //glass box
// #define DEVICE_RGBMICRO2 //projector                  
//#define DEVICE_RGBMICRO3 //bedroom string esp01
//#define DEVICE_RGBMICRO4 //gazebo
#define DEVICE_RGBBEDLIGHT                            
//#define DEVICE_RGBBEDLIGHT_TEST                            
// #define DEVICE_RGBDESK
//#define DEVICE_RGBCOOKER
//#define DEVICE_RGBUTILITY
// #define DEVICE_RGBFRIDGE              
// #define DEVICE_RGBBEDROOMFLOOR
// #define DEVICE_H801_TESTER
// #define DEVICE_H801_TESTER_NODEMCU
//#define DEVICE_RGBCUSTOM_USER_01
// #define DEVICE_TESTER_RGBW

/**
 *  CHRISTMAS -- CHRISTMAS -- CHRISTMAS -- CHRISTMAS -- CHRISTMAS -- CHRISTMAS -- CHRISTMAS -- CHRISTMAS -- CHRISTMAS -- 
**/

// #define DEVICE_RGBOUTSIDETREE   
// #define DEVICE_RGBBACKDOORWREATH
// #define DEVICE_RGBOUTSIDETREE_TESTER                         
// #define DEVICE_RGBHALLWAYTREE                            
// #define DEVICE_RGBDININGROOMTREE    
// #define DEVICE_RGBSHELFTOP1        
// #define DEVICE_RGBSHELFTOP2 // church biggest        
// #define DEVICE_RGBSHELFTOP3 // church smalle     

/**
 *  CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- 
**/
// #define DEVICE_GARAGELIGHT
//#define DEVICE_GARAGELIGHT2
// #define DEVICE_SIDEDOORLIGHT
// #define DEVICE_SIDEDOORLIGHT_TEST
// #define DEVICE_SHELLY1_TESTER
//#define DEVICE_RADIATORFAN
//#define DEVICE_BEDROOMBLINDS
// #define DEVICE_DOORBELLWALLCHIME                 
// #define DEVICE_OILFURNACE
//#define DEVICE_GAZEBCON
// #define DEVICE_HEATING
//#define DEVICE_KITCHENPANEL
// #define DEVICE_LANDINGPANEL
//#define DEVICE_BLACKDOORBELL
// #define DEVICE_BEDROOM_CEILINGFAN
//#define DEVICE_FLOORFAN1
// #define DEVICE_DESKFAN
// #define DEVICE_CAM_TESTER

/**
 *  ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- 
**/
//#define DEVICE_CONSUMERUNIT 
//#define DEVICE_PZEM_TESTER

/**
 *  SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- 
**/
// #define DEVICE_ATTICSENSOR
// #define DEVICE_BEDROOMSENSOR
// #define DEVICE_MASTERBEDROOMSENSOR
// #define DEVICE_IMMERSIONSENSOR
// #define DEVICE_KITCHENSENSOR
// #define DEVICE_UTILITYSENSOR
//#define DEVICE_LIVINGROOMSENSOR
// #define DEVICE_ENSUITESENSOR
// #define DEVICE_TESTSENSOR
// #define DEVICE_NODEMCU_TESTER

/**
 *  SONOFF and lighting  -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
**/
//#define DEVICE_SONOFFTESTER
//#define DEVICE_DRESSERLIGHT
//#define DEVICE_BEDSIDE_LAMP1
//#define DEVICE_CANDLEWARMER
//#define DEVICE_EXTENTION1
//#define DEVICE_SILVERLAMP1
//#define DEVICE_SILVERLAMP2
//#define DEVICE_HALLWAYMIRROR
// #define DEVICE_SOCKET_NUMBERED

/**
 *  SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- 
**/
//#define DEVICE_KITCHENLIGHT1
//#define DEVICE_KITCHENLIGHT2
//#define DEVICE_KITCHENLIGHT3
//#define DEVICE_KITCHENLIGHT4
//#define DEVICE_KITCHENLIGHT5 //tester



//-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------

#ifdef DEVICE_CONSUMERUNIT
  #define DEVICENAME_CTR          "consumerunit"
  #define DEVICENAME_FRIENDLY_CTR "Consumer Unit"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_BUILD_TYPE_ENERGY
  #define USE_MODULE_SENSORS_PZEM004T_MODBUS

  #define USE_MODULE_SENSORS_BME
  #define D_DEVICE_SENSOR_CLIMATE "Downstairs Toilet"

  // #define ENABLE_BUG_TRACING
  
  #define USE_SOFTWARE_SERIAL_DEBUG
  #define DISABLE_SERIAL_LOGGING //temp measure

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      "\"3\":\""  D_GPIO_FUNCTION_PZEM016_RX_CTR "\"," 
      "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Cooker"
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Tumble Dryer"
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "Washing Machine"
  #define D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "Dish Washer"
  #define D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "Shower"
  #define D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "Sockets"
  #define D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "Kitchen Sockets"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_4_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_5_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_6_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_7_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "]"
    "}"
  "}";
#endif


#ifdef DEVICE_PZEM_TESTER
  #define DEVICENAME_CTR          "pzem_tester"
  #define DEVICENAME_FRIENDLY_CTR "PZEM Tester"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_BUILD_TYPE_ENERGY
  #define USE_MODULE_SENSORS_PZEM004T_MODBUS
  #define MAX_ENERGY_SENSORS 2

  // #define ENABLE_BUG_TRACING
  
  #define USE_SOFTWARE_SERIAL_DEBUG
  #define DISABLE_SERIAL_LOGGING //temp measure

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      "\"3\":\""  D_GPIO_FUNCTION_PZEM016_RX_CTR "\"," 
      "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Closed CT 100A"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Open CT 100A"
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Inline 100ohm 10A"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\""
      "]"
    "}"
  "}";
#endif



//-----------------[User Defined Devices == USE_BUILD_TYPE_LIGHTING == RGB Lighting] ----------------------------


#ifdef DEVICE_CAM_TESTER 
  #define DEVICENAME_CTR          "cam_tester"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define NO_GLOBAL_MDNS

  #define ENABLE_DEVFEATURE_JSONPARSER
  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  // #define ENABLE_PIXEL_FUNCTION_EFFECTS

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_SENSORS_INA219
  // #define USE_MODULE_SENSORS_BME

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  //#define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  //#define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   
  #define ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW    "v79.31.22+" 
  #define ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      #if defined(USE_MODULE_SENSORS_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE)   
//  "{\"Na\":\"rgb\",\"FNa\":\"fname\",\"GC\":{\"pin1\":\"func1\",\"pin2\":\"func2\"}}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"" D_PIXEL_HARDWARE_COLOR_ORDER_GRB_CTR "\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":43,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_//Scene "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB "\":[120,100,0]" "}," //this set the brightness
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";

  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_INA219_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "]"
    "}"
  "}";

#endif





#ifdef DEVICE_RGBROOF
  #define DEVICENAME_CTR            "rgbroof"
  #define DEVICENAME_FRIENDLY_CTR   "Bedroom String Lights"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  #define USE_MODULE_CONTROLLER_STATUS_LEDS
  #define DISABLE_WEBSERVER
  
// RAM:   [=====     ]  50.6% (used 41448 bytes from 81920 bytes)
// Flash: [=====     ]  50.7% (used 519416 bytes from 1023984 bytes)

  //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_ANALOG

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    
      // "\"D2\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_SWT2_CTR  "\","

      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define STRIP_SIZE_MAX 100

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
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":10000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"ColourPalette\":\"Christmas MultiColoured Warmer\","
    "\"BrightnessRGB\":100"
  "}";

  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" "Switch1" "\","
        "\"" "Switch2" "\","
        "\"" "Switch3" "\","
        "\"" "Switch4" "\","
        "\"" "Switch5" "\""
      "]" 
    "}"
  "}";


#endif


#ifdef DEVICE_RGBCUSTOM_USER_01
  #define DEVICENAME_CTR            "rgbcustomuser01"
  #define DEVICENAME_FRIENDLY_CTR   "Micro String Lights 01"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define DISABLE_WEBSERVER
  #define DISABLE_NETWORK

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR

  // Advanced options  
  //#define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  // #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
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



#ifdef DEVICE_TESTER_RGBW
  #define DEVICENAME_CTR            "tester_rgbw"
  #define DEVICENAME_FRIENDLY_CTR   "Tester RGBW WS2812 Lights"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define USE_SK6812_METHOD_DEFAULT

  // Advanced options  
  //#define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  // #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_//Scene  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":0,"
    "\"ColourPalette\":\"" "User 01" "\","
    "\"BrightnessRGB\":100"
  "}";

#endif

#ifdef DEVICE_RGBDELL
  #define DEVICENAME_CTR            "rgbdell"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Dell"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix

  #define ENABLE_PIXEL_FUNCTION_AMBILIGHT
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  //ADALIGHT_ViaSerialAndWifi
  //SCREENEDGES


  #define STRIP_SIZE_MAX 133
  
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
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_//Scene  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":0,"
    "\"ColourPalette\":\"" "Custom User 01" "\","
    "\"BrightnessRGB\":100"
  "}";

#endif

#ifdef DEVICE_RGBDESK
  #define DEVICENAME_CTR            "rgbdesk"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Under Desk"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  // #define USE_MODULE_SENSORS_MOTION

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
        #ifdef USE_MODULE_SENSORS_MOTION
        "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
        #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 60
  
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
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":0,"
    "\"ColourPalette\":\"" "Custom User 01" "\","
    "\"BrightnessRGB\":0"
  "}";
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Radar"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif


#ifdef DEVICE_RGBMICRO1
  #define DEVICENAME_CTR          "rgbmicro1"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights - Xmas"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 11
   
  // #define ENABLE_PIXEL_FUNCTION_EFFECTS
  // #define DISABLE_WEBSERVER
   
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
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":5000,"
    "\"ColourPalette\":19," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";

#endif

#ifdef DEVICE_RGBMICRO2 //projector
  #define DEVICENAME_CTR          "rgbmicro2"
  #define DEVICENAME_FRIENDLY_CTR "Projector Micro Lights"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define NO_GLOBAL_MDNS

  #define ENABLE_DEVFEATURE_JSONPARSER
  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  // #define ENABLE_PIXEL_FUNCTION_EFFECTS

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_SENSORS_INA219
  // #define USE_MODULE_SENSORS_BME

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  //#define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  //#define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   
  #define ENABLE_DEVFEATURE_PIXEL_LIVEVIEW_IN_PAGE_ROW    "v79.31.22+" 
  #define ENABLE_DEVFEATURE_PIXEL_NEW_JSON_PALETTE_SELECT_ROOT

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      #if defined(USE_MODULE_SENSORS_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE)   
//  "{\"Na\":\"rgb\",\"FNa\":\"fname\",\"GC\":{\"pin1\":\"func1\",\"pin2\":\"func2\"}}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"" D_PIXEL_HARDWARE_COLOR_ORDER_RGB_CTR "\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":43,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_//Scene "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB "\":[120,100,0]" "}," //this set the brightness
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";

  #define D_DEVICE_SENSOR_CURRENT "Module 1"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_INA219_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_RGBMICRO3 //room speaker test
  #define DEVICENAME_CTR          "rgbmicro3"
  #define DEVICENAME_FRIENDLY_CTR "Micro Lights 3"  
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 3
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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
    
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_RGB "\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":30,\"" D_JSON_RATE "\":60,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"User 18\","
    "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_BRIGHTNESS     "\":60"
  "}";
#endif


#ifdef DEVICE_RGBMICRO4 //gazebo
  #define DEVICENAME_CTR          "rgbmicro4"
  #define DEVICENAME_FRIENDLY_CTR "RGB Micro LED Lights 4"

// #define ENABLE_BUG_TRACING
// #define ENABLE_DEVFEATURE_FLICKER_TESTING
// #define ENABLE_DEVFEATURE_FLICKERING_TEST2
// #define ENABLE_DEVFEATURE_FLICKERING_TEST3
// #define ENABLE_DEVFEATURE_FLICKERING_TEST5


/*


  #ifdef ENABLE_DEVFEATURE_FLICKER_TESTING

  #endif // ENABLE_DEVFEATURE_FLICKER_TESTING

  */

#define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)
{  // trans method, flasher, colours,   rate/time, mutlipler
  "Static/InOrder, SlowGlow, Few Colour, %d/%d, Single" "|" // 1
  "Medium/Random, SlowGlow, Few Colour, %d/%d, Single" "|" // 2
  "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Single" "|" // 3
  "Slow/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 4
  "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 5
  "Static/InOrder, SlowGlow, Few Colour 9, %d/%d, Rows" "|" // 6
  "Rotate/InOrder, Sequential, Many Colour 10, %d/%d, Rows" "|" // 7
  "Static/InOrder, SlowGlow, Berry Green, %d/%d, Rows" "|" // 8
  "Static/InOrder, SlowGlow, Berry Green, %d/%d, Single" "|" // 9
  "Fast/InOrder, TwinkleColour, Many Colour 10, %d/%d, Single" "|" // 10
  "Slow/Random, SlowGlow, Partial Colours 5, %d/%d, Single" "|" // 11
  "Slow/Random, SlowGlow, Partial Colours 5, %d/%d, Single" "|" // 11
};



#define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
#define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

  #define STRIP_SIZE_MAX 50// 750   *15 //changing gazebo to be 12v

  #define DISABLE_WEBSERVER
  #define DISABLE_NETWORK
  #define DISABLE_FOR_FAULTY_ESP32_FLICKERING


  // Memory reduction techniques
  // #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 10 
  // #define WEB_LOG_SIZE 1
  // #define LOG_BUFFER_SIZE 500

  // #define DISABLE_SLEEP

  // #define USE_WS28XX_METHOD_RMT0_800KBPS_ESP32

  // #define STRIP_REPEAT_OUTPUT_MAX STRIP_SIZE_MAX
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION



  #define USE_INTERFACE_NEW
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define DISBALE_TEST_SECTION

  // #define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  // #define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"User 00\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB    "\":[15,95,0]},"
    // "\"" D_JSON_BRIGHTNESS       "\":0,"
    // "\"" D_JSON_BRIGHTNESS_RGB          "\":0,"

    // new commands I want to run during boot

    "\"AnimationMode\":\"Flasher\","
    // "\"Flasher\":{"
    //   "\"Function\":11,"
    //   "\"AgedColouring\":0"
    // "},"
    "\"Mixer\":{"
      "\"Enabled\":1,"
      // "\"" D_JSON_RUNNING_ID "\":7,"
      "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":10"
    "},"
    "\"PixelGrouping\":{"
      // "\"Enabled\":\"Off\","
      "\"Mode\":3"
    "},"
    "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":100},"
    "\"BrightnessRGB\":100"

  "}";
/*
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  #define USE_MODULE_LIGHTS_ANIMATOR

  // #define ENABLE_BUG_TRACING
  // #define USE_DEBUG_LINE

  #define STRIP_SIZE_MAX 50

  // #define USE_MODULE_SENSORS_MOTION
  // #define MOTIONALERT_PAYLOAD1_CTR "Doppler"
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_INTERFACE
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif  
      // "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","    
      // "\"D4\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  // Need to add protection to not load templates if invalid

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"User 01\","
    // "\"" D_JSON_ANIMATIONMODE             "\":\"" D_JSON_//Scene "\","
    "\"" D_JSON_ANIMATIONMODE             "\":\"" "Flasher" "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB "\":[15,90,50]" "},"
    "\"" D_JSON_BRIGHTNESS       "\":0"
  "}";
*/
#endif


//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBOUTSIDETREE
  #define DEVICENAME_CTR          "rgboutsidetree"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree"

  //#define ESP32

  /*
  
  use pin held low to disable wifi if possible, so I can test wifi things
  make it if low, enable wifi

at minimum, I can swap two esp32w around, one with and without wifi, as they wont cause neetwork name issues


add new function, that sets output via progmem stored directly


add function that enables ,qtt control per pixel, which uses setpixel (no naimator) to control ouptu
use to work out rows, but also write jay and evans names, then sore htem in progmem


using adc and ldr, enable "sweep" that lights each pixel until it stops on the one the ldr is against, and have that pixel count print over serial every second
have methods : write one, and stop on ldr
with mqqt flag, write then light all subsequent pixels found
with another, deductive method, turn off pixels found

in all cases, in array form, print the pixels lit over serial every seround and over mqtt!!

use car rad test push buttons, try adding ldr to it
use black etherhetn czble in dads rooms for it
  
  */

  #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  #define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
  DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)  
  {  // movement, order, flasher, colours,   rate/time, mutlipler, (One look explanation for design)
    // Single basic
    "%d, Stat, InOrdr, SlowGlow, C 09 (basic), Single, %d/%d, %d \n\r(Simple Static)" "|" // 1
    "%d, Medm, Random, SlowGlow, C 09 (basic), Single, %d/%d, %d, \n\r(Random Blending with same colour)" "|" // 2
    "%d, Fast, Random, SlowGlow, C 10 (many),  Single, %d/%d, %d, \n\r(Fast random instant switching, more colours)" "|" // 3
    // Rows
    "%d, Slow, Random, SlowGlow, C 11 (many),  Rows, %d/%d, %d, \n\r(Rows, slow random blending)" "|" // 4
    "%d, Fast, Random, SlowGlow, C 11 (many),  Rows, %d/%d, %d, \n\r(Rows, fast random blending)" "|" // 5
    "%d, Stat, InOrdr, SlowGlow, C 09 (basic), Rows, %d/%d, %d, \n\r(Rows, Static InOrder)" "|" // 6  
    "%d, Slow, InOrdr, Sequentl, C 09 (basic), Rows, %d/%d, %d, \n\r(Rows, Now rotating)" "|" // 7
    // Berry green
    // "%d, Stat, InOrdr, SlowGlow, Berry Green,  Rows, %d/%d, %d, \n\r(Rows, Berry green static)" "|" // 8
    // "%d, Stat, InOrdr, SlowGlow, Berry Green,  Single, %d/%d, %d, \n\r(Red every 4th pixel)" "|" // 9
    // "%d, Fast, InOrdr, TwinkleC, C 10 (many),  Single, %d/%d, %d, \n\r(Simple Static)" "|" // 10
    // Twinkle
    "%d, Slow, Random, SlowGlow, C 10 (many),  Single, %d/%d, %d, \n\r(Simple Static)" "|" // 11
    // Step through palette
    "%d, Slow, Random, SlowGlow, C 13 (g adj), Single, %d/%d, %d, \n\r(2 colour stepping)" "|" // 12
    "%d, Slow, Random, SlowGlow, C 12 (hall),  Single, %d/%d, %d, \n\r(2 colour stepping)" "|" // 13
    // Random single, with dark colours as most
    "%d, Slow, Random, SlowGlow, C 14 (most dark), Single, %d/%d, %d, \n\r(Single, fast random)" "|" // 14
    // Returning to normal, but new hallway tree colours
    "%d, Slow, Random, SlowGlow, C 12 (Hallway), Single, %d/%d, %d, \n\r(Simple Static)" "|" // 15
  };


  #define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  // #define DEBUG_PRINT_FUNCTION_NAME

//  #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  #define DISABLE_WEBSERVER 
  
  #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS
  //#define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  #define STRIP_SIZE_MAX 1050// 750   *15 //changing gazebo to be 12v

  #define DISABLE_NETWORK
  #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define DISABLE_SLEEP

  // #define USE_WS28XX_METHOD_RMT0_800KBPS_ESP32
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  // #define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  // #define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":43,"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    // "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB    "\":[15,95,0]},"
    // "\"" D_JSON_BRIGHTNESS       "\":0,"
    // "\"" D_JSON_BRIGHTNESS_RGB          "\":0,"

    // new commands I want to run during boot

    "\"AnimationMode\":\"Flasher\","
    // "\"Flasher\":{"
    //   "\"Function\":11,"
    //   "\"AgedColouring\":0"
    // "},"
    "\"Mixer\":{"
      "\"Enabled\":1,"
      // "\"" D_JSON_RUNNING_ID "\":7,"
      "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":100"
    "},"
    "\"PixelGrouping\":{"
      // "\"Enabled\":\"Off\","
      "\"Mode\":3"
    "},"
    "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":100},"
    "\"BrightnessRGB\":20"

  "}";


#endif


//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBOUTSIDETREE_TESTER
  #define DEVICENAME_CTR          "rgboutsidetree_tester"
  #define DEVICENAME_FRIENDLY_CTR "Outside Tree Tester"

// #define ENABLE_BUG_TRACING
// #define ENABLE_DEVFEATURE_FLICKER_TESTING
// #define ENABLE_DEVFEATURE_FLICKERING_TEST2
// #define ENABLE_DEVFEATURE_FLICKERING_TEST3
#define ENABLE_DEVFEATURE_FLICKERING_TEST5


/*


  #ifdef ENABLE_DEVFEATURE_FLICKER_TESTING

  #endif // ENABLE_DEVFEATURE_FLICKER_TESTING

  */

#define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)
{  // trans method, flasher, colours,   rate/time, mutlipler
  "Static/InOrder, SlowGlow, Few Colour, %d/%d, Single" "|" // 1
  "Medium/Random, SlowGlow, Few Colour, %d/%d, Single" "|" // 2
  "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Single" "|" // 3
  "Slow/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 4
  "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 5
  "Static/InOrder, SlowGlow, Few Colour 9, %d/%d, Rows" "|" // 6
  "Rotate/InOrder, Sequential, Many Colour 10, %d/%d, Rows" "|" // 7
  "Static/InOrder, SlowGlow, Berry Green, %d/%d, Rows" "|" // 8
  "Static/InOrder, SlowGlow, Berry Green, %d/%d, Single" "|" // 9
  "Fast/InOrder, TwinkleColour, Many Colour 10, %d/%d, Single" "|" // 10
  "Slow/Random, SlowGlow, Partial Colours 5, %d/%d, Single" "|" // 11
};



#define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
#define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

  #define STRIP_SIZE_MAX 1000// 750   *15 //changing gazebo to be 12v

  #define DISABLE_WEBSERVER
  #define DISABLE_NETWORK
  #define DISABLE_FOR_FAULTY_ESP32_FLICKERING


  // Memory reduction techniques
  // #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 10 
  // #define WEB_LOG_SIZE 1
  // #define LOG_BUFFER_SIZE 500

  // #define DISABLE_SLEEP

  // #define USE_WS28XX_METHOD_RMT0_800KBPS_ESP32

  // #define STRIP_REPEAT_OUTPUT_MAX STRIP_SIZE_MAX
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION



  #define USE_INTERFACE_NEW
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define DISBALE_TEST_SECTION

  // #define ENABLE_DEVFEATURE_LIGHTING_//Scene_OBJECT_TO_STRUCT "v78.24.11+" //only remove when all device exceed this
  // #define ENABLE_DEVFEATURE_RGBCOLOR_DESIRED
  //#define ENABLE_DEVFEATURE_SINGLE_ANIMATOR_INTERFACE   "v79.31.22+"   

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"User 00\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_//Scene_COLOUR     "\":{\"" D_JSON_HSB    "\":[15,95,0]},"
    // "\"" D_JSON_BRIGHTNESS       "\":0,"
    // "\"" D_JSON_BRIGHTNESS_RGB          "\":0,"

    // new commands I want to run during boot

    "\"AnimationMode\":\"Flasher\","
    // "\"Flasher\":{"
    //   "\"Function\":11,"
    //   "\"AgedColouring\":0"
    // "},"
    "\"Mixer\":{"
      "\"Enabled\":1,"
      // "\"" D_JSON_RUNNING_ID "\":7,"
      "\"" D_JSON_TIME_SCALER_AS_PERCENTAGE "\":10"
    "},"
    "\"PixelGrouping\":{"
      // "\"Enabled\":\"Off\","
      "\"Mode\":3"
    "},"
    "\"Transition\":{\"Order\":\"Random\",\"PixelUpdatePerc\":100},"
    "\"BrightnessRGB\":100"

  "}";

#endif


#ifdef DEVICE_RGBDININGROOMTREE 
  #define DEVICENAME_CTR          "rgbdiningroomtree"
  #define DEVICENAME_FRIENDLY_CTR "RGB Dining Room Tree"

 #define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME

  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

  #define STRIP_SIZE_MAX 150

  #define DISABLE_WEBSERVER
  // #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      #if defined(USE_MODULE_SENSORS_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

   #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":30,\"RateMs\":2000},"
    "\"TimeMs\":1000,"
    "\"ColourPalette\":43," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";

#endif



//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_RGBBACKDOORWREATH
  #define DEVICENAME_CTR          "rgbbackdoorwreath"
  #define DEVICENAME_FRIENDLY_CTR "RGB Backdoor Wreath"
  
  // #define ENABLE_DEVFEATURE_FLICKERING_TEST5

  // #define USE_PM_OUTSIDE_TREE_MIXER_DESCRIPTION
  // DEFINE_PGM_CTR(PM_OUTSIDE_TREE_MIXER_DESCRIPTION)
  // {  // trans method, flasher, colours,   rate/time, mutlipler
  //   "Static/InOrder, SlowGlow, Few Colour, %d/%d, Single" "|" // 1
  //   "Medium/Random, SlowGlow, Few Colour, %d/%d, Single" "|" // 2
  //   "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Single" "|" // 3
  //   "Slow/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 4
  //   "Fast/Random, SlowGlow, Many Colour 10, %d/%d, Rows" "|" // 5
  //   "Static/InOrder, SlowGlow, Few Colour 9, %d/%d, Rows" "|" // 6
  //   "Rotate/InOrder, Sequential, Many Colour 10, %d/%d, Rows" "|" // 7
  //   "Static/InOrder, SlowGlow, Berry Green, %d/%d, Rows" "|" // 8
  //   "Static/InOrder, SlowGlow, Berry Green, %d/%d, Single" "|" // 9
  //   "Fast/InOrder, TwinkleColour, Many Colour 10, %d/%d, Single" "|" // 10
  //   "Slow/Random, SlowGlow, Partial Colours 5, %d/%d, Single" "|" // 11
  // };

/*
RAM:   [======    ]  56.2% (used 46024 bytes from 81920 bytes)
Flash: [======    ]  56.9% (used 582400 bytes from 1023984 bytes)*/
  #define ENABLE_DEVFEATURE_SETPIXELOUTPUT_VARIABLE
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

  #define STRIP_SIZE_MAX 100

  // #define DISABLE_WEBSERVER
  // #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

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
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"User 00\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":30,\"RateMs\":2000},"
    "\"TimeMs\":1000,"
    "\"ColourPalette\": 43,"
    "\"BrightnessRGB\":100"
  "}";


#endif






#ifdef DEVICE_H801_TESTER // for PWM dev
  #define DEVICENAME_CTR          "h801_tester"
  #define DEVICENAME_FRIENDLY_CTR "H801 Tester Strip"
    
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIOC "\":{"
    //   "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
    //   "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    // "},"
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

#ifdef DEVICE_H801_TESTER_NODEMCU // for PWM dev // build version that uses 2 of the nodemcu leds as pwm channels
  #define DEVICENAME_CTR          "h801_tester_nodemcu"
  #define DEVICENAME_FRIENDLY_CTR "H801 Tester Strip Nodemcu"
    
  // #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_PWM
  #define ENABLE_DEVFEATURE_PHASING_//Scene_OUT

  #define USE_MODULE_LIGHTS_ANIMATOR
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"2\":\""  D_GPIO_FUNCTION_PWM1_CTR "\"," //d4
      "\"16\":\""  D_GPIO_FUNCTION_PWM2_CTR "\"," //d0
      "\"D1\":\""  D_GPIO_FUNCTION_PWM3_CTR "\","
      "\"D2\":\""  D_GPIO_FUNCTION_PWM4_CTR "\","
      "\"D6\":\""  D_GPIO_FUNCTION_PWM5_CTR "\""
    
      // "\"D2\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_SWT2_CTR  "\","

      // "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"User 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"//Scene\","
    "\"" D_JSON_BRIGHTNESS     "\":0"
  "}";

#endif


#ifdef DEVICE_RGBCOOKER
  #define DEVICENAME_CTR          "rgbcooker"
  #define DEVICENAME_FRIENDLY_CTR "RGB Cooker H801"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
  
  #define USE_SERIAL_ALTERNATE_TX

  // #define ENABLE_FUNCTION_DEBUG  
  // #define ENABLE_BUG_TRACING

  #define USE_SERIAL_ALTERNATE_TX
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define DISABLE_TEMPORARY_RGBANIMATOR
  // #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM

  // #define USE_MODULE_SENSORS_MOTION
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "DiningRoom"
  
  #define DISABLE_SLEEP

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"FLAG\":0,"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR "\","
    "\"" D_JSON_CCT_TEMP       "\":500,"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

  // #define ESP8266

  // #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "Kitchen Test"
  
  // Drivers, Sensors and lights?
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //       "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
  //         "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\""
  //       "],"
  //       "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //         "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
  //       "]"
  //   "}"
  // "}";

#endif


#ifdef DEVICE_RGBBEDLIGHT // for PWM dev
  #define DEVICENAME_CTR          "rgbbedlight"
  #define DEVICENAME_FRIENDLY_CTR "H801 BedLight"
  
  // #define ENABLE_FUNCTION_DEBUG  

  #define USE_SERIAL_ALTERNATE_TX

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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




#ifdef DEVICE_RGBBEDROOMFLOOR
  #define DEVICENAME_CTR          "rgbbedroomfloor"
  #define DEVICENAME_FRIENDLY_CTR "H801 Bedroom Floor Light"
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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



#ifdef DEVICE_RGBBEDLIGHT_TEST // for PWM dev
  #define DEVICENAME_CTR          "rgbbedlight_test"
  #define DEVICENAME_FRIENDLY_CTR "H801 BedLight TEST"
  
  // #define ENABLE_FUNCTION_DEBUG  

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_PWM
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define DISABLE_TEMPORARY_RGBANIMATOR
  // #define USE_MODULE_LIGHTS_PWM

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
      "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR "\","   //"\":\"WS2812\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"USER_17\","
      "\"" D_JSON_ANIMATIONMODE           "\":\"Flasher\","
      "\"" D_JSON_BRIGHTNESS     "\":99"
    "}";
    
    // #define ESP8266

#endif


#ifdef DEVICE_RGBCRYSTAL1
  #define DEVICENAME_CTR            "rgbcrystal1"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 10
  #define STRIP_SIZE_MAX 150
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define ENABLE_PIXEL_FUNCTION_EFFECTS

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

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  //   "{"
  //     "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //     "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
  //     "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":4,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
  //     "\"" D_JSON_COLOUR_PALETTE "\":\"User 19\"," //purple/pink/red (at sunset orange?)
  //     "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
  //     "\"" D_JSON_BRIGHTNESS     "\":10,"
  //     "\"" D_JSON_BRIGHTNESS_RGB        "\":10"
  //   "}";

    
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
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":1,\"RateMs\":60000},"
    "\"TimeMs\":30000,"
    "\"ColourPalette\":43,"//\"" "Christmas Crystal 1" "\"," //c16    43 is the colours for this christmas
    "\"BrightnessRGB\":60"
  "}";





#endif

#ifdef DEVICE_RGBCRYSTAL2
  #define DEVICENAME_CTR            "rgbcrystal2"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light Round 2"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 10
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    "{"
      "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"User 19\"," //purple/pink/red (at sunset orange?)
      "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
      "\"" D_JSON_BRIGHTNESS     "\":14"
    "}";

#endif
#ifdef DEVICE_RGBSHELF
  #define DEVICENAME_CTR          "rgbshelf"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Lights"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  
  // #define ENABLE_PIXEL_FUNCTION_EFFECTS
  #define DISABLE_WEBSERVER
   
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

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_RGB "\","
  //   "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":20,\"" D_JSON_RATE "\":120,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"User 09\","
  //   "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";
  #define STRIP_SIZE_MAX 33

 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":60000},"
    "\"TimeMs\":30000,"
    "\"ColourPalette\":25," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":70"
  "}";


#endif
#ifdef DEVICE_RGBSHELFTOP1
  #define DEVICENAME_CTR          "rgbshelftop1"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Top 1Lights"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  
  #define ENABLE_PIXEL_FUNCTION_EFFECTS
   
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

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_RGB "\","
  //   "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":20,\"" D_JSON_RATE "\":120,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"User 09\","
  //   "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";
  #define STRIP_SIZE_MAX 100

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
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":60000},"
    "\"TimeMs\":30000,"
    "\"ColourPalette\":43," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";


#endif
#ifdef DEVICE_RGBSHELFTOP2
  #define DEVICENAME_CTR          "rgbshelftop2"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Top 2 Lights - Big Church"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  
  #define ENABLE_PIXEL_FUNCTION_EFFECTS
   
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

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_RGB "\","
  //   "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":20,\"" D_JSON_RATE "\":120,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"User 09\","
  //   "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";
  #define STRIP_SIZE_MAX 25

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
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":60000},"
    "\"TimeMs\":30000,"
    "\"ColourPalette\":43," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";


#endif
#ifdef DEVICE_RGBSHELFTOP3
  #define DEVICENAME_CTR          "rgbshelftop3"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Top 3 Lights - Little Church"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define ENABLE_PIXEL_FUNCTION_EFFECTS
   
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

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_RGB "\","
  //   "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":20,\"" D_JSON_RATE "\":120,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"User 09\","
  //   "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";
  #define STRIP_SIZE_MAX 25

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
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":60000},"
    "\"TimeMs\":30000,"
    "\"ColourPalette\":25," //pastel c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";


#endif
#ifdef DEVICE_RGBNOTIF1
  #define DEVICENAME_CTR          "rgbnotif1"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 1"
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix               
  #define PIXEL_HARDWARE_COLOR_ORDER_CUSTOM_ID           UO_PIXEL_HARDWARE_COLOR_ORDER_GRB_ID    
  #define DEFAULT_LIGHTING_TRANSITION_ORDER_ID           UO_TRANSITION_ORDER_RANDOM_ID  
  #define DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID   UO_TIME_MAP_SECS_1_ID
  #define DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID   UO_RATE_MAP_SECS_1_ID
  #define DEFAULT_LIGHTING_ANIMATION_PALETTE_ID          UO_PALETTELIST_STATIC_AUTUMN_RED_ID
  #define DEFAULT_LIGHTING_ANIMATION_MODE_ID             UO_ANIMATION_MODE_EFFECTS_ID
  #define DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID           UO_EFFECTS_FUNCTION_SLOW_GLOW_ID
  #define DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID   UO_PIXELS_UPDATE_PERCENTAGE_50_ID
  #define DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE         100
#endif
#ifdef DEVICE_RGBGAZEBO
  #define DEVICENAME_CTR          "rgbgazebo"
  #define DEVICENAME_FRIENDLY_CTR "RGB Gazebo"
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define STRIP_SIZE_MAX                200               
  #define PIXEL_HARDWARE_COLOR_ORDER_CUSTOM_ID           UO_PIXEL_HARDWARE_COLOR_ORDER_RGB_ID    
  #define DEFAULT_LIGHTING_TRANSITION_ORDER_ID           UO_TRANSITION_ORDER_RANDOM_ID  
  #define DEFAULT_LIGHTING_TRANSITION_TIME_MAP_SECS_ID   UO_TIME_MAP_SECS_1_ID
  #define DEFAULT_LIGHTING_TRANSITION_RATE_MAP_SECS_ID   UO_RATE_MAP_SECS_1_ID
  #define DEFAULT_LIGHTING_ANIMATION_PALETTE_ID          UO_PALETTELIST_STATIC_AUTUMN_RED_ID
  #define DEFAULT_LIGHTING_ANIMATION_MODE_ID             UO_ANIMATION_MODE_EFFECTS_ID
  #define DEFAULT_LIGHTING_EFFECTS_FUNCTION_ID           UO_EFFECTS_FUNCTION_SLOW_GLOW_ID
  #define DEFAULT_LIGHTING_PIXELS_UPDATE_PERCENTAGE_ID   UO_PIXELS_UPDATE_PERCENTAGE_50_ID
  #define DEFAULT_LIGHTING_BRIGHTNESS_PERCENTAGE         100
#endif


//-----------------[User Defined Devices == USE_BUILD_TYPE_CUSTOM == Specific types] ----------------------------


#ifdef DEVICE_TESTBUILDER
  #define DEVICENAME_CTR          "development_device"
  #define DEVICENAME_FRIENDLY_CTR "Development Device"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER random(1,10000)
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_DOOR
  // #define USE_SENSOR_DOOR_LOCK
  // #define DOORALERT_PAYLOAD_CTR "bedroom"

  // #define USE_MODULE_CUSTOM_SONOFF_IFAN
  // #define USE_MODULE_DRIVERS_IRTRANSCEIVER

  // #define USE_MODULE_SENSORS_DS18B20

  // #define USE_MODULE_SENSORS_MOTION
  // #define MOTIONALERT_PAYLOAD1_CTR "bedroom"
  
  // #define USE_MODULE_SENSORS_BME

  // #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"D0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18B20
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      //"\"D6\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"D0\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      #endif
      "\"D5\":\"" D_GPIO_FUNCTION_FAN_IRSEND_CTR    "\","
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"D6\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_INTERFACE
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","    
      "\"D4\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)                                                  //parsed via lighting interface
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_REPEAT   "\":5,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\"," //GRBW
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"USER_18\","
    "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
  // #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  // #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  // #define STRIP_SIZE_MAX                      12   


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Attic Socket"
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
          "\"" "Attic" "\""
        "],"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif



#ifdef DEVICE_ATTICSENSOR
  #define DEVICENAME_CTR          "atticsensor"
  #define DEVICENAME_FRIENDLY_CTR "Attic Sensor"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18B20
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18B20
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      #endif
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","    
      "\"D4\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" "Attic" "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" "Cold Water Tank" "\""
      "]"
    "}"
  "}";

#endif

#ifdef DEVICE_HEATING
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "Heating"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HEATING_DEBUG_TIMES
  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_CUSTOM
  #define USE_MODULE_CUSTOM_HEATING
  
  #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DS18B20

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 4
  #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","
      "\"9\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Upstairs"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Downstairs"
  #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Immersion"
  #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "Water Boiler"

  #define D_DHT_NAME_DOWNSTAIRS "Downstairs DHT"
  #define D_DHT_NAME_UPSTAIRS "Upstairs DHT"

  #define D_DB18_NAME_DOWNSTAIRS_PIPE "Downstairs_Pipe"
  #define D_DB18_NAME_UPSTAIRS_PIPE "Upstairs_Pipe"
  #define D_DB18_NAME_BOILER_PIPE "Boiler_Pipe"
  #define D_DB18_NAME_IMMERSION_HEATER "Immersion_Heater"
  #define D_DB18_NAME_TANK_TOP "Tank_Top"
  #define D_DB18_NAME_TANK_MIDDLE "Tank_Middle"
  #define D_DB18_NAME_TANK_BOTTOM "Tank_Bottom"
  #define D_DB18_NAME_TANK_OUT "Tank_Out"

  // maybe I need to delay template name loading until after init phase?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DB18_NAME_DOWNSTAIRS_PIPE "\","
        "\"" D_DB18_NAME_UPSTAIRS_PIPE "\","
        "\"" D_DB18_NAME_BOILER_PIPE "\","
        "\"" D_DB18_NAME_IMMERSION_HEATER "\","
        "\"" D_DB18_NAME_TANK_BOTTOM "\","
        "\"" D_DB18_NAME_TANK_OUT "\","
        "\"" D_DB18_NAME_TANK_TOP "\","
        "\"" D_DB18_NAME_TANK_MIDDLE "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DHT_NAME_DOWNSTAIRS "\","
        "\"" D_DHT_NAME_UPSTAIRS "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["                
        "[40,255,152,171,193,23,4,231],"
        "[40,255,131,6,194,23,4,59],"
        "[40,255,204,226,193,23,4,30],"
        "[40,255,136,105,53,22,4,114],"
        "[40,255,50,176,193,23,4,197],"
        "[40,255,216,108,53,22,4,102],"
        "[40,255,162,167,53,22,4,27],"
        "[40,255,219,93,53,22,4,239]"
      "]"  
    "}"
  "}";
  
#endif
#ifdef DEVICE_DOORBELLWALLCHIME
  #define DEVICENAME_CTR          "doorbellwallchime"
  #define DEVICENAME_FRIENDLY_CTR "Doorbell Wall Chime"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 3
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR
  
  // #define USE_MODULE_SENSORS_DOORCHIME
  #define DOORALERT_PAYLOAD_CTR   "frontdoorbell"

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_DOORCHIME
      "\"D6\":\"" D_GPIO_FUNCTION_CHIME_INPUT_CTR     "\","
      "\"D7\":\"" D_GPIO_FUNCTION_CHIME_RINGER_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_ANIMATIONMODE           "\":\""  D_JSON_EFFECTS "\","
    // "\"" D_JSON_//Scene_COLOUR   "\":{"
    //       "\"" D_JSON_HSB    "\":[330,100,100]" 
    //     "},"
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define PIXEL_LIGHTING_HARDWARE_SK6812_STRIP

#endif
#ifdef DEVICE_RADIATORFAN
  /* Single Relay controls TWO 12V DC fans to improve heating (1 D_Out)
   * Measure PWM speed of motors (2 D_in) (Needs level shifter from 12v to 3.3v)
   * Motion sensor (1 D_in)
   * Two DS18b20 to measure inside/outside of vent area (1 D_io)
   * One DHT22 for climate sensing -- towards door (1 D_io)
   * One BME280 for improved climate sensing -- towards hallway (1 I2C) */
  #define DEVICENAME_CTR          "radiatorfan"
  #define DEVICENAME_FRIENDLY_CTR "Radiator Fan"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Hallway"

  #define USE_MODULE_SENSORS_DS18B20
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_CUSTOM_RADIATORFAN

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18B20
      "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR     "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif 
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Fan"
  #define RELAYS_CONNECTED 1

  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "Top Vent"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "Inside Vent"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "Hallway"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_OILFURNACE
  #define DEVICENAME_CTR          "oilfurnace"
  #define DEVICENAME_FRIENDLY_CTR "Oil Furnace"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  // #define DEVICE_LOCATION_OUTSIDE
   
  //#define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  // #define USE_BUILD_TYPE_CUSTOM
  // #define USE_MODULE_CUSTOM_OILFURNACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define ENABLE_SWITCH_ACTIVE_LOW
  
  // #define USE_MODULE_SENSORS_BUTTONS
  
  #define USE_MODULE_SENSORS_DS18B20
  
  // #define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  //#define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  //#define ENABLE_FORCED_SKIP_AP_ON_IPUNSET

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif    
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D3\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      #endif      
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "FurnaceInflow"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "FurnaceOutflow"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "GarageOutside"
  #define D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_5_FRIENDLY_NAME_LONG "BackUpTank"
  #define D_DEVICE_FURNACE_ACTIVE_FRIENDLY_NAME_LONG "Furnace Power"
  
  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_5_FRIENDLY_NAME_LONG "\""
      "],"
      // "\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\":["
      //   "\"" D_DEVICE_FURNACE_ACTIVE_FRIENDLY_NAME_LONG "\""
      // "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_BUTTON_1_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["                                
        "[40,170,67,3,30,19,2,25],"     // D3 group of 2                                 
        "[40,255,169,120,53,22,4,240],"
        "[40,143,81,7,51,20,1,189],"   //D6 group of 3                                           
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "}"
  "}";

#endif


#ifdef DEVICE_IMMERSIONSENSOR
  #define DEVICENAME_CTR          "immersionsensor"
  #define DEVICENAME_FRIENDLY_CTR "Immersion Sensor"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
     
  #define USE_MODULE_SENSORS_DS18B20
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "TankPosition100" //top
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "TankPosition80"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "TankPosition60"
  #define D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "TankPosition40"
  #define D_DEVICE_TEMP_5_FRIENDLY_NAME_LONG "TankPosition20"
  #define D_DEVICE_TEMP_6_FRIENDLY_NAME_LONG "TankPosition00" //bottom
  #define D_DEVICE_TEMP_7_FRIENDLY_NAME_LONG "BoilerLoopTop"
  #define D_DEVICE_TEMP_8_FRIENDLY_NAME_LONG "BoilerLoopBottom"
  #define D_DEVICE_TEMP_9_FRIENDLY_NAME_LONG "ImmersionFeedIn"
  #define D_DEVICE_TEMP_10_FRIENDLY_NAME_LONG "FeedRed" // not sure what this does, cold in it seems
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_4_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_5_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_6_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_7_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_8_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_9_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_10_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["                   
        "[40,0,32,23,59,71,5,141],"  
        "[40,0,83,19,59,71,6,66],"  
        "[40,0,108,65,59,71,4,202],"  
        "[40,0,66,109,59,71,5,172],"   
        "[40,0,114,20,59,71,5,19],"  
        "[40,0,118,128,59,71,5,227],"
        "[40,0,40,61,59,71,4,134],"
        "[40,0,66,140,59,71,6,136]," // not calib
        "[40,0,95,50,59,71,5,126]," // not calib
        "[40,0,149,87,59,71,5,240]" // not calib
      "]"  
    "},"
    "\"" D_JSON_SENSOR_MEASURERATE "\":{"
      // 1 second
    "}"
  "}";

#endif


#ifdef DEVICE_LANDINGPANEL
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Control Panel"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define USE_MODULE_SENSORS_BME
  
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) "," //12
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_ANIMATIONMODE           "\":\""  D_JSON_//Scene  "\","
    "\"" D_JSON_//Scene_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[25,90,100]" 
          // "\"" D_JSON_RGBW    "\":[0,0,0,255]" 
        "},"
        "\"AnimationMode\":\"//Scene\","
        "\"//SceneName\":\"ColourSingle\","
        "\"hue\":20,"
        "\"sat\":90,"
        "\"cct_temp\":600,"
        "\"brt_rgb\":100,"
    "\"" D_JSON_BRIGHTNESS     "\":"    "100"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define PIXEL_LIGHTING_HARDWARE_SK6812_STRIP
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif

#ifdef DEVICE_TESTSENSOR
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Control Panel"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
        #ifdef USE_MODULE_SENSORS_MOTION
        "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
        #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Radar"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

  



#endif


#ifdef DEVICE_KITCHENPANEL
  #define DEVICENAME_CTR          "kitchenpanel"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Panel"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 4 //heating page

  #define USE_BUILD_TYPE_DISPLAY

  // #define ENABLE_BUG_TRACING
  
  // #define USE_SOFTWARE_SERIAL_DEBUG
  // #define DISABLE_SERIAL_LOGGING //temp measure

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"3\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," 
      "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif


#ifdef DEVICE_BEDROOMSENSOR
  #define DEVICENAME_CTR          "bedroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Sensor"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  // #define ENABLE_BUG_TRACING
  #define USE_MODULE_SENSORS_ANALOG
  
  #define ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS
  #define ENABLE_DEVFEATURE_TESTING_LONG_LOOPS
  #define ENABLE_DEVFEATURE_RTC_TIME_V2
  
  // #define USE_MODULE_SENSORS_SWITCHES

  // #define USE_MODULE_SENSORS_DOOR
  // // #define USE_SENSOR_DOOR_LOCK
  // #define DOORALERT_PAYLOAD_CTR "bedroom"

  // #define USE_MODULE_SENSORS_DS18B20
  // #define ENABLE_DEVFEATURE_DB18_TEMPLATE_CORRECTED_INDEXES

  #define USE_MODULE_SENSORS_MOTION
  

  #define ENABLE_PIXEL_FUNCTION_EFFECTS
  // Test ultrasonic oilfurnace code
  // #define USE_BUILD_TYPE_CUSTOM
  // #define USE_MODULE_CUSTOM_OILFURNACE
  // #define USE_MODULE_SENSORS_ULTRASONICS  
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  
  // #define USE_MODULE_SENSORS_BME

  // // #define USE_MODULE_SENSORS_BUTTONS

  // // #define USE_MODULE_DRIVERS_RELAY
  // // #define RELAYS_CONNECTED 1
  // // #define USE_MODULE_DRIVERS_INTERFACE
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"D0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif  
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18B20
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      //"\"D6\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"D0\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      #endif
      // "\"D5\":\"" D_GPIO_FUNCTION_FAN_IRSEND_CTR    "\","
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"D6\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      // add status led to here
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


 #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":60000},"
    "\"TimeMs\":30000,"
    "\"ColourPalette\":43," //c12    43 is the colours for this christmas
    "\"BrightnessRGB\":100"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  // #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  #define STRIP_SIZE_MAX                      12   

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "LED1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "LED2"

// maybe I need to delay template name loading until after init phase?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" "bedroom" "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" "Bedroom" "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" "speed_of_sound_ambient" "\","
        "\"" "Bedroom02" "\","
        "\"" "Bedroom03" "\","
        "\"" "Bedroom04" "\","
        "\"" "Bedroom05" "\","
        "\"" "Bedroom06" "\","
        "\"" "Bedroom07" "\""
      "],"
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" "bedroomDOOR" "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["                                
        "[40,255,100,29,194,102,202,187],"     // D3 group of 2                                 
        "[40,255,100,29,195,135,215,193],"
        "[40,255,100,29,194,124,254,111],"   //D6 group of 3                                           
        "[40,255,100,29,195,134,175,63],"                                             
        "[40,255,100,29,195,135,126,242]"  

                            
        // "[40,0,32,23,59,71,5,141],"  
        // "[40,0,66,109,59,71,5,172],"  
        // "[40,0,118,128,59,71,5,227],"  
        // "[40,0,83,19,59,71,6,66],"  
        // "[40,0,114,20,59,71,5,19],"  
        // "[40,0,108,65,59,71,4,202],"  
        // "[40,0,40,61,59,71,4,134]"       

      "]"  
    "}"
  "}";

#endif
#ifdef DEVICE_LIVINGROOMSENSOR
  #define DEVICENAME_CTR          "livingroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Living Room Sensor"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "DriveFront"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "LivingRoom"
  #define D_DEVICE_SENSOR_MOTION_2_FRIENDLY_NAME_LONG "LivingRadar" // Test device

  #define D_DEVICE_SENSOR_DRIVEWAY_ULTRSONIC_FRIENDLY_NAME_LONG "DriveFront"

  #define USE_MODULE_SENSORS_ANALOG

  #define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  
  #define USE_MODULE_SENSORS_BME
  #define D_DEVICE_SENSOR_CLIMATE "LivingRoom"

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D8\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR     "\"," //d0 to d3
      "\"D3\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_2_INV_CTR     "\"" //END
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_ULTRASONIC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DRIVEWAY_ULTRSONIC_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        // "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif

#ifdef DEVICE_BEDROOMBLINDS
  #define DEVICENAME_CTR          "bedroomblinds"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Blinds"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_CUSTOM_BLINDS
  #define USE_MODULE_DRIVERS_HBRIDGE
  //#define USE_MODULE_SENSORS_ANALOG
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_HBRIDGE
      "\"D1\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR   "\","
      "\"D6\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR   "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR   "\","
      // "\"D7\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ANALOG
      "\"A0\":\"" D_GPIO_FUNCTION_ANALOG_POSITION_CTR "\","
      #endif
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_FRIENDLY_NAME_LONG "Blinds"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_FRIENDLY_NAME_LONG "\""
      "]"
  "}";



  // #define DEVICE_CRITICALITY_CTR  CRITICALITY_LEVEL_LOW
  // #define DEVICE_STABILITY_CTR    STABILITY_LEVEL_PRE_RELEASE
  // #define USE_MODULE_CUSTOM_BLINDS
  // #define USE_MODULE_DRIVERS_HBRIDGE
  
  //   pCONT_set->Settings.module = MODULE_NODEMCU_ID;
  //   pCONT_set->Settings.module_pins.io[3] = GPIO_RGB_DATA_ID;
#endif




#ifdef DEVICE_SIDEDOORLIGHT
  #define DEVICENAME_CTR          "sidedoorlight"
  #define DEVICENAME_FRIENDLY_CTR "Side Door Motion Light"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 3

  //#define DISABLE_WEBSERVER
   
  #define USE_MODULE_SENSORS_MOTION

  #define ENABLE_DEVFEATURE_BLOCK_RESTART
  #define ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
  #define ENABLE_DEVFEATURE_RELAY_TIME_SCHEDULED_DEFAULT_ON
  
  #define USE_MODULE_CUSTOM_SECURITY_LIGHT //disable until I rewrite for the sidelight

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\"," //d2 normally
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define RELAYS_CONNECTED 1
  
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif


#ifdef DEVICE_SIDEDOORLIGHT_TEST
  #define DEVICENAME_CTR          "sidedoorlight_test"
  #define DEVICENAME_FRIENDLY_CTR "Side Door Motion Test"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 3

  //#define DISABLE_WEBSERVER
   
  #define USE_MODULE_SENSORS_MOTION

  #define ENABLE_DEVFEATURE_BLOCK_RESTART
  #define ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
  // #define ENABLE_DEVFEATURE_RELAY_TIME_SCHEDULED_DEFAULT_ON
  
  #define USE_MODULE_CUSTOM_SECURITY_LIGHT //disable until I rewrite for the sidelight

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\"," //d2 normally
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define RELAYS_CONNECTED 1
  
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif





#ifdef DEVICE_SHELLY1_TESTER
  #define DEVICENAME_CTR          "shelly1_tester"
  #define DEVICENAME_FRIENDLY_CTR "Shelly1 Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
   
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle"

  #define 
  
  // #define USE_MODULE_CUSTOM_SECURITY_LIGHT //disable until I rewrite for the sidelight

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_NP_INV_CTR     "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define RELAYS_CONNECTED 1
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif




// #ifdef DEVICE_RGBCOOKER
//   #define DEVICENAME_CTR          "rgbcooker"
//   #define DEVICENAME_FRIENDLY_CTR "RGB Cooker"

//   #define FORCE_TEMPLATE_LOADING
//   //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE //temp fix
//   #define USE_MODULE_LIGHTS_ANIMATOR
  
//   #define USE_MODULE_SENSORS_MOTION

//   #define USE_MODULE_SENSORS_DHT

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"D1\":\"" D_GPIO_FUNCTION_DHT22_1_CTR    "\","
//       #ifdef USE_MODULE_SENSORS_MOTION
//       "\"D2\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
//       #endif
//       "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
//     "\"" D_JSON_STRIP_SIZE     "\":"    "50"          ","
//     "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
//     "\"" D_JSON_ANIMATIONMODE           "\":\""  D_JSON_//Scene  "\","
//     "\"" D_JSON_//Scene_COLOUR   "\":{"
//           "\"" D_JSON_HSB    "\":[30,80,100]" 
//         "},"
//     "\"" D_JSON_BRIGHTNESS     "\":"    "50"
//   "}";

//   #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
//   #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
//   // Drivers, Sensors and lights?
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//         "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//           "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
//         "],"
//         "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
//           "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
//         "]"
//     "}"
//   "}";

// #endif
#ifdef DEVICE_KITCHENSENSOR
  #define DEVICENAME_CTR          "kitchensensor"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Sensor"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
     
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif
#ifdef DEVICE_ENSUITESENSOR
  #define DEVICENAME_CTR          "ensuitesensor"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_SENSORS_BME

  #define DISABLE_PIXEL_FUNCTION_EFFECTS
  #define ENABLE_DEVFEATURE_DISABLE_UNTIL_RGBCCT_CONVERSION_FIXED_FOR_WHITE_CHANNELS

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ANIMATOR  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_ANIMATIONMODE           "\":\""  D_JSON_//Scene  "\","
    "\"" D_JSON_//Scene_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[15,95,0]" 
        "},"
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define PIXEL_LIGHTING_HARDWARE_SK6812_STRIP

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif
#ifdef DEVICE_MASTERBEDROOMSENSOR
  #define DEVICENAME_CTR          "masterbedroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Master Bedroom Sensor"

  #define FORCE_TEMPLATE_LOADING
     
  // #define USE_MODULE_SENSORS_BME
  
  #define USE_MODULE_SENSORS_ANALOG

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "MasterBedroom"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif

#ifdef DEVICE_NODEMCU_TESTER
  #define DEVICENAME_CTR          "nodemcu_tester"
  #define DEVICENAME_FRIENDLY_CTR "NodeMCU Tester"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
  // #define USE_MODULE_SENSORS_MOTION
  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES

  // #define USE_MODULE_SENSORS_BME

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_ANIMATOR  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      // #ifdef USE_MODULE_SENSORS_BUTTONS
      // "\"D3\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      // "\"D4\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      // #endif    
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D3\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      "\"D4\":\""  D_GPIO_FUNCTION_SWT2_CTR  "\","
      #endif 
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_ANIMATIONMODE           "\":\""  D_JSON_//Scene  "\","
    "\"" D_JSON_//Scene_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[15,95,0]" 
        "},"
    "\"" D_JSON_BRIGHTNESS       "\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB          "\":0"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define PIXEL_LIGHTING_HARDWARE_SK6812_STRIP

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"

  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"
  #define D_DEVICE_BUTTON_2_CTR "Button2"
  #define D_DEVICE_BUTTON_3_CTR "Button3"
  #define D_DEVICE_BUTTON_4_CTR "Button4"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_BUTTON_1_CTR "\","
        "\"" D_DEVICE_BUTTON_2_CTR "\","
        "\"" D_DEVICE_BUTTON_3_CTR "\","
        "\"" D_DEVICE_BUTTON_4_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_BUTTON_1_CTR "\","
        "\"" D_DEVICE_BUTTON_2_CTR "\","
        "\"" D_DEVICE_BUTTON_3_CTR "\","
        "\"" D_DEVICE_BUTTON_4_CTR "\""
      "]"
    "}"
  "}";

#endif

#ifdef DEVICE_RGBUTILITY
  #define DEVICENAME_CTR          "rgbutility"
  #define DEVICENAME_FRIENDLY_CTR "RGB Utility H801"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
  
  #define USE_SERIAL_ALTERNATE_TX
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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
    "\"FLAG\":0,"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_PIXEL_HARDWARE_TYPE_RGBCCT_PWM_CTR "\","
    "\"" D_JSON_CCT_TEMP       "\":500,"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

#endif
#ifdef DEVICE_RGBFRIDGE
  #define DEVICENAME_CTR          "rgbfridge"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fridge H801"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
  
  #define USE_SERIAL_ALTERNATE_TX
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
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
    "\"FLAG\":0,"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"User 01\","
    "\"" D_JSON_ANIMATIONMODE  "\":\"//Scene\","
    "\"" D_JSON_BRIGHTNESS     "\":0"
  "}";
#endif


#ifdef DEVICE_UTILITYSENSOR
  #define DEVICENAME_CTR          "utilitysensor"
  #define DEVICENAME_FRIENDLY_CTR "Utility Sensor"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

   #define DISABLE_WEBSERVER //memory to low for OTA
     
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    DEFINE_APP_SVALUE("NAME",DEVICENAME_CTR)
    DEFINE_APP_SVALUE("FRIENDLYNAME",DEVICENAME_FRIENDLY_CTR)
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR    "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      DEFINE_APP_SVALUE("D6",D_GPIO_FUNCTION_PIR_1_INV_CTR)
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      DEFINE_APP_SVALUE("D7",D_GPIO_FUNCTION_DOOR_OPEN_CTR)
      #endif
      DEFINE_END_SVALUE("D4",D_GPIO_FUNCTION_LEDLNK_CTR)
    "},"
    DEFINE_END_SVALUE("BASE",D_MODULE_NAME_USERMODULE_CTR)
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Utility"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Utility"
  #define D_DEVICE_SENSOR_DOOROPEN_FRIENDLY_NAME_LONG "SideDoor"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DOOROPEN_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
#endif


#ifdef DEVICE_BLACKDOORBELL
  #define DEVICENAME_CTR          "blackdoorbell"
  #define DEVICENAME_FRIENDLY_CTR "Black Doorbell"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
     
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      #endif      
      "\"D2\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR     "\","
      
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "BlackDoorbell"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif

#ifdef DEVICE_GAZEBCON
  #define DEVICENAME_CTR          "gazebcon"
  #define DEVICENAME_FRIENDLY_CTR "Gazebo Controller"

  #define DEVICE_LOCATION_OUTSIDE
 
  #define STRIP_REPEAT_OUTPUT_MAX       500
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define USE_MODULE_SENSORS_BME
  #define D_DEVICE_SENSOR_CLIMATE "Outside"

  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_DRIVERS_RELAY
  
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR "\","
      "\"D5\":\"" D_GPIO_FUNCTION_REL1_INV_CTR "\","
      "\"D6\":\"" D_GPIO_FUNCTION_REL2_INV_CTR "\","
      "\"D7\":\"" D_GPIO_FUNCTION_REL3_INV_CTR "\","
      "\"D0\":\"" D_GPIO_FUNCTION_REL4_INV_CTR "\","
      "\"D3\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"      
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":50,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_RGB "\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"User 10\","
    "\"" D_JSON_ANIMATIONMODE           "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_BRIGHTNESS     "\":5"
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "BigBulbs"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Lanterns"
  #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Water"
  #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "Relay4"

  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Gazebo"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE "\""
      "]"
    "}"
  "}";

#endif

#ifdef DEVICE_WHITEFAN1
  #define DEVICENAME_CTR          "whitefan1"
  #define DEVICENAME_FRIENDLY_CTR "White Fan 1"
  #define USE_MODULE_DRIVERS_RELAY 1
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "relay"
  #define USE_MODULE_SENSORS_BUTTONS

  
    pCONT_set->Settings.module = MODULE_SONOFF_BASIC_ID;
    //pCONT_set->Settings.module_pins.io[14]= GPIO_REL1_INV_ID; 
    pCONT_set->devices_present = 1_ID;
#endif

#ifdef DEVICE_CANDLEWARMER
#define DEVICENAME_CTR          "candlewarmer"
  #define DEVICENAME_FRIENDLY_CTR "Candle Warmer"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  //#define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif


#ifdef DEVICE_DRESSERLIGHT
  #define DEVICENAME_CTR          "dresserlight"
  #define DEVICENAME_FRIENDLY_CTR "Dresser Light"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  // #define ENABLE_TEMPFIX_BLOCK_BUTTON_PRESS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"
  //     "\"0\":\"" D_GPIO_FUNCTION_KEY1_CTR  "\","
  //     "\"1\":\"" D_GPIO_FUNCTION_LED2_INV_CTR  "\","
  //     "\"12\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
  //     "\"13\":\"" D_GPIO_FUNCTION_LED1_INV_CTR  "\""
  //     // #ifdef USE_MODULE_SENSORS_SWITCHES
  //     // "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
  //     // "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
  //     // #endif
  //     // #ifdef USE_MODULE_DRIVERS_RELAY
  //     // "\"4\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
  //     // // "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
  //     // #endif 
  //     // "\"16\":\"" D_GPIO_FUNCTION?_LEDLNK_INV_CTR "\""
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  // "}";



  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

  // white wire in garage, add red led inside switch as optional led connected to gpio2

#endif


#ifdef DEVICE_BEDROOM_CEILINGFAN
  #define DEVICENAME_CTR          "bedroom_ceilingfan"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Ceiling Fan"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_CUSTOM_SONOFF_IFAN

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 4
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\""
  "}";

  // Need to create default names for all class in GetDeviceName

#endif


#ifdef DEVICE_FLOORFAN1
  #define DEVICENAME_CTR          "floorfan1"
  #define DEVICENAME_FRIENDLY_CTR "IFan Floor Fan 1"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_CUSTOM_SONOFF_IFAN

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 4
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\""
  "}";

  // Need to create default names for all class in GetDeviceName

#endif

#ifdef DEVICE_EXTENTION1
  #define DEVICENAME_CTR          "extention1"
  #define DEVICENAME_FRIENDLY_CTR "Extention Lead 1"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif


#ifdef DEVICE_KITCHENLIGHT1
  #define DEVICENAME_CTR          "kitchenlight1"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 1 [Main Kitchen]"
  
  #define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 124
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE

  #define DISABLE_SLEEP
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
      // "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Kitchen Main"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Light 2"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif

#ifdef DEVICE_KITCHENLIGHT2
  #define DEVICENAME_CTR          "kitchenlight2"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 2 [Table/Window]"
  
  #define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 2
  #define USE_MODULE_DRIVERS_INTERFACE
    
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  // "}";


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"15\":\"" D_GPIO_FUNCTION_REL1_CTR  "\"," //swapped relays
      "\"4\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Dining Room"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Window"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif


#ifdef DEVICE_KITCHENLIGHT2_TESTER
  #define DEVICENAME_CTR          "kitchenlight2_tester"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 2 T[Table/Window]"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 2
  #define USE_MODULE_DRIVERS_INTERFACE
    
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  // "}";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\"" D_GPIO_FUNCTION_REL1_CTR  "\","
      "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Table"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Window"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif



#ifdef DEVICE_KITCHENLIGHT3
  #define DEVICENAME_CTR          "kitchenlight3"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen 3 Virtual"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_VIRTUAL_REMOTE_URL_RELAY
  // #define VIRTUAL_DEVICE_MDNS_NAME          "kitchenlight4"
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"5\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  // "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Virtual Light"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif


#ifdef DEVICE_KITCHENLIGHT4
  #define DEVICENAME_CTR          "kitchenlight4"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Lgt 4 Cupboard P"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Cupboard"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif


#ifdef DEVICE_KITCHENLIGHT5
  #define DEVICENAME_CTR          "kitchenlight5"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 5 Shelly 2 desktester"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES

  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  // "}";

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"15\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Light1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Light2"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif


#ifdef DEVICE_GARAGELIGHT
  #define DEVICENAME_CTR          "garagelight"
  #define DEVICENAME_FRIENDLY_CTR "Garage Security Lights"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_SWITCHES

  //#define  "v78.23.10+" //only remove when all device exceed this

  // motion comes from switch inputs, which I need to write. Actually, all "motion" should use switch inputs!
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Top"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Back Garden"
  
  // #define USE_MODULE_CUSTOM_SECURITY_LIGHT //remote controls for now

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 2
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      // "\"13\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      // "\"5\":\""  D_GPIO_FUNCTION_SWT2_NP_CTR  "\","

//need to make no pullup versions

      "\"13\":\"" D_GPIO_FUNCTION_PIR_1_IN_CTR     "\","
      "\"5\":\"" D_GPIO_FUNCTION_PIR_2_INV_CTR     "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\""  D_GPIO_FUNCTION_REL1_CTR  "\"," 
      "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway Light"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden Light"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif

#ifdef DEVICE_GARAGELIGHT2
  #define DEVICENAME_CTR          "garagelight2"
  #define DEVICENAME_FRIENDLY_CTR "Garage Security Lights 2"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define  "v78.23.10+" //only remove when all device exceed this

  // #define ENABLE_BUG_TRACING

  #define DISABLE_MOTIONLIGHT_TIMED_LINK
  
  #define USE_MODULE_SENSORS_SWITCHES

  // motion comes from switch inputs, which I need to write. Actually, all "motion" should use switch inputs!
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Top"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Back Garden"
  
  #define USE_MODULE_CUSTOM_SECURITY_LIGHT

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 2
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"13\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR     "\","
      "\"5\":\"" D_GPIO_FUNCTION_PIR_2_INV_CTR     "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\""  D_GPIO_FUNCTION_REL1_CTR  "\"," 
      "\"15\":\"" D_GPIO_FUNCTION_REL2_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway Light"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden Light"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif

/**
 * 
 * Hoping you both have the perfect day and a long happy marriage filled with joy and laughter. 
 * 
 * 
 * */


#ifdef DEVICE_SILVERLAMP1
  #define DEVICENAME_CTR          "silverlamp1"
  #define DEVICENAME_FRIENDLY_CTR "Silver Lamp 1"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif
#ifdef DEVICE_SILVERLAMP2
  #define DEVICENAME_CTR          "silverlamp2"
  #define DEVICENAME_FRIENDLY_CTR "Silver Lamp 2"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif

#ifdef DEVICE_HALLWAYMIRROR
  #define DEVICENAME_CTR          "hallwaymirror"
  #define DEVICENAME_FRIENDLY_CTR "Hallway Mirror"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif

#ifdef DEVICE_SOCKET_NUMBERED
  #define DEVICENAME_CTR          "socket_number_" STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  #define DEVICENAME_FRIENDLY_CTR "Socket Number " STR2(DEVICENAME_SOCKET_NUMBER_CTR)
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_SLEEP
  #define DISABLE_WEBSERVER

  #define ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS
  #define ENABLE_DEVFEATURE_TESTING_LONG_LOOPS
  #define ENABLE_DEVFEATURE_RTC_TIME_V2
  
  #define USE_MODULE_SENSORS_BUTTONS
  
  #define ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
#endif

// #ifdef DEVICE_ENSUITESENSOR
//   #define DEVICENAME_CTR          "ensuitesensor"
//   #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
  
//   #define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 1
  
//   // #define ENABLE_BUG_TRACING

//   #define USE_MODULE_SENSORS_BUTTONS
//   #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

//   #define USE_MODULE_SENSORS_BUTTONS

//   #define USE_MODULE_SENSORS_BME

//   #define USE_MODULE_DRIVERS_RELAY
//   #define RELAYS_CONNECTED 1
//   #define USE_MODULE_DRIVERS_INTERFACE
    
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       #ifdef USE_MODULE_SENSORS_BME
//       "\"1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"3\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
//       #endif
//       "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
//   "}";

//   #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
//   // #define SET_POWERONSTATE_AS_ON

//   // Drivers, Sensors and lights?
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//         "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
//           "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
//         "]"
//     "}"
//   "}";
// #endif

#ifdef DEVICE_DESKFAN
  #define DEVICENAME_CTR          "deskfan"
  #define DEVICENAME_FRIENDLY_CTR "Desk Fan"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define  "v78.23.10+" //only remove when all device exceed this
  
  #define USE_MODULE_DRIVERS_PWM
  #define USE_MODULE_CUSTOM_PWM_FAN

  // #define USE_MODULE_DRIVERS_RELAY
  // #define RELAYS_CONNECTED 4
  // #define USE_MODULE_DRIVERS_INTERFACE
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"D0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_PWM
      "\"D1\":\"" D_GPIO_FUNCTION_FAN_PWM1_CTR   "\","    //custom class for fans, but uses driver pwm class
      #endif

      
      // "\"D2\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","


      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Table"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Window"
  #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Window1"
  #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "Window2"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";



#endif

#ifdef DEVICE_BEDSIDE_LAMP1
  #define DEVICENAME_CTR          "masterbedsidelamp1"
  #define DEVICENAME_FRIENDLY_CTR "Mums Bedside Table Lamp"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";


  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"
  //     // #ifdef USE_MODULE_SENSORS_BUTTONS
  //     "\"D2\":\"" D_GPIO_FUNCTION_KEY1_CTR  "\"," //d2=4
  //     // #endif
  //     // #ifdef USE_MODULE_SENSORS_BUTTONS
  //     // "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR  "\","
  //     // #endif
  //     // #ifdef USE_MODULE_DRIVERS_RELAY
  //     "\"D4\":\"" D_GPIO_FUNCTION_REL1_CTR  "\"," //d4=2
  //     // #endif 
  //     "\"16\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
  //     "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  // "}";



  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif

#ifdef DEVICE_SONOFFBASIC_FAIRYLIGHTS
  #define DEVICENAME_CTR          "sonoffbasic_fairylights"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff Basic Fairy Lights"
  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "relay"
  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS
#endif

#ifdef DEVICE_EXERCISE_BIKE
  #define DEVICENAME_CTR          "exercise_bike"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff Basic Tester"
  #define USE_MODULE_SENSORS_PULSE_COUNTER
  #define USE_MODULE_CUSTOM_EXERCISE_BIKE  
  
    pCONT_set->Settings.module_pins.io[D7] = GPIO_PULSE_COUNTER_ID;
#endif





// New groups for build type
// #define USE_BUILD_TYPE_GENERIC
// #define USE_BUILD_TYPE_MINIMAL
// #define USE_BUILD_TYPE_SENSORS
// #define USE_BUILD_TYPE_IR
// #define USE_BUILD_TYPE_DISPLAY
// #define USE_BUILD_TYPE_LIGHTING
//  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
// #define USE_BUILD_TYPE_ENERGY
// #define USE_BUILD_TYPE_CUSTOM    //currently all

// #define ENABLE_BUG_TRACING
  //to do
  /*
    1) Only update % of count with random enabled
    2) Enable basic mixer (switch case list of commands to set that is hardcoded)

{
  "AnimationMode": "Flasher",
  "Flasher": {
    "Function": 1,
    "ColourRefreshRate":100,
    "Direction":1,
    "BrightnessMin":20
  },
  "Transition":{"Order":"Random","PixelUpdatePerc":100,"RateMs":1000},
  "ColourPalette": 0,
  "BrightnessRGB": 50,
  "TimeMs":1
}

{
  "AnimationMode": "Flasher",
  "Flasher": {
    "Function": 1,
    "Direction":0,
    "AlternateBrightnessMax":50,
    "AlternateRandomAmount":3
  },
  "PixelGrouping":{
    "Enabled":"On",
    "MappedMultiplierData":[22,14,14,15,15,15,12,20,10,8,4,1],
    "Mode":3
  },
  "Transition":{"Order":"InOrder","PixelUpdatePerc":4,"RateMs":1000},
  "ColourPalette": 0,
  "BrightnessRGB": 40,
  "TimeMs":0
}

{
  "AnimationMode": "Flasher",
  "Flasher": {
    "Function": 1,
    "Direction":0,
    "AlternateBrightnessMax":50,
    "AlternateRandomAmount":3
  },
  "mixer":{
    "enabled":1,
    "run_time_duration_scaler_as_percentage":50
  },
  "PixelGrouping":{
    "Enabled":"On",
    "MappedMultiplierData":[22,14,14,15,15,15,12,20,10,8,4,1],
    "Mode":3
  },
  "Transition":{"Order":"InOrder","PixelUpdatePerc":100,"RateMs":2000},
  "ColourPalette": 0,
  "BrightnessRGB": 30,
  "TimeMs":0
}

{
  "AnimationMode": "Flasher",
  "Flasher": {
    "Function": 1,
    "Direction":0,
    "AlternateBrightnessMax":50,
    "AlternateRandomAmount":3,
    "AgedColouring":0
  },
  "mixer":{
    "enabled":0,
    "running_id":1,
    "run_time_duration_scaler_as_percentage":50
  },
  "PixelGrouping":{
    "Enabled":"Off",
    "MappedMultiplierData":[22,14,14,15,15,15,12,20,10,8,4,1],
    "Mode":3
  },
  "Transition":{"Order":"InOrder","PixelUpdatePerc":10,"RateMs":2000},
  "ColourPalette": 40,
  "BrightnessRGB": 100,
  "TimeMs":0
}


Add function that adds saturaiton variation flag on gett palette colours to make them less uniform like real lights

  */

// #define ENABLE_BUG_TRACING
// #define ENABLE_DEVFEATURE_FLICKER_TESTING
// #define ENABLE_DEVFEATURE_FLICKERING_TEST2
// #define ENABLE_DEVFEATURE_FLICKERING_TEST3
// #define ENABLE_DEVFEATURE_FLICKERING_TEST5


/*


  #ifdef ENABLE_DEVFEATURE_FLICKER_TESTING

  #endif // ENABLE_DEVFEATURE_FLICKER_TESTING

  */





#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H
