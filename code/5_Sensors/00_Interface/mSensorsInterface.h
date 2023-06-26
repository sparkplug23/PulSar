#ifndef _MODULE_SENSORS_INTERFACE_H
#define _MODULE_SENSORS_INTERFACE_H

#define D_UNIQUE_MODULE_SENSORS_INTERFACE_ID   ((5*1000)+00)  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_SENSORS_INTERFACE_ID    0    // Numerical accesending order of module within a group

#include "stdint.h"

#include "5_Sensors/00_Interface/mSensorType.h"

typedef struct event_motion_s{
  uint8_t  state     = false;
  uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  uint32_t tDetectTime;
  uint32_t tEndedTime;
  uint32_t detected_time;
  struct devove_name_t{
    uint16_t unique_module_id = 0;  // unique, not "em_##_id"
    uint8_t device_id = 0;
  }device_name;
}event_motion_t;

typedef struct sensorset_location_s
{   
  bool isvalid = false;
  float latitude = 54.5;// Latitude
  float longitude = -6.0;// Longitude
  float speed = 0;// Speed Over Ground. Unit is knots.
  float altitude = 0;// MSL Altitude. Unit is meters
  int vsat = 0;// GNSS Satellites in View
  int usat = 0;// GNSS Satellites Used
  float accuracy = 0;// Horizontal Dilution Of Precision
  int year = 0; // Four digit year
  int month = 0;// Two digit month
  int day = 0;// Two digit day
  int hour = 0;// Two digit hour
  int minute = 0;// Two digit minute
  int second = 0;// 6 digit second with subseconds
}sensorset_location_t;

#include "1_TaskerManager/mTaskerManager.h"

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_TEMPERATURE_COLOURS__CTR)     "sensors/colours";
DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR)                 "unified";
  

#ifdef USE_MODULE_SENSORS_INTERFACE

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_EVENT_CTR) "motion_event";

class mSensorsInterface :
  public mTaskerInterface
{
  public:
	  mSensorsInterface(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();

    
    static const char* PM_MODULE_SENSORS_INTERFACE_CTR;
    static const char* PM_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_INTERFACE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_INTERFACE_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mSensorsInterface);    };
    #endif

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS
    {
      uint8_t fEnableSensor = false;
      uint8_t tTicker_Splash_Sensors_To_Logs = 30;
      float sealevel_pressure; 
    }
    settings;


    void MQTT_Report_Event_Button();

    const char* GetUnifiedSensor_NameByTypeID(uint8_t id);

    #ifdef USE_MODULE_LIGHTS_INTERFACE
    uint8_t flag_unified_sensor_colour_heatmap_type = 0;
    RgbcctColor GetColourValueUsingMaps_ForUnifiedSensor(float temperature);
    #endif // USE_MODULE_LIGHTS_INTERFACE
        
    void CommandEvent_Motion(uint8_t event_type);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_SensorTemperatureColours(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Motion_Event(uint8_t json_level = 0, bool json_appending = true);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mSensorsInterface> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mSensorsInterface> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsInterface> mqtthandler_sensor_teleperiod;
    struct handler<mSensorsInterface> mqtthandler_sensor_temperature_colours;
    struct handler<mSensorsInterface> mqtthandler_motion_event_ifchanged;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MOTION_EVENT_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
    
    struct handler<mSensorsInterface>* mqtthandler_list[5] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod,
      &mqtthandler_sensor_temperature_colours,
      &mqtthandler_motion_event_ifchanged
    };
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif
