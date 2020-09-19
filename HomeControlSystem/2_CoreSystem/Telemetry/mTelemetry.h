
// KEEP - useful

//   sonoff.h - Master header file for Sonoff-Tasmota

//   Copyright (C) 2019  Theo Arends

//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation, either version 3 of the License, or
//   (at your option) any later version.

//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.

//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.


//#ifdef ESP8266
#ifndef _MTELEMETRY_H_
#define _MTELEMETRY_H_


#include "1_ConfigUser/mUserConfig.h"

#include "2_CoreSystem/Support/mSupport.h"

#include "2_CoreSystem/JSON/mJSON.h"


#include "2_CoreSystem/Logging/mLogging.h"


#include <ArduinoJson.h>
#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


#include "2_CoreSystem/Languages/mLanguage.h"
#include "2_CoreSystem/mHardwareTemplates.h"

#ifdef ESP32
  #include <WiFi.h>
#endif
#ifdef ESP8266
  #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
  #include <ESP8266httpUpdate.h>
#endif

#include "2_CoreSystem/Time/mTime.h"

enum HARDWARE_STATUS_IMPORTANCE_IDS{
  // No risk to remain on
  HARDWARE_STATUS_IMPORTANCE_LOW_ID=0,
  // RGB leds, not high voltage
  HARDWARE_STATUS_IMPORTANCE_MEDIUM_ID,
  // Plugs/sockets, mains running
  HARDWARE_STATUS_IMPORTANCE_HIGHEST_ID,
};


/*********************************************************************************************\
 * Watchdog extension (https://github.com/esp8266/Arduino/issues/1532)
\*********************************************************************************************/

#include <Ticker.h>

class mTelemetry{
  public:
    mTelemetry(){};
    
    int8_t Tasker(uint8_t function);
    void init(void);

    uint8_t ConstructJSON_Health(uint8_t json_level);
    uint8_t ConstructJSON_Settings(uint8_t json_level);
    uint8_t ConstructJSON_Firmware(uint8_t json_level);
    uint8_t ConstructJSON_Log(uint8_t json_level);
    uint8_t ConstructJSON_Memory(uint8_t json_level);
    uint8_t ConstructJSON_Network(uint8_t json_level);
    uint8_t ConstructJSON_MQTT(uint8_t json_level);
    uint8_t ConstructJSON_Time(uint8_t json_level);
    uint8_t ConstructJSON_Devices(uint8_t json_level);
    uint8_t ConstructJSON_Reboot(uint8_t json_level);
    #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
    uint8_t ConstructJSON_Debug_Pins(uint8_t json_level);
    uint8_t ConstructJSON_Debug_Template(uint8_t json_level);
    uint8_t ConstructJSON_Debug_ModuleInterface(uint8_t json_level);
    uint8_t ConstructJSON_Debug_Minimal(uint8_t json_level);
    #endif

    void Web_Status_Telemetry_Health_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Settings_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Firmware_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Log_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Memory_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Network_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_MQTT_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Time_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Devices_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Reboot_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Debug_Minimal_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Debug_Pins_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Debug_Template_JSON(AsyncWebServerRequest *request);
    void Web_Status_Telemetry_Debug_ModuleInterface_JSON(AsyncWebServerRequest *request);
    
    #define HARDWARE_STATUS_MAX_LENGTH 200
    struct STATUSHARDWARE{
//make function that appends pretty simple message

      char ctr[HARDWARE_STATUS_MAX_LENGTH];
      uint8_t len = 0;
      uint8_t importance = 0; //0 low, 1 med, 2 high
    }hardwarestatus;
    void ConstructCtr_HardwareStatus();

    uint16_t wifi_reconnects_counter = 0;

    void Init();

    #define HARDWARE_STATUS_OFFSET_INDEX MQTT_MAX_PACKET_SIZE/2

    uint8_t fParseTasmotaCommand = false;

    uint32_t tSavedTest;

    uint32_t loop_delay = 100;





};





#endif  // _SONOFF_H_
//#endif