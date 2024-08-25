#ifndef _MODULE_SENSORS_ROTARY_ENCODER_H
#define _MODULE_SENSORS_ROTARY_ENCODER_H

#define D_UNIQUE_MODULE_SENSORS_ROTARY_ENCODER_ID   ((5*1000)+21)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_ROTARY_ENCODER

#include "2_CoreSystem/07_Time/mTime.h"
#include "5_Sensors/00_Interface/mSensorsInterface.h"
#include <ESP32Encoder.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mRotaryEncoder :
  public mTaskerInterface
{
  public:
	  mRotaryEncoder(){};
    void Pre_Init(void);
    void Init(void);
    
    static constexpr const char* PM_MODULE_SENSORS_ROTARY_ENCODER_CTR = D_MODULE_SENSORS_ROTARY_ENCODER_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_ROTARY_ENCODER_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_ROTARY_ENCODER_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){ return sizeof(mRotaryEncoder); };
    #endif

    struct SETTINGS{
      uint8_t sensors_active = 0;
      uint8_t fEnableSensor = false;
    }settings;

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void EveryLoop();

    void EveryLoop_ReadSensor();    

    ESP32Encoder* encoder;

    struct DATA{
      int64_t value = 0;
    }data;

    void parse_JSONCommand(JsonParserObject obj);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();
    
    void MQTTHandler_Sender();
    struct handler<mRotaryEncoder> mqtthandler_settings_teleperiod;
    void MQTTHandler_Settings(uint8_t topic_id=0, uint8_t json_level=0);
    struct handler<mRotaryEncoder> mqtthandler_sensor_ifchanged;
    void MQTTHandler_Sensor(uint8_t message_type_id=0, uint8_t json_method=0);

    
    #endif // USE_MODULE_NETWORK_MQTT
    
    struct handler<mRotaryEncoder>* mqtthandler_list[2] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged
    };

};

/**
 * @brief LPD3806-360BM-G5-24C
 * LPD3806
 * 360BM    360 count positions per rotation (1 degree resolution)
 * G5       5V
 * 24C
 * 
 */

#endif

#endif
