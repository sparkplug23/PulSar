/*
  mImmersionPanel.h - Louvolite Blinds wireless control with SAW 433MHz transmitter

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
#ifndef HEADER_CONTROLLER_CUSTOM__IMMERSION_PANEL_H
#define HEADER_CONTROLLER_CUSTOM__IMMERSION_PANEL_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL_ID ((10*1000)+6) 

#ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"


class mImmersionPanel :
  public mTaskerInterface
{

  private:
  public:
    mImmersionPanel(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL_CTR = D_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mImmersionPanel); };
    #endif
    

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // uint8_t found = 0;
      // uint16_t rate_measure_ms = 1000;
      // // uint8_t sensor_count = 1;
      // uint8_t active_sensor = 0;
    }settings;

    struct STATES
    {
      uint8_t button1_state = 0;
      uint8_t button2_state = 0;

    }data;

    void Pre_Init();
    void Init();

    void EverySecond();
    void SubTask_UpdateOLED();
    void Event_InputStateChanged();
    void SubTask_UpdateStatusLEDs();
    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();

    struct handler<mImmersionPanel> mqtthandler_settings;
    struct handler<mImmersionPanel> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    
      
    struct handler<mImmersionPanel>* mqtthandler_list[2] = {
      &mqtthandler_settings,
      &mqtthandler_state_ifchanged
    };




};

#endif // USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL

#endif
