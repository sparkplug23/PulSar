#include "_AnimatorLight.h"


#ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS

/*
 * Methods to handle saving and loading presets to/from the filesystem
 * TODO:I want to serialise the preset file and send over mqtt anytime it changes as debug
 */

#ifdef ARDUINO_ARCH_ESP32
static char *tmpRAMbuffer = nullptr;
#endif

static volatile byte presetToApply = 0;
static volatile byte callModeToApply = 0;
static volatile byte presetToSave = 0;
static volatile int8_t saveLedmap = -1;
static char *quickLoad = nullptr;
static char *saveName = nullptr;
static bool includeBri = true, segBounds = true, selectedOnly = false, playlistSave = false;;

static const char presets_json[] PROGMEM = "/presets.json";
static const char tmp_json[] PROGMEM = "/tmp.json";
static const char presets_meta_json[] PROGMEM = "/presets_meta.json";

const char *mAnimatorLight::getPresetsFileName(bool persistent) {
  return persistent ? presets_json : tmp_json;
}
static const char* getPresetsMetaFileName() { //likely not needed, do direct later
  return (const char*)presets_meta_json;
}

// Date Modified: 12Dec25
#ifdef ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA

// Load metadata from /presets_meta.json
bool mAnimatorLight::LoadPresetFileMeta(PresetFileMeta &meta)
{
  if (!requestJSONBufferLock(21)) {
    ALOG_WRN(PSTR("LoadPresetFileMeta: failed to get JSON buffer lock"));
    return false;
  }

  tkr_mfile->pDoc->clear();

  char fname[33];
  strncpy_P(fname, presets_meta_json, 32);
  fname[32] = '\0';

  if (!FILE_SYSTEM.exists(fname)) {
    // No meta file yet → report "no meta", but fill sane defaults in RAM
    meta.enablePsn     = true;
    meta.parserVersion = kPresetMetaParserVersion;
    meta.lastScanMs    = 0;
    releaseJSONBufferLock();
    return false;
  }

  File f = FILE_SYSTEM.open(fname, "r");
  if (!f) {
    ALOG_WRN(PSTR("LoadPresetFileMeta: failed to open %s"), fname);
    releaseJSONBufferLock();
    return false;
  }

  DeserializationError err = deserializeJson(*tkr_mfile->pDoc, f);
  f.close();

  if (err) {
    ALOG_WRN(PSTR("LoadPresetFileMeta: JSON parse error (%d)"), (int)err.code());
    releaseJSONBufferLock();
    return false;
  }

  JsonObject m = tkr_mfile->pDoc->as<JsonObject>();

  meta.enablePsn              = (bool)(m["EnablePSN"]              | 1);
  meta.enablePlaylistTimeLocks= (bool)(m["EnablePlaylistTimeLocks"]| 0); // default OFF
  meta.parserVersion          = (uint8_t)(m["ParserVersion"]       | kPresetMetaParserVersion);
  meta.lastScanMs             = (uint32_t)(m["LastScanMs"]         | 0U);

  releaseJSONBufferLock();
  return true;
}

// Save metadata into /presets_meta.json
bool mAnimatorLight::SavePresetFileMeta(const PresetFileMeta &meta)
{
  if (!requestJSONBufferLock(22)) {
    ALOG_WRN(PSTR("SavePresetFileMeta: failed to get JSON buffer lock"));
    return false;
  }

  tkr_mfile->pDoc->clear();
  JsonObject m = tkr_mfile->pDoc->to<JsonObject>();

  m["EnablePSN"]              = meta.enablePsn ? 1 : 0;
  m["EnablePlaylistTimeLocks"]= meta.enablePlaylistTimeLocks ? 1 : 0;
  m["ParserVersion"]          = meta.parserVersion;
  m["LastScanMs"]             = meta.lastScanMs;

  char fname[33];
  strncpy_P(fname, presets_meta_json, 32);
  fname[32] = '\0';

  File f = FILE_SYSTEM.open(fname, "w");
  if (!f) {
    ALOG_WRN(PSTR("SavePresetFileMeta: failed to open %s for write"), fname);
    releaseJSONBufferLock();
    return false;
  }

  size_t written = serializeJson(*tkr_mfile->pDoc, f);
  f.close();

  if (written == 0) {
    ALOG_WRN(PSTR("SavePresetFileMeta: serializeJson wrote 0 bytes"));
    releaseJSONBufferLock();
    return false;
  }

  tkr_mfile->updateFSInfo();
  releaseJSONBufferLock();
  return true;
}

// Date Modified: 13Dec25
bool mAnimatorLight::IsPlaylistTimeLocksEnabled()
{
#ifdef ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
  PresetFileMeta meta;
  if (!LoadPresetFileMeta(meta)) {
    // If meta missing/unreadable, choose your default:
    return false; // or true if you want it on by default
  }
  return meta.enablePlaylistTimeLocks;
#else
  return false;
#endif
}

// Date Modified: 13Dec25
#ifdef ENABLE_FEATURE_LIGHTING__PLAYLIST_TIMELOCKS
uint8_t mAnimatorLight::Playlist_SelectAllowedIndexByTime(JsonObject playlist, uint8_t currentIndex, uint16_t nowHHMM)
{
  if (!IsPlaylistTimeLocksEnabled()) return currentIndex;

  JsonArray ps   = playlist["ps"];
  if (ps.isNull()) return currentIndex;

  uint8_t count = ps.size();
  if (count == 0) return currentIndex;

  JsonArray todS = playlist["todS"];
  JsonArray todE = playlist["todE"];

  // If no time arrays, nothing to do
  if (todS.isNull() || todE.isNull()) return currentIndex;

  uint8_t idx       = currentIndex;
  uint8_t attempts  = 0;

  while (attempts < count) {
    int16_t s = 0;
    int16_t e = 0;

    if (idx < todS.size()) s = todS[idx] | 0;
    if (idx < todE.size()) e = todE[idx] | 0;

    if (playlistEntryAllowedAtTime(s, e, nowHHMM)) {
      return idx; // allowed
    }

    // Skip this entry, move to next
    idx = (idx + 1) % count;
    attempts++;
  }

  // All entries locked out at this time. Options:
  //  - return currentIndex (hold)
  //  - or return some sentinel to pause playlist
  return currentIndex;
}
#endif


#endif // ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA



// // Date Modified: 12Dec25
// #ifdef ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA

// // struct PresetFileMeta {
// //   bool     enablePsn;     // 0/1: PSN helper enabled
// //   uint8_t  parserVersion; // version of PSN parser logic
// //   uint32_t lastScanMs;    // last PSN scan duration (ms)
// // };

// static const uint8_t kPresetMetaParserVersion = 1;

// // Date Modified: 12Dec25
// bool mAnimatorLight::LoadPresetFileMeta(PresetFileMeta &meta)
// {
//   // Try to read existing metadata from preset "0"
//   if (!requestJSONBufferLock(21)) {
//     ALOG_WRN(PSTR("LoadPresetFileMeta: failed to get JSON buffer lock"));
//     return false;
//   }

//   bool ok = tkr_mfile->readObjectFromFileUsingId(getPresetsFileName(), 0, tkr_mfile->pDoc);
//   JsonObject m;
//   bool needInit     = false;
//   bool needRefresh  = false;

//   if (ok) {
//     m = tkr_mfile->pDoc->as<JsonObject>();

//     // If "0" exists but is empty or missing fields, mark for refresh
//     bool hasEnable     = m.containsKey("EnablePSN");
//     bool hasParserVer  = m.containsKey("ParserVersion");
//     bool hasLastScanMs = m.containsKey("LastScanMs");

//     if (!hasEnable && !hasParserVer && !hasLastScanMs) {
//       // old style "0": {} or garbage, treat as missing
//       needInit = true;
//     } else {
//       // fill with defaults + existing values
//       meta.enablePsn     = (bool)(m["EnablePSN"]     | 1); // default ON
//       meta.parserVersion = (uint8_t)(m["ParserVersion"] | kPresetMetaParserVersion);
//       meta.lastScanMs    = (uint32_t)(m["LastScanMs"] | 0U);

//       // if some fields are missing, we will rewrite header with full set
//       needRefresh = (!hasEnable || !hasParserVer || !hasLastScanMs);
//     }
//   } else {
//     // No preset "0" at all → need full initialisation
//     needInit = true;
//   }

//   releaseJSONBufferLock();

//   if (needInit) {
//     // Initialise fresh metadata and write it
//     meta.enablePsn     = true;
//     meta.parserVersion = kPresetMetaParserVersion;
//     meta.lastScanMs    = 0;
//     ALOG_INF(PSTR("LoadPresetFileMeta: creating metadata in presets.json[0]"));
//     SavePresetFileMeta(meta);
//     return true;
//   }

//   if (needRefresh) {
//     ALOG_INF(PSTR("LoadPresetFileMeta: refreshing missing metadata fields in presets.json[0]"));
//     SavePresetFileMeta(meta);
//   }

//   // If we got here, either we successfully loaded existing meta,
//   // or we refreshed it; in both cases `meta` is valid.
//   return ok;
// }


// // Save metadata into preset "0"
// void mAnimatorLight::SavePresetFileMeta(const PresetFileMeta &meta)
// {
//   if (!requestJSONBufferLock(22)) return;

//   tkr_mfile->pDoc->clear();  // make sure we start from a clean doc

//   JsonObject m = tkr_mfile->pDoc->to<JsonObject>();
//   m["EnablePSN"]     = meta.enablePsn ? 1 : 0;
//   m["ParserVersion"] = meta.parserVersion;
//   m["LastScanMs"]    = meta.lastScanMs;

//   tkr_mfile->writeObjectToFileUsingId(getPresetsFileName(), 0, tkr_mfile->pDoc);

//   releaseJSONBufferLock();
// }

// #endif // ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA





void mAnimatorLight::doSaveState() 
{

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  ALOG_INF(PSTR("doSaveState() START"));
  // CommandSet_ReadFile("/presets.json");
  #endif
  
  unsigned long start = millis();
  while (isUpdating() && millis()-start < (2*FRAMETIME)+1) yield(); // wait 2 frames

  bool persist = (presetToSave < 251);
  const char *filename = getPresetsFileName(persist);
  
  if (!requestJSONBufferLock(10)) return; // will set gDoc

  initPresetsFile(); // just in case if someone deleted presets.json using /edit

  JsonObject sObj = tkr_mfile->pDoc->to<JsonObject>(); //needs done

  DEBUG_PRINTLN(F("Serialize current state"));
    

#ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLIST_BY_NAME_AUTOGENERATE_ID_LIST


#else


  if (playlistSave) 
  {
  
    #ifdef ENABLE_DEVFEATURE_LIGHTING__PLAYLISTS
    serializePlaylist(sObj);
    #endif
  
    if (includeBri) sObj["on"] = true;
  
  } 
  else 
  {
  
    serializeState(sObj, true, includeBri, segBounds, selectedOnly);
  
  }
  #endif

  sObj["n"] = saveName;
  if (quickLoad[0]) sObj[F("ql")] = quickLoad;
  if (saveLedmap >= 0) sObj[F("ledmap")] = saveLedmap;

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG_LINES
  DEBUG_LINE_HERE;
  DEBUG_PRINTLN(F("Serialized preset"));
  serializeJson(doc,Serial);
  DEBUG_PRINTLN();
  #endif

  #if defined(ARDUINO_ARCH_ESP32)
  if (!persist) 
  {

    if (tmpRAMbuffer!=nullptr) free(tmpRAMbuffer);

    size_t len = measureJson(*tkr_mfile->pDoc) + 1;

    ALOG_INF(PSTR("JSON Len=%d"),len);
  
    // if possible use SPI RAM on ESP32
    #if defined(BOARD_HAS_PSRAM) && defined(WLED_USE_PSRAM)
    if (psramFound())
      tmpRAMbuffer = (char*) ps_malloc(len);
    else
    #endif
  
    tmpRAMbuffer = (char*) malloc(len);
     
    if (tmpRAMbuffer!=nullptr) 
    {
        
      serializeJson(*tkr_mfile->pDoc, tmpRAMbuffer, len);
      
    }
    else
    {
  
      tkr_mfile->writeObjectToFileUsingId(filename, presetToSave, tkr_mfile->pDoc);
  
    }
  } 
  else
  {
  
    tkr_mfile->writeObjectToFileUsingId(filename, presetToSave, tkr_mfile->pDoc);
  
  }
  #endif

  if (persist) tkr_mfile->presetsModifiedTime = toki.second(); //unix time
  
  

  releaseJSONBufferLock();

  

  tkr_mfile->updateFSInfo();

  

  // clean up
  saveLedmap   = -1;
  presetToSave = 0;
  saveName[0]  = '\0';
  quickLoad[0] = '\0';
  playlistSave = false;

  #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS_DEBUG
  ALOG_INF(PSTR("doSaveState() END"));
  // CommandSet_ReadFile("/presets.json");
  #endif

}


bool mAnimatorLight::getPresetName(byte index, String& name)
{

  if (!requestJSONBufferLock(9))
  {
    ALOG_INF(PSTR("getPresetName() failed to get JSON buffer lock"));
    return false;  
  }  
  
  bool presetExists = false;
  if (tkr_mfile->readObjectFromFileUsingId(getPresetsFileName(), index, tkr_mfile->pDoc))
  {
    JsonObject fdo = tkr_mfile->pDoc->as<JsonObject>();
    if (fdo["n"]) {
      name = (const char*)(fdo["n"]);
      presetExists = true;
    }
  }
  releaseJSONBufferLock();
  return presetExists;

}


void mAnimatorLight::initPresetsFile()
{
  char fileName[33];
  strncpy_P(fileName, getPresetsFileName(), 32);
  fileName[32] = 0; //use PROGMEM safe copy as FS.open() does not

  if (FILE_SYSTEM.exists(fileName)) return;

  #ifdef ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
    // Create presets.json with metadata object at "0"
    StaticJsonDocument<128> doc;
    JsonObject root = doc.to<JsonObject>();
    JsonObject meta = root.createNestedObject("0");
    meta["EnablePSN"]     = 1;
    meta["ParserVersion"] = 1;   // must match kPresetMetaParserVersion
    meta["LastScanMs"]    = 0;

    File f = FILE_SYSTEM.open(fileName, "w");
    if (!f) {
      tkr_mfile->errorFlag = ERR_FS_GENERAL;
      return;
    }
    serializeJson(doc, f);
    f.close();
  #else
    // Original behaviour: dummy empty "0" object
    StaticJsonDocument<64> doc;
    JsonObject sObj = doc.to<JsonObject>();
    sObj.createNestedObject("0");
    File f = WLED_FS.open(fileName, "w");
    if (!f) {
      errorFlag = ERR_FS_GENERAL;
      return;
    }
    serializeJson(doc, f);
    f.close();
  #endif

}


bool mAnimatorLight::applyPreset(byte index, byte callMode)
{
  ALOG_INF(PSTR("Request to apply preset: %d"), index);
  presetToApply = index;
  callModeToApply = callMode;
  return true;
}


// apply preset or fallback to a effect and palette if it doesn't exist
void mAnimatorLight::applyPresetWithFallback(uint8_t index, uint8_t callMode, uint8_t effectID, uint8_t paletteID)
{
  applyPreset(index, callMode);  
  effectCurrent = effectID; // these two will be overwritten if preset exists in SubTask_Presets()
  effectPalette = paletteID; // these two will be overwritten if preset exists in SubTask_Presets()
}


/**
 * @brief Gets called every loop, returns if none are waiting
 * 
 */
void mAnimatorLight::SubTask_Presets()
{

  if (presetToSave) 
  {
    // ALOG_INF(PSTR(" (presetToSave)()"));
    doSaveState();
    return;
  }

  if (presetToApply == 0 || !requestJSONBufferLock(9))
  {
    // ALOG_INF(PSTR("(presetToApply == 0 || gDoc)()"));    
    return; // no preset waiting to apply, or JSON buffer is already allocated, return to loop until free
  }

  ALOG_INF(PSTR("mAnimatorLight::SubTask_Presets presetToApply %d"), presetToApply);


  

  bool changePreset = false;
  uint8_t tmpPreset = presetToApply; // store temporary since deserializeState() may call applyPreset()
  uint8_t tmpMode   = callModeToApply;
 
  

  JsonObject fdo;
  const char *filename = getPresetsFileName(tmpPreset < 255);

  // allocate buffer
  // if (!requestJSONBufferLock(9)) return;  // will also assign gDoc

  presetToApply = 0; //clear request for preset
  callModeToApply = 0;

  ALOG_INF(PSTR("Applying preset:%d"), tmpPreset);

  #if defined(ARDUINO_ARCH_ESP32S2) || defined(ARDUINO_ARCH_ESP32C3)
  unsigned long maxWait = millis() + tkr_lAni->getFrameTime();
  while (tkr_lAni->isUpdating() && millis() < maxWait) delay(1); // wait for strip to finish updating, accessing FS during sendout causes glitches
  #endif

  #ifdef ARDUINO_ARCH_ESP32
  if (tmpPreset==255 && tmpRAMbuffer!=nullptr) 
  { 
    deserializeJson(*tkr_mfile->pDoc,tmpRAMbuffer);
    tkr_mfile->errorFlag = ERR_NONE;
  } 
  else
  #endif
  { 


    tkr_mfile->errorFlag = tkr_mfile->readObjectFromFileUsingId(filename, tmpPreset, tkr_mfile->pDoc) ? ERR_NONE : ERR_FS_PLOAD;

  } 

  

  fdo = tkr_mfile->pDoc->as<JsonObject>();

  

  // ALOG_HGL(PSTR("mAnimatorLight::SubTask_Presets is now being read as gDoc and not compatable with my command structure"));

  // delay(4000);

  /**
   * @brief Run through my command structure.
   * 
   */
  if(data_buffer.requestLock(GetModuleUniqueID()))
  {



    data_buffer.ClearSoft();

    // Serialise from ArduinoJson into buffer for parser to load
    serializeJson(*tkr_mfile->pDoc, data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr));

    LoggingLevels level = LOG_LEVEL_INFO;
    #ifdef ENABLE_DEVFEATURE_SHOW_INCOMING_MQTT_COMMANDS
    level = LOG_LEVEL_DEV_TEST;
    #endif
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(level, PSTR(D_LOG_LIGHT "My parser payload [len:%d] %s"), data_buffer.payload.length_used,data_buffer.payload.ctr);
    #endif// ENABLE_LOG_LEVEL_INFO

    pCONT->Tasker_Interface(TASK_JSON_COMMAND_ID);

    data_buffer.releaseLock();

  }


  //HTTP API commands
  const char* httpwin = fdo["win"];
  if (httpwin) { 


    String apireq = "win"; // reduce flash string usage
    apireq += F("&IN&"); // internal call
    apireq += httpwin;
    #ifdef ENABLE_DEVFEATURE_LIGHTING__SETTINGS
    handleSet(nullptr, apireq, false); // may call applyPreset() via PL=
    setValuesFromFirstSelectedSeg(); // fills legacy values
    #endif // ENABLE_DEVFEATURE_LIGHTING__SETTINGS
    changePreset = true;

  } 
  else
  { 


    ALOG_INF(PSTR("Here might be what resets the segments without my commands"));

    if (!fdo["seg"].isNull() || !fdo["on"].isNull() || !fdo["bri"].isNull() || !fdo["nl"].isNull() || !fdo["ps"].isNull() || !fdo[F("playlist")].isNull()) changePreset = true;
    if (!(tmpMode == CALL_MODE_BUTTON_PRESET && fdo["ps"].is<const char *>() && strchr(fdo["ps"].as<const char *>(),'~') != strrchr(fdo["ps"].as<const char *>(),'~')))
      fdo.remove("ps"); // remove load request for presets to prevent recursive crash (if not called by button and contains preset cycling string "1~5~")
    deserializeState(fdo, CALL_MODE_NO_NOTIFY, tmpPreset); // may change presetToApply by calling applyPreset()

  }

  if (!tkr_mfile->errorFlag && tmpPreset < 255 && changePreset)
  {
    presetCycCurr = currentPreset = tmpPreset;
  }
 
  

  #if defined(ARDUINO_ARCH_ESP32)
  //Aircoookie recommended not to delete buffer
  if (tmpPreset==255 && tmpRAMbuffer!=nullptr) {
    free(tmpRAMbuffer);
    tmpRAMbuffer = nullptr;
  }
  #endif

  

  releaseJSONBufferLock(); // will also clear gDoc

  if (changePreset) notify(tmpMode); // force UDP notification

  // stateUpdated(tmpMode);  // was colorUpdated() if anything breaks
  Serial.println("stateUpdated() missing");
  // updateInterfaces(tmpMode);
  Serial.println("updateInterfaces() missing");

}

#ifdef ENABLE_FEATURE_LIGHTS__DEMO_MODE
void mAnimatorLight::SubTask_Demo()
{

}
#endif


//called from handleSet(PS=) [network callback (gDoc==nullptr), IR (irrational), deserializeState, UDP] and deserializeState() [network callback (filedoc!=nullptr)]
void mAnimatorLight::savePreset(byte index, const char* pname, JsonObject sObj)
{

  ALOG_INF(PSTR(DEBUG_INSERT_PAGE_BREAK "savePreset()"));

  if (index == 0 || (index > 250 && index < 255)) return;
  if (pname) strlcpy(saveName, pname, 33);
  else {
    if (sObj["n"].is<const char*>()) strlcpy(saveName, sObj["n"].as<const char*>(), 33);
    else                             sprintf_P(saveName, PSTR("Preset %d"), index);
  }

  DEBUG_PRINT(F("Saving preset (")); DEBUG_PRINT(index); DEBUG_PRINT(F(") ")); DEBUG_PRINTLN(saveName);

  presetToSave = index;
  playlistSave = false;
  if (sObj[F("ql")].is<const char*>())
  {
    strlcpy(quickLoad, sObj[F("ql")].as<const char*>(), 9); // client limits QL to 2 chars, buffer for 8 bytes to allow unicode
  } 
  
  if (sObj["o"].isNull()) 
  { // no "o" means not a playlist or custom API call, saving of state is async (not immediately)

    includeBri   = sObj["ib"].as<bool>() || index==255; // temporary preset needs brightness
    segBounds    = sObj["sb"].as<bool>() || index==255; // temporary preset needs bounds
    selectedOnly = sObj[F("sc")].as<bool>();
    saveLedmap   = sObj[F("ledmap")] | -1;

  } 
  else 
  {

    // this is a playlist or API call
    if (sObj[F("playlist")].isNull()) 
    {
      // we will save API call immediately (often causes presets.json corruption)
      presetToSave = 0;
      if (index > 250 || !tkr_mfile->pDoc) return; // cannot save API calls to temporary preset (255)
      sObj.remove("o");
      sObj.remove("v");
      sObj.remove("time");
      sObj.remove(F("error"));
      sObj.remove(F("psave"));
      if (sObj["n"].isNull()) sObj["n"] = saveName;
      initPresetsFile(); // just in case if someone deleted presets.json using /edit
      tkr_mfile->writeObjectToFileUsingId(getPresetsFileName(index<255), index, tkr_mfile->pDoc);    
      
      tkr_mfile->presetsModifiedTime = toki.second(); //unix time  

      tkr_mfile->updateFSInfo();

    } 
    else 
    {
  
      // store playlist
      // WARNING: playlist will be loaded in json.cpp after this call and will have repeat counter increased by 1
      includeBri   = true; // !sObj["on"].isNull();
      playlistSave = true;

    }
  }
  
  

}


void mAnimatorLight::deletePreset(byte index) 
{

  StaticJsonDocument<24> empty;
  tkr_mfile->writeObjectToFileUsingId(getPresetsFileName(), index, &empty);
  tkr_mfile->presetsModifiedTime = toki.second(); //unix time
  tkr_mfile->updateFSInfo();

}


#ifdef ENABLE_DEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25

void mAnimatorLight::ScanPresetsFile_GeneratePlaylistIDsFromPSN_2()
{
  ALOG_INF(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN2() begin"));  Serial.flush();

  uint32_t t_start = millis();

  #ifdef ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
  PresetFileMeta meta;
  bool haveMeta = LoadPresetFileMeta(meta);

  if (!haveMeta) {
    // No meta file yet or failed to read → defaults already set in Load
    ALOG_INF(PSTR("ScanPresetsFile...: no presets_meta.json, using defaults"));
  }

  if (!meta.enablePsn) {
    ALOG_INF(PSTR("PSN processing disabled via presets_meta.json.EnablePSN"));
    return;
  }
  #endif

  // Resolve persistent presets file name (PROGMEM-safe)
  char presetsFileName[33];
  strncpy_P(presetsFileName, getPresetsFileName(true), 32);
  presetsFileName[32] = '\0';

  ALOG_INF(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN() %s"),presetsFileName);  Serial.flush();

  // Sanity: if presets.json doesn't exist, nothing to do
  if (!FILE_SYSTEM.exists(presetsFileName)) {
    ALOG_WRN(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: %s not found"), presetsFileName);
    return;
  }
  ALOG_INF(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN() B"));  Serial.flush();

#ifdef ENABLE_DEBUGFEATURE_LIGHTING__PLAYLIST_PSN_TO_PS_CREATE_BACKUP_FILE
  // --- Create backup: presets_bu.json ---
  {
    const char *backupName = "/presets_bu.json";

    // Remove old backup if present
    if (FILE_SYSTEM.exists(backupName)) {
      FILE_SYSTEM.remove(backupName);
    }

    File src = FILE_SYSTEM.open(presetsFileName, "r");
    if (!src) {
      ALOG_WRN(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: failed to open %s for backup"), presetsFileName);
      return;
    }

    File dst = FILE_SYSTEM.open(backupName, "w");
    if (!dst) {
      ALOG_WRN(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: failed to open %s for backup"), backupName);
      src.close();
      return;
    }

    uint8_t buf[256];
    while (true) {
      size_t n = src.read(buf, sizeof(buf));
      if (!n) break;
      dst.write(buf, n);
    }

    src.close();
    dst.close();

    ALOG_INF(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: backup created as %s"), backupName);
  }
#endif // ENABLE_DEBUGFEATURE_LIGHTING__PLAYLIST_PSN_TO_PS_CREATE_BACKUP_FILE

  const char *fname = getPresetsFileName(true); // used by tkr_mfile helpers

  // One doc reused for all JSON work (both passes).
  DynamicJsonDocument doc(2048);

  // Upper bound on how many entries we expect in a playlist psn[]
  static const uint8_t MAX_PSN_PER_PLAYLIST = 32;

  // --- PASS 0: build name->id table for all presets 1..250 (in RAM) ---

  struct PresetNameEntry {
    uint8_t id;
    char    name[33]; // match WLED default name length + NUL
  };

  // Static to keep it out of stack; 251 entries is ~8.5kB, fine in .bss.
  static PresetNameEntry nameTable[251];
  uint8_t nameCount = 0;

  for (uint8_t presetId = 1; presetId < 251; presetId++) {
    doc.clear();
    if (!tkr_mfile->readObjectFromFileUsingId(fname, presetId, &doc)) {
      continue; // unused / invalid preset index
    }

    JsonObject fdo   = doc.as<JsonObject>();
    const char* cname = fdo["n"];
    if (!cname || !cname[0]) {
      continue; // presets without a name are not candidates for psn resolution
    }

    if (nameCount >= 251) {
      // Shouldn't really happen, but guard anyway
      break;
    }

    nameTable[nameCount].id = presetId;
    strlcpy(nameTable[nameCount].name, cname, sizeof(nameTable[nameCount].name));
    nameCount++;
  }

  ALOG_INF(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: built name table with %u entries"),
           (unsigned)nameCount);
  Serial.flush();

  auto findIdByName = [&](const char *targetName) -> uint8_t {
    if (!targetName || !targetName[0]) return 0;
    for (uint8_t i = 0; i < nameCount; i++) {
      if (strcasecmp(nameTable[i].name, targetName) == 0) {
        return nameTable[i].id;
      }
    }
    return 0;
  };

  // --- PASS 1: for each preset with playlist.psn, regenerate playlist.ps ---

  for (uint8_t playlistId = 1; playlistId < 251; playlistId++) {
    doc.clear();

    if (!tkr_mfile->readObjectFromFileUsingId(fname, playlistId, &doc)) {
      continue;  // preset index unused
    }

    JsonObject fdo      = doc.as<JsonObject>();
    JsonObject playlist = fdo["playlist"];
    if (playlist.isNull()) continue;

    JsonArray psn = playlist["psn"];
    if (psn.isNull() || psn.size() == 0) continue;

    // Copy psn names into a small local array to avoid dangling pointers
    char   psnNames[MAX_PSN_PER_PLAYLIST][33];
    uint8_t psnCount = 0;

    for (const char* name : psn) {
      if (psnCount >= MAX_PSN_PER_PLAYLIST) {
        ALOG_WRN(
          PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: preset %d psn truncated at %u entries"),
          playlistId, (unsigned)MAX_PSN_PER_PLAYLIST
        );
        break;
      }
      if (!name) {
        psnNames[psnCount][0] = '\0';
      } else {
        strlcpy(psnNames[psnCount], name, sizeof(psnNames[psnCount]));
      }
      psnCount++;
    }

    ALOG_INF(
      PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: preset %d has psn[%u], regenerating ps"),
      playlistId, (unsigned)psnCount
    );
    Serial.flush();

    // Resolve each psn name using the pre-built name table
    uint8_t psIds[MAX_PSN_PER_PLAYLIST];
    for (uint8_t i = 0; i < psnCount; i++) {
      const char* targetName = psnNames[i];
      uint8_t foundId = findIdByName(targetName);

      if (!foundId) {
        ALOG_WRN(
          PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN: name '%s' in preset %d not found, mapping to 0"),
          targetName ? targetName : "(null)", playlistId
        );
        Serial.flush();
      }

      psIds[i] = foundId;
    }

    // Re-read the playlist preset so we have its full JSON again
    doc.clear();
    if (!tkr_mfile->readObjectFromFileUsingId(fname, playlistId, &doc)) {
      continue; // something odd; skip writing
    }

    fdo      = doc.as<JsonObject>();
    playlist = fdo["playlist"];
    if (playlist.isNull()) continue;

    JsonArray ps = playlist["ps"];
    if (!ps.isNull()) {
      ps.clear();
    } else {
      ps = playlist.createNestedArray("ps");
    }

    for (uint8_t i = 0; i < psnCount; i++) {
      ps.add(psIds[i]);
    }

    // Write back updated preset
    tkr_mfile->writeObjectToFileUsingId(fname, playlistId, &doc);
  }


#ifdef ENABLE_FEATURE_LIGHTING__PRESET_FILE_METADATA
  uint32_t t_elapsed = millis() - t_start;

  meta.parserVersion = kPresetMetaParserVersion;
  if (meta.lastScanMs != t_elapsed) {
    meta.lastScanMs = t_elapsed;
    SavePresetFileMeta(meta); // if FS is *really* full, this can still fail, but it's a tiny file
  }
#endif

ALOG_INF(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN_2() end, took %u ms"),
         (unsigned)t_elapsed);


  ALOG_INF(PSTR("ScanPresetsFile_GeneratePlaylistIDsFromPSN() end"));
  Serial.flush();
}

#endif // ENABLE_DEVFEATURE_LIGHT__PLAYLIST_NAME_BASED_LOADING_4DEC25



#endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS