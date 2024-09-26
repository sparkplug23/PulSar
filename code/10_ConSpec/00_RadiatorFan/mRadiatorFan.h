#ifndef _MRADIATORFAN_H
#define _MRADIATORFAN_H

#define D_UNIQUE_MODULE_CONTROLLER_RADIATORFAN_ID 175

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_RADIATORFAN

class mRadiatorFan :
  public mTaskerInterface
{
  public:
    mRadiatorFan(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);

    struct SETTINGS{
      bool enabled_module = true;
      bool enabled_temperature_control = true; // ie could be false, therefore mqtt controls
    }settings;

    struct STATE{
      bool iswithin_temperature_limit = false;
      float threshold_maximum_temperature = 100;
      float threshold_minimum_temperature = 30;
      float temperature_current = -1;
    }state;

    static constexpr const char* PM_MODULE_CONTROLLER_RADIATORFAN_CTR = D_MODULE_CONTROLLER_RADIATORFAN_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_RADIATORFAN_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_RADIATORFAN_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mRadiatorFan);
    };
    #endif
    
    void SubTasker_MQTTSender();

    void   Task_UseTemperatureToControlRelay();

    void parse_JSONCommand(JsonParserObject obj);
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);
  
    #ifdef USE_MODULE_NETWORK_MQTT

      void MQTTHandler_Init();
      void MQTTHandler_RefreshAll();
      void MQTTHandler_Rate();
      
      void MQTTHandler_Sender();

      struct handler<mRadiatorFan> mqtthandler_settings;
      struct handler<mRadiatorFan> mqtthandler_state_ifchanged;
      struct handler<mRadiatorFan> mqtthandler_state_teleperiod;

      // No specialised payload therefore use system default instead of enum
      
      
      struct handler<mRadiatorFan>* mqtthandler_list[3] = {
        &mqtthandler_settings,
        &mqtthandler_state_ifchanged,
        &mqtthandler_state_teleperiod
      };

    #endif // USE_MODULE_NETWORK_MQTT

};

#endif //DEVICE_RADIATORFAN

#endif



/**
 * This is a basic HVAC, that could technically be performed by rules only
 * 
 * This is basically a usermod_radiatordfan
 * 
 * 
 * */

// Rename to something generic

// Description: Temperature controlled relays
// Use example: Temp probe that measures radiator temperature and controls fan to assist in heat disspersion

// This is the equivalent of a simple rule, temp drops below threshold, relay off, temp goes above, relay on

// "mTemperature"

// can this be described as hvac? temp drops, relay on, though, its continuous and no shcedules (too complex for its need)

// mTemperatureControlledRelays()



// What this could be merged into: sensor controlled, threshold high and low (hysteresis), output (any driver)
// Actions/Rule

// Relay mode to use temp sensor to control state
