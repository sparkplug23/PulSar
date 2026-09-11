#include "_AnimatorLight.h"

#ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS


/*******************************************************************************************************************************************************************************************************************
 * @description : Shuffle playlist entries.
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::shufflePlaylist()
{
  ALOG_HGL(PSTR("shuffleplaylist"));

  return; // currently blocked

  int currentIndex = playlistLen;
  PlaylistEntry temporaryValue;

  while (currentIndex--)
  {
    int randomIndex = random(0, currentIndex);

    temporaryValue = playlistEntries[currentIndex];
    playlistEntries[currentIndex] = playlistEntries[randomIndex];
    playlistEntries[randomIndex] = temporaryValue;
  }

  DEBUG_PRINTLN(F("Playlist shuffle."));
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Unload the currently active playlist.
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::unloadPlaylist()
{
  if (playlistEntries != nullptr)
  {
    delete[] playlistEntries;
    playlistEntries = nullptr;
  }

  currentPlaylist = -1;
  playlistIndex = -1;

  playlistLen = 0;
  playlistEntryDur = 0;
  playlistOptions = 0;
  playlistEndPreset = 0;
  playlistDefaultPreset = 0;

  playlistFallbackActive = false;

  ALOG_INF(PSTR("unloadPlaylist"));
}


#ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS

/*******************************************************************************************************************************************************************************************************************
 * @description :
 *   Parse a human-readable HH:MM string into minutes since midnight.
 *
 *   Examples:
 *     "00:00" -> 0
 *     "15:00" -> 900
 *     "23:45" -> 1425
 ********************************************************************************************************************************************************************************************************************/
bool mAnimatorLight::Playlist_ParseTime(const char* value, uint16_t& minute)
{
  if (!value)
  {
    return false;
  }

  if (strlen(value) != 5)
  {
    return false;
  }

  if (value[2] != ':')
  {
    return false;
  }

  if (value[0] < '0' || value[0] > '9') return false;
  if (value[1] < '0' || value[1] > '9') return false;
  if (value[3] < '0' || value[3] > '9') return false;
  if (value[4] < '0' || value[4] > '9') return false;

  uint8_t hour = ((value[0] - '0') * 10) + (value[1] - '0');
  uint8_t mins = ((value[3] - '0') * 10) + (value[4] - '0');

  if (hour > 23)
  {
    return false;
  }

  if (mins > 59)
  {
    return false;
  }

  minute = ((uint16_t)hour * 60U) + mins;

  return true;
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Convert minutes since midnight back into HH:MM.
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::Playlist_MinuteToString(uint16_t minute, char* buffer)
{
  minute %= 1440;

  uint8_t hour = minute / 60;
  uint8_t mins = minute % 60;

  snprintf(buffer, 6, "%02u:%02u", hour, mins);
}


/*******************************************************************************************************************************************************************************************************************
 * @description :
 *   Check whether current time falls inside a playlist entry window.
 *
 *   start == end means unrestricted/full day.
 *
 *   Supports midnight wrapping:
 *     20:00 -> 04:00
 ********************************************************************************************************************************************************************************************************************/
bool mAnimatorLight::Playlist_TimeAllowed(uint16_t startMinute, uint16_t endMinute, uint16_t nowMinute)
{
  if (startMinute == endMinute)
  {
    return true;
  }

  if (startMinute < endMinute)
  {
    return nowMinute >= startMinute && nowMinute < endMinute;
  }

  return nowMinute >= startMinute || nowMinute < endMinute;
}


/*******************************************************************************************************************************************************************************************************************
 * @description :
 *   Starting from startIndex, find the first playlist entry which is
 *   currently allowed.
 *
 *   Returns:
 *     >= 0 : playlist index
 *     -1   : no entries currently allowed
 ********************************************************************************************************************************************************************************************************************/
int16_t mAnimatorLight::Playlist_SelectAllowedIndexByTime(uint8_t startIndex, uint16_t nowMinute)
{
  if (!playlistEntries)
  {
    return -1;
  }

  if (!playlistLen)
  {
    return -1;
  }

  uint8_t index = startIndex % playlistLen;

  for (uint8_t attempt = 0; attempt < playlistLen; attempt++)
  {
    PlaylistEntry& entry = playlistEntries[index];

    if (!entry.time_window_enabled)
    {
      return index;
    }

    if (Playlist_TimeAllowed(entry.time_start_minute, entry.time_end_minute, nowMinute))
    {
      return index;
    }

    index = (index + 1) % playlistLen;
  }

  return -1;
}

#endif // ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS


/*******************************************************************************************************************************************************************************************************************
 * @description : Load playlist JSON into runtime playlistEntries[].
 ********************************************************************************************************************************************************************************************************************/
int32_t mAnimatorLight::loadPlaylist(JsonObject playlistObj, uint16_t presetId)
{
  ALOG_INF(PSTR("mAnimatorLight::loadPlaylist"));

  unloadPlaylist();


  // ==================================================================
  // Determine playlist length
  // ==================================================================

  JsonArray presets;
  JsonArray psn = playlistObj["psn"];

  bool useNames = false;

  if (!psn.isNull() && psn.size() > 0)
  {
    useNames = true;
    playlistLen = psn.size();
  }
  else
  {
    presets = playlistObj["ps"];
    playlistLen = presets.size();
  }

  if (playlistLen == 0)
  {
    ALOG_WRN(PSTR("Playlist: no entries"));
    return -1;
  }

  if (playlistLen > 100)
  {
    playlistLen = 100;
  }


  // ==================================================================
  // Allocate playlist runtime state
  // ==================================================================

  playlistEntries = new PlaylistEntry[playlistLen];

  if (playlistEntries == nullptr)
  {
    playlistLen = 0;
    ALOG_ERR(PSTR("Playlist: allocation failed"));
    return -1;
  }

  for (uint8_t i = 0; i < playlistLen; i++)
  {
    playlistEntries[i].preset = 0;
    playlistEntries[i].dur = 100;
    playlistEntries[i].tr = 0;

    #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS
    playlistEntries[i].time_window_enabled = false;
    playlistEntries[i].time_start_minute = 0;
    playlistEntries[i].time_end_minute = 0;
    #endif
  }


  // ==================================================================
  // Preset IDs
  // ==================================================================

  uint8_t it = 0;

  #ifdef ENABLE_DEVFEATURE_LIGHTS__PLAYLIST_NAME_BASED_LOADING

  if (useNames)
  {
    ALOG_INF(PSTR("Loading playlist via preset names (psn)"));

    for (const char* name : psn)
    {
      if (it >= playlistLen)
      {
        break;
      }

      uint16_t foundIndex = PRESET_ID_NONE;

      for (byte p = 1; p < 251; p++)
      {
        String pname;

        if (getPresetName(p, pname))
        {
          if (pname.equalsIgnoreCase(name))
          {
            foundIndex = p;
            break;
          }
        }
      }

      if (!foundIndex)
      {
        ALOG_ERR(PSTR("Playlist: preset name '%s' not found"), name ? name : "(null)");
      }

      playlistEntries[it].preset = foundIndex;

      ALOG_INF(PSTR("Playlist: psn[%u] '%s' -> preset %u"), it, name ? name : "(null)", foundIndex);

      it++;
    }
  }
  else

  #endif
  {
    ALOG_INF(PSTR("Loading playlist via preset indices (ps)"));

    for (uint32_t ps : presets)
    {
      if (it >= playlistLen)
      {
        break;
      }

      playlistEntries[it].preset = (ps > 0 && ps < PRESET_ID_TEMP) ? (uint16_t)ps : PRESET_ID_NONE;

      ALOG_INF(PSTR("Playlist: ps[%u] -> %u"), it, playlistEntries[it].preset);

      it++;
    }
  }


  // ==================================================================
  // Durations
  // ==================================================================

  JsonArray durations = playlistObj["dur"];

  if (durations.isNull())
  {
    uint32_t duration = playlistObj["dur"] | 100;

    if (duration <= 1)
    {
      duration = 100;
    }

    for (uint8_t i = 0; i < playlistLen; i++)
    {
      playlistEntries[i].dur = duration;
    }
  }
  else
  {
    uint32_t lastDuration = 100;

    it = 0;

    for (uint32_t duration : durations)
    {
      if (it >= playlistLen)
      {
        break;
      }

      if (duration <= 1)
      {
        duration = 100;
      }

      lastDuration = duration;

      playlistEntries[it].dur = duration;

      ALOG_INF(PSTR("Playlist: dur[%u]=%lu"), it, (unsigned long)duration);

      it++;
    }

    for (; it < playlistLen; it++)
    {
      playlistEntries[it].dur = lastDuration;
    }
  }


  // ==================================================================
  // Transitions
  // ==================================================================

  JsonArray transitions = playlistObj["transition"];

  if (transitions.isNull())
  {
    uint32_t transition = playlistObj["transition"] | 0;

    for (uint8_t i = 0; i < playlistLen; i++)
    {
      playlistEntries[i].tr = transition;
    }
  }
  else
  {
    uint32_t lastTransition = 0;

    it = 0;

    for (uint32_t transition : transitions)
    {
      if (it >= playlistLen)
      {
        break;
      }

      lastTransition = transition;
      playlistEntries[it].tr = transition;

      ALOG_INF(PSTR("Playlist: transition[%u]=%lu"), it, (unsigned long)transition);

      it++;
    }

    for (; it < playlistLen; it++)
    {
      playlistEntries[it].tr = lastTransition;
    }
  }


  // ==================================================================
  // Time windows
  //
  // Human-readable format:
  //
  // "time_window":{
  //   "start":["00:00","04:00","08:00"],
  //   "end"  :["04:00","08:00","12:00"]
  // }
  //
  // Behaviour:
  //
  // 1 entry:
  //   applies the same window to the entire playlist
  //
  // N entries:
  //   entry i applies to playlist entry i
  //
  // "":
  //   unrestricted entry
  // ==================================================================

  #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS

  JsonObject timeWindow = playlistObj["time_window"];

  if (!timeWindow.isNull())
  {
    JsonArray startArray = timeWindow["start"];
    JsonArray endArray = timeWindow["end"];

    if (!startArray.isNull() && !endArray.isNull())
    {
      bool singleWindow = startArray.size() == 1 && endArray.size() == 1;

      for (uint8_t i = 0; i < playlistLen; i++)
      {
        uint8_t sourceIndex = singleWindow ? 0 : i;

        if (sourceIndex >= startArray.size())
        {
          continue;
        }

        if (sourceIndex >= endArray.size())
        {
          continue;
        }

        const char* startText = startArray[sourceIndex];
        const char* endText = endArray[sourceIndex];

        if (!startText || !endText)
        {
          continue;
        }

        if (!startText[0] || !endText[0])
        {
          continue;
        }

        uint16_t startMinute = 0;
        uint16_t endMinute = 0;

        if (!Playlist_ParseTime(startText, startMinute))
        {
          ALOG_WRN(PSTR("Playlist: invalid start time index=%u value=%s"), i, startText);
          continue;
        }

        if (!Playlist_ParseTime(endText, endMinute))
        {
          ALOG_WRN(PSTR("Playlist: invalid end time index=%u value=%s"), i, endText);
          continue;
        }

        playlistEntries[i].time_window_enabled = true;
        playlistEntries[i].time_start_minute = startMinute;
        playlistEntries[i].time_end_minute = endMinute;

        ALOG_INF(PSTR("Playlist: entry=%u preset=%u time=%s-%s"), i, playlistEntries[i].preset, startText, endText);
      }
    }
  }

  #endif // ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS


  // ==================================================================
  // Repeat
  // ==================================================================

  int rep = playlistObj[F("repeat")];

  bool shuffle = false;

  if (rep < 0)
  {
    rep = 0;
    shuffle = true;

    ALOG_INF(PSTR("Playlist: infinite shuffle"));
  }

  playlistRepeat = rep;

  if (playlistRepeat > 0)
  {
    playlistRepeat++;
  }


  // ==================================================================
  // End preset
  // ==================================================================

  playlistEndPreset = playlistObj["end"] | 0;

  if (playlistEndPreset == PRESET_ID_TEMP && currentPreset > 0)
  {
    playlistEndPreset = currentPreset;
  }

  // ==================================================================
  // Default / fallback preset
  //
  // Used when ALL playlist entries are currently blocked by time.
  // ==================================================================

  playlistDefaultPreset = playlistObj["default"] | 0;

  // ==================================================================
  // Shuffle
  // ==================================================================

  shuffle = shuffle || playlistObj["r"];

  if (shuffle)
  {
    playlistOptions |= PL_OPTION_SHUFFLE;
  }


  // ==================================================================
  // Runtime state
  // ==================================================================

  playlistIndex = -1;
  playlistEntryDur = 0;
  playlistFallbackActive = false;

  currentPlaylist = presetId;

  ALOG_INF(PSTR("Playlist %d loaded len=%u default=%u"), currentPlaylist, playlistLen, playlistDefaultPreset);

  return currentPlaylist;
}


/*******************************************************************************************************************************************************************************************************************
 * @description :
 *   Playlist runtime.
 *
 *   Responsibilities:
 *     - normal duration cycling
 *     - time-window eligibility
 *     - immediately remove an entry when its window expires
 *     - fallback preset when all entries are unavailable
 *     - return from fallback as soon as an entry becomes available
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::SubTask_Playlist()
{
  static uint32_t presetCycledTime = 0;
  static uint32_t lastTimeWindowCheck = 0;

  if (currentPlaylist < 0 || playlistEntries == nullptr || playlistLen == 0)
  {
    return;
  }


  uint32_t nowMs = millis();

  bool forceCycle = false;


  // ==================================================================
  // Periodic time eligibility check
  //
  // This means a preset whose window ends at 21:00 is removed at
  // approximately 21:00, rather than being allowed to finish a long
  // playlist duration.
  // ==================================================================

  #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS

  if ((uint32_t)(nowMs - lastTimeWindowCheck) >= 1000UL)
  {
    lastTimeWindowCheck = nowMs;

    uint16_t nowMinute = (uint16_t)tkr_time->MinutesPastMidnight();


    // ---------------------------------------------------------------
    // Current real playlist entry has become forbidden
    // ---------------------------------------------------------------

    if (!playlistFallbackActive && playlistIndex >= 0 && playlistIndex < playlistLen)
    {
      PlaylistEntry& currentEntry = playlistEntries[playlistIndex];

      if (currentEntry.time_window_enabled)
      {
        if (!Playlist_TimeAllowed(currentEntry.time_start_minute, currentEntry.time_end_minute, nowMinute))
        {
          ALOG_INF(PSTR("Playlist: current entry expired index=%d preset=%u"), playlistIndex, currentEntry.preset);

          forceCycle = true;
        }
      }
    }


    // ---------------------------------------------------------------
    // We are on fallback. See whether anything has become available.
    // ---------------------------------------------------------------

    if (playlistFallbackActive)
    {
      int16_t availableIndex = Playlist_SelectAllowedIndexByTime(0, nowMinute);

      if (availableIndex >= 0)
      {
        ALOG_INF(PSTR("Playlist: timed entry became available index=%d preset=%u"), availableIndex, playlistEntries[availableIndex].preset);

        forceCycle = true;
      }
    }
  }

  #endif // ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS


  // ==================================================================
  // Normal playlist duration
  // ==================================================================

  uint32_t entryDurationMs = playlistEntryDur * 100UL;

  bool durationExpired = playlistIndex < 0;

  if (!durationExpired)
  {
    durationExpired = (uint32_t)(nowMs - presetCycledTime) >= entryDurationMs;
  }

  if (!durationExpired && !forceCycle)
  {
    return;
  }

  presetCycledTime = nowMs;


  // ==================================================================
  // Calculate next candidate index
  // ==================================================================

  uint8_t nextIndex = 0;

  if (!playlistFallbackActive && playlistIndex >= 0)
  {
    nextIndex = (playlistIndex + 1) % playlistLen;
  }


  // ==================================================================
  // Time-window selection
  // ==================================================================

  #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS

  {
    uint16_t nowMinute = (uint16_t)tkr_time->MinutesPastMidnight();

    int16_t allowedIndex = Playlist_SelectAllowedIndexByTime(nextIndex, nowMinute);

    if (allowedIndex < 0)
    {
      // -------------------------------------------------------------
      // No normal playlist entry is currently allowed
      // -------------------------------------------------------------

      if (playlistDefaultPreset)
      {
        if (!playlistFallbackActive)
        {
          ALOG_INF(PSTR("Playlist: no entries allowed, applying default preset=%u"), playlistDefaultPreset);

          applyPreset(playlistDefaultPreset);
        }

        playlistFallbackActive = true;

        // This is only the re-check interval while on fallback.
        // 10 x 100 ms = 1 second.
        playlistEntryDur = 10;

        return;
      }


      // No default configured.
      // Hold whatever is currently displayed and re-check once/sec.

      ALOG_WRN(PSTR("Playlist: no entries allowed and no default configured"));

      playlistFallbackActive = true;
      playlistEntryDur = 10;

      return;
    }

    nextIndex = (uint8_t)allowedIndex;
  }

  #endif // ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS


  // ==================================================================
  // Leaving fallback
  // ==================================================================

  playlistFallbackActive = false;


  // ==================================================================
  // Detect playlist wrap
  //
  // We only treat it as a completed loop if the selected index wrapped
  // numerically backwards through the list.
  // ==================================================================

  bool rolledOver = false;

  if (playlistIndex >= 0)
  {
    rolledOver = nextIndex <= playlistIndex;
  }


  playlistIndex = nextIndex;


  // ==================================================================
  // Repeat handling
  // ==================================================================

  if (rolledOver)
  {
    if (playlistRepeat == 1)
    {
      unloadPlaylist();

      if (playlistEndPreset)
      {
        applyPreset(playlistEndPreset);
      }

      return;
    }

    if (playlistRepeat > 1)
    {
      playlistRepeat--;
    }

    if (playlistOptions & PL_OPTION_SHUFFLE)
    {
      shufflePlaylist();
    }
  }


  // ==================================================================
  // Apply selected playlist entry
  // ==================================================================

  PlaylistEntry& entry = playlistEntries[playlistIndex];

  playlistEntryDur = entry.dur;

  jsonTransitionOnce = true;

  // Transition value is now uint32_t.
  // Your transition engine can be updated separately.
  if (entry.tr)
  {
    tkr_anim->setTransition(entry.tr);
  }

  ALOG_INF(PSTR("Playlist%d index=%d/%u preset=%u dur=%lu transition=%lu"), currentPlaylist, playlistIndex, playlistLen, entry.preset, (unsigned long)entry.dur, (unsigned long)entry.tr);

  applyPreset(entry.preset);
}


/*******************************************************************************************************************************************************************************************************************
 * @description : Serialize playlist runtime state.
 ********************************************************************************************************************************************************************************************************************/
void mAnimatorLight::serializePlaylist(JsonObject sObj)
{
  JsonObject playlist = sObj.createNestedObject(F("playlist"));

  JsonArray ps = playlist.createNestedArray("ps");
  JsonArray dur = playlist.createNestedArray("dur");
  JsonArray transition = playlist.createNestedArray("transition");


  playlist[F("repeat")] = (playlistIndex < 0) ? playlistRepeat - 1 : playlistRepeat;

  playlist["end"] = playlistEndPreset;
  playlist["default"] = playlistDefaultPreset;
  playlist["r"] = playlistOptions & PL_OPTION_SHUFFLE;


  #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS

  JsonObject timeWindow = playlist.createNestedObject("time_window");

  JsonArray startArray = timeWindow.createNestedArray("start");
  JsonArray endArray = timeWindow.createNestedArray("end");

  #endif


  for (uint8_t i = 0; i < playlistLen; i++)
  {
    ps.add(playlistEntries[i].preset);

    dur.add(playlistEntries[i].dur);

    transition.add(playlistEntries[i].tr);


    #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS

    if (playlistEntries[i].time_window_enabled)
    {
      char startText[6];
      char endText[6];

      Playlist_MinuteToString(playlistEntries[i].time_start_minute, startText);
      Playlist_MinuteToString(playlistEntries[i].time_end_minute, endText);

      startArray.add(startText);
      endArray.add(endText);
    }
    else
    {
      startArray.add("");
      endArray.add("");
    }

    #endif
  }
}


#endif // ENABLE_FEATURE_LIGHTS__PLAYLISTS


/*


With your current command parser, use these exact payloads. Sleep/Wake require standby to be armed.

1. Enter preset 1 indefinitely
{
  "Standby":{
    "Arm":1,
    "Target":1,
    "AwakeSecs":0,
    "StandbySecs":0,
    "Sleep":1
  }
}

Then manually leave:

{
  "Standby":{
    "Wake":1
  }
}
2. Arm: wait 30 seconds → preset 1 → remain there until manual wake
{
  "Standby":{
    "Arm":1,
    "Target":1,
    "AwakeSecs":30,
    "StandbySecs":0
  }
}

Manual wake:

{
  "Standby":{
    "Wake":1
  }
}

A subsequent Wake will restart that 30-second awake countdown, which is the behaviour you will later want for motion.

3. Force standby preset 1 for exactly 60 seconds
{
  "Standby":{
    "Arm":1,
    "Target":1,
    "AwakeSecs":0,
    "StandbySecs":60,
    "Sleep":1
  }
}

After 60 seconds it automatically restores the captured state.

I explicitly set AwakeSecs:0 here so this behaves as a one-shot suppression rather than waking and then starting another automatic standby countdown.

4. Enter standby using playlist 210

Your current 210 is the timed playlist containing presets 1,2,3,4,5,7.

{
  "Standby":{
    "Arm":1,
    "Target":210,
    "AwakeSecs":0,
    "StandbySecs":0,
    "Sleep":1
  }
}

Leave it:

{
  "Standby":{
    "Wake":1
  }
}

Those four tests match the behaviour your current Standby command block exposes: Arm, Target, AwakeSecs, StandbySecs, Wake, and Sleep
















*/