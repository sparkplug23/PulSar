#ifndef _MDOORSENSOR_H
#define _MDOORSENSOR_H 0.2

#define D_UNIQUE_MODULE_SENSORS_DOOR_ID ((5*1000)+11)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023

#include "1_TaskerManager/mTaskerManager.h"


DEFINE_PGM_CTR(PM_EVENT_DOOR_OPENED_CTR) D_JSON_OPENED;
DEFINE_PGM_CTR(PM_EVENT_DOOR_CLOSED_CTR) D_JSON_CLOSED;

#include "1_TaskerManager/mTaskerInterface.h"

class mDoorSensor :
  public mTaskerInterface
{
  public:
	  mDoorSensor(){};

    void init(void);
    void Pre_Init(void);
    
    static const char* PM_MODULE_SENSORS_DOOR_CTR;
    static const char* PM_MODULE_SENSORS_DOOR_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_DOOR_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_DOOR_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_DOOR_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mDoorSensor);
    };
    #endif

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t fSensorCount = 0;
    }settings;

    uint8_t     IsDoorOpen();
    const char* IsDoorOpen_Ctr(char* buffer, uint8_t buflen);
    
    void ShowSensor_AddLog();

    struct DOOR_DETECT{
      uint8_t state = false;
      uint8_t isactive = false;
      uint8_t wasactive = false; //last state
      uint8_t ischanged = false;
      //struct datetime changedtime;
      uint32_t tSaved;
      uint32_t tDetectTimeforDebounce;
      //char friendly_name_ctr[15];
      //char detected_rtc_ctr[9];
      time_short_t detected_time;
      uint32_t tDetectTime;
      uint32_t tEndedTime;
    }door_detect;


    void EveryLoop();

    // #ifdef DOORLOCK_SWITCH_PIN
    //   #define LOCKOPEN() digitalRead(DOORLOCK_SWITCH_PIN) //opened when high
    //   #define DOORLOCK_SWITCH_INIT() pinMode(DOORLOCK_SWITCH_PIN,INPUT_PULLUP)
    //   #define LOCKOPENCTR LOCKOPEN() ? "Unlocked" : "Locked"
      struct LOCK_DETECT{
        uint8_t state = false;
        uint8_t isactive = false;
        uint8_t ischanged = false;
        struct datetime changedtime; //time_Short
        uint32_t tSaved;
        uint32_t tDetectTimeforDebounce;
        time_short_t detected_time;
      }lock_detect;
      void MQTTSendDoorLockIfChanged();
    // #endif
    uint8_t IsLock_Locked();

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    //#ifdef MQTT
      void SubTasker_MQTTSender();
      // void MQTTSendDoorSensorIfChanged();
      void MQQTSendDoorUpdate();
      uint32_t tSavedSendDoorSensor;
      uint8_t fUpdateSendDoorSensor;
    //#endif
    
    #ifdef ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
    uint8_t GetSensorCount(void) override
    {
      return settings.fSensorCount;
    }
    void GetSensorReading(sensors_reading_t* value, uint8_t index = 0) override
    {
      if(index > 1) {value->type.push_back(0); return ;}
      value->type.push_back(SENSOR_TYPE_DOOR_POSITION_ID);
      value->data.push_back(door_detect.isactive);
      // #ifdef DOORLOCK_SWITCH_PIN
      value->type.push_back(SENSOR_TYPE_DOOR_LOCKED_ID);
      value->data.push_back(lock_detect.isactive);
      // #endif // DOORLOCK_SWITCH_PIN
      value->sensor_id = index;
    };
    #endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING
        


void WebAppend_Root_Status_Table_Draw();
void WebAppend_Root_Status_Table_Data();
  

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    
    struct handler<mDoorSensor>* ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mDoorSensor> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mDoorSensor> mqtthandler_sensor_ifchanged;
    struct handler<mDoorSensor> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    

    
  struct handler<mDoorSensor>* mqtthandler_list[3] = {
    &mqtthandler_settings_teleperiod, &mqtthandler_sensor_ifchanged, &mqtthandler_sensor_teleperiod
  };
    
  //#endif

};

#endif

#endif
