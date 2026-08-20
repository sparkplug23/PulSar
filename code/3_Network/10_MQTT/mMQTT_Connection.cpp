#include "mMQTT.h"

#ifdef USE_MODULE_NETWORK_MQTT

void MQTTConnection::MqttConnected(void)
{
  
  connected = true;
  retry_counter = 0;
  cConnectionAttempts = 0; // reset

  char lwt_message_ondisconnect_ctr[200];
  char buffer[40];
  sprintf_P(lwt_message_ondisconnect_ctr, PM_MQTT_LWT_PAYLOAD_FORMATED, tkr_sup->GetResetReason().c_str(), tkr_time->GetUptime(buffer,sizeof(buffer)) );
  
  #ifdef ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT // Show disconnect occured if we have reconnected inside timeout
    char lwt_topic[50];
    snprintf_P(lwt_topic, sizeof(lwt_topic), PSTR("%s/status/LWT"), prefix_topic);
    pubsub->publish(lwt_topic, lwt_message_ondisconnect_ctr, true); // onconnect message
    delay(100);
  #endif
  
  Send_LWT_Online();
    
  // Group name for setting all devices
  subscribe(PSTR("group_all/#"));
    
  // subscribe("<devicename>/set/#");
  subscribe_device(PSTR(D_MQTT_COMMAND "/#"));
  
  flag_start_reconnect = false;
  
  #ifndef ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
  tkr->Tasker_Interface(TASK_MQTT_CONNECTED);
  tkr->Tasker_Interface(TASK_MQTT_SUBSCRIBE);
  tkr->Tasker_Interface(TASK_TELEMETRY_REFRESH_SEND_ALL);
  #endif

}


void MQTTConnection::Send_LWT_Online()
{
  if(!pubsub)
  {
    ALOG_ERR(PSTR(D_LOG_MQTT "Send_LWT_Online failed: pubsub client not set"));
    return;
  }
  char lwt_topic[80]; snprintf_P(lwt_topic, sizeof(lwt_topic), PM_MQTT_LWT_TOPIC_FORMATED, prefix_topic);
  char payload[40];   snprintf_P(payload, sizeof(payload), PM_MQTT_LWT_PAYLOAD_ONLINE); // Required for ESP8266
  pubsub->publish(lwt_topic, payload, true);
}
    

void MQTTConnection::EverySecond()
{
  
  #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
  ALOG_HGL(PSTR("MQTTConnection::CheckConnection \"%s\""), host_address);
  #endif
  
  if (MqttIsConnected()==false) 
  {

    #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    ALOG_DBG(PSTR("MqttIsConnected == FALSE"));
    #endif

    tkr_set->runtime.global_state.mqtt_down = 1;
    uptime_seconds = 0;
    downtime_counter++;

    if(retry_counter == 0)
    {
      ALOG_DBM(PSTR("reconnect disabled, will be enabled elsewhere"));
    }
    else
    if (retry_counter==1) 
    {
      ALOG_DBG(PSTR(D_LOG_MQTT "retry_counter==1"));
      MqttReconnect();
    }
    else
    {
      retry_counter--;
      ALOG_DBG( PSTR(D_LOG_MQTT "retry_counter=%d"), retry_counter );
    }

  } 
  else 
  {
    #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    ALOG_DBG(PSTR("MqttIsConnected == TRUE"));
    #endif 
    tkr_set->runtime.global_state.mqtt_down = 0;
    downtime_counter = 0;
    uptime_seconds++;
  }  

}


void MQTTConnection::MqttReconnect(void)
{
  DEBUG_PRINT_FUNCTION_NAME;

  uint32_t before_millis = millis();

  const bool valid_config = en && allowed && host_address[0] && port && client_name[0] && network_client && pubsub;

  if(!valid_config)
  {
    ALOG_ERR(PSTR(D_LOG_MQTT "Reconnect blocked id=%s en=%u allowed=%u host=%s port=%u client=%s net=%p pubsub=%p"),
      id, en, allowed, host_address, port, client_name, network_client, pubsub);

    #ifdef DEBUG_MQTT_RECONNECTS
    ALOG_ERR(PSTR(D_LOG_MQTT "Reconnect debug retry=%u retry_counter=%u retry_start=%u client_type=%u connected=%u flag=%u"),
      retry, retry_counter, retry_counter_start_value, client_type, connected, flag_start_reconnect);
    #endif

    connected = false;
    flag_start_reconnect = false;
    retry_counter = 0;
    return;
  }

  const bool has_user = user[0] != '\0';
  const bool has_password = password[0] != '\0';
  const bool is_secure_connection = has_user && has_password;

  ALOG_INF(PSTR(D_LOG_MQTT "Attempting %ssecure connection to %s:%u client=%s"),
    is_secure_connection ? "" : "un", host_address, port, client_name);

  connected = false;
  retry_counter = retry_counter_start_value;
  tkr_set->runtime.global_state.mqtt_down = 1;

  if(pubsub->connected())
  {
    ALOG_INF(PSTR(D_LOG_MQTT "Disconnecting previous MQTT session"));
    pubsub->disconnect();
  }

  pubsub->setCallback(
    [this](char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){
      this->MqttDataHandler(mqtt_topic, mqtt_data, data_len);
    }
  );

  pubsub->setServer(host_address, port);

  char lwt_message_ondisconnect_ctr[200] = {0};
  char buffer[40] = {0};
  sprintf_P(lwt_message_ondisconnect_ctr, PM_MQTT_LWT_PAYLOAD_FORMATED,tkr_sup->GetResetReason().c_str(), tkr_time->GetUptime(buffer, sizeof(buffer)));

  char lwt_topic[50] = {0};
  snprintf_P(lwt_topic, sizeof(lwt_topic), PM_MQTT_LWT_TOPIC_FORMATED,tkr_set->Settings.system_name.device);

  if(is_secure_connection)
  {
    ALOG_INF(PSTR(D_LOG_MQTT "Credentials user=%s pw=%.3s*** lwt=%s"), user, password, lwt_topic);
    connected = pubsub->connect(client_name, user, password, lwt_topic, WILLQOS_CTR, WILLRETAIN_CTR, lwt_message_ondisconnect_ctr);
  }
  else
  {
    ALOG_INF(PSTR(D_LOG_MQTT "Credentials none lwt=%s"), lwt_topic);
    connected = pubsub->connect(client_name, lwt_topic, WILLQOS_CTR, WILLRETAIN_CTR, lwt_message_ondisconnect_ctr);
  }

  const int mqtt_state = pubsub->state();
  const uint32_t elapsed_millis = millis() - before_millis;

  if(connected)
  {
    ALOG_INF(PSTR(D_LOG_MQTT "Reconnect success state=%d elapsed=%ums"), mqtt_state, elapsed_millis);
    MqttConnected();
    flag_start_reconnect = false;
    retry_counter = 0;
    downtime_counter = 0;
  }
  else
  {
    ALOG_ERR(PSTR(D_LOG_MQTT "Reconnect failed state=%d elapsed=%ums"), mqtt_state, elapsed_millis);
    MqttDisconnected(mqtt_state);
  }
}


void MQTTConnection::MqttDisconnected(int state)
{
  DEBUG_PRINT_FUNCTION_NAME;
  
  cConnectionAttempts++;    

  ALOG_DBG(PSTR(D_LOG_MQTT "Connection FAILED, state = [%d]"), pubsub->state());

  connected = false;
  retry_counter = retry_counter_start_value;
  
}


void MQTTConnection::MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len)
{ 
  
  if (data_len >= DATA_BUFFER_PAYLOAD_MAX_LENGTH) 
  {  
    ALOG_ERR(PSTR("MqttDataHandler"));
    return;
  }
  
  if(data_buffer.requestLock(D_UNIQUE_MODULE_NETWORK_MQTT_ID))
  {

    data_buffer.ClearSoft();

    data_buffer.topic.length_used = strlen(mqtt_topic);
    strlcpy(data_buffer.topic.ctr, mqtt_topic, data_buffer.topic.length_used);
    data_buffer.topic.ctr[data_buffer.topic.length_used] = '\0'; // null terminate

    data_buffer.payload.length_used = data_len;
    memcpy(data_buffer.payload.ctr, mqtt_data, data_buffer.payload.length_used);
    data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // null terminate

    if(data_len){
      data_buffer.flags.waiting = true;
    }

    if(data_buffer.flags.waiting)
    {
      data_buffer.flags.waiting = false;
      // if (LOG_LEVEL_DEBUG_MORE <= tkr_set->Settings.logging.serial_level) {
        LoggingLevels level = LOG_LEVEL_DEBUG_MORE;
        #ifdef ENABLE_DEVFEATURE_SHOW_INCOMING_MQTT_COMMANDS
        level = LOG_LEVEL_DEV_TEST;
        #endif
        #ifdef ENABLE_LOG_LEVEL_INFO
          AddLog(level, PSTR(D_LOG_MQTT "<-- Topic   [len:%d] %s"), data_buffer.topic.length_used,  data_buffer.topic.ctr);
          AddLog(level, PSTR(D_LOG_MQTT "<-- Payload [len:%d] %s"), data_buffer.payload.length_used,data_buffer.payload.ctr);
        #endif// ENABLE_LOG_LEVEL_INFO
      // }

      data_buffer.isserviced = 0;

      #ifdef ENABLE_DEVFEATURE_MQTT__ESTIMATED_INCOMING_COMMANDS_AND_REPORT_ISSERVICED
      uint16_t estimated_commands = JBI->estimateJsonKeyValuePairs(data_buffer.payload.ctr, data_buffer.payload.length_used);
      // ALOG_INF(PSTR(D_LOG_MQTT "<-- Payload [len:%d] %s"), data_buffer.payload.length_used,data_buffer.payload.ctr);
      ALOG_DBM(PSTR("estimated_commands %d"), estimated_commands);
      #endif

      tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);
      
      ALOG_DBM( PSTR(D_LOG_MQTT "{\"CommandsMatched\":%d}"), data_buffer.isserviced);

      if(data_buffer.isserviced != estimated_commands)
      {
        uint8_t log_level = LOG_LEVEL_WARNING;
        #ifdef USE_MODULE_DISPLAYS_NEXTION
        log_level = LOG_LEVEL_DEBUG_MORE; // Don't report when nextion is active
        #endif
        AddLog(log_level, PSTR("Commands mismatch %d/%d"), data_buffer.isserviced, estimated_commands);
      }
      
    }
    data_buffer.releaseLock();

  }

}

boolean MQTTConnection::subscribe(const char* topic) {  
  char ttopic[70] = {0};
  strncpy_P(ttopic, topic, strlen(topic));
  ALOG_INF(PSTR(D_LOG_MQTT "Subscribing to \"%s\""), ttopic );
  if(pubsub)
    return pubsub->subscribe(ttopic, 0); // Expects topic in RAM (not PSTR)
}

boolean MQTTConnection::subscribe_device(const char* topic) {
  char ttopic[70] = {0};
  sprintf(ttopic, PSTR("%s/%s"), prefix_topic, topic);
  ALOG_INF(PSTR(D_LOG_MQTT "Subscribing to \"%s\""), ttopic );
  return pubsub->subscribe(ttopic, 0);
}


void MQTTConnection::SetPubSubClient(Client* client_in)
{
  if(!client_in)
  {
    ALOG_ERR(PSTR(D_LOG_MQTT "SetPubSubClient failed: client=null"));
    ClearPubSubClient();
    return;
  }

  if(pubsub)
  {
    delete pubsub;
    pubsub = nullptr;
  }

  network_client = client_in;
  pubsub = new PubSubClient(*client_in);

  if(!pubsub)
  {
    ALOG_ERR(PSTR(D_LOG_MQTT "SetPubSubClient allocation failed"));
    network_client = nullptr;
    return;
  }

  flag_start_reconnect = true;
}

void MQTTConnection::Send_Prefixed_P(const char* topic, PGM_P formatP, ...)
{

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
  va_end(arg);

  publish_device(topic, data_buffer.payload.ctr, false);

}


/**
 * @brief 
 * 
 * @param topic_type 
 * @param module_id 
 * @param postfix_topic_ctr 
 * @return true  if packet was sent
 * @return false if not successful
 */
bool MQTTConnection::MQTTHandler_Send_Formatted_UniqueID(uint8_t topic_type, uint16_t unique_id, const char* postfix_topic_ctr, bool retain_flag)
{

  PGM_P module_ctr = tkr->GetModuleName(unique_id);

  #ifdef ENABLE_DEBUG_TRACE__SERIAL_PRINT_MQTT_MESSAGE_OUT_BEFORE_FORMING
  Serial.printf("buffer length = %d\n\r", strlen(data_buffer.payload.ctr));
  #endif

  bool sent_status = false;

  sent_status =  publish_ft(module_ctr,
             topic_type,
             postfix_topic_ctr,
             data_buffer.payload.ctr,
             retain_flag
            );

  if(sent_status)
  {
    tSaved_LastOutGoingTopic = millis();
  }else{
    ALOG_ERR(PSTR(D_LOG_MQTT "MQTTHandler_Send_Formatted_UniqueID failed"));
  }

  return sent_status;

}


bool MQTTConnection::publish_ft(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic    [100]; memset(topic,0,sizeof(topic));
  char topic_type[30]; memset(topic_type,0,sizeof(topic_type));

  if(topic_type_id == MQTT_TOPIC_LWT_ONLINE_ID)
  {
    snprintf_P(topic, sizeof(topic), "%s/LWT", D_TOPIC_STATUS);
  }
  else
  {    
    switch(topic_type_id){
      default:    
      case MQTT_TOPIC_TYPE_IFCHANGED_ID:  sprintf(topic_type,"ifchanged/"); break;
      // case MQTT_TOPIC_TYPE_ROC1M_ID:      sprintf(topic_type,"roc1m/");     break;
      // case MQTT_TOPIC_TYPE_ROC10M_ID:     sprintf(topic_type,"roc10m/");    break;
      case MQTT_TOPIC_TYPE_TELEPERIOD_ID: sprintf(topic_type,"tele/");      break;
      case MQTT_TOPIC_TYPE__DEBUG__ID:    sprintf(topic_type,"debug/");     break;
      case MQTT_TOPIC_TYPE_SYSTEM_ID:      /*** none ***/                   break;
    }
    snprintf_P(topic, sizeof(topic), "%s/%s/%s%S", D_TOPIC_STATUS, module_name, topic_type, topic_postfix); // PSTR around string will crash
  }

  #ifdef ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  ALOG_DBG( PSTR(D_LOG_MQTT "topic=\"%s\""), topic );
  #endif
  #ifdef ENABLE_DEBUG_TRACE__MQTT_PAYLOAD_AS_TRANSMITTED
  ALOG_DBG( PSTR(D_LOG_MQTT "payload=\"%s\""), payload_ctr );
  #endif
  
  return publish_device(topic, payload_ctr, retain_flag);

}

 
void MQTTConnection::publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  sprintf_P(topic_ctr,PSTR("%s/%s/%s"),D_TOPIC_STATUS,module_name,topic_postfix); 
  
  publish_device(topic_ctr,payload_ctr,retain_flag);

}


boolean MQTTConnection::publish_device(const char* topic, const char* payload, boolean retained)
{
  #ifdef ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  Serial.println("MQTTConnection::publish_device"); Serial.flush();
  #endif

  if(!topic || !payload)
  {
    ALOG_ERR(PSTR(D_LOG_PUBSUB "Unable to publish, null topic/payload"));
    return false;
  }

  if(!network_client)
  {
    ALOG_WRN(PSTR(D_LOG_PUBSUB "Unable to publish, network_client=null id=%s"),id);
    connected = false;
    flag_start_reconnect = true;
    return false;
  }

  if(!pubsub)
  {
    ALOG_WRN(PSTR(D_LOG_PUBSUB "Unable to publish, pubsub=null id=%s"),id);
    connected = false;
    flag_start_reconnect = true;
    return false;
  }

  if(!strlen(payload))
  {
    ALOG_ERR(PSTR(D_LOG_PUBSUB "Unable to publish, empty payload"));
    return false;
  }

  /*
   * Check the MQTT session before publishing.
   *
   * Do not use network_client->connected() here as the sole test. For
   * TinyGsmClient/WiFiClient that reports transport/socket state, while
   * PubSubClient also owns MQTT session state.
   */
  if(!pubsub->connected())
  {
    ALOG_WRN(PSTR(D_LOG_PUBSUB "Unable to publish, MQTT disconnected id=%s net=%p pubsub=%p"),id,network_client,pubsub);
    connected = false;
    flag_start_reconnect = true;
    return false;
  }

  char convctr[100] = {0};
  snprintf(convctr,sizeof(convctr),PSTR("%s/%S"),prefix_topic,topic);

  #ifdef ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  ALOG_DBG(PSTR(D_LOG_PUBSUB "-->" D_TOPIC "%d [%s]"),strlen(convctr),convctr);
  ALOG_DBG(PSTR(D_LOG_PUBSUB "-->" D_PAYLOAD "%d [%s]"),strlen(payload),payload);
  #endif

  return pubsub->publish(convctr,(const uint8_t*)payload,strlen(payload),retained);
}


/**
 * @brief progmem payload version  My function for adding prefix by device name
 * 
 * @param topic 
 * @param payload 
 * @param retained 
 * @return boolean 
 */
boolean MQTTConnection::publish_device_P(const char* topic, const char* payload, boolean retained){

  char convctr[100] = {0};
  snprintf(convctr,sizeof(convctr),PSTR("%s/%S"), prefix_topic, topic);
  return pubsub->publish_P(convctr, payload, retained);
    
}

void MQTTConnection::ClearPubSubClient()
{
  if(pubsub)
  {
    delete pubsub;
    pubsub = nullptr;
  }

  network_client = nullptr;
  connected = false;
  flag_start_reconnect = false;
  retry_counter = 0;
}

#endif // USE_MODULE_NETWORK_MQTT