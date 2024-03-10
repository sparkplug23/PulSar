#ifndef _MODULE_SENSORS_BH1750_H
#define _MODULE_SENSORS_BH1750_H

#define D_UNIQUE_MODULE_SENSORS_BH1750_ID   ((5*1000)+06)  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BH1750


#define D_SENSOR_BH1750_MAX_COUNT 2

#define XSNS_10                          10
#define XI2C_11                          11    // See I2CDEVICES.md

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
	  mBH1750(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_SENSORS_BH1750_CTR;
    static const char* PM_MODULE_SENSORS_BH1750_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BH1750_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_BH1750_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BH1750_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mBH1750);
    };
    #endif

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t sensor_count = 0;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    struct {
      uint8_t addresses[2] = { BH1750_ADDR1, BH1750_ADDR2 };
      uint8_t resolution[3] = { BH1750_CONTINUOUS_HIGH_RES_MODE, BH1750_CONTINUOUS_HIGH_RES_MODE2, BH1750_CONTINUOUS_LOW_RES_MODE };
    } Bh1750;

    struct {
      uint8_t address;
      uint8_t valid = 0;
      uint8_t mtreg = 69;                          // Default Measurement Time
      float illuminance = 0;
      uint16_t level = 0;
    } device_data[D_SENSOR_BH1750_MAX_COUNT];

    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return settings.sensor_count;
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

        
    uint8_t Get_Resolution(uint32_t sensor_index);
    bool Set_Resolution(uint32_t sensor_index);
    bool Set_MeasurementTimeRegister(uint32_t sensor_index);
    bool Get_SensorReading(uint32_t sensor_index);
    void SubTask_ReadSensor(void);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
      
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender();
    struct handler<mBH1750> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mBH1750> mqtthandler_sensor_ifchanged;
    struct handler<mBH1750> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    
    //with extra handlers example
    
    struct handler<mBH1750>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
