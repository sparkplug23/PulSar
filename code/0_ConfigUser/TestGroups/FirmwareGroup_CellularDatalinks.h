#ifndef _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE2_TESTGROUPS_CELULLAR_DATALINKS_H
#define _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE2_TESTGROUPS_CELULLAR_DATALINKS_H

/*********************************************************************************************\
 * Developing/Testbeds: 
 *  ** Only devices that are actively being ran as test devices.
 * Aiming to move these into the garage on a shelf, with a camera looking at them. Everything should be OTA flashed and debugged.
\*********************************************************************************************/

#include "2_CoreSystem/mGlobalMacros.h"
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"

/**
 * @brief List of hardware, group L for lighting
 * 
 * Device L1: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single 100 micro leds (RGB)
 * 
 * Device L2:
 * - ESP32
 * - Dual Bus, Dual Segment as addressable
 * - 100 Micro + 16 RGB Ring
 * 
 * Device L3:
 * - ESP8266
 * - Single Bus, Single Segment
 * - Single 100 micro leds
 * 
 * Device L4: 
 * - H801 (ESP8266)
 * - Single RGBCCT Bus, 5 PWM
 * - Run as normal scene light
 * 
 * Device L5: 
 * - H801 (ESP8266)
 * - RGB Channels on Segment 0
 * - WW/CW Channels on Segment 1
 * 
 * Device L6: 
 * - H801 (ESP8266)
 * - 5 Split White channels
 * 
 * Device L7: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single 100 micro leds (RGB) on roof
 * 
 * Device L8: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L9: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L10: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L11: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 * Device L12: 
 * - ESP32
 * - Single Bus, Single Segment
 * - Single RGBW 60 Strip on metal holder   SK6812_RGBW
 * 
 */

//--------------------------------[Enable Device]-------------------------------------

//////////////////////// ESP32
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_1CH                                          // in room
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L2__ESP32_I2S_PARALLEL_4CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L14__ESP32_I2S_PARALLEL_8CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L14__ESP32_I2S_PARALLEL_16CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L16__ESP32_I2S_PARALLEL_RED_BOARD_4CH
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L10__ESP32__7SEGMENTCLOCK
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L12__ESP32_I2S_PARALLEL_4CH_INCLUDING_DEBUG_PINS       // Timing Tester

/////////////////////// ESP8266
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L3__ESP8266_1CH_RGB_100LEDS                            // in room
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L4__H801_5CH_PWM_RGBCCT                                // in room under desk to develop for h801
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L5__H801_3CHRGB_AND_2CHCW
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L6__H801_FIVE_1CH_WHITE_CHANNELS
// #define DEVICE_TESTGROUP__LIGHTING_EFFECTS__L9__ESP8266_WEBUI

// // #define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__2024
// #define DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_02__2024


// //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



// /**
//  * @brief Self contained 2024 testbed, with integrated battery for car tracking for testing
//  * 
//  */
// #ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_01__2024
//   #define DEVICENAME_CTR          "cellular_locator_01"
//   #define DEVICENAME_FRIENDLY_CTR "Selfcontained tracker with SIM staying" // Antennas covers SIM, but flip the board later
//   #define DEVICENAME_ROOMHINT_CTR "testbed"
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
//   #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator01"
//   #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
//   #define UART_CELLULAR_BAUD   921600
//   #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//   #define USE_GROUPFEATURE__MODEM_GPS
//   #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "10" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
//   #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "0"
//   #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH
// #endif
// /**
//  * @brief 2024 Device
//  * For testing lnog range devices, without a battery that uses the aircrafts power source. Last known will be used as tracking instead of constant updates.
//  * 
//  */
// #ifdef DEVICE_CELLULAR_LTE__GPS_POSITION_LOCATOR_02__2024
//   #define DEVICENAME_CTR          "cellular_locator_02"
//   #define DEVICENAME_FRIENDLY_CTR "Plane2024 - 1Hz position updater"
//   #define DEVICENAME_ROOMHINT_CTR "roaming"
//   #define D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED   "192.168.1.70"
//   #define D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "CellularLocator02"
//   #define SIM_CARD_PHONE_NUMBER "07518522105"
//   #define USE_GROUPFEATURE__MQTT_AS_CELLULAR
//   #define UART_CELLULAR_BAUD   115200
//   #define USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//   #define USE_GROUPFEATURE__MODEM_GPS
//   #define TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR  "1" // On plane this needs to be much faster, as signal comes and goes quicker. Dont worry about repeated reconnects
//   #define SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR "60"
//   #define SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024
// #endif
// #ifdef SUBDEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_MULTI_FLASH_2024


//   // #define ENABLE_ADVANCED_DEBUGGING
//   // #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//   // #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//   // #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 50
//   // #define ENABLE_DEBUG_FUNCTION_NAMES

  
//   // #define ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//   #define ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


//   #define ENABLE_FEATURE_WATCHDOG_TIMER
//     #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 120000
//   // #define ENABLE_DEVFEATURE_FASTBOOT_DETECTION
//   // #define ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID
//   //                                                               #define ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

//   #ifdef ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS
//     #define ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES
//     #define ENABLE_DEVFEATURE__MQTT_ENABLE_SENDING_LIMIT_MS 10    
//     #define ENABLE_DEVFEATURE__MQTT_SHOW_SENDING_LIMIT_DEBUT_MESSAGES    
//     // #define ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
//     #define ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//     #define ENABLE_DEBUG_FUNCTION_NAMES
//     #define ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS 20
//     // #define ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
//     #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_TASKS
//     #define ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
//     #define ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS 300
//     // #define ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
//     #define ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
//     #define ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
//     #define ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
//     // #define ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
//     // #define ENABLE_DEVFEATURE__NTP_OVER_CELLULAR_TEST_WITHOUT_INTERFACE_INTEGRATION
//     #define ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
//     // #define ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
//     #define ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS 600
//   #endif // ENABLE_GROUPFEATURE__TESTING_NEW_OPTIONS


//   #ifdef ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING
//     #define ENABLE_ADVANCED_DEBUGGING
//     #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
//   #endif // ENABLE_GROUPFEATURE__DEBUG_CONNECTION_WITH_LOGGING


//   #define ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE


//   // *************************************************************************************

//   #ifdef USE_GROUPFEATURE__FASTER_SERIAL_LOGGING
//     #define SERIAL_DEBUG_BAUD_DEFAULT 921600
//   #endif
 
//   #ifdef USE_GROUPFEATURE__MAVLINK_DECODER
//     #define USE_MODULE__DRIVERS_MAVLINK_DECODER
//       #define USE_FEATURE_SEARCH_FOR_UNHANDLED_MAVLINK_MESSAGES_ON_ALLOWEDLIST
//       #define ENABLE_FEATURE_MAVLINK_CONVERT_MQTT_DATA_VALUES
//       #define ENABLE_FEATURE_MAVLINK_MQTT_SEND_ALL_PACKETS_AS_TELEMETRY_TOPICS
//     #define USE_MODULE_CORE_SERIAL_UART
//       #define ENABLE_HARDWARE_UART_2
//       #define HARDWARE_UART_2_BAUD_RATE_SPEED  921600  //D_GPS_BAUD_RATE_FAST
//   #endif

//   #ifdef USE_GROUPFEATURE__MODEM_GPS
//     #define USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//     #define USE_MODULE_SENSORS_GPS_MODEM
//   #endif 
//   #ifdef USE_GROUPFEATURE__MQTT_AS_CELLULAR 
//     #define DISABLE_NETWORK_WIFI
//     #define USE_MODULE_NETWORK_CELLULAR
//     #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//     #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//     #define USE_MODULE_SENSORS_INTERFACE
//     #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//     #define USE_MODULE_NETWORK_MQTT
//     // #define MQTT_SOCKET_TIMEOUT 1
//     // #define MQTT_PORT 51884 // Temporary exposed primry broker : STABLE
//      #define MQTT_PORT_CELLULAR 51883 //external mqtt broker on TOWER  : Unstable 192.168.1.66
//     // #define ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
//     // #define ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
//     #define ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
//     // #define ENABLE_DEVFEATURE_CELLULAR_SMS__PDU_MODE  //no
//   #endif
//   #ifdef USE_GROUPFEATURE__MQTT_AS_WIFI
//     #define USE_MODULE_NETWORK_WIFI
//     #define JSON_VARIABLE_FLOAT_PRECISION_LENGTH 10
//     #define ENABLE_DEVFEATURE_MQTT_USING_WIFI
//     #define MQTT_HOST       D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED
//     #define MQTT_PORT     1883
//   #endif

//   #ifdef USE_GROUPFEATURE_CELLULAR_ONLY_FOR_SMS
//     #define DISABLE_NETWORK_WIFI
//     #define USE_MODULE_NETWORK_CELLULAR
//     #define USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
//     #define ENABLE_DEVFEATURE_DDNS_MQTT_TEST
//     // #define USE_MODULE_SENSORS_INTERFACE
//     // #define ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
//     // #define MQTT_PORT 51883 // Temporary exposed primry broker : STABLE
//     // #define ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
//   #endif


//   #define USE_MODULE_TEMPLATE
//   DEFINE_PGM_CTR(MODULE_TEMPLATE) 
//   "{"
//     "\"" D_JSON_NAME "\":\"" DEVICENAME_CTR "\","
//     "\"" D_JSON_FRIENDLYNAME "\":\"" DEVICENAME_FRIENDLY_CTR "\","
//     "\"" D_JSON_GPIO_NUMBER "\":{"
//       #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
//       "\"22\":\"" D_GPIO_FUNCTION_I2C_SCL_CTR   "\","
//       "\"21\":\"" D_GPIO_FUNCTION_I2C_SDA_CTR   "\","   
//       #endif // USE_MODULE_DISPLAYS_OLED_SH1106   
//       #ifdef USE_MODULE_NETWORK_CELLULAR
//       "\"25\":\"" D_GPIO_FUNCTION__MODEM_DATA_TERMINAL_READY_DTR__CTR   "\","
//       "\"27\":\"" D_GPIO_FUNCTION__MODEM_TX__CTR   "\","   
//       "\"26\":\"" D_GPIO_FUNCTION__MODEM_RX__CTR   "\","   
//       "\"4\":\""  D_GPIO_FUNCTION__MODEM_POWER__CTR   "\","   
//       #endif // USE_MODULE_NETWORK_CELLULAR   
//       #ifdef USE_MODULE_DRIVERS_SDCARD
//       "\"2\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MISO_CTR   "\","
//       "\"15\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_MOSI_CTR   "\","   
//       "\"14\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CLK_CTR   "\","
//       "\"13\":\"" D_GPIO_FUNCTION_SDCARD_HSPI_CSO_CTR   "\","  
//       #endif // USE_MODULE_DRIVERS_SDCARD   
//       #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
//       "\"19\":\"" D_GPIO_FUNCTION_HWSERIAL2_TX_CTR   "\","
//       "\"18\":\"" D_GPIO_FUNCTION_HWSERIAL2_RX_CTR   "\","   
//       #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER   
//       "\"12\":\"" D_GPIO_FUNCTION_LED1_INV_CTR "\","
//       "\"35\":\"" D_GPIO_FUNCTION_ADC1_CH7_CTR "\""
//     "},"
//     "\"" D_JSON_BASE     "\":\"" D_MODULE_NAME_USERMODULE_CTR "\","
//     "\"" D_JSON_ROOMHINT "\":\"" DEVICENAME_ROOMHINT_CTR "\""
//   "}";


//   #define USE_FUNCTION_TEMPLATE
//   DEFINE_PGM_CTR(FUNCTION_TEMPLATE)
//   "{"
//     "\"" D_JSON_DEVICENAME "\":{"
//       "\"" D_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
//       "],"
//       "\"" D_MODULE__SENSORS_BATTERY_MODEM__FRIENDLY_CTR "\":["
//         "\"" D_DEVICE_SENSOR_GPS_MODEM_FRIENDLY_NAME_LONG "\""
//       "]"
//     "},"   
//     "\"MQTTUpdateSeconds\":{\"IfChanged\":1,\"TelePeriod\":60,\"ConfigPeriod\":60},"   // if changed needs to be reconfigured so its only sent teleperiod amount, but flag is set when needed (rather than ischanged variables)
//     "\"SMSAuto_GPS\":"  SMS_AUTO_GPS_TIME_SECONDS_RATE_CTR ","
//     "\"MQTT\":{\"RetrySecs\":"  TEMP_MQTT_RECONNECT_SECOND_BACKOFF_CTR "}"
//   "}";

// #endif // DEVICE_TESTBED_ESP32_CELLULAR_LOCATOR_01




#endif // _CONFIG_USER_FIRMWARE_CUSTOM_SECRET_ACTIVE_TESTGROUPS_BETA_H

