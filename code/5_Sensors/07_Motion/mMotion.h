#ifndef _MODULE_SENSORS_MOTION_H
#define _MODULE_SENSORS_MOTION_H

#define D_UNIQUE_MODULE_SENSORS_MOTION_ID   ((5*1000)+07)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_MOTION

#include "2_CoreSystem/02_Time/mTime.h"

#include "5_Sensors/00_Interface/mSensorsInterface.h"

// #include "2_CoreSystem/"

#include "1_TaskerManager/mTaskerManager.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mMotion :
  public mTaskerInterface
{
  public:
	  mMotion(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_SENSORS_MOTION_CTR;
    static const char* PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_MOTION_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_MOTION_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mMotion);
    };
    #endif

    struct SETTINGS{
      uint8_t sensors_active = 0;
      uint8_t fEnableSensor = false;
      // uint8_t motion_trigger_type = MOTION_TRIGGER_TYPE_REPORT_ONLY_ID;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    
    void RulesEvent_Motion_Change();

    void parse_JSONCommand(JsonParserObject obj);
    
    #define MAXIMUM_SENSORS 3
    event_motion_t pir_detect[MAXIMUM_SENSORS]; // up to 2 sensors

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    void Rules_Add_Rule();

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender();
    struct handler<mMotion> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mMotion> mqtthandler_sensor_ifchanged;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    
    //with extra handlers example
    #endif // USE_MODULE_NETWORK_MQTT
    
    struct handler<mMotion>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged
    };

};

#endif

#endif
