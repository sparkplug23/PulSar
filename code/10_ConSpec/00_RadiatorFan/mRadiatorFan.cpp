#include "mRadiatorFan.h"

#ifdef USE_MODULE_CONTROLLER_RADIATORFAN

const char* mRadiatorFan::PM_MODULE_CONTROLLER_RADIATORFAN_CTR = D_MODULE_CONTROLLER_RADIATORFAN_CTR;
const char* mRadiatorFan::PM_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR = D_MODULE_CONTROLLER_RADIATORFAN_FRIENDLY_CTR;

int8_t mRadiatorFan::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_INIT:
      Init();
      break;
  }

  if(!settings.enabled_module){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    case FUNC_EVERY_MINUTE: 
      Task_UseTemperatureToControlRelay();
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
    #ifdef USE_MODULE_NETWORK_MQTT
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
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

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

  state.temperature_current = pCONT_msdb18->sensor_group[0].dallas->getTempC(known_address);

  AddLog(LOG_LEVEL_INFO, PSTR("temperature = %d"), state.temperature_current);

  state.iswithin_temperature_limit = IsWithinLimits(state.threshold_minimum_temperature, state.temperature_current, state.threshold_maximum_temperature);

  if(state.iswithin_temperature_limit)
  {
    /**
     * In case the sensor becomes unresponse, we will only command the relay on when the state&condition was correct, 
     * this will automatically turn off by the relay timer
     * Checks: 1 minute
     * Backoff: 2 minutes until it would turn off
     * */
    pCONT_mry->CommandSet_Timer_Decounter(60*5, 0 /*Relay0*/);   
  }

  #endif

}


uint8_t mRadiatorFan::ConstructJSON_Settings(uint8_t json_level, bool json_object_start_end_required)
{
  JBI->Start();
    JBI->Add("TemperatureControlEnabled", settings.enabled_temperature_control);
  return JBI->End();
}


uint8_t mRadiatorFan::ConstructJSON_State(uint8_t json_level)
{
  char buffer[100];

  JBI->Start();
    JBI->Level_Start("Temperature");
      JBI->Add("IsWithinLimit", state.iswithin_temperature_limit);
      JBI->Level_Start("Threshold");
        JBI->Add("Maximum", state.threshold_maximum_temperature);
        JBI->Add("Minimum", state.threshold_minimum_temperature);
      JBI->Level_End();
      JBI->Add("TriggerTemperature", state.temperature_current);
      JsonBuilderI->Level_Start("InternalSensors");
        for(int sensor_id=0;sensor_id<3;sensor_id++)
        { 
          // JsonBuilderI->Level_Start(DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_DB18S20_ID,pCONT_msdb18->sensor[sensor_id].address_id,buffer,sizeof(buffer)));    
          JsonBuilderI->Level_Start(DLI->GetDeviceName_WithModuleUniqueID( pCONT_msdb18->GetModuleUniqueID() ,pCONT_msdb18->sensor[sensor_id].address_id,buffer,sizeof(buffer)));         


          
            JsonBuilderI->Add(D_JSON_TEMPERATURE, pCONT_msdb18->sensor[sensor_id].reading.val);
          JsonBuilderI->Level_End();  
        }
      JsonBuilderI->Level_End();  
    JBI->Level_End();
    
    JBI->Level_Start("Fan"); // Based on relay controls
      JBI->Add("State",                  pCONT_mry->CommandGet_Relay_Power(0));
      JBI->Add("TimeOnSeconds",          pCONT_mry->CommandGet_SecondsRelayHasBeenOn(0));          // total time on
      JBI->Add("TimeOnDecounterSeconds", pCONT_mry->CommandGet_SecondsToRemainOn(0)); // time to stay on
    JBI->Level_End();
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

void mRadiatorFan::MQTTHandler_Init()
{

  struct handler<mRadiatorFan>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_Settings;

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_State;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mRadiatorFan::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mRadiatorFan::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mRadiatorFan::MQTTHandler_Set_TelePeriod()
{
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mRadiatorFan::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER_RADIATORFAN_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT



#endif
