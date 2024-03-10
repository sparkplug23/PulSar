/*
  mTreadmillLogger.h 
  
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
#ifndef HEADER_CONTROLLER_CUSTOM__TREADMILL_LOGGER_H
#define HEADER_CONTROLLER_CUSTOM__TREADMILL_LOGGER_H
  
#include "1_TaskerManager/mTaskerManager.h"
#define D_UNIQUE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_ID   10012

#ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER

// later, can I make this cycling through all sensors and show them line by line, and/or, show energy line by line

#include <Arduino.h>


#include "DoubleEMAFilterLib.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mTreadmillLogger :
  public mTaskerInterface
{

  private:
  public:
    mTreadmillLogger(){};
    void init(void);
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static const char* PM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_CTR;
    static const char* PM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mTreadmillLogger); };
    #endif
    

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    void Pre_Init();
    void Init();

    void EverySecond();
    void SubTask_UpdateOLED();

    struct RuntimeData{
      float estimated_speed = 0;
      struct DoubleEMAFilter_VALUES{
        float value = 0;
        float alpha1 = 0;
        float alpha2 = 0;
        DoubleEMAFilter<float>* filter = nullptr;
      }estimated_speed_smoothed;
    }rt;
    
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender();

    struct handler<mTreadmillLogger> mqtthandler_settings_teleperiod;
    struct handler<mTreadmillLogger> mqtthandler_state_ifchanged;

    //No extra handlers: ie settings and "state" only
    
      
    struct handler<mTreadmillLogger>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged
    };

};

#endif // USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED

#endif
