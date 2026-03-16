#ifndef _MMOISTURESENSOR_H
#define _MMOISTURESENSOR_H

#define D_UNIQUE_MODULE_SENSORS_MOTION_ID 5016 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_MOISTURE

#include "1_TaskerManager/mTaskerManager.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mMoistureSensor :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mMoistureSensor(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_SENSORS_MOISTURE_ADC_CTR = D_MODULE_SENSORS_MOISTURE_ADC_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_MOISTURE_ADC_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_MOTION_ID; } 
      

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/


    uint16_t adc_raw = 0;

    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/
    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > module_state.devices) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_MOISTURE_ID);
      value->data_f.push_back((float)adc_raw);      
      value->sensor_id = index;
    };


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
    std::vector<struct handler<mMoistureSensor>*> mqtthandler_list;
    struct handler<mMoistureSensor> mqtthandler_settings;
    struct handler<mMoistureSensor> mqtthandler_state_ifchanged;
    struct handler<mMoistureSensor> mqtthandler_state_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT

};
#endif

#endif
