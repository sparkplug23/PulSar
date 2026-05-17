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


#ifdef ESP8266
  #include <SPIFFSEditor.h>
  #include <FS.h>
  #include <LittleFS.h>
  #include <SPI.h>
  #ifdef USE_MODULE_FILESYSTEM_SDCARD
    #include <SD.h>
    #include <SDFAT.h>
  #endif
#endif  // ESP8266
#ifdef ESP32
  #include <LittleFS.h>
  #ifdef USE_MODULE_FILESYSTEM_SDCARD
    #include <SD.h>
  #endif
  #include "FFat.h"
  #include "FS.h"
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

#include <SPIFFSEditor.h>

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

    
    /*********************************************************************************************\
    This driver adds universal file system support for
    - ESP8266 (sd card or littlefs on  > 1 M devices with special linker file e.g. eagle.flash.4m2m.ld)
      (makes no sense on 1M devices without sd card)
    - ESP32 (sd card or littlefs or sfatfile system).

    The sd card chip select is the standard SDCARD_CS or when not found SDCARD_CS_PIN and initializes
    the FS System Pointer ufsp which can be used by all standard file system calls.

    The only specific call is UfsInfo() which gets the total size (0) and free size (1).

    A button is created in the setup section to show up the file directory to download and upload files
    subdirectories are supported.

    Supported commands:
    ufs       fs info
    ufstype   get filesytem type 0=none 1=SD  2=Flashfile
    ufssize   total size in kB
    ufsfree   free size in kB
    \*********************************************************************************************/

    #define UFS_TNONE         0
    #define UFS_TSDC          1
    #define UFS_TFAT          2
    #define UFS_TLFS          3

    // Global file system pointer
    FS *ufsp = nullptr;
    // Flash file system pointer
    FS *ffsp = nullptr;
    // Local pointer for file managment
    FS *dfsp = nullptr;

    char ufs_path[48];
    File ufs_upload_file;
    uint8_t ufs_dir;    
    uint8_t ufs_type; // 0 = None, 1 = SD, 2 = ffat, 3 = littlefs
    uint8_t ffs_type;

    struct {
      char run_file[48];
      int run_file_pos = -1;
      bool run_file_mutex = 0;
      bool download_busy;
    } UfsData;


    enum FileStorageTarget : uint8_t
    {
      FILE_STORAGE_INTERNAL = 0,   // Internal LittleFS, system/module config
      FILE_STORAGE_ACTIVE   = 1,   // Currently selected filesystem
      FILE_STORAGE_SD       = 2,   // SD card, future high-volume/user files
      FILE_STORAGE_FLASH    = FILE_STORAGE_INTERNAL
    };
    uint32_t GetFreeStorageSpace(FileStorageTarget target = FILE_STORAGE_INTERNAL);
    uint32_t SubCall__GetFreeStorageSpace__Active(void);
    uint32_t SubCall__GetFreeStorageSpace__LittleFS(void);
    uint32_t SubCall__GetFreeStorageSpace__FFat(void);
    uint32_t SubCall__GetFreeStorageSpace__SD(void);

    void SystemTask__Execute_Module_Data_Save();

    void JsonFile_Save__Stored_Module();
    bool JsonFile_Load__Stored_Module();
    void JsonFile_Load__Stored_Module_Or_Default_Template();
    void JsonFile_Save__Stored_Secure();
    void JsonFile_Load__Stored_Secure();    
    
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
     * SECTION: SD CARD FILESYSTEM BACKEND
     *
     * Purpose:
     * - SD card is owned by mFileSystem, not by a separate driver.
     * - Provides optional /sd/... filesystem backend.
     * - Handles pin discovery, SPI init, mount, card info, and direct SD file access.
     *
     * Virtual path convention:
     * - /system/...  -> existing internal filesystem, e.g. LittleFS
     * - /sd/...      -> SD card filesystem
     *
     * Date Modified: 16May26
     ************************************************************************************************/

    #ifdef USE_MODULE_FILESYSTEM_SDCARD

      #include "FS.h"
      #include "SD.h"
      #include "SPI.h"

      #ifndef FILESYSTEM_SDCARD_HEALTH_CHECK_PERIOD_MS
        #define FILESYSTEM_SDCARD_HEALTH_CHECK_PERIOD_MS 5000UL
      #endif

      #ifndef FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT
        #define FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT 2
      #endif

      struct SDCardState
      {
        bool enabled = false;
        bool mounted = false;
        bool mount_failed = false;

        int8_t pin_cs   = -1;
        int8_t pin_sck  = -1;
        int8_t pin_mosi = -1;
        int8_t pin_miso = -1;

        uint8_t card_type = CARD_NONE;

        uint64_t card_size_bytes  = 0;
        uint64_t total_bytes      = 0;
        uint64_t used_bytes       = 0;

        uint32_t mount_attempts   = 0;
        uint32_t mount_failures   = 0;
        uint32_t last_mount_ms    = 0;

        uint32_t last_health_check_ms = 0;
        uint32_t health_check_failures = 0;
        bool card_removed = false;

      } sdcard;

      // Keep SD SPI ownership inside filesystem.
      // No separate SD driver module.
      SPIClass spiSD = SPIClass(HSPI);

      bool SDCard_Init();
      bool SDCard_Mount();
      bool SDCard_Unmount();
      bool SDCard_IsMounted() const;
      bool SDCard_RefreshInfo();  
      void SDCard_ServiceMountRetry();
      const char* SDCard_CardTypeName(uint8_t card_type) const;
      bool SDCard_HealthCheck();
      void SDCard_MarkUnmounted();
      void SDCard_Service();

      #ifndef FILESYSTEM_SDCARD_MOUNT_RETRY_PERIOD_MS
        #define FILESYSTEM_SDCARD_MOUNT_RETRY_PERIOD_MS 10000UL
      #endif

      #ifndef FILESYSTEM_SDCARD_MOUNT_RETRY_MAX
        #define FILESYSTEM_SDCARD_MOUNT_RETRY_MAX 0
      #endif
      // 0 = retry forever, useful while debugging SD bring-up
      
      AsyncWebHandler *editSDHandler = nullptr;

      bool SDCard_ResolveLocalPath(
        const char* virtual_path,
        const char** local_path
      ) const;

      File SDCard_Open(const char* virtual_path, const char* mode);
      bool SDCard_Exists(const char* virtual_path);
      bool SDCard_Remove(const char* virtual_path);
      bool SDCard_Rename(const char* from_virtual_path, const char* to_virtual_path);
      bool SDCard_Mkdir(const char* virtual_path);
      bool SDCard_Rmdir(const char* virtual_path);

      void SDCard_ListDir(const char* virtual_path = "/sd/", uint8_t levels = 0);
      bool SDCard_ReadToSerial(const char* virtual_path);
      bool SDCard_WriteAll(const char* virtual_path, const uint8_t* data, size_t len, const char* mode = FILE_WRITE);
      bool SDCard_WriteText(const char* virtual_path, const char* text, const char* mode = FILE_WRITE);
      bool SDCard_AppendText(const char* virtual_path, const char* text);
      void SDCard_TestFileIO(const char* virtual_path);

    #endif // USE_MODULE_FILESYSTEM_SDCARD


    /************************************************************************************************
     * SECTION: SD CARD EDITOR WEB ACCESS
     *
     * Purpose:
     * - Registers SD-backed editor route using a project-local SDCardEditor handler.
     * - Uses generated gzip page PAGE_sd_editor from html_sdcard_editor.h.
     * - Drops the previous manual /sd browser/API method.
     *
     * Route:
     * - /sdedit
     *
     * Date Modified: 17May26
     ************************************************************************************************/
    #if defined(USE_MODULE_FILESYSTEM_SDCARD) && defined(USE_MODULE_NETWORK_WEBSERVER)

      void WebPage_Root_AddHandlers();

      void Web_SDCardEditor_GET(AsyncWebServerRequest* request);
      void Web_SDCardEditor_PUT(AsyncWebServerRequest* request);
      void Web_SDCardEditor_DELETE(AsyncWebServerRequest* request);
      void Web_SDCardEditor_POST_Final(AsyncWebServerRequest* request);

      void Web_SDCardEditor_POST_Upload(
        AsyncWebServerRequest* request,
        const String& filename,
        size_t index,
        uint8_t* data,
        size_t len,
        bool final
      );

      String Web_SDCardEditor_ContentTypeFromPath(const String& path);
      String Web_SDCardEditor_JSONEscape(const String& in);
      bool Web_SDCardEditor_NormalisePath(String& path);

      void AppendJSON_SDCard_Files(const char* virtual_dir, uint8_t max_files);

    #endif

    /************************************************************************************************
     * SECTION: SD CARD BUFFERED WRITE SUBSYSTEM
     *
     * Purpose:
     * - Filesystem-owned high-rate SD logging path.
     * - Producers such as GPS/UART/sensors only push bytes/lines into this subsystem.
     * - This owns the open File handle, ringbuffer, writer task, flush/close policy, and counters.
     *
     * Important:
     * - This is still part of mFileSystem, not a GPS/UART/controller module.
     * - Producers decide what bytes mean.
     * - Filesystem decides how bytes are safely written to SD.
     *
     * Date Modified: 16May26
     ************************************************************************************************/

    #if defined(USE_MODULE_FILESYSTEM_SDCARD) && defined(USE_FILESYSTEM_SDCARD_BUFFERS)

      #ifndef FILESYSTEM_SDLOG_RINGBUFFER_SIZE
        #define FILESYSTEM_SDLOG_RINGBUFFER_SIZE 65536
      #endif

      #ifndef FILESYSTEM_SDLOG_WRITER_STACK_SIZE
        #define FILESYSTEM_SDLOG_WRITER_STACK_SIZE 4096
      #endif

      #ifndef FILESYSTEM_SDLOG_WRITER_PRIORITY
        #define FILESYSTEM_SDLOG_WRITER_PRIORITY 1
      #endif

      #ifndef FILESYSTEM_SDLOG_WRITER_CORE
        #define FILESYSTEM_SDLOG_WRITER_CORE 0
      #endif

      #ifndef FILESYSTEM_SDLOG_CHUNK_SIZE
        #define FILESYSTEM_SDLOG_CHUNK_SIZE 4096
      #endif

      #ifndef FILESYSTEM_SDLOG_CLOSE_TIMEOUT_MS
        #define FILESYSTEM_SDLOG_CLOSE_TIMEOUT_MS 3000
      #endif

      enum SDLogStatus : uint8_t
      {
        SDLOG_STATUS_DISABLED = 0,
        SDLOG_STATUS_IDLE,
        SDLOG_STATUS_OPEN,
        SDLOG_STATUS_CLOSE_REQUESTED,
        SDLOG_STATUS_CLOSED,
        SDLOG_STATUS_ERROR
      };

      struct SDLogState
      {
        bool initialised = false;
        bool task_started = false;

        volatile SDLogStatus status = SDLOG_STATUS_DISABLED;

        RingbufHandle_t ringbuffer_handle = nullptr;
        TaskHandle_t writer_task_handle = nullptr;
        SemaphoreHandle_t file_mutex = nullptr;

        File file;

        char active_path[128] = {0};

        uint32_t bytes_written = 0;
        uint32_t bytes_queued = 0;
        uint32_t bytes_dropped = 0;
        uint32_t write_failures = 0;

        uint32_t open_count = 0;
        uint32_t close_count = 0;

        uint32_t last_write_ms = 0;
        uint32_t last_flush_ms = 0;
      } sdlog;

      bool SDLog_Init();
      bool SDLog_Open(const char* virtual_path, bool append = true);
      bool SDLog_IsOpen() const;
      bool SDLog_Write(const uint8_t* data, size_t len, uint32_t timeout_ms = 0);
      bool SDLog_Write(const char* data, uint32_t timeout_ms = 0);
      bool SDLog_WriteLine(const char* line, uint32_t timeout_ms = 0);
      bool SDLog_Flush();
      bool SDLog_Close();

      static void SDLog_WriterTask_Trampoline(void* param);
      void SDLog_WriterTask();

      uint32_t SDLog_GetBytesWritten() const;
      uint32_t SDLog_GetBytesQueued() const;
      uint32_t SDLog_GetBytesDropped() const;
      uint32_t SDLog_GetWriteFailures() const;
      SDLogStatus SDLog_GetStatus() const;
      const char* SDLog_GetActivePath() const;

      #ifdef ENABLE_DEVFEATURE_FILESYSTEM__SDCARD_LOGGING_PERFORMANCE_TEST
      void Loop__DevTest__FastSDLogging();
      #endif

    #endif // USE_MODULE_FILESYSTEM_SDCARD && USE_FILESYSTEM_SDCARD_BUFFERS


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