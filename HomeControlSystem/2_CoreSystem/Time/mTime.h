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
  uint32_t Dseconds; //seconds into day from midnight [0-86400]
  uint32_t Wseconds; // seconds into week [0-604800]
  uint32_t Yseconds; // seconds into year [0-31536000]
  uint32_t seconds_nonreset; // seconds counting infinitely [0-1e+32]
  char hhmmss_ctr[10]; //phase out into function with formatting
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

#include <EEPROM.h>
#define EEPROM_ADDR_REBOOT_COUNTER 0

#include "NTPClient.h"
#include <WiFiUdp.h>

#include "1_TaskerManager/mTaskerManager.h"


#include "TimeLib.h"
#ifdef ESP8266
  extern "C" {
    #include "sntp.h"
  }
#endif
#include <Ticker.h>


class mTime{

  private:
  public:
    mTime(){};
    void init(void);
    void UpdateStoredRTCVariables();
    void initUpTime();
    void UpdateUpTime();
    int8_t Tasker(uint8_t function);

    uint32_t uptime_seconds_nonreset = 0; //test new uptime with comparison of breaktime
    uint32_t boottime_epoch;
    
    // Used to keep the last midnight time, this stops multiple midnight runs at the same midnight
    uint16_t lastday_run = 0;
    
    const char* GetDateAndTime(uint8_t time_type);

    uint32_t tSavedUptime;

    uint8_t fTimeSet = false;

    const char* GetUTCTimeCtr(char* buffer = nullptr);
    const char* GetLocalTimeCtr(char* buffer = nullptr);
    const char* GetStartDSTTimeCtr(char* buffer = nullptr);
    const char* GetEndDSTTimeCtr(char* buffer = nullptr);
    const char* GetTimeZoneCtr(char* buffer = nullptr);
    const char* GetSunriseCtr(char* buffer = nullptr);
    const char* GetSunsetCtr(char* buffer = nullptr);


    uint32_t ConvertHHMMSStoSOD(uint8_t hh, uint8_t mm, uint8_t ss);
    uint8_t CheckBetweenSOD(uint32_t start, uint32_t end);

    #define DAYLIGHT_SAVINGS_ON
    #ifdef DAYLIGHT_SAVINGS_ON
      #define NTP_OFFSET 1 * 60 * 60 // In seconds
    #else
      #define NTP_OFFSET 0 * 60 * 60 // In seconds
    #endif
    bool fEnabled_DayLightSavings = 1;
    #define NTP_INTERVAL 60 * 1000 // In miliseconds
    #define NTP_ADDRESS "0.pool.ntp.org" //US address
    //#define NTP_ADDRESS "0.europe.pool.ntp.org" //US address

    WiFiUDP ntpUDP;
    NTPClient* timeClient;//(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

    void ResetRebootCounter();

    uint32_t UpTime(void);
    int8_t Tasker_InternetTime(void);

    enum MONTH_NUM{MONTH_JANUARY=1,MONTH_FEBRUARY,MONTH_MARCH,MONTH_APRIL,MONTH_MAY,MONTH_JUNE,MONTH_JULY,MONTH_AUGUST,MONTH_SEPTEMBER,MONTH_OCTOBER,MONTH_NOVEMBER,MONTH_DECEMBER};
    //enum WDAY_NUM{WDAY_MONDAY=1,MONTH_FEBRUARY,MONTH_MARCH,MONTH_APRIL,MONTH_MAY,MONTH_JUNE,MONTH_JULY,MONTH_AUGUST,MONTH_SEPTEMBER,MONTH_OCTOBER,MONTH_NOVEMBER,MONTH_DECEMBER};

    uint16_t GetDayOfYear(uint8_t day, uint8_t month);
    uint16_t GetDaysInMonth(uint8_t month);
    datetime_t GetDifferenceInDateTimes(datetime_t* dt1, datetime_t* dt2);

    void PrintDateTime(datetime_t* dt);
    void DateTimeWeek2HHMMSS(datetime_t* dt, uint8_t* hour, uint8_t* minute, uint8_t* second);

    uint8_t CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end);
    uint8_t CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end);
    int8_t  CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end);

    const char* getFormattedUptime(char* buffer);

    datetime_t mtime;


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

    #define LEAP_YEAR(Y)  (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))
    
    Ticker TickerRtc;

    uint8_t kDaysInMonth[12];// = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
    char kMonthNamesEnglish[37];// = "JanFebMarAprMayJunJulAugSepOctNovDec";

    uint32_t utc_time = 0;
    uint32_t local_time = 0;
    uint32_t daylight_saving_time = 0;
    uint32_t standard_time = 0;
    uint32_t ntp_time = 0;
    uint32_t midnight = 0;
    uint32_t restart_time = 0;
    int32_t  time_timezone = 0;
    uint8_t  midnight_now = 0;
    uint8_t  ntp_sync_minute = 0;
    
    uint32_t LocalTime(void);
    uint32_t Midnight(void);
    uint32_t MinutesPastMidnight(void);
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

    const char* getFormattedTime(datetime_t* dt_t, char* buffer);// = nullptr);
    const char* getFormattedDateTime(datetime_t* dt_t, char* buffer);// = nullptr);
    
    const char* GetBuildDateAndTime(char* buffer);// = nullptr);
    const char* GetDT(uint32_t time, char* buffer);// = nullptr);

    const char* GetDOWShortctr(uint8_t dow, char* buffer);// = nullptr);
    const char* GetDOWLongctr(uint8_t dow, char* buffer);// = nullptr);

    unsigned char tSavedMinute;
    unsigned char tSavedMinuteHeatingTimers;
    int cSavedMinutes;//counts minutes
    int tSavedMinuteTemps;
    unsigned long tSavedStoreRTCUpdate;

};
#endif
