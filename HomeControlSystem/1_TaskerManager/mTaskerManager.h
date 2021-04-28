#ifndef mInterfaceController_H2
#define mInterfaceController_H2 1.1

#define D_USER_MICHAEL

#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#include <vector>

#include "JsonParser.h"

#include "2_CoreSystem/esp32_compat.h"
#include "2_CoreSystem/mGlobalMacros.h"

// Minimal base configurations required for stable operation
#include "2_CoreSystem/mBaseConfig.h"           //DEFAULTS
// Optional user configs, which override defaults

#include "2_CoreSystem/Support/AveragingData.h"

#ifndef D_USER_MICHAEL // Include my personally named secret file
#include "0_ConfigUser/mFirmwareCustom_Secret.h"
#include "0_ConfigUser/mUserConfig_Secret.h"
#endif // D_USER_MICHAEL // Include my personally named secret file

#ifdef D_USER_MICHAEL // Include my personally named secret file
#include "0_ConfigUser/mFirmwareCustom_Secret_Michael.h"
#include "0_ConfigUser/mUserConfig_Secret_Michael.h"    
#endif // D_USER_MICHAEL

#ifdef USE_MODULE_CORE_RULES
#include "2_CoreSystem/RuleEngine/mRuleEngine.h"
#endif

// #ifdef D_USER_MICHAEL // Include my personally named secret file
// #include "0_ConfigUser/mFirmwareCustom_Secret_Michael.h"
// #include "0_ConfigUser/mUserConfig_Secret_Michael.h"    
// #else
// #include "0_ConfigUser/mFirmwareCustom_Secret.h"
// #include "0_ConfigUser/mUserConfig_Secret.h"
// #endif // D_USER_MICHAEL


#include "2_CoreSystem/mFirmwareDefaults.h"                    // Configuration overrides for all previous includes
#include "2_CoreSystem/Languages/mLanguageDefault.h"                           // Language support configured by .h
#include "1_TaskerManager/mTaskerManager_Defines.h"
#include "2_CoreSystem/HardwareTemplates/mHardwareTemplates.h"                // Hardware configuration
#include "2_CoreSystem/Support/BufferWriter.h"

#include "1_TaskerManager/mTasks.h"
#include <WiFiClient.h>
#ifdef ESP8266
  #include <core_version.h>                   // Arduino_Esp8266 version information (ARDUINO_ESP8266_RELEASE and ARDUINO_ESP8266_RELEASE_2_3_0)
#endif

#ifdef ESP8266
  #include "2_CoreSystem/Support/SupportESP8266.h"
  #define mSupportHardware SupportESP8266
#endif
#ifdef ESP32
  #include "2_CoreSystem/Support/SupportESP32.h"
  #define SupportHardware SupportESP32
#endif

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
  const int wdtTimeout = 60000;  //time in ms to trigger the watchdog
  // hw_timer_t *timerwdt = NULL;
  // void IRAM_ATTR resetModule(){ets_printf("\n\n\n\n\n\nWDT REBOOTING!!\n");ESP.restart();}
  #define WDT_RESET() //timerWrite(timerwdt, 0) //reset timerwdt (feed watchdog)
#endif
#ifdef ESP8266
  #include <ESP8266HTTPClient.h>
  #include <ESP8266httpUpdate.h>
  // #ifdef USE_NETWORK_MDNS
  // // #include <ESP8266mDNS.h>
  // #endif // #ifdef USE_NETWORK_MDNS
  
  #ifdef USE_DISCOVERY
    #include <ESP8266mDNS.h>                  // MQTT, Webserver, Arduino OTA
  #endif  // USE_DISCOVERY

  #include <ArduinoOTA.h>
  #include <WiFiUdp.h>
  #define WDT_RESET() ESP.wdtFeed()
  #include <ESP8266WiFi.h>
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <ESPAsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  #endif //USE_MODULE_NETWORK_WEBSERVER
#endif

#include "2_CoreSystem/JSON/mJSON.h"

#include "2_CoreSystem/Support/mSupport.h"


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

#define D_TARGET_TASKER_NONE 0

//Consider moving unique number inside the class, then further references to it must be called via array index (with reverse lookup)
//This would remove the need to have GetVectorIndex below, and IDs can be used directly
// D_MODULE_x_ID would need to be an enum list above this line
//Notice, vector.push_back order would need to align with this, making the use of arrays a better idea
enum MODULE_IDS{
  // Core
  #ifdef USE_MODULE_CORE_HARDWAREPINS
    EM_MODULE_CORE_HARDWAREPINS_ID,
  #endif 
  #ifdef USE_MODULE_CORE_SETTINGS
    EM_MODULE_CORE_SETTINGS_ID,
  #endif 
  #ifdef USE_MODULE_CORE_SUPPORT
    EM_MODULE_CORE_SUPPORT_ID,
  #endif 
  #ifdef USE_MODULE_CORE_LOGGING
    EM_MODULE_CORE_LOGGING_ID,
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
    EM_MODULE_CORE_TELEMETRY_ID,
  #endif 
  #ifdef USE_MODULE_CORE_TIME
    EM_MODULE_CORE_TIME_ID,
  #endif 
  #ifdef USE_MODULE_CORE_RULES
    EM_MODULE_CORE_RULES_ID,
  #endif 
  #ifdef USE_MODULE_CORE_UPDATES
    EM_MODULE_CORE_UPDATES_ID,
  #endif 
  // Network
  #ifdef USE_MODULE_NETWORK_WIFI
    EM_MODULE_NETWORK_WIFI_ID,
  #endif 
  #ifdef USE_MODULE_NETWORK_MQTT
    EM_MODULE_NETWORK_MQTT_ID,
  #endif 
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    EM_MODULE_NETWORK_WEBSERVER_ID,
  #endif
  // Displays
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
    EM_MODULE_DISPLAYS_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    EM_MODULE_DISPLAYS_NEXTION_ID,
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
    EM_MODULE_DISPLAYS_OLED_SSD1306_ID,
  #endif
  // Drivers (Range 40-129)
  #ifdef USE_MODULE_DRIVERS_INTERFACE
    // EM_MODULE_DRIVERS_HBRIDGE_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    EM_MODULE_DRIVERS_HBRIDGE_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    EM_MODULE_DRIVERS_IRTRANSCEIVER_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    EM_MODULE_DRIVERS_RELAY_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM
    EM_MODULE_DRIVERS_PWM_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433MHZ
    EM_MSAW_MODULE_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    EM_MODULE_DRIVERS_SDCARD_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_GPS
    EM_MODULE_DRIVERS_GPS_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_SERIAL_UART
    EM_MODULE_DRIVERS_SERIAL_UART_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    EM_MODULE_DRIVERS_SHELLY_DIMMER_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    EM_MODULE_DRIVERS_CAMERA_OV2640_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
    EM_MODULE_DRIVERS_STATUS_LEDS_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
    EM_MODULE_DRIVERS_FILESYSTEM_ID,
  #endif
  #ifdef USE_MODULE_DRIVERS_BUZZER
    EM_MODULE_DRIVERS_BUZZER_ID,
  #endif
  // Energy
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
  // Lights
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    EM_MODULE_LIGHTS_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    EM_MODULE_LIGHTS_ANIMATOR_ID,
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    EM_MODULE_LIGHTS_ADDRESSABLE_ID,
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    EM_MODULE_LIGHTS_PWM_ID,
  #endif
  #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
    EM_MODULE_LIGHTS_WLED_EFFECTS_ID,
  #endif
  // Sensors
  #ifdef USE_MODULE_SENSORS_INTERFACE
    EM_MODULE_SENSORS_INTERFACE_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
    EM_MODULE_SENSORS_BUTTONS_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    EM_MODULE_SENSORS_SWITCHES_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    EM_MODULE_SENSORS_ANALOG_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    EM_MODULE_SENSORS_DHT_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    EM_MODULE_SENSORS_BME_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    EM_MODULE_SENSORS_DB18S20_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    EM_MODULE_SENSORS_ULTRASONIC_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    EM_MODULE_SENSORS_DOOR_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    EM_MODULE_SENSORS_MOTION_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    EM_MODULE_SENSORS_PULSECOUNTER_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_PRESENCE
    EM_MODULE_SENSORS_PRESENCE_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_BH1750
    EM_MODULE_SENSORS_BH1750_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
    EM_MODULE_SENSORS_SR04_ID,
  #endif
  // Controllers
  #ifdef USE_MODULE_CONTROLLER_BLINDS
    EM_MODULE_CONTROLLER_BLINDS_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_HEATING
    EM_MODULE_CONTROLLER_HEATING_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_RADIATORFAN
    EM_MODULE_CONTROLLER_RADIATORFAN_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_IRTRANSMITTER
    EM_MODULE_CONTROLLER_IRTRANSMITTER_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_OILFURNACE
    EM_MODULE_CONTROLLER_TANKVOLUME_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_EXERCISE_BIKE
    EM_MODULE_CONTROLLER_EXERCISEBIKE_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
    EM_MODULE_CONTROLLER_SONOFF_IFAN_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_FAN
    EM_MODULE_CONTROLLER_FAN_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_TREADMILL
    EM_MODULE_CONTROLLER_TREADMILL_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_SENSORCOLOURS
    EM_MODULE_CONTROLLER_SENSORCOLOURS_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    EM_MODULE_CONTROLLER_DOORBELL_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
    EM_MODULE_CONTROLLER_SDCARDLOGGER_ID,
  #endif
  EM_MODULE_LENGTH_ID
};

// CoreSystem (Range 0-29)
#ifdef USE_MODULE_CORE_HARDWAREPINS
  #include "2_CoreSystem/HardwarePins/mHardwarePins.h"
  #define   pCONT_pins                              static_cast<mHardwarePins*>(pCONT->pModule[EM_MODULE_CORE_HARDWAREPINS_ID])
#endif 
#ifdef USE_MODULE_CORE_SETTINGS
  #include "2_CoreSystem/Settings/mSettings.h"
  #define   pCONT_set                               static_cast<mSettings*>(pCONT->pModule[EM_MODULE_CORE_SETTINGS_ID])
#endif 
#ifdef USE_MODULE_CORE_SUPPORT
  #include "2_CoreSystem/Support/mSupport.h"
  #define   pCONT_sup                               static_cast<mSupport*>(pCONT->pModule[EM_MODULE_CORE_SUPPORT_ID])
#endif 
#ifdef USE_MODULE_CORE_LOGGING
  #include "2_CoreSystem/Logging/mLogging.h"
  #define   pCONT_sto                               static_cast<mLogging*>(pCONT->pModule[EM_MODULE_CORE_LOGGING_ID])
#endif 
#ifdef USE_MODULE_CORE_TELEMETRY
  #include "2_CoreSystem/Telemetry/mTelemetry.h"
  #define   pCONT_tel                               static_cast<mTelemetry*>(pCONT->pModule[EM_MODULE_CORE_TELEMETRY_ID])
#endif 
#ifdef USE_MODULE_CORE_TIME
  #include "2_CoreSystem/Time/mTime.h"
  #define   pCONT_time                              static_cast<mTime*>(pCONT->pModule[EM_MODULE_CORE_TIME_ID])
#endif 
#ifdef USE_MODULE_CORE_RULES
  #include "2_CoreSystem/RuleEngine/mRuleEngine.h"
  #define   pCONT_rules                             static_cast<mRuleEngine*>(pCONT->pModule[EM_MODULE_CORE_RULES_ID])
#endif
#ifdef USE_MODULE_CORE_UPDATES
  #include "2_CoreSystem/Updates/mUpdates.h"
  #define   pCONT_updates                           static_cast<mUpdates*>(pCONT->pModule[EM_MODULE_CORE_UPDATES_ID])
#endif


// Network (20-29)
#ifdef USE_MODULE_NETWORK_WIFI
  #include "3_Network/WiFi/mWiFi.h"
  #define pCONT_wif                                 static_cast<mWiFi*>(pCONT->pModule[EM_MODULE_NETWORK_WIFI_ID])
#endif 
#ifdef USE_MODULE_NETWORK_MQTT
  #include "3_Network/MQTT/mMQTT.h"
  #define pCONT_mqtt                                static_cast<mMQTT*>(pCONT->pModule[EM_MODULE_NETWORK_MQTT_ID])
#endif 
#ifdef USE_MODULE_NETWORK_WEBSERVER
  #include "3_Network/WebServer/mWebServer.h"
  #define pCONT_web                                 static_cast<mWebServer*>(pCONT->pModule[EM_MODULE_NETWORK_WEBSERVER_ID])
#endif


// Displays (30-39)
#ifdef USE_MODULE_DISPLAYS_INTERFACE
  #include "8_Displays/_Interface/mDisplaysInterface.h"
  #define pCONT_iDisp                               static_cast<mDisplaysInterface*>(pCONT->pModule[EM_MODULE_DISPLAYS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_DISPLAYS_NEXTION
  #include "8_Displays/Nextion/mNextionPanel.h"
  #define pCONT_nex                                 static_cast<mNextionPanel*>(pCONT->pModule[EM_MODULE_DISPLAYS_NEXTION_ID])
#endif
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  #include "8_Displays/OLED_SSD1606/mOLED_SSD1306.h"
  #define pCONT_oled1306                            static_cast<mOLED_SSD1306*>(pCONT->pModule[EM_MODULE_DISPLAYS_OLED_SSD1306_ID])
#endif


// Drivers (Range 40-129)
#ifdef USE_MODULE_DRIVERS_INTERFACE
  // #include "3_Network/WebServer/mWebServer.h"
  // #define pCONT_nex                                 static_cast<mNextionPanel*>(pCONT->pModule[EM_MODULE_DISPLAY_INTERFACE])
#endif
#ifdef USE_MODULE_DRIVERS_HBRIDGE
  #include "4_Drivers/Motors/HBridgeL9110/mHBridge.h"
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pModule[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  #include "4_Drivers/IRDevices/mIRtransceiver.h"
  #define pCONT_mdirt                               static_cast<mIRtransceiver*>(pCONT->pModule[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RELAY
  #include "4_Drivers/Relays/mRelays.h"
  #define pCONT_mry                                 static_cast<mRelays*>(pCONT->pModule[EM_MODULE_DRIVERS_RELAY_ID])
#endif
#ifdef USE_MODULE_DRIVERS_PWM
  #include "4_Drivers/PWM/mPWM.h"
  #define pCONT_pwm                                 static_cast<mPWM*>(pCONT->pModule[EM_MODULE_DRIVERS_PWM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433MHZ
  #include "4_Drivers/SAWRadios/mSAWRadios.h"
  #define pCONT_433                                 static_cast<mSAWMain*>(pCONT->pModule[EM_MODULE_DRIVERS_SAWRADIOS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SDCARD
  #include "4_Drivers/SD/mSDCard.h"
  #define pCONT_sdcard                              static_cast<mSDCard*>(pCONT->pModule[EM_MODULE_DRIVERS_SDCARD_ID])
#endif
#ifdef USE_MODULE_DRIVERS_GPS
  #include "4_Drivers/GPS/mGPS.h"
  #define pCONT_gps                                 static_cast<mGPS*>(pCONT->pModule[EM_MODULE_DRIVERS_GPS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SERIAL_UART
  #include "4_Drivers/SerialUART/mSerialUART.h"
  #define pCONT_uart                                static_cast<mSerialUART*>(pCONT->pModule[EM_MODULE_DRIVERS_SERIAL_UART_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  #include "4_Drivers/ShellyDimmer/mShellyDimmer.h"
  #define pCONT_shelly                              static_cast<mShellyDimmer*>(pCONT->pModule[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
  #include "4_Drivers/CAM_OV2640/mCamera.h"
  #define pCONT_mdhbridge                           static_cast<mCamera*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_ID])
#endif
#ifdef USE_MODULE_DRIVERS_LEDS
#include "4_Drivers/LEDs/mLEDs.h"
  #define pCONT_status_leds                         static_cast<mHBridge*>(pCONT->pModule[EM_MODULE_DRIVERS_STATUS_LEDS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_FILESYSTEM
  #include "4_Drivers/FileSystem/mFileSystem.h"
  #define pCONT_mfile                               static_cast<mFileSystem*>(pCONT->pModule[EM_MODULE_DRIVERS_FILESYSTEM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_BUZZER
  #include "4_Drivers/Buzzer/mBuzzer.h"
  #define pCONT_buzzer                               static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE_DRIVERS_BUZZER_ID])
#endif

// Energy (Range 130-139)
#ifdef USE_MODULE_ENERGY_INTERFACE
  #include "7_Energy/_Interface/mEnergy.h"
  #define pCONT_iEnergy                           static_cast<mEnergyInterface*>(pCONT->pModule[EM_MODULE_ENERGY_INTERFACE_ID])
#endif
#ifdef USE_MODULE_ENERGY_PZEM004T_V3
  #include "7_Energy/Pzem004T_v3/mPZEM004T.h"
  #define pCONT_pzem                            static_cast<mEnergyPZEM004T*>(pCONT->pModule[EM_MODULE_ENERGY_PZEM004T_V3_ID])
#endif
#ifdef USE_MODULE_ENERGY_ADE7953
  #include "7_Energy/ADE7953/mADE7953.h"
  #define pCONT_ade7953                         static_cast<mEnergyADE7953*>(pCONT->pModule[EM_MODULE_ENERGY_ADE7953_ID])
#endif
#ifdef USE_MODULE_ENERGY_INA219
  #include "7_Energy/INA219/mEnergyINA219.h"
  #define pCONT_mina219                         static_cast<mEnergyINA219*>(pCONT->pModule[EM_MODULE_ENERGY_INA219_ID])
#endif

// Lights (Range 140-169)
#ifdef USE_MODULE_LIGHTS_INTERFACE
  #include "6_Lights/_Interface/mInterfaceLight.h"
  #define pCONT_iLight                          static_cast<mInterfaceLight*>(pCONT->pModule[EM_MODULE_LIGHTS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_LIGHTS_ANIMATOR
  #include "6_Lights/Animator/mAnimatorLight.h"
  #define pCONT_lAni                            static_cast<mAnimatorLight*>(pCONT->pModule[EM_MODULE_LIGHTS_ANIMATOR_ID])
#endif
#ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  #include "6_Lights/Hardware/Addressable/mAddressableLight.h"
  #define pCONT_ladd                            static_cast <mAddressableLight*>(pCONT->pModule[EM_MODULE_LIGHTS_ADDRESSABLE_ID])
#endif
#ifdef USE_MODULE_LIGHTS_PWM
  #include "6_Lights/Hardware/PWM/mPWMLight.h"
  #define pCONT_lPWM                            static_cast<mPWMLight*>(pCONT->pModule[EM_MODULE_LIGHTS_PWM_ID])
#endif
#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
  #include "6_Lights/WLEDEffects/mWLEDEffects.h"
  #define pCONT_lwled                           static_cast<mWLEDEffects*>(pCONT->pModule[EM_MODULE_LIGHTS_WLED_EFFECTS_ID])
#endif

// Sensors (Range 120-169)
#ifdef USE_MODULE_SENSORS_INTERFACE
  #include "5_Sensors/_Interface/mSensorsInterface.h"
  #define pCONT_iSensors                         static_cast<mSensorsInterface*>(pCONT->pModule[EM_MODULE_SENSORS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_SENSORS_BUTTONS
  #include "5_Sensors/Buttons/mButtons.h"
  #define pCONT_sbutton                         static_cast<mButtons*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_SENSORS_SWITCHES
  #include "5_Sensors/Switches/mSwitches.h"
  #define pCONT_swt                            static_cast<mSwitches*>(pCONT->pModule[EM_MODULE_SENSORS_SWITCHES_ID])
#endif
#ifdef USE_MODULE_SENSORS_ANALOG
  #include "5_Sensors/Analog/mSensorsAnalog.h"
  #define pCONT_msanalog                        static_cast<mSensorsAnalog*>(pCONT->pModule[EM_MODULE_SENSORS_ANALOG_ID])
#endif
#ifdef USE_MODULE_SENSORS_DHT
  #include "5_Sensors/DHT/mSensorsDHT.h"
  #define pCONT_dht                             static_cast<mSensorsDHT*>(pCONT->pModule[EM_MODULE_SENSORS_DHT_ID])
#endif
#ifdef USE_MODULE_SENSORS_BME
  #include "5_Sensors/BME/mSensorsBME.h"
  #define pCONT_bme                             static_cast<mSensorsBME*>(pCONT->pModule[EM_MODULE_SENSORS_BME_ID])
#endif
#ifdef USE_MODULE_SENSORS_DS18B20
  #include "5_Sensors/DB18x20/mSensorsDB18.h"
  #define pCONT_msdb18                          static_cast<mSensorsDB18*>(pCONT->pModule[EM_MODULE_SENSORS_DB18S20_ID])
#endif
#ifdef USE_MODULE_SENSORS_ULTRASONICS
  #include "5_Sensors/UltraSonic/mUltraSonicSensor.h"
  #define pCONT_ult                             static_cast<mUltraSonicSensor*>(pCONT->pModule[EM_MODULE_SENSORS_ULTRASONIC_ID])
#endif
#ifdef USE_MODULE_SENSORS_DOOR
  #include "5_Sensors/Door/mDoorSensor.h"
  #define pCONT_sdoor                           static_cast<mDoorSensor*>(pCONT->pModule[EM_MODULE_SENSORS_DOOR_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOTION
  #include "5_Sensors/Motion/mMotionSensor.h"
  #define pCONT_smot                            static_cast<mMotionSensor*>(pCONT->pModule[EM_MODULE_SENSORS_MOTION_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOISTURE
  #include "5_Sensors/Moisture/mMoistureSensor.h"
  #define pCONT_srmoisture                      static_cast<mMoistureSensor*>(pCONT->pModule[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID])
#endif
#ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  #include "5_Sensors/PulseCounter/mPulseCounter.h"
  #define pCONT_spulse                          static_cast<mPulseCounter*>(pCONT->pModule[EM_MODULE_SENSORS_PULSECOUNTER_ID])
#endif
#ifdef USE_MODULE_SENSORS_PRESENCE
  #include "5_Sensors/Presence/mPresence.h"
  #define pCONT_presence                          static_cast<mPresence*>(pCONT->pModule[EM_MODULE_SENSORS_PRESENCE_ID])
#endif
#ifdef USE_MODULE_SENSORS_BH1750
  #include "5_Sensors/BH1750Light/mBH1750.h"
  #define pCONT_presence                          static_cast<mBH1750*>(pCONT->pModule[EM_MODULE_SENSORS_BH1750_ID])
#endif
#ifdef USE_MODULE_SENSORS_SR04
  #include "5_Sensors/SR04/mSR04.h"
  #define pCONT_sr04                              static_cast<mSR04*>(pCONT->pModule[EM_MODULE_SENSORS_SR04_ID])
#endif

// Specefic Bespoke Modules (Range 170-189) to be named "CONTROLLER"
#ifdef USE_MODULE_CONTROLLER_BLINDS
  #include "9_Controller/Blinds/mBlinds.h"
  #define pCONT_sbut                            static_cast<mBlinds*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_HEATING
  #include "9_Controller/Heating/mHeating.h"
  #define pCONT_heating                         static_cast<mHeating*>(pCONT->pModule[EM_MODULE_CONTROLLER_HEATING_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_RADIATORFAN
  #include "9_Controller/RadiatorFan/mRadiatorFan.h"
  #define pCONT_sbut                            static_cast<mRadiatorFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_RADIATORFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_IRTRANSMITTER
  #include "4_Drivers/IRDevices/mIRtransceiver.h"
  #define pCONT_ir                              static_cast<mButtons*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_OILFURNACE
  #include "9_Controller/OilFurnace/mOilFurnace.h"
  #define pCONT_tankvolume                      static_cast<mOilFurnace*>(pCONT->pModule[EM_MODULE_CONTROLLER_TANKVOLUME_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_EXERCISE_BIKE
  #include "9_Controller/ExerciseBike/mExerciseBike.h"
  #define pCONT_bike                            static_cast<mExerciseBike*>(pCONT->pModule[EM_MODULE_CONTROLLER_EXERCISEBIKE_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
  #include "9_Controller/Sonoff_iFan/mSonoffIFan.h"
  #define pCONT_ifan                            static_cast<mSonoffIFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_SONOFF_IFAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_FAN
  #include "9_Controller/Fan/mFan.h"
  #define pCONT_mfan                            static_cast<mFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_FAN_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_TREADMILL
  #include "9_Controller/Treadmill/mTreadmill.h"
  #define pCONT_sbut                            static_cast<mTreadmill*>(pCONT->pModule[EM_MODULE_CONTROLLER_TREADMILL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SENSORCOLOURS
  #include "9_Controller/SensorColours/mSensorColours.h"
  #define pCONT_msenscol                        static_cast<mSensorColours*>(pCONT->pModule[EM_MODULE_CONTROLLER_SENSORCOLOURS_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_DOORCHIME
  #include "9_Controller/DoorBell/mDoorBell.h"
  #define pCONT_doorbell                        static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_DOORBELL_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
  #include "9_Controller/SDCardLogger/mSDCardLogger.h"
  #define pCONT_sdcardlogger                    static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_SDCARDLOGGER_ID])
#endif

#include  "1_TaskerManager/mTaskerInterface.h"

class mTaskerManager{

  friend class mTaskerInterface;

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

    mTaskerInterface* pModule[EM_MODULE_LENGTH_ID];




    int16_t GetModuleIndexbyFriendlyName(const char* c);
    int16_t GetModuleUniqueIDbyFriendlyName(const char* c);


    uint16_t GetModuleUniqueIDbyVectorIndex(uint8_t id);

    #if defined(ENABLE_ADVANCED_DEBUGGING) || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
      char buffer_taskname[50];
    #endif

    uint8_t Instance_Init();
    uint8_t CheckPointersPass();
    
    int8_t Tasker_Interface(uint8_t function, uint8_t target_tasker = 0, bool flags_is_executing_rule = false);

    // int8_t Tasker_ParseCommands(uint8_t function, char* buffer = nullptr, uint16_t buflen = 0);
    
    
    uint16_t GetClassSizeByID(uint8_t class_id);

    PGM_P GetModuleName(uint8_t id);
    PGM_P GetModuleFriendlyName(uint8_t module_id);
    uint16_t GetClassCount();

    bool ValidTaskID(uint8_t module_id);

    // enum 

    // #define CLASS_ID_MAX 255
    // struct CLASS_ID{
    //   uint8_t list[CLASS_ID_MAX];
    //   uint8_t module_type[CLASS_ID_MAX];
    //   // #ifdef DEBUG_EXECUTION_TIME
    //   // uint16_t execution_time_average_ms[CLASS_ID_MAX];
    //   // uint16_t execution_time_max_ms[CLASS_ID_MAX];
    //   // #endif
    //   uint8_t count = 0;
    // }module_settings; 

    uint16_t last_function = 255; // 0 will be first

    uint8_t fExitTaskerWithCompletion = false;

    uint8_t switch_index = 0;
    
    // JsonParserObject obj = 0;
    // JsonParser* parser = nullptr;//(parsing_buffer);
      
    int16_t GetModuleIDbyFriendlyName(const char* c);

};


#endif
