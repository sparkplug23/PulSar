#ifndef _MRELAYS_H
#define _MRELAYS_H

#define D_UNIQUE_MODULE_DRIVERS_RELAY_ID 4004  // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_RELAY

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SCHEDULED_CTR) "scheduled";

#define D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT 1 // not yet working >1 for commands, needs a better jsoncommand method

class mRelays :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mRelays(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static const char* PM_MODULE_DRIVERS_RELAY_CTR;
    static const char* PM_MODULE_DRIVERS_RELAY_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_RELAY_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_RELAY_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_RELAY_ID; }    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mRelays);    };
    #endif    

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    #if defined(ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__DRIVERS___RELAYS) && defined(USE_MODULE_CORE_FILESYSTEM)
    void Load_Module(bool erase = false);
    void Save_Module(void);
    bool Restore_Module(void);
    #endif // USE_MODULE_CORE_FILESYSTEM

    typedef unsigned long power_t;              // Power (Relay) type

    #ifndef MAX_RELAYS
      #define MAX_RELAYS 4 // Phase out, should be detected with pin set
      #warning "MAX_RELAYS Phase out, should be detected with pin set"
    #endif

    /**
     * @brief Needs to be renamed, "runtime" should not be
     **/
    struct MODULE_RUNTIME{ // these will be saved and recovered on boot

      typedef union RELAY_SETTINGS_FLAGS{
        uint16_t data; // allows full manipulating
        struct { 
          // enable seconds on
          uint16_t enabled_timer_decounters : 1;
          // enable seconds on
          uint16_t enabled_scheduled_autocontrol : 1;
          // enable seconds on
          uint16_t enabled_relays_allowed_time_window_checks : 1;
        };
      };
      RELAY_SETTINGS_FLAGS flags;

      struct BIT_PACKED_STATE{
        power_t last_power = 0;                     // Last power set state
        power_t blink_power;                        // Blink power state
        power_t blink_mask = 0;                     // Blink relay active mask
        power_t blink_powersave;                    // Blink start power save state
        power_t latching_power = 0;                 // Power state at latching start
        power_t rel_inverted = 0;                   // Relay inverted flag (1 = (0 = On, 1 = Off))
      }bitpacked;

      struct RELAY_STATUS
      {        
        uint32_t time_seconds_on = 0;
        /**
         * 0 = not running
         * 1 = turn off then set to 0 to be off
         * 2+ count down
         * */
        struct TIME_ON_DECOUNTERS{
          uint16_t seconds = 0;
          uint8_t active = false;
        }timer_decounter;     
        /**
         * 0 = not running
         * 1 = turn off then set to 0 to be off
         * 2+ count down
         * */
        struct TIME_OFF_THEN_ON_DECOUNTERS{
          uint16_t seconds = 0;
          uint8_t active = false;
        }timer_off_then_on_decounter;
        /**
         * Records when relays are turned on/off
         * */
        struct LAST_CHANGED_TIME{
          struct datetime ontime;   //to be short_time with operators added
          struct datetime offtime;
        }last;
        uint8_t ischanged = false;
        /**
         * Times during the day when relays auto turn on/off
         * */
        struct SCHEDULED_AUTO_TIME_PERIODS{
          struct time_short ontime;
          struct time_short offtime;          
          uint8_t days_of_week_enabled_bitpacked = 0x00; // Monday = bit0, Sunday = bit6, bit7 (MSB) = day_of_week enabled
          uint8_t enabled = false;
        }scheduled[D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT];
        struct SCHEDULED_ENABLED_TIME_PERIODS{
          struct time_short ontime;
          struct time_short offtime;
          uint8_t enabled = false;
        }enabled_ranges[D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT];

      }relay_status[MAX_RELAYS]; // Anything saved must have fixed size. If not, all data is lost and set to default.

    }rt;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    #ifdef USE_MODULE_CORE_RULES
    void RulesEvent_Set_Power();
    #endif// USE_MODULE_CORE_RULES
    const char* GetRelayNameStoredbyIDCtr(char* name_buffer, uint8_t name_buffer_space, uint8_t device_id);
    int8_t CheckAndExecute_JSONCommands();
    void SetLatchingRelay(power_t lpower, uint32_t state);
    void SetDevicePower(power_t rpower, uint32_t source);
    void RestorePower(bool publish_power, uint32_t source);   
    void SetAllPower(uint32_t state, uint32_t source);
    void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source);
    void SetPowerOnState(void);
    void SubTask_Relay_TimeOn();     
    bool IsRelayTimeWindowAllowed(uint8_t relay_id, uint8_t range_id=255);
    void SubTask_Every_Minute();
    void SubTask_Relay_PulseOff();

    void EverySecond();
    void SubTask_Relay_CycleTimer();


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    void CommandSet_Timer_Decounter(uint16_t time_secs, uint8_t relay_id = 0);
    uint16_t CommandGet_SecondsToRemainOn(uint8_t relay_id = 0);
    uint16_t CommandGet_SecondsToRemainOff(uint8_t relay_id);
    uint32_t CommandGet_SecondsRelayHasBeenOn(uint8_t relay_id = 0);   
    void CommandSet_RelayAsRessetingDevice_TurnOffThenOnAgain(uint16_t time_secs, uint8_t relay_id);
    void CommandSet_Relay_Power(uint8_t state, uint8_t relay_id = 0);
    uint8_t CommandGet_Relay_Power(uint8_t num);
    const char* GetRelayNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);
    const char* GetRelayNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);
    int8_t GetRelayIDbyName(const char* c);
    int8_t GetDeviceIDbyName(const char* c);
    void SubCommandSet_EnabledTime(JsonParserObject jobj, uint8_t relay_index = 0);

    void CommandSet_PowerCycle_Interval();
    void CommandSet_PowerTimeOnLimit_Until_Reset();

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true); //default is true
    uint8_t ConstructJSON_Scheduled(uint8_t json_level = 0, bool json_appending = true);

    uint8_t AppendJSONResponse_Drivers_Unified();

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mRelays>*> mqtthandler_list;
    struct handler<mRelays> mqtthandler_settings_teleperiod;
    struct handler<mRelays> mqtthandler_state_ifchanged;
    struct handler<mRelays> mqtthandler_state_teleperiod;
    struct handler<mRelays> mqtthandler_scheduled_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT
    
};

#endif

#endif
