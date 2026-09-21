#include "_mSIM7000G.h"

/***
 
 To become the primary controller for the modem. It should maintain connection, reconnect, keep the modem fucntioning. 

 The network module can therefore just "attach" itself to this function with callbacks.


 * */


#ifdef USE_MODULE_DRIVERS_MODEM_7000G


bool mSIM7000G::Handler_ModemResponses(uint8_t response_loglevel, uint16_t wait_millis)
{
  // Persistent RX assembly buffer (per modem instance)
  static char     rx_line[300];
  static uint16_t rx_len = 0;

  bool handled_any = false;

  const uint32_t t0 = millis();

  // Drain for up to wait_millis (0 means "just what's available right now")
  while (modem_serial->available() || ((wait_millis > 0) && ((millis() - t0) < wait_millis)))
  {
    if (!modem_serial->available())
    {
      // nothing right now, don’t burn CPU
      delay(1);
      continue;
    }

    const int c = modem_serial->read();
    if (c < 0) continue;

    // Drop CR (we’ll use LF as end-of-line)
    if (c == '\r') continue;

    // End-of-line: dispatch one complete line
    if (c == '\n')
    {
      if (rx_len == 0) continue; // ignore blank lines

      rx_line[rx_len] = '\0';

      // AddLog(response_loglevel, PSTR(D_LOG_CELLULAR "Handler_ModemResponses %d>> Line[%d] \"%s\""), response_loglevel, rx_len, rx_line);

      // parse_ATCommands() may mutate buffer (strtok), so pass a writable copy
      // We can pass rx_line directly because it is writable and we reset after.
      handled_any |= parse_ATCommands(rx_line, rx_len, response_loglevel);

      rx_len = 0;
      continue;
    }

    // Normal character: append if space
    if (rx_len < (sizeof(rx_line) - 1))
    {
      rx_line[rx_len++] = (char)c;
    }
    else
    {
      // Overflow: flush what we have as a line to avoid permanent wedge
      rx_line[sizeof(rx_line) - 1] = '\0';
      ALOG_ERR(PSTR(D_LOG_CELLULAR "RX line overflow, dropping tail: \"%s\""), rx_line);
      rx_len = 0;
    }
  }

  return handled_any;
}



//-------------------------------------------------------------------------------
// Modem bring-up (baseline AT-ready) - non-blocking state machine
// -------------------------------------------------------------------------------------------------


void mSIM7000G::ModemInit_ForceRestart()
{
  modem_sm_.restart_requested = true;
  ALOG_DBG(PSTR(D_LOG_CELLULAR "ModemInit_ForceRestart requested"));
}

bool mSIM7000G::ModemInit_IsReady() const
{
  return modem_sm_.ready;
}

bool mSIM7000G::ModemInit_IsBusy() const
{
  return modem_sm_.busy;
}

const mSIM7000G::modem_probe_t& mSIM7000G::ModemInit_GetProbe() const
{
  return modem_probe_;
}

// // ---- tick -------------------------------------------------------------------

void mSIM7000G::ModemInit_Tick(uint32_t now_ms)
{
  if (!modem_sm_.restart_requested)
  {
    return;
  }

  modem_sm_.restart_requested = false;

  ALOG_WRN(PSTR(D_LOG_CELLULAR "ModemInit_Tick: restart requested"));

  flag_modem_initialized = false;
  modem_sm_.busy = true;
  modem_sm_.ready = false;
  modem_sm_.probe_done = false;

  const bool ok = Modem_EnsurePowerOnAndBaud(MODEM_TARGET_BAUD);

  if (!ok)
  {
    modem_sm_.busy  = false;
    modem_sm_.ready = false;
    modem_sm_.state = modem_init_state_t::FAILED;

    ALOG_ERR(PSTR(D_LOG_CELLULAR "Modem restart failed"));
    return;
  }

  ModemInit_ProbeFillBasic();

  modem_sm_.state = modem_init_state_t::READY;
  modem_sm_.busy  = false;
  modem_sm_.ready = true;

  flag_modem_initialized = true;

  ALOG_DBG(PSTR(D_LOG_CELLULAR "Modem restart complete"));
}

// TinyGsmClient* mSIM7000G::DataNetwork_GetOrCreateClient(bool force_recreate)
// {
//   if (!modem) return nullptr;

//   if (force_recreate && gsm_client) {
//     gsm_client->stop();
//     delete gsm_client;
//     gsm_client = nullptr;
//   }

//   if (!gsm_client) {
//     gsm_client = new TinyGsmClient(*modem);
//   }

//   return gsm_client;
// }
TinyGsmClient* mSIM7000G::DataNetwork_GetOrCreateClient(bool force_recreate)
{
  if(!modem) return nullptr;

  if(!gsm_client)
  {
    gsm_client = new TinyGsmClient(*modem);

    if(!gsm_client)
    {
      ALOG_ERR(PSTR(D_LOG_CELLULAR "Failed to allocate TinyGsmClient"));
      return nullptr;
    }

    ALOG_INF(PSTR(D_LOG_CELLULAR "TinyGsmClient created ptr=%p"),gsm_client);
  }
  else if(force_recreate)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "TinyGsmClient recreate requested but persistent client retained ptr=%p"),gsm_client);

    gsm_client->stop();
  }

  return gsm_client;
}

void mSIM7000G::DataNetwork_LogDiagnostics(void)
{
  if(!modem)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "DIAG30 MODEM modem=NULL sm=%u ready=%u busy=%u restart=%u"),
      (unsigned)modem_sm_.state,
      modem_sm_.ready,
      modem_sm_.busy,
      modem_sm_.restart_requested
    );
    return;
  }

  const bool ready = ModemInit_IsReady();
  const bool at_ok = modem->testAT(200);

  if(!at_ok)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "DIAG30 MODEM sm=%u ready=%u busy=%u restart=%u AT=FAIL client=%p"),
      (unsigned)modem_sm_.state,
      ready,
      modem_sm_.busy,
      modem_sm_.restart_requested,
      gsm_client
    );
    return;
  }

  const int16_t csq = modem->getSignalQuality();
  const RegStatus reg = modem->getRegistrationStatus();
  const bool network_connected = modem->isNetworkConnected();
  const bool gprs_connected = modem->isGprsConnected();

  ALOG_INF(PSTR(D_LOG_CELLULAR "DIAG30 MODEM sm=%u ready=%u busy=%u AT=OK CSQ=%d RSSI=%d REG=%d NET=%u GPRS=%u client=%p"),
    (unsigned)modem_sm_.state,
    ready,
    modem_sm_.busy,
    (int)csq,
    (int)GetSignalQualityPower(csq),
    (int)reg,
    network_connected,
    gprs_connected,
    gsm_client
  );
}


void mSIM7000G::ModemInit_SM_Enter(mSIM7000G::modem_init_sm_t& sm,
                                  mSIM7000G::modem_init_state_t st,
                                  uint32_t now_ms)
{
  sm.state      = st;
  sm.t_enter_ms = now_ms;
}

bool mSIM7000G::ModemInit_ATSyncTry()
{
  if (!modem) return false;
  return modem->testAT(mSIM7000G::AT_SYNC_STEP_TIMEOUT_MS);
}

void mSIM7000G::ModemInit_ProbeFillBasic()
{
  if (!modem) return;

  modem_probe_.csq   = modem->getSignalQuality();
  modem_probe_.at_ok = true;

  #if defined(TINY_GSM_MODEM_SIM7000) || defined(TINY_GSM_MODEM_SIM7070) || defined(TINY_GSM_MODEM_SIM7080)
    auto st = modem->getSimStatus();
    modem_probe_.sim_present = (st != SIM_ERROR);
    modem_probe_.sim_ready   = (st == SIM_READY);
  #else
    modem_probe_.sim_present = false;
    modem_probe_.sim_ready   = false;
  #endif

  String m = modem->getModemName();
  if (!m.length()) m = modem->getModemInfo();
  if (m.length()) {
    m.toCharArray(modem_probe_.model, sizeof(modem_probe_.model));
  }
}


/**
 * @brief Query modem liveness (AT responsive) and keep lightweight health stats.
 *        Intended to be called from TASK_EVERY_SECOND.
 *
 * Behaviour:
 *  - Sends a short AT probe (TinyGSM testAT()) with a small timeout.
 *  - Logs only on state change (alive <-> dead) to avoid spam.
 *  - Tracks consecutive failures and triggers a restart request if it looks wedged.
 *  - Optionally refreshes CSQ/model less frequently (non-critical).
 *
 * @return true if modem responded to AT in this tick, else false
 */
// -------------------------------------------------------------------------------------------------
// Probe modem health once per second (non-blocking-ish: short timeouts only)
// Call from: case TASK_EVERY_SECOND: Query_Modem_Status(); break;
// -------------------------------------------------------------------------------------------------
void mSIM7000G::Query_Modem_Status()
{
  // --------------------------------------------------------------------------
  // Guards
  // --------------------------------------------------------------------------
  if (!modem) {
    ALOG_DBG(PSTR(D_LOG_CELLULAR "Query_Modem_Status: modem=null"));
    return;
  }

  // Only probe when init is ready (or if you explicitly want probing during init)
  if (!ModemInit_IsReady()) {
    // If you want visibility during init, change this to ALOG_DBM and keep going.
    ALOG_DBM(PSTR(D_LOG_CELLULAR "Query_Modem_Status: init not ready (state=%u)"),
             (unsigned)modem_sm_.state);
    return;
  }

  // Throttle (in case TASK_EVERY_SECOND is jittery / called more than expected)
  static uint32_t t_last_ms = 0;
  const uint32_t now_ms = millis();
  if ((now_ms - t_last_ms) < 950) return;
  t_last_ms = now_ms;

  // --------------------------------------------------------------------------
  // 1) Basic AT-alive check (fast)
  // --------------------------------------------------------------------------
  const uint16_t t_at_ms = 150;
  const bool at_ok = modem->testAT(t_at_ms);

  if (!at_ok) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "AT probe FAILED (%ums)"), t_at_ms);

    // Optional: escalate if repeated failures
    static uint8_t fail_count = 0;
    fail_count++;
    if (fail_count >= 5) {
      ALOG_DBG(PSTR(D_LOG_CELLULAR "AT failed %u times -> ModemInit_ForceRestart()"), fail_count);
      fail_count = 0;
      ModemInit_ForceRestart();
    }
    return;
  }

  // Reset failure counter on success
  {
    static uint8_t fail_count = 0;
    fail_count = 0;
  }

  // --------------------------------------------------------------------------
  // 2) Flush any stray unsolicited text so subsequent queries are clean
  // --------------------------------------------------------------------------
  while (modem_serial->available()) { (void)modem_serial->read(); }

  // --------------------------------------------------------------------------
  // 3) SIM presence / readiness via AT+CPIN? (most reliable quick check)
  // --------------------------------------------------------------------------
  bool sim_ready = false;
  bool sim_present = false; // "present" in practice == CPIN returns something meaningful
  {
    String r;
    modem->sendAT("+CPIN?");
    const int8_t rc = modem->waitResponse(400, r); // short timeout
    if (rc == 1) {
      // Typical:
      // +CPIN: READY
      // OK
      // or: +CPIN: NOT INSERTED
      // or: +CPIN: SIM PIN
      if (r.indexOf(F("+CPIN:")) >= 0) {
        sim_present = true;
        if (r.indexOf(F("READY")) >= 0) sim_ready = true;
      }
    }
  }

  // --------------------------------------------------------------------------
  // 4) Optional SIM ICCID (only if CPIN says SIM present; keep short)
  // --------------------------------------------------------------------------
  char ccid[32] = {0};
  if (sim_present && sim_ready) {
    String r;
    modem->sendAT("+CCID");
    const int8_t rc = modem->waitResponse(500, r);
    if (rc == 1) {
      // Response is typically the ICCID line + OK. Pull digits.
      // Keep it simple: copy digits only.
      uint8_t j = 0;
      for (uint16_t i = 0; i < r.length() && j < (sizeof(ccid)-1); i++) {
        const char c = r.charAt(i);
        if (c >= '0' && c <= '9') ccid[j++] = c;
      }
      ccid[j] = '\0';
    }
  }

  // --------------------------------------------------------------------------
  // 5) Signal + registration (signal can be 99 if no RF; that’s separate from SIM)
  // --------------------------------------------------------------------------
  const int16_t csq = modem->getSignalQuality();          // 0..31, 99 unknown
  const float   rssi_dbm = GetSignalQualityPower(csq);    // your mapper

  // Registration checks can be slow; keep them optional / short.
  // If TinyGSM getRegistrationStatus() is fast enough on your build, use it.
  RegStatus reg = REG_UNKNOWN;
  {
    // NOTE: TinyGSM’s getRegistrationStatus() usually uses AT+CGREG?/CEREG?
    // If this ever blocks too long, delete this block.
    reg = modem->getRegistrationStatus();
  }

  const bool network_connected = modem->isNetworkConnected();
  const bool gprs_connected    = modem->isGprsConnected();

  // --------------------------------------------------------------------------
  // 6) One-line status log (what you actually want in TASK_EVERY_SECOND)
  // --------------------------------------------------------------------------
  // static uint32_t t_last_status_log_ms = 0;
  // const bool force_log = (rssi_dbm <= -100.0f);

  // if (!force_log && (now_ms - t_last_status_log_ms) < 10000UL)
  // {
  //   return;
  // }

  // t_last_status_log_ms = now_ms;

  // if (!sim_present) {
  //   ALOG_DBG(PSTR(D_LOG_CELLULAR "AT OK, SIM: no response (CPIN?) , CSQ=%d (%.1f dBm), REG=%d"),
  //            (int)csq, (double)rssi_dbm, (int)reg);
  // } else if (!sim_ready) {
  //   ALOG_DBG(PSTR(D_LOG_CELLULAR "AT OK, SIM: present NOT READY, CSQ=%d (%.1f dBm), REG=%d"),
  //            (int)csq, (double)rssi_dbm, (int)reg);
  // } else {
  //   if (ccid[0] != '\0') {
  //     ALOG_DBG(PSTR(D_LOG_CELLULAR "SIM READY, CCID=%s, CSQ=%d (%.1f dBm), REG=%d"),
  //              ccid, (int)csq, (double)rssi_dbm, (int)reg);
  //   } else {
  //     ALOG_DBG(PSTR(D_LOG_CELLULAR "AT OK, SIM READY, CSQ=%d (%.1f dBm), REG=%d"),
  //              (int)csq, (double)rssi_dbm, (int)reg);
  //   }
  // }

  ALOG_DBG(PSTR(D_LOG_CELLULAR "SIM=%s CSQ=%d RSSI=%.1f dBm REG=%d NET=%u GPRS=%u"),
    sim_ready ? "READY" : (sim_present ? "NOT_READY" : "NO_RESPONSE"),
    (int)csq,
    (double)rssi_dbm,
    (int)reg,
    network_connected,
    gprs_connected
  );

  if(gprs_connected)
  {
    digitalWrite(12, !digitalRead(12));
  }else{
    digitalWrite(12, HIGH); // turn off, its active low
  }


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
void mSIM7000G::SendATCommand_FunctionalityMode_Minimum()
{
  modem->sendAT("+CFUN=0"); 
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN);
  ALOG_DBG(PSTR(D_LOG_CELLULAR "FunctionalityMode_Minimum %d"), result);
}
void mSIM7000G::SendATCommand_FunctionalityMode_Full()
{
  modem->sendAT("+CFUN=1");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN); 
  ALOG_DBG(PSTR(D_LOG_CELLULAR "FunctionalityMode_Full %d"), result);
}


void mSIM7000G::modemPowerOn()
{
  ALOG_WRN(PSTR(D_LOG_CELLULAR "modemPowerOn wrapper -> PWRKEY pulse"));
  Modem_PulsePowerKey(MODEM_PWRKEY_HOLD_MS);
  flag_modem_initialized = false;
}

void mSIM7000G::modemPowerOff()
{
  ALOG_WRN(PSTR(D_LOG_CELLULAR "modemPowerOff wrapper -> PWRKEY pulse"));
  Modem_PulsePowerKey(MODEM_PWRKEY_HOLD_MS);
  flag_modem_initialized = false;
}

void mSIM7000G::modemRestart()
{
  WDT_Reset();

  ALOG_WRN(PSTR(D_LOG_CELLULAR "modemRestart wrapper -> force re-sequence"));

  flag_modem_initialized = false;
  modem_sm_.ready = false;
  modem_sm_.busy = true;
  modem_sm_.restart_requested = true;
}


// ==============================================================================================




bool mSIM7000G::Modem_CheckAndRestartUnresponsiveModem()
{
  
  uint16_t timeout_wait_for_AT_response = 500;

  if(modem->testAT(timeout_wait_for_AT_response))
  {
    ALOG_DBG(PSTR(D_LOG_CELLULAR "Modem Running"));
    return true;
  }else{
    ALOG_DBG(PSTR(D_LOG_CELLULAR "Modem Not responding, restarting (TBC: causes 7.5 second pause)"));
    modemRestart(); // If no response, cause reboot and do something else
    return false;
  }

}




float mSIM7000G::GetSignalQualityPower()
{
  return GetSignalQualityPower(modem->getSignalQuality());
}

/**
 * @brief Conversion without calling modem again
 * 
 * @param signal_quality_raw 
 * @return float 
 */
float mSIM7000G::GetSignalQualityPower(int16_t signal_quality_raw)
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



void mSIM7000G::Get_Modem_Hardware()
{

  String name = modem->getModemName();
  ALOG_DBG(PSTR(D_LOG_CELLULAR "Modem Name: %s"),name.c_str());

  String modemInfo = modem->getModemInfo();
  ALOG_DBG(PSTR(D_LOG_CELLULAR "Modem Info: %s"),modemInfo.c_str());

}




/**
 * @brief Mostly to debug, but also return if it worked
 * 
 * @param buffer 
 * @return true 
 * @return false 
 */
bool mSIM7000G::SendAT(const char* buffer, uint16_t wait_millis)
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

  ALOG_DBG(PSTR(D_LOG_CELLULAR ">>>%s %s (%dms)"), buffer, result?"Success":"FAILED", tSaved_Elapsed);

  return result;
  
}


bool mSIM7000G::SendAT_F(uint16_t wait_millis, PGM_P formatP, ...)
{

  char command_buffer[200];

  va_list arg;
  va_start(arg, formatP);
  vsnprintf_P(command_buffer, sizeof(command_buffer), formatP, arg);
  va_end(arg);

  return SendAT(command_buffer, wait_millis);

}


bool mSIM7000G::SendAT_ATParseResponse_F(uint16_t wait_millis, uint8_t response_loglevel, PGM_P formatP, ...)
{

  AddLog(response_loglevel, PSTR("response_loglevel=%d"),response_loglevel);

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

  ALOG_ERR(PSTR(D_LOG_CELLULAR "========================================Response (%dms)"), millis()-tSaved_millis);
  
  return true;

}






/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

void mSIM7000G::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  JsonParserToken jtok2 = 0; 
  int8_t tmp_id = 0;

  
  if(jtok = obj["OpenHAB"].getObject()["SystemTime"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "OpenHAB:SystemTime"));
    ALOG_DBG(PSTR(D_LOG_CELLULAR "OH Time %s"),jtok.getStr());
    //toggle led
    DIGITAL_INVERT_PIN(12); //esp32 blue led
  }


  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if (jtok = obj["GPS_Enable"])
  {
    const bool enable = (jtok.getInt() == 1);

    ALOG_DBG(PSTR(D_LOG_CELLULAR "GPS_Enable desired=%d"), enable);

    gps.enabled = enable;

    if (enable)
    {
      gps.configured = false; // Force GNSS_Update() to configure cleanly
    }
    else
    {
      GNSS_Disable();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  if(jtok = obj["SMSAuto_GPS"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "SMSAuto_GPS smsauto_gps_messages.rate_seconds %d"), jtok.getInt());
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS 
    smsauto_gps_messages.rate_seconds = jtok.getInt();
    #endif
  }


  if(jtok = obj["GPRS_Enable"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "GPRS_Enable"));
    if(jtok.getInt() == 1)
    {
      DataNetwork__InitConfig();
    }
  }


  #ifdef ENABLE_FEATURE_SIM__SMS
  if(jtok = obj["SMS_Enable"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "SMS_Enable"));
    if(jtok.getInt() == 1)
    {
      SMS_Enable();
    }
  }
  #endif


  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["SMS_GPSLocation"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "SMS_GPSLocation"));
    if(jtok.getInt() == 1)
    {
      SMS_GPSLocation();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  #ifdef ENABLE_FEATURE_SIM__SMS
  if(jtok = obj["SMSForward"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "SMSForward"));
    if(jtok.getInt() == 1)
    {
      SendATCommand_SMSImmediateForwardOverSerial();
    }
  }
  #endif

  
  if(jtok = obj["BattVolt"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "BattVolt"));
    if(jtok.getInt() == 1)
    {    
      // read the battery voltage and percentage
      uint16_t vbat = modem->getBattVoltage();        
      ALOG_DBG(PSTR(D_LOG_CELLULAR "%d mV"), vbat);
    }
  }

  if(jtok = obj["ModemInit"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "ModemInit"));
    flag_modem_initialized = jtok.getInt();
  }

  if(jtok = obj["ADCVoltage"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "ADCVoltage"));
    if(jtok.getInt() == 1)
    {      
      ModemUpdate_BatteryStatus();
      ALOG_DBG(PSTR(D_LOG_CELLULAR "{\"volts_mv\":%d,\"percent\":%d,\"milliVolts\":%d}"),
        modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);
    }
  }


  if(jtok = obj["ReadSMS"]){

    ALOG_DBG( PSTR(D_LOG_CELLULAR "ReadSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 

      modem->sendAT("+CMGR=0");

      //parser right here

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_DBG(PSTR(D_LOG_CELLULAR "ReadSMS %d"), result);
      

    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }
  
  
  if(jtok = obj["ATDebugCommand"])
  {

    if(jtok2 = jtok.getObject()["SMSReadOnly"])
    {
      ALOG_DBG( PSTR(D_LOG_CELLULAR "SMSReadOnly"));

      if(jtok2.isStr()) //Assumed worded group type
      {        
        SendAT_F(100, PSTR("+CMGR=\"%s\""), jtok2.getStr());
      }
      if(jtok2.isNum()) // Assumed index (From 0-max)
      {        
        SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), jtok2.getInt());
      }
    }

    if(jtok2 = jtok.getObject()["SMSReadAndClear"])
    {
      ALOG_DBG( PSTR(D_LOG_CELLULAR "SMSReadAndClear"));

      if(jtok2.isNum()) // Assumed index (From 0-max)
      {        
        if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGR=%d"), jtok2.getInt()))
        {
          // Clear it if it was successfully read
          SendAT_F(100, PSTR("+CMGD=%d"), jtok2.getInt());
        }
      }
    }


    if(jtok2 = jtok.getObject()["SMSReadAndClearAll"])
    {
      ALOG_DBG( PSTR(D_LOG_CELLULAR "SMSReadAndClearAll"));
      
      if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGD=?")))
      {
        /**
         * @brief Placing in here now
         *  */
        // if(sms.messages_incoming_index_list.size())
        // {
        //   SMSReadAndEraseSavedSMS();
        // } 

      }
      
    }


    if(jtok2 = jtok.getObject()["SMSCheckWaitingIndexs"])
    {
      ALOG_DBG( PSTR(D_LOG_CELLULAR "SMSCheckWaitingIndexs"));
      
      if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGD=?")))
      {
        // Clear it if it was successfully read
        // SendAT_F(100, PSTR("+CMGD=%d"), jtok2.getInt());
      }
      
    }


    if(jtok2 = jtok.getObject()["SMSClearAll"])
    {
      ALOG_DBG( PSTR(D_LOG_CELLULAR "SMSClearAll"));
      uint8_t flag = 4; // delete all
      SendAT_F(100, PSTR("+CMGR=%d[,%d]"), jtok2.getInt(), flag);
    }

  }

  if(jtok = obj["ReadDeleteSMS"]){

    ALOG_DBG( PSTR(D_LOG_CELLULAR "ReadDeleteSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


      // SendAT("+CMGR=0[,4]", 100);

      // uint8_t flag = 4; // delete all

      // SendAT_F(PSTR("+CMGR=%d[,%d]"), index, flag);

      modem->sendAT("+CMGR=0[,4]");

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);      

      modem->sendAT("+CMGD=0");
      
      result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_DBG(PSTR(D_LOG_CELLULAR "ReadDeleteSMS %d"), result);
    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }


  if(jtok = obj["ATCommands"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "ATCommands"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      ALOG_DBG(PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      modem_serial->println(val.getStr());  
      delay(500);
      if (modem_serial->available()) {
        String r = modem_serial->readString();
        ALOG_DBG(PSTR(D_LOG_CELLULAR "ATResponse = \"%s\""), r.c_str());
      }
    }

  }

  if(jtok = obj["ATCommandsParsing"]){
    ALOG_DBG( PSTR(D_LOG_CELLULAR "ATCommandsParsing"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      ALOG_DBG(PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      modem_serial->println(val.getStr());  
      delay(500);

      uint32_t wait_millis = 1000;
      
      uint32_t tSaved_millis = millis();
      uint32_t tSaved_Elapsed = millis()-tSaved_millis;
      while(!Handler_ModemResponses(LOG_LEVEL_DEBUG))
      {
        if(
          (tSaved_Elapsed = (millis()-tSaved_millis)) > wait_millis
        ){
          ALOG_ERR(PSTR(D_LOG_CELLULAR "No response (%dms)"), tSaved_Elapsed);
        }

      }

    }

  }

    
}


bool mSIM7000G::parse_ATCommands(char* buffer, uint16_t buflen, uint8_t response_loglevel)
{
  if (!buffer || buflen == 0) return false;
  if (buffer[0] == '\0') return false;

  // ---- OPTIONAL: suppress OK spam but keep a flag ----
  if (ATParse__OK(buffer, buflen, response_loglevel))
  { // handled "OK"
    // ALOG_DBG(PSTR("Suppress OK response"));
    return true;
  }

  // Log everything else (or gate by level if you want)
  ALOG_DBG(PSTR(D_LOG_SIM7000G "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\rparse_ATCommands %d>> ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\rBuffer[%d] \"%s\""), response_loglevel, buflen, buffer);

  // 0) If we are waiting for the body line (after +CMGR or +CMT), consume it first.
  //    This must be first so "hello123" does not fall through as an unknown line.
  #ifdef ENABLE_FEATURE_SIM__SMS
  if (sms2_waiting_body)
  {
    return ATParse__BodyLine(buffer, buflen, response_loglevel);
  }
  #endif

  #ifdef ENABLE_FEATURE_SIM__SMS
  // 1) Stored message notification (CNMI mt=1): queue idx
  if (strstr(buffer, "+CMTI:")) return ATParse__CMTI(buffer, buflen, response_loglevel);

  // 2) Read message header (from AT+CMGR=<idx>): arms body consumption
  if (strstr(buffer, "+CMGR:")) return ATParse__CMGR_Header(buffer, buflen, response_loglevel);

  // 3) Immediate message delivery (CNMI mt=2): arms body consumption
  if (strstr(buffer, "+CMT:"))  return ATParse__CMT_Header(buffer, buflen, response_loglevel);

  // 4) List messages (from AT+CMGL=...): queue indices
  if (strstr(buffer, "+CMGL:")) return ATParse__CMGL(buffer, buflen, response_loglevel);

  // 5) Storage status (from AT+CPMS? or +CPMS=...)
  if (strstr(buffer, "+CPMS:")) return ATParse__CPMS(buffer, buflen, response_loglevel);

  // 6) Delete responses / supported ranges (from +CMGD=? etc)
  if (strstr(buffer, "+CMGD:")) return ATParse__CMGD(buffer, buflen, response_loglevel);
  #endif

  // (Add more URCs here later: RING, CLIP, audio, etc)

  // Not handled
  return false;
}

#ifdef ENABLE_FEATURE_SIM__SMS

// -------------------------------------------------------------------------------------------------
// Helpers: header parsing
// -------------------------------------------------------------------------------------------------

bool mSIM7000G::SMS2_ExtractNumberFromHeader(const char* header_line,
                                            char* out_number,
                                            size_t out_number_sz) const
{
  if (!header_line || !out_number || out_number_sz == 0) return false;
  out_number[0] = '\0';

  // Handles headers like:
  // +CMGR: "REC UNREAD","+4475...",,"26/01/17,00:07:39+00"
  // +CMT:  "+4475...",,"26/01/17,00:07:39+00"
  //
  // We extract the first phone number that appears inside quotes AFTER the first field.

  const char* p = strchr(header_line, '"');
  if (!p) return false;

  // Skip first quoted field
  const char* q = strchr(p + 1, '"');
  if (!q) return false;

  // Find next quoted field (the number)
  p = strchr(q + 1, '"');
  if (!p) return false;
  q = strchr(p + 1, '"');
  if (!q) return false;

  size_t n = (size_t)(q - (p + 1));
  if (n >= out_number_sz) n = out_number_sz - 1;

  memcpy(out_number, p + 1, n);
  out_number[n] = '\0';
  return (out_number[0] != '\0');
}



bool mSIM7000G::ATParse__CMGR_Header(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // +CMGR: <stat>,<oa>,...  (text mode)
  // Meaning: header of an SMS read response. The SMS body arrives on the NEXT line.
  // Flow:
  //   AT+CMGR=<idx>
  //   +CMGR: "REC UNREAD","+4475...",,"date"
  //   hello123
  //   OK
  if (!line) return false;

  char* p = strstr(line, "+CMGR:");
  if (!p) return false;

  ALOG_HGL(PSTR(D_LOG_CELLULAR "SMS2: CMGR header: %s"), p);

  // Save header and arm "next line is body"
  snprintf(sms2_header_line, sizeof(sms2_header_line), "%s", p);
  sms2_wait_kind   = SMS2_WAIT_CMGR_BODY;
  sms2_waiting_body = true;

  return true;
}

bool mSIM7000G::ATParse__CMT_Header(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // +CMT: <oa>,... (text mode)
  // Meaning: immediate SMS delivery over serial (CNMI mt=2).
  // Flow:
  //   +CMT: "+4475...",,"date"
  //   hello
  //
  // No storage index, so no CMGR/CMGD required.
  if (!line) return false;

  char* p = strstr(line, "+CMT:");
  if (!p) return false;

  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMS2: CMT header: %s"), p);

  snprintf(sms2_header_line, sizeof(sms2_header_line), "%s", p);
  sms2_wait_kind    = SMS2_WAIT_CMT_BODY;
  sms2_waiting_body = true;

  return true;
}

bool mSIM7000G::ATParse__BodyLine(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // This consumes the very next line after a +CMGR or +CMT header line.
  if (!sms2_waiting_body) return false;
  sms2_waiting_body = false;

  const char* body = line ? line : "";
  char from[32] = {0};

  if (!SMS2_ExtractNumberFromHeader(sms2_header_line, from, sizeof(from)))
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: sender parse failed, hdr=\"%s\""), sms2_header_line);
    sms2_header_line[0] = '\0';
    sms2_wait_kind = SMS2_WAIT_NONE;
    return true; // handled
  }

  // Dispatch SMS content into your command handler
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMS2: RX from=%s body=\"%s\""), from, body);
  SMS_Handle_IncomingText(from, body);

  // If this was a CMGR read, delete using the pending index (if set by your read logic)
  if (sms2_wait_kind == SMS2_WAIT_CMGR_BODY)
  {
    if (sms2_cmgr_pending_idx >= 0)
    {
      // Deleting is a “setup/process” action; blocking 1-2s is fine.
      SendAT_F(2000, PSTR("+CMGD=%d"), (int)sms2_cmgr_pending_idx);
      ALOG_DBG(PSTR(D_LOG_CELLULAR "SMS2: deleted idx=%d"), (int)sms2_cmgr_pending_idx);
      sms2_cmgr_pending_idx = -1;
    }
  }

  // Reset state
  sms2_header_line[0] = '\0';
  sms2_wait_kind = SMS2_WAIT_NONE;

  return true;
}


bool mSIM7000G::ATParse__CMTI(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // +CMTI: "SM",<index>
  // Meaning: SMS stored in memory. You must issue AT+CMGR=<index> to read it.
  // Then AT+CMGD=<index> to delete it.
  if (!line) return false;

  char* p = strstr(line, "+CMTI:");
  if (!p) return false;

  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMS2: URC CMTI (stored SMS): \"%s\""), p);

  // Find comma and parse index
  const char* comma = strchr(p, ',');
  if (!comma)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: CMTI parse failed (no comma)"));
    return true; // handled the line, but parse failed
  }

  int idx = atoi(comma + 1);
  if (idx < 0)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: CMTI parse failed (bad idx)"));
    return true;
  }

  sms.messages_incoming_index_list.push_back(idx);
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMS2: CMTI queued idx=%d"), idx);

  // Forward-looking: you can optionally trigger a read here (fire-and-forget),
  // but you asked to keep this generic, so we only queue.
  return true;
}
#endif // ENABLE_FEATURE_SIM__SMS




// -------------------------------------------------------------------------------------------------
// Dispatcher entry points (one line at a time)
// -------------------------------------------------------------------------------------------------

bool mSIM7000G::ATParse__OK(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // Typical OK line is "OK" (len=2). Some drivers include extra whitespace.
  if (!line || buflen < 2) return false;

  // Accept: "OK" or "OK " etc
  if (line[0] == 'O' && line[1] == 'K')
  {
    last_at_ok_seen = true;
    return true;
  }
  return false;
}

#ifdef ENABLE_FEATURE_SIM__SMS
bool mSIM7000G::ATParse__CMGL(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // +CMGL: <idx>,...
  // Meaning: list entry line from AT+CMGL=... (message list).
  // You still must issue AT+CMGR=<idx> to read each entry.
  if (!line) return false;

  char* p = strstr(line, "+CMGL:");
  if (!p) return false;

  int idx = -1;
  if (sscanf(p, "+CMGL: %d", &idx) == 1 && idx >= 0)
  {
    sms.messages_incoming_index_list.push_back(idx);
    ALOG_DBG(PSTR(D_LOG_CELLULAR "SMS: CMGL queued idx=%d"), idx);
  }
  else
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS: CMGL parse failed: \"%s\""), p);
  }
  return true;
}

bool mSIM7000G::ATParse__CPMS(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // +CPMS: "SM",used1,max1,"SM",used2,max2,"SM",used3,max3
  // Meaning: storage status for the selected SMS memory areas.
  if (!line) return false;

  char* p = strstr(line, "+CPMS:");
  if (!p) return false;

  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMS2: CPMS: %s"), p);
  // If you want structured numbers, parse here (but you asked to keep generic right now).
  return true;
}

bool mSIM7000G::ATParse__CMGD(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // +CMGD=? or CMGD responses; you already have:
  // ATParse_CMGD__CommandNameInTextDeleteMessage(line,...)
  if (!line) return false;

  if (!strstr(line, "+CMGD:")) return false;

  ATParse_CMGD__CommandNameInTextDeleteMessage(line, buflen, response_loglevel);
  return true;
}
#endif





// General Packet Radio Service



void mSIM7000G::GPRS_Disable()
{
  
  gprs.enabled = false;
}


void mSIM7000G::ModemUpdate_GPRS()
{

  if(mTime::TimeReached(&gprs.tReached_Update, 20))
  {
    
    if(gprs.enabled)
    {
      
      /**
       * @brief Check connection is still active
       */              
      
      if (modem->isGprsConnected()) 
      {

        GPRS_UpdateConnectionState(true);

        gprs.signal_quality_raw = modem->getSignalQuality();

        if(gprs.signal_quality_raw == 99)
        {
          gprs.signal_quality_rssi_dbm = -150;
          tkr_interface_network->data.cellular_state.isvalid = false;
          tkr_interface_network->data.mqtt_state.isvalid = false;
        }
        else
        {
          gprs.signal_quality_rssi_dbm = mSupport::mapfloat(gprs.signal_quality_raw, 0, 31, -113, -51);
          tkr_interface_network->data.cellular_state.isvalid = true;
        }

        ALOG_DBM(PSTR(D_LOG_CELLULAR "GPRS: Connected %d dBm (%d)"), int(gprs.signal_quality_rssi_dbm), gprs.signal_quality_raw);

        gprs.last_comms_millis_updated = millis();

            
        // String ccid = modem->getSimCCID();
        // ALOG_DBG(PSTR(D_LOG_CELLULAR "CCID: %s"), ccid.c_str());

        // String imei = modem->getIMEI();
        // ALOG_DBG(PSTR(D_LOG_CELLULAR "IMEI: %s"), imei.c_str());

        // String cop = modem->getOperator();
        // ALOG_DBG(PSTR(D_LOG_CELLULAR "Operator: %s"), cop.c_str());

        // IPAddress local = modem->localIP();
        // ALOG_DBG(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());

      } 
      else 
      {
        GPRS_UpdateConnectionState(false);
        ALOG_DBG(PSTR(D_LOG_CELLULAR "GPRS status: NOT connected Downtime: %d secs"), gprs.downtime_secs);
        DataNetwork__StartConnection();
      }

    }
    else
    {
      DataNetwork__InitConfig();
    }

  }

}


/**
 * @brief Function is called anytime the connection is checked, and will update the saved variable but configure flags when needed
 * 
 * @param state 
 */
void mSIM7000G::GPRS_UpdateConnectionState(bool state)
{

  /**
   * @brief State changed
   **/
  if(state != gprs.connected)
  {

    #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE
    if(state)
    {
  DEBUG_LINE_HERE;
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_NETWORK_CONNECTED__CELLULAR));
  DEBUG_LINE_HERE;
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_NETWORK_CONNECTED__ANY));
  DEBUG_LINE_HERE;
      gprs.reconnect_init_counts++;
    }
    else
    {
  DEBUG_LINE_HERE;
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_NETWORK_LOST__CELLULAR));
  DEBUG_LINE_HERE;
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_NETWORK_LOST__ANY));
  DEBUG_LINE_HERE;
    }
    #endif //ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

    gprs.connected = state; // Save new state
  }

  if(state)
  {
    gprs.connected_seconds++;
    gprs.downtime_secs = 0;
  }
  else
  {
    gprs.connected_seconds = 0;
    gprs.downtime_secs++;
  }

}
















/**
 * @brief Check for active comms on the serial line
 * 
 */
bool mSIM7000G::Modem__PowerUntilRunning(uint16_t wait_millis)
{

  Serial.println("> Check whether Modem is online");
    //test modem is online ?
    uint32_t  timeout = millis();
    while (!Modem__Running(100)) {
        Serial.print(".");
        if (millis() - timeout > wait_millis ) {
            Serial.println("> It looks like the modem is not responding, trying to restart");
            modemPowerOff();
            delay(5000);
            modemPowerOn();
            timeout = millis();
        }
    }
    Serial.println("\nModem is online");

    return true;

}


/**
 * @brief Check for active comms on the serial line
 * 
 */
bool mSIM7000G::Modem__Running(uint16_t wait_millis)
{

  ALOG_DBG(PSTR("mSIM7000G::Modem__Running"));
  bool modem_responded = false;
  modem_responded = modem->testAT(wait_millis);
  ALOG_DBG(PSTR("modem_responded = %d"), modem_responded);
  return modem_responded;

}


/**
 * @brief Commands required after restart
 * 
 */
bool mSIM7000G::SimNetwork__InitConfig()
{


}

/**
 * @brief Commands required to connect
 * 
 */
bool mSIM7000G::SimNetwork__StartConnection()
{


}

/**
 * @brief Check status of connection
 * 
 */
bool mSIM7000G::SimNetwork__CheckConnection()
{


}

/**
 * @brief Commands required after restart.
 *
 * IMPORTANT:
 * This function must never wait indefinitely for network registration.
 *
 * Its job is to:
 * - configure the modem radio/network preferences,
 * - inspect current registration state once,
 * - perform network-dependent setup only when registration already exists,
 * - return control to mCellular so retry/backoff remains handled by the
 *   cellular connection manager.
 *
 * No LTE service is a normal operating condition and must not block Tasker.
 */
bool mSIM7000G::DataNetwork__InitConfig()
{
  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__InitConfig()"));

  if(!modem)
  {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "DataNetwork__InitConfig: modem=null"));
    return false;
  }


  /************************************************************************************************
   * SECTION: Radio configuration
   ************************************************************************************************/

  modem->sendAT("+CBAND=ALL_MODE");

  if(modem->waitResponse(1000) != 1)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "CBAND configuration failed/timeout"));
  }


  // Args:
  // 1 CAT-M
  // 2 NB-IoT
  // 3 CAT-M and NB-IoT
  if(!modem->setPreferredMode(3))
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "setPreferredMode(3) failed"));
  }


  // Args:
  // 2  Automatic
  // 13 GSM only
  // 38 LTE only
  // 51 GSM and LTE only
  if(!modem->setNetworkMode(2))
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "setNetworkMode(2) failed"));
  }


  /************************************************************************************************
   * SECTION: Current network state
   *
   * Do NOT wait here for registration.
   *
   * mCellular owns retry/backoff policy. If there is no service, this function
   * must return so WiFi, GNSS, WebUI, MQTT and the rest of Tasker continue.
   ************************************************************************************************/

  const int16_t sq = modem->getSignalQuality();
  const RegStatus status = modem->getRegistrationStatus();

  ALOG_INF(
    PSTR(D_LOG_CELLULAR "Network status | CSQ=%d | RSSI=%d dBm | REG=%d"),
    sq,
    (int)GetSignalQualityPower(sq),
    status
  );


  if(status == REG_DENIED)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SIM/network registration denied"));
    return false;
  }


  if(status != REG_OK_HOME && status != REG_OK_ROAMING)
  {
    if(sq == 99)
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "No usable cellular signal; registration deferred"));
    }
    else
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Cellular network not registered yet; registration deferred"));
    }

    return false;
  }


  /************************************************************************************************
   * SECTION: Registered network configuration
   ************************************************************************************************/

  String res;

  ALOG_DBG(PSTR(D_LOG_CELLULAR "Obtain the APN issued by the network"));

  modem->sendAT("+CGNAPN");

  if(modem->waitResponse(1500,res) == 1)
  {
    const int comma_index = res.indexOf(",");

    if(comma_index >= 0)
    {
      res = res.substring(comma_index + 1);
      res.replace("\"","");
      res.replace("\r","");
      res.replace("\n","");
      res.replace("OK","");
      res.trim();

      ALOG_DBG(PSTR(D_LOG_CELLULAR "The APN issued by the network is: %s"),res.c_str());
    }
  }
  else
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "CGNAPN query failed/timeout"));
  }


  /************************************************************************************************
   * SECTION: Activate network context
   ************************************************************************************************/

  modem->sendAT("+CNACT=1");

  if(modem->waitResponse(2000) != 1)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "CNACT activation failed/timeout"));
    return false;
  }


  /************************************************************************************************
   * SECTION: Query current context/IP
   ************************************************************************************************/

  modem->sendAT("+CNACT?");

  if(modem->waitResponse("+CNACT: ") == 1)
  {
    modem->stream.read();
    modem->stream.read();

    res = modem->stream.readStringUntil('\n');

    res.replace("\"","");
    res.replace("\r","");
    res.replace("\n","");
    res.trim();

    modem->waitResponse(1000);

    ALOG_DBG(PSTR(D_LOG_CELLULAR "The current network IP address is: %s"),res.c_str());
  }
  else
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "CNACT status query failed/timeout"));
  }


  gprs.enabled = true;

  ALOG_INF(PSTR(D_LOG_CELLULAR "DataNetwork__InitConfig complete"));

  return true;
}


// uint32_t Cellular_GetRetryIntervalMs(uint32_t outage_seconds, cellular_failure_reason_t reason)
// {
//   if(reason == CELLULAR_FAILURE_NO_SIGNAL || reason == CELLULAR_FAILURE_NOT_REGISTERED)
//   {
//     if(outage_seconds < 60)        return 1000;
//     if(outage_seconds < 1800)      return 10000;
//     if(outage_seconds < 3600)      return 30000;
//     return 60000;
//   }

//   ...
// }


/**
 * @brief Commands required to connect
 * 
 */
bool mSIM7000G::DataNetwork__StartConnection()
{
  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__StartConnection()"));

  if(!modem)
  {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "DataNetwork__StartConnection failed: modem null"));
    return false;
  }

  ALOG_DBG(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Waiting for CELL TOWER network..."));

  if(!modem->waitForNetwork())  // SIM to cell tower connection, not GPRS
  {
    ALOG_DBG(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Timeout waiting for network"));
    return false;
  }

  if(modem->isNetworkConnected())
  {
    ALOG_DBG(PSTR(D_LOG_CELLULAR "Network connected"));
  }

  modem->gprsDisconnect();
  delay(50);

  ALOG_DBG(PSTR(D_LOG_CELLULAR "Connecting to: %s"), CELLULAR_APN);

  const bool gprs_connection_successful =
    modem->gprsConnect(CELLULAR_APN, CELLULAR_GPRS_USERNAME, CELLULAR_GPRS_PASSWORD);

  if(gprs_connection_successful)
  {
    ALOG_DBG(PSTR(D_LOG_CELLULAR "modem->gprsConnect success"));
    gprs.apn_connect_called++;
  }
  else
  {
    ALOG_DBG(PSTR(D_LOG_CELLULAR "modem->gprsConnect failed"));
    return false;
  }

  ALOG_DBG(PSTR(D_LOG_CELLULAR "GPRS Checking..."));

  if(!modem->isGprsConnected())
  {
    ALOG_HGL(PSTR(D_LOG_CELLULAR "GPRS NOT Connected"));
    GPRS_UpdateConnectionState(false);
    return false;
  }

  ALOG_DBG(PSTR(D_LOG_CELLULAR "GPRS Connected"));
  GPRS_UpdateConnectionState(true);

  String res;
  modem->sendAT("+CPSI?");

  if(modem->waitResponse("+CPSI: ") == 1)
  {
    res = modem->stream.readStringUntil('\n');
    res.replace("\r", "");
    res.replace("\n", "");
    modem->waitResponse();

    ALOG_DBG(PSTR(D_LOG_CELLULAR "The current network parameter is: %s"), res.c_str());
  }

  return true;
}






#endif