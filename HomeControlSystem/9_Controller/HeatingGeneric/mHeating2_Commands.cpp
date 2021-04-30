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

#ifdef USE_MODULE_CONTROLLER_HEATING2

void mHeating2::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  char buffer[50];
  int8_t device_id = 0;
  
  /**
   * @note device_id for which heating zone is being commanded
   * */
  if(jtok = obj[D_JSON_HEATING_DEVICE]){ 
    if(jtok.isStr()){
      if((device_id = GetDeviceIDbyName(jtok.getStr()))>=0){ // D_JSON_DEVICE
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_HEATING_DEVICE)),device_id);
      }
    }else
    if(jtok.isNum()){
      device_id = jtok.getInt();
    }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_HEATING_DEVICE)),device_id);
  }

  /**
   *  @note Timer Commands
   * */

  if(jtok = obj[D_JSON_TIME_ON]){ 
    CommandSet_ProgramTimer_TimeOn(device_id,jtok.getInt()); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)), jtok.getInt());
    // #endif
  }

  /**
   * Temperature Commands
   * */

  if(jtok = obj[D_JSON_TEMPERATURE].getObject()[D_JSON_SET]){ 
    CommandSet_ProgramTemperature_Desired_Temperature(device_id,jtok.getInt()); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TEMPERATURE D_JSON_SET)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_JSON_SCHEDULE].getObject()[D_JSON_MODE]){ 
    int8_t mode = GetScheduleModeIDByCtr(jtok.getStr());
    CommandSet_ProgramTemperature_Schedule_Mode(device_id,mode); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_SVALUE_K(D_JSON_SCHEDULE D_JSON_MODE)), jtok.getStr());
    // #endif
  }

  if(jtok = obj[D_JSON_TIME_RUNNING].getObject()[D_JSON_LIMIT]){ 
    CommandSet_ProgramTemperature_TimeRunning_Limit(device_id,jtok.getInt()); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TIME_RUNNING,D_JSON_LIMIT)), jtok.getInt()); // The debug prints should always get the jtok value, internal commandset will echo the set value
    // #endif
  }


}


void mHeating2::CommandSet_ProgramTemperature_Desired_Temperature(uint8_t device_id, float value)
{

  program_temps[device_id].temp.desired = value;
  // fForceHeatingTempsUpdate = true;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TEMPERATURE D_JSON_SET)), (int)program_temps[device_id].temp.desired);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


void mHeating2::CommandSet_ProgramTimer_TimeOn(uint8_t device_id, uint8_t value)
{

  program_timers[device_id].time_minutes_on = value;
  program_timers[device_id].time_minutes_on_start = value;
  
  functionhandler_programs_timers.flags.run_now = true;
  mqtthandler_program_timers_ifchanged.flags.SendNow = true;
  mqtthandler_program_timers_teleperiod.flags.SendNow = true;
  isanychanged_timers = true;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)), program_timers[device_id].time_minutes_on);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}



void mHeating2::CommandSet_ProgramTemperature_TimeRunning_Limit(uint8_t device_id, uint8_t value)
{

    program_temps[device_id].time_running.limit = value;
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TIME_RUNNING,D_JSON_LIMIT)),program_temps[device_id].time_running.limit);
  #endif
    // fForceHeatingTempsUpdate = true;
    data_buffer.isserviced++;
}




void mHeating2::CommandSet_ProgramTemperature_Schedule_Mode(uint8_t device_id, int8_t value)
{

  program_temps[device_id].schedule.mode_sch = value;
  // fForceHeatingTempsUpdate = true;

  AddLog(LOG_LEVEL_TEST, PSTR("mode_sch=%d"),program_temps[device_id].schedule.mode_sch);
  
  if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID)
  {
    program_temps[device_id].time_running.on = 0;
  }else 
  if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID)
  {
    program_temps[device_id].time_running.on = -1;
    program_temps[device_id].time_maintaining.on = -1;
    SetHeater(device_id,0);
  }
  
  AddLog(LOG_LEVEL_TEST, PSTR("mode_sch=%d %d %d %d"),
  device_id,
  program_temps[device_id].time_running.on,
  program_temps[device_id].time_maintaining.on,
  program_temps[device_id].schedule.mode_sch);

    //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TEMPERATURE D_JSON_SET)), (int)value);
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


#endif
