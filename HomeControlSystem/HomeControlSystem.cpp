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
    DisableBrownout();
  #endif
  #ifdef CONFIG_IDF_TARGET_ESP32
    // // restore GPIO16/17 if no PSRAM is found
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

/********************************************************************************************
 ** Serial **********************************************************************************
 ********************************************************************************************/
 
  #ifndef DISABLE_SERIAL0_CORE
  Serial.begin(SERIAL_DEBUG_BAUD_DEFAULT);
  #endif // DISABLE_SERIAL0_CORE
  #ifdef USE_SERIAL_ALTERNATE_TX
    Serial.set_tx(2);
  #endif // USE_SERIAL_ALTERNATE_TX
  Serial.println(F("\n\rRebooting..." DEBUG_INSERT_PAGE_BREAK));
  #ifndef DISABLE_SERIAL_LOGGING
  #ifdef ENABLE_BUG_TRACING
  Serial.println(F("DELAYED BOOT for 5 seconds...")); Serial.flush(); delay(5000);
  #endif
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
 ** Set boottime values *********************************************************************
 ********************************************************************************************/

  // Set boot method
  pCONT_set->seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
  
  RESET_BOOT_STATUS();

/********************************************************************************************
 ** RTC and Fastboot ************************************************************************
 ********************************************************************************************/

  #ifdef ENABLE_DEVFEATURE_RTC_FASTBOOT_V2
  // fastboot is currently contained within settings class, but this requires a stable class config.
  // I should consider not using cpp classes for the RTC, perhaps just header code, so truly basic and simple/safe code will always work
  // For now, get working as tas did (for v2) and use the header only method as version 3
  pCONT_set->RtcPreInit();

  pCONT_set->RtcRebootLoad();
  if (!pCONT_set->RtcRebootValid()) {
    pCONT_set->RtcReboot.fast_reboot_count = 0;
  }
#ifdef FIRMWARE_MINIMAL
  pCONT_set->RtcReboot.fast_reboot_count = 0;    // Disable fast reboot and quick power cycle detection
#else
  if (ResetReason() == REASON_DEEP_SLEEP_AWAKE) {
    pCONT_set->RtcReboot.fast_reboot_count = 0;  // Disable fast reboot and quick power cycle detection
  } else {
    pCONT_set->RtcReboot.fast_reboot_count++;
  }
#endif
  RtcRebootSave();

  if (RtcSettingsLoad(0)) {
    uint32_t baudrate = (RtcSettings.baudrate / 300) * 300;  // Make it a valid baudrate
    if (baudrate) { TasmotaGlobal.baudrate = baudrate; }
  }
  Serial.begin(TasmotaGlobal.baudrate);
  Serial.println();
#endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_V2


                      /**
                       * @brief TO BE DELETED
                       * 
                       */
                        #ifndef ENABLE_DEVFEATURE_RTC_FASTBOOT_V2
                        pCONT_set->RtcRebootLoad();
                        if(!pCONT_set->RtcRebootValid()) { 
                          pCONT_set->RtcReboot.fast_reboot_count = 0; 
                        }
                        pCONT_set->RtcReboot.fast_reboot_count++;
                        pCONT_set->RtcRebootSave();
                        #endif // ENABLE_DEVFEATURE_RTC_FASTBOOT_V2

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
 ** File System *****************************************************************************
 ********************************************************************************************/

// #ifdef USE_UFILESYS
//   UfsInit();  // xdrv_50_filesystem.ino
// #endif
  // #ifdef USE_MODULE_DRIVERS_FILESYSTEM
  // pCONT_mfile->UfsInit();  // xdrv_50_filesystem.ino
  // #endif

/********************************************************************************************
 ** Settings ********************************************************************************
 ********************************************************************************************/

  pCONT_sup->init_FirmwareVersion();

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

/********************************************************************************************
 ** Load Templates **************************************************************************
 ********************************************************************************************/

  // if (1 == RtcReboot.fast_reboot_count) {      // Allow setting override only when all is well
  //   UpdateQuickPowerCycle(true);
  // }
   
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
  #endif  // USE_ARDUINO_OTA
   
  pCONT->Tasker_Interface(FUNC_LOOP); DEBUG_LINE;
  
  if(pCONT_time->uptime.seconds_nonreset > 30){ pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_LOOP); } // Only run after stable boot
 
  //move into support, or into time, to align with every_minute, hour, etc
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
        if (pCONT_set->global_state.wifi_down) {
          pCONT_sup->SleepDelay(pCONT_sup->loop_runtime_millis /2); // If wifi down and loop_runtime_millis > setoption36 then force loop delay to 1/3 of loop_runtime_millis period
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
  WDT_RESET();
  
  LoopTasker();
      
  pCONT_sup->loop_runtime_millis = millis() - pCONT_sup->loop_start_millis;

  if(mTime::TimeReached(&pCONT_set->runtime_var.tSavedUpdateLoopStatistics, 1000)){
    pCONT_sup->activity.cycles_per_sec = pCONT_sup->activity.loop_counter; 
    ALOG_DBM(PSTR("LOOPSEC = %d %d"), pCONT_sup->activity.loop_counter, pCONT_sup->activity.cycles_per_sec);
    pCONT_sup->activity.loop_counter=0;
  }

  SmartLoopDelay();

  DEBUG_LINE;
  if (!pCONT_sup->loop_runtime_millis) { pCONT_sup->loop_runtime_millis++; }            // We cannot divide by 0
  pCONT_sup->loop_delay_temp = pCONT_set->runtime_var.sleep; 
  if (!pCONT_sup->loop_delay_temp) { pCONT_sup->loop_delay_temp++; }              // We cannot divide by 0
  pCONT_sup->loops_per_second = 1000 / pCONT_sup->loop_delay_temp;  // We need to keep track of this many loops per second, 20ms delay gives 1000/20 = 50 loops per second (50hz)
  pCONT_sup->this_cycle_ratio = 100 * pCONT_sup->loop_runtime_millis / pCONT_sup->loop_delay_temp;
  pCONT_set->loop_load_avg = pCONT_set->loop_load_avg - (pCONT_set->loop_load_avg / pCONT_sup->loops_per_second) + (pCONT_sup->this_cycle_ratio / pCONT_sup->loops_per_second); // Take away one loop average away and add the new one

}
