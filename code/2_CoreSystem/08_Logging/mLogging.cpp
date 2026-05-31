#include "2_CoreSystem/08_Logging/mLogging.h"


void AddLogOutput(uint8_t loglevel, const char* log_data)
{
  if (log_data == nullptr) {
    return;
  }

  char mxtime[25];  // "13:45:21 "
  memset(mxtime, 0, sizeof(mxtime));

  if (tkr_set->Settings.logging.time_isshort) {
    tkr_time->GetUptime(mxtime, sizeof(mxtime));
  } else {
    char up[16];
    tkr_time->GetUptime(up, sizeof(up));
    snprintf_P(
      mxtime,
      sizeof(mxtime),
      PSTR("%02u:%02u:%02u %s"),
      (unsigned)tkr_time->RtcTime.hour,
      (unsigned)tkr_time->RtcTime.minute,
      (unsigned)tkr_time->RtcTime.second,
      up
    );
  }

  bool isconnected = false;
  #ifdef USE_MODULE_NETWORK_WIFI
  isconnected = tkr_wifi->connection.fConnected;
  #endif

  #ifndef DISABLE_SERIAL_LOGGING
  // LOG : SERIAL
  if (loglevel <= tkr_set->Settings.logging.serial_level)
  {
    if ((loglevel == LOG_LEVEL_HIGHLIGHT) || (loglevel == LOG_LEVEL_HIGHLIGHT_TOP)) {
      SERIAL_DEBUG.printf("HIGHLIGHT START >>>>>>>>>>>>>>>>>\n\r");
    }

    #ifdef ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS
    if (loglevel == LOG_LEVEL_ERROR) {
      SERIAL_DEBUG.printf("\n\rERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r");
    }
    #endif

    #ifdef ENABLE_FREERAM_APPENDING_SERIAL
      char buffer[20];
      SERIAL_DEBUG.printf(
        EPSTR("R%05u%c %s %s %s\r\n"),
        (unsigned)ESP.getFreeHeap(),
        isconnected ? 'Y' : 'N',
        tkr_time->GetUptime(buffer, sizeof(buffer)),
        tkr_log->GetLogLevelNamebyID(loglevel),
        log_data
      );
    #else
      SERIAL_DEBUG.printf(
        PSTR("%s %S %s\r\n"),
        mxtime,
        tkr_log->GetLogLevelNamebyID(loglevel),
        log_data
      );
    #endif

    if ((loglevel == LOG_LEVEL_HIGHLIGHT) || (loglevel == LOG_LEVEL_HIGHLIGHT_BOT)) {
      SERIAL_DEBUG.printf("HIGHLIGHT END   <<<<<<<<<<<<<<<<<\n\r");
    }

    #ifdef ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS
    if (loglevel == LOG_LEVEL_ERROR) {
      SERIAL_DEBUG.printf("\n\rERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r");
    }
    #endif

    #ifdef ENABLE_DEBUGFEATURE_LOGS__FORCE_FLUSH_ON_TRANSMIT
    SERIAL_DEBUG.flush(); // To ensure all serial is sent before a reset
    #endif

    if ((tkr_set->Settings.logging.serial_level == LOG_LEVEL_DEBUG) ||
        (tkr_set->Settings.logging.serial_level == LOG_LEVEL_DEBUG_LOWLEVEL) ||
        (tkr_set->Settings.logging.serial_level == LOG_LEVEL_ALL)
        #ifdef ENABLE_SERIAL_FLUSH
        ||(true)
        #endif
    ) {
      SERIAL_DEBUG.flush();
    }
    
  }
  #endif // DISABLE_SERIAL_LOGGING


  // LOG : TELNET
  if (loglevel <= tkr_set->Settings.logging.telnet_level)
  {
    if (tkr_log->telnet.client) {
      tkr_log->telnet.client.printf(
        "%s%S %s\r\n",
        mxtime,
        tkr_log->GetLogLevelNamebyID(loglevel),
        log_data
      );
    }
  }

  // LOG : WEBSERVER
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  if (loglevel <= tkr_set->Settings.logging.web_level) {
    if (tkr_set->Settings.webserver && tkr_web->fConsole_active) { // only append values when active, however, this stops history
      // Delimited, zero-terminated buffer of log lines.
      // Each entry has this format: [index][log data]['\1']
      if (!tkr_log->weblog.index) {
        tkr_log->weblog.index++;   // Index 0 is not allowed as it is the end of char string
      }

      while (
        (tkr_log->weblog.index == tkr_log->weblog.buffer[0]) ||
        (strlen(tkr_log->weblog.buffer) + strlen(log_data) + 13 > WEB_LOG_SIZE)
      ) {
        char* it = tkr_log->weblog.buffer;
        it++;                                  // Skip weblog.index
        it += tkr_sup->strchrspn(it, '\1');    // Skip log line
        it++;                                  // Skip delimiting "\1"
        memmove(tkr_log->weblog.buffer, it, WEB_LOG_SIZE - (it - tkr_log->weblog.buffer));  // Move buffer forward to remove oldest log line
      }

      // creates line formatted with \1 meaning EOL
      snprintf_P(
        tkr_log->weblog.buffer,
        sizeof(tkr_log->weblog.buffer),
        PSTR("%s%c%s %S %s\1"),
        tkr_log->weblog.buffer,
        tkr_log->weblog.index++,
        mxtime,
        tkr_log->GetLogLevelNamebyID(loglevel),
        log_data
      );

      if (!tkr_log->weblog.index) {
        tkr_log->weblog.index++;   // Index 0 is not allowed as it is the end of char string
      }
    }
  }
  #endif  // USE_MODULE_NETWORK_WEBSERVER

  #ifdef USE_MODULE_DISPLAYS_NEXTION
    #ifdef ENABLE_FEATURE_NEXTION__ADDLOG_ON_LOG_PAGE
    if (loglevel <= tkr_set->Settings.logging.serial_level)
    {
      if (tkr_nex->settings.transmit_addlog_message) {
        tkr_nex->display->printf(
          PSTR("%S%s %s\r\n"),
          tkr_log->GetLogLevelNamebyID(loglevel),
          log_data,
          mxtime
        );
      }
    }
    #endif
  #endif

  // LOG : MQTT Broadcast Alerts
  /**
   * @brief New method that assuming enabled, will broadcast the error message so a central location can view any ongoing system messages
   * It will keep the device_name prefix, so the device can be identified.
   * It should get elevated previledges outside of device_name/status/ so it can be viewed as its own issue. Perhaps just logging.
   * device_name/logging/message/LOG_LEVEL  so LOG_LEVEL can be identified
   * device_name/logging/alert/LOG_LEVEL    alerts are reserved for special messages, and depending on the openhab may be broadcast as notifications on my phone
   **/
  #ifdef ENABLE_LOGGING_ADDLOG__MESSAGES_OVER_MQTT
  if (loglevel <= tkr_set->Settings.logging.mqtt_level)
  {
    char topic[100] = {0};
    snprintf_P(topic, sizeof(topic), "logging/message/%S", tkr_log->GetLogLevelNamebyID(loglevel));
    if (tkr_time->uptime_seconds_nonreset > 60) {
      tkr_mqtt->Publish(topic, log_data);
    }
    DEBUG_LINE_HERE3
  }
  #endif // ENABLE_LOGGING_ADDLOG__MESSAGES_OVER_MQTT

}

uint8_t HighestLogLevel()
{
  uint8_t highest_loglevel = tkr_set->Settings.logging.serial_level;

  if (tkr_set->Settings.logging.web_level > highest_loglevel) { highest_loglevel = tkr_set->Settings.logging.web_level; }
  if (tkr_set->Settings.logging.telnet_level > highest_loglevel) { highest_loglevel = tkr_set->Settings.logging.telnet_level; }
  if (tkr_set->Settings.logging.mqtt_level > highest_loglevel) { highest_loglevel = tkr_set->Settings.logging.mqtt_level; }
  if (tkr_set->Settings.logging.sys_level > highest_loglevel) { highest_loglevel = tkr_set->Settings.logging.sys_level; }
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  if (tkr_set->Settings.logging.display_serial_level > highest_loglevel) { highest_loglevel = tkr_set->Settings.logging.display_serial_level; }
  #endif
  #ifdef ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING
  if (tkr_time->uptime_seconds_nonreset < 3) { highest_loglevel = LOG_LEVEL_DEBUG_MORE; }
  #endif

  return highest_loglevel;
}

static char* AddLog_MallocV(PGM_P formatP, va_list arg)
{
  va_list arg_copy;
  va_copy(arg_copy, arg);
  int len = vsnprintf_P(nullptr, 0, formatP, arg_copy);
  va_end(arg_copy);

  if (len < 0) {
    return nullptr;
  }

  char* log_data = (char*)malloc((size_t)len + 1);
  if (log_data == nullptr) {
    return nullptr;
  }

  va_list arg_copy2;
  va_copy(arg_copy2, arg);
  vsnprintf_P(log_data, (size_t)len + 1, formatP, arg_copy2);
  va_end(arg_copy2);

  return log_data;
}

static void AddLogV(uint8_t loglevel, PGM_P formatP, va_list arg)
{
  #ifdef ESP32
  if (xPortInIsrContext()) {
    return;
  }
  #endif

  // Check if at least one log meets threshold
  if (loglevel > HighestLogLevel()) {
    return;
  }

  // Create log buffer
  char* log_data = AddLog_MallocV(formatP, arg);
  if (log_data == nullptr) {
    return;
  }

  // Send to output function for dispatch on any log methods
  AddLogOutput(loglevel, log_data);

  // Clear the log buffer
  free(log_data);

}

/**
 * Normal log starting point
 */
void AddLog(uint8_t loglevel, PGM_P formatP, ...)
{
  va_list arg;
  va_start(arg, formatP);
  AddLogV(loglevel, formatP, arg);
  va_end(arg);
}

/**
 * Optional time limited logging: eg always report errors but restrict flooding the log window
 */
void AddLog_TimeGated(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, PGM_P formatP, ...)
{
  if (tSaved == nullptr) {
    return;
  }

  uint32_t now = millis();
  if ((uint32_t)(now - *tSaved) < limit_ms) {
    return;
  }

  *tSaved = now;

  va_list arg;
  va_start(arg, formatP);
  AddLogV(loglevel, formatP, arg);
  va_end(arg);
}


void mLogging::GetLog(uint8_t idx, char** entry_pp, size_t* len_p)
{
  char* entry_p = nullptr;
  size_t len = 0;

  if (idx) {
    char* it = weblog.buffer;
    // get line
    do {
      uint8_t cur_idx = *it;
      it++;
      size_t tmp = tkr_sup->strchrspn(it, '\1');
      tmp++;                             // Skip terminating '\1'
      if (cur_idx == idx) {              // Found the requested entry
        len = tmp;
        entry_p = it;
        break;
      }
      it += tmp;
    } while (it < weblog.buffer + WEB_LOG_SIZE && *it != '\0');
  }
  *entry_pp = entry_p;
  *len_p = len;
}


void mLogging::StartTelnetServer()
{  
  // if(tkr_set->global_state.network_down) return;
  telnet.server = new WiFiServer(TELNET_PORT);  // set port here
  telnet.server->begin();
  telnet.running = true;
  // if(seriallog)
  // ALOG_INF(PSTR("telnet.client server started on port %d"),(uint8_t)TELNET_PORT);
}


void mLogging::handleTelnet(){
  if(!telnet.server) return; //not configured yet
  if (telnet.server->hasClient()){
  	// client is connected
    if (!telnet.client || !telnet.client.connected()){
      if(telnet.client) telnet.client.stop();          // client disconnected
      telnet.client = telnet.server->available(); // ready for new client
    } else {
      telnet.server->available().stop();  // have client, block new conections
    }
  }

  if (telnet.client && telnet.client.connected() && telnet.client.available()){
    // client input processing
    while(telnet.client.available())
      char c = telnet.client.read();//dump values
      // Serial.write(telnet.client.read()); // pass through
      // do other stuff with client input here
  } 
}

const char* mLogging::GetLogLevelNamebyID(uint8_t id) {
  switch(id) {
    default:
    case LOG_LEVEL_NONE:           return PM_LOG_LEVEL_NONE_CTR;
    case LOG_LEVEL_DEBUG_TRACE:    return PM_LOG_LEVEL_DEBUG_TRACE_CTR;
    case LOG_LEVEL_ERROR:          return PM_LOG_LEVEL_ERROR_CTR;
    case LOG_LEVEL_WARNING:        return PM_LOG_LEVEL_WARN_CTR;
    case LOG_LEVEL_DEV_TEST:           return PM_LOG_LEVEL_TEST_CTR;
    case LOG_LEVEL_HIGHLIGHT:      return PM_LOG_LEVEL_HIGHLIGHT_CTR;
    case LOG_LEVEL_IMPORTANT:      return PM_LOG_LEVEL_IMPORTANT_CTR;
    case LOG_LEVEL_INFO:           return PM_LOG_LEVEL_INFO_CTR;
    case LOG_LEVEL_COMMANDS:       return PM_LOG_LEVEL_COMMANDS_CTR;
    case LOG_LEVEL_DEBUG:          return PM_LOG_LEVEL_DEBUG_CTR;
    case LOG_LEVEL_DEBUG_MORE:     return PM_LOG_LEVEL_DEBUG_MORE_CTR;
    case LOG_LEVEL_DEBUG_LOWLEVEL: return PM_LOG_LEVEL_DEBUG_LOWLEVEL_CTR;
    case LOG_LEVEL_ALL:            return PM_LOG_LEVEL_ALL_CTR;
  }
}

uint8_t mLogging::GetLogLevelIDbyName(const char* name) {
  if (name == nullptr) return LOG_LEVEL_NONE;  // Handle null input

  if (strcmp_P(name, PM_LOG_LEVEL_NONE_CTR) == 0)            return LOG_LEVEL_NONE;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_TRACE_CTR) == 0)     return LOG_LEVEL_DEBUG_TRACE;
  if (strcmp_P(name, PM_LOG_LEVEL_ERROR_CTR) == 0)           return LOG_LEVEL_ERROR;
  if (strcmp_P(name, PM_LOG_LEVEL_WARN_CTR) == 0)            return LOG_LEVEL_WARNING;
  if (strcmp_P(name, PM_LOG_LEVEL_TEST_CTR) == 0)            return LOG_LEVEL_DEV_TEST;
  if (strcmp_P(name, PM_LOG_LEVEL_HIGHLIGHT_CTR) == 0)       return LOG_LEVEL_HIGHLIGHT;
  if (strcmp_P(name, PM_LOG_LEVEL_IMPORTANT_CTR) == 0)       return LOG_LEVEL_IMPORTANT;
  if (strcmp_P(name, PM_LOG_LEVEL_INFO_CTR) == 0)            return LOG_LEVEL_INFO;
  if (strcmp_P(name, PM_LOG_LEVEL_COMMANDS_CTR) == 0)        return LOG_LEVEL_COMMANDS;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_CTR) == 0)           return LOG_LEVEL_DEBUG;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_MORE_CTR) == 0)      return LOG_LEVEL_DEBUG_MORE;
  if (strcmp_P(name, PM_LOG_LEVEL_DEBUG_LOWLEVEL_CTR) == 0)  return LOG_LEVEL_DEBUG_LOWLEVEL;
  if (strcmp_P(name, PM_LOG_LEVEL_ALL_CTR) == 0)             return LOG_LEVEL_ALL;

  return LOG_LEVEL_NONE;  // Default return for no match
}



int8_t mLogging::Tasker(uint8_t function, JsonParserObject obj)
{ // KEEP TASKER ON TOP

  switch(function){
    case TASK_LOOP: 
      if(telnet.running){      handleTelnet();    }
    break;
    case TASK_EVERY_SECOND:
    
      #ifdef ENABLE_FEATURE_LOGGING__NORMAL_OPERATION_REDUCE_LOGGING_LEVEL_WHEN_NOT_DEBUGGING
      if(tkr_time->uptime_seconds_nonreset == 60*1)
      {
        tkr_set->Settings.logging.serial_level = LOG_LEVEL_INFO;
        Serial.printf("Reducing log level to %d to improve performance when not debugging", tkr_set->Settings.logging.serial_level);
        // ALOG_INF(PSTR(D_LOG_APPLICATION "Reducing log level to %d to improve performance when not debugging"), tkr_set->Settings.logging.serial_level);
      }
      #endif

    break;
    case TASK_EVERY_MINUTE:



  /**** For increasing log level temporarily then reseting
   * 
   * */
  // if (tkr_set->seriallog_timer) {
  //   seriallog_timer--;
  //   if (!seriallog_timer) {
  //     if (seriallog_level) {
  //       ALOG_INF(PSTR(D_LOG_APPLICATION D_SERIAL_LOGGING_DISABLED));
  //     }
  //     seriallog_level = 0;
  //   }
  // }

  // if (syslog_timer) {  // Restore syslog level
  //   syslog_timer--;
  //   if (!syslog_timer) {
  //     syslog_level = Settings.syslog_level;
  //     if (Settings.syslog_level) {
  //       ALOG_INF(PSTR(D_LOG_APPLICATION D_SYSLOG_LOGGING_REENABLED));  // Might trigger disable again (on purpose)
  //     }
  //   }
  // }

    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;


    case TASK_NETWORK_CONNECTED__WIFI:
      StartTelnetServer();
    break;
  }

  return TASKER_RESULT__UNKNOWN_ID;

}


void mLogging::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken tok;

  JsonParserObject jobj = obj[GetModuleName()];

  if(!jobj) return; // Only allow commands for this module

  if(tok = jobj["TelnetLevel"])
  {
    // AddLog(LOG_LEVEL_INFO,PSTR("TelnetLevel %d->%d"),tkr_set->Settings.logging.telnet_level,tok.getInt());
    tkr_set->Settings.logging.telnet_level = tok.getInt();
  }
  

}
