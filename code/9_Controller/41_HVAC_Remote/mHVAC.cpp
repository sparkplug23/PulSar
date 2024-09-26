/*
  mHVAC.cpp - HVAC Controller

  To replace physical wire and finally fix heating

  A second ESP32 will be configured to remotely sens and then report back to the primary HVAC unit

  Commands to turn on the immersion will still come via the primary_hvac, but must still include the time off part 
  Sensors will be measured and report back to the primary hvac via mqtt with new subset channel

  To develop this, I will use the desksensor as primary hvac and have a second device as remote
  ** "desksensor_slave_01" will report back to "desksensor"


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
      init();
    break;
  }

  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:
      Every_Second();
    break;
    /************
     * FUNCTION HANDLER SECTION * 
    *******************/
    case TASK_FUNCTION_LAMBDA_INIT:
      FunctionHandler_Init();
    break;
    case TASK_FUNCTION_LAMBDA_LOOP:
      FunctionHandler_Loop();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
  }
  
  /************
   * WEBPAGE SECTION * 
  *******************/  
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_NETWORK_WEBSERVER

} // END Tasker



void mHVAC::Every_Second()
{
// Handled 

// DIGITAL_INVERT_PIN(12);


}


void mHVAC::Pre_Init(void){

  settings.active_zones = HEATING_DEVICE_MAX;

}

void mHVAC::init(void)
{

  // pinMode(12, OUTPUT);

  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    // Create program methods
    #ifdef ENABLE_DEVFEATURE_CONTROLLER_HVAC_PROGRAM_TEMPERATURES
    zone[id].program_temp_method = new ProgramTemperature();
    #endif
    zone[id].program_timer_method = new ProgramTimer();
  }
  

  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    zone[id].active_mode = ZONE_MODE_HEAT_8BITS;
    zone[id].bitpacked_modes_enabled = ZONE_MODE_HEAT_8BITS;// | ZONE_MODE_COOL_8BITS;
  }
  zone[0].bitpacked_modes_enabled = ZONE_MODE_HEAT_8BITS | ZONE_MODE_COOL_8BITS;


  /**
   * Configure relays out TEST PHASE STILL, NOT SET VIA JSON TEMPLATES
   * */
  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    zone[id].output.module_ids.push_back(D_UNIQUE_MODULE_DRIVERS_RELAY_ID);
    zone[id].output.index.push_back(id);
    zone[id].output.driver_type.push_back(DRIVER_TYPE_HVAC_ID);
  }

  #ifdef ENABLE_DEVFEATURE_HVAC_COOLING_ON_ZONE0_WITH_RELAY
  zone[0].output.module_ids.push_back(D_UNIQUE_MODULE_DRIVERS_RELAY_ID);
  zone[0].output.index.push_back(1);
  zone[0].output.driver_type.push_back(DRIVER_TYPE_COOLING_ID);
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
  
  settings.fEnableModule = true;

}//end function



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
    pCONT_mqtt->Send_Prefixed_P("/debug/alert", PSTR("Heating DISABLED for testing! SetHeatingRelay(%d,%d)"), device, state);
  #endif

} // end function







/**
 * Generate Messages for users to glance at via web or mqtt, timers, temps, schedules set? append the messages.
 * */
void mHVAC::FunctionHandler_Program_Status(){

return;
  memset(&heating_status,0,sizeof(heating_status));

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
  pCONT_mry->CommandSet_Relay_Power(state,device_id);
}


uint8_t mHVAC::GetHeatingRelay(uint8_t device_id){
  return pCONT_mry->CommandGet_Relay_Power(device_id);
}


uint8_t mHVAC::GetAnyHeatingRelay(){
  uint8_t oncount = 0;
  for(uint8_t i = 0;i < settings.active_zones;i++){
    oncount += GetHeatingRelay(i);
  }
  return oncount;
}


void mHVAC::FunctionHandler_Relay_Status(){ 

  for(uint8_t device_id=0;device_id<settings.active_zones;device_id++){
    if(pCONT_mry->CommandGet_Relay_Power(device_id)){
      // activeprograms[device_id].relays.state = 1;
    }else{
      // activeprograms[device_id].relays.state = 0;
    }
  }

} // END function


// Keeps the highest importance flag
void mHVAC::SetHighestImportance(uint8_t* importanceset, int8_t thisvalue){
  if(thisvalue > *importanceset){
    *importanceset = thisvalue;
  }
  //moved into telemetrym
}





/**
 * @brief Setting the zone method as active or off
 * @note  This will turn the zone on/off, but relay on the active_mode type to know whether it should heat or cool the zone
 * @note  This will choose which relay to turn on, if more than one is set in a zone
 * */
void mHVAC::SetZoneActive(uint8_t zone_id, uint8_t state)
{

  switch(zone[zone_id].active_mode)
  {
    default:
    case ZONE_MODE_HEAT_8BITS:{

      // ALOG_INF(PSTR("zone[%d].output.index[0]=%d"),zone_id, zone[zone_id].output.index[0]);

      // Check if mode is permitted
      // if(zone[zone_id].bitpacked_modes_enabled,)
      uint8_t subindex_of_driver = zone[zone_id].output.index[0];
      
      SetHeater(subindex_of_driver, state);
    }
    break;
    case ZONE_MODE_COOL_8BITS:



    break;
    case ZONE_MODE_DISABLED_8BITS:


    break;
  }

}






#endif
