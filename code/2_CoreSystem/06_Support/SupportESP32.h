
#ifndef _SupportESP32_H_
#define _SupportESP32_H_

#ifdef ESP32
/*********************************************************************************************\
 * ESP32, ESP32-S2, ESP32-S3, ESP32-C2, ESP32-C3, ESP32-C6 and ESP32-H2 Support
\*********************************************************************************************/

//                                                   11b 11g 11n  11n  11ax
// const static char kWifiPhyMode[] PROGMEM = "low rate|11b|11g|HT20|HT40|HE20"; // Wi-Fi Modes

#include "stdint.h"
#include <Arduino.h>

// #include "bootloader_flash.h"
#include "soc/soc.h"
#include "soc/spi_reg.h"
/// ESP32_ARCH contains the name of the architecture (used by autoconf)
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
#elif CONFIG_IDF_TARGET_ESP32H2
  #define ESP32_ARCH              "esp32h2"
#else
  #define ESP32_ARCH              ""
#endif

// Handle 20k of NVM

#include <nvs.h>

// See libraries\ESP32\examples\ResetReason.ino
// #include "esp_chip_info.h"
// #if CONFIG_IDF_TARGET_ESP32      // ESP32/PICO-D4
//   #include "esp32/rom/rtc.h"
// #elif CONFIG_IDF_TARGET_ESP32S2  // ESP32-S2
//   #include "esp32s2/rom/rtc.h"
// #elif CONFIG_IDF_TARGET_ESP32S3  // ESP32-S3
//   #include "esp32s3/rom/rtc.h"
// #elif CONFIG_IDF_TARGET_ESP32C2  // ESP32-C2
//   #include "esp32c2/rom/rtc.h"
// #elif CONFIG_IDF_TARGET_ESP32C3  // ESP32-C3
//   #include "esp32c3/rom/rtc.h"
// #elif CONFIG_IDF_TARGET_ESP32C6  // ESP32-C6
//   #include "esp32c6/rom/rtc.h"
// #elif CONFIG_IDF_TARGET_ESP32H2  // ESP32-H2
//   #include "esp32h2/rom/rtc.h"
// #else
//   #error Target CONFIG_IDF_TARGET is not supported
// #endif
// See libraries\ESP32\examples\ResetReason.ino
#if ESP_IDF_VERSION_MAJOR > 3      // IDF 4+
  #if CONFIG_IDF_TARGET_ESP32      // ESP32/PICO-D4
    #include "esp32/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32S2  // ESP32-S2
    #include "esp32s2/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32S3  // ESP32-S3
    #include "esp32s3/rom/rtc.h"
  #elif CONFIG_IDF_TARGET_ESP32C3  // ESP32-C3
    #include "esp32c3/rom/rtc.h"
  #else
    #error Target CONFIG_IDF_TARGET is not supported
  #endif
#else // ESP32 Before IDF 4.0
  #include "rom/rtc.h"
#endif

// Set the Stacksize for Arduino core. Default is 8192, some builds may need a bigger one
// size_t getArduinoLoopTaskStackSize(void) {
//   return SET_ESP32_STACK_SIZE;
// }

//
// Flash memory mapping
//

// See Esp.cpp
#include "Esp.h"
#include "esp_spi_flash.h"
#include <memory>
#include <soc/soc.h>
#include <soc/efuse_reg.h>
#include <esp_partition.h>
extern "C" {
#include "esp_ota_ops.h"
#include "esp_image_format.h"
}
#include "esp_system.h"
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
  #elif CONFIG_IDF_TARGET_ESP32C3   // ESP32-C3
    #include "esp32c3/rom/spi_flash.h"
    #define ESP_FLASH_IMAGE_BASE 0x0000     // Esp32c3 is located at 0x0000
  #else
    #error Target CONFIG_IDF_TARGET is not supported
  #endif
#else // ESP32 Before IDF 4.0
  #include "rom/spi_flash.h"
  #define ESP_FLASH_IMAGE_BASE 0x1000
#endif

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


#include <esp_phy_init.h>

// Handle 20k of NVM

#include <nvs.h>


class SupportESP32{
  public:
    SupportESP32(){};
    String GetDeviceHardware(void);
    
    void init(void);

    
    static uint32_t ESP_getChipId(void);
    
    static bool FoundPSRAM(void);

    static void ESP_Restart(void) {
     ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
      // ESP.reset();
    }
    
// extern "C" {
//   bool esp_spiram_is_initialized(void);
// }

// // this function is a replacement for `psramFound()`.
// // `psramFound()` can return true even if no PSRAM is actually installed
// // This new version also checks `esp_spiram_is_initialized` to know if the PSRAM is initialized
// bool FoundPSRAM(void) {
// #if CONFIG_IDF_TARGET_ESP32C3
//   return psramFound();
// #else
//   return psramFound() && esp_spiram_is_initialized();
// #endif
// }

// // new function to check whether PSRAM is present and supported (i.e. required pacthes are present)
// bool UsePSRAM(void) {
//   static bool can_use_psram = CanUsePSRAM();
//   return FoundPSRAM() && can_use_psram;
// }

// void *special_malloc(uint32_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_malloc(size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return malloc(size);
//   }
// }
// void *special_realloc(void *ptr, size_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_realloc(ptr, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return realloc(ptr, size);
//   }
// }
// void *special_calloc(size_t num, size_t size) {
//   if (UsePSRAM()) {
//     return heap_caps_calloc(num, size, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
//   } else {
//     return calloc(num, size);
//   }
// }

// // Variants for IRAM heap, which need all accesses to be 32 bits aligned
// void *special_malloc32(uint32_t size) {
//   return heap_caps_malloc(size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }
// void *special_realloc32(void *ptr, size_t size) {
//   return heap_caps_realloc(ptr, size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }
// void *special_calloc32(size_t num, size_t size) {
//   return heap_caps_calloc(num, size, UsePSRAM() ? MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT : MALLOC_CAP_32BIT);
// }






};



#endif

#endif  // _SONOFF_H_
//#endif



// #ifndef support_esp32_H
// #define support_esp32_H


// #include <stdint.h>


// // #ifdef ESP8266

// // extern "C" {
// // extern struct rst_info resetInfo;
// // }

// // uint32_t ESP_ResetInfoReason(void) {
// //   return resetInfo.reason;
// // }

// // String ESP_getResetReason(void) {
// //   return ESP.getResetReason();
// // }

// // uint32_t ESP_getChipId(void) {
// //   return ESP.getChipId();
// // }

// // uint32_t ESP_getSketchSize(void) {
// //   return ESP.getSketchSize();
// // }

// // uint32_t ESP_getFreeHeap(void) {
// //   return ESP.getFreeHeap();
// // }

// // uint32_t ESP_getMaxAllocHeap(void) {
// // /*
// //   From libraries.rst
// //   ESP.getMaxFreeBlockSize() returns the largest contiguous free RAM block in
// //   the heap, useful for checking heap fragmentation.  **NOTE:** Maximum
// //   ``malloc()``able block will be smaller due to memory manager overheads.

// //   From HeapMetric.ino
// //   ESP.getMaxFreeBlockSize() does not indicate the amount of memory that is
// //   available for use in a single malloc call.  It indicates the size of a
// //   contiguous block of (raw) memory before the umm_malloc overhead is removed.

// //   It should also be pointed out that, if you allow for the needed overhead in
// //   your malloc call, it could still fail in the general case. An IRQ handler
// //   could have allocated memory between the time you call
// //   ESP.getMaxFreeBlockSize() and your malloc call, reducing the available
// //   memory.
// // */
// //   uint32_t free_block_size = ESP.getMaxFreeBlockSize();
// //   if (free_block_size > 100) { free_block_size -= 100; }
// //   return free_block_size;
// // }


// // #endif



// #ifdef ESP32

// // // Handle 20k of NVM

// // #include <nvs.h>
// // #include <rom/rtc.h>

// // void NvmLoad(const char *sNvsName, const char *sName, void *pSettings, unsigned nSettingsLen) {
// //   nvs_handle handle;
// //   noInterrupts();
// //   nvs_open(sNvsName, NVS_READONLY, &handle);
// //   size_t size = nSettingsLen;
// //   nvs_get_blob(handle, sName, pSettings, &size);
// //   nvs_close(handle);
// //   interrupts();
// // }

// // void NvmSave(const char *sNvsName, const char *sName, const void *pSettings, unsigned nSettingsLen) {
// //   nvs_handle handle;
// //   noInterrupts();
// //   nvs_open(sNvsName, NVS_READWRITE, &handle);
// //   nvs_set_blob(handle, sName, pSettings, nSettingsLen);
// //   nvs_commit(handle);
// //   nvs_close(handle);
// //   interrupts();
// // }

// // void NvmErase(const char *sNvsName) {
// //   nvs_handle handle;
// //   noInterrupts();
// //   nvs_open(sNvsName, NVS_READWRITE, &handle);
// //   nvs_erase_all(handle);
// //   nvs_commit(handle);
// //   nvs_close(handle);
// //   interrupts();
// // }

// // void SettingsErase(uint8_t type) {
// //   if (1 == type) {         // SDK parameter area
// //   } else if (2 == type) {  // Tasmota parameter area (0x0F3xxx - 0x0FBFFF)
// //   } else if (3 == type) {  // Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
// //   }

// //   NvmErase("main");

// //   AddLog_P2(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_ERASE " t=%d"), type);
// // }

// // void SettingsRead(void *data, size_t size) {
// //   NvmLoad("main", "Settings", data, size);
// // }

// // void SettingsWrite(const void *pSettings, unsigned nSettingsLen) {
// //   NvmSave("main", "Settings", pSettings, nSettingsLen);
// // }

// // void QPCRead(void *pSettings, unsigned nSettingsLen) {
// //   NvmLoad("qpc", "pcreg", pSettings, nSettingsLen);
// // }

// // void QPCWrite(const void *pSettings, unsigned nSettingsLen) {
// //   NvmSave("qpc", "pcreg", pSettings, nSettingsLen);
// // }

// // void ZigbeeErase(void) {
// //   NvmErase("zb");
// // }

// // void ZigbeeRead(void *pSettings, unsigned nSettingsLen) {
// //   NvmLoad("zb", "zigbee", pSettings, nSettingsLen);
// // }

// // void ZigbeeWrite(const void *pSettings, unsigned nSettingsLen) {
// //   NvmSave("zb", "zigbee", pSettings, nSettingsLen);
// // }

// // //
// // // sntp emulation
// // //
// // static bool bNetIsTimeSync = false;
// // //
// // void SntpInit() {
// //   bNetIsTimeSync = true;
// // }

// // uint32_t SntpGetCurrentTimestamp(void) {
// //   time_t now = 0;
// //   if (bNetIsTimeSync || ntp_force_sync)
// //   {
// //     //Serial_DebugX(("timesync configTime %d\n", ntp_force_sync, bNetIsTimeSync));
// //     // init to UTC Time
// //     configTime(0, 0, SettingsText(SET_NTPSERVER1), SettingsText(SET_NTPSERVER2), SettingsText(SET_NTPSERVER3));
// //     bNetIsTimeSync = false;
// //     ntp_force_sync = false;
// //   }
// //   time(&now);
// //   return now;
// // }

// // //
// // // Crash stuff
// // //

// // void CrashDump(void) {
// // }

// // bool CrashFlag(void) {
// //   return false;
// // }

// // void CrashDumpClear(void) {
// // }

// // void CmndCrash(void) {
// //   /*
// //   volatile uint32_t dummy;
// //   dummy = *((uint32_t*) 0x00000000);
// // */
// // }

// // // Do an infinite loop to trigger WDT watchdog
// // void CmndWDT(void) {
// //   /*
// //   volatile uint32_t dummy = 0;
// //   while (1) {
// //     dummy++;
// //   }
// // */
// // }
// // // This will trigger the os watch after OSWATCH_RESET_TIME (=120) seconds
// // void CmndBlockedLoop(void) {
// //   /*
// //   while (1) {
// //     delay(1000);
// //   }
// // */
// // }

// // //
// // // ESP32 specific
// // //

// // #include "soc/soc.h"
// // #include "soc/rtc_cntl_reg.h"

// // void DisableBrownout(void) {
// //   // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
// //   WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
// // }

// // //
// // // ESP32 Alternatives
// // //

// // String ESP32GetResetReason(uint32_t cpu_no) {
// // 	// tools\sdk\include\esp32\rom\rtc.h
// //   switch (rtc_get_reset_reason( (RESET_REASON) cpu_no)) {
// //     case POWERON_RESET          : return F("Vbat power on reset");                              // 1
// //     case SW_RESET               : return F("Software reset digital core");                      // 3
// //     case OWDT_RESET             : return F("Legacy watch dog reset digital core");              // 4
// //     case DEEPSLEEP_RESET        : return F("Deep Sleep reset digital core");                    // 5
// //     case SDIO_RESET             : return F("Reset by SLC module, reset digital core");          // 6
// //     case TG0WDT_SYS_RESET       : return F("Timer Group0 Watch dog reset digital core");        // 7
// //     case TG1WDT_SYS_RESET       : return F("Timer Group1 Watch dog reset digital core");        // 8
// //     case RTCWDT_SYS_RESET       : return F("RTC Watch dog Reset digital core");                 // 9
// //     case INTRUSION_RESET        : return F("Instrusion tested to reset CPU");                   // 10
// //     case TGWDT_CPU_RESET        : return F("Time Group reset CPU");                             // 11
// //     case SW_CPU_RESET           : return F("Software reset CPU");                               // 12
// //     case RTCWDT_CPU_RESET       : return F("RTC Watch dog Reset CPU");                          // 13
// //     case EXT_CPU_RESET          : return F("or APP CPU, reseted by PRO CPU");                   // 14
// //     case RTCWDT_BROWN_OUT_RESET : return F("Reset when the vdd voltage is not stable");         // 15
// //     case RTCWDT_RTC_RESET       : return F("RTC Watch dog reset digital core and rtc module");  // 16
// //     default                     : return F("NO_MEAN");                                          // 0
// //   }
// // }

// // String ESP_getResetReason(void) {
// //   return ESP32GetResetReason(0);  // CPU 0
// // }

// // uint32_t ESP_ResetInfoReason(void) {
// //   RESET_REASON reason = rtc_get_reset_reason(0);
// //   if (POWERON_RESET == reason) { return REASON_DEFAULT_RST; }
// //   if (SW_CPU_RESET == reason) { return REASON_SOFT_RESTART; }
// //   if (DEEPSLEEP_RESET == reason)  { return REASON_DEEP_SLEEP_AWAKE; }
// //   if (SW_RESET == reason) { return REASON_EXT_SYS_RST; }
// // }


// // uint32_t ESP_getSketchSize(void) {
// //   static uint32_t sketchsize = 0;

// //   if (!sketchsize) {
// //     sketchsize = ESP.getSketchSize();  // This takes almost 2 seconds on an ESP32
// //   }
// //   return sketchsize;
// // }

// // uint32_t ESP_getFreeHeap(void) {
// // //  return ESP.getFreeHeap();
// //   return ESP.getMaxAllocHeap();
// // }

// // uint32_t ESP_getMaxAllocHeap(void) {
// //   // largest block of heap that can be allocated at once
// //   uint32_t free_block_size = ESP.getMaxAllocHeap();
// //   if (free_block_size > 100) { free_block_size -= 100; }
// //   return free_block_size;
// // }

// // void ESP_Restart(void) {
// //   ESP.restart();
// // }

// #endif  // ESP32


// #endif // support_esp32
//  * 
//  * */