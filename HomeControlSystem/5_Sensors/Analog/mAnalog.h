#ifndef _MMOTIONSENSOR_H
#define _MMOTIONSENSOR_H 1.0

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_SENSORS_ANALOG_MEASURE

#include "1_TaskerManager/mInterfaceController.h"


class mAnalog{
  public:
    // Constructor
    mAnalog(){};
    // Init the class
    void Init(void);
    void Pre_Init(void);
    // Tasker that is called on each loop
    int8_t Tasker(uint8_t function);
    // All SubTasks called by Tasker 
    void EveryLoop();

    void parse_JSONCommand();
    void parsesub_JSONCommands(JsonObject& _obj);

    #ifndef PIR_DETECTOR_NAME_CTR
      #define PIR_DETECTOR_NAME_CTR   "HC-SR501"  //move into options list with enums
    #endif
    
    #define MAXIMUM_SENSORS 2
    int8_t pin[2] = {-1, -1};
    uint8_t sensors_active = 0;
    uint8_t fEnableSensor = false;
    pir_detect_t pir_detect[MAXIMUM_SENSORS]; // up to 2 sensors
    
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
    
    struct handler<mAnalog>* mqtthandler_ptr;
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mAnalog> mqtthandler_settings_teleperiod;
    // const char* postfix_topic_sensors = "sensors";
    struct handler<mAnalog> mqtthandler_sensor_ifchanged;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

};
#endif

#endif
