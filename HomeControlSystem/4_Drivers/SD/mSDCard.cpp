#include "mSDCard.h"

/***
 * Introduce a ringbuffer for sdcard writter, hence as long as the sdcard is open, anything written to buffer will be flushed to sdcard
 * */


#ifdef USE_MODULE_DRIVERS_SDCARD

const char* mSDCard::PM_MODULE_DRIVERS_SDCARD_CTR = D_MODULE_DRIVERS_SDCARD_CTR;
const char* mSDCard::PM_MODULE_DRIVERS_SDCARD_FRIENDLY_CTR = D_MODULE_DRIVERS_SDCARD_FRIENDLY_CTR;

SPIClass spiSD(HSPI);

int8_t mSDCard::Tasker(uint8_t function, JsonParserObject obj){

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    Pre_Init();
  }else
  if(function == FUNC_INIT){
    
    // #ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT
    //   init_SDCARD_is_Serial_Debug_Only();
    // #else
      init();
    // #endif
  }

  // Only continue in to tasker if module was configured properly
  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:
    {

    }
    break;
    case FUNC_EVERY_50_MSECOND:

      EveryLoop_Handle_Appending_File_Method();

      /**
       * check if ringbuffer stream has data, if so, write to sdcard (Assuming its open)
       * */
      #ifdef ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
        Handle_Write_Ringbuffer_Stream_To_SDCard();
      #endif // ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
          
    break;
    case FUNC_EVERY_SECOND:  
    {

      // If not init, retry init, but ignore press to start
      if(sdcard_status.init_error_on_boot == true)
      {
        init();
      }

    } 
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
    case FUNC_MQTT_CONNECTED:
      MQTTHandler_Set_fSendNow();
    break;
    #endif //USE_MODULE_NETWORK_MQTT
  }
  
} // END Tasker


/**
 * @brief - Function will handle if the file should be opened or closed
 * 
 * */
void mSDCard::EveryLoop_Handle_Appending_File_Method()
{

  // Check if desired SD card status change and update saved variable
  if(sdcard_status.isopened != sdcard_status.isopened_previous_state)
  {

    // If not init, retry init, but ignore press to start
    if(sdcard_status.init_error_on_boot == true)
    {
      init();
      // If still failed, then reject opening file
      if(sdcard_status.init_error_on_boot == true)
      {
        return;
      }
    }

    AddLog(LOG_LEVEL_TEST, PSTR("sdcard_status.isopened CHANGED =%d"), sdcard_status.isopened);
    sdcard_status.isopened_previous_state = sdcard_status.isopened;
    // If opened, close
    if(sdcard_status.isopened)
    {
      writer_settings.status = mSDCard::FILE_STATUS_CLOSING_ID;
      SubTask_Append_To_Open_File();
    }
    else
    {
      writer_settings.status = mSDCard::FILE_STATUS_OPENING_ID;
      SubTask_Append_To_Open_File();
    }
  }

}


void mSDCard::init(void)
{

  Serial.println("Initializing SD card...");

  int8_t chip_select = -1;
  int8_t clock = -1;
  int8_t mosi = -1;
  int8_t miso = -1;

  chip_select = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_CSO_ID);
  clock = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_CLK_ID);
  mosi = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_MOSI_ID);
  miso = pCONT_pins->GetPin(GPIO_FUNCTION_SDCARD_VSPI_MISO_ID);
  
  uint8_t sd_hardware_type = 0;

  if((chip_select>=0)&&(clock>=0)&&(mosi>=0)&&(miso>=0))
  {
    sd_hardware_type = 1;
  }

  /**
   * init VSPI
   * */
  if(sd_hardware_type == 1)
  {
    AddLog(LOG_LEVEL_INFO, PSTR("(clock, miso, mosi, chip_select)=(%d,%d,%d,%d)"), clock, miso, mosi, chip_select);
    spiSD.begin(clock, miso, mosi, chip_select); //SCK,MISO,MOSI,SS //HSPI1
  }else{
    return;
  }

  if(!SD.begin(chip_select, spiSD )){
    Serial.println("Card Mount Failed");
    sdcard_status.init_error_on_boot = true;
    // delay(3000);
    return;
  }


  uint8_t cardType = SD.cardType();

  Serial.print("SD Card Type: ");
  switch(cardType)
  {
    default:
    case CARD_NONE: 
      Serial.println("No SD card attached");
      sdcard_status.init_error_on_boot = true;
      return;      
    break;
    case CARD_MMC:
      Serial.println("MMC");
    break;
    case CARD_SD:
      Serial.println("SDSC");
    break;
    case CARD_SDHC:
      Serial.println("SDHC");
    break;
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);

  listDir(SD, "/", 0);

  Serial.printf("SD Card Size: %lluMB\n\r", cardSize);
  Serial.printf("Total space: %lluMB\n\r", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n\r", SD.usedBytes() / (1024 * 1024));

  // Success, no failure
  sdcard_status.init_error_on_boot = false;
  settings.fEnableModule = true;

}

#ifdef USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT
void mSDCard::init_SDCARD_is_Serial_Debug_Only()
{

  // Success, no failure
  sdcard_status.init_error_on_boot = false;
  settings.fEnableModule = true;

}
#endif  //   USE_SYSTEM_SIMULATE_SDCARD_OUTPUT_TO_RSS_UART_ESP32_OUTPUT


void mSDCard::Pre_Init(){

  #ifdef USE_SDCARD_RINGBUFFER_STEAM_OUT
    //chip select pin needs setting
    init_SDCard_StreamOut_RingBuffer();
  #endif // USE_SDCARD_RINGBUFFER_STEAM_OUT

}

#ifdef USE_SDCARD_RINGBUFFER_STEAM_OUT
void mSDCard::init_SDCard_StreamOut_RingBuffer()
{

  stream.ringbuffer_handle = xRingbufferCreate(stream.ring_buffer_size_rx, RINGBUF_TYPE_BYTEBUF);
  if (stream.ringbuffer_handle != NULL) {
    stream.initialised = true;
  }

  char data_ctr[] = "\0";
  UBaseType_t res =  xRingbufferSend(stream.ringbuffer_handle, data_ctr, strlen(data_ctr), pdMS_TO_TICKS(100));
  if (res != pdTRUE) {
    AddLog(LOG_LEVEL_ERROR, PSTR("%s FAILED"),data_ctr);
    stream.initialised = false; //disable if false
  }

}


void mSDCard::Handle_Write_Ringbuffer_Stream_To_SDCard()
{

  #ifdef ENABLE_SDLOGGER_APPEND_DATA_INTO_RINGBUFFER_STREAMOUT_TEST
  if(pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID)
  {

    uint16_t maximum_bytes_to_write_in_one_chunk = 2048; // 2kB per write
    uint16_t byte_read_limit = maximum_bytes_to_write_in_one_chunk > BufferWriterI->GetBufferSize() ? BufferWriterI->GetBufferSize() : maximum_bytes_to_write_in_one_chunk;

    // BufferWriterI->Clear();
    uint16_t bytes_read = GetRingBufferDataAndClear(0, stream_out_buffer, byte_read_limit, '\n', false);
    // if(strlen(BufferWriterI->GetPtr())==0){
    //   AddLog(LOG_LEVEL_TEST, PSTR("GPS UART%d >> [%d] \"%s\""), 1, bytes_to_read, BufferWriterI->GetPtr());
    // }

    //if any data found
    if(bytes_read)
    {  
      // char* pbuffer = BufferWriterI->GetPtr();
      AddLog(LOG_LEVEL_TEST, PSTR("buffer[%d]=\"%s\""),bytes_read, stream_out_buffer);
      pCONT_sdcard->SubTask_Append_To_Open_File(stream_out_buffer, bytes_read);      
    }


  }
  #endif // ENABLE_SDLOGGER_APPEND_TIME_TEST


}

/**
 * Universal to all UARTs, get RingBuffer
 * Returns: Bytes read from buffer and thus freed
 * Arg:     Buffer to write into and its size, or simply, how much to read at a time
 * Possible change: Read until length, or stop on a special character (with possible relative index also ie "a,5x" detect comma, plus 2)
 * @return item_size number of bytes read from buffer
 * */
uint16_t mSDCard::GetRingBufferDataAndClear(uint8_t stream_num, char* buffer, uint16_t buflen, char optional_read_until_char, bool flag_clear_buffer_after_read)
{

  // Receive an item from no-split ring buffer
  size_t item_size = 0;
  // Wait at most 1ms to read from buffer, read a maximum number of bytes
  char*  item = (char *)xRingbufferReceiveUpTo(stream.ringbuffer_handle, &item_size, pdMS_TO_TICKS(100), buflen);

  //Check received item
  if (item != NULL) 
  {
    // Read from buffer
    memcpy(buffer,item,item_size);
    //Return Item
    vRingbufferReturnItem(stream.ringbuffer_handle, (void *)item); // Free memory
  // }
  } 

  return item_size;

}


uint16_t mSDCard::AppendRingBuffer(char* buffer, uint16_t buflen)
{
  
  UBaseType_t res =  xRingbufferSend(stream.ringbuffer_handle, buffer, buflen, pdMS_TO_TICKS(1000));
  if (res != pdTRUE) {
      printf("Failed to send item\n");
  }
  
  stream.flag_data_waiting = true; // easier than checking for bytes each time

  return res;

}

#endif // USE_SDCARD_RINGBUFFER_STEAM_OUT


uint8_t mSDCard::ConstructJSON_Settings(uint8_t json_method)
{

  char buffer[30];
  
  JBI->Start();  

    JBI->Level_Start("Debug");
      JBI->Array_Start("message1");
          for(int i=0;i<debug.write_time.opened.size();i++)
              JBI->Add(debug.write_time.closed[i]-debug.write_time.opened[i]);
      JBI->Array_End();
    JBI->Level_End();
  
    JBI->Add_P(PM_JSON_TIME, 1000);
  return JBI->End();

}


uint8_t mSDCard::ConstructJSON_FileWriter(uint8_t json_method)
{

  char buffer[30];
  
  JBI->Start();  

    JBI->Add("file_name", writer_settings.file_name);
    JBI->Add("status", writer_settings.status);

  return JBI->End();

}

/**
 * @brief Created for write time tests of sector sizes
 * */
uint8_t mSDCard::ConstructJSON_Debug_WriteTimes(uint8_t json_method)
{

  char buffer[30];
  
  JBI->Start();  

    JBI->Level_Start("512B");
      JBI->Add("CompleteWriteTime", debug_write_times.complete_write_duration);
      JBI->Add("BytesWritten", debug_write_times.write_byte_count);
    JBI->Level_End();


  return JBI->End();

}


  #ifdef USE_MODULE_NETWORK_MQTT
void mSDCard::MQTTHandler_Init(){

  struct handler<mSDCard>* mqtthandler_ptr;

  mqtthandler_ptr = &mqtthandler_settings_teleperiod;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSDCard::ConstructJSON_Settings;

  mqtthandler_ptr = &mqtthandler_file_writer;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_FILE_WRITER_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSDCard::ConstructJSON_FileWriter;

  mqtthandler_ptr = &mqtthandler_debug_write_times;
  mqtthandler_ptr->tSavedLastSent = millis();
  mqtthandler_ptr->flags.PeriodicEnabled = true;
  mqtthandler_ptr->flags.SendNow = true;
  mqtthandler_ptr->tRateSecs = 1; 
  mqtthandler_ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  mqtthandler_ptr->json_level = JSON_LEVEL_DETAILED;
  mqtthandler_ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_DEBUG_WRITE_TIMES_CTR;
  mqtthandler_ptr->ConstructJSON_function = &mSDCard::ConstructJSON_Debug_WriteTimes;

}

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mSDCard::MQTTHandler_Set_fSendNow()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mSDCard::MQTTHandler_Set_TelePeriod()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
  }
}

void mSDCard::MQTTHandler_Sender(uint8_t id){
    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS_SDCARD_ID, handle, id);
  }

}


  #endif// USE_MODULE_NETWORK_MQTT


#endif
