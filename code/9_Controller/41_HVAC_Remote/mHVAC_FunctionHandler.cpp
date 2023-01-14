/*
  mHVAC.cpp - HVAC Controller

  Copyright (C) 2021  Michael Doone

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

  OilHeater will become its own Active testbed
  HeatingTestbed will be its own selfcontained nonactive replicate of the house heating (ie same sensors/pins etc just switch out addresses)

*/
#include "mHVAC.h"

#ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE

void mHVAC::FunctionHandler_Init(){
  
  struct functionhandler<mHVAC>* ptr = nullptr;

  ptr = &functionhandler_status_message;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::FunctionHandler_Program_Status;
  
  ptr = &functionhandler_failsafe;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::FunctionHandler_FailSafe;

  #ifdef USE_HVAC_PROFILE_ESTIMATION  
  ptr = &functionhandler_hvac_profiles;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::FunctionHandler_HVAC_Profiles;
  #endif //#ifdef USE_HVAC_PROFILE_ESTIMATION
      
  ptr = &functionhandler_relay_status;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::FunctionHandler_Relay_Status;

  ptr = &functionhandler_programs_timers;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  #ifdef ENABLE_HVAC_DEBUG_TIMES
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  #else
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  #endif
  ptr->time_val = 1;
  ptr->function = &mHVAC::FunctionHandler_Programs_Timers;
  
  #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  ptr = &functionhandler_programs_temps;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::FunctionHandler_Programs_Temps;
  #endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  
  ptr = &functionhandler_update_sensors;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_MS_ID; 
  ptr->time_val = 1000;
  ptr->function = &mHVAC::FunctionHandler_Update_Sensors;
      
}


void mHVAC::FunctionHandler_Loop()
{  
  for(auto& handle:functionhandler_list){
    pCONT_sup->FunctionHandler_Call(*this, EM_MODULE_CONTROLLER__HVAC__ID, handle);
  }
}

#endif