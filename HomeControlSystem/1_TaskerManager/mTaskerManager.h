#ifndef mInterfaceController_H2
#define mInterfaceController_H2 1.1

// ORDER NEEDS FIXED FOR WHAT TO INCLUDE AND WHEN


#include <Arduino.h>
#include <stdint.h>
#include <string.h>

#ifdef ENABLE_DEVFEATURE_JSONPARSER
#include "JsonParser.h"
#endif // ENABLE_DEVFEATURE_JSONPARSER

#include "2_CoreSystem/esp32_compat.h"
#include "2_CoreSystem/mGlobalMacros.h"
#include "0_ConfigUser/mUserConfig.h"           //DEFAULTS
#include "0_ConfigUser/mUserConfigSecret.h"            // Override previos declarations
//ifdef use custom
#include "0_ConfigUser/mFirmwareCustom.h"     //overrides defaults
//endif
#include "2_CoreSystem/mFirmwareDefaults.h"                    // Configuration overrides for all previous includes
#include "2_CoreSystem/Languages/mLanguageDefault.h"                           // Language support configured by .h
#include "2_CoreSystem/mHardwareTemplates.h"                // Hardware configuration

//https://www.fluentcpp.com/2018/03/06/issues-singletons-signals/

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
#include "0_ConfigUser/mFirmwareCustom.h"
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


// New method?
// typedef union {
//   uint8_t data;
//   struct {
//     uint8_t PeriodicEnabled : 1; 
//     uint8_t SendNow         : 1;
//     uint8_t FrequencyRedunctionLevel  : 2;     // 0= no redunction, maintain level, 1 = reduce after 1 minute, 1= 10 minutes, 2= 60 minutes
//     uint8_t reserved : 4; 
//   };
// }Interface_Handler_Flags;

// template <typename Class>
// struct interface_handler {
//   Interface_Handler_Flags flags;
//   const char*   pm_name_ctr;
//   const char*   pm_friendly_name_ctr;
//   uint16_t      module_id = 0;
//   uint8_t       module_subtype_id = 0; //lighting, energy, sensor, custom, core
//   uint8_t       (Class::*Tasker)(uint8_t function); // member-function to sender with one args
// };



// uint8_t (* const list[])(uint8_t) = {}



// CoreSystem (Range 0-29)
#include "2_CoreSystem/HardwarePins/mHardwarePins.h"
class mHardwarePins;
#define            D_MODULE_CORE_HARDWAREPINS_ID               0 //index wont be needed?
#define            D_MODULE_CORE_HARDWAREPINS_SUBTYPE_ID       MODULE_SUBTYPE_CORE_ID
#define            pCONT_pins                                  pCONT->mod
DEFINE_PGM_CTR(PM_MODULE_CORE_HARDWAREPINS_CTR)            "mHardwarePins";
DEFINE_PGM_CTR(PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR)   "hardwarepins";

#include "2_CoreSystem/Settings/mSettings.h"
class mSettings;
#define            D_MODULE_CORE_SETTINGS_ID                   1
#define            pCONT_set                               pCONT->mset
DEFINE_PGM_CTR(PM_MODULE_CORE_SETTINGS_CTR)                "mSettings";
DEFINE_PGM_CTR(PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR)       "settings";

#include "2_CoreSystem/Support/mSupport.h"
class mSupport;
#define            D_MODULE_CORE_SUPPORT_ID                     2
#define            pCONT_sup                                pCONT->msup
DEFINE_PGM_CTR(PM_MODULE_CORE_SUPPORT_CTR)                  "mSupport";
DEFINE_PGM_CTR(PM_MODULE_CORE_SUPPORT_FRIENDLY_CTR)         "support";

#include "2_CoreSystem/Logging/mLogging.h"
class mLogging;
#define            D_MODULE_CORE_LOGGING_ID                    3
#define            pCONT_sto                                 pCONT->mso
DEFINE_PGM_CTR(PM_MODULE_CORE_LOGGING_CTR)                 "mLogging";
DEFINE_PGM_CTR(PM_MODULE_CORE_LOGGING_FRIENDLY_CTR)        "logging";

#include "2_CoreSystem/Telemetry/mTelemetry.h"
class mTelemetry;
#define            D_MODULE_CORE_TELEMETRY_ID                   4
#define            pCONT_tel                                pCONT->mtel
DEFINE_PGM_CTR(PM_MODULE_CORE_TELEMETRY_CTR)                "mTelemetry";
DEFINE_PGM_CTR(PM_MODULE_CORE_TELEMETRY_FRIENDLY_CTR)       "telemetry";

#include "2_CoreSystem/Time/mTime.h"
class mTime;
#define            D_MODULE_CORE_TIME_ID 5
#define            pCONT_time                               pCONT->mt
DEFINE_PGM_CTR(PM_MODULE_CORE_TIME_CTR)                     "mTime";
DEFINE_PGM_CTR(PM_MODULE_CORE_TIME_FRIENDLY_CTR)            "time";

// Network (20-29)

#include "3_Network/WiFi/mWiFi.h"
class mWiFi;
#define D_MODULE_NETWORK_WIFI_ID 20
DEFINE_PGM_CTR(PM_MODULE_NETWORK_WIFI_CTR)              "mWiFi";
DEFINE_PGM_CTR(PM_MODULE_NETWORK_WIFI_FRIENDLY_CTR)              "wifi";
#define pCONT_wif pCONT->mwif

#include "3_Network/MQTT/mMQTT.h"
class mMQTT;
#define D_MODULE_NETWORK_MQTT_ID 21
DEFINE_PGM_CTR(PM_MODULE_NETWORK_MQTT_CTR)              "mMQTT";
DEFINE_PGM_CTR(PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR)              "system"; //changing to mqtt required total rewrite
#define pCONT_mqtt pCONT->mqt

#ifdef USE_MODULE_CORE_WEBSERVER
  #include "3_Network/WebServer/mWebServer.h"
  class mWebServer;
  #define D_MODULE_NETWORK_WEBSERVER_ID 22
  DEFINE_PGM_CTR(PM_MODULE_NETWORK_WEBSERVER_CTR)              "mWebServer";
  DEFINE_PGM_CTR(PM_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR)              "webserver";
  #define pCONT_web pCONT->mweb
#endif


// Displays (30-39)
#ifdef USE_MODULE_DISPLAYS_INTERFACE
  // #include "Displays/Nextion/mNextionPanel.h"
  // class mNextionPanel;
  // #define D_MODULE_DISPLAYS_NEXTION_ID 30
  // DEFINE_PGM_CTR(PM_MODULE_DISPLAYS_NEXTION_CTR)              "mNextionPanel";
  // DEFINE_PGM_CTR(PM_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR)              "nextionpanel";
  // #define pCONT_nex pCONT->mnext
#endif
#ifdef USE_MODULE_DISPLAYS_NEXTION
  #include "8_Displays/Nextion/mNextionPanel.h"
  class mNextionPanel;
  #define D_MODULE_DISPLAYS_NEXTION_ID 31
  DEFINE_PGM_CTR(PM_MODULE_DISPLAYS_NEXTION_CTR)              "mNextionPanel";
  DEFINE_PGM_CTR(PM_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR)              "nextionpanel";
  #define pCONT_nex pCONT->mnext
#endif


// Drivers (Range 40-129)
#ifdef USE_MODULE_DRIVERS_INTERFACE
  // #include "4_Drivers/Motors/HBridgeL9110/mHBridge.h"
  // class mHBridge;
  // #define D_MODULE_DRIVERS_HBRIDGE_ID 40
  // DEFINE_PGM_CTR(PM_MODULE_DRIVERS_HBRIDGE_CTR)              "mHBridge";
  // DEFINE_PGM_CTR(PM_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR)              "hbridge";
#endif
#ifdef USE_MODULE_DRIVERS_HBRIDGE
  #include "4_Drivers/Motors/HBridgeL9110/mHBridge.h"
  class mHBridge;
  #define D_MODULE_DRIVERS_HBRIDGE_ID 41
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_HBRIDGE_CTR)              "mHBridge";
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR)              "hbridge";
#endif
#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
  #include "4_Drivers/IRDevices/mIRtransceiver.h"
  class mIRtransceiver;
  #define D_MODULE_DRIVERS_IRTRANSCEIVER_ID 44
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_IRTRANSCEIVER_CTR)              "mIRtransceiver";
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_IRTRANSCEIVER_FRIENDLY_CTR)              "ir";
#endif
#ifdef USE_MODULE_DRIVERS_RELAY
  #include "4_Drivers/Relays/mRelays.h"
  class mRelays;
  #define            D_MODULE_DRIVERS_RELAY_ID       45
  #define            pCONT_mry                        pCONT->mry
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_RELAY_CTR)           D_MODULE_DRIVERS_RELAY_CTR;
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_RELAY_FRIENDLY_CTR)  D_MODULE_DRIVERS_RELAY_FRIENDLY_CTR;
#endif
#ifdef USE_MODULE_DRIVERS_PWM
  #include "4_Drivers/PWM/mPWM.h"
  class mPWM;
  #define            D_MODULE_DRIVERS_PWM_ID       46
  #define            pCONT_mpwm                        pCONT->mpwm
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_PWM_CTR)           D_MODULE_DRIVERS_PWM_CTR;
  DEFINE_PGM_CTR(PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR)  D_MODULE_DRIVERS_PWM_FRIENDLY_CTR;
#endif
#ifdef USE_MODULE_DRIVERS_RF433MHZ
  // #include "SAWRadios/mSAWMain.h"
  // class mSAWMain;
  // #define D_MSAW_MODULE_ID 48
  // DEFINE_PGM_CTR(MSAW_MODULE_CTR)              "mSAWMain";
  // DEFINE_PGM_CTR(MODULE_CORE_SETTINGS_FRIENDLY_CTR)              "rcswitch";
  // #include "SAWRadios/mSAWProtocol.h"
  // class mSAWProtocol;
  // #define mSAWProtocol_ID 25
#endif


// Energy (Range 130-139)
#ifdef USE_MODULE_DRIVERS_ENERGY
  #include "7_Energy/_Interface/mEnergy.h"
  class mEnergy;
  #define            D_MODULE_DRIVERS_ENERGY_ID                   42
  #define            pCONT_iEnergy                                pCONT->mdenergy  
  DEFINE_PGM_CTR(PM_INTERFACE_ENERGY_MODULE_CTR)              "mEnergy";
  DEFINE_PGM_CTR(PM_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR)     "energy";
#endif


// Lights (Range 140-169)
#ifdef USE_MODULE_LIGHTS_INTERFACE
  #include "6_Lights/_Interface/mInterfaceLight.h"
  class mInterfaceLight;
  #define D_MODULE_LIGHTS_INTERFACE_ID 140
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_INTERFACE_CTR)              "mInterfaceLight";
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR)              "lightinterface";
  #define pCONT_iLight pCONT->mil
#endif
#ifdef USE_MODULE_LIGHTS_ADDRESSABLE
  #include "6_Lights/AddressableLights/mRGBAnimator.h"
  class mRGBAnimator;
  #define D_MODULE_LIGHTS_ADDRESSABLE_ID 141
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_ADDRESSABLE_CTR)              "RGBAdressable";
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR)              "pixels";
  #define pCONT_ladd pCONT->mrgbani
#endif
#ifdef USE_MODULE_LIGHTS_PWM
  #include "6_Lights/PWMLights/mPWMLight.h"
  class mPWMLight;
  #define D_MODULE_LIGHTS_PWM_ID 142
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_PWM_CTR)              D_MODULE_LIGHTS_PWM_CTR;
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_PWM_FRIENDLY_CTR)              D_MODULE_LIGHTS_PWM_FRIENDLY_CTR;
  #define pCONT_lPWM pCONT->mlights_pwm
#endif
#ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
  #include "6_Lights/WLEDEffects/mWLEDEffects.h"
  class mWLEDEffects;
  #define D_MODULE_LIGHTS_WLED_EFFECTS_ID 143
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_WLED_EFFECTS_CTR)              D_MODULE_LIGHTS_WLED_EFFECTS_CTR;
  DEFINE_PGM_CTR(PM_MODULE_LIGHTS_WLED_EFFECTS_FRIENDLY_CTR)     D_MODULE_LIGHTS_WLED_EFFECTS_FRIENDLY_CTR;
  #define pCONT_lwled pCONT->mlights_wled
#endif




// Sensors (Range 120-169)
#ifdef USE_MODULE_SENSORS_BUTTONS
  #include "5_Sensors/Buttons/mButtons.h"
  class mButtons;
  #define D_MODULE_SENSORS_BUTTONS_ID 120
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_BUTTONS_CTR)              "mButtons";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR)              "buttons";
#endif
#ifdef USE_MODULE_SENSORS_SWITCHES
  #include "5_Sensors/Switches/mSwitches.h"
  class mSwitches;
  #define D_MODULE_SENSORS_SWITCHES_ID 121
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_SWITCHES_CTR)              "mSwitches";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR)              "switches";
  #define pCONT_swt                                               pCONT->mswh
#endif
#ifdef USE_MODULE_SENSORS_ANALOG
  #include "5_Sensors/Analog/mSensorsAnalog.h"
  class mSensorsAnalog;
  #define D_MODULE_SENSORS_ANALOG_ID 122
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_ANALOG_CTR)              D_MODULE_SENSORS_ANALOG_CTR;
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_ANALOG_FRIENDLY_CTR)     D_MODULE_SENSORS_ANALOG_FRIENDLY_CTR;
  #define pCONT_msanalog                                    pCONT->msanalog
#endif
#ifdef USE_MODULE_SENSORS_DOORCHIME
  #include "5_Sensors/DoorBell/mDoorBell.h"
  class mDoorBell;
  #define D_MODULE_SENSORS_DOORBELL_ID 123
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DOORBELL_CTR)              "mDoorBell";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DOORBELL_FRIENDLY_CTR)              "doorbell";
  #define pCONT_mdb                                           pCONT->mdb
#endif
#ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
  #include "7_Energy/PowerMeter/mPzem_AC.h"
  class mPzem_AC;  
  #define pCONT_pzem                                           pCONT->mspm
  #define D_MODULE_SENSORS_PZEM004T_MODBUS_ID 124
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_PZEM004T_MODBUS_CTR)              "mPzem_AC";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_PZEM004T_MODBUS_FRIENDLY_CTR)              "powermeter_pzem";
#endif
#ifdef USE_MODULE_SENSORS_DHT
  #include "5_Sensors/DHT/mSensorsDHT.h"
  class mSensorsDHT;
  #define            D_MODULE_SENSORS_DHT_ID                   125
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DHT_CTR)               "mSensorsDHT";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DHT_FRIENDLY_CTR)      "dht";
  #define pCONT_dht                                           pCONT->msdht
#endif
#ifdef USE_MODULE_SENSORS_BME
  #include "5_Sensors/BME/mSensorsBME.h"
  class mSensorsBME;
  #define            D_MODULE_SENSORS_BME_ID                  126
  #define            pCONT_bme                               pCONT->msbme
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_BME_CTR)              "mSensorsBME";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_BME_FRIENDLY_CTR)     "bme";
#endif
#ifdef USE_MODULE_SENSORS_DS18B20
  #include "5_Sensors/DB18/mSensorsDB18.h"
  class mSensorsDB18;
  #define D_MODULE_SENSORS_DB18S20_ID 127
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DB18S20_CTR)              "mSensorsDB18";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DB18S20_FRIENDLY_CTR)              "db18s20";
  #define pCONT_msdb18        pCONT->msdb18
#endif
#ifdef USE_MODULE_SENSORS_INA219
  #include "5_Sensors/INA219/mSensorsINA219.h"
  class mSensorsINA219;
  #define D_MODULE_SENSORS_INA219_ID 128
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_INA219_CTR)              "mSensorsINA219";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_INA219_FRIENDLY_CTR)     "ina219";
  #define pCONT_mina219        pCONT->msina219
#endif
#ifdef USE_MODULE_SENSORS_ULTRASONICS
  #include "5_Sensors/UltraSonic/mUltraSonicSensor.h"
  class mUltraSonicSensor;
  #define D_MODULE_SENSORS_ULTRASONIC_ID 129
  #define            pCONT_ult                               pCONT->mus
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_ULTRASONIC_CTR)           "mUltraSonicSensor";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_ULTRASONIC_FRIENDLY_CTR)     "ultrasonic";
#endif
#ifdef USE_MODULE_SENSORS_DOOR
  #include "5_Sensors/Door/mDoorSensor.h"
  class mDoorSensor;
  #define D_MODULE_SENSORS_DOOR_ID 130
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DOOR_CTR)                D_MODULE_SENSORS_DOOR_CTR;
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_DOOR_FRIENDLY_CTR)       D_MODULE_SENSORS_DOOR_FRIENDLY_CTR;
  #define pCONT_msdb18        pCONT->msdb18
#endif
#ifdef USE_MODULE_SENSORS_MOTION
  #include "5_Sensors/PIR/mMotionSensor.h"
  class mMotionSensor;
  #define D_MODULE_SENSORS_MOTION_ID 131
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_MOTION_CTR)                D_MODULE_SENSORS_MOTION_CTR;
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR)       D_MODULE_SENSORS_MOTION_FRIENDLY_CTR;
#endif
#ifdef USE_MODULE_SENSORS_MOISTURE
  #include "5_Sensors/Moisture/mMoistureSensor.h"
  class mMoistureSensor;
  #define D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID 132
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_RESISTIVE_MOISTURE_CTR)              "mMoistureSensor";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_RESISTIVE_MOISTURE_FRIENDLY_CTR)              "moisture";
#endif
#ifdef USE_MODULE_SENSORS_PULSE_COUNTER
  #include "5_Sensors/PulseCounter/mPulseCounter.h"
  class mPulseCounter;
  #define D_MODULE_SENSORS_PULSECOUNTER_ID 133
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_PULSECOUNTER_CTR)              "mPulseCounter";
  DEFINE_PGM_CTR(PM_MODULE_SENSORS_PULSECOUNTER_FRIENDLY_CTR)              "pulsecounter";
#endif


// Specefic Bespoke Modules (Range 170-189)
#ifdef USE_MODULE_CUSTOM_BLINDS
  #include "9_Controller/Blinds/mBlinds.h"
  class mBlinds;
  #define D_MODULE_CUSTOM_BLINDS_ID 170
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_BLINDS_CTR)              "mBlinds";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_BLINDS_FRIENDLY_CTR)              "blinds";
#endif
#ifdef USE_MODULE_CUSTOM_HEATING
  #include "9_Controller/Heating/mHeating.h"
  class mHeating;
  #define D_MODULE_CUSTOM_HEATING_ID 171
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_HEATING_CTR)              "mHeating";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_HEATING_FRIENDLY_CTR)              "heating";
#endif
#ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
  #include "9_Controller/SecurityLight/mSecurityLight.h"
  class mSecurityLight;
  #define D_MODULE_CUSTOM_SECURITYLIGHT_ID 174
  #define            pCONT_slgt                               pCONT->slgt
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_SECURITYLIGHT_CTR)              "mSecurityLight";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_SECURITYLIGHT_FRIENDLY_CTR)              "securitylight";
#endif
#ifdef USE_MODULE_CUSTOM_RADIATORFAN
  #include "9_Controller/RadiatorFan/mRadiatorFan.h"
  class mRadiatorFan;
  #define D_MODULE_CUSTOM_RADIATORFAN_ID 175
  DEFINE_PGM_CTR(MODULE_CUSTOM_RADIATORFAN_CTR)              "mRadiatorFan";
  DEFINE_PGM_CTR(MODULE_CUSTOM_RADIATORFAN_FRIENDLY_CTR)              "radiatorfan";
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
#endif
#ifdef USE_MODULE_CUSTOM_OILFURNACE
  #include "9_Controller/OilFurnace/mOilFurnace.h"
  class mOilFurnace;
  #define D_MODULE_CUSTOM_OILFURNACE_ID 177
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_OILFURNACE_CTR)              "mOilFurnace";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_OILFURNACE_FRIENDLY_CTR)              "oilfurnace";
#endif
#ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
  #include "9_Controller/ExerciseBike/mExerciseBike.h"
  class mExerciseBike;
  #define D_MODULE_CUSTOM_EXERCISEBIKE_ID 178
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_EXERCISEBIKE_CTR)              "mExerciseBike";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_EXERCISEBIKE_FRIENDLY_CTR)              "exercisebike";
#endif
#ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
  #include "9_Controller/Sonoff_iFan/mSonoffIFan.h"
  class mSonoffIFan;
  #define D_MODULE_CUSTOM_SONOFF_IFAN_ID                             179
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_IFAN_CTR)               "mSonoffIFan";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_IFAN_FRIENDLY_CTR)      "ifan";
  #define pCONT_ifan                                           pCONT->mifan
#endif
#ifdef USE_MODULE_CUSTOM_PWM_FAN
  #include "9_Controller/PWMFan/mPWMFan.h"
  class mPWMFan;
  #define D_MODULE_CUSTOM_PWM_FAN_ID                            180
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_PWMFAN_CTR)               "mPWMFan";
  DEFINE_PGM_CTR(PM_MODULE_CUSTOM_PWMFAN_FRIENDLY_CTR)      "pwmfan";
  #define pCONT_mpwmfan                                         pCONT->mpwmfan
#endif


DEFINE_PGM_CTR(PM_FUNC_POINTER_INIT_CTR)                            D_FUNC_POINTER_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_TEMPLATE_LOAD_CTR)                           D_FUNC_TEMPLATE_LOAD_CTR;
DEFINE_PGM_CTR(PM_FUNC_MODULE_INIT_CTR)                             D_FUNC_MODULE_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_PRE_INIT_CTR)                                D_FUNC_PRE_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_INIT_CTR)                                    D_FUNC_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR)            D_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR;
DEFINE_PGM_CTR(PM_FUNC_LOOP_CTR)                                    D_FUNC_LOOP_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_50_MSECOND_CTR)                        D_FUNC_EVERY_50_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_100_MSECOND_CTR)                       D_FUNC_EVERY_100_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_200_MSECOND_CTR)                       D_FUNC_EVERY_200_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_250_MSECOND_CTR)                       D_FUNC_EVERY_250_MSECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_SECOND_CTR)                            D_FUNC_EVERY_SECOND_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_MINUTE_CTR)                            D_FUNC_EVERY_MINUTE_CTR; 
DEFINE_PGM_CTR(PM_FUNC_EVERY_HOUR_CTR)                              D_FUNC_EVERY_HOUR_CTR; 
DEFINE_PGM_CTR(PM_FUNC_EVERY_MIDNIGHT_CTR)                          D_FUNC_EVERY_MIDNIGHT_CTR;
DEFINE_PGM_CTR(PM_FUNC_EVERY_MIDDAY_CTR)                            D_FUNC_EVERY_MIDDAY_CTR;
DEFINE_PGM_CTR(PM_FUNC_ON_SUCCESSFUL_BOOT_CTR)                      D_FUNC_ON_SUCCESSFUL_BOOT_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_10_SECONDS_CTR)                       D_FUNC_UPTIME_10_SECONDS_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_1_MINUTES_CTR)                        D_FUNC_UPTIME_1_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_10_MINUTES_CTR)                       D_FUNC_UPTIME_10_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_UPTIME_60_MINUTES_CTR)                       D_FUNC_UPTIME_60_MINUTES_CTR;
DEFINE_PGM_CTR(PM_FUNC_RESTART_SPLASH_INFORMATION_CTR)              D_FUNC_RESTART_SPLASH_INFORMATION_CTR;
DEFINE_PGM_CTR(PM_FUNC_PREP_BEFORE_TELEPERIOD_CTR)                  D_FUNC_PREP_BEFORE_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_APPEND_CTR)                             D_FUNC_JSON_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_SAVE_BEFORE_RESTART_CTR)                     D_FUNC_SAVE_BEFORE_RESTART_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_DEFAULT_CTR)                        D_FUNC_SETTINGS_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR)     D_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR)     D_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR)        D_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR)        D_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_FUNCTION_LAMBDA_INIT_CTR)                    D_FUNC_FUNCTION_LAMBDA_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_FUNCTION_LAMBDA_LOOP_CTR)                    D_FUNC_FUNCTION_LAMBDA_LOOP_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_CTR)                                 D_FUNC_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_SENSOR_CTR)                          D_FUNC_COMMAND_SENSOR_CTR;
DEFINE_PGM_CTR(PM_FUNC_COMMAND_DRIVER_CTR)                          D_FUNC_COMMAND_DRIVER_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_COMMAND_CTR)                            D_FUNC_JSON_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_COMMAND_OBJECT_CTR)                     D_FUNC_JSON_COMMAND_OBJECT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WIFI_CONNECTED_CTR)                          D_FUNC_WIFI_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WIFI_DISCONNECTED_CTR)                       D_FUNC_WIFI_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_SUBSCRIBE_CTR)                          D_FUNC_MQTT_SUBSCRIBE_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_INIT_CTR)                               D_FUNC_MQTT_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_CONNECTED_CTR)                          D_FUNC_MQTT_CONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_DISCONNECTED_CTR)                       D_FUNC_MQTT_DISCONNECTED_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_COMMAND_CTR)                            D_FUNC_MQTT_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_SENDER_CTR)                             D_FUNC_MQTT_SENDER_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_RESET_CTR)                     D_FUNC_MQTT_HANDLERS_RESET_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_INIT_CTR)                      D_FUNC_MQTT_HANDLERS_INIT_CTR;
DEFINE_PGM_CTR(PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR)        D_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_POWER_CTR)                               D_FUNC_SET_POWER_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_DEVICE_POWER_CTR)                        D_FUNC_SET_DEVICE_POWER_CTR;
DEFINE_PGM_CTR(PM_FUNC_SHOW_SENSOR_CTR)                             D_FUNC_SHOW_SENSOR_CTR;
DEFINE_PGM_CTR(PM_FUNC_RULES_PROCESS_CTR)                           D_FUNC_RULES_PROCESS_CTR;
DEFINE_PGM_CTR(PM_FUNC_SERIAL_CTR)                                  D_FUNC_SERIAL_CTR;
DEFINE_PGM_CTR(PM_FUNC_FREE_MEM_CTR)                                D_FUNC_FREE_MEM_CTR;
DEFINE_PGM_CTR(PM_FUNC_BUTTON_PRESSED_CTR)                          D_FUNC_BUTTON_PRESSED_CTR;
DEFINE_PGM_CTR(PM_FUNC_ENERGY_RESET_CTR)                            D_FUNC_ENERGY_RESET_CTR;
DEFINE_PGM_CTR(PM_FUNC_SENSOR_UPDATED_CTR)                          D_FUNC_SENSOR_UPDATED_CTR;
DEFINE_PGM_CTR(PM_FUNC_STATUS_MESSAGE_APPEND_CTR)                   D_FUNC_STATUS_MESSAGE_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_JSON_HARDWARE_APPEND_CTR)                    D_FUNC_JSON_HARDWARE_APPEND_CTR;
DEFINE_PGM_CTR(PM_FUNC_SET_CHANNELS_CTR)                            D_FUNC_SET_CHANNELS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_MAIN_BUTTON_CTR)                     D_FUNC_WEB_ADD_MAIN_BUTTON_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_BUTTON_CTR)                          D_FUNC_WEB_ADD_BUTTON_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR)          D_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_HANDLER_CTR)                         D_FUNC_WEB_ADD_HANDLER_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_STYLE_CTR)                     D_FUNC_WEB_ROOT_SEND_STYLE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_SCRIPT_CTR)                    D_FUNC_WEB_ROOT_SEND_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR) D_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_BODY_CTR)                      D_FUNC_WEB_ROOT_SEND_BODY_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ROOT_SEND_STATUS_CTR)                    D_FUNC_WEB_ROOT_SEND_STATUS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_SHOWS_CTR)                      D_FUNC_WEB_ADD_ROOT_SHOWS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR)               D_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR)             D_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR)                 D_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_SCRIPT_CTR)                     D_FUNC_WEB_ADD_ROOT_SCRIPT_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_ADD_ROOT_STYLE_CTR)                      D_FUNC_WEB_ADD_ROOT_STYLE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR)           D_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR)          D_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR)            D_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR)           D_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR)  D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR)     D_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR)        D_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR)                 D_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR)         D_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_COMMAND_CTR)                             D_FUNC_WEB_COMMAND_CTR;
DEFINE_PGM_CTR(PM_FUNC_CHECK_POINTERS_CTR)                          D_FUNC_CHECK_POINTERS_CTR;
DEFINE_PGM_CTR(PM_FUNC_WEB_SYSTEM_INFO_CTR)                         D_FUNC_WEB_SYSTEM_INFO_CTR;
DEFINE_PGM_CTR(PM_FUNC_DEBUG_CONFIGURE_CTR)                         D_FUNC_DEBUG_CONFIGURE_CTR;




class mTaskerManager{

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
    static mTaskerManager* GetInstance();

    // HardwarePins
    mHardwarePins *mod = nullptr;
    // interface_handler<mHardwarePins> interfacehandler_logging;
    // Settings
    mSettings *mset = nullptr;    
    // interface_handler<mSettings> interfacehandler_settings;
    // Support
    mSupport *msup = nullptr;
    // interface_handler<mSupport> interfacehandler_support;
    // Logging
    mLogging *mso = nullptr;
    // interface_handler<mLogging> interfacehandler_logging;
    


    mTime *mt = nullptr;
    mMQTT *mqt = nullptr;



    mWiFi *mwif = nullptr;
    mTelemetry *mtel = nullptr;

    #ifdef USE_MODULE_CORE_WEBSERVER
    mWebServer *mweb = nullptr;
    #endif

  #ifdef USE_MODULE_SENSORS_DHT
    mSensorsDHT *msdht = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    mSensorsBME *msbme = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    mSensorsDB18 *msdb18 = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_INA219
    mSensorsINA219 *msina219 = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    mUltraSonicSensor *mus = nullptr;
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    mOilFurnace *mof = nullptr;
  #endif
  #ifdef USE_MODULE_CUSTOM_BLINDS
    mBlinds *mbbl = nullptr;
  #endif
  #ifdef   USE_MODULE_SENSORS_PULSE_COUNTER
    mPulseCounter *mpc = nullptr;
  #endif

  
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    mHBridge *mdhb = nullptr;
  #endif

  #ifdef USE_MODULE_DRIVERS_ENERGY
    mEnergy *mdenergy = nullptr;
  #endif



  /**
   * Lights
   * */
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    mInterfaceLight *mil = nullptr;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    mRGBAnimator *mrgbani = nullptr;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    mPWMLight *mlights_pwm = nullptr;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
    mWLEDEffects *mlights_wled = nullptr;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif



  #ifdef USE_MODULE_CUSTOM_HEATING
    mHeating* mh = nullptr;
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    mRelays* mry = nullptr;
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM  //this is really a pin class, but keep it on its own for now
    mPWM* mpwm = nullptr;
  #endif
  #ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
    mSecurityLight* slgt = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    mMotionSensor *mms = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    mDoorSensor *mds = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_DOORCHIME
    mDoorBell *mdb = nullptr;
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    mRadiatorFan* mrf = nullptr;
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    mSonoffIFan* mifan = nullptr;
  #endif
  #ifdef USE_MODULE_CUSTOM_PWM_FAN
    mPWMFan* mpwmfan = nullptr;
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    mIRtransceiver* mir = nullptr;
  #endif
  #if defined(USE_IR_RECEIVER) || defined(USE_MODULE_CUSTOM_IRTRANSMITTER)
    void parse_IRCodes(char* topic, char* payload, unsigned int length);
  #endif
  #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined (USE_MODULE_DRIVERS_RF433MHZ)
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
   mNextionPanel* mnext = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    mMoistureSensor *mois = nullptr;
  #endif

  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    mButtons *mbtn = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    mSwitches *mswh = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    mPzem_AC *mspm = nullptr;
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    mSensorsAnalog *msanalog = nullptr;
  #endif

  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    mExerciseBike *meb = nullptr;
  #endif

  uint8_t Instance_Init();
  uint8_t CheckPointersPass();
  // returns flag if handled succesfully
  
  // template<typename T>
  // int8_t Tasker_Interface(uint8_t function, T param1, uint8_t target_tasker);
  // Overload
  int8_t Tasker_Interface(uint8_t function, uint8_t target_tasker = 0);

  // std::variant< int, std::string > GetClassObjectbyID(uint8_t id);
  // void TaskerTest();

  const char* GetTaskName(uint8_t task, char* buffer);
  PGM_P GetClassName(uint8_t id);
  uint8_t InitClassList();
  PGM_P GetModuleFriendlyName(uint8_t module_id);
  uint16_t GetClassCount();
  void Init_InterfaceHandler();

  // enum 

  #define CLASS_ID_MAX 255
  struct CLASS_ID{
    uint8_t list[CLASS_ID_MAX];
    uint8_t module_type[CLASS_ID_MAX];
    #ifdef DEBUG_EXECUTION_TIME
    uint16_t execution_time_average_ms[CLASS_ID_MAX];
    uint16_t execution_time_max_ms[CLASS_ID_MAX];
    #endif
    uint8_t count = 0;
  }module_settings; 

  uint16_t last_function = 255; // 0 will be first

  uint8_t fExitTaskerWithCompletion = false;

  uint8_t switch_index = 0;

};

#endif
