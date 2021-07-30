#ifndef MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_MEASUREMENT_H
#define MSYSTEMCONFIG_HARDWAREDEFAULTS_FIRMWAR_MIC4HAEL2_MEASUREMENT_H

/*********************************************************************************************\
 * This file is one of two user configurable files
 * 
 * "mFirmwareCustom.h" allows advanced users to create a custom build of firmware tailored to a specefic tasks 
 * For example, using a device to control heating, which includes numorous sensors, relays and controls that are unique.
 * This can also include lights or displays as notification/information systems. 
 * Although all sensors/drivers are accesible through default builds, this allows users to config and add their own controllers.
 * Ideally, when possible, templates can be used to replicate the custom firmware options performed here.
\*********************************************************************************************/

// #define DEVICE_FORCED_TO_BE_TESTER
#define DISABLE_WEBSERVER
#define FORCE_TEMPLATE_LOADING

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/Languages/mLanguageDefault.h"
#include "6_Lights/_Interface/mInterfaceLight_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"


/**
 * First measurement system, without esp adc until I can resolve that
 * This will be used for first aplha tests to plan things out
 * Future system will attempt to include esp adc later
 * 
 * Description: Use of button, oled and sdcard together on LIPO_ESP32
 * Button will toggle logging start/end (open/close), which will write the RTC time line by line. This will let me test sd opening and closing when expected
 * Test version before adding pic32 stream of data (although will use same format)
 * Aim: To log GPS data at 10hz, to sd card, and have me drive around and get that 3d fix saved and into matlab then google earth
 * 
 * GPS @ 912800 baud recorded to local fix
 * - each new fix, triggers a "save" flag
 * OLED screen will show bytes written every second
 * No networking
 * 
 * 
 * 
 * */




/**
 * 4 Measurement recorders
 * Node0 - UAV
 * Node1 - OffUAVa
 * Node2 - OffUAVb
 * Node2r - Receive only test module for I2S sampling
 * */

// //--------------------------------[Enable Device]-------------------------------------

/**
 * PhD systems
 * */
// #define DEVICE_NODE0_UAV_PIC32ADC
#define DEVICE_NODE1_OFFUAV_PIC32ADC
// #define DEVICE_NODE2_OFFUAV_PIC32ADC
// #define DEVICE_NODE2R_OFFUAV_PIC32ADC_ESP32I2S

// //-----------------[User Defined Devices == USE_BUILD_TYPE_ENERGY == Any Energy Monitoring Firmware]-------------------------------------

#ifdef DEVICE_NODE1_OFFUAV_PIC32ADC
  #define DEVICENAME_CTR            "node1"
  #define DEVICENAME_FRIENDLY_CTR   "node1"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "ND0"

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
  #define USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
  #define USE_SYSTEM_GPS_INPUT_USING_RINGBUFFER_INTERRUPTS
  #define USE_SYSTEM_RSS_FROM_PIC32_INPUT_STREAM
  #define USE_SYSTEM_OLED_LOGGER_FEEDBACK
  #define USE_SYSTEM_LOGGER_CONTROLLER
  //#define USE_SYSTEM_SDCARD_LOGGING
  //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  /**
   * Debug methods
   * */
  /**
   * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
   * */
  #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT
  // #define USE_SYSTEM_ENABLE_DEBUG_OUTPUT_ON_SERIAL2
  #define USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17
  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  /**
   * If enabled, disable normal logging methods
   * */
  #ifdef USE_SYSTEM_OUTPUT_SDCARD_STREAM_TO_SERIAL0_FOR_FAST_TESTING
    #define DISABLE_SERIAL_LOGGING
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
    #define USE_SDCARD_RINGBUFFER_STEAM_OUT
  #endif // USE_SYSTEM_SDCARD_LOGGING

  /**
   * Logger Controller
   * */
  #ifdef USE_SYSTEM_LOGGER_CONTROLLER
    #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
    #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
    //#define GPIO_SYNC_FRAME_ISR_PIN 25
    #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
    #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
    #define GPIO_ADC_CAPTURE_ISR_PIN 32
  #endif // USE_SYSTEM_LOGGER_CONTROLLER

  /**
   * Button input
   * */
  #ifdef USE_SYSTEM_BUTTON_INPUT_LOGGER_TOGGLE
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

  #define DEBUG_PIN3_GPIO     26
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER


#ifdef DEVICE_NODE2_OFFUAV_PIC32ADC
  #define DEVICENAME_CTR            "node1"

  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

  #define SHOW_SPLASH

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  // #define USE_SYSTEM_BUTTON_TO_TOGGLE_SDCARD_LOGGING
  #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
  //#define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  /**
   * Debug methods
   * */
  /**
   * @note SD Card will still be enabled, but data will be pushed out of serial2(17) 
   * */
  #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT


  /**
   * SDCARD name
   * */
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17
  #define SERIAL_DEBUG_BAUD_DEFAULT 921600
  // #define SERIAL_DEBUG_BAUD_DEFAULT 115200

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // Section A: GPS
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

  // // Section UART
  #define USE_MODULE_DRIVERS_INTERFACE
  #define USE_MODULE_DRIVERS_SERIAL_UART
  #define ENABLE_HARDWARE_UART_1
  #define ENABLE_HARDWARE_UART_2
  #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

  // Section B: SDCard driver
  #define USE_MODULE_DRIVERS_SDCARD
  #define USE_SDCARD_RINGBUFFER_STEAM_OUT


  // Seciton C: Logger controller
  #ifdef USE_SYSTEM_MAIN_LOGGER_CONTROLLER
    #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
    #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
    #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
    // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
    // #define GPIO_ADC_CAPTURE_ISR_PIN 32
  #endif // USE_SYSTEM_MAIN_LOGGER_CONTROLLER


  // // // Seciton C: Logger controller
  // #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  // // #define ENABLE_SDLOGGER_APPEND_TIME_TEST
  // #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  // #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  // #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
  // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
  // #define GPIO_ADC_CAPTURE_ISR_PIN 32


  // // // Section x: Button to toggle logging/sd state
  // // #define USE_MODULE_CORE_RULES
  // // // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
  // // #define USE_MODULE_SENSORS_BUTTONS


  // /***
  //  * ESP32    wire colour       PIC32                     PURPOSE
  //  *  25             
  //  *  33
  //  *  32         red               D1                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SAMPLE_TIMESLOT_PERIOD
  //  *  35        yellow             B1 - RSS 5800            AD1_CH7
  //  *  34        white              D7                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
  //  * 
  //  * */
  //   #define ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD


  //   #define PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM 32    // This allows me to call these directly, instead of via pinmethod, so faster    
  //   #define PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM 33    // This allows me to call these directly, instead of via pinmethod, so faster
  //   #define PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM 26    // This allows me to call these directly, instead of via pinmethod, so faster
  //   #define PIN_GPIO_FUNCTION_RXON_SAMPLE_5800_ADC_NUM         35

  //   #define ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT


  // #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
  //   #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL

  // #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER



  // // Section x: Button to toggle logging/sd state
  #define USE_MODULE_CORE_RULES
  // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
  #define USE_MODULE_SENSORS_BUTTONS
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

  // Section x: OLED display 
  #define USE_MODULE_DISPLAYS_INTERFACE
  #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
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

  #define DEBUG_PIN3_GPIO     26
  #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  #define DEBUG_PIN4_GPIO     27
  #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER

/**
 * 
 * Test of high speed I2S measurements
 * 
 * How it works:
 * - sync pin will reset the item counter, which is an array that stores an ID number for each item in the ringbuffer
 *    - reading the item from the buffer, will also mean popping the value out of it. I will use a SECOND ringbuffer for just this purpose
 * - sync pin keeps track of which ringbuffer items belongs to each group, so I need a counter for sync (in a ringbuffer) and a counter for RXON state changes
 * 
 * - When RXON pin is active low, then the I2S samples will be commited to the ringbuffer
 * - The values per item (DMA read), will still be averaged in 10s, ie 1000 samples becomes 100 samples per item
 *
 * 
 * 
 * 
 * 
 * 
 * Record 1000 samples per syncframe (then sum the regions that would be measurements, or try offload for summing later in code)
 * On finish of sync, average the sample rate down to 10 times more than actual measurement rate, save these to sd if not too many
 * 
 * 
 * 
 * 

sync period will trigger the i2s to run, and when X samples are recorded, will stop... 
This will be known as a frame, I can post downsample and remove other things after



sample 5.8ghz with esp32
have sync pulse restart counter, so I know which "item" is the start of that sync window
during capture constant capture, only commit to memory when a pin is low, which signifies "active rxon period"
There should be 40 of these windows per sync

sync_counter = [0,0,0,0,0 1,1,1,,,,,,,, 40,40,40 ] where X for each number will be many samples, which I can post downsample in matlab



 * 
 * */
#ifdef DEVICE_TESTBED_I2S_MEASUREMENT_SINGLE_CHANNEL2
  #define DEVICENAME_CTR            "system_1_alpha"

  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  // #define DEVICENAME_FRIENDLY_CTR   "Device 2"
  #define DEVICENAME_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 

  /**
   * New defines to enable functions below in the way I need for them to work (ie cross enable tasks where needed)
   * */
  // #define USE_SYSTEM_BUTTON_TO_TOGGLE_SDCARD_LOGGING
  #define USE_SYSTEM_MAIN_LOGGER_CONTROLLER
  #define USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

  /**
   * Debug methods
   * */
  #define USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT

  /**
   * Since I have no comms from esp32 to pic32, I should use this for debugging (disconnect from esp32)
   * Just on the debug lipo-esp32 (front soldered)
   * Use this to send "SDCARD" data directly to PC to logging to file and parsing.
   * */

  /**
   * Required defines based on above defines
   * */
  #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT
    // #define USE_MODULE_DRIVERS_SERIAL_UART
  #endif



  /**
   * SDCARD name
   * */

  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN0"  //white wire, blue tape, to be uav 
  // #define DEVICENAME_FOR_SDCARD_FRIENDLY_CTR   "DN2"  //white wire, blue tape, to be uav 


  #define SERIAL_DEBUG_BAUD_DEFAULT 921600

  // Also defining so "HardwareSerial.cpp" will use these
  #define RX1 18
  #define TX1 19
  #define RX2 16
  #define TX2 17

  // General defines for debugging only, not for finished
  #define DISABLE_NETWORK
  #define DISABLE_SLEEP

  // General defines needed in release version
  #define ESP32
  #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

  // // // Section A: GPS
  // #define USE_MODULE_DRIVERS_GPS
  // #define ENABLE_GPS_PARSER_NMEA
  // #define ENABLE_GPS_PARSER_UBX
  // #define USE_DEVFEATURE_GPS_RINGBUFFER_CONFIGURATION_UBX
  // #define NMEAGPS_DERIVED_TYPES
  // #define ENABLE_DEVFEATURE_GPS_FROM_RINGBUFFERS
  // #define NMEAGPS_PARSE_SAVE_MILLIS
  // #define gpsPort Serial1
  // #define D_GPS_BAUD_RATE_FAST    921600
  // #define D_GPS_BAUD_RATE_DEFAULT 9600

  // // // Section UART
  // #define USE_MODULE_DRIVERS_INTERFACE
  // #define USE_MODULE_DRIVERS_SERIAL_UART
  // #define ENABLE_HARDWARE_UART_1
  // #define ENABLE_HARDWARE_UART_2
  // #define HARDWARE_UART_1_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
  // #define HARDWARE_UART_2_BAUD_RATE_SPEED  2048000
  // #define ENABLE_DEVFEATURE_DEBUG_PRINT_UART1_INPUT_STREAM_FROM_RINGBUFFER
  // #define ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
  // #define ENABLE_DEVFEATURE_RSS_UART2_RINGBUFFER_TYPE_NOSPLIT

  // // Section B: SDCard driver
  // #define USE_MODULE_DRIVERS_SDCARD
  // #define USE_SDCARD_RINGBUFFER_STEAM_OUT

  // // // Seciton C: Logger controller
  #ifdef USE_SYSTEM_MAIN_LOGGER_CONTROLLER
    #define USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    #define ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
    #define ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
    #define PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM 25
    // #define ENABLE_INTERRUPT_ON_LOW_PINX_FOR_ADC_CAPTURE
    // #define GPIO_ADC_CAPTURE_ISR_PIN 32
  #endif // USE_SYSTEM_MAIN_LOGGER_CONTROLLER

  // // Section x: Button to toggle logging/sd state
  // #define USE_MODULE_CORE_RULES
  // // #define USE_MODULE_SENSORS_INTERFACE // NOT NEEDED
  // #define USE_MODULE_SENSORS_BUTTONS


  /***
   * ESP32    wire colour       PIC32                     PURPOSE
   *  25             
   *  33
   *  32         red               D1                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SAMPLE_TIMESLOT_PERIOD
   *  35        yellow             B1 - RSS 5800            AD1_CH7
   *  34        white              D7                       PIN_OUTPUT_ESP32_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
   * 
   * */
    #define ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD


    #define PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM 32    // This allows me to call these directly, instead of via pinmethod, so faster    
    #define PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM 33    // This allows me to call these directly, instead of via pinmethod, so faster
    #define PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM 26    // This allows me to call these directly, instead of via pinmethod, so faster
    #define PIN_GPIO_FUNCTION_RXON_SAMPLE_5800_ADC_NUM         35

    #define ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT


  #ifdef USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER
    #define USE_MODULE_SENSORS_ADC_I2S_INTERNAL

  #endif // USE_SYSTEM_I2S_SINGLE_CHANNEL_SAMPLER

    
  // // Section x: OLED display 
  // #define USE_MODULE_DISPLAYS_INTERFACE
  // #define USE_MODULE_DISPLAYS_OLED_SSD1306
  
  #define USE_MODULE_TEMPLATE
  DEFINE_PGM_CTR(MODULE_TEMPLATE) 
  "{"
    "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
    "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
    "\"" D_JSON_GPIOC "\":{"
      /** 4P large JST - ADC
       * Yellow     34(I), ADC5G, ADC1_CH6
       * White      35(I), RXON SAMPLING ENABLED (ie region of sampling on)
       * Red        32(I), Start of Sync period (resetting this windows values)
       * Black      GND
       * 
       * */
      //"\"" STR2(PIN_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_NUM) "\":\"" D_GPIO_FUNCTION_CC1110_SYNC_PULSE_SIGNAL_CTR   "\","  // 33 = Sync period from cc1110, this is used to reset the sampler (maybe change to another ringbuffer?, have two, one per active and writer)
      //"\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR   "\","               // 5.8 GHz RSS 


//35 is the new ADC

     // "\"32\":\"" D_GPIO_FUNCTION_EXTERNAL_INTERRUPT_TRIGGER_CTR   "\","   // pic32 tells the device when a new sync window starts, I will probably use this for swapping buffers
      //"\"" STR2(PIN_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_NUM) "\":\"" D_GPIO_FUNCTION_RXON_SAMPLING_ENABLED_CTR   "\","  // When low, samples are collected. An interrupt will also be applied to it, where state change will increment a counter (ie new time slot)

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
  // #define DEBUG_PIN1_GPIO     21
  // #define DEBUG_PIN1_INIT()   pinMode(DEBUG_PIN1_GPIO, OUTPUT); digitalWrite(DEBUG_PIN1_GPIO, HIGH);
  // #define DEBUG_PIN1_SET(X)   digitalWrite(DEBUG_PIN1_GPIO, X);
  // #define DEBUG_PIN1_TOGGLE()   digitalWrite(DEBUG_PIN1_GPIO, !digitalRead(DEBUG_PIN1_GPIO));

  // #define DEBUG_PIN2_GPIO     22
  // #define DEBUG_PIN2_INIT()   pinMode(DEBUG_PIN2_GPIO, OUTPUT); digitalWrite(DEBUG_PIN2_GPIO, HIGH);
  // #define DEBUG_PIN2_SET(X)   digitalWrite(DEBUG_PIN2_GPIO, X);
  // #define DEBUG_PIN2_TOGGLE()   digitalWrite(DEBUG_PIN2_GPIO, !digitalRead(DEBUG_PIN2_GPIO));

  // #define DEBUG_PIN3_GPIO     26
  // #define DEBUG_PIN3_INIT()   pinMode(DEBUG_PIN3_GPIO, OUTPUT); digitalWrite(DEBUG_PIN3_GPIO, HIGH);
  // #define DEBUG_PIN3_SET(X)   digitalWrite(DEBUG_PIN3_GPIO, X);
  // #define DEBUG_PIN3_TOGGLE()   digitalWrite(DEBUG_PIN3_GPIO, !digitalRead(DEBUG_PIN3_GPIO));

  // #define DEBUG_PIN4_GPIO     27
  // #define DEBUG_PIN4_INIT()   pinMode(DEBUG_PIN4_GPIO, OUTPUT); digitalWrite(DEBUG_PIN4_GPIO, HIGH);
  // #define DEBUG_PIN4_SET(X)   digitalWrite(DEBUG_PIN4_GPIO, X);
  // #define DEBUG_PIN4_TOGGLE()   digitalWrite(DEBUG_PIN4_GPIO, !digitalRead(DEBUG_PIN4_GPIO));

  #define DEBUG_ADC_ISR_EVENT_SET(X)          DEBUG_PIN1_SET(X)
  //#define DEBUG_ADC_ISR_EVENT_SET(X)          // nothing


#endif // DEVICE_GPSPARSER_TESTER









#endif // MSYSTEMCONFIG_HARDWAREDEFAULTS_H

