
#include "mSwitches.h"

#include "2_CoreSystem/mBaseConfig.h"

#ifdef USE_MODULE_SENSORS_SWITCHES

#ifdef USE_MODULE_NETWORK_MQTT

void mSwitches::MQTTHandler_Init(){

  struct handler<mSwitches>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mSwitches::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSwitches::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSwitches::MQTTHandler_Set_TelePeriod()
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
void mSwitches::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_SWITCHES_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif