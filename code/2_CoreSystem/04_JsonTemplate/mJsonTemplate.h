#ifndef _USE_MODULE_CORE__JSON_TEMPLATE_H
#define _USE_MODULE_CORE__JSON_TEMPLATE_H 0.3

#define D_UNIQUE_MODULE_CORE_JSON_TEMPLATE_ID 2004 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE__JSON_TEMPLATE

#include "1_TaskerManager/mTaskerInterface.h"

class mJsonTemplate :
  public mTaskerInterface
{

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mJsonTemplate(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Pre_Init(void);
        
    static constexpr const char* PM_MODULE_CORE__TEMPLATE_LOADING__CTR = D_MODULE_CORE__TEMPLATE_LOADING__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE__TEMPLATE_LOADING__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_JSON_TEMPLATE_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void Template_Load();
    bool ReadModuleTemplateFromProgmem();
    #ifdef ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
    void Templates__SaveHardcodedTemplateToFilesystem();
    #endif
        

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    // void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    // uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    // uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

     /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    
    // #ifdef USE_MODULE_NETWORK_MQTT
    // void MQTTHandler_Init();
    // void MQTTHandler_RefreshAll();
    // void MQTTHandler_Rate();    
    // void MQTTHandler_Sender();

    // std::vector<struct handler<mJsonTemplate>*> mqtthandler_list;    
    // struct handler<mJsonTemplate> mqtthandler_settings;    
    // struct handler<mJsonTemplate> mqtthandler_sensor_ifchanged;
    // struct handler<mJsonTemplate> mqtthandler_sensor_teleperiod;    
    // #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif