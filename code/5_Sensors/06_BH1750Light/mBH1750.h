#ifndef _MODULE_SENSORS_BH1750_H
#define _MODULE_SENSORS_BH1750_H

#define D_UNIQUE_MODULE_SENSORS_BH1750_ID   5006   // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BH1750


#define D_SENSOR_BH1750_MAX_COUNT 2

#define BH1750_ADDR1                     0x23
#define BH1750_ADDR2                     0x5C

#define BH1750_CONTINUOUS_HIGH_RES_MODE2 0x11  // Start measurement at 0.5 lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_HIGH_RES_MODE  0x10  // Start measurement at 1   lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_LOW_RES_MODE   0x13  // Start measurement at 4   lx resolution. Measurement time is approx 16ms.

#define BH1750_MEASUREMENT_TIME_HIGH     0x40  // Measurement Time register high 3 bits
#define BH1750_MEASUREMENT_TIME_LOW      0x60  // Measurement Time register low 5 bits

#include "1_TaskerManager/mTaskerInterface.h"

class mBH1750 :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mBH1750(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_SENSORS_BH1750_CTR = D_MODULE_SENSORS_BH1750_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BH1750_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BH1750_ID; }
    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/
    
    uint8_t i2c_addresses[2] = { BH1750_ADDR1, BH1750_ADDR2 };
    uint8_t i2c_resolution_register_value[3] = { BH1750_CONTINUOUS_HIGH_RES_MODE, BH1750_CONTINUOUS_HIGH_RES_MODE2, BH1750_CONTINUOUS_LOW_RES_MODE };

    struct {
      uint8_t address;
      uint8_t resolution_mode = 0; // index of the option, not the hex command
      uint8_t valid = 0;
      uint8_t mtreg = 69;                          // Default Measurement Time
      float illuminance = 0;
      uint16_t level = 0;
    } device_data[D_SENSOR_BH1750_MAX_COUNT];
    
    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/

    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > D_SENSOR_BH1750_MAX_COUNT-1) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_LIGHT_LEVEL_ID);
      value->data_f.push_back(device_data[index].level);
      value->sensor_type.push_back(SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID);
      value->data_f.push_back(device_data[index].illuminance);
      value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
        
    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    bool Set_Resolution_Mode(uint8_t sensor_id, uint8_t resolution_mode_id);
    uint8_t Get_Resolution_Mode(uint8_t sensor_id);
    uint8_t Get_Resolution_RegisterValue(uint8_t mode);
    bool Set_MeasurementTimeRegister(uint8_t sensor_id, uint8_t mtreg);
    bool Get_SensorReading(uint8_t sensor_id);
    void ReadSensor(void);

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
    void MQTTHandler_Init();
    std::vector<struct handler<mBH1750>*> mqtthandler_list;
    struct handler<mBH1750> mqtthandler_settings;
    struct handler<mBH1750> mqtthandler_sensor_ifchanged;
    struct handler<mBH1750> mqtthandler_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
