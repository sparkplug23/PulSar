#ifndef _MMOTIONSENSOR_H
#define _MMOTIONSENSOR_H 1.0

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_MOTION

#include "2_CoreSystem/Time/mTime.h"

typedef struct pir_detect_s{
  uint8_t  state     = false;
  uint8_t  isactive  = false;
  uint8_t  ischanged = false;
  uint32_t tDetectTime;
  uint32_t tEndedTime;
  time_short_t detected_time;
}pir_detect_t;

#include "1_TaskerManager/mTaskerManager.h"

class mMotionSensor{
  public:
    // Constructor
    mMotionSensor(){};
    // Init the class
    void Init(void);
    void Pre_Init(void);
    // Tasker that is called on each loop
    int8_t Tasker(uint8_t function);
    // All SubTasks called by Tasker 
    void EveryLoop();

    enum MOTION_TRIGGER_TYPE_IDS{
      MOTION_TRIGGER_TYPE_REPORT_ONLY_ID=0,
      MOTION_TRIGGER_TYPE_COMMANDS_INTERNAL_POWER_COMMAND_ID,
      MOTION_TRIGGER_TYPE_LENGTH_ID
    };

    struct SETTINGS{
      uint8_t sensors_active = 0;
      uint8_t fEnableSensor = false;
      uint8_t motion_trigger_type = MOTION_TRIGGER_TYPE_REPORT_ONLY_ID;
    }settings;

    void parse_JSONCommand();
    int8_t CheckAndExecute_JSONCommands();
    
    #define MAXIMUM_SENSORS 3
    int8_t pin[MAXIMUM_SENSORS] = {-1, -1, -1};
    uint8_t pin_isinverted[MAXIMUM_SENSORS] = {0, 0, 0};
    pir_detect_t pir_detect[MAXIMUM_SENSORS]; // up to 2 sensors
    
    #ifndef DISABLE_WEBSERVER
    void WebPage_Root_AddHandlers();
    void WebAppend_Root_Status_Table();
    void WebAppend_Root_Draw_PageTable();
    #endif // DISABLE_WEBSERVER

    uint8_t PIR_Detected(uint8_t sensor_id);

    const char* PIR_Detected_Ctr(uint8_t sensor_id, char* buffer, uint8_t buflen);

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mMotionSensor> mqtthandler_settings_teleperiod;
    struct handler<mMotionSensor> mqtthandler_sensor_ifchanged;
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

};
#endif

#endif
