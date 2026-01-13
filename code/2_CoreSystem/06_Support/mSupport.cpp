
#include "2_CoreSystem/06_Support/mSupport.h"

/*********************************************************************************************
 * Watchdog related
\*********************************************************************************************/
// #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
//   #ifdef ESP8266
//     void WDT_Init(){ ESP.wdtEnable(6000);}
//     void WDT_Reset(){ESP.wdtFeed(); }
//   #endif // ESP8266
//   #ifdef ESP32
//     #include "esp_system.h"
//     #ifndef D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS
//     #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 60000
//     #endif
//     const uint64_t wdtTimeout = D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS;  //time in ms to trigger the watchdog
//     hw_timer_t *timerwdt = NULL;
//     #ifndef ARDUINO_ISR_ATTR
//     #define ARDUINO_ISR_ATTR IRAM_ATTR 
//     #endif
//     void ARDUINO_ISR_ATTR resetModule() {
//       ets_printf("reboot resetModule\n");
//       Serial.println("WDT REBOOTING!!"); Serial.flush();
//       esp_restart();
//     }
//     // hw_timer_t *timerwdt = NULL;
//     // void IRAM_ATTR resetModule(){ets_printf("\n\n\n\n\n\nWDT REBOOTING!!\n");ESP.restart();}
//     // void WDT_Init() {
//     //   timerwdt = timerBegin(0, 80, true);  // timer 0, div 80
  
//     //   // Use LEVEL mode on ESP32-C3 (EDGE unsupported), keep EDGE for others
//     //   #if defined(CONFIG_IDF_TARGET_ESP32C3)
//     //       timerAttachInterrupt(timerwdt, &resetModule, false);
//     //   #else
//     //       timerAttachInterrupt(timerwdt, &resetModule, true);
//     //   #endif
  
//     //   timerAlarmWrite(timerwdt, wdtTimeout * 1000, false);  // set time in us
//     //   timerAlarmEnable(timerwdt);     
      
//       void WDT_Init() {
//         #if !defined(CONFIG_IDF_TARGET_ESP32C3)
//           timerwdt = timerBegin(0, 80, true); // Only setup watchdog timer if NOT ESP32-C3
//           timerAttachInterrupt(timerwdt, &resetModule, true);
//           timerAlarmWrite(timerwdt, wdtTimeout * 1000, false);
//           timerAlarmEnable(timerwdt);
//         #else
//           // On ESP32-C3, skip watchdog timer init for now
//         #endif
//         }
        
//         // enable interrupt
//     // }
  
//     void WDT_Reset(){
//       // DEBUG_LINE_HERE;
//       if(timerwdt==nullptr){ 
//         DEBUG_LINE_HERE; 
//         WDT_Init(); 
//         DEBUG_LINE_HERE;
//       }
//       timerWrite(timerwdt, 0); //reset timerwdt (feed watchdog)
//       DEBUG_LINE_HERE;
//       // Serial.println("WDT_Reset");
//     }
//   #endif // ESP32
// #else
// void WDT_Init(){};
// void WDT_Reset(){};
// #endif // WATCHDOG_TIMER_SECTION_GUARD_H
#ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  #ifdef ESP8266
    void WDT_Init(){ ESP.wdtEnable(6000); }
    void WDT_Reset(){ ESP.wdtFeed(); }
  #endif

  #ifdef ESP32
    #ifndef D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS
      #define D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS 6000
    #endif

    #if defined(CONFIG_IDF_TARGET_ESP32C3)
      #include "esp_task_wdt.h"
      void WDT_Init() {
        esp_task_wdt_init(D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS / 1000, true);  // panic = true
        esp_task_wdt_add(NULL);  // add current task
      }

      void WDT_Reset() {
        esp_task_wdt_reset();
      }
    #else
      #include "esp_system.h"
      hw_timer_t *timerwdt = NULL;
      const uint64_t wdtTimeout = D_WATCHDOG_TIMER_TIMEOUT_PERIOD_MS;

      void IRAM_ATTR resetModule() {
        ets_printf("reboot resetModule\n");
        Serial.println("WDT REBOOTING!!"); Serial.flush();
        esp_restart();
      }

      void WDT_Init() {
        timerwdt = timerBegin(0, 80, true);
        timerAttachInterrupt(timerwdt, &resetModule, true);
        timerAlarmWrite(timerwdt, wdtTimeout * 1000, false);
        timerAlarmEnable(timerwdt);
      }

      void WDT_Reset() {
        if (timerwdt == nullptr) WDT_Init();
        timerWrite(timerwdt, 0);
      }
    #endif
  #endif
#else
  void WDT_Init(){}
  void WDT_Reset(){}
#endif


#ifdef ESP32
  #include "rom/rtc.h"
#endif


// uint32_t ResetReason_g(void)
// {
//   DEBUG_LINE_HERE
// //   /*
// //     user_interface.h
// //     REASON_DEFAULT_RST      = 0,  // "Power on"                normal startup by power on
// //     REASON_WDT_RST          = 1,  // "Hardware Watchdog"       hardware watch dog reset
// //     REASON_EXCEPTION_RST    = 2,  // "Exception"               exception reset, GPIO status won’t change
// //     REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
// //     REASON_SOFT_RESTART     = 4,  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
// //     REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"         wake up from deep-sleep
// //     REASON_EXT_SYS_RST      = 6   // "External System"         external system reset
// //   */
 
// // #ifdef ESP8266
// //   return resetInfo.reason;
// // #else
// //   return 0;

// // #endif// ESP8266


//   /*
//     user_interface.h
//     REASON_DEFAULT_RST      = 0,  // "Power on"                normal startup by power on
//     REASON_WDT_RST          = 1,  // "Hardware Watchdog"       hardware watch dog reset
//     REASON_EXCEPTION_RST    = 2,  // "Exception"               exception reset, GPIO status won’t change
//     REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
//     REASON_SOFT_RESTART     = 4,  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
//     REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"         wake up from deep-sleep
//     REASON_EXT_SYS_RST      = 6   // "External System"         external system reset
//   */

//   DEBUG_LINE_HERE
// #ifdef ESP32

//  RESET_REASON reason = rtc_get_reset_reason(0);
//  Serial.println(reason); Serial.flush();
//  DEBUG_LINE_HERE
//   if (1  == reason) { return REASON_DEFAULT_RST; }       // POWERON_RESET
//   if (12 == reason) { return REASON_SOFT_RESTART; }      // SW_CPU_RESET / RTC_SW_CPU_RESET
//   if (5  == reason) { return REASON_DEEP_SLEEP_AWAKE; }  // DEEPSLEEP_RESET
//   if (3  == reason) { return REASON_EXT_SYS_RST; }       // SW_RESET / RTC_SW_SYS_RESET
//   return -1; // no "official error code", but should work with the current code base

  
//   // #ifdef DEBUG_FASTBOOT
//   // Serial.printf("ResetReason() = %d\n\r", ResetReason_g()); 
//   // #endif
  

// #endif 

//   DEBUG_LINE_HERE
// #ifdef ESP8266
//  return 0;// resetInfo.reason;  // causing crasg exception 3

// #endif

// return 0;
//   DEBUG_LINE_HERE


//   // return ESP_ResetInfoReason();
// }



uint32_t ResetReason_g(void)
{
  DEBUG_LINE_HERE

  /*
    user_interface.h (ESP8266 style)
    REASON_DEFAULT_RST      = 0,  // "Power on"
    REASON_WDT_RST          = 1,  // "Hardware Watchdog"
    REASON_EXCEPTION_RST    = 2,  // "Exception"
    REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"
    REASON_SOFT_RESTART     = 4,  // "Software/System restart"
    REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"
    REASON_EXT_SYS_RST      = 6   // "External System reset"
  */

#ifdef ESP32
  // Use IDF API (correct for ESP32 / S2 / S3 / C3 etc.)
  // Arduino core provides this via esp_system.h
  const esp_reset_reason_t rr = esp_reset_reason();

  // Optional debug print (safe early)
  Serial.printf("esp_reset_reason()=%d\r\n", (int)rr);

  switch (rr)
  {
    case ESP_RST_POWERON:
      return REASON_DEFAULT_RST;

    case ESP_RST_SW:
      // includes esp_restart(), software reset
      return REASON_SOFT_RESTART;

    case ESP_RST_PANIC:
      // abort(), assert(), stack smashing, etc.
      return REASON_EXCEPTION_RST;

    case ESP_RST_INT_WDT:
    case ESP_RST_TASK_WDT:
      // WDT reset (task watchdog / interrupt watchdog)
      // You may prefer REASON_SOFT_WDT_RST for TASK_WDT and REASON_WDT_RST for INT_WDT;
      // if you want that split, see note below.
      return REASON_WDT_RST;

    case ESP_RST_WDT:
      // generic watchdog reset
      return REASON_WDT_RST;

    case ESP_RST_DEEPSLEEP:
      return REASON_DEEP_SLEEP_AWAKE;

    case ESP_RST_BROWNOUT:
      // No direct equivalent in your enum; closest operationally is "power on / external"
      // I recommend treating brownout as "default/power" for user-facing messaging.
      return REASON_DEFAULT_RST;

    case ESP_RST_SDIO:
      // Rare; treat as external/system
      return REASON_EXT_SYS_RST;

    case ESP_RST_EXT:
      // External reset pin
      return REASON_EXT_SYS_RST;

    case ESP_RST_UNKNOWN:
    default:
      // Fail safe: treat unknown as power-on/default
      return REASON_DEFAULT_RST;
  }
#endif // ESP32


#ifdef ESP8266
  // Use SDK reset info (safe in Arduino-ESP8266)
  // Note: if your old path crashed, it was likely due to accessing an invalid pointer;
  // this is the standard safe call.
  const rst_info* ri = ESP.getResetInfoPtr();
  if (ri)
  {
    return (uint32_t)ri->reason;  // already matches REASON_* values
  }
  return REASON_DEFAULT_RST;
#endif // ESP8266

  return REASON_DEFAULT_RST;
}



#ifdef ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#endif
#ifdef ESP8266
// #include <ESP8266HTTPClient.h>
// #include <ESP8266httpUpdate.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>                   // Arduino OTA
#endif

#endif // ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID

#if defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) || defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)

const char* host = "recovery";
const char* ssid = STA_SSID1;
const char* password = STA_PASS1;

#endif //  defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) || (ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)


#ifdef ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID

#ifdef ESP32
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#endif
#ifdef ESP8266

  #include <ESP8266HTTPClient.h>
  #include <ESP8266httpUpdate.h> 
  #include <ESP8266WiFi.h> 

  #error "Need to figure out webserver replacement for esp8266"

#endif

/**
 * @brief Using pointer to reduce memory footprint
 **/
WebServer* http_safemode_server = nullptr;// server(80);

/*
 * Server Index Page
 */

const char* serverIndex =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')"
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";
#endif // ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID


#if defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) || defined(ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)

/**
 * @brief Single function that a fastboot'ing device will call to await for new code to be uploaded
 * "0" means to wait indefinitely, ie fastboot is recovering
 **/
void SafeMode_StartAndAwaitOTA(uint8_t seconds_to_wait)
{
  Serial.begin(115200);

  Serial.println("SafeMode_StartAndAwaitOTA");
  
  uint32_t tSaved_heartbeat = millis();
  #ifdef ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  bool http_triggered = false;
  #endif

  // Connect to WiFi network
  WiFi.begin(ssid, password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local           
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  
  Serial.println("mDNS responder started");

  #ifdef ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  http_safemode_server = new WebServer(80);

  /*return index page which is stored in serverIndex */
  http_safemode_server->on("/", HTTP_GET, []() {
    http_safemode_server->sendHeader("Connection", "close");
    http_safemode_server->send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  http_safemode_server->on("/update", HTTP_POST, []() {
    http_safemode_server->sendHeader("Connection", "close");
    http_safemode_server->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    
      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif

    HTTPUpload& upload = http_safemode_server->upload();
    if (upload.status == UPLOAD_FILE_START) {
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
      // http_triggered = true;
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", upload.totalSize);

        /**
         * @brief Reset RTC so device can reboot out of safemode
         **/
        RtcFastboot_Reset();
      } else {
        Update.printError(Serial);
      }
    }
  });
  http_safemode_server->begin();
  #endif // ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID

  #ifdef ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  // bool ota_triggered = false;
  ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      // ota_triggered = true;
      Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
      
      RtcFastboot_Reset();
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      
      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif

    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

  ArduinoOTA.begin();
  #endif // ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID


  Serial.printf("SafeMode: Awaiting OTA/HTTP recovery for %d seconds\n", seconds_to_wait);

  bool bExit = false;
  uint32_t tStart = millis();
  while(!bExit)
  {
    #ifdef ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
    http_safemode_server->handleClient();
    #endif
    #ifdef ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
    ArduinoOTA.handle();
    #endif

    if(seconds_to_wait>0)
    {
      if(millis()-tStart>seconds_to_wait*1000)
      {
        bExit = true;
      }
    }

    if(llabs(millis()-tSaved_heartbeat)>1000)
    {
      tSaved_heartbeat = millis();
      Serial.println("Waiting for Recovery");
      Serial.print("Connected to ");
      Serial.println(ssid);
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());

      #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
      WDT_Reset();
      #endif

    }
  }
  // while(1){ 
  //   // Serial.println("SafeMode: Just waiting for OTA"); delay(1000);
    
  //   #ifdef ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID
  //   ArduinoOTA.handle(); 
  //   #endif 
  //   #ifdef ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID
  //   http_safemode_server->handleClient(); 
  //   #endif
  //   delay(1);

  //   if(llabs(millis()-tSaved_heartbeat)>1000)
  //   {
  //     tSaved_heartbeat = millis();
  //     Serial.println("Waiting for Recovery");
  //     Serial.print("Connected to ");
  //     Serial.println(ssid);
  //     Serial.print("IP address: ");
  //     Serial.println(WiFi.localIP());

  //     #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
  //     WDT_Reset();
  //     #endif

  //   }

  // }

}
#endif // if defined(ENABLE_DEVFEATURE_FASTBOOT_OTA_FALLBACK_DEFAULT_SSID) || (ENABLE_DEVFEATURE_FASTBOOT_HTTP_FALLBACK_DEFAULT_SSID)


int8_t mSupport::Tasker(uint8_t function, JsonParserObject obj)
{

  switch(function){
    case TASK_INIT:
      // fSendTemplatesOnce = true;

      #ifdef ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
        WiFi.begin(STA_SSID1, STA_PASS1);
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.println("");
        Serial.println("WiFi connected.");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
      #endif // ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT

      
      #ifdef ESP8266
        randomSeed(analogRead(0));  // also check adc module is not active AND pin not in use
      #else
        // randomSeed(analogRead(34)); //esp32
        // Do not use analogRead
      #endif

      #ifndef USE_MODULE_NETWORK_WIFI
      WiFi.mode(WIFI_OFF);
      #ifdef ESP32
      btStop();
      #endif // esp32
      #endif // USE_MODULE_NETWORK_WIFI

    break;
    case TASK_LOOP: {

    }break;
    case TASK_EVERY_SECOND:{

      /***
       * Scheduled reboot ticker
       */
      if(restart_delayed_seconds_ticks > 1) restart_delayed_seconds_ticks--;
      if(restart_delayed_seconds_ticks == 0) ESP_Restart_Safe();

#ifdef ENABLE_DEBUGFEATURE_RELAY__TEMP_FORCE_ON_FOR_5_MINS
// bool onoff = tkr_time->uptime_seconds_nonreset < 120 ? 1 : 0;
bool onoff = tkr_time->uptime_seconds_nonreset < 120 ? 0 : 1;
pinMode(26,OUTPUT);
digitalWrite(26,onoff);

#endif

      #ifdef ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
      #endif // ENABLE_DEVFEATURE__WIFI_TEST_START_IN_SUPPORT

      #ifdef ENABLE_DEBUGFEATURE_SENSORS__SPLASH_I2C_SCAN
      if(tkr_pins->PinUsed(GPIO_I2C_SCL_ID)&&tkr_pins->PinUsed(GPIO_I2C_SDA_ID))
      {
        char mqtt_data[300];
        tkr_sup->I2cScan(mqtt_data, sizeof(mqtt_data));
        ALOG_INF(PSTR("I2C_Scan=%s"), mqtt_data);
      }
      #endif // ENABLE_DEBUGFEATURE_SENSORS__SPLASH_I2C_SCAN

      #ifdef INSERT_CODE_EVERY_SECOND
      INSERT_CODE_EVERY_SECOND;
      #endif // INSERT_CODE_EVERY_SECOND

      #ifdef ENABLE_DEVFEATURE_DEBUG_TEMPLATE_LIGHTING_MQTT_SEND
      if(tkr_time->UpTime()>60)
      {
        if(tkr_mqtt->pubsub->connected())
        {
          tkr_mqtt->publish_device_P("debug/template_lighting", LIGHTING_TEMPLATE, false);
        }
      }
      #endif // ENABLE_DEVFEATURE_DEBUG_TEMPLATE_LIGHTING_MQTT_SEND

      PerformEverySecond();
    }break;
    case TASK_EVERY_MINUTE:

#ifdef ENABLE_DEBUGFEATURE_RELAY__TEMP_FORCE_ON_FOR_5_MINS
// bool onoff = tkr_time->uptime_seconds_nonreset < 120 ? 1 : 0;
//bool onoff = tkr_time->uptime_seconds_nonreset < 120 ? 0 : 1;
//pinMode(26,OUTPUT);
digitalWrite(26,!digitalRead(26));

#endif
    break;
    case TASK_ON_BOOT_SUCCESSFUL:

      #if defined(ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES)
      memset(&tkr->debug_module_time,0,sizeof(tkr->debug_module_time));
      ALOG_INF(PSTR("Reset: debug_module_time"));
      #endif // ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES

      
    #ifdef ENABLE_DEBUGFEATURE_LIGHTS__ESP32C3_FLICKER_TEST
    if(tkr_time->uptime_seconds_nonreset > 120)
    {
      // 1030 = mAnimatorLight, 1031 = mPalette, 1032 = mPlaylist, etc.
      static constexpr uint16_t kAllowList[] = {
        // Put light-related modules here to allow only these after warmup
        D_UNIQUE_MODULE_LIGHTS_ANIMATOR_ID /*mAnimatorLight*/, /*1031, 1032, ...*/
      };

      static constexpr uint16_t kBlockList[] = {
        // Or, leave ALLOW empty and put non-light modules here to skip them
        // 2001, 2002, ...
      };
      WiFi.mode(WIFI_OFF); // Disable WiFi to prevent flicker
      uint16_t id = mod->GetModuleUniqueID();
      // If ALLOW has entries, enforce allow-list.
      if (sizeof(kAllowList) / sizeof(kAllowList[0]) > 0) {
        if (!in_list(id, kAllowList)) {
          continue; // skip all non-allowed modules
        }
      } 
      // else
      // Otherwise, if BLOCK has entries, enforce block-list.
      // if (sizeof(kBlockList) / sizeof(kBlockList[0]) > 0) {
      //   if (in_list(id, kBlockList)) {
      //     continue; // skip blocked modules
      //   }
      // }
    }
    #endif
    

    break;

    case TASK_LOG__SHOW_UPTIME:      
      ALOG_INF(PSTR("TASK_LOG__SHOW_UPTIME Uptime %s"), tkr_time->GetUptime().c_str());
    break;

    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;

    case TASK_NETWORK_CONNECTED:
    case TASK_WIFI_CONNECTED:
      ArduinoOTAInit();
    break;
  }

  return FUNCTION_RESULT_UNKNOWN_ID;
  
}


// Use overloading to get variabel type
uint8_t getIdentifierID(uint8_t x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint8_t* x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint16_t x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint16_t* x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint32_t x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(uint32_t* x){ return IDENTIFIER_NUMBER_ID; }
uint8_t getIdentifierID(float x){ return IDENTIFIER_FLOAT_ID; }
uint8_t getIdentifierID(float* x){ return IDENTIFIER_FLOAT_ID; }
uint8_t getIdentifierID(char* x){ return IDENTIFIER_STRING_ID; }
uint8_t getIdentifierID(const char* x){ return IDENTIFIER_STRING_ID; }
    

void mSupport::AppendDList(char* buffer, const char* to_add){
  sprintf(buffer+strlen(buffer), "%s|", to_add);
}
void mSupport::AppendDList(char* buffer, uint16_t buflen, const char* formatP, ...)
{
  uint16_t length = strlen(buffer);
  if(length >= buflen){ return; }
  va_list arg;
  Serial.println(buffer);
  va_start(arg, formatP);  
  length += vsnprintf(buffer+length, buflen, formatP, arg);
  va_end(arg);  
  length += snprintf(buffer+length, buflen, "|");
}



#ifdef ESP32

#ifdef ENABLE_DEVFEATURE_ESP32__AUTO_MUTEX
/*********************************************************************************************\
 * ESP32 AutoMutex
\*********************************************************************************************/

//////////////////////////////////////////
// automutex.
// create a mute in your driver with:
// void *mutex = nullptr;
//
// then protect any function with
// TasAutoMutex m(&mutex, "somename");
// - mutex is automatically initialised if not already intialised.
// - it will be automagically released when the function is over.
// - the same thread can take multiple times (recursive).
// - advanced options m.give() and m.take() allow you fine control within a function.
// - if take=false at creat, it will not be initially taken.
// - name is used in serial log of mutex deadlock.
// - maxWait in ticks is how long it will wait before failing in a deadlock scenario (and then emitting on serial)
// class TasAutoMutex {
//   SemaphoreHandle_t mutex;
//   bool taken;
//   int maxWait;
//   const char *name;
//   public:
//     TasAutoMutex(SemaphoreHandle_t* mutex, const char *name = "", int maxWait = 40, bool take=true);
//     ~TasAutoMutex();
//     void give();
//     void take();
//     static void init(SemaphoreHandle_t* ptr);
// };
//////////////////////////////////////////

mSupport::TasAutoMutex::TasAutoMutex(SemaphoreHandle_t*mutex, const char *name, int maxWait, bool take) {
  if (mutex) {
    if (!(*mutex)){
      TasAutoMutex::init(mutex);
    }
    this->mutex = *mutex;
    this->maxWait = maxWait;
    this->name = name;
    if (take) {
      this->taken = xSemaphoreTakeRecursive(this->mutex, this->maxWait);
//      if (!this->taken){
//        Serial.printf("\r\nMutexfail %s\r\n", this->name);
//      }
    }
  } else {
    this->mutex = (SemaphoreHandle_t)nullptr;
  }
}

mSupport::TasAutoMutex::~TasAutoMutex() {
  if (this->mutex) {
    if (this->taken) {
      xSemaphoreGiveRecursive(this->mutex);
      this->taken = false;
    }
  }
}

void mSupport::TasAutoMutex::init(SemaphoreHandle_t* ptr) {
  SemaphoreHandle_t mutex = xSemaphoreCreateRecursiveMutex();
  (*ptr) = mutex;
  // needed, else for ESP8266 as we will initialis more than once in logging
//  (*ptr) = (void *) 1;
}

void mSupport::TasAutoMutex::give() {
  if (this->mutex) {
    if (this->taken) {
      xSemaphoreGiveRecursive(this->mutex);
      this->taken= false;
    }
  }
}

void mSupport::TasAutoMutex::take() {
  if (this->mutex) {
    if (!this->taken) {
      this->taken = xSemaphoreTakeRecursive(this->mutex, this->maxWait);
//      if (!this->taken){
//        Serial.printf("\r\nMutexfail %s\r\n", this->name);
//      }
    }
  }
}

#endif  // ESP32

#endif // mutex



#ifdef USE_ARDUINO_OTA

/*********************************************************************************************\
 * Allow updating via the Arduino OTA-protocol.
 *
 * - Once started disables current wifi clients and udp
 * - Perform restart when done to re-init wifi clients
\*********************************************************************************************/

void mSupport::ArduinoOTAInit(void)
{

  if(ota_init_success){ return; }

  ArduinoOTA.setHostname(tkr_set->runtime.my_hostname);
  
  ArduinoOTA.onStart([this]()
  {
    #ifdef ENABLE_DEVFEATURE_ARDUINOOTA__ADVANCED
      tkr_set->SettingsSave(1);    // Free flash for OTA update
      #ifdef USE_MODULE_NETWORK_WEBSERVER
        if (tkr_set->Settings.webserver) { tkr_web->StopWebserver(); }
      #endif  // USE_MODULE_NETWORK_WEBSERVER
      AllowInterrupts(0);
      if (tkr_set->Settings.flag_system.mqtt_enabled) {
        MqttDisconnect();      // SetOption3  - Enable MQTT
      }
    #endif

    ALOG_IMP(PSTR(D_LOG_UPLOAD "OTA " D_UPLOAD_STARTED));

    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;

    RtcSettings.boot_was_completed_ota_event = false;

    // Disable parts (e.g. RF receive interrupts) before starting update
    tkr->Tasker_Interface(TASK_UPDATE_OTA_BEFORE_ON_START);

    tkr_set->Settings.logging.serial_level = LOG_LEVEL_NONE; // Disable serial logging
    tkr_set->Settings.logging.web_level = LOG_LEVEL_NONE; // Disable web logging
    
    delay(200);       // Allow time for message xfer
  });


  ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total)
  {
    if (tkr_set->runtime.seriallog_level >= LOG_LEVEL_DEBUG) { // for when hardware serial is in use for modules
      uint8_t progress_now = (progress/(total/100));
      if(arduino_ota_progress_dot_count != progress_now){
        Serial.println(progress_now);
        arduino_ota_progress_dot_count = progress_now;
      }
    }

    #ifdef ENABLE_FEATURE_WATCHDOG_TIMER
    WDT_Reset();
    #endif
    
  });


  ArduinoOTA.onError([this](ota_error_t error)
  {
    /*
    From ArduinoOTA.h:
    typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
    */
    char error_str[30];
    memset(error_str,0,sizeof(error_str));

    switch (error) {
      case OTA_AUTH_ERROR:    strncpy_P(error_str, PSTR("OTA_AUTH_ERROR"), sizeof(error_str)); break;    
      case OTA_BEGIN_ERROR:   strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
      case OTA_CONNECT_ERROR: sprintf(error_str, PSTR("Connect Error")); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
      case OTA_END_ERROR:     strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
      default:
        snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
    }

    ALOG_IMP(PSTR(D_LOG_OTA "Arduino OTA  %s. %d " D_RESTARTING), error_str,ESP.getFreeSketchSpace());

    #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
    RtcSettings.boot_was_completed_ota_event = false; // Reset the flag as an error has occured
    #endif


    if(error != OTA_BEGIN_ERROR)
      ESP.restart(); // Should only reach if the first failed

  });

  ArduinoOTA.onEnd([this]()
  {
    #ifdef ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA
    RtcSettings.boot_was_completed_ota_event = true; // To enable skip of delayed WiFi start when this individual device has succesful flash
        
    if(ResetReasonPowerOn()) // Only if we can trust stability from a good restart
    {
      ALOG_INF(PSTR("Previous start was safe, so RTCSettingsSave trusted"));
      RtcSettingsSave();
    }
    #endif // ENABLE_DEVFEATURE_OTA__ENABLE_RECORD_BOOTREASON_IS_OTA

    delay(150); // Allow time for the OTA success response to reach the host

    ALOG_IMP(PSTR(D_LOG_UPLOAD "OTA " D_SUCCESSFUL ". " D_RESTARTING));
    Serial.flush();
    ESP.restart();
	});

  ArduinoOTA.begin();
  ota_init_success = true;

  ALOG_IMP(PSTR(D_LOG_OTA "Started"));

}


void mSupport::ArduinoOtaLoop(void)
{
  #ifdef ESP8266
  MDNS.update();
  #endif
  // if(!ota_init_success){ Serial.println("block OTA"); return; }
  ArduinoOTA.handle();
  // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
  // Note async stuff can still occur, so I need to disable them
  while (arduino_ota_triggered){ ArduinoOTA.handle(); }
}

#endif  // USE_ARDUINO_OTA


char* mSupport::float2CString(float number, unsigned char prec, char *s)
{
  dtostrf(number, 1, prec, s);
  return s;
}

#ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
char* mSupport::Unescape(char* buffer, uint32_t* size)
{
  uint8_t* read = (uint8_t*)buffer;
  uint8_t* write = (uint8_t*)buffer;
  int32_t start_size = *size;
  int32_t end_size = *size;
  uint8_t che = 0;

  //  ALOG_DBG(PSTR("DBG: UnescapeIn %*_H"), *size, (uint8_t*)buffer);

  while (start_size > 0) {
    uint8_t ch = *read++;
    start_size--;
    if (ch != '\\') {
      *write++ = ch;
    } else {
      if (start_size > 0) {
        uint8_t chi = *read++;
        start_size--;
        end_size--;
        switch (chi) {
          case '\\': che = '\\'; break;  // 5C Backslash
          case 'a': che = '\a'; break;   // 07 Bell (Alert)
          case 'b': che = '\b'; break;   // 08 Backspace
          case 'e': che = '\e'; break;   // 1B Escape
          case 'f': che = '\f'; break;   // 0C Formfeed
          case 'n': che = '\n'; break;   // 0A Linefeed (Newline)
          case 'r': che = '\r'; break;   // 0D Carriage return
          case 's': che = ' ';  break;   // 20 Space
          case 't': che = '\t'; break;   // 09 Horizontal tab
          case 'v': che = '\v'; break;   // 0B Vertical tab
          case 'x': {
            uint8_t* start = read;
            che = (uint8_t)strtol((const char*)read, (char**)&read, 16);
            start_size -= (uint16_t)(read - start);
            end_size -= (uint16_t)(read - start);
            break;
          }
          case '"': che = '\"'; break;   // 22 Quotation mark
//          case '?': che = '\?'; break;   // 3F Question mark
          default : {
            che = chi;
            *write++ = ch;
            end_size++;
          }
        }
        *write++ = che;
      }
    }
  }
  *size = end_size;
  *write++ = 0;   // add the end string pointer reference
//  ALOG_DBG(PSTR("DBG: UnescapeOut %*_H"), *size, (uint8_t*)buffer);

  return buffer;
}

char* mSupport::RemoveSpace(char* p) {
  // Remove white-space character (' ','\t','\n','\v','\f','\r')
  char* write = p;
  char* read = p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (!isspace(ch)) {
      *write++ = ch;
    }
  }
  return p;
}


// remove spaces at the beginning and end of the string (but not in the middle)
char* mSupport::TrimSpace(char *p) {
  // Remove white-space character (' ','\t','\n','\v','\f','\r')
  char* write = p;
  char* read = p;
  char ch = '.';

  // skip all leading spaces
  while (isspace(*read)) {
    read++;
  }
  // copy the rest
  do {
    ch = *read++;
    *write++ = ch;
  } while (ch != '\0');
  // move to end
  read = p + strlen(p);
  // move backwards
  while (p != read) {
    read--;
    if (isspace(*read)) {
      *read = '\0';
    } else {
      break;
    }
  }
  return p;
}


char* mSupport::RemoveControlCharacter(char* p) {
  // Remove control character (0x00 .. 0x1F and 0x7F)
  char* write = p;
  char* read = p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (!iscntrl(ch)) {
      *write++ = ch;
    }
  }
  *write++ = '\0';
  return p;
}


char* mSupport::ReplaceChar(char* p, char find, char replace) {
  char* write = (char*)p;
  char* read = (char*)p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (ch == find) {
      ch = replace;
    }
    *write++ = ch;
  }
  return p;
}


char* mSupport::ReplaceCommaWithDot(char* p) {
  return ReplaceChar(p, ',', '.');
}
#endif


#ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
uint8_t mSupport::GetNormalDistributionRandom(uint8_t mean, uint8_t standard_deviation, uint8_t constrained_min, uint8_t constrained_max){

  std::random_device rd{};
  std::mt19937 gen{rd()};
  uint8_t result = 0;

  // values near the mean are the most likely
  // standard deviation affects the dispersion of generated values from the mean
  std::normal_distribution<> d{mean,standard_deviation};

  // std::map<int, int> hist{};
  // uint32_t array[10] = {0};
  // for(int n=0; n<10000; ++n) {
  //     array[round(d(gen))]++;
  // }

  result = round(d(gen));

  // If these are not equal, then apply contraint
  if(constrained_min != constrained_max){
    result = constrain(result, constrained_min, constrained_max);
  }

  return result; 

}
#endif

char mSupport::GetVersionBranchTypeCharNameByID(uint8_t id){
  switch(id){
    case FIRMWARE_VERSION_TYPE_RELEASE_ID : return 'R' ;
    case FIRMWARE_VERSION_TYPE_RELEASE_CANDIDATE_ID : return 'C' ;     
    case FIRMWARE_VERSION_TYPE_BETA_ID : return 'B' ;
    case FIRMWARE_VERSION_TYPE_DEVELOPING_ID : return 'D' ;
  }
}


void mSupport::init_FirmwareVersion()
{

  // Version Current
  tkr_set->runtime.firmware_version.current.part_branch = (PROJECT_VERSION >> 30) & 0x03;
  tkr_set->runtime.firmware_version.current.part_major =  (PROJECT_VERSION >> 24) & 0x3F;
  tkr_set->runtime.firmware_version.current.part_minor =  (PROJECT_VERSION >> 16) & 0xff;
  tkr_set->runtime.firmware_version.current.part_system = (PROJECT_VERSION >> 8 ) & 0xff;
  tkr_set->runtime.firmware_version.current.part_module = (PROJECT_VERSION      ) & 0xff;
  tkr_set->runtime.firmware_version.current.number =       PROJECT_VERSION;

  // Display Version Output
  // DEBUG_PRINTF("firmware_version.current = %X\n\r",PROJECT_VERSION);
    
  // Create ascii version name
  sprintf_P(tkr_set->runtime.firmware_version.current.name_ctr,PSTR("%c%d.%d.%d.%d"),
      tkr_sup->GetVersionBranchTypeCharNameByID(tkr_set->runtime.firmware_version.current.part_branch),
      tkr_set->runtime.firmware_version.current.part_major,
      tkr_set->runtime.firmware_version.current.part_minor,
      tkr_set->runtime.firmware_version.current.part_system,
      tkr_set->runtime.firmware_version.current.part_module
  );

}


float mSupport::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


// checks for 0 before dividing
int32_t mSupport::safeDivideInt(int32_t num, int32_t den)
{
  return (num!=0)?num/den:0;
}


float mSupport::roundfloat(float in, uint8_t dec){
  //float f = round(in*(10*dec))/(10*dec);
  float f = round(in*(10*dec));
  f /= (10*dec);///100;
  return f;
}


int mSupport::mSearchCtrIndexOf(const char* toSearch, const char* toFind){

  char *p = strstr(toSearch,toFind);

  if(p != NULL){
    return abs(toSearch - p); // get the position
  }else{
    return -1; //if null, it doesnt exist
  }

}


uint16_t mSupport::NumCtr2Num(char* numctr, uint8_t numberlength){

  uint16_t numout=0;

  for(int digit=0;digit<numberlength;digit++){
    numout += (numctr[digit]-48)*pow(10,numberlength-digit-1);
  }

  return numout;
}


uint16_t mSupport::WriteBuffer_P(char* buffer, const char* formatP, ...)     // Content send snprintf_P char data
{
  va_list arg;
  va_start(arg, formatP);
  int length = vsnprintf_P(buffer, DATA_BUFFER_PAYLOAD_MAX_LENGTH, formatP, arg);
  va_end(arg);
  return length;
}

void mSupport::WriteBuffer_P(char* buffer, uint16_t* length, const char* formatP, ...)     // Content send snprintf_P char data
{
  va_list arg;
  va_start(arg, formatP);
  *length += vsnprintf_P(&buffer[*length], DATA_BUFFER_PAYLOAD_MAX_LENGTH, formatP, arg);
  va_end(arg);
  return;
}


// Assumes primary buffer
uint16_t mSupport::WriteBuffer_P(const char* formatP, ...)     // Content send snprintf_P char data
{
  va_list arg;
  va_start(arg, formatP);
  char* buffer = data_buffer.payload.ctr;
  uint16_t length = strlen(buffer);
  uint16_t bytes_written = vsnprintf_P(&buffer[length], DATA_BUFFER_PAYLOAD_MAX_LENGTH-length, formatP, arg);
  va_end(arg);
  return bytes_written;
}

void mSupport::appendToBuffer(char* buffer, size_t bufferSize, const char* format, ...) 
{
    // Find the current length of the buffer (to append)
    size_t currentLength = strlen(buffer);

    // Create a va_list to handle the variable arguments
    va_list args;
    va_start(args, format);

    // Use vsnprintf to append to the buffer, taking into account the current length
    vsnprintf(buffer + currentLength, bufferSize - currentLength, format, args);

    // End the argument list
    va_end(args);
}

void mSupport::removeTrailingComma(char* buffer) {
    size_t len = strlen(buffer);

    // Check if the buffer ends with ", " (comma and space)
    if (len >= 2 && strcmp(&buffer[len - 2], ", ") == 0) {
        buffer[len - 2] = '\0';  // Remove the last ", "
    }
    // Otherwise, check if the buffer ends with ","
    else if (len >= 1 && buffer[len - 1] == ',') {
        buffer[len - 1] = '\0';  // Remove the last ","
    }
}



int mSupport::Response_P(const char* format, ...)     // Content send snprintf_P char data
{
  //BufferWriter
  data_buffer.ClearDeep();

  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int len = vsnprintf_P(data_buffer.payload.ctr, sizeof(data_buffer.payload.ctr), format, args);
  va_end(args);
  return len;
}


int mSupport::ResponseAppend_P(const char* format, ...)  // Content send snprintf_P char data
{
  // This uses char strings. Be aware of sending %% if % is needed
  va_list args;
  va_start(args, format);
  int mlen = strlen(data_buffer.payload.ctr);
  int len = vsnprintf_P(data_buffer.payload.ctr + mlen, sizeof(data_buffer.payload.ctr) - mlen, format, args);
  va_end(args);
  return len + mlen;
}


uint32_t mSupport::ResetReason(void)
{
  /*
    user_interface.h
    REASON_DEFAULT_RST      = 0,  // "Power on"                normal startup by power on
    REASON_WDT_RST          = 1,  // "Hardware Watchdog"       hardware watch dog reset
    REASON_EXCEPTION_RST    = 2,  // "Exception"               exception reset, GPIO status won’t change
    REASON_SOFT_WDT_RST     = 3,  // "Software Watchdog"       software watch dog reset, GPIO status won’t change
    REASON_SOFT_RESTART     = 4,  // "Software/System restart" software restart ,system_restart , GPIO status won’t change
    REASON_DEEP_SLEEP_AWAKE = 5,  // "Deep-Sleep Wake"         wake up from deep-sleep
    REASON_EXT_SYS_RST      = 6   // "External System"         external system reset
  */
 
#ifdef ESP8266
  return resetInfo.reason;
#else
  return 0;
#endif// ESP8266

}

String mSupport::GetResetReason(void)
{
  #ifdef ESP8266
  // char buff[32];
  // if (oswatch_blocked_loop) {
  //   strncpy_P(buff, PSTR(D_BLOCKED_LOOP), sizeof(buff));
  //   return String(buff);
  // } else {
    return ESP.getResetReason();
  // }
  #else
    return PSTR("Unsupported");
  #endif
}

// const char* mSupport::GetResetReason(char* buffer, uint8_t buflen)
// {
//   #ifdef ESP8266
//   // if (oswatch_blocked_loop) {
//   //   strncpy_P(buffer, PSTR(D_BLOCKED_LOOP), buflen);
//   // } else {
//     sprintf(buffer, "%s", ESP.getResetReason().c_str());
//   // }
//   #else
//     sprintf(buffer, "%s", "Unsupported");
//   #endif
//   return buffer;
// }
#include "mSupport.h"

#ifdef ESP32
  #include "esp_system.h"   // esp_reset_reason_t, esp_reset_reason()
#endif

const char* mSupport::GetResetReason(char* buffer, uint8_t buflen)
{
  if (!buffer || buflen == 0) {
    return "";
  }
  buffer[0] = '\0';

#ifdef ESP8266

  // ESP8266: already provides a human-readable string
  // (Example outputs: "Power On", "External System", "Exception", "Software Watchdog", etc.)
  snprintf(buffer, buflen, "%s", ESP.getResetReason().c_str());
  buffer[buflen - 1] = '\0';
  return buffer;

#elif defined(ESP32)

  // ESP32: enum reset reason
  const esp_reset_reason_t reason = esp_reset_reason();

  // Map enum to a stable short code + readable string.
  // Keep strings short so you can use them as tags/codes in MQTT.
  switch (reason)
  {
    case ESP_RST_UNKNOWN:   snprintf(buffer, buflen, "Unknown"); break;
    case ESP_RST_POWERON:   snprintf(buffer, buflen, "PowerOn"); break;
    case ESP_RST_EXT:       snprintf(buffer, buflen, "ExternalReset"); break;
    case ESP_RST_SW:        snprintf(buffer, buflen, "SoftwareReset"); break;
    case ESP_RST_PANIC:     snprintf(buffer, buflen, "Panic"); break;
    case ESP_RST_INT_WDT:   snprintf(buffer, buflen, "IntWDT"); break;
    case ESP_RST_TASK_WDT:  snprintf(buffer, buflen, "TaskWDT"); break;
    case ESP_RST_WDT:       snprintf(buffer, buflen, "OtherWDT"); break;
    case ESP_RST_DEEPSLEEP: snprintf(buffer, buflen, "DeepSleep"); break;
    case ESP_RST_BROWNOUT:  snprintf(buffer, buflen, "Brownout"); break;
    case ESP_RST_SDIO:      snprintf(buffer, buflen, "SDIO"); break;

    // Some ESP-IDF / core versions have extra reasons. Guard if your compile complains.
    #ifdef ESP_RST_USB
    case ESP_RST_USB:       snprintf(buffer, buflen, "USB"); break;
    #endif
    #ifdef ESP_RST_JTAG
    case ESP_RST_JTAG:      snprintf(buffer, buflen, "JTAG"); break;
    #endif

    default:
      snprintf(buffer, buflen, "Unknown(%d)", (int)reason);
      break;
  }

  buffer[buflen - 1] = '\0';
  return buffer;

#else

  snprintf(buffer, buflen, "Unsupported");
  buffer[buflen - 1] = '\0';
  return buffer;

#endif
}


bool mSupport::ResetReasonPowerOn(void) {
  uint32_t reset_reason = mSupportHardware::ESP_ResetInfoReason();
  return ((reset_reason == REASON_DEFAULT_RST) || (reset_reason == REASON_EXT_SYS_RST));
}



char* mSupport::GetStateText(uint32_t state)
{
  if (state >= MAX_STATE_TEXT) {
    state = 1;
  }
  return tkr_set->SettingsText(SET_STATE_TXT1 + state);
}



char* mSupport::GetPowerDevice(char* dest, uint32_t idx, size_t size, uint32_t option)
{
  #ifdef USE_MODULE_DRIVERS_RELAY
  strncpy(dest, "POWER", size);                // POWER
  if ((tkr_relay->rt.devices_present + option) > 1) {
    char sidx[8];
    snprintf_P(sidx, sizeof(sidx), PSTR("%d"), idx);  // x
    strncat(dest, sidx, size - strlen(dest) -1);      // POWERx
  }
  return dest;
  #else
  return dest;
  #endif

}

char* mSupport::GetPowerDevice(char* dest, uint32_t idx, size_t size)
{
  return GetPowerDevice(dest, idx, size, 0);
}


void mSupport::SetPulseTimer(uint32_t index, uint32_t time)
{
  tkr_set->runtime.pulse_timer[index] = (time > 111) ? millis() + (1000 * (time - 100)) : (time > 0) ? millis() + (100 * time) : 0L;
}

uint32_t mSupport::GetPulseTimer(uint32_t index)
{
  long time = tkr_time->TimePassedSince(tkr_set->runtime.pulse_timer[index]);
  if (time < 0) {
    time *= -1;
    return (time > 11100) ? (time / 1000) + 100 : (time > 0) ? time / 100 : 0;
  }
  return 0;
}


#ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
bool mSupport::OsWatchBlockedLoop(void)
{
  //return oswatch_blocked_loop;
}
#endif



/*********************************************************************************************\
 * Zero-cross support
\*********************************************************************************************/

//#define DEBUG_ZEROCROSS

#ifdef ENABLE_FEATURE_POWER__ZERO_CROSS_DETECTION

void ZeroCrossMomentStart(void) {
  if (!TasmotaGlobal.zc_interval) { return; }

#ifdef DEBUG_ZEROCROSS
  uint32_t dbg_interval = TasmotaGlobal.zc_interval;
  uint32_t dbg_zctime = TasmotaGlobal.zc_time;
  uint32_t dbg_starttime = micros();
#endif

  uint32_t trigger_moment = TasmotaGlobal.zc_time + TasmotaGlobal.zc_interval - TasmotaGlobal.zc_offset - TasmotaGlobal.zc_code_offset;
  while (TimeReachedUsec(trigger_moment)) {    // Trigger moment already passed so try next
    trigger_moment += TasmotaGlobal.zc_interval;
  }
  while (!TimeReachedUsec(trigger_moment)) {}  // Wait for trigger moment

#ifdef DEBUG_ZEROCROSS
  uint32_t dbg_endtime = micros();
  AddLog(LOG_LEVEL_DEBUG, PSTR("ZCD: CodeExecTime %d, StartTime %u, EndTime %u, ZcTime %u, Interval %d"),
    dbg_endtime - dbg_starttime, dbg_starttime, dbg_endtime, dbg_zctime, dbg_interval);
#endif

  TasmotaGlobal.zc_code_offset = micros();
}

void ZeroCrossMomentEnd(void) {
  if (!TasmotaGlobal.zc_interval) { return; }
  TasmotaGlobal.zc_code_offset = (micros() - TasmotaGlobal.zc_code_offset) / 2;

#ifdef DEBUG_ZEROCROSS
  AddLog(LOG_LEVEL_DEBUG, PSTR("ZCD: CodeExecTime %d"), TasmotaGlobal.zc_code_offset * 2);
#endif
}

void IRAM_ATTR ZeroCrossIsr(void);
void ZeroCrossIsr(void) {
  uint32_t time = micros();
  TasmotaGlobal.zc_interval = ((int32_t) (time - TasmotaGlobal.zc_time));
  TasmotaGlobal.zc_time = time;
}

void ZeroCrossInit(uint32_t offset) {
  if (PinUsed(GPIO_ZEROCROSS)) {
    TasmotaGlobal.zc_offset = offset;

    uint32_t gpio = Pin(GPIO_ZEROCROSS);
    pinMode(gpio, INPUT_PULLUP);
    attachInterrupt(gpio, ZeroCrossIsr, CHANGE);

    AddLog(LOG_LEVEL_INFO, PSTR("ZCD: Activated"));  // Zero-cross detection activated
  }
}

#endif // ENABLE_FEATURE_POWER__ZERO_CROSS_DETECTION



//Get span until single character in string
size_t mSupport::strchrspn(const char *str1, int character)
{
  size_t ret = 0;
  char *start = (char*)str1;
  char *end = strchr(str1, character);
  if (end) ret = end - start;
  return ret;
}


char* mSupport::LowerCase(char* dest, const char* source)
{
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    *write++ = tolower(ch);
  }
  return dest;
}


char* mSupport::UpperCase(char* dest, const char* source)
{
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    *write++ = toupper(ch);
  }
  return dest;
}


char* mSupport::UpperCase_P(char* dest, const char* source)
{
  char* write = dest;
  const char* read = source;
  char ch = '.';

  while (ch != '\0') {
    ch = pgm_read_byte(read++);
    *write++ = toupper(ch);
  }
  return dest;
}


char* mSupport::Trim(char* p)
{
  // while ((*p != '\0') && isblank(*p)) { p++; }  // Trim leading spaces
  // char* q = p + strlen(p) -1;
  // while ((q >= p) && isblank(*q)) { q--; }   // Trim trailing spaces
  // q++;
  // *q = '\0';
  // return p;
  if (*p != '\0') {
    while ((*p != '\0') && isblank(*p)) { p++; }  // Trim leading spaces
    char* q = p + strlen(p) -1;
    while ((q >= p) && isblank(*q)) { q--; }   // Trim trailing spaces
    q++;
    *q = '\0';
  }
  return p;
}


bool mSupport::ValidIpAddress(const char* str)
{
  const char* p = str;

  while (*p && ((*p == '.') || ((*p >= '0') && (*p <= '9')))) { p++; }
  return (*p == '\0');
}

bool mSupport::ParseIPv4(uint32_t* addr, const char* str)
{

  // #Issue: PSTR with stroul will crash on esp8266
  char buf[16];  // IPv4 addresses are at most 15 characters plus null terminator.
  strcpy_P(buf, str);  // Copy string from flash (PSTR) to RAM
  
  uint8_t *part = (uint8_t*)addr;
  uint8_t i;
  
  *addr = 0;
  for (i = 0; i < 4; i++) {
    part[i] = strtoul(buf, nullptr, 10);        // Convert byte
    str = strchr(buf, '.');
    if (str == nullptr || *str == '\0') {
      break;  // No more separators, exit
    }
    str++;                                   // Point to next character after separator
  }
  
  return (3 == i);
}


#ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
// Function to parse & check if version_str is newer than our currently installed version.
bool mSupport::NewerVersion(char* version_str)
{
  uint32_t version = 0;
  uint8_t i = 0;
  char *str_ptr;
  char* version_dup = strdup(version_str);  // Duplicate the version_str as strtok_r will modify it.

  if (!version_dup) {
    return false;  // Bail if we can't duplicate. Assume bad.
  }
  // Loop through the version string, splitting on '.' seperators.
  for (char *str = strtok_r(version_dup, ".", &str_ptr); str && i < sizeof(PROJECT_VERSION); str = strtok_r(nullptr, ".", &str_ptr), i++) {
    int field = atoi(str);
    // The fields in a version string can only range from 0-255.
    if ((field < 0) || (field > 255)) {
      free(version_dup);
      return false;
    }
    // Shuffle the accumulated bytes across, and add the new byte.
    version = (version << 8) + field;
    // Check alpha delimiter after 1.2.3 only
    if ((2 == i) && isalpha(str[strlen(str)-1])) {
      field = str[strlen(str)-1] & 0x1f;
      version = (version << 8) + field;
      i++;
    }
  }
  free(version_dup);  // We no longer need this.
  // A version string should have 2-4 fields. e.g. 1.2, 1.2.3, or 1.2.3a (= 1.2.3.1).
  // If not, then don't consider it a valid version string.
  if ((i < 2) || (i > sizeof(PROJECT_VERSION))) {
    return false;
  }
  // Keep shifting the parsed version until we hit the maximum number of tokens.
  // PROJECT_VERSION stores the major number of the version in the most significant byte of the uint32_t.
  while (i < sizeof(PROJECT_VERSION)) {
    version <<= 8;
    i++;
  }
  // Now we should have a fully constructed version number in uint32_t form.
  return (version > PROJECT_VERSION);
}
#endif


char* mSupport::GetTextIndexed_P(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);  //Makes sure byte is the right size
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}

char* mSupport::GetTextIndexed(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{

  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
      // Serial.printf("index=%d\n\r", index);
    while ((ch != '\0') && (ch != '|')) {

      // (addr) (*(const uint8_t *)(addr))

      ch = *read; //get vlaue from pointer
      read++; // move pointer forward

      // Serial.print(ch);

      // ch = pgm_read_byte(read++);  //pads

      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}


char* mSupport::GetTextIndexedTemp(char* destination, size_t destination_size, uint16_t index, const char* haystack)
{
  // Returns empty string if not found
  // Returns text of found
  char* write = destination;
  const char* read = haystack;

  index++;
  uint16_t index_hay = 0;
  while (index--) {
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {

      // (addr) (*(const uint8_t *)(addr))

      ch = read[index_hay++];  //pads
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    if (0 == ch) {
      if (index) {
        write = destination;
      }
      break;
    }
  }
  *write = '\0';
  return destination;
}




/*

 * I also want to check if the entire thing matches, ie "one|two" should allow "one", "two" AND "one|two"
 */
int16_t mSupport::GetCommandID16_P(const char* needle, const char* haystack, char* destination, size_t destination_size)
{
  if((needle == nullptr)||(haystack == nullptr)) return false;
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;
  //tmp fix, internal buffer to be removed
  if(destination == nullptr){
    // AddLog(LOG_LEVEL_WARN, PSTR("Should this be removed?? destination == nullptr"));
    char buffer_tmp[50];
    destination = buffer_tmp;
    destination_size = 50;
  }
  
  /**
   * @brief First check the whole message for a match (ie "A|B" will match when multiple commands are used in template)
   * check for exact WHOLE match
   * 
   * This may cause stability errors
   **/
  if(!strcasecmp_P(needle, haystack)) { //works v#.103.#.#
    // ALOG_INF(PSTR("CHECKING WHOLE Should this be removed?? destination == nullptr"));
    // delay(5000);
    return 1;
  }

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;

}

/*

 * I also want to check if the entire thing matches, ie "one|two" should allow "one", "two" AND "one|two"
 */
int16_t mSupport::GetCommandID16_MultipleSubMatches_P(const char* needle, const char* haystack, char* destination, size_t destination_size)
{
    if ((needle == nullptr) || (haystack == nullptr)) return -1;
    
    char buffer_tmp[100];  // Temporary buffer if destination is not provided
    if (destination == nullptr) {
        destination = buffer_tmp;
        destination_size = sizeof(buffer_tmp);
    }

    int segmentIndex = -1;  // Index of the segment containing a match
    const char* read = haystack;
    char* write = destination;
    int count = 0;  // Segment counter

    while (true) {
        char ch = pgm_read_byte(read);
        if (ch == '\0' || ch == '|') {
            *write = '\0';  // Null-terminate the current segment
            
            // Check if any part of the segment matches the needle
            char* part = strtok(destination, "/");
            while (part != NULL) {
                if (strstr(part, needle) != NULL) {  // Using strstr to find the needle as a substring
                    segmentIndex = count;
                    break;
                }
                part = strtok(NULL, "/");
            }

            if (segmentIndex != -1 || ch == '\0') {
                break;  // If match found or end of haystack
            }

            count++;  // Move to the next segment
            write = destination;  // Reset write pointer for the next segment
            read++;  // Skip the '|' character
        } else {
            if (write < destination + destination_size - 1) {
                *write++ = ch;  // Copy character to destination
            }
            read++;  // Move read pointer
        }
    }

    return segmentIndex;  // Return the index of the segment containing the match or -1
}


int mSupport::GetCommandCode(char* destination, size_t destination_size, const char* needle, const char* haystack)
{
  // Returns -1 of not found
  // Returns index and command if found
  int result = -1;
  const char* read = haystack;
  char* write = destination;

  while (true) {
    result++;
    size_t size = destination_size -1;
    write = destination;
    char ch = '.';
    while ((ch != '\0') && (ch != '|')) {
      ch = pgm_read_byte(read++);
      if (size && (ch != '|'))  {
        *write++ = ch;
        size--;
      }
    }
    *write = '\0';
    if (!strcasecmp(needle, destination)) {
      break;
    }
    if (0 == ch) {
      result = -1;
      break;
    }
  }
  return result;
}


float mSupport::CharToFloat(const char *str)
{
  // simple ascii to double, because atof or strtod are too large
  char strbuf[24];

  strlcpy(strbuf, str, sizeof(strbuf));
  char *pt = strbuf;
  if (*pt == '\0') { return 0.0f; }

  while ((*pt != '\0') && isspace(*pt)) { pt++; }  // Trim leading spaces

  signed char sign = 1;
  if (*pt == '-') { sign = -1; }
  if (*pt == '-' || *pt == '+') { pt++; }          // Skip any sign

  float left = 0;
  if (*pt != '.') {
    left = atoi(pt);                               // Get left part
    while (isdigit(*pt)) { pt++; }                 // Skip number
  }

  float right = 0;
  if (*pt == '.') {
    pt++;
    uint32_t max_decimals = 0;
    while ((max_decimals < 8) && isdigit(pt[max_decimals])) { max_decimals++; }
    pt[max_decimals] = '\0';                       // Limit decimals to float max of 8
    right = atoi(pt);                              // Decimal part
    while (isdigit(*pt)) {
      pt++;
      right /= 10.0f;
    }
  }

  float result = left + right;
  if (sign < 0) {
    return -result;                                // Add negative sign
  }
  return result;
}

char* mSupport::ReplaceChar(char* p, char find, char replace) {
  char* write = (char*)p;
  char* read = (char*)p;
  char ch = '.';

  while (ch != '\0') {
    ch = *read++;
    if (ch == find) {
      ch = replace;
    }
    *write++ = ch;
  }
  return p;
}

char* mSupport::ReplaceCommaWithDot(char* p) {
  return ReplaceChar(p, ',', '.');
}


int8_t mSupport::GetStateNumber(const char *state_text)
{
  char command[50];
  int8_t state_number = STATE_NUMBER_INVALID_ID;

  if (GetCommandCode(command, sizeof(command), state_text, kOptionOff) >= 0) {
    state_number = STATE_NUMBER_OFF_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionOn) >= 0) {
    state_number = STATE_NUMBER_ON_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionToggle) >= 0) {
    state_number = STATE_NUMBER_TOGGLE_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionBlink) >= 0) {
    state_number = STATE_NUMBER_BLINK_ID;
  }else 
  if (GetCommandCode(command, sizeof(command), state_text, kOptionBlinkOff) >= 0) {
    state_number = STATE_NUMBER_BLINK_OFF_ID;
  } else
  if (GetCommandCode(command, sizeof(command), state_text, kOptionIncrement) >= 0) {
    state_number = STATE_NUMBER_INCREMENT_ID;
  } else
  if (GetCommandCode(command, sizeof(command), state_text, kOptionDecrement) >= 0) {
    state_number = STATE_NUMBER_DECREMENT_ID;
  } else  
  if (GetCommandCode(command, sizeof(command), state_text, kOptionFollow) >= 0) {
    state_number = STATE_NUMBER_FOLLOW_ID;
  } else  
  if (GetCommandCode(command, sizeof(command), state_text, kOptionFollowInv) >= 0) {
    state_number = STATE_NUMBER_FOLLOW_INV_ID;
  } else  
  { // c_str to number
    state_number = (!strlen(state_text)) ? 0 : atoi(state_text);
  }

  #ifdef ENABLE_LOG_LEVEL_INFO
  AddLog(LOG_LEVEL_DEBUG,PSTR("%d=GetStateNumber(%s)"),state_number, state_text);
  #endif// ENABLE_LOG_LEVEL_INFO

  return state_number;

}


char* mSupport::GetState_Name_by_ID(uint8_t id, char* buffer, uint8_t buflen) 
{
  switch(id)
  {
    default:
    case STATE_NUMBER_OFF_ID:         tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionOff); break;
    case STATE_NUMBER_ON_ID:          tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionOn); break;
    case STATE_NUMBER_TOGGLE_ID:      tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionToggle); break;
    case STATE_NUMBER_BLINK_ID:       tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionBlink); break;
    case STATE_NUMBER_BLINK_OFF_ID:   tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionBlinkOff); break;
    case STATE_NUMBER_INCREMENT_ID:   tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionIncrement); break;
    case STATE_NUMBER_DECREMENT_ID:   tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionDecrement); break;
    case STATE_NUMBER_FOLLOW_ID:      tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionFollow); break;
    case STATE_NUMBER_FOLLOW_INV_ID:  tkr_sup->GetTextIndexed_P(buffer, buflen, 0, kOptionFollowInv); break;
  }
  return buffer;
}


#ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
void mSupport::SetSerialBaudrate(int baudrate)
{
  // tkr_set->Settings.baudrate = baudrate / 1200;
  // if (Serial.baudRate() != baudrate) {
  //   //if (seriallog_level) {
  //     AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SET_BAUDRATE_TO " %d"), baudrate);
  //   //}
  //   delay(100);
  //   Serial.flush();
  //   //Serial.begin(baudrate, serial_config);
  //   delay(10);
  //   Serial.println();
  // }
}



void mSupport::SerialSendRaw(char *codes)
{
  char *p;
  char stemp[3];
  uint8_t code;

  int size = strlen(codes);

  while (size > 0) {
    strlcpy(stemp, codes, sizeof(stemp));
    code = strtol(stemp, &p, 16);
    Serial.write(code);
    size -= 2;
    codes += 2;
  }
}


uint32_t mSupport::GetHash(const char *buffer, size_t size)
{
  uint32_t hash = 0;
  for (uint16_t i = 0; i <= size; i++) {
    hash += (uint8_t)*buffer++ * (i +1);
  }
  return hash;
}


#endif

void mSupport::ClaimSerial(void)
{
  // serial_local = true;
  ALOG_INF(PSTR("SNS: Hardware Serial"));
  // SetSeriallog(LOG_LEVEL_NONE);
  // baudrate = Serial.baudRate();
  // Settings.baudrate = baudrate / 1200;
}


void mSupport::ShowSource(int source)
{
  if ((source > 0) && (source < SRC_MAX)) {
    char stemp1[20];
    ALOG_INF(PSTR("SRC: %s"), GetTextIndexed_P(stemp1, sizeof(stemp1), source, kCommandSource));
  }
}


void mSupport::SleepDelay(uint32_t mseconds) {
  if (mseconds) {
    for (uint32_t wait = 0; wait < mseconds; wait++) {
      delay(1);
      /**
       * @brief Check Serial inputs before before overrun
       **/
      if (Serial.available()) { break; }
    }
  } else {
    delay(0);
  }
}


/*********************************************************************************************\
 * State loops
\*********************************************************************************************/
/*-------------------------------------------------------------------------------------------*\
 * Every second
\*-------------------------------------------------------------------------------------------*/


void mSupport::PerformEverySecond(void)
{

  // if (POWER_CYCLE_TIME == tkr_set->runtime.uptime) {
  //   UpdateQuickPowerCycle(false);
  // }

  CheckResetConditions(); //If restart is ordered, of type reset, save network, erase settings, set defaults, reload network, complete restart

  #ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
  tkr_set->runtime.uptime++;

  if (POWER_CYCLE_TIME == tkr_set->runtime.uptime) {
    UpdateQuickPowerCycle(false);
  }

  if (BOOT_LOOP_TIME == tkr_set->runtime.uptime) {
    RtcRebootReset();

    Settings->last_module = Settings->module;

#ifdef USE_DEEPSLEEP
    if (!(DeepSleepEnabled() && !Settings->flag3.bootcount_update)) {  // SetOption76  - (Deepsleep) Enable incrementing bootcount (1) when deepsleep is enabled
#endif
      Settings->bootcount++;              // Moved to here to stop flash writes during start-up
      AddLog(LOG_LEVEL_DEBUG, PSTR(D_LOG_APPLICATION D_BOOT_COUNT " %d"), Settings->bootcount);
#ifdef USE_DEEPSLEEP
    }
#endif
  }

  if (tkr_set->runtime.power_on_delay) {
    if (1 == Settings->param[P_POWER_ON_DELAY2]) {       // SetOption47 1
      // Allow relay power on once network is available
      if (!tkr_set->runtime.global_state.network_down) {
        tkr_set->runtime.power_on_delay = 0;
      }
    }
    else if (2 == Settings->param[P_POWER_ON_DELAY2]) {  // SetOption47 2
      // Allow relay power on once mqtt is available
      if (!tkr_set->runtime.global_state.mqtt_down) {
        tkr_set->runtime.power_on_delay = 0;
      }
    }
    else {                                               // SetOption47 3..255
      // Allow relay power on after x seconds
      tkr_set->runtime.power_on_delay--;
    }
    if (!tkr_set->runtime.power_on_delay && tkr_set->runtime.power_on_delay_state) {
      // Set relays according to last SetDevicePower() request
      SetDevicePower(tkr_set->runtime.power_on_delay_state, SRC_SO47);
    }
  }

  if (tkr_set->runtime.mqtt_cmnd_blocked_reset) {
    tkr_set->runtime.mqtt_cmnd_blocked_reset--;
    if (!tkr_set->runtime.mqtt_cmnd_blocked_reset) {
      tkr_set->runtime.mqtt_cmnd_blocked = 0;             // Clean up MQTT cmnd loop block
    }
  }

  if (tkr_set->runtime.seriallog_timer) {
    tkr_set->runtime.seriallog_timer--;
    if (!tkr_set->runtime.seriallog_timer) {
      if (tkr_set->runtime.seriallog_level) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SERIAL_LOGGING_DISABLED));
      }
      tkr_set->runtime.seriallog_level = 0;
    }
  }

  if (tkr_set->runtime.syslog_timer) {  // Restore syslog level
    tkr_set->runtime.syslog_timer--;
    if (!tkr_set->runtime.syslog_timer) {
      tkr_set->runtime.syslog_level = Settings->syslog_level;
      if (Settings->syslog_level) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION D_SYSLOG_LOGGING_REENABLED));  // Might trigger disable again (on purpose)
      }
    }
  }

  MqttPublishLoggingAsync(false);
#ifdef SYSLOG_UPDATE_SECOND
  SyslogAsync(false);
#endif  // SYSLOG_UPDATE_SECOND

  ResetGlobalValues();

  if ((tkr_set->runtime.init_state >= INIT_GPIOS) && PinUsed(GPIO_HEARTBEAT)) {
    digitalWrite(Pin(GPIO_HEARTBEAT), ~tkr_set->runtime.heartbeat_inverted &1);
    delayMicroseconds(50);
    digitalWrite(Pin(GPIO_HEARTBEAT), tkr_set->runtime.heartbeat_inverted);
  }

  // Teleperiod
  if (Settings->tele_period || (3601 == tkr_set->runtime.tele_period)) {
    if (tkr_set->runtime.tele_period >= 9999) {
      if (!tkr_set->runtime.global_state.network_down) {
        tkr_set->runtime.tele_period = 0;  // Allow teleperiod once wifi is connected
      }
    } else {
      tkr_set->runtime.tele_period++;
      if (tkr_set->runtime.tele_period >= Settings->tele_period) {
        tkr_set->runtime.tele_period = 0;

        MqttPublishTeleState();
        MqttPublishTeleperiodSensor();

        XsnsXdrvCall(FUNC_AFTER_TELEPERIOD);
      } else {
        // Global values (Temperature, Humidity and Pressure) update every 10 seconds
        if (!(tkr_set->runtime.tele_period % 10)) {
          for (uint32_t type = 0; type < 3; type++) {
            if (!Settings->global_sensor_index[type] || tkr_set->runtime.user_globals[type]) { continue; }
            GetSensorValues();
            break;
          }
        }
      }
    }
  }

#ifdef ESP8266
  // Wifi keep alive to send Gratuitous ARP
  wifiKeepAlive();
#endif

  WifiPollNtp();

#ifdef ESP32
  if (11 == tkr_set->runtime.uptime) {  // Perform one-time ESP32 houskeeping
    ESP_getSketchSize();             // Init sketchsize as it can take up to 2 seconds
  }
#endif

#ifdef USE_UFILESYS
  static bool settings_lkg = false;  // Settings saved as Last Known Good
  // Copy Settings as Last Known Good if no changes have been saved since 30 minutes
  if (!settings_lkg && (UtcTime() > START_VALID_TIME) && (Settings->cfg_timestamp < UtcTime() - (30 * 60))) {
    TfsSaveFile(TASM_FILE_SETTINGS_LKG, (const uint8_t*)Settings, sizeof(TSettings));
    settings_lkg = true;
  }
#endif

#endif // ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE


}


/**
 * @brief To Fix
 * Code that performs resets, and will be part of the filesystem.
 * Certain resets will include
 *  ** back to default, no wifi
 *  ** erase everything, except network connection
 *  ** clear other modules (not core), so sensors/gpio/drivers
 */
void mSupport::CheckResetConditions()
{

  #ifdef ENABLE_DEVFEATURE_FIRMWARE__FOR_FUTURE_RELEASE
  if (tkr_set->runtime.restart_flag && CommandsReady()) {
      if ((214 == tkr_set->runtime.restart_flag) ||          // Reset 4
          (215 == tkr_set->runtime.restart_flag) ||          // Reset 5
          (216 == tkr_set->runtime.restart_flag)) {          // Reset 6
        // Backup current SSIDs and Passwords
        char storage_ssid1[strlen(SettingsText(SET_STASSID1)) +1];
        strncpy(storage_ssid1, SettingsText(SET_STASSID1), sizeof(storage_ssid1));
        char storage_ssid2[strlen(SettingsText(SET_STASSID2)) +1];
        strncpy(storage_ssid2, SettingsText(SET_STASSID2), sizeof(storage_ssid2));
        char storage_pass1[strlen(SettingsText(SET_STAPWD1)) +1];
        strncpy(storage_pass1, SettingsText(SET_STAPWD1), sizeof(storage_pass1));
        char storage_pass2[strlen(SettingsText(SET_STAPWD2)) +1];
        strncpy(storage_pass2, SettingsText(SET_STAPWD2), sizeof(storage_pass2));

        char storage_mqtthost[strlen(SettingsText(SET_MQTT_HOST)) +1];
        strncpy(storage_mqtthost, SettingsText(SET_MQTT_HOST), sizeof(storage_mqtthost));
        char storage_mqttuser[strlen(SettingsText(SET_MQTT_USER)) +1];
        strncpy(storage_mqttuser, SettingsText(SET_MQTT_USER), sizeof(storage_mqttuser));
        char storage_mqttpwd[strlen(SettingsText(SET_MQTT_PWD)) +1];
        strncpy(storage_mqttpwd, SettingsText(SET_MQTT_PWD), sizeof(storage_mqttpwd));
        char storage_mqtttopic[strlen(SettingsText(SET_MQTT_TOPIC)) +1];
        strncpy(storage_mqtttopic, SettingsText(SET_MQTT_TOPIC), sizeof(storage_mqtttopic));
        uint16_t mqtt_port = Settings->mqtt_port;

//        if (216 == tkr_set->runtime.restart_flag) {
          // Backup mqtt host, port, client, username and password
//        }
        if ((215 == tkr_set->runtime.restart_flag) ||        // Reset 5
            (216 == tkr_set->runtime.restart_flag)) {        // Reset 6
          SettingsErase(2);  // Erase all flash from program end to end of physical excluding optional filesystem
        }
        SettingsDefault();
        // Restore current SSIDs and Passwords
        SettingsUpdateText(SET_STASSID1, storage_ssid1);
        SettingsUpdateText(SET_STASSID2, storage_ssid2);
        SettingsUpdateText(SET_STAPWD1, storage_pass1);
        SettingsUpdateText(SET_STAPWD2, storage_pass2);
        if (216 == tkr_set->runtime.restart_flag) {          // Reset 6
          // Restore the mqtt host, port, client, username and password
          SettingsUpdateText(SET_MQTT_HOST, storage_mqtthost);
          SettingsUpdateText(SET_MQTT_USER, storage_mqttuser);
          SettingsUpdateText(SET_MQTT_PWD, storage_mqttpwd);
          SettingsUpdateText(SET_MQTT_TOPIC, storage_mqtttopic);
          Settings->mqtt_port = mqtt_port;
        }

        XdrvCall(FUNC_RESET_SETTINGS);

        tkr_set->runtime.restart_flag = 3;                   // Finish backlog then Restart 1
      }
      else if (213 == tkr_set->runtime.restart_flag) {       // Reset 3
        SettingsSdkErase();  // Erase flash SDK parameters
        tkr_set->runtime.restart_flag = 2;                   // Restart 1
      }
      else if (212 == tkr_set->runtime.restart_flag) {       // Reset 2
        SettingsErase(0);    // Erase all flash from program end to end of physical flash
        tkr_set->runtime.restart_flag = 211;                 // Reset 1
      }

      if (211 == tkr_set->runtime.restart_flag) {            // Reset 1
        SettingsDefault();
        tkr_set->runtime.restart_flag = 3;                   // Finish backlog then Restart 1
      }

      if (2 == tkr_set->runtime.restart_flag) {              // Restart 1
        XsnsXdrvCall(FUNC_ABOUT_TO_RESTART);
        SettingsSaveAll();
      }

      tkr_set->runtime.restart_flag--;
      if (tkr_set->runtime.restart_flag <= 0) {
        AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_APPLICATION "%s"), (tkr_set->runtime.restart_halt) ? PSTR("Halted") : (tkr_set->runtime.restart_deepsleep) ? PSTR("Sleeping") : PSTR(D_RESTARTING));
        EspRestart();
      }
    }
    #endif



    if (tkr_set->runtime.restart_flag && (tkr_set->runtime.backlog_pointer == tkr_set->runtime.backlog_index)) {
      if ((214 == tkr_set->runtime.restart_flag) || (215 == tkr_set->runtime.restart_flag) || (216 == tkr_set->runtime.restart_flag)) {
// Backup current SSIDs and Passwords



                                      // Loading in new network configs only if they are considered save and correct

        // char storage_wifi[sizeof(tkr_set->Settings.sta_ssid) +
        //                   sizeof(tkr_set->Settings.sta_pwd)];
        // char storage_mqtt[sizeof(tkr_set->Settings.mqtt_host) +
        //                   sizeof(tkr_set->Settings.mqtt_port) +
        //                   sizeof(tkr_set->Settings.mqtt_client) +
        //                   sizeof(tkr_set->Settings.mqtt_user) +
        //                   sizeof(tkr_set->Settings.mqtt_pwd) +
        //                   sizeof(tkr_set->Settings.mqtt_topic)];
        // memcpy(storage_wifi, tkr_set->Settings.sta_ssid, sizeof(storage_wifi));     // Backup current SSIDs and Passwords

        // // Backup current SSIDs and Passwords
        // char storage_ssid1[strlen(SettingsText(SET_STASSID1)) +1];
        // strncpy(storage_ssid1, SettingsText(SET_STASSID1), sizeof(storage_ssid1));
        // char storage_ssid2[strlen(SettingsText(SET_STASSID2)) +1];
        // strncpy(storage_ssid2, SettingsText(SET_STASSID2), sizeof(storage_ssid2));
        // char storage_pass1[strlen(SettingsText(SET_STAPWD1)) +1];
        // strncpy(storage_pass1, SettingsText(SET_STAPWD1), sizeof(storage_pass1));
        // char storage_pass2[strlen(SettingsText(SET_STAPWD2)) +1];
        // strncpy(storage_pass2, SettingsText(SET_STAPWD2), sizeof(storage_pass2));

        // char storage_mqtthost[strlen(SettingsText(SET_MQTT_HOST)) +1];
        // strncpy(storage_mqtthost, SettingsText(SET_MQTT_HOST), sizeof(storage_mqtthost));
        // char storage_mqttuser[strlen(SettingsText(SET_MQTT_USER)) +1];
        // strncpy(storage_mqttuser, SettingsText(SET_MQTT_USER), sizeof(storage_mqttuser));
        // char storage_mqttpwd[strlen(SettingsText(SET_MQTT_PWD)) +1];
        // strncpy(storage_mqttpwd, SettingsText(SET_MQTT_PWD), sizeof(storage_mqttpwd));
        // char storage_mqtttopic[strlen(SettingsText(SET_MQTT_TOPIC)) +1];
        // strncpy(storage_mqtttopic, SettingsText(SET_MQTT_TOPIC), sizeof(storage_mqtttopic));
        // uint16_t mqtt_port = Settings.mqtt_port;

        // if (216 == tkr_set->restart_flag) {
        //   memcpy(storage_mqtt, tkr_set->Settings.mqtt_host, sizeof(storage_mqtt));  // Backup mqtt host, port, client, username and password
        // }
        if ((215 == tkr_set->runtime.restart_flag) || (216 == tkr_set->runtime.restart_flag)) {
          tkr_set->SettingsErase(0);  // Erase all flash from program end to end of physical flash
        }
        tkr_set->SettingsDefault();
        // memcpy(tkr_set->Settings.sta_ssid, storage_wifi, sizeof(storage_wifi));     // Restore current SSIDs and Passwords
        // if (216 == tkr_set->restart_flag) {
        //   memcpy(tkr_set->Settings.mqtt_host, storage_mqtt, sizeof(storage_mqtt));  // Restore the mqtt host, port, client, username and password
        //   strlcpy(tkr_set->Settings.mqtt_client, MQTT_CLIENT_ID, sizeof(tkr_set->Settings.mqtt_client));  // Set client to default
        // }
        tkr_set->runtime.restart_flag = 2;
      }



      else if (213 == tkr_set->runtime.restart_flag) {
        tkr_set->SettingsSdkErase();  // Erase flash SDK parameters
        tkr_set->runtime.restart_flag = 2;
      }
      else if (212 == tkr_set->runtime.restart_flag) {
        tkr_set->SettingsErase(0);    // Erase all flash from program end to end of physical flash
        tkr_set->runtime.restart_flag = 211;
      }
      if (211 == tkr_set->runtime.restart_flag) {
        tkr_set->SettingsDefault();
        tkr_set->runtime.restart_flag = 2;
      }
      // tkr_set->SettingsSaveAll();
      tkr_set->runtime.restart_flag--;
      if (tkr_set->runtime.restart_flag <= 0) 
      {

        #ifdef ENABLE_LOG_LEVEL_INFO
        ALOG_INF(PSTR(D_LOG_APPLICATION "tkr_set->restart_flag <= 0 " D_RESTARTING));
        #endif// ENABLE_LOG_LEVEL_INFO
    
        EspRestart();        
      }
    }


}



// Force a float value between two ranges, and adds or substract the range until we fit
float mSupport::ModulusRangef(float f, float a, float b) {
  if (b <= a) { return a; }       // inconsistent, do what we can
  float range = b - a;
  float x = f - a;                // now range of x should be 0..range
  x = fmodf(x, range);            // actual range is now -range..range
  if (x < 0.0f) { x += range; }   // actual range is now 0..range
  return x + a;                   // returns range a..b
}


#ifdef ESP8266
extern "C" {
extern struct rst_info resetInfo;
}
#endif // ESP8266


double mSupport::FastPrecisePow(double a, double b)
{
  // https://martin.ankerl.com/2012/01/25/optimized-approximative-pow-in-c-and-cpp/
  // calculate approximation with fraction of the exponent
  int e = abs((int)b);
  union {
    double d;
    int x[2];
  } u = { a };
  u.x[1] = (int)((b - e) * (u.x[1] - 1072632447) + 1072632447);
  u.x[0] = 0;
  // exponentiation by squaring with the exponent's integer part
  // double r = u.d makes everything much slower, not sure why
  double r = 1.0;
  while (e) {
    if (e & 1) {
      r *= a;
    }
    a *= a;
    e >>= 1;
  }
  return r * u.d;
}

float mSupport::FastPrecisePowf(const float x, const float y)
{
//  return (float)(pow((double)x, (double)y));
  return (float)FastPrecisePow(x, y);
}


/**
 * @brief Includes safe stopping any processes
 * 
 */
void mSupport::ESP_Restart_Safe(void)
{
  ALOG_INF(PSTR(D_LOG_SUPPORT "ESP_Restart_Safe: Modules instructed to cleanup" ));

  tkr->Tasker_Interface(TASK_RESTART_SET_DO_FINAL_CLEANUP);

  ALOG_INF(PSTR(D_LOG_SUPPORT "Complete. Waiting" ));

  long dly = millis();
  while (millis() - dly < 2000) {
    yield();        // enough time to send response to client
  }

  Serial.println("Restart"); Serial.flush();

  ESP.restart();

}

/**
 * @brief Hard shutdown
 * Only used in error cases
 * Splash on serial (no logs), immediate forced restart
 */
void mSupport::ESP_Restart_Immediate(void)
{
    ESP.restart();
}



void mSupport::EspRestart(void)
{

#ifdef ENABLE_DEVFEATURE_BLOCK_RESTART

  AddLog(LOG_LEVEL_ERROR, PSTR("ENABLE_DEVFEATURE_BLOCK_RESTART"));

#else

  //ResetPwm();
  tkr_wifi->WiFi_Radio_Shutdown(true);
  // CrashDumpClear();           // Clear the stack dump in RTC

  // if (restart_halt) {
  //   while (1) {
  //     OsWatchLoop();          // Feed OsWatch timer to prevent restart
  //     SetLedLink(1);          // Wifi led on
  //     delay(200);             // Satisfy SDK
  //     SetLedLink(0);          // Wifi led off
  //     delay(800);             // Satisfy SDK
  //   }
  // } else {
    ESP.restart(); //ESP_Restart();
  // }

  //  ResetPwm();
  // WiFi_Radio_Shutdown(true);
  // CrashDumpClear();           // Clear the stack dump in RTC

  // if (restart_halt) {
  //   while (1) {
  //     OsWatchLoop();          // Feed OsWatch timer to prevent restart
  //     SetLedLink(1);          // Wifi led on
  //     delay(200);             // Satisfy SDK
  //     SetLedLink(0);          // Wifi led off
  //     delay(800);             // Satisfy SDK
  //   }
  // } else {
  //   ESP_Restart();
  // }
  // delay(100);                 // Allow time for message xfer - disabled v6.1.0b
  // //if (Settings.flag_system.mqtt_enabled) MqttDisconnect();
  // WifiDisconnect();
  // //tkr_sup->CrashDumpClear();
  // ESP.restart();            // This results in exception 3 on restarts on core 2.3.0
  // #ifdef ESP8266
  //   ESP.reset();
  // #endif

#endif // ENABLE_DEVFEATURE_BLOCK_RESTART

}
