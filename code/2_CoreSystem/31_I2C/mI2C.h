#ifndef _USE_MODULE_CORE_I2C_H
#define _USE_MODULE_CORE_I2C_H 0.3

#define D_UNIQUE_MODULE_CORE__I2C__ID 2031 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_I2C

#include "1_TaskerManager/mTaskerInterface.h"

class mI2C :
  public mTaskerInterface
{

  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mI2C(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Pre_Init(void);
        
    static constexpr const char* PM_MODULE_CORE__I2C__CTR = D_MODULE_CORE__I2C__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE__I2C__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE__I2C__ID; }
    
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

    const uint8_t I2C_RETRY_COUNTER = 3;
    uint32_t i2c_active[4] = { 0 };
    uint32_t i2c_buffer = 0;
    TwoWire* wire = nullptr;  // I2C should really be moved to hardwarepins, or a new "peripherials" ie I2C, another for SPI
    bool I2cValidRead(uint8_t addr, uint8_t reg, uint8_t size);
    bool I2cValidRead8(uint8_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidRead16(uint16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidReadS16(int16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidRead16LE(uint16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidReadS16_LE(int16_t *data, uint8_t addr, uint8_t reg);
    bool I2cValidRead24(int32_t *data, uint8_t addr, uint8_t reg);
    uint8_t I2cRead8(uint8_t addr, uint8_t reg);
    uint16_t I2cRead16(uint8_t addr, uint8_t reg);
    int16_t I2cReadS16(uint8_t addr, uint8_t reg);
    uint16_t I2cRead16LE(uint8_t addr, uint8_t reg);
    int16_t I2cReadS16_LE(uint8_t addr, uint8_t reg);
    int32_t I2cRead24(uint8_t addr, uint8_t reg);
    bool I2cWrite(uint8_t addr, uint8_t reg, uint32_t val, uint8_t size);
    bool I2cWrite8(uint8_t addr, uint8_t reg, uint16_t val);
    bool I2cWrite16(uint8_t addr, uint8_t reg, uint16_t val);
    int8_t I2cReadBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len);
    int8_t I2cWriteBuffer(uint8_t addr, uint8_t reg, uint8_t *reg_data, uint16_t len);
    void I2cScan(char *devs, unsigned int devs_len);
    bool I2cDevice(uint8_t addr);
    bool I2cDevice_IsConnected(uint8_t addr) ;
    void I2cResetActive(uint32_t addr, uint32_t count = 1);
    void I2cSetActive(uint32_t addr, uint32_t count = 1);
    void I2cSetActiveFound(uint32_t addr, const char *types);
    void I2cSetActiveFound_P(uint32_t addr, const char *types);
    bool I2cActive(uint32_t addr);
    bool I2cSetDevice(uint32_t addr);
    bool I2cEnabled(uint32_t i2c_index);
    void Debug_I2CScan_To_Serial();

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

    // std::vector<struct handler<mI2C>*> mqtthandler_list;    
    // struct handler<mI2C> mqtthandler_settings;    
    // struct handler<mI2C> mqtthandler_sensor_ifchanged;
    // struct handler<mI2C> mqtthandler_sensor_teleperiod;    
    // #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif