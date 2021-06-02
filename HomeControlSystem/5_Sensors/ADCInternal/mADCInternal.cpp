/*
  mADCInternal.cpp
  Description: Uses the internal ADC for fast measurements of A0 on ESP8266, and all ESP32 channels

  Copyright (C) 2021  Michael

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "mADCInternal.h"

#ifdef USE_MODULE_SENSORS_ADC_INTERNAL

const char* mADCInternal::PM_MODULE_SENSORS_ADC_INTERNAL_CTR = D_MODULE_SENSORS_ADC_INTERNAL_CTR;
const char* mADCInternal::PM_MODULE_SENSORS_ADC_INTERNAL_FRIENDLY_CTR = D_MODULE_SENSORS_ADC_INTERNAL_FRIENDLY_CTR;


bool set_flag = false;

int8_t mADCInternal::Tasker(uint8_t function, JsonParserObject obj){
  
  switch(function){
    case FUNC_PRE_INIT:
      Pre_Init();
    break;
    case FUNC_INIT:
      Init();
    break;
  }

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;   
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
    //  parse_JSONCommand(obj);
    break;
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

  return FUNCTION_RESULT_SUCCESS_ID;

}


void mADCInternal::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 0;

  if(pCONT_pins->PinUsed(GPIO_ADC1_CH6_ID))
  {
    adc_config[settings.fSensorCount].pin = pCONT_pins->GetPin(GPIO_ADC1_CH6_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_6;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    settings.fSensorCount++;
  }

  if(pCONT_pins->PinUsed(GPIO_ADC1_CH7_ID))
  {
    adc_config[settings.fSensorCount].pin = pCONT_pins->GetPin(GPIO_ADC1_CH7_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_7;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    settings.fSensorCount++;
  }
  
  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
  }

}


/************************************************************************************
******** External Pin Interrupt Triggers For ADC ************************************
 @note Helper functions, that need to be static. The singlton class instance allows setting
       a flag that is inside the class
*************************************************************************************
*************************************************************************************/
void IRAM_ATTR ISR_External_Pin_ADC_Config0_Trigger()
{
  pCONT_adc_internal->adc_config[0].flag_external_interrupt_triggered_reading = true;
}

void IRAM_ATTR ISR_External_Pin_ADC_Config1_Trigger()
{
  pCONT_adc_internal->adc_config[0].flag_external_interrupt_triggered_reading = true;
}




void mADCInternal::Init(void){

// pinMode(12, INPUT);

      // Start the button monitoring passing in the helper function.  You COULD make this a lambda here, BUT I found that the lambda not being "IRAM" would periodically crash the device.
    // buttonExternal.
    Start(ISR_External_Pin_ADC_Config0_Trigger);



  // for (int sensor_id=0;sensor_id<MAX_SENSORS;sensor_id++){    
  //   sensor[sensor_id].tSavedMeasureClimate = millis();
  //   sensor[sensor_id].sReadSensor = SPLIT_TASK_SEC1_ID;    
  // }

  adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_11db );
  adc1_config_width(ADC_WIDTH_12Bit);

  adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_11db );
  adc1_config_width(ADC_WIDTH_12Bit);

  settings.measure_rate_ms = 1000;

  attachInterrupt(
    22, 
    ISR_TriggerGPIO_ADC_Record,
    FALLING
  );

  
}

void mADCInternal::Start(void (*ISR_callback)(void)) {
    pinMode(this->interruptPin, INPUT_PULLUP);
    attachInterrupt(this->interruptPin, ISR_callback, FALLING);
}



static void IRAM_ATTR ISR_TriggerGPIO_ADC_Record()
{
  // button1.numberKeyPresses += 1;
  // button1.pressed = true;
}


// Optional, add that another GPIO is interrupt on low to trigger ADC capture


void mADCInternal::EveryLoop(){
    

  if(mTime::TimeReached(&tSaved_adctest, 1000))
  {

      Serial.print(adc1_get_raw(ADC1_CHANNEL_7));
      Serial.print("===========");

  std::vector<int> samples;
  uint8_t channel_ = ADC1_CHANNEL_7;
  float lastReading_ = 0;
  uint8_t adcSampleCount_ = 10;

    // collect samples from ADC
    while(samples.size() < adcSampleCount_)
    {
      samples.push_back(adc1_get_raw(ADC1_CHANNEL_7));
      ets_delay_us(1);
    }
    // average the collected samples
    lastReading_ = (std::accumulate(samples.begin(), samples.end(), 0) / samples.size());

    Serial.println(lastReading_);

  }


  if(adc_config[0].flag_external_interrupt_triggered_reading)
  {
    adc_config[0].flag_external_interrupt_triggered_reading = false;
    Serial.println("adc_config[0].flag_external_interrupt_triggered_reading");
  }
  
  if(adc_config[1].flag_external_interrupt_triggered_reading)
  {
    adc_config[1].flag_external_interrupt_triggered_reading = false;
    Serial.println("adc_config[1].flag_external_interrupt_triggered_reading");
  }
  

}




uint8_t mADCInternal::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}

uint8_t mADCInternal::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();

  char buffer[50];

  // for(uint8_t sensor_id = 0;sensor_id<MAX_SENSORS;sensor_id++){
  //   if(sensor[sensor_id].ischanged_over_threshold || (json_level>JSON_LEVEL_IFCHANGED)){
  //     JsonBuilderI->Level_Start_P(DLI->GetDeviceNameWithEnumNumber(EM_MODULE_SENSORS_ADC_INTERNAL_ID,sensor_id,buffer,sizeof(buffer)));   
  //       JsonBuilderI->Add(D_JSON_TEMPERATURE, sensor[sensor_id].temperature);
  //       JsonBuilderI->Add(D_JSON_HUMIDITY, sensor[sensor_id].humidity);
  //       JsonBuilderI->Add(D_JSON_PRESSURE, sensor[sensor_id].pressure);
  //       JsonBuilderI->Add(D_JSON_ALTITUDE, sensor[sensor_id].altitude);
  //       JsonBuilderI->Level_Start(D_JSON_ISCHANGEDMETHOD);
  //         JsonBuilderI->Add(D_JSON_TYPE, D_JSON_SIGNIFICANTLY);
  //         JsonBuilderI->Add(D_JSON_AGE, (uint16_t)round(abs(millis()-sensor[sensor_id].ischangedtLast)/1000));
  //       JsonBuilderI->Level_End();  
  //     JsonBuilderI->Level_End();
  //   }
  // }
  
  return JsonBuilderI->End();

}


/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

#ifdef USE_MODULE_NETWORK_MQTT

void mADCInternal::MQTTHandler_Init(){

  struct handler<mADCInternal>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Settings;

  ptr = &mqtthandler_sensor_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.teleperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Sensor;

  ptr = &mqtthandler_sensor_ifchanged;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.ifchanged_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADCInternal::ConstructJSON_Sensor;
  
} //end "MQTTHandler_Init"



/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mADCInternal::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mADCInternal::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mADCInternal::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_SENSORS_ADC_INTERNAL_ID, handle, id);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif
