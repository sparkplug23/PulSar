#ifndef _MWEBCAM_WEBCAM_H
#define _MWEBCAM_WEBCAM_H 0.1

#define D_UNIQUE_MODULE_DRIVERS_CAMERA_WEBCAM_ID ((4*1000)+51)

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM

#include "2_CoreSystem/01_Settings/mSettings.h"

// #include "3_Network/MQTT/mMQTT.h"

#include "2_CoreSystem/02_Time/mTime.h"

DEFINE_PGM_CTR(PM_MQTT_HANDLER_POSTFIX_TOPIC_SCHEDULED_CTR) "scheduled";


#include "1_TaskerManager/mTaskerInterface.h"

class mWebCam :
  public mTaskerInterface
{
  public:
    mWebCam(){};

    #ifndef MAX_RELAYS
      #define MAX_RELAYS 4
    #endif

    static const char* PM_MODULE_DRIVERS_CAMERA_WEBCAM_CTR;
    static const char* PM_MODULE_DRIVERS_CAMERA_WEBCAM_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_DRIVERS_CAMERA_WEBCAM_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_DRIVERS_CAMERA_WEBCAM_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_CAMERA_WEBCAM_ID; }

    
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){
      return sizeof(mWebCam);
    };
    #endif

 uint8_t    server_started = 0;
 void StartLocalServer();
    
    void Pre_Init(void);
    void Init();

    uint8_t delay_Start = 0;

    void EveryLoop();


    struct SETTINGS{
      /**
       * Shared flags 
       * */
      // RELAY_SETTINGS_FLAGS flags;

      uint8_t fShowTable = false;
      uint8_t relays_connected = 0;
      uint8_t fEnableSensor = false;
      
      uint8_t caminit = false;
    }settings;

    int8_t CheckAndExecute_JSONCommands();
    void   parse_JSONCommand(JsonParserObject obj);

#ifdef USE_MODULE_CORE_RULES
    void RulesEvent_Set_Power();

#endif// USE_MODULE_CORE_RULES


    #define RELAYS_MAX_COUNT 4

    // enum RELAY_IDS{
    //   RELAY_0_ID=0,
    //   RELAY_1_ID,
    //   RELAY_2_ID,
    //   RELAY_3_ID,
    //   RELAY_MAX_ID
    // }; //phase out? use numbers

    const char* GetRelayNameStoredbyIDCtr(char* name_buffer, uint8_t name_buffer_space, uint8_t device_id);

    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    void MQTTConnected();

void setup_cam();
    
    
    void init(void);

    // typedef unsigned long power_t;              // Power (Relay) type
    
    // void SetLatchingRelay(power_t lpower, uint32_t state);
    // void SetDevicePower(power_t rpower, uint32_t source);
    // void RestorePower(bool publish_power, uint32_t source);   
    // void SetAllPower(uint32_t state, uint32_t source);
    // void ExecuteCommandPower(uint32_t device, uint32_t state, uint32_t source);
    

    // void SetPowerOnState(void);
    
    // power_t last_power = 0;                     // Last power set state
    // power_t blink_power;                        // Blink power state
    // power_t blink_mask = 0;                     // Blink relay active mask
    // power_t blink_powersave;                    // Blink start power save state
    // power_t latching_power = 0;                 // Power state at latching start
    // power_t rel_inverted = 0;                   // Relay inverted flag (1 = (0 = On, 1 = Off))

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

    // #ifdef ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
    
    void SubTask_Relay_Time_To_Remain_On_Seconds();

    // #endif // ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
    

//     struct RELAY_STATUS{
//     // #ifdef ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS
//     #define D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT 1 // not yet working >1 for commands, needs a better jsoncommand method
//       /**
//        * 0 = not running
//        * 1 = turn off then set to 0 to be off
//        * 2+ count down
//        * */
//       struct TIME_ON_DECOUNTERS{
//         uint16_t seconds = 0;
//         uint8_t active = false;
//       }timer_decounter;     
//       /**
//        * 0 = not running
//        * 1 = turn off then set to 0 to be off
//        * 2+ count down
//        * */
//       struct TIME_OFF_THEN_ON_DECOUNTERS{
//         uint16_t seconds = 0;
//         uint8_t active = false;
//       }timer_off_then_on_decounter;
//       /**
//        * Records when relays are turned on/off
//        * */
//       struct LAST_CHANGED_TIME{
//         struct datetime ontime;   //to be short_time with operators added
//         struct datetime offtime;
//       }last;
//       /**
//        * Times during the day when relays auto turn on/off
//        * */
//       struct SCHEDULED_AUTO_TIME_PERIODS{
//         struct time_short ontime;
//         struct time_short offtime;
//         // Monday = bit0, Sunday = bit6, bit7 (MSB) = day_of_week enabled
//         uint8_t days_of_week_enabled_bitpacked = 0x00;
//         uint8_t enabled = false;
//       }scheduled[D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT];
//       struct SCHEDULED_ENABLED_TIME_PERIODS{
//         struct time_short ontime;
//         struct time_short offtime;
//         uint8_t enabled = false;
//       }enabled_ranges[D_SCHEDULED_ENABLED_TIME_PERIODS_AMOUNT];
//     // #endif // ENABLE_DEVFEATURE_ADVANCED_RELAY_CONTROLS


//       // 0 minutes if off
//       // 1+ minutes, is 0+, so time_minutes_on is really "time_minutes_on-1"
//       //uint16_t time_minutes_on = 0; //phase out, make function to get minutes from seconds

//       uint32_t time_seconds_on = 0;


     
//       uint8_t ischanged = false;

//     }relay_status[MAX_RELAYS];
    
// bool IsRelayTimeWindowAllowed(uint8_t relay_id, uint8_t range_id=255);

// void SubCommandSet_EnabledTime(JsonParserObject jobj, uint8_t relay_index = 0);

void SubTask_Every_Minute();

// void SubTask_Relay_Time_To_Briefly_Turn_Off_Then_On_Seconds();


    /**
     * Commands
     */
    void CommandSet_Timer_Decounter(uint16_t time_secs, uint8_t relay_id = 0);
    uint16_t CommandGet_SecondsToRemainOn(uint8_t relay_id = 0);

    
uint16_t CommandGet_SecondsToRemainOff(uint8_t relay_id);

    uint32_t CommandGet_SecondsRelayHasBeenOn(uint8_t relay_id = 0);
    
    void CommandSet_RelayAsRessetingDevice_TurnOffThenOnAgain(uint16_t time_secs, uint8_t relay_id);


    void CommandSet_Relay_Power(uint8_t state, uint8_t relay_id = 0);
    uint8_t CommandGet_Relay_Power(uint8_t num);

    const char* GetRelayNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);//D_DEFAULT_DEVICE_BUFFER_LENGTH);
    const char* GetRelayNameWithStateLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buffer_length);//D_DEFAULT_DEVICE_BUFFER_LENGTH);
    int8_t GetRelayIDbyName(const char* c);

    int8_t GetDeviceIDbyName(const char* c);

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_object_start_end_required = true);
    uint8_t ConstructJSON_Scheduled(uint8_t json_level = 0);


    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_TelePeriod();
    
    void MQTTHandler_Sender(uint8_t mqtt_handler_id = MQTT_HANDLER_ALL_ID);

    struct handler<mWebCam> mqtthandler_settings_teleperiod;
    struct handler<mWebCam> mqtthandler_state_ifchanged;
    struct handler<mWebCam> mqtthandler_state_teleperiod;
    struct handler<mWebCam> mqtthandler_scheduled_teleperiod;

    // Extra module only handlers
    enum MQTT_HANDLER_MODULE_IDS{  // States need ifchanged, drivers do not, just telemetry
      MQTT_HANDLER_SCHEDULED_TELEPERIOD_ID = MQTT_HANDLER_LENGTH_ID,
      MQTT_HANDLER_MODULE_LENGTH_ID, // id count
    };
      
    struct handler<mWebCam>* mqtthandler_list[4] = {
      &mqtthandler_settings_teleperiod,
      &mqtthandler_state_ifchanged,
      &mqtthandler_state_teleperiod,
      &mqtthandler_scheduled_teleperiod
    };

    
};


// extern template void mWebCam::ftest<int>();
// extern template void mWebCam::ftest2<int>(int value);
// extern template void mWebCam::ftest2<char>(char value);


// extern template int8_t mWebCam::Tasker2<JsonObjectConst>(uint8_t function, JsonObjectConst param1);
// extern template int8_t mWebCam::Tasker2<uint8_t>(uint8_t function, uint8_t param1);


// extern template void mSupport::f2<int>();
#endif

#endif
