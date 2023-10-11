/**
 * @file mFileSystem.cpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-10-07
 * 
 * There are two types of filesystems:
 * 
 * 1) Internal flash filesystem
 * 2) SD Card filesystem
 * https://arduino-esp8266.readthedocs.io/en/latest/filesystem.html
 * @copyright Copyright (c) 2023
 * 
 * Step 1: build now and be able to view files on the SD card in the webui editor
 * 
 */

#include "mFileSystem.h"

#ifdef USE_MODULE_DRIVERS_FILESYSTEM

const char* mFileSystem::PM_MODULE_DRIVERS_FILESYSTEM_CTR          = D_MODULE_DRIVERS_FILESYSTEM_CTR;
const char* mFileSystem::PM_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR = D_MODULE_DRIVERS_FILESYSTEM_FRIENDLY_CTR;

int8_t mFileSystem::Tasker(uint8_t function, JsonParserObject obj)
{

  /************
   * INIT SECTION * 
  *******************/
  if(function == FUNC_PRE_INIT){
    Pre_Init();
  }else
  if(function == FUNC_INIT){
    init();
  }

  if(!settings.fEnableModule){ return FUNCTION_RESULT_MODULE_DISABLED_ID; }

  switch(function){
    /************
     * PERIODIC SECTION * 
    *******************/
    case FUNC_EVERY_SECOND:  
    break;
     case FUNC_LOOP:
    //   UfsExecuteCommandFileLoop();
    break;
    case FUNC_EVERY_FIVE_SECOND:
    // FileWrite_Test();    
    break;
    case FUNC_EVERY_MINUTE:
      // JsonFile_Save__Stored_Module();
      // JsonFile_Save__Stored_Secure();
    break;  
    case FUNC_EVERY_FIVE_MINUTE:
      JsonFile_Save__Stored_Module();
      JsonFile_Save__Stored_Secure();
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

    // case FUNC_MQTT_INIT:
    //   if (!TasmotaGlobal.no_autoexec) {
    //     UfsExecuteCommandFile(TASM_FILE_AUTOEXEC);
    //   }
    //   break;
    // case FUNC_COMMAND:
    //   result = DecodeCommand(kUFSCommands, kUFSCommand);
    //   break;
    #ifdef USE_WEBSERVER
    case FUNC_WEB_ADD_MANAGEMENT_BUTTON:
      if (ufs_type) {
      WSContentSend_PD(UFS_WEB_DIR, PSTR(D_MANAGE_FILE_SYSTEM));
      }
      break;
      case FUNC_WEB_ADD_HANDLER:
      //      Webserver->on(F("/ufsd"), UfsDirectory);
      //      Webserver->on(F("/ufsu"), HTTP_GET, UfsDirectory);
      //      Webserver->on(F("/ufsu"), HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
      Webserver->on("/ufsd", UfsDirectory);
      Webserver->on("/ufsu", HTTP_GET, UfsDirectory);
      Webserver->on("/ufsu", HTTP_POST,[](){Webserver->sendHeader(F("Location"),F("/ufsu"));Webserver->send(303);}, HandleUploadLoop);
      break;
    #endif // USE_WEBSERVER



    }
  

} // END Tasker


void mFileSystem::FileWrite_Test()
{

  File f; // don't export to other cpp files

  size_t pos = 0;

  // listDir(FILE_SYSTEM, "/", 0);

  const char* file = "/test.txt";
  
  f = FILE_SYSTEM.open(file, "r+");
  
  if (!f && !FILE_SYSTEM.exists(file))
  {
    f = FILE_SYSTEM.open(file, "w+");
    ALOG_INF(PSTR("File did not exist, creating a new one"));
  }
  
  if (!f) {
    Serial.println("Failed to open!");
    return;
  }


  JBI->Start();

  JBI->Add(PSTR("tEST"), millis());


  JBI->End();

  f.print(JBI->GetBufferPtr());
  
  f.close();
    ALOG_INF(PSTR("Writing file \"%s\""), JBI->GetBufferPtr());


}


void mFileSystem::init(void)
{
  
  // analogWriteFreq(25000);
  // if (SONOFF_IFAN03 == my_module_type) {
  //   SetSerial(9600, TS_SERIAL_8N1);
  // }
  // return false;  // Continue init chain

  
  // if(!SD_MMC.begin()){
  //     Serial.println("Card Mount Failed");
  //     return;
  // }
  // uint8_t cardType = SD_MMC.cardType();

  // if(cardType == CARD_NONE){
  //     Serial.println("No SD_MMC card attached");
  //     return;
  // }

  // Serial.print("SD_MMC Card Type: ");
  // if(cardType == CARD_MMC){
  //     Serial.println("MMC");
  // } else if(cardType == CARD_SD){
  //     Serial.println("SDSC");
  // } else if(cardType == CARD_SDHC){
  //     Serial.println("SDHC");
  // } else {
  //     Serial.println("UNKNOWN");
  // }

  // uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  // Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  // listDir(SD_MMC, "/", 0);
  // createDir(SD_MMC, "/mydir");
  // listDir(SD_MMC, "/", 0);
  // //removeDir(SD_MMC, "/mydir");
  // listDir(SD_MMC, "/", 2);
  // writeFile(SD_MMC, "/hello.txt", "Hello ");
  // appendFile(SD_MMC, "/hello.txt", "World!\n");
  // readFile(SD_MMC, "/hello.txt");
  // //deleteFile(SD_MMC, "/foo.txt");
  // renameFile(SD_MMC, "/hello.txt", "/foo.txt");
  // readFile(SD_MMC, "/foo.txt");
  // testFileIO(SD_MMC, "/test.txt");
  // Serial.printf("Total space: %lluMB\n", SD_MMC.totalBytes() / (1024 * 1024));
  // Serial.printf("Used space: %lluMB\n", SD_MMC.usedBytes() / (1024 * 1024));

  settings.fEnableModule = true;

}


// Called from tasmota.ino at restart. This inits flash file only
void mFileSystem::UfsInit(void) 
{

  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM "mFileSystem::UfsInit"));
  
  // #ifdef ENABLE_DEVFEATURE_LIGHTING__PRESETS
  bool fsinit = false;
  DEBUG_PRINTLN(F("Mount FS"));
  #ifdef ARDUINO_ARCH_ESP32
    fsinit = FILE_SYSTEM.begin(true);
  #else
    fsinit = FILE_SYSTEM.begin();
  #endif
  if (!fsinit) {
    DEBUG_PRINTLN(F("FS failed!"));
    // errorFlag = ERR_FS_BEGIN;
  }else{
    DEBUG_PRINTLN(F("FS mounted."));
  }
  // #endif // ENABLE_DEVFEATURE_LIGHTING__PRESETS

  // #ifdef ENABLE_DEVFEATURE__SETTINGS_STORAGE

  UfsData.run_file_pos = -1;
  UfsInitOnce();
  if (ufs_type) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM "FlashFS mounted with %d kB free"), UfsInfo(1, 0));
  }

  // #endif // ENABLE_DEVFEATURE__SETTINGS_STORAGE

}


// // Init flash file system
void mFileSystem::UfsInitOnce(void) {
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM "mFileSystem::UfsInitOnce"));
  ufs_type = 0;
  ffsp = 0;
  ufs_dir = 0;

#ifdef ESP8266
  ffsp = &LittleFS;
  if (!LittleFS.begin()) {
  	AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM "!LittleFS.begin()"));
    ffsp = 0;
    return;
  }
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM  "LittleFS.begin()"));
#endif  // ESP8266

#ifdef ESP32
#ifdef ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
  // try lfs first
  ffsp = &FILE_SYSTEM;
 if (!FILE_SYSTEM.begin(true, "") && !FILE_SYSTEM.begin(true, "", 5, "fs_1")) {         // force empty mount point to make it the fallback FS
    // ffat is second
    ffsp = &FFat;
   if (!FFat.begin(true, "")) {
      ffsp = nullptr;
      return;
    }
    ffs_type = UFS_TFAT;
    ufs_type = ffs_type;
    ufsp = ffsp;
    dfsp = ffsp;
    return;
  }
#endif // ENABLE_DEVFEATURE_STORAGE_IS_LITTLEFS
#endif // ESP32
  ffs_type = UFS_TLFS;
  ufs_type = ffs_type;
  ufsp = ffsp;
  dfsp = ffsp;
}



uint32_t mFileSystem::UfsInfo(uint32_t sel, uint32_t type) {
  uint64_t result = 0;
  FS *ifsp = ufsp;
  uint8_t itype = ufs_type;
  if (type) {
    ifsp = ffsp;
    itype = ffs_type;
  }

#ifdef ESP8266
  FSInfo64 fsinfo;
#endif  // ESP8266

  switch (itype) {
    case UFS_TSDC:
#ifdef USE_SDCARD
#ifdef ESP8266
      ifsp->info64(fsinfo);
      if (sel == 0) {
        result = fsinfo.totalBytes;
      } else {
        result = (fsinfo.totalBytes - fsinfo.usedBytes);
      }
#endif  // ESP8266
#ifdef ESP32
      if (sel == 0) {
        result = SD.totalBytes();
      } else {
        result = (SD.totalBytes() - SD.usedBytes());
      }
#endif  // ESP32
#endif  // USE_SDCARD
      break;

    case UFS_TLFS:
#ifdef ESP8266
      ifsp->info64(fsinfo);
      if (sel == 0) {
        result = fsinfo.totalBytes;
      } else {
        result = (fsinfo.totalBytes - fsinfo.usedBytes);
      }
#endif  // ESP8266
#ifdef ESP32
      if (sel == 0) {
        result = LITTLEFS.totalBytes();
      } else {
        result = LITTLEFS.totalBytes() - LITTLEFS.usedBytes();
      }
#endif  // ESP32
      break;

    case UFS_TFAT:
#ifdef ESP32
      if (sel == 0) {
        result = FFat.totalBytes();
      } else {
        result = FFat.freeBytes();
      }
#endif  // ESP32
      break;

  }
  return result / 1024;
}



bool mFileSystem::TfsLoadFile(const char *fname, uint8_t *buf, uint32_t len) 
{
  if (!ffs_type) { return false; }

  File file = ffsp->open(fname, "r");
  if (!file) {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM "File '%s' not found"), fname +1);  // Skip leading slash
    return false;
  }

  size_t flen = file.size();
  if (len > flen) { len = flen; }           // Adjust requested length to smaller file length
  file.read(buf, len);
  file.close();
  return true;
}


bool mFileSystem::TfsSaveFile(const char *fname, const uint8_t *buf, uint32_t len) 
{
  
  if (!ffs_type) { return false; }
  
  #ifdef USE_WEBCAM
    WcInterrupt(0);  // Stop stream if active to fix TG1WDT_SYS_RESET
  #endif
  
  AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM "ffsp %s"), fname);

  File file = ffsp->open(fname, "w");
  
  if (!file) 
  {
    AddLog(LOG_LEVEL_INFO, PSTR(D_LOG_FILESYSTEM "Save failed"));
    return false;
  } 
    
  // This will timeout on ESP32-webcam, but now solved with WcInterrupt(0) in support_esp.ino
  file.write(buf, len);     
  file.close();
  
  #ifdef USE_WEBCAM
    WcInterrupt(1);
  #endif
  
  return true;

}


void mFileSystem::Pre_Init(){

//change this into the fan module
//   if(pCONT_pins->PinUsed(GPIO_FAN_SDCARD1_ID)) {  // not set when 255
//     pin = pCONT_pins->GetPin(GPIO_FAN_SDCARD1_ID);
//     pinMode(pin, OUTPUT);
//     settings.fEnableModule = true;
//   }

    #ifdef USE_SDCARD
    UfsCheckSDCardInit();
    #endif // USE_SDCARD
}


uint8_t mFileSystem::ConstructJSON_Settings(uint8_t json_level, bool json_appending)
{
  
  JsonBuilderI->Start();
    JsonBuilderI->Add_P("test",0);  
  JsonBuilderI->End();

}


void mFileSystem::MQTTHandler_Init(){

  struct handler<mFileSystem>* ptr;

  ptr = &mqtthandler_settings_teleperiod;
  ptr->tSavedLastSent = millis();
  ptr->flags.PeriodicEnabled = true;
  ptr->flags.SendNow = true;
  ptr->tRateSecs = 1;//pCONT_set->Settings.sensors.configperiod_secs; 
  ptr->topic_type = MQTT_TOPIC_TYPE_TELEPERIOD_ID;
  ptr->json_level = JSON_LEVEL_DETAILED;
  ptr->postfix_topic = PM_MQTT_HANDLER_POSTFIX_TOPIC_SETTINGS_CTR;
  ptr->ConstructJSON_function = &mFileSystem::ConstructJSON_Settings;
  mqtthandler_list.push_back(ptr);

  
} //end "MQTTHandler_Init"

/**
 * @brief Set flag for all mqtthandlers to send
 * */
void mFileSystem::MQTTHandler_Set_RefreshAll()
{
  for(auto& handle:mqtthandler_list){
    handle->flags.SendNow = true;
  }
}

/**
 * @brief Update 'tRateSecs' with shared teleperiod
 * */
void mFileSystem::MQTTHandler_Set_DefaultPeriodRate()
{
  for(auto& handle:mqtthandler_list){
    if(handle->topic_type == MQTT_TOPIC_TYPE_TELEPERIOD_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.teleperiod_secs;
    if(handle->topic_type == MQTT_TOPIC_TYPE_IFCHANGED_ID)
      handle->tRateSecs = pCONT_set->Settings.sensors.ifchanged_secs;
  }
}

/**
 * @brief MQTTHandler_Sender
 * */
void mFileSystem::MQTTHandler_Sender(uint8_t id)
{    
  for(auto& handle:mqtthandler_list){
    pCONT_mqtt->MQTTHandler_Command(*this, EM_MODULE_DRIVERS_FILESYSTEM_ID, handle, id);
  }
}



void mFileSystem::parse_JSONCommand(JsonParserObject obj)
{

  JsonParserToken jtok = 0; 
  int16_t tmp_id = 0;
  char buffer[50];


  if(jtok = obj["Debug"].getObject()["TriggerJSONSave"]){

    // CommandSet_SerialPrint_FileNames(jtok.getStr());
    // listDir(FILE_SYSTEM, "/", 0);

    JsonFile_Save__Stored_Module();
    
  }
  
  if(jtok = obj["Debug"].getObject()["TriggerJSONLoad"]){

    // CommandSet_SerialPrint_FileNames(jtok.getStr());
    // listDir(FILE_SYSTEM, "/", 0);

    JsonFile_Load__Stored_Module();
    
  }
  

}




// void (* const kUFSCommand[])(void) PROGMEM = {
//   &UFSInfo, &UFSType, &UFSSize, &UFSFree, &UFSDelete, &UFSRename, &UFSRun
// #ifdef UFILESYS_STATIC_SERVING
//   ,&UFSServe
// #endif  
//   };

// // return true if SDC
// bool isSDC(void) {
// #ifndef SDC_HIDE_INVISIBLES
//   return false;
// #else
//   if (((uint32_t)ufsp != (uint32_t)ffsp) && ((uint32_t)ffsp == (uint32_t)dfsp)) return false;
//   if (((uint32_t)ufsp == (uint32_t)ffsp) && (ufs_type != UFS_TSDC)) return false;
//   return true;
// #endif
// }















#endif // USE_MODULE_DRIVERS_FILESYSTEM
