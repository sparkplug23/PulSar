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

/**
 * Special method, which will use interrupt on pin change, to cause a new adc value recording
 * Test 1:
 *  - use physical manual pin change, to cause a adc reading from both adc_pins and display result on serial
 * 
 * 
 * */
#include "mADCInternal.h"

#ifdef USE_MODULE_SENSORS_ADC_INTERNAL

const char* mADCInternal::PM_MODULE_SENSORS_ADC_INTERNAL_CTR = D_MODULE_SENSORS_ADC_INTERNAL_CTR;
const char* mADCInternal::PM_MODULE_SENSORS_ADC_INTERNAL_FRIENDLY_CTR = D_MODULE_SENSORS_ADC_INTERNAL_FRIENDLY_CTR;

#ifdef ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER


const byte interruptPin = 25;
volatile int interruptCounter = 0;
// volatile double interruptExecuteTime = 0;
static int taskCore0 = 0;
static int taskCore1 = 1;
int numberOfInterrupts = 0;
uint16_t adc6 = 0;
uint16_t adc7 = 0;

#include <soc/sens_reg.h>
#include <soc/sens_struct.h>

  // uint16_t adc_value;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
RTC_DATA_ATTR uint16_t adcValue;
uint16_t _adcValue;
// uint16_t adcValue1;

uint16_t IRAM_ATTR adc1_get_raw_ram(adc1_channel_t channel) {
    SENS.sar_read_ctrl.sar1_dig_force = 0; // switch SARADC into RTC channel 
    SENS.sar_meas_wait2.force_xpd_sar = SENS_FORCE_XPD_SAR_PU; // adc_power_on
    // RTCIO.hall_sens.xpd_hall = false; //disable other peripherals
    SENS.sar_meas_wait2.force_xpd_amp = SENS_FORCE_XPD_AMP_PD; // channel is set in the convert function
    
	// disable FSM, it's only used by the LNA.
    SENS.sar_meas_ctrl.amp_rst_fb_fsm = 0; 
    SENS.sar_meas_ctrl.amp_short_ref_fsm = 0;
    SENS.sar_meas_ctrl.amp_short_ref_gnd_fsm = 0;
    SENS.sar_meas_wait1.sar_amp_wait1 = 1;
    SENS.sar_meas_wait1.sar_amp_wait2 = 1;
    SENS.sar_meas_wait2.sar_amp_wait3 = 1; 

    //set controller
    SENS.sar_read_ctrl.sar1_dig_force = false;      //RTC controller controls the ADC, not digital controller
    SENS.sar_meas_start1.meas1_start_force = true;  //RTC controller controls the ADC,not ulp coprocessor
    SENS.sar_meas_start1.sar1_en_pad_force = true;  //RTC controller controls the data port, not ulp coprocessor
    SENS.sar_touch_ctrl1.xpd_hall_force = true;     // RTC controller controls the hall sensor power,not ulp coprocessor
    SENS.sar_touch_ctrl1.hall_phase_force = true;   // RTC controller controls the hall sensor phase,not ulp coprocessor
    
    //start conversion
    SENS.sar_meas_start1.sar1_en_pad = (1 << channel); //only one channel is selected.
    while (SENS.sar_slave_addr1.meas_status != 0);
    SENS.sar_meas_start1.meas1_start_sar = 0;
    SENS.sar_meas_start1.meas1_start_sar = 1;
    while (SENS.sar_meas_start1.meas1_done_sar == 0);
    _adcValue = SENS.sar_meas_start1.meas1_data_sar; // set adc value!

    SENS.sar_meas_wait2.force_xpd_sar = SENS_FORCE_XPD_SAR_PD; // adc power off

    return _adcValue;
}




/************************************************************************************
******** External Pin Interrupt Triggers For ADC ************************************
 @note Helper functions, that need to be static. The singlton class instance allows setting
       a flag that is inside the class
       Hardcoded variables to make this faster for measurements
*************************************************************************************
*************************************************************************************/
void IRAM_ATTR ISR_External_Pin_ADC_Config_All_Trigger()
{
  DEBUG_ADC_ISR_EVENT_SET(LOW);
  pCONT_adc_internal->external_interrupt.flag_pin_active = true;

  /**
   * Capture both adc pins for 5 samples (no delay)
   * */
  mADCInternal::ISR_DUAL_CAPTURE* adc_p = &pCONT_adc_internal->isr_capture;

  if(adc_p->within_buffer_iter_counter < 40)
  {
    adc_p->adc_readings[adc_p->active_buffer_to_write_to_index].buffer_ch6[adc_p->within_buffer_iter_counter] = adc1_get_raw_ram(ADC1_CHANNEL_6);
    adc_p->adc_readings[adc_p->active_buffer_to_write_to_index].buffer_ch7[adc_p->within_buffer_iter_counter] = adc1_get_raw_ram(ADC1_CHANNEL_7);
    adc_p->within_buffer_iter_counter++;
  }

  DEBUG_ADC_ISR_EVENT_SET(HIGH);
}

#endif // ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER



int8_t mADCInternal::Tasker(uint8_t function, JsonParserObject obj)
{
  
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
    case FUNC_EVERY_SECOND:
    {

    }
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
    adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH6_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_6;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
    settings.fSensorCount++;
  }

  if(pCONT_pins->PinUsed(GPIO_ADC1_CH7_ID))
  {
    adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH7_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_7;
    adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
    adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
    adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
    pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
    settings.fSensorCount++;
  }

  // Special pin set here
  if(pCONT_pins->PinUsed(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID))
  {
    external_interrupt.trigger_pin = pCONT_pins->GetPin(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID);
    external_interrupt.flag_enabled = true;
  }
  
  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
  }

}


void mADCInternal::Init(void){


  #ifdef ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER
  if(external_interrupt.flag_enabled)
  {
    pinMode(external_interrupt.trigger_pin, INPUT);
    attachInterrupt(external_interrupt.trigger_pin, ISR_External_Pin_ADC_Config_All_Trigger, FALLING);
  }
  #endif // ENABLE_ADC_INTERNAL_PIN_INTERRUPT_ADC_TRIGGER

  // Configure all channel atten and width
  for(int i=0; i<settings.fSensorCount; i++)
  {
    switch(adc_config[i].input_pin)
    {
      // case 32:
      //   adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );
      //   adc1_config_width(ADC_WIDTH_BIT_12);
      //   AddLog(LOG_LEVEL_TEST, PSTR("ADC1_CHANNEL_4 set"));
      // break;
      case 34:
        adc1_config_channel_atten(ADC1_CHANNEL_6, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        AddLog(LOG_LEVEL_TEST, PSTR("ADC1_CHANNEL_6 set"));
      break;
      case 35:
        adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        AddLog(LOG_LEVEL_TEST, PSTR("ADC1_CHANNEL_7 set"));
      break;
    }

  }
  
}

void mADCInternal::EveryLoop(){
    
  if(external_interrupt.flag_pin_active)
  {
    external_interrupt.flag_pin_active = false;
    // Serial.println("external_interrupt.flag_pin_active");
    // Update_Channel1_ADC_Readings();
    // AddLog(LOG_LEVEL_TEST, PSTR("adc_levels = \t%d\t%d %d"), readings[0].adc_level, readings[1].adc_level, isr_capture.active_buffer_to_write_to_index);
  } 

  // if(isr_capture.active_buffer_to_write_to_index!=0){
  // AddLog(LOG_LEVEL_TEST, PSTR("isr_capture.active_buffer_to_write_to_index=%d"),isr_capture.active_buffer_to_write_to_index);
  // }

}


void mADCInternal::Update_Channel1_ADC_Readings()
{

  #ifdef ENABLE_SMOOTHING_ON_ADC_READING
  for(int i = 0;i<settings.fSensorCount; i++)
  {
    samples.clear();

    // collect samples from ADC
    while(samples.size() < readings[i].adcSampleCount_){
      samples.push_back(adc1_get_raw((adc1_channel_t)adc_config[i].channel_id));
      ets_delay_us(1); // too long!?
    }
    // average the collected samples
    readings[i].adc_level = (std::accumulate(samples.begin(), samples.end(), 0) / samples.size());
    // AddLog(LOG_LEVEL_TEST, PSTR("readings[%d].adc_level = %d"),i,readings[i].adc_level);

    readings[i].samples_between_resets++;

    // Add nice function that adds reading into memory
    if(readings[i].stored_values.index < STORED_VALUE_ADC_MAX){
      readings[i].stored_values.adc[readings[i].stored_values.index++] = readings[i].adc_level;
    }else{
      // AddLog(LOG_LEVEL_TEST, PSTR("readings[%d].stored_values.index = %d OVERFLOW"),i,readings[i].stored_values.index);
      // readings[i].stored_values.index = 0;
    }

  }
  #else
    readings[0].adc_level = adc1_get_raw(ADC1_CHANNEL_6);
    ets_delay_us(1);
    readings[1].adc_level = adc1_get_raw(ADC1_CHANNEL_7);
    AddLog(LOG_LEVEL_TEST, PSTR("adc_level = \t%d\t%d"),readings[0].adc_level,readings[1].adc_level);
  #endif

}


uint8_t mADCInternal::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_SENSOR_COUNT, settings.fSensorCount);
  return JsonBuilderI->End();

}

uint8_t mADCInternal::ConstructJSON_Sensor(uint8_t json_level){

  JsonBuilderI->Start();

  char buffer[50];

  
    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_2,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);

    // AddLog(LOG_LEVEL_TEST, PSTR("adc = [%d,%d,%d,%d,%d,%d,%d,%d]"),
    //   adc1_get_raw(ADC1_CHANNEL_0),
    //   adc1_get_raw(ADC1_CHANNEL_1),
    //   adc1_get_raw(ADC1_CHANNEL_2),
    //   adc1_get_raw(ADC1_CHANNEL_3),
    //   adc1_get_raw(ADC1_CHANNEL_4),
    //   adc1_get_raw(ADC1_CHANNEL_5),
    //   adc1_get_raw(ADC1_CHANNEL_6),
    //   adc1_get_raw(ADC1_CHANNEL_7)
    // );

    // Update_Channel1_ADC_Readings();
    
  JBI->Array_Start("chADC1");
  for(int i=0;i<2;i++){
    JBI->Add(readings[i].adc_level);
  }
  JBI->Array_End();

  // JBI->Array_Start("ADC1");
  // for(int i=0;i<8;i++){
  //   JBI->Add(adc1_get_raw((adc1_channel_t)i));
  // }
  // JBI->Array_End();

  JBI->Array_Start("stored_values.index");
  for(int i=0;i<2;i++){
    JBI->Add(readings[0].stored_values.index);
  }
  JBI->Array_End();
  JBI->Array_Start("samples_between_resets");
  for(int i=0;i<2;i++){
    JBI->Add(readings[i].samples_between_resets);
  }
  JBI->Array_End();

  uint16_t send_size = 0;
  send_size = 10; //STORED_VALUE_ADC_MAX

  JBI->Array_Start("adc0");
  for(int i=0;i<send_size;i++){
    JBI->Add(readings[0].stored_values.adc[i]);
  }
  JBI->Array_End();
  JBI->Array_Start("adc1");
  for(int i=0;i<send_size;i++){
    JBI->Add(readings[1].stored_values.adc[i]);
  }
  JBI->Array_End();


  


  
  return JsonBuilderI->End();

}


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





    // readings[0].adc_level = adc1_get_raw(ADC1_CHANNEL_6);
    // ets_delay_us(1);
    // readings[1].adc_level = adc1_get_raw(ADC1_CHANNEL_7);



    // AddLog(LOG_LEVEL_TEST, PSTR("adc_level = \t%d\t%d"),readings[0].adc_level,readings[1].adc_level);


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // int val = adc1_get_raw(ADC1_CHANNEL_4);
    // printf("4val%d\n", val );


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);
    // val = adc1_get_raw(ADC1_CHANNEL_7);
    // printf("7val%d\n", val );


    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // val = adc1_get_raw(ADC1_CHANNEL_6);
    // printf("6val%d\n", val );

    // adc1_config_width(ADC_WIDTH_BIT_12);
    // adc1_config_channel_atten(ADC1_CHANNEL_0,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_1,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_2,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_3,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_4,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_5,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_6,ADC_ATTEN_DB_0);
    // adc1_config_channel_atten(ADC1_CHANNEL_7,ADC_ATTEN_DB_0);

    // AddLog(LOG_LEVEL_TEST, PSTR("adc = [%d,%d,%d,%d,%d,%d,%d,%d]"),
    //   adc1_get_raw(ADC1_CHANNEL_0),
    //   adc1_get_raw(ADC1_CHANNEL_1),
    //   adc1_get_raw(ADC1_CHANNEL_2),
    //   adc1_get_raw(ADC1_CHANNEL_3),
    //   adc1_get_raw(ADC1_CHANNEL_4),
    //   adc1_get_raw(ADC1_CHANNEL_5),
    //   adc1_get_raw(ADC1_CHANNEL_6),
    //   adc1_get_raw(ADC1_CHANNEL_7)
    // );


    //   int read_raw;
    //   adc2_config_channel_atten( ADC2_CHANNEL_6, ADC_ATTEN_0db );

    //   esp_err_t r = adc2_get_raw( ADC2_CHANNEL_6, ADC_WIDTH_12Bit, &read_raw);
    //   if ( r == ESP_OK ) {
    //       printf("%d\n", read_raw );
    //   } else if ( r == ESP_ERR_TIMEOUT ) {
    //       printf("ADC2 used by Wi-Fi.\n");
    //   }

