#include "mSensorsInterface.h" 

#ifdef USE_MODULE_SENSORS_INTERFACE 


uint8_t mSensorsInterface::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
  // Ask all modules for their sensor count to get total (or check devicelist via type sensor)
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}



#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING


#ifdef ENABLE_DEVFEATURE_CONSTRUCTJSON_VERSION1

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
uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_method){


      // ALOG_INF( PSTR("DEBUG_LINE_HERE 205") );
  JsonBuilderI->Start();


float sensor_data = -1;
char sensor_type_ctr[30] = {0};
char buffer[100] = {0};
char sensor_type_name[100] = {0};
bool flag_level_started = false;
bool flag_level_ended = false;
bool flag_level_started_needed = false;
bool flag_level_ended_needed = false;

for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
{

  // flag_level_started = false;
  // flag_level_ended   = false;
  // flag_level_started_needed = false;
  // flag_level_ended_needed = false;

  /**
   * @brief Check by sensor reported type
   **/
  for(auto& pmod:pCONT->pModule)
  {
    //Get any sensors in module
    uint8_t sensors_available = pmod->GetSensorCount();
    // ALOG_INF( PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());
    
    if(sensors_available)
    {
      ALOG_INF( PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());

      for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
      {
        sensors_reading_t val;
        pmod->GetSensorReading(&val, sensor_id);
        if(val.Valid())
        { 
          
          
      // ALOG_INF( PSTR("DEBUG_LINE_HERE b") );
          // I dont care what sensor reported it in the title, though appending it will be useful
          // JBI->Level_Start_P(PSTR("%S_%d"),pmod->GetModuleFriendlyName(),sensor_id);

          if((sensor_data = val.GetValue(type_id)) != SENSOR_TYPE_INVALID_READING)
          {
            
            // Only add sensor type if any has been found
            if(flag_level_started != true)
            {

              // sensor_id will give the name as defined in template
              uint16_t module_id_unique = pmod->GetModuleUniqueID();
              
              // // JBI->Level_Start_P(
              //   DLI->GetDeviceName_WithModuleUniqueID( module_id_unique, sensor_id, buffer, sizeof(buffer));
              //   // );
              // ALOG_INF( PSTR("module_id_unique =%d\t%d\t%s"),module_id_unique, sensor_id,buffer);

              // JBI->Add(buffer, sensor_data);

              
              /**
               * @brief Level start should contain the sensor_type_name
               * 
               */
              switch(type_id)
              {
                default:                                  JBI->Level_Start("Error");                            break;
                case SENSOR_TYPE_TEMPERATURE_ID:          JBI->Level_Start_P(PSTR(D_JSON_TEMPERATURE));         break;
                case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:    JBI->Level_Start_P(PSTR(D_JSON_HUMIDITY));            break;
                case SENSOR_TYPE_LIGHT_LEVEL_ID:          JBI->Level_Start_P(PSTR("Light Level"));            break;
                case SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID:  JBI->Level_Start_P(PSTR("Lux"));            break;
                break;
              }



              // JBI->Level_Start_P(PSTR("TEST %d"), type_id);
              flag_level_started = true;
              flag_level_ended_needed = true;
            }

            // switch(type_id)
            // {
            //   default:                                  JBI->Add("Error", 0);                         break;
            //   case SENSOR_TYPE_TEMPERATURE_ID:          JBI->Add(D_JSON_TEMPERATURE, sensor_data);    break;
            //   case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:    JBI->Add(D_JSON_HUMIDITY, sensor_data);    break;
            //   break;
            // }

            
              // sensor_id will give the name as defined in template
              uint16_t module_id_unique = pmod->GetModuleUniqueID();
              
              // JBI->Level_Start_P(
                DLI->GetDeviceName_WithModuleUniqueID( module_id_unique, sensor_id, buffer, sizeof(buffer));
                // );

              JBI->Add(buffer, sensor_data);

              // ALOG_INF( PSTR("module_id_unique =%d\t%d\t%s"),module_id_unique, sensor_id,buffer);

              // ALOG_INF( PSTR("DEBUG_LINE_HERE c") );

          }

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
      // ALOG_INF( PSTR("DEBUG_LINE_HERE d") );
    JBI->Level_End();
    flag_level_ended_needed = false;
    flag_level_started = false;     // closed level
    // flag_level_ended = true;
  }

} // END sensor_type



  // for(auto& pmod:pCONT->pModule)
  // {
  //   //Get any sensors in module
  //   uint8_t sensors_available = pmod->GetSensorCount();

  //   ALOG_DBM( PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());

  //   if(sensors_available)
  //   {

  //     for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
  //     {
  //       sensors_reading_t val;
  //       pmod->GetSensorReading(&val, sensor_id);
  //       if(val.Valid())
  //       { 
          
  //         // I dont care what sensor reported it in the title, though appending it will be useful
  //         JBI->Level_Start_P(PSTR("%S_%d"),pmod->GetModuleFriendlyName(),sensor_id);

  //           if((sensor_data = val.GetValue(SENSOR_TYPE_TEMPERATURE_ID)) != SENSOR_TYPE_INVALID_READING){
  //             JBI->Add(D_JSON_TEMPERATURE, sensor_data);
  //           }
  //           if((sensor_data = val.GetValue(SENSOR_TYPE_RELATIVE_HUMIDITY_ID)) != SENSOR_TYPE_INVALID_READING){
  //             JBI->Add(D_JSON_HUMIDITY, sensor_data);
  //           }
  //           if((sensor_data = val.GetValue(SENSOR_TYPE_CURRENT_ID)) != SENSOR_TYPE_INVALID_READING){
  //             JBI->Add(D_JSON_CURRENT, sensor_data);
  //           }
            
  //           // Report sensor
  //           snprintf_P(sensor_type_ctr, sizeof(sensor_type_ctr), "%S", pmod->GetModuleFriendlyName());
  //           JBI->Add(D_JSON_SENSOR_TYPE, sensor_type_ctr);
            
  //         JBI->Level_End();
 
  //       }
  //       // else{
  //       //   AddLog(LOG_LEVEL_TEST, PSTR("GetSensorCount =INVALID"));
  //       //   Serial.println(sensor_data.data[0]);
  //       // }
  //     }

  //   }


   
  // }


  // return JsonBuilderI->End();






DEBUG_LINE_HERE;

  return JsonBuilderI->End();
    
}

#endif // ENABLE_DEVFEATURE_CONSTRUCTJSON_VERSION1





#ifdef ENABLE_DEVFEATURE_CONSTRUCTJSON_VERSION2

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
uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_method){


      // ALOG_INF( PSTR("DEBUG_LINE_HERE 205") );
  JsonBuilderI->Start();


float sensor_data = -1;
char sensor_type_ctr[30] = {0};
char buffer[100] = {0};
char sensor_type_name[100] = {0};
bool flag_level_started = false;
bool flag_level_ended = false;
bool flag_level_started_needed = false;
bool flag_level_ended_needed = false;

for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
{

  /**
   * @brief Check by sensor reported type
   **/
  for(auto& pmod:pCONT->pModule)
  {
    //Get any sensors in module
    uint8_t sensors_available = pmod->GetSensorCount();
    // ALOG_INF( PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());
    
    if(sensors_available)
    {
      ALOG_INF( PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());

      for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
      {
        sensors_reading_t val;
        pmod->GetSensorReading(&val, sensor_id);
        if(val.Valid())
        { 
          
          if((sensor_data = val.GetValue(type_id)) != SENSOR_TYPE_INVALID_READING)
          {
            
            // Only add sensor type if any has been found
            if(flag_level_started != true)
            {              
              /**
               * @brief Level start should contain the sensor_type_name
               * */
              switch(type_id)
              {
                default:                                  JBI->Level_Start("Error");                            break;
                case SENSOR_TYPE_TEMPERATURE_ID:          JBI->Level_Start_P(PSTR(D_JSON_TEMPERATURE));         break;
                case SENSOR_TYPE_RELATIVE_HUMIDITY_ID:    JBI->Level_Start_P(PSTR(D_JSON_HUMIDITY));            break;
                case SENSOR_TYPE_LIGHT_LEVEL_ID:          JBI->Level_Start_P(PSTR("Light Level"));              break;
                case SENSOR_TYPE_LIGHT_LUMINANCE_LUX_ID:  JBI->Level_Start_P(PSTR("Lux"));                      break;
                case SENSOR_TYPE_SUN_AZIMUTH_ID:          JBI->Level_Start_P(PSTR("SolarAzimuth"));                      break;
                case SENSOR_TYPE_SUN_ELEVATION_ID:        JBI->Level_Start_P(PSTR("SolarElevation"));                      break;
              }

              flag_level_started = true;
              flag_level_ended_needed = true;
            }
            
            // sensor_id will give the name as defined in template
            DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer));

            JBI->Add(buffer, sensor_data);
            
          }

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



  // for(auto& pmod:pCONT->pModule)
  // {
  //   //Get any sensors in module
  //   uint8_t sensors_available = pmod->GetSensorCount();

  //   ALOG_DBM( PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());

  //   if(sensors_available)
  //   {

  //     for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
  //     {
  //       sensors_reading_t val;
  //       pmod->GetSensorReading(&val, sensor_id);
  //       if(val.Valid())
  //       { 
          
  //         // I dont care what sensor reported it in the title, though appending it will be useful
  //         JBI->Level_Start_P(PSTR("%S_%d"),pmod->GetModuleFriendlyName(),sensor_id);

  //           if((sensor_data = val.GetValue(SENSOR_TYPE_TEMPERATURE_ID)) != SENSOR_TYPE_INVALID_READING){
  //             JBI->Add(D_JSON_TEMPERATURE, sensor_data);
  //           }
  //           if((sensor_data = val.GetValue(SENSOR_TYPE_RELATIVE_HUMIDITY_ID)) != SENSOR_TYPE_INVALID_READING){
  //             JBI->Add(D_JSON_HUMIDITY, sensor_data);
  //           }
  //           if((sensor_data = val.GetValue(SENSOR_TYPE_CURRENT_ID)) != SENSOR_TYPE_INVALID_READING){
  //             JBI->Add(D_JSON_CURRENT, sensor_data);
  //           }
            
  //           // Report sensor
  //           snprintf_P(sensor_type_ctr, sizeof(sensor_type_ctr), "%S", pmod->GetModuleFriendlyName());
  //           JBI->Add(D_JSON_SENSOR_TYPE, sensor_type_ctr);
            
  //         JBI->Level_End();
 
  //       }
  //       // else{
  //       //   AddLog(LOG_LEVEL_TEST, PSTR("GetSensorCount =INVALID"));
  //       //   Serial.println(sensor_data.data[0]);
  //       // }
  //     }

  //   }


   
  // }


  // return JsonBuilderI->End();






DEBUG_LINE_HERE;

  return JsonBuilderI->End();
    
}

#endif // ENABLE_DEVFEATURE_CONSTRUCTJSON_VERSION2




#endif // ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING



































// void mSensorsInterface::JsonAdd_


/**
 * @brief For any connected temperature sensors, display their colour as a full brightness rgb colour
 * */
uint8_t mSensorsInterface::ConstructJSON_SensorTemperatureColours(uint8_t json_method){

  JsonBuilderI->Start();

// #ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

char buffer[50];

  for(auto& pmod:pCONT->pModule)
  {
    //Get any sensors in module
    uint8_t sensors_available = pmod->GetSensorCount();
    uint16_t module_id_unique = pmod->GetModuleUniqueID();

    if(sensors_available)
    {
      JBI->Level_Start_P(PSTR("Temperature"));//,pmod->GetModuleFriendlyName());

      for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
      {
        sensors_reading_t val;
        pmod->GetSensorReading(&val, sensor_id);
        if(val.Valid())
        {

          int8_t device_name_id = sensor_id;
          #ifdef USE_MODULE_SENSORS_DS18X
          //temp fix
          if(module_id_unique == pCONT_msdb18->GetModuleUniqueID())
          {
            device_name_id = pCONT_msdb18->sensor[sensor_id].address_id;
          }
          #endif // USE_MODULE_SENSORS_DS18X

          JBI->Add(
            DLI->GetDeviceName_WithModuleUniqueID( module_id_unique, device_name_id, buffer, sizeof(buffer)),
            val.GetValue(SENSOR_TYPE_TEMPERATURE_ID)
          );
        }
      }

      JBI->Level_End();

      #ifdef USE_MODULE_LIGHTS_INTERFACE
      JBI->Level_Start_P(PSTR("HeatMap"));//,pmod->GetModuleFriendlyName());

      for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
      {
        sensors_reading_t val;
        pmod->GetSensorReading(&val, sensor_id);
        if(val.Valid())
        {

          int8_t device_name_id = sensor_id;
          #ifdef USE_MODULE_SENSORS_DS18X
          //temp fix
          /**
           * Issue#1 address_id stored in sensor struct complicates retrieving devicename from module_id, 
           * Fix1: Remove address_id, instead making it the struct index, thus reordered contents of struct is required (maybe using address to poll sensor that is stored in struct, ie named sensor X, in index X, uses this address... if not, just append address as new struct indexes)
           * This means, on setting name, I should search for the address of X and put it into index X.. swap?
           * */
          if(module_id_unique == pCONT_msdb18->GetModuleUniqueID())
          {
            device_name_id = pCONT_msdb18->sensor[sensor_id].address_id;
          }
          #endif// USE_MODULE_SENSORS_DS18X

          // Convert into colour
          float temperature = val.GetValue(SENSOR_TYPE_TEMPERATURE_ID);
          RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps(temperature,0);

          JBI->Add_FV(
            DLI->GetDeviceName_WithModuleUniqueID( module_id_unique, device_name_id, buffer, sizeof(buffer)),
            PSTR("\"%02X%02X%02X\""),
            colour.R, colour.G, colour.B
          );
        }
      }

      JBI->Level_End();
      JBI->Level_Start_P(PSTR("HeatMapFullRange"));//,pmod->GetModuleFriendlyName());

      for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
      {
        sensors_reading_t val;
        pmod->GetSensorReading(&val, sensor_id);
        if(val.Valid())
        {

          int8_t device_name_id = sensor_id;
          #ifdef USE_MODULE_SENSORS_DS18X
          //temp fix
          /**
           * Issue#1 address_id stored in sensor struct complicates retrieving devicename from module_id, 
           * Fix1: Remove address_id, instead making it the struct index, thus reordered contents of struct is required (maybe using address to poll sensor that is stored in struct, ie named sensor X, in index X, uses this address... if not, just append address as new struct indexes)
           * This means, on setting name, I should search for the address of X and put it into index X.. swap?
           * */
          if(module_id_unique == pCONT_msdb18->GetModuleUniqueID())
          {
            device_name_id = pCONT_msdb18->sensor[sensor_id].address_id;
          }
          #endif// USE_MODULE_SENSORS_DS18X

          // Convert into colour
          float temperature = val.GetValue(SENSOR_TYPE_TEMPERATURE_ID);
          RgbColor colour  = pCONT_iLight->GetColourValueUsingMapsMaximumBrightness(temperature,0);

          JBI->Add_FV(
            DLI->GetDeviceName_WithModuleUniqueID( module_id_unique, device_name_id, buffer, sizeof(buffer)),
            PSTR("\"%02X%02X%02X\""),
            colour.R, colour.G, colour.B
          );
        }
      }

      JBI->Level_End();
      #endif // USE_MODULE_LIGHTS_INTERFACE
    }


   
  }


  return JsonBuilderI->End();





// #endif



  return JsonBuilderI->End();
    
}




uint8_t mSensorsInterface::ConstructJSON_Motion_Event(uint8_t json_method){

  JsonBuilderI->Start();
    // JsonBuilderI->Add("motion", 0);

    
  //   for(uint8_t sensor_id=0;sensor_id<settings.sensors_active;sensor_id++){
  //   if(pir_detect[sensor_id].ischanged){ 
      
  //     pir_detect[sensor_id].ischanged = false;
      
  //     JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(E M_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
  //     JsonBuilderI->Add(D_JSON_TIME, mTime::ConvertShortTime_HHMMSS(&pir_detect[sensor_id].detected_time, buffer, sizeof(buffer)));
  //     JsonBuilderI->Add(D_JSON_EVENT, pir_detect[sensor_id].isactive ? "detected": "over");
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




  return JsonBuilderI->End();
    
}



#endif
