/*
  mEnergyOLED.h 
  
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
#ifndef HEADER_CONTROLLER__ENERGY_OLED_H
#define HEADER_CONTROLLER__ENERGY_OLED_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_CONTROLLER__ENERGY_OLED_ID   9060 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_CONTROLLER__ENERGY_OLED

// later, can I make this cycling through all sensors and show them line by line, and/or, show energy line by line

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mEnergyOLED :
  public mTaskerInterface
{

  private:
  public:
    mEnergyOLED(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_CONTROLLER__ENERGY_OLED_CTR = D_MODULE_CONTROLLER__ENERGY_OLED_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER__ENERGY_OLED_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER__ENERGY_OLED_ID; }
    
    

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    void Pre_Init();
    void Init();

    void EverySecond();
    void SubTask_UpdateOLED();
    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();

    struct handler<mEnergyOLED> mqtthandler_settings;
    struct handler<mEnergyOLED> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    
      
    struct handler<mEnergyOLED>* mqtthandler_list[2] = {
      &mqtthandler_settings,
      &mqtthandler_state_ifchanged
    };

};

#endif // USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED

#endif
