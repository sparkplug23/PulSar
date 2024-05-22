#ifndef mInterfaceController_H2
#define mInterfaceController_H2 1.1

#define D_USER_MICHAEL

#ifdef D_USER_MICHAEL
#warning "------------------------------------------------------------------------------- Development by Author, may not compile for others"
#endif

#include <Arduino.h>
#include <stdint.h>
#include <string.h>


#include <ext_printf.h>


#define SET_FLAG   true
#define RESET_FLAG false

#ifdef CONFIG_IDF_TARGET_ESP32
#include "soc/efuse_reg.h"
#endif

#include <vector>

#include "JsonParser.h"

#include "2_CoreSystem/esp32_compat.h"
#include "2_CoreSystem/mGlobalMacros.h"


/***
 * Include template structure
 * 
 * First add the base level defaults
 * Then add any custom header layouts
 * finish with firmware defaults that may ovverride above
 * 
 * 
 * 
 * 
*/



// Minimal base configurations required for stable operation
#include "2_CoreSystem/mBaseConfig.h"           //DEFAULTS
// Optional user configs, which override defaults

// #include "2c_Internal_IsolatedNoTaskerSystems/Averaging/AveragingData.h"

#ifndef D_USER_MICHAEL // Include my personally named secret file
  #include "0_ConfigUser/mFirmwareCustom_Secret.h"
#endif // D_USER_MICHAEL // Include my personally named secret file
#ifdef D_USER_MICHAEL // Include my personally named secret file
  /**
   * In order of importance
   * */
  #include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_LongTerm.h"
  #include "0_ConfigUser/00_mFirmwareCustom_Secret_Home_Temporary.h"
  #include "0_ConfigUser/01_mFirmwareCustom_Secret_ExampleTemplates.h"
  #include "0_ConfigUser/02_mFirmwareCustom_Secret_DevTestbeds.h"
  #include "0_ConfigUser/TestGroups/FirmwareGroup_LightingEffects.h"
  #include "0_ConfigUser/BuiltUsingGroups/GroupUsing_LightingEffects.h"
  #include "0_ConfigUser/TestGroups/FirmwareGroup_CellularDatalinks.h"
#endif // D_USER_MICHAEL


#include "2_CoreSystem/06_Support/mSensorType.h"



#include "0_ConfigUser/G1_mUserConfig_Secret.h"  //wrong place??
#include "2_CoreSystem/09_Events/mEvents.h"

#include <DeCounter.h>

#ifdef USE_MODULE_CORE_RULES
#include "2_CoreSystem/10_RuleEngine/mRuleEngine.h"
#endif

#include "2_CoreSystem/mFirmwareDefaults.h"                    // Configuration overrides for all previous includes






#include "2_CoreSystem/11_Languages/mLanguageDefault.h"                           // Language support configured by .h
#include "1_TaskerManager/mTaskerManager_Defines.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"                // Hardware configuration
#include "2_CoreSystem/06_Support/BufferWriter.h"

#include "1_TaskerManager/mTasks.h"
#include <WiFiClient.h>
#ifdef ESP8266
  #include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#endif

#ifdef ESP8266
  #include "2_CoreSystem/06_Support/SupportESP8266.h"
  #define mSupportHardware SupportESP8266
#endif
#ifdef ESP32
  #include "2_CoreSystem/06_Support/SupportESP32.h"
  #define mSupportHardware SupportESP32
#endif

  #ifdef USE_MODULE_NETWORK_WEBSERVER
      
    #define LOROL_LITTLEFS 

    #ifdef ESP8266
      #include <ESP8266WiFi.h>
      #include <ESP8266mDNS.h>
      #include <ESPAsyncTCP.h>
      #include <LittleFS.h>
      extern "C"
      {
      #include <user_interface.h>
      }
      #ifndef WLED_DISABLE_ESPNOW
        #include <espnow.h>
      #endif
    #else // ESP32
      #include <HardwareSerial.h>  // ensure we have the correct "Serial" on new MCUs (depends on ARDUINO_USB_MODE and ARDUINO_USB_CDC_ON_BOOT)
      #include <WiFi.h>
      #include <ETH.h>
      #include "esp_wifi.h"
      #include <ESPmDNS.h>
      #include <AsyncTCP.h>
      #include "esp_task_wdt.h"

      #ifndef WLED_DISABLE_ESPNOW
        #include <esp_now.h>
      #endif
    #endif

  #endif // USE_MODULE_NETWORK_WEBSERVER


  #ifdef USE_MODULE_DRIVERS_FILESYSTEM


    #define LOROL_LITTLEFS 
    
      #ifdef LOROL_LITTLEFS
        #ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
          #define CONFIG_LITTLEFS_FOR_IDF_3_2
        #endif
        #include <LITTLEFS.h>
      #else
        #include <LittleFS.h>
      #endif


    //Filesystem to use for preset and config files. SPIFFS or LittleFS on ESP8266, SPIFFS only on ESP32 (now using LITTLEFS port by lorol)
    #ifdef ESP8266
      #define FILE_SYSTEM LittleFS
    #else
      #ifdef LOROL_LITTLEFS
        #define FILE_SYSTEM LITTLEFS
      #else
        #define FILE_SYSTEM LittleFS
      #endif
    #endif





  #endif // USE_MODULE_DRIVERS_FILESYSTEM

// #include <variant>

// Returns via tasker that report special status
// Can also be interpeted as basic numbers
enum FUNCTION_RESULT_IDS{
  // Errors
  FUNCTION_RESULT_ERROR_POINTER_INVALID_ID = -2,
  FUNCTION_RESULT_MODULE_DISABLED_ID = -1,
  // Unknown
  FUNCTION_RESULT_UNKNOWN_ID = 0,
  // Good results
  FUNCTION_RESULT_SUCCESS_ID,
  FUNCTION_RESULT_HANDLED_ID,
  FUNCTION_RESULT_ERROR_ID,
  // Length
  FUNCTION_RESULT_LENGTH_ID
};

enum ModuleStatus{
  Disabled,
  NoGPIOConfigured,
  Initialising,
  Running,
  DevicesPresent // Running means searching is enabled, but this is only set when devices are found
};


#include "2_CoreSystem/mFirmwareDefaults.h"
#include "2_CoreSystem/mSystemConfig.h"


// Libraries
#include <StreamString.h>                   // Webserver, Updater

#ifdef USE_ARDUINO_OTA
  #include <ArduinoOTA.h>                   // Arduino OTA
#endif  // USE_ARDUINO_OTA

//#ifdef USE_I2C
  #include <Wire.h>                         // I2C support library
//#endif  // USE_I2C

#ifdef ESP32
  #include <WiFi.h>
  #ifdef USE_NETWORK_MDNS
  #include <ESPmDNS.h>
  #endif // #ifdef USE_NETWORK_MDNS
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include "Ticker.h"
  #include "esp_system.h"
  #include "soc/soc.h"
  #include "soc/rtc_cntl_reg.h"
#endif
#ifdef ESP8266
  #include <ESP8266HTTPClient.h>
  #include <ESP8266httpUpdate.h>  
  #ifdef USE_DISCOVERY
    #include <ESP8266mDNS.h>                  // MQTT, Webserver, Arduino OTA
  #endif  // USE_DISCOVERY
  #include <ArduinoOTA.h>
  #include <WiFiUdp.h>
  #include <ESP8266WiFi.h>
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    // #include <ESPAsyncTCP.h>
    //?#include <ESPAsyncWebServer.h>
  #endif //USE_MODULE_NETWORK_WEBSERVER
#endif

#include "2_CoreSystem/08_JSON/mJSON.h"
#include "2_CoreSystem/06_Support/mSupport.h"

#include "2_CoreSystem/01b_RtcMemory/mRtcSettings.h"


enum MODULE_SUBTYPE_IDS{ //ignores the "interface"
  MODULE_SUBTYPE_CORE_ID,
  MODULE_SUBTYPE_NETWORK_ID,
  MODULE_SUBTYPE_DISPLAY_ID,
  MODULE_SUBTYPE_SENSOR_ID,
  MODULE_SUBTYPE_DRIVERS_ID,
  MODULE_SUBTYPE_LIGHTS_ID,
  MODULE_SUBTYPE_ENERGY_ID,
};

#define pCONT mTaskerManager::GetInstance()




// macro to know if a module is a type (ie within a range)
#define IS_MODULE_SENSOR_SUBMODULE(x)   (x >= 5001 && x <= 5999) // skipping the interface
#define IS_MODULE_LIGHTING_SUBMODULE(x) (x >= 6001 && x <= 6999) // skipping the interface
#define IS_MODULE_ENERGY_SUBMODULE(x)   (x >= 7001 && x <= 7999) // skipping the interface
#define IS_MODULE_DISPLAY_SUBMODULE(x)  (x >= 8001 && x <= 8999) // skipping the interface




#define D_TARGET_TASKER_NONE 0

enum TaskerID
{
  /**
   * @brief Core Modules
   **/
  #ifdef USE_MODULE_CORE_SETTINGS
    EM_MODULE_CORE_SETTINGS_ID,
  #endif 
  #ifdef USE_MODULE_CORE_TIME
    EM_MODULE_CORE_TIME_ID,
  #endif 
  #ifdef USE_MODULE_CORE_HARDWAREPINS
    EM_MODULE_CORE_HARDWAREPINS_ID,
  #endif 
  #ifdef USE_MODULE_CORE_LOGGING
    EM_MODULE_CORE_LOGGING_ID,
  #endif 
  #ifdef USE_MODULE_CORE_SUPPORT
    EM_MODULE_CORE_SUPPORT_ID,
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
    EM_MODULE_CORE_TELEMETRY_ID,
  #endif 
  #ifdef USE_MODULE_CORE_RULES
    EM_MODULE_CORE_RULES_ID,
  #endif 
  #ifdef USE_MODULE_CORE_UPDATES
    EM_MODULE_CORE_UPDATES_ID,
  #endif 
  #ifdef USE_MODULE_CORE_SERIAL_UART
    EM_MODULE_CORE_SERIAL_UART_ID,
  #endif
  #ifdef USE_MODULE_CORE__SERIAL //merge into above
    CORE__SERIAL,
  #endif 
  #ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
    EM_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID,
  #endif 
  /**
   * @brief Network
   **/
  #ifdef USE_MODULE_NETWORK_INTERFACE
    EM_MODULE__NETWORK_INTERFACE__ID,
  #endif
  #ifdef USE_MODULE_NETWORK_WIFI
    EM_MODULE_NETWORK_WIFI_ID,
  #endif 
  #ifdef USE_MODULE_NETWORK_ETHERNET
    EM_MODULE_NETWORK_ETHERNET_ID,
  #endif 
  #ifdef USE_MODULE_NETWORK_CELLULAR
    EM_MODULE__NETWORK_CELLULAR__ID,
  #endif
  #ifdef USE_MODULE_NETWORK_MQTT
    EM_MODULE_NETWORK_MQTT_ID,
  #endif 
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    EM_MODULE_NETWORK_WEBSERVER_ID,
  #endif
  /**
   * @brief Displays
   **/
  #ifdef USE_MODULE_DRIVERS_INTERFACE
    EM_MODULE_DRIVERS_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
    EM_MODULE_DRIVERS_FILESYSTEM_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
    EM_MODULE_DRIVERS_LEDS_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    EM_MODULE_DRIVERS_RELAY_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM

  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    EM_MODULE_DRIVERS_IRTRANSCEIVER_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_IRREMOTE
    EM_MODULE_DRIVERS_IRREMOTE_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
    EM_MODULE_DRIVERS_RF433_RCSWITCH_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
    EM_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    EM_MODULE_DRIVERS_HBRIDGE_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    EM_MODULE_DRIVERS_SDCARD_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    EM_MODULE_DRIVERS_SHELLY_DIMMER_ID,
  #endif
  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
    EM_MODULE__DRIVERS_BUZZER_BASIC__ID,
  #endif
  #ifdef USE_MODULE__DRIVERS_BUZZER_TONES
    EM_MODULE__DRIVERS_BUZZER_TONES__ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    EM_MODULE_DRIVERS_CAMERA_OV2640_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640_2
    EM_MODULE_DRIVERS_CAMERA_OV2640_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM
    EM_MODULE_DRIVERS_CAMERA_WEBCAM_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
    EM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO 
    EM_MODULE_DRIVERS__CAMERA_ARDUINO__ID,
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA
    EM_MODULE_DRIVERS__CAMERA_TASMOTA__ID,
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_MULTICLIENT 
    EM_MODULE_DRIVERS__CAMERA_MULTICLIENT__ID,
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
    EM_MODULE__DRIVERS_MAVLINK_DECODER__ID,
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI
    EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__ID,
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR
    EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR__ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_7000G
    EM_MODULE_DRIVERS__MODEM_7000G__ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_800L
    EM_MODULE_DRIVERS__MODEM_800L__ID,
  #endif
  /**
   * @brief Sensors
   **/
  #ifdef USE_MODULE_SENSORS_INTERFACE
    EM_MODULE_SENSORS_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    EM_MODULE_SENSORS_SWITCHES_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
    EM_MODULE_SENSORS_BUTTONS_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    EM_MODULE_SENSORS_BME_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    EM_MODULE_SENSORS_DHT_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_BH1750
    EM_MODULE_SENSORS_BH1750_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    EM_MODULE_SENSORS_MOTION_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    EM_MODULE_SENSORS_DOOR_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_L3G
    EM_MODULE_SENSORS_L3G_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
    EM_MODULE_SENSORS_LDR_BASIC_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_LSM303D
    EM_MODULE_SENSORS_LSM303D_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
    EM_MODULE_SENSORS_SR04_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_MPU9250
    EM_MODULE_SENSORS_MPU9250_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    EM_MODULE_SENSORS_PULSECOUNTER_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
    EM_MODULE_SENSORS_REMOTE_DEVICE_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
    EM_MODULE_SENSORS_ROTARY_ENCODER_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_SOLAR_LUNAR
    EM_MODULE_SENSORS_SOLAR_LUNAR_ID,
  #endif 
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    EM_MODULE_SENSORS_ULTRASONIC_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
    EM_MODULE_SENSORS_ADC_INTERNAL_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
    EM_MODULE_SENSORS_ADC_INTERNAL_ID,
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023
    EM_MODULE_SENSORS__DS18X20__ID,
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
    EM_MODULE_SENSORS__DS18X20__ID,
  #endif  
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  EM_MODULE__SENSORS_GPS_SERIAL__ID,
  #endif
  #ifdef USE_MODULE_SENSORS_GPS_MODEM
  EM_MODULE__SENSORS_GPS_MODEM__ID,
  #endif
  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  EM_MODULE__SENSORS_BATTERY_MODEM__ID,
  #endif
  /**
   * @brief Lights
   **/
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    EM_MODULE_LIGHTS_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    EM_MODULE_LIGHTS_ANIMATOR_ID,
  #endif
  /**
   * @brief Energy
   **/
  #ifdef USE_MODULE_ENERGY_INTERFACE
    EM_MODULE_ENERGY_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
    EM_MODULE_ENERGY_PZEM004T_V3_ID,
  #endif
  #ifdef USE_MODULE_ENERGY_ADE7953
    EM_MODULE_ENERGY_ADE7953_ID,
  #endif
  #ifdef USE_MODULE_ENERGY_INA219
    EM_MODULE_ENERGY_INA219_ID,
  #endif
  /**
   * @brief Displays
   **/
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
    EM_MODULE_DISPLAYS_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    EM_MODULE_DISPLAYS_NEXTION_ID,
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
    EM_MODULE_DISPLAYS_OLED_SSD1306_ID,
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
    EM_MODULE_DISPLAYS_OLED_SH1106_ID,
  #endif
  /**
   * @brief Controller Generic (Generic)
   **/
  #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
    EM_MODULE_CONTROLLER_SONOFF_IFAN_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_FAN
    EM_MODULE_CONTROLLER_FAN_ID,
  #endif
  // 3d printer encoder here
  #ifdef USE_MODULE_CONTROLLER_TANKVOLUME
    EM_MODULE_CONTROLLER_TANKVOLUME_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_BLINDS
    EM_MODULE_CONTROLLER_BLINDS_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
    EM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    EM_MODULE_CONTROLLER_DOORBELL_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
    EM_MODULE_CONTROLLER_GPS_SD_LOGGER_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
    EM_MODULE_CONTROLLER_SDCARDLOGGER_ID,
  #endif
  // USE_MODULE_CONTROLLER_UARTLOGGER
  // USE_MODULE_CONTROLLER_INTERNAL_CLOCK
  #ifdef USE_MODULE_CONTROLLER_HVAC
    EM_MODULE_CONTROLLER__HVAC__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE
    EM_MODULE_CONTROLLER__HVAC_REMOTE__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
    EM_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
    EM_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__ID,
  #endif
  // 50_Animator_Input_Controller
  // 51_Animator_Playlists
  #ifdef USE_MODULE_CONTROLLER__ENERGY_OLED
    EM_MODULE_CONTROLLER__ENERGY_OLED__ID,
  #endif
  /**
   * @brief 10 Controller Custom
   **/  
  #ifdef USE_MODULE_CONTROLLER_RADIATORFAN
    EM_MODULE_CONTROLLER_RADIATORFAN_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
    EM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
    EM_MODULE_CONTROLLER_FURNACE_SENSOR_ID,
  #endif
  #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
    EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
    EM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHT__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
    EM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
    EM_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX
    EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
    EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
    EM_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE
    EM_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER
    EM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER__ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
    EM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
    EM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_USERMOD_01
    EM_MODULE_CONTROLLER_USERMOD_01_ID,
  #endif
  EM_MODULE_LENGTH_ID
};

/**
 * @brief Core Modules
 **/
#ifdef USE_MODULE_CORE_SETTINGS
  #include "2_CoreSystem/01_Settings/mSettings.h"
  #define   pCONT_set                               static_cast<mSettings*>(pCONT->pModule[EM_MODULE_CORE_SETTINGS_ID])
#endif 
#ifdef USE_MODULE_CORE_TIME
  #include "2_CoreSystem/02_Time/mTime.h"
  #define   pCONT_time                              static_cast<mTime*>(pCONT->pModule[EM_MODULE_CORE_TIME_ID])
#endif 
#ifdef USE_MODULE_CORE_HARDWAREPINS
  #include "2_CoreSystem/04_HardwarePins/mHardwarePins.h"
  #define   pCONT_pins                              static_cast<mHardwarePins*>(pCONT->pModule[EM_MODULE_CORE_HARDWAREPINS_ID])
#endif 
#ifdef USE_MODULE_CORE_LOGGING
  #include "2_CoreSystem/05_Logging/mLogging.h"
  #define   pCONT_log                               static_cast<mLogging*>(pCONT->pModule[EM_MODULE_CORE_LOGGING_ID])
#endif 
#ifdef USE_MODULE_CORE_SUPPORT
  #include "2_CoreSystem/06_Support/mSupport.h"
  #define   pCONT_sup                               static_cast<mSupport*>(pCONT->pModule[EM_MODULE_CORE_SUPPORT_ID])
#endif 
#ifdef USE_MODULE_CORE_TELEMETRY
  #include "2_CoreSystem/07_Telemetry/mTelemetry.h"
  #define   pCONT_tel                               static_cast<mTelemetry*>(pCONT->pModule[EM_MODULE_CORE_TELEMETRY_ID])
#endif 
#ifdef USE_MODULE_CORE_RULES
  #include "2_CoreSystem/10_RuleEngine/mRuleEngine.h"
  #define   pCONT_rules                             static_cast<mRuleEngine*>(pCONT->pModule[EM_MODULE_CORE_RULES_ID])
#endif
#ifdef USE_MODULE_CORE_UPDATES
  #include "2_CoreSystem/14_Updates/mUpdates.h"
  #define   pCONT_updates                           static_cast<mUpdates*>(pCONT->pModule[EM_MODULE_CORE_UPDATES_ID])
#endif
#ifdef USE_MODULE_CORE_SERIAL_UART
  #include "2_CoreSystem/04b_SerialUART/mSerialUART.h"
  #define pCONT_uart                                static_cast<mSerialUART*>(pCONT->pModule[EM_MODULE_CORE_SERIAL_UART_ID])
#endif
#ifdef USE_MODULE_CORE__SERIAL
  #include "2_CoreSystem/15_SerialUART/mSerial.h"
  #define   tkr_Serial                              static_cast<mSerial*>(pCONT->pModule[TaskerID::CORE__SERIAL])
#endif
#ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
  #include "2_CoreSystem/99_DevelopmentDebugging/mDevelopmentDebugging.h"
  #define   pCONT_debug             static_cast<mDevelopmentDebugging*>(pCONT->pModule[EM_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID])
#endif 
/**
 * @brief Network
 **/
#ifdef USE_MODULE_NETWORK_INTERFACE
  #include "3_Network/00_Interface/mInterface.h"
  #define pCONT_interface_network                                static_cast<mInterfaceNetwork*>(pCONT->pModule[EM_MODULE__NETWORK_INTERFACE__ID])
#endif 
#ifdef USE_MODULE_NETWORK_WIFI
  #include "3_Network/03_WiFi/mWiFi.h"
  #define pCONT_wif                                 static_cast<mWiFi*>(pCONT->pModule[EM_MODULE_NETWORK_WIFI_ID])
#endif 
#ifdef USE_MODULE_NETWORK_ETHERNET
  //EM_MODULE_NETWORK_ETHERNET_ID,
#endif 
#ifdef USE_MODULE_NETWORK_CELLULAR
#include "3_Network/05_Cellular/mCellular.h"
  #define pCONT_cell                               static_cast<mCellular*>(pCONT->pModule[EM_MODULE__NETWORK_CELLULAR__ID])
#endif
#ifdef USE_MODULE_NETWORK_MQTT
  #include "3_Network/10_MQTT/mMQTT.h"
  #define pCONT_mqtt                                static_cast<mMQTT*>(pCONT->pModule[EM_MODULE_NETWORK_MQTT_ID])
#endif 
#ifdef USE_MODULE_NETWORK_WEBSERVER
  #include "3_Network/21_WebServer/mWebServer.h"
  #define pCONT_web                                 static_cast<mWebServer*>(pCONT->pModule[EM_MODULE_NETWORK_WEBSERVER_ID])
#endif
/**
 * @brief Displays
 **/
#ifdef USE_MODULE_DRIVERS_INTERFACE
  #include "4_Drivers/00_Interface/mDriverInterface.h"
  #define pCONT_iDrivers                           static_cast<mDriverInterface*>(pCONT->pModule[EM_MODULE_DRIVERS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_FILESYSTEM
  #include "4_Drivers/02_FileSystem/mFileSystem.h"
  #define pCONT_mfile                               static_cast<mFileSystem*>(pCONT->pModule[EM_MODULE_DRIVERS_FILESYSTEM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_LEDS
#include "4_Drivers/03_LEDs/mLEDs.h"
  #define pCONT_led                                static_cast<mLEDs*>(pCONT->pModule[EM_MODULE_DRIVERS_LEDS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RELAY
  #include "4_Drivers/04_Relays/mRelays.h"
  #define pCONT_mry                                 static_cast<mRelays*>(pCONT->pModule[EM_MODULE_DRIVERS_RELAY_ID])
#endif
#ifdef USE_MODULE_DRIVERS_PWM
  #include "4_Drivers/PWM/mPWM.h"
  #define pCONT_pwm                                 static_cast<mPWM*>(pCONT->pModule[EM_MODULE_DRIVERS_PWM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  #include "4_Drivers/IRDevices/mIRtransceiver.h"
  #define pCONT_mdirt                               static_cast<mIRtransceiver*>(pCONT->pModule[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRREMOTE
#include "4_Drivers/IRRemote/mIRRemote.h"
  #define pCONT_ir_remote                           static_cast<mIRRemote*>(pCONT->pModule[EM_MODULE_DRIVERS_IRREMOTE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
  #include "4_Drivers/09_RCSwitch/mRCSwitch.h"
  #define pCONT_rcswitch                            static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  #include "4_Drivers/10_RCSwitch_Extended/mRCSwitch_Extended.h"
  #define pCONT_rcswitch                            static_cast<mRCSwitch*>(pCONT->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID])
#endif
#ifdef USE_MODULE_DRIVERS_HBRIDGE
  #include "4_Drivers/Motors/HBridgeL9110/mHBridge.h"
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pModule[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SDCARD
  #include "4_Drivers/SD/mSDCard.h"
  #define pCONT_sdcard                              static_cast<mSDCard*>(pCONT->pModule[EM_MODULE_DRIVERS_SDCARD_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  #include "4_Drivers/15_ShellyDimmer/mShellyDimmer.h"
  #define pCONT_shelly                              static_cast<mShellyDimmer*>(pCONT->pModule[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID])
#endif
#ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  #include "4_Drivers/20_Buzzer_Basic/mBuzzer.h"
  #define pCONT_buzzer                              static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE__DRIVERS_BUZZER_BASIC__ID])
#endif
#ifdef USE_MODULE__DRIVERS_BUZZER_TONES
  #include "4_Drivers/21_Buzzer_Tones/mBuzzer.h"
  #define pCONT_buzzer                              static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE__DRIVERS_BUZZER_TONES__ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
  #include "4_Drivers/50_CAM_OV2640/mCamera.h"
  #define pCONT_mdhbridge                           static_cast<mCamera*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640_2
  #include "4_Drivers/Camera_OV2640/mCameraOV2640.h"
  #define pCONT_camera                              static_cast<mCameraOV2640*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_OV2640_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM
  #include "4_Drivers/51_WebCam/mWebCam.h"
  #define pCONT_camera                              static_cast<mWebCam*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
  #include "4_Drivers/52_WebCamera/mWebCamera.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO
  #include "4_Drivers/60_WebCam_Arduino/mWebCam.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_ARDUINO__ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA
  #include "4_Drivers/61_WebCam_Tas/mWebCam.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_TASMOTA__ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_MULTICLIENT
  #include "4_Drivers/52_WebCamera/mWebCamera.h"
  #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_MULTICLIENT__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  #include "4_Drivers/70_MAVLink_Decoder/mMAVLink_Decoder.h"
  #define pCONT_mavlink                              static_cast<mMAVLink_Decoder*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_DECODER__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI
  #include "4_Drivers/71_MAVLink_Telemetry_WiFi/mMAVLink_Telemetry_WiFi.h"
  #define pCONT_mavlink                              static_cast<mMAVLink_Telemetry_WiFi*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR
  #include "4_Drivers/72_MAVLink_Telemetry_Cellular/mMAVLink_Telemetry_Cellular.h"
  #define pCONT_mavlink                              static_cast<mMAVLink_Telemetry_Cellular*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR__ID])
#endif
#ifdef USE_MODULE_DRIVERS_MODEM_7000G
#include "4_Drivers/80_Modem_SIM7000G/mSIM7000G.h"
  #define pCONT_sim7000g                           static_cast<mSIM7000G*>(pCONT->pModule[EM_MODULE_DRIVERS__MODEM_7000G__ID])
#endif
#ifdef USE_MODULE_DRIVERS_MODEM_800L
#include "4_Drivers/81_Modem_SIM800L/mSIM800L.h"
  #define pCONT_sim800l                           static_cast<mSIM800L*>(pCONT->pModule[EM_MODULE_DRIVERS__MODEM_800L__ID])
#endif
/**
 * @brief Sensors
 **/
#ifdef USE_MODULE_SENSORS_INTERFACE
  #include "5_Sensors/00_Interface/mSensorsInterface.h"
  #define pCONT_iSensors                         static_cast<mSensorsInterface*>(pCONT->pModule[EM_MODULE_SENSORS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_SENSORS_SWITCHES
  #include "5_Sensors/01_Switches/mSwitches.h"
  #define pCONT_swt                            static_cast<mSwitches*>(pCONT->pModule[EM_MODULE_SENSORS_SWITCHES_ID])
#endif
#ifdef USE_MODULE_SENSORS_BUTTONS
  #include "5_Sensors/02_Buttons/mButtons.h"
  #define pCONT_sbutton                         static_cast<mButtons*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_SENSORS_BME
  #include "5_Sensors/03_BME/mBME.h"
  #define pCONT_bme                             static_cast<mBME*>(pCONT->pModule[EM_MODULE_SENSORS_BME_ID])
#endif
#ifdef USE_MODULE_SENSORS_DHT
  #include "5_Sensors/05_DHT/mSensorsDHT.h"
  #define pCONT_dht                             static_cast<mSensorsDHT*>(pCONT->pModule[EM_MODULE_SENSORS_DHT_ID])
#endif
#ifdef USE_MODULE_SENSORS_BH1750
  #include "5_Sensors/06_BH1750Light/mBH1750.h"
  #define pCONT_motion                          static_cast<mBH1750*>(pCONT->pModule[EM_MODULE_SENSORS_BH1750_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOTION
  #include "5_Sensors/07_Motion/mMotion.h"
  #define pCONT_smot                            static_cast<mMotionSensor*>(pCONT->pModule[EM_MODULE_SENSORS_MOTION_ID])
#endif
#ifdef USE_MODULE_SENSORS_DOOR
  #include "5_Sensors/11_Door/mDoorSensor.h"
  #define pCONT_sdoor                           static_cast<mDoorSensor*>(pCONT->pModule[EM_MODULE_SENSORS_DOOR_ID])
#endif
#ifdef USE_MODULE_SENSORS_L3G
  #include "5_Sensors/L3GD20_3Axis_Gryo/mSensorsL3G.h"
  #define pCONT_L3G                      static_cast<mSensorsL3G*>(pCONT->pModule[EM_MODULE_SENSORS_L3G_ID])
#endif
#ifdef USE_MODULE_SENSORS_LDR_BASIC
  #include "5_Sensors/14_LDRBasic/mLDRBasic.h"
  #define pCONT_ldr_basic                      static_cast<mLDRBasic*>(pCONT->pModule[EM_MODULE_SENSORS_LDR_BASIC_ID])
#endif
#ifdef USE_MODULE_SENSORS_LSM303D
  #include "5_Sensors/LSM303D_3Axis_AccMag/mSensorsLSM303D.h"
  #define pCONT_LSM303D                      static_cast<mSensorsLSM303D*>(pCONT->pModule[EM_MODULE_SENSORS_LSM303D_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOISTURE
  #include "5_Sensors/Moisture/mMoistureSensor.h"
  #define pCONT_srmoisture                      static_cast<mMoistureSensor*>(pCONT->pModule[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID])
#endif
#ifdef USE_MODULE_SENSORS_SR04
  #include "5_Sensors/17_SR04/mSR04.h"
  #define pCONT_sr04                              static_cast<mSR04*>(pCONT->pModule[EM_MODULE_SENSORS_SR04_ID])
#endif
#ifdef USE_MODULE_SENSORS_MPU9250
  #include "5_Sensors/MPU9250/mSensorsMPU9250.h"
  #define pCONT_MPU9250                      static_cast<mSensorsMPU9250*>(pCONT->pModule[EM_MODULE_SENSORS_MPU9250_ID])
#endif
#ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  #include "5_Sensors/PulseCounter/mPulseCounter.h"
  #define pCONT_spulse                          static_cast<mPulseCounter*>(pCONT->pModule[EM_MODULE_SENSORS_PULSECOUNTER_ID])
#endif
#ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
  #include "5_Sensors/20_RemoteDevice/mRemoteDevice.h"
  #define pCONT_sremote                           static_cast<mRemoteDevice*>(pCONT->pModule[EM_MODULE_SENSORS_REMOTE_DEVICE_ID])
#endif
#ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
  #include "5_Sensors/21_RotaryEncoder/mRotaryEncoder.h"
  #define pCONT_rotary_encoder               static_cast<mRotaryEncoder*>(pCONT->pModule[EM_MODULE_SENSORS_ROTARY_ENCODER_ID])
#endif
#ifdef USE_MODULE_SENSORS_SOLAR_LUNAR
  #include "5_Sensors/22_SolarLunar/mSolarLunar.h"
  #define   pCONT_solar                             static_cast<mSolarLunar*>(pCONT->pModule[EM_MODULE_SENSORS_SOLAR_LUNAR_ID])
#endif
#ifdef USE_MODULE_SENSORS_ULTRASONICS
  #include "5_Sensors/UltraSonic/mUltraSonicSensor.h"
  #define pCONT_ult                             static_cast<mUltraSonicSensor*>(pCONT->pModule[EM_MODULE_SENSORS_ULTRASONIC_ID])
#endif
#ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
  #include "5_Sensors/30_ADCInternal_ESP82/mADCInternal.h"
  #define pCONT_adc_internal                      static_cast<mADCInternal*>(pCONT->pModule[EM_MODULE_SENSORS_ADC_INTERNAL_ID])
#endif
#ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
  #include "5_Sensors/31_ADCInternal_ESP32/mADCInternal.h"
  #define pCONT_adc_internal                      static_cast<mADCInternal*>(pCONT->pModule[EM_MODULE_SENSORS_ADC_INTERNAL_ID])
#endif
#ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023
  #include "5_Sensors/40_ds18x20/mDB18x20.h"
  #define pCONT_db18                      static_cast<mDB18x20_ESP32*>(pCONT->pModule[EM_MODULE_SENSORS__DS18X20__ID])
#endif
#ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  #include "5_Sensors/41_ds18x20_esp32/mDB18x20_ESP32.h"
  #define pCONT_db18                      static_cast<mDB18x20_ESP32*>(pCONT->pModule[EM_MODULE_SENSORS__DS18X20__ID])
#endif
#ifdef USE_MODULE_SENSORS_GPS_SERIAL
  #include "5_Sensors/50_GPS_Serial/mGPS_Serial.h"
  #define pCONT_gps                                 static_cast<mGPS_Serial*>(pCONT->pModule[EM_MODULE__SENSORS_GPS_SERIAL__ID])
#endif
#ifdef USE_MODULE_SENSORS_GPS_MODEM
  #include "5_Sensors/51_GPS_Modem/mGPS_Modem.h"
  #define pCONT_gps                                 static_cast<mGPS_Modem*>(pCONT->pModule[EM_MODULE__SENSORS_GPS_MODEM__ID])
#endif
#ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  #include "5_Sensors/52_Battery_Modem/mBattery_Modem.h"
  #define pCONT_batt_modem                                 static_cast<mBattery_Modem*>(pCONT->pModule[EM_MODULE__SENSORS_BATTERY_MODEM__ID])
#endif
/**
 * @brief Lights
 **/
#ifdef USE_MODULE_LIGHTS_INTERFACE
  #include "6_Lights/00_Interface/mInterfaceLight.h"
  #define pCONT_iLight                          static_cast<mInterfaceLight*>(pCONT->pModule[EM_MODULE_LIGHTS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_LIGHTS_ANIMATOR
  #include "6_Lights/03_Animator/mAnimatorLight.h"
  #define pCONT_lAni                            static_cast<mAnimatorLight*>(pCONT->pModule[EM_MODULE_LIGHTS_ANIMATOR_ID])
#endif
/**
 * @brief Energy
 **/
#ifdef USE_MODULE_ENERGY_INTERFACE
  #include "7_Energy/00_Interface/mEnergyInterface.h"
  #define pCONT_iEnergy                           static_cast<mEnergyInterface*>(pCONT->pModule[EM_MODULE_ENERGY_INTERFACE_ID])
#endif
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  #include "7_Energy/01_Pzem004T_v3/mPZEM004T.h"
  #define pCONT_pzem                              static_cast<mEnergyPZEM004T*>(pCONT->pModule[EM_MODULE_ENERGY_PZEM004T_V3_ID])
#endif
#ifdef USE_MODULE_ENERGY_ADE7953
  #include "7_Energy/02_ADE7953/mADE7953.h"
  #define pCONT_ade7953                         static_cast<mEnergyADE7953*>(pCONT->pModule[EM_MODULE_ENERGY_ADE7953_ID])
#endif
#ifdef USE_MODULE_ENERGY_INA219
  #include "7_Energy/03_INA219/mINA219.h"
  #define pCONT_mina219                         static_cast<mEnergyINA219*>(pCONT->pModule[EM_MODULE_ENERGY_INA219_ID])
#endif
/**
 * @brief Displays
 **/
#ifdef USE_MODULE_DISPLAYS_INTERFACE
  #include "8_Displays/00_Interface/mDisplaysInterface.h"
  #define pCONT_iDisp                               static_cast<mDisplaysInterface*>(pCONT->pModule[EM_MODULE_DISPLAYS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_DISPLAYS_NEXTION
  #include "8_Displays/01_Nextion/mNextionPanel.h"
  #define pCONT_nex                                 static_cast<mNextionPanel*>(pCONT->pModule[EM_MODULE_DISPLAYS_NEXTION_ID])
#endif
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  #include "8_Displays/02_OLED_SSD1606/mOLED_SSD1306.h"
  #define pCONT_oled1306                            static_cast<mOLED_SSD1306*>(pCONT->pModule[EM_MODULE_DISPLAYS_OLED_SSD1306_ID])
#endif
#ifdef USE_MODULE_DISPLAYS_OLED_SH1106
  #include "8_Displays/03_OLED_SH1106/mOLED_SH1106.h"
  #define pCONT_oled1306                            static_cast<mOLED_SH1106*>(pCONT->pModule[EM_MODULE_DISPLAYS_OLED_SH1106_ID])
#endif
/**
 * @brief Controller Generic (Generic)
 **/
#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
  #include "9_Controller/03_Sonoff_iFan/mSonoffIFan.h"
  #define pCONT_ifan                            static_cast<mSonoffIFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_SONOFF_IFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_FAN
  #include "9_Controller/Fan/mFan.h"
  #define pCONT_mfan                            static_cast<mFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_FAN_ID])
#endif
// 3d printer encoder here
#ifdef USE_MODULE_CONTROLLER_TANKVOLUME
  #include "9_Controller/08_TankVolume/mTankVolume.h"
  #define pCONT_tankvolume                      static_cast<mTankVolume*>(pCONT->pModule[EM_MODULE_CONTROLLER_TANKVOLUME_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_BLINDS
  #include "9_Controller/Blinds/mBlinds.h"
  #define pCONT_sbut                            static_cast<mBlinds*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
  #include "9_Controller/BucketWaterLevel/mBucketWaterLevel.h"
  #define pCONT_bucket_water_level                static_cast<mBucketWaterLevel*>(pCONT->pModule[EM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_DOORCHIME
  #include "9_Controller/DoorBell/mDoorBell.h"
  #define pCONT_doorbell                        static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_DOORBELL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
  #include "9_Controller/GPS_SD_Logger/mGPS_SD_Logger.h"
  #define pCONT_gps_sd_log                  static_cast<mGPS_SD_Logger*>(pCONT->pModule[EM_MODULE_CONTROLLER_GPS_SD_LOGGER_ID])
  #define pCONT_serial_pos_log  pCONT_gps_sd_log
#endif
#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
  #include "9_Controller/SDCardLogger/mSDCardLogger.h"
  #define pCONT_sdcardlogger                    static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_SDCARDLOGGER_ID])
#endif
// USE_MODULE_CONTROLLER_UARTLOGGER
// USE_MODULE_CONTROLLER_INTERNAL_CLOCK
#ifdef USE_MODULE_CONTROLLER_HVAC
  #include "9_Controller/40_HVAC/mHVAC.h"
  #define pCONT_hvac                        static_cast<mHVAC*>(pCONT->pModule[EM_MODULE_CONTROLLER__HVAC__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE
  #include "9_Controller/41_HVAC_Remote/mHVAC.h"
  #define pCONT_hvac_remote                        static_cast<mHVAC_Remote*>(pCONT->pModule[EM_MODULE_CONTROLLER__HVAC_REMOTE__ID])
#endif
#ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  #include "9_Controller/42_SensorColourBar/mSensorColourBar.h"
  #define pCONT_colourbar                        static_cast<mSensorColourBar*>(pCONT->pModule[EM_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__ID])
#endif
#ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  #include "9_Controller/43_RelayStateLEDStrip/mRelayStateLEDStrip.h"
  #define pCONT_relay_strip                      static_cast<mRelayStateLEDStrip*>(pCONT->pModule[EM_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__ID])
#endif
// 50_Animator_Input_Controller
// 51_Animator_Playlists
#ifdef USE_MODULE_CONTROLLER__ENERGY_OLED
  #include "9_Controller/60_EnergyOnOLED/mEnergyOnOLED.h"
  #define pCONT_mEnergyOnOLED        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER__ENERGY_OLED__ID])
#endif
/**
 * @brief 10 Controller Custom
 **/  
#ifdef USE_MODULE_CONTROLLER_RADIATORFAN
  #include "10_ConSpec/00_RadiatorFan/mRadiatorFan.h"
  #define pCONT_sbut                            static_cast<mRadiatorFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_RADIATORFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
  #include "10_ConSpec/01_ImmersionTankColour/mImmersionTankColour.h"
  #define pCONT_msenscol                        static_cast<mImmersionTankColour*>(pCONT->pModule[EM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
  #include "10_ConSpec/03_FurnaceSensor/mFurnaceSensor.h"
  #define pCONT_furnace_sensor                static_cast<mFurnaceSensor*>(pCONT->pModule[EM_MODULE_CONTROLLER_FURNACE_SENSOR_ID])
#endif
#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
  #include "10_ConSpec/04_LouvoliteHub/mLouvoliteHub.h"
  #define pCONT_louv                static_cast<mLouvoliteHub*>(pCONT->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
#endif
#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2
  #include "10_ConSpec/04v2_LouvoliteHub/mLouvoliteHub.h"
  #define pCONT_louv                static_cast<mLouvoliteHub*>(pCONT->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
  #include "10_ConSpec/05_SideDoorLight/mSideDoorLight.h"
  #define pCONT_sdlight                static_cast<mSideDoorLight*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHT__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
  #include "10_ConSpec/06_ImmersionPanel/mImmersionPanel.h"
  #define pCONT_immersion_cont         static_cast<mImmersionPanel*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
  #include "10_ConSpec/07_TempSensorOnOLED/mTempSensorOLEDBath.h"
  #define pCONT_immersion_cont         static_cast<mTempSensorOLEDBath*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX
  #include "10_ConSpec/08_CellularBlackBox/mCellularBlackBox.h"
  #define pCONT_immersion_cont         static_cast<mCellularBlackBox*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
  #include "10_ConSpec/09_mMAVLink_Decoder_OLED/mMAVLink_Decoder_OLED.h"
  #define pCONT_cont_mavlinoled         static_cast<mMAVLink_Decoder_OLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
  #include "10_ConSpec/10_EnergyOnOLED/mEnergyOnOLED.h"
  #define pCONT_mEnergyOnOLED        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE
  #include "10_ConSpec/11_3DPrinterEnclosure/Printer3D.h"
  #define pCONT_3dprinter        static_cast<mPrinter3D*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER
  #include "10_ConSpec/12_TreadmillLogger/mTreadmillLogger.h"
  #define pCONT_treadmill        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
  #include "9_Controller/SDLoggerIMURadiationPattern/mSDLogger.h"
  #define pCONT_cont_imu_rad                static_cast<mSDLoggerIMURadiationPattern*>(pCONT->pModule[EM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  #include "9_Controller/SerialPositionalLogger/mSerialPositionalLogger.h"
  #define pCONT_serial_pos_log                  static_cast<mSerialPositionalLogger*>(pCONT->pModule[EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
  #include "9_Controller/SerialCalibrationMeasurmentLogger/mSerialCalibrationMeasurmentLogger.h"
  #define pCONT_serial_calibration_log                  static_cast<mSerialCalibrationMeasurmentLogger*>(pCONT->pModule[EM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_USERMOD_01
  #include "9_Controller/UserMod_01/mUserMod_01.h"
  #define pCONT_usermod_01                  static_cast<mUserMod_01*>(pCONT->pModule[EM_MODULE_CONTROLLER_USERMOD_01_ID])
#endif


#include  "1_TaskerManager/mTaskerInterface.h"

class mTaskerManager{

  friend class mTaskerInterface;

  public:
  
    // mTaskerInterface* pModule[EM_MODULE_LENGTH_ID] = {nullptr}; // Set to nullptr so init can be checked

    std::vector<mTaskerInterface*> pModule;

    // vectorise this!

  private:
    /* Prevent others from being created */
    mTaskerManager(mTaskerManager const& other) = delete;
    mTaskerManager(mTaskerManager&& other) = delete;
    /* Private constructor to prevent instancing. */
    mTaskerManager(){};
    /* Here will be the instance stored. */
    static mTaskerManager* instance;
  public:
    // External function to get instance
    static mTaskerManager* GetInstance(){
      if (instance == nullptr){
        instance = new mTaskerManager();
      }
      return instance;
    };

    void addTasker(TaskerID id, mTaskerInterface* mod);

    int16_t GetModuleIndexbyFriendlyName(const char* c);
    int16_t GetModuleUniqueIDbyFriendlyName(const char* c);
    uint16_t GetModuleUniqueIDbyVectorIndex(uint8_t id);

    uint16_t GetModule_UniqueID_byFriendlyName(const char* c);
    
    PGM_P GetModuleFriendlyName_WithUniqueID(uint16_t unique_id);

    #if defined(ENABLE_ADVANCED_DEBUGGING) || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
      char buffer_taskname[50];
    #endif

    uint8_t Instance_Init();
    // uint8_t CheckPointersPass();
    
    int8_t Tasker_Interface(uint16_t function, uint16_t target_tasker = 0);    
    
    uint16_t GetClassSizeByID(uint8_t class_id);

    PGM_P GetModuleName(uint8_t id);
    PGM_P GetModuleFriendlyName(uint16_t module_id);

    uint16_t GetClassCount();

    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
    const char* GetTaskName(uint8_t task, char* buffer);
    #endif
    
    uint16_t GetEnumNumber_UsingModuleUniqueID(uint16_t unique_id);

    bool ValidTaskID(uint8_t module_id);
    
    void JSONCommand_Run(char* json);
    
    mTaskerInterface* GetModuleObjectbyUniqueID(uint16_t id);
    
    int16_t GetEnumVectorIndexbyModuleUniqueID(int16_t unique_id);

    #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
    struct DEBUG_MODULE_TIME
    {
      uint16_t seconds_to_keep_stats_before_reset = 0;
      uint16_t max_time = 0;
      // uint16_t min_time = 0;
      uint16_t avg_time = 0;
      uint32_t last_loop_time = 0;
      uint16_t max_function_id = 0; // func_task that caused the max_time
      // Only record them after boot sucessful
    }debug_module_time[EM_MODULE_LENGTH_ID];
    #endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES


    #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

    struct FUNCTION_EXECUTION_EVENT
    {
      TASKER_FUNCTION_TYPES function_id;
      uint16_t delay_millis;
      uint32_t tSaved_millis;
    };
    FUNCTION_EXECUTION_EVENT FunctionEvent(TASKER_FUNCTION_TYPES function_id, uint16_t delay_millis = 0)
    {
      FUNCTION_EXECUTION_EVENT task;
      task.tSaved_millis = millis();
      task.delay_millis = delay_millis;
      task.function_id = function_id;
      return task;
    }
    std::vector<FUNCTION_EXECUTION_EVENT> function_event_queue;
    #endif // ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

    

    uint16_t last_function = 255; // 0 will be first

    uint8_t fExitTaskerWithCompletion = false;

    uint16_t switch_index = 0;
    
    int16_t GetModuleIDbyFriendlyName(const char* c);

};


#endif
