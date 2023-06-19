/**
 * @file    mTelemetry.cpp
 * @author  Michael Doone (michaeldoonehub@gmail.com)
 * @brief   Generate JSON of coresystem values
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

/**
 * @brief 
 * 
 * v0.110.25.28         RAM 47876     Flash 531632 == for sidelight
 * 
 * 
 */

const char* mTelemetry::PM_MODULE_CORE_TELEMETRY_CTR = D_MODULE_CORE_TELEMETRY_CTR;
const char* mTelemetry::PM_MODULE_CORE_TELEMETRY_FRIENDLY_CTR = D_MODULE_CORE_TELEMETRY_FRIENDLY_CTR;

int8_t mTelemetry::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function){
    case FUNC_INIT:
      Init();
    break;
  }

  switch(function){
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
      break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
      break;
    case FUNC_MQTT_SENDER:
      #ifndef ENABLE_DEBUGFEATURE__MQTT_STOP_STATUS_BASE_TELEMETRY
      MQTTHandler_Sender();
      #endif
      break;
    case FUNC_MQTT_CONNECTED:
    case FUNC_MQTT_STATUS_REFRESH_SEND_ALL:
      MQTTHandler_Set_RefreshAll();
      break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEGPAGE SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case FUNC_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif // USE_MODULE_NETWORK_WEBSERVER
  }

}


void mTelemetry::parse_JSONCommand(JsonParserObject obj){}

#ifdef USE_MODULE_NETWORK_WEBSERVER
void mTelemetry::WebPage_Root_AddHandlers(){

  /**
   *Add telemetry calls-- retrieve tele by http request and return as text 
   **/ 
  /* 
  pCONT_web->pWebServer->on("/status/telemetry/health.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Health(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/settings.json", HTTP_GET, [this](AsyncWebServerRequest *request){ 
    ConstructJSON_Settings(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/firmware.json", HTTP_GET, [this](AsyncWebServerRequest *request){    
    ConstructJSON_Firmware(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/log.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Log(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/memory.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Memory(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/network.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Network(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/mqtt.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_MQTT(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/time.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Time(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr);
  });
  pCONT_web->pWebServer->on("/status/telemetry/devices.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Devices(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/reboot.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Reboot(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/minimal.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_Minimal(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/pins.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_Pins(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/template.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_Template(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });
  pCONT_web->pWebServer->on("/status/telemetry/debug/moduleinterface.json", HTTP_GET, [this](AsyncWebServerRequest *request){  
    ConstructJSON_Debug_ModuleInterface(JSON_LEVEL_ALL);
    pCONT_web->WebSend_Response(request,200,CONTENT_TYPE_APPLICATION_JSON_ID,data_buffer.payload.ctr); 
  });*/

}
#endif //  #ifdef USE_MODULE_NETWORK_WEBSERVER






void mTelemetry::Init(){
  
  #ifdef ENABLE_DEVFEATURE_HARDWARE_STATUS
  memset(&hardwarestatus,0,sizeof(hardwarestatus));
  hardwarestatus.len += 0;//sprintf(hardwarestatus.ctr,"Restarted");
  #endif// ENABLE_DEVFEATURE_HARDWARE_STATUS

}





#ifdef USE_MODULE_NETWORK_MQTT

void mTelemetry::MQTTHandler_Init()
{

  handler<mTelemetry>* ptr;

  ptr = &mqtthandler_health;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_HEALTH_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  #if defined(ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS) || defined(ENABLE_DEVFEATURE_DEBUG_POINT_EVERY_SECOND_HEALTH_PACKETS)
  ptr->tRateSecs = 1; 
  #else
  ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS; 
  #endif // ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_10_MINUTES_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Health;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_settings;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_SETTINGS_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_log;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_LOG_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Log;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_firmware;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_FIRMWARE_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Firmware;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_memory;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_MEMORY_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Memory;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_network;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_NETWORK_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Network;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_mqtt;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_MQTT_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_MQTT;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_time;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_TIME_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Time;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_devices;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEVICES_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEVICES_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Devices;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_reboot;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_REBOOT_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_reboot_event;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_REBOOT_EVENT_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR;  
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_ALL;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
  ptr = &mqtthandler_debug_pins;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_PINS_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Pins;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_debug_template;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_TEMPLATE_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Template;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_debug_moduleinterface;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_MODULEINTERFACE_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULETEMPLATE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_ModuleInterface;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_debug_system_saved_settings;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_MODULEINTERFACE_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_System_Stored_Settings;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
  ptr = &mqtthandler_debug_tasker_interface_performance;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_TASKER_INTERFACE_PERFORMANCE;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 60; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TASKER_INTERFACE_PERFORMANCE;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Tasker_Interface_Performance;
  mqtthandler_list.push_back(ptr);
  #endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

  #endif // ENABLE_MQTT_DEBUG_TELEMETRY
  
  ptr = &mqtthandler_debug_minimal;
  ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_MINIMAL_ID;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Minimal;
  mqtthandler_list.push_back(ptr);

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









