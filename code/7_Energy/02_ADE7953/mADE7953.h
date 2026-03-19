#ifndef _MODULE_ENERGY_ADE7953_H
#define _MODULE_ENERGY_ADE7953_H

#define D_UNIQUE_MODULE_ENERGY_ADE7953_ID    7002 // [(Folder_Number*100)+ID_File]
#define D_GROUP_MODULE_ENERGY_ADE7953_ID 2

#include "1_TaskerManager/mTaskerManager.h"

/*********************************************************************************************\
 * ADE7953 - Energy (Shelly 2.5)
 *
 * Based on datasheet from https://www.analog.com/en/products/ade7953.html
 *
 * I2C Address: 0x38
\*********************************************************************************************/

#ifdef USE_MODULE_ENERGY_ADE7953

class mEnergyADE7953 :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	  mEnergyADE7953(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_ENERGY_ADE7953_CTR = D_MODULE_ENERGY_ADE7953_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_ENERGY_ADE7953_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_ENERGY_ADE7953_ID; }

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
    
    #define XI2C_07                 7  // See I2CDEVICES.md

    #define ADE7953_PREF            1540
    #define ADE7953_UREF            26000
    #define ADE7953_IREF            10000

    #define ADE7953_ADDR            0x38

    const uint16_t Ade7953Registers[10] = {
      0x31B,  // RMS current channel B (Relay 1)
      0x313,  // Active power channel B
      0x311,  // Apparent power channel B
      0x315,  // Reactive power channel B
      0x31A,  // RMS current channel A (Relay 2)
      0x312,  // Active power channel A
      0x310,  // Apparent power channel A
      0x314,  // Reactive power channel A
      0x31C,  // RMS voltage (Both relays)
      0x10E   // 16-bit unsigned period register
    };

    struct Ade7953 {
      uint32_t voltage_rms = 0;
      uint32_t period = 0;
      uint32_t current_rms[2] = { 0, 0 };
      uint32_t active_power[2] = { 0, 0 };
      uint8_t init_step = 0;
    } measured;

    void EverySecond();
    int RegSize(uint16_t reg);
    void Write(uint16_t reg, uint32_t val);
    int32_t Read(uint16_t reg);
    void GetData(void);
    bool Command(void);


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
    std::vector<struct handler<mEnergyADE7953>*> mqtthandler_list;
    struct handler<mEnergyADE7953> mqtthandler_settings;
    struct handler<mEnergyADE7953> mqtthandler_state_teleperiod;
    struct handler<mEnergyADE7953> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT
    

};

#endif

#endif
