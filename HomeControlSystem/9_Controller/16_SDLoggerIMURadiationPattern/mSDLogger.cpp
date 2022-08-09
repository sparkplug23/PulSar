#include "mSDLogger.h"

#ifdef USE_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN

const char* mSDLoggerIMURadiationPattern::PM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_CTR = D_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_CTR;
const char* mSDLoggerIMURadiationPattern::PM_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_FRIENDLY_CTR = D_MODULE_CONTROLLER_SDLOGGER_IMU_RADIATIONPATTERN_FRIENDLY_CTR;


int8_t mSDLoggerIMURadiationPattern::Tasker(uint8_t function, JsonParserObject obj)
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

  if(!settings.fEnableSensor){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP: {
      EveryLoop();

      // uint16_t adc_level_1 = adc1_get_raw(ADC1_CHANNEL_6);
      // // ets_delay_us(1);
      // uint16_t adc_level_2 = adc1_get_raw(ADC1_CHANNEL_7);
      // AddLog(LOG_LEVEL_TEST, PSTR("adc_level = \t%d\t%d"), adc_level_1, adc_level_2);

    }break;  
    case FUNC_EVERY_SECOND: 
      EverySecond();
    break;
    /************
     * TRIGGERS SECTION * 
    *******************/
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      #ifdef USE_MODULE_DRIVERS_SDCARD
      pCONT_sdcard->CommandSet_SDCard_Appending_File_Method_State(2);
      #endif
      sequence_test = 0;
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



void mSDLoggerIMURadiationPattern::UpdateInternalRTCTimeWithGPSTime()
{

  /**
   * Only update the time IF the sdcard is not open already and being written to
   * */
  #if defined(USE_MODULE_DRIVERS_GPS) && defined(USE_SYSTEM_SDCARD_LOGGING)
  if(pCONT_sdcard->sdcard_status.isopened==0)
  {

    pCONT_time->SetUTCTime(
      pCONT_gps->gps_result_stored.dateTime.year,
      pCONT_gps->gps_result_stored.dateTime.month,
      pCONT_gps->gps_result_stored.dateTime.day,
      pCONT_gps->gps_result_stored.dateTime.hours,
      pCONT_gps->gps_result_stored.dateTime.minutes,
      pCONT_gps->gps_result_stored.dateTime.seconds
    );

  }
  #endif // gps & sdcard

}

void mSDLoggerIMURadiationPattern::Pre_Init(void)
{

  settings.fEnableSensor = false;
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

  #ifdef ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING
  pinMode(GPIO_FUNCTION_MANUAL_ENABLE_SAMPLING_NUMBER, INPUT_PULLUP);
  #endif
  #ifdef GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER
  pinMode(GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER, INPUT_PULLUP);
  #endif

}


void mSDLoggerIMURadiationPattern::Init(void)
{

  #ifdef DISABLE_NETWORK
  WiFi.mode(WIFI_OFF);
  btStop();  
  #endif // DISABLE_NETWORK
  
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


void mSDLoggerIMURadiationPattern::EveryLoop()
{

  // The freq the Construct is called will be the sample rate, lets try 100ms (ie 10 per second)
  if(mTime::TimeReached(&tSaved_SampleAndSave, 20))
  {

    ConstructJSON_SDCardSuperFrame();

    /**
     * Append to sdcard stream
     * */
    #ifdef USE_MODULE_DRIVERS_SDCARD
      #ifdef ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING
      if(digitalRead(GPIO_FUNCTION_MANUAL_ENABLE_SAMPLING_NUMBER)==0)
      {
      #endif
        pCONT_sdcard->AppendRingBuffer(JBI->GetPtr(), JBI->GetLength());
      #ifdef ENABLE_DEVFEATURE_MANUAL_ENABLE_SAMPLING
      }
      #endif
      // AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream >> [%d] \"%s\""), JBI->GetLength(), JBI->GetPtr());
    #else
      AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream >> [%d] \"%s\""), JBI->GetLength(), JBI->GetPtr());
    #endif //USE_MODULE_DRIVERS_SDCARD

  }



}


void mSDLoggerIMURadiationPattern::EverySecond()
{

  SubTask_UpdateOLED();

}


/**
 * @brief Show logger status on OLED
 * @note 4x11 chars at font 1
 * 
 * "Open xxxxxx" where x is the time opened
 * "Byte bbbbbb" where b is the number of bytes written (changing to kB when >1000)
 * 
 * "ADC2 ADC2"  %04d of adc readings
 * "HHMMSS     " where HHMMSS is the GPS RTC time, which is also appended to the opened file name
 * */
void mSDLoggerIMURadiationPattern::SubTask_UpdateOLED()
{

#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  char line_ctr[15] = {0};
  char buffer[15] = {0};

      float heading;
      float roll;
      float pitch;
  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

  /**
   * Row 1
   * adc 
   * */
  #ifdef USE_DEVFEATURE_ADC_IN_CONTROLLER
  snprintf(line_ctr, sizeof(line_ctr), "%04d %04d%c",
    adc_values.adc2, 
    adc_values.adc5,digitalRead(GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER)?'N':'S'
  );
  pCONT_iDisp->LogBuffer_AddRow(line_ctr, 0);
  #endif // USE_DEVFEATURE_ADC_IN_CONTROLLER

  #ifdef USE_MODULE_DRIVERS_SDCARD
  snprintf(buffer, sizeof(buffer), "%c%s%s",
    pCONT_sdcard->sdcard_status.init_error_on_boot ? 'E' : 'f',
    pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID ?"OPEN!":"cd",
    &pCONT_sdcard->writer_settings.file_name[8] //skipping "APPEND_" to get just time
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);
  #endif //USE_MODULE_DRIVERS_SDCARD
  
  #ifdef USE_MODULE_DRIVERS_SDCARD
  uint32_t bytes_written = pCONT_sdcard->sdcard_status.bytes_written_to_file;
  char unit_type = 'B';

  if(bytes_written>50000)
  {
    bytes_written /= 1000; //into kB
    unit_type = 'k';
  }

      pCONT_LSM303D->CalculateOrientation(
        pCONT_LSM303D->sensor[0].lsm303d->a.x, pCONT_LSM303D->sensor[0].lsm303d->a.y, pCONT_LSM303D->sensor[0].lsm303d->a.z, 
        pCONT_LSM303D->sensor[0].lsm303d->m.x, pCONT_LSM303D->sensor[0].lsm303d->m.y, pCONT_LSM303D->sensor[0].lsm303d->m.z, 
        &heading, &roll, &pitch
      );

  snprintf(buffer, sizeof(buffer), "%d %c H%d",bytes_written,unit_type, (int)heading);
  
  pCONT_iDisp->LogBuffer_AddRow(buffer,2);
  #endif // USE_MODULE_DRIVERS_SDCARD

  /**
   * Show magnetometer as example, x-axis value
   * */
  #ifdef USE_MODULE_SENSORS_LSM303D

// pCONT_LSM303D->sensor[0].lsm303d
//   heading


  snprintf(buffer, sizeof(buffer), "R%d P:%d", (int)roll, (int)pitch);
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #else // USE_MODULE_SENSORS_LSM303D
  snprintf(buffer, sizeof(buffer), "LSM303D d/c");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #endif // USE_MODULE_SENSORS_LSM303D

  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}





uint8_t mSDLoggerIMURadiationPattern::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mSDLoggerIMURadiationPattern::ConstructJSON_Sensor(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_VOLTAGE, 0);
  return JsonBuilderI->End();
    
}


/**
 * @brief Used to generate the output to be written to the SD card
 * @note
 * {"SF":[array RSS superframe ie 50 samples... CC_state, RSS2, RRS5.... for 50],
 *  "GPS":{ minimal data, with 2 byte names}
 * }
 * */
uint8_t mSDLoggerIMURadiationPattern::ConstructJSON_SDCardSuperFrame(uint8_t json_method){

  JsonBuilderI->Start();
    
    /**
     * on first sequence number, send additional useful info
     * */
    // if(sequence_test == 0)
    // {
    //   JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
    // }

    // JBI->Add("N", sequence_test++);

    // Debug data only
    JBI->Add("M",millis());
    JBI->Add("S", (uint8_t)digitalRead(GPIO_FUNCTION_MANUAL_CC1110_IS_RECEIVING_PACKETS_NUMBER));

    #if defined(USE_MODULE_SENSORS_LSM303D) || defined(USE_MODULE_SENSORS_L3G)
    // Force update of sensor here

    JBI->Level_Start("IA"); // 9 axis gyro

      #ifdef USE_MODULE_SENSORS_LSM303D
      pCONT_LSM303D->sensor[0].lsm303d->read();
      JBI->Level_Start("M");
        JBI->Add("x", pCONT_LSM303D->sensor[0].lsm303d->m.x);
        JBI->Add("y", pCONT_LSM303D->sensor[0].lsm303d->m.y);
        JBI->Add("z", pCONT_LSM303D->sensor[0].lsm303d->m.z);
      JBI->Level_End();
      JBI->Level_Start("A");
        JBI->Add("x", pCONT_LSM303D->sensor[0].lsm303d->a.x);
        JBI->Add("y", pCONT_LSM303D->sensor[0].lsm303d->a.y);
        JBI->Add("z", pCONT_LSM303D->sensor[0].lsm303d->a.z);
      JBI->Level_End();
      
      float heading;
      float roll;
      float pitch;
      pCONT_LSM303D->CalculateOrientation(
        pCONT_LSM303D->sensor[0].lsm303d->a.x, pCONT_LSM303D->sensor[0].lsm303d->a.y, pCONT_LSM303D->sensor[0].lsm303d->a.z, 
        pCONT_LSM303D->sensor[0].lsm303d->m.x, pCONT_LSM303D->sensor[0].lsm303d->m.y, pCONT_LSM303D->sensor[0].lsm303d->m.z, 
        &heading, &roll, &pitch
      );

      JBI->Level_Start("O");
        JBI->Add("H", heading);
        JBI->Add("R", roll);
        JBI->Add("P", pitch);
      JBI->Level_End();

      #endif
      #ifdef USE_MODULE_SENSORS_L3G
      pCONT_L3G->gyro->read();
      JBI->Level_Start("G");
        JBI->Add("x", pCONT_L3G->gyro->g.x);
        JBI->Add("y", pCONT_L3G->gyro->g.y);
        JBI->Add("z", pCONT_L3G->gyro->g.z);
      JBI->Level_End();
      #endif
    
    
    JBI->Level_End();
    
    JBI->Level_Start("IL"); // 6 axis -- leg
      #ifdef USE_MODULE_SENSORS_LSM303D
      pCONT_LSM303D->sensor[1].lsm303d->read();
      JBI->Level_Start("M");
        JBI->Add("x", pCONT_LSM303D->sensor[1].lsm303d->m.x);
        JBI->Add("y", pCONT_LSM303D->sensor[1].lsm303d->m.y);
        JBI->Add("z", pCONT_LSM303D->sensor[1].lsm303d->m.z);
      JBI->Level_End();
      JBI->Level_Start("A");
        JBI->Add("x", pCONT_LSM303D->sensor[1].lsm303d->a.x);
        JBI->Add("y", pCONT_LSM303D->sensor[1].lsm303d->a.y);
        JBI->Add("z", pCONT_LSM303D->sensor[1].lsm303d->a.z);
      JBI->Level_End();
      
      pCONT_LSM303D->CalculateOrientation(
        pCONT_LSM303D->sensor[1].lsm303d->a.x, pCONT_LSM303D->sensor[1].lsm303d->a.y, pCONT_LSM303D->sensor[1].lsm303d->a.z, 
        pCONT_LSM303D->sensor[1].lsm303d->m.x, pCONT_LSM303D->sensor[1].lsm303d->m.y, pCONT_LSM303D->sensor[1].lsm303d->m.z, 
        &heading, &roll, &pitch
      );

      JBI->Level_Start("O");
        JBI->Add("H", heading);
        JBI->Add("R", roll);
        JBI->Add("P", pitch);
      JBI->Level_End();

      #endif    
    
    JBI->Level_End();


    #endif // USE_MODULE_SENSORS_LSM303D


    JBI->Level_Start("ADC");
      adc_values.adc2 = adc1_get_raw(ADC1_CHANNEL_4);
      adc_values.adc5 = adc1_get_raw(ADC1_CHANNEL_7);
      JBI->Add("2", adc_values.adc2);
      JBI->Add("5", adc_values.adc5);
    JBI->Level_End();

  return JsonBuilderI->End();
    
}

#endif
