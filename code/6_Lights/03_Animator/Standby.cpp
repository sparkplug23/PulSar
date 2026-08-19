#include "_AnimatorLight.h"

#ifdef USE_MODULE_LIGHTS_ANIMATOR
#ifdef ENABLE_FEATURE_LIGHTING__STANDBY_MODE

static const char STANDBY_CONFIG_FILE[] = "/standby.json";


void mAnimatorLight::Standby_ClearResumeRAM()
{
  if (standby.resumeRAM)
  {
    free(standby.resumeRAM);
    standby.resumeRAM = nullptr;
  }

  standby.resumeLen = 0;
}


bool mAnimatorLight::Standby_LoadConfig()
{
  if (!FILE_SYSTEM.exists(STANDBY_CONFIG_FILE))
  {
    ALOG_INF(PSTR("Standby: %s does not exist"), STANDBY_CONFIG_FILE);
    return false;
  }

  File f = FILE_SYSTEM.open(STANDBY_CONFIG_FILE, "r");

  if (!f)
  {
    ALOG_ERR(PSTR("Standby: failed to open %s"), STANDBY_CONFIG_FILE);
    return false;
  }

  DynamicJsonDocument doc(1024);

  DeserializationError error = deserializeJson(doc, f);

  f.close();

  if (error)
  {
    ALOG_ERR(PSTR("Standby: config parse failed %s"), error.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();

  standby.armed = root["Arm"] | false;
  standby.target_id = root["Target"] | 0;
  standby.awake_secs = root["AwakeSecs"] | 0;
  standby.standby_secs = root["StandbySecs"] | 0;
  standby.wake_transition_secs = root["WakeTransitionSecs"] | 1;
  standby.sleep_transition_secs = root["SleepTransitionSecs"] | 30;

  ALOG_INF(PSTR("Standby: config loaded arm=%u target=%u awake=%u standby=%u wakeTr=%u sleepTr=%u"), standby.armed, standby.target_id, (unsigned)standby.awake_secs, (unsigned)standby.standby_secs, standby.wake_transition_secs, standby.sleep_transition_secs);

  return true;
}


bool mAnimatorLight::Standby_SaveConfig()
{
  DynamicJsonDocument doc(1024);

  JsonObject root = doc.to<JsonObject>();

  root["Arm"] = standby.armed ? 1 : 0;
  root["Target"] = standby.target_id;
  root["AwakeSecs"] = standby.awake_secs;
  root["StandbySecs"] = standby.standby_secs;
  root["WakeTransitionSecs"] = standby.wake_transition_secs;
  root["SleepTransitionSecs"] = standby.sleep_transition_secs;

  File f = FILE_SYSTEM.open(STANDBY_CONFIG_FILE, "w");

  if (!f)
  {
    ALOG_ERR(PSTR("Standby: failed to open %s for write"), STANDBY_CONFIG_FILE);
    return false;
  }

  size_t written = serializeJson(doc, f);

  f.close();

  if (!written)
  {
    ALOG_ERR(PSTR("Standby: failed to write config"));
    return false;
  }

  tkr_mfile->updateFSInfo();

  ALOG_INF(PSTR("Standby: config saved bytes=%u"), (unsigned)written);

  return true;
}


bool mAnimatorLight::Standby_Init()
{
  standby.active = false;
  standby.tick__awake.Stop();
  standby.tick__standby.Stop();

  Standby_ClearResumeRAM();

  if (!Standby_LoadConfig())
  {
    standby.armed = false;
    standby.target_id = 0;
    standby.awake_secs = 0;
    standby.standby_secs = 0;
    standby.wake_transition_secs = 1;
    standby.sleep_transition_secs = 30;

    Standby_SaveConfig();
  }

  // Arm survives restart, but active standby does not.
  // Start the awake countdown from the freshly booted/current state.
  if (standby.armed && standby.awake_secs)
  {
    Standby_StartAwakeTimer();
  }

  ALOG_INF(PSTR("Standby: initialized arm=%u target=%u"), standby.armed, standby.target_id);

  return true;
}


bool mAnimatorLight::Standby_SetArm(bool armed)
{
  if (standby.armed == armed)
  {
    return true;
  }

  standby.armed = armed;

  if (!standby.armed)
  {
    Standby_CancelTimers();

    if (standby.active)
    {
      Standby_Leave(CALL_MODE_NO_NOTIFY);
    }
  }
  else
  {
    if (standby.active)
    {
      Standby_StartStandbyTimer();
    }
    else
    {
      Standby_StartAwakeTimer();
    }
  }

  Standby_SaveConfig();

  ALOG_INF(PSTR("Standby: arm=%u"), standby.armed);

  return true;
}


bool mAnimatorLight::Standby_SetTarget(uint8_t preset_id)
{
  standby.target_id = preset_id;

  Standby_SaveConfig();

  ALOG_INF(PSTR("Standby: target=%u"), standby.target_id);

  return true;
}


void mAnimatorLight::Standby_SetAwakeDuration(uint32_t seconds)
{
  standby.awake_secs = seconds;

  if (standby.armed && !standby.active)
  {
    Standby_StartAwakeTimer();
  }

  Standby_SaveConfig();
}


void mAnimatorLight::Standby_SetStandbyDuration(uint32_t seconds)
{
  standby.standby_secs = seconds;

  if (standby.armed && standby.active)
  {
    Standby_StartStandbyTimer();
  }

  Standby_SaveConfig();
}


void mAnimatorLight::Standby_SetWakeTransition(uint16_t seconds)
{
  standby.wake_transition_secs = seconds;

  Standby_SaveConfig();
}


void mAnimatorLight::Standby_SetSleepTransition(uint16_t seconds)
{
  standby.sleep_transition_secs = seconds;

  Standby_SaveConfig();
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

  ALOG_INF(PSTR("Standby: awake timer started %u secs"), (unsigned)standby.awake_secs);
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

  ALOG_INF(PSTR("Standby: standby timer started %u secs"), (unsigned)standby.standby_secs);
}


void mAnimatorLight::Standby_CancelTimers()
{
  standby.tick__awake.Stop();
  standby.tick__standby.Stop();
}


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

  const size_t required = measureJson(doc) + 1;

  char* buffer = (char*)malloc(required);

  if (!buffer)
  {
    ALOG_ERR(PSTR("Standby: resume snapshot OOM required=%u"), (unsigned)required);
    return false;
  }

  const size_t written = serializeJson(doc, buffer, required);

  if (!written)
  {
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
  if (!standby.resumeRAM || standby.resumeLen <= 1)
  {
    ALOG_ERR(PSTR("Standby: no resume state"));
    return false;
  }

  DynamicJsonDocument doc(standby.resumeLen + 1024);

  DeserializationError error = deserializeJson(doc, standby.resumeRAM);

  if (error)
  {
    ALOG_ERR(PSTR("Standby: resume parse failed %s"), error.c_str());
    return false;
  }

  JsonObject root = doc.as<JsonObject>();

  if (root.isNull())
  {
    ALOG_ERR(PSTR("Standby: resume root null"));
    return false;
  }

  deserializeState(root, callMode, 0);

  notify(callMode);

  ALOG_INF(PSTR("Standby: resume state restored"));

  return true;
}


void mAnimatorLight::Standby_ApplyTransition(uint16_t seconds)
{
  const uint32_t transition_ms = (uint32_t)seconds * 1000UL;

  tkr_anim->setTransition(transition_ms);
  jsonTransitionOnce = true;

  ALOG_INF(PSTR("Standby: one-shot transition=%u secs"), seconds);
}


bool mAnimatorLight::Standby_Enter(uint8_t callMode)
{
  if (!standby.armed)
  {
    ALOG_INF(PSTR("Standby: enter ignored, not armed"));
    return false;
  }

  if (!standby.target_id)
  {
    ALOG_ERR(PSTR("Standby: no target configured"));
    return false;
  }

  if (standby.active)
  {
    #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
    if (currentPlaylist >= 0)
    {
      ALOG_INF(PSTR("Standby: unloading active playlist=%d"), currentPlaylist);
      unloadPlaylist();
    }
    #endif

    Standby_ApplyTransition(standby.sleep_transition_secs);
    applyPreset(standby.target_id, callMode);
    Standby_StartStandbyTimer();

    ALOG_INF(PSTR("Standby: active target changed/reloaded=%u"), standby.target_id);

    return true;
  }

  if (!Standby_CaptureResumeToRAM())
  {
    return false;
  }

  #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
  if (currentPlaylist >= 0)
  {
    ALOG_INF(PSTR("Standby: unloading active playlist=%d"), currentPlaylist);
    unloadPlaylist();
  }
  #endif

  Standby_ApplyTransition(standby.sleep_transition_secs);

  if (!applyPreset(standby.target_id, callMode))
  {
    Standby_ClearResumeRAM();
    ALOG_ERR(PSTR("Standby: failed to queue target=%u"), standby.target_id);
    return false;
  }

  standby.active = true;

  Standby_StartStandbyTimer();

  ALOG_INF(PSTR("Standby: entered target=%u"), standby.target_id);

  return true;
}


bool mAnimatorLight::Standby_Leave(uint8_t callMode)
{
  if (!standby.active)
  {
    return true;
  }

  #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS
  if (currentPlaylist >= 0)
  {
    unloadPlaylist();
  }
  #endif

  Standby_ApplyTransition(standby.wake_transition_secs);

  if (!Standby_RestoreResumeFromRAM(callMode))
  {
    ALOG_ERR(PSTR("Standby: restore failed, remaining active"));
    return false;
  }

  standby.active = false;

  Standby_ClearResumeRAM();

  ALOG_INF(PSTR("Standby: left"));

  return true;
}


bool mAnimatorLight::Standby_Wake(uint8_t callMode)
{
  if (!standby.armed)
  {
    ALOG_INF(PSTR("Standby: wake ignored, not armed"));
    return false;
  }

  if (standby.active)
  {
    if (!Standby_Leave(callMode))
    {
      return false;
    }
  }

  Standby_StartAwakeTimer();

  ALOG_INF(PSTR("Standby: wake"));

  return true;
}


bool mAnimatorLight::Standby_Sleep(uint8_t callMode)
{
  if (!standby.armed)
  {
    ALOG_INF(PSTR("Standby: sleep ignored, not armed"));
    return false;
  }

  standby.tick__awake.Stop();

  return Standby_Enter(callMode);
}


void mAnimatorLight::EverySecond_Standby()
{
  if (!standby.armed)
  {
    return;
  }

  if (!standby.active)
  {
    if (!standby.tick__awake.IsRunning())
    {
      return;
    }

    const bool expire = standby.tick__awake.Value() == 1;

    standby.tick__awake.UpdateTick();

    if (expire)
    {
      standby.tick__awake.Stop();

      ALOG_INF(PSTR("Standby: awake timer expired"));

      Standby_Sleep(CALL_MODE_NO_NOTIFY);
    }

    return;
  }

  if (!standby.tick__standby.IsRunning())
  {
    return;
  }

  const bool expire = standby.tick__standby.Value() == 1;

  standby.tick__standby.UpdateTick();

  if (expire)
  {
    standby.tick__standby.Stop();

    ALOG_INF(PSTR("Standby: standby timer expired"));

    Standby_Wake(CALL_MODE_NO_NOTIFY);
  }
}


#endif
#endif

/*

{
  "Standby": {
    "Presets": [1,2]
  }
}


{
  "Standby": {
    "EnterIndex": 2
  }
}


*/