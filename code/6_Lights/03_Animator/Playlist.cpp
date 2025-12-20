#include "_AnimatorLight.h"

#ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS

/******
 * 
 * Need to make a new "standby" preset, with "timeon" end being switching to standby preset
 * the idea is standby is on dim, or different colour etc
 * 
 * 
 * 
 * 
 */

void mAnimatorLight::shufflePlaylist() 
{

  ALOG_HGL(PSTR("shuffleplaylist"));
  return; //block this

  int currentIndex = playlistLen;
  PlaylistEntry temporaryValue;

  // While there remain elements to shuffle...
  while (currentIndex--) {
    // Pick a random element...
    int randomIndex = random(0, currentIndex);
    // And swap it with the current element.
    temporaryValue = playlistEntries[currentIndex];
    playlistEntries[currentIndex] = playlistEntries[randomIndex];
    playlistEntries[randomIndex] = temporaryValue;
  }
  DEBUG_PRINTLN(F("Playlist shuffle."));

}


void mAnimatorLight::unloadPlaylist() 
{
  if (playlistEntries != nullptr) {
    delete[] playlistEntries;
    playlistEntries = nullptr;
  }
  currentPlaylist = playlistIndex = -1;
  playlistLen = playlistEntryDur = playlistOptions = 0;
  ALOG_INF(PSTR("unloadPlaylist"));
}


int16_t mAnimatorLight::loadPlaylist(JsonObject playlistObj, byte presetId) 
{
  ALOG_INF(PSTR("mAnimatorLight::loadPlaylist"));

  unloadPlaylist();

  JsonArray presets;       // numeric preset indices (ps)
  bool useNames = false;   // true if we are using psn[]
  
#ifdef ENABLE_DEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  JsonArray psn = playlistObj["psn"];

  #ifdef ENABLE_DEVFEATURE_LIGHT__GRADIENT_PATCH_4DEC25
  ALOG_INF(PSTR("Playlist name-based loading feature compiled in"));
  #endif

  if (!psn.isNull() && psn.size() > 0) {
    useNames    = true;
    playlistLen = psn.size();
  } else
#endif
  {
    presets     = playlistObj["ps"];
    playlistLen = presets.size();
  }

  if (playlistLen == 0) return -1;
  if (playlistLen > 100) playlistLen = 100;

  playlistEntries = new PlaylistEntry[playlistLen];
  if (playlistEntries == nullptr) return -1;

  byte it = 0;

#ifdef ENABLEDEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
  if (useNames) 
  {
    ALOG_INF(PSTR("Loading playlist via preset names (psn)"));

    for (const char* name : psn) 
    {


      ALOG_INF(PSTR("psn name='%s'"), name);

      if (it >= playlistLen) break;

      byte  foundIndex = 0;
      bool  ok         = false;

      // resolve preset name to ID 1..250
      /**
       * Searching for preset defined as by name, in all presets "n"
       * 
       */
      for (byte p = 1; p < 255; p++) 
      {
        String pname;
        if (getPresetName(p, pname)) 
        {
          if (pname.equalsIgnoreCase(name)) 
          {
            foundIndex = p;
            ok         = true;
            break;
          }
        }
      }

      #ifdef ENABLEDEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25
      return -1;
      #endif

      if (!ok) 
      {
        ALOG_ERR(PSTR("Playlist error: preset name '%s' not found"), name);
        foundIndex = 0; // invalid/placeholder
      }

      playlistEntries[it].preset = foundIndex;
      ALOG_INF(PSTR("psn[%d] '%s' -> preset %d"), it, name, foundIndex);
      it++;
    }
  }
  else
#endif
  {
    // original numeric "ps" behaviour
    ALOG_INF(PSTR("Loading playlist via preset indices (ps)"));

    for (int ps : presets) 
    {
      if (it >= playlistLen) break;
      playlistEntries[it].preset = ps;
      ALOG_INF(PSTR("ps[%d] -> %d"), it, ps);
      it++;
    }
  }

  // ---- durations ----
  it = 0;
  JsonArray durations = playlistObj["dur"];
  if (durations.isNull()) 
  {
    ALOG_INF(PSTR("mAnimatorLight::loadPlaylist: durations.isNull()"));
    playlistEntries[0].dur = playlistObj["dur"] | 100; // 10 seconds as fallback
    it = 1;
  } 
  else 
  {
    for (int dur : durations) 
    {
      if (it >= playlistLen) break;
      playlistEntries[it].dur = (dur > 1) ? dur : 100;
      ALOG_INF(PSTR("durations.dur %d->%d secs"), dur, playlistEntries[it].dur);
      it++;
    }
  }
  // copy last duration forward for remaining entries
  for (int i = it; i < playlistLen; i++) {
    playlistEntries[i].dur = playlistEntries[it - 1].dur;
  }

  // ---- transitions ----
  it = 0;
  JsonArray tr = playlistObj[F("transition")];
  if (tr.isNull()) 
  {
    playlistEntries[0].tr = playlistObj[F("transition")] | (transitionDelay / 100);
    it = 1;
  } 
  else 
  {
    for (int transition : tr) 
    {
      if (it >= playlistLen) break;
      playlistEntries[it].tr = transition;
      it++;
    }
  }
  for (int i = it; i < playlistLen; i++) {
    playlistEntries[i].tr = playlistEntries[it - 1].tr;
  }

  // ---- repeat / shuffle / end ----
  int  rep     = playlistObj[F("repeat")];
  bool shuffle = false;
  if (rep < 0) 
  { 
    // support negative values as infinite + shuffle
    rep     = 0; 
    shuffle = true;
    ALOG_INF(PSTR("Shuffle started ======================================"));
  }

  playlistRepeat = rep;
  if (playlistRepeat > 0) {
    // add one extra repetition immediately since it will be deducted on first start
    playlistRepeat++;
  }

  playlistEndPreset = playlistObj["end"] | 0;
  // if end preset is 255 restore original preset (if any running) upon playlist end
  if (playlistEndPreset == 255 && currentPreset > 0) {
    playlistEndPreset = currentPreset;
  }
  if (playlistEndPreset > 250) {
    playlistEndPreset = 0;
  }

  shuffle = shuffle || playlistObj["r"];
  if (shuffle) {
    playlistOptions += PL_OPTION_SHUFFLE;
  }

  currentPlaylist = presetId;

  ALOG_INF(PSTR("Playlist %d loaded (len=%d)"), currentPlaylist, playlistLen);

  return currentPlaylist;
}



void mAnimatorLight::SubTask_Playlist() 
{


  static unsigned long presetCycledTime = 0;

  // if gDoc is not null JSON buffer is in use so just quit
  // if (currentPlaylist < 0 || playlistEntries == nullptr || tkr_mfile->pDoc != nullptr) 
  // {
  //   ALOG_INF(PSTR("SubTask_Playlist return early"));    
  //   return;
  // }
   if (currentPlaylist < 0 || playlistEntries == nullptr) return;



  if(mTime::TimeReached(&tSaved_playlist_debug, 1000)){
    uint32_t remaining = millis() - presetCycledTime;
    remaining /= 1000;
    ALOG_INF(PSTR("Playlist%d %d/%d secs, Preset %d/%d %d"), currentPlaylist, remaining, playlistEntryDur/10, playlistIndex, playlistLen, playlistEntries[playlistIndex].preset);
    // ALOG_INF(PSTR("playlistIndex i%d/%d"), playlistIndex, playlistLen);
    //ALOG_INF(PSTR("presetCycledTime %d<%d %d"), millis() , presetCycledTime, (100*playlistEntryDur) );
  }


  if (millis() - presetCycledTime > (100*playlistEntryDur)) 
  {

    // ALOG_INF(PSTR("tSaved_playlist_debug Running"));//, currentPlaylist, playlistEntryDur);

    presetCycledTime = millis();
    // if (tkr_iLight->getBri_Global() == 0 || nightlightActive) return;

    ++playlistIndex %= playlistLen; // -1 at 1st run (limit to playlistLen)

    #ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS
    {
        uint16_t nowHM = mTime::Get_HHMM();   // Or however you obtain HHMM in your system

        // IMPORTANT: You need access to the playlist JSON for this preset
        // This retrieves the preset object again:
        if (requestJSONBufferLock(31)) {
            if (tkr_mfile->readObjectFromFileUsingId(getPresetsFileName(true),
                                                    currentPlaylist,
                                                    tkr_mfile->pDoc))
            {
                JsonObject presetObj = tkr_mfile->pDoc->as<JsonObject>();
                JsonObject playlist  = presetObj["playlist"];

                if (!playlist.isNull()) {
                    playlistIndex =
                        Playlist_SelectAllowedIndexByTime(
                            playlist,
                            playlistIndex,
                            nowHM
                        );
                }
            }
            releaseJSONBufferLock();
        }
    }
    #endif




    // playlist roll-over
    if (!playlistIndex) 
    {
      if (playlistRepeat == 1) { //stop if all repetitions are done
        unloadPlaylist();
        if (playlistEndPreset) applyPreset(playlistEndPreset);
        return;
      }
      if (playlistRepeat > 1) playlistRepeat--; // decrease repeat count on each index reset if not an endless playlist
      // playlistRepeat == 0: endless loop
      // if (playlistOptions & PL_OPTION_SHUFFLE) shufflePlaylist(); // shuffle playlist and start over
    }

    jsonTransitionOnce = true;
    playlistEntryDur = playlistEntries[playlistIndex].dur;
    // AddLog_Array(LOG_LEVEL_INFO, "preset arr", playlistEntries[playlistIndex].preset, 10);
    for(int i=0;i<10;i++){ Serial.println(playlistEntries[i].preset);};
    applyPreset(playlistEntries[playlistIndex].preset);

  }

  /**
   * @brief 
   
   "94":{
    "playlist":{
      "ps" :[ 32,  34,  64, 65,   66,  74,  75,  76,  77,  79,  80],    // the preset index to load
      "dur":[300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300],    // the duration, in 100ths of ms to load. So 300*100 is 30000ms or 30 seconds
      "repeat":0,"end":0,"r":1,                                         // repeat 0 means forever, repeat number means an amount of times. "end" would be the last preset to stop at. 
      "time_window":{  // If only one in array, then its the whole playlist limit. If the same length as the playlist, then each entry is the limit for that preset
        "start":[1600, 1830],  // as HHMM only
        "end":[1700, 1900]  // as HHMM only
      }    
    },
    "on":true,
    "n":"Meteor Colours"
  }





   * 
   */

}

/***
 * Playlist meanings
 * 
 * "repeat": [count] -1 means indefinitely, 0 to end is how many times
 * "dur": [array] is the tengths of seconds
 * "r":   [??] in this case appears to mean shuffle enabled, but not sure
 * "end": [index] if repeat causes the playlist to end, then this is the index for the preset/playlist to switch to
 * "ps":  [array] list of presets to load
 * "transition": [array] no longer used, no WLED transitions remained
 * 
 */
void mAnimatorLight::serializePlaylist(JsonObject sObj) 
{

  JsonObject playlist = sObj.createNestedObject(F("playlist"));
  JsonArray ps = playlist.createNestedArray("ps");
  JsonArray dur = playlist.createNestedArray("dur");

  playlist[F("repeat")] = (playlistIndex < 0) ? playlistRepeat - 1 : playlistRepeat; // remove added repetition count (if not yet running)
  playlist["end"] = playlistEndPreset;
  playlist["r"] = playlistOptions & PL_OPTION_SHUFFLE;

  for (int i=0; i<playlistLen; i++) 
  {
    ps.add(playlistEntries[i].preset);
    dur.add(playlistEntries[i].dur);
  }

}


#endif // ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS