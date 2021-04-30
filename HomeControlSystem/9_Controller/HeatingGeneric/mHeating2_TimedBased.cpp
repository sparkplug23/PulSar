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
#include "mHeating2.h"

/***
 * Time Based: 
 *  - All counters that enable timer (minutes on) based heating controls
 * */


#ifdef USE_MODULE_CONTROLLER_HEATING2

// void mHeating2::pre_init(void){

//   //init_success = true; // Begins true, anything bad sets to false

// }

void mHeating2::init_program_timers(void){

  for(int device_id=0;device_id<HEATING_DEVICE_MAX;device_id++){
    program_timers[device_id].time_minutes_on = -1;
    program_timers[device_id].time_minutes_on_start = program_timers[device_id].time_minutes_on;
    program_timers[device_id].ischanged=false;
  }

  program_timers[DEVICE_US_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_US_ID].mapped_defaults[1] = 60;
  program_timers[DEVICE_US_ID].mapped_defaults[2] = 90;
  program_timers[DEVICE_US_ID].mapped_defaults[3] = 120;
  
  program_timers[DEVICE_DS_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_DS_ID].mapped_defaults[1] = 60;
  program_timers[DEVICE_DS_ID].mapped_defaults[2] = 90;
  program_timers[DEVICE_DS_ID].mapped_defaults[3] = 120;
  
  program_timers[DEVICE_IH_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_IH_ID].mapped_defaults[1] = 30;
  program_timers[DEVICE_IH_ID].mapped_defaults[2] = 60;
  program_timers[DEVICE_IH_ID].mapped_defaults[3] = 120;
  
  program_timers[DEVICE_WB_ID].mapped_defaults[0] = 0;
  program_timers[DEVICE_WB_ID].mapped_defaults[1] = 30;
  program_timers[DEVICE_WB_ID].mapped_defaults[2] = 60;
  program_timers[DEVICE_WB_ID].mapped_defaults[3] = 120;

}//end function




#endif
