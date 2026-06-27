/**
 * @file mRtcMemory.cpp
 * @brief RTC retained-memory quick-state support.
 * @version 0.2
 * @date 2026-05-20
 *
 * @note This module does not control the real-time clock.
 *       It only uses RTC-retained memory for small data that should survive
 *       warm restart, watchdog reset, crash reboot, and deep-sleep wake.
 */
#include "RtcMemory.h"

const uint16_t RTC_MEM_VALID = 0xA55A; // Marker only. Value does not matter as long as it is stable.

#ifdef ENABLE_FEATURE_FASTBOOT__DETECTION

RtcMemoryBootState_t RtcMemory__BootState;

#if defined(ESP32)
RTC_NOINIT_ATTR RtcMemoryBootState_t RtcMemoryData__BootState;
#endif

uint32_t rtc_memory_boot_state_crc = 0;

uint32_t RtcMemory__BootState_CRC(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcMemory__BootState;

  for (uint32_t i = 0; i < sizeof(RtcMemory__BootState); i++) {
    crc += bytes[i] * (i + 1);
  }

  return crc;
}

void RtcMemory__BootState_Save(void)
{
  if (RtcMemory__BootState_CRC() != rtc_memory_boot_state_crc) // ie if the data has changed at all
  {
    #ifdef DEBUG_FASTBOOT
    Serial.println("RtcMemory__BootState_Save: new data");
    #endif

    RtcMemory__BootState.valid = RTC_MEM_VALID;

    #ifdef ESP8266
      ESP.rtcUserMemoryWrite(100 - sizeof(RtcMemory__BootState), (uint32_t*)&RtcMemory__BootState, sizeof(RtcMemory__BootState));
    #endif

    #if defined(ESP32)
      // Works for ESP32, ESP32-S2/S3, ESP32-C3, etc via RTC_NOINIT_ATTR storage.
      RtcMemoryData__BootState = RtcMemory__BootState;
    #endif
  }

  rtc_memory_boot_state_crc = RtcMemory__BootState_CRC();

  #ifdef DEBUG_FASTBOOT
  Serial.println("RtcMemory__BootState_Save DONE");
  #endif
}

/**
 * @brief Used to ensure the next reboot does not continue with previous fastboot count.
 */
void RtcMemory__BootState_Reset(void)
{
  RtcMemory__BootState.fast_reboot_count = 0;
  RtcMemory__BootState_Save();
}

void RtcMemory__BootState_Load(void)
{
  #ifdef DEBUG_FASTBOOT
  Serial.println("RtcMemory__BootState_Load START");
  #endif

  #ifdef ESP8266
    ESP.rtcUserMemoryRead(100 - sizeof(RtcMemory__BootState), (uint32_t*)&RtcMemory__BootState, sizeof(RtcMemory__BootState));
  #endif

  #if defined(ESP32)
    // Works for ESP32, ESP32-S2/S3, ESP32-C3, etc via RTC_NOINIT_ATTR storage.
    RtcMemory__BootState = RtcMemoryData__BootState;
  #endif

  if (RtcMemory__BootState.valid != RTC_MEM_VALID)
  {
    #ifdef DEBUG_FASTBOOT
    Serial.println("RtcMemory__BootState.valid != RTC_MEM_VALID");
    #endif

    memset(&RtcMemory__BootState, 0, sizeof(RtcMemory__BootState));
    RtcMemory__BootState.valid = RTC_MEM_VALID;

    RtcMemory__BootState_Save();
  }

  rtc_memory_boot_state_crc = RtcMemory__BootState_CRC();

  #ifdef DEBUG_FASTBOOT
  Serial.println("RtcMemory__BootState_Load DONE");
  #endif
}

/**
 * @brief Check if the boot-state data contains the known RTC memory marker.
 *
 * @return true  Data appears valid.
 * @return false Data is not valid and should be initialised.
 */
bool RtcMemory__BootState_Valid(void)
{
  #ifdef DEBUG_FASTBOOT
  Serial.printf("RtcMemory__BootState_Valid=%d\n\r", RTC_MEM_VALID == RtcMemory__BootState.valid);
  #endif

  return (RTC_MEM_VALID == RtcMemory__BootState.valid);
}

#endif // ENABLE_FEATURE_FASTBOOT__DETECTION

/***********************************************************************************************************************************************************
 * Runtime RTC memory state
 **********************************************************************************************************************************************************/

#ifdef ENABLE_FEATURE_RTC__SETTINGS

RtcMemoryRuntimeState_t RtcMemory__RuntimeState;

#if defined(ESP32)
RTC_NOINIT_ATTR RtcMemoryRuntimeState_t RtcMemoryData__RuntimeState;
#endif

uint32_t rtc_memory_runtime_state_crc = 0;

uint32_t RtcMemory__RuntimeState_CRC(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcMemory__RuntimeState;

  for (uint32_t i = 0; i < sizeof(RtcMemory__RuntimeState); i++) {
    crc += bytes[i] * (i + 1);
  }

  return crc;
}

/**
 * @brief Not threadsafe, only called after device has been successfully started.
 */
void RtcMemory__RuntimeState_Save(void)
{
  // Check at least the first module has been initialised.
  // Change to a dedicated runtime-ready function later.
  if (!tkr->pModule.size()) { return; }

  RtcMemory__RuntimeState.baudrate = tkr_set->Settings.baudrate * 300;

  if (tkr_time->GetUTCTime() > START_VALID_UTC_TIME) {  // 2016-01-01
    RtcMemory__RuntimeState.utc_time = tkr_time->GetUTCTime();
  }

  if (RtcMemory__RuntimeState_CRC() != rtc_memory_runtime_state_crc) {

    if (RTC_MEM_VALID != RtcMemory__RuntimeState.valid) {
      memset(&RtcMemory__RuntimeState, 0, sizeof(RtcMemory__RuntimeState));
      RtcMemory__RuntimeState.valid = RTC_MEM_VALID;
      // RtcMemory__RuntimeState.energy_kWhtoday = Settings->energy_kWhtoday;
      // RtcMemory__RuntimeState.energy_kWhtotal = Settings->energy_kWhtotal;
      // for (uint32_t i = 0; i < 3; i++) {
      //   RtcMemory__RuntimeState.energy_kWhtoday_ph[i] = Settings->energy_kWhtoday_ph[i];
      //   RtcMemory__RuntimeState.energy_kWhtotal_ph[i] = Settings->energy_kWhtotal_ph[i];
      //   RtcMemory__RuntimeState.energy_kWhexport_ph[i] = Settings->energy_kWhexport_ph[i];
      // }
      // RtcMemory__RuntimeState.energy_usage = Settings->energy_usage;
      // for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
      //   RtcMemory__RuntimeState.pulse_counter[i] = Settings->pulse_counter[i];
      // }
      // RtcMemory__RuntimeState.power = Settings->power;
      RtcMemory__RuntimeState.baudrate = APP_BAUDRATE;
    }

    #ifdef ESP8266
      ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcMemory__RuntimeState, sizeof(RtcMemory__RuntimeState));
    #endif

    #if defined(ESP32)
      RtcMemoryData__RuntimeState = RtcMemory__RuntimeState;
    #endif

    rtc_memory_runtime_state_crc = RtcMemory__RuntimeState_CRC();
  }
}

bool RtcMemory__RuntimeState_Load(uint32_t update)
{
  #ifdef ESP8266
    ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcMemory__RuntimeState, sizeof(RtcMemory__RuntimeState));
  #endif

  #if defined(ESP32)
    RtcMemory__RuntimeState = RtcMemoryData__RuntimeState;
  #endif

  bool read_valid = (RTC_MEM_VALID == RtcMemory__RuntimeState.valid);

  if (update) {
    if (!read_valid) {
      RtcMemory__RuntimeState_Save();
    }
  }

  return read_valid;
}

bool RtcMemory__RuntimeState_Valid(void)
{
  return (RTC_MEM_VALID == RtcMemory__RuntimeState.valid);
}

#endif // ENABLE_FEATURE_RTC__SETTINGS
