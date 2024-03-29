#ifndef _USE_MODULE_CORE_UPDATES_H
#define _USE_MODULE_CORE_UPDATES_H 0.3

#define D_UNIQUE_MODULE_CORE_UPDATES_ID 7

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_UPDATES

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
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_UPDATES_ID; }

    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mUpdates);
    };
    #endif

    void init(void);
    void Pre_Init();

    void parse_JSONCommand(JsonParserObject obj);

    void WebPage_Root_AddHandlers();
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
  
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    struct handler<mUpdates> mqtthandler_settings_teleperiod;

    
    std::vector<struct handler<mUpdates>*> mqtthandler_list;
    
};

#endif

#endif