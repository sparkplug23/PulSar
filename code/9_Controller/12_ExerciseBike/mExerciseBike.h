#ifndef _mExerciseBike_H
#define _mExerciseBike_H 0.1

#include "2_CoreSystem/mBaseConfig.h"

#ifdef USE_MODULE_CONTROLLER_EXERCISE_BIKE //move into lights? pir activated light, equally a driver of lights..but it senses
// make light activation method of motion

#include <ArduinoJson.h>
// #include "1_TaskerManager/mTaskerManager.h"
// 


#include "2_CoreSystem/02_Time/mTime.h"

class mExerciseBike{
  public:
    mExerciseBike(){};

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);

    uint32_t tSavedTick = millis(),tSavedForceUpdate;
    uint8_t toggle_bit = 0;
    uint32_t tSavedSeconds = millis();
   
    // struct LIGHT_STATUS{
    //   uint8_t onoff = false;
    //   uint8_t ischanged = false;
    //   struct datetime ontime;
    //   struct datetime offtime;
    // }light_status[2];

    enum LIGHT_ID{LIGHT_DRIVEWAY_ID=0,LIGHT_GARDEN_ID,LIGHT_ALL_ID};
    enum STATE_ID{OFF=0,ON=1,TIMED_ON=2};
    void SetLight(uint8_t light_id, uint8_t state);

    struct LIGHT_CONTROL{
      int16_t seconds_on = 0;
      uint8_t ischanged = false;
      // For automatic on board triggering without wifi
      uint8_t fEnableAutomaticLight = true;
      struct datetime enabled_starttime;
      struct datetime enabled_endtime;
    };
    struct LIGHT_CONTROL light_control_driveway;
    struct LIGHT_CONTROL light_control_garden;
    struct LIGHT_CONTROL* light_control_ptr;
    
    // struct PIR_DETECT_COPY{
    //   uint8_t isactive = false;
    //   uint8_t state = false;
    //   uint8_t ischanged = false;
    //   uint32_t tDetectTime;
    // }pir_detect_copy;

    // pir_detect_t pir_detect_copy;
    // struct pir_detect pir_detect_copy;

    void parse_JSONCommand();



    void SubTask_Light();


uint8_t ConstructJSON_Settings(uint8_t json_level, bool json_appending);
uint8_t ConstructJSON_Sensor(uint8_t json_level);
uint8_t ConstructJSON_LightStates(uint8_t json_level);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    struct handler<mExerciseBike>* ptr;
    void MQTTHandler_Sender();

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mExerciseBike> mqtthandler_settings_teleperiod;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mExerciseBike> mqtthandler_sensor_ifchanged;
    struct handler<mExerciseBike> mqtthandler_sensor_teleperiod;
    
    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_MODULE_LIGHTSTATE_IFCHANGED_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_LIGHTSTATE_TELEPERIOD_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };

    // const char* postfix_topic_lightstate = "state";
    struct handler<mExerciseBike> mqtthandler_lightstate_ifchanged;
    struct handler<mExerciseBike> mqtthandler_lightstate_teleperiod;
  //#endif



};
#endif

#endif
