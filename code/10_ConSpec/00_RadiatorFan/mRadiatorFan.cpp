#include "mRadiatorFan.h"

#ifdef USE_MODULE_CONTROLLER_RADIATORFAN


int8_t mRadiatorFan::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_INIT:
      Init();
      break;
  }

  if(!settings.enabled_module){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    case TASK_EVERY_MINUTE: 
      Task_UseTemperatureToControlRelay();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
     /************
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      //tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;
  
}


void mRadiatorFan::Init(void)
{

}


void mRadiatorFan::Task_UseTemperatureToControlRelay()
{

  #ifdef USE_MODULE_SENSORS_DS18X

  /**
   * hard coding solution for now, search for sensor by its address
   * later, search for sensor by its name (using generic sensor_interface ie "name" return "temp")
   * */
  DeviceAddress known_address = {40,159,147,2,0,0,0,117};

  state.temperature_current = tkr_db18->sensor_group[0].dallas->getTempC(known_address);

  ALOG_INF(PSTR("temperature = %d"), state.temperature_current);

  state.iswithin_temperature_limit = IsWithinLimits(state.threshold_minimum_temperature, state.temperature_current, state.threshold_maximum_temperature);

  if(state.iswithin_temperature_limit)
  {
    /**
     * In case the sensor becomes unresponse, we will only command the relay on when the state&condition was correct, 
     * this will automatically turn off by the relay timer
     * Checks: 1 minute
     * Backoff: 2 minutes until it would turn off
     * */
    tkr_relay->CommandSet_Timer_Decounter(60*5, 0 /*Relay0*/);   
  }

  #endif

}


uint8_t mRadiatorFan::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();
    JBI->Add("TemperatureControlEnabled", settings.enabled_temperature_control);
  return JBI->End();
}


uint8_t mRadiatorFan::ConstructJSON_State(uint8_t json_level, bool json_appending)
{
  char buffer[100];

  JBI->Start();
    JBI->Object_Start("Temperature");
      JBI->Add("IsWithinLimit", state.iswithin_temperature_limit);
      JBI->Object_Start("Threshold");
        JBI->Add("Maximum", state.threshold_maximum_temperature);
        JBI->Add("Minimum", state.threshold_minimum_temperature);
      JBI->Object_End();
      JBI->Add("TriggerTemperature", state.temperature_current);
      JBI->Object_Start("InternalSensors");
        for(int sensor_id=0;sensor_id<3;sensor_id++)
        { 
          if(tkr_db18->sensor_vector.size())
          {
            // JBI->Object_Start(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_DB18S20_ID,tkr_db18->sensor[sensor_id].address_id,buffer,sizeof(buffer)));    
            JBI->Object_Start(DLI->GetDeviceName_WithModuleUniqueID( tkr_db18->GetModuleUniqueID() ,tkr_db18->sensor_vector[sensor_id].device_name_index,buffer,sizeof(buffer)));         
              JBI->Add(D_TEMPERATURE, tkr_db18->sensor_vector[sensor_id].reading.val);
            JBI->Object_End();  
          }
        }
      JBI->Object_End();  
    JBI->Object_End();
    
    JBI->Object_Start("Fan"); // Based on relay controls
      JBI->Add("State",                  tkr_relay->CommandGet_Relay_Power(0));
      JBI->Add("TimeOnSeconds",          tkr_relay->CommandGet_SecondsRelayHasBeenOn(0));          // total time on
      JBI->Add("TimeOnDecounterSeconds", tkr_relay->CommandGet_SecondsToRemainOn(0)); // time to stay on
    JBI->Object_End();
  return JBI->End();
}


/******************************************************************************************************************
 * mRadiatorFan_Commands.cpp
*******************************************************************************************************************/


void mRadiatorFan::parse_JSONCommand(JsonParserObject obj){


}


/******************************************************************************************************************
 * mRadiatorFan_MQTT.cpp
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mRadiatorFan::Telemetry_Init()
{

  struct telemetry_handler<mRadiatorFan>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_State;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 


/**
 * @brief Set flag for all telemetryhandlers to send
 * */
void mRadiatorFan::MQTTHandler_RefreshAll()
{
  for(auto& handle:telemetry_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mRadiatorFan::MQTTHandler_Rate()
{
  for(auto& handle:telemetry_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = tkr_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mRadiatorFan::MQTTHandler_Sender()
{
  for(auto& handle:telemetry_list){
    tkr_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT



#endif
