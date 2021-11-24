// #ifndef _MMOTIONSENSOR_H
// #define _MMOTIONSENSOR_H 1.0

// #define D_UNIQUE_MODULE_SENSORS_MOTION_ID 131

// #include "1_TaskerManager/mTaskerManager.h"

// #ifdef USE_MODULE_SENSORS_MOTION

// #include "2_CoreSystem/Time/mTime.h"

// typedef struct pir_detect_s{
//   uint8_t  state     = false;
//   uint8_t  isactive  = false;
//   uint8_t  ischanged = false;
//   uint32_t tDetectTime;
//   uint32_t tEndedTime;
//   time_short_t detected_time;
// }pir_detect_t;

// #include "5_Sensors/_Interface/mSensorsInterface.h"

// #include "1_TaskerManager/mTaskerManager.h"

// #include "1_TaskerManager/mTaskerInterface.h"


// class mMotionSensor :
//   public mTaskerInterface
// {
//   public:
//     // Constructor
//     mMotionSensor(){};
//     // Init the class
//     void Init(void);
//     void Pre_Init(void);
//     // Tasker that is called on each loop
//     int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
//     // All SubTasks called by Tasker 
//     void EveryLoop();
        
//     static const char* PM_MODULE_SENSORS_MOTION_CTR;
//     static const char* PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR;
//     PGM_P GetModuleName(){          return PM_MODULE_SENSORS_MOTION_CTR; }
//     PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR; }
//     uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_MOTION_ID; }

//     #ifdef USE_DEBUG_CLASS_SIZE
//     uint16_t GetClassSize(){
//       return sizeof(mMotionSensor);
//     };
//     #endif

//     void parse_JSONCommand(JsonParserObject obj);


//     // enum MOTION_TRIGGER_TYPE_IDS{
//     //   MOTION_TRIGGER_TYPE_REPORT_ONLY_ID=0,
//     //   MOTION_TRIGGER_TYPE_COMMANDS_INTERNAL_POWER_COMMAND_ID,
//     //   MOTION_TRIGGER_TYPE_LENGTH_ID
//     // };

//     struct SETTINGS{
//       uint8_t sensors_active = 0;
//       uint8_t fEnableSensor = false;
//       // uint8_t motion_trigger_type = MOTION_TRIGGER_TYPE_REPORT_ONLY_ID;
//     }settings;
    
//     #define MAXIMUM_SENSORS 3
//     int8_t pin[MAXIMUM_SENSORS] = {-1, -1, -1};
//     uint8_t pin_isinverted[MAXIMUM_SENSORS] = {0, 0, 0};
//     // pir_detect_t pir_detect[MAXIMUM_SENSORS]; // up to 2 sensors
//     event_motion_t pir_detect[MAXIMUM_SENSORS]; // up to 2 sensors
//     // struct event_motion_s pir_detect2;//[MAXIMUM_SENSORS]; // up to 2 sensors
    
//     #ifndef DISABLE_WEBSERVER
//     void WebPage_Root_AddHandlers();
//     void WebAppend_Root_Status_Table();
//     void WebAppend_Root_Draw_PageTable();
//     #endif // DISABLE_WEBSERVER

//     uint8_t PIR_Detected(uint8_t sensor_id);

//     const char* PIR_Detected_Ctr(uint8_t sensor_id, char* buffer, uint8_t buflen);

//     uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
//     uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
//   //#ifdef USE_CORE_MQTT 

//     void MQTTHandler_Init();
//     void MQTTHandler_Set_RefreshAll();
//     void MQTTHandler_Set_TelePeriod();
//     void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
//     struct handler<mMotionSensor> mqtthandler_settings_teleperiod;
//     struct handler<mMotionSensor> mqtthandler_sensor_ifchanged;
//     const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
//   //#endif

// };
// #endif

// #endif
