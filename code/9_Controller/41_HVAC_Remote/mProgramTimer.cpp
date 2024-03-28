#include "9_Controller/41_HVAC_Remote/mHVAC.h"

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

void ProgramTimer::EverySecond(void)
{

  if(time_on_seconds_decounter == 0)
  {
      time_on_seconds_decounter = -1; //to disable it
      ALOG_DBM( PSTR("time_on_seconds_decounter=-1 DISABLED") );
      ischanged=true;
  }else 
  if(time_on_seconds_decounter > 0){
      time_on_seconds_decounter--;
      ischanged=true;
      ALOG_DBM( PSTR("time_on_seconds_decounter=%d Ticking..."), time_on_seconds_decounter );    
  }
  
} // end function



#endif