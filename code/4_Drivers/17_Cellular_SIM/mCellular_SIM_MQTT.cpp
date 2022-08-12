#include "mCellular_SIM.h" 

#ifdef USE_MODULE_DRIVERS__CELLULAR_SIM7000

#ifdef USE_MODULE_NETWORK_MQTT

void mCellular_SIM7000::MQTTHandler_Init()
{

  struct handler<mCellular_SIM7000>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellular_SIM7000::ConstructJSON_Settings;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellular_SIM7000::ConstructJSON_State;

  #ifdef ENABLE_DEBUG_FEATURE_MQTT__CELLULAR_SIM__DEBUG_POLL_LATEST
  ptr = &mqtthandler_debug_poll_latest_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_POLL_LATEST__CTR;
  ptr->ConstructJSON_function = &mCellular_SIM7000::ConstructJSON_Debug_RequestLatest;
  #endif    

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mCellular_SIM7000::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mCellular_SIM7000::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mCellular_SIM7000::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS__CELLULAR_SIM7000__ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_DRIVERS_RF433_RCSWITCH
