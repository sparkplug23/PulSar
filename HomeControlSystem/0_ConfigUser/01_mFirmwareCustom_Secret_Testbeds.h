#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_TB_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL_TB_H

/*********************************************************************************************\
 * Testbeds: Development devices to get specialised hardware working in their default minimum/basic operation
 *           This will be examples for others to follow
 * - Finished testbeds should be moved into "Examples" to be generated for others first build
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
// #define DISABLE_WEBSERVER
// //#define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "6_Lights/00_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"

/**
 *  TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- TESTBEDS   -- 
 * Special hardware, shelly, sonoff
 * 
 TESTBEDs can use flag to swap between esp8266 and esp32 varaints, so hardware may be tested on both
**/
// #define DEVICE_TESTBED_SHELLY1_01
// #define DEVICE_TESTBED_SHELLY2P5_01
// #define DEVICE_TESTBED_SHELLY2_DIMMER 
// #define DEVICE_TESTBED_SONOFF_4CHPRO 

/**
 * Generic hardware eg nodemcu
 * */
// #define DEVICE_TESTBED_MOTION
// #define DEVICE_TESTBED_DS18XX
// #define DEVICE_TESTBED_RGBCLOCK
// #define DEVICE_TESTBED_GPS_SDCARD_LOGGER
// #define DEVICE_TESTBED_ULTRASONIC
// #define DEVICE_TESTBED_9AXIS_GRYO
// #define DEVICE_TESTBED_9AXIS_GRYO_MPU9250
// #define DEVICE_TESTBED_9AXIS_GRYO_GY89
// #define DEVICE_TESTBED_6DOF_ECOMPASS_LSM303D
// #define DEVICE_TESTBED_ANIMATION_SEGMENTS_3PIXELS
// #define DEVICE_TESTBED_ANIMATION_SEGMENTS_4PIXELS
// #define DEVICE_TESTBED_ANIMATION_SEGMENTS_DUAL_MQTT
// #define DEVICE_TESTBED_NEXTION_HEATING
// #define DEVICE_TESTBED_NEXTION_DISPLAY_GENERIC
// #define DEVICE_TESTBED_NEXTION_DISPLAY_GENERIC_WITH_WEBUI
// #define DEVICE_TESTBED_MOTION_CLIMATE_SENSOR
// #define DEVICE_TESTBED_HVAC_HEAT_ONLY_WITH_ENERGY_SENSOR
// #define DEVICE_TESTBED_WEBUI_BASIC_GUI_ESP8266
// #define DEVICE_TESTBED_WEBUI_BASIC_GUI_ESP32
// #define DEVICE_TESTBED_WEBUI_FIRMWARE_UPDATE
// #define DEVICE_TESTBED_WEBUI_ANIMATION_SEGMENTS_3PIXELS
// #define DEVICE_TESTBED_WEBUI_ANIMATION_SEGMENTS_4PIXELS
// #define DEVICE_TESTBED_3PIXELS_MATRIX_E131_PROTOCOL
// #define DEVICE_TESTBED_STATUS_LEDS
// #define DEVICE_TESTBED_CLIMATE_SENSOR_BREADBOARD
// #define DEVICE_TESTBED_RTC_FASTBOOT
// #define DEVICE_TESTBED_ANIMATION_ADDING_NOTIFICATION_MODE
// #define DEVICE_7SEGMENT_WHITE_DOUBLE_OUTDOOR_TEMPERATURE
// #define DEVICE_TESTBED_HARDWARE_BME_ESP32
// #define DEVICE_TESTBED_H801_PWM_MANUAL
// #define DEVICE_TESTBED_BLENDING_VARIABLES
// #define DEVICE_TESTBED_BUTTON_MULTIPRESS
// #define DEVICE_TESTBED_H801_SUNELEVATION_REACTIVE_PALETTES


// #define DEVICE_ESP32_DEVKIT_BASIC
// #define DEVICE_ESP32_WEBCAM1
// #define DEVICE_ESP32_WEBCAM2
// #define DEVICE_ESP32_WEBCAM3

// #define DEVICE_TESTBED_BME_ESP32


// Include the home devices, which should ONLY contain final hardware
#include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_LongTerm.h"

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


#ifdef DEVICE_TESTBED_SONOFF_4CHPRO
  #define DEVICENAME_CTR          "testbed_4chpro"
  #define DEVICENAME_FRIENDLY_CTR "Sonoff 4CH Pro"
  
  #define USE_MODULE_CORE_RULES
  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_DRIVERS_RELAY

  // #define USE_MODULE_CONTROLLER_SONOFF_4CHPRO

  #define USE_MODULE_DRIVERS_RF433_RCSWITCH

  // default key# = relay#
  // RF Key433 using rules to match patterns then need limit to how often RF matches 

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SONOFF_4CHPRO_CTR "\""
  "}";
 
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0,"
        "\"State\":\"Toggle\""
      "}"
    "},"
    "\"Rule1\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1,"
        "\"State\":\"Toggle\""
      "}"
    "}"
    "},"
    "\"Rule2\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":2,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":2,"
        "\"State\":\"Toggle\""
      "}"
    "}"
    "},"
    "\"Rule3\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":3,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"Relays\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":3,"
        "\"State\":\"Toggle\""
      "}"
    "}"
  "}";

  /**
   * @brief Remote 433 Number 1
   ***
    1L:  Data 0xA37348 (10711880), Bits 24, Protocol 1, Delay 238
    1R:  Data 0xA3734C (10711884), Bits 24, Protocol 1, Delay 239
    2L:  Data 0xA37344 (10711876), Bits 24, Protocol 1, Delay 239
    2R:  Data 0xA37349 (10711881), Bits 24, Protocol 1, Delay 239
    3L:  Data 0xA37342 (10711874), Bits 24, Protocol 1, Delay 244
    3R:  Data 0xA37345 (10711877), Bits 24, Protocol 1, Delay 240
    4L:  Data 0xA37341 (10711873), Bits 24, Protocol 1, Delay 238
    4R:  Data 0xA37343 (10711875), Bits 24, Protocol 1, Delay 239

    rules packing

    function: tasker_func for "received rf"
    deviceid: int list
    values: [
      protocol int value
      delay number (2 bits)
      bits size eg 24 or 32 bits (and hence next bits)
      data1
      data2
      data3]

    For now, instead of handling via rules, lets do this via openhab (ie RF code triggers a press and "expire" resets it after X seconds)

   */

  /**
   * @brief Remote 433 Number 2
   ***
    1L:  Data 0x3194B8 (3249336), Bits 24, Protocol 1, Delay 243
    1R:  Data 0x3194BC (3249340), Bits 24, Protocol 1, Delay 242     
    2L:  Data 0x3194B4 (3249332), Bits 24, Protocol 1, Delay 241  
    2R:  Data 0x3194B9 (3249337), Bits 24, Protocol 1, Delay 241    
    3L:  Data 0x3194B2 (3249330), Bits 24, Protocol 1, Delay 241
    3R:  Data 0x3194B5 (3249333), Bits 24, Protocol 1, Delay 242
    4L:  Data 0x3194B1 (3249329), Bits 24, Protocol 1, Delay 241
    4R:  Data 0x3194B3 (3249331), Bits 24, Protocol 1, Delay 241                           
   */

  /**
   * @brief Remote 433 Number 2
   ***
    1L:  Data 0x6730C8 (6762696), Bits 24, Protocol 1, Delay 240
    1R:  Data 0x6730CC (6762700), Bits 24, Protocol 1, Delay 240
    2L:  Data 0x6730C4 (6762692), Bits 24, Protocol 1, Delay 241
    2R:  Data 0x6730C9 (6762697), Bits 24, Protocol 1, Delay 241
    3L:  Data 0x6730C2 (6762690), Bits 24, Protocol 1, Delay 240
    3R:  Data 0x6730C5 (6762693), Bits 24, Protocol 1, Delay 240
    4L:  Data 0x6730C1 (6762689), Bits 24, Protocol 1, Delay 242
    4R:  Data 0x6730C3 (6762691), Bits 24, Protocol 1, Delay 241 
   */

  /**
   * @brief Remote 433 Number 2
   ***
    1L:  Data 0xE632B8 (15086264), Bits 24, Protocol 1, Delay 237 
    1R:  Data 0xE632BC (15086268), Bits 24, Protocol 1, Delay 236 
    2L:  Data 0xE632B4 (15086260), Bits 24, Protocol 1, Delay 234 
    2R:  Data 0xE632B9 (15086265), Bits 24, Protocol 1, Delay 236 
    3L:  Data 0xE632B2 (15086258), Bits 24, Protocol 1, Delay 235 
    3R:  Data 0xE632B5 (15086261), Bits 24, Protocol 1, Delay 235 
    4L:  Data 0xE632B1 (15086257), Bits 24, Protocol 1, Delay 235 
    4R:  Data 0xE632B3 (15086259), Bits 24, Protocol 1, Delay 236 
   */


#endif



/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_NEXTION_DISPLAY_GENERIC
  #define DEVICENAME_CTR            "testbed_nextion_display"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":40,"
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgbwc\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":1000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":8"
  //   "},"
  //   "\"" D_JSON_CCT_TEMP "\":300,"
  //   "\"" D_JSON_HUE "\":25,"
  //   "\"" D_JSON_SAT "\":100,"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\"," //ie 10
  //   "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  // "}";
  
  // #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  // #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "}"
  // "}";

  // #define USE_FUNCTION_NEXTION_INIT_PANEL_COMMAND_TEMPLATE
  // DEFINE_PGM_CTR(NEXTION_INIT_PANEL_COMMAND_TEMPLATE)
  // "{"
  //   // "\"" D_JSON_DEVICENAME "\":{"
  //   //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //   //     "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //   //   "],"
  //   //   "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
  //   //     "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //   //   "]"
  //   // "}"
  //   "p[0]."


  // "}";

#endif


/**
 * This will become the final version of a panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_NEXTION_DISPLAY_GENERIC_WITH_WEBUI
  #define DEVICENAME_CTR            "testbed_nextion_display"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


#endif



/**
 * This will become the final version of this panel, but requires animation fixing first
 * */
#ifdef DEVICE_TESTBED_NEXTION_HEATING
  #define DEVICENAME_CTR            "testbed_nextion_heating"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Heating"

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      // #ifdef USE_MODULE_SENSORS_MOTION
      // "\"21\":\"" D_GPIO_FUNCTION_PIR_1_INV_CTR "\","
      // #endif
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";


  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":40,"
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"rgbwc\","
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":1000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":8"
  //   "},"
  //   "\"" D_JSON_CCT_TEMP "\":300,"
  //   "\"" D_JSON_HUE "\":25,"
  //   "\"" D_JSON_SAT "\":100,"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 01\"," //ie 10
  //   "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  // "}";
  
  // #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
  // #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
  //     "]"
  //   "}"
  // "}";

  // #define USE_FUNCTION_NEXTION_INIT_PANEL_COMMAND_TEMPLATE
  DEFINE_PGM_CTR(NEXTION_INIT_PANEL_COMMAND_TEMPLATE)
  "{"
    // "\"" D_JSON_DEVICENAME "\":{"
    //   "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
    //     "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
    //   "],"
    //   "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
    //     "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
    //   "]"
    // "}"
    "p[0]."


  "}";

#endif




#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_3PIXELS
  #define DEVICENAME_CTR          "testbed_animation_segment_01"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
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
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


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
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif


/**
 * @brief Used to test SK6812 rgbW (ie addressable 4 pixel)
 * 
 */
#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_4PIXELS
  #define DEVICENAME_CTR          "testbed_segment_4p_01"               APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Secondary Testbed for Segments 01"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 55                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   **/
  #define LIGHTING_TEMPLATE_DEFAULT_DOWNSTAIRS_TOILET

   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // cant be removed or does not compile, phase out, should always be included

  // I need to make a define difference between "h801" effects, and addressable effects, since there is overlapping effects

  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  /**
   * @brief Debug flags, used mostly be me
   * */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  

  /**
   * @brief The following templates are tested examples
   * */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_DEFAULT_DOWNSTAIRS_TOILET
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\"," 
    "\"Effects\":{"
      "\"Function\":\"Solid RGBCCT\""
    "},"
    "\"ColourPalette\":\"RGBCCTColour 00\"," 
    "\"Hue\":20,"
    "\"Sat\":100,"
    "\"CCT_TempPercentage\":100,"
    "\"Transition\":{"
      "\"TimeMs\":1000,"
      "\"RateMs\":2000"
    "},"    
    "\"BrightnessCCT\":100,"
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 01\"," 
    "\"Effects\":{"
      "\"Function\":\"" "Slow Glow" "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":1000,"
      "\"RateMs\":2000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
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
  
#endif



/**
 * @brief Using PWM manually, with blend time between previous and new number
 * This will fill the gap until PWM single channels in lighting is properly added
 * This will mean, each segment in lighting would be a single channel led with assigned pwm output
 * eg. 5 segments 1 per pin ||| 2 segments with 2 pin CCT only and segment with 3 pin RGB ||| etc
 * */
#ifdef DEVICE_TESTBED_H801_PWM_MANUAL
  #define DEVICENAME_CTR          "testbed_h801_manual"
  #define DEVICENAME_FRIENDLY_CTR "Testbed PWM H801 Manual"
  
  
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2 

  #define USE_MODULE_DRIVERS_PWM

  #define USE_MODULE_SENSORS_BUTTONS

  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR

  #define DEBUG_TARGET_ANIMATOR_SEGMENTS

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM
  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  #define MAX_NUM_SEGMENTS 5
  
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS


  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"0\":\""  D_GPIO_FUNCTION_KEY1_INV_CTR "\","
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
    "\"" D_JSON_COLOUR_PALETTE "\":10,"
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"light\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";

    
#endif


/**
 * @brief Testing of some special palettes that change their progress based on sun elevation
 * */
#ifdef DEVICE_TESTBED_H801_SUNELEVATION_REACTIVE_PALETTES
  #define DEVICENAME_CTR          "testbed_h801_sun_palettes"
  #define DEVICENAME_FRIENDLY_CTR "Testbed H801 Sun Palettes"
    
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_DEVFEATURE_BUTTON_HANDLER_V2

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  #define ENABLE_DEVFEATURE_SOLAR_PALETTES
  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
  #define DEBUG_TARGET_ANIMATOR_SEGMENTS
  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  #define ENABLE_EXTRA_EFFECTS_SUNPOSITIONS
  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"0\":\""  D_GPIO_FUNCTION_KEY1_INV_CTR "\","
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
    "\"" D_JSON_COLOUR_PALETTE "\":10,"
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"light\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";

    
#endif




/**
 * Development of LinearBlendVariable
 * */
#ifdef DEVICE_TESTBED_BLENDING_VARIABLES
  #define DEVICENAME_CTR          "testbed_h801_manual"
  #define DEVICENAME_FRIENDLY_CTR "Testbed PWM H801 Manual"
  
  
  // #define USE_SERIAL_ALTERNATE_TX
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 2 

  #define USE_MODULE_DRIVERS_PWM

  #define USE_MODULE_SENSORS_BUTTONS

  #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR

  #define DEBUG_TARGET_ANIMATOR_SEGMENTS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_PWM
  
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  // #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  #define MAX_NUM_SEGMENTS 5
  
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS


  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  
  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D4\":\""  D_GPIO_FUNCTION_PWM1_CTR "\","
      "\"D1\":\""  D_GPIO_FUNCTION_PWM2_CTR "\","
      "\"D2\":\""  D_GPIO_FUNCTION_PWM3_CTR "\","
      "\"D6\":\""  D_GPIO_FUNCTION_PWM4_CTR "\","
      "\"16\":\""  D_GPIO_FUNCTION_PWM5_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
 

  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"GRBcw\","
    "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":10,\"" D_JSON_RATE "\":20\"},"
    "\"" D_JSON_COLOUR_PALETTE "\":10,"
    "\"" D_JSON_ANIMATIONMODE  "\":\"" D_JSON_EFFECTS "\","
    "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"//Sun Elevation RGBCCT Solid Palette 01\"},"
    "\"" D_JSON_BRIGHTNESS     "\":100"
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"Buttons\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"light\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":\"Toggle\"" // toggle
      "}"
    "}"
  "}";

    
#endif




#ifdef DEVICE_TESTBED_ANIMATION_ADDING_NOTIFICATION_MODE
  #define DEVICENAME_CTR          "testbed_animation_segment_01"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work


  // Create a class that is just for notifications, which can be added into the buffer of segment runtime


  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
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
  

#endif


// #ifdef DEVICE_7SEGMENT_WHITE_DOUBLE_OUTDOOR_TEMPERATURE
//   #define DEVICENAME_CTR          "testbed_add_double_digit"
//   #define DEVICENAME_FRIENDLY_CTR "Testbed Addressable Digit"
//   #define DEVICENAME_ROOMHINT_CTR "Testbed"
 

// //  ##
// //   //#define FORCE_TEMPLATE_LOADING
// //   // #define SETTINGS_HOLDER 1



// //   #define USE_LIGHTING_TEMPLATE
// //   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
// //   "{"
// //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
// //     #ifdef STRIP_SIZE_MAX
// //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
// //     #else
// //     "\"" D_JSON_STRIP_SIZE       "\":50,"
// //     #endif //STRIP_SIZE_MAX
// //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
// //     "\"" D_JSON_TRANSITION       "\":{"
// //       "\"" D_JSON_TIME_MS "\":0,"
// //       "\"" D_JSON_RATE_MS "\":1000,"
// //       "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
// //       "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
// //     "},"
// //     "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
// //     "\"" D_JSON_EFFECTS "\":{" 
// //       "\"" D_JSON_FUNCTION "\":29"
// //     "},"
// //     // "\"CCT_Temp\": 152,"
// //     // "\"Hue\":25,"
// //     // "\"Sat\":100,"
// //     "\"" D_JSON_COLOUR_PALETTE "\":41,"
// //     "\"BrightnessCCT\":10,"
// //     "\"" D_JSON_BRIGHTNESS_RGB "\":100"

// //   "}";



//   // #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
//   // #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

//   // #define USE_MODULE_NETWORK_WEBSERVER // enable by default on esp32 going forward for pushing development
//   #define DISABLE_WEBSERVER  // phase out to only USE_MODULE_NETWORK_WEBSERVER

//   #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

//   #define USE_MODULE_DRIVERS_LEDS

//   // #define USE_MODULE_DRIVERS_IRREMOTE
//   // #define USE_IR_RECEIVE

//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//   #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
//   #define STRIP_SIZE_MAX 93
//   // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
//   // #define USE_SK6812_METHOD_DEFAULT
//   #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//   #define ENABLE_DEVFEATURE_RGB_CLOCK

//   // {a,b,c,d,e,f,g}

//   // #define USE_TASK_RGBLIGHTING_NOTIFICATIONS

//   #define ENABLE_SEGMENT_EFFECTS_SELECTIVE_NOTIFICATIONS

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"   
//     /**
//      * @brief Right side
//      **/
//       #ifdef USE_MODULE_SENSORS_DS18X   
//       "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
//       "\"22\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
//       #endif // USE_MODULE_SENSORS_DS18X
//       // TX0 - Debug Serial TX
//       // RX0 - Debug Serial RX
//       #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
//       "\"21\":\"" D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "\","
//       #endif
//       #ifdef USE_MODULE_SENSORS_DHT
//       "\"19\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
//       "\"18\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","
//       #endif
//       #ifdef USE_MODULE_SENSORS_MOTION
//       "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
//       #endif
//       #ifdef USE_MODULE_DISPLAYS_NEXTION
//       "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
//       "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
//       #endif
//       #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//       "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","   // Use the 12LED RGBW ring to permit led testing, and eventually, notification mode
//       #endif 
//       #ifdef USE_MODULE_DRIVERS_LEDS
//       "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
//       #endif
//       #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
//       "\"15\":\"" D_GPIO_FUNCTION_IR_RECV_CTR "\","
//       #endif
//       /**
//        * @brief Left side
//        **/
//       // EN
//       // VP
//       // VN
//       #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
//       "\"34\":\"" D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6
//       #endif
//       // 35 - adc1_7
//       // 32 - Touch9
//       // 33 - Touch8
//       // 25 - DAC1 = LM386 Amplifier Module
//       #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
//       "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
//       #endif
//       // 14
//       // 12
//       // 13
//       // Can I introduce a way that a comma at the end, does not make a broken json?
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";

//   #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "BedroomDesk"
//   #define D_DEVICE_SENSOR_CLIMATE_BME "BedroomDesk-BME"
//   #define D_DEVICE_SENSOR_CLIMATE_DHT1 "BedroomDesk-DHT1"
//   #define D_DEVICE_SENSOR_CLIMATE_DHT2 "BedroomDesk-DHT2"
  
  

//   #define D_DEVICE_SENSOR_DB18S20_0_NAME        "DB_01"
//   #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
//   #define D_DEVICE_SENSOR_DB18S20_1_NAME        "DB_02"
//   #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,205,206,170,25]"
//   #define D_DEVICE_SENSOR_DB18S20_2_NAME        "DB_03"
//   #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,134,175,63]"
//   #define D_DEVICE_SENSOR_DB18S20_3_NAME        "DB_04"
//   #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,205,202,237,231]"
//   #define D_DEVICE_SENSOR_DB18S20_4_NAME        "DB_05"
//   #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,195,135,126,242]"
//   #define D_DEVICE_SENSOR_DB18S20_5_NAME        "DB_06"
//   #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,195,135,215,193]"
  

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
//         "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
//       "],"
//       "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_CLIMATE_BME "\""
//       "],"  
//       "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_CLIMATE_DHT1 "\","
//         "\"" D_DEVICE_SENSOR_CLIMATE_DHT2 "\""
//       "],"  
//       "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
//       "]"  
//     "},"
//     "\"" D_JSON_SENSORADDRESS "\":{"
//       "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":[" 
//         D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
//         D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
//         D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
//         D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
//         D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
//         D_DEVICE_SENSOR_DB18S20_5_ADDRESS ""
//       "]"  
//     "},"    
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
//   "}";

  
//   #define USE_RULES_TEMPLATE
//   DEFINE_PGM_CTR(RULES_TEMPLATE)
//   "{"
//     // "\"AddRuleDefault\":\"Motion0=0\","
//     // MOTION
//     "\"Rule0\":{"
//       "\"Trigger\":{"
//         "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
//         "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
//         "\"DeviceName\":0,"
//         "\"State\":\"On\""
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
//         "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
//         "\"DeviceName\":0," 
//         "\"State\":\"Follow\""
//       "}"
//     "}"
//   "}";

//   #ifdef USE_MODULE_LIGHTS_INTERFACE
//   #define USE_LIGHTING_TEMPLATE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     "\"AnimationMode\": \"Effects\","
//     "\"ColourOrder\": \"rgb\","
//     "\"ColourPalette\": 0,"
//     "\"Effects\": {"
//       "\"Function\":\"Seven-Segment Manual\""
//     "},"
//     "\"BrightnessRGB\": 5,"
//     "\"Transition\": {"
//       "\"TimeMs\": 500"
//     "}"
//   "}";
//   #endif // USE_MODULE_LIGHTS_INTERFACE





// #endif





/**
 * @brief Used to test status leds on both hardware, needing analog (pwm) and digital drivers (the PWM vs IO needs to be lower level controls)
 * 
 */
#ifdef DEVICE_TESTBED_STATUS_LEDS
  #define DEVICENAME_CTR          "testbed_status_leds"               APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Testbed Status LEDs"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define USE_MODULE_DRIVERS_LEDS

   
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 
      "\"" "2" "\":\"" D_GPIO_FUNCTION_LED1_CTR  "\","
      "\"" "16" "\":\"" D_GPIO_FUNCTION_LED2_CTR  "\""
    #else
      "\"" "LBI" "\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
#endif

#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_DUAL_MQTT
  #define DEVICENAME_CTR          "testbed_animation_segment_01_dual"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 50                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  //#define ENABLE_DEVFEATURE_DUAL_MQTT_BROKER
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED  192,168,0,55

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
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
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


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
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif



/**
 * @brief Used to develop the core webui again, no extra functions/buttons, minimal page with console etc
 * 
 */
#ifdef DEVICE_TESTBED_WEBUI_BASIC_GUI_ESP8266
  #define DEVICENAME_CTR          "testbed_webui_basic"               APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Testbed for WebPageUI"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  #define DEVICENAME_ROOMHINT_CTR "Testbed"

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_DEFAULT_FIRMWARE_SENSORS

  // Step 1: Gaining ability to HTTP flash

  // Step 2: Debug screen, really just print json messages from all templates out onto the window so I can view them
             // LAter, introduce parsing of json for web html, so each key/valye pair becomes a line, and each json level becomes another indent ie "JsonPretty"

  // Step 3: view module template configured, as if I am going to change it (this requires settings saving first)

  // #define DISABLE_SLEEP

  #undef ESP32
  #define ESP8266

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIOC "\":{"
    // #ifdef ESP8266 
    //   "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    // #else
    //   "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    // #endif
    // "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif



#ifdef DEVICE_TESTBED_WEBUI_BASIC_GUI_ESP32
  #define DEVICENAME_CTR          "testbed_webui_basic"               APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Testbed for WebPageUI"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  #define DEVICENAME_ROOMHINT_CTR "Testbed"

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_FREERAM_APPENDING_SERIAL

  #define DISABLE_SLEEP

  #define ESP32
  #undef ESP8266

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIOC "\":{"
    // #ifdef ESP8266 
    //   "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    // #else
    //   "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    // #endif
    // "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
#endif


#ifdef DEVICE_TESTBED_WEBUI_ANIMATION_SEGMENTS_3PIXELS
  #define DEVICENAME_CTR          "testbed_webui_3pixels"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
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
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


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
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif


#ifdef DEVICE_TESTBED_WEBUI_ANIMATION_SEGMENTS_4PIXELS
  #define DEVICENAME_CTR          "testbed_webui_4pixels"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 3                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  // #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEVFEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_COMPILE_TIME_DEFINES_OF_MODULE_LIST


  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW

  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  // #define ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
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
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


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
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif





/**
 * @brief For the development and inclusion of the E131 protocol for xLight stream data
 * 
 */
#ifdef DEVICE_TESTBED_3PIXELS_MATRIX_E131_PROTOCOL
  #define DEVICENAME_CTR          "testbed_matrix_3p_e131"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 100                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266 // default pins for ws28xx
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

/**
 * @brief 
 * LIGHTING_TEMPLATE
 * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
 * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
 *    eg
 *        {
 *        "Segment0":{
 *                      "ColourPalette":"Christmas 01" 
 *                   },
 *        "Segment1":{
 *                      "ColourPalette":"Christmas 02" 
 *                   },
 *        "Segment2":{
 *                      "ColourPalette":"Christmas 03" 
 *                   }
 *         }
 * 
 */

/**
 * @brief The following templates are tested examples
 * 
 */

  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":5000,"
      "\"RateMs\":20000"
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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
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
  
  /**
   * @brief For easy copy and pasting (as backup for mqtt explorer commands)
   * These are simply copies of commands, that may or may not be translated into future templates
   * 
   */


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
    1300
  ],
  "Effects": {
    "Function":21,
    "Speed": 10,
    "Intensity": 255
  },
  "ColourPalette": 95,
  "WLED": {
    "Colour0": [
      255,
      0,
      0,
      0
    ],
    "Colour1": [
      0,
      0,
      0,
      0
    ],
    "Colour2": [
      0,0,
      255,
      0
    ]
  },
  "Hue": 10,
  "Sat": 100,
  "ColourOrder": "grbwc",
  "Transition": {
    "TimeMs": 0,
    "RateMs": 23
  },
  "BrightnessRGB": 0,
  "BrightnessCCT": 100
}

*/

#endif


#ifdef DEVICE_TESTBED_ANIMATION_SEGMENTS_DUAL_MQTT
  #define DEVICENAME_CTR          "testbed_animation_segment_01_dual"                                      // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Primary Testbed for Segments 01"                                   // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 50                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  //#define ENABLE_DEVFEATURE_DUAL_MQTT_BROKER
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED  192,168,0,55

  /**
   * @brief Uncomment one line to use testing template configs for lighting_template
   * 
   */
  // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
  #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
  /**
   * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
   **/
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  /**
   * @brief defines to be tested and incorporated fully
   **/
  #define ENABLE_FEATURE_INCLUDE_WLED_PALETTES
  #define ENABLE_CRGBPALETTES_IN_PROGMEM
  #define ENABLE_DEVFEATURE_SHIMMERING_PALETTE_BRIGHTNESS_LIMIT
  #define ENABLE_DEVFEATURE_MOVE_ALL_PALETTE_FASTLED_WLED_INTO_PALETTE_CLASS
  // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
  // #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
  // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
  // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
  /**
   * @brief Debug flags, used mostly be me
   * 
   */  
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define DEBUG_WLED_EFFECT_FUNCTIONS
  // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
    #ifdef ESP8266
      "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #else
      "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * @brief The following templates are tested examples
   * 
   */
  #define USE_LIGHTING_TEMPLATE

  #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
    "\"Effects\":{"
      "\"Function\":\"Shimmering Palette\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif


#endif

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Device Type: Shelly 2.5
 * Modules:   
 *  - 2 Mains " D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "
 *  - 2 Mains Inputs
 *  - Energy monitoring (Voltage, Current and power)
 *  - Reset button on PCB
 *  - LED
 * Notes: Requires neutral
 * 
 * Default configuration
 * Mode A (Inputs are switches eg basic wall switch)
 * Mode B (Inputs are motion events eg security light)
 * 
 * */
#ifdef DEVICE_TESTBED_SHELLY2P5_01
  #define DEVICENAME_CTR          "testbed_shelly25_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly 2.5 #01"

  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_ADE7953

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_BUTTONS
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 2

  // #define ENABLE_DEVFEATURE_OTA_FALLBACK_ON_BOOT_LOOPING
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY2P5_CTR "\""
  "}";

  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Power0"
  #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Power1"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Motion0"
  #define D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "Motion1"  
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    //device_names:{"module_name":["relay1","relay2"]}
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION_1_FRIENDLY_NAME_LONG "\""
      "]"    
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D20:00:00\",\"OffTime\":\"00D05:00:00\"},"
    "\"RelayEnabled1\":{\"Enabled\":1,\"OnTime\":\"00D20:01:00\",\"OffTime\":\"00D05:00:00\"}"
  "}";


  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Switch1 Toggle = Relay1 Power Toggle
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "},"
    // Button0 Single Press = Relay0 Power On for 10 seconds tester
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

  /**
   * Motion needs to change, to instead be a rule. ie.
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the motion detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
   * One rule will be required for direction, ie motion started (button low) and motion over (button high)
   * 
   * Similarly, switch change rule will also need to set the relays to be commanded based on how long I want
   * 
   * Example
   * 
   * Rule0
   * - Switch 0 = Low, Motion0 started
   * 
   * Rule1
   * - Switch 0 = low, Relay0 on for X minutes   (time of day on relay operation will be controlled via relay_commands, to set operation time ranges)
   * 
   * Rule2
   * - Switch 1 = Low, Motion1 started
   * 
   * Rule3
   * - Switch 1 = low, Relay1 on for X minutes
   * 
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Relay0 Power ON for Timed seconds
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "},"
    // Switch1 HIGH = Relay1 Power ON for Timed seconds
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1"      // 2 meaning either low or high, 1 would be high only
      "},"
      "\"Command\":{"
        // Module and Function omitted, as not needed, for this example
        "\"JsonCommands\":\"{\\\"PowerName\\\":1,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "},"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "},"
    // Switch1 HIGH = Motion1 Event Started, ie report as motion with motion name
    "\"Rule3\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1" // 
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "},"
    // Button0 Single Press = Relay0 Power On for 10 seconds tester
    "\"Rule4\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS


#endif // DEVICE_NAME



//------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/**
 * Device Type: Shelly 1
 * Modules:   
 *  - 2 Mains " D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "
 *  - 2 Mains Inputs
 *  - Energy monitoring (Voltage, Current and power)
 *  - Reset button on PCB
 *  - LED
 * Notes: Requires neutral
 * 
 * Default configuration
 * Mode A (Inputs are switches eg basic wall switch)
 * Mode B (Inputs are motion events eg security light)
 * 
 * */
#ifdef DEVICE_TESTBED_SHELLY1_01
  #define DEVICENAME_CTR          "testbed_shelly1_01"
  #define DEVICENAME_FRIENDLY_CTR "Shelly1 Tester"
  #define DEVICENAME_ROOMHINT_CTR   "Outdoor|Driveway"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
   
  /*
    Method should only activate if boot loop happens 10 times
    Method A: Switch to Wifi.begin hardcoded ssid/pw, start OTA and wait, rebooting every 10 minutes if wifi does not connect
    Method B: Begin wifi.ap as host, so I can directly connect to it via x.x.x.x
  */
  //#define DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS

  #define ENABLE_DEVFEATURE_RELAY_ENABLE_TIME_WINDOW_LOCKS

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_NETWORKS_MQTT

  #define USE_MODULE_CORE_RULES

  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  
  #define USE_MODULE_DRIVERS_RELAY
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY1_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
    
  #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Side Door"
  #define D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "Driveway Middle" 
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_0_FRIENDLY_NAME_LONG "\""
      "]"    
    "},"
    "\"RelayEnabled0\":{\"Enabled\":1,\"OnTime\":\"00D21:00:00\",\"OffTime\":\"00D05:00:00\"}"
  "}";


  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 Toggle = Relay0 Power Toggle
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2" // 3 (or other) means follow, so copy input from trigger
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_A_SWITCHES_TOGGLE_OUTPUTS

  /**
   * Motion needs to change, to instead be a rule. ie.
   * 
   * Switch, button, distance etc changes will trigger a rule which fires the motion detection class. This will then respond via mqtt that event/sensor input "X" occured, and what time etc.
   * One rule will be required for direction, ie motion started (button low) and motion over (button high)
  
   * Similarly, switch change rule will also need to set the relays to be commanded based on how long I want
   * Example
   * Rule0
   * - Switch 0 = Low, Motion0 started
   * Rule1
   * - Switch 0 = low, Relay0 on for X minutes   (time of day on relay operation will be controlled via relay_commands, to set operation time ranges)
   * Rule2
   * - Switch 1 = Low, Motion1 started
   * Rule3
   * - Switch 1 = low, Relay1 on for X minutes
   * */
  #ifdef DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Relay0 Power ON for Timed seconds
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":10}}\""
      "}"
    "},"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":1" // Started
      "}"
    "}"
  "}";
  #endif // DEVICE_DEFAULT_CONFIGURATION_MODE_B_SWITCHES_ARE_MOTION_DETECTION_TRIGGERING_TIMED_OUTPUTS




#endif


#ifdef DEVICE_TESTBED_MOTION_CLIMATE_SENSOR
  #define DEVICENAME_CTR          "testbed_mot_clim_sens"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_DOOR

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
      #ifdef USE_MODULE_SENSORS_DOOR
      "\"D7\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"D6\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\""
      #endif
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
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    // "},"
    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";
  

#endif

/**
 * @brief Test sensor code
 * 
 */
#ifdef DEVICE_TESTBED_HARDWARE_BME_ESP8266
  #define DEVICENAME_CTR          "testbed_bme"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_MOTION
  // #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
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
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    // "},"
    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";
  

#endif

#ifdef DEVICE_TESTBED_HARDWARE_BME_ESP32
  #define DEVICENAME_CTR          "testbed_bme"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Motion Climate Sensor"

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_SWITCHES
  // #define USE_MODULE_SENSORS_MOTION
  // #define USE_MODULE_SENSORS_DOOR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_BME
      "\"18\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
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
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    // "},"
    // // Switch0 HIGH = Motion0 Event Started, ie report as motion with motion name
    // "\"Rule1\":{"
    //   "\"Trigger\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"On\""
    //   "},"
    //   "\"Command\":{"
    //     "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
    //     "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
    //     "\"DeviceName\":1,"
    //     "\"State\":\"Follow\""
    //   "}"
    "}"
  "}";
  

#endif


// #ifdef DEVICE_SHELLYDIMMER_SPAREROOM_CEILING
//   #define DEVICENAME_CTR          "shellydimmer_spareroom"
//   #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  
//   //#define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 2

//   #define ENABLE_DEVFEATURE_SHELLYDIMMER2_INVERTED_EDGE_FOR_ERROR
  
//   #define USE_MODULE_SENSORS_SWITCHES

//   #define USE_MODULE_CORE_RULES
//   #define USE_HARDWARE_DEFAULT_RULES_1

//   #define DISABLE_SERIAL_LOGGING //temp measure
//   #define DISABLE_SERIAL0_CORE //dont think its needed

//   #define USE_MODULE_DRIVERS_SHELLY_DIMMER

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
//   "}";

//   #define D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "Light"
//   #define D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "Switch1"
//   #define D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "Switch2"
  
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "\""
//       "]"
//     "}"
//   "}";

// #endif


#ifdef DEVICE_TESTBED_SHELLY2_DIMMER
  #define DEVICENAME_CTR          "testbed_shellydimmer_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Shelly Dimmer 2"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_DRIVERS_SHELLY_INTERFACE
  #define USE_MODULE_DRIVERS_SHELLY_DIMMER
  #define DISABLE_SERIAL_LOGGING //temp measure
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
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
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\"," 
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":50"
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":2"
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":2,"
        "\"Value\":100"
      "}"
    "}"   
  "}";

#endif

/**
 * Testing motion triggers and rules
 * PIR and doppler, single GPIO input with timed output
 * */
#ifdef DEVICE_TESTBED_MOTION
  #define DEVICENAME_CTR            "testbed_motion" APPEND_ESP_TYPE_MQTT_STRING
  #define DEVICENAME_FRIENDLY_CTR   "testbed_motion" APPEND_ESP_TYPE_NAME_STRING
  
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION

  #define USE_MODULE_CORE_RULES

  // next step, create a that unkown "getsensoranme" will return the friendly class name

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef ESP8266
        #ifdef USE_MODULE_SENSORS_MOTION
        "\"D6\":\""  D_GPIO_FUNCTION_SWT1_INV_CTR "\","
        "\"D1\":\""  D_GPIO_FUNCTION_SWT2_INV_CTR "\","
        "\"D2\":\""  D_GPIO_FUNCTION_SWT3_INV_CTR "\","
        #endif
        "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #else
        #ifdef USE_MODULE_SENSORS_MOTION
        "\"23\":\""  D_GPIO_FUNCTION_SWT1_INV_CTR "\","
        #endif
        "\"19\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIR1"
  #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "PIR2"
  #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "DOP1"

  /**
   * @brief 
   * On value/off value (use | naming) ie On|Off,Open|Closed,Locked|unlocked, 1|0
   *  
   */
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";
    
  /**
   * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
   * */
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // PIR detected
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "},"
    "\"Rule1\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":1,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "},"
    "\"Rule2\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":2,"
        "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":2,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
        "\"State\":2" // Started
      "}"
    "}"
  "}";

#endif

// /**
//  * Testing motion triggers and rules
//  * PIR and doppler, door locks 
//  * */
// #ifdef DEVICE_TESTBED_MOTION
//   #define DEVICENAME_CTR            "testbed_motion"
//   #define DEVICENAME_FRIENDLY_CTR   "testbed_motion ESP8266"
  
//   #define USE_MODULE_SENSORS_INTERFACE
//   #define USE_MODULE_SENSORS_SWITCHES
//   #define USE_MODULE_SENSORS_DOOR
//   #define USE_MODULE_SENSORS_MOTION
//   #define USE_MODULE_SENSORS_BH1750

//   #define USE_MODULE_CORE_RULES

//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//       #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
//       "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
//       #endif
//       #ifdef USE_MODULE_SENSORS_MOTION
//       "\"D6\":\"" D_GPIO_FUNCTION_SWT1_INV_CTR "\","
//       #endif
//       #ifdef USE_MODULE_SENSORS_DOOR
//       "\"D5\":\"" D_GPIO_FUNCTION_DOOR_OPEN_CTR     "\","
//       "\"D7\":\"" D_GPIO_FUNCTION_DOOR_LOCK_CTR     "\","
//       #endif
//       "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";
    
//   #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Motion0"
//   #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "Motion1"

//   /**
//    * @brief 
//    * On value/off value (use | naming) ie On|Off,Open|Closed,Locked|unlocked, 1|0
//    *  
//    */


//   // #define D_DEVICE_SENSOR_DOOR_OPENING_FRIENDLY_NAME_LONG "Motion1"  // what the motion will say when it opens
//   // #define D_DEVICE_SENSOR_DOOR_POSITION_FRIENDLY_NAME_LONG "Motion1" // open/closed?
//   // #define D_DEVICE_SENSOR_DOOR_LOCK_FRIENDLY_NAME_LONG "Motion1"     // locked/unlock
  
//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\","
//         "\"" D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\":["
//         "\"" "bedroomDOOR" "\","
//         "\"" "bedroomlock" "\""
//       "]"
//     "}"
//   "}";
    
//   /**
//    * In the future, make a way to push this exact rule via single command (append new rule, start using vectors for indexing?)
//    * */
//   #define USE_RULES_TEMPLATE
//   DEFINE_PGM_CTR(RULES_TEMPLATE)
//   "{"
//     // PIR detected
//     "\"Rule0\":{"
//       "\"Trigger\":{"
//         "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\","
//         "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
//         "\"DeviceName\":0,"
//         "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
//         "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
//         "\"DeviceName\":0,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
//         "\"State\":2" // Started
//       "}"
//     "}," // DOOR opened
//     "\"Rule1\":{"
//       "\"Trigger\":{"
//         "\"Module\":\"" D_MODULE_SENSORS_DOOR_FRIENDLY_CTR "\","
//         "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\","
//         "\"DeviceName\":0,"
//         "\"State\":1" // FOLLOW, ie command follows trigger, or follow_inv, ie command is inverted to source
//       "},"
//       "\"Command\":{"
//         "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
//         "\"Function\":\"" D_FUNC_EVENT_MOTION_STARTED_CTR "\","
//         "\"DeviceName\":1,"     // Index of motion to be used for name eg garage, motion, then time from when mqtt is sent
//         "\"State\":2" // Started
//       "}"
//     "}"
//   "}";

// #endif




/**
 * Testbed for multiple types of climate sensors
 * - DHT
 * - BME280
 * - DS18B20
 * 
 * Add 4 LEDs to this, it can also be a hvac tester with 4 different temp inputs to control "relays" at different climate ranges for zone control
 * */
#ifdef DEVICE_TESTBED_CLIMATE_SENSOR_BREADBOARD
  #define DEVICENAME_CTR          "testbed_climate_sensor" APPEND_ESP_TYPE_MQTT_STRING  
  #define DEVICENAME_FRIENDLY_CTR "Testbed DS18XX" APPEND_ESP_TYPE_NAME_STRING
  
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_DS18X
  // #define USE_MODULE_SENSORS_DHT

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"   
      #ifdef USE_MODULE_SENSORS_DHT
      "\"18\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
      "\"19\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","
      #endif
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  

  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "DB_01"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "DB_02"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,205,206,170,25]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "DB_03"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,134,175,63]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "DB_04"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,205,202,237,231]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "DB_05"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,29,195,135,126,242]"
  

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
        D_DEVICE_SENSOR_DB18S20_4_ADDRESS ""
      "]"  
    "}"
  "}";
  
#endif


/**
 * New heating controller, designed to work from single device to multizone system
 * 
 * 
 * */
#ifdef DEVICE_TESTBED_DS18XX
  #define DEVICENAME_CTR          "testbed_ds18xx" APPEND_ESP_TYPE_MQTT_STRING  
  #define DEVICENAME_FRIENDLY_CTR "Testbed DS18XX" APPEND_ESP_TYPE_NAME_STRING
  
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_DS18X

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef ESP8266
      "\"D5\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      #else
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"15\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      #endif // esp8266
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  

  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "Room_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,12,164,2,0,0,0,72]"
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "Desk_DB18S20"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,50,199,3,0,0,0,102]"
  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "Boiler_Pipe"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,9,77,4,0,0,0,131]"
  #define D_DEVICE_SENSOR_DB18S20_3_NAME        "Immersion_Heater"
  #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,195,112,2,0,0,0,178]"
  #define D_DEVICE_SENSOR_DB18S20_4_NAME        "Tank_Top"
  #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,255,100,30,7,142,92,154]"
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "Tank_Middle"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,30,7,143,73,252]"  
  #define D_DEVICE_SENSOR_DB18S20_6_NAME        "Tank_Middle6"
  #define D_DEVICE_SENSOR_DB18S20_6_ADDRESS     "[40,121,172,3,0,0,0,138]"
  #define D_DEVICE_SENSOR_DB18S20_7_NAME        "Tank_Middle7"
  #define D_DEVICE_SENSOR_DB18S20_7_ADDRESS     "[40,103,49,3,0,0,0,153]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_6_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_7_NAME "\""
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
    "}"
  "}";
  
#endif



//rgbmicro2/set/light///Scene
//{"//SceneName":"COLOUR//Scene","hue":25,"sat":100,"brt_rgb":100,"cct_temp":500,"brt_cct":100,"Time":0,"time_on":3600}
#ifdef DEVICE_TESTBED_RGBCLOCK
  #define DEVICENAME_CTR          "testbed_rgbclock"
  #define DEVICENAME_FRIENDLY_CTR "RGB Clock 02"

  // TEST DEVICE, NOT WORKING
 
  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  //#define USE_TASK_RGBLIGHTING_NOTIFICATIONS  // BREAKS
  #define USE_SK6812_METHOD_DEFAULT

  #define ENABLE_DEVFEATURE_RGB_CLOCK

  // #define ENABLE_GAMMA_BRIGHTNESS_ON_DESIRED_COLOUR_GENERATION
  // // #define ENABLE_FEATURE_PIXEL_GROUP_MULTIPLIERS
  // // #define USE_DEVFEATURE_PIXEL_OUTPUT_MULTIPLIER 2

  // // #define ENABLE_BUG_TRACING
  // // #define DEBUG_PRINT_FUNCTION_NAME
  // //  #define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  // //#define ENABLE_LOG_FILTERING_TEST_ONLY
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION
  // // #define USE_DEBUG_PRINT_FUNCTION_NAME_TEST
  // #define DISABLE_WEBSERVER 
  
  // #define ENABLE_PIXEL_FUNCTION_MIXER
  
  // //#define ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL

  #define STRIP_SIZE_MAX 93// 750   *15 //changing gazebo to be 12v

  // #define DISABLE_NETWORK
  // // #define DISABLE_FOR_FAULTY_ESP32_FLICKERING

  // #define DISABLE_SLEEP
    
  #define DISABLE_WEBSERVER

  // #define USE_MODULE_SENSORS_BME
  
  // #define USE_MODULE_SENSORS_MOTION


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
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grbw\","
    "\"" D_JSON_TRANSITION       "\":{"
      "\"" D_JSON_TIME_MS "\":0,"
      "\"" D_JSON_RATE_MS "\":5000,"
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
    "\"" D_JSON_COLOUR_PALETTE "\":34,"
    "\"BrightnessCCT\":0,"
    "\"" D_JSON_BRIGHTNESS_RGB "\":5"

  "}";


#endif



/**
 * Basic version of measurement system
 * GPS will be recorded at 10Hz, and logged to SD card in json format for matlab parsing
 * */
#ifdef DEVICE_TESTBED_GPS_SDCARD_LOGGER

#define DEVICENAME_CTR            "node0"
  #define DEVICENAME_FRIENDLY_CTR   "node0"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "GPS"

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
  #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
  #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
  #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
  #define USE_SYSTEM_GPS_SD_LOGGER_CONTROLLER
  #define USE_SYSTEM_SDCARD_LOGGING
  //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  /**
   * Debug methods
   * */
  /**
   * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
   * */
  // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  // #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17
  #define SERIAL_DEBUG_BAUD_DEFAULT 115200//921600

  /**
   * If enabled, disable normal logging methods
   * */
  #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
    #define DISABLE_SERIAL_LOGGING
    #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_SERIAL0_ESP32_OUTPUT
  #endif // USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

  /**
   * General defines to disable systems not needed
   * */
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  /**
   *  GPS
   * */
  #ifdef USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
    #define USE_MODULE_DRIVERS_GPS
    #define ENABLE_GPS_PARSER_NMEA
    #define ENABLE_GPS_PARSER_UBX
    #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
    #define NMEAGPS_DERIVED_TYPES
    #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
    #define NMEAGPS_PARSE_SAVE_MILLIS
    #define gpsPort Serial1
    #define D_GPS_BAUD_RATE_FAST    921600
    #define D_GPS_BAUD_RATE_DEFAULT 9600
    #define USE_MODULE_DRIVERS_INTERFACE
    #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_1
    #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif // USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS

  /**
   * Comms with pic32
   * */
  #ifdef USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
    #define USE_MODULE_DRIVERS_INTERFACE
    #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_2
    #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
    #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
    #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
    #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  #endif // USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM

  /**
   * SDCard
   * */
  #ifdef USE_SYSTEM_SDCARD_LOGGING
    #define USE_MODULE_DRIVERS_SDCARD
    #define USE_SDCARD_RINGBUFFER_STREAM_OUT
  #endif // USE_SYSTEM_SDCARD_LOGGING

  /**
   * Logger Controller
   * */
  #ifdef USE_SYSTEM_GPS_SD_LOGGER_CONTROLLER
    #define USE_MODULE_CONTROLLER_GPS_SD_LOGGER
    #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
    #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
    #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
  #endif // USE_SYSTEM_LOGGER_CONTROLLER

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

  /**
   * Using Serial1 (which is RX RSS) for transmit of logging without requiring disconnect for flashing
   * */
  #ifdef USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
    #define USE_MODULE_DRIVERS_INTERFACE
    #define USE_MODULE_DRIVERS_SERIAL_UART
    #define ENABLE_HARDWARE_UART_2
    #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
    #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
    #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
    #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT
  #endif // USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  
  /**
   * I2S Internal Fast Sampling
   * */
  #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
    // #define ENABLE_ESP32_ADC_SAMPLING // not used in aplha
    // // Next phase, will include saving adc values
    // // #define USE_MODULE_SENSORS_ADC_INTERNAL
    // #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL
    // #define ENABLE_SMOOTHING_ON_ADC_READING
    // #define ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    // #define ENABLE_ADC_I2S_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
    // // #define ADC_CAPTURE_EXTERNAL_PIN 32
    // #define ADC_CAPTURE_INPUT0_PIN 34
    // #define ADC_CAPTURE_INPUT1_PIN 35
  #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     34(I), ADC2G, ADC1_CH6
       * White      35(I), ADC5G, ADC1_CH7
       * Red        32(I), ADC Record Trigger
       * Black      GND
       * */
      "\"34\":\"" D_GPIO_FUNCTION_ADC1_CH6_CTR   "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","
      "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","
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
      /** 2P small
       * Red        Button Logging Toggle
       * Black      GND
       * */
      "\"23\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  /**
   * Debug pins
   * */
  #define DEBUG_PIN1_GPIO     21
  #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  #define DEBUG_PIN2_GPIO     22
  #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  #define DEBUG_PIN3_GPIO     0 //USED
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing




#endif // DEVICE_GPS_SDCARD_LOGGER



#ifdef DEVICE_TESTBED_ULTRASONIC
  #define DEVICENAME_CTR          "testbed_ultrasonic"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"

  #define USE_MODULE_CONTROLLER_TANKVOLUME
    
  // #define USE_MODULE_SENSORS_DS18X
  
  #define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  //#define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_ULTRASONICS
      "\"D1\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif        
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "BackUpTank"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "GarageOutside"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "[40,143,81,7,51,20,1,189],"   //D6 group of 3                                           
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "}"
  "}";

#endif


/**
 * 
 * */
#ifdef DEVICE_TESTBED_BME_ESP8266
  #define DEVICENAME_CTR          "testbed_bme_esp8266"
  #define DEVICENAME_FRIENDLY_CTR "Testbed BME280"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif


#ifdef DEVICE_TESTBED_BME_ESP32
  #define DEVICENAME_CTR          "testbed_bme_esp32"
  #define DEVICENAME_FRIENDLY_CTR "Testbed BME280"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME

  // try tas to test it for esp32 and i2c scanning

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif



#ifdef DEVICE_TESTBED_BME_ESP8266
  #define DEVICENAME_CTR          "testbed_bme_esp8266"
  #define DEVICENAME_FRIENDLY_CTR "Testbed BME280"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif



#ifdef DEVICE_TESTBED_SENSORS_ESP8266
  #define DEVICENAME_CTR          "testbed_sensor_esp8266"
  #define DEVICENAME_FRIENDLY_CTR "Testbed BME280"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_BH1750

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif





/**
 * esp8266 version -- gy89
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_9AXIS_GRYO
  #define DEVICENAME_CTR          "testbed_9axis_gyro"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"

  //#define USE_MODULE_CONTROLLER_TANKVOLUME
    
  // #define USE_MODULE_SENSORS_DS18X

  #define USE_MODULE_SENSORS_LSM303D
  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !
  
  //#define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  //#define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"D1\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"D2\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "BackUpTank"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "GarageOutside"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "[40,143,81,7,51,20,1,189],"   //D6 group of 3                                           
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "}"
  "}";

#endif



/**
 * esp32 version -- mpu9250
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_9AXIS_GRYO_GY89
  #define DEVICENAME_CTR          "testbed_gy89"
  #define DEVICENAME_FRIENDLY_CTR "Testbed LSM303D2"


  #define DISABLE_NETWORK
  #define DISABLE_SLEEP
  
    #define USE_MODULE_SENSORS_LSM303D
    #define USE_MODULE_SENSORS_L3G

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1

  // #ifdef SENSOR_MULTIPLE_MODULE // new way of defined multiple sensors contained on one hardware  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_LSM303D)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "BackUpTank"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "GarageOutside"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "[40,143,81,7,51,20,1,189],"   //D6 group of 3                                           
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "}"
  "}";

#endif



/**
 * esp32 version -- mpu9250
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_6DOF_ECOMPASS_LSM303D
  #define DEVICENAME_CTR          "testbed_6dof_ecompass"
  #define DEVICENAME_FRIENDLY_CTR "Testbed LSM303D2"

  // #define DISABLE_NETWORK
  // #define DISABLE_SLEEP
  
  #define USE_MODULE_SENSORS_LSM303D
  // #define USE_MODULE_SENSORS_L3G

  // #define I2C_ADDRESS_LSM303D 0x1d

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1

  // #ifdef SENSOR_MULTIPLE_MODULE // new way of defined multiple sensors contained on one hardware  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
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


#endif


/**
 * esp32 version -- mpu9250
 * As part of measurement system, add 9 axis gyro (GY89) to sensors
 * */
#ifdef DEVICE_TESTBED_9AXIS_GRYO_MPU9250
  #define DEVICENAME_CTR          "testbed_9axis_gyro"
  #define DEVICENAME_FRIENDLY_CTR "Testbed MPU9260"

  //#define USE_MODULE_CONTROLLER_TANKVOLUME
    
  // #define USE_MODULE_SENSORS_DS18X

  #define USE_MODULE_SENSORS_MPU9250

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1
  

// I2C device found at address 0x1D  !
// I2C device found at address 0x6B  !
// I2C device found at address 0x77  !
  
  //#define USE_MODULE_SENSORS_ULTRASONICS
  // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
  // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK

  //#define ENABLE_DEVFEATURE_DELAYED_RESTART_WITHOTA_FOR_DEBUGGING
  // #define ENABLE_DEVFEATURE_OTAFALLBACK_WITH_FASTBOOT_DETECTED //to be done long term

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_MPU9250)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","
      #endif  
      "\"D6\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
  #define D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "SpeedOfSound_Ambient"
  #define D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "BackUpTank"
  #define D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "GarageOutside"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_TEMP_1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_2_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_TEMP_3_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "[40,143,81,7,51,20,1,189],"   //D6 group of 3                                           
        "[40,255,100,29,205,201,168,203],"                                             
        "[40,255,100,29,205,248,248,249]"  
      "]"  
    "}"
  "}";

#endif


#ifdef DEVICE_ESP32_WEBCAM1 //based on tasmota
  #define DEVICENAME_CTR            "esp32_webcam1"
  #define DEVICENAME_FRIENDLY_CTR   "esp32_webcam1"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define ESP32

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"LBI\":\""  D_GPIO_FUNCTION_PWM1_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_CAM_AITHINKER_CTR "\""
  "}";

#endif // DEVICE_ESP32_DEVKIT_BASIC



#ifdef DEVICE_ESP32_WEBCAM2   //based on git project
  #define DEVICENAME_CTR            "esp32_webcam2"
  #define DEVICENAME_FRIENDLY_CTR   "esp32_webcam2"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  #define DISABLE_SLEEP

  // #define DISABLE_NETWORK

  #define USE_MODULE_DRIVERS_CAMERA_OV2640_2

  // #define USE_DEVFEATURE_DISABLE_FOR_CAMERA

  // #define USE_DEBUG_DISABLE_GLOBAL_PIN_INIT // new set of debug formatted defines, for development only

  #define ESP32

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIOC "\":{"
    //   "\"LBI\":\""  D_GPIO_FUNCTION_PWM1_CTR "\""
    // "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_CAM_AITHINKER_CTR "\""
    // "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_ESP32_DEVKIT_BASIC



#ifdef DEVICE_ESP32_WEBCAM3   //based on tasmota : Feb 2022
  #define DEVICENAME_CTR            "esp32_webcam3"
  #define DEVICENAME_FRIENDLY_CTR   "esp32_webcam3"

  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 1

  // #define DISABLE_SLEEP

  // #define DISABLE_NETWORK

  #define USE_MODULE_DRIVERS_CAMERA_WEBCAM

  // #define USE_DEVFEATURE_DISABLE_FOR_CAMERA

  // #define USE_DEBUG_DISABLE_GLOBAL_PIN_INIT // new set of debug formatted defines, for development only

  #define ESP32

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIOC "\":{"
    //   "\"LBI\":\""  D_GPIO_FUNCTION_PWM1_CTR "\""
    // "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_CAM_AITHINKER_CTR "\""
    // "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_ESP32_DEVKIT_BASIC


/**
 * New heating controller, designed to work from single device to multizone system
 * For development of hvac with pzem to monitor power
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
#ifdef DEVICE_TESTBED_HVAC_HEAT_ONLY_WITH_ENERGY_SENSOR 
  #define DEVICENAME_CTR          "testbed_hvac_energy_sens"
  #define DEVICENAME_FRIENDLY_CTR "Testbed HVAC with Energy Sensor"

  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  #define ENABLE_HVAC_DEBUG_PINS_FOR_TESTING

  #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER

  
    /**
     * @brief 
     * Add special debug method here that will count loglevel by type so "error" messages can be counted and shared every X seconds on terminal
     * 
     */
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  

  #define ENABLE_DEVFEATURE_HVACTESTBED_CODE // for the testbed, so it does not ruin the heating code until functional

  // #define USE_MODULE_CONTROLLER_HVAC
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
      "\"LBI\":\"" D_GPIO_FUNCTION_BUILTIN_LED1_CTR  "\""      
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



/**
 * @brief DEVICE_TESTBED_RTC_FASTBOOT
 * 
 */
#ifdef DEVICE_TESTBED_RTC_FASTBOOT
  #define DEVICENAME_CTR          "testbed_rtc_fastboot"   APPEND_ESP_TYPE_MQTT_STRING                                    // Change: The unique mqtt topic, however, mqtt client names are appended with mac address, so for basic testing (ie of templates) it is not essential this be changed
  #define DEVICENAME_FRIENDLY_CTR "Testbed RTC Fastboot"   APPEND_ESP_TYPE_NAME_STRING                                 // Change: You may change this, but it is not important to do so (more important when webui is functioning)
  
  #define STRIP_SIZE_MAX 55                                                                           // Change: Set *total* length of string, segment0 will default to this length
  #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
  #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

  // #define ENABLE_DEVFEATURE_RTC_FASTBOOT_V2
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    // "\"" D_JSON_GPIOC "\":{"
    // #ifdef ESP8266 
    //   "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    // #else
    //   "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    // #endif
    // "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
    
#endif


#ifdef DEVICE_TESTBED_BUTTON_MULTIPRESS
  #define DEVICENAME_CTR          "testbed_button_longpress"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Button Longpress"

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_DEVFEATURE_BUTTONS_SEND_EVENT_MESSAGES

  #define MAX_KEYS 1                 // Max number of keys or buttons

  #define USE_MODULE_CORE_RULES

  #define ENABLE_DEVFEATURE_BUTTON_HANDLER_V2

  #define ENABLE_DEVFEATURE_BUTTON_MULTIPRESS

  #define ENABLE_DEVFEATURE_PHASEOUT_CLEARING_EVENT

  #define ENABLE_DEVFEATURE_BUTTON_SET_FLAG_BUTTON_SINGLE 0 // allow multipress = false

  #define ENABLE_DEBUG_DEV_BUTTONS
  
  #define USE_MODULE_DRIVERS_RELAY
  #define MAX_RELAYS 1
  #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D7\":\""  D_GPIO_FUNCTION_KEY1_CTR  "\","
      "\"4\":\""   D_GPIO_FUNCTION_REL1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // Builtin Button as toggle
    "\"Rule0\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":2" // toggle
      "}"
    "},"
    // Optional external button on RX pin
    "\"Rule1\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":0," //number, name, or all
        "\"State\":2" // toggle
      "}"
    "},"
    // Optional external button on RX pin
    "\"Rule2\":{" //switch example
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR "\","    //sensor
        "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
        "\"DeviceName\":1," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
        "\"State\":0" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
        "\"Data\":[0,2]" // [state,button_hold]
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
        "\"Function\":\"" D_FUNC_EVENT_SET_POWER_CTR "\","
        "\"DeviceName\":0,"
        "\"JsonCommands\":\"{\\\"PowerName\\\":0,\\\"Relay\\\":{\\\"TimeOn\\\":3}}\""
      "}"
    "}"
  "}";



#endif



#ifdef DEVICE_FORCED_TO_BE_TESTER
  #undef DEVICENAME_CTR
  #define DEVICENAME_CTR "testerdevice"
  #warning "--------------------------------------------------------------- DEVICE_FORCED_TO_BE_TESTER" is enabled for this compile
#endif


#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

