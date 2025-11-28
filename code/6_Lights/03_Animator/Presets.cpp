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

const char *mAnimatorLight::getPresetsFileName(bool persistent) {
  return persistent ? presets_json : tmp_json;
}


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

  if (!requestJSONBufferLock(9)) return false;
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
  
  // if (FILE_SYSTEM.exists(getFileName()))
  // {
  //   ALOG_INF(PSTR("initPresetsFile() -- already exists, exiting"));   
  //   return;
  // }
  
  char fileName[33]; strncpy_P(fileName, getPresetsFileName(), 32); fileName[32] = 0; //use PROGMEM safe copy as FS.open() does not
  if (FILE_SYSTEM.exists(fileName)) return;



  StaticJsonDocument<64> doc;
  JsonObject sObj = doc.to<JsonObject>();
  sObj.createNestedObject("0");
  File f = FILE_SYSTEM.open(getPresetsFileName(), "w");
  
  ALOG_INF(PSTR("initPresetsFile() -- creating init file"));

  if (!f) {
    tkr_mfile->errorFlag = ERR_FS_GENERAL;
    return;
  }
  serializeJson(doc, f);
  f.close();

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
  if(requestDataBufferLock(GetModuleUniqueID()))
  {



    D_DATA_BUFFER_SOFT_CLEAR();

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

    releaseDataBufferLock();

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


#endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS