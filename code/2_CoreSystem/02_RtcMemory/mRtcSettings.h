#ifndef _RTC_SETTINGS_H_
#define _RTC_SETTINGS_H_

#define D_UNIQUE_MODULE_CORE_RTCSETTINGS_ID ((2*1000)+02)

#include "2_CoreSystem/mBaseConfig.h"

#include "1_TaskerManager/mTaskerManager.h"
/**
 * @brief RTC Reboot/Fastboot data is made with global functions, so it is safe from TaskManager pointers and be processed immediately in setup()
 **/
#ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
typedef struct {
  uint16_t      valid;                     // 280  (RTC memory offset 100 - sizeof(RTCRBT))
  uint8_t       fast_reboot_count;         // 282
  uint8_t       free_003[1];               // 283
} TRtcFastboot;
extern TRtcFastboot RtcFastboot;
#ifdef ESP32
extern RTC_NOINIT_ATTR TRtcFastboot RtcDataFastboot;
#endif  // ESP32

extern const uint16_t RTC_MEM_VALID;
extern uint32_t rtc_fastboot_crc;
extern uint32_t RtcFastboot_CRC();
extern void     RtcFastboot_Save(void);
extern void     RtcFastboot_Reset(void);
extern void     RtcFastboot_Load(void);
extern bool     RtcFastboot_Valid(void);
#endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION

/**
 * @brief 
 **/
#ifdef ENABLE_DEVFEATURE_RTC_SETTINGS

typedef struct {
  uint16_t      valid;                     // 290  (RTC memory offset 100)
  uint8_t       oswatch_blocked_loop;      // 292
  uint8_t       ota_loader;                // 293
  uint32_t      energy_kWhtoday;           // 294
  uint32_t      energy_kWhtotal;           // 298
  // volatile uint32_t pulse_counter[MAX_COUNTERS];  // 29C - See #9521 why volatile
  // power_t       power;                     // 2AC
  // EnergyUsage   energy_usage;              // 2B0
  uint32_t      nextwakeup;                // 2C8
  uint32_t      baudrate;                  // 2CC
  uint32_t      ultradeepsleep;            // 2D0
  uint16_t      deepsleep_slip;            // 2D4
  uint8_t       improv_state;              // 2D6

  uint8_t       free_2d7[1];               // 2D7

  int32_t       energy_kWhtoday_ph[3];     // 2D8
  int32_t       energy_kWhtotal_ph[3];     // 2E4
  int32_t       energy_kWhexport_ph[3];    // 2F0
  uint32_t      utc_time;                  // 2FC
} TRtcSettings;
extern TRtcSettings RtcSettings;
#ifdef ESP32
extern RTC_NOINIT_ATTR TRtcSettings RtcDataSettings;
#endif  // ESP32

extern uint32_t rtc_settings_crc;
extern uint32_t GetRtcSettingsCrc(void);
extern void RtcSettingsSave(void);
extern bool RtcSettingsLoad(uint32_t update);
extern bool RtcSettingsValid(void);
#endif // ENABLE_DEVFEATURE_RTC_SETTINGS




#endif  // _RTC_SETTINGS_H_