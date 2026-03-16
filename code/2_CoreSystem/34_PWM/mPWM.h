#ifndef _CORE_PWM_H_
#define _CORE_PWM_H_

#define D_UNIQUE_MODULE_CORE__PWM__ID 2034 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_PWM

#include "1_TaskerManager/mTaskerInterface.h"

#include "Blending_Data.h"

class mPWM :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mPWM(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_CORE_PWM_CTR = D_MODULE_CORE_PWM_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_PWM_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE__PWM__ID; }
   
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

    void EveryLoop();
    void EverySecond();

    struct SETTINGS{
      bool fEnableSensor = false;
    }settings;

    #define MAX_PWM_PINS 5
    struct PWM_VALUES{

      uint16_t value = 0;
      int8_t pin = -1;
      
      LinearBlendVariable<uint16_t>*  blended_value = nullptr;

      uint16_t range_min = 0;
      uint16_t range_max = 1024;
      
    }pwm[MAX_PWM_PINS];

    uint8_t dir = 0;

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    std::vector<struct handler<mPWM>*> mqtthandler_list;
    struct handler<mPWM> mqtthandler_settings;
    struct handler<mPWM> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

};



#endif

#endif  // _SONOFF_H_
//#endif