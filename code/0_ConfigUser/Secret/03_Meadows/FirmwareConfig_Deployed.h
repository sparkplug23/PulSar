#ifndef _CONFIG__FIRMWARE__MEADOWS_DEPLOYED__H
#define _CONFIG__FIRMWARE__MEADOWS_DEPLOYED__H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are unique with lighting
 *  ** Any second versions of these devices should be placed into other files
 *  ** This list should track the ini file
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

//--------------------------------[Enable Device]-------------------------------------

/// KITCHEN //////////////////////////////////////////////////////////////////////////////////
// #define DEVICE_MEADOWS__KITCHEN__ROOM_SENSOR
// #define DEVICE_MEADOWS__KITCHEN__GLASS_BOX
// #define DEVICE_MEADOWS__KITCHEN__GREEN_PLANT
/// LANDING //////////////////////////////////////////////////////////////////////////////////

/// HALLWAY //////////////////////////////////////////////////////////////////////////////////
// #define DEVICE_MEADOWS__HALLWAY__VASE_LIGHT
/// MASTER BEDROOM ///////////////////////////////////////////////////////////////////////////
// #define DEVICE_MEADOWS__MASTER_BEDROOM__AMBIENT_SENSOR
// #define DEVICE_MEADOWS__MASTER_BEDROOM__BEDLIGHT
// #define DEVICE_MEADOWS__BED_ALARM_LIGHT
// #define DEVICE_MEADOWS__ENSUITE_DOOR_FRAME
// #define DEVICE_MEADOWS__HALLWAY__HEATING
/// OFFICE ///////////////////////////////////////////////////////////////////////////////////
#define DEVICE_MEADOWS__OFFICE__WS2815_PANEL_12V
/// LIVINGROOM ///////////////////////////////////////////////////////////////////////////////

/// GARAGE ///////////////////////////////////////////////////////////////////////////////////
// #define DEVICE_MEADOWS__TREADMILL_POWER_MONITOR
/// BATHROOM /////////////////////////////////////////////////////////////////////////////////

/// OUTSIDE //////////////////////////////////////////////////////////////////////////////////
// #define DEVICE_MEADOWS__OUTSIDE__FENCE_LIGHTS



// #define DEVICE_MEADOWS__OFFICE__HVAC_DESK
// #define DEVICE_MEADOWS__LIVINGROOM__HYPERION_LIGHT_SAMSUNG_65INCH
// #define DEVICE_MEADOWS__PRUSA_CLIMATE_CONTROL
// #define DEVICE_MEADOWS__LANDING__GLASS_BOX
// #define DEVICE_MEADOWS__OUTSIDE__OILTANK
// #define DEVICE_MEADOWS__OFFICE__433MHZ_NODE
// #define DEVICE_MEADOWS__BATHROOM__IMMERSION

// #define DEVICE_MEADOWS__OFFICE__SUN_PIXELS_1D
// #define DEVICE_MEADOWS__OFFICE__BLACK_STAND
// #define DEVICE_MEADOWS__OFFICE__GARAGE_TREE

// #define DEVICE_MEADOWS__OFFICE__MONITORS_BACKLIGHT
// #define DEVICE_OFFICE__DESK_LIGHTING
// #define DEVICE_OFFICE__RGBWW_VERTICAL_BAR
// #define DEVICE_MEADOWS__OFFICE__UNDER_DESK

// #define DEVICE_OFFICE__NEXTION_DISPLAY__DESK_3P5INCH
// #define DEVICE_OFFICE__NEXTION_DISPLAY__DESK_10INCH

// #define DEVICE_MEADOWS__ROAMING__REDBOARD_TESTER02
// #define DEVICE_MEADOWS__OFFICE__PEBBLE_ESP32C3_TESTBED
// #define DEVICE_MEADOWS__ROAMING__ADDRESSABLE_HARDWARE_TESTER


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------


/***
 * Name: KitchenSensor
 * LEDs of box
 * Motion detector
 * Later BME under glass for room
*/


/******************************************************************************************
 * Name: LandingSensor
 * LEDs in the box, change from the current white ones
 * Motion detector
 * Even dual motion, towards stairs and towards landing for different type of lighting
 * 
 ******************************************************************************************/


/***
 * Name: FrontWindow
 * Motion sensor towards outside, specifically the front door if possible. May need wired so it physically goes outside (since PIR does not work through glass?)
 * Motion detector
 * 
 * In a box, on the table? probably a strip attached the table under towards the floor.
 * 
 * 
*/


/***
 * Name: BackDoor
 * Motion towards driveway? (though from garage probably better, poe with camera?)
 * Use to trigger movement at the backdoor, internal and external lighting.
 * 
 * 
*/



/***
 * Name: OutdoorFence
 * 205x2 on top of fence
 * Uses red board
 * 
 * 
 * 
*/




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Kitchen ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/***
 * LEDs of box
 * Motion detector
 * Later BME under glass for room
 * 
*/

/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__KITCHEN__GLASS_BOX
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif

 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"
     "\"13\":\""  D_GPIO_PIR_1_CTR "\","
     "\"18\":\"" D_GPIO_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":12,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":50
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        50
      ],
      "ColourPalette":"Warm White",
      "ColourType":3,
      "Effects": {
        "Function":"Candles",
        "Speed":180,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 20
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 100,
    "BrightnessCCT": 0
  }
  )=====";

  
 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Kitchen"

 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "]"
   "}"
 "}";

#endif




/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                         |5V     |USB|     5|
 *                         |GND              6| 
 *                         |3V3              7|
 *                         |4  (fL, BIL)     8| I2C SDA   - Blue LED
 *                         |3                9| I2C SCL
 *                         |2 fL            10| 
 *                    U1RX |1               20| U0TX
 *                    U1TX |0               21| U0RX
 *                         ____________________
 * 
        Pin	Function	ESP Pin	Input/Output	Description
        1	5V	5V	POWER INPUT	5V power input for the board
        2	GND	GND	POWER GROUNT	Ground connection
        3	3V3	3.3V	POWER OUTPUT	3.3V power output
        4	IO0	A0	BIDIRECTIONAL	GPIO, ADC pin, PWM
        5	IO1	A1	BIDIRECTIONAL	GPIO, ADC pin, PWM
        6	IO2	A2	BIDIRECTIONAL	GPIO, ADC pin, PWM
        7	IO3	A3	BIDIRECTIONAL	GPIO, ADC pin, PWM
        8	IO4	A4	BIDIRECTIONAL	GPIO, ADC pin, SCK, PWM
        9	IO5	A5	BIDIRECTIONAL	GPIO, ADC pin, SPI Master In Slave Out, PWM
        10	IO6	MISO	BIDIRECTIONAL	GPIO, SPI Master Out Slave In, PWM
        11	IO7	SS	BIDIRECTIONAL	GPIO, SPI Slave Select, PWM
        12	IO8	SDA	BIDIRECTIONAL	GPIO, I2C Data line, PWM
        13	IO9	SCL	BIDIRECTIONAL	GPIO, I2C Clock line, PWM
        14	IO10	RX	BIDIRECTIONAL	GPIO, PWM
        15	IO21	TX	BIDIRECTIONAL	GPIO, UART Transmit
        16	IO20	RX	BIDIRECTIONAL	GPIO, UART Receive (secondary)

        Hardware serial port, there are two hardware serial ports on the board:
          USB serial port
          ART serial port

        GPIO Matrix pins with one of the following important functions, as described in Section 2.3.3
          Restrictions for GPIOs:–GPIO2,
          GPIO8, GPIO9 : Strapping pins.–GPIO18,
          GPIO19 : USB_C Serial/JTAG interface.–GPIO4,
          GPIO5, GPIO6, GPIO7 : JTAG interface.–GPIO2
          0, GPIO21 : UART0 interface.–GPIO11: The VDD_SPI pin. The power supply pin for flash by default, and can only be reconfigured
          as a GPIO pin if the flash is powered by an external power supply.

  * BH1750 (Good)
  * PIR small
  * Radar
  * BME680
  * maybe some thin leds?


 */
#ifdef DEVICE_MEADOWS__KITCHEN__ROOM_SENSOR
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "consumerunit"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
  

  
// /***********************************
//  * SECTION: System Debug Options
// ************************************/    
// ///////////////////////////////////////////// Enable Logs
// // #define DISABLE_SERIAL
// // #define DISABLE_SERIAL0_CORE
// // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays

// ///////////////////////////////////////////// System Logs
// // #define ENABLE_ADVANCED_DEBUGGING
// // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// // #define ENABLE_DEBUG_FUNCTION_NAMES
// // 
// // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
// // #define ENABLE_DEBUG_LINE_HERE
// // #define ENABLE_DEBUG_LINE_HERE2
// // #define ENABLE_DEBUG_LINE_HERE3
// // #define ENABLE_DEBUG_LINE_HERE_TRACE
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// // #define USE_DEBUG_PRINT
// // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// //new feature to only show logs for a specific module when developing code
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]

#define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

// #define ESP32
// #define CONFIG_IDF_TARGET_ESP32C3
// #define ENABLE_DEVFEATURE_GPIO_PIN_METHOD_MAY_2025


// ///////////////////////////////////////////// Module Logs
// // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// // #define ENABLE_FREERAM_APPENDING_SERIAL

// /***********************************
//  * SECTION: System Configs
// ************************************/    

// #define SETTINGS_HOLDER 1239

// 
// 
// #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//  // until devices can reliably be used without compiling per device
// 
    
// //  /***********************************
// //   * SECTION: Network Configs
// //  ************************************/    

// //  
// //    

// #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

// //  // #define USE_MODULE_SENSORS_INTERFACE  
// //  // #define USE_MODULE_SENSORS_BUTTONS
// //  //   
// //    /**
// //     * @brief 
// //     * Button 1: Single button installs, means {"short":"iter over nice palettes", "long": "iter over 4 brightness levels"}
// //     * Button 2: (Short) Two button installs, this button is iter common effects || (Long) starts a demo and debug mode
// //     **/

// //  /***********************************
// //   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
// //  ************************************/  

// // //  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
#define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
#define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define ENABLE_TEMPLATE_SECTION__SENSORS__BME
// #define ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXELBUS


// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_SPEAKER
// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_BUZZER

// // // #define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED

// // //  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

// #define USE_MODULE_DRIVERS_LEDS  

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

// // //  #define USE_MODULE_SENSORS_INTERFACE
// //  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
// //    #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
// //     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
// //  #endif
// // //  #define USE_MODULE_SENSORS_BUTTONS
// // //    #define ENABLE_DEVFATURE_BUTTON__REMOVE_MQTT_BUTTONS
// // //    #define SOC_TOUCH_VERSION_1

// //   #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// //    #define USE_MODULE_SENSORS_SR04
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
// //   #endif

#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BH1750
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define USE_MODULE_SENSORS_BME
    
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
  #define USE_MODULE_SENSORS__RADAR_HLK_LD2410
  #define ENABLE_FEATURE_HLK_LD2410__USE_SERIAL_CHUNK_MODE
#endif

// //   #define ENABLE_ADVANCED_DEBUGGING
// //   #define ENABLE_DEBUG_FUNCTION_NAMES
// //   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// //   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// //   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// //   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
// //   #define ENABLE_SERIAL_FLUSH
// //   #define DEBUG_FASTBOOT
// //   #define ENABLE_DEBUG_LINE_HERE


// // //  #define USE_MODULE_DRIVERS_LEDS  

// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// //   #define USE_MODULE_SENSORS__TOF_VL53L0X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   #define VL53L0X_LONG_RANGE
// //  #endif
// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// //   #define USE_MODULE_SENSORS__TOF_VL53L1X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   // #define USE_SENSORS_TOFVL_AVERAGING_DATA
// //  #endif

// //  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING

// //  #define ENABLE_DEVFEATURE_SENSORS__TOF_BOTH_VL53_ACTIVE_ON_SHARED_ADD29

/***********************************
 * SECTION: Lighting Configs
************************************/  

#ifdef ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXELBUS
  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":8,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":9
      }
    ],
    "Segment0": {
      "Name":"Door Edge",
      "PixelRange": [
        0,
        9
      ],
      "ColourPalette":"Rainbow 16",
      "Effects": {
        "Function":"Static",
        "Speed":255,
        "Intensity":0,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 100
    }
    "BrightnessRGB": 5
  }
  )=====";
#endif

//  /***********************************
//   * SECTION: Display Configs
//  ************************************/  
//  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED
//    #define USE_MODULE_DISPLAYS_INTERFACE
//    #define USE_MODULE_DISPLAYS_OLED_SH1106
//      #define SHOW_SPLASH
//  #endif

// /***********************************
//  * SECTION: Energy Configs
// ************************************/  

// /***********************************
//  * SECTION: Controller Configs
// ************************************/  

// // #define USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

// /***********************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
//  * SECTION: Module/GPIO Configs
// ************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
    #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    "\"10\":\"" D_GPIO_I2C_SDA_CTR   "\"," //instead of 9
    "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\","    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"4\":\""  D_GPIO_PIR_CTR "1" "\","
    "\"7\":\""  D_GPIO_PIR_CTR "2" "\","
    #endif
    #ifdef USE_MODULE_SENSORS_SR04
    "\"4\":\"" D_GPIO_SR04_ECHO_CTR   "\","
    "\"2\":\"" D_GPIO_SR04_TRIG_CTR  "\","  
    #endif 
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
    "\"35\":\""  D_GPIO_PIR_2_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
    "\"5\":\""  D_GPIO_PIR_3_INV_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    "\"6\":\""  D_GPIO_LD2410_TX_CTR "\","
    "\"5\":\""  D_GPIO_LD2410_RX_CTR "\""
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
    "\"34\":\""  D_GPIO_PIR_1_INV_CTR "\""
    #endif
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

//  #define ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
//  #define ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD


// /***********************************
//  * SECTION: TEMPLATE: Names
// ************************************/    

// #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIRLarge"
// #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "RADAR 3p18GHz"
// #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "PIRSmall"


// #define D_DEVICE_SENSOR_CLIMATE "PIRLarge"




// #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
// #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

#define D_DEVICE_SENSOR_BH1750_NAME "Kitchen"
#define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Kitchen"


#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_PIR_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "1\","
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "2\","
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "3\""
    "],"
    "\"" D_MODULE_SENSORS__TOF_VL53L0X__CTR "\":["
      "\"" "TOF_VL53L0X" "\""
    "],"
    "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
      "\"" "TOF_VL53L1X" "\""
    "],"
    "\"" D_MODULE_SENSORS__RADAR_HLK_LD2410__CTR "\":["
      "\"" "Kitchen" "\""
    "],"
    "\"" D_MODULE_SENSORS_SR04_CTR "\":["
      "\"" "SRO4" "\""
    "],"
    "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "],"
    "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
      "\"" "WallRed" "\","
      "\"" "WallBlue" "\","
      "\"" "DoorAlert" "\""
    "],"
    "\"" D_MODULE_DRIVERS_LEDS_CTR "\":["
      "\"" "WallRed" "\","
      "\"" "WallBlue" "\","
      "\"" "StatusLED" "\""
    "],"
    "\"" D_MODULE_SENSORS_BME_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "]"
  "},"
  "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
  "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
"}";


#endif



/**
 * @brief 
 * Using bad antenna supermini
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                         |5V     |USB|     5|
 *                         |GND              6| 
 *                         |3V3              7|
 *                         |4  (fL, BIL)     8| I2C SDA   - Blue LED
 *                         |3                9| I2C SCL
 *                         |2 fL            10| 
 *                    U1RX |1               20| U0TX
 *                    U1TX |0               21| U0RX
 *                         ____________________
 * 
        Pin	Function	ESP Pin	Input/Output	Description
        1	5V	5V	POWER INPUT	5V power input for the board
        2	GND	GND	POWER GROUNT	Ground connection
        3	3V3	3.3V	POWER OUTPUT	3.3V power output
        4	IO0	A0	BIDIRECTIONAL	GPIO, ADC pin, PWM
        5	IO1	A1	BIDIRECTIONAL	GPIO, ADC pin, PWM
        6	IO2	A2	BIDIRECTIONAL	GPIO, ADC pin, PWM
        7	IO3	A3	BIDIRECTIONAL	GPIO, ADC pin, PWM
        8	IO4	A4	BIDIRECTIONAL	GPIO, ADC pin, SCK, PWM
        9	IO5	A5	BIDIRECTIONAL	GPIO, ADC pin, SPI Master In Slave Out, PWM
        10	IO6	MISO	BIDIRECTIONAL	GPIO, SPI Master Out Slave In, PWM
        11	IO7	SS	BIDIRECTIONAL	GPIO, SPI Slave Select, PWM
        12	IO8	SDA	BIDIRECTIONAL	GPIO, I2C Data line, PWM
        13	IO9	SCL	BIDIRECTIONAL	GPIO, I2C Clock line, PWM
        14	IO10	RX	BIDIRECTIONAL	GPIO, PWM
        15	IO21	TX	BIDIRECTIONAL	GPIO, UART Transmit
        16	IO20	RX	BIDIRECTIONAL	GPIO, UART Receive (secondary)

        Hardware serial port, there are two hardware serial ports on the board:
          USB serial port
          ART serial port

        GPIO Matrix pins with one of the following important functions, as described in Section 2.3.3
          Restrictions for GPIOs:–GPIO2,
          GPIO8, GPIO9 : Strapping pins.–GPIO18,
          GPIO19 : USB_C Serial/JTAG interface.–GPIO4,
          GPIO5, GPIO6, GPIO7 : JTAG interface.–GPIO2
          0, GPIO21 : UART0 interface.–GPIO11: The VDD_SPI pin. The power supply pin for flash by default, and can only be reconfigured
          as a GPIO pin if the flash is powered by an external power supply. */
#ifdef DEVICE_MEADOWS__KITCHEN__GREEN_PLANT
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
    
  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define ENABLE_DEBUGFEATURE_SENSORS__SPLASH_I2C_SCAN

 /***********************************
  * SECTION: Network Configs
 ************************************/    

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

  #define USE_MODULE_SENSORS_INTERFACE

  #define USE_MODULE_SENSORS_MOISTURE

/***********************************
 * SECTION: Lighting Configs
************************************/  


/***********************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
 * SECTION: Module/GPIO Configs
************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"4\":\""  D_GPIO_MOISTURE_ANALOG_CTR "\"" 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
/***********************************
 * SECTION: TEMPLATE: Names
************************************/    

#define D_DEVICE_SENSOR_MOISTURE_ADC_NAME "KitchenTree"

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_MOISTURE_ADC_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOISTURE_ADC_NAME "\""
    "]"
  "}"
"}";


#endif





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Utility ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * @brief 
 * Using bad antenna supermini
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                         |5V     |USB|     5|
 *                         |GND              6| 
 *                         |3V3              7|
 *                         |4  (fL, BIL)     8| I2C SDA   - Blue LED
 *                         |3                9| I2C SCL
 *                         |2 fL            10| 
 *                    U1RX |1               20| U0TX
 *                    U1TX |0               21| U0RX
 *                         ____________________
 * 
        Pin	Function	ESP Pin	Input/Output	Description
        1	5V	5V	POWER INPUT	5V power input for the board
        2	GND	GND	POWER GROUNT	Ground connection
        3	3V3	3.3V	POWER OUTPUT	3.3V power output
        4	IO0	A0	BIDIRECTIONAL	GPIO, ADC pin, PWM
        5	IO1	A1	BIDIRECTIONAL	GPIO, ADC pin, PWM
        6	IO2	A2	BIDIRECTIONAL	GPIO, ADC pin, PWM
        7	IO3	A3	BIDIRECTIONAL	GPIO, ADC pin, PWM
        8	IO4	A4	BIDIRECTIONAL	GPIO, ADC pin, SCK, PWM
        9	IO5	A5	BIDIRECTIONAL	GPIO, ADC pin, SPI Master In Slave Out, PWM
        10	IO6	MISO	BIDIRECTIONAL	GPIO, SPI Master Out Slave In, PWM
        11	IO7	SS	BIDIRECTIONAL	GPIO, SPI Slave Select, PWM
        12	IO8	SDA	BIDIRECTIONAL	GPIO, I2C Data line, PWM
        13	IO9	SCL	BIDIRECTIONAL	GPIO, I2C Clock line, PWM
        14	IO10	RX	BIDIRECTIONAL	GPIO, PWM
        15	IO21	TX	BIDIRECTIONAL	GPIO, UART Transmit
        16	IO20	RX	BIDIRECTIONAL	GPIO, UART Receive (secondary)

        Hardware serial port, there are two hardware serial ports on the board:
          USB serial port
          ART serial port

        GPIO Matrix pins with one of the following important functions, as described in Section 2.3.3
          Restrictions for GPIOs:–GPIO2,
          GPIO8, GPIO9 : Strapping pins.–GPIO18,
          GPIO19 : USB_C Serial/JTAG interface.–GPIO4,
          GPIO5, GPIO6, GPIO7 : JTAG interface.–GPIO2
          0, GPIO21 : UART0 interface.–GPIO11: The VDD_SPI pin. The power supply pin for flash by default, and can only be reconfigured
          as a GPIO pin if the flash is powered by an external power supply. */
#ifdef DEVICE_MEADOWS__UTILITY__ROOM_SENSOR
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
    
  /***********************************
   * SECTION: System Configs
  ************************************/    

 /***********************************
  * SECTION: Network Configs
 ************************************/    

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  

#define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define ENABLE_TEMPLATE_SECTION__SENSORS__BME

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BH1750
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define USE_MODULE_SENSORS_BME    
#endif
#define USE_MODULE_SENSORS_PIR

/***********************************
 * SECTION: Lighting Configs
************************************/  


/***********************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
 * SECTION: Module/GPIO Configs
************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
    #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
    "\"9\":\"" D_GPIO_I2C_SDA_CTR   "\","
    "\"10\":\"" D_GPIO_I2C_SCL_CTR   "\","    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"4\":\""  D_GPIO_PIR_1_CTR "\""
    #endif
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
 * SECTION: TEMPLATE: Names
************************************/    

#define D_DEVICE_SENSOR_BH1750_NAME "Utility"
#define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Utility"

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_PIR_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
    "],"
    "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "],"
    "\"" D_MODULE_SENSORS_BME_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "]"
  "}"
"}";


#endif


/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__UTILITY__CLEARVASE_LIGHT
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif

 #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE



 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"
     "\"15\":\""  D_GPIO_PIR_1_CTR "\","
     "\"18\":\"" D_GPIO_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "PixelRange": [
          0,
          100
        ],
        "ColourPalette":"Warm White",
        "ColourType":3,
        "Effects": {
          "Function":"Candles",
          "Speed":180,
          "Intensity":85,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";

  
 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Utility"

 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "]"
   "}"
 "}";

#endif





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Landing ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__LANDING__GLASS_BOX
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif

 #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE



 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"
     "\"15\":\""  D_GPIO_PIR_1_CTR "\","
     "\"18\":\"" D_GPIO_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":"Warm White",
      "ColourType":3,
      "Effects": {
        "Function":"Candles",
        "Speed":180,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 20
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 100
  }
  )=====";

  
 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Landing"

 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "]"
   "}"
 "}";

#endif




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Hallway ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__HALLWAY__VASE_LIGHT
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif

 #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE



 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"
     "\"15\":\""  D_GPIO_PIR_1_CTR "\","
     "\"18\":\"" D_GPIO_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "PixelRange": [
          0,
          100
        ],
        "ColourPalette":"Warm White",
        "ColourType":3,
        "Effects": {
          "Function":"Candles",
          "Speed":180,
          "Intensity":85,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";

  
 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Hallway"

 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "]"
   "}"
 "}";

#endif





/**
 * @brief Seeed Studio XIAO ESP32S3 (and ESP32S3 Sense) pin map (quick reference)
 *
 * Notes:
 *  - BOOT (GPIO0): hold LOW at reset to enter ROM bootloader (download mode).
 *  - USER_LED: GPIO21 (on-board user LED).
 *  - D4/D5 are I2C SDA/SCL (GPIO5/GPIO6).
 *  - D6/D7 are UART TX/RX (GPIO43/GPIO44).
 *  - SPI: SCK=GPIO7 (D8), MISO=GPIO8 (D9), MOSI=GPIO10 (D10).
 *  - For the Sense variant, the digital mic uses GPIO42 (CLK) and GPIO41 (DATA).
 *
 * ┌───────────────────────────────────────────────────────────────┐
 * │  XIAO Pin   | Function(s)        | ESP32-S3 GPIO | Notes       │
 * ├───────────────────────────────────────────────────────────────┤
 * │  5V         | VBUS               | —             | USB 5V rail  │
 * │  GND        | GND                | —             | Ground       │
 * │  3V3        | 3V3_OUT            | —             | Regulated 3V │
 * │  D0 (A0)    | ADC, TOUCH1        | GPIO1         | GPIO/ADC     │
 * │  D1 (A1)    | ADC, TOUCH2        | GPIO2         | GPIO/ADC     │
 * │  D2 (A2)    | ADC, TOUCH3        | GPIO3         | GPIO/ADC     │
 * │  D3 (A3)    | ADC, TOUCH4        | GPIO4         | GPIO/ADC     │
 * │  D4 (A4)    | ADC, SDA, TOUCH5   | GPIO5         | I2C SDA      │
 * │  D5 (A5)    | ADC, SCL, TOUCH6   | GPIO6         | I2C SCL      │
 * │  D6         | TX                 | GPIO43        | UART TX      │
 * │  D7         | RX                 | GPIO44        | UART RX      │
 * │  D8 (A8)    | ADC, SCK, TOUCH7   | GPIO7         | SPI SCK      │
 * │  D9 (A9)    | ADC, MISO, TOUCH8  | GPIO8         | SPI MISO     │
 * │  D10 (A10)  | ADC, MOSI, TOUCH9  | GPIO10        | SPI MOSI     │
 * │  D11 (A11)  | ADC, TOUCH12       | GPIO42        | GPIO/ADC     │
 * │  D12 (A12)  | ADC, TOUCH13       | GPIO41        | GPIO/ADC     │
 * │  RESET      | EN                 | EN            | Chip enable  │
 * │  BOOT       | Boot strap         | GPIO0         | Bootloader   │
 * │  USER_LED   | User LED           | GPIO21        | On-board LED │
 * │  CHARGE_LED | Charge indicator   | —             | Board LED    │
 * │  U.FL       | Antenna (LNA_IN)   | LNA_IN         | UFL antenna  │
 * └───────────────────────────────────────────────────────────────┘
 *
 * Source: Seeed XIAO ESP32S3 “Pin Map” table (D0..D12, BOOT, USER_LED, etc.).
 */
#ifdef DEVICE_MEADOWS__HALLWAY__BLUE_VASE_LIGHT
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
  #define ENABLE_FEATURE_LIGHTING__REALTIME_MQTT_SETPIXEL

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif


  /***********************************
   * SECTION: Lighting Configs
  ************************************/   

  #define ENABLE_DEVFEATURE_NEOBUS__RMT_AS_PRIMARY
  #define ENABLE_PIXELBUS_BUSMETHODS__RMT_8_CHANNELS_THEN_I2S_DUAL_CHANNELS

  // #define CONFIG_IDF_TARGET_ESP32C3

 
  //     {
  //       "Pin":18,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":2000,
  //       "Length":1000
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":3000,
  //       "Length":1000
  //     }
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Pin":13,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":12,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      }
    ],
    "Segments":[
      {
        "PixelRange":[0,200],
        "ColourPalette":"Rainbow",
        "ColourType":3,
        "Effects": {
          "Function":"Gradient",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";
  #define BUSCONFIG_MAX_PINS_FOR_PARALLEL_I2S 1000
  #define MAX_LED_MEMORY 64000*5
  #define ENABLE_DEVFEATURE_LIGHTS__SEGMENT_MATCHBUS

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
    "\"48\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



#endif


/**
 * @brief 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                         |5V     |USB|     5|
 *                         |GND              6| 
 *                         |3V3              7|
 *                         |4  (fL, BIL)     8| I2C SDA   - Blue LED
 *                         |3                9| I2C SCL
 *                         |2 fL            10| 
 *                    U1RX |1               20| U0TX
 *                    U1TX |0               21| U0RX
 *                         ____________________
 *  */
#ifdef DEVICE_MEADOWS__HALLWAY__ROOM_SENSOR
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
    
  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define ENABLE_DEBUGFEATURE_SENSORS__SPLASH_I2C_SCAN

 /***********************************
  * SECTION: Network Configs
 ************************************/    

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  

#define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define ENABLE_TEMPLATE_SECTION__SENSORS__BME

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

//  #define ENABLE_DEBUGFEATURE_WIFI__SUPERMINI_REDUCE_WIFI_BAD_ANTENNA_HARDWARE

#define USE_MODULE_SENSORS_INTERFACE
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define USE_MODULE_SENSORS_BH1750
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define USE_MODULE_SENSORS_BME    
#endif
#define USE_MODULE_SENSORS_PIR

/***********************************
 * SECTION: Lighting Configs
************************************/  


/***********************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
 * SECTION: Module/GPIO Configs
************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
    #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
    "\"10\":\"" D_GPIO_I2C_SDA_CTR   "\","
    "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\","    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"4\":\""  D_GPIO_PIR_1_CTR "\""
    #endif
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
 * SECTION: TEMPLATE: Names
************************************/    

#define D_DEVICE_SENSOR_BH1750_NAME "Hallway"
#define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Hallway"

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_PIR_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
    "],"
    "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "],"
    "\"" D_MODULE_SENSORS_BME_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "]"
  "}"
"}";


#endif





#ifdef DEVICE_MEADOWS__HALLWAY__CONSUMER_UNIT_POWER
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

  // #define DEVICENAME_CTR          "treadmill_power_monitor"
  // #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  // #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define MQTT_HOST   "192.168.1.70" // primary
  //   #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  
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


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  // #define USE_MODULE_SENSORS_INTERFACE  
  // #define USE_MODULE_SENSORS_SWITCHES

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SH1106
  //   #define SHOW_SPLASH

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
//  #define USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"23\":\"" D_GPIO_SWT1_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","   
      #endif
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  #define D_DEVICE_SENSOR_PZEM004T_0_ADDRESS "1"

  #define D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "MainFeed"
  
  
  #define D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR   D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
  "}";



  
#endif



#ifdef DEVICE_MEADOWS__HALLWAY__HEATING
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_01__sensors_nextion"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testbeds"
  
  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  /***********************************
   * SECTION: System Configs
  ************************************/     

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_FULL
  
  /***********************************
   * SECTION: Sensor Configs
  ************************************/  
  
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
   #define USE_MODULE_DRIVERS_INTERFACE
   #define USE_MODULE_DRIVERS_RELAY
   #define USE_MODULE_DRIVERS_LEDS

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Energy Configs
  ************************************/  

  /***********************************
   * SECTION: Controller Configs
  ************************************/  
 
  #define USE_MODULE_CONTROLLER_HVAC
  #define HEATING_DEVICE_MAX 2

  /***********************************
   * SECTION: GPIO Template
  ************************************/  
  
  #define ENABLE_DEBUG_POINT_MODULE_TEMPLATE_BOOT_SPLASH
  #define USE_MODULE_TEMPLATE__OVERRIDE
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"       
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"17\":\"" D_GPIO_REL_CTR "1" "\","     // Left
      "\"16\":\"" D_GPIO_REL_CTR "2" "\","     // Right
      #endif
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"23\":\"" D_GPIO_LED1_CTR "\","
      #endif
      "\"0\":\"" D_GPIO_KEY1_INV_CTR   "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /**
   * @brief Drivers and Sensors for HVAC zones
   **/
  #define D_DEVICE_HEATER_0_NAME "Radiators"
  #define D_DEVICE_HEATER_1_NAME "Boiler"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_INTERFACE_CTR "\":["
        "\"" "Internal" "\""
      "],"  
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\""
      "]"
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" "None" "\","
        "\"" "None" "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "}"
  "}";

#endif




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Master Bedroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/



#ifdef DEVICE_MEADOWS__ENSUITE_DOOR_FRAME
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__final__snow_silver"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #ifndef MQTT_HOST
  #define MQTT_HOST   "192.168.3.70"
  #endif
    #define MQTT_PORT     1883

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE

DEFINE_PGM_CTR(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE)
R"=====(
  {
    "Segment0": {
      "ColourPalette":"Warm White",
      "ColourType":3,
      "Effects": {
        "Function":"Static",
        "Speed":0,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 1000
      },
      "Override":{
        "Animation":{
          "TimeMs":60000
        }
      }
    },
    "BrightnessRGB": 10
  }
)=====";
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":50
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        50
      ],
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Static",
        "Speed":127,
        "Intensity":127,
        "Decimate":0,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 0
  }
  )=====";
  
  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME          "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME  "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{},"
    "\"" D_BASE          "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT      "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif //



// supermini pro has ws2812 on pin8
#ifdef DEVICE_MEADOWS__MASTER_BEDROOM__AMBIENT_SENSOR
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "consumerunit"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883


  //  #define CONFIG_IDF_TARGET_ESP32C3=1
  //  #define ESP32

  
// /***********************************
//  * SECTION: System Debug Options
// ************************************/    
// ///////////////////////////////////////////// Enable Logs
// // #define DISABLE_SERIAL
// // #define DISABLE_SERIAL0_CORE
// 
// // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays

// ///////////////////////////////////////////// System Logs
// // #define ENABLE_ADVANCED_DEBUGGING
// // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// // #define ENABLE_DEBUG_FUNCTION_NAMES
// // 
// // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
// // #define ENABLE_DEBUG_LINE_HERE
// // #define ENABLE_DEBUG_LINE_HERE2
// // #define ENABLE_DEBUG_LINE_HERE3
// // #define ENABLE_DEBUG_LINE_HERE_TRACE
// // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
// // #define USE_DEBUG_PRINT
// // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

// //new feature to only show logs for a specific module when developing code
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
//  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]

#define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES

// #define ESP32
// #define CONFIG_IDF_TARGET_ESP32C3
// #define ENABLE_DEVFEATURE_GPIO_PIN_METHOD_MAY_2025


// ///////////////////////////////////////////// Module Logs
// // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
// // #define ENABLE_FREERAM_APPENDING_SERIAL

// /***********************************
//  * SECTION: System Configs
// ************************************/    

// #define SETTINGS_HOLDER 1239

// 
// 
// #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
//  // until devices can reliably be used without compiling per device
// 
    
// //  /***********************************
// //   * SECTION: Network Configs
// //  ************************************/    

// //  
// //    

// #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

// //  // #define USE_MODULE_SENSORS_INTERFACE  
// //  // #define USE_MODULE_SENSORS_BUTTONS
// //  //   
// //    /**
// //     * @brief 
// //     * Button 1: Single button installs, means {"short":"iter over nice palettes", "long": "iter over 4 brightness levels"}
// //     * Button 2: (Short) Two button installs, this button is iter common effects || (Long) starts a demo and debug mode
// //     **/

// //  /***********************************
// //   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
// //  ************************************/  

// // //  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
// // // #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
// #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
// #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define ENABLE_TEMPLATE_SECTION__SENSORS__BME
// #define ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXELBUS


// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_SPEAKER
// // // #define ENABLE_TEMPLATE_SECTION__DRIVERS__AUDIO_BUZZER

// // // #define ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED

// // //  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY
// // //  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM

// #define USE_MODULE_DRIVERS_LEDS  

// //  /***********************************
// //   * SECTION: Sensor Configs
// //  ************************************/  

 #define USE_MODULE_SENSORS_INTERFACE
// //  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
// //    #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
// //     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
// //  #endif
// // //  #define USE_MODULE_SENSORS_BUTTONS
// // //    #define ENABLE_DEVFATURE_BUTTON__REMOVE_MQTT_BUTTONS
// // //    #define SOC_TOUCH_VERSION_1

// //   #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__ULTRASONIC
// //    #define USE_MODULE_SENSORS_SR04
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_EMA
// //    #define ENABLE_DEVFEATURE_SR04_FILTERING_DEMA
// //   #endif

#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_BH1750
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define USE_MODULE_SENSORS_BME
    
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
  #define USE_MODULE_SENSORS__RADAR_HLK_LD2410
  #define ENABLE_FEATURE_HLK_LD2410__USE_SERIAL_CHUNK_MODE
#endif

// //   #define ENABLE_ADVANCED_DEBUGGING
// //   #define ENABLE_DEBUG_FUNCTION_NAMES
// //   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
// //   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
// //   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
// //   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
// //   #define ENABLE_SERIAL_FLUSH
// //   #define DEBUG_FASTBOOT
// //   #define ENABLE_DEBUG_LINE_HERE


// // //  #define USE_MODULE_DRIVERS_LEDS  

// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L0X
// //   #define USE_MODULE_SENSORS__TOF_VL53L0X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   #define VL53L0X_LONG_RANGE
// //  #endif
// //  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
// //   #define USE_MODULE_SENSORS__TOF_VL53L1X
// //   #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
// //   // #define USE_SENSORS_TOFVL_AVERAGING_DATA
// //  #endif

// //  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING

// //  #define ENABLE_DEVFEATURE_SENSORS__TOF_BOTH_VL53_ACTIVE_ON_SHARED_ADD29

/***********************************
 * SECTION: Lighting Configs
************************************/  

#ifdef ENABLE_TEMPLATE_SECTION__LIGHTS__NEOPIXELBUS
  #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025_NO_GPIO
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":8,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":9
      }
    ],
    "Segment0": {
      "Name":"Door Edge",
      "PixelRange": [
        0,
        9
      ],
      "ColourPalette":"Rainbow 16",
      "Effects": {
        "Function":"Static",
        "Speed":255,
        "Intensity":0,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 100
    }
    "BrightnessRGB": 5
  }
  )=====";
#endif

//  /***********************************
//   * SECTION: Display Configs
//  ************************************/  
//  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAYS__OLED
//    #define USE_MODULE_DISPLAYS_INTERFACE
//    #define USE_MODULE_DISPLAYS_OLED_SH1106
//      #define SHOW_SPLASH
//  #endif

// /***********************************
//  * SECTION: Energy Configs
// ************************************/  

// /***********************************
//  * SECTION: Controller Configs
// ************************************/  

// // #define USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS

// /***********************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
//  * SECTION: Module/GPIO Configs
// ************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
  // supermini pro has ws2812 on pin8
   //  "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
   //  "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
    #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
    "\"10\":\"" D_GPIO_I2C_SDA_CTR   "\","
    "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\","    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"4\":\""  D_GPIO_PIR_1_CTR "\""
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
    "\"33\":\""  D_GPIO__TOF_VL53L0X_XSHUT1__CTR "\","
   //  "\"33\":\""  D_GPIO_UNUSED_FORCED_HIGH_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
   //  "\"26\":\""  D_GPIO_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS__TOF_VL53L1X
     // "\"26\":\""  D_GPIO__TOF_VL53L1X_XSHUT1__CTR "\"," // turned off only for testing new sensor interface, needed for dual TOF use
   //  "\"33\":\""  D_GPIO_UNUSED_FORCED_LOW_CTR "\"," // Connected to XSHUT but not wanted. HIGH for remain enabled
    #endif
    #ifdef USE_MODULE_SENSORS_SR04
    "\"4\":\"" D_GPIO_SR04_ECHO_CTR   "\","
    "\"2\":\"" D_GPIO_SR04_TRIG_CTR  "\","  
    #endif 
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ
    "\"35\":\""  D_GPIO_PIR_2_INV_CTR "\","
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_SMALL
    "\"5\":\""  D_GPIO_PIR_3_INV_CTR "\","
    #endif
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    // "\"20\":\""  D_GPIO__HLK_LD2410_TX__CTR "\","
    // "\"21\":\""  D_GPIO__HLK_LD2410_RX__CTR "\","
    "\"6\":\""  D_GPIO__HLK_LD2410_TX__CTR "\","
    "\"5\":\""  D_GPIO__HLK_LD2410_RX__CTR "\""
    #endif
    #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__PIR_LARGE
    "\"34\":\""  D_GPIO_PIR_1_INV_CTR "\","
    #endif
   //  #ifdef USE_MODULE_SENSORS__TOF_VL53L0X
   //  "\"27\":\""  D_GPIO__TOF_VL53L1X_XSHUT__CTR "\","
   //  #endif
    #ifdef USE_MODULE_SENSORS_BUTTONS
   //  "\"18\":\"" D_GPIO_KEY1_INV_CTR  "\","
   //  "\"19\":\"" D_GPIO_KEY2_INV_CTR  "\","
   //  "\"33\":\"" D_GPIO_KEY3_INV_CTR  "\","
    #endif
    
   //  "\"4\":\"" D_GPIO_LED1_CTR  "\","
   //  "\"5\":\"" D_GPIO_LED2_CTR  "\","
    // "\"8\":\"" D_GPIO_LED1_CTR  "\""
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

//  #define ENABLE_DEVFEATURE_MQTT__PUBLUSH_TASMOTA_METHODS
//  #define ENABLE_FEATURE_BUTTON__FACTORY_RESET_WITH_LONG_HOLD


// /***********************************
//  * SECTION: TEMPLATE: Names
// ************************************/    

// #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "PIRLarge"
// #define D_DEVICE_SENSOR_MOTION1_FRIENDLY_NAME_LONG "RADAR 3p18GHz"
// #define D_DEVICE_SENSOR_MOTION2_FRIENDLY_NAME_LONG "PIRSmall"


// #define D_DEVICE_SENSOR_CLIMATE "PIRLarge"




// #define D_DEVICE_SENSOR_BME_280_NAME "BME280"
// #define D_DEVICE_SENSOR_BME_680_NAME "BME680"

#define D_DEVICE_SENSOR_BH1750_NAME "MasterBedroom"
#define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "MasterBedroom"


#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_PIR_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "1\","
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "2\","
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "3\""
    "],"
    "\"" D_MODULE_SENSORS__TOF_VL53L0X__CTR "\":["
      "\"" "TOF_VL53L0X" "\""
    "],"
    "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
      "\"" "TOF_VL53L1X" "\""
    "],"
    "\"" D_MODULE_SENSORS_SR04_CTR "\":["
      "\"" "SRO4" "\""
    "],"
    "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "],"
    "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
      "\"" "WallRed" "\","
      "\"" "WallBlue" "\","
      "\"" "DoorAlert" "\""
    "],"
    "\"" D_MODULE_DRIVERS_LEDS_CTR "\":["
      "\"" "WallRed" "\","
      "\"" "WallBlue" "\","
      "\"" "StatusLED" "\""
    "],"
    "\"" D_MODULE_SENSORS_BME_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "]"
  "},"
  "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":60}," 
  "\"MQTT_Interface_Priority\":{\"" D_MODULE_ENERGY_INTERFACE_CTR "\":1}" // Each interface will have ability to reduce its subclass mqtt "ifchanged" rate
"}";


#endif


#ifdef DEVICE_MEADOWS__MASTER_BEDROOM__BEDLIGHT
#ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__final__snow_silver"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #ifndef MQTT_HOST
  #define MQTT_HOST   "192.168.3.70"
  #endif
    #define MQTT_PORT     1883
    

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BUTTONS
  
  /***********************************
   * SECTION: System Configs
  ************************************/     

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_PIR
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BUTTONS
  #define USE_MODULE_SENSORS_BUTTONS   
  #endif 

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
  #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
  

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRBWC",
        "BusType":"WS2805_RGBWW",
        "Start":0,
        "Length":39
      },
      {
        "Pin":4,
        "ColourOrder":"GRBWC",
        "BusType":"WS2805_RGBWW",
        "Start":39,
        "Length":27,
        "Reversed":1
      },
      {
        "Pin":18,
        "ColourOrder":"GRBWC",
        "BusType":"WS2805_RGBWW",
        "Start":66,
        "Length":19
      },
      {
        "Name":"Headboard",
        "Pin":19,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":85,
        "Length":100
      }
    ],    
    "Segments":[
      {
        "Name":"Under Edge",
        "PixelRange": [
          0,
          66
        ],
        "ColourType":5,
        "ColourPalette":"Candy",
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":100
        },
        "Effects": {
          "Function":"Static",
          "RateMs": 1000,
          "Speed":127
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },
      {
        "Name":"Under Centre",
        "PixelRange": [
          67,
          85
        ],
        "ColourType":5,
        "ColourPalette":"Warm White",
        "SegColour0": {
          "Hue": 25,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":100
        },
        "Effects": {
          "Function":"Static",
          "RateMs": 1000,
          "Speed":127
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      },
      {
        "Name":"Headboard",
        "PixelRange": [
          86,
          186
        ],
        "ColourPalette":"Sunset",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":20,
          "RateMs": 20
        },
      }
    ],
    "BrightnessRGB":0,
    "BrightnessCCT":0
  }
  )=====";

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"    
    #ifdef USE_MODULE_SENSORS_BUTTONS
    "\"19\":\"" D_GPIO_KEY_INV_CTR "1"  "\","
    "\"21\":\"" D_GPIO_KEY_INV_CTR "2" "\","
    "\"22\":\"" D_GPIO_KEY_INV_CTR "3" "\","
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"13\":\""  D_GPIO_PIR_CTR "1" "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "BedPIR"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"" "WallRed" "\","
        "\"" "WallBlue" "\","
        "\"" "DoorAlert" "\""
      "]"
    "}"
  "}";


#endif


#ifdef DEVICE_MEADOWS__BED_ALARM_LIGHT
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        100
      ],
      "ColourPalette":"Cold White",
      "Effects": {
        "Function":"Solid",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 0
  }
  )=====";
 
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";  

#endif // END DEVICE



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Office ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/***
 * LEDs behind displays, eventually to be "dynamic wallpaper python windows"
 * Display 1: 32 inch main (WRGB)
 * Display 2: 24 inch vertical (RGB)
 * Later a python script "another space name" software like my custom firmware, but runs on a computer 24/7 doing tasks
 *  - Task 1: Check the windows wallpaper stored in memory, and use it to create the wallpaper and send via UDP.
*/
#ifdef DEVICE_MEADOWS__OFFICE__MONITORS_BACKLIGHT
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "xmas24__final__snow_silver"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#ifndef MQTT_HOST
#define MQTT_HOST   "192.168.1.1"
#endif
  #define MQTT_PORT     1883


 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"
     "\"18\":\"" D_GPIO_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

//   #ifdef   DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE
  
//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = RgbcctColor(255,175,0,255,0);//HsbColor(tkr_iLight->HUE_N2F(240),tkr_iLight->SatN2F(100),tkr_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = RgbcctColor(0,0,0,100,0);//HsbColor(tkr_iLight->HUE_N2F(0),tkr_iLight->SatN2F(100),tkr_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(tkr_iLight->HUE_N2F(340),tkr_iLight->SatN2F(100),tkr_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(tkr_iLight->HUE_N2F(120),tkr_iLight->SatN2F(100),tkr_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 42;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 44; // 2 extra pixels on centre inlay
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 23;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 23;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 0;
//   ambilightsettings.screens[SCREEN_CENTRE].right.blend_between_sides_gradient_percentage = 0;


//   #endif // DEVICE_RGB_COMPUTER_SCREEN_DELL_P3222QE

//   #ifdef DEVICE_RGB_COMPUTER_SCREEN_DELL_U2515H
//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(tkr_iLight->HUE_N2F(20),tkr_iLight->SatN2F(95),tkr_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour = HsbColor(tkr_iLight->HUE_N2F(8),tkr_iLight->SatN2F(95),tkr_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].left.colour   = HsbColor(tkr_iLight->HUE_N2F(240),tkr_iLight->SatN2F(100),tkr_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].right.colour  = HsbColor(tkr_iLight->HUE_N2F(330),tkr_iLight->SatN2F(100),tkr_iLight->BrtN2F(100));
//   ambilightsettings.screens[SCREEN_CENTRE].top.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.size = 33;
//   ambilightsettings.screens[SCREEN_CENTRE].left.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].right.size = 19;
//   ambilightsettings.screens[SCREEN_CENTRE].left.blend_between_sides_gradient_percentage = 50;

//   ambilightsettings.screens[SCREEN_CENTRE].top.colour    = HsbColor(tkr_iLight->HUE_N2F(20),tkr_iLight->SatN2F(95),tkr_iLight->BrtN2F(0));
//   ambilightsettings.screens[SCREEN_CENTRE].bottom.colour    = HsbColor(tkr_iLight->HUE_N2F(20),tkr_iLight->SatN2F(95),tkr_iLight->BrtN2F(50));


// lets make the new openhab IP at meadows be 192.168.1.80 to stop conflict with 192.168.3.70 at whitehall when using VPN
// Tower here can be put back to 192.168.1.65

  /***
   * Description: 32
   * Starts bottom left
   * Goes along bottom edge from left to right, ACW
   * Bottom 44 (extra 2 pixels on centre inlay)
   * Right  23
   * Top    42
   * Left   23
   * BRTL: BRTL 
   * 
   * 
   * Description: 25
   * Starts bottom right(when in horizontal orientation)
   * Goes along right edge from bottom to top, ACW
   * Right  19
   * Top    33
   * Left   19
   * Bottom 33 (extra 2 pixels on centre inlay)
   * To make these easier, since vertical display (with right edge downwards) they are already in agreement 
   * BRTL: RTLB (rotated 90 degrees)
   * 
   */


  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":132,
        "Reverse":1
      },
      {
        "Pin":4,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":132,
        "Length":104
      }
    ],
    "Segments":[
      {
        "Name":"32 Inch Horizontal",
        "PixelRange": [
          0,
          132
        ],
        "ColourPalette":0,
        "SegColour0": {
          "Hue": 15,
          "Sat":100,
          "BrightnessRGB":100
        },
        "ColourType":4,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":20,
          "RateMs": 20
        },
        "BrightnessRGB": 100,
        "BrightnessCCT":0
      },
      {
        "Name":"27 Inch Vertical",
        "PixelRange": [
          132,
          236
        ],
        "ColourPalette":"Sunset",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":20,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";

#endif





#ifdef DEVICE_MEADOWS__ROAMING__ADDRESSABLE_HARDWARE_TESTER
/**
 * @brief Device with all physical connectors, to allow testing of all the different types of lights and sensors
 * Can be used to calibrate power usage of different types of lights
 * 
 * Button to be added between ground/GPIO16 to run test sequences. 
 * * SINGLE press: All Red, G, B, Orange, Cyan, Purple, White, Warm white. Each for 1 second. 
 * * Long press: cycle through static, with grouping of 1,10,25,100 of RGBO (r with P for 25) for easy identifying. Or, what about counter effect? (or another)
 * 
 */
{
  "ColourPalette":0,
  "SegColour0":{"RGBWC":[255,255,0,0,0]},
  "Effects": {
    "Function":"DB Pixel Range",
    "Param0": 0,
    "Param1": 199
  }
}



  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

  // #define FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES

  #define PIXEL_LIGHT_SENSOR__DIGITAL_PIN 16
  #define PIXEL_LIGHT_SENSOR__DIGITAL_ACTIVE_LOW

  #define ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET
  #define ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM


  // #define ENABLE_DEBUG_LINE_HERE_TRACE

  #define ENABLE_EFFECT_DESCRIPTIONS

  #define ENABLE_DEBUG_FEATURE_MQTT__LIGHTS_INTERFACE__POWER_PROFILES


// ======================= Example PROGMEM template =======================
// Put this in your config header (mirrors your DEFINE_PGM_CTR style)
#define USE_STANDBY_TEMPLATE
#define LIGHTING_TEMPLATE__PRESET_STANDBY_MODE_VERSION 2
// compile-time gate
// #define LIGHTING_STANDBY_TEMPLATE_ID  5

#define ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS__ANIMATOR  // the sub module enable


DEFINE_PGM_CTR(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE)
R"=====(
  {
    "Segment0": {
      "ColourPalette":"Warm White",
      "ColourType":3,
      "Effects": {
        "Function":"Static",
        "Speed":0,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 1000
      },
      "Override":{
        "Animation":{
          "TimeMs":60000
        }
      }
    },
    "BrightnessRGB": 10
  }
)=====";
// DEFINE_PGM_CTR(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE)
// R"=====(
//   {
//     "Segment0": {
//       "ColourPalette":"Warm White",
//       "ColourType":3,
//       "Effects": {
//         "Function":"Candles",
//         "Speed":180,
//         "Intensity":85,
//         "Grouping":1,
//         "RateMs": 20
//       },
//       "BrightnessRGB": 100
//     },
//     "BrightnessRGB": 100
//   }
// )=====";


  // #define ENABLE_DEBUG_FEATURE__SORTING_EFFECTS_PROMOTE_ALPHA

//   #define ENABLE_ADVANCED_DEBUGGING
//   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//  #define ENABLE_DEBUG_FUNCTION_NAMES
//   #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
//   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
//   #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
//   #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
//   #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

#define ENABLE_DEBUGFEATURE_LIGHTING__SPLASH_FPS
// #define ENABLE_DEBUGFEATURE_LIGHTING__EFFECT_LOOP_TIME_SERIAL

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[      
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":200
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":200,
        "Length":200
      },
      {
        "Pin":18,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":400,
        "Length":200
      },
      {
        "Pin":19,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":600,
        "Length":200
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange": [
          0,
          800
        ],
        "ColourPalette":"RGPBY",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 25
  }
  )=====";
  
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":2,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":5
  //     },
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":100,
  //       "Length":100
  //     },
  //     {
  //       "Pin":18,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":300,
  //       "Length":100
  //     }
  //   ],
  //   "Segments":[
  //     {
  //       "Name":"Bus 1",
  //       "PixelRange": [
  //         0,
  //         400
  //       ],
  //       "ColourPalette":"RGPBY",
  //       "ColourType":3,
  //       "Effects": {
  //         "Function":"Solid",
  //         "Speed":255,
  //         "Intensity":127,
  //         "Grouping":1,
  //         "RateMs": 20
  //       },
  //       "BrightnessRGB": 100
  //     }
  //   ],
  //   "BrightnessRGB": 25
  // }
  // )=====";

#endif




#ifdef DEVICE_MEADOWS__OFFICE__PEBBLE_ESP32C3_TESTBED

// because of all the complex timing we do, there is probably not a way, but I was just to ask if we should be sharing code between effects?

// inwaves, seems to be chasing
// twinkle/flash, is just constant twinkle back and forth
// flashing, in chasing/flash, appears to be twinkle/flash

// most recent observations with real world lights, lets make sure we are doing this 

// inwaves, never turns off any lights, they just dim
// sequential, always has two lights on. Due to real world physics, there is about a 300ms cool down of the previous light turning off
// slo glo, I think we have it well now. 
// chasing/flash, actually chases, then flashes in one direction, then the chasing then flash are in the opposite direction. So it flips each cycle. We will want to do this instead of random. 
// twinkle/flash, how no reversing.


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

  // #define FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES

  #define PIXEL_LIGHT_SENSOR__DIGITAL_PIN 16
  #define PIXEL_LIGHT_SENSOR__DIGITAL_ACTIVE_LOW

  #define ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET
  #define ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM


  // #define ENABLE_DEBUG_LINE_HERE_TRACE

  #define ENABLE_EFFECT_DESCRIPTIONS


// ======================= Example PROGMEM template =======================
// Put this in your config header (mirrors your DEFINE_PGM_CTR style)
#define USE_STANDBY_TEMPLATE
#define LIGHTING_TEMPLATE__PRESET_STANDBY_MODE_VERSION 2
// compile-time gate
// #define LIGHTING_STANDBY_TEMPLATE_ID  5

#define ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS__ANIMATOR  // the sub module enable



DEFINE_PGM_CTR(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE)
R"=====(
  {
    "Segment0": {
      "ColourPalette":"Warm White",
      "ColourType":3,
      "Effects": {
        "Function":"Static",
        "Speed":0,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 1000
      },
      "Override":{
        "Animation":{
          "TimeMs":60000
        }
      }
    },
    "BrightnessRGB": 10
  }
)=====";
// DEFINE_PGM_CTR(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE)
// R"=====(
//   {
//     "Segment0": {
//       "ColourPalette":"Warm White",
//       "ColourType":3,
//       "Effects": {
//         "Function":"Candles",
//         "Speed":180,
//         "Intensity":85,
//         "Grouping":1,
//         "RateMs": 20
//       },
//       "BrightnessRGB": 100
//     },
//     "BrightnessRGB": 100
//   }
// )=====";


  // #define ENABLE_DEBUG_FEATURE__SORTING_EFFECTS_PROMOTE_ALPHA

//   #define ENABLE_ADVANCED_DEBUGGING
//   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//  #define ENABLE_DEBUG_FUNCTION_NAMES
//   #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
//   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
//   #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
//   #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
//   #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"BGR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange": [
          0,
          100
        ],
        "ColourPalette":"RGPBY",
        "ColourType":3,
        "Effects": {
          "Function":"Sweep Random",
          "Speed":70,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 25
  }
  )=====";
  
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":2,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":5
  //     },
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":100,
  //       "Length":100
  //     },
  //     {
  //       "Pin":18,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":200,
  //       "Length":100
  //     },
  //     {
  //       "Pin":19,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":300,
  //       "Length":100
  //     }
  //   ],
  //   "Segments":[
  //     {
  //       "Name":"Bus 1",
  //       "PixelRange": [
  //         0,
  //         400
  //       ],
  //       "ColourPalette":"RGPBY",
  //       "ColourType":3,
  //       "Effects": {
  //         "Function":"Solid",
  //         "Speed":255,
  //         "Intensity":127,
  //         "Grouping":1,
  //         "RateMs": 20
  //       },
  //       "BrightnessRGB": 100
  //     }
  //   ],
  //   "BrightnessRGB": 25
  // }
  // )=====";

#endif









// 01 is at whitehall, with all the connectors
#ifdef DEVICE_MEADOWS__ROAMING__REDBOARD_TESTER02

// because of all the complex timing we do, there is probably not a way, but I was just to ask if we should be sharing code between effects?

// inwaves, seems to be chasing
// twinkle/flash, is just constant twinkle back and forth
// flashing, in chasing/flash, appears to be twinkle/flash

// most recent observations with real world lights, lets make sure we are doing this 

// inwaves, never turns off any lights, they just dim
// sequential, always has two lights on. Due to real world physics, there is about a 300ms cool down of the previous light turning off
// slo glo, I think we have it well now. 
// chasing/flash, actually chases, then flashes in one direction, then the chasing then flash are in the opposite direction. So it flips each cycle. We will want to do this instead of random. 
// twinkle/flash, how no reversing.

//   #define ENABLE_ADVANCED_DEBUGGING
//   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//  #define ENABLE_DEBUG_FUNCTION_NAMES
//   #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
//   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
//   #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
//   #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
//   #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION


  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__BETA
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

  // #define FIRMWARE_DEFAULT__ENABLE_SOLAR_PALETTES

  #define PIXEL_LIGHT_SENSOR__DIGITAL_PIN 16
  #define PIXEL_LIGHT_SENSOR__DIGITAL_ACTIVE_LOW

  #define ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET
  #define ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM


  // #define ENABLE_DEBUG_LINE_HERE_TRACE

  #define ENABLE_EFFECT_DESCRIPTIONS


// ======================= Example PROGMEM template =======================
// Put this in your config header (mirrors your DEFINE_PGM_CTR style)
#define USE_STANDBY_TEMPLATE
#define LIGHTING_TEMPLATE__PRESET_STANDBY_MODE_VERSION 2
// compile-time gate
// #define LIGHTING_STANDBY_TEMPLATE_ID  5

#define 

DEFINE_PGM_CTR(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE)
R"=====(
  {
    "Segment0": {
      "ColourPalette":"Warm White",
      "ColourType":3,
      "Effects": {
        "Function":"Static",
        "Speed":0,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 1000
      },
      "Override":{
        "Animation":{
          "TimeMs":60000
        }
      }
    },
    "BrightnessRGB": 10
  }
)=====";
// DEFINE_PGM_CTR(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE)
// R"=====(
//   {
//     "Segment0": {
//       "ColourPalette":"Warm White",
//       "ColourType":3,
//       "Effects": {
//         "Function":"Candles",
//         "Speed":180,
//         "Intensity":85,
//         "Grouping":1,
//         "RateMs": 20
//       },
//       "BrightnessRGB": 100
//     },
//     "BrightnessRGB": 100
//   }
// )=====";


  // #define ENABLE_DEBUG_FEATURE__SORTING_EFFECTS_PROMOTE_ALPHA

//   #define ENABLE_ADVANCED_DEBUGGING
//   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//  #define ENABLE_DEBUG_FUNCTION_NAMES
//   #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
//   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
//   #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
//   #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
//   #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":250
      },
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":250,
        "Length":250
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange": [
          0,
          500
        ],
        "ColourPalette":"RGPBY",
        "ColourType":3,
        "Effects": {
          "Function":"Sweep Random",
          "Speed":255,
          "Intensity":127,
          "Grouping":20,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";

#endif




#ifdef DEVICE_MEADOWS__OFFICE__GARAGE_TREE

// #define ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING



// #define ENABLE_DEBUG_LINE_HERE_TRACE


//   #define ENABLE_ADVANCED_DEBUGGING
//   #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//  #define ENABLE_DEBUG_FUNCTION_NAMES
//   #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
//   #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
//   #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
//   #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
//   #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":200
      }
    ],
    "Segments":[
      {
        "Name":"Tree",
        "PixelRange": [
          0,
          200
        ],
        "ColourPalette":"Snowy 02",
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 25
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";

#endif












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
#ifdef DEVICE_MEADOWS__OFFICE__HVAC_DESK
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "xmas24__final__snow_silver"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #ifndef MQTT_HOST
  #define MQTT_HOST   "192.168.3.70"
  #endif
    #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1241
  #define ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
  #define USE_MODULE_CORE_FILESYSTEM
  #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CORE__MQTT

  #define ESP32

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE 
  
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 600
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  // #define ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
  // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  // #define ENABLE_DEBUG_LINE_HERE_MILLIS
  
  // #define ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
  // #define ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING

  // #define ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING

  // #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 200

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  //   #define ENABLE_DEBUG_LINE_HERE4
  //   #define ENABLE_WAIT_WITH_PRINT_TICK
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // 
  // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT
  // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT


  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE

  #define ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT

  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE


  // #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // #define ENABLE_DEVFEATURE_PINS__GPIO_UI_VIEWER

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BUTTONS
  #define ENABLE_TEMPLATE_SECTION__LIGHTING
  #define ENABLE_TEMPLATE_SECTION__ENERGY
  #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  #define ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  #define ENABLE_TEMPLATE_SECTION__DRIVERS__RELAYS
  #define ENABLE_TEMPLATE_SECTION__DISPLAY_OLED1106
  #define ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
  #define ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  /***********************************
   * SECTION: System Configs
  ************************************/     

  /***********************************
   * SECTION: Storage Configs
  ************************************/    

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS

  // #define ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  // 

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS
  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC

  // I should add new "purely for debugging" "serialise" data struct. So this will be a new way to take important data from the module data struct that will all be saved in binary, but instead 
  // include functions that "pretty print" them for easier comparing. Will use lots of memory, so debug only.

  #define ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

  // #define ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
  // #define ENABLE_DEVFEATURE_PERIODIC_SETTINGS_SAVING__EVERY_MINUTE

  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  
  


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
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
    #define USE_MODULE_SENSORS_SUN_TRACKING     
      #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
        #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
      #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
      #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
        // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

      // #define ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY

      // #define USE_MODULE_SENSORS_SUN_TRACKING__DETAILED_MQTT_INFO_UNIX


  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
    #define USE_MODULE_SENSORS_BH1750
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define USE_MODULE_SENSORS_PIR
    #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
  // #define USE_MODULE_SENSORS_SWITCHES
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BUTTONS
  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_BUTTONS  
  #endif

  // #define ENABLE_DEVFEATURE_MQTT__SUPPRESS_SUBMODULE_IFCHANGED_WHEN_UNIFIED_IS_PREFFERRED

  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_HARDCODED_ADD
  // #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_OILTANK_ADD
    
  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__DISPLAY_OLED1106) || defined(ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION) 
  #define USE_MODULE_DISPLAYS_INTERFACE
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_OLED1106
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
    #define USE_MODULE_DISPLAYS_NEXTION
    #define NEXTION_DEFAULT_PAGE_NUMBER 6  
      
      //  // change to be code option later
      #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
      #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE
      //                     
      #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
      #define ENABLE_DEVFEATURE_NEXTION_WEBUI
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
      #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
       
      
      #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK
        #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
        
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
        },
        "MQTTSubscribe":[
          "openhab_broadcast/nextion/group/hvac_home",
          "openhab_broadcast/nextion/group/hvac_desk_power"
        ]
      }
      )=====";

  #endif // USE_MODULE_DISPLAYS_NEXTION
  /***********************************
   * SECTION: Driver Configs
  ************************************/  
        
  #ifdef ENABLE_TEMPLATE_SECTION__DRIVERS__RELAYS
   #define USE_MODULE_DRIVERS_INTERFACE
   #define USE_MODULE_DRIVERS_RELAY
  #endif

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #ifdef ENABLE_TEMPLATE_SECTION__LIGHTING

  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__LATEST_LIGHTING_JANUARY_2025

  

  // #define USE_LIGHTING_TEMPLATE__1_DEVICES
  #define USE_LIGHTING_TEMPLATE__n_DEVICES
  // #define USE_LIGHTING_TEMPLATE__n_DEVICES_TRY_ENABLING_2_SEGMENTS_AGAIN
  // #define USE_LIGHTING_TEMPLATE__3_DEVICES
  // #define USE_LIGHTING_TEMPLATE__3_DEVICES_WITHOUT_4PIXELS
  
    // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // 21 = on PCB (4 pixels)
    // 32 = external

    /****
     * 
     * 
     * ,
        {
          "Pin":25,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":192,
          "Length":4
        }
     */
    #ifdef USE_LIGHTING_TEMPLATE__1_DEVICES
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":23,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          144
        ],
        "ColourPalette":"Snowy 02",
        "Effects": {
          "Function":"Randomise Gradient",
          "Speed":229,
          "RateMs": 25
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";
    #endif // USE_LIGHTING_TEMPLATE__1_DEVICES

    #ifdef USE_LIGHTING_TEMPLATE__n_DEVICES
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[        
        {
          "Pin":23,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":44
        },
        {
          "Pin":25,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":192,
          "Length":100
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          292
        ],
        "ColourPalette":"Snowy 02",
        "Effects": {
          "Function":"Randomise Gradient",
          "Speed":1,
          "Intensity":127,
          "RateMs": 25
        },
        "BrightnessRGB": 100
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";
    #endif // USE_LIGHTING_TEMPLATE__n_DEVICES

    
    #ifdef USE_LIGHTING_TEMPLATE__n_DEVICES_TRY_ENABLING_2_SEGMENTS_AGAIN
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[        
        {
          "Pin":23,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":44
        },
        {
          "Pin":25,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":192,
          "Length":100
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,
          100
        ],
        "ColourPalette":"Snowy 02",
        "Effects": {
          "Function":"Randomise Gradient",
          "Speed":229,
          "Intensity":0,
          "RateMs": 25
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "PixelRange": [
          100,
          292
        ],
        "ColourPalette":"Snowy 02",
        "Effects": {
          "Function":"Static",
          "Speed":229,
          "Intensity":0,
          "RateMs": 25
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";
    #endif // USE_LIGHTING_TEMPLATE__n_DEVICES

    #ifdef USE_LIGHTING_TEMPLATE__3_DEVICES
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":23,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":44
        },
        {
          "Pin":25,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":192,
          "Length":100
        }
      ],
      "Segment0": {
        "Name":"Solar Sky Metal",
        "PixelRange": [
          0,
          144
        ],
        "Effects": {
          "Function":"Randomise Gradient",
          "Speed":127,
          "Intensity":127,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 25
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "Name":"Relay Status",
        "PixelRange": [
          144,
          148
        ],
        "ColourPalette":"Christmas RGPBO",
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment2": {
        "Name":"Leg Vertical",
        "PixelRange": [
          148,
          184
        ],
        "ColourPalette":"Rainbow 16",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment3": {
        "Name":"Leg Floor",
        "PixelRange": [
          184,
          196
        ],
        "ColourPalette":"Snowy 02",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":100,
          "BrightnessRGB":5
        },
        "Effects": {
          "Function":"Static",
          "Speed":127,
          "Intensity":255,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 1000
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";
    #endif // USE_LIGHTING_TEMPLATE__3_DEVICES


    
    #ifdef USE_LIGHTING_TEMPLATE__3_DEVICES_WITHOUT_4PIXELS
    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":23,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":4,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":4
        },
        {
          "Pin":32,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":148,
          "Length":44
        },
        {
          "Pin":25,
          "ColourOrder":"GRBC",
          "BusType":"SK6812_RGBW",
          "Start":192,
          "Length":100
        }
      ],
      "Segment0": {
        "Name":"Solar Sky Metal",
        "PixelRange": [
          0,
          100
        ],
        "Effects": {
          "Function":"Randomise Gradient",
          "Speed":127,
          "Intensity":127,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 25
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "Segment1": {
        "Name":"Solar Sky Metal",
        "PixelRange": [
          100,
          292
        ],
        "Effects": {
          "Function":"Randomise Gradient",
          "Speed":127,
          "Intensity":127,
          "Decimate":0,
          "Grouping":1,
          "RateMs": 25
        },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    }
    )=====";
    #endif // USE_LIGHTING_TEMPLATE__3_DEVICES
    
    // // 13, 18, 19, 22, 23, 25, 26, 27       USED
    // // 33, 32, 21, 17, 16, 15*, 14*, 5*, 4, NOTUSED
    // // 21 = on PCB (4 pixels)
    // // 32 = external
    // #define USE_LIGHTING_TEMPLATE
    // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    // R"=====(
    // {
    //   "BusConfig":[
    //     {
    //       "Pin":32,
    //       "ColourOrder":"GRBC",
    //       "BusType":"SK6812_RGBW",
    //       "Start":0,
    //       "Length":144
    //     },
    //     {
    //       "Pin":21,
    //       "ColourOrder":"GRBC",
    //       "BusType":"SK6812_RGBW",
    //       "Start":144,
    //       "Length":4
    //     },
    //     {
    //       "Pin":4,
    //       "ColourOrder":"GRBC",
    //       "BusType":"SK6812_RGBW",
    //       "Start":148,
    //       "Length":44
    //     }
    //   ],
    //   "Segment0": {
    //     "Name":"Solar Sky Metal",
    //     "PixelRange": [
    //       0,
    //       144
    //     ],
    //     "ColourPalette":"Candy",
    //     "PaletteMappingValues":[10,15,20],
    //     "SegColour0": {
    //       "Hue": 0,
    //       "Sat":100,
    //       "BrightnessRGB":5
    //     },
    //     "Effects": {
    //       "Function":"Static",
    //       "Speed":127,
    //       "Intensity":255,
    //       "Decimate":0,
    //       "Grouping":1,
    //       "RateMs": 1000
    //     },
    //     "BrightnessRGB": 100,
    //     "BrightnessCCT": 0
    //   },
    //   "Segment1": {
    //     "Name":"Relay Status",
    //     "PixelRange": [
    //       144,
    //       148
    //     ],
    //     "ColourPalette":"Christmas RGPBO",
    //     "PaletteMappingValues":[10,15,20],
    //     "SegColour0": {
    //       "Hue": 0,
    //       "Sat":100,
    //       "BrightnessRGB":5
    //     },
    //     "Effects": {
    //       "Function":"Static",
    //       "Speed":127,
    //       "Intensity":255,
    //       "Decimate":0,
    //       "Grouping":1,
    //       "RateMs": 1000
    //     },
    //     "BrightnessRGB": 100,
    //     "BrightnessCCT": 0
    //   },
    //   "Segment2": {
    //     "Name":"Leg Vertical",
    //     "PixelRange": [
    //       148,
    //       184
    //     ],
    //     "ColourPalette":"Rainbow 16",
    //     "PaletteMappingValues":[10,15,20],
    //     "SegColour0": {
    //       "Hue": 0,
    //       "Sat":100,
    //       "BrightnessRGB":5
    //     },
    //     "Effects": {
    //       "Function":"Static",
    //       "Speed":127,
    //       "Intensity":255,
    //       "Decimate":0,
    //       "Grouping":1,
    //       "RateMs": 1000
    //     },
    //     "BrightnessRGB": 100,
    //     "BrightnessCCT": 0
    //   },
    //   "Segment3": {
    //     "Name":"Leg Floor",
    //     "PixelRange": [
    //       184,
    //       192
    //     ],
    //     "ColourPalette":"Snowy 02",
    //     "PaletteMappingValues":[10,15,20],
    //     "SegColour0": {
    //       "Hue": 0,
    //       "Sat":100,
    //       "BrightnessRGB":5
    //     },
    //     "Effects": {
    //       "Function":"Static",
    //       "Speed":127,
    //       "Intensity":255,
    //       "Decimate":0,
    //       "Grouping":1,
    //       "RateMs": 1000
    //     },
    //     "BrightnessRGB": 100,
    //     "BrightnessCCT": 0
    //   },
    //   "BrightnessRGB": 100,
    //   "BrightnessCCT": 0
    // }
    // )=====";

  #endif // ENABLE_TEMPLATE_SECTION__LIGHTING

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

  #ifdef ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC

  #define USE_MODULE_CONTROLLER_HVAC
     #define HEATING_DEVICE_MAX 4
     
     #define ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

    #ifdef USE_LIGHTING_TEMPLATE
      // #define USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
      // #define USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
        #define ENABLE_CONTROLLERS__RELAY_STATE_LEDSTRIP__SEGMENT_INDEX   1
    #endif // USE_LIGHTING_TEMPLATE
  //  #define USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
    #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
   #define USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED
    #endif
  #endif // ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC
 

   // add new controller custom that instead shows the other sensors as PZEM is on nextion
   /**
    * @brief 
    * BME680 Temp
    * BME280 Temp
    * Lux
    * LED current
    * 
    * Display 2, show all 4 db18s20. So every 5 seconds, flip between 1 and 2
    * 
    */

   //   // Make all the water tank sensors be "remote sensors", then internally they will let me do the colour bar in the same way with IDs

  /***********************************
   * SECTION: MQTT Template Test Loading
  ************************************/  

  // #define USE_MQTT_TEMPLATE
  // DEFINE_PGM_CTR(MQTT_TEMPLATE) 
  // "{"
  //   "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
  //   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  //   "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  //   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  // "}";

  /***********************************
   * SECTION: GPIO Template
  ************************************/  

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"13\":\"" D_GPIO_REL1_INV_CTR  "\","
      "\"27\":\"" D_GPIO_REL2_INV_CTR    "\","
      "\"26\":\"" D_GPIO_REL3_INV_CTR      "\","
      "\"14\":\"" D_GPIO_REL4_INV_CTR      "\"," //pins need sety on L
      #endif
      #ifdef USE_MODULE_ENERGY_PZEM004T_V3
      "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"33\":\"" D_GPIO_DS18X20_1_CTR "\"," // DS_DB - 3 pin
      #endif    
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219)
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","   
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"18\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"19\":\"" D_GPIO_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"15\":\""  D_GPIO_PIR_1_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"0\":\"" D_GPIO_KEY1_INV_CTR  "\","
      #endif  
      // "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // unused NEO pin
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 15 - PIR
      // 2
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
  
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Desk"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":4"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"" "ResetGPIO0" "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_DHT_CTR "\":["
        "\"" D_DEVICE_SENSOR_DHT_0_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "],"  
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_280_NAME "\","
        "\"" D_DEVICE_SENSOR_BME_680_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_INA219_CTR "\":["
        "\"" D_DEVICE_SENSOR_CURRENT "\""
      "],"
      "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_1_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_2_FRIENDLY_NAME_CTR "\","
        "\"" D_DRIVER_ENERGY_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_1_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_2_FRIENDLY_NAME_CTR "\","
        "\"" D_SENSOR_PZEM004T_3_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\","
        "\"" D_DEVICE_HEATER_1_NAME "\","
        "\"" D_DEVICE_HEATER_2_NAME "\","
        "\"" D_DEVICE_HEATER_3_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Downstairs
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "},"  
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
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
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_1_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_2_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "},"
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_3_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/hvac_home\","
      "\"openhab_broadcast/nextion/group/hvac_desk_power\""
    "]"
  "}";



  
#endif




/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__OFFICE__DOORFRAME
  // #ifndef DEVICENAME_CTR
  // #define DEVICENAME_CTR          "template_name"
  // #endif
  // #ifndef DEVICENAME_FRIENDLY_CTR
  // #define DEVICENAME_FRIENDLY_CTR "Template Name"
  // #endif
  // #ifndef DEVICENAME_DESCRIPTION_CTR
  // #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  // #endif
  // #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  // #define MQTT_HOST   "192.168.3.70"
  //   
  //   #define MQTT_PORT     1883

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif

 #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE

 #define USE_MODULE_SENSORS_SUN_TRACKING     
 #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
   #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
 #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
 #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
 #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
   // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

 #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
 // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS2

 #define USE_MODULE_SENSORS_SUN_TRACKING


 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

//  #define USE_MODULE_TEMPLATE
//  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//  "{"
//    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
//    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//    "\"" D_GPIO_NUMBER "\":{"
//      "\"13\":\""  D_GPIO_PIR_1_CTR "\","
//      "\"18\":\"" D_GPIO_LED1_CTR  "\""
//    "},"
//    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//  "}";

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBW",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":144
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        144
      ],
      "ColourPalette":"Warm White",
      "ColourType":3,
      "Effects": {
        "Function":"Static",
        "Speed":255,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 20
      },
      "BrightnessRGB": 100,
      "BrightnessCCT": 0
    },
    "BrightnessRGB": 15,
    "BrightnessCCT": 0
  }
  )=====";

  
 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Office"

 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "]"
   "}"
 "}";

#endif




#ifdef DEVICE_OFFICE__RGBWW_VERTICAL_BAR
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
    

  /***********************************
   * SECTION: System Debug Options
  ************************************/  

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  /***********************************
   * SECTION: System Configs
  ************************************/     

  /***********************************
   * SECTION: Storage Configs
  ************************************/    


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
   * SECTION: Lighting Configs
  ************************************/    

  #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE_DEBUG

  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
  #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE

  #define ENABLE_ANTIALIAS_WITH_RGBWW

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
        "ColourOrder":"GRBWC",
        "BusType":"WS2805_RGBWW",
        "Start":0,
        "Length":20
      }
    ],    
    "Segments":[
      {
        "PixelRange": [
          0,
          20
        ],
        "ColourPalette":0,
        "SegColour0": {
          "Hue": 0,
          "Sat": 100,
          "BrightnessRGB": 100,
          "BrightnessCCT": 100,
          "CCT_TempPercentage":100
        },
        "Effects": {
          "Function":"Solid",
          "RateMs": 1000,
          "ColourType":5,
          "Speed":255
        },
        "BrightnessRGB":100,
        "BrightnessCCT":100
      }
    ],
    "BrightnessRGB":100,
    "BrightnessCCT":100
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
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"    
    #ifdef USE_MODULE_SENSORS_BUTTONS
    "\"19\":\"" D_GPIO_KEY1_INV_CTR  "\","
    "\"21\":\"" D_GPIO_KEY2_INV_CTR  "\","
    "\"22\":\"" D_GPIO_KEY3_INV_CTR  "\","
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"13\":\""  D_GPIO_PIR_1_CTR "\""
    #endif
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "BedPIR"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "],"
      "\"" D_MODULE_SENSORS_BUTTONS_CTR "\":["
        "\"" "WallRed" "\","
        "\"" "WallBlue" "\","
        "\"" "DoorAlert" "\""
      "]"
    "}"
  "}";


#endif



/**
 * @brief 
 * 
 * For under the desk, with 12v leds (dual output)
 * Radar and PIR towards my seat
 * 3d print box, and screw to desk
 * Maybe TOF?
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                         |15 (fL)            13|
 *                         |2  (fL, BIL)  (fH) 12| 
 *                         |4             (fH) 14|
 *               RADAR TX2 |RX2/17             27| 
 *               RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *                         |19                 32| TOF0INT
 *                         |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| 
 *                         |TX0              ' VN| 
 *                         |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__OFFICE__UNDER_DESK
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  #define ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
  #define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
   #define USE_MODULE_SENSORS_INTERFACE
   #define USE_MODULE_SENSORS_PIR
    //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
 #endif

  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_24GHZ
    #define USE_MODULE_SENSORS__RADAR_HLK_LD2410
    #define ENABLE_FEATURE_HLK_LD2410__USE_SERIAL_CHUNK_MODE
  #endif

 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
  #define USE_MODULE_SENSORS__TOF_VL53L1X
  #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
  #define VL53L1X_DISTANCE_MODE Short
 #endif

 #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE

 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"
     "\"18\":\""  D_GPIO_PIR_1_CTR "\","
     "\"19\":\""  D_GPIO_PIR_2_CTR "\","
     "\"23\":\""  D_GPIO_PIR_3_CTR "\","
    #ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
    "\"16\":\""  D_GPIO__HLK_LD2410_TX__CTR "\","
    "\"17\":\""  D_GPIO__HLK_LD2410_RX__CTR "\""
    #endif
    #if defined(USE_MODULE_SENSORS__TOF_VL53L1X)
    "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","   
    "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
    #endif
     "\"2\":\"" D_GPIO_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";

  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      },
      {
        "Pin":12,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":100,
        "Length":100
      },
      {
        "Pin":14,
        "ColourOrder":"GRBWC",
        "BusType":"WS2805_RGBWW",
        "Start":200,
        "Length":100
      }
    ],
    "Segments":[
      {
        "Name":"Under Desk",
        "PixelRange": [
          0,
          200
        ],
        "ColourPalette":"Warm White",
        "ColourType":3,
        "Effects": {
          "Function":"Solid",
          "Speed":180,
          "Intensity":85,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      },
      {
        "Name":"Metal Pole",
        "PixelRange": [
          200,
          300
        ],
        "ColourPalette":"Cold White",
        "ColourType":3,
        "Effects": {
          "Function":"Solid",
          "Speed":180,
          "Intensity":85,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 100
  }
  )=====";

  
 #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Desk"

 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS_PIR_CTR "\":["
       "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
     "]"
   "}"
 "}";

#endif



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Livingroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


#ifdef DEVICE_MEADOWS__LIVINGROOM__HYPERION_LIGHT_SAMSUNG_65INCH
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883



  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  //   #define ENABLE_DEBUG_LINE_HERE4
  //   #define ENABLE_WAIT_WITH_PRINT_TICK
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // 
  // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT
  // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT


  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  // #define ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
 
  // #define ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  // New way to start the save into memory periodically, and then recover if available on boot. Init phase of full system.
  // #define ENABLE_FILESYSTEM__MODULES_CORE__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_CORE__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_SENSORS__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_SENSORS__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__RESTORE_ON_BOOT
  

  /***********************************
   * SECTION: System Configs
  ************************************/     

  // #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging

  #define USE_MODULE_CORE_FILESYSTEM
  // //   // #define ENABLE_FEATURE_SETTINGS_STORAGE__ENABLED_SAVING_BEFORE_OTA
    
  // 
  // 

  // #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  //  // until devices can reliably be used without compiling per device

  // 

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

  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__JUNE2025__NO_MODULE_GPIO
 
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_FUNCTION "\":{},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define USE_LIGHTING_TEMPLATE

  
  
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"GRBC",
        "BusType":"SK6812_RGBW",
        "Start":0,
        "Length":73
      },
      {
        "Pin":2,
        "ColourOrder":"GRBC",
        "BusType":"SK6812_RGBW",
        "Start":73,
        "Length":40,
        "Reversed":1
      },
      {
        "Pin":18,
        "ColourOrder":"GRBC",
        "BusType":"SK6812_RGBW",
        "Start":113,
        "Length":72
      },
      {
        "Pin":19,
        "ColourOrder":"GRBC",
        "BusType":"SK6812_RGBW",
        "Start":185,
        "Length":40,
        "Reversed":1
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        225
      ],
      "ColourPalette":"Colour 01",
      "SegColour0": {
        "Hue": 25,
        "Sat":94,
        "BrightnessRGB":100
      },
      "Effects": {
        "Function":"Static",
        "Speed":255,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 1000
      },
      "BrightnessRGB": 100
    },
    "BrightnessRGB": 60,
    "BrightnessCCT": 0
  }
  )=====";


#endif



#ifdef DEVICE_MEADOWS__LIVINGROOM__PEBBLES
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Bus1",
        "Pin":13,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "PixelRange":[0,100],
        "ColourPalette":0,
        "SegColour0":{"RGBWC":[255,117,31,0,0]},
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 30
  }
  )=====";

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
     "\"15\":\""  D_GPIO_PIR_1_CTR "\","
     "\"2\":\"" D_GPIO_LED1_CTR  "\""
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // DEVICE_END






#ifdef DEVICE_MEADOWS__LIVINGROOM__TV
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[     
      {
        "Name":"Bus1",
        "Pin":2,
        "ColourOrder":"GRB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":269
      }
    ],
    "Segments":[
      {
        "PixelRange":[0,269],
        "ColourPalette":0,
        "SegColour0":{"RGBWC":[255,117,31,0,0]},
        "ColourType":3,
        "Effects": {
          "Function":"Static",
          "Speed":255,
          "Intensity":127,
          "Grouping":1,
          "RateMs": 20
        },
        "BrightnessRGB": 100
      }
    ],
    "BrightnessRGB": 5
  }
  )=====";

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // DEVICE_END

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Hallway ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/

/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__OUTSIDE__FENCE_LIGHTS
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
  
  /***********************************
   * SECTION: System Configs
  ************************************/     

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
 
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":4,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":250,
  //       "Reversed":1
  //     },
  //     {
  //       "Pin":2,
  //       "ColourOrder":"RGB",
  //       "BusType":"WS2812_RGB",
  //       "Start":250,
  //       "Length":250
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       500
  //     ],
  //     "ColourPalette":"Warm White",
  //     "ColourType":3,
  //     "Effects": {
  //       "Function":"Candles",
  //       "Speed":180,
  //       "Intensity":85,
  //       "Grouping":10,
  //       "RateMs": 20
  //     },
  //     "BrightnessRGB": 100,
  //     "BrightnessCCT": 0

  //   },
  //   "BrightnessRGB": 100,
  //   "BrightnessCCT": 0
  // }
  // )=====";
  
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":4,
        "ColourOrder":"RGB",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":246,
        "Reversed":1
      }
    ],
    "Segment0": {
      "PixelRange": [
        0,
        246
      ],
      "ColourPalette":"Gradient Five",
      "ColourType":3,
      "Effects": {
        "Function":"Candles",
        "Speed":180,
        "Intensity":85,
        "Grouping":1,
        "RateMs": 20
      },
      
      "SegColour0":{"Hue":120,"Sat":100},
      "SegColour1":{"Hue":180,"Sat":100},
      "SegColour2":{"Hue":240,"Sat":100},
      "SegColour3":{"Hue":180,"Sat":100},
      "SegColour4":{"Hue":120,"Sat":100},

      "BrightnessRGB": 100,
      "BrightnessCCT": 0

    },
    "BrightnessRGB": 0,
    "BrightnessCCT": 0
  }
  )=====";
  
 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"
     "\"13\":\""  D_GPIO_PIR_1_CTR "\","
     "\"18\":\"" D_GPIO_LED1_CTR  "\""
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";


#endif




/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Guest Room ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/





/**
 * @description: 
 * 
 * Landing Panel
 *  ** PIR Detector for Landing
 *  ** Nextion Panel
 *  ** BME
 *  ** 4C RGB strip with upper/lower section
 * MasterBedroom
 * 
 * 
 * *********************************************************************************************************************************************************************************
*/
#ifdef DEVICE_MEADOWS__PRUSA_CLIMATE_CONTROL
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

  
  #define SETTINGS_HOLDER 1239

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define ENABLE_FREERAM_APPENDING_SERIAL

  /***********************************
   * SECTION: System Configs
  ************************************/     


  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device

  


  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__SOLAR
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION
  // #define ENABLE_TEMPLATE_SECTION__LIGHTING
  // #define ENABLE_TEMPLATE_SECTION__ENERGY
  // #define ENABLE_TEMPLATE_SECTION__ENERGY__PZEM
  // #define ENABLE_TEMPLATE_SECTION__ENERGY__INA219
  // #define ENABLE_TEMPLATE_SECTION__DISPLAY_NEXTION
  // #define ENABLE_TEMPLATE_SECTION__CONTROLLER__HVAC
  // #define ENABLE_TEMPLATE_SECTION__DISPLAY_OLED
  // #define ENABLE_TEMPLATE_SECTION__DRIVER_RELAYS


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  // 
  // 

  


  /***********************************
   * SECTION: Lighting Configs
  ************************************/    
  #define ENABLE_FEATURE_WATCHDOG_TIMER
  
  

  // #define ENABLE_DEBUGFEATURE_TELEMETRY__MQTT_SEND_HEALTH_EVERY_SECOND
  // #define ENABLE_DEVFEATURE_DEBUG_REMOVE_POSSIBLE_ERROR_CODE
  // #define ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS 110
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  #define DISABLE_SLEEP

  #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

  #define USE_MODULE_CORE_RULES

  // #define DEVICENAMEBUFFER_NAME_BUFFER_LENGTH 1000
  // #define DEVICENAMEBUFFER_NAME_INDEX_LENGTH  100
  // #define DB18_SENSOR_MAX                     15
  //#define DATA_BUFFER_PAYLOAD_MAX_LENGTH      3000 //needed for db sensosrs, but currently causes crash in lighting
  // #define MQTT_MAX_PACKET_SIZE                3000

  #define USE_MODULE_SENSORS_INTERFACE
    #
  //   #define USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
  // #define USE_MODULE_SENSORS_DS18X20
  //   #define DS18X20_MAX_SENSORS 20
  //   #define ENABLE_DEVFEATURE_DS18B20_SEARCHING_SENSOR_LOCATION_WITH_ADDRESS_TEMP_SPLASH
  
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 
  #define USE_MODULE_SENSORS_BME
    

  #ifdef ENABLE_TEMPLATE_SECTION__DISPLAY_OLED
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SH1106
    #define SHOW_SPLASH
    // 4x10
    // Uptime so I know its working by glance
    // Relay Minutes On
    // Shower Temp /     Bath Temp
  #endif
  

  // #define ENABLE_FEATURE_SENSORS_INTERFACE__SNAPSHOT_READINGS_TO_CALIBRATION_FILE

  // #define USE_MODULE_CORE_FILESYSTEM
  // 
  // 
  // #define CALIB_FS
  // #define ENABLE_DEBUGFEATURE__CALIB_CAPTURE_VERBOSE


  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_RELAY
  // #define USE_MODULE_DRIVERS_LEDS
  //   #define MAX_RELAYS 1
    
  /**
   * @brief 
   * Add extra settings parameter that allows for "expected total device count" (relays+sensors) and allow a NEW alert topic (status/alert status/event) to tell me something is wrong
   * 
   */
  // #define USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
    // #define ENABLE_DEVFEATURE_SENSORS_INTERFACE_SHOW_TEMPERATURE_AS_COLOUR  //should this be a "controller", or via interface "getTemp convert to heatmap"
    // #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 60  
    // #define USE_DEVFEATURE_SENSOR_COLOURS_TOP_TO_BOTTOM

  // #define USE_MODULE_CONTROLLER_HVAC
  //   
  //   #define HEATING_DEVICE_MAX 1

  // #define USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL




  //turn on later
  // #define USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE


  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"26\":\""  D_GPIO_REL1_CTR    "\","   // Immersion = Also add additonal LED with relay pin
      #endif
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"23\":\"" D_GPIO_I2C_SDA_CTR   "\","
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"4\":\"" D_GPIO_DS18X20_1_CTR  "\"," // Group 1 = Basic Set, use just these until device is stable
      // "\"19\":\"" D_GPIO_DS18X20_2_CTR  "\"," // Group 2 = Detailed, use these only after stress testing with 3 pins for sensors with rewrite. Read datasheet.
      #endif
      #ifdef USE_MODULE_SENSORS_PIR
      "\"32\":\"" D_GPIO_SWT1_CTR "\","       // Stairs
      "\"4\":\""  D_GPIO_SWT2_CTR "\","       // Landing
      "\"25\":\"" D_GPIO_SWT3_CTR "\","       // Hotpress (negating need of button? or use non-momentary switch to enable/disable it)
      #endif 
      #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
        // "\"26\":\"" D_GPIO_RGB_DATA1_CTR  "\"," // Orange - Immersional Relay
        // "\"27\":\"" D_GPIO_RGB_DATA1_CTR  "\"," // Green - Immersion Tank
        // "\"14\":\"" D_GPIO_RGB_DATA1_CTR  "\"," // Blue - Landing Panel
        // "\"13\":\"" D_GPIO_RGB_DATA1_CTR  "\"," // Brown - Immersion Relay (Label wrong, )
        #ifdef USE_RGB_OUT_LANDING_PANEL
        "\"14\":\"" D_GPIO_RGB_DATA1_CTR  "\"," // Blue - Landing Panel
        #endif
        #ifdef USE_RGB_OUT_TANK
        "\"27\":\"" D_GPIO_RGB_DATA1_CTR  "\"," // Green - Immersion Tank
        #endif      
        #ifdef ENABLE_DEVFEATURE_MULTIPLE_PIXEL_PINS
        "\"14\":\"" D_GPIO_RGB_DATA2_CTR  "\","
        "\"27\":\"" D_GPIO_RGB_DATA3_CTR  "\","
        #endif
      #endif
      #ifdef USE_MODULE_DISPLAYS_NEXTION
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\","
      #endif
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"33\":\"" D_GPIO_KEY1_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_CTR  "\","
      #endif
      #ifdef USE_MODULE_DRIVERS_LEDS
      "\"12\":\""  D_GPIO_LED2_INV_CTR "\"," 
      "\"21\":\""  D_GPIO_LED3_INV_CTR "\"," 
      #endif 
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""  // Also optional physical LED to be made external to box (buy one of those drill through ones!)
      // 5  // i2c oled needs to be another bus because of BME? or, what if it shared!!
      // 15
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  /***********************************
   * SECTION: Device Configs
  ************************************/    
 

//  {"NumDevices":13,"DeviceNameIndex":[-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1],"AddressList":[
//   [40,48,142,47,0,0,0,112],
//   [40,4,161,51,0,0,0,252],
//   [40,194,109,51,0,0,0,210],
//   [40,38,95,51,0,0,0,32],
//   [40,182,157,48,0,0,0,184],
//   [40,153,85,50,0,0,0,49],
//   [40,217,113,51,0,0,0,44],
//   [40,121,104,51,0,0,0,255],
//   [40,53,140,51,0,0,0,50],
//   [40,181,105,48,0,0,0,88],
//   [40,15,153,47,0,0,0,148],
//   [40,111,124,47,0,0,0,95],
//   [40,127,141,47,0,0,0,111]]}

/*** View of the enclosure fron the front, with signal starting at the bottom left
 * 
 *     *******************************************
 *     *   FTL             FTC             FTR   *
 *     * FL100              ||             FR100 *
 *     *                    ||                   *
 *     * FL75               ||              FR75 *
 *     *                 Front View              *
 *     * FL50               ||              FR50 *
 *     *                    ||                   *
 *     * FL25               ||              FR25 *
 *     *                    ||                   *
 *     * FL00               ||              FR00 *
 *     *                    ||                   *
 *     *******************************************
 */

/**
 * @brief Group 1: Front enclosure sensors (bottom left, up, across, to bottom right)
 * */
// Front right - top to bottom
#define D_DEVICE_SENSOR_DB18S20_01_NAME        "FrontRight00"
#define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,153,85,50,0,0,0,49]"
#define D_DEVICE_SENSOR_DB18S20_02_NAME        "FrontRight25"
#define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,48,142,47,0,0,0,112]"
#define D_DEVICE_SENSOR_DB18S20_03_NAME        "FrontRight50"
#define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,38,95,51,0,0,0,32]"
#define D_DEVICE_SENSOR_DB18S20_04_NAME        "FrontRight75"
#define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,15,153,47,0,0,0,148]"
#define D_DEVICE_SENSOR_DB18S20_05_NAME        "FrontRight100"
#define D_DEVICE_SENSOR_DB18S20_05_ADDRESS     "[40,217,113,51,0,0,0,44]"
// Front cetnre
#define D_DEVICE_SENSOR_DB18S20_06_NAME        "FrontTopLeft"
#define D_DEVICE_SENSOR_DB18S20_06_ADDRESS     "[40,182,157,48,0,0,0,184]"
#define D_DEVICE_SENSOR_DB18S20_07_NAME        "FrontTopCentre"
#define D_DEVICE_SENSOR_DB18S20_07_ADDRESS     "[40,121,104,51,0,0,0,255]"
#define D_DEVICE_SENSOR_DB18S20_08_NAME        "FrontTopRight"
#define D_DEVICE_SENSOR_DB18S20_08_ADDRESS     "[40,111,124,47,0,0,0,95]"
// Front left - bottom to top
#define D_DEVICE_SENSOR_DB18S20_09_NAME        "FrontLeft00"
#define D_DEVICE_SENSOR_DB18S20_09_ADDRESS     "[40,127,141,47,0,0,0,111]"
#define D_DEVICE_SENSOR_DB18S20_10_NAME        "FrontLeft25"
#define D_DEVICE_SENSOR_DB18S20_10_ADDRESS     "[40,53,140,51,0,0,0,50]"
#define D_DEVICE_SENSOR_DB18S20_11_NAME        "FrontLeft50"
#define D_DEVICE_SENSOR_DB18S20_11_ADDRESS     "[40,181,105,48,0,0,0,88]"
#define D_DEVICE_SENSOR_DB18S20_12_NAME        "FrontLeft75"
#define D_DEVICE_SENSOR_DB18S20_12_ADDRESS     "[40,4,161,51,0,0,0,252]"
#define D_DEVICE_SENSOR_DB18S20_13_NAME        "FrontLeft100"
#define D_DEVICE_SENSOR_DB18S20_13_ADDRESS     "[40,194,109,51,0,0,0,210]"

  
  #define D_DEVICE_SENSOR_DB18S20_14_NAME        "Sensor 01"
  #define D_DEVICE_SENSOR_DB18S20_14_ADDRESS     "[40,68,132,149,240,1,60,87]"
  #define D_DEVICE_SENSOR_DB18S20_15_NAME        "Sensor 02" 
  #define D_DEVICE_SENSOR_DB18S20_15_ADDRESS     "[40,174,159,3,0,0,0,1]"



  #define D_DEVICE_DRIVER_RELAY_01_NAME "Immersion"
  
  #define D_DEVICE_SENSOR_BME_LONG_WIRE_NAME  "BME Dryer"
  #define D_DEVICE_SENSOR_BME_SHORT_WIRE_NAME "BME Enclosure"

  #define D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "Immersion"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "PRINTER" "\""
      "],"  
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_DRIVER_RELAY_01_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BME_LONG_WIRE_NAME "\"," // 0x76
        "\"" D_DEVICE_SENSOR_BME_SHORT_WIRE_NAME  "\""       // 0x77
      "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\","
        // Group 2
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\""    
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_CONTROLLER_HVAC_ZONE0_NAME "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ","      
        "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\":" D_DEVICE_SENSOR_DB18S20_05_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\":" D_DEVICE_SENSOR_DB18S20_06_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\":" D_DEVICE_SENSOR_DB18S20_07_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\":" D_DEVICE_SENSOR_DB18S20_08_ADDRESS ","       
        "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\":" D_DEVICE_SENSOR_DB18S20_09_ADDRESS ","  
        // Group 2
        "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\":" D_DEVICE_SENSOR_DB18S20_10_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\":" D_DEVICE_SENSOR_DB18S20_11_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\":" D_DEVICE_SENSOR_DB18S20_12_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\":" D_DEVICE_SENSOR_DB18S20_13_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\":" D_DEVICE_SENSOR_DB18S20_14_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\":" D_DEVICE_SENSOR_DB18S20_15_ADDRESS ""
      "}"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_DRIVER_RELAY_01_NAME "\"," // Also an array to match heating/cooling
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "}"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":1,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
  
  //  "\"Calib\": {"
  //     "\"Enable\": 1,"            // start capturing at boot
  //     "\"Name\": \"\","             // empty → no filter (capture all)
  //     "\"SampleMs\": 1000,"       // collect once per second
  //     "\"DumpSecs\": 60"          // flush to /sensor_calib_captures.json every 60s
  //   "}"
    "\"Calib\": {"
        "\"Enable\": 1,"
        "\"Name\": \"\","                // ignored if NameList present
        "\"SampleMs\": 1000,"
        "\"DumpSecs\": 60,"
        "\"NameList\": ["
          "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_05_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_06_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_07_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_08_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_09_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_10_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_11_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_12_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_13_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_14_NAME "\","
          "\"" D_DEVICE_SENSOR_DB18S20_15_NAME "\""
        "]"
      "}"

    "}";

  // #ifdef USE_RGB_OUT_LANDING_PANEL
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // #define USE_SK6812_METHOD_DEFAULT
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "HardwareType":"SK6812",
  //   "AnimationMode":"Effects",
  //   "ColourOrder":"grbw",
  //   "ColourPalette":"Christmas 01",
  //   "Effects": {
  //     "Function":1,
  //     "Intensity":50
  //   },
  //   "Transition": {
  //     "TimeMs": 0,
  //     "RateMs": 1000
  //   },
  //   "SegColour": {
  //     "Hue": 120,
  //     "Sat": 100,
  //     "SubType":3
  //   },
  //   "BrightnessRGB_255": 255,
  //   "BrightnessCCT_255": 255
  // }
  // )=====";
  // #endif // USE_MODULE_LIGHTS_INTERFACE
  // #endif // USE_RGB_OUT_LANDING_PANEL

  // #ifdef USE_RGB_OUT_TANK
  // #ifdef USE_MODULE_LIGHTS_INTERFACE
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "HardwareType":"WS28XX",
  //   "AnimationMode":"Effects",
  //   "ColourOrder":"grb",
  //   "ColourPalette":"Christmas 01",
  //   "Effects": {
  //     "Function":1,
  //     "Intensity":50
  //   },
  //   "Transition": {
  //     "TimeMs": 0,
  //     "RateMs": 1000
  //   },
  //   "SegColour": {
  //     "Hue": 120,
  //     "Sat": 100,
  //     "SubType":3
  //   },
  //   "BrightnessRGB_255": 255,
  //   "BrightnessCCT_255": 1
  // }
  // )=====";
  // #endif // USE_MODULE_LIGHTS_INTERFACE
  // #endif // USE_RGB_OUT_TANK
    
  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // "{"// for PIR to follow
  //   "\"Rule0\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
  //       "\"DeviceName\":0,"
  //       "\"State\":\"On\""
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
  //       "\"DeviceName\":0," 
  //       "\"State\":\"Follow\""
  //     "}"
  //   "},"
  //   "\"Rule1\":{"
  //     "\"Trigger\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_SWITCHES_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_INPUT_STATE_CHANGED_CTR "\","
  //       "\"DeviceName\":1,"
  //       "\"State\":\"On\""
  //     "},"
  //     "\"Command\":{"
  //       "\"Module\":\"" D_MODULE_SENSORS_MOTION_FRIENDLY_CTR "\","
  //       "\"Function\":\"" D_TASK_EVENT_MOTION_STARTED_CTR "\","
  //       "\"DeviceName\":1," 
  //       "\"State\":\"Follow\""
  //     "}"
  //   "}"
  // "}";

#endif



/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Garage ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/




/**
 * @brief 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                         |5V     |USB|     5|
 *                         |GND              6| 
 *                         |3V3              7|
 *                         |4  (fL, BIL)     8| I2C SDA   - Blue LED
 *                         |3                9| I2C SCL
 *                         |2 fL            10| 
 *                    U1RX |1               20| U0TX
 *                    U1TX |0               21| U0RX
 *                         ____________________
 *  */
#ifdef DEVICE_MEADOWS__GARAGE__ROOM_SENSOR
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "template"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
   
   #define MQTT_PORT     1883
    
  /***********************************
   * SECTION: System Configs
  ************************************/    

  // #define ENABLE_DEBUGFEATURE_SENSORS__SPLASH_I2C_SCAN

 /***********************************
  * SECTION: Network Configs
 ************************************/    

 /***********************************
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
 ************************************/  

#define ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
#define ENABLE_TEMPLATE_SECTION__SENSORS__BME

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

//  #define ENABLE_DEBUGFEATURE_WIFI__SUPERMINI_REDUCE_WIFI_BAD_ANTENNA_HARDWARE

#define USE_MODULE_SENSORS_INTERFACE
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BH1750
  #define USE_MODULE_SENSORS_BH1750
#endif
#ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define USE_MODULE_SENSORS_BME    
#endif
#define USE_MODULE_SENSORS_PIR

/***********************************
 * SECTION: Lighting Configs
************************************/  


/***********************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
 * SECTION: Module/GPIO Configs
************************************/  

#define USE_MODULE_TEMPLATE
DEFINE_PGM_CTR(MODULE_TEMPLATE) 
"{"
  "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
  "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
  "\"" D_GPIO_NUMBER "\":{"          
    #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750)
    "\"10\":\"" D_GPIO_I2C_SDA_CTR   "\","
    "\"9\":\"" D_GPIO_I2C_SCL_CTR   "\","    
    #endif
    #ifdef USE_MODULE_SENSORS_PIR
    "\"4\":\""  D_GPIO_PIR_1_CTR "\""
    #endif
  "},"
  "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
  "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
"}";

/***********************************
 * SECTION: TEMPLATE: Names
************************************/    

#define D_DEVICE_SENSOR_BH1750_NAME "Garage"
#define D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "Garage"

#define USE_FUNCTION_TEMPLATE
DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
"{"
  "\"" D_DEVICENAME "\":{"
    "\"" D_MODULE_SENSORS_PIR_CTR "\":["
      "\"" D_DEVICE_SENSOR_MOTION_FRIENDLY_NAME_LONG "\""
    "],"
    "\"" D_MODULE_SENSORS_BH1750_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "],"
    "\"" D_MODULE_SENSORS_BME_CTR "\":["
      "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
    "]"
  "}"
"}";


#endif


/**
 * @description: 
 *  
 * **********************************************************************************************************************************************************************************/
#ifdef DEVICE_MEADOWS__GARAGE__FURNACE
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  /***********************************
   * SECTION: System Configs
  ************************************/     

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define USE_MODULE_SENSORS_INTERFACE
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"15\":\"" D_GPIO_DS18X20_1_CTR  "\","
      #endif
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  /***********************************
   * SECTION: Device Configs
  ************************************/    
  
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "FurnaceInFlow"
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,217,230,84,0,0,0,220]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "FurnaceOutFlow"
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,131,176,82,0,0,0,61]"
  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "FurnaceExhaust"
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,10,43,85,0,0,0,0]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\""    
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ""
      "}"  
    "}"
    "}";

#endif




#ifdef DEVICE_MEADOWS__TREADMILL_POWER_MONITOR
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

  // #define DEVICENAME_CTR          "treadmill_power_monitor"
  // #define DEVICENAME_FRIENDLY_CTR "HVAC Desk DevPlatform"
  // #define DEVICENAME_ROOMHINT_CTR "Bedroom"
  // #define MQTT_HOST   "192.168.1.70" // primary
  //   #define MQTT_PORT     1883
    
  #define SETTINGS_HOLDER 1239


  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  
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


  #define USE_MODULE_SENSORS_SUN_TRACKING


  /***********************************
   * SECTION: Network Configs
  ************************************/    

  

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  #define USE_MODULE_SENSORS_INTERFACE  
  #define USE_MODULE_SENSORS_SWITCHES
    #

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
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_SWITCHES
      "\"23\":\"" D_GPIO_SWT1_CTR  "\","
      #endif
      "\"16\":\""  D_GPIO_PZEM0XX_RX_MODBUS_CTR "\"," 
      "\"17\":\""  D_GPIO_PZEM0XX_TX_CTR "\","
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","   
      #endif
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""   // builtin led
      // 32 - LED Strip External
      // 21 - LED Strip Onboard
      // 25?
      // 
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
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
    "\"" D_ENERGY "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":{"
        "\"DeviceCount\":1"    
    "},"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":["
        "\"" D_DRIVER_ENERGY_0_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_ENERGY_PZEM004T_CTR "\":["
        "\"" D_SENSOR_PZEM004T_0_FRIENDLY_NAME_CTR "\""
      "],"
      "\"" D_MODULE_SENSORS_SWITCHES_CTR "\":["
        "\"" "TrackingActive" "\""
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_ENERGY_INTERFACE_CTR "\":[" 
        D_DEVICE_SENSOR_PZEM004T_0_ADDRESS ""
      "]"  
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":120}"  
  "}";



  
#endif


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Bathroom ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/*******
 * Controller:
 * - Relay for immersion
 * - DB18B20 for water tank
 * - BME280 for humidity and temperature
 * 
 * - Power from attic
 * 
 * -- Will use this to rework the heating code
 * -- Currently, heating and relay contain counters for ontime, I need to decide
 *   --- should controller maintain the timer, then control the relay direct
 *   --- should controller set the relay timer? I think controller should contain the timer, and we need a "disable relay counter" or "controlled by another system"
 * 
 ***/
 #ifdef DEVICE_MEADOWS__BATHROOM__IMMERSION
 #ifndef DEVICENAME_CTR
 #define DEVICENAME_CTR          "template"
 #endif
 #ifndef DEVICENAME_FRIENDLY_CTR
 #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
 #endif
 #ifndef DEVICENAME_DESCRIPTION_CTR
 #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
 #endif
 #define DEVICENAME_ROOMHINT_CTR "testgroup"
 #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883   
   
  /***********************************
  * SECTION: System Debug Options
  ************************************/    
  ///////////////////////////////////////////// Enable Logs
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays

  ///////////////////////////////////////////// System Logs
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // 
  // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT
  // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT
  
  #define ENABLE_DEBUG_MODULE_HARDWAREPINS_SUBSECTION_TEMPLATES
 
  #define ENABLE_TEMPLATE_SECTION__SENSORS__BME
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
 
  /***********************************
   * SECTION: Sensor Configs
  ************************************/  
 
  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_PIR
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__BME
    #define USE_MODULE_SENSORS_BME
      
  #endif
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 5
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 

  /***********************************
   * SECTION: Driver Configs
  ************************************/  
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_RELAY

  /***********************************
  * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  /***********************************
  * SECTION: Energy Configs
  ************************************/  

  /***********************************
  * SECTION: Controller Configs
  ************************************/  

  #define USE_MODULE_CONTROLLER_HVAC
  #define HEATING_DEVICE_MAX 1
  

  /***********************************                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
  * SECTION: Module/GPIO Configs
  ************************************/  
 
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      #if defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
      "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","
      "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\","    
      #endif
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"15\":\"" D_GPIO_DS18X20_1_CTR "\","
      #endif    
      #ifdef USE_MODULE_DRIVERS_RELAY
      "\"2\":\"" D_GPIO_REL_CTR "1" "\","
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  #define D_DEVICE_SENSOR_BH1750_NAME "HotPress"

  #define D_DEVICE_HEATER_0_NAME "Immersion"

  // #define D_DEVICE_SENSOR_DB18S20_0_NAME        "HotWaterTank_Out"
  // #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,93,107,83,0,0,0,75]"
  // #define D_DEVICE_SENSOR_DB18S20_1_NAME        "HotWaterTank_FurnaceInflow"
  // #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,211,122,83,0,0,0,3]"
  // #define D_DEVICE_SENSOR_DB18S20_2_NAME        "HotWaterTank_FurnaceOutflow"
  // #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,47,181,81,0,0,0,70]"

  #define D_DEVICE_SENSOR_DB18S20_0_NAME        "HotWaterTank_FurnaceInflow"
  #define D_DEVICE_SENSOR_DB18S20_0_ADDRESS     "[40,93,107,83,0,0,0,75]"

  #define D_DEVICE_SENSOR_DB18S20_1_NAME        "HotWaterTank_FurnaceOutflow"
  #define D_DEVICE_SENSOR_DB18S20_1_ADDRESS     "[40,211,122,83,0,0,0,3]"

  #define D_DEVICE_SENSOR_DB18S20_2_NAME        "HotWaterTank_Out"
  #define D_DEVICE_SENSOR_DB18S20_2_ADDRESS     "[40,47,181,81,0,0,0,70]"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\""
      "],"
      "\"" D_MODULE_SENSORS_BME_CTR "\":["
        "\"" D_DEVICE_SENSOR_BH1750_NAME "\""
      "],"
      "\"" D_MODULE_DRIVERS_RELAY_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\""
      "],"
      "\"" D_MODULE_CONTROLLER_HVAC_CTR "\":["
        "\"" D_DEVICE_HEATER_0_NAME "\""
      "]"
    "},"    
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\":" D_DEVICE_SENSOR_DB18S20_0_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_1_NAME "\":" D_DEVICE_SENSOR_DB18S20_1_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_2_NAME "\":" D_DEVICE_SENSOR_DB18S20_2_ADDRESS ""
      "}"  
    "},"
    "\"" "HVACZone" "\":{"
      "\"" "SetSensor" "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_0_NAME "\""
      "],"
      "\"" "SetOutput" "\":["
        "{"
          "\"" "ModuleID" "\":\"" D_MODULE_DRIVERS_RELAY_CTR "\","
          "\"" "DriverName" "\":\"" D_DEVICE_HEATER_0_NAME "\","
          "\"" "HVAC_Type" "\":[" "\"Heating\"" "]"
        "}"
      "]"
    "},"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60}" 
  "}"; 
 
#endif



/**
 * @description: 
 *  
 * **********************************************************************************************************************************************************************************/
#ifdef DEVICE_MEADOWS__BATHROOM__RADIATOR
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "template_name"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "Template Name"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "Template Description"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "template_roomhint"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    

  /***********************************
   * SECTION: System Configs
  ************************************/     

  /***********************************
   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  ************************************/  
  
  #define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  /***********************************
   * SECTION: Lighting Configs
  ************************************/    

  #define USE_MODULE_SENSORS_INTERFACE
  #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
    #define USE_MODULE_SENSORS_DS18X20
      #define DS18X20_MAX_SENSORS 20
        #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
  #endif 

  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_SENSORS_DS18X20
      "\"15\":\"" D_GPIO_DS18X20_1_CTR  "\","
      #endif
      "\"2\":\""  D_GPIO_LED1_INV_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  /***********************************
   * SECTION: Device Configs
  ************************************/    
  
  #define D_DEVICE_SENSOR_DB18S20_01_NAME        "BathroomRadiator100" // Long
  #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,68,213,84,0,0,0,237]"
  #define D_DEVICE_SENSOR_DB18S20_02_NAME        "BathroomRadiator90" // Long
  #define D_DEVICE_SENSOR_DB18S20_02_ADDRESS     "[40,137,228,84,0,0,0,113]"
  #define D_DEVICE_SENSOR_DB18S20_03_NAME        "BathroomRadiator40" // short, with label E7
  #define D_DEVICE_SENSOR_DB18S20_03_ADDRESS     "[40,170,67,3,30,19,2,25]" //hot
  #define D_DEVICE_SENSOR_DB18S20_04_NAME        "BathroomRadiator00" // short, with label
  #define D_DEVICE_SENSOR_DB18S20_04_ADDRESS     "[40,255,152,171,193,23,4,231]" //231=E7


  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\""    
      "]"
    "},"
    "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        // Group 1
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_02_NAME "\":" D_DEVICE_SENSOR_DB18S20_02_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_03_NAME "\":" D_DEVICE_SENSOR_DB18S20_03_ADDRESS ","
        "\"" D_DEVICE_SENSOR_DB18S20_04_NAME "\":" D_DEVICE_SENSOR_DB18S20_04_ADDRESS ""
      "}"  
    "}"
    "}";

#endif




 

/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** ROOM: Outside ****************************************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                  DB_3V3 |3V3     |USB|     VIN| TOF 5V
 *                  DB_GND |GND               GND| TOF GND
 *                    DB18 |15 (fL)            13|
 *                         |2  (fL, BIL)  (fH) 12| 
 *                         |4             (fH) 14|
 *                         |RX2/17             27| 
 *                         |TX2/16             26|  
 *                         |5  (fL)            25|  
 *                         |18                 33|  
 *                         |19                 32|  
 *             TOF I2C_SDA |21  SDA     (fL) * 35|   
 *                         |RX0         (fL) * 34|  
 *                         |TX0              ' VN| 
 *             TOF I2C_SCL |22  SCL          ' VP| 
 *                         |23               ' EN| 
 *                          _____________________
 * 
 
DELETE FROM "SENSOR_OilTankDistance_DistanceMM" 
WHERE time >= '2025-05-10T20:00:00Z' AND time <= '2025-05-11T10:30:00Z'



 */
 #ifdef DEVICE_MEADOWS__OUTSIDE__OILTANK
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "testbed_default"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883
    
 /***********************************
  * SECTION: System Debug Options
 ************************************/    
 ///////////////////////////////////////////// Enable Logs
 // #define DISABLE_SERIAL
 // #define DISABLE_SERIAL0_CORE
 
 // #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
 
 ///////////////////////////////////////////// System Logs
 // #define ENABLE_ADVANCED_DEBUGGING
 // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
 // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
 // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
 // #define ENABLE_DEBUG_FUNCTION_NAMES
 // 
 // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
 // #define ENABLE_DEBUG_LINE_HERE
 // #define ENABLE_DEBUG_LINE_HERE2
 // #define ENABLE_DEBUG_LINE_HERE3
 // #define ENABLE_DEBUG_LINE_HERE_TRACE
 // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
 // #define USE_DEBUG_PRINT
 // #define ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT

//new feature to only show logs for a specific module when developing code
  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE 5028
  // #define ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE_ARRAY [1, 2]


 /////////////////////////////////////////// Module Logs
//  #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
//  #define ENABLE_FREERAM_APPENDING_SERIAL
 
 /***********************************
  * SECTION: System Configs
 ************************************/    

 #define SETTINGS_HOLDER 1239

 
 
 #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
  // until devices can reliably be used without compiling per device
 
     
 /***********************************
  * SECTION: Network Configs
 ************************************/    

 #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC
   

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

 /*------------------------------------
  * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
  * -----------------------------------*/  
 
#define ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
#define ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20

 /***********************************
  * SECTION: Sensor Configs
 ************************************/  

 #define USE_MODULE_SENSORS_INTERFACE
 
 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__TOF_VL53L1X
  #define USE_MODULE_SENSORS__TOF_VL53L1X
  #define ENABLE_DEVFEATURE_I2C__SET_WIRE_INSTANCE_WITH_TWOWIRE_ZERO
  #define VL53L1X_DISTANCE_MODE Long
  // #define BYPASS_OILTANK_ADDED
 #endif
 #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
//  #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_HARDCODED_ADD
 #define ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_OILTANK_ADD
 #ifdef ENABLE_TEMPLATE_SECTION__SENSORS__DS18X20
   #define USE_MODULE_SENSORS_DS18X20
     #define DS18X20_MAX_SENSORS 5
       #define ENABLE_DEBUG_MQTT_CHANNEL_DB18X20    
        #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE
 #endif 

 /***********************************
  * SECTION: Lighting Configs
 ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

 /***********************************
  * SECTION: Energy Configs
 ************************************/  

 /***********************************
  * SECTION: Controller Configs
 ************************************/  

 /***********************************
  * SECTION: Module/GPIO Configs
 ************************************/  

 #define USE_MODULE_TEMPLATE
 DEFINE_PGM_CTR(MODULE_TEMPLATE) 
 "{"
   "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
   "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
   "\"" D_GPIO_NUMBER "\":{"  
     #ifdef USE_MODULE_SENSORS_DS18X20
     "\"15\":\"" D_GPIO_DS18X20_1_CTR "\","
     #endif            
     #if defined(USE_MODULE_SENSORS__TOF_VL53L0X) || defined(USE_MODULE_SENSORS__TOF_VL53L1X) || defined(USE_MODULE_SENSORS_BME) || defined(USE_MODULE_SENSORS_BH1750) || defined(USE_MODULE_ENERGY_INA219) || defined(USE_MODULE_DISPLAYS_OLED_SH1106)
     "\"21\":\"" D_GPIO_I2C_SDA_CTR   "\","
     "\"22\":\"" D_GPIO_I2C_SCL_CTR   "\""   
     #endif
   "},"
   "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
   "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
 "}";
 
 /***********************************
  * SECTION: TEMPLATE: Names
 ************************************/    

 #define D_DEVICE_SENSOR_DB18S20_01_NAME        "OilTank"
 #define D_DEVICE_SENSOR_DB18S20_01_ADDRESS     "[40,131,147,47,0,0,0,190]"

 #define USE_FUNCTION_TEMPLATE
 DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
 "{"
   "\"" D_DEVICENAME "\":{"
     "\"" D_MODULE_SENSORS__TOF_VL53L1X__CTR "\":["
       "\"" "OilTankDistance" "\""
     "],"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":["
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\""
      "],"
   "},"
   "\"" D_SENSORADDRESS "\":{"
      "\"" D_MODULE_SENSORS_DB18S20_CTR "\":{" 
        "\"" D_DEVICE_SENSOR_DB18S20_01_NAME "\":" D_DEVICE_SENSOR_DB18S20_01_ADDRESS ""
      "}"   
    "},"
   "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60}"
 "}";


#endif



/**
 * @brief 
 * 
 * Testbed for motion/distance sensors
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__OFFICE__433MHZ_NODE
#ifndef DEVICENAME_CTR
#define DEVICENAME_CTR          "testbed_default"
#endif
#ifndef DEVICENAME_FRIENDLY_CTR
#define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
#endif
#ifndef DEVICENAME_DESCRIPTION_CTR
#define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
#endif
#define DEVICENAME_ROOMHINT_CTR "testgroup"
#define MQTT_HOST   "192.168.3.70"
  
  #define MQTT_PORT     1883
  
  // #define USE_MODULE_DRIVERS_INTERFACE

  #define USE_MODULE_DRIVERS_RF433_CODES
    // #define ENABLE_DEVFETURE_DISABLE_EXTENDED_FEATURES_START
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      #ifdef USE_MODULE_DRIVERS_RF433_CODES
      "\"23\":\"" D_GPIO__RF_433MHZ_RX__CTR   "\","
      #endif  
      "\"2\":\"" D_GPIO_LED1_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
  
#endif


/**
 * @brief 
 * 
 * Using 3mm pixels to show sun position on office door frame and skirting.
 * -- vertical, is height (eleveation) of sun
 * -- horizontal, is azimuth of sun from sunrise to sunset, with end pixels showing distance from sun (blue far, red close)
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__OFFICE__SUN_PIXELS_1D

  #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS

  #define USE_MODULE_SENSORS_INTERFACE


  #define USE_MODULE_SENSORS_SUN_TRACKING 
  #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
  #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
  //   #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED


   
   #define USE_LIGHTING_TEMPLATE
   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
   R"=====(
   {
     "BusConfig":[
       {
          "Pin":2,
          "ColourOrder":"GRB",
          "BusType":"WS2812_RGB",
          "Start":0,
          "Length":415,
          "Skip":35
        }
     ],
     "Segments":[
        {
          "Name":"Horizontal L2R",
          "PixelRange": [
            0,
            102
          ],
          "ColourPalette":"Rainbow",
          "ColourType":3,
          "Effects": {
            "Function":"Sun Azimuth DuskDawn",
            "Intensity":0,
            "Custom1":92,
            "Custom2":53,
            "Check1":0,
            "Check3":1,
            "Grouping":1,
            "RateMs": 20,
            "Reverse":1
          },
          "BrightnessRGB": 100
        },
        {
          "Name":"Vertical B2T",
          "PixelRange": [
            102,
            415
          ],
          "ColourPalette":"Vintage",
          "ColourType":3,
          "Effects": {
            "Function":"Sun Elevation DuskDawn",
            "Intensity":0,
            "Custom1":151,
            "Custom2":87,
            "Check1":0,
            "Check3":1,
            "Grouping":1,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
      ],
      "BrightnessRGB": 100
   }
   )=====";
   
#endif




/**
 * @brief 
 * 
 * Using 3mm pixels to show sun position on office door frame and skirting.
 * -- vertical, is height (eleveation) of sun
 * -- horizontal, is azimuth of sun from sunrise to sunset, with end pixels showing distance from sun (blue far, red close)
 * 
 *          fH (Boot Fail - Pulled High) → Pin must be LOW at boot, else boot may fail
 *          fL (Boot Fail - Pulled Low) → Pin must be HIGH at boot, else boot may fail
 *          key (Key Pin) → GPIO0 on DOIT DevKit v1 (not )
 *          BIL (Built-in LED) → On some boards, pin is used for onboard LED
 *                               *I ~PWM 'NC    
 *                          _____________________
 *                    3V3  |3V3     |USB|     VIN|
 *                    GND  |GND               GND| 
 *                 =BUZZER |15 (fL)            13|
 *              =SONIC TX1 |2  (fL, BIL)  (fH) 12| 
 *              =SONIC RX1 |4             (fH) 14|
 *              =RADAR TX2 |RX2/17             27| 
 *              =RADAR RX2 |TX2/16             26| TOF1EN
 *                         |5  (fL)            25| TOF1INT
 *                         |18                 33| TOF0EN
 *              LM386 SPKR |19                 32| TOF0INT
 *        OLED,TOF I2C_SDA |21  SDA     (fL) * 35| RADAR_3p18GHZ 
 *                         |RX0         (fL) * 34| PIR_LARGE
 *                         |TX0              ' VN| 
 *        OLED,TOF I2C_SCL |22  SCL          ' VP| 
 *                     NEO |23               ' EN| 
 *                          _____________________
 * 
 * 
 */
#ifdef DEVICE_MEADOWS__OFFICE__BLACK_STAND
  // #ifndef DEVICENAME_CTR
  // #define DEVICENAME_CTR          "testbed_default"
  // #endif
  // #ifndef DEVICENAME_FRIENDLY_CTR
  // #define DEVICENAME_FRIENDLY_CTR "TestBed ESP32 WEBUI Neopixel"
  // #endif
  // #ifndef DEVICENAME_DESCRIPTION_CTR
  // #define DEVICENAME_DESCRIPTION_CTR "TestBed ESP32 WEBUI Neopixel"
  // #endif
  // #define DEVICENAME_ROOMHINT_CTR "testgroup"
  // #define MQTT_HOST   "192.168.3.70"
  //   
  //   #define MQTT_PORT     1883



  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES

  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_DEBUG_LINE_HERE
  // #define ENABLE_DEBUG_LINE_HERE2

  // #define ENABLE_FREERAM_APPENDING_SERIAL

  // #define ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS 10

  // #define ENABLE_DEBUGFEATURE__OVERIDE_FASTBOOT_DISABLE // comment out to enable fastboot recovery

  // #define ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY

  /***********************************
   * SECTION: Storage Configs
  ************************************/  
 
  // #define ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

  /**
   * For debugging and short term I may want to store everything as JSON, so I can view the data?
   * Longer term, a mixture of JSON/Binary for space.
   * Options should just be ifdef to switch between methods. 
  */
  // #define ENABLE_DEVFEATURE_STORAGE__ALL_DATA_AS_JSON // this will require methods to serialise and deserialise all data

  // New way to start the save into memory periodically, and then recover if available on boot. Init phase of full system.
  // #define ENABLE_FILESYSTEM__MODULES_CORE__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_CORE__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_DRIVERS__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_SENSORS__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_SENSORS__RESTORE_ON_BOOT
  // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__SAVE
  // #define ENABLE_FILESYSTEM__MODULES_LIGHTING__RESTORE_ON_BOOT
  

  // /***********************************
  //  * SECTION: System Configs
  // ************************************/     

  // #define ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING // reduce logging when not debugging


  // /***********************************
  //  * SECTION: Network Configs
  // ************************************/    

  // 
  // 

  // /***********************************
  //  * SECTION: Sensor Configs
  // ************************************/  

  
  // #define USE_MODULE_SENSORS_SUN_TRACKING     
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
  //   #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED
  //   // #define ENABLE_DEBUGFEATURE_SUNTRACKING__DEBUG_SUN_CALCULATIONS

  // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS
  // // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS2

  // // #define ENABLE_DEVFEATURE_SUNTRACKING__SUN_TIME_CALCULATE_SUN_PATHS_ACROSS_DAY

  // // #define USE_MODULE_SENSORS_SUN_TRACKING__DETAILED_MQTT_INFO_UNIX

  // /***********************************
  //  * SECTION: Display Configs
  // ************************************/  

  // /***********************************
  //  * SECTION: Driver Configs
  // ************************************/  

  // /***********************************
  //  * SECTION: Lighting Configs
  // ************************************/  

  // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__JUNE2025__NO_MODULE_GPIO

  // #define USE_LIGHTING_TEMPLATE__AS_SEGMENT_ARRAY
 

  // #ifdef USE_LIGHTING_TEMPLATE__AS_SEGMENTNUMBERED

  // 
  // #define USE_LIGHTING_TEMPLATE
  // DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  // R"=====(
  // {
  //   "BusConfig":[
  //     {
  //       "Pin":2,
  //       "ColourOrder":"GRB",
  //       "BusType":"WS2812_RGB",
  //       "Start":0,
  //       "Length":450
  //     }
  //   ],
  //   "Segment0": {
  //     "PixelRange": [
  //       0,
  //       35
  //     ],
  //     "ColourPalette":"Red",
  //     "ColourType":3,
  //     "Effects": {
  //       "Function":"Static",
  //       "RateMs": 1000
  //     },
  //     "BrightnessRGB": 0
  //   },
  //   "Segment1": {
  //     "PixelRange": [
  //       35,
  //       137
  //     ],
  //     "ColourPalette":"Rainbow",
  //     "ColourType":3,
  //     "Effects": {
  //       "Function":"Candles",
  //       "Speed":180,
  //       "Intensity":85,
  //       "Grouping":1,
  //       "RateMs": 20
  //     },
  //     "BrightnessRGB": 100
  //   },
  //   "Segment2": {
  //     "PixelRange": [
  //       137,
  //       450
  //     ],
  //     "ColourPalette":"Vintage",
  //     "ColourType":3,
  //     "Effects": {
  //       "Function":"Candles",
  //       "Speed":180,
  //       "Intensity":85,
  //       "Grouping":1,
  //       "RateMs": 20
  //     },
  //     "BrightnessRGB": 100
  //   },
  //   "BrightnessRGB": 100
  // }
  // )=====";

//   #endif // USE_LIGHTING_TEMPLATE__AS_SEGMENTNUMBERED


//   #ifdef USE_LIGHTING_TEMPLATE__AS_SEGMENT_ARRAY
 
   
   #define USE_LIGHTING_TEMPLATE
   DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
   R"=====(
   {
     "BusConfig":[
       {
          "Pin":22,
          "ColourOrder":"GRBW",
          "BusType":"SK6812_RGBW",
          "Start":0,
          "Length":144
        },
        {
          "Pin":23,
          "ColourOrder":"GRBW",
          "BusType":"SK6812_RGBW",
          "Start":144,
          "Length":77
        }
     ],
     "Segments":[
        {
          "Name":"Black",
          "PixelRange": [
            0,
            144
          ],
          "ColourPalette":"Orange & Teal",
          "ColourType":3,
          "Effects": {
            "Function":"Static",
            "Intensity":0,
            "Custom1":92,
            "Custom2":53,
            "Check1":0,
            "Check3":1,
            "Grouping":1,
            "RateMs": 20,
            "Reverse":1
          },
          "BrightnessRGB": 100
        },
        {
          "Name":"Silver",
          "PixelRange": [
            144,
            221
          ],
          "ColourPalette":"Retro Clown",
          "ColourType":3,
          "Effects": {
            "Function":"Static",
            "Intensity":0,
            "Custom1":151,
            "Custom2":87,
            "Check1":0,
            "Check3":1,
            "Grouping":1,
            "RateMs": 20
          },
          "BrightnessRGB": 100
        }
      ],
      "BrightnessRGB": 10
   }
   )=====";
//    #endif // USE_LIGHTING_TEMPLATE__AS_SEGMENTNUMBERED

//  /***********************************
//   * SECTION: Enable with one line (to make it easier to switch on and off for debugging)
//  ************************************/  
//   // #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

//  /***********************************
//   * SECTION: Sensor Configs
//  ************************************/  

//   #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION) || defined(ENABLE_TEMPLATE_SECTION__SENSORS__RADAR_3p18GHZ)
//    #define USE_MODULE_SENSORS_INTERFACE
//    #define USE_MODULE_SENSORS_PIR
//     //  #define USE_TEMPLATED_DEFAULT_MOTION_RULE_TEMPLATE_FIRST_SWITCH_IS_MOTION_SENSOR_EVENT
//  #endif

//  #define ENABLE_FEATURE_SYSTEM__SHOW_BOOT_MESSAGE



//  /***********************************
//   * SECTION: Module/GPIO Configs
//  ************************************/  

//  #define USE_MODULE_TEMPLATE
//  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//  "{"
//    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
//    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//    "\"" D_GPIO_NUMBER "\":{"
//      "\"13\":\""  D_GPIO_PIR_1_CTR "\","
//      "\"18\":\"" D_GPIO_LED1_CTR  "\""
//    "},"
//    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//  "}";

  
//  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Hallway"

//  #define USE_FUNCTION_TEMPLATE
//  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//  "{"
//    "\"" D_DEVICENAME "\":{"
//      "\"" D_MODULE_SENSORS_PIR_CTR "\":["
//        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
//      "]"
//    "}"
//  "}";

#endif



#ifdef DEVICE_OFFICE__DESK_LIGHTING 
  
  #define ENABLE_FEATURE_LIGHTING__RGBWW_GENERATE
  #define ENABLE_FEATURE_PALETTE__RGBWW_COLOURS

    // #define USE_TEMPLATED_DEFAULT_LIGHTING_DEFINES__JUNE2025__NO_MODULE_GPIO

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":[5,18,19,21,22],
        "ColourOrder":"RGBCW",
        "BusType":"ANALOG_5CH",
        "Start":0,
        "Length":1
      },
      {
        "Pin":27,
        "ColourOrder":"GRBWC",
        "BusType":"WS2805_RGBWW",
        "Start":1,
        "Length":20
      }
    ],    
    "Segment0":{
      "PixelRange": [
        0,
        1
      ],
      "ColourType":5,
      "ColourPalette":0,
      "SegColour0": {
        "Hue": 25,
        "Sat": 100,
        "BrightnessRGB": 100,
        "BrightnessCCT": 100,
        "CCT_TempPercentage":100
      },
      "Effects": {
        "Function":"Solid Colour",
        "RateMs": 1000,
        "Speed":255
      },
      "BrightnessRGB":100,
      "BrightnessCCT":100
    },
    "Segment1":{
      "PixelRange": [
        1,
        21
      ],
      "ColourType":5,
      "ColourPalette":"Orange & Teal",
      "Effects": {
        "Function":"Bands",
        "RateMs": 1000,
        "Speed":255
      },
      "BrightnessRGB":100,
      "BrightnessCCT":100
    },
    "BrightnessRGB":100,
    "BrightnessCCT":100
  }
  )=====";

#endif








#ifdef DEVICE_OFFICE__NEXTION_DISPLAY__DESK_10INCH
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "office__desk_display_10inch"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  ///////////////////////////////////////////// Enable Logs
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
  
  ///////////////////////////////////////////// System Logs
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT

  ///////////////////////////////////////////// Module Logs
  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  
  /***********************************
   * SECTION: System Configs
  ************************************/   
 
  #define SETTINGS_HOLDER 1239

  
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC
    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
   


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    
    
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
         
        
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK



// #define USE_FEATURE_NEXTION__SERIAL_DEFAULT_BUAD_NEW_PANEL_FIRST_OTA

// #define USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT 115200

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 10//6  
    
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // 
  
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


  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";


#endif



#ifdef DEVICE_OFFICE__NEXTION_DISPLAY__DESK_3P5INCH
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "office__desk_display_3p5inch"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"
  #define MQTT_HOST   "192.168.3.70"
    #define MQTT_PORT     1883

    #define ENABLE_NEXTION

  /***********************************
   * SECTION: System Debug Options
  ************************************/    
  ///////////////////////////////////////////// Enable Logs
  // #define DISABLE_SERIAL
  // #define DISABLE_SERIAL0_CORE
  
  #define ENABLE_DEBUG_MANUAL_DELAYS // permits blocking delays
  
  ///////////////////////////////////////////// System Logs
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  
  // #define SERIAL_LOG_LEVEL_DURING_BOOT 8
  // #define ENABLE_DEBUG_LINE_HERE3
  // #define ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
  // #define USE_DEBUG_PRINT

  ///////////////////////////////////////////// Module Logs
  // #define ENABLE_DEVFEATURE__PIXEL_COLOUR_VALUE_IN_MULTIPIN_SHOW_LOGS  
  #define ENABLE_FREERAM_APPENDING_SERIAL
  
  /***********************************
   * SECTION: System Configs
  ************************************/   
 
  #define SETTINGS_HOLDER 1239

  
  
  #define ENABLE_DEVFEATURE__SAVE_MODULE_DATA
   // until devices can reliably be used without compiling per device
  

  /***********************************
   * SECTION: Network Configs
  ************************************/    

  #define FIRMWARE_DEFAULT__INCLUDE_WEBSERVER_BASIC
    

  /***********************************
   * SECTION: Sensor Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/  

  /***********************************
   * SECTION: Display Configs
  ************************************/  

#ifdef ENABLE_NEXTION

  #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
   


  #ifdef ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
    
    
  #endif // ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
  // #define ENABLE_FREERAM_APPENDING_SERIAL

    // #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 115200
    #define ENABLE_DEVFEATURE_NEXTION__BAUDRETE_DEFAULT 921600

  // #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_NEXTION

  #define ENABLE_DEVFEATURE_NEXTION_DISPLAY        
        #define ENABLE_DEVFEATURE_NEXTION_WEBUI
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER 1
        #define ENABLE_DEVFEATURE_NEEXTION_SWITCH_TO_GLOBAL_WEBSERVER
         
        
        #define ENABLE_DEVFEATURE_NEXTION__TEMPORARY_FIX_SERIAL_PORT_NUMBER_SERIAL1_HVAC_DESK



// #define USE_FEATURE_NEXTION__SERIAL_DEFAULT_BUAD_NEW_PANEL_FIRST_OTA

// #define USE_FEATURE_NEXTION__FORCE_SERIAL_BAUDRATE_FROM_DEFAULT 115200

  
  #define USE_MODULE_DISPLAYS_NEXTION
    #define ENABLE_DEVFEATURE_NEXTION_DISPLAY
  #define NEXTION_DEFAULT_PAGE_NUMBER 5//6  
    
    //  // change to be code option later
    #define ENABLE_FEATURE_NEXTION__WEB_OTA_TFT_DISPLAY_UPDATE
    #define ENABLE_FEATURE_NEXTION__WEB_HTTP_TFT_DISPLAY_UPDATE

#endif

  
  #define ENABLE_DEVFEATURE_NEXTION_WEBUI

  // 
  
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


  /***********************************
   * SECTION: Template Configs
  ************************************/    

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIOC "\":{"
      "\"17\":\"" D_GPIO_NEXTION_TX_CTR "\","
      "\"16\":\"" D_GPIO_NEXTION_RX_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";



  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"MQTTUpdateSeconds\":{\"IfChanged\":10,\"TelePeriod\":60,\"ConfigPeriod\":120},"  
    "\"MQTTSubscribe\":["
      "\"openhab_broadcast/nextion/group/#\""
    "],"
  "}";


#endif



#ifdef DEVICE_MEADOWS__OFFICE__WS2815_PANEL_12V
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR          "coxmas24__redboard_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "testgroup"


  #define ENABLE_LIGHTING__GROUP_ENABLE_2D_TESTING
  

  #ifndef ENABLE_LIGHTING__GROUP_ENABLE_2D_TESTING

  // #define ENABLE_DEBUGFEATURE_LIGHT__PALETTE_RELOAD_LOGGING
  #define ENABLE_DEBUGFEATURE_LIGHTING__TRANSITION_ENDPOINT_PIXEL0

  #define ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION_ON_PULSAR_PALETTES

  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
  * SECTION: Network Configs
  ************************************/  

  #define ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   

  // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE


  // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS


  #define USE_MODULE_SENSORS_INTERFACE
  #define USE_MODULE_SENSORS_SUN_TRACKING      
  #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
  #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
  //   #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
  // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
  // #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED

  #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
  #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D
  #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM

  /**
   * @brief 
   * Need to add a new "realtime" system mode, which means ANYTHING not critical to the design in use, should have minimal impact
   * In this case, if effects are running
   * ** mqtt unless debugging, should reduce to 1hour at most (except health, make it 10 minutes)
   * ** filesystem operations that are slow, reduce 
   * 
   */
  
  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 500


  #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL2_FLASHING_BASIC
  #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL3_FLASHING_EXTENDED
  #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL4_FLASHING_COMPLETE

  #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__LED_SEGMENT_CLOCK
  #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS

  // #define USE_MODULE_NETWORK_WEBSERVER
  // #define ENABLE_FEATURE_LIGHTING__WEBUI
  // // #define ENABLE_DEBUGFEATURE_WEBUI__SHOW_BUILD_DATETIME_IN_FOOTER
  // #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  // #define ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET
  // // #define ENABLE_DEVFEATURE_NETWORK__CAPTIVE_PORTAL
  // // #define ENABLE_DEVFEATURE_WEBSERVER__STYLES_NOW_SHARED

  #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
  #define ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS

  #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 120

  /**
   * @brief tree physical wiring connections
   * 16 outputs
   */
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":2,
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
      "ColourPalette":"Snowy 02",
      "Effects": {
        "Function":"Wipe Random",
        "Speed":127,
        "Intensity":127,
        "Grouping":1,
        "RateMs": 25
      },
      "Brightness": 100
    },
    "Brightness": 1
  }
  )=====";
 
      // ,"Preset":{"Load":1}
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"    
      // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
      // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
      "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "1" "\","
      "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "2" "\","
      "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "3" "\","
      "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "4" "\","
      "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "5" "\","
      "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "6" "\","
      "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "7" "\","
      #ifdef USE_MODULE_SENSORS_BUTTONS
      "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
      "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
      "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
      #endif
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  
  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
        "\"" "Desk" "\""
      "]"
    "}"
  "}";

  #endif
  #ifdef ENABLE_LIGHTING__GROUP_ENABLE_2D_TESTING

  
    // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D
    #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE


    // REmove these, should be in defaults
    // #define ENABLE_EFFECT_DESCRIPTIONS
    // #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D
    // #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
    // #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D
    // #define ENABLE_DEBUGFEATURE_TRACE__LIGHT__DETAILED_PIXEL_INDEXING

    #define ENABLE_FEATURE_LIGHTS__DECIMATE_PIXELS_PIXELS


    #define ENABLE_FEATURE_LIGHTS__2D_MATRIX_EFFECTS
    #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
    #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__2D


    #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__RAY_TRACING

    #define ENABLE_DEVFEATURE_LIGHTING__MIRROR_BYTE_PACKED_PALETTES_IN_CRGBPALETTE16
    #define ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION_ON_PULSAR_PALETTES
    #define ENABLE_FEATURE_LIGHTING__SKIP_GAMMA_CORRECTION

    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE) 
    R"=====(
    {
      "BusConfig":[
        {
          "Pin":2,
          "ColourOrder":"RGB",
          "BusType":"WS2812_RGB",
          "Start":0,
          "Length":256
        }
      ],
      "MatrixConfig":[
        {
          "Width":16,
          "Height":16,
          "BottomStart":0,
          "RightStart":0,
          "Vertical":1,
          "Serpentine":1,
          "xOffset":0,
          "yOffset":0
        }
      ],
      "Segment0": {
        "PixelRange": [
          0,16,
          0,16
        ],
        "ColourPalette":"Rainbow",
        "PaletteMappingValues":[10,15,20],
        "SegColour0": {
          "Hue": 0,
          "Sat":0,
          "BrightnessRGB":100
        },
        "SegColour1": {
          "Hue": 120,
          "Sat":0,
          "BrightnessRGB":100
        },
        "SegColour2": {
          "Hue": 240,
          "Sat":100,
          "BrightnessRGB":100
        },
        "Effects": {
            "Function":"Squared Swirl",
            "Speed":141,
            "Intensity":203,
            "Custom1":255,
            "Custom2":128,
            "Custom3":128,
            "Grouping":1,
            "Decimate":0,
            "RateMs": 25
          },
        "BrightnessRGB": 100,
        "BrightnessCCT": 0
      },
      "BrightnessRGB": 20,
      "BrightnessCCT": 0
    }
    )=====";
            // "Function":"Black Hole",


    /***********************************
    * SECTION: Enable Grouped
    ************************************/  

    /***********************************
    * SECTION: Network Configs
    ************************************/  

    #define ENABLE_DEBUGFEATURE_WEBSERVER_URL_LIST

    /***********************************
     * SECTION: Lighting Configs
    ************************************/   

    // #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE


    // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__SUN_POSITIONS


    #define USE_MODULE_SENSORS_INTERFACE
    #define USE_MODULE_SENSORS_SUN_TRACKING      
    #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES
    #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_TODAY
    //   #define USE_MODULE_SENSORS_SUN_TRACKING__ANGLES__MANUAL_OVERRIDE_FOR_TESTING
    // #define USE_MODULE_SENSORS_SUN_TRACKING__SOLAR_TIMES_FULL
    // #define USE_MODULE_SENSORS_SUN_TRACKING__ADVANCED

    #define ENABLE_DEVFEATURE_LIGHT__INCLUDE_AUDIOREACTIVE_USERMOD
    // #define ENABLE_FEATURE_LIGHTS__EFFECT__AUDIO_REACTIVE__1D
    // #define ENABLE_FEATURE_LIGHTS__EFFECT_GENERAL__LEVEL5_PARTICLE_SYSTEM

  /**
   * @brief 
   * Need to add a new "realtime" system mode, which means ANYTHING not critical to the design in use, should have minimal impact
   * In this case, if effects are running
   * ** mqtt unless debugging, should reduce to 1hour at most (except health, make it 10 minutes)
   * ** filesystem operations that are slow, reduce 
   * 
   */
  
  #define ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS 500


    #define ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_CUSTOM_MAPPING_TABLE
    // #define ENABLE_DEBUGFEATURE_LIGHT__SEGMENTS

    #define SETTINGS_SENSORS_MQTT_IFCHANGED_PERIOD_SECONDS 120

        
    #define USE_MODULE_TEMPLATE
    DEFINE_PGM_CTR(MODULE_TEMPLATE) 
    "{"
      "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
      "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
      "\"" D_GPIO_NUMBER "\":{"    
        // "\"28\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\"," // Bus8
        // "\"13\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR   "\","
        "\"12\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "1" "\","
        "\"26\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "2" "\","
        "\"32\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "3" "\","
        "\"14\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "4" "\","
        "\"27\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "5" "\","
        "\"25\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "6" "\","
        "\"33\":\"" D_GPIO_UNUSED_FORCED_HIGH_CTR  "7" "\","
        #ifdef USE_MODULE_SENSORS_BUTTONS
        "\"35\":\"" D_GPIO_KEY1_INV_CTR  "\","
        "\"34\":\"" D_GPIO_KEY2_INV_CTR  "\","
        "\"0\":\"" D_GPIO_KEY3_INV_CTR  "\""
        #endif
      "},"
      "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
      "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
    "}";

    
    #define USE_FUNCTION_TEMPLATE
    DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
    "{"
      "\"" D_DEVICENAME "\":{"
        "\"" D_MODULE_SENSORS_SUN_TRACKING_CTR "\":["
          "\"" "Desk" "\""
        "]"
      "}"
    "}";


  #endif


#endif // END DEVICE






#endif // _CONFIG__FIRMWARE__MEADOWS_DEPLOYED__H

