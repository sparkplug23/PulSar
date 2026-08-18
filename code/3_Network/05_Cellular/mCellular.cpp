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
 * 
 * SIM7000G driver knows the hardware; Cellular makes it behave like a network transport; NetworkInterface decides how that transport is used.
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

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

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
      // 1Hz policy loop: keep cellular data network online.
      // MQTT must bind to this transport externally.
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
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;

} // END function


void mCellular::Pre_Init(void)
{
  
  module_state.mode = ModuleStatus::Initialising;
  
}

void mCellular::Init(void)
{
  Cellular_ConnMgr_Reset();
  
  module_state.mode = ModuleStatus::Running;
}

bool mCellular::Cellular_HasExternalConnectivity(void)
{
  if (module_state.mode != ModuleStatus::Running) { return false; }
  if (!tkr_modem) { return false; }

  if (conn_sm_.state != cellular_conn_state_t::ONLINE) { return false; }

  if (!tkr_modem->IsReady()) { return false; }
  if (!tkr_modem->DataNetwork_IsConnected()) { return false; }

  return true;
}


Client* mCellular::Cellular_GetNetworkClient(void)
{
  if (!Cellular_HasExternalConnectivity()) { return nullptr; }

  return tkr_modem->DataNetwork_GetOrCreateClient(false);
}

void mCellular::Cellular_ConnMgr_Reset()
{
  conn_sm_.state = cellular_conn_state_t::WAIT_MODEM_READY;
  conn_sm_.t_enter_ms = millis();
  conn_sm_.t_next_action_ms = 0;

  conn_sm_.attempts = 0;
  conn_sm_.init_config_done = false;
  
  conn_sm_.gprs_drop_count = 0;

  conn_sm_.last_gprs_connected = false;
}

void mCellular::Cellular_ConnMgr_Tick_1s(uint32_t now_ms)
{
  if (!tkr_modem) { return; }

  if (!tkr_modem->IsReady())
  {
    if (conn_sm_.state != cellular_conn_state_t::WAIT_MODEM_READY)
    {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "CELL: Modem not ready, returning to WAIT_MODEM_READY"));
      Cellular_ConnMgr_Reset();
    }

    return;
  }

  const bool gprs_connected = tkr_modem->DataNetwork_IsConnected();

  ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: gprs_connected=%d state=%u"), gprs_connected, conn_sm_.state);

  // ------------------------------------------------------------------
  // If GPRS is already connected, promote immediately.
  // This must happen before the t_next_action_ms rate-limit, otherwise
  // MQTT may see GPRS connected while cellular remains in START_CONNECTION.
  // ------------------------------------------------------------------
  if (conn_sm_.state == cellular_conn_state_t::START_CONNECTION && gprs_connected)
  {
    conn_sm_.attempts = 0;
    conn_sm_.gprs_drop_count = 0;

    _conn_enter(conn_sm_, cellular_conn_state_t::ONLINE, now_ms);
    conn_sm_.t_next_action_ms = 0;

    ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: ONLINE (GPRS connected)"));
    return;
  }

  // ------------------------------------------------------------------
  // ONLINE supervision.
  // Do not treat a single false GPRS poll as a real link drop.
  // SIM7000/TinyGSM status reads can occasionally report false negatives,
  // especially while other AT/socket activity is happening.
  // ------------------------------------------------------------------
  if (conn_sm_.state == cellular_conn_state_t::ONLINE)
  {
    if (!gprs_connected)
    {
      conn_sm_.gprs_drop_count++;

      ALOG_WRN(
        PSTR(D_LOG_CELLULAR "CELL: GPRS missing sample %u"),
        conn_sm_.gprs_drop_count
      );

      if (conn_sm_.gprs_drop_count >= 3)
      {
        ALOG_WRN(PSTR(D_LOG_CELLULAR "CELL: GPRS confirmed dropped, entering BACKOFF"));

        conn_sm_.attempts++;
        conn_sm_.gprs_drop_count = 0;

        _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 5000;
      }

      return;
    }

    conn_sm_.gprs_drop_count = 0;
    conn_sm_.t_next_action_ms = 0;
    return;
  }

  // ------------------------------------------------------------------
  // Rate-limit expensive state actions.
  // ------------------------------------------------------------------
  if (conn_sm_.t_next_action_ms && (int32_t)(now_ms - conn_sm_.t_next_action_ms) < 0)
  {
    return;
  }

  switch (conn_sm_.state)
  {
    default:
    case cellular_conn_state_t::WAIT_MODEM_READY:
    {
      conn_sm_.attempts = 0;
      conn_sm_.init_config_done = false;
      conn_sm_.gprs_drop_count = 0;

      _conn_enter(conn_sm_, cellular_conn_state_t::INIT_CONFIG, now_ms);
      conn_sm_.t_next_action_ms = now_ms;
    }
    break;

    case cellular_conn_state_t::INIT_CONFIG:
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: DataNetwork__InitConfig attempt=%u"), conn_sm_.attempts + 1);

      const bool ok = tkr_modem->DataNetwork__InitConfig();
      conn_sm_.init_config_done = ok;

      if (ok)
      {
        _conn_enter(conn_sm_, cellular_conn_state_t::START_CONNECTION, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 1000;
      }
      else
      {
        conn_sm_.attempts++;
        _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 5000;
      }
    }
    break;

    case cellular_conn_state_t::START_CONNECTION:
    {
      if (gprs_connected)
      {
        conn_sm_.attempts = 0;
        conn_sm_.gprs_drop_count = 0;

        _conn_enter(conn_sm_, cellular_conn_state_t::ONLINE, now_ms);
        conn_sm_.t_next_action_ms = 0;

        ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: ONLINE (GPRS connected)"));
        break;
      }

      ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: DataNetwork__StartConnection attempt=%u"), conn_sm_.attempts + 1);

      const bool ok = tkr_modem->DataNetwork__StartConnection();

      if (ok && tkr_modem->DataNetwork_IsConnected())
      {
        conn_sm_.attempts = 0;
        conn_sm_.gprs_drop_count = 0;

        _conn_enter(conn_sm_, cellular_conn_state_t::ONLINE, now_ms);
        conn_sm_.t_next_action_ms = 0;

        ALOG_INF(PSTR(D_LOG_CELLULAR "CELL: ONLINE (GPRS connected)"));
      }
      else
      {
        conn_sm_.attempts++;
        _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
        conn_sm_.t_next_action_ms = now_ms + 5000;
      }
    }
    break;

    case cellular_conn_state_t::ONLINE:
    {
      // Normally handled by the early ONLINE branch above.
      // Kept for switch completeness.
      if (!gprs_connected)
      {
        conn_sm_.gprs_drop_count++;

        ALOG_WRN(
          PSTR(D_LOG_CELLULAR "CELL: GPRS missing sample %u"),
          conn_sm_.gprs_drop_count
        );

        if (conn_sm_.gprs_drop_count >= 3)
        {
          ALOG_WRN(PSTR(D_LOG_CELLULAR "CELL: GPRS confirmed dropped, entering BACKOFF"));

          conn_sm_.attempts++;
          conn_sm_.gprs_drop_count = 0;

          _conn_enter(conn_sm_, cellular_conn_state_t::BACKOFF, now_ms);
          conn_sm_.t_next_action_ms = now_ms + 5000;
        }
      }
      else
      {
        conn_sm_.gprs_drop_count = 0;
        conn_sm_.t_next_action_ms = 0;
      }
    }
    break;

    case cellular_conn_state_t::BACKOFF:
    {
      const uint32_t backoff_ms = (uint32_t)min<uint32_t>(
        60000UL,
        5000UL * (uint32_t)max<uint8_t>(1, conn_sm_.attempts)
      );

      ALOG_WRN(
        PSTR(D_LOG_CELLULAR "CELL: BACKOFF %lu ms attempts=%u"),
        (unsigned long)backoff_ms,
        conn_sm_.attempts
      );

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

// This should be deleted


  char buffer[100] = {0};

  #ifdef USE_MODULE_DRIVERS_MODEM_7000G
  mSIM7000G::GPRS_STATUS gprs = tkr_modem_sim7000g->gprs;
  mSIM7000G::DATA modem_status = tkr_modem_sim7000g->modem_status;

  JBI->Start();

    JBI->Add("uptime", tkr_time->uptime_seconds_nonreset);
    JBI->Add("rtcseconds", tkr_time->RtcTime.second);
    JBI->Object_Start("GPRS");
        JBI->Add("ConSec", gprs.connected_seconds);
        JBI->Add("rss_dbm", gprs.signal_quality_rssi_dbm);
    JBI->Object_End();
    JBI->Object_Start("Battery");
      JBI->Add("Volts_mv", modem_status.battery.volts_mv);
      JBI->Add("Percentage", modem_status.battery.percentage);
      JBI->Add("ChargeState", modem_status.battery.charge_state);
      JBI->Add("Valid", modem_status.battery.isvalid);
    JBI->Object_End();

    #endif // USE_MODULE_DRIVERS_MODEM_7000G
  
  return JBI->End();
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mCellular::Telemetry_Init()
{

  struct telemetry_handler<mCellular>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true; // DEBUG CHANGE
  ptr->tRateSecs = 60; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_IFCHANGED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mCellular::ConstructJSON_State;
  telemetry_list.push_back(ptr);

} 

#endif // USE_MODULE_NETWORK_MQTT


#endif // USE_MODULE_NETWORK_CELLULAR