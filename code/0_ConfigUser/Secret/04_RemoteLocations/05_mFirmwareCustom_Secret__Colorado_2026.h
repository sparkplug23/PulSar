#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_COLORADO26_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_COLORADO26_H

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

/***
 * Devices:
 * hollow1 -> xmas1  [100 leds as start of xmas whitehall]
 * hollow2 -> xmas2  [meadows front]
 * hollow3 -> xmas3  [meadows side]
 * meadows_icicle_2d (create as a 2d matrix, with 10 pixel drops) - virtual
 * meadows_icicle_hub75 (do matrix of 10 pixel drops, 20*5 = 1000 pixel total. ie emulate with top of matrix)
 * 100leds+pir: TV backlight (standby mode)
 * living radar
 * cam1: south facing top
 * cam2: outside bedroom socket
 * 
 */




// use playlists on both matrix panels, so they run over tests. Which at midnight to 7am should be using the black/solid


//maybe single radar by fire, to detect motion? add radar and PIR, or over by food


// pulsar_mapped_h_cone // ie make like whitehall 3d mapped, I want to get the santa hat rotation down it
        // needs a mode that aids this, eg pixel forge? with "save to fileystem" which creates the mapped file saved, also allow "load" in that mode. So have special file for it.


/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** Enable Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/


// #define DEVICE_COLORADO26__LIGHT01__HALLOWEEN
// #define DEVICE_COLORADO26__LIGHT02__HALLOWEEN
// #define DEVICE_COLORADO26__LIGHT03__HALLOWEEN
// #define DEVICE_COLORADO26__BEDROOM__HUB75E_ESP32S3_N16R8_01
// #define DEVICE_COLORADO26__BEDROOM__HUB75E_ESP32S3_N16R8_02
#define DEVICE_COLORADO26__MEADOWS__ICICLE_HUB75

/**
 * String of 100 behind TV
 * Use: Nightlight only 
 * Status: On all the time
 * Will enter standby mode, as red, darkest.
 * Motion of PIR will turn them on for 1 minutes, then 1 minute fade to standby.
**/
// #define DEVICE_COLORADO26__MOTION_STANDBY_LED_STRING_01

/**
 * WEBCAM 1-3, other page
 * LOCATION: At fridge, facing stairs
 * LOCATION: Behind bed outside, facing mountains
 * LOCATION: ?
**/
// #define DEVICE_TESTBED_WEBCAM_ARDUINO

/***
 * GPS Devices: 
 * * GPS + 9axis, to nextion panel, just do multiline mode? or full UI.
 * Have it log to an SD card, which I can open on my computer later with a NDJSON->GPX conversion
 * LOCATION: Make for car, use my battery back.
 * 
 * 
 * GPS?
 * - Serial GPS to esp32
 * -- Create poll method
 * -- Create interrupt driven method (using uart module)
 * -- NMEA method
 * -- UBLOX method
 * -- TTL UART connectors so GUI can configure and sniff. Where is my GPS sniffer build (used to be in my room) */
// Yes, lets make a new board "GPS+Motion+SD Logger"
// Ie get my 2 9-axis DMOF boards, and have them write to an sd card. Make 2? 
// This would let me get the sd card file to google earth pro pipeline, and python reading working.
// Test in CO, but should be a new device.





/**************************************************************************************************************************************************
***************************************************************************************************************************************************
****** DEVICE Defines*******************************************************************************************************************************
****************************************************************************************************************************************************
*******************************************************************************************************************************************/



#ifdef DEVICE_COLORADO26__LIGHT01__HALLOWEEN

  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR "col26__light01__halloween"
  #endif

  #ifndef DEVICENAME_FRIENDLY_CTR
    #define DEVICENAME_FRIENDLY_CTR "Colorado Halloween 100 LEDs#Outside"
  #endif

  #ifndef DEVICENAME_DESCRIPTION_CTR
    #define DEVICENAME_DESCRIPTION_CTR "Colorado Halloween;Optional SoftAP"
  #endif

  #define DEVICENAME_ROOMHINT_CTR "Colorado"

  // =========================================================
  // Network
  // =========================================================

  #define USE_NETWORK_TEMPLATE
  #define USE_NETWORK_TEMPLATE__OVERRIDE

  DEFINE_PGM_CTR(NETWORK_TEMPLATE)
  "{"
    "\"Version\":1,"

    "\"Interface\":{"
      "\"Policy\":{"
        "\"PreferOrder\":[\"Ethernet\",\"WiFi\",\"Cellular\"],"
        "\"AllowMultipleActive\":true,"
        "\"BlockRemoteMqttWhenLocalAvailable\":true"
      "}"
    "},"

    "\"WiFi\":{"
      "\"EN\":true,"
      "\"Backoff\":[5,60,600],"

      "\"Mode\":{"
        "\"STA\":true,"
        "\"AP\":true,"
        "\"STA_AP\":true,"
        "\"APBootMins\":10,"
        "\"APOnSTAFail\":true,"
        "\"APFailDelayMins\":0,"
        "\"APAlwaysOn\":true"
      "},"

      "\"Station\":{"
        "\"Profiles\":["
          "{"
            "\"SSID\":\"" STA_SSID1 "\","
            "\"Password\":\"" STA_PASS1 "\""
          "}"
        "],"

        "\"IPv4\":{"
          "\"Static\":false,"
          "\"IP\":\"0.0.0.0\","
          "\"Gateway\":\"0.0.0.0\","
          "\"Subnet\":\"0.0.0.0\","
          "\"DNS1\":\"0.0.0.0\","
          "\"DNS2\":\"0.0.0.0\""
        "}"
      "},"

      "\"SoftAP\":{"
        "\"SSID\":\"" DEVICENAME_CTR "\","
        "\"Password\":\"Michael26\","
        "\"Channel\":1"
      "}"
    "},"

    "\"Ethernet\":{"
      "\"EN\":false"
    "},"

    "\"MQTT\":{"
      "\"EN\":true,"

      "\"UpdateSeconds\":{"
        "\"IfChanged\":1,"
        "\"TelePeriod\":60,"
        "\"ConfigPeriod\":60"
      "},"

      "\"Brokers\":["
        "{"
          "\"Id\":\"home\","
          "\"EN\":true,"
          "\"Host\":\"" MQTT_HOST "\","
          "\"Port\":" STR(MQTT_PORT) ","
          "\"User\":\"\","
          "\"Password\":\"\","
          "\"TopicPrefix\":\"" DEVICENAME_CTR "\","
          "\"ClientName\":\"" DEVICENAME_CTR "\","
          "\"Backoff\":[5,10,60],"
          "\"Transport\":[\"WiFi\"],"
          "\"PrefTransport\":[\"WiFi\"],"
          "\"OutgoingLevel\":3,"
          "\"OutgoingLimiterMs\":0"
        "}"
      "]"
    "}"
  "}";

  // =========================================================
  // Lighting: 100 WS2812 LEDs, GPIO13
  // =========================================================

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GBR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "Name":"Halloween",
        "PixelRange":[0,100],
        "ColourPalette":"Holloween OPY",
        "ColourType":3,
        "Effects":{
          "Function":"Candles",
          "Speed":88,
          "Intensity":255,
          "Grouping":1,
          "RateMs":20
        },
        "BrightnessRGB":100
      }
    ],
    "BrightnessRGB":100
  }
  )=====";

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"0\":\"" D_GPIO_KEY1_INV_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif


#ifdef DEVICE_COLORADO26__LIGHT02__HALLOWEEN

  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR "col26__light02__halloween"
  #endif

  #ifndef DEVICENAME_FRIENDLY_CTR
    #define DEVICENAME_FRIENDLY_CTR "Colorado Halloween 100 LEDs#Outside"
  #endif

  #ifndef DEVICENAME_DESCRIPTION_CTR
    #define DEVICENAME_DESCRIPTION_CTR "Colorado Halloween;Optional SoftAP"
  #endif

  #define DEVICENAME_ROOMHINT_CTR "Colorado"

  // =========================================================
  // Network
  // =========================================================

  #define USE_NETWORK_TEMPLATE
  #define USE_NETWORK_TEMPLATE__OVERRIDE

  DEFINE_PGM_CTR(NETWORK_TEMPLATE)
  "{"
    "\"Version\":1,"

    "\"Interface\":{"
      "\"Policy\":{"
        "\"PreferOrder\":[\"Ethernet\",\"WiFi\",\"Cellular\"],"
        "\"AllowMultipleActive\":true,"
        "\"BlockRemoteMqttWhenLocalAvailable\":true"
      "}"
    "},"

    "\"WiFi\":{"
      "\"EN\":true,"
      "\"Backoff\":[5,60,600],"

      "\"Mode\":{"
        "\"STA\":true,"
        "\"AP\":true,"
        "\"STA_AP\":true,"
        "\"APBootMins\":10,"
        "\"APOnSTAFail\":true,"
        "\"APFailDelayMins\":0,"
        "\"APAlwaysOn\":true"
      "},"

      "\"Station\":{"
        "\"Profiles\":["
          "{"
            "\"SSID\":\"" STA_SSID1 "\","
            "\"Password\":\"" STA_PASS1 "\""
          "}"
        "],"

        "\"IPv4\":{"
          "\"Static\":false,"
          "\"IP\":\"0.0.0.0\","
          "\"Gateway\":\"0.0.0.0\","
          "\"Subnet\":\"0.0.0.0\","
          "\"DNS1\":\"0.0.0.0\","
          "\"DNS2\":\"0.0.0.0\""
        "}"
      "},"

      "\"SoftAP\":{"
        "\"SSID\":\"" DEVICENAME_CTR "\","
        "\"Password\":\"Michael26\","
        "\"Channel\":1"
      "}"
    "},"

    "\"Ethernet\":{"
      "\"EN\":false"
    "},"

    "\"MQTT\":{"
      "\"EN\":true,"

      "\"UpdateSeconds\":{"
        "\"IfChanged\":1,"
        "\"TelePeriod\":60,"
        "\"ConfigPeriod\":60"
      "},"

      "\"Brokers\":["
        "{"
          "\"Id\":\"home\","
          "\"EN\":true,"
          "\"Host\":\"" MQTT_HOST "\","
          "\"Port\":" STR(MQTT_PORT) ","
          "\"User\":\"\","
          "\"Password\":\"\","
          "\"TopicPrefix\":\"" DEVICENAME_CTR "\","
          "\"ClientName\":\"" DEVICENAME_CTR "\","
          "\"Backoff\":[5,10,60],"
          "\"Transport\":[\"WiFi\"],"
          "\"PrefTransport\":[\"WiFi\"],"
          "\"OutgoingLevel\":3,"
          "\"OutgoingLimiterMs\":0"
        "}"
      "]"
    "}"
  "}";

  // =========================================================
  // Lighting: 100 WS2812 LEDs, GPIO13
  // =========================================================

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GBR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "Name":"Halloween",
        "PixelRange":[0,100],
        "ColourPalette":"Holloween OP",
        "ColourType":3,
        "Effects":{
          "Function":"Candles",
          "Speed":88,
          "Intensity":255,
          "Grouping":1,
          "RateMs":20
        },
        "BrightnessRGB":100
      }
    ],
    "BrightnessRGB":100
  }
  )=====";


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"0\":\"" D_GPIO_KEY1_INV_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif




#ifdef DEVICE_COLORADO26__LIGHT03__HALLOWEEN

  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR "col26__light03__halloween"
  #endif

  #ifndef DEVICENAME_FRIENDLY_CTR
    #define DEVICENAME_FRIENDLY_CTR "Colorado Halloween 100 LEDs#Outside"
  #endif

  #ifndef DEVICENAME_DESCRIPTION_CTR
    #define DEVICENAME_DESCRIPTION_CTR "Colorado Halloween;Optional SoftAP"
  #endif

  #define DEVICENAME_ROOMHINT_CTR "Colorado"

  // =========================================================
  // Network
  // =========================================================

  #define USE_NETWORK_TEMPLATE
  #define USE_NETWORK_TEMPLATE__OVERRIDE

  DEFINE_PGM_CTR(NETWORK_TEMPLATE)
  "{"
    "\"Version\":1,"

    "\"Interface\":{"
      "\"Policy\":{"
        "\"PreferOrder\":[\"Ethernet\",\"WiFi\",\"Cellular\"],"
        "\"AllowMultipleActive\":true,"
        "\"BlockRemoteMqttWhenLocalAvailable\":true"
      "}"
    "},"

    "\"WiFi\":{"
      "\"EN\":true,"
      "\"Backoff\":[5,60,600],"

      "\"Mode\":{"
        "\"STA\":true,"
        "\"AP\":true,"
        "\"STA_AP\":true,"
        "\"APBootMins\":10,"
        "\"APOnSTAFail\":true,"
        "\"APFailDelayMins\":0,"
        "\"APAlwaysOn\":true"
      "},"

      "\"Station\":{"
        "\"Profiles\":["
          "{"
            "\"SSID\":\"" STA_SSID1 "\","
            "\"Password\":\"" STA_PASS1 "\""
          "}"
        "],"

        "\"IPv4\":{"
          "\"Static\":false,"
          "\"IP\":\"0.0.0.0\","
          "\"Gateway\":\"0.0.0.0\","
          "\"Subnet\":\"0.0.0.0\","
          "\"DNS1\":\"0.0.0.0\","
          "\"DNS2\":\"0.0.0.0\""
        "}"
      "},"

      "\"SoftAP\":{"
        "\"SSID\":\"" DEVICENAME_CTR "\","
        "\"Password\":\"Michael26\","
        "\"Channel\":1"
      "}"
    "},"

    "\"Ethernet\":{"
      "\"EN\":false"
    "},"

    "\"MQTT\":{"
      "\"EN\":true,"

      "\"UpdateSeconds\":{"
        "\"IfChanged\":1,"
        "\"TelePeriod\":60,"
        "\"ConfigPeriod\":60"
      "},"

      "\"Brokers\":["
        "{"
          "\"Id\":\"home\","
          "\"EN\":true,"
          "\"Host\":\"" MQTT_HOST "\","
          "\"Port\":" STR(MQTT_PORT) ","
          "\"User\":\"\","
          "\"Password\":\"\","
          "\"TopicPrefix\":\"" DEVICENAME_CTR "\","
          "\"ClientName\":\"" DEVICENAME_CTR "\","
          "\"Backoff\":[5,10,60],"
          "\"Transport\":[\"WiFi\"],"
          "\"PrefTransport\":[\"WiFi\"],"
          "\"OutgoingLevel\":3,"
          "\"OutgoingLimiterMs\":0"
        "}"
      "]"
    "}"
  "}";

  // =========================================================
  // Lighting: 100 WS2812 LEDs, GPIO13
  // =========================================================

  #define USE_LIGHTING_TEMPLATE

  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GBR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "Name":"Halloween",
        "PixelRange":[0,100],
        "ColourPalette":"Holloween OPY",
        "ColourType":3,
        "Effects":{
          "Function":"Candles",
          "Speed":88,
          "Intensity":255,
          "Grouping":1,
          "RateMs":20
        },
        "BrightnessRGB":100
      }
    ],
    "BrightnessRGB":100
  }
  )=====";


  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"0\":\"" D_GPIO_KEY1_INV_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif




/**
 * @brief
 *
 * Office HUB75E Test Panel
 * ESP32-S3 N16R8
 *
 * HUB75E 128x64 panel
 * 1/32 scan
 * 8192 logical pixels
 *
 * Physical HUB75 signals are hard-coded in BusHub75Matrix.
 * The lighting BusConfig "Pin" array is NOT GPIO numbering for HUB75.
 *
 * BusConfig Pin[] semantics:
 *
 *   Pin[0] -> Panel width
 *   Pin[1] -> Panel height
 *   Pin[2] -> Chain length
 *   Pin[3] -> Panel rows
 *   Pin[4] -> Panel columns
 *
 * For this panel:
 *
 *   "Pin":[128,64,1,1,1]
 *
 *
 * HUB75_I2S_CFG::i2s_pins order
 *
 *   R1, G1, B1,
 *   R2, G2, B2,
 *   A, B, C, D, E,
 *   LAT, OE, CLK
 *
 *
 *                              ESP32-S3 N16R8
 *
 *                          _______________________
 *                    3V3  |3V3                GND | GND
 *                    3V3  |3V3              GPIO43| UART0 TX
 *                    RST  |RST              GPIO44| UART0 RX
 *                      E  GPIO4             GPIO1 | spare
 *                      B  GPIO5             GPIO2 | spare
 *                      D  GPIO6            GPIO42 | JTAG / spare
 *                    LAT  GPIO7            GPIO41 | JTAG / spare
 *                     B2  GPIO8            GPIO40 | JTAG / spare
 *                     R2  GPIO9            GPIO39 | JTAG / spare
 *                     B1 GPIO10            GPIO38 | spare
 *                     R1 GPIO11            GPIO37 | MEM / unavailable
 *                     G1 GPIO12            GPIO36 | MEM / unavailable
 *                     G2 GPIO13            GPIO35 | MEM / unavailable
 *                  spare GPIO14             GPIO0 | STRAP fL / BOOT
 *                     OE GPIO15            GPIO45 | STRAP fH
 *                    CLK GPIO16            GPIO48 | spare
 *                      C GPIO17            GPIO47 | BIL RGB LED / spare
 *                      A GPIO18            GPIO21 | spare
 *         5V INPUT ONLY   |5V0          ^     GND | GND
 *                    GND  |GND  |USB| |COM|   GND | GND
 *                          _______________________
 *
 *
 * HUB75 signal allocation
 *
 * Colour data
 *
 *   R1  -> GPIO11
 *   G1  -> GPIO12
 *   B1  -> GPIO10
 *
 *   R2  -> GPIO9
 *   G2  -> GPIO13
 *   B2  -> GPIO8
 *
 * Row address
 *
 *   A   -> GPIO18
 *   B   -> GPIO5
 *   C   -> GPIO17
 *   D   -> GPIO6
 *   E   -> GPIO4
 *
 * Control
 *
 *   LAT -> GPIO7
 *   OE  -> GPIO15
 *   CLK -> GPIO16
 *
 *
 * Driver GPIO order
 *
 *   mxconfig.gpio = {
 *     11, 12, 10,       // R1, G1, B1
 *      9, 13,  8,       // R2, G2, B2
 *     18,  5, 17, 6, 4, // A, B, C, D, E
 *      7, 15, 16        // LAT, OE, CLK
 *   };
 *
 *
 * HUB75 connector mapping
 *
 *   R1  -> GPIO11
 *   G1  -> GPIO12
 *   B1  -> GPIO10
 *   GND -> GND
 *
 *   R2  -> GPIO9
 *   G2  -> GPIO13
 *   B2  -> GPIO8
 *   E   -> GPIO4
 *
 *   A   -> GPIO18
 *   B   -> GPIO5
 *   C   -> GPIO17
 *   D   -> GPIO6
 *
 *   CLK -> GPIO16
 *   LAT -> GPIO7
 *   OE  -> GPIO15
 *   GND -> GND
 *
 *
 * Reserved / deliberately avoided
 *
 *   GPIO0  -> STRAP fL / BOOT
 *   GPIO45 -> STRAP fH
 *   GPIO46 -> STRAP fH
 *
 *   GPIO19, GPIO20 -> Native USB
 *   GPIO35, GPIO36, GPIO37 -> Octal PSRAM / unavailable on N16R8
 *   GPIO39..GPIO42 -> Leave available for JTAG/debug
 *   GPIO43, GPIO44 -> UART0
 *
 *
 * Octal PSRAM pins exposed on this header
 *
 *   GPIO35 -> MEM / unavailable
 *   GPIO36 -> MEM / unavailable
 *   GPIO37 -> MEM / unavailable
 *
 *   GPIO38 -> usable
 *   GPIO39 -> usable
 *   GPIO40 -> usable
 *   GPIO41 -> usable
 *   GPIO42 -> usable
 *
 *
 * Useful spare GPIO
 *
 *   GPIO1
 *   GPIO2
 *   GPIO14
 *   GPIO21
 *   GPIO38
 *   GPIO48
 *
 *   GPIO39..GPIO42 -> usable, but deliberately kept free for JTAG/debug
 *   GPIO47 -> Built-in RGB LED; usable if onboard LED is not required
 *
 *
 * Power
 *
 *   Panel power is supplied separately at 5 V.
 *   ESP32 and panel grounds MUST be common.
 *
 *   Do not power the HUB75 panel through the ESP32 board 5 V pin.
 *
 *
 * Panel configuration
 *
 *   Resolution     -> 128 x 64
 *   Logical pixels -> 8192
 *   Scan rate      -> 1/32
 *   Interface      -> HUB75E
 * 
 * 01 is denser panel, so should use for 2d effects
 * 02 is on fridge, use for xmas26 testing. Both should use playlists for multiple testing
 *
 */
#ifdef DEVICE_COLORADO26__BEDROOM__HUB75E_ESP32S3_N16R8_01
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

  #define USE_MODULE_LIGHTS_HUB75
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__COMPLETE
  #define FIRMWARE_DEFAULT__LIGHTING_WEBUI__ADVANCED
  #define FIRMWARE_DEFAULT__WEBSERVER__ADVANCED
  #define FIRMWARE_DEFAULT__LIGHTING_EFFECTS__ALL
  #define BOARD_HAS_PSRAM




  #define ENABLE_FEATURE_LIGHTING__2D_MATRIX
  #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__SOUND_REACTIVE

  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL3_FLASHING_EXTENDED
  #define ENABLE_FEATURE_LIGHTING__EFFECTS__GENERAL_LEVEL4_FLASHING_COMPLETE


  // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__CHRISTMAS_MULTIFUNCTION_CONTROLLER_DEV
  // #define ENABLE_FEATURE_LIGHTING__EFFECTS__SPECIAL_HARDWARE_TESTING
  // #define ENABLE_FEATURE_LIGHTS__EFFECT_SPECIALISED__RAY_TRACING

  /***********************************
  * SECTION: Enable Grouped
  ************************************/  

  /***********************************
  * SECTION: Network Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   

#define RT_COVERAGE_DEBUG

#define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG
#define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG__CHIP_SM16208S

  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "Name":"HUB75E P2.5 CHIP",
        "BusType":"HUB75E",
        "ColourOrder":"GBR",
        "Start":0,
        "Length":8192
      }
    ],
    "MatrixConfig":[
      {
        "Width":128,
        "Height":64,
        "BottomStart":0,
        "RightStart":0,
        "Vertical":0,
        "Serpentine":0,
        "xOffset":0,
        "yOffset":0
      }
    ],
    "Segments":[
      {
        "PixelRange":[0,8192],
        "ColourPalette":"Rainbow",
        "ColourType":3,
        "Effects":{
          "Function":"Spaceships",
          "Speed":127,
          "Intensity":127,
          "Grouping":1,
          "RateMs":20
        },
        "BrightnessRGB":100
      }
    ],
    "BrightnessRGB":100
  }
  )=====";
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"0\":\"" D_GPIO_KEY1_INV_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

#endif // END DEVICE



/**
 * @brief
 *
 * Office HUB75E Test Panel
 * ESP32-S3 N16R8
 *
 * HUB75E 128x64 panel
 * 1/32 scan
 * 8192 logical pixels
 *
 * Physical HUB75 signals are hard-coded in BusHub75Matrix.
 * The lighting BusConfig "Pin" array is NOT GPIO numbering for HUB75.
 *
 * BusConfig Pin[] semantics:
 *
 *   Pin[0] -> Panel width
 *   Pin[1] -> Panel height
 *   Pin[2] -> Chain length
 *   Pin[3] -> Panel rows
 *   Pin[4] -> Panel columns
 *
 * For this panel:
 *
 *   "Pin":[128,64,1,1,1]
 *
 *
 * HUB75_I2S_CFG::i2s_pins order
 *
 *   R1, G1, B1,
 *   R2, G2, B2,
 *   A, B, C, D, E,
 *   LAT, OE, CLK
 *
 *
 *                              ESP32-S3 N16R8
 *
 *                          _______________________
 *                    3V3  |3V3                GND | GND
 *                    3V3  |3V3              GPIO43| UART0 TX
 *                    RST  |RST              GPIO44| UART0 RX
 *                      E  GPIO4             GPIO1 | spare
 *                      B  GPIO5             GPIO2 | spare
 *                      D  GPIO6            GPIO42 | JTAG / spare
 *                    LAT  GPIO7            GPIO41 | JTAG / spare
 *                     B2  GPIO8            GPIO40 | JTAG / spare
 *                     R2  GPIO9            GPIO39 | JTAG / spare
 *                     B1 GPIO10            GPIO38 | spare
 *                     R1 GPIO11            GPIO37 | MEM / unavailable
 *                     G1 GPIO12            GPIO36 | MEM / unavailable
 *                     G2 GPIO13            GPIO35 | MEM / unavailable
 *                  spare GPIO14             GPIO0 | STRAP fL / BOOT
 *                     OE GPIO15            GPIO45 | STRAP fH
 *                    CLK GPIO16            GPIO48 | spare
 *                      C GPIO17            GPIO47 | BIL RGB LED / spare
 *                      A GPIO18            GPIO21 | spare
 *         5V INPUT ONLY   |5V0          ^     GND | GND
 *                    GND  |GND  |USB| |COM|   GND | GND
 *                          _______________________
 *
 *
 * HUB75 signal allocation
 *
 * Colour data
 *
 *   R1  -> GPIO11
 *   G1  -> GPIO12
 *   B1  -> GPIO10
 *
 *   R2  -> GPIO9
 *   G2  -> GPIO13
 *   B2  -> GPIO8
 *
 * Row address
 *
 *   A   -> GPIO18
 *   B   -> GPIO5
 *   C   -> GPIO17
 *   D   -> GPIO6
 *   E   -> GPIO4
 *
 * Control
 *
 *   LAT -> GPIO7
 *   OE  -> GPIO15
 *   CLK -> GPIO16
 *
 *
 * Driver GPIO order
 *
 *   mxconfig.gpio = {
 *     11, 12, 10,       // R1, G1, B1
 *      9, 13,  8,       // R2, G2, B2
 *     18,  5, 17, 6, 4, // A, B, C, D, E
 *      7, 15, 16        // LAT, OE, CLK
 *   };
 *
 *
 * HUB75 connector mapping
 *
 *   R1  -> GPIO11
 *   G1  -> GPIO12
 *   B1  -> GPIO10
 *   GND -> GND
 *
 *   R2  -> GPIO9
 *   G2  -> GPIO13
 *   B2  -> GPIO8
 *   E   -> GPIO4
 *
 *   A   -> GPIO18
 *   B   -> GPIO5
 *   C   -> GPIO17
 *   D   -> GPIO6
 *
 *   CLK -> GPIO16
 *   LAT -> GPIO7
 *   OE  -> GPIO15
 *   GND -> GND
 *
 *
 * Reserved / deliberately avoided
 *
 *   GPIO0  -> STRAP fL / BOOT
 *   GPIO45 -> STRAP fH
 *   GPIO46 -> STRAP fH
 *
 *   GPIO19, GPIO20 -> Native USB
 *   GPIO35, GPIO36, GPIO37 -> Octal PSRAM / unavailable on N16R8
 *   GPIO39..GPIO42 -> Leave available for JTAG/debug
 *   GPIO43, GPIO44 -> UART0
 *
 *
 * Octal PSRAM pins exposed on this header
 *
 *   GPIO35 -> MEM / unavailable
 *   GPIO36 -> MEM / unavailable
 *   GPIO37 -> MEM / unavailable
 *
 *   GPIO38 -> usable
 *   GPIO39 -> usable
 *   GPIO40 -> usable
 *   GPIO41 -> usable
 *   GPIO42 -> usable
 *
 *
 * Useful spare GPIO
 *
 *   GPIO1
 *   GPIO2
 *   GPIO14
 *   GPIO21
 *   GPIO38
 *   GPIO48
 *
 *   GPIO39..GPIO42 -> usable, but deliberately kept free for JTAG/debug
 *   GPIO47 -> Built-in RGB LED; usable if onboard LED is not required
 *
 *
 * Power
 *
 *   Panel power is supplied separately at 5 V.
 *   ESP32 and panel grounds MUST be common.
 *
 *   Do not power the HUB75 panel through the ESP32 board 5 V pin.
 *
 *
 * Panel configuration
 *
 *   Resolution     -> 128 x 64
 *   Logical pixels -> 8192
 *   Scan rate      -> 1/32
 *   Interface      -> HUB75E
 *
 */
#ifdef DEVICE_COLORADO26__BEDROOM__HUB75E_ESP32S3_N16R8_02
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
  * SECTION: Enable Grouped
  ************************************/  

  #define ENABLE_LIGHTING_TEMPLATE__HUB75_AS_2D
  // #define ENABLE_LIGHTING_TEMPLATE__HUB75_AS_1D

  /***********************************
  * SECTION: Network Configs
  ************************************/  

  /***********************************
   * SECTION: Lighting Configs
  ************************************/   


  #ifdef ENABLE_LIGHTING_TEMPLATE__HUB75_AS_1D

    #define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG
    #define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG__CHIP_ICN2038S

    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
    R"=====(
    {
      "BusConfig":[
        {
          "Name":"HUB75E P2 CHIP",
          "BusType":"HUB75E",
          "ColourOrder":"BGR",
          "Start":0,
          "Length":8192
        }
      ],
      "Segments":[
        {
          "PixelRange":[0,8192],
          "ColourPalette":"Rainbow",
          "ColourType":3,
          "Effects":{
            "Function":"PS Ballbit",
            "Speed":127,
            "Intensity":127,
            "Grouping":1,
            "RateMs":20
          },
          "BrightnessRGB":100,
          "BrightnessCCT":0
        }
      ],
      "BrightnessRGB":30,
      "BrightnessCCT":0
    }
    )=====";
  #endif // ENABLE_LIGHTING_TEMPLATE__HUB75_AS_2D

  #ifdef ENABLE_LIGHTING_TEMPLATE__HUB75_AS_2D

    #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D

    
    #define FIRMWARE_DEFAULT__LIGHTING_EFFECTS__ALL
    #define BOARD_HAS_PSRAM

    #define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG
    #define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG__CHIP_ICN2038S

    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
    R"=====(
    {
      "BusConfig":[
        {
          "Name":"HUB75E P2 CHIP",
          "BusType":"HUB75E",
          "ColourOrder":"BGR",
          "Start":0,
          "Length":8192
        }
      ],
      "MatrixConfig":[
        {
          "Width":128,
          "Height":64,
          "BottomStart":0,
          "RightStart":0,
          "Vertical":0,
          "Serpentine":0,
          "xOffset":0,
          "yOffset":0
        }
      ],
      "Segments":[
        {
          "PixelRange":[0,8192],
          "ColourPalette":"Rainbow",
          "ColourType":3,
          "Effects":{
            "Function":"PS Ballpit",
            "Speed":100,
            "Intensity":220,
            "Custom1":70,
            "Custom2":180,
            "Custom3":31,
            "Check3":1,
            "Grouping":1,
            "RateMs":20
          },
          "BrightnessRGB":100,
          "BrightnessCCT":0
        }
      ],
      "BrightnessRGB":30,
      "BrightnessCCT":0
    }
    )=====";
  #endif // ENABLE_LIGHTING_TEMPLATE__HUB75_AS_2D
      
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"0\":\"" D_GPIO_KEY1_INV_CTR  "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";


#endif // END DEVICE


/**
 * @brief Colorado 2026 - Meadows Icicle HUB75 Development
 *
 * ESP32-S3 N16R8 with HUB75E 128x64 panel.
 *
 * Icicle layout:
 *   5 strings
 *   20 drops per string
 *   10 pixels per drop
 *   100 columns x 10 rows = 1000 pixels
 *
 * HUB75 is used as a development display for the final
 * WS2812 icicle installation, using 5 parallel outputs.
 *
 * Physical HUB75 matrix: 128x64
 * Logical icicle segment: 100x10
 */
#ifdef DEVICE_COLORADO26__MEADOWS__ICICLE_HUB75
  #ifndef DEVICENAME_CTR
  #define DEVICENAME_CTR "co26_meadows_icicle_hub75"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
  #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
  #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "Meadows"

  /***********************************
  * SECTION: Enable Grouped
  ************************************/
  #define ENABLE_LIGHTING_TEMPLATE__HUB75_AS_2D
  // #define ENABLE_LIGHTING_TEMPLATE__HUB75_AS_1D

  /***********************************
  * SECTION: Network Configs
  ************************************/

  /***********************************
  * SECTION: Lighting Configs
  ************************************/
  #ifdef ENABLE_LIGHTING_TEMPLATE__HUB75_AS_2D

    #define FIRMWARE_DEFAULT__LIGHTING_CONFIG__2D
    #define FIRMWARE_DEFAULT__LIGHTING_EFFECTS__ALL
    #define BOARD_HAS_PSRAM

    #define ENABLE_FEATURE_LIGHTING__WEBSOCKETS

    #define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG
    #define ENABLE_DEVFEATURE_LIGHTS__HUB75_CONFIG__CHIP_ICN2038S

    #define USE_LIGHTING_TEMPLATE
    DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
    R"=====(
    {
      "BusConfig":[
        {
          "Name":"HUB75E Icicle Development",
          "BusType":"HUB75E",
          "ColourOrder":"BGR",
          "Start":0,
          "Length":8192
        }
      ],
      "MatrixConfig":[
        {
          "Width":128,
          "Height":64,
          "BottomStart":0,
          "RightStart":0,
          "Vertical":0,
          "Serpentine":0,
          "xOffset":0,
          "yOffset":0
        }
      ],
      "Segments":[
        {
          "Name":"Icicles 100x10",
          "PixelRange":[0,8192],
          "ColourPalette":"Rainbow",
          "ColourType":3,
          "Effects":{
            "Function":"PS Ballbit",
            "Speed":127,
            "Intensity":127,
            "Grouping":1,
            "RateMs":20
          },
          "BrightnessRGB":100,
          "BrightnessCCT":0
        }
      ],
      "BrightnessRGB":30,
      "BrightnessCCT":0
    }
    )=====";
  #endif // ENABLE_LIGHTING_TEMPLATE__HUB75_AS_2D

  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)
  "{"
    "\"" D_NAME         "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"0\":\"" D_GPIO_KEY1_INV_CTR "\""
    "},"
    "\"" D_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";
#endif // END DEVICE




#ifdef DEVICE_COLORADO26__MOTION_STANDBY_LED_STRING_01
  #ifndef DEVICENAME_CTR
    #define DEVICENAME_CTR "col26__motion_standby_01"
  #endif
  #ifndef DEVICENAME_FRIENDLY_CTR
    #define DEVICENAME_FRIENDLY_CTR DEVICENAME_CTR
  #endif
  #ifndef DEVICENAME_DESCRIPTION_CTR
    #define DEVICENAME_DESCRIPTION_CTR DEVICENAME_FRIENDLY_CTR
  #endif
  #define DEVICENAME_ROOMHINT_CTR "Colorado"

  /***********************************
   * SECTION: System Debug Options
  ************************************/
  // #define ENABLE_ADVANCED_DEBUGGING
  // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
  // #define ENABLE_DEBUG_FUNCTION_NAMES
  // #define ENABLE_DEBUG_LINE_HERE_TRACE
  // #define ENABLE_FREERAM_APPENDING_SERIAL
  // #define ENABLE_DEBUGFEATURE_TASKER__DEVELOPMENT_TASKS__ANIMATOR
  // #define ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM

  /***********************************
   * SECTION: Lighting Template
  ************************************/
  #define USE_LIGHTING_TEMPLATE
  DEFINE_PGM_CTR(LIGHTING_TEMPLATE)
  R"=====(
  {
    "BusConfig":[
      {
        "Pin":13,
        "ColourOrder":"GBR",
        "BusType":"WS2812_RGB",
        "Start":0,
        "Length":100
      }
    ],
    "Segments":[
      {
        "Name":"Bus 1",
        "PixelRange":[0,100],
        "ColourPalette":"Warm White",
        "ColourType":3,
        "Effects":{
          "Function":"Static",
          "Speed":0,
          "Intensity":85,
          "Grouping":1,
          "RateMs":1000
        },
        "BrightnessRGB":100
      }
    ],
    "BrightnessRGB":1
  }
  )=====";

  /***********************************
   * SECTION: Standby Template
  ************************************/
  #define USE_STANDBY_TEMPLATE
  #define LIGHTING_STANDBY_TEMPLATE_ID 1
  DEFINE_PGM_CTR(STANDBY_TEMPLATE)
  R"=====(
  {
    "Version":3,
    "TemplateID":1,
    "Arm":true,
    "AwakeSecs":60,
    "StandbySecs":0,
    "WakeTransitionSecs":1,
    "StandbyTransitionSecs":2,
    "LeaveMode":"Target",
    "LeaveTargetID":10,
    "Targets":[
      {
        "Enabled":true,
        "Start":"00:00",
        "End":"00:00",
        "TargetID":5
      }
    ]
  }
  )=====";

  /***********************************
   * SECTION: Sensor Configs
  ************************************/
  #define ENABLE_TEMPLATE_SECTION__SENSORS__MOTION

  #if defined(ENABLE_TEMPLATE_SECTION__SENSORS__MOTION)
    #define USE_MODULE_SENSORS_INTERFACE
    #define USE_MODULE_SENSORS_PIR
  #endif

  #define PIXEL_LIGHT_SENSOR__DIGITAL_PIN 16
  #define PIXEL_LIGHT_SENSOR__DIGITAL_ACTIVE_LOW

  /***********************************
   * SECTION: Module Template
  ************************************/
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE)
  "{"
    "\"" D_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_GPIO_NUMBER "\":{"
      "\"15\":\"" D_GPIO_PIR_1_CTR "\","
      "\"2\":\"" D_GPIO_LED1_CTR "\""
    "},"
    "\"" D_BASE "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
    "\"" D_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
  "}";

  /***********************************
   * SECTION: Function Template
  ************************************/
  #define D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "Hallway"

  #define USE_FUNCTION_TEMPLATE
  DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
  "{"
    "\"" D_DEVICENAME "\":{"
      "\"" D_MODULE__SENSORS__SUN_TRACKING__CTR "\":["
        "\"Roaming\""
      "],"
      "\"" D_MODULE__SENSORS__PIR__CTR "\":["
        "\"" D_DEVICE_SENSOR_MOTION0_FRIENDLY_NAME_LONG "\""
      "]"
    "}"
  "}";

  /***********************************
   * SECTION: Rules Template
  ************************************/
  #define D_RULES_DELAY_LOAD_FROM_BOOT_TIME_SECOND 15
  // #define USE_RULES_TEMPLATE
  // DEFINE_PGM_CTR(RULES_TEMPLATE)
  // R"=====(
  // {
  //   "Rules":[
  //     {
  //       "Name":"PIR Wake Standby",
  //       "Trigger":{
  //         "Module":"motion",
  //         "Function":"MotionStarted",
  //         "DeviceName":0,
  //         "State":1
  //       },
  //       "Command":{
  //         "Module":"pixels",
  //         "Function":"SetPower",
  //         "DeviceName":0,
  //         "State":"Follow",
  //         "JsonCommands":"{\"Standby\":{\"Wake\":1}}"
  //       }
  //     }
  //   ]
  // }
  // )=====";

  #define ENABLE_DEVFEATURE_PIR__TRIGGERING_WITH_RULES

#endif // DEVICE_COLORADO26__MOTION_STANDBY_LED_STRING_01



#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_CHRISTMAS23_H
