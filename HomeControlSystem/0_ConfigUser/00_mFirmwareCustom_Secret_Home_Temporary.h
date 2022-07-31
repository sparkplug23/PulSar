 #ifndef MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_TEMPORARY_H
#define MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_TEMPORARY_H


/*********************************************************************************************\
 * Personal configs for installed home devices as of 2022 -- getting ready for leaving home
\*********************************************************************************************/

#define USE_MODULE_NETWORKS_MQTT
// #define DEVICE_FORCED_TO_BE_TESTER
// #define ENABLE_TESTUSER
// #define DISABLE_WEBSERVER
//#define FORCE_TEMPLATE_LOADING
#define USE_MODULE_CORE_RULES

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

//--------------------------------[Enable Device]-------------------------------------


/**
 * @brief Bedroom
 * 
 */
// #define DEVICE_H801_BEDROOM_UNDERDESK
// #define DEVICE_SHELLYDIMMER_BEDROOM_LAMP
// #define DEVICE_HVAC_HAIRDRYER
// #define DEVICE_DESKSENSOR
// #define DEVICE_SHELLYDIMMER_BEDROOM_LAMP
// #define DEVICE_RGB_COMPUTER_SCREEN_DELL_U2515H // 3rd display (far left)
// #define DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE   // 1st New primary display
// #define DEVICE_RGBCLOCK_BEDROOM_WALL                             // Make this into an alarm? flashing effect and buzzer/speaker with esp32
// #define DEVICE_RGBDISPLAY_BEDROOM_OUTSIDE_TEMPERATURE
// #define DEVICE_BLACK_STAND_LIGHT



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Bedroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * @brief Device will largely remain active, as the primary test device for H801 code considered for deployment
 * 
 */
#ifdef DEVICE_H801_BEDROOM_UNDERDESK
  #define DEVICENAME_CTR          "h801_bedroom_underdesk"
  #define DEVICENAME_FRIENDLY_CTR "H801 UnderDesk"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70


  // #define ENABLE_DEBUG_MESSAGE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING


  #define ENABLE_DEVFEATURE_DEBUG_PWM_CHANNELS_MQTT


  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    // #define ENABLE_DEVFEATURE_TRANSITION_MODE_LEVEL_OPTIONS
    // #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_OLD_FUNCTIONS
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

    
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL0_DEVELOPING
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL1_MINIMAL_HOME
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__SUN_POSITIONS
// #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
// #define ENABLE_WLED_EFFECTS



/***
 * 
 * 
 * 

{
  "ColourOrder":"RGBCW",
  "ColourPalette": "Solid Rgbcct 00",
  "Hue": 350,
  "Sat": 100,
  "AnimationMode": "Effects",
  "Effects": {
    "Function": "Solid"
  },
  "BrightnessRGB": 100,
  "BrightnessCCT": 100,
  "CCT_TempPercentage": 100,
  "Transition": {
    "RateMs": 4000,
    "TimeMs": 5000
  },
  "TimeOn": 3600
}

{
  "ColourPalette": "Solid Rgbcct 00",
  "Hue": 350,
  "Sat": 100,
  "AnimationMode": "Effects",
  "Effects": {
    "Function": "Solid"
  },
  "BrightnessRGB": 100,
  "BrightnessCCT": 100,
  "CCT_TempPercentage": 100,
  "Transition": {
    "RateMs": 0,
    "TimeMs": 100
  },
  "TimeOn": 3600
}


 * 
 * 
 * */


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 2 // temporary fix!
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_JSON_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS_RGB   "\":0,"
    "\"" D_JSON_BRIGHTNESS_CCT   "\":0"
  "}";

#endif


/**
 * New heating controller, designed to work from single device to multizone system
 * */
#ifdef DEVICE_HVAC_HAIRDRYER
  #define DEVICENAME_CTR          "hvac_bedroom_dryer"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Bedroom Hairdryer"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1 //maintain other settings (bootcount)
   
  // #define ENABLE_BUG_TRACING
  //#define ENABLE_MQTT_DEBUG_MESSAGES

  //#define FORCE_DEVICENAME_CLEAR_ON_BOOT
  // #define ENABLE_HVAC_DEBUG_TIMES
  #define DISABLE_WEBSERVER
  
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
  #define EMABLE_DEVFEATURE_HARDWAREPINS_CLEANED_UP

  #define USE_MODULE_CONTROLLER_HVAC
  #define HEATING_DEVICE_MAX 4
  
  // #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_DHT
  // // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_DS18X
  // #define USE_MODULE_SENSORS_REMOTE_DEVICE

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
      "\"D5\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"D1\":\"" D_GPIO_FUNCTION_REL2_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      // "\"D6\":\"" D_GPIO_FUNCTION_REL3_CTR      "\","
      "\"D6\":\"" D_GPIO_FUNCTION_REL4_INV_CTR  "\""
      // "\"D0\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\","
      // "\"D7\":\"" D_GPIO_FUNCTION_DHT22_2_CTR   "\","

      // "\"D5\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
      
      // "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR   "\""    // LBI causing failed boot here
      
      // "\"D5\":\""  D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      // "\"D3\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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

#ifdef DEVICE_BEDROOM_CONTROLLER_BUTTONS_01
  #define DEVICENAME_CTR          "bedroom_controller_buttons_01"
  #define DEVICENAME_FRIENDLY_CTR "Bedside Controller for Room 01"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BUTTONS
  #define ENABLE_DEVFEATURE_BUTTONS_SEND_EVENT_MESSAGES

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"D1\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR  "\","
      "\"D2\":\"" D_GPIO_FUNCTION_KEY2_INV_CTR  "\","
      "\"D3\":\"" D_GPIO_FUNCTION_KEY3_INV_CTR  "\","
      "\"D4\":\"" D_GPIO_FUNCTION_KEY4_INV_CTR  "\","
      "\"D5\":\"" D_GPIO_FUNCTION_KEY5_INV_CTR  "\","
      "\"D6\":\"" D_GPIO_FUNCTION_KEY6_INV_CTR  "\","
      "\"D7\":\"" D_GPIO_FUNCTION_KEY7_INV_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif




/**
 * @brief 
 * 
 * - This will be the test device, full of as many sensors as possible, and also use 4 LEDs to emulate relay outputs
 * - This will be used as a rapid/testbed device for improving as many sensors as possible (at minimum, the "...sensor" devices)
 * 
 * Modules to be tested
 * - DS18B20
 * - DHT22
 * - BME280
 * - BH1750
 * - Status LEDs (mqtt/wifi down)
 * - Nextion = OTA updating
 * - rgbW lights = Notification lights as a mode
 * - Motion sensor = including nicely having interface showing "event" of motion
 * - ESP32 Touch inputs
 * - DAC Output - Mono Speaker Audio
 * - ADC Input (probably with LDR)
 * - Audio Input? MCP3208
 * 
 * development branch only
 * 
 * Use one pixel to show segment1 effect paired with sun elevation, where sunrise to midday, then midday to sunset is different colours
 * another pixel to show segment2 effect paired with sun azimuth, where sunrise point to sunset point is full colour wheel
 * 
 * add solar value I can poll to get time of day (day/dusk/twilight/) and direction of sun movement (rising/falling)
 * 
 * 
 **/
#ifdef DEVICE_DESKSENSOR
  #define DEVICENAME_CTR          "desksensor"
  #define DEVICENAME_FRIENDLY_CTR "Desk Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  // #define analogRead(x) error

  // #define DISABLE_NETWORK




  // #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  // #define USE_MODULE_NETWORK_WEBSERVER // enable by default on esp32 going forward for pushing development
  // #define DISABLE_WEBSERVER  // phase out to only USE_MODULE_NETWORK_WEBSERVER

  // #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

//   {
//   "AnimationMode": "Effects",
//   "Effects": {
//     "Function": "Static"
//   },
//   "Transition": {
//     "TimeMs": 0,
//     "RateMs": 1000
//   },
//   "ColourPalette": 15,
//   "PaletteEdit": {
//     "ColourPalette": 15,
//     "Data": [
//       4,6,0,0,0,
//       255,      0,      0,
//       0,      255,      0,
//       0,      0,      255,
//       255,      255,      0
//     ]
//   },
//   "BrightnessRGB": 100,
//   "BrightnessCCT": 100
// }

// {
//   "AnimationMode": "Effects",
//   "Effects": {
//     "Function": "Solid"
//   },
//   "ColourPalette": 10,
//   "BrightnessRGB": 10,
//   "Hue": 120,
//   "Sat": 100,
//   "CCT_TempPercentage": 100,
//   "BrightnessCCT": 0,
//   "Transition": {
//     "TimeMs": 0,
//     "RateMs": 5000
//   },
//   "Light":{"TimeOn":10}
// }


// {
//   "Segment0":{
//   "PixelRange":[0,3],
//   "AnimationMode": "Effects",
//   "Effects": {
//     "Function": 2
//   },
//   "ColourPalette": 0,
//   "BrightnessRGB": 10,
//   "BrightnessCCT": 10,
//   "Hue":0,"Sat":100,
//   "Transition": {
//     "TimeMs": 0,
//     "RateMs": 1000
//   }
//   },
//   "Segment1":{
//   "PixelRange":[5,8],
//   "AnimationMode": "Effects",
//   "Effects": {
//     "Function": 2
//   },
//   "ColourPalette": 16,
//   "BrightnessRGB": 100,
//   "BrightnessCCT": 10,
//   "Hue":0,"Sat":100,
//   "Transition": {
//     "TimeMs": 0,
//     "RateMs": 10000
//   }
//   }
// }
// {
//   "AnimationMode": "Effects",
//   "Effects": {
//     "Function": 0
//   },
//   "ColourPalette": 10,
//   "BrightnessRGB": 10,
//   "BrightnessCCT": 10,
//   "Hue":0,"Sat":100,
//   "Transition": {
//     "TimeMs": 0,
//     "RateMs": 1000
//   }
// }
  // #define USE_MODULE_DRIVERS_LEDS

  // #define USE_MODULE_DRIVERS_IRREMOTE
  // #define USE_IR_RECEIVE

  #define USE_MODULE_DISPLAYS_NEXTION
  #define NEXTION_DEFAULT_PAGE_NUMBER 3   // I should add "p[c]" where c means current page, so I need to search and replace "p[c]" as "p[0]"

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_DS18X
  #define USE_MODULE_SENSORS_DHT
  #define USE_MODULE_SENSORS_BME
    #define ENABLE_DEVFEATURE_BME_DUAL_DEVICES
  #define USE_MODULE_SENSORS_BH1750
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_LDR_BASIC

  #ifdef USE_MODULE_SENSORS_LDR_BASIC
    #define USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
    #define USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  #define USE_MODULE_LIGHTS_INTERFACE
    #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE_DEBUG_CONFIG
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  #define STRIP_SIZE_MAX 13
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT
  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


// #define ENABLE_DEVFEATURE_LIGHTING_CANSHOW_TO_PINNED_CORE_ESP32
// https://github.com/drzony/FastLED-tests/blob/master/src/main.cpp


  //   #define ENABLE_DEVFEATURE_CAUTION__BLOCK_ANIMATION_LOOP_FOR_DIRECT_TESTING

  //   #define ENABLE_DEVFEATURE_DEBUG_FREEZING_SK6812

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__NOTIFICATIONS
  // #define ENABLE_FEATURE_PIXEL__MODE_NOTIFICATION

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"   
      /**
       * @brief Right side
       **/
      #ifdef USE_MODULE_SENSORS_DS18X   
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      #endif // USE_MODULE_SENSORS_DS18X
      // TX0 - Debug Serial TX
      // RX0 - Debug Serial RX
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
      "\"21\":\"" D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"19\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
      "\"18\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","      
      #endif



      // "\"19\":\"" D_GPIO_FUNCTION__ROTARY_ENCODER_A__CTR "\","
      // "\"18\":\"" D_GPIO_FUNCTION__ROTARY_ENCODER_B__CTR "\","
      

      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","   // Use the 12LED RGBW ring to permit led testing, and eventually, notification mode
      #endif 
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
      "\"15\":\"" D_GPIO_FUNCTION_IR_RECV_CTR "\","
      #endif
      /**
       * @brief Left side
       **/
      // EN
      // VP
      // VN
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
      "\"34\":\"" D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6
      #endif
      // 35 - adc1_7
      // 32 - Touch9
      // 33 - Touch8
      // 25 - DAC1 = LM386 Amplifier Module
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""
      #endif
      // 14
      // 12
      // 13
      // Can I introduce a way that a comma at the end, does not make a broken json?
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "BedroomDesk"
  #define D_DEVICE_SENSOR_CLIMATE_BME "BedroomDesk-BME"
  #define D_DEVICE_SENSOR_CLIMATE_DHT1 "BedroomDesk-DHT1"
  #define D_DEVICE_SENSOR_CLIMATE_DHT2 "BedroomDesk-DHT2" 
  

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
  #define D_DEVICE_SENSOR_DB18S20_5_NAME        "DB_06"
  #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,100,29,195,135,215,193]"
  

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
        "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_BME "\""
      "],"  
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_DHT1 "\","
        "\"" D_DEVICE_SENSOR_CLIMATE_DHT2 "\""
      "],"  
      "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"  
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
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
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1}"
  "}";

  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"
    // "\"AddRuleDefault\":\"Motion0=0\","
    // MOTION
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
    "}"
  "}";

  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    // "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBW\","    
    // "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\"," 
    // "\"ColourPalette\":15," 
    // "\"PaletteEdit\": {"
    //   "\"ColourPalette\": 15,"
    //   "\"Data\": ["
    //     "4,5,0,0,0,"
    //     "255, 0, 0,"
    //     "0, 2550, 0,"
    //     "0, 0, 255,"
    //     "255, 0, 255"
    //   "]"
    // "},"
    // "\"Effects\":{"
    //   "\"Function\":\"Static\""
    // "},"
    // "\"Transition\":{"
    //   "\"TimeMs\":900,"
    //   "\"RateMs\":1000"
    // "},"    
    // "\"BrightnessRGB\":1"

    // "\"AnimationMode\": \"Effects\","
    // "\"ColourOrder\": \"grbw\","
    // "\"ColourPalette\": 10,"
    // "\"Effects\": {"
    //   "\"Function\": \"Solid\""
    // "},"
    // "\"Hue\": 15,"
    // "\"Sat\": 100,"
    // "\"BrightnessRGB\": 100,"
    // "\"BrightnessCCT\": 0,"
    // "\"CCT_TempPercentage\": 100,"
    // "\"Transition\": {"
    //   "\"Time\": 1"
    // "}"

    // "\"PixelRange\":[0,6],"
    "\"AnimationMode\": \"Effects\","
    "\"ColourOrder\": \"grbw\","
    "\"ColourPalette\": 0,"
    "\"Effects\": {"
      "\"Function\": \"Slow Glow\""
    "},"
    "\"Hue\": 120,"
    "\"Sat\": 100,"
    "\"BrightnessRGB\": 1,"
    "\"BrightnessCCT\": 0,"
    "\"CCT_TempPercentage\": 100,"
    "\"Transition\": {"
      "\"TimeMs\": 9000,"
      "\"RateMs\": 10000"
    "}"
    // "\"Light\":{\"TimeOn\":10}"

  "}";
  #endif // USE_MODULE_LIGHTS_INTERFACE



#endif



// #ifdef DEVICE_BEDROOM_WALLCLOCK_01
//   #define DEVICENAME_CTR            "bedroom_wallclock_01"
//   #define DEVICENAME_FRIENDLY_CTR   "RGB Clock 0l"

//   //#define FORCE_TEMPLATE_LOADING
//   #define SETTINGS_HOLDER 1

//   /**
//    * @brief 
//    * Add list of optional animations on the time
//    *  - on the top of the hour, display animation (idea, flash white (all on), then saturate back into the new time)
//    * 
//    */


   
//   // #define USE_BUILD_TYPE_LIGHTING
//   // #define USE_MODULE_LIGHTS_INTERFACE
//   // #define USE_MODULE_LIGHTS_ANIMATOR
//   // #define USE_MODULE_LIGHTS_ADDRESSABLE
//   #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
//   #define USE_SK6812_METHOD_DEFAULT

//   #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK



//   #define STRIP_SIZE_MAX 93
  
//   #define DISABLE_WEBSERVER

//   // #define USE_MODULE_TEMPLATE
//   // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//   //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//   //   "\"" D_JSON_GPIOC "\":{"
//   //     "\"LBI\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
//   //   "},"
//   //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   // "}";

//   // #define USE_LIGHTING_TEMPLATE
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
//   //   #ifdef STRIP_SIZE_MAX
//   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //   #else
//   //   "\"" D_JSON_STRIP_SIZE       "\":50,"
//   //   #endif //STRIP_SIZE_MAX
//   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
//   //   "\"" D_JSON_TRANSITION       "\":{"
//   //     "\"" D_JSON_TIME_MS "\":0,"
//   //     "\"" D_JSON_RATE_MS "\":1000,"
//   //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
//   //     "\"" D_JSON_ORDER "\":\"" D_JSON_INORDER "\""
//   //   "},"
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
//   //   "\"" D_JSON_EFFECTS "\":{" 
//   //     "\"" D_JSON_FUNCTION "\":29"
//   //   "},"
//   //   // "\"CCT_Temp\": 152,"
//   //   // "\"Hue\":25,"
//   //   // "\"Sat\":100,"
//   //   "\"" D_JSON_COLOUR_PALETTE "\":41,"
//   //   "\"BrightnessCCT\":10,"
//   //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
//   // "}";
  
//   // #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Landing"
//   // #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Landing"
  
//   // #define USE_FUNCTION_TEMPLATE
//   // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   // "{"
//   //   "\"" D_JSON_DEVICENAME "\":{"
//   //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
//   //       "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
//   //     "],"
//   //     "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
//   //       "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
//   //     "]"
//   //   "}"
//   // "}";

// /**
//  * @brief New format below
//  * 
//  */


  
//   // #define STRIP_SIZE_MAX 50                                                                           // Change: Set *total* length of string, segment0 will default to this length
//   #define PIN_NAME_STRING_ESP8266_DEFAULT   "RX"                                                      // Change: Set to the pin you want, esp8266 this will default to this anyway
//   #define PIN_NAME_STRING_ESP32_DEFAULT     "23"                                                      //         Set to the pin you want, any output pin should work

//   /**
//    * @brief Uncomment one line to use testing template configs for lighting_template
//    * 
//    */
//   // #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE                                         // Change: You can pick one as examples
//   #define LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
   
//   /**
//    * @brief Mostly for me testing, switching between my segments or testing orginal wled effects
//    **/
//   #define USE_BUILD_TYPE_LIGHTING
//   #define USE_MODULE_LIGHTS_INTERFACE
//   #define USE_MODULE_LIGHTS_ANIMATOR
//   #define USE_MODULE_LIGHTS_ADDRESSABLE
//   #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
//   #define D_EFFECT_INSIDE_TEMPLATE "Effects"
//   /**
//    * @brief defines to be tested and incorporated fully
//    **/
  
  
  
//   // #define ENABLE_DEVFEATURE_MULTIPLE_NEOPIXELBUS_OUTPUTS
//   // #define ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
//   // #define ENABLE_DEVFEATURE_WS2812FX_DEFAULT_PALETTE_EFFECTS
//   // #define ENABLE_DEVFEATURE_GET_COLOUR_PALETTE_JOINT_METHOD
//   // #define ENABLE_DEVFEATURE_PALETTE_ADVANCED_METHODS_GEN2 // ie the new way of merging fastled to mine
//   /**
//    * @brief Debug flags, used mostly be me
//    * 
//    */  
//   // #define ENABLE_FREERAM_APPENDING_SERIAL
//   // #define DEBUG_WLED_EFFECT_FUNCTIONS
//   // #define ENABLE_DEVFEATURE_LEARNING_FASTLED_PALETTES


//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIOC "\":{"
//     // #ifdef ESP8266 // default pins for ws28xx
//     //   "\"" PIN_NAME_STRING_ESP8266_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
//     // #else
//     //   "\"" PIN_NAME_STRING_ESP32_DEFAULT "\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
//     // #endif
    
//       "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""

//     "},"
//     "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
//   "}";

// /**
//  * @brief 
//  * LIGHTING_TEMPLATE
//  * 1) Without "Segment#:{}" used, it is assumed the command should be applied to the entire strip, defaulting entire strip as segment number 0
//  * 2) Multiple segments can be set, be using the same commands but under multiple json keys called segment with its number (currently maximum of 5 segments)
//  *    eg
//  *        {
//  *        "Segment0":{
//  *                      "ColourPalette":"Christmas 01" 
//  *                   },
//  *        "Segment1":{
//  *                      "ColourPalette":"Christmas 02" 
//  *                   },
//  *        "Segment2":{
//  *                      "ColourPalette":"Christmas 03" 
//  *                   }
//  *         }
//  * 
//  */

// /**
//  * @brief The following templates are tested examples
//  * 
//  */

//   #define USE_LIGHTING_TEMPLATE

//   // #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SHIMMERING_PALETTE
//   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   "{"
//     // "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
//     // "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//     // "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBW\","    


//     // "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
//     // "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
//     // "\"Effects\":{"
//     //   "\"Function\":\"Slow Glow\""
//     // "},"
//     // "\"Transition\":{"
//     //   "\"TimeMs\":0,"
//     //   "\"RateMs\":1000"
//     // "},"    
//     // "\"BrightnessRGB\":100"

//     "\"HardwareType\": \"SK6812\","
//     "\"LightSize\": 93,"
//     "\"ColourOrder\": \"GRBW\","
//     "\"AnimationMode\": \"Effects\","
//     "\"ColourPalette\": 1,"
//     "\"Effects\": {"
//       "\"Function\": 28"
//     "},"
//     "\"Transition\": {"
//       "\"TimeMs\": 500,"
//       "\"RateMs\": 1000"
//     "},"
//     "\"BrightnessRGB\": 100"

//   "}";




//   // #endif

//   // #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_SLOW_GLOW
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
//   //   "\"ColourPalette\":\"Coloured MultiColoured Warmer\"," 
//   //   "\"Effects\":{"
//   //     "\"Function\":\"" D_EFFECTS_FUNCTION__SLOW_GLOW__NAME_CTR "\""
//   //   "},"
//   //   "\"Transition\":{"
//   //     "\"TimeMs\":5000,"
//   //     "\"RateMs\":20000"
//   //   "},"    
//   //   "\"BrightnessRGB\":100"
//   // "}";
//   // #endif

//   // #ifdef LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
//   //   "\"ColourPalette\":\"Christmas 21\"," 
//   //   "\"Effects\":{"
//   //     "\"Function\":30"
//   //   "},"
//   //   "\"Transition\":{"
//   //     "\"TimeMs\":0,"
//   //     "\"RateMs\":30"
//   //   "},"    
//   //   "\"BrightnessRGB\":100"
//   // "}";
//   // #endif // LIGHTING_TEMPLATE_ADDING_WLED_FIREWORKS

//   // #ifdef LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"grb\","    
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
//   //   "\"ColourPalette\":\"Christmas 06\"," 
//   //   "\"Effects\":{"
//   //     "\"Function\":1"
//   //   "},"
//   //   "\"Transition\":{"
//   //     "\"TimeMs\":0,"
//   //     "\"RateMs\":30"
//   //   "},"    
//   //   "\"BrightnessRGB\":100"
//   // "}";
//   // #endif // LIGHTING_TEMPLATE_SINGLE_SEGMENT_CANDLE_CHRISTMAS

//   // #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"Segment0\":{"
//   //     "\"PixelRange\":[0,19],"
//   //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//   //     "\"Effects\":{"
//   //       "\"Function\":\"Static\""
//   //     "},"
//   //     "\"ColourPalette\":\"Christmas 09\","
//   //     "\"Transition\":{"
//   //       "\"TimeMs\":3000,"
//   //       "\"RateMs\":10000"
//   //     "},"    
//   //     "\"BrightnessRGB\":100"
//   //   "},"
//   //   "\"Segment1\":{"
//   //     "\"PixelRange\":[20,29],"
//   //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
//   //     "\"Effects\":{"
//   //       "\"Function\":\"Solid RGBCCT\""
//   //     "},"
//   //     "\"ColourPalette\":\"Solid Rgbcct 01\","
//   //     "\"Transition\":{"
//   //       "\"TimeMs\":500,"
//   //       "\"RateMs\":1000"
//   //     "},"    
//   //     "\"BrightnessRGB\":100"
//   //   "},"
//   //   "\"Segment2\":{"
//   //     "\"PixelRange\":[30,49],"
//   //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//   //     "\"Effects\":{"
//   //       "\"Function\":\"Slow Glow\""
//   //     "},"
//   //     "\"ColourPalette\":\"Christmas 01\","
//   //     "\"Transition\":{"
//   //       "\"TimeMs\":500,"
//   //       "\"RateMs\":1000"
//   //     "},"    
//   //     "\"BrightnessRGB\":100"
//   //   "},"
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
//   //   "\"BrightnessRGB\":100"
//   // "}";
//   // #endif
  
//   // #ifdef LIGHTING_TEMPLATE_MULTIPLE_SEGMENTS_FOR_UTILITY
//   // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
//   // "{"
//   //   "\"Segment0\":{"
//   //     "\"PixelRange\":[0,19],"
//   //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//   //     "\"Effects\":{"
//   //       "\"Function\":29"
//   //     "},"
//   //     "\"ColourPalette\":\"Christmas 09\","
//   //     "\"Transition\":{"
//   //       "\"TimeMs\":0,"
//   //       "\"RateMs\":23"
//   //     "},"    
//   //     "\"BrightnessRGB\":100"
//   //   "},"
//   //   "\"Segment1\":{"
//   //     "\"PixelRange\":[20,29],"
//   //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","   
//   //     "\"Effects\":{"
//   //       "\"Function\":28"
//   //     "},"
//   //     "\"ColourPalette\":11,"
//   //     "\"Hue\":20,"
//   //     "\"Sat\":90,"
//   //     "\"Transition\":{"
//   //       "\"TimeMs\":0,"
//   //       "\"RateMs\":25"
//   //     "},"    
//   //     "\"BrightnessRGB\":100"
//   //   "},"
//   //   "\"Segment2\":{"
//   //     "\"PixelRange\":[30,49],"
//   //     "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
//   //     "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
//   //     "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
//   //     "\"Effects\":{"
//   //       "\"Function\":\"Slow Glow\""
//   //     "},"
//   //     "\"ColourPalette\":\"Christmas 01\","
//   //     "\"Transition\":{"
//   //       "\"TimeMs\":500,"
//   //       "\"RateMs\":1000"
//   //     "},"    
//   //     "\"BrightnessRGB\":100"
//   //   "},"
//   //   "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
//   //   "\"BrightnessRGB\":100"
//   // "}";
//   // #endif




// #endif



#ifdef DEVICE_H801_INSIDE_BEDROOM_WARDROBE
  #define DEVICENAME_CTR          "h801_bedroom_wardrobe"
  #define DEVICENAME_FRIENDLY_CTR "H801 h801_bedroom_wardrobe 3"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  
  // #define USE_SERIAL_ALTERNATE_TX
  // #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  // //#define FORCE_TEMPLATE_LOADING
  // // #define SETTINGS_HOLDER 2 

  #define USE_MODULE_SENSORS_BUTTONS

  // #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR

  // #define DEBUG_TARGET_ANIMATOR_SEGMENTS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_PWM
  
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  // #define D_EFFECT_INSIDE_TEMPLATE "Effects"

  // #define MAX_NUM_SEGMENTS 5
  
  
  // #define 
  
  
  


  // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  
  #define USE_MODULE_CORE_RULES

  // #define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  
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


  // //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1   

  
  // // #define USE_MODULE_CORE_RULES
  
  // // #define USE_MODULE_SENSORS_INTERFACE
  // // #define USE_MODULE_SENSORS_BUTTONS

  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_PWM


  // #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT

  // // need to add motion here

  // // // #define USE_DEVFEATURE_SUNPOSITION_ELEVATION_USE_TESTING_VALUE

  // //#define USE_MODULE_SUBSYSTEM_SOLAR_LUNAR

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //    "\"" D_JSON_GPIOC "\":{"
  //     #ifdef USE_MODULE_SENSORS_BUTTONS
  //     "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
  //     #endif    
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\""
  // "}";
  
  // #define STRIP_SIZE_MAX 1 // PWM type, set size to 1
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBwc\","
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  D_JSON_EFFECTS  "\","
  //   "\"" D_JSON_EFFECTS "\":{" 
  //     // "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
  //     "\"" D_JSON_FUNCTION "\":8"//\"Sun Elevation RGBCCT Solid Palette 01\""
  //   "},"
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME "\":1,"
  //     "\"" D_JSON_RATE "\":5,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"" D_JSON_CCT_TEMP "\":300,"
  //   "\"" D_JSON_HUE "\":25,"
  //   "\"" D_JSON_SAT "\":100,"
  //   "\"" D_JSON_COLOUR_PALETTE "\":67,"
  //   "\"" D_JSON_BRIGHTNESS_CCT "\":100,"
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":100"
  // "}";


  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"
  //   "\"Rule0\":{" //switch example
  //     "\"Trigger\":{"
  //       "\"Module\":\"Buttons\","    //sensor
  //       "\"Function\":\"" D_FUNC_EVENT_INPUT_STATE_CHANGED_CTR "\"," //eg. InputChange (TemperatureThreshold)
  //       "\"DeviceName\":0," // eg Switch0, Switch1, Button#, Motion, # (number for index)  
  //       "\"State\":2" //eg. On, Off, Toggle, Any, LongPress, ShortPress, RisingEdge, FallingEdge, Started, Ended, TimerOnStarted
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"Relays\","
  //       "\"Function\":\"SetPower\"," //eg. InputChange (TemperatureThreshold)
  //       "\"DeviceName\":0," //number, name, or all
  //       "\"State\":2" // toggle
  //     "}"
  //   "}"
  // "}";


    
#endif

#ifdef DEVICE_BLACK_STAND_LIGHT
  #define DEVICENAME_CTR          "stand_light"
  #define DEVICENAME_FRIENDLY_CTR "Stand Light"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
    
  #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 800

  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
  #define D_EFFECT_INSIDE_TEMPLATE "Effects"
  #define STRIP_SIZE_MAX 77                                                                           // Change: Set *total* length of string, segment0 will default to this length
   
  
  #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  #define USE_SK6812_METHOD_DEFAULT

  
  // #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    // #define ENABLE_DEVFEATURE_TRANSITION_MODE_LEVEL_OPTIONS
    // #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_OLD_FUNCTIONS
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
    


  // Hard coded to alternate pin until I make this template based
  // #define ENABLE_DEVFEATURE_SET_ESP32_RGB_DATAPIN_BY_TEMPLATE
  // #define PINSET_TEMP_METHOD_RGB_PIN_RGB 4
  
  // #define LIGHTING_TEMPLATE_DEFAULT_STAND_RAINBOW
  // #define LIGHTING_TEMPLATE_DEFAULT_STAND_WARM_WHITE
  #define LIGHTING_TEMPLATE_DEFAULT_STAND_CHRISTMAS_GROUPED_STATIC

  #ifdef LIGHTING_TEMPLATE_DEFAULT_STAND_WARM_WHITE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":10," 
    "\"Hue\":25," 
    "\"Sat\":70," 
    "\"Effects\":{"
      "\"Function\":0"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":10"
  "}";
  #endif // LIGHTING_TEMPLATE_DEFAULT_STAND_WARM_WHITE
  #ifdef LIGHTING_TEMPLATE_DEFAULT_STAND_RAINBOW
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Rainbow\"," //rainbow gradient 1
    "\"Hue\":25," 
    "\"Sat\":70," 
    "\"Effects\":{"
      "\"Function\":\"Static\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":10"
  "}";
  #endif // LIGHTING_TEMPLATE_DEFAULT_STAND_RAINBOW
  #ifdef LIGHTING_TEMPLATE_DEFAULT_STAND_ORANGE_WHITE_BLUE
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Orange White Blue\"," 
    "\"Effects\":{"
      "\"Function\":\"Static\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";
  #endif // LIGHTING_TEMPLATE_DEFAULT_STAND_ORANGE_WHITE_BLUE

  #ifdef LIGHTING_TEMPLATE_DEFAULT_STAND_CHRISTMAS_GROUPED_STATIC
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_EFFECT_INSIDE_TEMPLATE "\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Effects\":{"
      "\"Function\":\"Static\","
      "\"Grouping\":7"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100,"
    "\"BrightnessCCT\":0"
  "}";
  #endif // LIGHTING_TEMPLATE_DEFAULT_STAND_ORANGE_WHITE_BLUE



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif



#ifdef DEVICE_SHELLYDIMMER_BEDROOM_LAMP
  #define DEVICENAME_CTR          "dimmer_bedroom_lamp"
  #define DEVICENAME_FRIENDLY_CTR "Shelly Dimmer Spare Room"
  
  //#define FORCE_TEMPLATE_LOADING
  #define SETTINGS_HOLDER 2

  // #define ENABLE_DEVFEATURE_SHELLYDIMMER2_INVERTED_EDGE_FOR_ERROR
  
  #define USE_MODULE_SENSORS_SWITCHES

  #define USE_MODULE_CORE_RULES
  #define USE_HARDWARE_DEFAULT_RULES_1

  #define DISABLE_SERIAL_LOGGING //temp measure
  // #define DISABLE_SERIAL0_CORE //dont think its needed

  #define USE_MODULE_DRIVERS_SHELLY_DIMMER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_SHELLY_DIMMER2_CTR "\""
  "}";

  #define D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "Light"
  #define D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "Switch1"
  #define D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "Switch2"
  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_OUTPUT1_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SWITCH1_FRIENDLY_NAME_LONG "\","
        "\"" D_DEVICE_SWITCH2_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

#endif




#ifdef DEVICE_BEDROOM_BEDLIGHT
  #define DEVICENAME_CTR          "bedroom_bedlight"
  #define DEVICENAME_FRIENDLY_CTR "Bedroom Bedlight H801"
  #define DEVICENAME_ROOMHINT_CTR "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
      
  #define USE_SERIAL_ALTERNATE_TX
  #define ENABLE_PIXEL_LIGHTING_GAMMA_CORRECTION

  #define DISABLE_WEBSERVER
    
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_PWM  
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define ENABLE_DEVFEATURE_SOLAR_PALETTES
    #define ENABLE_DEVFEATURE_CHECK_SEGMENT_INIT_ERROR
    #define DEBUG_TARGET_ANIMATOR_SEGMENTS
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT  

    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"1\":\""  D_GPIO_FUNCTION_LED1_CTR "\","
      "\"5\":\""  D_GPIO_FUNCTION_LED2_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_H801_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define STRIP_SIZE_MAX 1
  #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE  "\":\"" "RGBCCT_PWM" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":1,"
  //   "\"" D_JSON_RGB_COLOUR_ORDER   "\":\"RGBCW\","
  //   "\"" D_JSON_TRANSITION     "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"Solid Rgbcct 00\","
  //   "\"" D_JSON_ANIMATIONMODE  "\":\"Effects\","
  //   "\"" D_JSON_EFFECTS        "\"{\"Function\":\"Solid RGBCCT\"},"
  //   "\"" D_JSON_BRIGHTNESS     "\":100"
  // "}";

  
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "RGBCCT_PWM" "\","
    "\"" D_JSON_STRIP_SIZE       "\":1,"
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"RGBCW\","
    "\"" D_JSON_TRANSITION       "\":{\"" D_JSON_TIME "\":2,\"" D_JSON_RATE "\":20},"
    "\"" D_JSON_COLOUR_PALETTE   "\":\"Solid Rgbcct 00\","
    "\"Hue\":345,\"Sat\":100,"
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\","
    "\"" D_JSON_EFFECTS          "\":{\"Function\":\"Solid RGBCCT\"},"
    "\"" D_JSON_BRIGHTNESS_RGB   "\":100,"
    "\"" D_JSON_BRIGHTNESS_CCT   "\":100"
  "}";



#endif


/**
 * Original screen, to be renamed
 * Move 5V PSU into spare room, therefore it will remained powered going forward
 * Same for under desk
 * */
#ifdef DEVICE_RGB_COMPUTER_SCREEN_DELL_U2515H
  #define DEVICENAME_CTR            "rgb_computer_display_u25"//"rgbdell"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Dell"
  #define DEVICENAME_ROOMHINT_CTR   "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  

  //#define FORCE_TEMPLATE_LOADING
  // #define SETTINGS_HOLDER 1

  #define USE_DEVFEATURE_PIXEL0_BOTTOM_RIGHT // tmp fix
   
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE

  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 133
    
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS
  

  //#define ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  //ADALIGHT_ViaSerialAndWifi
  //SCREENEDGES


  #define STRIP_SIZE_MAX 133

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  "Ambilight"  "\","
  //    "\"" D_JSON_EFFECTS "\":{" 
  //     "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
  //   "},"
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":1000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"" D_JSON_CCT_TEMP "\":300,"
  //   "\"" D_JSON_HUE "\":15,"
  //   "\"" D_JSON_SAT "\":90,"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
  //   "\"" D_JSON_BRIGHTNESS_CCT "\":1,"
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":0"
  // "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\"," 
    "\"ColourPalette\":\"Christmas 06\"," 
    "\"Hue\":25," 
    "\"Sat\":70," 
    "\"Effects\":{"
      "\"Function\":\"Static Palette\","
      "\"Grouping\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";


#endif

/*
 * Move 5V PSU into spare room, therefore it will remained powered going forward
 * Same for under desk
*/

#ifdef DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE
  #define DEVICENAME_CTR            "rgb_computer_display_p32"
  #define DEVICENAME_FRIENDLY_CTR   "RGB Dell 32"
  #define DEVICENAME_ROOMHINT_CTR   "Temporary_Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  // #define USE_BUILD_TYPE_LIGHTING
  // #define USE_MODULE_LIGHTS_INTERFACE
  // #define USE_MODULE_LIGHTS_ANIMATOR
  // #define USE_MODULE_LIGHTS_ADDRESSABLE
  // #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
  // #define USE_SK6812_METHOD_DEFAULT
  

  // #define ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT

  #define USE_SCREEN_RGB_RESOLUTION_P32_TEMP_FIX // set with commands later
  #define USE_DEVFEATURE_PIXEL0_BOTTOM_LEFT_ANTICLOCKWISE_TO_BE_FEATURE_OPTION
 
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    // #define STRIP_SIZE_MAX 55
    #define USE_WS28XX_FEATURE_4_PIXEL_TYPE
    #define USE_SK6812_METHOD_DEFAULT
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"
  //     "\"RX\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\""
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  // "}";

  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  //ADALIGHT_ViaSerialAndWifi
  //SCREENEDGES

  #define STRIP_SIZE_MAX 133

  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","
  //   "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
  //   "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","
  //   "\"" D_JSON_ANIMATIONMODE    "\":\""  "Ambilight"  "\","
  //   "\"" D_JSON_EFFECTS "\":{"
  //     "\"" D_JSON_FUNCTION "\":\"" D_EFFECTS_FUNCTION_SOLID_COLOUR_NAME_CTR "\""
  //   "},"
  //   "\"" D_JSON_TRANSITION       "\":{"
  //     "\"" D_JSON_TIME_MS "\":1000,"
  //     "\"" D_JSON_RATE_MS "\":1000,"
  //     "\"" D_JSON_PIXELS_UPDATE_PERCENTAGE "\":2,"
  //     "\"" D_JSON_ORDER "\":\"" D_JSON_RANDOM "\""
  //   "},"
  //   "\"" D_JSON_CCT_PERCENTAGE "\":100,"
  //   "\"" D_JSON_HUE "\":15,"
  //   "\"" D_JSON_SAT "\":90,"
  //   "\"" D_JSON_COLOUR_PALETTE "\":\"RGBCCTColour 00\","
  //   "\"" D_JSON_BRIGHTNESS_CCT "\":40,"
  //   "\"" D_JSON_BRIGHTNESS_RGB "\":0"
  // "}";


  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "SK6812" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRBw\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"Effects\"," 
    "\"ColourPalette\":10," 
    "\"Hue\":20," 
    "\"Sat\":90," 
    "\"Effects\":{"
      "\"Function\":\"Solid\""
    "},"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"" D_JSON_CCT_PERCENTAGE "\":100,"
    "\"" D_JSON_BRIGHTNESS_CCT "\":40,"
    "\"BrightnessRGB\":0"
  "}";

#endif


#ifdef DEVICE_RGBDISPLAY_BEDROOM_OUTSIDE_TEMPERATURE
  #define DEVICENAME_CTR          "rgbdisplay_bedroom_outsidetemp"
  #define DEVICENAME_FRIENDLY_CTR "7-Segment White Display"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70

  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
  #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS // Not ready to remove
  #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT // Towards making bus dynamic and multiple pins
  #define STRIP_SIZE_MAX 93  
  
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  #define USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_COLOUR_TYPE "\":3,"//\"RGBW\","   //3=rgb, 4=rgbw
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_EFFECTS "\"," 
    "\"ColourPalette\":10," 
    "\"Effects\":{"
      "\"Function\":\"Clock Basic 01\""
    "},"
    "\"Hue\":10,\"Sat\":100,"
    "\"Transition\":{"
      "\"TimeMs\":900,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":1"
  "}";



#endif




#ifdef DEVICE_RGB_GLASSHEART
  #define DEVICENAME_CTR          "rgb_glassheart"
  #define DEVICENAME_FRIENDLY_CTR "RGB GLass Heart"
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   192,168,1,70
  
  #define USE_BUILD_TYPE_LIGHTING
  #define USE_MODULE_LIGHTS_INTERFACE
  #define USE_MODULE_LIGHTS_ANIMATOR
  #define USE_MODULE_LIGHTS_ADDRESSABLE
    #define ENABLE_PIXEL_FUNCTION_SEGMENTS_ANIMATION_EFFECTS
    #define STRIP_SIZE_MAX 25
    #define ENABLE_DEVFEATURE_NEOPIXELBUS_INTO_SEGMENTS_STRUCT
    
    #define ENABLE_DEVFEATURE_FIXING_SEGMENT_LENGTH_SIZE
    #define ENABLE_DEVFEATURE_ENABLE_INTENSITY_TO_REPLACE_PERCENTAGE_CHANGE_ON_RANDOMS
    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
    #define ENABLE_DEVFEATURE_INCREMENTING_PALETTE_ID
    #define ENABLE_DEVFEATURE_PALETTE_INTERMEDIATE_FUNCTION__USE_NEW_FUNCTIONS


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"RX\":\""  D_GPIO_FUNCTION_RGB_DATA_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  "{"
    "\"" D_JSON_HARDWARE_TYPE    "\":\"" "WS28XX" "\","                //should be default
    "\"" D_JSON_STRIP_SIZE       "\":" STR2(STRIP_SIZE_MAX) ","
    "\"" D_JSON_RGB_COLOUR_ORDER "\":\"GRB\","    
    "\"" D_JSON_ANIMATIONMODE    "\":\"" D_JSON_EFFECTS "\"," 
    "\"ColourPalette\":\"Pink purple 01\"," 
    "\"Effects\":{"
      "\"Function\":\"Static Palette\","
      "\"Intensity\":127,"
      "\"Speed\":10"
    "},"
    "\"Transition\":{"
      "\"TimeMs\":1000,"
      "\"RateMs\":1000"
    "},"    
    "\"BrightnessRGB\":100"
  "}";

#endif

#endif // MFIRMWARECUSTOM_SECRET_HOME_LONGTERM_H
