/*
  mCellularBlackBox.h 
  
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
#ifndef HEADER_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_H
#define HEADER_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_ID ((10*1000)+8) 

#ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

// #include <Averaging_Data.h>

class mCellularBlackBox :
  public mTaskerInterface
{

  private:
  public:
    mCellularBlackBox(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_CTR;
    static const char* PM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mCellularBlackBox); };
    #endif

    
    // Averaging_Data<float>* x;
    

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    struct DISPLAY_MESSAGE{
      char line0[22] = {0};
      char line1[22] = {0};
      char line2[22] = {0};
      char line3[22] = {0};
      char line4[22] = {0};
      char line5[22] = {0};
      char line6[22] = {0};
      char line7[22] = {0};
    }display_message;

    void Pre_Init();
    void Init();

    void EverySecond();
    void SubTask_UpdateOLED();
    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mCellularBlackBox> mqtthandler_settings_teleperiod;
    struct handler<mCellularBlackBox> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
      
    struct handler<mCellularBlackBox>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

};

#endif // USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX

#endif
