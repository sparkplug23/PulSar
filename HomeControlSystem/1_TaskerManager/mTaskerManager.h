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
#include "2_CoreSystem/mHardwareTemplates.h"                // Hardware configuration
#include "2_CoreSystem/Support/BufferWriter.h"

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

#define NO_GLOBAL_MDNS
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
  #include <ArduinoOTA.h>
  #include <WiFiUdp.h>
  #define WDT_RESET() ESP.wdtFeed()
  #include <ESP8266WiFi.h>
  #ifdef USE_MODULE_CORE_WEBSERVER
    #include <ESPAsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  #endif //USE_MODULE_CORE_WEBSERVER
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
  #ifdef USE_MODULE_CORE_WEBSERVER
    EM_MODULE_NETWORK_WEBSERVER_ID,
  #endif
  // Displays
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
    EM_MODULE_DISPLAYS_NEXTION_ID,
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    EM_MODULE_DISPLAYS_NEXTION_ID,
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
  // Energy
  #ifdef USE_MODULE_ENERGY_INTERFACE
    EM_MODULE_ENERGY_INTERFACE_ID,
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
  #ifdef USE_MODULE_SENSORS_BUTTONS
    EM_MODULE_SENSORS_BUTTONS_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    EM_MODULE_SENSORS_SWITCHES_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    EM_MODULE_SENSORS_ANALOG_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    EM_MODULE_SENSORS_PZEM004T_MODBUS_ID,
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
  #ifdef USE_MODULE_SENSORS_INA219
    EM_MODULE_SENSORS_INA219_ID,
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
    PM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID,
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    PM_MODULE_SENSORS_PULSECOUNTER_ID,
  #endif
  // Controllers
  #ifdef USE_MODULE_CUSTOM_BLINDS
    EM_MODULE_CUSTOM_BLINDS_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_HEATING
    EM_MODULE_CUSTOM_HEATING_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    EM_MODULE_CUSTOM_RADIATORFAN_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_IRTRANSMITTER
    EM_MODULE_CUSTOM_IRTRANSMITTER_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    EM_MODULE_CUSTOM_OILFURNACE_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    EM_MODULE_CUSTOM_EXERCISEBIKE_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    EM_MODULE_CUSTOM_SONOFF_IFAN_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_FAN
    EM_MODULE_CUSTOM_FAN_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_TREADMILL
    EM_MODULE_CUSTOM_TREADMILL_ID,
  #endif
  #ifdef USE_MODULE_CUSTOM_SENSORCOLOURS
    EM_MODULE_CUSTOM_SENSORCOLOURS_ID,
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    EM_MODULE_CONTROLLER_DOORBELL_ID,
  #endif
  EM_MODULE_LENGTH_ID
};

// CoreSystem (Range 0-29)
#ifdef USE_MODULE_CORE_HARDWAREPINS
  #include "2_CoreSystem/HardwarePins/mHardwarePins.h"
  // class                                             mHardwarePins;
#endif 
#ifdef USE_MODULE_CORE_SETTINGS
  #include "2_CoreSystem/Settings/mSettings.h"
  // class                                             mSettings;
  #define   pCONT_set                               static_cast<mSettings*>(pCONT->pInterface[EM_MODULE_CORE_SETTINGS_ID])
#endif 
#ifdef USE_MODULE_CORE_SUPPORT
  #include "2_CoreSystem/Support/mSupport.h"
  // class                                             mSupport;
  #define   pCONT_sup                               static_cast<mSupport*>(pCONT->pInterface[EM_MODULE_CORE_SUPPORT_ID])
#endif 
#ifdef USE_MODULE_CORE_LOGGING
  #include "2_CoreSystem/Logging/mLogging.h"
  // class                                             mLogging;/
  #define   pCONT_sto                               static_cast<mLogging*>(pCONT->pInterface[EM_MODULE_CORE_LOGGING_ID])
#endif 
#ifdef USE_MODULE_CORE_TELEMETRY
  #include "2_CoreSystem/Telemetry/mTelemetry.h"
  class                                             mTelemetry;
  #define   pCONT_tel                               static_cast<mTelemetry*>(pCONT->pInterface[EM_MODULE_CORE_TELEMETRY_ID])
#endif 
#ifdef USE_MODULE_CORE_TIME
  #include "2_CoreSystem/Time/mTime.h"
  class                                             mTime;
  #define   pCONT_time                              static_cast<mTime*>(pCONT->pInterface[EM_MODULE_CORE_TIME_ID])
#endif 
#ifdef USE_MODULE_CORE_RULES
  #include "2_CoreSystem/RuleEngine/mRuleEngine.h"
  class                                             mRuleEngine;
  #define            pCONT_rules                    static_cast<mRuleEngine*>(pCONT->pInterface[EM_MODULE_CORE_RULES_ID])
#endif
#ifdef USE_MODULE_CORE_UPDATES
  #include "2_CoreSystem/Updates/mUpdates.h"
  class                                             mUpdates;
  #define            pCONT_updates                  static_cast<mUpdates*>(pCONT->pInterface[EM_MODULE_CORE_UPDATES_ID])
#endif


// Network (20-29)
#ifdef USE_MODULE_NETWORK_WIFI
  #include "3_Network/WiFi/mWiFi.h"
  class                                             mWiFi;
  #define pCONT_wif                                 static_cast<mWiFi*>(pCONT->pInterface[EM_MODULE_NETWORK_WIFI_ID])
#endif 
#ifdef USE_MODULE_NETWORK_MQTT
  #include "3_Network/MQTT/mMQTT.h"
  class                                             mMQTT;
  #define pCONT_mqtt                                static_cast<mMQTT*>(pCONT->pInterface[EM_MODULE_NETWORK_MQTT_ID])
#endif 
#ifdef USE_MODULE_CORE_WEBSERVER
  #include "3_Network/WebServer/mWebServer.h"
  class                                             mWebServer;
  #define pCONT_web                                 static_cast<mWebServer*>(pCONT->pInterface[EM_MODULE_NETWORK_WEBSERVER_ID])
#endif


// Displays (30-39)
#ifdef USE_MODULE_DISPLAYS_INTERFACE
  // #include "3_Network/WebServer/mWebServer.h"
  // class                                             mDisplayInterface;
  // #define pCONT_nex                                 static_cast<mNextionPanel*>(pCONT->pInterface[EM_MODULE_DISPLAY_INTERFACE])
#endif
#ifdef USE_MODULE_DISPLAYS_NEXTION
  #include "8_Displays/Nextion/mNextionPanel.h"
  class                                             mNextionPanel;
  #define pCONT_nex                                 static_cast<mNextionPanel*>(pCONT->pInterface[EM_MODULE_DISPLAYS_NEXTION_ID])
#endif


// Drivers (Range 40-129)
#ifdef USE_MODULE_DRIVERS_INTERFACE
  // #include "3_Network/WebServer/mWebServer.h"
  // class                                             mDisplayInterface;
  // #define pCONT_nex                                 static_cast<mNextionPanel*>(pCONT->pInterface[EM_MODULE_DISPLAY_INTERFACE])
#endif
#ifdef USE_MODULE_DRIVERS_HBRIDGE
  #include "4_Drivers/Motors/HBridgeL9110/mHBridge.h"
  class                                             mHBridge;
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  #include "4_Drivers/IRDevices/mIRtransceiver.h"
  class                                             mIRtransceiver;
  #define pCONT_mdirt                               static_cast<mIRtransceiver*>(pCONT->pInterface[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RELAY
  #include "4_Drivers/Relays/mRelays.h"
  class                                             mRelays;
  #define pCONT_mry                                 static_cast<mRelays*>(pCONT->pInterface[EM_MODULE_DRIVERS_RELAY_ID])
#endif
#ifdef USE_MODULE_DRIVERS_PWM
  #include "4_Drivers/PWM/mPWM.h"
  class                                             mPWM;
  #define pCONT_nex                                 static_cast<mPWM*>(pCONT->pInterface[EM_MODULE_DRIVERS_PWM_ID])
#endif
#ifdef USE_MODULE_DRIVERS_RF433MHZ
  #include "4_Drivers/SAWRadios/mSAWRadios.h"
  class                                             mSAWMain;
  #define pCONT_433                                 static_cast<mSAWMain*>(pCONT->pInterface[EM_MODULE_DRIVERS_SAWRADIOS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SDCARD
  #include "4_Drivers/SD/mSDCard.h"
  class                                             mSDCard;
  #define pCONT_mdhbridge                           static_cast<mSDCard*>(pCONT->pInterface[EM_MODULE_DRIVERS_SDCARD_ID])
#endif
#ifdef USE_MODULE_DRIVERS_GPS
  #include "4_Drivers/GPS/mGPS.h"
  class                                             mGPS;
  #define pCONT_gps                                 static_cast<mGPS*>(pCONT->pInterface[EM_MODULE_DRIVERS_GPS_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SERIAL_UART
  #include "4_Drivers/SerialUART/mSerialUART.h"
  class mSerialUART;
  #define            D_MODULE_DRIVERS_SERIAL_UART_ID       49
  #define            pCONT_serial                        pCONT->mserial
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_SERIAL_UART_CTR)           D_MODULE_DRIVERS_SERIAL_UART_CTR;
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR)  D_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR;
  class                                             mHBridge;
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
  #include "4_Drivers/ShellyDimmer/mShellyDimmer.h"
  class mShellyDimmer;
  #define            D_MODULE_DRIVERS_SHELLY_DIMMER_ID        50
  #define            pCONT_shelly_dimmer                      pCONT->mshelly_dimmer 
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_SHELLY_DIMMER_CTR)         D_MODULE_DRIVERS_SHELLY_DIMMER_CTR;
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR)  D_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR;
  class                                             mHBridge;
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif
#ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
  #include "4_Drivers/CAM_OV2640/mCamera.h"
  class mCameraOV2640;
  #define            D_MODULE_DRIVERS_CAMERA_OV2640_ID       51
  #define            pCONT_cam                        pCONT->mcamera_ov2640
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_CAMERA_OV2640_CTR)           D_MODULE_DRIVERS_CAMERA_OV2640_CTR;
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_CAMERA_OV2640_FRIENDLY_CTR)  D_MODULE_DRIVERS_CAMERA_OV2640_FRIENDLY_CTR;
  class                                             mHBridge;
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif

#ifdef USE_MODULE_DRIVERS_LEDS
#include "4_Drivers/LEDs/mLEDs.h"
class mStatusLEDs;
#define            D_MODULE_DRIVERS_STATUS_LEDS_ID 52
#define            pCONT_status_leds                              pCONT->mstatus_leds
DEFINE_PGM_CTR(PM_MODULE_DRIVERS_STATUS_LEDS_CTR)                     D_MODULE_DRIVERS_STATUS_LEDS_CTR;
DEFINE_PGM_CTR(PM_MODULE_DRIVERS_STATUS_LEDS_FRIENDLY_CTR)            D_MODULE_DRIVERS_STATUS_LEDS_FRIENDLY_CTR;
  class                                             mHBridge;
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif //USE_MODULE_DRIVERS_LEDS


#ifdef USE_MODULE_DRIVERS_FILESYSTEM
  #include "4_Drivers/FileSystem/mFileSystem.h"
  class                                         mFileSystem;
  #define pCONT_mfile                           static_cast<mFileSystem*>(pCONT->pInterface[EM_MODULE_DRIVERS_FILESYSTEM_ID])
#endif //USE_MODULE_DRIVERS_FILESYSTEM




// Energy (Range 130-139)
#ifdef USE_MODULE_ENERGY_INTERFACE
  #include "7_Energy/_Interface/mEnergy.h"
  class mEnergy;
  #define            D_MODULE_DRIVERS_ENERGY_ID                   130
  #define            pCONT_iEnergy                                pCONT->mdenergy  
  DEFINE_PGM_CTR(PM_INTERFACE_ENERGY_MODULE_CTR)              "mEnergy";
  DEFINE_PGM_CTR(PM_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR)     "energy";
  class                                             mHBridge;
  #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID])
#endif

// Lights (Range 140-169)
#ifdef USE_MODULE_LIGHTS_INTERFACE
  #include "6_Lights/_Interface/mInterfaceLight.h"
  class                                         mInterfaceLight;
  #define pCONT_iLight                          static_cast<mInterfaceLight*>(pCONT->pInterface[EM_MODULE_LIGHTS_INTERFACE_ID])
#endif
#ifdef USE_MODULE_LIGHTS_ANIMATOR
  #include "6_Lights/Animator/mAnimatorLight.h"
  class                                         mAnimatorLight;
  #define pCONT_lAni                            static_cast<mAnimatorLight*>(pCONT->pInterface[EM_MODULE_LIGHTS_ANIMATOR_ID])
#endif
#ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  #include "6_Lights/Hardware/Addressable/mAddressableLight.h"
  class                                         mAddressableLight;
  #define pCONT_ladd                            static_cast <mAddressableLight*>(pCONT->pInterface[EM_MODULE_LIGHTS_ADDRESSABLE_ID])
#endif
#ifdef USE_MODULE_LIGHTS_PWM
  #include "6_Lights/Hardware/PWM/mPWMLight.h"
  class                                         mPWMLight;
  #define pCONT_lPWM                            static_cast<mPWMLight*>(pCONT->pInterface[EM_MODULE_LIGHTS_PWM_ID])
#endif
#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
  #include "6_Lights/WLEDEffects/mWLEDEffects.h"
  class                                         mWLEDEffects;
  #define pCONT_lwled                           static_cast<mWLEDEffects*>(pCONT->pInterface[EM_MODULE_LIGHTS_WLED_EFFECTS_ID])
#endif

// Sensors (Range 120-169)
#ifdef USE_MODULE_SENSORS_BUTTONS
  #include "5_Sensors/Buttons/mButtons.h"
  class                                         mButtons;
  #define pCONT_sbutton                         static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_SENSORS_SWITCHES
  #include "5_Sensors/Switches/mSwitches.h"
  class                                         mSwitches;
  #define pCONT_swt                            static_cast<mSwitches*>(pCONT->pInterface[EM_MODULE_SENSORS_SWITCHES_ID])
#endif
#ifdef USE_MODULE_SENSORS_ANALOG
  #include "5_Sensors/Analog/mSensorsAnalog.h"
  class                                         mSensorsAnalog;
  #define pCONT_msanalog                        static_cast<mSensorsAnalog*>(pCONT->pInterface[EM_MODULE_SENSORS_ANALOG_ID])
#endif
#ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
  #include "7_Energy/PowerMeter/mPzem_AC.h"
  class                                         mPzem_AC;
  #define pCONT_pzem                            static_cast<mPzem_AC*>(pCONT->pInterface[EM_MODULE_SENSORS_PZEM004T_MODBUS_ID])
#endif
#ifdef USE_MODULE_SENSORS_DHT
  #include "5_Sensors/DHT/mSensorsDHT.h"
  class                                         mSensorsDHT;
  #define pCONT_dht                             static_cast<mSensorsDHT*>(pCONT->pInterface[EM_MODULE_SENSORS_DHT_ID])
#endif
#ifdef USE_MODULE_SENSORS_BME
  #include "5_Sensors/BME/mSensorsBME.h"
  class                                         mSensorsBME;
  #define pCONT_bme                             static_cast<mSensorsBME*>(pCONT->pInterface[EM_MODULE_SENSORS_BME_ID])
#endif
#ifdef USE_MODULE_SENSORS_DS18B20
  #include "5_Sensors/DB18/mSensorsDB18.h"
  class                                         mSensorsDB18;
  #define pCONT_msdb18                          static_cast<mSensorsDB18*>(pCONT->pInterface[EM_MODULE_SENSORS_DB18S20_ID])
#endif
#ifdef USE_MODULE_SENSORS_INA219
  #include "5_Sensors/INA219/mSensorsINA219.h"
  class                                         mSensorsINA219;
  #define pCONT_mina219                         static_cast<mSensorsINA219*>(pCONT->pInterface[EM_MODULE_SENSORS_INA219_ID])
#endif
#ifdef USE_MODULE_SENSORS_ULTRASONICS
  #include "5_Sensors/UltraSonic/mUltraSonicSensor.h"
  class                                         mUltraSonicSensor;
  #define pCONT_ult                             static_cast<mUltraSonicSensor*>(pCONT->pInterface[EM_MODULE_SENSORS_ULTRASONIC_ID])
#endif
#ifdef USE_MODULE_SENSORS_DOOR
  #include "5_Sensors/Door/mDoorSensor.h"
  class                                         mDoorSensor;
  #define pCONT_sdoor                           static_cast<mDoorSensor*>(pCONT->pInterface[EM_MODULE_SENSORS_DOOR_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOTION
  #include "5_Sensors/PIR/mMotionSensor.h"
  class                                         mMotionSensor;
  #define pCONT_smot                            static_cast<mMotionSensor*>(pCONT->pInterface[EM_MODULE_SENSORS_MOTION_ID])
#endif
#ifdef USE_MODULE_SENSORS_MOISTURE
  #include "5_Sensors/Moisture/mMoistureSensor.h"
  class                                         mMoistureSensor;
  #define pCONT_srmoisture                      static_cast<mMoistureSensor*>(pCONT->pInterface[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID])
#endif
#ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  #include "5_Sensors/PulseCounter/mPulseCounter.h"
  class                                         mPulseCounter;
  #define pCONT_spulse                          static_cast<mPulseCounter*>(pCONT->pInterface[EM_MODULE_SENSORS_PULSECOUNTER_ID])
#endif


// Specefic Bespoke Modules (Range 170-189) to be named "CONTROLLER"
#ifdef USE_MODULE_CUSTOM_BLINDS
  #include "9_Controller/Blinds/mBlinds.h"
  class mBlinds;
  #define D_MODULE_CUSTOM_BLINDS_ID 170
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_BLINDS_CTR)              "mBlinds";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_BLINDS_FRIENDLY_CTR)              "blinds";
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_HEATING
  #include "9_Controller/Heating/mHeating.h"
  class mHeating;
  #define D_MODULE_CUSTOM_HEATING_ID 171
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_HEATING_CTR)              "mHeating";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_HEATING_FRIENDLY_CTR)              "heating";
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_RADIATORFAN
  #include "9_Controller/RadiatorFan/mRadiatorFan.h"
  class mRadiatorFan;
  #define D_MODULE_CUSTOM_RADIATORFAN_ID 175
  DEFINE_PGM_CTR(MODULE_CUSTOM_RADIATORFAN_CTR)              "mRadiatorFan";
  DEFINE_PGM_CTR(MODULE_CUSTOM_RADIATORFAN_FRIENDLY_CTR)              "radiatorfan";
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_IRTRANSMITTER
  // #ifdef ESP32
  //   #include "IRDevices/IRLib32/IRremote.h"
  // #endif
  // #ifdef ESP8266
  //   #include "4_Drivers/IRDevices/IRLib8266/IRremoteESP8266.h"
  //   #include "4_Drivers/IRDevices/IRLib8266/IRsend.h"
  // // #endifclass mOilFurnace;
  #define D_MODULE_CUSTOM_IRTRANSMITTER_ID 177
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_IRTRANSMITTER_CTR)              "infrared";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_IRTRANSMITTER_FRIENDLY_CTR)              "infrared";
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_OILFURNACE
  #include "9_Controller/OilFurnace/mOilFurnace.h"
  class mOilFurnace;
  #define D_MODULE_CUSTOM_OILFURNACE_ID 177
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_OILFURNACE_CTR)              "mOilFurnace";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_OILFURNACE_FRIENDLY_CTR)              "oilfurnace";
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
  #include "9_Controller/ExerciseBike/mExerciseBike.h"
  class mExerciseBike;
  #define D_MODULE_CUSTOM_EXERCISEBIKE_ID 178
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_EXERCISEBIKE_CTR)              "mExerciseBike";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_EXERCISEBIKE_FRIENDLY_CTR)              "exercisebike";
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
  #include "9_Controller/Sonoff_iFan/mSonoffIFan.h"
  class mSonoffIFan;
  #define D_MODULE_CUSTOM_SONOFF_IFAN_ID                             179
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_IFAN_CTR)               "mSonoffIFan";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_IFAN_FRIENDLY_CTR)      "fan";
  #define pCONT_ifan                                           pCONT->mifan
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_FAN
  #include "9_Controller/Fan/mFan.h"
  class mFan;
  #define D_MODULE_CUSTOM_FAN_ID                        180
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_FAN_CTR)               D_MODULE_CUSTOM_FAN_CTR;
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_FAN_FRIENDLY_CTR)      D_MODULE_CUSTOM_FAN_FRIENDLY_CTR;
  #define pCONT_mfan                                     pCONT->mfan
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_TREADMILL
  #include "9_Controller/Treadmill/mTreadmill.h"
  class mTreadmill;
  #define D_MODULE_CUSTOM_TREADMILL_ID                        181
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_TREADMILL_CTR)               D_MODULE_CUSTOM_TREADMILL_CTR;
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_TREADMILL_FRIENDLY_CTR)      D_MODULE_CUSTOM_TREADMILL_FRIENDLY_CTR;
  #define pCONT_mtreadmill                                    pCONT_timereadmill
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CUSTOM_SENSORCOLOURS
  #include "9_Controller/SensorColours/mSensorColours.h"
  class mSensorColours;
  #define D_MODULE_CUSTOM_SENSORCOLOURS_ID                        182
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_SENSORCOLOURS_CTR)               D_MODULE_CUSTOM_SENSORCOLOURS_CTR;
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_SENSORCOLOURS_FRIENDLY_CTR)      D_MODULE_CUSTOM_SENSORCOLOURS_FRIENDLY_CTR;
  #define pCONT_msenscol                                     pCONT->msenscol
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif
#ifdef USE_MODULE_CONTROLLER_DOORCHIME
  #include "9_Controller/DoorBell/mDoorBell.h"
  class mDoorBell;
  #define D_MODULE_CONTROLLER_DOORBELL_ID 123
  DEFINE_PGM_CTR(PM_MODULE_CONTROLLER_DOORBELL_CTR)              "mDoorBell";
  DEFINE_PGM_CTR(PM_MODULE_CONTROLLER_DOORBELL_FRIENDLY_CTR)              "doorbell";
  #define pCONT_mdb                                           pCONT->mdb
  class                                         mButtons;
  #define pCONT_sbut                            static_cast<mButtons*>(pCONT->pInterface[EM_MODULE_SENSORS_BUTTONS_ID])
#endif

// #define pModule(X) pInterface[GetVectorIndexbyModuleID(X)]
#define pModule(X) pInterface[X]



#include  "1_TaskerManager/mTaskerInterface.h"

class mTaskerManager{

  friend class mTaskerInterface;

  private:
    /* Prevent others from being created */
    mTaskerManager(mTaskerManager const& other) = delete;
    mTaskerManager(mTaskerManager&& other) = delete;
    /* Private constructor to prevent instancing. */
    mTaskerManager(){};

    // mTaskerInterface* pInterface[50];


    /* Here will be the instance stored. */
    static mTaskerManager* instance;
  public:
    // External function to get instance
    static mTaskerManager* GetInstance();

    int16_t GetVectorIndexbyModuleID(uint8_t id_search);

    // std::vector<mTaskerInterface*> pInterface;
    // std::vector<uint8_t> mTasksIDs;

    mTaskerInterface* pInterface[EM_MODULE_LENGTH_ID];

    uint8_t Instance_Init();
    uint8_t CheckPointersPass();
    
    int8_t Tasker_Interface(uint8_t function, uint8_t target_tasker = 0, bool flags_is_executing_rule = false);

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
    
    int16_t GetModuleIDbyFriendlyName(const char* c);

};


#endif
