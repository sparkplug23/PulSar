#include "2_CoreSystem/08_Logging/mLogging.h"



void mLogging::Syslog(void)
{
  // // Destroys log_data
  // char syslog_preamble[64];  // Hostname + Id

  // uint32_t current_hash = GetHash(Settings.syslog_host, strlen(Settings.syslog_host));
  // if (syslog_host_hash != current_hash) {
  //   syslog_host_hash = current_hash;
  //   WiFi.hostByName(Settings.syslog_host, syslog_host_addr);  // If sleep enabled this might result in exception so try to do it once using hash
  // }
  // if (PortUdp.beginPacket(syslog_host_addr, Settings.syslog_port)) {
  //   snprintf_P(syslog_preamble, sizeof(syslog_preamble), PSTR("%s ESP-"), my_hostname);
  //   memmove(log_data + strlen(syslog_preamble), log_data, sizeof(log_data) - strlen(syslog_preamble));
  //   log_data[sizeof(log_data) -1] = '\0';
  //   memcpy(log_data, syslog_preamble, strlen(syslog_preamble));
  //   PortUdp.write(log_data);
  //   PortUdp.endPacket();
  //   delay(1);  // Add time for UDP handling (#5512)
  // } else {
  //   syslog_level = 0;
  //   syslog_timer = SYSLOG_TIMER;
  //   AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_HOST_NOT_FOUND ". " D_RETRY_IN " %d " D_UNIT_SECOND), SYSLOG_TIMER);
  // }
}

/*********************************************************************************************\
 * Response data handling -- "AddLog" that are pushed to mqtt channel "status/response"
\*********************************************************************************************/

// return if response was sent, else use leds?
int Response_mP(const char* format, ...)     // Content send snprintf_P char data
{
  
  memset(&tkr_set->response_msg,0,sizeof(tkr_set->response_msg));

  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int len = vsnprintf_P(tkr_set->response_msg, RESPONSE_MESSAGE_BUFFER_SIZE, format, args);
  va_end(args);

//   //Share on serial/telnet
  ALOG_DBG(PSTR(D_LOG_RESPONSE "%s"), tkr_set->response_msg);
//   //Send via mqtt
//   #ifdef USE_MODULE_NETWORK_MQTT
//   tkr_mqtt->publish_device("status/result",tkr_set->response_msg,false);
//   #endif
  
  return 0;// len;
}

int ResponseAppend_mP(const char* format, ...)  // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  // va_list args;
  // va_start(args, format);
  // int mlen = strlen(tkr_set->response_msg);
  // int len = vsnprintf_P(tkr_set->response_msg + mlen, sizeof(tkr_set->response_msg) - mlen, format, args);
  // va_end(args);
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_RESPONSE "Response_P %s"),tkr_set->response_msg);
  return 0;// len + mlen;
}
