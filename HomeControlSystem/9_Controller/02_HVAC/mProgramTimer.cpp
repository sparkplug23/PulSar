#include "9_Controller/02_HVAC/mProgramTimer.h"


#ifdef USE_MODULE_CONTROLLER_HVAC

void ProgramTimer::EverySecond(void)
{

  // AddLog(LOG_LEVEL_DEBUG, PSTR("ProgramTimer::EverySecond"));

  // char buffer[50];

  if(time_on_seconds_decounter == 0){
      time_on_seconds_decounter = -1; //to disable it
      AddLog(LOG_LEVEL_DEBUG, PSTR("time_on_seconds_decounter=-1 DISABLED"));
      // time_minutes_on_start = time_on_seconds_decounter;
      // SetHeater(device_id, RELAY_STATE_OFF); //turn off
      ischanged=true;
      // state_tmp = ACTIVEP_OFF; // #1 error needs to be done here
      // activeprograms[device_id].timers.state = 0;  
  }else 
  if(time_on_seconds_decounter > 0){
      time_on_seconds_decounter--;
      ischanged=true;
      AddLog(LOG_LEVEL_DEBUG, PSTR("time_on_seconds_decounter=%d Ticking..."),time_on_seconds_decounter);
      // SetHeater(device_id, RELAY_STATE_ON); // only needs called once, that or add ifnoton inside this function turn on
      // ischanged=false;
      // state_tmp = ACTIVEP_ON;
      // activeprograms[device_id].timers.state = 1;        
  }else
  { // <=1 not active
      // ischanged=false;
      // activeprograms[device_id].timers.state = 0;  
  }

  // AddLog(LOG_LEVEL_ERROR,PSTR("time_minutes_on=%d"),time_minutes_on);
  // Serial.printf("time_on_seconds_decounter=%d\n\r",time_on_seconds_decounter);

  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING "activeprograms[%d].timers.state = %d"),device_id,
  //     activeprograms[device_id].timers.state);

} // end function



#endif