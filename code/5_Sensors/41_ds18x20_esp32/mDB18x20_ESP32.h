#ifndef _mDB18x20_ESP32_H
#define _mDB18x20_ESP32_H

#define D_UNIQUE_MODULE__DS18X20_ESP32_2023__ID ((5*1000)+41)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023

#include <OneWire.h>

#define MAX_DSB_PINS          2
#ifndef DS18X20_MAX_SENSORS
  #define DS18X20_MAX_SENSORS 12
#endif
#define D_SENSOR_VALID_TIMEOUT_SECS 10

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_READ_SCRATCHPAD   0xBE

// https://www.analog.com/en/technical-articles/guidelines-for-reliable-long-line-1wire-networks.html

class mDB18x20_ESP32 :
  public mTaskerInterface
{
  public:
	  mDB18x20_ESP32(){};

    void init(void);
    void Pre_Init(void);
    
    static const char* PM_MODULE_SENSORS__DS18X20_ESP32_2023__CTR;
    static const char* PM_MODULE_SENSORS__DS18X20_ESP32_2023__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__DS18X20_ESP32_2023__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS__DS18X20_ESP32_2023__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__DS18X20_ESP32_2023__ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mDB18x20_ESP32); };
    #endif

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void parse_JSONCommand(JsonParserObject obj);

    /**
     * @brief All module generic settings
     **/
    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t sensor_count = 0;
      uint8_t pins_used;    // Count of GPIO found
    }settings;

    /**
     * @brief Data in this module
     **/
    struct SENS
    { 
      uint8_t address[8] = {0};
      uint8_t index;
      int8_t pins_id;
      uint8_t resolution = 12;
      int8_t device_name_index = -1;
      struct READING{
        float val;
        uint8_t isvalid   = D_SENSOR_VALID_TIMEOUT_SECS;
        uint8_t ischanged = false;
        uint32_t capture_time_millis = 0;
      }reading;
    }
    sensors_t;
    std::vector<SENS> sensor_vector; // Using vector for dynamic memory

    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return settings.sensor_count;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > sensor_vector.size()-1) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data_f.push_back(sensor_vector[index].reading.val);
      value->sensor_id = sensor_vector[index].device_name_index;
      value->resolution = sensor_vector[index].resolution;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

    OneWire *ds = nullptr;
    OneWire *ds18x20_gpios[MAX_DSB_PINS];
    void SetDeviceNameID_WithAddress(const char* device_name, uint8_t device_name_index, uint8_t* array_val, uint8_t array_len);
    void Scan_ReportAsJsonBuilder();
    void Ds18x20Init(void);
    void Ds18x20Search(void);
    void Ds18x20Convert(void);
    bool Ds18x20Read(uint8_t sensor, float &t);
    void EverySecond(void);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    
    struct handler<mDB18x20_ESP32>* ptr;
    struct handler<mDB18x20_ESP32> mqtthandler_settings_teleperiod;
    struct handler<mDB18x20_ESP32> mqtthandler_sensor_ifchanged;
    struct handler<mDB18x20_ESP32> mqtthandler_sensor_teleperiod;

    #ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
    struct handler<mDB18x20_ESP32> mqtthandler_debug;
    uint8_t ConstructJSON_Debug(uint8_t json_level = 0, bool json_appending = true);
    #endif

    // No specialised payload therefore use system default instead of enum
    
    
    struct handler<mDB18x20_ESP32>* mqtthandler_list[
      3
      #ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
      +1
      #endif    
      ] = 
      {
        #ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
        &mqtthandler_debug,
        #endif
        &mqtthandler_settings_teleperiod, 
        &mqtthandler_sensor_ifchanged, 
        &mqtthandler_sensor_teleperiod
    };
    
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif
