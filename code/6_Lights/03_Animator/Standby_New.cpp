
#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR
#ifdef ENABLE_FEATURE_LIGHTING__STANDBY_NEW

static const char STANDBY_CONFIG_FILE[] = "/standby.json";
static const char STANDBY_CONFIG_TEMP_FILE[] = "/standby.tmp";

/********************************************************************
 * Resume state
 ********************************************************************/

void mAnimatorLight::Standby_ClearResumeRAM()
{
  if (standby.resumeRAM)
  {
    free(standby.resumeRAM);
    standby.resumeRAM = nullptr;
  }
  standby.resumeLen = 0;
}

/********************************************************************
 * Time pattern handling
 ********************************************************************/

bool mAnimatorLight::Standby_TimePatternMatches(const char* pattern, uint16_t minute)
{
  if (!pattern || strlen(pattern) != 5 || pattern[2] != ':') return false;

  minute %= 1440;

  char value[6];
  snprintf(value, sizeof(value), "%02u:%02u", minute / 60, minute % 60);

  for (uint8_t i = 0; i < 5; i++)
  {
    if (i == 2 || pattern[i] == '*') continue;
    if (pattern[i] != value[i]) return false;
  }

  return true;
}

bool mAnimatorLight::Standby_ValidateTimePattern(const char* value)
{
  if (!value || strlen(value) != 5 || value[2] != ':') return false;

  for (uint8_t i = 0; i < 5; i++)
  {
    if (i == 2 || value[i] == '*') continue;
    if (value[i] < '0' || value[i] > '9') return false;
  }

  for (uint16_t minute = 0; minute < 1440; minute++)
  {
    if (Standby_TimePatternMatches(value, minute)) return true;
  }

  return false;
}

bool mAnimatorLight::Standby_CompileTargetSchedule(STANDBY_TARGET& target)
{
  memset(target.allowed_minutes, 0, sizeof(target.allowed_minutes));

  if (!Standby_ValidateTimePattern(target.start)) return false;
  if (!Standby_ValidateTimePattern(target.end)) return false;

  // Equal expressions mean full-day eligibility.
  if (strcmp(target.start, target.end) == 0)
  {
    memset(target.allowed_minutes, 0xFF, sizeof(target.allowed_minutes));
    return true;
  }

  bool active = false;
  bool found_start = false;
  bool found_end = false;

  // Two passes handle wrapping and repeating time windows.
  for (uint8_t pass = 0; pass < 2; pass++)
  {
    for (uint16_t minute = 0; minute < 1440; minute++)
    {
      const bool start_match = Standby_TimePatternMatches(target.start, minute);
      const bool end_match = Standby_TimePatternMatches(target.end, minute);

      if (start_match) found_start = true;
      if (end_match) found_end = true;

      // End exclusive, start inclusive.
      if (end_match) active = false;
      if (start_match) active = true;

      if (pass == 1 && active)
      {
        target.allowed_minutes[minute >> 3] |= (1U << (minute & 7));
      }
    }
  }

  if (!found_start || !found_end)
  {
    memset(target.allowed_minutes, 0, sizeof(target.allowed_minutes));
    return false;
  }

  return true;
}

bool mAnimatorLight::Standby_TargetTimeAllowed(const STANDBY_TARGET& target, uint16_t minute)
{
  minute %= 1440;
  return (target.allowed_minutes[minute >> 3] & (1U << (minute & 7))) != 0;
}

/********************************************************************
 * Persistent configuration
 ********************************************************************/

bool mAnimatorLight::Standby_ReadConfig(JsonObject root)
{
  if (root.isNull()) return false;

  const uint16_t version = root["Version"] | 3;
  const uint16_t template_id = root["TemplateID"] | 0;
  const bool armed = root["Arm"] | false;
  const uint32_t awake_secs = root["AwakeSecs"] | 0UL;
  const uint32_t standby_secs = root["StandbySecs"] | 0UL;
  const uint16_t wake_transition = root["WakeTransitionSecs"] | 1;
  const uint16_t standby_transition = root["StandbyTransitionSecs"] | 2;
  const uint16_t leave_target = root["LeaveTargetID"] | 0;

  if (version != 3)
  {
    ALOG_ERR(PSTR("Standby: unsupported schema version=%u"), version);
    return false;
  }

  const char* leave_mode = root["LeaveMode"] | "Snapshot";
  STANDBY_LEAVE_MODE selected_leave_mode;

  if (!strcmp(leave_mode, "Snapshot"))
  {
    selected_leave_mode = STANDBY_LEAVE_MODE_SNAPSHOT;
  }
  else if (!strcmp(leave_mode, "Target"))
  {
    selected_leave_mode = STANDBY_LEAVE_MODE_TARGET;
  }
  else
  {
    ALOG_ERR(PSTR("Standby: invalid LeaveMode=%s"), leave_mode);
    return false;
  }

  if (selected_leave_mode == STANDBY_LEAVE_MODE_TARGET && !leave_target)
  {
    ALOG_ERR(PSTR("Standby: Target leave mode requires LeaveTargetID"));
    return false;
  }

  std::vector<STANDBY_TARGET> new_targets;

  JsonArray targets = root["Targets"].as<JsonArray>();

  if (!targets.isNull())
  {
    for (JsonObject target_obj : targets)
    {
      STANDBY_TARGET target;

      target.enabled = target_obj["Enabled"] | false;
      target.target_id = target_obj["TargetID"] | 0;

      const char* start = target_obj["Start"] | "00:00";
      const char* end = target_obj["End"] | "00:00";

      if (strlen(start) != 5 || strlen(end) != 5)
      {
        ALOG_ERR(PSTR("Standby: invalid target time length"));
        return false;
      }

      strlcpy(target.start, start, sizeof(target.start));
      strlcpy(target.end, end, sizeof(target.end));

      if (!Standby_CompileTargetSchedule(target))
      {
        ALOG_ERR(PSTR("Standby: invalid target schedule"));
        return false;
      }

      if (target.enabled && !target.target_id)
      {
        ALOG_ERR(PSTR("Standby: enabled target has no ID"));
        return false;
      }

      new_targets.push_back(target);
    }
  }

  // Commit only after successful validation.
  standby.version = version;
  standby.template_id = template_id;
  standby.armed = armed;
  standby.awake_secs = awake_secs;
  standby.standby_secs = standby_secs;
  standby.wake_transition_secs = wake_transition;
  standby.standby_transition_secs = standby_transition;
  standby.leave_mode = selected_leave_mode;
  standby.leave_target_id = leave_target;
  standby.targets = std::move(new_targets);

  ALOG_INF(
    PSTR("Standby: config loaded arm=%u awake=%u standby=%u leave=%u targets=%u"),
    standby.armed,
    (unsigned)standby.awake_secs,
    (unsigned)standby.standby_secs,
    (unsigned)standby.leave_mode,
    (unsigned)standby.targets.size()
  );

  return true;
}

bool mAnimatorLight::Standby_LoadConfig()
{
  if (!FILE_SYSTEM.exists(STANDBY_CONFIG_FILE))
  {
    ALOG_INF(PSTR("Standby: configuration missing"));
    return false;
  }

  File f = FILE_SYSTEM.open(STANDBY_CONFIG_FILE, "r");

  if (!f)
  {
    ALOG_ERR(PSTR("Standby: failed opening configuration"));
    return false;
  }

  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, f);
  f.close();

  if (error)
  {
    ALOG_ERR(PSTR("Standby: configuration parse failed %s"), error.c_str());
    return false;
  }

  return Standby_ReadConfig(doc.as<JsonObject>());
}

bool mAnimatorLight::Standby_SaveConfig()
{
  DynamicJsonDocument doc(8192);
  JsonObject root = doc.to<JsonObject>();

  root["Version"] = standby.version;
  root["TemplateID"] = standby.template_id;
  root["Arm"] = standby.armed;
  root["AwakeSecs"] = standby.awake_secs;
  root["StandbySecs"] = standby.standby_secs;
  root["WakeTransitionSecs"] = standby.wake_transition_secs;
  root["StandbyTransitionSecs"] = standby.standby_transition_secs;
  root["LeaveMode"] = standby.leave_mode == STANDBY_LEAVE_MODE_TARGET ? "Target" : "Snapshot";
  root["LeaveTargetID"] = standby.leave_target_id;

  JsonArray targets = root.createNestedArray("Targets");

  for (const STANDBY_TARGET& target : standby.targets)
  {
    JsonObject obj = targets.createNestedObject();
    obj["Enabled"] = target.enabled;
    obj["Start"] = target.start;
    obj["End"] = target.end;
    obj["TargetID"] = target.target_id;
  }

  if (doc.overflowed())
  {
    ALOG_ERR(PSTR("Standby: config JSON overflow"));
    return false;
  }

  File f = FILE_SYSTEM.open(STANDBY_CONFIG_TEMP_FILE, "w");

  if (!f)
  {
    ALOG_ERR(PSTR("Standby: cannot open temporary config"));
    return false;
  }

  const size_t expected = measureJson(doc);
  const size_t written = serializeJson(doc, f);

  f.flush();
  f.close();

  if (written != expected)
  {
    FILE_SYSTEM.remove(STANDBY_CONFIG_TEMP_FILE);
    ALOG_ERR(PSTR("Standby: incomplete config write"));
    return false;
  }

  if (!FILE_SYSTEM.rename(STANDBY_CONFIG_TEMP_FILE, STANDBY_CONFIG_FILE))
  {
    ALOG_ERR(PSTR("Standby: config rename failed"));
    return false;
  }

  tkr_mfile->updateFSInfo();

  ALOG_INF(PSTR("Standby: config saved bytes=%u"), (unsigned)written);
  return true;
}

/********************************************************************
 * Template provisioning
 ********************************************************************/

bool mAnimatorLight::Standby_ProvisionTemplate()
{
  #if defined(USE_STANDBY_TEMPLATE) && defined(LIGHTING_STANDBY_TEMPLATE_ID)

  DynamicJsonDocument doc(8192);
  DeserializationError error = deserializeJson(doc, STANDBY_TEMPLATE);

  if (error)
  {
    ALOG_ERR(PSTR("Standby: firmware template parse failed %s"), error.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();

  if (root.isNull()) return false;

  const uint16_t template_id = root["TemplateID"] | 0;

  if (template_id != LIGHTING_STANDBY_TEMPLATE_ID)
  {
    ALOG_ERR(PSTR("Standby: firmware template ID mismatch"));
    return false;
  }

  if (!Standby_ReadConfig(root))
  {
    ALOG_ERR(PSTR("Standby: invalid firmware template"));
    return false;
  }

  ALOG_INF(PSTR("Standby: provisioning template=%u"), standby.template_id);

  return Standby_SaveConfig();

  #else

  ALOG_ERR(PSTR("Standby: firmware template not configured"));
  return false;

  #endif
}

/********************************************************************
 * Initialisation
 ********************************************************************/

bool mAnimatorLight::Standby_Init()
{
  standby.enabled = false;
  standby.wake_target_started = false;
  standby.active_target_index = -1;
  standby.active_target_id = 0;

  Standby_CancelTimers();
  Standby_ClearResumeRAM();

  bool loaded = Standby_LoadConfig();

  #if defined(USE_STANDBY_TEMPLATE) && defined(LIGHTING_STANDBY_TEMPLATE_ID)

  if (loaded && standby.template_id != LIGHTING_STANDBY_TEMPLATE_ID)
  {
    ALOG_INF(
      PSTR("Standby: template revision changed %u -> %u"),
      standby.template_id,
      (unsigned)LIGHTING_STANDBY_TEMPLATE_ID
    );

    loaded = Standby_ProvisionTemplate();
  }
  else if (!loaded && !FILE_SYSTEM.exists(STANDBY_CONFIG_FILE))
  {
    loaded = Standby_ProvisionTemplate();
  }

  #endif

  if (!loaded)
  {
    ALOG_ERR(PSTR("Standby: no valid configuration"));
    standby.armed = false;
    return false;
  }

  if (standby.armed)
  {
    Standby_StartAwakeTimer();
  }

  ALOG_INF(PSTR("Standby: initialized template=%u"), standby.template_id);
  return true;
}

/********************************************************************
 * Timers
 ********************************************************************/

void mAnimatorLight::Standby_CancelTimers()
{
  standby.tick__awake.Stop();
  standby.tick__standby.Stop();
}

void mAnimatorLight::Standby_StartAwakeTimer()
{
  standby.tick__standby.Stop();

  if (!standby.armed || !standby.awake_secs)
  {
    standby.tick__awake.Stop();
    return;
  }

  standby.tick__awake.Start(standby.awake_secs);

  ALOG_INF(PSTR("Standby: awake timer=%u"), (unsigned)standby.awake_secs);
}

void mAnimatorLight::Standby_StartStandbyTimer()
{
  standby.tick__awake.Stop();

  if (!standby.armed || !standby.standby_secs)
  {
    standby.tick__standby.Stop();
    return;
  }

  standby.tick__standby.Start(standby.standby_secs);

  ALOG_INF(PSTR("Standby: active timer=%u"), (unsigned)standby.standby_secs);
}

bool mAnimatorLight::Standby_SetArm(bool armed)
{
  if (standby.armed == armed) return true;

  standby.armed = armed;

  if (!armed)
  {
    Standby_CancelTimers();

    if (standby.enabled)
    {
      return Standby_Leave(CALL_MODE_NO_NOTIFY);
    }
  }
  else if (standby.enabled)
  {
    Standby_StartStandbyTimer();
  }
  else
  {
    Standby_StartAwakeTimer();
  }

  return true;
}

/********************************************************************
 * Configuration setters
 ********************************************************************/

void mAnimatorLight::Standby_SetWakeTransition(uint16_t seconds)
{
  standby.wake_transition_secs = seconds;
}

void mAnimatorLight::Standby_SetStandbyTransition(uint16_t seconds)
{
  standby.standby_transition_secs = seconds;
}

/********************************************************************
 * Resume snapshot
 ********************************************************************/

bool mAnimatorLight::Standby_CaptureResumeToRAM()
{
  const unsigned long t0 = millis();

  while (isUpdating() && ((millis() - t0) < ((2 * FRAMETIME) + 1)))
  {
    yield();
  }

  DynamicJsonDocument doc(12 * 1024);
  JsonObject root = doc.to<JsonObject>();

  serializeState(root, true, true, true, false);

  if (doc.overflowed())
  {
    ALOG_ERR(PSTR("Standby: resume state JSON overflow"));
    return false;
  }

  const size_t required = measureJson(doc) + 1;
  char* buffer = (char*)malloc(required);

  if (!buffer)
  {
    ALOG_ERR(PSTR("Standby: resume snapshot OOM %u"), (unsigned)required);
    return false;
  }

  const size_t written = serializeJson(doc, buffer, required);

  if (!written)
  {
    free(buffer);
    return false;
  }

  Standby_ClearResumeRAM();

  standby.resumeRAM = buffer;
  standby.resumeLen = written + 1;

  ALOG_INF(PSTR("Standby: snapshot captured %u bytes"), (unsigned)written);
  return true;
}

bool mAnimatorLight::Standby_RestoreResumeFromRAM(uint8_t callMode)
{
  if (!standby.resumeRAM || standby.resumeLen <= 1)
  {
    ALOG_ERR(PSTR("Standby: no resume snapshot"));
    return false;
  }

  DynamicJsonDocument doc(standby.resumeLen + 1024);
  DeserializationError error = deserializeJson(doc, standby.resumeRAM);

  if (error)
  {
    ALOG_ERR(PSTR("Standby: snapshot parse failed %s"), error.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();
  if (root.isNull()) return false;

  deserializeState(root, callMode, 0);
  notify(callMode);

  ALOG_INF(PSTR("Standby: snapshot restored"));
  return true;
}

/********************************************************************
 * Transitions and targets
 ********************************************************************/

void mAnimatorLight::Standby_ApplyTransition(uint16_t seconds)
{
  tkr_anim->setTransition((uint32_t)seconds * 1000UL);
  jsonTransitionOnce = true;
}

bool mAnimatorLight::Standby_SelectTarget(int16_t& target_index, uint16_t& target_id)
{
  target_index = -1;
  target_id = 0;

  const uint16_t now_minute = (uint16_t)tkr_time->MinutesPastMidnight();

  for (uint16_t i = 0; i < standby.targets.size(); i++)
  {
    const STANDBY_TARGET& target = standby.targets[i];

    if (!target.enabled) continue;
    if (!target.target_id) continue;
    if (!Standby_TargetTimeAllowed(target, now_minute)) continue;

    target_index = (int16_t)i;
    target_id = target.target_id;
    return true;
  }

  return false;
}

bool mAnimatorLight::Standby_ApplyTarget(int16_t target_index, uint16_t target_id, uint8_t callMode)
{
  if (target_index < 0 || !target_id) return false;

  #ifdef ENABLE_FEATURE_LIGHTING__CORE__PLAYLISTS
  if (currentPlaylist >= 0) unloadPlaylist();
  #endif

  Standby_ApplyTransition(standby.standby_transition_secs);

  if (!applyPreset(target_id, callMode))
  {
    ALOG_ERR(PSTR("Standby: failed target=%u"), target_id);
    return false;
  }

  standby.active_target_index = target_index;
  standby.active_target_id = target_id;

  ALOG_INF(PSTR("Standby: target=%u row=%d"), target_id, target_index);
  return true;
}

bool mAnimatorLight::Standby_ApplyLeaveTarget(uint8_t callMode)
{
  if (!standby.leave_target_id)
  {
    ALOG_ERR(PSTR("Standby: LeaveTargetID is zero"));
    return false;
  }

  #ifdef ENABLE_FEATURE_LIGHTING__CORE__PLAYLISTS
  if (currentPlaylist >= 0) unloadPlaylist();
  #endif

  Standby_ApplyTransition(standby.wake_transition_secs);

  if (!applyPreset(standby.leave_target_id, callMode))
  {
    ALOG_ERR(PSTR("Standby: leave target failed=%u"), standby.leave_target_id);
    return false;
  }

  standby.wake_target_started = true;

  ALOG_INF(PSTR("Standby: leave target=%u"), standby.leave_target_id);
  return true;
}

/********************************************************************
 * Standby state control
 ********************************************************************/

bool mAnimatorLight::Standby_Update(uint8_t callMode)
{
  if (!standby.enabled) return true;

  int16_t selected_index = -1;
  uint16_t selected_id = 0;

  if (!Standby_SelectTarget(selected_index, selected_id))
  {
    ALOG_WRN(PSTR("Standby: no target for current time"));
    return false;
  }

  if (selected_index == standby.active_target_index &&
      selected_id == standby.active_target_id)
  {
    return true;
  }

  return Standby_ApplyTarget(selected_index, selected_id, callMode);
}

bool mAnimatorLight::Standby_Enter(uint8_t callMode)
{
  if (standby.enabled)
  {
    return Standby_Update(callMode);
  }

  int16_t selected_index = -1;
  uint16_t selected_id = 0;

  if (!Standby_SelectTarget(selected_index, selected_id))
  {
    ALOG_ERR(PSTR("Standby: no valid standby target"));
    return false;
  }

  // Capture exactly once when moving from awake to standby.
  if (!Standby_CaptureResumeToRAM()) return false;

  if (!Standby_ApplyTarget(selected_index, selected_id, callMode))
  {
    Standby_RestoreResumeFromRAM(callMode);
    Standby_ClearResumeRAM();
    return false;
  }

  standby.enabled = true;
  standby.wake_target_started = false;

  Standby_StartStandbyTimer();

  ALOG_INF(PSTR("Standby: entered"));
  return true;
}

bool mAnimatorLight::Standby_Leave(uint8_t callMode)
{
  if (!standby.enabled) return true;

  bool success = false;

  if (standby.leave_mode == STANDBY_LEAVE_MODE_TARGET)
  {
    success = Standby_ApplyLeaveTarget(callMode);
  }
  else
  {
    #ifdef ENABLE_FEATURE_LIGHTING__CORE__PLAYLISTS
    if (currentPlaylist >= 0) unloadPlaylist();
    #endif

    Standby_ApplyTransition(standby.wake_transition_secs);
    success = Standby_RestoreResumeFromRAM(callMode);
  }

  if (!success)
  {
    ALOG_ERR(PSTR("Standby: leave failed"));
    return false;
  }

  standby.enabled = false;
  standby.active_target_index = -1;
  standby.active_target_id = 0;

  Standby_ClearResumeRAM();
  Standby_StartAwakeTimer();

  ALOG_INF(PSTR("Standby: left mode=%u"), (unsigned)standby.leave_mode);
  return true;
}

bool mAnimatorLight::Standby_Wake(uint8_t callMode)
{
  if (standby.enabled)
  {
    return Standby_Leave(callMode);
  }

  // Already awake: do not restart an active motion playlist.
  // But on first motion after boot, apply the wake target.
  if (standby.leave_mode == STANDBY_LEAVE_MODE_TARGET &&
      !standby.wake_target_started)
  {
    if (!Standby_ApplyLeaveTarget(callMode)) return false;
  }

  Standby_StartAwakeTimer();

  ALOG_INF(PSTR("Standby: wake / timer refreshed"));
  return true;
}

bool mAnimatorLight::Standby_Sleep(uint8_t callMode)
{
  return Standby_Enter(callMode);
}

/********************************************************************
 * Periodic update
 ********************************************************************/

void mAnimatorLight::EverySecond_Standby()
{
  if (!standby.armed) return;

  if (!standby.enabled)
  {
    if (!standby.tick__awake.IsRunning()) return;

    const bool expired = standby.tick__awake.Value() == 1;
    standby.tick__awake.UpdateTick();

    if (expired)
    {
      standby.tick__awake.Stop();

      ALOG_INF(PSTR("Standby: awake timer expired"));
      Standby_Sleep(CALL_MODE_NO_NOTIFY);
    }

    return;
  }

  if (standby.tick__standby.IsRunning())
  {
    const bool expired = standby.tick__standby.Value() == 1;
    standby.tick__standby.UpdateTick();

    if (expired)
    {
      standby.tick__standby.Stop();

      ALOG_INF(PSTR("Standby: active timer expired"));
      Standby_Wake(CALL_MODE_NO_NOTIFY);
      return;
    }
  }

  // Re-evaluate time-window targets while in standby.
  Standby_Update(CALL_MODE_NO_NOTIFY);
}

#endif // ENABLE_FEATURE_LIGHTING__STANDBY_NEW
#endif // USE_MODULE_LIGHTS_ANIMATOR
