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
    IPAddress localip = WiFi.localIP();

    JBI->Add_FV(PM_IPADDRESS,         PSTR("\"%d.%d.%d.%d\""), localip[0], localip[1], localip[2], localip[3]);
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
     * This topic intentionally excludes:
     *  - sysopt_* groups: reported under settings/system, settings/network, etc.
     *  - text_pool/device_name_buffer/module_pins/template.hardware: large/debug topics
     *  - live WiFi/MQTT/network state: reported by /network, /mqtt, /health
     *  - legacy ipv4_* fields: superseded by the network module config
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

    JBI->Object_Start("Log");
      JBI->Add("Serial",     tkr_set->Settings.logging.serial_level);
      JBI->Add("Sys",        tkr_set->Settings.logging.sys_level);
      JBI->Add("SysPort",    tkr_set->Settings.logging.sys_port);
      JBI->Add("Web",        tkr_set->Settings.logging.web_level);
      JBI->Add("Telnet",     tkr_set->Settings.logging.telnet_level);
      JBI->Add("MQTT",       tkr_set->Settings.logging.mqtt_level);
      JBI->Add("Display",    tkr_set->Settings.logging.display_serial_level);
      JBI->Add("TimeShort",  tkr_set->Settings.logging.time_isshort);
    JBI->Object_End();

    JBI->Object_Start("Web");
      JBI->Add("Server",     tkr_set->Settings.webserver);
      JBI->Add("Refresh",    tkr_set->Settings.web_refresh);
    JBI->Object_End();

    JBI->Object_Start("Time");
      JBI->Add("TZ",         tkr_set->Settings.timezone);
      JBI->Add("TZMin",      tkr_set->Settings.timezone_minutes);
      JBI->Add("TZ2",        tkr_set->Settings.timezone2);
      JBI->Add("TZ2Min",     tkr_set->Settings.timezone_minutes2);
      JBI->Add("Offset0",    tkr_set->Settings.toffset[0]);
      JBI->Add("Offset1",    tkr_set->Settings.toffset[1]);
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


                            uint8_t mTelemetry::ConstructJSON_Firmware(uint8_t json_level, bool json_appending){ //BuildHealth
                              char buffer[30];
                              JBI->Start();
                              
                                JBI->Add(PM_VERSION_NAME,     tkr_set->runtime.firmware_version.current.name_ctr);
                                JBI->Add(PM_BUILDDATE,       __DATE__);
                                
                                JBI->Add_P(PM_SERIAL,          tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.serial_level), VALUE_IS_PROGMEM);
                                JBI->Add(PM_BOOTCOUNT,       tkr_set->Settings.bootcount);
                                JBI->Add(PM_BOOTCOUNTERRORS, tkr_set->Settings.bootcount_errors_only);
                                JBI->Add(PM_BUILDDATETIME,   tkr_time->GetBuildDateAndTime().c_str());
                                JBI->Add(PM_BUILDTIME,       __TIME__);
                                JBI->Add(PM_VERSION_NUMBER,   (uint32_t)PROJECT_VERSION);
                                JBI->Add(PM_VERSION_NUMBER_NOTYPE,   (uint32_t)PROJECT_VERSION & 0x3FFFFFFF); //suppres 2 MSBs

                                JBI->Array_Start(PM_VERSION_PARTS);
                                  JBI->Add( (uint8_t)FIRMWARE_VERSION_TYPE);
                                  JBI->Add( (uint8_t)FIRMWARE_VERSION_MAJOR);
                                  JBI->Add( (uint8_t)FIRMWARE_VERSION_MINOR);
                                  JBI->Add( (uint8_t)FIRMWARE_VERSION_CORE);
                                  JBI->Add( (uint8_t)FIRMWARE_VERSION_MODULE);
                                JBI->Array_End();

                                #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
                                JBI->Add(PM_ARDUINO_CORE,     ARDUINO_ESP8266_RELEASE); 
                                // JBI->Add(PM_DRIVERS,         "1,2,3,4,5,6,7,8,9,10,12,14,16,17,18,19,20,21,22,24,26,30");
                                // JBI->Add(PM_SENSORS,         "1,2,3,4,5,6,7,8,9,10,12,14,16,17,18,19,20,21,22,24,26,30");
                                JBI->Add_FV(PM_FEATURES,     PSTR("[\"%08X\",\"%08X\",\"%08X\",\"%08X\"]"), 0,0,0,0);
                                #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
                                
                                #ifdef ESP8266
                                JBI->Add(PM_BOOTVERSION,     ESP.getBootVersion());
                                #endif // ESP8266
                                JBI->Add(PM_SDKVERSION,      ESP.getSdkVersion());    

                                JBI->Add(PM_SDKVERSION,      ESP.getSdkVersion());    
                                JBI->Add(PM_FREESKETCHSPACE,      ESP.getFreeSketchSpace());

                                #ifdef DEVICENAME_BUILD_ENVIRONMENT
                                JBI->Add("BuildEnvironment", DEVICENAME_BUILD_ENVIRONMENT);
                                #endif

                                
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
                                JBI->Add(PM_WATCHDOG, "Enabled");
                                // JBI->Add("WatchDogTimeout", D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS);
                                #else
                                JBI->Add(PM_WATCHDOG, "Disabled");
                                #endif

                              return JBI->End();
                            }


                            uint8_t mTelemetry::ConstructJSON_Log(uint8_t json_level, bool json_appending){ 
                              char buffer[30];
                              JBI->Start();
                                JBI->Object_Start(PM_LOGLEVELS);
                                  JBI->Add_P(PM_SERIAL, tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.serial_level), VALUE_IS_PROGMEM);
                                  JBI->Add_P(PM_SYSTEM, tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.sys_level), VALUE_IS_PROGMEM);
                                  JBI->Add_P(PM_WEB,    tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.web_level), VALUE_IS_PROGMEM);
                                  JBI->Add_P(PM_TELNET, tkr_log->GetLogLevelNamebyID(tkr_set->Settings.logging.telnet_level), VALUE_IS_PROGMEM);
                                JBI->Object_End();
                              return JBI->End();
                            }


                            uint8_t mTelemetry::ConstructJSON_Memory(uint8_t json_level, bool json_appending)
                            {
                              JBI->Start();

                              // ---- Mostly-static (can be cached at boot if you want) ----
                              // JBI->Add(PM_PROGRAMSIZE, ESP.getSketchSize() / 1024);

                            #ifdef ESP8266
                              JBI->Add(PM_FREEMEMORY,       ESP.getFreeSketchSpace() / 1024);
                              JBI->Add(PM_HEAPSIZE,         ESP.getFreeHeap() / 1024);
                              JBI->Add(PM_PROGRAMFLASHSIZE, ESP.getFlashChipSize() / 1024);
                              JBI->Add(PM_FLASHSIZE,        ESP.getFlashChipRealSize() / 1024);
                              JBI->Add(PM_FLASHCHIPID,      ESP.getFlashChipId());
                            #else
                              // ESP32
                              // JBI->Add(PM_FREEMEMORY,       ESP.getFreeSketchSpace() / 1024);
                              JBI->Add(PM_HEAPSIZE,         ESP.getFreeHeap() / 1024);

                              // // ESP32: "chip flash size" is OK; no "real size" concept here in Arduino layer
                              // JBI->Add(PM_PROGRAMFLASHSIZE, ESP.getFlashChipSize() / 1024);

                              // // This isn't "flash chip id". It's the silicon MAC / eFuse ID.
                              // const uint64_t efuse_mac = ESP.getEfuseMac();
                              // JBI->Add(PM_FLASHCHIPID, efuse_mac);

                              // ---- Heap health (ESP32) ----
                              JBI->Add(PM_HEAP_FREE_8BIT,    heap_caps_get_free_size(MALLOC_CAP_8BIT) / 1024);
                              JBI->Add(PM_HEAP_LARGEST_8BIT, heap_caps_get_largest_free_block(MALLOC_CAP_8BIT) / 1024);
                              JBI->Add(PM_HEAP_MIN_8BIT,     heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT) / 1024);

                              // ---- Optional: PSRAM ----
                              // JBI->Add(PM_PSRAM_FREE, ESP.getFreePsram() / 1024);
                              // JBI->Add(PM_PSRAM_SIZE, ESP.getPsramSize() / 1024);
                            #endif

                              // JBI->Add(PM_FLASHMODE, (uint8_t)ESP.getFlashChipMode());
                              return JBI->End();
                            }



                            uint8_t mTelemetry::ConstructJSON_Network(uint8_t json_level, bool json_appending)
                            {
                              
                              IPAddress localip   = WiFi.localIP();
                              IPAddress staticip  = IPAddress(tkr_wifi->config.station.ipv4.ip[0]);
                              IPAddress gatewayip = IPAddress(tkr_wifi->config.station.ipv4.gw[0]);
                              IPAddress subnetip  = IPAddress(tkr_wifi->config.station.ipv4.sn[0]);
                              IPAddress dnsip     = IPAddress(tkr_wifi->config.station.ipv4.dns1[0]);

                              JBI->Start();
                                JBI->Add_FV(PM_IPADDRESS,PSTR("\"%d.%d.%d.%d\""),localip[0],localip[1],localip[2],localip[3]);
                                JBI->Add(PM_SSID, WiFi.SSID().c_str());
                                JBI->Add(PM_SSID_NUMBERED, tkr_wifi->config.station.active_profile); // Used to debug switching in grafana
                                JBI->Add(PM_RSSI, WiFi.RSSI());
                                #ifdef ESP32
                                JBI->Add("TXPower_dBm", tkr_wifi->WiFiPower_To_dBm(WiFi.getTxPower()) );
                                #endif
                                // JBI->Add(PM_CONNECTCOUNT, wifi_reconnects_counter);
                                JBI->Add(PM_HOSTNAME, tkr_set->runtime.my_hostname);
                                JBI->Add_P(PM_TELNET_PORT, TELNET_PORT);
                                JBI->Add_FV(PM_STATIC_IPADDRESS,PSTR("\"%d.%d.%d.%d\""),staticip[0],staticip[1],staticip[2],staticip[3]);
                                JBI->Add_FV(PM_GATEWAY,PSTR("\"%d.%d.%d.%d\""),gatewayip[0],gatewayip[1],gatewayip[2],gatewayip[3]);
                                JBI->Add_FV(PM_SUBNETMASK,PSTR("\"%d.%d.%d.%d\""),subnetip[0],subnetip[1],subnetip[2],subnetip[3]);
                                JBI->Add_FV(PM_DNSSERVER,PSTR("\"%d.%d.%d.%d\""),dnsip[0],dnsip[1],dnsip[2],dnsip[3]);
                                JBI->Add(PM_BSSID, WiFi.BSSIDstr().c_str());
                                JBI->Add(PM_MAC, WiFi.macAddress().c_str());
                                JBI->Add(PM_WEBSERVER_ENABLED, tkr_set->Settings.webserver);
                                JBI->Add(PM_WIFICONFIG_STATE, tkr_wifi->config.station.enabled);

                                JBI->Array_Start(PM_AP_LIST);
                                for(int i=0;i<WIFI_MAXIMUM_CONNECTIONS;i++)
                                {
                                if(tkr_wifi->config.station.profiles[i].ssid[0] != 0)
                                {
                                  JBI->Add(tkr_wifi->config.station.profiles[i].ssid); 
                                }
                                }
                                JBI->Array_End();

                              return JBI->End();

                            }


                            uint8_t mTelemetry::ConstructJSON_MQTT(uint8_t json_level, bool json_appending){

                              JBI->Start();


                                /**
                                * @brief Show wrapper stuff
                                * 
                                */

                                // JBI->Add("RetrySecs", tkr_mqtt->dt.connection[0].retry);

                                JBI->Object_Start(PM_REFRESH_RATES);
                                  JBI->Add(PM_MQTT_REFRESH_RATE_IFCHANGED, tkr_mqtt->dt.ifchanged_secs);
                                  JBI->Add(PM_MQTT_REFRESH_RATE_TELEPERIOD, tkr_mqtt->dt.teleperiod_secs);
                                JBI->Object_End();
                                
                                JBI->Add(PM_MQTT_ENABLE_RESTART,   (uint8_t)0);

                            // {
                            //   "ClientName":"tg_lighting_70-08:D1:F9:CA:0B:74",
                            //   "RetrySecs":10,
                            //   "RefreshRates":{
                            //     "IfChanged":10,
                            //     "Teleperiod":60},
                            //     "MQTT Enable Restart":0,
                            //     "Instance":[
                            //       [
                            //         "broker_url":"192.168.1.70",
                            //         "port":1883,"connect_count":0,"retry_counter":0,"downtime_counter":0,"connected":1,"allowed":0,"mqtt_tls":0,,"tSaved_LastOutGoingTopic":18664,"flag_start_reconnect":0,"cConnectionAttempts":0,"host_server_type":0]],"Connection":[["status":1,"client_name":"tg_lighting_70-08:D1:F9:CA:0B:74","prefixtopic":"tg_lighting_70"]]}



                                #ifdef ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY
                                JBI->Object_Start("Interface_Priority");
                                  // JBI->Add(D_MODULE_ENERGY_INTERFACE_CTR, tkr_mqtt->dt.connection[0].interface_reporting_priority.energy);
                                JBI->Object_End();
                                #endif // ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY


                                /**
                                * @brief Show each instance info
                                * 
                                **/
                                // #ifdef USE_MODULE_NETWORK_MQTT
                                JBI->Array_Start("Connection");
                                for(auto& con:tkr_mqtt->brokers)
                                {      
                                  JBI->Object_Start();
                                    JBI->Add("host_address", con->host_address);
                                    JBI->Add("port", con->port);

                                //     JBI->Add("connect_count", con->connect_count);
                                //     JBI->Add("retry_counter", con->retry_counter);
                                //     JBI->Add("downtime_counter", con->downtime_counter);
                                //     // JBI->Add("initial_connection_state", con->initial_connection_state);
                                //     JBI->Add("connected", con->connected);
                                //     JBI->Add("allowed", con->allowed);
                                //     JBI->Add("mqtt_tls", con->mqtt_tls);

                                //     JBI->Add("mqtt_client_type", con->client_type);

                                //     JBI->Add("tSaved_LastOutGoingTopic", con->tSaved_LastOutGoingTopic);

                                //     JBI->Add("flag_start_reconnect", con->flag_start_reconnect);
                                //     JBI->Add("cConnectionAttempts", con->cConnectionAttempts);

                                //     JBI->Add("host_server_type", con->host_server_type);

                                //     #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
                                //     JBI->Add("payload_publish_sent", con->debug_stats.payload_publish_sent);
                                //     JBI->Add("payload_publish_missed", con->debug_stats.payload_publish_missed);
                                //     JBI->Add("payload_publish_success_percentage", con->debug_stats.payload_publish_success_percentage);
                                //     #endif
                                  JBI->Object_End();

                                }
                                JBI->Array_End();

                                JBI->Array_Start("Manager");
                                for(auto& con:tkr_mqtt->brokers)
                                {
                                  JBI->Object_Start();
                                    JBI->Add("status", con->status);
                                    JBI->Add("host_address", con->host_address);
                                    JBI->Add("port", con->port);
                                    JBI->Add("client", con->client_name);
                                    JBI->Add("user", con->user);
                                    JBI->Add("pwd", con->password);
                                    JBI->Add("retry", con->retry);
                                    JBI->Add("prefixtopic", con->prefix_topic);
                                  JBI->Object_End();
                                }
                                JBI->Array_End();
                                
                              return JBI->End();

                            }


                            uint8_t mTelemetry::ConstructJSON_Time(uint8_t json_level, bool json_appending){ 

                              char buffer[100];
                                  
                              JBI->Start();
                                JBI->Add(PM_UTC_TIME,   tkr_time->GetDateAndTime(DT_UTC).c_str() );
                                JBI->Add(PM_LOCAL_TIME, tkr_time->GetDateAndTime(DT_LOCAL).c_str() );
                                JBI->Add(PM_STARTDST,   tkr_time->GetDateAndTime(DT_DST).c_str() );
                                JBI->Add(PM_ENDDST,     tkr_time->GetDateAndTime(DT_STD).c_str() );
                                JBI->Add(PM_TIMEZONE,   tkr_time->GetDateAndTime(DT_TIMEZONE).c_str() );
                                JBI->Add(PM_SUNRISE,    tkr_time->GetDateAndTime(DT_SUNRISE).c_str() );
                                JBI->Add(PM_SUNSET,     tkr_time->GetDateAndTime(DT_SUNSET).c_str() );
                              return JBI->End();

                            }


                            /**
                            * needs refactored into something else, perhaps everything moved into better named topics
                            * 
                            * 
                            * */
                            uint8_t mTelemetry::ConstructJSON_Debug_Devices(uint8_t json_level, bool json_appending)
                            { 

                              JBI->Start();

                              /**
                              * @brief DeviceNameBuffer, should become a debug topic
                              * 
                              */
                                JBI->Add(PM_MODULENAME,          tkr_set->Settings.module);
                                // If debug mode, show entire message
                                // JBI->Array_AddArray(PM_DEVICEID, tkr_set->Settings.device_name_buffer.device_id, ARRAY_SIZE(tkr_set->Settings.device_name_buffer.device_id));
                                // JBI->Array_AddArray(PM_CLASSID,  tkr_set->Settings.device_name_buffer.class_id,  ARRAY_SIZE(tkr_set->Settings.device_name_buffer.class_id));
                                // else, only show ysed
                                JBI->Add("DLI->GetLengthIndexUsed()", DLI->GetLengthIndexUsed());
                                JBI->Array_AddArray(PM_DEVICEID, tkr_set->Settings.device_name_buffer.device_id, DLI->GetLengthIndexUsed());
                                JBI->Array_AddArray(PM_CLASSID,  tkr_set->Settings.device_name_buffer.class_id,  DLI->GetLengthIndexUsed());  
                                JBI->Add(PM_BUFFER,        tkr_set->Settings.device_name_buffer.name_buffer);
                                JBI->Add(PM_BUFLEN,        strlen(tkr_set->Settings.device_name_buffer.name_buffer));

                                uint8_t count = 0;
                                if(strlen(tkr_set->Settings.device_name_buffer.name_buffer)){ //if anything in buffer
                                  count = DLI->CountCharInCtr(tkr_set->Settings.device_name_buffer.name_buffer,'|'); // first | indicates index from 0
                                }
                                JBI->Add("ItemCount", count);

                                
                                if(tkr_pins->PinUsed(GPIO_I2C_SCL)&&tkr_pins->PinUsed(GPIO_I2C_SDA))
                                {

                                  #ifdef ESP32
                                  JBI->Add("I2C_BusSpeed", tkr_i2c->wire->getClock());
                                  #endif

                                  char mqtt_data[300];
                                  tkr_i2c->I2cScan(mqtt_data, sizeof(mqtt_data));
                                  // Serial.println(mqtt_data);

                                  //need to escape option to function above
                                  // JBI->Add("I2C_Scan",          mqtt_data);

                                  JBI->Append(",\"I2C_Scan\":");
                                  JBI->Append(mqtt_data);

                                }

                                // BufferWriterI->Append(",\"I2C_Scan\":");

                                #ifdef USE_FUNCTION_TEMPLATE
                                //JBI->Add("Function Template",   FUNCTION_TEMPLATE);
                                #endif

                              return JBI->End();

                            }


                            // uint8_t mTelemetry::ConstructJSON_Reboot(uint8_t json_level, bool json_appending){ // 

                            //   JBI->Start();
                              
                            //   JBI->Add(PM_DEVICE, tkr_set->Settings.system_name.device);
                            //   JBI->Add(PM_DEVICEFRIENDLYNAME, tkr_set->Settings.system_name.friendly);
                            //   JBI->Add_FV(PM_DATETIME, PSTR("\"%02d-%02d-%02d %02d:%02d:%02d\""),
                            //                       tkr_time->RtcTime.day_of_month, tkr_time->RtcTime.month, tkr_time->RtcTime.year,
                            //                       tkr_time->RtcTime.hour, tkr_time->RtcTime.minute, tkr_time->RtcTime.second
                            //                     );
                            //   JBI->Object_Start(PM_COUNTER);
                            //     JBI->Add("All", (uint8_t)0);
                            //     // JBI->Add("WDT", (uint8_t)0);
                            //   JBI->Object_End();

                            //   // if (tkr_sup->CrashFlag()) {
                                
                            //   JBI->Object_Start(PM_CRASHDUMP);
                            //     tkr_sup->WriteBuffer_P(PSTR(","));
                            //     tkr_sup->CrashDump_AddJson();
                            //   JBI->Object_End();

                            //   // } else {
                            //   //   char buffer[30];
                            //   //   JBI->Add("Reason", tkr_sup->GetResetReason(buffer, sizeof(buffer)));
                            //   // }

                            //   return JBI->End();

                            // }


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


                            /****
                            * 
                            * 
                            * 
                            * Debug packets that are not part of the main telemetry and only used for debugging/developing the device
                            * 
                            * 
                            * 
                            * ****/

                            uint8_t mTelemetry::ConstructJSON_Debug_Minimal(uint8_t json_level, bool json_appending)
                            {

                              char buffer[30];
                              
                              JBI->Start();
                                JBI->Add(PM_UPTIME,         tkr_time->GetUptime(buffer, sizeof(buffer)));
                                JBI->Add(PM_UPSECONDS,      tkr_time->UpTime());
                                JBI->Add(PM_SLEEP,          tkr_sup->loop_delay_temp);
                                JBI->Add(PM_LOOPSSEC,       tkr_sup->activity.cycles_per_sec);
                                JBI->Add(PM_LOOPRATIO,      tkr_sup->this_cycle_ratio);
                                JBI->Add(PM_FREEHEAP,       ESP.getFreeHeap());
                                JBI->Add(PM_VERSION_NAME,    tkr_set->runtime.firmware_version.current.name_ctr);
                                IPAddress localip = WiFi.localIP();
                                JBI->Add_FV(PM_IPADDRESS,   PSTR("\"%d.%d.%d.%d\""), localip[0],localip[1],localip[2],localip[3]);
                                JBI->Add(PM_BOOTCOUNT,      tkr_set->Settings.bootcount);
                              return JBI->End();

                            }

                            uint8_t mTelemetry::ConstructJSON_Debug_Pins_GPIO(uint8_t json_level, bool json_appending)
                            {
                              char key_buffer[64];
                              char name_buffer[64];

                              JBI->Start();

                              /*******************************************************************************************\
                              * GPIO object
                              *
                              * Key   = decoded GPIO function name, e.g. "Modem TX1"
                              * Value = real physical pin number
                              \*******************************************************************************************/

                              JBI->Object_Start(PM_GPIO);

                              for(uint16_t real_pin = 0; real_pin < MAX_GPIO_PIN; real_pin++)
                              {
                                const uint16_t packed_gpio = tkr_pins->pin[real_pin].gpio_function;

                                if((packed_gpio == GPIO_NONE) || (packed_gpio == GPIO_USER))
                                {
                                  continue;
                                }

                                tkr_pins->GetGPIOFunctionNamebyID(packed_gpio, name_buffer, sizeof(name_buffer));

                                JBI->Add(name_buffer, real_pin);

                                // ALOG_INF(
                                //   PSTR("DBG_PIN: real_pin=%u packed=0x%04X dec=%u base=%u idx=%u owner=%u alloc=0x%04X phys=0x%04X name=\"%s\""),
                                //   real_pin,
                                //   packed_gpio,
                                //   packed_gpio,
                                //   UGPIO(packed_gpio),
                                //   packed_gpio & GPIO_INDEX_MASK,
                                //   tkr_pins->pin[real_pin].unique_module_owner_id,
                                //   tkr_pins->pin[real_pin].allocation.data,
                                //   tkr_pins->pin[real_pin].physical.data,
                                //   name_buffer
                                // );
                              }

                              JBI->Object_End();

                              return JBI->End();
                            }


                            uint8_t mTelemetry::ConstructJSON_Debug_Pins_Table(uint8_t json_level, bool json_appending)
                            {
                              
                              char name_buffer[64];

                              JBI->Start();

                              /*******************************************************************************************\
                              * Detailed pin table
                              *
                              * real_pin      = physical GPIO number
                              * raw           = packed GPIO function
                              * base          = unpacked GPIO function base ID
                              * idx           = packed function instance index
                              * name          = decoded GPIO function name
                              * owner         = current module/core owner ID
                              * allocation    = allocation flags bitfield
                              * physical      = physical capability flags bitfield
                              * allocated     = true only after AllocatePin() accepts ownership
                              \*******************************************************************************************/

                              JBI->Array_Start("pin_table");

                              for(uint16_t real_pin = 0; real_pin < MAX_GPIO_PIN; real_pin++)
                              {
                                const uint16_t packed_gpio = tkr_pins->pin[real_pin].gpio_function;

                                
                                const uint16_t base_id2 = UGPIO(packed_gpio);
                                const uint8_t  func_i2  = packed_gpio & GPIO_INDEX_MASK;
                                ALOG_DBM(PSTR("gpio %d %d"), real_pin, base_id2, func_i2);


                                if((packed_gpio == GPIO_NONE) || (packed_gpio == GPIO_USER))
                                {
                                  continue;
                                }

                                const uint16_t base_id = UGPIO(packed_gpio);
                                const uint8_t  func_i  = packed_gpio & GPIO_INDEX_MASK;

                                tkr_pins->GetGPIOFunctionNamebyID(packed_gpio, name_buffer, sizeof(name_buffer));

                                JBI->Object_Start();
                                  JBI->Add("pin",   real_pin);
                                  JBI->Add("name",  name_buffer);
                                  JBI->Add("raw",   packed_gpio);
                                  JBI->Add("base",  base_id);
                                  JBI->Add("i",     func_i);

                                  JBI->Add("m",     tkr_pins->pin[real_pin].unique_module_owner_id);

                                  JBI->Add("p",     tkr_pins->pin[real_pin].physical.data);

                                  JBI->Add("d",     tkr_pins->pin[real_pin].allocation.data);
                                  JBI->Add("a",     tkr_pins->pin[real_pin].allocation.allocated);
                                  JBI->Add("l",     tkr_pins->pin[real_pin].allocation.locked);
                                  JBI->Add("g",     tkr_pins->pin[real_pin].allocation.grouped);
                                  JBI->Add("s",     tkr_pins->pin[real_pin].allocation.shared);
                                  JBI->Add("c",     tkr_pins->pin[real_pin].allocation.conflict);
                                JBI->Object_End();
                              }

                              JBI->Array_End();


                              return JBI->End();
                            }







                            uint8_t mTelemetry::ConstructJSON_Debug_Template(uint8_t json_level, bool json_appending)
                            {
                              JBI->Start();

                                JBI->Object_Start("TemplateLoading");
                                  JBI->Add("Module",             tkr_set->runtime.template_loading.status.module);
                                  JBI->Add("Function",           tkr_set->runtime.template_loading.status.function);
                                  JBI->Add("Lighting",           tkr_set->runtime.template_loading.status.lighting);
                                  JBI->Add("NextionHMIInputMap", tkr_set->runtime.template_loading.status.nextion_hmi_input_map);
                                  JBI->Add("Rules",              tkr_set->runtime.template_loading.status.rules);
                                JBI->Object_End();

                                JBI->Add_P(PM_MODULENAME, tkr_pins->AnyModuleName(tkr_set->Settings.module));
                                JBI->Add(PM_MODULEID,     tkr_set->Settings.module);
                                JBI->Add("MyModuleType",  tkr_set->runtime.my_module_type);

                                /*******************************************************************************************\
                                * Resolved runtime GPIO table
                                *
                                * Physical GPIO indexed.
                                * Does not call TemplateGPIOs().
                                * This reports the currently selected GPIO function table after GpioInit().
                                \*******************************************************************************************/

                                JBI->Array_Start_P(PM_GPIO);
                                  for(uint8_t real_pin = 0; real_pin < ARRAY_SIZE(tkr_set->runtime.my_module.io); real_pin++)
                                  {
                                    JBI->Add(tkr_set->runtime.my_module.io[real_pin]);
                                  }
                                JBI->Array_End();

                              return JBI->End();
                            }


                            // uint8_t mTelemetry::ConstructJSON_Debug_ModuleInterface(uint8_t json_level, bool json_appending){ //BuildHealth

                            //   JBI->Start();
                              
                            //   // #ifdef DEBUG_EXECUTION_TIME
                            //   //   char buffer[50];
                            //   //   JBI->Object_Start(tkr->GetModuleFriendlyName(tkr->module_settings.list[ii], buffer));
                            //   //     JBI->Array_AddArray("average", tkr->module_settings.execution_time_average_ms, sizeof(tkr->module_settings.execution_time_average_ms));
                            //   //     JBI->Array_AddArray("max",     tkr->module_settings.execution_time_max_ms,     sizeof(tkr->module_settings.execution_time_max_ms));
                            //   //   JBI->Object_End();
                            //   // #endif

                            //   // JBI->Object_Start("ModuleSize");

                              
                            //   // // for(uint8_t i=0;i<tkr->module_settings.count;i++){
                            //   // //   JBI->Add_P(tkr->GetModuleFriendlyName(tkr->module_settings.list[i]),tkr->GetClassSizeByID(tkr->module_settings.list[i]));
                            //   // //   // if(tkr->GetClassSizeByID(i)>10000){
                            //   // //   //   JBI->Add("bad",i);
                            //   // //   // }
                            //   // // }

                            //   // /**
                            //   //  * @brief Add array of all unique id's (in a json array, this will just replace... so maybe use name+id? or rather ID_NAME so it will be easier to spot numbers the same)
                            //   //  * I could also run a "append" id but check its not in it already (easier with vector?)
                            //   //  * 
                            //   //  */
                            //   // char buffer[100] = {0};

                            //   // JBI->Array_Start("ModuleIDs");
                            //   // for(auto& mod: tkr->pModule)
                            //   // {
                            //   //   snprintf_P(buffer, sizeof(buffer), PSTR("%04d_%S"), mod->GetModuleUniqueID(), mod->GetModuleName()  );
                            //   //   JBI->Add(buffer);
                            //   // }
                            //   // JBI->Array_End();

                            //   // JBI->Object_End();


                            //   return JBI->End();
                            // }

                            // uint8_t mTelemetry::ConstructJSON_Debug_ModuleInterface(uint8_t json_level, bool json_appending)
                            // {
                            //   JBI->Start();

                            //   // ---- Config ----
                            //   constexpr uint16_t MAX_MODS = 96;   // hard cap for safety
                            //   constexpr uint16_t MAX_DUPS = 32;   // max duplicate IDs reported

                            //   // ---- Scratch storage (embedded-friendly) ----
                            //   uint16_t ids[MAX_MODS];
                            //   uint16_t counts[MAX_MODS];
                            //   uint16_t unique_count = 0;

                            //   // Track logger specifically
                            //   uint16_t logger_count = 0;

                            //   // ---- Modules list ----
                            //   JBI->Array_Start("modules");

                            //   uint16_t mod_i = 0;
                            //   for (auto &mod : tkr->pModule)
                            //   {
                            //     if (!mod) continue;

                            //     // Safety stop
                            //     if (mod_i >= MAX_MODS) break;
                            //     mod_i++;

                            //     const uint16_t id = (uint16_t)mod->GetModuleUniqueID();

                            //     // Update unique-id counts (O(N^2) but N is small; avoids std::map)
                            //     bool found = false;
                            //     for (uint16_t i = 0; i < unique_count; i++) {
                            //       if (ids[i] == id) { counts[i]++; found = true; break; }
                            //     }
                            //     if (!found && unique_count < MAX_MODS) {
                            //       ids[unique_count] = id;
                            //       counts[unique_count] = 1;
                            //       unique_count++;
                            //     }

                            //     if (id == (uint16_t)D_UNIQUE_MODULE_CORE_LOGGING_ID) {
                            //       logger_count++;
                            //     }

                            //     // Emit module object
                            //     // Note: pointer printed as string so JSON stays valid on all platforms.
                            //     char namebuf[64] = {0};
                            //     char ptrbuf[20]  = {0};

                            //     snprintf_P(namebuf, sizeof(namebuf), PSTR("%S"), mod->GetModuleName());
                            //     snprintf_P(ptrbuf,  sizeof(ptrbuf),  PSTR("0x%08lX"), (unsigned long)(uintptr_t)mod);

                            //     JBI->Object_Start();
                            //       JBI->Add("id", id);
                            //       JBI->Add("name", namebuf);
                            //       JBI->Add("ptr", ptrbuf);

                            //       // Optional (only if you have these methods; comment out if not present)
                            //       // JBI->Add("class_bytes", tkr->GetClassSizeByID(id));
                            //       // JBI->Add("enabled", mod->IsEnabled());

                            //     JBI->Object_End();
                            //   }

                            //   JBI->Array_End(); // modules

                            //   // ---- Duplicate report ----
                            //   JBI->Array_Start("duplicates");

                            //   uint16_t dup_reported = 0;
                            //   for (uint16_t i = 0; i < unique_count; i++)
                            //   {
                            //     if (counts[i] > 1)
                            //     {
                            //       if (dup_reported++ >= MAX_DUPS) break;

                            //       JBI->Object_Start();
                            //         JBI->Add("id", ids[i]);
                            //         JBI->Add("count", counts[i]);

                            //         // Provide an ID_NAME string for quick visual scan
                            //         // (First matching module name found)
                            //         const char* firstName = nullptr;
                            //         for (auto &mod : tkr->pModule) {
                            //           if (!mod) continue;
                            //           if ((uint16_t)mod->GetModuleUniqueID() == ids[i]) {
                            //             firstName = (const char*)mod->GetModuleName(); // PROGMEM string pointer
                            //             break;
                            //           }
                            //         }
                            //         if (firstName) {
                            //           char idname[80] = {0};
                            //           snprintf_P(idname, sizeof(idname), PSTR("%04u_%S"), (unsigned)ids[i], (const __FlashStringHelper*)firstName);
                            //           JBI->Add("id_name", idname);
                            //         }
                            //       JBI->Object_End();
                            //     }
                            //   }

                            //   JBI->Array_End(); // duplicates

                            //   // ---- Logger report (explicit) ----
                            //   JBI->Object_Start("logger");
                            //     JBI->Add("id", (uint16_t)D_UNIQUE_MODULE_CORE_LOGGING_ID);
                            //     JBI->Add("found", (logger_count > 0));
                            //     JBI->Add("count", logger_count);

                            //     // Add pointer list for logger instances (helps prove duplicates)
                            //     JBI->Array_Start("ptrs");
                            //       for (auto &mod : tkr->pModule) {
                            //         if (!mod) continue;
                            //         if ((uint16_t)mod->GetModuleUniqueID() == (uint16_t)D_UNIQUE_MODULE_CORE_LOGGING_ID) {
                            //           char ptrbuf[20] = {0};
                            //           snprintf_P(ptrbuf, sizeof(ptrbuf), PSTR("0x%08lX"), (unsigned long)(uintptr_t)mod);
                            //           JBI->Add(ptrbuf);
                            //         }
                            //       }
                            //     JBI->Array_End();
                            //   JBI->Object_End();

                            //   // ---- Summary ----
                            //   JBI->Object_Start("summary");
                            //     JBI->Add("modules_scanned", mod_i);
                            //     JBI->Add("unique_ids", unique_count);
                            //     JBI->Add("duplicate_ids", dup_reported);
                            //   JBI->Object_End();

                            //   return JBI->End();
                            // }



                            uint8_t mTelemetry::ConstructJSON_Debug_ModuleInterface(uint8_t json_level, bool json_appending)
                            {
                              JBI->Start();

                              JBI->Array_Start("unique_ids");
                              for (auto* mod : tkr->pModule) {
                                if (!mod) continue;
                                JBI->Add(mod->GetModuleUniqueID());
                              }
                              JBI->Array_End();

                              JBI->Object_Start("id_name_list");
                              char id_key[8];
                              for (auto* mod : tkr->pModule) {
                                if (!mod) continue;
                                snprintf_P(id_key, sizeof(id_key), PSTR("%u"), (unsigned)mod->GetModuleUniqueID());
                                JBI->Add(id_key, mod->GetModuleName());
                              }
                              JBI->Object_End();


                              JBI->Array_Start("module_ptr");
                              char ptrbuf[20]  = {0};
                              for (auto* mod : tkr->pModule) {
                                if (!mod) continue;
                                snprintf_P(ptrbuf,  sizeof(ptrbuf),  PSTR("0x%08lX"), (unsigned long)(uintptr_t)mod);
                                JBI->Add(ptrbuf);
                              }
                              JBI->Array_End();
                              
                              return JBI->End();
                            }


                            /**
                            * mSettings::SystemSettings_DefaultBody_Sensors
                            * Debugging of all values listed inside the settings struct, it shall all be outputted here, mqtt size permitted
                            * */
                            uint8_t mTelemetry::ConstructJSON_Debug_System_Stored_Settings(uint8_t json_level, bool json_appending)
                            { //BuildHealth
                              char buffer[50];
                              JBI->Start();

                              JBI->Object_Start("Header");
                                JBI->Add("cfg_holder", tkr_set->Settings.cfg_holder);
                                JBI->Add("cfg_size", tkr_set->Settings.cfg_size);
                                JBI->Add("save_flag", tkr_set->Settings.save_flag);
                                JBI->Add("version", tkr_set->Settings.version);
                                JBI->Add("bootcount", tkr_set->Settings.bootcount);
                              JBI->Object_End();


                              // JBI->Object_Start("Animations");
                              // //   JBI->Array_AddArray("controller", 
                              // //   &tkr_set->Settings.animation_settings.xmas_controller_params, 
                              // //   sizeof(tkr_set->Settings.animation_settings.xmas_controller_params));
                              // // JBI->Object_End();
                              // // JBI->Object_Start("Animations1");
                              //   // JBI->Array_Start("controller"); 
                              //   // &tkr_set->Settings.animation_settings.xmas_controller_params, 
                              //   // sizeof(tkr_set->Settings.animation_settings.xmas_controller_params));

                              //   // for(int i=0;i<10;i++)
                              //   // {
                              //   //   JBI->Add(tkr_set->Settings.animation_settings.xmas_controller_params[i]);
                              //   // }

                              //   // JBI->Array_End();
                              // JBI->Object_End();

                              /**
                              * Read stored values into temp struct, not use local variables
                              * */

                            //   struct SYSCFGH {
                            //   uint16_t      cfg_holder;                // 000 v6 header
                            //   uint16_t      cfg_size;                  // 002
                            //   unsigned long save_flag;                 // 004
                            //   unsigned long version;                   // 008
                            //   uint16_t      bootcount;                 // 00C
                            //   uint16_t      cfg_crc;                   // 00E
                            //   } _SettingsH;

                            //     extern "C" uint32_t _FS_end;
                            // const uint32_t SPIFFS_END = ((uint32_t)&_FS_end - 0x40200000) / SPI_FLASH_SEC_SIZE;
                            // const uint32_t SETTINGS_LOCATION = SPIFFS_END;  // No need for SPIFFS as it uses EEPROM area
                            // uint32_t flash_location = SETTINGS_LOCATION;
                            // // Just read the header
                            //  ESP.flashRead(flash_location * SPI_FLASH_SEC_SIZE, (uint32*)&_SettingsH, sizeof(_SettingsH));
                                




                                // // JBI->Add("ALTITUDE_ABOVE_SEALEVEL",       (float)ALTITUDE_ABOVE_SEALEVEL);
                                // JBI->Add("altitude",     tkr_set->Settings.sensors.altitude);
                                // // JBI->Add("LATITUDE",                      (float)LATITUDE);
                                // JBI->Add("latitude",     tkr_set->Settings.sensors.latitude);
                                // // JBI->Add("LONGITUDE",       (float)LONGITUDE);
                                // JBI->Add("longitude",                     tkr_set->Settings.sensors.longitude);

                              return JBI->End();
                            }


                            // #ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
                            // uint8_t mTelemetry::ConstructJSON_Debug_Tasker_Interface_Performance(uint8_t json_level, bool json_appending)
                            // {
                              
                            //     // //test devices
                            //     // JBI->Object_Start("Test");
                            //     //   JBI->Add("activity.loop_counter", tkr_sup->activity.loop_counter);
                            //     // //   JBI->Add("sleep", tkr_set->sleep);
                            //     // //   JBI->Add("loop_runtime_millis", tkr_sup->loop_runtime_millis);
                            //     // //   JBI->Add("loops_per_second", tkr_sup->loops_per_second);
                            //     // //   JBI->Add("this_cycle_ratio", tkr_sup->this_cycle_ratio);
                            //     // //   JBI->Add("loop_load_avg", tkr_set->loop_load_avg);
                            //     // //   JBI->Add("enable_sleep", tkr_set->Settings.enable_sleep);
                            //     // JBI->Object_End();
                            //     // // test end

                            //   JBI->Start();

                            //   char buffer2[100];
                              
                            //   // for(int ii=0;ii<tkr->GetClassCount();ii++)
                            //   // {
                            //   //   JBI->Level_Start_P(tkr->pModule[ii]->GetModuleName());

                            //   //     JBI->Add("max_time", tkr->debug_module_time[ii].max_time);
                            //   //     JBI->Add("avg_time", tkr->debug_module_time[ii].avg_time);
                            //   //     JBI->Add("max_function_id", tkr->debug_module_time[ii].max_function_id);

                            //   //   JBI->Object_End();




                            //   // }







                            //   //   JBI->Add_P(tkr->GetModuleFriendlyName(tkr->module_settings.list[i]),tkr->GetClassSizeByID(tkr->module_settings.list[i]));
                            //   //   // if(tkr->GetClassSizeByID(i)>10000){
                            //   //   //   JBI->Add("bad",i);
                            //   //   // }



                            //   // }

                            //   // /**
                            //   //  * @brief Add array of all unique id's (in a json array, this will just replace... so maybe use name+id? or rather ID_NAME so it will be easier to spot numbers the same)
                            //   //  * I could also run a "append" id but check its not in it already (easier with vector?)
                            //   //  * 
                            //   //  */
                            //   // char buffer[100];



                            //   // JBI->Array_Start("ModuleIDs");
                            //   // for(int ii=0;ii<tkr->GetClassCount();ii++)
                            //   // {
                            //   //   snprintf_P(buffer, sizeof(buffer), PSTR("%04d_%S"), tkr->pModule[ii]->GetModuleUniqueID(), tkr->pModule[ii]->GetModuleFriendlyName()  );

                            //   //   JBI->Add(buffer);
                            //   // }
                            //   // JBI->Array_End();

                            //   // JBI->Object_End();


                            //   return JBI->End();
                            // }
                            // #endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES


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




                            /**
                            * @brief Debug time
                            * 
                            */


                              //   #ifdef DEBUG_MODULE_TIME_STD
                              //   JBI->Object_Start("debug_v2");
                              //     JBI->Add("utc_time",tkr_time->Rtc.utc_time);
                              //     JBI->Add("local_time",tkr_time->Rtc.local_time);
                              //     JBI->Add("daylight_saving_time",tkr_time->Rtc.daylight_saving_time);
                              //     JBI->Add("standard_time",tkr_time->Rtc.standard_time);
                              //     JBI->Add("ntp_time",tkr_time->Rtc.ntp_time);
                              //     JBI->Add("midnight",tkr_time->Rtc.midnight);
                              //     JBI->Add("restart_time",tkr_time->Rtc.restart_time);
                              //     JBI->Add("millis",tkr_time->Rtc.millis);
                              //     JBI->Add("last_sync",tkr_time->Rtc.last_sync);
                              //     JBI->Add("time_timezone",tkr_time->Rtc.time_timezone);
                              //     JBI->Add("ntp_sync_minute",tkr_time->Rtc.ntp_sync_minute);
                              //     JBI->Add("midnight_now",tkr_time->Rtc.midnight_now);
                              //     JBI->Add("user_time_entry",tkr_time->Rtc.user_time_entry);
                              //     JBI->Add("ntp_last_active_secs", (millis()-tkr_time->Rtc.ntp_last_active)/1000);
                              //     JBI->Add("last_sync_secs", (tkr_time->Rtc.utc_time-tkr_time->Rtc.last_sync)/1000);
                              //     JBI->Add("GetUptime",tkr_time->GetUptime(buffer,sizeof(buffer)));
                              //     JBI->Object_Start("DST");
                              //       JBI->Add("IsDst", tkr_time->IsDst());
                              //       int32_t dstoffset = tkr_set->Settings.toffset[1] * tkr_time->SECS_PER_MIN;
                              //       int32_t stdoffset = tkr_set->Settings.toffset[0] * tkr_time->SECS_PER_MIN;
                              //       JBI->Add("utc_time>=dst_eoy",tkr_time->Rtc.utc_time >= (tkr_time->Rtc.daylight_saving_time - stdoffset));
                              //       JBI->Add("utc_time<standard_soy",tkr_time->Rtc.utc_time < (tkr_time->Rtc.standard_time - dstoffset));

                              //       JBI->Add("diff_sod", tkr_time->Rtc.utc_time - (tkr_time->Rtc.daylight_saving_time - stdoffset));
                              //       JBI->Add("dif_eod",tkr_time->Rtc.utc_time - (tkr_time->Rtc.standard_time - dstoffset));
                              //     JBI->Object_End();
                              //     JBI->Add("toffset[0]", tkr_set->Settings.toffset[0]);
                              //     JBI->Add("toffset[1]", tkr_set->Settings.toffset[1]);
                              //   JBI->Object_End();    
                              //   // JBI->Object_Start("RtcTime");
                              //   //   JBI->Add("valid",tkr_time->RtcTime.valid);
                              //   //   JBI->Add_FV("time","\"%02d:%02d:%02d\"",tkr_time->RtcTime.hour,tkr_time->RtcTime.minute,tkr_time->RtcTime.second);
                              //   // JBI->End();
                              // #endif //  DEBUG_MODULE_TIME_STD





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

                            #endif // #ifndef FIRMWARE_MINIMAL2