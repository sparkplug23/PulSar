/*
  mHVAC.cpp - mSensorsL3G

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
#ifndef MSENSORSL3G_H
#define MSENSORSL3G_H

#define D_UNIQUE_MODULE_SENSORS_L3G_ID 5013 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_L3G

#include "5_Sensors/13_L3GD20_3Axis_Gryo/internal/L3G.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mSensorsL3G :
  public mTaskerInterface
{
  
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mSensorsL3G(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_L3G_CTR = D_MODULE__SENSORS__L3G__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_L3G_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_L3G_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
    
    L3G* gyro = nullptr;

    struct SETTINGS{
    //   uint8_t fEnableSensor= false;
    //   uint8_t fSensorCount= 0; 
    //   uint8_t sModuleStatus =0;
      uint16_t measure_rate_ms = 1000;
    }settings;
    uint32_t tSavedMeasure = millis();


    void EveryLoop();

    struct SENSORDATA
    {
        
      struct ACC_READINGS{
        struct INSTANT{
          float x;
          float y;
          float z;
        }instant;
      }gyro;
    }sensor;

    
    

    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/

    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > GetSensorCount()) {value->sensor_type.push_back(0); return; }
      value->sensor_type.push_back(SENSOR_TYPE_GYROSCOPE_ID);
      value->data_f.push_back(sensor.gyro.instant.x);
      value->data_f.push_back(sensor.gyro.instant.y);
      value->data_f.push_back(sensor.gyro.instant.z);
      value->sensor_id = index;
    };
        
    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
      
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT 
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mSensorsL3G>*> telemetry_list;
    struct telemetry_handler<mSensorsL3G> telemetry_settings;
    struct telemetry_handler<mSensorsL3G> telemetry_sensor_ifchanged;
    struct telemetry_handler<mSensorsL3G> telemetry_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT

};
#endif

#endif
