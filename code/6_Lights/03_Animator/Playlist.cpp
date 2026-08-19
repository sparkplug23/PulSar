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
int16_t mAnimatorLight::loadPlaylist(JsonObject playlistObj, byte presetId)
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

      uint8_t foundIndex = 0;

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

    for (int ps : presets)
    {
      if (it >= playlistLen)
      {
        break;
      }

      playlistEntries[it].preset = (uint8_t)ps;

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

  if (playlistEndPreset == 255 && currentPreset > 0)
  {
    playlistEndPreset = currentPreset;
  }

  if (playlistEndPreset > 250)
  {
    playlistEndPreset = 0;
  }


  // ==================================================================
  // Default / fallback preset
  //
  // Used when ALL playlist entries are currently blocked by time.
  // ==================================================================

  playlistDefaultPreset = playlistObj["default"] | 0;

  if (playlistDefaultPreset > 250)
  {
    playlistDefaultPreset = 0;
  }


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


// #include "_AnimatorLight.h"

// #ifdef ENABLE_FEATURE_LIGHTS__PLAYLISTS

// /******
//  * 
//  * Need to make a new "standby" preset, with "timeon" end being switching to standby preset
//  * the idea is standby is on dim, or different colour etc
//  * 
//  * 
//  * 
//  * 
//  */

// void mAnimatorLight::shufflePlaylist() 
// {

//   ALOG_HGL(PSTR("shuffleplaylist"));
//   return; //block this

//   int currentIndex = playlistLen;
//   PlaylistEntry temporaryValue;

//   // While there remain elements to shuffle...
//   while (currentIndex--) {
//     // Pick a random element...
//     int randomIndex = random(0, currentIndex);
//     // And swap it with the current element.
//     temporaryValue = playlistEntries[currentIndex];
//     playlistEntries[currentIndex] = playlistEntries[randomIndex];
//     playlistEntries[randomIndex] = temporaryValue;
//   }
//   DEBUG_PRINTLN(F("Playlist shuffle."));

// }


// void mAnimatorLight::unloadPlaylist() 
// {
//   if (playlistEntries != nullptr) {
//     delete[] playlistEntries;
//     playlistEntries = nullptr;
//   }
//   currentPlaylist = playlistIndex = -1;
//   playlistLen = playlistEntryDur = playlistOptions = 0;
//   ALOG_INF(PSTR("unloadPlaylist"));
// }


// int16_t mAnimatorLight::loadPlaylist(JsonObject playlistObj, byte presetId) 
// {
//   ALOG_INF(PSTR("mAnimatorLight::loadPlaylist"));

//   unloadPlaylist();

//   JsonArray presets;       // numeric preset indices (ps)
//   bool useNames = false;   // true if we are using psn[]
  
//   JsonArray psn = playlistObj["psn"];

//   ALOG_INF(PSTR("Playlist name-based loading feature compiled in"));

//   if (!psn.isNull() && psn.size() > 0) {
//     useNames    = true;
//     playlistLen = psn.size();
//   } else
//   {
//     presets     = playlistObj["ps"];
//     playlistLen = presets.size();
//   }

//   if (playlistLen == 0) return -1;
//   if (playlistLen > 100) playlistLen = 100;

//   playlistEntries = new PlaylistEntry[playlistLen];
//   if (playlistEntries == nullptr) return -1;

//   byte it = 0;

// #ifdef ENABLE_DEVFEATURE_LIGHTS__PLAYLIST_NAME_BASED_LOADING
//   if (useNames) 
//   {
//     ALOG_INF(PSTR("Loading playlist via preset names (psn)"));

//     for (const char* name : psn) 
//     {


//       ALOG_INF(PSTR("psn name='%s'"), name);

//       if (it >= playlistLen) break;

//       byte  foundIndex = 0;
//       bool  ok         = false;

//       // resolve preset name to ID 1..250
//       /**
//        * Searching for preset defined as by name, in all presets "n"
//        * 
//        */
//       for (byte p = 1; p < 255; p++) 
//       {
//         String pname;
//         if (getPresetName(p, pname)) 
//         {
//           if (pname.equalsIgnoreCase(name)) 
//           {
//             foundIndex = p;
//             ok         = true;
//             break;
//           }
//         }
//       }

//       #ifdef ENABLE_DEVFEATURE_LIGHTS__PLAYLIST_NAME_BASED_LOADING
//       return -1;
//       #endif

//       if (!ok) 
//       {
//         ALOG_ERR(PSTR("Playlist error: preset name '%s' not found"), name);
//         foundIndex = 0; // invalid/placeholder
//       }

//       playlistEntries[it].preset = foundIndex;
//       ALOG_INF(PSTR("psn[%d] '%s' -> preset %d"), it, name, foundIndex);
//       it++;
//     }
//   }
//   else
// #endif
//   {
//     // original numeric "ps" behaviour
//     ALOG_INF(PSTR("Loading playlist via preset indices (ps)"));

//     for (int ps : presets) 
//     {
//       if (it >= playlistLen) break;
//       playlistEntries[it].preset = ps;
//       ALOG_INF(PSTR("ps[%d] -> %d"), it, ps);
//       it++;
//     }
//   }

//   // ---- durations ----
//   it = 0;
//   JsonArray durations = playlistObj["dur"];
//   if (durations.isNull()) 
//   {
//     ALOG_INF(PSTR("mAnimatorLight::loadPlaylist: durations.isNull()"));
//     playlistEntries[0].dur = playlistObj["dur"] | 100; // 10 seconds as fallback
//     it = 1;
//   } 
//   else 
//   {
//     for (int dur : durations) 
//     {
//       if (it >= playlistLen) break;
//       playlistEntries[it].dur = (dur > 1) ? dur : 100;
//       ALOG_INF(PSTR("durations.dur %d->%d secs"), dur, playlistEntries[it].dur);
//       it++;
//     }
//   }
//   // copy last duration forward for remaining entries
//   for (int i = it; i < playlistLen; i++) {
//     playlistEntries[i].dur = playlistEntries[it - 1].dur;
//   }

//   // ---- transitions ----
//   it = 0;
//   JsonArray tr = playlistObj[F("transition")];
//   if (tr.isNull()) 
//   {
//     playlistEntries[0].tr = playlistObj[F("transition")] | (transitionDelay / 100);
//     it = 1;
//   } 
//   else 
//   {
//     for (int transition : tr) 
//     {
//       if (it >= playlistLen) break;
//       playlistEntries[it].tr = transition;
//       it++;
//     }
//   }
//   for (int i = it; i < playlistLen; i++) {
//     playlistEntries[i].tr = playlistEntries[it - 1].tr;
//   }

//   // ---- repeat / shuffle / end ----
//   int  rep     = playlistObj[F("repeat")];
//   bool shuffle = false;
//   if (rep < 0) 
//   { 
//     // support negative values as infinite + shuffle
//     rep     = 0; 
//     shuffle = true;
//     ALOG_INF(PSTR("Shuffle started ======================================"));
//   }

//   playlistRepeat = rep;
//   if (playlistRepeat > 0) {
//     // add one extra repetition immediately since it will be deducted on first start
//     playlistRepeat++;
//   }

//   playlistEndPreset = playlistObj["end"] | 0;
//   // if end preset is 255 restore original preset (if any running) upon playlist end
//   if (playlistEndPreset == 255 && currentPreset > 0) {
//     playlistEndPreset = currentPreset;
//   }
//   if (playlistEndPreset > 250) {
//     playlistEndPreset = 0;
//   }

//   shuffle = shuffle || playlistObj["r"];
//   if (shuffle) {
//     playlistOptions += PL_OPTION_SHUFFLE;
//   }

//   currentPlaylist = presetId;

//   ALOG_INF(PSTR("Playlist %d loaded (len=%d)"), currentPlaylist, playlistLen);

//   return currentPlaylist;
// }



// void mAnimatorLight::SubTask_Playlist() 
// {


//   static unsigned long presetCycledTime = 0;

//   // if gDoc is not null JSON buffer is in use so just quit
//   // if (currentPlaylist < 0 || playlistEntries == nullptr || tkr_mfile->pDoc != nullptr) 
//   // {
//   //   ALOG_INF(PSTR("SubTask_Playlist return early"));    
//   //   return;
//   // }
//    if (currentPlaylist < 0 || playlistEntries == nullptr) return;



//   if(mTime::TimeReached(&tSaved_playlist_debug, 1000)){
//     uint32_t remaining = millis() - presetCycledTime;
//     remaining /= 1000;
//     ALOG_INF(PSTR("Playlist%d %d/%d secs, Preset %d/%d %d"), currentPlaylist, remaining, playlistEntryDur/10, playlistIndex, playlistLen, playlistEntries[playlistIndex].preset);
//     // ALOG_INF(PSTR("playlistIndex i%d/%d"), playlistIndex, playlistLen);
//     //ALOG_INF(PSTR("presetCycledTime %d<%d %d"), millis() , presetCycledTime, (100*playlistEntryDur) );
//   }


//   if (millis() - presetCycledTime > (100*playlistEntryDur)) 
//   {

//     // ALOG_INF(PSTR("tSaved_playlist_debug Running"));//, currentPlaylist, playlistEntryDur);

//     presetCycledTime = millis();
//     // if (tkr_iLight->getBri_Global() == 0 || nightlightActive) return;

//     ++playlistIndex %= playlistLen; // -1 at 1st run (limit to playlistLen)

//     #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS
//     {
//         uint16_t nowHM = mTime::Get_HHMM();   // Or however you obtain HHMM in your system

//         // IMPORTANT: You need access to the playlist JSON for this preset
//         // This retrieves the preset object again:
//         if (requestJSONBufferLock(31)) {
//             if (tkr_mfile->readObjectFromFileUsingId(getPresetsFileName(true),
//                                                     currentPlaylist,
//                                                     tkr_mfile->pDoc))
//             {
//                 JsonObject presetObj = tkr_mfile->pDoc->as<JsonObject>();
//                 JsonObject playlist  = presetObj["playlist"];

//                 if (!playlist.isNull()) {
//                     playlistIndex =
//                         Playlist_SelectAllowedIndexByTime(
//                             playlist,
//                             playlistIndex,
//                             nowHM
//                         );
//                 }
//             }
//             releaseJSONBufferLock();
//         }
//     }
//     #endif




//     // playlist roll-over
//     if (!playlistIndex) 
//     {
//       if (playlistRepeat == 1) { //stop if all repetitions are done
//         unloadPlaylist();
//         if (playlistEndPreset) applyPreset(playlistEndPreset);
//         return;
//       }
//       if (playlistRepeat > 1) playlistRepeat--; // decrease repeat count on each index reset if not an endless playlist
//       // playlistRepeat == 0: endless loop
//       // if (playlistOptions & PL_OPTION_SHUFFLE) shufflePlaylist(); // shuffle playlist and start over
//     }

//     jsonTransitionOnce = true;
//     playlistEntryDur = playlistEntries[playlistIndex].dur;
//     // AddLog_Array(LOG_LEVEL_INFO, "preset arr", playlistEntries[playlistIndex].preset, 10);
//     for(int i=0;i<10;i++){ Serial.println(playlistEntries[i].preset);};
//     applyPreset(playlistEntries[playlistIndex].preset);

//   }

//   /**
//    * @brief 
   
//    "94":{
//     "playlist":{
//       "ps" :[ 32,  34,  64, 65,   66,  74,  75,  76,  77,  79,  80],    // the preset index to load
//       "dur":[300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300],    // the duration, in 100ths of ms to load. So 300*100 is 30000ms or 30 seconds
//       "repeat":0,"end":0,"r":1,                                         // repeat 0 means forever, repeat number means an amount of times. "end" would be the last preset to stop at. 
//       "time_window":{  // If only one in array, then its the whole playlist limit. If the same length as the playlist, then each entry is the limit for that preset
//         "start":[1600, 1830],  // as HHMM only
//         "end":[1700, 1900]  // as HHMM only
//       }    
//     },
//     "on":true,
//     "n":"Meteor Colours"
//   }





//    * 
//    */

// }

// /***
//  * Playlist meanings
//  * 
//  * "repeat": [count] -1 means indefinitely, 0 to end is how many times
//  * "dur": [array] is the tengths of seconds
//  * "r":   [??] in this case appears to mean shuffle enabled, but not sure
//  * "end": [index] if repeat causes the playlist to end, then this is the index for the preset/playlist to switch to
//  * "ps":  [array] list of presets to load
//  * "transition": [array] no longer used, no WLED transitions remained
//  * 
//  */
// void mAnimatorLight::serializePlaylist(JsonObject sObj) 
// {

//   JsonObject playlist = sObj.createNestedObject(F("playlist"));
//   JsonArray ps = playlist.createNestedArray("ps");
//   JsonArray dur = playlist.createNestedArray("dur");

//   playlist[F("repeat")] = (playlistIndex < 0) ? playlistRepeat - 1 : playlistRepeat; // remove added repetition count (if not yet running)
//   playlist["end"] = playlistEndPreset;
//   playlist["r"] = playlistOptions & PL_OPTION_SHUFFLE;

//   for (int i=0; i<playlistLen; i++) 
//   {
//     ps.add(playlistEntries[i].preset);
//     dur.add(playlistEntries[i].dur);
//   }

// }


// #endif // ENABLE_FEATURE_LIGHTS__PLAYLISTS


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