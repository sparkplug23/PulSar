#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR
#ifdef ENABLE_FEATURE_LIGHTING__STANDBY_NEW

static const char STANDBY_CONFIG_FILE[] = "/standby.json";


/********************************************************************************************************************************************************************************************************************
 * Resume state
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::Standby_ClearResumeRAM()
{
  if (standby.resumeRAM) {
    free(standby.resumeRAM);
    standby.resumeRAM = nullptr;
  }

  standby.resumeLen = 0;
}


/********************************************************************************************************************************************************************************************************************
 * Time pattern handling
 *
 * Fixed format:
 *
 *   HH:MM
 *
 * Any individual digit may be replaced with '*'.
 *
 * Examples:
 *
 *   18:30
 *   **:10
 *   **:*0
 *   **:1*
 *   1*:00
 *   **:**
 *
 * '*' means that digit does not matter.
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_TimePatternMatches(const char* pattern, uint16_t minute)
{
  if (!pattern) return false;
  if (strlen(pattern) != 5) return false;
  if (pattern[2] != ':') return false;

  minute %= 1440;

  uint8_t hour = minute / 60;
  uint8_t mins = minute % 60;

  char value[6];
  snprintf(value, sizeof(value), "%02u:%02u", hour, mins);

  for (uint8_t i = 0; i < 5; i++)
  {
    if (i == 2) continue;
    if (pattern[i] == '*') continue;
    if (pattern[i] != value[i]) return false;
  }

  return true;
}


bool mAnimatorLight::Standby_ValidateTimePattern(const char* value)
{
  if (!value) return false;
  if (strlen(value) != 5) return false;
  if (value[2] != ':') return false;

  for (uint8_t i = 0; i < 5; i++)
  {
    if (i == 2) continue;
    if (value[i] == '*') continue;
    if (value[i] < '0' || value[i] > '9') return false;
  }

  // A pattern is considered valid only if it can match at least one
  // real minute in a 24-hour day.
  for (uint16_t minute = 0; minute < 1440; minute++)
  {
    if (Standby_TimePatternMatches(value, minute)) return true;
  }

  return false;
}


/********************************************************************************************************************************************************************************************************************
 * Compile one standby target into a 1440-bit eligibility map.
 *
 * Wildcards are paired by matching their wildcard positions.
 *
 * Example:
 *
 *   Start **:10
 *   End   **:20
 *
 * resolves to:
 *
 *   00:10 -> 00:20
 *   01:10 -> 01:20
 *   ...
 *   23:10 -> 23:20
 *
 *
 * Example:
 *
 *   Start **:*1
 *   End   **:*2
 *
 * resolves to:
 *
 *   00:01 -> 00:02
 *   00:11 -> 00:12
 *   00:21 -> 00:22
 *   ...
 *
 *
 * First matching standby target wins later in Standby_SelectTarget().
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_CompileTargetSchedule(STANDBY_TARGET& target)
{
  memset(target.allowed_minutes, 0, sizeof(target.allowed_minutes));

  if (!Standby_ValidateTimePattern(target.start)) {
    ALOG_WRN(PSTR("Standby: invalid start pattern %s"), target.start);
    return false;
  }

  if (!Standby_ValidateTimePattern(target.end)) {
    ALOG_WRN(PSTR("Standby: invalid end pattern %s"), target.end);
    return false;
  }

  bool found_window = false;

  for (uint16_t start = 0; start < 1440; start++)
  {
    if (!Standby_TimePatternMatches(target.start, start)) continue;

    char start_text[6];
    snprintf(start_text, sizeof(start_text), "%02u:%02u", start / 60, start % 60);

    for (uint16_t end = 0; end < 1440; end++)
    {
      if (!Standby_TimePatternMatches(target.end, end)) continue;

      char end_text[6];
      snprintf(end_text, sizeof(end_text), "%02u:%02u", end / 60, end % 60);

      bool compatible = true;

      // If the same position is wildcarded at both ends, require the
      // generated concrete values to match at that position.
      //
      // This is what pairs:
      //
      //   **:10 -> **:20
      //
      // within the same hour rather than pairing 01:10 with 17:20.
      for (uint8_t pos = 0; pos < 5; pos++)
      {
        if (pos == 2) continue;

        bool start_wildcard = target.start[pos] == '*';
        bool end_wildcard = target.end[pos] == '*';

        if (start_wildcard && end_wildcard && start_text[pos] != end_text[pos]) {
          compatible = false;
          break;
        }
      }

      if (!compatible) continue;

      found_window = true;

      // Same resolved start/end means full day.
      if (start == end)
      {
        memset(target.allowed_minutes, 0xFF, sizeof(target.allowed_minutes));
        return true;
      }

      if (start < end)
      {
        for (uint16_t minute = start; minute < end; minute++) {
          target.allowed_minutes[minute >> 3] |= (1U << (minute & 7));
        }
      }
      else
      {
        // Midnight-wrapping window.
        for (uint16_t minute = start; minute < 1440; minute++) {
          target.allowed_minutes[minute >> 3] |= (1U << (minute & 7));
        }

        for (uint16_t minute = 0; minute < end; minute++) {
          target.allowed_minutes[minute >> 3] |= (1U << (minute & 7));
        }
      }
    }
  }

  return found_window;
}


bool mAnimatorLight::Standby_TargetTimeAllowed(const STANDBY_TARGET& target, uint16_t minute)
{
  minute %= 1440;

  return target.allowed_minutes[minute >> 3] & (1U << (minute & 7));
}


/********************************************************************************************************************************************************************************************************************
 * Persistent configuration
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_LoadConfig()
{
  if (!FILE_SYSTEM.exists(STANDBY_CONFIG_FILE)) {
    ALOG_INF(PSTR("Standby: %s does not exist"), STANDBY_CONFIG_FILE);
    return false;
  }

  File f = FILE_SYSTEM.open(STANDBY_CONFIG_FILE, "r");

  if (!f) {
    ALOG_ERR(PSTR("Standby: failed to open %s"), STANDBY_CONFIG_FILE);
    return false;
  }

  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, f);

  f.close();

  if (error) {
    ALOG_ERR(PSTR("Standby: config parse failed %s"), error.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();

  if (root.isNull()) {
    ALOG_ERR(PSTR("Standby: config root null"));
    return false;
  }

  standby.wake_transition_secs = root["WakeTransitionSecs"] | 1;
  standby.standby_transition_secs = root["StandbyTransitionSecs"] | 30;

  standby.targets.clear();

  JsonArray targets = root["Targets"];

  if (!targets.isNull())
  {
    for (JsonObject target_obj : targets)
    {
      STANDBY_TARGET target;

      target.enabled = target_obj["Enabled"] | false;
      target.target_id = target_obj["TargetID"] | 0;

      const char* start_text = target_obj["Start"] | "00:00";
      const char* end_text = target_obj["End"] | "00:00";

      strlcpy(target.start, start_text, sizeof(target.start));
      strlcpy(target.end, end_text, sizeof(target.end));

      if (!Standby_CompileTargetSchedule(target)) {
        ALOG_WRN(PSTR("Standby: invalid target TargetID=%u Start=%s End=%s"), target.target_id, target.start, target.end);
        continue;
      }

      standby.targets.push_back(target);

      ALOG_INF(PSTR("Standby: target enabled=%u time=%s-%s TargetID=%u"), target.enabled, target.start, target.end, target.target_id);
    }
  }

  ALOG_INF(PSTR("Standby: config loaded targets=%u wakeTr=%u standbyTr=%u"), standby.targets.size(), standby.wake_transition_secs, standby.standby_transition_secs);

  return true;
}


bool mAnimatorLight::Standby_SaveConfig()
{
  DynamicJsonDocument doc(4096);
  JsonObject root = doc.to<JsonObject>();

  root["WakeTransitionSecs"] = standby.wake_transition_secs;
  root["StandbyTransitionSecs"] = standby.standby_transition_secs;

  JsonArray targets = root.createNestedArray("Targets");

  for (uint16_t i = 0; i < standby.targets.size(); i++)
  {
    STANDBY_TARGET& target = standby.targets[i];

    JsonObject target_obj = targets.createNestedObject();

    target_obj["Enabled"] = target.enabled;
    target_obj["Start"] = target.start;
    target_obj["End"] = target.end;
    target_obj["TargetID"] = target.target_id;
  }

  File f = FILE_SYSTEM.open(STANDBY_CONFIG_FILE, "w");

  if (!f) {
    ALOG_ERR(PSTR("Standby: failed to open %s for write"), STANDBY_CONFIG_FILE);
    return false;
  }

  size_t written = serializeJson(doc, f);

  f.close();

  if (!written) {
    ALOG_ERR(PSTR("Standby: failed to write config"));
    return false;
  }

  tkr_mfile->updateFSInfo();

  ALOG_INF(PSTR("Standby: config saved bytes=%u"), (unsigned)written);

  return true;
}


/********************************************************************************************************************************************************************************************************************
 * Initialisation
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_Init()
{
  // Runtime standby state never survives reboot.
  standby.enabled = false;
  standby.active_target_index = -1;
  standby.active_target_id = 0;

  Standby_ClearResumeRAM();

  standby.targets.clear();

  standby.wake_transition_secs = 1;
  standby.standby_transition_secs = 30;

  if (!Standby_LoadConfig()) {
    Standby_SaveConfig();
  }

  ALOG_INF(PSTR("Standby: initialized targets=%u"), standby.targets.size());

  return true;
}


/********************************************************************************************************************************************************************************************************************
 * Configuration setters
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::Standby_SetWakeTransition(uint16_t seconds)
{
  standby.wake_transition_secs = seconds;
  Standby_SaveConfig();
}


void mAnimatorLight::Standby_SetStandbyTransition(uint16_t seconds)
{
  standby.standby_transition_secs = seconds;
  Standby_SaveConfig();
}


/********************************************************************************************************************************************************************************************************************
 * Resume snapshot
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_CaptureResumeToRAM()
{
  const unsigned long t0 = millis();

  while (isUpdating() && ((millis() - t0) < ((2 * FRAMETIME) + 1))) {
    yield();
  }

  DynamicJsonDocument doc(12 * 1024);
  JsonObject root = doc.to<JsonObject>();

  serializeState(root, true, true, true, false);

  const size_t required = measureJson(doc) + 1;
  char* buffer = (char*)malloc(required);

  if (!buffer) {
    ALOG_ERR(PSTR("Standby: resume snapshot OOM required=%u"), (unsigned)required);
    return false;
  }

  const size_t written = serializeJson(doc, buffer, required);

  if (!written) {
    free(buffer);
    ALOG_ERR(PSTR("Standby: resume snapshot serialization failed"));
    return false;
  }

  Standby_ClearResumeRAM();

  standby.resumeRAM = buffer;
  standby.resumeLen = written + 1;

  ALOG_INF(PSTR("Standby: captured resume state bytes=%u"), (unsigned)written);

  return true;
}


bool mAnimatorLight::Standby_RestoreResumeFromRAM(uint8_t callMode)
{
  if (!standby.resumeRAM || standby.resumeLen <= 1) {
    ALOG_ERR(PSTR("Standby: no resume state"));
    return false;
  }

  DynamicJsonDocument doc(standby.resumeLen + 1024);
  DeserializationError error = deserializeJson(doc, standby.resumeRAM);

  if (error) {
    ALOG_ERR(PSTR("Standby: resume parse failed %s"), error.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();

  if (root.isNull()) {
    ALOG_ERR(PSTR("Standby: resume root null"));
    return false;
  }

  deserializeState(root, callMode, 0);
  notify(callMode);

  ALOG_INF(PSTR("Standby: resume state restored"));

  return true;
}


/********************************************************************************************************************************************************************************************************************
 * Transition
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::Standby_ApplyTransition(uint16_t seconds)
{
  tkr_anim->setTransition((uint32_t)seconds * 1000UL);
  jsonTransitionOnce = true;
}


/********************************************************************************************************************************************************************************************************************
 * Select target
 *
 * First enabled matching row wins.
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_SelectTarget(int16_t& target_index, uint16_t& target_id)
{
  target_index = -1;
  target_id = 0;

  uint16_t now_minute = (uint16_t)tkr_time->MinutesPastMidnight();

  for (uint16_t i = 0; i < standby.targets.size(); i++)
  {
    STANDBY_TARGET& target = standby.targets[i];

    if (!target.enabled) continue;
    if (!target.target_id) continue;
    if (!Standby_TargetTimeAllowed(target, now_minute)) continue;

    target_index = (int16_t)i;
    target_id = target.target_id;

    return true;
  }

  return false;
}


/********************************************************************************************************************************************************************************************************************
 * Apply target
 *
 * target_id may resolve to either:
 *
 *   - normal preset
 *   - playlist stored in preset ID space
 *
 * Standby itself does not care which.
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_ApplyTarget(int16_t target_index, uint16_t target_id, uint8_t callMode)
{
  if (target_index < 0 || !target_id) return false;

  #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
  if (currentPlaylist >= 0) {
    ALOG_INF(PSTR("Standby: unloading active playlist=%d"), currentPlaylist);
    unloadPlaylist();
  }
  #endif

  Standby_ApplyTransition(standby.standby_transition_secs);

  if (!applyPreset(target_id, callMode)) {
    ALOG_ERR(PSTR("Standby: failed target index=%d TargetID=%u"), target_index, target_id);
    return false;
  }

  standby.active_target_index = target_index;
  standby.active_target_id = target_id;

  ALOG_INF(PSTR("Standby: target index=%d TargetID=%u"), target_index, target_id);

  return true;
}


/********************************************************************************************************************************************************************************************************************
 * Update
 *
 * Called periodically while standby is enabled.
 *
 * Does NOT overwrite the pre-standby resume snapshot.
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_Update(uint8_t callMode)
{
  if (!standby.enabled) return true;

  int16_t selected_index = -1;
  uint16_t selected_id = 0;

  if (!Standby_SelectTarget(selected_index, selected_id)) {
    ALOG_WRN(PSTR("Standby: no target for current time"));
    return false;
  }

  if (selected_index == standby.active_target_index && selected_id == standby.active_target_id) {
    return true;
  }

  ALOG_INF(PSTR("Standby: target change %d/%u -> %d/%u"), standby.active_target_index, standby.active_target_id, selected_index, selected_id);

  return Standby_ApplyTarget(selected_index, selected_id, callMode);
}


/********************************************************************************************************************************************************************************************************************
 * Enter
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_Enter(uint8_t callMode)
{
  // Already enabled:
  // re-evaluate target only.
  // Never overwrite the original resume snapshot.
  if (standby.enabled) {
    return Standby_Update(callMode);
  }

  int16_t selected_index = -1;
  uint16_t selected_id = 0;

  // Make sure we actually have somewhere to go before modifying normal state.
  if (!Standby_SelectTarget(selected_index, selected_id)) {
    ALOG_ERR(PSTR("Standby: no valid target"));
    return false;
  }

  if (!Standby_CaptureResumeToRAM()) {
    return false;
  }

  #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
  if (currentPlaylist >= 0) {
    ALOG_INF(PSTR("Standby: unloading normal playlist=%d"), currentPlaylist);
    unloadPlaylist();
  }
  #endif

  standby.enabled = true;

  if (!Standby_ApplyTarget(selected_index, selected_id, callMode))
  {
    standby.enabled = false;
    standby.active_target_index = -1;
    standby.active_target_id = 0;

    Standby_ClearResumeRAM();

    ALOG_ERR(PSTR("Standby: enter failed"));

    return false;
  }

  ALOG_INF(PSTR("Standby: enabled TargetID=%u"), standby.active_target_id);

  return true;
}


/********************************************************************************************************************************************************************************************************************
 * Leave
 ********************************************************************************************************************************************************************************************************************/

bool mAnimatorLight::Standby_Leave(uint8_t callMode)
{
  if (!standby.enabled) {
    return true;
  }

  #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
  if (currentPlaylist >= 0) {
    ALOG_INF(PSTR("Standby: unloading standby playlist=%d"), currentPlaylist);
    unloadPlaylist();
  }
  #endif

  Standby_ApplyTransition(standby.wake_transition_secs);

  if (!Standby_RestoreResumeFromRAM(callMode)) {
    ALOG_ERR(PSTR("Standby: restore failed, remaining enabled"));
    return false;
  }

  standby.enabled = false;
  standby.active_target_index = -1;
  standby.active_target_id = 0;

  Standby_ClearResumeRAM();

  ALOG_INF(PSTR("Standby: disabled"));

  return true;
}


/********************************************************************************************************************************************************************************************************************
 * Periodic update
 ********************************************************************************************************************************************************************************************************************/

void mAnimatorLight::EverySecond_Standby()
{
  if (!standby.enabled) return;

  Standby_Update(CALL_MODE_NO_NOTIFY);
}


#endif // ENABLE_FEATURE_LIGHTING__STANDBY_NEW
#endif // USE_MODULE_LIGHTS_ANIMATOR