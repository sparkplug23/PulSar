
#include "2_CoreSystem/01_Settings/mSettings.h"

#include "2_CoreSystem/06_Support/mSupport.h"

#ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE_ESP_SUPPORT
/*
  support_esp.ino - ESP specific code for Tasmota

  Copyright (C) 2021  Theo Arends / Jörg Schüler-Maroldt

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*********************************************************************************************\
 * ESP8266 and ESP32 specific code
 *
 * At the end the common Tasmota calls are provided
\*********************************************************************************************/

#ifdef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023

#ifdef ESP32

// dummy defines
#define EEPROM_LOCATION (SPI_FLASH_SEC_SIZE * 200)
uint32_t SETTINGS_LOCATION = EEPROM_LOCATION;

#endif  // ESP32

const uint8_t CONFIG_FILE_XOR = 0x5A;       // Configuration file xor (0 = No Xor)
uint32_t settings_location = EEPROM_LOCATION;
uint32_t settings_crc32 = 0;
uint32_t settings_size = 0;
uint8_t *settings_buffer = nullptr;
uint8_t config_xor_on_set = CONFIG_FILE_XOR;

#endif // ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023



/*********************************************************************************************\
 * ESP8266 Support
\*********************************************************************************************/

#ifdef ESP8266

extern "C" {
extern struct rst_info resetInfo;
}

/*********************************************************************************************\
 * Core overrides executed by core
\*********************************************************************************************/

// Add below line to tasmota_globals.h
// extern "C" void resetPins();
//
// This function overrules __resetPins() which is executed by core init() as initPins() in core_esp8266_wiring.cpp
//
// 20221229 - (v12.3.1.2) Enabled with additional check to execute on power on only to fix relay clicks on power on
// 20200321 - (v8.2.0.1) Disable core functionality to fix relay clicks on restart after OTA - make function return without setting pinMode
void resetPins() {
  if ((resetInfo.reason == REASON_DEFAULT_RST) || (resetInfo.reason == REASON_EXT_SYS_RST)) {
    // Only perform at power on
    for (int i = 0; i <= 5; ++i) {
      pinMode(i, INPUT);
    }
    // pins 6-11 are used for the SPI flash interface ESP8266
    for (int i = 12; i <= 16; ++i) {
      pinMode(i, INPUT);
    }
  }
}

/*********************************************************************************************\
 * Hardware related
\*********************************************************************************************/

void HwWdtDisable(void) {
  *((volatile uint32_t*) 0x60000900) &= ~(1);  // Hardware WDT OFF
}

void HwWdtEnable(void) {
  *((volatile uint32_t*) 0x60000900) |= 1;     // Hardware WDT ON
}

void WdtDisable(void) {
  ESP.wdtDisable();
  HwWdtDisable();
}

void WdtEnable(void) {
  HwWdtEnable();
  ESP.wdtEnable(0);
}

/*********************************************************************************************\
 * ESP8266 specifics
\*********************************************************************************************/

uint32_t ESP_ResetInfoReason(void) {
  return resetInfo.reason;
}

String ESP_getResetReason(void) {
  return ESP.getResetReason();
}

uint32_t ESP_getChipId(void) {
  return ESP.getChipId();
}

uint32_t ESP_getFreeSketchSpace(void) {
  return ESP.getFreeSketchSpace();
}

uint32_t ESP_getSketchSize(void) {
  return ESP.getSketchSize();
}

uint32_t ESP_getFreeHeap(void) {
  return ESP.getFreeHeap();
}

uint32_t ESP_getFlashChipId(void) {
  return ESP.getFlashChipId();
}

uint32_t ESP_getFlashChipRealSize(void) {
  return ESP.getFlashChipRealSize();
}

uint32_t ESP_getFlashChipSize(void) {
  return ESP.getFlashChipSize();
}

void ESP_Restart(void) {
//  ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  ESP.reset();
}

uint32_t FlashWriteStartSector(void) {
  return (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 2;  // Stay on the safe side
}

uint32_t FlashWriteMaxSector(void) {
  return (((uint32_t)&_FS_start - 0x40200000) / SPI_FLASH_SEC_SIZE) - 2;
}

uint8_t* FlashDirectAccess(void) {
  return (uint8_t*)(0x40200000 + (FlashWriteStartSector() * SPI_FLASH_SEC_SIZE));
}

void *special_malloc(uint32_t size) {
  return malloc(size);
}

void *special_realloc(void *ptr, size_t size) {
  return realloc(ptr, size);
}

void *special_calloc(size_t num, size_t size) {
  return calloc(num, size);
}

String GetDeviceHardware(void) {
  /*
  ESP8266 SoCs
  - 32-bit MCU & 2.4 GHz Wi-Fi
  - High-performance 160 MHz single-core CPU
  - +19.5 dBm output power ensures a good physical range
  - Sleep current is less than 20 μA, making it suitable for battery-powered and wearable-electronics applications
  - Peripherals include UART, GPIO, I2C, I2S, SDIO, PWM, ADC and SPI
  */
  // esptool.py get_efuses
  uint32_t efuse0 = *(uint32_t*)(0x3FF00050);
//  uint32_t efuse1 = *(uint32_t*)(0x3FF00054);
  uint32_t efuse2 = *(uint32_t*)(0x3FF00058);
  uint32_t efuse3 = *(uint32_t*)(0x3FF0005C);

  bool r0_4 = efuse0 & (1 << 4);                   // ESP8285
  bool r2_16 = efuse2 & (1 << 16);                 // ESP8285
  if (r0_4 || r2_16) {                             // ESP8285
    //                                                              1M 2M 2M 4M flash size
    //   r0_4                                                       1  1  0  0
    bool r3_25 = efuse3 & (1 << 25);               // flash matrix  0  0  1  1
    bool r3_26 = efuse3 & (1 << 26);               // flash matrix  0  1  0  1
    bool r3_27 = efuse3 & (1 << 27);               // flash matrix  0  0  0  0
    uint32_t pkg_version = 0;
    if (!r3_27) {
      if (r0_4 && !r3_25) {
        pkg_version = (r3_26) ? 2 : 1;
      }
      else if (!r0_4 && r3_25) {
        pkg_version = (r3_26) ? 4 : 2;
      }
    }
    bool max_temp = efuse0 & (1 << 5);             // Max flash temperature (0 = 85C, 1 = 105C)
    switch (pkg_version) {
      case 1:
        if (max_temp) { return F("ESP8285H08"); }  // 1M flash
        else {          return F("ESP8285N08"); }
      case 2:
        if (max_temp) { return F("ESP8285H16"); }  // 2M flash
        else {          return F("ESP8285N16"); }
      case 4:
        if (max_temp) { return F("ESP8285H32"); }  // 4M flash
        else {          return F("ESP8285N32"); }
    }
    return F("ESP8285");
  }
  return F("ESP8266EX");
}

String GetDeviceHardwareRevision(void) {
  // No known revisions for ESP8266/85
  return GetDeviceHardware();
}

String GetCodeCores(void) {
  return F("");
}

#endif

/*********************************************************************************************\
 * ESP32 Support
\*********************************************************************************************/

#ifdef ESP32

#include "soc/soc.h"
#include "soc/spi_reg.h"
// ESP32_ARCH contains the name of the architecture (used by autoconf)
#if CONFIG_IDF_TARGET_ESP32
  #ifdef CORE32SOLO1
    #define ESP32_ARCH            "esp32solo1"
  #else
    #define ESP32_ARCH            "esp32"
  #endif
#elif CONFIG_IDF_TARGET_ESP32S2
  #define ESP32_ARCH              "esp32s2"
#elif CONFIG_IDF_TARGET_ESP32S3
  #define ESP32_ARCH              "esp32s3"
#elif CONFIG_IDF_TARGET_ESP32C2
  #define ESP32_ARCH              "esp32c2"
#elif CONFIG_IDF_TARGET_ESP32C3
  #define ESP32_ARCH              "esp32c3"
#elif CONFIG_IDF_TARGET_ESP32C6
  #define ESP32_ARCH              "esp32c6"
#else
  #define ESP32_ARCH              ""
#endif

// Handle 20k of NVM

#include <nvs.h>

// See libraries\ESP32\examples\ResetReason.ino
#if ESP_IDF_VERSION_MAJOR > 3      // IDF 4+
  #include "esp_chip_info.h"
  #if CONFIG_IDF_TARGET_ESP32      // ESP32/PICO-D4
    #include "esp32/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32S2  // ESP32-S2
    #include "esp32s2/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32S3  // ESP32-S3
    #include "esp32s3/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32C2  // ESP32-C2
    #include "esp32c2/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32C3  // ESP32-C3
    #include "esp32c3/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32C6  // ESP32-C6
    #include "esp32c6/rom/rtc.h"
  #else
    #error Target CONFIG_IDF_TARGET is not supported
  #endif
#else // ESP32 Before IDF 4.0
  #include "rom/rtc.h"
#endif

#if ESP_IDF_VERSION_MAJOR >= 5
  #include "esp_chip_info.h"
#endif

// Set the Stacksize for Arduino core. Default is 8192, some builds may need a bigger one
size_t getArduinoLoopTaskStackSize(void) {
    return SET_ESP32_STACK_SIZE;
}


#include <esp_phy_init.h>

bool NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen) {
  nvs_handle_t handle;
  esp_err_t result = nvs_open(sNvsName, NVS_READONLY, &handle);
  if (result != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NVS: Error %d"), result);
    return false;
  }
  size_t size = nSettingsLen;
  nvs_get_blob(handle, sName, pSettings, &size);
  nvs_close(handle);
  return true;
}

void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen) {
#ifdef USE_WEBCAM
  WcInterrupt(0);  // Stop stream if active to fix TG1WDT_SYS_RESET
#endif
  nvs_handle_t handle;
  esp_err_t result = nvs_open(sNvsName, NVS_READWRITE, &handle);
  if (result != ESP_OK) {
    AddLog(LOG_LEVEL_DEBUG, PSTR("NVS: Error %d"), result);
  } else {
    nvs_set_blob(handle, sName, pSettings, nSettingsLen);
    nvs_commit(handle);
    nvs_close(handle);
  }
#ifdef USE_WEBCAM
  WcInterrupt(1);
#endif
}

int32_t NvmErase(const char *sNvsName) {
  nvs_handle_t handle;
  int32_t result = nvs_open(sNvsName, NVS_READWRITE, &handle);
  if (ESP_OK == result) { result = nvs_erase_all(handle); }
  if (ESP_OK == result) { result = nvs_commit(handle); }
  nvs_close(handle);
  return result;
}

void SettingsErase(uint8_t type) {
  // SDK and Tasmota data is held in default NVS partition
  // Tasmota data is held also in file /.settings on default filesystem
  // cal_data - SDK PHY calibration data as documented in esp_phy_init.h
  // qpc      - Tasmota Quick Power Cycle state
  // main     - Tasmota Settings data
  int32_t r1, r2, r3 = 0;
  switch (type) {
    case 0:               // Reset 2 = Erase all flash from program end to end of physical flash
    case 2:               // Reset 5, 6 = Erase all flash from program end to end of physical flash excluding filesystem
//      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
#ifdef USE_UFILESYS
      r3 = TfsDeleteFile(TASM_FILE_SETTINGS);
#endif
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d,%d)"), r1, r2, r3);
      break;
    case 1:               // Reset 3 = SDK parameter area
    case 4:               // WIFI_FORCE_RF_CAL_ERASE = SDK parameter area
      r1 = esp_phy_erase_cal_data_in_nvs();
//      r1 = NvmErase("cal_data");
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " PHY data (%d)"), r1);
      break;
    case 3:               // QPC Reached = QPC, Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
//      nvs_flash_erase();  // Erase RTC, PHY, sta.mac, ap.sndchan, ap.mac, Tasmota etc.
      r1 = NvmErase("qpc");
      r2 = NvmErase("main");
//      r3 = esp_phy_erase_cal_data_in_nvs();
//      r3 = NvmErase("cal_data");
//      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota (%d,%d) and PHY data (%d)"), r1, r2, r3);
#ifdef USE_UFILESYS
      r3 = TfsDeleteFile(TASM_FILE_SETTINGS);
#endif
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " Tasmota data (%d,%d,%d)"), r1, r2, r3);
      break;
  }
}

uint32_t SettingsRead(void *data, size_t size) {
#ifdef USE_UFILESYS
  if (TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)data, size)) {
    return 2;
  }
#endif
  if (NvmLoad("main", "Settings", data, size)) {
    return 1;
  };
  return 0;
}

void SettingsWrite(const void *pSettings, unsigned nSettingsLen) {
#ifdef USE_UFILESYS
  TfsSaveFile(TASM_FILE_SETTINGS, (const uint8_t*)pSettings, nSettingsLen);
#endif
  NvmSave("main", "Settings", pSettings, nSettingsLen);
}

void QPCRead(void *pSettings, unsigned nSettingsLen) {
  NvmLoad("qpc", "pcreg", pSettings, nSettingsLen);
}

void QPCWrite(const void *pSettings, unsigned nSettingsLen) {
  NvmSave("qpc", "pcreg", pSettings, nSettingsLen);
}

bool OtaFactoryRead(void) {
  uint32_t pOtaLoader;
  NvmLoad("otal", "otal", &pOtaLoader, sizeof(pOtaLoader));
  return pOtaLoader;
}

void OtaFactoryWrite(bool value) {
  uint32_t pOtaLoader = value;
  NvmSave("otal", "otal", &pOtaLoader, sizeof(pOtaLoader));
}

void NvsInfo(void) {
  nvs_stats_t nvs_stats;
  nvs_get_stats(NULL, &nvs_stats);
  AddLog(LOG_LEVEL_INFO, PSTR("NVS: Used %d/%d entries, NameSpaces %d"),
    nvs_stats.used_entries, nvs_stats.total_entries, nvs_stats.namespace_count);
}

//
// Flash memory mapping
//

// See Esp.cpp
#include "Esp.h"
#if ESP_IDF_VERSION_MAJOR >= 5
  // esp_spi_flash.h is deprecated, please use spi_flash_mmap.h instead
  #include "spi_flash_mmap.h"
#else
  #include "esp_spi_flash.h"
#endif
#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
extern "C" {
#include "esp_ota_ops.h"
#include "esp_image_format.h"
}
#include "esp_system.h"
// #include "esp_flash.h" // NEEDS FIXED
#if ESP_IDF_VERSION_MAJOR > 3       // IDF 4+
  #if CONFIG_IDF_TARGET_ESP32       // ESP32/PICO-D4
    #include "esp32/rom/spi_flash.h"
    #define ESP_FLASH_IMAGE_BASE 0x1000     // Flash offset containing magic flash size and spi mode
  #elif CONFIG_IDF_TARGET_ESP32S2   // ESP32-S2
    #include "esp32s2/rom/spi_flash.h"
    #define ESP_FLASH_IMAGE_BASE 0x1000     // Flash offset containing magic flash size and spi mode
  #elif CONFIG_IDF_TARGET_ESP32S3   // ESP32-S3
    #include "esp32s3/rom/spi_flash.h"
    #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32s3 is located at 0x0000
  #elif CONFIG_IDF_TARGET_ESP32C2   // ESP32-C2
    #include "esp32c2/rom/spi_flash.h"
    #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32c2 is located at 0x0000
  #elif CONFIG_IDF_TARGET_ESP32C3   // ESP32-C3
    #include "esp32c3/rom/spi_flash.h"
    #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32c3 is located at 0x0000
  #elif CONFIG_IDF_TARGET_ESP32C6   // ESP32-C6
    #include "esp32c6/rom/spi_flash.h"
    #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32c6 is located at 0x0000
  #else
    #error Target CONFIG_IDF_TARGET is not supported
  #endif
#else // ESP32 Before IDF 4.0
  #include "rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x1000
#endif
#if ESP_IDF_VERSION_MAJOR >= 5
  #include "bootloader_common.h"
#endif

uint32_t EspProgramSize(const char *label) {
  const esp_partition_t *part = esp_partition_find_first(ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_ANY, label);
  if (!part) {
    return 0;
  }
  const esp_partition_pos_t part_pos  = {
    .offset = part->address,
    .size = part->size,
  };
  esp_image_metadata_t data;
  data.start_addr = part_pos.offset;
  esp_image_verify(ESP_IMAGE_VERIFY, &part_pos, &data);
  return data.image_len;
}

bool EspSingleOtaPartition(void) {
  return (1 == esp_ota_get_app_partition_count());
}

uint32_t EspRunningFactoryPartition(void) {
  const esp_partition_t *cur_part = esp_ota_get_running_partition();
//  return (cur_part->type == 0 && cur_part->subtype == 0);
  if (cur_part->type == 0 && cur_part->subtype == 0) {
    return cur_part->size;
  }
  return 0;
}

void EspPrepRestartToSafeBoot(void) {
  const esp_partition_t *otadata_partition = esp_partition_find_first(ESP_PARTITION_TYPE_DATA, ESP_PARTITION_SUBTYPE_DATA_OTA, NULL);
  if (otadata_partition) {
    esp_partition_erase_range(otadata_partition, 0, SPI_FLASH_SEC_SIZE * 2);
  }
}

bool EspPrepSwitchPartition(uint32_t state) {
  bool valid = EspSingleOtaPartition();
  if (valid) {
    bool running_factory = EspRunningFactoryPartition();
    switch (state) {
      case 0:  // Off = safeboot
        if (!running_factory) {
          EspPrepRestartToSafeBoot();
        } else {
          valid = false;
        }
        break;
      case 1:  // On = ota0
        if (running_factory) {
          const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
          esp_ota_set_boot_partition(partition);
        } else {
          valid = false;
        }
        break;
      case 2:  // Toggle
        if (!running_factory) {
          EspPrepRestartToSafeBoot();
        } else {
          const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
          esp_ota_set_boot_partition(partition);
        }
    }
  }
  return valid;
}

uint32_t EspFlashBaseAddress(void) {
  if (EspSingleOtaPartition()) {       // Only one partition so start at end of sketch
    const esp_partition_t *running = esp_ota_get_running_partition();
    if (!running) { return 0; }
    return running->address + ESP_getSketchSize();
  } else {                     // Select other partition
    const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
    if (!partition) { return 0; }
    return partition->address;  // For tasmota 0x00010000 or 0x00200000
  }
}

uint32_t EspFlashBaseEndAddress(void) {
  const esp_partition_t* partition = (EspSingleOtaPartition()) ? esp_ota_get_running_partition() : esp_ota_get_next_update_partition(nullptr);
  if (!partition) { return 0; }
  return partition->address + partition->size;  // For tasmota 0x00200000 or 0x003F0000
}

uint8_t* EspFlashMmap(uint32_t address) {
  static spi_flash_mmap_handle_t handle = 0;

  if (handle) {
    spi_flash_munmap(handle);
    handle = 0;
  }

  const uint8_t* data;
  int32_t err = spi_flash_mmap(address, 5 * SPI_FLASH_MMU_PAGE_SIZE, SPI_FLASH_MMAP_DATA, (const void **)&data, &handle);

/*
  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Spi_flash_map %d"), err);

  spi_flash_mmap_dump();
*/
  return (uint8_t*)data;
}

/*
int32_t EspPartitionMmap(uint32_t action) {
  static spi_flash_mmap_handle_t handle;

  int32_t err = 0;
  if (1 == action) {
    const esp_partition_t *partition = esp_ota_get_running_partition();
//    const esp_partition_t* partition = esp_ota_get_next_update_partition(nullptr);
    if (!partition) { return 0; }
    err = esp_partition_mmap(partition, 0, 4 * SPI_FLASH_MMU_PAGE_SIZE, SPI_FLASH_MMAP_DATA, (const void **)&TasmotaGlobal_mmap_data, &handle);

    AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Partition start 0x%08X, Partition end 0x%08X, Mmap data 0x%08X"), partition->address, partition->size, TasmotaGlobal_mmap_data);

  } else {
    spi_flash_munmap(handle);
    handle = 0;
  }
  return err;
}

*/

//
// ESP32 specific
//

#ifdef CONFIG_IDF_TARGET_ESP32
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

void DisableBrownout(void) {
  // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
}
#endif  // ESP32

//
// ESP32 Alternatives
//

String ESP32GetResetReason(uint32_t cpu_no) {
	// tools\sdk\include\esp32\rom\rtc.h
	// tools\sdk\esp32\include\esp_rom\include\esp32c3\rom\rtc.h
	// tools\sdk\esp32\include\esp_rom\include\esp32s2\rom\rtc.h
  switch (rtc_get_reset_reason(cpu_no)) {                                   //     ESP32             ESP32-S / ESP32-C
    case 1  : return F("Vbat power on reset");                              // 1   POWERON_RESET     POWERON_RESET
    case 3  : return F("Software reset digital core");                      // 3   SW_RESET          RTC_SW_SYS_RESET
    case 4  : return F("Legacy watch dog reset digital core");              // 4   OWDT_RESET        -
    case 5  : return F("Deep Sleep reset digital core");                    // 5   DEEPSLEEP_RESET   DEEPSLEEP_RESET
    case 6  : return F("Reset by SLC module, reset digital core");          // 6   SDIO_RESET
    case 7  : return F("Timer Group0 Watch dog reset digital core");        // 7   TG0WDT_SYS_RESET
    case 8  : return F("Timer Group1 Watch dog reset digital core");        // 8   TG1WDT_SYS_RESET
    case 9  : return F("RTC Watch dog Reset digital core");                 // 9   RTCWDT_SYS_RESET
    case 10 : return F("Instrusion tested to reset CPU");                   // 10  INTRUSION_RESET
    case 11 : return F("Time Group0 reset CPU");                            // 11  TGWDT_CPU_RESET   TG0WDT_CPU_RESET
    case 12 : return F("Software reset CPU");                               // 12  SW_CPU_RESET      RTC_SW_CPU_RESET
    case 13 : return F("RTC Watch dog Reset CPU");                          // 13  RTCWDT_CPU_RESET
    case 14 : return F("or APP CPU, reseted by PRO CPU");                   // 14  EXT_CPU_RESET     -
    case 15 : return F("Reset when the vdd voltage is not stable");         // 15  RTCWDT_BROWN_OUT_RESET
    case 16 : return F("RTC Watch dog reset digital core and rtc module");  // 16  RTCWDT_RTC_RESET
    case 17 : return F("Time Group1 reset CPU");                            // 17  -                 TG1WDT_CPU_RESET
    case 18 : return F("Super watchdog reset digital core and rtc module"); // 18  -                 SUPER_WDT_RESET
    case 19 : return F("Glitch reset digital core and rtc module");         // 19  -                 GLITCH_RTC_RESET
    case 20 : return F("Efuse reset digital core");                         // 20                    EFUSE_RESET
    case 21 : return F("Usb uart reset digital core");                      // 21                    USB_UART_CHIP_RESET
    case 22 : return F("Usb jtag reset digital core");                      // 22                    USB_JTAG_CHIP_RESET
    case 23 : return F("Power glitch reset digital core and rtc module");   // 23                    POWER_GLITCH_RESET
  }

  return F("No meaning");                                                   // 0 and undefined
}

String ESP_getResetReason(void) {
  return ESP32GetResetReason(0);  // CPU 0
}

uint32_t ESP_ResetInfoReason(void) {
  RESET_REASON reason = rtc_get_reset_reason(0);
  if (1  == reason) { return REASON_DEFAULT_RST; }       // POWERON_RESET
  if (12 == reason) { return REASON_SOFT_RESTART; }      // SW_CPU_RESET / RTC_SW_CPU_RESET
  if (5  == reason) { return REASON_DEEP_SLEEP_AWAKE; }  // DEEPSLEEP_RESET
  if (3  == reason) { return REASON_EXT_SYS_RST; }       // SW_RESET / RTC_SW_SYS_RESET
  return -1; //no "official error code", but should work with the current code base
}

uint32_t ESP_getChipId(void) {
  uint32_t id = 0;
  for (uint32_t i = 0; i < 17; i = i +8) {
    id |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  }
  return id;
}

uint32_t ESP_getFlashChipMagicSize(void) {
    esp_image_header_t fhdr;
//    if(ESP.flashRead(ESP_FLASH_IMAGE_BASE, (uint32_t*)&fhdr.magic, sizeof(esp_image_header_t)) && fhdr.magic != ESP_IMAGE_HEADER_MAGIC) {
//      return 0;
//    }
    if (esp_flash_read(esp_flash_default_chip, (void*)&fhdr, ESP_FLASH_IMAGE_BASE, sizeof(esp_image_header_t)) && fhdr.magic != ESP_IMAGE_HEADER_MAGIC) {
      return 0;
    }
    return ESP_magicFlashChipSize(fhdr.spi_size);
}

uint32_t ESP_magicFlashChipSize(uint8_t byte)
{
    switch(byte & 0x0F) {
    case 0x0: // 8 MBit (1MB)
        return 1048576;
    case 0x1: // 16 MBit (2MB)
        return 2097152;
    case 0x2: // 32 MBit (4MB)
        return 4194304;
    case 0x3: // 64 MBit (8MB)
        return 8388608;
    case 0x4: // 128 MBit (16MB)
        return 16777216;
    default: // fail?
        return 0;
    }
}

uint32_t ESP_getSketchSize(void) {
  static uint32_t sketchsize = 0;

  if (!sketchsize) {
    sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
  }
  return sketchsize;
}

uint32_t ESP_getFreeSketchSpace(void) {
  if (EspSingleOtaPartition()) {
    uint32_t size = EspRunningFactoryPartition();
    if (!size) {
      size = ESP.getFreeSketchSpace();
    }
    return size - ESP_getSketchSize();
  }
  return ESP.getFreeSketchSpace();
}

uint32_t ESP_getFreeHeap(void) {
  // ESP_getFreeHeap() returns also IRAM which we don't use
  return heap_caps_get_free_size(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
}

uint32_t ESP_getMaxAllocHeap(void) {
  // arduino returns IRAM but we want only DRAM
#ifdef USE_GT911 // GT911 IRQ crashes with heap_caps_get_largest_free_block
  return ESP_getFreeHeap();
#endif
  uint32_t free_block_size = heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  if (free_block_size > 100) { free_block_size -= 100; }
  return free_block_size;
}

int32_t ESP_getHeapFragmentation(void) {
  int32_t free_maxmem = 100 - (int32_t)(ESP_getMaxAllocHeap() * 100 / ESP_getFreeHeap());
  if (free_maxmem < 0) { free_maxmem = 0; }
  return free_maxmem;
}

void ESP_Restart(void) {
  ESP.restart();
}

uint32_t FlashWriteStartSector(void) {
  // Needs to be on SPI_FLASH_MMU_PAGE_SIZE (= 0x10000) alignment for mmap usage
  uint32_t aligned_address = ((EspFlashBaseAddress() + (2 * SPI_FLASH_MMU_PAGE_SIZE)) / SPI_FLASH_MMU_PAGE_SIZE) * SPI_FLASH_MMU_PAGE_SIZE;
  return aligned_address / SPI_FLASH_SEC_SIZE;
}

uint32_t FlashWriteMaxSector(void) {
  // Needs to be on SPI_FLASH_MMU_PAGE_SIZE (= 0x10000) alignment for mmap usage
  uint32_t aligned_end_address = (EspFlashBaseEndAddress() / SPI_FLASH_MMU_PAGE_SIZE) * SPI_FLASH_MMU_PAGE_SIZE;
  return aligned_end_address / SPI_FLASH_SEC_SIZE;
}

uint8_t* FlashDirectAccess(void) {
  uint32_t address = FlashWriteStartSector() * SPI_FLASH_SEC_SIZE;
  uint8_t* data = EspFlashMmap(address);
/*
  uint8_t buf[32];
  memcpy(buf, data, sizeof(buf));
  AddLog(LOG_LEVEL_DEBUG, PSTR("DBG: Flash start address 0x%08X, Mmap address 0x%08X, Data %*_H"), address, data, sizeof(buf), (uint8_t*)&buf);
*/
  return data;
}

extern "C" {
  #if ESP_IDF_VERSION_MAJOR >= 5
    // bool IRAM_ATTR __attribute__((pure)) esp_psram_is_initialized(void)
    bool esp_psram_is_initialized(void);
  #else
    bool esp_spiram_is_initialized(void);
  #endif
}

// this function is a replacement for `psramFound()`.
// `psramFound()` can return true even if no PSRAM is actually installed
// This new version also checks `esp_spiram_is_initialized` to know if the PSRAM is initialized
bool FoundPSRAM(void) {
#if CONFIG_IDF_TARGET_ESP32C3 || CONFIG_IDF_TARGET_ESP32C6
  return psramFound();
#else
  #if ESP_IDF_VERSION_MAJOR >= 5
    return psramFound() && esp_psram_is_initialized();
  #else
    return psramFound() && esp_spiram_is_initialized();
  #endif
#endif
}

// new function to check whether PSRAM is present and supported (i.e. required pacthes are present)
bool UsePSRAM(void) {
  static bool can_use_psram = CanUsePSRAM();
  return FoundPSRAM() && can_use_psram;
}

void *special_malloc(uint32_t size) {
  if (UsePSRAM()) {
    return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return malloc(size);
  }
}
void *special_realloc(void *ptr, size_t size) {
  if (UsePSRAM()) {
    return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return realloc(ptr, size);
  }
}
void *special_calloc(size_t num, size_t size) {
  if (UsePSRAM()) {
    return heap_caps_calloc(num, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
  } else {
    return calloc(num, size);
  }
}

// Variants for IRAM heap, which need all accesses to be 32 bits aligned
void *special_malloc32(uint32_t size) {
  return heap_caps_malloc(size, MALLOC_CAP_32BIT);
}

float CpuTemperature(void) {
  return (float)temperatureRead();  // In Celsius
}

/*
#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();

#ifdef __cplusplus
}
#endif

#ifdef CONFIG_IDF_TARGET_ESP32
uint8_t temprature_sens_read();

float CpuTemperature(void) {
  uint8_t t = temprature_sens_read();

  AddLog(LOG_LEVEL_DEBUG, PSTR("TMP: value %d"), t);

  return (t - 32) / 1.8;
}
#else
float CpuTemperature(void) {
  // Currently (20210801) repeated calls to temperatureRead() on ESP32C3 and ESP32S2 result in IDF error messages
  static float t = NAN;
  if (isnan(t)) {
    t = (float)temperatureRead();  // In Celsius
  }
  return t;
}
#endif
*/

/*
#if CONFIG_IDF_TARGET_ESP32S2
#include "esp32s2/esp_efuse.h"
#elif CONFIG_IDF_TARGET_ESP32S3
#include "esp32s3/esp_efuse.h"
#elif CONFIG_IDF_TARGET_ESP32C3
#include "esp32c3/esp_efuse.h"
#endif
*/

// #include "esp_chip_info.h"

String GetDeviceHardware(void) {
  // https://www.espressif.com/en/products/socs

/*
Source: esp-idf esp_system.h or arduino core esp_chip_info.h and esptool

typedef enum {
    CHIP_ESP32  = 1, //!< ESP32
    CHIP_ESP32S2 = 2, //!< ESP32-S2
    CHIP_ESP32S3 = 9, //!< ESP32-S3
    CHIP_ESP32C3 = 5, //!< ESP32-C3
    CHIP_ESP32C2 = 12, //!< ESP32-C2
    CHIP_ESP32C6 = 13, //!< ESP32-C6
    CHIP_ESP32H2 = 16, //!< ESP32-H2
    CHIP_POSIX_LINUX = 999, //!< The code is running on POSIX/Linux simulator
} esp_chip_model_t;

// Chip feature flags, used in esp_chip_info_t
#define CHIP_FEATURE_EMB_FLASH      BIT(0)      //!< Chip has embedded flash memory
#define CHIP_FEATURE_WIFI_BGN       BIT(1)      //!< Chip has 2.4GHz WiFi
#define CHIP_FEATURE_BLE            BIT(4)      //!< Chip has Bluetooth LE
#define CHIP_FEATURE_BT             BIT(5)      //!< Chip has Bluetooth Classic
#define CHIP_FEATURE_IEEE802154     BIT(6)      //!< Chip has IEEE 802.15.4
#define CHIP_FEATURE_EMB_PSRAM      BIT(7)      //!< Chip has embedded psram

// The structure represents information about the chip
typedef struct {
    esp_chip_model_t model;  //!< chip model, one of esp_chip_model_t
    uint32_t features;       //!< bit mask of CHIP_FEATURE_x feature flags
    uint16_t revision;       //!< chip revision number (in format MXX; where M - wafer major version, XX - wafer minor version)
    uint8_t cores;           //!< number of CPU cores
} esp_chip_info_t;

*/
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  uint32_t chip_model = chip_info.model;
  uint32_t chip_revision = chip_info.revision;
//  uint32_t chip_revision = ESP.getChipRevision();
  // idf5 efuse_hal_chip_revision(void)
  if (chip_revision < 100) { chip_revision *= 100; }  // Make <idf5 idf5
  bool rev3 = (chip_revision >= 300);
//  bool single_core = (1 == ESP.getChipCores());
  bool single_core = (1 == chip_info.cores);

  uint32_t pkg_version = 0;
#if (ESP_IDF_VERSION_MAJOR >= 5)
  pkg_version = bootloader_common_get_chip_ver_pkg();
#endif

  switch (chip_model) {
    case 0:
    case 1: {  // ESP32
      /*
      ESP32 Series
      - 32-bit MCU & 2.4 GHz Wi-Fi & Bluetooth/Bluetooth LE
      - Two or one CPU core(s) with adjustable clock frequency, ranging from 80 MHz to 240 MHz
      - +19.5 dBm output power ensures a good physical range
      - Classic Bluetooth for legacy connections, also supporting L2CAP, SDP, GAP, SMP, AVDTP, AVCTP, A2DP (SNK) and AVRCP (CT)
      - Support for Bluetooth Low Energy (Bluetooth LE) profiles including L2CAP, GAP, GATT, SMP, and GATT-based profiles like BluFi, SPP-like, etc
      - Bluetooth Low Energy (Bluetooth LE) connects to smart phones, broadcasting low-energy beacons for easy detection
      - Sleep current is less than 5 μA, making it suitable for battery-powered and wearable-electronics applications
      - Peripherals include capacitive touch sensors, Hall sensor, SD card interface, Ethernet, high-speed SPI, UART, I2S and I2C
      */
#ifdef CONFIG_IDF_TARGET_ESP32
#if (ESP_IDF_VERSION_MAJOR < 5)
      pkg_version = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG) & 0x7;
#endif

//      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d"), chip_info.model, chip_revision, chip_info.cores);

      switch (pkg_version) {
        case 0:
          if (single_core) { return F("ESP32-S0WDQ6"); }     // Max 240MHz, Single core, QFN 6*6
          else if (rev3)   { return F("ESP32-D0WDQ6-V3"); }  // Max 240MHz, Dual core, QFN 6*6
          else {             return F("ESP32-D0WDQ6"); }     // Max 240MHz, Dual core, QFN 6*6
        case 1:
          if (single_core) { return F("ESP32-S0WD"); }       // Max 160MHz, Single core, QFN 5*5, ESP32-SOLO-1, ESP32-DevKitC
          else if (rev3)   { return F("ESP32-D0WD-V3"); }    // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32E, ESP32_WROVER-E, ESP32-DevKitC
          else {             return F("ESP32-D0WD"); }       // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32D, ESP32_WROVER-B, ESP32-DevKitC
        case 2:              return F("ESP32-D2WD");         // Max 160MHz, Dual core, QFN 5*5, 2MB embedded flash
        case 3:
          if (single_core) { return F("ESP32-S0WD-OEM"); }   // Max 160MHz, Single core, QFN 5*5, Xiaomi Yeelight
          else {             return F("ESP32-D0WD-OEM"); }   // Max 240MHz, Dual core, QFN 5*5
        case 4:
          if (single_core) { return F("ESP32-U4WDH-S"); }    // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-MINI-1, ESP32-DevKitM-1
          else {             return F("ESP32-U4WDH-D"); }    // Max 240MHz, Dual core, QFN 5*5, 4MB embedded flash
        case 5:
          if (rev3)        { return F("ESP32-PICO-V3"); }    // Max 240MHz, Dual core, LGA 7*7, ESP32-PICO-V3-ZERO, ESP32-PICO-V3-ZERO-DevKit
          else {             return F("ESP32-PICO-D4"); }    // Max 240MHz, Dual core, LGA 7*7, 4MB embedded flash, ESP32-PICO-KIT
        case 6:              return F("ESP32-PICO-V3-02");   // Max 240MHz, Dual core, LGA 7*7, 8MB embedded flash, 2MB embedded PSRAM, ESP32-PICO-MINI-02, ESP32-PICO-DevKitM-2
        case 7:              return F("ESP32-D0WDR2-V3");    // Max 240MHz, Dual core, QFN 5*5, ESP32-WROOM-32E, ESP32_WROVER-E, ESP32-DevKitC
      }
#endif  // CONFIG_IDF_TARGET_ESP32
      return F("ESP32");
    }
    case 2: {  // ESP32-S2
      /*
      ESP32-S2 Series
      - 32-bit MCU & 2.4 GHz Wi-Fi
      - High-performance 240 MHz single-core CPU
      - Ultra-low-power performance: fine-grained clock gating, dynamic voltage and frequency scaling
      - Security features: eFuse、flash encryption, secure boot, signature verification, integrated AES, SHA and RSA algorithms
      - Peripherals include 43 GPIOs, 1 full-speed USB OTG interface, SPI, I2S, UART, I2C, LED PWM, LCD interface, camera interface, ADC, DAC, touch sensor, temperature sensor
      - Availability of common cloud connectivity agents and common product features shortens the time to market
      */
#ifdef CONFIG_IDF_TARGET_ESP32S2
#if (ESP_IDF_VERSION_MAJOR < 5)
      pkg_version = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_FLASH_VERSION) & 0xF;
#endif
      uint32_t psram_ver = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PSRAM_VERSION);
      pkg_version += ((psram_ver & 0xF) * 100);

//      AddLog(LOG_LEVEL_DEBUG_MORE, PSTR("HDW: ESP32 Model %d, Revision %d, Core %d, Package %d"), chip_info.model, chip_revision, chip_info.cores, chip_ver);

      switch (pkg_version) {
        case 0:              return F("ESP32-S2");           // Max 240MHz, Single core, QFN 7*7, ESP32-S2-WROOM, ESP32-S2-WROVER, ESP32-S2-Saola-1, ESP32-S2-Kaluga-1
        case 1:              return F("ESP32-S2FH2");        // Max 240MHz, Single core, QFN 7*7, 2MB embedded flash, ESP32-S2-MINI-1, ESP32-S2-DevKitM-1
        case 2:              return F("ESP32-S2FH4");        // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash
        case 3:              return F("ESP32-S2FN4R2");      // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash, 2MB embedded PSRAM, , ESP32-S2-MINI-1U, ESP32-S2-DevKitM-1U
        case 100:            return F("ESP32-S2R2");
        case 102:            return F("ESP32-S2FNR2");       // Max 240MHz, Single core, QFN 7*7, 4MB embedded flash, 2MB embedded PSRAM, , Lolin S2 mini
      }
#endif  // CONFIG_IDF_TARGET_ESP32S2
      return F("ESP32-S2");
    }
    case 5: {  // ESP32-C3 = ESP8685 if embedded flash
      /*
      ESP32-C3 Series
      - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
      - 32-bit RISC-V single-core processor with a four-stage pipeline that operates at up to 160 MHz
      - State-of-the-art power and RF performance
      - 400 KB of SRAM and 384 KB of ROM on the chip, and SPI, Dual SPI, Quad SPI, and QPI interfaces that allow connection to flash
      - Reliable security features ensured by RSA-3072-based secure boot, AES-128-XTS-based flash encryption, the innovative digital signature and the HMAC peripheral, hardware acceleration support for cryptographic algorithms
      - Rich set of peripheral interfaces and GPIOs, ideal for various scenarios and complex applications
      */
#ifdef CONFIG_IDF_TARGET_ESP32C3
#if (ESP_IDF_VERSION_MAJOR < 5)
      pkg_version = REG_GET_FIELD(EFUSE_RD_MAC_SPI_SYS_3_REG, EFUSE_PKG_VERSION) & 0x7;
#endif
      switch (pkg_version) {
        case 0:              return F("ESP32-C3");           // Max 160MHz, Single core, QFN 5*5, ESP32-C3-WROOM-02, ESP32-C3-DevKitC-02
//        case 1:              return F("ESP32-C3FH4");        // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-C3-MINI-1, ESP32-C3-DevKitM-1
        case 1:              return F("ESP8685");            // Max 160MHz, Single core, QFN 5*5, 4MB embedded flash, ESP32-C3-MINI-1, ESP32-C3-DevKitM-1
        case 2:              return F("ESP32-C3 AZ");        // QFN32
        case 3:              return F("ESP8686");            // QFN24
      }
#endif  // CONFIG_IDF_TARGET_ESP32C3
      return F("ESP32-C3");
    }
    case 4:     // ESP32-S3(beta2)
    case 6:     // ESP32-S3(beta3)
    case 9:  {  // ESP32-S3
      /*
      ESP32-S3 Series
      - 32-bit MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
      - Xtensa® 32-bit LX7 dual-core processor that operates at up to 240 MHz
      - 512 KB of SRAM and 384 KB of ROM on the chip, and SPI, Dual SPI, Quad SPI, Octal SPI, QPI, and OPI interfaces that allow connection to flash and external RAM
      - Additional support for vector instructions in the MCU, which provides acceleration for neural network computing and signal processing workloads
      - Peripherals include 45 programmable GPIOs, SPI, I2S, I2C, PWM, RMT, ADC and UART, SD/MMC host and TWAITM
      - Reliable security features ensured by RSA-based secure boot, AES-XTS-based flash encryption, the innovative digital signature and the HMAC peripheral, “World Controller”
      */
#ifdef CONFIG_IDF_TARGET_ESP32S3
#if (ESP_IDF_VERSION_MAJOR >= 5)
      switch (pkg_version) {
        case 0:              return F("ESP32-S3");           // QFN56
        case 1:              return F("ESP32-S3-PICO-1");    // LGA56
      }
#endif
#endif  // CONFIG_IDF_TARGET_ESP32S3
      return F("ESP32-S3");                                  // Max 240MHz, Dual core, QFN 7*7, ESP32-S3-WROOM-1, ESP32-S3-DevKitC-1
    }
    case 12: {  // ESP32-C2 = ESP8684 if embedded flash
      /*
      ESP32-C2 Series
      - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi & Bluetooth 5 (LE)
      - 32-bit RISC-V single-core processor that operates at up to 120 MHz
      - State-of-the-art power and RF performance
      - 576 KB ROM, 272 KB SRAM (16 KB for cache) on the chip
      - 14 programmable GPIOs: SPI, UART, I2C, LED PWM controller, General DMA controller (GDMA), SAR ADC, Temperature sensor
      */
#ifdef CONFIG_IDF_TARGET_ESP32C2
      switch (pkg_version) {
        case 0:              return F("ESP32-C2");
        case 1:              return F("ESP32-C2");
      }
#endif  // CONFIG_IDF_TARGET_ESP32C2
      return F("ESP32-C2");
    }
    case 7:     // ESP32-C6(beta)
    case 13: {  // ESP32-C6
      /*
      ESP32-C6 Series
      - 32-bit RISC-V MCU & 2.4 GHz Wi-Fi 6 & Bluetooth 5 (LE) & IEEE 802.15.4
      - 32-bit RISC-V single-core processor that operates at up to 160 MHz
      - State-of-the-art power and RF performance
      - 320 KB ROM, 512 KB SRAM, 16 KB Low-power SRAM on the chip, and works with external flash
      - 30 (QFN40) or 22 (QFN32) programmable GPIOs, with support for SPI, UART, I2C, I2S, RMT, TWAI and PWM
      */
#ifdef CONFIG_IDF_TARGET_ESP32C6
      switch (pkg_version) {
        case 0:              return F("ESP32-C6");
        case 1:              return F("ESP32-C6FH4");
      }
#endif  // CONFIG_IDF_TARGET_ESP32C6
      return F("ESP32-C6");
    }
    case 10:    // ESP32-H2(beta1)
    case 14:    // ESP32-H2(beta2)
    case 16: {  // ESP32-H2
#ifdef CONFIG_IDF_TARGET_ESP32H2
      switch (pkg_version) {
        case 0:              return F("ESP32-H2");
      }
#endif  // CONFIG_IDF_TARGET_ESP32H2
      return F("ESP32-H2");
    }
    case 18: {  // ESP32-P4
#ifdef CONFIG_IDF_TARGET_ESP32P4
      switch (pkg_version) {
        case 0:              return F("ESP32-P4");
      }
#endif  // CONFIG_IDF_TARGET_ESP32P4
      return F("ESP32-P4");
    }
  }
  return F("ESP32");
}

String GetDeviceHardwareRevision(void) {
  // ESP32-S2
  // ESP32-D0WDQ6 v1.0
  // ESP32-C3 v2.0
  // ESP32-C3 v3.0
  // ESP32-C6FH4 v0.0
  String result = GetDeviceHardware();   // ESP32-C3

  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
//  if (chip_info.revision) {              // Only show >rev 0.0
    // idf5 efuse_hal_chip_revision(void)
    uint32_t chip_revision = chip_info.revision;
    if (chip_revision < 100) { chip_revision *= 100; }  // Make <idf5 idf5
    char revision[16];
    snprintf_P(revision, sizeof(revision), PSTR(" v%d.%d"), chip_revision / 100, chip_revision % 100);
    result += revision;                  // ESP32-C3 v3.0
//  }

  return result;
}

String GetCodeCores(void) {
#if defined(CORE32SOLO1)
  return F("single-core");
#else
  return F("");
#endif
}

/*
 * ESP32 v1 and v2 needs some special patches to use PSRAM.
 * Standard Tasmota 32 do not include those patches.
 * If using ESP32 v1, please add: `-mfix-esp32-psram-cache-issue -lc-psram-workaround -lm-psram-workaround`
 *
 * This function returns true if the chip supports PSRAM natively (v3) or if the
 * patches are present.
 */
bool CanUsePSRAM(void) {
  if (!FoundPSRAM()) return false;
#ifdef HAS_PSRAM_FIX
  return true;
#endif
#ifdef CONFIG_IDF_TARGET_ESP32
  esp_chip_info_t chip_info;
  esp_chip_info(&chip_info);
  uint32_t chip_revision = chip_info.revision;
  // idf5 efuse_hal_chip_revision(void)
  if (chip_revision < 100) { chip_revision *= 100; }  // Make <idf5 idf5
  if ((CHIP_ESP32 == chip_info.model) && (chip_revision < 300)) {
    return false;
  }
#if ESP_IDF_VERSION_MAJOR < 4
  uint32_t pkg_version = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG) & 0x7;
  if ((CHIP_ESP32 == chip_info.model) && (pkg_version >= 6)) {
    return false;   // support for embedded PSRAM of ESP32-PICO-V3-02 requires esp-idf 4.4
  }
#endif // ESP_IDF_VERSION_MAJOR < 4

#endif // CONFIG_IDF_TARGET_ESP32
  return true;
}

#endif  // ESP32

/*********************************************************************************************\
 * ESP Support
\*********************************************************************************************/

uint32_t ESP_getFreeHeap1024(void) {
  return ESP_getFreeHeap() / 1024;
}
/*
float ESP_getFreeHeap1024(void) {
  return ((float)ESP_getFreeHeap()) / 1024;
}
*/

/*********************************************************************************************\
 * High entropy hardware random generator
 * Thanks to DigitalAlchemist
\*********************************************************************************************/
// Based on code from https://raw.githubusercontent.com/espressif/esp-idf/master/components/esp32/hw_random.c
uint32_t HwRandom(void) {
#if ESP8266
  // https://web.archive.org/web/20160922031242/http://esp8266-re.foogod.com/wiki/Random_Number_Generator
  #define _RAND_ADDR 0x3FF20E44UL
#endif  // ESP8266
#ifdef ESP32
  #define _RAND_ADDR 0x3FF75144UL
#endif  // ESP32
  static uint32_t last_ccount = 0;
  uint32_t ccount;
  uint32_t result = 0;
  do {
    ccount = ESP.getCycleCount();
    result ^= *(volatile uint32_t *)_RAND_ADDR;
  } while (ccount - last_ccount < 64);
  last_ccount = ccount;
  return result ^ *(volatile uint32_t *)_RAND_ADDR;
#undef _RAND_ADDR
}



#endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE_ESP_SUPPORT










/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void mSettings::SettingsSaveAll(void)
{
  Serial.println("SettingsSaveAll");
  // if (Settings.flag_system.save_state) {
  //   Settings.power = power;
  // } else {
  //   Settings.power = 0;
  // }
  pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);
#ifdef USE_EEPROM
  EepromCommit();
#endif
  SettingsSave(1);
}

// /*********************************************************************************************\
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
//  * Config Save - Save parameters to Flash ONLY if any parameter has changed
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/
// \*********************************************************************************************/

#ifdef ENABLE_SETTINGS_VERSION_1_ESP8266_ONLY





#endif // ENABLE_SETTINGS_VERSION_1_ESP8266_ONLY


/***
 * Modified version: 
 * - Removing rotating writing until location0 works well
 * - Adding that settings first gets read into a local temporary copy, then a version merges the loaded values into values I want to overwrite.
 *    This is so I can slowly add what I like to have saved in memory until I can trust it full
 * */
void mSettings::SettingsLoad(void) {
  
  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_LOAD ));
  #endif// ENABLE_LOG_LEVEL_INFO

  #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SAVE_LOAD_STRUCT


  #ifdef ESP8266
    // Load configuration from optional file and flash (eeprom and 7 additonal slots) if first valid load does not stop_flash_rotate
    // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
    settings_location = 0;
    uint32_t save_flag = 0;
    uint32_t max_slots = CFG_ROTATES +1;
    uint32_t flash_location;
    uint32_t slot = 1;
  #ifdef USE_UFILESYS
    if (TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings))) {
      flash_location = 1;
      slot = 0;
    }
  #endif  // USE_UFILESYS
    while (slot <= max_slots) {                                  // Read all config pages in search of valid and latest
      if (slot > 0) {
        flash_location = (1 == slot) ? EEPROM_LOCATION : (2 == slot) ? SETTINGS_LOCATION : flash_location -1;
        ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)Settings, sizeof(TSettings));
      }
      if ((Settings->cfg_crc32 != 0xFFFFFFFF) && (Settings->cfg_crc32 != 0x00000000) && (Settings->cfg_crc32 == GetSettingsCrc32())) {
        if (Settings->save_flag > save_flag) {                    // Find latest page based on incrementing save_flag
          save_flag = Settings->save_flag;
          settings_location = flash_location;
          if (Settings->flag.stop_flash_rotate && (1 == slot)) {  // Stop if only eeprom area should be used and it is valid
            break;
          }
        }
      }
      slot++;
      delay(1);
    }
    if (settings_location > 0) {
  #ifdef USE_UFILESYS
      if (1 == settings_location) {
        TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings));
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Loaded from File, " D_COUNT " %lu"), Settings->save_flag);
      } else
  #endif  // USE_UFILESYS
      {
        ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)Settings, sizeof(TSettings));
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu"), settings_location, Settings->save_flag);
      }
    }
  #endif  // ESP8266

  #ifdef ESP32
    uint32_t source = pCONT_sup->SettingsRead(Settings, sizeof(TSettings));
    if (source) {
      settings_location = 1;
      if (Settings->cfg_holder == (uint16_t)CFG_HOLDER) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Loaded from %s, " D_COUNT " %lu"), (2 == source)?"File":"NVS", Settings->save_flag);
      }
    }
  #endif  // ESP32

  #ifndef FIRMWARE_MINIMAL
    if ((0 == settings_location) || (Settings->cfg_holder != (uint16_t)CFG_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
  //  if ((0 == settings_location) || (Settings->cfg_size != sizeof(TSettings)) || (Settings->cfg_holder != (uint16_t)CFG_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
  #ifdef USE_UFILESYS
      if (TfsLoadFile(TASM_FILE_SETTINGS_LKG, (uint8_t*)Settings, sizeof(TSettings)) && (Settings->cfg_crc32 == GetSettingsCrc32())) {
        settings_location = 1;
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Loaded from LKG File, " D_COUNT " %lu"), Settings->save_flag);
      } else
  #endif  // USE_UFILESYS
      {
        SettingsDefault();
      }
    }
    settings_crc32 = GetSettingsCrc32();
  #endif  // FIRMWARE_MINIMAL

  RtcSettingsLoad(1);


#endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE__SAVE_LOAD_STRUCT








//   #ifdef ESP8266
//   settings_location = 0;
//   uint32_t save_flag = 0;
//   uint32_t flash_location = SETTINGS_LOCATION;

//   /**
//    * Local copy, used to read in, check against, and only then commit what I want by merging into working struct
//    * */
//   SYSCFG Settings_Temporary;

// // DEBUG_LINE_HERE;

//   /***
//    * Search for valid save location, and check for validity of saved data
//    * */

// uint32_t i = 0;
//   // for (uint32_t i = 0; i < CFG_ROTATES; i++) {              // Read all config pages in search of valid and latest

//     ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings_Temporary, sizeof(Settings_Temporary));
    
//     // #ifdef ENABLE_LOG_LEVEL_INFO
//     // AddLog(LOG_LEVEL_TEST, PSTR("\n\r"
//     //     "cfg_holder\t\t\t%d\n\r"
//     //     "cfg_size\t\t\t%d\n\r"
//     //     "save_flag\t\t\t%d\n\r"
//     //     "version\t\t\t%d\n\r"
//     //     "bootcount\t\t\t%d\n\r"
//     //     "cfg_crc\t\t\t%d\n\r"
//     //     //Test data
//     //     "%d:%d:%d:%d"
//     //   ),
//     //     Settings_Temporary.cfg_holder,
//     //     Settings_Temporary.cfg_size,
//     //     Settings_Temporary.save_flag,
//     //     Settings_Temporary.version,
//     //     Settings_Temporary.bootcount,
//     //     Settings_Temporary.cfg_crc
//     //     //Testdata
//     //     ,Settings.animation_settings.xmas_controller_params[0]
//     //     ,Settings.animation_settings.xmas_controller_params[1]
//     //     ,Settings.animation_settings.xmas_controller_params[2]
//     //     ,Settings.animation_settings.xmas_controller_params[3]
//     // );
//     // #endif// ENABLE_LOG_LEVEL_INFO

// // DEBUG_LINE_HERE;

//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_LOAD " i=%d bootcount=%d version=%X"),i,Settings_Temporary.bootcount,Settings_Temporary.version);
//     #endif // ENABLE_LOG_LEVEL_INFO

//     if ((Settings_Temporary.cfg_crc32 != 0xFFFFFFFF) && (Settings_Temporary.cfg_crc32 != 0x00000000))// && (Settings_Temporary.cfg_crc32 == GetSettingsCrc32())) 
//     {

// // DEBUG_LINE_HERE;

//     #ifdef ENABLE_LOG_LEVEL_INFO
//     // Since these are not equal, something is wrong
//     AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_LOAD " cfg_crc==GetSettingsCrc32()| %d==%d"),Settings_Temporary.cfg_crc32,GetSettingsCrc32());
//     if(Settings_Temporary.cfg_crc32 != GetSettingsCrc32())
//     {
//       AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_LOAD DEBUG_INSERT_PAGE_BREAK "CRC32 ERROR cfg_crc==GetSettingsCrc32()| %d==%d"),Settings_Temporary.cfg_crc32,GetSettingsCrc32());
//     }
//     #endif // ENABLE_LOG_LEVEL_INFO
      
//       if (Settings_Temporary.save_flag > save_flag) {                 // Find latest page based on incrementing save_flag
//         #ifdef ENABLE_LOG_LEVEL_INFO
//         AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_LOAD " Settings_Temporary.save_flag > save_flag %d>%d"),Settings_Temporary.save_flag,save_flag);
//         #endif // ENABLE_LOG_LEVEL_INFO
        
//         save_flag = Settings_Temporary.save_flag;
//         settings_location = flash_location;
//         // if (Settings_Temporary.flag_system.stop_flash_rotate && (0 == i)) {  // Stop if only eeprom area should be used and it is valid
//         //   #ifdef ENABLE_LOG_LEVEL_INFO
//         //   AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_LOAD " Settings_Temporary.flag_system.stop_flash_rotate && (0 == i)"));
//         //   #endif // ENABLE_LOG_LEVEL_INFO
//         //   break;
//         // }
//       }
//     }
//     // flash_location--;
//     // delay(1);
//   // }



//   /**
//    * Assuming we have valid read, then re-read the entire data
//    * */
//   if (settings_location > 0) {
    
//   //   AddLog(LOG_LEVEL_TEST, PSTR("BEFORE READ\n\r"
//   //     "cfg_holder\t\t\t%d\n\r"
//   //     "cfg_size\t\t\t%d\n\r"
//   //     "save_flag\t\t\t%d\n\r"
//   //     "version\t\t\t%d\n\r"
//   //     "bootcount\t\t\t%d\n\r"
//   //     "cfg_crc\t\t\t%d\n\r"
//   //   ),
//   //     Settings_Temporary.cfg_holder,
//   //     Settings_Temporary.cfg_size,
//   //     Settings_Temporary.save_flag,
//   //     Settings_Temporary.version,
//   //     Settings_Temporary.bootcount,
//   //     Settings_Temporary.cfg_crc
//   // );
//     ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
//   //   AddLog(LOG_LEVEL_TEST, PSTR("AFTER READ\n\r"
//   //     "cfg_holder\t\t\t%d\n\r"
//   //     "cfg_size\t\t\t%d\n\r"
//   //     "save_flag\t\t\t%d\n\r"
//   //     "version\t\t\t%d\n\r"
//   //     "bootcount\t\t\t%d\n\r"
//   //     "cfg_crc\t\t\t%d\n\r"
//   //   ),
//   //     Settings_Temporary.cfg_holder,
//   //     Settings_Temporary.cfg_size,
//   //     Settings_Temporary.save_flag,
//   //     Settings_Temporary.version,
//   //     Settings_Temporary.bootcount,
//   //     Settings_Temporary.cfg_crc
//   // );
    
//       // AddLog(LOG_LEVEL_INFO, PSTR("\n\r\r\n\r\r\n\r\r\n\r\r LOAD param5=%d:%d:%d:%d:%d:%d"), 
//       // Settings.animation_settings.xmas_controller_params[0],Settings.animation_settings.xmas_controller_params[1],Settings.animation_settings.xmas_controller_params[2],Settings.animation_settings.xmas_controller_params[3],Settings.animation_settings.xmas_controller_params[4],Settings.animation_settings.xmas_controller_params[5]);
//   // delay(5000);
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_TEST, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu " D_JSON_BOOTCOUNT " %d"), settings_location, Settings_Temporary.save_flag, Settings_Temporary.bootcount);
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }

  



// // #else  // ESP32
// //   SettingsRead(&Settings, sizeof(Settings));
// //   AddLog_P2(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded, " D_COUNT " %lu"), Settings.save_flag);
// // #endif  // ESP8266 - ESP32


//   /***
//    * Merging desired changes only
//    * */
//   #ifdef ENABLE_LOG_LEVEL_INFO
//   AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Merging loaded settings into local settings"));
//   #endif // ENABLE_LOG_LEVEL_INFO

//   SettingsMerge(&Settings, &Settings_Temporary);

//     // #ifdef ENABLE_LOG_LEVEL_INFO
//     // AddLog(LOG_LEVEL_TEST, PSTR("AFTER SettingsMerge\n\r"
//     //     "cfg_holder\t\t\t%d\n\r"
//     //     "cfg_size\t\t\t%d\n\r"
//     //     "save_flag\t\t\t%d\n\r"
//     //     "version\t\t\t%d\n\r"
//     //     "bootcount\t\t\t%d\n\r"
//     //     "cfg_crc\t\t\t%d\n\r"
//     //     //Test data
//     //     "%d:%d:%d:%d"
//     //   ),
//     //     Settings_Temporary.cfg_holder,
//     //     Settings_Temporary.cfg_size,
//     //     Settings_Temporary.save_flag,
//     //     Settings_Temporary.version,
//     //     Settings_Temporary.bootcount,
//     //     Settings_Temporary.cfg_crc
//     //     //Testdata
//     //     ,Settings.animation_settings.xmas_controller_params[0]
//     //     ,Settings.animation_settings.xmas_controller_params[1]
//     //     ,Settings.animation_settings.xmas_controller_params[2]
//     //     ,Settings.animation_settings.xmas_controller_params[3]
//     // );

//     // #endif // ENABLE_LOG_LEVEL_INFO
    
// #ifndef FIRMWARE_MINIMAL

// /***
//  * If something was bad with the read, fall back to defaults
//  * */
//   if ((0 == settings_location) || (Settings_Temporary.cfg_holder != (uint16_t)SETTINGS_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
//       // DEBUG_LINE_HERE;
//     //Settings_Temporary.seriallog_level = LOG_LEVEL_ALL;
//     pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
    
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY "cfg_holder(%d) != SETTINGS_HOLDER(%d), Erasing"),Settings.cfg_holder,SETTINGS_HOLDER);
//     #endif // ENABLE_LOG_LEVEL_INFO

//     SettingsDefault();
//   }else{
//       // DEBUG_LINE_HERE;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " " D_JSON_SUCCESSFUL ));
//     #endif // ENABLE_LOG_LEVEL_INFO
//   }
//       // DEBUG_LINE_HERE;
//   settings_crc32 = GetSettingsCrc32();
// #endif  // FIRMWARE_MINIMAL

//   #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
//   RtcSettingsLoad(1);
//   #endif

//       // DEBUG_LINE_HERE;
//   #endif //  ESP8266

//   #ifdef ENABLE_DEVFEATURE_ADVANCED_SETTINGS_SAVE_DEBUG
  
//       // AddLog(LOG_LEVEL_INFO, PSTR("LOAD param1=%d"), Settings.animation_settings.xmas_controller_params[0]);
//   // delay(5000);
//   #endif
}


/***
 * Until settings are correctly saving in memory and long term tested, only the values transferred here will be loaded from memory and overwrite the defaults
 * Caution: Wifi/mqtt must remain hardcoded, so NOT transferred here
 * */
void mSettings::SettingsMerge(SYSCFG* s, SYSCFG* l)
{
  #ifdef ENABLE_LOG_LEVEL_INFO
  // AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "SettingsMerge: Only using loaded settings currently tested %d"), l->animation_settings.xmas_controller_params[0]);
  #endif // ENABLE_LOG_LEVEL_INFO

  // Header
  s->cfg_holder = l->cfg_holder;
  s->cfg_size = l->cfg_size;
  s->save_flag = l->save_flag;
  s->version = l->version;
  s->bootcount = l->bootcount;
  s->cfg_crc = l->cfg_crc;

  // Xmas Controller
  // s->animation_settings.xmas_controller_params[0] = l->animation_settings.xmas_controller_params[0];
  // s->animation_settings.xmas_controller_params[1] = l->animation_settings.xmas_controller_params[1];
  // s->animation_settings.xmas_controller_params[2] = l->animation_settings.xmas_controller_params[2];
  // s->animation_settings.xmas_controller_params[3] = l->animation_settings.xmas_controller_params[3];
  // s->animation_settings.xmas_controller_params[4] = l->animation_settings.xmas_controller_params[4];
  // s->animation_settings.xmas_controller_params[5] = l->animation_settings.xmas_controller_params[5];

}

uint16_t mSettings::GetCfgCrc16(uint8_t *bytes, uint32_t size)
{
  uint16_t crc = 0;

  for (uint32_t i = 0; i < size; i++) {
    if ((i < 14) || (i > 15)) { crc += bytes[i]*(i+1); }  // Skip crc
  }
  return crc;
}

uint16_t mSettings::GetSettingsCrc(void)
{
  // Fix miscalculation if previous Settings was 3584 and current Settings is 4096 between 0x06060007 and 0x0606000A
  uint32_t size = /*((Settings.version < 0x06060007) || (Settings.version > 0x0606000A)) ? 3584 :*/ sizeof(Settings);
  return GetCfgCrc16((uint8_t*)&Settings, size);
}

uint32_t mSettings::GetCfgCrc32(uint8_t *bytes, uint32_t size)
{
  // https://create.stephan-brumme.com/crc32/#bitwise
  uint32_t crc = 0;

  while (size--) {
    crc ^= *bytes++;
    for (uint32_t j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (-int(crc & 1) & 0xEDB88320);
    }
  }
  return ~crc;
}

uint32_t mSettings::GetSettingsCrc32(void)
{
  return GetCfgCrc32((uint8_t*)&Settings, sizeof(Settings) -4);  // Skip crc32
}



void mSettings::SettingsSave(uint8_t rotate)
{

// Serial.println("SettingsSave NOT return");
//   return;
  
    #ifdef ESP8266
/* Save configuration in eeprom or one of 7 slots below
 *
 * rotate 0 = Save in next flash slot
 * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
 * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
 * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
 * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
 */
// #ifndef FIRMWARE_MINIMAL
  // UpdateBackwardCompatibility();
  if ((GetSettingsCrc32() != settings_crc32) || rotate) {
    
    // if (1 == rotate) 
    // {   // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
    //   stop_flash_rotate = 1;
    //   #ifdef ENABLE_LOG_LEVEL_INFO
    //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MEMORY D_SAVE " stop_flash_rotate"));//(upgrade) Use eeprom flash slot only and disable flash rotate from now on"));
    //   #endif// ENABLE_LOG_LEVEL_INFO
    // }
    // if (2 == rotate) 
    // {   // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
    //   settings_location = SETTINGS_LOCATION +1;
    //   #ifdef ENABLE_LOG_LEVEL_INFO
    //   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_MEMORY D_SAVE " (default) Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off(%d) (default)"),stop_flash_rotate);
    //   #endif// ENABLE_LOG_LEVEL_INFO
    // }

    // if (stop_flash_rotate) {
    //   settings_location = SETTINGS_LOCATION;
    // } else {
    //   settings_location--;
    //   #ifdef ENABLE_LOG_LEVEL_INFO
    //   AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " settings_location=%d"),settings_location);
    //   #endif// ENABLE_LOG_LEVEL_INFO

    //   if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
    //     settings_location = SETTINGS_LOCATION;
    //     #ifdef ENABLE_LOG_LEVEL_INFO
    //     AddLog(LOG_LEVEL_TEST,PSTR("settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)"));
    //     #endif // ENABLE_LOG_LEVEL_INFO
    //   }
    // }
    settings_location = SETTINGS_LOCATION; // tmp fix, no rotations

    Settings.save_flag++;
    // if (UtcTime() > START_VALID_TIME) {
    //   Settings.cfg_timestamp = UtcTime();
    // } else {
    //   Settings.cfg_timestamp++;
    // }
    Settings.cfg_size  = sizeof(Settings);
    Settings.cfg_crc   = GetSettingsCrc();  // Keep for backward compatibility in case of fall-back just after upgrade
    Settings.cfg_crc32 = GetSettingsCrc32();

    
    // AddLog(LOG_LEVEL_INFO, PSTR("\n\r\r\n\r\r\n\r\r\n\r\rSAVING param1=%d"), Settings.animation_settings.xmas_controller_params[0]);


// #ifdef ESP8266
    if (ESP.flashEraseSector(settings_location)) {
      //DEBUG_LINE_HERE;
      // Settings.animation_settings.xmas_controller_params[5] = 5;

      ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
      
    
      //AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("ESP.flashWrite %d %d"),stop_flash_rotate,rotate);

      // TestSettingsLoad();

      // delay(1000);



    }else{
      
      // DEBUG_LINE_HERE;
    }

    if (!stop_flash_rotate && rotate) {
      // DEBUG_LINE_HERE;
      for (uint32_t i = 1; i < CFG_ROTATES; i++) 
      {
        // DEBUG_LINE_HERE;
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("ESP.flashEraseSector(settings_location -i);"));
    #endif // ENABLE_LOG_LEVEL_INFO
        ESP.flashEraseSector(settings_location -i);  // Delete previous configurations by resetting to 0xFF
        delay(1);
      }
    }else{      
      // DEBUG_LINE_HERE;
    }

    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d BootCount %d"), settings_location, Settings.save_flag, sizeof(Settings), Settings.bootcount);
    #endif// ENABLE_LOG_LEVEL_INFO
// #else  // ESP32
//     SettingsWrite(&Settings, sizeof(Settings));
//     AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "Saved, " D_COUNT " %d, " D_BYTES " %d"), Settings.save_flag, sizeof(Settings));
// #endif  // ESP8266

    settings_crc32 = Settings.cfg_crc32;
  }


// #endif  // FIRMWARE_MINIMAL

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  RtcSettingsSave();
  #endif

  
  #endif //  ESP8266
}

/**
 * Test load and print critical test points from primary storage location
 * */
void mSettings::TestSettingsLoad()
{
    #ifdef ESP8266
  SYSCFG settings_tmp;

  settings_location = SETTINGS_LOCATION;
  
  ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&settings_tmp, sizeof(settings_tmp));

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("TestSettingsLoad \t\t\t\t TestSettingsLoad\n\r"
      "cfg_holder\t\t\t%d\n\r"
      "cfg_size\t\t\t%d\n\r"
      "save_flag\t\t\t%d\n\r"
      "version\t\t\t%d\n\r"
      "bootcount\t\t\t%d\n\r"
      "cfg_crc\t\t\t%d\n\r"
    ),
      settings_tmp.cfg_holder,
      settings_tmp.cfg_size,
      settings_tmp.save_flag,
      settings_tmp.version,
      settings_tmp.bootcount,
      settings_tmp.cfg_crc
  );
    #endif // ESP8266
  
    #endif // ENABLE_LOG_LEVEL_INFO
}


void mSettings::TestSettings_ShowLocal_Header()
{

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_TEST, PSTR("TestSettings_ShowLocal_Header\n\r"
      "cfg_holder\t\t\t%d\n\r"
      "cfg_size\t\t\t%d\n\r"
      "save_flag\t\t\t%d\n\r"
      "version\t\t\t%d\n\r"
      "bootcount\t\t\t%d\n\r"
      "cfg_crc\t\t\t%d\n\r"
    ),
      Settings.cfg_holder,
      Settings.cfg_size,
      Settings.save_flag,
      Settings.version,
      Settings.bootcount,
      Settings.cfg_crc
  );
    #endif// ENABLE_LOG_LEVEL_INFO
  
}


// #else // ENABLE_DEVFEATURE_SETTINGS_V2


// void mSettings::SettingsLoad(void)
// {
//   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD));

//   // Load configuration from eeprom or one of 7 slots below if first valid load does not stop_flash_rotate
//   struct SYSCFGH {
//     uint16_t cfg_holder;                     // 000
//     uint16_t cfg_size;                       // 002
//     unsigned long save_flag;                 // 004
//   } _SettingsH;
//   unsigned long save_flag = 0;

//   Settings.flag_system.stop_flash_rotate = 1;// temp measure

//   settings_location = 0;
//   uint32_t flash_location = SETTINGS_LOCATION +1; //next memory location

//   uint16_t cfg_holder = 0;

  
//   for (uint8_t i = 0; i < CFG_ROTATES; i++) {
//     flash_location--;
//     ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));

//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " i=%d bootcount=%d version=%X"),i,Settings.bootcount,Settings.version);

//     // if(Settings.version != 0xFFFFFFFF){
//     //   AddLog(LOG_LEVEL_TEST,PSTR("i=%d version=%X DOES NOT EQUAL tversion=%X"),i,Settings.version,0xFFFFFFFF);
//     // }else{
//     //   AddLog(LOG_LEVEL_TEST,PSTR("ELSE ELSE i=%d version=%X tversion=%X"),i,Settings.version,0xFFFFFFFF);
//     //   //break;// test
//     // }

//     bool valid = false;
//     if((Settings.version > 0x06000000)&&(Settings.version != 0xFFFFFFFF)) {
//       //AddLog(LOG_LEVEL_TEST,PSTR("ESP.flashRead %i IF"),i);
//       bool almost_valid = (Settings.cfg_crc == GetSettingsCrc());
//       AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " cfg_crc==GetSettingsCrc()| %d==%d"),Settings.cfg_crc,GetSettingsCrc());
//       // Sometimes CRC on pages below FB, overwritten by OTA, is fine but Settings are still invalid. So check cfg_holder too
//       if (almost_valid && (0 == cfg_holder)) { cfg_holder = Settings.cfg_holder; }  // At FB always active cfg_holder
//       valid = (cfg_holder == Settings.cfg_holder);
//     } else {


//       ESP.flashRead((flash_location -1) * SPI_FLASH_SEC_SIZE, (uint32*)&_SettingsH, sizeof(SYSCFGH));
//       valid = (Settings.cfg_holder == _SettingsH.cfg_holder);
//       if(Settings.cfg_holder == 65535){
//         // catch when memory is all 1's and not 0
//         valid = false;
//       }
//       AddLog(LOG_LEVEL_TEST,PSTR("flashRead ELSE %d %d %d"),valid,Settings.cfg_holder,_SettingsH.cfg_holder);
//     }
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " ESP.flashRead valid=%d"),valid);
//     if (valid) {
//       AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " Settings.save_flag > save_flag %d>%d"),Settings.save_flag,save_flag);
//       if (Settings.save_flag > save_flag) {
//         save_flag = Settings.save_flag;
//         settings_location = flash_location;
//         if (Settings.flag_system.stop_flash_rotate && (0 == i)) {  // Stop only if eeprom area should be used and it is valid
//           break;
//         }
//       }
//     }

//     delay(1);
//   }

// /*
// #else  // CFG_RESILIENT
//   // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
//   settings_location = 0;
//   uint32_t save_flag = 0;
//   uint32_t flash_location = SETTINGS_LOCATION;
//   for (uint32_t i = 0; i < CFG_ROTATES; i++) {              // Read all config pages in search of valid and latest
//     ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
//     if ((Settings.cfg_crc32 != 0xFFFFFFFF) && (Settings.cfg_crc32 != 0x00000000) && (Settings.cfg_crc32 == GetSettingsCrc32())) {
//       if (Settings.save_flag > save_flag) {                 // Find latest page based on incrementing save_flag
//         save_flag = Settings.save_flag;
//         settings_location = flash_location;
//         if (Settings.flag.stop_flash_rotate && (0 == i)) {  // Stop if only eeprom area should be used and it is valid
//           break;
//         }
//       }
//     }
//     flash_location--;
//     delay(1);
//   }
//   */
  
//   if (settings_location > 0) {
//     ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu" D_BOOT_COUNT " %d"), settings_location, Settings.save_flag, Settings.bootcount);
//   }
//   // else{    
//   //   AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD " %s>%d"), "settings_location",settings_location);
//   // }

//   #ifndef FIRMWARE_MINIMAL
//   if (!settings_location || (Settings.cfg_holder != (uint16_t)SETTINGS_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in .h
//     //Settings.seriallog_level = LOG_LEVEL_ALL;
//     pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
    
//     AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD "cfg_holder(%d) != SETTINGS_HOLDER(%d), Erasing settings"),Settings.cfg_holder,SETTINGS_HOLDER);

//     // Clear system defaults
//     SettingsDefault();

//   }
//   settings_crc = GetSettingsCrc();
//   #endif  // FIRMWARE_MINIMAL

//   RtcSettingsLoad();
  
// }


// // uint16_t mSettings::GetSettingsCrc(void)
// // {
// //   uint16_t crc = 0;
// //   uint8_t *bytes = (uint8_t*)&Settings;

// //   for (uint16_t i = 0; i < sizeof(SYSCFG); i++) {
// //     if ((i < 14) || (i > 15)) { crc += bytes[i]*(i+1); }  // Skip crc
// //   }
// //   return crc;
// // }


// void mSettings::SettingsSave(uint8_t rotate)
// { 

  

// // return ;
// // #endif

//   // Serial.print("SettingsSave> ");Serial.println(rotate);
//   /* Save configuration in eeprom or one of 7 slots below
//  *
//  * rotate 0 = Save in next flash slot
//  * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
//  * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
//  * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
//  * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
//  */
// //DEBUG_LINE_HERE;
// // #ifndef FIRMWARE_MINIMAL
//   if ((GetSettingsCrc() != settings_crc) || rotate) {
// //DEBUG_LINE_HERE;
//     if (1 == rotate) {   // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
//       stop_flash_rotate = 1;
//       //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " stop_flash_rotate"));//(upgrade) Use eeprom flash slot only and disable flash rotate from now on"));
//     }
//     if (2 == rotate) {   // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
//       settings_location = SETTINGS_LOCATION +1;
//       //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " (default) Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off(%d) (default)"),stop_flash_rotate);
//     }
//     if (stop_flash_rotate) {
//       settings_location = SETTINGS_LOCATION;
//     } else {
//       settings_location--;
//       //AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " settings_location=%d"),settings_location);
//       if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
//         settings_location = SETTINGS_LOCATION;
//       //AddLog(LOG_LEVEL_TEST,PSTR("settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)"));
//       }
//     }
//     Settings.save_flag++;
//     Settings.cfg_size = sizeof(SYSCFG);
//     Settings.cfg_crc = GetSettingsCrc();

// //DEBUG_LINE_HERE;
// // #ifdef USE_EEPROM
// //     if (SPIFFS_END == settings_location) {
// //       uint8_t* flash_buffer;
// //       flash_buffer = new uint8_t[SPI_FLASH_SEC_SIZE];
// //       if (eeprom_data && eeprom_size) {
// //         size_t flash_offset = SPI_FLASH_SEC_SIZE - eeprom_size;
// //         memcpy(flash_buffer + flash_offset, eeprom_data, eeprom_size);  // Write dirty EEPROM data
// //       } else {
// //         ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)flash_buffer, SPI_FLASH_SEC_SIZE);   // Read EEPROM area
// //       }
// //       memcpy(flash_buffer, &Settings, sizeof(Settings));
// //       ESP.flashEraseSector(settings_location);
// //       ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)flash_buffer, SPI_FLASH_SEC_SIZE);
// //       delete[] flash_buffer;
// //     } else {
// //       ESP.flashEraseSector(settings_location);
// //       ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
// //     }
// // #else

// // ESP.wdtFeed();
// // ESP.wdtDisable();
// //DEBUG_LINE_HERE;
//     // settings_location = SETTINGS_LOCATION;
// //     AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE " settings_location <= (SETTINGS_LOCATION - CFG_ROTATES) %lu"),settings_location);

// // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_SAVE " ENTERING UNSAFE AREA %d"),settings_location);

// // Serial.flush();
// // delay(1000);

// //DEBUG_LINE_HERE;

// #ifdef ENABLE_FLASH_ERASE_SECTOR_CURRENTLY_BUG
//   if (ESP.flashEraseSector(settings_location)) {
//     ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(Settings));
//   }
// #endif

// // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_MEMORY D_SAVE " LEAVING UNSAFE AREA %d"),settings_location);
// // ESP.wdtFeed();
// // delay(1000);

//     // ESP.flashEraseSector(settings_location);
// // //DEBUG_LINE_HERE;
// //     ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)&Settings, sizeof(SYSCFG));
// //DEBUG_LINE_HERE;
// // #endif  // USE_EEPROM

//     if (!stop_flash_rotate && rotate) {
//       for (uint8_t i = 1; i < CFG_ROTATES; i++) {
// //DEBUG_LINE_HERE;
//         ESP.flashEraseSector(settings_location -i);  // Delete previous configurations by resetting to 0xFF
//         delay(1);
//       }
//     }

//     AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_MEMORY D_SAVE D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings.save_flag, sizeof(SYSCFG));

//     settings_crc = Settings.cfg_crc;
//   }
// // #endif  // FIRMWARE_MINIMAL
// //DEBUG_LINE_HERE;
//   RtcSettingsSave();

// }



// #endif // ENABLE_DEVFEATURE_SETTINGS_V2



uint32_t mSettings::GetSettingsAddress(void)
{
  #ifdef ESP8266
  return settings_location * SPI_FLASH_SEC_SIZE;
  #else
  return 0;
  #endif
}

void mSettings::SettingsErase(uint8_t type)
{
  /*
    0 = Erase from program end until end of physical flash
    1 = Erase SDK parameter area at end of linker memory model (0x0FDxxx - 0x0FFFFF) solving possible wifi errors
  */

#ifndef FIRMWARE_MINIMAL
  bool result;

  uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
  uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;
  if (1 == type) {
    _sectorStart = SETTINGS_LOCATION +2;  // SDK parameter area above EEPROM area (0x0FDxxx - 0x0FFFFF)
    _sectorEnd = SETTINGS_LOCATION +5;
  }

  bool _serialoutput = (LOG_LEVEL_DEBUG_MORE <= seriallog_level);

    #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " %d " D_UNIT_SECTORS), _sectorEnd - _sectorStart);
    #endif// ENABLE_LOG_LEVEL_INFO

  for (uint32_t _sector = _sectorStart; _sector < _sectorEnd; _sector++) {
    result = ESP.flashEraseSector(_sector);
    if (_serialoutput) {
      Serial.print(F(D_LOG_APPLICATION D_ERASED_SECTOR " "));
      Serial.print(_sector);
      if (result) {
        Serial.println(F(" " D_OK));
      } else {
        Serial.println(F(" " D_ERROR));
      }
      delay(10);
    }
    //OsWatchLoop();
  }
#endif  // FIRMWARE_MINIMAL
}

// Copied from 2.4.0 as 2.3.0 is incomplete
bool mSettings::SettingsEraseConfig(void) 
{
  const size_t cfgSize = 0x4000;
  size_t cfgAddr = ESP.getFlashChipSize() - cfgSize;

  for (size_t offset = 0; offset < cfgSize; offset += SPI_FLASH_SEC_SIZE) {
    if (!ESP.flashEraseSector((cfgAddr + offset) / SPI_FLASH_SEC_SIZE)) {
      return false;
    }
  }
  return true;
}

void mSettings::SettingsSdkErase(void)
{
//   WiFi.disconnect(true);    // Delete SDK wifi config
//   SettingsErase(1);
//   SettingsEraseConfig();
//   delay(1000);
}

