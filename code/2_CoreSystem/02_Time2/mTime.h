#ifndef MTIME_2_H
#define MTIME_2_H
  
#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_CORE_TIME_ID 2002

#ifdef USE_MODULE_CORE_TIME2

class mTime2 :
  public mTaskerInterface
{

  private:
  public:
  
  /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mTime2(){};
    void   Init(void);
    void   Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);
    
    static const char* PM_MODULE_CORE_TIME2_CTR;
    static const char* PM_MODULE_CORE_TIME2_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_TIME2_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_TIME2_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_TIME_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mTime2);    };
    #endif

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    struct DATA_STORAGE
    {

    }
    dt;


    struct DATA_RUNTIME
    {

    }
    rt;


    /*********************************************************************************************\
     * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
     *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
    \*********************************************************************************************/

    const uint32_t SECS_PER_MIN = 60UL;
    const uint32_t SECS_PER_HOUR = 3600UL;
    const uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;
    const uint32_t MINS_PER_HOUR = 60UL;

    #define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

    struct RTC {
      uint32_t utc_time = 0;
      uint32_t local_time = 0;
      uint32_t daylight_saving_time = 0;
      uint32_t standard_time = 0;
      uint32_t midnight = 0;
      uint32_t restart_time = 0;
      uint32_t nanos = 0;
      uint32_t millis = 0;
    //  uint32_t last_sync = 0;
      int32_t time_timezone = 0;
      bool time_synced = false;
      bool last_synced = false;
      bool midnight_now = false;
      bool user_time_entry = false;               // Override NTP by user setting
    } Rtc;
    
    struct TIME_T {
      uint32_t      nanos;
      uint8_t       second;
      uint8_t       minute;
      uint8_t       hour;
      uint8_t       day_of_week;               // sunday is day 1
      uint8_t       day_of_month;
      uint8_t       month;
      char          name_of_month[4];
      uint16_t      day_of_year;
      uint16_t      year;
      uint32_t      days;
      uint32_t      valid;
    } RtcTime;
    

        
    typedef union {                            // Restricted by MISRA-C Rule 18.4 but so useful...
      uint16_t data;                           // Allow bit manipulation
      struct {
        uint16_t system_init : 1;              // Changing layout here needs adjustments in xdrv_10_rules.ino too
        uint16_t system_boot : 1;
        uint16_t time_init : 1;
        uint16_t time_set : 1;
        uint16_t mqtt_connected : 1;
        uint16_t mqtt_disconnected : 1;
        uint16_t wifi_connected : 1;
        uint16_t wifi_disconnected : 1;
        uint16_t eth_connected : 1;
        uint16_t eth_disconnected : 1;
        uint16_t http_init : 1;
        uint16_t shutter_moved : 1;
        uint16_t shutter_moving : 1;
        uint16_t spare13 : 1;
        uint16_t spare14 : 1;
        uint16_t spare15 : 1;
      };
    } RulesBitfield;

    struct TasmotaGlobal_t {
      uint32_t uptime;                          // Counting every second until 4294967295 = 130 year
      RulesBitfield rules_flag;                 // Rule state flags (16 bits)

      bool ntp_force_sync;                      // Force NTP sync
    } TasmotaGlobal;


    uint32_t UtcTime(void);
    uint32_t LocalTime(void);
    uint32_t Midnight(void);
    bool MidnightNow(void);
    bool IsDst(void);
    String GetBuildDateAndTime(void);
    String GetSyslogDate(char* mxtime);
    String GetDate(void);
    String GetMinuteTime(uint32_t minutes);
    String GetTimeZone(void);
    String GetDuration(uint32_t time);
    String GetDT(uint32_t time);
    String GetDateAndTime(uint8_t time_type);
    uint32_t UpTime(void);
    uint32_t MinutesUptime(void);
    String GetUptime(void);
    uint32_t MinutesPastMidnight(void);
    uint32_t RtcMillis(void);
    void BreakNanoTime(uint32_t time_input, uint32_t time_nanos, TIME_T &tm);
    void BreakTime(uint32_t time_input, TIME_T &tm);
    uint32_t MakeTime(TIME_T &tm);
    uint32_t RuleToTime(TimeRule r, int yr);
    void RtcGetDaylightSavingTimes(uint32_t local_time);
    uint32_t RtcTimeZoneOffset(uint32_t local_time);
    void RtcSetTimeOfDay(uint32_t local_time);
    void RtcSecond(void);
    void RtcSync(const char* source);
    void RtcSetTime(uint32_t epoch);
    void RtcInit(void);
    void RtcPreInit(void);
    void WifiPollNtp();
    uint64_t WifiGetNtp(void);
    inline int32_t TimeDifference(uint32_t prev, uint32_t next);
    int32_t TimePassedSince(uint32_t timestamp);
    bool TimeReached(uint32_t timer);

    
    uint8_t hour(uint32_t time);
    uint8_t minute(uint32_t time);
    uint8_t second(uint32_t time);
    int hourFormat12(time_t t);


    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    uint8_t ConstructJSON_Settings(uint8_t json_method = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_method = 0, bool json_appending = true);

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_Set_RefreshAll();
    void MQTTHandler_Set_DefaultPeriodRate();    
    void MQTTHandler_Sender();

    std::vector<struct handler<mTime2>*> mqtthandler_list;
    struct handler<mTime2> mqtthandler_settings_teleperiod;
    struct handler<mTime2> mqtthandler_state_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT



};


#endif // USE_MODULE_CORE_TIME2

#endif // MTIME_2_H
