#ifndef _MODULE_POWERMETER_H
#define _MODULE_POWERMETER_H 0.1

#include "0_ConfigUser/mUserConfig.h"

#include "2_CoreSystem/mFirmwareDefaults.h"
#include "0_ConfigUser/mFirmwareCustom.h"
#include "2_CoreSystem/mSystemConfig.h"

// #define USE_MODULE_SENSORS_PZEM004T_MODBUS

#ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS

#include <TasmotaSerial.h>
#include <TasmotaModbus.h>

#include "1_TaskerManager/mTaskerManager.h"


class mPzem_AC{
  public:
	  mPzem_AC(){};
    void Pre_Init(void);
    void Init(void);
    bool parse_Command();
    uint8_t sReadSensor = false;
    
    void EveryLoop();
    struct SETTINGS{
      uint8_t fEnableSensor = false;
    }settings;

    #ifdef DEVICE_CONSUMERUNIT
      const uint8_t PZEM_AC_DEVICE_ADDRESS = 0x10;  // PZEM default address
    #else
      const uint8_t PZEM_AC_DEVICE_ADDRESS = 0x01;  // PZEM default address
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
    
    struct handler<mPzem_AC>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mPzem_AC> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "sensors";
    struct handler<mPzem_AC> mqtthandler_sensor_ifchanged;
    struct handler<mPzem_AC> mqtthandler_sensor_teleperiod;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    //No extra handlers
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
  //#endif


};

#endif

#endif
