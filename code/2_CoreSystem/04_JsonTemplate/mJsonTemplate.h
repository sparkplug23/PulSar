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
        
    static constexpr const char* PM_MODULE_CORE__TEMPLATE_LOADING__CTR = D_MODULE__CORE__TEMPLATE_LOADING__CTR;
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
     * FILE: mJsonTemplate.h
     *
     * Replace the existing Internal Functions section with this block.
     ************************************************************************************************/

      void Template_Load();                    // Compatibility wrapper for old/manual task path.
      bool ReadModuleTemplateFromProgmem();    // Compatibility wrapper for older call sites.

      bool ModuleDeviceTemplate_CompileTime_Load_ModuleOnly(bool override_reloading_pass);
      bool ModuleDeviceTemplate_CompileTime_Load_Late(bool override_reloading_pass);

      bool ModuleDeviceTemplate_CompileTime_ApplyModuleTemplate_P(
        const char* template_name,
        const char* payload_p,
        size_t payload_size
      );

      bool ModuleDeviceTemplate_CompileTime_ApplyJsonCommand_P(
        const char* template_name,
        const char* payload_p,
        size_t payload_size
      );

      void ModuleDeviceTemplate_CompileTime_DevelopmentOverridePass();

      void ModuleDeviceTemplate__LoadDefault();

      #ifdef ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM

    /************************************************************************************************
     * SECTION: Compile-time template export
     *
     * Purpose:
     * - Saves compile-time PROGMEM templates into the internal filesystem.
     * - Files are flat root-level JSON files so they can be viewed through /edit.
     *
     * Date Modified: 17May26
     ************************************************************************************************/

      void Templates__SaveHardcodedTemplateToFilesystem();

      bool Templates__SaveProgmemTemplateToFilesystem_P(
        const char* file_path,
        const char* template_name,
        const char* payload_p,
        size_t payload_size
      );

      #endif

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