/**
 * @file mSIM7000G.cpp
 * @brief Driver module for the SIMCom SIM7000G multi-function cellular modem.
 *
 * The SIM7000G is treated as a hardware driver rather than a pure network module.
 * Although it provides LTE-M/NB-IoT data connectivity, it also exposes GNSS,
 * SMS, SIM status, signal diagnostics, modem power control, and device-specific
 * AT command behaviour through the same UART interface.
 *
 * This module owns the SIM7000G hardware control path, including UART setup,
 * baud-rate recovery, PWRKEY sequencing, modem readiness probing, AT command
 * coordination, and SIM7000G-specific capability handling. Higher-level
 * interface modules consume these capabilities through narrow abstractions:
 *
 * - Cellular/network modules consume only the data-network transport capability.
 * - GPS/location interfaces consume GNSS fixes reported by the modem driver.
 * - SMS/command interfaces consume parsed SMS events.
 * - Telemetry/status interfaces consume signal, SIM, battery, and modem state.
 *
 * In short:
 *   SIM7000G driver  = hardware and AT-command owner
 *   Cellular module  = network transport adapter
 *   NetworkInterface = top-level transport policy/orchestration
 */
#include "_mSIM7000G.h"

#ifdef USE_MODULE_DRIVERS_MODEM_7000G

int8_t mSIM7000G::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;  
  }

  if(!settings.fEnableSensor){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
    {      
      if(flag_modem_initialized)
      {

      //   #ifndef ENABLE_DEVFEATURE_STOP_MQTT_FROM_CONNECTING
      //   ModemUpdate_GPRS();
      //   #endif
        
        #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
        GNSS_Update();
        #endif
        
      //   ModemUpdate_SMS();        
        
      }

      // // When +CMTI is parsed, indices are pushed into sms.messages_incoming_index_list
      // if (sms.messages_incoming_index_list.size())
      // {
      //   SMSReadAndEraseSavedSMS();   // reads +CMGR, calls your handler, then +CMGD
      // }

      // // If you want periodic polling for waiting SMS (optional)
      // ModemUpdate_SMS();

    }
    break;
    case TASK_EVERY_250_MSECOND:{

      ModemInit_Tick(millis());

      if (modem)
      {
        Handler_ModemResponses(LOG_LEVEL_INFO);
        modem->maintain();
      }

      #ifdef ENABLE_FEATURE_SIM__SMS
      if(sms2.init_done)
        SMS2_Task_Tick(millis());
      #endif

    }break;
    case TASK_EVERY_SECOND:
    {
      Query_Modem_Status();

      #ifdef ENABLE_FEATURE_SIM__SMS
      if ((!sms2.init_done) && (modem_sm_.state == modem_init_state_t::READY))
      {
        SMS2_Init_NotifyOnly_Blocking();

        SendAT_WaitResponse__SMSNumberStored(200);
        SendAT_WaitResponse__SMSDeleteAll(200);
      }

      SubTask_SMS__Read_Any_Waiting_Messages();

      // SMS2_Task_Tick(millis());
      #endif
    }
    break;
    case TASK_EVERY_FIVE_SECOND:   
    {
      Query_Modem_Status();
      
      #ifdef ENABLE_FEATURE_SIM__SMS
      SendAT_WaitResponse__SMSNumberStored(200);
      #endif
      // SendAT_ATParseResponse_F(500, LOG_LEVEL_INFO, PSTR("+CNMI?"));
      // SendAT_ATParseResponse_F(500, LOG_LEVEL_INFO, PSTR("+CMGF?"));
      // SendAT_ATParseResponse_F(500, LOG_LEVEL_INFO, PSTR("+CPMS?"));

      // // After pushing new_sms_index
      // SendAT_ATParseResponse_F(1000, LOG_LEVEL_INFO, PSTR("+CMGR=%d"), 4);
      // SendAT_F(500, PSTR("+CMGD=%d"), 4);


      // tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_LOG__SHOW_UPTIME,1000));
  
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      // ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100)); 
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

    }
    break;
    case TASK_EVERY_MINUTE:

      // SMS_SendBootMessage();
      // SendATCommand_SMSImmediateForwardOverSerial();
      // ModemUpdate_BatteryStatus();
      // SMS_Send_TimedHeartbeat();

      #ifdef ENABLE_DEVFEATURE_TEXT_LOCATION_EVERY_MINUTE_WHEN_POWERED
      SMS_GPSLocation();
      #endif 

      // SubTask_SMS__Check_Any_SMS_Saved();

    break;
    case TASK_EVERY_FIVE_MINUTE:
    
      // SMS_Send_TimedHeartbeat2();
      break;
    case TASK_EVERY_30_MINUTES:
    
    break;
    case TASK_EVERY_HOUR:
      #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
      SMS_Send_TimedHeartbeat();
      #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case TASK_MQTT_CONNECTED:
      MQTTHandler_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT    
  }

  return TASKER_RESULT__SUCCESS_ID;

} // END function




void mSIM7000G::Pre_Init(void){
  
  pinMode(12, OUTPUT);

  #ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
  if (!stream_debugger) {
    stream_debugger = new StreamDebugger(SerialAT, Serial);
  }
  #endif

  ALOG_INF(PSTR(D_LOG_SIM7000G "Pre_Init"));
  
}
void mSIM7000G::Init(void)
{
  ALOG_INF(PSTR(D_LOG_SIM7000G "Init"));

  if (modem == nullptr)
  {
    #ifdef ENABLE_FEATURE_CELLULAR_ATCOMMANDS_STREAM_DEBUGGER_OUTPUT
      modem = new TinyGsm(*stream_debugger);
    #else
      modem = new TinyGsm(SerialAT);
    #endif
  }

  memset(&modem_probe_, 0, sizeof(modem_probe_));
  modem_probe_.csq = -1;
  modem_probe_.model[0] = '\0';

  modem_sm_.busy              = true;
  modem_sm_.ready             = false;
  modem_sm_.attempts          = 0;
  modem_sm_.restart_requested = false;
  modem_sm_.probe_done        = false;

  flag_modem_initialized = false;

  settings.fEnableSensor = true;

  const bool ok = Modem_EnsurePowerOnAndBaud(MODEM_TARGET_BAUD);

  if (!ok)
  {
    modem_sm_.busy  = false;
    modem_sm_.ready = false;
    modem_sm_.state = modem_init_state_t::FAILED;

    ALOG_ERR(PSTR(D_LOG_CELLULAR "Init failed: modem power/baud not established"));
    return;
  }

  ModemInit_ProbeFillBasic();

  modem_sm_.state = modem_init_state_t::READY;
  modem_sm_.busy  = false;
  modem_sm_.ready = true;

  flag_modem_initialized = true;

  ALOG_INF(
    PSTR(D_LOG_CELLULAR "Init complete: modem READY baud=%lu model=%s csq=%d"),
    (unsigned long)MODEM_TARGET_BAUD,
    modem_probe_.model,
    modem_probe_.csq
  );
}


/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSIM7000G::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Add("SMSEnabled", sms.enabled);
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Add("SMSAutoPosition", smsauto_gps_messages.rate_seconds);
    #endif

    JBI->Add("GPRSEnabled", gprs.enabled);
    
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Add("GPSEnabled", gps.enabled);
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    
  return JBI->End();

}


uint8_t mSIM7000G::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  JBI->Start();

    // JBI->Add("uptime", tkr_time->uptime_seconds_nonreset);
    // JBI->Add("rtcseconds", tkr_time->RtcTime.second);
    // JBI->Object_Start("GPRS");
    //     JBI->Add("ConSec", gprs.connected_seconds);
    //     JBI->Add("rss_dbm", gprs.signal_quality_rssi_dbm);
    // JBI->Object_End();
    // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // JBI->Object_Start("GPS");
    //   JBI->Add("second", gps.second);
    //   JBI->Add("Speed", gps.speed);
    //   JBI->Add("altitude", gps.altitude);
    //   JBI->Add("Latitude", gps.latitude);
    //   JBI->Add("Longitude", gps.longitude);
    //   JBI->Add("Enabled", gps.enabled);
    //   JBI->Add("vsat", gps.vsat);
    //   JBI->Add("usat", gps.usat);
    //   JBI->Add("accuracy", gps.accuracy);
    //   JBI->Add("year", gps.year);
    //   JBI->Add("month", gps.month);
    //   JBI->Add("day", gps.day);
    //   JBI->Add("hour", gps.hour);
    //   JBI->Add("minute", gps.minute);
    //   JBI->Add("course", gps.course);
    //   JBI->Add("FixMode", gps.FixMode);
    //   JBI->Add("accuracy_position", gps.accuracy_position);
    //   JBI->Add("accuracy_vertical", gps.accuracy_vertical);
    //   JBI->Add("usat_glonass", gps.usat_glonass);
    //   JBI->Add("cno_max", gps.cno_max);
    //   JBI->Add("HPA", gps.HPA);
    //   JBI->Add("VPA", gps.VPA);

    //   char convf_lat[TBUFFER_SIZE_FLOAT];
    //   mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    //   char convf_lon[TBUFFER_SIZE_FLOAT];
    //   mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    //   char convf_fix[TBUFFER_SIZE_FLOAT];
    //   mSupport::float2CString(gps.accuracy,2,convf_fix);

    //   snprintf_P(buffer, sizeof(buffer),
    //     PSTR(
    //       "https://www.google.com/maps/dir//%s,%s"
    //     ),
    //     convf_lat, 
    //     convf_lon
    //   );

    //   JBI->Add("url", buffer);
    // JBI->Object_End();
    // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    // JBI->Object_Start("Battery");
    //   JBI->Add("Volts_mv", modem_status.battery.volts_mv);
    //   JBI->Add("Percentage", modem_status.battery.percentage);
    //   JBI->Add("ChargeState", modem_status.battery.charge_state);
    //   JBI->Add("Valid", modem_status.battery.isvalid);
    // JBI->Object_End();
  
  return JBI->End();

}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mSIM7000G::MQTTHandler_Init()
{

  struct handler<mSIM7000G>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSIM7000G::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mSIM7000G::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSIM7000G::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSIM7000G::MQTTHandler_Rate()
{
  // While developing, set these by the Init() function to make independent
  // for(auto& handle:mqtthandler_list){
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
  //     handle->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
  //   if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
  //     handle->tRateSecs = tkr_mqtt->dt.ifchanged_secs;
  // }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mSIM7000G::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    tkr_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT



#endif
