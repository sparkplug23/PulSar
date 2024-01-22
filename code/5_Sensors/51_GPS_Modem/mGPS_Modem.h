#ifndef _MODULE__SENSORS_GPS_MODEM__H
#define _MODULE__SENSORS_GPS_MODEM__H

#define D_UNIQUE_MODULE__SENSORS_GPS_MODEM__ID   ((5*1000)+51)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_GPS_MODEM

#include "1_TaskerManager/mTaskerInterface.h"

class mGPS_Modem :
  public mTaskerInterface
{
  public:
	  mGPS_Modem(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE__SENSORS_GPS_MODEM__CTR;
    static const char* PM_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE__SENSORS_GPS_MODEM__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE__SENSORS_GPS_MODEM__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__SENSORS_GPS_MODEM__ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mGPS_Modem);    };
    #endif

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t sensor_count = 0;
    }settings;

    struct READINGS{
      uint32_t update_seconds = 0;
    }readings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    sensorset_location_t location;

    
    
    uint8_t GetSensorCount(void) override
    {
      return settings.sensor_count;
    }    
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > settings.sensor_count-1) {value->sensor_type.push_back(0); return ;}
      value->sensor_type.push_back(SENSOR_TYPE_LATITUDE_ID);
      value->data_f.push_back(location.latitude);
      value->sensor_type.push_back(SENSOR_TYPE_LONGITUDE_ID);
      value->data_f.push_back(location.longitude);
      value->sensor_type.push_back(SENSOR_TYPE_SPEED_ID);
      value->data_f.push_back(location.speed);
      value->sensor_type.push_back(SENSOR_TYPE_ALTITUDE_ID);
      value->data_f.push_back(location.altitude);
      value->sensor_id = index;
    };
    
        
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
      
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mGPS_Modem> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mGPS_Modem> mqtthandler_sensor_ifchanged;
    struct handler<mGPS_Modem> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    std::vector<struct handler<mGPS_Modem>*> mqtthandler_list;
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
