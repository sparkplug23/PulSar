#include "mSerialPositionalLogger.h"

#ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER

const char* mSerialPositionalLogger::PM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR = D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR;
const char* mSerialPositionalLogger::PM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_FRIENDLY_CTR = D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_FRIENDLY_CTR;


  #ifdef ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS
/************************************************************************************
******** External Pin Interrupt Triggers For ADC ************************************
 @note Helper functions, that need to be static. The singlton class instance allows setting
       a flag that is inside the class
*************************************************************************************
*************************************************************************************/
void IRAM_ATTR ISR_External_Pin_Sync_Frame_Status_Event_Trigger()
{
  pCONT_serial_pos_log->sync_frame_data.flag_pin_active = true;
  // pCONT_adc_internal->adc_config[1].flag_external_interrupt_triggered_reading = true;
  if(digitalRead(SYNC_FRAME_ISR_PIN)==LOW)
  {
    pCONT_serial_pos_log->sync_frame_data.flag_started = true;
  }
  else
  {
    pCONT_serial_pos_log->sync_frame_data.flag_ended = true;
  }
}


#endif // ENABLE_INTERRUPT_ON_CHANGE_PIN25_FOR_SYNCFRAME_TRANSMIT_STATUS






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

  //  if(external_interrupt.flag_enabled)
  // {
    
    sync_frame_data.trigger_pin = SYNC_FRAME_ISR_PIN;

    pinMode(sync_frame_data.trigger_pin, INPUT_PULLUP);
    attachInterrupt(sync_frame_data.trigger_pin, ISR_External_Pin_Sync_Frame_Status_Event_Trigger, CHANGE);
  // }




  #endif

}


void mSerialPositionalLogger::Init(void)
{


  //For ease, lets hard code the button rule

  
    #if defined(USE_MODULE_SENSORS_BUTTONS)
    
    // // Trigger0
    // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].trigger;   
    // p_event->module_id = EM_MODULE_SENSORS_BUTTONS_ID;
    // p_event->function_id = FUNC_EVENT_INPUT_STATE_CHANGED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = 1;  // Pressed 
    // // Command0
    // p_event = &pCONT_rules->rules[pCONT_rules->rules_active_index].command;   
    // p_event->module_id = EM_MODULE_CONTROLLER_SONOFF_IFAN_ID;
    // p_event->function_id = FUNC_EVENT_SET_SPEED_ID;
    // p_event->device_id = 0; // Button0
    // p_event->value.length = 0;
    // p_event->value.data[p_event->value.length++] = STATE_NUMBER_INCREMENT_ID;  // Increment 
    // pCONT_rules->rules_active_index++;
    // settings.loaded_default_for_moduled = true;

    #endif


}


void mSerialPositionalLogger::EveryLoop()
{
    // DEBUG_PIN3_SET(0);

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
      AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream UART%d >> [%d] \"%s\""), 2, BufferWriterI->GetLength(), BufferWriterI->GetPtr());
    #endif //USE_MODULE_DRIVERS_SDCARD


}



/**
 * Check ringbuffer for max size needed to get a superframe, then check for the index where the 0xFF 0xFF end-of-frame exists
 * Read it again, giving only this amount of data, return that space to the ringbuffer
 * */
void mSerialPositionalLogger::Construct_SuperFrame_Data_From_RingBuffer()
{

  memset(sync_frame_data.buffer, 0, sizeof(sync_frame_data.buffer));
  uint16_t maximum_sync_frame_length = 300;
  sync_frame_data.buffer_bytes_read = pCONT_uart->GetSingleItemFromNoSplitRingBuffer(RSS_RINGBUFFER_NUMBER_INDEX, sync_frame_data.buffer, maximum_sync_frame_length);

  
  if(sync_frame_data.buffer_bytes_read){
  //  AddLog(LOG_LEVEL_TEST, PSTR("SDCardStream bytes_read >> [%d]"), sync_frame_data.buffer_bytes_read);
    // AddLog_Array_P(LOG_LEVEL_INFO, PSTR("sync_frame_data.buffer"), sync_frame_data.buffer, sync_frame_data.buffer_bytes_read);
    // AddLog(LOG_LEVEL_TEST, PSTR("UART%d >> [%d] \"%s\""), i, bytes_in_line, BufferWriterI->GetPtr());
  }




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
    pCONT_sdcard->writer_settings.status == pCONT_sdcard->FILE_STATUS_OPENED_ID ?"Open":"CLOSED",
    &pCONT_sdcard->writer_settings.file_name[8] //skipping "APPEND_" to get just time
  );
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);
  #endif //USE_MODULE_DRIVERS_SDCARD

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
    
    JBI->Add("SN", sequence_test++);

    /**
     * on first sequence number, send additional useful info
     * */
    if(sequence_test == 0)
    {
      JBI->Add("NodeNumber", 1);
      JBI->Add("MAC", 1); //as this gives unique to hardware value
    }

    // Debug data only
    JBI->Add("MS",millis());
    //JBI->Add("Uptime", pCONT_time->RtcTime.hhmmss_ctr); //millis?


    // GPS data
    #ifdef USE_MODULE_DRIVERS_GPS
    JBI->Level_Start("gp");
      // JBI->Add("la", pCONT_gps->gps_result_stored.latitude()); 
      // JBI->Add("lg", pCONT_gps->gps_result_stored.longitude()); 

      //altitudem, msl approx 27

      JBI->Add("la", pCONT_gps->gps_result_stored.latitudeL()); 
      JBI->Add("lg", pCONT_gps->gps_result_stored.longitudeL()); 
      JBI->Add("at", pCONT_gps->gps_result_stored.altitude_cm()); //above mean sea level, in cm 
      JBI->Add("sd", pCONT_gps->gps_result_stored.speed());
    JBI->Level_End();
    #endif // USE_MODULE_DRIVERS_GPS


    /**
     * add esp32 rss data
     * */ 
    JBI->Array_Start("e2");
    for(int i=0;i<40;i++)
    {
      JBI->Add(i);
    }
    JBI->Array_End();

    JBI->Array_Start("e5");
    for(int i=0;i<40;i++)
    {
      JBI->Add(i);
    }
    JBI->Array_End();

    
    BufferWriterI->Append(",\"SF\":[");
    for(int i=0;i<sync_frame_data.buffer_bytes_read;i++)
    {
      BufferWriterI->Append_P(PSTR("%d%s"), sync_frame_data.buffer[i], i<sync_frame_data.buffer_bytes_read-1? ",": ""); 
    }
    BufferWriterI->Append("]");



    // RSS data from ringbuffer1, the arrival of X bytes into this buffer should trigger a write to the SD card.
    // This means, no exact SF is needed, but every RSS within it will have a gps position attached to it.

    //#ifdef ENABLE_DEVFEATURE_DUMMY_RSS_DATA

    // This will be pure UART_RSS Stream, triggered on GPIO pulse from pic32 interrupt
    // BufferWriterI->Append(",\"SF\":\"");
    // for(int i=0;i<163;i++)
    // {
      
    //   // sprintf(rss_single_frame, sizeof(rss_ingle_frame), 
    //   //   "%02d" // Frames
    //   //   "%02d" // Tx
    //   //   "%02d" // Rx
    //   //   "%02d" // RSS_2400
    //   //   "%02d" // RSS_5800
    //   //   ,
    //   //   i,
    //   //   2,
    //   //   3,
    //   //   1000,
    //   //   2000
    //   // );

    //   BufferWriterI->Append_P(PSTR("%c"), (i%10)+48);
    // }
    // BufferWriterI->Append("\"");

    //#endif // ENABLE_DEVFEATURE_DUMMY_RSS_DATA



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
