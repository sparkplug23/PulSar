#include "mEnergyInterface.h" 


#ifdef USE_MODULE_ENERGY_INTERFACE 

int8_t mEnergyInterface::Tasker(uint8_t function, JsonParserObject obj){
  
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

  if(module_state.mode != ModuleStatus::Running){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      EveryLoop();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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
     * TELEMETRY SECTION * 
    *******************/
    case TASK_TELEMETRY_HANDLERS_INIT:
      Telemetry_Init();
    break;
    case TASK_TELEMETRY_REFRESH_SEND_ALL:
      tkr_tele->Telemetry_RefreshAll(telemetry_list);
    break;
    case TASK_TELEMETRY_SET_DEFAULT_TRANSMIT_PERIOD:
      tkr_tele->Telemetry_Rate(telemetry_list);
    break;
    #ifdef USE_MODULE_NETWORK_MQTT
    case TASK_TELEMETRY__SENDER_MQTT:
      tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_SERIAL
    case TASK_SERIAL_TELEMETRY:
      tkr_serial->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
    #ifdef USE_MODULE_NETWORK_WEBSERVER
    case TASK_WEB_TELEMETRY:
      tkr_web->Telemetry_Sender(telemetry_list, *this);
    break;
    #endif
  }
  
  return function_result;

} // END function


   

void mEnergyInterface::Pre_Init(void)
{

}


void mEnergyInterface::Init(void)
{

  
  energy_usage.energy_power_delta = DEFAULT_POWER_DELTA;

  energy_usage.energy_power_calibration   = HLW_PREF_PULSE;
  energy_usage.energy_voltage_calibration = HLW_UREF_PULSE;
  energy_usage.energy_current_calibration = HLW_IREF_PULSE;

  energy_usage.energy_kWhtoday     = 0;
  energy_usage.energy_kWhyesterday = 0;
  energy_usage.energy_kWhdoy       = 0;
  energy_usage.energy_kWhtotal     = 0;

  energy_usage.energy_min_power   = 0;
  energy_usage.energy_max_power   = 0;
  energy_usage.energy_min_voltage = 0;
  energy_usage.energy_max_voltage = 0;
  energy_usage.energy_min_current = 0;
  energy_usage.energy_max_current = 0;

  energy_usage.energy_max_power_limit        = 0;
  energy_usage.energy_max_power_limit_hold   = MAX_POWER_HOLD;
  energy_usage.energy_max_power_limit_window = MAX_POWER_WINDOW;

  energy_usage.energy_max_power_safe_limit        = 0;
  energy_usage.energy_max_power_safe_limit_hold   = SAFE_POWER_HOLD;
  energy_usage.energy_max_power_safe_limit_window = SAFE_POWER_WINDOW;

  energy_usage.energy_max_energy       = 0;
  energy_usage.energy_max_energy_start = 0;


  // settings.sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  module_state.mode = ModuleStatus::Running;
}


void mEnergyInterface::EveryLoop()
{

}


  
/******************************************************************************************************************
 * Commands
*******************************************************************************************************************/

  
void mEnergyInterface::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;



    
}

/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/



uint8_t mEnergyInterface::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

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
uint8_t mEnergyInterface::ConstructJSON_Sensor(uint8_t json_level, bool json_appending)
{

  JBI->Start();

    // JBI->Add("Redunction", telemetry_sensor_ifchanged.flags.FrequencyRedunctionLevel);
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
    for(auto& pmod:tkr->pModule)
    {
      if( IS_MODULE_ENERGY_SUBMODULE( pmod->GetModuleUniqueID() ) )
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


  JBI->Add("Rate", telemetry_state_ifchanged.tRateSecs);


  return JBI->End();
    
}

  
/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/


#ifdef USE_MODULE_NETWORK_MQTT

void mEnergyInterface::Telemetry_Init(){

  struct telemetry_handler<mEnergyInterface>* ptr;
 
  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__ENERGY_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC__ENERGY_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mEnergyInterface::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);

}

#endif // USE_MODULE_NETWORK_MQTT

#endif // end header guard



