/*
  mHeating2.cpp - Generic Heating Controller

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

const char* mHeating2::PM_MODULE_CONTROLLER_HEATING2_CTR = D_MODULE_CONTROLLER_HEATING2_CTR;
const char* mHeating2::PM_MODULE_CONTROLLER_HEATING2_FRIENDLY_CTR = D_MODULE_CONTROLLER_HEATING2_FRIENDLY_CTR;

int8_t mHeating2::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      pre_init();
    break;
    case FUNC_INIT:
      init();
    break;
  }

  // Only continue in to tasker if module was configured properly
  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  // DEBUGGING_PROCEED_ONLY_AFTER_MILLIS_SHOWS_STABLE(0);

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:
      Every_Second();
    break;
    /************
     * FUNCTION HANDLER SECTION * 
    *******************/
    // case FUNC_FUNCTION_LAMBDA_INIT:
    //   FunctionHandler_Init();
    // break;
    // case FUNC_FUNCTION_LAMBDA_LOOP:       //calls from here, into support requiring no object
    //   FunctionHandler_Loop();
    // break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init(); // Reset to the initial parameters
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod(); // Load teleperiod setting into local handlers
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/
  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker



void mHeating2::Every_Second()
{
      //temp calculate always, later add flags to update it
      SubContructCtr_HardwareStatus();



}






/***
 * Primary entry point to heating: 
 *  - All sub cpp files called from here, tasker in here
 * 
Step 1 - heating water by temps again
Step 2 - room heating ONLY by time, since temps for entire house cant be relied on
Step 3 - Optional special heating controls 
  - "Downstairs -> Warm water"
  - "Downstairs -> Shower"
  - "Downstairs -> Bath"

  Optional water
  - Manual (not linked), Auto Priority (Hot water first, then house), Auto Secondary (Top up hot water after radiators enough for shower, but not bath)
  - Options
    - 30, 40, 50 (as buttons)



 * */


void mHeating2::pre_init(void){

  //init_success = true; // Begins true, anything bad sets to false

}

void mHeating2::init(void){

  for(uint8_t device_id=0;device_id<4;device_id++){ 
    SetHeatingRelay(device_id,RELAY_STATE_OFF); 
  }

  memset(&status_message, 0, sizeof(status_message));

  init_program_timers();
  init_program_temps();
  init_program_scheduling();
  
  init_relay_driver_parameters();
  init_dht22_sensor_parameters();
  init_db18_sensor_parameters();

  #if defined(USE_HEATING_TEMPS) && defined(USE_HEATING_PROFILE_ESTIMATION)
    init_HeatingProfiles();
  #endif

}//end function


void mHeating2::init_relay_driver_parameters(){
  

}


void mHeating2::init_db18_sensor_parameters(){

  // Measure and report every second
  pCONT_msdb18->settings.rate_measure_ms = 1000;
  
  //Reduce sensor lib from reporting to minimum
  pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

}


void mHeating2::init_dht22_sensor_parameters(){

  // pCONT_set->AddDeviceName(D_DHT_NAME_DOWNSTAIRS, D_MODULE_SENSORS_DHT_ID,   ID_DHT_DS);
  // pCONT_set->AddDeviceName(D_DHT_NAME_UPSTAIRS, D_MODULE_SENSORS_DHT_ID,     ID_DHT_US);

  // Measure and report every second
  pCONT_dht->settings.rate_measure_ms = 1000;

  //Reduce sensor lib from reporting to minimum
  pCONT_dht->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT_dht->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT_dht->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

}


/*******************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
********* RELAYS AND FAILSAFES ***************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************/


void mHeating2::SetHeatingRelay(uint8_t device_id, uint8_t state){
  pCONT_mry->CommandSet_Relay_Power(state,device_id); //index offset starts at 1
}


uint8_t mHeating2::GetHeatingRelay(uint8_t device_id){
  return pCONT_mry->CommandGet_Relay_Power(device_id);//index offset starts at 1
}


uint8_t mHeating2::GetAnyHeatingRelay(){
  uint8_t oncount = 0;
  for(uint8_t i = 0;i < 4;i++){
    oncount += GetHeatingRelay(i);
  }
  return oncount;
}


void mHeating2::FunctionHandler_FailSafe(void){

  uint8_t fMessageToSend = false;
  char tmpctr[100] = {0}; 
  char buffer[100];
  //memset(tmpctr,0,sizeof(tmpctr));

  uint16_t time_minutes_on = 0;


  for(int device_id=0;device_id<4;device_id++){

    time_minutes_on = pCONT_mry->relay_status[device_id].time_seconds_on/60;

    if(
      (time_minutes_on>FAILSAFE_MINUTES_WARNING)&&
      (time_minutes_on<FAILSAFE_MINUTES_ERROR)
    ){
      // snprintf(tmpctr, sizeof(tmpctr), "Warning, %s heating has been on for a long time of %d minutes",GetDeviceNameLongbyIDCtr(device_id),heating_device_relays[device_id].time_minutes_on);
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_WARNING "\"%s\""),tmpctr);
      fMessageToSend = true;
    }else 
    if(time_minutes_on>FAILSAFE_MINUTES_ERROR){
      // snprintf(tmpctr, sizeof(tmpctr), "Error, %s heating has been on for too long. Turning off now.",GetDeviceNameLongbyIDCtr(device_id));
      // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_JSON_FAILSAFE D_ERROR "\"%s\""),tmpctr);
      fMessageToSend = true;
    }

  //   // if(fMessageToSend){
  //   //   StaticJsonDocument<300> doc;
  //   //   JsonObject obj = doc.to<JsonObject>();
  //   //   obj[D_JSON_TYPE] = "voice+notification"; // broadcast (public/non bedroom speakers), notification(on phones) //SPECIAL, can contain both
  //   //   obj[D_JSON_MESSAGE] = tmpctr;
  //   //   data_buffer.payload.len = measureJson(obj)+1;
  //   //   serializeJson(doc,data_buffer.payload.ctr);
  //   //   pCONT->mqt->ppublish("status/alert",data_buffer.payload.ctr,false);
  //   // }
  }

} //end fucntion


void mHeating2::FunctionHandler_Relay_Status(){ DEBUG_LINE;

  uint8_t relay_state = 0;
  for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){

    switch(device_id){
      case DEVICE_DS_ID: relay_state = GetHeatingRelay(DEVICE_DS_ID); break;
      case DEVICE_US_ID: relay_state = GetHeatingRelay(DEVICE_US_ID); break;
      case DEVICE_WB_ID: relay_state = GetHeatingRelay(DEVICE_WB_ID); break;
      case DEVICE_IH_ID: relay_state = GetHeatingRelay(DEVICE_IH_ID); break;
    }

    //removed minutes on from here, now check relays

  }
  
  
  for(uint8_t device_id=0;device_id<4;device_id++){
    if(pCONT_mry->CommandGet_Relay_Power(device_id)){
      activeprograms[device_id].relays.state = 1;
    }else{
      activeprograms[device_id].relays.state = 0;
    }
  }

} // END function


// This function detects if the traditional heating system has been activated.
// Methods I can use:
//  - furnace active sensor without my own relays on, which means I need to listen for data from oilfurnace
//  - if temp is above a limit (ie radiators when the pulse on and off) and if +ve ROC exceeds a value
int8_t mHeating2::Tasker_PredictManualHeating(){

//if ROC > X with programs off = on

// /if(pCONT->mhs->watertemps.tank_top.ischanged){
//
//   // test for rising tank temps
//   if(watertemps.tank_top.roc1m.val>0.2){
//     pCONT->mso->MessagePrintln("watertemps.tank_top.roc1m.val>0.2");
//   }
//
// }

}


void mHeating2::FunctionHandler_Programs_Timers(void){

  char buffer[50];
  DEBUG_LINE;

  uint8_t log_level = LOG_LEVEL_INFO;//LOG_LEVEL_DEBUG;
  for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){
    if(program_timers[device_id].time_minutes_on){log_level = LOG_LEVEL_INFO;}
    AddLog(log_level, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "\"%s\" %d min"),GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),program_timers[device_id].time_minutes_on);
  }
  
  uint8_t state_tmp = 0;

  for(int device_id=0;device_id<DEVICE_ID_TOTAL;device_id++){

    if(program_timers[device_id].time_minutes_on == 0){
      program_timers[device_id].time_minutes_on = -1; //to disable it
      program_timers[device_id].time_minutes_on_start = program_timers[device_id].time_minutes_on;
      SetHeater(device_id, RELAY_STATE_OFF); //turn off
      program_timers[device_id].ischanged=true;
      state_tmp = ACTIVEP_OFF; // #1 error needs to be done here
      activeprograms[device_id].timers.state = 0;  
    }else 
    if(program_timers[device_id].time_minutes_on > 0){
      program_timers[device_id].time_minutes_on--;
      SetHeater(device_id, RELAY_STATE_ON); // only needs called once, that or add ifnoton inside this function turn on
      program_timers[device_id].ischanged=true;
      state_tmp = ACTIVEP_ON;
      activeprograms[device_id].timers.state = 1;        
    }else{ // <=1 not active
      activeprograms[device_id].timers.state = 0;  
    }

    // Update state change and run special functions if needed // makes no sense
    // if(activeprograms[device_id].timers.state != state_tmp){ // CAUSES ERROR #1 - duplicate on state
    //   activeprograms[device_id].timers.state = state_tmp;
    //   SendMQTTAlertOnChangeofState(device_id,state_tmp);//device,new state
    // }

    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_HEATING "activeprograms[%d].timers.state = %d"),device_id,
      activeprograms[device_id].timers.state);
    
  } // end for

  isanychanged_timers = 0;
  for(uint8_t device_id=0;device_id<4;device_id++){
    isanychanged_timers += program_timers[device_id].ischanged;
  }

} // end function


#ifdef USE_MODULE_NETWORK_MQTT

// schedule.run_time:hh:mm
// schedule.run_days:M-W-FSS
// schedule.mode
// temp.set
// temp.max_time
// new time values that make sense

/*********************************************************************** JSON Payload Constructors *********************************************************************************************/

uint8_t mHeating2::ConstructJSON_ProgramTimers(uint8_t json_level){

  JsonBuilderI->Start();
  
  char buffer[50];
  for(int ii=0;ii<4;ii++){

    if(CheckAndClearFlag(&program_timers[ii].ischanged) || 
      (json_level > JSON_LEVEL_IFCHANGED)
    ){
      JsonBuilderI->Level_Start(GetDeviceNamebyIDCtr(ii,buffer, sizeof(buffer)));
        JsonBuilderI->Add(D_JSON_TIME_ON, program_timers[ii].time_minutes_on==-1?0:program_timers[ii].time_minutes_on); // -1 is off, but only send 0 as limit
      JsonBuilderI->Level_End();
    } //END if

  } //END for

  return JsonBuilderI->End();
  
}



// relays{ds,us},timers{ds,us},temps{ds,us} in simple 1 or 0
uint8_t mHeating2::ConstructJSON_ProgramActive(uint8_t json_level){
  JsonBuilderI->Start();
    JsonBuilderI->Level_Start_P(D_JSON_RELAY);   
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, GetHeatingRelay(DEVICE_US_ID));
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, GetHeatingRelay(DEVICE_DS_ID));
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, GetHeatingRelay(DEVICE_IH_ID));
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, GetHeatingRelay(DEVICE_WB_ID));
      JsonBuilderI->Add(D_JSON_ANY,                     GetAnyHeatingRelay());      
    JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start_P(D_JSON_TIMER);  
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].timers.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].timers.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].timers.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].timers.state); 
    JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start_P(D_JSON_TEMPERATURE);  
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].temps.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].temps.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].temps.state); 
      JsonBuilderI->Add(D_HEATING_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].temps.state); 
    JsonBuilderI->Level_End();
    JsonBuilderI->Level_Start_P(D_JSON_STATUS);  
      JsonBuilderI->Add(D_JSON_MESSAGE, status_message.ctr);  //nice human message that remains the same
      JsonBuilderI->Add(D_JSON_LENGTH, status_message.len); 
      JsonBuilderI->Add(D_JSON_IMPORTANCE, status_message.importance); 
    JsonBuilderI->Level_End();

    // JsonBuilderI->Level_Start_P(D_JSON_STATUS "_Short");  
    //   JsonBuilderI->Add(D_JSON_MESSAGE, status_message.ctr);  //nice human message that remains the same
    //   JsonBuilderI->Add(D_JSON_LENGTH, status_message.len); 
    //   JsonBuilderI->Add(D_JSON_IMPORTANCE, status_message.importance); 
    // JsonBuilderI->Level_End();

  return JsonBuilderI->End();
}


uint8_t mHeating2::ConstructJSON_HeatingRelays(uint8_t json_level){

  char buffer[50];
  JsonBuilderI->Start();
  for(int device_id=0;device_id<4;device_id++){
    JsonBuilderI->Level_Start_P(pCONT_set->GetDeviceName(EM_MODULE_DRIVERS_RELAY_ID, device_id, buffer, sizeof(buffer)));
      JsonBuilderI->Add_FV(D_JSON_ONTIME, PSTR("\"%02d:%02d:%02d\""),  pCONT_mry->relay_status[device_id].last.ontime.hour,  pCONT_mry->relay_status[device_id].last.ontime.minute,  pCONT_mry->relay_status[device_id].last.ontime.second);
      JsonBuilderI->Add_FV(D_JSON_OFFTIME, PSTR("\"%02d:%02d:%02d\""), pCONT_mry->relay_status[device_id].last.offtime.hour,  pCONT_mry->relay_status[device_id].last.offtime.minute,  pCONT_mry->relay_status[device_id].last.offtime.second);
      JsonBuilderI->Add(D_JSON_TIME_ON "_Seconds",   pCONT_mry->relay_status[device_id].time_seconds_on);
      JsonBuilderI->Add(D_JSON_TIME_ON "_Mins",   pCONT_mry->relay_status[device_id].time_seconds_on/60);
    JsonBuilderI->Level_End();
  }
  return JsonBuilderI->End();

}



// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating2::ConstructJSON_PipeTemps(uint8_t json_level){

  char buffer[50];
  
  JsonBuilderI->Start();
    for(int i=0;i<pCONT_msdb18->db18_sensors_active;i++){ 
      if(pCONT_sup->JsonLevelFlagCheck(JSON_LEVEL_IFCHANGED, json_level, pCONT_msdb18->sensor[i].reading.ischanged)){
        JsonBuilderI->Level_Start_P(pCONT_set->GetDeviceName(EM_MODULE_SENSORS_DB18S20_ID, i, buffer, sizeof(buffer)));
          JsonBuilderI->Add(D_JSON_TEMPERATURE, pCONT_msdb18->sensor[i].reading.val);
          JsonBuilderI->Add(D_JSON_ISVALID, pCONT_msdb18->sensor[i].reading.isvalid);
          JsonBuilderI->Add(D_JSON_SECS, (int)abs(pCONT_time->uptime.seconds_nonreset-pCONT_msdb18->sensor[i].reading.captureupsecs));
        JsonBuilderI->Level_End();
      } 
    }
  return JsonBuilderI->End();

}


// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating2::ConstructJSON_PipeTempsROC10s(uint8_t json_level){

return 0;
  // StaticJsonDocument<700> doc;
  // JsonObject obj = doc.to<JsonObject>();
  // D_DATA_BUFFER_CLEAR();

  // for(uint8_t id=0;id<8;id++){

  //   switch(id){
  //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  //   }

  //   //if(pCONT->mhs->watertemps.ptr->roc10s.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer.payload.json_pairs++;
  //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
  //       json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc10s.val,1);
  //       json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc10s.isvalid;
  //       json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.second-pCONT->mhs->watertemps.ptr->roc10s.captureupsecs);
  //   //}

  // }

}



// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating2::ConstructJSON_PipeTempsROC1m(uint8_t json_level){

return 0;
//   for(uint8_t id=0;id<8;id++){

//     switch(id){
//       case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
//       case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
//       case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
//       case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
//       case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
//       case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
//       case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
//       case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
//     }

// //    pCONT->mhs->watertemps.ptr->roc1m.ischanged=1;
//     //if(pCONT->mhs->watertemps.ptr->roc1m.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer.payload.json_pairs++;
//       JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
//       json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc1m.val,1);
//       json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc1m.isvalid;
//       json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.second-pCONT->mhs->watertemps.ptr->roc1m.captureupsecs);
//     //}

//   }

}



// Update: when temps have changed (add flag for new value), or 60 seconds has elapsed (REQUIRES: retain)
uint8_t mHeating2::ConstructJSON_PipeTempsROC10m(uint8_t json_level){

  return 0;
  // for(uint8_t id=0;id<8;id++){

  //   switch(id){
  //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  //   }
    
  //   //if(pCONT->mhs->watertemps.ptr->roc10m.ischanged||fSendAllData||fSendSingleFunctionData){ //data_buffer.payload.json_pairs++;
  //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(id));
  //     json1[D_JSON_TEMP] = mSupport::roundfloat(pCONT->mhs->watertemps.ptr->roc10m.val,1);
  //     json1[D_JSON_ISVALID] = (int)pCONT->mhs->watertemps.ptr->roc10m.isvalid;
  //     json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.second-pCONT->mhs->watertemps.ptr->roc10m.captureupsecs);
  //  // }

  // }

}



uint8_t mHeating2::ConstructJSON_ClimateTemps(uint8_t json_level){

  uint8_t ischanged=false, isvalid = false;
  char name_buffer_tmp[25];

  JsonBuilderI->Start();

  for(int dht_id=0;dht_id<2;dht_id++){
    JsonBuilderI->Level_Start(pCONT_set->GetDeviceName(EM_MODULE_SENSORS_DHT_ID, dht_id, name_buffer_tmp, sizeof(name_buffer_tmp)));
      JsonBuilderI->Add(D_JSON_TEMPERATURE, pCONT_dht->sensor[dht_id].instant.temperature);
      JsonBuilderI->Add(D_JSON_HUMIDITY, pCONT_dht->sensor[dht_id].instant.humidity);
      JsonBuilderI->Add(D_JSON_ISVALID, pCONT_dht->sensor[dht_id].instant.isvalid);
      JsonBuilderI->Add(D_JSON_ISCHANGED, pCONT_dht->sensor[dht_id].instant.ischanged);
      // json1["iserrored"] = pCONT->mhs->climate.ptr->iserrored;
      // json1[D_JSON_SECS] = (int)abs(pCONT_time->uptime.seconds_nonreset-pCONT->mhs->climate.ptr->raw.captureupsecs);
      // json1["heatindex"] = pCONT->mhs->climate.ptr->raw.heatIndex; // DONT KNOW WHAT THEY ARE
      // json1["dewpoint"] = pCONT->mhs->climate.ptr->raw.dewPoint;
    JsonBuilderI->Level_End();
  }

  JsonBuilderI->End();

}


uint8_t mHeating2::ConstructJSON_ClimateTempsROC1m(uint8_t json_level){

  // for(int ii=0;ii<2;ii++){

  //   switch(ii){
  //     case 0: ischanged = pCONT->mhs->climate.downstairs.roc1m.ischanged; break;
  //     case 1: ischanged = pCONT->mhs->climate.upstairs.roc1m.ischanged; break;
  //   }

  //   //if(ischanged||fSendAllData||fSendSingleFunctionData){
  //     // data_buffer.payload.json_pairs++;
  //     switch(ii){
  //       case 0: pCONT->mhs->climate.ptr = &pCONT->mhs->climate.downstairs; break;
  //       case 1: pCONT->mhs->climate.ptr = &pCONT->mhs->climate.upstairs; break;
  //     }
  //     JsonObject json1 = obj.createNestedObject(GetDeviceNamebyIDCtr(ii));
  //       json1[D_JSON_TEMP] = pCONT->mhs->climate.ptr->roc1m.temperature;
  //       json1[D_JSON_HUM] = pCONT->mhs->climate.ptr->roc1m.humidity;
  //   //}

  // }

  return 0;

}

uint8_t mHeating2::ConstructJSON_ClimateTempsROC10m(uint8_t json_level){

  // char namectr[3]; memset(&namectr,0,sizeof(namectr));
  // uint8_t ischanged=false;

  // StaticJsonDocument<400> doc;
  //   JsonObject obj = doc.to<JsonObject>();

  //  for(int ii=0;ii<2;ii++){

  //    switch(ii){
  //      case 0: ischanged = pCONT->mhs->climate.downstairs.roc10m.ischanged; break;
  //      case 1: ischanged = pCONT->mhs->climate.upstairs.roc10m.ischanged; break;
  //    }

  //   // ischanged=1;
  //  //  if(ischanged||fSendAllData||fSendSingleFunctionData){

  //     //  data_buffer.payload.json_pairs++;

  //       switch(ii){
  //         case 0: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_DS,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.downstairs; break;
  //         case 1: memcpy(namectr,D_HEATING_SENSOR_NAME_SHORT_US,2*sizeof(char)); pCONT->mhs->climate.ptr = &pCONT->mhs->climate.upstairs; break;
  //       }

  //       JsonObject json1 = obj.createNestedObject(namectr);
  //       json1[D_JSON_TEMP] = pCONT->mhs->climate.ptr->roc10m.temperature;
  //       json1[D_JSON_HUM] = pCONT->mhs->climate.ptr->roc10m.humidity;

  //     }

  //  // }
  return 0;

}



uint8_t mHeating2::ConstructJSON_Settings(uint8_t json_method){

  return 0;

}



#endif








// Keeps the highest importance flag
void mHeating2::SetHighestImportance(uint8_t* importanceset, int8_t thisvalue){
  if(thisvalue > *importanceset){
    *importanceset = thisvalue;
  }
  //moved into telemetrym
}


// returns > 0 for events added
int8_t mHeating2::SubContructCtr_HardwareStatus(){

  //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
  uint8_t fNotFirstItem = 0;
  char buffer[100];
  memset(&status_message,0,sizeof(status_message));

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].timers.state){
  //     sprintf(&status_message.ctr[status_message.len],"%s timers %s",
  //       GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state, buffer, sizeof(buffer)));
  //     status_message.len = strlen(status_message.ctr);
  //     SetHighestImportance(&status_message.importance,activeprograms[device_id].timers.state); //med
  //   }
  //   if(fNotFirstItem){
  //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
  //     status_message.len = strlen(status_message.ctr);
  //   }
  // }

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].temps.state){ status_message.importance = 2;//med
  //     sprintf(&status_message.ctr[status_message.len],"%s temps %s",
  //       GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state, buffer, sizeof(buffer)));
  //     status_message.len = strlen(status_message.ctr);
  //   }
  //   SetHighestImportance(&status_message.importance,activeprograms[device_id].temps.state); //med
  //   if(fNotFirstItem){
  //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
  //     status_message.len = strlen(status_message.ctr);
  //   }
  // }

  for(int device_id=0;device_id<4;device_id++){
    if(activeprograms[device_id].relays.state){ status_message.importance = 2;//high
      sprintf(&status_message.ctr[status_message.len],"%s relays %d",
        GetDeviceNamebyIDCtr(device_id, buffer, sizeof(buffer)),
        // GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state, buffer, sizeof(buffer)),
        pCONT_mry->relay_status[device_id].time_seconds_on
      );
      status_message.len = strlen(status_message.ctr);
    }
    SetHighestImportance(&status_message.importance,activeprograms[device_id].relays.state); //med
    if(fNotFirstItem){
      sprintf(&status_message.ctr[status_message.len],"%c",'c');
      status_message.len = strlen(status_message.ctr);
    }
  }

  if(!status_message.len){
    sprintf(&status_message.ctr[status_message.len],"%s","Nothing Running");
    status_message.len = strlen(status_message.ctr);
  }

}

// returns > 0 for events added
int8_t mHeating2::SubContructCtr_HardwareStatus_Long(){

  //ORDER BY LEAST TO MOST IMPORTANT SO HIGHEST FLAG IS SET LAST
  uint8_t fNotFirstItem = 0;

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].timers.state){
  //     sprintf(&status_message.ctr[status_message.len],"%s Timer %s",
  //       GetDeviceNameLongbyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].timers.state));
  //     status_message.len = strlen(status_message.ctr);
  //     SetHighestImportance(&status_message.importance,activeprograms[device_id].timers.state); //med
  //   }
  //   if(fNotFirstItem){
  //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
  //     status_message.len = strlen(status_message.ctr);
  //   }
  // }

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].temps.state){ status_message.importance = 2;//med
  //     sprintf(&status_message.ctr[status_message.len],"%s Temp %s",
  //       GetDeviceNameLongbyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].temps.state));
  //     status_message.len = strlen(status_message.ctr);
  //   }
  //   SetHighestImportance(&status_message.importance,activeprograms[device_id].temps.state); //med
  //   if(fNotFirstItem){
  //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
  //     status_message.len = strlen(status_message.ctr);
  //   }
  // }

  // for(int device_id=0;device_id<4;device_id++){
  //   if(activeprograms[device_id].relays.state){ status_message.importance = 2;//high
  //     sprintf(&status_message.ctr[status_message.len],"%s relays %s",
  //       GetDeviceNameLongbyIDCtr(device_id),
  //       GetActiveProgramNameCtrbyID(activeprograms[device_id].relays.state));
  //     status_message.len = strlen(status_message.ctr);
  //   }
  //   SetHighestImportance(&status_message.importance,activeprograms[device_id].relays.state); //med
  //   if(fNotFirstItem){
  //     sprintf(&status_message.ctr[status_message.len],"%c",'c');
  //     status_message.len = strlen(status_message.ctr);
  //   }
  // }

  // if(!status_message.len){
  //   status_message.len += sprintf(&status_message.ctr[status_message.len],"Online");
  // }

} //end function



int mHeating2::mapHeatingTempToHueColour(int val){
   int val2 = mSupport::mapfloat(val,15.0,50.0,180,0); //float mSupport::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
   if(val2<0){
     if(val2<-60){ //limit so we stay within purple and never looping as far as blue
       val2 = 300;
     }else{
       val2 += 360;
     }
   }
  return val2;
}

int mHeating2::mapHeatingTempToBrightness(int temp){

  int brightness;

  if(temp<25){
    brightness = map(temp,15,25,1,10); //brightness propotional to temp; hotter = brighter
    brightness = constrain(brightness,9,10);
  }else{
    brightness = map(temp,25,50,10,100); //brightness propotional to temp; hotter = brighter
    brightness = constrain(brightness,10,100);
  }

  return brightness;

}




// Send all water sensors by [temp,huebytemp(0-360),brightnessbytemp(0-100)]
// ds:{temp:0,hue:0,bright:0},
// us:{temp:0,hue:0,bright:0},
// EFFECTS to be send seperate when programs change state
uint8_t mHeating2::ConstructSON_PipeTempsByColours(uint8_t json_level){

  // StaticJsonDocument<800> doc;
  // JsonObject obj = doc.to<JsonObject>();
  // D_DATA_BUFFER_CLEAR();

  // uint8_t ischanged = false;

  // for(uint8_t device_id=0;device_id<8;device_id++){

  //   switch(device_id){
  //     case ID_DB18_DS: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.downstairs_pipe; break;
  //     case ID_DB18_US: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.upstairs_pipe; break;
  //     case ID_DB18_WB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.boiler_pipe; break;
  //     case ID_DB18_IH: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.immersion_heater; break;
  //     case ID_DB18_TT: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_top; break;
  //     case ID_DB18_TM: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_middle; break;
  //     case ID_DB18_TB: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_bottom; break;
  //     case ID_DB18_TO: default: pCONT->mhs->watertemps.ptr = &pCONT->mhs->watertemps.tank_out; break;
  //   }

  //   if(stored_new.temp[device_id] != pCONT->mhs->watertemps.ptr->raw.val){
  //     stored_new.temp[device_id] = pCONT->mhs->watertemps.ptr->raw.val;
  //     ischanged = 1;
  //   }
  //   #ifdef DEBUG_SENDALL
  //     ischanged=1; //ALL
  //   #endif
  //   if(ischanged){ ischanged = 0;
  //     //data_buffer.payload.json_pairs++;
  //     JsonObject json1 = obj.createNestedObject(GetSensorNameByID(device_id));
  //     json1[D_JSON_TEMP] = pCONT->mhs->watertemps.ptr->raw.val;
  //     json1[D_JSON_HUE] = mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val);
  //     json1[D_JSON_BRT] = mapHeatingTempToBrightness(pCONT->mhs->watertemps.ptr->raw.val);

  //     char tmpctr[10];  memset(tmpctr,0,sizeof(tmpctr));
  //     RgbColor c = HsbColor(mapHeatingTempToHueColour(pCONT->mhs->watertemps.ptr->raw.val)/360.0f,100/100.0f,100/100.0f);
  //     sprintf(tmpctr,"%02X%02X%02X",c.R,c.G,c.B);
  //     json1[D_JSON_RGB] = tmpctr;

  //     switch(device_id){
  //       case ID_DB18_DS: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_DS_ID); break;
  //       case ID_DB18_US: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_US_ID); break;
  //       case ID_DB18_WB: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_WB_ID); break;
  //       case ID_DB18_IH: json1[D_JSON_EFFECTS] = GetHeatingRelay(DEVICE_IH_ID); break;
  //       default: break; //nothing
  //     }
  //   }

  // }

  // // if(data_buffer.payload.json_pairs>0){
  //   data_buffer.payload.len = measureJson(obj)+1;
  //   serializeJson(doc,data_buffer.payload.ctr);
  //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATINGPANEL D_PAYLOAD " \"%s\""),data_buffer.payload.ctr);
  // // }

  return 0;// data_buffer.payload.len>3?1:0;

}



void mHeating2::SetHeater(uint8_t device, uint8_t state){

  //AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_HEATING D_HEATING_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),program_temps[device_id].time_running.on,program_temps[device_id].time_running.limit);

  // #ifdef ENABLE_MQTT_DEBUG_MESSAGES //debug_message/function 
  // pCONT_mqtt->Send_Prefixed_P(PSTR("debug_message/setheater"),PSTR("device=%d,state=%d"),device,state);
  // #endif

  //add state check that only runs updates below if new state has been selected, else, return
  if(GetHeatingRelay(device)==state){
    return;
  }
  // else{
  //   fForceHeatingTempsUpdate = true;
  // }

  #ifdef ENABLE_RELAY_CONTROLS
    SetHeatingRelay(device,state);
  #else
    AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_RELAY_CONTROLS is disabled"));
    pCONT_mqtt->Send_Prefixed_P("/debug/alert", PSTR("Heating DISABLED for testing! SetHeatingRelay(%d,%d)"), device, state);
  #endif

} // end function
























const char* mHeating2::GetDeviceNamebyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen){
  
  switch(device_id){ 
    case DEVICE_DS_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_DS_CTR); break;
    case DEVICE_US_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_US_CTR); break;
    case DEVICE_WB_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_WB_CTR); break;
    case DEVICE_IH_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_SHORT_IH_CTR); break;
  }
  return buffer;

}


const char* mHeating2::GetDeviceNameLongbyIDCtr(uint8_t device_id, char* buffer, uint8_t buflen){
  
  switch(device_id){ 
    case DEVICE_DS_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_DS_CTR); break;
    case DEVICE_US_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_US_CTR); break;
    case DEVICE_WB_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_WB_CTR); break;
    case DEVICE_IH_ID: snprintf_P(buffer, buflen, PM_HEATING_SENSOR_NAME_LONG_IH_CTR); break;
  }
  return buffer;

}

int8_t mHeating2::GetDeviceIDbyName(const char* c){
  if(c=='\0'){
    return -1;
  }

//like getstate, use dlist for different names for same things
// /strcasecmp_P

  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_DS)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_DS_LOWERCASE)){
    return DEVICE_DS_ID;
  }else 
  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_US)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_US_LOWERCASE)){
    return DEVICE_US_ID;
  }else 
  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_WB)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_WB_LOWERCASE)){
    return DEVICE_WB_ID;
  }else 
  if(strstr(c,D_HEATING_SENSOR_NAME_SHORT_IH)||strstr(c,D_HEATING_SENSOR_NAME_SHORT_IH_LOWERCASE)){
    return DEVICE_IH_ID;
  }else{
    return -1;
  }
}

// #endif //USE_SCHEDULED_HEATING





void mHeating2::FunctionHandler_Init(){
  
  struct functionhandler<mHeating2>* functionhandler_ptr = nullptr;

  functionhandler_ptr = &functionhandler_status_message;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating2::FunctionHandler_Program_Status;
  
  functionhandler_ptr = &functionhandler_failsafe;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating2::FunctionHandler_FailSafe;

  #ifdef USE_HEATING_PROFILE_ESTIMATION  
  functionhandler_ptr = &functionhandler_heating_profiles;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating2::FunctionHandler_Heating_Profiles;
  #endif //#ifdef USE_HEATING_PROFILE_ESTIMATION
      
  functionhandler_ptr = &functionhandler_relay_status;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating2::FunctionHandler_Relay_Status;

  functionhandler_ptr = &functionhandler_programs_timers;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  #ifdef ENABLE_HEATING_DEBUG_TIMES
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  #else
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
  #endif
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating2::FunctionHandler_Programs_Timers;
  
  functionhandler_ptr = &functionhandler_programs_temps;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHeating2::FunctionHandler_Programs_Temps;
    
}


void mHeating2::FunctionHandler_Loop()
{  
  for(auto& handle:functionhandler_list){
    pCONT_sup->FunctionHandler_Call(*this, EM_MODULE_CONTROLLER_HEATING2_ID, handle);
  }
}


/**
 * Generate Messages for users to glance at via web or mqtt, timers, temps, schedules set? append the messages.
 * */
void mHeating2::FunctionHandler_Program_Status(){

  memset(&heating_status,0,sizeof(heating_status));

  for(uint8_t device_id=0;device_id<4;device_id++){
    
    //Timer program active
    // if(program_timers[device_id].time_minutes_on){
      heating_status[device_id].message_len += 
        sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"Timers %d mins",program_timers[device_id].time_minutes_on>=0?program_timers[device_id].time_minutes_on:0);
    // }
    // Temps
    // if(program_temps[device_id].time_running.on){
      heating_status[device_id].message_len += 
        sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%sTemps %d mins",heating_status[device_id].message_ctr>0?", ":"",program_temps[device_id].time_running.on>=0?program_temps[device_id].time_running.on:0);
    // }

    if(!heating_status[device_id].message_len){
      heating_status[device_id].message_len += sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%s","Nothing Set");
    }

  }

}


#ifdef USE_MODULE_NETWORK_MQTT

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHeating2::MQTTHandler_Init(){

  struct handler<mHeating2>* mqtthandler_ptr;

  //create "sendnow" (using handler) with passing construct level?

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_Settings;
  
  mqtthandler_ptr = &mqtthandler_program_timers_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TIMERS_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ProgramTimers;
  
  mqtthandler_ptr = &mqtthandler_program_timers_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TIMERS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ProgramTimers;

  mqtthandler_ptr = &mqtthandler_program_temps_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TEMPS_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ProgramTemps;
  
  mqtthandler_ptr = &mqtthandler_program_temps_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TEMPS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ProgramTemps;

  mqtthandler_ptr = &mqtthandler_program_overview_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ProgramActive;
  
  mqtthandler_ptr = &mqtthandler_program_overview_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ProgramActive;
  
  mqtthandler_ptr = &mqtthandler_relays_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_DRIVERS_RELAYS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_HeatingRelays;

  mqtthandler_ptr = &mqtthandler_relays_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_DRIVERS_RELAYS_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_HeatingRelays;
  
  mqtthandler_ptr = &mqtthandler_sensor_pipes_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_PipeTemps;

  mqtthandler_ptr = &mqtthandler_sensor_pipes_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_PipeTemps;

  mqtthandler_ptr = &mqtthandler_sensor_pipes_roc1m;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC1M_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_PipeTempsROC1m;
  
  mqtthandler_ptr = &mqtthandler_sensor_pipes_roc10m;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_ROC10M_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_PIPES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_PipeTempsROC10m;

  mqtthandler_ptr = &mqtthandler_sensor_climate_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_CLIMATE_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ClimateTemps;
  
  mqtthandler_ptr = &mqtthandler_sensor_climate_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_CLIMATE_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ClimateTemps;

  mqtthandler_ptr = &mqtthandler_sensor_climate_roc1m;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC1M_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ClimateTempsROC1m;
  
  mqtthandler_ptr = &mqtthandler_sensor_climate_roc10m;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_CLIMATE_ROC10M_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_CLIMATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructJSON_ClimateTempsROC10m;

  mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructSON_PipeTempsByColours;
  
  mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHeating2::ConstructSON_PipeTempsByColours;

  // Measure and report every second
  pCONT_msdb18->settings.rate_measure_ms = 1000;
  // pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = 5;

  //Reduce sensor lib from reporting to minimum
  pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT_msdb18->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

  //Reduce sensor lib from reporting to minimum
  pCONT_dht->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  pCONT_dht->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  pCONT_dht->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

  

}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHeating2::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mHeating2::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mHeating2::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER_HEATING2_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif
