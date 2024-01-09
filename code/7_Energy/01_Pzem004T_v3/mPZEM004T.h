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
#ifndef _MODULE_POWERMETER_H
#define _MODULE_POWERMETER_H 0.1

#define D_UNIQUE_MODULE_ENERGY_PZEM004T_ID    132
#define D_GROUP_MODULE_ENERGY_PZEM004T_ID      2

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

#define ENERGY_PZEM004T_MEASURE_RATE_MS_TIMEOUT 900

// #include "1_TaskerManager/mTaskerManager.h"
// http://innovatorsguru.com/wp-content/uploads/2019/06/PZEM-004T-V3.0-Datasheet-User-Manual.pdf

class mEnergyPZEM004T :
  public mTaskerInterface
{
  public:
	  mEnergyPZEM004T(){};
    void Pre_Init(void);
    void Init(void);
    void Post_Init();
    void Settings_Default();

    static const char* PM_MODULE_ENERGY_PZEM004T_CTR;
    static const char* PM_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_ENERGY_PZEM004T_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_ENERGY_PZEM004T_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mEnergyPZEM004T);
    };
    #endif

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t found = 0;
      uint16_t rate_measure_ms = 1000;
      // uint8_t sensor_count = 1;
      uint8_t active_sensor = 0;
    }settings;

    void EveryLoop();    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void AllocateDynamicMemory();
    
    #ifndef MAX_ENERGY_SENSORS
      #define MAX_ENERGY_SENSORS 12
    #endif

    enum TRANSCEIVE_MODE_IDS{
      TRANSCEIVE_RESPONSE_TIMEOUT_ID=-2,
      TRANSCEIVE_RESPONSE_ERROR_ID=-1,
      TRANSCEIVE_IDLE_ID=0,
      TRANSCEIVE_REQUEST_READING_ID,
      TRANSCEIVE_AWAITING_RESPONSE_ID,
      TRANSCEIVE_RESPONSE_SUCCESS_ID,
      TRANSCEIVE_AWAITING_LENGTH_ID,
    };
    TRANSCEIVE_MODE_IDS transceive_mode = TRANSCEIVE_REQUEST_READING_ID;

    TasmotaModbus *modbus;
    timereached_t measure_time;
    uint32_t timeout = millis();
    uint32_t tSaved_backoff = millis();
    
    struct DATA_MODBUS{
      float voltage;
      float current;
      float active_power;
      float frequency;
      float power_factor;
      float energy;  
      uint32_t measured_time = 0;
    };

    struct STATS
    {
      uint16_t success_reads = 0;
      uint16_t timeout_reads = 0;
      uint32_t sample_time = 0;
      uint32_t start_time = 0;
      uint32_t end_time = 0;
    }stats;

    DATA_MODBUS* data_modbus = nullptr;

    void ParseModbusBuffer(DATA_MODBUS* mod, uint8_t* buffer);

    void SplitTask_UpdateSensor(uint8_t device_id);
    void parse_JSONCommand(JsonParserObject obj);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mEnergyPZEM004T> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mEnergyPZEM004T> mqtthandler_sensor_ifchanged;
    struct handler<mEnergyPZEM004T> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mEnergyPZEM004T>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    
  //#endif


};

#endif

#endif
