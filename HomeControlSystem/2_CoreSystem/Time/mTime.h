#ifndef MTIME_H
#define MTIME_H 0.21
  
#include <Arduino.h>




typedef struct datetime{
  uint8_t Wday; // week day [0-7]
  uint8_t Mday; // month day [0-30]
  uint16_t Yday; // year day [0-365]
  uint16_t year;  // year 20XX
  uint8_t month;  // [1-12]
  uint8_t week;   // [1-52]
  uint8_t hour;   // [0-23]
  uint8_t minute; // [0-59]
  uint8_t second; // [0-59]

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


  uint8_t isvalid = false;
}datetime_t;

// used for basic time info only for week period - RENAME "time_t"
typedef struct time_short{
  uint8_t Wday; // week day [0-7]
  uint8_t hour;   // [0-23]
  uint8_t minute; // [0-59]
  uint8_t second; // [0-59]
}time_short_t;

// used for basic time info only for week period - RENAME "time_t"
typedef struct weektime{
  uint8_t  Wday; // week day [0-7]
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

#include "2_CoreSystem/Settings/mSettings.h"

#ifdef ESP8266
  extern "C" {
    #include "sntp.h"
  }
#endif
#include <Ticker.h>

#include "1_TaskerManager/mTaskerInterface.h"

class mTime :
  public mTaskerInterface
{

  private:
  public:
    mTime(){};
    void init(void);
    void UpdateStoredRTCVariables();
    void initUpTime();
    void UpdateUpTime();
    int8_t Tasker(uint8_t function);

    static const char* PM_MODULE_CORE_TIME_CTR;
    static const char* PM_MODULE_CORE_TIME_FRIENDLY_CTR;
    PGM_P GetModuleName(){          return PM_MODULE_CORE_TIME_CTR; }
    PGM_P GetModuleFriendlyName(){  return PM_MODULE_CORE_TIME_FRIENDLY_CTR; }


    uint32_t uptime_seconds_nonreset = 0; //test new uptime with comparison of breaktime
    uint32_t boottime_epoch;

    
    uint32_t GetTimeOfDay_Seconds(void);
    static const char* ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds, char* buffer, uint8_t buflen);
    static const char* ConvertShortTime_HHMMSS(time_short_t* time, char* buffer, uint8_t buflen);
    static time_short_t Parse_Time_TimeShortCtr_To_TimeShort(const char* time_ctr);
    
    // Used to keep the last midnight time, this stops multiple midnight runs at the same midnight
    uint16_t lastday_run = 0;
    
    char* GetDateAndTimeCtr(uint8_t time_type, char* buffer, uint8_t buflen);

    time_short_t GetTimeShortNow();

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


    uint32_t tSavedUptime;

    uint8_t fTimeSet = false;

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
    void TickRTCVariablesWithUptime();

    
void RtcSync(void);

    // WiFiUDP ntpUDP;
    // NTPClient* timeClient;//(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

    void ResetRebootCounter();

    uint32_t UpTime(void);
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

    // datetime_t RtcTime;
    datetime_t RtcTime;


    static bool TimeReached(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime);
    static bool TimeReached(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
    static bool TimeReached(TIMEREACHED_HANDLER* tSaved);
    static bool MillisReached(uint32_t* tTarget);
    static bool TimeReachedNonReset(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime);
    static uint32_t MillisElapsed(uint32_t* tSaved);

    timereached_t testtime;

    // /*********************************************************************************************\
    //  * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
    //  *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
    // \*********************************************************************************************/

    const uint32_t SECS_PER_MIN = 60UL;
    const uint32_t SECS_PER_HOUR = 3600UL;
    const uint32_t SECS_PER_DAY = SECS_PER_HOUR * 24UL;
    const uint32_t MINS_PER_HOUR = 60UL;

    void RtcSecond();
    
uint32_t RuleToTime(TimeRule r, int yr);

// void BreakTime(uint32_t time_input, struct TIME_T &tm);
// uint32_t MakeTime(struct TIME_T &tm);
uint32_t MakeTime(datetime_t &tm);

uint32_t RtcMillis(void) ;

void RtcSetTime(uint32_t epoch);

uint32_t MinutesUptime(void);
String GetUptime(void);
uint32_t MinutesPastMidnight(void);

String GetDuration(uint32_t time);

void WifiPollNtp() ;
uint32_t WifiGetNtp(void);

    #define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))
    
    #ifdef ESP8266
    Ticker* TickerRtc = nullptr;
    #endif //esp8266
    #ifdef ESP32
    uint32_t tSavedTicker_ESP32 = millis();
    #endif // ESP32

    uint8_t kDaysInMonth[12];// = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
    // char kMonthNamesEnglish[37];// = "JanFebMarAprMayJunJulAugSepOctNovDec";

void BreakTime(uint32_t time_input, datetime_t &tm);

uint8_t hour(uint32_t time);
uint8_t minute(uint32_t time);
uint8_t second(uint32_t time);
int hourFormat12(time_t t);

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
    
    uint32_t LocalTime(void);
    uint32_t Midnight(void);
    
    bool  IsDst(void);


    void RtcInit(void);


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

    uint8_t fTimeValid = false;

    datetime_t uptime;

    uint8_t CheckBetweenDateTimes(datetime_t* start, datetime_t* end);

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
#endif
