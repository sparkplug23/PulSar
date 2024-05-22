#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H


/*********************************************************************************************\
 * Devices I am currently working on, kept as its own file to make it easier
 * 
 * 
 * Restructing logging levels
 * INF - a module should only give one per minute (ie general health of module)
 * DBG - detailed logging, sensor readings each time they are read
 * 
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

// #define DEVICE_HVAC_BEDROOM_4CHANNEL_WITH_ENERGY_SENSORS
// #define DEVICE_TREADMILL_POWER_MONITOR
// #define DEVICE_LIGHTING__LED_MATRIX_BOX_01
// #define DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__10INCH
// #define DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH
// #define DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_01
// #define DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_02
// #define DEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
// #define DEVICE_TESTBED__LED_MATRIX
// #define DEVICE_CAMERA_XIAO_TESTBED
#define DEVICE_DESKSENSOR // tester with ring led/

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** LOCATION: Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/



/*
 * ** Main board **************************************************************************


Current plan, summary for myself:

Main board

esp32
multiple ds18b20 soldered in place onto 1 pin
at least one ds18b20 on a wire (eg. to attach to oil radiator), on another pin
buzzer/peizo speaker (new code)
stuck in place, one strip of 4 sk6812 (to show when relays are running)
with wire, a connector for a strip that will represent heatmap like my tank, for developing code. It will use those ds18b20 on board and show their temp as colour gradient.
strip of maybe 60 leds, powered via INA219 power meter
BME280 (hum, temp, pressure)
BME680 (hum, temp, pressure and gas) [to test 280 and 680 side by side]
DHT22 (maybe, just to clean that code up)
OLED display showing relay time on, temp and energy

via 4pin header connection

Nextion 2.5 inch, mostly to dev making one of them control the relays with schedules (ie to further dev my hvac heating)

via RJ45

The 3D printed board
now printing again, to include a 5V 4A PSU
May need to add two power connections too, so its not just the cat5e wire to let me drive the LEDs full power
4 30A relays
4 PZEM





  ESP32
  5v Jack input (screw terminals that can accept jack)
  RJ45 Power
  RJ45 Sensor
  4P Nextion
  dual pixel strip (relay state and colourbar)

 * ** Power board **************************************************************************
  - To keep power cleaner, make a daugter board that holds all relays and PZEM then use RJ45 to connect back to main board

  - create small PCB to breakout the RJ45 and distribute power to relays and PZEM. This board should also hold the diodes for PZEM. Screw this board to a 3d print bed.

  3D print should actually be mounted into an enclosed box, use glands to secure mains wires. Box could also include 5V PSU and backfeed the 5V. A jumper should be added on the main board to isolate this backfeed for USB.

  Relay1: Oil heater
  Relay2: Fan heater
  Relay3: Dryer
  Relay4: Foot heater (with "power_cycle" that includes a new opiton to turn off/on again the source. Say floor is set to say on for 5 hours, it should recycle every 45 minutes within that 5 hours)

 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           REL2           21*           Immersion relay (5v IO)
 * bl/w          REL3            12*           DHT22?     
 * w/bl          REL4            13*           UNUSED   
 * g/w           REL1            3V3           
 * w/br          SER2_TX            
 * br/w          SER2_RX         14*           ds18b20 water, 4k7 pulled high        (comes from blue by connector)   NOT WORKING, wiring too long, Needs rerouted
 * 
 * 
 * 
 * ** Sensor board **************************************************************************
 * 
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           1Wire          4*            DS18B20 water pipe sensors
 * bl/w          I2D            22*           BME
 * w/bl          I2C            23*           BME
 * g/w           3V3            3V3           White
 * w/br                                       NC
 * br/w          DHT22          25*                  
 * 
 * 
 * 
 * The usual devices, connected back via RJ45 as a "all in one tester".
 * 
 * 1 BME280
 * 1 BME680
 * X DS18B20 (across 2 pins)
 * NO DHT, bedroom desk can test that
 * 
 * 
 * ** Nextion board **************************************************************************
 * 
 * Just use the already designed 3D desk print for the RJ45, then use a 4pin header back (the screw ones used for printer) to give me 2m nextion panel
 * 
 * 
 * 
 * 3D print design with screw like holders inside plugs
 * 
 * 
 **/
/**
 * 
 * 
 * Ethernet R
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    left LED (upstairs)              D27
 * bl/w   centre Relay (downstairs)        D5
 * w/bl   centre LED (downstairs)          D33
 * g/w    left Relay (upstairs)            D18
 * w/br   right LED (boiler)               D26
 * br/w   right Relay (boiler)             D19
 * 
 * Ethernet N
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    SK6812 D4
 * bl/w   NC
 * w/bl   NC
 * g/w    NC
 * w/br   RX2 of esp32 from nextion
 * br/w   TX2 of esp32 from nextion    
 * 

*/
#ifdef DEVICE_HVAC_BEDROOM_4CHANNEL_WITH_ENERGY_SENSORS
  #define DEVICENAME_CTR          "hvac_desk"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // Auto as temporary IP
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE 
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__LIGHTING__DUAL_OUTPUT
  #define ENABLE_TEMPLATE_SECTION__ENERGY
  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  #define ENABLE_TEMPLATE_SECTION__ENERGY__INA219

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define USE_MODULE_SENSORS_SOLAR_LUNAR

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
      #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      #define ENABLE_DEVFEATURE_BME680
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SOLAR_LUNAR     
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
  #define USE_MODULE_DISPLAYS_NEXTION
    #ifdef USE_MODULE_DISPLAYS_NEXTION
      #define NEXTION_DEFAULT_PAGE_NUMBER 6  
        #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
        // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
        #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
        #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
        // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS                    
        #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
        #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
        #define USE_MODULE_NETWORK_WEBSERVER
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
          #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC1  "openhab_broadcast/nextion/group/hvac_home"
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC2  "openhab_broadcast/nextion/group/hvac_desk_power"
      
        DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
        R"=====(
        {
          "PageNames": [
            "boot_flash",
            "boot",
            "message",
            "multiline",
            "logger",
            "heating",
            "hvacdesk",
            "main"
          ],
          "DefaultPageName":"hvacdesk",
          "DefaultBaud":115200,
          "TargetBaud":115200,
          "ObjectNameID": {
            "hdIconHAI": 2,
            "hdTimeHAI": 6,
            "hdBoostHAI": 11,
            "hdAutoHAI": 16,
            "powHAI": 21,
            "hdIconFLR": 3,
            "hdTimeFLR": 7,
            "hdBoostFLR": 12,
            "hdAutoFLR": 17,
            "powFLR": 22,
            "hdIconFAN": 4,
            "hdTimeFAN": 8,
            "hdBoostFAN": 13,
            "hdAutoFAN": 18,
            "powFAN": 23,
            "hdIconOIL": 5,
            "hdTimeOIL": 9,
            "hdBoostOIL": 14,
            "hdAutoOIL": 19,
            "powOIL": 24
          }
        }
        )=====";

    #endif // USE_MODULE_DISPLAYS_NEXTION
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
   #define USE_MODULE_DRIVERS_INTERFACE
   #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING__DUAL_OUTPUT

    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
    #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
    #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE

    #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

    #define USE_MODULE_NETWORK_WEBSERVER
    #define ENABLE_WEBSERVER_LIGHTING_WEBUI

    #define ENABLE_DEVFEATURE_LIGHTING__PALETTE_ENCODED_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT

    
    #define ENABLE_DEVFEATURE_LIGHTING__PALETTE_ENCODED_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    #define ENABLE_DEVFEATUER_LIGHT__DECODE_DYNAMIC_ENCODED_WITH_FUNCTIONS

    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":21,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":144
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          144
        ],
        "ColourPalette":"IceCream Floats",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Spanned Palette",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 0,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "PixelRange": [
          144,
          148
        ],
        "ColourPalette":"Christmas RGPBO",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static Palette",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 0,
        "BrightnessCCT": 0
      },
      "Segment2": {
        "PixelRange": [
          148,
          292
        ],
        "ColourPalette":"Christmas RGPBO",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static Palette",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 0,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING__DUAL_OUTPUT

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY
    #define USE_MODULE_ENERGY_INTERFACE
  #endif
  
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
    #define USE_MODULE_ENERGY_PZEM004T_V3
      #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
    #define MAX_ENERGY_SENSORS 4
    #define MAX_PZEM004T_DEVICES 4
    #define ENABLE_DEVFEATURE_PZEM004T__AUTOSEARCH
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__ENERGY__INA219
    #define USE_MODULE_ENERGY_INA219
    // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING
  #endif

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  #define USE_MODULE_CONTROLLER_HVAC
     #define HEATING_DEVICE_MAX 4
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

   #ifdef USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
     #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
     #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
       #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
   #endif // USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
   #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED

   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SOLAR_LUNAR_FRIENDLY_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
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
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE 
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
    #define ENABLE_DEBUG_FUNCTION_NAMES

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  

  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define USE_MODULE_SENSORS_SOLAR_LUNAR

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_INTERFACE
      #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      #define ENABLE_DEVFEATURE_BME680
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SOLAR_LUNAR     
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif


  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    // #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
    #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
    #define USE_MODULE_SENSORS_DHT
    #define USE_MODULE_SENSORS_BME
      #define ENABLE_DEVFEATURE_BME680
    #define USE_MODULE_SENSORS_BH1750
    #define USE_MODULE_SENSORS_SWITCHES
    #define USE_MODULE_SENSORS_BUTTONS
    #define USE_MODULE_SENSORS_MOTION
    #define USE_MODULE_SENSORS_LDR_BASIC

  #ifdef USE_MODULE_SENSORS_LDR_BASIC
    #define USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
    #define USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  


    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
  #define USE_MODULE_DISPLAYS_NEXTION
    #ifdef USE_MODULE_DISPLAYS_NEXTION
      #define NEXTION_DEFAULT_PAGE_NUMBER 6  
        #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
        // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
        #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
        #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
        // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS                    
        #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
        #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
        #define USE_MODULE_NETWORK_WEBSERVER
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
          #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC1  "openhab_broadcast/nextion/group/hvac_home"
          // #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS_TOPIC2  "openhab_broadcast/nextion/group/hvac_desk_power"
      
        DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
        R"=====(
        {
          "PageNames": [
            "boot_flash",
            "boot",
            "message",
            "multiline",
            "logger",
            "heating",
            "hvacdesk",
            "main"
          ],
          "DefaultPageName":"hvacdesk",
          "DefaultBaud":115200,
          "TargetBaud":115200,
          "ObjectNameID": {
            "hdIconHAI": 2,
            "hdTimeHAI": 6,
            "hdBoostHAI": 11,
            "hdAutoHAI": 16,
            "powHAI": 21,
            "hdIconFLR": 3,
            "hdTimeFLR": 7,
            "hdBoostFLR": 12,
            "hdAutoFLR": 17,
            "powFLR": 22,
            "hdIconFAN": 4,
            "hdTimeFAN": 8,
            "hdBoostFAN": 13,
            "hdAutoFAN": 18,
            "powFAN": 23,
            "hdIconOIL": 5,
            "hdTimeOIL": 9,
            "hdBoostOIL": 14,
            "hdAutoOIL": 19,
            "powOIL": 24
          }
        }
        )=====";

    #endif // USE_MODULE_DISPLAYS_NEXTION

  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  //  #define USE_MODULE_DRIVERS_INTERFACE
  //  #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  


    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
    // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

    #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
    #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
    #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
    #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
    #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__PIXEL_SET_ELSEWHERE

    #define ENABLE_DEVFEATURE_LIGHT__PHASE_OUT_TIMEMS

    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

    #define USE_MODULE_NETWORK_WEBSERVER
    #define ENABLE_WEBSERVER_LIGHTING_WEBUI

    #define ENABLE_DEVFEATURE_LIGHTING__PALETTE_ENCODED_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT

    
    #define ENABLE_DEVFEATURE_LIGHTING__PALETTE_ENCODED_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC_HEATMAPS
    // #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_NO_GRADIENT
    #define ENABLE_DEBUGFEATURE_LIGHTING__PALETTE_ENCODED_DYNAMIC__TEST_INJECT_RGB_WITH_GRADIENT
    #define ENABLE_DEVFEATUER_LIGHT__DECODE_DYNAMIC_ENCODED_WITH_FUNCTIONS

    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":4,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":0,
          "Length":16
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          16
        ],
        "ColourPalette":"Christmas RGPBO",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static Palette",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1
        },
        "Transition": {
          "TimeMs": 200,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 100
      },
      "BrightnessRGB": 5,
      "BrightnessCCT": 100
    }
    )=====";


  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"
  //     #ifdef USE_MODULE_DRIVERS_RELAY
  //     "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
  //     "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
  //     "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
  //     "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
  //     #endif
  //     "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
  //     "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
  //     #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //     "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
  //     #endif    
  //     #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
  //     "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
  //     "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
  //     #endif
  //     #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
  //     "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
  //     #endif
  //     "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
  //     // 32 - LED Strip External
  //     // 21 - LED Strip Onboard
  //     // 25?
  //     // 
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  //   "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";
  
  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"   
      /**
       * @brief Right side
       **/
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023   
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
      "\"22\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
      #endif // USE_MODULE_SENSORS__DS18X20_ESP32_2023
      // GPIO1 - TX0 - Debug Serial TX
      // GPIO3 - RX0 - Debug Serial RX
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
      "\"21\":\"" D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DHT
      "\"19\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
      "\"18\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","      
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
      "\"15\":\"" D_GPIO_FUNCTION_IR_RECV_CTR "\","
      #endif
      // #if defined(USE_MODULE_DRIVERS_RELAY)// && !defined(USE_MODULE_DISPLAYS_NEXTION_V2) // Only debug pins
      // "\"2\":\""  GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\"," //pin2 - blue led
      // "\"15\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
      // #endif
      // GPIO0 - ADC2 CH1
      /**
       * @brief Left side
       **/
      // EN
      // 36 - INPUT ONLY - VP
      // 39 - INPUT ONLY - VN
      #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
      "\"34\":\"" D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6 // INPUT ONLY
      #endif
      // 35 - INPUT ONLY - adc1_7
      // 32 - Touch9 (Debug Header 1)
      // 33 - Touch8 (Debug Header 2)
      // 25 - DAC1 = LM386 Amplifier Module (Debug Header 3)
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      #endif
      "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
      // 14 (Debug Header 4)
      // 12 (Debug Header 5)
      // 13 (Debug Header 6)
      // Can I introduce a way that a comma at the end, does not make a broken json?
      "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SOLAR_LUNAR_FRIENDLY_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "}"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "],"
  "}";




  
  // /***********************************
  //  * SECTION: Lighting Configs
  // ************************************/    

  // #define ENABLE_DEBUGFEATURE_LIGHT__OPTIONAL_COMMANDS 
  // #define ANIMATION_UPDATOR_TIME_MINIMUM 20
  // #define ENABLE_DEVFEATURE_LIGHT__CREATE_VECTOR_RGBCCT_IN_HEADER_ONLY_NEVER_CLEAR
  // #define ENABLE_DEBUG_MANUAL_DELAYS

  // #define ENABLE_DEVFEATURE_PALETTE__CHANGE_MY_PALETTE_INDEXING_TO_255_RANGE

  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_SEPTEMBER_2023

  // #define USE_LIGHTING_TEMPLATE
  // // #define USE_LIGHTING_TEMPLATE__BUSSES_MIXED_TWO_I2S_CHANNELS_WITH_TWO_SEGMENTS

  // #define STRIP_SIZE_MAX 14
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"GRBW",
  //       "BusType":"SK6812_RGBW",
  //       "Start":0,
  //       "Length":14
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       14
  //     ],
  //     "ColourPalette":96,
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function": 1,
  //       "Speed":1,
  //       "Intensity":255
  //     },
  //     "Transition": {
  //       "TimeMs": 0,
  //       "RateMs": 2000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 5,
  //   "BrightnessCCT": 0
  // }
  // )=====";
  

  // // /***********************************
  // //  * SECTION: Template Configs
  // // ************************************/    
  // // // I need to add a mqtt send limit to telemetry to prevent race conditions in OH. Add as option in configm with 0 meaning none and then MS values going up
  // // // mqtt.h will need to check if original is from telemetry type, then only permit sending when last was greater than config amount. 
  // // // The effect should be hourly update are stagarded by few seconds

  // // #define ENABLE_FEATURE_WATCHDOG_TIMER
  // // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // // // #define ENABLE_ADVANCED_DEBUGGING
  // // // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // // // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // // // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // // // #define ENABLE_DEBUG_FUNCTION_NAMES

  // // #define   ENABLE_DEBUG_LINE_HERE


  // // #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // // #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  // // // #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // //   #define USE_MODULE_NETWORK_WEBSERVER
  // // //   #define USE_MODULE_NETWORK_WEBSERVER
  // // // #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // // // #define ENABLE_FREERAM_APPENDING_SERIAL

  // // // #define USE_MODULE_SENSORS_INTERFACE
  // // // #define USE_MODULE_SENSORS_BME
  // // // #define USE_MODULE_SENSORS_MOTION

  // // // #define USE_MODULE_DISPLAYS_INTERFACE
  // // #define USE_MODULE_DISPLAYS_NEXTION

  
  // // #define USE_MODULE_DISPLAYS_NEXTION
  // //   #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  // // #define NEXTION_DEFAULT_PAGE_NUMBER 2  
  // //   #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  // //   // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
  // //   #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  // //   #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  // // #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  // // #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // // // #define USE_MODULE_NETWORK_WEBSERVER






  // #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  // #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  //   #define USE_MODULE_NETWORK_WEBSERVER
  //   #define USE_MODULE_NETWORK_WEBSERVER
  // #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // // #define ENABLE_FREERAM_APPENDING_SERIAL


  // // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_NEXTION

  
  // #define USE_MODULE_DISPLAYS_NEXTION
  //   #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  // #define NEXTION_DEFAULT_PAGE_NUMBER 2  
  //   #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  //   // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
  //   #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
  //   #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  // #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  // #define ENABLE_DEVFEATURE_NEXTION_WEBUI














  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // // #define ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES

  // // #define USE_MODULE_NETWORK_WEBSERVER

  // // #define ENABLE_LOG_LEVEL__DEBUG_TRACE
  // // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // // #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  
  // #define ENABLE_DEVFEATURE_DEVICELIST_DEBUG_WITH_COMMANDS

  // #define DISABLE_SLEEP

  // #define USE_MODULE_CORE_RULES
  //   #define D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND 5
  //   #define ENABLE_DEVFEATURE_RULES_COMMAND_CAN_USE_TRIGGER_VALUE

  // // #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  // #define USE_MODULE_SENSORS_SOLAR_LUNAR

  // // #define USE_MODULE_DRIVERS_LEDS

  // // #define USE_MODULE_DRIVERS_IRREMOTE
  // // #define USE_IR_RECEIVE

  // #define USE_MODULE_SENSORS_INTERFACE
  //   #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  //   #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  //   // #define ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
  //   #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //   #define USE_MODULE_SENSORS_DHT
  //   #define USE_MODULE_SENSORS_BME
  //     #define ENABLE_DEVFEATURE_BME680
  //   #define USE_MODULE_SENSORS_BH1750
  //   #define USE_MODULE_SENSORS_SWITCHES
  //   #define USE_MODULE_SENSORS_BUTTONS
  //   #define USE_MODULE_SENSORS_MOTION
  //   #define USE_MODULE_SENSORS_LDR_BASIC

  // #ifdef USE_MODULE_SENSORS_LDR_BASIC
  //   #define USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
  //   #define USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  // #endif // USE_MODULE_SENSORS_LDR_BASIC
  
  









  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH
  // // #define USE_MODULE_DISPLAYS_NEXTION
  // //   // Note from now on, the top few pages will also follow the exaxt same so logging/error pages will always be the same!! 
  // //   // Any special pages will follow
  // //   #define NEXTION_DEFAULT_PAGE_NUMBER 2   // I should add "p[c]" where c means current page, so I need to search and replace "p[c]" as "p[0]"
  // //   #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
  // //   #define ENABLE_DEVFEATURE_NEXTION_INCREASE_BAUDRATE

  // // //  Work on ability to increase nextion baud if possible via code

  // // //   #define ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
  // // //   #define ENABLE_DEVFEATURE_CAUTION__BLOCK_ANIMATION_LOOP_FOR_DIRECT_TESTING


  // // /**
  // //  * @brief USe Logic analyser to find out where the delays in lighting code are
  // //  * - around drawing new palette
  // //  * - time to update each blend function
  // //  * - time to output "Show"
  // //  * Add header that breaks out multiple pins into a nice female header for easy logic analyzer connection
  // //  * 
  // //  */

  // // #define USE_MODULE_CONTROLLER_HVAC
  // //   #define HEATING_DEVICE_MAX 2
  // //   #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON
  // //   // #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  // // // #define USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS
  // // #define USE_MODULE_DRIVERS_INTERFACE
  // // #define USE_MODULE_DRIVERS_RELAY

  

  // // // Actual
  // // #define GPIO_NAME_ZONE0_UPSTAIRS_RELAY    D_GPIO_FUNCTION_REL1_CTR
  // // #define GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  D_GPIO_FUNCTION_REL2_CTR
  // // // #define GPIO_NAME_ZONE3_IMMERISON_RELAY   D_GPIO_FUNCTION_REL3_CTR
  // // // #define GPIO_NAME_ZONE4_BOILER_RELAY      D_GPIO_FUNCTION_REL4_INV_CTR












  // // //here
  // // /**
  // //  * Debug pins
  // //  * */
  // // // #define ENABLE_FEATURE_DEBUG_PINS

  // // // #define DEBUG_PIN1_GPIO     32
  // // // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  // // // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  // // // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  // // // #define DEBUG_PIN2_GPIO     33
  // // // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  // // // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  // // // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  // // // #define DEBUG_PIN3_GPIO     25
  // // // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  // // // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  // // // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  // // // #define DEBUG_PIN4_GPIO     14
  // // // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  // // // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  // // // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  // // // #define DEBUG_PIN5_GPIO     12
  // // // #define DEBUG_PIN5_INIT()   pinMode(DEBUG_PIN5_GPIO, OUTPUT); digitalWrite(DEBUG_PIN5_GPIO, HIGH);
  // // // #define DEBUG_PIN5_SET(X)   digitalWrite(DEBUG_PIN5_GPIO, X);
  // // // #define DEBUG_PIN5_TOGGLE()   digitalWrite(DEBUG_PIN5_GPIO, !digitalRead(DEBUG_PIN5_GPIO));

  // // // #define DEBUG_PIN6_GPIO     13
  // // // #define DEBUG_PIN6_INIT()   pinMode(DEBUG_PIN6_GPIO, OUTPUT); digitalWrite(DEBUG_PIN6_GPIO, HIGH);
  // // // #define DEBUG_PIN6_SET(X)   digitalWrite(DEBUG_PIN6_GPIO, X);
  // // // #define DEBUG_PIN6_TOGGLE()   digitalWrite(DEBUG_PIN6_GPIO, !digitalRead(DEBUG_PIN6_GPIO));

  // #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  // #define USE_MODULE_TEMPLATE
  // DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  // "{"
  //   "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_JSON_GPIOC "\":{"   
  //     /**
  //      * @brief Right side
  //      **/
  //     #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023   
  //     "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\","
  //     "\"22\":\"" D_GPIO_FUNCTION_DS18X20_2_CTR "\","
  //     #endif // USE_MODULE_SENSORS__DS18X20_ESP32_2023
  //     // GPIO1 - TX0 - Debug Serial TX
  //     // GPIO3 - RX0 - Debug Serial RX
  //     #ifdef USE_MODULE_SENSORS_LDR_BASIC_DIGITAL
  //     "\"21\":\"" D_GPIO_FUNCTION_LDR_BASIC_DIGITAL1_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_SENSORS_DHT
  //     "\"19\":\"" D_GPIO_FUNCTION_DHT22_1_CTR "\","
  //     "\"18\":\"" D_GPIO_FUNCTION_DHT22_2_CTR "\","      
  //     #endif
  //     #ifdef USE_MODULE_SENSORS_MOTION
  //     "\"5\":\""  D_GPIO_FUNCTION_SWT1_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_DISPLAYS_NEXTION
  //     "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
  //     "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  //     "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
  //     #endif 
  //     #ifdef USE_MODULE_DRIVERS_LEDS
  //     "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
  //     #endif
  //     #ifdef USE_MODULE_DRIVERS_IR_RECEIVER
  //     "\"15\":\"" D_GPIO_FUNCTION_IR_RECV_CTR "\","
  //     #endif
  //     #if defined(USE_MODULE_DRIVERS_RELAY)// && !defined(USE_MODULE_DISPLAYS_NEXTION_V2) // Only debug pins
  //     "\"2\":\""  GPIO_NAME_ZONE0_UPSTAIRS_RELAY    "\"," //pin2 - blue led
  //     "\"15\":\"" GPIO_NAME_ZONE1_DOWNSTAIRS_RELAY  "\","
  //     #endif
  //     // GPIO0 - ADC2 CH1
  //     /**
  //      * @brief Left side
  //      **/
  //     // EN
  //     // 36 - INPUT ONLY - VP
  //     // 39 - INPUT ONLY - VN
  //     #ifdef USE_MODULE_SENSORS_LDR_BASIC_ANALOG
  //     "\"34\":\"" D_GPIO_FUNCTION_LDR_BASIC_ANALOG1_CTR "\"," // adc1_6 // INPUT ONLY
  //     #endif
  //     // 35 - INPUT ONLY - adc1_7
  //     // 32 - Touch9 (Debug Header 1)
  //     // 33 - Touch8 (Debug Header 2)
  //     // 25 - DAC1 = LM386 Amplifier Module (Debug Header 3)
  //     #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
  //     "\"26\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
  //     #endif
  //     "\"27\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","      
  //     // 14 (Debug Header 4)
  //     // 12 (Debug Header 5)
  //     // 13 (Debug Header 6)
  //     // Can I introduce a way that a comma at the end, does not make a broken json?
  //     "\"0\":\"" D_GPIO_FUNCTION_KEY1_INV_CTR   "\""
  //   "},"
  //   "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  //   "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";

  // /***********************************
  //  * SECTION: Device Configs
  // ************************************/    

  // /**
  //  * @brief HVAC zones
  //  **/
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Upstairs"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Downstairs"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Immersion"
  // #define D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "Boiler"


  // #define D_DEVICE_DRIVER_RELAY_0_NAME "Upstairs"
  // #define D_DEVICE_DRIVER_RELAY_1_NAME "Downstairs"
  // #define D_DEVICE_DRIVER_RELAY_2_NAME "Immersion"
  // #define D_DEVICE_DRIVER_RELAY_3_NAME "Boiler"

  // #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "BedroomDesk"
  // #define D_DEVICE_SENSOR_CLIMATE_BME1 "BedroomDesk-BME1"
  // #define D_DEVICE_SENSOR_CLIMATE_BME2 "BedroomDesk-BME2"
  // #define D_DEVICE_SENSOR_CLIMATE_DHT1 "BedroomDesk-DHT1"
  // #define D_DEVICE_SENSOR_CLIMATE_DHT2 "BedroomDesk-DHT2" 
  

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "DB_01"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,255,100,29,194,124,254,111]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "DB_02"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,255,100,29,205,206,170,25]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "DB_03"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,255,100,29,195,134,175,63]"
  // #define D_DEVICE_SENSOR_DB18S20_3_NAME        "DB_04"
  // #define D_DEVICE_SENSOR_DB18S20_3_ADDRESS     "[40,255,100,29,205,202,237,231]"
  // #define D_DEVICE_SENSOR_DB18S20_4_NAME        "DB_05"
  // #define D_DEVICE_SENSOR_DB18S20_4_ADDRESS     "[40,237,161,47,0,0,0,105]"
  // #define D_DEVICE_SENSOR_DB18S20_5_NAME        "DB_06"
  // #define D_DEVICE_SENSOR_DB18S20_5_ADDRESS     "[40,255,135,47,0,0,0,56]"


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"" D_JSON_DEVICENAME "\":{"
  //     "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\""
  //     "],"
  //     "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME1 "\","
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME2 "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_DHT1 "\","
  //       "\"" D_DEVICE_SENSOR_CLIMATE_DHT2 "\""
  //     "]," 
  //     "\"" D_MODULE_SENSORS_SOLAR_LUNAR_FRIENDLY_CTR "\":["
  //       "\"" "Bedroom" "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
  //     "],"  
  //     "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
  //     "],"
  //     "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\","
  //       "\"" D_DEVICE_CONTROLLER_HVAC_ZONE3_NAME "\""
  //     "]," 
  //     "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
  //       "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
  //       "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
  //       "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
  //       "\"" D_DEVICE_DRIVER_RELAY_3_NAME "\""
  //     "]"
  //   "},"
  //   "\"" D_JSON_SENSORADDRESS "\":{"
  //     "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
  //       "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\":" D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\":" D_DEVICE_SENSOR_DB18S20_2_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_3_NAME "\":" D_DEVICE_SENSOR_DB18S20_3_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_4_NAME "\":" D_DEVICE_SENSOR_DB18S20_4_ADDRESS ","
  //       "\"" D_DEVICE_SENSOR_DB18S20_5_NAME "\":" D_DEVICE_SENSOR_DB18S20_5_ADDRESS ""
  //     "}"  
  //   "}," 
  //   "\"" "HVACZone" "\":{"
  //     "\"" "SetSensor" "\":["
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME1 "\","
  //       "\"" D_DEVICE_SENSOR_CLIMATE_BME2 "\"," 
  //       "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
  //       "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\""
  //     "],"
  //     "\"" "SetOutput" "\":["
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "},"
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "},"
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_2_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "},"
  //       "{"
  //         "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
  //         "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_3_NAME "\","
  //         "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
  //       "}"
  //     "]"
  //   "}," 
  //   "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
  //   "\"MQTTSubscribe\":["
  //       "\"desksensor_slave/status/drivers_interface/+/drivers/unified\""
  //   "]"
  // "}";

  
  // // #define USE_RULES_TEMPLATE
  // // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // // R"=====(
  // // {
  // //   "AddRule": {
  // //     "Default":["Switch1Change->Relay1Follow"]
  // //   }
  // // }
  // // )=====";

  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)  // needs appending method 
  "{"
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

  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // // #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES

  // // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // // #define USE_LIGHTING_TEMPLATE
  // // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // // R"=====(
  // // {
  // //   "HardwareType":"SK6812",
  // //   "ColourOrder":"grbw",
  // //   "AnimationMode":"Effects",
  // //   "ColourPalette":"Christmas 01",
  // //   "Effects": {
  // //     "Function":1,
  // //     "Intensity":50,
  // //     "Grouping":2
  // //   },
  // //   "Transition": {
  // //     "TimeMs": 0,
  // //     "RateMs": 1000
  // //   },
  // //   "BrightnessRGB": 1
  // // }
  // // )=====";
  // // #endif // USE_MODULE_LIGHTS_INTERFACE



  // /**
  //  * @brief New group of commands that will run after 1 minute, ideal for testing code 
  //  * 
  //  */
  // #define USE_DELAYED_COMMANDS_TEMPLATE
  // DEFINE_PGM_CTR(FIRMWARE_CUSTOM_DELAYED_START_TEMPLATE)
  // "{"
  //   "\"MQTTUpdateSeconds\":{\"IfChanged\":1},"
  //   "\"MQTTSubscribe\":["
  //       "\"desksensor_slave/status/drivers_interface/+/drivers/unified\""
  //   "]"
  // "}";


  // DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  // R"=====(
  // {
  //   "ObjectNameID": {
  //     "hIconUS": 2,
  //     "hTimeUS": 6,
  //     "hBoostUS": 11,
  //     "hAutoUS": 16,
  //     "hIconDS": 3,
  //     "hTimeDS": 7,
  //     "hBoostDS": 12,
  //     "hAutoDS": 17,
  //     "hIconIH": 4,
  //     "hTimeIH": 8,
  //     "hBoostIH": 13,
  //     "hAutoIH": 18,
  //     "hIconWB": 5,
  //     "hTimeWB": 9,
  //     "hBoostWB": 14,
  //     "hAutoWB": 19,
  //     "hIconDryer": 124,
  //     "hTimeDryer": 125,
  //     "hBoostDryer": 126,
  //     "hAutoDryer": 127
  //   }
  // }
  // )=====";

#endif



              



/**
 * 
 * 
 * Ethernet R
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    left LED (upstairs)              D27
 * bl/w   centre Relay (downstairs)        D5
 * w/bl   centre LED (downstairs)          D33
 * g/w    left Relay (upstairs)            D18
 * w/br   right LED (boiler)               D26
 * br/w   right Relay (boiler)             D19
 * 
 * Ethernet N
 * 
 * w/o  gnd
 * o/w     5v
 * w/g    SK6812 D4
 * bl/w   NC
 * w/bl   NC
 * g/w    NC
 * w/br   RX2 of esp32 from nextion
 * br/w   TX2 of esp32 from nextion    
 * 

*/
#ifdef DEVICE_TESTBED__LED_MATRIX
  #define DEVICENAME_CTR          "testbed_ledmatrix"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // Auto as temporary IP
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

  #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define USE_MODULE_SENSORS_SOLAR_LUNAR


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options
  
  #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS

  #ifdef ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS  
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__MATRIX_2D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__2D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT__AUDIO_REACTIVE__1D
    #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MATRIX
    #define ENABLE_DEVFEATURE_LIGHT__HARDCODE_MATRIX_SETUP  
    #define WLED_DEBUG
    #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
    // #define ENABLE_DEVFEATURE_LIGHT__MATRIX_LOAD_PALETTE_PATCH_IN_WEBUI_PALETTE_CHANGE
  #endif 

  #define ENABLE_DEVFEATURE_LIGHT__SWITCH_TO_JOINT_NAME_AND_DATA_PROGMEM


  #define ENABLE_DEVFEATURE_LIGHT__LOAD_PULSAR_PALETTES_INTO_CRGBPALETTE_FOR_WLED_EFFECTS // If this works, all future WLED effects should simply use this method allowing faster CRGB performance. My effects will still work in my effects.



  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
  #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI


  

  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // 21 = on PCB (4 pixels)
  // 32 = external
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":32,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":256
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        256
      ],
      "ColourPalette":"Party 16",
      "PaletteMappingValues":[10,15,20],
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":1
      },
      "SegColour1": {
        "Hue": 120,
        "Sat":20,
        "BrightnessRGB":1
      },
      "SegColour2": {
        "Hue": 240,
        "Sat":100,
        "BrightnessRGB":1
      },
      "Effects": {
        "Function":"Spanned Palette",
        "Speed":127,
        "Intensity":127,
        "Decimate":0,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 25
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 20,
    "BrightnessCCT": 0
  }
  )=====";

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_FUNCTION_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_FUNCTION_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_FUNCTION_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_FUNCTION_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"33\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"
  #define D_DEVICE_SENSOR_PZEM004T_1_ADDRESS "2"
  #define D_DEVICE_SENSOR_PZEM004T_2_ADDRESS "3"
  #define D_DEVICE_SENSOR_PZEM004T_3_ADDRESS "4"
  
  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR D_DEVICE_HEATER_0_NAME
  #define D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR D_DEVICE_HEATER_1_NAME
  #define D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR D_DEVICE_HEATER_2_NAME
  #define D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR D_DEVICE_HEATER_3_NAME 
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_0_NAME
  #define D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_1_NAME
  #define D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_2_NAME
  #define D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR   D_DEVICE_HEATER_3_NAME

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SOLAR_LUNAR_FRIENDLY_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_1_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_2_ADDRESS ","
        D_DEVICE_SENSOR_PZEM004T_3_ADDRESS
      "]"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
  "}";



  
#endif





#ifdef DEVICE_TREADMILL_POWER_MONITOR
  #define DEVICENAME_CTR          "treadmill_power_monitor"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define USE_MODULE_SENSORS_SOLAR_LUNAR


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_SWITCHES
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

  // Add this, 4 rows so show the power, current, voltage and energy

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
    #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  #define MAX_ENERGY_SENSORS 1
  #define MAX_PZEM004T_DEVICES 1

  /***********************************
   * SECTION: Controller Configs
  ************************************/  

//  #define USE_MODULE_CONTROLLER__ENERGY_OLED
 #define USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"23\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Dryer"
  #define D_DEVICE_HEATER_1_NAME "FloorMat"
  #define D_DEVICE_HEATER_2_NAME "FanHeater"
  #define D_DEVICE_HEATER_3_NAME "OilRadiator"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

  /** 
   * MainBoard
   * */
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

  #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
  #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

  #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

  #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"

  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Treadmill"
  
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" "TrackingActive" "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
  "}";



  
#endif


#ifdef DEVICE_CAMERA_XIAO_TESTBED
  #define DEVICENAME_CTR          "treadmill_power_monitor"
  #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70" // primary
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  // #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES

  #define USE_MODULE_SENSORS_SOLAR_LUNAR

  #define USE_MODULE_DRIVERS_FILESYSTEM


//   /***********************************
//    * SECTION: Network Configs
//   ************************************/    

//   /***********************************
//    * SECTION: Sensor Configs
//   ************************************/  

//   #define USE_MODULE_SENSORS_INTERFACE  
//   #define USE_MODULE_SENSORS_SWITCHES
//     #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

//   /***********************************
//    * SECTION: Display Configs
//   ************************************/  

//   #define USE_MODULE_DISPLAYS_INTERFACE
//   #define USE_MODULE_DISPLAYS_OLED_SH1106
//     #define SHOW_SPLASH

//   // Add this, 4 rows so show the power, current, voltage and energy

//   /***********************************
//    * SECTION: Driver Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Lighting Configs
//   ************************************/  

//   /***********************************
//    * SECTION: Energy Configs
//   ************************************/  

//   #define USE_MODULE_ENERGY_INTERFACE
//   #define USE_MODULE_ENERGY_PZEM004T_V3
//     #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
//   #define MAX_ENERGY_SENSORS 1
//   #define MAX_PZEM004T_DEVICES 1

//   /***********************************
//    * SECTION: Controller Configs
//   ************************************/  

// //  #define USE_MODULE_CONTROLLER__ENERGY_OLED
//  #define USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"23\":\"" D_GPIO_FUNCTION_SWT1_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_FUNCTION_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_FUNCTION_PZEM0XX_TX_CTR "\","
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


//   /**
//    * @brief Drivers and Sensors for HVAC zones
//    **/
//   #define D_DEVICE_HEATER_0_NAME "Dryer"
//   #define D_DEVICE_HEATER_1_NAME "FloorMat"
//   #define D_DEVICE_HEATER_2_NAME "FanHeater"
//   #define D_DEVICE_HEATER_3_NAME "OilRadiator"

//   #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

// // {"NumDevices":4,"DeviceNameIndex":[-1,-1,-1,-1],"AddressList":[[40,140,131,47,0,0,0,230],[40,18,77,49,0,0,0,233],[40,233,112,49,0,0,0,11],[40,165,161,47,0,0,0,189]]}

//   /** 
//    * MainBoard
//    * */
//   #define D_DEVICE_SENSOR_DB18S20_01_NAME        "MainBoard-1"
//   #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,165,161,47,0,0,0,189]"

//   #define D_DEVICE_SENSOR_DB18S20_02_NAME        "MainBoard-2"
//   #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,233,112,49,0,0,0,11]"

//   #define D_DEVICE_SENSOR_DB18S20_03_NAME        "MainBoard-3"
//   #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,140,131,47,0,0,0,230]"

//   #define D_DEVICE_SENSOR_DB18S20_04_NAME        "MainBoard-4"
//   #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,18,77,49,0,0,0,233]" //233 4

//   #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
//   #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

//   #define D_DEVICE_SENSOR_BH1750_NAME "Ambient"

//   #define D_DEVICE_SENSOR_CURRENT "LEDStrip"

  
//   #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"

//   #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "Treadmill"
  
  
//   #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR

//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_ENERGY "\":{"
//         "\"DeviceCount\":1"    
//     "},"
//     "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":{"
//         "\"DeviceCount\":1"    
//     "},"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":["
//         "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR "\":["
//         "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
//       "],"
//       "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
//         "\"" "TrackingActive" "\""
//       "]"
//     "},"
//     "\"" D_JSON_SENSORADDRESS "\":{"
//       "\"" D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR "\":[" 
//         D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
//       "]"  
//     "},"
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
//   "}";



  
#endif






/**
 * @brief 
 * Make several of these, one in the garage to show progress
 * One in my bedroom to motivate me
 * 
 */
#ifdef DEVICE_LIGHTING__LED_MATRIX_BOX_01
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR          "led_matrix_box_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
    #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
    #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "Roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     

  // #define USE_MODULE_DRIVERS_FILESYSTEM
  //   #define WLED_ENABLE_FS_EDITOR
  //   #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  //   #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
  //   #define ENABLE_FEATURE_TEMPLATES__LOAD_DEFAULT_PROGMEM_TEMPLATES_OVERRIDE_FILESYSTEM

  // Settings saving and loading
  //   // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING
  //   #define ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  //   #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_AS_FULL_USER_CONFIGURATION_REQUIRING_SETTINGS_HOLDER_CONTROL
  //   #define ENABLE_DEVFEATURE_SETTINGS__INCLUDE_EXTRA_SETTINGS_IN_STRING_FORMAT_FOR_VISUAL_FILE_DEBUG
  //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  #define ENABLE_DEVFEATURE_STORAGE__SYSTEM_CONFIG__LOAD_WITH_TEMPLATES_OVERRIDE
  #define ENABLE_DEVFEATURE_STORAGE__ANIMATION_PLAYLISTS

  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  #define ENABLE_DEVFEATURE__SAVE_CRITICAL_BOOT_DATA_FOR_DEBUG_BUT_ONLY_SPLASH_ON_BOOT_FOR_NOW__EG_SSID_MQTT_SERVER_IP_ADDRESS // until devices can reliably be used without compiling per device

  #define ENABLE_DEVFEATURE_ADD_TIMESTAMP_ON_SAVE_FILES


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__LED_MATRIX

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_OCTOBER_2023

  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE

  #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE

  #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY
  
  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":256
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        256
      ],
      "ColourPalette":"Christmas Snowy 02",
      "SegColour0": {
        "Hue": 0,
        "Sat":100,
        "BrightnessRGB":5
      },
      "Effects": {
        "Function":"Shimmer",
        "Speed":127,
        "Intensity":101,
        "Decimate":0,
        "Grouping":1
      },
      "Transition": {
        "TimeMs": 0,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME          "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME  "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_FUNCTION "\":{},"
    "\"" D_JSON_BASE          "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT      "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Device Configs
  ************************************/    

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"  
    "\"Logging\":{\"SerialLevel\":\"Info\"}" 
  "}";

#endif // DEVICE_CHRISTMAS__OUTSIDE_WREATH




#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__10INCH //10inch display
  #define DEVICENAME_CTR            "nextion_testbed"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
        #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
        #define USE_MODULE_NETWORK_WEBSERVER
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK


  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6  
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


// #define USE_FEATURE_NEXTION__SERIAL_DEFAULT_BUAD_NEW_PANEL_FIRST_OTA

// #define USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT 115200


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";


#endif



#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH
  #define DEVICENAME_CTR            "nextion_desk"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 2  
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";


#endif


#ifdef DEVICE_INSTALLED__NEXTION_DISPLAY__KITCHEN_7INCH
  #define DEVICENAME_CTR            "nextion_kitchen"
  #define DEVICENAME_FRIENDLY_CTR   "Installed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Installed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883
  #define SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
#endif
#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
  #define DEVICENAME_CTR            "nextion_kitchen_testbed"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883
  #define SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH
#endif
#ifdef SUBDEVICE_TESTBED__NEXTION_DISPLAY__KITCHEN_7INCH

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_DEBUG_LINE_HERE

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID



  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 5  
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

    #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
        #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 
        #define USE_MODULE_NETWORK_WEBSERVER
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK


  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 3,
      "hTimeUS": 7,
      "hBoostUS": 12,
      "hAutoUS": 16,
      "hIconDS": 4,
      "hTimeDS": 8,
      "hBoostDS": 13,
      "hAutoDS": 17,
      "hIconIH": 5,
      "hTimeIH": 9,
      "hBoostIH": 14,
      "hAutoIH": 18,
      "hIconWB": 6,
      "hTimeWB": 10,
      "hBoostWB": 15,
      "hAutoWB": 19,
      "hIconCook": 55,
      "hTimeCook": 56,
      "hBoostCook": 57,
      "hAutoCook": 58,
      "powGardenLgt0": 151,
      "powGardenLgt10": 152,
      "powGardenLgt1": 153,
      "powOutsLght0": 154,
      "powOutsLght10": 155,
      "powOutsLght1": 156,
      "powGazLED": 157,
      "powGazWater": 158,
      "posGazSpeaker": 159
    }
  }
  )=====";

  //start here, probably roll into general command structure. I maybe need to create "COMMAND_JSON_SET_01" and 02,03 up to 5 as needed




  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";




#endif


#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_01
  #define DEVICENAME_CTR            "nextion_treadmill_01"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL
    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 230400

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION


  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

    #define ENABLE_DEVFEATURE_NEXTION__FORCE_SUBSCRIBE_TO_OPENHAB_BROADCASTS

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";


#endif


#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__TREADMILL_02
  #define DEVICENAME_CTR            "nextion_treadmill_02"
  #define DEVICENAME_FRIENDLY_CTR   "Testbed Nextion Display"
  #define DEVICENAME_ROOMHINT_CTR                   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    #define MQTT_PORT 1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  #define   ENABLE_DEBUG_LINE_HERE


  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  #define ENABLE_DEVFEATURE_NETWORK__MOVE_LIGHTING_WEBUI_INTO_SHARED_MODULE 


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
    #define USE_MODULE_NETWORK_WEBSERVER
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define USE_MODULE_SENSORS_INTERFACE
  // #define USE_MODULE_SENSORS_BME
  // #define USE_MODULE_SENSORS_MOTION

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 6 
    #define ENABLE_DEVFEATURE_NEXTION_OTA_UPLOAD_TFT
    // #define ENABLE_DEBUG_FEATURE_REVERT_TO_ERROR_PAGE_WITH_NO_UPDATE // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 230400

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // #define ENABLE_DEVFEATURE_NEXTION__PHASE_OUT_COMMAND_FORMATTED

  // #define USE_MODULE_NETWORK_WEBSERVER

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  DEFINE_PGM_CTR(DISPLAY_TEMPLATE)
  R"=====(
  {
    "ObjectNameID": {
      "hIconUS": 2,
      "hTimeUS": 6,
      "hBoostUS": 11,
      "hAutoUS": 16,
      "hIconDS": 3,
      "hTimeDS": 7,
      "hBoostDS": 12,
      "hAutoDS": 17,
      "hIconIH": 4,
      "hTimeIH": 8,
      "hBoostIH": 13,
      "hAutoIH": 18,
      "hIconWB": 5,
      "hTimeWB": 9,
      "hBoostWB": 14,
      "hAutoWB": 19,
      "hIconDryer": 122,
      "hTimeDryer": 123,
      "hBoostDryer": 124,
      "hAutoDryer": 125
    }
  }
  )=====";


#endif


#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
