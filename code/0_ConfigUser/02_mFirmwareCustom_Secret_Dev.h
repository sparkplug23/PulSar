#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_H

/*********************************************************************************************\
 * Developing for home only: 
 *  - Devices to be put into home, but not yet ready for install. These are NOT "Devboards" (examples) but meant for actual developing of devices
\*********************************************************************************************/

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
 * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
 * This can also include lights or displays as notification/information systems. 
 * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
 * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
 * 
 * Ideas:
 * Create two basic LCD numbers, one for outside and one for bedroom temperature
 * Create world map (like I said to jess) that lights to show where the sun is, and day/night land-mass light up with cities being bright spots.
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
// #define DISABLE_WEBSERVER
// //#define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

//--------------------------------[Enable Device]-------------------------------------

/**
 * New devices
 * */
// #define DEVICE_RGBSTRING_DEVBOARD_SEGMENT
// #define DEVICE_RGBSTRING_DEVBOARD_SEGMENT_ESP32

/**
 *  SHORT TERM DEVICES IN BEDROOM -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- 
**/  
// #define DEVICE_RGBBEDLIGHT 
// #define DEVICE_RGBBEDROOMFLOOR
// #define DEVICE_RGBCLOCK_01
// #define DEVICE_RGBCLOCK_02 
// #define DEVICE_RGBFIREPLACE_TESTER 
// #define DEVICE_RGBDISPLAY_GARAGE   
// #define DEVICE_RGBSTRING_LIGHTS1
// #define DEVICE_RGBSTRING_LIGHTS2 //heart
// #define DEVICE_RGBSTRING_GARDEN_TREE_1
// #define DEVICE_BEDROOMBLINDS     
// #define DEVICE_DESKFAN
// #define DEVICE_DESKPANEL_01
// #define DEVICE_HVAC_BEDROOM
// #define DEVICE_HVAC_MASTERBEDROOM
// #define DEVICE_BEDROOM_PZEM_TESTER
// #define DEVICE_HVAC_KITCHEN
// #define DEVICE_HVAC_BEDROOM_RAD
// #define DEVICE_BEDROOM_CONTROLLER_BUTTONS_01
// #define DEVICE_HVAC_OIL_RADIATOR

/**
 *  DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- 
**/  
// #define DEVICE_CAM_TESTER
// #define DEVICE_CAMERA_DRIVEWAY
// #define DEVICE_H801_TESTER
// #define DEVICE_HEATING_ESP32_TESTER

/**
 *  LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- LIGHTING -- 
**/            
//#define DEVICE_RGBBEDLIGHT_TEST 
//#define DEVICE_RGBCUSTOM_USER_01
//#define DEVICE_RGBCUSTOM_USER_02
// #define DEVICE_RGBESP32_1_TESTER
// #define DEVICE_RGBGAZEBO_ROOF
// #define DEVICE_H801_SUNLIGHT
// #define DEVICE_H801_SUNLIGHT_2

/**
 *  CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- CONTROLLERS   -- 
**/
//#define DEVICE_KITCHENPANEL
//#define DEVICE_BLACKDOORBELL
// #define DEVICE_CAM_TESTER
// #define DEVICE_GPSPARSER_TESTER
// #define DEVICE_SDCARDWRITER_TESTER
// #define DEVICE_TREADMILL
// #define DEVICE_SDLOGGER1_TESTER

/**
 *  ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- ENERGY   -- 
**/
// #define DEVICE_PZEM_TESTER

/**
 *  SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- SENSOR   -- 
**/
// #define DEVICE_TESTSENSOR
// #define DEVICE_NODEMCU_TESTER
// #define DEVICE_ENSUITEFAN_SENSOR

/**
 *  DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- DISPLAYS   -- 
**/
// #define DEVICE_BEDROOMPANEL

/**
 *  SONOFF and lighting  -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
**/
//#define DEVICE_SONOFFTESTER

/**
 *  SONOFF iFANS
 * and lighting  -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- SONOFF   -- 
**/

/**
 *  SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- SHELLY   -- 
**/
// #define DEVICE_SHELLYDIMMER_GLOBE


/**
 * @brief To be developed totally in my room
 * 
 * >>> ESP32 Master <<< Understairs
 * 3 leds to similate heating
 * 1 BME (dining room side of wall)
 * 5 DS18XX
 * Nextion panel
 * 
 * >>> ESP32 Slave <<< Hotpress (rail mount)
 * 1 led as immersion
 * DS11XX from immersion (not including backup, keep those as another device, but only use a couple, move most to slave)
 * Nextion panel in hotpress for heating
 * BME masterbedroom
 * BME landing
 * 
 * >>> ESP32 Landing <<< Hotpress (rail mount)
 * Also used as backup immersion sensors (not important 3 sensor places)
 * Nextion panel? Is this really needed? a PIR in the roof would do the same job
 * PIR sensor
 * Masterbedroom sk6812 (rgbW)
 * Landing sk6812
 * 
 * 
 */
// #define DEVICE_LANDING
// #define DEVICE_HVAC_HOUSE_MASTER
// #define DEVICE_HVAC_HOUSE_SLAVE


// Include the home devices, which should ONLY contain final hardware
// #include "0_ConfigUser/00_mFirmwareCustom_Secret_Home.h"


//-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


/**
 * New heating controller, designed to work from single device to multizone system
 * For development of hvac with pzem to monitor power
 * 
 * Taken from testbeds for insitu development and openhab integration, this will remain the primary hvac dev code (unless I use a 4 LED tester)
 * Long USB will be used for always debugging
 * 
 * To include:
 *  - BME? If I could place this on the plug into the wall, it would be elevated heat sensing? with 1m cable
 *  - Optional Ds18 that can be added to oil heater
 *  - Relay output (requiring level shifter)
 *  - PZEM at 5v, via level shifter
 *  - Power with mains internal? (possible danger with sensors?) -- 
 *  - Since pzem, this needs to esp32, but good practive for house hvac
 * 
 * */
#ifdef DEVICE_HVAC_OIL_RADIATOR 
  #define DEVICENAME_CTR          "hvac_oil_radiator"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Oil Radiator with Energy Sensor"

  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  #define ENABLE_HVAC_DEBUG_PINS_FOR_TESTING

  #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER

  #define USE_MODULE_CONTROLLER_HVAC

  #define USE_DEVFEATURE_JSON_ADD_FLOAT_AS_OWN_FUNCTION

  
    /**
     * @brief 
     * Add special debug method here that will count loglevel by type so "error" messages can be counted and shared every X seconds on terminal
     * 
     */
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  

  #define ENABLE_DEVFEATURE_HVACTESTBED_CODE // for the testbed, so it does not ruin the heating code until functional

    #ifdef ENABLE_HVAC_DEBUG_PINS_FOR_TESTING
      #define HEATING_DEVICE_MAX 4
      #define HEATING_DEVICE_MAX 4
    #else  
      #define HEATING_DEVICE_MAX 1
      #define HEATING_DEVICE_MAX 1
    #endif // ENABLE_HVAC_DEBUG_PINS_FOR_TESTING

    #define ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS   

  
  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18X  // not working well on esp32, unreliable
  #define USE_MODULE_SENSORS_REMOTE_DEVICE

  #define REMOTE_SENSOR_1_MQTT_TOPIC "bedroomsensor/status/bme/+/sensors"
  #define REMOTE_SENSOR_JSON_NAME "Bedroom"
  
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_ENERGY_PZEM004T_V3
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #endif // USE_MODULE_ENERGY_PZEM004T_V3
      #ifdef USE_MODULE_SENSORS_BME
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif // USE_MODULE_SENSORS_BME
      #ifdef USE_MODULE_SENSORS_DS18X
      "\"19\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR     "\","
      #endif // USE_MODULE_SENSORS_DS18X
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"21\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      #ifdef ENABLE_HVAC_DEBUG_PINS_FOR_TESTING
      "\"25\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      #endif // ENABLE_HVAC_DEBUG_PINS_FOR_TESTING
      #endif // USE_MODULE_DRIVERS_RELAY
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""      
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "DriverZone0"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "DriverZone1"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "DriverZone2"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "DriverZone3"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Zone0"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Zone1"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Zone2"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Zone3"

  #define D_DEVICE_SENSOR_ZONE_0_NAME "BME0"
  #define D_DEVICE_SENSOR_ZONE_1_NAME "BME0"
  #define D_DEVICE_SENSOR_ZONE_2_NAME "BME0"
  #define D_DEVICE_SENSOR_ZONE_3_NAME REMOTE_SENSOR_JSON_NAME // tESTING REMOTE SENSOR VIA MQTT (LATER OPTIONS SHOULD INCLUDE DIRECT udp) "BME0"

  #define D_DEVICE_SENSOR_BME_0_NAME "BME0"
  #define D_DEVICE_SENSOR_BME_1_NAME "BME1"
  #define D_DEVICE_SENSOR_BME_2_NAME "BME2"
  #define D_DEVICE_SENSOR_BME_3_NAME "BME3"

  #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,170,67,3,30,19,2,25]"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,68,132,149,240,1,60,87]"



  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"

  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Cooker"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "16"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      // "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
      //   "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\"" //","
      //   // "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\""
      // "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      // "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":[" 
      //   D_DEVICE_SENSOR_DB18S20_0_ADDRESS //","
      //   // D_DEVICE_SENSOR_DB18S20_1_ADDRESS ""
      // "],"  
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS 
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_ZONE_0_NAME "\","
        "\"" D_DEVICE_SENSOR_ZONE_1_NAME "\"," // All use the same
        "\"" D_DEVICE_SENSOR_ZONE_2_NAME "\","
        "\"" D_DEVICE_SENSOR_ZONE_3_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Cooling\"" "]"
        "}"
      "]"
    "},"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "}"
  "}";
  
#endif



#ifdef DEVICE_RGBSTRING_DEVBOARD_SEGMENT
  #define DEVICENAME_CTR          "devboard_string_segment_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
    // #define USE_BUILD_TYPE_LIGHTING
    // #define USE_MODULE_LIGHTS_INTERFACE
    // #define USE_MODULE_LIGHTS_ANIMATOR
    // #define USE_MODULE_LIGHTS_ADDRESSABLE
    // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    // #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // // enable some wled conerted aniamtions
    // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define DEBUG_WLED_EFFECT_FUNCTIONS
    // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    // #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    // #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    // #define ENABLE_FREERAM_APPENDING_SERIAL

    // #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // #define ENABLE_CRGBPALETTES_IN_PROGMEM

    // #define STRIP_SIZE_MAX 100
    
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define DEBUG_WLED_EFFECT_FUNCTIONS
    #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #define ENABLE_FREERAM_APPENDING_SERIAL

    #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // to merge h801 rgbcct animation into the new method
    #define DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

    #define ENABLE_CRGBPALETTES_IN_PROGMEM

    #define STRIP_SIZE_MAX 1300
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS


    /**
     * @brief 
     * structs in headers
     * 13k to 18k
     * 
     */
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 
  #ifdef USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
 #define STRIP_SIZE_MAX 50
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #endif 
  #ifdef USE_DEVFEATURE_METHOD_WLED_BUILD
 #define STRIP_SIZE_MAX 50
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION  // to test existing effects in wled
    
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
    #define LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "WLED"
    #define DEBUG_WLED_EFFECT_FUNCTIONS
  #endif


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      //"\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 Segment command for single segment to replace old effects
 Without using a "Segment#" the commands will be assumed as for the entire strip and internally be segment0
 * 
 */
 #define USE_LIGHTING_TEMPLATE

//  #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS


  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":3000,"
      "\"RateMs\":10000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}

{
  "PixelRange": [
    0,
    49
  ],
  "AnimationMode": 3,
  "Strip": {
    "ClearTo": [
      0,
      0,
      0,
      0,0
    ],
    "ManualSetPixelToScene":[0,1,2,3,4,5,40,41,42,43,44,45,46,47,48,49]
  },
  "Effects": {
    "Function": 109
  },
  "ColourPalette": 10,
  "Hue": 20,
  "Sat": 100,
  "BrightnessCCT": 100,
  "CCT_TempPercentage": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 900,
    "RateMs": 20
  },
  "BrightnessRGB": 100
}

*/

#endif




#ifdef DEVICE_RGBSTRING_DEVBOARD_SEGMENT_ESP32
  #define DEVICENAME_CTR          "devboard_string_segment_01"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  

  #define USE_DEVFEATURE_METHOD_SEGMENTS_BUILD
  //#define USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
  // #define USE_DEVFEATURE_METHOD_WLED_BUILD
 
/**
 * @brief 
 * 
 * Outside show
 * 
 * 6 - red/green
 * 12
 * 31
 */

 
  #ifdef USE_DEVFEATURE_METHOD_SEGMENTS_BUILD

    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
    // enable some wled conerted aniamtions
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define DEBUG_WLED_EFFECT_FUNCTIONS
    #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

    #define ENABLE_DEVFEATURE_PHASE_OUT_LEDORDERARRAY
    #define ENABLE_DEVFEATURE_PHASE_OUT_ANIMATIONCOLOUR_STRUCT
    #define ENABLE_FREERAM_APPENDING_SERIAL

    #define  ENABLE_FEATURE_INCLUDE_WLED_PALETTES

    // to merge h801 rgbcct animation into the new method
    #define DISABLE_ANIMATION_COLOURS_FOR_RGBCCT_OLD_METHOD

    #define ENABLE_CRGBPALETTES_IN_PROGMEM

    #define STRIP_SIZE_MAX 1300

    #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  
    // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS

  #endif 
  #ifdef USE_DEVFEATURE_METHOD_HACS_LEGACY_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    
    #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #endif 
  #ifdef USE_DEVFEATURE_METHOD_WLED_BUILD
    #define USE_BUILD_TYPE_LIGHTING
    #define USE_MODULE_LIGHTS_INTERFACE
    #define USE_MODULE_LIGHTS_ANIMATOR
    #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION  // to test existing effects in wled
    
    // #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
    #define LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
    // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT
    #define D_EFFECT_INSIDE_TEMPLATE "WLED"
    #define DEBUG_WLED_EFFECT_FUNCTIONS
  #endif


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
      //"\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 Segment command for single segment to replace old effects
 Without using a "Segment#" the commands will be assumed as for the entire strip and internally be segment0
 * 
 */
//  #define STRIP_SIZE_MAX 1300
 #define USE_LIGHTING_TEMPLATE

//  #define LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS


  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":3000,"
      "\"RateMs\":10000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 21\"," 
    "\"Effects\":{"
      "\"Function\":30"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":1"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":30"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Static\""
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":3000,"
        "\"RateMs\":10000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":\"Solid RGBCCT\""
      "},"
      "\"ColourPalette\":\"Solid Rgbcct 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"Segment0\":{"
      "\"PixelRange\":[0,19],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":29"
      "},"
      "\"ColourPalette\":\"Christmas 09\","
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":23"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment1\":{"
      "\"PixelRange\":[20,29],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
      "\"Effects\":{"
        "\"Function\":28"
      "},"
      "\"ColourPalette\":11,"
      "\"Hue\":20,"
      "\"Sat\":90,"
      "\"Transition\":{"
        "\"TimeMs\":0,"
        "\"RateMs\":25"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"Segment2\":{"
      "\"PixelRange\":[30,49],"
      "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
      "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
      "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
      "\"Effects\":{"
        "\"Function\":\"Slow Glow\""
      "},"
      "\"ColourPalette\":\"Christmas 01\","
      "\"Transition\":{"
        "\"TimeMs\":500,"
        "\"RateMs\":1000"
      "},"    
      "\"BrightnessRGB\":100"
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"BrightnessRGB\":100"
  "}";
  #endif
  
  /** Copy from mqtt broker
   * 
   * {
  "Segment0": {
    "PixelRange": [
      31,
      40
    ],
    "Effects": {
      "Function":1
    },
    "ColourPalette": "Christmas 06",
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 1500,
      "RateMs": 2000
    },
    "BrightnessRGB": 100
  },
  "Segment1": {
    "PixelRange": [
      21,
      30
    ],
    "Effects": {
      "Function": 28
    },
    "ColourPalette": 11,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      0,
      20
    ],
    "Effects": {
      "Function": 29
    },
    "ColourPalette": "Christmas 06",
    "Hue": 120,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    },
    "BrightnessRGB": 100
  }
}


  {
  "Segment0": {
    "PixelRange": [
      40,
      49
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 20,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 23
    }
  },
  "Segment1": {
    "PixelRange": [
      0,
      39
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}


{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 10,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB":100
  },
    "Hue": 120,
    "Sat": 90,
  "Segment1": {
    "PixelRange": [
      11,
      49
    ],
    "ColourOrder":"grb",
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    }
  },
  "BrightnessRGB": 100
}

{
  "Segment0": {
    "PixelRange": [
      0,
      10
    ],
    "Effects": {
      "Function": 28,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 10,
    "Hue": 25,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 20
    },
    "BrightnessRGB": 10
  },
  "Segment1": {
    "PixelRange": [
      11,
      20
    ],
    "Effects": {
      "Function": 27,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 11,
    "Hue": 240,
    "Sat": 90,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  },
  "Segment2": {
    "PixelRange": [
      25,
      40
    ],
    "Effects": {
      "Function": 1,
      "Speed": 100,
      "Current": 127,
      "Palette": 0,
      "Intensity": 127,
      "Mode": 80
    },
    "ColourPalette": 14,
    "Hue": 120,
    "Sat": 100,
    "ColourOrder": "grbwc",
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "BrightnessRGB": 100
  }
}

*/

#endif

#ifdef DEVICE_BUCKET_WATER_LEVEL
  #define DEVICENAME_CTR          "bucketwatersensor"
  #define DEVICENAME_FRIENDLY_CTR "RGB Notifications 01"
  
  #define USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","

      "\"32\":\"" D_GPIO_FUNCTION_ADC1_CH4_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\""

    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";



  #define D_DEVICE_ADC_INPUT_NAME_LONG "WaterSensor"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_ADC_INPUT_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif





#ifdef DEVICE_PZEM_TESTER
  #define DEVICENAME_CTR          "pzem_tester"
  #define DEVICENAME_FRIENDLY_CTR "PZEM Tester"
  
//   //#define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 2

//   #define USE_BUILD_TYPE_ENERGY
//   #define USE_MODULE_SENSORS_PZEM004T_V3
//   #define MAX_ENERGY_SENSORS 2

//   // #define ENABLE_BUG_TRACING
  
//   #define USE_SOFTWARE_SERIAL_DEBUG
//   #define DISABLE_SERIAL_LOGGING //temp measure

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
//       "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
//       "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
//       "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Closed CT 100A"
//   #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Open CT 100A"
//   #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "Inline 100ohm 10A"
  
//   // Drivers, Sensors and lights?
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
//         "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\""
//       "]"
//     "}"
//   "}";
// #endif

  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_BUILD_TYPE_ENERGY
  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
  #define USE_DEVFEATURE_ENABLE_PZEM004T_SERIAL2

  // #define USE_MODULE_SENSORS_BME
  #define D_DEVICE_SENSOR_CLIMATE "Downstairs Toilet"

  #define ESP32

  // #define ENABLE_BUG_TRACING
  
  // #define USE_SOFTWARE_SERIAL_DEBUG
  //#define DISABLE_SERIAL_LOGGING //temp measure

  // Switching to esp32 for dual serial ports, to enable isolated pzem serial traffic until I can be assured I can disable serial0 on the esp8266

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // #ifdef ESP266
      //   #ifdef USE_MODULE_SENSORS_BME
      //   "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      //   "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      //   #endif
      //   "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      //   "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      //   "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
      //   "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
      // #endif //ESP266
      #ifdef ESP32
        #ifdef USE_MODULE_SENSORS_BME
        // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
        // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
        #endif
        // "\"1\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
        // "\"3\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
        // "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
        "\"LBI\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
      #endif //ESP32
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
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
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


#ifdef DEVICE_BEDROOM_PZEM_TESTER
  #define DEVICENAME_CTR          "bedroom_pzem"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom PZEM Tester"
  #define ESP32
    
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
  #define USE_DEVFEATURE_ENABLE_PZEM004T_SERIAL2

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "Mains"
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "Test"
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "24"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "25"
  #define ENERGY_SENSORS_DYNAMIC_TEMPFIX 2
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ""
      "]"  
    "},"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":2"    
    "}"
  "}";

#endif


//-----------------[User Defined Devices == USE_BUILD_TYPE_LIGHTING == RGB Lighting] ----------------------------


#ifdef DEVICE_CAM_TESTER 
  #define DEVICENAME_CTR          "cam_tester"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 Tester"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_ENERGY_INA219
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
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
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

#ifdef DEVICE_CAMERA_DRIVEWAY 
  #define DEVICENAME_CTR          "camera_driveway"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 Driveway"

  //#define FORCE_TEMPLATE_LOADING

  #define USE_MODULE_DRIVERS_CAMERA_OV2640
  
  // #define USE_MODULE_NETWORK_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      // #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
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

#ifdef DEVICE_CAMERA_2 
  #define DEVICENAME_CTR          "camera_2"
  #define DEVICENAME_FRIENDLY_CTR "Camera OV2640 2"

  //#define FORCE_TEMPLATE_LOADING

  #define USE_MODULE_DRIVERS_CAMERA_OV2640
  
  // #define USE_MODULE_NETWORK_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)   
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"    
      // #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
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



#ifdef DEVICE_RGBSTRING_LIGHTS1
  #define DEVICENAME_CTR            "rgbstring_lights1"
  #define DEVICENAME_FRIENDLY_CTR   "String Lights 1"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":9000,"
      "\"" D_JSON_RATE_MS "\":30000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":0"
  "}";

#endif



#ifdef DEVICE_RGBSTRING_LIGHTS2
  #define DEVICENAME_CTR            "rgbstring_lights2"
  #define DEVICENAME_FRIENDLY_CTR   "String Lights 2"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

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
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":9000,"
      "\"" D_JSON_RATE_MS "\":30000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";

#endif




#ifdef DEVICE_RGBSTRING_GARDEN_TREE_1
  #define DEVICENAME_CTR            "rgbstring_garden_tree1"
  #define DEVICENAME_FRIENDLY_CTR   "Garden Tree Lights 1"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":10,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":10"
  "}";

#endif


#ifdef DEVICE_TESTER_RGBW
  #define DEVICENAME_CTR            "tester_rgbw"
  #define DEVICENAME_FRIENDLY_CTR   "Tester RGBW WS2812 Lights"

  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

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
    "\"" D_JSON_COLOUR_PALETTE "\":\"" "User 01" "\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";

#endif


/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_BEDROOM
  #define DEVICENAME_CTR          "hvacbedroom"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Bedroom"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER
  
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_DHT
  // #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_REMOTE_DEVICE

  #define REMOTE_SENSOR_1_MQTT_TOPIC "bedroomsensor/status/bme/+/sensors"
  #define REMOTE_SENSOR_JSON_NAME "Bedroom"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","

      // "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      
      // "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR   "\""
      
      "\"D5\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_REMOTE_DEVICE_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_5_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";
  
#endif





/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_MASTERBEDROOM
  #define DEVICENAME_CTR          "hvac_masterbedroom"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Master Bedroom"
  #define DEVICENAME_ROOMHINT_CTR "Master Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HVAC_DEBUG_TIMES
  // #define DISABLE_WEBSERVER
  
  // #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  // #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  // #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define HEATING_DEVICE_MAX 1


  #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_DHT
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18X
  // #define USE_MODULE_SENSORS_REMOTE_DEVICE

  // #define REMOTE_SENSOR_1_MQTT_TOPIC "bedroomsensor/status/bme/+/sensors"
  // #define REMOTE_SENSOR_JSON_NAME "Bedroom"

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{" 
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\""
      // "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      // "\"D2\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"D5\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      // "\"D1\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\""
      // "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","

      // "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      
      // "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR   "\""
      
      // "\"D5\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  // #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  // #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";
  
#endif


/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_KITCHEN
  #define DEVICENAME_CTR          "hvac_kitchen"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Kitchen"

  #define HEATING_DEVICE_MAX 1

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  
  #define USE_MODULE_CONTROLLER_HVAC

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"   
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D2\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      #endif 
      "\"D6\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  // #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  // #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";

  
  //#define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":0"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":1}}\""
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":30}}\""
      "}"
    "}"
  "}";
  
#endif





/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_BEDROOM_RAD
  #define DEVICENAME_CTR          "hvac_bedroom_rad"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Bedroom Oil Rad"

  #define HEATING_DEVICE_MAX 1

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1

  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_CONTROLLER_HVAC

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"   
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR  "\","
      #endif 
      "\"D7\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Room"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Desk"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Test1"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Test2"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Room"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Desk"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Test1"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Test2"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Room_DHT"
  // #define D_DEVICE_SENSOR_DHT_1_NAME "Desk_DHT"

  // #define D_DEVICE_SENSOR_REMOTE_BME_BEDROOM_NAME "RemoteBedroomBME"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,194,102,202,187]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,135,126,242]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,195,135,215,193]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,205,206,170,25]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";

  
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":0"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOnSecs\\\":60}}\""
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"Switches\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"PowerState\\\":0}\""
      "}"
    "}"
  "}";
  
#endif



#ifdef DEVICE_HEATING_ESP32_TESTER
  #define DEVICENAME_CTR          "heating_esp32_tester"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Heating Gen 2"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP
  #define ENABLE_DEBUG_DELAYS

  #define DISABLE_WEBSERVER
  #define ESP32

  // #define USE_MODULE_CONTROLLER_HVAC
  
  #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_DS18X

  #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO1_SENSOR_COUNT 1
  #define ENABLE_DEVFEATURE_ESP32_FORCED_DB18S20_GPIO2_SENSOR_COUNT 1

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      // "\"12\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      // "\"14\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      // "\"27\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\","
      // "\"25\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      // "\"26\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"23\":\""  D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      // "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      // "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\"," //future bme
      // "\"23\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\"" //builtin led
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Immersion"
  #define D_DEVICE_DRIVER_RELAY_3_NAME "Boiler"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Immersion"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Upstairs_DHT"
  #define D_DEVICE_SENSOR_DHT_1_NAME "Downstairs_DHT"
//flip these
  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Downstairs_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,152,171,193,23,4,231]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Upstairs_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,131,6,194,23,4,59]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,204,226,193,23,4,30]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,136,105,53,22,4,114]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,50,176,193,23,4,197]"
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,216,108,53,22,4,102]"
  #define D_DEVICE_SENSOR_DB18S20_6_NAME        "Tank_Bottom"
  #define D_DEVICE_SENSOR_DB18S20_6_ADDRESS     "[40,255,162,167,53,22,4,27]"
  #define D_DEVICE_SENSOR_DB18S20_7_NAME        "Tank_Out"
  #define D_DEVICE_SENSOR_DB18S20_7_ADDRESS     "[40,255,219,93,53,22,4,239]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_6_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_7_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_5_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_6_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_7_ADDRESS ""
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DHT_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\",\"Cooling\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Cooling\"" "]"
        "}"
      "]"
    "}"
  "}";
  
#endif



#ifdef DEVICE_RGBMICRO1
  #define DEVICENAME_CTR          "rgbmicro1"
  #define DEVICENAME_FRIENDLY_CTR "Glass Box Lights"
  
  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 11
   
  
  // #define DISABLE_WEBSERVER

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
    // "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\"},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"Function\":1" //slow glow
    "},"
    "\"Transition\":{\"Order\":\"InOrder\",\"PixelUpdatePerc\":2,\"RateMs\":10000},"
    "\"TimeMs\":5000,"
    "\"" D_JSON_COLOUR_PALETTE "\":19," //c12    43 is the colours for this christmas
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";

#endif

#ifdef DEVICE_RGBMICRO2 //projector
  #define DEVICENAME_CTR          "rgbmicro2"
  #define DEVICENAME_FRIENDLY_CTR "Projector Micro Lights"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1//random(1,1000)

  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define ENABLE_DEVFEATURE_ARDUINOJSON

  // #define ENABLE_DEVFEATURE_BREAK_ADDLOG

  

//  #define USE_PUBSUB_V1

  //#define ENABLE_DEVFEATURE_MQTT_CONNECTION_EDIT1

  // #define ENABLE_BUG_TRACING

  // #define DISABLE_WEBSERVER

  //#define ENABLE_LOG_FILTERING_TEST_ONLY

  #define STRIP_SIZE_MAX 50//*15
  //#define ENABLE_PIXEL_LIGHTING_HARDWARE_WHITE_CHANNEL_CCT_SPACE
  //#define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // #define USE_INTERFACE_NEW

  // #define USE_MODULE_ENERGY_INA219
  // #define USE_MODULE_SENSORS_BME

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR

  #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL

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
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
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
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":9000,"
      "\"" D_JSON_RATE_MS "\":30000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";

#endif


/**
 * Using a RGB matrix, display a simple moving fire pattern
 * Aim is to get stability of esp32 animations 
 * */
#ifdef DEVICE_RGBFIREPLACE_TESTER
  #define DEVICENAME_CTR          "rgbfireplace_tester"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fire Place 32"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  #define DISABLE_WEBSERVER 
  // #define DISABLE_NETWORK
  // #define USE_MODULE_NETWORK_MQTT

  #define ENABLE_DEVFEATURE_DEBUG_GET_PIXEL_ZERO
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP
  // #define ENABLE_DEVFEATURE_BASIC_NEOPIXEL_TEST

  // #define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define STRIP_SIZE_MAX 300//256//100//256
  // #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 256
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
    "\"" D_JSON_TRANSITION       "\":{"
      // "\"" D_JSON_TIME_MS "\":2000,"
      // "\"" D_JSON_RATE_MS "\":5000,"
      "\"" D_JSON_TIME_MS "\":20,"
      "\"" D_JSON_RATE_MS "\":50,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "FirePlace01" "\""
      // "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Single Fire 01\","
    // "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":1"
  "}";

// "{HardwareType":"WS28XX","Transition":{"TimeMs":400,"RateMs":100,"PixelUpdatePerc":10,"Order":"Random"},"AnimationMode":"Effects","Effects":{"Function":"FirePlace01"},"ColourPalette":"Single Fire 01","BrightnessRGB":5}

#endif


#ifdef DEVICE_RGBESP32_1_TESTER
  #define DEVICENAME_CTR          "rgbesp32_1_tester"
  #define DEVICENAME_FRIENDLY_CTR "RGB Fire Place 32"

  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  // #define DISABLE_WEBSERVER 

  // #define ENABLE_DEVFEATURE_FLICKER_TESTING
  
  // #define DISABLE_NETWORK

  // #define ENABLE_DEVFEATURE_FLICKER_TESTING
  // #define ENABLE_DEVFEATURE_FLICKER_TESTING2
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  #define STRIP_SIZE_MAX 50//100//256
  // #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 256
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
      // "\"" D_JSON_TIME_MS "\":300,"
      // "\"" D_JSON_RATE_MS "\":100,"
      "\"" D_JSON_TIME_MS "\":4000,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      // "\"" D_JSON_FUNCTION "\":\"" "FirePlace01" "\""
      // "\"" D_JSON_FUNCTION "\":"  "1"
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    // "\"" D_JSON_COLOUR_PALETTE "\":\"Single Fire 01\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
    // "\"Brightness\":1"

    

    // command to enable random brightness from palette on generation
    // command to set the max and min of those
    // disable by defaultThanks
  "}";

#endif

#ifdef DEVICE_RGBGAZEBO_ROOF
  #define DEVICENAME_CTR          "rgbgazeboroof"
  #define DEVICENAME_FRIENDLY_CTR "rgbgazeboroof"

  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE

  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  // #define DISABLE_WEBSERVER 

  #define ESP32

  // #define ENABLE_DEVFEATURE_FLICKER_TESTING
  
  // #define DISABLE_NETWORK

  // #define ENABLE_DEVFEATURE_FLICKER_TESTING
  // #define ENABLE_DEVFEATURE_FLICKER_TESTING2
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  // #define STRIP_SIZE_MAX 50//100//256
  // // #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 256
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     // "\"" D_JSON_TIME_MS "\":300,"
  //     // "\"" D_JSON_RATE_MS "\":100,"
  //     "\"" D_JSON_TIME_MS "\":4000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":10,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     // "\"" D_JSON_FUNCTION "\":\"" "FirePlace01" "\""
  //     // "\"" D_JSON_FUNCTION "\":"  "1"
  //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
  //   "},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
  //   // "\"" D_JSON_COLOUR_PALETTE "\":\"Single Fire 01\","
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  //   // "\"Brightness\":1"

    

  //   // command to enable random brightness from palette on generation
  //   // command to set the max and min of those
  //   // disable by defaultThanks
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGB\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":1000,"
      "\"" D_JSON_RATE_MS "\":2000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
    "},"
    "\"" D_JSON_COLOUR_PALETTE "\":66,"
    "\"" D_JSON_BRIGHTNESS_RGB_255 "\":255"
  "}";


#endif



#ifdef DEVICE_RGBMICRO4 //gazebo
  #define DEVICENAME_CTR          "rgbmicro4"
  #define DEVICENAME_FRIENDLY_CTR "RGB Micro LED Lights 4"

  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 21

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
  // #define DISABLE_PIXEL_FUNCTION_EFFECTS
  // #define USE_MODULE_DRIVERS_LEDS
  #define DISABLE_WEBSERVER
  
//   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_ANALOG

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

  
  #define STRIP_SIZE_MAX 256

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
    "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
    "\"" D_JSON_BRIGHTNESS_RGB "\":10"
  "}";

#endif


#ifdef DEVICE_ENSUITEFAN_SENSOR
  #define DEVICENAME_CTR          "ensuitefan_sensor"
  #define DEVICENAME_FRIENDLY_CTR "ensuitefan Sensor"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
  // #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_SENSORS_INTERFACE
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
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "ExtractorEnsuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "ExtractorEnsuite"
  
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




#ifdef DEVICE_RGBDISPLAY_GARAGE
  #define DEVICENAME_CTR          "rgbdisplay_garage"
  #define DEVICENAME_FRIENDLY_CTR "RGB Display Garage"
 
  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK

  #define STRIP_SIZE_MAX 93// 750   *15 //changing gazebo to be 12
    
  #define DISABLE_WEBSERVER

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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":29"
    "},"
    // "\"CCT_Temp\": 152,"
    // "\"Hue\":25,"
    // "\"Sat\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":41,"
    "\"BrightnessCCT\":10,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"

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



#define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
#define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  #define ENABLE_PIXEL_FUNCTION_MIXER
  

  #define STRIP_SIZE_MAX 1000// 750   *15 //changing gazebo to be 12v

  #define DISABLE_WEBSERVER
  #define DISABLE_NETWORK
  // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING


  // Memory reduction techniques
  // #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 10 
  // #define WEB_LOG_SIZE 1
  // #define LOG_BUFFER_SIZE 500

  // #define DISABLE_SLEEP

  // #define USE_WS28XX_METHOD_RMT0_800KBPS_ESP32

  // #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES STRIP_SIZE_MAX
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
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"

  "}";

#endif


#ifdef DEVICE_RGBDININGROOMTREE 
  #define DEVICENAME_CTR          "rgbdiningroomtree"
  #define DEVICENAME_FRIENDLY_CTR "RGB Dining Room Tree"

 #define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME

  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  

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
      #if defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_SENSORS_BME)
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
    "\"" D_JSON_COLOUR_PALETTE "\":43," //c12    43 is the colours for this christmas
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
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
  #define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2


  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define ENABLE_BUG_TRACING
  #define DEBUG_PRINT_FUNCTION_NAME


  //#define ENABLE_LOG_FILTERING_TEST_ONLY
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  

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
    "\"" D_JSON_COLOUR_PALETTE "\": 43,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";


#endif






#ifdef DEVICE_H801_TESTER // for PWM dev
  #define DEVICENAME_CTR          "h801_tester"
  #define DEVICENAME_FRIENDLY_CTR "H801 Tester Strip"
    
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_WEBSERVER

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_PWM

  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
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
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":0,\"" D_JSON_RATE "\":20,\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":67,"
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":21},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

#endif



#ifdef DEVICE_H801_SUNLIGHT
  #define DEVICENAME_CTR          "h801_sunlight"
  #define DEVICENAME_FRIENDLY_CTR "H801 Sunlight"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1   

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM

  // #define USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE
  #define USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK // ie, use a function handler that will allow adding a function to enable a constructjson using a callback, debug handler only, as it will use too much memory up on finished product

  /**
   * Add extra option that makes the palette (or else brightness) more emphasis on the ends, ie change to colours in evening, but take less time in the morning to make transition
   * This will maybe need a different animation type, evening stay brighter, morning wait until after sunrise by X time then quickly ramp up birghtness by a time amount 
   * 
   * */

  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBcw\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      // "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
      "\"" D_JSON_FUNCTION "\":\"Sun Elevation RGBCCT Solid Palette 01\""
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

#ifdef DEVICE_H801_SUNLIGHT_2
  #define DEVICENAME_CTR          "h801_sunlight_2"
  #define DEVICENAME_FRIENDLY_CTR "H801 Sunlight 2"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1   

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM

  // // #define USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE

  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBwc\","
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      // "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
      "\"" D_JSON_FUNCTION "\":8"//\"Sun Elevation RGBCCT Solid Palette 01\""
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
    "\"" D_JSON_COLOUR_PALETTE "\":67,"
    "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  "}";
    
#endif


#ifdef DEVICE_RGBBEDROOMFLOOR
  #define DEVICENAME_CTR          "rgbbedroomfloor"
  #define DEVICENAME_FRIENDLY_CTR "H801 Bedroom Floor Light"
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2 

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM
  
  
  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
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
 

  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":67,"
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";
  
#endif


#ifdef DEVICE_RGBCRYSTAL2
  #define DEVICENAME_CTR            "rgbcrystal2"
  #define DEVICENAME_FRIENDLY_CTR   "Crystal Light Round 2"
  
  //#define FORCE_TEMPLATE_LOADING
  //#define SETTINGS_HOLDER 10
   
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


#ifdef DEVICE_TESTBUILDER
  #define DEVICENAME_CTR          "development_device"
  #define DEVICENAME_FRIENDLY_CTR "Development Device"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER random(1,10000)
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_DOOR
  // #define USE_SENSOR_DOOR_LOCK
  // #define DOORALERT_PAYLOAD_CTR "bedroom"

  // #define USE_MODULE_CONTROLLER_SONOFF_IFAN
  // #define USE_MODULE_DRIVERS_IRTRANSCEIVER

  // #define USE_MODULE_SENSORS_DS18X

  // #define USE_MODULE_SENSORS_MOTION
  // #define MOTIONALERT_PAYLOAD1_CTR "bedroom"
  
  // #define USE_MODULE_SENSORS_BME

  // #define USE_MODULE_SENSORS_BUTTONS

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  
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
      #ifdef USE_MODULE_SENSORS_DS18X
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
      "\"D4\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
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
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION   
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








#ifdef DEVICE_BEDROOMPANEL
  #define DEVICENAME_CTR            "bedroompanel"
  #define DEVICENAME_FRIENDLY_CTR   "Bedroom Control Panel"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // //#define ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION  // BREAKS
  // #define USE_SK6812_METHOD_DEFAULT

  
  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 4 //heating page

  #define USE_BUILD_TYPE_DISPLAY
  
  #define DISABLE_WEBSERVER

  // #define USE_MODULE_SENSORS_BME
  
  // #define USE_MODULE_SENSORS_MOTION

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
      
      "\"14\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\"," //1
      "\"12\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," //3

      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define STRIP_SIZE_MAX 40
  
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":10000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
  //   "},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  // "}";
  
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



#ifdef DEVICE_DESKPANEL_01
  #define DEVICENAME_CTR            "deskpanel_01"
  #define DEVICENAME_FRIENDLY_CTR   "Desk Panel 01 Motion Alerts"

  // //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // //#define ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION  // BREAKS
  // #define USE_SK6812_METHOD_DEFAULT

  
  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 9 //heating page

  // #define USE_BUILD_TYPE_DISPLAY
  
  // #define DISABLE_WEBSERVER

  // #define USE_MODULE_SENSORS_BME
  
  // #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_BME
      // "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      // #endif
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"D7\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      
      // "\"14\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\"," //1
      // "\"12\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," //3
      "\"17\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\"," //  SERIAL2
      "\"16\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," // SERIAL2

      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define STRIP_SIZE_MAX 40
  
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   #ifdef STRIP_SIZE_MAX
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   #else
  //   "\"" D_JSON_STRIP_SIZE       "\":50,"
  //   #endif //STRIP_SIZE_MAX
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":10000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
  //   "},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"Christmas MultiColoured Warmer\","
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  // "}";
  
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


#ifdef DEVICE_RGBCLOCK_01
  #define DEVICENAME_CTR            "rgbclock_01"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Clock 0l"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1
   
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  #define STRIP_SIZE_MAX 93
  
  #define DISABLE_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":1000,"
      "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
      "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
    "},"
    "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
    "\"" D_JSON_EFFECTS "\":{" 
      "\"" D_JSON_FUNCTION "\":29"
    "},"
    // "\"CCT_Temp\": 152,"
    // "\"Hue\":25,"
    // "\"Sat\":100,"
    "\"" D_JSON_COLOUR_PALETTE "\":41,"
    "\"BrightnessCCT\":10,"
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




#ifdef DEVICE_TESTSENSOR
  #define DEVICENAME_CTR            "landingpanel"
  #define DEVICENAME_FRIENDLY_CTR   "Landing Control Panel"

  //#define FORCE_TEMPLATE_LOADING
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


// #ifdef DEVICE_KITCHENPANEL //delete this
//   #define DEVICENAME_CTR          "kitchenpanel"
//   #define DEVICENAME_FRIENDLY_CTR "Kitchen Panel"

//   //#define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 2

//   #define USE_MODULE_DISPLAYS_NEXTION
//   #define NEXTION_DEFAULT_PAGE_NUMBER 4 //heating page

//   #define USE_BUILD_TYPE_DISPLAY

//   // #define ENABLE_BUG_TRACING
  
//   // #define USE_SOFTWARE_SERIAL_DEBUG
//   // #define DISABLE_SERIAL_LOGGING //temp measure

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       "\"1\":\""  D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
//       "\"3\":\""  D_GPIO_FUNCTION_NEXTION_RX_CTR "\"," 
//       "\"D0\":\""  D_GPIO_FUNCTION_LED1_INV_CTR   "\","  
//       "\"D4\":\""  D_GPIO_FUNCTION_LED1_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

// #endif






#ifdef DEVICE_NODEMCU_TESTER
  #define DEVICENAME_CTR          "nodemcu_tester"
  #define DEVICENAME_FRIENDLY_CTR "NodeMCU Tester"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
     
  // #define USE_MODULE_SENSORS_MOTION
  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES

  // #define USE_MODULE_SENSORS_BME

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
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
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
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



#ifdef DEVICE_BLACKDOORBELL
  #define DEVICENAME_CTR          "blackdoorbell"
  #define DEVICENAME_FRIENDLY_CTR "Black Doorbell"

  //#define FORCE_TEMPLATE_LOADING
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
      
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
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





#ifdef DEVICE_KITCHENLIGHT5
  #define DEVICENAME_CTR          "kitchenlight5"
  #define DEVICENAME_FRIENDLY_CTR "Kitchen Light 5 Shelly 2 desktester"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  // #define ENABLE_BUG_TRACING

  // #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_SWITCHES

  #define ENABLE_SONOFF_TEMPORARY_SHOW_LED_STATUS

  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
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
      "\"0\":\"" D_GPIO_FUNCTION_LED1_CTR "\""
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






// #ifdef DEVICE_SHELLYDIMMER_GLOBE
//   #define DEVICENAME_CTR          "shellydimmer_globe" //shelly1
//   #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer 1 Globe"
  
//   //#define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 2
  
//   #define USE_MODULE_SENSORS_SWITCHES

//   #define USE_MODULE_DRIVERS_SHELLY_DIMMER
//   #define DISABLE_SERIAL_LOGGING //temp measure

//   // #define USE_MODULE_SENSORS_MOTION
//   // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Top"
//   // #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Back Garden"
  
//   // #define USE_MODULE_CONTROLLER_SECURITY_LIGHT //remote controls for now

//   // #define USE_MODULE_DRIVERS_RELAY
//   // #define MAX_RELAYS 2
//   // #define USE_MODULE_DRIVERS_INTERFACE
    

//       #define PIN_GPIO_SHELLY_DIMMER_RST_INV 5
//       #define PIN_GPIO_SHELLY_DIMMER_BOOT0   4

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       //0,<100>,0,<101>,<174>,<175>,0,0,<6>,0,<6>,0,<10>,<148>
//       //0,    1,2,    3,    4,    5,9,10,11,12,13,14,15,16

//       //1=100, TX
//       //3=101, RX
//       "\"1\":\"" D_GPIO_FUNCTION_HWSERIAL0_TX_CTR  "\","
//       "\"3\":\"" D_GPIO_FUNCTION_HWSERIAL0_RX_CTR  "\","
//       //4=174, GPIO_SHELLY_DIMMER_BOOT0
//       //5=175, GPIO_SHELLY_DIMMER_RST_INV

//       //11=6, GPIO_SWT1_NP
//       //13=6, GPIO_SWT1_NP
//       #ifdef USE_MODULE_SENSORS_SWITCHES
//       "\"14\":\"" D_GPIO_FUNCTION_SWT1_NP_CTR  "\","
//       "\"12\":\"" D_GPIO_FUNCTION_SWT2_NP_CTR  "\","
//       #endif
//       //16=148, GPIO_ADC_TEMP
      
//       //15=10, GPIO_LED1_INV
//       "\"16\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Driveway Light"
//   #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Garden Light"
//   // #define SET_POWERONSTATE_AS_ON

//   // Drivers, Sensors and lights?
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     //device_names:{"module_name":["relay1","relay2"]}
//     "\"" D_JSON_DEVICENAME "\":{"
//         "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
//           "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
//           "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
//         "],"
//         "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
//           "\"" "Switch1" "\","
//           "\"" "Switch2" "\""
//         // "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//         //   "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
//         //   "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
//         "]"
//     "}"
//   "}";
// #endif




#ifdef DEVICE_DESKFAN
  #define DEVICENAME_CTR          "deskfan"
  #define DEVICENAME_FRIENDLY_CTR "Desk Fan"
    
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define ENABLE_DEVFEATURE_SHOW_INCOMING_MQTT_COMMANDS

  // Test hardware, receive bedroom sensor as remote device
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_REMOTE_DEVICE
  
  #define USE_MODULE_DRIVERS_PWM
  #define USE_MODULE_CONTROLLER_FAN
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_PWM
      "\"D1\":\"" D_GPIO_FUNCTION_FAN_PWM1_CTR   "\","
      #endif
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif


#ifdef DEVICE_EXERCISE_BIKE
  #define DEVICENAME_CTR          "exercise_bike"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff Basic Tester"
  #define USE_MODULE_SENSORS_PULSE_COUNTER
  #define USE_MODULE_CONTROLLER_EXERCISE_BIKE  
  
    pCONT_set->Settings.module_pins.io[D7] = GPIO_PULSE_COUNTER_ID;
#endif




/**
 *  DEVBOARDS   -- DEVBOARDS   -- DEVBOARDS   -- DEVBOARDS   -- DEVBOARDS   -- DEVBOARDS   -- DEVBOARDS   -- DEVBOARDS   -- DEVBOARDS   -- 
**/



// #ifdef DEVICE_DEVBOARD_SHELLY2P5_01
//   #define DEVICENAME_CTR          "devboard_shelly25_01"
//   #define DEVICENAME_FRIENDLY_CTR "Devboard Shelly 2.5 #01"

//   // #define USE_MODULE_SENSORS_MOTION
//   // #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Test"

//   // #define ENABLE_DEVFEATURE_RULES_MQTT_FASTER_SECS

//   // #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING
//   /*
//     Method should only activate if boot loop happens 10 times
//     Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
//     Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
//   */

//   //{"NAME":"Shelly 2.5","GPIO":[
    
//   /*
//   320,0,32,0,224,193,0,0,640,192,608,225,3456,4736
//   */
//   //],"FLAG":0,"BASE":18}
//   // #define USE_MODULE_SENSORS_MOTION   // to be added

//   // #define USE_MODULE_ENERGY_INTERFACE
//   // #define USE_MODULE_ENERGY_ADE7953

//   //#define FORCE_TEMPLATE_LOADING
//   // #define SETTINGS_HOLDER 2
  
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_BUTTONS
//   #define USE_MODULE_SENSORS_MOTION   //phasing out??

//   #define USE_MODULE_CORE_RULES

//   #define USE_MODULE_DRIVERS_RELAY
//   #define MAX_RELAYS 2
//   #define USE_MODULE_DRIVERS_INTERFACE

//   // #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS  
    
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
//   "}";

//   #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Power0"
//   #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Power1"
//   #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Motion0"
//   #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Motion1"
  
  
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     //device_names:{"module_name":["relay1","relay2"]}
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
//       "]"    
//     "},"
//     "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"},"
//     "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"}"
//   "}";


//   #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
//   DEFINE_PGM_CTR(RULES_TEMPLATE)
//   "{"
//     "\"Rule0\":{"
//       "\"Trigger\":{"
//         "\"Module\":\"Motion\","
//         "\"Function\":\"MotionStarted\","
//         "\"DeviceName\":0,"
//         "\"State\":2"
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         "\"Function\":\"SetPower\","
//         "\"DeviceName\":0,"
//         "\"State\":1,"
//         "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
//       "}"
//     "},"
//     "\"Rule1\":{"
//       "\"Trigger\":{"
//         "\"Module\":\"Motion\","
//         "\"Function\":\"MotionStarted\","
//         "\"DeviceName\":1,"
//         "\"State\":1"
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"Relays\","
//         "\"Function\":\"SetPower\","
//         "\"DeviceName\":1,"
//         "\"State\":1,"
//         "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
//       "}"
//     "}"
//   "}";



//   // #define USE_FUNCTION_TEMPLATE
//   // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   // "{"
//   //   "\"" D_JSON_DEVICENAME "\":{"
//   //     "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
//   //       "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
//   //       "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
//   //     "],"
//   //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//   //       "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
//   //       "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
//   //     "]"
//   //   "},"
//   //   "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D15:45:00\",\"OffTime\":\"00D00:00:00\"},"
//   //   "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D17:00:00\",\"OffTime\":\"00D00:00:00\"}"
//   // "}";

//   // #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
//   // DEFINE_PGM_CTR(RULES_TEMPLATE)
//   // "{"
//   //   "\"Rule0\":{"
//   //     "\"Trigger\":{"
//   //       "\"Module\":\"Switches\","
//   //       "\"Function\":\"StateChanged\","
//   //       "\"DeviceName\":0,"
//   //       "\"State\":2"
//   //     "},"
//   //     "\"Command\":{"
//   //       "\"Module\":\"Relays\","
//   //       "\"Function\":\"SetPower\","
//   //       "\"DeviceName\":0,"
//   //       "\"State\":2"
//   //     "}"
//   //   "},"
//   //   "\"Rule1\":{"
//   //     "\"Trigger\":{"
//   //       "\"Module\":\"Switches\","
//   //       "\"Function\":\"StateChanged\","
//   //       "\"DeviceName\":1,"
//   //       "\"State\":2"
//   //     "},"
//   //     "\"Command\":{"
//   //       "\"Module\":\"Relays\","
//   //       "\"Function\":\"SetPower\","
//   //       "\"DeviceName\":0,"
//   //       "\"State\":2"
//   //     "}"
//   //   "}"
//   // "}";

// //   // #define ENABLE_DEVFEATURE_PERIODIC_RULE_FILLING
// //   #define LOG_BUFFER_SIZE  1000

// //   #define USE_RULES_TEMPLATE // Rules, like the other templates, will be feed into the same command structure, so can actually be combined with `FUNCTION_TEMPLATE`
// //   DEFINE_PGM_CTR(RULES_TEMPLATE)
// //   "{"
// //     "\"Rule0\":{" //switch example
// //       "\"Trigger\":{"
// //       //module_id
// //         "\"Module\":\"Motion\","    //sensor
// //         //?
// //         "\"Function\":\"MotionStarted\"," //eg. InputChange (TemperatureThreshold)
// //         //[1]
// //         "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
// //         //[0]
// //         "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
// //       "},"
// //       "\"Command\":{"
// //         "\"Module\":\"Relays\","
// //         // set power?
// //         "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
// //         //1
// //         "\"DeviceName\":0," //number, name, or all
// //         //2
// //         "\"State\":1,"//setpower
// //         // Append other "normal" commands here? this would need storing
// //         // "\"JsonCommands\":\"{\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
// //         "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""

// //         //relay:timeon needs a way to specify which device
        
// //         // {"PowerName":0,"PowerState":1,"Relay":{"TimeOn":5}}


// //         //relay 
// //       "}"
// //     "},"
// //     "\"Rule1\":{" //switch example
// //       "\"Trigger\":{"
// //       //module_id
// //         "\"Module\":\"Motion\","    //sensor
// //         //?
// //         "\"Function\":\"MotionStarted\"," //eg. InputChange (TemperatureThreshold)
// //         //[1]
// //         "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
// //         //[0]
// //         "\"State\":1" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
// //       "},"
// //       "\"Command\":{"
// //         "\"Module\":\"Relays\","
// //         // set power?
// //         "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
// //         //1
// //         "\"DeviceName\":1," //number, name, or all
// //         //2
// //         "\"State\":1,"//setpower
// //         // Append other "normal" commands here? this would need storing
// //         // "\"JsonCommands\":\"{\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
        
// //         // "\"JsonCommands\":\"{\\\"PowerName\\\":5,\\\"Relay\\\":{\\\"TimeOn\\\":12}}\""
// //         // "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight5/set/relay\\\",\\\"Payload\\\":\\\"{\\\\\"PowerName\\\\\":0,\\\\\"PowerState\\\\\":2}\\\"}}\""
// //         // "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight5/set/relay\\\",\\\"Payload\\\":\\\"hello\\\"}\""
// //         "\"JsonCommands\":\"{\\\"MQTTSend\\\":{\\\"Topic\\\":\\\"kitchenlight4/set/relays\\\",\\\"Payload\\\":\\\"{~PowerName~:0,~PowerState~:2,~TimeOn~:10}\\\"}}\""
 
// // //  {"MQTTSend":
// // //   {
// // //     "Topic":"kitchenlight5/set/relay",
// // //     "Payload":
// // //       "{"PowerName":0,"PowerState":2}"
// // //   }
// // // }




// //         // "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":60}}\""
// //         //relay 
// //       "}"
// //     "}"
    

// //   "}";




// #endif



#ifdef DEVICE_ESP32_DEVKIT_BASIC
  #define DEVICENAME_CTR            "esp32_devkit_basic"
  #define DEVICENAME_FRIENDLY_CTR   "esp32_devkit_basic"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define ESP32

//   #define ENABLE_DEVFEATURE_PIN_FUNCTION_METHOD

//   #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST

//   #define ENABLE_SERIAL_FLUSH

//   #define USE_MODULE_DRIVERS_INTERFACE
//   #define USE_MODULE_DRIVERS_GPS

//   #define USE_MODULE_DRIVERS_SERIAL_UART
//   #define USE_DEVFEATURE_GPS_FROM_SERIAL2_BUFFER

//   //Interrupt method
//   // #define NMEAGPS_INTERRUPT_PROCESSING

//   #define ENABLE_UART2_ISR_BUFFERS

//   #define NEOGPS_USE_SERIAL1

//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED 192,168,1,116

//   #define USE_DEVFEATURE_RINGBUFFERS

//   // #define USE_MODULE_DRIVERS_SERIAL_UART

//   #define NMEAGPS_PARSE_SAVE_MILLIS
  
// #include <GPSport.h>

// // FEATURE
// // Create an "alert" mqtt topic that all devices will broadcast important info on

  #define USE_MODULE_DRIVERS_PWM
// #define ENABLE_DEVFEATURE_GPSTEST1

//   // #define DISABLE_WEBSERVER
  
// //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
// //   #define USE_MODULE_SENSORS_SWITCHES
// //   #define USE_MODULE_SENSORS_ANALOG

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"LBI\":\""  D_GPIO_FUNCTION_PWM1_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_ESP32_DEVKIT_BASIC


// #ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_MEASUREMENT_H
// #define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_MEASUREMENT_H

// /*********************************************************************************************\
//  * Devices used for work as measurements platform
// \*********************************************************************************************/

// // #define DEVICE_FORCED_TO_BE_TESTER
// #define DISABLE_WEBSERVER
// // //#define FORCE_TEMPLATE_LOADING

// #include "2_CoreSystem/mGlobalMacros.h"
// #include "2_CoreSystem/11_Languages/mLanguageDefault.h"
// #include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
// #include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"


// /**
//  * First measurement system, without esp adc until I can resolve that
//  * This will be used for first aplha tests to plan things out
//  * Future system will attempt to include esp adc later
//  * 
//  * Description: Use of button, oled and sdcard together on LIPO_ESP32
//  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
//  * Test version before adding pic32 stream of data (although will use same format)
//  * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
//  * 
//  * GPS @ 912800 baud recorded to local fix
//  * - each new fix, triggers a "save" flag
//  * OLED screen will show bytes written every second
//  * No networking
//  * 
//  * 
//  * 
//  * */

// // //--------------------------------[Enable Device]-------------------------------------

// /**
//  * PhD systems
//  * */
// // #define DEVICE_NODE0_UAV_PIC32ADC
// // #define DEVICE_NODE1_OFFUAV_PIC32ADC
// // #define DEVICE_NODE2_OFFUAV_PIC32ADC

// // #define DEVICE_NODE2R_OFFUAV_PIC32ADC_ESP32I2S

// //-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


// #ifdef DEVICE_NODE0_UAV_PIC32ADC
//   #define DEVICENAME_CTR            "node0"
//   #define DEVICENAME_FRIENDLY_CTR   "node0"  //white wire, blue tape, to be uav 
//   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "ND0"
//   #define DEVICENUM_NUM   0

//   #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
//   #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
//   #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

//   /**
//    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
//    * */
//   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
//   #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
//   #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
//   #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//   #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
//   #define USE_SYSTEM_LOGGER_CONTROLLER
//   #define USE_SYSTEM_SDCARD_LOGGING
//   //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER


//   // #define USE_CORE_ALTERNATE_WITH_SD_TO_CAPTURE_IMU

//   /**
//    * Debug methods
//    * */
//   /**
//    * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
//    * */
//   // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
//   // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

//   // Also defining so "HardwareSerial.cpp" will use these
//   #define RX1 18
//   #define TX1 19
//   #define RX2 16
//   #define TX2 17
//   #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

//   /**
//    * If enabled, disable normal logging methods
//    * */
//   #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
//     #define DISABLE_SERIAL_LOGGING
//     #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
//   #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

//   /**
//    * General defines to disable systems not needed
//    * */
//   #define DISABLE_NETWORK
//   #define DISABLE_SLEEP
//   #define ESP32
//   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

//   /**
//    *  GPS
//    * */
//   #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
//     #define USE_MODULE_DRIVERS_GPS
//     #define ENABLE_GPS_PARSER_NMEA
//     #define ENABLE_GPS_PARSER_UBX
//     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
//     #define NMEAGPS_DERIVED_TYPES
//     #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
//     #define NMEAGPS_PARSE_SAVE_MILLIS
//     #define gpsPort Serial1
//     #define D_GPS_BAUD_RATE_FAST    921600
//     #define D_GPS_BAUD_RATE_DEFAULT 9600
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_1
//     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

//   /**
//    * Comms with pic32
//    * */
//   #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_2
//     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
//     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
//     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
//     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

//   /**
//    * SDCard
//    * */
//   #ifdef USE_SYSTEM_SDCARD_LOGGING
//     #define USE_MODULE_DRIVERS_SDCARD
//     #define USE_SDCARD_RINGBUFFER_STREAM_OUT
//   #endif // USE_SYSTEM_SDCARD_LOGGING

//   /**
//    * Logger Controller
//    * */
//   #ifdef USE_SYSTEM_LOGGER_CONTROLLER
//     #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
//     #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
//     #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
//     #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
//   #endif // USE_SYSTEM_LOGGER_CONTROLLER

//   /**
//    * Button input
//    * */
//   #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
//     #define USE_MODULE_CORE_RULES
//     #define USE_MODULE_SENSORS_BUTTONS
//   #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

//   /**
//    * OLED display
//    * */
//   #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
//     #define USE_MODULE_DISPLAYS_INTERFACE
//     #define USE_MODULE_DISPLAYS_OLED_SSD1306
//     #define SHOW_SPLASH
//   #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

//   /**
//    * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
//    * */
//   #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_2
//     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
//     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
//     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
//     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
//   /**
//    * I2S Internal Fast Sampling
//    * */
//   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
//     // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
//     // // Next phase, will include saving adc values
//     // // #define USE_MODULE_SENSORS_ADC_INTERNAL
//     // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
//     // #define ENABLE_SMOOTHING_ON_ADC_READING
//     // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
//     // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
//     // // #define ADC_CAPTURE_EXTERNAL_PIN 32
//     // #define ADC_CAPTURE_INPUT0_PIN 34
//     // #define ADC_CAPTURE_INPUT1_PIN 35
//   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
  
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       /** 4P large JST - ADC
//        * Yellow     34(I), ADC2G, ADC1_CH6
//        * White      35(I), ADC5G, ADC1_CH7
//        * Red        32(I), ADC Record Trigger
//        * Black      GND
//        * */
//       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
//       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
//       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
//       /** 5P small - UART2 RSS Stream
//        * Orange      17, UART2_TX
//        * Yellow      16, UART2_RX
//        * White       25, ie superframe event over, rising edge interrupt
//        * Red         5V
//        * Black       GND
//        * */
//       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
//       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
//       /** 5P small - UART1 GPS Stream
//        * Orange      19, UART1_TX
//        * Yellow      18, UART1_RX
//        * White        
//        * Red         VCC, 3V3
//        * Black       GND
//        * */
//       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
//       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
//       /** 6P small - SD Card
//        * Green       15, CS
//        * Orange      14, SCK
//        * Yellow      13, MOSI
//        * White       12, MISO
//        * Red         3V3
//        * Black       GND
//        * */
//       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
//       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
//       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
//       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
//       /** Built in - OLED
//        * 
//        * */
//       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
//       /** 2P small
//        * Red        Button Logging Toggle
//        * Black      GND
//        * */
//       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
//     "},"
//   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   /**
//    * Debug pins
//    * */
//   // #define DEBUG_PIN1_GPIO     21
//   // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
//   // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
//   // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

//   // #define DEBUG_PIN2_GPIO     22
//   // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
//   // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
//   // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

//   // #define DEBUG_PIN3_GPIO     0 //USED
//   // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
//   // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
//   // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

//   // #define DEBUG_PIN4_GPIO     27
//   // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
//   // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
//   // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

//   // #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
//   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// #endif // DEVICE_GPSPARSER_TESTER



// #ifdef DEVICE_NODE1_OFFUAV_PIC32ADC
//   #define DEVICENAME_CTR            "node1"
//   #define DEVICENAME_FRIENDLY_CTR   "node1"  //white wire, blue tape, to be uav 
//   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "ND1"
//   #define DEVICENUM_NUM   1

// #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
// #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
// #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER


//   /**
//    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
//    * */
//   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
//   #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
//   #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
//   #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//   #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
//   #define USE_SYSTEM_LOGGER_CONTROLLER
//   #define USE_SYSTEM_SDCARD_LOGGING
//   //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

//   /**
//    * Debug methods
//    * */
//   /**
//    * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
//    * */
//   // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
//   // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

//   // Also defining so "HardwareSerial.cpp" will use these
//   #define RX1 18
//   #define TX1 19
//   #define RX2 16
//   #define TX2 17
//   #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

//   /**
//    * If enabled, disable normal logging methods
//    * */
//   #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
//     #define DISABLE_SERIAL_LOGGING
//     #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
//   #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

//   /**
//    * General defines to disable systems not needed
//    * */
//   #define DISABLE_NETWORK
//   #define DISABLE_SLEEP
//   #define ESP32
//   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

//   /**
//    *  GPS
//    * */
//   #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
//     #define USE_MODULE_DRIVERS_GPS
//     #define ENABLE_GPS_PARSER_NMEA
//     #define ENABLE_GPS_PARSER_UBX
//     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
//     #define NMEAGPS_DERIVED_TYPES
//     #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
//     #define NMEAGPS_PARSE_SAVE_MILLIS
//     #define gpsPort Serial1
//     #define D_GPS_BAUD_RATE_FAST    921600
//     #define D_GPS_BAUD_RATE_DEFAULT 9600
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_1
//     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

//   /**
//    * Comms with pic32
//    * */
//   #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_2
//     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
//     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
//     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
//     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

//   /**
//    * SDCard
//    * */
//   #ifdef USE_SYSTEM_SDCARD_LOGGING
//     #define USE_MODULE_DRIVERS_SDCARD
//     #define USE_SDCARD_RINGBUFFER_STREAM_OUT
//   #endif // USE_SYSTEM_SDCARD_LOGGING

//   /**
//    * Logger Controller
//    * */
//   #ifdef USE_SYSTEM_LOGGER_CONTROLLER
//     #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
//     #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
//     #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
//     #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
//   #endif // USE_SYSTEM_LOGGER_CONTROLLER

//   /**
//    * Button input
//    * */
//   #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
//     #define USE_MODULE_CORE_RULES
//     #define USE_MODULE_SENSORS_BUTTONS
//   #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

//   /**
//    * OLED display
//    * */
//   #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
//     #define USE_MODULE_DISPLAYS_INTERFACE
//     #define USE_MODULE_DISPLAYS_OLED_SSD1306
//     #define SHOW_SPLASH
//   #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

//   /**
//    * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
//    * */
//   #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_2
//     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
//     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
//     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
//     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
//   /**
//    * I2S Internal Fast Sampling
//    * */
//   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
//     // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
//     // // Next phase, will include saving adc values
//     // // #define USE_MODULE_SENSORS_ADC_INTERNAL
//     // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
//     // #define ENABLE_SMOOTHING_ON_ADC_READING
//     // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
//     // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
//     // // #define ADC_CAPTURE_EXTERNAL_PIN 32
//     // #define ADC_CAPTURE_INPUT0_PIN 34
//     // #define ADC_CAPTURE_INPUT1_PIN 35
//   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       /** 4P large JST - ADC
//        * Yellow     34(I), ADC2G, ADC1_CH6
//        * White      35(I), ADC5G, ADC1_CH7
//        * Red        32(I), ADC Record Trigger
//        * Black      GND
//        * */
//       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
//       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
//       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
//       /** 5P small - UART2 RSS Stream
//        * Orange      17, UART2_TX
//        * Yellow      16, UART2_RX
//        * White       25, ie superframe event over, rising edge interrupt
//        * Red         5V
//        * Black       GND
//        * */
//       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
//       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
//       /** 5P small - UART1 GPS Stream
//        * Orange      19, UART1_TX
//        * Yellow      18, UART1_RX
//        * White        
//        * Red         VCC, 3V3
//        * Black       GND
//        * */
//       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
//       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
//       /** 6P small - SD Card
//        * Green       15, CS
//        * Orange      14, SCK
//        * Yellow      13, MOSI
//        * White       12, MISO
//        * Red         3V3
//        * Black       GND
//        * */
//       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
//       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
//       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
//       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
//       /** Built in - OLED
//        * 
//        * */
//       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
//       /** 2P small
//        * Red        Button Logging Toggle
//        * Black      GND
//        * */
//       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
//     "},"
//   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   /**
//    * Debug pins
//    * */
//   #define DEBUG_PIN1_GPIO     21
//   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
//   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
//   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

//   #define DEBUG_PIN2_GPIO     22
//   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
//   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
//   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

//   #define DEBUG_PIN3_GPIO     0 //USED
//   #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
//   #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
//   #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

//   #define DEBUG_PIN4_GPIO     27
//   #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
//   #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
//   #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

//   #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
//   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// #endif // DEVICE_GPSPARSER_TESTER


// #ifdef DEVICE_NODE2_OFFUAV_PIC32ADC
//   #define DEVICENAME_CTR            "node2"
//   #define DEVICENAME_FRIENDLY_CTR   "node2"  //white wire, blue tape, to be uav 
//   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "ND2"
//   #define DEVICENUM_NUM   2

// #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
// #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
// #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER



//   /**
//    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
//    * */
//   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
//   #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
//   #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
//   #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//   #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
//   #define USE_SYSTEM_LOGGER_CONTROLLER
//   #define USE_SYSTEM_SDCARD_LOGGING
//   //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

//   /**
//    * Debug methods
//    * */
//   /**
//    * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
//    * */
//   // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
//   // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

//   // Also defining so "HardwareSerial.cpp" will use these
//   #define RX1 18
//   #define TX1 19
//   #define RX2 16
//   #define TX2 17
//   #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

//   /**
//    * If enabled, disable normal logging methods
//    * */
//   #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
//     #define DISABLE_SERIAL_LOGGING
//     #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
//   #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

//   /**
//    * General defines to disable systems not needed
//    * */
//   #define DISABLE_NETWORK
//   #define DISABLE_SLEEP
//   #define ESP32
//   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

//   /**
//    *  GPS
//    * */
//   #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
//     #define USE_MODULE_DRIVERS_GPS
//     #define ENABLE_GPS_PARSER_NMEA
//     #define ENABLE_GPS_PARSER_UBX
//     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
//     #define NMEAGPS_DERIVED_TYPES
//     #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
//     #define NMEAGPS_PARSE_SAVE_MILLIS
//     #define gpsPort Serial1
//     #define D_GPS_BAUD_RATE_FAST    921600
//     #define D_GPS_BAUD_RATE_DEFAULT 9600
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_1
//     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

//   /**
//    * Comms with pic32
//    * */
//   #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_2
//     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
//     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
//     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
//     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

//   /**
//    * SDCard
//    * */
//   #ifdef USE_SYSTEM_SDCARD_LOGGING
//     #define USE_MODULE_DRIVERS_SDCARD
//     #define USE_SDCARD_RINGBUFFER_STREAM_OUT
//   #endif // USE_SYSTEM_SDCARD_LOGGING

//   /**
//    * Logger Controller
//    * */
//   #ifdef USE_SYSTEM_LOGGER_CONTROLLER
//     #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
//     #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
//     #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
//     #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
//   #endif // USE_SYSTEM_LOGGER_CONTROLLER

//   /**
//    * Button input
//    * */
//   #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
//     #define USE_MODULE_CORE_RULES
//     #define USE_MODULE_SENSORS_BUTTONS
//   #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

//   /**
//    * OLED display
//    * */
//   #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
//     #define USE_MODULE_DISPLAYS_INTERFACE
//     #define USE_MODULE_DISPLAYS_OLED_SSD1306
//     #define SHOW_SPLASH
//   #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

//   /**
//    * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
//    * */
//   #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
//     #define USE_MODULE_DRIVERS_INTERFACE
//     #define USE_MODULE_DRIVERS_SERIAL_UART
//     #define ENABLE_HARDWARE_UART_2
//     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
//     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
//     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
//     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
//   #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
//   /**
//    * I2S Internal Fast Sampling
//    * */
//   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
//     // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
//     // // Next phase, will include saving adc values
//     // // #define USE_MODULE_SENSORS_ADC_INTERNAL
//     // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
//     // #define ENABLE_SMOOTHING_ON_ADC_READING
//     // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
//     // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
//     // // #define ADC_CAPTURE_EXTERNAL_PIN 32
//     // #define ADC_CAPTURE_INPUT0_PIN 34
//     // #define ADC_CAPTURE_INPUT1_PIN 35
//   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  
//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       /** 4P large JST - ADC
//        * Yellow     34(I), ADC2G, ADC1_CH6
//        * White      35(I), ADC5G, ADC1_CH7
//        * Red        32(I), ADC Record Trigger
//        * Black      GND
//        * */
//       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
//       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
//       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
//       /** 5P small - UART2 RSS Stream
//        * Orange      17, UART2_TX
//        * Yellow      16, UART2_RX
//        * White       25, ie superframe event over, rising edge interrupt
//        * Red         5V
//        * Black       GND
//        * */
//       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
//       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
//       /** 5P small - UART1 GPS Stream
//        * Orange      19, UART1_TX
//        * Yellow      18, UART1_RX
//        * White        
//        * Red         VCC, 3V3
//        * Black       GND
//        * */
//       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
//       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
//       /** 6P small - SD Card
//        * Green       15, CS
//        * Orange      14, SCK
//        * Yellow      13, MOSI
//        * White       12, MISO
//        * Red         3V3
//        * Black       GND
//        * */
//       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
//       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
//       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
//       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
//       /** Built in - OLED
//        * 
//        * */
//       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
//       /** 2P small
//        * Red        Button Logging Toggle
//        * Black      GND
//        * */
//       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
//     "},"
//   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

//   /**
//    * Debug pins
//    * */
//   // #define DEBUG_PIN1_GPIO     21
//   // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
//   // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
//   // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

//   // #define DEBUG_PIN2_GPIO     22
//   // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
//   // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
//   // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

//   // #define DEBUG_PIN3_GPIO     0 //USED
//   // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
//   // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
//   // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

//   // #define DEBUG_PIN4_GPIO     27
//   // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
//   // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
//   // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

//   // #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
//   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// #endif // DEVICE_GPSPARSER_TESTER

// // #ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_MEASUREMENT_CALIBRATION_H
// // #define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_MEASUREMENT_CALIBRATION_H

// // /*********************************************************************************************\
// //  * This file is one of two user configurable files
// //  * 
// //  * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
// //  * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
// //  * This can also include lights or displays as notification/information systems. 
// //  * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
// //  * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
// // \*********************************************************************************************/

// // // #define DEVICE_FORCED_TO_BE_TESTER
// // #define DISABLE_WEBSERVER
// // // //#define FORCE_TEMPLATE_LOADING

// // #include "2_CoreSystem/mGlobalMacros.h"
// // #include "2_CoreSystem/11_Languages/mLanguageDefault.h"
// // #include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
// // #include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"


// // /**
// //  * First measurement system, without esp adc until I can resolve that
// //  * This will be used for first aplha tests to plan things out
// //  * Future system will attempt to include esp adc later
// //  * 
// //  * Description: Use of button, oled and sdcard together on LIPO_ESP32
// //  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
// //  * Test version before adding pic32 stream of data (although will use same format)
// //  * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
// //  * 
// //  * GPS @ 912800 baud recorded to local fix
// //  * - each new fix, triggers a "save" flag
// //  * OLED screen will show bytes written every second
// //  * No networking
// //  * 
// //  * 
// //  * 
// //  * */

// // // //--------------------------------[Enable Device]-------------------------------------

// // /**
// //  * PhD systems -- CALIBRATION EDITIONS
// //  * 6 bytes into UART
// //  * when FF FF 2 bytes are received
// //  * sample esp32 adc, package into json
// //  * send to sd frame IF sd open
// //  * */
// // //#define DEVICE_NODE0_UAV_PIC32ADC_CALIBRATION
// // //#define DEVICE_NODE1_OFFUAV_PIC32ADC_CALIBRATION
// // //  #define DEVICE_NODE2_OFFUAV_PIC32ADC_CALIBRATION

// // // #define DEVICE_NODE2R_OFFUAV_PIC32ADC_ESP32I2S

// // //-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


// // #ifdef DEVICE_NODE0_UAV_PIC32ADC_CALIBRATION
// //   #define DEVICENAME_CTR            "node0"
// //   #define DEVICENAME_FRIENDLY_CTR   "node0"  //white wire, blue tape, to be uav 
// //   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "ND0"
// //   #define DEVICENUM_NUM   0

// //   #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
// //   #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
// //   #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

// //   #define USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1

// //   /**
// //    * ADC written directly into controller, not as a module
// //    * */
// //   #define USE_DEVFEATURE_ADC_IN_CONTROLLER

// //   // #define ENABLE_DEBUG_FUNCTION_NAMES

// //   /**
// //    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
// //    * */
// //   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
// //   #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
// //   #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
// //   #define USE_SYSTEM_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_SDCARD_LOGGING


// //   // #define USE_CORE_ALTERNATE_WITH_SD_TO_CAPTURE_IMU

// //   /**
// //    * Debug methods
// //    * */
// //   /**
// //    * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
// //    * */
// //   // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
// //   // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17
// //   #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

// //   /**
// //    * If enabled, disable normal logging methods
// //    * */
// //   #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
// //     #define DISABLE_SERIAL_LOGGING
// //     #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
// //   #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

// //   /**
// //    * General defines to disable systems not needed
// //    * */
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   /**
// //    *  GPS
// //    * */
// //   #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
// //     #define USE_MODULE_DRIVERS_GPS
// //     #define ENABLE_GPS_PARSER_NMEA
// //     #define ENABLE_GPS_PARSER_UBX
// //     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //     #define NMEAGPS_DERIVED_TYPES
// //     #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //     #define NMEAGPS_PARSE_SAVE_MILLIS
// //     #define gpsPort Serial1
// //     #define D_GPS_BAUD_RATE_FAST    921600
// //     #define D_GPS_BAUD_RATE_DEFAULT 9600
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_1
// //     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

// //   /**
// //    * Comms with pic32
// //    * */
// //   #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_2
// //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
// //   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

// //   /**
// //    * SDCard
// //    * */
// //   #ifdef USE_SYSTEM_SDCARD_LOGGING
// //     #define USE_MODULE_DRIVERS_SDCARD
// //     #define USE_SDCARD_RINGBUFFER_STREAM_OUT
// //   #endif // USE_SYSTEM_SDCARD_LOGGING

// //   /**
// //    * Logger Controller
// //    * */
// //   #ifdef USE_SYSTEM_LOGGER_CONTROLLER
// //     // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER // NOT TO USE, measurement system
// //     #define USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
// //     // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //     // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //     // #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
// //   #endif // USE_SYSTEM_LOGGER_CONTROLLER
// //   /**
// //    * Button input
// //    * */
// //   #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
// //     #define USE_MODULE_CORE_RULES
// //     #define USE_MODULE_SENSORS_BUTTONS
// //   #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

// //   /**
// //    * OLED display
// //    * */
// //   #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
// //     #define USE_MODULE_DISPLAYS_INTERFACE
// //     #define USE_MODULE_DISPLAYS_OLED_SSD1306
// //     #define SHOW_SPLASH
// //   #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

// //   /**
// //    * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
// //    * */
// //   #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_2
// //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
// //   #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
// //   /**
// //    * I2S Internal Fast Sampling
// //    * */
// //   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
// //     // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
// //     // // Next phase, will include saving adc values
// //     // // #define USE_MODULE_SENSORS_ADC_INTERNAL
// //     // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
// //     // #define ENABLE_SMOOTHING_ON_ADC_READING
// //     // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // // #define ADC_CAPTURE_EXTERNAL_PIN 32
// //     // #define ADC_CAPTURE_INPUT0_PIN 34
// //     // #define ADC_CAPTURE_INPUT1_PIN 35
// //   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   // #define DEBUG_PIN1_GPIO     21
// //   // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   // #define DEBUG_PIN2_GPIO     22
// //   // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   // #define DEBUG_PIN3_GPIO     0 //USED
// //   // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   // #define DEBUG_PIN4_GPIO     27
// //   // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   // #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER



// // #ifdef DEVICE_NODE1_OFFUAV_PIC32ADC_CALIBRATION
// //   #define DEVICENAME_CTR            "node1"
// //   #define DEVICENAME_FRIENDLY_CTR   "node1"  //white wire, blue tape, to be uav 
// //   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "CA1"
// //   #define DEVICENUM_NUM   1

// //   #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
// //   #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
// //   #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

// //   #define USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1

// //   /**
// //    * ADC written directly into controller, not as a module
// //    * */
// //   #define USE_DEVFEATURE_ADC_IN_CONTROLLER

// //   // #define ENABLE_DEBUG_FUNCTION_NAMES

// //   /**
// //    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
// //    * */
// //   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
// //   #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
// //   #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
// //   #define USE_SYSTEM_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_SDCARD_LOGGING

// //   /**
// //    * Debug methods
// //    * */
// //   /**
// //    * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
// //    * */
// //   // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
// //   // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17
// //   #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

// //   /**
// //    * If enabled, disable normal logging methods
// //    * */
// //   #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
// //     #define DISABLE_SERIAL_LOGGING
// //     #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
// //   #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

// //   /**
// //    * General defines to disable systems not needed
// //    * */
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   /**
// //    *  GPS
// //    * */
// //   #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
// //     #define USE_MODULE_DRIVERS_GPS
// //     #define ENABLE_GPS_PARSER_NMEA
// //     #define ENABLE_GPS_PARSER_UBX
// //     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //     #define NMEAGPS_DERIVED_TYPES
// //     #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //     #define NMEAGPS_PARSE_SAVE_MILLIS
// //     #define gpsPort Serial1
// //     #define D_GPS_BAUD_RATE_FAST    921600
// //     #define D_GPS_BAUD_RATE_DEFAULT 9600
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_1
// //     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

// //   /**
// //    * Comms with pic32
// //    * */
// //   #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_2
// //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
// //   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

// //   /**
// //    * SDCard
// //    * */
// //   #ifdef USE_SYSTEM_SDCARD_LOGGING
// //     #define USE_MODULE_DRIVERS_SDCARD
// //     #define USE_SDCARD_RINGBUFFER_STREAM_OUT
// //   #endif // USE_SYSTEM_SDCARD_LOGGING

// //   /**
// //    * Logger Controller
// //    * */
// //   #ifdef USE_SYSTEM_LOGGER_CONTROLLER
// //     // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER // NOT TO USE, measurement system
// //     #define USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
// //     // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //     // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //     // #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
// //   #endif // USE_SYSTEM_LOGGER_CONTROLLER

// //   /**
// //    * Button input
// //    * */
// //   #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
// //     #define USE_MODULE_CORE_RULES
// //     #define USE_MODULE_SENSORS_BUTTONS
// //   #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

// //   /**
// //    * OLED display
// //    * */
// //   #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
// //     #define USE_MODULE_DISPLAYS_INTERFACE
// //     #define USE_MODULE_DISPLAYS_OLED_SSD1306
// //     #define SHOW_SPLASH
// //   #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

// //   /**
// //    * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
// //    * */
// //   #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_2
// //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
// //   #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
// //   /**
// //    * I2S Internal Fast Sampling
// //    * */
// //   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
// //     // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
// //     // // Next phase, will include saving adc values
// //     // // #define USE_MODULE_SENSORS_ADC_INTERNAL
// //     // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
// //     // #define ENABLE_SMOOTHING_ON_ADC_READING
// //     // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // // #define ADC_CAPTURE_EXTERNAL_PIN 32
// //     // #define ADC_CAPTURE_INPUT0_PIN 34
// //     // #define ADC_CAPTURE_INPUT1_PIN 35
// //   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       // "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       // "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       // on uav device, but will consider the same for all
// //       "\"32\":\"" D_GPIO_FUNCTION_ADC1_CH4_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   // #define DEBUG_PIN1_GPIO     21
// //   // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   // #define DEBUG_PIN2_GPIO     22
// //   // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   // #define DEBUG_PIN3_GPIO     0 //USED
// //   // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   // #define DEBUG_PIN4_GPIO     27
// //   // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   // #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER



// // #ifdef DEVICE_NODE2_OFFUAV_PIC32ADC_CALIBRATION
// //   #define DEVICENAME_CTR            "node2"
// //   #define DEVICENAME_FRIENDLY_CTR   "node2"  //white wire, blue tape, to be uav 
// //   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "CA2"
// //   #define DEVICENUM_NUM   2

// //   #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
// //   #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
// //   #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER

// //   #define USE_DEVFEATURE_MEASUREMENT_SYSTEM_CALIBRATION_METHOD_1

// //   /**
// //    * ADC written directly into controller, not as a module
// //    * */
// //   #define USE_DEVFEATURE_ADC_IN_CONTROLLER

// //   // #define ENABLE_DEBUG_FUNCTION_NAMES

// //   /**
// //    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
// //    * */
// //   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
// //   #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
// //   #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
// //   #define USE_SYSTEM_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_SDCARD_LOGGING

// //   /**
// //    * Debug methods
// //    * */
// //   /**
// //    * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
// //    * */
// //   // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
// //   // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17
// //   #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

// //   /**
// //    * If enabled, disable normal logging methods
// //    * */
// //   #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
// //     #define DISABLE_SERIAL_LOGGING
// //     #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
// //   #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

// //   /**
// //    * General defines to disable systems not needed
// //    * */
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   /**
// //    *  GPS
// //    * */
// //   #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
// //     #define USE_MODULE_DRIVERS_GPS
// //     #define ENABLE_GPS_PARSER_NMEA
// //     #define ENABLE_GPS_PARSER_UBX
// //     #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //     #define NMEAGPS_DERIVED_TYPES
// //     #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //     #define NMEAGPS_PARSE_SAVE_MILLIS
// //     #define gpsPort Serial1
// //     #define D_GPS_BAUD_RATE_FAST    921600
// //     #define D_GPS_BAUD_RATE_DEFAULT 9600
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_1
// //     #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

// //   /**
// //    * Comms with pic32
// //    * */
// //   #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_2
// //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
// //   #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

// //   /**
// //    * SDCard
// //    * */
// //   #ifdef USE_SYSTEM_SDCARD_LOGGING
// //     #define USE_MODULE_DRIVERS_SDCARD
// //     #define USE_SDCARD_RINGBUFFER_STREAM_OUT
// //   #endif // USE_SYSTEM_SDCARD_LOGGING

// //   /**
// //    * Logger Controller
// //    * */
// //   #ifdef USE_SYSTEM_LOGGER_CONTROLLER
// //     // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER // NOT TO USE, measurement system
// //     #define USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
// //     // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //     // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //     // #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
// //   #endif // USE_SYSTEM_LOGGER_CONTROLLER

// //   /**
// //    * Button input
// //    * */
// //   #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
// //     #define USE_MODULE_CORE_RULES
// //     #define USE_MODULE_SENSORS_BUTTONS
// //   #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

// //   /**
// //    * OLED display
// //    * */
// //   #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
// //     #define USE_MODULE_DISPLAYS_INTERFACE
// //     #define USE_MODULE_DISPLAYS_OLED_SSD1306
// //     #define SHOW_SPLASH
// //   #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

// //   /**
// //    * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
// //    * */
// //   #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
// //     #define USE_MODULE_DRIVERS_INTERFACE
// //     #define USE_MODULE_DRIVERS_SERIAL_UART
// //     #define ENABLE_HARDWARE_UART_2
// //     #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //     #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //     #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //     #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
// //   #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
// //   /**
// //    * I2S Internal Fast Sampling
// //    * */
// //   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
// //     // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
// //     // // Next phase, will include saving adc values
// //     // // #define USE_MODULE_SENSORS_ADC_INTERNAL
// //     // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
// //     // #define ENABLE_SMOOTHING_ON_ADC_READING
// //     // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // // #define ADC_CAPTURE_EXTERNAL_PIN 32
// //     // #define ADC_CAPTURE_INPUT0_PIN 34
// //     // #define ADC_CAPTURE_INPUT1_PIN 35
// //   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       // "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       // "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       // on uav device, but will consider the same for all
// //       "\"32\":\"" D_GPIO_FUNCTION_ADC1_CH4_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   // #define DEBUG_PIN1_GPIO     21
// //   // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   // #define DEBUG_PIN2_GPIO     22
// //   // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   // #define DEBUG_PIN3_GPIO     0 //USED
// //   // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   // #define DEBUG_PIN4_GPIO     27
// //   // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   // #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER









// // #endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H





// // #ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_H
// // #define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_H

// // /*********************************************************************************************\
// //  * This file is one of two user configurable files
// //  * 
// //  * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
// //  * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
// //  * This can also include lights or displays as notification/information systems. 
// //  * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
// //  * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
// // \*********************************************************************************************/

// // // #define DEVICE_FORCED_TO_BE_TESTER
// // #define DISABLE_WEBSERVER
// // //#define FORCE_TEMPLATE_LOADING

// // #include "2_CoreSystem/mGlobalMacros.h"
// // #include "2_CoreSystem/11_Languages/mLanguageDefault.h"
// // #include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
// // #include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

// // // //--------------------------------[Enable Device]-------------------------------------

// // /**
// //  *  DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- DEV -- -- 
// // **/  
// // // #define DEVICE_GPSPARSER_TESTER_POLLING
// // // #define DEVICE_GPSPARSER_TESTER_INTERRUPT
// // // #define DEVICE_SDLOGGER1_TESTER
// // // #define DEVICE_UART_TESTER
// // // #define DEVICE_UART_GPSNMEA_TESTER
// // // #define DEVICE_UART_GPSUBX_TESTER
// // // #define DEVICE_SDCARD_TESTER
// // // #define DEVICE_GPS_TO_SDCARD_TESTER
// // // #define DEVICE_MEASUREMENT_SYSTEM_1
// // // #define DEVICE_OLED_WITH_SD_LIPO_TEST//this
// // // #define DEVICE_PIC32_RSS_STREAM_OUTPUT_TEST
// // // #define DEVICE_SDCARD_LIPO_TESTER
// // // #define DEVICE_GPS_SDCARD_LOGGER_WITH_OLED_TEST
// // // #define DEVICE_ADC_PIN_INTERRUPT_TRIGGER_TEST
// // // #define DEVICE_RECEIVE_RSS_STREAM_FROM_PIC32_INTO_ESP32_BUFFER_TEST
// // // #define DEVICE_I2S_ADC_TESTER
// // // #define DEVICE_DEVBOARD_I2S_MEASUREMENT
// // // #define DEVICE_DEVBOARD_I2S_MEASUREMENT_SINGLE_CHANNEL

// // /**
// //  * PhD systems
// //  * */
// // // #define DEVICE_MEASUREMENT_SYSTEM_ALPHA_1

// // // #include "0_ConfigUser/mFirmwareCustom_Secret_Home.h"


// // // //-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------


// // // /**
// // //  * Basic polling first, to get the GPS Parse to mqtt testing functional
// // //  * Advanced interrupt via buffers to parsing
// // //  * */

// // #ifdef DEVICE_GPSPARSER_TESTER_POLLING
// //   #define DEVICENAME_CTR            "gpsparser_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "GPS Parser Tester"

// //   //#define FORCE_TEMPLATE_LOADING

// //   #define USE_MODULE_DRIVERS_GPS
  
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #define USE_DEVFEATURE_GPS_POLLING_INPUT
// //   #define NMEAGPS_PARSE_SAVE_MILLIS
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER



// // // #ifdef DEVICE_GPSPARSER_TESTER_INTERRUPT
// // //   #define DEVICENAME_CTR            "gpsparser_tester"
// // //   #define DEVICENAME_FRIENDLY_CTR   "GPS Parser Tester"

// // //   //#define FORCE_TEMPLATE_LOADING
// // //   //#define SETTINGS_HOLDER 21
// // //   #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST

// // //   #define USE_MODULE_DRIVERS_GPS
// // //   // #define NMEAGPS_INTERRUPT_PROCESSING

// // //   #define USE_DEVFEATURE_RINGBUFFERS

// // //   // #define USE_MODULE_DRIVERS_SERIAL_UART

// // //   #define NMEAGPS_PARSE_SAVE_MILLIS
  
// // //   #define NEOGPS_USE_SERIAL1
// // // #include <GPSport.h>

// // // // FEATURE
// // // // Create an "alert" mqtt topic that all devices will broadcast important info on

// // // #define ENABLE_DEVFEATURE_GPSTEST1

// // //   // #define USE_BUILD_TYPE_LIGHTING
// // //   // #define USE_MODULE_LIGHTS_INTERFACE
// // //   // #define USE_MODULE_LIGHTS_ANIMATOR
// // //   // #define USE_MODULE_LIGHTS_ADDRESSABLE
// // //   // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
// // //   // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
// // //   // #define DISABLE_PIXEL_FUNCTION_EFFECTS
// // //   // #define USE_MODULE_DRIVERS_LEDS
// // //   #define DISABLE_WEBSERVER
  
// // // //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
// // // //   #define USE_MODULE_SENSORS_SWITCHES
// // // //   #define USE_MODULE_SENSORS_ANALOG

// // //   #define USE_MODULE_TEMPLATE
// // //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// // //   "{"
// // //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// // //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// // //     "\"" D_JSON_GPIOC "\":{"
// // //       "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
// // //     "},"
// // //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// // //   "}";

  
// // //   // #define STRIP_SIZE_MAX 100//256
// // //   // #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 256

// // //   // #define USE_LIGHTING_TEMPLATE
// // //   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
// // //   // "{"
// // //   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
// // //   //   #ifdef STRIP_SIZE_MAX
// // //   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
// // //   //   #else
// // //   //   "\"" D_JSON_STRIP_SIZE       "\":50,"
// // //   //   #endif //STRIP_SIZE_MAX
// // //   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
// // //   //   "\"" D_JSON_TRANSITION       "\":{"
// // //   //     "\"" D_JSON_TIME_MS "\":10000,"
// // //   //     "\"" D_JSON_RATE_MS "\":1000,"
// // //   //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
// // //   //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
// // //   //   "},"
// // //   //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
// // //   //   "\"" D_JSON_EFFECTS "\":{" 
// // //   //     "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
// // //   //   "},"
// // //   //   "\"" D_JSON_COLOUR_PALETTE "\":\"Single Fire 01\","//Christmas MultiColoured Warmer\","
// // //   //   "\"" D_JSON_BRIGHTNESS_RGB "\":2"
// // //   // "}";



// // // #endif // DEVICE_GPSPARSER_TESTER


// // // #ifdef DEVICE_SDCARDWRITER_TESTER
// // //   #define DEVICENAME_CTR            "sdcardwriter_tester"
// // //   #define DEVICENAME_FRIENDLY_CTR   "SD Card Writer Tester"

// // //   //#define FORCE_TEMPLATE_LOADING
// // //   //#define SETTINGS_HOLDER 21

// // //   #define USE_MODULE_DRIVERS_SDCARD

// // //   // #define USE_BUILD_TYPE_LIGHTING
// // //   // #define USE_MODULE_LIGHTS_INTERFACE
// // //   // #define USE_MODULE_LIGHTS_ANIMATOR
// // //   // #define USE_MODULE_LIGHTS_ADDRESSABLE
// // //   // #define USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
// // //   // #define WLED_DEFINE_GLOBAL_VARS //only in one source file, wled.cpp!
// // //   // #define DISABLE_PIXEL_FUNCTION_EFFECTS
// // //   // #define USE_MODULE_DRIVERS_LEDS
// // //   #define DISABLE_WEBSERVER
  
// // // //   //#define USE_WEBSERVER_ADVANCED_MULTIPAGES // new develop option to limit scope to only include root page while testing
  
// // // //   #define USE_MODULE_SENSORS_SWITCHES
// // // //   #define USE_MODULE_SENSORS_ANALOG

// // //   #define USE_MODULE_TEMPLATE
// // //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// // //   "{"
// // //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// // //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// // //     "\"" D_JSON_GPIOC "\":{"
// // //       "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
// // //     "},"
// // //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// // //   "}";

  
// // //   #define STRIP_SIZE_MAX 100//256
// // //   #define ENABLE_DEVFEATURE_REPEAT_SETPIXELOUT_MULTIPLE_TIMES 256

// // //   #define USE_LIGHTING_TEMPLATE
// // //   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
// // //   "{"
// // //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
// // //     #ifdef STRIP_SIZE_MAX
// // //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
// // //     #else
// // //     "\"" D_JSON_STRIP_SIZE       "\":50,"
// // //     #endif //STRIP_SIZE_MAX
// // //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
// // //     "\"" D_JSON_TRANSITION       "\":{"
// // //       "\"" D_JSON_TIME_MS "\":10000,"
// // //       "\"" D_JSON_RATE_MS "\":1000,"
// // //       "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
// // //       "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
// // //     "},"
// // //     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
// // //     "\"" D_JSON_EFFECTS "\":{" 
// // //       "\"" D_JSON_FUNCTION "\":\"" "Slow Glow" "\""
// // //     "},"
// // //     "\"" D_JSON_COLOUR_PALETTE "\":\"Single Fire 01\","//Christmas MultiColoured Warmer\","
// // //     "\"" D_JSON_BRIGHTNESS_RGB "\":2"
// // //   "}";



// // // #endif // DEVICE_GPSPARSER_TESTER



// // /**
// //  * 1) Serial0 is debugging, and will be disabled
// //  * 2) Serial1 is RSS samples in as superframes
// //  * 3) Serial1 is GPS input (9600)
// //  * 4) GPSParser using my hardware, as fast as possible
// //  * 5) OLED screen to give sd card status
// //  * 6) SDCard records superframe+gpsfix
// //  * 7) Need button to open/close sd (or dupont short? or small surface mount)
// //  * 
// //  * Configure dummy data, and feed it into both UART_GPS and UART_RSS simultaneously,
// //  * Echo both directly out of UART_RSS_TX onto putty, leave it working all the time (laptop side)
// //  * 
// //  * 
// //  * 1 - Check what gps can do, can I enter it into ublox? Does this improve speed, let me do manual decoding?
// //  * Test 1 - Create long wire for gps to sit in window and give active gps data
// //  * 
// //  * STEPS # - uncomment just the section below it, so I can quickly test configs as I go, only uncomment one step at a time
// //  * 
// //  * **/
// // #ifdef DEVICE_SDLOGGER1_TESTER
// //   #define DEVICENAME_CTR            "sdlogger1_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "SD & GPS Logger Tester"

// //   //#define FORCE_TEMPLATE_LOADING
// //   #define SETTINGS_HOLDER 1

// //   #define USE_MODULE_NETWORK_MQTT

// //   /**
// //    *  - Test Buttons
// //    * */
// //   // #define USE_MODULE_SENSORS_BUTTONS
// //   /**
// //    *  - Enable rule triggers
// //    * */
// //   // #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE
// //   // #define USE_MODULE_SENSORS_BUTTONS
// //   /**
// //    *  - Add triggered function in sdcard that will toggle sd later via button
// //    * */
// //   // #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE
// //   // #define USE_MODULE_SENSORS_BUTTONS
// //   // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
// //   /**
// //    *  - Enable displays, and show button pressed on it
// //    * */
// //   // #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE
// //   // #define USE_MODULE_SENSORS_BUTTONS
// //   // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306
// //   /**
// //    *  - Try sd open, basic write, close
// //    * */
// //   // #define USE_MODULE_DRIVERS_SDCARD
// //   /**
// //    *  - Putty Serial inputs into Serial1 buffer
// //    * */
// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #define ENABLE_UART2_ISR_BUFFERS
// //   /**
// //    *  - Get stable uart2 ringbuffer
// //    * */
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // // #define ENABLE_UART2_ISR_BUFFERS
// //   // #define ENABLE_UART_RINGBUFFERS
// //   // #define ENABLE_FEATURE_BLINK_ON_ISR_ACTIVITY
// //   /**
// //    *  - Read GPS Data into UART_GPS and UART_RSS (as dummy data)
// //    * */
// //   // #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE
// //   // #define USE_MODULE_SENSORS_BUTTONS
// //   // #define USE_MODULE_CONTROLLER_SDCARDLOGGER
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_UART2_ISR_BUFFERS
  
// // //   
// // //   #define USE_DEVFEATURE_GPS_FROM_SERIAL2_BUFFER
// // //   #define USE_DEVFEATURE_RINGBUFFERS
// // //   // #define USE_MODULE_DRIVERS_SERIAL_UART

// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"

// //     //https://randomnerdtutorials.com/esp32-adc-analog-read-arduino-ide/

// //     //   // OLED screen
// //     //   #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
// //     //   "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //     //   "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
// //     //   // "\"2\":\"" D_GPIO_FUNCTION_OLED_RESET_CTR   "\","
// //     //   #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

// //     //   #ifdef USE_MODULE_SENSORS_BUTTONS
// //     //   "\"22\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\","
// //     //   #endif 

// //     //   // 2 push buttons

// //     //   // SD card

// //     //   // GPS

// //     //   // Serial data logger (just receives into a buffer)
      
// //     //   "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //     //   "\"23\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","


// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","




// //     //   "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

  
// //   #define USE_FUNCTION_TEMPLATE
// //   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
// //   "{"
// //     // "\"" D_JSON_DEVICENAME "\":{"
// //     //   "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
// //     //     "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
// //     //   "],"
// //     //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
// //     //     "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
// //     //   "]"
// //     // "}"
    
// //     "\"" D_JSON_DISPLAY_MODE "\":6"
// //   "}";







// // #endif // 




// // // /**
// // //  * Basic polling first, to get the GPS Parse to mqtt testing functional
// // //  * Advanced interrupt via buffers to parsing
// // //  * */

// // #ifdef DEVICE_UART_TESTER
// //   #define DEVICENAME_CTR            "uart_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "UART Tester"

// //   #define ESP32

// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
  
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  1536000
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
// //       "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER


// // #ifdef DEVICE_UART_GPS_TESTER
// //   #define DEVICENAME_CTR            "uart_gps_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "UART GPS Tester"

// //   #define ESP32
  
// //   #define USE_MODULE_DRIVERS_GPS
  
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT
// //   #define NMEAGPS_PARSE_SAVE_MILLIS
// //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS

// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
// //       "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER


// // #ifdef DEVICE_UART_GPSUBX_TESTER
// //   #define DEVICENAME_CTR            "uart_gpsubx_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "UART GPS UBX Tester"

// //   #define ESP32
  
// //   #define USE_MODULE_DRIVERS_GPS

// //   #define DISABLE_NETWORK
  
// //   #define NMEAGPS_DERIVED_TYPES
// //   #define gpsPort Serial1

// //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT
// //   // #define NMEAGPS_PARSE_SAVE_MILLIS
// //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX

// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL

// //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT_UBX

// //   #define ENABLE_GPS_PARSER_UBX
  
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
// //       "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER



// // /**
// //  * Description: init dev of sdcard code, to open, write then close an SD card
// //  * Also adding time tests, writing of bytes bottleneck
// //  * */
// // #ifdef DEVICE_SDCARD_TESTER
// //   #define DEVICENAME_CTR            "sdcard_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "SDCard Tester"

// //   #define ESP32
  
// //   #define USE_MODULE_DRIVERS_SDCARD
// //   // #define USE_MODULE_CONTROLLER_SDCARDLOGGER

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
// //       "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER


// // /**
// //  * Description: gps via uart2 into ringbuffer, then ringbuffer into sdcard
// //  * */
// // #ifdef DEVICE_GPS_TO_SDCARD_TESTER
// //   #define DEVICENAME_CTR            "gps_to_sdcard_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "GPS to SDCard Tester"

// //   #define ESP32
  
// //   #define USE_MODULE_DRIVERS_GPS
  
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT
// //   #define NMEAGPS_PARSE_SAVE_MILLIS
// //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS

// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  9600

// //   #define USE_MODULE_DRIVERS_SDCARD
// //   #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
// //       "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER


// // /**
// //  * Description: init dev of sdcard code, to open, write then close an SD card
// //  * Also adding time tests, writing of bytes bottleneck
// //  * */
// // #ifdef DEVICE_SDCARD_LIPO_TESTER
// //   #define DEVICENAME_CTR            "sdcard_lipo_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "sdcard_lipo_tester Tester"

// //   #define ESP32
  
// //   #define USE_MODULE_DRIVERS_SDCARD
// //   // #define USE_MODULE_CONTROLLER_SDCARDLOGGER

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
// //       "\"21\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"22\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER




// // /**
// //  * Description: Use of button, oled and sdcard together on LIPO_ESP32
// //  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
// //  * */
// // #ifdef DEVICE_OLED_WITH_SD_LIPO_TEST
// //   #define DEVICENAME_CTR            "system_1_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   /** Next things to do
// //    * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
// //    * Add that the GPS can be init after a period of time with no updates, ie connected after
// //    * Add that a counter shows the last known time period of GPS signal, if none, alert me
// //    * Order cheap caps, to attach the hardware to
// //    * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
// //    * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
// //    * */

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // Section A: GPS
// //   #define USE_MODULE_DRIVERS_GPS
// //   #define ENABLE_GPS_PARSER_NMEA
// //   #define ENABLE_GPS_PARSER_UBX
// //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
// //   #define NMEAGPS_DERIVED_TYPES
// //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   #define NMEAGPS_PARSE_SAVE_MILLIS
// //   #define gpsPort Serial1
// //   #define D_GPS_BAUD_RATE_FAST 921600//230400//115200
// //   #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // Section UART
// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000

// //   // // Section RSS data
// //   // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

// //   // // Section B: SDCard driver
// //   #define USE_MODULE_DRIVERS_SDCARD

// //   // Seciton C: Logger controller
// //   #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //   #define ENABLE_SDLOGGER_APPEND_TIME_TEST

// //   // // Section x: Button to toggle logging/sd state
// //   // #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE
// //   // #define USE_MODULE_SENSORS_BUTTONS
// //   #define USE_MODULE_SENSORS_ADC_INTERNAL
// //   #define ENABLE_SMOOTHING_ON_ADC_READING
// //   #define ADC_CAPTURE_EXTERNAL_PIN 32
// //   #define ADC_CAPTURE_INPUT0_PIN 34
// //   #define ADC_CAPTURE_INPUT1_PIN 35

// //   // // Section x: OLED display 
// //   #define USE_MODULE_DISPLAYS_INTERFACE
// //   #define USE_MODULE_DISPLAYS_OLED_SSD1306


// //   // debug
// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /**
// //        * Debug pins
// //        * */

// // // Change this to be a 6pin between pic32 and esp32
// // /**
// //  * Shared power for stable IO
// //  * 3v3
// //  * ground
// //  * 
// //  * adc2
// //  * adc5
// //  * esp32 adc trigger
// //  * esp32 json completed
// //  * 
// //  * serial?
// //  * 
// //  * 
// //  * */

// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, 
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER


// // /**
// //  * Description: Use of button, oled and sdcard together on LIPO_ESP32
// //  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
// //  * Test version before adding pic32 stream of data (although will use same format)
// //  * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
// //  * 
// //  * GPS @ 912800 baud recorded to local fix
// //  * - each new fix, triggers a "save" flag
// //  * OLED screen will show bytes written every second
// //  * No networking
// //  * 
// //  * 
// //  * */
// // #ifdef DEVICE_GPS_SDCARD_LOGGER_WITH_OLED_TEST
// //   #define DEVICENAME_CTR            "system_1_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   /** Next things to do
// //    * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
// //    * Add that the GPS can be init after a period of time with no updates, ie connected after
// //    * Add that a counter shows the last known time period of GPS signal, if none, alert me
// //    * Order cheap caps, to attach the hardware to
// //    * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
// //    * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
// //    * */

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // Section A: GPS
// //   #define USE_MODULE_DRIVERS_GPS
// //   #define ENABLE_GPS_PARSER_NMEA
// //   #define ENABLE_GPS_PARSER_UBX
// //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
// //   #define NMEAGPS_DERIVED_TYPES
// //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   #define NMEAGPS_PARSE_SAVE_MILLIS
// //   #define gpsPort Serial1
// //   #define D_GPS_BAUD_RATE_FAST    921600//230400//115200
// //   #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // Section UART
// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000

// //   // // Section RSS data
// //   // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

// //   // // Section B: SDCard driver
// //   #define USE_MODULE_DRIVERS_SDCARD
// //   #define USE_SDCARD_RINGBUFFER_STREAM_OUT

// //   // Seciton C: Logger controller
// //   #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //   // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
// //   #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST

// //   // // Section x: Button to toggle logging/sd state
// //   #define USE_MODULE_CORE_RULES
// //   #define USE_MODULE_SENSORS_INTERFACE
// //   #define USE_MODULE_SENSORS_BUTTONS
// //   // Next phase, will include saving adc values
// //   // #define USE_MODULE_SENSORS_ADC_INTERNAL
// //   // #define ENABLE_SMOOTHING_ON_ADC_READING
// //   // #define ADC_CAPTURE_EXTERNAL_PIN 32
// //   // #define ADC_CAPTURE_INPUT0_PIN 34
// //   // #define ADC_CAPTURE_INPUT1_PIN 35

// //   // Section x: OLED display 
// //   #define USE_MODULE_DISPLAYS_INTERFACE
// //   #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /**
// //        * Debug pins
// //        * */

// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, 
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";



// //   // debug
// //   #define DEBUG_PIN1_GPIO     26
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     27
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   #define DEBUG_PIN3_GPIO     21
// //   #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   #define DEBUG_PIN4_GPIO     22
// //   #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// // #endif // DEVICE_GPSPARSER_TESTER



// // /**
// //  * Description: Use of button, oled and sdcard together on LIPO_ESP32
// //  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
// //  * Test version before adding pic32 stream of data (although will use same format)
// //  * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
// //  * 
// //  * GPS @ 912800 baud recorded to local fix
// //  * - each new fix, triggers a "save" flag
// //  * OLED screen will show bytes written every second
// //  * No networking
// //  * 
// //  * 
// //  * */
// // #ifdef DEVICE_ADC_PIN_INTERRUPT_TRIGGER_TEST
// //   #define DEVICENAME_CTR            "system_1_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

// //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   /** Next things to do
// //    * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
// //    * Add that the GPS can be init after a period of time with no updates, ie connected after
// //    * Add that a counter shows the last known time period of GPS signal, if none, alert me
// //    * Order cheap caps, to attach the hardware to
// //    * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
// //    * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
// //    * */

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // // Section A: GPS
// //   // #define USE_MODULE_DRIVERS_GPS
// //   // #define ENABLE_GPS_PARSER_NMEA
// //   // #define ENABLE_GPS_PARSER_UBX
// //   // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
// //   // #define NMEAGPS_DERIVED_TYPES
// //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   // #define NMEAGPS_PARSE_SAVE_MILLIS
// //   // #define gpsPort Serial1
// //   // #define D_GPS_BAUD_RATE_FAST    921600//230400//115200
// //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // // Section UART
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_HARDWARE_UART_1
// //   // #define ENABLE_HARDWARE_UART_2
// //   // #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
// //   // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000

// //   // // // Section RSS data
// //   // // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

// //   // // // Section B: SDCard driver
// //   // #define USE_MODULE_DRIVERS_SDCARD
// //   // #define USE_SDCARD_RINGBUFFER_STREAM_OUT

// //   // // Seciton C: Logger controller
// //   // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //   // // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
// //   // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST

// //   // // Section x: Button to toggle logging/sd state
// //   #define USE_MODULE_CORE_RULES
// //   #define USE_MODULE_SENSORS_INTERFACE
// //   #define USE_MODULE_SENSORS_BUTTONS
// //   // Next phase, will include saving adc values
// //   #define USE_MODULE_SENSORS_ADC_INTERNAL
// //   #define ENABLE_SMOOTHING_ON_ADC_READING
// //   #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //   #define ADC_CAPTURE_EXTERNAL_PIN 32
// //   #define ADC_CAPTURE_INPUT0_PIN 34
// //   #define ADC_CAPTURE_INPUT1_PIN 35

// //   // Section x: OLED display 
// //   #define USE_MODULE_DISPLAYS_INTERFACE
// //   #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /**
// //        * Debug pins
// //        * */

// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";



// //   // debug
// //   #define DEBUG_PIN1_GPIO     26
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     27
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   #define DEBUG_PIN3_GPIO     21
// //   #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   #define DEBUG_PIN4_GPIO     22
// //   #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// // #endif // DEVICE_GPSPARSER_TESTER



// // /**
// //  * Description: Serial2 will output stream as if it is the pic32, with entire superframe including:
// //  * - ramping analogwriter, so I can test that
// //  * - superframe of 50, with digital trigger lines for adc and when super frame is over
// //  * 
// //  * 
// //  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
// //  * */
// // #ifdef DEVICE_PIC32_RSS_STREAM_OUTPUT_TEST
// //   #define DEVICENAME_CTR            "pic32_emulator_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK

// //   /** Next things to do
// //    * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
// //    * Add that the GPS can be init after a period of time with no updates, ie connected after
// //    * Add that a counter shows the last known time period of GPS signal, if none, alert me
// //    * Order cheap caps, to attach the hardware to
// //    * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
// //    * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
// //    * */
// //   // #define DISABLE_SERIAL0_CORE
// //   #define DISABLE_SLEEP

// //   // General defines needed in release version
// //   #define ESP32
// //   // #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   #define USE_MODULE_CONTROLLER_USERMOD_01

// //   // // Section A: GPS
// //   // #define USE_MODULE_DRIVERS_GPS
// //   // #define ENABLE_GPS_PARSER_NMEA
// //   // #define ENABLE_GPS_PARSER_UBX
// //   // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
// //   // #define NMEAGPS_DERIVED_TYPES
// //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   // #define NMEAGPS_PARSE_SAVE_MILLIS
// //   // #define gpsPort Serial1
// //   // #define D_GPS_BAUD_RATE_FAST 921600//230400//115200
// //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // Section UART
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_HARDWARE_UART_1
// //   // #define ENABLE_HARDWARE_UART_2
// //   // #define HARDWARE_UART_1_BAUD_RATE_SPEED  D_GPS_BAUD_RATE_FAST
// //   // #define HARDWARE_UART_2_BAUD_RATE_SPEED  115200

// //   // // Section RSS data
// //   // #define ENABLE_DEVFEATURE_DUMMY_RSS_DATA

// //   // // Section B: SDCard driver
// //   // #define USE_MODULE_DRIVERS_SDCARD

// //   // // Seciton C: Logger controller
// //   // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //   // #define ENABLE_SDLOGGER_APPEND_TIME_TEST

// //   // // Section x: Button to toggle logging/sd state
// //   // #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE
// //   // #define USE_MODULE_SENSORS_BUTTONS

// //   // // Section x: OLED display 
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /**
// //        * Debug pins
// //        * */
// //       "\"22\":\"" D_GPIO_FUNCTION_PWM1_CTR   "\","
// //       "\"2\":\"" D_GPIO_FUNCTION_PWM2_CTR   "\","

// //     //   /** 4P large JST - ADC
// //     //    * Yellow     34(I), ADC2G, ADC1_CH6
// //     //    * White      35(I), ADC5G, ADC1_CH7
// //     //    * Red        32(I), ADC Record Trigger
// //     //    * Black      GND
// //     //    * */
// //     //   "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //     //   "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //     //   "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

// //     //   /** 5P small - UART2 RSS Stream
// //     //    * Orange      17, UART2_TX
// //     //    * Yellow      16, UART2_RX
// //     //    * White       25, 
// //     //    * Red         5V
// //     //    * Black       GND
// //     //    * */
// //     //   "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //     //   "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

// //     //   /** 5P small - UART1 GPS Stream
// //     //    * Orange      19, UART1_TX
// //     //    * Yellow      18, UART1_RX
// //     //    * White        
// //     //    * Red         VCC, 3V3
// //     //    * Black       GND
// //     //    * */
// //     //   "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //     //   "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

// //     //   /** 6P small - SD Card
// //     //    * Green       15, CS
// //     //    * Orange      14, SCK
// //     //    * Yellow      13, MOSI
// //     //    * White       12, MISO
// //     //    * Red         3V3
// //     //    * Black       GND
// //     //    * */
// //     //   "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
// //     //   "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
// //     //   "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
// //     //   "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

// //     //   /** Built in - OLED
// //     //    * 
// //     //    * */
// //     //   "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //     //   "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
// //     //   /** 2P small
// //     //    * Red        Button Logging Toggle
// //     //    * Black      GND
// //     //    * */
// //     //   "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER


// // /**
// //  * Description: gps via uart2 into ringbuffer, then ringbuffer into sdcard
// //  * */
// // #ifdef DEVICE_MEASUREMENT_SYSTEM_1
// //   #define DEVICENAME_CTR            "measurement_system_1_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "MEASUREMENT_SYSTEM_1 Tester"

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK

// //   // General defines needed in release version
// //   #define ESP32

// //   // Section A: GPS
// //   #define USE_MODULE_DRIVERS_GPS
// //   #define ENABLE_GPS_PARSER_UBX
// //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW1
// //   // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW2
// //   // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW3
// //   // #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW4
// //   #define ENABLE_GPS_DEVICE_CONFIG_SPEED_SLOW5
// //   #define NMEAGPS_DERIVED_TYPES
// //   #define gpsPort Serial1
// //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   #define NMEAGPS_PARSE_SAVE_MILLIS

// //   // Section UART
// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  9600
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  115200//921600

// //   // Section B: SDCard driver
// //   // #define USE_MODULE_DRIVERS_SDCARD

// //   // Seciton C: Logger controller
  
  
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define USE_DEVFEATURE_GPS_POLLING_INPUT

// //   // #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE
// //   // #define USE_MODULE_SENSORS_BUTTONS
  
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306

// //   // #define ENABLE_BUG_TRACING
// //   // #define DEBUG_NUM1

// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING



// //   // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER

// //   #define DEBUG_PIN1_GPIO     12
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     13
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       #ifdef USE_MODULE_SENSORS_BUTTONS
// //       "\"22\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\","
// //       #endif 
      
// //       // OLED screen
// //       #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
// //       // "\"2\":\"" D_GPIO_FUNCTION_OLED_RESET_CTR   "\","
// //       #endif // USE_MODULE_DISPLAYS_OLED_SSD1306
// //       "\"12\":\"" D_GPIO_FUNCTION_DEBUG_PIN1_CTR   "\","
// //       "\"26\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"27\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// // #endif // DEVICE_GPSPARSER_TESTER


// // /**
// //  * Description: Use of button, oled and sdcard together on LIPO_ESP32
// //  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
// //  * Test version before adding pic32 stream of data (although will use same format)
// //  * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
// //  * 
// //  * GPS @ 912800 baud recorded to local fix
// //  * - each new fix, triggers a "save" flag
// //  * OLED screen will show bytes written every second
// //  * No networking
// //  * 
// //  * 
// //  * */
// // #ifdef DEVICE_RECEIVE_RSS_STREAM_FROM_PIC32_INTO_ESP32_BUFFER_TEST
// //   #define DEVICENAME_CTR            "system_1_tester"
// //   #define DEVICENAME_FRIENDLY_CTR   "SYSTEM_1 Tester"

// //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   /** Next things to do
// //    * Completely make it so it can compile without NTP, wifi, mqtt and telemetry
// //    * Add that the GPS can be init after a period of time with no updates, ie connected after
// //    * Add that a counter shows the last known time period of GPS signal, if none, alert me
// //    * Order cheap caps, to attach the hardware to
// //    * Use a GPIO to trigger interrupt, which will actually inform when to record adc readings.. these will be filtered into vector...
// //    * I will need a GPIO_SuperFrame reset and GPIO_RSS_ADC measure to properly know when a packet is ready. The start of a new SF will trigger sending/saving latest completed SF frame, OR, every 1 second to keep the buffers clear
// //    * */

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // // Section A: GPS
// //   // #define USE_MODULE_DRIVERS_GPS
// //   // #define ENABLE_GPS_PARSER_NMEA
// //   // #define ENABLE_GPS_PARSER_UBX
// //   // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // #define NMEAGPS_DERIVED_TYPES
// //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   // #define NMEAGPS_PARSE_SAVE_MILLIS
// //   // #define gpsPort Serial1
// //   // #define D_GPS_BAUD_RATE_FAST    921600//230400//115200
// //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // // Section UART
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_HARDWARE_UART_1
// //   // #define ENABLE_HARDWARE_UART_2
// //   // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //   // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //   // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //   // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

// //   // // Section B: SDCard driver
// //   // #define USE_MODULE_DRIVERS_SDCARD
// //   // #define USE_SDCARD_RINGBUFFER_STREAM_OUT

// //   // // // Seciton C: Logger controller
// //   // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //   // // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
// //   // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //   // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //   // #define GPIO_SYNC_FRAME_ISR_PIN 25
// //   // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
// //   // #define GPIO_ADC_CAPTURE_ISR_PIN 32

// //   // // Section x: Button to toggle logging/sd state
// //   #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
// //   #define USE_MODULE_SENSORS_BUTTONS
// //   // Next phase, will include saving adc values
// //   // #define USE_MODULE_SENSORS_ADC_INTERNAL
// //   #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
// //   #define ENABLE_SMOOTHING_ON_ADC_READING
// //   #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //   #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //   // #define ADC_CAPTURE_EXTERNAL_PIN 32
// //   #define ADC_CAPTURE_INPUT0_PIN 34
// //   #define ADC_CAPTURE_INPUT1_PIN 35

// //   // // Section x: OLED display 
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /**
// //        * Debug pins
// //        * */

// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       // "\"32\":\"" D_GPIO_FUNCTION_GENERIC_GPIO1_CTR   "\","

// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","

// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","

// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR  "\","

// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";



// //   // debug
// //   #define DEBUG_PIN1_GPIO     21
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     22
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   #define DEBUG_PIN3_GPIO     26
// //   #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   #define DEBUG_PIN4_GPIO     27
// //   #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER




// // /**
// //  * I2S ADC Tester
// //  * 
// //  * */
// // #ifdef DEVICE_I2S_ADC_TESTER
// //   #define DEVICENAME_CTR            "system_1_alpha"
// //   #define DEVICENAME_FRIENDLY_CTR   "Device 0"

// //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600

// //   // General defines for debugging only
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   // Section ADC: High speed internal ADC
// //   // #define ENABLE_ESP32_ADC_SAMPLING
// //   //   #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
// //   //   #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //   //   #define ADC_CAPTURE_EXTERNAL_PIN 32 // Trigger sample period
// //   //   #define ADC_CAPTURE_INPUT0_PIN 34
// //   //   #define ADC_CAPTURE_INPUT1_PIN 35

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   // // Section UART
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_HARDWARE_UART_1
// //   // #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000



// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   #define DEBUG_PIN1_GPIO     21
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     22
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   #define DEBUG_PIN3_GPIO     26
// //   #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   #define DEBUG_PIN4_GPIO     27
// //   #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER



// // /**
// //  * Test of high speed I2S measurements
// //  * Record 1000 samples per syncframe (then sum the regions that would be measurements, or try offload for summing later in code)
// //  * On finish of sync, average the sample rate down to 10 times more than actual measurement rate, save these to sd if not too many
// //  * 
// //  * 
// //  * */
// // #ifdef DEVICE_DEVBOARD_I2S_MEASUREMENT
// //   #define DEVICENAME_CTR            "system_1_alpha"

// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

// // /**
// //  * SDCARD name
// //  * */

// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


// //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // // Section A: GPS
// //   // #define USE_MODULE_DRIVERS_GPS
// //   // #define ENABLE_GPS_PARSER_NMEA
// //   // #define ENABLE_GPS_PARSER_UBX
// //   // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // #define NMEAGPS_DERIVED_TYPES
// //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   // #define NMEAGPS_PARSE_SAVE_MILLIS
// //   // #define gpsPort Serial1
// //   // #define D_GPS_BAUD_RATE_FAST    921600
// //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // // Section UART
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_HARDWARE_UART_1
// //   // #define ENABLE_HARDWARE_UART_2
// //   // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //   // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //   // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //   // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

// //   // // Section B: SDCard driver
// //   // #define USE_MODULE_DRIVERS_SDCARD
// //   // #define USE_SDCARD_RINGBUFFER_STREAM_OUT

// //   // // // Seciton C: Logger controller
// //   // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //   // // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
// //   // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //   // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //   // #define GPIO_SYNC_FRAME_ISR_PIN 25
// //   // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
// //   // #define GPIO_ADC_CAPTURE_ISR_PIN 32

// //   // // Section x: Button to toggle logging/sd state
// //   #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
// //   #define USE_MODULE_SENSORS_BUTTONS

// //   #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
// //     // Next phase, will include saving adc values
// //     // #define USE_MODULE_SENSORS_ADC_INTERNAL
// //     #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
// //     #define ENABLE_SMOOTHING_ON_ADC_READING
// //     #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // #define ADC_CAPTURE_EXTERNAL_PIN 32
// //     #define ADC_CAPTURE_INPUT0_PIN 34
// //     #define ADC_CAPTURE_INPUT1_PIN 35

// //   // // Section x: OLED display 
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   #define DEBUG_PIN1_GPIO     21
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     22
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   #define DEBUG_PIN3_GPIO     26
// //   #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   #define DEBUG_PIN4_GPIO     27
// //   #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER



// // /**
// //  * 
// //  * Test of high speed I2S measurements
// //  * 
// //  * How it works:
// //  * - sync pin will reset the item counter, which is an array that stores an ID number for each item in the ringbuffer
// //  *    - reading the item from the buffer, will also mean popping the value out of it. I will use a SECOND ringbuffer for just this purpose
// //  * - sync pin keeps track of which ringbuffer items belongs to each group, so I need a counter for sync (in a ringbuffer) and a counter for RXON state changes
// //  * 
// //  * - When RXON pin is active low, then the I2S samples will be commited to the ringbuffer
// //  * - The values per item (DMA read), will still be averaged in 10s, ie 1000 samples becomes 100 samples per item
// //  *
// //  * 
// //  * 
// //  * 
// //  * 
// //  * 
// //  * Record 1000 samples per syncframe (then sum the regions that would be measurements, or try offload for summing later in code)
// //  * On finish of sync, average the sample rate down to 10 times more than actual measurement rate, save these to sd if not too many
// //  * 
// //  * 
// //  * 
// //  * 

// // sync period will trigger the i2s to run, and when X samples are recorded, will stop... 
// // This will be known as a frame, I can post downsample and remove other things after



// // sample 5.8ghz with esp32
// // have sync pulse restart counter, so I know which "item" is the start of that sync window
// // during capture constant capture, only commit to memory when a pin is low, which signifies "active rxon period"
// // There should be 40 of these windows per sync

// // sync_counter = [0,0,0,0,0 1,1,1,,,,,,,, 40,40,40 ] where X for each number will be many samples, which I can post downsample in matlab



// //  * 
// //  * */
// // #ifdef DEVICE_DEVBOARD_I2S_MEASUREMENT_SINGLE_CHANNEL
// //   #define DEVICENAME_CTR            "system_1_alpha"

// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

// //   /**
// //    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
// //    * */
// //   // #define USE_SYSTEM_BUTTON_TO_TOGGLE_SDCARD_LOGGING
// //   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

// //   /**
// //    * Debug methods
// //    * */
// //   #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT

// //   /**
// //    * Since I have no comms from esp32 to pic32, I should use this for debugging (disconnect from esp32)
// //    * Just on the debug lipo-esp32 (front soldered)
// //    * Use this to send "SDCARD" data directly to PC to logging to file and parsing.
// //    * */

// //   /**
// //    * Required defines based on above defines
// //    * */
// //   #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
// //     // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #endif



// //   /**
// //    * SDCARD name
// //    * */

// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


// //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // // Section A: GPS
// //   // #define USE_MODULE_DRIVERS_GPS
// //   // #define ENABLE_GPS_PARSER_NMEA
// //   // #define ENABLE_GPS_PARSER_UBX
// //   // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // #define NMEAGPS_DERIVED_TYPES
// //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   // #define NMEAGPS_PARSE_SAVE_MILLIS
// //   // #define gpsPort Serial1
// //   // #define D_GPS_BAUD_RATE_FAST    921600
// //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // // Section UART
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_HARDWARE_UART_1
// //   // #define ENABLE_HARDWARE_UART_2
// //   // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //   // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //   // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //   // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

// //   // // Section B: SDCard driver
// //   // #define USE_MODULE_DRIVERS_SDCARD
// //   // #define USE_SDCARD_RINGBUFFER_STREAM_OUT

// //   // // // Seciton C: Logger controller
// //   #ifdef USE_SYSTEM_MAIN_LOGGER_CONTROLLER
// //     #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //     #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //     #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //     #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
// //     // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
// //     // #define GPIO_ADC_CAPTURE_ISR_PIN 32
// //   #endif // USE_SYSTEM_MAIN_LOGGER_CONTROLLER

// //   // // Section x: Button to toggle logging/sd state
// //   // #define USE_MODULE_CORE_RULES
// //   // // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
// //   // #define USE_MODULE_SENSORS_BUTTONS


// //   /***
// //    * ESP32    wire colour       PIC32                     PURPOSE
// //    *  25             
// //    *  33
// //    *  32         red               D1                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SAMPLE_TIMESLOT_PERIOD
// //    *  35        yellow             B1 - RSS 5800            AD1_CH7
// //    *  34        white              D7                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
// //    * 
// //    * */
// //     #define ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD


// //     #define PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM 32    // This allows me to call these directly, instead of via pinmethod, so faster    
// //     #define PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM 33    // This allows me to call these directly, instead of via pinmethod, so faster
// //     #define PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM 26    // This allows me to call these directly, instead of via pinmethod, so faster
// //     #define PIN_GPIO_FUNCTION_RXON_SAMPLE_5800_ADC_NUM         35

// //     #define ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT


// //   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
// //     #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL

// //   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

    
// //   // // Section x: OLED display 
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC5G, ADC1_CH6
// //        * White      35(I), RXON SAMPLING ENABLED (ie region of sampling on)
// //        * Red        32(I), Start of Sync period (resetting this windows values)
// //        * Black      GND
// //        * 
// //        * */
// //       //"\"" STR2(PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM) "\":\"" D_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR   "\","  // 33 = Sync period from cc1110, this is used to reset the sampler (maybe change to another ringbuffer?, have two, one per active and writer)
// //       //"\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","               // 5.8 GHz RSS 


// // //35 is the new ADC

// //      // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","   // pic32 tells the device when a new sync window starts, I will probably use this for swapping buffers
// //       //"\"" STR2(PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM) "\":\"" D_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR   "\","  // When low, samples are collected. An interrupt will also be applied to it, where state change will increment a counter (ie new time slot)

// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   // #define DEBUG_PIN1_GPIO     21
// //   // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   // #define DEBUG_PIN2_GPIO     22
// //   // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   // #define DEBUG_PIN3_GPIO     26
// //   // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   // #define DEBUG_PIN4_GPIO     27
// //   // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER







// // /**
// //  * First measurement system, without esp adc until I can resolve that
// //  * This will be used for first aplha tests to plan things out
// //  * Future system will attempt to include esp adc later
// //  * 
// //  * Description: Use of button, oled and sdcard together on LIPO_ESP32
// //  * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
// //  * Test version before adding pic32 stream of data (although will use same format)
// //  * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
// //  * 
// //  * GPS @ 912800 baud recorded to local fix
// //  * - each new fix, triggers a "save" flag
// //  * OLED screen will show bytes written every second
// //  * No networking
// //  * 
// //  * 
// //  * 
// //  * */
// // #ifdef DEVICE_MEASUREMENT_SYSTEM_ALPHA_1
// //   #define DEVICENAME_CTR            "system_1_alpha"

// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

// // /**
// //  * SDCARD name
// //  * */

// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


// //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // Section A: GPS
// //   #define USE_MODULE_DRIVERS_GPS
// //   #define ENABLE_GPS_PARSER_NMEA
// //   #define ENABLE_GPS_PARSER_UBX
// //   #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   #define NMEAGPS_DERIVED_TYPES
// //   #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   #define NMEAGPS_PARSE_SAVE_MILLIS
// //   #define gpsPort Serial1
// //   #define D_GPS_BAUD_RATE_FAST    921600
// //   #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // Section UART
// //   #define USE_MODULE_DRIVERS_INTERFACE
// //   #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #define ENABLE_HARDWARE_UART_1
// //   #define ENABLE_HARDWARE_UART_2
// //   #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //   #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //   #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //   #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

// //   // Section B: SDCard driver
// //   #define USE_MODULE_DRIVERS_SDCARD
// //   #define USE_SDCARD_RINGBUFFER_STREAM_OUT

// //   // // Seciton C: Logger controller
// //   #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //   // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
// //   #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //   #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //   #define GPIO_SYNC_FRAME_ISR_PIN 25
// //   #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
// //   #define GPIO_ADC_CAPTURE_ISR_PIN 32

// //   // // Section x: Button to toggle logging/sd state
// //   #define USE_MODULE_CORE_RULES
// //   // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
// //   #define USE_MODULE_SENSORS_BUTTONS
// //   // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
// //     // // Next phase, will include saving adc values
// //     // // #define USE_MODULE_SENSORS_ADC_INTERNAL
// //     // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
// //     // #define ENABLE_SMOOTHING_ON_ADC_READING
// //     // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
// //     // // #define ADC_CAPTURE_EXTERNAL_PIN 32
// //     // #define ADC_CAPTURE_INPUT0_PIN 34
// //     // #define ADC_CAPTURE_INPUT1_PIN 35

// //   // Section x: OLED display 
// //   #define USE_MODULE_DISPLAYS_INTERFACE
// //   #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC2G, ADC1_CH6
// //        * White      35(I), ADC5G, ADC1_CH7
// //        * Red        32(I), ADC Record Trigger
// //        * Black      GND
// //        * */
// //       "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
// //       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
// //       "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   #define DEBUG_PIN1_GPIO     21
// //   #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   #define DEBUG_PIN2_GPIO     22
// //   #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   #define DEBUG_PIN3_GPIO     26
// //   #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   #define DEBUG_PIN4_GPIO     27
// //   #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER





// // /**
// //  * 
// //  * Test of high speed I2S measurements
// //  * 
// //  * How it works:
// //  * - sync pin will reset the item counter, which is an array that stores an ID number for each item in the ringbuffer
// //  *    - reading the item from the buffer, will also mean popping the value out of it. I will use a SECOND ringbuffer for just this purpose
// //  * - sync pin keeps track of which ringbuffer items belongs to each group, so I need a counter for sync (in a ringbuffer) and a counter for RXON state changes
// //  * 
// //  * - When RXON pin is active low, then the I2S samples will be commited to the ringbuffer
// //  * - The values per item (DMA read), will still be averaged in 10s, ie 1000 samples becomes 100 samples per item
// //  *
// //  * 
// //  * 
// //  * 
// //  * 
// //  * 
// //  * Record 1000 samples per syncframe (then sum the regions that would be measurements, or try offload for summing later in code)
// //  * On finish of sync, average the sample rate down to 10 times more than actual measurement rate, save these to sd if not too many
// //  * 
// //  * 
// //  * 
// //  * 

// // sync period will trigger the i2s to run, and when X samples are recorded, will stop... 
// // This will be known as a frame, I can post downsample and remove other things after



// // sample 5.8ghz with esp32
// // have sync pulse restart counter, so I know which "item" is the start of that sync window
// // during capture constant capture, only commit to memory when a pin is low, which signifies "active rxon period"
// // There should be 40 of these windows per sync

// // sync_counter = [0,0,0,0,0 1,1,1,,,,,,,, 40,40,40 ] where X for each number will be many samples, which I can post downsample in matlab



// //  * 
// //  * */
// // #ifdef DEVICE_DEVBOARD_I2S_MEASUREMENT_SINGLE_CHANNEL2
// //   #define DEVICENAME_CTR            "system_1_alpha"

// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
// //   #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

// //   /**
// //    * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
// //    * */
// //   // #define USE_SYSTEM_BUTTON_TO_TOGGLE_SDCARD_LOGGING
// //   #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
// //   #define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

// //   /**
// //    * Debug methods
// //    * */
// //   #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT

// //   /**
// //    * Since I have no comms from esp32 to pic32, I should use this for debugging (disconnect from esp32)
// //    * Just on the debug lipo-esp32 (front soldered)
// //    * Use this to send "SDCARD" data directly to PC to logging to file and parsing.
// //    * */

// //   /**
// //    * Required defines based on above defines
// //    * */
// //   #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
// //     // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   #endif



// //   /**
// //    * SDCARD name
// //    * */

// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
// //   // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


// //   #define SERIAL_DEBUG_BAUD_DEFAULT 921600

// //   // Also defining so "HardwareSerial.cpp" will use these
// //   #define RX1 18
// //   #define TX1 19
// //   #define RX2 16
// //   #define TX2 17

// //   // General defines for debugging only, not for finished
// //   #define DISABLE_NETWORK
// //   #define DISABLE_SLEEP

// //   // General defines needed in release version
// //   #define ESP32
// //   #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

// //   // // // Section A: GPS
// //   // #define USE_MODULE_DRIVERS_GPS
// //   // #define ENABLE_GPS_PARSER_NMEA
// //   // #define ENABLE_GPS_PARSER_UBX
// //   // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
// //   // #define NMEAGPS_DERIVED_TYPES
// //   // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
// //   // #define NMEAGPS_PARSE_SAVE_MILLIS
// //   // #define gpsPort Serial1
// //   // #define D_GPS_BAUD_RATE_FAST    921600
// //   // #define D_GPS_BAUD_RATE_DEFAULT 9600

// //   // // // Section UART
// //   // #define USE_MODULE_DRIVERS_INTERFACE
// //   // #define USE_MODULE_DRIVERS_SERIAL_UART
// //   // #define ENABLE_HARDWARE_UART_1
// //   // #define ENABLE_HARDWARE_UART_2
// //   // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
// //   // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
// //   // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
// //   // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
// //   // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

// //   // // Section B: SDCard driver
// //   // #define USE_MODULE_DRIVERS_SDCARD
// //   // #define USE_SDCARD_RINGBUFFER_STREAM_OUT

// //   // // // Seciton C: Logger controller
// //   #ifdef USE_SYSTEM_MAIN_LOGGER_CONTROLLER
// //     #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
// //     #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
// //     #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
// //     #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
// //     // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
// //     // #define GPIO_ADC_CAPTURE_ISR_PIN 32
// //   #endif // USE_SYSTEM_MAIN_LOGGER_CONTROLLER

// //   // // Section x: Button to toggle logging/sd state
// //   // #define USE_MODULE_CORE_RULES
// //   // // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
// //   // #define USE_MODULE_SENSORS_BUTTONS


// //   /***
// //    * ESP32    wire colour       PIC32                     PURPOSE
// //    *  25             
// //    *  33
// //    *  32         red               D1                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SAMPLE_TIMESLOT_PERIOD
// //    *  35        yellow             B1 - RSS 5800            AD1_CH7
// //    *  34        white              D7                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
// //    * 
// //    * */
// //     #define ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD


// //     #define PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM 32    // This allows me to call these directly, instead of via pinmethod, so faster    
// //     #define PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM 33    // This allows me to call these directly, instead of via pinmethod, so faster
// //     #define PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM 26    // This allows me to call these directly, instead of via pinmethod, so faster
// //     #define PIN_GPIO_FUNCTION_RXON_SAMPLE_5800_ADC_NUM         35

// //     #define ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT


// //   #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
// //     #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL

// //   #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

    
// //   // // Section x: OLED display 
// //   // #define USE_MODULE_DISPLAYS_INTERFACE
// //   // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
// //   #define USE_MODULE_TEMPLATE
// //   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
// //     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
// //     "\"" D_JSON_GPIOC "\":{"
// //       /** 4P large JST - ADC
// //        * Yellow     34(I), ADC5G, ADC1_CH6
// //        * White      35(I), RXON SAMPLING ENABLED (ie region of sampling on)
// //        * Red        32(I), Start of Sync period (resetting this windows values)
// //        * Black      GND
// //        * 
// //        * */
// //       //"\"" STR2(PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM) "\":\"" D_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR   "\","  // 33 = Sync period from cc1110, this is used to reset the sampler (maybe change to another ringbuffer?, have two, one per active and writer)
// //       //"\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","               // 5.8 GHz RSS 


// // //35 is the new ADC

// //      // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","   // pic32 tells the device when a new sync window starts, I will probably use this for swapping buffers
// //       //"\"" STR2(PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM) "\":\"" D_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR   "\","  // When low, samples are collected. An interrupt will also be applied to it, where state change will increment a counter (ie new time slot)

// //       /** 5P small - UART2 RSS Stream
// //        * Orange      17, UART2_TX
// //        * Yellow      16, UART2_RX
// //        * White       25, ie superframe event over, rising edge interrupt
// //        * Red         5V
// //        * Black       GND
// //        * */
// //       "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
// //       "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
// //       /** 5P small - UART1 GPS Stream
// //        * Orange      19, UART1_TX
// //        * Yellow      18, UART1_RX
// //        * White        
// //        * Red         VCC, 3V3
// //        * Black       GND
// //        * */
// //       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
// //       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
// //       /** 6P small - SD Card
// //        * Green       15, CS
// //        * Orange      14, SCK
// //        * Yellow      13, MOSI
// //        * White       12, MISO
// //        * Red         3V3
// //        * Black       GND
// //        * */
// //       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
// //       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
// //       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
// //       "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
// //       /** Built in - OLED
// //        * 
// //        * */
// //       "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
// //       "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
// //       /** 2P small
// //        * Red        Button Logging Toggle
// //        * Black      GND
// //        * */
// //       "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
// //     "},"
// //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
// //   "}";

// //   /**
// //    * Debug pins
// //    * */
// //   // #define DEBUG_PIN1_GPIO     21
// //   // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
// //   // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
// //   // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

// //   // #define DEBUG_PIN2_GPIO     22
// //   // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
// //   // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
// //   // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

// //   // #define DEBUG_PIN3_GPIO     26
// //   // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
// //   // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
// //   // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

// //   // #define DEBUG_PIN4_GPIO     27
// //   // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
// //   // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
// //   // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

// //   #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
// //   //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


// // #endif // DEVICE_GPSPARSER_TESTER


// // #endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H









// #endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H



/**
 * Need to add cc1110 input, simple low/high into json output so I can show sync messages in recording
 * need 12v power source
 * 
 * */
#ifdef DEVICE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_UAV
  #define DEVICENAME_CTR            "radpattern"
  #define DEVICENAME_FRIENDLY_CTR   "radpattern"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "RP0"
  #define DEVICENUM_NUM   0

  // #define ENABLE_DEVFEATURE_SAMPLER_FIX_CYAN
  // #define ENABLE_DEVFEATURE_UART2RXBUFFER_INTO_MULTIPLE_BUFFERS_INSTEAD_OF_RINGBUFFER
  #define ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
  #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
  #define USE_SYSTEM_SDCARD_LOGGING
  #define USE_SYSTEM_IMU_RADIATIONPATTERN

  /**
   * Debug methods
   * */
  /**
   * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
   * */
  // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
    // #define USE_MODULE_SENSORS_LSM303D

  /**
   * ADC written directly into controller, not as a module
   * */
  #define USE_DEVFEATURE_ADC_IN_CONTROLLER

  /**
   *  IMU recording
   * */
  #ifdef USE_SYSTEM_IMU_RADIATIONPATTERN
    #define USE_MODULE_SENSORS_LSM303D
    #define USE_MODULE_SENSORS_L3G
    #define USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
  #endif

  /**
   * General defines to disable systems not needed
   * */
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  /**
   * SDCard
   * */
  #ifdef USE_SYSTEM_SDCARD_LOGGING
    #define USE_MODULE_DRIVERS_SDCARD
    #define USE_SDCARD_RINGBUFFER_STREAM_OUT
    #define ENABLE_DEVFEATURE_DUALCORE_SDCARD_WRITER
  #endif // USE_SYSTEM_SDCARD_LOGGING

  /**
   * Button input
   * */
  #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
    #define USE_MODULE_CORE_RULES
    #define USE_MODULE_SENSORS_BUTTONS
  #endif // USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE

  /**
   * OLED display
   * */
  #ifdef USE_SYSTEM_OLED_LOGGER_FEEDBACK
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_OLED_SSD1306
    #define SHOW_SPLASH
  #endif // USE_SYSTEM_OLED_LOGGER_FEEDBACK

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     32(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      // "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_ADC1_CH4_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
      /** 5P small - UART2 RSS Stream
       * Orange      17, UART2_TX
       * Yellow      16, UART2_RX
       * White       25, ie superframe event over, rising edge interrupt
       * Red         5V
       * Black       GND
       * */
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_RX_CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_RING_BUFFER_TX_CTR   "\","
      /** 5P small - UART1 GPS Stream
       * Orange      19, UART1_TX
       * Yellow      18, UART1_RX
       * White        
       * Red         VCC, 3V3
       * Black       GND
       * */
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_RX_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL1_RING_BUFFER_TX_CTR   "\","
      /** 6P small - SD Card
       * Green       15, CS
       * Orange      14, SCK
       * Yellow      13, MOSI
       * White       12, MISO
       * Red         3V3
       * Black       GND
       * */
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CSO_CTR   "\","
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MOSI_CTR  "\","
      "\"12\":\"" D_GPIO_FUNCTION_SDCARD_VSPI_MISO_CTR  "\","
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      /**
       * Optional added switches for controls eg radiation pattern enabling
       * GPIO21 - manual digitalread/set in radpat
       * GPIO22 - manual digitalread/set in radpat
       * */
      // "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      // "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define GPIO_FUNCTION_MANUAL_ENABLE_SAMPLING_NUMBER 22
  #define GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER 27

  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "ARM"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "LEG"
  

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_LSM303D_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";



#endif // DEVICE_GPSPARSER_TESTER



#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
  #warning "--------------------------------------------------------------- DEVICE_FORCED_TO_BE_TESTER" is enabled for this compile
#endif


#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

