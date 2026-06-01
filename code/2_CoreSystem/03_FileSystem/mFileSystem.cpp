/**
 * @file mFileSystem.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-07
 * 
 * There are two types of filesystems:
 * 
 * 1) Internal flash filesystem
 * 2) SD Card filesystem
 * https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
 * @copyright Copyright (c) 2023
 * 
 * Step 1: build now and be able to view files on the SD card in the webui editor
 * 
 * Three types of files, stored as (.byte, .json, .txt)
 * 
 * sys_ = system files
 * drv_
 * sns_
 * lgt_anim_0# = When data is  more than signal file can hold, it is split into multiple files
 * egy_
 * dsp_
 * con_
 * cns_
 * 
 * 
 * When possible, all data from a module should be held within a "DATA"/"data" struct, and this will be called to save as ".byte" and when requested, "restore state at boot"
 * "If data cant fit in a struct because of its format, then save and load as .json". Anything in json will be passed through the command parser.
 * ".txt" files are for "logs" and "debugging". 
 */

#include "mFileSystem.h"

#ifdef USE_MODULE_CORE_FILESYSTEM


int8_t mFileSystem::Tasker(uint8_t function, JsonParserObject obj)
{
  /************
   * INIT SECTION
   *******************/
  switch(function)
  {
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function)
  {
    /************
     * PERIODIC SECTION
     *******************/
    case TASK_EVERY_FIVE_SECOND:
      #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_FIVE_SECONDS
      SystemTask__Execute_Module_Data_Save();
      #endif
      Handle_FileChanges_WebUIEdits();
    break;

    case TASK_EVERY_MINUTE:
      #ifdef ENABLE_DEVFEATURE_STORAGE__SAVE_TRIGGER_EVERY_MINUTE
      SystemTask__Execute_Module_Data_Save();
      #endif
    break;

    case TASK_EVERY_FIVE_MINUTE:
    break;

    /************
     * COMMANDS SECTION
     *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    /************
     * MQTT SECTION
     *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;

    case TASK_MQTT_STATUS_REFRESH_SEND_ALL:
      tkr_mqtt->MQTTHandler_RefreshAll(mqtthandler_list);
    break;

    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_mqtt->MQTTHandler_Rate(mqtthandler_list);
    break;

    case TASK_MQTT_SENDER:
      tkr_mqtt->MQTTHandler_Sender(mqtthandler_list, *this);
    break;
    #endif
  }

  return TASKER_RESULT__UNKNOWN_ID;
} // END Tasker


void mFileSystem::Pre_Init()
{
  
  static bool done = false;
  if(done) { return; }
  done = true;

  /************************************************************************************************
   * SECTION: JSON BUFFER / PSRAM INITIALISATION
   *
   * SD card mounting is intentionally NOT done here.
   ************************************************************************************************/

#if defined(ARDUINO_ARCH_ESP32)
  #if !defined(BOARD_HAS_PSRAM) && !(defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3))
  if (psramFound() && ESP.getChipRevision() < 3) psramSafe = false;
  if (!psramSafe) DEBUG_PRINTLN(F("Not using PSRAM."));
  #endif

  if(!pDoc)
  {
    pDoc = new PSRAMDynamicJsonDocument((psramSafe && psramFound() ? 2 : 1) * JSON_BUFFER_SIZE);

    DEBUG_PRINTF_P(
      PSTR("JSON buffer allocated: %u\n"),
      (psramSafe && psramFound() ? 2 : 1) * JSON_BUFFER_SIZE
    );

    if (psramFound()) {
      DEBUG_PRINTF_P(
        PSTR("PSRAM: %dkB/%dkB\n"),
        ESP.getFreePsram() / 1024,
        ESP.getPsramSize() / 1024
      );
    }
  }
#endif
}



// void mFileSystem::Pre_Init()
// {
//   /************************************************************************************************
//    * SECTION: JSON BUFFER / PSRAM INITIALISATION
//    *
//    * SD card mounting is intentionally NOT done here.
//    *
//    * Reason:
//    * - Pre_Init should only prepare filesystem module memory/state.
//    * - SD mount requires the pin manager to be ready.
//    * - SD is mounted from Init() after flash filesystem state is prepared.
//    ************************************************************************************************/

// #if defined(ARDUINO_ARCH_ESP32)
//   #if !defined(BOARD_HAS_PSRAM) && !(defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3))
//   if (psramFound() && ESP.getChipRevision() < 3) psramSafe = false;
//   if (!psramSafe) DEBUG_PRINTLN(F("Not using PSRAM."));
//   #endif

//   pDoc = new PSRAMDynamicJsonDocument((psramSafe && psramFound() ? 2 : 1) * JSON_BUFFER_SIZE);

//   DEBUG_PRINTF_P(
//     PSTR("JSON buffer allocated: %u\n"),
//     (psramSafe && psramFound() ? 2 : 1) * JSON_BUFFER_SIZE
//   );

//   if (psramFound()) {
//     DEBUG_PRINTF_P(
//       PSTR("PSRAM: %dkB/%dkB\n"),
//       ESP.getFreePsram() / 1024,
//       ESP.getPsramSize() / 1024
//     );
//   }
// #endif
// }

#if defined(ESP8266)
extern "C" {
  extern uint32_t _FS_start;
  extern uint32_t _FS_end;
  extern uint32_t _FS_page;
  extern uint32_t _FS_block;
}

#endif

void mFileSystem::Init(void)
{
  static bool done = false;
  if(done) { return; }
  done = true;

  ALOG_INF(PSTR(D_LOG_FILESYSTEM "Init__InternalStorage"));

  bool fsinit = false;

  module_state.mode = ModuleStatus::Initialising;

  UfsData.run_file_pos = -1;

  ufs_type = PFS_TNONE;
  ffs_type = PFS_TNONE;
  ufsp     = nullptr;
  ffsp     = nullptr;
  dfsp     = nullptr;
  ufs_dir  = 0;

#if defined(PULSAR_HAS_FILESYSTEM) && (PULSAR_HAS_FILESYSTEM == 0)

  ALOG_WRN(PSTR(D_LOG_FILESYSTEM "Internal filesystem disabled by build"));
  module_state.mode = ModuleStatus::Running;
  PFS_Init();
  return;

#endif

#ifdef ESP8266

  ffsp = &LittleFS;

  #if defined(ENABLE_DEBUGFEATURE_FILESYSTEM__SHOW_FS_SYMBOLS)
  ALOG_INF(PSTR(D_LOG_FILESYSTEM "FS symbols start=0x%08X end=0x%08X size=%u page=%u block=%u"),
           (uint32_t)&_FS_start,
           (uint32_t)&_FS_end,
           (uint32_t)((uint32_t)&_FS_end - (uint32_t)&_FS_start),
           (uint32_t)&_FS_page,
           (uint32_t)&_FS_block);
  #endif

  fsinit = LittleFS.begin();

  if (!fsinit)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "LittleFS mount failed"));

    #if defined(ENABLE_DEBUGFEATURE_FILESYSTEM__FORMAT_ON_MOUNT_FAIL)
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "Formatting LittleFS after mount failure"));
      LittleFS.format();
      fsinit = LittleFS.begin();
    #endif
  }

  if (!fsinit)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "Flash filesystem unavailable"));

    ffsp     = nullptr;
    ufsp     = nullptr;
    dfsp     = nullptr;
    ffs_type = PFS_TNONE;
    ufs_type = PFS_TNONE;
    ufs_dir  = 0;

    module_state.mode = ModuleStatus::Running;
    PFS_Init();
    return;
  }

  ALOG_INF(PSTR(D_LOG_FILESYSTEM "LittleFS mounted"));

#endif // ESP8266

#ifdef ESP32

  ffsp = &FILE_SYSTEM;

  fsinit =
    FILE_SYSTEM.begin(true, "") ||
    FILE_SYSTEM.begin(true, "", 5, "fs_1");

  if (!fsinit)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "Primary filesystem mount failed"));

    ffsp = &FFat;
    fsinit = FFat.begin(true, "");

    if (!fsinit)
    {
      ALOG_ERR(PSTR(D_LOG_FILESYSTEM "FFat fallback mount failed"));

      ffsp = nullptr;
      module_state.mode = ModuleStatus::Running;
      PFS_Init();
      return;
    }

    ffs_type = PFS_TFAT;
    ufs_type = ffs_type;
    ufsp     = ffsp;
    dfsp     = ffsp;

    module_state.mode = ModuleStatus::Running;

    ALOG_INF(
      PSTR(D_LOG_FILESYSTEM "FFat mounted with %d kB free"),
      GetFreeStorageSpace()
    );

    PFS_Init();
    return;
  }

  ALOG_INF(PSTR(D_LOG_FILESYSTEM "Primary filesystem mounted"));

#endif // ESP32

  ffs_type = PFS_TLFS;
  ufs_type = ffs_type;
  ufsp     = ffsp;
  dfsp     = ffsp;

  module_state.mode = ModuleStatus::Running;

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "FlashFS mounted with %d kB free"),
    GetFreeStorageSpace()
  );

  PFS_Init();
}

#ifndef ERR_FS_QUOTA
#define ERR_FS_QUOTA    11  // The FS is full or the maximum file size is reached
#endif

/*
 * Utility for SPIFFS filesystem
 */

#ifdef ARDUINO_ARCH_ESP32 //FS info bare IDF function until FS wrapper is available for ESP32
#if FILE_SYSTEM != LITTLEFS && ESP_IDF_VERSION_MAJOR < 4
  #include "esp_spiffs.h"
#endif
#endif

// #define WLED_DEBUG_FS2

bool mFileSystem::IsMounted(void) const
{
#if defined(PULSAR_HAS_FILESYSTEM) && (PULSAR_HAS_FILESYSTEM == 0)
  return false;
#else
  return (module_state.mode == ModuleStatus::Running) &&
         (ufsp != nullptr) &&
         (ufs_type != 0);
#endif
}

#define FS_BUFSIZE 256

/*
 * Structural requirements for files managed by writeObjectToFile() and readObjectFromFile() utilities:
 * 1. File must be a string representation of a valid JSON object
 * 2. File must have '{' as first character
 * 3. There must not be any additional characters between a root-level key and its value object (e.g. space, tab, newline)
 * 4. There must not be any characters between an root object-separating ',' and the next object key string
 * 5. There may be any number of spaces, tabs, and/or newlines before such object-separating ','
 * 6. There must not be more than 5 consecutive spaces at any point except for those permitted in condition 5
 * 7. If it is desired to delete the first usable object (e.g. preset file), a dummy object '"0":{}' is inserted at the beginning.
 *    It shall be disregarded by receiving software.
 *    The reason for it is that deleting the first preset would require special code to handle commas between it and the 2nd preset
 */

// There are no consecutive spaces longer than this in the file, so if more space is required, findSpace() can return false immediately
// Actual space may be lower
constexpr size_t MAX_SPACE = UINT16_MAX * 2U;           // smallest supported config has 128Kb flash size
volatile size_t knownLargestSpace = MAX_SPACE;

File f; // don't export to other cpp files

// wrapper to find out how long closing takes
void mFileSystem::closeFile() {
  #ifdef WLED_DEBUG_FS2
    Serial.println("Close -> ");
    uint32_t s = millis();
  #endif
  f.close();
  #ifdef WLED_DEBUG_FS2
  Serial.printf("took %d ms\n", millis() - s);
  #endif
  doCloseFile = false;
}


// find() that reads and buffers data from file stream in 256-byte blocks.
// Significantly faster, f.find(key) can take SECONDS for multi-kB files
bool mFileSystem::bufferedFind(const char *target, bool fromStart)
{
  #ifdef WLED_DEBUG_FS2
    Serial.printf("Find ");
    Serial.printf(target);
    uint32_t s = millis();
  #endif

  if (!f || !f.size()) return false;
  size_t targetLen = strlen(target);

  size_t index = 0;
  byte buf[FS_BUFSIZE];
  if (fromStart) f.seek(0);

  while (f.position() < f.size() -1) {
    size_t bufsize = f.read(buf, FS_BUFSIZE); // better to use size_t instead if uint16_t
    size_t count = 0;
    while (count < bufsize) {
      if(buf[count] != target[index])
      index = 0; // reset index if any char does not match

      if(buf[count] == target[index]) {
        if(++index >= targetLen) { // return true if all chars in the target match
          f.seek((f.position() - bufsize) + count +1);
  #ifdef WLED_DEBUG_FS2
          Serial.printf("Found at pos %d, took %d ms", f.position(), millis() - s);
  #endif
          return true;
        }
      }
      count++;
    }
  }
  #ifdef WLED_DEBUG_FS2
  Serial.printf("No match, took %d ms\n", millis() - s);
  #endif
  return false;
}


// find empty spots in file stream in 256-byte blocks.
bool mFileSystem::bufferedFindSpace(size_t targetLen, bool fromStart) 
{

  #ifdef WLED_DEBUG_FS2
    Serial.printf("Find %d spaces\n", targetLen);
    uint32_t s = millis();
  #endif

  if (knownLargestSpace < targetLen) {
    Serial.printf("No match, KLS ");
    Serial.println(knownLargestSpace);
    return false;
  }

  if (!f || !f.size()) return false;

  size_t index = 0; // better to use size_t instead if uint16_t
  byte buf[FS_BUFSIZE];
  if (fromStart) f.seek(0);

  while (f.position() < f.size() -1) {
    size_t bufsize = f.read(buf, FS_BUFSIZE);
    size_t count = 0;

    while (count < bufsize) {
      if(buf[count] == ' ') {
        if(++index >= targetLen) { // return true if space long enough
          if (fromStart) {
            f.seek((f.position() - bufsize) + count +1 - targetLen);
            knownLargestSpace = MAX_SPACE; //there may be larger spaces after, so we don't know
          }
  #ifdef WLED_DEBUG_FS2
          Serial.printf("Found at pos %d, took %d ms", f.position(), millis() - s);
  #endif  
          return true;
        }
      } else {
        if (!fromStart) return false;
        if (index) {
          if (knownLargestSpace < index || (knownLargestSpace == MAX_SPACE)) knownLargestSpace = index;
          index = 0; // reset index if not space
        }
      }

      count++;
    }
  }
  #ifdef WLED_DEBUG_FS2
  Serial.printf("No match, took %d ms\n", millis() - s);
  #endif
  return false;
}


// find the closing bracket corresponding to the opening bracket at the file pos when calling this function
bool mFileSystem::bufferedFindObjectEnd() 
{
  #ifdef WLED_DEBUG_FS2
    Serial.println("Find obj end");
    uint32_t s = millis();
  #endif

  if (!f || !f.size()) return false;

  uint16_t objDepth = 0; //num of '{' minus num of '}'. return once 0
  //size_t start = f.position();
  byte buf[FS_BUFSIZE];

  while (f.position() < f.size() -1) {
    size_t bufsize = f.read(buf, FS_BUFSIZE); // better to use size_t instead of uint16_t
    size_t count = 0;

    while (count < bufsize) {
      if (buf[count] == '{') objDepth++;
      if (buf[count] == '}') objDepth--;
      if (objDepth == 0) {
        f.seek((f.position() - bufsize) + count +1);
  #ifdef WLED_DEBUG_FS2
        Serial.printf("} at pos %d, took %d ms", f.position(), millis() - s);
  #endif  
        return true;
      }
      count++;
    }
  }
  #ifdef WLED_DEBUG_FS2
  Serial.printf("No match, took %d ms\n", millis() - s);
  #endif
  return false;
}


// fills n bytes from current file pos with ' ' characters
void mFileSystem::writeSpace(size_t l)
{
  byte buf[FS_BUFSIZE];
  memset(buf, ' ', FS_BUFSIZE);

  while (l > 0) {
    size_t block = (l>FS_BUFSIZE) ? FS_BUFSIZE : l;
    f.write(buf, block);
    l -= block;
  }

  if (knownLargestSpace < l) knownLargestSpace = l;
}


bool mFileSystem::appendObjectToFile(const char* key, JsonDocument* content, uint32_t s, uint32_t contentLen)
{
  #ifdef WLED_DEBUG_FS2
    Serial.println("Append");
    uint32_t s1 = millis();
  #endif
  uint32_t pos = 0;
  if (!f) return false;

  if (f.size() < 3) {
    char init[10];
    strcpy_P(init, PSTR("{\"0\":{}}"));
    f.print(init);
  }

  if (content->isNull()) {
    doCloseFile = true;
    return true; //nothing  to append
  }

  //if there is enough empty space in file, insert there instead of appending
  if (!contentLen) contentLen = measureJson(*content);
  Serial.printf("CLen %d\n", contentLen);
  if (bufferedFindSpace(contentLen + strlen(key) + 1)) {
    if (f.position() > 2) f.write(','); //add comma if not first object
    f.print(key);
    serializeJson(*content, f);
  #ifdef WLED_DEBUG_FS2
    Serial.printf("Inserted, took %d ms (total %d)", millis() - s1, millis() - s);
  #endif  
    doCloseFile = true;
    return true;
  }

  //not enough space, append at end

  //permitted space for presets exceeded
  updateFSInfo();

  if (f.size() + 9000 > (fsBytesTotal - fsBytesUsed)) { //make sure there is enough space to at least copy the file once
    errorFlag = ERR_FS_QUOTA;
    doCloseFile = true;
    return false;
  }

  //check if last character in file is '}' (typical)
  uint32_t eof = f.size() -1;
  f.seek(eof, SeekSet);
  if (f.read() == '}') pos = eof;

  if (pos == 0) //not found
  {
    Serial.printf("not }");
    f.seek(0);
    while (bufferedFind("}",false)) //find last closing bracket in JSON if not last char
    {
      pos = f.position();
    }
    if (pos > 0) pos--;
  }
  Serial.printf("pos "); Serial.println(pos);
  if (pos > 2)
  {
    f.seek(pos, SeekSet);
    f.write(',');
  } else { //file content is not valid JSON object
    f.seek(0, SeekSet);
    f.print('{'); //start JSON
  }

  f.print(key);

  //Append object
  serializeJson(*content, f);
  f.write('}');

  doCloseFile = true;
  #ifdef WLED_DEBUG_FS2
  Serial.printf("Appended, took %d ms (total %d)", millis() - s1, millis() - s);
  #endif  
  return true;
}


bool mFileSystem::writeObjectToFileUsingId(const char* file, uint16_t id, JsonDocument* content)
{
  char objKey[10];
  sprintf(objKey, "\"%d\":", id);
  DEBUG_LINE_HERE;
  return writeObjectToFile(file, objKey, content);
}


bool mFileSystem::writeObjectToFile(const char* file, const char* key, JsonDocument* content)
{
  uint32_t s = 0; // timing

  #ifdef WLED_DEBUG_FS2
    Serial.printf("Write to %s with key %s >>>\n\r", file, (key==nullptr)?"nullptr":key);
    serializeJson(*content, Serial); 
    Serial.println();
    s = millis();
  #endif

  size_t pos = 0;
  f = FILE_SYSTEM.open(file, "r+");
  if (!f && !FILE_SYSTEM.exists(file)) f = FILE_SYSTEM.open(file, "w+");
  if (!f) {
    Serial.println("Failed to open!");
    return false;
  }

  DEBUG_LINE_HERE;
  if (!bufferedFind(key)) //key does not exist in file
  {
  DEBUG_LINE_HERE;
    return appendObjectToFile(key, content, s);
  }

  DEBUG_LINE_HERE;
  //an object with this key already exists, replace or delete it
  pos = f.position();
  //measure out end of old object
  bufferedFindObjectEnd();
  size_t pos2 = f.position();

  uint32_t oldLen = pos2 - pos;
  Serial.printf("Old obj len %d\n", oldLen);

  //Three cases:
  //1. The new content is null, overwrite old obj with spaces
  //2. The new content is smaller than the old, overwrite and fill diff with spaces
  //3. The new content is larger than the old, but smaller than old + trailing spaces, overwrite with new
  //4. The new content is larger than old + trailing spaces, delete old and append

  size_t contentLen = 0;
  if (!content->isNull()) contentLen = measureJson(*content);

  if (contentLen && contentLen <= oldLen) { //replace and fill diff with spaces
    Serial.println("replace");
    f.seek(pos);
    serializeJson(*content, f);
    writeSpace(pos2 - f.position());
  } else if (contentLen && bufferedFindSpace(contentLen - oldLen, false)) { //enough leading spaces to replace
    Serial.println("replace (trailing)");
    f.seek(pos);
    serializeJson(*content, f);
  } else {
    Serial.println("delete");
    pos -= strlen(key);
    if (pos > 3) pos--; //also delete leading comma if not first object
    f.seek(pos);
    writeSpace(pos2 - pos);
    if (contentLen) return appendObjectToFile(key, content, s, contentLen);
  }

  doCloseFile = true;
  Serial.printf("Replaced/deleted, took %d ms\n", millis() - s);
  return true;
}


bool mFileSystem::readObjectFromFileUsingId(const char* file, uint16_t id, JsonDocument* dest)
{
  char objKey[10];
  sprintf(objKey, "\"%d\":", id);
  return readObjectFromFile(file, objKey, dest);
}


//if the key is a nullptr, deserialize entire object
bool mFileSystem::readObjectFromFile(const char* file, const char* key, JsonDocument* dest)
{
  if (doCloseFile) closeFile();
  #ifdef WLED_DEBUG_FS2
    // Serial.printf("Read from %s with key %s >>>\n", file, (key==nullptr)?"nullptr":key);
    uint32_t s = millis();
  #endif
  f = FILE_SYSTEM.open(file, "r");
  if (!f) return false;

  if (key != nullptr && !bufferedFind(key)) //key does not exist in file
  {
    f.close();
    dest->clear();
    #ifdef WLED_DEBUG_FS2
    Serial.println("Obj not found.");
    #endif
    return false;
  }

  deserializeJson(*dest, f);

  f.close();
  // Serial.printf("Read, took %d ms\n", millis() - s);
  return true;
}


void mFileSystem::updateFSInfo() 
{
  #ifdef ARDUINO_ARCH_ESP32
    #if FILE_SYSTEM == LITTLEFS || ESP_IDF_VERSION_MAJOR >= 4
    fsBytesTotal = FILE_SYSTEM.totalBytes();
    fsBytesUsed = FILE_SYSTEM.usedBytes();
    #else
    esp_spiffs_info(nullptr, &fsBytesTotal, &fsBytesUsed);
    #endif
  #else
    FSInfo fsi;
    FILE_SYSTEM.info(fsi);
    fsBytesUsed  = fsi.usedBytes;
    fsBytesTotal = fsi.totalBytes;
  #endif
}


#ifdef USE_MODULE_NETWORK_WEBSERVER


//Un-comment any file types you need
String mFileSystem::getContentType(AsyncWebServerRequest* request, String filename)
{

  if(request->hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".json")) return "application/json";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
//  else if(filename.endsWith(".xml")) return "text/xml";
//  else if(filename.endsWith(".pdf")) return "application/x-pdf";
//  else if(filename.endsWith(".zip")) return "application/x-zip";
//  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


#ifdef ARDUINO_ARCH_ESP32
// caching presets in PSRAM may prevent occasional flashes seen when HomeAssitant polls WLED
// original idea by @akaricchi (https://github.com/Akaricchi)
// returns a pointer to the PSRAM buffer, updates size parameter
static const uint8_t *getPresetCache(size_t &size) {
  static unsigned long presetsCachedTime = 0;
  static uint8_t *presetsCached = nullptr;
  static size_t presetsCachedSize = 0;
  static byte presetsCachedValidate = 0;

  //if (presetsModifiedTime != presetsCachedTime) DEBUG_PRINTLN(F("getPresetCache(): presetsModifiedTime changed."));
  //if (presetsCachedValidate != cacheInvalidate) DEBUG_PRINTLN(F("getPresetCache(): cacheInvalidate changed."));

  #ifdef USE_MODULE_LIGHTS_INTERFACE

  if ((tkr_anim->presetsModifiedTime != presetsCachedTime) || (presetsCachedValidate != tkr_web->cacheInvalidate)) {
    if (presetsCached) {
      free(presetsCached);
      presetsCached = nullptr;
    }
  }

  if (!presetsCached) {
    File file = FILE_SYSTEM.open(FPSTR( tkr_anim->getPresetsFileName() ), "r");
    if (file) {
      presetsCachedTime = tkr_anim->presetsModifiedTime;
      presetsCachedValidate = tkr_web->cacheInvalidate;
      presetsCachedSize = 0;
      presetsCached = (uint8_t*)ps_malloc(file.size() + 1);
      if (presetsCached) {
        presetsCachedSize = file.size();
        file.read(presetsCached, presetsCachedSize);
        presetsCached[presetsCachedSize] = 0;
        file.close();
      }
    }
  }

  #endif // 

  size = presetsCachedSize;
  return presetsCached;
}
#endif 

bool mFileSystem::handleFileRead(AsyncWebServerRequest* request, String path){
  
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  ALOG_DBG(PSTR("WS FileRead: %s"), path);
  if(path.endsWith("/")) path += "index.htm";
  if(path.indexOf(F("sec")) > -1) return false;
  #ifdef ARDUINO_ARCH_ESP32
  if (psramSafe && psramFound() && path.endsWith(FPSTR(  tkr_anim->getPresetsFileName() ))) {
    size_t psize;
    const uint8_t *presets = getPresetCache(psize);
    if (presets) {
      AsyncWebServerResponse *response = request->beginResponse_P(200, FPSTR(CONTENT_TYPE_JSON), presets, psize);
      request->send(response);
      return true;
    }
  }
  #endif
  if(FILE_SYSTEM.exists(path) || FILE_SYSTEM.exists(path + ".gz")) {
    request->send(request->beginResponse(FILE_SYSTEM, path, {}, request->hasArg(F("download")), {}));
    return true;
  }
  #endif // USE_MODULE_LIGHTS_INTERFACE
  return false;



  // ALOG_DBG(PSTR("WS FileRead: %s"), path);
  
  // if(path.endsWith("/")) path += "index.htm";
  // if(path.indexOf("sec") > -1) return false;
  // String contentType = getContentType(request, path);
  // if(FILE_SYSTEM.exists(path)) {
  //   ALOG_INF(PSTR("Sending file %s from FILE_SYSTEM"), path.c_str());
  //   request->send(FILE_SYSTEM, path, contentType);
  //   return true;
  // }
  // return false;
  
}


#endif // USE_MODULE_NETWORK_WEBSERVER





// Date Modified: 12Dec25
void mFileSystem::Handle_FileChanges_WebUIEdits()
{
  String changedFile;

  // Check if any file changes have occurred (clears the flag if true)
  if (!SPIFFSEditor::Check_AnyFilesEdited()) {
    ALOG_DBG(PSTR("No file change detected.")); //debug for now
    return;
  }

  ALOG_DBG(PSTR("File change detected: %s"), SPIFFSEditor::Get_LastEditedFileName().c_str());

  tkr->Tasker_Interface(TASK_FILESYSTEM__HANDLE_FILE_CHANGES_FROM_EDIT_URL__ID);

  SPIFFSEditor::Check_ClearFilesEditFlag();
  
  return;
}





void mFileSystem::SystemTask__Execute_Module_Data_Save()
{

  ALOG_INF(PSTR("SystemTask__Execute_Module_Data_Save"));

  tkr->Tasker_Interface(TASK_FILESYSTEM__SAVE__MODULE_DATA__ID);

}



/*********************************************************************************************\
 * GetFreeStorageSpace
 *
 * Public wrapper for querying free storage space on the active filesystem.
 *
 * Returns:
 *   Free storage space in kB.
 *
 * Notes:
 *   - The active storage backend is selected during Init().
 *   - Current normal PulSar backend is LittleFS.
 *   - Future backends, such as SD card, should be handled by backend-specific
 *     SubCall__GetFreeStorageSpace__TYPE() functions.
\*********************************************************************************************/
uint32_t mFileSystem::GetFreeStorageSpace(PFSStorageTarget target)
{
  switch (target)
  {
    case PFS_STORAGE_INTERNAL:
      return SubCall__GetFreeStorageSpace__LittleFS();

    case PFS_STORAGE_ACTIVE:
      return SubCall__GetFreeStorageSpace__Active();

    default:
      return 0;
  }
}

uint32_t mFileSystem::SubCall__GetFreeStorageSpace__Active(void)
{
  switch (ufs_type)
  {
    case PFS_TLFS:
      return SubCall__GetFreeStorageSpace__LittleFS();

    case PFS_TFAT:
      return SubCall__GetFreeStorageSpace__FFat();

    default:
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "No active internal filesystem"));
      return 0;
  }
}


/*********************************************************************************************\
 * SubCall__GetFreeStorageSpace__LittleFS
 *
 * Returns free space on the internal LittleFS filesystem in kB.
\*********************************************************************************************/
uint32_t mFileSystem::SubCall__GetFreeStorageSpace__LittleFS(void)
{
  uint64_t free_bytes = 0;

#ifdef ESP8266
  if (!ffsp || !ffs_type) {
    return 0;
  }

  FSInfo64 fsinfo;

  if (!ffsp->info64(fsinfo)) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "LittleFS info64 failed"));
    return 0;
  }

  free_bytes = fsinfo.totalBytes - fsinfo.usedBytes;
#endif

#ifdef ESP32
  if(!ffsp || !ffs_type) {
    return 0;
  }

  free_bytes = LITTLEFS.totalBytes() - LITTLEFS.usedBytes();
#endif

  return free_bytes / 1024;
}


/*********************************************************************************************\
 * SubCall__GetFreeStorageSpace__FFat
 *
 * Returns free space on the FFat filesystem in kB.
\*********************************************************************************************/
uint32_t mFileSystem::SubCall__GetFreeStorageSpace__FFat(void)
{
#ifdef ESP32
  return FFat.freeBytes() / 1024;
#else
  return 0;
#endif
}

/*********************************************************************************************\
 * low level functions
\*********************************************************************************************/

bool mFileSystem::FileExists(const char *fname)
{
  if (!ffsp || !ffs_type || !fname) { return false; }

  bool yes = ffsp->exists(fname);
  if (!yes) {
    ALOG_DBM(PSTR("TFS: File '%s' not found"), fname + 1);
  }
  return yes;
}

size_t mFileSystem::FileSize(const char *fname)
{
  if (!ffsp || !ffs_type || !fname) { return 0; }

  File file = ffsp->open(fname, "r");
  if (!file) { return 0; }

  size_t flen = file.size();
  file.close();
  return flen;
}


bool mFileSystem::SaveFile(const char *fname, const uint8_t *buf, uint32_t len)
{
  if (!ffsp || !ffs_type || !fname || !buf) { return false; }

#ifdef USE_WEBCAM
  WcInterrupt(0);
#endif

  bool result = false;
  File file = ffsp->open(fname, "w");

  if (!file) {
    ALOG_INF(PSTR("TFS: Save failed"));
  } else {
    file.write(buf, len);
    file.close();
    result = true;
  }

#ifdef USE_WEBCAM
  WcInterrupt(1);
#endif

  return result;
}

bool mFileSystem::InitFile(const char *fname, uint32_t len, uint8_t init_value)
{
  if (!ffsp || !ffs_type || !fname) { return false; }

  File file = ffsp->open(fname, "w");
  if (!file) {
    ALOG_INF(PSTR("TFS: Erase failed"));
    return false;
  }

  for (uint32_t i = 0; i < len; i++) {
    file.write(&init_value, 1);
  }

  file.close();
  return true;
}

bool mFileSystem::LoadFile(const char *fname, uint8_t *buf, uint32_t len)
{
  if (!ffsp || !ffs_type || !fname || !buf) { return false; }

  File file = ffsp->open(fname, "r");
  if (!file) {
    ALOG_DBM(PSTR("TFS: File '%s' not found"), fname + 1);
    return false;
  }

  size_t flen = file.size();
  if (len > flen) { len = flen; }

  file.read(buf, len);
  file.close();
  return true;
}

// String mFileSystem::LoadString(const char *fname) {
//   // Use a reasonable amount of stack space considering 4k/8k available on ESP8266/ESP32 and manageable string length
//   char buf[2048] = { 0 };                   // Prepare empty string of max 2047 characters on stack
//   LoadFile(fname, (uint8_t*)buf, 2047);  // Leave last position as end of string ('\0')
//   return String(buf);                       // Received string or empty on error
// }
String mFileSystem::LoadString(const char *fname)
{
  if (!ffsp || !ffs_type || !fname) { return String(); }

  char buf[2048] = { 0 };
  LoadFile(fname, (uint8_t*)buf, sizeof(buf) - 1);
  return String(buf);
}



bool mFileSystem::DeleteFile(const char *fname)
{
  if (!ffsp || !ffs_type || !fname) { return false; }

  if (!ffsp->remove(fname)) {
    ALOG_INF(PSTR("TFS: Delete failed"));
    return false;
  }

  return true;
}

bool mFileSystem::RenameFile(const char *fname1, const char *fname2)
{
  if (!ffsp || !ffs_type || !fname1 || !fname2) { return false; }

  if (!ffsp->rename(fname1, fname2)) {
    ALOG_INF(PSTR("TFS: Rename failed"));
    return false;
  }

  return true;
}



uint8_t mFileSystem::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  JBI->Add_P("InternalFreeKB", GetFreeStorageSpace(PFS_STORAGE_INTERNAL));
  JBI->Add_P("ActiveFreeKB",   GetFreeStorageSpace(PFS_STORAGE_ACTIVE));
  JBI->Add_P("PFSBackend",     PFS_GetBackendName());

  return JBI->End();
}


#ifdef USE_MODULE_NETWORK_MQTT

void mFileSystem::MQTTHandler_Init(){

  struct handler<mFileSystem>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//tkr_mqtt->dt.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);
} 

#endif // USE_MODULE_NETWORK_MQTT


/************************************************************************************************
 * FUNCTION: parse_JSONCommand
 *
 * SUMMARY:
 * - Parses filesystem-level JSON commands.
 * - Delegates SD-card-specific commands to subparse_JSONCommand__SDCards().
 *
 * CHANGED:
 * - 17May26: Cleaned command flow and added early returns.
 ************************************************************************************************/
void mFileSystem::parse_JSONCommand(JsonParserObject obj)
{
  JsonParserToken jtok = 0;
  /************************************************************************************************
   * SECTION: DEBUG COMMANDS
   ************************************************************************************************/

  if (jtok = obj["Debug"])
  {
    JsonParserObject debug = jtok.getObject();

    // if (debug["TriggerJSONSave"])
    // {
    //   JsonFile_Save__Stored_Module();
    //   return;
    // }

    // if (debug["TriggerJSONLoad"])
    // {
    //   JsonFile_Load__Stored_Module();
    //   return;
    // }
  }


  /************************************************************************************************
   * SECTION: INTERNAL FILESYSTEM COMMANDS
   ************************************************************************************************/

  if (jtok = obj["ListDir"])
  {
    listDir(FILE_SYSTEM, "/", 0);
    return;
  }

  if (jtok = obj["ReadFile"])
  {
    CommandSet_ReadFile(jtok.getStr());
    return;
  }

}

void mFileSystem::listDir(fs::FS &fs, const char * dirname, uint8_t levels){
  
  Serial.printf("Listing directory: %s\n", dirname);

  #ifdef ESP32
  File root = fs.open(dirname);
  if(!root){
      Serial.println("Failed to open directory");
      return;
  }
  if(!root.isDirectory()){
      Serial.println("Not a directory");
      return;
  }

  File file = root.openNextFile();
  while(file){
      if(file.isDirectory()){
          Serial.print("  DIR : ");
          Serial.println(file.name());
          if(levels){
              listDir(fs, file.name(), levels -1);
          }
      } else {
          Serial.print("  FILE: ");
          Serial.print(file.name());
          Serial.print("  SIZE: ");
          Serial.println(file.size());
      }
      file = root.openNextFile();
  }
  #endif 

}


void mFileSystem::readFile(fs::FS &fs, const char * path)
{
    
  Serial.printf("Reading file: %s\n\r", path);

  #ifdef ESP32
  File file = fs.open(path);
  if(!file){
      Serial.println("Failed to open file for reading");
      return;
  }

  Serial.print("Read from file: \n\r");
  while(file.available()){
      Serial.write(file.read());
  }
  Serial.println();
  #endif

}


void mFileSystem::CommandSet_ReadFile(const char* filename){

  readFile(FILE_SYSTEM, filename);

  ALOG_COM(PSTR(D_LOG_FILESYSTEM D_COMMAND_SVALUE_K("ReadFile")), filename);

} 


/**
 * @brief 
 * 
 * Three files are needed:
 * "/config_system.json" -- to be moved to flash later
 * "/config_modules.json"
 * "/config_secure.json"
 * 
 * 
 */


//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

// void mFileSystem::JsonFile_Save__Stored_Module()
// {
  
//   ALOG_INF( PSTR("JsonFile_Save__Stored_Module") );

//   const char* file_path = "/config_module.json";
//   char buffer[100] = {0};

//   File file;  
//   // Open file for writing, if it does not exist, create it
//   // Seek is placed at the start of the file, contents will be overwriten
//   file = FILE_SYSTEM.open(file_path, "w+");
  
//   if(!file) 
//   {
//     ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
//     return;
//   }

//   if(!JBI->RequestLock(GetModuleUniqueID())){
//     return;
//   }
 
//   JBI->Start();
//     JBI->Add(PM_UTC_TIME, tkr_time->GetDateAndTime(DT_UTC).c_str());
//     JBI->Add(PSTR("millis"), millis());
//     // tkr->Tasker_Interface(TASK_FILESYSTEM_APPEND__CONFIG_MODULES__ID);
//   JBI->End();

//   file.print(JBI->GetBufferPtr());
//   file.close();
    
//   ALOG_INF(PSTR("Writing file \"%s\""), JBI->GetBufferPtr());

//   JBI->ReleaseLock();

// }

// /**
//  * @brief Test if file exists, if not, load default template from progmem if it exists
//  * 
//  */
// void mFileSystem::JsonFile_Load__Stored_Module_Or_Default_Template()
// {

//   DEBUG_LINE_HERE;

//   bool force_default_template = false; // ie on reset
 
//   if(!JsonFile_Load__Stored_Module() || force_default_template){
//     ALOG_INF(PSTR("No config_module.json file found, loading default template from progmem"));
//     tkr->Tasker_Interface(TASK_CONFIG_LOAD_POST_INIT_DEFAULTS_FROM_PROGMEM);
//   }

//   DEBUG_LINE_HERE;

// }


// bool mFileSystem::JsonFile_Load__Stored_Module()
// {
  
//   ALOG_INF( PSTR("JsonFile_Load__Stored_Module") );

//   File file;  
//   const char* file_path = "/config_module.json";
  
//   // Open file for read only
//   file = FILE_SYSTEM.open(file_path, "r");
  
//   if (!file) {
//     ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
//     return false;
//   }

//   Serial.printf("Read from file [%d] \n\r", file.available());

//   if(!JBI->RequestLock(GetModuleUniqueID())){
//     return false;
//   }
 
//   // Read into local buffer, this should be locked from async access using buffer class method
//   data_buffer.ClearDeep();
//   uint8_t* buffer_p = (uint8_t*)data_buffer.payload.ctr;
//   file.read(buffer_p, file.available());
//   file.close();
  
//   data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

//   ALOG_INF( PSTR(DEBUG_INSERT_PAGE_BREAK "Loaded file = \"%d|%s\""),data_buffer.payload.length_used, data_buffer.payload.ctr);

//   tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);

//   JBI->ReleaseLock();

//   return true;
  
// }



//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

// void mFileSystem::JsonFile_Save__Stored_Secure()
// {
  
//   ALOG_INF( PSTR("JsonFile_Save__Stored_Secure") );

//   const char* file_path = "/config_secure.json";
//   char buffer[100] = {0};

//   File file;  
//   // Open file for writing, if it does not exist, create it
//   // Seek is placed at the start of the file, contents will be overwriten
//   file = FILE_SYSTEM.open(file_path, "w+");
  
//   if(!file) 
//   {
//     ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
//     return;
//   }

//   if(!JBI->RequestLock(GetModuleUniqueID())){
//     return;
//   }
 
//   JBI->Start();
//     JBI->Add(PM_UTC_TIME, tkr_time->GetDateAndTime(DT_UTC).c_str() );
//     JBI->Add(PSTR("millis"), millis());
//     tkr->Tasker_Interface(TASK_FILESYSTEM_APPEND__Stored_Secure__ID);
//   JBI->End();

//   file.print(JBI->GetBufferPtr());
//   file.close();
    
//   ALOG_INF(PSTR("Writing file \"%s\""), JBI->GetBufferPtr());

//   JBI->ReleaseLock();

// }

// void mFileSystem::JsonFile_Load__Stored_Secure()
// {
  
//   ALOG_INF( PSTR("JsonFile_Load__Stored_Secure") );

//   File file;  
//   const char* file_path = "/config_secure.json";
  
//   // Open file for read only
//   file = FILE_SYSTEM.open(file_path, "r");
  
//   if (!file) {
//     ALOG_ERR(PSTR("Failed to open \"%s\""), file_path);
//     return;
//   }

//   Serial.printf("Read from file [%d]: \n\r", file.available());

//   if(!JBI->RequestLock(GetModuleUniqueID())){
//     return;
//   }
 
//   // Read into local buffer, this should be locked from async access using buffer class method
//   data_buffer.ClearDeep();
//   uint8_t* buffer_p = (uint8_t*)data_buffer.payload.ctr;
//   file.read(buffer_p, file.available());
//   file.close();
  
//   data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

//   ALOG_INF( PSTR("Loaded file = \"%d|%s\""),data_buffer.payload.length_used, data_buffer.payload.ctr);

//   tkr->Tasker_Interface(TASK_JSON_COMMAND_ID);

//   JBI->ReleaseLock();
  
// }




//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//// Shared functions: For all modules to pass its file name and bytes to save, or byte to load and the position to write into
//////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////

void mFileSystem::ByteFile_Save(char* filename_With_extension, uint8_t* buffer, uint16_t buflen) // where to write the data from
{
  
  ALOG_INF( PSTR("ByteFile_Save") );

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(filename_With_extension, "w+");
  
  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), filename_With_extension);
    return;
  }

  file.write((const uint8_t*)buffer, buflen);
  file.close();
    
  ALOG_INF(PSTR("Writing file (%s) \"%s\""), filename_With_extension, buffer);

}

uint32_t mFileSystem::ByteFile_Load(char* filename_With_extension, uint8_t* buffer, uint16_t buflen) // where to write the data into
{
  
  ALOG_INF( PSTR("ByteFile_Load") );

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(filename_With_extension, "r");

  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), filename_With_extension);
    return 0;
  }


  uint32_t filesize = file.size();

  ALOG_INF(PSTR("Reading file \"%s\" %d bytes (expected %d bytes)"), filename_With_extension, filesize, buflen);

  if(filesize != buflen){
    ALOG_ERR(PSTR("File size mismatch, expected %d bytes, got %d bytes"), buflen, filesize);
    file.close();
    return 0;
  }else{
    ALOG_ERR(PSTR("File file.read(buffer, filesize)"));
    file.read(buffer, filesize);
  }

  file.close();
//must return the file size, note that if the read does not match the expected (ie file mismatch in internal memory), then the file is corrupted and must be reset to default
  return 0;
  
}



void mFileSystem::JSONFile_Save(char* filename_With_extension, char* buffer, uint16_t buflen) // where to write the data from
{
  
  ALOG_INF( PSTR("JSONFile_Save") );

  File file;  
  // Open file for writing, if it does not exist, create it
  // Seek is placed at the start of the file, contents will be overwriten
  file = FILE_SYSTEM.open(filename_With_extension, "w+");
  
  if(!file) 
  {
    ALOG_ERR(PSTR("Failed to open \"%s\""), filename_With_extension);
    return;
  }

  file.write((const uint8_t*)buffer, buflen);
  file.close();
    
  ALOG_INF(PSTR("Writing file (%s) \"%s\""), filename_With_extension, buffer);

}


void mFileSystem::JSONFile_Load(char* filename_With_extension, char* buffer, uint16_t buflen) // where to write the data into
{

  ALOG_INF( PSTR("JSONFile_Load") );

}




















#endif // USE_MODULE_CORE_FILESYSTEM
