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
  
  http://innovatorsguru.com/wp-content/uploads/2019/06/PZEM-004T-V3.0-Datasheet-User-Manual.pdf

*/
#ifndef _MODULE_POWERMETER_H
#define _MODULE_POWERMETER_H

#define D_UNIQUE_MODULE_ENERGY_PZEM004T_ID 7001 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_ENERGY_PZEM004T_V3

#define REG_VOLTAGE     0x0000
#define REG_CURRENT_L   0x0001
#define REG_CURRENT_H   0X0002
#define REG_POWER_L     0x0003
#define REG_POWER_H     0x0004
#define REG_ENERGY_L    0x0005
#define REG_ENERGY_H    0x0006
#define REG_FREQUENCY   0x0007
#define REG_PF          0x0008
#define REG_ALARM       0x0009

#define CMD_RHR         0x03
#define CMD_RIR         0X04
#define CMD_WSR         0x06
#define CMD_CAL         0x41
#define CMD_REST        0x42


#define WREG_ALARM_THR   0x0001
#define WREG_ADDR        0x0002

#include <TasmotaSerial.h>
#include <TasmotaModbus.h>

#define ENERGY_PZEM004T_MEASURE_RATE_MS_TIMEOUT 200

class mEnergyPZEM004T :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mEnergyPZEM004T(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_ENERGY_PZEM004T_CTR = D_MODULE_ENERGY_PZEM004T_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_ENERGY_PZEM004T_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_ENERGY_PZEM004T_ID; }    
      

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct DATA_RUNTIME
    {
      uint8_t devices_present = 0;
      uint16_t rate_measure_ms = 1000;
      uint8_t device_current = 0;
      uint8_t waiting_address_response = 0;
      uint8_t enable_search_for_devices_seconds_delayed = 5; // count down to it to enable stable first
      uint8_t expected_device_count = 0; // 0 means ignore function, otherwise set by command
    }rt;
    timereached_t request_reading;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void EveryLoop();    
        
    #ifndef MAX_PZEM004T_DEVICES
      #define MAX_PZEM004T_DEVICES 12
    #endif

    #ifndef DEFAULT_MAX_PZEM004T_ADDRESS_FOR_SEARCH
      #define DEFAULT_MAX_PZEM004T_ADDRESS_FOR_SEARCH 20//MAX_PZEM004T_DEVICES // this needs changed out!
      // or if none is found, a slower search happens, ie every 5 seconds, another 5 devices is searched for until 
      // a practical upper limit of like 20. Any address higher than this should be enabled as option only.
    #endif
    
    void DeviceSearch(uint8_t address_limit = DEFAULT_MAX_PZEM004T_ADDRESS_FOR_SEARCH); // address 248 is reserved for broadcast
    void AddDeviceIfNotExists(uint8_t address_to_save);    
    void SetIDWithAddress(uint8_t address_id, uint8_t address_to_save);

    TasmotaModbus *modbus;
    timereached_t measure_time;
    uint32_t timeout = millis();
    uint32_t tSaved_backoff = millis();
    
    typedef struct DeviceData_t{
      float voltage;
      float current;
      float active_power;
      float frequency;
      float power_factor;
      float import_active;  
      uint8_t address = 0;
      uint32_t measured_time = 0;
    };

    std::vector<DeviceData_t> data_v;

    void ParseModbusBuffer(uint8_t* buffer, mEnergyPZEM004T::DeviceData_t* data);

    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/

    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return data_v.size();
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index >= data_v.size()) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_VOLTAGE_ID);       value->data_f.push_back(data_v[index].voltage);
      value->sensor_type.push_back(SENSOR_TYPE_CURRENT_ID);       value->data_f.push_back(data_v[index].current);
      value->sensor_type.push_back(SENSOR_TYPE_ACTIVE_POWER_ID);  value->data_f.push_back(data_v[index].active_power);
      value->sensor_type.push_back(SENSOR_TYPE_FREQUENCY_ID);     value->data_f.push_back(data_v[index].frequency);
      value->sensor_type.push_back(SENSOR_TYPE_POWER_FACTOR_ID);  value->data_f.push_back(data_v[index].power_factor);
      value->sensor_type.push_back(SENSOR_TYPE_ENERGY_ID);        value->data_f.push_back(data_v[index].import_active);
      value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void   parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    std::vector<struct handler<mEnergyPZEM004T>*> mqtthandler_list;
    struct handler<mEnergyPZEM004T> mqtthandler_settings;
    struct handler<mEnergyPZEM004T> mqtthandler_state_ifchanged;
    struct handler<mEnergyPZEM004T> mqtthandler_state_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif
