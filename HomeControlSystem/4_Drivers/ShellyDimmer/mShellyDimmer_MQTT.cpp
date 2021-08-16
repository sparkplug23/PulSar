/*
    mShellyDimmer.cpp - Shelly Dimmer 2 support, created for Tasmota and Refactored for TBD_PROJECT_NAME
    
    Copyright (C) 2021  James Turton Created
    Copyright (C) 2021  Michael Refactored

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

#include "mShellyDimmer.h"

#ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER

#ifdef USE_MODULE_NETWORK_MQTT

void mShellyDimmer::MQTTHandler_Init(){

  struct handler<mShellyDimmer>* ptr;
    
  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_Settings;

  
  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_State;

//   ptr = &mqtthandler_sensdebug_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_CTR;
//   ptr->ConstructJSON_function = &mShellyDimmer::ConstructJSON_Debug;

} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mShellyDimmer::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mShellyDimmer::MQTTHandler_Set_TelePeriod()
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
void mShellyDimmer::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS_SHELLY_DIMMER_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif
