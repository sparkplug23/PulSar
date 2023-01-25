/**
 * @file    mTelemetry_MQTT.cpp
 * @author  Michael Doone (michaeldoonehub@gmail.com)
 * @brief   MQTT Configuration
 * @version 1.0
 * @date    2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

#include "2_CoreSystem/07_Telemetry/mTelemetry.h"

#ifdef USE_MODULE_NETWORK_MQTT

void mTelemetry::MQTTHandler_Init()
{

  handler<mTelemetry>* p;

  p = &mqtthandler_health;
  p->handler_id = MQTT_HANDLER_SYSTEM_HEALTH_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  #if defined(ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS) || defined(ENABLE_DEVFEATURE_DEBUG_POINT_EVERY_SECOND_HEALTH_PACKETS)
  p->tRateSecs = 1; 
  #else
  p->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS; 
  #endif // ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_10_MINUTES_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Health;
  
  p = &mqtthandler_settings;
  p->handler_id = MQTT_HANDLER_SYSTEM_SETTINGS_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings;
  
  p = &mqtthandler_log;
  p->handler_id = MQTT_HANDLER_SYSTEM_LOG_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Log;
  
  p = &mqtthandler_firmware;
  p->handler_id = MQTT_HANDLER_SYSTEM_FIRMWARE_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Firmware;
  
  p = &mqtthandler_memory;
  p->handler_id = MQTT_HANDLER_SYSTEM_MEMORY_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Memory;
  
  p = &mqtthandler_network;
  p->handler_id = MQTT_HANDLER_SYSTEM_NETWORK_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Network;
  
  p = &mqtthandler_mqtt;
  p->handler_id = MQTT_HANDLER_SYSTEM_MQTT_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_MQTT;
  
  p = &mqtthandler_time;
  p->handler_id = MQTT_HANDLER_SYSTEM_TIME_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Time;
  
  p = &mqtthandler_devices;
  p->handler_id = MQTT_HANDLER_SYSTEM_DEVICES_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEVICES_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Devices;
  
  p = &mqtthandler_reboot;
  p->handler_id = MQTT_HANDLER_SYSTEM_REBOOT_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  
  p = &mqtthandler_reboot_event;
  p->handler_id = MQTT_HANDLER_SYSTEM_REBOOT_EVENT_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = false;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR;  
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_ALL;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;

  #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
  p = &mqtthandler_debug_pins;
  p->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_PINS_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Pins;

  p = &mqtthandler_debug_template;
  p->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_TEMPLATE_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Template;

  p = &mqtthandler_debug_moduleinterface;
  p->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_MODULEINTERFACE_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULETEMPLATE_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_ModuleInterface;

  p = &mqtthandler_debug_system_saved_settings;
  p->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_MODULEINTERFACE_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = SEC_IN_HOUR; 
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_System_Stored_Settings;
  #endif // ENABLE_MQTT_DEBUG_TELEMETRY
  
  p = &mqtthandler_debug_minimal;
  p->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_MINIMAL_ID;
  p->tSavedLastSent = millis();
  p->flags.PeriodicEnabled = true;
  p->flags.SendNow = true;
  p->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS;
  p->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  p->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  p->json_level = JSON_LEVEL_DETAILED;
  p->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR;
  p->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Minimal;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mTelemetry::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mTelemetry::MQTTHandler_Set_DefaultPeriodRate()
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
void mTelemetry::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CORE_TELEMETRY_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT










