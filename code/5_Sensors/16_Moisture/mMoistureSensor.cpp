#include "mMoistureSensor.h"

#ifdef USE_MODULE_SENSORS_MOISTURE


int8_t mMoistureSensor::Tasker(uint8_t function, JsonParserObject obj){

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
    case TASK_EVERY_SECOND:

      adc_raw = analogRead(tkr_pins->Pin(GPIO_MOISTURE_ANALOG));
      
      ALOG_INF(PSTR("val p%d,v%d"), tkr_pins->Pin(GPIO_MOISTURE_ANALOG), adc_raw);

    break;
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
      //tkr_mqtt->Telemetry_Sender(telemetry_list, *this);
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

  return TASKER_RESULT__SUCCESS_ID;

} // END function

void mMoistureSensor::Init(void)
{ 

  int8_t pin = tkr_pins->Pin(GPIO_MOISTURE_ANALOG);

  if(pin >= 0)
  {
    pinMode(pin,INPUT);
    ALOG_INF(PSTR("pin %d val%d"), pin, analogRead(pin));
    module_state.devices++;
  }

  module_state.mode = ModuleStatus::Running;

}

void mMoistureSensor::Pre_Init(void)
{
  if (tkr_pins->PinUsed(GPIO_MOISTURE_ANALOG))
  {
    module_state.mode = ModuleStatus::Initialising;
  }
}

  
/******************************************************************************************************************
 * ConstructJson
*******************************************************************************************************************/


uint8_t mMoistureSensor::ConstructJSON_Settings(uint8_t json_level, bool json_appending){

  JBI->Start();
    JBI->Add_P(PM_SENSOR_COUNT, GetSensorCount());
  return JBI->End();

}

uint8_t mMoistureSensor::ConstructJSON_Sensor(uint8_t json_level, bool json_appending){

  char buffer[40];

  JBI->Start();
    JBI->Add("ADC", adc_raw);
  return JBI->End();

}

/******************************************************************************************************************
 * MQTT
*******************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mMoistureSensor::Telemetry_Init(){

  struct telemetry_handler<mMoistureSensor>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetConfigPeriod_SubModule(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mMoistureSensor::ConstructJSON_Settings;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetTelePeriod_SubModule(); 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mMoistureSensor::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);

  ptr = &telemetry_state_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = false;
  ptr->tRateSecs = tkr_mqtt->GetIfChangedPeriod_SubModule();
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mMoistureSensor::ConstructJSON_Sensor;
  telemetry_list.push_back(ptr);
  
}
#endif // USE_MODULE_NETWORK_MQTT



#endif
