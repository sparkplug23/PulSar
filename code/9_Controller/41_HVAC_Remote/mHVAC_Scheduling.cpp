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

/***
 * Scheduling: 
 *  - Enable schedules to heat by time or temp as needed
 *  - Timed eg heat for 2 hours in this window
 *  - Temp eg heat to 40 for THIS time, so it heats before hand
 * */


#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

// void mHVAC::Pre_Init(void){

//   //init_success = true; // Begins true, anything bad sets to false

// }

void mHVAC::init_program_scheduling(void){

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



const char* mHVAC::GetActiveProgramNameCtrbyID(uint8_t activeprogram_id, char* buffer, uint8_t buflen){
 
  // switch(activeprogram_id){
  //   default:          snprintf_P(buffer, buflen, PM_SEARCH_NOMATCH); break;
  //   case ACTIVEP_OFF: snprintf(buffer, buflen, "ACTIVEP_OFF"); break;
  //   case ACTIVEP_ON: snprintf(buffer, buflen, "ACTIVEP_ON"); break;
  //   case ACTIVEP_SCHEDULED: snprintf(buffer, buflen, "ACTIVEP_SCHEDULED"); break;
  // }
  return buffer;

}

// #ifdef USE_SCHEDULED_HEATING
int8_t mHVAC::GetScheduleModeIDByCtr(const char* c){
  if(*c=='\0'){ return -1; }
  if(strcasecmp(c,D_JSON_SCHEDULED_OFF)==0){ return SCHEDULED_OFF_ID; }
  if(strcasecmp(c,D_JSON_SCHEDULED_SET)==0){ return SCHEDULED_SET_ID; }
  if(strcasecmp(c,D_JSON_SCHEDULED_ON)==0){ return SCHEDULED_ON_ID; }
  if(strcasecmp(c,D_JSON_MANUAL_ON)==0){ return SCHEDULED_MANUAL_ON_ID; }
  return -1; // none
}
const char* mHVAC::GetScheduleNameCtrbyID(uint8_t mode, char* buffer, uint8_t buflen){
  switch(mode){
    case SCHEDULED_OFF_ID:       memcpy(buffer, D_JSON_SCHEDULED_OFF, sizeof(D_JSON_SCHEDULED_OFF)); break;
    case SCHEDULED_SET_ID:       memcpy(buffer, D_JSON_SCHEDULED_SET, sizeof(D_JSON_SCHEDULED_SET)); break;
    case SCHEDULED_ON_ID:        memcpy(buffer, D_JSON_SCHEDULED_ON, sizeof(D_JSON_SCHEDULED_ON)); break;
    case SCHEDULED_MANUAL_ON_ID: memcpy(buffer, D_JSON_MANUAL_ON, sizeof(D_JSON_MANUAL_ON)); break;
  }
  return buffer;
}


#endif
