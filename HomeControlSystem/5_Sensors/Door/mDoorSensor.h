#ifndef _MDOORSENSOR_H
#define _MDOORSENSOR_H 0.2

#define D_UNIQUE_MODULE_SENSORS_DOOR_ID 130

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_DOOR

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
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_DOOR_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mDoorSensor);
    };
    #endif

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    uint8_t     IsDoorOpen();
    const char* IsDoorOpen_Ctr(char* buffer, uint8_t buflen);
    
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

    #ifdef DOORLOCK_SWITCH_PIN
      #define LOCKOPEN() digitalRead(DOORLOCK_SWITCH_PIN) //opened when high
      #define DOORLOCK_SWITCH_INIT() pinMode(DOORLOCK_SWITCH_PIN,INPUT_PULLUP)
      #define LOCKOPENCTR LOCKOPEN() ? "Unlocked" : "Locked"
      struct LOCK_DETECT{
        uint8_t state = false;
        uint8_t isactive = false;
        uint8_t ischanged = false;
        struct datetime changedtime; //time_Short
        uint32_t tSaved;
      }lock_detect;
      void MQTTSendDoorLockIfChanged();
    #endif

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    //#ifdef MQTT
      void SubTasker_MQTTSender();
      // void MQTTSendDoorSensorIfChanged();
      void MQQTSendDoorUpdate();
      uint32_t tSavedSendDoorSensor;
      uint8_t fUpdateSendDoorSensor;
    //#endif
    

void WebAppend_Root_Status_Table_Draw();
void WebAppend_Root_Status_Table_Data();
  

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mDoorSensor>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mDoorSensor> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mDoorSensor> mqtthandler_sensor_ifchanged;
    struct handler<mDoorSensor> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    
  struct handler<mDoorSensor>* mqtthandler_list[3] = {
    &mqtthandler_settings_teleperiod, &mqtthandler_sensor_ifchanged, &mqtthandler_sensor_teleperiod
  };
    
  //#endif

};

#endif

#endif
