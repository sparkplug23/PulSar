#include "_WebServer.h"

#ifdef USE_MODULE_NETWORK_WEBSERVER

// ============================================================================
// Page__Console.cpp
//
// Platform partitioning (explicit and easy to follow)
//
//   ESP32:
//     - WebSocket console handlers + WS service
//     - Polling console handlers (testing only)
//
//   ESP8266 (and others):
//     - Polling console handlers only (lightweight page mapped to PAGE_console)
// ============================================================================


// ============================================================================
// ESP32
// ============================================================================
#ifdef ESP32


// -----------------------------------------------------------------------------
// WebSocket Console (ESP32)
// -----------------------------------------------------------------------------
#ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET

void mWebServer::HandlePage_Console_WebSocket(AsyncWebServerRequest *request)
{

  fConsole_active = true;


  AsyncWebServerResponse *response =
    request->beginResponse_P(
      200,
      "text/html",
      PAGE_console_ws,
      PAGE_console_ws_length
    );

  response->addHeader(F("Content-Encoding"), F("gzip"));
  response->addHeader(F("Cache-Control"), F("no-store"));

  request->send(response);
}


// -----------------------------------------------------------------------------
// WebSocket Console2: push any NEW log lines since last send
// -----------------------------------------------------------------------------
// Date Modified: 02Jan26
//
// PURPOSE
//   Streams Tasmota-style "web_log" content to the Console2 WebUI over WebSocket.
//
// IMPORTANT DETAILS (matches your logging ring format)
//   - tkr_log->web_log is a delimited char buffer storing many log entries.
//   - Each entry is stored as: [index byte][text ...]['\1']
//   - tkr_log->web_log_index is the NEXT index to be used (i.e., current "head").
//   - Index 0 is reserved/invalid (used as terminator rules), so we skip 0.
//
// CURSOR RULES
//   - wsConsole2LastIdx == 0 means "not synced yet" (first connect / reset).
//     On first call we align wsConsole2LastIdx to current head and DO NOT dump history.
//   - Thereafter we walk indices from wsConsole2LastIdx up to (but not including) head,
//     collecting any lines that exist in the buffer.
//   - At end we always align wsConsole2LastIdx = head, even if nothing was added,
//     to prevent repeated re-walking.
//
// SENDING
//   - If 'client' is provided, send only to that client (and respect queueLength()).
//   - Else, broadcast to all clients.
//
// NOTES
//   - If GetLog() returns empty while idx is advancing, it usually means
//     the ring buffer has already dropped those indices due to overflow/eviction,
//     or the producer gate flags (fConsole_active / fConsole_history) prevented
//     those log lines from being captured into web_log in the first place.
// -----------------------------------------------------------------------------
void mWebServer::sendConsoleWs(AsyncWebSocketClient *client /*= nullptr*/)
{
  // ---------------------------------------------------------------------------
  // Guards: websocket must exist and have at least one connected client.
  // ---------------------------------------------------------------------------
  if (!websocket_console) return;

  if (!websocket_console->count()) {
    // Useful during debugging; can be commented later.
    // Serial.println("console2ws: no clients");
    return;
  }

  // Current "head" index of the web log ring.
  // This is the next value the logger will write.
  const uint8_t head_idx = tkr_log->web_log_index;

  // ---------------------------------------------------------------------------
  // First sync behaviour:
  //   - Align cursor to head
  //   - Do NOT dump history (keeps WS light and avoids large initial burst)
  // ---------------------------------------------------------------------------
  if (wsConsoleLastIdx == 0) {
    wsConsoleLastIdx = head_idx;
    // Serial.printf("console2ws: sync lastIdx=%u\n\r", (unsigned)wsConsole2LastIdx);
    return;
  }

  // Nothing new since last push.
  if (wsConsoleLastIdx == head_idx) {
    // Serial.printf("console2ws: no new logs (%u)\n\r", (unsigned)head_idx);
    return;
  }

  // ---------------------------------------------------------------------------
  // Build a payload containing as many NEW log lines as fit into 'out'.
  // This is intentionally stack-based (no heap allocations).
  // ---------------------------------------------------------------------------
  char   out[1400];
  size_t pos     = 0;
  bool   need_nl = false;

  // Start from the last cursor and walk forward until we reach head.
  uint8_t  idx   = wsConsoleLastIdx;
  uint16_t tried = 0;   // how many indices we attempted
  uint16_t added = 0;   // how many lines we actually appended

  while (idx != head_idx) {
    char*  line = nullptr;
    size_t len  = 0;

    // GetLog() searches for an entry with the index byte == idx.
    // If that entry has already been evicted from the ring, it returns nullptr/0.
    tkr_log->GetLog(idx, &line, &len);
    tried++;

    if (line && len) {
      // Ensure we have enough room for: optional '\n' + line bytes.
      // (We keep a small safety margin by reserving +2.)
      if (pos + len + 2 >= sizeof(out)) {
        // If truncation happens often, reduce WS interval, increase buffer,
        // or send multiple frames per call.
        // Serial.println("console2ws: tx buffer full, truncating");
        break;
      }

      if (need_nl) out[pos++] = '\n';
      memcpy(out + pos, line, len);
      pos += len;
      need_nl = true;
      added++;
    }

    // Advance index, skipping 0 (reserved).
    idx++;
    if (idx == 0) idx = 1;
  }

  // ---------------------------------------------------------------------------
  // Always align cursor to the head after walking.
  // This prevents repeatedly re-walking old indices in case of gaps/eviction.
  // ---------------------------------------------------------------------------
  wsConsoleLastIdx = head_idx;

  // If we walked indices but appended nothing, GetLog() did not find any entries.
  // Typical reasons:
  //   - ring buffer evicted those indices due to size pressure
  //   - logging gate flags prevented capture into web_log
  //   - idx bookkeeping mismatch vs how web_log_index is inserted
  if (pos == 0) {
    // Serial.printf("console2ws: walked %u slots, got 0 lines; head=%u\n\r",
    //               (unsigned)tried, (unsigned)head_idx);
    return;
  }

  // ---------------------------------------------------------------------------
  // Transmit:
  //   - if single client mode, only send if its TX queue is empty
  //   - else broadcast
  // ---------------------------------------------------------------------------
  if (client) {
    if (client->queueLength() == 0) {
      client->text(out, pos);
    } else {
      // This is not an error; it simply means the browser is not draining fast enough.
      // Serial.println("console2ws: client queue busy, drop");
    }
  } else {
    websocket_console->textAll(out, pos);
  }

  // Serial.printf("console2ws: sent %u lines (%u bytes) head=%u\n\r",
  //               (unsigned)added, (unsigned)pos, (unsigned)head_idx);
}


// -----------------------------------------------------------------------------
// WebSocket: Console2 event handler
// -----------------------------------------------------------------------------
// Date Modified: 02Jan26
//
// PURPOSE
//   Handles the /ws_console2 WebSocket endpoint used by /console2.htm.
//   This is intentionally a free function (not a class member) so it can be
//   registered with AsyncWebSocket::onEvent() without std::function, lambdas,
//   or trampolines. This keeps it deterministic (no heap alloc, no lifetime
//   coupling to a C++ object), and mirrors the WLED/Tasmota style.
//
// BEHAVIOUR
//   - WS_EVT_CONNECT:
//       * Acknowledge connection.
//       * Reset the console cursor so the next periodic push can resync.
//   - WS_EVT_DISCONNECT:
//       * Clear live-client tracking if it was this client.
//   - WS_EVT_DATA (single-frame WS_TEXT only):
//       * Heartbeat: "p" -> "pong"
//       * Otherwise treat payload as a raw JSON command.
//
// COMMAND INJECTION MODES
//   A) Normal (non-delayed):
//       - Uses shared data_buffer with locking, then immediately calls
//         Tasker_Interface(TASK_JSON_COMMAND_ID).
//       - Preferred for simplicity if JSON/log pipelines are already correctly
//         lock-protected everywhere.
//
//   B) Delayed (ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING):
//       - Avoids touching the shared JSON/mqtt buffer by copying the WS payload
//         into a dedicated heap buffer (pending_cmd). The main loop later parses
//         it in a safe context.
//       - This exists to avoid timing overlap where other producers overwrite
//         shared buffers between the WS callback and the later parse.
//
// NOTES
//   - This function does NOT push log frames itself. Log streaming should be
//     done by handleConsole2Ws() at a controlled rate, to avoid flooding WS
//     queues and starving the networking task.
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
// WebSocket: Console2 event handler (free function)
// -----------------------------------------------------------------------------
// Date Modified: 02Jan26
//
// This handler services /ws_console2 for the Console2 WebUI.
// It is intentionally NOT a class method so it can be registered
// directly with AsyncWebSocket::onEvent() without lambdas or trampolines.
//
// The owning mWebServer instance is resolved ONCE at entry via tkr_web
// and then used through the local `web` pointer.
//
// Supports two execution paths:
//   1) Immediate execution using shared data_buffer (locked)
//   2) Delayed execution using PendingJsonCommand (heap-backed)
// -----------------------------------------------------------------------------

void mWebServer::wsEventConsole(AsyncWebSocket *server,
                     AsyncWebSocketClient *client,
                     AwsEventType type,
                     void *arg,
                     uint8_t *data,
                     size_t len)
{

  // ---------------------------------------------------------------------------
  // CONNECT
  // ---------------------------------------------------------------------------
  if (type == WS_EVT_CONNECT) {
    DEBUG_PRINTLN(F("WS console client connected"));
    client->text(F("ACK connected"));

    wsConsoleLastIdx = 0;   // force resync on next periodic push
    return;
  }

  // ---------------------------------------------------------------------------
  // DISCONNECT
  // ---------------------------------------------------------------------------
  if (type == WS_EVT_DISCONNECT) {
    DEBUG_PRINTLN(F("WS console client disconnected"));
    if (client->id() == wsConsoleLiveClientId) {
      wsConsoleLiveClientId = 0;
    }
    return;
  }

  // ---------------------------------------------------------------------------
  // DATA
  // ---------------------------------------------------------------------------
  if (type != WS_EVT_DATA) return;

  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (!info || !info->final || info->index != 0 || info->len != len) return;
  if (info->opcode != WS_TEXT) return;

  // Heartbeat
  if (len > 0 && len < 10 && data[0] == 'p') {
    client->text(F("pong"));
    return;
  }

  // Bounds
  if (len == 0 || len >= DATA_BUFFER_PAYLOAD_MAX_LENGTH) {
    client->text(F("ERR len"));
    return;
  }

  // ---------------------------------------------------------------------------
  // DELAYED JSON MODE (heap-backed, no shared buffer touch)
  // ---------------------------------------------------------------------------
#ifdef ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING

  if (pending_cmd.has()) {
    client->text(F("BUSY"));
    return;
  }

  if (!pending_cmd.create((uint16_t)len)) {
    client->text(F("ERR mem"));
    return;
  }

  memcpy(pending_cmd.ptr, data, len);
  pending_cmd.ptr[len] = '\0';

  client->text(F("OK"));
  return;

#else
  // ---------------------------------------------------------------------------
  // IMMEDIATE MODE (shared buffer + lock)
  // ---------------------------------------------------------------------------

  if (!data_buffer.requestLock(GetModuleUniqueID())) {
    client->text(F("BUSY"));
    return;
  }

  data_buffer.ClearSoft();
  data_buffer.payload.length_used = (uint16_t)len;
  memcpy(data_buffer.payload.ctr, data, len);
  data_buffer.payload.ctr[len] = '\0';

  // Execute immediately
  tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);

  data_buffer.releaseLock();

  client->text(F("OK"));
  return;

#endif
}


// -----------------------------------------------------------------------------
// WebSocket Console: periodic service loop
// -----------------------------------------------------------------------------
// Date Modified: 02Jan26
//
// PURPOSE
//   This must be called regularly (TASK_LOOP or similar).
//   It does two jobs:
//
//   1) Gates web-ring logging capture:
//        Your logger only appends to web_log when tkr_fConsole_active==true.
//        With Console2 using WebSocket (not HTTP polling), nothing else will
//        automatically toggle that flag, so we enable it here whenever a WS
//        client is connected.
//
//      Without this, web_idx will still increment for Serial, but web_log will
//      not capture the lines, so GetLog() returns empty.
//
//   2) Pushes new log deltas over WS at a controlled interval.
//      Uses WS_CONSOLE2_INTERVAL as a rate limit to avoid flooding.
// -----------------------------------------------------------------------------
void mWebServer::handleConsoleWs()
{
  // Guard: feature may be compiled out or websocket not created
  if (!websocket_console) return;

  // ---------------------------------------------------------------------------
  // Enable/disable web-log capture based on live WS clients.
  // This is the critical piece for WS mode (poll mode used to imply "active").
  // ---------------------------------------------------------------------------
  const bool active_now = (websocket_console->count() > 0);
  #ifndef ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING
  tkr_fConsole_active = active_now; // Only enable turning off console logging if not in conflict with console1
  #endif

  // If no clients, do nothing else.
  if (!active_now) return;

  // ---------------------------------------------------------------------------
  // Rate limit WS pushes (simple elapsed-ms check).
  // ---------------------------------------------------------------------------
  const unsigned long now = millis();
  if ((unsigned long)(now - wsConsoleLastPushTime) < (unsigned long)WS_CONSOLE_INTERVAL) return;
  wsConsoleLastPushTime = now;

  // ---------------------------------------------------------------------------
  // Maintain WS client list and push delta logs.
  // ---------------------------------------------------------------------------
  websocket_console->cleanupClients();

  // Broadcast mode (delta sender uses wsConsole2LastIdx cursor)
  sendConsoleWs();
}


#endif // ENABLE_DEVFEATURE_NETWORK__CONSOLE_WEBSOCKET



// -----------------------------------------------------------------------------
// Polling Console (ESP32) - testing only
// -----------------------------------------------------------------------------
#ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING

void mWebServer::HandlePage_Console_Poll(AsyncWebServerRequest *request)
{
  fConsole_active = true;

  if (request->hasParam("c2")) { HandleConsoleRefresh(request); return; }

  ALOG_INF(PSTR("Loading: Console Poll"));

  if (handleIfNoneMatchCacheHeader(request, 200)) return;

  AsyncWebServerResponse *response =
    request->beginResponse_P(200, "text/html", PAGE_console_polling, PAGE_console_polling_length);

  response->addHeader("Content-Encoding", "gzip");
  setStaticContentCacheHeaders(response);
  request->send(response);
}

void mWebServer::HandleConsoleRefresh(AsyncWebServerRequest *request)
{

  // -------- Pass 1: Handle Web Console Command (c1) --------
  if (request->hasParam("c1")) {

    const String cmd = request->arg("c1");
    const size_t len = cmd.length();

    if (len && len < DATA_BUFFER_PAYLOAD_MAX_LENGTH) {

      #ifdef ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING
      if (data_buffer.tryLock(GetModuleUniqueID())) {
      #else
      if (data_buffer.requestLock(GetModuleUniqueID())) {
      #endif

        data_buffer.ClearSoft();
        data_buffer.payload.length_used = (uint16_t)len;
        memcpy(data_buffer.payload.ctr, cmd.c_str(), len);
        data_buffer.payload.ctr[len] = '\0';

        #ifdef ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING        
        data_buffer.delayedJSONCommandWaiting = true;
        #else
        tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);
        data_buffer.releaseLock();
        #endif

      } else {
        ALOG_WRN(PSTR("WebConsole c1: buffer busy, command ignored"));
      }

    } else if (len) {
      ALOG_ERR(PSTR("WebConsole c1: payload too large (%u)"), (unsigned)len);
    }
  }



  // Parse c2 as uint8 (Tasmota-style). 0 means "initial sync"
  uint8_t counter = 0;
  {
    char stmp[12] = {0};
    WebGetArg(request, "c2", stmp, sizeof(stmp));   // stable even with _cb present
    if (stmp[0]) {
      const int v = atoi(stmp);
      if (v > 0 && v < 256) counter = (uint8_t)v;
      Serial.printf("Initial Sync c2=%d\n\r",counter);
    }
  }
  
  Serial.printf("Sync c2=%d\n\r",counter);
  // Stream reply (no shared JSON buffer)
  AsyncResponseStream *response = request->beginResponseStream("text/plain");

  const uint8_t web_idx = tkr_log->web_log_index;

  // Header: "<idx>}1<reset_flag>}1"
  response->printf("%u}1%u}1", (unsigned)web_idx, (unsigned)reset_web_log_flag);

  // One-shot reset flag (same semantics as you had)
  if (!reset_web_log_flag) {
    reset_web_log_flag = true;
    counter = 0;
  }

  // If counter==0: sync only (no history dump)
  if (counter == 0) {
    response->print("}1");
    request->send(response);
    return;
  }

  // If nothing new
  if (counter == web_idx) {
    response->print("}1");
    request->send(response);
    return;
  }

  bool need_newline = false;

  // Walk forward until we reach the current index
  while (counter != web_idx) {
    char*  line = nullptr;
    size_t len  = 0;

    tkr_log->GetLog(counter, &line, &len);

    if (line && len) {
      if (need_newline) response->write('\n');
      response->write((const uint8_t*)line, len);
      need_newline = true;
    }

    counter++;
    if (counter == 0) counter = 1; // skip 0 (matches your “0 not allowed” rule)
  }

  response->print("}1");
  request->send(response);
}

#endif // ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING


// End ESP32
// ============================================================================
#else  // ESP8266 (and others)


// ============================================================================
// ESP8266 (Polling only, lightweight page mapped to PAGE_console)
// ============================================================================
#ifdef ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING

void mWebServer::HandlePage_Console_Poll(AsyncWebServerRequest *request)
{
  fConsole_active = true;

  if (request->hasParam("c2")) { HandleConsoleRefresh(request); return; }

  ALOG_INF(PSTR("Loading: Console Poll"));

  if (handleIfNoneMatchCacheHeader(request, 200)) return;

  AsyncWebServerResponse *response =
    request->beginResponse_P(200, "text/html", PAGE_console_polling, PAGE_console_polling_length);

  response->addHeader("Content-Encoding", "gzip");
  setStaticContentCacheHeaders(response);
  request->send(response);
}

void mWebServer::HandleConsoleRefresh(AsyncWebServerRequest *request)
{

  // -------- Pass 1: Handle Web Console Command (c1) --------
  if (request->hasParam("c1")) {

    const String cmd = request->arg("c1");
    const size_t len = cmd.length();

    if (len && len < DATA_BUFFER_PAYLOAD_MAX_LENGTH) {

      #ifdef ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING
      if (data_buffer.tryLock(GetModuleUniqueID())) {
      #else
      if (data_buffer.requestLock(GetModuleUniqueID())) {
      #endif

        data_buffer.ClearSoft();
        data_buffer.payload.length_used = (uint16_t)len;
        memcpy(data_buffer.payload.ctr, cmd.c_str(), len);
        data_buffer.payload.ctr[len] = '\0';

        #ifdef ENABLE_FEATURE_WEBSERVER__DELAYED_JSONLOCKED_COMMAND_PROCESSING        
        data_buffer.delayedJSONCommandWaiting = true;
        #else
        tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);
        data_buffer.releaseLock();
        #endif

      } else {
        ALOG_WRN(PSTR("WebConsole c1: buffer busy, command ignored"));
      }

    } else if (len) {
      ALOG_ERR(PSTR("WebConsole c1: payload too large (%u)"), (unsigned)len);
    }
  }




  // Parse c2 as uint8 (Tasmota-style). 0 means "initial sync"
  uint8_t counter = 0;
  {
    char stmp[12] = {0};
    WebGetArg(request, "c2", stmp, sizeof(stmp));   // stable even with _cb present
    if (stmp[0]) {
      const int v = atoi(stmp);
      if (v > 0 && v < 256) counter = (uint8_t)v;
      // Serial.printf("Initial Sync c2=%d\n\r",counter);
    }
  }
  
  // Serial.printf("Sync c2=%d\n\r",counter);
  // Stream reply (no shared JSON buffer)
  AsyncResponseStream *response = request->beginResponseStream("text/plain");

  const uint8_t web_idx = tkr_log->web_log_index;

  // Header: "<idx>}1<reset_flag>}1"
  response->printf("%u}1%u}1", (unsigned)web_idx, (unsigned)reset_web_log_flag);

  // One-shot reset flag (same semantics as you had)
  if (!reset_web_log_flag) {
    reset_web_log_flag = true;
    counter = 0;
  }

  // If counter==0: sync only (no history dump)
  if (counter == 0) {
    response->print("}1");
    request->send(response);
    return;
  }

  // If nothing new
  if (counter == web_idx) {
    response->print("}1");
    request->send(response);
    return;
  }

  bool need_newline = false;

  // Walk forward until we reach the current index
  while (counter != web_idx) {
    char*  line = nullptr;
    size_t len  = 0;

    tkr_log->GetLog(counter, &line, &len);

    if (line && len) {
      if (need_newline) response->write('\n');
      response->write((const uint8_t*)line, len);
      need_newline = true;
    }

    counter++;
    if (counter == 0) counter = 1; // skip 0 (matches your “0 not allowed” rule)
  }

  response->print("}1");
  request->send(response);
}

#endif // ENABLE_DEVFEATURE_NETWORK__CONSOLE_POLLING

#endif // ESP32



#endif // USE_MODULE_NETWORK_WEBSERVER

