#include "mTelemetry.h"


                            uint8_t mTelemetry::ConstructJSON_LWT_Online(uint8_t json_level, bool json_appending)
                            { 

                              JBI->Start();
                                JBI->Add("LWT", "Online");
                                JBI->Add("ResetReason", "TBA");
                                #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
                                if(RtcMemory__RuntimeState.boot_was_completed_ota_event == 1)
                                {
                                  JBI->Add("OTABootReason", "OTAYes");
                                }else{
                                  JBI->Add("OTABootReason", "OTANo");
                                }
                                
                                #else
                                JBI->Add("OTABootReason", "Unknown");
                                #endif
                              return JBI->End();
                                
                            }


uint8_t mTelemetry::ConstructJSON_Health(uint8_t json_level, bool json_appending)
{
  char buffer[30];

  JBI->Start();

    /********************************************************************
    * Core runtime health
    ********************************************************************/
    JBI->Add(PM_TIME,                 tkr_time->GetTime().c_str());
    JBI->Add(PM_UPTIME,               tkr_time->GetUptime(buffer, sizeof(buffer)));
    
    IPAddress localip = WiFi.localIP(); JBI->Add_FV(PM_IPADDRESS, PSTR("\"%d.%d.%d.%d\""), localip[0], localip[1], localip[2], localip[3]); // Placed as 3rd in JSON list to view in short payloads views

    JBI->Add(PM_UPSECONDS,            tkr_time->UpTime());

    JBI->Add(PM_SLEEPMODE,            tkr_set->runtime.sleep ? "Dynamic" : "Unknown");
    JBI->Add(PM_SLEEP,                tkr_set->runtime.sleep);                  // Typical: 20
    JBI->Add(PM_LOOPSSEC,             tkr_sup->activity.cycles_per_sec);         // Typical: 50 Hz
    JBI->Add(PM_LOADAVERAGE,          tkr_set->runtime.loop_load_avg);           // Average loop load

    /********************************************************************
    * Memory health
    ********************************************************************/
    JBI->Add(PM_FREEHEAP,             ESP.getFreeHeap());

    #ifdef ESP8266
    JBI->Add(PM_HEAPFRAGMENTATION,    ESP.getHeapFragmentation());
    JBI->Add(PM_MAXFREEBLOCKSIZE,     ESP.getMaxFreeBlockSize());
    #endif

    #ifdef ESP32
    JBI->Add(PM_FREEHEAP,             ESP.getMinFreeHeap());
    JBI->Add(PM_PSRAM_FREE,           ESP.getFreePsram());
    JBI->Add(PM_PSRAM_SIZE,           ESP.getPsramSize());
    #endif

    /********************************************************************
    * Device identity
    ********************************************************************/
    JBI->Add(PM_DEVICEFRIENDLYNAME,   tkr_set->Settings.system_name.friendly);

    /********************************************************************
    * WiFi health
    ********************************************************************/

    JBI->Add(PM_SSID,                 WiFi.SSID().c_str());
    JBI->Add(PM_RSSI,                 WiFi.RSSI());
    JBI->Add(PM_WIFI_CHANNEL,         WiFi.channel());

    #ifdef ESP32
    JBI->Add(PM_WIFI_SLEEP,           (int8_t)WiFi.getSleep());
    #endif

    /********************************************************************
    * MQTT health
    *
    * Keep this as state/counter information only. Actual MQTT telemetry
    * payloads and topic-level send state should remain in the MQTT/status
    * payloads.
    ********************************************************************/
    #ifdef USE_MODULE_NETWORK_MQTT
    {
      uint8_t mqtt_brokers = 0;
      uint8_t mqtt_allowed = 0;
      uint8_t mqtt_connected = 0;
      uint16_t mqtt_connect_count = 0;
      uint16_t mqtt_downtime_counter = 0;

      for (auto& con : tkr_mqtt->brokers)
      {
        if (!con) { continue; }

        mqtt_brokers++;

        if (con->allowed) {
          mqtt_allowed++;
        }

        if (con->MqttIsConnected()) {
          mqtt_connected++;
        }

        mqtt_connect_count += con->connect_count;
        mqtt_downtime_counter += con->downtime_counter;
      }

      JBI->Add(PM_MQTT_BROKERS,           mqtt_brokers);
      JBI->Add(PM_MQTT_ALLOWED,           mqtt_allowed);
      JBI->Add(PM_MQTT_CONNECTED,         mqtt_connected);
      JBI->Add(PM_MQTT_CONNECTCOUNT,      mqtt_connect_count);
      JBI->Add(PM_MQTT_DOWNTIME_COUNTER,  mqtt_downtime_counter);
    }
    #endif

  return JBI->End();
}



#ifndef FIRMWARE_MINIMAL2

uint8_t mTelemetry::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Compact persisted settings summary.
     *
     * This topic reports the core settings identity and general persistent
     * configuration that does not have a better owner.
     *
     * Excluded by design:
     *  - sysopt_* groups: settings/system, settings/network, etc.
     *  - logging: log
     *  - time/timezone/DST/build age: time
     *  - device_name_buffer/text_pool internals: settings/text_buffer
     *  - live WiFi/MQTT/network state: network, mqtt, health
     *  - legacy ipv4_* fields: superseded by network module config
     *  - full template hardware/pin arrays: future settings/template or pins topic
     ********************************************************************/

    JBI->Object_Start("Hdr");
      JBI->Add("Holder",     tkr_set->Settings.cfg_holder);
      JBI->Add("Size",       tkr_set->Settings.cfg_size);
      JBI->Add("Version",    tkr_set->Settings.version);
      JBI->Add("SaveCount",  tkr_set->Settings.save_flag);
      JBI->Add("BootCount",  tkr_set->Settings.bootcount);
      JBI->Add("BootErr",    tkr_set->Settings.bootcount_errors_only);
      JBI->Add("Timestamp",  tkr_set->Settings.cfg_timestamp);
      JBI->Add_FV("CRC32",   PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.cfg_crc32);
      JBI->Add_FV("Address", PSTR("\"%X\""), tkr_set->GetSettingsAddress());
      JBI->Add("FileUTC",    tkr_set->Settings.settings_file_update_utc_ascii);
      JBI->Add("HolderCtr",  tkr_set->Settings.settings_holder_ctr);
    JBI->Object_End();

    JBI->Object_Start("Name");
      JBI->Add("Device",     tkr_set->Settings.system_name.device);
      JBI->Add("Friendly",   tkr_set->Settings.system_name.friendly);
      JBI->Add("Room",       tkr_set->Settings.room_hint);
      JBI->Add_P("ModuleName", tkr_pins->ModuleName());
    JBI->Object_End();

    JBI->Object_Start("Module");
      JBI->Add("Module",     tkr_set->Settings.module);
      JBI->Add("Last",       tkr_set->Settings.last_module);
      JBI->Add("TplBase",    tkr_set->Settings.user_template.base);
      JBI->Add("TplFlags",   tkr_set->Settings.user_template.flags);
    JBI->Object_End();

    JBI->Object_Start("Core");
      JBI->Add("SaveData",   tkr_set->Settings.save_data);
      JBI->Add("RuleStop",   tkr_set->Settings.rule_stop);
      JBI->Add("EnSleep",    tkr_set->Settings.enable_sleep);
      JBI->Add("Sleep",      tkr_set->Settings.sleep);
      JBI->Add("Baud",       tkr_set->Settings.baudrate);
      JBI->Add("SBaud",      tkr_set->Settings.sbaudrate);
      JBI->Add("SerDelim",   tkr_set->Settings.serial_delimiter);
      JBI->Add("InvalidAge", tkr_set->Settings.unified_interface_reporting_invalid_reading_timeout_seconds);
    JBI->Object_End();

    JBI->Object_Start("Web");
      JBI->Add("Server",     tkr_set->Settings.webserver);
      JBI->Add("Refresh",    tkr_set->Settings.web_refresh);
    JBI->Object_End();

    JBI->Object_Start("Pulse");
      JBI->Add("Type",       tkr_set->Settings.pulse_counter_type);
      JBI->Add("Debounce",   tkr_set->Settings.pulse_counter_debounce);
    JBI->Object_End();

    JBI->Object_Start("Input");
      JBI->Add("BtnDeb",     tkr_set->Settings.button_debounce);
      JBI->Add("SwDeb",      tkr_set->Settings.switch_debounce);
    JBI->Object_End();

    JBI->Object_Start("Output");
      JBI->Add("Power",      (uint32_t)tkr_set->Settings.power);
      JBI->Add("PowerOn",    tkr_set->Settings.poweronstate);
      JBI->Add("LedMask",    tkr_set->Settings.ledmask);
      JBI->Add("LedState",   tkr_set->Settings.ledstate);
      JBI->Add("LedPwmMask", tkr_set->Settings.ledpwm_mask);
      JBI->Add("LedPwmOn",   tkr_set->Settings.ledpwm_on);
      JBI->Add("LedPwmOff",  tkr_set->Settings.ledpwm_off);
      JBI->Add("BlinkTime",  tkr_set->Settings.blinktime);
      JBI->Add("BlinkCount", tkr_set->Settings.blinkcount);
      JBI->Add("Monitors",   tkr_set->Settings.monitors);
      JBI->Add("PwmRange",   tkr_set->Settings.pwm_range);
      JBI->Add("PwmFreq",    tkr_set->Settings.pwm_frequency);
      JBI->Add("RfDupTime",  tkr_set->Settings.rf_duplicate_time);
      JBI->Add_FV("RfMask",  PSTR("\"%016llX\""), (unsigned long long)tkr_set->Settings.rf_protocol_mask);
      JBI->Add_FV("PowerLock", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.power_lock);
    JBI->Object_End();

    JBI->Object_Start("Bus");
      JBI->Add_FV("I2C0", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.i2c_drivers[0]);
      JBI->Add_FV("I2C1", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.i2c_drivers[1]);
      JBI->Add_FV("I2C2", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.i2c_drivers[2]);
    JBI->Object_End();

    JBI->Object_Start("Boot");
      JBI->Add("ResetTime", tkr_set->Settings.bootcount_reset_time);
    JBI->Object_End();

    #ifdef ENABLE_FEATURE_SETTINGS__ADD_LOCAL_TIME_AS_ASCII_FOR_SAVE_TIME_DEBUGGING
    JBI->Add("LocalTimeSave", tkr_set->Settings.local_time_ascii_debug);
    #endif

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_System(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add_FV("Bits", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.sysopt_system.bit.data);

    JBI->Object_Start("B");
      JBI->Add("SaveState",    tkr_set->Settings.sysopt_system.bit.save_state);
      JBI->Add("Mqtt",         tkr_set->Settings.sysopt_system.bit.mqtt_enabled);
      JBI->Add("MqttPowerRet", tkr_set->Settings.sysopt_system.bit.mqtt_power_retain);
      JBI->Add("MqttBtnRet",   tkr_set->Settings.sysopt_system.bit.mqtt_button_retain);
      JBI->Add("MqttSwRet",    tkr_set->Settings.sysopt_system.bit.mqtt_switch_retain);
      JBI->Add("BtnSingle",    tkr_set->Settings.sysopt_system.bit.button_single_press_only);
      JBI->Add("Interlock",    tkr_set->Settings.sysopt_system.bit.interlock);
      JBI->Add("DecText",      tkr_set->Settings.sysopt_system.bit.decimal_text);
      JBI->Add("Cors",         tkr_set->Settings.sysopt_system.bit.ex_cors_enabled);
      JBI->Add("Compat",       tkr_set->Settings.sysopt_system.bit.compatibility_check);
      JBI->Add("GuiNoState",   tkr_set->Settings.sysopt_system.bit.gui_no_state_text);
      JBI->Add("Timers",       tkr_set->Settings.sysopt_system.bit.timers_enable);
      JBI->Add("SleepNormal",  tkr_set->Settings.sysopt_system.bit.sleep_normal);
      JBI->Add("Esp8285",      tkr_set->Settings.sysopt_system.bit.user_esp8285_enable);
    JBI->Object_End();

    JBI->Object_Start("P");
      JBI->Add("BootLoopOff",  tkr_set->Settings.sysopt_system.param.boot_loop_offset);
      JBI->Add("BacklogMs",    tkr_set->Settings.sysopt_system.param.backlog_delay_ms);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_Network(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add_FV("Bits", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.sysopt_network.bit.data);

    JBI->Object_Start("B");
      JBI->Add("Mdns",         tkr_set->Settings.sysopt_network.bit.mdns_enabled);
      JBI->Add("WifiScan",     tkr_set->Settings.sysopt_network.bit.use_wifi_scan);
      JBI->Add("WifiRescan",   tkr_set->Settings.sysopt_network.bit.use_wifi_rescan);
      JBI->Add("SleepNormal",  tkr_set->Settings.sysopt_network.bit.sleep_normal);
      JBI->Add("NoFastCycle",  tkr_set->Settings.sysopt_network.bit.fast_power_cycle_disable);
      JBI->Add("Wifi",         tkr_set->Settings.sysopt_network.bit.network_wifi);
      JBI->Add("Eth",          tkr_set->Settings.sysopt_network.bit.network_ethernet);
      JBI->Add("DnsIPv6",      tkr_set->Settings.sysopt_network.bit.dns_ipv6_priority);
    JBI->Object_End();

    JBI->Object_Start("P");
      JBI->Add("MdnsDelay",    tkr_set->Settings.sysopt_network.param.mdns_delayed_start_s);
      JBI->Add("GratArp",      tkr_set->Settings.sysopt_network.param.gratuitous_arp_s);
      JBI->Add("WifiScanMin",  tkr_set->Settings.sysopt_network.param.wifi_rescan_interval_min);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_Drivers(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add_FV("Bits", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.sysopt_drivers.bit.data);

    JBI->Object_Start("B");
      JBI->Add("BuzzFreq",     tkr_set->Settings.sysopt_drivers.bit.buzzer_freq_mode);
      JBI->Add("Buzzer",       tkr_set->Settings.sysopt_drivers.bit.buzzer_enable);
      JBI->Add("Esp32Temp",    tkr_set->Settings.sysopt_drivers.bit.use_esp32_temperature);
      JBI->Add("WsReverse",    tkr_set->Settings.sysopt_drivers.bit.ws_clock_reverse);
      JBI->Add("RawRx",        tkr_set->Settings.sysopt_drivers.bit.receive_raw);
      JBI->Add("RfDec",        tkr_set->Settings.sysopt_drivers.bit.rf_receive_decimal);
      JBI->Add("IrDec",        tkr_set->Settings.sysopt_drivers.bit.ir_receive_decimal);
      JBI->Add("ArtNet",       tkr_set->Settings.sysopt_drivers.bit.artnet_autorun);
      JBI->Add("NeoSensitive", tkr_set->Settings.sysopt_drivers.bit.neopool_outputsensitive);
      JBI->Add("CounterBoth",  tkr_set->Settings.sysopt_drivers.bit.counter_both_edges);
      JBI->Add("NoPowerFb",    tkr_set->Settings.sysopt_drivers.bit.no_power_feedback);
    JBI->Object_End();

    JBI->Object_Start("P");
      JBI->Add("IrUnkThres",   tkr_set->Settings.sysopt_drivers.param.ir_unknown_threshold);
      JBI->Add("IrTol",        tkr_set->Settings.sysopt_drivers.param.ir_tolerance_percent);
      JBI->Add("BistableMs",   tkr_set->Settings.sysopt_drivers.param.bistable_pulse_ms);
      JBI->Add("RotaryMax",    tkr_set->Settings.sysopt_drivers.param.rotary_max_step);
      JBI->Add("HoldIgnore",   tkr_set->Settings.sysopt_drivers.param.hold_ignore_s);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_Sensors(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add_FV("Bits", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.sysopt_sensors.bit.data);

    JBI->Object_Start("B");
      JBI->Add("BtnRestrict",  tkr_set->Settings.sysopt_sensors.bit.button_restrict);
      JBI->Add("TempConv",     tkr_set->Settings.sysopt_sensors.bit.temperature_conversion);
      JBI->Add("PressConv",    tkr_set->Settings.sysopt_sensors.bit.pressure_conversion);
      JBI->Add("DS18Pullup",   tkr_set->Settings.sysopt_sensors.bit.ds18x20_internal_pullup);
      JBI->Add("HX711Change",  tkr_set->Settings.sysopt_sensors.bit.hx711_json_weight_change);
      JBI->Add("MHZ19NoABC",   tkr_set->Settings.sysopt_sensors.bit.mhz19b_abc_disable);
      JBI->Add("BtnSwap1Dev",  tkr_set->Settings.sysopt_sensors.bit.button_swap_on_single_device);
    JBI->Object_End();

    JBI->Object_Start("P");
      JBI->Add("DecPrec",      tkr_set->Settings.sysopt_sensors.param.decimal_precision);
      JBI->Add("HoldMs",       tkr_set->Settings.sysopt_sensors.param.key_hold_time_ms);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_Lights(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add_FV("Bits", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.sysopt_lights.bit.data);

    JBI->Object_Start("B");
      JBI->Add("PwmMulti",     tkr_set->Settings.sysopt_lights.bit.pwm_multi_channels);
    JBI->Object_End();

    JBI->Object_Start("P");
      JBI->Add("Reserved",     tkr_set->Settings.sysopt_lights.param.reserved);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_Power(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add_FV("Bits", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.sysopt_power.bit.data);

    JBI->Object_Start("B");
      JBI->Add("EnergyWeekend", tkr_set->Settings.sysopt_power.bit.energy_weekend);
      JBI->Add("HwEnergyTot",   tkr_set->Settings.sysopt_power.bit.hardware_energy_total);
      JBI->Add("NoExportToday", tkr_set->Settings.sysopt_power.bit.no_export_energy_today);
      JBI->Add("Bistable1Pin",  tkr_set->Settings.sysopt_power.bit.bistable_single_pin);
    JBI->Object_End();

    JBI->Object_Start("P");
      JBI->Add("MaxRetry",      tkr_set->Settings.sysopt_power.param.max_power_retry);
      JBI->Add("OverTemp",      tkr_set->Settings.sysopt_power.param.over_temperature_c);
      JBI->Add("CalcRes",       tkr_set->Settings.sysopt_power.param.calc_resolution);
      JBI->Add("WeightRes",     tkr_set->Settings.sysopt_power.param.weight_resolution);
      JBI->Add("FreqRes",       tkr_set->Settings.sysopt_power.param.frequency_resolution);
      JBI->Add("AxisRes",       tkr_set->Settings.sysopt_power.param.axis_resolution);
      JBI->Add("CurrentRes",    tkr_set->Settings.sysopt_power.param.current_resolution);
      JBI->Add("VoltageRes",    tkr_set->Settings.sysopt_power.param.voltage_resolution);
      JBI->Add("WattRes",       tkr_set->Settings.sysopt_power.param.wattage_resolution);
      JBI->Add("Emulation",     tkr_set->Settings.sysopt_power.param.emulation);
      JBI->Add("EnergyRes",     tkr_set->Settings.sysopt_power.param.energy_resolution);
      JBI->Add("PressRes",      tkr_set->Settings.sysopt_power.param.pressure_resolution);
      JBI->Add("HumRes",        tkr_set->Settings.sysopt_power.param.humidity_resolution);
      JBI->Add("TempRes",       tkr_set->Settings.sysopt_power.param.temperature_resolution);
      JBI->Add("PowerOnMs",     tkr_set->Settings.sysopt_power.param.power_on_delay_ms);
      JBI->Add("PowerOnS",      tkr_set->Settings.sysopt_power.param.power_on_delay_s);
      JBI->Add("BistableMs",    tkr_set->Settings.sysopt_power.param.bistable_pulse_ms);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_Rules(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    JBI->Add_FV("Bits", PSTR("\"%08lX\""), (unsigned long)tkr_set->Settings.sysopt_rules.bit.data);

    JBI->Object_Start("B");
      JBI->Add("SysInit",       tkr_set->Settings.sysopt_rules.bit.system_init);
      JBI->Add("SysBoot",       tkr_set->Settings.sysopt_rules.bit.system_boot);
      JBI->Add("TimeInit",      tkr_set->Settings.sysopt_rules.bit.time_init);
      JBI->Add("TimeSet",       tkr_set->Settings.sysopt_rules.bit.time_set);
      JBI->Add("MqttCon",       tkr_set->Settings.sysopt_rules.bit.mqtt_connected);
      JBI->Add("MqttDis",       tkr_set->Settings.sysopt_rules.bit.mqtt_disconnected);
      JBI->Add("WifiCon",       tkr_set->Settings.sysopt_rules.bit.wifi_connected);
      JBI->Add("WifiDis",       tkr_set->Settings.sysopt_rules.bit.wifi_disconnected);
      JBI->Add("EthCon",        tkr_set->Settings.sysopt_rules.bit.eth_connected);
      JBI->Add("EthDis",        tkr_set->Settings.sysopt_rules.bit.eth_disconnected);
      JBI->Add("HttpInit",      tkr_set->Settings.sysopt_rules.bit.http_init);
      JBI->Add("ShutterMoved",  tkr_set->Settings.sysopt_rules.bit.shutter_moved);
      JBI->Add("ShutterMoving", tkr_set->Settings.sysopt_rules.bit.shutter_moving);
    JBI->Object_End();

    JBI->Object_Start("P");
      JBI->Add("Reserved",      tkr_set->Settings.sysopt_rules.param.reserved);
    JBI->Object_End();

  return JBI->End();
}

uint8_t mTelemetry::ConstructJSON_Settings_Runtime(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Runtime settings/state snapshot.
     *
     * This reports the active runtime copy of selected settings and internal
     * state. It intentionally excludes large active GPIO structs and raw
     * runtime buffers unless they are exposed as compact status values.
     ********************************************************************/

    JBI->Object_Start("Core");
      JBI->Add("Sleep",       tkr_set->runtime.sleep);
      JBI->Add("Power",       (uint32_t)tkr_set->runtime.power);
      JBI->Add("PowerLatch",  (uint32_t)tkr_set->runtime.power_latching);
      JBI->Add("ActiveDev",   tkr_set->runtime.active_device);
      JBI->Add("MyModule",    tkr_set->runtime.my_module_type);
      JBI->Add("BootDone",    tkr_set->runtime.flag_boot_complete);
      JBI->Add("LastSource",  tkr_set->runtime.last_source);
      JBI->Add("LoopLoadAvg", tkr_set->runtime.loop_load_avg);
    JBI->Object_End();

    JBI->Object_Start("Firmware");
      JBI->Add("LightDriver", tkr_set->runtime.light_driver);
      JBI->Add("LightType",   tkr_set->runtime.light_type);
    JBI->Object_End();

    JBI->Object_Start("OTA");
      JBI->Add("State",       tkr_set->runtime.ota_state_flag);
      JBI->Add("Result",      tkr_set->runtime.ota_result);
      JBI->Add("Restart",     tkr_set->runtime.restart_flag);
      JBI->Add_FV("UrlPtr",   PSTR("\"%08lX\""), (unsigned long)tkr_set->runtime.ota_url);
    JBI->Object_End();

    JBI->Object_Start("Save");
      JBI->Add("Counter",     tkr_set->runtime.save_data_counter);
      JBI->Add("DelayS",      tkr_set->runtime.settings_save_decounter_seconds_delayed_save);
      JBI->Add("LKG",         tkr_set->runtime.settings_lkg);
      JBI->Add("HolderChanged", tkr_set->runtime.settings_holder_hardcorded_stored_changed);
    JBI->Object_End();

    JBI->Object_Start("Logging");
      JBI->Add("Serial",      tkr_set->runtime.seriallog_level);
      JBI->Add("Sys",         tkr_set->runtime.syslog_level);
      JBI->Add("SerialBoot",  tkr_set->runtime.seriallog_level_during_boot);
      JBI->Add("WebFilter",   tkr_set->runtime.enable_web_logging_filtering);
      JBI->Add("SerFilter",   tkr_set->runtime.enable_serial_logging_filtering);
    JBI->Object_End();

    JBI->Object_Start("Backlog");
      JBI->Add("Index",       tkr_set->runtime.backlog_index);
      JBI->Add("Pointer",     tkr_set->runtime.backlog_pointer);
    JBI->Object_End();

    JBI->Object_Start("LED");
      JBI->Add("Blinks",      tkr_set->runtime.blinks);
      JBI->Add("BlinkSpeed",  tkr_set->runtime.blinkspeed);
      JBI->Add("BlinkState",  tkr_set->runtime.blinkstate);
      JBI->Add("BlinkTimer",  tkr_set->runtime.blink_timer);
      JBI->Add("BlinkCounter", tkr_set->runtime.blink_counter);
    JBI->Object_End();

    JBI->Object_Start("Relay");
      JBI->Add("LatchPulse",  tkr_set->runtime.latching_relay_pulse);
      JBI->Add("PowerOnDelay", tkr_set->runtime.power_on_delay);
    JBI->Object_End();

    JBI->Object_Start("PWM");
      JBI->Add("Present",     tkr_set->runtime.pwm_present);
      JBI->Add("Inverted",    tkr_set->runtime.pwm_inverted);
    JBI->Object_End();

    JBI->Object_Start("Bus");
      JBI->Add("I2C",         tkr_set->runtime.i2c_enabled);
      #ifdef ESP32
      JBI->Add("I2C2",        tkr_set->runtime.i2c_enabled_2);
      #endif
      JBI->Add("SPI",         tkr_set->runtime.spi_flg);
      JBI->Add("SoftSPI",     tkr_set->runtime.soft_spi_flg);
    JBI->Object_End();

    JBI->Object_Start("Network");
      JBI->Add("WifiState",   tkr_set->runtime.wifi_state_flag);
      JBI->Add("MdnsDelay",   tkr_set->runtime.mdns_delayed_start);
      JBI->Add("NtpForce",    tkr_set->runtime.ntp_force_sync);
      JBI->Add("Hostname",    tkr_set->runtime.my_hostname);
    JBI->Object_End();

    JBI->Object_Start("Drivers");
      JBI->Add("Camera",      tkr_set->runtime.camera_initialized);
      JBI->Add("CounterNoPullup", tkr_set->runtime.counter_no_pullup);
      JBI->Add("SerialByte",  tkr_set->runtime.serial_in_byte);
    JBI->Object_End();

    JBI->Object_Start("Template");
      JBI->Add("Loading",     tkr_set->runtime.template_loading.status);
    JBI->Object_End();

    JBI->Object_Start("State");
      JBI->Add_FV("Global",   PSTR("\"%02X\""), tkr_set->runtime.global_state.data);
    JBI->Object_End();

    JBI->Object_Start("Fastboot");
      JBI->Add("NoRules",      tkr_set->runtime.fastboot.disable_rules);
      JBI->Add("NoSensors",    tkr_set->runtime.fastboot.disable_sensors);
      JBI->Add("NoDrivers",    tkr_set->runtime.fastboot.disable_drivers);
      JBI->Add("NoModCfg",     tkr_set->runtime.fastboot.disable_module_config_load);
      JBI->Add("NoTemplates",  tkr_set->runtime.fastboot.disable_templates);
      JBI->Add("SafeCfg",      tkr_set->runtime.fastboot.force_safe_compiled_config);
      JBI->Add("FactoryAP",    tkr_set->runtime.fastboot.factory_ap_recovery);
      JBI->Add("BlockingSafe", tkr_set->runtime.fastboot.blocking_safe_mode);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Settings_TextBuffer(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Settings text buffer / device-name registry internals.
     *
     * This reports the compact indexed string buffer used by settings for
     * device/class/name lookup. It is kept out of /settings because it can be
     * verbose and is mainly useful for debugging registry allocation.
     ********************************************************************/

    const uint8_t used = DLI->GetLengthIndexUsed();
    const uint16_t buflen = strlen(tkr_set->Settings.device_name_buffer.name_buffer);

    uint8_t item_count = 0;
    if (buflen) {
      item_count = DLI->CountCharInCtr(tkr_set->Settings.device_name_buffer.name_buffer, '|');
    }

    JBI->Object_Start("Summary");
      JBI->Add("Used",      used);
      JBI->Add("BufLen",    buflen);
      JBI->Add("Items",     item_count);
    JBI->Object_End();

    if (json_level >= JSON_LEVEL_DETAILED){
      JBI->Add(PM_BUFFER, tkr_set->Settings.device_name_buffer.name_buffer);
    }

    JBI->Array_AddArray(PM_DEVICEID, tkr_set->Settings.device_name_buffer.device_id, used);
    JBI->Array_AddArray(PM_CLASSID,  tkr_set->Settings.device_name_buffer.class_id,  used);


  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Peripherals(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Peripheral / bus summary.
     *
     * This reports physical peripheral availability and compact bus state.
     * Detailed module readings remain in their own module telemetry topics.
     ********************************************************************/

    JBI->Object_Start("Summary");
      JBI->Add("I2C",     tkr_pins->PinUsed(GPIO_I2C_SCL) && tkr_pins->PinUsed(GPIO_I2C_SDA));
      JBI->Add("SPI",     tkr_set->runtime.spi_flg);
      JBI->Add("SoftSPI", tkr_set->runtime.soft_spi_flg);
      JBI->Add("PWM",     tkr_set->runtime.pwm_present);
      JBI->Add("ADC",     false);  // placeholder until ADC summary is wired
      JBI->Add("UART",    false);  // placeholder until UART summary is wired
    JBI->Object_End();

    JBI->Object_Start("I2C");
      const bool i2c_enabled = tkr_pins->PinUsed(GPIO_I2C_SCL) && tkr_pins->PinUsed(GPIO_I2C_SDA);

      JBI->Add("Enabled", i2c_enabled);

      if (i2c_enabled)
      {
        #ifdef ESP32
        JBI->Add("Speed", tkr_i2c->wire->getClock());
        #endif

        char mqtt_data[300];
        tkr_i2c->I2cScan(mqtt_data, sizeof(mqtt_data));

        // I2cScan already returns JSON content, so append it raw.
        JBI->Append(",\"Scan\":");
        JBI->Append(mqtt_data);
      }
    JBI->Object_End();

    JBI->Object_Start("SPI");
      JBI->Add("Enabled", tkr_set->runtime.spi_flg);
      JBI->Add("Soft",    tkr_set->runtime.soft_spi_flg);
    JBI->Object_End();

    JBI->Object_Start("PWM");
      JBI->Add("Present",  tkr_set->runtime.pwm_present);
      JBI->Add("Inverted", tkr_set->runtime.pwm_inverted);
    JBI->Object_End();

    JBI->Object_Start("ADC");
      JBI->Add("Present", false);
    JBI->Object_End();

    JBI->Object_Start("UART");
      JBI->Add("Present", false);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Firmware(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Firmware/build identity.
     *
     * This payload describes what binary is running, how it was built, and
     * what platform it is running on. Runtime health belongs in /health.
     * Runtime recovery/safe-mode flags belong in settings/runtime.
     ********************************************************************/

    JBI->Object_Start("Firmware");
      JBI->Add(PM_VERSION_NAME,            tkr_set->runtime.firmware_version.current.name_ctr);
      JBI->Add(PM_VERSION_NUMBER,          (uint32_t)PROJECT_VERSION);
      JBI->Add(PM_VERSION_NUMBER_NOTYPE,   (uint32_t)PROJECT_VERSION & 0x3FFFFFFF); // suppress 2 MSBs

      #ifdef FIRMWARE_BRANCH_NAME_STRING
      JBI->Add("Branch",                     FIRMWARE_BRANCH_NAME_STRING);
      #endif

      JBI->Array_Start(PM_VERSION_PARTS);
        JBI->Add((uint8_t)FIRMWARE_VERSION_TYPE);
        JBI->Add((uint8_t)FIRMWARE_VERSION_MAJOR);
        JBI->Add((uint8_t)FIRMWARE_VERSION_MINOR);
        JBI->Add((uint8_t)FIRMWARE_VERSION_CORE);
        JBI->Add((uint8_t)FIRMWARE_VERSION_MODULE);
      JBI->Array_End();
    JBI->Object_End();

    JBI->Object_Start("Build");
      JBI->Add(PM_BUILDDATE,               __DATE__);
      JBI->Add(PM_BUILDTIME,               __TIME__);
      JBI->Add(PM_BUILDDATETIME,           tkr_time->GetBuildDateAndTime().c_str());

      #ifdef DEVICENAME_BUILD_ENVIRONMENT
      JBI->Add("Environment",              DEVICENAME_BUILD_ENVIRONMENT);
      #endif

      #ifdef PIOENV
      JBI->Add("PIOEnv",                   PIOENV);
      #endif
    JBI->Object_End();


    JBI->Object_Start("Platform");

      #if defined(ARDUINO_ARCH_ESP32)
      JBI->Add("Arch",                     "ESP32");
      JBI->Add(PM_SDKVERSION,              ESP.getSdkVersion());
      JBI->Add("ChipModel",                ESP.getChipModel());
      JBI->Add("ChipRev",                  ESP.getChipRevision());
      JBI->Add("CpuMHz",                   ESP.getCpuFreqMHz());
      JBI->Add("FlashSize",                ESP.getFlashChipSize());
      JBI->Add("SketchSize",               ESP.getSketchSize());
      JBI->Add(PM_FREESKETCHSPACE,         ESP.getFreeSketchSpace());

      #elif defined(ARDUINO_ARCH_ESP8266)
      JBI->Add("Arch",                     "ESP8266");
      JBI->Add(PM_SDKVERSION,              ESP.getSdkVersion());
      JBI->Add("Core",                     ESP.getCoreVersion().c_str());
      JBI->Add("CpuMHz",                   ESP.getCpuFreqMHz());
      JBI->Add("FlashSize",                ESP.getFlashChipRealSize());
      JBI->Add("SketchSize",               ESP.getSketchSize());
      JBI->Add(PM_FREESKETCHSPACE,         ESP.getFreeSketchSpace());
      JBI->Add(PM_BOOTVERSION,             ESP.getBootVersion());

      #else
      JBI->Add("Arch",                     "Unknown");
      #endif

    JBI->Object_End();

    JBI->Array_Start(PM_FASTBOOT_RECOVERY);
      #ifdef ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
      JBI->Add("OTA");
      #endif

      #ifdef ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
      JBI->Add("HTTP");
      #endif

      #if !defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) && !defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)
      JBI->Add("None");
      #endif
    JBI->Array_End();

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    JBI->Add(PM_WATCHDOG, true);
    #else
    JBI->Add(PM_WATCHDOG, false);
    #endif

  return JBI->End();
}

uint8_t mTelemetry::ConstructJSON_Log(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Logging telemetry.
     *
     * This topic owns logging configuration and runtime logging state.
     * /settings should not duplicate these values.
     ********************************************************************/

    JBI->Object_Start("Levels");
      JBI->Add_P(PM_SERIAL,  tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.serial_level),         VALUE_IS_PROGMEM);
      JBI->Add_P(PM_SYSTEM,  tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.sys_level),            VALUE_IS_PROGMEM);
      JBI->Add_P(PM_WEB,     tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.web_level),            VALUE_IS_PROGMEM);
      JBI->Add_P(PM_TELNET,  tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.telnet_level),         VALUE_IS_PROGMEM);
      JBI->Add_P("MQTT",     tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.mqtt_level),           VALUE_IS_PROGMEM);
      JBI->Add_P("Display",  tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.display_serial_level), VALUE_IS_PROGMEM);
    JBI->Object_End();

    JBI->Object_Start("Runtime");
      JBI->Add_P("SerialNow", tkr_log->GetLogLevelNamebyID(tkr_set->runtime.seriallog_level),              VALUE_IS_PROGMEM);
      JBI->Add_P("SysNow",    tkr_log->GetLogLevelNamebyID(tkr_set->runtime.syslog_level),                 VALUE_IS_PROGMEM);
      JBI->Add_P("SerialBoot", tkr_log->GetLogLevelNamebyID(tkr_set->runtime.seriallog_level_during_boot), VALUE_IS_PROGMEM);
      JBI->Add("WebFilter",   tkr_set->runtime.enable_web_logging_filtering);
      JBI->Add("SerFilter",   tkr_set->runtime.enable_serial_logging_filtering);
    JBI->Object_End();

    JBI->Object_Start("Syslog");
      JBI->Add("Port",        tkr_set->Settings.logging.sys_port);
      JBI->Add_P("Level",     tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.sys_level),           VALUE_IS_PROGMEM);
    JBI->Object_End();

    JBI->Object_Start("Telnet");
      JBI->Add("Port",        TELNET_PORT);
      JBI->Add_P("Level",     tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.telnet_level),        VALUE_IS_PROGMEM);
      JBI->Add("Running",     tkr_log->telnet.running);
      JBI->Add("Client",      (bool)(tkr_log->telnet.client && tkr_log->telnet.client.connected()));
    JBI->Object_End();

    JBI->Object_Start("Format");
      JBI->Add("TimeShort",   tkr_set->Settings.logging.time_isshort);
    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Memory(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Memory/storage telemetry.
     *
     * /health carries the small frequent memory indicators.
     * This topic carries the fuller heap/sketch/flash view.
     ********************************************************************/

    JBI->Object_Start("Heap");

      #ifdef ESP8266
      JBI->Add("FreeKB",        ESP.getFreeHeap() / 1024);
      JBI->Add("MaxBlockKB",    ESP.getMaxFreeBlockSize() / 1024);
      JBI->Add("FragPct",       ESP.getHeapFragmentation());

      #else
      JBI->Add("FreeKB",        ESP.getFreeHeap() / 1024);
      JBI->Add("Free8KB",       heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024);
      JBI->Add("Largest8KB",    heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) / 1024);
      JBI->Add("Min8KB",        heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT) / 1024);
      #endif

    JBI->Object_End();

    JBI->Object_Start("Sketch");
      JBI->Add("SizeKB",        ESP.getSketchSize() / 1024);
      JBI->Add("FreeKB",        ESP.getFreeSketchSpace() / 1024);
    JBI->Object_End();

    JBI->Object_Start("Flash");

      #ifdef ESP8266
      JBI->Add("ChipSizeKB",    ESP.getFlashChipSize() / 1024);
      JBI->Add("RealSizeKB",    ESP.getFlashChipRealSize() / 1024);
      JBI->Add("ChipID",        ESP.getFlashChipId());
      JBI->Add("Mode",          (uint8_t)ESP.getFlashChipMode());

      #else
      JBI->Add("ChipSizeKB",    ESP.getFlashChipSize() / 1024);
      JBI->Add("Mode",          (uint8_t)ESP.getFlashChipMode());

      const uint64_t efuse_mac = ESP.getEfuseMac();
      JBI->Add_FV("EfuseMac",   PSTR("\"%016llX\""), (unsigned long long)efuse_mac);
      #endif

    JBI->Object_End();

    #ifdef ESP32
    if (ESP.getPsramSize() > 0)
    {
      JBI->Object_Start("PSRAM");
        JBI->Add("SizeKB",      ESP.getPsramSize() / 1024);
        JBI->Add("FreeKB",      ESP.getFreePsram() / 1024);
      JBI->Object_End();
    }
    #endif

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Network(uint8_t json_level, bool json_appending)
{
  JBI->Start();
  
    /********************************************************************
     * Network telemetry.
     *
     * This topic reports active/resolved network state.
     * Persistent network intent/options belong in settings/network.
     * MQTT connection state belongs in mqtt/health.
     * Telnet/syslog/logging details belong in log.
     ********************************************************************/

    JBI->Object_Start("Summary");
      #ifdef USE_MODULE_NETWORK_INTERFACE
      JBI->Add("Any",       tkr_interface_network->Connected(mInterfaceNetwork::NETWORK_TYPE_ANY));
      JBI->Add("Local",     tkr_interface_network->Network_HasLocalConnectivity());
      JBI->Add("External",  tkr_interface_network->Network_HasExternalConnectivity());
      JBI->Add("LocalOnly", tkr_interface_network->Network_IsLocalOnly());
      #endif

      JBI->Add(PM_HOSTNAME, tkr_set->runtime.my_hostname);
      JBI->Add("WebServer", tkr_set->Settings.webserver);
    JBI->Object_End();

    #ifdef USE_MODULE_NETWORK_WIFI
    {
      IPAddress sta_ip      = WiFi.localIP();
      IPAddress sta_gw      = WiFi.gatewayIP();
      IPAddress sta_sn      = WiFi.subnetMask();
      IPAddress sta_dns     = WiFi.dnsIP();
      IPAddress ap_ip       = WiFi.softAPIP();

      IPAddress cfg_ip      = IPAddress(tkr_wifi->config.station.ipv4.ip[0],   tkr_wifi->config.station.ipv4.ip[1],   tkr_wifi->config.station.ipv4.ip[2],   tkr_wifi->config.station.ipv4.ip[3]);
      IPAddress cfg_gw      = IPAddress(tkr_wifi->config.station.ipv4.gw[0],   tkr_wifi->config.station.ipv4.gw[1],   tkr_wifi->config.station.ipv4.gw[2],   tkr_wifi->config.station.ipv4.gw[3]);
      IPAddress cfg_sn      = IPAddress(tkr_wifi->config.station.ipv4.sn[0],   tkr_wifi->config.station.ipv4.sn[1],   tkr_wifi->config.station.ipv4.sn[2],   tkr_wifi->config.station.ipv4.sn[3]);
      IPAddress cfg_dns1    = IPAddress(tkr_wifi->config.station.ipv4.dns1[0], tkr_wifi->config.station.ipv4.dns1[1], tkr_wifi->config.station.ipv4.dns1[2], tkr_wifi->config.station.ipv4.dns1[3]);
      IPAddress cfg_dns2    = IPAddress(tkr_wifi->config.station.ipv4.dns2[0], tkr_wifi->config.station.ipv4.dns2[1], tkr_wifi->config.station.ipv4.dns2[2], tkr_wifi->config.station.ipv4.dns2[3]);

      JBI->Object_Start("WiFi");

        JBI->Object_Start("State");
          JBI->Add("Mode",        (uint8_t)WiFi.getMode());
          JBI->Add("Status",      (uint8_t)WiFi.status());
          JBI->Add("Local",       tkr_wifi->WiFi_HasLocalConnectivity());
          JBI->Add("External",    tkr_wifi->WiFi_HasExternalConnectivity());
          JBI->Add("Routable",    tkr_wifi->WiFi_Link_IsIpRoutable());
          JBI->Add("Connected",   tkr_wifi->connection.fConnected);
          JBI->Add("Reconnect",   tkr_wifi->connection.fReconnect);
          JBI->Add("LinkCount",   tkr_wifi->connection.link_count);
          JBI->Add("Downtime",    tkr_wifi->connection.downtime);
          JBI->Add("RetryIn",     tkr_wifi->connection.counter);
          JBI->Add("FreshIn",     tkr_wifi->connection.seconds_to_wait_for_fresh_connection_attempt);
        JBI->Object_End();

        JBI->Object_Start("STA");
          JBI->Add("Enabled",     tkr_wifi->config.station.enabled);
          JBI->Add("Profile",     tkr_wifi->config.station.active_profile);
          JBI->Add("SSID",        WiFi.SSID().c_str());
          JBI->Add(PM_RSSI,       WiFi.RSSI());
          JBI->Add(PM_BSSID,      WiFi.BSSIDstr().c_str());
          JBI->Add(PM_MAC,        WiFi.macAddress().c_str());

          #ifdef ESP32
          JBI->Add("TXPower_dBm", tkr_wifi->WiFiPower_To_dBm(WiFi.getTxPower()));
          #endif

          JBI->Add_FV(PM_IPADDRESS,  PSTR("\"%d.%d.%d.%d\""), sta_ip[0],  sta_ip[1],  sta_ip[2],  sta_ip[3]);
          JBI->Add_FV(PM_GATEWAY,    PSTR("\"%d.%d.%d.%d\""), sta_gw[0],  sta_gw[1],  sta_gw[2],  sta_gw[3]);
          JBI->Add_FV(PM_SUBNETMASK, PSTR("\"%d.%d.%d.%d\""), sta_sn[0],  sta_sn[1],  sta_sn[2],  sta_sn[3]);
          JBI->Add_FV(PM_DNSSERVER,  PSTR("\"%d.%d.%d.%d\""), sta_dns[0], sta_dns[1], sta_dns[2], sta_dns[3]);
        JBI->Object_End();

        JBI->Object_Start("STAConfig");
          JBI->Add("Static",      tkr_wifi->config.station.ipv4.is_static);
          JBI->Add_FV(PM_STATIC_IPADDRESS, PSTR("\"%d.%d.%d.%d\""), cfg_ip[0],   cfg_ip[1],   cfg_ip[2],   cfg_ip[3]);
          JBI->Add_FV(PM_GATEWAY,          PSTR("\"%d.%d.%d.%d\""), cfg_gw[0],   cfg_gw[1],   cfg_gw[2],   cfg_gw[3]);
          JBI->Add_FV(PM_SUBNETMASK,       PSTR("\"%d.%d.%d.%d\""), cfg_sn[0],   cfg_sn[1],   cfg_sn[2],   cfg_sn[3]);
          JBI->Add_FV("DNS1",              PSTR("\"%d.%d.%d.%d\""), cfg_dns1[0], cfg_dns1[1], cfg_dns1[2], cfg_dns1[3]);
          JBI->Add_FV("DNS2",              PSTR("\"%d.%d.%d.%d\""), cfg_dns2[0], cfg_dns2[1], cfg_dns2[2], cfg_dns2[3]);
        JBI->Object_End();

        JBI->Object_Start("AP");
          JBI->Add("Enabled",     tkr_wifi->config.softap.enabled);
          JBI->Add("SSID",        tkr_wifi->config.softap.ssid);
          JBI->Add("Channel",     tkr_wifi->config.softap.channel);
          JBI->Add("Hidden",      tkr_wifi->config.softap.hidden);
          JBI->Add_FV("IP",       PSTR("\"%d.%d.%d.%d\""), ap_ip[0], ap_ip[1], ap_ip[2], ap_ip[3]);
          JBI->Add("DnsRunning",  tkr_wifi->dnsServerRunning);
        JBI->Object_End();

        JBI->Array_Start(PM_AP_LIST);
          for (uint8_t i = 0; i < WIFI_MAXIMUM_CONNECTIONS; i++)
          {
            if (tkr_wifi->config.station.profiles[i].ssid[0] != 0)
            {
              JBI->Object_Start();
                JBI->Add("I",        i);
                JBI->Add("SSID",     tkr_wifi->config.station.profiles[i].ssid);
                JBI->Add("ID",       tkr_wifi->config.station.profiles[i].id);
                JBI->Add("Priority", tkr_wifi->config.station.profiles[i].priority);
                JBI->Add("Hidden",   tkr_wifi->config.station.profiles[i].ssid_hidden);
                JBI->Add("BSSID",    tkr_wifi->config.station.profiles[i].has_bssid);
              JBI->Object_End();
            }
          }
        JBI->Array_End();

      JBI->Object_End();
    }
    #endif

    #ifdef USE_MODULE_NETWORK_ETHERNET
    #ifdef ESP32
    {
      IPAddress eth_ip  = ETH.localIP();
      IPAddress eth_gw  = ETH.gatewayIP();
      IPAddress eth_sn  = ETH.subnetMask();
      IPAddress eth_dns = ETH.dnsIP();

      JBI->Object_Start("Ethernet");
        JBI->Add("Link",       tkr_eth->Ethernet_HasLink());
        JBI->Add("HasIP",      tkr_eth->Ethernet_HasIP());
        JBI->Add("Routable",   tkr_eth->Ethernet_IsRoutable());
        JBI->Add("Connected",  tkr_eth->Ethernet_IsRoutable());
        JBI->Add("Hostname",   tkr_eth->EthernetHostname());
        JBI->Add("MAC",        tkr_eth->EthernetMacAddress().c_str());

        JBI->Add_FV("IP",      PSTR("\"%d.%d.%d.%d\""), eth_ip[0],  eth_ip[1],  eth_ip[2],  eth_ip[3]);
        JBI->Add_FV("Gateway", PSTR("\"%d.%d.%d.%d\""), eth_gw[0],  eth_gw[1],  eth_gw[2],  eth_gw[3]);
        JBI->Add_FV("Subnet",  PSTR("\"%d.%d.%d.%d\""), eth_sn[0],  eth_sn[1],  eth_sn[2],  eth_sn[3]);
        JBI->Add_FV("DNS",     PSTR("\"%d.%d.%d.%d\""), eth_dns[0], eth_dns[1], eth_dns[2], eth_dns[3]);
      JBI->Object_End();
    }
    #endif
    #endif

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_MQTT(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * MQTT telemetry.
     *
     * This topic reports MQTT manager state and all configured broker
     * instances. It intentionally excludes full network transport state.
     *
     * Note:
     *  - User is published for broker diagnostics.
     *  - Password is not published; only PasswordSet is reported.
     ********************************************************************/

    uint8_t broker_count     = 0;
    uint8_t broker_enabled   = 0;
    uint8_t broker_allowed   = 0;
    uint8_t broker_connected = 0;

    uint16_t connect_count_total    = 0;
    uint16_t downtime_counter_total = 0;

    for (auto& con : tkr_mqtt->brokers)
    {
      if (!con) { continue; }

      broker_count++;

      if (con->en) {
        broker_enabled++;
      }

      if (con->allowed) {
        broker_allowed++;
      }

      if (con->MqttIsConnected()) {
        broker_connected++;
      }

      connect_count_total    += con->connect_count;
      downtime_counter_total += con->downtime_counter;
    }

    JBI->Object_Start("Summary");
      JBI->Add("Brokers",      broker_count);
      JBI->Add("Enabled",      broker_enabled);
      JBI->Add("Allowed",      broker_allowed);
      JBI->Add("Connected",    broker_connected);
      JBI->Add("ConnectCount", connect_count_total);
      JBI->Add("Downtime",     downtime_counter_total);
    JBI->Object_End();

    JBI->Object_Start(PM_REFRESH_RATES);
      JBI->Add(PM_MQTT_REFRESH_RATE_IFCHANGED, tkr_mqtt->dt.ifchanged_secs);
      JBI->Add(PM_MQTT_REFRESH_RATE_TELEPERIOD, tkr_mqtt->dt.teleperiod_secs);
    JBI->Object_End();

    JBI->Add("ReduceFreq", tkr_mqtt->flag_uptime_reached_reduce_frequency);
 
    JBI->Array_Start("Brokers");

      for (uint8_t i = 0; i < tkr_mqtt->brokers.size(); i++)
      {
        MQTTConnection* con = tkr_mqtt->brokers[i];

        if (!con) { continue; }

        JBI->Object_Start();

          JBI->Add("ID", con->id);

          JBI->Add("Host", con->host_address);
          JBI->Add("Port", con->port);
          JBI->Add("TLS",  con->mqtt_tls);
          JBI->Add("Type", con->host_server_type);
          
          JBI->Add("Name",        con->client_name);
          JBI->Add("User",        con->user);
          JBI->Add("PasswordSet", con->password[0] != '\0');

          JBI->Add("En",         con->en);
          JBI->Add("Allowed",    con->allowed);
          JBI->Add("Status",     con->status);

          JBI->Add("Connect",    con->connect_count);
          JBI->Add("Attempts",   con->cConnectionAttempts);
          JBI->Add("Retry",      con->retry_counter);
          JBI->Add("RetryStart", con->retry_counter_start_value);
          JBI->Add("Downtime",   con->downtime_counter);
          JBI->Add("Uptime",     con->uptime_seconds);
          JBI->Add("LastOut",    con->tSaved_LastOutGoingTopic);
          JBI->Add("RetryS",     con->retry);
          JBI->Add("FastS",      con->backoff_fast_secs);
          JBI->Add("PeriodicS",  con->backoff_periodic_secs);
          JBI->Add("MaxS",       con->backoff_max_secs);
          JBI->Add("KeepAliveS", con->keepalive_secs);
        
          JBI->Object_Start("Transport");
            JBI->Add("Client", (uint8_t)con->client_type);

            JBI->Array_Start("Allowed");
              for (uint8_t j = 0; j < con->transport_count; j++){ JBI->Add((uint8_t)con->transport[j]); }
            JBI->Array_End();

            JBI->Array_Start("Pref");
              for (uint8_t j = 0; j < con->pref_transport_count; j++){ JBI->Add((uint8_t)con->pref_transport[j]); }
            JBI->Array_End();
          JBI->Object_End();

          JBI->Object_Start("Limits");
            JBI->Add("OutLevel",  con->outgoing_level);
            JBI->Add("LimiterMs", con->outgoing_limiter_ms);
          JBI->Object_End();

          #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
          JBI->Object_Start("Debug");
            JBI->Add("Sent",       con->debug_stats.payload_publish_sent);
            JBI->Add("Missed",     con->debug_stats.payload_publish_missed);
            JBI->Add("SuccessPct", con->debug_stats.payload_publish_success_percentage);
          JBI->Object_End();
          #endif

        JBI->Object_End();
      }

    JBI->Array_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Time(uint8_t json_level, bool json_appending)
{
  char buffer[40];

  JBI->Start();

    /********************************************************************
     * Time telemetry.
     *
     * This topic reports current RTC/NTP state, timezone/DST state, and
     * derived day/week/year counters. Settings should not duplicate this.
     ********************************************************************/

    JBI->Object_Start("Now");
      JBI->Add(PM_UTC_TIME,       tkr_time->GetDateAndTime(DT_UTC).c_str());
      JBI->Add(PM_LOCAL_TIME,     tkr_time->GetDateAndTime(DT_LOCAL).c_str());
      JBI->Add(PM_TIMEZONE,       tkr_time->GetDateAndTime(DT_TIMEZONE).c_str());
      JBI->Add("EpochUTC",        tkr_time->UtcTime());
      JBI->Add("EpochLocal",      tkr_time->LocalTime());
      JBI->Add("Millis",          tkr_time->RtcMillis());
      JBI->Add("Valid",           tkr_time->Rtc.time_synced);
      JBI->Add("Synced",          tkr_time->Rtc.time_synced);
      JBI->Add("LastSynced",      tkr_time->Rtc.last_synced);
      JBI->Add("UserEntry",       tkr_time->Rtc.user_time_entry);
    JBI->Object_End();

    JBI->Object_Start("Uptime");
      JBI->Add("Text",            tkr_time->GetUptime(buffer, sizeof(buffer)));
      JBI->Add("Seconds",         tkr_time->UpTime());
      JBI->Add("Minutes",         tkr_time->MinutesUptime());
      JBI->Add("NonReset",        tkr_time->uptime_seconds_nonreset);
    JBI->Object_End();

    JBI->Object_Start("Day");
      JBI->Add("Midnight",        tkr_time->Midnight());
      JBI->Add("MidnightNow",     tkr_time->MidnightNow());
      JBI->Add("Seconds",         tkr_time->SecondsPastMidnight_SecondsOfCurrentDay());
      JBI->Add("Minutes",         tkr_time->MinutesPastMidnight());
      JBI->Add("LastDayRun",      tkr_time->lastday_run);
    JBI->Object_End();

    JBI->Object_Start("RTC");
      JBI->Add("UTC",             tkr_time->Rtc.utc_time);
      JBI->Add("Local",           tkr_time->Rtc.local_time);
      JBI->Add("DST",             tkr_time->Rtc.daylight_saving_time);
      JBI->Add("STD",             tkr_time->Rtc.standard_time);
      JBI->Add("Restart",         tkr_time->Rtc.restart_time);
      JBI->Add("TimezoneOffset",  tkr_time->Rtc.time_timezone);
      JBI->Add("ForceNTP",        tkr_time->ntp_force_sync);
    JBI->Object_End();

    JBI->Object_Start("DST");
      JBI->Add("Active",          tkr_time->IsDst());
      JBI->Add(PM_STARTDST,       tkr_time->GetDateAndTime(DT_DST).c_str());
      JBI->Add(PM_ENDDST,         tkr_time->GetDateAndTime(DT_STD).c_str());

      JBI->Object_Start("Rule0");
        JBI->Add("Hemis",         tkr_time->tflag[0].hemis);
        JBI->Add("Week",          tkr_time->tflag[0].week);
        JBI->Add("Month",         tkr_time->tflag[0].month);
        JBI->Add("DOW",           tkr_time->tflag[0].dow);
        JBI->Add("Hour",          tkr_time->tflag[0].hour);
      JBI->Object_End();

      JBI->Object_Start("Rule1");
        JBI->Add("Hemis",         tkr_time->tflag[1].hemis);
        JBI->Add("Week",          tkr_time->tflag[1].week);
        JBI->Add("Month",         tkr_time->tflag[1].month);
        JBI->Add("DOW",           tkr_time->tflag[1].dow);
        JBI->Add("Hour",          tkr_time->tflag[1].hour);
      JBI->Object_End();
    JBI->Object_End();

    JBI->Object_Start("Sun");
      JBI->Add(PM_SUNRISE,        tkr_time->GetDateAndTime(DT_SUNRISE).c_str());
      JBI->Add(PM_SUNSET,         tkr_time->GetDateAndTime(DT_SUNSET).c_str());
    JBI->Object_End();

    JBI->Object_Start("Calendar");
      JBI->Add("Second",          tkr_time->RtcTime.second);
      JBI->Add("Minute",          tkr_time->RtcTime.minute);
      JBI->Add("Hour",            tkr_time->RtcTime.hour);
      JBI->Add("DOW",             tkr_time->RtcTime.day_of_week);
      JBI->Add("DOM",             tkr_time->RtcTime.day_of_month);
      JBI->Add("Month",           tkr_time->RtcTime.month);
      JBI->Add("Year",            tkr_time->RtcTime.year);
      JBI->Add("DOY",             tkr_time->RtcTime.day_of_year);
      JBI->Add("Week",            tkr_time->RtcTime.week);
      JBI->Add("DSeconds",        tkr_time->RtcTime.Dseconds);
      JBI->Add("WSeconds",        tkr_time->RtcTime.Wseconds);
      JBI->Add("YSeconds",        tkr_time->RtcTime.Yseconds);
      JBI->Add("Valid",           tkr_time->RtcTime.valid);
    JBI->Object_End();

    JBI->Object_Start("Build");
      JBI->Add("DateTime",        tkr_time->GetBuildDateAndTime().c_str());
      JBI->Add("Age",             tkr_time->BuildDateTimeElapsed());
      JBI->Add("SplashAllowed",   tkr_time->IsBuildDateTimeElapsedBeyond(SECONDS_FROM_BUILDTIME_TO_ENABLE_SPLASHING_TELEMETRY));
    JBI->Object_End();

  return JBI->End();
}


                            uint8_t mTelemetry::ConstructJSON_Reboot(uint8_t json_level, bool json_appending)
                            {

                              // I could add if reboot event (based on json_appending)
                              // Then if last was clean, dont send and return early


                              JBI->Start();

                              // Onlu sent, when "ALL" indicates a "Event" was fired
                              if(json_level == JSON_LEVEL_ALL)
                                JBI->Add("ResetEvent", "Once");

                              JBI->Add(PM_DEVICE,             tkr_set->Settings.system_name.device);
                              JBI->Add(PM_DEVICEFRIENDLYNAME, tkr_set->Settings.system_name.friendly);

                              JBI->Add_FV("ResetDateTime", PSTR("\"%02d-%02d-%02d %02d:%02d:%02d\""),
                                          tkr_time->RtcTime.day_of_month, tkr_time->RtcTime.month, tkr_time->RtcTime.year,
                                          tkr_time->RtcTime.hour, tkr_time->RtcTime.minute, tkr_time->RtcTime.second);

                              // Reset reason (ESP8266 string / ESP32 enum mapped by GetResetReason)
                              char reset_reason[64] = {0};
                              tkr_sup->GetResetReason(reset_reason, sizeof(reset_reason));
                              if (reset_reason[0] == '\0') {
                                strncpy(reset_reason, "Unknown", sizeof(reset_reason) - 1);
                                reset_reason[sizeof(reset_reason) - 1] = '\0';
                              }
                              JBI->Add("ResetReason", reset_reason);

                              // OTA boot flag (same behaviour as your LWT)
                              #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
                              JBI->Add("OTABootReason", (RtcMemory__RuntimeState.boot_was_completed_ota_event == 1) ? "OTAYes" : "OTANo");
                              #else
                              JBI->Add("OTABootReason", "Unknown");
                              #endif

                              // Distinguish “crash-loop on boot” vs “long-running then rebooted”
                              JBI->Add("UptimeSeconds", tkr_time->UpTime());

                              // Existing blocks (leave as-is)
                              JBI->Object_Start(PM_COUNTER);
                                JBI->Add("All", (uint8_t)0);
                              JBI->Object_End();

                              JBI->Object_Start(PM_CRASHDUMP);
                                tkr_sup->WriteBuffer_P(PSTR(","));
                                // tkr_crash_recorder->CrashDump_AddJson();
                              JBI->Object_End();

                              return JBI->End();
                            }


uint8_t mTelemetry::ConstructJSON_Debug_Pins_GPIO(uint8_t json_level, bool json_appending)
{
  char name_buffer[64];

  JBI->Start();

    /********************************************************************
     * GPIO summary.
     *
     * Object key   = decoded GPIO function name
     * Object value = real physical GPIO pin number
     *
     * This is the compact human-readable pin map.
     ********************************************************************/

    JBI->Object_Start(PM_GPIO);

      for (uint16_t real_pin = 0; real_pin < MAX_GPIO_PIN; real_pin++)
      {
        const uint16_t packed_gpio = tkr_pins->pin[real_pin].gpio_function;

        if ((packed_gpio == GPIO_NONE) || (packed_gpio == GPIO_USER))
        {
          continue;
        }

        tkr_pins->GetGPIOFunctionNamebyID(
          packed_gpio,
          name_buffer,
          sizeof(name_buffer)
        );

        JBI->Add(name_buffer, real_pin);
      }

    JBI->Object_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Debug_Pins_Table(uint8_t json_level, bool json_appending)
{
  char name_buffer[64];

  JBI->Start();

    /********************************************************************
     * Detailed pin table.
     *
     * Compact keys are intentional because this payload can become large.
     *
     * p   = real physical GPIO pin number
     * n   = decoded GPIO function name
     * r   = raw packed GPIO function
     * b   = unpacked GPIO base function ID
     * i   = packed function instance index
     * m   = owning module/core ID
     * ph  = physical capability flags
     * af  = allocation flags
     * a   = allocated
     * l   = locked
     * g   = grouped
     * s   = shared
     * c   = conflict
     ********************************************************************/

    JBI->Array_Start("Pin");

      for (uint16_t real_pin = 0; real_pin < MAX_GPIO_PIN; real_pin++)
      {
        const uint16_t packed_gpio = tkr_pins->pin[real_pin].gpio_function;

        if ((packed_gpio == GPIO_NONE) || (packed_gpio == GPIO_USER))
        {
          continue;
        }

        const uint16_t base_id = UGPIO(packed_gpio);
        const uint8_t  func_i  = packed_gpio & GPIO_INDEX_MASK;

        tkr_pins->GetGPIOFunctionNamebyID(
          packed_gpio,
          name_buffer,
          sizeof(name_buffer)
        );

        JBI->Object_Start();
          JBI->Add("p",  real_pin);
          JBI->Add("n",  name_buffer);
          JBI->Add("r",  packed_gpio);
          JBI->Add("b",  base_id);
          JBI->Add("i",  func_i);

          JBI->Add("m",  tkr_pins->pin[real_pin].unique_module_owner_id);

          JBI->Add("ph", tkr_pins->pin[real_pin].physical.data);
          JBI->Add("af", tkr_pins->pin[real_pin].allocation.data);

          JBI->Add("a",  tkr_pins->pin[real_pin].allocation.allocated);
          JBI->Add("l",  tkr_pins->pin[real_pin].allocation.locked);
          JBI->Add("g",  tkr_pins->pin[real_pin].allocation.grouped);
          JBI->Add("s",  tkr_pins->pin[real_pin].allocation.shared);
          JBI->Add("c",  tkr_pins->pin[real_pin].allocation.conflict);
        JBI->Object_End();
      }

    JBI->Array_End();

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Debug_Template(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Template telemetry.
     *
     * This topic owns template/module selection and the resolved runtime
     * GPIO table. It should not duplicate the detailed pin allocation table;
     * that belongs in pins/gpio and pins/table.
     ********************************************************************/

    JBI->Object_Start("Loading");
      JBI->Add("Module",     tkr_set->runtime.template_loading.status.module);
      JBI->Add("Function",   tkr_set->runtime.template_loading.status.function);
      JBI->Add("Lighting",   tkr_set->runtime.template_loading.status.lighting);
      JBI->Add("NextionMap", tkr_set->runtime.template_loading.status.nextion_hmi_input_map);
      JBI->Add("Rules",      tkr_set->runtime.template_loading.status.rules);
    JBI->Object_End();

    JBI->Object_Start("Module");
      JBI->Add_P(PM_MODULENAME, tkr_pins->AnyModuleName(tkr_set->Settings.module));
      JBI->Add(PM_MODULEID,     tkr_set->Settings.module);
      JBI->Add("Last",          tkr_set->Settings.last_module);
      JBI->Add("Runtime",       tkr_set->runtime.my_module_type);
    JBI->Object_End();

    JBI->Object_Start("UserTemplate");
      JBI->Add("Base",          tkr_set->Settings.user_template.base);
      JBI->Add("Flags",         tkr_set->Settings.user_template.flags);
      JBI->Add_P("BaseName",    tkr_pins->AnyModuleName(tkr_set->Settings.user_template.base));
    JBI->Object_End();

    /********************************************************************
     * Stored user-template GPIO table.
     *
     * This is the template stored in Settings.user_template.hardware.
     * It may differ from the resolved runtime table below if defaults,
     * module loading, overrides, or allocation logic changed it at boot.
     ********************************************************************/
    JBI->Array_Start("StoredGPIO");
      for (uint8_t real_pin = 0; real_pin < ARRAY_SIZE(tkr_set->Settings.user_template.hardware.gp.io); real_pin++)
      {
        JBI->Add(tkr_set->Settings.user_template.hardware.gp.io[real_pin]);
      }
    JBI->Array_End();

    /********************************************************************
     * Runtime resolved GPIO table.
     *
     * Physical GPIO indexed. This reports the selected GPIO function table
     * after module/template resolution. It does not call TemplateGPIOs().
     ********************************************************************/
    JBI->Array_Start("RuntimeGPIO");
      for (uint8_t real_pin = 0; real_pin < ARRAY_SIZE(tkr_set->runtime.my_module.io); real_pin++)
      {
        JBI->Add(tkr_set->runtime.my_module.io[real_pin]);
      }
    JBI->Array_End();

    JBI->Object_Start("RuntimeFlags");
      JBI->Add("Data", tkr_set->runtime.my_module_flag.data);
    JBI->Object_End();

    #ifdef MODULE_TEMPLATE
    JBI->Add("ModuleTemplate", true);
    #else
    JBI->Add("ModuleTemplate", false);
    #endif

    #ifdef FUNCTION_TEMPLATE
    JBI->Add("FunctionTemplate", true);
    #else
    JBI->Add("FunctionTemplate", false);
    #endif

    #ifdef LIGHTING_TEMPLATE
    JBI->Add("LightingTemplate", true);
    #else
    JBI->Add("LightingTemplate", false);
    #endif

    #ifdef DISPLAY_TEMPLATE
    JBI->Add("DisplayTemplate", true);
    #else
    JBI->Add("DisplayTemplate", false);
    #endif

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_TaskerManager(uint8_t json_level, bool json_appending)
{
  JBI->Start();

    /********************************************************************
     * Tasker manager module registry.
     *
     * This reports the runtime tasker modules registered with tkr->pModule.
     * It is not related to the selected hardware/template module.
     ********************************************************************/

    JBI->Object_Start("Summary");
      JBI->Add("Count", tkr->pModule.size());
    JBI->Object_End();

    JBI->Array_Start("Modules");

      char ptrbuf[20] = {0};

      for (auto* mod : tkr->pModule)
      {
        if (!mod) {
          continue;
        }

        snprintf_P(
          ptrbuf,
          sizeof(ptrbuf),
          PSTR("0x%08lX"),
          (unsigned long)(uintptr_t)mod
        );

        JBI->Object_Start();
          JBI->Add("ID",   mod->GetModuleUniqueID());
          JBI->Add("Name", mod->GetModuleName());

          if (json_level >= JSON_LEVEL_DETAILED)
          {
            JBI->Add("Ptr", ptrbuf);
          }
        JBI->Object_End();
      }

    JBI->Array_End();

  return JBI->End();
}


                            uint8_t mTelemetry::ConstructJSON_Debug_Tasker_Interface_Performance(uint8_t json_level, bool json_appending)
                            {
                              JBI->Start();

                              #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
                              // Existing perf block (if you want)
                              JBI->Object_Start("TaskerPerf");
                              for (uint8_t ii = 0; ii < tkr->GetClassCount(); ii++) {
                                JBI->Object_Start_P(tkr->pModule[ii]->GetModuleName());
                                  JBI->Add("max_ms", tkr->debug_module_time[ii].max_time);
                                  JBI->Add("avg_ms", tkr->debug_module_time[ii].avg_time);
                                  JBI->Add("max_task", tkr->debug_module_time[ii].max_function_id);
                                JBI->Object_End();
                              }
                              JBI->Object_End();
                            #endif

                              // New: per-module heap deltas
                              #ifdef ENABLE_DEBUGFEATURE_TASKER__DEBUG_MEMORY_PER_MODULE
                              JBI->Object_Start("TaskerMemDelta8");
                              for (uint8_t ii = 0; ii < tkr->GetClassCount(); ii++) {
                                const auto &s = tkr->memstats[ii];

                                // Optional: suppress zeros to reduce payload
                                if (s.samples == 0) continue;
                                if ((s.free8_sum == 0) && (s.largest8_sum == 0) && (s.free8_min_delta == 0) && (s.largest8_min_delta == 0)) continue;

                                JBI->Object_Start_P(tkr->pModule[ii]->GetModuleName());
                                  // JBI->Add("n",        s.samples);
                                  JBI->Add("free_sum", s.free8_sum);
                                  JBI->Add("lb_sum",   s.largest8_sum);
                                  // JBI->Add("free_min", (int32_t)s.free8_min_delta);
                                  // JBI->Add("lb_min",   (int32_t)s.largest8_min_delta);
                                JBI->Object_End();
                              }
                              JBI->Object_End();
                              #endif
                              ALOG_INF(PSTR("per %s"), JBI->GetBuffer());

                              return JBI->End();
                            }

                            #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES
                            uint8_t mTelemetry::ConstructJSON_Debug__Settings_Storage(uint8_t json_level, bool json_appending)
                            {

                              mqtthandler_debug__settings_storage.tRateSecs = 1;

                              JBI->Start();

                              char buffer2[100];
                              
                              JBI->Add("bootcount", tkr_set->Settings.bootcount);

                              return JBI->End();
                            }
                            #endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE__SEND_DEBUG_MQTT_MESSAGES

#endif // FIRMWARE_MINIMAL2