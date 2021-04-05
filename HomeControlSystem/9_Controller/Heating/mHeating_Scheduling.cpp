#include "mHeating.h"

/***
 * Scheduling: 
 *  - Enable schedules to heat by time or temp as needed
 *  - Timed eg heat for 2 hours in this window
 *  - Temp eg heat to 40 for THIS time, so it heats before hand
 * */


#ifdef USE_MODULE_CONTROLLER_HEATING

// void mHeating::pre_init(void){

//   //init_success = true; // Begins true, anything bad sets to false

// }

void mHeating::init_program_scheduling(void){

  // datetime_t dummy_t;

  // dummy_t.Mday = 6; // set to today with __DATE__ later
  // dummy_t.month = mTime::MONTH_AUGUST;
  // dummy_t.year = 2019;
  // dummy_t.hour = 23;
  // dummy_t.minute = 30;
  // dummy_t.second = 0;

#ifdef USE_SCHEDULED_HEATING

  for(int device_id=0;device_id<4;device_id++){
    // program_temps[device_id].schedule.ontime = &dummy_t;
    // program_temps[device_id].schedule.offtime = &dummy_t;
    // program_temps[device_id].schedule.untilontime = &dummy_t;
  }
  #endif

}//end function



// void mHeating::init_relay_driver_parameters(){
  

// }

// void mHeating::init_db18_sensor_parameters(){

//   // Measure and report every second
//   pCONT_msdb18->settings.rate_measure_ms = 1000;
  
//   //Reduce sensor lib from reporting to minimum
//   pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
//   pCONT_msdb18->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
//   pCONT_msdb18->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

// }



// void mHeating::init_dht22_sensor_parameters(){

//   // pCONT_set->AddDeviceName(D_DHT_NAME_DOWNSTAIRS, D_MODULE_SENSORS_DHT_ID,   ID_DHT_DS);
//   // pCONT_set->AddDeviceName(D_DHT_NAME_UPSTAIRS, D_MODULE_SENSORS_DHT_ID,     ID_DHT_US);

//   // Measure and report every second
//   pCONT_msdb18->settings.rate_measure_ms = 1000;

//   //Reduce sensor lib from reporting to minimum
//   pCONT->msdht->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
//   pCONT->msdht->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
//   pCONT->msdht->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

// }

// /*******************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ********* RELAYS AND FAILSAFES ***************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************/


// void mHeating::SetHeatingRelay(uint8_t device_id, uint8_t state){
//   pCONT_mry->CommandSet_Relay_Power(state,device_id); //index offset starts at 1
// }

// uint8_t mHeating::GetHeatingRelay(uint8_t device_id){
//   DEBUG_LINE;
//   return pCONT_mry->CommandGet_Relay_Power(device_id);//index offset starts at 1
// }

// uint8_t mHeating::GetAnyHeatingRelay(){
//   uint8_t oncount = 0;
//   for(uint8_t i = 0;i < 4;i++){
//     oncount += GetHeatingRelay(i);
//   }
//   return oncount;
// }


// //temps at max/above threshold for extended time (count++ if temp>threshold, reset when lower)
// // check once a minute, if relay on OR temp above threshold, add time to counter
// void mHeating::FunctionHandler_FailSafe(void){

//   // if(mTime::TimeReached(&failsafes.tSaved,60000)){

//     // uint8_t fMessageToSend = false;
//     // char tmpctr[100];

//     // for(int device_id=0;device_id<4;device_id++){
//     //   if((heating_device_relays[device_id].time_minutes_on>FAILSAFE_MINUTES_WARNING)&&(heating_device_relays[device_id].time_minutes_on<FAILSAFE_MINUTES_ERROR)){
//     //     memset(tmpctr,0,sizeof(tmpctr));
//     //     sprintf(tmpctr, "Warning, %s heating has been on for a long time of %d minutes",GetDeviceNameLongbyIDCtr(device_id),heating_device_relays[device_id].time_minutes_on);
//     //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_WARNING "\"%s\""),tmpctr);
//     //     fMessageToSend = true;
//     //   }else if(heating_device_relays[device_id].time_minutes_on>FAILSAFE_MINUTES_ERROR){
//     //     memset(tmpctr,0,sizeof(tmpctr));
//     //     sprintf(tmpctr, "Error, %s heating has been on for too long. Turning off now.",GetDeviceNameLongbyIDCtr(device_id));
//     //     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_ERROR "\"%s\""),tmpctr);
//     //     fMessageToSend = true;
//     //   }

//     //   // if(fMessageToSend){
//     //   //   StaticJsonDocument<300> doc;
//     //   //   JsonObject obj = doc.to<JsonObject>();
//     //   //   obj[D_JSON_TYPE] = "voice+notification"; // broadcast (public/non bedroom speakers), notification(on phones) //SPECIAL, can contain both
//     //   //   obj[D_JSON_MESSAGE] = tmpctr;
//     //   //   data_buffer.payload.len = measureJson(obj)+1;
//     //   //   serializeJson(doc,data_buffer.payload.ctr);
//     //   //   pCONT->mqt->ppublish("status/alert",data_buffer.payload.ctr,false);
//     //   // }
//     // }
//   // } // end timer check

// } //end fucntion


// void mHeating::FunctionHandler_Relay_Status(){ DEBUG_LINE;

//   uint8_t relay_state = 0;
//   for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){

//     switch(device_id){
//       case DEVICE_DS_ID: relay_state = GetHeatingRelay(DEVICE_DS_ID); break;
//       case DEVICE_US_ID: relay_state = GetHeatingRelay(DEVICE_US_ID); break;
//       case DEVICE_WB_ID: relay_state = GetHeatingRelay(DEVICE_WB_ID); break;
//       case DEVICE_IH_ID: relay_state = GetHeatingRelay(DEVICE_IH_ID); break;
//     }

//     //removed minutes on from here, now check relays

//   }

// } // END function


// // This function detects if the traditional heating system has been activated.
// // Methods I can use:
// //  - furnace active sensor without my own relays on, which means I need to listen for data from oilfurnace
// //  - if temp is above a limit (ie radiators when the pulse on and off) and if +ve ROC exceeds a value
// int8_t mHeating::Tasker_PredictManualHeating(){

// //if ROC > X with programs off = on

// // /if(pCONT->mhs->watertemps.tank_top.ischanged){
// //
// //   // test for rising tank temps
// //   if(watertemps.tank_top.roc1m.val>0.2){
// //     pCONT->mso->MessagePrintln("watertemps.tank_top.roc1m.val>0.2");
// //   }
// //
// // }

// }


// /*******************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ********* HEATING TIMERS ***************************************************************************************************************************************
// *************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ***********************************************************************************************************************************************
// ************************************************************************************************************************************************/

// void mHeating::FunctionHandler_Programs_Timers(void){

//   char buffer[50];
//   DEBUG_LINE;

//   uint8_t log_level = LOG_LEVEL_INFO;//LOG_LEVEL_DEBUG;
//   for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){
//     if(program_timers[device_id].time_minutes_on){log_level = LOG_LEVEL_INFO;}
//     AddLog_P(log_level, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "\"%s\" %d min"),GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),program_timers[device_id].time_minutes_on);
//   }
  
//   uint8_t state_tmp = 0;

//   for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){

//     if(program_timers[device_id].time_minutes_on == 0){
//       program_timers[device_id].time_minutes_on = -1; //to disable it
//       program_timers[device_id].time_minutes_on_start = program_timers[device_id].time_minutes_on;
//       SetHeater(device_id, RELAY_STATE_OFF); //turn off
//       program_timers[device_id].ischanged=true;
//       state_tmp = ACTIVEP_OFF; // #1 error needs to be done here
//       activeprograms[device_id].timers.state = 0;  
//     }else 
//     if(program_timers[device_id].time_minutes_on > 0){
//       program_timers[device_id].time_minutes_on--;
//       SetHeater(device_id, RELAY_STATE_ON); // only needs called once, that or add ifnoton inside this function turn on
//       program_timers[device_id].ischanged=true;
//       state_tmp = ACTIVEP_ON;
//       activeprograms[device_id].timers.state = 1;        
//     }else{ // <=1 not active
//       activeprograms[device_id].timers.state = 0;  
//     }

//     // Update state change and run special functions if needed // makes no sense
//     // if(activeprograms[device_id].timers.state != state_tmp){ // CAUSES ERROR #1 - duplicate on state
//     //   activeprograms[device_id].timers.state = state_tmp;
//     //   SendMQTTAlertOnChangeofState(device_id,state_tmp);//device,new state
//     // }

//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING "activeprograms[%d].timers.state = %d"),device_id,
//       activeprograms[device_id].timers.state);
    
//   } // end for

//   isanychanged_timers = 0;
//   for(uint8_t device_id=0;device_id<4;device_id++){
//     isanychanged_timers += program_timers[device_id].ischanged;
//   }

// } // end function



// /*******************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ********* Heating TEMPS and Schedules ***************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************/

// #ifdef USE_HEATING_TEMPS


// void mHeating::FunctionHandler_Programs_Temps(void){

// // DISABLE UNTIL REWRITE
//   // Update times running 1/minute
//   if(mTime::TimeReached(&tSavedHeatingTemps2,60000)){
//     SubTask_HeatingTemps_Ticker();
//     fHeatingTempsChanged = true;
//   }

//   // Update temp variables 1/sec
//   if(mTime::TimeReached(&tSavedHeatingTemps,60000)||fForceHeatingTempsUpdate){
//     SubTask_HeatingTemps();
//   }

//   // Update mqtt status info (5 secs)
//   if(mTime::TimeReached(&tSavedHeatingTemps3,60000)||fForceHeatingTempsUpdate){
//     SubTask_HeatingTemps_StatusMessage();
//   }


//   // Use 1 ROC to deactive rapid heating decreases indicating using water

//   fForceHeatingTempsUpdate = false; // RESET FLAG 

// }


// void mHeating::SubTask_HeatingTemps_Ticker(void){

//   for(int device_id=0;device_id<4;device_id++){
//     if(program_temps[device_id].time_running.on>=0){
//       program_temps[device_id].time_running.on++;
//     }
//   }

// }

// uint8_t mHeating::CheckIfROCExceedsNegativeLimit(uint8_t id){

//   // switch(id){
//   //   case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
//   //   case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
//   //   case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
//   //   case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
//   //   case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
//   //   case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
//   //   case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
//   //   case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
//   // }

//   // if(pCONT->mhs->watertemps.ptr->roc1m.val < -10){ // large change = water emptying therefore turn off
//   //   return 1;
//   // }
//   return 0;

// }// END function

// // Change to switch cases later for easier reading
// void mHeating::SubTask_HeatingTemps(void){

//   // NEED TO HAVE CUTOFF WHEN ITS USED PRIOR TO HITTING TEMP

//   float measured_temp=0;

//   for(int device_id=0;device_id<HEATING_DEVICE_TIMERS_MAX;device_id++){ //needs converted to only run this seciton of code IF timer>-1 so on, but less than max

//     //run on every loop
//     switch(device_id){
//       case DEVICE_DS_ID: measured_temp = pCONT->msdht->sensor[DHT_DOWNSTAIRS_ID].instant.temperature; break;
//       case DEVICE_US_ID: measured_temp = pCONT->msdht->sensor[DHT_UPSTAIRS_ID].instant.temperature; break;
//       case DEVICE_WB_ID: measured_temp = pCONT_msdb18->sensor[ID_DB18_WB].reading.val; break;
//       case DEVICE_IH_ID: measured_temp = pCONT_msdb18->sensor[ID_DB18_IH].reading.val; break;
//     }

//     // Recaulate parameters
//     program_temps[device_id].temp.current = measured_temp;
//     program_temps[device_id].temp.error = measured_temp - program_temps[device_id].temp.desired;

//     // Schedule temp settings
//     switch(program_temps[device_id].schedule.mode_sch){
//       case SCHEDULED_MANUAL_ON_ID:
//         program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
//         activeprograms[device_id].temps.state = ACTIVEP_ON;
//         fRunTemp = true;
//         break;
//       case SCHEDULED_ON_ID: //IE RUNNING
//         program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
//         activeprograms[device_id].temps.state = ACTIVEP_ON;
//         fRunTemp = true;
//         //if on, check time limits
//       break;
//       case SCHEDULED_SET_ID:{

//           //program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;

//           activeprograms[device_id].temps.state = ACTIVEP_SCHEDULED;

//           //uint8_t fTimeReached = pCONT_time->CheckBetween_Week_DateTimes(&program_temps[device_id].schedule.ontime,&program_temps[device_id].schedule.offtime);

//           uint8_t fTimeReached = pCONT_time->CheckDateTimeWeekIsNow(&program_temps[device_id].schedule.ontime);

//           program_temps[device_id].schedule.untilontime = pCONT_time->GetDifferenceInDateTimes(&pCONT_time->RtcTime,&program_temps[device_id].schedule.ontime);

//           AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "CheckBetween_Week_DateTimes [%d]"),fTimeReached);

//           // uint8_t hours, minutes, seconds;
//           // pCONT_time->DateTimeWeek2HHMMSS(&program_temps[device_id].schedule.untilontime,&hours,&minutes,&seconds);
//           //
//           // pCONT->mso->MessagePrint("hours> "); pCONT->mso->MessagePrintln(hours);
//           // pCONT->mso->MessagePrint("minutes> "); pCONT->mso->MessagePrintln(minutes);
//           // pCONT->mso->MessagePrint("seconds> "); pCONT->mso->MessagePrintln(seconds);

//           // if is reached AND NOT run yet
//           if(fTimeReached&&(!program_temps[device_id].schedule.fHasRun)){
//             program_temps[device_id].schedule.fHasRun = 1;
//             program_temps[device_id].schedule.mode_sch = SCHEDULED_ON_ID;
//             program_temps[device_id].time_running.on = 0;
//           }

//         // if(program_temps[device_id].schedule.fRunning){}

//         // check times and update time until it will run (use this as seconds count check?)
//         fRunTemp = false;
//       }
//       break;
//       case SCHEDULED_OFF_ID: // off, no temp heating
//         activeprograms[device_id].temps.state = ACTIVEP_OFF;
//         fRunTemp = false;
//       break;
//     }


//     // Running section of code
//     if(fRunTemp){
 
//      // program_temps[device_id].mqtt.fForceUpdate = 1;  //CAUSES ERROR
//       //pCONT->mso->MessagePrint(program_temps[device_id].mqtt.fForceUpdate);

//       // if timer is active
//       if(program_temps[device_id].time_running.on >= 0){

//           // Time checks - if we have hit max time on (INCLUDES maintain)
//           if(program_temps[device_id].time_running.on > program_temps[device_id].time_running.limit){   
//             AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),program_temps[device_id].time_running.on,program_temps[device_id].time_running.limit);
//             program_temps[device_id].time_running.on = -1; //deactviate
//             program_temps[device_id].time_maintaining.limit = -1;
//           }else{ //stay on
//             SetHeater(device_id,1);
//             program_temps[device_id].status.mode = TEMP_MODE_HEATING_ID;
//             activeprograms[device_id].temps.state = ACTIVEP_ON;
//           }

//           // Temp checks
//           if(measured_temp >= program_temps[device_id].temp.desired){   
//             AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Measured Temp [%f] > Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
//             AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Exceeded Set Point"));
      
//             // Check should we maintain temp (> minus 1, less than max)
//             if((program_temps[device_id].time_maintaining.on>=0)&&(program_temps[device_id].time_maintaining.on < program_temps[device_id].time_maintaining.limit)){
//               program_temps[device_id].status.mode = TEMP_MODE_MAINTAINING_ID;
//               activeprograms[device_id].temps.state = ACTIVEP_ON;
//             }
//             // Turn off
//             else{
              
//               AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "desired_count > desired_max"));
      
//               program_temps[device_id].time_running.on = -2; // -1 is just not active, -2 is a freeze out that lasts one hour unless overriden
//               program_temps[device_id].time_maintaining.limit = -1;

//               //record time it turned off ie record temp difference and how long we took to get there, I can use this later to estimate


//               //add mode to repeat schedule

//               //will make a basic estimate table ie time per degree

//               SetHeater(device_id,0);

//               if(program_temps[device_id].schedule.mode_sch == SCHEDULED_ON_ID){
//                 program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
//                 program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
//                 activeprograms[device_id].temps.state = ACTIVEP_OFF;
//               }else{ //manual
//                 program_temps[device_id].schedule.mode_sch = SCHEDULED_OFF_ID;
//                 program_temps[device_id].status.mode = TEMP_MODE_OFF_ID;
//                 activeprograms[device_id].temps.state = ACTIVEP_OFF;
//               }

//               fForceHeatingTempsUpdate = true;

//             }

//       }//if temp check
//       else{ 
//         AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "Measured Temp [%f] < Set Temp [%f]"),measured_temp,program_temps[device_id].temp.desired);
//       }
//       // else{
//       //   program_temps[device_id].status.mode = TEMP_MODE_OFF;
//       //   activeprograms[device_id].temps.state = ACTIVEP_OFF;
//       // }// end if timer active

//     } //if time running on

//   }//if(fRunTemp){

//   }//end for loop

// }//end temps fucntion




// // MESSAGES
// // Heating for 0/120 minutes
// // Maintaining for 0/120 minutes
// // Off : Unset
// // Waiting: Set for 07:00 (T-03:45)
// void mHeating::SubTask_HeatingTemps_StatusMessage(){

//   int16_t timeon;
//   char tmp[10];
//   char namectr[2];
//   char time_ctr[50];
//   uint8_t ischanged = 0;
//   float valf,val;
//   int vali;
//   int i=0;

//   char tmpctr[50];  memset(tmpctr,0,sizeof(tmpctr));

//   for(int device_id=0;device_id<4;device_id++){

//     // clear message
//     memset(&program_temps[device_id].status.data,0,sizeof(program_temps[device_id].status.data));

//     switch(program_temps[device_id].status.mode){
//       default:
//         memcpy(&program_temps[device_id].status.data.ctr[0],"Error:Unknown",sizeof("Error:Unknown")-1);
//       break;
//       case TEMP_MODE_OFF_ID:
//         memcpy(&program_temps[device_id].status.data.ctr[0],"Off",sizeof("Off")-1); program_temps[device_id].status.data.len += sizeof("Off")-1;
//       break;
//       case TEMP_MODE_HEATING_ID: // Heating for 0/120 minutes
//         memset(time_ctr,'\0',sizeof(time_ctr));
//         sprintf(time_ctr, "Heating for %d/%d minutes",(program_temps[device_id].time_running.on<1?0:program_temps[device_id].time_running.on),(program_temps[device_id].time_running.limit));
//         memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//         program_temps[device_id].status.data.len += strlen(time_ctr);
//       break;
//       case TEMP_MODE_MAINTAINING_ID:
//         memset(time_ctr,'\0',sizeof(time_ctr));
//         sprintf(time_ctr, "Maintaining for %d/%d minutes",(program_temps[device_id].time_maintaining.on<1?0:program_temps[device_id].time_maintaining.on),(program_temps[device_id].time_maintaining.limit));
//         memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//         program_temps[device_id].status.data.len += strlen(time_ctr);
//       break;
//       case TEMP_MODE_SCHEDULED_ID: // Waiting: Set for 07:00 (T-03:45)
//         memset(time_ctr,'\0',sizeof(time_ctr));
//         sprintf(time_ctr, "Set for %02d:%02d (in %02d:%02d)",
//         program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,
//         program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,program_temps[device_id].schedule.untilontime.second);
//         memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//         program_temps[device_id].status.data.len += strlen(time_ctr);
//       break;
//       case TEMP_MODE_SPLASH_RUN_TIME_ID:
//         // Scheduled to run in xx hours and yy minutes (xx:xx:xx/yy:yy:yy)
//         if(abs(millis()-program_temps[device_id].status.tSplashTime)<10000){//program_temps[device_id].status.msSplashtime){
//           memset(time_ctr,'\0',sizeof(time_ctr));
//           sprintf(time_ctr, "Running in %d hours and %d minutes (%02d:%02d:%02d/%02d:%02d:%02d)",//abs(millis()-program_temps[device_id].status.tSplashTime),
//             program_temps[device_id].schedule.untilontime.hour,program_temps[device_id].schedule.untilontime.minute,
//             program_temps[device_id].schedule.ontime.hour,program_temps[device_id].schedule.ontime.minute,program_temps[device_id].schedule.ontime.second,
//             program_temps[device_id].schedule.offtime.hour,program_temps[device_id].schedule.offtime.minute,program_temps[device_id].schedule.offtime.second);
//           memcpy(&program_temps[device_id].status.data.ctr[program_temps[device_id].status.data.len],time_ctr,strlen(time_ctr));
//           program_temps[device_id].status.data.len += strlen(time_ctr);
//          }else{
//            program_temps[device_id].status.mode = program_temps[device_id].status.mode_next;
//          }
//       break;
//     }//END switch
//   }//END for
// }// END function

// #ifdef USE_HEATING_PROFILE_ESTIMATION
    
// // 6.25-25
// // 6.55 -49.3
// // 24.3 in 30 minutes
// // 24.4/30 = deg/min = 0.8133
// // 30/24.4 = minutes per degree
// //float temp_step = 1.0/100.0; //degree range 0-1
// void mHeating::init_HeatingProfiles(){

//   for(int device_id=0;device_id<4;device_id++){

//     switch(device_id){
//       default:
//       case DEVICE_DS_ID:
//       case DEVICE_US_ID:{
//         heating_profiles[device_id].temperature_max = 25;
//         heating_profiles[device_id].temperature_min = 15;
//         heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
//         for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
//           heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
//           heating_profiles[device_id].duration_secs[index] = (60*30)/10; // seconds
//         }
//       }
//       break;
//       case DEVICE_IH_ID:{
//         heating_profiles[device_id].temperature_max = 60;
//         heating_profiles[device_id].temperature_min = 15;
//         heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
//         for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
//           heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
//           heating_profiles[device_id].duration_secs[index] = 214/2; // seconds
//         }
//       }
//       break;
//       case DEVICE_WB_ID:{
//         heating_profiles[device_id].temperature_max = 60;
//         heating_profiles[device_id].temperature_min = 15;
//         heating_profiles[device_id].temperature_step = (heating_profiles[device_id].temperature_max-heating_profiles[device_id].temperature_min)/100.0; //degree range 0-1
//         for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){ //index = percentage
//           heating_profiles[device_id].temperature[index] = mSupport::mapfloat(index,0,100,heating_profiles[device_id].temperature_min,heating_profiles[device_id].temperature_max);
//           heating_profiles[device_id].duration_secs[index] = 70;//140; // seconds by 1 degree then convert to new range
//         }
//       }
//       break;
//     }
//   }
// }

// void mHeating::FunctionHandler_Heating_Profiles(){

//  // if(abs(millis()-tSavedHeatingProfiles)>60000){tSavedHeatingProfiles = millis();

//     AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROFILES));
            
//     //MQQTSendHeatingProfile_Raw_IfChanged();

//     //GetHeatingProfilesTimeSeconds(DEVICE_IH,random(20,30)*1.12,40);

//   //}

// }

// uint32_t mHeating::GetHeatingProfilesTimeMinutes(uint8_t device_id, float temp_now, float temp_target){
//   uint32_t seconds = GetHeatingProfilesTimeSeconds(device_id, temp_now, temp_target);
//   uint32_t minutes = seconds/60;
//   return minutes;
// }

// uint32_t mHeating::GetHeatingProfilesTimeSeconds(uint8_t device_id, float temp_now, float temp_target){

// uint32_t seconds=0;
// uint8_t index_now=20, index_target=30;

// /*

// //find nerest temp to now (get index)
// float difference[HEATINGPROFILE_RESOLUTION];

// // temp_now = 50;
// // for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
// //   heating_profiles[device_id].temperature[index]=index;
// // }

// // Get difference
// for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
//   difference[index] = fabs(temp_now - heating_profiles[device_id].temperature[index]);
//   //Serial.print(fabs(temp_now - heating_profiles[device_id].temperature[index])); Serial.print("_");
//   //Serial.print(difference[index]); Serial.println("_");
// }

// //Get smallest difference ie closest
// float lowest_difference_val,lowest_difference_temp_val;// = min()
// uint8_t lowest_difference_index;

// lowest_difference_val = difference[0];// record first value
// for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
//   if(difference[index]<lowest_difference_val){
//     lowest_difference_val = difference[index];
//     lowest_difference_temp_val = heating_profiles[device_id].temperature[index];
//     lowest_difference_index = index;
//     // pCONT->mso->MessagePrint("lowest_difference_val<difference[index]> ");
//     // pCONT->mso->MessagePrint(lowest_difference_val);
//     // pCONT->mso->MessagePrint("lowest_difference_temp_val> ");
//     // pCONT->mso->MessagePrint(lowest_difference_temp_val);
//     // pCONT->mso->MessagePrint(" lowest_difference_index> ");
//     // pCONT->mso->MessagePrintln(lowest_difference_index);
//   }
// }



// // pCONT->mso->MessagePrint("[HEAT] Searching for [");
// // pCONT->mso->MessagePrint(temp_now);
// // pCONT->mso->MessagePrint("]");
// //
// // pCONT->mso->MessagePrint(" found [");
// // pCONT->mso->MessagePrint(heating_profiles[device_id].temperature[lowest_difference_index]);
// // pCONT->mso->MessagePrint("] at index [");
// // pCONT->mso->MessagePrint(lowest_difference_index);
// // pCONT->mso->MessagePrintln("]");

// index_now = lowest_difference_index;

// //find nereast temp to target (get index)

// // Get difference
// for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
//   difference[index] = fabs(temp_target - heating_profiles[device_id].temperature[index]);
//   //Serial.print(fabs(temp_now - heating_profiles[device_id].temperature[index])); Serial.print("_");
//   //Serial.print(difference[index]); Serial.println("_");
// }

// //Get smallest difference ie closest
// // float lowest_difference_val,lowest_difference_temp_val;// = min()
// // uint8_t lowest_difference_index;

// lowest_difference_val = difference[0];// record first value
// for(int index=0;index<HEATINGPROFILE_RESOLUTION;index++){
//   if(difference[index]<lowest_difference_val){
//     lowest_difference_val = difference[index];
//     lowest_difference_temp_val = heating_profiles[device_id].temperature[index];
//     lowest_difference_index = index;
//     // pCONT->mso->MessagePrint("lowest_difference_val<difference[index]> ");
//     // pCONT->mso->MessagePrint(lowest_difference_val);
//     // pCONT->mso->MessagePrint("lowest_difference_temp_val> ");
//     // pCONT->mso->MessagePrint(lowest_difference_temp_val);
//     // pCONT->mso->MessagePrint(" lowest_difference_index> ");
//     // pCONT->mso->MessagePrintln(lowest_difference_index);
//   }
// }



// // pCONT->mso->MessagePrint("[HEAT] Searching for [");
// // pCONT->mso->MessagePrint(temp_target);
// // pCONT->mso->MessagePrint("]");
// //
// // pCONT->mso->MessagePrint(" found [");
// // pCONT->mso->MessagePrint(heating_profiles[device_id].temperature[lowest_difference_index]);
// // pCONT->mso->MessagePrint("] at index [");
// // pCONT->mso->MessagePrint(lowest_difference_index);
// // pCONT->mso->MessagePrintln("]");

// index_target = lowest_difference_index;

// //sum seconds
// for(int ii=index_now;index_now<index_target;index_now++){
//   seconds += heating_profiles[device_id].duration_secs[ii];
//   //Serial.print("secs> ");
//   //Serial.println(seconds);
// }

//   // Possibly adding heating start time too? so curves are assuming heating is on

//   datetime_t untiltime = pCONT_time->GetDifferenceInDateTimes(&pCONT_time->RtcTime,&heating_device_relays[device_id].ontime);
//   uint32_t secondson = pCONT_time->GetSecondsOfDayFromDateTime(&untiltime);

//   int cold_start_seconds = 0;
//   float sensor_temp = 0;
//   // Add time on if system is only starting
//   if(secondson<(60*10)){




//     switch(device_id){   /// WRONG INDEXING -- NEED TO USE FUNCTION THAT RETURNS TEMP CORRECTLY FOR ID, NOT BASED ON INDEX, OR RETURNS POINTER TO THE TEMP INFO CONTAINED WITHIN IT
//       case ID_DB18_DS: sensor_temp = pCONT_msdb18->sensor[ID_DB18_DS].reading.val; break;
//       case ID_DB18_US: sensor_temp = pCONT_msdb18->sensor[ID_DB18_US].reading.val; break;
//       case ID_DB18_WB: sensor_temp = pCONT_msdb18->sensor[ID_DB18_WB].reading.val; break;
//       case ID_DB18_IH: sensor_temp = pCONT_msdb18->sensor[ID_DB18_IH].reading.val; break;
//     }

//     // Colder = more time added
//     cold_start_seconds = map(sensor_temp,15,50,(60*10),1);
//     cold_start_seconds = constrain(cold_start_seconds,1,(60*10));

//     seconds += cold_start_seconds;// 10 minutes needs to go by relay on timer (needs added!)
//   }
// */
// return seconds;
// }








// void mHeating::ConstructJSON_HeatingProfile_Raw(uint8_t device_id){

//   // StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
//   // JsonObject obj = doc.to<JsonObject>();

//   // obj[D_JSON_SIZE] = HEATINGPROFILE_RESOLUTION;
  
//   // JsonObject temp = obj.createNestedObject(D_JSON_TEMP);
//   //   temp[D_JSON_MAX] = mSupport::roundfloat(heating_profiles[device_id].temperature_max,1);
//   //   temp[D_JSON_MIN] = mSupport::roundfloat(heating_profiles[device_id].temperature_min,1);;
//   //   temp[D_JSON_STEP] = mSupport::roundfloat(heating_profiles[device_id].temperature_step,1);;
//   //   JsonArray temparr = temp.createNestedArray(D_JSON_VALUE);
//   //   for(int i=0;i<HEATINGPROFILE_RESOLUTION;i++){
//   //     temparr.add(mSupport::roundfloat(heating_profiles[device_id].temperature[i],1));
//   //   }

//   // JsonArray dursarr = obj.createNestedArray(D_JSON_DURATION);
//   // for(int i=0;i<HEATINGPROFILE_RESOLUTION;i++){
//   //   dursarr.add(mSupport::roundfloat(heating_profiles[device_id].duration_secs[i],1));
//   // }

//   // data_buffer.payload.len = measureJson(obj)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);

//   // return 0;

// }
// #endif

// #endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
    









// /*******************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ********* HEating TEMPS and Schedules ***************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************/










// #ifdef USE_MODULE_NETWORK_MQTT

// // schedule.run_time:hh:mm
// // schedule.run_days:M-W-FSS
// // schedule.mode
// // temp.set
// // temp.max_time
// // new time values that make sense

// /*********************************************************************** JSON Payload Constructors *********************************************************************************************/

// uint8_t mHeating::ConstructJSON_ProgramTimers(uint8_t json_level){

//   JsonBuilderI->Start();
  
//   char buffer[50];
//   for(int ii=0;ii<4;ii++){

//     if(CheckAndClearFlag(&program_timers[ii].ischanged) || 
//       (json_level > JSON_LEVEL_IFCHANGED)
//     ){
//       JsonBuilderI->Level_Start(GetDeviceNamebyIDCtr(ii,buffer, sizeof(buffer)));
//         JsonBuilderI->Add(D_JSON_TIME_ON, program_timers[ii].time_minutes_on==-1?0:program_timers[ii].time_minutes_on); // -1 is off, but only send 0 as limit
//       JsonBuilderI->Level_End();
//     } //END if

//   } //END for

//   return JsonBuilderI->End();
  
// }

// //Update: when times changed (set forced), or 60s elapsed
// //new method, diving up Constructs if they are too big, return if all data was sent
// uint8_t mHeating::ConstructJSON_ProgramTemps(uint8_t json_level){ //MQQTSendTimers
// // return false;
//   char buffer[50];
//   JsonBuilderI->Start();
//   for(int device=0;device<4;device++){

//     // if((program_temps[device].mqtt.fForceUpdate)||(json_level==JSON_LEVEL_IFCHANGED)){program_temps[device].mqtt.fForceUpdate = false;

//       JsonBuilderI->Level_Start_P(GetDeviceNamebyIDCtr(device, buffer, sizeof(buffer))); 
//         JsonBuilderI->Level_Start_P(D_JSON_TEMPERATURE);   
//           JsonBuilderI->Add(D_JSON_CURRENT, program_temps[device].temp.current);
//           JsonBuilderI->Add(D_JSON_DESIRED, program_temps[device].temp.desired);
//           JsonBuilderI->Add(D_JSON_ERROR, program_temps[device].temp.error);
//         JsonBuilderI->Level_End();
//         JsonBuilderI->Level_Start_P(D_JSON_STATUS);   
//           JsonBuilderI->Add(D_JSON_MODE, GetTempModeByDeviceIDCtr(device, buffer, sizeof(buffer)));
//           JsonBuilderI->Add(D_JSON_DATA, program_temps[device].status.data.ctr);
//         JsonBuilderI->Level_End();

//     //       JsonObject maintainingobj = deviceobj.createNestedObject(D_JSON_TIME_MAINTAINING);
//     //         maintainingobj[D_JSON_ONTIME] = program_temps[device].time_maintaining.on;
//     //         maintainingobj[D_JSON_LIMIT] = program_temps[device].time_maintaining.limit;

//     //       JsonObject timerunningobj = deviceobj.createNestedObject(D_JSON_TIME_RUNNING);
//     //         timerunningobj[D_JSON_ONTIME] = program_temps[device].time_maintaining.on;
//     //         timerunningobj[D_JSON_LIMIT] = program_temps[device].time_maintaining.limit;

//     //       #ifdef USE_HEATING_TEMPS
//     //       // JsonObject time_to_heatobj = deviceobj.createNestedObject(D_JSON_TIME_TO_HEAT);
//     //       //   time_to_heatobj[D_JSON_SECONDS] = GetHeatingProfilesTimeSeconds(device,program_temps[device].temp.current,program_temps[device].temp.desired);
//     //       //   time_to_heatobj[D_JSON_MINUTES] = GetHeatingProfilesTimeMinutes(device,program_temps[device].temp.current,program_temps[device].temp.desired);
//     //     #endif

//     //     JsonObject scheduleobj = deviceobj.createNestedObject(D_JSON_SCHEDULE);
//     //       scheduleobj[D_JSON_MODE] = GetScheduleNameCtrbyID(program_temps[device].schedule.mode_sch);
//     //       scheduleobj[D_JSON_ISRUNNING] = program_temps[device].schedule.fRunning;

//     //       // char time_ctr[20]; memset(time_ctr,'\0',sizeof(time_ctr));
//     //       // sprintf(time_ctr, "%02d:%02d:%02d",(int)program_temps[device].schedule.ontime.hour,(int)program_temps[device].schedule.ontime.minute,(int)program_temps[device].schedule.ontime.second);
//     //       // scheduleobj[D_JSON_ONTIME] = time_ctr;//pCONT_time->getFormattedTime();
          
//     //       // char time_ctr2[20];
//     //       // memset(time_ctr2,'\0',sizeof(time_ctr2));
//     //       // sprintf(time_ctr2, "%02d:%02d:%02d",(int)program_temps[device].schedule.offtime.hour,(int)program_temps[device].schedule.offtime.minute,(int)program_temps[device].schedule.offtime.second);
//     //       // scheduleobj[D_JSON_OFFTIME] = time_ctr2;//pCONT_time->getFormattedTime();

//     //     //data_buffer.payload.json_pairs++;
//     JsonBuilderI->Level_End();
//     // }
//   }
//   return JsonBuilderI->End();

// }



// // relays{ds,us},timers{ds,us},temps{ds,us} in simple 1 or 0
// uint8_t mHeating::ConstructJSON_ProgramActive(uint8_t json_level){
//   JsonBuilderI->Start();
//     JsonBuilderI->Level_Start_P(D_JSON_RELAY);   
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, GetHeatingRelay(DEVICE_US_ID));
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, GetHeatingRelay(DEVICE_DS_ID));
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, GetHeatingRelay(DEVICE_IH_ID));
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, GetHeatingRelay(DEVICE_WB_ID));
//       JsonBuilderI->Add(D_JSON_ANY,                     GetAnyHeatingRelay());      
//     JsonBuilderI->Level_End();
//     JsonBuilderI->Level_Start_P(D_JSON_TIMER);  
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].timers.state); 
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].timers.state); 
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].timers.state); 
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].timers.state); 
//     JsonBuilderI->Level_End();
//     JsonBuilderI->Level_Start_P(D_JSON_TEMP);  
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].temps.state); 
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].temps.state); 
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].temps.state); 
//       JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].temps.state); 
//     JsonBuilderI->Level_End();
//     JsonBuilderI->Level_Start_P(D_JSON_STATUS);  
//       JsonBuilderI->Add(D_JSON_MESSAGE, status_message.ctr);  //nice human message that remains the same
//       JsonBuilderI->Add(D_JSON_LENGTH, status_message.len); 
//       JsonBuilderI->Add(D_JSON_IMPORTANCE, status_message.importance); 
//     JsonBuilderI->Level_End();
//   return JsonBuilderI->End();
// }


// uint8_t mHeating::ConstructJSON_HeatingRelays(uint8_t json_level){

//   char buffer[50];
//   JsonBuilderI->Start();
//   for(int device_id=0;device_id<4;device_id++){
//     JsonBuilderI->Level_Start_P(pCONT_set->GetDeviceName(D_MODULE_DRIVERS_RELAY_ID, device_id, buffer, sizeof(buffer)));
//       JsonBuilderI->Add_FV(D_JSON_ONTIME, PSTR("\"%02d:%02d:%02d\""),  pCONT_mry->relay_status[device_id].last.ontime.hour,  pCONT_mry->relay_status[device_id].last.ontime.minute,  pCONT_mry->relay_status[device_id].last.ontime.second);
//       JsonBuilderI->Add_FV(D_JSON_OFFTIME, PSTR("\"%02d:%02d:%02d\""), pCONT_mry->relay_status[device_id].last.offtime.hour,  pCONT_mry->relay_status[device_id].last.offtime.minute,  pCONT_mry->relay_status[device_id].last.offtime.second);
//       JsonBuilderI->Add(D_JSON_TIME_ON "_Mins",   pCONT_mry->relay_status[device_id].time_minutes_on);
//     JsonBuilderI->Level_End();
//   }
//   return JsonBuilderI->End();

// }



// // Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
// uint8_t mHeating::ConstructJSON_PipeTemps(uint8_t json_level){

//   char buffer[50];
  
//   JsonBuilderI->Start();
//     for(int i=0;i<pCONT_msdb18->db18_sensors_active;i++){ 
//       if(pCONT_sup->JsonLevelFlagCheck(JSON_LEVEL_IFCHANGED, json_level, pCONT_msdb18->sensor[i].reading.ischanged)){
//         JsonBuilderI->Level_Start_P(pCONT_set->GetDeviceName(D_MODULE_SENSORS_DB18S20_ID, i, buffer, sizeof(buffer)));
//           JsonBuilderI->Add(D_JSON_TEMPERATURE, pCONT_msdb18->sensor[i].reading.val);
//           JsonBuilderI->Add(D_JSON_ISVALID, pCONT_msdb18->sensor[i].reading.isvalid);
//           JsonBuilderI->Add(D_JSON_SECS, (int)abs(pCONT_time->uptime.seconds_nonreset-pCONT_msdb18->sensor[i].reading.captureupsecs));
//         JsonBuilderI->Level_End();
//       } 
//     }
//   return JsonBuilderI->End();

// }


// // Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
// uint8_t mHeating::ConstructJSON_PipeTempsROC10s(uint8_t json_level){

// return 0;
//   // StaticJsonDocument<700> doc;
//   // JsonObject obj = doc.to<JsonObject>();
//   // D_DATA_BUFFER_CLEAR();

//   // for(uint8_t id=0;id<8;id++){

//   //   switch(id){
//   //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
//   //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
//   //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
//   //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
//   //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
//   //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
//   //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
//   //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
//   //   }

//   //   //if(pCONT->mhs->watertemps.ptr->roc10s.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer.payload.json_pairs++;
//   //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
//   //       json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc10s.val,1);
//   //       json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc10s.isvalid;
//   //       json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.second-pCONT->mhs->watertemps.ptr->roc10s.captureupsecs);
//   //   //}

//   // }

  

//   // data_buffer.payload.len = measureJson(obj)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);
//   // return data_buffer.payload.len>3?1:0;

// }



// // Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
// uint8_t mHeating::ConstructJSON_PipeTempsROC1m(uint8_t json_level){

// return 0;
// //   StaticJsonDocument<700> doc;
// //   JsonObject obj = doc.to<JsonObject>();
// //   D_DATA_BUFFER_CLEAR();

// //   for(uint8_t id=0;id<8;id++){

// //     switch(id){
// //       case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
// //       case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
// //       case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
// //       case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
// //       case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
// //       case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
// //       case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
// //       case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
// //     }

// // //    pCONT->mhs->watertemps.ptr->roc1m.ischanged=1;
// //     //if(pCONT->mhs->watertemps.ptr->roc1m.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer.payload.json_pairs++;
// //       JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
// //       json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc1m.val,1);
// //       json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc1m.isvalid;
// //       json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.second-pCONT->mhs->watertemps.ptr->roc1m.captureupsecs);
// //     //}

// //   }

  

// //   data_buffer.payload.len = measureJson(obj)+1;
// //   serializeJson(doc,data_buffer.payload.ctr);
// //   return data_buffer.payload.len>3?1:0;

// }



// // Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
// uint8_t mHeating::ConstructJSON_PipeTempsROC10m(uint8_t json_level){

//   return 0;

//   // StaticJsonDocument<700> doc;
//   // JsonObject obj = doc.to<JsonObject>();
//   // D_DATA_BUFFER_CLEAR();

//   // for(uint8_t id=0;id<8;id++){

//   //   switch(id){
//   //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
//   //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
//   //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
//   //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
//   //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
//   //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
//   //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
//   //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
//   //   }
    
//   //   //if(pCONT->mhs->watertemps.ptr->roc10m.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer.payload.json_pairs++;
//   //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
//   //     json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc10m.val,1);
//   //     json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc10m.isvalid;
//   //     json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.second-pCONT->mhs->watertemps.ptr->roc10m.captureupsecs);
//   //  // }

//   // }

  

//   // data_buffer.payload.len = measureJson(obj)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);
//   // return data_buffer.payload.len>3?1:0;

// }



// uint8_t mHeating::ConstructJSON_ClimateTemps(uint8_t json_level){

//   uint8_t ischanged=false, isvalid = false;
//   char name_buffer_tmp[25];

//   JsonBuilderI->Start();

//   for(int dht_id=0;dht_id<2;dht_id++){
//     JsonBuilderI->Level_Start(pCONT_set->GetDeviceName(D_MODULE_SENSORS_DHT_ID, dht_id, name_buffer_tmp, sizeof(name_buffer_tmp)));
//       JsonBuilderI->Add(D_JSON_TEMP, pCONT->msdht->sensor[dht_id].instant.temperature);
//       JsonBuilderI->Add(D_JSON_HUM, pCONT->msdht->sensor[dht_id].instant.humidity);
//       JsonBuilderI->Add(D_JSON_ISVALID, pCONT->msdht->sensor[dht_id].instant.isvalid);
//       JsonBuilderI->Add(D_JSON_ISCHANGED, pCONT->msdht->sensor[dht_id].instant.ischanged);
//       // json1["iserrored"] = pCONT->mhs->climate.ptr->iserrored;
//       // json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.seconds_nonreset-pCONT->mhs->climate.ptr->raw.captureupsecs);
//       // json1["heatindex"] = pCONT->mhs->climate.ptr->raw.heatIndex; // DONT KNOW WHAT THEY ARE
//       // json1["dewpoint"] = pCONT->mhs->climate.ptr->raw.dewPoint;
//     JsonBuilderI->Level_End();
//   }

//   JsonBuilderI->End();

// }


// uint8_t mHeating::ConstructJSON_ClimateTempsROC1m(uint8_t json_level){

// // clear entire mqtt packet
// D_DATA_BUFFER_CLEAR();

//   // uint8_t ischanged=false;
//   // StaticJsonDocument<400> doc;
//   // JsonObject obj = doc.to<JsonObject>();

//   // for(int ii=0;ii<2;ii++){

//   //   switch(ii){
//   //     case 0: ischanged = pCONT->mhs->climate.downstairs.roc1m.ischanged; break;
//   //     case 1: ischanged = pCONT->mhs->climate.upstairs.roc1m.ischanged; break;
//   //   }

//   //   //if(ischanged||fSendAllData||fSendSingleFunctionData){
//   //     // data_buffer.payload.json_pairs++;
//   //     switch(ii){
//   //       case 0: pCONT->mhs->climate.ptr = &pCONT->mhs->climate.downstairs; break;
//   //       case 1: pCONT->mhs->climate.ptr = &pCONT->mhs->climate.upstairs; break;
//   //     }
//   //     JsonObject json1 = obj.createNestedObject(GetDeviceNamebyIDCtr(ii));
//   //       json1[D_JSON_TEMP] = pCONT->mhs->climate.ptr->roc1m.temperature;
//   //       json1[D_JSON_HUM] = pCONT->mhs->climate.ptr->roc1m.humidity;
//   //   //}

//   // }


//   // data_buffer.payload.len = measureJson(obj)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);
//   // return data_buffer.payload.len>3?1:0;
//   return 0;

// }

// uint8_t mHeating::ConstructJSON_ClimateTempsROC10m(uint8_t json_level){

//   // clear entire mqtt packet
//   D_DATA_BUFFER_CLEAR();

//   // char namectr[3]; memset(&namectr,0,sizeof(namectr));
//   // uint8_t ischanged=false;

//   // StaticJsonDocument<400> doc;
//   //   JsonObject obj = doc.to<JsonObject>();

//   //  for(int ii=0;ii<2;ii++){

//   //    switch(ii){
//   //      case 0: ischanged = pCONT->mhs->climate.downstairs.roc10m.ischanged; break;
//   //      case 1: ischanged = pCONT->mhs->climate.upstairs.roc10m.ischanged; break;
//   //    }

//   //   // ischanged=1;
//   //  //  if(ischanged||fSendAllData||fSendSingleFunctionData){

//   //     //  data_buffer.payload.json_pairs++;

//   //       switch(ii){
//   //         case 0: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_DS,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.downstairs; break;
//   //         case 1: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_US,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.upstairs; break;
//   //       }

//   //       JsonObject json1 = obj.createNestedObject(namectr);
//   //       json1[D_JSON_TEMP] = pCONT->mhs->climate.ptr->roc10m.temperature;
//   //       json1[D_JSON_HUM] = pCONT->mhs->climate.ptr->roc10m.humidity;

//   //     }

//   //  // }

     

//   // data_buffer.payload.len = measureJson(obj)+1;
//   // serializeJson(doc,data_buffer.payload.ctr);
//   // return data_buffer.payload.len>3?1:0;
//   return 0;

// }



// uint8_t mHeating::ConstructJSON_Settings(uint8_t json_method){

//     // D_DATA_BUFFER_CLEAR();
//     // StaticJsonDocument<400> doc;
//     // JsonObject root = doc.to<JsonObject>();

//     // root["json_teleperiod_level"] = pCONT_set->GetTelePeriodJsonLevelCtr();


//     // data_buffer.payload.len = measureJson(root)+1;
//     // serializeJson(doc,data_buffer.payload.ctr);

//     return 0;//data_buffer.payload.len>3?true:false;

// }



// #endif








// // Keeps the highest importance flag
// void mHeating::SetHighestImportance(uint8_t* importanceset, int8_t thisvalue){
//   if(thisvalue > *importanceset){
//     *importanceset = thisvalue;
//   }
//   //moved into telemetrym
// }


// // returns > 0 for events added
// int8_t mHeating::SubContructCtr_HardwareStatus(){

//   //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
//   uint8_t fNotFirstItem = 0;
//   char buffer[100];

//   for(int device_id=0;device_id<4;device_id++){
//     if(activeprograms[device_id].timers.state){
//       sprintf(&status_message.ctr[status_message.len],"%s timers %s",
//         GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
//         GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state, buffer, sizeof(buffer)));
//       status_message.len = strlen(status_message.ctr);
//       SetHighestImportance(&status_message.importance,activeprograms[device_id].timers.state); //med
//     }
//     if(fNotFirstItem){
//       sprintf(&status_message.ctr[status_message.len],"%c",'c');
//       status_message.len = strlen(status_message.ctr);
//     }
//   }

//   for(int device_id=0;device_id<4;device_id++){
//     if(activeprograms[device_id].temps.state){ status_message.importance = 2;//med
//       sprintf(&status_message.ctr[status_message.len],"%s temps %s",
//         GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
//         GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state, buffer, sizeof(buffer)));
//       status_message.len = strlen(status_message.ctr);
//     }
//     SetHighestImportance(&status_message.importance,activeprograms[device_id].temps.state); //med
//     if(fNotFirstItem){
//       sprintf(&status_message.ctr[status_message.len],"%c",'c');
//       status_message.len = strlen(status_message.ctr);
//     }
//   }

//   for(int device_id=0;device_id<4;device_id++){
//     if(activeprograms[device_id].relays.state){ status_message.importance = 2;//high
//       sprintf(&status_message.ctr[status_message.len],"%s relays %s",
//         GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
//         GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state, buffer, sizeof(buffer)));
//       status_message.len = strlen(status_message.ctr);
//     }
//     SetHighestImportance(&status_message.importance,activeprograms[device_id].relays.state); //med
//     if(fNotFirstItem){
//       sprintf(&status_message.ctr[status_message.len],"%c",'c');
//       status_message.len = strlen(status_message.ctr);
//     }
//   }

//   if(!status_message.len){
//     sprintf(&status_message.ctr[status_message.len],"%s","Nothing Running");
//     status_message.len = strlen(status_message.ctr);
//   }

// }

// // returns > 0 for events added
// int8_t mHeating::SubContructCtr_HardwareStatus_Long(){

//   //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
//   uint8_t fNotFirstItem = 0;

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].timers.state){
//   //     sprintf(&status_message.ctr[status_message.len],"%s Timer %s",
//   //       GetDeviceNameLongbyIDCtr(device_id),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state));
//   //     status_message.len = strlen(status_message.ctr);
//   //     SetHighestImportance(&status_message.importance,activeprograms[device_id].timers.state); //med
//   //   }
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].temps.state){ status_message.importance = 2;//med
//   //     sprintf(&status_message.ctr[status_message.len],"%s Temp %s",
//   //       GetDeviceNameLongbyIDCtr(device_id),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state));
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   //   SetHighestImportance(&status_message.importance,activeprograms[device_id].temps.state); //med
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // for(int device_id=0;device_id<4;device_id++){
//   //   if(activeprograms[device_id].relays.state){ status_message.importance = 2;//high
//   //     sprintf(&status_message.ctr[status_message.len],"%s relays %s",
//   //       GetDeviceNameLongbyIDCtr(device_id),
//   //       GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state));
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   //   SetHighestImportance(&status_message.importance,activeprograms[device_id].relays.state); //med
//   //   if(fNotFirstItem){
//   //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
//   //     status_message.len = strlen(status_message.ctr);
//   //   }
//   // }

//   // if(!status_message.len){
//   //   status_message.len += sprintf(&status_message.ctr[status_message.len],"Online");
//   // }

// } //end function



// int mHeating::mapHeatingTempToHueColour(int val){
//    int val2 = mSupport::mapfloat(val,15.0,50.0,180,0); //float mSupport::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
//    if(val2<0){
//      if(val2<-60){ //limit so we stay within purple and never looping as far as blue
//        val2 = 300;
//      }else{
//        val2 += 360;
//      }
//    }
//   return val2;
// }

// int mHeating::mapHeatingTempToBrightness(int temp){

//   int brightness;

//   if(temp<25){
//     brightness = map(temp,15,25,1,10); //brightness propotional to temp; hotter = brighter
//     brightness = constrain(brightness,9,10);
//   }else{
//     brightness = map(temp,25,50,10,100); //brightness propotional to temp; hotter = brighter
//     brightness = constrain(brightness,10,100);
//   }

//   return brightness;

// }




// // Send all water sensors by [temp,huebytemp(0-360),brightnessbytemp(0-100)]
// // ds:{temp:0,hue:0,bright:0},
// // us:{temp:0,hue:0,bright:0},
// // EFFECTS to be send seperate when programs change state
// uint8_t mHeating::ConstructSON_PipeTempsByColours(uint8_t json_level){

//   // StaticJsonDocument<800> doc;
//   // JsonObject obj = doc.to<JsonObject>();
//   // D_DATA_BUFFER_CLEAR();

//   // uint8_t ischanged = false;

//   // for(uint8_t device_id=0;device_id<8;device_id++){

//   //   switch(device_id){
//   //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
//   //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
//   //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
//   //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
//   //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
//   //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
//   //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
//   //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
//   //   }

//   //   if(stored_new.temp[device_id] != pCONT->mhs->watertemps.ptr->raw.val){
//   //     stored_new.temp[device_id] = pCONT->mhs->watertemps.ptr->raw.val;
//   //     ischanged = 1;
//   //   }
//   //   #ifdef DEBUG_SENDALL
//   //     ischanged=1; //ALL
//   //   #endif
//   //   if(ischanged){ ischanged = 0;
//   //     //data_buffer.payload.json_pairs++;
//   //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(device_id));
//   //     json1[D_JSON_TEMP] = pCONT->mhs->watertemps.ptr->raw.val;
//   //     json1[D_JSON_HUE] = mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val);
//   //     json1[D_JSON_BRT] = mapHeatingTempToBrightness(pCONT->mhs->watertemps.ptr->raw.val);

//   //     char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
//   //     RgbColor c = HsbColor(mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val)/360.0f,100/100.0f,100/100.0f);
//   //     sprintf(tmpctr,"%02X%02X%02X",c.R,c.G,c.B);
//   //     json1[D_JSON_RGB] = tmpctr;

//   //     switch(device_id){
//   //       case ID_DB18_DS: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_DS_ID); break;
//   //       case ID_DB18_US: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_US_ID); break;
//   //       case ID_DB18_WB: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_WB_ID); break;
//   //       case ID_DB18_IH: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_IH_ID); break;
//   //       default: break; //nothing
//   //     }
//   //   }

//   // }

//   // // if(data_buffer.payload.json_pairs>0){
//   //   data_buffer.payload.len = measureJson(obj)+1;
//   //   serializeJson(doc,data_buffer.payload.ctr);
//   //   AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATINGPANEL D_PAYLOAD " \"%s\""),data_buffer.payload.ctr);
//   // // }

//   return 0;// data_buffer.payload.len>3?1:0;

// }


// /*******************************************************************************************************************************************************
// ************************************************************************************************************************************************
// ********* Controls ***************************************************************************************************************************************
// ************************************************************************************************************************************************
// ************************************************************************************************************************************************/

// // reads relay position for devices
// // int8_t mHeating::GetHeater(uint8_t device){
// //   switch(device){
// //     case DEVICE_DS_ID: return GetHeatingRelay(DEVICE_DS_ID);
// //     case DEVICE_US_ID: return GetHeatingRelay(DEVICE_US_ID);
// //     case DEVICE_WB_ID: return GetHeatingRelay(DEVICE_WB_ID);
// //     case DEVICE_IH_ID: return GetHeatingRelay(DEVICE_IH_ID);
// //     default: return -1;
// //   }
// // }

// void mHeating::SetHeater(uint8_t device, uint8_t state){

//   //AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),program_temps[device_id].time_running.on,program_temps[device_id].time_running.limit);

//   // #ifdef ENABLE_MQTT_DEBUG_MESSAGES //debug_message/function 
//   // pCONT_mqtt->Send_Prefixed_P(PSTR("debug_message/setheater"),PSTR("device=%d,state=%d"),device,state);
//   // #endif

//   //add state check that only runs updates below if new state has been selected, else, return
//   if(GetHeatingRelay(device)==state){
//     return;
//   }else{
//     fForceHeatingTempsUpdate = true;
//   }

//   #ifdef ENABLE_RELAY_CONTROLS

//     SetHeatingRelay(device,state);

//   #else
//     //Serial.println(F("SetHeater Disabled for debugging!"));
//     //pCONT->mqt->pubsub->publish("heating/alert","Heating DISABLED for testing!");}
//   #endif

// } // end function























// const char* mHeating::GetActiveProgramNameCtrbyID(uint8_t activeprogram_id, char* buffer, uint8_t buflen){
//   // if(buffer == nullptr){ return 0;}
//     // return (activeprogram_id == ACTIVEP_OFF ? "OFF" :
//     //     (activeprogram_id == ACTIVEP_ON ? "ON" :
//     //     (activeprogram_id == ACTIVEP_SCHEDULED ? "SCH" :
//     //     "unk\0")));

//     snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH);

//     return buffer;
// }


// const char* mHeating::GetDeviceNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen){
//   // if(buffer == nullptr){ return 0;}
//   switch(device_id){ 
//     case DEVICE_DS_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_DS_CTR); break;
//     case DEVICE_US_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_US_CTR); break;
//     case DEVICE_WB_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_WB_CTR); break;
//     case DEVICE_IH_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_IH_CTR); break;
//   }
//   return buffer;
// }
// const char* mHeating::GetDeviceNameLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen){
//   // if(buffer == nullptr){ return 0;}
//   switch(device_id){ 
//     case DEVICE_DS_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_DS_CTR); break;
//     case DEVICE_US_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_US_CTR); break;
//     case DEVICE_WB_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_WB_CTR); break;
//     case DEVICE_IH_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_IH_CTR); break;
//   }
//   return buffer;
// }
// int8_t mHeating::GetDeviceIDbyName(const char* c){
//   if(c=='\0'){
//     return -1;
//   }

// //like getstate, use dlist for different names for same things
// // /strcasecmp_P

//   if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_DS)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_DS_LOWERCASE)){
//     return DEVICE_DS_ID;
//   }else 
//   if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_US)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_US_LOWERCASE)){
//     return DEVICE_US_ID;
//   }else 
//   if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_WB)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_WB_LOWERCASE)){
//     return DEVICE_WB_ID;
//   }else 
//   if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_IH)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_IH_LOWERCASE)){
//     return DEVICE_IH_ID;
//   }else{
//     return -1;
//   }
// }


// #ifdef USE_SCHEDULED_HEATING
// int8_t mHeating::GetScheduleModeIDByCtr(const char* c){
//   if(c=='\0'){ return -1; }
//   // if(strstr(c,D_JSON_SCHEDULED_OFF)){ return SCHEDULED_OFF; }
//   // if(strstr(c,D_JSON_SCHEDULED_SET)){ return SCHEDULED_SET; }
//   // if(strstr(c,D_JSON_SCHEDULED_ON)){ return SCHEDULED_ON; }
//   // if(strstr(c,D_JSON_MANUAL_ON)){ return SCHEDULED_MANUAL_ON; }
//   return -1; // none
// }
// const char* mHeating::GetScheduleNameCtrbyID(uint8_t mode, char* buffer){
//   // if(buffer == nullptr){ return 0;}
//   switch(mode){
//     case SCHEDULED_OFF_ID:       memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_DS_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_DS_CTR)); break;
//     case SCHEDULED_SET_ID:       memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_US_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_US_CTR)); break;
//     case SCHEDULED_ON_ID:        memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_WB_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_WB_CTR)); break;
//     case SCHEDULED_MANUAL_ON_ID: memcpy_P(buffer, PM_HEATING_SENSOR_NAME_LONG_IH_CTR, sizeof(PM_HEATING_SENSOR_NAME_LONG_IH_CTR)); break;
//   }
//   return buffer;
// }
// #endif //USE_SCHEDULED_HEATING

// const char* mHeating::GetTempModeByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen){
//   // if(buffer == nullptr){ return 0;}
//   switch(program_temps[device].status.mode){
//     default:               snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH); break;
//     case TEMP_MODE_OFF_ID: snprintf_P(buffer, buflen, PM_TEMP_MODE_OFF_CTR); break;
//   }
//   return buffer;
//   //   (program_temps[device].status.mode == TEMP_MODE_HEATING ? "TEMP_MODE_HEATING\0" :
//   //   (program_temps[device].status.mode == TEMP_MODE_MAINTAINING ? "TEMP_MODE_MAINTAINING\0" :
//   //   (program_temps[device].status.mode == TEMP_MODE_SCHEDULED ? "TEMP_MODE_SCHEDULED\0" :
//   //   D_UNKNOWN))));
// }

// const char* mHeating::GetTempActiveProgramByDeviceIDCtr(uint8_t device, char* buffer, uint8_t buflen){
//   // if(buffer == nullptr){ return 0;}
//   // return 0;
//   // return 
//   //   (program_temps[device].status.mode == TEMP_MODE_OFF ? "TEMP_MODE_OFF\0" :
//   //   (program_temps[device].status.mode == TEMP_MODE_HEATING ? "TEMP_MODE_HEATING\0" :
//   //   (program_temps[device].status.mode == TEMP_MODE_MAINTAINING ? "TEMP_MODE_MAINTAINING\0" :
//   //   (program_temps[device].status.mode == TEMP_MODE_SCHEDULED ? "TEMP_MODE_SCHEDULED\0" :
//   //   D_UNKNOWN))));
  
//     snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH);
//     return buffer;
// }


// int8_t mHeating::Tasker(uint8_t function){

//   /************
//    * INIT SECTION * 
//   *******************/
//   if(function == FUNC_PRE_INIT){
//     pre_init();
//   }else
//   if(function == FUNC_INIT){
//     init();
//   }

//   // Only continue in to tasker if module was configured properly
//   if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

//   switch(function){
//     /************
//      * SETTINGS SECTION * 
//     *******************/
//     case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE: 
//       // Settings_Load();
//     break;
//     case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE: 
//       // Settings_Save();
//     break;
//     case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:
//       // Settings_Default();
//     break;
//     case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:
//       // Settings_Default();
//       // pCONT_set->SettingsSave(2);
//     break;
//     /************
//      * PERIODIC SECTION * 
//     *******************/
//    case FUNC_EVERY_SECOND:
//    //temp calculate always, later add flags to update it
//     SubContructCtr_HardwareStatus();
//    break;
//     case FUNC_EVERY_MINUTE:     

//     break;
//     /************
//      * FUNCTION HANDLER SECTION * 
//     *******************/
//     case FUNC_FUNCTION_LAMBDA_INIT:
//       FunctionHandler_Init();
//     break;
//     case FUNC_FUNCTION_LAMBDA_LOOP:       //calls from here, into support requiring no object
//       FunctionHandler_Loop();
//     break;
//     /************
//      * COMMANDS SECTION * 
//     *******************/
//     case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
//       CheckAndExecute_JSONCommands();
//     break;
//     case FUNC_JSON_COMMAND_ID:
//       parse_JSONCommand();
//     break;
//     /************
//      * MQTT SECTION * 
//     *******************/
//     case FUNC_MQTT_HANDLERS_INIT:
//     case FUNC_MQTT_HANDLERS_RESET:
//       MQTTHandler_Init(); // Reset to the initial parameters
//     break;
//     case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
//       MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
//     break;
//     case FUNC_MQTT_SENDER:
//       MQTTHandler_Sender(); //optional pass parameter
//     break;
//     case FUNC_MQTT_CONNECTED:
//       MQTTHandler_Set_fSendNow();
//     break;
//   }
  
//   /************
//    * WEBPAGE SECTION * 
//   *******************/
  
//   #ifdef USE_MODULE_NETWORK_WEBSERVER
//   return Tasker_Web(function);
//   #endif // USE_MODULE_NETWORK_WEBSERVER

// } // END Tasker



// int8_t mHeating::CheckAndExecute_JSONCommands(){

//   // Check if instruction is for me
//   if(mSupport::SetTopicMatch(data_buffer.topic.ctr,D_MODULE_CONTROLLER_HEATING_FRIENDLY_CTR)>=0){
//     #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     AddLog_P(LOG_LEVEL_COMMANDS, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND D_MODULE_CONTROLLER_HEATING_FRIENDLY_CTR));
//     #endif // #ifdef ENABLE_LOG_LEVEL_COMMANDS
//     pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//     parse_JSONCommand();
//     return FUNCTION_RESULT_HANDLED_ID;
//   }else{
//     return FUNCTION_RESULT_UNKNOWN_ID; // not meant for here
//   }

// }


// void mHeating::parse_JSONCommand(void){

//   // Need to parse on a copy
//   char parsing_buffer[data_buffer.payload.len+1];
//   memcpy(parsing_buffer,data_buffer.payload.ctr,sizeof(char)*data_buffer.payload.len+1);
//   JsonParser parser(parsing_buffer);
//   JsonParserObject obj = parser.getRootObject();   
//   if (!obj) { 
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog_P(LOG_LEVEL_ERROR, PSTR("DeserializationError with \"%s\""),parsing_buffer);
//     #endif// ENABLE_LOG_LEVEL_INFO
//     return;
//   }  
//   JsonParserToken jtok = 0; 
//   int8_t tmp_id = 0;
//   char buffer[50];

//   /***
//    * Manual
//    * */

//   int8_t device_id = 0; // default to 0, incase D_JSON_HEATING_DEVICE is not found
//   int8_t schedule_id = 0;
//   uint8_t timeon,tempset;
  
//   if(jtok = obj[D_JSON_HEATING_DEVICE]){ 
//     if(jtok.isStr()){
//       if((device_id = GetDeviceIDbyName(jtok.getStr()))>=0){ // D_JSON_DEVICE
//         AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_HEATING_DEVICE)),device_id);
//       }
//     }else
//     if(jtok.isNum()){
//       device_id = jtok.getInt();
//     }
//   }

//   /**
//    *  parsesub_ProgramTimers
//    * */

//   if(jtok = obj[D_JSON_TIME_ON]){ 
//     program_timers[device_id].time_minutes_on = jtok.getInt();
//     program_timers[device_id].time_minutes_on_start = jtok.getInt();
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),program_timers[device_id].time_minutes_on);
//     functionhandler_programs_timers.flags.run_now = true;
//     mqtthandler_program_timers_ifchanged.flags.SendNow = true;
//     mqtthandler_program_timers_teleperiod.flags.SendNow = true;
//     isanychanged_timers = true;
//     data_buffer.isserviced++;
//   }

//   if(jtok = obj[D_JSON_TIME_RUNNING].getObject()[D_JSON_LIMIT]){ 
//     program_temps[device_id].time_running.limit = jtok.getInt();
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TIME_RUNNING,D_JSON_LIMIT)),program_temps[device_id].time_running.limit);
//     fForceHeatingTempUpdate = true;
//     data_buffer.isserviced++;
//   }

// // // #ifdef USE_HEATING_TEMPS
// // //   // TEMP RELATED
// // //   if(jtok = obj[D_JSON_TEMP][D_JSON_SET].isNull()){ 
// // //     program_temps[device_id].temp.desired = obj[D_JSON_TEMP][D_JSON_SET];
// // //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_SET,program_temps[device_id].temp.desired);
// // //     //Response_mP(S_JSON_COMMAND_NVALUE, D_SET,program_temps[device_id].temp.desired);
// // //     fForceHeatingTempUpdate = true;
// // //     data_buffer.isserviced++;
// // //   }

// // //   // if(jtok = obj[D_JSON_SCHEDULE][D_JSON_MODE].isNull()){ 
// // //   //   program_temps[device_id].schedule.mode_sch = GetScheduleModeIDByCtr(obj[D_JSON_SCHEDULE][D_JSON_MODE]);
// // //   //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
// // //   //   //Response_mP(S_JSON_COMMAND_SVALUE, D_SET,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
// // //   //   fForceHeatingTempsUpdate = true;
// // //   //   data_buffer.isserviced++;

// // //   //   if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID){
// // //   //     program_temps[device_id].time_running.on = 0;
// // //   //   }else if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID){
// // //   //     program_temps[device_id].time_running.on = -1;
// // //   //     program_temps[device_id].time_maintaining.on = -1;
// // //   //     SetHeater(device_id,0);
// // //   //   }
// // //   // }
// // //   // if(jtok = obj[D_JSON_SCHEDULE][D_JSON_ONTIME].isNull()){
// // //   //   datetime_t ontime = pCONT_time->GetTimefromCtr(obj["schedule"]["ontime"]);
// // //   //   program_temps[device_id].schedule.ontime = ontime;

// // //   //   //pCONT->mso->MessagePrint("ontime>>\t "); 
// // //   //   //pCONT_time->PrintDateTime(&program_temps[device_id].schedule.ontime);
// // //   //   //pCONT->mso->MessagePrintln(" ");
// // //   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.hour);pCONT->mso->MessagePrint(":");
// // //   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.minute);pCONT->mso->MessagePrint(":");
// // //   //   // pCONT->mso->MessagePrintln(program_temps[device_id].schedule.ontime.second);

// // //   //   // Check if new hour is tomorrow
// // //   //   // if(program_temps[device_id].schedule.ontime.hour<pCONT_time->RtcTime.hour){
// // //   //   //   pCONT_time->AddSecondsToDateTime(program_temps[device_id].schedule.ontime,SEC2DAY);
// // //   //   // }

// // //   //   // add off time based on max limit possible
// // //   //    program_temps[device_id].schedule.offtime = program_temps[device_id].schedule.ontime; // Copy everything
// // //   //    pCONT_time->AddSecondsToDateTime(&program_temps[device_id].schedule.offtime,SEC2MIN*program_temps[device_id].time_running.limit);

// // //   //   // pCONT->mso->MessagePrint("offtime>>\t "); pCONT_time->PrintDateTime(&program_temps[device_id].schedule.offtime);
// // //   //   //pCONT->mso->MessagePrintln(" ");
// // //   //   //pCONT->mso->MessagePrint("RtcTime>>\t "); pCONT_time->PrintDateTime(&pCONT_time->RtcTime);

// // //   //    // splash result (add new ctr for this message)
// // //   //    program_temps[device_id].schedule.mode_sch = SCHEDULED_SET_ID;
// // //   //    program_temps[device_id].schedule.fHasRun = 0;
// // //   //    program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;
// // //   //    program_temps[device_id].status.mode_next = program_temps[device_id].status.mode;
// // //   //    program_temps[device_id].status.mode = TEMP_MODE_SPLASH_RUN_TIME;

// // //   //    program_temps[device_id].status.tSplashTime = millis();
// // //   //    program_temps[device_id].status.msSplashtime = 5000; // 2 seconds

// // //   //   fForceHeatingTempsUpdate = true;
// // //   //   fForceMQTTUpdate = true;


// // //   //   data_buffer.isserviced++;
// // //   // }
// // //   #endif

// //   mqtthandler_program_timers_ifchanged.flags.SendNow = true;
// //   mqtthandler_program_temps_ifchanged.flags.SendNow = true;

// //   //    temp mesaure, send everything
// //   // MQTTHandler_Set_fSendNow();




// // // void mHeating::parsesub_ProgramTemps(JsonObjectConst obj){

// //   int8_t device_id,schedule_id;
// //   uint8_t timeon,tempset;

// //   if((device_id = GetDeviceIDbyName(obj[D_JSON_DEVICE]))>=0){
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
// //     //Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
// //   }else{
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_NOMATCH));
// //     //Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
// //     return; // Unknown device, can't execute
// //   }

// // #ifdef USE_HEATING_TEMPS
// //   // TEMP RELATED
// //   if(jtok = obj[D_JSON_TEMP][D_JSON_SET].isNull()){ 
// //     program_temps[device_id].temp.desired = obj[D_JSON_TEMP][D_JSON_SET];
// //     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_SET,program_temps[device_id].temp.desired);
// //     //Response_mP(S_JSON_COMMAND_NVALUE, D_SET,program_temps[device_id].temp.desired);
// //     fForceHeatingTempUpdate = true;
// //     data_buffer.isserviced++;
// //   }

// //   // if(jtok = obj[D_JSON_SCHEDULE][D_JSON_MODE].isNull()){ 
// //   //   program_temps[device_id].schedule.mode_sch = GetScheduleModeIDByCtr(obj[D_JSON_SCHEDULE][D_JSON_MODE]);
// //   //   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
// //   //   //Response_mP(S_JSON_COMMAND_SVALUE, D_SET,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
// //   //   fForceHeatingTempsUpdate = true;

// //   //   if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID){
// //   //     program_temps[device_id].time_running.on = 0;
// //   //   }else if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID){
// //   //     program_temps[device_id].time_running.on = -1;
// //   //     program_temps[device_id].time_maintaining.on = -1;
// //   //     SetHeater(device_id,0);
// //   //   }
// //   // }
// //   // if(jtok = obj[D_JSON_SCHEDULE][D_JSON_ONTIME].isNull()){
// //   //   datetime_t ontime = pCONT_time->GetTimefromCtr(obj["schedule"]["ontime"]);
// //   //   program_temps[device_id].schedule.ontime = ontime;

// //   //   //pCONT->mso->MessagePrint("ontime>>\t "); 
// //   //   //pCONT_time->PrintDateTime(&program_temps[device_id].schedule.ontime);
// //   //   //pCONT->mso->MessagePrintln(" ");
// //   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.hour);pCONT->mso->MessagePrint(":");
// //   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.minute);pCONT->mso->MessagePrint(":");
// //   //   // pCONT->mso->MessagePrintln(program_temps[device_id].schedule.ontime.second);

// //   //   // Check if new hour is tomorrow
// //   //   // if(program_temps[device_id].schedule.ontime.hour<pCONT_time->RtcTime.hour){
// //   //   //   pCONT_time->AddSecondsToDateTime(program_temps[device_id].schedule.ontime,SEC2DAY);
// //   //   // }

// //   //   // add off time based on max limit possible
// //   //    program_temps[device_id].schedule.offtime = program_temps[device_id].schedule.ontime; // Copy everything
// //   //    pCONT_time->AddSecondsToDateTime(&program_temps[device_id].schedule.offtime,SEC2MIN*program_temps[device_id].time_running.limit);

// //   //   // pCONT->mso->MessagePrint("offtime>>\t "); pCONT_time->PrintDateTime(&program_temps[device_id].schedule.offtime);
// //   //   //pCONT->mso->MessagePrintln(" ");
// //   //   //pCONT->mso->MessagePrint("RtcTime>>\t "); pCONT_time->PrintDateTime(&pCONT_time->RtcTime);

// //   //    // splash result (add new ctr for this message)
// //   //    program_temps[device_id].schedule.mode_sch = SCHEDULED_SET_ID;
// //   //    program_temps[device_id].schedule.fHasRun = 0;
// //   //    program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;
// //   //    program_temps[device_id].status.mode_next = program_temps[device_id].status.mode;
// //   //    program_temps[device_id].status.mode = TEMP_MODE_SPLASH_RUN_TIME;

// //   //    program_temps[device_id].status.tSplashTime = millis();
// //   //    program_temps[device_id].status.msSplashtime = 5000; // 2 seconds

// //   //   fForceHeatingTempsUpdate = true;
// //   //   fForceMQTTUpdate = true;

// //   // }
// //   #endif

// //   mqtthandler_program_temps_ifchanged.flags.SendNow = true;

// //   //    temp mesaure, send everything
// //   // MQTTHandler_Set_fSendNow();

  

// }






// void mHeating::FunctionHandler_Init(){
  
//   functionhandler_ptr = &functionhandler_status_message;
//   functionhandler_ptr->saved_millis = millis();
//   functionhandler_ptr->flags.periodic_enabled = true;
//   functionhandler_ptr->flags.run_now = true;
//   functionhandler_ptr->flags.run_always = false; //on loop
//   functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
//   functionhandler_ptr->time_val = 1;
//   functionhandler_ptr->function = &mHeating::FunctionHandler_Program_Status;
  
//   functionhandler_ptr = &functionhandler_failsafe;
//   functionhandler_ptr->saved_millis = millis();
//   functionhandler_ptr->flags.periodic_enabled = true;
//   functionhandler_ptr->flags.run_now = true;
//   functionhandler_ptr->flags.run_always = false; //on loop
//   functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
//   functionhandler_ptr->time_val = 1;
//   functionhandler_ptr->function = &mHeating::FunctionHandler_FailSafe;

//   #ifdef USE_HEATING_PROFILE_ESTIMATION  
//   functionhandler_ptr = &functionhandler_heating_profiles;
//   functionhandler_ptr->saved_millis = millis();
//   functionhandler_ptr->flags.periodic_enabled = true;
//   functionhandler_ptr->flags.run_now = true;
//   functionhandler_ptr->flags.run_always = false; //on loop
//   functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
//   functionhandler_ptr->time_val = 1;
//   functionhandler_ptr->function = &mHeating::FunctionHandler_Heating_Profiles;
//   #endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
      
//   functionhandler_ptr = &functionhandler_relay_status;
//   functionhandler_ptr->saved_millis = millis();
//   functionhandler_ptr->flags.periodic_enabled = true;
//   functionhandler_ptr->flags.run_now = true;
//   functionhandler_ptr->flags.run_always = false; //on loop
//   functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
//   functionhandler_ptr->time_val = 1;
//   functionhandler_ptr->function = &mHeating::FunctionHandler_Relay_Status;

//   functionhandler_ptr = &functionhandler_programs_timers;
//   functionhandler_ptr->saved_millis = millis();
//   functionhandler_ptr->flags.periodic_enabled = true;
//   functionhandler_ptr->flags.run_now = true;
//   functionhandler_ptr->flags.run_always = false; //on loop
//   #ifdef ENABLE_HEATING_DEBUG_TIMES
//   functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
//   #else
//   functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
//   #endif
//   functionhandler_ptr->time_val = 1;
//   functionhandler_ptr->function = &mHeating::FunctionHandler_Programs_Timers;
  
//   functionhandler_ptr = &functionhandler_programs_temps;
//   functionhandler_ptr->saved_millis = millis();
//   functionhandler_ptr->flags.periodic_enabled = true;
//   functionhandler_ptr->flags.run_now = true;
//   functionhandler_ptr->flags.run_always = true; //on loop
//   functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MS_ID; 
//   functionhandler_ptr->time_val = 1;
//   functionhandler_ptr->function = &mHeating::FunctionHandler_Programs_Temps;
    
// }


// void mHeating::FunctionHandler_Loop(){

//   struct functionhandler<mHeating>* functionhandler_list_ptr[] = {
//     &functionhandler_failsafe,
//     &functionhandler_status_message,
//     #ifdef USE_HEATING_PROFILE_ESTIMATION
//     &functionhandler_heating_profiles,
//     #endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
//     &functionhandler_relay_status,
//     &functionhandler_programs_timers,
//     &functionhandler_programs_temps
//   };

//   for(uint8_t list_id=0;list_id<sizeof(functionhandler_list_ptr)/sizeof(functionhandler_list_ptr[0]);list_id++){
//     pCONT_sup->FunctionHandler_Call(*this,D_MODULE_CONTROLLER_HEATING_ID,functionhandler_list_ptr[list_id]); 
//   }   
  
// }


// /**
//  * Generate Messages for users to glance at via web or mqtt, timers, temps, schedules set? append the messages.
//  * */
// void mHeating::FunctionHandler_Program_Status(){

//   memset(&heating_status,0,sizeof(heating_status));

//   for(uint8_t device_id=0;device_id<4;device_id++){
    
//     //Timer program active
//     // if(program_timers[device_id].time_minutes_on){
//       heating_status[device_id].message_len += 
//         sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"Timers %d mins",program_timers[device_id].time_minutes_on>=0?program_timers[device_id].time_minutes_on:0);
//     // }
//     // Temps
//     // if(program_temps[device_id].time_running.on){
//       heating_status[device_id].message_len += 
//         sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%sTemps %d mins",heating_status[device_id].message_ctr>0?", ":"",program_temps[device_id].time_running.on>=0?program_temps[device_id].time_running.on:0);
//     // }

//     if(!heating_status[device_id].message_len){
//       heating_status[device_id].message_len += sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%s","Nothing Set");
//     }

//   }

// }




// void mHeating::MQTTHandler_Init(){

//   struct handler<mHeating>* mqtthandler_ptr;

//   //create "sendnow" (using handler) with passing construct level?

//   mqtthandler_ptr = &mqtthandler_settings_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60*60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_Settings;
  
//   mqtthandler_ptr = &mqtthandler_program_timers_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTimers;
  
//   mqtthandler_ptr = &mqtthandler_program_timers_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTimers;

//   mqtthandler_ptr = &mqtthandler_program_temps_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 1; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTemps;
  
//   mqtthandler_ptr = &mqtthandler_program_temps_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramTemps;

//   mqtthandler_ptr = &mqtthandler_program_overview_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 1; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramActive;
  
//   mqtthandler_ptr = &mqtthandler_program_overview_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ProgramActive;
  
//   mqtthandler_ptr = &mqtthandler_relays_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_HeatingRelays;

//   mqtthandler_ptr = &mqtthandler_relays_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_HeatingRelays;
  
//   mqtthandler_ptr = &mqtthandler_sensor_pipes_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 1; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTemps;

//   mqtthandler_ptr = &mqtthandler_sensor_pipes_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTemps;

//   mqtthandler_ptr = &mqtthandler_sensor_pipes_roc1m;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTempsROC1m;
  
//   mqtthandler_ptr = &mqtthandler_sensor_pipes_roc10m;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60*10; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_PipeTempsROC10m;

//   mqtthandler_ptr = &mqtthandler_sensor_climate_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTemps;
  
//   mqtthandler_ptr = &mqtthandler_sensor_climate_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTemps;

//   mqtthandler_ptr = &mqtthandler_sensor_climate_roc1m;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTempsROC1m;
  
//   mqtthandler_ptr = &mqtthandler_sensor_climate_roc10m;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60*10; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructJSON_ClimateTempsROC10m;

//   mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_ifchanged;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 10; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructSON_PipeTempsByColours;
  
//   mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_teleperiod;
//   mqtthandler_ptr->tSavedLastSent = millis();
//   mqtthandler_ptr->flags.PeriodicEnabled = true;
//   mqtthandler_ptr->flags.SendNow = true;
//   mqtthandler_ptr->tRateSecs = 60; 
//   mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
//   mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;
//   mqtthandler_ptr->ConstructJSON_function = &mHeating::ConstructSON_PipeTempsByColours;

//   // Measure and report every second
//   pCONT_msdb18->settings.rate_measure_ms = 1000;
//   // pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = 5;

//   //Reduce sensor lib from reporting to minimum
//   pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
//   pCONT_msdb18->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
//   pCONT_msdb18->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

//   //Reduce sensor lib from reporting to minimum
//   pCONT_dht->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
//   pCONT_dht->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
//   pCONT_dht->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

  

// }


// void mHeating::MQTTHandler_Set_fSendNow(){

//   mqtthandler_settings_teleperiod.flags.SendNow = true;
//   mqtthandler_program_timers_ifchanged.flags.SendNow = true;
//   mqtthandler_program_timers_teleperiod.flags.SendNow = true;
//   mqtthandler_program_temps_ifchanged.flags.SendNow = true;
//   mqtthandler_program_temps_teleperiod.flags.SendNow = true;
//   mqtthandler_program_overview_ifchanged.flags.SendNow = true;
//   mqtthandler_program_overview_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_pipes_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_pipes_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_pipes_roc1m.flags.SendNow = true;
//   mqtthandler_sensor_pipes_roc10m.flags.SendNow = true;
//   mqtthandler_sensor_climate_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_climate_teleperiod.flags.SendNow = true;
//   mqtthandler_sensor_climate_roc1m.flags.SendNow = true;
//   mqtthandler_sensor_climate_roc10m.flags.SendNow = true;
//   mqtthandler_sensor_pipes_colours_ifchanged.flags.SendNow = true;
//   mqtthandler_sensor_pipes_colours_teleperiod.flags.SendNow = true;

// }


// void mHeating::MQTTHandler_Set_TelePeriod(){

//   mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_program_timers_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_program_temps_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_program_overview_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_sensor_pipes_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_sensor_climate_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//   mqtthandler_sensor_pipes_colours_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

// }


// void mHeating::MQTTHandler_Sender(uint8_t mqtt_handler_id){

//   uint8_t mqtthandler_list_ids[] = {
//     MQTT_HANDLER_SETTINGS_ID, 
//     MQTT_HANDLER_MODULE_PROGRAM_TIMERS_IFCHANGED_ID, 
//     MQTT_HANDLER_MODULE_PROGRAM_TIMERS_TELEPERIOD_ID,
//     MQTT_HANDLER_MODULE_PROGRAM_TEMPS_IFCHANGED_ID, 
//     MQTT_HANDLER_MODULE_PROGRAM_TEMPS_TELEPERIOD_ID, 
//     MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_IFCHANGED_ID,
//     MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_TELEPERIOD_ID, 
//     MQTT_HANDLER_MODULE_SENSOR_PIPES_IFCHANGED_ID, 
//     MQTT_HANDLER_MODULE_SENSOR_PIPES_TELEPERIOD_ID,
//     MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC1M_ID,    
//     MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC10M_ID,
//     MQTT_HANDLER_MODULE_SENSOR_CLIMATE_IFCHANGED_ID,
//     MQTT_HANDLER_MODULE_SENSOR_CLIMATE_TELEPERIOD_ID,
//     MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC1M_ID,
//     MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC10M_ID,
//     MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_IFCHANGED_ID,
//     MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_TELEPERIOD_ID,
//     MQTT_HANDLER_MODULE_DRIVERS_RELAYS_IFCHANGED_ID,
//     MQTT_HANDLER_MODULE_DRIVERS_RELAYS_TELEPERIOD_ID
//   };
  
//   struct handler<mHeating>* mqtthandler_list_ptr[] = {
//     &mqtthandler_settings_teleperiod,
//     &mqtthandler_program_timers_ifchanged,
//     &mqtthandler_program_timers_teleperiod,
//     &mqtthandler_program_temps_ifchanged,
//     &mqtthandler_program_temps_teleperiod,
//     &mqtthandler_program_overview_ifchanged,
//     &mqtthandler_program_overview_teleperiod,
//     &mqtthandler_sensor_pipes_ifchanged,
//     &mqtthandler_sensor_pipes_teleperiod,
//     &mqtthandler_sensor_pipes_roc1m,
//     &mqtthandler_sensor_pipes_roc10m,
//     &mqtthandler_sensor_climate_ifchanged,
//     &mqtthandler_sensor_climate_teleperiod,
//     &mqtthandler_sensor_climate_roc1m,
//     &mqtthandler_sensor_climate_roc10m,
//     &mqtthandler_sensor_pipes_colours_ifchanged,
//     &mqtthandler_sensor_pipes_colours_teleperiod,
//     &mqtthandler_relays_ifchanged,
//     &mqtthandler_relays_teleperiod
//   };

//   pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_CONTROLLER_HEATING_ID,
//     mqtthandler_list_ptr, mqtthandler_list_ids,
//     sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
//     mqtt_handler_id
//   );

// }

#endif
