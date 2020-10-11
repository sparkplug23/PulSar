#include "mMQTT.h"

// const char S_CONFIGURE_MQTT[] PROGMEM = D_CONFIGURE_MQTT;

// const char HTTP_BTN_MENU_MQTT[] PROGMEM =
//   "<p><form action='" WEB_HANDLE_MQTT "' method='get'><button>" D_CONFIGURE_MQTT "</button></form></p>";

// bool flag_uptime_reached_reduce_frequency = false;


// const char HTTP_FORM_MQTT1[] PROGMEM =
//   "<fieldset><legend><b>&nbsp;" D_MQTT_PARAMETERS "&nbsp;</b></legend>"
//   "<form method='get' action='" WEB_HANDLE_MQTT "'>"
//   "<p><b>" D_HOST "</b> (" MQTT_HOST ")<br/><input id='mh' name='mh' placeholder='" MQTT_HOST" ' value='%s'></p>"
//   "<p><b>" D_PORT "</b> (" STR(MQTT_PORT) ")<br/><input id='ml' name='ml' placeholder='" STR(MQTT_PORT) "' value='%d'></p>"
//   "<p><b>" D_CLIENT "</b> (%s)<br/><input id='mc' name='mc' placeholder='%s' value='%s'></p>";
// const char HTTP_FORM_MQTT2[] PROGMEM =
//   "<p><b>" D_USER "</b> (" MQTT_USER ")<br/><input id='mu' name='mu' placeholder='" MQTT_USER "' value='%s'></p>"
//   "<p><b>" D_PASSWORD "</b><br/><input id='mp' name='mp' type='password' placeholder='" D_PASSWORD "' value='" D_ASTERISK_PWD "'></p>"
//   "<p><b>" D_TOPIC "</b> = %%topic%% (%s)<br/><input id='mt' name='mt' placeholder='%s' value='%s'></p>"
//   "<p><b>" D_FULL_TOPIC "</b> (%s)<br/><input id='mf' name='mf' placeholder='%s' value='%s'></p>";
  

void mMQTT::init(void){

  pubsub = new mPubSubClient(client);
  
  memset(settings.client_name,0,sizeof(settings.client_name));
  sprintf(settings.client_name,PSTR("%s-%s"),pCONT_set->Settings.system_name.device,WiFi.macAddress().c_str());

  //Set primary mqtt broker address as first
  memset(settings.hostname_ctr,0,sizeof(settings.hostname_ctr));
  memcpy(settings.hostname_ctr,MDNS_MQTT_HOSTNAME1,strlen(MDNS_MQTT_HOSTNAME1));

  memset(settings.prefixtopic,0,sizeof(settings.prefixtopic));

//new name needed!
  setprefixtopic(pCONT_set->Settings.system_name.device);



  memset(settings.lwt_topic,0,sizeof(settings.lwt_topic));
  sprintf(settings.lwt_topic,PSTR("%s/status/LWT"),pCONT_set->Settings.system_name.device);

  #ifdef ESP8266
    //pubsub->setServer(mdns_mqtt_hostname_ctr, 1883);
    //Serial.print("mMQTT:setServer> \""); Serial.print(mdns_mqtt_hostname_ctr); Serial.println("\"");
    IPAddress mqqtserver(192,168,1,65); //desktop
    pubsub->setServer(mqqtserver, 1883);
   #endif
   #ifdef ESP32
     IPAddress mqqtserver(192,168,1,65); //desktop
     pubsub->setServer(mqqtserver, 1883);
   #endif
   
  MQTTHandler_Init();  

}

void mMQTT::setprefixtopic(const char* _prefixtopic){
  strncpy(settings.prefixtopic,_prefixtopic,strlen(_prefixtopic));
}


// #ifdef USE_DISCOVERY
// #ifdef MQTT_HOST_DISCOVERY
void mMQTT::DiscoverServer(void)
{
  if (!pCONT_wif->mdns_begun) { return; }

  int n = MDNS.queryService("mqtt", "tcp");  // Search for mqtt service

  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_QUERY_DONE " %d"), n);

  if(n == 0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_QUERY_DONE " No services found"));
  }

  if (n > 0) {

  Serial.print(n); Serial.println(" service(s) found"); 

    uint8_t i = 0;             // If the hostname isn't set, use the first record found.
//#ifdef MDNS_HOSTNAME
    for (i = n; i > 0; i--) {  // Search from last to first and use first if not found
      if (!strcmp(MDNS.hostname(i).c_str(), MDNS_MQTT_HOSTNAME1)) {
        break;                 // Stop at matching record
      }
    }
//#endif  // MDNS_HOSTNAME
    snprintf_P(pCONT_set->Settings.mqtt.host, sizeof(pCONT_set->Settings.mqtt.host), MDNS.IP(i).toString().c_str());
    pCONT_set->Settings.mqtt.port = MDNS.port(i);

    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MDNS D_MQTT_SERVICE_FOUND " %s, " D_IP_ADDRESS " %s, " D_PORT " %d"), 
                    MDNS.hostname(i).c_str(), pCONT_set->Settings.mqtt.host, pCONT_set->Settings.mqtt.port);
  }
}
// #endif  // MQTT_HOST_DISCOVERY
// #endif  // USE_DISCOVERY



void mMQTT::HandleMqttConfiguration(void)
{
// //  if (!HttpCheckPriviledgedAccess()) { return; }

//   // AddLog_P(LOG_LEVEL_DEBUG, S_LOG_HTTP, S_CONFIGURE_MQTT);

//   if (pCONT_web->pWebServer->hasParam("save")) {
//     //MqttSaveSettings();
//     //WebRestart(1);
//     return;
//   }

//   char str[sizeof(pCONT_set->Settings.mqtt_client)];

//   pCONT_web->WSStartAppend_P(S_CONFIGURE_MQTT);
//   pCONT_web->WSContentSendStyle();
//   pCONT_web->WSBufferAppend_P(HTTP_FORM_MQTT1,
//     pCONT_set->Settings.mqtt_host,
//     pCONT_set->Settings.mqtt_port,
//     pCONT_sup->Format(str, MQTT_CLIENT_ID, sizeof(str)), MQTT_CLIENT_ID, pCONT_set->Settings.mqtt_client);
//   pCONT_web->WSBufferAppend_P(HTTP_FORM_MQTT2,
//     (pCONT_set->Settings.mqtt_user[0] == '\0') ? "0" : pCONT_set->Settings.mqtt_user,
//     pCONT_sup->Format(str, MQTT_TOPIC, sizeof(str)), MQTT_TOPIC, pCONT_set->Settings.mqtt_topic,
//     MQTT_FULLTOPIC, MQTT_FULLTOPIC, pCONT_set->Settings.mqtt_fulltopic);
//   pCONT_web->WSBufferAppend_P(HTTP_FORM_END);
//   pCONT_web->WSContentSpaceButton(BUTTON_CONFIGURATION);
//   pCONT_web->WSContentStop();

}

void mMQTT::MqttSaveSettings(void)
{
  // char tmp[100];
  // char stemp[TOPSZ];
  // char stemp2[TOPSZ];

  // WebGetArg(request,"mt", tmp, sizeof(tmp));
  // strlcpy(stemp, (!strlen(tmp)) ? MQTT_TOPIC : tmp, sizeof(stemp));
  // MakeValidMqtt(0, stemp);
  // WebGetArg(request,"mf", tmp, sizeof(tmp));
  // strlcpy(stemp2, (!strlen(tmp)) ? MQTT_FULLTOPIC : tmp, sizeof(stemp2));
  // MakeValidMqtt(1,stemp2);
  // if ((strcmp(stemp, pCONT_set->Settings.mqtt_topic)) || (strcmp(stemp2, pCONT_set->Settings.mqtt_fulltopic))) {
  //   Response_P((pCONT_set->Settings.flag_system.mqtt_offline) ? S_OFFLINE : "");
  //   MqttPublishPrefixTopic_P(TELE, S_LWT, true);  // Offline or remove previous retained topic
  // }
  // strlcpy(pCONT_set->Settings.mqtt_topic, stemp, sizeof(pCONT_set->Settings.mqtt_topic));
  // strlcpy(pCONT_set->Settings.mqtt_fulltopic, stemp2, sizeof(pCONT_set->Settings.mqtt_fulltopic));
  // WebGetArg(request,"mh", tmp, sizeof(tmp));
  // strlcpy(pCONT_set->Settings.mqtt_host, (!strlen(tmp)) ? MQTT_HOST : (!strcmp(tmp,"0")) ? "" : tmp, sizeof(pCONT_set->Settings.mqtt_host));
  // WebGetArg(request,"ml", tmp, sizeof(tmp));
  // pCONT_set->Settings.mqtt_port = (!strlen(tmp)) ? MQTT_PORT : atoi(tmp);
  // WebGetArg(request,"mc", tmp, sizeof(tmp));
  // strlcpy(pCONT_set->Settings.mqtt_client, (!strlen(tmp)) ? MQTT_CLIENT_ID : tmp, sizeof(pCONT_set->Settings.mqtt_client));
  // WebGetArg(request,"mu", tmp, sizeof(tmp));
  // strlcpy(pCONT_set->Settings.mqtt_user, (!strlen(tmp)) ? MQTT_USER : (!strcmp(tmp,"0")) ? "" : tmp, sizeof(pCONT_set->Settings.mqtt_user));
  // WebGetArg(request,"mp", tmp, sizeof(tmp));
  // strlcpy(pCONT_set->Settings.mqtt_pwd, (!strlen(tmp)) ? "" : (!strcmp(tmp, D_ASTERISK_PWD)) ? pCONT_set->Settings.mqtt_pwd : tmp, sizeof(pCONT_set->Settings.mqtt_pwd));
  // AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_JSON_MQTTHOST " %s, " D_JSON_MQTTPORT " %d, " D_JSON_MQTTCLIENT " %s, " D_JSON_MQTTUSER " %s, " D_JSON_TOPIC " %s, " D_JSON_FULLTOPIC " %s"),
  //   pCONT_set->Settings.mqtt_host, pCONT_set->Settings.mqtt_port, pCONT_set->Settings.mqtt_client, pCONT_set->Settings.mqtt_user, pCONT_set->Settings.mqtt_topic, pCONT_set->Settings.mqtt_fulltopic);

}
// #endif  // USE_WEBSERVER

/*before progmem topic postfix
RAM:   [=====     ]  47.8% (used 39188 bytes from 81920 bytes)
Flash: [=====     ]  48.2% (used 493564 bytes from 1023984 bytes)

//after mqtt, pgm + inside class
RAM:   [=====     ]  47.9% (used 39200 bytes from 81920 bytes)
Flash: [=====     ]  48.2% (used 493744 bytes from 1023984 bytes)

//after mqtt, pgm only
RAM:   [=====     ]  47.7% (used 39092 bytes from 81920 bytes)
Flash: [=====     ]  48.2% (used 493556 bytes from 1023984 bytes)

//
RAM:   [=====     ]  47.6% (used 39028 bytes from 81920 bytes)
Flash: [=====     ]  48.2% (used 493484 bytes from 1023984 bytes)
*/

void mMQTT::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_health;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_10_MINUTES_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_HEALTH_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Health;
  
  mqtthandler_ptr = &mqtthandler_settings;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Settings;
  
  mqtthandler_ptr = &mqtthandler_log;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_LOG_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Log;
  
  mqtthandler_ptr = &mqtthandler_firmware;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FIRMWARE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Firmware;
  
  mqtthandler_ptr = &mqtthandler_memory;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MEMORY_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Memory;
  
  mqtthandler_ptr = &mqtthandler_network;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_NETWORK_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Network;
  
  mqtthandler_ptr = &mqtthandler_mqtt;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MQTT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_MQTT;
  
  mqtthandler_ptr = &mqtthandler_time;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_TIME_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Time;
  
  mqtthandler_ptr = &mqtthandler_devices;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEVICES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Devices;
  
  mqtthandler_ptr = &mqtthandler_reboot;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Reboot;
  
  mqtthandler_ptr = &mqtthandler_reboot_event;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR;  
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_ALL;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_REBOOT_EVENT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Reboot;

  #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
  mqtthandler_ptr = &mqtthandler_debug_pins;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_PINS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Debug_Pins;

  mqtthandler_ptr = &mqtthandler_debug_template;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_TEMPLATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Debug_Template;

  mqtthandler_ptr = &mqtthandler_debug_moduleinterface;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = SEC_IN_HOUR; 
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_UNCHANGED_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULETEMPLATE_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Debug_ModuleInterface;
  #endif
  
  mqtthandler_ptr = &mqtthandler_debug_minimal;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = DEFAULT_MQTT_SYSTEM_MINIMAL_RATE_SECS;
  mqtthandler_ptr->flags.FrequencyRedunctionLevel = MQTT_FREQUENCY_REDUCTION_LEVEL_REDUCE_AFTER_10_MINUTES_ID;
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_SYSTEM_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_MODULEMINIMAL_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mMQTT::ConstructJSON_Debug_Minimal;

} //end "MQTTHandler_Init"

// Send packet with payload using va_list
void mMQTT::Send_Prefixed_P(const char* topic, PGM_P formatP, ...)
{

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(data_buffer2.payload.ctr, sizeof(data_buffer2.payload.ctr), formatP, arg);
  va_end(arg);

  ppublish(topic, data_buffer2.payload.ctr, false);

}// END function

void mMQTT::connect(void) {

  // Generate will message
  char lwt_message_ondisconnect_ctr[200];
  sprintf(lwt_message_ondisconnect_ctr,"{\"LWT\":\"Offline\",\"reset_reason\":\"%s\",\"uptime\":\"%s\"}",pCONT_sup->GetResetReason().c_str(),pCONT->mt->uptime.hhmmss_ctr);

  if(pubsub->connect(settings.client_name,settings.lwt_topic,WILLQOS_CTR,WILLRETAIN_CTR,lwt_message_ondisconnect_ctr)){  //boolean connect (clientID, willTopic, willQoS, willRetain, willMessage)

    #ifdef ENABLE_MQTT_SEND_DISCONNECT_ON_RECONNECT // Show disconnect occured if we have reconnected inside timeout
      pubsub->publish(settings.lwt_topic,lwt_message_ondisconnect_ctr,true); // onconnect message
      delay(100);
    #endif
    pubsub->publish(settings.lwt_topic,WILLMESSAGE_ONCONNECT_CTR,true); // onconnect message

    #ifdef USE_MQTT_RETAINED_VERSION_HISTORY_CHECK // for checking version history
      pubsub->subscribe(PSTR("hacs_firmware/set/settings/firmware"));
    #endif

    // Group name for setting all devices
    pubsub->subscribe(PSTR("group_all/#"));

    //pubsub->subscribe("<devicename>/sync"); //namesake sync name
    psubscribe(PSTR(D_MQTT_SYNC "/#"));

    // //pubsub->subscribe("<devicename>/set/#");
    psubscribe(PSTR(D_MQTT_COMMAND "/#"));

    connection_maintainer.cConnectionAttempts = 0; // reset

    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT " \"Connection SUCCESS!\""));

    DEBUG_LINE;
    pCONT->Tasker_Interface(FUNC_MQTT_CONNECTED);

    DEBUG_LINE;

  }else{
    
    connection_maintainer.cConnectionAttempts++;    
    AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "Connection FAILED, state = [%s], retrying in %d ms"),pubsub->state_ctr(),connection_maintainer.rSavedReconnectAttempt);

    // #ifdef USE_ADVANCED_MQTT_CONNECTION
    //   Serial.print(F(" : Attempts ["));  Serial.print(connection_maintainer.cConnectionAttempts); Serial.print(F("] : Addresss \""));
    //   Serial.print(mdns_mqtt_hostname_ctr); Serial.println(F("\"")); 

    //   // If multiple failed attempts, switch to next broker address
    //   if(connection_maintainer.cConnectionAttempts>10){ Serial.print(F("[MQTT] Switching broker... "));
    // scan for new mqtt server
        
    //     connection_maintainer.cConnectionAttempts = 0;

    //     if(strstr(mdns_mqtt_hostname_ctr,MDNS_MQTT_HOSTNAME1)){ //Set primary mqtt broker address
    //       memset(mdns_mqtt_hostname_ctr,0,sizeof(mdns_mqtt_hostname_ctr));
    //       memcpy(mdns_mqtt_hostname_ctr,MDNS_MQTT_HOSTNAME2,strlen(MDNS_MQTT_HOSTNAME2));
    //     }else if(strstr(mdns_mqtt_hostname_ctr,MDNS_MQTT_HOSTNAME2)){
    //       memset(mdns_mqtt_hostname_ctr,0,sizeof(mdns_mqtt_hostname_ctr));
    //       memcpy(mdns_mqtt_hostname_ctr,MDNS_MQTT_HOSTNAME1,strlen(MDNS_MQTT_HOSTNAME1));
    //     }else{
    //       Serial.print(F("[MQTT] ERROR Switching broker - FALLING BACK TO PRIMARY "));
    //       memset(mdns_mqtt_hostname_ctr,0,sizeof(mdns_mqtt_hostname_ctr));
    //       memcpy(mdns_mqtt_hostname_ctr,MDNS_MQTT_HOSTNAME1,strlen(MDNS_MQTT_HOSTNAME1));
    //     }

    //     Serial.println(mdns_mqtt_hostname_ctr);

    //     #ifdef ESP8266
    //       pubsub->setServer(mdns_mqtt_hostname_ctr, 1883);
    //       Serial.print("mMQTT:setServer> \""); Serial.print(mdns_mqtt_hostname_ctr); Serial.println("\"");
    //       //IPAddress mqqtserver(192,168,1,99); //desktop
    //       //pubsub->setServer(mqqtserver, 1883);
    //     #endif
    //     #ifdef ESP32
    //       IPAddress mqqtserver(192,168,1,99); //desktop
    //       pubsub->setServer(mqqtserver, 1883);
    //     #endif

    //   }
    // #endif

  }
} // END function



/************ PARSERS ************************************************************************************************************************************************
*********************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************/

//<devicename>/set/<function>/<subfunction>
//<devicename>/status/<function>/<subfunction>
void mMQTT::parse_JSONCommand(){

  // Check if instruction is for me
  if(mSupport::mSearchCtrIndexOf(data_buffer2.topic.ctr,"set/mqtt")>=0){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC_COMMAND " mqtt"));
    pCONT->fExitTaskerWithCompletion = true; // set true, we have found our handler
  }else{
    return; // not meant for here
  }

  
    
  //new topic names must include pixels
  
  if(strstr(data_buffer2.topic.ctr,"/system")){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC "system"));    
    //isserviced += parsesub_MQTTSettingsCommand();
  }else
  if(strstr(data_buffer2.topic.ctr,"hacs_firmware/information")){
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC "hacs_firmware/information"));    
    //isserviced += parsesub_FirmwareInformation();
  }else
  {
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_MQTT D_PARSING_MATCHED D_TOPIC "INVALID"));    
  } 

} // END function


//,(animation_override.fRefreshAllPixels?"Set":"UNSET")
void mMQTT::parsesub_MQTTSettingsCommand(){

  int8_t tmp_id = 0;

  #ifdef JSONDOCUMENT_STATIC
    StaticJsonDocument<800> doc;
  #else
    DynamicJsonDocument doc(600);
  #endif
  DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);
  
  if(error){
    AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_ERROR_JSON_DESERIALIZATION));
    Response_mP(S_JSON_COMMAND_SVALUE, D_ERROR,D_ERROR_JSON_DESERIALIZATION);
    return;
  }
  JsonObject obj = doc.as<JsonObject>();
  
  if(!obj["command"].isNull()){ 
    const char* command = obj["command"];
    if(strcmp(command,"system_send_all")==0){ 
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_NEO D_PARSING_MATCHED "\"command\"=\"system_send_all\""));
      //MQTTHandler_Set_fSendNow();
      data_buffer2.isserviced++;
    }
    else{
      AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_NEO D_PARSING_NOMATCH));
    }
  }

  

} // END FUNCTION


// New sender functions to replace ppublish call





void mMQTT::MQTTHandler_Send_Formatted(uint8_t topic_type, uint8_t module_id, const char* postfix_topic_ctr){

  PGM_P module_ctr = pCONT->GetModuleFriendlyName(module_id);

  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MQTT "MQTTHandler_Send_Formatted %d %s %s"),topic_type,module_ctr,postfix_topic_ctr);

  publish_ft(module_ctr,
             topic_type,
             postfix_topic_ctr,
             data_buffer2.payload.ctr,
             pCONT_set->Settings.sensors.flags.mqtt_retain
            );

}


//formatted topic
void mMQTT::publish_ft(const char* module_name, uint8_t topic_type_id, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  //can be replaced with a function call
  switch(topic_type_id){
    case MQTT_TOPIC_TYPE_SYSTEM_ID: break; // nothing
    case MQTT_TOPIC_TYPE_IFCHANGED_ID: sprintf(topic_id_ctr,"%s/",MQTT_TOPIC_TYPE_IFCHANGED_CTR); break;
    case MQTT_TOPIC_TYPE_ROC1M_ID: sprintf(topic_id_ctr,"%s/","roc1m"); break;
    case MQTT_TOPIC_TYPE_ROC10M_ID: sprintf(topic_id_ctr,"%s/","roc10m"); break;
    case MQTT_TOPIC_TYPE_TELEPERIOD_ID: sprintf(topic_id_ctr,"%s/",MQTT_TOPIC_TYPE_TELEPERIOD_CTR); break;
    default: sprintf(topic_id_ctr,"%s/","ERROR"); break;
  }

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  snprintf_P(topic_ctr, sizeof(topic_ctr), PSTR("%s/%s/%s%S"),D_TOPIC_STATUS,module_name,topic_id_ctr,topic_postfix); 

  AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MQTT "topic_ctr=\"%s\""),topic_ctr);
  
  ppublish(topic_ctr,payload_ctr,retain_flag);

}

 
void mMQTT::publish_status_module(const char* module_name, const char* topic_postfix, const char* payload_ctr, uint8_t retain_flag){

  char topic_ctr[100]; memset(topic_ctr,0,sizeof(topic_ctr));
  char topic_id_ctr[30]; memset(topic_id_ctr,0,sizeof(topic_id_ctr));
  
  sprintf(topic_ctr,PSTR("%s/%s/%s"),D_TOPIC_STATUS,module_name,topic_postfix); 
  
  ppublish(topic_ctr,payload_ctr,retain_flag);

}


// My function for adding prefix by device name
boolean mMQTT::ppublish(const char* topic, const char* payload, boolean retained){

  // return 0;

        // AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "!pubsub->connected() BEFORE "));
    if (!pubsub->connected()) {
        AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "NOT CONNECTED \"ppublish\" failed!"));
        return 0;
    }

    if(strlen(payload)<1){
        AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "strlen(payload)<1"));
    }

// Serial.println(WiFi.localIP());
// Serial.println(static_cast<uint32_t>(WiFi.localIP())); 

    if (!pCONT_wif->WifiCheckIpConnected()) {
        AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early pp"));
        return false;
    }else{
        //AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "WiFi.status()=%d"),WiFi.status());
    }

    //if(prefixtopic[0]!=0){}
//     AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Exiting early?"));
//     // delay(2500); ESP.wdtFeed();
// Serial.print("HERE1");
// Serial.flush();
    // delay(2500); ESP.wdtFeed();
DEBUG_LINE;

if(WiFi.status() != WL_CONNECTED){ 
  AddLog_P(LOG_LEVEL_ERROR, PSTR(D_LOG_PUBSUB "Unable to publish no connection -- Exiting early"));
  return false;
}

DEBUG_LINE;

//return 0;//

    char convctr[100]; memset(convctr,0,sizeof(convctr));
    // Serial.println(settings.prefixtopic); Serial.flush();
    // Serial.println(topic); Serial.flush();
    snprintf(convctr,sizeof(convctr),PSTR("%s/%S"),settings.prefixtopic,topic);
    
    // Serial.println(convctr); Serial.flush();
DEBUG_LINE;
//TRACE();
    // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_PUBSUB "-->" D_TOPIC " [%s] %d"),convctr,strlen(convctr));
    // AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_PUBSUB "-->" D_PAYLOAD " [%s] %d"),payload,strlen(payload));

DEBUG_LINE;
// //TRACE();
//     #ifdef SERIAL_DEBUG_LOW_LEVEL
//     if(strstr(payload,"{}")){
//         AddLog_P(LOG_LEVEL_DEBUG_MORE, PSTR(D_LOG_PUBSUB D_ERROR "> {}"));
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


boolean mMQTT::psubscribe(const char* topic) {
  char ttopic[70];
  memset(ttopic,0,sizeof(ttopic));
  sprintf(ttopic,PSTR("%s/%s"),settings.prefixtopic,topic);
  return pubsub->subscribe(ttopic, 0);
}


 

// A future PSTR mqtt sender for topic name
// Sender(payload,retain,topic,....)
// void AddLog_NoTime(uint8_t loglevel, PGM_P formatP, ...)
// {

//   // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
//   if(
//     (loglevel>mcl2->mset->Settings.seriallog_level)&&
//     (loglevel>mcl2->mset->Settings.weblog_level)
//     ){
//     return;
//   }

//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(mcl2->mset->log_data, sizeof(mcl2->mset->log_data), formatP, arg);
//   va_end(arg);

//   // LOG : SERIAL
//   if (loglevel <= mcl2->mset->Settings.seriallog_level) {
//     Serial.printf("%s%s %s\r\n", 
//     mxtime,
//     mcl2->mso->GetLogLevelNameShortbyID(loglevel),  
//     mcl2->mset->log_data);
//     //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
//     Serial.flush();
//   }

//   // LOG : TELNET
//   if (loglevel <= mcl2->mset->Settings.telnetlog_level) {
//     if((mcl2->mso!=NULL)&&(mcl2->mso->server!=NULL)){
//       if(!mcl2->mso->client.connected()) {
//         mcl2->mso->client = mcl2->mso->server->available();
//       }
//       if(mcl2->mso->client.connected()) {
//         mcl2->mso->client.printf("%s%s %s\r\n", mxtime,mcl2->mso->GetLogLevelNameShortbyID(loglevel), mcl2->mset->log_data);
//       }
//     }
//   }



/************ JSON BUILDER CALLS ************************************************************************************************************************************************
*******************************************************************************************************************************************************************************
*********************************************************************************************************************************************************************************
*******************************************************************************************************************************************************************************/






void mMQTT::MQTTHandler_Set_fSendNow(){
  mqtthandler_health.flags.SendNow = true;
  mqtthandler_settings.flags.SendNow = true;
  // mqtthandler_parameters.flags.SendNow = true;
  mqtthandler_log.flags.SendNow = true;
  mqtthandler_firmware.flags.SendNow = true;
  mqtthandler_memory.flags.SendNow = true;
  mqtthandler_network.flags.SendNow = true;
  mqtthandler_mqtt.flags.SendNow = true;
  mqtthandler_time.flags.SendNow = true;
  mqtthandler_devices.flags.SendNow = true;
  mqtthandler_reboot.flags.SendNow = true;
  mqtthandler_debug_pins.flags.SendNow = true;
  mqtthandler_debug_template.flags.SendNow = true;
  mqtthandler_debug_moduleinterface.flags.SendNow = true;
  mqtthandler_debug_minimal.flags.SendNow = true;



} //end "MQTTHandler_Init


//move system sender INTO telemetry class, remove intermediatery functions
void mMQTT::MQQTHandler_System_Sender(uint8_t status_id_requested){

  #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_TEST " MQQTHandler_System_Sender"));
  #endif

  uint8_t status_id = 0;
  uint8_t flag_run_once = false;
  
  // If we target a status, only send it then exit
  if(status_id_requested!=MQTT_HANDLER_SYSTEM_ALL_ID){
    status_id = status_id_requested;
    flag_run_once = true;
  }

  do{

    #ifdef ENABLE_ADVANCED_DEBUGGING
      AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_TEST " MQQTHandler_System_Sender %d"),status_id);
    #endif

    switch(status_id){
      case MQTT_HANDLER_SYSTEM_HEALTH_ID:           mqtthandler_ptr = &mqtthandler_health;  break;
      case MQTT_HANDLER_SYSTEM_SETTINGS_ID:            mqtthandler_ptr = &mqtthandler_settings;  break;
      // case MQTT_HANDLER_SYSTEM_PARAMETERS_ID:       mqtthandler_ptr = &mqtthandler_parameters; break;
      case MQTT_HANDLER_SYSTEM_LOG_ID:              mqtthandler_ptr = &mqtthandler_log; break;
      case MQTT_HANDLER_SYSTEM_FIRMWARE_ID:         mqtthandler_ptr = &mqtthandler_firmware; break;
      case MQTT_HANDLER_SYSTEM_MEMORY_ID:           mqtthandler_ptr = &mqtthandler_memory; break;
      case MQTT_HANDLER_SYSTEM_NETWORK_ID:          mqtthandler_ptr = &mqtthandler_network; break;
      case MQTT_HANDLER_SYSTEM_MQTT_ID:             mqtthandler_ptr = &mqtthandler_mqtt; break;
      case MQTT_HANDLER_SYSTEM_TIME_ID:             mqtthandler_ptr = &mqtthandler_time; break;
      case MQTT_HANDLER_SYSTEM_DEVICES_ID:          mqtthandler_ptr = &mqtthandler_devices; break;
      case MQTT_HANDLER_SYSTEM_REBOOT_ID:           mqtthandler_ptr = &mqtthandler_reboot; break;
      case MQTT_HANDLER_SYSTEM_REBOOT_EVENT_ID:     mqtthandler_ptr = &mqtthandler_reboot_event; break;
      #ifdef ENABLE_MQTT_DEBUG_TELEMETRY
      case MQTT_HANDLER_SYSTEM_DEBUG_PINS_ID:       mqtthandler_ptr = &mqtthandler_debug_pins; break;
      case MQTT_HANDLER_SYSTEM_DEBUG_TEMPLATE_ID:   mqtthandler_ptr = &mqtthandler_debug_template; break;
      case MQTT_HANDLER_SYSTEM_DEBUG_MODULEINTERFACE_ID:   mqtthandler_ptr = &mqtthandler_debug_moduleinterface; break;
      case MQTT_HANDLER_SYSTEM_DEBUG_MINIMAL_ID:   mqtthandler_ptr = &mqtthandler_debug_minimal; break;
      #endif
    }

    #ifdef DEBUG_MQTT_HANDLER
      mqtthandler_ptr->tRateSecs = 1;
    #endif
    
    MQTTHandler_Command(*this,D_MODULE_CORE_TELEMETRY_ID,mqtthandler_ptr);
    
  }while(
    (++status_id<MQTT_HANDLER_SYSTEM_SYSTEM_LENGTH_ID)&&
    (!flag_run_once)
  );

}



uint8_t mMQTT::ConstructJSON_Health(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Health(json_level);
}
uint8_t mMQTT::ConstructJSON_Settings(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Settings(json_level);
}
// uint8_t mMQTT::ConstructJSON_Parameters(uint8_t json_level){
//   return pCONT_tel->ConstructJSON_Parameters(json_level);
// }
uint8_t mMQTT::ConstructJSON_Firmware(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Firmware(json_level);
}
uint8_t mMQTT::ConstructJSON_Log(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Log(json_level);
}
uint8_t mMQTT::ConstructJSON_Memory(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Memory(json_level);
}
uint8_t mMQTT::ConstructJSON_Network(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Network(json_level);
}
uint8_t mMQTT::ConstructJSON_Time(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Time(json_level);
}
uint8_t mMQTT::ConstructJSON_Reboot(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Reboot(json_level);
}
// uint8_t mMQTT::ConstructJSON_Reboot_Event(uint8_t json_level){
//   return pCONT_tel->ConstructJSON_Reboot_Event(json_level);
// }
uint8_t mMQTT::ConstructJSON_MQTT(uint8_t json_level){
  return pCONT_tel->ConstructJSON_MQTT(json_level);
}
uint8_t mMQTT::ConstructJSON_Devices(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Devices(json_level);
}
#ifdef ENABLE_MQTT_DEBUG_TELEMETRY
/***
 * 
 * DEBUGING packets that are only sent as extra information during code development and bug tracking
 * 
 * */
uint8_t mMQTT::ConstructJSON_Debug_Pins(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Debug_Pins(json_level);
}
uint8_t mMQTT::ConstructJSON_Debug_Template(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Debug_Template(json_level);
}
uint8_t mMQTT::ConstructJSON_Debug_ModuleInterface(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Debug_ModuleInterface(json_level);
}
uint8_t mMQTT::ConstructJSON_Debug_Minimal(uint8_t json_level){
  return pCONT_tel->ConstructJSON_Debug_Minimal(json_level);
}
#endif



int8_t mMQTT::Tasker(uint8_t function){

  if(pCONT_set->Settings.flag_system.mqtt_enabled){

  switch(function){
    case FUNC_INIT:
      init();
    break;
    case FUNC_JSON_COMMAND:
      parse_JSONCommand(); // mqtt config commands
    break;
    case FUNC_LOOP: 

      if(WiFi.status() != WL_CONNECTED) {
        //#ifdef ENABLE_ADVANCED_DEBUGGING
          //AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_MQTT "WiFi.status() != WL_CONNECTED : Exiting MQTT"));
        //#endif
        break;
      }

      if((!pubsub->connected())&&mSupport::TimeReached(&connection_maintainer.tSavedReconnectAttempt,connection_maintainer.rSavedReconnectAttempt)){    // wait 1-10 seconds before retrying to prevent rush event
        connection_maintainer.rSavedReconnectAttempt = random(1,20)*500; // random backup period (0:0.5:10 seconds)
        pCONT->Tasker_Interface(FUNC_MQTT_DISCONNECTED);
        connect();
      }else{
        if(mSupport::TimeReached(&connection_maintainer.tSavedLoop,100)){   // https://github.com/knolleary/pubsubclient/issues/556
          pubsub->loop();
          //AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_MQTT "pubsub->loop()"));
          // New data
          if(data_buffer2.fWaiting){data_buffer2.fWaiting = false;
            // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "<-- NEWTopic   [len:%d] %s"),data_buffer2.topic.len,  data_buffer2.topic.ctr);
            // AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "<-- NEWPayload [len:%d] %s"),data_buffer2.payload.len,data_buffer2.payload.ctr);
            
            Serial.println("DeserializationError BEFORE"); Serial.flush();

            StaticJsonDocument<MQTT_MAX_PACKET_SIZE> doc;
            DeserializationError error = deserializeJson(doc, data_buffer2.payload.ctr);

            Serial.println("DeserializationError AFTER"); Serial.flush();

            if(error){
              AddLog_P(LOG_LEVEL_ERROR, PSTR("DeserializationError")); Serial.flush();
            }

            // Check for json formatted messages
            if(!error){
              pCONT->Tasker_Interface(FUNC_JSON_COMMAND_OBJECT_WITH_TOPIC, doc.as<JsonObjectConst>());
            }

            //old method for now
            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "USING OLD JSON METHOD"));
            if(pCONT->Tasker_Interface(FUNC_JSON_COMMAND)){
              AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT "NEW METHOD SUCCESS"));
            }else{
              AddLog_P(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_PARSING_NOMATCH));
            }






            AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT "isserviced %d"),data_buffer2.isserviced);
          }
        }
      }

      if((WiFi.status() != WL_CONNECTED)&&(!pCONT->mqt->pubsub->connected())){ 
        //AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT " Not publishing with no network!")); 
        break; 
      }

      #ifdef ENABLE_ADVANCED_DEBUGGING
        AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL, PSTR(D_LOG_UPTIME "MQTT %s"),pCONT->mt->uptime.hhmmss_ctr);
      #endif
      
      // Send system telemetry from "mTelemetry.h"
      MQQTHandler_System_Sender();

      // Send mqtt from all modules
      pCONT->Tasker_Interface(FUNC_MQTT_SENDER);

    break;
    case FUNC_EVERY_SECOND:

// Serial.println("flag_uptime_reached_reduce_frequency");
            
//             handler_ptr->tRateSecs = 10;


// AddLog_P(LOG_LEVEL_INFO,PSTR("tRateSecs = %d %d %d"),mqtthandler_health.tRateSecs,mqtthandler_health.flags.FrequencyRedunctionLevel,flag_uptime_reached_reduce_frequency);

    break;
    case FUNC_EVERY_MINUTE:
      //DiscoverServer();
      //pCONT->Tasker_Interface(FUNC_MQTT_CHECK_REDUNCTION_LEVEL);
    break;
    case FUNC_UPTIME_1_MINUTES:

    break;
    case FUNC_UPTIME_10_MINUTES:
      // mqtthandler_debug_minimal.tRateSecs = 120;
      // //#ifndef DEVICE_MYBEDROOM
      // mqtthandler_health.tRateSecs = 120;
      // //#endif
flag_uptime_reached_reduce_frequency = true;
    break;
    // case FUNC_MQTT_CHECK_REDUNCTION_LEVEL:

    // break;    
// #ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS:
      // pCONT_web->WSBufferAppend_P(request, HTTP_BTN_MENU_MQTT);
    break;
    case FUNC_WEB_ADD_HANDLER:
      // pCONT_web->pWebServer->on("/" WEB_HANDLE_MQTT, [this](){HandleMqttConfiguration(); });
    break;
// #endif  // USE_WEBSERVER


  }

}//enabled off

} // END function
