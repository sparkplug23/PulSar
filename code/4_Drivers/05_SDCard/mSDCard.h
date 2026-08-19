#ifndef _MSDCARD_H
#define _MSDCARD_H

#define D_UNIQUE_MODULE_DRIVERS_SDCARD_ID 4005  // [(Folder_Number*1000)+ID_File]

#include "1_TaskerManager/mTaskerManager.h"

#ifdef USE_MODULE_DRIVERS_SDCARD

#include "1_TaskerManager/mTaskerInterface.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#ifdef ESP32
  #if defined(ENABLE_FEATURE_FILESYSTEM__SDCARD_MMC) && !defined(ENABLE_FEATURE_SDCARD__MMC)
    #define ENABLE_FEATURE_SDCARD__MMC
  #endif

  #ifdef ENABLE_FEATURE_SDCARD__MMC
    #include "SD_MMC.h"
  #endif
#endif

#if defined(USE_FILESYSTEM_SDCARD_BUFFERS) && !defined(USE_SDCARD_BUFFERS)
  #define USE_SDCARD_BUFFERS
#endif

#ifndef FILESYSTEM_SDCARD_MOUNT_RETRY_PERIOD_MS
  #define FILESYSTEM_SDCARD_MOUNT_RETRY_PERIOD_MS 10000UL
#endif

#ifndef FILESYSTEM_SDCARD_MOUNT_RETRY_MAX
  #define FILESYSTEM_SDCARD_MOUNT_RETRY_MAX 0
#endif

#ifndef FILESYSTEM_SDCARD_HEALTH_CHECK_PERIOD_MS
  #define FILESYSTEM_SDCARD_HEALTH_CHECK_PERIOD_MS 5000UL
#endif

#ifndef FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT
  #define FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT 2
#endif

class mSDCard :
  public mTaskerInterface
{
  public:
    /************************************************************************************************
     * SECTION: Construct Class Base
     ************************************************************************************************/
    mSDCard(){};
    void Pre_Init(void);
    void Init(void);
    int8_t Tasker(uint8_t function, JsonParserObject obj = 0);

    static constexpr const char* PM_MODULE_DRIVERS_SDCARD_CTR = D_MODULE_DRIVERS_SDCARD_CTR;
    PGM_P GetModuleName(){ return PM_MODULE_DRIVERS_SDCARD_CTR; }
    uint16_t GetModuleUniqueID(){ return D_UNIQUE_MODULE_DRIVERS_SDCARD_ID; }

    struct ClassState
    {
      uint8_t devices = 0;
      uint8_t mode = ModuleStatus::Initialising;
    } module_state;

    /************************************************************************************************
     * SECTION: SD CARD DRIVER BACKEND
     *
     * Purpose:
     * - SD card is owned by mSDCard, not mFileSystem.
     * - Handles pin discovery, SPI/SD_MMC init, mount, health, retry, and direct SD file access.
     *
     * Path convention:
     * - mSDCard uses local SD paths only:
     *     "/"
     *     "/logs/gps.ndjson"
     *     "/photos/img001.jpg"
     *
     * - Do NOT pass:
     *     "/sd/..."
     *     "/sdcard/..."
     *
     * Date Modified: 31May26
     ************************************************************************************************/

    struct SDCardState
    {
      bool enabled = false;
      bool mounted = false;
      bool mount_failed = false;
      bool pin_config_valid = false;

      // Active mounted filesystem backend.
      fs::FS* fs = nullptr;

      // SPI SD pins.
      int8_t pin_cs   = -1;
      int8_t pin_sck  = -1;
      int8_t pin_mosi = -1;
      int8_t pin_miso = -1;

      // SD_MMC pins.
      int8_t pin_mmc_clk = -1;
      int8_t pin_mmc_cmd = -1;
      int8_t pin_mmc_d0  = -1;
      int8_t pin_mmc_d1  = -1;
      int8_t pin_mmc_d2  = -1;
      int8_t pin_mmc_d3  = -1;

      bool using_mmc = false;
      bool using_mmc_1bit = true;

      uint8_t card_type = CARD_NONE;

      uint64_t card_size_bytes = 0;
      uint64_t total_bytes     = 0;
      uint64_t used_bytes      = 0;

      uint32_t mount_attempts = 0;
      uint32_t mount_failures = 0;
      uint32_t last_mount_ms  = 0;

      uint32_t last_health_check_ms = 0;
      uint32_t health_check_failures = 0;
      bool card_removed = false;

      uint16_t pin_config_missing_count = 0;

    } sdcard;

    #ifdef ESP32
      SPIClass spiSD_HSPI = SPIClass(HSPI);
      SPIClass spiSD_VSPI = SPIClass(VSPI);
    #else
      SPIClass spiSD;
    #endif

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

    #if defined(ESP32) && defined(ENABLE_FEATURE_SDCARD__MMC)
    bool SDCard_Mount_MMC();
    #endif

    bool SDCard_Mount_SPI_HSPI();
    bool SDCard_Mount_SPI_VSPI();

    /************************************************************************************************
     * SECTION: SD CARD LOCAL FILE IO
     *
     * Purpose:
     * - Direct SD-card file operations.
     * - All paths are local SD paths.
     *
     * Examples:
     * - Open("/test.txt", FILE_READ)
     * - Open("/logs/gps.ndjson", FILE_APPEND)
     *
     * Date Modified: 31May26
     ************************************************************************************************/

    File Open(const char* path, const char* mode);
    bool Exists(const char* path);
    bool Remove(const char* path);
    bool Rename(const char* from_path, const char* to_path);
    bool Mkdir(const char* path);
    bool Rmdir(const char* path);

    void ListDir(const char* path = "/", uint8_t levels = 0);
    bool ReadToSerial(const char* path);
    bool WriteAll(const char* path, const uint8_t* data, size_t len, const char* mode = FILE_WRITE);
    bool WriteText(const char* path, const char* text, const char* mode = FILE_WRITE);
    bool AppendText(const char* path, const char* text);
    void TestFileIO(const char* path);

    bool SaveFile(const char* path, const uint8_t* data, size_t len);
    bool AppendFile(const char* path, const uint8_t* data, size_t len);
    size_t FileSize(const char* path);

    /************************************************************************************************
     * SECTION: SD CARD EDITOR WEB ACCESS
     *
     * Purpose:
     * - Registers SD-backed editor route.
     * - Uses generated gzip page PAGE_sd_editor from html_sdcard_editor.h.
     *
     * Route:
     * - /sdedit
     *
     * Date Modified: 31May26
     ************************************************************************************************/

    #if defined(USE_MODULE_NETWORK_WEBSERVER)

    AsyncWebHandler* editSDHandler = nullptr;

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

    void AppendJSON_SDCard_Files(const char* path, uint8_t max_files);

    #endif // USE_MODULE_NETWORK_WEBSERVER

    /************************************************************************************************
     * SECTION: SD CARD BUFFERED WRITE SUBSYSTEM
     *
     * Purpose:
     * - High-rate SD logging path.
     * - Producers such as GPS/UART/sensors/camera only push bytes/lines into this subsystem.
     * - mSDCard owns the open File handle, ringbuffer, writer task, flush/close policy, and counters.
     *
     * Important:
     * - Paths are local SD paths.
     * - Do not pass "/sd/..." here.
     *
     * Date Modified: 31May26
     ************************************************************************************************/

    #if defined(USE_SDCARD_BUFFERS)

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
    bool SDLog_Open(const char* path, bool append = true);
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

    #ifdef ENABLE_DEVFEATURE_SDCARD__LOGGING_PERFORMANCE_TEST
    void Loop__DevTest__FastSDLogging();
    #endif

    #endif // USE_SDCARD_BUFFERS

    /************************************************************************************************
     * SECTION: Commands
     ************************************************************************************************/

    void parse_JSONCommand(JsonParserObject obj);

    /************************************************************************************************
     * SECTION: Construct Messages
     ************************************************************************************************/

    uint8_t ConstructJSON_Settings(uint8_t json_level = 0, bool json_appending = true);
    uint8_t ConstructJSON_State(uint8_t json_level = 0, bool json_appending = true);

    /************************************************************************************************
     * SECTION: MQTT
     ************************************************************************************************/

    #ifdef USE_MODULE_NETWORK_MQTT
    void Telemetry_Init();

    std::vector<struct telemetry_handler<mSDCard>*> telemetry_list;

    struct telemetry_handler<mSDCard> telemetry_settings;
    struct telemetry_handler<mSDCard> telemetry_state_ifchanged;
    struct telemetry_handler<mSDCard> telemetry_state_teleperiod;
    #endif

};

#endif // USE_MODULE_DRIVERS_SDCARD

#endif // _MSDCARD_H