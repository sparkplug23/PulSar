#ifndef _MODULE_SENSORS_BH1750_H
#define _MODULE_SENSORS_BH1750_H

#define D_UNIQUE_MODULE_SENSORS_BH1750_ID   140  // Unique value across all classes from all groups (e.g. sensor, light, driver, energy)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_BH1750

#define D_GROUP_MODULE_SENSORS_BH1750_ID    1    // Numerical accesending order of module within a group

// #define USE_MODULE_EXAMPLE_CLASS

#define XSNS_10                          10
#define XI2C_11                          11    // See I2CDEVICES.md

#define BH1750_ADDR1                     0x23
#define BH1750_ADDR2                     0x5C

#define BH1750_CONTINUOUS_HIGH_RES_MODE2 0x11  // Start measurement at 0.5 lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_HIGH_RES_MODE  0x10  // Start measurement at 1   lx resolution. Measurement time is approx 120ms.
#define BH1750_CONTINUOUS_LOW_RES_MODE   0x13  // Start measurement at 4   lx resolution. Measurement time is approx 16ms.

#define BH1750_MEASUREMENT_TIME_HIGH     0x40  // Measurement Time register high 3 bits
#define BH1750_MEASUREMENT_TIME_LOW      0x60  // Measurement Time register low 5 bits

#define D_PRFX_BH1750 "Bh1750"
#define D_CMND_RESOLUTION "Resolution"
#define D_CMND_MTREG "MTime"

#include "1_TaskerManager/mTaskerManager.h"
#include "1_TaskerManager/mTaskerInterface.h"


class mBH1750 :
  public mTaskerInterface
{
  public:
	  mBH1750(){};
    void Pre_Init(void);
    void Init(void);
    
    static const char* PM_MODULE_SENSORS_BH1750_CTR;
    static const char* PM_MODULE_SENSORS_BH1750_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_BH1750_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_BH1750_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_BH1750_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mBH1750);
    };
    #endif
    void parse_JSONCommand(JsonParserObject obj);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    
    struct {
      uint8_t addresses[2] = { BH1750_ADDR1, BH1750_ADDR2 };
      uint8_t resolution[3] = { BH1750_CONTINUOUS_HIGH_RES_MODE, BH1750_CONTINUOUS_HIGH_RES_MODE2, BH1750_CONTINUOUS_LOW_RES_MODE };
      uint8_t count = 0;
      char types[7] = "BH1750";
    } Bh1750;

    struct {
      uint8_t address;
      uint8_t valid = 0;
      uint8_t mtreg = 69;                          // Default Measurement Time
      uint16_t illuminance = 0;
      uint16_t level = 0;
    } Bh1750_sensors[2];
        
    uint8_t Bh1750Resolution(uint32_t sensor_index);
    bool Bh1750SetResolution(uint32_t sensor_index);
    bool Bh1750SetMTreg(uint32_t sensor_index);
    bool Bh1750Read(uint32_t sensor_index);
    void Bh1750EverySecond(void);

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
      
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mBH1750> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mBH1750> mqtthandler_sensor_ifchanged;
    struct handler<mBH1750> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers example
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    //with extra handlers example
    #endif // USE_MODULE_NETWORK_MQTT


};

#endif

#endif
