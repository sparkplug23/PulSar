#include "mSerialPositionalLogger.h"

#ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER

const char* mSerialPositionalLogger::PM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR = D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR;
const char* mSerialPositionalLogger::PM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_FRIENDLY_CTR = D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_FRIENDLY_CTR;

/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***** INTERRUPT pin controls ******************************************************************************************************************************************************************

- Syncframe duration ie durtion of T45 on the pic32
            - started = reset RXON_TIMESLOT_COUNTER
            - ended   =
- RXON sample duration ie when sampling on the esp32 should be recorded
            - started = increment a RXON_TIMESLOT_COUNTER, which is reset on syncframe
            - ended   =
                      
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/


/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
******SYNCFRAME_EVENT =  External Pin Interrupt Triggers For ADC *****************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/

#ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS

void IRAM_ATTR ISR_External_Pin_Sync_Frame_Status_Event_Trigger()
{
  // pCONT_serial_pos_log->sync_frame_data.flag_pin_active = true;
  // // pCONT_adc_internal->adc_config[1].flag_external_interrupt_triggered_reading = true;
  // if(digitalRead(PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM)==LOW)
  // {
  //   pCONT_serial_pos_log->sync_frame_data.flag_started = true;

  //   //Serial2.printf("ISR_External_Pin_Sync_Frame_Status_Event_Trigger");
      
  //   #ifdef ENABLE_ESP32_ADC_SAMPLING
  //   /**
  //    * toggle to the other buffer to be writting into, the read will check which is not active
  //    * */
  //   pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index ^= 1; // Reset ADC syncframe index
  //   /**
  //    * Use new buffer set here, to reset its counter
  //    * The other buffer not being written into, the counter values here wont be reset until the next ISR, allowing it to be checked for length of data on previous frame
  //    * */
  //     pCONT_adc_internal->isr_capture.within_buffer_iter_counter = 0;
  //   #endif // ENABLE_ESP32_ADC_SAMPLING

  // }
  // else
  // {
  //   pCONT_serial_pos_log->sync_frame_data.flag_ended = true;
  // }
}
#endif // ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS




/***********************************************************************************************************************************************************************
***********************************************************************************************************************************************************************
****** RXON_TIMESLOT_ENABLED = Where the pic32 RXON sampling timeslot is happening *****************************************************************************************************************************************************************
******
* @note - This will be set during the sampling window of the pic32 timeslot, and will be recorded during this period
On each started/low, a RXON_SAMPLE_COUNTER will be incremented so all samples in item will be known which slot it comes from
Instead of using a second buffer, instead, 2 bytes of the DMA read will also have this number appended to it (bytes 1001 and 1002), these will be extracted during json format and thus not during matlab



*****************************************************************************************************************************************************************
***********************************************************************************************************************************************************************/


#ifdef ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD

void IRAM_ATTR ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger()
{

  pCONT_serial_pos_log->sync_frame_data.flag_pin_active = true;

  gpio_num_t pin = (gpio_num_t)(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM & 0x1F);
  int state = state = (GPIO_REG_READ(GPIO_IN_REG)  >> pin) & 1U;

  if(state==0)
  {
    pCONT_serial_pos_log->sync_frame_data.flag_started = true;

    /**
     * Flag to reset the I2S ADC buffers
     * */
    extern_flag_swap_ringbuffers_before_writting_is2_data = true;
          
    pCONT_serial_pos_log->rxon_counter = 0;
    pCONT_adc_internal->adcSampler1->item_id_counter=0;
  //   // Serial.printf("RESET rxon_counter=%d\n\r",pCONT_serial_pos_log->rxon_counter);
      
  //   /**
  //    * Swap between ringbuffers for internal-ADC
  //    * */
  //   // pCONT_adc_internal->SwapReaderWritersRingbuffers();    

  //   // #ifdef ENABLE_ESP32_ADC_SAMPLING
  //   // /**
  //   //  * toggle to the other buffer to be writting into, the read will check which is not active
  //   //  * */
  //   // pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index ^= 1; // Reset ADC syncframe index
  //   // /**
  //   //  * Use new buffer set here, to reset its counter
  //   //  * The other buffer not being written into, the counter values here wont be reset until the next ISR, allowing it to be checked for length of data on previous frame
  //   //  * */
  //   // pCONT_adc_internal->isr_capture.within_buffer_iter_counter = 0;
  //   // #endif // ENABLE_ESP32_ADC_SAMPLING

  }
  else
  {
    pCONT_serial_pos_log->sync_frame_data.flag_ended = true;
    // Serial.println("ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger HIGH HERE");
  }
}
#endif // ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD






















int8_t mSerialPositionalLogger::Tasker(uint8_t function, JsonParserObject obj){
  
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
    case FUNC_LOOP: 
      EveryLoop();
    break;  
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


void mSerialPositionalLogger::parse_JSONCommand(JsonParserObject obj)
{

}


void mSerialPositionalLogger::Pre_Init(void)
{
  // if (pCONT_pins->PinUsed(GPIO_PZEM0XX_RX_MODBUS_ID) && pCONT_pins->PinUsed(GPIO_PZEM0XX_TX_ID))
  // {
    settings.fEnableSensor = true;
  // }



/**
 * Pin "sync_frame_status" on falling edge signifies the start of a new frame being sent, while, rising edge is the completion of the data.
 * Rising edge should occur approx 2.5ms after falling, and will set a flag to begin formation of sync_frame sample data to be sent to SDCARD stream
 * The amount of data to be read from RSS_STREAM will be known as it ends with two 0xFF 0xFF EOF bytes.
 * */
  #ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
    pinMode(PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM, INPUT_PULLUP);
    attachInterrupt(PIN_GPIO_FUNCTION_SYNC_FRAME_ISR_PIN_NUM, ISR_External_Pin_Sync_Frame_Status_Event_Trigger, CHANGE);
  #endif // ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
  
  #ifdef ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD
    pinMode(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, INPUT_PULLUP );
    // attachInterrupt(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger_LOW, FALLING);
    attachInterrupt(PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM, ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger, CHANGE);
  #endif // ENABLE_INTERRUPT_ON_CHANGE_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD



// PIN_GPIO_FUNCTION_TRIGGER_ADC_SYNC_PERIOD_COMPLETED_TIMESLOT_PERIOD_NUM
}


void mSerialPositionalLogger::Init(void)
{

  WiFi.mode(WIFI_OFF);
  btStop();

  
    /**
     * Only on debug lipo-esp32, for simplifying the data to pc process
     * */
    #if defined(USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT) && !defined(USE_MODULE_DRIVERS_SERIAL_UART)
      // Configure here for debugging only
      Serial2.begin(2048000);
    #endif

}


void mSerialPositionalLogger::EveryLoop()
{
    // DEBUG_PIN3_SET(0);

    if(
    pCONT_serial_pos_log->sync_frame_data.flag_started)
    {
    
    pCONT_serial_pos_log->sync_frame_data.flag_started = false;

    Serial.println("ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger");
    
}
    // = true;

    //Serial.println("ISR_External_Pin_ADC_Sync_Period_Completed_Timeslot_Event_Trigger");

    // pinMode(34, INPUT);

//     if(digitalRead(26)==0)
//     {
// Serial.println("if(digitalRead(26)) low");
// delay(500);
//     }else
//     {
// Serial.println("ELSE if(digitalRead(34))");

    //Serial.printf("rxon_counter=%d\n\r",rxon_counter);
      
    // rxon_counter=0;
//     }

  #ifdef ENABLE_GPS_10MS_LOG_TEST1
  if(mTime::TimeReached(&tSaved_MillisWrite, 10))
  {
    //DEBUG_PIN2_SET(0);
    SubTask_HandleSDCardLogging();
    //DEBUG_PIN2_SET(1);
  }
    // DEBUG_PIN3_SET(1);/
  #endif // ENABLE_GPS_10MS_LOG_TEST1

  Handle_Primary_Service_RSS_Stream_To_Create_SDCard_Stream();


}


void mSerialPositionalLogger::Handle_Primary_Service_RSS_Stream_To_Create_SDCard_Stream()
{

  if(pCONT_serial_pos_log->sync_frame_data.flag_started)
  {
    pCONT_serial_pos_log->sync_frame_data.flag_started = false;
    // AddLog(LOG_LEVEL_INFO, PSTR("sync_frame_data.flag_started"));








// uint8_t read_index = 0;//!pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index; // invert

// Serial.println();
// Serial.println();
// Serial.println();

// Serial.println(pCONT_adc_internal->isr_capture.within_buffer_iter_counter);
// for(int i=0;i<40;i++)
// {
// Serial.print(pCONT_adc_internal->isr_capture.adc_readings[read_index].buffer_ch6[i]);
// Serial.print('-');
// }
// Serial.println();

// Serial.println(pCONT_adc_internal->isr_capture.within_buffer_iter_counter);
// for(int i=0;i<40;i++)
// {
// Serial.print(pCONT_adc_internal->isr_capture.adc_readings[read_index].buffer_ch7[i]);
// Serial.print('-');
// }
// Serial.println();





  }
  
  if(pCONT_serial_pos_log->sync_frame_data.flag_ended)
  {
    pCONT_serial_pos_log->sync_frame_data.flag_ended = false;
    // AddLog(LOG_LEVEL_INFO, PSTR("sync_frame_data.flag_ended"));
    SubTask_Generate_SyncFrame_To_SDCard_Stream();
  }

}

void mSerialPositionalLogger::SubTask_Generate_SyncFrame_To_SDCard_Stream()
{
    // #ifdef ENABLE_DEVFEATURE_SPLASH_RINGBUFFER_TO_DEBUG_SERIAL
    // // for(
    //   int i=2;
    // //   i<3;i++)
    // // {
    //   BufferWriterI->Clear();
    //   uint16_t bytes_in_line = pCONT_uart->GetRingBufferDataAndClear(i, BufferWriterI->GetPtr(), BufferWriterI->GetBufferSize(), '\n', false);
    //   if(bytes_in_line){
    //     AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream UART%d >> [%d]"), i, bytes_in_line);
    //     // AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
    //   }
    // // }
    // #endif

    /**
     * Warning: Assume flags and this next part of code happen fast enough (approx. 7.5ms or less) so no special char is needed, what is in the buffer should be the entire pic32_syncframe
     * */
    Construct_SuperFrame_Data_From_RingBuffer();


    // memset(sync_frame_data.buffer, 0, sizeof(sync_frame_data.buffer));
    // uint16_t maximum_sync_frame_length = 300;
    // sync_frame_data.buffer_bytes_read = pCONT_uart->GetRingBufferDataAndClear(RSS_RINGBUFFER_NUMBER_INDEX, sync_frame_data.buffer, maximum_sync_frame_length);
    //   if(sync_frame_data.buffer_bytes_read){
    //   //  AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream bytes_read >> [%d]"), sync_frame_data.buffer_bytes_read);
    //     // AddLog_Array_P(LOG_LEVEL_INFO, PSTR("sync_frame_data.buffer"), sync_frame_data.buffer, sync_frame_data.buffer_bytes_read);
    //     // AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
    //   }

    /**
     * Get data that is superframe, and save as its own small buffer read from RSS ring, instead of global buffer
     * */


    ConstructJSON_SDCardSuperFrame();

    /**
     * Append to sdcard stream
     * */
    #ifdef USE_MODULE_DRIVERS_SDCARD
      pCONT_sdcard->AppendRingBuffer(BufferWriterI->GetPtr(), BufferWriterI->GetLength());
    #else
     // AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream UART%d >> [%d] \"%s\""), 2, BufferWriterI->GetLength(), BufferWriterI->GetPtr());
    #endif //USE_MODULE_DRIVERS_SDCARD

    /**
     * Only on debug lipo-esp32, for simplifying the data to pc process
     * */
    #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT
      char* buff_ptr = BufferWriterI->GetPtr();
      for(int i=0;i<BufferWriterI->GetLength();i++)
      {
        Serial2.print(buff_ptr[i]);
      }
      // For pretty output
      Serial2.println();
    #endif



}



/**
 * Check ringbuffer for max size needed to get a superframe, then check for the index where the 0xFF 0xFF end-of-frame exists
 * Read it again, giving only this amount of data, return that space to the ringbuffer
 * */
void mSerialPositionalLogger::Construct_SuperFrame_Data_From_RingBuffer()
{

  #ifdef USE_MODULE_DRIVERS_SERIAL_UART

  memset(sync_frame_data.buffer, 0, sizeof(sync_frame_data.buffer));
  uint16_t maximum_sync_frame_length = 300;
  sync_frame_data.buffer_bytes_read = pCONT_uart->GetSingleItemFromNoSplitRingBuffer(RSS_RINGBUFFER_NUMBER_INDEX, sync_frame_data.buffer, maximum_sync_frame_length);

  
  if(sync_frame_data.buffer_bytes_read){
  //  AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream bytes_read >> [%d]"), sync_frame_data.buffer_bytes_read);
    // AddLog_Array_P(LOG_LEVEL_INFO, PSTR("sync_frame_data.buffer"), sync_frame_data.buffer, sync_frame_data.buffer_bytes_read);
    // AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
  }

  #endif // USE_MODULE_DRIVERS_SERIAL_UART




}




//Use gps time to set rtc time?

void mSerialPositionalLogger::EverySecond()
{

  SubTask_UpdateOLED();

  // SubTask_HandleSDCardLogging();

  // SubTask_Debug_BasicFileWriteTest();


// ConstructJSON_SDCardSuperFrame();
//   AddLog(LOG_LEVEL_TEST, PSTR("SuperFrame=\"%s\""), JBI->GetBufferPtr());


  

}



/**
 * @brief Test1: Button will open the file, then each time it is called, append the current RTC time.
 * Button will then close the file again. 
 * This method will try writting it directly, but future method may need a ringbuffer just for the sdcard stream... "BufferSDCARDStream-> print to it"
 * */
void mSerialPositionalLogger::SubTask_HandleSDCardLogging()
{
  #ifdef USE_MODULE_DRIVERS_SDCARD

  #ifdef ENABLE_SDLOGGER_APPEND_JSON_SUPERFRAME
  if(sdcard_status.isopened)
  {

    ConstructJSON_SDCardSuperFrame();
    AddLog(LOG_LEVEL_TEST, PSTR("sdcardframe=\"%s\""), BufferWriterI->GetPtr());

    pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());

  }
  #endif // ENABLE_SDLOGGER_APPEND_JSON_SUPERFRAME

  #ifdef ENABLE_SDLOGGER_APPEND_TIME_TEST
  if(pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID)
  {

    ConstructJSON_SDCardSuperFrame();
    
    // if(mTime::TimeReached(&tSaved_MillisWrite2, 1000))
    // {
    //   AddLog(LOG_LEVEL_TEST, PSTR("sdcardframe[%d]=\"%s\""), pCONT_sdcard->sdcard_status.bytes_written_to_file, BufferWriterI->GetPtr());
    // }

    pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());

  }
  #endif // ENABLE_SDLOGGER_APPEND_TIME_TEST


  #ifdef ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  if(pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID)
  {

    ConstructJSON_SDCardSuperFrame();
    
    // if(mTime::TimeReached(&tSaved_MillisWrite2, 1000))
    // {
    //   AddLog(LOG_LEVEL_TEST, PSTR("sdcardframe[%d]=\"%s\""), pCONT_sdcard->sdcard_status.bytes_written_to_file, BufferWriterI->GetPtr());
    // }

    // pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());
    /**
     * Append to sdcard stream
     * */
    pCONT_sdcard->AppendRingBuffer(BufferWriterI->GetPtr(), BufferWriterI->GetLength());
    




  }
  #endif // ENABLE_SDLOGGER_APPEND_TIME_TEST

  #endif // USE_MODULE_DRIVERS_SDCARD

}


/**
 * @brief basic test, that fully opens, appends and closed the file automatically on call
 * */
void mSerialPositionalLogger::SubTask_Debug_BasicFileWriteTest()
{

  #ifdef USE_MODULE_DRIVERS_SDCARD
  // As a test, lets open, write/append and close sd card
  

  // Open file
  sprintf(pCONT_sdcard->writer_settings.file_name, "/%s%d.txt", "SDCardTest",1);
  File file = SD.open(pCONT_sdcard->writer_settings.file_name, FILE_APPEND);
  if(!file){
    AddLog(LOG_LEVEL_TEST, PSTR("file \"%s\" did not open"),pCONT_sdcard->writer_settings.file_name);
  }
  AddLog(LOG_LEVEL_TEST, PSTR("file \"%s\" Opened!"),pCONT_sdcard->writer_settings.file_name);

  ConstructJSON_SDCardSuperFrame();
  AddLog(LOG_LEVEL_TEST, PSTR("sdcardframe=\"%s\""), BufferWriterI->GetPtr());
  // write all bytes
  char* buffer_to_write = BufferWriterI->GetPtr();
  for(int i=0; i<strlen(BufferWriterI->GetPtr()); i++)
  {
    file.print(buffer_to_write[i]);
  }
  file.print('\n');

  //close file
  file.close();

  #endif// USE_MODULE_DRIVERS_SDCARD

}

/**
 * @brief Show logger status on OLED
 * @note 4x11 chars at font 1
 * 
 * "LAT LON SQ"  LAT and LONG of gps (Without decimel) and S=Satellite count and Q=[V]alid or [E]rror
 * "Open xxxxxx" where x is the time opened
 * "Byte bbbbbb" where b is the number of bytes written (changing to kB when >1000)
 * "HHMMSS     " where HHMMSS is the GPS RTC time, which is also appended to the opened file name
 * */
void mSerialPositionalLogger::SubTask_UpdateOLED()
{

#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  char line_ctr[15] = {0};
  char buffer[15] = {0};

  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;

  /**
   * Row 1
   * GPS data
   * 
   * */
  #ifdef USE_MODULE_DRIVERS_GPS
  float latitude = pCONT_gps->gps_result_stored.latitudeL();
  uint32_t latitude_U32 = (uint32_t)latitude;
  uint8_t latitude_num_digits = mSupport::NumDigitsT(latitude_U32);
  uint32_t latitude_three_largest_chars = 0;
  if(latitude_num_digits>3){
    latitude_three_largest_chars = latitude_U32;
    for(int i=0;i<latitude_num_digits-3;i++)
    {
      latitude_three_largest_chars /= 10;
    }
  }else{
    latitude_three_largest_chars = latitude_U32;
  }

  float longitude = pCONT_gps->gps_result_stored.longitudeL();
  uint32_t longitude_U32 = (uint32_t)fabs(longitude);
  uint8_t longitude_num_digits = mSupport::NumDigitsT(longitude_U32);
  uint32_t longitude_three_largest_chars = 0;
  if(longitude_num_digits>3){
    longitude_three_largest_chars = longitude_U32;
    for(int i=0;i<longitude_num_digits-3;i++)
    {
      longitude_three_largest_chars /= 10;
    }
  }else{
    longitude_three_largest_chars = longitude_U32;
  }

  AddLog(LOG_LEVEL_TEST, PSTR("SubTask_UpdateOLED %d %d %d"), latitude_U32, latitude_num_digits, latitude_three_largest_chars);

  char quality[4] = {0};
  //if valid, then simply show valid, if not, show satellites
  if(pCONT_gps->gps_result_stored.valid.location)
  {
    snprintf(quality, sizeof(quality), "%s", "Val");  
  }else{
    // if(pCONT_gps->gps_result_stored.satellites<10)
    // {
      snprintf(quality, sizeof(quality), "%02d", pCONT_gps->gps_result_stored.satellites);  
    // }else{
    //   snprintf(quality, sizeof(quality), " 9+");        
    // }
  }

//https://cdn-shop.adafruit.com/datasheets/PMTK_A08.pdf


  snprintf(line_ctr, sizeof(line_ctr), "%03d %03d %s",
    latitude_three_largest_chars, 
    longitude_three_largest_chars,
    quality
    // pCONT_gps->gps_result_stored.satellites,
    // pCONT_gps->gps_result_stored.valid.location?'V':'E' //this needs updating regardless or V/A in RMC, to include timeouts without new updates
  );
  pCONT_iDisp->LogBuffer_AddRow(line_ctr, 0);
  #endif // USE_MODULE_DRIVERS_GPS

  //Test display messages
  //[1234567890123456]
  //[Op f123456] // Op/Cd for open and closed file, f# where # is GPS time for file name (although also using random in case gps is not working ie millis+gpsUTC)
  //[Val 123456] // Val Err for GPS fix, showing UTC time
  //[ ] //Also show lat/long so I know its working
  //[] packets received on serialRSS in thousands

// DEBUG_PIN2_TOGGLE();

  #ifdef USE_MODULE_DRIVERS_SDCARD
  snprintf(buffer, sizeof(buffer), "%c%s%s",
    pCONT_sdcard->sdcard_status.init_error_on_boot ? 'E' : 'f',
    pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID ?"OPEN!":"cd",
    &pCONT_sdcard->writer_settings.file_name[8] //skipping "APPEND_" to get just time
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);
  #endif //USE_MODULE_DRIVERS_SDCARD

  
uint32_t bytes_written = pCONT_sdcard->sdcard_status.bytes_written_to_file;
char unit_type = 'B';

if(bytes_written>50000)
{
  bytes_written /= 1000; //into kB
  unit_type = 'k';
}

  snprintf(buffer, sizeof(buffer), "%d %c",bytes_written,unit_type);
  pCONT_iDisp->LogBuffer_AddRow(buffer,2);

  snprintf(buffer, sizeof(buffer), "%s %s","Op",pCONT_time->RtcTime.hhmmss_ctr);//pCONT_time->GEt DT_UTC;
  pCONT_iDisp->LogBuffer_AddRow(buffer, 3);
  #endif // USE_MODULE_DISPLAYS_OLED_SSD1306

}





uint8_t mSerialPositionalLogger::ConstructJSON_Settings(uint8_t json_method){

  JsonBuilderI->Start();
    JsonBuilderI->Add(D_JSON_CHANNELCOUNT, 0);
  return JsonBuilderI->End();

}


uint8_t mSerialPositionalLogger::ConstructJSON_Sensor(uint8_t json_method){

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
uint8_t mSerialPositionalLogger::ConstructJSON_SDCardSuperFrame(uint8_t json_method){

  JsonBuilderI->Start();
    
    /**
     * on first sequence number, send additional useful info
     * */
    if(sequence_test == 0)
    {
      JBI->Add("DeviceName", DEVICENAME_FRIENDLY_CTR);
    }

    JBI->Add("SN", sequence_test++);

    // // Debug data only
    // JBI->Add("MS",millis());

    // // GPS data
    // #ifdef USE_MODULE_DRIVERS_GPS
    // JBI->Level_Start("gp");
    //   JBI->Add("la", pCONT_gps->gps_result_stored.latitudeL()); 
    //   JBI->Add("lg", pCONT_gps->gps_result_stored.longitudeL()); 
    //   JBI->Add("at", pCONT_gps->gps_result_stored.altitude_cm()); //above mean sea level, in cm 
    //   JBI->Add("sd", pCONT_gps->gps_result_stored.speed());
    //   JBI->Add("hd", pCONT_gps->gps_result_stored.heading_cd());
    //   JBI->Add("gh", pCONT_gps->gps_result_stored.geoidHeight_cm()); // Height of the geoid above the WGS84 ellipsoid
    //   JBI->Add("s",  pCONT_gps->gps_result_stored.satellites);

    //   uint32_t timeofday_seconds = 
    //     (pCONT_gps->gps_result_stored.dateTime.hours*3600) +
    //     (pCONT_gps->gps_result_stored.dateTime.minutes*60) +
    //     (pCONT_gps->gps_result_stored.dateTime.seconds*1000);

    //   uint32_t tod_millis = 
    //     (timeofday_seconds*1000) + 
    //     pCONT_gps->gps_result_stored.dateTime_ms();

    //   JBI->Add("tms",  tod_millis);
    // JBI->Level_End();
    // #endif // USE_MODULE_DRIVERS_GPS


    // /**
    //  * add esp32 rss data
    //  * */ 
    // #ifdef ENABLE_ESP32_ADC_SAMPLING
    // uint8_t read_index = pCONT_adc_internal->isr_capture.active_buffer_to_write_to_index?0:1;
    // JBI->Array_Start("e2");
    // for(int i=0;i<40;i++)
    // {
    //   JBI->Add(pCONT_adc_internal->isr_capture.adc_readings[read_index].buffer_ch6[i]);
    // }
    // JBI->Array_End();

    // JBI->Array_Start("e5");
    // for(int i=0;i<40;i++)
    // {
    //   JBI->Add(pCONT_adc_internal->isr_capture.adc_readings[read_index].buffer_ch7[i]);
    // }
    // JBI->Array_End();
    // #endif // ENABLE_ESP32_ADC_SAMPLING

    
    // BufferWriterI->Append(",\"SF\":[");
    // for(int i=0;i<sync_frame_data.buffer_bytes_read;i++)
    // {
    //   BufferWriterI->Append_P(PSTR("%d%s"), sync_frame_data.buffer[i], i<sync_frame_data.buffer_bytes_read-1? ",": ""); 
    // }
    // BufferWriterI->Append("]");

    /**
     * Test, add only first sample from each esp32_adc item
     * */
    JBI->Level_Start("EA");
      pCONT_adc_internal->Append_JSONPart_ESP32ADCReadings();
    JBI->Level_End();


  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

  #ifdef USE_MODULE_NETWORK_MQTT
void mSerialPositionalLogger::MQTTHandler_Init(){

  struct handler<mSerialPositionalLogger>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_sensor_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 60; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sensor_ifchanged;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SENSORS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_Sensor;

  mqtthandler_ptr = &mqtthandler_sdcard_superframe;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_IFCHANGED_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SDCARD_SUPERFRAME_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSerialPositionalLogger::ConstructJSON_SDCardSuperFrame;
  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSerialPositionalLogger::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSerialPositionalLogger::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

/**
 * @brief Check all handlers if they require action
 * */
void mSerialPositionalLogger::MQTTHandler_Sender(uint8_t id)
{
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_ID, handle, id);
  }
}

  #endif // USE_MODULE_NETWORK_MQTT


////////////////////// END OF MQTT /////////////////////////

#endif
