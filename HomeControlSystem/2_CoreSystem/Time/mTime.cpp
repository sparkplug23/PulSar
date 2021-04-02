#include "mTime.h"



const char* mTime::PM_MODULE_CORE_TIME_CTR = D_MODULE_CORE_TIME_CTR;
const char* mTime::PM_MODULE_CORE_TIME_FRIENDLY_CTR = D_MODULE_CORE_TIME_FRIENDLY_CTR;


int8_t mTime::Tasker(uint8_t function){

// return 0;
  // DEBUG_PRINT_FUNCTION_NAME_TEST;

  switch(function){
    case FUNC_INIT:
      init();
      testtime.millis = millis();
      testtime.run = false;
    break;
  }

  switch(function){
    
    case FUNC_LOOP: {

      // Temp fix for ticker not working
      #ifdef ESP32
      if(mTime::TimeReached(&tSavedTicker_ESP32, 1000)){
        RtcSecond();
      }
      #endif //ESP32

      uint8_t show_time_rate = 60; // default
      if(uptime.seconds_nonreset<2*60){ show_time_rate = 1; } // first 2 minutes
      if(uptime.seconds_nonreset<10*60){ show_time_rate = 10; } // first 10 minutes

      if(mTime::TimeReached(&tSavedUptime,show_time_rate*1000)){ // 10 secs then 60 secs
        // #ifdef ENABLE_LOG_LEVEL_INFO
        //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPTIME "%s"),uptime.hhmmss_ctr);    
        // #endif// ENABLE_LOG_LEVEL_INFO
      }

      if(mTime::TimeReached(&testtime,1)){ // 10 secs then 60 secs
        // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPTIME "TEST %s"),uptime.hhmmss_ctr);    
        // AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION "FreeRam %d"), ESP.getFreeHeap());
        //       pCONT_sup->DebugFreeMem();
        
        #ifndef DISABLE_SERIAL_LOGGING
        // if(ESP.getFreeHeap()<4000){
        //   // Serial.printf("WARNING FreeRam %d\n\r", ESP.getFreeHeap()); Serial.flush();
        // }
        #endif
      }
      
    }break;
    case FUNC_EVERY_SECOND:{

      //Serial.println(GetUptime());
      
      pCONT_time->WifiPollNtp();

      //   // Serial.printf("time_start1=%d\n\r",millis()-time_start);
        UpdateStoredRTCVariables();
      //   // Serial.printf("time_start2=%d\n\r",millis()-time_start);
        UpdateUpTime();

      // Check for midnight
      if((RtcTime.hour==0)&&(RtcTime.minute==0)&&(RtcTime.second==0)&&(lastday_run != RtcTime.Yday)){
        lastday_run = RtcTime.Yday;
        pCONT->Tasker_Interface(FUNC_EVERY_MIDNIGHT); 
      }
      if(RtcTime.second==0){                  pCONT->Tasker_Interface(FUNC_EVERY_MINUTE); }
      if((RtcTime.second%5)==0){              pCONT->Tasker_Interface(FUNC_EVERY_FIVE_SECOND); }
      if(
        ((RtcTime.minute%5)==0)&&
        (uptime.seconds_nonreset>60)
      ){                                    pCONT->Tasker_Interface(FUNC_EVERY_FIVE_MINUTE); }

      //I need another for stable boot
      if(RtcTime.seconds_nonreset==10){       pCONT->Tasker_Interface(FUNC_ON_BOOT_SUCCESSFUL);}

      if(RtcTime.seconds_nonreset==10){       pCONT->Tasker_Interface(FUNC_BOOT_MESSAGE);}
      if(uptime.seconds_nonreset == 600){   pCONT->Tasker_Interface(FUNC_UPTIME_10_MINUTES); }
      if(uptime.seconds_nonreset == 36000){ pCONT->Tasker_Interface(FUNC_UPTIME_60_MINUTES); }

    }break;
    case FUNC_EVERY_MIDNIGHT:
    
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_SENDER:
      //SubTasker_MQTTSender();
    break;
    case FUNC_MQTT_CONNECTED:{
      char message[50];
      memset(message,0,sizeof(message));
      sprintf_P(message,PSTR("{\"connected\":{\"time\":\"%s\"}}"), uptime.hhmmss_ctr);
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog_P(LOG_LEVEL_INFO,PSTR("FUNC_MQTT_CONNECTED %s %d"),message, strlen(message));
    #endif// ENABLE_LOG_LEVEL_INFO
      pCONT_mqtt->ppublish("status/system/mqtt/event",message,false); //reconnect message
    }
    break;
    #endif // USE_MODULE_NETWORK_MQTT
  }

}//end

void mTime::init(void){
  
  RtcInit();
  // initUpTime();

  tSavedStoreRTCUpdate = millis()+2000;
  
  memset(&uptime,0,sizeof(uptime));

// DEBUG_LINE_HERE;
  //timeClient = new NTPClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
  // timeClient = new NTPClient(ntpUDP, NTP_ADDRESS, (fEnabled_DayLightSavings?NTP_OFFSET:0), NTP_INTERVAL);

  // DEBUG_LINE_HERE;


  // DEBUG_LINE_HERE;
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

  // memcpy(kMonthNamesEnglish,"JanFebMarAprMayJunJulAugSepOctNovDec",sizeof("JanFebMarAprMayJunJulAugSepOctNovDec")-1);// = ;

  // Serial.print("getEpochTime");
  // Serial.println(timeClient->getEpochTime());


}


bool mTime::CheckOrStartNTPService(){

  if(pCONT_wif->WifiCheckIpConnected()){
    // If just connected, and not already started
    if(!settings.timeclient_is_started){ 
      // timeClient->begin();
      //Serial.println("CheckOrStartNTPService TRUE");
      settings.timeclient_is_started = true;
    }
  }else{    
    //Serial.println("CheckOrStartNTPService False");
  }

  if(!settings.timeclient_is_started){
    fTimeSet = false;
    RtcTime.isvalid = false;
    return false; // failed to start
  }


// #ifdef ESP8266
// #else
//   settings.timeclient_is_started = false;
// #endif

  // if(
  //   timeClient->getEpochTime()>(
  //     (fEnabled_DayLightSavings?NTP_OFFSET:0)+2000
  //     )
  //   ){

  if(RtcTime.valid){
    fTimeSet = true;
    RtcTime.isvalid = true;
    //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_TIME "fTimeSet = true"));
  }else{
    fTimeSet = false;
    RtcTime.isvalid = false;
    //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_TIME "fTimeSet = false"));
  }

  RtcTime.isvalid = true;

  return true;

}



uint32_t mTime::GetTimeOfDay_Seconds(void){
  return RtcTime.Dseconds;
}

const char* mTime::ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds_tod, char* buffer, uint8_t buflen){
  
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

// const char* mTime::ConvertTimeOfDay_Seconds_HHMMSS(uint32_t seconds, char* buffer, uint8_t buflen){
//   snprintf_P(buffer, buflen, 
//               PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              
              
//               //RtcTime.hour, RtcTime.minute, RtcTime.second
//   );
//   return buffer;
// }





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

//phase out or join methods together
  // setTime(Rtc.utc_time);//timeClient->getEpochTime()); // Set to use conversion to units

  RtcTime.Yseconds = 0;//timeClient->getEpochTime() - NTP_EPOCH_AT_START_OF_2019;
  RtcTime.Wseconds = (RtcTime.Wday*SEC2DAY)+(RtcTime.hour*SEC2HOUR)+(RtcTime.minute*SEC2MIN)+(RtcTime.second);
  RtcTime.Dseconds = (RtcTime.hour*SEC2HOUR)+(RtcTime.minute*SEC2MIN)+(RtcTime.second);

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG_MORE,
    PSTR(D_LOG_TIME "%02d/%02d/%02d W%02dT%02d:%02d:%02d secs=(%02d,%02d,%02d)"),
    RtcTime.Mday,RtcTime.month,RtcTime.year,
    RtcTime.Wday,RtcTime.hour,RtcTime.minute,RtcTime.second,
    RtcTime.Dseconds,RtcTime.Wseconds,RtcTime.Yseconds
  ); 
  #endif// ENABLE_LOG_LEVEL_INFO

  memset(RtcTime.hhmmss_ctr,0,sizeof(RtcTime.hhmmss_ctr));
  sprintf_P(RtcTime.hhmmss_ctr, PSTR("%02d:%02d:%02d"),RtcTime.hour,RtcTime.minute,RtcTime.second);
  // timeClient->getFormattedTime(RtcTime.hhmmss_ctr);
  
}


void mTime::TickRTCVariablesWithUptime(void){

  // setTime(timeClient->getEpochTime()); // Set to use conversion to units

  // RtcTime.year = year();
  // RtcTime.month = month();
  // RtcTime.Wday = weekday();
  // RtcTime.Mday = day();
  // RtcTime.hour = hour();
  // RtcTime.minute = minute();
  // RtcTime.second = second();

  // RtcTime.second++;
  // if(RtcTime.second>59){
  //   RtcTime.second = 0;
  //   RtcTime.minute++;
  // }
  // if(RtcTime.minute>59){
  //   RtcTime.minute = 0;
  //   RtcTime.hour++;
  // }
  // if(RtcTime.hour>23){
  //   RtcTime.hour = 0;
  //   RtcTime.Yday++;
  // }

  RtcTime = uptime;

  // RtcTime.Yseconds = timeClient->getEpochTime() - NTP_EPOCH_AT_START_OF_2019;
  // RtcTime.Wseconds = (RtcTime.Wday*SEC2DAY)+(RtcTime.hour*SEC2HOUR)+(RtcTime.minute*SEC2MIN)+(RtcTime.second);
  // RtcTime.Dseconds = (RtcTime.hour*SEC2HOUR)+(RtcTime.minute*SEC2MIN)+(RtcTime.second);

  // AddLog_P(LOG_LEVEL_DEBUG_MORE,
  //   PSTR(D_LOG_TIME "%02d/%02d/%02d W%02dT%02d:%02d:%02d secs=(%02d,%02d,%02d)"),
  //   RtcTime.Mday,RtcTime.month,RtcTime.year,
  //   RtcTime.Wday,RtcTime.hour,RtcTime.minute,RtcTime.second,
  //   RtcTime.Dseconds,RtcTime.Wseconds,RtcTime.Yseconds
  // ); 

  // memset(RtcTime.hhmmss_ctr,0,sizeof(RtcTime.hhmmss_ctr));
  // timeClient->getFormattedTime(RtcTime.hhmmss_ctr);
  
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


// -- Internal counters  -- Internal counters  -- Internal counters  -- Internal counters  -- Internal counters

void mTime::ResetRebootCounter(){
  
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_UPTIME D_ERROR_UNSUPPORTED "\"rebootinfo\"" ));
    #endif// ENABLE_LOG_LEVEL_INFO
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
  sprintf_P(uptime.hhmmss_ctr, PSTR("%02dT%02d:%02d:%02d"),uptime.Yday,uptime.hour,uptime.minute,uptime.second);

}


time_short_t mTime::GetTimeShortNow(){

  time_short_t now;

  now.Wday = RtcTime.Wday;
  now.hour = RtcTime.hour;
  now.minute = RtcTime.minute;
  now.second = RtcTime.second;

  return now;

}

const char* mTime::ConvertShortTime_HHMMSS(time_short_t* time, char* buffer, uint8_t buflen){
  
  snprintf_P(buffer, buflen, 
              PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d"),
              time->hour, time->minute, time->second
  );
  return buffer;
}


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
      AddLog_P(LOG_LEVEL_TEST, PSTR("Invalid time"));
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
    AddLog_P(LOG_LEVEL_TEST, PSTR("Parse_Time_TimeShortCtr_To_TimeShort NOT D found"));
  }

  if(includes_week){
    time_s.Wday   = (uint8_t) strtol( &time_ctr[0], NULL, 10);  //days 1-7 so 0 means none set
    time_s.hour   = (uint8_t) strtol( &time_ctr[3], NULL, 10);
    time_s.minute = (uint8_t) strtol( &time_ctr[6], NULL, 10);
    time_s.second = (uint8_t) strtol( &time_ctr[9], NULL, 10);
  }else{
    time_s.Wday   = 0;
    time_s.hour   = (uint8_t) strtol( &time_ctr[0], NULL, 10);
    time_s.minute = (uint8_t) strtol( &time_ctr[3], NULL, 10);
    time_s.second = (uint8_t) strtol( &time_ctr[6], NULL, 10);
  }

  pCONT_time->PrintDateTime(time_s);

  return time_s;

}




// //"Tue Jan 21 12:40:24 2020";//GetTime(0).c_str(); 
// const char* mTime::GetUTCTimeCtr(char* buffer, uint8_t buflen){
//   return GetDateAndTimeCtr(DT_UTC, buffer, buflen);
// }
// const char* mTime::GetLocalTimeCtr(char* buffer, uint8_t buflen){
//   return GetDateAndTimeCtr(DT_LOCALNOTZ, buffer, buflen);
// }
// const char* mTime::GetStartDSTTimeCtr(char* buffer, uint8_t buflen){
//   return GetDateAndTimeCtr(DT_DST, buffer, buflen);
// }
// const char* mTime::GetEndDSTTimeCtr(char* buffer, uint8_t buflen){
//   return GetDateAndTimeCtr(DT_STD, buffer, buflen);
// }
// const char* mTime::GetTimeZoneCtr(char* _buffer){
//   return GetTimeZone();
// }
// const char* mTime::GetSunriseCtr(char* buffer, uint8_t buflen){
//   return GetSunTimeAtHorizon(0, buffer, buflen);
// }
// const char* mTime::GetSunsetCtr(char* buffer, uint8_t buflen){
//   return GetSunTimeAtHorizon(1, buffer, buflen);
// }




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
// New datetime checker for week only (Wday,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Week_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sow = (start->Wday*SEC2DAY)+(start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sow = (end->Wday*SEC2DAY)+(end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = start_sow-RtcTime.Wseconds;
  int32_t time_until_end = end_sow-RtcTime.Wseconds;

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Week_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
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

// New datetime checker for week only (Wday,hours,minutes,seconds)
uint8_t mTime::CheckBetween_Day_DateTimes(datetime_t* start, datetime_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = RtcTime.Dseconds-start_sod; 
  int32_t time_until_end = end_sod-RtcTime.Dseconds;

  //need to add check if start>end, then add 24 hours

  // #ifdef SERIAL_DEBUG_HIGH_LEVEL

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
      start->hour,start->minute,start->second,time_until_start,
      RtcTime.Dseconds,
      time_until_end,end->hour,end->minute,end->second
    ); 
    
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_TIME "CheckBetween_Day_DateTimes " "%02d<%02d (%02d) | %02d<%02d (%02d)"),
      start_sod,RtcTime.Dseconds,(start_sod < RtcTime.Dseconds?1:0),
      RtcTime.Dseconds,end_sod,(RtcTime.Dseconds < end_sod)?1:0
    );
    #endif// ENABLE_LOG_LEVEL_INFO
    
   if((start_sod < RtcTime.Dseconds)&&(RtcTime.Dseconds < end_sod)){ //now>start AND now<END
     return 1;
   }
  return 0;

}



// New datetime checker for week only (Wday,hours,minutes,seconds)
int8_t mTime::CheckBetween_Day_DateTimesShort(time_short_t* start, time_short_t* end){

  uint32_t start_sod = (start->hour*SEC2HOUR)+(start->minute*SEC2MIN)+(start->second);
  uint32_t end_sod =   (end->hour*SEC2HOUR)+(end->minute*SEC2MIN)+(end->second);

  int32_t time_until_start = RtcTime.Dseconds-start_sod; 
  int32_t time_until_end = end_sod-RtcTime.Dseconds;

  //if times are equal, return early as false
  if(start_sod == end_sod) return false;

  //need to add check if start>end, then add 24 hours
  if(end_sod < start_sod){
    end_sod += SECS_PER_DAY;
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_TIME "end<start, Add 24 hours"));
  }

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_TIME "%02d:%02d:%02d (%02d) | (%02d) | (%02d) %02d:%02d:%02d"),
    start->hour,start->minute,start->second,time_until_start,
    RtcTime.Dseconds,
    time_until_end,end->hour,end->minute,end->second
  );
  #endif// ENABLE_LOG_LEVEL_INFO
  
  if((start_sod < RtcTime.Dseconds)&&(RtcTime.Dseconds < end_sod)){ //now>start AND now<END
    return 1;
  }
  return 0;

}



// New datetime checker for week only (Wday,hours,minutes,seconds)
uint8_t mTime::CheckDateTimeWeekIsNow(datetime_t* dt, uint8_t window){ //window default of 0

  uint16_t dt_sow = (dt->Wday*SEC2DAY)+(dt->hour*SEC2HOUR)+(dt->minute*SEC2MIN)+(dt->second);

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

  uint32_t start_soy = (GetDayOfYear(start->Mday,start->month)*86400)+(start->hour*3600)+(start->minute*60)+(start->second);
  uint32_t end_soy = (GetDayOfYear(end->Mday,end->month)*86400)+(end->hour*3600)+(end->minute*60)+(end->second);

  #ifdef SERIAL_DEBUG_HIGH_LEVEL
    pCONT->mso->MessagePrint("[f::CheckBetweenDateTimes] ");
    int32_t time_until_start = start_soy-RtcTime.Yseconds;
    int32_t time_until_end = end_soy-RtcTime.Yseconds;
    // start[until]<this<end[until]
    pCONT->mso->MessagePrint(start_soy);
    pCONT->mso->MessagePrint("[");
    pCONT->mso->MessagePrint(time_until_start);
    pCONT->mso->MessagePrint("]\t");
    pCONT->mso->MessagePrint(RtcTime.Yseconds);
    pCONT->mso->MessagePrint("\t");
    pCONT->mso->MessagePrint(end_soy);
    pCONT->mso->MessagePrint("[");
    pCONT->mso->MessagePrint(time_until_end);
    pCONT->mso->MessagePrintln("]");
  #endif

   if((RtcTime.Yseconds > start_soy)&&(RtcTime.Yseconds < end_soy)){
     return 1;
   }

  return 0;
  //return ((start > RtcTime.SOD)&&(RtcTime.SOD < end)) ? true : false;
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

  PrintDateTime(datetime_new);

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


void mTime::PrintDateTime(datetime_t dt){

  //[Yxx-Mxx-Dxx-Wxx:H:M:S]
  char ctr[80];memset(ctr,0,sizeof(ctr));
  sprintf_P(ctr, PSTR("%02d-%02d-%04d W%02d T%02d:%02d:%02d"), dt.Wday, dt.month, dt.year, dt.Wday, dt.hour, dt.minute, dt.second);
  // pCONT.mso.MessagePrintln(ctr);

}

void mTime::PrintDateTime(time_short_t dt){

  char buffer[40];memset(buffer,0,sizeof(buffer));
  sprintf_P(buffer, PSTR("W%02d T%02d:%02d:%02d"), dt.Wday, dt.hour, dt.minute, dt.second);
  AddLog_P(LOG_LEVEL_TEST,PSTR("PrintDateTime>\"%s\""),buffer);
  
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
  return GetDaysInMonth(RtcTime.month);
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

  // #ifdef SERIAL_DEBUG_LOW_LEVEL
  //   char test[150];
  //   memset(test,0,sizeof(test));
  //   sprintf(test, "%u years, %u months, %u weeks, %u days, %u hours, %u minutes, %u seconds", years, months, weeks, days, hours, minutes, seconds);
  //   pCONT->mso->MessagePrintln(test);
  // #endif

  return 0;
}


uint32_t mTime::GetSecondsOfDayFromDateTime(datetime_t* dt_t){
  return ((dt_t->hour*SEC2HOUR)+(dt_t->minute*SEC2MIN)+(dt_t->second));
}

//format: HH:MM:SS (check with ::)
datetime_t mTime::GetTimefromCtr(const char* c){

  datetime_t datetime = RtcTime; //fill with current info

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
  sprintf_P(ctr, PSTR("GetTimefromCtr = T%02d:%02d:%02d"),datetime.hour,datetime.minute,datetime.second);
  // pCONT->mso->MessagePrintln(ctr);

  return datetime;
}



const char* mTime::getFormattedTime(datetime_t* dt_t, char* buffer, uint8_t buflen){ //Serial.println("getFormattedTime");
  if(buffer == nullptr){ return 0; }
  // char time_ctr[40]; memset(time_ctr,'\0',sizeof(time_ctr));
  sprintf_P(buffer, PSTR("%02d:%02d:%02d"), dt_t->hour, dt_t->minute, dt_t->second);
  //sprintf(time_ctr,"%d%d",12,34);
  //Serial.println("time_ctr"); Serial.println(time_ctr);
  return buffer;
}
const char* mTime::getFormattedDateTime(datetime_t* dt_t, char* buffer, uint8_t buflen) {
  if(buffer == nullptr){ return 0; }
  // char time_ctr[24]; memset(time_ctr,0,sizeof(time_ctr));
  sprintf_P(buffer, PSTR("%02d:%02d:%02dT%02d:%02d:%02d"),dt_t->Mday,dt_t->month,dt_t->year,dt_t->hour,dt_t->minute,dt_t->second);
  return buffer;
}


const char* mTime::getFormattedUptime(char* buffer, uint8_t buflen){ //Serial.println("getFormattedTime");
if(buffer == nullptr){ return 0; }
  //char uptime_ctr[40]; memset(uptime_ctr,0,sizeof(uptime_ctr));
  sprintf_P(buffer, PSTR("%02dT%02d:%02d:%02d"),
    (int)uptime.Yday,(int)uptime.hour,(int)uptime.minute,(int)uptime.second);
  return buffer;
}


const char* mTime::GetBuildDateAndTime(char* buffer, uint8_t buflen)
{
  if(buffer == nullptr){ return 0; }
  sprintf_P(buffer,PSTR("2017-03-07T11:08:02"));
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




String mTime::GetDuration(uint32_t time)
{
  char dt[16];

  datetime_t ut;
  // TIME_T ut;
  BreakTime(time, ut);

  // "P128DT14H35M44S" - ISO8601:2004 - https://en.wikipedia.org/wiki/ISO_8601 Durations
//  snprintf_P(dt, sizeof(dt), PSTR("P%dDT%02dH%02dM%02dS"), ut.days, ut.hour, ut.minute, ut.second);

  // "128 14:35:44" - OpenVMS
  // "128T14:35:44" - Tasmota
  snprintf_P(dt, sizeof(dt), PSTR("%dT%02d:%02d:%02d"), ut.days, ut.hour, ut.minute, ut.second);

  return String(dt);  // 128T14:35:44
}

const char* mTime::GetDT(uint32_t time, char* buffer, uint8_t buflen)
{
  // if(buffer == nullptr){ return 0; }
  // "2017-03-07T11:08:02" - ISO8601:2004
  datetime_t tmpTime;
  BreakTime(time, tmpTime);
  snprintf_P(buffer, buflen, PSTR("%04d-%02d-%02dT%02d:%02d:%02d"),
    tmpTime.year +1970, tmpTime.month, tmpTime.Mday, tmpTime.hour, tmpTime.minute, tmpTime.second);

  return buffer;  // 2017-03-07T11:08:02
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
// const char* mTime::GetDateAndTimeCtr(uint8_t time_type)
// {
//   // // "2017-03-07T11:08:02-07:00" - ISO8601:2004
//   // uint32_t time = local_time;

//   // switch (time_type) {
//   //   case DT_ENERGY:
//   //     time = Settings.energy_kWhtotal_time;
//   //     break;
//   //   case DT_UTC:
//   //     time = utc_time;
//   //     break;
//   //   case DT_RESTART:
//   //     if (restart_time == 0) {
//   //       return "";
//   //     }
//   //     time = restart_time;
//   //     break;
//   // }
//   // String dt = GetDT(time);  // 2017-03-07T11:08:02
//   // if (Settings.flag_network.time_append_timezone && (DT_LOCAL == time_type)) {
//   //   dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
//   // }
//   return "2017-03-07T11:08:02-07:00";//dt.c_str();  // 2017-03-07T11:08:02-07:00
// }

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



uint32_t mTime::RuleToTime(TimeRule r, int yr)
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
  tm.Wday = 1;
  tm.month = m;
  tm.year = yr - 1970;
  t = MakeTime(tm);         // First day of the month, or first day of next month for "Last" rules
  BreakTime(t, tm);
  t += (7 * (w - 1) + (r.dow - tm.Wday + 7) % 7) * SECS_PER_DAY;
  if (0 == r.week) {
    t -= 7 * SECS_PER_DAY;  // back up a week if this is a "Last" rule
  }
  return t;
}

// uint32_t UtcTime(void)
// {
//   return utc_time;
// }

uint32_t mTime::LocalTime(void)
{
  return Rtc.local_time;
}

uint32_t mTime::Midnight(void)
{
  return Rtc.midnight;
}

// bool MidnightNow(void)
// {
//   bool mnflg = midnight_now;
//   if (mnflg) midnight_now = 0;
//   return mnflg;
// }



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



/*********************************************************************************************\
 * Sources: Time by Michael Margolis and Paul Stoffregen (https://github.com/PaulStoffregen/Time)
 *          Timezone by Jack Christensen (https://github.com/JChristensen/Timezone)
\*********************************************************************************************/

uint32_t UtcTime(void)
{
  // return Rtc.utc_time;
}

uint32_t LocalTime(void)
{
  // return Rtc.local_time;
}

uint32_t Midnight(void)
{
  // return Rtc.midnight;
}

bool MidnightNow(void)
{
  // if (Rtc.midnight_now) {
  //   Rtc.midnight_now = false;
  //   return true;
  // }
  // return false;
}


String GetBuildDateAndTime(void)
{
  // "2017-03-07T11:08:02" - ISO8601:2004
  // char bdt[21];
  // char *p;
  // char mdate[] = __DATE__;  // "Mar  7 2017"
  // char *smonth = mdate;
  // int day = 0;
  // int year = 0;

  // // sscanf(mdate, "%s %d %d", bdt, &day, &year);  // Not implemented in 2.3.0 and probably too much code
  // uint8_t i = 0;
  // for (char *str = strtok_r(mdate, " ", &p); str && i < 3; str = strtok_r(nullptr, " ", &p)) {
  //   switch (i++) {
  //   case 0:  // Month
  //     smonth = str;
  //     break;
  //   case 1:  // Day
  //     day = atoi(str);
  //     break;
  //   case 2:  // Year
  //     year = atoi(str);
  //   }
  // }
  // int month = (strstr(kMonthNamesEnglish, smonth) -kMonthNamesEnglish) /3 +1;
  // snprintf_P(bdt, sizeof(bdt), PSTR("%d" D_YEAR_MONTH_SEPARATOR "%02d" D_MONTH_DAY_SEPARATOR "%02d" D_DATE_TIME_SEPARATOR "%s"), year, month, day, __TIME__);
  // return String(bdt);  // 2017-03-07T11:08:02
}

String GetMinuteTime(uint32_t minutes)
{
  // char tm[6];
  // snprintf_P(tm, sizeof(tm), PSTR("%02d:%02d"), minutes / 60, minutes % 60);

  // return String(tm);  // 03:45
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
char* mTime::GetDateAndTimeCtr(uint8_t time_type, char* buffer, uint8_t buflen)
{
  // "2017-03-07T11:08:02-07:00" - ISO8601:2004

  // Types that write into the buffer directly
  switch(time_type){
    case DT_TIMEZONE:
      snprintf_P(buffer, buflen, PSTR("%+03d:%02d"), Rtc.time_timezone / 60, abs(Rtc.time_timezone % 60));
      return buffer;
    break;
    case DT_SUNRISE:
      return GetSunTimeAtHorizon(0, buffer, buflen);
    break;
    case DT_SUNSET:
      return GetSunTimeAtHorizon(1, buffer, buflen);
    break;
  }
  
  // Types that use breaktime to write buffer
  uint32_t time = Rtc.local_time;
  switch (time_type) {
    case DT_UTC:
      time = Rtc.utc_time;
      break;
   case DT_LOCALNOTZ:  // Is default anyway but allows for not appendig timezone
     time = Rtc.local_time;
     break;
    case DT_DST:
      time = Rtc.daylight_saving_time;
      break;
    case DT_STD:
      time = Rtc.standard_time;
      break;
//     case DT_RESTART:
//       if (Rtc.restart_time == 0) {
//         return "";
//       }
//       time = Rtc.restart_time;
//       break;
//     case DT_ENERGY:
//       time = Settings.energy_kWhtotal_time;
//       break;
//     case DT_BOOTCOUNT:
//       time = Settings.bootcount_reset_time;
//       break;

  }

  GetDT(time, buffer, buflen);  // 2017-03-07T11:08:02

//   if (DT_LOCAL_MILLIS == time_type) {
//     char ms[10];
//     snprintf_P(ms, sizeof(ms), PSTR(".%03d"), RtcMillis());
//     dt += ms;
//     time_type = DT_LOCAL;
//   }

//   if (Settings.flag3.time_append_timezone && (DT_LOCAL == time_type)) {  // SetOption52 - Append timezone to JSON time
//     dt += GetTimeZone();    // 2017-03-07T11:08:02-07:00
//   }
  return buffer;  // 2017-03-07T11:08:02-07:00
}

uint32_t UpTime(void)
{
  // if (Rtc.restart_time) {
  //   return Rtc.utc_time - Rtc.restart_time;
  // } else {
  //   return uptime;
  // }
}

uint32_t mTime::MinutesUptime(void)
{
  return (UpTime() / 60);
}

String mTime::GetUptime(void)
{
  return GetDuration(UpTime());
}

uint32_t mTime::MinutesPastMidnight(void)
{
  uint32_t minutes = 0;

  if (RtcTime.valid) {
    minutes = (RtcTime.hour *60) + RtcTime.minute;
  }
  return minutes;
}

uint32_t mTime::RtcMillis(void) {
  return (millis() - Rtc.millis) % 1000;
}

// void mTime::BreakTime(uint32_t time_input, struct TIME_T &tm)
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
//   // strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
//   tm.month = month + 1;      // jan is month 1
//   tm.day_of_month = time + 1;         // day of month
//   tm.valid = (time_input > START_VALID_TIME);  // 2016-01-01
// }

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

void mTime::BreakTime(uint32_t time_input, datetime_t &tm)
{
// break the given time_input into time components
// this is a more compact version of the C library localtime function
// note that year is offset from 1970 !!!

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
  tm.Wday = ((time + 4) % 7) + 1;  // Sunday is day 1

  year = 0;
  days = 0;
  while((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= time) {
    year++;
  }
  tm.year = year;            // year is offset from 1970

  days -= LEAP_YEAR(year) ? 366 : 365;
  time -= days;              // now it is days in this year, starting at 0
  tm.Yday = time;

  for (month = 0; month < 12; month++) {
    if (1 == month) { // february
      if (LEAP_YEAR(year)) {
        month_length = 29;
      } else {
        month_length = 28;
      }
    } else {
      month_length = kDaysInMonth[month];
    }

    if (time >= month_length) {
      time -= month_length;
    } else {
      break;
    }
  }
  // strlcpy(tm.name_of_month, kMonthNames + (month *3), 4);
  tm.month = month + 1;      // jan is month 1
  tm.Wday = time + 1;         // day of month
  tm.valid = (time_input > START_VALID_TIME);  // 2016-01-01
}

uint32_t mTime::MakeTime(datetime_t &tm)
{
// assemble time elements into time_t
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
      seconds += SECS_PER_DAY * kDaysInMonth[i-1];  // monthDay array starts from 0
    }
  }
  seconds+= (tm.Wday - 1) * SECS_PER_DAY;
  seconds+= tm.hour * SECS_PER_HOUR;
  seconds+= tm.minute * SECS_PER_MIN;
  seconds+= tm.second;
  return seconds;
}


void mTime::RtcSecond(void)
{
  datetime_t tmpTime;
  // TIME_T tmpTime;
// Serial.printf("RtcSecond=%d\n\r",millis());

  Rtc.millis = millis();

  if (!Rtc.user_time_entry) {
    if (!pCONT_set->global_state.network_down) {
      uint8_t uptime_minute = (pCONT_set->uptime / 60) % 60;  // 0 .. 59
      // Serial.printf("uptime_minute=%d\n\r",uptime_minute);
      if ((Rtc.ntp_sync_minute > 59) && (uptime_minute > 2)) {
        Rtc.ntp_sync_minute = 1;                   // If sync prepare for a new cycle
      }
      uint8_t offset = (pCONT_set->uptime < 30) ? RtcTime.second : (((mSupportHardware::ESP_getChipId() & 0xF) * 3) + 3) ;  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
      
      // Serial.printf("uptime%d,offset=%d\n\r",pCONT_set->uptime,offset);
      
      if ( (((offset == RtcTime.second) && ( (RtcTime.year < 2016) ||                          // Never synced
                                            (Rtc.ntp_sync_minute == uptime_minute))) ||       // Re-sync every hour
                                              pCONT_set->ntp_force_sync ) ) {                             // Forced sync
        // Rtc.ntp_time = sntp_get_current_timestamp();



        if (Rtc.ntp_time > START_VALID_TIME) {  // Fix NTP bug in core 2.4.1/Sdeclination_of_sun 2.2.1 (returns Thu Jan 01 08:00:10 1970 after power on)
          pCONT_set->ntp_force_sync = false;
          Rtc.utc_time = Rtc.ntp_time;
          Rtc.last_sync = Rtc.ntp_time;
          Rtc.ntp_sync_minute = 60;  // Sync so block further requests
          if (Rtc.restart_time == 0) {
            Rtc.restart_time = Rtc.utc_time - pCONT_set->uptime;  // save first ntp time as restart time
          }
          BreakTime(Rtc.utc_time, tmpTime);
          RtcTime.year = tmpTime.year + 1970;
          Rtc.daylight_saving_time = RuleToTime(pCONT_set->Settings.tflag[1], RtcTime.year);
          Rtc.standard_time = RuleToTime(pCONT_set->Settings.tflag[0], RtcTime.year);

          // Do not use AddLog_P2 here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
          // PrepLog_P2(LOG_LEVEL_DEBUG, PSTR("NTP: " D_UTC_TIME " %s, " D_DST_TIME " %s, " D_STD_TIME " %s"),
          //   GetDateAndTimeCtr(DT_UTC).c_str(), GetDateAndTimeCtr(DT_DST).c_str(), GetDateAndTimeCtr(DT_STD).c_str());

          // if (Rtc.local_time < START_VALID_TIME) {  // 2016-01-01
          //   rules_flag.time_init = 1;
          // } else {
          //   rules_flag.time_set = 1;
          // }
        } else {
          Rtc.ntp_sync_minute++;  // Try again in next minute
        }
      }
    }
    if ((Rtc.utc_time > (2 * 60 * 60)) && (Rtc.last_sync < Rtc.utc_time - (2 * 60 * 60))) {  // Every two hours a warning
      // Do not use AddLog_P2 here (interrupt routine) if syslog or mqttlog is enabled. UDP/TCP will force exception 9
      // PrepLog_P2(LOG_LEVEL_DEBUG, PSTR("NTP: Not synced"));
      Rtc.last_sync = Rtc.utc_time;
    }
  }

  Rtc.utc_time++;  // Increment every second
  Rtc.local_time = Rtc.utc_time;

      // Serial.printf("Rtc.utc_time=%d\n\r",Rtc.utc_time);


  if (Rtc.local_time > START_VALID_TIME) {  // 2016-01-01
    int16_t timezone_minutes = pCONT_set->Settings.timezone_minutes;
    if (pCONT_set->Settings.timezone < 0) { timezone_minutes *= -1; }
    Rtc.time_timezone = (pCONT_set->Settings.timezone * SECS_PER_HOUR) + (timezone_minutes * SECS_PER_MIN);
      // AddLog_P(LOG_LEVEL_TEST,PSTR("pCONT_set->Settings.timezone=%d"),pCONT_set->Settings.timezone);
    if (99 == pCONT_set->Settings.timezone) {
      int32_t dstoffset = pCONT_set->Settings.toffset[1] * SECS_PER_MIN;
      int32_t stdoffset = pCONT_set->Settings.toffset[0] * SECS_PER_MIN;
      if (pCONT_set->Settings.tflag[1].hemis) {
        // Southern hemisphere
        if ((Rtc.utc_time >= (Rtc.standard_time - dstoffset)) && (Rtc.utc_time < (Rtc.daylight_saving_time - stdoffset))) {
          Rtc.time_timezone = stdoffset;  // Standard Time
        } else {
          Rtc.time_timezone = dstoffset;  // Daylight Saving Time
        }
      } else {
        // Northern hemisphere
        if ((Rtc.utc_time >= (Rtc.daylight_saving_time - stdoffset)) && (Rtc.utc_time < (Rtc.standard_time - dstoffset))) {
          Rtc.time_timezone = dstoffset;  // Daylight Saving Time
        } else {
          Rtc.time_timezone = stdoffset;  // Standard Time
        }
      }
    }
      // AddLog_P(LOG_LEVEL_TEST,PSTR("Rtc.time_timezone=%d"),Rtc.time_timezone);
    Rtc.local_time += Rtc.time_timezone;
    Rtc.time_timezone /= 60;
    // if (!pCONT_set->Settings.energy_kWhtotal_time) {
    //   pCONT_set->Settings.energy_kWhtotal_time = Rtc.local_time;
    // }
    // if (pCONT_set->Settings.bootcount_reset_time < START_VALID_TIME) {
    //   pCONT_set->Settings.bootcount_reset_time = Rtc.local_time;
    // }
  }

  BreakTime(Rtc.local_time, RtcTime);
  if (RtcTime.valid) {
    // Serial.printf("\"%02d:%02d:%02d\"\n\r",RtcTime.hour,RtcTime.minute,RtcTime.second);
    if (!Rtc.midnight) {
      Rtc.midnight = Rtc.local_time - (RtcTime.hour * 3600) - (RtcTime.minute * 60) - RtcTime.second;
    }
    if (!RtcTime.hour && !RtcTime.minute && !RtcTime.second) {
      Rtc.midnight = Rtc.local_time;
      Rtc.midnight_now = true;
    }
  }

  RtcTime.year += 1970;
}


void mTime::RtcSync(void) {
  // Rtc.time_synced = true;
  RtcSecond();
 AddLog_P(LOG_LEVEL_TEST, PSTR("RTC: Synced"));
}

void mTime::RtcSetTime(uint32_t epoch)
{
  if (epoch < START_VALID_TIME) {  // 2016-01-01
    Rtc.user_time_entry = false;
    pCONT_set->ntp_force_sync = true;
  } else {
    Rtc.user_time_entry = true;
    Rtc.utc_time = epoch -1;    // Will be corrected by RtcSecond
  }
}

// #endif


void mTime::RtcInit(void)
{

  Rtc.utc_time = 0;
  BreakTime(Rtc.utc_time, RtcTime);
  
  #ifdef ESP8266
  TickerRtc = new Ticker();

  TickerRtc->attach(1, 
    [this](void){
      this->RtcSecond();
    }
  );
  #endif // ESP8266


}



void mTime::WifiPollNtp() {
  static uint8_t ntp_sync_minute = 0;

    // AddLog_P(LOG_LEVEL_TEST, PSTR("gWifiPollNtp"));
  // if (TasmotaGlobal.global_state.network_down || Rtc.user_time_entry) { return; }
  if(pCONT_set->global_state.wifi_down){ 
    
    
    AddLog_P(LOG_LEVEL_TEST, PSTR(D_LOG_TIME "global_state.wifi_down"));
    
    return ; }
    
    // AddLog_P(LOG_LEVEL_TEST, PSTR("gWifiPollNtp here"));

  uint8_t uptime_minute = (pCONT_set->uptime / 60) % 60;  // 0 .. 59
  if ((ntp_sync_minute > 59) && (uptime_minute > 2)) {
    ntp_sync_minute = 1;                 // If sync prepare for a new cycle
  }
  // First try ASAP to sync. If fails try once every 60 seconds based on chip id
  uint8_t offset = (pCONT_set->uptime < 30) ? RtcTime.second : (((mSupportHardware::ESP_getChipId() & 0xF) * 3) + 3) ;
  if ( (((offset == RtcTime.second) && ( (RtcTime.year < 2016) ||                  // Never synced
                                         (ntp_sync_minute == uptime_minute))) ||   // Re-sync every hour
       pCONT_set->ntp_force_sync ) ) {                                          // Forced sync
 AddLog_P(LOG_LEVEL_TEST, PSTR("WifiPollNtp Sync Attempt"));

    pCONT_set->ntp_force_sync = false;
    uint32_t ntp_time = WifiGetNtp();
    AddLog_P(LOG_LEVEL_TEST, PSTR(DEBUG_INSERT_PAGE_BREAK "ntp_time=%d"),ntp_time);

    if (ntp_time > START_VALID_TIME) {
      pCONT_time->Rtc.utc_time = ntp_time;
      pCONT_time->Rtc.ntp_time = ntp_time; //me
      ntp_sync_minute = 60;             // Sync so block further requests
      pCONT_time->RtcSync();
    } else {
      ntp_sync_minute++;                // Try again in next minute
    }
  }
}

uint32_t mTime::WifiGetNtp(void) {
  static uint8_t ntp_server_id = 0;
  pCONT_time->Rtc.ntp_last_active = millis();

  IPAddress time_server_ip;

  char fallback_ntp_server[16];
  snprintf_P(fallback_ntp_server, sizeof(fallback_ntp_server), PSTR("%d.pool.ntp.org"), random(0,3));

  char* ntp_server;
  bool resolved_ip = false;
  for (uint32_t i = 0; i <= MAX_NTP_SERVERS; i++) {
    if (i < MAX_NTP_SERVERS) {
      ntp_server = NTP_ADDRESS;//SettingsText(SET_NTPSERVER1 + ntp_server_id);
    } else {
      ntp_server = fallback_ntp_server;
    }
    if (strlen(ntp_server)) {
      resolved_ip = (WiFi.hostByName(ntp_server, time_server_ip) == 1);
      if (255 == time_server_ip[0]) { resolved_ip = false; }
      yield();
      if (resolved_ip) { break; }
    }
    ntp_server_id++;
    if (ntp_server_id > 2) { ntp_server_id = 0; }
  }
  if (!resolved_ip) {
    AddLog_P(LOG_LEVEL_TEST, PSTR("NTP: No server found"));
    return 0;
  }

  AddLog_P(LOG_LEVEL_TEST, PSTR("NTP: Name %s, IP %s"), ntp_server, time_server_ip.toString().c_str());

  WiFiUDP udp;

  uint32_t attempts = 3;
  while (attempts > 0) {
    uint32_t port = random(1025, 65535);   // Create a random port for the UDP connection.
    if (udp.begin(port) != 0) {
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
    ntp_server_id++;
    if (ntp_server_id > 2) { ntp_server_id = 0; }   // Next server next time
    udp.stop();
    return 0;
  }
  udp.write(packet_buffer, NTP_PACKET_SIZE);
  udp.endPacket();

  uint32_t begin_wait = millis();
  // while (!TimeReached(begin_wait + 1000)) {         // Wait up to one second
  while (abs(millis()-begin_wait)<1000) {         // Wait up to one second
    uint32_t size        = udp.parsePacket();
    uint32_t remote_port = udp.remotePort();

    if ((size >= NTP_PACKET_SIZE) && (remote_port == 123)) {
      udp.read(packet_buffer, NTP_PACKET_SIZE);     // Read packet into the buffer
      udp.stop();

      if ((packet_buffer[0] & 0b11000000) == 0b11000000) {
        // Leap-Indicator: unknown (clock unsynchronized)
        // See: https://github.com/letscontrolit/ESPEasy/issues/2886#issuecomment-586656384
        AddLog_P(LOG_LEVEL_TEST, PSTR("NTP: IP %s unsynched"), time_server_ip.toString().c_str());
        return 0;
      }

      // convert four bytes starting at location 40 to a long integer
      // TX time is used here.
      uint32_t secs_since_1900 = (uint32_t)packet_buffer[40] << 24;
      secs_since_1900 |= (uint32_t)packet_buffer[41] << 16;
      secs_since_1900 |= (uint32_t)packet_buffer[42] << 8;
      secs_since_1900 |= (uint32_t)packet_buffer[43];
      if (0 == secs_since_1900) {                   // No time stamp received
        return 0;
      }
      return secs_since_1900 - 2208988800UL;
    }
    delay(10);
  }
  // Timeout.
  AddLog_P(LOG_LEVEL_DEBUG, PSTR("NTP: No reply"));
  udp.stop();
  return 0;
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
  uint32_t Day = now.Wday;     // 1..31
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
  return pCONT_sup->ModulusRangef(x, 0.0f, pi2);
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
  RA_Mean = pCONT_sup->ModulusRangef(RA_Mean, 0.0f, 24.0f);
  float dRA = pCONT_sup->ModulusRangef(RA_Mean - RA, -12.0f, 12.0f);
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

  float lat = pCONT_set->Settings.sensors.latitude / (1000000.0f / RAD); // geographische Breite
  float lon = ((float) pCONT_set->Settings.sensors.longitude)/1000000;
  
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
  rise = pCONT_sup->ModulusRangef(rise, 0.0f, 24.0f);        // force 0 <= x < 24.0
  int rise_hours = (int)rise;
  int rise_minutes = (int)(60.0f * fmodf(rise, 1.0f));

  float downfall = downfall_world_time + timezone;
  downfall = pCONT_sup->ModulusRangef(downfall, 0.0f, 24.0f);
  int downfall_hours = (int)downfall;
  int downfall_minutes = (int)(60.0f * fmodf(downfall, 1.0f));

  // Return transit times
  *hour_up = rise_hours;
  *minute_up = rise_minutes;
  *hour_down = downfall_hours;
  *minute_down = downfall_minutes;
}

// void mTime::ApplyTimerOffsets(Timer *duskdawn)
// {
//   uint8_t hour[2];
//   uint8_t minute[2];
//   Timer stored = (Timer)*duskdawn;

//   // replace hours, minutes by sunrise
//   DuskTillDawn(&hour[0], &minute[0], &hour[1], &minute[1]);
//   uint8_t mode = (duskdawn->mode -1) &1;
//   duskdawn->time = (hour[mode] *60) + minute[mode];

//   if (hour[mode]==255) {
//     // Permanent day/night sets the unreachable limit values
//     if ((Settings.latitude > 0) != (RtcTime.month>=4 && RtcTime.month<=9)) {
//       duskdawn->time=2046; // permanent night
//     } else {
//       duskdawn->time=2047; // permanent day
//     }
//     // So skip the offset/underflow/overflow/day-shift
//     return;
//   }

//   // apply offsets, check for over- and underflows
//   uint16_t timeBuffer;
//   if ((uint16_t)stored.time > 719) {
//     // negative offset, time after 12:00
//     timeBuffer = (uint16_t)stored.time - 720;
//     // check for underflow
//     if (timeBuffer > (uint16_t)duskdawn->time) {
//       timeBuffer = 1440 - (timeBuffer - (uint16_t)duskdawn->time);
//       duskdawn->days = duskdawn->days >> 1;
//       duskdawn->days |= (stored.days << 6);
//     } else {
//       timeBuffer = (uint16_t)duskdawn->time - timeBuffer;
//     }
//   } else {
//     // positive offset
//     timeBuffer = (uint16_t)duskdawn->time + (uint16_t)stored.time;
//     // check for overflow
//     if (timeBuffer >= 1440) {
//       timeBuffer -= 1440;
//       duskdawn->days = duskdawn->days << 1;
//       duskdawn->days |= (stored.days >> 6);
//     }
//   }
//   duskdawn->time = timeBuffer;
// }

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