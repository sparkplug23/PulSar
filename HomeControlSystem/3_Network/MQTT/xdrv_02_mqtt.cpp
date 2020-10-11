// #ifdef ESP8266

// #define XDRV_02                2

// #include "0_ConfigUser/mUserConfig.h"

// // #ifdef USE_MQTT_TLS
// // #ifdef USE_MQTT_TLS_CA_CERT
// //   #include "sonoff_letsencrypt.h"           // LetsEncrypt certificate
// // #endif
// //   WiFiClientSecure EspClient;               // Wifi Secure Client
// // #else
//   WiFiClient EspClient;                     // Wifi Client
// // #endif

// enum MqttCommands {
//   CMND_MQTTHOST, CMND_MQTTPORT, CMND_MQTTRETRY, CMND_STATETEXT, CMND_MQTTFINGERPRINT, CMND_MQTTCLIENT,
//   CMND_MQTTUSER, CMND_MQTTPASSWORD, CMND_FULLTOPIC, CMND_PREFIX, CMND_GROUPTOPIC, CMND_TOPIC, CMND_PUBLISH,
//   CMND_BUTTONTOPIC, CMND_SWITCHTOPIC, CMND_BUTTONRETAIN, CMND_SWITCHRETAIN, CMND_POWERRETAIN, CMND_SENSORRETAIN };
// const char kMqttCommands[] PROGMEM =
//   D_JSON_MQTTHOST "|" D_JSON_MQTTPORT "|" D_JSON_MQTTRETRY "|" D_JSON_STATETEXT "|" D_JSON_MQTTFINGERPRINT "|" D_JSON_MQTTCLIENT "|"
//   D_JSON_MQTTUSER "|" D_JSON_MQTTPASSWORD "|" D_JSON_FULLTOPIC "|" D_JSON_PREFIX "|" D_JSON_GROUPTOPIC "|" D_JSON_TOPIC "|" D_JSON_PUBLISH "|"
//   D_JSON_BUTTONTOPIC "|" D_JSON_SWITCHTOPIC "|" D_JSON_BUTTONRETAIN "|" D_JSON_SWITCHRETAIN "|" D_JSON_POWERRETAIN "|" D_JSON_SENSORRETAIN ;

// IPAddress mqtt_host_addr;                   // MQTT host IP address
// uint32_t mqtt_host_hash = 0;                // MQTT host name hash

// uint16_t mqtt_connect_count = 0;            // MQTT re-connect count
// uint16_t mqtt_retry_counter = 1;            // MQTT connection retry counter
// uint8_t mqtt_initial_connection_state = 2;  // MQTT connection messages state
// bool mqtt_connected = false;                // MQTT virtual connection status
// bool mqtt_allowed = false;                  // MQTT enabled and parameters valid

// /*********************************************************************************************\
//  * MQTT driver specific code need to provide the following functions:
//  *
//  * bool MqttIsConnected()
//  * void MqttDisconnect()
//  * void MqttSubscribeLib(char *topic)
//  * bool MqttPublishLib(const char* topic, bool retained)
// \*********************************************************************************************/

// #include <PubSubClient.h>

// // Max message size calculated by PubSubClient is (MQTT_MAX_PACKET_SIZE < 5 + 2 + strlen(topic) + plength)
// #if (MQTT_MAX_PACKET_SIZE -TOPSZ -7) < MIN_MESSZ  // If the max message size is too small, throw an error at compile time. See PubSubClient.cpp line 359
//   #error "MQTT_MAX_PACKET_SIZE is too small in libraries/PubSubClient/src/PubSubClient.h, increase it to at least 1000"
// #endif

// PubSubClient MqttClient(EspClient);

// bool MqttIsConnected(void)
// {
//   return mqt.pubsub->connected();//1;//M qttClient.connected();
// }

// void MqttDisconnect(void)
// {
//   mqt.pubsub->disconnect();
//   //M qttClient.disconnect();
// }

// void MqttSubscribeLib(const char *topic)
// {
//   mqt.pubsub->subscribe(topic);
//   mqt.pubsub->loop();
//   // M qttClient.subscribe(topic);
//   // M qttClient.loop();  // Solve LmacRxBlk:1 messages
// }

// void MqttUnsubscribeLib(const char *topic)
// {
//   mqt.pubsub->unsubscribe(topic);
//   mqt.pubsub->loop();
//   // M qttClient.unsubscribe(topic);
//   // M qttClient.loop();  // Solve LmacRxBlk:1 messages
// }

// bool MqttPublishLib(const char* topic, bool retained)
// {
// //  bool result = M qttClient.publish(topic, mcl->mset->data_buffer.payload.ctr, retained);
//     mqt.pubsub->publish(topic,mcl->mset->data_buffer.payload.ctr,true);
//     // Serial.print("topic>> ");
//     // Serial.println(topic);
//     // Serial.print("mcl->mset->data_buffer.payload.ctr>>");
//     // Serial.println(mcl->mset->data_buffer.payload.ctr);

//   yield();  // #3313
//   return 1;
// }



































// /*********************************************************************************************/


// void MqttRetryCounter(uint8_t value)
// {
//   mqtt_retry_counter = value;
// }

// void MqttSubscribe(const char *topic)
// {
//   AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_SUBSCRIBE_TO " %s"), topic);
//   MqttSubscribeLib(topic);
// }

// void MqttUnsubscribe(const char *topic)
// {
//   AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_UNSUBSCRIBE_FROM " %s"), topic);
//   MqttUnsubscribeLib(topic);
// }

// void MqttPublishDirect(const char* topic, bool retained)
// {
//   char sretained[CMDSZ];
//   char slog_type[10];

// #ifdef USE_DEBUG_DRIVER
//   //(PSTR("MqttPublishDirect"));
// #endif

//   sretained[0] = '\0';
//   snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_RESULT));

//   if (Settings.flag_system.mqtt_enabled) {
//     if (MqttIsConnected()) {
//       if (MqttPublishLib(topic, retained)) {
//         snprintf_P(slog_type, sizeof(slog_type), PSTR(D_LOG_MQTT));
//         if (retained) {
//           snprintf_P(sretained, sizeof(sretained), PSTR(" (" D_RETAINED ")"));
//         }
//       }
//     }
//   }

//   snprintf_P(log_data, sizeof(log_data), PSTR("%s%s = %s"), slog_type, (Settings.flag_system.mqtt_enabled) ? topic : strrchr(topic,'/')+1, mcl->mset->data_buffer.payload.ctr);
//   if (strlen(log_data) >= (sizeof(log_data) - strlen(sretained) -1)) {
//     log_data[sizeof(log_data) - strlen(sretained) -5] = '\0';
//     snprintf_P(log_data, sizeof(log_data), PSTR("%s ..."), log_data);
//   }
//   snprintf_P(log_data, sizeof(log_data), PSTR("%s%s"), log_data, sretained);
//   AddLogAddLog(LOG_LEVEL_INFO);

//   if (Settings.ledstate &0x04) {
//     blinks++;
//   }
// }

// void MqttPublish(const char* topic, bool retained)
// {
//   char *me;

//   if (!strcmp(Settings.mqtt_prefix[0],Settings.mqtt_prefix[1])) {
//     me = strstr(topic,Settings.mqtt_prefix[0]);
//     if (me == topic) {
//       mqtt_cmnd_publish += 3;
//     }
//   }
//   MqttPublishDirect(topic, retained);
// }

// void MqttPublish(const char* topic)
// {
//   MqttPublish(topic, false);
// }

// void MqttPublishPrefixTopic_P(uint8_t prefix, const char* subtopic, bool retained)
// {
// /* prefix 0 = cmnd using subtopic
//  * prefix 1 = stat using subtopic
//  * prefix 2 = tele using subtopic
//  * prefix 4 = cmnd using subtopic or RESULT
//  * prefix 5 = stat using subtopic or RESULT
//  * prefix 6 = tele using subtopic or RESULT
//  */
//   char romram[33];
//   char stopic[TOPSZ];

//   snprintf_P(romram, sizeof(romram), ((prefix > 3) && !Settings.flag_system.mqtt_response) ? S_RSLT_RESULT : subtopic);
//   for (uint8_t i = 0; i < strlen(romram); i++) {
//     romram[i] = toupper(romram[i]);
//   }
//   prefix &= 3;
//   GetTopic_P(stopic, prefix, mqtt_topic, romram);
//   MqttPublish(stopic, retained);
// }

// void MqttPublishPrefixTopic_P(uint8_t prefix, const char* subtopic)
// {
//   MqttPublishPrefixTopic_P(prefix, subtopic, false);
// }

// void MqttPublishPowerState(uint8_t device)
// {
//   char stopic[TOPSZ];
//   char scommand[33];

//   if ((device < 1) || (device > devices_present)) { device = 1; }

// //   if ((SONOFF_IFAN02 == my_module_type) && (device > 1)) {
// //     if (GetFanspeed() < MAX_FAN_SPEED) {  // 4 occurs when fanspeed is 3 and RC button 2 is pressed
// // #ifdef USE_DOMOTICZ
// //     //  DomoticzUpdateFanState();  // RC Button feedback
// // #endif  // USE_DOMOTICZ
// //       snprintf_P(scommand, sizeof(scommand), PSTR(D_JSON_FANSPEED));
// //       GetTopic_P(stopic, STAT, mqtt_topic, (Settings.flag_system.mqtt_response) ? scommand : S_RSLT_RESULT);
// //       Response_P(S_JSON_COMMAND_NVALUE, scommand, GetFanspeed());
// //       MqttPublish(stopic);
// //     }
// //   } else {
//     GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag_system.device_index_enable);
//     GetTopic_P(stopic, STAT, mqtt_topic, (Settings.flag_system.mqtt_response) ? scommand : S_RSLT_RESULT);
//     Response_P(S_JSON_COMMAND_SVALUE, scommand, GetStateText(bitRead(power, device -1)));
//     MqttPublish(stopic);

//     GetTopic_P(stopic, STAT, mqtt_topic, scommand);
//     Response_P(GetStateText(bitRead(power, device -1)));
//     MqttPublish(stopic, Settings.flag_system.mqtt_power_retain);
//  // }
// }

// void MqttPublishPowerBlinkState(uint8_t device)
// {
//   char scommand[33];

//   if ((device < 1) || (device > devices_present)) {
//     device = 1;
//   }
//   Response_P(PSTR("{\"%s\":\"" D_JSON_BLINK " %s\"}"),
//     GetPowerDevice(scommand, device, sizeof(scommand), Settings.flag_system.device_index_enable), GetStateText(bitRead(blink_mask, device -1)));

//   MqttPublishPrefixTopic_P(RESULT_OR_STAT, S_RSLT_POWER);
// }

// /*********************************************************************************************/

// uint16_t MqttConnectCount()
// {
//   return mqtt_connect_count;
// }

// void MqttDisconnected(int state)
// {
//   mqtt_connected = false;
//   mqtt_retry_counter = Settings.mqtt_retry;

//   AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_MQTT D_CONNECT_FAILED_TO " %s:%d, rc %d. " D_RETRY_IN " %d " D_UNIT_SECOND), Settings.mqtt_host, Settings.mqtt_port, state, mqtt_retry_counter);
//   rules_flag.mqtt_disconnected = 1;
// }

// void MqttConnected(void)
// {
//   char stopic[TOPSZ];

//   if (mqtt_allowed) {
//     AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_CONNECTED));
//     mqtt_connected = true;
//     mqtt_retry_counter = 0;
//     mqtt_connect_count++;

//     GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
//     Response_P(PSTR(D_ONLINE));
//     MqttPublish(stopic, true);

//     // Satisfy iobroker (#299)
//     mcl->mset->data_buffer.payload.ctr[0] = '\0';
//     MqttPublishPrefixTopic_P(CMND, S_RSLT_POWER);

//     GetTopic_P(stopic, CMND, mqtt_topic, PSTR("#"));
//     MqttSubscribe(stopic);
//     if (strstr_P(Settings.mqtt_fulltopic, MQTT_TOKEN_TOPIC) != nullptr) {
//       GetTopic_P(stopic, CMND, Settings.mqtt_grptopic, PSTR("#"));
//       MqttSubscribe(stopic);
//       GetFallbackTopic_P(stopic, CMND, PSTR("#"));
//       MqttSubscribe(stopic);
//     }

//     XdrvCall(FUNC_MQTT_SUBSCRIBE);
//   }

//   if (mqtt_initial_connection_state) {
//     Response_P(PSTR("{\"" D_JSON_MODULE "\":\"%s\",\"" D_JSON_VERSION "\":\"%s%s\",\"" D_JSON_FALLBACKTOPIC "\":\"%s\",\"" D_JSON_GROUPTOPIC "\":\"%s\"}"),
//       ModuleName().c_str(), my_version, my_image, GetFallbackTopic_P(stopic, CMND, ""), Settings.mqtt_grptopic);
//     MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "1"));
// #ifdef USE_WEBSERVER
//     if (Settings.webserver) {
//       Response_P(PSTR("{\"" D_JSON_WEBSERVER_MODE "\":\"%s\",\"" D_JSON_HOSTNAME "\":\"%s\",\"" D_JSON_IPADDRESS "\":\"%s\"}"),
//         (2 == Settings.webserver) ? D_ADMIN : D_USER, my_hostname, WiFi.localIP().toString().c_str());
//       MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "2"));
//     }
// #endif  // USE_WEBSERVER
//     Response_P(PSTR("{\"" D_JSON_RESTARTREASON "\":\"%s\"}"), (GetResetReason() == "Exception") ? ESP.getResetInfo().c_str() : GetResetReason().c_str());
//     MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_INFO "3"));
//     for (uint8_t i = 1; i <= devices_present; i++) {
//       MqttPublishPowerState(i);
//       if (SONOFF_IFAN02 == my_module_type) { break; }  // Report status of light relay only
//     }
//     if (Settings.tele_period) { tele_period = Settings.tele_period -9; }  // Enable TelePeriod in 9 seconds
//     rules_flag.system_boot = 1;
//     XdrvCall(FUNC_MQTT_INIT);
//   }
//   mqtt_initial_connection_state = 0;

//   global_state.mqtt_down = 0;
//   if (Settings.flag_system.mqtt_enabled) {
//     rules_flag.mqtt_connected = 1;
//   }
// }

// // #ifdef USE_MQTT_TLS
// // bool MqttCheckTls(void)
// // {
// //   char fingerprint1[60];
// //   char fingerprint2[60];
// //   bool result = false;
// //
// //   fingerprint1[0] = '\0';
// //   fingerprint2[0] = '\0';
// //   for (uint8_t i = 0; i < sizeof(Settings.mqtt_fingerprint[0]); i++) {
// //     snprintf_P(fingerprint1, sizeof(fingerprint1), PSTR("%s%s%02X"), fingerprint1, (i) ? " " : "", Settings.mqtt_fingerprint[0][i]);
// //     snprintf_P(fingerprint2, sizeof(fingerprint2), PSTR("%s%s%02X"), fingerprint2, (i) ? " " : "", Settings.mqtt_fingerprint[1][i]);
// //   }
// //
// //   AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_FINGERPRINT));
// //
// // //#ifdef ARDUINO_ESP8266_RELEASE_2_4_1
// //   EspClient = WiFiClientSecure();               // Wifi Secure Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
// // //#endif
// //
// //   if (!EspClient.connect(Settings.mqtt_host, Settings.mqtt_port)) {
// //     AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_MQTT D_TLS_CONNECT_FAILED_TO " %s:%d. " D_RETRY_IN " %d " D_UNIT_SECOND), Settings.mqtt_host, Settings.mqtt_port, mqtt_retry_counter);
// //   } else {
// // #ifdef USE_MQTT_TLS_CA_CERT
// //     unsigned char tls_ca_cert[] = MQTT_TLS_CA_CERT;
// //     if (EspClient.setCACert(tls_ca_cert, MQTT_TLS_CA_CERT_LENGTH)) {
// //       if (EspClient.verifyCertChain(Settings.mqtt_host)) {
// //         AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "CA"));
// //         result = true;
// //       }
// //     }
// // #else
// //     if (EspClient.verify(fingerprint1, Settings.mqtt_host)) {
// //       AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "1"));
// //       result = true;
// //     }
// //     else if (EspClient.verify(fingerprint2, Settings.mqtt_host)) {
// //       AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "2"));
// //       result = true;
// //     }
// // #ifdef MDNS_HOSTNAME
// //     // If the hostname is set, check that as well.
// //     // This lets certs with the hostname for the CN be used.
// //     else if (EspClient.verify(fingerprint1, MDNS_HOSTNAME)) {
// //       AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "1"));
// //       result = true;
// //     }
// //     else if (EspClient.verify(fingerprint2, MDNS_HOSTNAME)) {
// //       AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_VERIFIED "2"));
// //       result = true;
// //     }
// // #endif  // MDNS_HOSTNAME
// // #endif  // USE_MQTT_TLS_CA_CERT
// //   }
// //   if (!result) AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_FAILED));
// //   EspClient.stop();
// //   yield();
// //   return result;
// // }
// // #endif  // USE_MQTT_TLS

// void MqttReconnect(void)
// {
//   char stopic[TOPSZ];

//   mqtt_allowed = Settings.flag_system.mqtt_enabled;
//   if (mqtt_allowed) {
// #ifdef USE_DISCOVERY
// #ifdef MQTT_HOST_DISCOVERY
//     MqttDiscoverServer();
// #endif  // MQTT_HOST_DISCOVERY
// #endif  // USE_DISCOVERY
//     if (!strlen(Settings.mqtt_host) || !Settings.mqtt_port) {
//       mqtt_allowed = false;
//     }
//   }
//   if (!mqtt_allowed) {
//     MqttConnected();
//     return;
//   }

// #if defined(USE_WEBSERVER) && defined(USE_EMULATION)
// //  UdpDisconnect();
// #endif  // USE_EMULATION

//   AddLog_P(LOG_LEVEL_INFO, S_LOG_MQTT, PSTR(D_ATTEMPTING_CONNECTION));

//   mqtt_connected = false;
//   mqtt_retry_counter = Settings.mqtt_retry;
//   global_state.mqtt_down = 1;

//   char *mqtt_user = nullptr;
//   char *mqtt_pwd = nullptr;
//   if (strlen(Settings.mqtt_user) > 0) mqtt_user = Settings.mqtt_user;
//   if (strlen(Settings.mqtt_pwd) > 0) mqtt_pwd = Settings.mqtt_pwd;

//   GetTopic_P(stopic, TELE, mqtt_topic, S_LWT);
//   Response_P(S_OFFLINE);

// // #ifdef USE_MQTT_TLS
// //   EspClient = WiFiClientSecure();         // Wifi Secure Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
// // #else
//   EspClient = WiFiClient();               // Wifi Client reconnect issue 4497 (https://github.com/esp8266/Arduino/issues/4497)
// //#endif

//   if (2 == mqtt_initial_connection_state) {  // Executed once just after power on and wifi is connected
// // #ifdef USE_MQTT_TLS
// //     if (!MqttCheckTls()) return;
// // #endif  // USE_MQTT_TLS

//     mqtt_initial_connection_state = 1;
//   }
// /*
//   MqttClient.setCallback(MqttDataHandler);
//   MqttClient.setServer(Settings.mqtt_host, Settings.mqtt_port);
// /*
//   // Skip MQTT host DNS lookup if not needed
//   uint32_t current_hash = GetHash(Settings.mqtt_host, strlen(Settings.mqtt_host));
//   if (mqtt_host_hash != current_hash) {
//     mqtt_host_hash = current_hash;
//     WiFi.hostByName(Settings.mqtt_host, mqtt_host_addr);  // Skips DNS lookup if mqtt_host is IP address string as from mDns
//   }
//   MqttClient.setServer(mqtt_host_addr, Settings.mqtt_port);
// *
//   if (MqttClient.connect(mqtt_client, mqtt_user, mqtt_pwd, stopic, 1, true, mcl->mset->data_buffer.payload.ctr)) {
//     MqttConnected();
//   } else {
//     MqttDisconnected(MqttClient.state());  // status codes are documented here http://pubsubclient.knolleary.net/api.html#state
//   }
//   */
// }

// void MqttCheck(void)
// {
//   if (Settings.flag_system.mqtt_enabled) {
//     if (!MqttIsConnected()) {
//       global_state.mqtt_down = 1;
//       if (!mqtt_retry_counter) {
// #ifdef USE_DISCOVERY
// #ifdef MQTT_HOST_DISCOVERY
//         if (!strlen(Settings.mqtt_host) && !mdns_ begun) { return; }
// #endif  // MQTT_HOST_DISCOVERY
// #endif  // USE_DISCOVERY
//         MqttReconnect();
//       } else {
//         mqtt_retry_counter--;
//       }
//     } else {
//       global_state.mqtt_down = 0;
//     }
//   } else {
//     global_state.mqtt_down = 0;
//     if (mqtt_initial_connection_state) MqttReconnect();
//   }
// }

// /*********************************************************************************************/

// bool MqttCommand(void)
// {
//   char command [CMDSZ];
//   bool serviced = true;
//   char stemp1[TOPSZ];
//   char scommand[CMDSZ];
//   uint16_t i;

//   uint16_t index = XdrvMailbox.index;
//   uint16_t data_len = XdrvMailbox.data_len;
//   uint16_t payload16 = XdrvMailbox.payload16;
//   int16_t payload = XdrvMailbox.payload;
//   bool grpflg =  XdrvMailbox.grpflg;
//   char *type = XdrvMailbox.topic;
//   char *dataBuf = XdrvMailbox.data;

//   int command_code = GetCommandCode(command, sizeof(command), type, kMqttCommands);
//   if (-1 == command_code) {
//     serviced = false;  // Unknown command
//   }
//   else if (CMND_MQTTHOST == command_code) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_host))) {
//       strlcpy(Settings.mqtt_host, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_HOST : dataBuf, sizeof(Settings.mqtt_host));
//       restart_flag = 2;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.mqtt_host);
//   }
//   else if (CMND_MQTTPORT == command_code) {
//     if (payload16 > 0) {
//       Settings.mqtt_port = (1 == payload16) ? MQTT_PORT : payload16;
//       restart_flag = 2;
//     }
//     Response_P(S_JSON_COMMAND_NVALUE, command, Settings.mqtt_port);
//   }
//   else if (CMND_MQTTRETRY == command_code) {
//     if ((payload >= MQTT_RETRY_SECS) && (payload < 32001)) {
//       Settings.mqtt_retry = payload;
//       mqtt_retry_counter = Settings.mqtt_retry;
//     }
//     Response_P(S_JSON_COMMAND_NVALUE, command, Settings.mqtt_retry);
//   }
//   else if ((CMND_STATETEXT == command_code) && (index > 0) && (index <= 4)) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.state_text[0]))) {
//       for(i = 0; i <= data_len; i++) {
//         if (dataBuf[i] == ' ') dataBuf[i] = '_';
//       }
//       strlcpy(Settings.state_text[index -1], dataBuf, sizeof(Settings.state_text[0]));
//     }
//     Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, GetStateText(index -1));
//   }
// // #ifdef USE_MQTT_TLS
// //   else if ((CMND_MQTTFINGERPRINT == command_code) && (index > 0) && (index <= 2)) {
// //     char fingerprint[60];
// //     if ((data_len > 0) && (data_len < sizeof(fingerprint))) {
// //       strlcpy(fingerprint, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? (1 == index) ? MQTT_FINGERPRINT1 : MQTT_FINGERPRINT2 : dataBuf, sizeof(fingerprint));
// //       char *p = fingerprint;
// //       for (uint8_t i = 0; i < 20; i++) {
// //         Settings.mqtt_fingerprint[index -1][i] = strtol(p, &p, 16);
// //       }
// //       restart_flag = 2;
// //     }
// //     fingerprint[0] = '\0';
// //     for (uint8_t i = 0; i < sizeof(Settings.mqtt_fingerprint[index -1]); i++) {
// //       snprintf_P(fingerprint, sizeof(fingerprint), PSTR("%s%s%02X"), fingerprint, (i) ? " " : "", Settings.mqtt_fingerprint[index -1][i]);
// //     }
// //     Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, fingerprint);
// //   }
// // #endif
//   else if (CMND_MQTTCLIENT == command_code) {
//     if (!grpflg && (data_len > 0) && (data_len < sizeof(Settings.mqtt_client))) {
//       strlcpy(Settings.mqtt_client, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_CLIENT_ID : dataBuf, sizeof(Settings.mqtt_client));
//       restart_flag = 2;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.mqtt_client);
//   }
//   else if (CMND_MQTTUSER == command_code) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_user))) {
//       strlcpy(Settings.mqtt_user, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_USER : dataBuf, sizeof(Settings.mqtt_user));
//       restart_flag = 2;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.mqtt_user);
//   }
//   else if (CMND_MQTTPASSWORD == command_code) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_pwd))) {
//       strlcpy(Settings.mqtt_pwd, (SC_CLEAR == Shortcut(dataBuf)) ? "" : (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_PASS : dataBuf, sizeof(Settings.mqtt_pwd));
//       Response_P(S_JSON_COMMAND_SVALUE, command, Settings.mqtt_pwd);
//       restart_flag = 2;
//     } else {
//       Response_P(S_JSON_COMMAND_ASTERIX, command);
//     }
//   }
//   else if (CMND_FULLTOPIC == command_code) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_fulltopic))) {
//       MakeValidMqtt(1, dataBuf);
//       if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
//       strlcpy(stemp1, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_FULLTOPIC : dataBuf, sizeof(stemp1));
//       if (strcmp(stemp1, Settings.mqtt_fulltopic)) {
//         Response_P((Settings.flag_system.mqtt_offline) ? S_OFFLINE : "");
//         MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);  // Offline or remove previous retained topic
//         strlcpy(Settings.mqtt_fulltopic, stemp1, sizeof(Settings.mqtt_fulltopic));
//         restart_flag = 2;
//       }
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.mqtt_fulltopic);
//   }
//   else if ((CMND_PREFIX == command_code) && (index > 0) && (index <= 3)) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_prefix[0]))) {
//       MakeValidMqtt(0, dataBuf);
//       strlcpy(Settings.mqtt_prefix[index -1], (SC_DEFAULT == Shortcut(dataBuf)) ? (1==index)?SUB_PREFIX:(2==index)?PUB_PREFIX:PUB_PREFIX2 : dataBuf, sizeof(Settings.mqtt_prefix[0]));
// //      if (Settings.mqtt_prefix[index -1][strlen(Settings.mqtt_prefix[index -1])] == '/') Settings.mqtt_prefix[index -1][strlen(Settings.mqtt_prefix[index -1])] = 0;
//       restart_flag = 2;
//     }
//     Response_P(S_JSON_COMMAND_INDEX_SVALUE, command, index, Settings.mqtt_prefix[index -1]);
//   }
//   else if (CMND_PUBLISH == command_code) {
//     if (data_len > 0) {
//       char *mqtt_part = strtok(dataBuf, " ");
//       if (mqtt_part) {
//         strlcpy(stemp1, mqtt_part, sizeof(stemp1));
//         mqtt_part = strtok(nullptr, " ");
//         if (mqtt_part) {
//           strlcpy(mcl->mset->data_buffer.payload.ctr, mqtt_part, sizeof(mcl->mset->data_buffer.payload.ctr));
//         } else {
//           mcl->mset->data_buffer.payload.ctr[0] = '\0';
//         }
//         MqttPublishDirect(stemp1, (index == 2));
// //        Response_P(S_JSON_COMMAND_SVALUE, command, D_JSON_DONE);
//         mcl->mset->data_buffer.payload.ctr[0] = '\0';
//       }
//     }
//   }
//   else if (CMND_GROUPTOPIC == command_code) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.mqtt_grptopic))) {
//       MakeValidMqtt(0, dataBuf);
//       if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
//       strlcpy(Settings.mqtt_grptopic, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_GRPTOPIC : dataBuf, sizeof(Settings.mqtt_grptopic));
//       restart_flag = 2;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.mqtt_grptopic);
//   }
//   else if (CMND_TOPIC == command_code) {
//     if (!grpflg && (data_len > 0) && (data_len < sizeof(Settings.mqtt_topic))) {
//       MakeValidMqtt(0, dataBuf);
//       if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
//       strlcpy(stemp1, (SC_DEFAULT == Shortcut(dataBuf)) ? MQTT_TOPIC : dataBuf, sizeof(stemp1));
//       if (strcmp(stemp1, Settings.mqtt_topic)) {
//         Response_P((Settings.flag_system.mqtt_offline) ? S_OFFLINE : "");
//         MqttPublishPrefixTopic_P(TELE, PSTR(D_LWT), true);  // Offline or remove previous retained topic
//         strlcpy(Settings.mqtt_topic, stemp1, sizeof(Settings.mqtt_topic));
//         restart_flag = 2;
//       }
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.mqtt_topic);
//   }
//   else if (CMND_BUTTONTOPIC == command_code) {
//     if (!grpflg && (data_len > 0) && (data_len < sizeof(Settings.button_topic))) {
//       MakeValidMqtt(0, dataBuf);
//       if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
//       switch (Shortcut(dataBuf)) {
//         case SC_CLEAR: strlcpy(Settings.button_topic, "", sizeof(Settings.button_topic)); break;
//         case SC_DEFAULT: strlcpy(Settings.button_topic, mqtt_topic, sizeof(Settings.button_topic)); break;
//         case SC_USER: strlcpy(Settings.button_topic, MQTT_BUTTON_TOPIC, sizeof(Settings.button_topic)); break;
//         default: strlcpy(Settings.button_topic, dataBuf, sizeof(Settings.button_topic));
//       }
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.button_topic);
//   }
//   else if (CMND_SWITCHTOPIC == command_code) {
//     if ((data_len > 0) && (data_len < sizeof(Settings.switch_topic))) {
//       MakeValidMqtt(0, dataBuf);
//       if (!strcmp(dataBuf, mqtt_client)) SetShortcut(dataBuf, SC_DEFAULT);
//       switch (Shortcut(dataBuf)) {
//         case SC_CLEAR: strlcpy(Settings.switch_topic, "", sizeof(Settings.switch_topic)); break;
//         case SC_DEFAULT: strlcpy(Settings.switch_topic, mqtt_topic, sizeof(Settings.switch_topic)); break;
//         case SC_USER: strlcpy(Settings.switch_topic, MQTT_SWITCH_TOPIC, sizeof(Settings.switch_topic)); break;
//         default: strlcpy(Settings.switch_topic, dataBuf, sizeof(Settings.switch_topic));
//       }
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, Settings.switch_topic);
//   }
//   else if (CMND_BUTTONRETAIN == command_code) {
//     if ((payload >= 0) && (payload <= 1)) {
//       if (!payload) {
//         for(i = 1; i <= MAX_KEYS; i++) {
//           //SendKey(0, i, 9);  // Clear MQTT retain in broker
//         }
//       }
//       Settings.flag_system.mqtt_button_retain = payload;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag_system.mqtt_button_retain));
//   }
//   else if (CMND_SWITCHRETAIN == command_code) {
//     if ((payload >= 0) && (payload <= 1)) {
//       if (!payload) {
//         for(i = 1; i <= MAX_SWITCHES; i++) {
//           //SendKey(1, i, 9);  // Clear MQTT retain in broker
//         }
//       }
//       Settings.flag_system.mqtt_switch_retain = payload;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag_system.mqtt_switch_retain));
//   }
//   else if (CMND_POWERRETAIN == command_code) {
//     if ((payload >= 0) && (payload <= 1)) {
//       if (!payload) {
//         for(i = 1; i <= devices_present; i++) {  // Clear MQTT retain in broker
//           GetTopic_P(stemp1, STAT, mqtt_topic, GetPowerDevice(scommand, i, sizeof(scommand), Settings.flag_system.device_index_enable));
//           mcl->mset->data_buffer.payload.ctr[0] = '\0';
//           MqttPublish(stemp1, Settings.flag_system.mqtt_power_retain);
//         }
//       }
//       Settings.flag_system.mqtt_power_retain = payload;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag_system.mqtt_power_retain));
//   }
//   else if (CMND_SENSORRETAIN == command_code) {
//     if ((payload >= 0) && (payload <= 1)) {
//       if (!payload) {
//         mcl->mset->data_buffer.payload.ctr[0] = '\0';
//         MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_SENSOR), Settings.flag_system.mqtt_sensor_retain);
//         MqttPublishPrefixTopic_P(TELE, PSTR(D_RSLT_ENERGY), Settings.flag_system.mqtt_sensor_retain);
//       }
//       Settings.flag_system.mqtt_sensor_retain = payload;
//     }
//     Response_P(S_JSON_COMMAND_SVALUE, command, GetStateText(Settings.flag_system.mqtt_sensor_retain));
//   }
//   else serviced = false;  // Unknown command

//   return serviced;
// }

// /*********************************************************************************************\
//  * Presentation
// \*********************************************************************************************/

// #ifdef USE_WEBSERVER


// /*********************************************************************************************\
//  * Interface
// \*********************************************************************************************/

// bool Xdrv02(uint8_t function)
// {
//   //DEBUGLLN("Xdrv02 - START");
//   bool result = false;
//
// //   if (Settings.flag_system.mqtt_enabled) {
// //     switch (function) {
// //       case FUNC_EVERY_50_MSECOND:  // https://github.com/knolleary/pubsubclient/issues/556
// //       //  M qttClient.loop();
// //         break;
// //       case FUNC_COMMAND:
// //         result = MqttCommand();
// //         break;
// //     }
// //   }
// //   DEBUGLLN("Xdrv02 - END");
//   return result;
// }
// #endif