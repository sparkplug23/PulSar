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

  // if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_SECOND:{
      // Serial.println(sizeof(sensors_reading_t));
      // Serial.println(pCONT_msdb18->GetSensorReading());
      // Serial.println(pCONT_dht->GetSensorReading());
      // Serial.println(pCONT_msdb18->test123());
      // Serial.println(pCONT_dht->test123());
      

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

      
      //   pModule[switch_index]->Tasker(function, obj);


      // pCONT_msdb18->test1234(&val);
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
    case FUNC_MQTT_HANDLERS_RESET:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_REFRESH_TELEPERIOD:
      MQTTHandler_Set_TelePeriod();
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
  settings.sealevel_pressure = 1013.25;//SENSORS_PRESSURE_SEALEVELHPA;
  settings.fEnableSensor = true;
}


void mSensorsInterface::EveryLoop()
{

}


void mSensorsInterface::CommandEvent_Motion(uint8_t event_type)
{

  #ifdef USE_MODULE_NETWORK_MQTT
  mqtthandler_motion_event_ifchanged.flags.SendNow = true;
  #endif // USE_MODULE_NETWORK_MQTT

}


uint8_t mSensorsInterface::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
  // Ask all modules for their sensor count to get total (or check devicelist via type sensor)
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);

    

  return JsonBuilderI->End();

}


uint8_t mSensorsInterface::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();

#ifdef ENABLE_DEVFEATURE_SENSOR_INTERFACE_UNIFIED_SENSOR_REPORTING


  for(auto& pmod:pCONT->pModule)
  {
    //Get any sensors in module
    uint8_t sensors_available = pmod->GetSensorCount();

    AddLog(LOG_LEVEL_TEST, PSTR("GetSensorCount =%d\t%s"),sensors_available,pmod->GetModuleFriendlyName());

    if(sensors_available)
    {

      for(int sensor_id=0;sensor_id<sensors_available;sensor_id++)
      {
        sensors_reading_t val;
        pmod->GetSensorReading(&val, sensor_id);
        if(val.Valid())
        {
          JBI->Level_Start_P(PSTR("%S_%d"),pmod->GetModuleFriendlyName(),sensor_id);
            JBI->Add(D_JSON_TEMPERATURE, val.GetValue(SENSOR_TYPE_TEMPERATURE_ID));
            JBI->Add(D_JSON_HUMIDITY, val.GetValue(SENSOR_TYPE_RELATIVE_HUMIDITY_ID));
          JBI->Level_End();
        }
        // else{
        //   AddLog(LOG_LEVEL_TEST, PSTR("GetSensorCount =INVALID"));
        //   Serial.println(sensor_data.data[0]);
        // }
      }

    }


   
  }


  return JsonBuilderI->End();





#endif



  return JsonBuilderI->End();
    
}


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
    uint16_t module_id = pCONT->GetVectorIndexbyModuleUniqueID(pmod->GetModuleUniqueID());

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
if(module_id == EM_MODULE_SENSORS_DB18S20_ID)
{
  device_name_id = pCONT_msdb18->sensor[sensor_id].address_id;
}
#endif // USE_MODULE_SENSORS_DS18X


          JBI->Add(
            DLI->GetDeviceNameWithEnumNumber(module_id, device_name_id, buffer, sizeof(buffer)),
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
if(module_id == EM_MODULE_SENSORS_DB18S20_ID)
{
  device_name_id = pCONT_msdb18->sensor[sensor_id].address_id;
}

#endif// USE_MODULE_SENSORS_DS18X
          // Convert into colour
          float temperature = val.GetValue(SENSOR_TYPE_TEMPERATURE_ID);
          RgbColor colour  = pCONT_iLight->GetColourValueUsingMaps(temperature,0);


          JBI->Add_FV(
            DLI->GetDeviceNameWithEnumNumber(module_id, device_name_id, buffer, sizeof(buffer)),
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
      
  //     JsonBuilderI->Add(D_JSON_LOCATION, DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_MOTION_ID, sensor_id, buffer, sizeof(buffer)));
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




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

  #ifdef USE_MODULE_NETWORK_MQTT
  

void mSensorsInterface::MQTTHandler_Init(){

  struct handler<mSensorsInterface>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_temperature_colours;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 10; 
  #ifdef DEVICE_IMMERSIONSENSOR //temp fix
  mqtthandler_ptr->tRateSecs = 60; 
  #endif
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_IFCHANGED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSOR_TEMPERATURE_COLOURS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_SensorTemperatureColours;

  //motion events
  mqtthandler_ptr = &mqtthandler_motion_event_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = false;
  mqtthandler_ptr->flags.SendNow = false;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_MOTION_EVENT_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSensorsInterface::ConstructJSON_Motion_Event;

 
}

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
void mSensorsInterface::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSensorsInterface::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_INTERFACE_ID, handle, id);
  }
}

  #endif // USE_MODULE_NETWORK_MQTT

#endif
