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
      // Serial.println(pCONT_msdb18->GetSensorReading());
      // Serial.println(pCONT_dht->GetSensorReading());
      // Serial.println(pCONT_msdb18->test123());
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
  settings.sealevel_pressure = SENSORS_PRESSURE_SEALEVELHPA;
  settings.fEnableSensor = true;
}


void mSensorsInterface::EveryLoop()
{

}


const char* mSensorsInterface::GetUnifiedSensor_NameByTypeID(uint8_t id)
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
    
  }
  return PM_SEARCH_NOMATCH;      

}






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





#endif
