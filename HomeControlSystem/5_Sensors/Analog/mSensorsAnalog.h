#ifndef _mSensorsAnalog_H
#define _mSensorsAnalog_H 1.0

#define D_UNIQUE_MODULE_SENSORS_ANALOG_ID 122

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_ANALOG

#include "1_TaskerManager/mTaskerInterface.h"

class mSensorsAnalog :
  public mTaskerInterface
{
  public:
    // Constructor
    mSensorsAnalog(){};
    // Init the class
    void Init(void);
    void Pre_Init(void);
    // Tasker that is called on each loop
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    // All SubTasks called by Tasker 
    void EveryLoop();


    static const char* PM_MODULE_SENSORS_ANALOG_CTR;
    static const char* PM_MODULE_SENSORS_ANALOG_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_ANALOG_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_ANALOG_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_ANALOG_ID; }

    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSensorsAnalog);
    };
    #endif

    void parse_JSONCommand();

    // #ifndef PIR_DETECTOR_NAME_CTR
    //   #define PIR_DETECTOR_NAME_CTR   "HC-SR501"  //move into options list with enums
    // #endif
    
    // #define MAXIMUM_SENSORS 2
    // int8_t pin[2] = {-1, -1};
    // uint8_t sensors_active = 0;
    // uint8_t fEnableSensor = false;
    // pir_detect_t pir_detect[MAXIMUM_SENSORS]; // up to 2 sensors
    
    
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebPage_Root_AddHandlers();
    void WebAppend_Root_Status_Table();
    void WebSend_JSON_Root_Table(AsyncWebServerRequest *request);
    #endif// USE_MODULE_NETWORK_WEBSERVER

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
    
    struct handler<mSensorsAnalog>* mqtthandler_ptr;
    struct handler<mSensorsAnalog> mqtthandler_settings_teleperiod;
    struct handler<mSensorsAnalog> mqtthandler_sensor_ifchanged;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif

};
#endif

#endif
