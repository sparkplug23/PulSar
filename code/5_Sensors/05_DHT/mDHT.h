#ifndef MSENSORSDHT_H
#define MSENSORSDHT_H 0.2

#define D_UNIQUE_MODULE_SENSORS_DHT_ID 5005 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_DHT

#include "DHTesp.h"
class DHTesp;

#include "1_TaskerManager/mTaskerInterface.h"

#define MAX_DHT_SENSORS 4
#define MAX_DHT_SENSORS_PER_MODEL 2

class mSensorsDHT :
  public mTaskerInterface
{
  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSensorsDHT(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_SENSORS_DHT_CTR = D_MODULE_SENSORS_DHT_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_DHT_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_DHT_ID; }

    struct ClassState{
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    static const uint32_t DHT_POLL_PERIOD_MS   = 2000;
    static const uint32_t DHT_BACKOFF_MIN_MS   = 3000;
    static const uint32_t DHT_BACKOFF_MAX_MS   = 60000;
    static const uint32_t DHT_RESCAN_PERIOD_MS = 15000;

    struct DHT_Sensor {
      // hardware / driver
      DHTesp* dht = nullptr;
      uint16_t gpio_base = 0;
      uint8_t gpio_index = 0;
      int16_t pin = -1;

      // latest values
      float temperature = NAN;
      float humidity    = NAN;
      float heatIndex   = NAN;
      float dewPoint    = NAN;
      float cr          = NAN;

      // flags
      uint8_t isvalid = 0;
      uint8_t ischanged = 0;

      // scheduling / health
      uint32_t next_poll_ms   = 0;
      uint32_t backoff_ms     = DHT_BACKOFF_MIN_MS;
      uint32_t last_ok_ms     = 0;
      uint32_t last_change_ms = 0;
    };

    // one element per configured sensor
    DHT_Sensor s[MAX_DHT_SENSORS];

    uint32_t next_rescan_ms = 0;
    
    void EveryLoop();


    void ClearSensors(void);
    bool AddSensor(uint16_t gpio_base, uint8_t index, DHTesp::DHT_MODEL_t model, const char* tag);
    bool PollOne(uint8_t sensor_id);

    /************************************************************************************************
     * SECTION: Unified Reporting
     ************************************************************************************************/
    uint8_t GetSensorCount(void) override
    {
      return module_state.devices;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index >= module_state.devices) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_TEMPERATURE_ID);
      value->data_f.push_back(s[index].temperature);
      value->sensor_type.push_back(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
      value->data_f.push_back(s[index].humidity);
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
    std::vector<struct handler<mSensorsDHT>*> mqtthandler_list;    
    struct handler<mSensorsDHT> mqtthandler_settings;
    struct handler<mSensorsDHT> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsDHT> mqtthandler_sensor_teleperiod;
    #endif

};
#endif

#endif
