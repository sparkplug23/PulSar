#include "1_TaskerManager/mTaskerManager.h"


/* Null, because instance will be initialized on demand. */
mTaskerManager* mTaskerManager::instance = nullptr;


mTaskerManager* mTaskerManager::GetInstance(){
  if (instance == nullptr){
    instance = new mTaskerManager();
  }
  return instance;
}



// Checks if defined pointers are NOT nullptr and therefore initiated
uint8_t mTaskerManager::Instance_Init(){
  
  // Core
  #ifdef USE_MODULE_CORE_HARDWAREPINS
  mTasks[EM_MODULE_CORE_HARDWAREPINS_ID] = new mHardwarePins();
  #endif 
  #ifdef USE_MODULE_CORE_SETTINGS
  mTasks[EM_MODULE_CORE_SETTINGS_ID] = new mSettings();
  #endif 
  #ifdef USE_MODULE_CORE_SUPPORT
  mTasks[EM_MODULE_CORE_SUPPORT_ID] = new mSupport();
  #endif 
  #ifdef USE_MODULE_CORE_LOGGING
  mTasks[EM_MODULE_CORE_LOGGING_ID] = new mLogging();
  #endif 
  #ifdef USE_MODULE_CORE_TELEMETRY
  mTasks[EM_MODULE_CORE_TELEMETRY_ID] = new mTelemetry();
  #endif 
  #ifdef USE_MODULE_CORE_TIME
  mTasks[EM_MODULE_CORE_TIME_ID] = new mTime();
  #endif 
  // Network
  #ifdef USE_MODULE_NETWORK_WIFI
  mTasks[EM_MODULE_NETWORK_WIFI_ID] = new mWiFi();
  #endif 
  #ifdef USE_MODULE_NETWORK_MQTT
  mTasks[EM_MODULE_NETWORK_MQTT_ID] = new mMQTT();
  #endif 
  #ifdef USE_MODULE_CORE_WEBSERVER
  mTasks[EM_MODULE_NETWORK_WEBSERVER_ID] = new mWebserver();
  #endif
  // Displays
  #ifdef USE_MODULE_DISPLAYS_INTERFACE
    mTasks[EM_MODULE_DISPLAYS_NEXTION_ID] = new X();
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    mTasks[EM_MODULE_DISPLAYS_NEXTION_ID] = new X();
  #endif
  // Drivers (Range 40-129)
  #ifdef USE_MODULE_DRIVERS_INTERFACE
    mTasks[EM_MODULE_DRIVERS_HBRIDGE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    mTasks[EM_MODULE_DRIVERS_HBRIDGE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    mTasks[EM_MODULE_DRIVERS_IRTRANSCEIVER_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    mTasks[EM_MODULE_DRIVERS_RELAY_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_PWM
    mTasks[EM_MODULE_DRIVERS_PWM_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_RF433MHZ
    mTasks[EM_MSAW_MODULE_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SDCARD
    mTasks[EM_MODULE_DRIVERS_SDCARDID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_GPS
    mTasks[EM_MODULE_DRIVERS_GPS_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SERIAL_UART
    mTasks[EM_MODULE_DRIVERS_SERIAL_UART_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
    mTasks[EM_MODULE_DRIVERS_SHELLY_DIMMER_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
    mTasks[EM_MODULE_DRIVERS_CAMERA_OV2640_ID] = new X();
  #endif
  #ifdef USE_MODULE_DRIVERS_LEDS
    mTasks[EM_MODULE_DRIVERS_STATUS_LEDS_ID] = new X();
  #endif
  // Energy
  #ifdef USE_MODULE_DRIVERS_ENERGY
    mTasks[EM_MODULE_DRIVERS_ENERGY_ID] = new X();
  #endif
  // Lights
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    mTasks[EM_MODULE_LIGHTS_INTERFACE_ID] = new mInterfaceLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_ANIMATOR
    mTasks[EM_MODULE_LIGHTS_ANIMATOR_ID] = new mAnimatorLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    mTasks[EM_MODULE_LIGHTS_ADDRESSABLE_ID] = new mAddressableLight();
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    mTasks[EM_MODULE_LIGHTS_PWM_ID] = new X();
  #endif
  #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
    mTasks[EM_MODULE_LIGHTS_WLED_EFFECTS_ID] = new X();
  #endif
  // Sensors
  #ifdef USE_MODULE_SENSORS_BUTTONS
    mTasks[EM_MODULE_SENSORS_BUTTONS_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    mTasks[EM_MODULE_SENSORS_SWITCHES_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_ANALOG
    mTasks[EM_MODULE_SENSORS_ANALOG_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    mTasks[EM_MODULE_SENSORS_PZEM004T_MODBUS_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    mTasks[EM_MODULE_SENSORS_DHT_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    mTasks[EM_MODULE_SENSORS_BME_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    mTasks[EM_MODULE_SENSORS_DB18S20_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_INA219
    mTasks[EM_MODULE_SENSORS_INA219_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    mTasks[EM_MODULE_SENSORS_ULTRASONIC_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    mTasks[EM_MODULE_SENSORS_DOOR_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    mTasks[EM_MODULE_SENSORS_MOTION_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    mTasks[EM_MODULE_SENSORS_RESISTIVE_MOISTURE_ID] = new X();
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    mTasks[EM_MODULE_SENSORS_PULSECOUNTER_ID] = new X();
  #endif
  // Controllers
  #ifdef USE_MODULE_CUSTOM_BLINDS
    mTasks[EM_MODULE_CUSTOM_BLINDS_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_HEATING
    mTasks[EM_MODULE_CUSTOM_HEATING_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    mTasks[EM_MODULE_CUSTOM_RADIATORFAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_IRTRANSMITTER
    mTasks[EM_MODULE_CUSTOM_IRTRANSMITTER_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    mTasks[EM_MODULE_CUSTOM_OILFURNACE_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    mTasks[EM_MODULE_CUSTOM_EXERCISEBIKE_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
    mTasks[EM_MODULE_CUSTOM_SONOFF_IFAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_FAN
    mTasks[EM_MODULE_CUSTOM_FAN_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_TREADMILL
    mTasks[EM_MODULE_CUSTOM_TREADMILL_ID] = new X();
  #endif
  #ifdef USE_MODULE_CUSTOM_SENSORCOLOURS
    mTasks[EM_MODULE_CUSTOM_SENSORCOLOURS_ID] = new X();
  #endif
  #ifdef USE_MODULE_CONTROLLER_DOORCHIME
    mTasks[EM_MODULE_CONTROLLER_DOORBELL_ID] = new X();
  #endif

  // for(auto& task:mTasksIDs){
  //   Serial.printf("task:mTasksIDs=%d\n\r",mTasksIDs[task]); Serial.flush();
  // }


}


// // Checks if defined pointers are NOT nullptr and therefore initiated
// uint8_t mTaskerManager::Instance_Init(){
  
//   mTasks[EM_MODULE_NETWORK_MQTT_ID] = new mMQTT();

//   mTasks[] = new mHardwarePins());
//   mTasksIDs.push_back(D_MODULE_CORE_HARDWAREPINS_ID);

//   mTasks.push_back(new mTime());
//   mTasksIDs.push_back(D_MODULE_CORE_TIME_ID);

//   mTasks.push_back(new mSettings());
//   mTasksIDs.push_back(D_MODULE_CORE_SETTINGS_ID);

//   mTasks.push_back(new mSupport());
//   mTasksIDs.push_back(D_MODULE_CORE_SUPPORT_ID);

//   mTasks.push_back(new mWiFi());
//   mTasksIDs.push_back(D_MODULE_NETWORK_WIFI_ID);

//   #ifdef USE_MODULE_CORE_WEBSERVER
//   mTasks.push_back(new mWebServer());
//   mTasksIDs.push_back(D_MODULE_NETWORK_WEBSERVER_ID);
//   #endif //USE_MODULE_CORE_WEBSERVER

//   mTasks.push_back(new mLogging());
//   mTasksIDs.push_back(D_MODULE_CORE_LOGGING_ID);

//   mTasks.push_back(new mTelemetry());
//   mTasksIDs.push_back(D_MODULE_CORE_TELEMETRY_ID);

//   #ifdef USE_MODULE_CORE_RULES
//   mTasks.push_back(new mRuleEngine());
//   mTasksIDs.push_back(D_MODULE_CORE_RULES_ID);
//   #endif // USE_MODULE_CORE_RULES

//   // Sensors
//   #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
//   mTasks.push_back(new mPzem_AC());
//   mTasksIDs.push_back(D_MODULE_SENSORS_PZEM004T_MODBUS_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_DHT
//   mTasks.push_back(new mSensorsDHT());
//   mTasksIDs.push_back(D_MODULE_SENSORS_DHT_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_BME
//   mTasks.push_back(new mSensorsBME());
//   mTasksIDs.push_back(D_MODULE_SENSORS_BME_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_DS18B20
//   mTasks.push_back(new mSensorsDB18());
//   mTasksIDs.push_back(D_MODULE_SENSORS_DB18S20_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_INA219
//   mTasks.push_back(new mSensorsINA219());
//   mTasksIDs.push_back(D_MODULE_SENSORS_INA219_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_MOTION
//   mTasks.push_back(new mMotionSensor());
//   mTasksIDs.push_back(D_MODULE_SENSORS_MOTION_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_MOISTURE
//   mTasks.push_back(new mMoistureSensor());
//   mTasksIDs.push_back(D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_DOOR
//   mTasks.push_back(new mDoorSensor());
//   mTasksIDs.push_back(D_MODULE_SENSORS_DOOR_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
//   mTasks.push_back(new mPulseCounter());
//   mTasksIDs.push_back(D_MODULE_SENSORS_PULSECOUNTER_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_BUTTONS
//   mTasks.push_back(new mButtons());
//   mTasksIDs.push_back(D_MODULE_SENSORS_BUTTONS_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_SWITCHES
//   mTasks.push_back(new mSwitches());
//   mTasksIDs.push_back(D_MODULE_SENSORS_SWITCHES_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_ANALOG
//   mTasks.push_back(new mSensorsAnalog());
//   mTasksIDs.push_back(D_MODULE_SENSORS_ANALOG_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
//   mTasks.push_back(new mPzem_AC());
//   mTasksIDs.push_back(D_MODULE_SENSORS_PZEM004T_MODBUS_ID);
//   #endif

//   /**
//    * Lights
//    * */
//   #ifdef USE_MODULE_LIGHTS_INTERFACE
//   mTasks.push_back(new mInterfaceLight());
//   mTasksIDs.push_back(D_MODULE_LIGHTS_INTERFACE_ID);
//   #endif  
//   #ifdef USE_MODULE_LIGHTS_ANIMATOR
//   mTasks.push_back(new mAnimatorLight());
//   mTasksIDs.push_back(D_MODULE_LIGHTS_ANIMATOR_ID);
//   #endif 
//   #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//   mTasks.push_back(new mAddressableLight());
//   mTasksIDs.push_back(D_MODULE_LIGHTS_ADDRESSABLE_ID);
//   #endif
//   #ifdef USE_MODULE_LIGHTS_PWM
//   mTasks.push_back(new mPWMLight());
//   mTasksIDs.push_back(D_MODULE_LIGHTS_PWM_ID);
//   #endif
//   #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
//   mTasks.push_back(new mWLEDEffects());
//   mTasksIDs.push_back(D_MODULE_LIGHTS_WLED_EFFECTS_ID);
//   #endif

//   /**
//    * Drivers
//    * */
//   #if defined(USE_MODULE_CUSTOM_IRTRANSMITTER) || defined (USE_IR_RECEIVER)
//     // #ifdef ESP32
//   #endif
//   #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined(USE_MODULE_DRIVERS_RF433MHZ)
//     mSAWTransceive mst;
//     mSAWMain msm;
//     mSAWProtocol msp;
//   #endif
//   #ifdef USE_MODULE_CUSTOM_RADIATORFAN
//   mTasks.push_back(new mRadiatorFan());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_RADIATORFAN_ID);
//   #endif
//   #ifdef USE_MODULE_CUSTOM_BLINDS
//   mTasks.push_back(new mBlinds());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_BLINDS_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_HBRIDGE
//   mTasks.push_back(new mHBridge());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_HBRIDGE_ID);
//   #endif
//   #ifdef USE_MODULE_SENSORS_ULTRASONICS
//   mTasks.push_back(new mUltraSonicSensor());
//   mTasksIDs.push_back(D_MODULE_SENSORS_ULTRASONIC_ID);
//   #endif
//   #ifdef USE_MODULE_CUSTOM_OILFURNACE
//   mTasks.push_back(new mOilFurnace());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_OILFURNACE_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
//   mTasks.push_back(new mIRtransceiver());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_IRTRANSCEIVER_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_RELAY
//   mTasks.push_back(new mRelays());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_RELAY_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SDCARD
//   mTasks.push_back(new mSDCard());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_SDCARD_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_GPS
//   mTasks.push_back(new mGPS());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_GPS_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SERIAL_UART
//   mTasks.push_back(new mSerialUART());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_SERIAL_UART_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
//   mTasks.push_back(new mShellyDimmer());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_SHELLY_DIMMER_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
//   mTasks.push_back(new mCameraOV2640());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_CAMERA_OV2640_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_LEDS
//   mTasks.push_back(new mStatusLEDs());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_STATUS_LEDS_ID);
//   #endif // USE_MODULE_DRIVERS_LEDS


//   #ifdef USE_MODULE_DRIVERS_PWM
//   mTasks.push_back(new mPWM());
//   mTasksIDs.push_back(D_MODULE_LIGHTS_PWM_ID);
//   #endif
//   #ifdef USE_MODULE_DISPLAYS_NEXTION
//   mTasks.push_back(new mNextionPanel());
//   mTasksIDs.push_back(D_MODULE_DISPLAYS_NEXTION_ID);
//   #endif
//   #ifdef USE_MODULE_DRIVERS_ENERGY
//   mTasks.push_back(new mEnergy());
//   mTasksIDs.push_back(D_MODULE_DRIVERS_ENERGY_ID);
//   #endif

//   // Controllers
//   #ifdef USE_MODULE_CUSTOM_HEATING
//   mTasks.push_back(new mHeating());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_HEATING_ID);
//   #endif
//   #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
//   mTasks.push_back(new mExerciseBike());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_EXERCISEBIKE_ID);
//   #endif
//   #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
//   mTasks.push_back(new mSonoffIFan());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_SONOFF_IFAN_ID);
//   #endif
//   #ifdef USE_MODULE_CUSTOM_FAN
//   mTasks.push_back(new mFan());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_FAN_ID);
//   #endif
//   #ifdef USE_MODULE_CUSTOM_TREADMILL
//   mTasks.push_back(new mTreadmill());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_TREADMILL_ID);
//   #endif
//   #ifdef USE_MODULE_CUSTOM_SENSORCOLOURS
//   mTasks.push_back(new mSensorColours());
//   mTasksIDs.push_back(D_MODULE_CUSTOM_SENSORCOLOURS_ID);
//   #endif
//   #ifdef USE_MODULE_CONTROLLER_DOORCHIME
//   mTasks.push_back(new mDoorBell());
//   mTasksIDs.push_back(D_MODULE_CONTROLLER_DOORBELL_ID);
//   #endif

//   for(auto& task:mTasksIDs){
//     Serial.printf("task:mTasksIDs=%d\n\r",mTasksIDs[task]); Serial.flush();
//   }


// }


uint16_t mTaskerManager::GetClassCount(){
  return EM_MODULE_LENGTH_ID;//mTasksIDs.size();
}


/**
 * 
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
    


  for(auto& task:mTasks){
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
      uint8_t boot_percentage = map(function,0,FUNC_ON_SUCCESSFUL_BOOT,0,100);
      // Optional progress bar using block symbols?
      //5% = 1 bar, 20 bars total [B                   ]
      //if(pCONT_set->Settings.seriallog_level >= LOG_LEVEL_INFO){
      #ifndef DISABLE_SERIAL_LOGGING
      Serial.printf("[");
      for(uint8_t percent=0;percent<100;percent+=5){  
        if(percent<boot_percentage){ Serial.print((char)219); }else{ Serial.printf(" "); }
      }
      Serial.printf("] %03d %s\n\r",boot_percentage,GetTaskName(function, buffer_taskname));
      #endif
      //}
      last_function = function;
    }
    if(function == FUNC_ON_SUCCESSFUL_BOOT){ pCONT_set->flag_boot_complete = true; }
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


// Switch case should be faster than getext progmem
// Use progmem WITHOUT buffer for speed improvements, should be read as expected progmem and handled that way
const char* mTaskerManager::GetTaskName(uint8_t task, char* buffer){

  switch(task){
    default:
    case FUNC_POINTER_INIT:                           return PM_FUNC_POINTER_INIT_CTR;
    case FUNC_TEMPLATE_MODULE_LOAD:                          return PM_FUNC_TEMPLATE_LOAD_CTR;
    case FUNC_MODULE_INIT:                            return PM_FUNC_MODULE_INIT_CTR;
    case FUNC_PRE_INIT:                               return PM_FUNC_PRE_INIT_CTR;
    case FUNC_INIT:                                   return PM_FUNC_INIT_CTR;
    case FUNC_CONFIGURE_MODULES_FOR_DEVICE:           return PM_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR;
    case FUNC_LOOP:                                   return PM_FUNC_LOOP_CTR;
    case FUNC_EVERY_50_MSECOND:                       return PM_FUNC_EVERY_50_MSECOND_CTR;
    case FUNC_EVERY_100_MSECOND:                      return PM_FUNC_EVERY_100_MSECOND_CTR;
    case FUNC_EVERY_200_MSECOND:                      return PM_FUNC_EVERY_200_MSECOND_CTR;
    case FUNC_EVERY_250_MSECOND:                      return PM_FUNC_EVERY_250_MSECOND_CTR;
    case FUNC_EVERY_SECOND:                           return PM_FUNC_EVERY_SECOND_CTR;
    case FUNC_EVERY_MINUTE:                           return PM_FUNC_EVERY_MINUTE_CTR; 
    case FUNC_EVERY_HOUR:                             return PM_FUNC_EVERY_HOUR_CTR; 
    case FUNC_EVERY_MIDNIGHT:                         return PM_FUNC_EVERY_MIDNIGHT_CTR;
    case FUNC_EVERY_MIDDAY:                           return PM_FUNC_EVERY_MIDDAY_CTR;
    case FUNC_ON_SUCCESSFUL_BOOT:                     return PM_FUNC_ON_SUCCESSFUL_BOOT_CTR;
    case FUNC_UPTIME_10_SECONDS:                      return PM_FUNC_UPTIME_10_SECONDS_CTR;
    case FUNC_UPTIME_1_MINUTES:                       return PM_FUNC_UPTIME_1_MINUTES_CTR;
    case FUNC_UPTIME_10_MINUTES:                      return PM_FUNC_UPTIME_10_MINUTES_CTR;
    case FUNC_UPTIME_60_MINUTES:                      return PM_FUNC_UPTIME_60_MINUTES_CTR;
    // case FUNC_RESTART_SPLASH_INFORMATION:             return PM_FUNC_RESTART_SPLASH_INFORMATION_CTR;
    // case FUNC_PREP_BEFORE_TELEPERIOD:                 return PM_FUNC_PREP_BEFORE_TELEPERIOD_CTR;
    case FUNC_JSON_APPEND:                            return PM_FUNC_JSON_APPEND_CTR;
    case FUNC_SAVE_BEFORE_RESTART:                    return PM_FUNC_SAVE_BEFORE_RESTART_CTR;
    case FUNC_SETTINGS_DEFAULT:                       return PM_FUNC_SETTINGS_DEFAULT_CTR;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:    return PM_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR;
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
    case FUNC_MQTT_HANDLERS_RESET:                    return PM_FUNC_MQTT_HANDLERS_RESET_CTR;
    case FUNC_MQTT_HANDLERS_INIT:                     return PM_FUNC_MQTT_HANDLERS_INIT_CTR;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:       return PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
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
    case FUNC_CHECK_POINTERS:                         return PM_FUNC_CHECK_POINTERS_CTR;
    case FUNC_WEB_SYSTEM_INFO:                        return PM_FUNC_WEB_SYSTEM_INFO_CTR;
    case FUNC_DEBUG_CONFIGURE:                        return PM_FUNC_DEBUG_CONFIGURE_CTR;
  }

}




int16_t mTaskerManager::GetModuleIDbyFriendlyName(const char* c){

  if(c=='\0'){ return -1; }

  for(int ii=0;ii<GetClassCount();ii++){
    if(strcasecmp_P(c, mTasks[ii]->GetModuleFriendlyName())){
      return ii;//mTasksIDs[ii];
    }    
  }
  return -1;
}


int16_t mTaskerManager::GetVectorIndexbyModuleID(uint8_t id_search)
{
  if(id_search < GetClassCount()){
    return id_search;
  }

  // uint8_t counter = 0;
  // for(auto& id:mTasksIDs)
  // {
  //   if(id == id_search){
  //     return counter;
  //   }   
  //   counter++; 
  // }
  return -1;

}

bool mTaskerManager::ValidTaskID(uint8_t module_id){
  return GetVectorIndexbyModuleID(module_id) >= 0 ? true : false;
}


PGM_P mTaskerManager::GetModuleFriendlyName(uint8_t module_id){
  if(!ValidTaskID(module_id)){
    return PM_SEARCH_NOMATCH;
  }  
  return pModule(module_id)->GetModuleFriendlyName();
}


PGM_P mTaskerManager::GetModuleName(uint8_t module_id){
  if(!ValidTaskID(module_id)){
    return PM_SEARCH_NOMATCH;
  }  
  return pModule(module_id)->GetModuleName();
}
  

