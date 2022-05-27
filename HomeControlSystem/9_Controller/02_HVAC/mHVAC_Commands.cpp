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

void mHVAC::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  char buffer[50];
  int8_t device_id = 0;
  
  /**
   * @note device_id for which heating zone is being commanded
   * */
  if(jtok = obj[D_JSON_HVAC_DEVICE]){ 
    if(jtok.isStr()){
      if((device_id = DLI->GetDeviceIDbyName(jtok.getStr()))>=0){ // D_JSON_DEVICE
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_HVAC_DEVICE)),device_id);
      }else{
        AddLog(LOG_LEVEL_ERROR, PSTR(D_JSON_HVAC_DEVICE "device_id=%d"), device_id);
      }
    }else
    if(jtok.isNum()){
      device_id = jtok.getInt();
    }
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_HVAC_DEVICE)),device_id);
  }

// ALOG_DBM(PSTR("TESTPOINT"));
// return;

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

  if(jtok = obj[D_JSON_TEMPERATURE].getObject()[D_JSON_MODE]){ 
    CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TEMPERATURE,D_JSON_MODE)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_JSON_TEMPERATURE].getObject()["StartDesired"]){ 
    // CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    zone[device_id].program_temp_method->StartDesiredTemperature(jtok.getFloat());
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TEMPERATURE,D_JSON_MODE)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_JSON_TEMPERATURE].getObject()[D_JSON_TIME_RUNNING].getObject()[D_JSON_LIMIT]){ 
    // CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    zone[device_id].program_temp_method->SetTimer_Running_Limit_Minutes(jtok.getInt());
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TEMPERATURE,D_JSON_MODE)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_JSON_TEMPERATURE].getObject()[D_JSON_TIME_MAINTAINING].getObject()[D_JSON_LIMIT]){ 
    // CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    zone[device_id].program_temp_method->SetTimer_Maintaining_Limit_Minutes(jtok.getInt());
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TEMPERATURE,D_JSON_MODE)), jtok.getInt());
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

  if(jtok = obj["HVACZone"].getObject()["SetSensor"])
  { 
    
    ALOG_DBM(PSTR( DEBUG_INSERT_PAGE_BREAK D_LOG_RELAYS "device_name_ctr1 = %s"),arr.getStr()); 
  
    JsonParserArray array = jtok;
    uint8_t index = 0;
    for(auto& arr:array)
    {

      AddLog(LOG_LEVEL_DEBUG, PSTR( DEBUG_INSERT_PAGE_BREAK D_LOG_RELAYS "device_name_ctr1 = %s"),arr.getStr()); 

      int16_t device_id_found = DLI->GetDeviceIDbyName(arr.getStr());
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "device_id_found = %d"),device_id_found);

      if(device_id_found>=0)
      {      
        if(device_id_found > ARRAY_SIZE(pCONT_set->Settings.device_name_buffer.class_id))
        {
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS "device_id_found>pCONT_set->Settings.device_name_buffer.class_id = %d,%d"),device_id_found,ARRAY_SIZE(pCONT_set->Settings.device_name_buffer.class_id) );
        }

        if(index < HEATING_DEVICE_MAX)
        {
          AddLog(LOG_LEVEL_ERROR, PSTR("pCONT_set->Settings.device_name_buffer.class_id[device_id_found] = %d,%d"),device_id_found,pCONT_set->Settings.device_name_buffer.class_id[device_id_found]);
        
          zone[index].sensor.module_id = pCONT_set->Settings.device_name_buffer.class_id[device_id_found];
          zone[index].sensor.index = pCONT_set->Settings.device_name_buffer.device_id[device_id_found];
        }
        AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RELAYS "module_id,index = %d,%d"),zone[index].sensor.module_id,zone[index].sensor.index);

        index++;
      }

    }

    // #ifdef ENABLE_HVAC_DEBUG_PINS_FOR_TESTING
    // DEBUG_LINE_HERE;
    // delay(5000); // to view if this is working
    // #endif // ENABLE_HVAC_DEBUG_PINS_FOR_TESTING

  }
  



}


void mHVAC::CommandSet_ProgramTemperature_Desired_Temperature(uint8_t zone_id, float value)
{

  zone[zone_id].program_temp_method->SetDesiredTemperature(value);
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TEMPERATURE D_JSON_SET)), (int)zone[zone_id].program_temp_method->GetDesiredTemperature());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


void mHVAC::CommandSet_ProgramTemperature_Mode(uint8_t zone_id, uint8_t value)
{

  zone[zone_id].program_temp_method->SetMode(value);
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TEMPERATURE,D_JSON_MODE)), zone[zone_id].program_temp_method->GetMode());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


void mHVAC::CommandSet_ProgramTimer_TimeOn(uint8_t zone_id, uint8_t value)
{

  // check if zone id is valid
  if(zone_id == -1){ return; }

  zone[zone_id].program_timer_method->StartTimer_Minutes(value);
  
  functionhandler_programs_timers.flags.run_now = true;
  mqtthandler_program_timers_ifchanged.flags.SendNow = true;
  mqtthandler_program_timers_teleperiod.flags.SendNow = true;
  // isanychanged_timers = true;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)), zone[zone_id].program_timer_method->GetTimer_Minutes());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}



void mHVAC::CommandSet_ProgramTemperature_TimeRunning_Limit(uint8_t device_id, uint8_t value)
{

  //   program_temps[device_id].time_running.limit = value;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TIME_RUNNING,D_JSON_LIMIT)),program_temps[device_id].time_running.limit);
  // #endif
  //   // fForceHeatingTempsUpdate = true;
  //   data_buffer.isserviced++;
}




void mHVAC::CommandSet_ProgramTemperature_Schedule_Mode(uint8_t device_id, int8_t value)
{

  // program_temps[device_id].schedule.mode_sch = value;
  // // fForceHeatingTempsUpdate = true;

  // AddLog(LOG_LEVEL_TEST, PSTR("mode_sch=%d"),program_temps[device_id].schedule.mode_sch);
  
  // if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID)
  // {
  //   program_temps[device_id].time_running.on = 0;
  // }else 
  // if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID)
  // {
  //   program_temps[device_id].time_running.on = -1;
  //   program_temps[device_id].time_maintaining.on = -1;
  //   SetHeater(device_id,0);
  // }
  
  // AddLog(LOG_LEVEL_TEST, PSTR("mode_sch=%d %d %d %d"),
  // device_id,
  // program_temps[device_id].time_running.on,
  // program_temps[device_id].time_maintaining.on,
  // program_temps[device_id].schedule.mode_sch);

  //   //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_JSON_COMMAND_NVALUE_K(D_JSON_TEMPERATURE D_JSON_SET)), (int)value);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

}


#endif
