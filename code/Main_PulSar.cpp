/**
 * @file    PulSar.cpp
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

  #ifdef DEVICENAME_BUILD_ENVIRONMENT
    Serial.printf("Build Environment: %s\r\n", DEVICENAME_BUILD_ENVIRONMENT);
  #else
    Serial.println(F("Build Environment:"));
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
/********************* ENABLE_FEATURE_FASTBOOT__DETECTION ******************************************************************/
/********************************************************************************************/


#ifdef ENABLE_FEATURE_FASTBOOT__DETECTION
void Fastboot_RecoveryCheck(void)
{
  ALOG_DBM(PSTR("ARESET TWICE! \t\t\t%d"), RtcMemory__BootState.fast_reboot_count);

  const uint8_t boot_loop_offset = tkr_set->Settings.sysopt_system.param.boot_loop_offset;

  if (!boot_loop_offset) {
    return;
  }

  // Not yet considered a boot loop
  if (RtcMemory__BootState.fast_reboot_count <= boot_loop_offset) {
    return;
  }

  const uint8_t fastboot_stage = RtcMemory__BootState.fast_reboot_count - boot_loop_offset;

  ALOG_DBM(
    PSTR("Fastboot: Recovery stage %d, fast_reboot_count=%d, offset=%d"),
    fastboot_stage,
    RtcMemory__BootState.fast_reboot_count,
    boot_loop_offset
  );

  // --------------------------------------------------------------------------
  // Stage 1:
  // First detected fast reboot beyond offset.
  // Log only / reserve for very light test-disable behaviour.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 1) {
    ALOG_DBG(PSTR("Fastboot: Stage 1 - detected boot loop, no destructive action"));
  }

  // --------------------------------------------------------------------------
  // Stage 2:
  // Disable newest experimental/test code.
  // Modules may listen for this event and disable risky dev features.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 2) {
    ALOG_DBG(PSTR("Fastboot: Stage 2 - FASTBOOT_EVENT_1"));
    tkr->Tasker_Interface(TASK_FASTBOOT_EVENT_1);
  }

  // --------------------------------------------------------------------------
  // Stage 3:
  // Disable broader experimental code.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 3) {
    ALOG_DBG(PSTR("Fastboot: Stage 3 - FASTBOOT_EVENT_2"));
    tkr->Tasker_Interface(TASK_FASTBOOT_EVENT_2);
  }

  // --------------------------------------------------------------------------
  // Stage 4:
  // Disable rules first. Rules cross-link modules and are high-risk during boot.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 4) {
    ALOG_DBG(PSTR("Fastboot: Stage 4 - Disable rules"));

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
    ALOG_DBG(PSTR("Fastboot: Stage 5 - Disable sensors"));

    tkr_set->runtime.fastboot.disable_sensors = true;
  }

  // --------------------------------------------------------------------------
  // Stage 6:
  // Disable drivers / actuators.
  // Keep network, OTA, WebUI, filesystem, logging alive.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 6) {
    ALOG_DBG(PSTR("Fastboot: Stage 6 - Disable drivers"));

    tkr_set->runtime.fastboot.disable_drivers = true;
  }

  // --------------------------------------------------------------------------
  // Stage 7:
  // Skip stored module JSON/config loads.
  // This recovers from bad module config files, not bad core settings.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 7) {
    ALOG_DBG(PSTR("Fastboot: Stage 7 - Disable module filesystem config load"));

    tkr_set->runtime.fastboot.disable_module_config_load = true;
  }

  // --------------------------------------------------------------------------
  // Stage 8:
  // Skip templates and use compiled-safe module configuration.
  // --------------------------------------------------------------------------
  if (fastboot_stage >= 8) {
    ALOG_DBG(PSTR("Fastboot: Stage 8 - Disable templates, force safe compiled config"));

    tkr_set->runtime.fastboot.disable_templates = true;
    tkr_set->runtime.fastboot.force_safe_compiled_config = true;
  }

  // --------------------------------------------------------------------------
  // Stage 9:
  // Reset settings to defaults, but force known STA WiFi for OTA recovery.
  // First destructive recovery stage.
  // --------------------------------------------------------------------------
  if (fastboot_stage == 9) {
    ALOG_DBG(PSTR("Fastboot: Stage 9 - Reset settings to defaults with STA recovery"));

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
    SafeMode_StartAndAwaitOTA();
  }

  ALOG_INF(PSTR("FRC: " D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcMemory__BootState.fast_reboot_count);
}
#endif // ENABLE_FEATURE_FASTBOOT__DETECTION


/********************************************************************************************/
/********************* SETUP ****************************************************************/
/********************************************************************************************/
/************************************************************************************************
 * FUNCTION: setup
 *
 * PURPOSE
 * -------
 * Boot the PulSar runtime in a deterministic order:
 *
 *   - bring up minimum hardware/recovery support,
 *   - initialise core services,
 *   - build RAM defaults,
 *   - load persistent settings,
 *   - apply recovery policy,
 *   - initialise GPIO/modules,
 *   - load late templates and module-owned filesystem config,
 *   - start runtime services.
 *
 *
 * BOOT PHASES
 * -----------
 *
 *  0. Early hardware / recovery layer
 *
 *     Runs before normal settings, filesystem config, and module activation.
 *
 *     Responsibilities:
 *       - disable brownout if configured,
 *       - start early Serial for crash/fastboot diagnostics,
 *       - start watchdog,
 *       - update RTC fastboot/reboot counter,
 *       - enter SafeMode OTA/cellular recovery if crash count is excessive.
 *
 *     Rule:
 *       This layer must not depend on loaded settings, module config, or normal AddLog state.
 *
 *
 *  1. Core bootstrap
 *
 *     Responsibilities:
 *       - restore minimal RTC runtime settings where available,
 *       - print boot splash,
 *       - start shared JSON/data buffers,
 *       - initialise global module pointers with tkr->Instance_Init(),
 *       - initialise internal filesystem.
 *
 *
 *  2. Settings
 *
 *     SettingsDefault():
 *       - builds compiled binary defaults in RAM,
 *       - does not save,
 *       - does not erase existing settings.
 *
 *     MODULE_TEMPLATE / device manifest:
 *       - compile-time device identity and GPIO mapping,
 *       - seeded into RAM defaults before SettingsLoad(),
 *       - saved settings override this unless USE_MODULE_TEMPLATE__OVERRIDE is enabled.
 *
 *     SettingsLoad():
 *       - loads /settings.txt if valid,
 *       - validates holder, size, and CRC,
 *       - restores last-known-good on appropriate corruption paths,
 *       - falls back to RAM defaults if storage is missing/invalid/incompatible.
 *
 *     SettingsDelta():
 *       - applies version/layout migration for valid loaded settings,
 *       - is not a default builder,
 *       - is not a module config loader.
 *
 *
 *  3. Fastboot recovery after settings
 *
 *     Fastboot_RecoveryCheck():
 *       - runs after settings are available,
 *       - applies staged recovery flags,
 *       - can disable rules, sensors, drivers, templates, and module filesystem config,
 *       - must run before GPIO/module init and before late config loading.
 *
 *
 *  4. Pointer initialisation
 *
 *     TASK_POINTER_INIT:
 *       - gives modules their cross-module pointers,
 *       - must run before GPIO/module init.
 *
 *
 *  5. GPIO and module initialisation
 *
 *     Order:
 *       - GpioInit(),
 *       - TASK_PRE_INIT,
 *       - TASK_INIT,
 *       - TASK_POST_INIT,
 *       - YTASK_INIT.
 *
 *     Rule:
 *       GPIO/device identity must already be established before this phase.
 *
 *
 *  6. Normal post-init compile-time defaults
 *
 *     TASK_CONFIG_LOAD_POST_INIT_DEFAULTS_FROM_PROGMEM:
 *       - normal boot path,
 *       - runs after modules exist,
 *       - uses module JSON command handlers where required,
 *       - applies late defaults such as LIGHTING_TEMPLATE, FUNCTION_TEMPLATE, RULES_TEMPLATE,
 *       - is not GPIO/device identity,
 *       - is not the development override pass.
 *
 *
 *  7. Module-owned filesystem config/data
 *
 *     TASK_INIT_LOAD_MODULE_CONFIG_FROM_FILESYSTEM:
 *       - loads module-owned config files.
 *
 *     TASK_FILESYSTEM__LOAD__MODULE_DATA__ID:
 *       - loads optional module-owned state/data/presets.
 *
 *     Examples:
 *       - module_lighting_config.json,
 *       - module_lighting_presets.json,
 *       - module_rules_config.json,
 *       - module_sensor_calibration.json.
 *
 *     Rule:
 *       These files override normal compile-time defaults.
 *
 *
 *  8. Development compile-time override pass
 *
 *     TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM:
 *       - late development/testing enforcement path,
 *       - runs after normal defaults and filesystem config,
 *       - only override templates should apply here,
 *       - used when compiled templates must win over filesystem/web/UI/user config.
 *
 *
 *  9. Dynamic memory refresh
 *
 *     TASK_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID:
 *       - runs after templates and module filesystem config,
 *       - allows modules to resize/rebuild buffers after config/presets are known.
 *
 *
 * 10. Runtime services
 *
 *     Responsibilities:
 *       - optional debug export of compile-time templates to filesystem,
 *       - initialise MQTT handlers,
 *       - configure default MQTT transmit periods,
 *       - add default GPIO-derived rules if rules are not disabled,
 *       - restore saved runtime log level,
 *       - apply debug/boot override init hooks,
 *       - mark boot complete,
 *       - emit boot message.
 *
 *
 * CURRENT HIGH-LEVEL FLOW
 * -----------------------
 *
 *   [Reset]
 *      |
 *      v
 *   [Early Serial / WDT / RTC fastboot counter]
 *      |
 *      v
 *   [Optional SafeMode OTA/cellular recovery]
 *      |
 *      v
 *   [RTC runtime settings]
 *      |
 *      v
 *   [Serial splash]
 *      |
 *      v
 *   [JsonBuilder / BufferWriter]
 *      |
 *      v
 *   [tkr->Instance_Init()]
 *      |
 *      v
 *   [Filesystem Init]
 *      |
 *      v
 *   [SettingsInit]
 *      |
 *      v
 *   [SettingsDefault]
 *      |
 *      v
 *   [MODULE_TEMPLATE seed into RAM defaults]
 *      |
 *      v
 *   [SettingsLoad]
 *      |
 *      v
 *   [SettingsDelta]
 *      |
 *      v
 *   [Optional MODULE_TEMPLATE hard override]
 *      |
 *      v
 *   [Fastboot_RecoveryCheck]
 *      |
 *      v
 *   [TASK_POINTER_INIT]
 *      |
 *      v
 *   [GpioInit]
 *      |
 *      v
 *   [TASK_PRE_INIT]
 *      |
 *      v
 *   [TASK_INIT]
 *      |
 *      v
 *   [TASK_POST_INIT / YTASK_INIT]
 *      |
 *      v
 *   [TASK_CONFIG_LOAD_POST_INIT_DEFAULTS_FROM_PROGMEM]
 *      |
 *      v
 *   [TASK_INIT_LOAD_MODULE_CONFIG_FROM_FILESYSTEM]
 *      |
 *      v
 *   [TASK_FILESYSTEM__LOAD__MODULE_DATA__ID]
 *      |
 *      v
 *   [TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM]
 *      |
 *      v
 *   [TASK_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID]
 *      |
 *      v
 *   [Optional template export to filesystem]
 *      |
 *      v
 *   [MQTT handlers]
 *      |
 *      v
 *   [Rules default GPIO binding]
 *      |
 *      v
 *   [Restore runtime log level]
 *      |
 *      v
 *   [Debug/override init]
 *      |
 *      v
 *   [TASK_ON_BOOT_COMPLETE]
 *      |
 *      v
 *   [TASK_BOOT_MESSAGE]
 ************************************************************************************************/


void setup(void)
{ 

  /********************************************************************************************
   ** Brownout / Early ESP32 Hardware *********************************************************
  ********************************************************************************************/

  #ifdef ESP32

    #ifdef DISABLE_ESP32_BROWNOUT
      DisableBrownout();      // Workaround possible weak LDO resulting in brownout detection during Wifi connection
    #endif  // DISABLE_ESP32_BROWNOUT

    /************************************************************************************************
     * SECTION: Early Serial
     *
     * Keep this early for fastboot/recovery/crash debug.
     * Normal baudrate refinement can happen later after RTC/settings are available.
     ************************************************************************************************/

    Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT); // to be baudrate_tmp later

    #ifdef ENABLE_DEVFEATURE_SETDEBUGOUTPUT
    Serial.setDebugOutput(true);
    #endif

    #ifdef CONFIG_IDF_TARGET_ESP32
    /************************************************************************************************
     * SECTION: Restore GPIO16/17 when PSRAM is absent
     *
     * These pins can be reserved for PSRAM on some ESP32 packages. If no PSRAM is detected,
     * release them so normal GPIO use can work.
     ************************************************************************************************/
    if (!SupportESP32::FoundPSRAM())
    {
      uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
      uint32_t pkg_version = chip_ver & 0x7;

      if (pkg_version <= 3) {   // D0WD, S0WD, D2WD
        gpio_reset_pin(GPIO_NUM_16);
        gpio_reset_pin(GPIO_NUM_17);
      }
    }
    #endif  // CONFIG_IDF_TARGET_ESP32

    #ifdef USE_MODULE_DRIVERS__CAMERA
    /************************************************************************************************
     * SECTION: Camera PSRAM early init/debug
     ************************************************************************************************/
    psramInit();

    // Serial.printf("psramFound: %d\n\r", psramFound());
    // Serial.printf("esp_spiram_is_initialized: %d\n\r", esp_spiram_is_initialized());
    // Serial.printf("Free PSRAM: %u\n\r", ESP.getFreePsram());
    #endif

  #endif  // ESP32


  /********************************************************************************************
   ** Watchdog ********************************************************************************
  ********************************************************************************************/

  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    WDT_Init();
  #else
    #warning "No WDT has been enabled, this is not recommended for production code!"
  #endif


  /********************************************************************************************
   ** Fastboot: Base Setup Recovery ***********************************************************
  *
  * Purpose:
  * - Detects repeated crashes before normal settings/modules are loaded.
  * - Can enter OTA/cellular recovery before the crashing subsystem is reached.
  *
  * Code priority:
  * - High, before normal settings load and module init.
  *
  * Notes:
  * - AddLog cannot be used here.
  ********************************************************************************************/

  #ifdef DEBUG_FASTBOOT
    Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT);
    Serial.println(F("\n\rSerial Enabled Early for FastBoot Debug" DEBUG_INSERT_PAGE_BREAK));
  #endif

  #ifdef ENABLE_FEATURE_FASTBOOT__DETECTION

    RtcMemory__BootState_Load();

    if (!RtcMemory__BootState_Valid())
    {
      RtcMemory__BootState.fast_reboot_count = 0;
    }

    /************************************************************************************************
     * SECTION: Good boot source
     *
     * Deep sleep wake is not treated as a crash/reboot loop.
     ************************************************************************************************/

    if (ResetReason_g() == REASON_DEEP_SLEEP_AWAKE)
    {
      RtcMemory__BootState.fast_reboot_count = 0;
    }

    /************************************************************************************************
     * SECTION: Normal/bad boot source
     *
     * Increment early so repeated crashes later in setup are detected.
     ************************************************************************************************/

    else
    {
      RtcMemory__BootState.fast_reboot_count++;

      #ifdef DEBUG_FASTBOOT
      Serial.printf("FastBoot: Count %d\n\r", RtcMemory__BootState.fast_reboot_count);
      #endif
    }

    Serial.printf("FastBoot: Count %u\r\n", RtcMemory__BootState.fast_reboot_count);

    RtcMemory__BootState_Save();

    /************************************************************************************************
     * SECTION: OTA/HTTP recovery fallback
     ************************************************************************************************/

    #if defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) || defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)
    if (RtcMemory__BootState.fast_reboot_count > 10)
    {
      SafeMode_StartAndAwaitOTA();
    }
    #endif

  #endif // ENABLE_FEATURE_FASTBOOT__DETECTION


  /********************************************************************************************
   ** Forced crash test ***********************************************************************
  ********************************************************************************************/

  #ifdef ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING
    Serial.flush();
    delay(1000);
    tkr_sup->CmndCrash();
  #endif

/********************************************************************************************
 ** RTC Settings ****************************************************************************
 ********************************************************************************************/
  
  uint32_t baudrate_tmp = 115200;

  #ifdef ENABLE_FEATURE_RTC__SETTINGS
  if (RtcMemory__RuntimeState_Load(0)) {
    uint32_t baudrate = (RtcMemory__RuntimeState.baudrate / 300) * 300;
    if (baudrate) { baudrate_tmp = baudrate; }
  }
  #endif

/********************************************************************************************
 ** Serial **********************************************************************************
 ********************************************************************************************/
 
  #ifndef DISABLE_SERIAL0_CORE
  Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT);
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

  // Serial.printf("baudrate_tmp = %d\n\r", baudrate_tmp);

  #ifdef ENABLE_DEVFEATURE_SETDEBUGOUTPUT
  Serial.setDebugOutput(true);
  #endif 

/********************************************************************************************
 ** Init Pointers ***************************************************************************
 ********************************************************************************************/
 
  JsonBuilderI ->Start(data_buffer.payload.ctr, data_buffer.payload.length_used, DATA_BUFFER_PAYLOAD_MAX_LENGTH);  
  BufferWriterI->Start(data_buffer.payload.ctr, data_buffer.payload.length_used, DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  
  tkr->Instance_Init();

/********************************************************************************************
 ** LOGGING: Set boot log levels ************************************************************
 ********************************************************************************************/

  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;

  ALOG_DBM(PSTR("AddLog Started"));

  #ifdef ENABLE_DEBUGFEATURE_LOGGING__ENABLE_TELNET_IMMEDIATE_WITH_WAIT
  ALOG_DBG(PSTR("Early Wifi connection attempt disabled/commented"));
  #endif

/********************************************************************************************
 ** Splash boot reason **********************************************************************
 ********************************************************************************************/

  ALOG_DBG(PSTR("ResetReason=%d"), ResetReason_g());

/********************************************************************************************
 ** Hardware splash *************************************************************************
 ********************************************************************************************/

  #ifdef ESP32
  ALOG_DBG(
    PSTR("HDW: %s %s"),
    SupportESP32::GetDeviceHardwareRevision().c_str(),
    SupportESP32::FoundPSRAM() ? (SupportESP32::CanUsePSRAM() ? "(PSRAM)" : "(PSRAM disabled)") : ""
  );

    #if !defined(HAS_PSRAM_FIX)
    if (SupportESP32::FoundPSRAM() && !SupportESP32::CanUsePSRAM()) {
      ALOG_DBG(PSTR("HDW: PSRAM is disabled, requires specific compilation on this hardware (see doc)"));
    }
    #endif
  #endif

/********************************************************************************************
 ** File System : Init **********************************************************************
 ********************************************************************************************/

#ifdef USE_MODULE_CORE_FILESYSTEM
tkr_mfile->Pre_Init();
tkr_mfile->Init();
#endif

tkr_jsona->Init();

/********************************************************************************************
 ** Settings ********************************************************************************
 *
 * SettingsDefault():
 * - Builds compiled binary defaults in RAM.
 *
 * SettingsLoad():
 * - Loads saved binary settings if valid.
 * - Falls back to defaults if missing/invalid.
 * - Owns early MODULE_TEMPLATE / device-manifest policy.
 *
 * SettingsDelta():
 * - Applies version-dependent settings migration/fixes.
 ********************************************************************************************/
tkr_set->SettingsInit();

#ifdef ENABLE_FEATURE_RESET__EMERGENCY_SERIAL_SETTINGS_RESET_TO_DEFAULT
EmergencySerial_SettingsReset();
#endif

tkr_sup->init_FirmwareVersion();

/**
 * Build compiled binary defaults in RAM.
 */
tkr_set->SettingsDefault();

/**
 * Seed compiled MODULE_TEMPLATE into the default RAM settings.
 *
 * If SettingsLoad() finds no valid saved settings, these seeded defaults remain.
 * If SettingsLoad() finds valid saved settings, saved settings overwrite this.
 */
// tkr_json_template->ModuleDeviceTemplate__LoadDefault();

ALOG_DBG(PSTR("Loading settings from saved memory"));

tkr_set->SettingsLoad();
tkr_set->SettingsDelta();

/**
 * Optional hard firmware override.
 *
 * This deliberately re-applies the compiled MODULE_TEMPLATE after saved settings
 * and migration/delta handling, so the firmware template wins.
 */
#ifdef USE_MODULE_TEMPLATE__OVERRIDE
ALOG_WRN(PSTR("SET: USE_MODULE_TEMPLATE__OVERRIDE active, forcing compiled module template"));
tkr_json_template->ModuleDeviceTemplate__LoadDefault();
#endif

/********************************************************************************************
 ** LOGGING: Set boot log levels again to override settings load *****************************
 ********************************************************************************************/

  uint8_t saved_serial_loglevel = tkr_set->Settings.logging.serial_level;
  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;

  ALOG_DBG(PSTR("Log level for boot: %d"), saved_serial_loglevel);

/********************************************************************************************
 ** Fastboot ********************************************************************************
 ********************************************************************************************/

  #ifdef ENABLE_FEATURE_FASTBOOT__DETECTION
  Fastboot_RecoveryCheck();
  #endif

 /********************************************************************************************
   ** Pointer Init ****************************************************************************
   *
   * Must run before GPIO/module init.
   *
   * MODULE_TEMPLATE / device-manifest loading is not called directly from setup().
   * SettingsLoad() owns that early policy.
  ********************************************************************************************/
  
  tkr->Tasker_Interface(TASK_POINTER_INIT);

  /********************************************************************************************
   ** Set RUNTIME log values ******************************************************************
  ********************************************************************************************/

  tkr_set->runtime.seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  #ifdef ENABLE_LOGGING__BOOT_LOGLEVEL_SET_BY_FASTBOOT_COUNTER
  #ifdef ENABLE_FEATURE_FASTBOOT__DETECTION
  uint8_t fastboot_count = RtcMemory__BootState.fast_reboot_count;
  if(fastboot_count == 0){ tkr_set->runtime.seriallog_level_during_boot = LOG_LEVEL_DEBUG; } //normal boot
  if(fastboot_count >= 1){ tkr_set->runtime.seriallog_level_during_boot = LOG_LEVEL_INFO; }
  if(fastboot_count >= 4){ tkr_set->runtime.seriallog_level_during_boot = LOG_LEVEL_DEBUG_LOWLEVEL; }
  ALOG_DBG(PSTR("seriallog by fastboot %d:%s"), fastboot_count, tkr_log->GetLogLevelNamebyID(tkr_set->runtime.seriallog_level_during_boot));
  #endif
  #endif

  tkr_set->Settings.logging.serial_level = tkr_set->runtime.seriallog_level_during_boot;  
  // tkr_set->Settings.logging.serial_level = LOG_LEVEL_DEBUG_LOWLEVEL;//tkr_set->runtime.seriallog_level_during_boot;  

  /********************************************************************************************
   ** Initialise System and Modules ***********************************************************
  *
  * Order is critical:
  * - GpioInit()
  * - TASK_PRE_INIT
  * - TASK_INIT
  * - TASK_POST_INIT
  *
  * GPIO/device identity must already be established before GpioInit().
  ********************************************************************************************/

  #ifdef ENABLE_DEVFEATURE_PINS__GPIO_VIEWER_LIBRARY
  gpio_viewer.begin();
  #endif

  tkr_pins->GpioInit();

  tkr_pins->PinTable_SerialPrint("after GpioInit");
  
  tkr->Tasker_Interface(TASK_PRE_INIT);
  
  tkr->Tasker_Interface(TASK_INIT);

  ALOG_DBG(PSTR("TASK_INIT Complete\n\r------------------------------------------------------\n\r------------------------------------------------------"));
  
  tkr->Tasker_Interface(TASK_POST_INIT);
  tkr->Tasker_Interface(YTASK_INIT);
  
  /********************************************************************************************
   ** Normal post-init PROGMEM default config **************************************************
  *
  * This is part of normal boot.
  *
  * Examples:
  * - LIGHTING_TEMPLATE
  * - FUNCTION_TEMPLATE
  * - RULES_TEMPLATE
  * - module/class defaults that require active modules and JSON command parsers
  *
  * This is not the development override pass.
  * This is not used for GPIO mapping or device identity.
  ********************************************************************************************/
  
  tkr->Tasker_Interface(TASK_CONFIG_LOAD_POST_INIT_DEFAULTS_FROM_PROGMEM);

  /********************************************************************************************
   ** Module-owned filesystem config **********************************************************
  *
  * Optional module-owned config/state/presets from filesystem.
  *
  * Examples:
  * - module_lighting_config.json
  * - module_lighting_presets.json
  * - module_rules_config.json
  *
  * These should override normal compile-time defaults.
  ********************************************************************************************/

  #ifndef ENABLE_FEATURE_FASTBOOT__DISABLE_MODULE_FILESYSTEM_CONFIG
  tkr->Tasker_Interface(TASK_INIT_LOAD_MODULE_CONFIG_FROM_FILESYSTEM);
  
  #ifdef ENABLE_DEVFEATURE_STORAGE__LOAD_TRIGGER_DURING_BOOT
  tkr->Tasker_Interface(TASK_FILESYSTEM__LOAD__MODULE_DATA__ID);
  #endif
  #endif  

 /************************************************************************************************
   * SECTION: Development compile-time template enforcement
   *
   * Purpose:
   * - Development/testing pass only.
   * - Forces late compile-time templates over normal PROGMEM defaults, filesystem config,
   *   web UI changes, and saved user changes when matching *_OVERRIDE defines are enabled.
   *
   * Notes:
   * - Must run after module init because it uses TASK_JSON_COMMAND_ID internally.
   * - Must run after filesystem config if the override should win.
   * - Does not handle GPIO/device identity.
   *
   * Important:
   * - This calls the JSON template module directly.
   * - It avoids broadcasting TASK_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM through every module.
   * - This prevents mSettings and mJsonTemplate from both executing the same override pass.
   ************************************************************************************************/
  
  #if defined(USE_MODULE_CORE__JSON_TEMPLATE) || defined(USE_MODULE_CORE_JSON_TEMPLATE)
  if (tkr_json_template)
  {
    tkr_json_template->ModuleDeviceTemplate_CompileTime_DevelopmentOverridePass();
  }
  #endif  

  /********************************************************************************************
   ** Dynamic memory buffers ******************************************************************
   *
   * Runs after normal defaults, filesystem config/data, and late override templates.
  ********************************************************************************************/
  
  tkr->Tasker_Interface(TASK_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID);

  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif
  
  /********************************************************************************************
   ** SECTION: Debug/export hardcoded templates into filesystem *******************************
   *
   * Diagnostic only.
   * Creates /orig_template_*.json files for /edit viewing.
   * Not part of boot-critical config loading.
  ********************************************************************************************/

  #ifdef ENABLE_DEBUGFEATURE__FILESYSTEM__LOAD_HARDCODED_TEMPLATES_INTO_FILESYSTEM
  #ifdef USE_MODULE_CORE_FILESYSTEM
  if (tkr_mfile && tkr_mfile->IsMounted())
  {
    tkr->Tasker_Interface(TASK_TEMPLATES__MOVE_HARDCODED_TEMPLATES_INTO_FILESYSTEM);
  }
  else
  {
    ALOG_WRN(PSTR("Template export skipped, filesystem not mounted"));
  }
  #endif
  #endif 

/********************************************************************************************
 ** MQTT: Configure mqtt handlers in modules ************************************************
 ********************************************************************************************/

  tkr->Tasker_Interface(TASK_TELEMETRY_HANDLERS_INIT);  

  #ifndef ENABLE_DEBUGFEATURE_MQTT__DISABLE_SETTING_DYNAMIC_REFRESH_RATES
  tkr->Tasker_Interface(TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD);
  #endif

/********************************************************************************************
 ** RULES ***********************************************************************************
 ********************************************************************************************/

  #ifdef USE_MODULE_CORE_RULES
  tkr->Tasker_Interface(TASK_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID);
  #endif 

/********************************************************************************************
 ** LOGGING: Set runtime log levels *********************************************************
 ********************************************************************************************/

  tkr_set->Settings.logging.serial_level = saved_serial_loglevel;

/********************************************************************************************
 ** Debug / Override Init *******************************************************************
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

  tkr->Tasker_Interface(TASK_BOOT_MESSAGE);
  
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

    DEBUG_LINE_HERE;


    /***
     * Upseconds require valid time and be running so not to immediate trigger on uptime=0 at boot
     */
    if(tkr_time->UptimeValid())
    {

      if((tkr_time->UpTime()%5)==0){     tkr->Tasker_Interface(TASK_EVERY_FIVE_SECOND); }
      if((tkr_time->UpTime()%10)==0){    tkr->Tasker_Interface(TASK_EVERY_10_SECONDS); }
      if((tkr_time->UpTime()%30)==0){    tkr->Tasker_Interface(TASK_EVERY_30_SECOND); }
      if((tkr_time->UpTime()%60)==0){    tkr->Tasker_Interface(TASK_EVERY_MINUTE); }
      if((tkr_time->UpTime()%300)==0){   tkr->Tasker_Interface(TASK_EVERY_FIVE_MINUTE); }
      if((tkr_time->UpTime()%18000)==0){ tkr->Tasker_Interface(TASK_EVERY_30_MINUTES); }

      // Uptime triggers: Fire Once (based on uptime seconds, but due to this function being called every second, it will only fire once)
      if(tkr_time->UpTime() == 10){    tkr->Tasker_Interface(TASK_UPTIME_10_SECONDS); }
      if(tkr_time->UpTime() == 30){    tkr->Tasker_Interface(TASK_UPTIME_30_SECONDS); }
      if(tkr_time->UpTime() == 60){    tkr->Tasker_Interface(TASK_UPTIME_1_MINUTES);  }
      if(tkr_time->UpTime() == 600){   tkr->Tasker_Interface(TASK_UPTIME_10_MINUTES); }
      if(tkr_time->UpTime() == 36000){ tkr->Tasker_Interface(TASK_UPTIME_60_MINUTES); }
      
      if(tkr_time->UpTime()==120){     tkr->Tasker_Interface(TASK_ON_BOOT_SUCCESSFUL);}
      
    }

    // Check for midnight
    if (tkr_time->RtcTime.valid)
    {
      if((tkr_time->RtcTime.hour==0)&&(tkr_time->RtcTime.minute==0)&&(tkr_time->RtcTime.second==0)&&(tkr_time->lastday_run != tkr_time->RtcTime.day_of_year)){
        tkr_time->lastday_run = tkr_time->RtcTime.day_of_year;
        tkr->Tasker_Interface(TASK_EVERY_MIDNIGHT); 
      }
    }

  } // END secondloop

    
  #ifdef ENABLE_DEVFEATURE_TASKER__TASK_FUNCTION_QUEUE
  if(tkr->function_event_queue.size())
  {
    bool execute_function = false;
    uint8_t iter_count = 0;
    for(auto& queue:tkr->function_event_queue)
    {
      if(queue.delay_millis == 0){ execute_function = true; } // no delay
      if(mTime::TimeReached(&queue.tSaved_millis,queue.delay_millis)){ execute_function = true; }

      if(execute_function)
      {
        ALOG_HGL(PSTR("Executing Event Queue Item [%d]: func_id %d"), iter_count, queue.function_id);
        tkr->Tasker_Interface(queue.function_id);
    // std::vector<mTaskerManager::FUNCTION_EXECUTION_EVENT>::iterator index = tkr->function_event_queue.begin()+iter_count;

    
        ALOG_INF(PSTR("erase %d/%d"), iter_count, tkr->function_event_queue.size());

        tkr->function_event_queue.erase(tkr->function_event_queue.begin()+iter_count);  
      }
      iter_count++;
    }
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
  //   if (tkr_set->Settings.sysopt_network.bit.flag.sleep_normal) {
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
  #else
  ESP.wdtFeed(); 
  #endif
  
  LoopTasker();
  
  tkr_sup->loop_runtime_millis = millis() - tkr_sup->loop_start_millis;
// ALOG_INF(PSTR("LOOPSEC = \t\t\t\tLPS=%d, LoopTime=%d"), tkr_sup->activity.cycles_per_sec, tkr_sup->loop_runtime_millis);
    
  if(mTime::TimeReached(&tkr_set->runtime.tSavedUpdateLoopStatistics, 1000)){
    tkr_sup->activity.cycles_per_sec = tkr_sup->activity.loop_counter; 
    #ifdef ENABLE_DEBUGFEATURE__SPLASH__LOOPS_PER_SECOND
    ALOG_INF(PSTR("LOOPSEC = \t\t\t\tLPS=%d, LoopTime=%d"), tkr_sup->activity.cycles_per_sec, tkr_sup->loop_runtime_millis);
    #endif
    tkr_sup->activity.loop_counter=0;
  }

  if(tkr_sup->loop_runtime_millis > 500)
  {
    ALOG_ERR(PSTR("LONG_LOOP ============= %d %d %d"), tkr_sup->activity.loop_counter, tkr_sup->activity.cycles_per_sec, tkr_sup->loop_runtime_millis);
  }

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
