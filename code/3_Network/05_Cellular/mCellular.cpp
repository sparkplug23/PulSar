/**
 * @file mCellular.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-29
 * 
 * @note Cellular should only be handling interface level. 
 *       In relation to GPS, it should be read here if available then inserted into sensor interface
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"

#ifdef USE_MODULE_NETWORK_CELLULAR

#ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
StreamDebugger debugger(SerialAT, Serial);
#endif // ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT

const char* mCellular::PM_MODULE__NETWORK_CELLULAR__CTR = D_MODULE__NETWORK_CELLULAR__CTR;
const char* mCellular::PM_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR = D_MODULE__NETWORK_CELLULAR__FRIENDLY_CTR;

int8_t mCellular::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
    {      

      Handler_ModemResponses(LOG_LEVEL_DEBUG_MORE);
      
      /**
       * @brief Here so it can be set from anywhere/method and then read later
       * 
       */
      if(sms.messages_incoming_index_list.size())
      {
        SMSReadAndEraseSavedSMS();
      } 
      
      if(flag_modem_initialized)
      {

        #ifndef ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
        ModemUpdate_GPRS();
        #endif
        
        #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
        ModemUpdate_GPS();
        #endif
        
        ModemUpdate_SMS();        
        
      }

    }
    break;
    case FUNC_EVERY_50_MSECOND:{

      if(modem){
        modem->maintain();
      }

      bool is_connected = modem->isNetworkConnected();
      if(!is_connected)
      {
        ALOG_ERR(PSTR(D_LOG_CELLULAR "isNetworkConnected %d"), is_connected);    
      }

    }break;
    case FUNC_EVERY_SECOND: 

      if(!flag_modem_initialized)
      {

        DEBUG_LINE_HERE;

        if(Modem_CheckAndRestartUnresponsiveModem() == false) // break until rebooted
        {
          break;
        }

        // Only complete config if modem is working as expected, otherwise retry init again
        if(DataNetwork__InitConfig())
        {
          
          DataNetwork__StartConnection();

          #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
          GPS_Enable();
          #endif 

          SMS_Enable();

          flag_modem_initialized = true;
          DEBUG_LINE_HERE;
        }


      }

      AutoSMS_Messages_Handle();
      
      #ifdef ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES

      Serial.printf(PSTR("\n\r\n\r=========================SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS\n\r")); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "waitresponse_error_counter %d"), modem->waitresponse_error_counter); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.apn_connect_called %d"), gprs.apn_connect_called); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.reconnect_init_counts %d"), gprs.reconnect_init_counts); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "gprs.connected_seconds %d"), gprs.connected_seconds);    

      ALOG_INF(PSTR(D_LOG_CELLULAR "mqtt downtime counter %d"), pCONT_mqtt->brokers[0]->downtime_counter);    

      if(pCONT_mqtt->brokers.size()){ 
        ALOG_INF(PSTR(D_LOG_CELLULAR "pubsub->connected %d"), pCONT_mqtt->brokers[0]->pubsub->connected() );        
        char buffer[20];
        mSupport::float2CString(pCONT_mqtt->brokers[0]->debug_stats.payload_publish_success_percentage*100, 6, buffer);
        ALOG_INF(PSTR(D_LOG_CELLULAR "payload_publish_success_percentage %d/%d %s"), pCONT_mqtt->brokers[0]->debug_stats.payload_publish_sent, pCONT_mqtt->brokers[0]->debug_stats.payload_publish_missed, buffer);
      }
      ALOG_INF(PSTR(D_LOG_CELLULAR "isGprsConnected %d"), modem->isGprsConnected());   
      ALOG_INF(PSTR(D_LOG_CELLULAR "Sim Connected %d"), modem->isNetworkConnected()); 
      ALOG_INF(PSTR(D_LOG_CELLULAR "smsauto_gps_messages.rate_seconds %d"), smsauto_gps_messages.rate_seconds); 
      
      ALOG_INF(PSTR(D_LOG_CELLULAR "function_event_queue %d"), pCONT->function_event_queue.size()); 

      Serial.printf(PSTR("=========================EEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEEE\n\r\n\r")); 

      #endif // ENABLE_DEBUGFEATURE__CELLULAR_CONNECTION_ISSUES


      #ifdef ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS
      // Need to enable passing back if "send failed" to set connection status as down     
      if(pCONT_mqtt->brokers.size())
      {
        if(pCONT_mqtt->brokers[0]->downtime_counter > ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS) // 10 minutes, do long modem reconnect
        {
        DEBUG_LINE_HERE;

          ALOG_INF(PSTR(D_LOG_CELLULAR "MQTT Downtime limit on LTE, forcing modem restart"));
          flag_modem_initialized = false; // no response, force restart
        DEBUG_LINE_HERE;

        }
      }
      #endif // ENABLE_DEVFEATURE__MODEM_FORCE_RECONNECT_WHEN_MQTT_IS_DISCONNECTED_SECONDS

    break;
    case FUNC_EVERY_FIVE_SECOND:   
    {
      
      // pCONT->function_event_queue.push_back(pCONT->FunctionEvent(FUNC_LOG__SHOW_UPTIME,1000));
  
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100)); 
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    }
    break;
    case FUNC_EVERY_MINUTE:

        // SendATCommand_SMSImmediateForwardOverSerial();
      // ModemUpdate_BatteryStatus();
      // SMS_Send_TimedHeartbeat();

      #ifdef ENABLE_DEVFEATURE_TEXT_LOCATION_EVERY_MINUTE_WHEN_POWERED
      SMS_GPSLocation();
      #endif 

    break;
    case FUNC_EVERY_HOUR:
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      SMS_Send_TimedHeartbeat();
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }

} // END function


void mCellular::Pre_Init(void){
  
  settings.fEnableSensor = true;

  pinMode(12, OUTPUT);
  
}


void mCellular::Init(void)
{

  #ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT  // if enabled it requires the streamDebugger lib
  modem = new TinyGsm(debugger);
  #else
  modem = new TinyGsm(SerialAT);
  #endif

  #ifdef USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED
    SerialAT.begin(115200, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
    delay(100);
    modem->setBaud(UART_CELLULAR_BAUD); // Change from default
    SerialAT.flush();
    SerialAT.end();
    delay(100);
    SerialAT.begin(UART_CELLULAR_BAUD, SERIAL_8N1, PIN_RX, PIN_TX); // #define UART_BAUD   115200
  #else 
    SerialAT.begin(115200, SERIAL_8N1, 26,27);// PIN_RX, PIN_TX);
  #endif // USE_MODULE_NETWORK_CELLULAR__USE_FASTER_BAUD_SPEED

  SerialAT.setTimeout(100); //ms

  /**
   * @brief Power on
   **/ 
  modemPowerOn();

  #ifdef ENABLE_DEVFEATURE_SIM7000G_INIT_SKIP_MODEM_RESTART
    ALOG_INF(PSTR(D_LOG_CELLULAR "Faster init modem"));
    if (!modem->init()) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Failed to init modem"));
    }
  #else
    // Restart takes quite some time, to skip it, call init() instead of restart()
    ALOG_INF(PSTR(D_LOG_CELLULAR "Slower restarting modem"));
    if (!modem->restart()) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Failed to restart modem"));
    }
  #endif

}


bool mCellular::Handler_ModemResponses(uint8_t response_loglevel, uint16_t wait_millis)
{

  if (SerialAT.available()) 
  {

    uint32_t receive_millis = millis();

    uint16_t buflen = 0;
    char buffer[300] = {0};


  // delay(50);


    while(SerialAT.available() || ((millis()-receive_millis)<100)) // wait at least 100 between AT_Send and receive
    {
      if (SerialAT.available()) 
      {
        buffer[buflen++] = SerialAT.read();
        if(buflen>(sizeof(buffer)-1))
        {
          ALOG_ERR(PSTR(D_LOG_CELLULAR "RX Overflow"));
          break;
        }
        // if
        // {
        //   ALOG_ERR(PSTR(D_LOG_CELLULAR "RX Timeout"));
        //   break;
        // }
      }
    }

    AddLog(response_loglevel, PSTR(D_LOG_CELLULAR "Handler_ModemResponses %d>> Buffer[%d] \"%s\""), response_loglevel, buflen, buffer);
     
    return parse_ATCommands(buffer, buflen, response_loglevel);

  }

  return false; // nothing handled or received

}



/**
 * @brief 
 * Response syntax:
  +CFUN: <fun>
  The read response parameter and its defined value are the following:
  <fun> 
    0 – Power off and store. RF circuits are disabled by deactivating LTE and Global Navigation Satellite System (GNSS) services.
    1 – Normal mode. The active mode is either LTE or GNSS, or both. Full functional mode. Active modes depend on %XSYSTEMMODE setting.
    2 – Receive only mode in active system mode. Active modes depend on %XSYSTEMMODE setting.
    4 – Flight mode. RF circuits are disabled by deactivating LTE and GNSS services. 
  **/
void mCellular::SendATCommand_FunctionalityMode_Minimum()
{
  modem->sendAT("+CFUN=0"); 
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN);
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Minimum %d"), result);
}
void mCellular::SendATCommand_FunctionalityMode_Full()
{
  modem->sendAT("+CFUN=1");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN); 
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Full %d"), result);
}



void mCellular::ModemUpdate_BatteryStatus()
{
  modem_status.battery.isvalid = modem->getBattStats(
    modem_status.battery.charge_state, 
    modem_status.battery.percentage, 
    modem_status.battery.volts_mv
  );

  ALOG_DBM(PSTR(D_LOG_CELLULAR "{\"Volts_mv\":%d,\"Percent\":%d,\"Charging\":%d}"),
    modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);

  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  pCONT_batt_modem->readings.battery.volts_mv = modem_status.battery.volts_mv;
  pCONT_batt_modem->readings.battery.percentage = modem_status.battery.percentage;
  #endif

}


void mCellular::modemPowerOn()
{
  ALOG_INF(PSTR("modemPowerOn"));
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);    // Datasheet T_on = 72ms
  digitalWrite(PWR_PIN, HIGH);
  flag_modem_initialized = false;
        DEBUG_LINE_HERE;

}

void mCellular::modemPowerOff()
{
  ALOG_INF(PSTR("modemPowerOff"));
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);    // Datasheet T_off = 1.2secs
  digitalWrite(PWR_PIN, HIGH);
}

void mCellular::modemRestart()
{
  ALOG_INF(PSTR("modemRestart"));
  modemPowerOff();
  delay(5000);
  modemPowerOn();
}


// ==============================================================================================




bool mCellular::Modem_CheckAndRestartUnresponsiveModem()
{
  
  uint16_t timeout_wait_for_AT_response = 500;

  if(modem->testAT(timeout_wait_for_AT_response))
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Running"));
    return true;
  }else{
    ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Not responding, restarting (TBC: causes 7.5 second pause)"));
    modemRestart(); // If no response, cause reboot and do something else
    return false;
  }

}




float mCellular::GetSignalQualityPower()
{
  return GetSignalQualityPower(modem->getSignalQuality());
}

/**
 * @brief Conversion without calling modem again
 * 
 * @param signal_quality_raw 
 * @return float 
 */
float mCellular::GetSignalQualityPower(int16_t signal_quality_raw)
{

  float signal_quality_rssi_dbm = 0;

  if(signal_quality_raw == 99)
  {
    signal_quality_rssi_dbm = -150;
  }
  else
  {
    signal_quality_rssi_dbm = mSupport::mapfloat(signal_quality_raw, 0, 31, -113, -51);
  }

  return signal_quality_rssi_dbm;

}



void mCellular::Get_Modem_Hardware()
{

  String name = modem->getModemName();
  ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Name: %s"),name.c_str());

  String modemInfo = modem->getModemInfo();
  ALOG_INF(PSTR(D_LOG_CELLULAR "Modem Info: %s"),modemInfo.c_str());


}




/**
 * @brief Mostly to debug, but also return if it worked
 * 
 * @param buffer 
 * @return true 
 * @return false 
 */
bool mCellular::SendAT(const char* buffer, uint16_t wait_millis)
{
  if(!modem){ return false; }

  uint32_t tSaved_millis = millis();

  modem->sendAT(buffer);
  bool result = true;
  if(wait_millis)
  {
    result = modem->waitResponse(wait_millis);
  }
  uint32_t tSaved_Elapsed = millis()-tSaved_millis;

  ALOG_INF(PSTR(D_LOG_CELLULAR ">>>%s %s (%dms)"), buffer, result?"Success":"FAILED", tSaved_Elapsed);

  return result;
  
}


bool mCellular::SendAT_F(uint16_t wait_millis, PGM_P formatP, ...)
{

  char command_buffer[200];

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(command_buffer, sizeof(command_buffer), formatP, arg);
  va_end(arg);

  return SendAT(command_buffer, wait_millis);

}


bool mCellular::SendAT_ATParseResponse_F(uint16_t wait_millis, uint8_t response_loglevel, PGM_P formatP, ...)
{

  // AddLog(response_loglevel, PSTR("response_loglevel=%d"),response_loglevel);

  char command_buffer[200];

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(command_buffer, sizeof(command_buffer), formatP, arg);
  va_end(arg);
  
  if(!modem){ return false; }

  uint32_t tSaved_millis = millis();

  modem->sendAT(command_buffer);
  

  /**
   * @brief With timeout, expect response to be parsed
   * 
   */
  uint32_t tSaved_Elapsed = millis()-tSaved_millis;
  while(!Handler_ModemResponses(response_loglevel))
  {
    if(
      (tSaved_Elapsed = (millis()-tSaved_millis)) > wait_millis
    ){
      ALOG_ERR(PSTR(D_LOG_CELLULAR "No response (%dms)"), tSaved_Elapsed);
      return false;
    }

  }

  // ALOG_ERR(PSTR(D_LOG_CELLULAR "========================================Response (%dms)"), millis()-tSaved_millis);
  
  return true;

}
  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mCellular::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add("SMSEnabled", sms.enabled);
    JBI->Add("GPRSEnabled", gprs.enabled);
    
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Add("GPSEnabled", gps.enabled);
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    
  return JBI->End();

}


uint8_t mCellular::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    JBI->Add("uptime", pCONT_time->uptime_seconds_nonreset);
    JBI->Add("rtcseconds", pCONT_time->RtcTime.second);
    JBI->Level_Start("GPRS");
        JBI->Add("ConSec", gprs.connected_seconds);
        JBI->Add("rss_dbm", gprs.signal_quality_rssi_dbm);
    JBI->Level_End();
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Level_Start("GPS");
      JBI->Add("second", gps.second);
      JBI->Add("Speed", gps.speed);
      JBI->Add("altitude", gps.altitude);
      JBI->Add("Latitude", gps.latitude);
      JBI->Add("Longitude", gps.longitude);
      JBI->Add("Enabled", gps.enabled);
      JBI->Add("vsat", gps.vsat);
      JBI->Add("usat", gps.usat);
      JBI->Add("accuracy", gps.accuracy);
      JBI->Add("year", gps.year);
      JBI->Add("month", gps.month);
      JBI->Add("day", gps.day);
      JBI->Add("hour", gps.hour);
      JBI->Add("minute", gps.minute);
      JBI->Add("course", gps.course);
      JBI->Add("FixMode", gps.FixMode);
      JBI->Add("accuracy_position", gps.accuracy_position);
      JBI->Add("accuracy_vertical", gps.accuracy_vertical);
      JBI->Add("usat_glonass", gps.usat_glonass);
      JBI->Add("cno_max", gps.cno_max);
      JBI->Add("HPA", gps.HPA);
      JBI->Add("VPA", gps.VPA);

      char convf_lat[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
      char convf_lon[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
      char convf_fix[TBUFFER_SIZE_FLOAT];
      mSupport::float2CString(gps.accuracy,2,convf_fix);

      snprintf_P(buffer, sizeof(buffer),
        PSTR(
          "https://www.google.com/maps/dir//%s,%s"
        ),
        convf_lat, 
        convf_lon
      );

      JBI->Add("url", buffer);
    JBI->Level_End();
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Level_Start("Battery");
      JBI->Add("Volts_mv", modem_status.battery.volts_mv);
      JBI->Add("Percentage", modem_status.battery.percentage);
      JBI->Add("ChargeState", modem_status.battery.charge_state);
      JBI->Add("Valid", modem_status.battery.isvalid);
    JBI->Level_End();
  
  return JBI->End();

}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mCellular::MQTTHandler_Init()
{

  struct handler<mCellular>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 2; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 2; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mCellular::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mCellular::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mCellular::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE__NETWORK_CELLULAR__ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_NETWORK_CELLULAR