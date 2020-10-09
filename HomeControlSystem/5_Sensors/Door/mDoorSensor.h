#ifndef _MDOORSENSOR_H
#define _MDOORSENSOR_H 0.2

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_DOOR

#include "1_TaskerManager/mInterfaceController.h"


class mDoorSensor{
  public:
	  mDoorSensor(){};

    // Reed switch active high (low when magnet is near)
    // #define DOOROPEN() digitalRead(REED_SWITCH_PIN) //opened when high
    // #define REED_SWITCH_INIT() pinMode(REED_SWITCH_PIN,INPUT_PULLUP)
    // #define DOOROPENCTR DOOROPEN() ? "Open" : "Closed"

    void init(void);
    void pre_init(void);

    int8_t pin_open = -1;
    int8_t pin_lock = -1;

    uint8_t     IsDoorOpen();
    const char* IsDoorOpen_Ctr();

    struct DOOR_DETECT{
      uint8_t state = false;
      uint8_t isactive = false;
      uint8_t ischanged = false;
      struct datetime changedtime;
      uint32_t tSaved;
      uint32_t tDetectTimeforDebounce;
      char friendly_name_ctr[15];
      char detected_rtc_ctr[9];
      uint32_t tDetectTime;
      uint32_t tEndedTime;
    }door_detect;

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

    int8_t Tasker(uint8_t function);

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
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mDoorSensor>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mDoorSensor> mqtthandler_settings_teleperiod;
    // const char* postfix_topic_sensors = "sensors";
    struct handler<mDoorSensor> mqtthandler_sensor_ifchanged;
    struct handler<mDoorSensor> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

};

#endif

#endif
