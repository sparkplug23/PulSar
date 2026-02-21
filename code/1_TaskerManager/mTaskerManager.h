#ifndef mInterfaceController_H2
#define mInterfaceController_H2 1.1

#define USE_USER_MICHAEL

#ifdef USE_USER_MICHAEL
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

#include "1_TaskerManager/mTaskerInterface.h"  // MUST BE INCLUDED FIRST TET ADDED MAY2025


#include "2_CoreSystem/esp32_compat.h"
#include "2_CoreSystem/mGlobalMacros.h"

/**
 * SECTION: Minimal/Inital States
 */
#include "2_CoreSystem/mBaseConfig.h"           //DEFAULTS
#include "2_CoreSystem/Version.h"

/**
 * SECTION: User defined options/build patterns 
 */
#ifdef USE_USER_MICHAEL
#include "0_ConfigUser/Secret/ConfigUser.h"
#else
#include "0_ConfigUser/mFirmwareCustom_Secret.h"  // Fallback to generic as endpoint users
#endif

#include "0_ConfigUser/G1_mUserConfig_Secret.h"

/**
 * SECTION: Based on user options, expand these into the completed patterns
 */
#include "2_CoreSystem/00_FirmwareDefaults/mFirmwareDefaults.h"                    // Configuration overrides for all previous includes

// INCLUDE ORDER TO HERE OKAY


/**
 * SECTION: General Helpers
 */
#include "2_CoreSystem/06_Support/mSensorType.h"
#include <DeCounter.h>
#include "2_CoreSystem/11_Languages/mLanguageDefault.h"                           // Language support configured by .h
#include "2_CoreSystem/05_HardwarePins/mPins_Templates.h"                // Hardware configuration
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

    #ifndef WLED_DISABLE_ESPNOW
      #include <esp_now.h>
    #endif
  #endif

#endif // USE_MODULE_NETWORK_WEBSERVER

    #include "esp_task_wdt.h"

#ifdef USE_MODULE_CORE_FILESYSTEM
  #define LOROL_LITTLEFS   
    #ifdef LOROL_LITTLEFS
      #ifndef CONFIG_LITTLEFS_FOR_IDF_3_2
        #define CONFIG_LITTLEFS_FOR_IDF_3_2
      #endif
      #include <LittleFS.h>
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
#endif // USE_MODULE_CORE_FILESYSTEM


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
  FUNCTION_RESULT_HANDLED_ID, // e.g. button press handled by another module, so it can be ignored by hardcoded rules
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



// Libraries
// #include <StreamString.h>                   // Webserver, Updater

#ifdef USE_ARDUINO_OTA
  #include <ArduinoOTA.h>                   // Arduino OTA
#endif  // USE_ARDUINO_OTA

#ifdef USE_MODULE_CORE_I2C
  #include <Wire.h>                         // I2C support library
#endif

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

#include "2_CoreSystem/20_JSON/mJSON.h"
#include "2_CoreSystem/06_Support/mSupport.h"

#include "2_CoreSystem/02_RtcMemory/mRtcSettings.h"


    
  template <size_t N>
  static inline bool in_list(uint16_t id, const uint16_t (&lst)[N]) {
    for (size_t i = 0; i < N; ++i) if (lst[i] == id) return true;
    return false;
  }

enum MODULE_SUBTYPE_IDS{ //ignores the "interface"
  MODULE_SUBTYPE_CORE_ID,
  MODULE_SUBTYPE_NETWORK_ID,
  MODULE_SUBTYPE_DISPLAY_ID,
  MODULE_SUBTYPE_SENSOR_ID,
  MODULE_SUBTYPE_DRIVERS_ID,
  MODULE_SUBTYPE_LIGHTS_ID,
  MODULE_SUBTYPE_ENERGY_ID,
};

#define tkr mTaskerManager::GetInstance()


// macro to know if a module is a type (ie within a range)
#define IS_MODULE_SENSOR_SUBMODULE(x)   (x >= 5001 && x <= 5999) // skipping the interface
#define IS_MODULE_LIGHTING_SUBMODULE(x) (x >= 6001 && x <= 6999) // skipping the interface
#define IS_MODULE_ENERGY_SUBMODULE(x)   (x >= 7001 && x <= 7999) // skipping the interface
#define IS_MODULE_DISPLAY_SUBMODULE(x)  (x >= 8001 && x <= 8999) // skipping the interface

// Added to maybe fix header include issue
#include "3_Network/10_MQTT/mMQTT.h"

/**
 * @brief Core Modules
 **/
#ifdef USE_MODULE_CORE_SETTINGS
  #include "2_CoreSystem/01_Settings/mSettings.h"
  #define   tkr_set                               static_cast<mSettings*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_SETTINGS_ID))
#endif 
#ifdef USE_MODULE_CORE_FILESYSTEM
  #include "2_CoreSystem/03_FileSystem/mFileSystem.h"
  #define tkr_mfile                               static_cast<mFileSystem*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_FILESYSTEM_ID))
#endif
#ifdef USE_MODULE_CORE__JSON_TEMPLATE
  #include "2_CoreSystem/04_JsonTemplate/mJsonTemplate.h"
  #define tkr_json_template                       static_cast<mJsonTemplate*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_JSON_TEMPLATE_ID))
#endif
#ifdef USE_MODULE_CORE_HARDWAREPINS
  #include "2_CoreSystem/05_HardwarePins/mPins.h"
  #define   tkr_pins                              static_cast<mPins*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_HARDWAREPINS_ID))
#endif 
#ifdef USE_MODULE_CORE_TIME
  #include "2_CoreSystem/07_Time/mTime.h"
  #define   tkr_time                              static_cast<mTime*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_TIME_ID))
#endif 
#ifdef USE_MODULE_CORE_LOGGING
  #include "2_CoreSystem/08_Logging/mLogging.h"
  #define   tkr_log                               static_cast<mLogging*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_LOGGING_ID))
#endif 
#ifdef USE_MODULE_CORE_TELEMETRY
  #include "2_CoreSystem/09_Telemetry/mTelemetry.h"
  #define   tkr_tel                               static_cast<mTelemetry*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_TELEMETRY_ID))
#endif 
#ifdef USE_MODULE_CORE__CRASH_RECORDER
  #include "2_CoreSystem/10_CrashRecorder/CrashRecorder.h"
  #define   tkr_crash_recorder                               static_cast<CrashRecorder*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE__CRASH_RECORDER__ID))
#endif 
// #ifdef USE_MODULE_CORE_EVENTS
  #include "2_CoreSystem/12_Events/mEvents.h"
  #define   tkr_events                           static_cast<mEvent*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_EVENTS_ID))
// #endif
// #ifdef USE_MODULE_CORE_RULES
  #include "2_CoreSystem/13_RuleEngine/mRuleEngine.h"
  #define   tkr_rules                             static_cast<mRuleEngine*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_RULES_FRIENDLY_ID))
// #endif
#ifdef USE_MODULE_CORE_UPDATES
  #include "2_CoreSystem/14_Updates/mUpdates.h"
  #define   tkr_updates                           static_cast<mUpdates*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_UPDATES_ID))
#endif
#ifdef USE_MODULE_CORE__SERIAL
  #include "2_CoreSystem/15_SerialUART/mSerial.h"
  #define   tkr_Serial                           static_cast<mSerial*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE__SERIAL__ID))
#endif
// #ifdef USE_MODULE_CORE_SERIAL_UART
//   #include "2_CoreSystem/04b_SerialUART/mSerialUART.h"
//   #define tkr_uart                                static_cast<mSerialUART*>(tkr->pModule[EM_MODULE_CORE_SERIAL_UART_ID])
// #endif
#ifdef USE_MODULE_CORE_I2C
  #include "2_CoreSystem/31_I2C/mI2C.h"
  #define tkr_i2c                       static_cast<mI2C*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE__I2C__ID))
#endif
#ifdef USE_MODULE_CORE_SPI
  #include "2_CoreSystem/32_SPI/mSPI.h"
  #define tkr_spi                       static_cast<mSPI*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE__SPI__ID))
#endif
#ifdef USE_MODULE_CORE_SUPPORT
  #include "2_CoreSystem/06_Support/mSupport.h"
  #define   tkr_sup                               static_cast<mSupport*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_SUPPORT_ID))
#endif 
#ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
  #include "2_CoreSystem/99_DevelopmentDebugging/mDevelopmentDebugging.h"
  #define   tkr_debug             static_cast<mDevelopmentDebugging*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID))
#endif 
/**
 * @brief Network
 **/
#ifdef USE_MODULE_NETWORK_INTERFACE
  #include "3_Network/00_Interface/mInterface.h"
  #define tkr_interface_network                                static_cast<mInterfaceNetwork*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__NETWORK_INTERFACE__ID))
#endif 
#ifdef USE_MODULE_NETWORK_WIFI
  #include "3_Network/03_WiFi2/mWiFi.h"
  #define tkr_wifi                                 static_cast<mWiFi*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_NETWORK_WIFI_ID))
#endif 
#ifdef USE_MODULE_NETWORK_ETHERNET
  #include "3_Network/04_Ethernet/mEthernet.h"
  #define tkr_eth                                 static_cast<mEthernet*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_NETWORK_ETHERNET_ID))
#endif 
#ifdef USE_MODULE_NETWORK_CELLULAR
  #include "3_Network/05_Cellular/mCellular.h"
  #define tkr_cell                               static_cast<mCellular*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__NETWORK_CELLULAR__ID))
#endif
#ifdef USE_MODULE_DRIVERS_MODEM_7000G
  #include "3_Network/80_Modem_SIM7000G/mSIM7000G.h"
  #define tkr_sim7000g                           static_cast<mSIM7000G*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS_MODEM_7000G_ID))
#endif
#ifdef USE_MODULE_DRIVERS_MODEM_800L
#include "4_Drivers/81_Modem_SIM800L/mSIM800L.h"
  #define tkr_sim800l                           static_cast<mSIM800L*>(tkr->pModule[EM_MODULE_DRIVERS__MODEM_800L__ID])
#endif
#ifdef USE_MODULE_NETWORK_MQTT
  #include "3_Network/10_MQTT/mMQTT.h"
  #define tkr_mqtt                                static_cast<mMQTTManager*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_NETWORK_MQTT_ID))
#endif 
#ifdef USE_MODULE_NETWORK_WEBSERVER
  #include "3_Network/21_WebServer/_WebServer.h"
  #define tkr_web                                 static_cast<mWebServer*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_NETWORK_WEBSERVER_ID))
#endif
/**
 * @brief Displays
 **/
#ifdef USE_MODULE_DRIVERS_INTERFACE
  #include "4_Drivers/00_Interface/mDriverInterface.h"
  #define tkr_iDrivers                           static_cast<mDriverInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_DRIVERS_LEDS
#include "4_Drivers/03_LEDs/mLEDs.h"
  #define tkr_led                                static_cast<mLEDs*>(tkr->pModule[EM_MODULE_DRIVERS_LEDS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RELAY
  #include "4_Drivers/04_Relays/mRelays.h"
  #define tkr_relay                                 static_cast<mRelays*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS_RELAY_ID))
#endif
#ifdef USE_MODULE_DRIVERS_PWM
  #include "4_Drivers/PWM/mPWM.h"
  #define tkr_pwm                                 static_cast<mPWM*>(tkr->pModule[EM_MODULE_DRIVERS_PWM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  #include "4_Drivers/IRDevices/mIRtransceiver.h"
  #define tkr_mdirt                               static_cast<mIRtransceiver*>(tkr->pModule[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRREMOTE
#include "4_Drivers/IRRemote/mIRRemote.h"
  #define tkr_ir_remote                           static_cast<mIRRemote*>(tkr->pModule[EM_MODULE_DRIVERS_IRREMOTE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
  #include "4_Drivers/09_RCSwitch/mRCSwitch.h"
  #define tkr_rcswitch                            static_cast<mBuzzer*>(tkr->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
  #include "4_Drivers/10_RCSwitch_Extended/mRCSwitch_Extended.h"
  #define tkr_rcswitch                            static_cast<mRCSwitch*>(tkr->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID])
#endif
#ifdef USE_MODULE_DRIVERS_HBRIDGE
  #include "4_Drivers/Motors/HBridgeL9110/mHBridge.h"
  #define tkr_mdhbridge                           static_cast<mHBridge*>(tkr->pModule[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SDCARD
  #include "4_Drivers/SD/mSDCard.h"
  #define tkr_sdcard                              static_cast<mSDCard*>(tkr->pModule[EM_MODULE_DRIVERS_SDCARD_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  #include "4_Drivers/15_ShellyDimmer/mShellyDimmer.h"
  #define tkr_shelly                              static_cast<mShellyDimmer*>(tkr->pModule[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID])
#endif
#ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  #include "4_Drivers/20_Buzzer_Basic/mBuzzer.h"
  #define tkr_buzzer                              static_cast<mBuzzer*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS_BUZZER_BASIC_ID))
#endif
#ifdef USE_MODULE__DRIVERS_BUZZER_TONES
  #include "4_Drivers/21_Buzzer_Tones/mBuzzer.h"
  #define tkr_buzzer                              static_cast<mBuzzer*>(tkr->pModule[EM_MODULE__DRIVERS_BUZZER_TONES__ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_2025
  #include "4_Drivers/50_Camera_2025/mCamera.h"
  #define tkr_camera                              static_cast<mCamera*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS__CAMERA_ID))
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
  #include "4_Drivers/52_WebCamera/mWebCamera.h"
  #define tkr_camera                              static_cast<mWebCamera*>(tkr->pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO
  #include "4_Drivers/60_WebCam_Arduino/mWebCam.h"
  #define tkr_camera                              static_cast<mWebCamera*>(tkr->pModule[EM_MODULE_DRIVERS__CAMERA_ARDUINO__ID])
#endif
#ifdef USE_MODULE_DRIVERS__CAMERA_TAS25
  #include "4_Drivers/63_WebCam_Tas25/mCamera.h"
  #define tkr_camera                              static_cast<mCamera*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DRIVERS__CAMERA_TAS25_ID))
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  #include "4_Drivers/70_MAVLink_Decoder/mMAVLink_Decoder.h"
  #define tkr_mavlink                              static_cast<mMAVLink_Decoder*>(tkr->pModule[EM_MODULE__DRIVERS_MAVLINK_DECODER__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI
  #include "4_Drivers/71_MAVLink_Telemetry_WiFi/mMAVLink_Telemetry_WiFi.h"
  #define tkr_mavlink                              static_cast<mMAVLink_Telemetry_WiFi*>(tkr->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__ID])
#endif
#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR
  #include "4_Drivers/72_MAVLink_Telemetry_Cellular/mMAVLink_Telemetry_Cellular.h"
  #define tkr_mavlink                              static_cast<mMAVLink_Telemetry_Cellular*>(tkr->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR__ID])
#endif
/**
 * @brief Sensors
 **/
#ifdef USE_MODULE_SENSORS_INTERFACE
  #include "5_Sensors/00_Interface/mSensorsInterface.h"
  #define tkr_iSensors                         static_cast<mSensorsInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_SENSORS_SWITCHES
  #include "5_Sensors/01_Switches/mSwitches.h"
  #define tkr_switch                            static_cast<mSwitches*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_SWITCHES_ID))
#endif
#ifdef USE_MODULE_SENSORS_BUTTONS
  #include "5_Sensors/02_Buttons/mButtons.h"
  #define tkr_button                         static_cast<mButtons*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_BUTTONS_ID))
#endif
#ifdef USE_MODULE_SENSORS_BME
  #include "5_Sensors/03_BME/mBME.h"
  #define tkr_bme                             static_cast<mBME*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_BME_ID))
#endif
#ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  #include "5_Sensors/04_DB18x20/mDB18x20.h"
  #define tkr_db18                      static_cast<mDB18x20*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__DS18X20_ESP32_2023__ID))
#endif
#ifdef USE_MODULE_SENSORS_DHT
  #include "5_Sensors/05_DHT/mSensorsDHT.h"
  #define tkr_dht                             static_cast<mSensorsDHT*>(tkr->pModule[EM_MODULE_SENSORS_DHT_ID])
#endif
#ifdef USE_MODULE_SENSORS_BH1750
  #include "5_Sensors/06_BH1750Light/mBH1750.h"
  #define tkr_bh1750                          static_cast<mBH1750*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_BH1750_ID))
#endif
#ifdef USE_MODULE_SENSORS_PIR
  #include "5_Sensors/07_PIR/mPIR.h"
  #define tkr_motion                            static_cast<mPIR*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_PIR_ID))
#endif
#ifdef USE_MODULE_SENSORS_DOOR
  #include "5_Sensors/11_Door/mDoorSensor.h"
  #define tkr_sdoor                           static_cast<mDoorSensor*>(tkr->pModule[EM_MODULE_SENSORS_DOOR_ID])
#endif
#ifdef USE_MODULE_SENSORS_L3G
  #include "5_Sensors/L3GD20_3Axis_Gryo/mSensorsL3G.h"
  #define tkr_L3G                      static_cast<mSensorsL3G*>(tkr->pModule[EM_MODULE_SENSORS_L3G_ID])
#endif
#ifdef USE_MODULE_SENSORS_LDR_BASIC
  #include "5_Sensors/14_LDRBasic/mLDRBasic.h"
  #define tkr_ldr_basic                      static_cast<mLDRBasic*>(tkr->pModule[EM_MODULE_SENSORS_LDR_BASIC_ID])
#endif
#ifdef USE_MODULE_SENSORS_LSM303D
  #include "5_Sensors/LSM303D_3Axis_AccMag/mSensorsLSM303D.h"
  #define tkr_LSM303D                      static_cast<mSensorsLSM303D*>(tkr->pModule[EM_MODULE_SENSORS_LSM303D_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOISTURE
  #include "5_Sensors/Moisture/mMoistureSensor.h"
  #define tkr_srmoisture                      static_cast<mMoistureSensor*>(tkr->pModule[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID])
#endif
#ifdef USE_MODULE_SENSORS_SR04
  #include "5_Sensors/17_SR04/mSR04.h"
  #define tkr_sr04                              static_cast<mSR04*>(tkr->pModule[EM_MODULE_SENSORS_SR04_ID])
#endif
#ifdef USE_MODULE_SENSORS_MPU9250
  #include "5_Sensors/MPU9250/mSensorsMPU9250.h"
  #define tkr_MPU9250                      static_cast<mSensorsMPU9250*>(tkr->pModule[EM_MODULE_SENSORS_MPU9250_ID])
#endif
#ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  #include "5_Sensors/PulseCounter/mPulseCounter.h"
  #define tkr_spulse                          static_cast<mPulseCounter*>(tkr->pModule[EM_MODULE_SENSORS_PULSECOUNTER_ID])
#endif
#ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
  #include "5_Sensors/20_RemoteDevice/mRemoteDevice.h"
  #define tkr_sremote                           static_cast<mRemoteDevice*>(tkr->pModule[EM_MODULE_SENSORS_REMOTE_DEVICE_ID])
#endif
#ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
  #include "5_Sensors/21_RotaryEncoder/mRotaryEncoder.h"
  #define tkr_rotary_encoder               static_cast<mRotaryEncoder*>(tkr->pModule[EM_MODULE_SENSORS_ROTARY_ENCODER_ID])
#endif
#if defined(USE_MODULE_SENSORS_SUN_TRACKING) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
  #include "5_Sensors/22_SunTracking/mSunTracking.h"
  #define   tkr_solar                             static_cast<mSunTracking*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_ID))
#endif
#if defined(USE_MODULE_SENSORS_SUN_TRACKING2) || defined(USE_MODULE_SENSORS_SUN_TRACKING__BASIC_ESTIMATE)
  #include "5_Sensors/22b_SunTracking/mSunTracking.h"
  #define   tkr_solar                             static_cast<mSunTracking*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_SUN_TRACKING_ID))
#endif
#ifdef USE_MODULE_SENSORS_ULTRASONICS
  #include "5_Sensors/UltraSonic/mUltraSonicSensor.h"
  #define tkr_ult                             static_cast<mUltraSonicSensor*>(tkr->pModule[EM_MODULE_SENSORS_ULTRASONIC_ID])
#endif
#ifdef USE_MODULE_SENSORS__TOF_VL53L0X
  #include "5_Sensors/26_TOF_VL53L0X/mTOF_VL53L0X.h"
  #define tkr_tof_vl0x                         static_cast<mTOF_VL53L0X*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS__TOF_VL53L0X__ID))
#endif
#ifdef USE_MODULE_SENSORS__TOF_VL53L1X
  #include "5_Sensors/27_TOF_VL53L1X/mTOF_VL53L1X.h"
  #define tkr_tof_vl1x                         static_cast<mTOF_VL53L1X*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS__TOF_VL53L1X__ID))
#endif
#ifdef USE_MODULE_SENSORS__RADAR_HLK_LD2410
  #include "5_Sensors/28_Radar_HLK_LD2410/Radar_HLK_LD2410.h"
  #define tkr_radar_ld2410                     static_cast<mHLK_LD2410*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS__HLK_LD2410__ID))
#endif
#ifdef USE_MODULE_SENSORS_ADC_INTERNAL
  #include "5_Sensors/30_ADCInternal/mADCInternal.h"
  #define tkr_adc_internal                          static_cast<mADCInternal*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS_ADC_INTERNAL_ID))
#endif
#ifdef USE_MODULE_SENSORS_GPS_SERIAL
  #include "5_Sensors/50_GPS_Serial/mGPS_Serial.h"
  #define tkr_gps                                static_cast<mGPS_Serial*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_SENSORS__GPS_SERIAL_ID))
#endif
#ifdef USE_MODULE_SENSORS_GPS_MODEM
  #include "5_Sensors/51_GPS_Modem/mGPS_Modem.h"
  #define tkr_gps                                 static_cast<mGPS_Modem*>(tkr->pModule[EM_MODULE__SENSORS_GPS_MODEM__ID])
#endif
#ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  #include "5_Sensors/52_Battery_Modem/mBattery_Modem.h"
  #define tkr_batt_modem                                 static_cast<mBattery_Modem*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__SENSORS_BATTERY_MODEM__ID))
  //static_cast<mBattery_Modem*>(tkr->pModule[EM_MODULE__SENSORS_BATTERY_MODEM__ID])
#endif
/**
 * @brief Lights
 **/
#ifdef USE_MODULE_LIGHTS_INTERFACE
  #include "6_Lights/00_Interface/mInterfaceLight.h"
  #define tkr_iLight                          static_cast<mInterfaceLight*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_LIGHTS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_LIGHTS_ANIMATOR
  #include "6_Lights/03_Animator/_AnimatorLight.h"
  #define tkr_lAni                            static_cast<mAnimatorLight*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID))
  #ifdef ENABLE_FEATURE_LIGHTS__GLOBAL_ANIMATOR_LIGHT_CLASS_ACCESS
  #define tkr_anim tkr_extern_lAni // using a more direct access method, with a local pointer in the class header
  #else
  #define tkr_anim tkr_lAni // pointer to the instance of the mAnimatorLight class
  #endif
#endif
/**
 * @brief Energy
 **/
#ifdef USE_MODULE_ENERGY_INTERFACE
  #include "7_Energy/00_Interface/mEnergyInterface.h"
  #define tkr_iEnergy                           static_cast<mEnergyInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_ENERGY_INTERFACE_ID))
#endif
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  #include "7_Energy/01_PZEM004T/mPZEM004T.h"
  #define tkr_pzem                              static_cast<mEnergyPZEM004T*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_ENERGY_PZEM004T_ID))
#endif
#ifdef USE_MODULE_ENERGY_ADE7953
  #include "7_Energy/02_ADE7953/mADE7953.h"
  #define tkr_ade7953                         static_cast<mEnergyADE7953*>(tkr->pModule[EM_MODULE_ENERGY_ADE7953_ID])
#endif
#ifdef USE_MODULE_ENERGY_INA219
  #include "7_Energy/03_INA219/mINA219.h"
  #define tkr_mina219                         static_cast<mEnergyINA219*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_ENERGY_INA219_ID))
#endif
/**
 * @brief Displays
 **/
#ifdef USE_MODULE_DISPLAYS_INTERFACE
  #include "8_Displays/00_Interface/mDisplaysInterface.h"
  #define tkr_iDisp                               static_cast<mDisplaysInterface*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DISPLAYS_INTERFACE_ID))
#endif
#ifdef USE_MODULE_DISPLAYS_NEXTION
  #include "8_Displays/01_Nextion/mNextion.h"
  #define tkr_nex                                 static_cast<mNextion*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DISPLAYS_NEXTION_ID))
#endif
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  #include "8_Displays/02_OLED_SSD1606/mOLED_SSD1306.h"
  #define tkr_oled1306                            static_cast<mOLED_SSD1306*>(tkr->pModule[EM_MODULE_DISPLAYS_OLED_SSD1306_ID])
#endif
#ifdef USE_MODULE_DISPLAYS_OLED_SH1106
  #include "8_Displays/03_OLED_SH1106/mOLED_SH1106.h"
  #define tkr_oled1306                            static_cast<mOLED_SH1106*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_DISPLAYS_OLED_SH1106_ID))
#endif
/**
 * @brief Controller Generic (Generic)
 **/
#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
  #include "9_Controller/03_Sonoff_iFan/mSonoffIFan.h"
  #define tkr_ifan                            static_cast<mSonoffIFan*>(tkr->pModule[EM_MODULE_CONTROLLER_SONOFF_IFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_FAN
  #include "9_Controller/Fan/mFan.h"
  #define tkr_mfan                            static_cast<mFan*>(tkr->pModule[EM_MODULE_CONTROLLER_FAN_ID])
#endif
// 3d printer encoder here
#ifdef USE_MODULE_CONTROLLER_TANKVOLUME
  #include "9_Controller/08_TankVolume/mTankVolume.h"
  #define tkr_tankvolume                      static_cast<mTankVolume*>(tkr->pModule[EM_MODULE_CONTROLLER_TANKVOLUME_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_BLINDS
  #include "9_Controller/Blinds/mBlinds.h"
  #define tkr_sbut                            static_cast<mBlinds*>(tkr->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
  #include "9_Controller/BucketWaterLevel/mBucketWaterLevel.h"
  #define tkr_bucket_water_level                static_cast<mBucketWaterLevel*>(tkr->pModule[EM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_DOORCHIME
  #include "9_Controller/DoorBell/mDoorBell.h"
  #define tkr_doorbell                        static_cast<mDoorBell*>(tkr->pModule[EM_MODULE_CONTROLLER_DOORBELL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
  #include "9_Controller/GPS_SD_Logger/mGPS_SD_Logger.h"
  #define tkr_gps_sd_log                  static_cast<mGPS_SD_Logger*>(tkr->pModule[EM_MODULE_CONTROLLER_GPS_SD_LOGGER_ID])
  #define tkr_serial_pos_log  tkr_gps_sd_log
#endif
#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
  #include "9_Controller/SDCardLogger/mSDCardLogger.h"
  #define tkr_sdcardlogger                    static_cast<mDoorBell*>(tkr->pModule[EM_MODULE_CONTROLLER_SDCARDLOGGER_ID])
#endif
// USE_MODULE_CONTROLLER_UARTLOGGER
// USE_MODULE_CONTROLLER_INTERNAL_CLOCK
#ifdef USE_MODULE_CONTROLLER_HVAC
  #include "9_Controller/40_HVAC/mHVAC.h"
  #define tkr_hvac                        static_cast<mHVAC*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLER_HVAC_ID))
#endif
#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE
  #include "9_Controller/41_HVAC_Remote/mHVAC.h"
  #define tkr_hvac_remote                        static_cast<mHVAC_Remote*>(tkr->pModule[EM_MODULE_CONTROLLER__HVAC_REMOTE__ID])
#endif
#ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  #include "9_Controller/42_SensorColourBar/mSensorColourBar.h"
  #define tkr_colourbar                        static_cast<mSensorColourBar*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__ID))
#endif
#ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  #include "9_Controller/43_RelayStateLEDStrip/mRelayStateLEDStrip.h"
  #define tkr_relay_strip                      static_cast<mRelayStateLEDStrip*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__ID))
#endif
// 50_Animator_Input_Controller
// 51_Animator_Playlists
#ifdef USE_MODULE_CONTROLLER__ENERGY_OLED
  #include "9_Controller/60_EnergyOnOLED/mEnergyOnOLED.h"
  #define tkr_mEnergyOnOLED        static_cast<mEnergyOnOLED*>(tkr->pModule[EM_MODULE_CONTROLLER__ENERGY_OLED__ID])
#endif
/**
 * @brief 10 Controller Custom
 **/  
#ifdef USE_MODULE_CONTROLLER_RADIATORFAN
  #include "10_ConSpec/00_RadiatorFan/mRadiatorFan.h"
  #define tkr_sbut                            static_cast<mRadiatorFan*>(tkr->pModule[EM_MODULE_CONTROLLER_RADIATORFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
  #include "10_ConSpec/01_ImmersionTankColour/mImmersionTankColour.h"
  #define tkr_msenscol                        static_cast<mImmersionTankColour*>(tkr->pModule[EM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
  #include "10_ConSpec/03_FurnaceSensor/mFurnaceSensor.h"
  #define tkr_furnace_sensor                static_cast<mFurnaceSensor*>(tkr->pModule[EM_MODULE_CONTROLLER_FURNACE_SENSOR_ID])
#endif
#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
  #include "10_ConSpec/04_LouvoliteHub/mLouvoliteHub.h"
  #define tkr_louv                static_cast<mLouvoliteHub*>(tkr->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
#endif
#ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2
  #include "10_ConSpec/04v2_LouvoliteHub/mLouvoliteHub.h"
  #define tkr_louv                static_cast<mLouvoliteHub*>(tkr->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
  #include "10_ConSpec/05_SideDoorLight/mSideDoorLight.h"
  #define tkr_sdlight                static_cast<mSideDoorLight*>(tkr->pModule[EM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHT__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
  #include "10_ConSpec/06_ImmersionPanel/mImmersionPanel.h"
  #define tkr_immersion_cont         static_cast<mImmersionPanel*>(tkr->pModule[EM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
  #include "10_ConSpec/07_TempSensorOnOLED/mTempSensorOLEDBath.h"
  #define tkr_immersion_cont         static_cast<mTempSensorOLEDBath*>(tkr->pModule[EM_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX
  #include "10_ConSpec/08_CellularBlackBox/mCellularBlackBox.h"
  #define tkr_immersion_cont         static_cast<mCellularBlackBox*>(tkr->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
  #include "10_ConSpec/09_mMAVLink_Decoder_OLED/mMAVLink_Decoder_OLED.h"
  #define tkr_cont_mavlinoled         static_cast<mMAVLink_Decoder_OLED*>(tkr->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
  #include "10_ConSpec/10_EnergyOnOLED/mEnergyOnOLED.h"
  #define tkr_mEnergyOnOLED        static_cast<mEnergyOnOLED*>(tkr->pModule[EM_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE
  #include "10_ConSpec/11_3DPrinterEnclosure/Printer3D.h"
  #define tkr_3dprinter        static_cast<mPrinter3D*>(tkr->pModule[EM_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER
  #include "10_ConSpec/12_TreadmillLogger/mTreadmillLogger.h"
  #define tkr_treadmill        static_cast<mTreadmillLogger*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_ID))
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS
  #include "10_ConSpec/13_ServerResetRelays/mServerResetRelays.h"
  #define tkr_server_reset_relays        static_cast<mServerResetRelays*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLER_CUSTOM__SERVER_RESET_RELAYS_ID))
#endif
#ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
  #include "9_Controller/SDLoggerIMURadiationPattern/mSDLogger.h"
  #define tkr_cont_imu_rad                static_cast<mSDLoggerIMURadiationPattern*>(tkr->pModule[EM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
  #include "9_Controller/SerialPositionalLogger/mSerialPositionalLogger.h"
  #define tkr_serial_pos_log                  static_cast<mSerialPositionalLogger*>(tkr->pModule[EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
  #include "9_Controller/SerialCalibrationMeasurmentLogger/mSerialCalibrationMeasurmentLogger.h"
  #define tkr_serial_calibration_log                  static_cast<mSerialCalibrationMeasurmentLogger*>(tkr->pModule[EM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS
  #include "10_ConSpec/19_MAVLinkFlyingLEDS/mMAVLinkFlyingLEDS.h"
  #define tkr_mavlink_leds                            static_cast<mMavlinkFlyingLEDS*>(tkr->pModule[TaskerID::CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS__ID])
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED
  #include "10_ConSpec/20_DeskSensorsOnOLED/mDeskSensorsOnOLED.h"
  #define tkr_DeskSensorsOnOLED        static_cast<mDeskSensorsOnOLED*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_ID))
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS
  #include "10_ConSpec/21_LightNeo_MotionAlerts/mLightNeo_MotionAlerts.h"
  #define tkrCC_motion_alerts        static_cast<mLightNeo_MotionAlerts*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_MOTION_ALERTS__ID))
#endif
#ifdef USE_MODULE_CONTROLLER_CUSTOM__LIGHTNEO_RADAR_DISTANCE
  #include "10_ConSpec/22_LightNeo_RadarDistance/mLightNeo_RadarDistance.h"
  #define tkrCC_motion_alerts        static_cast<mLightNeo_RadarDistance*>(mTaskerManager::GetInstance()->GetModule(D_UNIQUE_MODULE__CONTROLLER_CUSTOM__LIGHTNEO_RADAR_DISTANCE__ID))
#endif
#ifdef USE_MODULE_CONTROLLER_USERMOD_01
  #include "9_Controller/UserMod_01/mUserMod_01.h"
  #define tkr_usermod_01                  static_cast<mUserMod_01*>(tkr->pModule[EM_MODULE_CONTROLLER_USERMOD_01_ID])
#endif


#include  "1_TaskerManager/mTaskerInterface.h"

class mTaskerManager{

  friend class mTaskerInterface;

  public:

    std::vector<mTaskerInterface*> pModule;
    
  private:
    /* Prevent others from being created */
    mTaskerManager(mTaskerManager const& other) = delete;
    mTaskerManager(mTaskerManager&& other) = delete;
    /* Private constructor to prevent instancing. */
    mTaskerManager(){};
    /* Here will be the instance stored. */
    static mTaskerManager* instance;

    ~mTaskerManager(){ Serial.println("Destructor, should never reach this"); Serial.flush(); }; // Destructor
    
    
  public:
    // External function to get instance
    static mTaskerManager* GetInstance(){
      if (instance == nullptr){
        instance = new mTaskerManager();
      }
      return instance;
    };

    // Register a new module
    void addTasker(mTaskerInterface* module);

    // Find a module by unique ID
    mTaskerInterface* GetModule(uint16_t uniqueID) const {
        auto it = std::find_if(pModule.begin(), pModule.end(), 
            [uniqueID](mTaskerInterface* module) {
                return module->GetModuleUniqueID() == uniqueID;
            });
        return (it != pModule.end()) ? *it : nullptr;
    }

    // Find a module by name
    // Flash only
    mTaskerInterface* GetModule_F(const char* name) const {
        char buffer[64];  // Adjust size as needed
        auto it = std::find_if(pModule.begin(), pModule.end(),
            [name, &buffer](mTaskerInterface* module) {
                strcpy_P(buffer, module->GetModuleName());  // Copy from PROGMEM to RAM
                return strcmp(name, buffer) == 0;
            });
        return (it != pModule.end()) ? *it : nullptr;
    }
    mTaskerInterface* GetModule_P(const char* name) const {
        auto it = std::find_if(pModule.begin(), pModule.end(),
            [name](mTaskerInterface* module) {
                return strcmp_P(name, module->GetModuleName()) == 0; // GetModuleName is PROGMEM, and strcmp is RAM only, PROGMEM is read only
            });
        return (it != pModule.end()) ? *it : nullptr;
    }

    #ifdef ENABLE_DEBUGFEATURE_LOGGING__RESTRICT_SERIAL_LOGS_TO_MODULE
    [[gnu::hot]] uint16_t module_id_being_serviced = 0;
    #endif


    // Function to find a module class name by unique ID
    const char* GetModuleName(uint16_t uniqueID) const {
        auto it = std::find_if(pModule.begin(), pModule.end(),
            [uniqueID](mTaskerInterface* module) {
                return module->GetModuleUniqueID() == uniqueID;
            });

        if (it != pModule.end()) {
            return (*it)->GetModuleName();
        }

        return nullptr; // Return nullptr if the module is not found
    }
    
    // Function to find a module unique ID by class name
    // From now on, no module is allowed to be called number 0 as unique ID so it can be reserved for no match
    // uint16_t GetModuleID(const char* name) const {
    //   auto it = std::find_if(pModule.begin(), pModule.end(),
    //       [name](mTaskerInterface* module) {
    //           // Use strcmp_P if name is stored in PROGMEM
    //           return strcmp_P(name, module->GetModuleName()) == 0;
    //       });

    //       if (it != pModule.end()) {
    //           return (*it)->GetModuleUniqueID();
    //     }

    //     return 0; // Return 0 or another appropriate invalid ID if the module is not found
    // }
    uint16_t GetModuleID(const char* name, bool caseInsensitive = false) const {
      auto it = std::find_if(pModule.begin(), pModule.end(),
          [name, caseInsensitive](mTaskerInterface* module) {
              const char* namePtr = name;
              const char* modName = module->GetModuleName();
              char c1, c2;
  
              while ((c1 = *namePtr++) != 0) {
                  c2 = pgm_read_byte(modName++);
                  if (caseInsensitive) {
                      c1 = tolower(c1);
                      c2 = tolower(c2);
                  }
                  if (c1 != c2) return false;
              }
              return pgm_read_byte(modName) == 0;
          });
  
      if (it != pModule.end()) {
          return (*it)->GetModuleUniqueID();
      }
  
      return 0;
  }

    #ifdef ENABLE_DEBUGFEATURE_TASKER__DEBUG_MEMORY_PER_MODULE
    struct MemDeltaLast {
        int32_t last_free_used;     // bytes used in last call  (before - after)
        int32_t last_lb_used;       // bytes lost in largest block (before - after)
        int32_t max_free_used;      // max bytes used in any single call
        int32_t max_lb_used;        // max lb loss in any single call
      };

      MemDeltaLast memdelta_last[64];  // set >= max modules
    struct MemDeltaStats {
      int32_t  free8_sum;        // bytes
      int32_t  largest8_sum;     // bytes
      int16_t  free8_min_delta;  // bytes (most negative single-call delta, clamp)
      int16_t  largest8_min_delta;
      uint32_t samples;
    };
    
int32_t sum_free_used[64] = {0};
int32_t sum_lb_used[64]   = {0};
uint32_t last_second_ms = 0;

const int32_t ALERT_FREE_PER_CALL = 256;
const int32_t ALERT_LB_PER_CALL   = 512;

const int32_t ALERT_FREE_PER_SEC  = 1024;
const int32_t ALERT_LB_PER_SEC    = 2048;

    MemDeltaStats memstats[64];   // set to your max modules
    static inline uint32_t HeapFree8() {
      return heap_caps_get_free_size(MALLOC_CAP_8BIT);
    }
    static inline uint32_t HeapLargest8() {
      return heap_caps_get_largest_free_block(MALLOC_CAP_8BIT);
    }
    #endif

    

    uint8_t Instance_Init();
    
    int8_t Tasker_Interface(uint16_t function);    
        
    uint16_t GetClassCount()
    {
      return pModule.size();
    }

    const char* GetTaskName(uint16_t task);
    const char* GetTaskName_Full(uint16_t task);
    
    
    void JSONCommand_Run(char* json);
    

    uint16_t last_function = 255; // 0 will be first

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



    #if defined(ENABLE_ADVANCED_DEBUGGING) || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
      char buffer_taskname[50];
    #endif

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
    };
    std::vector<DEBUG_MODULE_TIME> debug_module_time;
    #endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

    #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
    struct TaskMetrics {
      TASKER_FUNCTION_TYPES task_id;   // Task being monitored
      uint16_t unique_id;
      uint32_t max_time;      // Maximum time in microseconds
      uint32_t min_time;      // Minimum time in microseconds
      uint32_t total_time;    // Total time for calculating the average
      uint32_t count;         // Number of times the task was executed
      uint32_t avg_time;      // Average time in microseconds
    };
    #endif
    #ifdef ENABLE_DEBUGFEATURE_TASKERMANAGER__ADVANCED_METRICS
    std::vector<TASKER_FUNCTION_TYPES> monitor_task; // Vector to hold the tasks to monitor
    std::vector<TaskMetrics> task_metrics;
    #endif



    

};


#endif
