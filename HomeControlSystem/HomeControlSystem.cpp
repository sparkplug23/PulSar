/*
   Copyright (c) 2020, Michael G. Doone
   All rights reserved.
*/

#include "2_CoreSystem/InterfaceController/mInterfaceController.h"

// #define USE_DECLARE_AT_COMPILE_TO_DEBUG

// #ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG
// mRGBAnimator mrgbani;

// #endif



#ifdef USE_ARDUINO_OTA
/*********************************************************************************************\
 * Allow updating via the Arduino OTA-protocol.
 *
 * - Once started disables current wifi clients and udp
 * - Perform restart when done to re-init wifi clients
\*********************************************************************************************/

bool arduino_ota_triggered = false;
uint16_t arduino_ota_progress_dot_count = 0;

// #define TEST_OTA_ISSUE

void ArduinoOTAInit(void)
{
  #ifndef ESP32
    //ArduinoOTA.setPort(8266);
  #endif
  // #ifdef ESP8266
  #ifndef TEST_OTA_ISSUE
    ArduinoOTA.setHostname(pCONT_set->my_hostname);
  #endif
    //ArduinoOTA.setPort(8266);
    //if (pCONT_set->Settings.web_password[0] !=0) { ArduinoOTA.setPassword(pCONT_set->Settings.web_password); }
  // #endif
  ArduinoOTA.onStart([]()
  {
    // #ifdef ESP8266
      //pCONT_set->SettingsSave(1);  // Free flash for OTA update
      //#ifdef USE_WEBSERVER
        // if (pCONT_set->Settings.webserver) { 
          //pCONT_web->StopWebserver(); 
          // /}
      // #endif  // USE_WEBSERVER
      //if (pCONT_set->Settings.flag_system_phaseout.mqtt_enabled) { MqttDisconnect(); }
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_UPLOAD_STARTED));
    // #endif
    arduino_ota_triggered = true;
    arduino_ota_progress_dot_count = 0;
    
    #ifdef ESP32
      pinMode(2,OUTPUT);
      timerWrite(timerwdt, 0); //reset timer (feed watchdog)
    #endif

    // Stop server otherwise OTA can fail
    // pCONT_web->StopWebserver();

    //LED_BLUE_INIT();
    //delay(100);       // Allow time for message xfer
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
  //  if ((LOG_LEVEL_DEBUG <= pCONT_set->seriallog_level)) { // for when hardware serial is in use for modules

      uint8_t progress_now = (progress/(total/100));

      if(arduino_ota_progress_dot_count != progress_now){
        Serial.println(progress_now);
        arduino_ota_progress_dot_count = progress_now;
      }
      //if (!(arduino_ota_progress_dot_count % 80)) { Serial.println(); }

      #ifdef ESP32
        timerWrite(timerwdt, 0);
      #else
        ESP.wdtFeed();
      #endif
    // }
    
  });

  ArduinoOTA.onError([](ota_error_t error)
  {
    /*
    From ArduinoOTA.h:
    typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
    */
    char error_str[30];
    memset(error_str,0,sizeof(error_str));

    //#ifdef ESP8266
    //if ((LOG_LEVEL_DEBUG <= seriallog_level) && arduino_ota_progress_dot_count) { Serial.println(); }
    switch (error) {
      case OTA_BEGIN_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
      // case OTA_CONNECT_ERROR: sprintf(error_str, PSTR("Connect Error")); break;
      case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
      case OTA_END_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
      default:
        snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
    }
    #ifdef ENABLE_LOG
    AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA  %s. " D_RESTARTING), error_str);
    #endif
    // #ifdef ESP8266
      // pCONT_wif->EspRestart(); //try this first
    // #else
      ESP.restart(); //should only reach if the first failed
    //#endif
    //#endif
  });

  ArduinoOTA.onEnd([]()
  {
    // Serial.println("end");
    #ifdef ESP8266
      // if ((LOG_LEVEL_DEBUG <= mso->seriallog_level)) { Serial.println(); }
      #ifdef ENABLE_LOG
      AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_SUCCESSFUL ". " D_RESTARTING));
      #endif
      //LED_BLUE_OFF();
    // #else
    #endif
  #ifndef TEST_OTA_ISSUE
    pCONT_wif->EspRestart(); //try this first
#endif
    ESP.restart();

	});

  ArduinoOTA.begin();
  
  #ifdef ENABLE_LOG
  AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_ENABLED " " D_PORT " 8266"));
  #endif
// #ifdef ESP8266
//   AddLog_P2(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_ENABLED " " D_PORT " 8266"));
// #endif
}

void ArduinoOtaLoop(void)
{
  MDNS.update();
  ArduinoOTA.handle();
  // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
  // Note async stuff can still occur, so I need to disable them
  while (arduino_ota_triggered) { ArduinoOTA.handle(); }
}

#endif  // USE_ARDUINO_OTA


void HandleFailedBootFailBack(){

  //ota_startup_period_ms = 5000;
  //so.seriallog_level=LOG_LEVEL_ALL;

  //#ifndef DEBUG_TESTCODE
  // Connect first
  // if(ota_startup_period_ms){
  //   if(WiFi.status() != WL_CONNECTED){
  //     mwif.WifiConnectForced(); //only wifi connection in setup for OTA recovery
  //     MDNS.begin(DEVICENAME_CTR);
  //   }
  //   Serial.print("[OTA ] Started ("); Serial.print(round(ota_startup_period_ms/1000)); Serial.println(" seconds)...");
  //   unsigned long tProgram = millis(), tFlash = millis();
  //   while(abs(millis()-tProgram)<ota_startup_period_ms){ //give me 10 seconds to reprogram from bad code
  //     ArduinoOTA.handle(); while (arduino_ota_triggered) ArduinoOTA.handle();
  //     #ifdef ESP32
  //       timerWrite(timerwdt, 0);
  //     #else
  //       ESP.wdtFeed();
  //     #endif
  //     if(abs(millis()-tFlash)>=1000){ tFlash = millis();
  //       Serial.print("[OTA ] Seconds remaining "); Serial.println(int((ota_startup_period_ms-abs(millis()-tProgram))/1000));
  //     }
  //   }
  //   Serial.println("[OTA ] Ended");
  //   }
  // END OTA
  //#endif

}


#ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG

void init_class_instances(){

  pCONT->mrgbani = &mrgbani;

}

#endif

#define DISABLE

/********************************************************************************************/
/*********************SETUP******************************************************************/
/********************************************************************************************/

void setup(void)
{ 
  Serial.begin(115200);

  #ifdef USE_SERIAL_ALTERNATE_TX
    Serial.set_tx(2);
  #endif
  Serial.println(F("Rebooting..."));
  #ifndef DISABLE_SERIAL_LOGGING
  #ifdef ENABLE_BUG_TRACING
  Serial.println(F("DELAYED BOOT for 5 seconds...")); Serial.flush(); delay(5000);
  #endif
  #endif

  // Init Json builder with memory address and size
  JsonBuilder_Start(data_buffer2.payload.ctr,&data_buffer2.payload.len,DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  JsonBuilderI->Start(data_buffer2.payload.ctr,&data_buffer2.payload.len,DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  BufferWriterI->Start(data_buffer2.payload.ctr,&data_buffer2.payload.len,DATA_BUFFER_PAYLOAD_MAX_LENGTH);

  // Configure WDT
  // msup.WDT_Begin();
  #ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG
  void init_class_instances();
  #endif 
  
  pCONT->Instance_Init();
  
  // Clear boot status info
  memset(&pCONT_set->boot_status,0,sizeof(pCONT_set->boot_status));

  // All pointers must be set above
  int8_t result = pCONT->Tasker_Interface(FUNC_CHECK_POINTERS);

  if(result != FUNCTION_RESULT_SUCCESS_ID){
    // jump into a safe state // if failed, connect to wifi, wait for ota, restart after 10 minutes
    delay(10000); // long enough to cause reset
  }  

  pCONT->InitClassList();
    
  pCONT_set->RtcRebootLoad();
  if(!pCONT_set->RtcRebootValid()) { pCONT_set->RtcReboot.fast_reboot_count = 0; }
  pCONT_set->RtcReboot.fast_reboot_count++;
  pCONT_set->RtcRebootSave();

  pCONT_sup->init_FirmwareVersion();
  
  #ifdef ENABLE_SETTINGS_STORAGE
  // Load config from memory
  pCONT_set->SettingsDefault(); //preload minimal required
  // Overwrite with latest values, including template if new SETTINGS_CONFIG exists
  pCONT_set->SettingsLoad();    //overwrite stored settings from defaults
  // Check Load was successful
  pCONT_set->SettingsLoad_CheckSuccessful();
  #endif
  
  #ifdef USE_ARDUINO_OTA
    ArduinoOTAInit();
  #endif // USE_ARDUINO_OTA

  pCONT->Tasker_Interface(FUNC_POINTER_INIT); // confirgure any memory address needed as part of module init or templates
  
  #ifdef FORCE_TEMPLATE_LOADING
  // This will overwrite the settings, temporary, will use a second flag to force template loads "TEMPLATE_HOLDER"
  // need to if template not provided, load defaults else use settings -- add protection in settings defaults to use templates instead (progmem or user desired)
  // Load template before init
  #ifdef ENABLE_LOG
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD "Temporary loading any progmem templates"));
  #endif
  pCONT->Tasker_Interface(FUNC_TEMPLATE_MODULE_LOAD); // loading module, only interface modules will have these
  #endif

  // Init the GPIOs
  pCONT_pins->GpioInit();
  // Start pins in modules
  pCONT->Tasker_Interface(FUNC_PRE_INIT);
  // Init devices
  pCONT->Tasker_Interface(FUNC_INIT);
  // Run system functions 
  pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_INIT);
  // Load the minimal default settings in modules (hard coded) before loading any stored user values
  pCONT->Tasker_Interface(FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES); // load the minimal
  // Load any stored user values into module
  pCONT->Tasker_Interface(FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE);
  // Configure sensor/drivers to values desired for modules
  pCONT->Tasker_Interface(FUNC_CONFIGURE_MODULES_FOR_DEVICE);
  // init mqtt handlers from memory
  pCONT->Tasker_Interface(FUNC_MQTT_HANDLERS_INIT);
  // init mqtt handlers
  pCONT->Tasker_Interface(FUNC_MQTT_INIT); // phase out of handlers to only be init/start

  #ifdef FORCE_TEMPLATE_LOADING // after settings load..? needs to be after too, think this through
  // This will overwrite the settings, temporary, will use a second flag to force template loads "TEMPLATE_HOLDER"
  // need to if template not provided, load defaults else use settings -- add protection in settings defaults to use templates instead (progmem or user desired)
  #ifdef ENABLE_LOG
  AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_MEMORY D_LOAD "Temporary loading any progmem templates"));
  #endif
  pCONT->Tasker_Interface(FUNC_TEMPLATE_MODULE_LOAD); // loading module, only interface modules will have these
  pCONT->Tasker_Interface(FUNC_TEMPLATE_DEVICE_LOAD);
  #endif

  pCONT_wif->WifiConnect();

  #ifdef ENABLE_FUNCTION_DEBUG
    pCONT->Tasker_Interface(FUNC_DEBUG_CONFIGURE);
  #endif
  
    // TEST test;
    // test.f<int>();

    // pCONT->mry->ftest<int>();
    // pCONT->mry->ftest2((int)2);
    // pCONT->mry->ftest2((char)'2');


    // // JsonObjectConst dummy;
    // // pCONT->mry->Tasker2((uint8_t)2,dummy);
    // pCONT->mry->Tasker2((uint8_t)2,(uint8_t)1);
    // pCONT->mry->Tasker2((uint8_t)2,(uint8_t)3);

}

// /********************************************************************************************/
// /*********************loop*******************************************************************/
// /********************************************************************************************/

void loop(void)
{

  pCONT_sup->activity.loop_counter++;
  pCONT_sup->loop_start_millis = millis();
  
  #ifdef USE_ARDUINO_OTA
    ArduinoOtaLoop();
  #endif  // USE_ARDUINO_OTA

  pCONT->Tasker_Interface(FUNC_LOOP); // EVERY_LOOP
  if(pCONT_time->uptime.seconds_nonreset > 30){ pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_LOOP); } // Only run after stable boot

  //move into support, or into time, to align with every_minute, hour, etc
  if(mSupport::TimeReached(&pCONT_sup->tSavedLoop50mSec ,50  )){ pCONT->Tasker_Interface(FUNC_EVERY_50_MSECOND);  }
  if(mSupport::TimeReached(&pCONT_sup->tSavedLoop100mSec,100 )){ pCONT->Tasker_Interface(FUNC_EVERY_100_MSECOND); }
  if(mSupport::TimeReached(&pCONT_sup->tSavedLoop200mSec,200 )){ pCONT->Tasker_Interface(FUNC_EVERY_200_MSECOND); }
  if(mSupport::TimeReached(&pCONT_sup->tSavedLoop250mSec,250 )){ pCONT->Tasker_Interface(FUNC_EVERY_250_MSECOND); }
  if(mSupport::TimeReached(&pCONT_sup->tSavedLoop1Sec   ,1000)){ pCONT->Tasker_Interface(FUNC_EVERY_SECOND); 
  
  
  pCONT_sup->activity.cycles_per_sec = pCONT_sup->activity.loop_counter; 
  // AddLog_P(LOG_LEVEL_TEST,PSTR("LOOPSEC2 = %d"), pCONT_sup->activity.loop_counter);
  pCONT_sup->activity.loop_counter=0;

     }
     
  /********************************************************************************************************
  ******************************************************************************************************** 
    End of loop, only calculate or sleep after this point
  ********************************************************************************************************
  ********************************************************************************************************/
    
  pCONT_sup->loop_runtime_millis = millis() - pCONT_sup->loop_start_millis;


  //  pCONT_mqtt->flag_uptime_reached_reduce_frequency = true;

  // Change this to my own way
  // DO THIS NEXT
  //SmartLoopDelay()
  // #ifndef DISABLE_SLEEP
  // if(pCONT_set->Settings.enable_sleep){
  //   if (pCONT_set->Settings.flag_network_phaseout.sleep_normal) {
  //     delay(pCONT_set->sleep);
  //   } else {

  //     // Loop time < sleep length of time
  //     if (pCONT_sup->loop_runtime_millis < (uint32_t)pCONT_set->sleep) {
  //       //delay by loop time
  //       delay((uint32_t)pCONT_set->sleep - pCONT_sup->loop_runtime_millis);  // Provide time for background tasks like wifi
  //     } else {

  //       // if loop takes longer than sleep period, no delay, IF wifi is down, devote half loop time to wifi connect
  //       if (pCONT_set->global_state.wifi_down) {
  //         delay(pCONT_sup->loop_runtime_millis /2); // If wifi down and loop_runtime_millis > setoption36 then force loop delay to 1/3 of loop_runtime_millis period
  //       }
  //     }
  //   }
  // }
  // #endif

  if (!pCONT_sup->loop_runtime_millis) { pCONT_sup->loop_runtime_millis++; }            // We cannot divide by 0
  pCONT_sup->loop_delay = pCONT_set->sleep;
  if (!pCONT_sup->loop_delay) { pCONT_sup->loop_delay++; }              // We cannot divide by 0
  pCONT_sup->loops_per_second = 1000 / pCONT_sup->loop_delay;  // We need to keep track of this many loops per second
  pCONT_sup->this_cycle_ratio = 100 * pCONT_sup->loop_runtime_millis / pCONT_sup->loop_delay;
  pCONT_set->loop_load_avg = pCONT_set->loop_load_avg - (pCONT_set->loop_load_avg / pCONT_sup->loops_per_second) + (pCONT_sup->this_cycle_ratio / pCONT_sup->loops_per_second); // Take away one loop average away and add the new one

  // Create a debug mqtt packet for timings, of main loop and interface loops
  // Serial.printf("%s=%d\r\n","tick",pCONT_sup->loop_runtime_millis);
  // Serial.printf("%s=%d\r\n","tick",pCONT_sup->activity.cycles_per_sec);
  // uint32_t start_millis = millis();
  // AddLog_P(LOG_LEVEL_TEST,PSTR("LOOPSEC = %d \t %d"),loops_per_second,mtel.activity.cycles_per_sec);
  // Serial.printf("ADD TIME = %d\n\r",millis()-start_millis);

  pCONT_set->fSystemRestarted = false; //phase out and use module flag instead

}
