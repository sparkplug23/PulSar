#include "mMQTT.h"

#ifdef USE_MODULE_NETWORK_MQTT

void MQTTConnection::MqttConnected(void)
{
  
  connected = true;
  retry_counter = 0;
  cConnectionAttempts = 0; // reset

  char lwt_message_ondisconnect_ctr[200];
  sprintf_P(lwt_message_ondisconnect_ctr,PSTR("{\"LWT\":\"Offline\",\"ResetReason\":\"%s\",\"Uptime\":\"%s\"}"), pCONT_sup->GetResetReason().c_str(), pCONT_time->uptime.hhmmss_ctr);
  
  #ifdef ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT // Show disconnect occured if we have reconnected inside timeout
    pubsub->publish(pCONT_set->Settings.mqtt.lwt_topic, lwt_message_ondisconnect_ctr, true); // onconnect message
    delay(100);
  #endif
  
  Send_LWT_Online();
  
  #ifdef USE_MQTT_RETAINED_VERSION_HISTORY_CHECK // for checking version history
    psubscribe(PSTR("TBD_firmware/set/settings/firmware"));
  #endif
  
  // Group name for setting all devices
  psubscribe(PSTR("group_all/#"));
  
  // subscribe("<devicename>/sync"); //namesake sync name
  psubscribe(PSTR(D_MQTT_SYNC "/#"));
  
  // subscribe("<devicename>/set/#");
  psubscribe(PSTR(D_MQTT_COMMAND "/#"));

  // if succesful, clear flag
  flag_start_reconnect = false;
  
  #ifndef ENABLE_DEVFEATURE__MQTT_STOP_SENDING_EVERYTHING_ON_RECONNECT
  pCONT->Tasker_Interface(FUNC_MQTT_CONNECTED);
  #endif

}


void MQTTConnection::Send_LWT_Online()
{
  pubsub->publish(pCONT_set->Settings.mqtt.lwt_topic, WILLMESSAGE_ONCONNECT_CTR, true);
}
    

void MQTTConnection::EverySecond()
{

  #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
  ALOG_INF(PSTR("MQTTConnection::CheckConnection \"%s\""), broker_url);
  #endif
  
  if (MqttIsConnected()==false) 
  {

    #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    ALOG_INF(PSTR("MqttIsConnected == FALSE"));
    #endif

    pCONT_set->global_state.mqtt_down = 1;
    uptime_seconds = 0;
    downtime_counter++;

    if(retry_counter == 0)
    {
      // reconnect disabled, will be enabled elsewhere
      ALOG_INF(PSTR("reconnect disabled, will be enabled elsewhere"));
    }
    else
    if (retry_counter==1) 
    {
      ALOG_INF(PSTR("retry_counter==1"));
      MqttReconnect();
    } 
    else 
    {
      retry_counter--;
      ALOG_INF( PSTR("retry_counter=%d"), retry_counter );
    }

  } 
  else 
  {
    #ifdef ENABLE_DEBUGFEATURE__LOGGING_MQTT__CHECK_CONNECTION
    ALOG_INF(PSTR("MqttIsConnected == TRUE"));
    #endif
    pCONT_set->global_state.mqtt_down = 0;
    downtime_counter = 0;
    uptime_seconds++;
  }  

}


void MQTTConnection::MqttReconnect(void){ DEBUG_PRINT_FUNCTION_NAME;
  
  uint32_t before_millis = millis();

  ALOG_HGL(PSTR(D_LOG_MQTT D_ATTEMPTING_CONNECTION " to \"%s:%d\""), broker_url, port);
  
  connected = false;
  retry_counter = pCONT_set->Settings.mqtt_retry;
  pCONT_set->global_state.mqtt_down = 1;

  if(pubsub!=nullptr)
  {
    if (pubsub->connected()) { pubsub->disconnect(); }
  }  

  pubsub->setCallback(
    [this](char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){
      this->MqttDataHandler(mqtt_topic, mqtt_data, data_len);
    }
  );

  pubsub->setServer(broker_url, port); // main broker
  
  // Generate will message
  char lwt_message_ondisconnect_ctr[200];
  sprintf_P(lwt_message_ondisconnect_ctr,PSTR("{\"LWT\":\"Offline\",\"ResetReason\":\"%s\",\"Uptime\":\"%s\"}"),pCONT_sup->GetResetReason().c_str(),pCONT_time->uptime.hhmmss_ctr);

  uint8_t loglevel = LOG_LEVEL_DEBUG_MORE;
  #ifdef ENABLE_DEVFEATURE_DEBUG_MQTT_RECONNECT
  loglevel = LOG_LEVEL_TEST;
  #endif
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(loglevel, PSTR("client_name = %s"),pCONT_set->Settings.mqtt.client_name);
  AddLog(loglevel, PSTR("lwt_topic = %s"),pCONT_set->Settings.mqtt.lwt_topic);
  AddLog(loglevel, PSTR("lwt_message_ondisconnect_ctr = %s"),lwt_message_ondisconnect_ctr);
  #endif// ENABLE_LOG_LEVEL_INFO

  AddLog(LOG_LEVEL_INFO, PSTR("mMQTT::MqttReconnect START                 Connect"));

  if(pubsub->connect(pCONT_set->Settings.mqtt.client_name,pCONT_set->Settings.mqtt.lwt_topic,WILLQOS_CTR,WILLRETAIN_CTR,lwt_message_ondisconnect_ctr)){  //boolean connect (clientID, willTopic, willQoS, willRetain, willMessage)
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("mMQTT::MqttReconnect Connected"));
    #endif // ENABLE_LOG_LEVEL_INFO
    MqttConnected();
    
    flag_start_reconnect = false;
    retry_counter = 0; // disables it running again

    downtime_counter = 0; //reset

  }
  else
  {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("mMQTT::MqttReconnect Failed %d"),pubsub->state());
    #endif // 
    MqttDisconnected(pubsub->state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
  }
  
  uint32_t elapsed_millis = millis() - before_millis;

  ALOG_INF(PSTR("MqttReconnect elapsed_millis = %d"), elapsed_millis);

} // END function


void MQTTConnection::MqttDisconnected(int state)
{
  DEBUG_PRINT_FUNCTION_NAME;
  
  cConnectionAttempts++;    

  // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Connection FAILED, state = [%s], retrying in %d ms"),state_ctr(),rSavedReconnectAttempt);

  connected = false;
  retry_counter = pCONT_set->Settings.mqtt_retry; // begin reconnect phase

  // pubsub->disconnect();

  // AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND), SettingsText(SET_MQTT_HOST), Settings.mqtt_port, state, retry_counter);
  // rules_flag.mqtt_disconnected = 1;

}




void MQTTConnection::MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){ DEBUG_PRINT_FUNCTION_NAME;

  // Do not allow more data than would be feasable within stack space
  if (data_len >= MQTT_MAX_PACKET_SIZE) { return; }

  // Save MQTT data ASAP as it's data is discarded by PubSubClient with next publish as used in MQTTlog
  D_DATA_BUFFER_CLEAR();
  data_buffer.topic.len = strlen(mqtt_topic)+1;
  strlcpy(data_buffer.topic.ctr, mqtt_topic, data_buffer.topic.len);
  data_buffer.payload.len = data_len;
  memcpy(data_buffer.payload.ctr, mqtt_data, sizeof(char)*data_buffer.payload.len);

  if(data_len){
    data_buffer.flags.waiting = true;
  }

  if(data_buffer.flags.waiting){data_buffer.flags.waiting = false;
    // if (LOG_LEVEL_DEBUG_MORE <= pCONT_set->Settings.seriallog_level) {
      LoggingLevels level = LOG_LEVEL_DEBUG_MORE;
      #ifdef ENABLE_DEVFEATURE_SHOW_INCOMING_MQTT_COMMANDS
      level = LOG_LEVEL_TEST;
      #endif
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(level, PSTR(D_LOG_MQTT "<-- NEWTopic   [len:%d] %s"),data_buffer.topic.len,  data_buffer.topic.ctr);
      AddLog(level, PSTR(D_LOG_MQTT "<-- NEWPayload [len:%d] %s"),data_buffer.payload.len,data_buffer.payload.ctr);
    #endif// ENABLE_LOG_LEVEL_INFO
    // }

    pCONT->Tasker_Interface(FUNC_JSON_COMMAND_ID);
    
    ALOG_COM( PSTR(D_LOG_MQTT "{\"CommandsMatched\":%d}"),data_buffer.isserviced);
    
  }

}


boolean MQTTConnection::psubscribe(const char* topic) {
  char ttopic[70];
  memset(ttopic,0,sizeof(ttopic));
  sprintf(ttopic,PSTR("%s/%s"),pCONT_set->Settings.mqtt.prefixtopic,topic);
  ALOG_INF( PSTR("Subscribing to \"%s\""), ttopic );
  return pubsub->subscribe(ttopic, 0);
}


/**
 * @brief 
 * 
 * @param client_in 
 */
void MQTTConnection::SetPubSubClient(Client* client_in)
{
      Serial.printf("MQTT::SetPubSubClient A\n\r"); Serial.flush();
  pubsub = new PubSubClient(*client_in);
      Serial.printf("MQTT::SetPubSubClient B\n\r"); Serial.flush();
  flag_start_reconnect = true;
  DEBUG_LINE_HERE;
}


void MQTTConnection::Send_Prefixed_P(const char* topic, PGM_P formatP, ...)
{

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
  va_end(arg);

  ppublish(topic, data_buffer.payload.ctr, false);

}// END function


/**
 * @brief 
 * 
 * @param topic_type 
 * @param module_id 
 * @param postfix_topic_ctr 
 * @return true  if packet was sent
 * @return false if not successful
 */
bool MQTTConnection::MQTTHandler_Send_Formatted(uint8_t topic_type, uint16_t module_id, const char* postfix_topic_ctr)
{

  PGM_P module_ctr = pCONT->GetModuleFriendlyName(module_id);

  bool sent_status = false;

  sent_status =  publish_ft(module_ctr,
             topic_type,
             postfix_topic_ctr,
             data_buffer.payload.ctr,
             pCONT_set->Settings.sensors.flags.mqtt_retain
            );

  if(sent_status)
  {
    tSaved_LastOutGoingTopic = millis();
  }

  return sent_status;

}



bool MQTTConnection::publish_ft(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));

  // pCONT_mqtt->TopicFormatted(module_name, topic_type_id, topic_postfix, topic_ctr, sizeof(topic_ctr));  // Needs fixed

  if(topic_type_id == MQTT_TOPIC_LWT_ONLINE_ID)
  {

    // case MQTT_TOPIC_TYPE_LWT_ONLINE_ID: sprintf(topic_id_ctr,"%s/",MQTT_TOPIC_TYPE_IFCHANGED_CTR); break;

    snprintf_P(topic_ctr, sizeof(topic_ctr), "%s/LWT", D_TOPIC_STATUS);//, D_TOPIC_STATUS, module_name, topic_id_ctr, topic_postfix);  //PSTR may broke this?
  }
  else
  {
    //can be replaced with a function call
    switch(topic_type_id){
      case MQTT_TOPIC_TYPE_SYSTEM_ID: break; // nothing
      case MQTT_TOPIC_TYPE_IFCHANGED_ID: sprintf(topic_id_ctr,"%s/",MQTT_TOPIC_TYPE_IFCHANGED_CTR); break;
      case MQTT_TOPIC_TYPE_ROC1M_ID: sprintf(topic_id_ctr,"%s/","roc1m"); break;
      case MQTT_TOPIC_TYPE_ROC10M_ID: sprintf(topic_id_ctr,"%s/","roc10m"); break;
      case MQTT_TOPIC_TYPE_TELEPERIOD_ID: sprintf(topic_id_ctr,"%s/",MQTT_TOPIC_TYPE_TELEPERIOD_CTR); break;
      case MQTT_TOPIC_TYPE__DEBUG__ID: sprintf(topic_id_ctr,"%s/","debug"); break;
      default: sprintf(topic_id_ctr,"%s/","ERROR"); break;
    }

    snprintf_P(topic_ctr, sizeof(topic_ctr), "%s/%s/%s%S", D_TOPIC_STATUS, module_name, topic_id_ctr, topic_postfix);  //PSTR may broke this?

  }
  #ifdef ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  ALOG_TRA( PSTR(D_LOG_MQTT "topic_ctr=\"%s\""), topic_ctr );
  #endif
  
  return ppublish(topic_ctr,payload_ctr,retain_flag);

}

 
void MQTTConnection::publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  sprintf_P(topic_ctr,PSTR("%s/%s/%s"),D_TOPIC_STATUS,module_name,topic_postfix); 
  
  ppublish(topic_ctr,payload_ctr,retain_flag);

}


boolean MQTTConnection::ppublish(const char* topic, const char* payload, boolean retained)
{

  /**
   * @brief THIS SHOULD BE THE ONLY checks of network.
   * In reality wifi/cellular should always be checking itself directly
   * 
   */
  #ifdef ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS
  if(network_client->connected())
  {
    #ifdef ENABLE_DEVFEATURE__MQTT_SPLASH_CONNECTION_STATUS_BEFORE_SENDING
    ALOG_ERR(PSTR(D_LOG_PUBSUB "MQTTConnection::ppublish::network->Connected"));
    #endif
  }else{
    ALOG_ERR(PSTR(D_LOG_PUBSUB "-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- Unable to publish, No Network connection B"));
    flag_start_reconnect = true;
    return false;
  }

  #endif // ENABLE_DEVFEATURE__MQTT_CLEANING_UP_MANY_NETWORK_CHECKS

  if(strlen(payload)<1)
  {
    ALOG_ERR(PSTR(D_LOG_PUBSUB "Unable to publish, empty payload"));
    return false;
  }

  char convctr[100]; memset(convctr,0,sizeof(convctr));
  snprintf(convctr,sizeof(convctr),PSTR("%s/%S"),pCONT_set->Settings.mqtt.prefixtopic,topic);
  
  ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_TOPIC " [%s] %d"),convctr,strlen(convctr));
  ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_PAYLOAD " [%s] %d"),payload,strlen(payload));

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
boolean MQTTConnection::ppublish_device_name_prefix_P(const char* topic, const char* payload, boolean retained){

  char convctr[100]; memset(convctr,0,sizeof(convctr));
  snprintf(convctr,sizeof(convctr),PSTR("%s/%S"),pCONT_set->Settings.mqtt.prefixtopic,topic);
  return pubsub->publish_P(convctr, payload, retained);
    
}

#endif // USE_MODULE_NETWORK_MQTT