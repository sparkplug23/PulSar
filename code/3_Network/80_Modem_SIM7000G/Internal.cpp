#include "mSIM7000G.h"

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
  while (SerialAT.available() || ((wait_millis > 0) && ((millis() - t0) < wait_millis)))
  {
    if (!SerialAT.available())
    {
      // nothing right now, don’t burn CPU
      delay(1);
      continue;
    }

    const int c = SerialAT.read();
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
  ALOG_INF(PSTR(D_LOG_CELLULAR "ModemInit_ForceRestart requested"));
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

// ---- helpers ----------------------------------------------------------------

// static bool _at_sync_try(mSIM7000G* self)
// {
//   if (!self || !self->modem) return false;
//   // TinyGSM testAT() is already "AT" + wait OK.
//   return self->modem->testAT(mSIM7000G::AT_SYNC_STEP_TIMEOUT_MS);
// }

// static void _probe_fill_basic(mSIM7000G* self)
// {
//   if (!self || !self->modem) return;

//   self->modem_probe_.csq   = self->modem->getSignalQuality();
//   self->modem_probe_.at_ok = true;

//   // SIM primitive (not policy)
//   // If your TinyGSM build supports getSimStatus(), use it. Otherwise keep false.
//   #if defined(TINY_GSM_MODEM_SIM7000) || defined(TINY_GSM_MODEM_SIM7070) || defined(TINY_GSM_MODEM_SIM7080)
//     auto st = self->modem->getSimStatus();
//     self->modem_probe_.sim_present = (st != SIM_ERROR);
//     self->modem_probe_.sim_ready   = (st == SIM_READY);
//   #else
//     self->modem_probe_.sim_present = false;
//     self->modem_probe_.sim_ready   = false;
//   #endif

//   // Model string
//   String m = self->modem->getModemName();
//   if (!m.length()) m = self->modem->getModemInfo();
//   if (m.length()) {
//     m.toCharArray(self->modem_probe_.model, sizeof(self->modem_probe_.model));
//   }
// }

// // ---- tick -------------------------------------------------------------------

void mSIM7000G::ModemInit_Tick(uint32_t now_ms)
{
  // Done and no restart pending: nothing to do
  if (!modem_sm_.busy && !modem_sm_.restart_requested) {
    return;
  }

// Restart request wins
if (modem_sm_.restart_requested) {
  modem_sm_.restart_requested = false;
  modem_sm_.attempts          = 0;
  modem_sm_.ready             = false;
  modem_sm_.busy              = true;
  flag_modem_initialized       = false;

  modem_sm_.probe_done         = false;   // <-- ADD THIS

  ModemInit_SM_Enter(modem_sm_, modem_init_state_t::POWER_CYCLE_BEGIN, now_ms);
}


  // Give up
  if (modem_sm_.attempts >= INIT_MAX_ATTEMPTS &&
      modem_sm_.state != modem_init_state_t::READY) {
    modem_sm_.state = modem_init_state_t::FAILED;
  }

  // return;

  ALOG_INF(PSTR("state %d"),modem_sm_.state);

  switch (modem_sm_.state)
  {
    default:
    case modem_init_state_t::IDLE:
      modem_sm_.busy  = false;
      modem_sm_.ready = false;
      return;

    case modem_init_state_t::POWER_CYCLE_BEGIN:
    {
      modem_sm_.attempts++;

      ALOG_INF(PSTR(D_LOG_CELLULAR "ModemInit: power cycle begin (attempt %u)"),
               modem_sm_.attempts);

      // Ensure UART is down while toggling power key
      SerialAT.end();

      // Driver-owned power cycle (pins + delays live in these functions)
      modemPowerOff();
      ModemInit_SM_Enter(modem_sm_, modem_init_state_t::POWER_CYCLE_WAIT, now_ms);
    }
    break;

    case modem_init_state_t::POWER_CYCLE_WAIT:
    {
      // Minimum off time before powering on
      if ((now_ms - modem_sm_.t_enter_ms) >= POWER_OFF_MIN_MS) {
        modemPowerOn();
        ModemInit_SM_Enter(modem_sm_, modem_init_state_t::UART_BEGIN_BOOT_BAUD, now_ms);
      }
    }
    break;

    case modem_init_state_t::UART_BEGIN_BOOT_BAUD:
    {
      // Give modem time to boot before talking UART
      if ((now_ms - modem_sm_.t_enter_ms) < MODEM_BOOT_WAIT_MS) {
        break;
      }

      SerialAT.end();
      SerialAT.begin(MODEM_BOOT_BAUD, SERIAL_8N1, PIN_RX, PIN_TX);
      SerialAT.setTimeout(AT_STREAM_TIMEOUT_MS);

      ALOG_INF(PSTR(D_LOG_CELLULAR "ModemInit: UART begin @%lu"),
               (unsigned long)MODEM_BOOT_BAUD);

      ModemInit_SM_Enter(modem_sm_, modem_init_state_t::UART_SET_MODEM_BAUD, now_ms);
    }
    break;

    case modem_init_state_t::UART_SET_MODEM_BAUD:
    {
      // Let UART settle
      if ((now_ms - modem_sm_.t_enter_ms) < UART_SETTLE_MS) break;

      const uint32_t target = (uint32_t)UART_CELLULAR_BAUD;
      if (target == MODEM_BOOT_BAUD) {
        ModemInit_SM_Enter(modem_sm_, modem_init_state_t::AT_SYNC, now_ms);
        break;
      }

      ALOG_INF(PSTR(D_LOG_CELLULAR "ModemInit: set modem baud -> %lu"),
               (unsigned long)target);

      // Tell modem to switch baud. If it fails, AT sync will fail and we retry.
      modem->setBaud(target);

      SerialAT.flush();
      SerialAT.end();
      ModemInit_SM_Enter(modem_sm_, modem_init_state_t::UART_REOPEN_TARGET_BAUD, now_ms);
    }
    break;

    case modem_init_state_t::UART_REOPEN_TARGET_BAUD:
    {
      if ((now_ms - modem_sm_.t_enter_ms) < UART_REOPEN_DELAY_MS) break;

      const uint32_t target = (uint32_t)UART_CELLULAR_BAUD;
      SerialAT.begin(target, SERIAL_8N1, PIN_RX, PIN_TX);
      SerialAT.setTimeout(AT_STREAM_TIMEOUT_MS);

      ALOG_INF(PSTR(D_LOG_CELLULAR "ModemInit: UART reopen @%lu"),
               (unsigned long)target);

      ModemInit_SM_Enter(modem_sm_, modem_init_state_t::AT_SYNC, now_ms);
    }
    break;

    case modem_init_state_t::AT_SYNC:
    {
      bool ok = false;

      // Micro-attempts per tick, no long blocks
      for (uint8_t i = 0; i < AT_SYNC_ATTEMPTS_PER_TICK; i++) {
        if (ModemInit_ATSyncTry()) { ok = true; break; }
      }

      if (ok) {
        modem_probe_.at_ok = true;
        ModemInit_SM_Enter(modem_sm_, modem_init_state_t::BASIC_CFG, now_ms);
      } else {
        // If AT sync hasn’t succeeded within window, restart the sequence
        if ((now_ms - modem_sm_.t_enter_ms) > AT_SYNC_TOTAL_WINDOW_MS) {
          ALOG_INF(PSTR(D_LOG_CELLULAR "ModemInit: AT sync failed, retrying"));
          ModemInit_SM_Enter(modem_sm_, modem_init_state_t::POWER_CYCLE_BEGIN, now_ms);
        }
      }
    }
    break;

    case modem_init_state_t::BASIC_CFG:
    {
      // Keep this minimal and fast. Optional but recommended.

      // Disable echo
      modem->sendAT("E0");
      modem->waitResponse(AT_CFG_STEP_TIMEOUT_MS);

      // Verbose errors
      modem->sendAT("+CMEE=2");
      modem->waitResponse(AT_CFG_STEP_TIMEOUT_MS);

      ModemInit_SM_Enter(modem_sm_, modem_init_state_t::PROBE_ID, now_ms);
    }
    break;

    case modem_init_state_t::PROBE_ID:
{
  if (!modem_sm_.probe_done) {
    ModemInit_ProbeFillBasic();
    modem_sm_.probe_done = true;
  }
  ModemInit_SM_Enter(modem_sm_, modem_init_state_t::READY, now_ms);
}
break;

    case modem_init_state_t::READY:
    {
      modem_sm_.ready = true;
      modem_sm_.busy  = false;

      ALOG_INF(PSTR(D_LOG_CELLULAR "ModemInit: READY (AT OK, model=%s, csq=%d)"),
               modem_probe_.model, modem_probe_.csq);
    }
    break;

    case modem_init_state_t::FAILED:
    {
      modem_sm_.busy  = false;
      modem_sm_.ready = false;

      ALOG_ERR(PSTR(D_LOG_CELLULAR "ModemInit: FAILED after %u attempts"),
               modem_sm_.attempts);
    }
    break;
  }
}


TinyGsmClient* mSIM7000G::DataNetwork_GetOrCreateClient(bool force_recreate)
{
  if (!modem) return nullptr;

  if (force_recreate && gsm_client) {
    gsm_client->stop();
    delete gsm_client;
    gsm_client = nullptr;
  }

  if (!gsm_client) {
    gsm_client = new TinyGsmClient(*modem);
  }

  return gsm_client;
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
    ALOG_INF(PSTR(D_LOG_CELLULAR "Query_Modem_Status: modem=null"));
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
      ALOG_INF(PSTR(D_LOG_CELLULAR "AT failed %u times -> ModemInit_ForceRestart()"), fail_count);
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
  while (SerialAT.available()) { (void)SerialAT.read(); }

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

  // --------------------------------------------------------------------------
  // 6) One-line status log (what you actually want in TASK_EVERY_SECOND)
  // --------------------------------------------------------------------------
  
  if(rssi_dbm > -100) return; // no message if we have good signal


  if (!sim_present) {
    ALOG_INF(PSTR(D_LOG_CELLULAR "AT OK, SIM: no response (CPIN?) , CSQ=%d (%.1f dBm), REG=%d"),
             (int)csq, (double)rssi_dbm, (int)reg);
  } else if (!sim_ready) {
    ALOG_INF(PSTR(D_LOG_CELLULAR "AT OK, SIM: present NOT READY, CSQ=%d (%.1f dBm), REG=%d"),
             (int)csq, (double)rssi_dbm, (int)reg);
  } else {
    if (ccid[0] != '\0') {
      ALOG_INF(PSTR(D_LOG_CELLULAR "SIM READY, CCID=%s, CSQ=%d (%.1f dBm), REG=%d"),
               ccid, (int)csq, (double)rssi_dbm, (int)reg);
    } else {
      ALOG_INF(PSTR(D_LOG_CELLULAR "AT OK, SIM READY, CSQ=%d (%.1f dBm), REG=%d"),
               (int)csq, (double)rssi_dbm, (int)reg);
    }
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
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Minimum %d"), result);
}
void mSIM7000G::SendATCommand_FunctionalityMode_Full()
{
  modem->sendAT("+CFUN=1");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CFUN); 
  ALOG_INF(PSTR(D_LOG_CELLULAR "FunctionalityMode_Full %d"), result);
}



void mSIM7000G::ModemUpdate_BatteryStatus()
{
  modem_status.battery.isvalid = modem->getBattStats(
    modem_status.battery.charge_state, 
    modem_status.battery.percentage, 
    modem_status.battery.volts_mv
  );

  ALOG_DBM(PSTR(D_LOG_CELLULAR "{\"Volts_mv\":%d,\"Percent\":%d,\"Charging\":%d}"),
    modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);

  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
  tkr_batt_modem->readings.battery.volts_mv = modem_status.battery.volts_mv;
  tkr_batt_modem->readings.battery.percentage = modem_status.battery.percentage;
  #endif

}


void mSIM7000G::modemPowerOn()
{
  WDT_Reset();

  ALOG_INF(PSTR("modemPowerOn"));
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1000);    // Datasheet T_on = 72ms
  digitalWrite(PWR_PIN, HIGH);
  flag_modem_initialized = false;
        DEBUG_LINE_HERE;

}

void mSIM7000G::modemPowerOff()
{
  WDT_Reset();
  ALOG_INF(PSTR("modemPowerOff"));
  pinMode(PWR_PIN, OUTPUT);
  digitalWrite(PWR_PIN, LOW);
  delay(1500);    // Datasheet T_off = 1.2secs
  digitalWrite(PWR_PIN, HIGH);
}

void mSIM7000G::modemRestart()
{
  WDT_Reset();
  ALOG_INF(PSTR("modemRestart"));
  modemPowerOff();
  delay(5000);
  modemPowerOn();
}


// ==============================================================================================




bool mSIM7000G::Modem_CheckAndRestartUnresponsiveModem()
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

  ALOG_INF(PSTR(D_LOG_CELLULAR ">>>%s %s (%dms)"), buffer, result?"Success":"FAILED", tSaved_Elapsed);

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
    ALOG_INF( PSTR(D_LOG_CELLULAR "OpenHAB:SystemTime"));
    ALOG_INF(PSTR(D_LOG_CELLULAR "OH Time %s"),jtok.getStr());
    //toggle led
    DIGITAL_INVERT_PIN(12); //esp32 blue led
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["GPS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "GPS_Enable"));
    if(jtok.getInt() == 1)
    {
      GPS_Enable();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  if(jtok = obj["SMSAuto_GPS"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMSAuto_GPS smsauto_gps_messages.rate_seconds %d"), jtok.getInt());
    #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS 
    smsauto_gps_messages.rate_seconds = jtok.getInt();
    #endif
  }



  if(jtok = obj["GPRS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "GPRS_Enable"));
    if(jtok.getInt() == 1)
    {
      DataNetwork__InitConfig();
    }
  }


  if(jtok = obj["SMS_Enable"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMS_Enable"));
    if(jtok.getInt() == 1)
    {
      SMS_Enable();
    }
  }

  #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
  if(jtok = obj["SMS_GPSLocation"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMS_GPSLocation"));
    if(jtok.getInt() == 1)
    {
      SMS_GPSLocation();
    }
  }
  #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


  if(jtok = obj["SMSForward"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "SMSForward"));
    if(jtok.getInt() == 1)
    {
      SendATCommand_SMSImmediateForwardOverSerial();
    }
  }

  
  if(jtok = obj["BattVolt"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "BattVolt"));
    if(jtok.getInt() == 1)
    {    
      // read the battery voltage and percentage
      uint16_t vbat = modem->getBattVoltage();        
      ALOG_INF(PSTR(D_LOG_CELLULAR "%d mV"), vbat);
    }
  }

  if(jtok = obj["ModemInit"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ModemInit"));
    flag_modem_initialized = jtok.getInt();
  }

  if(jtok = obj["ADCVoltage"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ADCVoltage"));
    if(jtok.getInt() == 1)
    {      
      ModemUpdate_BatteryStatus();
      ALOG_INF(PSTR(D_LOG_CELLULAR "{\"volts_mv\":%d,\"percent\":%d,\"milliVolts\":%d}"),
        modem_status.battery.volts_mv,modem_status.battery.percentage,modem_status.battery.charge_state);
    }
  }


  if(jtok = obj["ReadSMS"]){

    ALOG_INF( PSTR(D_LOG_CELLULAR "ReadSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 

      modem->sendAT("+CMGR=0");

      //parser right here

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_INF(PSTR(D_LOG_CELLULAR "ReadSMS %d"), result);
      

    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }
  
  
  if(jtok = obj["ATDebugCommand"])
  {

    if(jtok2 = jtok.getObject()["SMSReadOnly"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadOnly"));

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
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadAndClear"));

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
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSReadAndClearAll"));
      
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
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSCheckWaitingIndexs"));
      
      if(SendAT_ATParseResponse_F(100, LOG_LEVEL_COMMANDS, PSTR("+CMGD=?")))
      {
        // Clear it if it was successfully read
        // SendAT_F(100, PSTR("+CMGD=%d"), jtok2.getInt());
      }
      
    }


    if(jtok2 = jtok.getObject()["SMSClearAll"])
    {
      ALOG_INF( PSTR(D_LOG_CELLULAR "SMSClearAll"));
      uint8_t flag = 4; // delete all
      SendAT_F(100, PSTR("+CMGR=%d[,%d]"), jtok2.getInt(), flag);
    }

  }

  if(jtok = obj["ReadDeleteSMS"]){

    ALOG_INF( PSTR(D_LOG_CELLULAR "ReadDeleteSMS"));

    // flag_modem_initialized = jtok.getInt();
  
      // modem->sendAT("+CNMI?"); // <mode>,<mt>,<bm>,<ds>,<bfr> //expected 2,2,0,0,0 


      // SendAT("+CMGR=0[,4]", 100);

      // uint8_t flag = 4; // delete all

      // SendAT_F(PSTR("+CMGR=%d[,%d]"), index, flag);

      modem->sendAT("+CMGR=0[,4]");

// //+CMGD: (list_of_indexes)[,(list_of_flag_values)]

//       //parser right here

      bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
//       ALOG_INF(PSTR(D_LOG_CELLULAR "ReadDeleteSMS %d"), result);
      

      modem->sendAT("+CMGD=0");

      
       result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
      ALOG_INF(PSTR(D_LOG_CELLULAR "ReadDeleteSMS %d"), result);
    // "AT+CMGD=,4",// DELETE ALL MESSAGES (Read or not)
    // "AT+CMGR=1",
    // "AT+CMGL=\"REC UNREAD\""


  }

      // modem->sendAT("AT+CMGL=\"ALL\"");

      // IPAddress local = modem->localIP();
      // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());
      
      // modem->sendAT("+CPMS?"); // How many SMS are waiting?
  // SendATCommand_SMSImmediateForwardOverSerial();



  if(jtok = obj["ATCommands"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ATCommands"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      SerialAT.println(val.getStr());  
      delay(500);
      if (SerialAT.available()) {
        String r = SerialAT.readString();
        ALOG_INF(PSTR(D_LOG_CELLULAR "ATResponse = \"%s\""), r.c_str());
      }
    }

  }

  if(jtok = obj["ATCommandsParsing"]){
    ALOG_INF( PSTR(D_LOG_CELLULAR "ATCommandsParsing"));
    JsonParserArray array = jtok;
    for(auto val : array) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "F::%s %s"),__FUNCTION__,val.getStr());
      SerialAT.println(val.getStr());  
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







// bool mSIM7000G::parse_ATCommands(char* buffer, uint16_t buflen, uint8_t response_loglevel)
// {

//   if (!buffer || buflen == 0) return false;

//   // Ensure null termination within bounds
//   // buffer[buflen - 1] = '\0';

//   // Optional: ignore empty lines early
//   if (buffer[0] == '\0') return false;

//   // ALOG_INF(PSTR(D_LOG_CELLULAR "parse_ATCommands %d>> Buffer[%d] \"%s\""),
//           //  response_loglevel, buflen, buffer);

//   // #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
//   ALOG_INF(PSTR(D_LOG_CELLULAR "parse_ATCommands %d>> Buffer[%d] \"%s\""),response_loglevel, buflen, buffer);
//   // #endif



//   // +CDS:  A GSM/GPRS modem or mobile phone uses +CDS to forward a newly received SMS status report to the computer / PC.
//   // +CDSI: A GSM/GPRS modem or mobile phone uses +CDSI to notify the computer / PC that a new SMS status report has been received and the memory location where it is stored.
//   // +CMT:  A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
//   // +CMTI: A GSM/GPRS modem or mobile phone uses +CMTI to notify the computer / PC that a new SMS message has been received and the memory location where it is stored.
//   /**
//    * @brief CMT is when messages are set to be forwarded, but may be missed, so leaving this as legacy to match messages
//    **/
//   char buffer2[300] = {0};
//   char *search = "+CMT:"; // A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
//   char *result = strstr(buffer, search);
//   if(result)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMT\" result >>>\n\r%s\n\r<<<"), result);

//     // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

//     // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

//     // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     // // Check for request at GPS
//     // if (strcasecmp(buffer2, "GPS") == 0)
//     // {
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
//     //   SMS_GPSLocation();
//     // }else
//     // if (strcasecmp(buffer2, "Batt") == 0)
//     // {
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
//     //   SMS_BatteryDetailed();
//     // }
//     // else{
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
//     //   SMS_GPSLocation();
//     // }
//     // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


//     // const char* delims = "\r\n"; //space + ,
//     // char* tok = strtok(result, delims); // I believe all delims are replaced by NULL?

//     // if(tok)
//     // {  
//     //   /**
//     //   for (int i=0;i<5;i++)
//     //   {
//     //     ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
//     //     tok = strtok(NULL, delims);
//     //   }

//     //   01:06 INF tok[0] = +CMGR: "REC READ","+447515358597",,"23/05/12,21:28:36+04"
//     //   01:06 INF tok[1] = Message1
//     //   01:06 INF tok[2] = OK
//     //   01:06 INF tok[3] = ERROR
//     //   01:06 INF tok[4] = ERROR
//     //   **/
    

//     //   tok = strtok(NULL, delims);   // SMS header
//     //   // tok = strtok(NULL, delims); 
//     //   char sms_contents[160];

//     //   if(tok != NULL) 
//     //   {   
//     //     snprintf(sms_contents, sizeof(sms_contents), tok); 
//     //     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Message = \"%s\""), sms_contents);

//     //     #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     //     if (strcasecmp(sms_contents, "GPS") == 0)
//     //     {
//     //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
//     //       SMS_GPSLocation();
//     //     }
//     //     else
//     //     if (strcasecmp(sms_contents, "Batt") == 0)
//     //     {
//     //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
//     //       SMS_BatteryDetailed();
//     //     }
//     //     else
//     //     {
//     //       ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
//     //       SMS_GPSLocation();
//     //     }
//     //     #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

//     //   }

//     // }


//   }
    
//   char *cmgl = strstr(buffer, "+CMGL:");
//   if (cmgl)
//   {
//     // Format: +CMGL: <idx>,...
//     // Extract idx safely without strtok on the main buffer if possible.
//     int idx = -1;
//     if (sscanf(cmgl, "+CMGL: %d", &idx) == 1 && idx >= 0)
//     {
//       sms.messages_incoming_index_list.push_back(idx);
//       ALOG_INF(PSTR(D_LOG_CELLULAR "CMGL queued idx=%d"), idx);
//     }
//   }

//   char *result2 = strstr(buffer, "+CMTI:"); 
//   if(result2)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMTI\" result >>>\n\r%s\n\r<<<"), result2);


//     const char* delims = ",\r"; //space + ,
//     char* tokens = strtok(result2, delims); // I believe all delims are replaced by NULL?

//     if(tokens)
//     {  
//     tokens = strtok(NULL, delims); 
//     int new_sms_index = atoi(tokens);

//     /**
//      * @brief Add unique name
//      **/
    
//     sms.messages_incoming_index_list.push_back(new_sms_index);
//     }



//     // /**
//     //  * @brief Splitting the Indexs out
//     //  **/
//     // const char* delims2 = ","; //space + ,
//     // char* tok_ids = strtok(buffer_id, delims2); 
//     // uint8_t i = 0;
//     // sms.messages_incoming_index_list.clear();
//     // while(tok_ids){
//     //   ALOG_INF(PSTR("tok_ids[%d] = %s"), i++, tok_ids?tok_ids:"ERROR");
//     //   sms.messages_incoming_index_list.push_back(atoi(tok_ids));
//     //   tok_ids = strtok(NULL, delims2);
//     // }



//     // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

//     // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

//     // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     // // Check for request at GPS
//     // if (strcasecmp(buffer2, "GPS") == 0)
//     // {
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
//     //   SMS_GPSLocation();
//     // }
//     // else{
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
//     //   SMS_GPSLocation();
//     // }
//     // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

//   }
  
//   /**
//    * @brief 
//    * +CPMS: used_space1,max_space1,used_space2,max_space2,used_space3,max_space3
//    * used_space1 is the number of messages currently stored in the message storage area specified by the message_storage1 parameter.
//    * max_space1 is the maximum number of messages that can be stored in the message storage area specified by the message_storage1 parameter. 
//    */
//   result2 = strstr(buffer, "+CPMS:"); 
//   if(result2)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "MATCHED \"CPMS\" >>%s<<"), result2);

//     const char* delims = ","; //space + ,
//     char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

//     if(tok)
//     {  
//     tok = strtok(NULL, delims); 
//     int used_space1 = atoi(tok);
//     tok = strtok(NULL, delims); 
//     int max_space1  = atoi(tok);

//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Stored = %d/%d"), used_space1, max_space1);
//     }

//     // tok


//     // for (int i=0;i<5;i++)
//     // {
//     //   ALOG_INF(PSTR("tok[%d] = %s"), i, tok);
//     //   tok = strtok(NULL, delims);
//     // }

//   /**
//    *                           +CPMS: (storage1a,storage1b,...),(storage2a,storage2b,...),(storage3a,storage3b,...)
//    * INF CEL: MATCHED "CPMS" >>+CPMS: "SM",5,10,"SM",5,10,"SM",5,10
//    * OK<<
//    * 16:35 INF tok[0] = 5
//    * 16:35 INF tok[1] = 10
//    * 16:35 INF tok[2] = "SM"
//    * 16:35 INF tok[3] = 5
//    * 16:35 INF tok[4] = 10 
//    */



//     // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

//     // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

//     // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     // // Check for request at GPS
//     // if (strcasecmp(buffer2, "GPS") == 0)
//     // {
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
//     //   SMS_GPSLocation();
//     // }
//     // else{
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
//     //   SMS_GPSLocation();
//     // }
//     // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

//   }

// //   result2 = strstr(buffer, "+CMGR:"); 
// //   if(result2)
// //   {
// //     ALOG_HGL(PSTR(D_LOG_CELLULAR "MATCHED \"CMGR\"\n\r>>%s<<"), result2);

// //     const char* delims = "\r\n"; //space + ,
// //     char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

// //     if(tok)
// //     {  
// //       /**
// //       for (int i=0;i<5;i++)
// //       {
// //         ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
// //         tok = strtok(NULL, delims);
// //       }

// //       01:06 INF tok[0] = +CMGR: "REC READ","+447515358597",,"23/05/12,21:28:36+04"
// //       01:06 INF tok[1] = Message1
// //       01:06 INF tok[2] = OK
// //       01:06 INF tok[3] = ERROR
// //       01:06 INF tok[4] = ERROR
// //       **/
    

// //       tok = strtok(NULL, delims);   // SMS header
// //       // tok = strtok(NULL, delims); 
// //       char sms_contents[160];

// //       if(tok != NULL) 
// //       {   
// //         snprintf(sms_contents, sizeof(sms_contents), "%s", tok);

// //         ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Message = \"%s\""), sms_contents);

// //         #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
// //         if (strcasecmp(sms_contents, "GPS") == 0)
// //         {
// //           ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
// //           SMS_GPSLocation();
// //         }
// //         else
// //         if (strcasecmp(sms_contents, "Batt") == 0)
// //         {
// //           ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
// //           SMS_BatteryDetailed();
// //         }
// //         else
// //         if (strstr(sms_contents, "Command") != NULL)
// //         {
// //           ALOG_INF(PSTR(D_LOG_CELLULAR "Future Command with format \"Command KEY VALUE\" that will be shaped into JSON and send to parser"));
// //           SMS_CommandIntoJSONCommand(sms_contents);
// //         }
// //         else
// //         {
// //           ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
// //           SMS_GPSLocation();
// //         }
// //         #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

// //       }

// //     }

// //   }

// result2 = strstr(buffer, "+CMGR:");
// if (result2)
// {
//   ALOG_HGL(PSTR(D_LOG_CELLULAR "MATCHED \"CMGR\"\n\r>>%s<<"), result2);

//   // New unified parse: extract sender + message body
//   char from[32]  = {0};
//   char body[180] = {0};

//   if (SMS_Parse_CMGR(result2, from, sizeof(from), body, sizeof(body)))
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS from=%s body=\"%s\""), from, body);

//     // Primary: keyword router (add "hello" here, later gps/batt/etc)
//     // Returns true if it handled and (optionally) replied.
//     bool handled = SMS_Handle_IncomingText(from, body);

//     // Optional: keep legacy behaviour if not handled by router
//     if (!handled)
//     {
//       // Legacy command handling (existing logic, now uses "body")
//       // NOTE: body is not normalised here; do case-insensitive comparisons.
//       #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//       if (strcasecmp(body, "GPS") == 0)
//       {
//         ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
//         SMS_GPSLocation();
//       }
//       else if (strcasecmp(body, "Batt") == 0)
//       {
//         ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
//         SMS_BatteryDetailed();
//       }
//       else
//       #endif 
//       if (strstr(body, "Command") != NULL)
//       {
//         ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Command -> JSON"));
//         SMS_CommandIntoJSONCommand(body);
//       }
//       else
//       {
//         ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Unknown command"));
//         // No default action, or keep your previous default:
//         // SMS_GPSLocation();
//       }
//     }
//   }
//   else
//   {
//     ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS_Parse_CMGR failed"));
//   }
// }



//   if(strstr(buffer, "+CMGD:"))
//   {
//     #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
//     ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage[%d] %s"), buflen , buffer);
//     #endif
//     ATParse_CMGD__CommandNameInTextDeleteMessage(buffer, buflen, response_loglevel);
//   }


//   return false;

// }


// bool mSIM7000G::parse_ATCommands(char* buffer, uint16_t buflen, uint8_t response_loglevel)
// {

//   if (!buffer || buflen == 0) return false;

//   // Ensure null termination within bounds
//   // buffer[buflen - 1] = '\0';

//   // Optional: ignore empty lines early
//   if (buffer[0] == '\0') return false;

//     // Fast-path: bare "OK"
//     // Typical buffer is "OK" or "OK\n" → buflen == 2
//     if (buflen <= 3 && buffer[0] == 'O' && buffer[1] == 'K')
//     {
//       last_at_ok_seen = true;   // <-- define this flag in your class
//       return true;             // handled, nothing else to do
//     }


//   // ALOG_INF(PSTR(D_LOG_CELLULAR "parse_ATCommands %d>> Buffer[%d] \"%s\""),
//           //  response_loglevel, buflen, buffer);

//   // #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
//   ALOG_INF(PSTR(D_LOG_CELLULAR "parse_ATCommands %d>> Buffer[%d] \"%s\""),response_loglevel, buflen, buffer);
//   // #endif
// #ifdef ENABLE_FEATURE_SIM__SMS

//   // If we are waiting for the CMGR body line, this buffer IS the body
//   if (sms2_waiting_cmgr_body)
//   {
//     sms2_waiting_cmgr_body = false;

//     const char* body = buffer;     // current line is the SMS text
//     const char* hdr  = sms2_cmgr_header;

//     // Extract sender from saved header (simple, safe)
//     char from[32] = {0};
//     if (SMS2_ExtractNumberFromCMGRHeader(hdr, from, sizeof(from)))
//     {
//       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: from=%s body=\"%s\""), from, body);
//       SMS_Handle_IncomingText(from, body);
//     }
//     else
//     {
//       ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: CMGR sender parse failed"));
//     }

//     // Delete the message we read
//     if (sms2_cmgr_pending_idx >= 0)
//     {
//       SendAT_F(2000, PSTR("+CMGD=%d"), sms2_cmgr_pending_idx);
//       sms2_cmgr_pending_idx = -1;
//     }

//     sms2_cmgr_header[0] = '\0';
//     return true;
//   }

//   // CMGR header line arms the "next line is body" state
//   char* cmgr = strstr(buffer, "+CMGR:");
//   if (cmgr)
//   {
//     snprintf(sms2_cmgr_header, sizeof(sms2_cmgr_header), "%s", cmgr);
//     sms2_waiting_cmgr_body = true;
//     return true;
//   }

// #endif

//   #ifdef ENABLE_FEATURE_SIM__SMS
//     SMS2_ATParse_Line(buffer, buflen, response_loglevel);
//   #endif


//   // +CDS:  A GSM/GPRS modem or mobile phone uses +CDS to forward a newly received SMS status report to the computer / PC.
//   // +CDSI: A GSM/GPRS modem or mobile phone uses +CDSI to notify the computer / PC that a new SMS status report has been received and the memory location where it is stored.
//   // +CMT:  A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
//   // +CMTI: A GSM/GPRS modem or mobile phone uses +CMTI to notify the computer / PC that a new SMS message has been received and the memory location where it is stored.
//   /**
//    * @brief CMT is when messages are set to be forwarded, but may be missed, so leaving this as legacy to match messages
//    **/
//   char buffer2[300] = {0};
//   char *search = "+CMT:"; // A GSM/GPRS modem or mobile phone uses +CMT to forward a newly received SMS message to the computer / PC.
//   char *result = strstr(buffer, search);
//   if(result)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMT\" result >>>\n\r%s\n\r<<<"), result);

// #ifdef ENABLE_FEATURE_SIM__SMS
//     /**
//      * @brief IMMEDIATE SMS HANDLING (CNMI=3,2,0,0,0)
//      * +CMT is self-contained (header + body), no storage, no CMGR.
//      */
//     // if (sms.enabled)
//     // {
//       char from[32]  = {0};
//       char body[160] = {0};

//       if (SMS_Parse_CMT(buffer,
//                         from, sizeof(from),
//                         body, sizeof(body)))
//       {
//         ALOG_INF(PSTR(D_LOG_CELLULAR "SMS RX from=%s body=\"%s\""), from, body);

//         // Minimal test response
//         SMS_SendText(from, "ACK");
//       }
//       else
//       {
//         ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS_Parse_CMT failed"));
//       }
//     // }
// #endif // ENABLE_FEATURE_SIM__SMS

//     // ATResponse_Parse_CMT(buffer, buffer2, sizeof(buffer2));

//     // ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Content \"%s\""), buffer2);

//     // #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     // // Check for request at GPS
//     // if (strcasecmp(buffer2, "GPS") == 0)
//     // {
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
//     //   SMS_GPSLocation();
//     // }else
//     // if (strcasecmp(buffer2, "Batt") == 0)
//     // {
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
//     //   SMS_BatteryDetailed();
//     // }
//     // else{
//     //   ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
//     //   SMS_GPSLocation();
//     // }
//     // #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS


//     // const char* delims = "\r\n"; //space + ,
//     // char* tok = strtok(result, delims); // I believe all delims are replaced by NULL?

//     // if(tok)
//     // {  
//     //   /**
//     //   for (int i=0;i<5;i++)
//     //   {
//     //     ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
//     //     tok = strtok(NULL, delims);
//     //   }

//     //   01:06 INF tok[0] = +CMGR: "REC READ","+447515358597",,"23/05/12,21:28:36+04"
//     //   01:06 INF tok[1] = Message1
//     //   01:06 INF tok[2] = OK
//     //   01:06 INF tok[3] = ERROR
//     //   01:06 INF tok[4] = ERROR
//     //   **/
    

//     //   tok = strtok(NULL, delims);   // SMS header
//     //   // tok = strtok(NULL, delims); 
//     //   char sms_contents[160];

//     //   if(tok != NULL) 
//     //   {   
//     //     snprintf(sms_contents, sizeof(sms_contents), tok); 
//     //     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Message = \"%s\""), sms_contents);

//     //     #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//     //     if (strcasecmp(sms_contents, "GPS") == 0)
//     //     {
//     //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for GPS"));
//     //       SMS_GPSLocation();
//     //     }
//     //     else
//     //     if (strcasecmp(sms_contents, "Batt") == 0)
//     //     {
//     //       ALOG_INF(PSTR(D_LOG_CELLULAR "SMS Request for Battery"));
//     //       SMS_BatteryDetailed();
//     //     }
//     //     else
//     //     {
//     //       ALOG_INF(PSTR(D_LOG_CELLULAR "Unknown Message"));
//     //       SMS_GPSLocation();
//     //     }
//     //     #endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

//     //   }

//     // }

//   }
    
//   char *cmgl = strstr(buffer, "+CMGL:");
//   if (cmgl)
//   {
//     // Format: +CMGL: <idx>,...
//     // Extract idx safely without strtok on the main buffer if possible.
//     int idx = -1;
//     if (sscanf(cmgl, "+CMGL: %d", &idx) == 1 && idx >= 0)
//     {
//       sms.messages_incoming_index_list.push_back(idx);
//       ALOG_INF(PSTR(D_LOG_CELLULAR "CMGL queued idx=%d"), idx);
//     }
//   }

//   char* result2 = strstr(buffer, "+CMTI:");
//   if (result2)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "Parsed \"CMTI\" result >>>\n\r%s\n\r<<<"), result2);

//     // Expected format:
//     // +CMTI: "SM",<index>

//     int new_sms_index = -1;

//     // Find comma separating storage and index
//     const char* comma = strchr(result2, ',');
//     if (comma)
//     {
//       new_sms_index = atoi(comma + 1);
//     }

//     if (new_sms_index >= 0)
//     {
//       sms.messages_incoming_index_list.push_back(new_sms_index);
//       ALOG_INF(PSTR(D_LOG_CELLULAR "CMTI queued SMS index=%d"), new_sms_index);
//     }
//     else
//     {
//       ALOG_WRN(PSTR(D_LOG_CELLULAR "CMTI parse failed"));
//     }
//   }

  
//   /**
//    * @brief 
//    * +CPMS: used_space1,max_space1,used_space2,max_space2,used_space3,max_space3
//    */
//   result2 = strstr(buffer, "+CPMS:"); 
//   if(result2)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "MATCHED \"CPMS\" >>%s<<"), result2);
//   }

// result2 = strstr(buffer, "+CMGR:");
// if (result2)
// {
//   ALOG_HGL(PSTR(D_LOG_CELLULAR DEBUG_INSERT_PAGE_BREAK "MATCHED \"CMGR\"\n\r>>%s<<"), result2);

//   char from[32]  = {0};
//   char body[180] = {0};

//   if (SMS_Parse_CMGR(result2, from, sizeof(from), body, sizeof(body)))
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS from=%s body=\"%s\""), from, body);

//     bool handled = SMS_Handle_IncomingText(from, body);

//     if (!handled)
//     {
// #ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
//       if (strcasecmp(body, "GPS") == 0)
//       {
//         SMS_GPSLocation();
//       }
//       else if (strcasecmp(body, "Batt") == 0)
//       {
//         SMS_BatteryDetailed();
//       }
//       else
// #endif 
//       if (strstr(body, "Command") != NULL)
//       {
//         SMS_CommandIntoJSONCommand(body);
//       }
//     }
//   }
// }

//   if(strstr(buffer, "+CMGD:"))
//   {
//     ATParse_CMGD__CommandNameInTextDeleteMessage(buffer, buflen, response_loglevel);
//   }

//   return false;
// }

bool mSIM7000G::parse_ATCommands(char* buffer, uint16_t buflen, uint8_t response_loglevel)
{
  if (!buffer || buflen == 0) return false;
  if (buffer[0] == '\0') return false;

  // ---- OPTIONAL: suppress OK spam but keep a flag ----
  if (ATParse__OK(buffer, buflen, response_loglevel))
  { // handled "OK"
    // ALOG_INF(PSTR("Suppress OK response"));
    return true;
  }

  // Log everything else (or gate by level if you want)
  ALOG_INF(PSTR(D_LOG_SIM7000G "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\rparse_ATCommands %d>> ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n\rBuffer[%d] \"%s\""), response_loglevel, buflen, buffer);

  // 0) If we are waiting for the body line (after +CMGR or +CMT), consume it first.
  //    This must be first so "hello123" does not fall through as an unknown line.
  if (sms2_waiting_body)
  {
    return ATParse__BodyLine(buffer, buflen, response_loglevel);
  }

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

bool mSIM7000G::ATParse__CMTI(char* line, uint16_t buflen, uint8_t response_loglevel)
{
  // +CMTI: "SM",<index>
  // Meaning: SMS stored in memory. You must issue AT+CMGR=<index> to read it.
  // Then AT+CMGD=<index> to delete it.
  if (!line) return false;

  char* p = strstr(line, "+CMTI:");
  if (!p) return false;

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: URC CMTI (stored SMS): \"%s\""), p);

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
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: CMTI queued idx=%d"), idx);

  // Forward-looking: you can optionally trigger a read here (fire-and-forget),
  // but you asked to keep this generic, so we only queue.
  return true;
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

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: CMT header: %s"), p);

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
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: RX from=%s body=\"%s\""), from, body);
  SMS_Handle_IncomingText(from, body);

  // If this was a CMGR read, delete using the pending index (if set by your read logic)
  if (sms2_wait_kind == SMS2_WAIT_CMGR_BODY)
  {
    if (sms2_cmgr_pending_idx >= 0)
    {
      // Deleting is a “setup/process” action; blocking 1-2s is fine.
      SendAT_F(2000, PSTR("+CMGD=%d"), (int)sms2_cmgr_pending_idx);
      ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: deleted idx=%d"), (int)sms2_cmgr_pending_idx);
      sms2_cmgr_pending_idx = -1;
    }
  }

  // Reset state
  sms2_header_line[0] = '\0';
  sms2_wait_kind = SMS2_WAIT_NONE;

  return true;
}

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
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: CMGL queued idx=%d"), idx);
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

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: CPMS: %s"), p);
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

#endif // ENABLE_FEATURE_SIM__SMS










#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS

void mSIM7000G::GPS_Enable()
{
  if(modem)
  {
    // Set SIM7000G GPIO4 LOW ,turn on GPS power
    // CMD:AT+SGPIO=0,4,1,1
    // Only in version 20200415 is there a function to control GPS power
    modem->sendAT("+SGPIO=0,4,1,1");
    if(modem->waitResponse(10000L) != 1) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,1 false "));
    }

    modem->enableGPS();
    gps.enabled = true;
  }
}


void mSIM7000G::GPS_Disable()
{
  // Set SIM7000G GPIO4 LOW ,turn off GPS power
  // CMD:AT+SGPIO=0,4,1,0
  // Only in version 20200415 is there a function to control GPS power
  modem->sendAT("+SGPIO=0,4,1,0");
  if (modem->waitResponse(10000L) != 1) {
    ALOG_INF(PSTR(D_LOG_CELLULAR " SGPIO=0,4,1,0 false "));
  }
  modem->disableGPS();
  gps.enabled = false;
}


void mSIM7000G::ModemUpdate_GPS()
{
  if(mTime::TimeReached(&gps.tReached_Update, 1000)) // Not sending any faster so no point reading it faster
  {
    if(gps.enabled)
    {
      
      if(modem->getGPS_Detailed(
        &gps.latitude, 
        &gps.longitude, 
        &gps.speed, 
        &gps.altitude, 
        &gps.vsat, 
        &gps.usat, 
        &gps.accuracy, 
        &gps.year, 
        &gps.month, 
        &gps.day, 
        &gps.hour, 
        &gps.minute, 
        &gps.second, 
        &gps.course, 
        &gps.FixMode, 
        &gps.accuracy_position, 
        &gps.accuracy_vertical, 
        &gps.usat_glonass, 
        &gps.cno_max, 
        &gps.HPA, 
        &gps.VPA
      )) 
      {
        // ALOG_INF(PSTR(D_LOG_CELLULAR "GPS u/v_sat %d/%d Fix (%d cm)"), gps.usat, gps.vsat, (int)(gps.accuracy*100));
        
        #ifdef USE_MODULE_SENSORS_GPS_MODEM
        tkr_gps->readings.update_seconds = millis();
        tkr_gps->location.latitude  = gps.latitude;
        tkr_gps->location.longitude = gps.longitude;
        tkr_gps->location.speed = gps.speed;
        tkr_gps->location.altitude = gps.altitude;
        tkr_gps->location.accuracy = gps.accuracy;
        ALOG_INF(PSTR("CEL: Updating devices location (%d)"), (int)(tkr_gps->location.accuracy*100));
        #endif // USE_MODULE_SENSORS_GPS_MODEM

        #ifdef ENABLE_DEVFEATURE__TIME_UPDATE_WITH_GPS_TIME
        uint16_t minimum_update_seconds = 0; //always
        tkr_time->SystemTime_Update(gps.year, gps.month, gps.day, gps.hour, gps.minute, gps.second, minimum_update_seconds);
        #endif
      
      }else{
        ALOG_ERR(PSTR(D_LOG_CELLULAR "GPS NO FIX"));
        // GPS_Enable(); //need a backoff timer but otherwise should force it
      }

            
        
    }
  }

}




// https://www.google.com/maps?ll=xx.x,-x.xx
void mSIM7000G::SMS_GPSLocation()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  tkr_mavlink->pkt.battery_status.data.battery_remaining, tkr_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), tkr_mavlink->pkt.mission_current.data.seq, tkr_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-tkr_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.accuracy,2,convf_fix);
  
  sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}


// https://www.google.com/maps?ll=xx.x,-x.xx
void mSIM7000G::SMS_GPSLocationAuto()
{
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = (float)tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
  Serial.println(mavlink_lat);
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = (float)tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  tkr_mavlink->pkt.battery_status.data.battery_remaining, tkr_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), tkr_mavlink->pkt.mission_current.data.seq, tkr_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-tkr_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  /**
   * @brief GPS Modem
   **/
  char convf_lat[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
  char convf_lon[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
  char convf_fix[TBUFFER_SIZE_FLOAT];
  mSupport::float2CString(gps.accuracy,2,convf_fix);
  
  sms_text.Append_P(PSTR("AutoSend %d secs\n"),  smsauto_gps_messages.rate_seconds);
  sms_text.Append_P(PSTR("f%s m\n"),  convf_fix);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_lat, convf_lon);


  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}



void mSIM7000G::SMS_BatteryDetailed()
{

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending Battery"));

  CStringWriter sms_text(STANDARD_SMS_CHAR_LENGTH);

  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  float mavlink_lat = tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
  char convf_mavlink_lat[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lat);

  float mavlink_lon = tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;
  char convf_mavlink_lon[TBUFFER_SIZE_FLOAT]; 
  mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_mavlink_lon);

  sms_text.Append_P(PSTR("b%d%% c%d\n"),  tkr_mavlink->pkt.battery_status.data.battery_remaining, tkr_mavlink->pkt.battery_status.data.current_consumed);
  sms_text.Append_P(PSTR("WP i%d %dm\n"), tkr_mavlink->pkt.mission_current.data.seq, tkr_mavlink->pkt.nav_controller_output.data.wp_dist);
  sms_text.Append_P(PSTR("%dms\n"),       millis()-tkr_mavlink->pkt.tSaved_Last_Response);
  sms_text.Append_P(PSTR("https://www.google.com/maps/dir//%s,%s\n"), convf_mavlink_lat, convf_mavlink_lon);

  #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
  
  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_text[%d] \"%s\""), sms_text.length(), sms_text.data());
  
  bool res = modem->sendSMS(SMS_TARGET, String(sms_text.data()));
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

}



// void mSIM7000G::SMS_GPSLocation()
// {
//   if(modem)
//   {
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
//     // --------TESTING SENDING SMS--------

//     // https://www.google.com/maps/dir//54.505,-6.299
//     // https://www.google.com/maps/dir//54.505044,-6.298891

//     char convf_lat[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
//     char convf_lon[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
//     char convf_fix[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(gps.accuracy,2,convf_fix);

//     uint16_t buflen = 0;
//     char     buffer[STANDARD_SMS_CHAR_LENGTH] = {0};

// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
//       PSTR(
//         "Batt %d%% %dmV\n"
//         "WP  %d (%dm)\n"
//         "PKT Age  %d\n"
//         "Accuracy %s m\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       tkr_mavlink->pkt.battery_status.data.battery_remaining,
//       tkr_mavlink->pkt.battery_status.data.current_consumed,

//       tkr_mavlink->pkt.mission_current.data.seq,
//       tkr_mavlink->pkt.nav_controller_output.data.wp_dist,

//       millis()-tkr_mavlink->pkt.tSaved_Last_Response,
//       convf_fix,
//       convf_lat, 
//       convf_lon
//     );
//     // buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//     //   PSTR(
//     //     "Accuracy %s m\n"
//     //     "https://www.google.com/maps/dir//%s,%s"
//     //   ), 
//     //   convf_fix,
//     //   convf_lat, 
//     //   convf_lon
//     // );
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferA%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
//     /**
//      * @brief MAVLink Data
//      **/    
//     float mavlink_lat = tkr_mavlink->pkt.gps_raw_int.data.lat/10000000;
//     float mavlink_lon = tkr_mavlink->pkt.gps_raw_int.data.lon/10000000;


//     char convf_lat2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
//     char convf_lon2[TBUFFER_SIZE_FLOAT];
//     mSupport::float2CString(mavlink_lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lat2 \"%s\""),convf_lat2);
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "convf_lon2 \"%s\""), convf_lon2);
// // DEBUG_LINE_HERE;
//     buflen += sprintf_P(buffer+buflen, //sizeof(buffer),
//       PSTR(
//         "\n"
//         "MAV\n"
//         "https://www.google.com/maps/dir//%s,%s"
//       ), 
//       convf_lat2, 
//       convf_lon2
//     );
//     #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
// //     ALOG_INF(PSTR(D_LOG_CELLULAR "bufferB%d \"%s\""), buflen, buffer);
// // DEBUG_LINE_HERE;
//     ALOG_INF(PSTR(D_LOG_CELLULAR "buffer \"%s\""), buffer);
    
// // DEBUG_LINE_HERE;
//     String res;
//     res = modem->sendSMS(SMS_TARGET, String(buffer));
//     ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");

// // DEBUG_LINE_HERE;
//   }
// }

void mSIM7000G::SMS_Send_TimedHeartbeat()
{
  DEBUG_LINE_HERE;
  if(modem)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
    // --------TESTING SENDING SMS--------

    // https://www.google.com/maps/dir//54.505,-6.299
    // https://www.google.com/maps/dir//54.505044,-6.298891

  DEBUG_LINE_HERE;
    char convf_lat[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    char convf_lon[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    char convf_fix[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(gps.accuracy,2,convf_fix);

  DEBUG_LINE_HERE;
    uint16_t buflen = 0;
    char     buffer[STANDARD_SMS_CHAR_LENGTH];

  DEBUG_LINE_HERE;
    buflen += snprintf_P(buffer+buflen, sizeof(buffer),
      PSTR(
        // "Battery  %d mV\n"
        // "Battery  %d mA\n"
        // "Mission  %d (%dm)\n"
        // "PKT Age  %d\n"
        "Timed Heartbeat\n"
        "Accuracy %s m\n"
        "\n"
        "https://www.google.com/maps/dir//%s,%s"
      ), 
      // tkr_mavlink->pkt.battery_status.data.battery_remaining,
      // tkr_mavlink->pkt.battery_status.data.current_consumed,
      // tkr_mavlink->pkt.mission_current.data.seq,
      // tkr_mavlink->pkt.nav_controller_output.data.wp_dist,
      // millis()-tkr_mavlink->pkt.tSaved_Last_Response,
      convf_fix,
      convf_lat, 
      convf_lon
    );

  DEBUG_LINE_HERE;
    #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
    /**
     * @brief MAVLink Data
     **/    
    char convf_lat2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(tkr_mavlink->pkt.gps_raw_int.data.lat,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat2);
    char convf_lon2[TBUFFER_SIZE_FLOAT];
    mSupport::float2CString(tkr_mavlink->pkt.gps_raw_int.data.lon,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon2);

    // buflen += snprintf_P(buffer+buflen, sizeof(buffer),
    //   PSTR(
    //     "\n"
    //     "MAV\n"
    //     "https://www.google.com/maps/dir//%s,%s"
    //   ), 
    //   convf_lat2, 
    //   convf_lon2
    // );
    #endif // USE_MODULE__DRIVERS_MAVLINK_DECODER
    
  DEBUG_LINE_HERE;
    ALOG_INF(PSTR(D_LOG_CELLULAR "buffer  %s"),buffer);
    
  DEBUG_LINE_HERE;
    String res;
    res = modem->sendSMS(SMS_TARGET, String(buffer));
  DEBUG_LINE_HERE;
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS:"), res ? "OK" : "fail");
  DEBUG_LINE_HERE;

  }
}

#endif // USE_MODULE_NETWORK_CELLULAR_MODEM_GPS






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
        // ALOG_INF(PSTR(D_LOG_CELLULAR "CCID: %s"), ccid.c_str());

        // String imei = modem->getIMEI();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "IMEI: %s"), imei.c_str());

        // String cop = modem->getOperator();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "Operator: %s"), cop.c_str());

        // IPAddress local = modem->localIP();
        // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());

      } 
      else 
      {
        GPRS_UpdateConnectionState(false);
        ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS status: NOT connected Downtime: %d secs"), gprs.downtime_secs);
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
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_CELLULAR_CONNECTION_ESTABLISHED));
  DEBUG_LINE_HERE;
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_NETWORK_CONNECTION_ESTABLISHED));
  DEBUG_LINE_HERE;
      gprs.reconnect_init_counts++;
    }
    else
    {
  DEBUG_LINE_HERE;
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_CELLULAR_CONNECTION_LOST));
  DEBUG_LINE_HERE;
      tkr->function_event_queue.push_back(tkr->FunctionEvent(TASK_NETWORK_CONNECTION_LOST));
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

  ALOG_INF(PSTR("mSIM7000G::Modem__Running"));
  bool modem_responded = false;
  modem_responded = modem->testAT(wait_millis);
  ALOG_INF(PSTR("modem_responded = %d"), modem_responded);
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
 * @brief Commands required after restart
 * 
 */
bool mSIM7000G::DataNetwork__InitConfig()
{
  
  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__InitConfig()"));


    //Set mobile operation band
    modem->sendAT("+CBAND=ALL_MODE");
    modem->waitResponse();

    // Args:
    // 1 CAT-M
    // 2 NB-IoT
    // 3 CAT-M and NB-IoT
    // Set network preferre to auto
    modem->setPreferredMode(3);

    // Args:
    // 2 Automatic
    // 13 GSM only
    // 38 LTE only
    // 51 GSM and LTE only
    // Set network mode to auto
    modem->setNetworkMode(2);

    uint32_t  timeout = millis();
    // Check network signal and registration information
    // ALOG_INF(PSTR(D_LOG_CELLULAR "> SIM7000/SIM7070 uses automatic mode to access the network. The access speed may be slow. Please wait patiently"));
    RegStatus status;
    timeout = millis();
    do {
      int16_t sq =  modem->getSignalQuality();

      status = modem->getRegistrationStatus();

      if (status == REG_DENIED) {
        ALOG_INF(PSTR(D_LOG_CELLULAR "The SIM card you use has been rejected by the network operator"));
        return false;
      } else {
        ALOG_INF(PSTR(D_LOG_CELLULAR "Signal %d dBm, %d, pause %d"), (int)GetSignalQualityPower(sq), status, 10000-(timeout-millis()) );
      }

      if (millis() - timeout > 10000 ) { //!! THIS IS BLOCKING CODE, NEEDS RESOLVED TO RETRY AGAIN
        if (sq == 99) {
          ALOG_INF(PSTR(D_LOG_CELLULAR "It seems that there is no signal."));
          return false;
        }
        timeout = millis();
      }

      delay(50);
      // DEBUG_LINE_HERE;
    } while (status != REG_OK_HOME && status != REG_OK_ROAMING);


    String res;

    ALOG_INF(PSTR(D_LOG_CELLULAR "Obtain the APN issued by the network"));
    modem->sendAT("+CGNAPN");
    if (modem->waitResponse(3000, res) == 1) 
    {
      res = res.substring(res.indexOf(",") + 1);
      res.replace("\"", "");
      res.replace("\r", "");
      res.replace("\n", "");
      res.replace("OK", "");
      ALOG_INF(PSTR(D_LOG_CELLULAR "The APN issued by the network is: %s"), res.c_str());
    }

    modem->sendAT("+CNACT=1");
    modem->waitResponse();

    modem->sendAT("+CNACT?");
    // modem->waitResponse();
    if (modem->waitResponse("+CNACT: ") == 1) 
    {
      modem->stream.read();
      modem->stream.read();
      res = modem->stream.readStringUntil('\n');
      res.replace("\"", "");
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR(D_LOG_CELLULAR "The current network IP address is: %s"), res.c_str());
    }

    
    gprs.enabled = true;

    return true;

}

/**
 * @brief Commands required to connect
 * 
 */
bool mSIM7000G::DataNetwork__StartConnection()
{

  ALOG_HGL(PSTR(D_LOG_CELLULAR "DataNetwork__StartConnection()"));

  if(modem)
  {

    ALOG_INF(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Waiting for CELL TOWER network..."));
    if (!modem->waitForNetwork())  // Sim to cell tower connection, not the gprs
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "RECONNECTING GPRS_Connect Timeout?? I need to connect here??Waiting for network..."));
      // if this was false, I should really restart the cell connection
      // SimNetwork_Connect(); // might take long, so I need a "re-enable"
      return false;
    }

    if (modem->isNetworkConnected()) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "Network connected"));
    }else{
      
    }

    
    modem->gprsDisconnect();
    delay(50);


    ALOG_INF(PSTR(D_LOG_CELLULAR "Connecting to: %s"), CELLULAR_APN);
    bool gprs_connection_successful = modem->gprsConnect(CELLULAR_APN, CELLULAR_GPRS_USERNAME, CELLULAR_GPRS_PASSWORD);

    if(gprs_connection_successful)
    {
      ALOG_INF(PSTR("modem->gprsConnect                 YEEEEEEEEEEEEEEEEEEEES"));
      gprs.apn_connect_called++;
    }else{
      ALOG_INF(PSTR("modem->gprsConnect                 NOOOOOOOOOOOOOOOOOOOOT"));

    }


    if (!gprs_connection_successful) 
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS !gprs_connection_successful..."));
      return false;
    }

    ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Checking..."));
    if (modem->isGprsConnected()) 
    {
      DEBUG_LINE_HERE;
      #ifdef ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      ALOG_INF(PSTR(D_LOG_CELLULAR "GPRS Connected"));
      ALOG_HGL(PSTR("Start MQTTConnection with Cellular"));

      /**
       * @brief The idea is on a connection, the new connection should be added to the network interface
       * For now, set the mqtt directly
       **/
      if(gsm_client != nullptr)
      {
        gsm_client->stop();
        // gsm_client->setTimeout
        delay(50); //flush time
      }
      if(gsm_client == nullptr)
      {
        gsm_client = new TinyGsmClient(*modem); // Only create new if it didnt already exist
      }

      tkr_mqtt->CreateConnection(gsm_client, MQTT_HOST_CELLULAR, MQTT_PORT_CELLULAR, CLIENT_TYPE_CELLULAR_ID, MQTT_HOST__USERNAME, MQTT_HOST__PASSWORD);
        
      #endif // ENABLE_DEVFEATURE_MQTT_USING_CELLULAR
      
      DEBUG_LINE_HERE;
      GPRS_UpdateConnectionState(true);
      DEBUG_LINE_HERE;


    } 
    else 
    {
      ALOG_HGL(PSTR(D_LOG_CELLULAR "GPRS NOT Connected: REINIT THE LAZY WAY"));
      
      // Init();
    }

    // String ccid = modem->getSimCCID();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "CCID: %s"), ccid.c_str());

    // String imei = modem->getIMEI();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "IMEI: %s"), imei.c_str());

    // String cop = modem->getOperator();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Operator: %s"), cop.c_str());

    // IPAddress local = modem->localIP();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Local IP: %s"), String(local).c_str());

    // int csq = modem->getSignalQuality();
    // ALOG_INF(PSTR(D_LOG_CELLULAR "Signal quality: %d"), csq);

  }

    String res;
    modem->sendAT("+CPSI?");
    // modem->waitResponse();
    if (modem->waitResponse("+CPSI: ") == 1) 
    {
      res = modem->stream.readStringUntil('\n');
      res.replace("\r", "");
      res.replace("\n", "");
      modem->waitResponse();
      ALOG_INF(PSTR(D_LOG_CELLULAR "The current network parameter is: %s"), res.c_str());
    }


    return true;
}






#endif