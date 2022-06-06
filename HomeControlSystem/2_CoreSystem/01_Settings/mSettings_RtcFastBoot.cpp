
#include "2_CoreSystem/01_Settings/mSettings.h"



/*********************************************************************************************\
 * RTC memory v2 for Fastboot fixing
\*********************************************************************************************/

#ifdef ENABLE_DEVFEATURE_RTC_FASTBOOT_V2



const uint16_t RTC_MEM_VALID = 0xA55A;

uint32_t rtc_settings_crc = 0;

uint32_t mSettings::GetRtcSettingsCrc(void) {
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint32_t i = 0; i < sizeof(RtcSettings); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void mSettings::RtcSettingsSave(void) {
  RtcSettings.baudrate = Settings.baudrate * 300;
  if (GetRtcSettingsCrc() != rtc_settings_crc) {

    if (RTC_MEM_VALID != RtcSettings.valid) {
      memset(&RtcSettings, 0, sizeof(RtcSettings));
      RtcSettings.valid = RTC_MEM_VALID;
      // RtcSettings.energy_kWhtoday = Settings.energy_kWhtoday;
      // RtcSettings.energy_kWhtotal = Settings.energy_kWhtotal;
      // for (uint32_t i = 0; i < 3; i++) {
      //   RtcSettings.energy_kWhtoday_ph[i] = Settings.energy_kWhtoday_ph[i];
      //   RtcSettings.energy_kWhtotal_ph[i] = Settings.energy_kWhtotal_ph[i];
      // }
      // RtcSettings.energy_usage = Settings.energy_usage;
      // for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
      //   RtcSettings.pulse_counter[i] = Settings.pulse_counter[i];
      // }
      RtcSettings.power = Settings.power;
  //    RtcSettings.baudrate = Settings.baudrate * 300;
      RtcSettings.baudrate = APP_BAUDRATE;
    }

#ifdef ESP8266
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcSettings, sizeof(RtcSettings));
#endif  // ESP8266
#ifdef ESP32
    RtcDataSettings = RtcSettings;
#endif  // ESP32

    rtc_settings_crc = GetRtcSettingsCrc();
  }
}

bool mSettings::RtcSettingsLoad(uint32_t update) {
#ifdef ESP8266
  ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcSettings, sizeof(RtcSettings));  // 0x290
#endif  // ESP8266
#ifdef ESP32
  RtcSettings = RtcDataSettings;
#endif  // ESP32

  bool read_valid = (RTC_MEM_VALID == RtcSettings.valid);
  if (update) {
    if (!read_valid) {
      RtcSettingsSave();
    }
  }
  return read_valid;
}

bool mSettings::RtcSettingsValid(void) {
  return (RTC_MEM_VALID == RtcSettings.valid);
}

/********************************************************************************************/

uint32_t rtc_reboot_crc = 0;

uint32_t mSettings::GetRtcRebootCrc(void) {
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcReboot;

  for (uint32_t i = 0; i < sizeof(RtcReboot); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void mSettings::RtcRebootSave(void) {
  if (GetRtcRebootCrc() != rtc_reboot_crc) {
    RtcReboot.valid = RTC_MEM_VALID;
#ifdef ESP8266
    ESP.rtcUserMemoryWrite(100 - sizeof(RtcReboot), (uint32_t*)&RtcReboot, sizeof(RtcReboot));
#endif  // ESP8266
#ifdef ESP32
    RtcDataReboot = RtcReboot;
#endif  // ESP32
    rtc_reboot_crc = GetRtcRebootCrc();
  }
}

void mSettings::RtcRebootReset(void) {
  RtcReboot.fast_reboot_count = 0;
  RtcRebootSave();
}

void mSettings::RtcRebootLoad(void) 
{

#ifdef ESP8266
  ESP.rtcUserMemoryRead(100 - sizeof(RtcReboot), (uint32_t*)&RtcReboot, sizeof(RtcReboot));  // 0x280
#endif  // ESP8266
#ifdef ESP32
  RtcReboot = RtcDataReboot;
#endif  // ESP32

  if (RtcReboot.valid != RTC_MEM_VALID) {
    memset(&RtcReboot, 0, sizeof(RtcReboot));
    RtcReboot.valid = RTC_MEM_VALID;
//    RtcReboot.fast_reboot_count = 0;  // Explicit by memset
    RtcRebootSave();
  }
  rtc_reboot_crc = GetRtcRebootCrc();
}

bool mSettings::RtcRebootValid(void) {
  return (RTC_MEM_VALID == RtcReboot.valid);
}

#endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_V2


/**
 * @brief Old version of fastboot and rtc, phase out into new version above
 * 
 */
#ifndef ENABLE_DEVFEATURE_RTC_FASTBOOT_V2



uint32_t mSettings::GetRtcRebootCrc(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcReboot;

  for (uint16_t i = 0; i < sizeof(RTCRBT); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void mSettings::RtcRebootSave(void)
{
  #ifdef ESP8266
  if (GetRtcRebootCrc() != rtc_reboot_crc) {
    RtcReboot.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100 - sizeof(RTCRBT), (uint32_t*)&RtcReboot, sizeof(RTCRBT));
    rtc_reboot_crc = GetRtcRebootCrc();
  }
  #endif
}

void mSettings::RtcRebootLoad(void)
{
  #ifdef ESP8266

  ESP.rtcUserMemoryRead(100 - sizeof(RTCRBT), (uint32_t*)&RtcReboot, sizeof(RTCRBT));  // 0x280
  if (RtcReboot.valid != RTC_MEM_VALID) {
    memset(&RtcReboot, 0, sizeof(RTCRBT));
    RtcReboot.valid = RTC_MEM_VALID;
//    RtcReboot.fast_reboot_count = 0;  // Explicit by memset
    RtcRebootSave();
  }
  rtc_reboot_crc = GetRtcRebootCrc();
  DEBUG_PRINTF("fast_reboot=%d\n\r",RtcReboot.fast_reboot_count);
  #endif
}



/*********************************************************************************************\
 * RTC memory
\*********************************************************************************************/

uint32_t mSettings::GetRtcSettingsCrc(void)
{
  uint32_t crc = 0;
  uint8_t *bytes = (uint8_t*)&RtcSettings;

  for (uint32_t i = 0; i < sizeof(RTCMEM); i++) {
    crc += bytes[i]*(i+1);
  }
  return crc;
}

void mSettings::RtcSettingsSave(void)
{
  
  #ifdef ESP8266
  if (GetRtcSettingsCrc() != rtc_settings_crc) {
    RtcSettings.valid = RTC_MEM_VALID;
    ESP.rtcUserMemoryWrite(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));
    rtc_settings_crc = GetRtcSettingsCrc();
  }
  #endif// ESP8266
}

void mSettings::RtcSettingsLoad(void)
{
    #ifdef ESP8266
  ESP.rtcUserMemoryRead(100, (uint32_t*)&RtcSettings, sizeof(RTCMEM));  // 0x290
  if (RtcSettings.valid != RTC_MEM_VALID) {
    memset(&RtcSettings, 0, sizeof(RTCMEM));
    RtcSettings.valid = RTC_MEM_VALID;
    
  #ifdef USE_ENERGY
    RtcSettings.energy_kWhtoday = Settings.energy_usage.energy_kWhtoday;
    RtcSettings.energy_kWhtotal = Settings.energy_usage.energy_kWhtotal;
    RtcSettings.energy_usage = Settings.energy_usage;
    // for (uint32_t i = 0; i < MAX_COUNTERS; i++) {
    //   RtcSettings.pulse_counter[i] = Settings.pulse_counter[i];
    //}
    #endif
    RtcSettings.power = Settings.power;
    RtcSettingsSave();
  }
  rtc_settings_crc = GetRtcSettingsCrc();
    #endif // ESP8266
}

bool mSettings::RtcSettingsValid(void)
{
  return (RTC_MEM_VALID == RtcSettings.valid);
}

/********************************************************************************************/


void mSettings::RtcRebootReset(void)
{
  RtcReboot.fast_reboot_count = 0;
  RtcRebootSave();
}


bool mSettings::RtcRebootValid(void)
{
  return (RTC_MEM_VALID == RtcReboot.valid);
}



#endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_V2

