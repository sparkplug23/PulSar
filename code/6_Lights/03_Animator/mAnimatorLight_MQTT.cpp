#include "mAnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR

#ifdef USE_MODULE_NETWORK_MQTT

void mAnimatorLight::MQTTHandler_Init()
{

  struct handler<mAnimatorLight>* ptr;
  

  ptr = &mqtthandler_settings_teleperiod;
  ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Settings;

  ptr = &mqtthandler_animation_active_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE__ANIMATION_ACTIVE_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATION_ACTIVE_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Animation_Active;

  ptr = &mqtthandler_playlists_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE__PLAYLISTS_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__PLAYLISTS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Playlist;
  
  #ifdef ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT
  ptr = &mqtthandler_mode_ambilight_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE__AMBILIGHT_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_AMBILIGHT__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_Ambilight;
  #endif // ENABLE_FEATURE_PIXEL__MODE_AMBILIGHT

  #ifdef ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
  ptr = &mqtthandler_manual_setpixel;
  ptr->handler_id = MQTT_HANDLER_MODULE__NOTIFICATION_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  #endif // ENABLE_FEATURE_PIXEL__MODE_MANUAL_SETPIXEL
    
  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
  ptr = &mqtthandler_automation_presets;
  ptr->handler_id = MQTT_HANDLER_MODULE__AUTOMATION_PRESETS__ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__AUTOMATION_PRESETS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Auto_Presets;
  #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PRESETS
    
  #ifdef ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
  ptr = &mqtthandler_manual_setpixel;
  ptr->handler_id = MQTT_HANDLER_MODULE__NOTIFICATION_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__MODE_MANUAL_SETPIXEL_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Mode_SetManual;
  #endif // ENABLE_FEATURE_PIXEL__AUTOMATION_PLAYLISTS
    
    

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE
  ptr = &mqtthandler_debug_palette;
  ptr->handler_id = MQTT_HANDLER_MODULE__DEBUG_PALETTE_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_PALETTE__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Palette;
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_PALETTE

  #ifdef ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS
  ptr                         = &mqtthandler_debug_segments;
  ptr->handler_id             = MQTT_HANDLER_MODULE__DEBUG_SEGMENTS_TELEPERIOD_ID;
  ptr->tSavedLastSent         = millis();
  ptr->flags.PeriodicEnabled  = true;
  ptr->flags.SendNow          = true;
  ptr->tRateSecs              = 1; 
  ptr->topic_type             = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level             = JSON_LEVEL_DETAILED;
  ptr->postfix_topic          = PM_MQTT_HANDLER_POSTFIX_TOPIC__DEBUG_SEGMENTS__CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Segments;
  #endif // ENABLE_DEBUG_FEATURE_MQTT_ANIMATOR_DEBUG_SEGMENTS


  #ifdef USE_DEVFEATURE_ENABLE_ANIMATION_SPECIAL_DEBUG_FEEDBACK_OVER_MQTT_WITH_FUNCTION_CALLBACK
  ptr = &mqtthandler_debug_animations_progress;
  ptr->handler_id = MQTT_HANDLER_MODULE__DEBUG_ANIMATOR_ANIMATION_PROGRESS_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__ANIMATIONS_PROGRESS_CTR;
  ptr->ConstructJSON_function = &mAnimatorLight::ConstructJSON_Debug_Animations_Progress;
  #endif

} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mAnimatorLight::MQTTHandler_Set_RefreshAll()
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
void mAnimatorLight::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_LIGHTS_ANIMATOR_ID, handle, id);
  }
}
  
#endif// USE_MODULE_NETWORK_MQTT

#endif //USE_MODULE_LIGHTS_ANIMATOR




