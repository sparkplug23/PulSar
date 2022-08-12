// /**
//  * @file mBME.cpp
//  * @author Michael Doone (michaeldoonehub@gmail.com)
//  * @brief BME280 Temperature, Humidity and Pressure sensor 
//  * @version 1.0
//  * @date 2022-04-20
//  * 
//  * @copyright Copyright (c) 2022
//  * 
//  * This program is free software: you can redistribute it and/or modify
//  * it under the terms of the GNU General Public License as published by
//  * the Free Software Foundation, either version 3 of the License, or
//  * (at your option) any later version.

//  * This program is distributed in the hope that it will be useful,
//  * but WITHOUT ANY WARRANTY; without even the implied warranty of
//  * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  * GNU General Public License for more details.

//  * You should have received a copy of the GNU General Public License
//  * along with this program.  If not, see <http://www.gnu.org/licenses/>.
//  **/

// #include "mBME.h"

// #ifdef USE_MODULE_SENSORS_BME

// #ifdef USE_MODULE_NETWORK_MQTT

// void mBME::MQTTHandler_Init()
// {

//   struct handler<mBME>* ptr;

//   ptr = &mqtthandler_settings_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = false;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = SEC_IN_MIN; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
//   ptr->ConstructJSON_function = &mBME::ConstructJSON_Settings;

//   ptr = &mqtthandler_sensor_teleperiod;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = SEC_IN_MIN; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;

//   ptr = &mqtthandler_sensor_ifchanged;
//   ptr->tSavedLastSent = millis();
//   ptr->flags.PeriodicEnabled = true;
//   ptr->flags.SendNow = true;
//   ptr->tRateSecs = 10; 
//   ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
//   ptr->json_level = JSON_LEVEL_DETAILED;
//   ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
//   ptr->ConstructJSON_function = &mBME::ConstructJSON_Sensor;
  
// } //end "MQTTHandler_Init"

// /**
//  * @brief Set flag for all mqtthandlers to send
//  * */
// void mBME::MQTTHandler_Set_RefreshAll()
// {
//   for(auto& handle:mqtthandler_list){
//     handle->flags.SendNow = true;
//   }
// }

// /**
//  * @brief Update 'tRateSecs' with shared teleperiod
//  * */
// void mBME::MQTTHandler_Set_TelePeriod()
// {
//   for(auto& handle:mqtthandler_list){
//     if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
//       handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
//     if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
//       handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
//   }
// }

// /**
//  * @brief MQTTHandler_Sender
//  * */
// void mBME::MQTTHandler_Sender(uint8_t id)
// {    
//   for(auto& handle:mqtthandler_list){
//     pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_BME_ID, handle, id);
//   }
// }
  
// #endif // USE_MODULE_NETWORK_MQTT

// #endif