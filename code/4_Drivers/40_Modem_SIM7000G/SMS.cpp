#include "_mSIM7000G.h"



#ifdef USE_MODULE_DRIVERS_MODEM_7000G


#ifdef ENABLE_FEATURE_SIM__SMS


int mSIM7000G::SendAT_WaitResponse__SMSNumberStored(uint32_t timeout_ms)
{

  // ALOG_INF(PSTR("SendAT_WaitResponse__SMSNumberStored"));

  // CPMS? returns: +CPMS: "SM",used,max,"SM",used,max,"SM",used,max

  modem->sendAT("+CPMS?");

  // Wait until the "+CPMS:" token is seen (TinyGSM will have consumed it)
  if (modem->waitResponse(timeout_ms, "+CPMS:") != 1)
  {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CPMS query failed"));
    return -1;
  }

  // Read remainder of the line after "+CPMS:"
  String tail = modem->stream.readStringUntil('\n');
  tail.replace("\r", "");
  tail.trim();

  // tail is expected to be: "\"SM\",0,10,\"SM\",0,10,\"SM\",0,10"
  int used = -1;
  int max  = -1;

  // Accept either:
  // 1) full line including prefix (if it ever happens)
  // 2) tail-only line (normal with waitResponse(token))
  if (tail.startsWith("+CPMS:"))
  {
    if (sscanf(tail.c_str(), "+CPMS: \"%*[^\"]\",%d,%d", &used, &max) != 2)
    {
      ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CPMS parse failed: %s"), tail.c_str());
      return -1;
    }
  }
  else
  {
    if (sscanf(tail.c_str(), "\"%*[^\"]\",%d,%d", &used, &max) != 2)
    {
      ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CPMS parse failed: %s"), tail.c_str());
      return -1;
    }
  }

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS stored: \t\t\t\t ======== %d / %d"), used, max);
  return used;
}


bool mSIM7000G::SendAT_WaitResponse__SMSDeleteAll(uint32_t timeout_ms)
{
  if (!modem) return false;

  // Make sure we are in text mode (safe, cheap)
  modem->sendAT("+CMGF=1");
  modem->waitResponse(500);

  // Preferred, single-command delete if supported
  // Mode 4 = delete ALL messages from all read statuses
  modem->sendAT("+CMGD=1,4");
  int r = modem->waitResponse(timeout_ms);

  if (r == 1)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: all stored messages deleted (CMGD=1,4)"));
    return true;
  }

  // Fallback: some firmware rejects mode=4, so brute-force by index
  ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS: CMGD=1,4 failed, falling back to index loop"));

  modem->sendAT("+CPMS?");
  if (modem->waitResponse(timeout_ms, "+CPMS:") != 1)
  {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CPMS query failed"));
    return false;
  }

  String line = modem->stream.readStringUntil('\n');
  line.trim();

  int used = 0;
  if (sscanf(line.c_str(), "+CPMS: \"%*[^\"]\",%d", &used) != 1)
  {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CPMS parse failed"));
    return false;
  }

  for (int i = 1; i <= used; i++)
  {
    modem->sendAT("+CMGD=", i);
    modem->waitResponse(500); // ignore per-index errors
  }

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: all stored messages deleted (loop)"));
  return true;
}


void mSIM7000G::SMS_NormaliseText(char* s)
{
  if (!s) return;

  // Trim leading
  while (*s && isspace((unsigned char)*s)) {
    memmove(s, s + 1, strlen(s));
  }

  // Trim trailing
  size_t n = strlen(s);
  while (n > 0 && isspace((unsigned char)s[n - 1])) {
    s[n - 1] = '\0';
    n--;
  }

  // Lowercase
  for (char* p = s; *p; ++p) {
    *p = (char)tolower((unsigned char)*p);
  }
}

bool mSIM7000G::SMS_Handle_IncomingText(const char* from_number, const char* body)
{
  if (!from_number || !body) return false;

  char msg[200];
  snprintf(msg, sizeof(msg), "%s", body);
  SMS_NormaliseText(msg);

  if (strcmp(msg, "hello") == 0)
  {
    char reply[160];
    snprintf(reply, sizeof(reply), "System: %s", tkr_set->Settings.system_name.device);
    ALOG_HGL(PSTR(D_LOG_CELLULAR "SMS CMD: hello -> reply system name"));
    return SMS_SendText(from_number, reply);
  }

  return false;
}



bool mSIM7000G::SMS_SendText(const char* to_number, const char* body)
{
  if (!modem || !to_number || !body) return false;

  // Ensure text mode
  modem->sendAT("+CMGF=1");
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS_SendText: CMGF=1 failed"));
    return false;
  }

  // CMGS flow requires '>' prompt then Ctrl+Z (0x1A)
  modem->sendAT("+CMGS=\"", to_number, "\"");
  if (modem->waitResponse(5000, ">") != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS_SendText: no prompt"));
    return false;
  }

  modem->streamWrite(body);
  modem->streamWrite((char)0x1A); // Ctrl+Z terminator

  // Wait for +CMGS: and OK (SIM7000 can take a while)
  int r = modem->waitResponse(30000L, "+CMGS:");
  if (r != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS_SendText: CMGS failed r=%d"), r);
    return false;
  }
  r = modem->waitResponse(30000L, "OK");
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS_SendText: OK r=%d"), r);
  return (r == 1);
}

void mSIM7000G::SMS_Enable()
{
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS_Enable"));
  delay(2000);

  if (!modem) return;

  SendATCommand_FunctionalityMode_Full();      // you already have this
  SendATCommand_SMSFormatAscii();              // CMGF=1
  SendATCommand_SMSImmediateForwardOverSerial(); // CNMI=3,2,0,0,0

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS_Enable ================================"));
  // sms.enabled = true;
}

void mSIM7000G::SMS_Disable()
{
  
  // sms.enabled = false;
}


// void mSIM7000G::ModemUpdate_SMS()
// {
  


//   if(mTime::TimeReached(&sms.tReached_Update, 1000))
//   {
//     if(sms.enabled)
//     {
//       // Although SMS should be forwarded, lets also check if any are waiting and read them out

//       /**
//        * @brief Check for waiting SMS messages
//        **/
//       SendAT_ATParseResponse_F(200, LOG_LEVEL_DEBUG_MORE, PSTR("+CMGD=?"));

//     }
//   }

// }


#ifdef ENABLE_FEATURE_SIM__SMS

void mSIM7000G::SubTask_SMS__Read_Any_Waiting_Messages()
{
  if (sms.messages_incoming_index_list.empty()) return;

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  ALOG_INF(PSTR(D_LOG_CELLULAR "SubTask_SMS__Read_Any_Waiting_Messages -- START"));
  #endif

  if (!modem) return;

  // Ensure text mode for predictable CMGR format
  // modem->sendAT("+CMGF=1");
  // modem->waitResponse(1000);

  for (auto& id : sms.messages_incoming_index_list)
  {
    // Build a local CMGR block: "+CMGR: ...\r\n<body>\r\n"
    char cmgr_block[512];
    cmgr_block[0] = '\0';
    size_t used = 0;

    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: CMGR index=%d"), (int)id);

    // Issue CMGR
    sms2_cmgr_pending_idx = (int)id;     // <-- ADD THIS LINE
    modem->sendAT("+CMGR=", (int)id);

    // We now need to read the modem stream directly and assemble the CMGR block.
    // Minimal, robust approach:
    // - wait for "+CMGR:" line
    // - read header line
    // - read body line
    // - stop on "OK" (or timeout)

    uint32_t t0 = millis();
    bool got_cmgr = false;
    bool got_body = false;

    while (millis() - t0 < 3000)
    {
      // This reads one line (TinyGSM stream is a Stream)
      String line = modem->stream.readStringUntil('\n'); // includes \r sometimes
      if (!line.length()) { delay(5); continue; }

      // Normalise line endings to CRLF in our block
      line.replace("\r", "");
      line.trim(); // keep content, remove whitespace around

      if (!got_cmgr)
      {
        if (line.startsWith("+CMGR:"))
        {
          got_cmgr = true;

          int n = snprintf(cmgr_block + used, sizeof(cmgr_block) - used,
                           "%s\r\n", line.c_str());
          if (n > 0) used += (size_t)n;
        }
        continue;
      }

      // After header, next non-empty line is body (in text mode)
      if (got_cmgr && !got_body)
      {
        // Body can be empty, but usually not; accept empty too.
        got_body = true;

        int n = snprintf(cmgr_block + used, sizeof(cmgr_block) - used,
                         "%s\r\n", line.c_str());
        if (n > 0) used += (size_t)n;
        continue;
      }

      // After body, we expect OK or ERROR
      if (line == "OK" || line.startsWith("+CMS ERROR") || line == "ERROR")
      {
        break;
      }
    }

    if (got_cmgr)
    {
      ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: CMGR block built (%u bytes) %s"), (unsigned)used, cmgr_block);

      // Feed block into your existing parser so your "+CMGR:" handler runs.
      // NOTE: parse_ATCommands expects a mutable char*.
      parse_ATCommands(cmgr_block, (uint16_t)strlen(cmgr_block), LOG_LEVEL_INFO);

      // Delete
      if (SendAT_F(2000, PSTR("+CMGD=%d"), (int)id))
      {
        ALOG_INF(PSTR(D_LOG_CELLULAR "SMS index %d deleted"), (int)id);
      }
      else
      {
        ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS index %d delete FAILED"), (int)id);
      }
    }
    else
    {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS: CMGR did not return header for index=%d"), (int)id);
    }
  }

  sms.messages_incoming_index_list.clear();

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SubTask_SMS__Read_Any_Waiting_Messages -- END"));
  #endif
}

#endif
#ifdef ENABLE_FEATURE_SIM__SMS

void mSIM7000G::SubTask_SMS__Check_Any_SMS_Saved()
{
  if (!modem) return;

  // Ensure text mode (idempotent)
  modem->sendAT("+CMGF=1");
  modem->waitResponse(200); // short, just to avoid backlog; OK if it times out

  // Ask modem to list unread messages.
  // Handler_ModemResponses() will receive +CMGL: lines and queue indices via parse_ATCommands().
  modem->sendAT("+CMGL=\"REC UNREAD\"");
  // Do NOT read stream here. Forever listener will catch it.
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: CMGL request sent (REC UNREAD)"));
}

#endif




void mSIM7000G::SendATCommand_SMSFormatAscii()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=1"); // Set the ascii messages (not HEX)
  bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSFormatAscii %d"), result);
}


void mSIM7000G::SendATCommand_SMSFormatPDU()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CMGF=0"); // Set the ascii messages (not HEX)
  bool result = modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSFormatPDU %d"), result);
}


/**
 * @brief Make it so new SMS messages are send over serial automatically
 * +CNMI: <mode>,<mt>,<bm>,<ds>,<bfr>
 * The read command parameters and their defined values are the following:
  <mode>
    0 – Do not forward unsolicited result codes to the Terminal Equipment (TE) (default)
    3 – Forward unsolicited result codes directly to the TE
  <mt>
    0 – No received message notifications, the modem acts as an SMS client
    2 – SMS-DELIVERs (except class 2 and message waiting indication group) are routed directly to the TE
  <bm>
    No CBM notifications are routed to the TE
  <ds>
    0 – No SMS-STATUS-REPORTs are routed to the TE
    1 – SMS-STATUS-REPORTs are routed to the TE using unsolicited result code: +CDS: <length><CR><LF><pdu>
  <bfr>
    1 – The buffer of unsolicited result codes is cleared when <mode> 1 to 3 is entered
**/
void mSIM7000G::SendATCommand_SMSImmediateForwardOverSerial()
{
  // Enable SMS always send to serial when they arrive, move to function later
  modem->sendAT("+CNMI=3,2,0,0,0");
  bool result = modem->waitResponse(AT_COMMAND_RESPONSE_TIMEOUT__CNMI);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMSImmediateForwardOverSerial %d"), result);
}



#ifdef USE_MODULE_NETWORK_CELLULAR_MODEM_GPS
void mSIM7000G::AutoSMS_Messages_Handle()
{

  /**
   * @brief Message1: Send GPS if requested
   * 
   */
  if(smsauto_gps_messages.rate_seconds>0) //if it set to auto
  {

    if(mTime::TimeReached(&smsauto_gps_messages.tSaved_LastSent, smsauto_gps_messages.rate_seconds*1000))
    {
      SMS_GPSLocationAuto();
    }

  }


}
#endif



/**
 * @brief New method that takes in SMS formatted as "Command:Key:Value" so test case is Command:SMS_GPS_Auto:1 or Command:SMS_GPS_Something:"string", ie key is assumed string but value must have its type added, "string", number, [array]
 * 
 * @param sms_command 
 */
void mSIM7000G::SMS_CommandIntoJSONCommand(char* sms_command)
{

  /**
   * @brief SMS message will contain the repeat rate, and its handled in another call (in seconds)
   * 
   */
  ALOG_INF(PSTR(D_LOG_CELLULAR "sms_command >>%s<<"), sms_command);

  const char* delims = ":"; //space + , "GPSAuto 123" where the space splits the command and number
  char* tok = strtok(sms_command, delims); // I believe all delims are replaced by NULL?

  if(tok)
  {  
    // for (int i=0;i<5;i++)
    // {
    //   ALOG_INF(PSTR("tok[%d] = %s"), i, tok?tok:"ERROR");
    //   tok = strtok(NULL, delims);
    // }


    // tok = strtok(NULL, delims); // "Command" skip
    
    tok = strtok(NULL, delims); // Consume first split string and advance token (i.e. skip Command)
    char key[100];
    snprintf(key, sizeof(key), tok);
    ALOG_INF(PSTR(D_LOG_CELLULAR "key=%s"),key);

    tok = strtok(NULL, delims);     
    char value[100];
    snprintf(value, sizeof(value), tok);
    ALOG_INF(PSTR(D_LOG_CELLULAR "value=%s"),value);

    // Read into local
    data_buffer.ClearDeep();

    sprintf(data_buffer.payload.ctr, "{\"%s\":%s}", key, value);

    data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

    ALOG_COM( PSTR(DEBUG_INSERT_PAGE_BREAK  "SMS->JsonCommandBuffer = \"%d|%s\""), data_buffer.payload.length_used, data_buffer.payload.ctr);

    tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);

    ALOG_INF(PSTR(D_LOG_CELLULAR "JsonCommand Finished"));

  }

    // tok




}





#ifdef ENABLE_FEATURE_SIM__SMS

void mSIM7000G::SMS_Enable_Basic()
{
  if (!modem) return;

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: basic enable"));

  // Text mode
  modem->sendAT("+CMGF=1");
  if (modem->waitResponse(2000) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CMGF failed"));
    return;
  }

  // Immediate forward to serial
  modem->sendAT("+CNMI=3,2,0,0,0");
  if (modem->waitResponse(2000) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CNMI set failed"));
    return;
  }

  // Verify
  modem->sendAT("+CNMI?");
  if (modem->waitResponse(2000, "+CNMI:") != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS: CNMI verify failed"));
    return;
  }

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: ready (CNMI=3,2,0,0,0)"));
}

#endif
#ifdef ENABLE_FEATURE_SIM__SMS

void mSIM7000G::ATParse_CMT(char* buffer)
{
  if (!buffer) return;
  if (strncmp(buffer, "+CMT:", 5) != 0) return;

  char from[32];
  char body[160];

  if (!SMS_Parse_CMT(buffer, from, sizeof(from),
                     body, sizeof(body)))
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS CMT parse failed"));
    return;
  }

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS RX %s: %s"), from, body);

  SMS_SendText(from, "ACK");
}

bool mSIM7000G::SMS_Parse_CMT(const char* block,
                             char* out_from, size_t from_sz,
                             char* out_body, size_t body_sz)
{
  if (!block || !out_from || !out_body) return false;

  const char* hdr_end = strstr(block, "\r\n");
  if (!hdr_end) return false;

  const char* p = strchr(block, '"');
  if (!p) return false;
  const char* q = strchr(p + 1, '"');
  if (!q) return false;

  size_t len = q - (p + 1);
  if (len >= from_sz) len = from_sz - 1;
  memcpy(out_from, p + 1, len);
  out_from[len] = 0;

  const char* body = hdr_end + 2;
  const char* body_end = strstr(body, "\r\n");
  if (!body_end) body_end = body + strlen(body);

  len = body_end - body;
  if (len >= body_sz) len = body_sz - 1;
  memcpy(out_body, body, len);
  out_body[len] = 0;

  return true;
}

#endif


#ifdef ENABLE_FEATURE_SIM__SMS

void mSIM7000G::SMS2_Enable()
{
  sms2.init_done = true;
  sms2.init_done = false;
  sms2.pending_idx.clear();
  sms2.cmgr_inflight = false;
  sms2.cmgr_idx = -1;
  sms2.delete_pending = false;
  sms2.delete_idx = -1;
  sms2.t_last_cmgl_ms = 0;

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: enabled"));
}

void mSIM7000G::SMS2_Disable()
{
  sms2.init_done = false;
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: disabled"));
}

void mSIM7000G::SMS2_Init_NotifyOnly()
{
  if (!modem || !sms2.init_done) return;

  // Text mode
  modem->sendAT("+CMGF=1");
  if (modem->waitResponse(2000) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CMGF failed"));
    return;
  }

  // Select storage (SIM) - consistent with your CMTI "SM"
  modem->sendAT("+CPMS=\"SM\",\"SM\",\"SM\"");
  if (modem->waitResponse(2000) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CPMS failed"));
    return;
  }

  // Notify-only: store +CMTI, do NOT push +CMT
  // Recommended: +CNMI=3,1,0,0,0
  modem->sendAT("+CNMI=3,1,0,0,0");
  if (modem->waitResponse(2000) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CNMI set failed"));
    return;
  }

  // Verify (optional, but useful)
  modem->sendAT("+CNMI?");
  if (modem->waitResponse(2000, "+CNMI:") != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CNMI verify failed"));
    return;
  }

  sms2.init_done = true;
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: init done (CNMI notify-only)"));
}

void mSIM7000G::SMS2_QueueIndexUnique(int idx)
{
  if (idx < 0) return;

  for (auto& v : sms2.pending_idx) {
    if (v == idx) return;
  }
  sms2.pending_idx.push_back(idx);
}

void mSIM7000G::SMS2_Request_List_Unread()
{
  if (!modem || !sms2.init_done || !sms2.init_done) return;

  modem->sendAT("+CMGL=\"REC UNREAD\"");
  // Do not read the stream here. Handler_ModemResponses() will parse +CMGL lines.
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: CMGL REC UNREAD requested"));
}

void mSIM7000G::SMS2_Task_Tick(uint32_t now_ms)
{
  if (!modem || !sms2.init_done) return;

  // Init once when modem is ready enough (you decide the gate; simplest: modem exists)
  if (!sms2.init_done) {
    SMS2_Init_NotifyOnly();
    if (!sms2.init_done) return;
  }

  // Optional: periodic sweep to catch missed URCs (e.g., every 60s)
  if (now_ms - sms2.t_last_cmgl_ms > 60000UL) {
    sms2.t_last_cmgl_ms = now_ms;
    SMS2_Request_List_Unread();
  }

  // If we are waiting for CMGR to be parsed, timeout and recover
  if (sms2.cmgr_inflight) {
    if (now_ms - sms2.cmgr_t0_ms > 5000UL) {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: CMGR timeout idx=%d"), sms2.cmgr_idx);
      sms2.cmgr_inflight = false;
      sms2.cmgr_idx = -1;
    }
    return; // wait for parser to complete the CMGR
  }

  // If a delete is pending, execute delete now (fire-and-forget)
  if (sms2.delete_pending) {
    int idx = sms2.delete_idx;
    sms2.delete_pending = false;
    sms2.delete_idx = -1;

    if (SendAT_F(2000, PSTR("+CMGD=%d"), idx)) {
      ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: deleted idx=%d"), idx);
    } else {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: delete FAILED idx=%d"), idx);
    }
    // continue to next message next tick
    return;
  }

  // If we have pending indices, request CMGR for the next one
  if (!sms2.pending_idx.empty())
  {
    int idx = sms2.pending_idx.front();
    sms2.pending_idx.erase(sms2.pending_idx.begin());

    sms2.cmgr_inflight = true;
    sms2.cmgr_idx      = idx;
    sms2.cmgr_t0_ms    = now_ms;

    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: request CMGR idx=%d"), idx);
    modem->sendAT("+CMGR=", idx);
    // Handler_ModemResponses() will read +CMGR block and call parse_ATCommands(),
    // which must call SMS2_ATParse_Line() to complete this transaction.
    sms2_cmgr_pending_idx = idx;
    sms2_waiting_cmgr_body = false;
    sms2_cmgr_header[0] = '\0';

  }
}

#endif

bool mSIM7000G::SMS2_ExtractNumberFromCMGRHeader(const char* header, char* out_from, size_t out_from_sz)
{
  if (!header || !out_from || out_from_sz == 0) return false;
  out_from[0] = '\0';

  // +CMGR: "REC UNREAD","+4475...",,"date"
  const char* p = strchr(header, '"');         // "REC UNREAD"
  if (!p) return false;
  p = strchr(p + 1, '"');                      // end first quoted
  if (!p) return false;
  p = strchr(p + 1, '"');                      // start number quote
  if (!p) return false;
  const char* q = strchr(p + 1, '"');          // end number quote
  if (!q) return false;

  size_t n = (size_t)(q - (p + 1));
  if (n >= out_from_sz) n = out_from_sz - 1;
  memcpy(out_from, p + 1, n);
  out_from[n] = '\0';
  return true;
}



#ifdef ENABLE_FEATURE_SIM__SMS
void mSIM7000G::SMS2_ATParse_Line(char* buffer, uint16_t buflen, uint8_t response_loglevel)
{
  // Dispatcher for SMS-related URCs / responses.
  //
  // Expected inputs (single line at a time, from Handler_ModemResponses):
  //
  //   +CMTI: "SM",<idx>
  //     Meaning: a new SMS is STORED in memory (SIM/ME storage).
  //     Action: issue AT+CMGR=<idx> to read it, then delete it with AT+CMGD=<idx>.
  //
  //   +CMGL: <idx>,...
  //     Meaning: list entry for stored SMS during AT+CMGL.
  //     Action: queue index (optionally drain later).
  //
  //   +CMGR: ...
  //     Meaning: header line for a read SMS (AT+CMGR=<idx>).
  //     NOTE: body comes on the NEXT line, which is handled elsewhere (parse_ATCommands).
  //
  // IMPORTANT DESIGN CHOICE:
  // - We DO drain immediately on +CMTI by firing AT+CMGR=<idx>.
  // - We DO NOT handle CMGR header/body parsing here. parse_ATCommands owns that.
  // - We only set the minimal state needed for parse_ATCommands to delete correctly.

  (void)buflen;
  (void)response_loglevel;

  if (!buffer || buflen == 0) return;
  if (!sms2.init_done) return;

  // ---- CMTI: notify stored SMS index ----
  char* cmti = strstr(buffer, "+CMTI:");
  if (cmti)
  {
    // +CMTI: "SM",8
    const char* comma = strchr(cmti, ',');
    int idx = -1;
    if (comma) idx = atoi(comma + 1);

    if (idx >= 0)
    {
      // Queue (unique)
      SMS2_QueueIndexUnique(idx);
      ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: CMTI queued idx=%d"), idx);

      // Drain immediately: request the message.
      //
      // Response will arrive as:
      //   +CMGR: ...      (header line)
      //   <body text>     (next line)
      //   OK
      //
      // parse_ATCommands() handles:
      //   - seeing +CMGR: and arming "next line is body"
      //   - consuming body
      //   - deleting using sms2_cmgr_pending_idx
      sms2_cmgr_pending_idx = idx;   // REQUIRED for delete-on-body design
      sms2.cmgr_inflight    = true;
      sms2.cmgr_idx         = idx;

      SendAT_F(2000, PSTR("+CMGR=%d"), idx);
    }
    else
    {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: CMTI parse failed"));
    }
    return;
  }

  // ---- CMGL: list returns indices ----
  // Used when you explicitly request message listing (AT+CMGL=...).
  // We only queue indices here. Drain policy is elsewhere.
  char* cmgl = strstr(buffer, "+CMGL:");
  if (cmgl)
  {
    int idx = -1;
    if (sscanf(cmgl, "+CMGL: %d", &idx) == 1 && idx >= 0)
    {
      SMS2_QueueIndexUnique(idx);
      ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: CMGL queued idx=%d"), idx);
    }
    else
    {
      ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: CMGL parse failed"));
    }
    return;
  }

  // ---- CMGR header line ----
  // Do NOT handle here. parse_ATCommands owns CMGR header/body sequencing.
  // We return so no other SMS2 parsing triggers on this line.
  if (strstr(buffer, "+CMGR:"))
  {
    return;
  }

  // Any other SMS-related URCs you may later add:
  // +CDS / +CDSI (status report), +CUSD (USSD), etc.
}




void mSIM7000G::ATParse_CMGD__CommandNameInTextDeleteMessage(char* buffer, uint8_t buflen, uint8_t response_loglevel)
{

  char* result2 = strstr(buffer, "+CMGD:"); 
  if(result2)
  {
    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage S %s"),result2);
    #endif

    const char* delims = "()"; //space + ,
    char* tok = strtok(result2, delims); // I believe all delims are replaced by NULL?

    if(tok)
    {                 
      // 00:23 INF tok[0] = +CMGD: 
      // 00:23 INF tok[1] = 0,1,2,4,5
      // 00:23 INF tok[2] = ,
      // 00:23 INF tok[3] = 0-4
      // 00:23 INF tok[4] =       

      ALOG_DBG(PSTR(D_LOG_CELLULAR "MATCHED \"CMGD\" >>%s<<"), result2);

      tok = strtok(NULL, delims); //skip 

      if(tok)
      {

        char buffer[100];
        snprintf(buffer, sizeof(buffer), tok); // 0,1,2,4,5
        AddLog(response_loglevel,PSTR("hereeeeeeeeeeeeeeeeeeeeeeeeee buffer split = %s"), buffer);

        /**
         * @brief Splitting the Indexs out
         **/
        const char* delims2 = ","; //space + ,
        char* tok_ids = strtok(buffer, delims2); 
        uint8_t i = 0;
        sms.messages_incoming_index_list.clear();
        while(tok_ids){
          AddLog(response_loglevel,PSTR("CommandNameInTextDeleteMessage SMS IDs[%d] = %s"), i++, tok_ids?tok_ids:"ERROR");
          sms.messages_incoming_index_list.push_back(atoi(tok_ids));
          tok_ids = strtok(NULL, delims2);
        }

      }

    }
  }

    #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
    ALOG_INF(PSTR("ATParse_CMGD__CommandNameInTextDeleteMessage E"));
    #endif

}

#ifdef ENABLE_FEATURE_SIM__SMS
#ifdef ENABLE_FEATURE_SIM__SMS

bool mSIM7000G::SMS2_Init_NotifyOnly_Blocking()
{
  if (sms2.init_done) return true;

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: init (blocking)"));

  modem->sendAT("+CMEE=2");
  modem->waitResponse(1000);

  modem->sendAT("+CMGF=1");
  if (modem->waitResponse(2000) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CMGF failed"));
    return false;
  }

  modem->sendAT("+CPMS=\"SM\",\"SM\",\"SM\"");
  if (modem->waitResponse(3000) != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CPMS failed"));
    return false;
  }

  // Optional (don’t hard fail)
  modem->sendAT("+CSMS=1");
  modem->waitResponse(2000);

  // Capability probe so you can SEE what is allowed on this firmware
  modem->sendAT("+CNMI=?");
  modem->waitResponse(3000);   // just print/log whatever comes back in your normal stream

  // Try preferred notify-only first
  modem->sendAT("+CNMI=3,1,0,0,0");
  if (modem->waitResponse(3000) != 1)
  {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS2: CNMI 3,1 failed, trying mode=2"));

    // Fallback that is widely accepted on SIMCom
    modem->sendAT("+CNMI=2,1,0,0,0");
    if (modem->waitResponse(3000) != 1) {
      ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CNMI set failed (both mode=3 and mode=2)"));
      return false;
    }
  }

  modem->sendAT("+CNMI?");
  if (modem->waitResponse(3000, "+CNMI:") != 1) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS2: CNMI verify failed"));
    return false;
  }

  sms2.init_done = true;
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS2: init done"));
  return true;
}

#endif


#endif




#endif




#ifdef ENABLE_DEVFEATURE_SMS__REMOVE_OLD


void mSIM7000G::SMSReadAndEraseSavedSMS()
{

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- START"));
  #endif 

  for(auto& id:sms.messages_incoming_index_list)
  {
    if(SendAT_ATParseResponse_F(200, LOG_LEVEL_INFO, PSTR("+CMGR=%d"), id))
    {
      // modem->waitResponse(200);
      // delay(200);
      
      // Erase from memory
      if(SendAT_F(200, PSTR("+CMGD=%d"), id))
      {
        ALOG_INF(PSTR("SMSReadAndEraseSavedSMS %d deleted"), id);
      }else{
        ALOG_INF(PSTR("SMSReadAndEraseSavedSMS %d FAILED TO delete"), id);

      }
    }
  }

  sms.messages_incoming_index_list.clear(); // for now assumed its done

  #ifdef ENABLE_DEBUG_GROUP__CELLULAR_READ_SMS
  ALOG_DBG(PSTR(D_LOG_CELLULAR "SMSReadAndEraseSavedSMS -- END"));
  #endif

}


void mSIM7000G::SMS_PollAndDrainBacklog()
{
  if (!modem || !sms.enabled) return;

  ALOG_HGL(PSTR(D_LOG_CELLULAR "SMS backlog sweep (CMGL -> CMGR -> CMGD)"));

  // Ensure text mode for predictable parsing
  modem->sendAT("+CMGF=1");
  modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT);

  // List ALL messages (or change to "REC UNREAD" if you prefer)
  // NOTE: SIMCom returns multiple lines: +CMGL: <idx>,...
  modem->sendAT("+CMGL=\"ALL\"");

  // You MUST read/parse the stream responses and collect indices.
  // The simplest integration with your current architecture:
  // - rely on your existing parser hooks to capture indices into sms.messages_incoming_index_list
  // - then call SMSReadAndEraseSavedSMS()

  // Wait a bit for lines to arrive and be parsed by Handler_ModemResponses()
  // Keep this short; we are not blocking for minutes.
  const uint32_t t0 = millis();
  while (millis() - t0 < 2000)   // 2 seconds window to receive CMGL lines
  {
    Handler_ModemResponses(LOG_LEVEL_DEBUG_MORE);
    delay(5);
  }

  if (sms.messages_incoming_index_list.size())
  {
    SMSReadAndEraseSavedSMS();
  }
  else
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS backlog sweep: none"));
  }
}


void mSIM7000G::ModemUpdate_SMS()
{
  if (!sms.enabled) return;

  if (mTime::TimeReached(&sms.tReached_Update, 1000))
  {
    // If we have queued message indexes, read+process+delete them
    if (!sms.messages_incoming_index_list.empty())
    {
      SMSReadAndEraseSavedSMS();
    }
  }
}


bool mSIM7000G::SMS_ApplyCNMI_Forwarding()
{
  if (!modem) return false;

  modem->sendAT("+CMEE=2");          // verbose errors
modem->waitResponse(1000);
SMS_ApplyConfig();



  // 1) Ensure text mode (idempotent)
  modem->sendAT("+CMGF=1");
  if (modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT) != 1) {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS: CMGF set failed"));
    return false;
  }

  // 2) Select message storage explicitly (helps a lot on SIMCom)
  modem->sendAT("+CPMS=\"SM\",\"SM\",\"SM\"");
  if (modem->waitResponse(2000) != 1) {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS: CPMS set failed"));
    // not fatal, but commonly required; treat as failure for now
    return false;
  }

  // 3) Now try CNMI
  modem->sendAT("+CNMI=3,2,0,0,0");
  int r = modem->waitResponse(2000);

  if (r != 1) {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS: CNMI set failed (waitResponse=%d)"), r);
    return false;
  }

  // 4) Verify CNMI actually changed
  modem->sendAT("+CNMI?");
  if (modem->waitResponse(2000, "+CNMI:") != 1) {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS: CNMI verify failed"));
    return false;
  }

  return true;
}
bool mSIM7000G::SMS_ApplyConfig()
{
  if (!modem) return false;

  // Gate: SIM ready
  modem->sendAT("+CPIN?");
  if (modem->waitResponse(1000, "READY") != 1) return false;

  // Ensure text mode
  modem->sendAT("+CMGF=1");
  if (modem->waitResponse(1000) != 1) return false;

  // Storage
  modem->sendAT("+CPMS=\"SM\",\"SM\",\"SM\"");
  if (modem->waitResponse(2000) != 1) return false;

  // SMS service profile (SIMCom quirk)
  modem->sendAT("+CSMS=1");
  modem->waitResponse(1000); // don’t fail hard, but log if not OK

  // Now CNMI
  modem->sendAT("+CNMI=3,2,0,0,0");
  if (modem->waitResponse(2000) != 1) return false;

  // Verify
  modem->sendAT("+CNMI?");
  if (modem->waitResponse(1000, "+CNMI:") != 1) return false;

  return true;
}




void mSIM7000G::SMS_Send_TimedHeartbeat2()
{
  DEBUG_LINE_HERE;
  if(modem)
  {
    ALOG_INF(PSTR(D_LOG_CELLULAR "SMS: Sending GPS Location"));
        
    // --------TESTING SENDING SMS--------

    // https://www.google.com/maps/dir//54.505,-6.299
    // https://www.google.com/maps/dir//54.505044,-6.298891

  DEBUG_LINE_HERE;
    // char convf_lat[TBUFFER_SIZE_FLOAT];
    // mSupport::float2CString(gps.latitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lat);
    // char convf_lon[TBUFFER_SIZE_FLOAT];
    // mSupport::float2CString(gps.longitude,JSON_VARIABLE_FLOAT_PRECISION_LENGTH,convf_lon);
    // char convf_fix[TBUFFER_SIZE_FLOAT];
    // mSupport::float2CString(gps.accuracy,2,convf_fix);

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
      "convf_fix",
      "convf_lat", 
      "convf_lon"
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



void mSIM7000G::SMS_SendBootMessage()
{
  if (!modem || !sms.enabled) {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS_SendBootMessage skipped (modem or SMS disabled)"));
    return;
  }

  const char* dest = SMS_TARGET;
  if (!dest || !dest[0]) {
    ALOG_WRN(PSTR(D_LOG_CELLULAR "SMS_SendBootMessage no destination number"));
    return;
  }

  char msg[160];
  snprintf(msg, sizeof(msg),
           "BOOT OK: %s",
           tkr_set->Settings.system_name.device);

  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS boot -> %s : \"%s\""), dest, msg);

  // Ensure ASCII mode
  modem->sendAT("+CMGF=1");
  if (!modem->waitResponse(DEFAULT_AT_COMMAND_RESPONSE_WAIT)) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS CMGF failed"));
    return;
  }

  // Begin SMS
  modem->sendAT("+CMGS=\"%s\"", dest);
  if (!modem->waitResponse(">")) {
    ALOG_ERR(PSTR(D_LOG_CELLULAR "SMS CMGS prompt failed"));
    return;
  }

  // Message body + Ctrl-Z
  modem->stream.write(msg);
  modem->stream.write(0x1A);

  bool ok = modem->waitResponse(10000);
  ALOG_INF(PSTR(D_LOG_CELLULAR "SMS boot send result=%d"), ok);
}





bool mSIM7000G::SMS_Parse_CMGR(const char* cmgr_block,
                              char* out_from, size_t out_from_sz,
                              char* out_body, size_t out_body_sz)
{
  if (!cmgr_block || !out_from || !out_body) return false;

  out_from[0] = '\0';
  out_body[0] = '\0';

  // Find first line end
  const char* line_end = strstr(cmgr_block, "\r\n");
  if (!line_end) return false;

  // Copy header line
  char header[200];
  size_t hdr_len = (size_t)(line_end - cmgr_block);
  if (hdr_len >= sizeof(header)) hdr_len = sizeof(header) - 1;
  memcpy(header, cmgr_block, hdr_len);
  header[hdr_len] = '\0';

  // Extract number between second set of quotes
  // header example: +CMGR: "REC READ","+4475...",,"date"
  const char* p = strchr(header, '"');
  if (!p) return false;
  p = strchr(p + 1, '"');
  if (!p) return false;
  p = strchr(p + 1, '"');
  if (!p) return false;
  const char* q = strchr(p + 1, '"');
  if (!q) return false;

  size_t num_len = (size_t)(q - (p + 1));
  if (num_len >= out_from_sz) num_len = out_from_sz - 1;
  memcpy(out_from, p + 1, num_len);
  out_from[num_len] = '\0';

  // Body is the next line
  const char* body_start = line_end + 2;
  const char* body_end = strstr(body_start, "\r\n");
  if (!body_end) return false;

  size_t body_len = (size_t)(body_end - body_start);
  if (body_len >= out_body_sz) body_len = out_body_sz - 1;
  memcpy(out_body, body_start, body_len);
  out_body[body_len] = '\0';

  return true;
}

#endif // ENABLE_DEVFEATURE_SMS__REMOVE_OLD

#endif // ENABLE_FEATURE_SIM__SMS


#endif
