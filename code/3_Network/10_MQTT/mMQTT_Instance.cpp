#include "mMQTT.h"

#if defined(USE_MODULE_NETWORK_MQTT)

void MQTTConnection::MqttConnected(void)
{
  
  connected = true;
  retry_counter = 0;

  char lwt_message_ondisconnect_ctr[200];
  sprintf_P(lwt_message_ondisconnect_ctr,PSTR("{\"LWT\":\"Offline\",\"reset_reason\":\"%s\",\"uptime\":\"%s\"}"),pCONT_sup->GetResetReason().c_str(),pCONT_time->uptime.hhmmss_ctr);
  
  #ifdef ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT // Show disconnect occured if we have reconnected inside timeout
    pubsub->publish(pCONT_set->Settings.mqtt.lwt_topic,lwt_message_ondisconnect_ctr,true); // onconnect message
    delay(100);
  #endif
  
  Send_LWT_Online();
  
  #ifdef USE_MQTT_RETAINED_VERSION_HISTORY_CHECK // for checking version history
    pubsub->subscribe(("TBD_firmware/set/settings/firmware")); //PSTR failure
  #endif
  
  // Group name for setting all devices
  pubsub->subscribe(("group_all/#"));//(PSTR("group_all/#"));
  
  // subscribe("<devicename>/sync"); //namesake sync name
  psubscribe(PSTR(D_MQTT_SYNC "/#"));
  
  // subscribe("<devicename>/set/#");
  psubscribe(PSTR(D_MQTT_COMMAND "/#"));
  
  cConnectionAttempts = 0; // reset

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT " \"Connection SUCCESS!\""));
  #endif// ENABLE_LOG_LEVEL_INFO
  
  // if succesful, clear flag
  flag_start_reconnect = false;

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT " \"Connected? %d!\""),pubsub->connected());
  #endif //  ENABLE_LOG_LEVEL_INFO

  // DEBUG_LINE;
  pCONT->Tasker_Interface(FUNC_MQTT_CONNECTED);

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT " \"Connected? %d!\""),pubsub->connected());
  #endif //  ENABLE_LOG_LEVEL_INFO
    
  initial_connection_state = 0;

}

void MQTTConnection::Send_LWT_Online()
{
  pubsub->publish(pCONT_set->Settings.mqtt.lwt_topic, WILLMESSAGE_ONCONNECT_CTR, true); // onconnect message
}
    

void MQTTConnection::CheckConnection()
{ DEBUG_PRINT_FUNCTION_NAME;

  if(pubsub==nullptr)
  {// tmp fix, currently showing nullptr every other Tasker
    return;
  }

  if (pCONT_set->Settings.flag_system.mqtt_enabled) 
  {
    
    if (MqttIsConnected()==false) 
    {
      // ALOG_INF(PSTR("MqttIsConnected == FALSE"));

      pCONT_set->global_state.mqtt_down = 1;

      if (retry_counter==0) 
      {
        ALOG_INF(PSTR("retry_counter==0"));
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
      // ALOG_INF(PSTR("MqttIsConnected == TRUE"));
      pCONT_set->global_state.mqtt_down = 0;
      downtime_counter = 0;
    }

  } 
  else 
  {
      
    // AddLog(LOG_LEVEL_TEST, PSTR("========================================= NOT pCONT_set->Settings.flag_system.mqtt_enabled"));
    pCONT_set->global_state.mqtt_down = 0;
    if (initial_connection_state) 
    {
      MqttReconnect();
    }

  } // mqtt_enabled

  /**
   * @brief Restart connection e.g. client changed
   * 
   */
  if(flag_start_reconnect)
  {
    MqttReconnect();
  }

}

void MQTTConnection::EverySecond()
{
  //needs changed to if "network" (wifi/ethernet/gsm)
  // if(pCONT_wif->WifiCheckIpConnected()){
  if(pCONT_interface_network->Connected())
  {
    // AddLog(LOG_LEVEL_TEST, PSTR("IS Connected"));
    CheckConnection();
  }else{
    AddLog(LOG_LEVEL_TEST, PSTR("NOT Connected"));

  }
}


void MQTTConnection::MqttReconnect(void){ DEBUG_PRINT_FUNCTION_NAME;
  
  ALOG_HGL(PSTR(D_LOG_MQTT D_ATTEMPTING_CONNECTION));
  
 
  // allowed = Settings.flag.mqtt_enabled;  // SetOption3 - Enable MQTT
  // if (allowed) {
  //   if (!strlen(SettingsText(SET_MQTT_HOST)) || !Settings.mqtt_port) {
  //     allowed = false;
  //   }
  // }
  // if (!allowed) {
  //   MqttConnected();
  //   return;
  // }

  
  DEBUG_LINE;
  connected = false;
  retry_counter = pCONT_set->Settings.mqtt_retry;
  pCONT_set->global_state.mqtt_down = 1;

  // char *mqtt_user = nullptr;
  // char *mqtt_pwd = nullptr;
  // if (strlen(SettingsText(SET_MQTT_USER))) {
  //   mqtt_user = SettingsText(SET_MQTT_USER);
  // }
  // if (strlen(SettingsText(SET_MQTT_PWD))) {
  //   mqtt_pwd = SettingsText(SET_MQTT_PWD);
  // }

  // GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
  // Response_P(S_LWT_OFFLINE);

  DEBUG_LINE;
  // DEBUG_CHECK_AND_PRINT_NULLPTR(pubsub);
  // If object was created, AND connected was the previous state, disconnect first
  if(pubsub!=nullptr){
    if (pubsub->connected()) { pubsub->disconnect(); }
  }  

  if(pubsub==nullptr)
  {
    ALOG_ERR(PSTR("Wifi or GSM must set client first depending on network connection type"));
    return ;
  }
  
  DEBUG_LINE;

  if (2 == initial_connection_state) {  // Executed once just after power on and wifi is connected
    initial_connection_state = 1;
  }

// change this to use callback in the future again
  pubsub->setCallback(
    [this](char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){
      this->MqttDataHandler(mqtt_topic, mqtt_data, data_len);
    }
  );

  // pubsub->setServer(SettingsText(SET_MQTT_HOST), Settings.mqtt_port);
  //pubsub->setServer(mdns_mqtt_hostname_ctr, 1883);
  //Serial.print("mMQTT:setServer> \""); Serial.print(mdns_mqtt_hostname_ctr); Serial.println("\"");
  // IPAddress mqqtserver(192,168,1,65); //desktop
  // IPAddress mqqtserver(192,168,1,65); //desktop

    
  /**
   * using mdns instead of just IP
   * */
  #ifdef USE_MQTT_MDNS_HOST

  // try mdns, if it fails, then fallback to IP, really try both?
  // if(host_server_type== 0)
  // {
    
  // pubsub->setServer(mqqtserver, 1883);
  // }
  // pubsub->setServer(mqqtserver, 1883);

  // This will require testng with connect()

  
  pubsub->setServer(mqqtserver, 1883);


  #else

#ifndef ENABLE_DEVFEATURE_MQTT_DUAL_WIFI_CONNECTION_TEST2
    #ifdef ENABLE_DEVFEATURE_DDNS_MQTT_TEST

    // pubsub->setServer(D_MQTT_MDNS_DDNS_ADDRESS_URL, 51883); // second broker for external
    pubsub->setServer(D_MQTT_MDNS_DDNS_ADDRESS_URL, D_MQTT_PORT); // main broker

    #else

    IPAddress mqqtserver(D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED);
    pubsub->setServer(mqqtserver, 1883);

    #endif // ENABLE_DEVFEATURE_DDNS_MQTT_TEST

#endif // ENABLE_DEVFEATURE_MQTT_DUAL_WIFI_CONNECTION_TEST


  #endif
    
  
  // Generate will message
  char lwt_message_ondisconnect_ctr[200];
  sprintf_P(lwt_message_ondisconnect_ctr,PSTR("{\"LWT\":\"Offline\",\"reset_reason\":\"%s\",\"uptime\":\"%s\"}"),pCONT_sup->GetResetReason().c_str(),pCONT_time->uptime.hhmmss_ctr);

  uint8_t loglevel = LOG_LEVEL_DEBUG_MORE;
  #ifdef ENABLE_DEVFEATURE_DEBUG_MQTT_RECONNECT
  loglevel = LOG_LEVEL_TEST;
  #endif
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(loglevel, PSTR("client_name = %s"),pCONT_set->Settings.mqtt.client_name);
  AddLog(loglevel, PSTR("lwt_topic = %s"),pCONT_set->Settings.mqtt.lwt_topic);
  AddLog(loglevel, PSTR("lwt_message_ondisconnect_ctr = %s"),lwt_message_ondisconnect_ctr);
  #endif// ENABLE_LOG_LEVEL_INFO

  if(pubsub->connect(pCONT_set->Settings.mqtt.client_name,pCONT_set->Settings.mqtt.lwt_topic,WILLQOS_CTR,WILLRETAIN_CTR,lwt_message_ondisconnect_ctr)){  //boolean connect (clientID, willTopic, willQoS, willRetain, willMessage)
    
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("mMQTT::MqttReconnect Connected"));
    #endif // ENABLE_LOG_LEVEL_INFO
    MqttConnected();
    
    flag_start_reconnect = false;

    
        downtime_counter = 0; //reset

  }
  else
  {
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("mMQTT::MqttReconnect Failed %d"),pubsub->state());
    #endif // 
    MqttDisconnected(pubsub->state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
  }

} // END function


void MQTTConnection::MqttDisconnected(int state)
{
  DEBUG_PRINT_FUNCTION_NAME;
  
  cConnectionAttempts++;    
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Connection FAILED, state = [%s], retrying in %d ms"),state_ctr(),rSavedReconnectAttempt);

  connected = false;
  retry_counter = pCONT_set->Settings.mqtt_retry;

  // pubsub->disconnect();

  // AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND), SettingsText(SET_MQTT_HOST), Settings.mqtt_port, state, retry_counter);
  // rules_flag.mqtt_disconnected = 1;

}




void MQTTConnection::MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){ DEBUG_PRINT_FUNCTION_NAME;

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_TEST, PSTR("MqttDataHandler"));
  // #endif// ENABLE_LOG_LEVEL_INFO

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

  // Method which checks if the data was directly meant for it (not json formatted but raw)
  // if(pCONT->Tasker_Interface(FUNC_MQTT_DATA_ID)){ return; }

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
  pubsub = new PubSubClient(*client_in);
  flag_start_reconnect = true;
  DEBUG_LINE_HERE;
}


// Send packet with payload using va_list
void MQTTConnection::Send_Prefixed_P(const char* topic, PGM_P formatP, ...)
{

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
  va_end(arg);

  ppublish(topic, data_buffer.payload.ctr, false);

}// END function




void MQTTConnection::MQTTHandler_Send_Formatted(uint8_t topic_type, uint16_t module_id, const char* postfix_topic_ctr){

  if(pubsub==nullptr) //can be removed when class constructor includes always creating this
  {
      DEBUG_LINE_HERE;
      return; // to stop crashing as some buffers below are only configured on connection

  }

  if(!pubsub->connected())
  {

  #ifdef ENABLE_DEVFEATURE_MQTT_BLOCK_TRANSMIT_IF_NOT_CONNECTED
    ALOG_ERR(PSTR("BLOCKED: MQTT not connected"));
  #endif
    return; // to stop crashing as some buffers below are only configured on connection
  }

  PGM_P module_ctr = pCONT->GetModuleFriendlyName(module_id);

  // uint8_t loglevel = LOG_LEVEL_DEBUG_MORE;
  // #ifdef ENABLE_DEVFEATURE_DEBUG_MQTT_RECONNECT
  // loglevel = LOG_LEVEL_TEST;
  // #endif
  // loglevel = LOG_LEVEL_INFO;

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(loglevel,PSTR(D_LOG_MQTT "MQTTHandler_Send_Formatted %d %s %s %d"),topic_type,module_ctr,postfix_topic_ctr,pubsub->connected());
  // #endif// ENABLE_LOG_LEVEL_INFO


  publish_ft(module_ctr,
             topic_type,
             postfix_topic_ctr,
             data_buffer.payload.ctr,
             pCONT_set->Settings.sensors.flags.mqtt_retain
            );

  // #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(loglevel,PSTR(D_LOG_MQTT "MQTTHandler_Send_Formatted COMPLETE %d %s %s %d %d"),topic_type,module_ctr,postfix_topic_ctr,pubsub->connected(), strlen(data_buffer.payload.ctr));
  // #endif// ENABLE_LOG_LEVEL_INFO

}


//formatted topic
void MQTTConnection::publish_ft(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
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

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  snprintf_P(topic_ctr, sizeof(topic_ctr), "%s/%s/%s%S", D_TOPIC_STATUS,module_name,topic_id_ctr,topic_postfix);  //PSTR may broke this?

  #ifdef ENABLE_DEBUG_TRACE__MQTT_TOPIC_AS_TRASNMITTED
  ALOG_TRA( PSTR(D_LOG_MQTT "topic_ctr=\"%s\""), topic_ctr );
  #endif
  
  ppublish(topic_ctr,payload_ctr,retain_flag);

}

 
void MQTTConnection::publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  sprintf_P(topic_ctr,PSTR("%s/%s/%s"),D_TOPIC_STATUS,module_name,topic_postfix); 
  
  ppublish(topic_ctr,payload_ctr,retain_flag);

}

// My function for adding prefix by device name
boolean MQTTConnection::ppublish(const char* topic, const char* payload, boolean retained)
{

  if(pubsub == nullptr)
  {
    ALOG_ERR(PSTR("pubsub == nullptr"));
    return false;
  }

  if(!pCONT_interface_network->Connected())
  {
    ALOG_ERR(PSTR(D_LOG_PUBSUB "Unable to publish, No Network connection"));
    return false;
  }

  if (!pubsub->connected()) 
  {    
    ALOG_ERR(PSTR(D_LOG_PUBSUB "Unable to publish, No Broker connection"));
    flag_start_reconnect = true;
    return false;
  }

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
 * @brief progmem payload version 
 * 
 * @param topic 
 * @param payload 
 * @param retained 
 * @return boolean 
 */
// My function for adding prefix by device name
boolean MQTTConnection::ppublish_device_name_prefix_P(const char* topic, const char* payload, boolean retained){

  //// AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "!pubsub->connected() BEFORE "));
    if (!pubsub->connected()) {
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "NOT CONNECTED \"ppublish\" failed!"));
      #endif// ENABLE_LOG_LEVEL_INFO
      flag_start_reconnect = true;
      return false;
    }
    
    if(strlen_P(payload)<1){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "strlen(payload)<1"));
    #endif// ENABLE_LOG_LEVEL_INFO
    }

// Serial.println(WiFi.localIP());
// Serial.println(static_cast<uint32_t>(WiFi.localIP())); 

#ifdef USE_MODULE_NETWORK_WIFI
#ifndef DISABLE_DEVFEATURE_NETWORK_WIFI
    if (!pCONT_wif->WifiCheckIpConnected()) {
    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early pp"));
    #endif// ENABLE_LOG_LEVEL_INFO
        return false;
    }else{
        //AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "WiFi.status()=%d"),WiFi.status());
    }

if(WiFi.status() != WL_CONNECTED){ 
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early"));
    #endif// ENABLE_LOG_LEVEL_INFO
  return false;
}

#endif // DISABLE_DEVFEATURE_NETWORK_WIFI
#endif // USE_MODULE_NETWORK_WIFI
DEBUG_LINE;

//return 0;//

    char convctr[100]; memset(convctr,0,sizeof(convctr));
    //Serial.println(pCONT_set->Settings.mqtt.prefixtopic); Serial.flush();
    // Serial.println(topic); Serial.flush();
    snprintf(convctr,sizeof(convctr),PSTR("%s/%S"),pCONT_set->Settings.mqtt.prefixtopic,topic);
    
    // Serial.println(convctr); Serial.flush();
DEBUG_LINE;
//TRACE();
    // #ifdef ENABLE_LOG_LEVEL_INFO
    // ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_TOPIC " [%s] %d"),convctr,strlen(convctr));
    // ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_PAYLOAD " [%s] %d"),payload,strlen(payload));
    // #endif// ENABLE_LOG_LEVEL_INFO

DEBUG_LINE;
// //TRACE();
//     #ifdef SERIAL_DEBUG_LOW_LEVEL
//     if(strstr(payload,"{}")){
//         ALOG_DBM( PSTR(D_LOG_PUBSUB D_ERROR "> {}"));
//     }
//     #endif

DEBUG_LINE;
if(pubsub!=nullptr){
  DEBUG_LINE;
    return pubsub->publish_P(convctr, payload, retained);
    
    
    // (const uint8_t*)payload,strlen(payload),retained);
  }
  DEBUG_LINE;
  return 0;
  //TRACE();
}

#endif // USE_MODULE_NETWORK_MQTT