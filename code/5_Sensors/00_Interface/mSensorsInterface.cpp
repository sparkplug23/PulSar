#include "mSensorsInterface.h" 

/***
 * New joint motion triggered class, all future motion events will also trigger a response from this class (no rules required)
 * */

// All sensors should have generic functions for getting their status
// We should get it from their name, as this would be truly unique and doesnt need any module name or indexing (unless I use that as identifier)
//

/*

struct to return "sensors"


float GetSensorTemperature(module_id, sensor_id)
*/



#ifdef USE_MODULE_SENSORS_INTERFACE 

const char* mSensorsInterface::PM_MODULE_SENSORS_INTERFACE_CTR = D_MODULE_SENSORS_INTERFACE_CTR;
const char* mSensorsInterface::PM_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR = D_MODULE_SENSORS_INTERFACE_FRIENDLY_CTR;

int8_t mSensorsInterface::Tasker(uint8_t function, JsonParserObject obj){
  
  int8_t function_result = 0;
  
  switch(function){
    /************
     * INIT SECTION * 
    *******************/
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND:{
      // Serial.println(sizeof(sensors_reading_t));
      // Serial.println(pCONT_db18->GetSensorReading());
      // Serial.println(pCONT_dht->GetSensorReading());
      // Serial.println(pCONT_db18->test123());
      // Serial.println(pCONT_dht->test123());

      // Make nicer later with json command to enable and time period to show
      if(settings.tTicker_Splash_Sensors_To_Logs-- == 1)
      {
        // Measurement level feedback will be "DebugMore" and show level should be "Debug". "Info" should be reserved for essential stuff not in mqtt
        ALOG_DBM(PSTR(">>> Sensor Readings <<<"));
        pCONT->Tasker_Interface(FUNC_SENSOR_SHOW_LATEST_LOGGED_ID);
        settings.tTicker_Splash_Sensors_To_Logs = 30 ; // reset
      }
      
  


      // for(auto& pmod:pCONT->pModule)
      // {
      //   for(int sensor_id=0;sensor_id<pmod->GetSensorCount();sensor_id++)
      //   {
      //     sensors_reading_t val;
      //     pmod->GetSensorReading(&val, sensor_id);
      //     if(val.type[0])
      //     {
      //       AddLog(LOG_LEVEL_TEST, PSTR("%S %d|%d val.data[%d]=%d"),pmod->GetModuleFriendlyName(), sensor_id, pmod->GetSensorCount(), sensor_id, (int)val.GetValue(SENSOR_TYPE_TEMPERATURE_ID));
      //     }
      //   }
      // }

//REMOTE SENSOR NEEDS TO INCLUDE THE UTC TIME IT WAS READ FOR "AGE" TO WORK REMOTELY
      
      //   pModule[switch_index]->Tasker(function, obj);


      // pCONT_db18->test1234(&val);
      // Serial.println(val.data[0]);
      // pCONT_dht->test1234(&val);
      // Serial.println(val.data[0]);
  }break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    case FUNC_EVENT_MOTION_STARTED_ID:
      CommandEvent_Motion(1);
    break;
    case FUNC_EVENT_MOTION_ENDED_ID:
      CommandEvent_Motion(0);
    break; 
    #ifdef ENABLE_DEVFEATURE_BUTTONS_SEND_EVENT_MESSAGES    
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      MQTT_Report_Event_Button();
      break;
    #endif
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case FUNC_EVENT_SET_POWER_ID:
    //   RulesEvent_Set_Power();
    // break;
    #endif// USE_MODULE_CORE_RULES
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      // MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


   

void mSensorsInterface::Pre_Init(void)
{

}


void mSensorsInterface::Init(void)
{
  settings.sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  settings.fEnableSensor = true;
}


void mSensorsInterface::EveryLoop()
{

}

// move into sensor type?? YES!!
const char* mSensorsInterface::GetUnifiedSensor_NameByTypeID(uint8_t id)
{
  
  switch(id){
    default: 
    return PM_SEARCH_NOMATCH;
    // Floats
    case SENSOR_TYPE_TEMPERATURE_ID:            return PM_JSON_TEMPERATURE;
    case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:      return PM_JSON_HUMIDITY;
    case SENSOR_TYPE_PRESSURE_ID:               return PM_JSON_PRESSURE;
    case SENSOR_TYPE_LIGHT_LEVEL_ID:            return PM_JSON_LIGHT_LEVEL;
    case SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID:    return PM_JSON_LIGHT_LUMINANCE_LUX;
    case SENSOR_TYPE_SUN_AZIMUTH_ID:            return PM_JSON_SUN_AZIMUTH;
    case SENSOR_TYPE_SUN_ELEVATION_ID:          return PM_JSON_SUN_ELEVATION;
    case SENSOR_TYPE_DOOR_POSITION_ID:          return PM_JSON_DOOR_POSITION;
    case SENSOR_TYPE_DOOR_LOCKED_ID:            return PM_JSON_DOOR_LOCKED;    
    case SENSOR_TYPE_ULTRASONIC_DISTANCE_CM_ID: return PM_JSON_ULTRASONIC_DISTANCE_CM;    
    // Strings
    case SENSOR_TYPE_TEMPERATURE_HEATMAP_RGBSTRING_ID:            return PM_JSON_TEMPERATURE_HEATMAP_RGBSTRING;
  }
  return PM_SEARCH_NOMATCH;      

}

#ifdef USE_MODULE_LIGHTS_INTERFACE
/**
 * @brief Single function that applies colour map to any temperature sensors.
 * In InterfaceSensors, the type of applied map can be selected to be added to unified method directly
 * 
 * @param temperature 
 * @return RgbcctColor 
 */
RgbcctColor mSensorsInterface::GetColourValueUsingMaps_ForUnifiedSensor(float temperature) // flag_unified_sensor_colour_heatmap_type
{

  if(flag_unified_sensor_colour_heatmap_type==1) //adjusted
  {
    return pCONT_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);
  }
  else //full colour-
  {
    return pCONT_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);
  }

}
#endif // USE_MODULE_LIGHTS_INTERFACE




void mSensorsInterface::CommandEvent_Motion(uint8_t event_type)
{

  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_motion_event_ifchanged.flags.SendNow = true;
  #endif // USE_MODULE_NETWORK_MQTT

}



void mSensorsInterface::MQTT_Report_Event_Button()
{

    char event_ctr[20]; memset(event_ctr,0,sizeof(event_ctr));

  JsonBuilderI->Start();
  
  // if(pCONT_rules->event_triggered.device_id == 0)
  // {
    sprintf(event_ctr,"%s-%d","Button",pCONT_rules->event_triggered.device_id);

  // }

    JsonBuilderI->Add("Event", event_ctr);

    JsonBuilderI->Add("Device", pCONT_rules->event_triggered.device_id);
    JsonBuilderI->Add("Function", "ButtonPress");

    JsonBuilderI->Add("Task", "Button");
    // JsonBuilderI->Add("Task", "Button");

    JsonBuilderI->Add("State", "SHORT_PRESS");// : "LONG_PRESS");
    
    // JsonBuilderI->Add("value", (tSavedTimeSincePressOn<LONG_PRESS_DURATION) ? "SHORT_PRESS" : "LONG_PRESS");
    JsonBuilderI->Add("Duration", 0);//tSavedTimeSincePressOn);
  JsonBuilderI->End();



  pCONT_mqtt->ppublish("status/sensors_interface/event",JsonBuilderI->GetBufferPtr(),false);


  /**
   * If event was serviced, then clear it
   * */
  #ifndef ENABLE_DEVFEATURE_PHASEOUT_CLEARING_EVENT
  pCONT_rules->Reset(&pCONT_rules->event_triggered);  // I need to remember the last event, so simply use another flag outside of the struct as waiting and clear that if needed
  #endif 

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

		pCONT->Tasker_Interface(FUNC_SENSOR_SCAN_REPORT_TO_JSON_BUILDER_ID);

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
    	AddLog(LOG_LEVEL_TEST, PSTR("ScanSensors=\"%s\""), JBI->GetBufferPtr());
			pCONT_mqtt->Send_Prefixed_P(PSTR(D_TOPIC_RESPONSE), JBI->GetBufferPtr()); // new thread, set/status/response
		}

	}



    
}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/



uint8_t mSensorsInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_CHANNELCOUNT, 0);
  return JBI->End();

}


#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

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

  JBI->Start();

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
                  JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
                  flag_level_started = true;
                  flag_level_ended_needed = true;
                }
                
                RgbColor colour  = GetColourValueUsingMaps_ForUnifiedSensor(temperature);

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
                  JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
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

    } // END modules checking
    
    /**
     * @brief Only finish Level if type was added at least once
     * 
     * @return * Only 
     */
    if(flag_level_ended_needed)
    {
      JBI->Level_End();
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
//                 JBI->Level_Start("TemperatureHeatMapBrightness");//PM_JSON_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


//               // Convert into colour
//               float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//               RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

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
//       JBI->Level_End();
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
//                 JBI->Level_Start_P(PM_JSON_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
//                 flag_level_started = true;
//                 flag_level_ended_needed = true;
//               }
              
//               // val.sensor_id is used to since the order of devicename list may not match in accending order
//               // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


//               // Convert into colour
//               float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//               RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);

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
//       JBI->Level_End();
//       flag_level_ended_needed = false;
//       flag_level_started = false;     // closed level
//     }














  return JBI->End();
    
}


#else // until devfeature is fixed

uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  JBI->Start();
    JBI->Add("ToBeRemoved");
  return JBI->End();
    
}

#endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING



































// void mSensorsInterface::JsonAdd_


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
                JBI->Level_Start(PM_JSON_TEMPERATURE_HEATMAP_RGBSTRING);//PM_JSON_TEMPERATURE_HEATMAP_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_MODULE_LIGHTS_INTERFACE
              RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                colour.R, colour.G, colour.B
              );
              #endif // USE_MODULE_LIGHTS_INTERFACE


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
      JBI->Level_End();
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
                JBI->Level_Start_P(PM_JSON_TEMPERATURE_HEATMAP_ADJUSTED_BRIGHTNESS_RGBSTRING);//GetUnifiedSensor_NameByTypeID(type_id));
                flag_level_started = true;
                flag_level_ended_needed = true;
              }
              
              // val.sensor_id is used to since the order of devicename list may not match in accending order
              // DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer));


              // Convert into colour
              float temperature = sensor_data;//val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
              #ifdef USE_MODULE_LIGHTS_INTERFACE
              RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);

              JBI->Add_FV(
                DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
                PSTR("\"%02X%02X%02X\""),
                colour.R, colour.G, colour.B
              );
              #endif // USE_MODULE_LIGHTS_INTERFACE


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
      JBI->Level_End();
      flag_level_ended_needed = false;
      flag_level_started = false;     // closed level
    }









    






  // } // END sensor_type

  return JBI->End();

}



//   JBI->Start();

//   // #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

//   char buffer[50];

// //   for(auto& pmod:pCONT->pModule)
// //   {
// //     //Get any sensors in module
// //     uint8_t sensors_available = pmod->GetSensorCount();

// //     if(sensors_available)
// //     {

// //       JBI->Level_Start_P(PSTR("Temperature"));
// //       for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
// //       {
// //         sensors_reading_t val;
// //         pmod->GetSensorReading(&val, sensor_id);
// //         if(val.isFloatWaiting_WithSensorType(SENSOR_TYPE_TEMPERATURE_ID))
// //         {
// //           JBI->Add(
// //             DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
// //             val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID)
// //           );
// //         }
// //       }
// //       JBI->Level_End();
// //     }
// //   }


// // {
// //   "Temperature":{},
// //   "Temperature":{"BedroomDesk-DHT1":24.400,"BedroomDesk-DHT2":24.800},
// //   "Temperature":{"BedroomDesk-BME":26.280,"BedroomDesk-BME2":25.280},
// //   "Temperature":{},
// //   "Temperature":{"DB_04":25.250,"DB_03":25.125,"DB_01":25.687,"DB_02":26.063}
// // }




//   #ifdef USE_MODULE_LIGHTS_INTERFACE

//   //     // JBI->Level_Start_P(PSTR("HeatMap"));
//   //     // for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
//   //     // {
//   //     //   sensors_reading_t val;
//   //     //   pmod->GetSensorReading(&val, sensor_id);
//   //     //   if(val.Valid())
//   //     //   {
//   //     //     // Convert into colour
//   //     //     float temperature = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//   //     //     RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_FullBrightness(temperature,0);

//   //     //     JBI->Add_FV(
//   //     //       DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
//   //     //       PSTR("\"%02X%02X%02X\""),
//   //     //       colour.R, colour.G, colour.B
//   //     //     );

//   //     //   }
//   //     // }
//   //     // JBI->Level_End();

//     // Need to loop for each type of sensor
//     for(auto& pmod:pCONT->pModule)
//     {
//       //Get any sensors in module
//       uint8_t sensors_available = pmod->GetSensorCount();

//       if(sensors_available)
//       {

//         JBI->Level_Start_P(PM_JSON_TEMPERATURE_HEATMAP_RGBSTRING);
//         for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
//         {
//           sensors_reading_t val;
//           pmod->GetSensorReading(&val, sensor_id);
//           if(val.isFloatWaiting_WithSensorType(sensor_id))
//           {
//             // Convert into colour
//             float temperature = val.GetFloat(SENSOR_TYPE_TEMPERATURE_ID);
//             RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps_AdjustedBrightness(temperature,0);

//             JBI->Add_FV(
//               DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), val.sensor_id, buffer, sizeof(buffer)),
//               PSTR("\"%02X%02X%02X\""),
//               colour.R, colour.G, colour.B
//             );
//           }
//         }
//       // JBI->Level_End();

//       }

//     }
  
//   #endif // USE_MODULE_LIGHTS_INTERFACE

//   return JBI->End();

// }




uint8_t mSensorsInterface::ConstructJSON_Motion_Event(uint8_t json_level, bool json_appending){

  JBI->Start();
    // JBI->Add("motion", 0);

    
  //   for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
  //   if(pir_detect[sensor_id].ischanged){ 
      
  //     pir_detect[sensor_id].ischanged = false;
      
  //     JBI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
  //     JBI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
  //     JBI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");
  //triggering sensor (via module id)
  // also module name
  /// also sensor index

/**
 * 
 * Module needs a way to provide known format of type motion, then GetMotion will return a struct with all the important info
 * Should maybe be called GetSensor and GetDriver and internally there are different
 * 
 * 
 * */



  //     //if another is yet to send, then reset the mqtt_handler to fire immeditely again!
  //     //if any mtion flag remains, then set mqtt again
      
  //   }
  // }




  return JBI->End();
    
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mSensorsInterface::MQTTHandler_Init(){

  struct handler<mSensorsInterface>* ptr;
 
  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_temperature_colours;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__SENSORS_TEMPERATURE_COLOURS__CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_SensorTemperatureColours;

  //motion events
  ptr = &mqtthandler_motion_event_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = false;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_EVENT_CTR;
  ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Motion_Event;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSensorsInterface::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSensorsInterface::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}


/**
 * @brief MQTTHandler_Sender
 * */
void mSensorsInterface::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_INTERFACE_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

/******************************************************************************************************************
 * WebServer
*******************************************************************************************************************/





#endif
