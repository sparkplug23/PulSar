#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef USE_MODULE_NETWORK_MQTT

void mAnimatorLight::MQTTHandler_Init(){

  uint8_t ii = 0;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Settings;
  // mqtthandler_list_ids[ii] = mqtthandler_ptr->handler_id;
  // mqtthandler_list_ptr[ii] = &mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_animation_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_ANIMATION_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Animation;

  mqtthandler_ptr = &mqtthandler_ambilight_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_AMBILIGHT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Ambilight;
  
  #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS      //make this optional, as it uses extra data and is only for special cases
  mqtthandler_ptr = &mqtthandler_notifications_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NOTIFICATIONS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Notifications;
  #endif
    
  mqtthandler_ptr = &mqtthandler_state_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_State;
  
  #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  mqtthandler_ptr = &mqtthandler_flasher_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_EFFECTS_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_EFFECTS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Flasher;
  #endif

  #ifdef ENABLE_PIXEL_FUNCTION_MIXER
  mqtthandler_ptr = &mqtthandler_mixer_teleperiod;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MIXER_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mixer;
  #endif
  
  // mqtt_handler_ids
  //  = {
  //   MQTT_HANDLER_SETTINGS_ID, MQTT_HANDLER_MODULE_ANIMATION_TELEPERIOD_ID, MQTT_HANDLER_MODULE_AMBILIGHT_TELEPERIOD_ID,
  //   MQTT_HANDLER_MODULE_STATE_TELEPERIOD_ID
  //   #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  //     MQTT_HANDLER_MODULE_NOTIFICATION_TELEPERIOD_ID,
  //   #endif
  //   #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  //     MQTT_HANDLER_MODULE_EFFECTS_TELEPERIOD_ID, MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID
  //   #endif
  // };
  
  // struct handler<mAnimatorLight>* mqtthandler_list_ptr[];
  // 4] = {
  //   &mqtthandler_settings_teleperiod, &mqtthandler_animation_teleperiod, &mqtthandler_ambilight_teleperiod,
  //   &mqtthandler_state_teleperiod,
  //   #ifdef USE_TASK_RGBLIGHTING_NOTIFICATIONS 
  //     &mqtthandler_notifications_teleperiod,
  //   #endif
  //   #ifdef ENABLE_PIXEL_FUNCTION_EFFECTS
  //     &mqtthandler_flasher_teleperiod, &mqtthandler_mixer_teleperiod
  //   #endif
  // };


#ifdef ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL
  mqtthandler_ptr = &mqtthandler_manual_setpixel;
  mqtthandler_ptr->handler_id = MQTT_HANDLER_MODULE_MIXER_TELEPERIOD_ID;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETPIXEL_MANUALLY_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Manual_SetPixel;  
#endif // ENABLE_PIXEL_FUNCTION_MANUAL_SETPIXEL



} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mAnimatorLight::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mAnimatorLight::MQTTHandler_Set_TelePeriod()
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
void mAnimatorLight::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_LIGHTS_ANIMATOR_ID, handle, id);
  }
}
  
#endif// USE_MODULE_NETWORK_MQTT

#endif //USE_MODULE_LIGHTS_ANIMATOR



