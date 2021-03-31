// #include "1_TaskerManager/mTaskerManager.h"


// /* Null, because instance will be initialized on demand. */
// mTaskerManager* mTaskerManager::instance = nullptr;

// void mTaskerManager::uart_intr_handle_u2(void *arg){

// }

// mTaskerManager* mTaskerManager::GetInstance(){
//   if (instance == nullptr){
//     instance = new mTaskerManager();
//   }
//   return instance;
// }

// // Checks if defined pointers are NOT nullptr and therefore initiated
// uint8_t mTaskerManager::Instance_Init(){

//   if(mqt  == nullptr){ mqt  = new mMQTT();      }
//   if(mod  == nullptr){ mod  = new mHardwarePins(); }
//   if(mt   == nullptr){ mt   = new mTime();      }
//   if(mset == nullptr){ mset = new mSettings();  }
//   if(msup == nullptr){ msup = new mSupport();   }
//   if(mwif == nullptr){ mwif = new mWiFi();      }
//   #ifdef USE_MODULE_CORE_WEBSERVER
//   if(mweb == nullptr){ mweb = new mWebServer(); }
//   #endif //USE_MODULE_CORE_WEBSERVER
//   if(mso  == nullptr){ mso  = new mLogging(); }
//   if(mtel == nullptr){ mtel = new mTelemetry(); }

//   #ifdef USE_MODULE_CORE_RULES
//   if(mrules == nullptr){ mrules = new mRuleEngine();}//100); }
//   #endif // USE_MODULE_CORE_RULES

//   #ifdef USE_MODULE_DRIVERS_LEDS
//   if(mstatus_leds == nullptr){ mstatus_leds = new mStatusLEDs();}//100); }
//   #endif // USE_MODULE_DRIVERS_LEDS

//   // Sensors
//   #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
//     if(mspm == nullptr){ mspm = new mPzem_AC(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_DHT
//     if(msdht == nullptr){ msdht = new mSensorsDHT(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_BME
//     if(msbme == nullptr){ msbme = new mSensorsBME(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_DS18B20
//     if(msdb18 == nullptr){ msdb18 = new mSensorsDB18(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_INA219
//     if(msina219 == nullptr){ msina219 = new mSensorsINA219(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_MOTION
//     if(mms == nullptr){ mms = new mMotionSensor(); }
//     // if(mTasks[0] == nullptr){ mTasks[0] = new mMotionSensor(); }
//     mTasks.push_back(new mMotionSensor());
//   #endif
//   #ifdef USE_MODULE_SENSORS_MOISTURE
//     if(mois == nullptr){ mois = new mMoistureSensor(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_DOOR
//     if(mds == nullptr){ mds = new mDoorSensor(); }
//   #endif
//   #ifdef USE_MODULE_CONTROLLER_DOORCHIME
//     if(mdb == nullptr){ mdb = new mDoorBell(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
//     if(mpc == nullptr){ mpc = new mPulseCounter(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_BUTTONS
//     if(mbtn == nullptr){ mbtn = new mButtons(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_SWITCHES
//     if(mswh == nullptr){ mswh = new mSwitches(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_ANALOG
//     if(msanalog == nullptr){ msanalog = new mSensorsAnalog(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
//     if(mspm == nullptr){ mspm = new mPzem_AC(); }
//   #endif

//   /**
//    * Lights
//    * */
//   #ifdef USE_MODULE_LIGHTS_INTERFACE
//     if(minterface_light == nullptr){ minterface_light = new mInterfaceLight(); }
//   #endif  
//   #ifdef USE_MODULE_LIGHTS_ANIMATOR
//     if(mlights_animator == nullptr){ mlights_animator = new mAnimatorLight();}
//   #endif 
//   #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//     if(mlights_addressable == nullptr){ mlights_addressable = new mAddressableLight();}
//   #endif
//   #ifdef USE_MODULE_LIGHTS_PWM
//     if(mlights_pwm == nullptr){ mlights_pwm = new mPWMLight(); }
//   #endif
//   #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
//     if(mlights_wled == nullptr){ mlights_wled = new mWLEDEffects(); }
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
//     if(mrf == nullptr){ mrf = new mRadiatorFan(); }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_BLINDS
//     if(mbbl == nullptr){ mbbl = new mBlinds(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_HBRIDGE
//     if(mdhb == nullptr){ mdhb = new mHBridge(); }
//   #endif
//   #ifdef USE_MODULE_SENSORS_ULTRASONICS
//     if(mus == nullptr){ mus = new mUltraSonicSensor(); }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_OILFURNACE
//     if(mof == nullptr){ mof = new mOilFurnace(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
//     if(mir == nullptr){ mir = new mIRtransceiver(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_RELAY
//     if(mry == nullptr){ mry = new mRelays(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SDCARD
//     if(msdcard == nullptr){ msdcard = new mSDCard(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_GPS
//     if(mgps == nullptr){ mgps = new mGPS(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SERIAL_UART
//     if(mserial == nullptr){ mserial = new mSerialUART(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
//     if(mshelly_dimmer == nullptr){ mshelly_dimmer = new mShellyDimmer(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
//     if(mcamera_ov2640 == nullptr){ mcamera_ov2640 = new mCameraOV2640(); }
//   #endif


//   #ifdef USE_MODULE_DRIVERS_PWM
//     if(mpwm == nullptr){ mpwm = new mPWM(); }
//   #endif
//   #ifdef USE_MODULE_DISPLAYS_NEXTION
//     if(mnext == nullptr){ mnext = new mNextionPanel(); }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_ENERGY
//     if(mdenergy == nullptr){ mdenergy = new mEnergy(); }
//   #endif

//   // Controllers
//   #ifdef USE_MODULE_CUSTOM_HEATING
//     if(mh == nullptr){ mh = new mHeating(); }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
//     if(meb == nullptr){ meb = new mExerciseBike(); }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_SONOFF_IFAN
//     if(mifan == nullptr){ mifan = new mSonoffIFan(); }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_FAN
//     if(mfan == nullptr){ mfan = new mFan(); }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_TREADMILL
//     if(mtreadmill == nullptr){ mtreadmill = new mTreadmill(); }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_SENSORCOLOURS
//     if(msenscol == nullptr){ msenscol = new mSensorColours(); }
//   #endif


// }

// void mTaskerManager::Init_InterfaceHandler(){


//   // if(mqt  == nullptr){ mqt  = new mMQTT();      }
//   // if(mod  == nullptr){ mod  = new mHardwarePins(); }
//   // if(mt   == nullptr){ mt   = new mTime();      }
//   // if(mset == nullptr){ mset = new mSettings();  }
//   // if(msup == nullptr){ msup = new mSupport();   }
//   // if(mwif == nullptr){ mwif = new mWiFi();      }
//   // if(mweb == nullptr){ mweb = new mWebServer(); }
//   // if(mso  == nullptr){ mso  = new mLogging(); }
//   // if(mtel == nullptr){ mtel = new mTelemetry(); }


//   // int array[] = {mset, msup};

//   // mTaskerManager *array[30];
//   // array[0] = new mSettings();
//   // array[1] = new mSupport();

// }

// // Gets the class id's and stores in array, used in interface 
// uint8_t mTaskerManager::InitClassList(){

//   memset(&module_settings,0,sizeof(module_settings));

//   // Core system
//   #ifdef D_MODULE_CORE_TIME_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CORE_TIME_ID;
//   #endif
//   #ifdef D_MODULE_NETWORK_MQTT_ID
//     module_settings.list[module_settings.count++] = D_MODULE_NETWORK_MQTT_ID;
//   #endif
//   #ifdef D_MODULE_CORE_LOGGING_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CORE_LOGGING_ID;
//   #endif
//   #ifdef D_MODULE_NETWORK_WIFI_ID
//     module_settings.list[module_settings.count++] = D_MODULE_NETWORK_WIFI_ID;
//   #endif
//   #ifdef D_MODULE_CORE_SETTINGS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CORE_SETTINGS_ID;
//   #endif
//   #ifdef D_MODULE_CORE_SUPPORT_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CORE_SUPPORT_ID;
//   #endif
//   #ifdef D_MODULE_CORE_HARDWAREPINS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CORE_HARDWAREPINS_ID;
//   #endif
//   #ifdef D_MODULE_CORE_RULES_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CORE_RULES_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_STATUS_LEDS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_STATUS_LEDS_ID;
//   #endif


//   #ifdef D_MODULE_NETWORK_WEBSERVER_ID
//     module_settings.list[module_settings.count++] = D_MODULE_NETWORK_WEBSERVER_ID;
//   #endif
//   #ifdef D_MODULE_CORE_TELEMETRY_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CORE_TELEMETRY_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_BUTTONS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_BUTTONS_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_SWITCHES_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_SWITCHES_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_ANALOG_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_ANALOG_ID;
//   #endif

//   // Sensors
//   #ifdef D_MODULE_SENSORS_DHT_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_DHT_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_BME_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_BME_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_DB18S20_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_DB18S20_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_INA219_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_INA219_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_ULTRASONIC_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_DOOR_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_DOOR_ID;
//   #endif
//   #ifdef D_MODULE_CONTROLLER_DOORBELL_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CONTROLLER_DOORBELL_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_MOTION_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_MOTION_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_PULSECOUNTER_ID;
//   #endif
//   #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_SENSORS_PZEM004T_MODBUS_ID;
//   #endif

//   // Wireless
//   #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_IRTRANSCEIVER_ID;
//   #endif
//   #ifdef mSAWMain_ID
//     module_settings.list[module_settings.count++] = mSAWMain_ID;
//   #endif
//   #ifdef mSAWProtocol_ID
//     module_settings.list[module_settings.count++] = mSAWProtocol_ID;
//   #endif

//   /**
//    * Drivers
//    * */
//   #ifdef D_MODULE_DRIVERS_RELAY_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_RELAY_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_PWM_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_PWM_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_HBRIDGE_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_ENERGY_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_ENERGY_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_SDCARD_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_SDCARD_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_GPS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_GPS_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_SERIAL_UART_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_SERIAL_UART_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_SHELLY_DIMMER_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_SHELLY_DIMMER_ID;
//   #endif
//   #ifdef D_MODULE_DRIVERS_CAMERA_OV2640_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_CAMERA_OV2640_ID;
//   #endif

//   /**
//    * Lighting
//    * */
//   #ifdef D_MODULE_LIGHTS_INTERFACE_ID
//     module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_INTERFACE_ID;
//   #endif
//   #ifdef D_MODULE_LIGHTS_ANIMATOR_ID
//     module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_ANIMATOR_ID;
//   #endif  
//   #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
//     module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_ADDRESSABLE_ID;
//   #endif
//   #ifdef D_MODULE_LIGHTS_PWM_ID
//     module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_PWM_ID
//   #endif  
//   #ifdef D_MODULE_LIGHTS_WLED_EFFECTS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_WLED_EFFECTS_ID
//   #endif  

//   // Displays
//   #ifdef D_MODULE_DISPLAYS_NEXTION_ID
//     module_settings.list[module_settings.count++] = D_MODULE_DISPLAYS_NEXTION_ID;
//   #endif
  
//   //Specific
//   #ifdef D_MODULE_CUSTOM_BLINDS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_BLINDS_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_HEATING_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_HEATING_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_SONOFF_IFAN_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_SONOFF_IFAN_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_FAN_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_FAN_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_SENSORCOLOURS_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_SENSORCOLOURS_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_RADIATORFAN_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_OILFURNACE_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_EXERCISEBIKE_ID;
//   #endif
//   #ifdef D_MODULE_CUSTOM_TREADMILL_ID
//     module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_TREADMILL_ID;
//   #endif
//   // #ifdef D_MOILTANK_MODULE_ID
//   //   module_settings.list[module_settings.count++] = D_MOILTANK_MODULE_ID;
//   // #endif
  
// DEBUG_LINE;

// }

// uint16_t mTaskerManager::GetClassCount(){
//   return module_settings.count;
// }



// // Checks if defined pointers are NOT nullptr and therefore initiated
// uint8_t mTaskerManager::CheckPointersPass(){

// //probably phase out? singletons will remove need


//   // system
//   if(mset==nullptr){ return false; }
//   if(msup==nullptr){ return false; }
//   if(mwif==nullptr){ return false; }
//   #ifdef USE_MQTT
//     if(mqt==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_CORE_WEBSERVER
//     if(mweb==nullptr){ return false; }
//   #endif

//   //Lights
//   #ifdef USE_MODULE_LIGHTS_INTERFACE
//     if(minterface_light==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_LIGHTS_ANIMATOR
//     if(mlights_animator==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
//     if(mlights_addressable==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_LIGHTS_PWM
//     if(mlights_pwm==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_LIGHTS_WLED_EFFECTS
//     if(mlights_wled==nullptr){ return false; }
//   #endif

//   // Sensor
//   #ifdef USE_MODULE_SENSORS_DOOR
//     if(mds==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_SENSORS_MOTION
//     if(mms==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_SENSORS_DHT
//     if(msdht==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_SENSORS_BME
//     if(msbme==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_SENSORS_DS18B20
//     if(msdb18==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_SENSORS_INA219
//     if(msina219==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_SENSORS_MOISTURE
//     if(mois==nullptr){ return false; }
//   #endif



//   #ifdef USE_RTC
//     if(mt==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_RADIATORFAN
//     if(mrf==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_CUSTOM_BLINDS
//     if(mbbl==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_SENSORS_ULTRASONICS
//     if(mus==nullptr){ return false; }
//   #endif
//   // #ifdef USE_OILTANK
//   //   if(mot==nullptr){ return false; }
//   // #endif
//   #ifdef USE_MODULE_CUSTOM_HEATING
//     if(mh==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_RELAY
//     if(mry==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SDCARD
//     if(msdcard==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_GPS
//     if(mgps==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SERIAL_UART
//     if(mserial==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_SHELLY_DIMMER
//     if(mshelly_dimmer==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DRIVERS_CAMERA_OV2640
//     if(mcamera_ov2640==nullptr){ return false; }
//   #endif

//   #ifdef USE_MODULE_DRIVERS_PWM
//     if(mpwm==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DISPLAYS_NEXTION
//     if(mnext==nullptr){ return false; }
//   #endif
//   #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined(USE_MODULE_DRIVERS_RF433MHZ)
//     if(msm==nullptr){ return false; }
//   #endif
//   #ifdef D_MODULE_CUSTOM_SONOFF_IFAN_ID
//     if(mifan==nullptr){ return false; }
//   #endif
//   #ifdef D_MODULE_CUSTOM_FAN_ID
//     if(mfan==nullptr){ return false; }
//   #endif
//   #ifdef D_MODULE_CUSTOM_SENSORCOLOURS_ID
//     if(msenscol==nullptr){ return false; }
//   #endif
//   #ifdef USE_MODULE_DISPLAYS_NEXTION
//     if(mod==nullptr){ return false; }
//   #endif

//   // Drivers
//   #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
//     if(mdhb==nullptr){ return false; }
//   #endif
//   #ifdef D_MODULE_DRIVERS_ENERGY_ID
//     if(mdenergy==nullptr){ return false; }
//   #endif

//   return true;

// }


// // to replace interface found in tasmota 
// // single function will call ALL cpp files that have a Tasker, passing the command
// /**
//  * 
//  * Default is Tasker_Interface(uint8_t function) with target_tasker = 0. If 0, all classes are called. If !0, a specific tasker will be called and this function will exit after completion
//  * */

// int8_t mTaskerManager::Tasker_Interface(uint8_t function, uint8_t target_tasker, bool flags_is_executing_rule){
// // }
// // template<typename T>
// // int8_t mTaskerManager::Tasker_Interface(uint8_t function, T param1, uint8_t target_tasker = 0){

//   // if(function == FUNC_JSON_COMMAND_OBJECT){
//   // Serial.println("BEFORE FUNC_JSON_COMMAND_OBJECT TASKER"); Serial.flush();
//   // }

//   int8_t result = 0;

//   DEBUG_LINE;
//   // Serial.printf("Tasker_Interface %d %d\n\r", function, target_tasker); Serial.flush();

//   if(target_tasker){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker %d %s"),target_tasker,GetModuleFriendlyName(target_tasker));
//     #endif// ENABLE_LOG_LEVEL_INFO
//   }

//   // Serial.printf("Tasker_Interface2 %d %d\n\r", function, target_tasker); Serial.flush();

//   // Check system is safe to run
//   if(function==FUNC_CHECK_POINTERS){
//     // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%s"),GetTaskName(function));
//     if(!CheckPointersPass()){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST "CheckPointers FAILED!!"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//       return FUNCTION_RESULT_ERROR_POINTER_INVALID_ID;
//     }else{
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST "CheckPointers PASSED"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//       return FUNCTION_RESULT_SUCCESS_ID;
//     }
//   }else
//   if(function==FUNC_PRE_INIT){
//     InitClassList();
//     // InitRules();
//     #ifdef ENABLE_LOG_LEVEL_INFO
//     AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST "FUNC_PRE_INIT  GetClassCount %d"),module_settings.count);
//     #endif// ENABLE_LOG_LEVEL_INFO
//   }

  
//   #ifdef USE_MODULE_CORE_RULES

// // I dont think i want this in here, as not only do I need to pass a trigger event, I might need to pass (or change a struct) tp include matching values
  
// // For now, this function is called always, later it will only happen without a set range of function_inputs
//   if(flags_is_executing_rule != true){
//     pCONT_rules->Tasker_Rules_Interface(function);
//   }
//   #endif

//   DEBUG_LINE;

//   uint8_t fModule_present = false;
//   #if defined(ENABLE_ADVANCED_DEBUGGING) || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
//     char buffer_taskname[50];
//   #endif

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
    
//     switch(switch_index){
//       // CoreSystem
//       #ifdef D_MODULE_NETWORK_WIFI_ID
//         case D_MODULE_NETWORK_WIFI_ID:      

//           // #ifdef USE_INTERFACE_NEW
//           //   // result = (mLogging.*interfacehandler_logging.Tasker)(function);
//           // #else
//           // #endif

//           result = mwif->Tasker(function); 

        
//         break;
//       #endif
//       #ifdef D_MODULE_CORE_SETTINGS_ID
//         case D_MODULE_CORE_SETTINGS_ID:           
        
//           // #ifdef USE_INTERFACE_NEW
//           //   // result = (mLogging.*interfacehandler_logging.Tasker)(function);
//           // #else
//             result = mset->Tasker(function); 
//           // #endif

//         break;
//       #endif
//       #ifdef D_MODULE_CORE_SUPPORT_ID
//         case D_MODULE_CORE_SUPPORT_ID:    
        
//           // #ifdef DISBALE_TEST_SECTION
//           result = msup->Tasker(function);  
//           // #endif
          
//           break;
//       #endif
//       #ifdef D_MODULE_CORE_LOGGING_ID
//         case D_MODULE_CORE_LOGGING_ID:  
        
//           // #ifdef DISBALE_TEST_SECTION
          
//           result = mso->Tasker(function); 
//           // #endif
//            break;
//       #endif
//       #ifdef D_MODULE_CORE_TIME_ID
//         case D_MODULE_CORE_TIME_ID:       
//           result = mt->Tasker(function); 
//         break;
//       #endif
//       #ifdef D_MODULE_NETWORK_MQTT_ID
//         case D_MODULE_NETWORK_MQTT_ID:    
//           result = mqt->Tasker(function);  
//         break;
//       #endif
//       #ifdef D_MODULE_NETWORK_WEBSERVER_ID
//         case D_MODULE_NETWORK_WEBSERVER_ID:  result = mweb->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CORE_TELEMETRY_ID
//         case D_MODULE_CORE_TELEMETRY_ID:    
//         result = mtel->Tasker(function); 
//         break;
//       #endif
//       #ifdef D_MODULE_CORE_HARDWAREPINS_ID
//         case D_MODULE_CORE_HARDWAREPINS_ID:  
        
//           // #ifdef DISBALE_TEST_SECTION
//           result = mod->Tasker(function); 
          
          
//           // #endif
//           break;
//       #endif
//       #ifdef D_MODULE_CORE_RULES_ID
//         case D_MODULE_CORE_RULES_ID:  
        
//           // #ifdef DISBALE_TEST_SECTION
//           result = mrules->Tasker(function); 
          
          
//           // #endif
//           break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_STATUS_LEDS_ID
//         case D_MODULE_DRIVERS_STATUS_LEDS_ID:       result = mstatus_leds->Tasker(function); break;
//       #endif


//       //Sensors
//       #ifdef D_MODULE_SENSORS_DOOR_ID
//         case D_MODULE_SENSORS_DOOR_ID:       result = mds->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_MOTION_ID
//         case D_MODULE_SENSORS_MOTION_ID: 
//           // result = mms->Tasker(function);
//           // result = mTasks[0]->Tasker(function);

//           for(auto& task:mTasks){
//             task->Tasker(function);
//           }

//           break;
//       #endif
//       #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
//         case D_MODULE_SENSORS_PZEM004T_MODBUS_ID: result = mspm->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
//         case D_MODULE_SENSORS_ULTRASONIC_ID: result = mus->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_DHT_ID
//         case D_MODULE_SENSORS_DHT_ID:       result = msdht->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_BME_ID
//         case D_MODULE_SENSORS_BME_ID:       result = msbme->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_DB18S20_ID
//         case D_MODULE_SENSORS_DB18S20_ID:      result = msdb18->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_INA219_ID
//         case D_MODULE_SENSORS_INA219_ID:      result = msina219->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
//         case D_MODULE_CUSTOM_RADIATORFAN_ID:      result = mrf->Tasker(function); break;
//       #endif
//       #ifdef USE_MODULE_CUSTOM_BLINDS
//         case D_MODULE_CUSTOM_BLINDS_ID:           result = mbbl->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
//         case D_MODULE_CUSTOM_OILFURNACE_ID:       result = mof->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
//         case D_MODULE_CUSTOM_EXERCISEBIKE_ID:     result = meb->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CUSTOM_HEATING_ID
//         case D_MODULE_CUSTOM_HEATING_ID:          result = mh-> Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_RELAY_ID
//         case D_MODULE_DRIVERS_RELAY_ID:    
        
//         result = mry->Tasker(function);

//         //  f<int>();
//         //  mry->f2<int>();
        
//         // result = mry->Tasker(function);
//          break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_SDCARD_ID
//         case D_MODULE_DRIVERS_SDCARD_ID:    
//           result = msdcard->Tasker(function);
//         break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_GPS_ID
//         case D_MODULE_DRIVERS_GPS_ID:    
//           result = mgps->Tasker(function);
//         break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_SERIAL_UART_ID
//         case D_MODULE_DRIVERS_SERIAL_UART_ID:    
//           result = mserial->Tasker(function);
//         break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_SHELLY_DIMMER_ID
//         case D_MODULE_DRIVERS_SHELLY_DIMMER_ID:    
//           result = mshelly_dimmer->Tasker(function);
//         break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_CAMERA_OV2640_ID
//         case D_MODULE_DRIVERS_CAMERA_OV2640_ID:    
//           result = mcamera_ov2640->Tasker(function);
//         break;
//       #endif

//       #ifdef D_MODULE_DRIVERS_PWM_ID
//         case D_MODULE_DRIVERS_PWM_ID:    
        
//         result = mpwm->Tasker(function);

//          break;
//       #endif

//       // Customs to be named "Controllers or Systems"
//       #ifdef D_MODULE_CUSTOM_SONOFF_IFAN_ID
//         case D_MODULE_CUSTOM_SONOFF_IFAN_ID:      result = mifan->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CUSTOM_FAN_ID
//         case D_MODULE_CUSTOM_FAN_ID:      result = mfan->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CUSTOM_SENSORCOLOURS_ID
//         case D_MODULE_CUSTOM_SENSORCOLOURS_ID:      result = msenscol->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CUSTOM_TREADMILL_ID
//         case D_MODULE_CUSTOM_TREADMILL_ID:      result = mtreadmill->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_CONTROLLER_DOORBELL_ID
//         case D_MODULE_CONTROLLER_DOORBELL_ID:   result = mdb->Tasker(function); break;
//       #endif



//       #ifdef D_MSAW_MODULE_ID
//         case D_MSAW_MODULE_ID:             result = msm->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_DISPLAYS_NEXTION_ID
//         case D_MODULE_DISPLAYS_NEXTION_ID:    result = mnext->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_BUTTONS_ID
//         case D_MODULE_SENSORS_BUTTONS_ID:         result =  mbtn->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_SWITCHES_ID
//         case D_MODULE_SENSORS_SWITCHES_ID:        result = mswh->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_ANALOG_ID
//         case D_MODULE_SENSORS_ANALOG_ID:        result = msanalog->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
//         case D_MODULE_SENSORS_PULSECOUNTER_ID:    result = mpc->Tasker(function); break;
//       #endif
//       //Drivers
//       #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
//         case D_MODULE_DRIVERS_IRTRANSCEIVER_ID:   result = mir->Tasker(function);  break;
//       #endif
//       #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
//         case D_MODULE_DRIVERS_HBRIDGE_ID:         result = mdhb->Tasker(function); break;
//       #endif


//       /**
//        * Lights
//        * */
//       #ifdef D_MODULE_LIGHTS_INTERFACE_ID
//         case D_MODULE_LIGHTS_INTERFACE_ID:       result = minterface_light->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_LIGHTS_ANIMATOR_ID
//         case D_MODULE_LIGHTS_ANIMATOR_ID:     result = mlights_animator->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
//         case D_MODULE_LIGHTS_ADDRESSABLE_ID:     result = mlights_addressable->Tasker(function); break;
//       #endif
//       #ifdef D_MODULE_LIGHTS_PWM_ID
//         case D_MODULE_LIGHTS_PWM_ID:     result = mlights_pwm->Tasker(function); break;
//       #endif  
//       #ifdef D_MODULE_LIGHTS_WLED_EFFECTS_ID
//         case D_MODULE_LIGHTS_WLED_EFFECTS_ID:     result = mlights_wled->Tasker(function); break;
//       #endif  



//       #ifdef D_MODULE_DRIVERS_ENERGY_ID
//         case D_MODULE_DRIVERS_ENERGY_ID:   result = mdenergy->Tasker(function); break;
//       #endif
//       //default
//       default:
//         fModule_present = false;
//         // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "default"));
//       break;
//     } //end switch

//     #if defined(DEBUG_EXECUTION_TIME)  || defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
//     // Remember start millis
//     uint32_t end_millis = millis();

//     // Get this execution time 
//     uint32_t this_millis = end_millis - start_millis;
    
//     #if defined(DEBUG_EXECUTION_TIME)
//     // Get average
//     if(fModule_present){ //only update tasks that run .. IMPROVE this later with flags (manually) or via returns of tasks
//       module_settings.execution_time_average_ms[i] += this_millis;
//       module_settings.execution_time_average_ms[i] /= 2; //gets average

//       // Get max
//       if(this_millis > module_settings.execution_time_max_ms[i]){
//         module_settings.execution_time_max_ms[i] = this_millis; // remember max
//       }
//     }
//     #endif // DEBUG_EXECUTION_TIME
//     #endif
    
//     #ifdef ENABLE_ADVANCED_DEBUGGING
//       AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " module completed \t%d ms %s"),millis()-start_millis, GetTaskName(function, buffer_taskname));
//     #endif
//     #if defined(ENABLE_DEVFEATURE_SERIAL_PRINT_LONG_LOOP_TASKERS)
//       if(this_millis > 500){
//         AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%d ms %s %S"),millis()-start_millis, GetTaskName(function, buffer_taskname), GetModuleFriendlyName(switch_index));
//       }
//     #endif

//     // print boot info as it proceeds
//     // if(function == FUNC_INIT){
//     //   AddLog_P(LOG_LEVEL_INFO, PSTR("\t\tBootProgress: %s %d %%"),GetModuleFriendlyName(switch_index),map(i,0,module_settings.count,0,100));
//     // }

//     if(target_tasker!=0){
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker EXITING EARLY"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//       break; //only run for loop for the class set. if 0, rull all
//     }
//     // Special flag that can be set to end interface ie event handled, no need to check others
//     if(fExitTaskerWithCompletion){fExitTaskerWithCompletion=false;
//     #ifdef ENABLE_LOG_LEVEL_INFO
//       AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
//     #endif// ENABLE_LOG_LEVEL_INFO
//       break; //only run for loop for the class set. if 0, rull all
//     }
    
//   DEBUG_LINE;

//   } //end for

//   DEBUG_LINE;
//   if(!mset->flag_boot_complete){
//     char buffer_taskname[50];
//     if(function != last_function){
//       uint8_t boot_percentage = map(function,0,FUNC_ON_SUCCESSFUL_BOOT,0,100);
//       // Optional progress bar using block symbols?
//       //5% = 1 bar, 20 bars total [B                   ]
//       //if(pCONT_set->Settings.seriallog_level >= LOG_LEVEL_INFO){
//       #ifndef DISABLE_SERIAL_LOGGING
//       Serial.printf("[");
//       for(uint8_t percent=0;percent<100;percent+=5){  
//         if(percent<boot_percentage){ Serial.print((char)219); }else{ Serial.printf(" "); }
//       }
//       Serial.printf("] %03d %s\n\r",boot_percentage,GetTaskName(function, buffer_taskname));
//       #endif
//       //}
//       last_function = function;
//     }
//     if(function == FUNC_ON_SUCCESSFUL_BOOT){ mset->flag_boot_complete = true; }
//   }//flag_boot_complete

  
//   DEBUG_LINE;
//   #ifdef ENABLE_ADVANCED_DEBUGGING
//     AddLog_P(LOG_LEVEL_TEST,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " FINISHED"));
//   #endif

//   DEBUG_LINE;
//   return result;

// }


// //ifdef debug only include then
// uint16_t mTaskerManager::GetClassSizeByID(uint8_t class_id){
    
//   switch(class_id){
//     // CoreSystem
//     #ifdef D_MODULE_NETWORK_WIFI_ID
//       case D_MODULE_NETWORK_WIFI_ID:  return sizeof(mWiFi);
//     #endif
//     #ifdef D_MODULE_CORE_SETTINGS_ID
//       case D_MODULE_CORE_SETTINGS_ID: return sizeof(mSettings);
//     #endif
//     #ifdef D_MODULE_CORE_SUPPORT_ID
//       case D_MODULE_CORE_SUPPORT_ID: return sizeof(mSupport);
//     #endif
//     #ifdef D_MODULE_CORE_LOGGING_ID
//       case D_MODULE_CORE_LOGGING_ID: return sizeof(mLogging);
//     #endif
//     #ifdef D_MODULE_CORE_TIME_ID
//       case D_MODULE_CORE_TIME_ID: return sizeof(mTime);
//     #endif
//     #ifdef D_MODULE_NETWORK_MQTT_ID
//       case D_MODULE_NETWORK_MQTT_ID: return sizeof(mMQTT);
//     #endif
//     #ifdef D_MODULE_NETWORK_WEBSERVER_ID
//       case D_MODULE_NETWORK_WEBSERVER_ID:   return sizeof(mWebServer);
//     #endif
//     #ifdef D_MODULE_CORE_TELEMETRY_ID
//       case D_MODULE_CORE_TELEMETRY_ID:     return sizeof(mTelemetry);
//     #endif
//     #ifdef D_MODULE_CORE_HARDWAREPINS_ID
//       case D_MODULE_CORE_HARDWAREPINS_ID:   return sizeof(mHardwarePins);
//     #endif
//     #ifdef D_MODULE_CORE_RULES_ID
//       case D_MODULE_CORE_RULES_ID:   return sizeof(mRuleEngine);
//     #endif
//     #ifdef D_MODULE_DRIVERS_STATUS_LEDS_ID
//       case D_MODULE_DRIVERS_STATUS_LEDS_ID: return sizeof(mStatusLEDs);
//     #endif

//     //Sensors
//     #ifdef D_MODULE_SENSORS_DOOR_ID
//       case D_MODULE_SENSORS_DOOR_ID:  return sizeof(mDoorSensor);
//     #endif
//     #ifdef D_MODULE_SENSORS_MOTION_ID
//       case D_MODULE_SENSORS_MOTION_ID: return sizeof(mMotionSensor);
//     #endif
//     #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
//       case D_MODULE_SENSORS_PZEM004T_MODBUS_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
//       case D_MODULE_SENSORS_ULTRASONIC_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_SENSORS_DHT_ID
//       case D_MODULE_SENSORS_DHT_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_SENSORS_BME_ID
//       case D_MODULE_SENSORS_BME_ID: return sizeof(mSensorsBME);
//     #endif
//     #ifdef D_MODULE_SENSORS_DB18S20_ID
//       case D_MODULE_SENSORS_DB18S20_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_SENSORS_INA219_ID
//       case D_MODULE_SENSORS_INA219_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
//       case D_MODULE_CUSTOM_RADIATORFAN_ID: return sizeof();
//     #endif
//     #ifdef USE_MODULE_CUSTOM_BLINDS
//       case D_MODULE_CUSTOM_BLINDS_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
//       case D_MODULE_CUSTOM_OILFURNACE_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
//       case D_MODULE_CUSTOM_EXERCISEBIKE_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_CUSTOM_HEATING_ID
//       case D_MODULE_CUSTOM_HEATING_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_DRIVERS_RELAY_ID
//       case D_MODULE_DRIVERS_RELAY_ID:  return sizeof();
//     #endif
//     #ifdef D_MODULE_DRIVERS_SDCARD_ID
//       case D_MODULE_DRIVERS_SDCARD_ID:   return sizeof();
//     #endif
//     #ifdef D_MODULE_DRIVERS_GPS_ID
//       case D_MODULE_DRIVERS_GPS_ID:     return sizeof();
//     #endif
//     #ifdef D_MODULE_DRIVERS_SERIAL_UART_ID
//       case D_MODULE_DRIVERS_SERIAL_UART_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_DRIVERS_SHELLY_DIMMER_ID
//       case D_MODULE_DRIVERS_SHELLY_DIMMER_ID:  return sizeof();
//     #endif
//     #ifdef D_MODULE_DRIVERS_CAMERA_OV2640_ID
//       case D_MODULE_DRIVERS_CAMERA_OV2640_ID:   return sizeof();
//     #endif

//     #ifdef D_MODULE_DRIVERS_PWM_ID
//       case D_MODULE_DRIVERS_PWM_ID:    return sizeof();
//     #endif

//     // Customs to be named "Controllers or Systems"
//     #ifdef D_MODULE_CUSTOM_SONOFF_IFAN_ID
//       case D_MODULE_CUSTOM_SONOFF_IFAN_ID:   return sizeof();
//     #endif
//     #ifdef D_MODULE_CUSTOM_FAN_ID
//       case D_MODULE_CUSTOM_FAN_ID:   return sizeof();
//     #endif
//     #ifdef D_MODULE_CUSTOM_SENSORCOLOURS_ID
//       case D_MODULE_CUSTOM_SENSORCOLOURS_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_CUSTOM_TREADMILL_ID
//       case D_MODULE_CUSTOM_TREADMILL_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_CONTROLLER_DOORBELL_ID
//       case D_MODULE_CONTROLLER_DOORBELL_ID: return sizeof();
//     #endif



//     #ifdef D_MSAW_MODULE_ID
//       case D_MSAW_MODULE_ID:   return sizeof();
//     #endif
//     #ifdef D_MODULE_DISPLAYS_NEXTION_ID
//       case D_MODULE_DISPLAYS_NEXTION_ID:  return sizeof(mNextionPanel);
//     #endif
//     #ifdef D_MODULE_SENSORS_BUTTONS_ID
//       case D_MODULE_SENSORS_BUTTONS_ID:    return sizeof();
//     #endif
//     #ifdef D_MODULE_SENSORS_SWITCHES_ID
//       case D_MODULE_SENSORS_SWITCHES_ID:   return sizeof();
//     #endif
//     #ifdef D_MODULE_SENSORS_ANALOG_ID
//       case D_MODULE_SENSORS_ANALOG_ID:  return sizeof();
//     #endif
//     #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
//       case D_MODULE_SENSORS_PULSECOUNTER_ID:  return sizeof();
//     #endif
//     //Drivers
//     #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
//       case D_MODULE_DRIVERS_IRTRANSCEIVER_ID: return sizeof();
//     #endif
//     #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
//       case D_MODULE_DRIVERS_HBRIDGE_ID:    return sizeof();
//     #endif


//     /**
//      * Lights
//       * */
//     #ifdef D_MODULE_LIGHTS_INTERFACE_ID
//       case D_MODULE_LIGHTS_INTERFACE_ID:    return sizeof(mInterfaceLight);
//     #endif
//     #ifdef D_MODULE_LIGHTS_ANIMATOR_ID
//       case D_MODULE_LIGHTS_ANIMATOR_ID:    return sizeof(mAnimatorLight);
//     #endif
//     #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
//       case D_MODULE_LIGHTS_ADDRESSABLE_ID:     return sizeof(mAddressableLight);
//     #endif
//     #ifdef D_MODULE_LIGHTS_PWM_ID
//       case D_MODULE_LIGHTS_PWM_ID:    return sizeof(mPWMLight);
//     #endif  
//     #ifdef D_MODULE_LIGHTS_WLED_EFFECTS_ID
//       case D_MODULE_LIGHTS_WLED_EFFECTS_ID:     return sizeof(mWLEDEffects);
//     #endif  



//     #ifdef D_MODULE_DRIVERS_ENERGY_ID
//       case D_MODULE_DRIVERS_ENERGY_ID:  return sizeof();
//     #endif
//     //default
//     default: return -1; 
//   } //end switch

//   return -1;

// }


// // Switch case should be faster than getext progmem
// // Use progmem WITHOUT buffer for speed improvements, should be read as expected progmem and handled that way
// const char* mTaskerManager::GetTaskName(uint8_t task, char* buffer){

//   switch(task){
//     default:
//     case FUNC_POINTER_INIT:                           return PM_FUNC_POINTER_INIT_CTR;
//     case FUNC_TEMPLATE_MODULE_LOAD:                          return PM_FUNC_TEMPLATE_LOAD_CTR;
//     case FUNC_MODULE_INIT:                            return PM_FUNC_MODULE_INIT_CTR;
//     case FUNC_PRE_INIT:                               return PM_FUNC_PRE_INIT_CTR;
//     case FUNC_INIT:                                   return PM_FUNC_INIT_CTR;
//     case FUNC_CONFIGURE_MODULES_FOR_DEVICE:           return PM_FUNC_CONFIGURE_MODULES_FOR_DEVICE_CTR;
//     case FUNC_LOOP:                                   return PM_FUNC_LOOP_CTR;
//     case FUNC_EVERY_50_MSECOND:                       return PM_FUNC_EVERY_50_MSECOND_CTR;
//     case FUNC_EVERY_100_MSECOND:                      return PM_FUNC_EVERY_100_MSECOND_CTR;
//     case FUNC_EVERY_200_MSECOND:                      return PM_FUNC_EVERY_200_MSECOND_CTR;
//     case FUNC_EVERY_250_MSECOND:                      return PM_FUNC_EVERY_250_MSECOND_CTR;
//     case FUNC_EVERY_SECOND:                           return PM_FUNC_EVERY_SECOND_CTR;
//     case FUNC_EVERY_MINUTE:                           return PM_FUNC_EVERY_MINUTE_CTR; 
//     case FUNC_EVERY_HOUR:                             return PM_FUNC_EVERY_HOUR_CTR; 
//     case FUNC_EVERY_MIDNIGHT:                         return PM_FUNC_EVERY_MIDNIGHT_CTR;
//     case FUNC_EVERY_MIDDAY:                           return PM_FUNC_EVERY_MIDDAY_CTR;
//     case FUNC_ON_SUCCESSFUL_BOOT:                     return PM_FUNC_ON_SUCCESSFUL_BOOT_CTR;
//     case FUNC_UPTIME_10_SECONDS:                      return PM_FUNC_UPTIME_10_SECONDS_CTR;
//     case FUNC_UPTIME_1_MINUTES:                       return PM_FUNC_UPTIME_1_MINUTES_CTR;
//     case FUNC_UPTIME_10_MINUTES:                      return PM_FUNC_UPTIME_10_MINUTES_CTR;
//     case FUNC_UPTIME_60_MINUTES:                      return PM_FUNC_UPTIME_60_MINUTES_CTR;
//     // case FUNC_RESTART_SPLASH_INFORMATION:             return PM_FUNC_RESTART_SPLASH_INFORMATION_CTR;
//     // case FUNC_PREP_BEFORE_TELEPERIOD:                 return PM_FUNC_PREP_BEFORE_TELEPERIOD_CTR;
//     case FUNC_JSON_APPEND:                            return PM_FUNC_JSON_APPEND_CTR;
//     case FUNC_SAVE_BEFORE_RESTART:                    return PM_FUNC_SAVE_BEFORE_RESTART_CTR;
//     case FUNC_SETTINGS_DEFAULT:                       return PM_FUNC_SETTINGS_DEFAULT_CTR;
//     case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:    return PM_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR;
//     case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:    return PM_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
//     case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE:       return PM_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
//     case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE:       return PM_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
//     case FUNC_FUNCTION_LAMBDA_INIT:                   return PM_FUNC_FUNCTION_LAMBDA_INIT_CTR;
//     case FUNC_FUNCTION_LAMBDA_LOOP:                   return PM_FUNC_FUNCTION_LAMBDA_LOOP_CTR;
//     // case FUNC_COMMAND:                                return PM_FUNC_COMMAND_CTR;
//     // case FUNC_COMMAND_SENSOR:                         return PM_FUNC_COMMAND_SENSOR_CTR;
//     // case FUNC_COMMAND_DRIVER:                         return PM_FUNC_COMMAND_DRIVER_CTR;
//     // case FUNC_JSON_COMMAND:                           return PM_FUNC_JSON_COMMAND_CTR;
//     // case FUNC_JSON_COMMAND_OBJECT:                    return PM_FUNC_JSON_COMMAND_OBJECT_CTR;
//     case FUNC_WIFI_CONNECTED:                         return PM_FUNC_WIFI_CONNECTED_CTR;
//     case FUNC_WIFI_DISCONNECTED:                      return PM_FUNC_WIFI_DISCONNECTED_CTR;
//     case FUNC_MQTT_SUBSCRIBE:                         return PM_FUNC_MQTT_SUBSCRIBE_CTR;
//     // case FUNC_MQTT_INIT:                              return PM_FUNC_MQTT_INIT_CTR;
//     case FUNC_MQTT_CONNECTED:                         return PM_FUNC_MQTT_CONNECTED_CTR;
//     case FUNC_MQTT_DISCONNECTED:                      return PM_FUNC_MQTT_DISCONNECTED_CTR;
//     case FUNC_MQTT_COMMAND:                           return PM_FUNC_MQTT_COMMAND_CTR;
//     case FUNC_MQTT_SENDER:                            return PM_FUNC_MQTT_SENDER_CTR;
//     case FUNC_MQTT_HANDLERS_RESET:                    return PM_FUNC_MQTT_HANDLERS_RESET_CTR;
//     case FUNC_MQTT_HANDLERS_INIT:                     return PM_FUNC_MQTT_HANDLERS_INIT_CTR;
//     case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:       return PM_FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD_CTR;
//     case FUNC_SET_POWER:                              return PM_FUNC_SET_POWER_CTR;
//     case FUNC_SET_DEVICE_POWER:                       return PM_FUNC_SET_DEVICE_POWER_CTR;
//     case FUNC_SHOW_SENSOR:                            return PM_FUNC_SHOW_SENSOR_CTR;
//     case FUNC_RULES_PROCESS:                          return PM_FUNC_RULES_PROCESS_CTR;
//     case FUNC_SERIAL:                                 return PM_FUNC_SERIAL_CTR;
//     case FUNC_FREE_MEM:                               return PM_FUNC_FREE_MEM_CTR;
//     case FUNC_BUTTON_PRESSED:                         return PM_FUNC_BUTTON_PRESSED_CTR;
//     case FUNC_ENERGY_RESET:                           return PM_FUNC_ENERGY_RESET_CTR;
//     case FUNC_SENSOR_UPDATED:                         return PM_FUNC_SENSOR_UPDATED_CTR;
//     case FUNC_STATUS_MESSAGE_APPEND:                  return PM_FUNC_STATUS_MESSAGE_APPEND_CTR;
//     case FUNC_JSON_HARDWARE_APPEND:                   return PM_FUNC_JSON_HARDWARE_APPEND_CTR;
//     case FUNC_SET_CHANNELS:                           return PM_FUNC_SET_CHANNELS_CTR;
//     case FUNC_WEB_ADD_MAIN_BUTTON:                    return PM_FUNC_WEB_ADD_MAIN_BUTTON_CTR;
//     case FUNC_WEB_ADD_BUTTON:                         return PM_FUNC_WEB_ADD_BUTTON_CTR;
//     case FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS:         return PM_FUNC_WEB_ADD_BUTTON_SYSTEM_SETTINGS_CTR;
//     case FUNC_WEB_ADD_HANDLER:                        return PM_FUNC_WEB_ADD_HANDLER_CTR;
//     case FUNC_WEB_ROOT_SEND_STYLE:                    return PM_FUNC_WEB_ROOT_SEND_STYLE_CTR;
//     case FUNC_WEB_ROOT_SEND_SCRIPT:                   return PM_FUNC_WEB_ROOT_SEND_SCRIPT_CTR;
//     case FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING:return PM_FUNC_WEB_ROOT_SCRIPT_JSON_FETCH_MODULEPARSING_CTR;
//     case FUNC_WEB_ROOT_SEND_BODY:                     return PM_FUNC_WEB_ROOT_SEND_BODY_CTR;
//     case FUNC_WEB_ROOT_SEND_STATUS:                   return PM_FUNC_WEB_ROOT_SEND_STATUS_CTR;
//     case FUNC_WEB_ADD_ROOT_SHOWS:                     return PM_FUNC_WEB_ADD_ROOT_SHOWS_CTR;
//     case FUNC_WEB_ADD_JSON_FETCH_RESULT:              return PM_FUNC_WEB_ADD_JSON_FETCH_RESULT_CTR;
//     case FUNC_WEB_ADD_JSON_DATA_FETCH_URL:            return PM_FUNC_WEB_ADD_JSON_DATA_FETCH_URL_CTR;
//     case FUNC_WEB_ADD_ROOT_TABLE_ROWS:                return PM_FUNC_WEB_ADD_ROOT_TABLE_ROWS_CTR;
//     case FUNC_WEB_ADD_ROOT_SCRIPT:                    return PM_FUNC_WEB_ADD_ROOT_SCRIPT_CTR;
//     case FUNC_WEB_ADD_ROOT_STYLE:                     return PM_FUNC_WEB_ADD_ROOT_STYLE_CTR;
//     // case FUNC_WEB_APPEND_LOADTIME_ROOT_URLS:          return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
//     // case FUNC_WEB_APPEND_LOADTIME_ROOT_RATES:         return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
//     case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:           return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
//     // case FUNC_WEB_APPEND_RUNTIME_ROOT_RATES:          return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
//     case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED: return PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_IFCHANGED_CTR;
//     case FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED:    return PM_FUNC_WEB_APPEND_ROOT_STATUS_TABLE_FORCED_CTR;
//     case FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS:       return PM_FUNC_WEB_APPEND_ROOT_ADD_MAIN_BUTTONS_CTR;
//     case FUNC_WEB_APPEND_ROOT_BUTTONS:                return PM_FUNC_WEB_APPEND_ROOT_BUTTONS_CTR;
//     case FUNC_WEB_PAGEINFORMATION_SEND_MODULE:        return PM_FUNC_WEB_PAGEINFORMATION_SEND_MODULE_CTR;
//     case FUNC_WEB_COMMAND:                            return PM_FUNC_WEB_COMMAND_CTR;
//     case FUNC_CHECK_POINTERS:                         return PM_FUNC_CHECK_POINTERS_CTR;
//     case FUNC_WEB_SYSTEM_INFO:                        return PM_FUNC_WEB_SYSTEM_INFO_CTR;
//     case FUNC_DEBUG_CONFIGURE:                        return PM_FUNC_DEBUG_CONFIGURE_CTR;
//   }

// }


// PGM_P mTaskerManager::GetClassName(uint8_t task){
//   switch(task){
//     // CoreSystem
//     case D_MODULE_CORE_TIME_ID:        return PM_MODULE_CORE_TIME_CTR; 
//     case D_MODULE_NETWORK_MQTT_ID:        return PM_MODULE_NETWORK_MQTT_CTR;
//     case D_MODULE_CORE_LOGGING_ID:   return PM_MODULE_CORE_LOGGING_CTR;
//     case D_MODULE_NETWORK_WIFI_ID:        return PM_MODULE_NETWORK_WIFI_CTR;
//     case D_MODULE_CORE_SETTINGS_ID:    return PM_MODULE_CORE_SETTINGS_CTR;
//     case D_MODULE_CORE_SUPPORT_ID:     return PM_MODULE_CORE_SUPPORT_CTR;
//     case D_MODULE_CORE_HARDWAREPINS_ID:     return PM_MODULE_CORE_HARDWAREPINS_CTR;
//     #ifdef D_MODULE_CORE_RULES_ID
//       case D_MODULE_CORE_RULES_ID:        return PM_MODULE_CORE_RULES_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_STATUS_LEDS_ID
//       case D_MODULE_DRIVERS_STATUS_LEDS_ID:        return PM_MODULE_DRIVERS_STATUS_LEDS_CTR;
//     #endif




//     #ifdef D_MODULE_CUSTOM_HEATING_ID
//       case D_MODULE_CUSTOM_HEATING_ID:         return PM_MODULE_CUSTOM_HEATING_CTR;
//     #endif
//     #ifdef D_MODULE_DISPLAYS_NEXTION_ID
//       case D_MODULE_DISPLAYS_NEXTION_ID:  return D_MODULE_DISPLAYS_NEXTION_CTR;
//     #endif

//     #ifdef D_MODULE_NETWORK_WEBSERVER_ID
//       case D_MODULE_NETWORK_WEBSERVER_ID:        return PM_MODULE_NETWORK_WEBSERVER_CTR;
//     #endif
//     #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
//       case D_MODULE_LIGHTS_ADDRESSABLE_ID:        return PM_MODULE_LIGHTS_ADDRESSABLE_CTR;
//     #endif
//     #ifdef D_MODULE_LIGHTS_INTERFACE_ID
//       case D_MODULE_LIGHTS_INTERFACE_ID:        return PM_MODULE_LIGHTS_INTERFACE_CTR;
//     #endif

//     #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
//       case D_MODULE_CUSTOM_OILFURNACE_ID:        return PM_MODULE_CUSTOM_OILFURNACE_CTR;
//     #endif
//     #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
//       case D_MODULE_CUSTOM_EXERCISEBIKE_ID:        return PM_MODULE_CUSTOM_EXERCISEBIKE_CTR;
//     #endif

//     // Drivers
//     #ifdef D_MODULE_DRIVERS_RELAY_ID
//       case D_MODULE_DRIVERS_RELAY_ID:        return PM_MODULE_DRIVERS_RELAY_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_PWM_ID
//       case D_MODULE_DRIVERS_PWM_ID:        return PM_MODULE_DRIVERS_PWM_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_SDCARD_ID
//       case D_MODULE_DRIVERS_SDCARD_ID:        return PM_MODULE_DRIVERS_SDCARD_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_GPS_ID
//       case D_MODULE_DRIVERS_GPS_ID:        return PM_MODULE_DRIVERS_GPS_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_SERIAL_UART_ID
//       case D_MODULE_DRIVERS_SERIAL_UART_ID:        return PM_MODULE_DRIVERS_SERIAL_UART_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_SHELLY_DIMMER_ID
//       case D_MODULE_DRIVERS_SHELLY_DIMMER_ID:        return PM_MODULE_DRIVERS_SHELLY_DIMMER_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_CAMERA_OV2640_ID
//       case D_MODULE_DRIVERS_CAMERA_OV2640_ID:        return PM_MODULE_DRIVERS_CAMERA_OV2640_CTR;
//     #endif

//     // Custom
//     #ifdef D_MODULE_CUSTOM_SONOFF_IFAN_ID
//       case D_MODULE_CUSTOM_SONOFF_IFAN_ID:        return PM_MODULE_CUSTOM_IFAN_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_CUSTOM_FAN_ID
//       case D_MODULE_CUSTOM_FAN_ID:        return PM_MODULE_CUSTOM_FAN_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_CUSTOM_TREADMILL_ID
//       case D_MODULE_CUSTOM_TREADMILL_ID:        return PM_MODULE_CUSTOM_TREADMILL_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_CUSTOM_SENSORCOLOURS_ID
//       case D_MODULE_CUSTOM_SENSORCOLOURS_ID:        return PM_MODULE_CUSTOM_SENSORCOLOURS_FRIENDLY_CTR;
//     #endif



//     default: return 0;
//   }
// }



// int16_t mTaskerManager::GetModuleIDbyFriendlyName(const char* c){

//   if(c=='\0'){
//     return -1;
//   }
//   #ifdef D_MODULE_DRIVERS_RELAY_ID
//   if(strcasecmp_P(c,PM_MODULE_DRIVERS_RELAY_FRIENDLY_CTR)==0){ return D_MODULE_DRIVERS_RELAY_ID; }
//   #endif // D_MODULE_DRIVERS_RELAY_ID
//   #ifdef D_MODULE_DRIVERS_SHELLY_DIMMER_ID
//   if(strcasecmp_P(c,PM_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR)==0){ return D_MODULE_DRIVERS_SHELLY_DIMMER_ID; }
//   #endif // D_MODULE_DRIVERS_RELAY_ID

//   #ifdef USE_MODULE_SENSORS_SWITCHES
//   if(strcasecmp_P(c,PM_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR)==0){ return D_MODULE_SENSORS_SWITCHES_ID; }
//   #endif //USE_MODULE_SENSORS_SWITCHES
//   #ifdef USE_MODULE_SENSORS_MOTION
//   if(strcasecmp_P(c,PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR)==0){ return D_MODULE_SENSORS_MOTION_ID; }
//   #endif //USE_MODULE_SENSORS_MOTION


//   return -1;
// }




// PGM_P mTaskerManager::GetModuleFriendlyName(uint8_t module_id){

//   switch(module_id){
//     // CoreSystem (Range 0-29)
//     #ifdef D_MODULE_CORE_HARDWAREPINS_ID
//       case D_MODULE_CORE_HARDWAREPINS_ID:     return PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_CORE_SETTINGS_ID
//       case D_MODULE_CORE_SETTINGS_ID:         return PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_CORE_SUPPORT_ID
//       case D_MODULE_CORE_SUPPORT_ID:          return PM_MODULE_CORE_SUPPORT_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_CORE_LOGGING_ID
//       case D_MODULE_CORE_LOGGING_ID:          return PM_MODULE_CORE_LOGGING_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_CORE_TELEMETRY_ID
//       case D_MODULE_CORE_TELEMETRY_ID:        return PM_MODULE_CORE_TELEMETRY_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_CORE_TIME_ID
//       case D_MODULE_CORE_TIME_ID:             return PM_MODULE_CORE_TIME_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_CORE_RULES_ID
//       case D_MODULE_CORE_RULES_ID:        return PM_MODULE_CORE_RULES_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_STATUS_LEDS_ID
//       case D_MODULE_DRIVERS_STATUS_LEDS_ID:        return PM_MODULE_DRIVERS_STATUS_LEDS_FRIENDLY_CTR;
//     #endif
// // Displays (30-39)
//     #ifdef D_MODULE_DISPLAYS_NEXTION_ID
//       case D_MODULE_DISPLAYS_NEXTION_ID:      return D_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR; 
//     #endif
// // Drivers (Range 40-99)
//     #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
//       case D_MODULE_DRIVERS_HBRIDGE_ID:       return D_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_DRIVERS_ENERGY_ID
//       case D_MODULE_DRIVERS_ENERGY_ID:        return PM_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR;     
//     #endif
//     #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
//       case D_MODULE_LIGHTS_ADDRESSABLE_ID:    return PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR;//, sizeof(PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR)); break; 
//     #endif
//     #ifdef D_MODULE_LIGHTS_INTERFACE_ID
//       case D_MODULE_LIGHTS_INTERFACE_ID:      return PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_LIGHTS_ANIMATOR_ID
//       case D_MODULE_LIGHTS_ANIMATOR_ID:      return PM_MODULE_LIGHTS_ANIMATOR_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
//       case D_MODULE_DRIVERS_IRTRANSCEIVER_ID: return PM_MODULE_DRIVERS_IRTRANSCEIVER_FRIENDLY_CTR;  break;
//     #endif
//     #ifdef D_MODULE_DRIVERS_RELAY_ID
//       case D_MODULE_DRIVERS_RELAY_ID:         return PM_MODULE_DRIVERS_RELAY_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_DRIVERS_CAMERA_OV2640_ID
//       case D_MODULE_DRIVERS_CAMERA_OV2640_ID:        return PM_MODULE_DRIVERS_CAMERA_OV2640_FRIENDLY_CTR;
//     #endif
//     #ifdef D_MODULE_DRIVERS_GPS_ID
//       case D_MODULE_DRIVERS_GPS_ID:         return PM_MODULE_DRIVERS_GPS_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_DRIVERS_SERIAL_UART_ID
//       case D_MODULE_DRIVERS_SERIAL_UART_ID:         return PM_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_DRIVERS_SHELLY_DIMMER_ID
//       case D_MODULE_DRIVERS_SHELLY_DIMMER_ID:        return PM_MODULE_DRIVERS_SHELLY_DIMMER_FRIENDLY_CTR;
//     #endif
//     // #ifdef D_MODULE_DRIVERS_SERIAL_UART_ID
//     //   case D_MODULE_DRIVERS_SERIAL_UART_ID:         return PM_MODULE_DRIVERS_SERIAL_UART_FRIENDLY_CTR; 
//     // #endif
//     #ifdef D_MODULE_DRIVERS_PWM_ID
//       case D_MODULE_DRIVERS_PWM_ID:         return PM_MODULE_DRIVERS_PWM_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MSAW_MODULE_ID
//       case D_MSAW_MODULE_ID:
//         return D_MSAW_MODULE_FRIENDLY_CTR; 
//       break;
//     #endif
// // Network (100-119)
//     #ifdef D_MODULE_NETWORK_WIFI_ID
//       case D_MODULE_NETWORK_WIFI_ID:         return PM_MODULE_NETWORK_WIFI_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_NETWORK_MQTT_ID
//       case D_MODULE_NETWORK_MQTT_ID:        return PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_NETWORK_WEBSERVER_ID
//       case D_MODULE_NETWORK_WEBSERVER_ID:        return PM_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR; 
//     #endif
// // Sensors (Range 120-169)
//     #ifdef D_MODULE_SENSORS_BUTTONS_ID
//       case D_MODULE_SENSORS_BUTTONS_ID:
//         return D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR; 
//       break;
//     #endif
//     #ifdef D_MODULE_SENSORS_SWITCHES_ID
//       case D_MODULE_SENSORS_SWITCHES_ID:
//         return D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR; 
//       break;
//     #endif
//     #ifdef D_MODULE_SENSORS_ANALOG_ID
//       case D_MODULE_SENSORS_ANALOG_ID:
//         return PM_MODULE_SENSORS_ANALOG_FRIENDLY_CTR; 
//       break;
//     #endif
//     #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
//       case D_MODULE_SENSORS_PULSECOUNTER_ID:
//         return D_MODULE_SENSORS_PULSECOUNTER_FRIENDLY_CTR; 
//       break;
//     #endif
//     #ifdef D_MODULE_CONTROLLER_DOORBELL_ID
//       case D_MODULE_CONTROLLER_DOORBELL_ID:        return PM_MODULE_CONTROLLER_DOORBELL_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
//       case D_MODULE_SENSORS_PZEM004T_MODBUS_ID:     return PM_MODULE_SENSORS_PZEM004T_MODBUS_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_SENSORS_DHT_ID
//       case D_MODULE_SENSORS_DHT_ID:
//         return PM_MODULE_SENSORS_DHT_FRIENDLY_CTR; 
//       break;
//     #endif
//     #ifdef D_MODULE_SENSORS_BME_ID
//       case D_MODULE_SENSORS_BME_ID:             return PM_MODULE_SENSORS_BME_FRIENDLY_CTR; 
//     #endif    
//     #ifdef D_MODULE_SENSORS_DB18S20_ID
//       case D_MODULE_SENSORS_DB18S20_ID:         return PM_MODULE_SENSORS_DB18S20_FRIENDLY_CTR;  break;
//     #endif
//     #ifdef D_MODULE_SENSORS_INA219_ID
//       case D_MODULE_SENSORS_INA219_ID:         return PM_MODULE_SENSORS_INA219_FRIENDLY_CTR;  break;
//     #endif
//     #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
//       case D_MODULE_SENSORS_ULTRASONIC_ID:      return PM_MODULE_SENSORS_ULTRASONIC_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_SENSORS_MOTION_ID
//       case D_MODULE_SENSORS_MOTION_ID:        return PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_SENSORS_DOOR_ID
//       case D_MODULE_SENSORS_DOOR_ID:        return PM_MODULE_SENSORS_DOOR_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID
//       case D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID:
//         return D_MODULE_SENSORS_RESISTIVE_MOISTURE_FRIENDLY_CTR; 
//       break;
//     #endif
// // Specefic Bespoke Modules (Range 170-189)
//     #ifdef D_MODULE_CUSTOM_BLINDS_ID
//       case D_MODULE_CUSTOM_BLINDS_ID:
//         return D_MODULE_CUSTOM_BLINDS_FRIENDLY_CTR; 
//       break;
//     #endif
//     #ifdef D_MODULE_CUSTOM_HEATING_ID
//       case D_MODULE_CUSTOM_HEATING_ID:
//         return PM_MODULE_CUSTOM_HEATING_FRIENDLY_CTR; 
//       break;
//     #endif
//     #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
//       case D_MODULE_CUSTOM_RADIATORFAN_ID:
//         return D_MODULE_CUSTOM_RADIATORFAN_FRIENDLY_CTR; 
//       break;
//     #endif

//     // Custom
//     #ifdef D_MODULE_CUSTOM_SONOFF_IFAN_ID
//       case D_MODULE_CUSTOM_SONOFF_IFAN_ID:         return PM_MODULE_CUSTOM_IFAN_FRIENDLY_CTR;  
//     #endif
//     #ifdef D_MODULE_CUSTOM_FAN_ID
//       case D_MODULE_CUSTOM_FAN_ID:         return PM_MODULE_CUSTOM_FAN_FRIENDLY_CTR;  
//     #endif
//     #ifdef D_MODULE_CUSTOM_TREADMILL_ID
//       case D_MODULE_CUSTOM_TREADMILL_ID:         return PM_MODULE_CUSTOM_TREADMILL_FRIENDLY_CTR;  
//     #endif
//     #ifdef D_MODULE_CUSTOM_SENSORCOLOURS_ID
//       case D_MODULE_CUSTOM_SENSORCOLOURS_ID:         return PM_MODULE_CUSTOM_SENSORCOLOURS_FRIENDLY_CTR;  
//     #endif



//     #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
//       case D_MODULE_CUSTOM_OILFURNACE_ID:        return PM_MODULE_CUSTOM_OILFURNACE_FRIENDLY_CTR; 
//     #endif
//     #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
//       case D_MODULE_CUSTOM_EXERCISEBIKE_ID:
//         return D_MODULE_CUSTOM_EXERCISEBIKE_FRIENDLY_CTR; 
//       break;
//     #endif
//     default:
//       return PM_SEARCH_NOMATCH;
//     break;
//   }

// }
  

