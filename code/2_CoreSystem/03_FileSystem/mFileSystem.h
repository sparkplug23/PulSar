#ifndef _MODULE_CORE_FILESYSTEM_H
#define _MODULE_CORE_FILESYSTEM_H

#define D_UNIQUE_MODULE_CORE_FILESYSTEM_ID 2003 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_FILESYSTEM

/************************************************************************************************
 * MODULE: mFileSystem
 *
 * SUMMARY:
 * - Internal PulSar file storage only.
 * - Owns LittleFS / FFat mounted flash filesystem.
 * - Does not own SD card.
 * - SD card is owned by mSDCard.
 *
 * CHANGED:
 * - 31May26: Removed SD ownership and added PFS minimal sector fallback declarations.
 ************************************************************************************************/

#include <string.h>
#include <strings.h>

#ifdef ESP8266
  #include <LittleFS.h>
#endif

#ifdef ESP32
  #include <LittleFS.h>
  #include "FFat.h"
  #include "FS.h"
#endif

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
    #ifdef USE_MODULE_NETWORK_WEBSERVER
      #include <AsyncTCP.h>
      #include <ESPAsyncWebServer.h>
      #include "3_Network/21_WebServer/FileEditor.h"
    #endif
  #endif
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <ESP8266WiFi.h>
    #include <ESPAsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  #endif
#endif




#define ARDUINOJSON_DECODE_UNICODE 0
#include "3_Network/21_WebServer/AsyncJson-v6.h"
#include "3_Network/21_WebServer/ArduinoJson-v6.h"

#if defined(ARDUINO_ARCH_ESP32)
struct PSRAM_Allocator {
  void* allocate(size_t size) {
    if (1 && psramFound()) return ps_malloc(size);
    else                   return malloc(size);
  }
  void* reallocate(void* ptr, size_t new_size) {
    if (1 && psramFound()) return ps_realloc(ptr, new_size);
    else                   return realloc(ptr, new_size);
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};
using PSRAMDynamicJsonDocument = BasicJsonDocument<PSRAM_Allocator>;
#else
#define PSRAMDynamicJsonDocument DynamicJsonDocument
#endif

#define FILE_EXTENSION_JSON ".json"
#define FILE_EXTENSION_BIN ".txt"

#include "1_TaskerManager/mTaskerInterface.h"

class mFileSystem :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mFileSystem(){};
    void Init(void);
    void Pre_Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_CORE_FILESYSTEM_CTR = D_MODULE_CORE_FILESYSTEM_CTR;
    PGM_P GetModuleName(){ return PM_MODULE_CORE_FILESYSTEM_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_FILESYSTEM_ID; }

    struct ClassState
    {
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    } module_state;

    /************************************************************************************************
     * SECTION: Internal filesystem state
     ************************************************************************************************/

    #define PFS_TNONE         0
    #define PFS_TFAT          1
    #define PFS_TLFS          2

    // Legacy aliases retained while older cpp/helpers are being cleaned.
    #define UFS_TNONE         PFS_TNONE
    #define UFS_TFAT          PFS_TFAT
    #define UFS_TLFS          PFS_TLFS

    struct UfsData_t
    {
      int32_t run_file_pos = -1;
    } UfsData;

    FS* ufsp = nullptr;   // active internal PulSar filesystem
    FS* ffsp = nullptr;   // mounted flash filesystem
    FS* dfsp = nullptr;   // directory/listing filesystem

    uint8_t ufs_type = PFS_TNONE;
    uint8_t ffs_type = PFS_TNONE;
    uint8_t ufs_dir  = 0;

    /************************************************************************************************
     * SECTION: PFS - PulSar File Storage
     *
     * SUMMARY:
     * - Internal persistent storage only.
     * - Normal backend: mounted internal filesystem.
     * - ESP8266 no-FS fallback: minimal fixed flash sector storage for main settings.
     ************************************************************************************************/

    #ifndef PFS_SETTINGS_FILE_PATH
      #define PFS_SETTINGS_FILE_PATH "/settings_main.bin"
    #endif

    #ifndef PFS_MINIMAL_SECTOR_MAGIC
      #define PFS_MINIMAL_SECTOR_MAGIC 0x50465331UL  // "PFS1"
    #endif

    #ifndef PFS_MINIMAL_SECTOR_VERSION
      #define PFS_MINIMAL_SECTOR_VERSION 1
    #endif

    #ifndef PFS_MINIMAL_SECTOR_SIZE
      #define PFS_MINIMAL_SECTOR_SIZE 4096UL
    #endif

    #ifndef PFS_MINIMAL_SECTOR_SETTINGS_MAIN_OFFSET
      #define PFS_MINIMAL_SECTOR_SETTINGS_MAIN_OFFSET 0x0000
    #endif

    enum PFSStorageTarget : uint8_t
    {
      PFS_STORAGE_INTERNAL = 0,
      PFS_STORAGE_ACTIVE   = 1,
      PFS_STORAGE_FLASH    = PFS_STORAGE_INTERNAL
    };

    enum PFSBackendType : uint8_t
    {
      PFS_BACKEND_NONE = 0,
      PFS_BACKEND_FILESYSTEM,
      PFS_BACKEND_MINIMAL_SECTOR
    };

    struct PFSMinimalSectorHeader
    {
      uint32_t magic;
      uint16_t version;
      uint16_t header_size;
      uint32_t data_size;
      uint32_t data_crc;
      uint32_t reserved;
    };

    PFSBackendType pfs_backend = PFS_BACKEND_NONE;

    uint32_t GetFreeStorageSpace(PFSStorageTarget target = PFS_STORAGE_INTERNAL);
    uint32_t SubCall__GetFreeStorageSpace__Active(void);
    uint32_t SubCall__GetFreeStorageSpace__LittleFS(void);
    uint32_t SubCall__GetFreeStorageSpace__FFat(void);

    bool PFS_Init();
    bool PFS_IsAvailable() const;
    PFSBackendType PFS_GetBackendType() const;
    const char* PFS_GetBackendName() const;

    bool PFS_SaveSettings(const uint8_t* data, uint32_t len);
    bool PFS_LoadSettings(uint8_t* data, uint32_t max_len, uint32_t* loaded_len = nullptr);
    bool PFS_SettingsExists();

    bool PFS_File_SaveSettings(const uint8_t* data, uint32_t len);
    bool PFS_File_LoadSettings(uint8_t* data, uint32_t max_len, uint32_t* loaded_len);
    bool PFS_File_SettingsExists();

    bool PFS_MinimalSector_Init();
    bool PFS_MinimalSector_Save(uint32_t sector_offset, const uint8_t* data, uint32_t len);
    bool PFS_MinimalSector_Load(uint32_t sector_offset, uint8_t* data, uint32_t max_len, uint32_t* loaded_len);
    bool PFS_MinimalSector_Exists(uint32_t sector_offset);
    uint32_t PFS_MinimalSector_CRC32(const uint8_t* data, uint32_t len) const;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    void SystemTask__Execute_Module_Data_Save();

    void ByteFile_Save(char* filename_With_extension, uint8_t* buffer, uint16_t buflen);
    uint32_t ByteFile_Load(char* filename_With_extension, uint8_t* buffer, uint16_t buflen);
    void JSONFile_Save(char* filename_With_extension, char* buffer, uint16_t buflen);
    void JSONFile_Load(char* filename_With_extension, char* buffer, uint16_t buflen);

    bool   FileExists(const char *fname);
    size_t FileSize(const char *fname);
    bool   SaveFile(const char *fname, const uint8_t *buf, uint32_t len);
    bool   InitFile(const char *fname, uint32_t len, uint8_t init_value);
    bool   LoadFile(const char *fname, uint8_t *buf, uint32_t len);
    String LoadString(const char *fname);
    bool   DeleteFile(const char *fname);
    bool   RenameFile(const char *fname1, const char *fname2);

    #if defined(ARDUINO_ARCH_ESP32)
    JsonDocument *pDoc = nullptr;
    SemaphoreHandle_t jsonBufferLockMutex = xSemaphoreCreateRecursiveMutex();
    #else
    StaticJsonDocument<JSON_BUFFER_SIZE> gDoc;
    JsonDocument *pDoc = &gDoc;
    #endif

    bool IsMounted(void) const;

    void Handle_FileChanges_WebUIEdits();

    bool doCloseFile = false;
    byte errorFlag = 0;
    size_t fsBytesUsed = 0;
    size_t fsBytesTotal = 0;
    unsigned long presetsModifiedTime = 0L;
    bool psramSafe = true;

    void closeFile();
    bool bufferedFind(const char *target, bool fromStart = true);
    bool bufferedFindSpace(size_t targetLen, bool fromStart = true);
    bool bufferedFindObjectEnd();
    void writeSpace(size_t l);
    bool appendObjectToFile(const char* key, JsonDocument* content, uint32_t s, uint32_t contentLen = 0);
    bool writeObjectToFileUsingId(const char* file, uint16_t id, JsonDocument* content);
    bool writeObjectToFile(const char* file, const char* key, JsonDocument* content);
    bool readObjectFromFileUsingId(const char* file, uint16_t id, JsonDocument* dest, const JsonDocument* filter = nullptr);
    bool readObjectFromFile(const char* file, const char* key, JsonDocument* dest, const JsonDocument* filter = nullptr);
    void updateFSInfo();

    inline bool writeObjectToFileUsingId(const String &file, uint16_t id, const JsonDocument* content) { return writeObjectToFileUsingId(file.c_str(), id, content); };
    inline bool writeObjectToFile(const String &file, const char* key, const JsonDocument* content) { return writeObjectToFile(file.c_str(), key, content); };
    inline bool readObjectFromFileUsingId(const String &file, uint16_t id, JsonDocument* dest, const JsonDocument* filter = nullptr) { return readObjectFromFileUsingId(file.c_str(), id, dest); };
    inline bool readObjectFromFile(const String &file, const char* key, JsonDocument* dest, const JsonDocument* filter = nullptr) { return readObjectFromFile(file.c_str(), key, dest); };


    String getContentType(AsyncWebServerRequest* request, String filename);
    bool handleFileRead(AsyncWebServerRequest* request, String path);

    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void readFile(fs::FS &fs, const char * path);

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);
    void CommandSet_ReadFile(const char* filename);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECTION: MQTT
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT
    void Telemetry_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();
    void MQTTHandler_Sender();

    std::vector<struct telemetry_handler<mFileSystem>*> telemetry_list;

    struct telemetry_handler<mFileSystem> telemetry_settings;
    struct telemetry_handler<mFileSystem> telemetry_sensor_ifchanged;
    struct telemetry_handler<mFileSystem> telemetry_sensor_teleperiod;
    #endif

};

#endif // USE_MODULE_CORE_FILESYSTEM

#endif // _MODULE_CORE_FILESYSTEM_H
