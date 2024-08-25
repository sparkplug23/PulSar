/*
  mDeskSensorsOnOLED.h 
  
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
#ifndef HEADER_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_H
#define HEADER_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_ID   10020

#ifdef USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED

// later, can I make this cycling through all sensors and show them line by line, and/or, show energy line by line

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mDeskSensorsOnOLED :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mDeskSensorsOnOLED(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_CTR = D_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED_ID; }    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mDeskSensorsOnOLED);    };
    #endif    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void EverySecond();
    void SubTask_UpdateOLED();
    void SubTask_UpdateOLED_Page1();
    void SubTask_UpdateOLED_Page2();
    uint8_t page_showing_index = 0;
    uint8_t pages_active = 2;
    
    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mDeskSensorsOnOLED>*> mqtthandler_list;
    struct handler<mDeskSensorsOnOLED> mqtthandler_settings_teleperiod;
    struct handler<mDeskSensorsOnOLED> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif // USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED

#endif
