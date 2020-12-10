#ifndef _MRELAYS_H
#define _MRELAYS_H 0.1

#include "0_ConfigUser/mUserConfig.h"

#ifdef USE_MODULE_DRIVERS_RELAY

#include "1_TaskerManager/mTaskerManager.h"
#include "2_CoreSystem/Settings/mSettings.h"

#include "3_Network/MQTT/mMQTT.h"

#include "2_CoreSystem/Time/mTime.h"

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SCHEDULED_CTR) "scheduled";


class mRelays{
  public:
    mRelays(){};

    #ifndef RELAYS_CONNECTED
      #define RELAYS_CONNECTED 4
    #endif
    
    typedef union {
      uint16_t data; // allows full manipulating
      struct { 
        // enable seconds on
        uint16_t enabled_timer_decounters : 1;
        // enable seconds on
        uint16_t enabled_scheduled_autocontrol : 1;
        // enable seconds on
        uint16_t enabled_relays_allowed_time_window_checks : 1;
      };
    } RELAY_SETTINGS_FLAGS;


    struct SETTINGS{
      /**
       * Shared flags 
       * */
      RELAY_SETTINGS_FLAGS flags;

      uint8_t fShowTable = false;
      uint8_t relays_connected = 0;
    }settings;

    int8_t CheckAndExecute_JSONCommands();
    void   parse_JSONCommand(void);

    #define RELAYS_MAX_COUNT 4

    // enum RELAY_IDS{
    //   RELAY_0_ID=0,
    //   RELAY_1_ID,
    //   RELAY_2_ID,
    //   RELAY_3_ID,
    //   RELAY_MAX_ID
    // }; //phase out? use numbers

    const char* GetRelayNameStoredbyIDCtr(char* name_buffer, uint8_t name_buffer_space, uint8_t device_id);

    int8_t Tasker(uint8_t function);

    void MQTTConnected();

    
    
    void init(void);

    typedef unsigned long power_t;              // Power (Relay) type
    
    void SetLatchingRelay(power_t lpower, uint32_t state);
    void SetDevicePower(power_t rpower, uint32_t source);
    void RestorePower(bool publish_power, uint32_t source);   
    void SetAllPower(uint32_t state, uint32_t source);
    void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source);
    

    void SetPowerOnState(void);
    
    power_t last_power = 0;                     // Last power set state
    power_t blink_power;                        // Blink power state
    power_t blink_mask = 0;                     // Blink relay active mask
    power_t blink_powersave;                    // Blink start power save state
    power_t latching_power = 0;                 // Power state at latching start
    power_t rel_inverted = 0;                   // Relay inverted flag (1 = (0 = On, 1 = Off))

    uint8_t active_device = 1;                  // Active device in ExecuteCommandPower

    uint32_t tSavedTick = millis(),tSavedForceUpdate;
    uint8_t toggle_bit = 0;
    
  #ifndef DISABLE_WEBSERVER
    void WebAppend_Root_Status_Table();
    void WebAppend_Root_Draw_PageTable();
    void WebAppend_Root_Add_Buttons();
  #endif // DISABLE_WEBSERVER

    // uint32_t tSavedTest = millis();

    uint8_t fForceMQTTUpdate = true; //each local subtasker has its own fForce


//
  // I should add a relay "type" ie, external power, internal power, light

    #ifdef ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
    
    void SubTask_Relay_Timed_Seconds();

    #endif // ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
    

    struct RELAY_STATUS{
    #ifdef ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
      /**
       * 0 = not running
       * 1 = turn off then set to 0 to be off
       * 2+ count down
       * */
      struct TIME_DECOUNTERS{
        uint16_t seconds = 0;
        uint8_t active = false;
      }timer_decounter;
      /**
       * Records when relays are turned on/off
       * */
      struct LAST_CHANGED_TIME{
        struct datetime ontime;   //to be short_time with operators added
        struct datetime offtime;
      }last;
      /**
       * Times during the day when relays auto turn on/off
       * */
      struct SCHEDULED_AUTO_TIME_PERIODS{
        struct time_short ontime;
        struct time_short offtime;
        // Monday = bit0, Sunday = bit6, bit7 (MSB) = day_of_week enabled
        uint8_t days_of_week_enabled_bitpacked = 0x00;
        uint8_t enabled = false;
      }scheduled[3];
      struct SCHEDULED_ENABLED_TIME_PERIODS{
        struct time_short ontime;
        struct time_short offtime;
        uint8_t enabled = false;
      }enabled_ranges[3];
    #endif // ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS

      
      uint8_t ischanged = false;

    }relay_status[RELAYS_CONNECTED];
    
bool IsRelayTimeWindowAllowed(uint8_t relay_id);


    /**
     * Commands
     */
    void CommandSet_Timer_Decounter(uint8_t time_secs, uint8_t relay_id = 0);



    void CommandSet_Relay_Power(uint8_t state, uint8_t relay_id = 0);
    uint8_t CommandGet_Relay_Power(uint8_t num);

    const char* GetRelayNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);//D_DEFAULT_DEVICE_BUFFER_LENGTH);
    const char* GetRelayNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);//D_DEFAULT_DEVICE_BUFFER_LENGTH);
    int8_t GetRelayIDbyName(const char* c);

    int8_t GetDeviceIDbyName(const char* c);

    uint8_t ConstructJSON_Settings(uint8_t json_method = 0);
    uint8_t ConstructJSON_Sensor(uint8_t json_method = 0);
    uint8_t ConstructJSON_Scheduled(uint8_t json_level = 0);


    void MQTTHandler_Init();
    void MQTTHandler_Set_fSendNow();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mRelays> mqtthandler_settings_teleperiod;
    struct handler<mRelays> mqtthandler_sensor_ifchanged;
    struct handler<mRelays> mqtthandler_sensor_teleperiod;
    struct handler<mRelays> mqtthandler_scheduled_teleperiod;

    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // Sensors need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_SCHEDULED_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
    
};


// extern template void mRelays::ftest<int>();
// extern template void mRelays::ftest2<int>(int value);
// extern template void mRelays::ftest2<char>(char value);


// extern template int8_t mRelays::Tasker2<JsonObjectConst>(uint8_t function, JsonObjectConst param1);
// extern template int8_t mRelays::Tasker2<uint8_t>(uint8_t function, uint8_t param1);


// extern template void mSupport::f2<int>();
#endif

#endif
