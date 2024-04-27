#include "mTime.h"

#ifdef USE_MODULE_CORE_TIME

const char* mTime::PM_MODULE_CORE_TIME_CTR = D_MODULE_CORE_TIME_CTR;
const char* mTime::PM_MODULE_CORE_TIME_FRIENDLY_CTR = D_MODULE_CORE_TIME_FRIENDLY_CTR;

// int8_t mTime::Tasker(uint8_t function, JsonParserObject obj)
// {

//   switch(function){
//     case FUNC_INIT:
//       init();
//       testtime.millis = millis();
//       testtime.run = false;
//     break;
//   }

//   switch(function){
//     case FUNC_EVERY_SECOND:{
      
//       UpdateStoredRTCVariables();
//       UpdateUpTime();
      
//       #ifndef DISABLE_SERIAL0_CORE
//         #ifdef DEBUG_MODULE_TIME_STD
//         char buffer[40];
//         AddLog(LOG_LEVEL_TEST, PSTR("DT_DST=%s"), pCONT_time->GetDateAndTimeCtr(DT_DST, buffer, sizeof(buffer)));
//         AddLog(LOG_LEVEL_TEST, PSTR("DT_STD=%s"), pCONT_time->GetDateAndTimeCtr(DT_STD, buffer, sizeof(buffer)));
//         AddLog(LOG_LEVEL_TEST, PSTR("DT_TIMEZONE=%s IsDst=%d"), pCONT_time->GetDateAndTimeCtr(DT_TIMEZONE, buffer, sizeof(buffer)), IsDst());
//         #endif
//       #endif

//       #ifdef ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME
//       Serial.println(GetUptime());
//       #endif // ENABLE_FEATURE_EVERY_SECOND_SPLASH_UPTIME

//     }break;
//     case FUNC_EVERY_FIVE_SECOND:

//       #ifdef ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2

//       SystemTime_NTPUpdate();

//       #else
//       #ifdef USE_MODULE_NETWORK_WIFI
//       // if(pCONT_interface_network->Connected(mInterfaceNetwork::NETWORK_TYPE_WIFI))
//       // {
// #ifndef ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
//         WifiPollNtp();
// #endif // ENABLE_DEVFEATURE__WIFI_BLOCK_BAD_CODE_TEST
//       // }
//       #endif // USE_MODULE_NETWORK_WIFI
//       #endif // ENABLE_DEVFEATURE__TIME_NTP_UPDATE_WITH_VERSION2

//     break;
//     case FUNC_EVERY_MINUTE:


//     break;
//   }

// }//end







#include <Ticker.h>

Ticker TickerRtc2;

static const uint8_t kDaysInMonth2[] PROGMEM = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // API starts months from 1, this array starts from 0
static const char kMonthNamesEnglish2[] PROGMEM = "JanFebMarAprMayJunJulAugSepOctNovDec";

const uint32_t START_VALID_TIME = 1451602800;  // Time is synced and after 2016-01-01

/**
 * @brief: Module Interface
 * */
int8_t mTime::Tasker(uint8_t function, JsonParserObject obj)
{

  int8_t function_result = 0;

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:
      // EverySecond();
      WifiPollNtp();

      
      UpdateStoredRTCVariables();
      UpdateUpTime();
    break;
    /************
     * STORAGE SECTION * 
    *******************/  
    /************
     * COMMANDS SECTION * 
    *******************/
    /************
     * RULES SECTION * 
    *******************/
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init(); //make a FUNC_MQTT_INIT and group mqtt togather
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate(); // Load teleperiod setting into local handlers
    break; 
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif // USE_MODULE_NETWORK_MQTT
  } // end switch
} // END function



void mTime::Pre_Init(void){}

void mTime::Init(void)
{
  module_state.mode = ModuleStatus::Running;
  RtcInit();


  
  tSavedStoreRTCUpdate = millis()+2000;
  
  memset(&uptime,0,sizeof(uptime));

  kDaysInMonth[0] = 31;
  kDaysInMonth[1] = 28;
  kDaysInMonth[2] = 31;
  kDaysInMonth[3] = 30;
  kDaysInMonth[4] = 31;
  kDaysInMonth[5] = 30;
  kDaysInMonth[6] = 31;
  kDaysInMonth[7] = 31;
  kDaysInMonth[8] = 30;
  kDaysInMonth[9] = 31;
  kDaysInMonth[10] = 30;
  kDaysInMonth[11] = 31;



}




uint32_t mTime::UtcTime(void) {
  return Rtc.utc_time;
}

uint32_t mTime::LocalTime(void) {
  return Rtc.local_time;
}

uint32_t mTime::Midnight(void) {
  return Rtc.midnight;
}

bool mTime::MidnightNow(void) {
  if (Rtc.midnight_now) {
    Rtc.midnight_now = false;
    return true;
  }
  return false;
}

bool mTime::IsDst(void) {
  return (Rtc.time_timezone == pCONT_set->Settings.toffset[1]);
}

String mTime::GetBuildDateAndTime(void) {
  // "2017-03-07T11:08:02" - ISO8601:2004
  char bdt[21];
  char *p;
  static const char mdate_P[] PROGMEM = __DATE__;  // "Mar  7 2017"
  char mdate[strlen_P(mdate_P)+1];      // copy on stack first
  strcpy_P(mdate, mdate_P);
  char *smonth = mdate;
  int day = 0;
  int year = 0;

  // sscanf(mdate, "%s %d %d", bdt, &day, &year);  // Not implemented in 2.3.0 and probably too much code
  uint8_t i = 0;
  for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(nullptr, " ", &p)) {
    switch (i++) {
    case 0:  // Month
      smonth = str;
      break;
    case 1:  // Day
      day = atoi(str);
      break;
    case 2:  // Year
      year = atoi(str);
    }
  }
  char MonthNamesEnglish[sizeof(kMonthNamesEnglish2)];
  strcpy_P(MonthNamesEnglish, kMonthNamesEnglish2);
  int month = (strstr(MonthNamesEnglish, smonth) -MonthNamesEnglish) /3 +1;
  snprintf_P(bdt, sizeof(bdt), PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"), year, month, day, PSTR(__TIME__));
  return String(bdt);  // 2017-03-07T11:08:02
}

String mTime::GetSyslogDate(char* mxtime) {
  // Mmm dd hh:mm:ss
  // Jan  3 09:23:45
  // Assuming the day hasn't changed yet ;-)
  uint32_t month_idx = (RtcTime.month -1) * 3;
  char month[4] = { 0 };
  strncpy_P(month, kMonthNamesEnglish + month_idx, 3);
  char dt[16];
  snprintf_P(dt, sizeof(dt), PSTR("%s %2d %s"), month, RtcTime.day_of_month, mxtime);
  return String(dt);
}

String mTime::GetDate(void) {
  // yyyy-mm-ddT
  char dt[12];
  snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT"), RtcTime.year, RtcTime.month, RtcTime.day_of_month);
  return String(dt);
}

String mTime::GetMinuteTime(uint32_t minutes) {
  char tm[6];
  snprintf_P(tm, sizeof(tm), PSTR("%02d:%02d"), minutes / 60, minutes % 60);

  return String(tm);  // 03:45
}

String mTime::GetTimeZone(void) {
  char tz[7];
  snprintf_P(tz, sizeof(tz), PSTR("%+03d:%02d"), Rtc.time_timezone / 60, abs(Rtc.time_timezone % 60));

  return String(tz);  // -03:45
}

String mTime::GetDuration(uint32_t time) {
  char dt[16];

  datetime_t ut;
  BreakTime(time, ut);

  // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
//  snprintf_P(dt, sizeof(dt), PSTR("P%dDT%02dH%02dM%02dS"), ut.days, ut.hour, ut.minute, ut.second);

  // "128 14:35:44" - OpenVMS
  // "128T14:35:44" - Tasmota
  snprintf_P(dt, sizeof(dt), PSTR("%dT%02d:%02d:%02d"), ut.days, ut.hour, ut.minute, ut.second);

  return String(dt);  // 128T14:35:44
}

String mTime::GetDT(uint32_t time) {
  // "2017-03-07T11:08:02" - ISO8601:2004

  char dt[20];
  datetime_t tmpTime;

  BreakTime(time, tmpTime);
  snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    tmpTime.year +1970, tmpTime.month, tmpTime.day_of_month, tmpTime.hour, tmpTime.minute, tmpTime.second);

  return String(dt);  // 2017-03-07T11:08:02
}

/*
 * timestamps in https://en.wikipedia.org/wiki/ISO_8601 format
 *
 *  DT_UTC     - current data and time in Greenwich, England (aka GMT)
 *  DT_LOCAL   - current date and time taking timezone into account
 *  DT_RESTART - the date and time this device last started, in local timezone
 *
 * Format:
 *  "2017-03-07T11:08:02-07:00" - if DT_LOCAL and SetOption52 = 1
 *  "2017-03-07T11:08:02"       - otherwise
 */
String mTime::GetDateAndTime(uint8_t datetime_type) {
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = Rtc.local_time;
  

  if(datetime_type == DT_LOCAL_TIME){
    // make another GetTime (no date function)
    time = Rtc.local_time;
    char dt[20];
    datetime_t tmpTime;
    BreakTime(time, tmpTime);
    snprintf_P(dt, sizeof(dt), PSTR("%02d:%02d:%02d"), tmpTime.hour, tmpTime.minute, tmpTime.second);
    return String(dt);  // 11:08:02
  }  


  switch (datetime_type) {
    case DT_LOCAL_TIME:
      time = Rtc.local_time;
      break;
    case DT_UTC:
      time = Rtc.utc_time;
      break;
//    case DT_LOCALNOTZ:  // Is default anyway but allows for not appendig timezone
//      time = Rtc.local_time;
//      break;
    case DT_DST:
      time = Rtc.daylight_saving_time;
      break;
    case DT_STD:
      time = Rtc.standard_time;
      break;
    case DT_RESTART:
      if (Rtc.restart_time == 0) {
        return "";
      }
      time = Rtc.restart_time;
      break;
    case DT_BOOTCOUNT:
      time = pCONT_set->Settings.bootcount_reset_time;
      break;
  }
  String dt = GetDT(time);  // 2017-03-07T11:08:02

  if (DT_LOCAL_MILLIS == datetime_type) {
    char ms[10];
    snprintf_P(ms, sizeof(ms), PSTR(".%03d"), RtcMillis());
    dt += ms;               // 2017-03-07T11:08:02.123
    datetime_type = DT_LOCAL;
  }

  if (DT_UTC == datetime_type) {
    dt += "Z";              // 2017-03-07T11:08:02.123Z
  }
  else if (pCONT_set->Settings.flag3.time_append_timezone && (DT_LOCAL == datetime_type)) {  // SetOption52 - Append timezone to JSON time
    dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
  }
  return dt;                // 2017-03-07T11:08:02-07:00 or 2017-03-07T11:08:02.123-07:00
}

uint32_t mTime::UpTime(void) {
  if (Rtc.restart_time) {
    return Rtc.utc_time - Rtc.restart_time;
  } else {
    return TasmotaGlobal.uptime;
  }
}

uint32_t mTime::MinutesUptime(void) {
  return (UpTime() / 60);
}

String mTime::GetUptime(void) {
  return GetDuration(UpTime());
}

uint32_t mTime::MinutesPastMidnight(void) {
  uint32_t minutes = 0;

  if (RtcTime.valid) {
    minutes = (RtcTime.hour *60) + RtcTime.minute;
  }
  return minutes;
}

uint32_t mTime::RtcMillis(void) {
  return (millis() - Rtc.millis) % 1000;
}

void mTime::BreakNanoTime(uint32_t time_input, uint32_t time_nanos, datetime_t &tm) {
// break the given time_input into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

  time_input += time_nanos / 1000000000U;
  tm.nanos = time_nanos % 1000000000U;

  uint8_t year;
  uint8_t month;
  uint8_t month_length;
  uint32_t time;
  unsigned long days;

  time = time_input;
  tm.second = time % 60;
  time /= 60;                // now it is minutes
  tm.minute = time % 60;
  time /= 60;                // now it is hours
  tm.hour = time % 24;
  time /= 24;                // now it is days
  tm.days = time;
  tm.day_of_week = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.year = year;            // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  tm.day_of_year = time;

  for (month = 0; month < 12; month++) {
    if (1 == month) { // february
      if (LEAP_YEAR(year)) {
        month_length = 29;
      } else {
        month_length = 28;
      }
    } else {
      month_length = pgm_read_byte(&kDaysInMonth[month]);
    }

    if (time >= month_length) {
      time -= month_length;
    } else {
      break;
    }
  }
  strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
  tm.month = month + 1;      // jan is month 1
  tm.day_of_month = time + 1;         // day of month
  tm.valid = (time_input > START_VALID_TIME);  // 2016-01-01
}

void mTime::BreakTime(uint32_t time_input, datetime_t &tm) {
  BreakNanoTime(time_input, 0, tm);
}

uint32_t mTime::MakeTime(datetime_t &tm) {
// assemble time elements into datetime_t
// note year argument is offset from 1970

  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds = tm.year * (SECS_PER_DAY * 365);
  for (i = 0; i < tm.year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < tm.month; i++) {
    if ((2 == i) && LEAP_YEAR(tm.year)) {
      seconds += SECS_PER_DAY * 29;
    } else {
      seconds += SECS_PER_DAY * pgm_read_byte(&kDaysInMonth[i-1]);  // monthDay array starts from 0
    }
  }
  seconds+= (tm.day_of_month - 1) * SECS_PER_DAY;
  seconds+= tm.hour * SECS_PER_HOUR;
  seconds+= tm.minute * SECS_PER_MIN;
  seconds+= tm.second;
  return seconds;
}

uint32_t mTime::GetUTCTime()
{
  return Rtc.utc_time;
}

uint32_t mTime::RuleToTime( TimeRule r, int yr) {
  datetime_t tm;
  uint32_t t;
  uint8_t m;
  uint8_t w;                // temp copies of r.month and r.week

  m = r.month;
  w = r.week;
  if (0 == w) {             // Last week = 0
    if (++m > 12) {         // for "Last", go to the next month
      m = 1;
      yr++;
    }
    w = 1;                  // and treat as first week of next month, subtract 7 days later
  }

  tm.hour = r.hour;
  tm.minute = 0;
  tm.second = 0;
  tm.day_of_month = 1;
  tm.month = m;
  tm.year = yr - 1970;
  t = MakeTime(tm);         // First day of the month, or first day of next month for "Last" rules
  BreakTime(t, tm);
  t += (7 * (w - 1) + (r.dow - tm.day_of_week + 7) % 7) * SECS_PER_DAY;
  if (0 == r.week) {
    t -= 7 * SECS_PER_DAY;  // back up a week if this is a "Last" rule
  }
  return t;
}

void mTime::RtcGetDaylightSavingTimes(uint32_t local_time) {
  datetime_t tmpTime;
  BreakTime(local_time, tmpTime);
  tmpTime.year += 1970;


  pCONT_set->Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  pCONT_set->Settings.tflag[0].week = TIME_STD_WEEK;
  pCONT_set->Settings.tflag[0].dow = TIME_STD_DAY;
  pCONT_set->Settings.tflag[0].month = TIME_STD_MONTH;
  pCONT_set->Settings.tflag[0].hour = TIME_STD_HOUR;
  pCONT_set->Settings.toffset[0] = TIME_STD_OFFSET;

  pCONT_set->Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  pCONT_set->Settings.tflag[1].week = TIME_DST_WEEK;
  pCONT_set->Settings.tflag[1].dow = TIME_DST_DAY;
  pCONT_set->Settings.tflag[1].month = TIME_DST_MONTH;
  pCONT_set->Settings.tflag[1].hour = TIME_DST_HOUR;
  pCONT_set->Settings.toffset[1] = TIME_DST_OFFSET;


  Rtc.daylight_saving_time = RuleToTime(pCONT_set->Settings.tflag[1], tmpTime.year);
  Rtc.standard_time = RuleToTime( pCONT_set->Settings.tflag[0], tmpTime.year);

  // ALOG_HGL(PSTR("RtcGetDaylightSavingTimes: %s %s"), GetDT(Rtc.daylight_saving_time).c_str(), GetDT(Rtc.standard_time).c_str());

}

uint32_t mTime::RtcTimeZoneOffset(uint32_t local_time) {
  
  // ALOG_INF(PSTR(D_LOG_TIME2 "RtcTimeZoneOffset"));
  //FORCED TO TEST
  pCONT_set->Settings.toffset[1] = 60;//FORCED TO TEST
  pCONT_set->Settings.toffset[0] = 0;//FORCED TO TEST

  int16_t timezone_minutes = pCONT_set->Settings.timezone_minutes2;
  if (pCONT_set->Settings.timezone2 < 0) { timezone_minutes *= -1; }
  uint32_t timezone = (pCONT_set->Settings.timezone2 * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
  // ALOG_INF(PSTR(D_LOG_TIME2 "RtcTimeZoneOffset timezone %d"), timezone);
  
  if (99 == pCONT_set->Settings.timezone2) 
  {
    int32_t dstoffset = pCONT_set->Settings.toffset[1] * SECS_PER_MIN;
    int32_t stdoffset = pCONT_set->Settings.toffset[0] * SECS_PER_MIN;
    if (pCONT_set->Settings.tflag[1].hemis) {
      // Southern hemisphere
      if (
          (local_time >= (Rtc.standard_time        - dstoffset)) && 
          (local_time <  (Rtc.daylight_saving_time - stdoffset))
      ){
        timezone = stdoffset;  // Standard Time
      } else {
        timezone = dstoffset;  // Daylight Saving Time
      }
    } else {
      // Northern hemisphere
      if (
          (local_time >= (Rtc.daylight_saving_time - stdoffset)) && 
          (local_time <  (Rtc.standard_time        - dstoffset))
      ){
        timezone = dstoffset;  // Daylight Saving Time
      } else {
        timezone = stdoffset;  // Standard Time
      }
    }
  }

  return timezone;
}

void mTime::RtcSetTimeOfDay(uint32_t local_time) {
  // Sync Core/RTOS time to be used by file system time stamps
  struct timeval tv;
  tv.tv_sec = local_time;
  tv.tv_usec = 0;
  settimeofday(&tv, nullptr);
}

void mTime::RtcSecond(void) {
  static uint32_t last_sync = 0;
  static bool mutex = false;

  if (mutex) { return; }

  if (Rtc.time_synced) {
    mutex = true;

    Rtc.time_synced = false;
    Rtc.last_synced = true;
    last_sync = Rtc.utc_time;

    if (Rtc.restart_time == 0) {
      Rtc.restart_time = Rtc.utc_time - TasmotaGlobal.uptime;  // save first synced time as restart time
    }

    RtcGetDaylightSavingTimes(Rtc.utc_time);

    AddLog(LOG_LEVEL_INFO, PSTR("RTC: " D_UTC_TIME " %s, " D_DST_TIME " %s, " D_STD_TIME " %s"),
      GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());

    if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
      TasmotaGlobal.rules_flag.time_init = 1;
    } else {
      TasmotaGlobal.rules_flag.time_set = 1;
    }
  } else {
    if (Rtc.last_synced) {
      Rtc.last_synced = false;
      uint32_t nanos = Rtc.nanos + (millis() - Rtc.millis) * 1000000U;
      Rtc.utc_time += nanos / 1000000000U;
      Rtc.nanos = nanos % 1000000000U;
    } else
      Rtc.utc_time++;  // Increment every second
  }
  Rtc.millis = millis();

  if ((Rtc.utc_time > (2 * 60 * 60)) && (last_sync < Rtc.utc_time - (2 * 60 * 60))) {  // Every two hours a warning
    AddLog(LOG_LEVEL_INFO, PSTR("RTC: Not synced"));
    last_sync = Rtc.utc_time;
  }

  if (Rtc.utc_time > START_VALID_TIME) {  // 2016-01-01
      Rtc.time_timezone = RtcTimeZoneOffset(Rtc.utc_time);
    Rtc.local_time = Rtc.utc_time + Rtc.time_timezone;
    Rtc.time_timezone /= 60;
    if (pCONT_set->Settings.bootcount_reset_time < START_VALID_TIME) {
      pCONT_set->Settings.bootcount_reset_time = Rtc.local_time;
    }
  } else {
    Rtc.local_time = Rtc.utc_time;
  }

  BreakNanoTime(Rtc.local_time, Rtc.nanos, RtcTime);
  if (RtcTime.valid) {
    if (!Rtc.midnight) {
      Rtc.midnight = Rtc.local_time - (RtcTime.hour * 3600) - (RtcTime.minute * 60) - RtcTime.second;
    }
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) {
      Rtc.midnight = Rtc.local_time;
      Rtc.midnight_now = true;
    }

    if (mutex) {  // Time is just synced and is valid
      // Sync Core/RTOS time to be used by file system time stamps
      RtcSetTimeOfDay(Rtc.local_time);
    }
  }

  RtcTime.year += 1970;

  mutex = false;
}

void mTime::RtcSync(const char* source) {

  Rtc.time_synced = true;
  RtcSecond();
  AddLog(LOG_LEVEL_INFO, PSTR("RTC: Synced by %s"), source);

  pCONT->Tasker_Interface(FUNC_TIME_SYNCED);
  
}

void mTime::RtcSetTime(uint32_t epoch) {
  if (epoch < START_VALID_TIME) {  // 2016-01-01
    Rtc.user_time_entry = false;
    TasmotaGlobal.ntp_force_sync = true;
  } else {
    Rtc.user_time_entry = true;
//    Rtc.utc_time = epoch -1;    // Will be corrected by RtcSecond
    Rtc.utc_time = epoch;
    RtcSync("Time");
  }
}

void mTime::RtcInit(void) 
{

  ALOG_HGL(PSTR("================================================================== RTCINIT =================================================================="));


  Rtc.utc_time = 0;
  BreakTime(Rtc.utc_time, RtcTime);
  // TickerRtc.attach(1, RtcSecond);
  // TickerRtc.attach(1, 
  //     [this](void){
  //       this->RtcSecond();
  //     }
  //   );
  TickerRtc2.attach_ms(1000, 
      +[](mTime* instance){ instance->RtcSecond(); }, this
    );

  if (pCONT_set->Settings.cfg_timestamp > START_VALID_TIME) {
    // Fix file timestamp while utctime is not synced
    uint32_t utc_time = pCONT_set->Settings.cfg_timestamp;
    if (RtcSettings.utc_time > utc_time) {
      utc_time = RtcSettings.utc_time;
    }
    utc_time++;
    RtcGetDaylightSavingTimes(utc_time);
    uint32_t local_time = utc_time + RtcTimeZoneOffset(utc_time);
    RtcSetTimeOfDay(local_time);
   AddLog(LOG_LEVEL_INFO, PSTR("RTC: Timestamp %s"), GetDT(local_time).c_str());
  }
}

void mTime::RtcPreInit(void) {
  Rtc.millis = millis();
}

inline int32_t mTime::TimeDifference(uint32_t prev, uint32_t next)
{
  return ((int32_t) (next - prev));
}

int32_t mTime::TimePassedSince(uint32_t timestamp)
{
  // Compute the number of milliSeconds passed since timestamp given.
  // Note: value can be negative if the timestamp has not yet been reached.
  return TimeDifference(timestamp, millis());
}

bool mTime::TimeReached(uint32_t timer)
{
  // Check if a certain timeout has been reached.
  const long passed = TimePassedSince(timer);
  return (passed >= 0);
}


void mTime::WifiPollNtp() {
  static uint8_t ntp_sync_minute = 0;
  static uint32_t ntp_run_time = 0;

    ALOG_INF(PSTR("WifiPollNtp: pCONT_set->runtime.global_state.wifi_down || Rtc.user_time_entry %d %d"), pCONT_set->runtime.global_state.wifi_down, Rtc.user_time_entry);
  if (pCONT_set->runtime.global_state.wifi_down || Rtc.user_time_entry) { 
    
    ALOG_INF(PSTR("WifiPollNtp: pCONT_set->runtime.global_state.wifi_down || Rtc.user_time_entry %d %d"), pCONT_set->runtime.global_state.wifi_down, Rtc.user_time_entry);
    return; }

    TasmotaGlobal.ntp_force_sync = true; // tmp, forced to test

  uint8_t uptime_minute = (TasmotaGlobal.uptime / 60) % 60;  // 0 .. 59
  if ((ntp_sync_minute > 59) && (uptime_minute > 2)) {
    ntp_sync_minute = 1;                 // If sync prepare for a new cycle
  }
  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
  uint8_t offset = (TasmotaGlobal.uptime < 30) ? RtcTime.second + ntp_run_time : (((mSupportHardware::ESP_getChipId() & 0xF) * 3) + 3) ;

  if ( (((offset == RtcTime.second) && ( (RtcTime.year < 2016) ||                  // Never synced
                                         (ntp_sync_minute == uptime_minute))) ||   // Re-sync every hour
       TasmotaGlobal.ntp_force_sync ) ) {                                          // Forced sync

    TasmotaGlobal.ntp_force_sync = false;

    AddLog(LOG_LEVEL_INFO, PSTR("NTP: Sync time..."));

    ntp_run_time = millis();
    uint64_t ntp_nanos = WifiGetNtp();
    uint32_t ntp_time = ntp_nanos / 1000000000;
    ntp_run_time = (millis() - ntp_run_time) / 1000;

    AddLog(LOG_LEVEL_INFO, PSTR("NTP: Runtime %d"), ntp_run_time);

    if (ntp_run_time < 5) { ntp_run_time = 0; }  // DNS timeout is around 10s

    ALOG_INF(PSTR("ntp_time %d"), ntp_time);
    ALOG_INF(PSTR("START_VALID_TIME %d"), START_VALID_TIME);

    if (ntp_time > START_VALID_TIME) {
    ALOG_INF(PSTR("ntp_time > START_VALID_TIME %d"), START_VALID_TIME);
      Rtc.utc_time = ntp_time;
      Rtc.nanos = ntp_nanos % 1000000000;
      ntp_sync_minute = 60;             // Sync so block further requests
      RtcSync("NTP");
    } else {
      ntp_sync_minute++;                // Try again in next minute
    }
  }
}

uint64_t mTime::WifiGetNtp(void) 
{
  static uint8_t ntp_server_id = 0;

 AddLog(LOG_LEVEL_INFO, PSTR("NTP: Start NTP Sync %d ..."), ntp_server_id);

  IPAddress time_server_ip;

  char fallback_ntp_server[2][32];
  ext_snprintf_P(fallback_ntp_server[0], sizeof(fallback_ntp_server[0]), PSTR("%_I"), pCONT_set->Settings.ipv4_address[1]);  // #17984
  ext_snprintf_P(fallback_ntp_server[1], sizeof(fallback_ntp_server[1]), PSTR("%d.pool.ntp.org"), random(0,3));

  char* ntp_server;
  for (uint32_t i = 0; i < MAX_NTP_SERVERS +2; i++) {
    if (ntp_server_id >= MAX_NTP_SERVERS +2) { ntp_server_id = 0; }
    ntp_server = (ntp_server_id < MAX_NTP_SERVERS) ? pCONT_set->SettingsText(SET_NTPSERVER1 + ntp_server_id) : fallback_ntp_server[ntp_server_id - MAX_NTP_SERVERS];
    if (strlen(ntp_server)) {
      break;
    }
    ntp_server_id++;
  }
  if (!pCONT_wif->WifiHostByName(ntp_server, time_server_ip)) {
    ntp_server_id++;
    AddLog(LOG_LEVEL_DEBUG, PSTR("NTP: Unable to resolve '%s'"), ntp_server);
    return 0;
  }
  
  WiFiUDP udp;

  uint32_t attempts = 3;
  while (attempts > 0) {
    uint32_t port = random(1025, 65535);   // Create a random port for the UDP connection.

    #ifdef USE_IPV6
    if (udp.begin(IPAddress(IPv6), port) != 0)
    #else
    if (udp.begin(port) != 0) 
    #endif
    {
      break;
    }
    attempts--;
  }
  if (0 == attempts) { return 0; }

  while (udp.parsePacket() > 0) {          // Discard any previously received packets
    yield();
  }

  const uint32_t NTP_PACKET_SIZE = 48;     // NTP time is in the first 48 bytes of message
  uint8_t packet_buffer[NTP_PACKET_SIZE];  // Buffer to hold incoming & outgoing packets
  memset(packet_buffer, 0, NTP_PACKET_SIZE);
  packet_buffer[0]  = 0b11100011;          // LI, Version, Mode
  packet_buffer[1]  = 0;                   // Stratum, or type of clock
  packet_buffer[2]  = 6;                   // Polling Interval
  packet_buffer[3]  = 0xEC;                // Peer Clock Precision
  packet_buffer[12] = 49;
  packet_buffer[13] = 0x4E;
  packet_buffer[14] = 49;
  packet_buffer[15] = 52;

  if (udp.beginPacket(time_server_ip, 123) == 0) {  // NTP requests are to port 123
    ntp_server_id++;                                // Next server next time
    udp.stop();
    return 0;
  }
  udp.write(packet_buffer, NTP_PACKET_SIZE);
  udp.endPacket();

  uint32_t begin_wait = millis();
  while (!TimeReached(begin_wait + 1000)) {         // Wait up to one second
    uint32_t size        = udp.parsePacket();
    uint32_t remote_port = udp.remotePort();

    if ((size >= NTP_PACKET_SIZE) && (remote_port == 123)) {
      udp.read(packet_buffer, NTP_PACKET_SIZE);     // Read packet into the buffer
      udp.stop();

      if ((packet_buffer[0] & 0b11000000) == 0b11000000) {
        // Leap-Indicator: unknown (clock unsynchronized)
        // See: https://github.com/letscontrolit/ESPEasy/issues/2886#issuecomment-586656384
        AddLog(LOG_LEVEL_INFO, PSTR("NTP: IP %_I unsynced"), (uint32_t)time_server_ip);
        ntp_server_id++;                            // Next server next time
        return 0;
      }

      // convert four bytes starting at location 40 to a long integer
      // TX time is used here.
      uint32_t secs_since_1900 = (uint32_t)packet_buffer[40] << 24;
      secs_since_1900 |= (uint32_t)packet_buffer[41] << 16;
      secs_since_1900 |= (uint32_t)packet_buffer[42] << 8;
      secs_since_1900 |= (uint32_t)packet_buffer[43];
      if (0 == secs_since_1900) {                   // No time stamp received
        ntp_server_id++;                            // Next server next time
        return 0;
      }
      uint32_t tmp_fraction = (uint32_t)packet_buffer[44] << 24;
      tmp_fraction |= (uint32_t)packet_buffer[45] << 16;
      tmp_fraction |= (uint32_t)packet_buffer[46] << 8;
      tmp_fraction |= (uint32_t)packet_buffer[47];
      uint32_t fraction = (((uint64_t)tmp_fraction) * 1000000000) >> 32;
      return (((uint64_t)secs_since_1900) - 2208988800UL) * 1000000000 + fraction;
    }
    delay(10);
  }
  // Timeout.
  AddLog(LOG_LEVEL_INFO, PSTR("NTP: No reply from %_I"), (uint32_t)time_server_ip);
  udp.stop();
  ntp_server_id++;                                  // Next server next time
  return 0;
}




/**
 * Breaking time to return parts of time
 * */
uint8_t mTime::hour(uint32_t time){
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.hour;
}
uint8_t mTime::minute(uint32_t time){
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.minute;
}
uint8_t mTime::second(uint32_t time){
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.second;
}
int mTime::hourFormat12(time_t time) { // the hour for the given time in 12 hour format
  // refreshCache(t);
  datetime_t time_temp;
  BreakTime(time, time_temp);
  if( time_temp.hour == 0 )
    return 12; // 12 midnight
  else if( time_temp.hour  > 12)
    return time_temp.hour - 12 ;
  else
    return time_temp.hour ;
}







uint8_t mTime::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  JBI->Start();

    JBI->Add("test", 123);

  JBI->End();

}


uint8_t mTime::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100];

  if(json_appending)
  {
    JBI->Start();
  }
  
  
  JBI->Add("LOCAL", GetDateAndTime(DT_LOCAL).c_str());
  JBI->Add("UTC", GetDateAndTime(DT_UTC).c_str());
  JBI->Add("LOCALNOTZ", GetDateAndTime(DT_LOCALNOTZ).c_str());
  JBI->Add("DST", GetDateAndTime(DT_DST).c_str());
  JBI->Add("STD", GetDateAndTime(DT_STD).c_str());
  JBI->Add("RESTART", GetDateAndTime(DT_RESTART).c_str());
  JBI->Add("ENERGY", GetDateAndTime(DT_ENERGY).c_str());
  JBI->Add("BOOTCOUNT", GetDateAndTime(DT_BOOTCOUNT).c_str());
  JBI->Add("LOCAL_MILLIS", GetDateAndTime(DT_LOCAL_MILLIS).c_str());
  JBI->Add("TIMEZONE", GetDateAndTime(DT_TIMEZONE).c_str());
  JBI->Add("SUNRISE", GetDateAndTime(DT_SUNRISE).c_str());
  JBI->Add("SUNSET", GetDateAndTime(DT_SUNSET).c_str());



  JBI->Add(D_JSON_UPTIME, GetUptime().c_str());
  JBI->Add("Time", GetDateAndTime(DT_LOCAL).c_str());
  JBI->Add(D_JSON_STARTUPUTC, GetDateAndTime(DT_RESTART).c_str());
  JBI->Add("BCResetTime", GetDateAndTime(DT_BOOTCOUNT).c_str());

  JBI->Add("Rtc.utc_time", Rtc.utc_time);

  JBI->Add("UtcTime", UtcTime());
  JBI->Add("LocalTime", LocalTime());
  JBI->Add("Midnight", Midnight());
  JBI->Add("MidnightNow", MidnightNow());
  JBI->Add("IsDst", IsDst());


  if(json_appending)
  {
    return JBI->End();
  }
  return JBI->Length();

}


#ifdef USE_MODULE_NETWORK_MQTT

void mTime::MQTTHandler_Init()
{

  struct handler<mTime>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTime::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mTime::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);


} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mTime::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mTime::MQTTHandler_Set_DefaultPeriodRate()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mTime::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CORE_TIME_ID, handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

























#endif // USE_MODULE_CORE_TIME


