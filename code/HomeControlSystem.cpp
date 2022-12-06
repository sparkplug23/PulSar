/**
 * @file    HomeControlSystem.cpp
 * @author  Michael Doone (michaeldoonehub@gmail.com)
 * @brief   Primary code setup() and loop()
 * @version 1.0
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


#include "1_TaskerManager/mTaskerManager.h"


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

#ifdef USE_EMERGENCY_RESET
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
      SettingsErase(3);       // Reset all settings including QuickPowerCycle flag

      do {                    // Wait for user to remove Rx Tx jumper and power cycle
        Serial.write(0xA5);
        delay(1000);          // Satisfy SDK
      } while (Serial.read() == 0xA5);  // Poll for removal of jumper

      ESP_Restart();          // Restart to init default settings
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
#endif  // USE_EMERGENCY_RESET

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

  #ifdef CONFIG_IDF_TARGET_ESP32
    // restore GPIO16/17 if no PSRAM is found
    // if (!FoundPSRAM()) {
    //   // test if the CPU is not pico
    //   uint32_t chip_ver = REG_GET_FIELD(EFUSE_BLK0_RDATA3_REG, EFUSE_RD_CHIP_VER_PKG);
    //   uint32_t pkg_version = chip_ver & 0x7;
    //   if (pkg_version <= 3) {   // D0WD, S0WD, D2WD
    //     gpio_reset_pin(GPIO_NUM_16);
    //     gpio_reset_pin(GPIO_NUM_17);
    //   }
    // }
  #endif  // CONFIG_IDF_TARGET_ESP32
  #endif  // ESP32

  /**
   * @brief WatchDog timer
   * Code priority: Highest (Before Fastboot, Primary Recovery)
   **/
  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Init();
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
    RtcFastboot.reboot_count_since_poweron++;
    #ifdef DEBUG_FASTBOOT
    Serial.printf("FastBoot: Count %d\n\r", RtcFastboot.fast_reboot_count); 
    #endif
  }
  
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


#endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION

#ifdef ENABLE_DEVFEATURE___CAUTION_CAUTION__FORCE_CRASH_FASTBOOT_TESTING
Serial.flush();
delay(1000);
pCONT_sup->CmndCrash();
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
  #endif // DISABLE_SERIAL0_CORE
  #ifdef USE_SERIAL_ALTERNATE_TX
    Serial.set_tx(2);
  #endif // USE_SERIAL_ALTERNATE_TX
  #ifndef DISABLE_SERIAL
  Serial.println(F("\n\rRebooting..." DEBUG_INSERT_PAGE_BREAK));
  #endif // DISABLE_SERIAL
  #ifndef DISABLE_SERIAL_LOGGING
  #ifdef ENABLE_BUG_TRACING
  Serial.println(F("DELAYED BOOT for 5 seconds...")); Serial.flush(); delay(5000);
  #endif
  #endif

  Serial.printf("baudrate_tmp = %d\n\r", baudrate_tmp);

  #ifdef ENABLE_DEVFEATURE_SETDEBUGOUTPUT
  Serial.setDebugOutput(true);
  #endif 
  
/********************************************************************************************
 ** Init Pointers ***************************************************************************
 ********************************************************************************************/
 
  // Init Json builder with memory address and size
  JsonBuilderI ->Start(data_buffer.payload.ctr, &data_buffer.payload.len, DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  BufferWriterI->Start(data_buffer.payload.ctr, &data_buffer.payload.len, DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  
  /**
   * @brief Start the Tasker_Interface module
   **/
  pCONT->Instance_Init();
  
/********************************************************************************************
 ** Init Pointers ***************************************************************************
 ********************************************************************************************/
 
  ALOG_INF(PSTR("AddLog Started"));

/********************************************************************************************
 ** Splash boot reason ***************************************************************************
 ********************************************************************************************/

  ALOG_HGL(PSTR("ResetReason=%d"), ResetReason_g());

/********************************************************************************************
 ** Set boottime values *********************************************************************
 ********************************************************************************************/

  // Set boot method
  pCONT_set->seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
  
  RESET_BOOT_STATUS();

/********************************************************************************************
 ** Show PSRAM Present **********************************************************************
 ********************************************************************************************/

//  AddLog(LOG_LEVEL_INFO, PSTR("ADR: Settings %p, Log %p"), Settings, TasmotaGlobal.log_buffer);
// #ifdef ESP32
//   AddLog(LOG_LEVEL_INFO, PSTR("HDW: %s %s"), GetDeviceHardware().c_str(),
//             FoundPSRAM() ? (CanUsePSRAM() ? "(PSRAM)" : "(PSRAM disabled)") : "" );
//   AddLog(LOG_LEVEL_DEBUG, PSTR("HDW: FoundPSRAM=%i CanUsePSRAM=%i"), FoundPSRAM(), CanUsePSRAM());
//   #if !defined(HAS_PSRAM_FIX)
//   if (FoundPSRAM() && !CanUsePSRAM()) {
//     AddLog(LOG_LEVEL_INFO, PSTR("HDW: PSRAM is disabled, requires specific compilation on this hardware (see doc)"));
//   }
//   #endif
// #else // ESP32
//   AddLog(LOG_LEVEL_INFO, PSTR("HDW: %s"), GetDeviceHardware().c_str());
// #endif // ESP32

/********************************************************************************************
 ** Internal RTC Time PreInit ***************************************************************
 ********************************************************************************************/

  pCONT_time->RtcPreInit();

/********************************************************************************************
 ** File System *****************************************************************************
 ********************************************************************************************/

// #ifdef USE_UFILESYS
//   UfsInit();  // xdrv_50_filesystem.ino
// #endif
  // #ifdef USE_MODULE_DRIVERS_FILESYSTEM
  // pCONT_mfile->UfsInit();  // xdrv_50_filesystem.ino
  // #endif


//  AddLog(LOG_LEVEL_INFO, PSTR("ADR: Settings %p, Log %p"), Settings, TasmotaGlobal.log_buffer);
// #ifdef ESP32
//   AddLog(LOG_LEVEL_INFO, PSTR("HDW: %s %s"), GetDeviceHardware().c_str(),
//             FoundPSRAM() ? (CanUsePSRAM() ? "(PSRAM)" : "(PSRAM disabled)") : "" );
//   AddLog(LOG_LEVEL_DEBUG, PSTR("HDW: FoundPSRAM=%i CanUsePSRAM=%i"), FoundPSRAM(), CanUsePSRAM());
//   #if !defined(HAS_PSRAM_FIX)
//   if (FoundPSRAM() && !CanUsePSRAM()) {
//     AddLog(LOG_LEVEL_INFO, PSTR("HDW: PSRAM is disabled, requires specific compilation on this hardware (see doc)"));
//   }
//   #endif
// #else // ESP32
//   AddLog(LOG_LEVEL_INFO, PSTR("HDW: %s"), GetDeviceHardware().c_str()); // This function GetDeviceHardware needs added and supporting functions
// #endif // ESP32

#ifdef USE_UFILESYS
  UfsInit();  // xdrv_50_filesystem.ino
#endif

/********************************************************************************************
 ** Settings ********************************************************************************
 ********************************************************************************************/

  pCONT_set->SettingsInit();

  #ifdef USE_EMERGENCY_RESET
    EmergencySerial_SettingsReset();
  #endif  // USE_EMERGENCY_RESET

  pCONT_sup->init_FirmwareVersion();

  /**
   * @brief Before getting settings to work, I need to first get UFS working and understand it, as its the future of settings saving. 
   * Both methods need to be working.
   * 
   * File system saving will become useful when I create an esp32 gps data logger as part of future LTE monitor.
   */

  // pCONT_set->TestSettingsLoad();
  // pCONT_set->TestSettings_ShowLocal_Header();

  ALOG_INF(PSTR("Loading minimal defaults"));

  pCONT_set->SettingsDefault(); //preload minimal required

  ALOG_INF(PSTR("Loading settings from saved memory"));
  
  // Overwrite with latest values, including template if new SETTINGS_CONFIG exists
  pCONT_set->SettingsLoad();    //overwrite stored settings from defaults
  // Check Load was successful
  pCONT_set->SettingsLoad_CheckSuccessful();
  // #endif

  // AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("setup, before4"));
  // pCONT_set->TestSettingsLoad();
  // pCONT_set->TestSettings_ShowLocal_Header();
  // AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("setup, after4"));

  // Test save and read back
  // pCONT_set->Settings.bootcount = 13;
  // pCONT_set->SettingsSave(1);

  // pCONT_set->TestSettingsLoad();

//   if (ResetReason() != REASON_DEEP_SLEEP_AWAKE) {
// #ifdef ESP8266
//     Settings->flag4.network_wifi = 1;           // Make sure we're in control
// #endif
// #ifdef ESP32
//     if (!Settings->flag4.network_ethernet) {
//       Settings->flag4.network_wifi = 1;         // Make sure we're in control
//     }
// #endif
//   }



/********************************************************************************************
 ** OsWatch ********************************************************************************
 ********************************************************************************************/

  // OsWatchInit();

/********************************************************************************************
 ** Fastboot ********************************************************************************
 ********************************************************************************************/

  /********************************************************************************************
   ** Fastboot: >> Configuration Recovery <<  
   *     - Loaded settings to determine how fastboot is configured
   *     - This code must run before drivers/sensors are initiated, so they may be disabled if recovery is required
  ********************************************************************************************/
  #ifdef ENABLE_DEVFEATURE_FASTBOOT_DETECTION
  
    AddLog(LOG_LEVEL_HIGHLIGHT, PSTR("ARESET TWICE! \t\t\t%d"), RtcFastboot.fast_reboot_count);

    if (pCONT_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET]) // SetOption36
    {         
      // Disable functionality as possible cause of fast restart within BOOT_LOOP_TIME seconds (Exception, WDT or restarts)
      if (RtcFastboot.fast_reboot_count > pCONT_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET]) {       // Restart twice
        
        // Settings->flag3.user_esp8285_enable = 0;       // SetOption51 - Enable ESP8285 user GPIO's - Disable ESP8285 Generic GPIOs interfering with flash SPI
        if (RtcFastboot.fast_reboot_count > pCONT_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +1) {  // Restart 3 times
          // for (
            uint32_t i = 0; //i < MAX_RULE_SETS; i++) {
          //   if (bitRead(Settings->rule_stop, i)) {
          //     bitWrite(Settings->rule_enabled, i, 0);  // Disable rules causing boot loop
          ALOG_INF( PSTR("Fastboot: Disable Rule %d"), i );
          //   }
          // }
        }
        if (RtcFastboot.fast_reboot_count > pCONT_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +2) {  // Restarted 4 times
          // Settings->rule_enabled = 0;                  // Disable all rules
          // TasmotaGlobal.no_autoexec = true;
          ALOG_INF( PSTR("Fastboot: Disable All Rules") );
        }
        if (RtcFastboot.fast_reboot_count > pCONT_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +3) {  // Restarted 5 times
          // for (uint32_t i = 0; i < nitems(Settings->my_gp.io); i++) {
          //   Settings->my_gp.io[i] = GPIO_NONE;         // Reset user defined GPIO disabling sensors
          // }
          ALOG_INF( PSTR("Fastboot: Disable GPIO Functions") );
        }
        if (RtcFastboot.fast_reboot_count > pCONT_set->Settings.setoption_255[P_BOOT_LOOP_OFFSET] +4) {  // Restarted 6 times
          // Settings->module = Settings->fallback_module;  // Reset module to fallback module
          // Settings->last_module = Settings->fallback_module;
          ALOG_INF( PSTR("Fastboot: Reset Module") );
        }
        AddLog(LOG_LEVEL_INFO, PSTR("FRC: " D_LOG_SOME_SETTINGS_RESET " (%d)"), RtcFastboot.fast_reboot_count);
      }
    }

#endif // ENABLE_DEVFEATURE_FASTBOOT_DETECTION

/********************************************************************************************
 ** Reconfigure baud rate if SettingsLoad changed it ****************************************
 ********************************************************************************************/

  // TasmotaGlobal.seriallog_level = Settings->seriallog_level;
  // TasmotaGlobal.syslog_level = Settings->syslog_level;

  // TasmotaGlobal.module_changed = (Settings->module != Settings->last_module);
  // if (TasmotaGlobal.module_changed) {
  //   Settings->baudrate = APP_BAUDRATE / 300;
  //   Settings->serial_config = TS_SERIAL_8N1;
  // }
  // SetSerialBaudrate(Settings->baudrate * 300);  // Reset serial interface if current baudrate is different from requested baudrate

/********************************************************************************************
 ** Quick Power Cycle ***********************************************************************
 ********************************************************************************************/

  // if (1 == RtcReboot.fast_reboot_count) {      // Allow setting override only when all is well
  //   UpdateQuickPowerCycle(true);
  // }

/********************************************************************************************
 ** Load Templates **************************************************************************
 ********************************************************************************************/
   
  pCONT->Tasker_Interface(FUNC_POINTER_INIT); // confirgure any memory address needed as part of module init or templates
  
  // #ifdef FORCE_TEMPLATE_LOADING
  // This will overwrite the settings, temporary, will use a second flag to force template loads "TEMPLATE_HOLDER"
  // need to if template not provided, load defaults else use settings -- add protection in settings defaults to use templates instead (progmem or user desired)
  // Load template before init
  
  ALOG_WRN(PSTR(D_LOG_MEMORY D_LOAD " Temporary loading any progmem templates"));
  
  pCONT->Tasker_Interface(FUNC_TEMPLATE_MODULE_LOAD_FROM_PROGMEM); // loading module, only interface modules will have these
  // load
  // pCONT->Tasker_Interface(FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM);  //load/overwrite names AFTER init (FUNC_TEMPLATE_DEVICE_CONFIG_BEFORE_INIT)
  // #else
  // #warning "FORCE_TEMPLATE_LOADING is disabled, trying to use settings may result in improper loaded values"
  // #endif
  
  // // Set boot method
  // pCONT_set->seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  // pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;  
  
/********************************************************************************************
 ** Initialise System and Modules ***********************************************************
 ********************************************************************************************/

  // Init the GPIOs
  pCONT_pins->GpioInit();
  // Start pins in modules
  pCONT->Tasker_Interface(FUNC_PRE_INIT);
  // Init devices
  pCONT->Tasker_Interface(FUNC_INIT);
  // Init devices after others have been configured fully
  pCONT->Tasker_Interface(FUNC_POST_INIT);
  // Run system functions 
  pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_INIT);
  // Preload any templates defined in mFirmwareCustom.h
  // Load the minimal default settings in modules (hard coded) before loading any stored user values
  pCONT->Tasker_Interface(FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES); // load the minimal
  // Load any stored user values into module
  pCONT->Tasker_Interface(FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE);
  // load
  /**
   * This can only happen AFTER each module is running/enabled (port init checks). This will override the settings load, so should be tested if needed when settings work
   * */
  pCONT->Tasker_Interface(FUNC_TEMPLATE_DEVICE_LOAD_FROM_PROGMEM);//load/overwrite names AFTER init (FUNC_TEMPLATE_DEVICE_CONFIG_AFTER_INIT)
  // Configure sensor/drivers to values desired for modules
  pCONT->Tasker_Interface(FUNC_CONFIGURE_MODULES_FOR_DEVICE);
  // init mqtt handlers from memory
  pCONT->Tasker_Interface(FUNC_MQTT_HANDLERS_INIT);  
  // Init the refresh periods for mqtt
  pCONT->Tasker_Interface(FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD);
  #ifdef ENABLE_FUNCTION_DEBUG
    pCONT->Tasker_Interface(FUNC_DEBUG_CONFIGURE);
  #endif
  // Init any dynamic memory buffers
  pCONT->Tasker_Interface(FUNC_REFRESH_DYNAMIC_MEMORY_BUFFERS_ID);
  // For debugging, allow method to override init/loaded values
  #ifdef ENABLE_BOOT_OVERRIDE_INIT
  pCONT->Tasker_Interface(FUNC_OVERRIDE_BOOT_INIT);
  #endif

  #ifdef USE_MODULE_CORE_RULES
  pCONT->Tasker_Interface(FUNC_RULES_ADD_DEFAULT_RULES_USING_GPIO_FUNCTIONS_ID);
  #endif 
  
/********************************************************************************************
 ** Boot Completed **************************************************************************
 ********************************************************************************************/

  pCONT->Tasker_Interface(FUNC_ON_BOOT_COMPLETE);


}

void LoopTasker()
{
  
  #ifdef USE_ARDUINO_OTA
    pCONT_sup->ArduinoOtaLoop();
  #endif
   
  pCONT->Tasker_Interface(FUNC_LOOP); DEBUG_LINE;
 
  if(pCONT_time->uptime.seconds_nonreset > 30){ pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_LOOP); } // Only run after stable boot
 
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop50mSec ,50  )){ pCONT->Tasker_Interface(FUNC_EVERY_50_MSECOND);  }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop100mSec,100 )){ pCONT->Tasker_Interface(FUNC_EVERY_100_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop250mSec,250 )){ pCONT->Tasker_Interface(FUNC_EVERY_250_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop1Sec   ,1000)){ pCONT->Tasker_Interface(FUNC_EVERY_SECOND);      }  DEBUG_LINE;

}

/********************************************************************************************/
/*********************loop*******************************************************************/
/********************************************************************************************/


void SmartLoopDelay()
{
  #ifndef DISABLE_SLEEP
  if(pCONT_set->Settings.enable_sleep){
    if (pCONT_set->Settings.flag_network.sleep_normal) {
      pCONT_sup->SleepDelay(pCONT_set->runtime_var.sleep);
    } else {
      // Loop time < sleep length of time
      if (pCONT_sup->loop_runtime_millis < (uint32_t)pCONT_set->runtime_var.sleep) {
        //delay by loop time
        pCONT_sup->SleepDelay((uint32_t)pCONT_set->runtime_var.sleep - pCONT_sup->loop_runtime_millis);  // Provide time for background tasks like wifi
      } else {

        // if loop takes longer than sleep period, no delay, IF wifi is down, devote half loop time to wifi connect 
        // If wifi down and loop_runtime_millis > setoption36 then force loop delay to 1/3 of loop_runtime_millis period
        if (pCONT_set->global_state.wifi_down) {
          pCONT_sup->SleepDelay(pCONT_sup->loop_runtime_millis /2);
        }

      }
    }
  }
  #endif
}


void loop(void)
{
  pCONT_sup->activity.loop_counter++;
  pCONT_sup->loop_start_millis = millis();
  
  #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  WDT_Reset();
  #endif

  LoopTasker();
      
  pCONT_sup->loop_runtime_millis = millis() - pCONT_sup->loop_start_millis;

  if(mTime::TimeReached(&pCONT_set->runtime_var.tSavedUpdateLoopStatistics, 1000)){
    pCONT_sup->activity.cycles_per_sec = pCONT_sup->activity.loop_counter; 
    ALOG_DBM(PSTR("LOOPSEC = %d %d"), pCONT_sup->activity.loop_counter, pCONT_sup->activity.cycles_per_sec);
    pCONT_sup->activity.loop_counter=0;
  }

  /**
   * @brief Until code has been stress tested, removing all delays to stop starving resources. This will need to be introduced later one device at a time.
   * 
   */
  // #ifndef USE_MODULE_LIGHTS_INTERFACE // Temporarily remove delay, long term enable pausing delays while animations are running
  // SmartLoopDelay();
  // #endif // USE_MODULE_LIGHTS_INTERFACE

  DEBUG_LINE;
  if (!pCONT_sup->loop_runtime_millis) { pCONT_sup->loop_runtime_millis++; } // We cannot divide by 0
  pCONT_sup->loop_delay_temp = pCONT_set->runtime_var.sleep; 
  if (!pCONT_sup->loop_delay_temp) { pCONT_sup->loop_delay_temp++; }              // We cannot divide by 0
  pCONT_sup->loops_per_second = 1000 / pCONT_sup->loop_delay_temp;  // We need to keep track of this many loops per second, 20ms delay gives 1000/20 = 50 loops per second (50hz)
  pCONT_sup->this_cycle_ratio = 100 * pCONT_sup->loop_runtime_millis / pCONT_sup->loop_delay_temp;
  pCONT_set->loop_load_avg = pCONT_set->loop_load_avg - (pCONT_set->loop_load_avg / pCONT_sup->loops_per_second) + (pCONT_sup->this_cycle_ratio / pCONT_sup->loops_per_second); // Take away one loop average away and add the new one

}
