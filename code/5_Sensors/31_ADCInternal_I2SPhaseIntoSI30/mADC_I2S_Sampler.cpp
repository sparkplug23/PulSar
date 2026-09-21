/**
 * Special method, which will use interrupt on pin change, to cause a new adc value recording
 * Test 1:
 *  - use physical manual pin change, to cause a adc reading from both adc_pins and display result on serial
 * 
 * 
 * Method 2:
 *  - record 10 times faster than sample rate of 6khz, so 60,000 hz
 *  - Test 1: for text, average the entire measurements so I can see what the sync window is
 * 
 * 
 * */
#include "mADC_I2S_Sampler.h"

#ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL

/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
****** RXON_TIMESLOT_ENABLED = Where the pic32 RXON sampling timeslot is happening *****************************************************************************************************************************************************************
******
* @note - This will be set during the sampling window of the pic32 timeslot, and will be recorded during this period
On each started/low, a RXON_SAMPLE_COUNTER will be incremented so all samples in item will be known which slot it comes from
Instead of using a second buffer, instead, 2 bytes of the DMA read will also have this number appended to it (bytes 1001 and 1002), these will be extracted during json format and thus not during matlab



*****************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/





int8_t mADC_I2S_Sampler::Tasker(uint8_t function, JsonParserObject obj)
{
  
  switch(function){
    case TASK_PRE_INIT:
      Pre_Init();
    break;
    case TASK_INIT:
      Init();
    break;
  }

  // if(!settings.fEnableSensor){ return TASKER_RESULT__MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case TASK_LOOP: 
      // EveryLoop();





      SendADCReadingsSerial();
    break;   
    case TASK_EVERY_SECOND:
    {
        // Serial.println(tkr_adc_internal->adc_reading_1);

    }
    break;
    /************
     * COMMANDS SECTION * 
    *******************/
    case TASK_JSON_COMMAND_ID:
    //  parse_JSONCommand(obj);
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

  return TASKER_RESULT__SUCCESS_ID;

}


void mADC_I2S_Sampler::Pre_Init(){

  settings.fEnableSensor = false;
  settings.fSensorCount = 1;

  // if(tkr_pins->PinUsed(GPIO_ADC1_CH6_ID))
  // {
  //   adc_config[settings.fSensorCount].input_pin = tkr_pins->GetPin(GPIO_ADC1_CH6_ID);
  //   adc_config[settings.fSensorCount].channel_group = ADC_CHANNEL_GROUP_1_ID;
  //   adc_config[settings.fSensorCount].channel_id = ADC_CHANNEL_6;
  //   adc_config[settings.fSensorCount].attentuation_db_level = ADC_ATTEN_DB_11;
  //   adc_config[settings.fSensorCount].adc_width_bit = ADC_WIDTH_BIT_12;
  //   adc_config[settings.fSensorCount].mode = ADC_MODE_EXTERNAL_INTERRUPT_TRIGGERED_ID;
  //   pinMode(adc_config[settings.fSensorCount].input_pin, INPUT);
  //   AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "adc_config[%d].input_pin=%d"),settings.fSensorCount,adc_config[settings.fSensorCount].input_pin);
  //   settings.fSensorCount++;
  // }

  // // Special pin set here
  // if(tkr_pins->PinUsed(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID))
  // {
  //   external_interrupt.trigger_pin = tkr_pins->GetPin(GPIO_ADC1_EXTERNAL_INTERRUPT_TRIGGER_ID);
  //   external_interrupt.flag_enabled = true;
  // }
  
  // if(settings.fSensorCount){
    settings.fEnableSensor = true;
    AddLog(LOG_LEVEL_INFO,PSTR(D_LOG_DHT "ADC_I2S_INTERNAL Sensor Enabled"));
  // }

}



#ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT

void IRAM_ATTR ISR_External_Pin_RXON_Sampling_Timeslot_Event_Trigger()
{
  // tkr_serial_pos_log->sync_frame_data.flag_pin_active = true;
  // // tkr_adc_internal->adc_config[1].flag_external_interrupt_triggered_reading = true;
  // if(digitalRead(GPIO_SYNC_FRAME_ISR_PIN)==LOW)
  // {
  //   tkr_serial_pos_log->sync_frame_data.flag_started = true;

    // Serial.println("ISR_External_Pin_Sync_Frame_Status_Event_Trigger");

    /**
     * Swap between ringbuffers for internal-ADC
     * */
    #ifdef USE_MODULE_SENSORS_ADC_I2S_INTERNAL
    // tkr_adc_internal->adcSampler1->
    // SwapReaderWritersRingbuffers();   

    //extern_flag_swap_ringbuffers_before_writting_is2_data = true;

    
  // if(!adcSampler1->flag_sampler_initialized){
  // std::swap(adcSampler1->ringbuffer_handle_writer_ptr, adcSampler1->ringbuffer_handle_reader_ptr);
  // adcSampler1->active_ringbuffer_writting_index ^= 1;


    #endif

    
    // tkr_serial_pos_log->rxon_counter++;
    // tkr_adc_internal->adcSampler1->item_id_counter++;
    // Serial.printf("rxon_counter=%d\n\r",tkr_serial_pos_log->rxon_counter);
      
  //   /**
  //    * Swap between ringbuffers for internal-ADC
  //    * */
  //   tkr_adc_internal->SwapReaderWritersRingbuffers();    

  //   #ifdef ENABLE_ESP32_ADC_SAMPLING
  //   /**
  //    * toggle to the other buffer to be writting into, the read will check which is not active
  //    * */
  //   tkr_adc_internal->isr_capture.active_buffer_to_write_to_index ^= 1; // Reset ADC syncframe index
  //   /**
  //    * Use new buffer set here, to reset its counter
  //    * The other buffer not being written into, the counter values here wont be reset until the next ISR, allowing it to be checked for length of data on previous frame
  //    * */
  //     tkr_adc_internal->isr_capture.within_buffer_iter_counter = 0;
  //   #endif // ENABLE_ESP32_ADC_SAMPLING

  // }
  // else
  // {
  //   tkr_serial_pos_log->sync_frame_data.flag_ended = true;
  // }
}
#endif // ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT




void mADC_I2S_Sampler::Init(void){
  
  // i2s config for using the internal ADC
  i2s_config_t adcI2SConfig1 = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate = SAMPLE_RATE_I2S_RUNNING_FREQUENCY, //60khz
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_LSB,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    /**
     * 4 DMA buffers, each of size 1024. 
     * This allows 4 buffers to be queued (4096) then pushed into the ringbuffer
     * */
    .dma_buf_count = 6,
    .dma_buf_len = 1000, //1024 max
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  
  Serial2.begin(2048000);

  // #ifdef PIN_GPIO_RXON_SAMPLING_ENABLED_NUM
  //   pinMode(PIN_GPIO_RXON_SAMPLING_ENABLED_NUM, INPUT);
  // #endif

  // input from analog microphones such as the MAX9814 or MAX4466
  // internal analog to digital converter sampling using i2s create our samplers
  adcSampler1 = new ADCSampler(ADC_UNIT_1, ADC1_CHANNEL_7);

  /**
   * Lets not use the writer, instead a flag inside the while loop
   * no, I need the writer to push.. no.. its pushed in sampler
   * */
  TaskHandle_t adcWriterTaskHandle1;
  // xTaskCreatePinnedToCore(adcWriterTask, "ADC Writer Task", 4096, adcSampler1, 1, &adcWriterTaskHandle, 0);

  /**
   * Start reader
   * */
  adcSampler1->start(I2S_NUM_0, adcI2SConfig1, 16384, adcWriterTaskHandle1);
  
  // Serial.printf("\n\rREBOOTING\n\r\n\r");
  Serial2.printf("\n\rREBOOTING mADC_I2S_Sampler %d\n\r\n\r",random(1,1000));

  WiFi.mode(WIFI_OFF);
  btStop();

  #ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT
    pinMode(PIN_GPIO_RXON_SAMPLING_ENABLED_NUM, INPUT_PULLUP );
    attachInterrupt(PIN_GPIO_RXON_SAMPLING_ENABLED_NUM, ISR_External_Pin_RXON_Sampling_Timeslot_Event_Trigger, CHANGE);
  #endif // ENABLE_INTERRUPT_ON_CHANGE_PIN35_FOR_RXON_SAMPLING_TIMESLOT
  
  
}


/************************************************************************************
******** External Pin Interrupt Triggers For ADC ************************************
 @note Helper functions, that need to be static. The singlton class instance allows setting
       a flag that is inside the class
       Hardcoded variables to make this faster for measurements
*************************************************************************************
*************************************************************************************/
// void IRAM_ATTR ISR_External_Pin_ADC_Config_All_Trigger()
// {
//   DEBUG_ADC_ISR_EVENT_SET(LOW);
//   tkr_adc_internal->external_interrupt.flag_pin_active = true;

//   /**
//    * Capture both adc pins for 5 samples (no delay)
//    * */
//   mADC_I2S_Sampler::ISR_DUAL_CAPTURE* adc_p = &tkr_adc_internal->isr_capture;

//   if(adc_p->within_buffer_iter_counter < 40)
//   {
//     adc_p->adc_readings[adc_p->active_buffer_to_write_to_index].buffer_ch6[adc_p->within_buffer_iter_counter] = tkr_adc_internal->adc1_read_dma_value_1();//adc1_get_raw_ram(ADC1_CHANNEL_6);
//     // adc_p->adc_readings[adc_p->active_buffer_to_write_to_index].buffer_ch7[adc_p->within_buffer_iter_counter] = adc1_read_dma_value_1();//adc1_get_raw_ram(ADC1_CHANNEL_7);
//     adc_p->within_buffer_iter_counter++;
//   }

//   DEBUG_ADC_ISR_EVENT_SET(HIGH);
// }


/**
 * Test method, to read both ringbuffers by single item, and fire the result out as one json
 * */
void mADC_I2S_Sampler::SendADCReadingsSerial()
{

  /**
   * Read once uncase of atomic operations
   * */
  uint8_t ringbuffer_index = adcSampler1->active_ringbuffer_writting_index;

  size_t item_size1;
  char *item1 = nullptr;
  
  if(ringbuffer_index){
    item1 = (char *)xRingbufferReceive(adcSampler1->ringbuffer_handle1, &item_size1, pdMS_TO_TICKS(1000));
  }else{
    item1 = (char *)xRingbufferReceive(adcSampler1->ringbuffer_handle2, &item_size1, pdMS_TO_TICKS(1000));
  }

  //Check received item
  if (item1 != NULL) {

    /**
     * Read from buffer
     * */
    memcpy(buffer1,item1,item_size1);

    if(ringbuffer_index){
      vRingbufferReturnItem(adcSampler1->ringbuffer_handle1, (void *)item1); // Free memory
    }else{
      vRingbufferReturnItem(adcSampler1->ringbuffer_handle2, (void *)item1); // Free memory
    }


  
    uint16_t data_length = (item_size1/2)-1; // conversion from U8 to U16

    /**
     * extract number from buffer
     **/ 
    uint16_t rxon_id_number_of_item = buffer1[500]; //since 0-49 is rss samples
    uint16_t active_buffer_number_of_item = buffer1[501]; //since 0-49 is rss samples

    uint8_t samples_used_in_average = 0;
    uint32_t averaged_sample = 0;
    uint16_t new_sample = 0;
    
    #ifdef USE_RINGBUFFER_TO_SERIAL2_METHOD1

    Serial.printf("{\"size1\":%d,",data_length);
    Serial.printf("\"rxon\":%d,",rxon_id_number_of_item);
    Serial.printf("\"id\":%d,",adcSampler1->active_ringbuffer_writting_index);
    Serial.printf("\"ab\":%d,",active_buffer_number_of_item);
    // Serial.printf("\"averaged_samples\":%d,\"data1\":[", data_length/SAMPLE_DOWNSAMPLE_MOVING_AVERAGE);
    // for(uint16_t i=0;i<data_length;/*NO ITER*/)
    // {
    //   averaged_sample = 0;
    //   samples_used_in_average = 0;
    //   for(int j=0;j<SAMPLE_DOWNSAMPLE_MOVING_AVERAGE;j++)
    //   {
    //     new_sample = (offset1 - buffer1[i++]);  //also progress the sample iterator, suppress highest bits
    //     /**
    //      * Remove erronous values from the averaging
    //      * */
    //     if(new_sample<5000)
    //     {
    //       averaged_sample += new_sample;
    //       // test_sample = new_sample;
    //       samples_used_in_average++;
    //     }
    //   }

    //   if(samples_used_in_average)
    //   {
    //     averaged_sample /= samples_used_in_average;
    //   }
      
    //   if(averaged_sample<5000)
    //   {
    //     Serial.printf("%d",averaged_sample);
    //     if(i<data_length-1) {
    //       Serial.printf(",");
    //     }
    //   }
      
    // }
    // Serial.printf("]"); 
    Serial.printf("}\n\r"); 

    #endif

  }

  

}



/**
 * Test method, to read both ringbuffers by single item, and fire the result out as one json
 * */
void mADC_I2S_Sampler::Append_JSONPart_ESP32ADCReadings()
{

  /**
   * Read once uncase of atomic operations
   * */
  uint8_t ringbuffer_index = adcSampler1->active_ringbuffer_writting_index;

  size_t item_size1;
  char *item1 = nullptr;
  
  if(ringbuffer_index){
    item1 = (char *)xRingbufferReceive(adcSampler1->ringbuffer_handle1, &item_size1, pdMS_TO_TICKS(1000));
  }else{
    item1 = (char *)xRingbufferReceive(adcSampler1->ringbuffer_handle2, &item_size1, pdMS_TO_TICKS(1000));
  }

  //Check received item
  if (item1 != NULL) {

    /**
     * Read from buffer
     * */
    memcpy(buffer1,item1,item_size1);

    if(ringbuffer_index){
      vRingbufferReturnItem(adcSampler1->ringbuffer_handle1, (void *)item1); // Free memory
    }else{
      vRingbufferReturnItem(adcSampler1->ringbuffer_handle2, (void *)item1); // Free memory
    }


  
    uint16_t data_length = (item_size1/2)-1; // conversion from U8 to U16

    /**
     * extract number from buffer
     **/ 
    uint16_t rxon_id_number_of_item = buffer1[500]; //since 0-49 is rss samples
    uint16_t active_buffer_number_of_item = buffer1[501]; //since 0-49 is rss samples

    uint8_t samples_used_in_average = 0;
    uint32_t averaged_sample = 0;
    uint16_t new_sample = 0;
    
    #ifdef USE_RINGBUFFER_TO_SERIAL2_METHOD1

    // BufferWriterI->Append_P("\"size1\":%d,", data_length);
    BufferWriterI->Append_P("\"rxon\":%d,",   rxon_id_number_of_item);
    BufferWriterI->Append_P("\"id\":%d,",     adcSampler1->active_ringbuffer_writting_index);

    // BufferWriterI->Append_P("\"averaged_samples\":%d", data_length/SAMPLE_DOWNSAMPLE_MOVING_AVERAGE);
    BufferWriterI->Append_P(",\"data1\":[", data_length/SAMPLE_DOWNSAMPLE_MOVING_AVERAGE);
    BufferWriterI->Append_P("\"ab\":%d,",active_buffer_number_of_item);

    data_length = 20;

    for(uint16_t i=0;i<data_length;/*NO ITER*/)
    {
      averaged_sample = 0;
      samples_used_in_average = 0;
      for(int j=0;j<SAMPLE_DOWNSAMPLE_MOVING_AVERAGE;j++)
      {
        new_sample = (offset1 - buffer1[i++]);  //also progress the sample iterator, suppress highest bits
        /**
         * Remove erronous values from the averaging
         * */
        if(new_sample<5000)
        {
          averaged_sample += new_sample;
          samples_used_in_average++;
        }
      }

      if(samples_used_in_average)
      {
        averaged_sample /= samples_used_in_average;
      }
      
      if(averaged_sample<5000)
      {
        BufferWriterI->Append_P("%d",averaged_sample);
        if(i<data_length-1) {
          BufferWriterI->Append_P(",");
        }
      }
      
    }
    BufferWriterI->Append_P("]"); 

    #endif

  }

  

}


#ifdef USE_MODULE_NETWORK_MQTT

void mADC_I2S_Sampler::Telemetry_Init(){

  struct telemetry_handler<mADC_I2S_Sampler>* ptr;

  ptr = &telemetry_settings;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = tkr_mqtt->dt.configperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mADC_I2S_Sampler::ConstructJSON_Settings;

  ptr = &telemetry_sensor_teleperiod;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//tkr_mqtt->dt.teleperiod_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADC_I2S_Sampler::ConstructJSON_Sensor;

  ptr = &telemetry_sensor_ifchanged;
  ptr->tSavedLastSent = 0;
  ptr->flags.PeriodicEnabled = FLAG_ENABLE_DEFAULT_PERIODIC_SENSOR_MQTT_MESSAGES;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//tkr_mqtt->dt.ifchanged_secs; 
  ptr->flags.topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  ptr->flags.json_level = JSON_LEVEL_DETAILED;
  ptr->key = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  ptr->ConstructJSON_function = &mADC_I2S_Sampler::ConstructJSON_Sensor;
  
} 

/**
 * @brief Set flag for all telemetryhandlers to send
 * */
void mADC_I2S_Sampler::MQTTHandler_RefreshAll()
{
  for(auto& handle:telemetry_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mADC_I2S_Sampler::MQTTHandler_Rate()
{
  for(auto& handle:telemetry_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = tkr_mqtt->dt.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = tkr_mqtt->dt.ifchanged_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mADC_I2S_Sampler::MQTTHandler_Sender()
{
  for(auto& handle:telemetry_list){
    tkr_mqtt->MQTTHandler_Command_UniqueID(*this, GetModuleUniqueID(), handle);
  }
}

#endif // USE_MODULE_NETWORK_MQTT

#endif
