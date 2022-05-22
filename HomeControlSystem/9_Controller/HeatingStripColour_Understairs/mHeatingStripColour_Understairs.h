#ifndef _MCONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_H
#define _MCONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_H

#define D_UNIQUE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_ID 175

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS

class mHeatingStripColour_Understairs :
  public mTaskerInterface
{
  public:
    mHeatingStripColour_Understairs(){};
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void Init(void);
    void Pre_Init(void);

    struct SETTINGS{
      uint8_t fEnableSensor = false;
      uint8_t fSensorCount= 0; 
    }settings;
    
    void EveryLoop();
    
    void Every_Second();

    static const char* PM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_CTR;
    static const char* PM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_FRIENDLY_CTR; }
    uint8_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mHeatingStripColour_Understairs);
    };
    #endif
    
    void SubTasker_MQTTSender();

    RgbcctColor colour[4];

    void parse_JSONCommand(JsonParserObject obj);
    
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_State(uint8_t json_method = 0);
  
    #ifdef USE_MODULE_NETWORK_MQTT

      void MQTTHandler_Init();
      void MQTTHandler_Set_RefreshAll();
      void MQTTHandler_Set_TelePeriod();
      
      void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

      struct handler<mHeatingStripColour_Understairs> mqtthandler_settings_teleperiod;
      struct handler<mHeatingStripColour_Understairs> mqtthandler_state_ifchanged;
      struct handler<mHeatingStripColour_Understairs> mqtthandler_state_teleperiod;

      // No specialised payload therefore use system default instead of enum
      const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;
      
      struct handler<mHeatingStripColour_Understairs>* mqtthandler_list[3] = {
        &mqtthandler_settings_teleperiod,
        &mqtthandler_state_ifchanged,
        &mqtthandler_state_teleperiod
      };

    #endif // USE_MODULE_NETWORK_MQTT

};

#endif //DEVICE_BUCKET_WATER_LEVEL

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
