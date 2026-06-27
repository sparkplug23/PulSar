#include "mTelemetry.h"

int8_t mTelemetry::Tasker(uint8_t function, JsonParserObject obj)
{
  

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:

      if (serial_messages_remaining_to_send > 0) {
        serial_messages_remaining_to_send--;
        auto handle = mqtthandler_list[serial_messages_remaining_to_send];
        uint8_t fSendPayload = CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->json_level, true);\
        ALOG_INF(PSTR(D_LOG_TELEMETRY ">>>>>>>%d/%d%S<<<<<<<<\n\r%s"), 
          mqtthandler_list.size()-serial_messages_remaining_to_send,
          mqtthandler_list.size(),
          mqtthandler_list[serial_messages_remaining_to_send]->postfix_topic,
          JBI->GetBuffer());
      }    
      
    break;
    case TASK_UPTIME_10_MINUTES:

      if(tkr_time->IsBuildDateTimeElapsedBeyond(SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY))
      {
        ALOG_INF(PSTR("IsBuildDateTimeElapsedBeyond %d"), tkr_time->IsBuildDateTimeElapsedBeyond(SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY));
        ALOG_INF(PSTR("BuildDateTimeElapsed %d"), tkr_time->BuildDateTimeElapsed());
        serial_messages_remaining_to_send = mqtthandler_list.size();
      }
      
    break;
    /******************
     * Connections
     ******************/
    case TASK_MQTT_CONNECTED:
      /**
       * Broadcasting reboot event just once after power on, when connection is made
       **/
      if(mqtthandler_reboot_event.tSavedLastSent == 0){
        ALOG_INF(PSTR("MQTT Connected - Sending Reboot Event"));
        mqtthandler_reboot_event.flags.SendNow = true; // set to send now
      }
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      #ifdef ENABLE_DEBUGFEATURE_TELEMETRY__MQTT_SEND_HEALTH_EVERY_SECOND
      mqtthandler_health.tRateSecs = 1; 
      #else
      // tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
      #endif
    break;
    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

  return TASKER_RESULT__UNKNOWN_ID;

}


#ifdef USE_MODULE_NETWORK_MQTT

void mTelemetry::MQTTHandler_Init()
{

  handler<mTelemetry>* ptr;
  
  ptr = &mqtthandler_lwt_online;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = 3600; // Hourly 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_LWT_ONLINE_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LWT_ONLINE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_LWT_Online;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_health;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  #if defined(ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS) || defined(ENABLE_DEBUGFEATURE_TELEMETRY__MQTT_SEND_HEALTH_EVERY_SECOND)
  ptr->tRateSecs = 1; 
  #else
  ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS; 
  #endif // ENABLE_DEVFEATURE_DEBUG_SLOW_LOOPS
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_1_MINUTES_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Health;
  mqtthandler_list.push_back(ptr);

  #ifndef FIRMWARE_MINIMAL2
  
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_log;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Log;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_firmware;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Firmware;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_memory;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = 10;//SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Memory;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_network;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Network;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_mqtt;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_MQTT;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_time;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Time;
  mqtthandler_list.push_back(ptr);
    
  ptr = &mqtthandler_reboot;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_reboot_event; //I think this needs phased away, and only ever is sent on boot
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->flags.retain = false;
  ptr->tRateSecs = SEC_IN_HOUR;  
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->json_level = JSON_LEVEL_ALL;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
    
    ptr = &mqtthandler_devices;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = SEC_IN_HOUR; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_DEVICES_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Devices;
    mqtthandler_list.push_back(ptr);

    ptr = &mqtthandler_debug_pins_gpio;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = SEC_IN_HOUR; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_GPIO_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Pins_GPIO;
    mqtthandler_list.push_back(ptr);

    ptr = &mqtthandler_debug_pins_table;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = SEC_IN_HOUR; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_TABLE_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Pins_Table;
    mqtthandler_list.push_back(ptr);

    ptr = &mqtthandler_debug_template;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = SEC_IN_HOUR; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Template;
    mqtthandler_list.push_back(ptr);

    ptr = &mqtthandler_debug_moduleinterface;
    ptr->tSavedLastSent = 0;
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
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = SEC_IN_HOUR; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SYSTEM_STORED_SETTINGS_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_System_Stored_Settings;
    mqtthandler_list.push_back(ptr);

    #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_PERFORMANCE
    ptr = &mqtthandler_debug_tasker_interface_performance; 
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = 5; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = MQTT_TOPIC_TYPE_IFCHANGED_ID;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TASKER_INTERFACE_PERFORMANCE;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Tasker_Interface_Performance;
    mqtthandler_list.push_back(ptr);
    #endif 
    
    #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
    ptr = &mqtthandler_debug__settings_storage;
    ptr->handler_id = MQTT_HANDLER_SYSTEM_DEBUG_SETTINGS_STORAGE;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.retain = true;
    ptr->flags.SendNow = true;
    ptr->tRateSecs = 60; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = MQTT_TOPIC_TYPE_IFCHANGED_ID;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SETTINGS_STORAGE_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug__Settings_Storage;
    mqtthandler_list.push_back(ptr);
    #endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
    
    ptr = &mqtthandler_debug_minimal;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS;
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Minimal;
    mqtthandler_list.push_back(ptr);

  #endif // ENABLE_MQTT_DEBUG_TELEMETRY
  
  #endif // FIRMWARE_MINIMAL2

} 

#endif // USE_MODULE_NETWORK_MQTT








