#include "2_CoreSystem/05_Logging/mLogging.h"


const char* mLogging::PM_MODULE_CORE_LOGGING_CTR = D_MODULE_CORE_LOGGING_CTR;
const char* mLogging::PM_MODULE_CORE_LOGGING_FRIENDLY_CTR = D_MODULE_CORE_LOGGING_FRIENDLY_CTR;



int8_t mLogging::Tasker(uint8_t function, JsonParserObject obj)
{ // KEEP TASKER ON TOP


  switch(function){
    case FUNC_INIT:
      // init();
      // StartTelnetServer();
    break;
    case FUNC_LOOP: 
    // Serial.println("mLogging::Tasker");
      // if(!Telnet) StartTelnetServer();
      if(telnet_started){      handleTelnet();    }
      // pCONT_log->Telnet.println("FUNC_LOOP: ");
    break;
    case FUNC_EVERY_SECOND:
      // Serial.println("mLogging::Tasker");
    break;
    case FUNC_EVERY_MINUTE:


  /**** For increasing log level temporarily then reseting
   * 
   * */
  // if (pCONT_set->seriallog_timer) {
  //   seriallog_timer--;
  //   if (!seriallog_timer) {
  //     if (seriallog_level) {
  //       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SERIAL_LOGGING_DISABLED));
  //     }
  //     seriallog_level = 0;
  //   }
  // }

  // if (syslog_timer) {  // Restore syslog level
  //   syslog_timer--;
  //   if (!syslog_timer) {
  //     syslog_level = Settings.syslog_level;
  //     if (Settings.syslog_level) {
  //       AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_LOGGING_REENABLED));  // Might trigger disable again (on purpose)
  //     }
  //   }
  // }

    break;
    case FUNC_WIFI_CONNECTED:
      StartTelnetServer();
    break;
  }

}//end


void mLogging::init(void){
  // StartTelnetServer();
}
 
void mLogging::parse_JSONCommand(JsonParserObject obj){};

// for quick prototyping, set to test level

void AddLog(uint8_t loglevel, uint32_t* tSaved, uint16_t limit_ms, PGM_P formatP, ...){
  if(ABS_FUNCTION(millis()-*tSaved)>=limit_ms){ *tSaved=millis();
    va_list arg;
    //AddLog(loglevel,formatP,arg);
  }
}



// template<typename T, typename U>
// void mLogging::AddLog_Array4(uint8_t loglevel, const char* name_ctr, T* arr, U arr_len)
// {


//   char buffer[100] = {0}; // short buffer
//   uint16_t buflen = 0;

//   buflen += snprintf(buffer+buflen, sizeof(buffer), "AddLog_Array4 %s = ", name_ctr);

//   AddLog(loglevel, buffer);



// }




void ErrorMessage_P(uint8_t error_type, const char* message)
{


  AddLog(LOG_LEVEL_ERROR, PSTR("Invalid Format: %S"), message);


}

void ErrorMessage(uint8_t error_type, const char* message)
{

  AddLog(LOG_LEVEL_ERROR, PSTR("Invalid Format: %s"), message);



}



void AddLog(uint8_t loglevel, PGM_P formatP, ...)
{
  //Block software stream for now
  // if(fDebugOutputMode==DEBUG_OUTPUT_SOFTWARE_ID){return;}
  // if(!fDebugSerialMode){return;}

  // if(pCONT_log->Telnet!=nullptr){
    //   if(!pCONT_log->client.connected()) {
      
    //     pCONT_log->client = pCONT_log->server->available();
    //   }
//     if(pCONT_time->uptime.seconds_nonreset>30){
  // pCONT_log->Telnet.println("AddLog: ");
//     //   if(pCONT_log->client.connected()) {
//     //     pCONT_log->client.printf("%s%s %s\r\n", mxtime, pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer), log_data);
//     //   }
//     // }
//     // }else{
// }
    //   Serial.println("Telnet else");
    // }

    /**
     * @brief 
     * Add special debug method here that will count loglevel by type so "error" messages can be counted and shared every X seconds on terminal
     * 
     */


  #ifdef DEBUG_FOR_FAULT
    pCONT_set->Settings.logging.serial_level = LOG_LEVEL_ALL;
  #endif
  // SERIAL_DEBUG.printf("%s %d\r\n","START",millis()); 
  // pCONT_set->Settings.logging.serial_level = LOG_LEVEL_ALL;

  // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
  if(
    (loglevel>pCONT_set->Settings.logging.serial_level)&&
    (loglevel>pCONT_set->Settings.logging.web_level)
    ){
    return;
  }
  
  // Filtering
  if(pCONT_set->runtime.enable_serial_logging_filtering){ // if true, only permit exact log level and not all above
    if(loglevel == pCONT_set->Settings.logging.serial_level){
      //permit messages
    }else{
      return;
    }
  }
  if(pCONT_set->runtime.enable_web_logging_filtering){ // if true, only permit exact log level and not all above
    if(loglevel == pCONT_set->Settings.logging.web_level){
      //permit messages
    }else{
      return;
    }
  }

  // SERIAL_DEBUG.printf("%s %d\r\n","before vsn",millis());

  // DEBUG_LINE_HERE;

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(pCONT_log->log_data, sizeof(pCONT_log->log_data), formatP, arg);
  va_end(arg);

  // DEBUG_LINE_HERE;

  //AddLogAddLog(loglevel);

  char mxtime[25];  // "13:45:21 " //9

  /**
   * Change to different log layouts, using a switch case. Each method (serial/web etc) can use different formats
   * */

  memset(mxtime,0,sizeof(mxtime));
  // if time is short, ie debugging, them only show uptime (not RTCTime)
  if(pCONT_set->Settings.logging.time_isshort){
    if(pCONT_time->uptime.hour<1){
      snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d "),
        pCONT_time->uptime.minute,pCONT_time->uptime.second
      );
    }else{
      snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d:%02d "), //add hour
        pCONT_time->uptime.hour,pCONT_time->uptime.minute,pCONT_time->uptime.second);
    }
    
  }else{
    snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d %02dT%02d:%02d:%02d "),
    // sprintf(mxtime, PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d %02dT%02d:%02d:%02d "),
      pCONT_time->RtcTime.hour,pCONT_time->RtcTime.minute,pCONT_time->RtcTime.second,
      pCONT_time->uptime.Mday,pCONT_time->uptime.hour,pCONT_time->uptime.minute,pCONT_time->uptime.second);
  }

  // SERIAL_DEBUG.printf("%s %d\r\n","serail",millis());
  char level_buffer[10];

  bool isconnected = false;
  #ifdef USE_MODULE_NETWORK_WIFI
  isconnected = pCONT_wif->connection.fConnected;
  #endif

  #ifndef DISABLE_SERIAL_LOGGING
  // LOG : SERIAL
  if (loglevel <= pCONT_set->Settings.logging.serial_level) {
    #ifdef ENABLE_FREERAM_APPENDING_SERIAL
      // register uint32_t *sp asm("a1"); 
      // SERIAL_DEBUG.printf("R%05d S%04d U%02d%02d %s %s\r\n", 
      SERIAL_DEBUG.printf(PSTR("R%05d%c %02d%02d %s %s\r\n"), 
        ESP.getFreeHeap(), // 4 * (sp - g_pcont->stack), 
        isconnected ? 'Y' : 'N',
        pCONT_time->uptime.minute, pCONT_time->uptime.second,
        pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer, sizeof(level_buffer)),
        log_data
      );
    #else

      if(loglevel == LOG_LEVEL_HIGHLIGHT){ SERIAL_DEBUG.printf("\n\r\n\r>>HIGHLIGHT START<<\n\r\n\r"); }
      #ifdef ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS
      if(loglevel == LOG_LEVEL_ERROR){ SERIAL_DEBUG.printf("\n\rERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r"); }
      #endif // ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS


      SERIAL_DEBUG.printf(PSTR("%s%s %s\r\n"), mxtime, pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer, sizeof(level_buffer)), pCONT_log->log_data);

      if(loglevel == LOG_LEVEL_HIGHLIGHT){ SERIAL_DEBUG.printf("\n\r\n\r>>HIGHLIGHT END<<\n\r\n\r"); }
      #ifdef ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS
      if(loglevel == LOG_LEVEL_ERROR){ SERIAL_DEBUG.printf("\n\rERROR!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\r"); }
      #endif // ENABLE_DEVFEATURE_LOGLEVEL_ERROR_TERMINAL_EMPHASIS

    #endif
    //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
    // Only flush if we all doing all for debugging
    
    //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
	// IMPORTANT!!! The code will pause here if flush is set, only for ms until the serial print has been sent
	// Normally, serial is passed to hardware internal the the chip, and serial is printed in the background. However, if a problem/bug with forced reseting exists,
	// you want to print all serial BEFORE tripping the reset, so only enable when fault tracing
	// #ifdef ENABLE_SERIAL_DEBUG_FLUSH

    if(pCONT_set->Settings.logging.serial_level == LOG_LEVEL_ALL){ SERIAL_DEBUG.flush(); } 
  } 
  #endif //DISABLE_SERIAL_LOGGING

  #ifdef ENABLE_SERIAL_FLUSH
    SERIAL_DEBUG.flush(); 
  #endif 

  // SERIAL_DEBUG.printf("%s %d\r\n","telnet",millis());
  // LOG : TELNET
  // #ifdef ENABLE_TELNET_LOGGING // off by default for performance
  // if (loglevel <= pCONT_set->Settings.telnetlog_level) {
    // if(pCONT_log->server!=nullptr){ // needs ignored before it has been started!
    
  if(pCONT_log->Telnet){
    //   if(!pCONT_log->client.connected()) {
      
    //     pCONT_log->client = pCONT_log->server->available();
    //   }
    
  pCONT_log->Telnet.printf(
    // "uptime2: ");
    //   if(pCONT_log->client.connected()) {
    //     pCONT_log->client.printf(
      "%s%s %s\r\n", mxtime, pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer, sizeof(level_buffer)), pCONT_log->log_data);
    //   }
    // }
    }


  // }
  // #endif

  // SERIAL_DEBUG.printf("%s %d\r\n","webserver",millis());
  // LOG : WEBSERVER
  // #ifdef USE_MODULE_NETWORK_WEBSERVER23
  // if(pCONT_web->fConsole_active && !pCONT_web->fConsole_history){ //only append values when active, however, this stops history
  //   if (pCONT_set->Settings.webserver && (loglevel <= pCONT_set->Settings.logging.web_level)) {
  //     // Delimited, zero-terminated buffer of log lines.
  //     // Each entry has this format: [index][log data]['\1']
  //     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
      
  //     while (web_log_index == web_log[0] ||  // If log already holds the next index, remove it
  //           strlen(web_log) + strlen(log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
  //     {
  //       char* it = web_log;
  //       it++;                                  // Skip web_log_index
  //       it += pCONT_sup->strchrspn(it, '\1'); // Skip log line
  //       it++;                                  // Skip delimiting "\1"
  //       // circle uffer
  //       memmove(web_log, it, WEB_LOG_SIZE -(it-web_log));  // Move buffer forward to remove oldest log line
  //     }
  //     // creates line formatted with \1 meaning EOL
  //     snprintf_P(web_log, sizeof(web_log), PSTR("%s%c%s%s %s\1"), web_log, web_log_index++, mxtime, pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer), log_data);
  //     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
    
  //   }
  // }
  // #endif  // USE_MODULE_NETWORK_WEBSERVER23
  
  // SERIAL_DEBUG.printf("%s %d\r\n","end",millis());


}



void mLogging::handleTelnet(){
  if(!TelnetServer) return; //not configured yet
  if (TelnetServer->hasClient()){
  	// client is connected
    if (!Telnet || !Telnet.connected()){
      if(Telnet) Telnet.stop();          // client disconnected
      Telnet = TelnetServer->available(); // ready for new client
    } else {
      TelnetServer->available().stop();  // have client, block new conections
    }
  }

  if (Telnet && Telnet.connected() && Telnet.available()){
    // client input processing
    while(Telnet.available())
      char c = Telnet.read();//dump values
      // Serial.write(Telnet.read()); // pass through
      // do other stuff with client input here
  } 
}


// // For sending without network during uploads
// void AddSerialLog_mP2(uint8_t loglevel, PGM_P formatP, ...)
// {

//   // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
//   if(
//     (loglevel>pCONT_set->Settings.logging.serial_level)
//     ){
//     return;
//   }
  
//   // Filtering
//   if(pCONT_set->enable_serial_logging_filtering){ // if true, only permit exact log level and not all above
//     if(loglevel == pCONT_set->Settings.logging.serial_level){
//       //permit messages
//     }else{
//       return;
//     }
//   }

//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
//   va_end(arg);

//   //AddLogAddLog(loglevel);

//   char mxtime[25];  // "13:45:21 " //9
//   char level_buffer[10];

//   uint8_t hour = 0;
//   uint8_t minute = 0;
//   uint8_t second = 0;
//   uint8_t uday = 0;
//   uint8_t uhour = 0;
//   uint8_t uminute = 0;
//   uint8_t usecond = 0;
//   if(pCONT_time!=NULL){ 
//     hour = pCONT_time->RtcTime.hour; 
//     minute = pCONT_time->RtcTime.minute; 
//     second = pCONT_time->RtcTime.second; 
//     uday = pCONT_time->uptime.Mday; 
//     uhour = pCONT_time->uptime.hour;   
//     uminute = pCONT_time->uptime.minute; 
//     usecond = pCONT_time->uptime.second; 
//   }

//   memset(mxtime,0,sizeof(mxtime));
//   if(pCONT_set->Settings.log_time_isshort){
    
//     if(uhour<1){
//       snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d:%02d %02d:%02d "),minute,second,uminute,usecond);
//     }

//   }else{
//     // Show all information
//     snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d %02dT%02d:%02d:%02d "),hour,minute,second,uday,uhour,uminute,usecond);
//   }


//   // Overrides
//   //uint8_t seriallog_level = LOG_LEVEL_DEBUG_MORE;
//   //pCONT_log->seriallog_level = LOG_LEVEL_DEBUG_MORE;
//   //pCONT_set->Settings.logging.serial_level = LOG_LEVEL_DEBUG;
//   //pCONT_set->Settings.logging.web_level = LOG_LEVEL_INFO;

//   // LOG : SERIAL
//   if (loglevel <= pCONT_set->Settings.logging.serial_level) {
//     SERIAL_DEBUG.printf("%s%s %s\r\n", mxtime,pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer),  log_data);
//     //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
//     // SERIAL_DEBUG.flush();
//   }

// }



void AddLog_NoTime(uint8_t loglevel, PGM_P formatP, ...)
{

  // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
  if(
    (loglevel>pCONT_set->Settings.logging.serial_level)&&
    (loglevel>pCONT_set->Settings.logging.web_level)
    ){
    return;
  }

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(pCONT_log->log_data, sizeof(pCONT_log->log_data), formatP, arg);
  va_end(arg);

  //AddLogAddLog(loglevel);

  char mxtime[25];  // "13:45:21 " //9
  memset(mxtime,0,sizeof(mxtime));

  char level_buffer[10];

  // uint8_t hour = 0;
  // uint8_t minute = 0;
  // uint8_t second = 0;
  // uint8_t uday = 0;
  // uint8_t uhour = 0;
  // uint8_t uminute = 0;
  // uint8_t usecond = 0;
  // if(pCONT_time!=NULL){ 
  //   hour = pCONT_time->RtcTime.hour; 
  //   minute = pCONT_time->RtcTime.minute; 
  //   second = pCONT_time->RtcTime.second; 
  //   uday = pCONT_time->uptime.Mday; 
  //   usecond = pCONT_time->uptime.second; 
  //   uminute = pCONT_time->uptime.minute; 
  //   uhour = pCONT_time->uptime.hour;   
  // }

  // memset(mxtime,0,sizeof(mxtime));
  // snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d %02dT%02d:%02d:%02d "),hour,minute,second,uday,uhour,uminute,usecond);

  // Overrides
  //uint8_t seriallog_level = LOG_LEVEL_DEBUG_MORE;
  //pCONT_log->seriallog_level = LOG_LEVEL_DEBUG_MORE;
  //pCONT_set->Settings.logging.serial_level = LOG_LEVEL_DEBUG;
  //pCONT_set->Settings.logging.web_level = LOG_LEVEL_INFO;

  // LOG : SERIAL
  if (loglevel <= pCONT_set->Settings.logging.serial_level) {
    SERIAL_DEBUG.printf("%s%s %s\r\n", 
    mxtime,
    pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer, sizeof(level_buffer)),  
    pCONT_log->log_data);
    //To stop asynchronous serial prints, flush it, but remove this under normal operation so code runs better (sends serial after the fact)
    // SERIAL_DEBUG.flush();
  }

  // LOG : TELNET
  // if (loglevel <= pCONT_set->Settings.telnetlog_level) {
  //   if((pCONT_log!=NULL)&&(pCONT_log->server!=NULL)){
  //     if(!pCONT_log->client.connected()) {
  //       pCONT_log->client = pCONT_log->server->available();
  //     }
  //     if(pCONT_log->client.connected()) {
  //       pCONT_log->client.printf("%s%s %s\r\n", mxtime,pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer), log_data);
  //     }
  //   }
  // }
  if(pCONT_log->Telnet){
    //   if(!pCONT_log->client.connected()) {
      
    //     pCONT_log->client = pCONT_log->server->available();
    //   }
    
  pCONT_log->Telnet.println("uptime: ");
    //   if(pCONT_log->client.connected()) {
    //     pCONT_log->client.printf("%s%s %s\r\n", mxtime, pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer), log_data);
    //   }
    // }
    }

  // LOG : WEBSERVER
  #ifdef USE_MODULE_NETWORK_WEBSERVER23
  if (pCONT_set->Settings.webserver && (loglevel <= pCONT_set->Settings.logging.web_level)) {
    // Delimited, zero-terminated buffer of log lines.
    // Each entry has this format: [index][log data]['\1']
    if (!pCONT_log->web_log_index) pCONT_log->web_log_index++;   // Index 0 is not allowed as it is the end of char string
    
    while (pCONT_log->web_log_index == pCONT_log->web_log[0] ||  // If log already holds the next index, remove it
           strlen(pCONT_log->web_log) + strlen(pCONT_log->log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
    {
      char* it = pCONT_log->web_log;
      it++;                                  // Skip web_log_index
      it += pCONT_sup->strchrspn(it, '\1'); // Skip log line
      it++;                                  // Skip delimiting "\1"
      memmove(pCONT_log->web_log, it, WEB_LOG_SIZE -(it-pCONT_log->web_log));  // Move buffer forward to remove oldest log line
    }

    // creates line formatted with \1 meaning EOL
    snprintf_P(pCONT_log->web_log, sizeof(pCONT_log->web_log), PSTR("%s%c%s%s %s\1"), pCONT_log->web_log, 
    pCONT_log->web_log_index++, mxtime, pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer, sizeof(level_buffer)), pCONT_log->log_data);
    if (!pCONT_log->web_log_index) pCONT_log->web_log_index++;   // Index 0 is not allowed as it is the end of char string
  
  // SERIAL_DEBUG.printf("WEBLOG");
  // SERIAL_DEBUG.printf(web_log_index);
  // // for(int i=web_log_index;i<500;i++){
  // //   SERIAL_DEBUG.printf(web_log[i]);
  // // }
  // SERIAL_DEBUG.printf("END WEBLOG");

  }
  #endif  // USE_MODULE_NETWORK_WEBSERVER23

}



// void AddLog_Basic(uint8_t loglevel, PGM_P formatP, ...)
// {

//   // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
//   if(
//     (loglevel>pCONT_set->Settings.logging.serial_level)&&
//     (loglevel>pCONT_set->Settings.logging.web_level)
//     ){
//     return;
//   }

//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
//   va_end(arg);

//   char level_buffer[10];

//   // LOG : SERIAL
//   if (loglevel <= pCONT_set->Settings.logging.serial_level) {
//     SERIAL_DEBUG.printf("%s%s %s\r\n", 
//       mxtime,
//       pCONT_log->GetLogLevelNameShortbyID(loglevel, level_buffer),  
//       log_data
//     );
//   }

// }

// uint8_t seriallog_level = LOG_LEVEL_DEBUG;
// char log_data[300];

// void AddLog_Basic(uint8_t loglevel, PGM_P formatP, ...)
// {

//   // Speed/stability improvements, check log level and return early if it doesnt apply to any log events
//   if(loglevel>seriallog_level){
//     return;
//   }

//   va_list arg;
//   va_start(arg, formatP);
//   vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
//   va_end(arg);

//   // LOG : SERIAL
//   if (loglevel <= seriallog_level) {
//     SERIAL_DEBUG.printf("%s %s\r\n", 
//       "INF: ",  
//       log_data
//     );
//   }

// }






/*********************************************************************************************\
 * Syslog
 *
 * Example:
 *   AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_LOG "Any value %d"), value);
 *
\*********************************************************************************************/

void mLogging::SetSeriallog(uint8_t loglevel)
{
  // Settings.seriallog_level = loglevel;
  // seriallog_level = loglevel;
  // seriallog_timer = 0;
}


void mLogging::GetLog(uint8_t idx, char** entry_pp, size_t* len_p)
{
  char* entry_p = nullptr;
  size_t len = 0;

  if (idx) {
    char* it = web_log;
    // get line
    do {
      uint8_t cur_idx = *it;
      it++;
      size_t tmp = pCONT_sup->strchrspn(it, '\1');
      tmp++;                             // Skip terminating '\1'
      if (cur_idx == idx) {              // Found the requested entry
        len = tmp;
        entry_p = it;
        break;
      }
      it += tmp;
    } while (it < web_log + WEB_LOG_SIZE && *it != '\0');
  }
  *entry_pp = entry_p;
  *len_p = len;
}
//#endif  // USE_MODULE_NETWORK_WEBSERVER23

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

void mLogging::AddLogAddLog(uint8_t loglevel)
{
//   char mxtime[10];  // "13:45:21 "

//   snprintf_P(mxtime, sizeof(mxtime), PSTR("%02d" D_HOUR_MINUTE_SEPARATOR "%02d" D_MINUTE_SECOND_SEPARATOR "%02d "), RtcTime.hour, RtcTime.minute, RtcTime.second);

//   if (loglevel <= seriallog_level) {
//     SERIAL_DEBUG.printf("%s%s\r\n", mxtime, log_data);
//   }
// #ifdef USE_MODULE_NETWORK_WEBSERVER23
//   if (Settings.webserver && (loglevel <= Settings.weblog_level)) {
//     // Delimited, zero-terminated buffer of log lines.
//     // Each entry has this format: [index][log data]['\1']
//     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
//     while (web_log_index == web_log[0] ||  // If log already holds the next index, remove it
//            strlen(web_log) + strlen(log_data) + 13 > WEB_LOG_SIZE)  // 13 = web_log_index + mxtime + '\1' + '\0'
//     {
//       char* it = web_log;
//       it++;                                // Skip web_log_index
//       it += strchrspn(it, '\1');           // Skip log line
//       it++;                                // Skip delimiting "\1"
//       memmove(web_log, it, WEB_LOG_SIZE -(it-web_log));  // Move buffer forward to remove oldest log line
//     }
//     snprintf_P(web_log, sizeof(web_log), PSTR("%s%c%s%s\1"), web_log, web_log_index++, mxtime, log_data);
//     if (!web_log_index) web_log_index++;   // Index 0 is not allowed as it is the end of char string
//   }
// #endif  // USE_MODULE_NETWORK_WEBSERVER23
//   if (!global_state.wifi_down && (loglevel <= syslog_level)) { Syslog(); }
}

void mLogging::AddLog(uint8_t loglevel, const char *formatP)
{
  // snprintf_P(log_data, sizeof(log_data), formatP);
  // AddLogAddLog(loglevel);
}

void mLogging::AddLog(uint8_t loglevel, const char *formatP, const char *formatP2)
{
  // char message[100];

  // snprintf_P(log_data, sizeof(log_data), formatP);
  // snprintf_P(message, sizeof(message), formatP2);
  // strncat(log_data, message, sizeof(log_data) - strlen(log_data) -1);
  // AddLogAddLog(loglevel);
}

void mLogging::AddLog_P2(uint8_t loglevel, PGM_P formatP, ...)
{
  // va_list arg;
  // va_start(arg, formatP);
  // vsnprintf_P(log_data, sizeof(log_data), formatP, arg);
  // va_end(arg);

  // AddLogAddLog(loglevel);
}

void mLogging::AddLogBuffer(uint8_t loglevel, uint8_t *buffer, int count)
{
  // snprintf_P(log_data, sizeof(log_data), PSTR("DMP:"));
  // for (int i = 0; i < count; i++) {
  //   snprintf_P(log_data, sizeof(log_data), PSTR("%s %02X"), log_data, *(buffer++));
  // }
  // AddLogAddLog(loglevel);
}

void mLogging::AddLogSerial(uint8_t loglevel)
{
  //AddLogBuffer(loglevel, (uint8_t*)serial_in_buffer, serial_in_byte_counter);
}

void mLogging::AddLogMissed(char *sensor, uint8_t misses)
{
  // AddLog(LOG_LEVEL_DEBUG, PSTR("SNS: %s missed %d"), sensor, SENSOR_MAX_MISS - misses);
}


/*********************************************************************************************\
 * Response data handling -- "AddLog" that are pushed to mqtt channel "status/response"
\*********************************************************************************************/

// return if response was sent, else use leds?
int Response_mP(const char* format, ...)     // Content send snprintf_P char data
{
  
  // if(pCONT_time->uptime.seconds_nonreset<60){ return 0 ;}

  memset(&pCONT_set->response_msg,0,sizeof(pCONT_set->response_msg));

  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int len = vsnprintf_P(pCONT_set->response_msg, RESPONSE_MESSAGE_BUFFER_SIZE, format, args);
  va_end(args);

//   //Share on serial/telnet
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_RESPONSE "%s"), pCONT_set->response_msg);
//   //Send via mqtt
//   #ifdef USE_MODULE_NETWORK_MQTT
//   pCONT_mqtt->ppublish("status/result",pCONT_set->response_msg,false);
//   #endif
  
  return 0;// len;
}

int ResponseAppend_mP(const char* format, ...)  // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  // va_list args;
  // va_start(args, format);
  // int mlen = strlen(pCONT_set->response_msg);
  // int len = vsnprintf_P(pCONT_set->response_msg + mlen, sizeof(pCONT_set->response_msg) - mlen, format, args);
  // va_end(args);
  // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_RESPONSE "Response_P %s"),pCONT_set->response_msg);
  return 0;// len + mlen;
}


void mLogging::StartTelnetServer(){
  
  // TelnetServer.begin();

  // if(pCONT_set->global_state.network_down) return;


  TelnetServer = new WiFiServer(TELNET_PORT);  // set port here
  TelnetServer->begin();

  telnet_started = true;
  // if(seriallog)
  // AddLog(LOG_LEVEL_INFO, PSTR("Telnet server started on port %d"),(uint8_t)TELNET_PORT);
}



//enum LoggingLevels {LOG_LEVEL_NONE, LOG_LEVEL_ERROR, LOG_LEVEL_INFO, 
//LOG_LEVEL_DEBUG, LOG_LEVEL_DEBUG_MORE, LOG_LEVEL_DEBUG_LOWLEVEL, LOG_LEVEL_ALL};


const char* mLogging::GetLogLevelNameShortbyID(uint8_t id, char* buffer, uint8_t buflen){
  if(buffer == nullptr){ return PM_SEARCH_NOMATCH;}
  switch(id){
    default: 
    case LOG_LEVEL_NONE:           memcpy_P(buffer, PM_LOG_LEVEL_NONE_SHORT_CTR, sizeof(PM_LOG_LEVEL_NONE_SHORT_CTR)); break;
    case LOG_LEVEL_DEBUG_TRACE: memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_TRACE_SHORT_CTR, sizeof(PM_LOG_LEVEL_DEBUG_TRACE_SHORT_CTR)); break;
    case LOG_LEVEL_ERROR:          memcpy_P(buffer, PM_LOG_LEVEL_ERROR_SHORT_CTR, sizeof(PM_LOG_LEVEL_ERROR_SHORT_CTR)); break;
    case LOG_LEVEL_WARN:           memcpy_P(buffer, PM_LOG_LEVEL_WARN_SHORT_CTR, sizeof(PM_LOG_LEVEL_WARN_SHORT_CTR)); break;
    case LOG_LEVEL_TEST:           memcpy_P(buffer, PM_LOG_LEVEL_TEST_SHORT_CTR, sizeof(PM_LOG_LEVEL_TEST_SHORT_CTR)); break;
    case LOG_LEVEL_HIGHLIGHT:           memcpy_P(buffer, PM_LOG_LEVEL_HIGHLIGHT_SHORT_CTR, sizeof(PM_LOG_LEVEL_HIGHLIGHT_SHORT_CTR)); break;
    case LOG_LEVEL_INFO:           memcpy_P(buffer, PM_LOG_LEVEL_INFO_SHORT_CTR, sizeof(PM_LOG_LEVEL_INFO_SHORT_CTR)); break;
    case LOG_LEVEL_COMMANDS:   memcpy_P(buffer, PM_LOG_LEVEL_COMMANDS_SHORT_CTR, sizeof(PM_LOG_LEVEL_COMMANDS_SHORT_CTR)); break;
    case LOG_LEVEL_DEBUG:          memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_SHORT_CTR, sizeof(PM_LOG_LEVEL_DEBUG_SHORT_CTR)); break;
    case LOG_LEVEL_DEBUG_MORE:     memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_MORE_SHORT_CTR, sizeof(PM_LOG_LEVEL_DEBUG_MORE_SHORT_CTR)); break;
    case LOG_LEVEL_DEBUG_LOWLEVEL: memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_LOWLEVEL_SHORT_CTR, sizeof(PM_LOG_LEVEL_DEBUG_LOWLEVEL_SHORT_CTR)); break;
    case LOG_LEVEL_ALL:            memcpy_P(buffer, PM_LOG_LEVEL_ALL_SHORT_CTR, sizeof(PM_LOG_LEVEL_ALL_SHORT_CTR)); break;
  }
  return buffer;        
}
const char* mLogging::GetLogLevelNamebyID(uint8_t id, char* buffer, uint8_t buflen){ //"Name" assumes Long friendy name
  if(buffer == nullptr){ return 0;}
  switch(id){ 
    default:
    case LOG_LEVEL_NONE:           memcpy_P(buffer, PM_LOG_LEVEL_NONE_LONG_CTR, sizeof(PM_LOG_LEVEL_NONE_LONG_CTR)); break;
    case LOG_LEVEL_DEBUG_TRACE: memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_TRACE_LONG_CTR, sizeof(PM_LOG_LEVEL_DEBUG_TRACE_LONG_CTR)); break;
    case LOG_LEVEL_ERROR:          memcpy_P(buffer, PM_LOG_LEVEL_ERROR_LONG_CTR, sizeof(PM_LOG_LEVEL_ERROR_LONG_CTR)); break;
    case LOG_LEVEL_WARN:           memcpy_P(buffer, PM_LOG_LEVEL_WARN_LONG_CTR, sizeof(PM_LOG_LEVEL_WARN_LONG_CTR)); break;
    case LOG_LEVEL_TEST:           memcpy_P(buffer, PM_LOG_LEVEL_TEST_LONG_CTR, sizeof(PM_LOG_LEVEL_TEST_LONG_CTR)); break;
    case LOG_LEVEL_INFO:           memcpy_P(buffer, PM_LOG_LEVEL_INFO_LONG_CTR, sizeof(PM_LOG_LEVEL_INFO_LONG_CTR)); break;
    case LOG_LEVEL_COMMANDS:   memcpy_P(buffer, PM_LOG_LEVEL_COMMANDS_LONG_CTR, sizeof(PM_LOG_LEVEL_COMMANDS_LONG_CTR)); break;
    case LOG_LEVEL_DEBUG:          memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_LONG_CTR, sizeof(PM_LOG_LEVEL_DEBUG_LONG_CTR)); break;
    case LOG_LEVEL_DEBUG_MORE:     memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_MORE_LONG_CTR, sizeof(PM_LOG_LEVEL_DEBUG_MORE_LONG_CTR)); break;
    case LOG_LEVEL_DEBUG_LOWLEVEL: memcpy_P(buffer, PM_LOG_LEVEL_DEBUG_LOWLEVEL_LONG_CTR, sizeof(PM_LOG_LEVEL_DEBUG_LOWLEVEL_LONG_CTR)); break;
    case LOG_LEVEL_ALL:            memcpy_P(buffer, PM_LOG_LEVEL_ALL_LONG_CTR, sizeof(PM_LOG_LEVEL_ALL_LONG_CTR)); break;
  }
  return buffer;        
}
int8_t mLogging::SetLogLevelIDbyName(const char* c){
  if(*c=='\0'){
    return LOG_LEVEL_DEBUG; //default
  }
  if(strcasecmp_P(c,PSTR("NONE"))==0){
    return LOG_LEVEL_NONE;
  }else if(strcasecmp_P(c,PSTR("ERROR"))==0){
    return LOG_LEVEL_ERROR;
  }else if(strcasecmp_P(c,PSTR("TEST"))==0){
    return LOG_LEVEL_TEST;
  }else if(strcasecmp_P(c,PSTR("INFO"))==0){
    return LOG_LEVEL_INFO;
  }else if(strcasecmp_P(c,PSTR("DEBUGMORE"))==0){
    return LOG_LEVEL_DEBUG_MORE;
  }else if(strcasecmp_P(c,PSTR("Commands"))==0){
    return LOG_LEVEL_COMMANDS;
  }else if(strcasecmp_P(c,PSTR("DEBUGLOWLEVEL"))==0){
    return LOG_LEVEL_DEBUG_LOWLEVEL;
  }else if(strcasecmp_P(c,PSTR("DEBUG"))==0){
    return LOG_LEVEL_DEBUG;
  }else if(strcasecmp_P(c,PSTR("ALL"))==0){
    return LOG_LEVEL_ALL;
  }else{
    return LOG_LEVEL_DEBUG;
  }
}



// ////////////////////////////////////////////////////////////////////////////////////////////////////
// void telnetHandleClient()
// { // Basic telnet client handling code from: https://gist.github.com/tablatronix/4793677ca748f5f584c95ec4a2b10303
//   if (debugTelnetEnabled)
//   { // Only do any of this if we're actually enabled
//     static unsigned long telnetInputIndex = 0;
//     if (telnetServer.hasClient())
//     { // client is connected
//       if (!telnetClient || !telnetClient.connected())
//       {
//         if (telnetClient)
//         {
//           telnetClient.stop(); // client disconnected
//         }
//         telnetClient = telnetServer.available(); // ready for new client
//         telnetInputIndex = 0;                    // reset input buffer index
//       }
//       else
//       {
//         telnetServer.available().stop(); // have client, block new connections
//       }
//     }
//     // Handle client input from telnet connection.
//     if (telnetClient && telnetClient.connected() && telnetClient.available())
//     { // client input processing
//       static char telnetInputBuffer[telnetInputMax];

//       if (telnetClient.available())
//       {
//         char telnetInputByte = telnetClient.read(); // Read client byte
//         if (telnetInputByte == 5)
//         { // If the telnet client sent a bunch of control commands on connection (which end in ENQUIRY/0x05), ignore them and restart the buffer
//           telnetInputIndex = 0;
//         }
//         else if (telnetInputByte == 13)
//         { // telnet line endings should be CRLF: https://tools.ietf.org/html/rfc5198#appendix-C
//           // If we get a CR just ignore it
//         }
//         else if (telnetInputByte == 10)
//         {                                          // We've caught a LF (DEC 10), send buffer contents to the Nextion
//           telnetInputBuffer[telnetInputIndex] = 0; // null terminate our char array
//           nextionSendCmd(String(telnetInputBuffer));
//           telnetInputIndex = 0;
//         }
//         else if (telnetInputIndex < telnetInputMax)
//         { // If we have room left in our buffer add the current byte
//           telnetInputBuffer[telnetInputIndex] = telnetInputByte;
//           telnetInputIndex++;
//         }
//       }
//     }
//   }
// }
