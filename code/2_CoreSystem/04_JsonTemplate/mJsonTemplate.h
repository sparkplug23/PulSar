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
    ~mJsonTemplate() {          }
    
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
    
    
    /************************************************************************************************
     * SECTION: Compile-time module template loading
     *
     * Purpose:
     * - MODULE_TEMPLATE is an early boot template.
     * - It configures module identity/GPIO map before GpioInit(), TASK_PRE_INIT, and TASK_INIT.
     * - It must use the direct module-template parser, not TASK_JSON_COMMAND_ID.
     *
     * Policy:
     * - Normal/default pass is called only by Settings when saved settings are missing/invalid.
     * - Override pass is called by Settings when USE_MODULE_TEMPLATE__OVERRIDE is defined.
     *
     * Date Modified: 17May26
     ************************************************************************************************/

    void Template_Load(); // compatibility wrapper for old tasker/manual debug path

    bool ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(bool override_reloading_pass);

    bool ModuleDeviceTemplate_CompileTime_ApplyModuleTemplate_P(
      const char* template_name,
      const char* payload_p,
      size_t payload_size
    );

    bool ReadModuleTemplateFromProgmem(); // compatibility wrapper


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
     /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    

};

#endif

#endif