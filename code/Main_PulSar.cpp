
// #include <Arduino.h>

// void setup(void)
// { 
//   Serial.begin(115200); // to be baudrate_tmp later
//   Serial.println(F("Starting up...")); Serial.flush();  
// }

// void loop(void)
// { 
//   Serial.println(F("Looping...")); Serial.flush();
//   delay(100);  
// } 


/**
 * @file    HomeControlSystem.cpp
 * @author  Michael Doone (michaeldoonehub@gmail.com)
 * @brief   Primary code setup() and loop()
 * @version 0.1
 * @date    2022-04-20
 * 
 * @copyright Copyright (c) 2022
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **/

// #define ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY


#include "1_TaskerManager/mTaskerManager.h"

#ifdef ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
// In Main_PulSar.cpp or another relevant file
// int channels_resolution = 10;  // or whatever the appropriate value/type is
// uint8_t channels_resolution[30];
#include <gpio_viewer.h> // Must me the first include in your project
GPIOViewer gpio_viewer;
#endif



/*********************************************************************************************
 * Hardware related
\*********************************************************************************************/

#ifdef ESP32
  #include "soc/soc.h"
  #include "soc/rtc_cntl_reg.h"
  #define DISABLE_ESP32_BROWNOUT
  void DisableBrownout(void) 
  { // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
    WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
  }
  #ifdef ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING
  void hw_wdt_disable(){
    *((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF 
  }
  void hw_wdt_enable(){ 
    *((volatile uint32_t*) 0x60000900) |= 1; // Hardware WDT ON 
  }
  #endif  
#endif // ESP32

#ifdef ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT
/*********************************************************************************************\
 * Emergency reset if Rx and Tx are tied together
\*********************************************************************************************/
void EmergencySerial_SettingsReset(void) {
  Serial.begin(115200);
  Serial.write(0xA5);
  Serial.write(0x5A);
  delay(1);
  if (Serial.available() == 2) {
    if ((Serial.read() == 0xA5) && (Serial.read() == 0x5A)) {
      tkr_set->SettingsErase(3);       // Reset all settings including QuickPowerCycle flag

      do {                    // Wait for user to remove Rx Tx jumper and power cycle
        Serial.write(0xA5);
        delay(1000);          // Satisfy SDK
      } while (Serial.read() == 0xA5);  // Poll for removal of jumper

      mSupportHardware::ESP_Restart();          // Restart to init default settings
    }
  }
  Serial.println();
  Serial.flush();
#ifdef ESP32
  delay(10);                  // Allow time to cleanup queues - if not used hangs ESP32
  Serial.end();
  delay(10);                  // Allow time to cleanup queues - if not used hangs ESP32
#endif  // ESP32
}
#endif  // ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT


#ifdef ESP32
// IDF5.3 fix esp_gpio_reserve used in init PSRAM. Needed by Tasmota.ino esp_gpio_revoke
// #include "esp_private/esp_gpio_reserve.h"
#endif  // ESP32
extern bool psramInit();   // forward declare if needed
/**
 * @brief Print basic PulSar firmware/device splash to Serial.
 * 
 * Put this after Serial.begin(...) and after any early delay needed for USB serial.
 */
void Serial_PrintFirmwareSplash()
{
  Serial.println();
  Serial.println(F("=================================================="));
  Serial.println(F(" PulSar Firmware Boot"));
  Serial.println(F("=================================================="));

  #ifdef DEVICENAME_CTR
    Serial.printf("Device:          %s\r\n", DEVICENAME_CTR);
  #else
    Serial.println(F("Device:          <DEVICENAME_CTR not defined>"));
  #endif

  #ifdef DEVICENAME_FRIENDLY_CTR
    Serial.printf("Friendly Name:   %s\r\n", DEVICENAME_FRIENDLY_CTR);
  #else
    Serial.println(F("Friendly Name:   <DEVICENAME_FRIENDLY_CTR not defined>"));
  #endif

  #ifdef FIRMWARE_NAME_CTR
    Serial.printf("Firmware:        %s\r\n", FIRMWARE_NAME_CTR);
  #else
    Serial.println(F("Firmware:        PulSar"));
  #endif

  #ifdef FIRMWARE_VERSION_CTR
    Serial.printf("Version:         %s\r\n", FIRMWARE_VERSION_CTR);
  #else
    Serial.println(F("Version:         <FIRMWARE_VERSION_CTR not defined>"));
  #endif

  Serial.printf("Build Date:      %s\r\n", __DATE__);
  Serial.printf("Build Time:      %s\r\n", __TIME__);

  #if defined(ARDUINO_ARCH_ESP32)
    Serial.println(F("Platform:        ESP32"));
    Serial.printf("ESP-IDF:         %s\r\n", ESP.getSdkVersion());
    Serial.printf("Chip Model:      %s\r\n", ESP.getChipModel());
    Serial.printf("Chip Revision:   %d\r\n", ESP.getChipRevision());
    Serial.printf("CPU Freq:        %u MHz\r\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash Size:      %u bytes\r\n", ESP.getFlashChipSize());
    Serial.printf("Sketch Size:     %u bytes\r\n", ESP.getSketchSize());
    Serial.printf("Free Sketch:     %u bytes\r\n", ESP.getFreeSketchSpace());
    Serial.printf("Heap Free:       %u bytes\r\n", ESP.getFreeHeap());
  #elif defined(ARDUINO_ARCH_ESP8266)
    Serial.println(F("Platform:        ESP8266"));
    Serial.printf("SDK:             %s\r\n", ESP.getSdkVersion());
    Serial.printf("Core Version:    %s\r\n", ESP.getCoreVersion().c_str());
    Serial.printf("CPU Freq:        %u MHz\r\n", ESP.getCpuFreqMHz());
    Serial.printf("Flash Size:      %u bytes\r\n", ESP.getFlashChipRealSize());
    Serial.printf("Sketch Size:     %u bytes\r\n", ESP.getSketchSize());
    Serial.printf("Free Sketch:     %u bytes\r\n", ESP.getFreeSketchSpace());
    Serial.printf("Heap Free:       %u bytes\r\n", ESP.getFreeHeap());
  #else
    Serial.println(F("Platform:        <unknown>"));
  #endif

  #ifdef PIOENV
    Serial.printf("PIO Env:         %s\r\n", PIOENV);
  #endif

  Serial.println(F("=================================================="));
  Serial.println();
}

/********************************************************************************************/
/********************* ENABLE_DEVFEATURE_FASTBOOT_DETECTION ******************************************************************/
/********************************************************************************************/


#ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
void Fastboot_RecoveryCheck(void)
{
  ALOG_DBM(PSTR("ARESET TWICE! \t\t\t%d"), RtcFastboot.fast_reboot_count);

  const uint8_t boot_loop_offset = tkr_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET]; // SetOption36

  if (!boot_loop_offset) {
    return;
  }

  // Not yet considered a boot loop
  if (RtcFastboot.fast_reboot_count <= boot_loop_offset) {
    return;
  }

  const uint8_t fastboot_stage = RtcFastboot.fast_reboot_count - boot_loop_offset;

  ALOG_WRN(
    PSTR("Fastboot: Recovery stage %d, fast_reboot_count=%d, offset=%d"),
    fastboot_stage,
    RtcFastboot.fast_reboot_count,
    boot_loop_offset
  );

  // --------------------------------------------------------------------------
  // Stage 1:
  // First detected fast reboot beyond offset.
  // Log only / reserve for very light test-disable behaviour.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 1) {
    ALOG_INF(PSTR("Fastboot: Stage 1 - detected boot loop, no destructive action"));
  }

  // --------------------------------------------------------------------------
  // Stage 2:
  // Disable newest experimental/test code.
  // Modules may listen for this event and disable risky dev features.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 2) {
    ALOG_INF(PSTR("Fastboot: Stage 2 - FASTBOOT_EVENT_1"));
    tkr->Tasker_Interface(TASK_FASTBOOT_EVENT_1);
  }

  // --------------------------------------------------------------------------
  // Stage 3:
  // Disable broader experimental code.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 3) {
    ALOG_INF(PSTR("Fastboot: Stage 3 - FASTBOOT_EVENT_2"));
    tkr->Tasker_Interface(TASK_FASTBOOT_EVENT_2);
  }

  // --------------------------------------------------------------------------
  // Stage 4:
  // Disable rules first. Rules cross-link modules and are high-risk during boot.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 4) {
    ALOG_INF(PSTR("Fastboot: Stage 4 - Disable rules"));

    // Runtime flag preferred, if available:
    tkr_set->runtime.fastboot.disable_rules = true;

    // Later, also guard rule startup:
    // if (!tkr_set->runtime.fastboot.disable_rules) {
    //   tkr->Tasker_Interface(TASK_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID);
    // }
  }

  // --------------------------------------------------------------------------
  // Stage 5:
  // Disable sensors. Sensors touch I2C/SPI/UART/external devices and can block.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 5) {
    ALOG_INF(PSTR("Fastboot: Stage 5 - Disable sensors"));

    tkr_set->runtime.fastboot.disable_sensors = true;
  }

  // --------------------------------------------------------------------------
  // Stage 6:
  // Disable drivers / actuators.
  // Keep network, OTA, WebUI, filesystem, logging alive.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 6) {
    ALOG_INF(PSTR("Fastboot: Stage 6 - Disable drivers"));

    tkr_set->runtime.fastboot.disable_drivers = true;
  }

  // --------------------------------------------------------------------------
  // Stage 7:
  // Skip stored module JSON/config loads.
  // This recovers from bad module config files, not bad core settings.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 7) {
    ALOG_INF(PSTR("Fastboot: Stage 7 - Disable module filesystem config load"));

    tkr_set->runtime.fastboot.disable_module_config_load = true;
  }

  // --------------------------------------------------------------------------
  // Stage 8:
  // Skip templates and use compiled-safe module configuration.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 8) {
    ALOG_INF(PSTR("Fastboot: Stage 8 - Disable templates, force safe compiled config"));

    tkr_set->runtime.fastboot.disable_templates = true;
    tkr_set->runtime.fastboot.force_safe_compiled_config = true;
  }

  // --------------------------------------------------------------------------
  // Stage 9:
  // Reset settings to defaults, but force known STA WiFi for OTA recovery.
  // First destructive recovery stage.
  // --------------------------------------------------------------------------
  if (fastboot_stage == 9) {
    ALOG_WRN(PSTR("Fastboot: Stage 9 - Reset settings to defaults with STA recovery"));

    tkr_set->runtime.fastboot.disable_rules = true;
    tkr_set->runtime.fastboot.disable_sensors = true;
    tkr_set->runtime.fastboot.disable_drivers = true;
    tkr_set->runtime.fastboot.disable_module_config_load = true;
    tkr_set->runtime.fastboot.disable_templates = true;

    // Later implement these as plain/global-safe helpers or thin settings calls.
    // SettingsSave_Prev();
    // SettingsDefault();
    // ApplyHardcodedStaWifiRecoverySettings();
    // SettingsSaveAll();
  }

  // --------------------------------------------------------------------------
  // Stage 10:
  // Factory recovery. Clear settings and boot AP recovery mode with OTA/WebUI.
  // --------------------------------------------------------------------------
  if (fastboot_stage == 10) {
    ALOG_ERR(PSTR("Fastboot: Stage 10 - Factory AP recovery"));

    tkr_set->runtime.fastboot.disable_rules = true;
    tkr_set->runtime.fastboot.disable_sensors = true;
    tkr_set->runtime.fastboot.disable_drivers = true;
    tkr_set->runtime.fastboot.disable_module_config_load = true;
    tkr_set->runtime.fastboot.disable_templates = true;
    tkr_set->runtime.fastboot.factory_ap_recovery = true;

    // Later:
    // SettingsSave_Prev();
    // SettingsErase(3);
    // SettingsDefault();
    // ApplyFactoryApRecoverySettings();
    // SettingsSaveAll();
  }

  // --------------------------------------------------------------------------
  // Stage 11+:
  // Do not continue normal boot. Recovery only.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 11) {
    ALOG_ERR(PSTR("Fastboot: Stage 11+ - Blocking safe recovery mode"));

    tkr_set->runtime.fastboot.blocking_safe_mode = true;

    // Later:
    // SafeMode_StartAndAwaitOTA();
  }

  ALOG_INF(PSTR("FRC: " D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcFastboot.fast_reboot_count);
}
#endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION


/********************************************************************************************/
/*********************SETUP******************************************************************/
/********************************************************************************************/

void setup(void)
{ 

/********************************************************************************************
 ** Brownout ********************************************************************************
 ********************************************************************************************/
 
  #ifdef ESP32
    #ifdef DISABLE_ESP32_BROWNOUT
      DisableBrownout();      // Workaround possible weak LDO resulting in brownout detection during Wifi connection
    #endif  // DISABLE_ESP32_BROWNOUT

    Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT); // to be baudrate_tmp later
    Serial.setDebugOutput(true);
    #ifdef CONFIG_IDF_TARGET_ESP32
    // restore GPIO16/17 if no PSRAM is found
    if (!SupportESP32::FoundPSRAM()) {
      // // test if the CPU is not pico
      uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
      uint32_t pkg_version = chip_ver & 0x7;
      if (pkg_version <= 3) {   // D0WD, S0WD, D2WD
        gpio_reset_pin(GPIO_NUM_16);
        gpio_reset_pin(GPIO_NUM_17);
      }
      // uint32_t pkg_version = bootloader_common_get_chip_ver_pkg();
      // if (pkg_version <= 3) {         // D0WD, S0WD, D2WD
      //   gpio_reset_pin((gpio_num_t)CONFIG_D0WD_PSRAM_CS_IO);
      //   gpio_reset_pin((gpio_num_t)CONFIG_D0WD_PSRAM_CLK_IO);
      //   // IDF5.3 fix esp_gpio_reserve used in init PSRAM
      //   esp_gpio_revoke(BIT64(CONFIG_D0WD_PSRAM_CS_IO) | BIT64(CONFIG_D0WD_PSRAM_CLK_IO));
      // }
    }
    #endif  // CONFIG_IDF_TARGET_ESP32
    
    #ifdef USE_MODULE_DRIVERS__CAMERA
    psramInit();               // initialize PSRAM        
    Serial.printf("psramFound: %d\n\r", psramFound());
    Serial.printf("esp_spiram_is_initialized: %d\n\r", esp_spiram_is_initialized());
    Serial.printf("Free PSRAM: %u\n\r", ESP.getFreePsram());
    #endif
  #endif  // ESP32

  /**
   * @brief WatchDog timer
   * Code priority: Highest (Before Fastboot, Primary Recovery)
   **/
  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Init();
  #else
  #warning "No WDT has been enabled, this is not recommended for production code!"
  #endif

  /********************************************************************************************
   ** Fastboot: >> Base Setup Recovery <<  
   *     - If Settings, TaskerManager etc is corrupt
   *     - Minimal code must run above this (Serial start, RTC check)
   * Code priority: High (Secondary Recovery)
   * @note AddLog can not be used
  ********************************************************************************************/

  /**
   * @brief Only enable serial first if fastboot debugging
   **/
  #ifdef DEBUG_FASTBOOT
  Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT);
  Serial.println(F("\n\rSerial Enabled Early for FastBoot Debug" DEBUG_INSERT_PAGE_BREAK));
  #endif

  #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION

    RtcFastboot_Load();

    if (!RtcFastboot_Valid())
    {
      RtcFastboot.fast_reboot_count = 0;
    }
    
    /**
     * @brief Good Boot: Waking from sleep, keep reseting the counter
     **/
    if (ResetReason_g() == REASON_DEEP_SLEEP_AWAKE) 
    {
      RtcFastboot.fast_reboot_count = 0;  // Disable fast reboot and quick power cycle detection
    } 
    /**
     * @brief Bad Boot: Increment count
     **/
    else
    {
      RtcFastboot.fast_reboot_count++;
      #ifdef DEBUG_FASTBOOT
      Serial.printf("FastBoot: Count %d\n\r", RtcFastboot.fast_reboot_count); 
      #endif
    }
    Serial.printf("FastBoot: Count %u\r\n", RtcFastboot.fast_reboot_count);

    RtcFastboot_Save(); // Save reboot

    /**
     * @brief If fastboot has exceeded OTA fallback bootcount, then immediately enter safemode/recoverymode
     * @note:  Code below will first attempt to recover device by disabling feature, this is a last step measure
     **/
    #if defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) || defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)
    if(RtcFastboot.fast_reboot_count > 10)
    {
      SafeMode_StartAndAwaitOTA();
    }
    #endif // ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
    /**
     * @brief If fastboot has exceeded OTA fallback bootcount, then immediately enter safemode/recoverymode
     * @note:  Code below will first attempt to recover device by disabling feature, this is a last step measure
     **/
    #if defined(ENABLE_DEVFEATURE_FASTBOOT_CELLULAR_SMS_BEACON_FALLBACK_DEFAULT_SSID) || defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)
    if(RtcFastboot.fast_reboot_count > 10)
    {
      SafeMode_CellularConnectionAndSendLocation();
    }
    #endif // ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

  #endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION

  #ifdef ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING
  Serial.flush();
  delay(1000);
  tkr_sup->CmndCrash();
  #endif  // ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING

/********************************************************************************************
 ** RTC Settings ********************************************************************************
 ********************************************************************************************/
  
  // Load the baudrate from RTC into temp value, and after FlashSettings are loaded, move its value into flash settings
  uint32_t baudrate_tmp = 115200;

  #ifdef ENABLE_DEVFEATURE_RTC_SETTINGS
  if (RtcSettingsLoad(0)) {
    uint32_t baudrate = (RtcSettings.baudrate / 300) * 300;  // Make it a valid baudrate
    if (baudrate) { baudrate_tmp = baudrate; } // Only modify if valid
  }
  #endif // ENABLE_DEVFEATURE_RTC_SETTINGS

/********************************************************************************************
 ** Serial **********************************************************************************
 ********************************************************************************************/
 
  #ifndef DISABLE_SERIAL0_CORE
  Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT); // to be baudrate_tmp later
  #endif
  #ifdef USE_SERIAL_ALTERNATE_TX
    #ifndef ESP32
      Serial.set_tx(2);
    #endif
  #endif
  Serial.println(F("\n\rRebooting,.." DEBUG_INSERT_PAGE_BREAK));
  #ifndef DISABLE_SERIAL_LOGGING
  #ifdef ENABLE_BUG_TRACING
  Serial.println(F("DELAYED BOOT for 5 seconds...")); Serial.flush(); delay(5000);
  #endif
  #endif
  
  Serial_PrintFirmwareSplash();

  Serial.printf("baudrate_tmp = %d\n\r", baudrate_tmp);

  #ifdef ENABLE_DEVFEATURE_SETDEBUGOUTPUT
  Serial.setDebugOutput(true);
  #endif 

/********************************************************************************************
 ** Debug: When debugging is crashing, priority is to start wifi and get OTA going and waiting a few seconds to catch an OTA **************************************************************************
 ********************************************************************************************/
  // #ifdef ENABLE_DEVFEATURE_FAST_REBOOT_OTA_SAFEMODE
  // if (RtcFastboot.fast_reboot_count > 0) {
  //   SafeMode_StartAndAwaitOTA(10);
  // }
  // #endif
  
/********************************************************************************************
 ** Init Pointers ***************************************************************************
 ********************************************************************************************/
 
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE

  // Init Json builder with memory address and size
  JsonBuilderI ->Start(data_buffer.payload.ctr, data_buffer.payload.length_used, DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  
  DEBUG_LINE_HERE

  if(data_buffer.payload.ctr){
    DEBUG_LINE_HERE
  }else{
    DEBUG_LINE_HERE
  }
  
  
Serial.print(F("payload.ctr addr = 0x"));
Serial.println((uintptr_t)data_buffer.payload.ctr, HEX);

Serial.print(F("len_used addr   = 0x"));
Serial.println((uintptr_t)&data_buffer.payload.length_used, HEX);

Serial.print(F("DATA_BUFFER addr= 0x"));
Serial.println((uintptr_t)&data_buffer, HEX);

Serial.print(F("size = "));
Serial.println((unsigned)DATA_BUFFER_PAYLOAD_MAX_LENGTH);

  BufferWriterI->Start(data_buffer.payload.ctr, data_buffer.payload.length_used, DATA_BUFFER_PAYLOAD_MAX_LENGTH); //length prob doesnt need to be set either after its defined in the class
  
  /**
   * @brief Start the Tasker_Interface module
   **/
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  Serial.printf("time %dms\n\r", millis());
  tkr->Instance_Init();
  DEBUG_LINE_HERE
  Serial.printf("time %dms\n\r", millis());
  DEBUG_LINE_HERE
  // DEBUG_CRITICAL_STOP_CODE_PRINT
/********************************************************************************************
 ** LOGGING: Set boot log levels *********************************************************************
 ********************************************************************************************/

  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;

  DEBUG_LINE_HERE
  DEBUG_LINE_HERE3
  
/********************************************************************************************
 ** Init Pointers ***************************************************************************
 ********************************************************************************************/
 
  ALOG_DBM(PSTR("AddLog Started"));

  DEBUG_LINE_HERE
  DEBUG_LINE_HERE3

  #ifdef ENABLE_DEBUGFEATURE_LOGGING__ENABLE_TELNET_IMMEDIATE_WITH_WAIT
  ALOG_INF(PSTR("Early Wifi connection attempt (up to 20 seconds delay)"));
  // WiFi.begin(STA_SSID1,STA_PASS1);
  // for (int i = 0; i < 20; ++i) {   // Retry for ~20 seconds
  //   delay(1000);
  //   if ((WiFi.status() == WL_CONNECTED) && WiFi.localIP()){
  //     tkr_log->StartTelnetServer();
  //     if(tkr_log->telnet.running){      tkr_log->handleTelnet();    }
  //     delay(2000);
  //     ALOG_INF(PSTR("Attempt %d: Successful"), i); 
  //     break;
  //   }
  //   delay(1000);
  // }
  #endif

  /********************************************************************************************
   ** Splash boot reason ***************************************************************************
  ********************************************************************************************/

  DEBUG_LINE_HERE3
  
  ALOG_INF(PSTR("ResetReason=%d"), ResetReason_g());
  
  DEBUG_LINE_HERE3

/********************************************************************************************
 ** Show PSRAM Present **********************************************************************
 ********************************************************************************************/
// #if CONFIG_IDF_TARGET_ESP32
//   esp_err_t psram_status = esp_psram_init();
//   if (psram_status != ESP_OK) {
//     AddLog(LOG_LEVEL_INFO, "PSRAM init failed: 0x%x", psram_status);
//   }
// #endif

  #ifdef ESP32
    // ALOG_INF(PSTR("HDW: %s %s"), GetDeviceHardware().c_str(),
    //           SupportESP32::FoundPSRAM() ? (SupportESP32::CanUsePSRAM() ? "(PSRAM)" : "(PSRAM disabled)") : "" );
    // ALOG_DBG(PSTR("HDW: FoundPSRAM=%i CanUsePSRAM=%i"), SupportESP32::FoundPSRAM(), SupportESP32::CanUsePSRAM());
    // #if !defined(HAS_PSRAM_FIX)
    // if (SupportESP32::FoundPSRAM() && !SupportESP32::CanUsePSRAM()) {
    //   ALOG_INF(PSTR("HDW: PSRAM is disabled, requires specific compilation on this hardware (see doc)"));
    // }
    // #endif
  #else // ESP32
    // ALOG_INF(PSTR("HDW: %s"), GetDeviceHardware().c_str());
  #endif // ESP32
  DEBUG_LINE_HERE3
  #ifdef ESP32
  ALOG_INF(PSTR("HDW: %s %s"), SupportESP32::GetDeviceHardwareRevision().c_str(),SupportESP32::FoundPSRAM() ? (SupportESP32::CanUsePSRAM() ? "(PSRAM)" : "(PSRAM disabled)") : "" );
  DEBUG_LINE_HERE3
  // AddLog(LOG_LEVEL_DEBUG, PSTR("HDW: FoundPSRAM=%i CanUsePSRAM=%i"), FoundPSRAM(), CanUsePSRAM());
#if !defined(HAS_PSRAM_FIX)
DEBUG_LINE_HERE3
  if (SupportESP32::FoundPSRAM() && !SupportESP32::CanUsePSRAM()) {
    DEBUG_LINE_HERE3
    ALOG_INF(PSTR("HDW: PSRAM is disabled, requires specific compilation on this hardware (see doc)"));
  }
  DEBUG_LINE_HERE3
  // DELAY_DEBUG(5000); // Allow time to read the log
#endif  // HAS_PSRAM_FIX
#else   // ESP8266
  // AddLog(LOG_LEVEL_INFO, PSTR("HDW: %s"), GetDeviceHardware().c_str());
#endif  // ESP32
  DEBUG_LINE_HERE3


/********************************************************************************************
 ** File System : Init *****************************************************************************
 ********************************************************************************************/

  #ifdef USE_MODULE_CORE_FILESYSTEM
    tkr_mfile->UfsInit();  // xdrv_50_filesystem.ino
  #endif

  #ifdef ENABLE_DEVFEATURE__SETTINGS_NEW_STRUCT_2023
  if (tkr_set->Settings2 == nullptr) {
    tkr_set->Settings2 = (mSettings::TSettings2*)malloc(sizeof(mSettings::TSettings2));
  }
  #endif
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE
  DEBUG_LINE_HERE

/********************************************************************************************
 ** Settings ********************************************************************************
 ********************************************************************************************/

  tkr_set->SettingsInit();

  #ifdef ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT
    EmergencySerial_SettingsReset();
  #endif  // ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT

  DEBUG_LINE_HERE
  tkr_sup->init_FirmwareVersion();
  DEBUG_LINE_HERE

   //preload minimal required
  DEBUG_LINE_HERE
  tkr_set->SettingsDefault();
  ALOG_DBG(PSTR("Loading minimal defaults"));
   // Overwrite with latest values, including template if new SETTINGS_CONFIG exists  
  ALOG_DBG(PSTR("Loading settings from saved memory"));
  DEBUG_LINE_HERE
  tkr_set->SettingsLoad();                   // Only the system level settings are loaded here, not the module settings which should happen below
  
  DEBUG_LINE_HERE
  tkr_set->SettingsDelta();
  DEBUG_LINE_HERE
  
  // sprintf(tkr_set->Settings.debug, "debug12\0");

/********************************************************************************************
 ** LOGGING: Set boot log levels again to override settings load *********************************************************************
 ********************************************************************************************/

  uint8_t saved_serial_loglevel = tkr_set->Settings.logging.serial_level;
  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;

  ALOG_INF(PSTR("Log level for boot: %d"), saved_serial_loglevel);

/********************************************************************************************
 ** System OSWatch: To detect loop hangs that might happen during (OTA) upgrades  ******************
 ********************************************************************************************/

#ifdef ENABLE_FEATURE_SYSTEM__OSWATCH_FOR_LOOP_HANGS
  // OsWatchInit();
#endif

/********************************************************************************************
 ** Fastboot ********************************************************************************
 ********************************************************************************************/

  /********************************************************************************************
   ** Fastboot: >> Configuration Recovery <<  
   *     - Loaded settings to determine how fastboot is configured
   *     - This code must run before drivers/sensors are initiated, so they may be disabled if recovery is required
  ********************************************************************************************/
  #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  Fastboot_RecoveryCheck();
  #endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION

/********************************************************************************************
 ** SERIAL: Change baud to module default if module has changed ****************************************
 ********************************************************************************************/

#ifdef ENABLE_FEATURE_BOOT__RESET_BAUDRATE_ON_BOOT_WITH_MODULE_CHANGE
  // TasmotaGlobal.module_changed = (Settings->module != Settings->last_module);
  // if (TasmotaGlobal.module_changed) {
  //   Settings->baudrate = APP_BAUDRATE / 300;
  //   Settings->serial_config = TS_SERIAL_8N1;
  // }
  // SetSerialBaudrate(Settings->baudrate * 300);  // Reset serial interface if current baudrate is different from requested baudrate
#endif // ENABLE_FEATURE_BOOT__RESET_BAUDRATE_ON_BOOT_WITH_MODULE_CHANGE

/********************************************************************************************
 ** Quick Power Cycle ***********************************************************************
 ********************************************************************************************/

#ifdef ENABLE_FEATURE_BOOT__QUICK_POWER_CYCLE_TO_CAUSE_MANUAL_RESET
  // if (1 == RtcReboot.fast_reboot_count) {      // Allow setting override only when all is well
  //   UpdateQuickPowerCycle(true);
  // }
#endif // ENABLE_FEATURE_BOOT__QUICK_POWER_CYCLE_TO_CAUSE_MANUAL_RESET

/********************************************************************************************
 ** Load Templates **************************************************************************
 ********************************************************************************************/
  
  // configure any memory address needed as part of module init or templates
  tkr->Tasker_Interface(TASK_POINTER_INIT);

  #ifdef ENABLE_DEVFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
  tkr->Tasker_Interface(TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM);
  #endif
  
  /**
   * @brief DEBUG: Load template from progmem and override settings to ensure my developing devices always enter a known state.
   *               This should be disabled for production devices, and only enabled for development devices.
   **/
  #ifdef ENABLE_FEATURE_TEMPLATES__LOAD_FROM_PROGMEM_TO_OVERRIDE_STORED_SETTINGS_TO_MAINTAIN_KNOWN_WORKING_VALUES
  ALOG_DBM(PSTR(D_LOG_MEMORY D_LOAD " Temporary loading any progmem templates"));
  tkr->Tasker_Interface(TASK_TEMPLATES__LOAD_MODULE); // loading module, only interface modules will have these
  #else
  #warning "FORCE_TEMPLATE_LOADING is disabled, and production/release is assumed. This REQUIRES valid settings/storage or device may be unstable"
  #endif
  
/********************************************************************************************
 ** Set RUNTIME log values *********************************************************************
 ********************************************************************************************/

  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;  
    
  /********************************************************************************************
   ** Initialise System and Modules ***********************************************************
  ********************************************************************************************/

  #ifdef ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  // Must be at the end of your setup
  // gpio_viewer.setSamplingInterval(25); // You can set the sampling interval in ms, if not set default is 100ms
  gpio_viewer.begin();
  #endif

  // Init the GPIOs
  tkr_pins->GpioInit();

  // Start pins in modules
  tkr->Tasker_Interface(TASK_PRE_INIT);

  // Init devices with safe compiled/fallback defaults
  tkr->Tasker_Interface(TASK_INIT);

  ALOG_INF(PSTR("TASK_INIT Complete\n\r------------------------------------------------------\n\r------------------------------------------------------"));

  // Init devices after basic module init
  tkr->Tasker_Interface(TASK_POST_INIT);

  // Run system functions
  tkr->Tasker_Interface(YTASK_INIT);

  // Load module-owned filesystem config.
  tkr->Tasker_Interface(TASK_INIT_LOAD_MODULE_CONFIG_FROM_FILESYSTEM);
  
  DEBUG_LINE_HERE
  // Init any dynamic memory buffers
  tkr->Tasker_Interface(TASK_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID);
  DEBUG_LINE_HERE


  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif  

  /********************************************************************************************
   ** File System : Load after settings for now, so this method overrides any defaults   ******
  ********************************************************************************************/

  #ifdef ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
  #ifdef ENABLE_FEATURE_FILESYSTEM__LOAD_MODULE_CONFIG_JSON_ON_BOOT
    tkr_mfile->JsonFile_Load__Stored_Module_Or_Default_Template();
  #endif
  #endif // ENABLE_SYSTEM_SETTINGS_IN_FILESYSTEM
  #ifdef ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT
  tkr->Tasker_Interface(TASK_FILESYSTEM__LOAD__MODULE_DATA__ID);
  #endif // ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT

  /**
   * This can only happen AFTER each module is running/enabled (port init checks). This will override the settings load, so should be tested if needed when settings work
   * */
  tkr->Tasker_Interface(TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM); //USED
  // Configure sensor/drivers to values desired for modules
  tkr->Tasker_Interface(TASK_CONFIGURE_MODULES_FOR_DEVICE); //??

  /********************************************************************************************
   ** MQTT: Configure mqtt handlers in modules   ******
  ********************************************************************************************/

  tkr->Tasker_Interface(TASK_MQTT_HANDLERS_INIT);  

  // Init the refresh periods for mqtt
  #ifndef ENABLE_DEBUGFEATURE_MQTT__DISABLE_SETTING_DYNAMIC_REFRESH_RATES
  tkr->Tasker_Interface(TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD);
  #endif

  /********************************************************************************************
   ** RULES: Configure mqtt handlers in modules   ******
  ********************************************************************************************/

  #ifdef USE_MODULE_CORE_RULES
  tkr->Tasker_Interface(TASK_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID);
  #endif 
  
/********************************************************************************************
 ** LOGGING: Set runtime log levels *********************************************************************
 ********************************************************************************************/

  tkr_set->Settings.logging.serial_level = saved_serial_loglevel;

  /********************************************************************************************
   ** // For debugging, allow method to override init/loaded values **************************************************************************
  ********************************************************************************************/
  
  #ifdef ENABLE_FUNCTION_DEBUG
    tkr->Tasker_Interface(TASK_DEBUG_CONFIGURE);
  #endif

  #ifdef ENABLE_BOOT_OVERRIDE_INIT
  tkr->Tasker_Interface(TASK_OVERRIDE_BOOT_INIT);
  #endif

  /********************************************************************************************
   ** Boot Completed **************************************************************************
  ********************************************************************************************/

  tkr->Tasker_Interface(TASK_ON_BOOT_COMPLETE);

  // Serial.println("END OF SETUP REACHED"); Serial.flush();

  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif  

  #ifdef ENABLE_FEATURE_SYSTEM__BOOT_SPLASH__DISPLAY_BLOCK_TO_SHOW_END_OF_INIT
  for(int i=0;i<(70*25)+1;i++){ // +1 so final line has \n
    Serial.print('='); 
    if(i % 70 == 0)
      Serial.println();
  }
  #endif

  tkr->Tasker_Interface(TASK_BOOT_MESSAGE); // Display status of system
  

}


void LoopTasker()
{
  // Serial.println("LOOP STARTED"); Serial.flush();

  // log_v("wifi heap=%u", ESP.getFreeHeap());

  // Serial.println("ArduinoOtaLoop passed STARTED"); Serial.flush();
   
  tkr->Tasker_Interface(TASK_LOOP); DEBUG_LINE;
 
  if(tkr_time->UpTime() > 30){ tkr->Tasker_Interface(YTASK_LOOP); } // Only run after stable boot
 
  if(mTime::TimeReached(&tkr_sup->tSavedLoop50mSec ,50  )){ tkr->Tasker_Interface(TASK_EVERY_50_MSECOND);  }  DEBUG_LINE;
  if(mTime::TimeReached(&tkr_sup->tSavedLoop100mSec,100 )){ tkr->Tasker_Interface(TASK_EVERY_100_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&tkr_sup->tSavedLoop250mSec,250 )){ tkr->Tasker_Interface(TASK_EVERY_250_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&tkr_sup->tSavedLoop1Sec   ,1000))
  {

    /**Since this only gets checked every second, we can use the uptime ticking to make sure it runs just once*/
    #ifdef ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS
    if(tkr_time->UpTime()==ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS){
      tkr->Tasker_Interface(TASK_PRE_INIT_DELAYED);     // Configure sub modules and classes as needed, should this be renamed to "INIT_PINS"?
      tkr->Tasker_Interface(TASK_INIT_DELAYED);         // Actually complete init, read sensors, enable modules fully etc
      tkr->Tasker_Interface(TASK_MQTT_HANDLERS_INIT_DELAYED);
    }
    #endif // ENABLE_DEBUGFEATURE_TASKER__DELAYED_START_OF_MODULES_SECONDS


    tkr->Tasker_Interface(TASK_EVERY_SECOND); 

    if(
      ((tkr_time->UpTime()%5)==0)&&
      (tkr_time->UpTime()>20)
    ){                                      tkr->Tasker_Interface(TASK_EVERY_FIVE_SECOND); }
    if(
      ((tkr_time->UpTime()%10)==0)&&
      (tkr_time->UpTime()>20)
    ){                                      tkr->Tasker_Interface(TASK_EVERY_10_SECONDS); }

    if((tkr_time->UpTime()%30)==0){                  tkr->Tasker_Interface(TASK_EVERY_30_SECOND); }

    if((tkr_time->UpTime()%60)==0){                  tkr->Tasker_Interface(TASK_EVERY_MINUTE); }
    

    if(
      ((tkr_time->UpTime()%300)==0)&&
      (tkr_time->UpTime()>60)
    ){                                    tkr->Tasker_Interface(TASK_EVERY_FIVE_MINUTE); }
    if(
      ((tkr_time->UpTime()%18000)==0)&&
      (tkr_time->UpTime()>60)
    ){                                    tkr->Tasker_Interface(TASK_EVERY_30_MINUTES); }

    // Uptime triggers: Fire Once (based on uptime seconds, but due to this function being called every second, it will only fire once)
    if(tkr_time->UpTime() == 10){   tkr->Tasker_Interface(TASK_UPTIME_10_SECONDS); }
    if(tkr_time->UpTime() == 30){   tkr->Tasker_Interface(TASK_UPTIME_30_SECONDS); }
    if(tkr_time->UpTime() == 60){   tkr->Tasker_Interface(TASK_UPTIME_1_MINUTES); }
    if(tkr_time->UpTime() == 600){   tkr->Tasker_Interface(TASK_UPTIME_10_MINUTES); }
    if(tkr_time->UpTime() == 36000){ tkr->Tasker_Interface(TASK_UPTIME_60_MINUTES); }

    // Check for midnight
    if((tkr_time->RtcTime.hour==0)&&(tkr_time->RtcTime.minute==0)&&(tkr_time->RtcTime.second==0)&&(tkr_time->lastday_run != tkr_time->RtcTime.day_of_year)){
      tkr_time->lastday_run = tkr_time->RtcTime.day_of_year;
      tkr->Tasker_Interface(TASK_EVERY_MIDNIGHT); 
    }

    if(tkr_time->UpTime()==10){
      ALOG_INF(PSTR("Boot Message>>>>>>>>>>>>>>>>>>>"));
      tkr->Tasker_Interface(TASK_BOOT_MESSAGE);
      ALOG_INF(PSTR("Boot Message<<<<<<<<<<<<<<<<<<<"));
    }

    if(tkr_time->UpTime()==120){       tkr->Tasker_Interface(TASK_ON_BOOT_SUCCESSFUL);}
      
    tkr->Tasker_Interface(TASK_INIT_DELAYED_SECONDS);

  } // END secondloop

    
  #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE
  if(tkr->function_event_queue.size())
  {
    DEBUG_LINE_HERE;
    bool execute_function = false;
    uint8_t iter_count = 0;
    for(auto& queue:tkr->function_event_queue)
    {
      if(queue.delay_millis == 0){ execute_function = true; } // no delay
      if(mTime::TimeReached(&queue.tSaved_millis,queue.delay_millis)){ execute_function = true; }
    DEBUG_LINE_HERE;

      if(execute_function)
      {
        ALOG_HGL(PSTR("Executing Event Queue Item [%d]: func_id %d"), iter_count, queue.function_id);
    DEBUG_LINE_HERE;
        tkr->Tasker_Interface(queue.function_id);
    DEBUG_LINE_HERE;
    // std::vector<mTaskerManager::FUNCTION_EXECUTION_EVENT>::iterator index = tkr->function_event_queue.begin()+iter_count;

    
        ALOG_INF(PSTR("erase %d/%d"), iter_count, tkr->function_event_queue.size());

        tkr->function_event_queue.erase(tkr->function_event_queue.begin()+iter_count);    
    DEBUG_LINE_HERE;   
      }
      iter_count++;
    }
    DEBUG_LINE_HERE;
  }
  #endif // ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE

}

/********************************************************************************************/
/*********************loop*******************************************************************/
/********************************************************************************************/

#ifdef ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY
void SmartLoopDelay()
{
  tkr_sup->SleepDelay(20);

  // #ifndef DISABLE_SLEEP
  // if(tkr_set->Settings.enable_sleep){
  //   if (tkr_set->Settings.network.flag.sleep_normal) {
  //     tkr_sup->SleepDelay(tkr_set->runtime.sleep);
  //   } else {
  //     // Loop time < sleep length of time
  //     if (tkr_sup->loop_runtime_millis < (uint32_t)tkr_set->runtime.sleep) {
  //       //delay by loop time
  //       tkr_sup->SleepDelay((uint32_t)tkr_set->runtime.sleep - tkr_sup->loop_runtime_millis);  // Provide time for background tasks like wifi
  //     } else {

  //       // if loop takes longer than sleep period, no delay, IF wifi is down, devote half loop time to wifi connect 
  //       // If wifi down and loop_runtime_millis > setoption36 then force loop delay to 1/3 of loop_runtime_millis period
  //       if (tkr_set->global_state.wifi_down) {
  //         tkr_sup->SleepDelay(tkr_sup->loop_runtime_millis /2);
  //       }

  //     }
  //   }
  // }

  
  // #endif
}
#endif // ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY


void loop(void)
{
  tkr_sup->activity.loop_counter++;
  tkr_sup->loop_start_millis = millis();
  
  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif
  
  LoopTasker();
  
  tkr_sup->loop_runtime_millis = millis() - tkr_sup->loop_start_millis;

  if(mTime::TimeReached(&tkr_set->runtime.tSavedUpdateLoopStatistics, 1000)){
    tkr_sup->activity.cycles_per_sec = tkr_sup->activity.loop_counter; 
    #ifdef ENABLE_DEBUGFEATURE__SPLASH__LOOPS_PER_SECOND
    ALOG_INF(PSTR("LOOPSEC = \t\t\t\tLPS=%d, LoopTime=%d"), tkr_sup->activity.cycles_per_sec, tkr_sup->loop_runtime_millis);
    #endif
    tkr_sup->activity.loop_counter=0;
  }

  // if(tkr_sup->loop_runtime_millis > 500)
  // {
  //   ALOG_ERR(PSTR("LONG_LOOP ============= %d %d %d"), tkr_sup->activity.loop_counter, tkr_sup->activity.cycles_per_sec, tkr_sup->loop_runtime_millis);
  // }

  #ifdef ENABLE_FEATURE_CORESYSTEM__SMART_LOOP_DELAY
  SmartLoopDelay();
  #endif

  if (!tkr_sup->loop_runtime_millis) { tkr_sup->loop_runtime_millis++; } // We cannot divide by 0
  tkr_sup->loop_delay_temp = tkr_set->runtime.sleep; 
  if (!tkr_sup->loop_delay_temp) { tkr_sup->loop_delay_temp++; }              // We cannot divide by 0
  tkr_sup->loops_per_second = 1000 / tkr_sup->loop_delay_temp;  // We need to keep track of this many loops per second, 20ms delay gives 1000/20 = 50 loops per second (50hz)
  tkr_sup->this_cycle_ratio = 100 * tkr_sup->loop_runtime_millis / tkr_sup->loop_delay_temp;
  tkr_set->runtime.loop_load_avg = tkr_set->runtime.loop_load_avg - (tkr_set->runtime.loop_load_avg / tkr_sup->loops_per_second) + (tkr_sup->this_cycle_ratio / tkr_sup->loops_per_second); // Take away one loop average away and add the new one

}
