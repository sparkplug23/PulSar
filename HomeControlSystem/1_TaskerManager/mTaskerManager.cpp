#include "1_TaskerManager/mTaskerManager.h"


mTaskerManager* mTaskerManager::instance = nullptr;


mTaskerManager* mTaskerManager::GetInstance(){
  if (instance == nullptr){
    instance = new mTaskerManager();
  }
  return instance;
}


uint8_t mTaskerManager::Instance_Init(){
  
  // Core
  #ifdef USE_MODULE_CORE_HARDWAREPINS
  pInterface[EM_MODULE_CORE_HARDWAREPINS_ID] = new mHardwarePins();
  #endif 
  #ifdef USE_MODULE_CORE_SETTINGS
  pInterface[EM_MODULE_CORE_SETTINGS_ID] = new mSettings();
  #endif 
  #ifdef USE_MODULE_CORE_SUPPORT
  pInterface[EM_MODULE_CORE_SUPPORT_ID] = new mSupport();
  #endif 
  #ifdef USE_MODULE_CORE_LOGGING
  pInterface[EM_MODULE_CORE_LOGGING_ID] = new mLogging();
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
  pInterface[EM_MODULE_CORE_TELEMETRY_ID] = new mTelemetry();
  #endif 
  #ifdef USE_MODULE_CORE_TIME
  pInterface[EM_MODULE_CORE_TIME_ID] = new mTime();
  #endif 
  #ifdef USE_MODULE_CORE_RULES
  pInterface[EM_MODULE_CORE_RULES_ID] = new mRuleEngine();
  #endif
  #ifdef USE_MODULE_CORE_UPDATES
  pInterface[EM_MODULE_CORE_UPDATES_ID] = new mUpdates();
  #endif
  // Network
  #ifdef USE_MODULE_NETWORK_WIFI
  pInterface[EM_MODULE_NETWORK_WIFI_ID] = new mWiFi();
  #endif 
  #ifdef USE_MODULE_NETWORK_MQTT
  pInterface[EM_MODULE_NETWORK_MQTT_ID] = new mMQTT();
  #endif 
  #ifdef USE_MODULE_NETWORK_WEBSERVER
  pInterface[EM_MODULE_NETWORK_WEBSERVER_ID] = new mWebserver();
  #endif
  // Displays
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
    pInterface[EM_MODULE_DISPLAYS_NEXTION_ID] = new X();
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    pInterface[EM_MODULE_DISPLAYS_NEXTION_ID] = new X();
  #endif
  // Drivers (Range 40-129)
  #ifdef USE_MODULE_DRIVERS_INTERFACE
    // pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    pInterface[EM_MODULE_DRIVERS_HBRIDGE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    pInterface[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    pInterface[EM_MODULE_DRIVERS_RELAY_ID] = new mRelays();
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM
    pInterface[EM_MODULE_DRIVERS_PWM_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433MHZ
    pInterface[EM_MSAW_MODULE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    pInterface[EM_MODULE_DRIVERS_SDCARDID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_GPS
    pInterface[EM_MODULE_DRIVERS_GPS_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SERIAL_UART
    pInterface[EM_MODULE_DRIVERS_SERIAL_UART_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    pInterface[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    pInterface[EM_MODULE_DRIVERS_CAMERA_OV2640_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
    pInterface[EM_MODULE_DRIVERS_STATUS_LEDS_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_FILESYSTEM
    pInterface[EM_MODULE_DRIVERS_FILESYSTEM_ID] = new mFileSystem();
  #endif
  // Energy
  #ifdef USE_MODULE_ENERGY_INTERFACE
    pInterface[EM_MODULE_ENERGY_INTERFACE_ID] = new X();
  #endif
  // Lights
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    pInterface[EM_MODULE_LIGHTS_INTERFACE_ID] = new mInterfaceLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    pInterface[EM_MODULE_LIGHTS_ANIMATOR_ID] = new mAnimatorLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    pInterface[EM_MODULE_LIGHTS_ADDRESSABLE_ID] = new mAddressableLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    pInterface[EM_MODULE_LIGHTS_PWM_ID] = new X();
  #endif
  #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
    pInterface[EM_MODULE_LIGHTS_WLED_EFFECTS_ID] = new X();
  #endif
  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    pInterface[EM_MODULE_SENSORS_BUTTONS_ID] = new mButtons();
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    pInterface[EM_MODULE_SENSORS_SWITCHES_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    pInterface[EM_MODULE_SENSORS_ANALOG_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    pInterface[EM_MODULE_SENSORS_PZEM004T_MODBUS_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    pInterface[EM_MODULE_SENSORS_DHT_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    pInterface[EM_MODULE_SENSORS_BME_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    pInterface[EM_MODULE_SENSORS_DB18S20_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_INA219
    pInterface[EM_MODULE_SENSORS_INA219_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    pInterface[EM_MODULE_SENSORS_ULTRASONIC_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    pInterface[EM_MODULE_SENSORS_DOOR_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    pInterface[EM_MODULE_SENSORS_MOTION_ID] = new mMotionSensor();
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    pInterface[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    pInterface[EM_MODULE_SENSORS_PULSECOUNTER_ID] = new X();
  #endif
  // Controllers
  #ifdef USE_MODULE_CUSTOM_BLINDS
    pInterface[EM_MODULE_CUSTOM_BLINDS_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_HEATING
    pInterface[EM_MODULE_CUSTOM_HEATING_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    pInterface[EM_MODULE_CUSTOM_RADIATORFAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_IRTRANSMITTER
    pInterface[EM_MODULE_CUSTOM_IRTRANSMITTER_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    pInterface[EM_MODULE_CUSTOM_OILFURNACE_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    pInterface[EM_MODULE_CUSTOM_EXERCISEBIKE_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    pInterface[EM_MODULE_CUSTOM_SONOFF_IFAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_FAN
    pInterface[EM_MODULE_CUSTOM_FAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_TREADMILL
    pInterface[EM_MODULE_CUSTOM_TREADMILL_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_SENSORCOLOURS
    pInterface[EM_MODULE_CUSTOM_SENSORCOLOURS_ID] = new X();
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    pInterface[EM_MODULE_CONTROLLER_DOORBELL_ID] = new X();
  #endif

}

uint16_t mTaskerManager::GetClassCount(){
  return EM_MODULE_LENGTH_ID;
}


/**
 * Default is Tasker_Interface(uint8_t function) with target_tasker = 0. If 0, all classes are called. 
 If !0, a specific tasker will be called and this function will exit after completion
 * */
int8_t mTaskerManager::Tasker_Interface(uint8_t function, uint8_t target_tasker, bool flags_is_executing_rule){


  int8_t result = 0;

  DEBUG_LINE;
  // Serial.printf("Tasker_Interface %d %d\n\r", function, target_tasker); Serial.flush();

  if(target_tasker){
    #ifdef ENABLE_LOG_LEVEL_INFO
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker %d %s"),target_tasker,GetModuleFriendlyName(target_tasker));
    #endif// ENABLE_LOG_LEVEL_INFO
  }

  // Serial.printf("Tasker_Interface2 %d %d\n\r", function, target_tasker); Serial.flush();

  // Check system is safe to run
  if(function==FUNC_PRE_INIT){
    // InitRules();
    // #ifdef ENABLE_LOG_LEVEL_INFO
    // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST "FUNC_PRE_INIT  GetClassCount %d"),module_settings.count);
    // #endif// ENABLE_LOG_LEVEL_INFO
  }

  #ifdef USE_MODULE_CORE_RULES
// I dont think i want this in here, as not only do I need to pass a trigger event, I might need to pass (or change a struct) tp include matching values
// For now, this function is called always, later it will only happen without a set range of function_inputs
  if(flags_is_executing_rule != true){
    pCONT_rules->Tasker_Rules_Interface(function);
  }
  #endif
  
  uint8_t fModule_present = false;
  #if defined(ENABLE_ADVANCED_DEBUGGING) || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
    char buffer_taskname[50];
  #endif
//   for(uint8_t i=0;i<module_settings.count;i++){
//     // If target_tasker != 0, then use it, else, use indexed array

// // Serial.printf("%s\n\r",pCONT_mqtt->MqttIsConnected()?"con":"NOT CONNECTED");
//     DEBUG_LINE;
//     switch_index = target_tasker ? target_tasker : module_settings.list[i];
//     #ifdef ENABLE_ADVANCED_DEBUGGING
//       AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%02d %s\t%S"),
//         switch_index, 
//         GetTaskName(function, buffer_taskname),
//         GetModuleFriendlyName(switch_index));
//     #endif
    
//     // char buffer_taskname[40];
//     //   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%02d %s\t%S"),
//     //     switch_index, 
//     //     GetTaskName(function, buffer_taskname),
//     //     GetModuleFriendlyName(switch_index));
//     // DEBUG_LINE;

//     // Remember start millis
//     #if defined(DEBUG_EXECUTION_TIME) || defined(ENABLE_ADVANCED_DEBUGGING)  || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
//     uint32_t start_millis = millis();
//     #endif
//     fModule_present = true; //assume true
    


  for(auto& task:pInterface){
    task->Tasker(function);
  }

  //   #if defined(DEBUG_EXECUTION_TIME)  || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
  //   // Remember start millis
  //   uint32_t end_millis = millis();

  //   // Get this execution time 
  //   uint32_t this_millis = end_millis - start_millis;
    
  //   #if defined(DEBUG_EXECUTION_TIME)
  //   // Get average
  //   if(fModule_present){ //only update tasks that run .. IMPROVE this later with flags (manually) or via returns of tasks
  //     module_settings.execution_time_average_ms[i] += this_millis;
  //     module_settings.execution_time_average_ms[i] /= 2; //gets average

  //     // Get max
  //     if(this_millis > module_settings.execution_time_max_ms[i]){
  //       module_settings.execution_time_max_ms[i] = this_millis; // remember max
  //     }
  //   }
  //   #endif // DEBUG_EXECUTION_TIME
  //   #endif
    
  //   #ifdef ENABLE_ADVANCED_DEBUGGING
  //     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " module completed \t%d ms %s"),millis()-start_millis, GetTaskName(function, buffer_taskname));
  //   #endif
  //   #if defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
  //     if(this_millis > 500){
  //       AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%d ms %s %S"),millis()-start_millis, GetTaskName(function, buffer_taskname), GetModuleFriendlyName(switch_index));
  //     }
  //   #endif

  //   // print boot info as it proceeds
  //   // if(function == FUNC_INIT){
  //   //   AddLog_P(LOG_LEVEL_INFO, PSTR("\t\tBootProgress: %s %d %%"),GetModuleFriendlyName(switch_index),map(i,0,module_settings.count,0,100));
  //   // }

  //   if(target_tasker!=0){
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //     AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker EXITING EARLY"));
  //   #endif// ENABLE_LOG_LEVEL_INFO
  //     break; //only run for loop for the class set. if 0, rull all
  //   }
  //   // Special flag that can be set to end interface ie event handled, no need to check others
  //   if(fExitTaskerWithCompletion){fExitTaskerWithCompletion=false;
  //   #ifdef ENABLE_LOG_LEVEL_INFO
  //     AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
  //   #endif// ENABLE_LOG_LEVEL_INFO
  //     break; //only run for loop for the class set. if 0, rull all
  //   }
    
  // DEBUG_LINE;

  // } //end for

  DEBUG_LINE;
  if(!pCONT_set->flag_boot_complete){
    char buffer_taskname[50];
    if(function != last_function){
      uint8_t boot_percentage = map(function,0,FUNC_ON_BOOT_COMPLETE,0,100);
      // Optional progress bar using block symbols?
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

  DEBUG_LINE;
  return result;

}


//ifdef debug only include then
uint16_t mTaskerManager::GetClassSizeByID(uint8_t class_id){
    
  switch(class_id){
    // CoreSystem
    #ifdef D_MODULE_NETWORK_WIFI_ID
      case D_MODULE_NETWORK_WIFI_ID:  return sizeof(mWiFi);
    #endif
    #ifdef D_MODULE_CORE_SETTINGS_ID
      case D_MODULE_CORE_SETTINGS_ID: return sizeof(mSettings);
    #endif
    #ifdef D_MODULE_CORE_SUPPORT_ID
      case D_MODULE_CORE_SUPPORT_ID: return sizeof(mSupport);
    #endif
    #ifdef D_MODULE_CORE_LOGGING_ID
      case D_MODULE_CORE_LOGGING_ID: return sizeof(mLogging);
    #endif
    #ifdef D_MODULE_CORE_TIME_ID
      case D_MODULE_CORE_TIME_ID: return sizeof(mTime);
    #endif
    #ifdef D_MODULE_NETWORK_MQTT_ID
      case D_MODULE_NETWORK_MQTT_ID: return sizeof(mMQTT);
    #endif
    #ifdef D_MODULE_NETWORK_WEBSERVER_ID
      case D_MODULE_NETWORK_WEBSERVER_ID:   return sizeof(mWebServer);
    #endif
    #ifdef D_MODULE_CORE_TELEMETRY_ID
      case D_MODULE_CORE_TELEMETRY_ID:     return sizeof(mTelemetry);
    #endif
    #ifdef D_MODULE_CORE_HARDWAREPINS_ID
      case D_MODULE_CORE_HARDWAREPINS_ID:   return sizeof(mHardwarePins);
    #endif
    #ifdef D_MODULE_CORE_RULES_ID
      case D_MODULE_CORE_RULES_ID:   return sizeof(mRuleEngine);
    #endif
    #ifdef D_MODULE_DRIVERS_STATUS_LEDS_ID
      case D_MODULE_DRIVERS_STATUS_LEDS_ID: return sizeof(mStatusLEDs);
    #endif

    //Sensors
    #ifdef D_MODULE_SENSORS_DOOR_ID
      case D_MODULE_SENSORS_DOOR_ID:  return sizeof(mDoorSensor);
    #endif
    #ifdef D_MODULE_SENSORS_MOTION_ID
      case D_MODULE_SENSORS_MOTION_ID: return sizeof(mMotionSensor);
    #endif
    #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
      case D_MODULE_SENSORS_PZEM004T_MODBUS_ID: return sizeof();
    #endif
    #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
      case D_MODULE_SENSORS_ULTRASONIC_ID: return sizeof();
    #endif
    #ifdef D_MODULE_SENSORS_DHT_ID
      case D_MODULE_SENSORS_DHT_ID: return sizeof();
    #endif
    #ifdef D_MODULE_SENSORS_BME_ID
      case D_MODULE_SENSORS_BME_ID: return sizeof(mSensorsBME);
    #endif
    #ifdef D_MODULE_SENSORS_DB18S20_ID
      case D_MODULE_SENSORS_DB18S20_ID: return sizeof();
    #endif
    #ifdef D_MODULE_SENSORS_INA219_ID
      case D_MODULE_SENSORS_INA219_ID: return sizeof();
    #endif
    #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
      case D_MODULE_CUSTOM_RADIATORFAN_ID: return sizeof();
    #endif
    #ifdef USE_MODULE_CUSTOM_BLINDS
      case D_MODULE_CUSTOM_BLINDS_ID: return sizeof();
    #endif
    #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
      case D_MODULE_CUSTOM_OILFURNACE_ID: return sizeof();
    #endif
    #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
      case D_MODULE_CUSTOM_EXERCISEBIKE_ID: return sizeof();
    #endif
    #ifdef D_MODULE_CUSTOM_HEATING_ID
      case D_MODULE_CUSTOM_HEATING_ID: return sizeof();
    #endif
    #ifdef D_MODULE_DRIVERS_RELAY_ID
      case D_MODULE_DRIVERS_RELAY_ID:  return sizeof();
    #endif
    #ifdef D_MODULE_DRIVERS_SDCARD_ID
      case D_MODULE_DRIVERS_SDCARD_ID:   return sizeof();
    #endif
    #ifdef D_MODULE_DRIVERS_GPS_ID
      case D_MODULE_DRIVERS_GPS_ID:     return sizeof();
    #endif
    #ifdef D_MODULE_DRIVERS_SERIAL_UART_ID
      case D_MODULE_DRIVERS_SERIAL_UART_ID: return sizeof();
    #endif
    #ifdef D_MODULE_DRIVERS_SHELLY_DIMMER_ID
      case D_MODULE_DRIVERS_SHELLY_DIMMER_ID:  return sizeof();
    #endif
    #ifdef D_MODULE_DRIVERS_CAMERA_OV2640_ID
      case D_MODULE_DRIVERS_CAMERA_OV2640_ID:   return sizeof();
    #endif

    #ifdef D_MODULE_DRIVERS_PWM_ID
      case D_MODULE_DRIVERS_PWM_ID:    return sizeof();
    #endif

    // Customs to be named "Controllers or Systems"
    #ifdef D_MODULE_CUSTOM_SONOFF_IFAN_ID
      case D_MODULE_CUSTOM_SONOFF_IFAN_ID:   return sizeof();
    #endif
    #ifdef D_MODULE_CUSTOM_FAN_ID
      case D_MODULE_CUSTOM_FAN_ID:   return sizeof();
    #endif
    #ifdef D_MODULE_CUSTOM_SENSORCOLOURS_ID
      case D_MODULE_CUSTOM_SENSORCOLOURS_ID: return sizeof();
    #endif
    #ifdef D_MODULE_CUSTOM_TREADMILL_ID
      case D_MODULE_CUSTOM_TREADMILL_ID: return sizeof();
    #endif
    #ifdef D_MODULE_CONTROLLER_DOORBELL_ID
      case D_MODULE_CONTROLLER_DOORBELL_ID: return sizeof();
    #endif



    #ifdef D_MSAW_MODULE_ID
      case D_MSAW_MODULE_ID:   return sizeof();
    #endif
    #ifdef D_MODULE_DISPLAYS_NEXTION_ID
      case D_MODULE_DISPLAYS_NEXTION_ID:  return sizeof(mNextionPanel);
    #endif
    #ifdef D_MODULE_SENSORS_BUTTONS_ID
      case D_MODULE_SENSORS_BUTTONS_ID:    return sizeof();
    #endif
    #ifdef D_MODULE_SENSORS_SWITCHES_ID
      case D_MODULE_SENSORS_SWITCHES_ID:   return sizeof();
    #endif
    #ifdef D_MODULE_SENSORS_ANALOG_ID
      case D_MODULE_SENSORS_ANALOG_ID:  return sizeof();
    #endif
    #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
      case D_MODULE_SENSORS_PULSECOUNTER_ID:  return sizeof();
    #endif
    //Drivers
    #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
      case D_MODULE_DRIVERS_IRTRANSCEIVER_ID: return sizeof();
    #endif
    #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
      case D_MODULE_DRIVERS_HBRIDGE_ID:    return sizeof();
    #endif


    /**
     * Lights
      * */
    #ifdef D_MODULE_LIGHTS_INTERFACE_ID
      case D_MODULE_LIGHTS_INTERFACE_ID:    return sizeof(mInterfaceLight);
    #endif
    #ifdef D_MODULE_LIGHTS_ANIMATOR_ID
      case D_MODULE_LIGHTS_ANIMATOR_ID:    return sizeof(mAnimatorLight);
    #endif
    #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
      case D_MODULE_LIGHTS_ADDRESSABLE_ID:     return sizeof(mAddressableLight);
    #endif
    #ifdef D_MODULE_LIGHTS_PWM_ID
      case D_MODULE_LIGHTS_PWM_ID:    return sizeof(mPWMLight);
    #endif  
    #ifdef D_MODULE_LIGHTS_WLED_EFFECTS_ID
      case D_MODULE_LIGHTS_WLED_EFFECTS_ID:     return sizeof(mWLEDEffects);
    #endif  



    #ifdef D_MODULE_DRIVERS_ENERGY_ID
      case D_MODULE_DRIVERS_ENERGY_ID:  return sizeof();
    #endif
    //default
    default: return -1; 
  } //end switch

  return -1;

}



int16_t mTaskerManager::GetModuleIDbyFriendlyName(const char* c){

  if(c=='\0'){ return -1; }

  for(int ii=0;ii<GetClassCount();ii++){
    if(strcasecmp_P(c, pInterface[ii]->GetModuleFriendlyName())==0){
      AddLog_P(LOG_LEVEL_TEST, PSTR("MATCHED GetModuleIDbyFriendlyName \"%s\" => \"%s\" %d"),c,pInterface[ii]->GetModuleFriendlyName(),ii);
      return ii;//mTasksIDs[ii];
    }    
  }
  return -1;
}


int16_t mTaskerManager::GetVectorIndexbyModuleID(uint8_t id_search)
{
  return id_search < GetClassCount() ? id_search : -1;
}

bool mTaskerManager::ValidTaskID(uint8_t module_id){
  // return GetVectorIndexbyModuleID(module_id) >= 0 ? true : false;
  return module_id < GetClassCount() ? true : false;
}


PGM_P mTaskerManager::GetModuleFriendlyName(uint8_t module_id){
  if(!ValidTaskID(module_id)){
    return PM_SEARCH_NOMATCH;
  }  
  return pInterface[module_id]->GetModuleFriendlyName();
}


PGM_P mTaskerManager::GetModuleName(uint8_t module_id){
  if(!ValidTaskID(module_id)){
    return PM_SEARCH_NOMATCH;
  }  
  return pInterface[module_id]->GetModuleName();
}
  

