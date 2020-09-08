#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_H

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
#include <avr/pgmspace.h>
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/mHardwareTemplates.h"

#define USE_JSON_TO_FLASH_MEMORY_TEST

//--------------------------------[Enable Device]-------------------------------------

/**
 *  LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- 
**/
// #define DEVICE_RGBROOF 
// #define DEVICE_RGBDELL
//#define DEVICE_RGBCRYSTAL1
//#define DEVICE_RGBSHELF
//#define DEVICE_RGBMICRO1 //glass box
#define DEVICE_RGBMICRO2 //projector
// #define DEVICE_RGBBEDLIGHT
// #define DEVICE_DESKLIGHT
// #define DEVICE_RGBCOOKER // new lighting H801
//  #define DEVICE_RGBUTILITY // new lighting H801
// #define DEVICE_RGBFRIDGE
//#define DEVICE_RGBMICRO3 //bedroom string esp01
// #define DEVICE_RGBMICRO4 //gazebo
// #define DEVICE_RGBDESK

/**
 *  CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- CUSTOM   -- 
**/
// #define DEVICE_TESTBUILDER
// #define DEVICE_GARAGELIGHT
// #define DEVICE_SIDEDOORLIGHT
// #define DEVICE_RADIATORFAN
// #define DEVICE_BEDROOMBLINDS
// #define DEVICE_DOORBELLWALLCHIME
// #define DEVICE_LIVINGROOMSENSOR
// #define DEVICE_OILFURNACE
//#define DEVICE_GAZEBCON
// #define DEVICE_HEATING
// #define DEVICE_MYBEDROOM
//#define DEVICE_KITCHENPANEL
// #define DEVICE_LANDINGPANEL
//#define DEVICE_EXERCISE_BIKE
// #define DEVICE_BLACKDOORBELL
// #define DEVICE_BEDROOM_CEILINGFAN


/**
 *  ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- 
**/
// #define DEVICE_CONSUMERUNIT 

/**
 *  SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- 
**/
// #define DEVICE_ATTIC_NODE
// #define DEVICE_KITCHENSENSOR
// #define DEVICE_UTILITYSENSOR

/**
 *  SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
**/
// #define DEVICE_DESKFAN
// #define DEVICE_SONOFFTESTER
// #define DEVICE_DRESSERLIGHT
// #define DEVICE_CANDLEWARMER
//#define DEVICE_EXTRACTORFAN
// #define DEVICE_EXTENTION1
// #define DEVICE_SILVERLAMP1
// #define DEVICE_SILVERLAMP2
// #define DEVICE_HALLWAYMIRROR

/**
 *  SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- 
**/
// #define DEVICE_KITCHENLIGHT1
// #define DEVICE_KITCHENLIGHT2
//  #define DEVICE_KITCHENLIGHT3
// #define DEVICE_KITCHENLIGHT4
// #define DEVICE_KITCHENLIGHT5 //tester



//-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------

#ifdef DEVICE_CONSUMERUNIT
  #define DEVICENAME_CTR          "consumerunit"
  #define DEVICENAME_FRIENDLY_CTR "Consumer Unit"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_BUILD_TYPE_ENERGY
  #define USE_MODULE_SENSORS_PZEM004T_MODBUS

  // #define ENABLE_BUG_TRACING
  
  #define USE_SOFTWARE_SERIAL_DEBUG
  #define DISABLE_SERIAL_LOGGING //temp measure

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      "\"3\":\""  D_GPIO_FUNCTION_PZEM016_RX_CTR "\"," 
      "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
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
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
        "]"
    "}"
  "}";
#endif


//-----------------[User Defined Devices == USE_BUILD_TYPE_LIGHTING == RGB Lighting] ----------------------------

#ifdef DEVICE_RGBROOF
  #define DEVICENAME_CTR            "rgbroof"
  #define DEVICENAME_FRIENDLY_CTR   "Bedroom String Lights"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"      
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":50,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_RGB "\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"User 17\","
    "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
#endif


#ifdef DEVICE_RGBDELL
  #define DEVICENAME_CTR            "rgbdell"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Dell"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 133
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":"    "133"          ","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_MODE           "\":\""  D_JSON_SCENE  "\","
    "\"" D_JSON_SCENE_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[15,95,70]" 
        "},"
    "\"" D_JSON_BRIGHTNESS     "\":"    "70"
  "}";  
#endif

#ifdef DEVICE_RGBDESK
  #define DEVICENAME_CTR            "rgbdesk"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Under Desk"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 133
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":"    "133"          ","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_MODE           "\":\""  D_JSON_SCENE  "\","
    "\"" D_JSON_SCENE_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[15,95,70]" 
        "},"
    "\"" D_JSON_BRIGHTNESS     "\":"    "70"
  "}";  
#endif


#ifdef DEVICE_RGBMICRO1
  #define DEVICENAME_CTR          "rgbmicro1"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 11
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
      //"\"A0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\"" //special adc functions
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
    "{"      
      "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
      "\"" D_JSON_STRIP_SIZE     "\":50,"
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_GRB "\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":4,\"" D_JSON_RATE "\":10,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"User 16\","
      "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
      "\"" D_JSON_BRIGHTNESS     "\":100"
    "}";
#endif


#ifdef DEVICE_RGBMICRO2 //projector
  #define DEVICENAME_CTR          "rgbmicro2"
  #define DEVICENAME_FRIENDLY_CTR "Projector Micro Lights"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  
  // #define STRIP_SIZE_MAX 5
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_DEBUG_BOOT_DELAYS

  // #define USE_MODULE_SENSORS_MOTION
  // #define MOTIONALERT_PAYLOAD1_CTR "Doppler"
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_PWM
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\"," 
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      #endif  

      SUBDEFINE_GPIO_TEMPLATE_PIN("D1",D_GPIO_FUNCTION_SERIAL_DEBUG_TX_ID)
      SUBDEFINE_GPIO_TEMPLATE_PIN("D2",D_GPIO_FUNCTION_SERIAL_DEBUG_RX_ID)



      "\"D4\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":50,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"Random\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"USER_17\","
    "\"" D_JSON_MODE           "\":\"" D_JSON_SCENE "\","
    "\"" D_JSON_SCENE_COLOUR   "\":{\"" D_JSON_HSB "\":[15,90,50]" "},"
    "\"" D_JSON_BRIGHTNESS     "\":50"
  "}";

#endif


#ifdef DEVICE_RGBMICRO3 //room speaker test
  #define DEVICENAME_CTR          "rgbmicro3"
  #define DEVICENAME_FRIENDLY_CTR "Micro Lights 3"  
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 3
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":50,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_RGB "\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":30,\"" D_JSON_RATE "\":60,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"User 18\","
    "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
    "\"" D_JSON_BRIGHTNESS     "\":60"
  "}";
#endif


#ifdef DEVICE_RGBMICRO4 //gazebo
  #define DEVICENAME_CTR          "rgbmicro4"
  #define DEVICENAME_FRIENDLY_CTR "RGB Micro LED Lights 4"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  // #define ENABLE_BUG_TRACING
  // #define USE_DEBUG_LINE

  #define STRIP_SIZE_MAX 50

  // #define USE_MODULE_SENSORS_MOTION
  // #define MOTIONALERT_PAYLOAD1_CTR "Doppler"
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_LIGHTS_INTERFACE
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      #endif  
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","    
      "\"D4\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  // Need to add protection to not load templates if invalid

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":50,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":1,\"" D_JSON_RATE "\":2,\"" D_JSON_ORDER "\":\"Random\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"USER_17\","
    "\"" D_JSON_MODE           "\":\"" D_JSON_SCENE "\","
    "\"" D_JSON_SCENE_COLOUR   "\":{\"" D_JSON_HSB "\":[120,90,50]" "},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
#endif


#ifdef DEVICE_H801_TESTER // for PWM dev
  #define DEVICENAME_CTR          "h801_tester"
  #define DEVICENAME_FRIENDLY_CTR "H801 Tester Strip"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)

  #define USE_SERIAL_ALTERNATE_TX
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"FLAG\":0,"
    "\"BASE\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
    "{"
      "\"" D_JSON_HARDWARE_TYPE  "\":\"RGBCCT_PWM\","
      "\"" D_JSON_CCT_TEMP       "\":500,"
      "\"" D_JSON_BRIGHTNESS     "\":99"
    "}";

    #define ESP8266

#endif
#ifdef DEVICE_DESKLIGHT
  #define DEVICENAME_CTR          "desklight"
  #define DEVICENAME_FRIENDLY_CTR "Desk Light"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)

  // #define ENABLE_FUNCTION_DEBUG  
  // #define ENABLE_BUG_TRACING

  #define USE_SERIAL_ALTERNATE_TX
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define DISABLE_TEMPORARY_RGBANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_MODULE_LIGHTS_PWM

  // #define USE_MODULE_SENSORS_MOTION
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "DiningRoom"
  
  #define DISABLE_SLEEP

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"D2\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      // #endif 
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"FLAG\":0,"
    "\"BASE\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"RGBCCT_PWM\","
    "\"" D_JSON_CCT_TEMP       "\":500,"
    "\"" D_JSON_BRIGHTNESS     "\":99"
  "}";

  #define ESP8266

  // #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "Kitchen Test"
  
  // Drivers, Sensors and lights?
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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


#ifdef DEVICE_RGBCOOKER
  #define DEVICENAME_CTR          "rgbcooker"
  #define DEVICENAME_FRIENDLY_CTR "RGB Cooker H801"
    
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)

  // #define ENABLE_FUNCTION_DEBUG  
  // #define ENABLE_BUG_TRACING

  #define USE_SERIAL_ALTERNATE_TX
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define DISABLE_TEMPORARY_RGBANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_MODULE_LIGHTS_PWM

  // #define USE_MODULE_SENSORS_MOTION
  // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "DiningRoom"
  
  #define DISABLE_SLEEP

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"FLAG\":0,"
    "\"BASE\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"RGBCCT_PWM\","
    "\"" D_JSON_CCT_TEMP       "\":500,"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

  #define ESP8266

  // #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "Kitchen Test"
  
  // Drivers, Sensors and lights?
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_PWM
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define DISABLE_TEMPORARY_RGBANIMATOR
  #define USE_MODULE_LIGHTS_PWM


  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
    "{"
      "\"" D_JSON_HARDWARE_TYPE  "\":\"RGBCCT_PWM\","   //"\":\"WS2812\","
      "\"" D_JSON_STRIP_SIZE     "\":50,"
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"Random\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"USER_17\","
      "\"" D_JSON_MODE           "\":\"Flasher\","
      "\"" D_JSON_BRIGHTNESS     "\":99"
    "}";
    
    #define ESP8266

#endif


#ifdef DEVICE_RGBCRYSTAL1
  #define DEVICENAME_CTR            "rgbcrystal1"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 10
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
    "{"
      "\"NAME\":\"" DEVICENAME_CTR "\","
      "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
      "\"GPIOC\":{"
        "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
      "},"
      "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
    "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
    "{"
      "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
      "\"" D_JSON_STRIP_SIZE     "\":50," //really 150
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":4,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"USER_19\"," //purple/pink/red (at sunset orange?)
      "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
      "\"" D_JSON_BRIGHTNESS     "\":50"
    "}";

#endif
#ifdef DEVICE_RGBSHELF
  #define DEVICENAME_CTR          "rgbshelf"
  #define DEVICENAME_FRIENDLY_CTR "Shelf Lights"
  
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 12
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
    "{"
      "\"NAME\":\"" DEVICENAME_CTR "\","
      "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
      "\"GPIOC\":{"
        "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
      "},"
      "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
    "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
    "{"
      "\"" D_JSON_HARDWARE_TYPE  "\":\"" D_JSON_WS2812 "\","
      "\"" D_JSON_STRIP_SIZE     "\":33,"
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"" D_JSON_RGB "\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":20,\"" D_JSON_RATE "\":120,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"User 03\","
      "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
      "\"" D_JSON_BRIGHTNESS     "\":60"
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
  #define DEFAULT_LIGHTING_ANIMATION_MODE_ID             UO_ANIMATION_MODE_FLASHER_ID
  #define DEFAULT_LIGHTING_FLASHER_FUNCTION_ID           UO_FLASHER_FUNCTION_SLOW_GLOW_ID
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
  #define DEFAULT_LIGHTING_ANIMATION_MODE_ID             UO_ANIMATION_MODE_FLASHER_ID
  #define DEFAULT_LIGHTING_FLASHER_FUNCTION_ID           UO_FLASHER_FUNCTION_SLOW_GLOW_ID
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

  // #define USE_MODULE_CUSTOM_CEILING_FAN
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
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
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
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
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
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE)                                                  //parsed via lighting interface
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
    "\"" D_JSON_STRIP_SIZE     "\":50,"
    "\"" D_JSON_STRIP_REPEAT   "\":5,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\"," //GRBW
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"Random\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"USER_18\","
    "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
  // #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  // #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  // #define STRIP_SIZE_MAX                      12   


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Attic Socket"
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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



#ifdef DEVICE_ATTIC_NODE
  #define DEVICENAME_CTR          "attic_node"
  #define DEVICENAME_FRIENDLY_CTR "Attic Node"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING
  
  #define USE_MODULE_SENSORS_BME
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
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
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  // #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // // Drivers, Sensors and lights?
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   //device_names:{"module_name":["relay1","relay2"]}
  //   "\"" D_JSON_DEVICENAME "\":{"
  //       "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
  //         "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
  //       "]"
  //   "}"
  // "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
          "\"" "Attic" "\""
        "]"
    "}"
  "}";

#endif

#ifdef DEVICE_HEATING
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "Heating"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  //#define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  #define FORCE_DEVICENAME_CLEAR_ON_BOOT

  #define ENABLE_HEATING_DEBUG_TIMES

  #define USE_BUILD_TYPE_CUSTOM
  #define USE_MODULE_CUSTOM_HEATING
  
  #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DS18B20
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","
      "\"9\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  // Controller Device - Sets device names by hard coded
  
#endif
#ifdef DEVICE_DOORBELLWALLCHIME
  #define DEVICENAME_CTR          "doorbellwallchime"
  #define DEVICENAME_FRIENDLY_CTR "Doorbell Wall Chime"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  
  // #define USE_MODULE_SENSORS_DOORCHIME
  // #define DOORALERT_PAYLOAD_CTR   "frontdoorbell"

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_SENSORS_DOORCHIME
      "\"D6\":\"" D_GPIO_FUNCTION_CHIME_INPUT_CTR     "\","
      "\"D7\":\"" D_GPIO_FUNCTION_CHIME_RINGER_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":"    "12"          ","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_MODE           "\":\""  D_JSON_SCENE  "\","
    "\"" D_JSON_SCENE_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[180,100,10]" 
        "},"
    "\"" D_JSON_BRIGHTNESS     "\":"    "10"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define PIXEL_LIGHTING_HARDWARE_SK6812_STRIP

#endif
#ifdef DEVICE_RADIATORFAN
  /**
   * Single Relay controls TWO 12V DC fans to improve heating (1 D_Out)
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
  //#define USE_MODULE_SENSORS_BME

  #define USE_MODULE_CUSTOM_RADIATORFAN

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
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
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      #endif 
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Fan"
  #define RELAYS_CONNECTED 1

  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "Top Vent"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "Inside Vent"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
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
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  //#define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  #define USE_BUILD_TYPE_CUSTOM
  #define USE_MODULE_CUSTOM_OILFURNACE
  
  // #define USE_MODULE_SENSORS_DHT
  // #define USE_MODULE_SENSORS_DS18B20
  // #define USE_MODULE_DRIVERS_RELAY
  // #define USE_DIGITALIO
  #define USE_MODULE_SENSORS_ULTRASONICS
  //#define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","
      "\"D3\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif


//-----------------[User Defined Devices == USE_BUILD_TYPE_SENSORS == Sensing only] ----------------------------


#ifdef DEVICE_MASTERBEDROOMSENSOR
  #define DEVICENAME_CTR          "masterbedroomsensor"  
  #define DEVICENAME_FRIENDLY_CTR "Master Bedroom Controller"
  #define USE_MODULE_SENSORS_BME
  
    pCONT_set->Settings.module = MODULE_NODEMCU_ID;
    pCONT_set->Settings.module_pins.io[D1] = GPIO_I2C_SCL_ID;
    pCONT_set->Settings.module_pins.io[D2] = GPIO_I2C_SDA_ID;
#endif


//-----------------[User Defined Devices == USE_BUILD_TYPE_DISPLAYS == Sensing only] ----------------------------


//-----------------[User Defined Devices == TO SORT == ] ----------------------------




#ifdef DEVICE_BEDROOMPANEL
  #define DEVICENAME_CTR          "bedroompanel"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Switch Panel"
  #define DEVICE_CRITICALITY_CTR  CRITICALITY_LEVEL_LOW
  #define DEVICE_STABILITY_CTR    STABILITY_LEVEL_NIGHTLY
  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 5
  #define DEBUG_TESTCODE
  
    pCONT_set->Settings.module = MODULE_NODEMCU_ID;
    pCONT_set->Settings.module_pins.io[D5] = GPIO_NEXTION_RX_ID;
    pCONT_set->Settings.module_pins.io[D6] = GPIO_NEXTION_TX_ID;
#endif
#ifdef DEVICE_LANDINGPANEL
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Control Panel"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
        #ifdef USE_MODULE_SENSORS_MOTION
        "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
        #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":"    "28"          ","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_MODE           "\":\""  D_JSON_SCENE  "\","
    "\"" D_JSON_SCENE_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[25,90,10]" 
          // "\"" D_JSON_RGBW    "\":[0,0,0,255]" 
        "},"
    "\"" D_JSON_BRIGHTNESS     "\":"    "100"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define PIXEL_LIGHTING_HARDWARE_SK6812_STRIP

  
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
#ifdef DEVICE_KITCHENPANEL
  #define DEVICENAME_CTR          "nextionkitchen"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Panel"
  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 6
#endif
#ifdef DEVICE_MYBEDROOM
  #define DEVICENAME_CTR          "mybedroom"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Controller"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_DOOR
  #define USE_SENSOR_DOOR_LOCK
  #define DOORALERT_PAYLOAD_CTR "bedroom"

  // #define USE_MODULE_CUSTOM_CEILING_FAN
  // #define USE_MODULE_DRIVERS_IRTRANSCEIVER

  #define USE_MODULE_SENSORS_DS18B20

  #define USE_MODULE_SENSORS_MOTION
  #define MOTIONALERT_PAYLOAD1_CTR "bedroom"
  
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
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
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"D6\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      // add status led to here
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE)                                                  //parsed via lighting interface
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
    "\"" D_JSON_STRIP_SIZE     "\":12,"
    "\"" D_JSON_STRIP_REPEAT   "\":5,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\"," //GRBW
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"Random\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"USER_18\","
    "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
    "\"" D_JSON_BRIGHTNESS     "\":10"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  #define STRIP_SIZE_MAX                      12   

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "LED1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "LED2"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
    //     "],"
    //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
    //       "\"" "bedroom" "\""
    //     "],"
    //     "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
    //       "\"" "bedroomDOOR" "\""
    //     "]"
    // "}"
  "}";

#endif
#ifdef DEVICE_LIVINGROOMSENSOR
  #define DEVICENAME_CTR          "livingroomsensor"
  #define DEVICENAME_FRIENDLY_CTR "Living Room Sensor"
    
  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
     
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "LivingRoom"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "DriveFront"

  #define D_DEVICE_SENSOR_CLIMATE "LivingRoom"

  #define D_DEVICE_SENSOR_DRIVEWAY_ULTRSONIC_FRIENDLY_NAME_LONG D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG

  #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_ULTRASONICS

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_SENSORS_DHT
      "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR    "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR     "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR     "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D5\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_2_CTR     "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_ULTRASONIC_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_DRIVEWAY_ULTRSONIC_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_CLIMATE "\""
        "],"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
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
  //#define USE_MODULE_SENSORS_ANALOG_MEASURE
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_DRIVERS_HBRIDGE
      "\"D1\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_IA_CTR   "\","
      "\"D6\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_OA_CTR   "\","
      "\"D5\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_IB_CTR   "\","
      "\"D7\":\"" D_GPIO_FUNCTION_HBRIDGE_L9110_OB_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_ANALOG_MEASURE
      "\"A0\":\"" D_GPIO_FUNCTION_ANALOG_POSITION_CTR "\","
      #endif
      "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE)                                                  //parsed via lighting interface
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
    "\"" D_JSON_STRIP_SIZE     "\":12,"
    "\"" D_JSON_STRIP_REPEAT   "\":5,"
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\"," //GRBW
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"Random\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"USER_18\","
    "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
    "\"" D_JSON_BRIGHTNESS     "\":10"
  "}";
  #define PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL
  #define USE_TASK_RGBLIGHTING_NOTIFICATIONS   
  #define STRIP_SIZE_MAX                      12   

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "LED1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "LED2"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
        "],"
    //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
    //       "\"" "bedroom" "\""
    //     "],"
    //     "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
    //       "\"" "bedroomDOOR" "\""
    //     "]"
    // "}"
  "}";



  // #define DEVICE_CRITICALITY_CTR  CRITICALITY_LEVEL_LOW
  // #define DEVICE_STABILITY_CTR    STABILITY_LEVEL_PRE_RELEASE
  // #define USE_MODULE_CUSTOM_BLINDS
  // #define USE_MODULE_DRIVERS_HBRIDGE
  
  //   pCONT_set->Settings.module = MODULE_NODEMCU_ID;
  //   pCONT_set->Settings.module_pins.io[3] = GPIO_RGB_DATA_ID;
#endif


#ifdef DEVICE_GARAGELIGHT
  #define DEVICENAME_CTR          "garagelight"
  #define DEVICENAME_FRIENDLY_CTR "Garage Light"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway"
  // #define MOTIONALERT_PAYLOAD2_CTR "Garden"
  
  #define USE_MODULE_CUSTOM_SECURITY_LIGHT

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D1\":\"" D_GPIO_FUNCTION_REL1_INV_CTR   "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL2_INV_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D2\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_PIR_2_CTR     "\","
      #endif 
      // "\"D0\":\"" D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway"
  // #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden"
  #define RELAYS_CONNECTED 1
  
  // Drivers, Sensors and lights?
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //       "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
  //         "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
  //         "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
  //       "]"
  //   "}"
  // "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define RELAYS_CONNECTED 1
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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


#ifdef DEVICE_SIDEDOORLIGHT
  #define DEVICENAME_CTR          "sidedoorlight"
  #define DEVICENAME_FRIENDLY_CTR "Side Door Motion Light"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 4 //maintain other settings (bootcount)
   
  #define USE_MODULE_SENSORS_MOTION
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Middle Driveway"
  
  #define USE_MODULE_CUSTOM_SECURITY_LIGHT

  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_NP_CTR     "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"D2\":\"" D_GPIO_FUNCTION_REL1_CTR   "\","
      #endif
      "\"D4\":\""  D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  // DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"NAME\":\"" DEVICENAME_CTR "\","
  //   "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"BASE\":\"" D_MODULE_NAME_SHELLY1_CTR "\""
  // "}";



  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define RELAYS_CONNECTED 1
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
  
//   #define USE_MODULE_SENSORS_MOTION

//   #define USE_MODULE_SENSORS_DHT

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"NAME\":\"" DEVICENAME_CTR "\","
//     "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"GPIOC\":{"
//       "\"D1\":\"" D_GPIO_FUNCTION_DHT22_1_CTR    "\","
//       #ifdef USE_MODULE_SENSORS_MOTION
//       "\"D2\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
//       #endif
//       "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
//     "},"
//     "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
//     "\"" D_JSON_STRIP_SIZE     "\":"    "50"          ","
//     "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
//     "\"" D_JSON_MODE           "\":\""  D_JSON_SCENE  "\","
//     "\"" D_JSON_SCENE_COLOUR   "\":{"
//           "\"" D_JSON_HSB    "\":[30,80,100]" 
//         "},"
//     "\"" D_JSON_BRIGHTNESS     "\":"    "50"
//   "}";

//   #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
//   #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
//   // Drivers, Sensors and lights?
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Sensor (p.RGBCooker)"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_SENSORS_DHT

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"D1\":\"" D_GPIO_FUNCTION_DHT22_1_CTR    "\","
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D2\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      #endif
      // "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\""  D_JSON_WS2812 "\","
    "\"" D_JSON_STRIP_SIZE     "\":"    "50"          ","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\""  D_JSON_GRB    "\","
    "\"" D_JSON_MODE           "\":\""  D_JSON_SCENE  "\","
    "\"" D_JSON_SCENE_COLOUR   "\":{"
          "\"" D_JSON_HSB    "\":[30,80,100]" 
        "},"
    "\"" D_JSON_BRIGHTNESS     "\":"    "50"
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Kitchen"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif
#ifdef DEVICE_RGBUTILITY
  #define DEVICENAME_CTR          "rgbutility"
  #define DEVICENAME_FRIENDLY_CTR "RGB Utility H801"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define DISABLE_TEMPORARY_RGBANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"FLAG\":0,"
    "\"BASE\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"RGBCCT_PWM\","
    "\"" D_JSON_CCT_TEMP       "\":500,"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

#endif
#ifdef DEVICE_RGBFRIDGE
  #define DEVICENAME_CTR          "rgbfridge"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fridge H801"

  #define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 2 //maintain other settings (bootcount)
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define DISABLE_TEMPORARY_RGBANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"FLAG\":0,"
    "\"BASE\":\"" D_MODULE_NAME_H801_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"RGBCCT_PWM\","
    "\"" D_JSON_CCT_TEMP       "\":500,"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

#endif
#ifdef DEVICE_UTILITYSENSOR
  #define DEVICENAME_CTR          "utilitysensor"
  #define DEVICENAME_FRIENDLY_CTR "Utility Sensor (prev. RGB Utility)"

  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 3 //maintain other settings (bootcount)
     
  #define USE_MODULE_SENSORS_MOTION
  #define MOTIONALERT_PAYLOAD1_CTR "utility"

  #define USE_MODULE_SENSORS_DOOR
  #define DOORALERT_PAYLOAD_CTR   "backdoor"

  #define USE_MODULE_SENSORS_DHT

  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    DEFINE_APP_SVALUE("NAME",DEVICENAME_CTR)
    DEFINE_APP_SVALUE("FRIENDLYNAME",DEVICENAME_FRIENDLY_CTR)
    "\"GPIOC\":{"
      #ifdef USE_MODULE_SENSORS_DHT
      DEFINE_APP_SVALUE("D1",D_GPIO_FUNCTION_DHT22_1_CTR)
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      DEFINE_APP_SVALUE("D2",D_GPIO_FUNCTION_PIR_1_CTR)
      #endif
      #ifdef USE_MODULE_SENSORS_DOOR
      DEFINE_APP_SVALUE("D7",D_GPIO_FUNCTION_DOOR_DETECT_CTR)
      #endif
      DEFINE_END_SVALUE("D4",D_GPIO_FUNCTION_LEDLNK_CTR)
    "},"
    DEFINE_END_SVALUE("BASE",D_MODULE_NAME_USERMODULE_CTR)
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Utility"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Utility"
  #define D_DEVICE_SENSOR_DOOROPEN_FRIENDLY_NAME_LONG "Side Door"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_DOOROPEN_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
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
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D1\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
      #endif      
      "\"D2\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR     "\","
      
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "},"
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "BlackDoorbell"
  
  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

#endif

#ifdef DEVICE_GAZEBCON
 
  #define STRIP_REPEAT_OUTPUT_MAX                        250
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE //temp fix
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
    "{"
      "\"NAME\":\"gazebconT\","
      "\"FRIENDLYNAME\":\"Gazebo Controller\","
      "\"GPIOC\":{"
        "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR "\","
        "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR "\","
        "\"D5\":\"" D_GPIO_FUNCTION_REL1_INV_CTR "\","
        "\"D6\":\"" D_GPIO_FUNCTION_REL2_INV_CTR "\","
        "\"D7\":\"" D_GPIO_FUNCTION_REL3_INV_CTR "\","
        "\"D0\":\"" D_GPIO_FUNCTION_REL4_INV_CTR "\","
        "\"D3\":\"" D_GPIO_FUNCTION_PIR_1_CTR    "\","
        "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR "\""
      "},"
      "\"FLAG\":0," //RENAME "FEATURE?"
      "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
    "}";

  DEFINE_PROGMEM_CTR(LIGHTING_TEMPLATE)                                                  //parsed via lighting interface
    "{"
      "\"" D_JSON_HARDWARE_TYPE  "\":\"WS2812\","
      "\"" D_JSON_STRIP_SIZE     "\":50,"
      "\"" D_JSON_STRIP_REPEAT   "\":5,"
      "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRB\","
      "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"Random\"},"
      "\"" D_JSON_COLOUR_PALETTE "\":\"USER_18\","
      "\"" D_JSON_MODE           "\":\"" D_JSON_FLASHER "\","
      "\"" D_JSON_BRIGHTNESS     "\":100"
    "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "BigBulbs"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Lanterns"
  #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Water"
  #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "Relay4"
  // DEFINE_PROGMEM_CTR(DRIVER_TEMPLATE)  // drivers that are not lights
  //   "{"
  //     "\"" D_JSON_RELAY "_" D_JSON_NAME  "\":["
  //       "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
  //       "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\","
  //       "\"" D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "\","
  //       "\"" D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "}";

  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_DRIVERS_RELAY
  // DEFINE_PROGMEM_CTR(SENSOR_TEMPLATE)  // drivers that are not lights
  // "{"
  //   "\"" "motion" "\":{\"name\":\"Gazebo\",\"hardware\":\"HC-SR501\"}"
  //   "\"" "climate" "\":{\"name\":\"something\"}"
  // "}";

  
    pCONT_set->Settings.module = MODULE_NODEMCU_ID;
    pCONT_set->Settings.module_pins.io[3] =  GPIO_RGB_DATA_ID;

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

#ifdef DEVICE_SONOFFBASIC_TESTER
  #define DEVICENAME_CTR          "sonoffbasic_tester"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff Basic Tester"
  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "relay"
  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS
#endif

#ifdef DEVICE_SONOFFTESTER
  #define DEVICENAME_CTR          "dresserlight_tester"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff Dresser Socket"
  
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
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"GPIOC\":{"
    //   #ifdef USE_MODULE_SENSORS_BME
    //   "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
    //   "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
    //   #endif
    //   #ifdef USE_MODULE_SENSORS_DS18B20
    //   "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
    //   "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
    //   #endif
    //   #ifdef USE_MODULE_DRIVERS_RELAY
    //   "\"D4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
    //   // "\"D0\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
    //   #endif
    //   #ifdef USE_MODULE_SENSORS_BUTTONS
    //   "\"D0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
    //   #endif
    //   "\"D5\":\"" D_GPIO_FUNCTION_FAN_IRSEND_CTR    "\","
    //   #ifdef USE_MODULE_SENSORS_MOTION
    //   "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_CTR     "\","
    //   #endif
    //   "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    //   // add status led to here
    // "},"
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

  // white wire in garage, add red led inside switch as optional led connected to gpio2



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
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
  #define DEVICENAME_FRIENDLY_CTR "Sonoff Dresser Socket"
  
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
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
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

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 4
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_IFAN03_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Relay0"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Relay1"
  #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Relay2"
  #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "Relay3"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}

// fan controller will name these directly

    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";

  // white wire in garage, add red led inside switch as optional led connected to gpio2

#endif


#ifdef DEVICE_EXTRACTORFAN
  #define DEVICENAME_CTR          "extractorfan"
  #define DEVICENAME_FRIENDLY_CTR "Extractor Fan"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
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
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
  
  // debounce needs to be 1 second

  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE

  #define DISABLE_SLEEP
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
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
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  
  // {  D_MODULE_NAME_SHELLY2P5_CTR,        // Shelly2 (ESP8266 - 2MB) - https://shelly.cloud/shelly2/
  //    0,
  //    0,//GPIO_MCP39F5_TX_ID,  // GPIO01 MCP39F501 Serial input
  //    0,//GPIO_KEY1_ID,
  //    0,//GPIO_MCP39F5_RX_ID,  // GPIO03 MCP39F501 Serial output
  //    GPIO_REL1_ID,        // GPIO04
  //    GPIO_SWT2_NP_ID,        // GPIO05
  //                      // GPIO06 (SD_CLK   Flash)
  //                      // GPIO07 (SD_DATA0 Flash QIO/DIO/DOUT)
  //                      // GPIO08 (SD_DATA1 Flash QIO/DIO/DOUT)
  //    0,                // GPIO09 (SD_DATA2 Flash QIO or ESP8285)
  //    0,                // GPIO10 (SD_DATA3 Flash QIO or ESP8285)
  //                      // GPIO11 (SD_CMD   Flash)
  //    0,//GPIO_I2C_SDA_ID,        // GPIO12
  //    GPIO_SWT1_NP_ID,
  //    0,//GPIO_I2C_SCL_ID,        // GPIO14
  //    GPIO_REL2_ID,//GPIO_MCP39F5_RST_ID, // GPIO15 MCP39F501 Reset
  //    0,




  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Light 1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Light 2"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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

#ifdef DEVICE_KITCHENLIGHT2
  #define DEVICENAME_CTR          "kitchenlight2"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 2 [Table/Window]"
  
  #define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 2
  #define USE_MODULE_DRIVERS_INTERFACE
    
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"NAME\":\"" DEVICENAME_CTR "\","
  //   "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"BASE\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  // "}";


  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
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
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Light1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Light2"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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

#ifdef DEVICE_KITCHENLIGHT3
  #define DEVICENAME_CTR          "kitchenlight3"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen 3 Virtual Switch"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 3

  //showing no name right now
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  // #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"5\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"4\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      #endif 
      "\"0\":\"" D_GPIO_FUNCTION_LEDLNK_INV_CTR "\""
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Virtual Light"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif


#ifdef DEVICE_KITCHENLIGHT4
  #define DEVICENAME_CTR          "kitchenlight4"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 4 Cupboard"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  //#define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
  // DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"NAME\":\"" DEVICENAME_CTR "\","
  //   "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"BASE\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  // "}";

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
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
    "\"BASE\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Light1"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Light2"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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



#ifdef DEVICE_SILVERLAMP1
  #define DEVICENAME_CTR          "silverlamp1"
  #define DEVICENAME_FRIENDLY_CTR "Silver Lamp 1"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 3
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
  #define SETTINGS_HOLDER 3
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif

#ifdef DEVICE_ENSUITESENSOR
  #define DEVICENAME_CTR          "ensuitesensor"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"GPIOC\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"3\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
      "},"
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
#endif

#ifdef DEVICE_DESKFAN
  #define DEVICENAME_CTR          "deskfan"
  #define DEVICENAME_FRIENDLY_CTR "Desk Fan"
  
  #define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
  
  // #define ENABLE_BUG_TRACING

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define RELAYS_CONNECTED 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PROGMEM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"NAME\":\"" DEVICENAME_CTR "\","
    "\"FRIENDLYNAME\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"BASE\":\"" D_MODULE_NAME_SONOFF_BASIC_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Socket"
  // #define SET_POWERONSTATE_AS_ON

  // Drivers, Sensors and lights?
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PROGMEM_CTR(FUNCTION_TEMPLATE)
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





#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H
