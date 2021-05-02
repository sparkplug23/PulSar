/*
  mHVAC.cpp - HVAC Controller

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

const char* mHVAC::PM_MODULE_CONTROLLER_HVAC_CTR = D_MODULE_CONTROLLER_HVAC_CTR;
const char* mHVAC::PM_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR = D_MODULE_CONTROLLER_HVAC_FRIENDLY_CTR;

int8_t mHVAC::Tasker(uint8_t function, JsonParserObject obj){

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

  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

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
    case FUNC_FUNCTION_LAMBDA_INIT:
      FunctionHandler_Init();
    break;
    case FUNC_FUNCTION_LAMBDA_LOOP:
      FunctionHandler_Loop();
    break;
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
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
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



void mHVAC::Every_Second()
{
  //temp calculate always, later add flags to update it
  // SubContructCtr_HardwareStatus();

  //Update zone variables 

  for(uint8_t zone_id=0; zone_id<settings.active_zones; zone_id++)
  {
    sensors_reading_t reading;
    int16_t module_array_index = pCONT->GetVectorIndexbyModuleUniqueID(zone[zone_id].sensor.module_id);
    pCONT->pModule[module_array_index]->GetSensorReading(&reading);
    float measured_zone_temp = reading.GetValue(SENSOR_TYPE_TEMPERATURE_ID);
    float measured_zone_hum  = reading.GetValue(SENSOR_TYPE_RELATIVE_HUMIDITY_ID);
    // Store current temperature
    zone[zone_id].sensor.temperature = measured_zone_temp;
    zone[zone_id].sensor.humidity = measured_zone_hum;
  }
    


}



void mHVAC::pre_init(void){

  settings.active_zones = 4;

}

void mHVAC::init(void){

  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    // Create program methods
    zone[id].program_temp_method = new ProgramTemperature();
    zone[id].program_timer_method = new ProgramTimer();
  }

  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    zone[id].active_mode = ZONE_MODE_HEAT_8BITS;
    zone[id].bitpacked_modes_enabled = ZONE_MODE_HEAT_8BITS;// | ZONE_MODE_COOL_8BITS;
  }
    zone[0].bitpacked_modes_enabled = ZONE_MODE_HEAT_8BITS | ZONE_MODE_COOL_8BITS;

  /**
   * Save sensor type into zone so I know later how to get it
   * */
  // Lets set these by names (make fully unique, so they can get them from buffer easily)
  // zone[0].sensor.module_id = D_UNIQUE_MODULE_SENSORS_DHT_ID;
  // zone[0].sensor.index = 0; //First index
  // zone[1].sensor.module_id = D_UNIQUE_MODULE_SENSORS_DHT_ID;
  // zone[1].sensor.index = 0; //First index

  // zone[2].sensor.module_id = D_UNIQUE_MODULE_SENSORS_DB18_ID;
  // zone[2].sensor.index = 0;
  // zone[3].sensor.module_id = D_UNIQUE_MODULE_SENSORS_DB18_ID;
  // zone[3].sensor.index = 1;

  /**
   * Configure relays out
   * */
  for(uint8_t id=0; id<settings.active_zones; id++)
  {
    zone[id].output.module_ids.push_back(D_UNIQUE_MODULE_DRIVERS_RELAY_ID);
    zone[id].output.index.push_back(id);
    zone[id].output.driver_type.push_back(DRIVER_TYPE_HVAC_ID);
  }

  zone[0].output.module_ids.push_back(D_UNIQUE_MODULE_DRIVERS_RELAY_ID);
  zone[0].output.index.push_back(1);
  zone[0].output.driver_type.push_back(DRIVER_TYPE_COOLING_ID);
  
  
  for(uint8_t device_id=0;device_id<4;device_id++){ 
    SetHeatingRelay(device_id,RELAY_STATE_OFF); 
  }

  // memset(&status_message, 0, sizeof(status_message));
// 
  init_program_timers();
  // init_program_temps();
  // init_program_scheduling();
  
  // #if defined(USE_HVAC_TEMPS) && defined(USE_HVAC_PROFILE_ESTIMATION)
  //   init_HeatingProfiles();
  // #endif
  
  settings.fEnableModule = true;

}//end function


void mHVAC::SetHeater(uint8_t device, uint8_t state){

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







/**
 * Generate Messages for users to glance at via web or mqtt, timers, temps, schedules set? append the messages.
 * */
void mHVAC::FunctionHandler_Program_Status(){

return;
  memset(&heating_status,0,sizeof(heating_status));

  // for(uint8_t device_id=0;device_id<4;device_id++){
    
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

/*******************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
********* RELAYS AND FAILSAFES ***************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************/


void mHVAC::SetHeatingRelay(uint8_t device_id, uint8_t state){
  pCONT_mry->CommandSet_Relay_Power(state,device_id);
}


uint8_t mHVAC::GetHeatingRelay(uint8_t device_id){
  return pCONT_mry->CommandGet_Relay_Power(device_id);
}


uint8_t mHVAC::GetAnyHeatingRelay(){
  uint8_t oncount = 0;
  for(uint8_t i = 0;i < 4;i++){
    oncount += GetHeatingRelay(i);
  }
  return oncount;
}


void mHVAC::FunctionHandler_Relay_Status(){ DEBUG_LINE;

//remove, assume part of other functions

  for(uint8_t device_id=0;device_id<4;device_id++){
    if(pCONT_mry->CommandGet_Relay_Power(device_id)){
      // activeprograms[device_id].relays.state = 1;
    }else{
      // activeprograms[device_id].relays.state = 0;
    }
  }

} // END function

/*******************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
********* RELAYS AND FAILSAFES ***************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************
************************************************************************************************************************************************/









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

      AddLog(LOG_LEVEL_INFO, PSTR("zone[%d].output.index[0]=%d"),zone_id, zone[zone_id].output.index[0]);

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





void mHVAC::FunctionHandler_Init(){
  
  struct functionhandler<mHVAC>* functionhandler_ptr = nullptr;

  functionhandler_ptr = &functionhandler_status_message;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHVAC::FunctionHandler_Program_Status;
  
  functionhandler_ptr = &functionhandler_failsafe;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_MINS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHVAC::FunctionHandler_FailSafe;

  #ifdef USE_HVAC_PROFILE_ESTIMATION  
  functionhandler_ptr = &functionhandler_hvac_profiles;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHVAC::FunctionHandler_HVAC_Profiles;
  #endif //#ifdef USE_HVAC_PROFILE_ESTIMATION
      
  functionhandler_ptr = &functionhandler_relay_status;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHVAC::FunctionHandler_Relay_Status;

  functionhandler_ptr = &functionhandler_programs_timers;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  #ifdef ENABLE_HVAC_DEBUG_TIMES
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  #else
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  #endif
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHVAC::FunctionHandler_Programs_Timers;
  
  functionhandler_ptr = &functionhandler_programs_temps;
  functionhandler_ptr->saved_millis = millis();
  functionhandler_ptr->flags.periodic_enabled = true;
  functionhandler_ptr->flags.run_now = true;
  functionhandler_ptr->flags.run_always = false;
  functionhandler_ptr->flags.time_unit = FUNCHANDLER_TIME_SECS_ID; 
  functionhandler_ptr->time_val = 1;
  functionhandler_ptr->function = &mHVAC::FunctionHandler_Programs_Temps;
    
}


void mHVAC::FunctionHandler_Loop()
{  
  for(auto& handle:functionhandler_list){
    pCONT_sup->FunctionHandler_Call(*this, EM_MODULE_CONTROLLER_HVAC_ID, handle);
  }
}


#ifdef USE_MODULE_NETWORK_MQTT

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHVAC::MQTTHandler_Init(){

  struct handler<mHVAC>* mqtthandler_ptr;

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
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_Settings;
  
  mqtthandler_ptr = &mqtthandler_program_timers_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TIMERS_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTimers;
  
  mqtthandler_ptr = &mqtthandler_program_timers_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TIMERS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTimers;

  mqtthandler_ptr = &mqtthandler_program_temps_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TEMPS_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTemps;
  
  mqtthandler_ptr = &mqtthandler_program_temps_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TEMPS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTemps;

  mqtthandler_ptr = &mqtthandler_program_overview_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramActive;
  
  mqtthandler_ptr = &mqtthandler_program_overview_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramActive;
  
  mqtthandler_ptr = &mqtthandler_relays_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_DRIVERS_RELAYS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_HeatingRelays;

  mqtthandler_ptr = &mqtthandler_relays_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_DRIVERS_RELAYS_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_HeatingRelays;
  
  mqtthandler_ptr = &mqtthandler_sensor_zone_ifchanged;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_IFCHANGED_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors;

  mqtthandler_ptr = &mqtthandler_sensor_zone_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors;

  mqtthandler_ptr = &mqtthandler_sensor_zone_roc1m;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_ROC1M_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors_ROC1m;
  
  mqtthandler_ptr = &mqtthandler_sensor_zone_roc10m;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_ROC10M_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60*10; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors_ROC10m;


  // mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_ifchanged;
  // mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_IFCHANGED_ID;
  // mqtthandler_ptr->tSavedLastSent = millis();
  // mqtthandler_ptr->flags.PeriodicEnabled = true;
  // mqtthandler_ptr->flags.SendNow = true;
  // mqtthandler_ptr->tRateSecs = 10; 
  // mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  // mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  // mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;
  // mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructSON_PipeTempsByColours;
  
  // mqtthandler_ptr = &mqtthandler_sensor_pipes_colours_teleperiod;
  // mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_PIPES_COLOURS_TELEPERIOD_ID;
  // mqtthandler_ptr->tSavedLastSent = millis();
  // mqtthandler_ptr->flags.PeriodicEnabled = true;
  // mqtthandler_ptr->flags.SendNow = true;
  // mqtthandler_ptr->tRateSecs = 60; 
  // mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  // mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  // mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PIPES_COLOUR_CTR;
  // mqtthandler_ptr->ConstructJSON_function = &mHVAC::ConstructSON_PipeTempsByColours;

  // // Measure and report every second
  // pCONT_msdb18->settings.rate_measure_ms = 1000;
  // // pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = 5;

  // //Reduce sensor lib from reporting to minimum
  // pCONT_msdb18->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  // pCONT_msdb18->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  // pCONT_msdb18->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

  // //Reduce sensor lib from reporting to minimum
  // pCONT_dht->mqtthandler_sensor_ifchanged.tRateSecs = SEC_IN_HOUR;
  // pCONT_dht->mqtthandler_sensor_teleperiod.tRateSecs = SEC_IN_HOUR;
  // pCONT_dht->mqtthandler_settings_teleperiod.tRateSecs = SEC_IN_HOUR;

  

}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHVAC::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mHVAC::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mHVAC::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER_HVAC_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif
