#include "mDriverInterface.h" 

#ifdef USE_MODULE_DRIVERS_INTERFACE 

const char* mDriverInterface::PM_MODULE_DRIVERS_INTERFACE_CTR = D_MODULE_DRIVERS_INTERFACE_CTR;
const char* mDriverInterface::PM_MODULE_DRIVERS_INTERFACE_FRIENDLY_CTR = D_MODULE_DRIVERS_INTERFACE_FRIENDLY_CTR;

int8_t mDriverInterface::Tasker(uint8_t function, JsonParserObject obj)
{
  
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
    case FUNC_EVERY_SECOND:
    {
  
    }
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
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
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      // MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
  return function_result;

} // END function


   

void mDriverInterface::Pre_Init(void)
{

}


void mDriverInterface::Init(void)
{
  settings.fEnableSensor = true;
}


void mDriverInterface::EveryLoop()
{

}


const char* mDriverInterface::GetUnifiedSensor_NameByTypeID(uint8_t id)
{
  
  switch(id){
    default: 
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
  }
  return PM_SEARCH_NOMATCH;      

}



void mDriverInterface::MQTT_Report_Event_Button()
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
 * mDriverInterface_Commands.cpp
*******************************************************************************************************************/

void mDriverInterface::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int8_t tmp_id = 0;

	if(jtok = obj["MQTT"].getObject()["DriverStatus"]) // Function should simply call and then send the ConstructJson_DriverStatus()
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
 * mDriverInterface_ConstructJSON.cpp
*******************************************************************************************************************/

uint8_t mDriverInterface::ConstructJSON_Settings(uint8_t json_method, bool json_appending){

  JBI->Start();
    JBI->Add(D_JSON_CHANNELCOUNT, 0);
  return JBI->End();

}


#ifdef ENABLE_FEATURE_DRIVERS_INTERFACE_UNIFIED_DRIVER_REPORTING

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
 * 
 * 
 * 


For drivers, since they are so unique, instead I will just have a function call every driver to append json status




 * @param json_method 
 * @return uint8_t 
 */
uint8_t mDriverInterface::ConstructJson_DriverStatus(uint8_t json_method, bool json_appending)
{

  JBI->Start();

  JBI->Add("Started",1);

  /**
   * @brief Ask all tasks to appendjson_response
   * 
   * AppendJSONResponse_Drivers_Unified()
   * 
   */
  pCONT->Tasker_Interface(FUNC_APPEND_RESPONSE_JSON_DRIVERS_STATUS_ID);
  
  // float sensor_data = -1;
  // char buffer[100] = {0};
  // bool flag_level_started = false;
  // bool flag_level_ended_needed = false;

  // for (uint16_t type_id = 0; type_id < SENSOR_TYPE_LENGTH_ID; type_id++)
  // {

  //       // ALOG_INF( PSTR("type_id = %d"), type_id);
  //   /**
  //    * @brief Check by sensor reported type
  //    **/
  //   for(auto& pmod:pCONT->pModule)
  //   {
  //     //Get any sensors in module
  //     uint8_t sensors_available = pmod->GetSensorCount();
  //     // ALOG_INF( PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());
      
  //     if(sensors_available)
  //     {
  //       // ALOG_INF( PSTR("GetSensorCount =%d\t%s"), sensors_available, pmod->GetModuleFriendlyName());

  //       for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
  //       {
  //         sensors_reading_t val;
  //         pmod->GetSensorReading(&val, sensor_id);
          
  //         if(val.Valid())
  //         {         

  //           if((sensor_data = val.GetValue(type_id)) != SENSOR_TYPE_INVALID_READING)
  //           {
              
  //             // Only add sensor type if any has been found
  //             if(flag_level_started != true)
  //             {              
  //               JBI->Level_Start_P(GetUnifiedSensor_NameByTypeID(type_id));
  //               flag_level_started = true;
  //               flag_level_ended_needed = true;
  //             }
              
  //             // sensor_id will give the name as defined in template
  //             DLI->GetDeviceName_WithModuleUniqueID( pmod->GetModuleUniqueID(), sensor_id, buffer, sizeof(buffer));

  //             JBI->Add(buffer, sensor_data);
              
  //           }

  //         }
  //       }

  //     }

  //   } // END modules checking
    
  //   /**
  //    * @brief Only finish Level if type was added at least once
  //    * 
  //    * @return * Only 
  //    */
  //   if(flag_level_ended_needed)
  //   {
  //     JBI->Level_End();
  //     flag_level_ended_needed = false;
  //     flag_level_started = false;     // closed level
  //   }

  // } // END sensor_type

  return JBI->End();
    
}


#else // until devfeature is fixed

uint8_t mDriverInterface::ConstructJson_DriverStatus(uint8_t json_method, bool json_appending)
{

  JBI->Start();
    JBI->Add("ToBeRemoved");
  return JBI->End();
    
}

#endif // ENABLE_FEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING

/******************************************************************************************************************
 * mDriverInterface_MQTT.cpp
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mDriverInterface::MQTTHandler_Init(){

  struct handler<mDriverInterface>* ptr;
 
  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mDriverInterface::ConstructJSON_Settings;

  ptr = &mqtthandler_driver_status_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC__DRIVERS_UNIFIED__CTR;
  ptr->ConstructJSON_function = &mDriverInterface::ConstructJson_DriverStatus;

} //end "MQTTHandler_Init"


/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mDriverInterface::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mDriverInterface::MQTTHandler_Set_DefaultPeriodRate()
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
void mDriverInterface::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS_INTERFACE_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT


#endif
