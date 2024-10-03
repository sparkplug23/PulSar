#ifndef _mSensorColourBar_H
#define _mSensorColourBar_H

#define D_UNIQUE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__ID 9042  // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR


class mSensorColourBar :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSensorColourBar(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);

    static constexpr const char* PM_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__CTR = D_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__ID; }    
        

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void Load_Module(bool erase);
    void Save_Module(void);
    bool Default_Module(void);

    typedef unsigned long power_t;              // Power (Relay) type

    #ifndef MAX_RELAYS
      #define MAX_RELAYS 4 // Phase out, should be detected with pin set
      #warning "MAX_RELAYS Phase out, should be detected with pin set"
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

    struct MODULE_RUNTIME{ // these will be saved and recovered on boot

      RELAY_SETTINGS_FLAGS flags;

      power_t last_power = 0;                     // Last power set state
      power_t blink_power;                        // Blink power state
      power_t blink_mask = 0;                     // Blink relay active mask
      power_t blink_powersave;                    // Blink start power save state
      power_t latching_power = 0;                 // Power state at latching start
      power_t rel_inverted = 0;                   // Relay inverted flag (1 = (0 = On, 1 = Off))


      struct RELAY_STATUS{
      // #ifdef ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
      #define D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT 1 // not yet working >1 for commands, needs a better jsoncommand method
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
        /**
         * Times during the day when relays auto turn on/off
         * */
        struct SCHEDULED_AUTO_TIME_PERIODS{
          struct time_short ontime;
          struct time_short offtime;
          // Monday = bit0, Sunday = bit6, bit7 (MSB) = day_of_week enabled
          uint8_t days_of_week_enabled_bitpacked = 0x00;
          uint8_t enabled = false;
        }scheduled[D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT];
        struct SCHEDULED_ENABLED_TIME_PERIODS{
          struct time_short ontime;
          struct time_short offtime;
          uint8_t enabled = false;
        }enabled_ranges[D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT];
      // #endif // ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS


        // 0 minutes if off
        // 1+ minutes, is 0+, so time_minutes_on is really "time_minutes_on-1"
        //uint16_t time_minutes_on = 0; //phase out, make function to get minutes from seconds

        uint32_t time_seconds_on = 0;
        uint8_t ischanged = false;

      }relay_status[MAX_RELAYS]; // Anything saved must have fixed size. If not, all data is lost and set to default.

    }rt;


    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    void EverySecond();


    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/
    
    

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true);

    uint8_t AppendJSONResponse_Drivers_Unified();

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mSensorColourBar>*> mqtthandler_list;
    struct handler<mSensorColourBar> mqtthandler_settings;
    struct handler<mSensorColourBar> mqtthandler_state_ifchanged;
    struct handler<mSensorColourBar> mqtthandler_state_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT
    
};

#endif

#endif
