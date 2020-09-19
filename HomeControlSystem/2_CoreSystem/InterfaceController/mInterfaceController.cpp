#include "2_CoreSystem/InterfaceController/mInterfaceController.h"


// Checks if defined pointers are NOT nullptr and therefore initiated
uint8_t mInterfaceController::Instance_Init(){

  if(mqt  == nullptr){ mqt  = new mMQTT();      }
  if(mod  == nullptr){ mod  = new mHardwarePins(); }
  if(mt   == nullptr){ mt   = new mTime();      }
  if(mset == nullptr){ mset = new mSettings();  }
  if(msup == nullptr){ msup = new mSupport();   }
  if(mwif == nullptr){ mwif = new mWiFi();      }
  if(mweb == nullptr){ mweb = new mWebServer(); }
  if(mso  == nullptr){ mso  = new mLogging(); }
  if(mtel == nullptr){ mtel = new mTelemetry(); }
  
  // Sensors
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    if(mspm == nullptr){ mspm = new mPzem_AC(); }
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    if(msdht == nullptr){ msdht = new mSensorsDHT(); }
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    if(msbme == nullptr){ msbme = new mSensorsBME(); }
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    if(msdb18 == nullptr){ msdb18 = new mSensorsDB18(); }
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    if(mms == nullptr){ mms = new mMotionSensor(); }
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    if(mois == nullptr){ mois = new mMoistureSensor(); }
  #endif
  #ifdef USE_MODULE_SENSORS_DOOR
    if(mds == nullptr){ mds = new mDoorSensor(); }
  #endif
  #ifdef USE_MODULE_SENSORS_DOORCHIME
    if(mdb == nullptr){ mdb = new mDoorBell(); }
  #endif
  #ifdef USE_MODULE_SENSORS_PULSE_COUNTER
    mPulseCounter mpc;
    if(mois == nullptr){ mois = new mMoistureSensor(); }
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
    if(mbtn == nullptr){ mbtn = new mButtons(); }
  #endif
  #ifdef USE_MODULE_SENSORS_SWITCHES
    if(mswh == nullptr){ mswh = new mSwitches(); }
  #endif
  #ifdef USE_MODULE_SENSORS_PZEM004T_MODBUS
    if(mspm == nullptr){ mspm = new mPzem_AC(); }
  #endif

  /**
   * Lights
   * */
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    if(mil == nullptr){ mil = new mInterfaceLight(); }
  #endif  
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    if(mrgbani == nullptr){ mrgbani = new mRGBAnimator();}
  #endif
  #ifdef USE_MODULE_LIGHTS_PWM
    if(mlights_pwm == nullptr){ mlights_pwm = new mPWMLight(); }
  #endif



  // Drivers
  #ifdef USE_SONOFF_IFAN
    if(mifan == nullptr){ mifan = new mSonoffIFan(); }
  #endif



  #if defined(USE_MODULE_CUSTOM_IRTRANSMITTER) || defined (USE_IR_RECEIVER)
    // #ifdef ESP32
  #endif
  #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined(USE_MODULE_DRIVERS_RF433MHZ)
    mSAWTransceive mst;
    mSAWMain msm;
    mSAWProtocol msp;
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    if(mrf == nullptr){ mrf = new mRadiatorFan(); }
  #endif
  #ifdef USE_MODULE_CUSTOM_BLINDS
    if(mbbl == nullptr){ mbbl = new mBlinds(); }
  #endif
  #ifdef USE_MODULE_DRIVERS_HBRIDGE
    if(mdhb == nullptr){ mdhb = new mHBridge(); }
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    if(mus == nullptr){ mus = new mUltraSonicSensor(); }
  #endif
  #ifdef USE_MODULE_CUSTOM_OILFURNACE
    if(mof == nullptr){ mof = new mOilFurnace(); }
  #endif
  #ifdef USE_MODULE_DRIVERS_IRTRANSCEIVER
    if(mir == nullptr){ mir = new mIRtransceiver(); }
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    if(mry == nullptr){ mry = new mRelays(); }
  #endif
  #ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
    if(mrl == nullptr){ mrl = new mGarageLights(); }
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    mNextionPanel mod;
    if(mois == nullptr){ mois = new mMoistureSensor(); }
  #endif
  #ifdef USE_MODULE_DRIVERS_ENERGY
    if(mdenergy == nullptr){ mdenergy = new mEnergy(); }
  #endif

  // Controllers
  #ifdef USE_MODULE_CUSTOM_HEATING
    if(mh == nullptr){ mh = new mHeating(); }
  #endif
  #ifdef USE_MODULE_CUSTOM_EXERCISE_BIKE
    if(meb == nullptr){ meb = new mExerciseBike(); }
  #endif

}

// Gets the class id's and stores in array, used in interface 
uint8_t mInterfaceController::InitClassList(){

DEBUG_LINE;

  memset(&module_settings,0,sizeof(module_settings));

  // Core system
  #ifdef D_MODULE_CORE_TIME_ID
    module_settings.list[module_settings.count++] = D_MODULE_CORE_TIME_ID;
  #endif
  #ifdef D_MODULE_NETWORK_MQTT_ID
    module_settings.list[module_settings.count++] = D_MODULE_NETWORK_MQTT_ID;
  #endif
  #ifdef D_MODULE_CORE_LOGGING_ID
    module_settings.list[module_settings.count++] = D_MODULE_CORE_LOGGING_ID;
  #endif
  #ifdef D_MODULE_NETWORK_WIFI_ID
    module_settings.list[module_settings.count++] = D_MODULE_NETWORK_WIFI_ID;
  #endif
  #ifdef D_MODULE_CORE_SETTINGS_ID
    module_settings.list[module_settings.count++] = D_MODULE_CORE_SETTINGS_ID;
  #endif
  #ifdef D_MODULE_CORE_SUPPORT_ID
    module_settings.list[module_settings.count++] = D_MODULE_CORE_SUPPORT_ID;
  #endif
  #ifdef D_MODULE_NETWORK_WEBSERVER_ID
    module_settings.list[module_settings.count++] = D_MODULE_NETWORK_WEBSERVER_ID;
  #endif
  #ifdef D_MODULE_CORE_TELEMETRY_ID
    module_settings.list[module_settings.count++] = D_MODULE_CORE_TELEMETRY_ID;
  #endif
  #ifdef D_MODULE_SENSORS_BUTTONS_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_BUTTONS_ID;
  #endif
  #ifdef D_MODULE_SENSORS_SWITCHES_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_SWITCHES_ID;
  #endif
  #ifdef D_MODULE_CORE_HARDWAREPINS_ID
    module_settings.list[module_settings.count++] = D_MODULE_CORE_HARDWAREPINS_ID;
  #endif

  // Sensors
  #ifdef D_MODULE_SENSORS_DHT_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_DHT_ID;
  #endif
  #ifdef D_MODULE_SENSORS_BME_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_BME_ID;
  #endif
  #ifdef D_MODULE_SENSORS_DB18S20_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_DB18S20_ID;
  #endif
  #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_ULTRASONIC_ID;
  #endif
  #ifdef D_MODULE_SENSORS_DOOR_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_DOOR_ID;
  #endif
  #ifdef D_MODULE_SENSORS_DOORBELL_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_DOORBELL_ID;
  #endif
  #ifdef D_MODULE_SENSORS_MOTION_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_MOTION_ID;
  #endif
  #ifdef D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID;
  #endif
  #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_PULSECOUNTER_ID;
  #endif
  #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
    module_settings.list[module_settings.count++] = D_MODULE_SENSORS_PZEM004T_MODBUS_ID;
  #endif

  
  // Wireless
  #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
    module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_IRTRANSCEIVER_ID;
  #endif
  #ifdef mSAWMain_ID
    module_settings.list[module_settings.count++] = mSAWMain_ID;
  #endif
  #ifdef mSAWProtocol_ID
    module_settings.list[module_settings.count++] = mSAWProtocol_ID;
  #endif

  // Drivers
  #ifdef D_MODULE_DRIVERS_RELAY_ID
    module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_RELAY_ID;
  #endif
  #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
    module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_HBRIDGE_ID;
  #endif
  #ifdef D_MODULE_DRIVERS_ENERGY_ID
    module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_ENERGY_ID;
  #endif

  /**
   * Lighting
   * */
  #ifdef D_MODULE_LIGHTS_INTERFACE_ID
    module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_INTERFACE_ID;
  #endif  
  #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
    module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_ADDRESSABLE_ID;
  #endif
  #ifdef D_MODULE_LIGHTS_PWM_ID
    module_settings.list[module_settings.count++] = D_MODULE_LIGHTS_PWM_ID
  #endif  

  // Displays
  #ifdef D_MODULE_DISPLAYS_NEXTION_ID
    module_settings.list[module_settings.count++] = D_MODULE_DISPLAYS_NEXTION_ID;
  #endif
  
  //Specific
  #ifdef D_MODULE_CUSTOM_BLINDS_ID
    module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_BLINDS_ID;
  #endif
  #ifdef D_MODULE_CUSTOM_HEATING_ID
    module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_HEATING_ID;
  #endif
  #ifdef D_MODULE_DRIVERS_IFAN_ID
    module_settings.list[module_settings.count++] = D_MODULE_DRIVERS_IFAN_ID;
  #endif
  #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
    module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_RADIATORFAN_ID;
  #endif
  #ifdef D_MODULE_CUSTOM_SECURITYLIGHT_ID
    module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_SECURITYLIGHT_ID;
  #endif
  #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
    module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_OILFURNACE_ID;
  #endif
  #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
    module_settings.list[module_settings.count++] = D_MODULE_CUSTOM_EXERCISEBIKE_ID;
  #endif
  // #ifdef D_MOILTANK_MODULE_ID
  //   module_settings.list[module_settings.count++] = D_MOILTANK_MODULE_ID;
  // #endif
  
DEBUG_LINE;

}

uint16_t mInterfaceController::GetClassCount(){
  return module_settings.count;
}


/* Null, because instance will be initialized on demand. */
mInterfaceController* mInterfaceController::instance = nullptr;


mInterfaceController* mInterfaceController::GetInstance(){
  if (instance == nullptr){
    instance = new mInterfaceController();
  }
  return instance;
}



// Checks if defined pointers are NOT nullptr and therefore initiated
uint8_t mInterfaceController::CheckPointersPass(){

//probably phase out? singletons will remove need


  // system
  if(mset==nullptr){ return false; }
  if(msup==nullptr){ return false; }
  if(mwif==nullptr){ return false; }
  #ifdef USE_MQTT
    if(mqt==nullptr){ return false; }
  #endif
  #ifdef USE_WEBSERVER
    if(mweb==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_LIGHTS_ADDRESSABLE
    if(mrgbani==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_LIGHTS_INTERFACE
    if(mil==nullptr){ return false; }
  #endif

  // Sensor
  #ifdef USE_MODULE_SENSORS_DOOR
    if(mds==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_SENSORS_MOTION
    if(mms==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_SENSORS_DHT
    if(msdht==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_SENSORS_BME
    if(msbme==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_SENSORS_DS18B20
    if(msdb18==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_SENSORS_MOISTURE
    if(mois==nullptr){ return false; }
  #endif



  #ifdef USE_RTC
    if(mt==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_CUSTOM_RADIATORFAN
    if(mrf==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_CUSTOM_BLINDS
    if(mbbl==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_SENSORS_ULTRASONICS
    if(mus==nullptr){ return false; }
  #endif
  // #ifdef USE_OILTANK
  //   if(mot==nullptr){ return false; }
  // #endif
  #ifdef USE_MODULE_CUSTOM_HEATING
    if(mh==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_DRIVERS_RELAY
    if(mry==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_CUSTOM_SECURITY_LIGHT
    if(mrl==nullptr){ return false; }
  #endif
  #if defined(USE_MODULE_DRIVERS_RF433MHZ) || defined(USE_MODULE_DRIVERS_RF433MHZ)
    if(msm==nullptr){ return false; }
  #endif
  #ifdef USE_SONOFF_IFAN
    if(mifan==nullptr){ return false; }
  #endif
  #ifdef USE_MODULE_DISPLAYS_NEXTION
    if(mod==nullptr){ return false; }
  #endif

  // Drivers
  #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
    if(mdhb==nullptr){ return false; }
  #endif
  #ifdef D_MODULE_DRIVERS_ENERGY_ID
    if(mdenergy==nullptr){ return false; }
  #endif

  return true;

}


// to replace interface found in tasmota 
// single function will call ALL cpp files that have a Tasker, passing the command
/**
 * 
 * Default is Tasker_Interface(uint8_t function) with target_tasker = 0. If 0, all classes are called. If !0, a specific tasker will be called and this function will exit after completion
 * */

int8_t mInterfaceController::Tasker_Interface(uint8_t function, uint8_t target_tasker){
//   JsonObjectConst dummy; return Tasker_Interface(function, dummy, target_tasker);
// }
// template<typename T>
// int8_t mInterfaceController::Tasker_Interface(uint8_t function, T param1, uint8_t target_tasker = 0){

  // if(function == FUNC_JSON_COMMAND_OBJECT){
  // Serial.println("BEFORE FUNC_JSON_COMMAND_OBJECT TASKER"); Serial.flush();
  // }

  int8_t result = 0;

  DEBUG_LINE;
  // Serial.printf("Tasker_Interface %d %d\n\r", function, target_tasker); Serial.flush();

  if(target_tasker){
    AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker %d %s"),target_tasker,GetModuleFriendlyName(target_tasker));
  }

  // Serial.printf("Tasker_Interface2 %d %d\n\r", function, target_tasker); Serial.flush();

  // Check system is safe to run
  if(function==FUNC_CHECK_POINTERS){
    // AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%s"),GetTaskName(function));
    if(!CheckPointersPass()){
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST "CheckPointers FAILED!!"));
      return FUNCTION_RESULT_ERROR_POINTER_INVALID_ID;
    }else{
      AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST "CheckPointers PASSED"));
      return FUNCTION_RESULT_SUCCESS_ID;
    }
  }else
  if(function==FUNC_PRE_INIT){
    InitClassList();
    AddLog_P(LOG_LEVEL_INFO,PSTR(D_LOG_CLASSLIST "FUNC_PRE_INIT  GetClassCount %d"),module_settings.count);
  }

  DEBUG_LINE;

  uint8_t fModule_present = false;
  #ifdef ENABLE_ADVANCED_DEBUGGING
    char buffer_taskname[50];
  #endif

  for(uint8_t i=0;i<module_settings.count;i++){
    // If target_tasker != 0, then use it, else, use indexed array

    DEBUG_LINE;
    switch_index = target_tasker ? target_tasker : module_settings.list[i];
    // #ifdef ENABLE_ADVANCED_DEBUGGING
    //   AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%02d %s\t%S"),
    //     switch_index, 
    //     GetTaskName(function, buffer_taskname),
    //     GetModuleFriendlyName(switch_index));
    // #endif
    
    // char buffer_taskname[40];
    //   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%02d %s\t%S"),
    //     switch_index, 
    //     GetTaskName(function, buffer_taskname),
    //     GetModuleFriendlyName(switch_index));
    // DEBUG_LINE;

    // Remember start millis
    #if defined(DEBUG_EXECUTION_TIME) || defined(ENABLE_ADVANCED_DEBUGGING)
    uint32_t start_millis = millis();
    #endif
    fModule_present = true; //assume true
    
    switch(switch_index){
      // CoreSystem
      #ifdef D_MODULE_NETWORK_WIFI_ID
        case D_MODULE_NETWORK_WIFI_ID:       result = mwif->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CORE_SETTINGS_ID
        case D_MODULE_CORE_SETTINGS_ID:   
        
        {
// Serial.println("D_MODULE_CORE_SETTINGS_ID");
          //  char test;
          //uint8_t test;
        
         result = mset->Tasker(function);// param1); 

        // result = mrgbani->Tasker(function, param1); 

        // mset->Tasker(function, test);
        

        }
        break;
      #endif
      #ifdef D_MODULE_CORE_SUPPORT_ID
        case D_MODULE_CORE_SUPPORT_ID:    result = msup->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_CORE_LOGGING_ID
        case D_MODULE_CORE_LOGGING_ID:  result = msup->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_CORE_TIME_ID
        case D_MODULE_CORE_TIME_ID:       result = mt->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_NETWORK_MQTT_ID
        case D_MODULE_NETWORK_MQTT_ID:       result = mqt->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_NETWORK_WEBSERVER_ID
        case D_MODULE_NETWORK_WEBSERVER_ID:  result = mweb->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CORE_TELEMETRY_ID
      case D_MODULE_CORE_TELEMETRY_ID:    result = mtel->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CORE_HARDWAREPINS_ID
        case D_MODULE_CORE_HARDWAREPINS_ID:  result = mod->Tasker(function); break;
      #endif
      //Sensors
      #ifdef D_MODULE_SENSORS_DOOR_ID
        case D_MODULE_SENSORS_DOOR_ID:       result = mds->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_DOORBELL_ID
        case D_MODULE_SENSORS_DOORBELL_ID:   result = mdb->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_MOTION_ID
        case D_MODULE_SENSORS_MOTION_ID:     result = mms->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
        case D_MODULE_SENSORS_PZEM004T_MODBUS_ID: result = mspm->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
        case D_MODULE_SENSORS_ULTRASONIC_ID: result = mus->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_DHT_ID
        case D_MODULE_SENSORS_DHT_ID:       result = msdht->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_BME_ID
        case D_MODULE_SENSORS_BME_ID:       result = msbme->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_DB18S20_ID
        case D_MODULE_SENSORS_DB18S20_ID:      result = msdb18->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
        case D_MODULE_CUSTOM_RADIATORFAN_ID:      result = mrf->Tasker(function); break;
      #endif
      #ifdef USE_MODULE_CUSTOM_BLINDS
        case D_MODULE_CUSTOM_BLINDS_ID:           result = mbbl->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
        case D_MODULE_CUSTOM_OILFURNACE_ID:       result = mof->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
        case D_MODULE_CUSTOM_EXERCISEBIKE_ID:     result = meb->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_HEATING_ID
        case D_MODULE_CUSTOM_HEATING_ID:          result = mh-> Tasker(function); break;
      #endif
      #ifdef D_MODULE_DRIVERS_RELAY_ID
        case D_MODULE_DRIVERS_RELAY_ID:    
        
        result = mry->Tasker(function);

        //  f<int>();
        //  mry->f2<int>();
        
        // result = mry->Tasker(function);
         break;
      #endif
      #ifdef D_MODULE_CUSTOM_SECURITYLIGHT_ID
        case D_MODULE_CUSTOM_SECURITYLIGHT_ID:     result = mrl->Tasker(function); break;
      #endif
      #ifdef D_MODULE_DRIVERS_IFAN_ID
        case D_MODULE_DRIVERS_IFAN_ID:      result = mifan->Tasker(function); break;
      #endif
      #ifdef D_MSAW_MODULE_ID
        case D_MSAW_MODULE_ID:             result = msm->Tasker(function); break;
      #endif
      #ifdef D_MODULE_DISPLAYS_NEXTION_ID
        case D_MODULE_DISPLAYS_NEXTION_ID:    result = mod->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_BUTTONS_ID
        case D_MODULE_SENSORS_BUTTONS_ID:         result =  mbtn->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_SWITCHES_ID
        case D_MODULE_SENSORS_SWITCHES_ID:        result = mswh->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
        case D_MODULE_SENSORS_PULSECOUNTER_ID:    result = mpc->Tasker(function); break;
      #endif
      //Drivers
      #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
        case D_MODULE_DRIVERS_IRTRANSCEIVER_ID:   result = mir->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
        case D_MODULE_DRIVERS_HBRIDGE_ID:         result = mdhb->Tasker(function); break;
      #endif


      /**
       * Lights
       * */
      #ifdef D_MODULE_LIGHTS_INTERFACE_ID
        case D_MODULE_LIGHTS_INTERFACE_ID:       result = mil->Tasker(function); break;
      #endif
      #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
        case D_MODULE_LIGHTS_ADDRESSABLE_ID:     result = mrgbani->Tasker(function); break;
      #endif
      #ifdef D_MODULE_LIGHTS_PWM_ID
        case D_MODULE_LIGHTS_PWM_ID:     result = mlights_pwm->Tasker(function); break;
      #endif  



      #ifdef D_MODULE_DRIVERS_ENERGY_ID
        case D_MODULE_DRIVERS_ENERGY_ID:   result = mdenergy->Tasker(function); break;
      #endif
      //default
      default:
        fModule_present = false;
        // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "default"));
      break;
    } //end switch

    #ifdef DEBUG_EXECUTION_TIME
    // Remember start millis
    uint32_t end_millis = millis();

    // Get this execution time 
    uint32_t this_millis = end_millis - start_millis;

    // Get average
    if(fModule_present){ //only update tasks that run .. IMPROVE this later with flags (manually) or via returns of tasks
      module_settings.execution_time_average_ms[i] += this_millis;
      module_settings.execution_time_average_ms[i] /= 2; //gets average

      // Get max
      if(this_millis > module_settings.execution_time_max_ms[i]){
        module_settings.execution_time_max_ms[i] = this_millis; // remember max
      }
    }
    #endif
    
    #ifdef ENABLE_ADVANCED_DEBUGGING
      AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " module completed \t%d ms"),millis()-start_millis);
    #endif

    if(target_tasker!=0){
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker EXITING EARLY"));
      break; //only run for loop for the class set. if 0, rull all
    }
    // Special flag that can be set to end interface ie event handled, no need to check others
    if(fExitTaskerWithCompletion){fExitTaskerWithCompletion=false;
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
      break; //only run for loop for the class set. if 0, rull all
    }
    
  DEBUG_LINE;

  } //end for

  // yield();
  
  DEBUG_LINE;
  #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " FINISHED"));
  #endif

  DEBUG_LINE;
  return result;

}


// JsonObjectConst  basic 
int8_t mInterfaceController::Tasker_Interface(uint8_t function, JsonObjectConst param1, uint8_t target_tasker){

  int8_t result = 0;
  uint8_t fModule_present = false;
  for(uint8_t i=0;i<module_settings.count;i++){
    // If target_tasker != 0, then use it, else, use indexed array

    DEBUG_LINE;
    switch_index = target_tasker ? target_tasker : module_settings.list[i];
    // // #ifdef ENABLE_ADVANCED_DEBUGGING
    // char buffer_taskname[40];
    //   AddLog_P(LOG_LEVEL_DEBUG,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE "%02d %s\t%S"),
    //     switch_index, 
    //     GetTaskName(function, buffer_taskname),
    //     GetModuleFriendlyName(switch_index));
    // #endif
    DEBUG_LINE;

    // Remember start millis
    #if defined(DEBUG_EXECUTION_TIME) || defined(ENABLE_ADVANCED_DEBUGGING)
    uint32_t start_millis = millis();
    #endif
    fModule_present = true; //assume true
    
    switch(switch_index){
      // CoreSystem
      #ifdef D_MODULE_NETWORK_WIFI_ID
        case D_MODULE_NETWORK_WIFI_ID:       result = mwif->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CORE_SETTINGS_ID
        case D_MODULE_CORE_SETTINGS_ID:{

// Serial.println("D_MODULE_CORE_SETTINGS_ID");
          //  char test;
          //uint8_t test;
        
         result = mset->Tasker(function, param1); 

        // result = mrgbani->Tasker(function, param1); 

        // mset->Tasker(function, test);
        

        }
        break;
      #endif
      #ifdef D_MODULE_CORE_SUPPORT_ID
        case D_MODULE_CORE_SUPPORT_ID:    result = msup->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_CORE_LOGGING_ID
        case D_MODULE_CORE_LOGGING_ID:  result = msup->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_CORE_TIME_ID
        case D_MODULE_CORE_TIME_ID:       result = mt->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_NETWORK_MQTT_ID
        case D_MODULE_NETWORK_MQTT_ID:       result = mqt->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_NETWORK_WEBSERVER_ID
        case D_MODULE_NETWORK_WEBSERVER_ID:  result = mweb->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CORE_TELEMETRY_ID
      case D_MODULE_CORE_TELEMETRY_ID:    result = mtel->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CORE_HARDWAREPINS_ID
        case D_MODULE_CORE_HARDWAREPINS_ID:  result = mod->Tasker(function); break;
      #endif
      //Sensors
      #ifdef D_MODULE_SENSORS_DOOR_ID
        case D_MODULE_SENSORS_DOOR_ID:       result = mds->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_DOORBELL_ID
        case D_MODULE_SENSORS_DOORBELL_ID:   result = mdb->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_MOTION_ID
        case D_MODULE_SENSORS_MOTION_ID:     result = mms->Tasker(function, param1); break;
      #endif
      #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
        case D_MODULE_SENSORS_PZEM004T_MODBUS_ID: result = mspm->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
        case D_MODULE_SENSORS_ULTRASONIC_ID: result = mus->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_DHT_ID
        case D_MODULE_SENSORS_DHT_ID:       result = msdht->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_BME_ID
        case D_MODULE_SENSORS_BME_ID:       result = msbme->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_DB18S20_ID
        case D_MODULE_SENSORS_DB18S20_ID:      result = msdb18->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
        case D_MODULE_CUSTOM_RADIATORFAN_ID:      result = mrf->Tasker(function); break;
      #endif
      #ifdef USE_MODULE_CUSTOM_BLINDS
        case D_MODULE_CUSTOM_BLINDS_ID:           result = mbbl->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
        case D_MODULE_CUSTOM_OILFURNACE_ID:       result = mof->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
        case D_MODULE_CUSTOM_EXERCISEBIKE_ID:     result = meb->Tasker(function); break;
      #endif
      #ifdef D_MODULE_CUSTOM_HEATING_ID
        case D_MODULE_CUSTOM_HEATING_ID:          result = mh-> Tasker(function, param1); break;
      #endif
      #ifdef D_MODULE_DRIVERS_RELAY_ID
        case D_MODULE_DRIVERS_RELAY_ID:           result = mry->Tasker(function,param1);      break;
      #endif
      #ifdef D_MODULE_CUSTOM_SECURITYLIGHT_ID
        case D_MODULE_CUSTOM_SECURITYLIGHT_ID:     result = mrl->Tasker(function); break;
      #endif
      #ifdef D_MODULE_DRIVERS_IFAN_ID
        case D_MODULE_DRIVERS_IFAN_ID:      result = mifan->Tasker(function, param1); break;
      #endif
      #ifdef D_MSAW_MODULE_ID
        case D_MSAW_MODULE_ID:             result = msm->Tasker(function); break;
      #endif
      #ifdef D_MODULE_DISPLAYS_NEXTION_ID
        case D_MODULE_DISPLAYS_NEXTION_ID:    result = mod->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_BUTTONS_ID
        case D_MODULE_SENSORS_BUTTONS_ID:         result =  mbtn->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_SWITCHES_ID
        case D_MODULE_SENSORS_SWITCHES_ID:        result = mswh->Tasker(function); break;
      #endif
      #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
        case D_MODULE_SENSORS_PULSECOUNTER_ID:    result = mpc->Tasker(function); break;
      #endif
      //Drivers
      #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
        case D_MODULE_DRIVERS_IRTRANSCEIVER_ID:   result = mir->Tasker(function);  break;
      #endif
      #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
        case D_MODULE_DRIVERS_HBRIDGE_ID:         result = mdhb->Tasker(function); break;
      #endif
      #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
        case D_MODULE_LIGHTS_ADDRESSABLE_ID:        result = mrgbani->Tasker(function, param1); 
        break;
      #endif
      #ifdef D_MODULE_LIGHTS_PWM_ID
        case D_MODULE_LIGHTS_PWM_ID:     //result = mlights_pwm->Tasker(function, param1); 
        break;
      #endif  


      #ifdef D_MODULE_LIGHTS_INTERFACE_ID
        case D_MODULE_LIGHTS_INTERFACE_ID:  result = mil->Tasker(function, param1); break;
      #endif
      #ifdef D_MODULE_DRIVERS_ENERGY_ID
        case D_MODULE_DRIVERS_ENERGY_ID:   result = mdenergy->Tasker(function); break;
      #endif
      //default
      default:
        fModule_present = false;
        // AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "default"));
      break;
    } //end switch

    #ifdef DEBUG_EXECUTION_TIME
    // Remember start millis
    uint32_t end_millis = millis();

    // Get this execution time 
    uint32_t this_millis = end_millis - start_millis;

    // Get average
    if(fModule_present){ //only update tasks that run .. IMPROVE this later with flags (manually) or via returns of tasks
      module_settings.execution_time_average_ms[i] += this_millis;
      module_settings.execution_time_average_ms[i] /= 2; //gets average

      // Get max
      if(this_millis > module_settings.execution_time_max_ms[i]){
        module_settings.execution_time_max_ms[i] = this_millis; // remember max
      }
    }
    #endif
    
    #ifdef ENABLE_ADVANCED_DEBUGGING
      AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " module completed \t%d ms"),millis()-start_millis);
    #endif

    if(target_tasker!=0){
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "target_tasker EXITING EARLY"));
      break; //only run for loop for the class set. if 0, rull all
    }
    // Special flag that can be set to end interface ie event handled, no need to check others
    if(fExitTaskerWithCompletion){fExitTaskerWithCompletion=false;
      AddLog_P(LOG_LEVEL_DEBUG_MORE,PSTR(D_LOG_CLASSLIST "fExitTaskerWithCompletion EXITING EARLY"));
      break; //only run for loop for the class set. if 0, rull all
    }
    
  DEBUG_LINE;

  } //end for

  // yield();
  
  DEBUG_LINE;
  #ifdef ENABLE_ADVANCED_DEBUGGING
    AddLog_P(LOG_LEVEL_DEBUG_LOWLEVEL,PSTR(D_LOG_CLASSLIST D_FUNCTION_TASKER_INTERFACE " FINISHED"));
  #endif

  DEBUG_LINE;
  return result;

}


// Switch case should be faster than getext progmem
// Use progmem WITHOUT buffer for speed improvements, should be read as expected progmem and handled that way
const char* mInterfaceController::GetTaskName(uint8_t task, char* buffer){

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
    case FUNC_RESTART_SPLASH_INFORMATION:             return PM_FUNC_RESTART_SPLASH_INFORMATION_CTR;
    case FUNC_PREP_BEFORE_TELEPERIOD:                 return PM_FUNC_PREP_BEFORE_TELEPERIOD_CTR;
    case FUNC_JSON_APPEND:                            return PM_FUNC_JSON_APPEND_CTR;
    case FUNC_SAVE_BEFORE_RESTART:                    return PM_FUNC_SAVE_BEFORE_RESTART_CTR;
    case FUNC_SETTINGS_DEFAULT:                       return PM_FUNC_SETTINGS_DEFAULT_CTR;
    case FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES:    return PM_FUNC_SETTINGS_PRELOAD_DEFAULT_IN_MODULES_CTR;
    case FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT:    return PM_FUNC_SETTINGS_OVERWRITE_SAVED_TO_DEFAULT_CTR;
    case FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE:       return PM_FUNC_SETTINGS_LOAD_VALUES_INTO_MODULE_CTR;
    case FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE:       return PM_FUNC_SETTINGS_SAVE_VALUES_FROM_MODULE_CTR;
    case FUNC_FUNCTION_LAMBDA_INIT:                   return PM_FUNC_FUNCTION_LAMBDA_INIT_CTR;
    case FUNC_FUNCTION_LAMBDA_LOOP:                   return PM_FUNC_FUNCTION_LAMBDA_LOOP_CTR;
    case FUNC_COMMAND:                                return PM_FUNC_COMMAND_CTR;
    case FUNC_COMMAND_SENSOR:                         return PM_FUNC_COMMAND_SENSOR_CTR;
    case FUNC_COMMAND_DRIVER:                         return PM_FUNC_COMMAND_DRIVER_CTR;
    case FUNC_JSON_COMMAND:                           return PM_FUNC_JSON_COMMAND_CTR;
    case FUNC_JSON_COMMAND_OBJECT:                    return PM_FUNC_JSON_COMMAND_OBJECT_CTR;
    case FUNC_WIFI_CONNECTED:                         return PM_FUNC_WIFI_CONNECTED_CTR;
    case FUNC_WIFI_DISCONNECTED:                      return PM_FUNC_WIFI_DISCONNECTED_CTR;
    case FUNC_MQTT_SUBSCRIBE:                         return PM_FUNC_MQTT_SUBSCRIBE_CTR;
    case FUNC_MQTT_INIT:                              return PM_FUNC_MQTT_INIT_CTR;
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
    case FUNC_WEB_APPEND_LOADTIME_ROOT_URLS:          return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_URLS_CTR;
    case FUNC_WEB_APPEND_LOADTIME_ROOT_RATES:         return PM_FUNC_WEB_APPEND_LOADTIME_ROOT_RATES_CTR;
    case FUNC_WEB_APPEND_RUNTIME_ROOT_URLS:           return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_URLS_CTR;
    case FUNC_WEB_APPEND_RUNTIME_ROOT_RATES:          return PM_FUNC_WEB_APPEND_RUNTIME_ROOT_RATES_CTR;
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


PGM_P mInterfaceController::GetClassName(uint8_t task){
  switch(task){
    // CoreSystem
    case D_MODULE_CORE_TIME_ID:        return PM_MODULE_CORE_TIME_CTR; 
    case D_MODULE_NETWORK_MQTT_ID:        return PM_MODULE_NETWORK_MQTT_CTR;
    case D_MODULE_CORE_LOGGING_ID:   return PM_MODULE_CORE_LOGGING_CTR;
    case D_MODULE_NETWORK_WIFI_ID:        return PM_MODULE_NETWORK_WIFI_CTR;
    case D_MODULE_CORE_SETTINGS_ID:    return PM_MODULE_CORE_SETTINGS_CTR;
    case D_MODULE_CORE_SUPPORT_ID:     return PM_MODULE_CORE_SUPPORT_CTR;
    case D_MODULE_CORE_HARDWAREPINS_ID:     return PM_MODULE_CORE_HARDWAREPINS_CTR;
    #ifdef D_MODULE_CUSTOM_HEATING_ID
      case D_MODULE_CUSTOM_HEATING_ID:         return PM_MODULE_CUSTOM_HEATING_CTR;
    #endif
    #ifdef D_MODULE_DISPLAYS_NEXTION_ID
      case D_MODULE_DISPLAYS_NEXTION_ID:  return D_MODULE_DISPLAYS_NEXTION_CTR;
    #endif

    #ifdef D_MODULE_NETWORK_WEBSERVER_ID
      case D_MODULE_NETWORK_WEBSERVER_ID:        return PM_MODULE_NETWORK_WEBSERVER_CTR;
    #endif
    #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
      case D_MODULE_LIGHTS_ADDRESSABLE_ID:        return PM_MODULE_LIGHTS_ADDRESSABLE_CTR;
    #endif
    #ifdef D_MODULE_LIGHTS_INTERFACE_ID
      case D_MODULE_LIGHTS_INTERFACE_ID:        return PM_MODULE_LIGHTS_INTERFACE_CTR;
    #endif

    #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
      case D_MODULE_CUSTOM_OILFURNACE_ID:        return PM_MODULE_CUSTOM_OILFURNACE_CTR;
    #endif
    #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
      case D_MODULE_CUSTOM_EXERCISEBIKE_ID:        return PM_MODULE_CUSTOM_EXERCISEBIKE_CTR;
    #endif

    // Drivers
    #ifdef D_MODULE_DRIVERS_RELAY_ID
      case D_MODULE_DRIVERS_RELAY_ID:        return PM_MODULE_DRIVERS_RELAY_CTR;
    #endif
    #ifdef D_MODULE_DRIVERS_IFAN_ID
      case D_MODULE_DRIVERS_IFAN_ID:        return PM_MODULE_DRIVER_IFAN_FRIENDLY_CTR;
    #endif
    default: return 0;
  }
}






PGM_P mInterfaceController::GetModuleFriendlyName(uint8_t module_id){

  switch(module_id){
    // CoreSystem (Range 0-29)
    #ifdef D_MODULE_CORE_HARDWAREPINS_ID
      case D_MODULE_CORE_HARDWAREPINS_ID:     return PM_MODULE_CORE_HARDWAREPINS_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_CORE_SETTINGS_ID
      case D_MODULE_CORE_SETTINGS_ID:         return PM_MODULE_CORE_SETTINGS_FRIENDLY_CTR;
    #endif
    #ifdef D_MODULE_CORE_SUPPORT_ID
      case D_MODULE_CORE_SUPPORT_ID:          return PM_MODULE_CORE_SUPPORT_FRIENDLY_CTR;
    #endif
    #ifdef D_MODULE_CORE_LOGGING_ID
      case D_MODULE_CORE_LOGGING_ID:          return PM_MODULE_CORE_LOGGING_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_CORE_TELEMETRY_ID
      case D_MODULE_CORE_TELEMETRY_ID:        return PM_MODULE_CORE_TELEMETRY_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_CORE_TIME_ID
      case D_MODULE_CORE_TIME_ID:             return PM_MODULE_CORE_TIME_FRIENDLY_CTR; 
    #endif
// Displays (30-39)
    #ifdef D_MODULE_DISPLAYS_NEXTION_ID
      case D_MODULE_DISPLAYS_NEXTION_ID:      return D_MODULE_DISPLAYS_NEXTION_FRIENDLY_CTR; 
    #endif
// Drivers (Range 40-99)
    #ifdef D_MODULE_DRIVERS_HBRIDGE_ID
      case D_MODULE_DRIVERS_HBRIDGE_ID:       return D_MODULE_DRIVERS_HBRIDGE_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_DRIVERS_ENERGY_ID
      case D_MODULE_DRIVERS_ENERGY_ID:        return PM_INTERFACE_ENERGY_MODULE_FRIENDLY_CTR;     
    #endif
    #ifdef D_MODULE_LIGHTS_ADDRESSABLE_ID
      case D_MODULE_LIGHTS_ADDRESSABLE_ID:    return PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR;//, sizeof(PM_MODULE_LIGHTS_ADDRESSABLE_FRIENDLY_CTR)); break; 
    #endif
    #ifdef D_MODULE_LIGHTS_INTERFACE_ID
      case D_MODULE_LIGHTS_INTERFACE_ID:      return PM_MODULE_LIGHTS_INTERFACE_FRIENDLY_CTR;
    #endif
    #ifdef D_MODULE_DRIVERS_IRTRANSCEIVER_ID
      case D_MODULE_DRIVERS_IRTRANSCEIVER_ID: return PM_MODULE_DRIVERS_IRTRANSCEIVER_FRIENDLY_CTR;  break;
    #endif
    #ifdef D_MODULE_DRIVERS_RELAY_ID
      case D_MODULE_DRIVERS_RELAY_ID:         return PM_MODULE_DRIVERS_RELAY_FRIENDLY_CTR; 
    #endif
    #ifdef D_MSAW_MODULE_ID
      case D_MSAW_MODULE_ID:
        return D_MSAW_MODULE_FRIENDLY_CTR; 
      break;
    #endif
// Network (100-119)
    #ifdef D_MODULE_NETWORK_WIFI_ID
      case D_MODULE_NETWORK_WIFI_ID:         return PM_MODULE_NETWORK_WIFI_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_NETWORK_MQTT_ID
      case D_MODULE_NETWORK_MQTT_ID:        return PM_MODULE_NETWORK_MQTT_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_NETWORK_WEBSERVER_ID
      case D_MODULE_NETWORK_WEBSERVER_ID:        return PM_MODULE_NETWORK_WEBSERVER_FRIENDLY_CTR; 
    #endif
// Sensors (Range 120-169)
    #ifdef D_MODULE_SENSORS_BUTTONS_ID
      case D_MODULE_SENSORS_BUTTONS_ID:
        return D_MODULE_SENSORS_BUTTONS_FRIENDLY_CTR; 
      break;
    #endif
    #ifdef D_MODULE_SENSORS_SWITCHES_ID
      case D_MODULE_SENSORS_SWITCHES_ID:
      Serial.println("D_MODULE_SENSORS_SWITCHES_ID");
        return D_MODULE_SENSORS_SWITCHES_FRIENDLY_CTR; 
      break;
    #endif
    #ifdef D_MODULE_SENSORS_PULSECOUNTER_ID
      case D_MODULE_SENSORS_PULSECOUNTER_ID:
        return D_MODULE_SENSORS_PULSECOUNTER_FRIENDLY_CTR; 
      break;
    #endif
    #ifdef D_MODULE_SENSORS_DOORBELL_ID
      case D_MODULE_SENSORS_DOORBELL_ID:        return MODULE_SENSORS_DOORBELL_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_SENSORS_PZEM004T_MODBUS_ID
      case D_MODULE_SENSORS_PZEM004T_MODBUS_ID:     return PM_MODULE_SENSORS_PZEM004T_MODBUS_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_SENSORS_DHT_ID
      case D_MODULE_SENSORS_DHT_ID:
        return PM_MODULE_SENSORS_DHT_FRIENDLY_CTR; 
      break;
    #endif
    #ifdef D_MODULE_SENSORS_BME_ID
      case D_MODULE_SENSORS_BME_ID:             return PM_MODULE_SENSORS_BME_FRIENDLY_CTR; 
    #endif    
    #ifdef D_MODULE_SENSORS_DB18S20_ID
      case D_MODULE_SENSORS_DB18S20_ID:         return PM_MODULE_SENSORS_DB18S20_FRIENDLY_CTR;  break;
    #endif
    #ifdef D_MODULE_SENSORS_ULTRASONIC_ID
      case D_MODULE_SENSORS_ULTRASONIC_ID:      return PM_MODULE_SENSORS_ULTRASONIC_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_SENSORS_MOTION_ID
      case D_MODULE_SENSORS_MOTION_ID:        return PM_MODULE_SENSORS_MOTION_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID
      case D_MODULE_SENSORS_RESISTIVE_MOISTURE_ID:
        return D_MODULE_SENSORS_RESISTIVE_MOISTURE_FRIENDLY_CTR; 
      break;
    #endif
// Specefic Bespoke Modules (Range 170-189)
    #ifdef D_MODULE_CUSTOM_BLINDS_ID
      case D_MODULE_CUSTOM_BLINDS_ID:
        return D_MODULE_CUSTOM_BLINDS_FRIENDLY_CTR; 
      break;
    #endif
    #ifdef D_MODULE_CUSTOM_HEATING_ID
      case D_MODULE_CUSTOM_HEATING_ID:
        return PM_MODULE_CUSTOM_HEATING_FRIENDLY_CTR; 
      break;
    #endif
    #ifdef D_MODULE_CUSTOM_SECURITYLIGHT_ID
      case D_MODULE_CUSTOM_SECURITYLIGHT_ID:         return PM_MODULE_CUSTOM_SECURITYLIGHT_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_CUSTOM_RADIATORFAN_ID
      case D_MODULE_CUSTOM_RADIATORFAN_ID:
        return D_MODULE_CUSTOM_RADIATORFAN_FRIENDLY_CTR; 
      break;
    #endif
    #ifdef D_MODULE_DRIVERS_IFAN_ID
      case D_MODULE_DRIVERS_IFAN_ID:         return PM_MODULE_DRIVER_IFAN_FRIENDLY_CTR;  
    #endif
    #ifdef D_MODULE_CUSTOM_OILFURNACE_ID
      case D_MODULE_CUSTOM_OILFURNACE_ID:        return PM_MODULE_CUSTOM_OILFURNACE_FRIENDLY_CTR; 
    #endif
    #ifdef D_MODULE_CUSTOM_EXERCISEBIKE_ID
      case D_MODULE_CUSTOM_EXERCISEBIKE_ID:
        return D_MODULE_CUSTOM_EXERCISEBIKE_FRIENDLY_CTR; 
      break;
    #endif
    default:
      return PM_SEARCH_NOMATCH;
    break;
  }

}
  






