#ifndef _MODULE_SENSORS_INTERFACE_H
#define _MODULE_SENSORS_INTERFACE_H

#define D_UNIQUE_MODULE_SENSORS_INTERFACE_ID   139  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_SENSORS_INTERFACE_ID    0    // Numerical accesending order of module within a group

#include "stdint.h"

#include "5_Sensors/_Interface/mSensorType.h"

typedef struct event_motion_s{
  uint8_t  state     = false;
  uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  uint32_t tDetectTime;
  uint32_t tEndedTime;
  uint32_t detected_time;
}event_motion_t;

#include "1_TaskerManager/mTaskerManager.h"

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_TEMPERATURE_COLOURS_CTR)     "sensors/colours";
  

#ifdef USE_MODULE_SENSORS_INTERFACE

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_EVENT_CTR) "motion_event";

class mSensorsInterface :
  public mTaskerInterface
{
  public:
	  mSensorsInterface(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_SENSORS_INTERFACE_CTR;
    static const char* PM_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_INTERFACE_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_INTERFACE_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSensorsInterface);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      // move this into settings so all devices can share it
      float sealevel_pressure; 
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();

    void MQTT_Report_Event_Button();

    // sensor_t

    
    void CommandEvent_Motion(uint8_t event_type);
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
    uint8_t ConstructJSON_SensorTemperatureColours(uint8_t json_method = 0);
    uint8_t ConstructJSON_Motion_Event(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mSensorsInterface> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mSensorsInterface> mqtthandler_sensor_ifchanged;
    struct handler<mSensorsInterface> mqtthandler_sensor_teleperiod;
    struct handler<mSensorsInterface> mqtthandler_sensor_temperature_colours;
    struct handler<mSensorsInterface> mqtthandler_motion_event_ifchanged;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    // const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers exampleenum
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MOTION_EVENT_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
    
    // uint8_t list_ids[4] = {
    //   MQTT_HANDLER_SETTINGS_ID, 
    //   MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    //   MQTT_HANDLER_SENSOR_TELEPERIOD_ID,
    //   MQTT_HANDLER_MOTION_EVENT_IFCHANGED_ID
    // };
    
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
