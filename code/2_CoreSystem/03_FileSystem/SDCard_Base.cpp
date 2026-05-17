#include "mFileSystem.h"

/************************************************************************************************
 * FILE: SDCard_Base.cpp
 *
 * PURPOSE:
 * - SD card mount and generic SD filesystem operations.
 * - SD is treated as an optional backend of mFileSystem.
 * - No separate SD driver module should exist.
 *
 * VIRTUAL PATH:
 * - Public/system-facing path: /sd/log/test.txt
 * - Local SD path passed to SD.open(): /log/test.txt
 *
 * Date Modified: 16May26
 ************************************************************************************************/

#ifdef USE_MODULE_FILESYSTEM_SDCARD



/************************************************************************************************
 * SECTION: SD CARD BASE STATE
 ************************************************************************************************/
/************************************************************************************************
 * FUNCTION: SDCard_Init
 *
 * SUMMARY:
 * - Initialise optional SD filesystem backend.
 * - If SD pins are missing, SD is disabled cleanly.
 * - If SD pins exist but mount fails, retry service may attempt remount later.
 *
 * CHANGED:
 * - 16May26: SD enable state now follows configured pin presence.
 ************************************************************************************************/
bool mFileSystem::SDCard_Init()
{
  sdcard.enabled = false;
  sdcard.mounted = false;
  sdcard.mount_failed = false;

  sdcard.card_type = CARD_NONE;
  sdcard.card_size_bytes = 0;
  sdcard.total_bytes = 0;
  sdcard.used_bytes = 0;

  ALOG_INF(PSTR(D_LOG_FILESYSTEM "SDCard_Init"));

  const bool mounted = SDCard_Mount();

  if (!sdcard.enabled)
  {
    ALOG_INF(PSTR(D_LOG_FILESYSTEM "SDCard_Init: SD backend disabled"));
    return false;
  }

  if (!mounted)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_Init: SD backend enabled but mount failed"));
    return false;
  }

  SDCard_ListDir("/sd/", 0);

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDCard_Init: size=%lluMB total=%lluMB used=%lluMB"),
    sdcard.card_size_bytes / (1024ULL * 1024ULL),
    sdcard.total_bytes / (1024ULL * 1024ULL),
    sdcard.used_bytes / (1024ULL * 1024ULL)
  );

  return true;
}

/************************************************************************************************
 * FUNCTION: SDCard_Mount
 *
 * SUMMARY:
 * - Mount SD card as an optional mFileSystem backend.
 * - SD is enabled only if all required SD SPI pins are configured.
 * - No fallback pins are used.
 * - Missing pins means SD is disabled, not failed.
 *
 * CHANGED:
 * - 16May26: Removed fallback pins. SD now enables only when all SD pins are configured.
 ************************************************************************************************/
bool mFileSystem::SDCard_Mount()
{
  if (sdcard.mounted) {
    return true;
  }

  /************************************************************************************************
   * SECTION: Resolve SD pins
   *
   * SD card is a core filesystem backend, but optional hardware.
   *
   * Therefore:
   * - If all SD pins are configured, SD is enabled and mount is attempted.
   * - If any SD pin is missing, SD is disabled cleanly.
   * - Missing pins are not treated as mount failure.
   * - No fallback/default pins are used.
   ************************************************************************************************/

  sdcard.pin_cs   = -1;
  sdcard.pin_sck  = -1;
  sdcard.pin_mosi = -1;
  sdcard.pin_miso = -1;

  sdcard.pin_cs   = tkr_pins->GetPin(GPIO_FUNCTION_SDCARD_HSPI_CSO);
  sdcard.pin_sck  = tkr_pins->GetPin(GPIO_FUNCTION_SDCARD_HSPI_CLK);
  sdcard.pin_mosi = tkr_pins->GetPin(GPIO_FUNCTION_SDCARD_HSPI_MOSI);
  sdcard.pin_miso = tkr_pins->GetPin(GPIO_FUNCTION_SDCARD_HSPI_MISO);

  const bool pins_configured =
    (sdcard.pin_cs   >= 0) &&
    (sdcard.pin_sck  >= 0) &&
    (sdcard.pin_mosi >= 0) &&
    (sdcard.pin_miso >= 0);

  if (!pins_configured)
  {
    sdcard.enabled = false;
    sdcard.mounted = false;
    sdcard.mount_failed = false;

    sdcard.card_type = CARD_NONE;
    sdcard.card_size_bytes = 0;
    sdcard.total_bytes = 0;
    sdcard.used_bytes = 0;

    ALOG_INF(
      PSTR(D_LOG_FILESYSTEM "SDCard_Mount: SD disabled, pins not configured cs=%d sck=%d mosi=%d miso=%d"),
      sdcard.pin_cs,
      sdcard.pin_sck,
      sdcard.pin_mosi,
      sdcard.pin_miso
    );

    return false;
  }

  /************************************************************************************************
   * SECTION: SD mount attempt
   *
   * From this point onward, SD is considered configured hardware.
   * Failures here are real mount failures and should be retried.
   ************************************************************************************************/

  sdcard.enabled = true;
  sdcard.mount_attempts++;
  sdcard.last_mount_ms = millis();

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDCard_Mount pins sck=%d miso=%d mosi=%d cs=%d"),
    sdcard.pin_sck,
    sdcard.pin_miso,
    sdcard.pin_mosi,
    sdcard.pin_cs
  );

  spiSD.begin(
    sdcard.pin_sck,
    sdcard.pin_miso,
    sdcard.pin_mosi,
    sdcard.pin_cs
  );

  if (!SD.begin(sdcard.pin_cs, spiSD))
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_Mount: SD.begin failed"));

    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;

    sdcard.card_type = CARD_NONE;
    sdcard.card_size_bytes = 0;
    sdcard.total_bytes = 0;
    sdcard.used_bytes = 0;

    return false;
  }

  sdcard.card_type = SD.cardType();

  if (sdcard.card_type == CARD_NONE)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_Mount: no SD card attached"));

    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;

    sdcard.card_size_bytes = 0;
    sdcard.total_bytes = 0;
    sdcard.used_bytes = 0;

    return false;
  }

    sdcard.mounted = true;
    sdcard.mount_failed = false;
    sdcard.card_removed = false;
    sdcard.health_check_failures = 0;
    sdcard.last_health_check_ms = millis();

    SDCard_RefreshInfo();

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDCard_Mount: mounted type=%s size=%lluMB total=%lluMB used=%lluMB"),
    SDCard_CardTypeName(sdcard.card_type),
    sdcard.card_size_bytes / (1024ULL * 1024ULL),
    sdcard.total_bytes / (1024ULL * 1024ULL),
    sdcard.used_bytes / (1024ULL * 1024ULL)
  );

  return true;
}


bool mFileSystem::SDCard_Unmount()
{
  if (!sdcard.mounted) {
    return true;
  }

  #if defined(USE_FILESYSTEM_SDCARD_BUFFERS)
    if (SDLog_IsOpen()) {
      SDLog_Close();
    }
  #endif

  SD.end();

  sdcard.mounted = false;
  sdcard.card_type = CARD_NONE;
  sdcard.card_size_bytes = 0;
  sdcard.total_bytes = 0;
  sdcard.used_bytes = 0;

  ALOG_INF(PSTR(D_LOG_FILESYSTEM "SDCard_Unmount: complete"));

  return true;
}


bool mFileSystem::SDCard_IsMounted() const
{
  return sdcard.mounted;
}


bool mFileSystem::SDCard_RefreshInfo()
{
  if (!sdcard.mounted) {
    return false;
  }

  sdcard.card_type = SD.cardType();

  if (sdcard.card_type == CARD_NONE) {
    sdcard.mounted = false;
    sdcard.mount_failed = true;
    return false;
  }

  sdcard.card_size_bytes = SD.cardSize();
  sdcard.total_bytes = SD.totalBytes();
  sdcard.used_bytes = SD.usedBytes();

  return true;
}


const char* mFileSystem::SDCard_CardTypeName(uint8_t card_type) const
{
  switch (card_type)
  {
    case CARD_MMC:  return "MMC";
    case CARD_SD:   return "SDSC";
    case CARD_SDHC: return "SDHC";
    case CARD_NONE:
    default:        return "NONE";
  }
}

/************************************************************************************************
 * FUNCTION: SDCard_ServiceMountRetry
 *
 * SUMMARY:
 * - Retry SD mount only when SD pins were configured and a real mount failed.
 * - Do not retry when SD is disabled due to missing pins.
 *
 * CHANGED:
 * - 16May26: Added sdcard.enabled gate to avoid retry spam on devices without SD pins.
 ************************************************************************************************/
void mFileSystem::SDCard_ServiceMountRetry()
{
  if (!sdcard.enabled) {
    return;
  }

  if (sdcard.mounted) {
    return;
  }

  if (!sdcard.mount_failed) {
    return;
  }

  if (FILESYSTEM_SDCARD_MOUNT_RETRY_MAX > 0)
  {
    if (sdcard.mount_attempts >= FILESYSTEM_SDCARD_MOUNT_RETRY_MAX) {
      return;
    }
  }

  if ((millis() - sdcard.last_mount_ms) < FILESYSTEM_SDCARD_MOUNT_RETRY_PERIOD_MS) {
    return;
  }

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDCard_ServiceMountRetry: attempt=%u failures=%u"),
    (unsigned)sdcard.mount_attempts + 1,
    (unsigned)sdcard.mount_failures
  );

  SDCard_Mount();
}

/************************************************************************************************
 * SECTION: SD VIRTUAL PATH ROUTING
 ************************************************************************************************/

bool mFileSystem::SDCard_ResolveLocalPath(
  const char* virtual_path,
  const char** local_path
) const
{
  if (!virtual_path || !local_path) {
    return false;
  }

  // Accept "/sd" as SD root.
  if (strcmp(virtual_path, "/sd") == 0) {
    *local_path = "/";
    return true;
  }

  // Accept "/sd/" and anything below it.
  if (strncmp(virtual_path, "/sd/", 4) == 0) {
    *local_path = virtual_path + 3;  // "/sd/log/a.txt" -> "/log/a.txt"
    return true;
  }

  return false;
}


/************************************************************************************************
 * SECTION: SD FILE OPERATIONS
 ************************************************************************************************/

File mFileSystem::SDCard_Open(const char* virtual_path, const char* mode)
{
  if (!virtual_path || !mode) {
    return File();
  }

  if (!sdcard.mounted) {
    if (!SDCard_Mount()) {
      ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_Open: mount failed path=%s"), virtual_path);
      return File();
    }
  }

  const char* local_path = nullptr;

  if (!SDCard_ResolveLocalPath(virtual_path, &local_path)) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_Open: invalid SD virtual path=%s"), virtual_path);
    return File();
  }
    
    File file = SD.open(local_path, mode);

    if (!file)
    {
    if (sdcard.mounted)
    {
        ALOG_WRN(
        PSTR(D_LOG_FILESYSTEM "SDCard_Open: open failed, marking SD unhealthy path=%s"),
        virtual_path
        );

        SDCard_MarkUnmounted();
    }

    return File();
    }

    return file;
}


bool mFileSystem::SDCard_Exists(const char* virtual_path)
{
  if (!virtual_path) {
    return false;
  }

  if (!sdcard.mounted && !SDCard_Mount()) {
    return false;
  }

  const char* local_path = nullptr;

  if (!SDCard_ResolveLocalPath(virtual_path, &local_path)) {
    return false;
  }

  return SD.exists(local_path);
}


bool mFileSystem::SDCard_Remove(const char* virtual_path)
{
  if (!virtual_path) {
    return false;
  }

  if (!sdcard.mounted && !SDCard_Mount()) {
    return false;
  }

  const char* local_path = nullptr;

  if (!SDCard_ResolveLocalPath(virtual_path, &local_path)) {
    return false;
  }

  return SD.remove(local_path);
}


bool mFileSystem::SDCard_Rename(const char* from_virtual_path, const char* to_virtual_path)
{
  if (!from_virtual_path || !to_virtual_path) {
    return false;
  }

  if (!sdcard.mounted && !SDCard_Mount()) {
    return false;
  }

  const char* from_local_path = nullptr;
  const char* to_local_path = nullptr;

  if (!SDCard_ResolveLocalPath(from_virtual_path, &from_local_path)) {
    return false;
  }

  if (!SDCard_ResolveLocalPath(to_virtual_path, &to_local_path)) {
    return false;
  }

  return SD.rename(from_local_path, to_local_path);
}


bool mFileSystem::SDCard_Mkdir(const char* virtual_path)
{
  if (!virtual_path) {
    return false;
  }

  if (!sdcard.mounted && !SDCard_Mount()) {
    return false;
  }

  const char* local_path = nullptr;

  if (!SDCard_ResolveLocalPath(virtual_path, &local_path)) {
    return false;
  }

  return SD.mkdir(local_path);
}


bool mFileSystem::SDCard_Rmdir(const char* virtual_path)
{
  if (!virtual_path) {
    return false;
  }

  if (!sdcard.mounted && !SDCard_Mount()) {
    return false;
  }

  const char* local_path = nullptr;

  if (!SDCard_ResolveLocalPath(virtual_path, &local_path)) {
    return false;
  }

  return SD.rmdir(local_path);
}


void mFileSystem::SDCard_ListDir(const char* virtual_path, uint8_t levels)
{
  if (!virtual_path) {
    virtual_path = "/sd/";
  }

  if (!sdcard.mounted && !SDCard_Mount()) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_ListDir: SD not mounted"));
    return;
  }

  const char* local_path = nullptr;

  if (!SDCard_ResolveLocalPath(virtual_path, &local_path)) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_ListDir: invalid path=%s"), virtual_path);
    return;
  }

  File root = SD.open(local_path);

  if (!root) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_ListDir: failed open path=%s"), virtual_path);
    return;
  }

  if (!root.isDirectory()) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_ListDir: not directory path=%s"), virtual_path);
    root.close();
    return;
  }

  ALOG_INF(PSTR(D_LOG_FILESYSTEM "SDCard_ListDir: %s"), virtual_path);

  File file = root.openNextFile();

  while (file) {
    if (file.isDirectory()) {
      ALOG_INF(PSTR(D_LOG_FILESYSTEM "  DIR : %s"), file.name());

      if (levels) {
        // file.name() is already local to SD. Convert is not attempted here.
        // For deeper WebUI use, implement a JSON listing function instead.
      }
    } else {
      ALOG_INF(
        PSTR(D_LOG_FILESYSTEM "  FILE: %s SIZE: %u"),
        file.name(),
        (unsigned)file.size()
      );
    }

    file = root.openNextFile();
  }

  root.close();
}


bool mFileSystem::SDCard_ReadToSerial(const char* virtual_path)
{
  File file = SDCard_Open(virtual_path, FILE_READ);

  if (!file) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_ReadToSerial: failed path=%s"), virtual_path);
    return false;
  }

  while (file.available()) {
    Serial.write(file.read());
  }

  file.close();
  return true;
}


bool mFileSystem::SDCard_WriteAll(
  const char* virtual_path,
  const uint8_t* data,
  size_t len,
  const char* mode
)
{
  if (!virtual_path || !data || !mode) {
    return false;
  }

  File file = SDCard_Open(virtual_path, mode);

  if (!file) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_WriteAll: open failed path=%s"), virtual_path);
    return false;
  }

  size_t written = file.write(data, len);
  file.close();

  return (written == len);
}


bool mFileSystem::SDCard_WriteText(const char* virtual_path, const char* text, const char* mode)
{
  if (!text) {
    return false;
  }

  return SDCard_WriteAll(
    virtual_path,
    reinterpret_cast<const uint8_t*>(text),
    strlen(text),
    mode
  );
}


bool mFileSystem::SDCard_AppendText(const char* virtual_path, const char* text)
{
  return SDCard_WriteText(virtual_path, text, FILE_APPEND);
}


void mFileSystem::SDCard_TestFileIO(const char* virtual_path)
{
  if (!virtual_path) {
    virtual_path = "/sd/test.bin";
  }

  if (!sdcard.mounted && !SDCard_Mount()) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_TestFileIO: SD not mounted"));
    return;
  }

  File file = SDCard_Open(virtual_path, FILE_WRITE);

  if (!file) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_TestFileIO: write open failed path=%s"), virtual_path);
    return;
  }

  static uint8_t buf[512];
  memset(buf, 0xA5, sizeof(buf));

  uint32_t start = millis();

  for (uint16_t i = 0; i < 2048; i++) {
    file.write(buf, sizeof(buf));
  }

  uint32_t write_ms = millis() - start;
  file.close();

  file = SDCard_Open(virtual_path, FILE_READ);

  if (!file) {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_TestFileIO: read open failed path=%s"), virtual_path);
    return;
  }

  size_t remaining = file.size();
  size_t total_read = remaining;

  start = millis();

  while (remaining) {
    size_t to_read = remaining;
    if (to_read > sizeof(buf)) {
      to_read = sizeof(buf);
    }

    size_t n = file.read(buf, to_read);
    if (!n) {
      break;
    }

    remaining -= n;
  }

  uint32_t read_ms = millis() - start;
  file.close();

  ALOG_INF(
    PSTR(D_LOG_FILESYSTEM "SDCard_TestFileIO: wrote=%u bytes in %u ms, read=%u bytes in %u ms"),
    (unsigned)(2048UL * sizeof(buf)),
    (unsigned)write_ms,
    (unsigned)total_read,
    (unsigned)read_ms
  );
}

/************************************************************************************************
 * SECTION: SD CARD HEALTH / REMOUNT SERVICE
 *
 * Purpose:
 * - Detect SD card removal while running.
 * - Mark SD unmounted when removed/unhealthy.
 * - Retry mount automatically when card is reinserted.
 *
 * Behaviour:
 * - Missing SD pins: SD remains disabled, no retry.
 * - SD mounted: periodic health check.
 * - SD removed/unhealthy: SD.end(), mounted=false, retry active.
 * - SD reinserted: SDCard_ServiceMountRetry() attempts remount.
 *
 * Date Modified: 17May26
 ************************************************************************************************/


/************************************************************************************************
 * FUNCTION: SDCard_MarkUnmounted
 *
 * SUMMARY:
 * - Forces SD card state back to unmounted.
 * - Used when card is removed or SD backend becomes unhealthy.
 * - Does not disable SD permanently.
 *
 * CHANGED:
 * - 17May26: Added SD removal/remount support.
 ************************************************************************************************/
void mFileSystem::SDCard_MarkUnmounted()
{
  if (sdcard.mounted)
  {
    ALOG_WRN(PSTR(D_LOG_FILESYSTEM "SDCard_MarkUnmounted: SD removed or unhealthy"));
  }

#if defined(USE_FILESYSTEM_SDCARD_BUFFERS)
  if (SDLog_IsOpen()) {
    SDLog_Close();
  }
#endif

  SD.end();

  sdcard.mounted = false;
  sdcard.mount_failed = true;
  sdcard.card_removed = true;

  sdcard.card_type = CARD_NONE;
  sdcard.card_size_bytes = 0;
  sdcard.total_bytes = 0;
  sdcard.used_bytes = 0;

  sdcard.health_check_failures = 0;
  sdcard.last_mount_ms = millis();
}


/************************************************************************************************
 * FUNCTION: SDCard_HealthCheck
 *
 * SUMMARY:
 * - Checks whether a mounted SD card is still present and responsive.
 *
 * RETURNS:
 * - true  = SD appears healthy
 * - false = SD is missing/unhealthy and has been marked unmounted
 *
 * CHANGED:
 * - 17May26: Added periodic SD card health check.
 ************************************************************************************************/
bool mFileSystem::SDCard_HealthCheck()
{
  if (!sdcard.enabled) {
    return false;
  }

  if (!sdcard.mounted) {
    return false;
  }

  if ((millis() - sdcard.last_health_check_ms) < FILESYSTEM_SDCARD_HEALTH_CHECK_PERIOD_MS) {
    return true;
  }

  sdcard.last_health_check_ms = millis();

  /************************************************************************************************
   * SECTION: Card type check
   *
   * On SPI SD, SD.cardType() often returns CARD_NONE after card removal.
   ************************************************************************************************/

  uint8_t card_type = SD.cardType();

  if (card_type == CARD_NONE)
  {
    sdcard.health_check_failures++;

    ALOG_WRN(
      PSTR(D_LOG_FILESYSTEM "SDCard_HealthCheck: CARD_NONE failure=%u"),
      (unsigned)sdcard.health_check_failures
    );

    if (sdcard.health_check_failures >= FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT)
    {
      SDCard_MarkUnmounted();
      return false;
    }

    return true;
  }

  /************************************************************************************************
   * SECTION: Root directory check
   *
   * This catches cases where cardType still reports stale information but file IO has failed.
   ************************************************************************************************/

  File root = SD.open("/");

  if (!root)
  {
    sdcard.health_check_failures++;

    ALOG_WRN(
      PSTR(D_LOG_FILESYSTEM "SDCard_HealthCheck: root open failed failure=%u"),
      (unsigned)sdcard.health_check_failures
    );

    if (sdcard.health_check_failures >= FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT)
    {
      SDCard_MarkUnmounted();
      return false;
    }

    return true;
  }

  root.close();

  sdcard.card_type = card_type;
  sdcard.health_check_failures = 0;
  sdcard.card_removed = false;

  return true;
}


/************************************************************************************************
 * FUNCTION: SDCard_Service
 *
 * SUMMARY:
 * - Unified SD periodic service.
 * - If mounted, checks for removal.
 * - If unmounted but enabled/configured, retries mount.
 *
 * CHANGED:
 * - 17May26: Added single service function for health check and remount.
 ************************************************************************************************/
void mFileSystem::SDCard_Service()
{
  if (!sdcard.enabled) {
    return;
  }

  if (sdcard.mounted)
  {
    SDCard_HealthCheck();
    return;
  }

  SDCard_ServiceMountRetry();
}

/************************************************************************************************
 * SECTION: SD CARD JSON REPORTING
 *
 * Purpose:
 * - Construct SD-card status as a normal pointer-compatible ConstructJSON function.
 * - Append SD-card file list using a helper, not a ConstructJSON_* function.
 *
 * Date Modified: 17May26
 ************************************************************************************************/

#ifdef USE_MODULE_FILESYSTEM_SDCARD

uint8_t mFileSystem::ConstructJSON_SDCard(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  if (sdcard.mounted) {
    SDCard_RefreshInfo();
  }

  JBI->Add_P("Enabled",       sdcard.enabled);
  JBI->Add_P("Mounted",       sdcard.mounted);
  JBI->Add_P("MountFailed",   sdcard.mount_failed);
  JBI->Add_P("CardRemoved",   sdcard.card_removed);

  JBI->Add_P("CardType",      SDCard_CardTypeName(sdcard.card_type));

  JBI->Add_P("MountAttempts", sdcard.mount_attempts);
  JBI->Add_P("MountFailures", sdcard.mount_failures);
  JBI->Add_P("HealthFails",   sdcard.health_check_failures);

  JBI->Object_Start("Pins");
    JBI->Add_P("CS",   sdcard.pin_cs);
    JBI->Add_P("SCK",  sdcard.pin_sck);
    JBI->Add_P("MOSI", sdcard.pin_mosi);
    JBI->Add_P("MISO", sdcard.pin_miso);
  JBI->Object_End();

  JBI->Object_Start("Storage");
    JBI->Add_P("CardSizeMB", (uint32_t)(sdcard.card_size_bytes / (1024ULL * 1024ULL)));
    JBI->Add_P("TotalMB",    (uint32_t)(sdcard.total_bytes / (1024ULL * 1024ULL)));
    JBI->Add_P("UsedMB",     (uint32_t)(sdcard.used_bytes / (1024ULL * 1024ULL)));

    if (sdcard.total_bytes >= sdcard.used_bytes) {
      JBI->Add_P("FreeMB", (uint32_t)((sdcard.total_bytes - sdcard.used_bytes) / (1024ULL * 1024ULL)));
    } else {
      JBI->Add_P("FreeMB", 0);
    }
  JBI->Object_End();

  #if defined(USE_FILESYSTEM_SDCARD_BUFFERS)
  JBI->Object_Start("Logger");
    JBI->Add_P("Status",        (uint8_t)SDLog_GetStatus());
    JBI->Add_P("BytesWritten",  SDLog_GetBytesWritten());
    JBI->Add_P("BytesQueued",   SDLog_GetBytesQueued());
    JBI->Add_P("BytesDropped",  SDLog_GetBytesDropped());
    JBI->Add_P("WriteFailures", SDLog_GetWriteFailures());
    JBI->Add_P("Path",          SDLog_GetActivePath());
  JBI->Object_End();
  #endif

  if (json_level >= JSON_LEVEL_DETAILED) {
    AppendJSON_SDCard_Files("/sd/", 16);
  }

  return JBI->End();
}


/************************************************************************************************
 * FUNCTION: AppendJSON_SDCard_Files
 *
 * SUMMARY:
 * - Appends a file array to an already-open JSON object.
 * - This is a helper, not a ConstructJSON_* MQTT/query entry point.
 *
 * OUTPUT:
 * - "Files":["/test1.txt","/test2.txt"]
 * - "FileCount":2
 *
 * Date Modified: 17May26
 ************************************************************************************************/

void mFileSystem::AppendJSON_SDCard_Files(const char* virtual_dir, uint8_t max_files)
{
  if (!virtual_dir) {
    virtual_dir = "/sd/";
  }

  uint8_t file_count = 0;

  JBI->Array_Start("Files");

  if (!SDCard_IsMounted())
  {
    JBI->Array_End();
    JBI->Add_P("FileCount", file_count);
    return;
  }

  const char* local_path = nullptr;

  if (!SDCard_ResolveLocalPath(virtual_dir, &local_path))
  {
    JBI->Array_End();
    JBI->Add_P("FileCount", file_count);
    return;
  }

  File root = SD.open(local_path);

  if (!root || !root.isDirectory())
  {
    if (root) {
      root.close();
    }

    JBI->Array_End();
    JBI->Add_P("FileCount", file_count);
    return;
  }

  File file = root.openNextFile();

  while (file && file_count < max_files)
  {
    if (!file.isDirectory())
    {
      const char* name = file.name();

      if (name) {
        JBI->Add(name);
        file_count++;
      }
    }

    file = root.openNextFile();
  }

  root.close();

  JBI->Array_End();
  JBI->Add_P("FileCount", file_count);
}

#endif // USE_MODULE_FILESYSTEM_SDCARD



/************************************************************************************************
 * FUNCTION: subparse_JSONCommand__SDCards
 *
 * SUMMARY:
 * - Handles SD-card JSON commands.
 * - Kept separate from the main filesystem JSON parser so SD logic remains grouped with SD code.
 *
 * COMMANDS:
 * - {"SDCard":{"Info":1}}
 * - {"SDCard":{"Mount":1}}
 * - {"SDCard":{"Unmount":1}}
 * - {"SDCard":{"List":"/sd/"}}
 * - {"SDCard":{"ReadFile":"/sd/test1.txt"}}
 * - {"SDCard":{"CreateFile":"/sd/new.txt"}}
 * - {"SDCard":{"WriteFile":"/sd/new.txt","Data":"hello"}}
 * - {"SDCard":{"AppendFile":"/sd/new.txt","Data":"\nworld"}}
 * - {"SDCard":{"DeleteFile":"/sd/new.txt"}}
 *
 * CHANGED:
 * - 17May26: Split SD-card command handling into dedicated subparser.
 ************************************************************************************************/
void mFileSystem::subparse_JSONCommand__SDCards(JsonParserObject obj)
{
  JsonParserToken jtok = 0;

  if (!(jtok = obj["SDCard"]))
  {
    return;
  }

  JsonParserObject sd = jtok.getObject();

  /************************************************************************************************
   * SECTION: SD CARD STATUS
   ************************************************************************************************/

  if (sd["Info"])
  {
    ConstructJSON_SDCard(JSON_LEVEL_DETAILED, false);
    ALOG_INF(PSTR(D_LOG_FILESYSTEM "SDCard Info: %s"), JBI->GetBufferPtr());
    return;
  }


  /************************************************************************************************
   * SECTION: SD CARD MOUNT CONTROL
   ************************************************************************************************/

  if (sd["Mount"])
  {
    SDCard_Mount();
    return;
  }

  if (sd["Unmount"])
  {
    SDCard_Unmount();
    return;
  }


  /************************************************************************************************
   * SECTION: SD CARD FILE OPERATIONS
   ************************************************************************************************/

  if (jtok = sd["List"])
  {
    const char* path = jtok.getStr();

    if (!path) {
      path = "/sd/";
    }

    SDCard_ListDir(path, 1);
    return;
  }

  if (jtok = sd["ReadFile"])
  {
    const char* path = jtok.getStr();

    if (path) {
      SDCard_ReadToSerial(path);
    }

    return;
  }

  if (jtok = sd["CreateFile"])
  {
    const char* path = jtok.getStr();

    if (path) {
      SDCard_WriteText(path, "", FILE_WRITE);
    }

    return;
  }

  if (jtok = sd["WriteFile"])
  {
    const char* path = jtok.getStr();
    const char* data = sd["Data"].getStr();

    if (path && data) {
      SDCard_WriteText(path, data, FILE_WRITE);
    }

    return;
  }

  if (jtok = sd["AppendFile"])
  {
    const char* path = jtok.getStr();
    const char* data = sd["Data"].getStr();

    if (path && data) {
      SDCard_AppendText(path, data);
    }

    return;
  }

  if (jtok = sd["DeleteFile"])
  {
    const char* path = jtok.getStr();

    if (path) {
      SDCard_Remove(path);
    }

    return;
  }
}



#endif // USE_MODULE_FILESYSTEM_SDCARD