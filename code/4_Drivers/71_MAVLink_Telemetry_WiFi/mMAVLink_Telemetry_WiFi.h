/*
  mPZEM004T.h - PZEM004T v3 MODBUS

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef HEADER_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__H
#define HEADER_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__H
  
#define D_UNIQUE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__ID ((4*1000)+03)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"


#include <stdio.h>
#include <nvs_flash.h>
#include <esp_wifi_types.h>
#include <mdns.h>
#include <string.h>
#include <driver/gpio.h>
#include <lwip/apps/netbiosns.h>
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "db_esp32_control.h"
#include "http_server.h"
#include "db_esp32_comm.h"
#include "db_protocol.h"
// #include "esp_vfs_semihost.h"
#include "esp_spiffs.h"
#include "http_server.h"
#include "main.h"


class mMAVLink_Telemetry_WiFi :
  public mTaskerInterface
{

  private:
  public:
    mMAVLink_Telemetry_WiFi(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__CTR = D_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mMAVLink_Telemetry_WiFi);
    };
    #endif
    
    void CommandSet_LED_Power(uint8_t state, uint8_t index = 0);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t leds_found = 0;
    }settings;

// const uint8_t MAX_LEDS = 8;                 // Max number of leds

/**
 * @brief 
 * Definitely need to move to "mpin" since LED1-8 is a hard limit of 8, hence I can turn 1-8 as status type
 * Or else LED1 will always be "status" for combined mqtt/wifi connected
 * 
 * 
 * StatusLED
 * - Single blinks = wifi
 * - Double blinks = mqtt
 * - Solid LED = Power state
 * 
 */


void SubTask_Status_LEDs();
struct STATUS_LEDs{
  uint32_t tSaved_Update = millis();
  bool flag_enabled = false;

  // I need a way to enable much slow (than 1 second)
  uint16_t freq = 0; // flashes per 1 second


}status_leds;




#define MAX_LEDS 8

/**
 * @brief Bitpacked LEDs
 * 
 */
uint8_t leds_present = 0;                   // Max number of LED supported
uint8_t led_inverted = 0;                   // LED inverted flag (1 = (0 = On, 1 = Off))
uint8_t led_power = 0;                      // LED power state
uint8_t ledlnk_inverted = 0;                // Link LED inverted flag (1 = (0 = On, 1 = Off))

  bool GetState(uint8_t index);
  void SetState(uint8_t index);


    struct LEDS
    {    
    
      int8_t pin = -1; // -1 is not active

    }leds[MAX_LEDS];


    void EveryLoop();

    void Pre_Init();
    void Init(void);
    
    void parse_JSONCommand(JsonParserObject obj);

    void UpdateStatusBlink();
    
    void UpdateLedPowerAll();
    void SetLedPowerIdx(uint32_t led, uint32_t state);
    void SetLedPower(uint32_t state);
    void SetLedPowerAll(uint32_t state);
    void SetLedLink(uint32_t state);


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    
    struct handler<mMAVLink_Telemetry_WiFi> mqtthandler_settings_teleperiod;
    struct handler<mMAVLink_Telemetry_WiFi> mqtthandler_state_ifchanged;

    // No specialised payload therefore use system default instead of enum
      

    struct handler<mMAVLink_Telemetry_WiFi>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

    
};

#endif // USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI

#endif