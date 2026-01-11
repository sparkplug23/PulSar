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

/***
 * Metrics: 
 *  - Using rate-of-change to calculate heating times
 *  - Calculate estimated heating times 
 * */




#ifdef USE_MODULE_CONTROLLER_HVAC


/******************************************************************************************************************
 * SECTION: Tasker
*******************************************************************************************************************/


int8_t mHVAC::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * YTASK SCHEDULED * 
    *******************/
    case YTASK_INIT:
      YTask_Init();
    break;
    case YTASK_LOOP:
      YTask_Loop();
    break;
    /************
     * STORAGE SECTION *
    *******************/  
    #ifdef USE_MODULE_CORE_FILESYSTEM
    #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC
    case TASK_FILESYSTEM__SAVE__MODULE_DATA__ID:
      Save_Module();
    break;
    case TASK_FILESYSTEM__LOAD__MODULE_DATA__ID:
      // Load_Module();
    break;
    #endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC
    #endif // USE_MODULE_CORE_FILESYSTEM
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }  
  return 1;

} // END Tasker


#ifdef USE_MODULE_CORE_FILESYSTEM
#ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC
void mHVAC::Save_Module()
{

  ALOG_INF(PSTR("mHVAC::Save_Module()"));

  ALOG_INF(PSTR("%d Save_Module relayA %d"), 0, rt.zone[0].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d Save_Module  %d"), 1, rt.zone[1].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d Save_Module  %d"), 2, rt.zone[2].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d Save_Module  %d"), 3, rt.zone[3].program_timer_method.GetTimer_Seconds());


  tkr_mfile->ByteFile_Save("/hvac" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));

}
void mHVAC::Load_Module(bool erase)
{

  ALOG_INF(PSTR("mHVAC::Load_Module()"  DEBUG_INSERT_PAGE_BREAK));

  uint16_t expected_filesize = sizeof(rt);

  //pass pointer into start of read buffer IF matched length

  
  ALOG_INF(PSTR("===================================mHVAC::Load_Module() %d"), expected_filesize);

  ALOG_INF(PSTR("%d relayA %d"), 0, rt.zone[0].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d relay %d"), 1, rt.zone[1].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d relay %d"), 2, rt.zone[2].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d relay %d"), 3, rt.zone[3].program_timer_method.GetTimer_Seconds());

  tkr_mfile->ByteFile_Load("/hvac" FILE_EXTENSION_BIN, (uint8_t*)&rt, sizeof(rt));

  ALOG_INF(PSTR("%d Load_Module %d"), 0, rt.zone[0].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d Load_Module %d"), 1, rt.zone[1].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d Load_Module %d"), 2, rt.zone[2].program_timer_method.GetTimer_Seconds());
  ALOG_INF(PSTR("%d Load_Module %d"), 3, rt.zone[3].program_timer_method.GetTimer_Seconds());
}
#endif // ENABLE_DEVFEATURE_STORAGE__SAVE_MODULE__CONTROLLERS___HVAC
#endif // USE_MODULE_CORE_FILESYSTEM




/******************************************************************************************************************
 * SECTION: Scheduler: New method that takes the main "Tasker" entry point and instead performs the tasks required by this module in more detail
*******************************************************************************************************************/

void mHVAC::YTask_Init(){
  
  struct functionhandler<mHVAC>* ptr;

  ptr = &functionhandler_status_message;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::YTask_Program_Status;
  functionhandler_list.push_back(ptr);
  
  #ifdef USE_HVAC_PROFILE_ESTIMATION  
  ptr = &functionhandler_hvac_profiles;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::YTask_HVAC_Profiles;
  functionhandler_list.push_back(ptr);
  #endif //#ifdef USE_HVAC_PROFILE_ESTIMATION
      
  ptr = &functionhandler_relay_status;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::YTask_Relay_Status;
  functionhandler_list.push_back(ptr);

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
  ptr->function = &mHVAC::YTask_Programs_Timers;
  functionhandler_list.push_back(ptr);
  
  #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  ptr = &functionhandler_programs_temps;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  ptr->time_val = 1;
  ptr->function = &mHVAC::YTask_Programs_Temps;
  functionhandler_list.push_back(ptr);
  #endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  
  ptr = &functionhandler_update_sensors;
  ptr->saved_millis = millis();
  ptr->flags.periodic_enabled = true;
  ptr->flags.run_now = true;
  ptr->flags.run_always = false;
  ptr->flags.time_unit = FUNCHANDLER_TIME_MS_ID; 
  ptr->time_val = 1000;
  ptr->function = &mHVAC::YTask_Update_Sensors;
  functionhandler_list.push_back(ptr);
      
}


void mHVAC::YTask_Loop()
{  
  for(auto& handle:functionhandler_list){
    tkr_sup->YTask_Call(*this, GetModuleUniqueID(), handle);
  }
}



/******************************************************************************************************************
 * SECTION: Init
*******************************************************************************************************************/



void mHVAC::Pre_Init(void){

  settings.active_zones = HEATING_DEVICE_MAX;

}

void mHVAC::Init(void)
{

  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    rt.zone[id].active_mode = ZONE_MODE_HEAT_8BITS;
    rt.zone[id].bitpacked_modes_enabled = ZONE_MODE_HEAT_8BITS;// | ZONE_MODE_COOL_8BITS;
  }
  rt.zone[0].bitpacked_modes_enabled = ZONE_MODE_HEAT_8BITS | ZONE_MODE_COOL_8BITS;


  /**
   * Configure relays out TEST PHASE STILL, NOT SET VIA JSON TEMPLATES
   * */
  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    rt.zone[id].output.module_ids.push_back(D_UNIQUE_MODULE_DRIVERS_RELAY_ID);
    rt.zone[id].output.index.push_back(id);
    rt.zone[id].output.driver_type.push_back(DRIVER_TYPE_HVAC_ID);
  }

  #ifdef ENABLE_DEVFEATURE_HVAC_COOLING_ON_ZONE0_WITH_RELAY
  rt.zone[0].output.module_ids.push_back(D_UNIQUE_MODULE_DRIVERS_RELAY_ID);
  rt.zone[0].output.index.push_back(1);
  rt.zone[0].output.driver_type.push_back(DRIVER_TYPE_COOLING_ID);
  #endif
  
  
  for(uint8_t device_id=0;device_id<settings.active_zones;device_id++){ 
    SetHeatingRelay(device_id,RELAY_STATE_OFF); 
  }

  // init_prog/ram_timers();
  // init_program_temps();
  // init_program_scheduling();
  
  // #if defined(USE_HVAC_TEMPS) && defined(USE_HVAC_PROFILE_ESTIMATION)
  //   init_HeatingProfiles();
  // #endif

  module_state.mode = ModuleStatus::Running;

}//end function


/******************************************************************************************************************
 * SECTION: Helpers
*******************************************************************************************************************/



/**
 * @brief Needs phased out or simply renamed along the lines of "SetZoneOutput" to handle both hot and cold
 * 
 * @param device 
 * @param state 
 */
void mHVAC::SetHeater(uint8_t device, uint8_t state){

  // Add state check that only runs updates below if new state has been selected, else, return
  if(GetHeatingRelay(device)==state){
    return;
  }

  #ifdef ENABLE_RELAY_CONTROLS
    SetHeatingRelay(device,state);
  #else
    AddLog(LOG_LEVEL_WARN, PSTR("ENABLE_RELAY_CONTROLS is disabled"));
    tkr_mqtt->Send_Prefixed_P("/debug/alert", PSTR("Heating DISABLED for testing! SetHeatingRelay(%d,%d)"), device, state);
  #endif

} // end function



/**
 * @brief Read from sensors to update HVAC sensor values
 * 
 */
void mHVAC::YTask_Update_Sensors()
{

  for(int zone_id=0; zone_id<settings.active_zones; zone_id++)
  {
    if(rt.zone[zone_id].sensor.module_id != 0) // Only if set
    {
      sensors_reading_t reading; 
      mTaskerInterface* pMod = nullptr; 

      if((pMod = tkr->GetModule(rt.zone[zone_id].sensor.module_id)) != nullptr)
      {
        
        pMod->GetSensorReading(&reading, rt.zone[zone_id].sensor.index);
        
        if(reading.Valid())
        {   
          rt.zone[zone_id].sensor.temperature = reading.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
          rt.zone[zone_id].sensor.humidity    = reading.GetFloat(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
          // Serial.println(rt.zone[zone_id].sensor.temperature);
          // Serial.println(rt.zone[zone_id].sensor.humidity);
        }
      }

    }

  }

}



/***
 * Scheduling: 
 *  - Enable schedules to heat by time or temp as needed
 *  - Timed eg heat for 2 hours in this window
 *  - Temp eg heat to 40 for THIS time, so it heats before hand
 * */


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
  if(strcasecmp(c,D_SCHEDULED_OFF)==0){ return SCHEDULED_OFF_ID; }
  if(strcasecmp(c,D_SCHEDULED_SET)==0){ return SCHEDULED_SET_ID; }
  if(strcasecmp(c,D_SCHEDULED_ON)==0){ return SCHEDULED_ON_ID; }
  if(strcasecmp(c,D_MANUAL_ON)==0){ return SCHEDULED_MANUAL_ON_ID; }
  return -1; // none
}
const char* mHVAC::GetScheduleNameCtrbyID(uint8_t mode, char* buffer, uint8_t buflen){
  switch(mode){
    case SCHEDULED_OFF_ID:       memcpy(buffer, D_SCHEDULED_OFF, sizeof(D_SCHEDULED_OFF)); break;
    case SCHEDULED_SET_ID:       memcpy(buffer, D_SCHEDULED_SET, sizeof(D_SCHEDULED_SET)); break;
    case SCHEDULED_ON_ID:        memcpy(buffer, D_SCHEDULED_ON, sizeof(D_SCHEDULED_ON)); break;
    case SCHEDULED_MANUAL_ON_ID: memcpy(buffer, D_MANUAL_ON, sizeof(D_MANUAL_ON)); break;
  }
  return buffer;
}




/**
 * Generate Messages for users to glance at via web or mqtt, timers, temps, schedules set? append the messages.
 * */
void mHVAC::YTask_Program_Status()
{

  return;
  // memset(&heating_status,0,sizeof(heating_status));

  // for(uint8_t device_id=0;device_id<settings.active_zones;device_id++){
    
  //   //Timer program active
  //   // if(program_timers[device_id].time_minutes_on){
  //     heating_status[device_id].message_len += 
  //       sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"Timers %d mins",program_timers[device_id].time_minutes_on>=0?program_timers[device_id].time_minutes_on:0);
  //   // }
  //   // Temps
  //   // if(program_temps[device_id].time_running.on){
  //     heating_status[device_id].message_len += 
  //       sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%sTemps %d mins",heating_status[device_id].message_ctr>0?", ":"",program_temps[device_id].time_running.on>=0?program_temps[device_id].time_running.on:0);
  //   // }

  //   if(!heating_status[device_id].message_len){
  //     heating_status[device_id].message_len += sprintf(heating_status[device_id].message_ctr+heating_status[device_id].message_len,"%s","Nothing Set");
  //   }

  // }

}


void mHVAC::SetHeatingRelay(uint8_t device_id, uint8_t state){
  tkr_relay->CommandSet_Relay_Power(state,device_id);
}


uint8_t mHVAC::GetHeatingRelay(uint8_t device_id){
  return tkr_relay->CommandGet_Relay_Power(device_id);
}


uint8_t mHVAC::GetAnyHeatingRelay(){
  uint8_t oncount = 0;
  for(uint8_t i = 0;i < settings.active_zones;i++){
    oncount += GetHeatingRelay(i);
  }
  return oncount;
}


void mHVAC::YTask_Relay_Status(){ 

  for(uint8_t device_id=0;device_id<settings.active_zones;device_id++){
    if(tkr_relay->CommandGet_Relay_Power(device_id)){
      // activeprograms[device_id].relays.state = 1;
    }else{
      // activeprograms[device_id].relays.state = 0;
    }
  }

} // END function


/**
 * @brief Setting the zone method as active or off
 * @note  This will turn the zone on/off, but relay on the active_mode type to know whether it should heat or cool the zone
 * @note  This will choose which relay to turn on, if more than one is set in a zone
 * */
void mHVAC::SetZoneActive(uint8_t zone_id, uint8_t state)
{

  switch(rt.zone[zone_id].active_mode)
  {
    default:
    case ZONE_MODE_HEAT_8BITS:{

      // ALOG_INF(PSTR("rt.zone[%d].output.index[0]=%d"),zone_id, rt.zone[zone_id].output.index[0]);

      // Check if mode is permitted
      // if(rt.zone[zone_id].bitpacked_modes_enabled,)
      uint8_t subindex_of_driver = rt.zone[zone_id].output.index[0];
      
      SetHeater(subindex_of_driver, state);
    }
    break;
    case ZONE_MODE_COOL_8BITS:



    break;
    case ZONE_MODE_DISABLED_8BITS:


    break;
  }

}



// This function detects if the traditional heating system has been activated.
// Methods I can use:
//  - furnace active sensor without my own relays on, which means I need to listen for data from oilfurnace
//  - if temp is above a limit (ie radiators when the pulse on and off) and if +ve ROC exceeds a value
int8_t mHVAC::Tasker_PredictManualHeating(){

//if ROC > X with programs off = on

// /if(tkr->mhs->watertemps.tank_top.ischanged){
//
//   // test for rising tank temps
//   if(watertemps.tank_top.roc1m.val>0.2){
//     tkr->mso->MessagePrintln("watertemps.tank_top.roc1m.val>0.2");
//   }
//
// }

}


int mHVAC::mapHeatingTempToHueColour(int val){
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

int mHVAC::mapHeatingTempToBrightness(int temp){

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


/******************************************************************************************************************
 * SECTION: Commands
*******************************************************************************************************************/


void mHVAC::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  char buffer[50];
  int8_t device_id = 0;

  
  
  /**
   * @note device_id for which heating zone is being commanded
   * */
  if(jtok = obj[D_HVAC_DEVICE]){ 
    if(jtok.isStr()){
      if((device_id = DLI->GetDeviceIDbyName(jtok.getStr(), GetModuleUniqueID()))>=0)
      {
        ALOG_INF(PSTR(D_LOG_HEATING D_PARSING_MATCHED D_COMMAND_NVALUE_K(D_HVAC_DEVICE)),device_id);
      }else{
        AddLog(LOG_LEVEL_ERROR, PSTR(D_HVAC_DEVICE "device_id=%d"), device_id);
      }
    }else
    if(jtok.isNum()){
      device_id = jtok.getInt();
    }
    ALOG_INF(PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_HVAC_DEVICE)),device_id);
  }


  /**
   *  @note Timer Commands
   * */
  if(jtok = obj["HVAC"].getObject()[D_TIME_ON]){ 
    CommandSet_ProgramTimer_TimeOn(device_id,jtok.getInt()); 
    ALOG_INF(PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TIME_ON)), jtok.getInt());
    data_buffer.isserviced++;
  }

  if(jtok = obj["HVAC"].getObject()[D_ADD_TIME_ON]){ 
    CommandSet_ProgramTimer_AddTimeOn(device_id, jtok.getInt()); 
    ALOG_INF(PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_ADD_TIME_ON)), jtok.getInt());
    data_buffer.isserviced++;
  }


  /**
   * Temperature Commands
   * */
  if(jtok = obj[D_TEMPERATURE].getObject()[D_SET]){ 
    CommandSet_ProgramTemperature_Desired_Temperature(device_id,jtok.getInt()); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TEMPERATURE D_SET)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_TEMPERATURE].getObject()[D_MODE]){ 
    CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_HEATING D_COMMAND_SVALUE_NVALUE_K(D_TEMPERATURE,D_MODE)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_TEMPERATURE].getObject()["StartDesired"]){ 
    // CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    rt.zone[device_id].program_temp_method.StartDesiredTemperature(jtok.getFloat());
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // ALOG_DBG(PSTR(D_LOG_HEATING D_COMMAND_SVALUE_NVALUE_K(D_TEMPERATURE,D_MODE)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_TEMPERATURE].getObject()[D_TIME_RUNNING].getObject()[D_LIMIT]){ 
    // CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    rt.zone[device_id].program_temp_method.SetTimer_Running_Limit_Minutes(jtok.getInt());
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // ALOG_DBG(PSTR(D_LOG_HEATING D_COMMAND_SVALUE_NVALUE_K(D_TEMPERATURE,D_MODE)), jtok.getInt());
    // #endif
  }

  if(jtok = obj[D_TEMPERATURE].getObject()[D_TIME_MAINTAINING].getObject()[D_LIMIT]){ 
    // CommandSet_ProgramTemperature_Mode(device_id,jtok.getInt()); 
    rt.zone[device_id].program_temp_method.SetTimer_Maintaining_Limit_Minutes(jtok.getInt());
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    // ALOG_DBG(PSTR(D_LOG_HEATING D_COMMAND_SVALUE_NVALUE_K(D_TEMPERATURE,D_MODE)), jtok.getInt());
    // #endif
  }


  if(jtok = obj[D_SCHEDULE].getObject()[D_MODE]){ 
    int8_t mode = GetScheduleModeIDByCtr(jtok.getStr());
    CommandSet_ProgramTemperature_Schedule_Mode(device_id,mode); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_HEATING D_COMMAND_SVALUE_K(D_SCHEDULE D_MODE)), jtok.getStr());
    // #endif
  }

  if(jtok = obj[D_TIME_RUNNING].getObject()[D_LIMIT]){ 
    CommandSet_ProgramTemperature_TimeRunning_Limit(device_id,jtok.getInt()); 
    data_buffer.isserviced++;
    // #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR(D_LOG_HEATING D_COMMAND_SVALUE_NVALUE_K(D_TIME_RUNNING,D_LIMIT)), jtok.getInt()); // The debug prints should always get the jtok value, internal commandset will echo the set value
    // #endif
  }
  

  if(jtok = obj["HVACZone"].getObject()["SetSensor"])
  { 
    
    // ALOG_HGL(PSTR( DEBUG_INSERT_PAGE_BREAK D_LOG_RELAYS "device_name_ctr1 = %s"), arr.getStr()); 
  
    JsonParserArray array = jtok;
    uint8_t index = 0;
    for(auto& arr:array)
    {

      ALOG_HGL( PSTR( DEBUG_INSERT_PAGE_BREAK D_LOG_RELAYS "SetSensor device_name_ctr1 = %s"),arr.getStr()); 

      int16_t device_id_found = DLI->GetDeviceIDbyName(arr.getStr(),GetModuleUniqueID());
      ALOG_DBG(PSTR(D_LOG_RELAYS "device_id_found = %d"),device_id_found);

      if(device_id_found>=0)
      {      
        if(device_id_found > ARRAY_SIZE(tkr_set->Settings.device_name_buffer.class_id))
        {
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_RELAYS "device_id_found>tkr_set->Settings.device_name_buffer.class_id = %d,%d"),device_id_found,ARRAY_SIZE(tkr_set->Settings.device_name_buffer.class_id) );
        }

        if(index < HEATING_DEVICE_MAX)
        {
          AddLog(LOG_LEVEL_ERROR, PSTR("tkr_set->Settings.device_name_buffer.class_id[device_id_found] = %d,%d"),device_id_found,tkr_set->Settings.device_name_buffer.class_id[device_id_found]);
        
          rt.zone[index].sensor.module_id = tkr_set->Settings.device_name_buffer.class_id[device_id_found];
          rt.zone[index].sensor.index = tkr_set->Settings.device_name_buffer.device_id[device_id_found];
        }
        ALOG_DBG(PSTR(D_LOG_RELAYS "module_id,index = %d,%d"),rt.zone[index].sensor.module_id,rt.zone[index].sensor.index);

        index++;
      }

    }


  }
  



}



void mHVAC::CommandSet_ProgramTimer_TimeOn(uint8_t zone_id, uint16_t value)
{

  // check if zone id is valid
  if(zone_id == -1){ return; }

  rt.zone[zone_id].program_timer_method.StartTimer_Minutes(value);
  
  functionhandler_programs_timers.flags.run_now = true;
  mqtthandler_program_timers_ifchanged.flags.SendNow = true;
  mqtthandler_program_timers_teleperiod.flags.SendNow = true;
  // isanychanged_timers = true;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_INF(PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TIME_ON)), rt.zone[zone_id].program_timer_method.GetTimer_Minutes());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

void mHVAC::CommandSet_ProgramTimer_AddTimeOn(uint8_t zone_id, uint16_t value)
{

  // check if zone id is valid
  if(zone_id == -1){ return; }

  rt.zone[zone_id].program_timer_method.StartTimer_Minutes(rt.zone[zone_id].program_timer_method.GetTimer_Minutes()+value);
  
  functionhandler_programs_timers.flags.run_now = true;
  mqtthandler_program_timers_ifchanged.flags.SendNow = true;
  mqtthandler_program_timers_teleperiod.flags.SendNow = true;
  // isanychanged_timers = true;
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
    ALOG_INF(PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TIME_ON)), rt.zone[zone_id].program_timer_method.GetTimer_Minutes());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}

uint16_t mHVAC::CommandGet_ProgramTimer_TimeOn(uint8_t zone_id)
{

  // check if zone id is valid
  // if(zone_id == -1){ return; }

  

  // rt.zone[zone_id].program_timer_method.StartTimer_Minutes(value);
  
  // functionhandler_programs_timers.flags.run_now = true;
  // mqtthandler_program_timers_ifchanged.flags.SendNow = true;
  // mqtthandler_program_timers_teleperiod.flags.SendNow = true;
  // // isanychanged_timers = true;
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   ALOG_INF(PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TIME_ON)), rt.zone[zone_id].program_timer_method.GetTimer_Minutes());
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

}

void mHVAC::CommandSet_ProgramTemperature_Desired_Temperature(uint8_t zone_id, float value)
{

  rt.zone[zone_id].program_temp_method.SetDesiredTemperature(value);
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TEMPERATURE D_SET)), (int)rt.zone[zone_id].program_temp_method.GetDesiredTemperature());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}


void mHVAC::CommandSet_ProgramTemperature_Mode(uint8_t zone_id, uint8_t value)
{

  rt.zone[zone_id].program_temp_method.SetMode(value);
  
  #ifdef ENABLE_LOG_LEVEL_COMMANDS
  AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_COMMAND_SVALUE_NVALUE_K(D_TEMPERATURE,D_MODE)), rt.zone[zone_id].program_temp_method.GetMode());
  #endif // ENABLE_LOG_LEVEL_COMMANDS

}



void mHVAC::CommandSet_ProgramTemperature_TimeRunning_Limit(uint8_t device_id, uint8_t value)
{

  //   program_temps[device_id].time_running.limit = value;
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  //   ALOG_INF(PSTR(D_LOG_HEATING D_PARSING_MATCHED D_COMMAND_SVALUE_NVALUE_K(D_TIME_RUNNING,D_LIMIT)),program_temps[device_id].time_running.limit);
  // #endif
  //   // fForceHeatingTempsUpdate = true;
  //   data_buffer.isserviced++;
}




void mHVAC::CommandSet_ProgramTemperature_Schedule_Mode(uint8_t device_id, int8_t value)
{

  // program_temps[device_id].schedule.mode_sch = value;
  // // fForceHeatingTempsUpdate = true;

  // ALOG_TST(PSTR("mode_sch=%d"),program_temps[device_id].schedule.mode_sch);
  
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
  
  // ALOG_TST(PSTR("mode_sch=%d %d %d %d"),
  // device_id,
  // program_temps[device_id].time_running.on,
  // program_temps[device_id].time_maintaining.on,
  // program_temps[device_id].schedule.mode_sch);

  //   //ALOG_INF(PSTR(D_LOG_HEATING D_PARSING_MATCHED D_COMMAND_SVALUE),D_MODE,GetScheduleNameCtrbyID(program_temps[device_id].schedule.mode_sch));
  
  // #ifdef ENABLE_LOG_LEVEL_COMMANDS
  // // AddLog(LOG_LEVEL_COMMANDS, PSTR(D_LOG_HEATING D_COMMAND_NVALUE_K(D_TEMPERATURE D_SET)), (int)value);
  // #endif // ENABLE_LOG_LEVEL_COMMANDS

}


void ProgramTemperature::EverySecond(void)
{

  #ifdef ENABLE_LOG_LEVEL_DEBUG
    ALOG_DBG(PSTR("ProgramTemperature::EverySecond"));
  #endif

  /**
   * increment any timer variables if active (ie >0 )
   * */
  if(time_running.on >= 0) time_running.on++;
  if(time_maintaining.on >= 0) time_maintaining.on++;

  /**
   * Check (Here I will introduce other heating profiles, ie cycling the maintain period)
   * */
  if(mode_heating_enabled) CheckRunningProgram_Heating_Profile1();
  // if(mode_cooling_enabled) CheckRunningProgram_Cooling();

  // ischanged = true;
  
}

/**
 * @brief This section will
 * */
void ProgramTemperature::CheckRunningProgram_Heating_Profile1()
{

  // if timer is active
  if(time_running.on >= 0){

    /**
     * @brief Time checks - If maximum "time on" has been reached, turn EVERYTHING off
     * @note TimeMaintain includes/overlaps the "on" period
     * */
    if(time_running.on < time_running.limit){     //stay on
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_running = on [%d] < limit [%d]"),time_running.on,time_running.limit);
      isrunning_heating = true;
    }
    else{       
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Time On Exceeded - Turn Zone OFF"));
      time_running.on = -1; //deactviate
      time_maintaining.on = -1; // Turn maintaining period on
      isrunning_heating = false;
    }

    /**
     * @brief Temp checks (Has the current values reached the desired), if so, also continue if maintain is set
     * @note This week need a better method in the future, to "cycle" the maintain period on/off (with different "profiles" available)
     * @note Currently, maintain in the home heating assumes the cycling is handled by a physical real world thermostat
     * */
    if(temperature.current >= temperature.desired){   
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured = Current [%d] > Set [%d]"),(int)temperature.current,(int)temperature.desired);

      /**
       * If maintain was not counting yet, turn it on for first temp crossing above threshold
       * */
      if(time_maintaining.on == -1)
      {
        time_maintaining.on = 0;
      }

      // Check if temp should be maintained above desired point
      if((time_maintaining.on>=0)&&(time_maintaining.on < time_maintaining.limit)){
        ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_maintaining = on [%d] < limit [%d]"),time_maintaining.on,time_maintaining.limit);
        isrunning_heating = true;
      }
      // Turn off
      else{
        ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Time Maintaining Exceededon%d [%d] < limit [%d]"),time_maintaining.on, time_maintaining.on,time_maintaining.limit);
        time_running.on = -2;
        time_maintaining.on = -1;
        isrunning_heating = false;
      }
    }
    else{ 
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured = Current [%d] < Set [%d]"),(int)temperature.current,(int)temperature.desired);
    }

  }// time_on is not running
  else{ 
    ALOG_DBM( PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_on is not running"));
    // isrunning_heating = false;
  }


}

/**
 * @brief This section will
 * */
void ProgramTemperature::CheckRunningProgram_Cooling()
{

  // if timer is active
  if(time_running.on >= 0){

    /**
     *  Time checks - If limit is hit, stop, or else continue maintaining on
     * */
    if(time_running.on > time_running.limit){   
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "minutes_on [%d] > minutes_max [%d]"),time_running.on,time_running.limit);
      time_running.on = -2; //deactviate
      time_maintaining.limit = -1;
      isrunning_cooling = false;
    }else
    { //stay on
      isrunning_cooling = true;
    }

    /**
     * Temp checks (Has the current values reached the desired)
     * */
    if(temperature.current <= temperature.desired){    // Also need to add a "fudge" value
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured Temp [%d] > Set Temp [%d]"),(int)temperature.current,(int)temperature.desired);
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Exceeded Set Point"));

      // Check if temp should be maintained above desired point
      if((time_maintaining.on>=0)&&(time_maintaining.on < time_maintaining.limit)){
        isrunning_cooling = true;
      }
      // Turn off
      else{
        ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Time Maintaining Exceeded"));
        time_running.on = -2;
        time_maintaining.limit = -1;
        isrunning_cooling = false;
      }
    }
    else{ 
      ALOG_DBG(PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "Measured Temp [%d] < Set Temp [%d]"),(int)temperature.current,(int)temperature.desired);
    }

  }// time_on is not running
  else{ 
    ALOG_DBM( PSTR(D_LOG_HEATING D_HVAC_PROGRAM_TEMP "time_on is not running"));
  }


}


/******************************************************************************************************************
 * SECTION: ConstructJson
*******************************************************************************************************************/


uint8_t mHVAC::ConstructJSON_ProgramTimers(uint8_t json_level, bool json_appending){

  char buffer[50];

  JBI->Start();
  
  for(uint8_t zone_id=0; zone_id<settings.active_zones; zone_id++)
  {
    JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),zone_id,buffer,sizeof(buffer)));
      JBI->Add(D_TIME_ON, rt.zone[zone_id].program_timer_method.GetTimer_Minutes() != -1 ? rt.zone[zone_id].program_timer_method.GetTimer_Minutes() : 0); // If "-1" ie disabled, send zero
      JBI->Add(D_TIME_ON_SECS, rt.zone[zone_id].program_timer_method.GetTimer_Seconds());
    JBI->Object_End();
  }

  return JBI->End();
  
}


uint8_t mHVAC::ConstructJSON_ProgramTemps(uint8_t json_level, bool json_appending){
  
  char buffer[50];

  JBI->Start();
  
  #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
  
  for(int zone_id=0; zone_id<settings.active_zones; zone_id++){

    JBI->Level_Start_P( DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(),zone_id,buffer,sizeof(buffer)) );
  
      JBI->Level_Start_P(D_TEMPERATURE);   
        JBI->Add(D_CURRENT, rt.zone[zone_id].program_temp_method.GetCurrentTemperature());
        JBI->Add(D_DESIRED, rt.zone[zone_id].program_temp_method.GetDesiredTemperature());
        JBI->Add(D_ERROR,   rt.zone[zone_id].program_temp_method.GetErrorTemperature());
      JBI->Object_End();
      JBI->Level_Start_P(D_STATUS);   
        // JBI->Add(D_MODE, GetTempModeByDeviceIDCtr(zone_id, buffer, sizeof(buffer)));
        // JBI->Add(D_DATA, program_temps[zone_id].status.data.ctr);
      JBI->Object_End();
      JBI->Level_Start_P(D_TIME_RUNNING); 
        JBI->Add(D_TIME_ON, rt.zone[zone_id].program_temp_method.GetTimer_Running_Minutes() != -1 ? rt.zone[zone_id].program_temp_method.GetTimer_Running_Minutes() : 0);
        JBI->Add(D_TIME_ON_SECS, rt.zone[zone_id].program_temp_method.GetTimer_Running_Seconds());  
        JBI->Add(D_LIMIT, rt.zone[zone_id].program_temp_method.GetTimer_Running_Limit_Minutes());
        JBI->Add(D_LIMIT D_SECS, rt.zone[zone_id].program_temp_method.GetTimer_Maintaining_Limit_Seconds());        
      JBI->Object_End();  
      JBI->Level_Start_P(D_TIME_MAINTAINING); 
        JBI->Add(D_TIME_ON, rt.zone[zone_id].program_temp_method.GetTimer_Maintaining_Minutes() != -1 ? rt.zone[zone_id].program_temp_method.GetTimer_Maintaining_Minutes() : 0);
        JBI->Add(D_TIME_ON_SECS, rt.zone[zone_id].program_temp_method.GetTimer_Maintaining_Seconds());  
        JBI->Add(D_LIMIT, rt.zone[zone_id].program_temp_method.GetTimer_Maintaining_Minutes());
        JBI->Add(D_LIMIT D_SECS, rt.zone[zone_id].program_temp_method.GetTimer_Maintaining_Limit_Seconds());  
      JBI->Object_End();  
      JBI->Level_Start_P(D_TIME_TO_HEAT);
        //  time_to_heatobj[D_SECONDS] = GetHeatingProfilesTimeSeconds(zone_id,program_temps[zone_id].temp.current,program_temps[zone_id].temp.desired);
        //       //   time_to_heatobj[D_MINUTES] = GetHeatingProfilesTimeMinutes(zone_id,program_temps[zone_id].temp.current,program_temps[zone_id].temp.desired);    
      JBI->Object_End();  
      JBI->Level_Start_P(D_SCHEDULE); 
        // JBI->Add(D_MODE, GetScheduleNameCtrbyID(program_temps[zone_id].schedule.mode_sch, buffer, sizeof(buffer)));
        // JBI->Add(D_ISRUNNING, program_temps[zone_id].schedule.fRunning);     
        // char time_ctr[20]; memset(time_ctr,'\0',sizeof(time_ctr));
        // sprintf(time_ctr, "%02d:%02d:%02d",(int)program_temps[zone_id].schedule.ontime.hour,(int)program_temps[zone_id].schedule.ontime.minute,(int)program_temps[zone_id].schedule.ontime.second);
        // scheduleobj[D_ONTIME] = time_ctr;//tkr_time->getFormattedTime();
        // char time_ctr2[20];
        // memset(time_ctr2,'\0',sizeof(time_ctr2));
        // sprintf(time_ctr2, "%02d:%02d:%02d",(int)program_temps[zone_id].schedule.offtime.hour,(int)program_temps[zone_id].schedule.offtime.minute,(int)program_temps[zone_id].schedule.offtime.second);
        // scheduleobj[D_OFFTIME] = time_ctr2;//tkr_time->getFormattedTime();   
      JBI->Object_End();  
    JBI->Object_End();
  }

  #endif // ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES

  return JBI->End();

}


uint8_t mHVAC::ConstructJSON_ProgramActive(uint8_t json_level, bool json_appending){
  JBI->Start();
  // DEBUG_LINE_HERE;
    JBI->Level_Start_P(D_RELAY);   
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_US, GetHeatingRelay(DEVICE_US_ID));
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_DS, GetHeatingRelay(DEVICE_DS_ID));
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_IH, GetHeatingRelay(DEVICE_IH_ID));
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_WB, GetHeatingRelay(DEVICE_WB_ID));
      // JBI->Add(D_ANY,                     GetAnyHeatingRelay());      
    JBI->Object_End();
    JBI->Level_Start_P(D_TIMER);  
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].timers.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].timers.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].timers.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].timers.state); 
    JBI->Object_End();
    JBI->Level_Start_P(D_TEMPERATURE);  
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_US, activeprograms[DEVICE_US_ID].temps.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_DS, activeprograms[DEVICE_DS_ID].temps.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_IH, activeprograms[DEVICE_IH_ID].temps.state); 
      // JBI->Add(D_HVAC_SENSOR_NAME_SHORT_WB, activeprograms[DEVICE_WB_ID].temps.state); 
    JBI->Object_End();
    JBI->Level_Start_P(D_STATUS);  
      // JBI->Add(D_MESSAGE, status_message.ctr);  //nice human message that remains the same
      // JBI->Add(D_LENGTH, status_message.len); 
      // JBI->Add(D_IMPORTANCE, status_message.importance); 
    JBI->Object_End();

    // JBI->Level_Start_P(D_STATUS "_Short");  
    //   JBI->Add(D_MESSAGE, status_message.ctr);  //nice human message that remains the same
    //   JBI->Add(D_LENGTH, status_message.len); 
    //   JBI->Add(D_IMPORTANCE, status_message.importance); 
    // JBI->Object_End();

  return JBI->End();
}


uint8_t mHVAC::ConstructJSON_HeatingRelays(uint8_t json_level, bool json_appending){

  // DEBUG_LINE_HERE;
  char buffer[50];
  JBI->Start();
  for(int device_id=0;device_id<settings.active_zones;device_id++){
    JBI->Level_Start_P(DLI->GetDeviceName_WithModuleUniqueID(tkr_relay->GetModuleUniqueID(), device_id, buffer, sizeof(buffer)));
      JBI->Add_FV(D_ONTIME, PSTR("\"%02d:%02d:%02d\""),  tkr_relay->rt.relay_status[device_id].last.ontime.hour,  tkr_relay->rt.relay_status[device_id].last.ontime.minute,  tkr_relay->rt.relay_status[device_id].last.ontime.second);
      JBI->Add_FV(D_OFFTIME, PSTR("\"%02d:%02d:%02d\""), tkr_relay->rt.relay_status[device_id].last.offtime.hour,  tkr_relay->rt.relay_status[device_id].last.offtime.minute,  tkr_relay->rt.relay_status[device_id].last.offtime.second);
      JBI->Add(D_TIME_ON "_Seconds",   tkr_relay->rt.relay_status[device_id].time_seconds_on);
      JBI->Add(D_TIME_ON "_Mins",   tkr_relay->rt.relay_status[device_id].time_seconds_on/60);
    JBI->Object_End();
  }
  return JBI->End();

}


uint8_t mHVAC::ConstructJSON_ZoneSensors(uint8_t json_level, bool json_appending){

  // DEBUG_LINE_HERE;
  char buffer[25];

  JBI->Start();

  for(int zone_id=0;zone_id<settings.active_zones;zone_id++){
    JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( GetModuleUniqueID(), zone_id, buffer, sizeof(buffer)));
      // if(rt.zone[zone_id].sensor.temperature){ JBI->Add(D_TEMPERATURE, rt.zone[zone_id].sensor.temperature); }
      // if(rt.zone[zone_id].sensor.humidity)   { JBI->Add(D_HUMIDITY, rt.zone[zone_id].sensor.humidity);       }
      JBI->Add("ModuleID",rt.zone[zone_id].sensor.module_id);
      JBI->Add("Index",rt.zone[zone_id].sensor.index);
    JBI->Object_End();
  }
  return JBI->End();

  // DEBUG_LINE_HERE;
}


uint8_t mHVAC::ConstructJSON_ZoneSensors_ROC1m(uint8_t json_level, bool json_appending){

  JBI->Start();
  // DEBUG_LINE_HERE;
  return JBI->End();

}


uint8_t mHVAC::ConstructJSON_ZoneSensors_ROC10m(uint8_t json_level, bool json_appending){

  JBI->Start();
  // DEBUG_LINE_HERE;
  return JBI->End();
}


uint8_t mHVAC::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  // return 0;
  // DEBUG_LINE_HERE;
  // DEBUG_LINE_HERE;
  // DEBUG_LINE_HERE;
  // 

// Serial.printf("settings.active_zones=%d", settings.active_zones); Serial.flush();


  JBI->Start();
    JBI->Add("active_zones", settings.active_zones);

    JBI->Object_Start("Zone");
    
        // JBI->Object_Start("BitPacked_Modes_Enabled");
        //   for(int8_t zone_id=0;zone_id<settings.active_zones;zone_id++)
        //   {
        //     JBI->Array_Start_P(PSTR("Zone%d"),zone_id);
        //       for(int8_t bits=0; bits<8; bits++)
        //       {
        //         JBI->Add(bitRead(rt.zone[zone_id].bitpacked_modes_enabled,bits));
        //       }
        //     JBI->Array_End();
        //   }         
        // JBI->Object_End();



      JBI->Object_Start("Driver");

        JBI->Array_Start("ModuleID");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(rt.zone[i].output.module_ids[0]);
        }
        JBI->Array_End();

        JBI->Array_Start("Index");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(rt.zone[i].output.index[0]);
        }
        JBI->Array_End();
      
      JBI->Object_End();
      
      
      JBI->Object_Start("Sensor");

        JBI->Array_Start("ModuleID");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(rt.zone[i].sensor.module_id);
        }
        JBI->Array_End();

        JBI->Array_Start("Index");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(rt.zone[i].sensor.index);
        }
        JBI->Array_End();
        
        JBI->Array_Start("Temperature");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(rt.zone[i].sensor.temperature);
        }
        JBI->Array_End();
        
        JBI->Array_Start("Humidity");
        for(int8_t i=0; i<settings.active_zones; i++){
          JBI->Add(rt.zone[i].sensor.humidity);
        }
        JBI->Array_End();
      
      JBI->Object_End();

    JBI->Object_End();

      // DEBUG_LINE_HERE;
  return JBI->End();


}


/******************************************************************************************************************
 * SECTION: MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHVAC::MQTTHandler_Init(){

  struct handler<mHVAC>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_program_timers_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTimers;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_program_timers_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTimers;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_program_temps_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTemps;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_program_temps_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTemps;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_program_overview_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramActive;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_program_overview_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramActive;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_relays_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_HeatingRelays;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_relays_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_HeatingRelays;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_zone_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_zone_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_zone_roc1m;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors_ROC1m;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_sensor_zone_roc10m;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 60*10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors_ROC10m;
  mqtthandler_list.push_back(ptr);

}

#endif // USE_MODULE_NETWORK_MQTT


#endif
