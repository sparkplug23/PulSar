#include "1_TaskerManager/mTaskerManager.h"

mTaskerManager* mTaskerManager::instance = nullptr;


int8_t mTaskerManager::Tasker_Interface(uint16_t function, uint16_t target_tasker)
{

  int8_t result = 0;

  if(target_tasker){
    ALOG_INF(PSTR(D_LOG_CLASSLIST "target_tasker %d %s"),target_tasker,GetModuleFriendlyName_WithUniqueID(target_tasker));
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
      return 0;
    }else{
      ALOG_DBG(PSTR("JSON PARSED OK"));

      // JsonParserToken jtok = 0; 
      // if(jtok = obj["Segment0"])
      // {
      //   ALOG_HGL(PSTR("Segment0 YES"));
      // }else{
      //   ALOG_HGL(PSTR("Segment0 MISSING >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"));
      // }

    }

    for(uint8_t i=0;i<GetClassCount();i++)
    { 
      
      switch_index = target_tasker ? GetEnumNumber_UsingModuleUniqueID(target_tasker) : i; // passed value module is in unique_module_id format
      
      pModule[switch_index]->Tasker(function, obj);

      if(target_tasker) {
        ALOG_DBG( PSTR(D_LOG_CLASSLIST "EXECUTED ONCE %d %s"), target_tasker, GetModuleFriendlyName_WithUniqueID(target_tasker) );
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
        
    // AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "========================%d/%d"), i, GetClassCount());  
    
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
        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "TIS_%d\t %02d %02d T:%S\tM:%S"), millis(), switch_index, i, 
        GetTaskName(function, buffer_taskname), 
        GetModuleFriendlyName(switch_index));      
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
    
    pModule[switch_index]->Tasker(function, obj);    

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
        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "TASKER @@@@@@@@@@@@@@@@@@ %d ms %s %S"), millis()-start_millis, GetTaskName(function, buffer_taskname), GetModuleFriendlyName(switch_index));
      }
    #endif
    #endif // ENABLE_ADVANCED_DEBUGGING
    #ifdef ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS
    uint32_t long_end_millis = millis() - long_start_millis;
    if(long_end_millis > ENABLE_DEBUGFEATURE_TASKER_INTERFACE__LONG_LOOPS)
    {
      #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      char buffer_taskname2[50];
      ALOG_WRN(PSTR(D_LOG_CLASSLIST "TASKER LONG: %d-%s %d-%S %d"), function,GetTaskName(function, buffer_taskname2), switch_index,GetModuleFriendlyName(switch_index), long_end_millis);
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
      ALOG_DBM(PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
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





void mTaskerManager::addTasker(TaskerID id, mTaskerInterface* mod)
{
  pModule.push_back(mod);
  Serial.printf("AddTasker[%d]\t%S\n\r", id, mod->GetModuleFriendlyName());
}


uint8_t mTaskerManager::Instance_Init(){

  // Core
  #ifdef USE_MODULE_CORE_HARDWAREPINS
  addTasker(EM_MODULE_CORE_HARDWAREPINS_ID, new mHardwarePins());
  #endif 
  #ifdef USE_MODULE_CORE_SETTINGS
  addTasker(EM_MODULE_CORE_SETTINGS_ID, new mSettings());
  #endif 
  #ifdef USE_MODULE_CORE_SUPPORT
  addTasker(EM_MODULE_CORE_SUPPORT_ID, new mSupport());
  #endif 
  #ifdef USE_MODULE_CORE_LOGGING
  addTasker(EM_MODULE_CORE_LOGGING_ID, new mLogging());
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
  addTasker(EM_MODULE_CORE_TELEMETRY_ID, new mTelemetry());
  #endif 
  #ifdef USE_MODULE_CORE_TIME
  addTasker(EM_MODULE_CORE_TIME_ID, new mTime());
  #endif 
  #ifdef USE_MODULE_CORE_TIME2
  addTasker(EM_MODULE_CORE_TIME2_ID, new mTime2());
  #endif 
  #ifdef USE_MODULE_CORE_RULES
  addTasker(EM_MODULE_CORE_RULES_ID, new mRuleEngine());
  #endif
  #ifdef USE_MODULE_CORE_UPDATES
  addTasker(EM_MODULE_CORE_UPDATES_ID, new mUpdates());  // this should be enabled by default and OTA/HTTP flashing moved here
  #endif
  #ifdef USE_MODULE_CORE__SERIAL
  addTasker(TaskerID::CORE__SERIAL, new mSerial());  // this should be enabled by default and OTA/HTTP flashing moved here
  #endif
  #ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
  addTasker(EM_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID, new mDevelopmentDebugging());
  #endif
  
  // Network  
  #ifdef USE_MODULE_NETWORK_INTERFACE
  addTasker(EM_MODULE__NETWORK_INTERFACE__ID, new mInterfaceNetwork());
  #endif 
  #ifdef USE_MODULE_NETWORK_WIFI
  addTasker(EM_MODULE_NETWORK_WIFI_ID, new mWiFi());
  #endif 
  #ifdef USE_MODULE_NETWORK_CELLULAR
  addTasker(EM_MODULE__NETWORK_CELLULAR__ID, new mCellular());
  #endif
  #ifdef USE_MODULE_NETWORK_MQTT
  addTasker(EM_MODULE_NETWORK_MQTT_ID, new mMQTT());
  #endif 
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  addTasker(EM_MODULE_NETWORK_WEBSERVER_ID, new mWebServer());
  #endif

  // Displays
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
    addTasker(EM_MODULE_DISPLAYS_INTERFACE_ID, new mDisplaysInterface());
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    addTasker(EM_MODULE_DISPLAYS_NEXTION_ID, new mNextionPanel());
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
    addTasker(EM_MODULE_DISPLAYS_OLED_SSD1306_ID, new mOLED_SSD1306());
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
    addTasker(EM_MODULE_DISPLAYS_OLED_SH1106_ID, new mOLED_SH1106());
  #endif

  // Drivers
  #ifdef USE_MODULE_DRIVERS_INTERFACE
    addTasker(EM_MODULE_DRIVERS_INTERFACE_ID, new mDriverInterface());
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    addTasker(EM_MODULE_DRIVERS_HBRIDGE_ID, new X());
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    addTasker(EM_MODULE_DRIVERS_IRTRANSCEIVER_ID, new X());
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    addTasker(EM_MODULE_DRIVERS_RELAY_ID, new mRelays());
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM
    addTasker(EM_MODULE_DRIVERS_PWM_ID, new mPWM());
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    addTasker(EM_MODULE_DRIVERS_SDCARD_ID, new mSDCard());
  #endif
  #ifdef USE_MODULE_DRIVERS_SERIAL_UART
    addTasker(EM_MODULE_DRIVERS_SERIAL_UART_ID, new mSerialUART());
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    addTasker(EM_MODULE_DRIVERS_SHELLY_DIMMER_ID, new mShellyDimmer());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    addTasker(EM_MODULE_DRIVERS_CAMERA_OV2640_ID, new mCameraOV2640());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640_2
    addTasker(EM_MODULE_DRIVERS_CAMERA_OV2640_ID, new mCameraOV2640());
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
    addTasker(EM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_ID, new mWebCamera());
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
    addTasker(EM_MODULE_DRIVERS_LEDS_ID, new mLEDs());
  #endif
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
    addTasker(EM_MODULE_DRIVERS_FILESYSTEM_ID, new mFileSystem());
  #endif
  #ifdef USE_MODULE__DRIVERS_BUZZER_BASIC
    addTasker(EM_MODULE__DRIVERS_BUZZER_BASIC__ID, new mBuzzer());
  #endif
  #ifdef USE_MODULE_DRIVERS_BUZZER
    addTasker(EM_MODULE_DRIVERS_BUZZER_ID, new mBuzzer());
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
    addTasker(EM_MODULE_DRIVERS_RF433_RCSWITCH_ID, new mRCSwitch());
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
    addTasker(EM_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID, new mRCSwitch());
  #endif
  #ifdef USE_MODULE_DRIVERS_IRREMOTE
    addTasker(EM_MODULE_DRIVERS_IRREMOTE_ID, new mIRRemote());
  #endif
  #ifdef USE_MODULE_DRIVERS_FONA_CELLULAR
    addTasker(EM_MODULE_DRIVERS_FONA_CELLULAR_ID, new mFona_Cellular());
  #endif
  #ifdef USE_MODULE_DRIVERS__CELLULAR_SIM7000
    addTasker(EM_MODULE_DRIVERS__CELLULAR_SIM7000__ID, new mCellular_SIM7000());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_ARDUINO
    addTasker(EM_MODULE_DRIVERS__CAMERA_ARDUINO__ID, new mWebCamera());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_TASMOTA
    addTasker(EM_MODULE_DRIVERS__CAMERA_TASMOTA__ID, new mWebCamera());
  #endif
  #ifdef USE_MODULE_DRIVERS__CAMERA_MULTICLIENT
    addTasker(EM_MODULE_DRIVERS__CAMERA_MULTICLIENT__ID, new mWebCamera());
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_7000G
    addTasker(EM_MODULE_DRIVERS__MODEM_7000G__ID, new mSIM7000G());
  #endif
  #ifdef USE_MODULE_DRIVERS_MODEM_800L
    addTasker(EM_MODULE_DRIVERS__MODEM_800L__ID, new mSIM800L());
  #endif

  // Energy
  #ifdef USE_MODULE_ENERGY_INTERFACE
    addTasker(EM_MODULE_ENERGY_INTERFACE_ID, new mEnergyInterface());
  #endif
  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
    addTasker(EM_MODULE_ENERGY_PZEM004T_V3_ID, new mEnergyPZEM004T());
  #endif
  #ifdef USE_MODULE_ENERGY_ADE7953
    addTasker(EM_MODULE_ENERGY_ADE7953_ID, new mEnergyADE7953());
  #endif
  #ifdef USE_MODULE_ENERGY_INA219
    addTasker(EM_MODULE_ENERGY_INA219_ID, new mEnergyINA219());
  #endif

  // Lights
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    addTasker(EM_MODULE_LIGHTS_INTERFACE_ID, new mInterfaceLight());
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    addTasker(EM_MODULE_LIGHTS_ANIMATOR_ID, new mAnimatorLight());
  #endif

  // Sensors
  #ifdef USE_MODULE_SENSORS_INTERFACE
    addTasker(EM_MODULE_SENSORS_INTERFACE_ID, new mSensorsInterface());
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
    addTasker(EM_MODULE_SENSORS_BUTTONS_ID, new mButtons());
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    addTasker(EM_MODULE_SENSORS_SWITCHES_ID, new mSwitches());
  #endif
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
    addTasker(EM_MODULE_SENSORS_LDR_BASIC_ID, new mLDRBasic());
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    addTasker(EM_MODULE_SENSORS_ANALOG_ID, new mSensorsAnalog());
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    addTasker(EM_MODULE_SENSORS_DHT_ID, new mSensorsDHT());
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    addTasker(EM_MODULE_SENSORS_BME_ID, new mBME());
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    addTasker(EM_MODULE_SENSORS_ULTRASONIC_ID, new mUltraSonicSensor());
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    addTasker(EM_MODULE_SENSORS_DOOR_ID, new mDoorSensor());
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    addTasker(EM_MODULE_SENSORS_MOTION_ID, new mMotion());
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    addTasker(EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID, new X());
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    addTasker(EM_MODULE_SENSORS_PULSECOUNTER_ID, new X());
  #endif
  #ifdef USE_MODULE_SENSORS_BH1750
    addTasker(EM_MODULE_SENSORS_BH1750_ID, new mBH1750());
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
    addTasker(EM_MODULE_SENSORS_SR04_ID, new mSR04());
  #endif
  #ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
    addTasker(EM_MODULE_SENSORS_REMOTE_DEVICE_ID, new mRemoteDevice());
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
    addTasker(EM_MODULE_SENSORS_ADC_INTERNAL_ID, new mADCInternal());
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
    addTasker(EM_MODULE_SENSORS_ADC_INTERNAL_ID, new mADCInternal());
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL
    addTasker(EM_MODULE_SENSORS_ADC_I2S_INTERNAL_ID, new mADC_I2S_Sampler());
  #endif
  #ifdef USE_MODULE_SENSORS_LSM303D
    addTasker(EM_MODULE_SENSORS_LSM303D_ID, new mSensorsLSM303D());
  #endif
  #ifdef USE_MODULE_SENSORS_L3G
    addTasker(EM_MODULE_SENSORS_L3G_ID, new mSensorsL3G());
  #endif
  #ifdef USE_MODULE_SENSORS_MPU9250
    addTasker(EM_MODULE_SENSORS_MPU9250_ID, new mSensorsMPU9250());
  #endif
  #ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
    addTasker(EM_MODULE_SENSORS_ROTARY_ENCODER_ID, new mRotaryEncoder());
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023
    addTasker(EM_MODULE_SENSORS_ROTARY_ENCODER_ID, new mDB18x20_ESP32());
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP8266_2023
    addTasker(EM_MODULE_SENSORS__DS18X20__ID, new mDB18x20_ESP8266());
  #endif
  #ifdef USE_MODULE_SENSORS__DS18X20_ESP32_2023
    addTasker(EM_MODULE_SENSORS__DS18X20__ID, new mDB18x20_ESP32());
  #endif
  #ifdef USE_MODULE_SENSORS_GPS_SERIAL
    addTasker(EM_MODULE__SENSORS_GPS_SERIAL__ID, new mGPS_Serial());
  #endif
  #ifdef USE_MODULE_SENSORS_GPS_MODEM
    addTasker(EM_MODULE__SENSORS_GPS_MODEM__ID, new mGPS_Modem());
  #endif
  #ifdef USE_MODULE_SENSORS_BATTERY_MODEM
    addTasker(EM_MODULE__SENSORS_BATTERY_MODEM__ID, new mDB18x20_ESP32());
  #endif
  #ifdef USE_MODULE_SENSORS_SOLAR_LUNAR
  addTasker(EM_MODULE_SENSORS_SOLAR_LUNAR_ID, new mSolarLunar());
  #endif

  // Controllers
  #ifdef USE_MODULE_CONTROLLER_BLINDS
    addTasker(EM_MODULE_CONTROLLER_BLINDS_ID, new X());
  #endif
  #ifdef USE_MODULE_CONTROLLER_HVAC
    addTasker(EM_MODULE_CONTROLLER__HVAC__ID, new mHVAC());
  #endif
  #ifdef USE_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR
    addTasker(EM_MODULE_CONTROLLERS__SENSOR_COLOUR_BAR__ID, new mSensorColourBar());
  #endif
  #ifdef USE_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP
    addTasker(EM_MODULE_CONTROLLERS__RELAY_STATE_LEDSTRIP__ID, new mRelayStateLEDStrip());
  #endif
  #ifdef USE_MODULE_CONTROLLER_IRTRANSMITTER
    addTasker(EM_MODULE_CONTROLLER_IRTRANSMITTER_ID, new X());
  #endif
  #ifdef USE_MODULE_CONTROLLER_TANKVOLUME
    addTasker(EM_MODULE_CONTROLLER_TANKVOLUME_ID, new mTankVolume());
  #endif
  #ifdef USE_MODULE_CONTROLLER_EXERCISE_BIKE
    addTasker(EM_MODULE_CONTROLLER_EXERCISEBIKE_ID, new X());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
    addTasker(EM_MODULE_CONTROLLER_SONOFF_IFAN_ID, new mSonoffIFan());
  #endif
  #ifdef USE_MODULE_CONTROLLER_FAN
    addTasker(EM_MODULE_CONTROLLER_FAN_ID, new mFan());
  #endif
  #ifdef USE_MODULE_CONTROLLER_TREADMILL
    addTasker(EM_MODULE_CONTROLLER_TREADMILL_ID, new X());
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    addTasker(EM_MODULE_CONTROLLER_DOORBELL_ID, new mDoorBell());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
    addTasker(EM_MODULE_CONTROLLER_SDCARDLOGGER_ID, new mSDCardLogger());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    addTasker(EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID, new mSerialPositionalLogger());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
    addTasker(EM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID, new mSerialCalibrationMeasurmentLogger());
  #endif
  #ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
    addTasker(EM_MODULE_CONTROLLER_GPS_SD_LOGGER_ID, new mGPS_SD_Logger());
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
    addTasker(EM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_ID, new mSDLoggerIMURadiationPattern());
  #endif
  #ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
    addTasker(EM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_ID, new mBucketWaterLevel());
  #endif
  #ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
    addTasker(EM_MODULE_CONTROLLER_FURNACE_SENSOR_ID, new mFurnaceSensor());
  #endif
  #ifdef USE_MODULE_CONTROLLER_USERMOD_01
    addTasker(EM_MODULE_CONTROLLER_USERMOD_01_ID, new mUserMod_01());
  #endif
  #ifdef USE_MODULE_CONTROLLER__ENERGY_OLED
    addTasker(EM_MODULE_CONTROLLER__ENERGY_OLED__ID, new mEnergyOLED());
  #endif
  
  // Custom Controllers
  #ifdef USE_MODULE_CONTROLLER_RADIATORFAN
    addTasker(EM_MODULE_CONTROLLER_RADIATORFAN_ID, new mRadiatorFan());
  #endif
  #ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
    addTasker(EM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID, new mImmersionTankColour());
  #endif
  #ifdef USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS
    addTasker(EM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_ID, new mHeatingStripColour_Understairs());
  #endif
  #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
    addTasker(EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID, new mLouvoliteHub());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHT__ID, new mSideDoorLight());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__IMMERSION_PANEL__ID, new mImmersionPanel());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__PORTABLE_TEMPSENSOR_OLED__ID, new mTempSensorOLEDBath());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_TESTER
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_TESTER__ID, new mWLEDWebUI());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__WEBUI_WLED_DEVELOPER
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__WLED_WEBUI_DEVELOPER__ID, new mWLEDWebUI());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__3DPRINTER_ENCLOSURE__ID, new mPrinter3D());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__ENERGY_OLED__ID, new mEnergyOLED());
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER
    addTasker(EM_MODULE_CONTROLLER_CUSTOM__TREADMILL_LOGGER__ID, new mTreadmillLogger());
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
  return EM_MODULE_LENGTH_ID;
}


uint16_t mTaskerManager::GetClassSizeByID(uint8_t class_id)
{
  #ifdef USE_DEBUG_CLASS_SIZE
  return pModule[class_id]->GetClassSize();
  #else
  return 0;
  #endif
}


int16_t mTaskerManager::GetModuleIndexbyFriendlyName(const char* c)
{

  ALOG_WRN( PSTR("GetModuleIndexbyFriendlyName, should this be GetModule_UniqueID_byFriendlyName") );

  if(*c == '\0'){ return -1; }
  for(int ii=0;ii<GetClassCount();ii++){
    if(strcasecmp_P(c, pModule[ii]->GetModuleFriendlyName())==0){
      //AddLog(LOG_LEVEL_TEST, PSTR("MATCHED GetModuleIDbyFriendlyName \"%s\" => \"%s\" %d"),c,pModule[ii]->GetModuleFriendlyName(),ii);
      return ii;
    }    
  }
  return -1;
}


uint16_t mTaskerManager::GetModule_UniqueID_byFriendlyName(const char* c)
{
  if(*c=='\0'){ return -1; }
  for(int ii=0;ii<GetClassCount();ii++){
    if(strcasecmp_P(c, pModule[ii]->GetModuleFriendlyName())==0){
      //AddLog(LOG_LEVEL_TEST, PSTR("MATCHED GetModuleIDbyFriendlyName \"%s\" => \"%s\" %d"),c,pModule[ii]->GetModuleFriendlyName(),ii);
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


int16_t mTaskerManager::GetModuleUniqueIDbyFriendlyName(const char* c)
{
  int16_t index = GetModuleIndexbyFriendlyName(c);
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


PGM_P mTaskerManager::GetModuleFriendlyName(uint16_t id)
{
  if(ValidTaskID(id))
  {
    return pModule[id]->GetModuleFriendlyName();
  }  
  return PM_SEARCH_NOMATCH;
}


PGM_P mTaskerManager::GetModuleFriendlyName_WithUniqueID(uint16_t unique_id)
{
  uint8_t enum_id = GetEnumVectorIndexbyModuleUniqueID(unique_id);
  if(ValidTaskID(enum_id))
  {
    return pModule[enum_id]->GetModuleFriendlyName();
  }  
  return PM_SEARCH_NOMATCH;
}


PGM_P mTaskerManager::GetModuleName(uint8_t id)
{
  if(ValidTaskID(id))
  {
    return pModule[id]->GetModuleName();
  }  
  return PM_SEARCH_NOMATCH;
}
  


  
#ifdef ENABLE_DEBUG_FUNCTION_NAMES

// should be moved into task.h

// Switch case should be faster than getext progmem
// Use progmem WITHOUT buffer for speed improvements, should be read as expected progmem and handled that way
const char* mTaskerManager::GetTaskName(uint8_t task, char* buffer){

  switch(task){
    default:
                                                      return PM_SEARCH_NOMATCH;
    case FUNC_POINTER_INIT:                           return PM_FUNC_POINTER_INIT_CTR;
    case FUNC_TEMPLATE_MODULE_LOAD_FROM_PROGMEM:      return PM_FUNC_TEMPLATE_LOAD_CTR;
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
