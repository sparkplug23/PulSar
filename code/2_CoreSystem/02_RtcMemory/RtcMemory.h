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
extern const uint16_t RTC_WIFI_QUICK_CONNECT_VALID;


/**
 * @brief Minimal boot-loop / fastboot state.
 */
#ifdef ENABLE_FEATURE_FASTBOOT__DETECTION

typedef struct {
  uint16_t valid;
  uint8_t fast_reboot_count;
  uint8_t boot_state;
}RtcMemoryBootState_t;

extern RtcMemoryBootState_t RtcMemory__BootState;

#define RTC_BOOT_FLAG_SAFE_MODE_PENDING   0x01
#define RTC_BOOT_FLAG_OTA_RECOVERY        0x02
#define RTC_BOOT_FLAG_CRASH_RECORDED      0x04
#define RTC_BOOT_FLAG_FS_RECOVERY         0x08

#if defined(ESP32)
extern RTC_NOINIT_ATTR RtcMemoryBootState_t RtcMemoryData__BootState;
#endif

extern uint32_t rtc_memory_boot_state_crc;
extern uint32_t RtcMemory__BootState_CRC(void);
extern void RtcMemory__BootState_Save(void);
extern void RtcMemory__BootState_Reset(void);
extern void RtcMemory__BootState_Load(void);
extern bool RtcMemory__BootState_Valid(void);

#endif


/**
 * @brief Runtime quick-state retained in RTC memory.
 *
 * This is not configuration storage. It is for small, fast-changing runtime
 * state that is useful immediately after reboot, crash, watchdog reset,
 * deep-sleep wake, or a successful OTA restart.
 */
#ifdef ENABLE_FEATURE_RTC__SETTINGS

typedef struct {
  uint16_t valid;                       // ESP8266 RTC user memory offset: 100

  uint8_t oswatch_blocked_loop;
  uint8_t ota_loader;

  uint8_t boot_was_completed_ota_event;

  /*
   * WiFi OTA quick-connect.
   *
   * The dedicated magic value makes this safe when upgrading from an older
   * RTC structure which did not contain these fields.
   */
  uint8_t wifi_quick_connect_profile;
  uint8_t wifi_quick_connect_channel;
  uint8_t wifi_quick_connect_reserved;
  uint8_t wifi_quick_connect_bssid[6];
  uint16_t wifi_quick_connect_magic;

  uint32_t energy_kWhtoday;
  uint32_t energy_kWhtotal;

  uint32_t nextwakeup;
  uint32_t baudrate;
  uint32_t ultradeepsleep;

  uint16_t deepsleep_slip;
  uint8_t improv_state;
  uint8_t free_02b[1];

  int32_t energy_kWhtoday_ph[3];
  int32_t energy_kWhtotal_ph[3];
  int32_t energy_kWhexport_ph[3];

  uint32_t utc_time;
}RtcMemoryRuntimeState_t;

extern RtcMemoryRuntimeState_t RtcMemory__RuntimeState;

#if defined(ESP32)
extern RTC_NOINIT_ATTR RtcMemoryRuntimeState_t RtcMemoryData__RuntimeState;
#endif

extern uint32_t rtc_memory_runtime_state_crc;

extern uint32_t RtcMemory__RuntimeState_CRC(void);
extern void RtcMemory__RuntimeState_Save(void);
extern bool RtcMemory__RuntimeState_Load(uint32_t update);
extern bool RtcMemory__RuntimeState_Valid(void);

extern bool RtcMemory__WiFiQuickConnect_Valid(void);
extern void RtcMemory__WiFiQuickConnect_Set(uint8_t profile, uint8_t channel, const uint8_t bssid[6]);
extern void RtcMemory__WiFiQuickConnect_Clear(void);

#endif


#endif