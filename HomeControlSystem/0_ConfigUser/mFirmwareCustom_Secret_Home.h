#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_HOME_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_HOME_H

// #ifdef USER_MICHAEL

// /*********************************************************************************************\
//  * This file is one of two user configurable files
//  * 
//  * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
//  * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
//  * This can also include lights or displays as notification/information systems. 
//  * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
//  * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
// \*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
// // #define ENABLE_TESTUSER
// #define DISABLE_WEBSERVER
// #define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "2_CoreSystem/mHardwareTemplates.h"


// //--------------------------------[Enable Device]-------------------------------------

/**
 *  Landing
  - CeilingLight = dimmer, 2 inputs
  - LandingPanel = PIR, Nextion, BME, light strip
  - dht
 **/  
// #define DEVICE_SHELLYDIMMER_LANDING_CEILING
// #define DEVICE_LANDINGPANEL

/**
 * 
 Ensuite
  - CeilingLight = ShellyDimmer
  - Fan ? 
  - Sensor = motion, led strip, bme
  - home mini
 */
// #define DEVICE_ENSUITESENSOR


/**
 * 
Masterbedroom
  - CeilingLight = ShellyDimmer
  - FloorFan
  - sensor?
  - non camera home display
 */
#define DEVICE_SHELLYDIMMER_MASTERBEDROOM_CEILING


/*********
 Ensuite
  - CeilingLight = ShellyDimmer
  - Fan ? 
  - Sensor = motion, led strip, bme
  - home mini


Bathroom
  - home mini
  - shellydimmer on pull cord?


Immersion
  - sensor (8 inputs)
  - light strip

Michaels room


Spareroom
  - CeilingLight

Hallway
  - radiatorfan = fan controls, 2 db, bme, dht, pir
  - front door/table socket
  - hallway table

LivingRoom
  - 3 optional sockets for the corners
  - sensor = pir inside, bme..... ultrasonic, outside pir

Kitchen/Dining
  - kitchensensor = pir, bme
  - rgbcooker = h801
  - rgbfridge - h801
  - shelf1
  - crystal1

Utility
  - crystal2
  - pir, bme, door sensor

DSToilet
  - consumerunit = 8 pzem, bme



*/

// // #define DEVICE_RGBROOF
// // #define DEVICE_RGBDELL
// // #define DEVICE_RGBNOTIFICATION_01
// // #define DEVICE_RGBCRYSTAL1
// //#define DEVICE_RGBCRYSTAL2
// // #define DEVICE_RGBCRYSTAL3
// // #define DEVICE_RGBSHELF
// // #define DEVICE_RGBFIREPLACE
// // #define DEVICE_RGBFIREPLACE_TESTER
// // #define DEVICE_RGBBEDLIGHT                            
// //#define DEVICE_RGBBEDLIGHT_TEST                            
// // #define DEVICE_RGBDESK
// // #define DEVICE_RGBCOOKER
// // #define DEVICE_RGBUTILITY
// // #define DEVICE_RGBFRIDGE              
// // #define DEVICE_RGBBEDROOMFLOOR
// //#define DEVICE_H801_TESTER
// // #define DEVICE_H801_TESTER_NODEMCU
// //#define DEVICE_RGBCUSTOM_USER_01
// //#define DEVICE_RGBCUSTOM_USER_02
// // #define DEVICE_TESTER_RGBW
// // #define DEVICE_RGBMICRO1 //glass box
// // #define DEVICE_RGBMICRO2 //projector  
// // #define DEVICE_RGBMICRO3 //Alpha tester, before rgbroof "RGBROOF2"              
// // #define DEVICE_RGBMICRO4 //gazebo
// // #define DEVICE_RGBESP32_1_TESTER
// // #define DEVICE_RGBCLOCK_01
// // #define DEVICE_RGBCLOCK_02
// // #define DEVICE_RGBDISPLAY_GARAGE

// /**
//  *  CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- 
// **/
// // #define DEVICE_SHELLY1_TESTER
// // #define DEVICE_RADIATORFAN
// //#define DEVICE_BEDROOMBLINDS
// // #define DEVICE_DOORBELLWALLCHIME                 
// // #define DEVICE_OILTANK               
// // #define DEVICE_OILFURNACE
// // #define DEVICE_GAZEBCON
// // #define DEVICE_HEATING
// //#define DEVICE_KITCHENPANEL
// #define DEVICE_LANDINGPANEL
// //#define DEVICE_BLACKDOORBELL
// // #define DEVICE_DESKFAN
// // #define DEVICE_CAM_TESTER
// // #define DEVICE_GPSPARSER_TESTER
// // #define DEVICE_SDCARDWRITER_TESTER
// // #define DEVICE_TREADMILL

// /**
//  * ESP32 devices with special features
//  * */
// // #define DEVICE_CAM_TESTER
// // #define DEVICE_CAMERA_DRIVEWAY

// /**
//  *  ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- 
// **/
// // #define DEVICE_CONSUMERUNIT 
// //#define DEVICE_PZEM_TESTER

// /**
//  *  SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- 
// **/
// // #define DEVICE_ATTICSENSOR
// //#define DEVICE_BEDROOMSENSOR
// // #define DEVICE_MASTERBEDROOMSENSOR
// //#define DEVICE_IMMERSIONSENSOR
// // #define DEVICE_KITCHENSENSOR
// // #define DEVICE_UTILITYSENSOR
// //#define DEVICE_LIVINGROOMSENSOR
// // #define DEVICE_TESTSENSOR
// // #define DEVICE_NODEMCU_TESTER

// /**
//  *  DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- 
// **/
// // #define DEVICE_BEDROOMPANEL
// #define DEVICE_DESKPANEL

// /**
//  *  SONOFF and lighting  -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
// **/
// //#define DEVICE_SONOFFTESTER
// // #define DEVICE_SOCKET_NUMBERED

// /**
//  *  SONOFF iFANS
//  * and lighting  -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
// **/
// // #define DEVICE_BEDROOM_CEILINGFAN
// // #define DEVICE_AIRPURIFIER
// // #define DEVICE_FLOORFAN1
// // #define DEVICE_FLOORFAN2

// /**
//  *  SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- 
// **/
// //#define DEVICE_KITCHENLIGHT1
// //#define DEVICE_KITCHENLIGHT2
// //#define DEVICE_KITCHENLIGHT3
// // #define DEVICE_KITCHENLIGHT4
// //#define DEVICE_KITCHENLIGHT5 //tester
// // #define DEVICE_SHELLYDIMMER_GLOBE
// // #define DEVICE_SHELLYDIMMER_SPAREROOM_CEILING
// // #define DEVICE_GARAGELIGHT
// // #define DEVICE_SIDEDOORLIGHT
// // #define DEVICE_SIDEDOORLIGHT_TEST

// /**
//  *  TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- 
// **/
// // #define DEVICE_TESTBED_SHELLY1_01
// // #define DEVICE_TESTBED_SHELLY2_DIMMER
// // #define DEVICE_TESTBED_SHELLY2P5_01


// //-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Kitchen ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Utility ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Hallway ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Outside ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Garage ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Spare room ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Masterbedroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

#ifdef DEVICE_SHELLYDIMMER_MASTERBEDROOM_CEILING
  #define DEVICENAME_CTR          "shellydimmer_masterbedroom"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
    
  #define USE_MODULE_CORE_RULES
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define DISABLE_SERIAL_LOGGING
    
  #define PIN_GPIO_SHELLY_DIMMER_RST_INV 5
  #define PIN_GPIO_SHELLY_DIMMER_BOOT0   4

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\"" D_GPIO_FUNCTION_HWSERIAL0_TX_CTR  "\","
      "\"3\":\"" D_GPIO_FUNCTION_HWSERIAL0_RX_CTR  "\","
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"14\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "Main"
  #define D_DEVICE_SWITCH_2_FRIENDLY_NAME_LONG "Bed"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH_2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\"," 
        "\"Function\":\"StateChanged\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"StateChanged\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":50"
      "}"
    "}"    
  "}";


#endif

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Landing ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_LANDINGPANEL
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Control Panel"

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT
  #define STRIP_SIZE_MAX 40
  
  #define DISABLE_WEBSERVER

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
      "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":40,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgbwc\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":8"
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":25,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\"," //ie 10
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";
  
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

#ifdef DEVICE_SHELLYDIMMER_LANDING_CEILING
  #define DEVICENAME_CTR          "shellydimmer_landing"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Landing Room"
  
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER
  #define USE_MODULE_CORE_RULES

  #define DISABLE_SERIAL_LOGGING
    
  #define PIN_GPIO_SHELLY_DIMMER_RST_INV 5
  #define PIN_GPIO_SHELLY_DIMMER_BOOT0   4

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\"" D_GPIO_FUNCTION_HWSERIAL0_TX_CTR  "\","
      "\"3\":\"" D_GPIO_FUNCTION_HWSERIAL0_RX_CTR  "\","
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"14\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "Stairs"
  #define D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "Landing"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
        "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_DIMMER_FRIENDLY_NAME_LONG "\""
        "],"
        "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
          "\"" D_DEVICE_SWITCH_STAIRS_FRIENDLY_NAME_LONG "\","
          "\"" D_DEVICE_SWITCH_BATHROOM_FRIENDLY_NAME_LONG "\""
        "]"
    "}"
  "}";
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"StateChanged\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\"," 
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":50"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"StateChanged\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"ShellyDimmer\","
        "\"Function\":\"SetPower\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "}"
    

  "}";

#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Ensuite ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_ENSUITESENSOR
  #define DEVICENAME_CTR          "ensuitesensor"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
     
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BME
  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

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
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    #ifdef STRIP_SIZE_MAX
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    #else
    "\"" D_JSON_STRIP_SIZE       "\":50,"
    #endif //STRIP_SIZE_MAX
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":8"
    "},"
    "\"" D_JSON_CCT_TEMP "\":300,"
    "\"" D_JSON_HUE "\":25,"
    "\"" D_JSON_SAT "\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\"," //ie 10
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":1"
  "}";

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

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** vv ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/
















//  */


// #endif // user_mciaehl

#endif //header
