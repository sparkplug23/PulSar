/*
  mHeating_Commands.cpp - Home Heating Controller

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "mHVAC.h"

#ifdef USE_MODULE_CONTROLLER_HVAC

#ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

void mHVAC::init_program_temps(void){

  for(int device_id=0;device_id<HEATING_DEVICE_MAX;device_id++){
    // program_temps[device_id].time_running.on=-1;
    // program_temps[device_id].time_maintaining.limit = -1;
  }

  // test fill
  // memset(&program_temps[DEVICE_US_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_US_ID].status.data.ctr));
  // program_temps[DEVICE_US_ID].status.data.len = 0;
  // memset(&program_temps[DEVICE_DS_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_DS_ID].status.data.ctr));
  // program_temps[DEVICE_DS_ID].status.data.len = 0;
  // memset(&program_temps[DEVICE_IH_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_IH_ID].status.data.ctr));
  // program_temps[DEVICE_IH_ID].status.data.len = 0;
  // memset(&program_temps[DEVICE_WB_ID].status.data.ctr,0,sizeof(program_temps[DEVICE_WB_ID].status.data.ctr));
  // program_temps[DEVICE_WB_ID].status.data.len = 0;

  // program_temps[DEVICE_US_ID].temp.desired = 20;
  // program_temps[DEVICE_DS_ID].temp.desired = 21;
  // program_temps[DEVICE_IH_ID].temp.desired = 40;
  // program_temps[DEVICE_WB_ID].temp.desired = 50;


  // program_temps[DEVICE_US_ID].temp.mapped_defaults[0] = 0;
  // program_temps[DEVICE_US_ID].temp.mapped_defaults[1] = 10;
  // program_temps[DEVICE_US_ID].temp.mapped_defaults[2] = 23;
  // program_temps[DEVICE_US_ID].temp.mapped_defaults[3] = 25;
  
  // program_temps[DEVICE_DS_ID].temp.mapped_defaults[0] = 0;
  // program_temps[DEVICE_DS_ID].temp.mapped_defaults[1] = 20;
  // program_temps[DEVICE_DS_ID].temp.mapped_defaults[2] = 23;
  // program_temps[DEVICE_DS_ID].temp.mapped_defaults[3] = 25;
  
  // program_temps[DEVICE_IH_ID].temp.mapped_defaults[0] = 0;
  // program_temps[DEVICE_IH_ID].temp.mapped_defaults[1] = 30;
  // program_temps[DEVICE_IH_ID].temp.mapped_defaults[2] = 40;
  // program_temps[DEVICE_IH_ID].temp.mapped_defaults[3] = 50;
  
  // program_temps[DEVICE_WB_ID].temp.mapped_defaults[0] = 0;
  // program_temps[DEVICE_WB_ID].temp.mapped_defaults[1] = 30;
  // program_temps[DEVICE_WB_ID].temp.mapped_defaults[2] = 40;
  // program_temps[DEVICE_WB_ID].temp.mapped_defaults[3] = 50;


}//end function


/**
 * @brief 
 * Change to switch cases later for easier reading
 * Next step, accept temp controls for water methods
 */
void mHVAC::FunctionHandler_Programs_Temps(void){

  for(uint8_t zone_id=0; zone_id<settings.active_zones; zone_id++)
  {

    // Store current temperature
    zone[zone_id].program_temp_method->SetCurrentTemperature(zone[zone_id].sensor.temperature);
    
    // Servicing program temps every second
    zone[zone_id].program_temp_method->EverySecond();

    //  React if timer has started or ended
    if(zone[zone_id].program_temp_method->IsChangedThenReset())
    {
      ALOG_DBM( PSTR("program_temp_method IsChangedThenReset %d"), zone_id);
      if(zone[zone_id].program_temp_method->OutputDesiredState())
      {
        SetZoneActive(zone_id, 1); // This can be changed to "FUNC_SET_POWER" for internal relay driver control
      }
      else
      {
        SetZoneActive(zone_id, 0);
      }
    }

  }
  
  // SubTask_HeatingTemps_StatusMessage();

}




// MESSAGES
// Heating for 0/120 minutes
// Maintaining for 0/120 minutes
// Off : Unset
// Waiting: Set for 07:00 (T-03:45)
// void mHVAC::SubTask_HeatingTemps_StatusMessage(){

//   // int16_t timeon;
//   // char tmp[10];
//   // char namectr[2];
//   // char time_ctr[50];
//   // uint8_t ischanged = 0;
//   // float valf,val;
//   // int vali;
//   // int i=0;

//   // char tmpctr[50];  memset(tmpctr,0,sizeof(tmpctr));

//   // for(int device_id=0;device_id<4;device_id++){

//   //   // clear message
//   //   memset(&program_temps[device_id].status.data,0,sizeof(program_temps[device_id].status.data));

//   //   switch(program_temps[device_id].status.mode){
//   //     default:
//   //       memcpy(&program_temps[device_id].status.data.ctr[0],"Error:Unknown",sizeof("Error:Unknown")-1);
//   //     break;
//   //     case TEMP_MODE_OFF_ID:
//   //       memcpy(&program_temps[device_id].status.data.ctr[0],"Off",sizeof("Off")-1); program_temps[device_id].status.data.len += sizeof("Off")-1;
//   //     break;
//   //     case TEMP_MODE_HVAC_ID: // Heating for 0/120 minutes
//   //       memset(time_ctr,'\0',sizeof(time_ctr));
//   //       sprintf(time_ctr, "Heating for %d/%d minutes",(program_temps[device_id].time_running.on<1?0:program_temps[device_id].time_running.on),(program_temps[device_id].time_running.limit));
//   //       memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//   //       program_temps[device_id].status.data.len += strlen(time_ctr);
//   //     break;
//   //     case TEMP_MODE_MAINTAINING_ID:
//   //       memset(time_ctr,'\0',sizeof(time_ctr));
//   //       sprintf(time_ctr, "Maintaining for %d/%d minutes",(program_temps[device_id].time_maintaining.on<1?0:program_temps[device_id].time_maintaining.on),(program_temps[device_id].time_maintaining.limit));
//   //       memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//   //       program_temps[device_id].status.data.len += strlen(time_ctr);
//   //     break;
//   //     case TEMP_MODE_SCHEDULED_ID: // Waiting: Set for 07:00 (T-03:45)
//   //       memset(time_ctr,'\0',sizeof(time_ctr));
//   //       sprintf(time_ctr, "Set for %02d:%02d (in %02d:%02d)",
//   //       program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,
//   //       program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,program_temps[device_id].schedule.untilontime.second);
//   //       memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//   //       program_temps[device_id].status.data.len += strlen(time_ctr);
//   //     break;
//   //     case TEMP_MODE_SPLASH_RUN_TIME_ID:
//   //       // Scheduled to run in xx hours and yy minutes (xx:xx:xx/yy:yy:yy)
//   //       if(abs(millis()-program_temps[device_id].status.tSplashTime)<10000){//program_temps[device_id].status.msSplashtime){
//   //         memset(time_ctr,'\0',sizeof(time_ctr));
//   //         sprintf(time_ctr, "Running in %d hours and %d minutes (%02d:%02d:%02d/%02d:%02d:%02d)",//abs(millis()-program_temps[device_id].status.tSplashTime),
//   //           program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,
//   //           program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,program_temps[device_id].schedule.ontime.second,
//   //           program_temps[device_id].schedule.offtime.hour,program_temps[device_id].schedule.offtime.minute,program_temps[device_id].schedule.offtime.second);
//   //         memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//   //         program_temps[device_id].status.data.len += strlen(time_ctr);
//   //        }else{
//   //          program_temps[device_id].status.mode = program_temps[device_id].status.mode_next;
//   //        }
//   //     break;
//   //   }//END switch
//   // }//END for
// }// END function

    
// const char* mHVAC::GetTempModeByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen){
//   // if(buffer == nullptr){ return 0;}
//   // switch(program_temps[device].status.mode){
//   //   default:               
//     snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH); 
//   //   b
//   //   reak;
//   //   case TEMP_MODE_OFF_ID: snprintf_P(buffer, buflen, PM_TEMP_MODE_OFF_CTR); break;
//   // }
//   return buffer;
//   //   (program_temps[device].status.mode == TEMP_MODE_HEATING ? "TEMP_MODE_HEATING\0" :
//   //   (program_temps[device].status.mode == TEMP_MODE_MAINTAINING ? "TEMP_MODE_MAINTAINING\0" :
//   //   (program_temps[device].status.mode == TEMP_MODE_SCHEDULED ? "TEMP_MODE_SCHEDULED\0" :
//   //   D_UNKNOWN))));
// }

const char* mHVAC::GetTempActiveProgramByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen){
  // if(buffer == nullptr){ return 0;}
  // return 0;
  // return 
  //   (program_temps[device].status.mode == TEMP_MODE_OFF ? "TEMP_MODE_OFF\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_HEATING ? "TEMP_MODE_HEATING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_MAINTAINING ? "TEMP_MODE_MAINTAINING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_SCHEDULED ? "TEMP_MODE_SCHEDULED\0" :
  //   D_UNKNOWN))));
  
    snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH);
    return buffer;
}



#endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

#endif
