#include "mMQTT_Cellular.h"

#ifdef USE_MODULE_NETWORK_MQTT_CELLULAR


const char* mMQTT_Cellular::PM_MODULE_NETWORK_MQTT_CELLULAR_CTR = D_MODULE_NETWORK_MQTT_CELLULAR_CTR;
const char* mMQTT_Cellular::PM_MODULE_NETWORK_MQTT_FRIENDLY_CELLULAR_CTR = D_MODULE_NETWORK_MQTT_FRIENDLY_CELLULAR_CTR;


int8_t mMQTT_Cellular::Tasker(uint8_t function, JsonParserObject obj){ DEBUG_PRINT_FUNCTION_NAME;

  // DEBUG_PRINT_FUNCTION_NAME_TEST;

  // #ifdef DISABLE_NETWORK
    // return 0;
  // #endif

  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_INIT:
      init();
    break;
  }

  if(pCONT_set->Settings.flag_system.mqtt_enabled){

    switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
      case FUNC_LOOP:
        if(Mqtt.connected){
          EveryLoop();
        }
      break;
      case FUNC_MQTT_CONNECTED:
        // Load_New_Subscriptions_From_Function_Template();
      break;
      case FUNC_EVERY_50_MSECOND:
        if(Mqtt.connected){
          pubsub->loop();
        }
      break;
      case FUNC_EVERY_SECOND:{

        // AddLog(LOG_LEVEL_TEST, PSTR("pCONT_set->Settings.mqtt.client_name=\"%s\""),pCONT_set->Settings.mqtt.client_name);
        // AddLog(LOG_LEVEL_TEST, PSTR("pCONT_set->Settings.system_name.device=\"%s\""),pCONT_set->Settings.system_name.device);

        // init();
    
        // AddLog(LOG_LEVEL_TEST, PSTR("pCONT_set->Settings.mqtt.client_name=\"%s\""),pCONT_set->Settings.mqtt.client_name);
        // AddLog(LOG_LEVEL_TEST, PSTR("pCONT_set->Settings.system_name.device=\"%s\""),pCONT_set->Settings.system_name.device);
  // memset(pCONT_set->Settings.mqtt.client_name,0,sizeof(pCONT_set->Settings.mqtt.client_name));
  // sprintf_P(pCONT_set->Settings.mqtt.client_name,PSTR("%s-%s"),pCONT_set->Settings.system_name.device,WiFi.macAddress().c_str());



        // if(pCONT_wif->WifiCheckIpConnected()){

        if(pCONT_gsm->modem)
        {

          if(pCONT_gsm-> gprs.enabled==false)
          { 
            AddLog(LOG_LEVEL_TEST, PSTR("gprs.enabled FALSE")); 

            return 1;  
          }

          if(pCONT_gsm->modem->isNetworkConnected())
          {

            AddLog(LOG_LEVEL_TEST, PSTR("mMQTT_Cellular pCONT_gsm->modem->isNetworkConnected() IS connceted"));
            CheckConnection();
          }else{
            AddLog(LOG_LEVEL_TEST, PSTR("mMQTT_Cellular pCONT_gsm->modem->isNetworkConnected() NOT connceted"));
          }
        }
      }
      break;
      case FUNC_EVERY_MINUTE:
        //DiscoverServer();
        //pCONT->Tasker_Interface(FUNC_MQTT_CHECK_REDUNCTION_LEVEL);
      break;
      case FUNC_EVERY_HOUR:
        Send_LWT_Online();
      break;
      case FUNC_UPTIME_10_MINUTES:
        flag_uptime_reached_reduce_frequency = true;
      break;
      /************
       * COMMANDS SECTION * 
      *******************/
     
    } // END switch

  }//enabled mqtt_enabled

} // END function


void mMQTT_Cellular::Load_New_Subscriptions_From_Function_Template()
{

  #ifdef USE_FUNCTION_TEMPLATE  
  D_DATA_BUFFER_CLEAR();
  memcpy_P(data_buffer.payload.ctr,FUNCTION_TEMPLATE,sizeof(FUNCTION_TEMPLATE));
  data_buffer.payload.len = strlen(data_buffer.payload.ctr);
  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK  "Load_New_Subscriptions_From_Function_Template READ = \"%d|%s\""), data_buffer.payload.len, data_buffer.payload.ctr);
  #endif //USE_FUNCTION_TEMPLATE

  JsonParser parser(data_buffer.payload.ctr);
  JsonParserObject obj = parser.getRootObject();   
  JsonParserToken jtok = 0; 

  if(jtok = obj["MQTTSubscribe"])
  {
    if(jtok.isArray())
    {

      JsonParserArray arrobj = jtok;
      for(auto v : arrobj) 
      {

        const char* new_topic = v.getStr();
        ALOG_INF(PSTR("New Subscribe = \"%s\""), new_topic);


      }
    }

  }





  //   // if(jtok.isStr()){
  //   //   relay_id = GetRelayIDbyName(jtok.getStr());
  //   // }else 
  //   // if(jtok.isNum()){
  //   //   relay_id  = jtok.getInt();
  //   // }
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog(LOG_LEVEL_TEST,PSTR("mMQTT_Cellular::parse_JSONCommand MQTTSend"));
  //   #endif //
  //   JsonParserToken jtok_topic = jtok.getObject()["Topic"];
  //   JsonParserToken jtok_payload = jtok.getObject()["Payload"];

  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog(LOG_LEVEL_TEST,PSTR("mMQTT_Cellular::parse_JSONCommand MQTTSend %d"),jtok_topic.size());
  //   AddLog(LOG_LEVEL_TEST,PSTR("mMQTT_Cellular::parse_JSONCommand MQTTSend %d"),jtok_payload.size());
  //   #endif // 

  //   char topic_ctr[100] = {0};
  //   char payload_ctr[300] = {0};

  //   snprintf(topic_ctr, sizeof(topic_ctr), jtok_topic.getStr());
  //   snprintf(payload_ctr, sizeof(payload_ctr), jtok_payload.getStr());

  //   // char payload2_ctr[300] = {0};

  //   // char buffer_unescaped[200] = {0};
  //   char buffer_escaped[200] = {0};
  //   uint8_t len  = 0;
  //   for(int i=0;i<strlen(payload_ctr);i++){
  //       if(payload_ctr[i] == '~'){
  //       len+=sprintf(buffer_escaped+len,"\"");
  //       }else{    
  //       buffer_escaped[len++] = payload_ctr[i];
  //       }
  //   }

  //   // mSupport


  //   // jtok_topic.size()

  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //   AddLog(LOG_LEVEL_TEST,PSTR("Topic=%s"),topic_ctr);
  //   AddLog(LOG_LEVEL_TEST,PSTR("Payload=%s"),payload_ctr);
  //   AddLog(LOG_LEVEL_TEST,PSTR("buffer_escaped=%s"),buffer_escaped);
  //   #endif // ENABLE_LOG_LEVEL_INFO

  //   // ppublish(jtok_topic.getStr(),jtok_payload.getStr(),false);
  //   pubsub->publish(topic_ctr,buffer_escaped,false);

  // }

}


void mMQTT_Cellular::parse_JSONCommand(JsonParserObject obj){

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

  // if(parser.)

  // if(obj)
 
  // uint8_t relay_id= 0,state=-1;    //assume index 0 if none given

  if(jtok = obj["MQTTSend"]){
    // if(jtok.isStr()){
    //   relay_id = GetRelayIDbyName(jtok.getStr());
    // }else 
    // if(jtok.isNum()){
    //   relay_id  = jtok.getInt();
    // }
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST,PSTR("mMQTT_Cellular::parse_JSONCommand MQTTSend"));
    #endif //
    JsonParserToken jtok_topic = jtok.getObject()["Topic"];
    JsonParserToken jtok_payload = jtok.getObject()["Payload"];

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST,PSTR("mMQTT_Cellular::parse_JSONCommand MQTTSend %d"),jtok_topic.size());
    AddLog(LOG_LEVEL_TEST,PSTR("mMQTT_Cellular::parse_JSONCommand MQTTSend %d"),jtok_payload.size());
    #endif // 

    char topic_ctr[100] = {0};
    char payload_ctr[300] = {0};

    snprintf(topic_ctr, sizeof(topic_ctr), jtok_topic.getStr());
    snprintf(payload_ctr, sizeof(payload_ctr), jtok_payload.getStr());

    // char payload2_ctr[300] = {0};

    // char buffer_unescaped[200] = {0};
    char buffer_escaped[200] = {0};
    uint8_t len  = 0;
    for(int i=0;i<strlen(payload_ctr);i++){
        if(payload_ctr[i] == '~'){
        len+=sprintf(buffer_escaped+len,"\"");
        }else{    
        buffer_escaped[len++] = payload_ctr[i];
        }
    }

    // mSupport


    // jtok_topic.size()

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_TEST,PSTR("Topic=%s"),topic_ctr);
    AddLog(LOG_LEVEL_TEST,PSTR("Payload=%s"),payload_ctr);
    AddLog(LOG_LEVEL_TEST,PSTR("buffer_escaped=%s"),buffer_escaped);
    #endif // ENABLE_LOG_LEVEL_INFO

    // ppublish(jtok_topic.getStr(),jtok_payload.getStr(),false);
    pubsub->publish(topic_ctr,buffer_escaped,false);

  }
  // else{

  //   AddLog(LOG_LEVEL_TEST,PSTR("mMQTT_Cellular::parse_JSONCommand !MQTTSend"));
  
  // }

  /**
   * @brief Adding a new status stream, which is basically the default response to user requested commands ie scan for i2c devices, or simply, scan all devices and report addresses
   * 
   */
  // if(jtok = obj["MQTTSend"]){

  // }




  //Search for json sending
  //{"MQTT"}
  /**
   * {"MQTTSend":{"Topic":"kitchenlight4/set/relay","Payload":"{\"PowerName\":0,\"PowerState\":2}"}}
   * for light, later use a TCP socket for cross comms? though, I would need IP
   * 
   * subscribe
   * 

   */


}//end function



void mMQTT_Cellular::init(void){

  memset(pCONT_set->Settings.mqtt.client_name,0,sizeof(pCONT_set->Settings.mqtt.client_name));
  snprintf_P(pCONT_set->Settings.mqtt.client_name,sizeof(pCONT_set->Settings.mqtt.client_name)-1,PSTR("%s-%s"),pCONT_set->Settings.system_name.device,WiFi.macAddress().c_str());


  //Set primary mqtt broker address as first
  // COMES FROM DEFAULT SETTINGS
  
  // memset(pCONT_set->Settings.mqtt.hostname_ctr,0,sizeof(pCONT_set->Settings.mqtt.hostname_ctr));
  // #ifdef USE_NETWORK_MDNS
  // snprintf(pCONT_set->Settings.mqtt.hostname_ctr,sizeof(pCONT_set->Settings.mqtt.hostname_ctr),MDNS_MQTT_HOSTNAME1);//,strlen(MDNS_MQTT_HOSTNAME1));
  // #endif // #ifdef USE_NETWORK_MDNS

  // AddLog(LOG_LEVEL_INFO, PSTR("Settings.system_name.device = %s"),pCONT_set->Settings.system_name.device);

// delay(5000);

//new name needed!
  setprefixtopic(pCONT_set->Settings.system_name.device);

  memset(pCONT_set->Settings.mqtt.lwt_topic,0,sizeof(pCONT_set->Settings.mqtt.lwt_topic));
  snprintf_P(pCONT_set->Settings.mqtt.lwt_topic,sizeof(pCONT_set->Settings.mqtt.lwt_topic)-1,PSTR("%s/status/LWT"),pCONT_set->Settings.system_name.device);

   
}

/**
 * @brief Connected
 * 
 */
void mMQTT_Cellular::Send_LWT_Online()
{

  pubsub->publish(pCONT_set->Settings.mqtt.lwt_topic, WILLMESSAGE_ONCONNECT_CTR, true); // onconnect message

}




void mMQTT_Cellular::CheckConnection()
{ DEBUG_PRINT_FUNCTION_NAME;

  // if (pCONT_set->Settings.flag_system.mqtt_enabled) {  // SetOption3 - Enable MQTT
    if (!MqttIsConnected()) {

    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_TEST, PSTR("========================================!MqttIsConnected"));
    #endif //  ENABLE_LOG_LEVEL_INFO


      // pCONT_set->global_state.mqtt_down = 1;
      if (!Mqtt.retry_counter) 
      {
        MqttReconnect();
      } else {
        Mqtt.retry_counter--;
        ALOG_INF( PSTR("Mqtt.retry_counter=%d") ,Mqtt.retry_counter );
      }



    } else {
      pCONT_set->global_state.mqtt_down = 0;
    }
  // } else {
  //   pCONT_set->global_state.mqtt_down = 0;
  //   if (Mqtt.initial_connection_state) {
  //     MqttReconnect();
  //   }
  // } // mqtt_enabled

}



bool mMQTT_Cellular::MqttIsConnected(){
  DEBUG_PRINT_FUNCTION_NAME;

  // if pubsub is null, then its also not been set
  if(pubsub == nullptr){
    return false;
  }
  return pubsub->connected();

}

void mMQTT_Cellular::MqttConnected(void)
{
  
    // AddLog(LOG_LEVEL_INFO, PSTR("mMQTT_Cellular::MqttConnected %s %d"), D_CONNECTED, Mqtt.connected);
    Mqtt.connected = true;
    
    // AddLog(LOG_LEVEL_WARN, S_LOG_MQTT, PSTR("Mqtt.retry_counter = 0; disabled"));
    Mqtt.retry_counter = 0;


  // DEBUG_LINE;
  // DEBUG_PRINT_FUNCTION_NAME;
    // DEBUG_LINE;
  char lwt_message_ondisconnect_ctr[200];
  sprintf_P(lwt_message_ondisconnect_ctr,PSTR("{\"LWT\":\"Offline\",\"reset_reason\":\"%s\",\"uptime\":\"%s\"}"),pCONT_sup->GetResetReason().c_str(),pCONT_time->uptime.hhmmss_ctr);
  // DEBUG_LINE;
  
    #ifdef ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT // Show disconnect occured if we have reconnected inside timeout
      // DEBUG_LINE;
      pubsub->publish(pCONT_set->Settings.mqtt.lwt_topic,lwt_message_ondisconnect_ctr,true); // onconnect message
        // DEBUG_LINE;
      delay(100);
    #endif
    
    Send_LWT_Online();
    
    #ifdef USE_MQTT_RETAINED_VERSION_HISTORY_CHECK // for checking version history
      pubsub->subscribe(("TBD_firmware/set/settings/firmware")); //PSTR failure
    #endif
    
    // Group name for setting all devices
    pubsub->subscribe(("group_all/#"));//(PSTR("group_all/#"));
    
    //pubsub->subscribe("<devicename>/sync"); //namesake sync name
    psubscribe(PSTR(D_MQTT_SYNC "/#"));
    
    // //pubsub->subscribe("<devicename>/set/#");
    psubscribe(PSTR(D_MQTT_COMMAND "/#"));
    
    connection_maintainer.cConnectionAttempts = 0; // reset

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT " \"Connection SUCCESS!\""));
    #endif// ENABLE_LOG_LEVEL_INFO
    
    // if succesful, clear flag
    connection_maintainer.flag_require_reconnection = false;


    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT " \"Connected? %d!\""),pubsub->connected());
    #endif //  ENABLE_LOG_LEVEL_INFO

    // DEBUG_LINE;
    pCONT->Tasker_Interface(FUNC_MQTT_CONNECTED);

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT " \"Connected? %d!\""),pubsub->connected());
    #endif //  ENABLE_LOG_LEVEL_INFO
    
    // DEBUG_LINE;


  // char stopic[TOPSZ];

//   if (Mqtt.allowed) {
//     AddLog(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_CONNECTED));
    // Mqtt.connected = true;
    
    // AddLog(LOG_LEVEL_WARN, S_LOG_MQTT, PSTR("Mqtt.retry_counter = 0; disabled"));
    // Mqtt.retry_counter = 0;


//     Mqtt.connect_count++;

//     GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
//     Response_P(PSTR(MQTT_LWT_ONLINE));
//     MqttPublish(stopic, true);

//     if (!Settings.flag4.only_json_message) {  // SetOption90 - Disable non-json MQTT response
//       // Satisfy iobroker (#299)
//       mqtt_data[0] = '\0';
//       MqttPublishPrefixTopic_P(CMND, S_RSLT_POWER);
//     }

//     GetTopic_P(stopic, CMND, mqtt_topic, PSTR("#"));
//     MqttSubscribe(stopic);
//     if (strstr_P(SettingsText(SET_MQTT_FULLTOPIC), MQTT_TOKEN_TOPIC) != nullptr) {
//       uint32_t real_index = SET_MQTT_GRP_TOPIC;
//       for (uint32_t i = 0; i < MAX_GROUP_TOPICS; i++) {
//         if (1 == i) { real_index = SET_MQTT_GRP_TOPIC2 -1; }
//         if (strlen(SettingsText(real_index +i))) {
//           GetGroupTopic_P(stopic, PSTR("#"), real_index +i);  // SetOption75 0: %prefix%/nothing/%topic% = cmnd/nothing/<grouptopic>/# or SetOption75 1: cmnd/<grouptopic>
//           MqttSubscribe(stopic);
//         }
//       }
//       GetFallbackTopic_P(stopic, PSTR("#"));
//       MqttSubscribe(stopic);
//     }

//     XdrvCall(FUNC_MQTT_SUBSCRIBE);
//   }

//   if (Mqtt.initial_connection_state) {
//     if (ResetReason() != REASON_DEEP_SLEEP_AWAKE) {
//       char stopic2[TOPSZ];
//       Response_P(PSTR("{\"" D_CMND_MODULE "\":\"%s\",\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_FALLBACKTOPIC "\":\"%s\",\"" D_CMND_GROUPTOPIC "\":\"%s\"}"),
//         ModuleName().c_str(), my_version, my_image, GetFallbackTopic_P(stopic, ""), GetGroupTopic_P(stopic2, "", SET_MQTT_GRP_TOPIC));
//       MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "1"));
// #ifdef USE_WEBSERVER
//       if (Settings.webserver) {
// #if LWIP_IPV6
//         Response_P(PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\",\"IPv6Address\":\"%s\"}"),
//           (2 == Settings.webserver) ? D_ADMIN : D_USER, NetworkHostname(), NetworkAddress().toString().c_str(), WifiGetIPv6().c_str());
// #else
//         Response_P(PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_CMND_HOSTNAME "\":\"%s\",\"" D_CMND_IPADDRESS "\":\"%s\"}"),
//           (2 == Settings.webserver) ? D_ADMIN : D_USER, NetworkHostname(), NetworkAddress().toString().c_str());
// #endif // LWIP_IPV6 = 1
//         MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "2"));
//       }
// #endif  // USE_WEBSERVER
//       Response_P(PSTR("{\"" D_JSON_RESTARTREASON "\":"));
//       if (CrashFlag()) {
//         CrashDump();
//       } else {
//         ResponseAppend_P(PSTR("\"%s\""), GetResetReason().c_str());
//       }
//       ResponseJsonEnd();
//       MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "3"));
//     }

//     MqttPublishAllPowerState();
//     if (Settings.tele_period) {
//       tele_period = Settings.tele_period -5;  // Enable TelePeriod in 5 seconds
//     }
//     rules_flag.system_boot = 1;
//     XdrvCall(FUNC_MQTT_INIT);
//   }
  Mqtt.initial_connection_state = 0;

//   global_state.mqtt_down = 0;
//   if (Settings.flag.mqtt_enabled) {  // SetOption3 - Enable MQTT
//     rules_flag.mqtt_connected = 1;
//   }
}

void mMQTT_Cellular::MqttDisconnected(int state)
{
  DEBUG_PRINT_FUNCTION_NAME;
  
    connection_maintainer.cConnectionAttempts++;    
    // AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Connection FAILED, state = [%s], retrying in %d ms"),state_ctr(),connection_maintainer.rSavedReconnectAttempt);

  Mqtt.connected = false;
  Mqtt.retry_counter = pCONT_set->Settings.mqtt_retry;

  // pubsub->disconnect();

  // AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND), SettingsText(SET_MQTT_HOST), Settings.mqtt_port, state, Mqtt.retry_counter);
  // rules_flag.mqtt_disconnected = 1;

}






void mMQTT_Cellular::MqttReconnect(void){ DEBUG_PRINT_FUNCTION_NAME;
  
  DEBUG_LINE;
 
  // Mqtt.allowed = Settings.flag.mqtt_enabled;  // SetOption3 - Enable MQTT
  // if (Mqtt.allowed) {
  //   if (!strlen(SettingsText(SET_MQTT_HOST)) || !Settings.mqtt_port) {
  //     Mqtt.allowed = false;
  //   }
  // }
  // if (!Mqtt.allowed) {
  //   MqttConnected();
  //   return;
  // }

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_ATTEMPTING_CONNECTION));
  #endif// ENABLE_LOG_LEVEL_INFO

  DEBUG_LINE;
  Mqtt.connected = false;
  Mqtt.retry_counter = pCONT_set->Settings.mqtt_retry;
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
  
    DEBUG_LINE_HERE;
  // Create again if needed
  if(pubsub == nullptr){
    // if(client != nullptr){
    //   client->stop(); // force off before starting again
    //   delay(20);      // Allow wifi to clear
    // }
    // if(pCONT_gsm->modem == nullptr)
    // TinyGsm* modem2; //(Serial);// = pCONT_gsm->modem;
    client = new TinyGsmClient(*pCONT_gsm->modem); // Wifi Client reconnect issue 4497 ie memory leak on each attempt without this (https://github.com/esp8266/Arduino/issues/4497)
    pubsub = new mPubSubClient(*client);

    DEBUG_LINE_HERE;
  }

  if (2 == Mqtt.initial_connection_state) {  // Executed once just after power on and wifi is connected
    Mqtt.initial_connection_state = 1;
  }

// change this to use callback in the future again
  pubsub->setCallback(
    [this](char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){
      this->MqttDataHandler(mqtt_topic, mqtt_data, data_len);
    }
  );

  // pubsub->setServer(SettingsText(SET_MQTT_HOST), Settings.mqtt_port);
  //pubsub->setServer(mdns_mqtt_hostname_ctr, 1883);
  //Serial.print("mMQTT_Cellular:setServer> \""); Serial.print(mdns_mqtt_hostname_ctr); Serial.println("\"");
  // IPAddress mqqtserver(192,168,1,65); //desktop
  // IPAddress mqqtserver(192,168,1,65); //desktop


  IPAddress mqqtserver(D_MQTTSERVER_IP_ADDRESS_COMMA_DELIMITED);
    
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


  // pubsub->setServer(mqqtserver, 1883);


  // #ifdef ENABLE_DEVFEATURE_DDNS_MQTT_TEST

  pubsub->setServer("sparkequinox.ddns.net", 51883);
    DEBUG_LINE_HERE;

  // #else

  // pubsub->setServer(mqqtserver, 1883);

  // #endif // ENABLE_DEVFEATURE_DDNS_MQTT_TEST


  #endif
    
    // 192,168,1,65); //desktop


  
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

    DEBUG_LINE_HERE;
  if(pubsub->connect(pCONT_set->Settings.mqtt.client_name,pCONT_set->Settings.mqtt.lwt_topic,WILLQOS_CTR,WILLRETAIN_CTR,lwt_message_ondisconnect_ctr)){  //boolean connect (clientID, willTopic, willQoS, willRetain, willMessage)
    
    DEBUG_LINE_HERE;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("mMQTT_Cellular::MqttReconnect Connected"));
    #endif // ENABLE_LOG_LEVEL_INFO
    MqttConnected();

    DEBUG_LINE_HERE;
  }
  else
  {
    DEBUG_LINE_HERE;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_INFO, PSTR("mMQTT_Cellular::MqttReconnect Failed %d"),pubsub->state());
    #endif // 
    MqttDisconnected(pubsub->state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
    
    DEBUG_LINE_HERE;
  }

    DEBUG_LINE_HERE;
} // END function



void mMQTT_Cellular::MqttDataHandler(char* mqtt_topic, uint8_t* mqtt_data, unsigned int data_len){ DEBUG_PRINT_FUNCTION_NAME;

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


void mMQTT_Cellular::EveryLoop(){ DEBUG_PRINT_FUNCTION_NAME;


  // Send mqtt from all modules
  // pCONT->Tasker_Interface(FUNC_MQTT_SENDER);

}



void mMQTT_Cellular::setprefixtopic(const char* _prefixtopic){
  memset(pCONT_set->Settings.mqtt.prefixtopic,0,sizeof(pCONT_set->Settings.mqtt.prefixtopic));
  strncpy(pCONT_set->Settings.mqtt.prefixtopic,_prefixtopic,strlen(_prefixtopic));
}


// Send packet with payload using va_list
void mMQTT_Cellular::Send_Prefixed_P(const char* topic, PGM_P formatP, ...)
{

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), formatP, arg);
  va_end(arg);

  ppublish(topic, data_buffer.payload.ctr, false);

}// END function



/************ PARSERS ************************************************************************************************************************************************
*********************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************/

//<devicename>/set/<function>/<subfunction>
//<devicename>/status/<function>/<subfunction>
// void mMQTT_Cellular::parse_JSONCommand(){

//   // // Check if instruction is for me
//   // if(mSupport::mSearchCtrIndexOf(data_buffer.topic.ctr,"set/mqtt")>=0){
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND " mqtt"));
//   //   pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
//   // }else{
//   //   return; // not meant for here
//   // }

  
    
//   // //new topic names must include pixels
  
//   // if(strstr(data_buffer.topic.ctr,"/system")){
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC "system"));    
//   //   //isserviced += parsesub_MQTTSettingsCommand();
//   // }else
//   // if(strstr(data_buffer.topic.ctr,"TBD_firmware/information")){
//   //   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC "TBD_firmware/information"));    
//   //   //isserviced += parsesub_FirmwareInformation();
//   // }else
//   // {
//   //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC "INVALID"));    
//   // } 

// } // END function


//,(animation_override.fRefreshAllPixels?"Set":"UNSET")
void mMQTT_Cellular::parsesub_MQTTSettingsCommand(){

  // int8_t tmp_id = 0;

  // #ifdef JSONDOCUMENT_STATIC
  //   StaticJsonDocument<800> doc;
  // #else
  //   DynamicJsonDocument doc(600);
  // #endif
  // DeserializationError error = deserializeJson(doc, data_buffer.payload.ctr);
  
  // if(error){
  //   AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_JSON_DESERIALIZATION_ERROR));
  //   Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_JSON_DESERIALIZATION_ERROR);
  //   return;
  // }
  // JsonObject obj = doc.as<JsonObject>();
  
  // if(!obj["command"].isNull()){ 
  //   const char* command = obj["command"];
  //   if(strcmp(command,"system_send_all")==0){ 
  //     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"command\"=\"system_send_all\""));
  //     //MQTTHandler_Set_RefreshAll();
  //     data_buffer.isserviced++;
  //   }
  //   else{
  //     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
  //   }
  // }

  

} // END FUNCTION


// New sender functions to replace ppublish call





void mMQTT_Cellular::MQTTHandler_Send_Formatted(uint8_t topic_type, uint8_t module_id, const char* postfix_topic_ctr){

  if(pubsub==nullptr)
  {
      DEBUG_LINE_HERE;
      return; // to stop crashing as some buffers below are only configured on connection

  }

  if(!pubsub->connected())
  {
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
void mMQTT_Cellular::publish_ft(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

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

 
void mMQTT_Cellular::publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  sprintf_P(topic_ctr,PSTR("%s/%s/%s"),D_TOPIC_STATUS,module_name,topic_postfix); 
  
  ppublish(topic_ctr,payload_ctr,retain_flag);

}

// My function for adding prefix by device name
boolean mMQTT_Cellular::ppublish(const char* topic, const char* payload, boolean retained){


  if(pubsub == nullptr)
  {
    ALOG_ERR(PSTR("pubsub == nullptr"));
    return false;
  }

  //// AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "!pubsub->connected() BEFORE "));
    if (!pubsub->connected()) {
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "NOT CONNECTED \"ppublish\" failed!"));
      #endif// ENABLE_LOG_LEVEL_INFO
      connection_maintainer.flag_require_reconnection = true;
      return false;
    }

    // DEBUG_LINE_HERE;

    if(strlen(payload)<1){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "strlen(payload)<1"));
    #endif// ENABLE_LOG_LEVEL_INFO
    }

// Serial.println(WiFi.localIP());
// Serial.println(static_cast<uint32_t>(WiFi.localIP())); 

    if (!pCONT_wif->WifiCheckIpConnected()) {
    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early pp"));
    #endif// ENABLE_LOG_LEVEL_INFO
        return false;
    }else{
        //AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "WiFi.status()=%d"),WiFi.status());
    }

    //if(prefixtopic[0]!=0){}
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Exiting early?"));
//     // delay(2500); ESP.wdtFeed();
// Serial.print("HERE1");
// Serial.flush();
    // delay(2500); ESP.wdtFeed();
DEBUG_LINE;

if(WiFi.status() != WL_CONNECTED){ 
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early"));
    #endif// ENABLE_LOG_LEVEL_INFO
  return false;
}

DEBUG_LINE;

//return 0;//

    char convctr[100]; memset(convctr,0,sizeof(convctr));
    //Serial.println(pCONT_set->Settings.mqtt.prefixtopic); Serial.flush();
    // Serial.println(topic); Serial.flush();
    snprintf(convctr,sizeof(convctr),PSTR("%s/%S"),pCONT_set->Settings.mqtt.prefixtopic,topic);
    
    // Serial.println(convctr); Serial.flush();
DEBUG_LINE;
//TRACE();
    #ifdef ENABLE_LOG_LEVEL_INFO
    ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_TOPIC " [%s] %d"),convctr,strlen(convctr));
    ALOG_DBM( PSTR(D_LOG_PUBSUB "-->" D_PAYLOAD " [%s] %d"),payload,strlen(payload));
    #endif// ENABLE_LOG_LEVEL_INFO

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
    return pubsub->publish(convctr,(const uint8_t*)payload,strlen(payload),retained);
  }
  DEBUG_LINE;
  return 0;
  //TRACE();
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
boolean mMQTT_Cellular::ppublish_device_name_prefix_P(const char* topic, const char* payload, boolean retained){

  //// AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "!pubsub->connected() BEFORE "));
    if (!pubsub->connected()) {
      #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "NOT CONNECTED \"ppublish\" failed!"));
      #endif// ENABLE_LOG_LEVEL_INFO
      connection_maintainer.flag_require_reconnection = true;
      return false;
    }

    if(strlen_P(payload)<1){
    #ifdef ENABLE_LOG_LEVEL_INFO
      AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "strlen(payload)<1"));
    #endif// ENABLE_LOG_LEVEL_INFO
    }

// Serial.println(WiFi.localIP());
// Serial.println(static_cast<uint32_t>(WiFi.localIP())); 

    if (!pCONT_wif->WifiCheckIpConnected()) {
    #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early pp"));
    #endif// ENABLE_LOG_LEVEL_INFO
        return false;
    }else{
        //AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "WiFi.status()=%d"),WiFi.status());
    }

    //if(prefixtopic[0]!=0){}
//     AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Exiting early?"));
//     // delay(2500); ESP.wdtFeed();
// Serial.print("HERE1");
// Serial.flush();
    // delay(2500); ESP.wdtFeed();
DEBUG_LINE;

if(WiFi.status() != WL_CONNECTED){ 
    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early"));
    #endif// ENABLE_LOG_LEVEL_INFO
  return false;
}

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


boolean mMQTT_Cellular::psubscribe(const char* topic) {
  char ttopic[70];
  memset(ttopic,0,sizeof(ttopic));
  sprintf(ttopic,PSTR("%s/%s"),pCONT_set->Settings.mqtt.prefixtopic,topic);
  ALOG_INF( PSTR("Subscribing to \"%s\""), ttopic );
  return pubsub->subscribe(ttopic, 0);
}




/**
 * @brief This is not thread safe
 * 
 * @return const char* 
 */
const char* mMQTT_Cellular::state_ctr(void)
{

  ALOG_ERR( PSTR("mMQTT_Cellular::state_ctr - Not thread safe") );

  DEBUG_PRINT_FUNCTION_NAME;
    switch(pubsub->state()){
      case MQTT_CONNECTION_TIMEOUT:       return PSTR("Connection Timeout");
      case MQTT_CONNECTION_LOST:          return PSTR("Connecttion Lost");
      case MQTT_CONNECT_FAILED:           return PSTR("Connect Failed");
      case MQTT_DISCONNECTED:             return PSTR("Disconnected");
      case MQTT_CONNECTED:                return PSTR("Connected");
      case MQTT_CONNECT_BAD_PROTOCOL:     return PSTR("Bad Protocol");
      case MQTT_CONNECT_BAD_CLIENT_ID:    return PSTR("Bad Client ID");
      case MQTT_CONNECT_UNAVAILABLE:      return PSTR("Unavailable");
      case MQTT_CONNECT_BAD_CREDENTIALS:  return PSTR("Bad Credentials");
      case MQTT_CONNECT_UNAUTHORIZED:     return PSTR("Unauthorized");
    }
}



#endif // USE_MODULE_NETWORK_MQTT