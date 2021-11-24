#ifndef _USE_MODULE_CORE_UPDATES_H
#define _USE_MODULE_CORE_UPDATES_H 0.3

#define D_UNIQUE_MODULE_CORE_UPDATES_ID 7

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_UPDATES

#include "3_Network/MQTT/mMQTT.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mUpdates :
  public mTaskerInterface
{

  public:
    mUpdates(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    int8_t Tasker_Web(uint8_t function);

    static const char* PM_MODULE_CORE_UPDATES_CTR;
    static const char* PM_MODULE_CORE_UPDATES_FRIENDLY_CTR;
    PGM_P GetModuleName(){         return PM_MODULE_CORE_UPDATES_CTR; }
    PGM_P GetModuleFriendlyName(){ return PM_MODULE_CORE_UPDATES_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_UPDATES_ID; }

    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mUpdates);
    };
    #endif

    void init(void);
    void Pre_Init();

    void parse_JSONCommand(JsonParserObject obj);
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
    //#ifdef USE_CORE_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mUpdates> mqtthandler_settings_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif