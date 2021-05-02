#include "9_Controller/HVAC/mProgramTemperatue.h"


#ifdef USE_MODULE_CONTROLLER_HVAC

void ProgramTemperature::EverySecond(void)
{

  #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR("ProgramTemperature::EverySecond"));
  #endif
  
  // update timer
  if(time_on_seconds_decounter == 0){
    time_on_seconds_decounter = -1; //to disable it
    AddLog(LOG_LEVEL_DEBUG, PSTR("temp decounter=-1 DISABLED"));
    ischanged=true;
  }else 
  if(time_on_seconds_decounter > 0){
    time_on_seconds_decounter--;
    ischanged=true;
    AddLog(LOG_LEVEL_DEBUG, PSTR("temp decounter=%d Ticking..."),time_on_seconds_decounter); 
  }else
  { // <=1 not active
      // ischanged=false;
  }

  // If active
  if(time_on_seconds_decounter)
  {
    // 3 options
    /**
     * heat
     * heat maintain
     * cool
     * cool maintain
     * 
     * maintain exceeded, standby
     * dont allow cycling between modes, its either hear or cool for X minutes?
     * 
     * */

    // If negative error, we must heat
    if(GetErrorTemperature()<0)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("We must heat"));
    }

    if(GetErrorTemperature()>0)
    {
      AddLog(LOG_LEVEL_INFO, PSTR("We must cool"));
    }



    /**
     * @note Change
     * */
    // switch(program_temps[device_id].schedule.mode_sch){
    //   case SCHEDULED_MANUAL_ON_ID:
    //     program_temps[device_id].status.mode = TEMP_MODE_HVAC_ID;
    //     activeprograms[device_id].temps.state = ACTIVEP_ON;
    //     fRunTemp = true;
    //     break;
    //   // case SCHEDULED_ON_ID: //IE RUNNING
    //   //   program_temps[device_id].status.mode = TEMP_MODE_HVAC_ID;
    //   //   activeprograms[device_id].temps.state = ACTIVEP_ON;
    //   //   fRunTemp = true;
    //   //   //if on, check time limits
    //   // break;
    //   // // Awaiting for time before turning on 
    //   // // NEEDS complete revamp, to allow advanced scheduling (15 minute increments of on and off)
    //   // // use binary?

    //   // case SCHEDULED_SET_ID:{

    //   //   //program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;

    //   //   activeprograms[device_id].temps.state = ACTIVEP_SCHEDULED;

    //   //   //uint8_t fTimeReached = pCONT_time->CheckBetween_Week_DateTimes(&program_temps[device_id].schedule.ontime,&program_temps[device_id].schedule.offtime);

    //   //   uint8_t fTimeReached = pCONT_time->CheckDateTimeWeekIsNow(&program_temps[device_id].schedule.ontime);

    //   //   program_temps[device_id].schedule.untilontime = pCONT_time->GetDifferenceInDateTimes(&pCONT_time->RtcTime,&program_temps[device_id].schedule.ontime);

    //   //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "CheckBetween_Week_DateTimes [%d]"),fTimeReached);

    //   //   // uint8_t hours, minutes, seconds;
    //   //   // pCONT_time->DateTimeWeek2HHMMSS(&program_temps[device_id].schedule.untilontime,&hours,&minutes,&seconds);
    //   //   // if is reached AND NOT run yet
    //   //   if(fTimeReached&&(!program_temps[device_id].schedule.fHasRun)){
    //   //     program_temps[device_id].schedule.fHasRun = 1;
    //   //     program_temps[device_id].schedule.mode_sch = SCHEDULED_ON_ID;
    //   //     program_temps[device_id].time_running.on = 0;
    //   //   }

    //   //   // if(program_temps[device_id].schedule.fRunning){}

    //   //   // check times and update time until it will run (use this as seconds count check?)
    //   //   fRunTemp = false;
    //   // }
    //   // break;
    //   case SCHEDULED_OFF_ID: // off, no temp heating
    //     activeprograms[device_id].temps.state = ACTIVEP_OFF;
    //     fRunTemp = false;
    //   break;
    // }
  }

  // Running section of code
  // if(fRunTemp){

  //  // program_temps[device_id].mqtt.fForceUpdate = 1;  //CAUSES ERROR
  //   //pCONT->mso->MessagePrint(program_temps[device_id].mqtt.fForceUpdate);

  //   // if timer is active
  //   if(program_temps[device_id].time_running.on >= 0){

  //     /**
  //      *  Time checks - If limit is hit, stop, or else continue heating on
  //      * */
  //     if(program_temps[device_id].time_running.on > program_temps[device_id].time_running.limit){   
  //       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),program_temps[device_id].time_running.on,program_temps[device_id].time_running.limit);
  //       program_temps[device_id].time_running.on = -2; //deactviate
  //       program_temps[device_id].time_maintaining.limit = -1;
  //     }else
  //     { //stay on
  //       SetHeater(device_id,1);
  //       program_temps[device_id].status.mode = TEMP_MODE_HVAC_ID;
  //       activeprograms[device_id].temps.state = ACTIVEP_ON;
  //     }

  //     /**
  //      * Temp checks
  //      * */
  //     // Exceeded desired temperature
  //     if(measured_temp >= program_temps[device_id].temp.desired){   
  //       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured Temp [%f] > Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
  //       AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Exceeded Set Point"));
  
  //       // Check if temp should be maintained above desired point
  //       if((program_temps[device_id].time_maintaining.on>=0)&&(program_temps[device_id].time_maintaining.on < program_temps[device_id].time_maintaining.limit)){
  //         program_temps[device_id].status.mode = TEMP_MODE_MAINTAINING_ID;
  //         activeprograms[device_id].temps.state = ACTIVEP_ON;
  //       }
  //       // Turn off
  //       else{
          
  //         AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "desired_count > desired_max"));
  
  //         program_temps[device_id].time_running.on = -2; // -1 is just not active, -2 is a freeze out that lasts one hour unless overriden
  //         program_temps[device_id].time_maintaining.limit = -1;

  //         //record time it turned off ie record temp difference and how long we took to get there, I can use this later to estimate

  //         SetHeater(device_id,0);

  //         // Reset modes
  //         if(program_temps[device_id].schedule.mode_sch == SCHEDULED_ON_ID){
  //           program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
  //           program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
  //           activeprograms[device_id].temps.state = ACTIVEP_OFF;
  //         }else{ //manual
  //           program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
  //           program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
  //           activeprograms[device_id].temps.state = ACTIVEP_OFF;
  //         }

  //         // fForceHeatingTempsUpdate = true;

  //       }

  //   }
  //   // Continue with heating
  //   else{ 
  //     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured Temp [%f] < Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
  //   }

  //   //?
  //   // else{
  //   //   program_temps[device_id].status.mode = TEMP_MODE_OFF;
  //   //   activeprograms[device_id].temps.state = ACTIVEP_OFF;
  //   // }// end if timer active

  // } //if time running on

//   }//if(fRunTemp){

//   }//end for loop



}


#endif