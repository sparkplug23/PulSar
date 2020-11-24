/*
   Copyright (c) 2020, Michael G. Doone
   All rights reserved.
*/

#include "1_TaskerManager/mTaskerManager.h"

//#define USE_DECLARE_AT_COMPILE_TO_DEBUG

// #define ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING

#ifdef ENABLE_DEVFEATURE_DISABLE_ALL_WDT_FOR_TESTING
void hw_wdt_disable(){
  *((volatile uint32_t*) 0x60000900) &= ~(1); // Hardware WDT OFF 
}

void hw_wdt_enable(){ 
  *((volatile uint32_t*) 0x60000900) |= 1; // Hardware WDT ON 
}
#endif

/*

/*********************************************************************************************
 * Hardware related
\*********************************************************************************************

#ifdef ESP8266

void HwWdtDisable(void) {
  *((volatile uint32_t*) 0x60000900) &= ~(1);  // Hardware WDT OFF
}

void HwWdtEnable(void) {
  *((volatile uint32_t*) 0x60000900) |= 1;     // Hardware WDT ON
}

void WdtDisable(void) {
  ESP.wdtDisable();
  HwWdtDisable();
}

void WdtEnable(void) {
  HwWdtEnable();
  ESP.wdtEnable(0);
}

#endif  // ESP8266*/

/*
without energyusage in rtc
RAM:   [======    ]  64.7% (used 53000 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510748 bytes from 1023984 bytes)
// with energy rtc
RAM:   [======    ]  64.8% (used 53112 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510828 bytes from 1023984 bytes)
//
RAM:   [======    ]  64.2% (used 52576 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510764 bytes from 1023984 bytes)
RAM:   [======    ]  63.1% (used 51680 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510224 bytes from 1023984 bytes)

RAM:   [======    ]  63.1% (used 51656 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510224 bytes from 1023984 bytes)

RAM:   [======    ]  63.0% (used 51640 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510224 bytes from 1023984 bytes)
//
RAM:   [======    ]  59.6% (used 48840 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510176 bytes from 1023984 bytes)


RAM:   [======    ]  63.5% (used 52040 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510352 bytes from 1023984 bytes)

RAM:   [======    ]  63.5% (used 52040 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510268 bytes from 1023984 bytes)

RAM:   [======    ]  60.0% (used 49128 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510632 bytes from 1023984 bytes)

RAM:   [======    ]  60.0% (used 49112 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510668 bytes from 1023984 bytes)

RAM:   [======    ]  59.8% (used 48952 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510492 bytes from 1023984 bytes)

RAM:   [======    ]  59.6% (used 48856 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510612 bytes from 1023984 bytes)

RAM:   [======    ]  59.6% (used 48856 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510652 bytes from 1023984 bytes)

RAM:   [======    ]  59.6% (used 48792 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510316 bytes from 1023984 bytes)

RAM:   [======    ]  59.3% (used 48552 bytes from 81920 bytes)
Flash: [=====     ]  49.9% (used 510620 bytes from 1023984 bytes)


RAM:   [======    ]  58.9% (used 48248 bytes from 81920 bytes)
Flash: [=====     ]  49.8% (used 510260 bytes from 1023984 bytes)

=====================================================================
*/


#ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG
    mHardwarePins mod;
    mSettings mset;
    mSupport msup;
    mLogging mso;
    mTime mt;
    mMQTT mqt;
    mWiFi mwif;
    mTelemetry mtel;
    #ifdef USE_MODULE_CORE_WEBSERVER
    mWebServer mweb;
    #endif
  #ifdef USE_MODULE_SENSORS_DHT
    mSensorsDHT msdht;
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    mSensorsBME msbme;
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    mSensorsDB18 msdb18;
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    mUltraSonicSensor mus;
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    mOilFurnace mof;
  #endif
  #ifdef USE_MODULE_CUSTOM_BLINDS
    mBlinds mbbl;
  #endif
  #ifdef   USE_MODULE_SENSORS_PULSE_COUNTER
    mPulseCounter mpc;
  #endif

  
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    mHBridge mdhb;
  #endif

  #ifdef USE_MODULE_DRIVERS_ENERGY
    mEnergy mdenergy;
  #endif
  /*
    Lights
    */
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    mInterfaceLight mil;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    mRGBAnimator mrgbani;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    mPWMLight mlights_pwm;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif



  #ifdef USE_MODULE_CUSTOM_HEATING
    mHeating mh;
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    mRelays mry;
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM  //this is really a pin class, but keep it on its own for now
    mPWM mpwm;
  #endif
  #ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
    mGarageLights mrl;
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    mMotionSensor mms;
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    mDoorSensor mds;
  #endif
  #ifdef USE_MODULE_SENSORS_DOORCHIME
    mDoorBell mdb;
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    mRadiatorFan mrf;
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    mSonoffIFan mifan;
  #endif
  #ifdef USE_MODULE_CUSTOM_PWM_FAN
    mPWMFan mpwmfan;
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    mIRtransceiver mir;
  #endif
  #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined (USE_MODULE_DRIVERS_RF433MHZ)
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
   mNextionPanel mnext;
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    mMoistureSensor mois;
  #endif

  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    mButtons mbtn;
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    mSwitches mswh;
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    mPzem_AC mspm;
  #endif


  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    mExerciseBike meb;
  #endif
  
void init_class_instances(){
   pCONT->mod = &mod;
   pCONT->mset = &mset;
    pCONT-> msup = &msup;
    pCONT-> mso = &mso;
    pCONT-> mt = &mt;
    pCONT-> mqt = &mqt;
    pCONT-> mwif = &mwif;
    pCONT-> mtel = &mtel;
    #ifdef USE_MODULE_CORE_WEBSERVER
    pCONT-> mweb = &mweb;
    #endif
  #ifdef USE_MODULE_SENSORS_DHT
    pCONT-> msdht = &mset;
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    pCONT-> msbme = &mset;
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    pCONT-> msdb18;
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    pCONT-> mus;
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    pCONT-> mof;
  #endif
  #ifdef USE_MODULE_CUSTOM_BLINDS
    pCONT-> mbbl;
  #endif
  #ifdef   USE_MODULE_SENSORS_PULSE_COUNTER
    pCONT-> mpc;
  #endif

  
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    pCONT-> mdhb;
  #endif

  #ifdef USE_MODULE_DRIVERS_ENERGY
    pCONT-> mdenergy;
  #endif
  /*
    Lights
    */
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    pCONT-> mil = &mil;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    pCONT-> mrgbani = &mrgbani;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    pCONT-> mlights_pwm;    // this should be moved to accessing from inside USE_MODULE_LIGHTS_INTERFACE
  #endif



  #ifdef USE_MODULE_CUSTOM_HEATING
    pCONT-> mh;
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    pCONT-> mry;
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM  //this is really a pin class, but keep it on its own for now
    pCONT-> mpwm;
  #endif
  #ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
    pCONT-> mrl;
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    pCONT-> mms;
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    pCONT-> mds;
  #endif
  #ifdef USE_MODULE_SENSORS_DOORCHIME
    pCONT-> mdb;
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    pCONT-> mrf;
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    pCONT-> mifan;
  #endif
  #ifdef USE_MODULE_CUSTOM_PWM_FAN
    pCONT-> mpwmfan;
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    pCONT-> mir;
  #endif
  #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined (USE_MODULE_DRIVERS_RF433MHZ)
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
   pCONT-> mnext;
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    pCONT-> mois;
  #endif

  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    pCONT-> mbtn;
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    pCONT-> mswh;
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    pCONT-> mspm;
  #endif


  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    pCONT-> meb;
  #endif

}

#endif




//   #ifndef ENABLE_DEVFEATURE_OTA_METHOD

// #ifdef USE_ARDUINO_OTA
// /*********************************************************************************************\
//  * Allow updating via the Arduino OTA-protocol.
//  *
//  * - Once started disables current wifi clients and udp
//  * - Perform restart when done to re-init wifi clients
// \*********************************************************************************************/

// bool arduino_ota_triggered = false;
// uint16_t arduino_ota_progress_dot_count = 0;

// void ArduinoOTAInit(void)
// {
//   #ifndef TEST_OTA_ISSUE
//     ArduinoOTA.setHostname(pCONT_set->my_hostname);
//   #endif
//   ArduinoOTA.onStart([]()
//   {
//     // #ifdef ESP8266
//       //pCONT_set->SettingsSave(1);  // Free flash for OTA update
//       //#ifdef USE_MODULE_CORE_WEBSERVER
//         // if (pCONT_set->Settings.webserver) { 
//           //pCONT_web->StopWebserver(); 
//           // /}
//       // #endif  // USE_MODULE_CORE_WEBSERVER
//       //if (pCONT_set->Settings.flag_system.mqtt_enabled) { MqttDisconnect(); }
//       AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_UPLOAD_STARTED));
//     // #endif
//     arduino_ota_triggered = true;
//     arduino_ota_progress_dot_count = 0;
    
//     #ifdef ESP32
//       pinMode(2,OUTPUT);
//       // timerWrite(timerwdt, 0); //reset timer (feed watchdog)
//     #endif

//     // Stop server otherwise OTA can fail
//     // pCONT_web->StopWebserver();

//     delay(100);       // Allow time for message xfer
//   });

//   ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
//   {
//     if (pCONT_set->seriallog_level >= LOG_LEVEL_DEBUG) { // for when hardware serial is in use for modules

//       uint8_t progress_now = (progress/(total/100));
//       if(arduino_ota_progress_dot_count != progress_now){
//         Serial.println(progress_now);
//         arduino_ota_progress_dot_count = progress_now;
//       }
//       #ifdef ESP8266
//       ESP.wdtFeed();
//       #endif // ESP8266
//     }
    
//   });

//   ArduinoOTA.onError([](ota_error_t error)
//   {
//     /*
//     From ArduinoOTA.h:
//     typedef enum { OTA_AUTH_ERROR, OTA_BEGIN_ERROR, OTA_CONNECT_ERROR, OTA_RECEIVE_ERROR, OTA_END_ERROR } ota_error_t;
//     */
//     char error_str[30];
//     memset(error_str,0,sizeof(error_str));

//     switch (error) {
//       case OTA_AUTH_ERROR:    strncpy_P(error_str, PSTR("OTA_AUTH_ERROR"), sizeof(error_str)); break;    
//       case OTA_BEGIN_ERROR:   strncpy_P(error_str, PSTR(D_UPLOAD_ERR_2), sizeof(error_str)); break;
//       case OTA_CONNECT_ERROR: sprintf(error_str, PSTR("Connect Error")); break;
//       case OTA_RECEIVE_ERROR: strncpy_P(error_str, PSTR(D_UPLOAD_ERR_5), sizeof(error_str)); break;
//       case OTA_END_ERROR:     strncpy_P(error_str, PSTR(D_UPLOAD_ERR_7), sizeof(error_str)); break;
//       default:
//         snprintf_P(error_str, sizeof(error_str), PSTR(D_UPLOAD_ERROR_CODE " %d"), error);
//     }
//     #ifdef ENABLE_LOG
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA  %s. " D_RESTARTING), error_str);
//     #endif
    
//     ESP.restart(); //should only reach if the first failed
//   });

//   ArduinoOTA.onEnd([]()
//   {
//     #ifdef ENABLE_LOG
//     AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_SUCCESSFUL ". " D_RESTARTING));
//     #endif
//     ESP.restart();
// 	});

//   ArduinoOTA.begin();
  
//   #ifdef ENABLE_LOG
//   AddLog_P(LOG_LEVEL_INFO, PSTR(D_LOG_UPLOAD "Arduino OTA " D_ENABLED " " D_PORT " 8266"));
//   #endif
// }

// void ArduinoOtaLoop(void)
// {
//   ArduinoOTA.handle();
//   // Once OTA is triggered, only handle that and dont do other stuff. (otherwise it fails)
//   // Note async stuff can still occur, so I need to disable them
//   while (arduino_ota_triggered){ ArduinoOTA.handle(); }
// }

// #endif  // USE_ARDUINO_OTA


// void HandleFailedBootFailBack(){

//   //ota_startup_period_ms = 5000;
//   //so.seriallog_level=LOG_LEVEL_ALL;

//   //#ifndef DEBUG_TESTCODE
//   // Connect first
//   // if(ota_startup_period_ms){
//   //   if(WiFi.status() != WL_CONNECTED){
//   //     mwif.WifiConnectForced(); //only wifi connection in setup for OTA recovery
//   //   }
//   //   Serial.print("[OTA ] Started ("); Serial.print(round(ota_startup_period_ms/1000)); Serial.println(" seconds)...");
//   //   unsigned long tProgram = millis(), tFlash = millis();
//   //   while(abs(millis()-tProgram)<ota_startup_period_ms){ //give me 10 seconds to reprogram from bad code
//   //     ArduinoOTA.handle(); while (arduino_ota_triggered) ArduinoOTA.handle();
//   //     #ifdef ESP32
//   //       timerWrite(timerwdt, 0);
//   //     #else
//   //       ESP.wdtFeed();
//   //     #endif
//   //     if(abs(millis()-tFlash)>=1000){ tFlash = millis();
//   //       Serial.print("[OTA ] Seconds remaining "); Serial.println(int((ota_startup_period_ms-abs(millis()-tProgram))/1000));
//   //     }
//   //   }
//   //   Serial.println("[OTA ] Ended");
//   //   }
//   // END OTA
//   //#endif

// }

// #endif //   #ifndef ENABLE_DEVFEATURE_OTA_METHOD


#define DISABLE

#ifdef ESP32
// TMP placement
// ESP32 specific
//

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#define DISABLE_ESP32_BROWNOUT

void DisableBrownout(void) {
  // https://github.com/espressif/arduino-esp32/issues/863#issuecomment-347179737
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);  // Disable brownout detector
}
#endif // ESP32

/********************************************************************************************/
/*********************SETUP******************************************************************/
/********************************************************************************************/

void setup(void)
{ 
  
#ifdef ESP32
#ifdef DISABLE_ESP32_BROWNOUT
  DisableBrownout();      // Workaround possible weak LDO resulting in brownout detection during Wifi connection
#endif
#endif

  Serial.begin(115200);

  #ifdef USE_SERIAL_ALTERNATE_TX
  // #ifdef DISABLE_SERIAL_ALTERNATE_TX
    Serial.set_tx(2);
  #endif
  Serial.println(F("\n\rRebooting..." DEBUG_INSERT_PAGE_BREAK));
  #ifndef DISABLE_SERIAL_LOGGING
  #ifdef ENABLE_BUG_TRACING
  Serial.println(F("DELAYED BOOT for 5 seconds...")); Serial.flush(); delay(5000);
  #endif
  #endif

  // Init Json builder with memory address and size
  JsonBuilderI->Start(data_buffer.payload.ctr,&data_buffer.payload.len,DATA_BUFFER_PAYLOAD_MAX_LENGTH);
  BufferWriterI->Start(data_buffer.payload.ctr,&data_buffer.payload.len,DATA_BUFFER_PAYLOAD_MAX_LENGTH);

  // Configure WDT
  // msup.WDT_Begin();
  #ifdef USE_DECLARE_AT_COMPILE_TO_DEBUG
  void init_class_instances();
  #endif 
  
  pCONT->Instance_Init();

  // Set boot method
  pCONT_set->seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
  
  // Clear boot status info
  memset(&pCONT_set->boot_status,0,sizeof(pCONT_set->boot_status));

  // All pointers must be set above
  int8_t result = pCONT->Tasker_Interface(FUNC_CHECK_POINTERS);

  if(result != FUNCTION_RESULT_SUCCESS_ID){
    delay(10000); // long enough to cause reset
  }  

  pCONT->InitClassList();
    
  pCONT_set->RtcRebootLoad();
  if(!pCONT_set->RtcRebootValid()) { pCONT_set->RtcReboot.fast_reboot_count = 0; }
  pCONT_set->RtcReboot.fast_reboot_count++;
  pCONT_set->RtcRebootSave();

  pCONT_sup->init_FirmwareVersion();
  
  // // Load config from memory
  pCONT_set->SettingsDefault(); //preload minimal required
  #ifdef ENABLE_SETTINGS_STORAGE
  // // Overwrite with latest values, including template if new SETTINGS_CONFIG exists
  // pCONT_set->SettingsLoad();    //overwrite stored settings from defaults
  // // Check Load was successful
  // pCONT_set->SettingsLoad_CheckSuccessful();
  #endif
   
   
  // #ifndef ENABLE_DEVFEATURE_OTA_METHOD
  // #ifdef ESP8266  // ESP 32 can't start this process until a connection happens
  // #ifdef USE_ARDUINO_OTA
  //   ArduinoOTAInit();
  // #endif // USE_ARDUINO_OTA
  // #endif
  // #endif //   #ifndef ENABLE_DEVFEATURE_OTA_METHOD

  pCONT->Tasker_Interface(FUNC_POINTER_INIT); // confirgure any memory address needed as part of module init or templates
  
  #ifdef FORCE_TEMPLATE_LOADING
  // This will overwrite the settings, temporary, will use a second flag to force template loads "TEMPLATE_HOLDER"
  // need to if template not provided, load defaults else use settings -- add protection in settings defaults to use templates instead (progmem or user desired)
  // Load template before init
    #ifdef ENABLE_LOG
    AddLog_P(LOG_LEVEL_WARN,PSTR(D_LOG_MEMORY D_LOAD "Temporary loading any progmem templates"));
    #endif
  pCONT->Tasker_Interface(FUNC_TEMPLATE_MODULE_LOAD); // loading module, only interface modules will have these
  // load
  // pCONT->Tasker_Interface(FUNC_TEMPLATE_DEVICE_LOAD);  //load/overwrite names AFTER init (FUNC_TEMPLATE_DEVICE_CONFIG_BEFORE_INIT)
   // Set boot method
  pCONT_set->seriallog_level_during_boot = SERIAL_LOG_LEVEL_DURING_BOOT;
  pCONT_set->Settings.seriallog_level = pCONT_set->seriallog_level_during_boot;
  #endif

// pCONT_wif->WifiConnect();


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
  // load
  pCONT->Tasker_Interface(FUNC_TEMPLATE_DEVICE_LOAD);         //load/overwrite names AFTER init (FUNC_TEMPLATE_DEVICE_CONFIG_AFTER_INIT)
  // Configure sensor/drivers to values desired for modules
  pCONT->Tasker_Interface(FUNC_CONFIGURE_MODULES_FOR_DEVICE);
  // init mqtt handlers from memory
  pCONT->Tasker_Interface(FUNC_MQTT_HANDLERS_INIT);
  
  #ifdef ENABLE_FUNCTION_DEBUG
    pCONT->Tasker_Interface(FUNC_DEBUG_CONFIGURE);
  #endif
  // Used to show progress of boot in logs
  pCONT->Tasker_Interface(FUNC_ON_SUCCESSFUL_BOOT);


// #ifdef ESP32
//   // Must connect to wifi before contining on, later make this a flag
//       //set faster until first connect
//       pCONT_wif->connection.config_counter = 10;
//     while(!pCONT_wif->WifiCheckIpConnected()){
//       pCONT_wif->WifiCheck(pCONT_set->wifi_state_flag);
//       pCONT_set->wifi_state_flag = WIFI_RESTART;
//       Serial.println("HELD IN BOOT");
//       delay(500);
//     }    
//     pCONT_wif->connection.config_counter = D_WIFI_CONFIG_SEC; //set to normal after connect success
//     Serial.println("");
//     Serial.println("WiFi connected.");
//     Serial.println("IP address: ");
//     Serial.println(WiFi.localIP());

// #ifndef ENABLE_DEVFEATURE_OTA_METHOD
//     ArduinoOTAInit();
// #endif

// #endif

}

// /********************************************************************************************/
// /*********************loop*******************************************************************/
// /********************************************************************************************/

void loop(void)
{

  DEBUG_LINE;
  pCONT_sup->activity.loop_counter++;
  pCONT_sup->loop_start_millis = millis();
  WDT_RESET();

  #ifdef DEVICE_SIDEDOORLIGHT
  pinMode(5, INPUT);
  #endif
  
  DEBUG_LINE;
  
  // #ifndef ENABLE_DEVFEATURE_OTA_METHOD
  // #ifdef USE_ARDUINO_OTA
  //   ArduinoOtaLoop();
  // #endif  // USE_ARDUINO_OTA
  // #endif // ENABLE_DEVFEATURE_OTA_METHOD

  // #ifdef ENABLE_DEVFEATURE_OTA_METHOD
  #ifdef USE_ARDUINO_OTA
    pCONT_sup->ArduinoOtaLoop();
  #endif  // USE_ARDUINO_OTA
  // #endif

  DEBUG_LINE;
  pCONT->Tasker_Interface(FUNC_LOOP); // EVERY_LOOP

  DEBUG_LINE;
  if(pCONT_time->uptime.seconds_nonreset > 30){ pCONT->Tasker_Interface(FUNC_FUNCTION_LAMBDA_LOOP); } // Only run after stable boot

 
  //move into support, or into time, to align with every_minute, hour, etc
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop50mSec ,50  )){ pCONT->Tasker_Interface(FUNC_EVERY_50_MSECOND);  }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop100mSec,100 )){ pCONT->Tasker_Interface(FUNC_EVERY_100_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop200mSec,200 )){ pCONT->Tasker_Interface(FUNC_EVERY_200_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop250mSec,250 )){ pCONT->Tasker_Interface(FUNC_EVERY_250_MSECOND); }  DEBUG_LINE;
  if(mTime::TimeReached(&pCONT_sup->tSavedLoop1Sec   ,1000)){ pCONT->Tasker_Interface(FUNC_EVERY_SECOND);   };// DEBUG_LINE_HERE;   } 
       
  /********************************************************************************************************
  ******************************************************************************************************** 
    End of loop, only calculate or sleep after this point
  ********************************************************************************************************
  ********************************************************************************************************/
    
  pCONT_sup->loop_runtime_millis = millis() - pCONT_sup->loop_start_millis;

  DEBUG_LINE;

  if(mTime::TimeReached(&pCONT_set->runtime_value.tSavedUpdateLoopStatistics, 1000)){
    pCONT_sup->activity.cycles_per_sec = pCONT_sup->activity.loop_counter; 
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR("LOOPSEC = %d %d"), pCONT_sup->activity.loop_counter, pCONT_sup->activity.cycles_per_sec);
    pCONT_sup->activity.loop_counter=0;
  }

  if(pCONT_sup->loop_runtime_millis > 40){
    Serial.printf("loop_runtime_millis=%d\n\r", pCONT_sup->loop_runtime_millis);
  }

  //  pCONT_mqtt->flag_uptime_reached_reduce_frequency = true;

  // Change this to my own way
  // DO THIS NEXT
  //SmartLoopDelay()
  // #ifndef DISABLE_SLEEP
  // if(pCONT_set->Settings.enable_sleep){
  //   if (pCONT_set->Settings.flag_network.sleep_normal) {
  //     pCONT_sup->SleepDelay(pCONT_set->runtime_value.sleep);
  //   } else {

  //     // Loop time < sleep length of time
  //     if (pCONT_sup->loop_runtime_millis < (uint32_t)pCONT_set->runtime_value.sleep) {
  //       //delay by loop time
  //       pCONT_sup->SleepDelay((uint32_t)pCONT_set->runtime_value.sleep - pCONT_sup->loop_runtime_millis);  // Provide time for background tasks like wifi
  //     } else {

  //       // if loop takes longer than sleep period, no delay, IF wifi is down, devote half loop time to wifi connect
  //       if (pCONT_set->global_state.wifi_down) {
  //         pCONT_sup->SleepDelay(pCONT_sup->loop_runtime_millis /2); // If wifi down and loop_runtime_millis > setoption36 then force loop delay to 1/3 of loop_runtime_millis period
  //       }
  //     }
  //   }
  // }
  // #endif

  // delay(100);

  DEBUG_LINE;
  if (!pCONT_sup->loop_runtime_millis) { pCONT_sup->loop_runtime_millis++; }            // We cannot divide by 0
  pCONT_sup->loop_delay_temp = pCONT_set->runtime_value.sleep; 
  if (!pCONT_sup->loop_delay_temp) { pCONT_sup->loop_delay_temp++; }              // We cannot divide by 0
  pCONT_sup->loops_per_second = 1000 / pCONT_sup->loop_delay_temp;  // We need to keep track of this many loops per second, 20ms delay gives 1000/20 = 50 loops per second (50hz)
  pCONT_sup->this_cycle_ratio = 100 * pCONT_sup->loop_runtime_millis / pCONT_sup->loop_delay_temp;
  pCONT_set->loop_load_avg = pCONT_set->loop_load_avg - (pCONT_set->loop_load_avg / pCONT_sup->loops_per_second) + (pCONT_sup->this_cycle_ratio / pCONT_sup->loops_per_second); // Take away one loop average away and add the new one

  DEBUG_LINE;
  // Create a debug mqtt packet for timings, of main loop and interface loops
  // Serial.printf("%s=%d\r\n","tick",pCONT_sup->loop_runtime_millis);
  // Serial.printf("%s=%d\r\n","tick",pCONT_sup->activity.cycles_per_sec);
  // uint32_t start_millis = millis();
  // AddLog_P(LOG_LEVEL_TEST,PSTR("LOOPSEC = %d \t %d"),loops_per_second,mtel.activity.cycles_per_sec);
  // Serial.printf("ADD TIME = %d\n\r",millis()-start_millis);

  pCONT_set->fSystemRestarted = false; //phase out and use module flag instead

  DEBUG_LINE;
}



