#ifndef _MODULE__DRIVERS_BUZZER_TONES__H
#define _MODULE__DRIVERS_BUZZER_TONES__H

#define D_UNIQUE_MODULE__DRIVERS_BUZZER_TONES__ID   4021 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE__DRIVERS_BUZZER_TONES

class mBuzzerTones :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mBuzzerTones(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE__DRIVERS_BUZZER_TONES__CTR = D_MODULE__DRIVERS__BUZZER__CTR;
    PGM_P GetModuleName(){          return PM_MODULE__DRIVERS_BUZZER_TONES__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE__DRIVERS_BUZZER_TONES__ID; }
   
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

    struct BUZZER {
      uint32_t tune = 0;
      uint32_t tune_reload = 0;
      bool active = true;
      bool enable = false;
      uint8_t inverted = 0;            // Buzzer inverted flag (1 = (0 = On, 1 = Off))
      uint8_t count = 0;               // Number of buzzes
      uint8_t mode = 0;                // Buzzer mode (0 = regular, 1 = infinite, 2 = follow LED)
      uint8_t set[2];
      uint8_t duration;
      uint8_t state = 0;
      uint8_t tune_size = 0;
      uint8_t size = 0;
      uint8_t pin = 0;
    } Buzzer;

    void BuzzerSet(uint32_t state);
    void BuzzerBeep(uint32_t count, uint32_t on, uint32_t off, uint32_t tune, uint32_t mode);
    void BuzzerSetStateToLed(uint32_t state);
    void BuzzerBeep(uint32_t count);
    void BuzzerEnabledBeep(uint32_t count, uint32_t duration);
    void BuzzerEvery100mSec();

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
    void Telemetry_Init();
    std::vector<struct telemetry_handler<mBuzzerTones>*> telemetry_list;
    struct telemetry_handler<mBuzzerTones> telemetry_settings;
    struct telemetry_handler<mBuzzerTones> telemetry_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif
