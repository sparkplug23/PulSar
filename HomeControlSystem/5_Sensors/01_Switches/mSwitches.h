#ifndef _MSWITCHES_H_
#define _MSWITCHES_H_

#define D_UNIQUE_MODULE_SENSORS_SWITCHES_ID  ((5*1000)+01) 

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

#include <Ticker.h>

#include "2_CoreSystem/mBaseConfig.h"

#include "2_CoreSystem/02_Time/mTime.h"
#include "2_CoreSystem/05_Logging/mLogging.h"
// #include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/03_HardwareTemplates/mHardwareTemplates.h"
#include "3_Network/01_MQTT/mMQTT.h"
#include "2_CoreSystem/02_Time/mTime.h"


// #ifdef ESP32
//   #include <WiFi.h>
// #endif
// #ifdef ESP8266
//   #include <ESP8266WiFi.h>            // Wifi, MQTT, Ota, WifiManager
//   #include <ESP8266httpUpdate.h>
// #endif



#define SWITCH_PROBE_INTERVAL 10   // Time in milliseconds between switch input probe

#define MAX_SWITCHES 8             // Max number of switches

#include <Ticker.h>

enum SwitchStates { SWITCH_PRESSED_ID, SWITCH_NOT_PRESSED_ID };

#include "1_TaskerManager/mTaskerInterface.h"

class mSwitches :
  public mTaskerInterface
{
  public:
    mSwitches(){};
    
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void init(void);

    static const char* PM_MODULE_SENSORS_SWITCHES_CTR;
    static const char* PM_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_SENSORS_SWITCHES_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_SENSORS_SWITCHES_ID; }

    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mSwitches);
    };
    #endif

    struct SETTINGS{
      uint8_t switches_found = 0;
    }settings;

    struct SWITCHES{
      uint8_t  state     = false;
      // uint8_t  isactive  = false;
      uint8_t  ischanged = false;

      // I need to adopt tas method, as switches can expand to much larger on esp32
      // pins can stay here, but inverted will need set as flag
      int8_t pin = -1; // -1 is not active

      uint8_t active_state_value = false; //defualt active high

      uint8_t lastwallswitch;
      uint8_t holdwallswitch;
            
      uint8_t switch_state_buf; //[MAX_SWITCHES] = { 0 };
      uint8_t switch_virtual;   //[MAX_SWITCHES]; 

      // uint8_t is_linked_to_internal_relay = false;
      int8_t linked_internal_relay_id = -1; // -1 inactive

      // uint32_t tDetectTime;
      // uint32_t tEndedTime;
      // time_short_t detected_time;
    }switches[MAX_SWITCHES];

    bool IsSwitchActive(uint8_t id);

    Ticker* TickerSwitch = nullptr;

    #ifdef USE_MODULE_NETWORK_WEBSERVER
    void WebAppend_Root_Draw_Table();
    void WebAppend_Root_Status_Table();

    #endif// USE_MODULE_NETWORK_WEBSERVER

    uint32_t tSavedSwitchProbe = millis();
    uint32_t switch_debounce = 0;          // Switch debounce timer
    uint16_t switch_no_pullup = 0;              // Switch pull-up bitmask flags
    
    void SetPullupFlag(uint16_t switch_bit);
    uint8_t GetLastState(uint8_t index);
    void SetVirtual(uint8_t index, uint8_t state);
    uint8_t GetVirtual(uint8_t index);


    void SwitchHandler(uint8_t mode);
    void SwitchInit(void);
    void SwitchLoop();
    void SwitchProbe(void);


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
  
    #ifdef USE_MODULE_NETWORK_MQTT 
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);
    
    struct handler<mSwitches> mqtthandler_settings_teleperiod;
    struct handler<mSwitches> mqtthandler_sensor_ifchanged;
    struct handler<mSwitches> mqtthandler_sensor_teleperiod;

    // No specialised payload therefore use system default instead of enum
    const uint8_t MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

    struct handler<mSwitches>* mqtthandler_list[3] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_sensor_ifchanged,
      &mqtthandler_sensor_teleperiod
    };

    #endif // USE_MODULE_NETWORK_MQTT 


};



#endif

#endif  // _SONOFF_H_
//#endif