#ifndef _MODULE__SENSORS_BATTERY_MODEM__H
#define _MODULE__SENSORS_BATTERY_MODEM__H

#define D_UNIQUE_MODULE__SENSORS_BATTERY_MODEM__ID   ((5*1000)+52)  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BATTERY_MODEM

#include "1_TaskerManager/mTaskerInterface.h"

class mBattery_Modem :
  public mTaskerInterface
{
  public:
	  mBattery_Modem(){};
    void Pre_Init(void);
    void Init(void);
    
    static constexpr const char* PM_MODULE__SENSORS_BATTERY_MODEM__CTR = D_MODULE__SENSORS_BATTERY_MODEM__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__SENSORS_BATTERY_MODEM__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__SENSORS_BATTERY_MODEM__ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mBattery_Modem);
    };
    #endif

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t sensor_count = 0;
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

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);



    
    
    uint8_t GetSensorCount(void) override
    {
      return settings.sensor_count;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > settings.sensor_count-1) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_VOLTAGE_ID);
      value->data_f.push_back(readings.battery.volts_mv);
      value->sensor_id = index;
    };
    
        
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
      
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender();
    struct handler<mBattery_Modem> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mBattery_Modem> mqtthandler_sensor_ifchanged;
    struct handler<mBattery_Modem> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    
    //with extra handlers example
    
    struct handler<mBattery_Modem>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
