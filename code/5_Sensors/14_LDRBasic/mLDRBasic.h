#ifndef _mLDRBasic_H_
#define _mLDRBasic_H_

#define D_UNIQUE_MODULE_SENSORS_LDR_BASIC_ID 122

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_LDR_BASIC

#include <Ticker.h>

#include "2_CoreSystem/mBaseConfig.h"

#include "2_CoreSystem/02_Time/mTime.h"
#include "2_CoreSystem/05_Logging/mLogging.h"
// #include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"
// #include "3_Network/MQTT/mMQTT.h"
#include "2_CoreSystem/02_Time/mTime.h"


#define MAX_LDR_BASIC 2             // Max number of switches

#include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mLDRBasic :
  public mTaskerInterface
{
  public:
    mLDRBasic(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    void Pre_Init(void);
    void Init(void);

    static const char* PM_MODULE_SENSORS_LDR_BASIC_CTR;
    static const char* PM_MODULE_SENSORS_LDR_BASIC_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_LDR_BASIC_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_LDR_BASIC_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_LDR_BASIC_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mLDRBasic);
    };
    #endif

    void EveryLoop();
    void EverySecond();

    struct SETTINGS{
      bool fEnableSensor = false;
    }settings;

    struct LDR_BASIC{

      uint8_t  digital_reading = 0;
      uint16_t analog_reading = 0;

    }ldr[MAX_LDR_BASIC];

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebAppend_Root_Draw_Table();
    void WebAppend_Root_Status_Table();
    #endif// USE_MODULE_NETWORK_WEBSERVER

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_object_start_end_required = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mLDRBasic> mqtthandler_settings_teleperiod;
    struct handler<mLDRBasic> mqtthandler_sensor_ifchanged;
    struct handler<mLDRBasic> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mLDRBasic>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT 


};



#endif

#endif  // _SONOFF_H_
//#endif