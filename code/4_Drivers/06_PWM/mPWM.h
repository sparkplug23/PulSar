#ifndef _DRIVERS_PWM_H_
#define _DRIVERS_PWM_H_

#define D_UNIQUE_MODULE_DRIVERS_PWM_ID 4006 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_PWM

#include <Ticker.h>

#include "2_CoreSystem/mBaseConfig.h"
#include "2_CoreSystem/07_Time/mTime.h"
#include "2_CoreSystem/08_Logging/mLogging.h"
#include "3_Network/MQTT/mMQTT.h"
#include "2_CoreSystem/07_Time/mTime.h"

#include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

#include "2c_Internal_IsolatedNoTaskerSystems/DataBlend/mDataBlend.h"

class mPWM :
  public mTaskerInterface
{
  public:
    mPWM(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    
    void Pre_Init(void);
    void Init(void);

    static constexpr const char* PM_MODULE_DRIVERS_PWM_CTR = D_MODULE_DRIVERS_PWM_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_PWM_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_PWM_ID; }


    void parse_JSONCommand(JsonParserObject obj);

    void EveryLoop();
    void EverySecond();

    // LinearBlendVariable<uint16_t>* variable_blend = nullptr;
    // LinearBlendVariable<uint16_t>*  var_blend = nullptr;

    // LinearBlendVariable<float>*  var_blend_f = nullptr;
    // LinearBlendVariable<uint8_t>*  var_blend_u8 = nullptr;

    struct SETTINGS{
      bool fEnableSensor = false;
    }settings;

    #define MAX_PWM_PINS 5
    struct PWM_VALUES{

      uint16_t value = 0;
      int8_t pin = -1;
      
      LinearBlendVariable<uint16_t>*  blended_value = nullptr;

      uint16_t range_min = 0;
      uint16_t range_max = 1024;
      
    }pwm[MAX_PWM_PINS];

    uint8_t dir = 0;

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    
    struct handler<mPWM>* ptr;
    void MQTTHandler_Sender();

    struct handler<mPWM> mqtthandler_settings;
    struct handler<mPWM> mqtthandler_state_ifchanged;
    struct handler<mPWM> mqtthandler_state_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
    
    struct handler<mPWM>* mqtthandler_list[3] = {
      &mqtthandler_settings,
      &mqtthandler_state_ifchanged,
      &mqtthandler_state_teleperiod
    };
    #endif // USE_MODULE_NETWORK_MQTT 


};



#endif

#endif  // _SONOFF_H_
//#endif