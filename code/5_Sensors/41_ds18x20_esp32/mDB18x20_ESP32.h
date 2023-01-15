#ifndef _mDB18x20_ESP32_H
#define _mDB18x20_ESP32_H

#define D_UNIQUE_MODULE__DS18X20_ESP32_2023__ID ((5*1000)+41)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023

#include "1_TaskerManager/mTaskerInterface.h"

#include <OneWire.h>

#define MAX_DSB_PINS          2
#ifndef DS18X20_MAX_SENSORS
  #define DS18X20_MAX_SENSORS 12
#endif

#define DS18S20_CHIPID       0x10  // +/-0.5C 9-bit
#define DS1822_CHIPID        0x22  // +/-2C 12-bit
#define DS18B20_CHIPID       0x28  // +/-0.5C 12-bit
#define MAX31850_CHIPID      0x3B  // +/-0.25C 14-bit

#define W1_SKIP_ROM          0xCC
#define W1_CONVERT_TEMP      0x44
#define W1_READ_SCRATCHPAD   0xBE


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
    uint16_t GetClassSize(){
      return sizeof(mDB18x20_ESP32);
    };
    #endif

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // uint8_t fSensorCount = 0;
    }settings;

    void EveryLoop();
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    void SubTasker_MQTTSender();
    
    void SetDeviceNameID_WithAddress(const char* device_name, uint8_t device_name_index, uint8_t* array_val, uint8_t array_len);

    struct 
    { 
      uint8_t address[8] = {0};
      uint8_t index;
      uint8_t valid;
      int8_t pins_id;
      // uint8_t alias_index;
      int8_t device_name_index = -1; // -1 for unknown
      // uint8_t address_stored[8];
      int8_t sensor_group_id = -1; // which pin it comes from 
      /**
       * @brief method is not right, I need to align this with devicename ids, so sensor[id] where id matches devicename should be configured
       * */
      int8_t address_id = -1; //set this manually with template, else, as 0 (check for any -1, set incremented and name "sens01")
      struct READING{
        float val;
        uint8_t isvalid;
        uint8_t ischanged;
        uint32_t captureupsecs; //sensor age
      }reading;
    } sensor_new[DS18X20_MAX_SENSORS];

    uint8_t devices_pin1 = 0;
    uint8_t devices_pin2 = 0;



    void Scan_ReportAsJsonBuilder();

    void parse_JSONCommand(JsonParserObject obj);

    #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return DS18X20Data.sensors;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > DS18X20_MAX_SENSORS-1) {value->type.push_back(0); return ;}
      value->type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data.push_back(sensor_new[index].reading.val);
      value->sensor_id = index;
      value->resolution = 123;
    };
    #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING


    OneWire *ds = nullptr;
    OneWire *ds18x20_gpios[MAX_DSB_PINS];

    struct {
      char name[17];
      uint8_t sensors;
      uint8_t gpios;    // Count of GPIO found
    } DS18X20Data;





    void Ds18x20Init(void);
    void Ds18x20Search(void);
    void Ds18x20Convert(void);
    bool Ds18x20Read(uint8_t sensor, float &t);
    void Ds18x20Name(uint8_t sensor);
    void Ds18x20EverySecond(void);
    void Ds18x20Show(bool json);


    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_object_start_end_required = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mDB18x20_ESP32>* ptr;
    struct handler<mDB18x20_ESP32> mqtthandler_settings_teleperiod;
    struct handler<mDB18x20_ESP32> mqtthandler_sensor_ifchanged;
    struct handler<mDB18x20_ESP32> mqtthandler_sensor_teleperiod;

    #ifdef ENABLE_DEBUG_MQTT_CHANNEL_DB18X20
    struct handler<mDB18x20_ESP32> mqtthandler_debug;
    uint8_t ConstructJSON_Debug(uint8_t json_level = 0, bool json_object_start_end_required = true);
    #endif

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
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
