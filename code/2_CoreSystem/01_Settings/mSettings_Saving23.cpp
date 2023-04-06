#include "mSettings.h"


/*********************************************************************************************\
 * ESP8266 Tasmota Flash usage offset from 0x40200000
 *
 * Tasmota 1M  Tasmota 2M  Tasmota 4M - Flash usage
 * 0x00000000                         - 4k Unzipped binary bootloader
 * 0x00000FFF
 *
 * 0x00001000                         - Unzipped binary code start
 *    ::::
 * 0x000xxxxx                         - Unzipped binary code end
 * 0x000x1000                         - First page used by Core OTA
 *    ::::
 * 0x000F2FFF  0x000F5FFF  0x000F5FFF
 ******************************************************************************
 *                                      Next 32k is overwritten by OTA
 * 0x000F3000  0x000F6000  0x000F6000 - 4k Tasmota Quick Power Cycle counter (SETTINGS_LOCATION - CFG_ROTATES) - First four bytes only
 * 0x000F3FFF  0x000F6FFF  0x000F6FFF
 * 0x000F4000  0x000F7000  0x000F7000 - 4k First Tasmota rotating settings page
 *    ::::
 * 0x000FA000  0x000FD000  0x000FD000 - 4k Last Tasmota rotating settings page = Last page used by Core OTA (SETTINGS_LOCATION)
 * 0x000FAFFF  0x000FDFFF  0x000FDFFF
 ******************************************************************************
 *             0x000FE000  0x000FE000 - 3k9 Not used
 *             0x000FEFF0  0x000FEFF0 - 4k1  Empty
 *             0x000FFFFF  0x000FFFFF
 *
 * 0x000FB000  0x00100000  0x00100000 - 0k, 980k or 2980k Core FS start (LittleFS)
 * 0x000FB000  0x001FA000  0x003FA000 - 0k, 980k or 2980k Core FS end (LittleFS)
 *             0x001FAFFF  0x003FAFFF
 *
 * 0x000FB000  0x001FB000  0x003FB000 - 4k Core EEPROM = Tasmota settings page during OTA and when no flash rotation is active (EEPROM_LOCATION)
 * 0x000FBFFF  0x001FBFFF  0x003FBFFF
 *
 * 0x000FC000  0x001FC000  0x003FC000 - 4k SDK - Uses first 128 bytes for phy init data mirrored by Core in RAM. See core_esp8266_phy.cpp phy_init_data[128] = Core user_rf_cal_sector
 * 0x000FD000  0x001FD000  0x003FD000 - 4k SDK - Uses scattered bytes from 0x340 (iTead use as settings storage from 0x000FD000)
 * 0x000FE000  0x001FE000  0x003FE000 - 4k SDK - Uses scattered bytes from 0x340 (iTead use as mirrored settings storage from 0x000FE000)
 * 0x000FF000  0x001FF000  0x0031F000 - 4k SDK - Uses at least first 32 bytes of this page - Tasmota Zigbee persistence from 0x000FF800 to 0x000FFFFF
 * 0x000FFFFF  0x001FFFFF  0x003FFFFF
\*********************************************************************************************/

// extern "C" {
// #include "spi_flash.h"
// }

// #ifdef ESP8266

// #include "eboot_command.h"

// extern "C" uint32_t _FS_start;      // 1M = 0x402fb000, 2M = 0x40300000, 4M = 0x40300000
// const uint32_t FLASH_FS_START = (((uint32_t)&_FS_start - 0x40200000) / SPI_FLASH_SEC_SIZE);
// uint32_t SETTINGS_LOCATION = FLASH_FS_START -1;                                                 // 0xFA, 0x0FF or 0x0FF

// // From libraries/EEPROM/EEPROM.cpp EEPROMClass
// extern "C" uint32_t _EEPROM_start;  // 1M = 0x402FB000, 2M = 0x403FB000, 4M = 0x405FB000
// const uint32_t EEPROM_LOCATION = ((uint32_t)&_EEPROM_start - 0x40200000) / SPI_FLASH_SEC_SIZE;  // 0xFB, 0x1FB or 0x3FB

// #endif  // ESP8266

// #ifdef ESP32

// // dummy defines
// #define EEPROM_LOCATION (SPI_FLASH_SEC_SIZE * 200)
// uint32_t SETTINGS_LOCATION = EEPROM_LOCATION;

// #endif  // ESP32

// const uint8_t CFG_ROTATES = 7;      // Number of flash sectors used (handles uploads)

// uint32_t settings_location = EEPROM_LOCATION;
// uint32_t settings_crc32 = 0;
// uint8_t *settings_buffer = nullptr;
// uint8_t config_xor_on_set = CONFIG_FILE_XOR;

// void SettingsInit(void) {
//   if (SETTINGS_LOCATION > 0xFA) {
//     SETTINGS_LOCATION = 0xFD;       // Skip empty partition part and keep in first 1M
//   }
// }


/*********************************************************************************************\
 * Settings services
\*********************************************************************************************/


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



void mSettings::SettingsSaveAll(void)
{
  Serial.println("SettingsSaveAll");
  if (Settings.flag_system.save_state) {
    Settings.power = power;
  } else {
    Settings.power = 0;
  }
  pCONT->Tasker_Interface(FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE);
  #ifdef USE_EEPROM
  EepromCommit();
  #endif
  SettingsSave(0);
}


// /*********************************************************************************************\
//  * Settings backup and restore
// \*********************************************************************************************/

// void SettingsBufferFree(void) {
//   if (settings_buffer != nullptr) {
//     free(settings_buffer);
//     settings_buffer = nullptr;
//   }
// }

// bool SettingsBufferAlloc(void) {
//   SettingsBufferFree();
//   if (!(settings_buffer = (uint8_t *)malloc(sizeof(TSettings)))) {
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_UPLOAD_ERR_2));  // Not enough (memory) space
//     return false;
//   }
//   return true;
// }

// String SettingsConfigFilename(void) {
//   char filename[TOPSZ];
//   char hostname[sizeof(TasmotaGlobal.hostname)];
//   snprintf_P(filename, sizeof(filename), PSTR("Config_%s_%s.dmp"), NoAlNumToUnderscore(hostname, TasmotaGlobal.hostname), TasmotaGlobal.version);
//   return String(filename);
// }

// uint32_t SettingsConfigBackup(void) {
//   if (!SettingsBufferAlloc()) { return 0; }

//   uint32_t cfg_crc32 = Settings->cfg_crc32;
//   Settings->cfg_crc32 = GetSettingsCrc32();  // Calculate crc (again) as it might be wrong when savedata = 0 (#3918)

//   uint32_t config_len = sizeof(TSettings);
//   memcpy(settings_buffer, Settings, config_len);

//   Settings->cfg_crc32 = cfg_crc32;  // Restore crc in case savedata = 0 to make sure settings will be noted as changed

//   if (config_xor_on_set) {
//     for (uint32_t i = 2; i < config_len; i++) {
//       settings_buffer[i] ^= (config_xor_on_set +i);
//     }
//   }
//   return config_len;
// }

// bool SettingsConfigRestore(void) {
//   uint32_t config_len = sizeof(TSettings);

//   if (config_xor_on_set) {
//     for (uint32_t i = 2; i < config_len; i++) {
//       settings_buffer[i] ^= (config_xor_on_set +i);
//     }
//   }

//   bool valid_settings = false;

//   // unsigned long version;                   // 008
//   unsigned long buffer_version = settings_buffer[11] << 24 | settings_buffer[10] << 16 | settings_buffer[9] << 8 | settings_buffer[8];
//   if (buffer_version > 0x06000000) {
//     // uint16_t      cfg_size;                  // 002
//     uint32_t buffer_size = settings_buffer[3] << 8 | settings_buffer[2];
//     if (buffer_version > 0x0606000A) {
//       // uint32_t      cfg_crc32;                 // FFC
//       uint32_t buffer_crc32 = settings_buffer[4095] << 24 | settings_buffer[4094] << 16 | settings_buffer[4093] << 8 | settings_buffer[4092];
//       valid_settings = (GetCfgCrc32(settings_buffer, buffer_size -4) == buffer_crc32);
//     } else {
//       // uint16_t      cfg_crc;                   // 00E
//       uint16_t buffer_crc16 = settings_buffer[15] << 8 | settings_buffer[14];
//       valid_settings = (GetCfgCrc16(settings_buffer, buffer_size) == buffer_crc16);
//     }
//   } else {
//     valid_settings = (settings_buffer[0] == CONFIG_FILE_SIGN);
//   }

//   if (valid_settings) {
//     // uint8_t       config_version;            // F36
// #ifdef ESP8266
//     valid_settings = (0 == settings_buffer[0xF36]);  // Settings->config_version
// #endif  // ESP8266
// #ifdef ESP32

// #ifdef CONFIG_IDF_TARGET_ESP32S3
//     valid_settings = (2 == settings_buffer[0xF36]);  // Settings->config_version ESP32S3
// #elif CONFIG_IDF_TARGET_ESP32S2
//     valid_settings = (3 == settings_buffer[0xF36]);  // Settings->config_version ESP32S2
// #elif CONFIG_IDF_TARGET_ESP32C3
//     valid_settings = (4 == settings_buffer[0xF36]);  // Settings->config_version ESP32C3
// #else
//     valid_settings = (1 == settings_buffer[0xF36]);  // Settings->config_version ESP32 all other
// #endif  // CONFIG_IDF_TARGET_ESP32S3
// #endif  // ESP32
//   }

//   if (valid_settings) {
//     SettingsDefaultSet2();
//     memcpy((char*)Settings +16, settings_buffer +16, config_len -16);
//     Settings->version = buffer_version;  // Restore version and auto upgrade after restart
//   }

//   SettingsBufferFree();

//   return valid_settings;
// }

// /*********************************************************************************************\
//  * Config Settings->text char array support
// \*********************************************************************************************/

// uint32_t GetSettingsTextLen(void) {
//   char* position = Settings->text_pool;
//   for (uint32_t size = 0; size < SET_MAX; size++) {
//     while (*position++ != '\0') { }
//   }
//   return position - Settings->text_pool;
// }

// bool settings_text_mutex = false;
// uint32_t settings_text_busy_count = 0;

// bool SettingsUpdateFinished(void) {
//   uint32_t wait_loop = 10;
//   while (settings_text_mutex && wait_loop) {  // Wait for any update to finish
//     yield();
//     delayMicroseconds(1);
//     wait_loop--;
//   }
//   return (wait_loop > 0);  // true if finished
// }

// bool SettingsUpdateText(uint32_t index, const char* replace_me) {
//   if (index >= SET_MAX) {
//     return false;  // Setting not supported - internal error
//   }

//   // Make a copy first in case we use source from Settings->text
//   uint32_t replace_len = strlen_P(replace_me);
//   char replace[replace_len +1];
//   memcpy_P(replace, replace_me, sizeof(replace));
//   uint32_t index_save = index;

//   uint32_t start_pos = 0;
//   uint32_t end_pos = 0;
//   char* position = Settings->text_pool;
//   for (uint32_t size = 0; size < SET_MAX; size++) {
//     while (*position++ != '\0') { }
//     if (1 == index) {
//       start_pos = position - Settings->text_pool;
//     }
//     else if (0 == index) {
//       end_pos = position - Settings->text_pool -1;
//     }
//     index--;
//   }
//   uint32_t char_len = position - Settings->text_pool;

//   uint32_t current_len = end_pos - start_pos;
//   int diff = replace_len - current_len;

// //  AddLog(LOG_LEVEL_DEBUG, PSTR("TST: start %d, end %d, len %d, current %d, replace %d, diff %d"),
// //    start_pos, end_pos, char_len, current_len, replace_len, diff);

//   int too_long = (char_len + diff) - settings_text_size;
//   if (too_long > 0) {
//     AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_CONFIG "Text overflow by %d char(s)"), too_long);
//     return false;  // Replace text too long
//   }

//   if (settings_text_mutex && !SettingsUpdateFinished()) {
//     settings_text_busy_count++;
//   } else {
//     settings_text_mutex = true;

//     if (diff != 0) {
//       // Shift Settings->text up or down
//       memmove_P(Settings->text_pool + start_pos + replace_len, Settings->text_pool + end_pos, char_len - end_pos);
//     }
//     // Replace text
//     memmove_P(Settings->text_pool + start_pos, replace, replace_len);
//     // Fill for future use
//     memset(Settings->text_pool + char_len + diff, 0x00, settings_text_size - char_len - diff);

//     settings_text_mutex = false;
//   }

// #ifdef DEBUG_FUNC_SETTINGSUPDATETEXT
//   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d, Id %02d = \"%s\""), GetSettingsTextLen(), settings_text_size, settings_text_busy_count, index_save, replace);
// #else
//   AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "CR %d/%d, Busy %d"), GetSettingsTextLen(), settings_text_size, settings_text_busy_count);
// #endif

//   return true;
// }

// char* SettingsText(uint32_t index) {
//   char* position = Settings->text_pool;

//   if (index >= SET_MAX) {
//     position += settings_text_size -1;  // Setting not supported - internal error - return empty string
//   } else {
//     SettingsUpdateFinished();
//     for (;index > 0; index--) {
//       while (*position++ != '\0') { }
//     }
//   }
//   return position;
// }

// /*********************************************************************************************\
//  * Config Save - Save parameters to Flash ONLY if any parameter has changed
// \*********************************************************************************************/

// void UpdateBackwardCompatibility(void) {
//   // Perform updates for backward compatibility
//   strlcpy(Settings->user_template_name, SettingsText(SET_TEMPLATE_NAME), sizeof(Settings->user_template_name));
// }

// uint32_t GetSettingsAddress(void) {
//   return settings_location * SPI_FLASH_SEC_SIZE;
// }

// void SettingsSave(uint8_t rotate) {
// /* Save configuration in eeprom or one of 7 slots below
//  *
//  * rotate 0 = Save in next flash slot
//  * rotate 1 = Save only in eeprom flash slot until SetOption12 0 or restart
//  * rotate 2 = Save in eeprom flash slot, erase next flash slots and continue depending on stop_flash_rotate
//  * stop_flash_rotate 0 = Allow flash slot rotation (SetOption12 0)
//  * stop_flash_rotate 1 = Allow only eeprom flash slot use (SetOption12 1)
//  */
// #ifndef FIRMWARE_MINIMAL
//   XsnsXdrvCall(FUNC_SAVE_SETTINGS);
//   UpdateBackwardCompatibility();
//   if ((GetSettingsCrc32() != settings_crc32) || rotate) {
//     if (1 == rotate) {                                 // Use eeprom flash slot only and disable flash rotate from now on (upgrade)
//       TasmotaGlobal.stop_flash_rotate = 1;
//     }

//     if (TasmotaGlobal.stop_flash_rotate || (2 == rotate)) {  // Use eeprom flash slot and erase next flash slots if stop_flash_rotate is off (default)
//       settings_location = EEPROM_LOCATION;
//     } else {                                           // Rotate flash slots
//       if (settings_location == EEPROM_LOCATION) {
//         settings_location = SETTINGS_LOCATION;
//       } else {
//         settings_location--;
//       }
//       if (settings_location <= (SETTINGS_LOCATION - CFG_ROTATES)) {
//         settings_location = EEPROM_LOCATION;
//       }
//     }

//     Settings->save_flag++;
//     if (UtcTime() > START_VALID_TIME) {
//       Settings->cfg_timestamp = UtcTime();
//     } else {
//       Settings->cfg_timestamp++;
//     }
//     Settings->cfg_size = sizeof(TSettings);
//     Settings->cfg_crc = GetSettingsCrc();               // Keep for backward compatibility in case of fall-back just after upgrade
//     Settings->cfg_crc32 = GetSettingsCrc32();
// #ifdef USE_COUNTER
//     CounterInterruptDisable(true);
// #endif
// #ifdef ESP8266
// #ifdef USE_UFILESYS
//     TfsSaveFile(TASM_FILE_SETTINGS, (const uint8_t*)Settings, sizeof(TSettings));
// #endif  // USE_UFILESYS
//     if (ESP.flashEraseSector(settings_location)) {
//       ESP.flashWrite(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)Settings, sizeof(TSettings));
//     }

//     if (!TasmotaGlobal.stop_flash_rotate && rotate) {  // SetOption12 - (Settings) Switch between dynamic (0) or fixed (1) slot flash save location
//       for (uint32_t i = 0; i < CFG_ROTATES; i++) {
//         ESP.flashEraseSector(SETTINGS_LOCATION -i);    // Delete previous configurations by resetting to 0xFF
//         delay(1);
//       }
//     }
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG D_SAVED_TO_FLASH_AT " %X, " D_COUNT " %d, " D_BYTES " %d"), settings_location, Settings->save_flag, sizeof(TSettings));
// #endif  // ESP8266
// #ifdef ESP32
//     SettingsWrite(Settings, sizeof(TSettings));
//     AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_CONFIG "Saved, " D_COUNT " %d, " D_BYTES " %d"), Settings->save_flag, sizeof(TSettings));
// #endif  // ESP32

//     settings_crc32 = Settings->cfg_crc32;
//   }
// #endif  // FIRMWARE_MINIMAL
//   RtcSettingsSave();
// #ifdef USE_COUNTER
//   CounterInterruptDisable(false);
// #endif
// }
// void SettingsLoad(void) {
// #ifdef ESP8266
//   // Load configuration from optional file and flash (eeprom and 7 additonal slots) if first valid load does not stop_flash_rotate
//   // Activated with version 8.4.0.2 - Fails to read any config before version 6.6.0.11
//   settings_location = 0;
//   uint32_t save_flag = 0;
//   uint32_t max_slots = CFG_ROTATES +1;
//   uint32_t flash_location;
//   uint32_t slot = 1;
// #ifdef USE_UFILESYS
//   if (TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings))) {
//     flash_location = 1;
//     slot = 0;
//   }
// #endif  // USE_UFILESYS
//   while (slot <= max_slots) {                                  // Read all config pages in search of valid and latest
//     if (slot > 0) {
//       flash_location = (1 == slot) ? EEPROM_LOCATION : (2 == slot) ? SETTINGS_LOCATION : flash_location -1;
//       ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)Settings, sizeof(TSettings));
//     }
//     if ((Settings->cfg_crc32 != 0xFFFFFFFF) && (Settings->cfg_crc32 != 0x00000000) && (Settings->cfg_crc32 == GetSettingsCrc32())) {
//       if (Settings->save_flag > save_flag) {                    // Find latest page based on incrementing save_flag
//         save_flag = Settings->save_flag;
//         settings_location = flash_location;
//         if (Settings->flag.stop_flash_rotate && (1 == slot)) {  // Stop if only eeprom area should be used and it is valid
//           break;
//         }
//       }
//     }
//     slot++;
//     delay(1);
//   }
//   if (settings_location > 0) {
// #ifdef USE_UFILESYS
//     if (1 == settings_location) {
//       TfsLoadFile(TASM_FILE_SETTINGS, (uint8_t*)Settings, sizeof(TSettings));
//       AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded from File, " D_COUNT " %lu"), Settings->save_flag);
//     } else
// #endif  // USE_UFILESYS
//     {
//       ESP.flashRead(settings_location * SPI_FLASH_SEC_SIZE, (uint32*)Settings, sizeof(TSettings));
//       AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG D_LOADED_FROM_FLASH_AT " %X, " D_COUNT " %lu"), settings_location, Settings->save_flag);
//     }
//   }
// #endif  // ESP8266

// #ifdef ESP32
//   uint32_t source = SettingsRead(Settings, sizeof(TSettings));
//   if (source) {
//     settings_location = 1;
//     if (Settings->cfg_holder == (uint16_t)CFG_HOLDER) {
//       AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded from %s, " D_COUNT " %lu"), (2 == source)?"File":"NVS", Settings->save_flag);
//     }
//   }
// #endif  // ESP32

// #ifndef FIRMWARE_MINIMAL
//   if ((0 == settings_location) || (Settings->cfg_holder != (uint16_t)CFG_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
// //  if ((0 == settings_location) || (Settings->cfg_size != sizeof(TSettings)) || (Settings->cfg_holder != (uint16_t)CFG_HOLDER)) {  // Init defaults if cfg_holder differs from user settings in my_user_config.h
// #ifdef USE_UFILESYS
//     if (TfsLoadFile(TASM_FILE_SETTINGS_LKG, (uint8_t*)Settings, sizeof(TSettings)) && (Settings->cfg_crc32 == GetSettingsCrc32())) {
//       settings_location = 1;
//       AddLog(LOG_LEVEL_NONE, PSTR(D_LOG_CONFIG "Loaded from LKG File, " D_COUNT " %lu"), Settings->save_flag);
//     } else
// #endif  // USE_UFILESYS
//     {
//       SettingsDefault();
//     }
//   }
//   settings_crc32 = GetSettingsCrc32();
// #endif  // FIRMWARE_MINIMAL

//   RtcSettingsLoad(1);
// }

// // Used in TLS - returns the timestamp of the last Flash settings write
// uint32_t CfgTime(void) {
//   return Settings->cfg_timestamp;
// }

// #ifdef ESP8266
// void SettingsErase(uint8_t type) {
//   /*
//     For Arduino core and SDK:
//     Erase only works from flash start address to SDK recognized flash end address (flashchip->chip_size = ESP_getFlashChipSize).
//     Addresses above SDK recognized size (up to ESP.getFlashChipRealSize) are not accessable.
//     For Esptool:
//     The only way to erase whole flash is esptool which uses direct SPI writes to flash.

//     The default erase function is EspTool (EsptoolErase)

//     0 = Erase from program end until end of flash as seen by SDK including optional filesystem
//     1 = Erase 16k SDK parameter area near end of flash as seen by SDK (0x0XFCxxx - 0x0XFFFFF) solving possible wifi errors
//     2 = Erase from program end until end of flash as seen by SDK excluding optional filesystem
//     3 = Erase Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
//     4 = Erase SDK parameter area used for wifi calibration (0x0FCxxx - 0x0FCFFF)
//   */

// #ifndef FIRMWARE_MINIMAL
//                            // Reset 2 = Erase all flash from program end to end of physical flash
//   uint32_t _sectorStart = (ESP.getSketchSize() / SPI_FLASH_SEC_SIZE) + 1;
//   uint32_t _sectorEnd = ESP.getFlashChipRealSize() / SPI_FLASH_SEC_SIZE;  // Flash size as reported by hardware
//   if (1 == type) {         // Reset 3 = SDK parameter area
//     // source Esp.cpp and core_esp8266_phy.cpp
//     _sectorStart = (ESP.getFlashChipSize() / SPI_FLASH_SEC_SIZE) - 4;
//   }
//   else if (2 == type) {    // Reset 5, 6 = Erase all flash from program end to end of physical flash but skip filesystem
// /*
// #ifdef USE_UFILESYS
//     TfsDeleteFile(TASM_FILE_SETTINGS);  // Not needed as it is recreated by set defaults before restart
// #endif
// */
//     EsptoolErase(_sectorStart, FLASH_FS_START);
//     _sectorStart = EEPROM_LOCATION;
//     _sectorEnd = ESP_getFlashChipSize() / SPI_FLASH_SEC_SIZE;  // Flash size as seen by SDK
//   }
//   else if (3 == type) {    // QPC Reached = QPC and Tasmota and SDK parameter area (0x0F3xxx - 0x0FFFFF)
// #ifdef USE_UFILESYS
//     TfsDeleteFile(TASM_FILE_SETTINGS);
// #endif
//     EsptoolErase(SETTINGS_LOCATION - CFG_ROTATES, SETTINGS_LOCATION +1);
//     _sectorStart = EEPROM_LOCATION;
//     _sectorEnd = ESP.getFlashChipSize() / SPI_FLASH_SEC_SIZE;  // Flash size as seen by SDK
//   }
//   else if (4 == type) {    // WIFI_FORCE_uF_CAL_ERASE = SDK wifi calibration
//     _sectorStart = EEPROM_LOCATION +1;   u                     // SDK phy area and Core calibration sector (0x0XFC000)
//     _sectorEnd = _sectorStart +1;        u                     // SDK end of phy area and Core calibration sector (0x0XFCFFF)
//   }

//   EsptoolErase(_sectorStart, _sectorEnd);u                     // Esptool - erases flash completely
// #endif  // FIRMWARE_MINIMAL
// }
// #endif  // ESP8266

void mSettings::SettingsSdkErase(void) {
  WiFi.disconnect(false);  // Delete SDK wifi config
  SettingsErase(1);
  delay(1000);
}
