/**
 * @file mTelemetry_ConstructJSON.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2022-08-17
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "mTelemetry.h"


uint8_t mTelemetry::ConstructJSON_Health(uint8_t json_level, bool json_appending)
{ //BuildHealth

  IPAddress localip = WiFi.localIP();

  JBI->Start();
  
    // //test devices
    // JBI->Level_Start("Test");
    //   JBI->Add("activity.loop_counter", pCONT_sup->activity.loop_counter);
    // //   JBI->Add("sleep", pCONT_set->sleep);
    // //   JBI->Add("loop_runtime_millis", pCONT_sup->loop_runtime_millis);
    // //   JBI->Add("loops_per_second", pCONT_sup->loops_per_second);
    // //   JBI->Add("this_cycle_ratio", pCONT_sup->this_cycle_ratio);
    // //   JBI->Add("loop_load_avg", pCONT_set->loop_load_avg);
    // //   JBI->Add("enable_sleep", pCONT_set->Settings.enable_sleep);
    // JBI->Level_End();
    // // test end

    JBI->Add(PM_JSON_TIME,           pCONT_time->RtcTime.hhmmss_ctr);
    JBI->Add_FV(PM_JSON_UPTIME,      PSTR("\"%02dT%02d:%02d:%02d\""), pCONT_time->uptime.Yday,pCONT_time->uptime.hour,pCONT_time->uptime.minute,pCONT_time->uptime.second);
    JBI->Add(PM_JSON_UPSECONDS,      pCONT_time->uptime.seconds_nonreset);
    JBI->Add(PM_JSON_SLEEPMODE,      pCONT_set->runtime_var.sleep ? "Dynamic" : "Unknown");
    JBI->Add(PM_JSON_SLEEP,          pCONT_set->runtime_var.sleep); // typ. 20
    JBI->Add(PM_JSON_LOOPSSEC,       pCONT_sup->activity.cycles_per_sec); // typ. 50hz
    JBI->Add(PM_JSON_LOADAVERAGE,    pCONT_set->loop_load_avg); // average loops_per_second
    JBI->Add(PM_JSON_FREEHEAP,       ESP.getFreeHeap());
    JBI->Add(PM_JSON_DEVICEFRIENDLYNAME, pCONT_set->Settings.system_name.friendly);
    JBI->Level_Start(PM_JSON_NETWORK);
      JBI->Add_FV(PM_JSON_IPADDRESS, PSTR("\"%d.%d.%d.%d\""), localip[0],localip[1],localip[2],localip[3]);
      JBI->Add(PM_JSON_SSID,         WiFi.SSID().c_str());
      JBI->Add(PM_JSON_RSSI,         WiFi.RSSI());
      #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
      // JBI->Add(PM_JSON_CONNECTCOUNT, wifi_reconnects_counter);
      JBI->Add(PM_JSON_DOWNTIME,     "00T00:00:00");
      JBI->Add(PM_JSON_DOWNSECS,     (uint8_t)0);
      #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    JBI->Level_End();
    #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    // JBI->Level_Start(PM_JSON_MQTT);
    //   JBI->Add(PM_JSON_SENTCOUNT,       pCONT_mqtt->pubsub->stats.packets_sent_counter);
    //   JBI->Add(PM_JSON_RECEIVEDCOUNT,   pCONT_mqtt->pubsub->stats.packets_sent_counter);
    //   JBI->Add(PM_JSON_SENTPERMINUTE,   pCONT_mqtt->pubsub->stats.packets_sent_per_minute);
    // JBI->Level_End();
    #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    #ifdef ENABLE_DEVFEATURE_HARDWARE_STATUS // Generate status message from all modules for human readable message
    // 2023, keep this, its for others to read, a simple ~100 char max message to say "Stable/Working/Boot Loops/Unstable/Sensor Error etc"
    memset(&hardwarestatus,0,sizeof(hardwarestatus));
    pCONT->Tasker_Interface(FUNC_STATUS_MESSAGE_APPEND);
    JBI->Level_Start(PM_JSON_STATUS);
      JBI->Add(PM_JSON_MESSAGE,         hardwarestatus.ctr); //this can be turned into a subadd method
      JBI->Add(PM_JSON_LEVEL,           hardwarestatus.importance);
    JBI->Level_End();
    #endif// ENABLE_DEVFEATURE_HARDWARE_STATUS
    // JBI->Add(PM_JSON_PAYLOAD_RATE,      pCONT_time->RtcTime.hhmmss_ctr);
    // ALOG_INF(PSTR("JBI=\"%s\""),JBI->GetPtr());
  return JBI->End();
    // DEBUG_LINE_HERE_VALUE(JBI->Length());
    
}


uint8_t mTelemetry::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{ 
  char buffer[50];

  JBI->Start();
    JBI->Add(PM_JSON_MODULENAME,     pCONT_set->Settings.system_name.friendly);
    JBI->Add(PM_JSON_FRIENDLYNAME,   pCONT_pins->ModuleName(buffer, sizeof(buffer))); 
    JBI->Add(PM_JSON_ROOMHINT, pCONT_set->Settings.room_hint);

    JBI->Add(PM_JSON_POWER,          pCONT_set->power); 
    
    JBI->Add(PM_JSON_SETTINGS_HOLDER,pCONT_set->Settings.cfg_holder);
    JBI->Add_FV(PM_JSON_SAVEADDRESS, PSTR("\"%X\""), pCONT_set->GetSettingsAddress());
    JBI->Add(PM_JSON_SAVECOUNT,      pCONT_set->Settings.save_flag);
    
    #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    JBI->Add(PM_JSON_POWERONSTATE,   pCONT_set->Settings.poweronstate); 
    JBI->Add(PM_JSON_LEDSTATE,       pCONT_set->Settings.ledstate);
    JBI->Add_FV(PM_JSON_LEDMASK,     PSTR("\"%04X\""), 0); 
    JBI->Add(PM_JSON_SAVEDATA,       0); 
    JBI->Add(PM_JSON_SAVESTATE,      0); 
    JBI->Add(PM_JSON_SWITCHMODE,     0);
    JBI->Add(PM_JSON_BAUDRATE,       (uint16_t)115200);
    JBI->Add(PM_JSON_BUTTONRETAIN,   pCONT_set->Settings.flag_system.mqtt_button_retain); 
    JBI->Add(PM_JSON_SWITCHRETAIN,   pCONT_set->Settings.flag_system.mqtt_switch_retain); 
    JBI->Add(PM_JSON_SENSORRETAIN,   pCONT_set->Settings.flag_system.mqtt_sensor_retain); 
    JBI->Add(PM_JSON_POWERRETAIN,    pCONT_set->Settings.flag_system.mqtt_power_retain);
    JBI->Add(PM_JSON_OTAURL,         D_OTA_URL);
    JBI->Add(PM_JSON_STARTUPUTC,     "2019-12-10T21:35:44");
    #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Firmware(uint8_t json_level, bool json_appending){ //BuildHealth
  char buffer[30];
  JBI->Start();
  
    JBI->Add(PM_JSON_VERSION_NAME,     pCONT_set->firmware_version.current.name_ctr);
    JBI->Add(PM_JSON_BUILDDATE,       __DATE__);
    
    JBI->Add(PM_JSON_SERIAL,          pCONT_log->GetLogLevelNamebyID(pCONT_set->Settings.seriallog_level, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_BOOTCOUNT,       pCONT_set->Settings.bootcount);
    JBI->Add(PM_JSON_BOOTCOUNTERRORS, pCONT_set->Settings.bootcount_errors_only);
    JBI->Add(PM_JSON_BUILDDATETIME,   pCONT_time->GetBuildDateAndTime(buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_BUILDTIME,       __TIME__);
    // JBI->Add(PM_JSON_VERSION,         pCONT_set->my_version);
    JBI->Add(PM_JSON_VERSION_NUMBER,   (uint32_t)PROJECT_VERSION);
    JBI->Add(PM_JSON_VERSION_NUMBER_MINIMUM,   (uint32_t)PROJECT_VERSION_MINIMAL);
    JBI->Add(PM_JSON_VERSION_NUMBER_NOTYPE,   (uint32_t)PROJECT_VERSION & 0x3FFFFFFF); //suppres 2 MSBs

    JBI->Level_Start(PM_JSON_VERSION_PARTS);
      JBI->Level_Start(PM_JSON_CURRENT);
        JBI->Add(PM_JSON_TYPE,   (uint8_t)FIRMWARE_VERSION_TYPE);
        JBI->Add(PM_JSON_MAJOR,   (uint8_t)FIRMWARE_VERSION_MAJOR);
        JBI->Add(PM_JSON_MINOR,   (uint8_t)FIRMWARE_VERSION_MINOR);
        JBI->Add(PM_JSON_CORE,   (uint8_t)FIRMWARE_VERSION_CORE);
        JBI->Add(PM_JSON_MODULE,   (uint8_t)FIRMWARE_VERSION_MODULE);
      JBI->Level_End();
      JBI->Level_Start(PM_JSON_MINIMAL);
        JBI->Add(PM_JSON_TYPE,   (uint8_t)FIRMWARE_VERSION_TYPE_MINIMAL);
        JBI->Add(PM_JSON_MAJOR,   (uint8_t)FIRMWARE_VERSION_MAJOR_MINIMAL);
        JBI->Add(PM_JSON_MINOR,   (uint8_t)FIRMWARE_VERSION_MINOR_MINIMAL);
        JBI->Add(PM_JSON_CORE,   (uint8_t)FIRMWARE_VERSION_CORE_MINIMAL);
        JBI->Add(PM_JSON_MODULE,   (uint8_t)FIRMWARE_VERSION_MODULE_MINIMAL);
      JBI->Level_End();
    JBI->Level_End();

    #ifdef ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    JBI->Add(PM_JSON_ARDUINO_CORE,     ARDUINO_ESP8266_RELEASE); 
    // JBI->Add(PM_JSON_DRIVERS,         "1,2,3,4,5,6,7,8,9,10,12,14,16,17,18,19,20,21,22,24,26,30");
    // JBI->Add(PM_JSON_SENSORS,         "1,2,3,4,5,6,7,8,9,10,12,14,16,17,18,19,20,21,22,24,26,30");
    JBI->Add_FV(PM_JSON_FEATURES,     PSTR("[\"%08X\",\"%08X\",\"%08X\",\"%08X\"]"), 0,0,0,0);

    #endif // ENABLE_DEVFEATURE_INCLUDE_INCOMPLETE_TELEMETRY_VALUES
    
    #ifdef ESP8266
    JBI->Add(PM_JSON_BOOTVERSION,     ESP.getBootVersion());
    #endif // ESP8266
    JBI->Add(PM_JSON_SDKVERSION,      ESP.getSdkVersion());    

    JBI->Add(PM_JSON_SDKVERSION,      ESP.getSdkVersion());    
    JBI->Add(PM_JSON_FREESKETCHSPACE,      ESP.getFreeSketchSpace());
    JBI->Add(PM_JSON_TEMPLATE_USED,   pCONT_set->boot_status.module_template_used);  
    JBI->Add(PM_JSON_TEMPLATE_PARSE_SUCCESS, pCONT_set->boot_status.module_template_parse_success); 

    
    JBI->Array_Start(PM_JSON_FASTBOOT_RECOVERY);
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
    JBI->Add(PM_JSON_WATCHDOG, "Enabled");
    // JBI->Add("WatchDogTimeout", D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS);
    #else
    JBI->Add(PM_JSON_WATCHDOG, "Disabled");
    #endif

  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Log(uint8_t json_level, bool json_appending){ 
  char buffer[30];
  JBI->Start();
    JBI->Level_Start(PM_JSON_LOGLEVELS);
      JBI->Add(PM_JSON_SERIAL, pCONT_log->GetLogLevelNamebyID(pCONT_set->Settings.seriallog_level, buffer, sizeof(buffer)));
      JBI->Add(PM_JSON_SYSTEM, pCONT_log->GetLogLevelNamebyID(pCONT_set->Settings.syslog_level, buffer, sizeof(buffer)));
      JBI->Add(PM_JSON_WEB,    pCONT_log->GetLogLevelNamebyID(pCONT_set->Settings.weblog_level, buffer, sizeof(buffer)));
      JBI->Add(PM_JSON_TELNET, pCONT_log->GetLogLevelNamebyID(pCONT_set->Settings.telnetlog_level, buffer, sizeof(buffer)));
    JBI->Level_End();
  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Memory(uint8_t json_level, bool json_appending){ // Debug info
  JBI->Start();
    JBI->Add(PM_JSON_PROGRAMSIZE,      ESP.getSketchSize()/1024);
    #ifdef ESP8266
    JBI->Add(PM_JSON_FREEMEMORY,       ESP.getFreeSketchSpace()/1024); // this takes seconds on esp32, what about here?... should I get and store on boot
    JBI->Add(PM_JSON_HEAPSIZE,         ESP.getFreeHeap()/1024);
    JBI->Add(PM_JSON_PROGRAMFLASHSIZE, ESP.getFlashChipSize()/1024);
    JBI->Add(PM_JSON_FLASHSIZE,        ESP.getFlashChipRealSize()/1024);
    JBI->Add(PM_JSON_FLASHCHIPID,      ESP.getFlashChipId());
    #else
    JBI->Add(PM_JSON_FREEMEMORY,       ESP.getFreeSketchSpace()/1024); // this takes seconds on esp32, what about here?... should I get and store on boot
    JBI->Add(PM_JSON_HEAPSIZE,         ESP.getFreeHeap()/1024);
    JBI->Add(PM_JSON_PROGRAMFLASHSIZE, ESP.getFlashChipSize()/1024);
    JBI->Add(PM_JSON_FLASHSIZE,        ESP.getFlashChipRealSize()/1024);
    // JBI->Add(PM_JSON_FLASHCHIPID,      ESP.getFlashChipId());
    #endif // ESP8266
    JBI->Add(PM_JSON_FLASHMODE,        (uint8_t)ESP.getFlashChipMode()); //FlashMode_t
  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Network(uint8_t json_level, bool json_appending){ // Debug info not just wifi, might be ethernet later
  
  IPAddress localip   = WiFi.localIP();
  IPAddress staticip  = IPAddress(pCONT_set->Settings.ip_address[0]);
  IPAddress gatewayip = IPAddress(pCONT_set->Settings.ip_address[1]);
  IPAddress subnetip  = IPAddress(pCONT_set->Settings.ip_address[2]);
  IPAddress dnsip     = IPAddress(pCONT_set->Settings.ip_address[3]);

  JBI->Start();
    JBI->Add_FV(PM_JSON_IPADDRESS,PSTR("\"%d.%d.%d.%d\""),localip[0],localip[1],localip[2],localip[3]);
    JBI->Add(PM_JSON_SSID, WiFi.SSID().c_str());
    JBI->Add(PM_JSON_SSID_NUMBERED, pCONT_set->Settings.sta_active); // Used to debug switching in grafana
    JBI->Add(PM_JSON_RSSI, WiFi.RSSI());
    // JBI->Add(PM_JSON_CONNECTCOUNT, wifi_reconnects_counter);
    JBI->Add(PM_JSON_HOSTNAME, pCONT_set->my_hostname);
    JBI->Add_P(PM_JSON_TELNET_PORT, TELNET_PORT);
    JBI->Add_FV(PM_JSON_STATIC_IPADDRESS,PSTR("\"%d.%d.%d.%d\""),staticip[0],staticip[1],staticip[2],staticip[3]);
    JBI->Add_FV(PM_JSON_GATEWAY,PSTR("\"%d.%d.%d.%d\""),gatewayip[0],gatewayip[1],gatewayip[2],gatewayip[3]);
    JBI->Add_FV(PM_JSON_SUBNETMASK,PSTR("\"%d.%d.%d.%d\""),subnetip[0],subnetip[1],subnetip[2],subnetip[3]);
    JBI->Add_FV(PM_JSON_DNSSERVER,PSTR("\"%d.%d.%d.%d\""),dnsip[0],dnsip[1],dnsip[2],dnsip[3]);
    JBI->Add(PM_JSON_BSSID, WiFi.BSSIDstr().c_str());
    JBI->Add(PM_JSON_MAC, WiFi.macAddress().c_str());
    JBI->Add(PM_JSON_WEBSERVER_ENABLED, pCONT_set->Settings.webserver);
    JBI->Add(PM_JSON_WIFICONFIG_STATE, pCONT_set->Settings.sta_config);

    JBI->Array_Start(PM_JSON_AP_LIST);
      JBI->Add(pCONT_set->Settings.sta_ssid[0]);
      JBI->Add(pCONT_set->Settings.sta_ssid[1]);
      JBI->Add(pCONT_set->Settings.sta_ssid[2]);
    JBI->Array_End();

  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_MQTT(uint8_t json_level, bool json_appending){

  JBI->Start();


    /**
     * @brief Show wrapper stuff
     * 
     */

    JBI->Add(PM_JSON_CLIENT_NAME, pCONT_set->Settings.mqtt.client_name);

    JBI->Add("RetrySecs", pCONT_set->Settings.mqtt_retry);

    JBI->Level_Start(PM_JSON_REFRESH_RATES);
      JBI->Add(PM_JSON_MQTT_REFRESH_RATE_IFCHANGED, pCONT_set->Settings.sensors.ifchanged_secs);
      JBI->Add(PM_JSON_MQTT_REFRESH_RATE_TELEPERIOD, pCONT_set->Settings.sensors.teleperiod_secs);
    JBI->Level_End();
    
    JBI->Add(PM_JSON_MQTT_ENABLE_RESTART,   (uint8_t)0);


    #ifdef ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY
    JBI->Level_Start("Interface_Priority");
      JBI->Add(D_MODULE_ENERGY_INTERFACE_FRIENDLY_CTR, pCONT_set->Settings.mqtt.interface_reporting_priority.energy);
    JBI->Level_End();
    #endif // ENABLE_DEVFEATURE_REDUCE_SUBORDINATE_MQTT_REPORTING_ENERGY


    /**
     * @brief Show each instance info
     * 
    **/
    #ifdef USE_MODULE_NETWORK_MQTT
    JBI->Array_Start("Instance");
    for(auto& con:pCONT_mqtt->brokers)
    {
      
      JBI->Array_Start();
        JBI->Add("broker_url", con->broker_url);
        JBI->Add("port", con->port);

        JBI->Add("connect_count", con->connect_count);
        JBI->Add("retry_counter", con->retry_counter);
        JBI->Add("downtime_counter", con->downtime_counter);
        // JBI->Add("initial_connection_state", con->initial_connection_state);
        JBI->Add("connected", con->connected);
        JBI->Add("allowed", con->allowed);
        JBI->Add("mqtt_tls", con->mqtt_tls);

        JBI->Add("mqtt_client_type", con->client_type);

        JBI->Add("tSaved_LastOutGoingTopic", con->tSaved_LastOutGoingTopic);

        JBI->Add("flag_start_reconnect", con->flag_start_reconnect);
        JBI->Add("cConnectionAttempts", con->cConnectionAttempts);

        JBI->Add("host_server_type", con->host_server_type);

        #ifdef ENABLE_DEBUGFEATURE__MQTT_COUNT_PUBLISH_SUCCESS_RATE
        JBI->Add("payload_publish_sent", con->debug_stats.payload_publish_sent);
        JBI->Add("payload_publish_missed", con->debug_stats.payload_publish_missed);
        JBI->Add("payload_publish_success_percentage", con->debug_stats.payload_publish_success_percentage);
        #endif
      JBI->Array_End();

    }
    JBI->Array_End();
    #endif // USE_MODULE_NETWORK_MQTT
  
  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Time(uint8_t json_level, bool json_appending){ 

  char buffer[100];
       
  JBI->Start();
    JBI->Add(PM_JSON_UTC_TIME,   pCONT_time->GetDateAndTimeCtr(DT_UTC, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_LOCAL_TIME, pCONT_time->GetDateAndTimeCtr(DT_LOCALNOTZ, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_STARTDST,   pCONT_time->GetDateAndTimeCtr(DT_DST, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_ENDDST,     pCONT_time->GetDateAndTimeCtr(DT_STD, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_TIMEZONE,   pCONT_time->GetDateAndTimeCtr(DT_TIMEZONE, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_SUNRISE,    pCONT_time->GetDateAndTimeCtr(DT_SUNRISE, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_SUNSET,     pCONT_time->GetDateAndTimeCtr(DT_SUNSET, buffer, sizeof(buffer)));

    #ifdef DEBUG_MODULE_TIME_STD
    JBI->Level_Start("debug_v2");
      JBI->Add("utc_time",pCONT_time->Rtc.utc_time);
      JBI->Add("local_time",pCONT_time->Rtc.local_time);
      JBI->Add("daylight_saving_time",pCONT_time->Rtc.daylight_saving_time);
      JBI->Add("standard_time",pCONT_time->Rtc.standard_time);
      JBI->Add("ntp_time",pCONT_time->Rtc.ntp_time);
      JBI->Add("midnight",pCONT_time->Rtc.midnight);
      JBI->Add("restart_time",pCONT_time->Rtc.restart_time);
      JBI->Add("millis",pCONT_time->Rtc.millis);
      JBI->Add("last_sync",pCONT_time->Rtc.last_sync);
      JBI->Add("time_timezone",pCONT_time->Rtc.time_timezone);
      JBI->Add("ntp_sync_minute",pCONT_time->Rtc.ntp_sync_minute);
      JBI->Add("midnight_now",pCONT_time->Rtc.midnight_now);
      JBI->Add("user_time_entry",pCONT_time->Rtc.user_time_entry);
      JBI->Add("ntp_last_active_secs", (millis()-pCONT_time->Rtc.ntp_last_active)/1000);
      JBI->Add("last_sync_secs", (pCONT_time->Rtc.utc_time-pCONT_time->Rtc.last_sync)/1000);
      JBI->Add("GetUptime",pCONT_time->GetUptime().c_str());
      JBI->Level_Start("DST");
        JBI->Add("IsDst", pCONT_time->IsDst());
        int32_t dstoffset = pCONT_set->Settings.toffset[1] * pCONT_time->SECS_PER_MIN;
        int32_t stdoffset = pCONT_set->Settings.toffset[0] * pCONT_time->SECS_PER_MIN;
        JBI->Add("utc_time>=dst_eoy",pCONT_time->Rtc.utc_time >= (pCONT_time->Rtc.daylight_saving_time - stdoffset));
        JBI->Add("utc_time<standard_soy",pCONT_time->Rtc.utc_time < (pCONT_time->Rtc.standard_time - dstoffset));

        JBI->Add("diff_sod", pCONT_time->Rtc.utc_time - (pCONT_time->Rtc.daylight_saving_time - stdoffset));
        JBI->Add("dif_eod",pCONT_time->Rtc.utc_time - (pCONT_time->Rtc.standard_time - dstoffset));
      JBI->Level_End();
      JBI->Add("toffset[0]", pCONT_set->Settings.toffset[0]);
      JBI->Add("toffset[1]", pCONT_set->Settings.toffset[1]);
    JBI->Level_End();    
  #endif //  DEBUG_MODULE_TIME_STD

    // JBI->Level_Start("RtcTime");
    //   JBI->Add("valid",pCONT_time->RtcTime.valid);
    //   JBI->Add_FV("time","\"%02d:%02d:%02d\"",pCONT_time->RtcTime.hour,pCONT_time->RtcTime.minute,pCONT_time->RtcTime.second);
    // JBI->End();
// #endif // ENABLE_DEVFEATURE_RTC_TIME_V2_MQTT_DEBUG
  return JBI->End();

}


/**
 * should this be renamed, as "devices" needs to refer to the internal list, 
 * but, I also want to have a list of connected hardware, so that might be better suited to devices (combining for now)
 * */
uint8_t mTelemetry::ConstructJSON_Devices(uint8_t json_level, bool json_appending){ 

  JBI->Start();
    JBI->Add(PM_JSON_MODULENAME,          pCONT_set->Settings.module);
    // If debug mode, show entire message
    // JBI->Array_AddArray(PM_JSON_DEVICEID, pCONT_set->Settings.device_name_buffer.device_id, ARRAY_SIZE(pCONT_set->Settings.device_name_buffer.device_id));
    // JBI->Array_AddArray(PM_JSON_CLASSID,  pCONT_set->Settings.device_name_buffer.class_id,  ARRAY_SIZE(pCONT_set->Settings.device_name_buffer.class_id));
    // else, only show ysed
    JBI->Add("DLI->GetLengthIndexUsed()", DLI->GetLengthIndexUsed());
    JBI->Array_AddArray(PM_JSON_DEVICEID, pCONT_set->Settings.device_name_buffer.device_id, DLI->GetLengthIndexUsed());
    JBI->Array_AddArray(PM_JSON_CLASSID,  pCONT_set->Settings.device_name_buffer.class_id,  DLI->GetLengthIndexUsed());
    
    
    JBI->Add(PM_JSON_BUFFER,        pCONT_set->Settings.device_name_buffer.name_buffer);
    JBI->Add(PM_JSON_BUFLEN,        strlen(pCONT_set->Settings.device_name_buffer.name_buffer));

    uint8_t count = 0;
    if(strlen(pCONT_set->Settings.device_name_buffer.name_buffer)){ //if anything in buffer
      count = DLI->CountCharInCtr(pCONT_set->Settings.device_name_buffer.name_buffer,'|'); // first | indicates index from 0
    }
    JBI->Add("ItemCount", count);

    
    if(pCONT_pins->PinUsed(GPIO_I2C_SCL_ID)&&pCONT_pins->PinUsed(GPIO_I2C_SDA_ID))
    {

      #ifdef ESP32
      JBI->Add("I2C_BusSpeed", pCONT_sup->wire->getClock());
      #endif

      char mqtt_data[300];
      pCONT_sup->I2cScan(mqtt_data, sizeof(mqtt_data));
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


uint8_t mTelemetry::ConstructJSON_Reboot(uint8_t json_level, bool json_appending){ // 

  JBI->Start();
  
  JBI->Add(PM_JSON_DEVICE, pCONT_set->Settings.system_name.device);
  JBI->Add(PM_JSON_DEVICEFRIENDLYNAME, pCONT_set->Settings.system_name.friendly);
  JBI->Add_FV(PM_JSON_DATETIME, PSTR("\"%02d-%02d-%02d %02d:%02d:%02d\""),
                      pCONT_time->RtcTime.Mday, pCONT_time->RtcTime.month, pCONT_time->RtcTime.year,
                      pCONT_time->RtcTime.hour, pCONT_time->RtcTime.minute, pCONT_time->RtcTime.second
                    );
  JBI->Level_Start(PM_JSON_COUNTER);
    JBI->Add("All", (uint8_t)0);
    // JBI->Add("WDT", (uint8_t)0);
  JBI->Level_End();

  // if (pCONT_sup->CrashFlag()) {
    
  JBI->Level_Start(PM_JSON_CRASHDUMP);
    pCONT_sup->WriteBuffer_P(PSTR(","));
    pCONT_sup->CrashDump_AddJson();
  JBI->Level_End();

  // } else {
  //   char buffer[30];
  //   JBI->Add("Reason", pCONT_sup->GetResetReason(buffer, sizeof(buffer)));
  // }

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

  IPAddress localip = WiFi.localIP();
  
  JBI->Start();
    JBI->Add_FV(PM_JSON_UPTIME,      PSTR("\"%02dT%02d:%02d:%02d\""), pCONT_time->uptime.Yday,pCONT_time->uptime.hour,pCONT_time->uptime.minute,pCONT_time->uptime.second);
    JBI->Add(PM_JSON_UPSECONDS,      pCONT_time->uptime.seconds_nonreset);
    JBI->Add(PM_JSON_SLEEP,          pCONT_sup->loop_delay_temp);
    JBI->Add(PM_JSON_LOOPSSEC,       pCONT_sup->activity.cycles_per_sec);
    JBI->Add(PM_JSON_LOOPRATIO,      pCONT_sup->this_cycle_ratio);
    #ifdef USE_NETWORK_MDNS
    JBI->Add(PM_JSON_MDNS,           pCONT_set->boot_status.mdns_started_succesfully);
    #endif // #ifdef USE_NETWORK_MDNS
    JBI->Add(PM_JSON_FREEHEAP,       ESP.getFreeHeap());
    JBI->Add(PM_JSON_VERSION_NAME,    pCONT_set->firmware_version.current.name_ctr);
    JBI->Add_FV(PM_JSON_IPADDRESS,   PSTR("\"%d.%d.%d.%d\""), localip[0],localip[1],localip[2],localip[3]);
    JBI->Add(PM_JSON_BOOTCOUNT,      pCONT_set->Settings.bootcount);
  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Debug_Pins(uint8_t json_level, bool json_appending)
{

  // return 0;

  char buffer[30];
  JBI->Start();
  JBI->Add("flag_serial_set_tx_set",pCONT_pins-> flag_serial_set_tx_set);
    // JBI->Level_Start(PM_JSON_GPIO);
    // for(uint16_t i=0;i<sizeof(pCONT_set->pin);i++){ 
    //   if(pCONT_pins->PinUsed(i)){ // skip pins not configured
    //     sprintf_P(buffer, PSTR("FUNC_%d"), i);
    //     JBI->Add(buffer, pCONT_pins->GetPin(i));
    //   }
    // }
    // JBI->Level_End();
    JBI->Level_Start(PM_JSON_GPIO);
    for(uint16_t i=0;i<ARRAY_SIZE(pCONT_pins->pin_attached_gpio_functions);i++){ 
      if(pCONT_pins->PinUsed(pCONT_pins->pin_attached_gpio_functions[i])){ // skip pins not configured

      // AddLog(LOG_LEVEL_TEST, PSTR("buffer=%s %d %d %d"),
      // pCONT_pins->GetGPIOFunctionNamebyID_P(pCONT_pins->pin_attached_gpio_functions[i]),
      // pCONT_pins->pin_attached_gpio_functions[i],
      // pCONT_pins->GetPin(i),
      // i 
      // );


        sprintf_P(buffer, PSTR("%s"), pCONT_pins->GetGPIOFunctionNamebyID_P(pCONT_pins->pin_attached_gpio_functions[i]));
        JBI->Add(buffer, pCONT_pins->GetPin(pCONT_pins->pin_attached_gpio_functions[i]));
      }
    }
    
    JBI->Level_End();
    // JBI->Level_Start(D_JSON_GPIO "_map");
    // for(uint16_t i=0;i<MAX_USER_PINS;i++){ 
    //   sprintf_P(buffer, PSTR("%d"),
      
    //   gpio_pin_by_index[i]
    //   //  i
    //    );
    //   JBI->Add(buffer, pCONT_pins->GetPin(i));
    // }
    // JBI->Level_End();

    // Debug by printing all arrays out
    JBI->Array_Start("pin_attached_gpio_functions");
    for(int i=0; i<ARRAY_SIZE(pCONT_pins->pin_attached_gpio_functions);i++)
      JBI->Add(pCONT_pins->pin_attached_gpio_functions[i]);
    JBI->Array_End();

    JBI->Array_Start("user_template_io");
    for(int i=0; i<ARRAY_SIZE(pCONT_set->Settings.user_template2.hardware.gp.io);i++)
      JBI->Add(pCONT_set->Settings.user_template2.hardware.gp.io[i]);
    JBI->Array_End();

    JBI->Array_Start("getpin");
    for(int i=0; i<ARRAY_SIZE(pCONT_pins->pin_attached_gpio_functions);i++)
    {
// DEBUG_LINE_HERE;
      JBI->Add(
        pCONT_pins->GetPinWithGPIO(pCONT_pins->pin_attached_gpio_functions[i])
        
        
        );
// DEBUG_LINE_HERE;
    }
    JBI->Array_End();





  return JBI->End();

}


uint8_t mTelemetry::ConstructJSON_Debug_Template(uint8_t json_level, bool json_appending){ //BuildHealth
  char buffer[50];
  JBI->Start();
    JBI->Add("Module" D_JSON_TEMPLATE,       pCONT_set->boot_status.module_template_parse_success?"Default":"Saved");
    JBI->Add("Function" D_JSON_TEMPLATE,       pCONT_set->boot_status.function_template_parse_success);//?"Default":"Saved");

    JBI->Add(PM_JSON_MODULENAME, pCONT_pins->AnyModuleName(pCONT_set->Settings.module, buffer, sizeof(buffer)));
    JBI->Add(PM_JSON_MODULEID,   pCONT_set->Settings.module);
    JBI->Add("MyModuleType",pCONT_set->my_module_type);
    myio cmodule;
    pCONT_pins->TemplateGPIOs(&cmodule);
    // JBI->Array_AddArray(PM_JSON_GPIO, cmodule.io, (uint8_t)sizeof(cmodule.io));
    JBI->Array_Start_P(PM_JSON_GPIO);
    for(int i=0;i<ARRAY_SIZE(cmodule.io);i++)
      JBI->Add(cmodule.io[i]);
    JBI->Array_End();
  return JBI->End();
}


uint8_t mTelemetry::ConstructJSON_Debug_ModuleInterface(uint8_t json_level, bool json_appending){ //BuildHealth

  JBI->Start();
  
  #ifdef DEBUG_EXECUTION_TIME
    char buffer[50];
    JBI->Level_Start(pCONT->GetModuleFriendlyName(pCONT->module_settings.list[ii], buffer));
      JBI->Array_AddArray("average", pCONT->module_settings.execution_time_average_ms, sizeof(pCONT->module_settings.execution_time_average_ms));
      JBI->Array_AddArray("max",     pCONT->module_settings.execution_time_max_ms,     sizeof(pCONT->module_settings.execution_time_max_ms));
    JBI->Level_End();
  #endif

  JBI->Level_Start("ModuleSize");

  
  // for(uint8_t i=0;i<pCONT->module_settings.count;i++){
  //   JBI->Add_P(pCONT->GetModuleFriendlyName(pCONT->module_settings.list[i]),pCONT->GetClassSizeByID(pCONT->module_settings.list[i]));
  //   // if(pCONT->GetClassSizeByID(i)>10000){
  //   //   JBI->Add("bad",i);
  //   // }
  // }

  /**
   * @brief Add array of all unique id's (in a json array, this will just replace... so maybe use name+id? or rather ID_NAME so it will be easier to spot numbers the same)
   * I could also run a "append" id but check its not in it already (easier with vector?)
   * 
   */
  char buffer[100];

  JBI->Array_Start("ModuleIDs");
  for(int ii=0;ii<pCONT->GetClassCount();ii++)
  {
    snprintf_P(buffer, sizeof(buffer), PSTR("%04d_%S"), pCONT->pModule[ii]->GetModuleUniqueID(), pCONT->pModule[ii]->GetModuleFriendlyName()  );

    JBI->Add(buffer);
  }
  JBI->Array_End();

  JBI->Level_End();


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

  JBI->Level_Start("Header");
    JBI->Add("cfg_holder", pCONT_set->Settings.cfg_holder);
    JBI->Add("cfg_size", pCONT_set->Settings.cfg_size);
    JBI->Add("save_flag", pCONT_set->Settings.save_flag);
    JBI->Add("version", pCONT_set->Settings.version);
    JBI->Add("bootcount", pCONT_set->Settings.bootcount);
    JBI->Add("cfg_crc", pCONT_set->Settings.cfg_crc);
  JBI->Level_End();


  JBI->Level_Start("Animations");
  //   JBI->Array_AddArray("controller", 
  //   &pCONT_set->Settings.animation_settings.xmas_controller_params, 
  //   sizeof(pCONT_set->Settings.animation_settings.xmas_controller_params));
  // JBI->Level_End();
  // JBI->Level_Start("Animations1");
    JBI->Array_Start("controller"); 
    // &pCONT_set->Settings.animation_settings.xmas_controller_params, 
    // sizeof(pCONT_set->Settings.animation_settings.xmas_controller_params));

    for(int i=0;i<10;i++)
    {
      JBI->Add(pCONT_set->Settings.animation_settings.xmas_controller_params[i]);
    }

    JBI->Array_End();
  JBI->Level_End();

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
    




    // JBI->Add("ALTITUDE_ABOVE_SEALEVEL",       (float)ALTITUDE_ABOVE_SEALEVEL);
    JBI->Add("altitude",     pCONT_set->Settings.sensors.altitude);
    // JBI->Add("LATITUDE",                      (float)LATITUDE);
    JBI->Add("latitude",     pCONT_set->Settings.sensors.latitude);
    // JBI->Add("LONGITUDE",       (float)LONGITUDE);
    JBI->Add("longitude",                     pCONT_set->Settings.sensors.longitude);

  return JBI->End();
}


#ifdef ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES
uint8_t mTelemetry::ConstructJSON_Debug_Tasker_Interface_Performance(uint8_t json_level, bool json_appending)
{

  JBI->Start();

  char buffer2[100];
  
  for(int ii=0;ii<pCONT->GetClassCount();ii++)
  {
    JBI->Level_Start_P(pCONT->pModule[ii]->GetModuleFriendlyName());

      JBI->Add("max_time", pCONT->debug_module_time[ii].max_time);
      JBI->Add("avg_time", pCONT->debug_module_time[ii].avg_time);
      JBI->Add("max_function_id", pCONT->debug_module_time[ii].max_function_id);

    JBI->Level_End();




  }







  //   JBI->Add_P(pCONT->GetModuleFriendlyName(pCONT->module_settings.list[i]),pCONT->GetClassSizeByID(pCONT->module_settings.list[i]));
  //   // if(pCONT->GetClassSizeByID(i)>10000){
  //   //   JBI->Add("bad",i);
  //   // }



  // }

  // /**
  //  * @brief Add array of all unique id's (in a json array, this will just replace... so maybe use name+id? or rather ID_NAME so it will be easier to spot numbers the same)
  //  * I could also run a "append" id but check its not in it already (easier with vector?)
  //  * 
  //  */
  // char buffer[100];



  // JBI->Array_Start("ModuleIDs");
  // for(int ii=0;ii<pCONT->GetClassCount();ii++)
  // {
  //   snprintf_P(buffer, sizeof(buffer), PSTR("%04d_%S"), pCONT->pModule[ii]->GetModuleUniqueID(), pCONT->pModule[ii]->GetModuleFriendlyName()  );

  //   JBI->Add(buffer);
  // }
  // JBI->Array_End();

  // JBI->Level_End();


  return JBI->End();
}
#endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES