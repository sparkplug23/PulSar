#include "mSerialPositionalLogger.h"

#ifdef USE_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER

const char* mSerialPositionalLogger::PM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR = D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_CTR;
const char* mSerialPositionalLogger::PM_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_FRIENDLY_CTR = D_MODULE_CONTROLLER_SERIAL_POSITIONAL_LOGGER_FRIENDLY_CTR;


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
    //Rule based on button toggle for sd open and close 
    //have buitin led on when sd card has shown write activity in last 100ms
    /************
     * RULES SECTION * 
    *******************/
    #ifdef USE_MODULE_CORE_RULES
    // case FUNC_EVENT_BUTTON_PRESSED:
    //   RulesEvent_Button_Pressed();
    // break;
    #endif// USE_MODULE_CORE_RULES

    /************
     * TRIGGERS SECTION * 
    *******************/
    case FUNC_EVENT_INPUT_STATE_CHANGED_ID:
      CommandSet_SDCard_OpenClose_Toggle();
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


}

//Use gps time to set rtc time?

void mSerialPositionalLogger::EverySecond()
{

  SubTask_UpdateOLED();

  SubTask_HandleSDCardLogging();

  // SubTask_Debug_BasicFileWriteTest();

}



/**
 * @brief Test1: Button will open the file, then each time it is called, append the current RTC time.
 * Button will then close the file again. 
 * */
void mSerialPositionalLogger::SubTask_HandleSDCardLogging()
{

  // If desired card state has changed, then change the card mode needed
  
  // Check if desired SD card status change and update saved variable
  if(sdcard_status.enable_logging != enable_logging_previous_state)
  {
    AddLog(LOG_LEVEL_TEST, PSTR("sdcard_status.enable_logging CHANGED =%d"), sdcard_status.enable_logging);
    enable_logging_previous_state = sdcard_status.enable_logging;
    // If opened, close
    if(sdcard_status.isopened)
    {
      pCONT_sdcard->writer_settings.status = mSDCard::FILE_STATUS_CLOSING_ID;
      pCONT_sdcard->SubTask_Append_To_Open_File();
    }
    else
    {
      pCONT_sdcard->writer_settings.status = mSDCard::FILE_STATUS_OPENING_ID;
      pCONT_sdcard->SubTask_Append_To_Open_File();
    }
  }

  /**
   * Write to card
   * */
  // If more superframe data

  if(sdcard_status.isopened)
  {

    ConstructJSON_SDCardSuperFrame();
    AddLog(LOG_LEVEL_TEST, PSTR("sdcardframe=\"%s\""), BufferWriterI->GetPtr());

    pCONT_sdcard->SubTask_Append_To_Open_File(BufferWriterI->GetPtr(), BufferWriterI->GetLength());

  }

  // if(sdcard_status.enable_logging)
  // {
    // AddLog(LOG_LEVEL_TEST, PSTR("sdcard_status.enable_logging=%d"), sdcard_status.enable_logging);
  // }

}


/**
 * @brief basic test, that fully opens, appends and closed the file automatically on call
 * */
void mSerialPositionalLogger::SubTask_Debug_BasicFileWriteTest()
{

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

  //Test display messages
  //[1234567890123456]
  //[Op f123456] // Op/Cd for open and closed file, f# where # is GPS time for file name (although also using random in case gps is not working ie millis+gpsUTC)
  //[Val 123456] // Val Err for GPS fix, showing UTC time
  //[ ] //Also show lat/long so I know its working
  //[] packets received on serialRSS in thousands
#ifdef USE_MODULE_DISPLAYS_OLED_SSD1306
  pCONT_set->Settings.display.mode = EM_DISPLAY_MODE_LOG_STATIC_ID;
  char buffer[25];
  snprintf(buffer, sizeof(buffer), "%s %s","Op","fMillis123456");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 0);

  snprintf(buffer, sizeof(buffer), "%s",sdcard_status.isopened?"Open":"CLOSED");
  pCONT_iDisp->LogBuffer_AddRow(buffer, 1);

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
    
    // RSS data from ringbuffer1, the arrival of X bytes into this buffer should trigger a write to the SD card.
    // This means, no exact SF is needed, but every RSS within it will have a gps position attached to it.

    // JBI->Array_Start("SF");
    //   for(int i=0; i<10; i++) //or byte packing? this would save 1 comma per sample, worth it!
    //     JBI->Add(i);
    // JBI->Array_End();

    JBI->Add("SF", "AABBCCDDEEFF");

    // GPS data
    JBI->Level_Start("GPS");
      JBI->Add("la", pCONT_gps->gps_result_stored.latitude()); 
      JBI->Add("lg", pCONT_gps->gps_result_stored.longitude()); 
      JBI->Add("at", pCONT_gps->gps_result_stored.altitude_cm()); 
      JBI->Add("sd", pCONT_gps->gps_result_stored.speed());
    JBI->Level_End();

  return JsonBuilderI->End();
    
}




/*********************************************************************************************************************************************
******** MQTT Stuff **************************************************************************************************************************
**********************************************************************************************************************************************
********************************************************************************************************************************************/

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



////////////////////// END OF MQTT /////////////////////////

#endif
