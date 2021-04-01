#ifndef _USE_MODULE_CORE_UPDATES_H
#define _USE_MODULE_CORE_UPDATES_H 0.3

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_UPDATES

#include "3_Network/MQTT/mMQTT.h"

#include "1_TaskerManager/mTaskerInterface.h"

class mUpdates :
  public mTaskerInterface
{

  public:
    mUpdates(){};
    int8_t Tasker(uint8_t function);
    int8_t Tasker_Web(uint8_t function);

    static const char* PM_MODULE_CORE_UPDATES_CTR;
    static const char* PM_MODULE_CORE_UPDATES_FRIENDLY_CTR;
    PGM_P GetModuleName(){         return PM_MODULE_CORE_UPDATES_CTR; }
    PGM_P GetModuleFriendlyName(){ return PM_MODULE_CORE_UPDATES_FRIENDLY_CTR; }

    void init(void);
    void pre_init();

    int8_t CheckAndExecute_JSONCommands();
    void parse_JSONCommand(void);

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
    //#ifdef USE_CORE_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mUpdates> mqtthandler_settings_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;


};

#endif

#endif