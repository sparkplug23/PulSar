#ifndef _MODULE_POWERMETER_H
#define _MODULE_POWERMETER_H 0.1

#define D_UNIQUE_MODULE_ENERGY_PZEM004T_ID    131

#include "1_TaskerManager/mTaskerManager.h"
// #include "2_CoreSystem/mBaseConfig.h"

// #include "2_CoreSystem/mFirmwareDefaults.h"
// #include "0_ConfigUser/mFirmwareCustom_Secret.h"
// #include "2_CoreSystem/mSystemConfig.h"

// // #define USE_MODULE_ENERGY_PZEM004T_MODBUS

#ifdef USE_MODULE_ENERGY_PZEM004T_MODBUS

#include <TasmotaSerial.h>
#include <TasmotaModbus.h>

// #include "1_TaskerManager/mTaskerManager.h"


class mPzem_AC :
  public mTaskerInterface
{
  public:
	  mPzem_AC(){};
    void Pre_Init(void);
    void Init(void);
    bool parse_Command();
    uint8_t sReadSensor = false;
    
    static const char* PM_MODULE_ENERGY_PZEM004T_CTR;
    static const char* PM_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_ENERGY_PZEM004T_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_ENERGY_PZEM004T_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_ENERGY_PZEM004T_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mPzem_AC);
    };
    #endif


    void EveryLoop();
    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    #ifdef DEVICE_CONSUMERUNIT
      // dec=16-23, 24,25,26
      const uint8_t PZEM_AC_DEVICE_ADDRESS = 0x10;  // PZEM default address
    #else
      const uint8_t PZEM_AC_DEVICE_ADDRESS = 24;  // PZEM default address
    #endif

    #ifndef MAX_ENERGY_SENSORS
      #define MAX_ENERGY_SENSORS 12
    #endif

    int8_t pin_rx = -1;
    int8_t pin_tx = -1;

    uint32_t tSavedLoop = millis();

    timereached_t measure_time;
    timereached_t measure_time_backoff;

    const uint32_t PZEM_AC_STABILIZE = 30;        // Number of seconds to stabilize configuration

    TasmotaModbus *PzemAcModbus;
    int8_t Tasker(uint8_t function);

    enum AddressConfigSteps { ADDR_IDLE, ADDR_RECEIVE, ADDR_SEND };

    struct PZEMAC {
      float energy = 0;
      float last_energy = 0;
      uint8_t send_retry = 0;
      uint8_t phase = 0;
      uint8_t address = 0;
      uint8_t address_step = ADDR_IDLE;
    } PzemAc;
    
    struct PZEM_MODBUS{
      float voltage;
      float current;
      float active_power;
      float frequency;
      float power_factor;
      float energy;  
    }pzem_modbus[MAX_ENERGY_SENSORS];




    void ParseModbusBuffer(PZEM_MODBUS* mod, uint8_t* buffer);

    uint8_t active_phases = 0;

    void PzemAcEverySecond(void);
    void PzemAcDrvInit(void);
    bool PzemAcCommand(void);
    void ReadAndParse(void);
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    struct handler<mPzem_AC> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mPzem_AC> mqtthandler_sensor_ifchanged;
    struct handler<mPzem_AC> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif


};

#endif

#endif
