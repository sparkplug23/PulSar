#include "mTreadmill.h"

#ifdef USE_MODULE_CUSTOM_TREADMILL


void mTreadmill::pre_init(){
  
  // #ifndef ENABLE_DEVFEATURE_FANTYPE_IFAN03
  // if(pCONT_pins->PinUsed(GPIO_FAN_PWM1_ID)) {  // not set when 255
  //   pin = pCONT_pins->GetPin(GPIO_FAN_PWM1_ID);
  //   pinMode(pin, OUTPUT);
  //   settings.fEnableModule = true;
  // }
  // #endif
  
  measured.averaging = new AVERAGING_DATA<float>(10);
  measured.averaging->SetBoundaryLimits(0,11);
    measured.millis_last_updated = millis();

}

int8_t mTreadmill::Tasker(uint8_t function){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    pre_init();
  }else
  if(function == FUNC_INIT){
    // init();
  }

  // Only continue in to tasker if module was configured properly
  // if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:    
      measured.mph = GetSpeed_Averaging_Mph();
      EverySecond();
    break;
    case FUNC_EVERY_250_MSECOND:
      // SpeedRefresh();
    break;
    // case FUNC_EVERY_100_MSECOND:
    // break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_CHECK_TOPIC_ID:
      CheckAndExecute_JSONCommands();
    break;
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand();
    break;
    /************
     * MQTT SECTION * 
    *******************/
    case FUNC_MQTT_HANDLERS_INIT:
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender(); //optional pass parameter
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
  }
  
  /************
   * WEBPAGE SECTION * ó
  *******************/
  
  #ifdef USE_MODULE_CORE_WEBSERVER
  return Tasker_Web(function);
  #endif // USE_MODULE_CORE_WEBSERVER

} // END Tasker

void mTreadmill::EverySecond(){

  // Add "deadband" zone which converts near zero to be zero

  // Record mph, convert to mps
  // measured.mph = GetSpeed_Averaging_Mph();//GetSpeed_Mph(1); /// add averaging, which takes in last value and returns new value
  measured.mps = measured.mph * 0.44704; 

  measured.metres_reseting += measured.mps / 1; // already 1 second
  measured.metres_nonreseting += measured.mps / 1; // already 1 second

  if(measured.mph>1){
    measured.reset_seconds_decounter = measured.reset_seconds_reset_period; // reset timer
  }else{
    if(measured.reset_seconds_decounter != 0){
      measured.reset_seconds_decounter--;
    }
  }

  if(measured.reset_seconds_decounter==0){ //reset hit
    measured.metres_reseting = 0;
  }





  // if(measured.mph)

}

// AVERAGING_DATA<float> averaging_data(10);



//needs to be class, so I can wrap all helper functions into it
// template<typename VALUE>
// struct AVERAGING_DATA{
//   VALUE data[100];
//   uint8_t index = 0;
// };
// AVERAGING_DATA<float> speed_averaged;



float mTreadmill::GetSpeed_Averaging_Mph(uint8_t calibration_method, bool averaging, bool ignore_transients){

  float mph = GetSpeed_Mph(1);

  // Update if new value or timeout exceeded to force update
  // if((measured.mph_last != mph)||(abs(millis()-measured.millis_last_updated)>5000)){
  //   measured.averaging->Add(mph);
  //   measured.millis_last_updated = millis();
  // }

  if(
    (mph != 0)
    ||    (abs(millis()-measured.millis_last_updated)>5000)
    ){
    measured.averaging->Add(mph);
    measured.millis_last_updated = millis();

    // mqtthandler_settings_teleperiod.flags.SendNow = true;

  }

  measured.mph_last = mph; // update saved

  return measured.averaging->Mean();


      //  AVERAGING_DATA<float> averaging_data(10);

      //   // for(int i=0;i<10;i++){
      //   //   averaging_data[i] = i;
      //   // }

      //   averaging_data.SetBoundaryLimits(0,300);
      //   bool result = averaging_data.Add(123);
      //   Serial.println(result);
      //   result = averaging_data.Add(456);
      //   Serial.println(result);


      //   // AddLog_Array(LOG_LEVEL_TEST, PSTR("test"), averaging_data[0], 10);

      //   SERIAL_PRINT_ARRAY("averaging_data",averaging_data,10);

      //   Serial.println(averaging_data.Mean());


}

/**
 * function takes in voltage reading, and maps it to speeds. This will assume smoothing is turned on for sensor reading
 * */
float mTreadmill::GetSpeed_Mph(uint8_t calibration_method){

  float volts_mv = pCONT_mina219->sensor[0].bus_voltage_mv;

  float m = 0.0029;
  float c = -0.7562;

  float calibrated_speed_1 = 0;//(volts_mv*m)+c;

  // uint8_t method = 1;
  
  float input_cal[] = {
        0,   496.39, // 0
   563.47,   696.00,  853.54, 1040.53, // 1
   1193.26, 1375.00, 1553.85, 1733.89, // 3
   1952.11, 2079.37, 2302.00, 2588.10, // 5
   2821.03, 3075.73, 3264.11, 3434.84, // 7
   3661.19, 3663.79, 3659.46, 3804.21, // 9
   3926.11, 4037.03, 4190.46, 4376.11, // 11
   4463.47, 4688.82, 4745.37 // 13
  };
  float output_cal[] = {
    0, 0.8, 
    1, 0.5, 2, 2.5, 
    3, 3.5, 4, 4.5, 
    5, 5.5, 6, 6.5, 
    7, 7.5, 8, 8.5, 
    9, 9.5, 10, 10.5, 
    11, 11.5, 12, 12.5, 
    13, 13.5, 14
  };

  switch(calibration_method){
    case 0:{
      calibrated_speed_1 = (volts_mv*m)+c; //linear fit    
    }break;
    case 1:{

      //make into function, pass arrays
      //search for value within a range
      bool isfound = false;
      uint8_t found_lower_index = 0;
      for(int search_idx=0;search_idx<29-1;search_idx++){
        if(IsWithinRange(volts_mv, input_cal[search_idx], input_cal[search_idx+1])){
          isfound = true;
          found_lower_index = search_idx;
          break; // leave for
        }else{
          isfound = false;
        }
      }

      // map i.e. linear range
      if(isfound){
        calibrated_speed_1 = map(volts_mv, 
          input_cal[found_lower_index], input_cal[found_lower_index+1],
          output_cal[found_lower_index], output_cal[found_lower_index+1]        
        );

        //debug, save values to struct
        av.mx_in = volts_mv;
        av.input_cal_l = input_cal[found_lower_index];
        av.input_cal_h = input_cal[found_lower_index+1];
        av.output_cal_l = output_cal[found_lower_index];
        av.output_cal_h = output_cal[found_lower_index+1];
        av.calib = calibrated_speed_1;
        av.isfound = isfound;
        
        // .mx_in = volts_mv;
        // av.input_cal_l;
        // av.input_cal_h;
        // av.output_cal_l;
        // av.output_cal_h;
        // av.calib;
        // av.isfound = true;

      }

      if(!isfound){
        calibrated_speed_1 = (volts_mv*m)+c; //backup linear method
      }



    }break;
    case 2:{

        // use ranges as boundary conditions, if between range, then output is value. Useful for DAC style sensor averaging
    }
    break;
  }



  // float volt_meas[] = {0, 520,    };
  // float mph_meas[]  = {
  //   0, 
  //   0, 0.8, 1, 0.5, 2, 2.5, 3, 3.5, 4, 4.5, 
  //   5, 5.5, 6, 6.5, 7, 7.5, 8, 8.5, 9, 9.5, 
  //   10, 10.5, 11, 11.5, 12, 12.5, 13, 13.5, 14};

  return constrain(calibrated_speed_1,0,20);


}



uint8_t mTreadmill::ConstructJSON_Settings(uint8_t json_method){
  
  JsonBuilderI->Start();
    JsonBuilderI->Add_P("test",0);  
  JsonBuilderI->End();

}

uint8_t mTreadmill::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    // JsonBuilderI->Add_P(D_JSON_LIGHTPOWER, GetLightState());
    // JsonBuilderI->Add_P(D_JSON_FANSPEED, GetFanspeed());  
    // JsonBuilderI->Add_P(D_JSON_FANPWM, set_fan_pwm);  


    JsonBuilderI->Add_P("voltage_raw", pCONT_mina219->sensor[0].bus_voltage_mv);  
    JsonBuilderI->Add_P("voltage_average", pCONT_mina219->sensor_averages[0].bus_voltage_mv);  

    JBI->Level_Start("AveragingStats");
      JBI->Add("last_updated",measured.millis_last_updated);
      JBI->Add("indexes_used",measured.averaging->indexes_used);
      JBI->Add("index",measured.averaging->index);
      JBI->Add("mean",measured.averaging->Mean());
      JBI->Array_Start("values");
      for(int i=0;i<10;i++){
        JBI->Add(measured.averaging->data[i]);
      }
      JBI->Array_End();
    JBI->Level_End();


    
    JsonBuilderI->Add("MPH_Linear", GetSpeed_Mph(0));
    JsonBuilderI->Add("MPH_Mapped", GetSpeed_Mph(1));

    JBI->Level_Start("Measured");
      JBI->Add("MPH", measured.mph);
      JBI->Add("MPS", measured.mps);
      JBI->Add("MetresResetting", measured.metres_reseting);
      JBI->Add("MetresNonResetting", measured.metres_nonreseting);
      JBI->Add("ResetSecondsDecounter", measured.reset_seconds_decounter);
      JBI->Add("ResetSecondsPeriod", measured.reset_seconds_reset_period);
    JBI->Level_End();


    

    JsonBuilderI->Level_Start("calib");
      JBI->Add("mx_in", av.mx_in);
      JBI->Add("input_cal_l", av.input_cal_l);
      JBI->Add("input_cal_h", av.input_cal_h);
      JBI->Add("output_cal_l", av.output_cal_l);
      JBI->Add("output_cal_h", av.output_cal_h);
      JBI->Add("calib", av.calib);
      JBI->Add("isfound", av.isfound);
    JBI->Level_End();



  JsonBuilderI->End();

}




/*********************************************************************************************************************************************
******** MQTT **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

void mTreadmill::MQTTHandler_Init(){

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mTreadmill::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 600; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mTreadmill::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mTreadmill::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"


void mTreadmill::MQTTHandler_Set_fSendNow(){

  mqtthandler_settings_teleperiod.flags.SendNow = true;
  mqtthandler_sensor_ifchanged.flags.SendNow = true;
  mqtthandler_sensor_teleperiod.flags.SendNow = true;

} //end "MQTTHandler_Init"


void mTreadmill::MQTTHandler_Set_TelePeriod(){

  // mqtthandler_settings_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  // mqtthandler_sensor_teleperiod.tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;

} //end "MQTTHandler_Set_TelePeriod"


void mTreadmill::MQTTHandler_Sender(uint8_t mqtt_handler_id){

  uint8_t mqtthandler_list_ids[] = {
    MQTT_HANDLER_SETTINGS_ID, 
    MQTT_HANDLER_SENSOR_IFCHANGED_ID, 
    MQTT_HANDLER_SENSOR_TELEPERIOD_ID
  };
  
  struct handler<mTreadmill>* mqtthandler_list_ptr[] = {
    &mqtthandler_settings_teleperiod,
    &mqtthandler_sensor_ifchanged,
    &mqtthandler_sensor_teleperiod
  };

  pCONT_mqtt->MQTTHandler_Command_Array_Group(*this, D_MODULE_CUSTOM_TREADMILL_ID,
    mqtthandler_list_ptr, mqtthandler_list_ids,
    sizeof(mqtthandler_list_ptr)/sizeof(mqtthandler_list_ptr[0]),
    mqtt_handler_id
  );

}

////////////////////// END OF MQTT /////////////////////////


#endif
