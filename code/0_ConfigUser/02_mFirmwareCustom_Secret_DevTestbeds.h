#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** New code under test, not yet ready to be used as a template (ie may not work yet)
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"
#include "6_Lights/03_Animator/EffectNames/defines.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"

//--------------------------------[Enable Device]-------------------------------------

/**
 * DEVICE_TYPE CORE: Any testbeds related to core system
 * */
// #define DEVICE_TESTBED__SETTINGS_STORING
// #define DEVICE_TESTBED__FILESYSTEM


/**
 * DEVICE_TYPE NETWORK: Any testbeds related to lighting
 * */
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01
// #define DEVICE_MAVLINK_DECODER_LOCATOR_LTE_DOWNLINK_01
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_01
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_02
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_03
// #define DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_04_MQTT_ON_WIFI
// #define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__2024
#define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_02__2024

/**
 * DEVICE_TYPE LIGHTING: Any testbeds related to lighting
 * */
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_V2
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_02
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM800L
// #define DEVICE_TESTBED_ESP32_BUZZER_TONES
// #define DEVICE_TESTBED_MAVLINK_DECODER
// #define DEVICE_TESTBED_MAVLINK_TELEMETRY_WIFI_01
// #define DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_01

/**
 * DEVICE_TYPE DRIVERS: Any testbeds related to lighting
 * */


/**
 * DEVICE_TYPE CONTROLLERS: Any testbeds related to lighting
 * */


/**
 * DEVICE_TYPE DISPLAYS: Any testbeds related to lighting
 * */
// #define DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH

/**
 *  SHORT TERM DEVICES IN BEDROOM -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- SHORT TERM DEVICES -- 
**/
// #define DEVICE_TESTBED_CAMERA_SENSOR_MODULE

// #define DEVICE_TESTBED_RTC_FASTBOOT
// #define DEVICE_TESTBED_BUTTON_MULTIPRESS
// #define DEVICE_TESTBED_FONA_800L_BASIC
// #define DEVICE_TESTBED_RCSWITCH_NORMAL
// #define DEVICE_testbed_rcs_ext
// #define DEVICE_TESTBED_FASTBOOT_ESP8266
// #define DEVICE_TESTBED_CRASHREPORT
// #define DEVICE_TESTBED_OLED_SH1106


//-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------



/* ** 

3D printed base, to hold an esp32 that replicates the whole hvac heating for the house from a hardware perspective.

Four single socket outputs, with pzem 100A CT monitoring them.

Hardware to implement:
- 4 PZEM 400A
- 4 16A relays (remove V2 relays, and make sure its just one going forward)
- 2 BME680
- 8 DB18S20 across 2 pins for pin developing
- 1 DHT22
- 4 sk6812 strip to show relays active
- another strip (and pin) with maybe 10 144/m pixels that should show the 8 DB18S20 sensors in a colour gradient
- maybe peizo buzzer for alarm
- Add OLED for debug
- Nextion 2.5inch could be used for manual control and feedback
- Add BH1750 for light level (since i2c is already in use)

Software to implement:
- Filesystem
  -- Heating schedules should be done, so the electric floor mat automatically turns on, this needs to include power cycle tool to stop the timeout
- Should enable integration with remote sensor to use the rooms temperature as the target temperature for oil heater




Blue (Upstairs Link) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           R_IH           21*           Immersion relay (5v IO)
 * bl/w          I2D            12*           DHT22?     
 * w/bl          I2C            13*           UNUSED   
 * g/w           3V3            3V3           
 * w/br                      
 * br/w          DSX            14*           ds18b20 water, 4k7 pulled high        (comes from blue by connector)   NOT WORKING, wiring too long, Needs rerouted
 * ** Green (Downstairs Sensors) **************************************************************************
 * w/o           GND            GND           Black
 * o/w           5V             5V            Red
 * w/g           1Wire          4*            DS18B20 water pipe sensors
 * bl/w          I2D            22*           BME in Dinning Room (Alternatively, DHT22 data pin)
 * w/bl          I2C            23*           BME in Dinning Room
 * g/w           3V3            3V3           White
 * w/br                                       NC
 * br/w          DHT22          25*           DHT22 (until BME is working)            
 * ** Orange (Nextion Display) **************************************************************************
 * w/o           GND            GND           
 * o/w           5V             5V            
 * w/g           NEO            27*             SK6812 4 pixels of rgbW
 * bl/w                                      
 * w/bl                                      
 * g/w           3V3            3V3           
 * w/br          RX2            17*              Nextion TX
 * br/w          TX2            16*              Nextion RX   -- "SERIAL TX" of Serial will always be dominant colour (brown) as its important to know the output pin
 * Twin          Switch
 * Twin          Switch
 * ** Power Screw Jacks **************************************************************************
 * 4 (Top)       12V
 * 3             5V
 * 2
 * 1 (Bottom)    GND
 * ** ADC Input **************************************************************************
 * 4 (Top)       LDR_US         33
 *               LDR_DS         32 
 *               LDR_WB         35
 * Extra Ethernet for LDRs hot glued onto the red led of the servos?
 * 
 * Upstairs Connectors
 * 3pin (DHT22)    - gnd,5v,dht_data 
 * 3pin (relay US) - gnd,5v,relay_ih
 * 3pin (water sensors) - gnd,3v3,ds18b20
 * 
 * Create seond device dad can swap in, only have basic relay control, no sensors, so it should be the most stable.
 * 
 **/
#ifdef DEVICE_TESTBED__HEATING_ENERGY_SYSTEM
  #define DEVICENAME_CTR          "heating"
  #define DEVICENAME_FRIENDLY_CTR "HVAC House Heating 2023#2"
  #define DEVICENAME_ROOMHINT_CTR "Hallway"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define ENABLE_DEVFEATURE_GETDEVICEIDBYNAME_V3

  #define DISABLE_SLEEP // loops per second less than 1hz // I need to make an "mqtt/alert" channel that lets me know this
  
  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES_SK6812_FOR_ROOM_SENSORS
  // #define USE_TEMPLATED_DEFAULT_LIGHTING_TEMPLATE_SK6812_FOR_ROOM_SENSORS__BOOT_STATE_OFF
  //   #define STRIP_SIZE_MAX 10

  //   #define USE_RGB_OUT_BASIC_SHOW_PALETTE


  #define USE_MODULE_CONTROLLER_HVAC
    #define HEATING_DEVICE_MAX 3
    #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_NEW_HVAC_TIMEON

  #define USE_MODULE_SENSORS_INTERFACE
    #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS__DS18X20_ESP32_2023
    #define DS18X20_MAX_SENSORS 20
      #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20      
  #define USE_MODULE_SENSORS_DHT
              
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  #define USE_MODULE_SENSORS_SWITCHES
  
    // #define SHOW_SPLASH
    // #define USE_MODULE_DISPLAYS_NEXTION
    // #define NEXTION_DEFAULT_PAGE_NUMBER 3   // I should add "p[c]" where c means current page, so I need to search and replace "p[c]" as "p[0]"


  // Actual
  #define GPIO_NAME_ZONE0_DOWNSTAIRS_RELAY  D_GPIO_FUNCTION_REL1_INV_CTR
  #define GPIO_NAME_ZONE1_UPSTAIRS_RELAY    D_GPIO_FUNCTION_REL2_INV_CTR
  #define GPIO_NAME_ZONE2_BOILER_RELAY      D_GPIO_FUNCTION_REL3_INV_CTR
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

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"5\":\""  GPIO_NAME_ZONE0_DOWNSTAIRS_RELAY  "\","
      "\"18\":\"" GPIO_NAME_ZONE1_UPSTAIRS_RELAY    "\","
      "\"19\":\"" GPIO_NAME_ZONE2_BOILER_RELAY      "\","
      #endif
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"33\":\""  D_GPIO_FUNCTION_SWT1_INV_CTR  "\","
      "\"27\":\""  D_GPIO_FUNCTION_SWT2_INV_CTR  "\","
      "\"26\":\""  D_GPIO_FUNCTION_SWT3_INV_CTR  "\","
      #endif  
      #ifdef USE_MODULE_SENSORS_DHT
      "\"25\":\"" D_GPIO_FUNCTION_DHT22_1_CTR   "\"," // DiningRoom 
      #endif
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
      "\"4\":\"" D_GPIO_FUNCTION_RGB_DATA_CTR  "\","
      #endif 
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_FUNCTION_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_FUNCTION_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
      "\"23\":\"" D_GPIO_FUNCTION_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""   // builtin led
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_DRIVER_RELAY_0_NAME "Downstairs"
  #define D_DEVICE_DRIVER_RELAY_1_NAME "Upstairs"
  #define D_DEVICE_DRIVER_RELAY_2_NAME "Boiler"

  #define D_DEVICE_SENSOR_DHT_0_NAME "Downstairs_DHT"

  /**
   * @brief HVAC zones
   **/
  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Downstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "Upstairs"
  #define D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "Boiler"

  /** 
   * Pin_DS
   * */
  #define D_DEVICE_SENSOR_DB18S20_06_NAME        "Water21-Upstairs"
  #define D_DEVICE_SENSOR_DB18S20_06_ADDRESS     "[40,208,174,149,240,1,60,127]"

  #define D_DEVICE_SENSOR_DB18S20_07_NAME        "Water22-HotCross"
  #define D_DEVICE_SENSOR_DB18S20_07_ADDRESS     "[40,168,253,149,240,1,60,157]"

  #define D_DEVICE_SENSOR_DB18S20_08_NAME        "Water23-R/C"
  #define D_DEVICE_SENSOR_DB18S20_08_ADDRESS     "[40,12,164,2,0,0,0,72]"

  #define D_DEVICE_SENSOR_DB18S20_09_NAME        "Water24-Mains"
  #define D_DEVICE_SENSOR_DB18S20_09_ADDRESS     "[40,9,77,4,0,0,0,131]"

  #define D_DEVICE_SENSOR_DB18S20_10_NAME        "Water25-HotFromBoiler"
  #define D_DEVICE_SENSOR_DB18S20_10_ADDRESS     "[40,121,172,3,0,0,0,138]"

  #define D_DEVICE_SENSOR_DB18S20_11_NAME        "Water26-Downstairs"
  #define D_DEVICE_SENSOR_DB18S20_11_ADDRESS     "[40,205,241,149,240,1,60,148]"

  #define D_DEVICE_SENSOR_DB18S20_12_NAME        "Water27-R/H"
  #define D_DEVICE_SENSOR_DB18S20_12_ADDRESS     "[40,195,112,2,0,0,0,178]"

  #define D_DEVICE_SENSOR_DB18S20_13_NAME        "Water28-HotFromFurnace"
  #define D_DEVICE_SENSOR_DB18S20_13_ADDRESS     "[40,103,49,3,0,0,0,153]"

  #define D_DEVICE_SENSOR_DB18S20_14_NAME        "Water29-WaterBoiler"
  #define D_DEVICE_SENSOR_DB18S20_14_ADDRESS     "[40,183,162,149,240,1,60,24]"


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_1_NAME "\","
        "\"" D_DEVICE_DRIVER_RELAY_2_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE1_NAME "\","
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE2_NAME "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\":" D_DEVICE_SENSOR_DB18S20_06_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\":" D_DEVICE_SENSOR_DB18S20_07_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\":" D_DEVICE_SENSOR_DB18S20_08_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\":" D_DEVICE_SENSOR_DB18S20_09_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\":" D_DEVICE_SENSOR_DB18S20_10_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\":" D_DEVICE_SENSOR_DB18S20_11_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\":" D_DEVICE_SENSOR_DB18S20_12_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\":" D_DEVICE_SENSOR_DB18S20_13_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\":" D_DEVICE_SENSOR_DB18S20_14_ADDRESS ""
      "}"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_0_NAME "\","
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
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
  "}";
  
#endif


#ifdef DEVICE_TESTBED_CAMERA_SENSOR_MODULE
  #define DEVICENAME_CTR          "testbed_camera_sensor"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Ensuite"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BH1750

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"3\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"1\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\"," //should be 27, missoldered, repair later
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"16\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"
  
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
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "},"    
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)

  "}";


  #define STRIP_SIZE_MAX 58
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_SK6812_METHOD_DEFAULT
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"SK6812",
    "AnimationMode":"Effects",
    "ColourOrder":"grbw",
    "ColourPalette":"Rgbcct 01",
    "Effects": {
      "Function":0,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "SegColour": {
      "Hue": 18,
      "Sat": 100,
      "SubType":3
    },
    "BrightnessRGB":0,
    "BrightnessCCT":0
  }
  )=====";
  #endif // USE_MODULE_LIGHTS_INTERFACE
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"// for PIR to follow
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

#endif



/**
 * @brief Integrating best camera code
 * 
 */
#ifdef DEVICE_TESTBED_CAMERA_OV26_MODULE
  #define DEVICENAME_CTR          "testbed_camera"
  #define DEVICENAME_FRIENDLY_CTR "Ensuite Sensor"
  #define DEVICENAME_ROOMHINT_CTR "Ensuite"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define DISABLE_SERIAL
  #define DISABLE_SERIAL0_CORE
  #define DISABLE_SERIAL_LOGGING

  #define USE_MODULE_CORE_RULES
       
  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_BME
  #define USE_MODULE_SENSORS_SWITCHES
  #define USE_MODULE_SENSORS_MOTION
  #define USE_MODULE_SENSORS_BH1750

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_BME
      "\"3\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"1\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\"," //should be 27, missoldered, repair later
      #endif
      #ifdef USE_MODULE_SENSORS_MOTION
      "\"16\":\"" D_GPIO_FUNCTION_SWT1_CTR   "\""
      #endif
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 1
  #define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Ensuite"
  #define D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "Ensuite"
  
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
      "],"
      "\"" D_MODULE_SENSORS_BH1750_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_CLIMATE_FRIENDLY_NAME_LONG "\""
      "]"
    "},"    
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)

  "}";


  #define STRIP_SIZE_MAX 58
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  #define USE_SK6812_METHOD_DEFAULT
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "HardwareType":"SK6812",
    "AnimationMode":"Effects",
    "ColourOrder":"grbw",
    "ColourPalette":"Rgbcct 01",
    "Effects": {
      "Function":0,
      "Intensity":50
    },
    "Transition": {
      "TimeMs": 0,
      "RateMs": 1000
    },
    "SegColour": {
      "Hue": 18,
      "Sat": 100,
      "SubType":3
    },
    "BrightnessRGB":0,
    "BrightnessCCT":0
  }
  )=====";
  #endif // USE_MODULE_LIGHTS_INTERFACE
  
  #define USE_RULES_TEMPLATE
  DEFINE_PGM_CTR(RULES_TEMPLATE)
  "{"// for PIR to follow
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

#endif






/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G
  #define DEVICENAME_CTR          "testbed_sim7000g"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define MQTT_PORT 51883 //external mqtt broker on TOWER 

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM


  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

    #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G


/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_V2
  #define DEVICENAME_CTR          "testbed_sim7000g_v2"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM


  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

    // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker02"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G



/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_01
  #define DEVICENAME_CTR          "cellular_locator_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION //fails with no network
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define MQTT_PORT 51884 //external mqtt broker on TOWER 

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM

  #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

    // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G



/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM7000G_CELLULAR_LOCATOR_02
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  
  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define MQTT_PORT 51883 //external mqtt broker on TOWER 

  /**
   * @brief WiFi MQTT
   * 
   */
  // #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  #define DISABLE_NETWORK_WIFI
  #define USE_MODULE_NETWORK_CELLULAR
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  #define USE_MODULE_SENSORS_GPS_MODEM
  #define USE_MODULE_SENSORS_INTERFACE
  #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  #define USE_MODULE_SENSORS_BATTERY_MODEM


  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

    #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"  
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_MAVLINK_DECODE_R
  #define DEVICENAME_CTR          "testbed_mavlink_decoder"
  #define DEVICENAME_FRIENDLY_CTR "Testbed MAVLink Decoder"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20

  












  #define USE_MODULE__DRIVERS_MAVLINK_DECODER
    #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
    #define USE_DEVFEATURE_MAVLINK_DECOVER_SERIAL_NUMBER 2

  #define USE_MODULE_CORE_SERIAL_UART
    #define ENABLE_HARDWARE_UART_2
    #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST


  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
  // #define MQTT_PORT 51883 //external mqtt broker on TOWER 


  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

  // #define USE_MODULE_CORE_SERIAL_UART


  //   #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"17\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\""   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"  
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";

#endif // DEVICE_TESTBED_MAVLINK_DECODER



/**
 * @brief 
 * 
 * @brief 6P connector
 * Green VIN (5v)
 * Orange 18
 * Yellow 19
 * White 25
 * Red 26
 * Black gnd
 *
 *
 * @brief 
 * Connecting 6P to pixhawk 2.4.8 with Serial5 enabled will clash with modem serial
 * 
 *
 * Next priority is getting cell/wifi mqtt swapping working well
 * 
 */
#ifdef DEVICE_MAVLINK_DECODER_LOCATOR_LTE_DOWNLINK_01
  #define DEVICENAME_CTR          "mavlink_cellular_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
  // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
  #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 100
  #define ENABLE_DEBUG_FUNCTION_NAMES


  #define USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  // #define USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE
  #define USE_GROUPFEATURE__MODEM_GPS
  // #define USE_GROUPFEATURE__MAVLINK_DECODER
  // #define USE_GROUPFEATURE__OLED_SH1106_MAVLINK
  #define USE_GROUPFEATURE__MQTT_ON_WIFI_AND_CELLULAR

  // *************************************************************************************

  #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
    #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  #endif
 
  #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE_SERIAL_UART
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif

  #ifdef USE_GROUPFEATURE__MODEM_GPS
    #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define USE_MODULE_SENSORS_GPS_MODEM
  #endif 
  #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    // #define MQTT_SOCKET_TIMEOUT 1
    // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
     #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable
    // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
    // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    // #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
    // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
    #define ENABLE_DEVFEATURE_MQTT_ENABLE_CONSECUTIVE_TELEMETRY_TOPICS_SEND_LIMIT // ie if one sends, return early from sending others
  #endif

  #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    #define USE_MODULE_SENSORS_INTERFACE
    // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
    // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
  #endif

  #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE
    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
    #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
    #define MQTT_PORT     1883
  #endif // USE_GROUPFEATURE__MQTT_AS_WIFI_WHEN_CELLULAR_IS_ACTIVE

  #if !defined(USE_GROUPFEATURE__MQTT_AS_CELLULAR) && !defined(USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS)
    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
  #endif

  #ifdef USE_GROUPFEATURE__MQTT_ON_WIFI_AND_CELLULAR

    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
    #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
    #define MQTT_PORT     1883

  #endif

  #ifdef USE_GROUPFEATURE__OLED_SH1106_MAVLINK
    #define USE_MODULE_DISPLAYS_INTERFACE
    #define USE_MODULE_DISPLAYS_OLED_SH1106
      #define SHOW_SPLASH
    #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
  #endif


// #define UART_BAUD   115200
// #define PIN_DTR     25
// #define PIN_TX      27
// #define PIN_RX      26
// #define PWR_PIN     4

// #define SD_MISO     2
// #define SD_MOSI     15
// #define SD_SCLK     14
// #define SD_CS       13
// #define LED_PIN     12

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    
    // "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G








/**
 * @brief 
 * Minimal ESP32 7000G Modem to log position in realtime and SMS readback
 */
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_01
  #define DEVICENAME_CTR          "cellular_locator_01"
  #define DEVICENAME_FRIENDLY_CTR "Selfcontained tracker with SIM staying" // Antennas covers SIM, but flip the board later
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   921600
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "10" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0"
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_02
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "MAVlink ^Pin "
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   921600
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_03
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Plane - Optional LIPO for weight on Plane"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define SIM_CARD_PHONE_NUMBER "07518522105"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   115200
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60"
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_04_MQTT_ON_WIFI
  /**
   * @brief Device should run the same code designed for LTE MQTT devices, but connect via WiFi and therefore
   * test that LTE network stuff is not breaking the normal WiFi stuff and stable code is being developed for both methods.
   **/
  #define DEVICENAME_CTR          "cellular_locator_04"
  #define DEVICENAME_FRIENDLY_CTR "Tester - Run MQTT over WiFi without LTE connected"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define USE_GROUPFEATURE__MQTT_AS_WIFI
  #define UART_CELLULAR_BAUD   115200
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
#ifdef SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH


  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  
  // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
  #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #define ENABLE_FEATURE_WATCHDOG_TIMER
    #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
  //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

  #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
    #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
    #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
    #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
    // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
    #define ENABLE_DEBUG_FUNCTION_NAMES
    #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
    // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
    #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
    // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
    #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
    #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
    // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
    // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
    #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
    // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
    #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
  #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
    #define ENABLE_ADVANCED_DEBUGGING
    #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


  #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


  // *************************************************************************************

  #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
    #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  #endif
 
  #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE_SERIAL_UART
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif

  #ifdef USE_GROUPFEATURE__MODEM_GPS
    #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define USE_MODULE_SENSORS_GPS_MODEM
  #endif 
  #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    #define USE_MODULE_NETWORK_MQTT
    // #define MQTT_SOCKET_TIMEOUT 1
    // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
     #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
    // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
    // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
    // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
  #endif
  #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
    #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
    #define MQTT_PORT     1883
  #endif

  #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    // #define USE_MODULE_SENSORS_INTERFACE
    // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
    // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
  #endif


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE_NETWORK_CELLULAR
      "\"25\":\"" D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION__MODEM_TX__CTR   "\","   
      "\"26\":\"" D_GPIO_FUNCTION__MODEM_RX__CTR   "\","   
      "\"4\":\""  D_GPIO_FUNCTION__MODEM_POWER__CTR   "\","   
      #endif // USE_MODULE_NETWORK_CELLULAR   
      #ifdef USE_MODULE_DRIVERS_SDCARD
      "\"2\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR   "\","
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR   "\","   
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","  
      #endif // USE_MODULE_DRIVERS_SDCARD   
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
    "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
  "}";

#endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01


/**
 * @brief Self contained 2024 testbed, with integrated battery for car tracking for testing
 * 
 */
#ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__2024
  #define DEVICENAME_CTR          "cellular_locator_01"
  #define DEVICENAME_FRIENDLY_CTR "Selfcontained tracker with SIM staying" // Antennas covers SIM, but flip the board later
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   921600
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "10" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0"
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
#endif
/**
 * @brief 2024 Device
 * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
 * 
 */
#ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_02__2024
  #define DEVICENAME_CTR          "cellular_locator_02"
  #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
  #define DEVICENAME_ROOMHINT_CTR "roaming"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
  #define SIM_CARD_PHONE_NUMBER "07518522105"
  #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
  #define UART_CELLULAR_BAUD   115200
  #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
  #define USE_GROUPFEATURE__MODEM_GPS
  #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
  #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60"
  #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024
#endif
#ifdef SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024

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
  

  /***********************************
   * SECTION: Network Configs
  ************************************/    



  /***********************************
   * SECTION: Lighting Configs
  ************************************/    


  // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
  #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #define ENABLE_FEATURE_WATCHDOG_TIMER
    #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
  //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

  #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
    #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
    #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
    #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
    // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
    #define ENABLE_DEBUG_FUNCTION_NAMES
    #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
    // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
    #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
    #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
    // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
    #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
    #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
    #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
    // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
    // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
    #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
    // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
    #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
  #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


  #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
    #define ENABLE_ADVANCED_DEBUGGING
    #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


  #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


  // *************************************************************************************

  #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
    #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  #endif 
  #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
    #define USE_MODULE__DRIVERS_MAVLINK_DECODER
      #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
      #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
      #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
    #define USE_MODULE_CORE_SERIAL_UART
      #define ENABLE_HARDWARE_UART_2
      #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #endif
  #ifdef USE_GROUPFEATURE__MODEM_GPS
    #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define USE_MODULE_SENSORS_GPS_MODEM
  #endif 
  #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    #define USE_MODULE_NETWORK_MQTT
    // #define MQTT_SOCKET_TIMEOUT 1
    // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
     #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
    // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
    // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
    // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
  #endif
  #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
    #define USE_MODULE_NETWORK_WIFI
    #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
    #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
    #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
    #define MQTT_PORT     1883
  #endif
  #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
    #define DISABLE_NETWORK_WIFI
    #define USE_MODULE_NETWORK_CELLULAR
    #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
    // #define USE_MODULE_SENSORS_INTERFACE
    // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
    // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
    // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
  #endif


  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      #ifdef USE_MODULE_NETWORK_CELLULAR
      "\"25\":\"" D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
      "\"27\":\"" D_GPIO_FUNCTION__MODEM_TX__CTR   "\","   
      "\"26\":\"" D_GPIO_FUNCTION__MODEM_RX__CTR   "\","   
      "\"4\":\""  D_GPIO_FUNCTION__MODEM_POWER__CTR   "\","   
      #endif // USE_MODULE_NETWORK_CELLULAR   
      #ifdef USE_MODULE_DRIVERS_SDCARD
      "\"2\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR   "\","
      "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR   "\","   
      "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
      "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","  
      #endif // USE_MODULE_DRIVERS_SDCARD   
      #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
      "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
      "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
      #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


  /***********************************
   * SECTION: Device Configs
  ************************************/    
 
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
    "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
  "}";

#endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01



/**
 * @brief 
 * 
 * 
// Add function to "settime_update" ie with GPS ticks. When it happens, reset the 
// update millis so it does not double update, same for uptime
 * 
 * 
 * If LTE connects, then set the mqtt retry counter to immediate
 * 
 */







/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_MAVLINK_TELEMETRY_WIFI_01
  #define DEVICENAME_CTR          "testbed_mavlink_telemetry_01"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"


  #define ENABLE_DEBUG_FUNCTION_NAMES
  #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20

  
  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define USE_MODULE__DRIVERS_MAVLINK_DECODER
  #define USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI

  #define USE_DEVFEATURE_DEFINED_SERIAL2

  #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES


  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI


  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  // // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION //fails with no network
  // // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20

  
  // // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // // #define USE_MODULE_NETWORK_MQTT
  // // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // // #define USE_SSIDS_NONE_DEBUGGING
  // // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // // #define DISABLE_NETWORK

  // // #define DISABLE_SERIAL
  // // #define DISABLE_SERIAL0_CORE
  // // #define DISABLE_SERIAL_LOGGING

  // // #define ENABLE_ADVANCED_DEBUGGING
  // // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define USE_MODULE__DRIVERS_MAVLINK_DECODER

  // #define MQTT_PORT 51883 //external mqtt broker on TOWER 

  #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART


  // #define USE_DEVFEATURE_DEFINED_SERIAL2

 #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
//  #define MQTT_PORT 51883 //external mqtt broker on TOWER  : Unstable

  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  // #define USE_MODULE_SENSORS_BATTERY_MODEM

  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // // #define USE_MODULE_NETWORK_MQTT
  // // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // // #define USE_SSIDS_NONE_DEBUGGING
  // // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // // #define DISABLE_NETWORK

  // // #define DISABLE_SERIAL
  // // #define DISABLE_SERIAL0_CORE
  // // #define DISABLE_SERIAL_LOGGING

  // // #define ENABLE_ADVANCED_DEBUGGING
  // // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define MQTT_PORT 51884 //external mqtt broker on TOWER 

  // /**
  //  * @brief WiFi MQTT
  //  * 
  //  */
  // // #define USE_MODULE_NETWORK_WIFI
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // // #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  // /**
  //  * @brief Cellular MQTT
  //  * 
  //  */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  // #define USE_MODULE_SENSORS_BATTERY_MODEM

  // #define ENABLE_DEVFEATURE_DISABLE_MQTT_FREQUENCY_REDUNCTION_RATE

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

  //   // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX

  // #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED

//  6P connector

//  Green VIN (5v)
// Orange 18
// Yellow 19
// White 25
// Red 26
// Black gnd


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
      "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_BUZZER_TONES
  #define DEVICENAME_CTR          "testbed_buzzer_tones"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Version 2"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE__DRIVERS_BUZZER_TONES

  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      "\"23\":\"" D_GPIO_FUNCTION_BUZZER_CTR   "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker02"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    

    "\"BuzzerMode\":{\"FreqMode\":1},"





    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G




/**
 * @brief 
 * Primary testbed for all new lighting code on nodemcu
 */
#ifdef DEVICE_TESTBED_ESP32_LILYGO_SIM800L
  #define DEVICENAME_CTR          "testbed_sim800l"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Segment Multiple Pin String"
  #define DEVICENAME_ROOMHINT_CTR "testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SOLAR_LUNAR

  #define USE_MODULE_NETWORK_MQTT
  // #define USE_MODULE_NETWORK_MQTT_MULTIPLE

  // #define USE_SSIDS_NONE_DEBUGGING
  // #define DISABLE_DEVFEATURE_NETWORK_WIFI

  // #define DISABLE_NETWORK

  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  // #define DISABLE_SERIAL_LOGGING

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  /**
   * @brief WiFi MQTT
   * 
   */
  #define USE_MODULE_NETWORK_WIFI
  #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  #define ENABLE_DEVFEATURE_MQTT_USING_WIFI

  /**
   * @brief Cellular MQTT
   * 
   */  
  // #define DISABLE_NETWORK_WIFI
  // #define USE_MODULE_NETWORK_CELLULAR
  // #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
  // #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
  // #define USE_MODULE_SENSORS_GPS_MODEM
  // #define USE_MODULE_SENSORS_INTERFACE
  // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR

  // #define USE_MODULE_SENSORS_BATTERY_MODEM


  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

  //   #define USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIO_NUMBER "\":{"
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\""   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularTracker01"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
      "]"
    "},"   


    "\"" D_JSON_DISPLAY "\":{"
      "\"" "DisplayRows" "\":8,"
      "\"" "DisplayCols" "\":[21,2],"
      "\"" "DisplaySize" "\":1"
    "},"    





    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
    "\"MQTT\":{\"RetrySecs\":10}"
  "}";


  // #define USE_FUNCTION_TEMPLATE
  // DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  // "{"
  //   "\"MQTT\":{\"RetrySecs\":10}"
  // "}";
  /*
  https://infocenter.nordicsemi.com/index.jsp?topic=%2Fref_at_commands%2FREF%2Fat_commands%2Ftext_mode%2Fcnmi_read.html
  
  {
  "GPS_Enable": 0,
  "SMS_Enable": 1,
  "ATCommands": [
    "AT",
    "AT+CMGF=1",
    "AT+CMTI?",
    "AT+CNMI= 2,2,0,0,0"
  ]
}
  
  
  
  
  
  
  
  
  
  
  
  
  */




#endif // DEVICE_TESTBED_ESP32_LILYGO_SIM7000G









/**
 * Basic version of measurement system
 * GPS will be recorded at 10Hz, and logged to SD card in json format for matlab parsing
 * */
#ifdef DEVICE_TESTBED_OLED_SH1106
  #define DEVICENAME_CTR            "testbed_oled_sh1106"
  #define DEVICENAME_FRIENDLY_CTR   "TestBed SH1106"
  #define DEVICENAME_ROOMHINT_CTR   "Testbed"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH

    #define ENABLE_DEVFEATURE_SETTING_I2C_TO_DEFAULT
    
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 
       * OLED
       * */
      #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
      "\"4\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
      "\"5\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
      #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
      /** 
       * BUILTIN LED
       * */
      "\"2\":\""  D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
  "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";

#endif // DEVICE_TESTBED_OLED_SH1106




#ifdef DEVICE_TESTBED__NEXTION_DISPLAY__GENERIC_WITH_WEBUI__7INCH
  #define DEVICENAME_CTR            "nextion_desk"
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


  DEFINE_PGM_CTR(NEXTION_HMI_CONTROL_MAP)
  R"=====(
  {
    "ObjectNameIDList": {
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


/**
 * esp32 version -- SIM800L
 * */
#ifdef DEVICE_TESTBED_FONA_800L_BASIC
  #define DEVICENAME_CTR          "testbed_fona"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Fona 800L"

  #define USE_MODULE_DRIVERS_FONA_CELLULAR

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_FONA_CELLULAR
      "\"23\":\"" D_GPIO_FUNCTION__FONA_POWER_KEY__CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION__FONA_POWER_STATUS__CTR   "\","
      "\"21\":\"" D_GPIO_FUNCTION__FONA_NETWORK_STATUS__CTR   "\","
      "\"4\":\"" D_GPIO_FUNCTION__FONA_RESET__CTR   "\","
      "\"17\":\"" D_GPIO_FUNCTION__FONA_UART_TX__CTR   "\","
      "\"16\":\"" D_GPIO_FUNCTION__FONA_UART_RX__CTR   "\","
      "\"19\":\"" D_GPIO_FUNCTION__FONA_RING_INDICATOR__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
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
 * esp32 version -- SIM800L
 * */
#ifdef DEVICE_TESTBED_RCSWITCH_NORMAL
  #define DEVICENAME_CTR          "testbed_rcswitch_normal"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Fona 800L"

  #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_DRIVERS_RF433_RCSWITCH

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
      "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif

#ifdef DEVICE_testbed_rcs_ext
  #define DEVICENAME_CTR          "testbed_rcs_ext"
  #define DEVICENAME_FRIENDLY_CTR "Testbed 433MHz RCSwitch Extended"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
    #define ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif


#ifdef DEVICE_TESTBED_FASTBOOT_ESP8266
  #define DEVICENAME_CTR          "testbed_fastboot_esp8266"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Fona 800L"

  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // #define ENABLE_DEVFEATURE_RTC_FASTBOOT_GLOBALTEST_V3 // FI-128
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION // FI-128

  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_CREATE_SSID_AP
  // #define ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  // #define DEBUG_FASTBOOT
  // #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

  /**
   * @brief wEBSERVER ISSUES
   * 
   */


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
      // "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\""
  "}";
  
#endif



/**
 * @brief Getting crashreports to work so I can now start tracking crashes.
 * Start saving the mqtt json response with openhab string item, therefore I can easily go back and track issues.
 * A rule to request item string info from persistance will allow me to recall the data (print into logs?)
 * 
 */
#ifdef DEVICE_TESTBED_CRASHREPORT
  #define DEVICENAME_CTR          "testbed_crashreport" APPEND_ESP_TYPE_MQTT_STRING
  #define DEVICENAME_FRIENDLY_CTR "Testbed CrashReport" APPEND_ESP_TYPE_NAME_STRING

  // #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // // #define ENABLE_DEVFEATURE_RTC_FASTBOOT_GLOBALTEST_V3 // FI-128
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION // FI-128
  // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  // // #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_CREATE_SSID_AP
  // // #define ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  // // #define DEBUG_FASTBOOT
  // // #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING



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
  
#endif


#ifdef DEVICE_TESTBED_SR04
  #define DEVICENAME_CTR          "testbed_sr04"
  #define DEVICENAME_FRIENDLY_CTR "Oil Tank"
  #define DEVICENAME_ROOMHINT_CTR "Outside"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"

  // add db18 dropping from the waterproof box to know extra temp, perhaps add two for backup? (independant pin from tank sensors)

  #define ENABLE_FEATURE_WATCHDOG_TIMER
  #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  #define ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  #define ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #define USE_MODULE_SENSORS_INTERFACE
    #define ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
  #define USE_MODULE_SENSORS_SR04
    // #define USE_AMBIENT_TEMP_SENSOR_FOR_SPEEDOFSOUND
    // #define ENABLE_DEVFEATURE_ULTRASONIC_DURATION_RAW_THRESHOLD_CHECK
    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
    // #define ENABLE_DEVFEATURE_TEMPERATURE_SOUND_OF_SOUND_COMPENSATION
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"      
      #ifdef USE_MODULE_SENSORS_SR04
      "\"21\":\"" D_GPIO_FUNCTION_SR04_ECHO_CTR   "\","
      "\"22\":\"" D_GPIO_FUNCTION_SR04_TRIG_CTR  "\","  
      #endif
      "\"2\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\""
    "},"
    "\"" D_JSON_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  

  #define D_DEVICE_SENSOR_DB18S20_01_NAME "AmbientTank"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS "[40,226,218,149,240,1,60,142]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME "OutsideGarage"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS "[40,255,169,120,53,22,4,240]"

  #define D_DEVICE_SR04_FRIENDLY_NAME_LONG "LevelSensor"
  
  #define D_DEVICE_BUTTON_1_CTR "FurnaceActive"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_JSON_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SR04_FRIENDLY_CTR "\":["
        "\"" D_DEVICE_SR04_FRIENDLY_NAME_LONG "\""
      "]"
    "},"
    "\"" D_JSON_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_FRIENDLY_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ""
      "}"  
    "}"
  "}";

#endif


#ifdef DEVICE_BUCKET_WATER_LEVEL //get capacitive one for each chritmas!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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





#ifdef DEVICE_TESTBED__SETTINGS_STORING
  #define DEVICENAME_CTR          "testbed_settings_storage"
  #define DEVICENAME_FRIENDLY_CTR "Testbed 433MHz RCSwitch Extended"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define DEVICENAME_ROOMHINT_CTR "Example"

  /***********************************
   * SECTION: Core Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE__SETTINGS_STORAGE
  //   #define ENABLE_DEVFEATURE__SETTINGS_STORAGE__SAVE_LOAD_STRUCT
  //   #define ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
  //   #define ENABLE_DEVFEATURE__SETTINGS_STORAGE_ESP_SUPPORT
  //   #define ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

  
  // #define USE_MODULE_DRIVERS_FILESYSTEM
  // #define WLED_ENABLE_FS_EDITOR
  // #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif




#ifdef DEVICE_TESTBED__FILESYSTEM
  #define DEVICENAME_CTR          "testbed_filesystem"
  #define DEVICENAME_FRIENDLY_CTR "Testbed Filesystem"
  #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
  #define DEVICENAME_ROOMHINT_CTR "Example"

  /***********************************
   * SECTION: Core Configs
  ************************************/     
  #define USE_MODULE_DRIVERS_FILESYSTEM
    #define WLED_ENABLE_FS_EDITOR
    #define ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    #define ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define USE_MODULE_NETWORK_WEBSERVER



  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"22\":\"" D_GPIO_FUNCTION__RF_433MHZ_TX__CTR   "\","
      #endif  
      #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
      "\"23\":\"" D_GPIO_FUNCTION__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_FUNCTION_LED1_CTR  "\""
    "},"
    "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif




#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_DEV_TESTBEDS_H

