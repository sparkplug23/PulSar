/**
 * @file mRtcMemory.cpp
 * @brief RTC retained-memory quick-state support.
 * @version 0.3
 * @date 2026-08-19
 *
 * @note This module does not control the real-time clock.
 *       It only uses RTC-retained memory for small data that should survive
 *       warm restart, watchdog reset, crash reboot, and deep-sleep wake.
 */

#include "RtcMemory.h"

const uint16_t RTC_MEM_VALID = 0xA55A;
const uint16_t RTC_WIFI_QUICK_CONNECT_VALID = 0x51A7;


/***********************************************************************************************************************************************************
 * Boot RTC memory state
 **********************************************************************************************************************************************************/

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

  for(uint32_t i=0;i<sizeof(RtcMemory__BootState);i++)
  {
    crc += bytes[i] * (i + 1);
  }

  return crc;
}


void RtcMemory__BootState_Save(void)
{
  if(RtcMemory__BootState_CRC() != rtc_memory_boot_state_crc)
  {
    #ifdef DEBUG_FASTBOOT
    Serial.println("RtcMemory__BootState_Save: new data");
    #endif

    RtcMemory__BootState.valid = RTC_MEM_VALID;

    #ifdef ESP8266
    ESP.rtcUserMemoryWrite(100 - sizeof(RtcMemory__BootState),(uint32_t*)&RtcMemory__BootState,sizeof(RtcMemory__BootState));
    #endif

    #if defined(ESP32)
    RtcMemoryData__BootState = RtcMemory__BootState;
    #endif
  }

  rtc_memory_boot_state_crc = RtcMemory__BootState_CRC();

  #ifdef DEBUG_FASTBOOT
  Serial.println("RtcMemory__BootState_Save DONE");
  #endif
}


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
  ESP.rtcUserMemoryRead(100 - sizeof(RtcMemory__BootState),(uint32_t*)&RtcMemory__BootState,sizeof(RtcMemory__BootState));
  #endif

  #if defined(ESP32)
  RtcMemory__BootState = RtcMemoryData__BootState;
  #endif

  if(RtcMemory__BootState.valid != RTC_MEM_VALID)
  {
    #ifdef DEBUG_FASTBOOT
    Serial.println("RtcMemory__BootState.valid != RTC_MEM_VALID");
    #endif

    memset(&RtcMemory__BootState,0,sizeof(RtcMemory__BootState));
    RtcMemory__BootState.valid = RTC_MEM_VALID;

    RtcMemory__BootState_Save();
  }

  rtc_memory_boot_state_crc = RtcMemory__BootState_CRC();

  #ifdef DEBUG_FASTBOOT
  Serial.println("RtcMemory__BootState_Load DONE");
  #endif
}


bool RtcMemory__BootState_Valid(void)
{
  #ifdef DEBUG_FASTBOOT
  Serial.printf("RtcMemory__BootState_Valid=%d\n\r",RTC_MEM_VALID == RtcMemory__BootState.valid);
  #endif

  return RTC_MEM_VALID == RtcMemory__BootState.valid;
}

#endif


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

  for(uint32_t i=0;i<sizeof(RtcMemory__RuntimeState);i++)
  {
    crc += bytes[i] * (i + 1);
  }

  return crc;
}


/**
 * @brief Directly write the already-prepared runtime structure.
 *
 * Unlike RtcMemory__RuntimeState_Save(), this does not inspect Tasker,
 * Settings, Time, or any other runtime module. It exists for very early boot
 * and pre-reboot quick-state operations.
 */
static void RtcMemory__RuntimeState_WriteRaw(void)
{
  RtcMemory__RuntimeState.valid = RTC_MEM_VALID;

  #ifdef ESP8266
  ESP.rtcUserMemoryWrite(100,(uint32_t*)&RtcMemory__RuntimeState,sizeof(RtcMemory__RuntimeState));
  #endif

  #if defined(ESP32)
  RtcMemoryData__RuntimeState = RtcMemory__RuntimeState;
  #endif

  rtc_memory_runtime_state_crc = RtcMemory__RuntimeState_CRC();
}


/**
 * @brief Not threadsafe, only called after device has been successfully started.
 */
void RtcMemory__RuntimeState_Save(void)
{
  if(!tkr->pModule.size()){ return; }

  if(RTC_MEM_VALID != RtcMemory__RuntimeState.valid)
  {
    memset(&RtcMemory__RuntimeState,0,sizeof(RtcMemory__RuntimeState));
    RtcMemory__RuntimeState.valid = RTC_MEM_VALID;
    RtcMemory__RuntimeState.baudrate = APP_BAUDRATE;
  }

  RtcMemory__RuntimeState.baudrate = tkr_set->Settings.baudrate * 300;

  if(tkr_time->GetUTCTime() > START_VALID_UTC_TIME)
  {
    RtcMemory__RuntimeState.utc_time = tkr_time->GetUTCTime();
  }

  if(RtcMemory__RuntimeState_CRC() != rtc_memory_runtime_state_crc)
  {
    RtcMemory__RuntimeState_WriteRaw();
  }
}


bool RtcMemory__RuntimeState_Load(uint32_t update)
{
  #ifdef ESP8266
  ESP.rtcUserMemoryRead(100,(uint32_t*)&RtcMemory__RuntimeState,sizeof(RtcMemory__RuntimeState));
  #endif

  #if defined(ESP32)
  RtcMemory__RuntimeState = RtcMemoryData__RuntimeState;
  #endif

  const bool read_valid = RTC_MEM_VALID == RtcMemory__RuntimeState.valid;

  if(update)
  {
    if(!read_valid)
    {
      RtcMemory__RuntimeState_Save();
    }
  }

  rtc_memory_runtime_state_crc = RtcMemory__RuntimeState_CRC();

  return read_valid;
}


bool RtcMemory__RuntimeState_Valid(void)
{
  return RTC_MEM_VALID == RtcMemory__RuntimeState.valid;
}


/***********************************************************************************************************************************************************
 * WiFi quick-connect RTC state
 **********************************************************************************************************************************************************/

bool RtcMemory__WiFiQuickConnect_Valid(void)
{
  if(RtcMemory__RuntimeState.valid != RTC_MEM_VALID)
  {
    return false;
  }

  if(RtcMemory__RuntimeState.boot_was_completed_ota_event == 0)
  {
    return false;
  }

  if(RtcMemory__RuntimeState.wifi_quick_connect_magic != RTC_WIFI_QUICK_CONNECT_VALID)
  {
    return false;
  }

  if(RtcMemory__RuntimeState.wifi_quick_connect_channel == 0)
  {
    return false;
  }

  bool bssid_valid = false;

  for(uint8_t i=0;i<6;i++)
  {
    if(RtcMemory__RuntimeState.wifi_quick_connect_bssid[i] != 0)
    {
      bssid_valid = true;
      break;
    }
  }

  return bssid_valid;
}


void RtcMemory__WiFiQuickConnect_Set(uint8_t profile, uint8_t channel, const uint8_t bssid[6])
{
  if(RtcMemory__RuntimeState.valid != RTC_MEM_VALID)
  {
    memset(&RtcMemory__RuntimeState,0,sizeof(RtcMemory__RuntimeState));
    RtcMemory__RuntimeState.valid = RTC_MEM_VALID;
  }

  RtcMemory__RuntimeState.boot_was_completed_ota_event = 1;

  RtcMemory__RuntimeState.wifi_quick_connect_profile = profile;
  RtcMemory__RuntimeState.wifi_quick_connect_channel = channel;
  RtcMemory__RuntimeState.wifi_quick_connect_reserved = 0;

  memcpy(RtcMemory__RuntimeState.wifi_quick_connect_bssid,bssid,6);

  RtcMemory__RuntimeState.wifi_quick_connect_magic = RTC_WIFI_QUICK_CONNECT_VALID;

  RtcMemory__RuntimeState_WriteRaw();
}


void RtcMemory__WiFiQuickConnect_Clear(void)
{
  if(RtcMemory__RuntimeState.valid != RTC_MEM_VALID)
  {
    return;
  }

  RtcMemory__RuntimeState.boot_was_completed_ota_event = 0;

  RtcMemory__RuntimeState.wifi_quick_connect_profile = 0;
  RtcMemory__RuntimeState.wifi_quick_connect_channel = 0;
  RtcMemory__RuntimeState.wifi_quick_connect_reserved = 0;

  memset(RtcMemory__RuntimeState.wifi_quick_connect_bssid,0,6);

  RtcMemory__RuntimeState.wifi_quick_connect_magic = 0;

  RtcMemory__RuntimeState_WriteRaw();
}

#endif