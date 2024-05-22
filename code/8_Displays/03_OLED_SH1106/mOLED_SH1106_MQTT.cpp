#include "mOLED_SH1106.h" 

#ifdef USE_MODULE_DISPLAYS_OLED_SH1106

#ifdef USE_MODULE_NETWORK_MQTT

void mOLED_SH1106::MQTTHandler_Init()
{

  struct handler<mOLED_SH1106>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; 
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mOLED_SH1106::ConstructJSON_Settings;

} // END "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mOLED_SH1106::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mOLED_SH1106::MQTTHandler_Set_DefaultPeriodRate()
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
void mOLED_SH1106::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DISPLAYS_OLED_SH1106_ID, handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif // USE_MODULE_DISPLAYS_OLED_SH1106
