#include "mBucketWaterLevel.h" 

#ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL

#ifdef USE_MODULE_NETWORK_MQTT

void mBucketWaterLevel::MQTTHandler_Init()
{

  struct handler<mBucketWaterLevel>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 120; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mBucketWaterLevel::ConstructJSON_Settings;

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mBucketWaterLevel::ConstructJSON_State;

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mBucketWaterLevel::ConstructJSON_State;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mBucketWaterLevel::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mBucketWaterLevel::MQTTHandler_Set_DefaultPeriodRate()
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
void mBucketWaterLevel::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif
