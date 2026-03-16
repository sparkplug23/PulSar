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
 * To be split into the driver module.
 * Cellular should just be a wrapper/top level API that translates whichever modem is used into a common interface that works with WiFi/Ethernet and just assumes the connection is there (ie handled by the driver)
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "mCellular.h"

#ifdef USE_MODULE_NETWORK_CELLULAR


int8_t mCellular::Tasker(uint8_t function, JsonParserObject obj)
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

    }
    break;
    case TASK_EVERY_SECOND:
{
  // 1Hz policy loop: connect GPRS, then start MQTT using modem-provided TinyGsmClient
  Cellular_ConnMgr_Tick_1s(millis());
}
break;

    case TASK_EVERY_FIVE_SECOND:   
    {
      
    }
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

  return TASKER_RESULT__UNKNOWN_ID;

} // END function


void mCellular::Pre_Init(void){
  
  settings.fEnableSensor = true;
  
}

void mCellular::Init(void)
{
  Cellular_ConnMgr_Reset();
}



void mCellular::Cellular_ConnMgr_Reset()
{
  conn_sm_.state = cellular_conn_state_t::WAIT_MODEM_READY;
  conn_sm_.t_enter_ms = millis();
  conn_sm_.t_next_action_ms = 0;

  conn_sm_.attempts = 0;
  conn_sm_.init_config_done = false;
  conn_sm_.mqtt_started = false;

  conn_sm_.last_gprs_connected = false;
}

void mCellular::Cellular_ConnMgr_Tick_1s(uint32_t now_ms)
{
  // Hard dependency: modem driver must exist
  if (!tkr_modem) return;

  #ifndef ENABLE_FEATURE_CELLULAR__INCLUDE_MOBILE_NETWORKS
  return; //tmp block networking, to focus on sms
  #endif

  // Gate: only act when modem driver baseline is ready (AT responsive)
  if (!tkr_modem->IsReady())
  {
    // If we were online previously, collapse state
    if (conn_sm_.state != cellular_conn_state_t::WAIT_MODEM_READY) {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "CELL: Modem not ready, returning to WAIT_MODEM_READY"));
      Cellular_ConnMgr_Reset();
    }
    return;
  }

  const bool gprs_connected = tkr_modem->DataNetwork_IsConnected();

  ALOG_INF(PSTR("gprd_Connected %d %d"),gprs_connected,conn_sm_.state);

  // Detect drop while “ONLINE”
  if (conn_sm_.state == cellular_conn_state_t::ONLINE)
  {
    if (!gprs_connected)
    {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "CELL: GPRS dropped, entering BACKOFF"));
      conn_sm_.mqtt_started = false;     // allow re-create if needed
      conn_sm_.attempts++;
      _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
    }
    return; // online and still connected => nothing to do at 1Hz
  }

  // Simple rate-limit for expensive calls (prevents hammering)
  if (conn_sm_.t_next_action_ms && (int32_t)(now_ms - conn_sm_.t_next_action_ms) < 0) {
    return;
  }

  switch (conn_sm_.state)
  {
    default:
    case cellular_conn_state_t::WAIT_MODEM_READY:
    {
      // modem is ready (we passed gate), move on
      conn_sm_.attempts = 0;
      conn_sm_.init_config_done = false;
      conn_sm_.mqtt_started = false;
      _conn_enter(conn_sm_, cellular_conn_state_t::INIT_CONFIG, now_ms);
      conn_sm_.t_next_action_ms = now_ms; // immediate
    }
    break;

    case cellular_conn_state_t::INIT_CONFIG:
    {
      // Run once per boot/restart (operator preference, band, etc.)
      // NOTE: your current DataNetwork__InitConfig() contains blocking loops.
      // This policy SM ensures it is *not called repeatedly*.
      ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: DataNetwork__InitConfig (attempt %u)"), conn_sm_.attempts + 1);

      const bool ok = tkr_modem->DataNetwork__InitConfig();
      conn_sm_.init_config_done = ok;

      if (ok) {
        _conn_enter(conn_sm_, cellular_conn_state_t::START_CONNECTION, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 1000; // small settle
      } else {
        conn_sm_.attempts++;
        _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 5000;
      }
    }
    break;

    case cellular_conn_state_t::START_CONNECTION:
    {
      // If already connected, skip straight to MQTT stage
      if (gprs_connected) {
        _conn_enter(conn_sm_, cellular_conn_state_t::START_MQTT, now_ms);
        conn_sm_.t_next_action_ms = now_ms;
        break;
      }

      ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: DataNetwork__StartConnection (attempt %u)"), conn_sm_.attempts + 1);

      const bool ok = tkr_modem->DataNetwork__StartConnection();

      if (ok && tkr_modem->DataNetwork_IsConnected()) {
        _conn_enter(conn_sm_, cellular_conn_state_t::START_MQTT, now_ms);
        conn_sm_.t_next_action_ms = now_ms;
      } else {
        conn_sm_.attempts++;
        _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 5000;
      }
    }
    break;

    case cellular_conn_state_t::START_MQTT:
    {
      // Must have data plane up
      if (!tkr_modem->DataNetwork_IsConnected()) {
        _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 3000;
        break;
      }

      if (!conn_sm_.mqtt_started)
      {
        TinyGsmClient* gsm_client = tkr_modem->DataNetwork_GetOrCreateClient(false);
        if (!gsm_client) {
          ALOG_ERR(PSTR(D_LOG_CELLULAR "CELL: GSM client creation failed"));
          conn_sm_.attempts++;
          _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
          conn_sm_.t_next_action_ms = now_ms + 5000;
          break;
        }

        ALOG_HGL(PSTR(D_LOG_CELLULAR "CELL: Starting MQTT over cellular"));
        tkr_mqtt->CreateConnection(
          gsm_client,
          MQTT_HOST_CELLULAR,
          MQTT_PORT_CELLULAR,
          CLIENT_TYPE_CELLULAR_ID,
          MQTT_HOST__USERNAME,
          MQTT_HOST__PASSWORD
        );

        
          tkr_mqtt->brokers.back()->SetCredentials(MQTT_USER, MQTT_PASS);
          DEBUG_LINE_HERE3

          tkr_mqtt->brokers.back()->SetReConnectBackoffTime(MQTT_RETRY_SECS);
          DEBUG_LINE_HERE3
          
          // char client_name[100]; snprintf_P(client_name, sizeof(client_name), PSTR("%s-%s"), tkr_set->Settings.system_name.device, WiFi.macAddress().c_str()); 
          
          uint8_t mac[6];           WiFi.macAddress(mac);
          DEBUG_LINE_HERE3
          char client_name[100]; snprintf_P(client_name, sizeof(client_name), PSTR("%s-%02X:%02X:%02X"), tkr_set->Settings.system_name.device, mac[3], mac[4], mac[5]); 
          DEBUG_LINE_HERE3
          tkr_mqtt->brokers.back()->SetClientName(client_name);
          DEBUG_LINE_HERE3

          tkr_mqtt->brokers.back()->SetTopicPrefix(tkr_set->Settings.system_name.device);

          tkr_mqtt->brokers.back()->SetCredentials(MQTT_HOST__USERNAME, MQTT_HOST__PASSWORD);

          



        conn_sm_.mqtt_started = true;
      }

      _conn_enter(conn_sm_, cellular_conn_state_t::ONLINE, now_ms);
      conn_sm_.t_next_action_ms = 0;
      ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: ONLINE (GPRS + MQTT init issued)"));
    }
    break;

    case cellular_conn_state_t::BACKOFF:
    {
      // Simple exponential-ish backoff (bounded)
      const uint32_t backoff_ms = (uint32_t)min<uint32_t>(60000UL, 5000UL * (uint32_t)max<uint8_t>(1, conn_sm_.attempts));
      ALOG_WRN(PSTR(D_LOG_CELLULAR "CELL: BACKOFF %lu ms (attempts=%u)"), (unsigned long)backoff_ms, conn_sm_.attempts);

      // After backoff, retry from START_CONNECTION (don’t redo init config every time)
      _conn_enter(conn_sm_, cellular_conn_state_t::START_CONNECTION, now_ms);
      conn_sm_.t_next_action_ms = now_ms + backoff_ms;
    }
    break;
  }
}




/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mCellular::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok2 = 0; 
  int8_t tmp_id = 0;

  
    
}










  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mCellular::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
  
    
  return JBI->End();

}


uint8_t mCellular::ConstructJSON_State(uint8_t json_level, bool json_appending){

  char buffer[100] = {0};

  #ifdef USE_MODULE_DRIVERS_MODEM_7000G
  mSIM7000G::GPRS_STATUS gprs = tkr_sim7000g->gprs;
  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  mSIM7000G::GPS_STATUS gps = tkr_sim7000g->gps;
  #endif
  mSIM7000G::DATA modem_status = tkr_sim7000g->modem_status;
  // #endif // USE_MODULE_DRIVERS_MODEM_7000G
  // #ifdef USE_MODULE_DRIVERS_MODEM_7000G
  // mSIM7000G::GPRS_STATUS gprs = tkr_sim7000g->gprs;
  // mSIM7000G::GPS_STATUS gps = tkr_sim7000g->gps;
  // mSIM7000G::DATA modem_status = tkr_sim7000g->modem_status;
  // #endif // USE_MODULE_DRIVERS_MODEM_7000G

  JBI->Start();

    JBI->Add("uptime", tkr_time->uptime_seconds_nonreset);
    JBI->Add("rtcseconds", tkr_time->RtcTime.second);
    JBI->Object_Start("GPRS");
        JBI->Add("ConSec", gprs.connected_seconds);
        JBI->Add("rss_dbm", gprs.signal_quality_rssi_dbm);
    JBI->Object_End();
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Object_Start("GPS");
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
    JBI->Object_End();
    #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
    JBI->Object_Start("Battery");
      JBI->Add("Volts_mv", modem_status.battery.volts_mv);
      JBI->Add("Percentage", modem_status.battery.percentage);
      JBI->Add("ChargeState", modem_status.battery.charge_state);
      JBI->Add("Valid", modem_status.battery.isvalid);
    JBI->Object_End();

    #endif
  
  return JBI->End();
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mCellular::MQTTHandler_Init()
{

  struct handler<mCellular>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 60; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mCellular::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mCellular::MQTTHandler_Rate()
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
void mCellular::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    tkr_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_NETWORK_CELLULAR