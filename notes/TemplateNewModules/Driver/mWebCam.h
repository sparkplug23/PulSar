#ifndef _DRIVERS__CAMERA_TASMOTA_H
#define _DRIVERS__CAMERA_TASMOTA_H

#define D_UNIQUE_MODULE_DRIVERS__CAMERA_TASMOTA__ID ((4*1000)+61)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA

#include "1_TaskerManager/mTaskerInterface.h"

class mWebCamera :
  public mTaskerInterface
{
  public:
    mWebCamera(){};

    static const char* PM_MODULE_DRIVERS__CAMERA_TASMOTA__CTR;
    static const char* PM_MODULE_DRIVERS__CAMERA_TASMOTA__FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS__CAMERA_TASMOTA__CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS__CAMERA_TASMOTA__FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS__CAMERA_TASMOTA__ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){    return sizeof(mWebCamera); };
    #endif

    struct SETTINGS    { 
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();
    void EverySecond();
    void Pre_Init();
    void Init();
    void parse_JSONCommand(JsonParserObject obj);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    void MQTTHandler_Sender();
    
    struct handler<mWebCamera> mqtthandler_settings_teleperiod;
    struct handler<mWebCamera> mqtthandler_sensor_ifchanged;
    struct handler<mWebCamera> mqtthandler_sensor_teleperiod;
 
    struct handler<mWebCamera>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    // No specialised payload therefore use system default instead of enum
    
    
    #endif // USE_MODULE_NETWORK_MQTT
    
};

#endif // USE_MODULE_DRIVERS__CAMERA_TASMOTA

#endif // _DRIVERS__CAMERA_TASMOTA_H
