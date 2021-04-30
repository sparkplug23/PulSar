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
#include "mHeating.h"

#ifdef USE_MODULE_CONTROLLER_HEATING2

void mHeating::init_program_temps(void){

  for(int device_id=0;device_id<HEATING_DEVICE_MAX;device_id++){
    program_temps[device_id].time_running.on=-1;
    program_temps[device_id].time_maintaining.limit = -1;
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

  program_temps[DEVICE_US_ID].temp.desired = 20;
  program_temps[DEVICE_DS_ID].temp.desired = 21;
  program_temps[DEVICE_IH_ID].temp.desired = 40;
  program_temps[DEVICE_WB_ID].temp.desired = 50;


  program_temps[DEVICE_US_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_US_ID].temp.mapped_defaults[1] = 10;
  program_temps[DEVICE_US_ID].temp.mapped_defaults[2] = 23;
  program_temps[DEVICE_US_ID].temp.mapped_defaults[3] = 25;
  
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[1] = 20;
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[2] = 23;
  program_temps[DEVICE_DS_ID].temp.mapped_defaults[3] = 25;
  
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[1] = 30;
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[2] = 40;
  program_temps[DEVICE_IH_ID].temp.mapped_defaults[3] = 50;
  
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[0] = 0;
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[1] = 30;
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[2] = 40;
  program_temps[DEVICE_WB_ID].temp.mapped_defaults[3] = 50;


}//end function


#ifdef USE_HEATING_TEMPS

// Change to switch cases later for easier reading
// Next step, accept temp controls for water methods
void mHeating::FunctionHandler_Programs_Temps(void){

  #ifdef ENABLE_LOG_LEVEL_DEBUG
  AddLog(LOG_LEVEL_DEBUG, PSTR("FunctionHandler_Programs_Temps"));
  #endif
  
  /**
   * @note if seconds_on is >0, then tick up
   * */
  for(int device_id=0;device_id<4;device_id++){
    if(program_temps[device_id].time_running.on>=0){
      program_temps[device_id].time_running.on++;
    }
  }

  /**
   * @note Check if desired temp > current temp and time limit has not been reached
   * */
  for(int device_id=0;device_id<HEATING_DEVICE_MAX;device_id++){

    float measured_temp=0;
    
    /**
     * @note Get measured temp
     * */
    switch(device_id){
      case DEVICE_DS_ID: measured_temp = pCONT_dht->sensor[DHT_DOWNSTAIRS_ID].instant.temperature; break;
      case DEVICE_US_ID: measured_temp = pCONT_dht->sensor[DHT_UPSTAIRS_ID].instant.temperature; break;
      case DEVICE_WB_ID: measured_temp = pCONT_msdb18->sensor[ID_DB18_WB].reading.val; break;
      case DEVICE_IH_ID: measured_temp = pCONT_msdb18->sensor[ID_DB18_IH].reading.val; break;
    }

    /**
     * @note Calculate errors
     * */
    program_temps[device_id].temp.current = measured_temp;
    program_temps[device_id].temp.error = measured_temp - program_temps[device_id].temp.desired;

    /**
     * @note Change
     * */
    switch(program_temps[device_id].schedule.mode_sch){
      case SCHEDULED_MANUAL_ON_ID:
        program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
        activeprograms[device_id].temps.state = ACTIVEP_ON;
        fRunTemp = true;
        break;
      // case SCHEDULED_ON_ID: //IE RUNNING
      //   program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
      //   activeprograms[device_id].temps.state = ACTIVEP_ON;
      //   fRunTemp = true;
      //   //if on, check time limits
      // break;
      // // Awaiting for time before turning on 
      // // NEEDS complete revamp, to allow advanced scheduling (15 minute increments of on and off)
      // // use binary?

      // case SCHEDULED_SET_ID:{

      //   //program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;

      //   activeprograms[device_id].temps.state = ACTIVEP_SCHEDULED;

      //   //uint8_t fTimeReached = pCONT_time->CheckBetween_Week_DateTimes(&program_temps[device_id].schedule.ontime,&program_temps[device_id].schedule.offtime);

      //   uint8_t fTimeReached = pCONT_time->CheckDateTimeWeekIsNow(&program_temps[device_id].schedule.ontime);

      //   program_temps[device_id].schedule.untilontime = pCONT_time->GetDifferenceInDateTimes(&pCONT_time->RtcTime,&program_temps[device_id].schedule.ontime);

      //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "CheckBetween_Week_DateTimes [%d]"),fTimeReached);

      //   // uint8_t hours, minutes, seconds;
      //   // pCONT_time->DateTimeWeek2HHMMSS(&program_temps[device_id].schedule.untilontime,&hours,&minutes,&seconds);
      //   //
      //   // pCONT->mso->MessagePrint("hours> "); pCONT->mso->MessagePrintln(hours);
      //   // pCONT->mso->MessagePrint("minutes> "); pCONT->mso->MessagePrintln(minutes);
      //   // pCONT->mso->MessagePrint("seconds> "); pCONT->mso->MessagePrintln(seconds);

      //   // if is reached AND NOT run yet
      //   if(fTimeReached&&(!program_temps[device_id].schedule.fHasRun)){
      //     program_temps[device_id].schedule.fHasRun = 1;
      //     program_temps[device_id].schedule.mode_sch = SCHEDULED_ON_ID;
      //     program_temps[device_id].time_running.on = 0;
      //   }

      //   // if(program_temps[device_id].schedule.fRunning){}

      //   // check times and update time until it will run (use this as seconds count check?)
      //   fRunTemp = false;
      // }
      // break;
      case SCHEDULED_OFF_ID: // off, no temp heating
        activeprograms[device_id].temps.state = ACTIVEP_OFF;
        fRunTemp = false;
      break;
    }


    // Running section of code
    if(fRunTemp){
 
     // program_temps[device_id].mqtt.fForceUpdate = 1;  //CAUSES ERROR
      //pCONT->mso->MessagePrint(program_temps[device_id].mqtt.fForceUpdate);

      // if timer is active
      if(program_temps[device_id].time_running.on >= 0){

        /**
         *  Time checks - If limit is hit, stop, or else continue heating on
         * */
        if(program_temps[device_id].time_running.on > program_temps[device_id].time_running.limit){   
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),program_temps[device_id].time_running.on,program_temps[device_id].time_running.limit);
          program_temps[device_id].time_running.on = -2; //deactviate
          program_temps[device_id].time_maintaining.limit = -1;
        }else
        { //stay on
          SetHeater(device_id,1);
          program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
          activeprograms[device_id].temps.state = ACTIVEP_ON;
        }

        /**
         * Temp checks
         * */
        // Exceeded desired temperature
        if(measured_temp >= program_temps[device_id].temp.desired){   
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Measured Temp [%f] > Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
          AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Exceeded Set Point"));
    
          // Check if temp should be maintained above desired point
          if((program_temps[device_id].time_maintaining.on>=0)&&(program_temps[device_id].time_maintaining.on < program_temps[device_id].time_maintaining.limit)){
            program_temps[device_id].status.mode = TEMP_MODE_MAINTAINING_ID;
            activeprograms[device_id].temps.state = ACTIVEP_ON;
          }
          // Turn off
          else{
            
            AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "desired_count > desired_max"));
    
            program_temps[device_id].time_running.on = -2; // -1 is just not active, -2 is a freeze out that lasts one hour unless overriden
            program_temps[device_id].time_maintaining.limit = -1;

            //record time it turned off ie record temp difference and how long we took to get there, I can use this later to estimate

            SetHeater(device_id,0);

            // Reset modes
            if(program_temps[device_id].schedule.mode_sch == SCHEDULED_ON_ID){
              program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
              program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
              activeprograms[device_id].temps.state = ACTIVEP_OFF;
            }else{ //manual
              program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
              program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
              activeprograms[device_id].temps.state = ACTIVEP_OFF;
            }

            // fForceHeatingTempsUpdate = true;

          }

      }
      // Continue with heating
      else{ 
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Measured Temp [%f] < Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
      }

      //?
      // else{
      //   program_temps[device_id].status.mode = TEMP_MODE_OFF;
      //   activeprograms[device_id].temps.state = ACTIVEP_OFF;
      // }// end if timer active

    } //if time running on

  }//if(fRunTemp){

  }//end for loop


  
  // SubTask_HeatingTemps_StatusMessage();

}



uint8_t mHeating::CheckIfROCExceedsNegativeLimit(uint8_t id){

  // switch(id){
  //   case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //   case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //   case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //   case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //   case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //   case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //   case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //   case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  // }

  // if(pCONT->mhs->watertemps.ptr->roc1m.val < -10){ // large change = water emptying therefore turn off
  //   return 1;
  // }
  return 0;

}// END function



// MESSAGES
// Heating for 0/120 minutes
// Maintaining for 0/120 minutes
// Off : Unset
// Waiting: Set for 07:00 (T-03:45)
void mHeating::SubTask_HeatingTemps_StatusMessage(){

  // int16_t timeon;
  // char tmp[10];
  // char namectr[2];
  // char time_ctr[50];
  // uint8_t ischanged = 0;
  // float valf,val;
  // int vali;
  // int i=0;

  // char tmpctr[50];  memset(tmpctr,0,sizeof(tmpctr));

  // for(int device_id=0;device_id<4;device_id++){

  //   // clear message
  //   memset(&program_temps[device_id].status.data,0,sizeof(program_temps[device_id].status.data));

  //   switch(program_temps[device_id].status.mode){
  //     default:
  //       memcpy(&program_temps[device_id].status.data.ctr[0],"Error:Unknown",sizeof("Error:Unknown")-1);
  //     break;
  //     case TEMP_MODE_OFF_ID:
  //       memcpy(&program_temps[device_id].status.data.ctr[0],"Off",sizeof("Off")-1); program_temps[device_id].status.data.len += sizeof("Off")-1;
  //     break;
  //     case TEMP_MODE_HEATING_ID: // Heating for 0/120 minutes
  //       memset(time_ctr,'\0',sizeof(time_ctr));
  //       sprintf(time_ctr, "Heating for %d/%d minutes",(program_temps[device_id].time_running.on<1?0:program_temps[device_id].time_running.on),(program_temps[device_id].time_running.limit));
  //       memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
  //       program_temps[device_id].status.data.len += strlen(time_ctr);
  //     break;
  //     case TEMP_MODE_MAINTAINING_ID:
  //       memset(time_ctr,'\0',sizeof(time_ctr));
  //       sprintf(time_ctr, "Maintaining for %d/%d minutes",(program_temps[device_id].time_maintaining.on<1?0:program_temps[device_id].time_maintaining.on),(program_temps[device_id].time_maintaining.limit));
  //       memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
  //       program_temps[device_id].status.data.len += strlen(time_ctr);
  //     break;
  //     case TEMP_MODE_SCHEDULED_ID: // Waiting: Set for 07:00 (T-03:45)
  //       memset(time_ctr,'\0',sizeof(time_ctr));
  //       sprintf(time_ctr, "Set for %02d:%02d (in %02d:%02d)",
  //       program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,
  //       program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,program_temps[device_id].schedule.untilontime.second);
  //       memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
  //       program_temps[device_id].status.data.len += strlen(time_ctr);
  //     break;
  //     case TEMP_MODE_SPLASH_RUN_TIME_ID:
  //       // Scheduled to run in xx hours and yy minutes (xx:xx:xx/yy:yy:yy)
  //       if(abs(millis()-program_temps[device_id].status.tSplashTime)<10000){//program_temps[device_id].status.msSplashtime){
  //         memset(time_ctr,'\0',sizeof(time_ctr));
  //         sprintf(time_ctr, "Running in %d hours and %d minutes (%02d:%02d:%02d/%02d:%02d:%02d)",//abs(millis()-program_temps[device_id].status.tSplashTime),
  //           program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,
  //           program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,program_temps[device_id].schedule.ontime.second,
  //           program_temps[device_id].schedule.offtime.hour,program_temps[device_id].schedule.offtime.minute,program_temps[device_id].schedule.offtime.second);
  //         memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
  //         program_temps[device_id].status.data.len += strlen(time_ctr);
  //        }else{
  //          program_temps[device_id].status.mode = program_temps[device_id].status.mode_next;
  //        }
  //     break;
  //   }//END switch
  // }//END for
}// END function

#ifdef USE_HEATING_PROFILE_ESTIMATION
    
// 6.25-25
// 6.55 -49.3
// 24.3 in 30 minutes
// 24.4/30 = deg/min = 0.8133
// 30/24.4 = minutes per degree
//float temp_step = 1.0/100.0; //degree range 0-1
void mHeating::init_HeatingProfiles(){

  for(int device_id=0;device_id<4;device_id++){

    switch(device_id){
      default:
      case DEVICE_DS_ID:
      case DEVICE_US_ID:{
        heating_profiles[device_id].temperature_max = 25;
        heating_profiles[device_id].temperature_min = 15;
        heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
        for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
          heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
          heating_profiles[device_id].duration_secs[index] = (60*30)/10; // seconds
        }
      }
      break;
      case DEVICE_IH_ID:{
        heating_profiles[device_id].temperature_max = 60;
        heating_profiles[device_id].temperature_min = 15;
        heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
        for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
          heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
          heating_profiles[device_id].duration_secs[index] = 214/2; // seconds
        }
      }
      break;
      case DEVICE_WB_ID:{
        heating_profiles[device_id].temperature_max = 60;
        heating_profiles[device_id].temperature_min = 15;
        heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
        for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
          heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
          heating_profiles[device_id].duration_secs[index] = 70;//140; // seconds by 1 degree then convert to new range
        }
      }
      break;
    }
  }
}

void mHeating::FunctionHandler_Heating_Profiles(){

 // if(abs(millis()-tSavedHeatingProfiles)>60000){tSavedHeatingProfiles = millis();

    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROFILES));
            
    //MQQTSendHeatingProfile_Raw_IfChanged();

    //GetHeatingProfilesTimeSeconds(DEVICE_IH,random(20,30)*1.12,40);

  //}

}

uint32_t mHeating::GetHeatingProfilesTimeMinutes(uint8_t device_id, float temp_now, float temp_target){
  uint32_t seconds = GetHeatingProfilesTimeSeconds(device_id, temp_now, temp_target);
  uint32_t minutes = seconds/60;
  return minutes;
}

uint32_t mHeating::GetHeatingProfilesTimeSeconds(uint8_t device_id, float temp_now, float temp_target){

uint32_t seconds=0;
uint8_t index_now=20, index_target=30;

/*

//find nerest temp to now (get index)
float difference[HEATINGPROFILE_RESOLUTION];

// temp_now = 50;
// for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
//   heating_profiles[device_id].temperature[index]=index;
// }

// Get difference
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  difference[index] = fabs(temp_now - heating_profiles[device_id].temperature[index]);
  //Serial.print(fabs(temp_now - heating_profiles[device_id].temperature[index])); Serial.print("_");
  //Serial.print(difference[index]); Serial.println("_");
}

//Get smallest difference ie closest
float lowest_difference_val,lowest_difference_temp_val;// = min()
uint8_t lowest_difference_index;

lowest_difference_val = difference[0];// record first value
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  if(difference[index]<lowest_difference_val){
    lowest_difference_val = difference[index];
    lowest_difference_temp_val = heating_profiles[device_id].temperature[index];
    lowest_difference_index = index;
    // pCONT->mso->MessagePrint("lowest_difference_val<difference[index]> ");
    // pCONT->mso->MessagePrint(lowest_difference_val);
    // pCONT->mso->MessagePrint("lowest_difference_temp_val> ");
    // pCONT->mso->MessagePrint(lowest_difference_temp_val);
    // pCONT->mso->MessagePrint(" lowest_difference_index> ");
    // pCONT->mso->MessagePrintln(lowest_difference_index);
  }
}



// pCONT->mso->MessagePrint("[HEAT] Searching for [");
// pCONT->mso->MessagePrint(temp_now);
// pCONT->mso->MessagePrint("]");
//
// pCONT->mso->MessagePrint(" found [");
// pCONT->mso->MessagePrint(heating_profiles[device_id].temperature[lowest_difference_index]);
// pCONT->mso->MessagePrint("] at index [");
// pCONT->mso->MessagePrint(lowest_difference_index);
// pCONT->mso->MessagePrintln("]");

index_now = lowest_difference_index;

//find nereast temp to target (get index)

// Get difference
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  difference[index] = fabs(temp_target - heating_profiles[device_id].temperature[index]);
  //Serial.print(fabs(temp_now - heating_profiles[device_id].temperature[index])); Serial.print("_");
  //Serial.print(difference[index]); Serial.println("_");
}

//Get smallest difference ie closest
// float lowest_difference_val,lowest_difference_temp_val;// = min()
// uint8_t lowest_difference_index;

lowest_difference_val = difference[0];// record first value
for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
  if(difference[index]<lowest_difference_val){
    lowest_difference_val = difference[index];
    lowest_difference_temp_val = heating_profiles[device_id].temperature[index];
    lowest_difference_index = index;
    // pCONT->mso->MessagePrint("lowest_difference_val<difference[index]> ");
    // pCONT->mso->MessagePrint(lowest_difference_val);
    // pCONT->mso->MessagePrint("lowest_difference_temp_val> ");
    // pCONT->mso->MessagePrint(lowest_difference_temp_val);
    // pCONT->mso->MessagePrint(" lowest_difference_index> ");
    // pCONT->mso->MessagePrintln(lowest_difference_index);
  }
}



// pCONT->mso->MessagePrint("[HEAT] Searching for [");
// pCONT->mso->MessagePrint(temp_target);
// pCONT->mso->MessagePrint("]");
//
// pCONT->mso->MessagePrint(" found [");
// pCONT->mso->MessagePrint(heating_profiles[device_id].temperature[lowest_difference_index]);
// pCONT->mso->MessagePrint("] at index [");
// pCONT->mso->MessagePrint(lowest_difference_index);
// pCONT->mso->MessagePrintln("]");

index_target = lowest_difference_index;

//sum seconds
for(int ii=index_now;index_now<index_target;index_now++){
  seconds += heating_profiles[device_id].duration_secs[ii];
  //Serial.print("secs> ");
  //Serial.println(seconds);
}

  // Possibly adding heating start time too? so curves are assuming heating is on

  datetime_t untiltime = pCONT_time->GetDifferenceInDateTimes(&pCONT_time->RtcTime,&heating_device_relays[device_id].ontime);
  uint32_t secondson = pCONT_time->GetSecondsOfDayFromDateTime(&untiltime);

  int cold_start_seconds = 0;
  float sensor_temp = 0;
  // Add time on if system is only starting
  if(secondson<(60*10)){




    switch(device_id){   /// WRONG INDEXING -- NEED TO USE FUNCTION THAT RETURNS TEMP CORRECTLY FOR ID, NOT BASED ON INDEX, OR RETURNS POINTER TO THE TEMP INFO CONTAINED WITHIN IT
      case ID_DB18_DS: sensor_temp = pCONT_msdb18->sensor[ID_DB18_DS].reading.val; break;
      case ID_DB18_US: sensor_temp = pCONT_msdb18->sensor[ID_DB18_US].reading.val; break;
      case ID_DB18_WB: sensor_temp = pCONT_msdb18->sensor[ID_DB18_WB].reading.val; break;
      case ID_DB18_IH: sensor_temp = pCONT_msdb18->sensor[ID_DB18_IH].reading.val; break;
    }

    // Colder = more time added
    cold_start_seconds = map(sensor_temp,15,50,(60*10),1);
    cold_start_seconds = constrain(cold_start_seconds,1,(60*10));

    seconds += cold_start_seconds;// 10 minutes needs to go by relay on timer (needs added!)
  }
*/
return seconds;
}








void mHeating::ConstructJSON_HeatingProfile_Raw(uint8_t device_id){

  // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
  // JsonObject obj = doc.to<JsonObject>();

  // obj[D_JSON_SIZE] = HEATINGPROFILE_RESOLUTION;
  
  // JsonObject temp = obj.createNestedObject(D_JSON_TEMP);
  //   temp[D_JSON_MAX] = mSupport::roundfloat(heating_profiles[device_id].temperature_max,1);
  //   temp[D_JSON_MIN] = mSupport::roundfloat(heating_profiles[device_id].temperature_min,1);;
  //   temp[D_JSON_STEP] = mSupport::roundfloat(heating_profiles[device_id].temperature_step,1);;
  //   JsonArray temparr = temp.createNestedArray(D_JSON_VALUE);
  //   for(int i=0;i<HEATINGPROFILE_RESOLUTION;i++){
  //     temparr.add(mSupport::roundfloat(heating_profiles[device_id].temperature[i],1));
  //   }

  // JsonArray dursarr = obj.createNestedArray(D_JSON_DURATION);
  // for(int i=0;i<HEATINGPROFILE_RESOLUTION;i++){
  //   dursarr.add(mSupport::roundfloat(heating_profiles[device_id].duration_secs[i],1));
  // }

  // data_buffer.payload.len = measureJson(obj)+1;
  // serializeJson(doc,data_buffer.payload.ctr);

  // return 0;

}
#endif

#endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
    
const char* mHeating::GetTempModeByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen){
  // if(buffer == nullptr){ return 0;}
  // switch(program_temps[device].status.mode){
  //   default:               
    snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH); 
  //   b
  //   reak;
  //   case TEMP_MODE_OFF_ID: snprintf_P(buffer, buflen, PM_TEMP_MODE_OFF_CTR); break;
  // }
  return buffer;
  //   (program_temps[device].status.mode == TEMP_MODE_HEATING ? "TEMP_MODE_HEATING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_MAINTAINING ? "TEMP_MODE_MAINTAINING\0" :
  //   (program_temps[device].status.mode == TEMP_MODE_SCHEDULED ? "TEMP_MODE_SCHEDULED\0" :
  //   D_UNKNOWN))));
}

const char* mHeating::GetTempActiveProgramByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen){
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





//Update: when times changed (set forced), or 60s elapsed
//new method, diving up Constructs if they are too big, return if all data was sent
uint8_t mHeating::ConstructJSON_ProgramTemps(uint8_t json_level){ //MQQTSendTimers
// return false;
  char buffer[50];
  JsonBuilderI->Start();
  // for(
    int device=2;
    // device<4;device++){

    // if((program_temps[device].mqtt.fForceUpdate)||(json_level==JSON_LEVEL_IFCHANGED)){program_temps[device].mqtt.fForceUpdate = false;

      JsonBuilderI->Level_Start_P(GetDeviceNamebyIDCtr(device, buffer, sizeof(buffer))); 
        JsonBuilderI->Level_Start_P(D_JSON_TEMPERATURE);   
          JsonBuilderI->Add(D_JSON_CURRENT, program_temps[device].temp.current);
          JsonBuilderI->Add(D_JSON_DESIRED, program_temps[device].temp.desired);
          JsonBuilderI->Add(D_JSON_ERROR, program_temps[device].temp.error);
        JsonBuilderI->Level_End();
        JsonBuilderI->Level_Start_P(D_JSON_STATUS);   
          JsonBuilderI->Add(D_JSON_MODE, GetTempModeByDeviceIDCtr(device, buffer, sizeof(buffer)));
          // JsonBuilderI->Add(D_JSON_DATA, program_temps[device].status.data.ctr);
        JsonBuilderI->Level_End();
        JsonBuilderI->Level_Start_P(D_JSON_TIME_RUNNING); 
          JsonBuilderI->Add(D_JSON_ONTIME, program_temps[device].time_running.on);
          JsonBuilderI->Add(D_JSON_LIMIT, program_temps[device].time_running.limit);        
        JsonBuilderI->Level_End();  
        JsonBuilderI->Level_Start_P(D_JSON_TIME_MAINTAINING); 
          JsonBuilderI->Add(D_JSON_ONTIME, program_temps[device].time_maintaining.on);
          JsonBuilderI->Add(D_JSON_LIMIT, program_temps[device].time_maintaining.limit);        
        JsonBuilderI->Level_End();  

    //       #ifdef USE_HEATING_TEMPS
    //       // JsonObject time_to_heatobj = deviceobj.createNestedObject(D_JSON_TIME_TO_HEAT);
    //       //   time_to_heatobj[D_JSON_SECONDS] = GetHeatingProfilesTimeSeconds(device,program_temps[device].temp.current,program_temps[device].temp.desired);
    //       //   time_to_heatobj[D_JSON_MINUTES] = GetHeatingProfilesTimeMinutes(device,program_temps[device].temp.current,program_temps[device].temp.desired);
    //     #endif

        JsonBuilderI->Level_Start_P(D_JSON_SCHEDULE); 
          JsonBuilderI->Add(D_JSON_MODE, GetScheduleNameCtrbyID(program_temps[device].schedule.mode_sch, buffer, sizeof(buffer)));
          JsonBuilderI->Add(D_JSON_ISRUNNING, program_temps[device].schedule.fRunning);        
        JsonBuilderI->Level_End();  

    //       // char time_ctr[20]; memset(time_ctr,'\0',sizeof(time_ctr));
    //       // sprintf(time_ctr, "%02d:%02d:%02d",(int)program_temps[device].schedule.ontime.hour,(int)program_temps[device].schedule.ontime.minute,(int)program_temps[device].schedule.ontime.second);
    //       // scheduleobj[D_JSON_ONTIME] = time_ctr;//pCONT_time->getFormattedTime();
          
    //       // char time_ctr2[20];
    //       // memset(time_ctr2,'\0',sizeof(time_ctr2));
    //       // sprintf(time_ctr2, "%02d:%02d:%02d",(int)program_temps[device].schedule.offtime.hour,(int)program_temps[device].schedule.offtime.minute,(int)program_temps[device].schedule.offtime.second);
    //       // scheduleobj[D_JSON_OFFTIME] = time_ctr2;//pCONT_time->getFormattedTime();

    //     //data_buffer.payload.json_pairs++;
    JsonBuilderI->Level_End();
    // }
  // }
  return JsonBuilderI->End();

}





#endif
