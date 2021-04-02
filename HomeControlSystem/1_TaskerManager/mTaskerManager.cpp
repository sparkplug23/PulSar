#include "1_TaskerManager/mTaskerManager.h"

mTaskerManager* mTaskerManager::instance = nullptr;

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
  // Network
  #ifdef USE_MODULE_NETWORK_WIFI
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
    pModule[EM_MODULE_DISPLAYS_NEXTION_ID] = new X();
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    pModule[EM_MODULE_DISPLAYS_NEXTION_ID] = new X();
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
    pModule[EM_MODULE_DRIVERS_PWM_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433MHZ
    pModule[EM_MSAW_MODULE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    pModule[EM_MODULE_DRIVERS_SDCARDID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_GPS
    pModule[EM_MODULE_DRIVERS_GPS_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SERIAL_UART
    pModule[EM_MODULE_DRIVERS_SERIAL_UART_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    pModule[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    pModule[EM_MODULE_DRIVERS_CAMERA_OV2640_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
    pModule[EM_MODULE_DRIVERS_STATUS_LEDS_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
    pModule[EM_MODULE_DRIVERS_FILESYSTEM_ID] = new mFileSystem();
  #endif
  // Energy
  #ifdef USE_MODULE_ENERGY_INTERFACE
    pModule[EM_MODULE_ENERGY_INTERFACE_ID] = new X();
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
    pModule[EM_MODULE_LIGHTS_PWM_ID] = new X();
  #endif
  #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
    pModule[EM_MODULE_LIGHTS_WLED_EFFECTS_ID] = new X();
  #endif
  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    pModule[EM_MODULE_SENSORS_BUTTONS_ID] = new mButtons();
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    pModule[EM_MODULE_SENSORS_SWITCHES_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    pModule[EM_MODULE_SENSORS_ANALOG_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    pModule[EM_MODULE_SENSORS_PZEM004T_MODBUS_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    pModule[EM_MODULE_SENSORS_DHT_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    pModule[EM_MODULE_SENSORS_BME_ID] = new mSensorsBME();
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    pModule[EM_MODULE_SENSORS_DB18S20_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_INA219
    pModule[EM_MODULE_SENSORS_INA219_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    pModule[EM_MODULE_SENSORS_ULTRASONIC_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    pModule[EM_MODULE_SENSORS_DOOR_ID] = new mDoorSensor();
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    pModule[EM_MODULE_SENSORS_MOTION_ID] = new mMotionSensor();
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    pModule[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    pModule[EM_MODULE_SENSORS_PULSECOUNTER_ID] = new X();
  #endif
  // Controllers
  #ifdef USE_MODULE_CUSTOM_BLINDS
    pModule[EM_MODULE_CUSTOM_BLINDS_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_HEATING
    pModule[EM_MODULE_CUSTOM_HEATING_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    pModule[EM_MODULE_CUSTOM_RADIATORFAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_IRTRANSMITTER
    pModule[EM_MODULE_CUSTOM_IRTRANSMITTER_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    pModule[EM_MODULE_CUSTOM_OILFURNACE_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    pModule[EM_MODULE_CUSTOM_EXERCISEBIKE_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    pModule[EM_MODULE_CUSTOM_SONOFF_IFAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_FAN
    pModule[EM_MODULE_CUSTOM_FAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_TREADMILL
    pModule[EM_MODULE_CUSTOM_TREADMILL_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_SENSORCOLOURS
    pModule[EM_MODULE_CUSTOM_SENSORCOLOURS_ID] = new X();
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    pModule[EM_MODULE_CONTROLLER_DOORBELL_ID] = new X();
  #endif

}

/**
 * Default is Tasker_Interface(uint8_t function) with target_tasker = 0. If 0, all classes are called. 
 If !0, a specific tasker will be called and this function will exit after completion
 * */
int8_t mTaskerManager::Tasker_Interface(uint8_t function, uint8_t target_tasker, bool flags_is_executing_rule){

  int8_t result = 0;

  if(target_tasker){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker %d %s"),target_tasker,GetModuleFriendlyName(target_tasker));
    #endif// ENABLE_LOG_LEVEL_INFO
  }

  #ifdef USE_MODULE_CORE_RULES
  if(flags_is_executing_rule != true){
    pCONT_rules->Tasker_Rules_Interface(function);
  }
  #endif
  
  for(uint8_t i=0;i<GetClassCount();i++){     // If target_tasker != 0, then use it, else, use indexed array

    switch_index = target_tasker;//target_tasker ? target_tasker : module_settings.list[i];
    #ifdef ENABLE_ADVANCED_DEBUGGING
      AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%02d %s\t%S"),
        switch_index, 
        GetTaskName(function, buffer_taskname),
        GetModuleFriendlyName(switch_index));
    #endif
    
    #if defined(DEBUG_EXECUTION_TIME) || defined(ENABLE_ADVANCED_DEBUGGING)  || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
    uint32_t start_millis = millis();
    #endif

    pModule[i]->Tasker(function);

    #if defined(DEBUG_EXECUTION_TIME)  || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
    // Remember start millis
    uint32_t end_millis = millis();
    // Get this execution time 
    uint32_t this_millis = end_millis - start_millis;
    #if defined(DEBUG_EXECUTION_TIME)
    // Get average
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
      AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " module completed \t%d ms %s"),millis()-start_millis, GetTaskName(function, buffer_taskname));
    #endif
    #if defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
      if(this_millis > 500){
        AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%d ms %s %S"),millis()-start_millis, GetTaskName(function, buffer_taskname), GetModuleFriendlyName(switch_index));
      }
    #endif

    if(target_tasker!=0){
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker EXITING EARLY"));
      #endif// ENABLE_LOG_LEVEL_INFO
      break; //only run for loop for the class set. if 0, rull all
    }
    // Special flag that can be set to end interface ie event handled, no need to check others
    if(fExitTaskerWithCompletion){
      fExitTaskerWithCompletion=false;
      #ifdef ENABLE_LOG_LEVEL_INFO
        AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
      #endif// ENABLE_LOG_LEVEL_INFO
      break; //only run for loop for the class set. if 0, rull all
    }
  
  } //end for

  DEBUG_LINE;
  if(!pCONT_set->flag_boot_complete){
    char buffer_taskname[50];
    if(function != last_function){
      uint8_t boot_percentage = map(function,0,FUNC_ON_BOOT_COMPLETE,0,100);
      //5% = 1 bar, 20 bars total [B                   ]
      //if(pCONT_set->Settings.seriallog_level >= LOG_LEVEL_INFO){
      #ifndef DISABLE_SERIAL_LOGGING
      Serial.printf("[");
      for(uint8_t percent=0;percent<100;percent+=5){  
        if(percent<boot_percentage){ Serial.print((char)219); }else{ Serial.printf(" "); }
      }      
      #ifdef ENABLE_DEBUG_FUNCTION_NAMES
      Serial.printf("] %03d %s\n\r",boot_percentage,GetTaskName(function, buffer_taskname));
      #else
      Serial.printf("] %03d\n\r",boot_percentage);
      #endif // ENABLE_DEBUG_FUNCTION_NAMES
      #endif
      //}
      last_function = function;
    }
    if(function == FUNC_ON_BOOT_COMPLETE){ pCONT_set->flag_boot_complete = true; }
  }//flag_boot_complete
  
  DEBUG_LINE;
  #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " FINISHED"));
  #endif

  return result;

}


uint16_t mTaskerManager::GetClassCount(){
  return EM_MODULE_LENGTH_ID;
}


uint16_t mTaskerManager::GetClassSizeByID(uint8_t class_id){
  #ifdef USE_DEBUG_CLASS_SIZE
  return pModule[class_id]->GetClassSize(); // can I maybe get the class size via its pointer, thus, removing the need for function callbacks
  //return sizeof(&pModule[class_id]);// size of what it points to?
  #else
  return 0;
  #endif
}


int16_t mTaskerManager::GetModuleIndexbyFriendlyName(const char* c){
  if(c=='\0'){ return -1; }
  for(int ii=0;ii<GetClassCount();ii++){
    if(strcasecmp_P(c, pModule[ii]->GetModuleFriendlyName())==0){
      //AddLog_P(LOG_LEVEL_TEST, PSTR("MATCHED GetModuleIDbyFriendlyName \"%s\" => \"%s\" %d"),c,pModule[ii]->GetModuleFriendlyName(),ii);
      return ii;
    }    
  }
  return -1;
}


int16_t mTaskerManager::GetModuleUniqueIDbyFriendlyName(const char* c)
{
  int16_t index = GetModuleIndexbyFriendlyName(c);
  if(index != -1)
  {
    return GetModuleUniqueIDbyVectorIndex(index);
  }
}


uint16_t mTaskerManager::GetModuleUniqueIDbyVectorIndex(uint8_t id)
{
  return pModule[id]->GetModuleUniqueID();
}


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


PGM_P mTaskerManager::GetModuleName(uint8_t id)
{
  if(ValidTaskID(id))
  {
    return pModule[id]->GetModuleName();
  }  
  return PM_SEARCH_NOMATCH;
}
  