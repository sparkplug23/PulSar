#ifndef _mESP32Temperature_H
#define _mESP32Temperature_H

#define D_UNIQUE_MODULE_SENSORS_ESP32_TEMPERATURE__ID 5032 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_ESP32_TEMPERATURE

#include <Arduino.h>

#define D_SENSOR_VALID_TIMEOUT_SECS__ESP32_TEMP 10

/**
 * @brief Classic ESP32 internal temperature is treated as unsupported by default.
 *        Enable this only if you want to test temperatureRead() on classic ESP32 anyway.
 */
// #define ENABLE_FEATURE_INTERNAL_CHIP_TEMPERATURE__ALLOW_CLASSIC_ESP32

class mESP32Temperature :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mESP32Temperature(){};

    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(void);

    static constexpr const char* PM_MODULE_SENSORS_ESP32_TEMPERATURE__CTR = D_MODULE__SENSORS__ESP32_TEMPERATURE__CTR;
    PGM_P GetModuleName(){ return PM_MODULE_SENSORS_ESP32_TEMPERATURE__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_ESP32_TEMPERATURE__ID; }

    struct ClassState
    {
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    } module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct SENSOR_READING
    {
      float val = NAN;
      uint8_t isvalid = 0;
      uint8_t ischanged = false;
    };

    struct SENSOR_DATA
    {
      SENSOR_READING reading;
      uint32_t utc_measured_timestamp = 0;
      bool supported = false;
    } sensor;

    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/
    bool IsSupported(void);
    bool ReadTemperature(float* value_c);
    void EverySecond(void);

    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/
    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }

    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index >= module_state.devices){ value->sensor_type.push_back(0); return; }
      value->timestamp = sensor.utc_measured_timestamp;
      value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data_f.push_back(sensor.reading.val);
      value->sensor_id = 0;
      value->resolution = 0;
    };

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: ConstructJSON
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECTION: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void Telemetry_Init(void);
    std::vector<struct telemetry_handler<mESP32Temperature>*> telemetry_list;
    struct telemetry_handler<mESP32Temperature> telemetry_settings;
    struct telemetry_handler<mESP32Temperature> telemetry_sensor_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif // USE_MODULE_SENSORS_ESP32_TEMPERATURE
#endif // _mESP32Temperature_H