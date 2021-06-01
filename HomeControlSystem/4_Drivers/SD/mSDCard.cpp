#include "mSDCard.h"

//  xdrv_22_sonoff_ifan.ino - sonoff iFan02 and iFan03 support for Tasmota

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
    init();
  }

  // Only continue in to tasker if module was configured properly
  //if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_LOOP:{

      EveryLoop_Handle_Appending_File_Method();

      // EveryLoop_RingBuffers_To_SDCard

        // if(debug.bytes_to_write == 0)
        // {
        //     debug.test_mode = STANDBY_ID; //stop trying
        // }else
        // if(debug.test_mode == WRITE_BYTES_ID)
        // {
        //     //Open card
        //     debug.write_time.opened.push_back(millis());
        //     char filename[50] = {0};
        //     uint8_t textmessage[50] = {0};
        //     sprintf(filename, "/debugfile%d.txt",1);// millis());

        //     // For loop write
        //     uint8_t j = 0;
        //     for(int i=0;i<debug.bytes_to_write;i++)
        //     {
        //         // sprintf(textmessage, "%d", j);
        //         textmessage[j] = i;
        //         if(j++>9){ j = 0; }
        //         // appendFile(SD, filename, textmessage);
        //         write_append_array(SD, filename, textmessage, 10);

        //     }

        //     // close card
        //     debug.write_time.closed.push_back(millis());


        //     debug.test_mode = STANDBY_ID;


        // }

    }
    break;
    case FUNC_EVERY_SECOND:  
{
        // Open SD card, show directory

//         if(close_decounter==0)
//         {
//     writer_settings.status = FILE_STATUS_CLOSE_ID;
//     SubTask_Append_To_Open_File();

//         }else{
//           close_decounter--;
//         }


// //   listDir(SD, "/", 0);
//     readFile(SD, "/debugfile1.txt");

  // listDir(SD, "/", 0);

      // SDCardSpeedDebug();
}
    
    break;
    case FUNC_EVERY_FIVE_SECOND:


      // char timectr[30];
      // snprintf(timectr, sizeof(timectr), "%s\n", pCONT_time->RtcTime.hhmmss_ctr);

      // appendFile_open_and_close(SD, "/time_test.txt", timectr);

      // readFile(SD, "/time_test.txt");

    break;

    case FUNC_EVERY_MINUTE:


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


// SPIClass SDSPI(HSPI);

// void setup(){
//   SDSPI.begin(sck, miso, mosi, -1);
//   SD.begin(ss, SDSPI);
// }
  Serial.print("Initializing SD card...");
// Chip select for SD card
#if defined(ESP32)
#define SD_CS 15 // 5 **
#elif defined (ARDUINO)
#define SD_CS BUILTIN_SDCARD
//#define SD_CS 15
#endif
  spiSD.begin(14, 12, 13, 15); //SCK,MISO,MOSI,SS //HSPI1
// if (!SD.begin( SD_CS, spiSD ))
// {
// Serial.println("Card Mount Failed");
// return;
// }

  if(!SD.begin( SD_CS,spiSD )){
    Serial.println("Card Mount Failed");
    sdcard_status.init_error_on_boot = true;
    delay(3000);
    return;
  }
  uint8_t cardType = SD.cardType();

  if(cardType == CARD_NONE){
      Serial.println("No SD card attached");
    sdcard_status.init_error_on_boot = true;
      return;
  }

  Serial.print("SD Card Type: ");
  if(cardType == CARD_MMC){
      Serial.println("MMC");
  } else if(cardType == CARD_SD){
      Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
      Serial.println("SDHC");
  } else {
      Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);

  listDir(SD, "/", 0);
  // createDir(SD, "/mydir");
  // listDir(SD, "/", 0);
  // removeDir(SD, "/mydir");
  // listDir(SD, "/", 2);
  // writeFile(SD, "/hello.txt", "Hello ");
  // appendFile(SD, "/hello.txt", "World!\n");
  // readFile(SD, "/hello.txt");
  // deleteFile(SD, "/foo.txt");
  // renameFile(SD, "/hello.txt", "/foo.txt");
  // readFile(SD, "/foo.txt");
  // testFileIO(SD, "/test.txt");
  Serial.printf("Total space: %lluMB\n", SD.totalBytes() / (1024 * 1024));
  Serial.printf("Used space: %lluMB\n", SD.usedBytes() / (1024 * 1024));

  // Success, no failure
  sdcard_status.init_error_on_boot = false;

}


void mSDCard::Pre_Init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

//chip select pin needs setting

}


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




#endif
