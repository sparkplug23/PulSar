#include "mSDCard.h"

#ifdef USE_MODULE_DRIVERS_SDCARD

/************************************************************************************************
 * FUNCTION: Tasker
 *
 * SUMMARY:
 * - Main module task dispatcher.
 *
 * CHANGED:
 * - 31May26: Added standalone SDCard web handler registration and fixed init return behaviour.
 * - 31May26: Created minimum mSDCard driver module skeleton.
 ************************************************************************************************/
int8_t mSDCard::Tasker(uint8_t function, JsonParserObject obj)
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

  if(module_state.mode != ModuleStatus::Running)
  {
    return TASKER_RESULT__MODULE_DISABLED_ID;
  }

  switch(function)
  {
    /************
     * PERIODIC SECTION
     *******************/
    case TASK_EVERY_SECOND:
      SDCard_Service();
    break;

    /************
     * COMMANDS SECTION
     *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    /************
     * WEBUI SECTION
     *******************/
    #if defined(USE_MODULE_DRIVERS_SDCARD) && defined(USE_MODULE_NETWORK_WEBSERVER)
    case TASK_WEB_ADD_HANDLER:
      WebPage_Root_AddHandlers();
    break;
    #endif

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
}


/************************************************************************************************
 * FUNCTION: Pre_Init
 *
 * SUMMARY:
 * - Keeps SD service eligible.
 * - Does not mount.
 * - Safe to call before GPIO/template pins exist.
 *
 * CHANGED:
 * - 31May26: Created minimum SDCard pre-init.
 ************************************************************************************************/
void mSDCard::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;

  sdcard.enabled = true;
  sdcard.mounted = false;
  sdcard.mount_failed = false;
  sdcard.pin_config_valid = false;
  sdcard.fs = nullptr;

  sdcard.card_type = CARD_NONE;
  sdcard.card_size_bytes = 0;
  sdcard.total_bytes = 0;
  sdcard.used_bytes = 0;
}

/************************************************************************************************
 * FUNCTION: Init
 *
 * SUMMARY:
 * - Attempts SD mount using current GPIO/template configuration.
 * - Missing pins are not fatal, service remains available for runtime retry.
 *
 * CHANGED:
 * - 31May26: Created minimum SDCard init.
 ************************************************************************************************/
void mSDCard::Init(void)
{
  ALOG_INF(PSTR(D_LOG_SDCARD "SDCard Init"));

  sdcard.enabled = true;

  SDCard_Init();

  module_state.devices = 1;
  module_state.mode = ModuleStatus::Running;
}

/************************************************************************************************
 * FUNCTION: SDCard_Init
 *
 * SUMMARY:
 * - Wrapper around mount logic.
 * - Does not permanently disable SD if pins are missing.
 *
 * CHANGED:
 * - 31May26: Created standalone SDCard init.
 ************************************************************************************************/
bool mSDCard::SDCard_Init()
{
  ALOG_INF(PSTR(D_LOG_SDCARD "SDCard_Init"));

  if(SDCard_Mount())
  {
    ALOG_INF(PSTR(D_LOG_SDCARD "SD backend mounted and available"));
    return true;
  }

  if(!sdcard.pin_config_valid)
  {
    ALOG_INF(PSTR(D_LOG_SDCARD "SD backend waiting for valid pin configuration"));
  }
  else
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SD backend configured but not mounted, retry service active"));
  }

  return false;
}

/************************************************************************************************
 * FUNCTION: SDCard_Mount
 *
 * SUMMARY:
 * - Detects configured SD pins and selects SD_MMC, VSPI, or HSPI.
 *
 * CHANGED:
 * - 31May26: Created standalone SDCard mount detection.
 ************************************************************************************************/
bool mSDCard::SDCard_Mount()
{
  if(!sdcard.enabled)
  {
    return false;
  }

  if(sdcard.mounted && sdcard.fs)
  {
    return true;
  }

  sdcard.pin_config_valid = false;
  sdcard.mount_failed = false;
  sdcard.fs = nullptr;

  sdcard.using_mmc = false;
  sdcard.using_mmc_1bit = true;

  sdcard.pin_cs   = -1;
  sdcard.pin_sck  = -1;
  sdcard.pin_mosi = -1;
  sdcard.pin_miso = -1;

  sdcard.pin_mmc_clk = -1;
  sdcard.pin_mmc_cmd = -1;
  sdcard.pin_mmc_d0  = -1;
  sdcard.pin_mmc_d1  = -1;
  sdcard.pin_mmc_d2  = -1;
  sdcard.pin_mmc_d3  = -1;

  /************************************************************************************************
   * Prefer SD_MMC on ESP32 when MMC pins exist.
   ************************************************************************************************/
#if defined(ESP32) && defined(ENABLE_FEATURE_SDCARD__MMC)

  sdcard.pin_mmc_clk = tkr_pins->GetPin(GPIO_SDCARD_MMC_CLK);
  sdcard.pin_mmc_cmd = tkr_pins->GetPin(GPIO_SDCARD_MMC_CMD);
  sdcard.pin_mmc_d0  = tkr_pins->GetPin(GPIO_SDCARD_MMC_D, 0);
  sdcard.pin_mmc_d1  = tkr_pins->GetPin(GPIO_SDCARD_MMC_D, 1);
  sdcard.pin_mmc_d2  = tkr_pins->GetPin(GPIO_SDCARD_MMC_D, 2);
  sdcard.pin_mmc_d3  = tkr_pins->GetPin(GPIO_SDCARD_MMC_D, 3);

  if((sdcard.pin_mmc_clk >= 0) &&
     (sdcard.pin_mmc_cmd >= 0) &&
     (sdcard.pin_mmc_d0  >= 0))
  {
    sdcard.pin_config_valid = true;
    return SDCard_Mount_MMC();
  }

#endif

  /************************************************************************************************
   * VSPI SD mode.
   ************************************************************************************************/
#ifdef ESP32

  sdcard.pin_cs   = tkr_pins->GetPin(GPIO_SDCARD_VSPI_CSO);
  sdcard.pin_sck  = tkr_pins->GetPin(GPIO_SDCARD_VSPI_CLK);
  sdcard.pin_mosi = tkr_pins->GetPin(GPIO_SDCARD_VSPI_MOSI);
  sdcard.pin_miso = tkr_pins->GetPin(GPIO_SDCARD_VSPI_MISO);

  if((sdcard.pin_cs   >= 0) &&
     (sdcard.pin_sck  >= 0) &&
     (sdcard.pin_mosi >= 0) &&
     (sdcard.pin_miso >= 0))
  {
    sdcard.pin_config_valid = true;
    return SDCard_Mount_SPI_VSPI();
  }

#endif

  /************************************************************************************************
   * HSPI SD mode.
   ************************************************************************************************/

  sdcard.pin_cs   = tkr_pins->GetPin(GPIO_SDCARD_HSPI_CSO);
  sdcard.pin_sck  = tkr_pins->GetPin(GPIO_SDCARD_HSPI_CLK);
  sdcard.pin_mosi = tkr_pins->GetPin(GPIO_SDCARD_HSPI_MOSI);
  sdcard.pin_miso = tkr_pins->GetPin(GPIO_SDCARD_HSPI_MISO);

  if((sdcard.pin_cs   >= 0) &&
     (sdcard.pin_sck  >= 0) &&
     (sdcard.pin_mosi >= 0) &&
     (sdcard.pin_miso >= 0))
  {
    sdcard.pin_config_valid = true;
    return SDCard_Mount_SPI_HSPI();
  }

  /************************************************************************************************
   * No valid pins yet.
   *
   * Not a failure.
   * Runtime template/GPIO changes may configure pins later.
   ************************************************************************************************/

  sdcard.pin_config_missing_count++;

  sdcard.mounted = false;
  sdcard.mount_failed = false;
  sdcard.card_removed = false;
  sdcard.fs = nullptr;

  sdcard.card_type = CARD_NONE;
  sdcard.card_size_bytes = 0;
  sdcard.total_bytes = 0;
  sdcard.used_bytes = 0;

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_Mount: no valid SD pin set configured yet "
                       "mmc clk=%d cmd=%d d0=%d | hspi cs=%d sck=%d mosi=%d miso=%d"),
    sdcard.pin_mmc_clk,
    sdcard.pin_mmc_cmd,
    sdcard.pin_mmc_d0,
    sdcard.pin_cs,
    sdcard.pin_sck,
    sdcard.pin_mosi,
    sdcard.pin_miso
  );

  return false;
}

/************************************************************************************************
 * FUNCTION: SDCard_Mount_MMC
 *
 * SUMMARY:
 * - Mounts ESP32 SD_MMC backend.
 *
 * CHANGED:
 * - 31May26: Created standalone SD_MMC mount.
 ************************************************************************************************/
#if defined(ESP32) && defined(ENABLE_FEATURE_SDCARD__MMC)
bool mSDCard::SDCard_Mount_MMC()
{
  if(sdcard.mounted && sdcard.fs == &SD_MMC)
  {
    return true;
  }

  // Required before re-calling setPins after any previous partial begin.
  SD_MMC.end();
  delay(50);

  sdcard.mount_attempts++;
  sdcard.last_mount_ms = millis();

  const bool has_4bit =
    (sdcard.pin_mmc_d1 >= 0) &&
    (sdcard.pin_mmc_d2 >= 0) &&
    (sdcard.pin_mmc_d3 >= 0);

  sdcard.using_mmc = true;
  sdcard.using_mmc_1bit = !has_4bit;

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_Mount_MMC: clk=%d cmd=%d d0=%d d1=%d d2=%d d3=%d mode=%s"),
    sdcard.pin_mmc_clk,
    sdcard.pin_mmc_cmd,
    sdcard.pin_mmc_d0,
    sdcard.pin_mmc_d1,
    sdcard.pin_mmc_d2,
    sdcard.pin_mmc_d3,
    sdcard.using_mmc_1bit ? "1-bit" : "4-bit"
  );

  if(sdcard.using_mmc_1bit)
  {
    SD_MMC.setPins(sdcard.pin_mmc_clk, sdcard.pin_mmc_cmd, sdcard.pin_mmc_d0);
  }
  else
  {
    SD_MMC.setPins(
      sdcard.pin_mmc_clk,
      sdcard.pin_mmc_cmd,
      sdcard.pin_mmc_d0,
      sdcard.pin_mmc_d1,
      sdcard.pin_mmc_d2,
      sdcard.pin_mmc_d3
    );
  }

  if(!SD_MMC.begin("/sdcard", sdcard.using_mmc_1bit, false, SDMMC_FREQ_DEFAULT, 5))
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard_Mount_MMC: SD_MMC.begin failed"));

    SD_MMC.end();

    sdcard.fs = nullptr;
    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;
    sdcard.card_type = CARD_NONE;

    return false;
  }

  sdcard.card_type = SD_MMC.cardType();

  if(sdcard.card_type == CARD_NONE)
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard_Mount_MMC: CARD_NONE after begin"));

    SD_MMC.end();

    sdcard.fs = nullptr;
    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;

    return false;
  }

  sdcard.fs = &SD_MMC;
  sdcard.mounted = true;
  sdcard.mount_failed = false;
  sdcard.card_removed = false;
  sdcard.health_check_failures = 0;
  sdcard.last_health_check_ms = millis();

  SDCard_RefreshInfo();

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_Mount_MMC: mounted type=%s size=%lluMB total=%lluMB used=%lluMB"),
    SDCard_CardTypeName(sdcard.card_type),
    sdcard.card_size_bytes / (1024ULL * 1024ULL),
    sdcard.total_bytes / (1024ULL * 1024ULL),
    sdcard.used_bytes / (1024ULL * 1024ULL)
  );

  return true;
}
#endif

/************************************************************************************************
 * FUNCTION: SDCard_Mount_SPI_HSPI
 *
 * SUMMARY:
 * - Mounts SD card through SPI HSPI.
 *
 * CHANGED:
 * - 31May26: Created standalone HSPI SD mount.
 ************************************************************************************************/
bool mSDCard::SDCard_Mount_SPI_HSPI()
{
  sdcard.mount_attempts++;
  sdcard.last_mount_ms = millis();

  sdcard.using_mmc = false;
  sdcard.using_mmc_1bit = false;

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_HSPI: sck=%d miso=%d mosi=%d cs=%d"),
    sdcard.pin_sck,
    sdcard.pin_miso,
    sdcard.pin_mosi,
    sdcard.pin_cs
  );

#ifdef ESP32
  spiSD_HSPI.begin(sdcard.pin_sck, sdcard.pin_miso, sdcard.pin_mosi, sdcard.pin_cs);

  if(!SD.begin(sdcard.pin_cs, spiSD_HSPI))
#else
  spiSD.begin(sdcard.pin_sck, sdcard.pin_miso, sdcard.pin_mosi, sdcard.pin_cs);

  if(!SD.begin(sdcard.pin_cs, spiSD))
#endif
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_HSPI: SD.begin failed"));

    sdcard.fs = nullptr;
    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;
    sdcard.card_type = CARD_NONE;

    return false;
  }

  sdcard.fs = &SD;
  sdcard.card_type = SD.cardType();

  if(sdcard.card_type == CARD_NONE)
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_HSPI: CARD_NONE after begin"));

    SD.end();

    sdcard.fs = nullptr;
    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;

    return false;
  }

  sdcard.mounted = true;
  sdcard.mount_failed = false;
  sdcard.card_removed = false;
  sdcard.health_check_failures = 0;
  sdcard.last_health_check_ms = millis();

  SDCard_RefreshInfo();

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_HSPI: mounted type=%s size=%lluMB total=%lluMB used=%lluMB"),
    SDCard_CardTypeName(sdcard.card_type),
    sdcard.card_size_bytes / (1024ULL * 1024ULL),
    sdcard.total_bytes / (1024ULL * 1024ULL),
    sdcard.used_bytes / (1024ULL * 1024ULL)
  );

  return true;
}

/************************************************************************************************
 * FUNCTION: SDCard_Mount_SPI_VSPI
 *
 * SUMMARY:
 * - Mounts SD card through SPI VSPI.
 *
 * CHANGED:
 * - 31May26: Created standalone VSPI SD mount.
 ************************************************************************************************/
bool mSDCard::SDCard_Mount_SPI_VSPI()
{
#ifndef ESP32
  return false;
#else

  sdcard.mount_attempts++;
  sdcard.last_mount_ms = millis();

  sdcard.using_mmc = false;
  sdcard.using_mmc_1bit = false;

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_VSPI: sck=%d miso=%d mosi=%d cs=%d"),
    sdcard.pin_sck,
    sdcard.pin_miso,
    sdcard.pin_mosi,
    sdcard.pin_cs
  );

  spiSD_VSPI.begin(sdcard.pin_sck, sdcard.pin_miso, sdcard.pin_mosi, sdcard.pin_cs);

  if(!SD.begin(sdcard.pin_cs, spiSD_VSPI))
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_VSPI: SD.begin failed"));

    sdcard.fs = nullptr;
    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;
    sdcard.card_type = CARD_NONE;

    return false;
  }

  sdcard.fs = &SD;
  sdcard.card_type = SD.cardType();

  if(sdcard.card_type == CARD_NONE)
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_VSPI: CARD_NONE after begin"));

    SD.end();

    sdcard.fs = nullptr;
    sdcard.mounted = false;
    sdcard.mount_failed = true;
    sdcard.mount_failures++;

    return false;
  }

  sdcard.mounted = true;
  sdcard.mount_failed = false;
  sdcard.card_removed = false;
  sdcard.health_check_failures = 0;
  sdcard.last_health_check_ms = millis();

  SDCard_RefreshInfo();

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_Mount_SPI_VSPI: mounted type=%s size=%lluMB total=%lluMB used=%lluMB"),
    SDCard_CardTypeName(sdcard.card_type),
    sdcard.card_size_bytes / (1024ULL * 1024ULL),
    sdcard.total_bytes / (1024ULL * 1024ULL),
    sdcard.used_bytes / (1024ULL * 1024ULL)
  );

  return true;

#endif
}

bool mSDCard::SDCard_IsMounted() const
{
  return sdcard.mounted && sdcard.fs;
}

void mSDCard::SDCard_Service()
{
  if(!sdcard.enabled)
  {
    return;
  }

  if(sdcard.mounted)
  {
    SDCard_HealthCheck();
    return;
  }

  SDCard_ServiceMountRetry();
}

void mSDCard::SDCard_ServiceMountRetry()
{
  if(!sdcard.enabled || sdcard.mounted)
  {
    return;
  }

  if((millis() - sdcard.last_mount_ms) < FILESYSTEM_SDCARD_MOUNT_RETRY_PERIOD_MS)
  {
    return;
  }

  ALOG_INF(
    PSTR(D_LOG_SDCARD "SDCard_ServiceMountRetry: attempt=%u failures=%u pin_config_valid=%u"),
    (unsigned)(sdcard.mount_attempts + 1),
    (unsigned)sdcard.mount_failures,
    (unsigned)sdcard.pin_config_valid
  );

  sdcard.last_mount_ms = millis();

  SDCard_Mount();
}

void mSDCard::SDCard_MarkUnmounted()
{
  sdcard.mounted = false;
  sdcard.mount_failed = true;
  sdcard.card_removed = true;
  sdcard.fs = nullptr;

  sdcard.card_type = CARD_NONE;
  sdcard.card_size_bytes = 0;
  sdcard.total_bytes = 0;
  sdcard.used_bytes = 0;
}

bool mSDCard::SDCard_Unmount()
{
  if(!sdcard.mounted)
  {
    return true;
  }

#if defined(ESP32) && defined(ENABLE_FEATURE_SDCARD__MMC)
  if(sdcard.using_mmc)
  {
    SD_MMC.end();
  }
  else
#endif
  {
    SD.end();
  }

  SDCard_MarkUnmounted();

  ALOG_INF(PSTR(D_LOG_SDCARD "SDCard_Unmount: complete"));

  return true;
}


/************************************************************************************************
 * FUNCTION: SDCard_CardTypeName
 *
 * SUMMARY:
 * - Converts Arduino SD card type ID to readable text.
 *
 * CHANGED:
 * - 31May26: Added standalone mSDCard implementation.
 ************************************************************************************************/
const char* mSDCard::SDCard_CardTypeName(uint8_t card_type) const
{
  switch(card_type)
  {
    case CARD_MMC:  return "MMC";
    case CARD_SD:   return "SDSC";
    case CARD_SDHC: return "SDHC";
    default:        return "NONE";
  }
}


/************************************************************************************************
 * FUNCTION: SDCard_RefreshInfo
 *
 * SUMMARY:
 * - Refreshes cached card size, total, used, and type data from the active backend.
 *
 * CHANGED:
 * - 31May26: Added backend-aware standalone mSDCard implementation.
 ************************************************************************************************/
bool mSDCard::SDCard_RefreshInfo()
{
  if(!sdcard.mounted || !sdcard.fs)
  {
    return false;
  }

#if defined(ESP32) && defined(ENABLE_FEATURE_SDCARD__MMC)
  if(sdcard.using_mmc)
  {
    sdcard.card_type = SD_MMC.cardType();

    if(sdcard.card_type == CARD_NONE)
    {
      SDCard_MarkUnmounted();
      return false;
    }

    sdcard.card_size_bytes = SD_MMC.cardSize();
    sdcard.total_bytes     = SD_MMC.totalBytes();
    sdcard.used_bytes      = SD_MMC.usedBytes();

    return true;
  }
#endif

  sdcard.card_type = SD.cardType();

  if(sdcard.card_type == CARD_NONE)
  {
    SDCard_MarkUnmounted();
    return false;
  }

  sdcard.card_size_bytes = SD.cardSize();
  sdcard.total_bytes     = SD.totalBytes();
  sdcard.used_bytes      = SD.usedBytes();

  return true;
}


/************************************************************************************************
 * FUNCTION: SDCard_HealthCheck
 *
 * SUMMARY:
 * - Checks whether a mounted SD card is still present and responsive.
 *
 * CHANGED:
 * - 31May26: Added backend-aware standalone mSDCard implementation.
 ************************************************************************************************/
bool mSDCard::SDCard_HealthCheck()
{
  if(!sdcard.enabled || !sdcard.mounted || !sdcard.fs)
  {
    return false;
  }

  if((millis() - sdcard.last_health_check_ms) < FILESYSTEM_SDCARD_HEALTH_CHECK_PERIOD_MS)
  {
    return true;
  }

  sdcard.last_health_check_ms = millis();

  uint8_t card_type = CARD_NONE;

#if defined(ESP32) && defined(ENABLE_FEATURE_SDCARD__MMC)
  if(sdcard.using_mmc)
  {
    card_type = SD_MMC.cardType();
  }
  else
#endif
  {
    card_type = SD.cardType();
  }

  if(card_type == CARD_NONE)
  {
    sdcard.health_check_failures++;

    ALOG_WRN(
      PSTR(D_LOG_SDCARD "SDCard_HealthCheck: CARD_NONE failure=%u"),
      (unsigned)sdcard.health_check_failures
    );

    if(sdcard.health_check_failures >= FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT)
    {
      SDCard_MarkUnmounted();
      return false;
    }

    return true;
  }

  File root = sdcard.fs->open("/");

  if(!root)
  {
    sdcard.health_check_failures++;

    ALOG_WRN(
      PSTR(D_LOG_SDCARD "SDCard_HealthCheck: root open failed failure=%u"),
      (unsigned)sdcard.health_check_failures
    );

    if(sdcard.health_check_failures >= FILESYSTEM_SDCARD_HEALTH_CHECK_FAIL_LIMIT)
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
 * SECTION: SD CARD LOCAL FILE IO
 ************************************************************************************************/

File mSDCard::Open(const char* path, const char* mode)
{
  if(!path || !mode)
  {
    return File();
  }

  if(!SDCard_IsMounted())
  {
    if(!SDCard_Mount())
    {
      ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard Open: mount failed path=%s"), path);
      return File();
    }
  }

  if(!sdcard.fs)
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard Open: no active FS path=%s"), path);
    return File();
  }

  return sdcard.fs->open(path, mode);
}

bool mSDCard::Exists(const char* path)
{
  if(!path || !SDCard_IsMounted() || !sdcard.fs)
  {
    return false;
  }

  return sdcard.fs->exists(path);
}

bool mSDCard::Remove(const char* path)
{
  if(!path || !SDCard_IsMounted() || !sdcard.fs)
  {
    return false;
  }

  return sdcard.fs->remove(path);
}

bool mSDCard::Rename(const char* from_path, const char* to_path)
{
  if(!from_path || !to_path || !SDCard_IsMounted() || !sdcard.fs)
  {
    return false;
  }

  return sdcard.fs->rename(from_path, to_path);
}

bool mSDCard::Mkdir(const char* path)
{
  if(!path || !SDCard_IsMounted() || !sdcard.fs)
  {
    return false;
  }

  return sdcard.fs->mkdir(path);
}

bool mSDCard::Rmdir(const char* path)
{
  if(!path || !SDCard_IsMounted() || !sdcard.fs)
  {
    return false;
  }

  return sdcard.fs->rmdir(path);
}

void mSDCard::ListDir(const char* path, uint8_t levels)
{
  if(!path)
  {
    path = "/";
  }

  File root = Open(path, FILE_READ);

  if(!root)
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard ListDir: open failed path=%s"), path);
    return;
  }

  if(!root.isDirectory())
  {
    ALOG_WRN(PSTR(D_LOG_SDCARD "SDCard ListDir: not directory path=%s"), path);
    root.close();
    return;
  }

  File file = root.openNextFile();

  while(file)
  {
    if(file.isDirectory())
    {
      ALOG_INF(PSTR(D_LOG_SDCARD "SD DIR : %s"), file.name());

      if(levels)
      {
        ListDir(file.name(), levels - 1);
      }
    }
    else
    {
      ALOG_INF(PSTR(D_LOG_SDCARD "SD FILE: %s SIZE: %u"), file.name(), (unsigned)file.size());
    }

    file = root.openNextFile();
  }

  root.close();
}

bool mSDCard::ReadToSerial(const char* path)
{
  File file = Open(path, FILE_READ);

  if(!file)
  {
    return false;
  }

  while(file.available())
  {
    Serial.write(file.read());
  }

  file.close();
  return true;
}

bool mSDCard::WriteAll(const char* path, const uint8_t* data, size_t len, const char* mode)
{
  if(!path || !data || !mode)
  {
    return false;
  }

  File file = Open(path, mode);

  if(!file)
  {
    return false;
  }

  const size_t written = file.write(data, len);
  file.close();

  return written == len;
}

bool mSDCard::WriteText(const char* path, const char* text, const char* mode)
{
  if(!text)
  {
    return false;
  }

  return WriteAll(path, (const uint8_t*)text, strlen(text), mode);
}

bool mSDCard::AppendText(const char* path, const char* text)
{
  return WriteText(path, text, FILE_APPEND);
}

void mSDCard::TestFileIO(const char* path)
{
  static const char test_data[] = "PulSar SDCard Test\r\n";

  WriteAll(path, (const uint8_t*)test_data, sizeof(test_data) - 1, FILE_WRITE);
  ReadToSerial(path);
}

bool mSDCard::SaveFile(const char* path, const uint8_t* data, size_t len)
{
  return WriteAll(path, data, len, FILE_WRITE);
}

bool mSDCard::AppendFile(const char* path, const uint8_t* data, size_t len)
{
  return WriteAll(path, data, len, FILE_APPEND);
}

size_t mSDCard::FileSize(const char* path)
{
  File file = Open(path, FILE_READ);

  if(!file)
  {
    return 0;
  }

  const size_t size = file.size();
  file.close();

  return size;
}


void mSDCard::parse_JSONCommand(JsonParserObject obj)
{
  if(!obj)
  {
    return;
  }

  JsonParserToken jtok = 0;

  if(jtok = obj["SDCard"])
  {
    const char* cmd = jtok.getStr();

    if(cmd && strcmp(cmd, "mount") == 0)
    {
      SDCard_Mount();
    }
    else if(cmd && strcmp(cmd, "unmount") == 0)
    {
      SDCard_Unmount();
    }
  }
}

uint8_t mSDCard::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  JBI->Add("Enabled", sdcard.enabled);
  JBI->Add("PinConfigValid", sdcard.pin_config_valid);
  JBI->Add("UsingMMC", sdcard.using_mmc);
  JBI->Add("UsingMMC1Bit", sdcard.using_mmc_1bit);

  return JBI->End();
}

uint8_t mSDCard::ConstructJSON_State(uint8_t json_level, bool json_appending)
{
  JBI->Start();

  JBI->Add("Mounted", sdcard.mounted);
  JBI->Add("MountFailed", sdcard.mount_failed);
  JBI->Add("CardRemoved", sdcard.card_removed);
  JBI->Add("CardType", SDCard_CardTypeName(sdcard.card_type));
  JBI->Add("SizeMB", (uint32_t)(sdcard.card_size_bytes / (1024ULL * 1024ULL)));
  JBI->Add("TotalMB", (uint32_t)(sdcard.total_bytes / (1024ULL * 1024ULL)));
  JBI->Add("UsedMB", (uint32_t)(sdcard.used_bytes / (1024ULL * 1024ULL)));
  JBI->Add("MountAttempts", sdcard.mount_attempts);
  JBI->Add("MountFailures", sdcard.mount_failures);

  return JBI->End();
}

#ifdef USE_MODULE_NETWORK_MQTT
void mSDCard::MQTTHandler_Init()
{
  struct handler<mSDCard>* ptr;

  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSDCard::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mSDCard::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_IFCHANGED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_STATE_CTR;
  ptr->ConstructJSON_function = &mSDCard::ConstructJSON_State;
  mqtthandler_list.push_back(ptr);
}
#endif


#endif
