#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H


/*********************************************************************************************\
 * Devices I am currently working on, kept as its own file to make it easier
 * 
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

#define DEVICE_HVAC_BEDROOM_4CHANNEL_WITH_ENERGY_SENSORS

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

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  /***********************************
   * SECTION: System Configs
  ************************************/     

  #define ENABLE_DEVFEATURE_BUILD_REPAIR__FIXING_COMPILE_FOR_SONOFF_BASIC_DEC2023

  #define ENABLE_DEVFEATURE_RTC_SETTINGS

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
    #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
    #define DS18X20_MAX_SENSORS 20
      #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20     
  #define USE_MODULE_SENSORS_BME
    #define ENABLE_DEVFEATURE_BME680
  #define USE_MODULE_SENSORS_SOLAR_LUNAR
  #define USE_MODULE_SENSORS_BH1750
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

  /***********************************
   * SECTION: Driver Configs
  ************************************/  

  #define ENABLE_DEVFEATURE_GETDEVICEIDBYNAME_V3             
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  //  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL2_FLASHING_BASIC        // ie shimmering. Used around house all year
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED     // ie christmas. Seasonal, flashing
  // #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE     // ie all options

  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_FEBRUARY_2023
  #define ENABLE_NEOPIXELBUS_BUSMETHODS__I2S1_PARALLEL_8_CHANNELS_MODE
  #define ENABLE_DEVFEATURE_LIGHTS__DECIMATE
  #define ENABLE_DEVFEATURE_LIGHTS__EFFECT_ROTATE_PREV_WITH_INTENSITY  
  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__HARDWARE_TESTING      // effects that enable colour mapping for counting positions and testing hardware/pins
  #define ENABLE_DEVFEATURE_JSON__ASYNCJSON_V6

  #define ENABLE_FEATURE_ANIMATORLIGHT_EFFECT_SPECIALISED__MANUAL

  #define USE_MODULE_NETWORK_WEBSERVER
  #define ENABLE_WEBSERVER_LIGHTING_WEBUI
  
  // ARRAY_SIZE

  // NEw effect idea, lava lamp, both 1D and matrix. Have lgihting "gravity" then also "heating" for uplifting of random colours.

  // 13, 18, 19, 22, 23, 25, 26, 27       USED
  // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":21,
  //       "ColourOrder":"GRBC",
  //       "BusType":"SK6812_RGBW",
  //       "Start":0,
  //       "Length":144
  //     },
  //     {
  //       "Pin":32,
  //       "ColourOrder":"GRBC",
  //       "BusType":"SK6812_RGBW",
  //       "Start":144,
  //       "Length":4
  //     }
  //   ],
  //   "Segment1": {
  //     "PixelRange": [
  //       144,
  //       148
  //     ],
  //     "ColourPalette":"IceCream Floats",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function":"Static Palette",
  //       "Speed":127,
  //       "Intensity":255,
  //       "Decimate":0,
  //       "Grouping":1
  //     },
  //     "Transition": {
  //       "TimeMs": 200,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       144
  //     ],
  //     "ColourPalette":"Christmas RGPBO",
  //     "SegColour0": {
  //       "Hue": 0,
  //       "Sat":100,
  //       "BrightnessRGB":5
  //     },
  //     "Effects": {
  //       "Function":"Static Palette",
  //       "Speed":127,
  //       "Intensity":255,
  //       "Decimate":0,
  //       "Grouping":1
  //     },
  //     "Transition": {
  //       "TimeMs": 200,
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0
  //   },
  //   "BrightnessRGB": 24,
  //   "BrightnessCCT": 0
  // }
  // )=====";



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
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        144
      ],
      "ColourPalette":"IceCream Floats",
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
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 24,
    "BrightnessCCT": 0
  }
  )=====";

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  #define USE_MODULE_ENERGY_INTERFACE
  #define USE_MODULE_ENERGY_PZEM004T_V3
    #define ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY // If energy_interface is primary reporting, reduce pzem to slower (debug only)
  #define MAX_ENERGY_SENSORS 4
  #define MAX_PZEM004T_DEVICES 4
  #define USE_MODULE_ENERGY_INA219
  // #define ENABLE_DEVFEATURE_ENERGY__DISABLE_ENERGY_INTERFACE_FOR_DEBUGGING



  /***********************************
   * SECTION: Controller Configs
  ************************************/  

  #define USE_MODULE_CONTROLLER_HVAC
    #define HEATING_DEVICE_MAX 4
    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON

  #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
    #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1

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
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
      "\"23\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
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


#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVEDEVELOPMENT_H
