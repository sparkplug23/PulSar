#ifndef _RTC_MEMORY_H_
#define _RTC_MEMORY_H_

#define D_UNIQUE_MODULE_CORE_RTCMEMORY_ID 2002 // [(Folder_Number*100)+ID_File]

#include "2_CoreSystem/mBaseConfig.h"
#include "1_TaskerManager/mTaskerManager.h"

/*
 * RTC memory note:
 * This module does not control the real-time clock.
 * It only uses RTC-retained memory for small pieces of state that should
 * survive warm restart, watchdog reset, crash reboot, and deep-sleep wake.
 */

extern const uint16_t RTC_MEM_VALID;

/**
 * @brief Minimal boot-loop / fastboot state.
 *
 * This data is intentionally independent of TaskManager pointers so it can be
 * loaded very early in setup(), before the normal module tree is running.
 */
#ifdef ENABLE_FEATURE_FASTBOOT__DETECTION
typedef struct {
  uint16_t      valid;                     // ESP8266 RTC user memory offset: 100 - sizeof(RtcMemoryBootState_t)
  uint8_t       fast_reboot_count;
  uint8_t       boot_state;
} RtcMemoryBootState_t;

extern RtcMemoryBootState_t RtcMemory__BootState;

// To be added later with boot_state, to allow detection of pending safe mode, OTA recovery, crash recovery, etc.
#define RTC_BOOT_FLAG_SAFE_MODE_PENDING   0x01
#define RTC_BOOT_FLAG_OTA_RECOVERY        0x02
#define RTC_BOOT_FLAG_CRASH_RECORDED      0x04
#define RTC_BOOT_FLAG_FS_RECOVERY         0x08

#if defined(ESP32)
extern RTC_NOINIT_ATTR RtcMemoryBootState_t RtcMemoryData__BootState;
#endif

extern uint32_t rtc_memory_boot_state_crc;
extern uint32_t RtcMemory__BootState_CRC(void);
extern void     RtcMemory__BootState_Save(void);
extern void     RtcMemory__BootState_Reset(void);
extern void     RtcMemory__BootState_Load(void);
extern bool     RtcMemory__BootState_Valid(void);
#endif // ENABLE_FEATURE_FASTBOOT__DETECTION

/**
 * @brief Runtime quick-state retained in RTC memory.
 *
 * This is not configuration storage. It is for small, fast-changing runtime
 * state that is useful immediately after reboot, crash, watchdog reset, or
 * deep-sleep wake.
 */
#ifdef ENABLE_FEATURE_RTC__SETTINGS
typedef struct {
  uint16_t      valid;                     // ESP8266 RTC user memory offset: 100
  uint8_t       oswatch_blocked_loop;
  uint8_t       ota_loader;
  uint8_t       boot_was_completed_ota_event; // Set prior to OTA reset on success only, so next boot can skip WiFi delayed start if required.
  uint8_t       free_005[3];               // Explicit padding after uint8_t fields before uint32_t fields.

  uint32_t      energy_kWhtoday;
  uint32_t      energy_kWhtotal;
  // volatile uint32_t pulse_counter[MAX_COUNTERS];
  // power_t       power;
  // EnergyUsage   energy_usage;
  uint32_t      nextwakeup;
  uint32_t      baudrate;
  uint32_t      ultradeepsleep;
  uint16_t      deepsleep_slip;
  uint8_t       improv_state;
  uint8_t       free_02b[1];

  int32_t       energy_kWhtoday_ph[3];
  int32_t       energy_kWhtotal_ph[3];
  int32_t       energy_kWhexport_ph[3];
  uint32_t      utc_time;
} RtcMemoryRuntimeState_t;

extern RtcMemoryRuntimeState_t RtcMemory__RuntimeState;

#if defined(ESP32)
extern RTC_NOINIT_ATTR RtcMemoryRuntimeState_t RtcMemoryData__RuntimeState;
#endif

extern uint32_t rtc_memory_runtime_state_crc;
extern uint32_t RtcMemory__RuntimeState_CRC(void);
extern void     RtcMemory__RuntimeState_Save(void);
extern bool     RtcMemory__RuntimeState_Load(uint32_t update);
extern bool     RtcMemory__RuntimeState_Valid(void);
#endif // ENABLE_FEATURE_RTC__SETTINGS


#endif  // _RTC_MEMORY_H_
