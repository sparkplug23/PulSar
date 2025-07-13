#include "mSensorsInterface.h" 

#ifdef USE_MODULE_SENSORS_INTERFACE 

int8_t mSensorsInterface::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }
  
  if(module_state.mode != ModuleStatus::Running){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
      
      #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
        Update_UnifiedFilteredReadings();
      #endif
    break;  
    case TASK_EVERY_SECOND:{
      // Serial.println(sizeof(sensors_reading_t));
      // Serial.println(tkr_db18->GetSensorReading());
      // Serial.println(tkr_dht->GetSensorReading());
      // Serial.println(tkr_db18->test123());
      // Serial.println(tkr_dht->test123());

      // Make nicer later with json command to enable and time period to show
      if(rt.tTicker_Splash_Sensors_To_Logs-- == 1)
      {
        // Measurement level feedback will be "DebugMore" and show level should be "Debug". "Info" should be reserved for essential stuff not in mqtt
        ALOG_DBM(PSTR(">>> Sensor Readings <<<"));
        pCONT->Tasker_Interface(TASK_SENSOR_SHOW_LATEST_LOGGED_ID);
        rt.tTicker_Splash_Sensors_To_Logs = 30 ; // reset
      }
      
  


      // for(auto& pmod:pCONT->pModule)
      // {
      //   for(int sensor_id=0;sensor_id<pmod->GetSensorCount();sensor_id++)
      //   {
      //     sensors_reading_t val;
      //     pmod->GetSensorReading(&val, sensor_id);
      //     if(val.type[0])
      //     {
      //       ALOG_TST(PSTR("%S %d|%d val.data[%d]=%d"),pmod->GetModuleFriendlyName(), sensor_id, pmod->GetSensorCount(), sensor_id, (int)val.GetValue(SENSOR_TYPE_TEMPERATURE_ID));
      //     }
      //   }
      // }
     
      //   pModule[switch_index]->Tasker(function, obj);

    }break;
    case TASK_EVERY_10_SECONDS:
      pCONT->Tasker_Interface(TASK_SENSOR_SHOW_LATEST_LOGGED_ID);
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case TASK_EVENT_MOTION_STARTED_ID:
    case TASK_EVENT_MOTION_ENDED_ID:
      Broadcast_Event_MotionDetected();
    break; 
    case TASK_EVENT_INPUT_STATE_CHANGED_ID:
      Broadcast_Event_UserInput();
    break;
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case TASK_EVENT_SET_POWER_ID:
    //   RulesEvent_Set_Power();
    // break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case TASK_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Rate();
    break;
    case TASK_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


void mSensorsInterface::Pre_Init(void)
{
  module_state.mode = ModuleStatus::Initialising;
}


void mSensorsInterface::Init(void)
{
  rt.sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  module_state.mode = ModuleStatus::Running;


  #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_OILTANK_ADD
  // AddFilteredSensor(5027, 0, 60, 120);
  // TEMP: Filter for temperature sensor in module 5027, sensor index 0

  AddFilteredSensor(tkr_tof_vl1x->GetModuleUniqueID(), 0, 60, 60, SENSOR_TYPE_DISTANCE_ID, "OilReading1min");
  AddFilteredSensor(tkr_tof_vl1x->GetModuleUniqueID(), 0, 3600, 360, SENSOR_TYPE_DISTANCE_ID, "OilReading1Hr");
  

  // filtername, which allows for OH decoding.
  #endif

  #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING__HVACDESK_HARDCODED_ADD
  // AddFilteredSensor(5027, 0, 60, 120);
  // TEMP: Filter for temperature sensor in module 5027, sensor index 0


  AddFilteredSensor(tkr_db18->GetModuleUniqueID(), 0, 10, 60, SENSOR_TYPE_TEMPERATURE_ID, "DB10Sec");
  AddFilteredSensor(tkr_db18->GetModuleUniqueID(), 0, 60, 60, SENSOR_TYPE_TEMPERATURE_ID, "DB1Min");
  AddFilteredSensor(tkr_bme->GetModuleUniqueID(), 0, 10, 60, SENSOR_TYPE_TEMPERATURE_ID, "BME10Sec");
  #endif



}


void mSensorsInterface::EveryLoop()
{

}


#ifdef USE_MODULE_LIGHTS_INTERFACE
/**
 * @brief Single function that applies colour map to any temperature sensors.
 * In InterfaceSensors, the type of applied map can be selected to be added to unified method directly
 * 
 * @param temperature 
 * @return RgbwwColor 
 */
RgbwwColor mSensorsInterface::GetColourValueUsingMaps_ForUnifiedSensor(float temperature) // flag_unified_sensor_colour_heatmap_type
{

  if(flag_unified_sensor_colour_heatmap_type==1) //adjusted
  {
    return tkr_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);
  }
  else //full colour-
  {
    return tkr_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);
  }

}
#endif // USE_MODULE_LIGHTS_INTERFACE


void mSensorsInterface::Broadcast_Event_MotionDetected()
{

  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_motion_event_ifchanged.flags.SendNow = true;
  Tasker(TASK_MQTT_SENDER);
  #endif // USE_MODULE_NETWORK_MQTT

}

void mSensorsInterface::Broadcast_Event_UserInput()
{
  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_event_input.flags.SendNow = true;
  Tasker(TASK_MQTT_SENDER);
  #endif // USE_MODULE_NETWORK_MQTT
}


/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/


void mSensorsInterface::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["MQTT"].getObject()["ScanSensors"])
	{
		JBI->Start();

		pCONT->Tasker_Interface(TASK_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

		bool ready_to_send = JBI->End();

		if(!ready_to_send)
		{
			// Nothing was found, create new message
			JBI->Start();
				JBI->Add("SensorScan", "No Devices Found");
			ready_to_send = JBI->End();
		}


		if(ready_to_send)
		{			
    	ALOG_TST(PSTR("ScanSensors=\"%s\""), JBI->GetBufferPtr());
			tkr_mqtt->brokers[0]->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		}

	}
    
}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/

uint8_t mSensorsInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_CHANNELCOUNT, 0);
  return JBI->End();

}

/**
 * @brief 
 * 
 * Format with multiple locations? maybe should be flipped and the name added. ie
 * 
 * {"Temperature":{"Master Bedroom":23.580,"Landing":25.020}}
 * {"Humidity":{name:12,other:13} //therefore "for each key"
 * {"lum"..}
 * 
 * Step 1: Using module pointer, request all sensor readings which will be returned in sensors_reading_t
 * Step 2: Go through all sensors_reading_t values, if configured, then append their values to the JSON array with Key based on the module pointer
 * 
 * @param json_method 
 * @return uint8_t 
 */
uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  // if(tkr_time->uptime_seconds_nonreset > 30)
  //   ALOG_INF(PSTR("ConstructJSON_Sensor()"));

  JBI->Start();

    // JBI->Add("Redunction", mqtthandler_sensor_ifchanged.flags.FrequencyRedunctionLevel);
  // return 0;
  
  float sensor_data = -1;
  String sensor_data_string = String("none");
  char buffer[100] = {0};
  bool flag_level_started = false;
  bool flag_level_ended_needed = false;

  uint16_t type_id_adjusted = 0;

  for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:pCONT->pModule)
    {
      if( IS_MODULE_SENSOR_SUBMODULE( pmod->GetModuleUniqueID() ) )
      {
        //Get any sensors in module
        uint8_t sensors_available = pmod->GetSensorCount();
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleName());

        uint16_t unified_sensor_reporting_invalid_reading_timeout_seconds = tkr_set->Settings.unified_interface_reporting_invalid_reading_timeout_seconds;
        // ALOG_WRN(PSTR("reading_timeout_seconds %d"), unified_sensor_reporting_invalid_reading_timeout_seconds);
                  
        if(sensors_available) 
        {
          // ALOG_INF( PSTR("GetSensorCount\t\t =%d\t%s"), sensors_available, pmod->GetModuleName());
          for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
          {
            sensors_reading_t val;
            pmod->GetSensorReading(&val, sensor_id);

            if(unified_sensor_reporting_invalid_reading_timeout_seconds != 0) // Check active
            {
                              
              if(val.timestamp)
              {
                if(tkr_time->RtcTime.valid) // Only enable timestamp checks when time is valid
                {
                  uint32_t sensor_elapsed_time = tkr_time->UtcTime() - val.timestamp;
                  if(sensor_elapsed_time) // If positive and NOT val.timestamp set to 0 as skipped
                  {
                    if(sensor_elapsed_time > unified_sensor_reporting_invalid_reading_timeout_seconds)
                    {
                      // ALOG_INF(PSTR("sensor_id %d val.sensor_if %d"), sensor_id, val.sensor_id);
                      #ifdef ENABLE_DEVFEATURE_UNIFIED_REPORTING_SKIPPING_INVALID_TIMEOUT_READINGS
                      ALOG_WRN(PSTR("sensor time invalid %d > %d"), sensor_elapsed_time, unified_sensor_reporting_invalid_reading_timeout_seconds);
                      continue; // skip the result in this loop
                      #else
                      DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));
                      ALOG_DBM(PSTR("sensor_elapsed_time missing %S %s %d %d"), pmod->GetModuleName(), buffer, sensor_elapsed_time, unified_sensor_reporting_invalid_reading_timeout_seconds);
                      #endif
                    }
                  }
                }
              }
            }
            
            if(val.Valid())
            {

              /**
               * @brief Special cases
               * 
               */
              if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
              {
                type_id_adjusted = SENSOR_TYPE_TEMPERATURE_ID;              
              }
              else
              {
                type_id_adjusted = type_id;
              }

              if(val.isFloatWaiting_WithSensorType(type_id_adjusted))
              {
                
                // val.sensor_id is used to since the order of devicename list may not match in accending order
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

                /**
                 * @brief Modify for special cases
                 * 
                 */
                if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
                {

                  sensor_data = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);                
                  // Convert into colour
                  float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
                  #ifdef USE_MODULE_LIGHTS_INTERFACE
                  
                  // Only add sensor type if any has been found
                  if(flag_level_started != true)
                  {     
                    JBI->Level_Start_P( GetUnifiedSensor_NameByTypeID(type_id) );
                    flag_level_started = true;
                    flag_level_ended_needed = true;
                  }
                  
                  RgbwwColor colour  = GetColourValueUsingMaps_ForUnifiedSensor(temperature);

                  JBI->Add_FV(
                    DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                    PSTR("\"%02X%02X%02X\""),
                    colour.R, colour.G, colour.B
                  );
                  #endif // USE_MODULE_LIGHTS_INTERFACE

                }
                /**
                 * @brief As read from sensor
                 * 
                 */
                else
                {
                  // Only add sensor type if any has been found
                  if(flag_level_started != true)
                  {     
                    JBI->Level_Start_P( GetUnifiedSensor_NameByTypeID(type_id) );
                    flag_level_started = true;
                    flag_level_ended_needed = true;
                  }
                  
                  sensor_data = val.GetFloat(type_id);
                  JBI->Add(buffer, sensor_data);
                }
                
              }

              // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
              // {
              //   DEBUG_LINE_HERE;
              // Serial.println(val.GetString(type_id));
              //     sensor_data_string = val.GetString(type_id);
              // Serial.println(sensor_data_string);
              // Serial.println(sensor_data_string.c_str());
              
              // }
              
              // sensor_data_string = val.GetString(type_id);
              // if(!sensor_data_string.equals("error"))
              // {

              #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
              if(val.HasString(sensor_id))
              {
              // if((sensor_data_string = val.GetString(type_id)) != "error" )
              // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
              // {

                sensor_data_string = val.GetString(type_id);

                
                // Only add sensor type if any has been found
                if(flag_level_started != true)
                {              
                  JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                  flag_level_started = true;
                  flag_level_ended_needed = true;
                }
                
                // val.sensor_id is used to since the order of devicename list may not match in accending order
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

                JBI->Add(buffer, sensor_data_string.c_str());
                
              }
              #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

            }
          }

        }
      } // oNLY USE SENSOR MODULES

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }

  } // END sensor_type





  
//   // for (
//     uint16_t type_id = SENSOR_TYPE_TEMPERATURE_ID; 
//   // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
//   // {

//     // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
//     /**
//      * @brief Check by sensor reported type
//      **/
//     for(auto& pmod:pCONT->pModule)
//     {
//       //Get any sensors in module
//       uint8_t sensors_available = pmod->GetSensorCount();
//       // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
//       if(sensors_available)
//       {
//         // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

//         for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
//         {
//           sensors_reading_t val;
//           pmod->GetSensorReading(&val, sensor_id);
          
//           if(val.Valid())
//           {

//             if(val.isFloatWaiting_WithSensorType(type_id))
//             {

//             // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
//             // {
//             // if(val.HasFloat(sensor_id))
//             // {

//               sensor_data = val.GetFloat(type_id);

//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {     
//                 JBI->Object_Start("TemperatureHeatMapBrightness");//PM_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


//               // Convert into colour
//               float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//               RgbColor colour  = tkr_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

//               JBI->Add_FV(
//                 DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
//                 PSTR("\"%02X%02X%02X\""),
//                 colour.R, colour.G, colour.B
//               );


//               // JBI->Add(buffer, sensor_data);
              
//             }


// // {
// //   "Temperature":{
// //     "BedroomDesk-DHT1":"00FF33",
// //     "BedroomDesk-DHT1":27.900,
// //     "BedroomDesk-DHT2":"00FF33",
// //     "BedroomDesk-DHT2":28.000,
// //     "BedroomDesk-BME":"00FF1D",
// //     "BedroomDesk-BME":28.750,
// //     "BedroomDesk-BME2":"00FF22",
// //     "BedroomDesk-BME2":28.510,
// //     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


//             // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
//             // {
//             //   DEBUG_LINE_HERE;
//             // Serial.println(val.GetString(type_id));
//             //     sensor_data_string = val.GetString(type_id);
//             // Serial.println(sensor_data_string);
//             // Serial.println(sensor_data_string.c_str());
            
//             // }
            
//             // sensor_data_string = val.GetString(type_id);
//             // if(!sensor_data_string.equals("error"))
//             // {

//             #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
//             if(val.HasString(sensor_id))
//             {
//             // if((sensor_data_string = val.GetString(type_id)) != "error" )
//             // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
//             // {

//               sensor_data_string = val.GetString(type_id);

              
//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {              
//                 JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

//               JBI->Add(buffer, sensor_data_string.c_str());
              
//             }
//             #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

//           }
//         }

//       }

//     } // END modules checking
    
//     /**
//      * @brief Only finish Level if type was added at least once
//      * 
//      * @return * Only 
//      */
//     if(flag_level_ended_needed)
//     {
//       JBI->Object_End();
//       flag_level_ended_needed = false;
//       flag_level_started = false;     // closed level
//     }





//   // for (
//     // uint16_t
//     type_id = SENSOR_TYPE_TEMPERATURE_ID; 
//   // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
//   // {

//     // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
//     /**
//      * @brief Check by sensor reported type
//      **/
//     for(auto& pmod:pCONT->pModule)
//     {
//       //Get any sensors in module
//       uint8_t sensors_available = pmod->GetSensorCount();
//       // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
//       if(sensors_available)
//       {
//         // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

//         for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
//         {
//           sensors_reading_t val;
//           pmod->GetSensorReading(&val, sensor_id);
          
//           if(val.Valid())
//           {

//             if(val.isFloatWaiting_WithSensorType(type_id))
//             {

//             // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
//             // {
//             // if(val.HasFloat(sensor_id))
//             // {

//               sensor_data = val.GetFloat(type_id);

//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {     
//                 JBI->Level_Start_P(PM_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


//               // Convert into colour
//               float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//               RgbColor colour  = tkr_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);

//               JBI->Add_FV(
//                 DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
//                 PSTR("\"%02X%02X%02X\""),
//                 colour.R, colour.G, colour.B
//               );


//               // JBI->Add(buffer, sensor_data);
              
//             }


// // {
// //   "Temperature":{
// //     "BedroomDesk-DHT1":"00FF33",
// //     "BedroomDesk-DHT1":27.900,
// //     "BedroomDesk-DHT2":"00FF33",
// //     "BedroomDesk-DHT2":28.000,
// //     "BedroomDesk-BME":"00FF1D",
// //     "BedroomDesk-BME":28.750,
// //     "BedroomDesk-BME2":"00FF22",
// //     "BedroomDesk-BME2":28.510,
// //     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


//             // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
//             // {
//             //   DEBUG_LINE_HERE;
//             // Serial.println(val.GetString(type_id));
//             //     sensor_data_string = val.GetString(type_id);
//             // Serial.println(sensor_data_string);
//             // Serial.println(sensor_data_string.c_str());
            
//             // }
            
//             // sensor_data_string = val.GetString(type_id);
//             // if(!sensor_data_string.equals("error"))
//             // {

//             #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
//             if(val.HasString(sensor_id))
//             {
//             // if((sensor_data_string = val.GetString(type_id)) != "error" )
//             // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
//             // {

//               sensor_data_string = val.GetString(type_id);

              
//               // Only add sensor type if any has been found
//               if(flag_level_started != true)
//               {              
//                 JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

//               JBI->Add(buffer, sensor_data_string.c_str());
              
//             }
//             #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

//           }
//         }

//       }

//     } // END modules checking
    
//     /**
//      * @brief Only finish Level if type was added at least once
//      * 
//      * @return * Only 
//      */
//     if(flag_level_ended_needed)
//     {
//       JBI->Object_End();
//       flag_level_ended_needed = false;
//       flag_level_started = false;     // closed level
//     }

    JBI->Add("Rate", mqtthandler_sensor_ifchanged.tRateSecs);

  return JBI->End();
    
}

#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
uint8_t mSensorsInterface::ConstructJSON_Unified_Filtered(uint8_t json_level, bool json_appending)
{
  ALOG_INF(PSTR("ConstructJSON_Unified_Filtered()"));

  JBI->Start();

  char buffer[100] = {0};

  // Step 1: Get unique sensor types
  std::vector<uint16_t> unique_types;
  for (auto& entry : filtered_sensors) {
    if (std::find(unique_types.begin(), unique_types.end(), entry.desired_type_id) == unique_types.end()) {
      unique_types.push_back(entry.desired_type_id);
    }
  }

  // Step 2: Loop through each type and group entries
  for (auto type_id : unique_types)
  {
    bool any_sensor_added = false;

    for (auto& entry : filtered_sensors)
    {
      if (entry.desired_type_id != type_id)
        continue;

      // Get module and latest raw value
      sensors_reading_t val;
      auto* module = pCONT->GetModule(entry.module_id);
      if (!module) continue;

      module->GetSensorReading(&val, entry.sensor_index);
      if (!val.Valid()) continue;

      for (size_t i = 0; i < val.sensor_type.size(); i++)
      {
        if (val.sensor_type[i] == type_id && i < val.data_f.size())
        {
          float raw_value = val.data_f[i];
          float filtered_value = entry.filter_buffer.Mean();

          // Start type-level JSON object if this is the first sensor for this type
          if (!any_sensor_added)
          {
            JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
            any_sensor_added = true;
          }

          // Sensor name → object with metadata
          DLI->GetDeviceName_WithModuleUniqueID(entry.module_id, entry.sensor_index, buffer, sizeof(buffer));
          JBI->Object_Start(entry.filter_name.c_str()); // "FilterName": {
            JBI->Add("Name", buffer); // device name inside
            JBI->Add("Raw", raw_value);
            JBI->Add("Avg", filtered_value);
            JBI->Add("Samples", static_cast<uint16_t>(entry.filter_buffer.GetRawData().size()));
            JBI->Add("WindowSec", static_cast<uint16_t>(entry.window_secs));
            JBI->Add("SamplesPerSec", (float)entry.sample_count / (float)entry.window_secs);
          JBI->Object_End(); // }

          break; // done with this sensor
        }
      }
    }

    if (any_sensor_added)
    {
      JBI->Object_End(); // Close the type block
    }
  }

  return JBI->End();
}
#endif





#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
void mSensorsInterface::Update_UnifiedFilteredReadings()
{
    uint32_t now = millis();

    for (auto& entry : filtered_sensors)
    {
        if (now - entry.tLastUpdate < entry.sample_interval_ms) {
            continue; // Not time to sample this sensor yet
        }

        // Find module by unique ID
        auto* module = pCONT->GetModule(entry.module_id);
        if (!module) {
            continue; // Module not found
        }

        sensors_reading_t val;
        module->GetSensorReading(&val, entry.sensor_index);

        if (!val.Valid()) {
            continue; // Skip invalid reading
        }

        // Look for the desired sensor_type within the reading
        for (size_t i = 0; i < val.sensor_type.size(); i++) {
            ALOG_DBM(PSTR("sensor_type[%d] = %d"), i, val.sensor_type[i]);

            if (val.sensor_type[i] == entry.desired_type_id && i < val.data_f.size()) {
                entry.filter_buffer.Add(val.data_f[i]);
                // Serial.print(val.data_f[i]); Serial.print(" ");
                // Serial.println(entry.filter_buffer.Mean());
                entry.tLastUpdate = now;
                break; // Only add the first match
            }
        }
    }
}
#endif





/**
 * @brief For any connected temperature sensors, display their colour as a full brightness rgb colour
 * */
uint8_t mSensorsInterface::ConstructJSON_SensorTemperatureColours(uint8_t json_level, bool json_appending){


  JBI->Start();

  // return 0;
  
  float sensor_data = -1;
  String sensor_data_string = String("none");
  char buffer[100] = {0};
  bool flag_level_started = false;
  bool flag_level_ended_needed = false;

  // for (
    uint16_t type_id = SENSOR_TYPE_TEMPERATURE_ID; 
  // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  // {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:pCONT->pModule)
    {
      //Get any sensors in module
      uint8_t sensors_available = pmod->GetSensorCount();
      // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
      if(sensors_available)
      {
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

        for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
        {
          sensors_reading_t val;
          pmod->GetSensorReading(&val, sensor_id);
          
          if(val.Valid())
          {

            if(val.isFloatWaiting_WithSensorType(type_id))
            {

            // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
            // {
            // if(val.HasFloat(sensor_id))
            // {

              sensor_data = val.GetFloat(type_id);

              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {     
                JBI->Object_Start(PM_TEMPERATURE_HEATMAP_RGBSTRING);//PM_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              // sensor_data = tkr_debug->debug_data.input_float1;//map(tkr_debug->debug_data.input_float1, 0,59, 0,70);

              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP // I need to make a version that does not require lighting at all, so simple colour conversion
              uint32_t colour  = GetColourValueUsingMaps_FullBrightness(temperature, 0);
              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                R32(colour), G32(colour), B32(colour)
              );
              #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

              // ALOG_INF(PSTR("%s %d \"%02X%02X%02X\""), buffer, (int)(sensor_data*1000), R32(colour), G32(colour), B32(colour));

              // JBI->Add(buffer, sensor_data);
              
            }


// {
//   "Temperature":{
//     "BedroomDesk-DHT1":"00FF33",
//     "BedroomDesk-DHT1":27.900,
//     "BedroomDesk-DHT2":"00FF33",
//     "BedroomDesk-DHT2":28.000,
//     "BedroomDesk-BME":"00FF1D",
//     "BedroomDesk-BME":28.750,
//     "BedroomDesk-BME2":"00FF22",
//     "BedroomDesk-BME2":28.510,
//     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


            // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
            // {
            //   DEBUG_LINE_HERE;
            // Serial.println(val.GetString(type_id));
            //     sensor_data_string = val.GetString(type_id);
            // Serial.println(sensor_data_string);
            // Serial.println(sensor_data_string.c_str());
            
            // }
            
            // sensor_data_string = val.GetString(type_id);
            // if(!sensor_data_string.equals("error"))
            // {

            #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
            if(val.HasString(sensor_id))
            {
            // if((sensor_data_string = val.GetString(type_id)) != "error" )
            // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
            // {

              sensor_data_string = val.GetString(type_id);

              
              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {              
                JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              JBI->Add(buffer, sensor_data_string.c_str());
              
            }
            #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

          }
        }

      }

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }





  // for (
    // uint16_t
    type_id = SENSOR_TYPE_TEMPERATURE_ID; 
  // type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  // {

    // ALOG_INF( PSTR("type_id = %d %S"), type_id, GetUnifiedSensor_NameByTypeID(type_id));
    
    /**
     * @brief Check by sensor reported type
     **/
    for(auto& pmod:pCONT->pModule)
    {
      //Get any sensors in module
      uint8_t sensors_available = pmod->GetSensorCount();
      // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());
      
      if(sensors_available)
      {
        // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

        for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
        {
          sensors_reading_t val;
          pmod->GetSensorReading(&val, sensor_id);
          
          if(val.Valid())
          {

            if(val.isFloatWaiting_WithSensorType(type_id))
            {

            // if((sensor_data = val.GetFloat(type_id)) != SENSOR_TYPE_INVALID_READING) // "has float needs to perform this check!"
            // {
            // if(val.HasFloat(sensor_id))
            // {

              sensor_data = val.GetFloat(type_id);

              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {     
                JBI->Level_Start_P(PM_TEMPERATURE_HEATMAP_ADJUSTED_BRIGHTNESS_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
              uint32_t colour  = GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                R32(colour), G32(colour), B32(colour)
              );
              #endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP


              // JBI->Add(buffer, sensor_data);
              
            }


// {
//   "Temperature":{
//     "BedroomDesk-DHT1":"00FF33",
//     "BedroomDesk-DHT1":27.900,
//     "BedroomDesk-DHT2":"00FF33",
//     "BedroomDesk-DHT2":28.000,
//     "BedroomDesk-BME":"00FF1D",
//     "BedroomDesk-BME":28.750,
//     "BedroomDesk-BME2":"00FF22",
//     "BedroomDesk-BME2":28.510,
//     "DB_04":"00FF19","DB_04":28.937,"DB_03":"00FF1D","DB_03":28.812,"DB_01":"00FF1D","DB_01":28.750,"DB_02":"00FF1D","DB_02":28.687}}


            // if(type_id == SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID)
            // {
            //   DEBUG_LINE_HERE;
            // Serial.println(val.GetString(type_id));
            //     sensor_data_string = val.GetString(type_id);
            // Serial.println(sensor_data_string);
            // Serial.println(sensor_data_string.c_str());
            
            // }
            
            // sensor_data_string = val.GetString(type_id);
            // if(!sensor_data_string.equals("error"))
            // {

            #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES
            if(val.HasString(sensor_id))
            {
            // if((sensor_data_string = val.GetString(type_id)) != "error" )
            // // if((sensor_data_string = val.GetString(type_id)) != val.error ) //SENSOR_STRING_TYPE_INVALID)
            // {

              sensor_data_string = val.GetString(type_id);

              
              // Only add sensor type if any has been found
              if(flag_level_started != true)
              {              
                JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));

              JBI->Add(buffer, sensor_data_string.c_str());
              
            }
            #endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNFIED_SENSOR_STRING_TYPES

          }
        }

      }

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Object_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }









    






  // } // END sensor_type

  return JBI->End();

}



#ifdef USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP
/**
 * @brief future global way to map colours using palette
 * 
 * Option in the future to apply different colours to the map.
 * 
 * Global temp colours reporting should have flag otpion to pick between full/adjusted brightness inside primary unified sensor.
 * I should still produce them all for the sub mqtt colours
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return uint32_t colour packed W,R,G,B 
 */
uint32_t mSensorsInterface::GetColourValueUsingMaps_AdjustedBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 0;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

    // Generate Hue and Brt values
    if(value<20){
      hue = 240;
      brt = 10;
    }else
    if((value>=20)&&(value<50)){
      hue = mSupport::mapfloat(value, 20,50, 180,0);
      brt = mSupport::mapfloat(value, 20,50, 10,100);
    }else
    if((value>=50)&&(value<60)){      
      hue = mSupport::mapfloat(value, 50,60, 359,345);
      brt = 100;
    }else
    if(value>=60){
      hue = 340;
      brt = 100;
    }

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t w = 0;
  
  HsbToRgb(hue/360.0f,1.0f,1.0f,&r,&g,&b);

  return RGBW32(r,g,b,w);



}

/**
 * @brief maximum sat/brt, hue changes
 * 
 * In the future, simply create "profiles" that have common temp to colour mappings
 * ie "Hot Water", "Room Comfort"
 * 
 * @param value 
 * @param map_style_id 
 * @param value_min 
 * @param value_max 
 * @param map_is_palette_id 
 * @return RgbColor 
 */
uint32_t mSensorsInterface::GetColourValueUsingMaps_FullBrightness(float value, 
                                            uint8_t map_style_id,
                                            float value_min, float value_max, //not need for some mappings
                                            bool map_is_palette_id
                                          ){

  // map_style_id can use some internal ones here, or else scale and get from palettes

  // Heating rainbow with brighter red end
  uint16_t hue = 0;
  uint8_t  sat = 0;
  uint8_t  brt = 100;

  // if(map is water temperature in celcius ie have different range styles) then convert into rainbow gradient

  // Generate Hue and Brt values
  if(value<15){
    hue = 240;
    brt = 100;
  }else
  if((value>=15)&&(value<25)){  // BLUE to GREEN
    hue = mSupport::mapfloat(value, 15,25, 240,120);
  }else
  if((value>=25)&&(value<30)){  //         GREEN to YELLOW
    hue = mSupport::mapfloat(value, 25,30, 120,60);
  }else
  if((value>=30)&&(value<35)){  //                  YELLOW to ORANGE
    hue = mSupport::mapfloat(value, 30,35, 60,17);
  }else
  if((value>=35)&&(value<40)){  //                            ORANGE to RED
    hue = mSupport::mapfloat(value, 35,40, 17,0);
  }else
  if((value>=40)&&(value<50)){  //                                      RED
    hue = 0;
  }else
  if((value>=50)&&(value<60)){  //                                      RED to PINK
    hue = mSupport::mapfloat(value, 50,60, 360,340);
  }else
  if(value>=60){   // PINK
    hue = 340;
  }

  uint8_t r = 0;
  uint8_t g = 0;
  uint8_t b = 0;
  uint8_t w = 0;

  // hue = 180;

  // char buffer[20];
  // mSupport::float2CString(value, JSON_VARIABLE_FLOAT_PRECISION_LENGTH, buffer);       
  // ALOG_INF(PSTR("value=%s, hue=%d"), buffer, hue);
  
  HsbToRgb((float)hue/360.0f,1.0f,1.0f,&r,&g,&b);
  // HsbToRgb(0.0f,1.0f,1.0f,&r,&g,&b);

  return  RGBW32(r,g,b,w);

}


void mSensorsInterface::HsbToRgb(float h, float s, float v, uint8_t* r8, uint8_t* g8, uint8_t* b8)
{
  float r, g, b;

  if (s == 0.0f)
  {
    r = g = b = v; // achromatic or black
  }
  else
  {
    if (h < 0.0f)
    {
      h += 1.0f;
    }
    else if (h >= 1.0f)
    {
      h -= 1.0f;
    }
    h *= 6.0f;
    int i = (int)h;
    float f = h - i;
    float q = v * (1.0f - s * f);
    float p = v * (1.0f - s);
    float t = v * (1.0f - s * (1.0f - f));
    switch (i)
    {
    case 0:
      r = v;
      g = t;
      b = p;
      break;
    case 1:
      r = q;
      g = v;
      b = p;
      break;
    case 2:
      r = p;
      g = v;
      b = t;
      break;
    case 3:
      r = p;
      g = q;
      b = v;
      break;
    case 4:
      r = t;
      g = p;
      b = v;
      break;
    default:
      r = v;
      g = p;
      b = q;
      break;
    }
  }

  *r8 = r * 255;
  *g8 = g * 255;
  *b8 = b * 255;

}

#endif // USE_DEVFEATURE_INTERNALISE_UNIFIED_SENSOR_INTERFACE_COLOUR_HEATMAP

float mSensorsInterface::ConvertTemp(float c)
{
  float result = c;
  // if (!isnan(c) && Settings.flag_system.temperature_conversion) {
  //   result = c * 1.8 + 32;  // Fahrenheit
  // }
  return result;
}


char mSensorsInterface::TempUnit(void)
{
  return (tkr_set->Settings.flag_system.temperature_conversion) ? 'F' : 'C';
}


float mSensorsInterface::ConvertPressure(float p)
{
  // float result = p;

  // if (!isnan(p) && Settings.flag_system.pressure_conversion) {
  //   result = p * 0.75006375541921;  // mmHg
  // }
  // return result;
}

String mSensorsInterface::PressureUnit(void)
{
  // return (Settings.flag_system.pressure_conversion) ? String(D_UNIT_MILLIMETER_MERCURY) : String(D_UNIT_PRESSURE);
}




/***
 * 
 * The idea is that this function, is the unified place that reports motion
 * But other subfunctions should still report their own motion etc
 * In this case, the "PIR module" should be used instead of SWT and rules, PIR_1 is the new way, not SWT and rule
 * 
 */
uint8_t mSensorsInterface::ConstructJSON_Event_Motion(uint8_t json_level, bool json_appending){

  char buffer[100];

  JBI->Start();

  /**
   * @brief Motion Event : PIR module
   **/
  #ifdef USE_MODULE_SENSORS_PIR
  if(tkr_rules->event_triggered.module_id == tkr_motion->GetModuleUniqueID())
  {
    uint16_t device_id   = tkr_rules->event_triggered.device_id;
    uint16_t state_id = tkr_rules->event_triggered.value.data[0];  

    JBI->Add(D_LOCATION, DLI->GetDeviceName_WithModuleUniqueID( tkr_motion->GetModuleUniqueID(), device_id, buffer, sizeof(buffer))); 
    JBI->Add("Time", tkr_time->GetTimeStr(tkr_time->Rtc.local_time).c_str());
    JBI->Add("UTCTime", tkr_time->Rtc.local_time);
    JBI->Add(D_EVENT, state_id ? "detected": "over");
    JBI->Add("Sensor", tkr_motion->GetModuleName());

  }
  #endif // USE_MODULE_SENSORS_PIR

  return JBI->End();
    
}



uint8_t mSensorsInterface::ConstructJSON_Event_UserInput(uint8_t json_level, bool json_appending){

  char buffer[100];

  if(!tkr_rules->event_triggered.isvalid) return JBI->End();

  JBI->Start();

  JBI->Add("Source", tkr->GetModuleName( tkr_rules->event_triggered.module_id ));
  JBI->Add("Name", DLI->GetDeviceName_WithModuleUniqueID( tkr_rules->event_triggered.module_id, tkr_rules->event_triggered.device_id, buffer, sizeof(buffer)));
  JBI->Add("Value", tkr_rules->event_triggered.value.data[0]);

  char state[100];

  #ifdef USE_MODULE_SENSORS_SWITCHES
  if(tkr_rules->event_triggered.module_id == tkr_switch->GetModuleUniqueID())
  {
    tkr_switch->GetStateName(tkr_rules->event_triggered.value.data[0], state, sizeof(state));
  }
  #endif
  #ifdef USE_MODULE_SENSORS_BUTTONS
  if(tkr_rules->event_triggered.module_id == tkr_button->GetModuleUniqueID())
  {
    tkr_button->GetStateName(tkr_rules->event_triggered.value.data[0], tkr_rules->event_triggered.value.data[1], state, sizeof(state)); // data0=type, data1=presses
  }
  #endif

  JBI->Add("State", state);
  JBI->Add("LocalTime", tkr_time->GetTime().c_str());


  return JBI->End();
    
}
  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsInterface::MQTTHandler_Init(){

  struct handler<mSensorsInterface>* ptr;
 
  ptr = &mqtthandler_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->dt.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_sensor_temperature_colours;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_TEMPERATURE_COLOURS__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_SensorTemperatureColours;
  mqtthandler_list.push_back(ptr);
  
  #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE__UNIFIED_SENSOR_FILTERING
  ptr = &mqtthandler_sensor_unified_filtered;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED_FILTERED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Unified_Filtered;
  mqtthandler_list.push_back(ptr);
  #endif

  //motion events
  ptr = &mqtthandler_motion_event_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->dt.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__EVENT_MOTION__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Event_Motion;
  mqtthandler_list.push_back(ptr);

  ptr = &mqtthandler_event_input;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = SEC_IN_HOUR; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__EVENT_USER_INPUT__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Event_UserInput;
  mqtthandler_list.push_back(ptr);


  

} 


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsInterface::MQTTHandler_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsInterface::MQTTHandler_Rate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = tkr_mqtt->dt.ifchanged_secs;
  }
}


/**
 * @brief MQTTHandler_Sender
 * */
void mSensorsInterface::MQTTHandler_Sender()
{
  for(auto& handle:mqtthandler_list){
    tkr_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/


#endif
