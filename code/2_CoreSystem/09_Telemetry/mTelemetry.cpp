#include "mTelemetry.h"

int8_t mTelemetry::Tasker(uint8_t function, JsonParserObject obj)
{
  

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_EVERY_SECOND:

      if (serial_messages_remaining_to_send > 0) {

        /**
         * Serial telemetry splash.
         *
         * serial_messages_remaining_to_send is loaded with mqtthandler_list.size()
         * by TASK_UPTIME_10_MINUTES. Each TASK_EVERY_SECOND call consumes one handler,
         * builds that handler's JSON into JBI, then prints a compact debug block:
         *
         *   >------ <topic>,<rate_secs>,<json_level>
         *   <payload>
         *
         * This intentionally sends one telemetry block per second rather than dumping
         * all handlers in one burst, keeping serial/debug output readable and avoiding
         * long blocking log writes.
         */
        const uint16_t handler_count = mqtthandler_list.size();
        const uint16_t handler_index = handler_count - serial_messages_remaining_to_send;
        serial_messages_remaining_to_send--;
        auto handle = mqtthandler_list[handler_index];
        CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->json_level, true);
        ALOG_INF(PSTR(D_LOG_TELEMETRY ">------ %S,%d,%d\r\n%s\r\n"), handle->postfix_topic, handle->tRateSecs, handle->json_level, JBI->GetBuffer());

      }
      
    break;
    case TASK_UPTIME_1_MINUTES:

      /**
       * Delayed serial telemetry splash.
       *
       * This schedules a one-shot dump of each telemetry handler to the serial/debug log,
       * but only after the firmware build has been running for a minimum age.
       *
       * Reason:
       *  - During development, the first boot after compiling is already noisy with boot,
       *    WiFi, module init, MQTT, and filesystem logs.
       *  - The telemetry splash is useful for validating handler output, JSON structure,
       *    topic postfixes, and module telemetry registration.
       *  - Delaying it avoids cluttering the initial boot path while still giving a
       *    periodic sanity check once the device has settled.
       *
       * The build-time age check is intentional. It prevents freshly flashed builds from
       * immediately dumping telemetry on every early boot/reboot cycle. Once the compiled
       * firmware is older than SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY, the
       * splash is allowed to run when this uptime task fires.
       *
       * serial_messages_remaining_to_send is consumed by TASK_EVERY_SECOND so that only
       * one telemetry block is printed per second. This avoids producing a large burst of
       * serial output in a single task call.
       */
      if (tkr_time->IsBuildDateTimeElapsedBeyond(SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY))
      {
        ALOG_DBM(PSTR("BuildDateTimeElapsed %d"), tkr_time->BuildDateTimeElapsed());
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
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this, 1000);
    break;
    #endif //USE_MODULE_NETWORK_MQTT
    /************
     * WEBUI SECTION *
    *******************/
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;

}

/******************************************************************************************************************
 * SECTION: Web
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_WEBSERVER

static void WebPrint_JSString(AsyncResponseStream& response, const char* s)
{
  if (!s) { return; }

  while (*s)
  {
    const char c = *s++;

    switch (c)
    {
      case '\\': response.print(F("\\\\")); break;
      case '"':  response.print(F("\\\"")); break;
      case '\n': response.print(F("\\n"));  break;
      case '\r': response.print(F("\\r"));  break;
      case '\t': response.print(F("\\t"));  break;

      default:
        if ((uint8_t)c < 32) {
          response.print(' ');
        } else {
          response.print(c);
        }
      break;
    }
  }
}


void mTelemetry::WebPage_Root_AddHandlers()
{
  SPGM_CTR(PM_WEB_TELEMETRY_JS) "/m/telemetry/s.js";
  SPGM_CTR(PM_WEB_TELEMETRY)    "/m/telemetry";

  /**
   * Route order matters.
   *
   * The webserver's URL matching can allow shorter parent routes to catch
   * longer child paths. Always register the longest/specific telemetry URL
   * first, otherwise /m/telemetry may catch /m/telemetry/s.js and return the
   * HTML page instead of the JavaScript data chunk.
   */
  tkr_web->server->on(PM_WEB_TELEMETRY_JS, HTTP_GET, [this](AsyncWebServerRequest *request){
    this->Serve_Web_Telemetry_JS(request);
  });
  AddURLtoList(PM_WEB_TELEMETRY_JS, HTTP_GET);

  tkr_web->server->on(PM_WEB_TELEMETRY, HTTP_GET, [this](AsyncWebServerRequest *request){
    this->Serve_Web_Telemetry_Page(request);
  });
  AddURLtoList(PM_WEB_TELEMETRY, HTTP_GET);
}


void mTelemetry::Serve_Web_Telemetry_Page(AsyncWebServerRequest* request)
{
  AsyncWebServerResponse *response = request->beginResponse_P(
    200,
    FPSTR(CONTENT_TYPE_HTML),
    WEB_2_CoreSystem_09_Telemetry_telemetry_handler_htm,
    WEB_2_CoreSystem_09_Telemetry_telemetry_handler_htm_length
  );

  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));
  tkr_web->setStaticContentCacheHeaders(response);

  request->send(response);
}

void mTelemetry::Serve_Web_Telemetry_JS(AsyncWebServerRequest* request)
{
  static const uint8_t HANDLERS_PER_PAGE = 2;

  uint16_t page = 0;

  if (request->hasParam("p")) {
    page = request->getParam("p")->value().toInt();
  }

  const uint16_t total_handlers = mqtthandler_list.size();
  const uint16_t total_pages = (total_handlers + HANDLERS_PER_PAGE - 1) / HANDLERS_PER_PAGE;

  AsyncResponseStream *response = request->beginResponseStream(FPSTR(CONTENT_TYPE_JAVASCRIPT));

  response->addHeader(F("Cache-Control"), F("no-store"));
  response->addHeader(F("Expires"), F("0"));

  if ((total_handlers == 0) || (page >= total_pages))
  {
    response->print(F("H(0,0,0);N(-1);"));
    request->send(response);
    return;
  }

  const uint16_t start = page * HANDLERS_PER_PAGE;
  uint16_t end = start + HANDLERS_PER_PAGE;

  if (end > total_handlers) {
    end = total_handlers;
  }

  response->printf_P(
    PSTR("H(%u,%u,%u);"),
    page,
    total_pages,
    total_handlers
  );

  for (uint16_t i = start; i < end; i++)
  {
    auto handle = mqtthandler_list[i];

    if (!handle || !handle->ConstructJSON_function) {
      continue;
    }

    char topic[96] = {0};

    if (handle->postfix_topic)
    {
      snprintf_P(topic, sizeof(topic), PSTR("%S"), handle->postfix_topic);
    }
    else
    {
      snprintf(topic, sizeof(topic), "handler_%u", i);
    }

    CALL_MEMBER_FUNCTION(*this, handle->ConstructJSON_function)(handle->flags.json_level, true);

    response->printf_P(PSTR("A(%u,\""), i);
    WebPrint_JSString(*response, topic);

    response->printf_P(
      PSTR("\",%u,%u,%u,\"%04X\",\""),
      handle->tRateSecs,
      handle->flags.json_level,
      handle->flags.topic_type,
      handle->flags.data
    );

    WebPrint_JSString(*response, JBI->GetBuffer());

    response->print(F("\");"));
  }

  if ((page + 1) < total_pages)
  {
    response->printf_P(PSTR("N(%u);"), page + 1);
  }
  else
  {
    response->print(F("N(-1);"));
  }

  request->send(response);
}

#endif // USE_MODULE_NETWORK_WEBSERVER

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
  ptr->flags.topic_type = MQTT_TOPIC_LWT_ONLINE_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_settings_system;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_SYSTEM_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_System;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_settings_network;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_NETWORK_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_Network;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_settings_drivers;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_DRIVERS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_Drivers;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_settings_sensors;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_SENSORS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_Sensors;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_settings_lights;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_LIGHTS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_Lights;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_settings_power;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_POWER_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_Power;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_settings_rules;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_RULES_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_Rules;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_settings_runtime;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_RUNTIME_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_Runtime;
  mqtthandler_list.push_back(ptr);
  
  ptr = &mqtthandler_settings_text_buffer;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_TEXT_BUFFER_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Settings_TextBuffer;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_peripherals;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR;
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_PERIPHERALS_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Peripherals;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_log;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Time;
  mqtthandler_list.push_back(ptr);
    
  ptr = &mqtthandler_taskermanager;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_TASKERMANAGER_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_TaskerManager;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_reboot;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->flags.retain = true;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  ptr->flags.json_level = JSON_LEVEL_ALL;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR;
  ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Reboot;
  mqtthandler_list.push_back(ptr);

  #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
    
    ptr = &mqtthandler_debug_pins_gpio;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = SEC_IN_HOUR; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->flags.json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_GPIO_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Pins_GPIO;
    mqtthandler_list.push_back(ptr);

    ptr = &mqtthandler_debug_pins_table;
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = 10;//SEC_IN_HOUR; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->flags.json_level = JSON_LEVEL_DETAILED;
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
    ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->flags.json_level = JSON_LEVEL_DETAILED;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug_Template;
    mqtthandler_list.push_back(ptr);

    #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_PERFORMANCE
    ptr = &mqtthandler_debug_tasker_interface_performance; 
    ptr->tSavedLastSent = 0;
    ptr->flags.PeriodicEnabled = true;
    ptr->flags.SendNow = true;
    ptr->flags.retain = true;
    ptr->tRateSecs = 5; 
    ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
    ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->flags.json_level = MQTT_TOPIC_TYPE_IFCHANGED_ID;
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
    ptr->flags.topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
    ptr->flags.json_level = MQTT_TOPIC_TYPE_IFCHANGED_ID;
    ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_SETTINGS_STORAGE_CTR;
    ptr->ConstructJSON_function = &mTelemetry::ConstructJSON_Debug__Settings_Storage;
    mqtthandler_list.push_back(ptr);
    #endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
    
  #endif // ENABLE_MQTT_DEBUG_TELEMETRY
  
  #endif // FIRMWARE_MINIMAL2

} 

#endif // USE_MODULE_NETWORK_MQTT








