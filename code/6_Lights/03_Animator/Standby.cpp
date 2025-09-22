#include "mAnimatorLight.h"

#ifdef ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET

// =============================================================
// Standby.cpp — virtual preset (standby) + state snapshot utils
// =============================================================

// ---------- internal helpers ----------
static inline void freePtr(char*& p){ if(p){ free(p); p=nullptr; } }

// ---------- ODR constants ----------
const char mAnimatorLight::kStandbyProfilePath[] PROGMEM = "/lgt_standby.json";
const char mAnimatorLight::kStateSnapshotPath[]  PROGMEM = "/lgt_state.json";
#ifdef ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM
const char mAnimatorLight::kResumeSnapshotPath[] PROGMEM = "/lgt_state_resume.json";
#endif

// returns -1 if missing/parse error; parses a duplicate so it never mutates source
static int Standby_ReadTemplateId(const char* json, size_t len)
{
  if (!json || !len) return -1;
  char* dup = (char*)malloc(len + 1);
  if (!dup) return -1;
  memcpy(dup, json, len);
  dup[len] = '\0';

  DynamicJsonDocument d(len + 128);
  DeserializationError err = deserializeJson(d, dup);
  free(dup);
  if (err) return -1;

  JsonVariant v = d["template_id"];
  if (v.isNull()) return -1;
  return (int)v.as<int>();
}


bool mAnimatorLight::Standby_WriteProfileToFS(const char* json)
{
  if (!json) return false;

  size_t actual = strlen(json); // always trust the string
  char path[24]; strncpy_P(path, kStandbyProfilePath, sizeof(path)); path[sizeof(path)-1]=0;

  File f = FILE_SYSTEM.open(path, "w");
  if (!f) {
    ALOG_ERR(PSTR("WriteProfileToFS: open('%s','w') failed"), path);
    return false;
  }

  // (debug) you can keep or remove this
  ALOG_INF(PSTR("Writing\r\n%s\r\n"), json);

  size_t wr = f.write((const uint8_t*)json, actual);
  f.close();

  ALOG_INF(PSTR("WriteProfileToFS: wrote %u/%u to %s"), (unsigned)wr, (unsigned)actual, path);
  return wr == actual;
}


bool mAnimatorLight::Standby_Init()
{
  ALOG_INF(PSTR("Standby_Init: init----------------------------"));

  // 1) Load profile from FS (if any)
  bool haveFile = Standby_LoadProfileFromFS();
  ALOG_INF(PSTR("haveFile=%d profileLen=%u"), (int)haveFile, (unsigned)standby.profileLen);
  if (haveFile && standby.profileRAM && standby.profileLen > 0) {
    const size_t fsLen = standby.profileLen - 1; // exclude NUL
    ALOG_INF(PSTR("Standby_Init: FS raw (len=%u): '%s'"), (unsigned)fsLen, standby.profileRAM);
  }

  // 2) Read template_id from FS (if present)
  int8_t fsId = -1;
  if (haveFile && standby.profileRAM) {
    fsId = Standby_ReadTemplateId(standby.profileRAM, standby.profileLen - 1);
  }

#ifdef USE_STANDBY_TEMPLATE
  // 3) Compare with compile-time template id
  const int tplId = LIGHTING_TEMPLATE__PRESET_STANDBY_MODE_VERSION;
  ALOG_INF(PSTR("Standby_Init: fsId=%d tplId=%d"), (int)fsId, tplId);

  // 4) If no FS or mismatch, overwrite FS from template + injected template_id
  if (!haveFile || fsId < 0 || fsId != tplId) {
    if (!Load_StandbyTemplate_Into_ProfileRAM(/*injectTemplateId=*/true)) return false;
    Standby_WriteProfileToFS(standby.profileRAM);   // persist from RAM
    Standby_LoadProfileFromFS();                       // single source of truth in RAM
    return true;
  }

  // 6) FS profile is valid & up-to-date
  ALOG_INF(PSTR("Standby_Init: using existing FS profile (template_id=%d)"), (int)fsId);
  return true;

#else
  // No template compiled: ensure we at least have a minimal FS profile
  if (!haveFile) {
    static const char kDefault[] PROGMEM = "{\"BrightnessRGB\":100,\"version\":1}";
    const size_t defLen = strlen_P(kDefault);
    char def[defLen + 1];
    memcpy_P(def, kDefault, defLen);
    def[defLen] = '\0';

    Standby_WriteProfileToFS(def, 0);
    Standby_LoadProfileFromFS();
    ALOG_INF(PSTR("Standby_Init: created default FS profile (no template compiled)"));
  } else {
    ALOG_INF(PSTR("Standby_Init: using existing FS profile (no template compiled)"));
  }
  return true;
#endif
}


bool mAnimatorLight::Standby_SetProfileFromJson(const char* json, STBY_SRC src)
{
  if (!json) return false;

  // Validate the incoming payload first (so we don't poison RAM/FS on failure)
  if (!ValidateJSON(json)) {
    ALOG_ERR(PSTR("Standby_SetProfileFromJson: invalid JSON"));
    return false;
  }

  const size_t len = strlen(json);
  char* buf = (char*)malloc(len + 1);
  if (!buf) return false;

  memcpy(buf, json, len);
  buf[len] = '\0';

  // Adopt to RAM
  free(standby.profileRAM);
  standby.profileRAM = buf;
  standby.profileLen = len + 1;
  standby.last_src   = src;
  standby.last_ver   = Standby_ReadTemplateId(buf, len); // may be -1 (user JSON)

  // Mirror to FS (use the overload you actually have)
  // If your impl is Standby_WriteProfileToFS(const char*):
  Standby_WriteProfileToFS(standby.profileRAM);
  // If it's the two-arg version that ignores len when 0:
  // Standby_WriteProfileToFS(standby.profileRAM, 0);

  ALOG_INF(PSTR("Standby profile saved (source=%s template_id=%d)"),
           StbySrcName(src), standby.last_ver);
  return true;
}


bool mAnimatorLight::Standby_Start(uint16_t /*fadeMs*/, uint8_t callMode)
{
  // Ensure we have a standby profile available (RAM or FS→RAM)
  if (!standby.profileRAM && !Standby_Init()) {
    ALOG_ERR(PSTR("Standby_Start: no standby profile available"));
    return false;
  }

  // Take a runtime snapshot before switching away (into RAM)
  if (!Standby_CaptureResumeToRAM()) {
    ALOG_ERR(PSTR("Standby_Start: failed to capture resume snapshot"));
    return false;
  }
  ALOG_INF(PSTR("Standby_Start: resume snapshot captured (%u bytes)"),
           (unsigned)standby.resumeLen);

  // Optional: mirror the snapshot to FS for inspection/debug (and safety)
  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM
  {
    const bool okFS = FileSave__State(/*includeBounds*/true,
                                      /*includeBri*/true,
                                      /*selectedOnly*/false,
                                      /*fullGlobals*/false);
    ALOG_INF(PSTR("Standby_Start: snapshot mirrored to FS = %s"), okFS ? "OK" : "FAIL");
  }
  #endif

  // Apply the standby profile
  const char* srcJson = standby.profileRAM;
  const size_t lenToApply = (standby.profileLen > 0) ? (standby.profileLen - 1) : 0;
  if (!srcJson || lenToApply == 0) {
    ALOG_ERR(PSTR("Standby_Start: standby profile is empty"));
    return false;
  }

  const bool ok = Standby_JsonCommand_Run(srcJson, lenToApply, callMode);
  standby.active       = ok;
  standby.callModeLast = callMode;

  ALOG_INF(PSTR("Standby_Start: %s"), ok ? "OK" : "FAILED");
  return ok;
}

bool mAnimatorLight::Standby_Stop(uint16_t /*fadeMs*/, uint8_t callMode)
{
  if (!standby.active) {
    ALOG_INF(PSTR("Standby_Stop: not active; nothing to restore"));
    return true;
  }
  if (!standby.haveResumeRAM || !standby.resumeRAM) {
    ALOG_ERR(PSTR("Standby_Stop: no resume snapshot in RAM"));
    return false;
  }

  const char* src = standby.resumeRAM;
  const size_t len = (standby.resumeLen > 0) ? (standby.resumeLen - 1) : 0;
  if (!src || len == 0) {
    ALOG_ERR(PSTR("Standby_Stop: resume snapshot is empty"));
    return false;
  }

  const bool ok = Standby_JsonCommand_Run(src, len, callMode);
  if (ok) {
    standby.active = false;
    ALOG_INF(PSTR("Standby_Stop: restored previous state (OK)"));
  } else {
    ALOG_ERR(PSTR("Standby_Stop: failed to restore previous state"));
  }
  return ok;
}

// ---------- source-name helper ----------
const char* mAnimatorLight::StbySrcName(STBY_SRC s){
  switch(s){
    case STBY_SRC::Template: return "template";
    case STBY_SRC::Uploaded: return "uploaded";
    case STBY_SRC::Current:  return "current";
    case STBY_SRC::FS:       return "fs";
    default:                 return "unknown";
  }
}

bool mAnimatorLight::ValidateJSON(const char* json_str)
{
  // quick check for valid JSON
  if (!json_str) return false;
  // Must be copied to a mutable buffer before parsing
  char buffer[strlen(json_str) + 1];
  strcpy(buffer, json_str);
  JsonParser parser(buffer);
  JsonParserObject obj = parser.getRootObject();   
  if (!obj) {
    ALOG_ERR(PM_DESERIALIZATION_ERROR);
    return false;
  }
  return true;
}






// ---------- standby profile FS I/O ----------
bool mAnimatorLight::Standby_LoadProfileFromFS()
{
  ALOG_INF(PSTR("Standby_LoadProfileFromFS>>>>>>>>>>>>>>>>>>>>>"));
  char path[24]; strncpy_P(path, kStandbyProfilePath, sizeof(path)); path[sizeof(path)-1]=0;
  File f = FILE_SYSTEM.open(path, "r");
  if (!f) { ALOG_WRN(PSTR("Standby_LoadProfileFromFS: open('%s','r') failed"), path); return false; }
  size_t len = f.size();
  char* buf = (char*)malloc(len+1);
  if (!buf){ f.close(); ALOG_ERR(PSTR("Standby_LoadProfileFromFS: OOM %u"), (unsigned)len); return false; }
  size_t rd = f.readBytes(buf, len);
  f.close();
  buf[rd] = '\0';
  freePtr(standby.profileRAM);

  bool isaValid = ValidateJSON(buf);//, rd);
  if (!isaValid) {
    ALOG_ERR(PSTR("Standby_LoadProfileFromFS: invalid JSON in '%s'"), path);
    free(buf);
    return false;
  }

  standby.profileRAM = buf;
  standby.profileLen = rd+1;
  ALOG_INF(PSTR("Standby_LoadProfileFromFS: %s"), buf);
  ALOG_INF(PSTR("Standby_LoadProfileFromFS: %u bytes"), (unsigned)rd);
  ALOG_INF(PSTR("Standby_LoadProfileFromFS<<<<<<<<<<<<<<<<<<<<<"));
  return true;
}


bool mAnimatorLight::Standby_SaveProfileToFS()
{
ALOG_INF(PSTR("SaveProfileToFS: Saving RAM into FS as debug"));
  if (!standby.profileRAM || standby.profileLen==0) {
    ALOG_WRN(PSTR("SaveProfileToFS: no profile in RAM"));
    return false;
  }
  return Standby_WriteProfileToFS(standby.profileRAM);
}
bool mAnimatorLight::Load_StandbyTemplate_Into_ProfileRAM(bool injectTemplateId)
{
  const int tplId = LIGHTING_TEMPLATE__PRESET_STANDBY_MODE_VERSION;

  // Copy raw template with a little headroom for ,"template_id":NNN}
  size_t tplLen = strlen_P(LIGHTING_TEMPLATE__PRESET_STANDBY_MODE);
  const size_t extra = 24; // safe overhead for splice
  char* buf = (char*)malloc(tplLen + 1 + extra);
  if (!buf) {
    ALOG_ERR(PSTR("Load_StandbyTemplate_Into_ProfileRAM: OOM (%u)"), (unsigned)(tplLen + 1 + extra));
    return false;
  }
  memcpy_P(buf, LIGHTING_TEMPLATE__PRESET_STANDBY_MODE, tplLen);
  buf[tplLen] = '\0';

  // Validate template as-is (lightweight)
  if (!ValidateJSON(buf)) {
    ALOG_ERR(PSTR("Load_StandbyTemplate_Into_ProfileRAM: template JSON invalid"));
    free(buf);
    return false;
  }

  if (injectTemplateId) {
    // Trim trailing whitespace/newlines from raw string literal
    size_t end = tplLen;
    while (end > 0 && (buf[end-1] == ' ' || buf[end-1] == '\t' || buf[end-1] == '\r' || buf[end-1] == '\n')) --end;

    if (end == 0 || buf[end-1] != '}') {
      ALOG_WRN(PSTR("Load_StandbyTemplate_Into_ProfileRAM: no closing '}', keeping template as-is"));
    } else {
      // Decide if there are existing members to choose comma or not
      bool hasContent = false;
      size_t brace = 0; while (brace < end && buf[brace] != '{') ++brace;
      for (size_t i = brace + 1; i + 1 < end; ++i) {
        char c = buf[i];
        if (!(c == ' ' || c == '\t' || c == '\r' || c == '\n')) { hasContent = true; break; }
      }

      // Overwrite the final '}' with , "template_id": <ID> }
      const size_t pos = end - 1;
      const size_t capFromPos = (tplLen + 1 + extra) - pos;
      int n = snprintf(&buf[pos], capFromPos, "%s\"template_id\":%d}", hasContent ? "," : "", tplId);
      if (n <= 0 || (size_t)n >= capFromPos) {
        ALOG_ERR(PSTR("Load_StandbyTemplate_Into_ProfileRAM: splice snprintf failed"));
        free(buf);
        return false;
      }

      // Re-validate after splice (defensive)
      if (!ValidateJSON(buf)) {
        ALOG_ERR(PSTR("Load_StandbyTemplate_Into_ProfileRAM: JSON invalid after splice"));
        free(buf);
        return false;
      }
    }
  }

  // Adopt into RAM
  if (standby.profileRAM) free(standby.profileRAM);
  standby.profileRAM = buf;
  standby.profileLen = strlen(buf) + 1;
  standby.last_src   = STBY_SRC::Template;
  standby.last_ver   = tplId;

  ALOG_INF(PSTR("Load_StandbyTemplate_Into_ProfileRAM: loaded (template_id=%d, len=%u)"),
           tplId, (unsigned)(standby.profileLen - 1));
  // ALOG_INF(PSTR("Template JSON: '%s'"), standby.profileRAM); // enable if you want the full dump

  return true;
}


// ---------- explicit force: reload template into RAM (+ FS optional) ----------
bool mAnimatorLight::Standby_ReloadTemplate(bool persist)
{
#ifndef USE_STANDBY_TEMPLATE
  ALOG_WRN(PSTR("Standby_ReloadTemplate: no template compiled"));
  return false;
#else
  if (!Load_StandbyTemplate_Into_ProfileRAM(/*injectTemplateId=*/true)) return false;
  if (persist) Standby_SaveProfileToFS();
  return true;
#endif
}



// ---------- capture current scene → RAM (optionally mirror to FS for /edit) ----------
bool mAnimatorLight::Standby_CaptureResumeToRAM()
{
  // wait a couple frames if updating
  const unsigned long t0 = millis();
  while (isUpdating() && millis()-t0 < (2*FRAMETIME)+1) yield();

  DynamicJsonDocument d(12*1024);
  JsonObject root = d.to<JsonObject>();
  serializeState(root, /*forPreset=*/true, /*includeBri=*/true, /*segmentBounds=*/true, /*selectedOnly=*/false);

  size_t len = measureJson(d) + 1;
  char* buf = (char*)malloc(len);
  if (!buf) { ALOG_ERR(PSTR("Standby_CaptureResumeToRAM: OOM %u"), (unsigned)len); return false; }
  serializeJson(d, buf, len);

  freePtr(standby.resumeRAM);
  standby.resumeRAM = buf;
  standby.resumeLen = len;
  standby.haveResumeRAM = true;

  ALOG_INF(PSTR("Standby_CaptureResumeToRAM: %u bytes"), (unsigned)(len-1));

  #ifdef ENABLE_DEBUGFEATURE_LIGHTING__STANDBY_STATE_SNAPSHOT_MIRROR_FILESYSTEM
  {
    char path[28]; strncpy_P(path, kResumeSnapshotPath, sizeof(path)); path[sizeof(path)-1]=0;
    File f = FILE_SYSTEM.open(path, "w");
    if (f) {
      size_t wr = serializeJson(d, f);
      f.close();
      ALOG_INF(PSTR("Standby: resume snapshot mirrored to %s (%u bytes)"), path, (unsigned)wr);
    } else {
      ALOG_WRN(PSTR("Standby: mirror open('%s') failed"), path);
    }
  }
  #endif

  return true;
}

/********************************************************************************************************************************************************************************************************************
 * @function              : Standby_JsonCommand_Run
 * @description           :
 *   Executes a standby JSON scene/command in two coordinated passes so BOTH the platform-wide
 *   command system and the lighting stack are applied:
 *
 *     1) Global Tasker pass
 *        - Routes the EXACT same payload through the system-wide JSON command handler (Tasker),
 *          so non-lighting keys (MQTT, device, system toggles, etc.) are honored.
 *        - Uses the shared data buffer with lock, then dispatches Tasker_Interface().
 *
 *     2) Lighting (WLED) pass
 *        - Parses again with ArduinoJson and applies lighting state via deserializeState()
 *          (segments, palettes, brightness, etc.), then calls notify(callMode).
 *
 *   The incoming bytes are duplicated into a private, NUL-terminated buffer for safe logging and to
 *   ensure we never mutate the caller’s memory. Tasker runs FIRST (when lock available) so global
 *   state updates precede the lighting scene application.
 *
 * @flow
 *   - Validate input → duplicate (len+1, NUL-terminated) → log payload
 *   - Try requestDataBufferLock() → copy into shared buffer → Tasker_Interface(TASK_JSON_COMMAND_ID) → release
 *   - Parse duplicate with ArduinoJson → deserializeState(root, callMode, 0) → notify(callMode)
 *
 * @params
 *   @param json     Pointer to JSON bytes (not required to be NUL-terminated).
 *   @param len      Number of valid bytes at @json.
 *   @param callMode Notify behavior for deserializeState()/notify() (e.g., CALL_MODE_NO_NOTIFY).
 *
 * @returns
 *   true  = Successfully ran Tasker (if lock available) and applied lighting state.
 *   false = OOM (OutOfMemory), parse failure, or invalid input (reason is logged).
 *
 * @notes
 *   - Dual pipeline is intentional: Tasker covers platform-level commands; WLED path covers segments/state.
 *   - If the Tasker lock is busy, we WARN and still run the lighting pass (to avoid stalling standby).
 *   - We log the full JSON we’re about to apply for traceability.
 *
 * @errors & logging
 *   - Logs OOM, parse errors, missing root object, and lock failures with payload length and content.
 *
 * @threading / locking
 *   - Uses requestDataBufferLock() ONLY for the Tasker pass; releases immediately after dispatch.
 *   - ArduinoJson pass does not use the shared buffer lock.
 *
 * @performance
 *   - One malloc for the duplicate buffer (len+1), freed before return.
 *   - ArduinoJson document capacity is len + 1024 for headroom.
 ********************************************************************************************************************************************************************************************************************/
bool mAnimatorLight::Standby_JsonCommand_Run(const char* json, size_t len, uint8_t callMode)
{
  LoggingLevels level = LOG_LEVEL_INFO;

  if (!json || !len) {
    ALOG_ERR(PSTR("Standby_JsonCommand_Run: empty (len=%u)"), (unsigned)len);
    return false;
  }

  // Duplicate into a mutable, NUL-terminated buffer (safe for logging & parsing)
  char* buf = (char*)malloc(len + 1);
  if (!buf) {
    ALOG_ERR(PSTR("Standby_JsonCommand_Run: OOM (%u)"), (unsigned)len);
    return false;
  }
  memcpy(buf, json, len);
  buf[len] = '\0';

  // Exact payload log (what we are about to apply)
  AddLog(level, PSTR("Standby_JsonCommand_Run: len=%u json='%s'"), (unsigned)len, buf);

  // -------- Pass 1: Global Tasker (platform-wide commands) --------
  if (requestDataBufferLock(GetModuleUniqueID())) {
    D_DATA_BUFFER_SOFT_CLEAR();
    data_buffer.payload.length_used = len;
    memcpy(data_buffer.payload.ctr, buf, data_buffer.payload.length_used);
    data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // NUL-terminate

    AddLog(level, PSTR(D_LOG_LIGHT "State Payload [len:%d] %s"),
           data_buffer.payload.length_used, data_buffer.payload.ctr);

    pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);
    releaseDataBufferLock();
  } else {
    ALOG_WRN(PSTR("Standby_JsonCommand_Run: Tasker buffer busy; skipping Tasker pass"));
  }

  // -------- Pass 2: Lighting (WLED) state apply --------
  DynamicJsonDocument d(len + 1024);
  DeserializationError err = deserializeJson(d, buf);
  if (err) {
    ALOG_ERR(PSTR("Standby_JsonCommand_Run: parse failed: %s; json='%s'"), err.c_str(), buf);
    free(buf);
    return false;
  }

  JsonObject root = d.as<JsonObject>();
  if (root.isNull()) {
    ALOG_ERR(PSTR("Standby_JsonCommand_Run: root null; json='%s'"), buf);
    free(buf);
    return false;
  }

  deserializeState(root, callMode, /*presetIndex=*/0);
  notify(callMode);

  free(buf);
  return true;
}


// ---------- Debug: save/load full state to FS (/edit inspection) ----------
bool mAnimatorLight::FileSave__State(bool includeBounds, bool includeBri, bool selectedOnly, bool fullGlobals)
{
  const unsigned long t0 = millis();
  while (isUpdating() && millis() - t0 < (2*FRAMETIME)+1) yield();

  DynamicJsonDocument doc(8192);
  JsonObject root = doc.to<JsonObject>();

  serializeState(root,
                 /*forPreset=*/!fullGlobals ? true : false,
                 /*includeBri=*/includeBri,
                 /*segmentBounds=*/includeBounds,
                 /*selectedSegmentsOnly=*/selectedOnly);

  JsonObject meta = root.createNestedObject("meta");
  meta["ts"]     = (uint32_t)millis();
  meta["schema"] = 1;
  meta["fw"]     = PROJECT_VERSION;

  char path[24]; strncpy_P(path, kStateSnapshotPath, sizeof(path)); path[sizeof(path)-1]=0;
  File f = FILE_SYSTEM.open(path, "w");
  if (!f) { ALOG_ERR(PSTR("FileSave__State: open('%s','w') failed"), path); return false; }
  size_t written = serializeJson(doc, f);
  f.close();
  if (!written) { ALOG_ERR(PSTR("FileSave__State: wrote 0 bytes")); return false; }
  ALOG_INF(PSTR("FileSave__State: wrote %u bytes to %s"), (unsigned)written, path);
  return true;
}

bool mAnimatorLight::FileLoad__State(uint8_t callMode)
{
  char path[24]; strncpy_P(path, kStateSnapshotPath, sizeof(path)); path[sizeof(path)-1]=0;
  File f = FILE_SYSTEM.open(path, "r");
  if (!f) { ALOG_ERR(PSTR("FileLoad__State: open('%s','r') failed"), path); return false; }

  size_t len = f.size();
  char* buf = (char*)malloc(len+1);
  if (!buf) { f.close(); ALOG_ERR(PSTR("FileLoad__State: OOM %u"), (unsigned)len); return false; }
  size_t rd = f.readBytes(buf, len);
  f.close();
  buf[rd] = '\0';

  DynamicJsonDocument doc(rd + 1024);
  DeserializationError err = deserializeJson(doc, buf);
  free(buf);
  if (err) {
    ALOG_ERR(PSTR("FileLoad__State: parse failed: %s"), err.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();
  if (root.isNull() || root["seg"].isNull()) { ALOG_ERR(PSTR("FileLoad__State: missing 'seg' array")); return false; }

  const unsigned long t0 = millis();
  while (isUpdating() && millis() - t0 < (2*FRAMETIME)+1) yield();

  deserializeState(root, callMode, /*presetIndex=*/0);
  notify(callMode);

  ALOG_INF(PSTR("FileLoad__State: applied snapshot from %s"), path);
  return true;
}

#endif // ENABLE_FEATURE_LIGHTING__STANDBY_VIRTUAL_PRESET
