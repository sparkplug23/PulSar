#ifndef _MRELAYS_H
#define _MRELAYS_H 0.1

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_DRIVERS_RELAY

#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/Settings/mSettings.h"

#include "3_Network/MQTT/mMQTT.h"

#include "2_CoreSystem/Time/mTime.h"


// template<typename T>
// void f();

// extern template void f<int>();


class mRelays{
  public:
    mRelays(){};

    #ifndef RELAYS_CONNECTED
      #define RELAYS_CONNECTED 4
    #endif

    
    // template<typename T>
    // void ftest();

    // template<typename T>
    // void ftest2(T value);


    // //overload
    // int8_t Tasker2(uint8_t function, uint8_t param1);
    // template<typename T = int>
    // int8_t Tasker2(uint8_t function, T param1);
    
    



    void parsesub_TopicCheck_JSONCommand(JsonObjectConst obj);

    #define RELAYS_MAX_COUNT 4

    enum RELAY_IDS{
      RELAY_0_ID=0,
      RELAY_1_ID,
      RELAY_2_ID,
      RELAY_3_ID,
      RELAY_MAX_ID
    }; //phase out? use numbers

    const char* GetRelayNameStoredbyIDCtr(char* name_buffer, uint8_t name_buffer_space, uint8_t device_id);

    #define D_DEVICE_RELAY_0_NAME "R0"
    #ifndef D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG
      #define D_DEVICE_RELAY_0_FRIENDLY_NAME_LONG "Relay0"
    #endif
    #define D_DEVICE_RELAY_1_NAME "R1"
    #ifndef D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG
      #define D_DEVICE_RELAY_1_FRIENDLY_NAME_LONG "Relay1"
    #endif
    #define D_DEVICE_RELAY_2_NAME "R2"
      #ifndef D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG
      #define D_DEVICE_RELAY_2_FRIENDLY_NAME_LONG "Relay2"
    #endif
    #define D_DEVICE_RELAY_3_NAME "R3"
      #ifndef D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG
      #define D_DEVICE_RELAY_3_FRIENDLY_NAME_LONG "Relay3"
    #endif
    

    int8_t Tasker(uint8_t function);
    int8_t Tasker(uint8_t function, JsonObjectConst obj);

    
int8_t CheckAndExecute_JSONCommands(JsonObjectConst obj);

    void MQTTConnected();

    void WebAppend_Root_Add_Buttons();
    
    
    void init(void);

    typedef unsigned long power_t;              // Power (Relay) type
    uint8_t relays_connected = 0;
    
    void SetLatchingRelay(power_t lpower, uint32_t state);
    void SetDevicePower(power_t rpower, uint32_t source);
    void RestorePower(bool publish_power, uint32_t source);   
    void SetAllPower(uint32_t state, uint32_t source);
    void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source);
    // void ExecuteCommandPowerZeroIndex(uint32_t device, uint32_t state, uint32_t source);

    void SetPowerOnState(void);
    // power_t power = 0;                          // Current copy of Settings.power
    power_t last_power = 0;                     // Last power set state
    power_t blink_power;                        // Blink power state
    power_t blink_mask = 0;                     // Blink relay active mask
    power_t blink_powersave;                    // Blink start power save state
    power_t latching_power = 0;                 // Power state at latching start
    power_t rel_inverted = 0;                   // Relay inverted flag (1 = (0 = On, 1 = Off))

    uint8_t active_device = 1;                  // Active device in ExecuteCommandPower

    uint32_t tSavedTick = millis(),tSavedForceUpdate;
    uint8_t toggle_bit = 0;
    void WebAppend_Root_Status_Table();
    void parse_JSONCommand(JsonObjectConst obj);

    uint32_t tSavedTest = millis();

    uint8_t fForceMQTTUpdate = true; //each local subtasker has its own fForce



//
  // I should add a relay "type" ie, external power, internal power, light


    struct RELAY_STATUS{

      //also flags bit
      uint8_t onoff = false;
      uint8_t ischanged = false;
      // uint8_t flags.relay_type
      struct datetime ontime;
      struct datetime offtime;
    }relay_status[RELAYS_CONNECTED];

    void SetRelay(uint8_t state);
    uint8_t GetRelay(uint8_t num);
    void SetRelay(uint8_t num, uint8_t state);

    const char* GetRelayNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);//D_DEFAULT_DEVICE_BUFFER_LENGTH);
   const char* GetRelayNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);//D_DEFAULT_DEVICE_BUFFER_LENGTH);
    int8_t GetRelayIDbyName(const char* c);

    int8_t GetDeviceIDbyName(const char* c);
    void WebAppend_Root_Draw_PageTable();

struct SETTINGS{
uint8_t fShowTable = false;
}settings;


    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);

  
  //#ifdef USE_CORE_MQTT 

    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    struct handler<mRelays>* mqtthandler_ptr;
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR = "settings";
    struct handler<mRelays> mqtthandler_settings_teleperiod;
    
    // const char* PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR = "power";
    struct handler<mRelays> mqtthandler_sensor_ifchanged;
    struct handler<mRelays> mqtthandler_sensor_teleperiod;
    
    const int MQTT_HANDLER_MODULE_LENGTH_ID = MQTT_HANDLER_LENGTH_ID;

  //#endif



};


// extern template void mRelays::ftest<int>();
// extern template void mRelays::ftest2<int>(int value);
// extern template void mRelays::ftest2<char>(char value);


// extern template int8_t mRelays::Tasker2<JsonObjectConst>(uint8_t function, JsonObjectConst param1);
// extern template int8_t mRelays::Tasker2<uint8_t>(uint8_t function, uint8_t param1);


// extern template void mSupport::f2<int>();
#endif

#endif
