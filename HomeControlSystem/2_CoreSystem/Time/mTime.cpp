#include "mTime.h"


int8_t mTime::Tasker(uint8_t function){

  switch(function){
    case FUNC_INIT:
      init();
      testtime.millis = millis();
      testtime.run = false;
    break;
    case FUNC_LOOP: {

      uint8_t show_time_rate = 60; // default
      if(uptime.seconds_nonreset<2*60){ show_time_rate = 1; } // first 2 minutes
      if(uptime.seconds_nonreset<10*60){ show_time_rate = 10; } // first 10 minutes

      if(mTime::TimeReached(&tSavedUptime,show_time_rate*1000)){ // 10 secs then 60 secs
        AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPTIME "%s"),uptime.hhmmss_ctr);    
      }

      if(mTime::TimeReached(&testtime,1)){ // 10 secs then 60 secs
        // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPTIME "TEST %s"),uptime.hhmmss_ctr);    
        // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "FreeRam %d"), ESP.getFreeHeap());
        //       pCONT_sup->DebugFreeMem();
        
        #ifndef DISABLE_SERIAL_LOGGING
        if(ESP.getFreeHeap()<4000){
          Serial.printf("WARNING FreeRam %d\n\r", ESP.getFreeHeap()); Serial.flush();
        }
        #endif
      }
      
    }
    break;
    case FUNC_EVERY_SECOND:{

      UpdateStoredRTCVariables();
      UpdateUpTime();
      timeClient->update();
          
      // Check for midnight
      if((mtime.hour==0)&&(mtime.minute==0)&&(mtime.second==0)&&(lastday_run != mtime.Yday)){
        lastday_run = mtime.Yday;
        pCONT->Tasker_Interface(FUNC_EVERY_MIDNIGHT); 
      }
      if(mtime.second==0){                  pCONT->Tasker_Interface(FUNC_EVERY_MINUTE); }
      if((mtime.second%5)==0){              pCONT->Tasker_Interface(FUNC_EVERY_FIVE_SECOND); }
      if(
        ((mtime.minute%5)==0)&&
        (uptime.seconds_nonreset>60)
      ){                                    pCONT->Tasker_Interface(FUNC_EVERY_FIVE_MINUTE); }
      if(mtime.seconds_nonreset==10){       pCONT->Tasker_Interface(FUNC_ON_SUCCESSFUL_BOOT);}
      if(uptime.seconds_nonreset == 600){   pCONT->Tasker_Interface(FUNC_UPTIME_10_MINUTES); }
      if(uptime.seconds_nonreset == 36000){ pCONT->Tasker_Interface(FUNC_UPTIME_60_MINUTES); }

    }
    break;
    case FUNC_EVERY_MIDNIGHT:
    
    break;
    case FUNC_MQTT_SENDER:
      //SubTasker_MQTTSender();
    break;
  }

}//end

void mTime::init(void){
  
  // RtcInit();
  // initUpTime();

  tSavedStoreRTCUpdate = millis()+2000;
  
  memset(&uptime,0,sizeof(uptime));

  //timeClient = new NTPClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
  timeClient = new NTPClient(ntpUDP, NTP_ADDRESS, (fEnabled_DayLightSavings*60*60), NTP_INTERVAL);

  timeClient->begin();

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

  memcpy(kMonthNamesEnglish,"JanFebMarAprMayJunJulAugSepOctNovDec",sizeof("JanFebMarAprMayJunJulAugSepOctNovDec")-1);// = ;

  // Serial.print("getEpochTime");
  // Serial.println(timeClient->getEpochTime());

  if(timeClient->getEpochTime()>(NTP_OFFSET+2000)){
    fTimeSet = true;
    mtime.isvalid = true;
    //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_TIME "fTimeSet = true"));
  }else{
    fTimeSet = false;
    mtime.isvalid = false;
    //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_TIME "fTimeSet = false"));
  }

  mtime.isvalid = true;

}


// Time elapsed function that updates the time when true
bool mTime::TimeReached(uint32_t* tSaved, uint32_t ElapsedTime){
  if(abs(millis()-*tSaved)>=ElapsedTime){ *tSaved=millis();
    return true;
  }
  return false;
}


bool mTime::TimeReached(TIMEREACHED_HANDLER* tSaved, uint32_t ElapsedTime){
  if(
    (abs(millis()-tSaved->millis)>=ElapsedTime)
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
    (abs(millis()-tSaved->millis)>=ElapsedTime)
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
  if(abs(millis()-*tSaved)>=ElapsedTime){
    return true;
  }
  return false;
}


// Time elapsed function that updates the time when true
uint32_t mTime::MillisElapsed(uint32_t* tSaved){
  return abs(millis()-*tSaved);
}


// Time elapsed function that updates the time when true
// WARNING: doesnt allow for wrap around
bool mTime::MillisReached(uint32_t* tTarget){
  if(millis()>*tTarget){
    return true;
  }
  return false;
}


void mTime::UpdateStoredRTCVariables(void){

  setTime(timeClient->getEpochTime()); // Set to use conversion to units

  mtime.year = year();
  mtime.month = month();
  mtime.Wday = weekday();
  mtime.Mday = day();
  mtime.hour = hour();
  mtime.minute = minute();
  mtime.second = second();

  mtime.Yseconds = timeClient->getEpochTime() - NTP_EPOCH_AT_START_OF_2019;
  mtime.Wseconds = (mtime.Wday*SEC2DAY)+(mtime.hour*SEC2HOUR)+(mtime.minute*SEC2MIN)+(mtime.second);
  mtime.Dseconds = (mtime.hour*SEC2HOUR)+(mtime.minute*SEC2MIN)+(mtime.second);

  AddLog_P(LOG_LEVEL_DEBUG_MORE,
    PSTR(D_LOG_TIME "%02d/%02d/%02d W%02dT%02d:%02d:%02d secs=(%02d,%02d,%02d)"),
    mtime.Mday,mtime.month,mtime.year,
    mtime.Wday,mtime.hour,mtime.minute,mtime.second,
    mtime.Dseconds,mtime.Wseconds,mtime.Yseconds
  ); 

  memset(mtime.hhmmss_ctr,0,sizeof(mtime.hhmmss_ctr));
  timeClient->getFormattedTime(mtime.hhmmss_ctr);
  
}


uint32_t mTime::UpTime(void)
{
  // if (restart_time) {
  //   return utc_time - restart_time;
  // } else {
  //   return uptime;
  // }
  return uptime.seconds_nonreset;
}

// uint32_t MinutesUptime(void)
// {
//   return (UpTime() / 60);
// }

// String GetUptime(void)
// {
//   return GetDuration(UpTime());
// }

// -- Internal counters  -- Internal counters  -- Internal counters  -- Internal counters  -- Internal counters

void mTime::ResetRebootCounter(){
  
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPTIME D_ERROR_UNSUPPORTED "\"rebootinfo\"" ));
  // rebootinfo.count = 0;
  
  //  Settings.rebootcounter_all = 0; //RESET
  //  Settings.rebootcounter_error = 0; //increase

}

void mTime::UpdateUpTime(){

  uptime.seconds_nonreset++;

  // Consider using boottime and nowtime breakdown and using the seconds difference, add another uptime counter to check it
  uptime_seconds_nonreset++;
  // timeClient->getEpochTime()





  uptime.second++;
  if(uptime.second>59){
    uptime.second = 0;
    uptime.minute++;
  }
  if(uptime.minute>59){
    uptime.minute = 0;
    uptime.hour++;
  }
  if(uptime.hour>23){
    uptime.hour = 0;
    uptime.Yday++;
  }

  // change to function get
  memset(uptime.hhmmss_ctr,0,sizeof(uptime.hhmmss_ctr));
  sprintf(uptime.hhmmss_ctr, PSTR("%02dT%02d:%02d:%02d"),uptime.Yday,uptime.hour,uptime.minute,uptime.second);

}


//"Tue Jan 21 12:40:24 2020";//GetTime(0).c_str(); 
const char* mTime::GetUTCTimeCtr(char* _buffer){
  char* buffer;
  SET_BUFFER_AS_GLOBAL_OR_LOCAL(buffer, _buffer);
  sprintf_P(buffer, PSTR("%02d-%02d-%02d %02d:%02d:%02d"),
    mtime.Mday, mtime.month, mtime.year, mtime.hour, mtime.minute, mtime.second
  );
  return buffer;
}
const char* mTime::GetLocalTimeCtr(char* _buffer){
  char* buffer;
  SET_BUFFER_AS_GLOBAL_OR_LOCAL(buffer, _buffer);
  sprintf_P(buffer, PSTR("%02d-%02d-%02d %02d:%02d:%02d"),
    mtime.Mday, mtime.month, mtime.year,
    mtime.hour, mtime.minute, mtime.second
  );
  return buffer;
}
const char* mTime::GetStartDSTTimeCtr(char* _buffer){
  char* buffer;
  SET_BUFFER_AS_GLOBAL_OR_LOCAL(buffer, _buffer);
  sprintf_P(buffer, PSTR("%02d-%02d-%02d %02d:%02d:%02d"),
    mtime.Mday, mtime.month, mtime.year,
    mtime.hour, mtime.minute, mtime.second
  );
  return buffer;
}
const char* mTime::GetEndDSTTimeCtr(char* _buffer){
  char* buffer;
  SET_BUFFER_AS_GLOBAL_OR_LOCAL(buffer, _buffer);
  sprintf_P(buffer, PSTR("%02d-%02d-%02d %02d:%02d:%02d"),
    mtime.Mday, mtime.month, mtime.year,
    mtime.hour, mtime.minute, mtime.second
  );
  return buffer;
}
const char* mTime::GetTimeZoneCtr(char* _buffer){
  char* buffer;
  SET_BUFFER_AS_GLOBAL_OR_LOCAL(buffer, _buffer);
  sprintf_P(buffer, PSTR("%02d-%02d-%02d %02d:%02d:%02d"),
    mtime.Mday, mtime.month, mtime.year,
    mtime.hour, mtime.minute, mtime.second
  );
  return buffer;
}
const char* mTime::GetSunriseCtr(char* _buffer){
  char* buffer;
  SET_BUFFER_AS_GLOBAL_OR_LOCAL(buffer, _buffer);
  sprintf_P(buffer, PSTR("%02d-%02d-%02d %02d:%02d:%02d"),
    mtime.Mday, mtime.month, mtime.year,
    mtime.hour, mtime.minute, mtime.second
  );
  return buffer;
}
const char* mTime::GetSunsetCtr(char* _buffer){
  char* buffer;
  SET_BUFFER_AS_GLOBAL_OR_LOCAL(buffer, _buffer);
  sprintf_P(buffer, PSTR("%02d-%02d-%02d %02d:%02d:%02d"),
    mtime.Mday, mtime.month, mtime.year,
    mtime.hour, mtime.minute, mtime.second
  );
  return buffer;
}




uint32_t mTime::ConvertHHMMSStoSOD(uint8_t hh, uint8_t mm, uint8_t ss){
  return ((hh*3600)+(mm*60)+(ss));
}


uint8_t mTime::CheckBetweenSOD(uint32_t start, uint32_t end){

  if((mtime.Dseconds > start)&&(mtime.Dseconds < end)){
    return 1;
  }

  return 0;
  //return ((start > mtime.SOD)&&(mtime.SOD < end)) ? true : false;
}




//PROBABLY breaks on week rollover!! Needs fixed
// New datetime checker for week only (Wday,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sow = (start->Wday*SEC2DAY)+(start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sow = (end->Wday*SEC2DAY)+(end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = start_sow-mtime.Wseconds;
  int32_t time_until_end = end_sow-mtime.Wseconds;

  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Week_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
    start->hour,start->minute,start->second,time_until_start,
    mtime.Dseconds,
    time_until_end,end->hour,end->minute,end->second
  ); 

  if((start_sow < mtime.Wseconds)&&(mtime.Wseconds < end_sow)){
    return 1;
  }
  return 0;

}

// New datetime checker for week only (Wday,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = mtime.Dseconds-start_sod; 
  int32_t time_until_end = end_sod-mtime.Dseconds;

  //need to add check if start>end, then add 24 hours

  // #ifdef SERIAL_DEBUG_HIGH_LEVEL

    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
      start->hour,start->minute,start->second,time_until_start,
      mtime.Dseconds,
      time_until_end,end->hour,end->minute,end->second
    ); 
    
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d<%02d (%02d) | %02d<%02d (%02d)"),
      start_sod,mtime.Dseconds,(start_sod < mtime.Dseconds?1:0),
      mtime.Dseconds,end_sod,(mtime.Dseconds < end_sod)?1:0
    );
    
   if((start_sod < mtime.Dseconds)&&(mtime.Dseconds < end_sod)){ //now>start AND now<END
     return 1;
   }
  return 0;

}



// New datetime checker for week only (Wday,hours,minutes,seconds)
int8_t mTime::CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = mtime.Dseconds-start_sod; 
  int32_t time_until_end = end_sod-mtime.Dseconds;

  //need to add check if start>end, then add 24 hours

  // #ifdef SERIAL_DEBUG_HIGH_LEVEL

    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Day_DateTimesShort " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
      start->hour,start->minute,start->second,time_until_start,
      mtime.Dseconds,
      time_until_end,end->hour,end->minute,end->second
    ); 
    
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d<%02d (%02d) | %02d<%02d (%02d)"),
      start_sod,mtime.Dseconds,(start_sod < mtime.Dseconds?1:0),
      mtime.Dseconds,end_sod,(mtime.Dseconds < end_sod)?1:0
    );
    
   if((start_sod < mtime.Dseconds)&&(mtime.Dseconds < end_sod)){ //now>start AND now<END
     return 1;
   }
  return 0;

}



// New datetime checker for week only (Wday,hours,minutes,seconds)
uint8_t mTime::CheckDateTimeWeekIsNow(datetime_t* dt, uint8_t window){ //window default of 0

  uint16_t dt_sow = (dt->Wday*SEC2DAY)+(dt->hour*SEC2HOUR)+(dt->minute*SEC2MIN)+(dt->second);

  if(
    (dt_sow == mtime.Wseconds)||
    (((dt_sow-window) > mtime.Wseconds))&&(((dt_sow+window) < mtime.Wseconds))
    ){
    return 1;
  }
  return 0;

}


// keep method, but add another
uint8_t mTime::CheckBetweenDateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_soy = (GetDayOfYear(start->Mday,start->month)*86400)+(start->hour*3600)+(start->minute*60)+(start->second);
  uint32_t end_soy = (GetDayOfYear(end->Mday,end->month)*86400)+(end->hour*3600)+(end->minute*60)+(end->second);

  #ifdef SERIAL_DEBUG_HIGH_LEVEL
    pCONT->mso->MessagePrint("[f::CheckBetweenDateTimes] ");
    int32_t time_until_start = start_soy-mtime.Yseconds;
    int32_t time_until_end = end_soy-mtime.Yseconds;
    // start[until]<this<end[until]
    pCONT->mso->MessagePrint(start_soy);
    pCONT->mso->MessagePrint("[");
    pCONT->mso->MessagePrint(time_until_start);
    pCONT->mso->MessagePrint("]\t");
    pCONT->mso->MessagePrint(mtime.Yseconds);
    pCONT->mso->MessagePrint("\t");
    pCONT->mso->MessagePrint(end_soy);
    pCONT->mso->MessagePrint("[");
    pCONT->mso->MessagePrint(time_until_end);
    pCONT->mso->MessagePrintln("]");
  #endif

   if((mtime.Yseconds > start_soy)&&(mtime.Yseconds < end_soy)){
     return 1;
   }

  return 0;
  //return ((start > mtime.SOD)&&(mtime.SOD < end)) ? true : false;
}


// now > future
datetime_t mTime::GetDifferenceInDateTimes(datetime_t* dt1, datetime_t* dt2){

  datetime_t datetime_new;
  memset(&datetime_new,0,sizeof(datetime_new));

  //check if new dt1 > dt2 THEN add day
  uint32_t dt1_sow = (dt1->Wday*SEC2DAY)+(dt1->hour*SEC2HOUR)+(dt1->minute*SEC2MIN)+(dt1->second);
  uint32_t dt2_sow = (dt2->Wday*SEC2DAY)+(dt2->hour*SEC2HOUR)+(dt2->minute*SEC2MIN)+(dt2->second);
  int32_t diff_sow = dt2_sow - dt1_sow;

    // pCONT->mso->MessagePrint("dt1_sow");pCONT->mso->MessagePrintln(dt1_sow);
    //   pCONT->mso->MessagePrint("dt2_sow");pCONT->mso->MessagePrintln(dt2_sow);
    //   pCONT->mso->MessagePrint("Bdiff_sow");pCONT->mso->MessagePrintln(diff_sow);

  if(diff_sow<0){ //tomorrow = future < now //if negative, add day
      //pCONT->mso->MessagePrintln("dt1_sow > dt2_sow");
    diff_sow += SEC2DAY; //add day
  }

  // pCONT->mso->MessagePrint("Adiff_sow");
  // pCONT->mso->MessagePrintln(diff_sow);

  // break new seconds into datetime
  AddSecondsToDateTime(&datetime_new,(uint32_t)diff_sow);

  PrintDateTime(&datetime_new);

  // datetime_new.Mday = abs(dt1->Mday-dt2->Mday);
  // datetime_new.Yday = abs(dt1->Yday-dt2->Yday);
  // datetime_new.month = abs(dt1->month-dt2->month);
  // datetime_new.year = abs(dt1->year-dt2->year);
  // datetime_new.hour = abs(dt1->hour-dt2->hour);
  // datetime_new.second = abs(dt1->second-dt2->second);
  // datetime_new.minute = abs(dt1->minute-dt2->minute);
  // //datetime_new.week = abs(dt1->week-dt2->week);
  // datetime_new.Wday = abs(dt1->Wday-dt2->Wday);

  // pCONT->mso->MessagePrint("dt1->Wday");pCONT->mso->MessagePrintln(dt1->Wday);
  // pCONT->mso->MessagePrint("dt2->Wday");pCONT->mso->MessagePrintln(dt2->Wday);
  // pCONT->mso->MessagePrint("datetime_new.Wday");pCONT->mso->MessagePrintln(datetime_new.Wday);
  //
  // if(start_sow>end_sow){ //tomorrow
  //   pCONT->mso->MessagePrintln("  if(start_sow>end_sow){ //tomorrow");
  //   AddSecondsToDateTime(&datetime_new,SEC2DAY); //add day
  // }

  return datetime_new;

}


// Break week units into hours, minutes and seconds
void mTime::DateTimeWeek2HHMMSS(datetime_t* dt, uint8_t* hour, uint8_t* minute, uint8_t* second){

  // start_sow
  uint32_t sec = (dt->Wday*SEC2DAY)+(dt->hour*SEC2HOUR)+(dt->minute*SEC2MIN)+(dt->second);

  int hours = sec / SEC2HOUR;
  if (hours){
    sec = sec % SEC2HOUR;
  }

  int minutes = sec / SEC2MIN;
  if (minutes){
    sec = sec % SEC2MIN;
  }

  int seconds = sec; //seconds should now be less than 60 because of minutes

  *hour = hours;
  *minute = minutes;
  *second = seconds;

}


void mTime::PrintDateTime(datetime_t* dt){

  //[Yxx-Mxx-Dxx-Wxx:H:M:S]
  char ctr[80];memset(ctr,0,sizeof(ctr));
  sprintf(ctr, PSTR("%02d-%02d-%04d W%02d T%02d:%02d:%02d"), dt->Wday, dt->month, dt->year, dt->Wday, dt->hour, dt->minute, dt->second);
  // pCONT->mso->MessagePrintln(ctr);

}

uint16_t mTime::GetDayOfYear(uint8_t day, uint8_t month){

  uint16_t days_in_year = 0;
  for(int i=1;i<month;i++){ //without this month

    #ifdef SERIAL_DEBUG_LOW_LEVEL
      Serial.print("GetDaysInMonth\t");
      Serial.print(i);
      Serial.print("\t");
      Serial.println(GetDaysInMonth(i));
    #endif

    days_in_year += GetDaysInMonth(i);
  }

  days_in_year += day; // plus days into this month

  #ifdef SERIAL_DEBUG_LOW_LEVEL
    Serial.print("days_in_year");
    Serial.println(days_in_year);
  #endif

  days_in_year -= 1; //jan 1st is day 0

  return days_in_year;

}

uint16_t mTime::GetDaysInMonth(uint8_t month){

  uint8_t days_in_month=0;
  uint8_t isleapyear = 0;

  switch(month){
    case MONTH_JANUARY:
    case MONTH_MARCH:
    case MONTH_MAY:
    case MONTH_JULY:
    case MONTH_AUGUST:
    case MONTH_OCTOBER:
    case MONTH_DECEMBER: days_in_month = 31; break;
    case MONTH_APRIL:
    case MONTH_JUNE:
    case MONTH_SEPTEMBER:
    case MONTH_NOVEMBER: days_in_month = 30; break;
    case MONTH_FEBRUARY: days_in_month = (isleapyear?29:28); break;
  }

  return days_in_month;

}

uint16_t mTime::DaysInThisMonth(){
  return GetDaysInMonth(mtime.month);
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
    dt_t->Wday+=days;
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

  #ifdef SERIAL_DEBUG_LOW_LEVEL
    char test[150];
    memset(test,0,sizeof(test));
    sprintf(test, "%u years, %u months, %u weeks, %u days, %u hours, %u minutes, %u seconds", years, months, weeks, days, hours, minutes, seconds);
    pCONT->mso->MessagePrintln(test);
  #endif

  return 0;
}


uint32_t mTime::GetSecondsOfDayFromDateTime(datetime_t* dt_t){
  return ((dt_t->hour*SEC2HOUR)+(dt_t->minute*SEC2MIN)+(dt_t->second));
}

//format: HH:MM:SS (check with ::)
datetime_t mTime::GetTimefromCtr(const char* c){

  datetime_t datetime = mtime; //fill with current info

  //01:45:78
  if(!((c[2]==':')&&(c[5]==':'))){ //check format is correct
    // pCONT->mso->MessagePrintln("Time format is not correct");
    //return;
  }
  // for(int i=0;i<8;i++){pCONT->mso->MessagePrintchar(c[i]);}

  datetime.hour = ((c[0]-48)*10)+(c[1]-48);
  datetime.minute = ((c[3]-48)*10)+(c[4]-48);
  datetime.second = ((c[6]-48)*10)+(c[7]-48);

  char ctr[80];memset(ctr,0,sizeof(ctr));
  sprintf(ctr, PSTR("GetTimefromCtr = T%02d:%02d:%02d"),datetime.hour,datetime.minute,datetime.second);
  // pCONT->mso->MessagePrintln(ctr);

  return datetime;
}



const char* mTime::getFormattedTime(datetime_t* dt_t, char* buffer){ //Serial.println("getFormattedTime");
  if(buffer == nullptr){ return 0; }
  // char time_ctr[40]; memset(time_ctr,'\0',sizeof(time_ctr));
  sprintf(buffer, PSTR("%02d:%02d:%02d"), dt_t->hour, dt_t->minute, dt_t->second);
  //sprintf(time_ctr,"%d%d",12,34);
  //Serial.println("time_ctr"); Serial.println(time_ctr);
  return buffer;
}
const char* mTime::getFormattedDateTime(datetime_t* dt_t, char* buffer) {
  if(buffer == nullptr){ return 0; }
  // char time_ctr[24]; memset(time_ctr,0,sizeof(time_ctr));
  sprintf(buffer, PSTR("%02d:%02d:%02dT%02d:%02d:%02d"),dt_t->Mday,dt_t->month,dt_t->year,dt_t->hour,dt_t->minute,dt_t->second);
  return buffer;
}


const char* mTime::getFormattedUptime(char* buffer){ //Serial.println("getFormattedTime");
if(buffer == nullptr){ return 0; }
  //char uptime_ctr[40]; memset(uptime_ctr,0,sizeof(uptime_ctr));
  sprintf(buffer, PSTR("%02dT%02d:%02d:%02d"),
    (int)uptime.Yday,(int)uptime.hour,(int)uptime.minute,(int)uptime.second);
  return buffer;
}


const char* mTime::GetBuildDateAndTime(char* buffer)
{
  if(buffer == nullptr){ return 0; }
  sprintf(buffer,"%s","2017-03-07T11:08:02");
//   // "2017-03-07T11:08:02" - ISO8601:2004
  //  char bdt[40];
//   char *p;
//   char mdate[] = __DATE__;  // "Mar  7 2017"
//   char *smonth = mdate;
//   int day = 0;
//   int year = 0;

//   // sscanf(mdate, "%s %d %d", bdt, &day, &year);  // Not implemented in 2.3.0 and probably too much code
//   uint8_t i = 0;
//   for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(nullptr, " ", &p)) {
//     switch (i++) {
//     case 0:  // Month
//       smonth = str;
//       break;
//     case 1:  // Day
//       day = atoi(str);
//       break;
//     case 2:  // Year
//       year = atoi(str);
//     }
//   }
//   int month = (strstr(kMonthNamesEnglish, smonth) -kMonthNamesEnglish) /3 +1;
//   //snprintf?_P(bdt, sizeof(bdt),
//   sprintf(bdt,
//     PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"),
//     //year, month, day, __TIME__);
//     1, 2, 3, __TIME__);

  // char mdate[] = "__DATE__";  // "Mar  7 2017"
  // char mday[] = "__TIME__";  // "Mar  7 2017"



  // sprintf(bdt, PSTR("%sT%s\0"),mdate,mday);
  // AddLog_P(LOG_LEVEL_TEST,PSTR("bdt=%s"));

  // return bdt;  // 2017-03-07T11:08:02
  return buffer;//PSTR("bdt");  // 2017-03-07T11:08:02
}




// String GetTimeZone(void)
// {
//   char tz[7];

//   snprintf_P(tz, sizeof(tz), PSTR("%+03d:%02d"), time_timezone / 60, abs(time_timezone % 60));

//   return String(tz);  // -03:45
// }

// String GetDuration(uint32_t time)
// {
//   char dt[16];

//   TIME_T ut;
//   BreakTime(time, ut);

//   // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
// //  snprintf_P(dt, sizeof(dt), PSTR("P%dDT%02dH%02dM%02dS"), ut.days, ut.hour, ut.minute, ut.second);

//   // "128 14:35:44" - OpenVMS
//   // "128T14:35:44" - Tasmota
//   snprintf_P(dt, sizeof(dt), PSTR("%dT%02d:%02d:%02d"), ut.days, ut.hour, ut.minute, ut.second);

//   return String(dt);  // 128T14:35:44
// }

const char* mTime::GetDT(uint32_t time, char* buffer)
{
  if(buffer == nullptr){ return 0; }
  // "2017-03-07T11:08:02" - ISO8601:2004

  // char dt[20];
  // TIME_T tmpTime;

  // BreakTime(time, tmpTime);
  // snprintf_P(dt, sizeof(dt), PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
  //   tmpTime.year +1970, tmpTime.month, tmpTime.day_of_month, tmpTime.hour, tmpTime.minute, tmpTime.second);

  // return "2017-03-07T11:08:02";//String(dt);  // 2017-03-07T11:08:02
}

// /*
//  * timestamps in https://en.wikipedia.org/wiki/ISO_8601 format
//  *
//  *  DT_UTC - current data and time in Greenwich, England (aka GMT)
//  *  DT_LOCAL - current date and time taking timezone into account
//  *  DT_RESTART - the date and time this device last started, in local timezone
//  *
//  * Format:
//  *  "2017-03-07T11:08:02-07:00" - if DT_LOCAL and SetOption52 = 1
//  *  "2017-03-07T11:08:02"       - otherwise
//  */
const char* mTime::GetDateAndTime(uint8_t time_type)
{
  // // "2017-03-07T11:08:02-07:00" - ISO8601:2004
  // uint32_t time = local_time;

  // switch (time_type) {
  //   case DT_ENERGY:
  //     time = Settings.energy_kWhtotal_time;
  //     break;
  //   case DT_UTC:
  //     time = utc_time;
  //     break;
  //   case DT_RESTART:
  //     if (restart_time == 0) {
  //       return "";
  //     }
  //     time = restart_time;
  //     break;
  // }
  // String dt = GetDT(time);  // 2017-03-07T11:08:02
  // if (Settings.flag_network_phaseout.time_append_timezone && (DT_LOCAL == time_type)) {
  //   dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
  // }
  return "2017-03-07T11:08:02-07:00";//dt.c_str();  // 2017-03-07T11:08:02-07:00
}

// String GetTime(int type)
// {
//   /* type 1 - Local time
//    * type 2 - Daylight Savings time
//    * type 3 - Standard time
//    */
//   char stime[25];   // Skip newline

//   uint32_t time = utc_time;
//   if (1 == type) time = local_time;
//   if (2 == type) time = daylight_saving_time;
//   if (3 == type) time = standard_time;
//   snprintf_P(stime, sizeof(stime), sntp_get_real_time(time));

//   return String(stime);  // Thu Nov 01 11:41:02 2018
// }


bool  mTime::IsDst(void) //is daylight savings time
{
  // if (Rtc.time_timezone == Settings.toffset[1]) {
  //   return true;
  // }
  return false;
}


uint32_t mTime::MinutesPastMidnight(void)
{
  uint32_t minutes = 0;

  // if (pCONT_set->RtcTime.valid) {
  //   minutes = (pCONT_set->RtcTime.hour *60) + pCONT_set->RtcTime.minute;
  // }
  return minutes;
}

// void BreakTime(uint32_t time_input, TIME_T &tm)
// {
// // break the given time_input into time components
// // this is a more compact version of the C library localtime function
// // note that year is offset from 1970 !!!

//   uint8_t year;
//   uint8_t month;
//   uint8_t month_length;
//   uint32_t time;
//   unsigned long days;

//   time = time_input;
//   tm.second = time % 60;
//   time /= 60;                // now it is minutes
//   tm.minute = time % 60;
//   time /= 60;                // now it is hours
//   tm.hour = time % 24;
//   time /= 24;                // now it is days
//   tm.days = time;
//   tm.day_of_week = ((time + 4) % 7) + 1;  // Sunday is day 1

//   year = 0;
//   days = 0;
//   while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
//     year++;
//   }
//   tm.year = year;            // year is offset from 1970

//   days -= LEAP_YEAR(year) ? 366 : 365;
//   time -= days;              // now it is days in this year, starting at 0
//   tm.day_of_year = time;

//   days = 0;
//   month = 0;
//   month_length = 0;
//   for (month = 0; month < 12; month++) {
//     if (1 == month) { // february
//       if (LEAP_YEAR(year)) {
//         month_length = 29;
//       } else {
//         month_length = 28;
//       }
//     } else {
//       month_length = kDaysInMonth[month];
//     }

//     if (time >= month_length) {
//       time -= month_length;
//     } else {
//       break;
//     }
//   }
//   strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
//   tm.month = month + 1;      // jan is month 1
//   tm.day_of_month = time + 1;         // day of month
//   tm.valid = (time_input > 1451602800);  // 2016-01-01
// }

// uint32_t MakeTime(TIME_T &tm)
// {
// // assemble time elements into time_t
// // note year argument is offset from 1970

//   int i;
//   uint32_t seconds;

//   // seconds from 1970 till 1 jan 00:00:00 of the given year
//   seconds = tm.year * (SECS_PER_DAY * 365);
//   for (i = 0; i < tm.year; i++) {
//     if (LEAP_YEAR(i)) {
//       seconds +=  SECS_PER_DAY;   // add extra days for leap years
//     }
//   }

//   // add days for this year, months start from 1
//   for (i = 1; i < tm.month; i++) {
//     if ((2 == i) && LEAP_YEAR(tm.year)) {
//       seconds += SECS_PER_DAY * 29;
//     } else {
//       seconds += SECS_PER_DAY * kDaysInMonth[i-1];  // monthDay array starts from 0
//     }
//   }
//   seconds+= (tm.day_of_month - 1) * SECS_PER_DAY;
//   seconds+= tm.hour * SECS_PER_HOUR;
//   seconds+= tm.minute * SECS_PER_MIN;
//   seconds+= tm.second;
//   return seconds;
// }

// uint32_t RuleToTime(TimeRule r, int yr)
// {
//   TIME_T tm;
//   uint32_t t;
//   uint8_t m;
//   uint8_t w;                // temp copies of r.month and r.week

//   m = r.month;
//   w = r.week;
//   if (0 == w) {             // Last week = 0
//     if (++m > 12) {         // for "Last", go to the next month
//       m = 1;
//       yr++;
//     }
//     w = 1;                  // and treat as first week of next month, subtract 7 days later
//   }

//   tm.hour = r.hour;
//   tm.minute = 0;
//   tm.second = 0;
//   tm.day_of_month = 1;
//   tm.month = m;
//   tm.year = yr - 1970;
//   t = MakeTime(tm);         // First day of the month, or first day of next month for "Last" rules
//   BreakTime(t, tm);
//   t += (7 * (w - 1) + (r.dow - tm.day_of_week + 7) % 7) * SECS_PER_DAY;
//   if (0 == r.week) {
//     t -= 7 * SECS_PER_DAY;  // back up a week if this is a "Last" rule
//   }
//   return t;
// }

// uint32_t UtcTime(void)
// {
//   return utc_time;
// }

uint32_t mTime::LocalTime(void)
{
  return local_time;
}

uint32_t mTime::Midnight(void)
{
  return midnight;
}

// bool MidnightNow(void)
// {
//   bool mnflg = midnight_now;
//   if (mnflg) midnight_now = 0;
//   return mnflg;
// }

// void RtcSecond(void)
// {
//   TIME_T tmpTime;

// //Serial.println("RTCHERE1");
//   if ((ntp_sync_minute > 59) && (RtcTime.minute > 2)) ntp_sync_minute = 1;                 // If sync prepare for a new cycle
//   uint8_t offset = (uptime < 30) ? RtcTime.second : (((ESP.getChipId() & 0xF) * 3) + 3) ;  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
//   if (!global_state.wifi_down && (offset == RtcTime.second) && ((RtcTime.year < 2016) || (ntp_sync_minute == RtcTime.minute) || ntp_force_sync)) {
//     ntp_time = sntp_get_current_timestamp();
//     if (ntp_time > 1451602800) {  // Fix NTP bug in core 2.4.1/SDK 2.2.1 (returns Thu Jan 01 08:00:10 1970 after power on)
//       ntp_force_sync = false;
//       utc_time = ntp_time;
//       ntp_sync_minute = 60;  // Sync so block further requests
//       if (restart_time == 0) {
//         restart_time = utc_time - uptime;  // save first ntp time as restart time
//       }
//       BreakTime(utc_time, tmpTime);
//       RtcTime.year = tmpTime.year + 1970;
//       daylight_saving_time = RuleToTime(Settings.tflag[1], RtcTime.year);
//       standard_time = RuleToTime(Settings.tflag[0], RtcTime.year);
// //Serial.println("RTCHERE1c");
//       //AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "(" D_UTC_TIME ") %s, (" D_DST_TIME ") %s, (" D_STD_TIME ") %s"), GetTime(0).c_str(), GetTime(2).c_str(), GetTime(3).c_str());
      
// //Serial.println("RTCHERE1d ");
// if (local_time < 1451602800) {  // 2016-01-01
//         rules_flag.time_init = 1;
//       } else {
//         rules_flag.time_set = 1;
//       }
//     } else {
//       ntp_sync_minute++;  // Try again in next minute
//     }
//   }
// //Serial.println("RTCHERE1b");
//   utc_time++;
//   local_time = utc_time;
//   if (local_time > 1451602800) {  // 2016-01-01
//     int16_t timezone_minutes = Settings.timezone_minutes;
//     if (Settings.timezone < 0) { timezone_minutes *= -1; }
//     time_timezone = (Settings.timezone * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
//     if (99 == Settings.timezone) {
//       int32_t dstoffset = Settings.toffset[1] * SECS_PER_MIN;
//       int32_t stdoffset = Settings.toffset[0] * SECS_PER_MIN;
//       if (Settings.tflag[1].hemis) {
//         // Southern hemisphere
//         if ((utc_time >= (standard_time - dstoffset)) && (utc_time < (daylight_saving_time - stdoffset))) {
//           time_timezone = stdoffset;  // Standard Time
//         } else {
//           time_timezone = dstoffset;  // Daylight Saving Time
//         }
//       } else {
//         // Northern hemisphere
//         if ((utc_time >= (daylight_saving_time - stdoffset)) && (utc_time < (standard_time - dstoffset))) {
//           time_timezone = dstoffset;  // Daylight Saving Time
//         } else {
//           time_timezone = stdoffset;  // Standard Time
//         }
//       }
//     }
//     local_time += time_timezone;
//     time_timezone /= 60;
//     if (!Settings.energy_kWhtotal_time) { Settings.energy_kWhtotal_time = local_time; }
//   }
  
// //Serial.println("RTCHERE2");
//   BreakTime(local_time, RtcTime);

// //Serial.println("RTCHERE3");
//   if (RtcTime.valid) {
//     if (!midnight) {
//       midnight = local_time - (RtcTime.hour * 3600) - (RtcTime.minute * 60) - RtcTime.second;
//     }
//     if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) {
//       midnight = local_time;
//       midnight_now = 1;
//     }
//   }

//   RtcTime.year += 1970;


// //Serial.println("RTCHERE4");


// }

void mTime::RtcInit(void)
{
  // sntp_setservername(0, Settings.ntp_server[0]);
  // sntp_setservername(1, Settings.ntp_server[1]);
  // sntp_setservername(2, Settings.ntp_server[2]);
  // sntp_stop();
  // sntp_set_timezone(0);      // UTC time
  // sntp_init();
  // utc_time = 0;
  // BreakTime(utc_time, RtcTime);
  // TickerRtc.attach(1, RtcSecond);
}
//#endif


const char* mTime::GetDOWLongctr(uint8_t Wday, char* buffer){
  // if(buffer == nullptr){ return 0; }
  // return (Wday == 2 ? "Monday" :
  //     (Wday == 3 ? "Tuesday" :
  //     (Wday == 4 ? "Wednesday" :
  //     (Wday == 5 ? "Thursday" :
  //     (Wday == 6 ? "Friday" :
  //     (Wday == 7 ? "Saturday" :
  //     (Wday == 1 ? "Sunday" :
  //     "unk")))))));
  
  memcpy(buffer,"UNK",3);


  return buffer;
}

// Add short and long versions
const char* mTime::GetDOWShortctr(uint8_t Wday, char* buffer){
  // if(buffer == nullptr){ return 0; }

  memcpy(buffer,"UNK",3);


  return buffer;
  // return (Wday == 2 ? "Mon" :
  //     (Wday == 3 ? "Tue" :
  //     (Wday == 4 ? "Wed" :
  //     (Wday == 5 ? "Thu" :
  //     (Wday == 6 ? "Fri" :
  //     (Wday == 7 ? "Sat" :
  //     (Wday == 1 ? "Sun" :
  //     "unk")))))));
}
