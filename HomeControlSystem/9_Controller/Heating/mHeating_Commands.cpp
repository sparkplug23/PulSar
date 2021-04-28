/*
  xdsp_02_ssd1306.ino - Display Oled SSD1306 support for Tasmota

  Copyright (C) 2021  Theo Arends and Adafruit

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

#ifdef USE_MODULE_CONTROLLER_HEATING

void mHeating::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;
  char buffer[50];

  /***
   * Manual
   * */

  int8_t device_id = 0; // default to 0, incase D_JSON_HEATING_DEVICE is not found
  int8_t schedule_id = 0;
  uint8_t timeon,tempset;
  
  if(jtok = obj[D_JSON_HEATING_DEVICE]){ 
    if(jtok.isStr()){
      if((device_id = GetDeviceIDbyName(jtok.getStr()))>=0){ // D_JSON_DEVICE
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_HEATING_DEVICE)),device_id);
      }
    }else
    if(jtok.isNum()){
      device_id = jtok.getInt();
    }
  }

  /**
   *  parsesub_ProgramTimers
   * */

  if(jtok = obj[D_JSON_TIME_ON]){ 
    program_timers[device_id].time_minutes_on = jtok.getInt();
    program_timers[device_id].time_minutes_on_start = jtok.getInt();
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_JSON_TIME_ON)),program_timers[device_id].time_minutes_on);
    functionhandler_programs_timers.flags.run_now = true;
    mqtthandler_program_timers_ifchanged.flags.SendNow = true;
    mqtthandler_program_timers_teleperiod.flags.SendNow = true;
    isanychanged_timers = true;
    data_buffer.isserviced++;
  }

  if(jtok = obj[D_JSON_TIME_RUNNING].getObject()[D_JSON_LIMIT]){ 
    program_temps[device_id].time_running.limit = jtok.getInt();
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE_NVALUE_K(D_JSON_TIME_RUNNING,D_JSON_LIMIT)),program_temps[device_id].time_running.limit);
    fForceHeatingTempsUpdate = true;
    data_buffer.isserviced++;
  }

  if(jtok = obj[D_JSON_TEMPERATURE].getObject()[D_JSON_SET]){ 
    program_temps[device_id].temp.desired = jtok.getInt();
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE_K(D_SET)), program_temps[device_id].temp.desired);
    fForceHeatingTempsUpdate = true;
    data_buffer.isserviced++;
  }

  if(jtok = obj[D_JSON_SCHEDULE].getObject()[D_JSON_MODE]){ 
    program_temps[device_id].schedule.mode_sch = GetScheduleModeIDByCtr(jtok.getStr());
    //AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_SVALUE),D_JSON_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
    fForceHeatingTempsUpdate = true;
    data_buffer.isserviced++;

    if(program_temps[device_id].schedule.mode_sch == SCHEDULED_MANUAL_ON_ID){
      program_temps[device_id].time_running.on = 0;
    }else if(program_temps[device_id].schedule.mode_sch == SCHEDULED_OFF_ID){
      program_temps[device_id].time_running.on = -1;
      program_temps[device_id].time_maintaining.on = -1;
      SetHeater(device_id,0);
    }
  }
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


// //   //   data_buffer.isserviced++;
// //   // }
// //   #endif

//   mqtthandler_program_timers_ifchanged.flags.SendNow = true;
//   mqtthandler_program_temps_ifchanged.flags.SendNow = true;

//   //    temp mesaure, send everything
//   // MQTTHandler_Set_fSendNow();




// // void mHeating::parsesub_ProgramTemps(JsonObjectConst obj){

//   int8_t device_id,schedule_id;
//   uint8_t timeon,tempset;

//   if((device_id = GetDeviceIDbyName(obj[D_JSON_DEVICE]))>=0){
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_MATCHED D_JSON_COMMAND_NVALUE),D_DEVICE,device_id);
//     //Response_mP(S_JSON_COMMAND_NVALUE, D_DEVICE,device_id);
//   }else{
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING D_PARSING_NOMATCH));
//     //Response_mP(S_JSON_COMMAND_SVALUE, D_DEVICE,D_PARSING_NOMATCH);
//     return; // Unknown device, can't execute
//   }


//   // if(jtok = obj[D_JSON_SCHEDULE][D_JSON_ONTIME].isNull()){
//   //   datetime_t ontime = pCONT_time->GetTimefromCtr(obj["schedule"]["ontime"]);
//   //   program_temps[device_id].schedule.ontime = ontime;

//   //   //pCONT->mso->MessagePrint("ontime>>\t "); 
//   //   //pCONT_time->PrintDateTime(&program_temps[device_id].schedule.ontime);
//   //   //pCONT->mso->MessagePrintln(" ");
//   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.hour);pCONT->mso->MessagePrint(":");
//   //   // pCONT->mso->MessagePrint(program_temps[device_id].schedule.ontime.minute);pCONT->mso->MessagePrint(":");
//   //   // pCONT->mso->MessagePrintln(program_temps[device_id].schedule.ontime.second);

//   //   // Check if new hour is tomorrow
//   //   // if(program_temps[device_id].schedule.ontime.hour<pCONT_time->RtcTime.hour){
//   //   //   pCONT_time->AddSecondsToDateTime(program_temps[device_id].schedule.ontime,SEC2DAY);
//   //   // }

//   //   // add off time based on max limit possible
//   //    program_temps[device_id].schedule.offtime = program_temps[device_id].schedule.ontime; // Copy everything
//   //    pCONT_time->AddSecondsToDateTime(&program_temps[device_id].schedule.offtime,SEC2MIN*program_temps[device_id].time_running.limit);

//   //   // pCONT->mso->MessagePrint("offtime>>\t "); pCONT_time->PrintDateTime(&program_temps[device_id].schedule.offtime);
//   //   //pCONT->mso->MessagePrintln(" ");
//   //   //pCONT->mso->MessagePrint("RtcTime>>\t "); pCONT_time->PrintDateTime(&pCONT_time->RtcTime);

//   //    // splash result (add new ctr for this message)
//   //    program_temps[device_id].schedule.mode_sch = SCHEDULED_SET_ID;
//   //    program_temps[device_id].schedule.fHasRun = 0;
//   //    program_temps[device_id].status.mode = TEMP_MODE_SCHEDULED;
//   //    program_temps[device_id].status.mode_next = program_temps[device_id].status.mode;
//   //    program_temps[device_id].status.mode = TEMP_MODE_SPLASH_RUN_TIME;

//   //    program_temps[device_id].status.tSplashTime = millis();
//   //    program_temps[device_id].status.msSplashtime = 5000; // 2 seconds

//   //   fForceHeatingTempsUpdate = true;
//   //   fForceMQTTUpdate = true;

//   // }
//   #endif

//   mqtthandler_program_temps_ifchanged.flags.SendNow = true;

//   //    temp mesaure, send everything
//   // MQTTHandler_Set_fSendNow();

  

}


#endif
