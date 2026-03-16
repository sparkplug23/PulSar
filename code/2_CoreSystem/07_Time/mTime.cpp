#include "mTime.h"

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
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:    
      
      #ifdef ENABLE_DEBUGFEATURE_TIME__SHOW_UPTIME_EVERY_SECOND
      char up[16];
      ALOG_INF(PSTR("Uptime: %s"), tkr_time->GetUptime(up, sizeof(up)));
      #endif

      
      if(tkr_interface_network->Network_HasExternalConnectivity())
      {
        WifiPollNtp();
      }
      
      uptime_seconds_nonreset++;
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef ENABLE_DEBUGFEATURE_TIME__MQTT_DIRECT_PUBLISH_WITHOUT_TELEMETRY
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break; 
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif // USE_MODULE_NETWORK_MQTT
    #endif // ENABLE_DEBUGFEATURE_TIME__MQTT_DIRECT_PUBLISH_WITHOUT_TELEMETRY
  } // end switch

  return TASKER_RESULT__UNKNOWN_ID;
  
} // END function



time_t mTime::ConvertToUTCTime(int year, int month, int day, int hour, int min, int sec) {
    struct tm timeinfo = { 0 };
    timeinfo.tm_year = year - 1900;  // tm_year is years since 1900
    timeinfo.tm_mon  = month - 1;    // tm_mon is 0-based
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min  = min;
    timeinfo.tm_sec  = sec;
  
    // Use mktime assuming system is set to UTC (or ignore timezone entirely)
    time_t t = mktime(&timeinfo);
    return t;
  }



void mTime::Pre_Init(void)
{
  Rtc.millis = millis();
  RtcTime.valid = false;
}


uint32_t mTime::UtcTime(void) 
{
  return Rtc.utc_time;
}


uint32_t mTime::LocalTime(void) 
{
  return Rtc.local_time;
}


uint32_t mTime::Midnight(void) 
{
  return Rtc.midnight;
}


bool mTime::MidnightNow(void) 
{
  if (Rtc.midnight_now) {
    Rtc.midnight_now = false;
    return true;
  }
  return false;
}


bool mTime::IsDst(void) 
{
  return (Rtc.time_timezone == tkr_set->Settings.toffset[1]);
}

// // mktime works only with local time
// time_t mTime::GetStartOfDayUTC(time_t utc_time) {
//     // Convert to a tm structure in UTC
//     struct tm* time_info = gmtime(&utc_time);

//     // Reset the hour, minute, and second to zero (midnight)
//     time_info->tm_hour = 0;
//     time_info->tm_min = 0;
//     time_info->tm_sec = 0;

//     // Convert back to time_t (UTC time)
//     return mktime(time_info);
// }
#include <ctime>

// time_t my_timegm(struct tm* time_info) {
//     // Save the current timezone settings
//     time_t local_time;

//     // Save the current timezone settings
//     char* tz = getenv("TZ");
//     setenv("TZ", "UTC", 1);
//     tzset();

//     // Convert the tm structure to time_t (UTC)
//     local_time = mktime(time_info);

//     // Restore the original timezone settings
//     if (tz) {
//         setenv("TZ", tz, 1);
//     } else {
//         unsetenv("TZ");
//     }
//     tzset();

//     return local_time;
// }


// time_t mTime::GetStartOfDayUTC(time_t utc_time) {
//     // Convert to a tm structure in UTC
//     struct tm* time_info = gmtime(&utc_time);

//     // Reset the hour, minute, and second to zero (midnight UTC)
//     time_info->tm_hour = 0;
//     time_info->tm_min = 0;
//     time_info->tm_sec = 0;

//     // Use timegm() to convert back to UTC time_t
//     return my_timegm(time_info);  // Use timegm() instead of mktime()
// }
time_t mTime::GetStartOfDayUTC(time_t utc_time)
{
  // utc_time is seconds since epoch (UTC). Midnight UTC is just day truncation.
  const time_t day = (time_t)86400;
  if (utc_time >= 0) return (utc_time / day) * day; // basically get remainder of "current" day
  // handle negative epochs safely
  return -(((-utc_time + day - 1) / day) * day);
}


String mTime::GetBuildDateAndTime(void) 
{
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
  char MonthNames[sizeof(kMonthNames)];
  strcpy_P(MonthNames, kMonthNames);
  int month = (strstr(MonthNames, smonth) -MonthNames) /3 +1;
  snprintf_P(bdt, sizeof(bdt), PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"), year, month, day, PSTR(__TIME__));
  return String(bdt);  // 2017-03-07T11:08:02
}


String mTime::GetSyslogDate(char* mxtime) {
  // Mmm dd hh:mm:ss
  // Jan  3 09:23:45
  // Assuming the day hasn't changed yet ;-)
  uint32_t month_idx = (RtcTime.month -1) * 3;
  char month[4] = { 0 };
  strncpy_P(month, kMonthNames + month_idx, 3);
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


String mTime::GetMinuteTime(uint32_t minutes) 
{
  char tm[6];
  snprintf_P(tm, sizeof(tm), PSTR("%02d:%02d"), minutes / 60, minutes % 60);
  return String(tm);  // 03:45
}


String mTime::GetTimeZone(void) 
{
  char tz[7];
  snprintf_P(tz, sizeof(tz), PSTR("%+03d:%02d"), Rtc.time_timezone / 60, abs(Rtc.time_timezone % 60));
  return String(tz);  // -03:45
}


bool mTime::GetDuration(char* out, size_t out_len, uint32_t seconds)
{
  // "128T14:35:44" worst case fits easily in <16, you used 16 already
  if (!out || out_len < 16) { if(out && out_len) out[0]=0; return false; }

  datetime_t ut;
  BreakTime(seconds, ut);
  int n = snprintf(out, out_len, "%uT%02u:%02u:%02u",
                   (unsigned)ut.days, (unsigned)ut.hour, (unsigned)ut.minute, (unsigned)ut.second);
  if (n < 0 || (size_t)n >= out_len) { out[0]=0; return false; }
  return true;
}


String mTime::GetDT(uint32_t time) 
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  char dt[20];
  datetime_t tmpTime;
  BreakTime(time, tmpTime);
  snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    tmpTime.year +1970, tmpTime.month, tmpTime.day_of_month, tmpTime.hour, tmpTime.minute, tmpTime.second);
  return String(dt);  // 2017-03-07T11:08:02
}


String mTime::GetTime(uint8_t time_type)
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = Rtc.local_time;

  switch (time_type)
  {
    default:
    case DT_LOCAL_TIME:{ // replaces hhmmss_ctr
      
      // make another GetTime (no date function)
      time = Rtc.local_time;
      char dt[20];
      datetime_t tmpTime;
      BreakTime(time, tmpTime);
      snprintf_P(dt, sizeof(dt), PSTR("%02d:%02d:%02d"), tmpTime.hour, tmpTime.minute, tmpTime.second);
      return String(dt);  // 11:08:02
    }
    break;
  }
  
}


/**
 * @brief Testing: New way of getting the TimeOfDay (TOD) in a string format
 * 
 * @param time_type 
 * @return String 
 */
String mTime::GetTimeStr(uint32_t time, bool include_day_of_week)
{
  // HH:MM:SS or WWTHH:MM:SS
        
  char dt[20];
  datetime_t tmpTime;
  BreakTime(time, tmpTime);
  snprintf_P(dt, sizeof(dt), PSTR("%02d:%02d:%02d"), tmpTime.hour, tmpTime.minute, tmpTime.second);
  return String(dt);  // 11:08:02

}





// -----------------------------------------------------------------------------
// FormatTime (no-heap)
// - Writes UTC timestamp into caller buffer.
// - Output: "YYYY-MM-DD HH:MM:SS" (19 chars + null)
// - Returns true on success, false if buffer too small or gmtime fails.
// -----------------------------------------------------------------------------
bool mTime::FormatTime(char* out, size_t out_len, time_t t)
{
  if (!out || out_len < 20) { // 19 + '\0'
    if (out && out_len) out[0] = '\0';
    return false;
  }

  // gmtime_r is thread-safe / re-entrant. On ESP32/newlib it exists.
  struct tm tm_utc;
  if (!gmtime_r(&t, &tm_utc)) {
    out[0] = '\0';
    return false;
  }

  // YYYY-MM-DD HH:MM:SS
  int n = snprintf(out, out_len, "%04d-%02d-%02d %02d:%02d:%02d",
                   tm_utc.tm_year + 1900,
                   tm_utc.tm_mon + 1,
                   tm_utc.tm_mday,
                   tm_utc.tm_hour,
                   tm_utc.tm_min,
                   tm_utc.tm_sec);

  if (n < 0 || (size_t)n >= out_len) {
    out[0] = '\0';
    return false;
  }

  return true;
}


// -----------------------------------------------------------------------------
// mTime::formatTimeCTime (no-heap)
// - Formats like ctime(): "Wed Jun 30 21:49:08 1993" (no trailing '\n')
// -----------------------------------------------------------------------------
bool mTime::formatTimeCTime(char* out, size_t out_len, time_t t)
{
  if (!out || out_len < 2) {
    if (out && out_len) out[0] = '\0';
    return false;
  }

  const char* s = ctime(&t);     // static internal buffer
  if (!s) {
    out[0] = '\0';
    return false;
  }

  // Copy, stripping trailing newline if present
  size_t i = 0;
  for (; i + 1 < out_len && s[i] && s[i] != '\n' && s[i] != '\r'; ++i) {
    out[i] = s[i];
  }
  out[i] = '\0';
  return true;
}


// -----------------------------------------------------------------------------
// mTime::formatTimeUntil (no-heap)
// - Writes "HH:MM:SS" into caller buffer.
// - Returns true on success.
// -----------------------------------------------------------------------------
bool mTime::formatTimeUntil(char* out, size_t out_len, uint32_t time_until_seconds)
{
  if (!out || out_len < 9) { // "HH:MM:SS" + '\0'
    if (out && out_len) out[0] = '\0';
    return false;
  }

  uint32_t hours   = time_until_seconds / 3600UL;
  uint32_t minutes = (time_until_seconds % 3600UL) / 60UL;
  uint32_t seconds = time_until_seconds % 60UL;

  // If you want to clamp hours to 2 digits (00-99), uncomment:
  // if (hours > 99) hours = 99;

  int n = snprintf(out, out_len, "%02lu:%02lu:%02lu",
                   (unsigned long)hours,
                   (unsigned long)minutes,
                   (unsigned long)seconds);

  if (n < 0 || (size_t)n >= out_len) {
    out[0] = '\0';
    return false;
  }

  return true;
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
String mTime::GetDateAndTime(uint8_t datetime_type) 
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  uint32_t time = Rtc.local_time;
  
  switch (datetime_type) {
    case DT_LOCAL_TIME:
      time = Rtc.local_time;
      break;
    case DT_UTC:
      time = Rtc.utc_time;
      break;
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
      time = tkr_set->Settings.bootcount_reset_time;
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
  else if (tkr_set->Settings.flag3.time_append_timezone && (DT_LOCAL == datetime_type)) {  // SetOption52 - Append timezone to JSON time
    dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
  }
  return dt;                // 2017-03-07T11:08:02-07:00 or 2017-03-07T11:08:02.123-07:00
}


uint32_t mTime::UpTime(void) 
{
  if (Rtc.restart_time) {
    return Rtc.utc_time - Rtc.restart_time;
  } else {
    return uptime_seconds_nonreset;
  }
}


uint32_t mTime::MinutesUptime(void) 
{
  return (UpTime() / 60);
}


// String mTime::GetUptime(void) 
// {
//   return GetDuration(UpTime());
// }
const char* mTime::GetUptime(char* out, size_t out_len)
{
  GetDuration(out, out_len, UpTime());
  return out;
}



uint32_t mTime::MinutesPastMidnight(void) 
{
  uint32_t minutes = 0;

  if (RtcTime.valid) {
    minutes = (RtcTime.hour *60) + RtcTime.minute;
  }
  return minutes;
}

uint32_t mTime::SecondsPastMidnight_SecondsOfCurrentDay(void) 
{
  uint32_t seconds = 0;

  if (RtcTime.valid) {
    seconds = (RtcTime.hour * 3600) + (RtcTime.minute * 60) + RtcTime.second;
  }

  // ALOG_INF(PSTR("%d:%d:%d"), RtcTime.hour, RtcTime.minute, RtcTime.second);

  return seconds;
}


uint32_t mTime::RtcMillis(void) 
{
  return (millis() - Rtc.millis) % 1000;
}


void mTime::BreakNanoTime(uint32_t time_input, uint32_t time_nanos, datetime_t &tm) 
{
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
  
  #ifdef ESP32
  strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
  #else
  strncpy(tm.name_of_month, kMonthNames + (month *3), 3);   // fix for esp8266, as strlcpy causes Exception (3). Should be fixable. 
  #endif 
  
  tm.month = month + 1;      // jan is month 1
  tm.day_of_month = time + 1;         // day of month
  tm.valid = (time_input > START_VALID_TIME);  // 2016-01-01
}


void mTime::BreakTime(uint32_t time_input, datetime_t &tm) 
{
  BreakNanoTime(time_input, 0, tm);
}


uint32_t mTime::MakeTime(datetime_t &tm) 
{
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



uint32_t mTime::BuildDateTimeElapsed() {
  if(!RtcTime.valid) return 0;
    // Compute once and cache the build timestamp.
    static const uint32_t buildEpoch = ConvertBuildDateTimeToEpoch();
    uint32_t currentTime = Rtc.local_time;
    if (currentTime > buildEpoch) {
        return currentTime - buildEpoch;
    }
    return 0;
}

bool mTime::IsBuildDateTimeElapsedBeyond(uint32_t seconds_check) {
    return BuildDateTimeElapsed() > seconds_check;
}



uint32_t mTime::GetUTCTime()
{
  return Rtc.utc_time;
}


uint32_t mTime::RuleToTime( TimeRule r, int yr) 
{
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


void mTime::RtcGetDaylightSavingTimes(uint32_t local_time) 
{
  datetime_t tmpTime;
  BreakTime(local_time, tmpTime);
  tmpTime.year += 1970;

  /**
   * @brief Temporary fix for the time rules, since loading of settings is erasing these values.
   * 
   */
  tkr_set->Settings.tflag[0].hemis = TIME_STD_HEMISPHERE;
  tkr_set->Settings.tflag[0].week = TIME_STD_WEEK;
  tkr_set->Settings.tflag[0].dow = TIME_STD_DAY;
  tkr_set->Settings.tflag[0].month = TIME_STD_MONTH;
  tkr_set->Settings.tflag[0].hour = TIME_STD_HOUR;
  tkr_set->Settings.toffset[0] = TIME_STD_OFFSET;

  tkr_set->Settings.tflag[1].hemis = TIME_DST_HEMISPHERE;
  tkr_set->Settings.tflag[1].week = TIME_DST_WEEK;
  tkr_set->Settings.tflag[1].dow = TIME_DST_DAY;
  tkr_set->Settings.tflag[1].month = TIME_DST_MONTH;
  tkr_set->Settings.tflag[1].hour = TIME_DST_HOUR;
  tkr_set->Settings.toffset[1] = TIME_DST_OFFSET;


  Rtc.daylight_saving_time = RuleToTime(tkr_set->Settings.tflag[1], tmpTime.year);
  Rtc.standard_time = RuleToTime( tkr_set->Settings.tflag[0], tmpTime.year);

  // ALOG_HGL(PSTR("RtcGetDaylightSavingTimes: %s %s"), GetDT(Rtc.daylight_saving_time).c_str(), GetDT(Rtc.standard_time).c_str());

}


uint32_t mTime::RtcTimeZoneOffset(uint32_t local_time) 
{

  #ifndef ENABLE_DEVFEATURE_TIME__TIMEZONE_COLORADO  
  // ALOG_INF(PSTR(D_LOG_TIME2 "RtcTimeZoneOffset"));
  tkr_set->Settings.toffset[1] = 60;//FORCED TO TEST
  tkr_set->Settings.toffset[0] = 0;//FORCED TO TEST
  #endif

  int16_t timezone_minutes = tkr_set->Settings.timezone_minutes2;
  if (tkr_set->Settings.timezone2 < 0) { timezone_minutes *= -1; }
  uint32_t timezone = (tkr_set->Settings.timezone2 * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
  // ALOG_INF(PSTR(D_LOG_TIME2 "RtcTimeZoneOffset timezone %d"), timezone);
  
  if (99 == tkr_set->Settings.timezone2) 
  {
    int32_t dstoffset = tkr_set->Settings.toffset[1] * SECS_PER_MIN;
    int32_t stdoffset = tkr_set->Settings.toffset[0] * SECS_PER_MIN;
    if (tkr_set->Settings.tflag[1].hemis) {
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


void mTime::RtcSetTimeOfDay(uint32_t local_time) 
{
  // Sync Core/RTOS time to be used by file system time stamps
  struct timeval tv;
  tv.tv_sec = local_time;
  tv.tv_usec = 0;
  settimeofday(&tv, nullptr);
}


void mTime::RtcSecond(void) 
{
  static uint32_t last_sync = 0;
  static bool mutex = false;

  if (mutex) { return; }

  if (Rtc.time_synced) {
    mutex = true;

    Rtc.time_synced = false;
    Rtc.last_synced = true;
    last_sync = Rtc.utc_time;

    if (Rtc.restart_time == 0) {
      Rtc.restart_time = Rtc.utc_time - uptime_seconds_nonreset;  // save first synced time as restart time
    }

    RtcGetDaylightSavingTimes(Rtc.utc_time);

    ALOG_INF(PSTR("RTC: " D_UTC_TIME " %s, " D_DST_TIME " %s, " D_STD_TIME " %s"),
      GetDateAndTime(DT_UTC).c_str(), GetDateAndTime(DT_DST).c_str(), GetDateAndTime(DT_STD).c_str());

    if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
      tkr_set->Settings.rules_flag.time_init = 1;
    } else {
      tkr_set->Settings.rules_flag.time_set = 1;
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
    ALOG_INF(PSTR("RTC: Not synced"));
    last_sync = Rtc.utc_time;
  }

  if (Rtc.utc_time > START_VALID_TIME) {  // 2016-01-01
    Rtc.time_timezone = RtcTimeZoneOffset(Rtc.utc_time);
    Rtc.local_time = Rtc.utc_time + Rtc.time_timezone;
    Rtc.time_timezone /= 60;
    if (tkr_set->Settings.bootcount_reset_time < START_VALID_TIME) {
      tkr_set->Settings.bootcount_reset_time = Rtc.local_time;
    }
    RtcTime.valid = true;
    ALOG_DBM(PSTR(D_LOG_TIME "Valid"));
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


void mTime::RtcSync(const char* source) 
{

  Rtc.time_synced = true;
  RtcSecond();
  ALOG_INF(PSTR("RTC: Synced by %s"), source);

  tkr->Tasker_Interface(TASK_TIME_SYNCED);
  
}


void mTime::RtcSetTime(uint32_t epoch) 
{
  if (epoch < START_VALID_TIME) {  // 2016-01-01
    Rtc.user_time_entry = false;
    ntp_force_sync = true;
  } else {
    Rtc.user_time_entry = true;
    Rtc.utc_time = epoch;
    RtcSync("Time");
  }
}


void mTime::Init(void) 
{

  TickerRtc = new Ticker();

  Rtc.utc_time = 0;
  BreakTime(Rtc.utc_time, RtcTime);
  
  #ifdef ESP32
    TickerRtc->attach_ms(1000, +[](mTime* instance){ instance->RtcSecond(); }, this);
  #else
    TickerRtc->attach   (1,            [this](void){ this->RtcSecond(); });
  #endif

  if (tkr_set->Settings.cfg_timestamp > START_VALID_TIME) {
    // Fix file timestamp while utctime is not synced
    uint32_t utc_time = tkr_set->Settings.cfg_timestamp;
    if (RtcSettings.utc_time > utc_time) {
      utc_time = RtcSettings.utc_time;
    }
    utc_time++;
    RtcGetDaylightSavingTimes(utc_time);
    uint32_t local_time = utc_time + RtcTimeZoneOffset(utc_time);
    RtcSetTimeOfDay(local_time);
   ALOG_INF(PSTR("RTC: Timestamp %s"), GetDT(local_time).c_str());
  }
  
  module_state.mode = ModuleStatus::Running;

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


// SUMMARY
//   Poll NTP over WiFi with deterministic scheduling and backoff.
//   - First sync: shortly after WiFi comes up (with stagger per-chip).
//   - Retry on failure: exponential backoff (capped).
//   - Re-sync on success: once per hour (staggered per-chip).
//
// ARGUMENTS
//   (none)
//
// EXAMPLES
//   // Call from your periodic loop (e.g., EverySecond / Tasker)
//   tkr_time->WifiPollNtp();
//
// RETURNS
//   (none)
//
// CHANGED
//   04Feb26  Replace minute/second gating with millis-based scheduler, add backoff and reentrancy guard.
void mTime::WifiPollNtp()
{
  // ---- Static state ----
  static bool     ntp_busy = false;
  static uint32_t next_try_ms = 0;
  static uint32_t backoff_s = 5;            // start retry at 5s
  static bool     ever_synced = false;

  if (tkr_set->runtime.global_state.wifi_down || Rtc.user_time_entry) { return; }
  if (ntp_busy) { return; }

  const uint32_t now_ms = millis();

  // ---- Chip-stagger (0..45s) to avoid fleet thundering herd ----
  const uint32_t chip_stagger_s = ((mSupportHardware::ESP_getChipId() & 0xF) * 3) + 3;

  // ---- Forced sync overrides schedule ----
  if (!ntp_force_sync) {
    if (next_try_ms != 0 && (int32_t)(now_ms - next_try_ms) < 0) { return; }
  }

  ntp_force_sync = false;
  ntp_busy = true;

  ALOG_INF(PSTR("NTP: Sync time..."));

  const uint32_t t0 = now_ms;
  const uint64_t ntp_nanos = WifiGetNtp();
  const uint32_t dt_s = (millis() - t0) / 1000;

  ALOG_INF(PSTR("NTP: Runtime %u"), (unsigned)dt_s);

  const uint32_t ntp_time = (uint32_t)(ntp_nanos / 1000000000ULL);
  ALOG_HGL(PSTR("NTP: ntp_time %u"), (unsigned)ntp_time);

  if (ntp_time > START_VALID_TIME) {
    // ---- Success ----
    Rtc.utc_time = ntp_time;
    Rtc.nanos    = (uint32_t)(ntp_nanos % 1000000000ULL);
    RtcSync("NTP");

    ever_synced = true;
    backoff_s   = 5;

    // Re-sync hourly, staggered
    next_try_ms = millis() + (3600UL + chip_stagger_s) * 1000UL;
  } else {
    // ---- Failure ----
    // Backoff: 5,10,20,40,80,160,320,600 (cap at 10 min)
    if (backoff_s < 600) {
      backoff_s = (backoff_s < 300) ? (backoff_s * 2) : 600;
    }

    // First-sync failures: retry sooner, but still staggered
    const uint32_t retry_s = (ever_synced ? backoff_s : (backoff_s + chip_stagger_s));

    next_try_ms = millis() + retry_s * 1000UL;
  }

  ntp_busy = false;
}


uint64_t mTime::WifiGetNtp(void)
{
  static uint8_t ntp_server_id = 0;

  ALOG_INF(PSTR("NTP: Start NTP Sync %d ..."), ntp_server_id);

  // --------------------------------------------------------------------------
  // Pick server (configured list, then fallbacks)
  // --------------------------------------------------------------------------
  IPAddress time_server_ip;

  char fallback_ntp_server[2][32];
  ext_snprintf_P(fallback_ntp_server[0], sizeof(fallback_ntp_server[0]), PSTR("%_I"), tkr_set->Settings.ipv4_address[1]);  // local gateway/DNS-ish
  ext_snprintf_P(fallback_ntp_server[1], sizeof(fallback_ntp_server[1]), PSTR("%d.pool.ntp.org"), (int)random(0, 3));

  const uint32_t server_count = MAX_NTP_SERVERS + 2;

  // Find first non-empty server string, starting at ntp_server_id (wraps)
  const char* ntp_server = nullptr;
  for (uint32_t tries = 0; tries < server_count; tries++) {
    if (ntp_server_id >= server_count) { ntp_server_id = 0; }

    const char* s =
      (ntp_server_id < MAX_NTP_SERVERS)
        ? tkr_set->SettingsText(SET_NTPSERVER1 + ntp_server_id)
        : fallback_ntp_server[ntp_server_id - MAX_NTP_SERVERS];

    if (s && s[0] != '\0') { ntp_server = s; break; }
    ntp_server_id++;
  }

  if (!ntp_server || ntp_server[0] == '\0') {
    ALOG_DBG(PSTR("NTP: No server configured"));
    ntp_server_id++;
    return 0;
  }

  ALOG_INF(PSTR("NTP: server '%s'"), ntp_server);

  // --------------------------------------------------------------------------
  // Resolve hostname -> IP
  // --------------------------------------------------------------------------
  #ifdef USE_MODULE_NETWORK_MQTT
  if (!tkr_wifi->WiFi_Dns_ResolveHostname(ntp_server, time_server_ip)) {
    ALOG_DBG(PSTR("NTP: Unable to resolve '%s'"), ntp_server);
    ntp_server_id++;
    return 0;
  }
  #else
  // No DNS support in this build
  ntp_server_id++;
  return 0;
  #endif

  // --------------------------------------------------------------------------
  // Open UDP + flush stale RX (bounded)
  // --------------------------------------------------------------------------
  WiFiUDP udp;

  bool begun = false;
  #ifdef USE_IPV6
    // If you truly need IPv6, keep your existing IPv6 binding logic.
    // NOTE: many cores don't support IPv6 binding this way; leaving it guarded.
    begun = (udp.begin(IPAddress(IPv6), 0) != 0);
  #else
    // Prefer ephemeral port if supported.
    begun = (udp.begin((uint16_t)0) != 0);
  #endif

  if (!begun) {
    // Fallback: a few known-high ports
    const uint16_t fallback_ports[] = { 49152, 49153, 49154 };
    for (uint8_t k = 0; k < (sizeof(fallback_ports)/sizeof(fallback_ports[0])); k++) {
      #ifdef USE_IPV6
        if (udp.begin(IPAddress(IPv6), fallback_ports[k]) != 0) { begun = true; break; }
      #else
        if (udp.begin(fallback_ports[k]) != 0) { begun = true; break; }
      #endif
      yield();
    }
  }

  if (!begun) {
    ALOG_DBG(PSTR("NTP: udp.begin failed"));
    ntp_server_id++;
    return 0;
  }

  // Bounded flush to avoid busy-loop if network is noisy.
  {
    const uint32_t flush_start = millis();
    uint8_t flushed = 0;
    while ((udp.parsePacket() > 0) && (flushed < 8) && ((millis() - flush_start) < 50)) {
      while (udp.available()) { (void)udp.read(); }
      flushed++;
      yield();
    }
  }

  // --------------------------------------------------------------------------
  // Build NTP request
  // --------------------------------------------------------------------------
  const uint32_t NTP_PACKET_SIZE = 48;
  uint8_t packet_buffer[NTP_PACKET_SIZE] = {0};

  packet_buffer[0]  = 0b11100011;  // LI, Version, Mode (client)
  packet_buffer[1]  = 0;           // Stratum
  packet_buffer[2]  = 6;           // Polling interval
  packet_buffer[3]  = 0xEC;        // Precision
  packet_buffer[12] = 49;
  packet_buffer[13] = 0x4E;
  packet_buffer[14] = 49;
  packet_buffer[15] = 52;

  // --------------------------------------------------------------------------
  // Send request
  // --------------------------------------------------------------------------
  if (udp.beginPacket(time_server_ip, 123) == 0) {
    ALOG_DBG(PSTR("NTP: beginPacket failed (%_I)"), (uint32_t)time_server_ip);
    udp.stop();
    ntp_server_id++;
    return 0;
  }

  udp.write(packet_buffer, NTP_PACKET_SIZE);
  udp.endPacket();

  // --------------------------------------------------------------------------
  // Wait for reply (bounded, safe validation)
  // --------------------------------------------------------------------------
  const uint32_t wait_start = millis();
  const uint32_t wait_ms    = 1000;

  while (!TimeReached(wait_start + wait_ms)) {

    const int size = udp.parsePacket();
    if (size <= 0) {
      delay(10);
      yield();
      continue;
    }

    // Drain packets that are too small
    if (size < (int)NTP_PACKET_SIZE) {
      while (udp.available()) { (void)udp.read(); }
      continue;
    }

    // Validate origin (optional: don’t require remotePort==123, NAT can vary)
    // Keep IP check; allow port flexibility.
    const IPAddress rip = udp.remoteIP();
    if (rip != time_server_ip) {
      // Not from the server we queried; discard.
      while (udp.available()) { (void)udp.read(); }
      continue;
    }

    // Read exactly 48 bytes (discard remainder if any)
    const int rd = udp.read(packet_buffer, NTP_PACKET_SIZE);
    while (udp.available()) { (void)udp.read(); } // discard any extra

    udp.stop();

    if (rd != (int)NTP_PACKET_SIZE) {
      ALOG_DBG(PSTR("NTP: short read %d"), rd);
      ntp_server_id++;
      return 0;
    }

    // Basic NTP sanity:
    // - LI must not be 3 (unsynchronized)
    // - Mode in low bits should be server (4) or broadcast (5), but some stacks vary; keep light-touch
    const uint8_t li = (packet_buffer[0] >> 6) & 0x03;
    if (li == 3) {
      ALOG_INF(PSTR("NTP: IP %_I unsynced (LI=3)"), (uint32_t)time_server_ip);
      ntp_server_id++;
      return 0;
    }

    // Extract transmit timestamp (seconds + fraction) at bytes 40..47
    const uint32_t secs_since_1900 =
      ((uint32_t)packet_buffer[40] << 24) |
      ((uint32_t)packet_buffer[41] << 16) |
      ((uint32_t)packet_buffer[42] << 8)  |
       (uint32_t)packet_buffer[43];

    if (secs_since_1900 == 0) {
      ALOG_DBG(PSTR("NTP: zero timestamp"));
      ntp_server_id++;
      return 0;
    }

    const uint32_t frac_raw =
      ((uint32_t)packet_buffer[44] << 24) |
      ((uint32_t)packet_buffer[45] << 16) |
      ((uint32_t)packet_buffer[46] << 8)  |
       (uint32_t)packet_buffer[47];

    // Convert NTP epoch to Unix epoch
    // NTP(1900-01-01) -> Unix(1970-01-01) offset = 2208988800 seconds
    if (secs_since_1900 < 2208988800UL) {
      ALOG_DBG(PSTR("NTP: timestamp before Unix epoch"));
      ntp_server_id++;
      return 0;
    }

    const uint64_t unix_seconds = (uint64_t)(secs_since_1900 - 2208988800UL);

    // fraction: 32-bit fixed point of a second => nanoseconds
    const uint64_t fraction_ns = (((uint64_t)frac_raw) * 1000000000ULL) >> 32;

    const uint64_t result_ns = (unix_seconds * 1000000000ULL) + fraction_ns;

    // Optional debug (guarded)
    #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
    ALOG_DBG(PSTR("NTP: %_I unix=%lu frac_ns=%lu -> %llu ns"),
      (uint32_t)time_server_ip,
      (uint32_t)unix_seconds,
      (uint32_t)fraction_ns,
      (unsigned long long)result_ns
    );
    #endif

    // Success: do NOT advance ntp_server_id here (stick to working server)
    return result_ns;
  }

  // Timeout
  ALOG_INF(PSTR("NTP: No reply from %_I"), (uint32_t)time_server_ip);
  udp.stop();
  ntp_server_id++; // rotate server next time after failure
  return 0;
}


/**
 * Breaking time to return parts of time
 * */

uint8_t mTime::day(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.day_of_week;
}

uint8_t mTime::month(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.month;
}

uint16_t mTime::year(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.year;
}


uint8_t mTime::hour(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.hour;
}


uint8_t mTime::minute(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.minute;
}


uint8_t mTime::second(uint32_t time)
{
  datetime_t time_temp;
  BreakTime(time, time_temp);
  return time_temp.second;
}


int mTime::hourFormat12(time_t time) 
{ // the hour for the given time in 12 hour format
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

// Time elapsed function that updates the time when true
bool mTime::TimeReached(uint32_t* tSaved, uint32_t ElapsedTime){

  unsigned long long elapsed = millis()-*tSaved;

  if(
    llabs(elapsed)>=ElapsedTime
  ){ *tSaved=millis();
    return true;
  }
  return false;
}


bool mTime::TimeReached(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime){
  if(
    (ABS_FUNCTION(millis()-tSaved->millis)>=ElapsedTime)
    ||(tSaved->run == true)    
    ){ 
      tSaved->millis=millis();
      tSaved->run = false;
    return true;
  }
  return false;
}


bool mTime::TimeReachedNonReset(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime){
  if(
    (ABS_FUNCTION(millis()-tSaved->millis)>=ElapsedTime)
    ||(tSaved->run == true)    
    ){ 
      //tSaved->millis=millis();
      tSaved->run = false;
    return true;
  }
  return false;
}


// Time elapsed function that updates the time when true
bool mTime::TimeReachedNonReset(uint32_t* tSaved, uint32_t ElapsedTime){
  if(ABS_FUNCTION(millis()-*tSaved)>=ElapsedTime){
    return true;
  }
  return false;
}


// Time elapsed function that updates the time when true
uint32_t mTime::MillisElapsed(uint32_t* tSaved){
  return labs(millis()-*tSaved);
}


/**
 * @brief Get the millis from saved millis count to now
 * */
uint32_t mTime::MillisElapsed(uint32_t tSaved)
{
  // uint64_t time = millis()-tSaved;
  if(millis()<=tSaved)
    return 0;
  return labs(millis()-tSaved);
}


// Time elapsed function that updates the time when true
// WARNING: doesnt allow for wrap around
bool mTime::MillisReached(uint32_t* tTarget){
  if(millis()>*tTarget){
    return true;
  }
  return false;
}


#ifdef USE_SUNRISE
/*********************************************************************************************\
 * Sunrise and sunset (+13k code)
 *
 * https://forum.arduino.cc/index.php?topic=218280.0
 * Source: C-Programm von http://lexikon.astronomie.info/equation_of_time/neu.html
 *         Rewrite for Arduino by 'jurs' for German Arduino forum
\*********************************************************************************************/

const float pi2 = TWO_PI;
const float pi = PI;
const float RAD = DEG_TO_RAD;

// Compute the Julian date from the Calendar date, using only unsigned ints for code compactness
// Warning this formula works only from 2000 to 2099, after 2100 we get 1 day off per century.
// In astronomy, a Julian year (symbol: a) is a unit of measurement of time defined as exactly 365.25 days of 86400 SI seconds each
uint32_t mTime::JulianDate(const datetime_t &now) {
  // https://en.wikipedia.org/wiki/Julian_day

  uint32_t Year = now.year;             // Year ex:2020
  uint32_t Month = now.month;            // 1..12
  uint32_t Day = now.day_of_week;     // 1..31
  uint32_t Julian;                          // Julian day number

  if (Month <= 2) {
    Month += 12;
    Year -= 1;
  }
  // Warning, this formula works only for the 20th century, afterwards be are off by 1 day - which does not impact Sunrise much
  // Julian = (1461 * Year + 6884472) / 4 + (153 * Month - 457) / 5 + Day -1 -13;
  Julian = (1461 * Year + 6884416) / 4 + (153 * Month - 457) / 5 + Day;   // -1 -13 included in 6884472 - 14*4 = 6884416
  return Julian;
}

// Force value in the 0..pi2 range
float mTime::InPi(float x)
{
  return tkr_sup->ModulusRangef(x, 0.0f, pi2);
}

// Time formula
// Tdays is the number of days since Jan 1 2000, and replaces T as the Tropical Century. T = Tdays / 36525.0
float mTime::TimeFormula(float *declination_of_sun, uint32_t Tdays) {
  float RA_Mean = 18.71506921f + (2400.0513369f / 36525.0f) * Tdays;    // we keep only first order value as T is between 0.20 and 0.30
  float M = InPi( (pi2 * 0.993133f) + (pi2 * 99.997361f / 36525.0f) * Tdays);
  float L = InPi( (pi2 * 0.7859453f) + M + (6893.0f * sinf(M) + 72.0f * sinf(M+M) + (6191.2f / 36525.0f) * Tdays) * (pi2 / 1296.0e3f));

  float cos_eps = 0.91750f;     // precompute cos(eps)
  float sin_eps = 0.39773f;     // precompute sin(eps)

  float RA = atanf(tanf(L) * cos_eps);
  if (RA < 0.0f) RA += pi;
  if (L > pi) RA += pi;
  RA = RA * (24.0f/pi2);
  *declination_of_sun = asinf(sin_eps * sinf(L));
  RA_Mean = tkr_sup->ModulusRangef(RA_Mean, 0.0f, 24.0f);
  float dRA = tkr_sup->ModulusRangef(RA_Mean - RA, -12.0f, 12.0f);
  dRA = dRA * 1.0027379f;
  return dRA;
}

void mTime::DuskTillDawn(uint8_t *hour_up,uint8_t *minute_up, uint8_t *hour_down, uint8_t *minute_down)
{
  const uint32_t JD2000 = 2451545;
  uint32_t JD = JulianDate(RtcTime);
  uint32_t Tdays = JD - JD2000;           // number of days since Jan 1 2000 (n)

  const float h = SUNRISE_DAWN_ANGLE * RAD;
  const float sin_h = sinf(h);    // let GCC pre-compute the sin() at compile time // \phi  is the north latitude of the observer (north is positive, south is negative) on the Earth.

  float lat = tkr_set->Settings.sensors.latitude / (1000000.0f / RAD); // geographische Breite
  float lon = ((float) tkr_set->Settings.sensors.longitude)/1000000;
  
  /**
   * The Earth rotates at an angular velocity of 15°/hour. 
   * Therefore, the expression {\displaystyle \omega _{\circ }\times {\frac {\mathrm {15} ^{\circ }}{hour}}}{\displaystyle \omega _{\circ }\times {\frac {\mathrm {15} ^{\circ }}{hour}}} 
   * gives the interval of time before and after local solar noon that sunrise or sunset will occur.
   * https://en.wikipedia.org/wiki/Sunrise_equation
   * */

  float timezone = ((float)Rtc.time_timezone) / 60;

  float declination_of_sun;
  // https://en.wikipedia.org/wiki/Equation_of_time ie. difference between "calender time" and "true sun position time"
  float equation_of_time = TimeFormula(&declination_of_sun, Tdays);  //declination_of_sun = \delta  is the declination of the sun.
  // This is the equation from above with corrections for atmospherical refraction and solar disc diameter.
  float time_difference = acosf((sin_h - sinf(lat)*sinf(declination_of_sun)) / (cosf(lat)*cosf(declination_of_sun))) * (12.0f / pi); //hour angle //ωo is the hour angle from the observer's zenith;
  
  //local time = midday, downfall = sun transition
  float local_time = 12.0f - time_difference - equation_of_time; //12 hours = transition times (twice a day)
  float downfall_local_time = 12.0f + time_difference - equation_of_time;

  // Sunrise/Sunset transitions which are related by degrees of rotation
  float rise_world_time = local_time - lon / 15.0f;
  float downfall_world_time = downfall_local_time - lon / 15.0f;

  float rise = rise_world_time + timezone + (1/120.0f);         // In hours, with rounding to nearest minute (1/60 * .5)
  rise = tkr_sup->ModulusRangef(rise, 0.0f, 24.0f);        // force 0 <= x < 24.0
  int rise_hours = (int)rise;
  int rise_minutes = (int)(60.0f * fmodf(rise, 1.0f));

  float downfall = downfall_world_time + timezone;
  downfall = tkr_sup->ModulusRangef(downfall, 0.0f, 24.0f);
  int downfall_hours = (int)downfall;
  int downfall_minutes = (int)(60.0f * fmodf(downfall, 1.0f));

  // Return transit times
  *hour_up = rise_hours;
  *minute_up = rise_minutes;
  *hour_down = downfall_hours;
  *minute_down = downfall_minutes;
}


char* mTime::GetSunTimeAtHorizon(uint32_t dawn, char* buffer, uint8_t buflen)
{

  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  snprintf_P(buffer, buflen, PSTR("%02d:%02d"), hour[dawn], minute[dawn]);
  return buffer;

}


uint16_t mTime::SunMinutes(uint32_t dawn)
{
  uint8_t hour[2];
  uint8_t minute[2];

  DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
  dawn &= 1;
  return (hour[dawn] *60) + minute[dawn];

}


#endif  // USE_SUNRISE



uint32_t mTime::EncodeTimeShortToU32(const time_short_t& time) 
{
  return (time.day_of_week << 24) | (time.hour << 16) | (time.minute << 8) | time.second;
}

time_short_t mTime::DecodeU32ToTimeShort(uint32_t encoded_time) 
{
  time_short_t time;
  time.day_of_week = (encoded_time >> 24) & 0xFF;
  time.hour        = (encoded_time >> 16) & 0xFF;
  time.minute      = (encoded_time >> 8) & 0xFF;
  time.second      = encoded_time & 0xFF;
  return time;
}

const char* mTime::GetFormattedTime(uint32_t encoded_time, char* buffer, uint8_t buflen, bool include_weekday) 
{
  time_short_t time = DecodeU32ToTimeShort(encoded_time);
  if (include_weekday) {
    snprintf_P(buffer, buflen, PSTR("D%d" D_DATE_TIME_SEPARATOR "%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
                time.day_of_week, time.hour, time.minute, time.second);
  } else {
    snprintf_P(buffer, buflen, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
                time.hour, time.minute, time.second);
  }
  return buffer;
}

time_short_t mTime::MakeTimeShort(uint8_t hour, uint8_t minute, uint8_t second, uint8_t day_of_week) 
{
  time_short_t time;
  time.hour = hour;
  time.minute = minute;
  time.second = second;
  time.day_of_week = day_of_week;
  return time;
}

String mTime::GetTimeStrFromTimeShort(const time_short_t& time, bool include_day_of_week) {
    char dt[20];
    if (include_day_of_week) {
        snprintf_P(dt, sizeof(dt), PSTR("D%dT%02d:%02d:%02d"), 
                   time.day_of_week, time.hour, time.minute, time.second);
    } else {
        snprintf_P(dt, sizeof(dt), PSTR("%02d:%02d:%02d"), 
                   time.hour, time.minute, time.second);
    }
    return String(dt);  // e.g., "D3T11:08:02" or "11:08:02"
}



#ifdef ENABLE_PHASEOUT_TIME__LEGACY_CODE

uint32_t mTime::GetTimeOfDay_Seconds(void){
  return 0;//RtcTime.Dseconds;
}

const char* mTime::ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds_tod, char* buffer, uint8_t buflen)
{
  
  seconds_tod = seconds_tod % (24 * 3600);
  int hours = seconds_tod / 3600;

  seconds_tod %= 3600;
  int minutes = seconds_tod / 60;

  seconds_tod %= 60;
  int seconds = seconds_tod;

  snprintf_P(buffer, buflen, 
              PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              hours, minutes, seconds
  );
  return buffer;

}


time_short_t mTime::GetTimeShortNow(){

  time_short_t now;

  now.day_of_week = RtcTime.day_of_week;
  now.hour = RtcTime.hour;
  now.minute = RtcTime.minute;
  now.second = RtcTime.second;

  return now;

}

uint32_t mTime::GetTimeShortNowU32()
{

  // time_short_t now;

  // now.day_of_week = RtcTime.day_of_week;
  // now.hour = RtcTime.hour;
  // now.minute = RtcTime.minute;
  // now.second = RtcTime.second;

  uint32_t now = (RtcTime.day_of_week<<24) | (RtcTime.hour<<16) | (RtcTime.minute<<8) | (RtcTime.second);
  // uint32_t now2 = reinterpret_cast<uint32_t>(now);

  return now;

}

const char* mTime::ConvertShortTimetoCtr(time_short_t* time, char* buffer, uint8_t buflen)
{
  
  snprintf_P(buffer, buflen, 
              PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              time->hour, time->minute, time->second
  );
  return buffer;
}

// const char* mTime::ConvertShortTimetoCtr(time_short_t time, char* buffer, uint8_t buflen)
// {
  
//   snprintf_P(buffer, buflen, 
//               PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
//               time.hour, time.minute, time.second
//   );
//   return buffer;
// }



/**
 * @brief 
 * */
const char* mTime::ConvertU32TimetoCtr(uint32_t* _time, char* buffer, uint8_t buflen, bool flag_include_weekday ) //pointer is not needed!
{
  // typedef struct time_short{
  //   uint8_t day_of_week; // week day [0-7]
  //   uint8_t hour;   // [0-23]
  //   uint8_t minute; // [0-59]
  //   uint8_t second; // [0-59]
  // }time_short_t;

  uint32_t time = *_time;
  uint8_t day_of_week   = (uint8_t)((time & 0xFF000000) >> 24);
  uint8_t hour   = (uint8_t)((time & 0x00FF0000) >> 16);
  uint8_t minute = (uint8_t)((time & 0x0000FF00) >>  8);
  uint8_t second = (uint8_t)((time & 0x000000FF)      );

  // time_short_t* time = reinterpret_cast<time_short_t*>(_time);
  // uint8_t day_of_week = _time
  if(flag_include_weekday)
{


  snprintf_P(buffer, buflen, 
              PSTR("D%d" D_DATE_TIME_SEPARATOR "%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              day_of_week, hour, minute, second
  );

}else
{

  snprintf_P(buffer, buflen, 
              PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              hour, minute, second
  );
}
  return buffer;
}



// const char* mTime::ConvertShortTimetoCtr(time_short_t* time, char* buffer, uint8_t buflen)
// {

//   // time_short_t* time = reinterpret_cast<time_short_t*>(_time);
  
//   snprintf_P(buffer, buflen, 
//               PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
//               time->hour, time->minute, time->second
//   );
//   return buffer;
// }


time_short_t mTime::Parse_Time_TimeShortCtr_To_TimeShort(const char* time_ctr){

  bool includes_week = false;
  time_short_t time_s = {255, 255, 255, 255}; //invalid time of 255 max range

  if(strstr(time_ctr, "D")){
    //wwDHH:MM:SS // 11 bytes
    includes_week = true;
    if(
      (strlen(time_ctr)!=11) && 
      (time_ctr[2]!='D')
    ){
      
    #ifdef ENABLE_LOG_LEVEL_ERROR
      ALOG_TST(PSTR("Invalid time"));
    #endif // ENABLE_LOG_LEVEL_INFO
      return time_s;
    }
  }else{
    //HH:MM:SS // 8 bytes
    if(
      (strlen(time_ctr)!=8)
    ){
      return time_s;
    }
    includes_week = false;
    #ifdef ENABLE_LOG_LEVEL_ERROR
    ALOG_TST(PSTR("Parse_Time_TimeShortCtr_To_TimeShort NOT D found"));
    #endif //  ENABLE_LOG_LEVEL_INFO
  }

  if(includes_week){
    time_s.day_of_week   = (uint8_t) strtol( &time_ctr[0], NULL, 10);  //days 1-7 so 0 means none set
    time_s.hour   = (uint8_t) strtol( &time_ctr[3], NULL, 10);
    time_s.minute = (uint8_t) strtol( &time_ctr[6], NULL, 10);
    time_s.second = (uint8_t) strtol( &time_ctr[9], NULL, 10);
  }else{
    time_s.day_of_week   = 0;
    time_s.hour   = (uint8_t) strtol( &time_ctr[0], NULL, 10);
    time_s.minute = (uint8_t) strtol( &time_ctr[3], NULL, 10);
    time_s.second = (uint8_t) strtol( &time_ctr[6], NULL, 10);
  }

  return time_s;

}


uint32_t mTime::ConvertHHMMSStoSOD(uint8_t hh, uint8_t mm, uint8_t ss){
  return ((hh*3600)+(mm*60)+(ss));
}


uint8_t mTime::CheckBetweenSOD(uint32_t start, uint32_t end){

  if((RtcTime.Dseconds > start)&&(RtcTime.Dseconds < end)){
    return 1;
  }

  return 0;
  //return ((start > RtcTime.SOD)&&(RtcTime.SOD < end)) ? true : false;
}


//PROBABLY breaks on week rollover!! Needs fixed
// New datetime checker for week only (day_of_week,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sow = (start->day_of_week*SEC2DAY)+(start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sow = (end->day_of_week*SEC2DAY)+(end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = start_sow-RtcTime.Wseconds;
  int32_t time_until_end = end_sow-RtcTime.Wseconds;

    #ifdef ENABLE_LOG_LEVEL_INFO
  ALOG_DBG(PSTR(D_LOG_TIME "CheckBetween_Week_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
    start->hour,start->minute,start->second,time_until_start,
    RtcTime.Dseconds,
    time_until_end,end->hour,end->minute,end->second
  ); 
    #endif// ENABLE_LOG_LEVEL_INFO

  if((start_sow < RtcTime.Wseconds)&&(RtcTime.Wseconds < end_sow)){
    return 1;
  }
  return 0;

}

// New datetime checker for week only (day_of_week,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = RtcTime.Dseconds-start_sod; 
  int32_t time_until_end = end_sod-RtcTime.Dseconds;

  //need to add check if start>end, then add 24 hours

  // #ifdef SERIAL_DEBUG_HIGH_LEVEL

    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_DBG(PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
      start->hour,start->minute,start->second,time_until_start,
      RtcTime.Dseconds,
      time_until_end,end->hour,end->minute,end->second
    ); 
    
    ALOG_DBG(PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d<%02d (%02d) | %02d<%02d (%02d)"),
      start_sod,RtcTime.Dseconds,(start_sod < RtcTime.Dseconds?1:0),
      RtcTime.Dseconds,end_sod,(RtcTime.Dseconds < end_sod)?1:0
    );
    #endif// ENABLE_LOG_LEVEL_INFO
    
   if((start_sod < RtcTime.Dseconds)&&(RtcTime.Dseconds < end_sod)){ //now>start AND now<END
     return 1;
   }
  return 0;

}


bool mTime::IsShortTimeWithinRange(time_short start, time_short end)
{

  uint32_t start_sod = (start.hour*SEC2HOUR)+(start.minute*SEC2MIN)+(start.second);
  uint32_t end_sod =   (end.hour*SEC2HOUR)+(end.minute*SEC2MIN)+(end.second);

  uint32_t time_of_day_secs_now = RtcTime.Dseconds;
  int32_t time_until_start = time_of_day_secs_now-start_sod; 
  int32_t time_until_end = end_sod-time_of_day_secs_now;

  bool flag_24hrs_added = false;
  bool flag_time_period_over_midnight = false;
  bool flag_within_time_window = false;

  // if times are equal, return early as false
  if(start_sod == end_sod) return false;

  /**
   * Check if condition has time going over midnight into next day
   * */
  if(end_sod < start_sod)
  {
    flag_time_period_over_midnight = true;
  }
  
  // ALOG_INF(PSTR(D_LOG_TIME "end<start, flag_time_period_over_midnight = %d"), flag_time_period_over_midnight);

  if(flag_time_period_over_midnight)
  {
    /**
     * Check if current time is within current, or next day (after midnight)
     * If tod < end, it is already inside time window
     * */
    if(time_of_day_secs_now < end_sod)
    {
      flag_within_time_window = true;
    }
    /**
     * If not inside next day window, lets check previous day (ie from start to midnight)
     * */
    else
    if(time_of_day_secs_now > start_sod)
    {
      flag_within_time_window = true;
    }
  }
  /**
   * time window spans same day ie start before end, on same day
   * */
  else
  {

    if((start_sod < time_of_day_secs_now)&&(time_of_day_secs_now < end_sod)){ //now>start AND now<END
      flag_within_time_window = true;
    }else{
      flag_within_time_window = false;
    }

  }

  #ifdef ENABLE_LOG_LEVEL_INFO
  // ALOG_INF(PSTR(D_LOG_TIME "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
  //   start.hour,start.minute,start.second,time_until_start,
  //   RtcTime.Dseconds,
  //   time_until_end,end.hour,end.minute,end.second
  // );
  
  ALOG_INF(PSTR(D_LOG_TIME 
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d %d)"
    "\n\r\t%02d:%02d:%02d (%d seconds)"
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d) %s %s"),   
    start.hour,start.minute,start.second, start_sod, time_until_start,
    RtcTime.hour,RtcTime.minute,RtcTime.second, RtcTime.Dseconds, time_of_day_secs_now,
    end.hour,end.minute,end.second, end_sod, time_until_end,
    // flag_24hrs_added?"+24hrs added":"",
    flag_time_period_over_midnight?"Window Across Midnight":"",
    flag_within_time_window?"WITHIN":"OUTSIDE"
  );
  #endif// ENABLE_LOG_LEVEL_INFO

  return flag_within_time_window;




}

// New datetime checker for week only (day_of_week,hours,minutes,seconds)
int8_t mTime::CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  uint32_t time_of_day_secs_now = RtcTime.Dseconds;
  int32_t time_until_start = time_of_day_secs_now-start_sod; 
  int32_t time_until_end = end_sod-time_of_day_secs_now;

  bool flag_24hrs_added = false;
  bool flag_time_period_over_midnight = false;
  bool flag_within_time_window = false;

  //if times are equal, return early as false
  if(start_sod == end_sod) return false;

  /**
   * Check if condition has time going over midnight into next day
   * */
  if(end_sod < start_sod)
  {
    flag_time_period_over_midnight = true;
  }
  
  // ALOG_INF(PSTR(D_LOG_TIME "end<start, flag_time_period_over_midnight = %d"), flag_time_period_over_midnight);

  if(flag_time_period_over_midnight)
  {
    /**
     * Check if current time is within current, or next day (after midnight)
     * If tod < end, it is already inside time window
     * */
    if(time_of_day_secs_now < end_sod)
    {
      flag_within_time_window = true;
    }
    /**
     * If not inside next day window, lets check previous day (ie from start to midnight)
     * */
    else
    if(time_of_day_secs_now > start_sod)
    {
      flag_within_time_window = true;
    }
  }
  /**
   * time window spans same day ie start before end, on same day
   * */
  else
  {

    if((start_sod < time_of_day_secs_now)&&(time_of_day_secs_now < end_sod)){ //now>start AND now<END
      flag_within_time_window = true;
    }else{
      flag_within_time_window = false;
    }

  }


  /**
   * If end of window is less than start of window, assume its the next day, so add 24hrs
   * need to add check if start>end, then add 24 hours
   * */
  // if(end_sod < start_sod){
  //   /**
  //    * TOD now should only be added to "now" if it has went beyond midnight
  //    * eg 9pm to 3am, 9pm check time should be after, but tod_now is only +24 when after 0am
  //    * */
  //   if(time_of_day_secs_now < end_sod){
  //     time_of_day_secs_now += SECS_PER_DAY; // Also need the time_now needs moved forward into next bracket
  //     end_sod += SECS_PER_DAY; // bring start to be the next day by adding 24 hours
  //   }

  //   flag_24hrs_added = true;
  //   ALOG_INF(PSTR(D_LOG_TIME "end<start, Add 24 hours"));
  // }


  #ifdef ENABLE_LOG_LEVEL_INFO
  // ALOG_INF(PSTR(D_LOG_TIME "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
  //   start->hour,start->minute,start->second,time_until_start,
  //   RtcTime.Dseconds,
  //   time_until_end,end->hour,end->minute,end->second
  // );
  
  ALOG_INF(PSTR(D_LOG_TIME 
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d %d)"
    "\n\r\t%02d:%02d:%02d (%d seconds)"
    "\n\r\t%02d:%02d:%02d (%d seconds : diff %d) %s %s"),   
    start->hour,start->minute,start->second, start_sod, time_until_start,
    RtcTime.hour,RtcTime.minute,RtcTime.second, RtcTime.Dseconds, time_of_day_secs_now,
    end->hour,end->minute,end->second, end_sod, time_until_end,
    // flag_24hrs_added?"+24hrs added":"",
    flag_time_period_over_midnight?"Window Across Midnight":"",
    flag_within_time_window?"WITHIN":"OUTSIDE"
  );
  #endif// ENABLE_LOG_LEVEL_INFO

  return flag_within_time_window;
  
}



// New datetime checker for week only (day_of_week,hours,minutes,seconds)
uint8_t mTime::CheckDateTimeWeekIsNow(datetime_t* dt, uint8_t window){ //window default of 0

  uint16_t dt_sow = (dt->day_of_week*SEC2DAY)+(dt->hour*SEC2HOUR)+(dt->minute*SEC2MIN)+(dt->second);

  if(
    (dt_sow == RtcTime.Wseconds)||
    (((dt_sow-window) > RtcTime.Wseconds))&&(((dt_sow+window) < RtcTime.Wseconds))
    ){
    return 1;
  }
  return 0;

}


// keep method, but add another
uint8_t mTime::CheckBetweenDateTimes(datetime_t* start, datetime_t* end){

  // uint32_t start_soy = (GetDayOfYear(start->day_of_month,start->month)*86400)+(start->hour*3600)+(start->minute*60)+(start->second);
  // uint32_t end_soy = (GetDayOfYear(end->day_of_month,end->month)*86400)+(end->hour*3600)+(end->minute*60)+(end->second);

  //  if((RtcTime.Yseconds > start_soy)&&(RtcTime.Yseconds < end_soy)){
  //    return 1;
  //  }

  return 0;
  //return ((start > RtcTime.SOD)&&(RtcTime.SOD < end)) ? true : false;
}


// now > future
datetime_t mTime::GetDifferenceInDateTimes(datetime_t* dt1, datetime_t* dt2){

  datetime_t datetime_new;
  memset(&datetime_new,0,sizeof(datetime_new));

  //check if new dt1 > dt2 THEN add day
  uint32_t dt1_sow = (dt1->day_of_week*SEC2DAY)+(dt1->hour*SEC2HOUR)+(dt1->minute*SEC2MIN)+(dt1->second);
  uint32_t dt2_sow = (dt2->day_of_week*SEC2DAY)+(dt2->hour*SEC2HOUR)+(dt2->minute*SEC2MIN)+(dt2->second);
  int32_t diff_sow = dt2_sow - dt1_sow;

    // tkr->mso->MessagePrint("dt1_sow");tkr->mso->MessagePrintln(dt1_sow);
    //   tkr->mso->MessagePrint("dt2_sow");tkr->mso->MessagePrintln(dt2_sow);
    //   tkr->mso->MessagePrint("Bdiff_sow");tkr->mso->MessagePrintln(diff_sow);

  if(diff_sow<0){ //tomorrow = future < now //if negative, add day
      //tkr->mso->MessagePrintln("dt1_sow > dt2_sow");
    diff_sow += SEC2DAY; //add day
  }

  // tkr->mso->MessagePrint("Adiff_sow");
  // tkr->mso->MessagePrintln(diff_sow);

  // break new seconds into datetime
  AddSecondsToDateTime(&datetime_new,(uint32_t)diff_sow);

  // datetime_new.day_of_month = abs(dt1->day_of_month-dt2->day_of_month);
  // datetime_new.day_of_year = abs(dt1->day_of_year-dt2->day_of_year);
  // datetime_new.month = abs(dt1->month-dt2->month);
  // datetime_new.year = abs(dt1->year-dt2->year);
  // datetime_new.hour = abs(dt1->hour-dt2->hour);
  // datetime_new.second = abs(dt1->second-dt2->second);
  // datetime_new.minute = abs(dt1->minute-dt2->minute);
  // //datetime_new.week = abs(dt1->week-dt2->week);
  // datetime_new.day_of_week = abs(dt1->day_of_week-dt2->day_of_week);

  // tkr->mso->MessagePrint("dt1->day_of_week");tkr->mso->MessagePrintln(dt1->day_of_week);
  // tkr->mso->MessagePrint("dt2->day_of_week");tkr->mso->MessagePrintln(dt2->day_of_week);
  // tkr->mso->MessagePrint("datetime_new.day_of_week");tkr->mso->MessagePrintln(datetime_new.day_of_week);
  //
  // if(start_sow>end_sow){ //tomorrow
  //   tkr->mso->MessagePrintln("  if(start_sow>end_sow){ //tomorrow");
  //   AddSecondsToDateTime(&datetime_new,SEC2DAY); //add day
  // }

  return datetime_new;

}


uint8_t mTime::AddSecondsToDateTime(datetime_t* dt_t, uint32_t sec){

  uint8_t years = sec / SEC2YR;
  if (years){
    sec = sec % SEC2YR;
    dt_t->year+=years;
  }

  uint8_t months = sec / SEC2MONTH;
  if (months){
    sec = sec % SEC2MONTH;
    dt_t->month+=months;
  }

  uint8_t weeks = sec / SEC2WEEK;
  if (weeks){
    sec = sec % SEC2WEEK;
    dt_t->week+=weeks;
  }

  uint8_t days = sec / SEC2DAY;
  if (days){
    sec = sec % SEC2DAY;
    dt_t->day_of_week+=days;
  }

  uint8_t hours = sec / SEC2HOUR;
  if (hours){
    sec = sec % SEC2HOUR;
    dt_t->hour+=hours;
  }

  uint8_t minutes = sec / SEC2MIN;
  if (minutes){
    sec = sec % SEC2MIN;
    dt_t->minute+=minutes;
  }

  int seconds = sec; //seconds should now be less than 60 because of minutes
  dt_t->second += seconds;

  return 0;
}


uint32_t mTime::GetSecondsOfDayFromDateTime(datetime_t* dt_t){
  return ((dt_t->hour*SEC2HOUR)+(dt_t->minute*SEC2MIN)+(dt_t->second));
}

#endif // ENABLE_PHASEOUT_TIME__LEGACY_CODE




