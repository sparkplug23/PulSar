#ifndef _MMOTIONSENSOR_H
#define _MMOTIONSENSOR_H 1.0

#include "1_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_MOTION

typedef struct pir_detect_s{
  uint8_t  state = false;
  uint8_t  isactive = false;
  uint8_t  ischanged = false;
  uint32_t tDetectTime;
  uint32_t tEndedTime;
  char     friendly_name_ctr[15];
  char     detected_rtc_ctr[10]; //HH:MM:SS\0  IMPORTANT! COPIED INTO GARAGE LIGHT, CAUSES ERROR IF CHANGED
}pir_detect_t;

#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


class mMotionSensor{
  public:
    // Constructor
    mMotionSensor(){};
    // Init the class
    void Init(void);
    void Pre_Init(void);
    // Tasker that is called on each loop
    int8_t Tasker(uint8_t function);
    int8_t Tasker(uint8_t function, JsonObjectConst obj);
    // All SubTasks called by Tasker 
    void EveryLoop();

    void parse_JSONCommand(JsonObjectConst obj);
    // void parsesub_JSONCommand(JsonObject& _obj);
int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);


    #ifndef PIR_DETECTOR_NAME_CTR
      #define PIR_DETECTOR_NAME_CTR   "HC-SR501"  //move into options list with enums
    #endif
    
    #define MAXIMUM_SENSORS 2
    int8_t pin[2] = {-1, -1};
    uint8_t pin_isinverted[2] = {0, 0};
    uint8_t sensors_active = 0;
    uint8_t fEnableSensor = false;
    pir_detect_t pir_detect[MAXIMUM_SENSORS]; // up to 2 sensors

    
    const char* GetDeviceNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);
    const char* GetDeviceNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);

    
    void WebPage_Root_AddHandlers();
    void WebAppend_Root_Status_Table();
    void WebSend_JSON_Root_Table(AsyncWebServerRequest *request);

    uint8_t PIR_Detected(uint8_t sensor_id);
    const char* PIR_Detected_Ctr(uint8_t sensor_id);

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Connected();
    void MQTTHandler_Disconnected();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mMotionSensor>* mqtthandler_ptr;
    const char* postfix_topic_settings = "settings\0";
    struct handler<mMotionSensor> mqtthandler_settings_teleperiod;
    const char* postfix_topic_sensors = "sensors\0";
    struct handler<mMotionSensor> mqtthandler_sensor_ifchanged;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

};
#endif

#endif
