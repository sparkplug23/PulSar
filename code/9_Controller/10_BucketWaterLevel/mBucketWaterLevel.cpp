#include "mBucketWaterLevel.h"

#ifdef USE_MODULE_CONTROLLER_BUCKET_WATER_LEVEL

const char* mBucketWaterLevel::PM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_CTR = D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_CTR;
const char* mBucketWaterLevel::PM_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_FRIENDLY_CTR = D_MODULE_CONTROLLER_BUCKET_WATER_LEVEL_FRIENDLY_CTR;

int8_t mBucketWaterLevel::Tasker(uint8_t function, JsonParserObject obj)
{


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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: 
      EveryLoop();
    break;  
    case FUNC_EVERY_HOUR:
    MeasureADCWithRelay();
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case FUNC_JSON_COMMAND_ID:
      parse_JSONCommand(obj);
    break;
    /************
     * MQTT SECTION * 
    *******************/
    #ifdef USE_MODULE_NETWORK_MQTT
    case FUNC_MQTT_HANDLERS_INIT:
      MQTTHandler_Init();
    break;
    case FUNC_MQTT_HANDLERS_SET_DEFAULT_TRANSMIT_PERIOD:
      MQTTHandler_Set_DefaultPeriodRate();
    break;
    case FUNC_MQTT_SENDER:
      MQTTHandler_Sender();
    break;
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_RefreshAll();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }

}


void mBucketWaterLevel::Pre_Init(void)
{

  settings.fEnableSensor = true;

  settings.fSensorCount = 0;

  if(pCONT_pins->PinUsed(GPIO_ADC1_CH4_ID))
  {
    adc_config[settings.fSensorCount].input_pin = pCONT_pins->GetPin(GPIO_ADC1_CH4_ID);
    adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
    adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_4;
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

  if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_INTERNAL Sensor Enabled"));
  }



}


void mBucketWaterLevel::Init(void)
{
  
  // Configure all channel atten and width
  for(int i=0; i<settings.fSensorCount; i++)
  {
    switch(adc_config[i].input_pin)
    {
      case 32:
        adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );
        adc1_config_width(ADC_WIDTH_BIT_12);
        AddLog(LOG_LEVEL_TEST, PSTR("ADC1_CHANNEL_4 set"));
      break;
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


void mBucketWaterLevel::EveryLoop()
{

if(pCONT_mry->CommandGet_Relay_Power(0))
{
  adc_values.adc1 = adc1_get_raw(ADC1_CHANNEL_4);
  
 }
 
  // adc_values.esp32.adc5 = adc1_get_raw(ADC1_CHANNEL_7);
  // JBI->Add("e2", adc_values.esp32.adc2);
  // JBI->Add("e5", adc_values.esp32.adc5);




}

void mBucketWaterLevel::MeasureADCWithRelay()
{

  /**
   * Relay on
   * */

  pCONT_mry->CommandSet_Relay_Power(1);

  delay(1000);

  // adc_values.adc1 = analogRead(A0);

  adc_values.adc1 = adc1_get_raw(ADC1_CHANNEL_4);

  // Serial.println(adc_level);

  

  if(!IsWithinLimits((uint16_t)500, adc_values.adc1, (uint16_t)4095))
  {
    return; // error values
  }

  // search for adc

  float mapped_litres = 0;

  uint16_t adc_now = adc_values.adc1;
  adc_upper_boundary_index = 0;
  for (int i=0;i<ARRAY_SIZE(adc_raw_calibration_pairs_readings_adc)-1;i++)
  {
    // Search "down" the list until now is less than list
    if(IsWithinLimits((uint16_t)adc_raw_calibration_pairs_readings_adc[i+1], adc_now, (uint16_t)adc_raw_calibration_pairs_readings_adc[i]))
    {
      adc_upper_boundary_index = i;
      break;
    }
  }

  // first index
  if(adc_now > 4095)
  {
    // AddLog(LOG_LEVEL_TEST, PSTR("adc_now >  adc_now %d"), adc_now);
    // cant be
    adc_values.litres = 0;
  }
  else
  if(IsWithinLimits((uint16_t)1000, adc_now, (uint16_t)4095)) // within mapped range
  {

    // AddLog(LOG_LEVEL_TEST, PSTR("WithinLimits adc_now %d"), adc_now);
    if(adc_upper_boundary_index < ARRAY_SIZE(adc_raw_calibration_pairs_readings_adc)-1)
    {
      adc_values.litres = mapvalue(
        adc_now,
        adc_raw_calibration_pairs_readings_adc[adc_upper_boundary_index],
        adc_raw_calibration_pairs_readings_adc[adc_upper_boundary_index+1],
        adc_raw_calibration_pairs_readings_milliletres[adc_upper_boundary_index],
        adc_raw_calibration_pairs_readings_milliletres[adc_upper_boundary_index+1]   
      );
    }

  }
  else // lower off scale ie "full"
  {      
    // AddLog(LOG_LEVEL_TEST, PSTR("ELSE adc_now %d"), adc_now);

    adc_values.litres = mapvalue(
        adc_now,
        1000, 0,
        2200, 2300
        );
  }


  pCONT_mry->CommandSet_Relay_Power(0);
  mqtthandler_state_ifchanged.flags.SendNow = true;







}


uint8_t mBucketWaterLevel::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  JBI->Start();
  

  return JBI->End();
}


uint8_t mBucketWaterLevel::ConstructJSON_State(uint8_t json_level)
{
  char buffer[100];

  JBI->Start();
    if(adc_values.adc1 != 0)
    {
    JBI->Add("adc_raw", adc_values.adc1);
    }
    JBI->Add("litres", adc_values.litres);
    JBI->Add("adc_upper_boundary_index", adc_upper_boundary_index);
    if(adc_values.litres != 0)
    {
    JBI->Add("litres_non_zero", adc_values.litres);
    }
  return JBI->End();
}


#endif
