/*
  mHVAC.cpp - HVAC Controller

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

#ifdef USE_MODULE_NETWORK_MQTT

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHVAC::MQTTHandler_Init(){

  struct handler<mHVAC>* ptr;

  //create "sendnow" (using handler) with passing construct level?

  ptr = &mqtthandler_settings_teleperiod;
  ptr->handler_id = MQTT_HANDLER_SETTINGS_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_Settings;
  
  ptr = &mqtthandler_program_timers_ifchanged;
  ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TIMERS_IFCHANGED_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTimers;
  
  ptr = &mqtthandler_program_timers_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TIMERS_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TIMERS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTimers;

  ptr = &mqtthandler_program_temps_ifchanged;
  ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TEMPS_IFCHANGED_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTemps;
  
  ptr = &mqtthandler_program_temps_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_TEMPS_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_TEMPS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramTemps;

  ptr = &mqtthandler_program_overview_ifchanged;
  ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_IFCHANGED_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramActive;
  
  ptr = &mqtthandler_program_overview_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE_PROGRAM_OVERVIEW_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PROGRAM_OVERVIEW_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ProgramActive;
  
  ptr = &mqtthandler_relays_ifchanged;
  ptr->handler_id = MQTT_HANDLER_MODULE_DRIVERS_RELAYS_IFCHANGED_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_HeatingRelays;
  
  ptr = &mqtthandler_relays_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE_DRIVERS_RELAYS_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_RELAYS_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_HeatingRelays;

  ptr = &mqtthandler_sensor_zone_ifchanged;
  ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_IFCHANGED_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors;

  ptr = &mqtthandler_sensor_zone_teleperiod;
  ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_TELEPERIOD_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors;

  ptr = &mqtthandler_sensor_zone_roc1m;
  ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_ROC1M_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_ROC1M_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors_ROC1m;
  
  ptr = &mqtthandler_sensor_zone_roc10m;
  ptr->handler_id = MQTT_HANDLER_MODULE_SENSOR_ZONE_ROC10M_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = 60*10; 
  ptr->topic_type = MQTT_TOPIC_TYPE_ROC10M_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_ZONE_CTR;
  ptr->ConstructJSON_function = &mHVAC::ConstructJSON_ZoneSensors_ROC10m;

}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mHVAC::MQTTHandler_Set_RefreshAll()
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
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mHVAC::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){  
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER__HVAC__ID, handle, id);
  }
}
  
#endif // USE_MODULE_NETWORK_MQTT

#endif