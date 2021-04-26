#ifndef _MODULE_CONTROLLER_SDCARDLOGGER_H
#define _MODULE_CONTROLLER_SDCARDLOGGER_H

#define D_UNIQUE_MODULE_CONTROLLER_SDCARDLOGGER_ID   131  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)
#define D_GROUP_MODULE_CONTROLLER_SDCARDLOGGER_ID    1    // Numerical accesending order of module within a group

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER

class mSDCardLogger :
  public mTaskerInterface
{
  public:
	  mSDCardLogger(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_CONTROLLER_SDCARDLOGGER_CTR;
    static const char* PM_MODULE_CONTROLLER_SDCARDLOGGER_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_SDCARDLOGGER_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_SDCARDLOGGER_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_SDCARDLOGGER_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSDCardLogger);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mSDCardLogger> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mSDCardLogger> mqtthandler_sensor_ifchanged;
    struct handler<mSDCardLogger> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
