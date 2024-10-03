#ifndef _mDB18x20_ESP32_H
#define _mDB18x20_ESP32_H

#define D_UNIQUE_MODULE__DS18X20_ESP32_2023__ID 5041 // [(Folder_Number*100)+ID_File]

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
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mDB18x20_ESP32(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage();
    
    static constexpr const char* PM_MODULE_SENSORS__DS18X20_ESP32_2023__CTR = D_MODULE_SENSORS_DB18S20_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS__DS18X20_ESP32_2023__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__DS18X20_ESP32_2023__ID; }
    

    struct ClassState
    {
      uint8_t devices = 0; // How many devices are being managed, physical or virtual
      uint8_t pins_used = 0;   // How many physical pins are being used
      uint8_t mode = ModuleStatus::Initialising; // Disabled, Initialise, Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot

    }rt;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
      
    // DS18S20: Fixed 9-bit resolution (1/2°C). Measures temperature in 750 mS.
    // DS18B20: Resolution of 9 to 12 bits (12-1/16°C).
    static constexpr const char* kDs18x20Types = "DS18x20|DS18S20|DS1822|DS18B20|MAX31850";
    static constexpr uint8_t ds18x20_chipids[] = { 0, DS18S20_CHIPID, DS1822_CHIPID, DS18B20_CHIPID, MAX31850_CHIPID };


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
      }reading;
      uint32_t utc_measured_timestamp = 0;
    }
    sensors_t;
    std::vector<SENS> sensor_vector; // Using vector for dynamic memory

    OneWire *ds = nullptr;
    OneWire *ds18x20_gpios[MAX_DSB_PINS];
    void SetDeviceNameID_WithAddress(const char* device_name, uint8_t device_name_index, uint8_t* array_val, uint8_t array_len);
    void Scan_ReportAsJsonBuilder();
    void Ds18x20Init(void);
    void Ds18x20Search(void);
    void Ds18x20Convert(void);
    bool Ds18x20Read(uint8_t sensor, float &t);
    void EverySecond(void);

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
      if(index > sensor_vector.size()-1) {value->sensor_type.push_back(0); return ;}
      
      value->timestamp = sensor_vector[index].utc_measured_timestamp;

      value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data_f.push_back(sensor_vector[index].reading.val);
      value->sensor_id = sensor_vector[index].device_name_index;
      value->resolution = sensor_vector[index].resolution;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

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
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();
    
    std::vector<struct handler<mDB18x20_ESP32>*> mqtthandler_list;
    struct handler<mDB18x20_ESP32> mqtthandler_settings;
    struct handler<mDB18x20_ESP32> mqtthandler_sensor_ifchanged;
    struct handler<mDB18x20_ESP32> mqtthandler_sensor_teleperiod;

      #ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
      struct handler<mDB18x20_ESP32> mqtthandler_debug;
      uint8_t ConstructJSON_Debug(uint8_t json_level = 0, bool json_appending = true);
      #endif
    
    #endif // USE_MODULE_NETWORK_MQTT

};


#endif

#endif
