#ifndef _MODULE__SENSORS_BATTERY_MODEM__H
#define _MODULE__SENSORS_BATTERY_MODEM__H

#define D_UNIQUE_MODULE__SENSORS_BATTERY_MODEM__ID   5052 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BATTERY_MODEM

#include "1_TaskerManager/mTaskerInterface.h"

class mBattery_Modem :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mBattery_Modem(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE__SENSORS_BATTERY_MODEM__CTR = D_MODULE__SENSORS_BATTERY_MODEM__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__SENSORS_BATTERY_MODEM__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__SENSORS_BATTERY_MODEM__ID; }

    struct ClassState{
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/


    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    struct READINGS{
      uint32_t update_seconds = 0;
      // uint16_t battery_millivolts = 0;
      // float battery_percentage = 0;
      //   uint8_t  charge_state = 0;
      //   bool isvalid = false;

        
      struct batt_status
      {
        uint16_t volts_mv    = 0;
        int8_t   percentage = 0;
        uint8_t  charge_state = 0;
        bool isvalid = false;
      }
      battery;


    }readings;

    
    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/
    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > GetSensorCount()-1) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_VOLTAGE_ID);
      value->data_f.push_back(readings.battery.volts_mv);
      value->sensor_id = index;
    };
    
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
    std::vector<struct handler<mBattery_Modem>*> mqtthandler_list;    
    struct handler<mBattery_Modem> mqtthandler_settings;
    struct handler<mBattery_Modem> mqtthandler_sensor_ifchanged;
    struct handler<mBattery_Modem> mqtthandler_sensor_teleperiod;
    #endif

};

#endif

#endif
