#ifndef _MODULE_CORE_FILESYSTEM_H
#define _MODULE_CORE_FILESYSTEM_H

#define D_UNIQUE_MODULE_CORE_FILESYSTEM_ID 2003 // [(Folder_Number*100)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_CORE_FILESYSTEM

// 03_FileSystem/
//   mFileSystem.cpp          // core tasker, init, status, shared helpers
//   mFileSystem.h            // single class declaration, all method prototypes
//   Module_SaveLoad.cpp      // config/template/settings persistence
//   SDCard_Base.cpp          // SD mount, card info, path routing backend
//   SDCard_Buffers.cpp       // high-rate buffered SD write subsystem


#include <string.h>
#include <strings.h>

// #ifndef PULSAR_HAS_FILESYSTEM
//   #define PULSAR_HAS_FILESYSTEM 1
// #endif

// #if defined(DISABLE_FILESYSTEM)
//   #undef  PULSAR_HAS_FILESYSTEM
//   #define PULSAR_HAS_FILESYSTEM 0
// #endif

#ifdef ESP8266
  // #include <SPIFFSEditor.h>
  // // #include <LittleFS.h>
  // #include <FS.h>
  // #include <LittleFS.h>
  // #include <SPI.h>
  // #ifdef USE_MODULE_FILESYSTEM_SDCARD
  //   #include <SD.h>
  //   #include <SDFAT.h>
  // #endif

  
  #ifdef ESP8266
  #include <LittleFS.h>
  #include <SPI.h>
  #ifdef USE_MODULE_FILESYSTEM_SDCARD
  #include <SD.h>
  #include <SdFat.h>
  #endif  // USE_MODULE_FILESYSTEM_SDCARD
  #endif  // ESP8266

  #include <SPIFFSEditor.h>

#endif  // ESP8266
#ifdef ESP32
  #include <LittleFS.h>


  #ifdef USE_MODULE_FILESYSTEM_SDCARD
    #include <SD.h>
  #endif


  #include "FFat.h"
  #include "FS.h"
  #include "SD.h"
  #include "SPI.h"

  #ifdef ESP32
    #include "SD_MMC.h"
  #endif

  #include <SPIFFSEditor.h>
#endif  // ESP32

#ifdef ESP32
  #include <WiFi.h>
  #ifndef DISABLE_NETWORK
  #ifdef USE_MODULE_NETWORK_WEBSERVER
    #include <AsyncTCP.h>
    #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
  #endif // DISABLE_NETWORK
#elif defined(ESP8266)
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  #endif // USE_MODULE_NETWORK_WEBSERVER
#endif


#define ARDUINOJSON_DECODE_UNICODE 0
#include "3_Network/21_WebServer/AsyncJson-v6.h"
#include "3_Network/21_WebServer/ArduinoJson-v6.h"

#include "3_Network/21_WebServer/Webpages/Generated/html_sdcard_editor.h"


// ESP32-WROVER features SPI RAM (aka PSRAM) which can be allocated using ps_malloc()
// we can create custom PSRAMDynamicJsonDocument to use such feature (replacing DynamicJsonDocument)
// The following is a construct to enable code to compile without it.
// There is a code that will still not use PSRAM though:
//    AsyncJsonResponse is a derived class that implements DynamicJsonDocument (AsyncJson-v6.h)
#if defined(ARDUINO_ARCH_ESP32)
// extern bool psramSafe;
struct PSRAM_Allocator {
  void* allocate(size_t size) {
    if (1 && psramFound()) return ps_malloc(size); // use PSRAM if it exists
    else                           return malloc(size);    // fallback
  }
  void* reallocate(void* ptr, size_t new_size) {
    if (1 && psramFound()) return ps_realloc(ptr, new_size); // use PSRAM if it exists
    else                           return realloc(ptr, new_size);    // fallback
  }
  void deallocate(void* pointer) {
    free(pointer);
  }
};
using PSRAMDynamicJsonDocument = BasicJsonDocument<PSRAM_Allocator>;
#else
#define PSRAMDynamicJsonDocument DynamicJsonDocument
#endif

#if defined(USE_MODULE_FILESYSTEM_SDCARD) && defined(USE_FILESYSTEM_SDCARD_BUFFERS)
  #include "freertos/FreeRTOS.h"
  #include "freertos/task.h"
  #include "freertos/ringbuf.h"
  #include "freertos/semphr.h"
#endif

#define FILE_EXTENSION_JSON ".json"
#define FILE_EXTENSION_BIN ".txt"   //debug version so the editor can open the file
// #define FILE_EXTENSION_BIN ".bin" //release version


#ifdef ESP32
  #include <AsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif
#ifdef ESP8266
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
#endif

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
    PGM_P GetModuleName(){         return PM_MODULE_CORE_FILESYSTEM_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_CORE_FILESYSTEM_ID; }
    
    struct ClassState
    {
      uint8_t devices = 0; // sensors/drivers etc, if class operates on multiple items how many are present.
      uint8_t mode = ModuleStatus::Initialising; // Disabled,Initialise,Running
    }module_state;

    /************************************************************************************************
     * SECTION: DATA_RUNTIME saved/restored on boot with filesystem
     ************************************************************************************************/

    /************************************************************************************************
 * SECTION: PFS - PulSar File System / Minimal Persistent Storage
 *
 * SUMMARY:
 * - PFS owns internal persistent storage only.
 * - If an internal filesystem is mounted, blobs are stored as files.
 * - If filesystem is disabled on ESP8266, blobs can fall back to fixed raw-flash slots.
 * - SD card is not part of PFS. SD card is owned by mSDCard.
 *
 * CHANGED:
 * - 31May26: Added PFS backend abstraction and ESP8266 raw flash fallback interface.
 ************************************************************************************************/

#define PFS_TNONE         0
#define PFS_TFAT          1
#define PFS_TLFS          2

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
  PFS_BACKEND_RAW_FLASH
};

enum PFSBlobId : uint8_t
{
  PFS_BLOB_SETTINGS_MAIN = 0,
  PFS_BLOB_SETTINGS_NETWORK,
  PFS_BLOB_SETTINGS_MODULE,
  PFS_BLOB_SETTINGS_FUNCTION,
  PFS_BLOB_COUNT
};

struct PFSBlobSlot
{
  PFSBlobId id;
  const char* name;
  const char* file_path;
  uint32_t raw_offset;
  uint32_t raw_size;
};

struct PFSRawFlashHeader
{
  uint32_t magic;
  uint16_t version;
  uint16_t header_size;
  uint32_t blob_id;
  uint32_t payload_len;
  uint32_t payload_crc;
  uint32_t sequence;
  uint32_t reserved;
};

PFSBackendType pfs_backend = PFS_BACKEND_NONE;

uint32_t GetFreeStorageSpace(PFSStorageTarget target = PFS_STORAGE_INTERNAL);
uint32_t SubCall__GetFreeStorageSpace__Active(void);
uint32_t SubCall__GetFreeStorageSpace__LittleFS(void);
uint32_t SubCall__GetFreeStorageSpace__FFat(void);

bool PFSBackend_Init();
bool PFSBackend_IsAvailable() const;
PFSBackendType PFSBackend_GetType() const;
const char* PFSBackend_GetTypeName() const;

const PFSBlobSlot* PFS_GetBlobSlot(PFSBlobId id) const;
const char* PFS_GetBlobName(PFSBlobId id) const;

bool PFS_SaveBlob(PFSBlobId id, const uint8_t* data, uint32_t len);
bool PFS_LoadBlob(PFSBlobId id, uint8_t* data, uint32_t max_len, uint32_t* loaded_len = nullptr);
bool PFS_DeleteBlob(PFSBlobId id);
bool PFS_BlobExists(PFSBlobId id);
uint32_t PFS_BlobSize(PFSBlobId id);

bool PFS_SaveSettingsBlob(const uint8_t* data, uint32_t len);
bool PFS_LoadSettingsBlob(uint8_t* data, uint32_t max_len, uint32_t* loaded_len = nullptr);
bool PFS_SettingsBlobExists();

bool PFS_FileBackend_SaveBlob(const PFSBlobSlot* slot, const uint8_t* data, uint32_t len);
bool PFS_FileBackend_LoadBlob(const PFSBlobSlot* slot, uint8_t* data, uint32_t max_len, uint32_t* loaded_len);
bool PFS_FileBackend_DeleteBlob(const PFSBlobSlot* slot);
bool PFS_FileBackend_BlobExists(const PFSBlobSlot* slot);
uint32_t PFS_FileBackend_BlobSize(const PFSBlobSlot* slot);

bool PFS_RawFlash_Init();
bool PFS_RawFlash_SaveBlob(const PFSBlobSlot* slot, const uint8_t* data, uint32_t len);
bool PFS_RawFlash_LoadBlob(const PFSBlobSlot* slot, uint8_t* data, uint32_t max_len, uint32_t* loaded_len);
bool PFS_RawFlash_DeleteBlob(const PFSBlobSlot* slot);
bool PFS_RawFlash_BlobExists(const PFSBlobSlot* slot);
uint32_t PFS_RawFlash_BlobSize(const PFSBlobSlot* slot);
uint32_t PFS_RawFlash_CRC32(const uint8_t* data, uint32_t len) const;

































    void SystemTask__Execute_Module_Data_Save();

    // void JsonFile_Save__Stored_Module();
    // bool JsonFile_Load__Stored_Module();
    // void JsonFile_Load__Stored_Module_Or_Default_Template();
    // void JsonFile_Save__Stored_Secure();
    // void JsonFile_Load__Stored_Secure();    
    
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

    bool doCloseFile =false;
    byte errorFlag = 0;
    size_t fsBytesUsed =0;
    size_t fsBytesTotal =0;
    unsigned long presetsModifiedTime = 0L;
    bool psramSafe = true;         // is it safe to use PSRAM (on ESP32 rev.1; compiler fix used "-mfix-esp32-psram-cache-issue")

    void closeFile();
    bool bufferedFind(const char *target, bool fromStart = true);
    bool bufferedFindSpace(size_t targetLen, bool fromStart = true);
    bool bufferedFindObjectEnd() ;
    void writeSpace(size_t l);
    bool appendObjectToFile(const char* key, JsonDocument* content, uint32_t s, uint32_t contentLen = 0);
    bool writeObjectToFileUsingId(const char* file, uint16_t id, JsonDocument* content);
    bool writeObjectToFile(const char* file, const char* key, JsonDocument* content);
    bool readObjectFromFileUsingId(const char* file, uint16_t id, JsonDocument* dest);
    bool readObjectFromFile(const char* file, const char* key, JsonDocument* dest);
    void updateFSInfo();

    String getContentType(AsyncWebServerRequest* request, String filename);
    bool handleFileRead(AsyncWebServerRequest* request, String path);

    void listDir(fs::FS &fs, const char * dirname, uint8_t levels);
    void readFile(fs::FS &fs, const char * path);
        
    /************************************************************************************************
     * SECTION: Internal Functions
     ************************************************************************************************/

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);
    #ifdef USE_MODULE_FILESYSTEM_SDCARD
    void subparse_JSONCommand__SDCards(JsonParserObject obj);
    #endif

    void CommandSet_ReadFile(const char* filename);
      
    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/
    
    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_Sensor(uint8_t json_level = 0, bool json_appending = true);
    #ifdef USE_MODULE_FILESYSTEM_SDCARD
    uint8_t ConstructJSON_SDCard(uint8_t json_level = 0, bool json_appending = true);
    #endif

    /************************************************************************************************
     * SECITON: MQTT
     ************************************************************************************************/
    
    #ifdef USE_MODULE_NETWORK_MQTT
    void MQTTHandler_Init();
    void MQTTHandler_RefreshAll();
    void MQTTHandler_Rate();    
    void MQTTHandler_Sender();
    std::vector<struct handler<mFileSystem>*> mqtthandler_list;    
    struct handler<mFileSystem> mqtthandler_settings;    
    #ifdef USE_MODULE_FILESYSTEM_SDCARD
    struct handler<mFileSystem> mqtthandler_sdcard;
    #endif
    struct handler<mFileSystem> mqtthandler_sensor_ifchanged;
    struct handler<mFileSystem> mqtthandler_sensor_teleperiod;    
    #endif // USE_MODULE_NETWORK_MQTT

};

#endif

#endif