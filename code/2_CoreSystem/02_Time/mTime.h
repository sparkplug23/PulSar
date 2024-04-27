#ifndef MTIME_H
#define MTIME_H 0.21
  
#include "1_TaskerManager/mTaskerManager.h"

#define D_UNIQUE_MODULE_CORE_TIME_ID 2002

#ifdef USE_MODULE_CORE_TIME


#include <Arduino.h>


enum WeekInMonthOptions {Last, First, Second, Third, Fourth};
enum DayOfTheWeekOptions {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum MonthNamesOptions {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};
enum HemisphereOptions {North, South};
enum GetDateAndTimeOptions { DT_LOCAL, DT_UTC, DT_LOCALNOTZ, DT_DST, DT_STD, DT_RESTART, DT_ENERGY, DT_BOOTCOUNT, DT_LOCAL_MILLIS, DT_TIMEZONE, DT_SUNRISE, DT_SUNSET, DT_LOCAL_TIME };

typedef struct datetime{
  uint8_t day_of_week; // week day [0-7]
  uint8_t day_of_month; // month day [0-30]
  uint16_t day_of_year; // year day [0-365]



  uint16_t year;  // year 20XX
  uint8_t month;  // [1-12]
  uint8_t week;   // [1-52]
  uint8_t hour;   // [0-23]
  uint8_t minute; // [0-59]
  uint8_t second; // [0-59]
  uint32_t      nanos;

  //generate these with a function START
  uint32_t Dseconds; //seconds into day from midnight [0-86400]
  uint32_t Wseconds; // seconds into week [0-604800]
  uint32_t Yseconds; // seconds into year [0-31536000]
  //generate these with a function END


  uint32_t seconds_nonreset; // seconds counting infinitely [0-1e+32]

  //PHASE OUT
  char hhmmss_ctr[10]; //phase out into function with formatting

  //from tas
  
  unsigned long days;
  unsigned long valid;


  // uint8_t isvalid = false;

  
      char          name_of_month[4];


}datetime_t;

// used for basic time info only for week period - RENAME "datetime_t"
typedef struct time_short{
  uint8_t day_of_week; // week day [0-7], weekday 1-7 is Mon to Sun, 0 is all days
  uint8_t hour;   // [0-23]
  uint8_t minute; // [0-59]
  uint8_t second; // [0-59]
}time_short_t;

// used for basic time info only for week period - RENAME "datetime_t"
typedef struct weektime{
  uint8_t  day_of_week; // week day [0-7]
  uint8_t  hour;   // [0-23]
  uint8_t  minute; // [0-59]
  uint8_t  second; // [0-59]
  uint16_t millis; // millis into 1 second [0-1000]
}weektime_t;




// 7 bytes
typedef struct TIMEREACHED_HANDLER{
  uint32_t millis = 0;
  uint8_t run = false; // run immediately
  // uint8_t interval_is_millis = true; //else seconds
  // maximum of 65 seconds (65535 millis)
  // uint16_t interval = 0;  // if not 0, then use this instead of what is passed in
}timereached_t;

#define SEC2CENT 3153600000
#define SEC2DEC 315360000
#define SEC2YR 31536000
#define SEC2MONTH 2592000
#define SEC2WEEK 604800
#define SEC2DAY 86400
#define SEC2HOUR 3600
#define SEC2MIN 60

#define SEC_IN_CENT 3153600000
#define SEC_IN_DEC 315360000
#define SEC_IN_YR 31536000
#define SEC_IN_MONTH 2592000
#define SEC_IN_WEEK 604800
#define SEC_IN_DAY 86400
#define SEC_IN_HOUR 3600
#define SEC_IN_MIN 60

enum TIME_UNITS_IDS{
  TIME_UNIT_SECONDS_ID = 0,
  TIME_UNIT_MILLISECONDS_ID
};


template <typename TIME, typename UNIT>
uint32_t ConvertTimeToMilliSecondsWithUnit(TIME time_secs, UNIT unit){
  if(unit == TIME_UNIT_SECONDS_ID){
    time_secs *= 1000;
  }
  return time_secs;
}


#include <EEPROM.h>
#define EEPROM_ADDR_REBOOT_COUNTER 0

#include <WiFiUdp.h>

#include "1_TaskerManager/mTaskerManager.h"

#include "2_CoreSystem/01_Settings/mSettings.h"

#ifdef ESP8266
  extern "C" {
    #include "sntp.h"
  }
#endif
#include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

#include "5_Sensors/22_SolarLunar/mSolarLunar.h"

static const uint8_t kDaysInMonth[] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char kMonthNamesEnglish[] PROGMEM = "JanFebMarAprMayJunJulAugSepOctNovDec";

class mTime :
  public mTaskerInterface
{

  private:
  public:
  
  /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mTime(){};
    void   Init(void);
    void   Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);
    void   parse_JSONCommand(JsonParserObject obj);
    
    static const char* PM_MODULE_CORE_TIME_CTR;
    static const char* PM_MODULE_CORE_TIME_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_TIME_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_TIME_FRIENDLY_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_TIME_ID; }
    #ifdef USE_DEBUG_CLASS_SIZE
    uint16_t GetClassSize(){      return sizeof(mTime);    };
    #endif

    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = 0;//ModuleStatus::Initialising; // Disabled,Initialise,Running
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
    
    // struct TIME_T {
    //   uint32_t      nanos;
    //   uint8_t       second;
    //   uint8_t       minute;
    //   uint8_t       hour;
    //   uint8_t       day_of_week;               // sunday is day 1
    //   uint8_t       day_of_month;
    //   uint8_t       month;
    //   char          name_of_month[4];
    //   uint16_t      day_of_year;
    //   uint16_t      year;
    //   uint32_t      days;
    //   uint32_t      valid;

      
    //   uint32_t Dseconds; //seconds into day from midnight [0-86400]
    //   uint32_t Wseconds; // seconds into week [0-604800]
    //   uint32_t Yseconds; // seconds into year [0-31536000]
    //   uint32_t seconds_nonreset; // seconds counting infinitely [0-1e+32]

    // } RtcTime;
    

        
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
    void BreakNanoTime(uint32_t time_input, uint32_t time_nanos, datetime_t &tm);
    void BreakTime(uint32_t time_input, datetime_t &tm);
    uint32_t MakeTime(datetime_t &tm);
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

    std::vector<struct handler<mTime>*> mqtthandler_list;
    struct handler<mTime> mqtthandler_settings_teleperiod;
    struct handler<mTime> mqtthandler_state_teleperiod;
    #endif // USE_MODULE_NETWORK_MQTT


























































    // mTime(){};
    void init(void);
    void UpdateStoredRTCVariables();
    void initUpTime();
    void UpdateUpTime();
    // int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    // static const char* PM_MODULE_CORE_TIME_CTR;
    // static const char* PM_MODULE_CORE_TIME_FRIENDLY_CTR;
    // PGM_P GetModuleName(){          return PM_MODULE_CORE_TIME_CTR; }
    // PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_TIME_FRIENDLY_CTR; }
    // uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_TIME_ID; }
    
    // #ifdef USE_DEBUG_CLASS_SIZE
    // uint16_t GetClassSize(){
    //   return sizeof(mTime);
    // };
    // #endif
    #ifndef DISABLE_TIME1_OLD_CODE
    void parse_JSONCommand(JsonParserObject obj);
    #endif

    void  SystemTime_Update(
          uint16_t year,
          uint8_t month,
          uint8_t day,
          uint8_t hour,
          uint8_t minute,
          uint8_t second,
          uint16_t minimum_update_seconds = 0 // 0 means force the update
        );

    void  SetUTCTime(
          uint16_t year,
          uint8_t month,
          uint8_t day,
          uint8_t hour,
          uint8_t minute,
          uint8_t second
        );

    uint32_t uptime_seconds_nonreset = 0; //test new uptime with comparison of breaktime
    uint32_t boottime_epoch;

    
    uint32_t GetTimeOfDay_Seconds(void);
    static const char* ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds, char* buffer, uint8_t buflen);
    // static const char* ConvertShortTime_HHMMSS(time_short_t* time, char* buffer, uint8_t buflen);



    
    static const char* ConvertShortTimetoCtr(time_short_t* time, char* buffer, uint8_t buflen);
    // static const char* ConvertShortTimetoCtr(time_short_t time, char* buffer, uint8_t buflen);

    static const char* ConvertU32TimetoCtr(uint32_t* _time, char* buffer, uint8_t buflen, bool flag_include_weekday = false);

    // static const char* ConvertShortTime_HHMMSS(uint32_t* _time, char* buffer, uint8_t buflen);
    static time_short_t Parse_Time_TimeShortCtr_To_TimeShort(const char* time_ctr);
    
    // Used to keep the last midnight time, this stops multiple midnight runs at the same midnight
    uint16_t lastday_run = 0;
    
    char* GetDateAndTimeCtr(uint8_t time_type, char* buffer, uint8_t buflen);

    time_short_t GetTimeShortNow();
    uint32_t GetTimeShortNowU32();

    #ifdef USE_SUNRISE
      uint32_t JulianDate(const datetime_t &now);      
      // Force value in the 0..pi2 range
      float InPi(float x);
      float TimeFormula(float *DK, uint32_t Tdays) ;
      void DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down);
      // void ApplyTimerOffsets(Timer *duskdawn);
      char* GetSunTimeAtHorizon(uint32_t dawn, char* buffer, uint8_t buflen);
      uint16_t SunMinutes(uint32_t dawn);
    #endif //  USE_SUNRISE

    #ifndef DISABLE_TIME1_OLD_CODE
    uint32_t UtcTime(void);
    #endif// DISABLE_TIME1_OLD_CODE


    uint32_t tSavedUptime;

    // uint8_t fTimeSet = false;

//     const char* GetUTCTimeCtr(char* buffer = nullptr);
//     const char* GetLocalTimeCtr(char* buffer = nullptr);
    
//     const char* GetTimeZoneCtr(char* buffer = nullptr);

    
// const char* GetStartDSTTimeCtr(char* buffer, uint8_t buflen);
// const char* GetEndDSTTimeCtr(char* buffer, uint8_t buflen);


//     const char* GetSunriseCtr(char* buffer, uint8_t buflen);
//     const char* GetSunsetCtr(char* buffer, uint8_t buflen);


    uint32_t ConvertHHMMSStoSOD(uint8_t hh, uint8_t mm, uint8_t ss);
    uint8_t CheckBetweenSOD(uint32_t start, uint32_t end);

    // #define DAYLIGHT_SAVINGS_ON
    // #ifdef DAYLIGHT_SAVINGS_ON
    //   #define NTP_OFFSET_WITH_DLS 1 * 60 * 60 // In seconds
    // #else
    //   #define NTP_OFFSET_WITHOUT_DLS 0 * 60 * 60 // In seconds
    #define NTP_OFFSET 60 * 60
    // #endif?
    bool fEnabled_DayLightSavings = false;
    #define NTP_INTERVAL 60 * 1000 // In miliseconds
    #define NTP_ADDRESS "0.pool.ntp.org" //US address
    // #define NTP_ADDRESS "europe.pool.ntp.org"
    //#define NTP_ADDRESS "0.europe.pool.ntp.org" //US address

    struct SETTINGS{
      uint8_t timeclient_is_started = false;
    }settings;

    bool CheckOrStartNTPService();


    #ifdef ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2
    void SystemTime_NTPUpdate();
    bool SystemTime_NTPUpdate_GetNTPTime(uint32_t* ntp_time);


    #endif // ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2


    void TickRTCVariablesWithUptime();

    
void RtcSync(void);

    // WiFiUDP ntpUDP;
    // NTPClient* timeClient;//(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

    void ResetRebootCounter();

    #ifndef DISABLE_TIME1_OLD_CODE
    uint32_t UpTime(void);
    #endif // DISABLE_TIME1_OLD_CODE
    int8_t Tasker_InternetTime(void);

    enum MONTH_NUM{MONTH_JANUARY=1,MONTH_FEBRUARY,MONTH_MARCH,MONTH_APRIL,MONTH_MAY,MONTH_JUNE,MONTH_JULY,MONTH_AUGUST,MONTH_SEPTEMBER,MONTH_OCTOBER,MONTH_NOVEMBER,MONTH_DECEMBER};
    //enum WDAY_NUM{WDAY_MONDAY=1,MONTH_FEBRUARY,MONTH_MARCH,MONTH_APRIL,MONTH_MAY,MONTH_JUNE,MONTH_JULY,MONTH_AUGUST,MONTH_SEPTEMBER,MONTH_OCTOBER,MONTH_NOVEMBER,MONTH_DECEMBER};

    uint16_t GetDayOfYear(uint8_t day, uint8_t month);
    uint16_t GetDaysInMonth(uint8_t month);
    datetime_t GetDifferenceInDateTimes(datetime_t* dt1, datetime_t* dt2);

    void PrintDateTime(datetime_t dt);
    void PrintDateTime(time_short_t dt);
    void DateTimeWeek2HHMMSS(datetime_t* dt, uint8_t* hour, uint8_t* minute, uint8_t* second);

    uint8_t CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end);
    uint8_t CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end);
    int8_t  CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end);

    const char* getFormattedUptime(char* buffer, uint8_t buflen);

    // #ifndef DISABLE_TIME1_OLD_CODE
    // datetime_t RtcTime;
    datetime_t RtcTime;
    // #endif // DISABLE_TIME1_OLD_CODE


    static bool TimeReached(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeReached(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
    static bool TimeReached(TIMEREACHED_HANDLER* tSaved);
    static bool MillisReached(uint32_t* tTarget);
    static bool TimeReachedNonReset(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
    static uint32_t MillisElapsed(uint32_t* tSaved);
    static uint32_t MillisElapsed(uint32_t tSaved);

    timereached_t testtime;

    // /*********************************************************************************************\
    //  * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
    //  *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
    // \*********************************************************************************************/

    #ifndef DISABLE_TIME1_OLD_CODE
    const uint32_t SECS_PER_MIN = 60UL;
    const uint32_t SECS_PER_HOUR = 3600UL;
    const uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;
    const uint32_t MINS_PER_HOUR = 60UL;

    void RtcSecond();
    
    
typedef union {
  uint16_t data;
  struct {
    uint16_t hemis : 1;                    // bit 0        = 0=Northern, 1=Southern Hemisphere (=Opposite DST/STD)
    uint16_t week : 3;                     // bits 1 - 3   = 0=Last week of the month, 1=First, 2=Second, 3=Third, 4=Fourth
    uint16_t month : 4;                    // bits 4 - 7   = 1=Jan, 2=Feb, ... 12=Dec
    uint16_t dow : 3;                      // bits 8 - 10  = day of week, 1=Sun, 2=Mon, ... 7=Sat
    uint16_t hour : 5;                     // bits 11 - 15 = 0-23
  };
} TimeRule;
uint32_t RuleToTime(TimeRule r, int yr);
TimeRule tflag[2];
int16_t       toffset[2];                // 30E

    #endif // DISABLE_TIME1_OLD_CODE


    uint32_t GetUTCTime();

    #ifndef DISABLE_TIME1_OLD_CODE
// void BreakTime(uint32_t time_input, struct datetime_t &tm);
// uint32_t MakeTime(struct datetime_t &tm);
uint32_t MakeTime(datetime_t &tm);

uint32_t RtcMillis(void) ;

void RtcSetTime(uint32_t epoch);

uint32_t MinutesUptime(void);
String GetUptime(void);
uint32_t MinutesPastMidnight(void);

String GetDuration(uint32_t time);

void WifiPollNtp() ;
uint32_t WifiGetNtp(void);
    #endif // DISABLE_TIME1_OLD_CODE

    #define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))
    
    Ticker* TickerRtc = nullptr;

    uint8_t kDaysInMonth[12];// = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
    // char kMonthNamesEnglish[37];// = "JanFebMarAprMayJunJulAugSepOctNovDec";

    #ifndef DISABLE_TIME1_OLD_CODE
void BreakTime(uint32_t time_input, datetime_t &tm);

uint8_t hour(uint32_t time);
uint8_t minute(uint32_t time);
uint8_t second(uint32_t time);
int hourFormat12(datetime_t t);

struct RTC {
  uint32_t utc_time = 0;
  uint32_t local_time = 0;
  uint32_t daylight_saving_time = 0;
  uint32_t standard_time = 0;
  uint32_t ntp_time = 0;  //previous local time?
  uint32_t midnight = 0;
  uint32_t restart_time = 0;
  uint32_t millis = 0;
  uint32_t last_sync = 0;
  int32_t time_timezone = 0;
  uint8_t ntp_sync_minute = 0;
  bool midnight_now = false;
  bool user_time_entry = false;               // Override NTP by user setting

  uint32_t ntp_last_active = 0;
  
} Rtc;
    #endif // DISABLE_TIME1_OLD_CODE


    // uint32_t utc_time = 0;
    // uint32_t local_time = 0;
    // uint32_t daylight_saving_time = 0;
    // uint32_t standard_time = 0;
    // uint32_t ntp_time = 0;
    // uint32_t midnight = 0;
    // uint32_t restart_time = 0;
    // int32_t  time_timezone = 0;
    // uint8_t  midnight_now = 0;
    // uint8_t  ntp_sync_minute = 0;
    
    #ifndef DISABLE_TIME1_OLD_CODE
    uint32_t LocalTime(void);
    uint32_t Midnight(void);
    
    bool  IsDst(void);


    void RtcInit(void);
    void RtcPreInit(void);
    #endif // DISABLE_TIME1_OLD_CODE


    datetime_t GetTimefromCtr(const char* c);

    uint16_t DaysInThisMonth();

    unsigned char ntp_attempt_count;

    // struct UPTIME{
    //   uint8_t seconds;
    //   uint8_t minute;
    //   uint16_t hours;
    //   uint16_t days;
    //   uint8_t rebootcount;
    //   uint32_t nonresetseconds=0;
    //   char ddhhmmss_ctr[13];
    // }uptime;

    struct REBOOTINFO{
      uint8_t count=0;
    }rebootinfo;


    datetime_t uptime;

    uint8_t CheckBetweenDateTimes(datetime_t* start, datetime_t* end);
    bool IsShortTimeWithinRange(time_short start, time_short end);

    uint8_t AddSecondsToDateTime(datetime_t* start, uint32_t seconds);

    uint8_t CheckDateTimeWeekIsNow(datetime_t* dt, uint8_t window = 0);

    uint32_t GetSecondsOfDayFromDateTime(datetime_t* dt_t);

    const char* getFormattedTime(datetime_t* dt_t, char* buffer, uint8_t buflen);
    const char* getFormattedDateTime(datetime_t* dt_t, char* buffer, uint8_t buflen);
    
    const char* GetBuildDateAndTime(char* buffer, uint8_t buflen);
    const char* GetDT(uint32_t time, char* buffer, uint8_t buflen);

    const char* GetDOWShortctr(uint8_t dow, char* buffer);
    const char* GetDOWLongctr(uint8_t dow, char* buffer);

    unsigned char tSavedMinute;
    unsigned char tSavedMinuteHeatingTimers;
    int cSavedMinutes;//counts minutes
    int tSavedMinuteTemps;
    unsigned long tSavedStoreRTCUpdate;

};

#endif // USE_MODULE_CORE_TIME

#endif // header
