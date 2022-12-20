#include "1_TaskerManager/mTaskerManager.h"

mTaskerManager* mTaskerManager::instance = nullptr;

int8_t mTaskerManager::Tasker_Interface(uint16_t function, uint16_t target_tasker)
{

  int8_t result = 0;

  if(target_tasker){
    ALOG_INF(PSTR(D_LOG_CLASSLIST "target_tasker %d %s"),target_tasker,GetModuleFriendlyName_WithUniqueID(target_tasker));
  }

  JsonParserObject obj = 0;
  
  switch(function)
  {
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:    
      // if(obj && GetModuleIDbyFriendlyName(topic)==i)
      // else{break;}
      // If match, switch target tasker to be the directed command
    case FUNC_JSON_COMMAND_ID:
    { 
      
      JsonParser parser(data_buffer.payload.ctr);
      
      // Single parsing, for now, make copy as we are modifying the original with tokens, otherwise, no new copy when phased over
      obj = parser.getRootObject();   
      if (!obj)
      {
        #ifdef ENABLE_LOG_LEVEL_COMMANDS
        AddLog(LOG_LEVEL_ERROR, PSTR(D_JSON_DESERIALIZATION_ERROR));
        #endif //ENABLE_LOG_LEVEL_COMMANDS
        break;
      }

      for(uint8_t i=0;i<GetClassCount();i++)
      { 
        // switch_index = target_tasker ? target_tasker : i;

        
    if(target_tasker){
      switch_index = GetEnumNumber_UsingModuleUniqueID(target_tasker); // passed value module is in unique_module_id format
    }else{
      switch_index = i; // Normally index is synonomous with enum list
    }

          // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CLASSLIST "switch_index %d"),switch_index);  


        pModule[switch_index]->Tasker(function, obj);
        if(target_tasker)
        {
          // #ifdef ENABLE_LOG_LEVEL_DEBUG_MORE
          AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CLASSLIST "EXECUTED ONCE %d %s"),target_tasker,GetModuleFriendlyName_WithUniqueID(target_tasker));  
          // #endif // ENABLE_LOG_LEVEL_COMMANDS          
          break; 
        }
      }
// DEBUG_LINE_HERE;
      return 0; // needs to return via "Tasker"
    } 
    break;
  } //END switch

  #ifdef  ENABLE_DEBUG_FUNCTION_NAMES
    char buffer_taskname[50];
  #endif
  // #ifdef DEBUG_PIN3_GPIO
  //   for(int i=0;i<1;i++){
  //   DEBUG_PIN3_SET(0); //green
  //   DEBUG_PIN3_SET(1);
  //   }
  // #endif // DEBUG_PIN3_GPIO

  for(uint8_t i=0;i<GetClassCount();i++){     // If target_tasker != 0, then use it, else, use indexed array

    if(target_tasker){
      switch_index = GetEnumNumber_UsingModuleUniqueID(target_tasker); // passed value module is in unique_module_id format
    }else{
      switch_index = i; // Normally index is synonomous with enum list
    }



    // switch_index = target_tasker ? target_tasker : i;
    // #ifdef ENABLE_ADVANCED_DEBUGGING
    // Serial.printf("switch_index=%d\n\r",switch_index);
    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST "TI_%d\t %02d %S\t%S"), millis(), switch_index, pCONT_set->GetTaskName(function, buffer_taskname), GetModuleFriendlyName(switch_index));
    #endif
    #ifdef ENABLE_ADVANCED_DEBUGGING
    #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      ALOG_INF(PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " module started \t%d ms %s"),millis(), pCONT_set->GetTaskName(function, buffer_taskname));
      #endif
    #endif
    
    #if defined(DEBUG_EXECUTION_TIME) || defined(ENABLE_ADVANCED_DEBUGGING)  || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
    uint32_t start_millis = millis();
    #endif
    
  //   if(function == FUNC_EVENT_INPUT_STATE_CHANGED_ID)
  //   {
  // DEBUG_LINE_HERE; ALOG_INF(PSTR("event_triggered.module_id = i%d f%d\t %d"),switch_index, function, pCONT_rules->event_triggered.module_id); 
  //   }

  // DEBUG_LINE_HERE;

  /**
   * @brief Convert "unique_module_id" to "enum_index"
   * 
   */

// DEBUG_LINE_HERE;

// DEBUG_PIN6_SET(LOW);
    switch(function)
    {
      // case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:  
      case FUNC_JSON_COMMAND_ID: 
        // AddLog(LOG_LEVEL_TEST, PSTR("FUNC_JSON_COMMAND_ID=%d"),obj["test2"].getInt());   
      //   if(obj)
      //   {
      //     // pModule[switch_index]->parse_JSONCommand(obj); // This should only happen if the function is enabled internally
      //     pModule[switch_index]->Tasker(function,obj);
      //   }
      break;
      default:

      // DEBUG_LINE_HERE;
      
          // AddLog(LOG_LEVEL_DEBUG,PSTR(D_LOG_CLASSLIST "switch_index %d"),switch_index);  
        // switch(switch_index)
        // {
        //   case EM_MODULE_CORE_HARDWAREPINS_ID:
        //   case EM_MODULE_CORE_SETTINGS_ID:
        //   case EM_MODULE_CORE_SUPPORT_ID:
        //   case EM_MODULE_CORE_LOGGING_ID:
        //   case EM_MODULE_CORE_TELEMETRY_ID:
        //   case EM_MODULE_CORE_TIME_ID:
        //   case EM_MODULE_CORE_RULES_ID:
        //   case EM_MODULE_DRIVERS_CAMERA_OV2640_ID:
        pModule[switch_index]->Tasker(function, obj);
        //   break;
        //   default:
        //   //nothing
        //   break;
        // }

      break;
    }
// DEBUG_PIN6_SET(HIGH);

    #if defined(DEBUG_EXECUTION_TIME)  || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
    uint32_t end_millis = millis(); // Remember start millis
    uint32_t this_millis = end_millis - start_millis; // Get this execution time 
    #if defined(DEBUG_EXECUTION_TIME) // Get average
    //if(fModule_present){ //only update tasks that run .. IMPROVE this later with flags (manually) or via returns of tasks
      module_settings.execution_time_average_ms[i] += this_millis;
      module_settings.execution_time_average_ms[i] /= 2; //gets average
     // Get max
      if(this_millis > module_settings.execution_time_max_ms[i]){
        module_settings.execution_time_max_ms[i] = this_millis; // remember max
      }
    //}
    #endif // DEBUG_EXECUTION_TIME
    #endif
    
    #ifdef ENABLE_ADVANCED_DEBUGGING
      AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " module completed \t%d ms %s"),millis()-start_millis, pCONT_set->GetTaskName(function, buffer_taskname));
    #endif
    #if defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
      if(this_millis > 500){
        AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%d ms %s %S"),millis()-start_millis, pCONT_set->GetTaskName(function, buffer_taskname), GetModuleFriendlyName(switch_index));
      }
    #endif

    if(target_tasker!=0){
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker EXITING EARLY"));
      #endif// ENABLE_LOG_LEVEL_INFO
      break; //only run for loop for the class set. if 0, rull all
    }
    // Special flag that can be set to end interface ie event handled, no need to check others
    if(fExitTaskerWithCompletion){
      fExitTaskerWithCompletion=false;
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
      #endif// ENABLE_LOG_LEVEL_INFO
      break; //only run for loop for the class set. if 0, rull all
    }
  
  } //end for

  #ifdef ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL
  if(!pCONT_set->flag_boot_complete){
    char buffer_taskname[50];
    if(function != last_function){
      uint8_t boot_percentage = map(function,0,FUNC_ON_BOOT_COMPLETE,0,100);
      //5% = 1 bar, 20 bars total [B                   ]
      //if(pCONT_set->Settings.seriallog_level >= LOG_LEVEL_INFO){
      #ifndef DISABLE_SERIAL_LOGGING
      DEBUG_PRINTF("[");
      for(uint8_t percent=0;percent<100;percent+=5){  
        if(percent<boot_percentage){ Serial.print((char)219); }else{ DEBUG_PRINTF(" "); }
      }      
      #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      DEBUG_PRINTF("] %03d %s\n\r",boot_percentage,pCONT_set->GetTaskName(function, buffer_taskname));
      #else
      DEBUG_PRINTF("] %03d\n\r",boot_percentage);
      #endif // ENABLE_DEBUG_FUNCTION_NAMES
      #endif
      //}
      last_function = function;
    }
  }//flag_boot_complete
  #endif // ENABLE_DEVFEATURE_SHOW_BOOT_PROGRESS_ON_SERIAL

  if(function == FUNC_ON_BOOT_COMPLETE){ pCONT_set->flag_boot_complete = true; }
  
  DEBUG_LINE;
  #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " FINISHED"));
  #endif

  return result;

}


uint8_t mTaskerManager::Instance_Init(){
  
  // Core
  #ifdef USE_MODULE_CORE_HARDWAREPINS
  pModule[EM_MODULE_CORE_HARDWAREPINS_ID] = new mHardwarePins();
  #endif 
  #ifdef USE_MODULE_CORE_SETTINGS
  pModule[EM_MODULE_CORE_SETTINGS_ID] = new mSettings();
  #endif 
  #ifdef USE_MODULE_CORE_SUPPORT
  pModule[EM_MODULE_CORE_SUPPORT_ID] = new mSupport();
  #endif 
  #ifdef USE_MODULE_CORE_LOGGING
  pModule[EM_MODULE_CORE_LOGGING_ID] = new mLogging();
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
  pModule[EM_MODULE_CORE_TELEMETRY_ID] = new mTelemetry();
  #endif 
  #ifdef USE_MODULE_CORE_TIME
  pModule[EM_MODULE_CORE_TIME_ID] = new mTime();
  #endif 
  #ifdef USE_MODULE_CORE_RULES
  pModule[EM_MODULE_CORE_RULES_ID] = new mRuleEngine();
  #endif
  #ifdef USE_MODULE_CORE_UPDATES
  pModule[EM_MODULE_CORE_UPDATES_ID] = new mUpdates();
  #endif
  #ifdef USE_MODULE_CORE_DEVELOPMENT_DEBUGGING
  pModule[EM_MODULE_CORE_DEVELOPMENT_DEBUGGING_ID] = new mDevelopmentDebugging();
  #endif
  // Subsystems
  #ifdef USE_MODULE_SUBSYSTEM_SOLAR_LUNAR
  pModule[EM_MODULE_SUBSYSTEM_SOLAR_LUNAR_ID] = new mSolarLunar();
  #endif
  // Network
  #if defined(USE_MODULE_NETWORK_WIFI) || defined(USE_MODULE_NETWORK_WIFI_V2)
  pModule[EM_MODULE_NETWORK_WIFI_ID] = new mWiFi();
  #endif 
  #ifdef USE_MODULE_NETWORK_MQTT
  pModule[EM_MODULE_NETWORK_MQTT_ID] = new mMQTT();
  #endif 
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  pModule[EM_MODULE_NETWORK_WEBSERVER_ID] = new mWebServer();
  #endif
  // Displays
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
    pModule[EM_MODULE_DISPLAYS_INTERFACE_ID] = new mDisplaysInterface();
  #endif
  #if defined(USE_MODULE_DISPLAYS_NEXTION) || defined(USE_MODULE_DISPLAYS_NEXTION_V2)
    pModule[EM_MODULE_DISPLAYS_NEXTION_ID] = new mNextionPanel();
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
    pModule[EM_MODULE_DISPLAYS_OLED_SSD1306_ID] = new mOLED_SSD1306();
  #endif
  #ifdef USE_MODULE_DISPLAYS_OLED_SH1106
    pModule[EM_MODULE_DISPLAYS_OLED_SH1106_ID] = new mOLED_SH1106();
  #endif
  // Drivers (Range 40-129)
  #ifdef USE_MODULE_DRIVERS_INTERFACE
    // pModule[EM_MODULE_DRIVERS_HBRIDGE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    pModule[EM_MODULE_DRIVERS_HBRIDGE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    pModule[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    pModule[EM_MODULE_DRIVERS_RELAY_ID] = new mRelays();
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM
    pModule[EM_MODULE_DRIVERS_PWM_ID] = new mPWM();
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    pModule[EM_MODULE_DRIVERS_SDCARD_ID] = new mSDCard();
  #endif
  #ifdef USE_MODULE_DRIVERS_GPS
    pModule[EM_MODULE_DRIVERS_GPS_ID] = new mGPS();
  #endif
  #ifdef USE_MODULE_DRIVERS_SERIAL_UART
    pModule[EM_MODULE_DRIVERS_SERIAL_UART_ID] = new mSerialUART();
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    pModule[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID] = new mShellyDimmer();
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    pModule[EM_MODULE_DRIVERS_CAMERA_OV2640_ID] = new mCameraOV2640();
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640_2
    pModule[EM_MODULE_DRIVERS_CAMERA_OV2640_ID] = new mCameraOV2640();
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_WEBCAM_V4
    pModule[EM_MODULE_DRIVERS_CAMERA_WEBCAM_V4_ID] = new mWebCamera();
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
    pModule[EM_MODULE_DRIVERS_LEDS_ID] = new mLEDs();
  #endif
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
    pModule[EM_MODULE_DRIVERS_FILESYSTEM_ID] = new mFileSystem();
  #endif
  #ifdef USE_MODULE_DRIVERS_BUZZER
    pModule[EM_MODULE_DRIVERS_BUZZER_ID] = new mBuzzer();
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH
    pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_ID] = new mRCSwitch();
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED
    pModule[EM_MODULE_DRIVERS_RF433_RCSWITCH_EXTENDED_ID] = new mRCSwitch();
  #endif
  #ifdef USE_MODULE_DRIVERS_IRREMOTE
    pModule[EM_MODULE_DRIVERS_IRREMOTE_ID] = new mIRRemote();
  #endif
  #ifdef USE_MODULE_DRIVERS_FONA_CELLULAR
    pModule[EM_MODULE_DRIVERS_FONA_CELLULAR_ID] = new mFona_Cellular();
  #endif
  #ifdef USE_MODULE_DRIVERS__CELLULAR_SIM7000
    pModule[EM_MODULE_DRIVERS__CELLULAR_SIM7000__ID] = new mCellular_SIM7000();
  #endif
  // Energy
  #ifdef USE_MODULE_ENERGY_INTERFACE
    pModule[EM_MODULE_ENERGY_INTERFACE_ID] = new mEnergyInterface();
  #endif
  #ifdef USE_MODULE_ENERGY_PZEM004T_V3
    pModule[EM_MODULE_ENERGY_PZEM004T_V3_ID] = new mEnergyPZEM004T();
  #endif
  #ifdef USE_MODULE_ENERGY_ADE7953
    pModule[EM_MODULE_ENERGY_ADE7953_ID] = new mEnergyADE7953();
  #endif
  #ifdef USE_MODULE_ENERGY_INA219
    pModule[EM_MODULE_ENERGY_INA219_ID] = new X();
  #endif
  // Lights
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    pModule[EM_MODULE_LIGHTS_INTERFACE_ID] = new mInterfaceLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    pModule[EM_MODULE_LIGHTS_ANIMATOR_ID] = new mAnimatorLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    pModule[EM_MODULE_LIGHTS_ADDRESSABLE_ID] = new mAddressableLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    pModule[EM_MODULE_LIGHTS_PWM_ID] = new mPWMLight();
  #endif
  // #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS_FOR_CONVERSION
  //   pModule[EM_MODULE_LIGHTS_WLED_EFFECTS_ID] = new mWLEDEffects();
  // #endif
  // Sensors
  #ifdef USE_MODULE_SENSORS_INTERFACE
    pModule[EM_MODULE_SENSORS_INTERFACE_ID] = new mSensorsInterface();
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
    pModule[EM_MODULE_SENSORS_BUTTONS_ID] = new mButtons();
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    pModule[EM_MODULE_SENSORS_SWITCHES_ID] = new mSwitches();
  #endif
  #ifdef USE_MODULE_SENSORS_LDR_BASIC
    pModule[EM_MODULE_SENSORS_LDR_BASIC_ID] = new mLDRBasic();
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    pModule[EM_MODULE_SENSORS_ANALOG_ID] = new mSensorsAnalog();
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    pModule[EM_MODULE_SENSORS_DHT_ID] = new mSensorsDHT();
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    pModule[EM_MODULE_SENSORS_BME_ID] = new mBME();
  #endif
  #if defined(USE_MODULE_SENSORS_DS18X) || defined(USE_MODULE_SENSORS_DS18X_V2) || defined(USE_MODULE_SENSORS_DS18X_V3) || defined(USE_MODULE_SENSORS_DS18X_V4)
    pModule[EM_MODULE_SENSORS_DB18S20_ID] = new mDS18X();
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    pModule[EM_MODULE_SENSORS_ULTRASONIC_ID] = new mUltraSonicSensor();
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    pModule[EM_MODULE_SENSORS_DOOR_ID] = new mDoorSensor();
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    pModule[EM_MODULE_SENSORS_MOTION_ID] = new mMotion();
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    pModule[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    pModule[EM_MODULE_SENSORS_PULSECOUNTER_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_BH1750
    pModule[EM_MODULE_SENSORS_BH1750_ID] = new mBH1750();
  #endif
  #ifdef USE_MODULE_SENSORS_SR04
    pModule[EM_MODULE_SENSORS_SR04_ID] = new mSR04();
  #endif
  #ifdef USE_MODULE_SENSORS_REMOTE_DEVICE
    pModule[EM_MODULE_SENSORS_REMOTE_DEVICE_ID] = new mRemoteDevice();
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP8266
    pModule[EM_MODULE_SENSORS_ADC_INTERNAL_ID] = new mADCInternal();
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_INTERNAL_ESP32
    pModule[EM_MODULE_SENSORS_ADC_INTERNAL_ID] = new mADCInternal();
  #endif
  #ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL
    pModule[EM_MODULE_SENSORS_ADC_I2S_INTERNAL_ID] = new mADC_I2S_Sampler();
  #endif
  #ifdef USE_MODULE_SENSORS_LSM303D
    pModule[EM_MODULE_SENSORS_LSM303D_ID] = new mSensorsLSM303D();
  #endif
  #ifdef USE_MODULE_SENSORS_L3G
    pModule[EM_MODULE_SENSORS_L3G_ID] = new mSensorsL3G();
  #endif
  #ifdef USE_MODULE_SENSORS_MPU9250
    pModule[EM_MODULE_SENSORS_MPU9250_ID] = new mSensorsMPU9250();
  #endif
  #ifdef USE_MODULE_SENSORS_ROTARY_ENCODER
    pModule[EM_MODULE_SENSORS_ROTARY_ENCODER_ID] = new mRotaryEncoder();
  #endif
  // Controllers
  #ifdef USE_MODULE_CONTROLLER_BLINDS
    pModule[EM_MODULE_CONTROLLER_BLINDS_ID] = new X();
  #endif
  #ifdef USE_MODULE_CONTROLLER_HVAC
    pModule[EM_MODULE_CONTROLLER_HVAC_ID] = new mHVAC();
  #endif
  #ifdef USE_MODULE_CONTROLLER_IRTRANSMITTER
    pModule[EM_MODULE_CONTROLLER_IRTRANSMITTER_ID] = new X();
  #endif
  #ifdef USE_MODULE_CONTROLLER_TANKVOLUME
    pModule[EM_MODULE_CONTROLLER_TANKVOLUME_ID] = new mTankVolume();
  #endif
  #ifdef USE_MODULE_CONTROLLER_EXERCISE_BIKE
    pModule[EM_MODULE_CONTROLLER_EXERCISEBIKE_ID] = new X();
  #endif
  #ifdef USE_MODULE_CONTROLLER_SONOFF_IFAN
    pModule[EM_MODULE_CONTROLLER_SONOFF_IFAN_ID] = new mSonoffIFan();
  #endif
  #ifdef USE_MODULE_CONTROLLER_FAN
    pModule[EM_MODULE_CONTROLLER_FAN_ID] = new mFan();
  #endif
  #ifdef USE_MODULE_CONTROLLER_TREADMILL
    pModule[EM_MODULE_CONTROLLER_TREADMILL_ID] = new X();
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME //phasing out
    pModule[EM_MODULE_CONTROLLER_DOORBELL_ID] = new mDoorBell();
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDCARDLOGGER
    pModule[EM_MODULE_CONTROLLER_SDCARDLOGGER_ID] = new mSDCardLogger();
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER
    pModule[EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID] = new mSerialPositionalLogger();
  #endif
  #ifdef USE_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER
    pModule[EM_MODULE_CONTROLLER_SERIAL_CALIBRATION_PIC32_LOGGER_ID] = new mSerialCalibrationMeasurmentLogger();
  #endif
  #ifdef USE_MODULE_CONTROLLER_GPS_SD_LOGGER
    pModule[EM_MODULE_CONTROLLER_GPS_SD_LOGGER_ID] = new mGPS_SD_Logger();
  #endif
  #ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN
    pModule[EM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_ID] = new mSDLoggerIMURadiationPattern();
  #endif
  #ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL
    pModule[EM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_ID] = new mBucketWaterLevel();
  #endif
  #ifdef USE_MODULE_CONTROLLER_FURNACE_SENSOR
    pModule[EM_MODULE_CONTROLLER_FURNACE_SENSOR_ID] = new mFurnaceSensor();
  #endif
  #ifdef USE_MODULE_CONTROLLER_USERMOD_01
    pModule[EM_MODULE_CONTROLLER_USERMOD_01_ID] = new mUserMod_01();
  #endif
  
// 10 Controller (Unique to one use case)
  #ifdef USE_MODULE_CONTROLLER_RADIATORFAN
    pModule[EM_MODULE_CONTROLLER_RADIATORFAN_ID] = new mRadiatorFan();
  #endif
  #ifdef USE_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR
    pModule[EM_MODULE_CONTROLLER_IMMERSION_TANK_COLOUR_ID] = new mImmersionTankColour();
  #endif
  #ifdef USE_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS
    pModule[EM_MODULE_CONTROLLER_HEATING_STRIP_COLOUR_UNDERSTAIRS_ID] = new mHeatingStripColour_Understairs();
  #endif
  #if defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB) || defined(USE_MODULE_CONTROLLER__LOUVOLITE_HUB_V2)
    pModule[EM_MODULE_CONTROLLER__LOUVOLITE_HUB__ID] = new mLouvoliteHub();
  #endif
  #ifdef USE_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHTS
    pModule[EM_MODULE_CONTROLLER_CUSTOM__SIDEDOOR_LIGHT__ID] = new mSideDoorLight();
  #endif
};


uint16_t mTaskerManager::GetClassCount()
{
  return EM_MODULE_LENGTH_ID;
}


uint16_t mTaskerManager::GetClassSizeByID(uint8_t class_id)
{
  #ifdef USE_DEBUG_CLASS_SIZE
  return pModule[class_id]->GetClassSize(); // can I maybe get the class size via its pointer, thus, removing the need for function callbacks
  //return sizeof(&pModule[class_id]);// size of what it points to?
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


/**
 * @brief Method for going between EM_MODULE_ID (ie small int) and UniqueID (ie 1000's)
 * 
 * @return uint16_t 
 */
// uint16_t mTaskerManager::GetModuleUniqueID_UsingEmNumberID()
// {

//   for(int ii=0;ii<GetClassCount();ii++)
//   {

//     if()



//   }

// }

uint16_t mTaskerManager::GetEnumNumber_UsingModuleUniqueID(uint16_t unique_id)
// ModuleUniqueID_UsingEmNumberID()
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
  return pModule[GetEnumVectorIndexbyModuleUniqueID(id)];
}

/**
 * @brief Must check for validity, if not, returns nullptr
 * 
 * @param id 
 * @return mTaskerInterface* 
 */
// mTaskerInterface* mTaskerManager::GetModuleObjectbyUniqueID_Protected(uint16_t id)
// {

// safe way by returning a default class

//   return pModule[GetEnumVectorIndexbyModuleUniqueID(id)];
// }


bool mTaskerManager::ValidTaskID(uint8_t id)
{
  return id <= GetClassCount() ? true : false;
}

PGM_P mTaskerManager::GetModuleFriendlyName(uint8_t id)
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
  