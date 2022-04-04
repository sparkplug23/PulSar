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
#ifndef HEADER_MODULE_DRIVERS_LEDS_H
#define HEADER_MODULE_DRIVERS_LEDS_H
  
#define D_UNIQUE_MODULE_DRIVERS_LEDS_ID 132

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_LEDS

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"


class mLEDs :
  public mTaskerInterface
{

  private:
  public:
    mLEDs(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_DRIVERS_LEDS_CTR;
    static const char* PM_MODULE_DRIVERS_LEDS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_LEDS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_LEDS_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_LEDS_ID; }
    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mLEDs);
    };
    #endif
    
    
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t leds_found = 0;
    }settings;

// const uint8_t MAX_LEDS = 8;                 // Max number of leds

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


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_State(uint8_t json_method = 0);
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mLEDs> mqtthandler_settings_teleperiod;
    struct handler<mLEDs> mqtthandler_state_ifchanged;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;  

    struct handler<mLEDs>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

    
};

#endif // USE_MODULE_DRIVERS_LEDS

#endif