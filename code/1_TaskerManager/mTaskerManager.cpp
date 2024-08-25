#include "1_TaskerManager/mTaskerManager.h"

mTaskerManager* mTaskerManager::instance = nullptr;


int8_t mTaskerManager::Tasker_Interface(uint16_t function, uint16_t target_tasker)
{

  int8_t result = 0;

  if(target_tasker){
    ALOG_INF(PSTR(D_LOG_CLASSLIST "target_tasker %d %s"), target_tasker, GetModuleName_WithUniqueID(target_tasker));
  }

  JsonParserObject obj = 0;
  
  if(function == FUNC_JSON_COMMAND_ID)
  { 
    
  ALOG_DBM(PSTR("buffer_writer before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
  // Serial.println(data_buffer.payload.ctr);
  // delay(1000);
    JsonParser parser(data_buffer.payload.ctr);
  ALOG_DBM(PSTR("buffer_writer after parser ------------- >>>>>>>>>> %d"), JBI->GetBufferSize());

  if(JBI->GetBufferSize()==0)
  {
    Serial.println("error occured");
    // delay(4000);
    return 0;
  }
    
    // Single parsing, for now, make copy as we are modifying the original with tokens, otherwise, no new copy when phased over
    obj = parser.getRootObject();   
    if (!obj) {
      ALOG_ERR(PM_JSON_DESERIALIZATION_ERROR);
  DEBUG_LINE_HERE;
      return 0;
    }else{
      ALOG_DBG(PSTR("JSON PARSED OK"));

  DEBUG_LINE_HERE;
      // JsonParserToken jtok = 0; 
      // if(jtok = obj["Segment0"])
      // {
      //   ALOG_HGL(PSTR("Segment0 YES"));
      // }else{
      //   ALOG_HGL(PSTR("Segment0 MISSING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"));
      // }

    }

  DEBUG_LINE_HERE;
    for(uint8_t i=0;i<GetClassCount();i++)
    { 
      
  DEBUG_LINE_HERE;
      switch_index = target_tasker ? GetEnumNumber_UsingModuleUniqueID(target_tasker) : i; // passed value module is in unique_module_id format
      
  DEBUG_LINE_HERE;
      pModule[switch_index]->Tasker(function, obj);

  DEBUG_LINE_HERE;
      if(target_tasker) {
        ALOG_DBG( PSTR(D_LOG_CLASSLIST "EXECUTED ONCE %d %s"), target_tasker, GetModuleName_WithUniqueID(target_tasker) );
        break; 
      }

    }
    
    return 0; // needs to return via "Tasker"
  } 

  #ifdef  ENABLE_DEBUG_FUNCTION_NAMES
    char buffer_taskname[50];
  #endif

  /**
   * @brief ==============================================================================================================================
   **/
  for(uint8_t i=0;i<GetClassCount();i++)
  {     
        
    #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "========================%d/%d"), i, GetClassCount());  
    #endif
    
    // If target_tasker != 0, then use it, else, use indexed array
    switch_index = target_tasker ? GetEnumNumber_UsingModuleUniqueID(target_tasker) : i; // passed value module is in unique_module_id format
      
    #ifdef ENABLE_ADVANCED_DEBUGGING
    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      #ifdef ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS
      if(pCONT_time->uptime_seconds_nonreset>ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS)
      {
      #endif
      #ifdef ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS
      if(pCONT_time->uptime_seconds_nonreset<ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS)
      {
      #endif
  DEBUG_LINE_HERE;
        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "TIS_%d\t %02d %02d T:%d%S\tM:%S"), millis(), switch_index, i, 
        function, GetTaskName(function, buffer_taskname), 
        GetModuleName(switch_index));  
  DEBUG_LINE_HERE;    
      #ifdef ENABLE_FEATURE_DEBUG_POINT_TASKER_INFO_AFTER_UPSECONDS
      }
      #endif
      #ifdef ENABLE_DEBUG_SHOW_ADVANCED_LOGS_FOR_STARTUP_UPSECONDS
      }
      #endif
    #endif // ENABLE_DEBUG_FUNCTION_NAMES
    #endif // ENABLE_ADVANCED_DEBUGGING    
    #ifdef ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS
    uint32_t long_start_millis = millis();
    #endif

    #if defined(DEBUG_EXECUTION_TIME) || defined(ENABLE_ADVANCED_DEBUGGING)  || defined(ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES)
    uint32_t start_millis = millis();
    ALOG_INF(PSTR("buffer_writer START ------------------------------- >>>>>>>>>> %d"),JBI->GetBufferSize());
    #endif

    /**
     * @brief Special debug cases, when I want to insert debug code that acts on one function type, but for all module calls to aid to tracing an error
     * Uncomment when needed
     */
    // if(function == FUNC_JSON_COMMAND_ID)
    // if(function == FUNC_EVERY_SECOND)
    // {
      // ALOG_COM( PSTR(D_LOG_TASKER "switch_index %d}"), switch_index);
      // ALOG_COM( PSTR(D_LOG_MQTT "{\"CommandsMatched\":%d}"), data_buffer.isserviced);
      
  // ALOG_INF(PSTR("%dM6host_address: %s"), switch_index, pCONT_mqtt->dt.connection[0].host_address);

  // ALOG_INF(PSTR("M---host_address-------- %s"), pCONT_mqtt->dt.connection[0].host_address);
    // }

    pModule[switch_index]->Tasker(function, obj);    

    // if(millis()-long_start_millis > 100)
    //   Serial.printf("time %d %d %d\n\r", millis()-long_start_millis, switch_index, function);

    #if defined(DEBUG_EXECUTION_TIME)  || defined(ENABLE_FEATURE_DEBUG_TASKER_INTERFACE_LOOP_TIMES)
    uint32_t end_millis = millis(); // Remember start millis
    uint32_t this_millis = end_millis - start_millis; // Get this execution time 
    debug_module_time[switch_index].last_loop_time = this_millis;
    debug_module_time[switch_index].avg_time += this_millis;
    debug_module_time[switch_index].avg_time /= 2;
    if(this_millis > debug_module_time[switch_index].max_time){
      debug_module_time[switch_index].max_time = this_millis; // Save Max
      debug_module_time[switch_index].max_function_id = function;
    }
    #endif 
    
    #ifdef ENABLE_ADVANCED_DEBUGGING
    ALOG_INF(PSTR("buffer_writerT ------------------------------- <<<<<<<<<<< END %d"),JBI->GetBufferSize());
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "TIE_%d FUNC time %dms"), millis(), millis()-start_millis);
    #if defined(ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS)
      if(this_millis > ENABLE_DEBUG_FEATURE__TASKER_INTERFACE_SPLASH_LONG_LOOPS_WITH_MS){
        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "TASKER @@@@@@@@@@@@@@@@@@ %d ms %s %S"), millis()-start_millis, GetTaskName(function, buffer_taskname), GetModuleName(switch_index));
      }
    #endif
    #endif // ENABLE_ADVANCED_DEBUGGING
    #ifdef ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS
    uint32_t long_end_millis = millis() - long_start_millis;
    if(long_end_millis > ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS)
    {
      #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      char buffer_taskname2[50];
      ALOG_WRN(PSTR(D_LOG_CLASSLIST "LONG: %d|%d-%S %d-%s %d"), switch_index,i,GetModuleName_WithEnum(switch_index), function,GetTaskName(function, buffer_taskname2), long_end_millis);
      #else
      ALOG_WRN(PSTR(D_LOG_CLASSLIST "TASKER LONG LOOP %d %d %d"), switch_index, function, long_end_millis);      
      #endif
    }
    #endif

    if(target_tasker!=0)
    {
      ALOG_DBM(PSTR(D_LOG_CLASSLIST "target_tasker EXITING EARLY"));
      break; //only run for loop for the class set. if 0, rull all
    }
    
    // Special flag that can be set to end interface ie event handled, no need to check others
    if(fExitTaskerWithCompletion)
    {
      fExitTaskerWithCompletion=false;
      ALOG_INF(PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
      break; //only run for loop for the class set. if 0, rull all
    }
  
  } // end for


  #ifdef ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL
  if(!pCONT_set->flag_boot_complete){
    char buffer_taskname[50];
    if(function != last_function){
      uint8_t boot_percentage = map(function,0,FUNC_ON_BOOT_COMPLETE,0,100);
      //5% = 1 bar, 20 bars total [B                   ]
      //if(pCONT_set->Settings.logging.serial_level >= LOG_LEVEL_INFO){
      #ifndef DISABLE_SERIAL_LOGGING
      DEBUG_PRINTF("[");
      for(uint8_t percent=0;percent<100;percent+=5){  
        if(percent<boot_percentage){ Serial.print((char)219); }else{ DEBUG_PRINTF(" "); }
      }      
      #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      DEBUG_PRINTF("] %03d %s\n\r",boot_percentage, GetTaskName(function, buffer_taskname));
      #else
      DEBUG_PRINTF("] %03d\n\r",boot_percentage);
      #endif // ENABLE_DEBUG_FUNCTION_NAMES
      #endif
      //}
      last_function = function;
    }
  }
  #endif // ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL


  if(function == FUNC_ON_BOOT_COMPLETE){ pCONT_set->runtime.flag_boot_complete = true; }
  
  #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " FINISHED"));
  #endif

  return result;

}



void mTaskerManager::JSONCommand_Run(char* json)
{


  #ifdef USE_LIGHTING_TEMPLATE
    // ALOG_INF(PSTR("buffer_writer Template_Load ------- A >>>>>>>>>> %d"),JBI->GetBufferSize());
    D_DATA_BUFFER_CLEAR();

    sprintf(data_buffer.payload.ctr, "%s", json);

    // ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
    data_buffer.payload.length_used = strlen(data_buffer.payload.ctr);

    data_buffer.payload.ctr[data_buffer.payload.length_used] = '\0'; // to avoid need to memset everything

    // ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());
    ALOG_HGL(PSTR("Template Json Size %d/%d %d%%"), sizeof(LIGHTING_TEMPLATE), data_buffer.payload.length_used, (sizeof(LIGHTING_TEMPLATE)*100)/DATA_BUFFER_PAYLOAD_MAX_LENGTH);

    #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
    Serial.println(data_buffer.payload.ctr);
    #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD


    // ALOG_INF(PSTR("Tasker_Interface before parser ------------ >>>>>>>>>> %d"), JBI->GetBufferSize());

    // ALOG_HGL( PSTR("LIGHTING_TEMPLATE" " READ = \"%s\""), data_buffer.payload.ctr);

    Tasker_Interface(FUNC_JSON_COMMAND_ID);

    // ALOG_INF(PSTR("buffer_writer STTemplate_LoadART ------G- >>>>>>>>>> %d"),JBI->GetBufferSize());

  #endif // USE_LIGHTING_TEMPLATE

  #ifdef ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD
  Serial.println("Template_Load PAUSE");
  delay(10000);
  #endif // ENABLE_DEBUGCRITICAL__STOPPING_CODE_AFTER_TEMPLATE_LOAD

}





void mTaskerManager::addTasker(mTaskerInterface* mod)
{
  pModule.push_back(mod);
  Serial.printf("AddTasker[]\t%S\n\r", mod->GetModuleName());
}


uint8_t mTaskerManager::Instance_Init()
{
    
  /**
   * @brief Core Modules
   **/
  #ifdef USE_MODULE_CORE_SETTINGS
  addTasker(new mSettings());
  #endif 
  #ifdef USE_MODULE_CORE_FILESYSTEM
  addTasker(new mFileSystem());
  #endif
  #ifdef USE_MODULE_CORE_HARDWAREPINS
  addTasker(new mHardwarePins());
  #endif 


  #ifdef USE_MODULE_CORE_SUPPORT
  addTasker(new mSupport());
  #endif 
  #ifdef USE_MODULE_CORE_TIME
  addTasker(new mTime());
  #endif 


  #ifdef USE_MODULE_CORE_LOGGING
  addTasker(new mLogging());
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
  addTasker(new mTelemetry());
  #endif 
  #ifdef USE_MODULE_CORE_RULES
  addTasker(new mRuleEngine());
  #endif
  #ifdef USE_MODULE_CORE_UPDATES
  addTasker(new mUpdates());
  #endif
  // #ifdef USE_MODULE_CORE_SERIAL_UART
  // addTasker(EM_MODULE_CORE_SERIAL_UART_ID, new mSerialUART());
  // #endif
  #ifdef USE_MODULE_CORE__SERIAL
  addTasker(TaskerID::CORE__SERIAL, new mSerial());
  #endif
  #ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
  addTasker(new mDevelopmentDebugging());
  #endif 
  /**
   * @brief Network
   **/
  #ifdef USE_MODULE_NETWORK_INTERFACE
  addTasker(new mInterfaceNetwork());
  #endif 
  #ifdef USE_MODULE_NETWORK_WIFI
  addTasker(new mWiFi());
  #endif 
  #ifdef USE_MODULE_NETWORK_ETHERNET
    //EM_MODULE_NETWORK_ETHERNET_ID,
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif 
  #ifdef USE_MODULE_NETWORK_CELLULAR
  addTasker(EM_MODULE__NETWORK_CELLULAR__ID, new mCellular());
  #endif
  #ifdef USE_MODULE_NETWORK_MQTT
  addTasker(new mMQTTManager());
  #endif 
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  addTasker(new mWebServer());
  #endif
  /**
   * @brief Drivers
   **/
  #ifdef USE_MODULE_DRIVERS_INTERFACE
  addTasker(new mDriverInterface());
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
  INCLUDE_FIX"4_Drivers/03_LEDs/mLEDs.h"
    #define pCONT_led                                static_cast<mLEDs*>(pCONT->pModule[EM_MODULE_DRIVERS_LEDS_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
  addTasker(new mRelays());
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM
    INCLUDE_FIX"4_Drivers/PWM/mPWM.h"
    #define pCONT_pwm                                 static_cast<mPWM*>(pCONT->pModule[EM_MODULE_DRIVERS_PWM_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    INCLUDE_FIX"4_Drivers/IRDevices/mIRtransceiver.h"
    #define pCONT_mdirt                               static_cast<mIRtransceiver*>(pCONT->pModule[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_IRREMOTE
  INCLUDE_FIX"4_Drivers/IRRemote/mIRRemote.h"
    #define pCONT_ir_remote                           static_cast<mIRRemote*>(pCONT->pModule[EM_MODULE_DRIVERS_IRREMOTE_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
    INCLUDE_FIX"4_Drivers/09_RCSwitch/mRCSwitch.h"
    #define pCONT_rcswitch                            static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
    INCLUDE_FIX"4_Drivers/10_RCSwitch_Extended/mRCSwitch_Extended.h"
    #define pCONT_rcswitch                            static_cast<mRCSwitch*>(pCONT->pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    INCLUDE_FIX"4_Drivers/Motors/HBridgeL9110/mHBridge.h"
    #define pCONT_mdhbridge                           static_cast<mHBridge*>(pCONT->pModule[EM_MODULE_DRIVERS_HBRIDGE_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    INCLUDE_FIX"4_Drivers/SD/mSDCard.h"
    #define pCONT_sdcard                              static_cast<mSDCard*>(pCONT->pModule[EM_MODULE_DRIVERS_SDCARD_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    INCLUDE_FIX"4_Drivers/15_ShellyDimmer/mShellyDimmer.h"
    #define pCONT_shelly                              static_cast<mShellyDimmer*>(pCONT->pModule[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
  addTasker(EM_MODULE__DRIVERS_BUZZER_BASIC__ID, new mBuzzer());
  #endif
  #ifdef USE_MODULE__DRIVERS_BUZZER_TONES
    INCLUDE_FIX"4_Drivers/21_Buzzer_Tones/mBuzzer.h"
    #define pCONT_buzzer                              static_cast<mBuzzer*>(pCONT->pModule[EM_MODULE__DRIVERS_BUZZER_TONES__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    INCLUDE_FIX"4_Drivers/50_CAM_OV2640/mCamera.h"
    #define pCONT_mdhbridge                           static_cast<mCamera*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640_2
    INCLUDE_FIX"4_Drivers/Camera_OV2640/mCameraOV2640.h"
    #define pCONT_camera                              static_cast<mCameraOV2640*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_OV2640_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM
    INCLUDE_FIX"4_Drivers/51_WebCam/mWebCam.h"
    #define pCONT_camera                              static_cast<mWebCam*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
    INCLUDE_FIX"4_Drivers/52_WebCamera/mWebCamera.h"
    #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO
    INCLUDE_FIX"4_Drivers/60_WebCam_Arduino/mWebCam.h"
    #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_ARDUINO__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA
    INCLUDE_FIX"4_Drivers/61_WebCam_Tas/mWebCam.h"
    #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_TASMOTA__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_MULTICLIENT
    INCLUDE_FIX"4_Drivers/52_WebCamera/mWebCamera.h"
    #define pCONT_camera                              static_cast<mWebCamera*>(pCONT->pModule[EM_MODULE_DRIVERS__CAMERA_MULTICLIENT__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_DECODER
  addTasker(EM_MODULE__DRIVERS_MAVLINK_DECODER__ID, new mMAVLink_Decoder());
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI
    INCLUDE_FIX"4_Drivers/71_MAVLink_Telemetry_WiFi/mMAVLink_Telemetry_WiFi.h"
    #define pCONT_mavlink                              static_cast<mMAVLink_Telemetry_WiFi*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_WIFI__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR
    INCLUDE_FIX"4_Drivers/72_MAVLink_Telemetry_Cellular/mMAVLink_Telemetry_Cellular.h"
    #define pCONT_mavlink                              static_cast<mMAVLink_Telemetry_Cellular*>(pCONT->pModule[EM_MODULE__DRIVERS_MAVLINK_TELEMETRY_CELLULAR__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_7000G
  INCLUDE_FIX"4_Drivers/80_Modem_SIM7000G/mSIM7000G.h"
    #define pCONT_sim7000g                           static_cast<mSIM7000G*>(pCONT->pModule[EM_MODULE_DRIVERS__MODEM_7000G__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_800L
  INCLUDE_FIX"4_Drivers/81_Modem_SIM800L/mSIM800L.h"
    #define pCONT_sim800l                           static_cast<mSIM800L*>(pCONT->pModule[EM_MODULE_DRIVERS__MODEM_800L__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  /**
   * @brief Sensors
   **/
  #ifdef USE_MODULE_SENSORS_INTERFACE
  addTasker(new mSensorsInterface());
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSwitches());
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
  addTasker(EM_MODULE_SENSORS_BUTTONS_ID, new mButtons());
  #endif
  #ifdef USE_MODULE_SENSORS_BME
  addTasker(new mBME());
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
  addTasker(EM_MODULE_SENSORS_DHT_ID, new mSensorsDHT());
  #endif
  #ifdef USE_MODULE_SENSORS_BH1750
  addTasker(new mBH1750());
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
  addTasker(EM_MODULE_SENSORS_MOTION_ID, new mMotion());
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    INCLUDE_FIX"5_Sensors/11_Door/mDoorSensor.h"
    #define pCONT_sdoor                           static_cast<mDoorSensor*>(pCONT->pModule[EM_MODULE_SENSORS_DOOR_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_L3G
    INCLUDE_FIX"5_Sensors/L3GD20_3Axis_Gryo/mSensorsL3G.h"
    #define pCONT_L3G                      static_cast<mSensorsL3G*>(pCONT->pModule[EM_MODULE_SENSORS_L3G_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
  addTasker(EM_MODULE_SENSORS_LDR_BASIC_ID, new mLDRBasic());
  #endif
  #ifdef USE_MODULE_SENSORS_LSM303D
    INCLUDE_FIX"5_Sensors/LSM303D_3Axis_AccMag/mSensorsLSM303D.h"
    #define pCONT_LSM303D                      static_cast<mSensorsLSM303D*>(pCONT->pModule[EM_MODULE_SENSORS_LSM303D_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    INCLUDE_FIX"5_Sensors/Moisture/mMoistureSensor.h"
    #define pCONT_srmoisture                      static_cast<mMoistureSensor*>(pCONT->pModule[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
  addTasker(EM_MODULE_SENSORS_SR04_ID, new mSR04());
  #endif
  #ifdef USE_MODULE_SENSORS_MPU9250
    INCLUDE_FIX"5_Sensors/MPU9250/mSensorsMPU9250.h"
    #define pCONT_MPU9250                      static_cast<mSensorsMPU9250*>(pCONT->pModule[EM_MODULE_SENSORS_MPU9250_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    INCLUDE_FIX"5_Sensors/PulseCounter/mPulseCounter.h"
    #define pCONT_spulse                          static_cast<mPulseCounter*>(pCONT->pModule[EM_MODULE_SENSORS_PULSECOUNTER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
    INCLUDE_FIX"5_Sensors/20_RemoteDevice/mRemoteDevice.h"
    #define pCONT_sremote                           static_cast<mRemoteDevice*>(pCONT->pModule[EM_MODULE_SENSORS_REMOTE_DEVICE_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
    INCLUDE_FIX"5_Sensors/21_RotaryEncoder/mRotaryEncoder.h"
    #define pCONT_rotary_encoder               static_cast<mRotaryEncoder*>(pCONT->pModule[EM_MODULE_SENSORS_ROTARY_ENCODER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_SOLAR_LUNAR
  addTasker(new mSolarLunar());
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    INCLUDE_FIX"5_Sensors/UltraSonic/mUltraSonicSensor.h"
    #define pCONT_ult                             static_cast<mUltraSonicSensor*>(pCONT->pModule[EM_MODULE_SENSORS_ULTRASONIC_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
    INCLUDE_FIX"5_Sensors/30_ADCInternal_ESP82/mADCInternal.h"
    #define pCONT_adc_internal                      static_cast<mADCInternal*>(pCONT->pModule[EM_MODULE_SENSORS_ADC_INTERNAL_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
    INCLUDE_FIX"5_Sensors/31_ADCInternal_ESP32/mADCInternal.h"
    #define pCONT_adc_internal                      static_cast<mADCInternal*>(pCONT->pModule[EM_MODULE_SENSORS_ADC_INTERNAL_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023
    INCLUDE_FIX"5_Sensors/40_ds18x20/mDB18x20.h"
    #define pCONT_db18                      static_cast<mDB18x20_ESP32*>(pCONT->pModule[EM_MODULE_SENSORS__DS18X20__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
  addTasker(new mDB18x20_ESP32());
  #endif
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
  addTasker(EM_MODULE__SENSORS_GPS_SERIAL__ID, new mGPS_Serial());
  #endif
  #ifdef USE_MODULE_SENSORS_GPS_MODEM
    INCLUDE_FIX"5_Sensors/51_GPS_Modem/mGPS_Modem.h"
    #define pCONT_gps                                 static_cast<mGPS_Modem*>(pCONT->pModule[EM_MODULE__SENSORS_GPS_MODEM__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
    INCLUDE_FIX"5_Sensors/52_Battery_Modem/mBattery_Modem.h"
    #define pCONT_batt_modem                                 static_cast<mBattery_Modem*>(pCONT->pModule[EM_MODULE__SENSORS_BATTERY_MODEM__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  /**
   * @brief Lights
   **/
  #ifdef USE_MODULE_LIGHTS_INTERFACE
  addTasker(new mInterfaceLight());
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
  addTasker(new mAnimatorLight());
  #endif
  /**
   * @brief Energy
   **/
  #ifdef USE_MODULE_ENERGY_INTERFACE
  addTasker(new mEnergyInterface());
  #endif
  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
  addTasker(new mEnergyPZEM004T());
  #endif
  #ifdef USE_MODULE_ENERGY_ADE7953
    INCLUDE_FIX"7_Energy/02_ADE7953/mADE7953.h"
    #define pCONT_ade7953                         static_cast<mEnergyADE7953*>(pCONT->pModule[EM_MODULE_ENERGY_ADE7953_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_ENERGY_INA219
  addTasker(new mEnergyINA219());
  #endif
  /**
   * @brief Displays
   **/
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
  addTasker(new mDisplaysInterface());
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
  addTasker(new mNextionPanel());
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
    INCLUDE_FIX"8_Displays/02_OLED_SSD1606/mOLED_SSD1306.h"
    #define pCONT_oled1306                            static_cast<mOLED_SSD1306*>(pCONT->pModule[EM_MODULE_DISPLAYS_OLED_SSD1306_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
  addTasker(new mOLED_SH1106());
  #endif
  /**
   * @brief Controller Generic (Generic)
   **/
  #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
  addTasker(EM_MODULE_CONTROLLER_SONOFF_IFAN_ID, new mSonoffIFan());
  #endif
  #ifdef USE_MODULE_CONTROLLER_FAN
    INCLUDE_FIX"9_Controller/Fan/mFan.h"
    #define pCONT_mfan                            static_cast<mFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_FAN_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  // 3d printer encoder here
  #ifdef USE_MODULE_CONTROLLER_TANKVOLUME
    INCLUDE_FIX"9_Controller/08_TankVolume/mTankVolume.h"
    #define pCONT_tankvolume                      static_cast<mTankVolume*>(pCONT->pModule[EM_MODULE_CONTROLLER_TANKVOLUME_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_BLINDS
    INCLUDE_FIX"9_Controller/Blinds/mBlinds.h"
    #define pCONT_sbut                            static_cast<mBlinds*>(pCONT->pModule[EM_MODULE_SENSORS_BUTTONS_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
    INCLUDE_FIX"9_Controller/BucketWaterLevel/mBucketWaterLevel.h"
    #define pCONT_bucket_water_level                static_cast<mBucketWaterLevel*>(pCONT->pModule[EM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    INCLUDE_FIX"9_Controller/DoorBell/mDoorBell.h"
    #define pCONT_doorbell                        static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_DOORBELL_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
    INCLUDE_FIX"9_Controller/GPS_SD_Logger/mGPS_SD_Logger.h"
    #define pCONT_gps_sd_log                  static_cast<mGPS_SD_Logger*>(pCONT->pModule[EM_MODULE_CONTROLLER_GPS_SD_LOGGER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
    #define pCONT_serial_pos_log  pCONT_gps_sd_log
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
    INCLUDE_FIX"9_Controller/SDCardLogger/mSDCardLogger.h"
    #define pCONT_sdcardlogger                    static_cast<mDoorBell*>(pCONT->pModule[EM_MODULE_CONTROLLER_SDCARDLOGGER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  // USE_MODULE_CONTROLLER_UARTLOGGER
  // USE_MODULE_CONTROLLER_INTERNAL_CLOCK
  #ifdef USE_MODULE_CONTROLLER_HVAC
  addTasker(new mHVAC());
  #endif
  #ifdef USE_MODULE_CONTROLLER_HVAC_REMOTE
    INCLUDE_FIX"9_Controller/41_HVAC_Remote/mHVAC.h"
    #define pCONT_hvac_remote                        static_cast<mHVAC_Remote*>(pCONT->pModule[EM_MODULE_CONTROLLER__HVAC_REMOTE__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
  addTasker(new mSensorColourBar());
  #endif
  #ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
  addTasker(new mRelayStateLEDStrip());
  #endif
  // 50_Animator_Input_Controller
  // 51_Animator_Playlists
  #ifdef USE_MODULE_CONTROLLER__ENERGY_OLED
    INCLUDE_FIX"9_Controller/60_EnergyOnOLED/mEnergyOnOLED.h"
    #define pCONT_mEnergyOnOLED        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER__ENERGY_OLED__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  /**
   * @brief 10 Controller Custom
   **/  
  #ifdef USE_MODULE_CONTROLLER_RADIATORFAN
    INCLUDE_FIX"10_ConSpec/00_RadiatorFan/mRadiatorFan.h"
    #define pCONT_sbut                            static_cast<mRadiatorFan*>(pCONT->pModule[EM_MODULE_CONTROLLER_RADIATORFAN_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
    INCLUDE_FIX"10_ConSpec/01_ImmersionTankColour/mImmersionTankColour.h"
    #define pCONT_msenscol                        static_cast<mImmersionTankColour*>(pCONT->pModule[EM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
    INCLUDE_FIX"10_ConSpec/03_FurnaceSensor/mFurnaceSensor.h"
    #define pCONT_furnace_sensor                static_cast<mFurnaceSensor*>(pCONT->pModule[EM_MODULE_CONTROLLER_FURNACE_SENSOR_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB
    INCLUDE_FIX"10_ConSpec/04_LouvoliteHub/mLouvoliteHub.h"
    #define pCONT_louv                static_cast<mLouvoliteHub*>(pCONT->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2
    INCLUDE_FIX"10_ConSpec/04v2_LouvoliteHub/mLouvoliteHub.h"
    #define pCONT_louv                static_cast<mLouvoliteHub*>(pCONT->pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
    INCLUDE_FIX"10_ConSpec/05_SideDoorLight/mSideDoorLight.h"
    #define pCONT_sdlight                static_cast<mSideDoorLight*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHT__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
    INCLUDE_FIX"10_ConSpec/06_ImmersionPanel/mImmersionPanel.h"
    #define pCONT_immersion_cont         static_cast<mImmersionPanel*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
    INCLUDE_FIX"10_ConSpec/07_TempSensorOnOLED/mTempSensorOLEDBath.h"
    #define pCONT_immersion_cont         static_cast<mTempSensorOLEDBath*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX
    INCLUDE_FIX"10_ConSpec/08_CellularBlackBox/mCellularBlackBox.h"
    #define pCONT_immersion_cont         static_cast<mCellularBlackBox*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_BLACK_BOX__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED
    INCLUDE_FIX"10_ConSpec/09_mMAVLink_Decoder_OLED/mMAVLink_Decoder_OLED.h"
    #define pCONT_cont_mavlinoled         static_cast<mMAVLink_Decoder_OLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__CELLULAR_MAVLINK_BLACK_BOX_OLED__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
  addTasker(EM_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED__ID, new mEnergyOLED());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE
    INCLUDE_FIX"10_ConSpec/11_3DPrinterEnclosure/Printer3D.h"
    #define pCONT_3dprinter        static_cast<mPrinter3D*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER
    INCLUDE_FIX"10_ConSpec/12_TreadmillLogger/mTreadmillLogger.h"
    #define pCONT_treadmill        static_cast<mEnergyOnOLED*>(pCONT->pModule[EM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER__ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
    INCLUDE_FIX"9_Controller/SDLoggerIMURadiationPattern/mSDLogger.h"
    #define pCONT_cont_imu_rad                static_cast<mSDLoggerIMURadiationPattern*>(pCONT->pModule[EM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    INCLUDE_FIX"9_Controller/SerialPositionalLogger/mSerialPositionalLogger.h"
    #define pCONT_serial_pos_log                  static_cast<mSerialPositionalLogger*>(pCONT->pModule[EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
    INCLUDE_FIX"9_Controller/SerialCalibrationMeasurmentLogger/mSerialCalibrationMeasurmentLogger.h"
    #define pCONT_serial_calibration_log                  static_cast<mSerialCalibrationMeasurmentLogger*>(pCONT->pModule[EM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__DESK_SENSORS_ON_OLED
  addTasker(new mDeskSensorsOnOLED());
  #endif
  #ifdef USE_MODULE_CONTROLLER_USERMOD_01
    INCLUDE_FIX"9_Controller/UserMod_01/mUserMod_01.h"
    #define pCONT_usermod_01                  static_cast<mUserMod_01*>(pCONT->pModule[EM_MODULE_CONTROLLER_USERMOD_01_ID])
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS
  addTasker(TaskerID::CONTROLLER_CUSTOM__MAVLINK_FLYING_LEDS__ID, new mMavlinkFlyingLEDS());
  #endif


  Serial.printf(D_LOG_CLASSLIST "Loaded %d|%d modules\n\r",  pModule.size(), GetClassCount());
  if(pModule.size() != GetClassCount())
  {    
    Serial.printf(D_LOG_CLASSLIST "ERROR, mismatch in modules added\n\r");
    delay(5000);
  }

};


uint16_t mTaskerManager::GetClassCount()
{
  return pModule.size();
}


uint16_t mTaskerManager::GetClassSizeByID(uint8_t class_id)
{
  #ifdef USE_DEBUG_CLASS_SIZE
  return pModule[class_id]->GetClassSize();
  #else
  return 0;
  #endif
}


int16_t mTaskerManager::GetModuleIndexbyName(const char* c)
{

  ALOG_WRN( PSTR("GetModuleIndexbyName, should this be GetModule_UniqueID_byName") );

  if(*c == '\0'){ return -1; }
  for(int ii=0;ii<GetClassCount();ii++){
    if(strcasecmp_P(c, pModule[ii]->GetModuleName())==0){
      //AddLog(LOG_LEVEL_TEST, PSTR("MATCHED GetModuleIDbyName \"%s\" => \"%s\" %d"),c,pModule[ii]->GetModuleName(),ii);
      return ii;
    }    
  }
  return -1;
}


uint16_t mTaskerManager::GetModule_UniqueID_byName(const char* c)
{
  if(*c=='\0'){ return -1; }
  for(int ii=0;ii<GetClassCount();ii++){
    if(strcasecmp_P(c, pModule[ii]->GetModuleName())==0){
      //AddLog(LOG_LEVEL_TEST, PSTR("MATCHED GetModuleIDbyName \"%s\" => \"%s\" %d"),c,pModule[ii]->GetModuleName(),ii);
      return pModule[ii]->GetModuleUniqueID();
    }    
  }
  return 0;
}


uint16_t mTaskerManager::GetEnumNumber_UsingModuleUniqueID(uint16_t unique_id)
{
  for(int ii=0;ii<GetClassCount();ii++)
  {
    if( unique_id == pModule[ii]->GetModuleUniqueID() )
    {
      return ii;
    }
  }
}


int16_t mTaskerManager::GetModuleUniqueIDbyName(const char* c)
{
  int16_t index = GetModuleIndexbyName(c);
  if(index != -1)
  {
    return GetModuleUniqueIDbyVectorIndex(index);
  }
}


/**
 * @brief Using the unique ID each module must have, get the TaskerInterface array enum ID
 * */
int16_t mTaskerManager::GetEnumVectorIndexbyModuleUniqueID(int16_t unique_id)
{
  for(int ii=0;ii<GetClassCount();ii++)
  {
    if(unique_id == pModule[ii]->GetModuleUniqueID())
    {
      return ii; //return array index
    }
  }
  return -1;
}


uint16_t mTaskerManager::GetModuleUniqueIDbyVectorIndex(uint8_t id)
{
  return pModule[id]->GetModuleUniqueID();
}


mTaskerInterface* mTaskerManager::GetModuleObjectbyUniqueID(uint16_t id)
{
  int16_t mod_id = GetEnumVectorIndexbyModuleUniqueID(id);
  return mod_id >= 0 ? pModule[mod_id] : nullptr;
}

/**
 * @brief Must check for validity, if not, returns nullptr
 * @param id 
 * @return mTaskerInterface* 
 */
bool mTaskerManager::ValidTaskID(uint8_t id)
{
  return id <= GetClassCount() ? true : false;
}


PGM_P mTaskerManager::GetModuleName_WithUniqueID(uint16_t unique_id)
{
  uint8_t enum_id = GetEnumVectorIndexbyModuleUniqueID(unique_id);
  if(ValidTaskID(enum_id))
  {
    return pModule[enum_id]->GetModuleName();
  }  
  return PM_SEARCH_NOMATCH;
}


PGM_P mTaskerManager::GetModuleName_WithEnum(uint16_t id)
{
  if(ValidTaskID(id))
  {
    return pModule[id]->GetModuleName();
  }  
  return PM_SEARCH_NOMATCH;
}
  

mTaskerInterface* mTaskerManager::GetModule_IndexEnum(uint8_t id_enum)
{
  if(ValidTaskID(id_enum))
  {
    return pModule[id_enum];
  }  
  return pModule[0]; // Don't return nullptr, as it will crash when used. Instead, return a module as default
}
  
mTaskerInterface* mTaskerManager::GetModule_IndexUnique(uint16_t id_unique)
{
  uint16_t id_enum = GetEnumVectorIndexbyModuleUniqueID(id_unique);
  ALOG_INF(PSTR("GetModule_IndexUnique %d %d"), id_unique, id_enum);
  if(ValidTaskID(id_enum))
  {
    return pModule[id_enum];
  }  
  return pModule[0]; // Don't return nullptr, as it will crash when used. Instead, return a module as default
}
  


  
#ifdef ENABLE_DEBUG_FUNCTION_NAMES

// should be moved into task.h

// Switch case should be faster than getext progmem
// Use progmem WITHOUT buffer for speed improvements, should be read as expected progmem and handled that way
const char* mTaskerManager::GetTaskName(uint8_t task, char* buffer){

  DEBUG_LINE_HERE;    
  switch(task){
    default:
                                                      return PM_SEARCH_NOMATCH;
    case FUNC_POINTER_INIT:                           return PM_FUNC_POINTER_INIT_CTR;
    case FUNC_TEMPLATES__LOAD_MODULE:      return PM_FUNC_TEMPLATE_LOAD_CTR;
    case FUNC_PRE_INIT:                               return PM_FUNC_PRE_INIT_CTR;
    case FUNC_INIT:                                   return PM_FUNC_INIT_CTR;
    case FUNC_CONFIGURE_MODULES_FOR_DEVICE:           return PM_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR;
    case FUNC_LOOP:                                   return PM_FUNC_LOOP_CTR;
    case FUNC_EVERY_50_MSECOND:                       return PM_FUNC_EVERY_50_MSECOND_CTR;
    case FUNC_EVERY_100_MSECOND:                      return PM_FUNC_EVERY_100_MSECOND_CTR;
    case FUNC_EVERY_250_MSECOND:                      return PM_FUNC_EVERY_250_MSECOND_CTR;
    case FUNC_EVERY_SECOND:                           return PM_FUNC_EVERY_SECOND_CTR;
    case FUNC_EVERY_MINUTE:                           return PM_FUNC_EVERY_MINUTE_CTR; 
    case FUNC_EVERY_HOUR:                             return PM_FUNC_EVERY_HOUR_CTR; 
    case FUNC_EVERY_MIDNIGHT:                         return PM_FUNC_EVERY_MIDNIGHT_CTR;
    case FUNC_EVERY_MIDDAY:                           return PM_FUNC_EVERY_MIDDAY_CTR;
    case FUNC_ON_BOOT_SUCCESSFUL:                     return PM_FUNC_ON_SUCCESSFUL_BOOT_CTR;
    case FUNC_UPTIME_10_SECONDS:                      return PM_FUNC_UPTIME_10_SECONDS_CTR;
    case FUNC_UPTIME_1_MINUTES:                       return PM_FUNC_UPTIME_1_MINUTES_CTR;
    case FUNC_UPTIME_10_MINUTES:                      return PM_FUNC_UPTIME_10_MINUTES_CTR;
    case FUNC_UPTIME_60_MINUTES:                      return PM_FUNC_UPTIME_60_MINUTES_CTR;
    case FUNC_JSON_APPEND:                            return PM_FUNC_JSON_APPEND_CTR;
    case FUNC_SAVE_BEFORE_RESTART:                    return PM_FUNC_SAVE_BEFORE_RESTART_CTR;
    case FUNC_SETTINGS_DEFAULT:                       return PM_FUNC_SETTINGS_DEFAULT_CTR;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:    return PM_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE:       return PM_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE:       return PM_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
    case FUNC_FUNCTION_LAMBDA_INIT:                   return PM_FUNC_FUNCTION_LAMBDA_INIT_CTR;
    case FUNC_FUNCTION_LAMBDA_LOOP:                   return PM_FUNC_FUNCTION_LAMBDA_LOOP_CTR;
    // case FUNC_COMMAND:                                return PM_FUNC_COMMAND_CTR;
    // case FUNC_COMMAND_SENSOR:                         return PM_FUNC_COMMAND_SENSOR_CTR;
    // case FUNC_COMMAND_DRIVER:                         return PM_FUNC_COMMAND_DRIVER_CTR;
    // case FUNC_JSON_COMMAND:                           return PM_FUNC_JSON_COMMAND_CTR;
    // case FUNC_JSON_COMMAND_OBJECT:                    return PM_FUNC_JSON_COMMAND_OBJECT_CTR;
    case FUNC_WIFI_CONNECTED:                         return PM_FUNC_WIFI_CONNECTED_CTR;
    case FUNC_WIFI_DISCONNECTED:                      return PM_FUNC_WIFI_DISCONNECTED_CTR;
    case FUNC_MQTT_SUBSCRIBE:                         return PM_FUNC_MQTT_SUBSCRIBE_CTR;
    // case FUNC_MQTT_INIT:                              return PM_FUNC_MQTT_INIT_CTR;
    case FUNC_MQTT_CONNECTED:                         return PM_FUNC_MQTT_CONNECTED_CTR;
    case FUNC_MQTT_DISCONNECTED:                      return PM_FUNC_MQTT_DISCONNECTED_CTR;
    case FUNC_MQTT_COMMAND:                           return PM_FUNC_MQTT_COMMAND_CTR;
    case FUNC_MQTT_SENDER:                            return PM_FUNC_MQTT_SENDER_CTR;
    case FUNC_MQTT_HANDLERS_INIT:                     return PM_FUNC_MQTT_HANDLERS_INIT_CTR;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:       return PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
    case FUNC_SET_POWER:                              return PM_FUNC_SET_POWER_CTR;
    case FUNC_SET_DEVICE_POWER:                       return PM_FUNC_SET_DEVICE_POWER_CTR;
    case FUNC_SHOW_SENSOR:                            return PM_FUNC_SHOW_SENSOR_CTR;
    case FUNC_RULES_PROCESS:                          return PM_FUNC_RULES_PROCESS_CTR;
    case FUNC_SERIAL:                                 return PM_FUNC_SERIAL_CTR;
    case FUNC_FREE_MEM:                               return PM_FUNC_FREE_MEM_CTR;
    case FUNC_BUTTON_PRESSED:                         return PM_FUNC_BUTTON_PRESSED_CTR;
    case FUNC_ENERGY_RESET:                           return PM_FUNC_ENERGY_RESET_CTR;
    case FUNC_SENSOR_UPDATED:                         return PM_FUNC_SENSOR_UPDATED_CTR;
    case FUNC_STATUS_MESSAGE_APPEND:                  return PM_FUNC_STATUS_MESSAGE_APPEND_CTR;
    case FUNC_JSON_HARDWARE_APPEND:                   return PM_FUNC_JSON_HARDWARE_APPEND_CTR;
    case FUNC_SET_CHANNELS:                           return PM_FUNC_SET_CHANNELS_CTR;
    case FUNC_WEB_ADD_MAIN_BUTTON:                    return PM_FUNC_WEB_ADD_MAIN_BUTTON_CTR;
    case FUNC_WEB_ADD_BUTTON:                         return PM_FUNC_WEB_ADD_BUTTON_CTR;
    case FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS:         return PM_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
    case FUNC_WEB_ADD_HANDLER:                        return PM_FUNC_WEB_ADD_HANDLER_CTR;
    case FUNC_WEB_ROOT_SEND_STYLE:                    return PM_FUNC_WEB_ROOT_SEND_STYLE_CTR;
    case FUNC_WEB_ROOT_SEND_SCRIPT:                   return PM_FUNC_WEB_ROOT_SEND_SCRIPT_CTR;
    case FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING:return PM_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
    case FUNC_WEB_ROOT_SEND_BODY:                     return PM_FUNC_WEB_ROOT_SEND_BODY_CTR;
    case FUNC_WEB_ROOT_SEND_STATUS:                   return PM_FUNC_WEB_ROOT_SEND_STATUS_CTR;
    case FUNC_WEB_ADD_ROOT_SHOWS:                     return PM_FUNC_WEB_ADD_ROOT_SHOWS_CTR;
    case FUNC_WEB_ADD_JSON_FETCH_RESULT:              return PM_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR;
    case FUNC_WEB_ADD_JSON_DATA_FETCH_URL:            return PM_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR;
    case FUNC_WEB_ADD_ROOT_TABLE_ROWS:                return PM_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR;
    case FUNC_WEB_ADD_ROOT_SCRIPT:                    return PM_FUNC_WEB_ADD_ROOT_SCRIPT_CTR;
    case FUNC_WEB_ADD_ROOT_STYLE:                     return PM_FUNC_WEB_ADD_ROOT_STYLE_CTR;
    // case FUNC_WEB_APPEND_LOADTIME_ROOT_URLS:          return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
    // case FUNC_WEB_APPEND_LOADTIME_ROOT_RATES:         return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
    case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:           return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
    // case FUNC_WEB_APPEND_RUNTIME_ROOT_RATES:          return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED: return PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
    case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED:    return PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
    case FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS:       return PM_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
    case FUNC_WEB_APPEND_ROOT_BUTTONS:                return PM_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR;
    case FUNC_WEB_PAGEINFORMATION_SEND_MODULE:        return PM_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
    case FUNC_WEB_COMMAND:                            return PM_FUNC_WEB_COMMAND_CTR;
    // case FUNC_CHECK_POINTERS:                         return PM_FUNC_CHECK_POINTERS_CTR;
    case FUNC_WEB_SYSTEM_INFO:                        return PM_FUNC_WEB_SYSTEM_INFO_CTR;
    case FUNC_DEBUG_CONFIGURE:                        return PM_FUNC_DEBUG_CONFIGURE_CTR;
  }

}

    #endif // ENABLE_DEBUG_FUNCTION_NAMES
