#ifndef HEADER_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT_H
#define HEADER_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT_H
  
#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT_ID 10030 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT

#include <Arduino.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mCustom :
  public mTaskerInterface
{

  private:
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mCustom(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void BootMessage(){};
    
    static constexpr const char* PM_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT_CTR = D_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_CUSTOM__OLED_NITC_AMBIENT_ID; }

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void EverySecond();
    void SubTask_UpdateOLED();

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

     void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: ConstructJSON
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
    
    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT 
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mCustom>*> telemetry_list;    
    struct telemetry_handler<mCustom> telemetry_settings;
    struct telemetry_handler<mCustom> telemetry_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT 

};

#endif // USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE

#endif
