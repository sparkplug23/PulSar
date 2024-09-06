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

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

void mHVAC::init_program_timers(void)
{

  // program_timers[DEVICE_US_ID].mapped_defaults[0] = 0;
  // program_timers[DEVICE_US_ID].mapped_defaults[1] = 60;
  // program_timers[DEVICE_US_ID].mapped_defaults[2] = 90;
  // program_timers[DEVICE_US_ID].mapped_defaults[3] = 120;
  
  // program_timers[DEVICE_DS_ID].mapped_defaults[0] = 0;
  // program_timers[DEVICE_DS_ID].mapped_defaults[1] = 60;
  // program_timers[DEVICE_DS_ID].mapped_defaults[2] = 90;
  // program_timers[DEVICE_DS_ID].mapped_defaults[3] = 120;
  
  // program_timers[DEVICE_IH_ID].mapped_defaults[0] = 0;
  // program_timers[DEVICE_IH_ID].mapped_defaults[1] = 30;
  // program_timers[DEVICE_IH_ID].mapped_defaults[2] = 60;
  // program_timers[DEVICE_IH_ID].mapped_defaults[3] = 120;
  
  // program_timers[DEVICE_WB_ID].mapped_defaults[0] = 0;
  // program_timers[DEVICE_WB_ID].mapped_defaults[1] = 30;
  // program_timers[DEVICE_WB_ID].mapped_defaults[2] = 60;
  // program_timers[DEVICE_WB_ID].mapped_defaults[3] = 120;

}//end function


/**
 * @brief Function called using function handler method
 * @note Runs every 1 second
 * @note This will update the `ProgramTimer` and if required switch the relay output
 * */
void mHVAC::FunctionHandler_Programs_Timers(void){

  for(uint8_t zone_id=0; zone_id<settings.active_zones; zone_id++)
  {

    // Servicing program timers every second
    zone[zone_id].program_timer_method->EverySecond();

    //  React if timer has started or ended
    if(zone[zone_id].program_timer_method->IsChangedThenReset())
    {
      ALOG_DBM( PSTR("program_timer_method IsChangedThenReset %d"), zone_id);
      if(zone[zone_id].program_timer_method->OutputDesiredState())
      {
        SetZoneActive(zone_id, 1); // This can be changed to "TASK_SET_POWER" for internal relay driver control
      }
      else
      {
        SetZoneActive(zone_id, 0);
      }
    }

  } 

  
  // Update mqtt faster if active  
  for(uint8_t zone_id=0; zone_id<settings.active_zones; zone_id++)
  {
    if(zone[zone_id].program_timer_method->IsRunning())
    {
      mqtthandler_program_timers_ifchanged.tRateSecs = 1;
      break; // Stop if any are true
    }
    else
    {
      mqtthandler_program_timers_ifchanged.tRateSecs = 10;
    }
  }


  

} // end function

#endif
