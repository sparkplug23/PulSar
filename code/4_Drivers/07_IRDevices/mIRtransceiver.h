#ifndef HEADER_DRIVERS__IRTRANSCEIVER_H
#define HEADER_DRIVERS__IRTRANSCEIVER_H

#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_DRIVERS_IRTRANSCEIVER_ID 4007 // [(Folder_Number*100)+ID_File]

#ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER

#ifdef ESP32
   #include "IRremote.h"
#endif
#ifdef ESP8266
   #include "4_Drivers/IRDevices/IRLib8266/IRremoteESP8266.h"
   #include "4_Drivers/IRDevices/IRLib8266/IRsend.h"
#endif

class mIRtransceiver :
  public mTaskerInterface
{

  private:

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
	 mIRtransceiver(){};
    void Pre_Init(void);
    void Init(void);
    void BootMessage();
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    static constexpr const char* PM_MODULE_DRIVERS_IRTRANSCEIVER_CTR = D_MODULE_DRIVERS_IRTRANSCEIVER_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_IRTRANSCEIVER_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_IRTRANSCEIVER_ID; }
   
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
    

    int8_t pin = -1;

    #define ONE_BIT_TIME 1252 //+- 30 std
    #define ZERO_BIT_TIME 439 //+- 30 std

    void TransmitCode(uint32_t code, uint8_t code_bits, uint8_t repeat_count, uint8_t pin = 255); // to be moved into here




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
    std::vector<struct handler<mIRtransceiver>*> mqtthandler_list;
    struct handler<mIRtransceiver> mqtthandler_settings;
    struct handler<mIRtransceiver> mqtthandler_state_ifchanged;
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif