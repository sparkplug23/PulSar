#ifndef _MODULE_SENSORS_EXAMPLE_H
#define _MODULE_SENSORS_EXAMPLE_H

#define D_UNIQUE_MODULE_SENSORS_EXAMPLE_ID   4020 // [(Folder_Number*100)+ID_File]

// #define USE_MODULE_EXAMPLE_CLASS

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_EXAMPLE

class mExampleClass :
  public mTaskerInterface
{
  public:
	  mExampleClass(){};
    void Pre_Init(void);
    void Init(void);
    
    static constexpr const char* PM_MODULE_SENSORS_EXAMPLE_CTR = D_MODULE_SENSORS_EXAMPLE_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_EXAMPLE_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_EXAMPLE_ID; }

    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    void MQTTHandler_Sender();
    struct handler<mExampleClass> mqtthandler_settings;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mExampleClass> mqtthandler_sensor_ifchanged;
    struct handler<mExampleClass> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    uint8_t list_ids[3] = {
      MQTT_HANDLER_SETTINGS_ID, 
      MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
      MQTT_HANDLER_SENSOR_TELEPERIOD_ID
    };
    
    struct handler<mExampleClass>* list_ptr[3] = {
      &mqtthandler_settings,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    //No extra handlers example
    
    //with extra handlers example
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
