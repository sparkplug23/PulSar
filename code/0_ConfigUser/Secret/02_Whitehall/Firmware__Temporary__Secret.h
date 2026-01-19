 #ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_HOME_TEMPORARY_TEMPLATES_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_HOME_TEMPORARY_TEMPLATES_H


/*********************************************************************************************\
 * Personal configs for installed home devices as of 2023
 * ** Will be turned off when I leave
\*********************************************************************************************/

// #define USE_MODULE_NETWORK_MQTT
// #define DEVICE_FORCED_TO_BE_TESTER
// #define ENABLE_TESTUSER
// #define DISABLE_WEBSERVER
//#define FORCE_TEMPLATE_LOADING
// #define USE_MODULE_CORE_RULES

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * @brief Bedroom
 * 
 */
// #define DEVICE_PORTABLE_TEMPERATURE_SENSOR
// #define DEVICE_H801__MASTERBEDROOM_UPLIGHT
// #define DEVICE_TEMPORARY__LIGHTING__HYPERION_LIGHT_SAMSUNG_65INCH

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Bedroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/







/**
 * @brief 
 * 
 * 
 **/
#ifdef DEVICE_PORTABLE_TEMPERATURE_SENSOR
  #define DEVICENAME_CTR          "portable_bath_sensor"
  #define DEVICENAME_FRIENDLY_CTR "Portable Bath Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define MQTT_HOST   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SSD1306
  #define SHOW_SPLASH

  #define USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
    
  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"   
      /** Built in - OLED
       * 
       * */
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023   
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\""
      #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1
  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "PortableBathTop"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,174,159,3,0,0,0,1]" 
  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "PortableBathBottom"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,68,132,149,240,1,60,87]"


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\""
      "]"  
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":[" 
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\":" D_DEVICE_SENSOR_DB18S20_1_ADDRESS ""
      "]"  
    "},"    
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // "\"AddRuleDefault\":\"Motion0=0\","
    // MOTION
    "\"Rule0\":{"
      "\"Trigger\":{"
        "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
        "\"DeviceName\":0,"
        "\"State\":\"On\""
      "},"
      "\"Command\":{"
        "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
        "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
        "\"DeviceName\":0," 
        "\"State\":\"Follow\""
      "}"
    "}"
  "}";


#endif


#ifdef DEVICE_H801_INSIDE_BEDROOM_WARDROBE

  #define DEVICENAME_CTR          "h801_bedroom_wardrobe"
  #define DEVICENAME_FRIENDLY_CTR "H801 h801_bedroom_wardrobe 3"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define MQTT_HOST   "192.168.1.70"

    #define MQTT_PORT     1883


  #define USE_SERIAL_ALTERNATE_TX
  
  // #define USE_MODULE_DRIVERS_PWM

#define USE_SERIAL_ALTERNATE_TX
  

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // 
  // 
    // 

  // #define ESP8266
  // #undef ESP32
  

  


    



  // 
  // 

  // #define ESP32
  // #undef ESP8266

  // #define ENABLE_DEVFEATURE_SYSTEM__UNSORTED_CODE

  // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
  // 

  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
  // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

  


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES


  #define USE_MODULE_SENSORS_SUN_TRACKING

  //mqtt debug
  #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
  // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // #define DISABLE_NETWORK
  // #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_WIFI
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  #define ANIMATION_UPDATOR_TIME_MINIMUM 20

  #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #undef ESP32
  // #define ESP8266

  

  #define ENABLE_DEBUG_MANUAL_DELAYS

    /********* Group: Needed to build ************************/
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    /********* Group: Ready for full integration ************************/
    // 
    /********* Group: Testing ************************/
    
    #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
    
    
    
    
    #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
    #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
    // 
    /********* Group: Debug options only ************************/
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
    // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
    #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
    #define ENABLE_DEBUG_SERIAL

    // Needs significant change to merge WLED and HACS
    
    // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

    #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

    #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

    // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

    // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
    // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
    #define ENABLE_DEBUG_LINE_HERE
    #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

    #define ENABLE_DEVFEATURE_LIGHT__HYPERION
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
    // #

    #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD





    // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS

    // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
    // #define ENABLE_LOG_LEVEL_DEBUG
    // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
    // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

    
    

    // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
    



//   #define USE_SERIAL_ALTERNATE_TX
//   
//   // #define USE_MODULE_DRIVERS_PWM


  

//     


// // Step A: Replicate WLED to function as it was designed on the webui part. Only when this works for effect/colour controls etc, slowly change it to add more of my original stuff. 
// // Gain complete understanding/implemenation as is, only then start changes. 
// // WebUI of WLED style should become its own webserver option (not just the debug version).

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


//   // #define ENABLE_DEVFEATURE_MQTT__TRYING_TO_USE_ADDHANDLER_INSIDE_MQTT_CAPTURED
//   // 

//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
//   // #define USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER

//   


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES


//   //mqtt debug
//   #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 2
//   // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

//   // #define DISABLE_NETWORK
//   // #define DISABLE_NETWORK_WIFI
//   #define USE_MODULE_NETWORK_WIFI
//   #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

//   #define ANIMATION_UPDATOR_TIME_MINIMUM 20

//   #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR

//   // #define DISABLE_SERIAL
//   // #define DISABLE_SERIAL0_CORE
//   // #define DISABLE_SERIAL_LOGGING

//   // #undef ESP32
//   // #define ESP8266

//   

//   #define ENABLE_DEBUG_MANUAL_DELAYS

//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//   #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
//   // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

//     /********* Group: Needed to build ************************/
//     #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
//     /********* Group: Ready for full integration ************************/
//     // 
//     /********* Group: Testing ************************/
//     #define ENABLE_DEVFEATURE_NEOSPEED_ESP32_I2S_WS2812_METHOD
//     
//     #define ENABLE_DEVFEATURE_COLOR_WHEEL_CHANGED
//     
//     
//     #define DISABLE_DEVFEATURE_MULTIPIN_BUSSES_REMOVING_CODE_NOT_NEEDED
//     #define ENABLE_DEVFEATURE_REMOVE_OLD_NEOPIXEL_BUS_METHOD_ONLY_WHEN_FULLY_PHASED_OUT
//     
//     #define ENABLE_DEVFEATURE_CREATE_MINIMAL_BUSSES_SINGLE_OUTPUT
//     #define ENABLE_DEVFEATURE__PIXELS_ENABLE_COLOUR_ORDER_CONVERSION_WITHOUT_COPY_OF_EXTERNAL_ORDER
//     // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL0_DEVELOPING            // Development and testing only
//     #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL1_MINIMAL_HOME             // Basic/Static just for home
//     #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
//     #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
//     #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
//     // 
//     /********* Group: Debug options only ************************/
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_ENCODING
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_DATA_LENGTH
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CONTAINER
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_ARRAY
//     // #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE__SHOW_ALL_PALETTE_NAMES_AS_OBJECT_INDEXED_LIST
//     #define ENABLE_DEVFEATURE_PALETTE_GET_NAMES_FROM_PALETTE_WHEN_STATIC
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_HARDWARE
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS_NEW
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR__DEBUG_PALETTE_VECTOR
//     #define ENABLE_DEBUG_FEATURE_SEGMENT_PRINT_MESSAGES // WLED _DEBUG
//     #define ENABLE_DEBUG_SERIAL

//     #define ENABLE_DEVFEATURE_LIGHT__WEBUI_APPEND_EFFECT_CONFIG_TO_JSON_RESPONSE

//     // Needs significant change to merge WLED and HACS
//     
//     // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_ORDER_IN_MULTIPIN_SHOW_LOGS

//     #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__BUS_CONFIG

//     #define ENABLE_DEVFEATURE_LIGHT__BRIGHTNESS_GET_IN_SEGMENTS_INCLUDES_BOTH_SEGMENT_AND_GLOBAL

//     #define ENABLE_DEVFEATURE_LIGHT__WLED_WEBUI_SEND_MY_PALETTE_COLOUR_BARS

//     // #define ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST

//     // #define ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
//     // #define ENABLE_DEVFEATURE_LIGHT__ONLY_ENABLE_WRITING_TO_ANIMATION_IF_PINNED_TASK_NOT_ACTIVE
    
//     #define ENABLE_DEBUG_LINE_HERE
//     #define ENABLE_DEVFEATURE_PALETTE__FIX_WEBUI_GRADIENT_PREVIEW

//     #define ENABLE_DEVFEATURE_LIGHT__HYPERION
//     #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE_CRGB16PALETTE
//     // #

//     #define ENABLE_DEVFEATURE_LIGHT__ESP32_USE_I2S_CHANNELS_AS_PRIMARY_METHOD


//     


//     // #define ENABLE_DEVFEATURE_LIGHT__PERMIT_PIXEL_INDEXING_GREATER_THAN_FIRST_ON_PWM_CHANNELS_FOR_MULTIPLE_SEGMENTS



//     // #define ENABLE_DEBUG_POINTS_GetColourFromPreloadedPalette
//     // #define ENABLE_LOG_LEVEL_DEBUG
//     // #define ENABLE_DEBUG_TRACE__ANIMATOR_UPDATE_DESIRED_COLOUR
//     // #define ENABLE__DEBUG_POINT__ANIMATION_EFFECTS   // "DEBUG_POINT" is the new unified way of turning on temporary debug items

//     
    

//     // #define ENABLE_DEVFEATURE_LIGHT__PRELOAD_BUSCONFIG_FROM_TEMPLATE_AS_TEMPORARY_MEASURE
//     

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_AUGUST_2023


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{" 
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_A_CTR "\":15,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_B_CTR "\":13,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_C_CTR "\":12,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_D_CTR "\":14,"  // PWM RGBCCT
      "\"" D_GPIO_FUNCTION_PIXELBUS_01_E_CTR "\":4"  // PWM RGBCCT
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":1},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  "}";



  #define USE_LIGHTING_TEMPLATE
  #define USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS

  #ifdef USE_LIGHTING_TEMPLATE__H801_FIVE_WHITE_CHANNELS_FIVE_SEGMENTS
    #define STRIP_SIZE_MAX 5
    #define ENABLE_DEVFEATURE_LIGHT__BUS_MANAGER_DEFAULT_FORCED_AS_PWM
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":[15],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":0,
          "Length":1
        },
        {
          "Pin":[13],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":1,
          "Length":1
        },
        {
          "Pin":[14],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":2,
          "Length":1
        },
        {
          "Pin":[4],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":3,
          "Length":1
        },
        {
          "Pin":[12],
          "ColourOrder":"W",
          "BusType":"ANALOG_1CH",
          "Start":4,
          "Length":1
        }
      ],      
      "Segment0":{
        "PixelRange": [
          0,
          2
        ],
        "ColourPalette":0,
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":100,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":10
      },      
      "Segment1":{
        "PixelRange": [
          3,
          5
        ],
        "ColourPalette":0,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":100,
          "ColourType":5
        },
        "Effects": {
          "Function":0
        },
        "Transition": {
          "TimeMs": 0,
          "RateMs": 1000
        },
        "BrightnessRGB":100,
        "BrightnessCCT":10
      },
      "BrightnessRGB":100,
      "BrightnessCCT":10
    }
    )=====";


#endif // DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS


#endif




/*************************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * Office ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************
 * ***********************************************************************************************************************************************************/





#ifdef DEVICE_TVROOM_H801_UPLIGHTS
  #define DEVICENAME_CTR          "h801_tvroom_uplights"//"h801_spareroom_wardrobe"
  #define DEVICENAME_FRIENDLY_CTR "H801 Spareroom Wardrobe"
  #define DEVICENAME_ROOMHINT_CTR "TV Room"
  #define MQTT_HOST   "192.168.1.70"
      
  #define USE_SERIAL_ALTERNATE_TX
  

  #define DISABLE_WEBSERVER
    
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
    
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
    "\"" D_STRIP_SIZE       "\":1,"
    "\"" D_RGB_COLOUR_ORDER   "\":\"RGBCW\","
    "\"" D_TRANSITION     "\":{\"" D_TIME "\":2,\"" D_RATE "\":20,\"" D_ORDER "\":\"" D_INORDER "\"},"
    "\"" D_COLOUR_PALETTE "\":\"Solid Rgbcct 00\","
    "\"" D_ANIMATIONMODE  "\":\"Effects\","
    "\"" D_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_BRIGHTNESS     "\":100"
  "}";
  
#endif




#ifdef DEVICE_RGBFIREPLACE
  #define DEVICENAME_CTR            "rgbfireplace"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Fire Place"
  #define DEVICENAME_ROOMHINT_CTR   "Livingroom"
  #define MQTT_HOST   "192.168.1.70"
  
  #define DISABLE_WEBSERVER

  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
     // Not ready to remove
    #define STRIP_SIZE_MAX 300
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins

    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
    "\"" D_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"AnimationMode\": \"Effects\","
    "\"ColourOrder\": \"grb\","
    "\"ColourPalette\":\"Single Flame 01\","
    "\"Effects\":{"
      "\"Function\":\"FirePlace01\","
      "\"Intensity\":255,"
      "\"Speed\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":0,"
      "\"RateMs\":23"
    "},"    
    "\"BrightnessRGB\":50,"
    "\"TimeOn\":3600"
  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE
  
#endif

#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_HOME_TEMPORARY_TEMPLATES_H
